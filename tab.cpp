#include "tab.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>
#include <QScrollBar>
#include <QTextCursor>
#include <QTabBar>
#include <QMessageBox>
#include <QInputDialog>

Tab::Tab(QWidget *parent, s_tail *_tail) : QWidget(parent) {

    tail = _tail;
    tabController = qobject_cast<TabController*>(parent);
    m_new_lines = 0;
    m_connected = false;
    m_closing_tab = false;
    m_hostkey_verified = false;
    m_manual_password = "";

    createUI();

    sshWrapper = new SshWrapper(this, _tail);
    sshWrapper->start();
}

Tab::~Tab() {

}

void Tab::createUI() {

    QVBoxLayout *mainLayout = new QVBoxLayout();

    labelStatus = new QLabel(this);
    labelStatus->setText(QString("[<span style=\"color:grey;\">Connecting</span>]" \
        " <span style=\"color:#121212;\">%1 (%2)</span>").arg(tail->alias, tail->command));
    mainLayout->addWidget(labelStatus);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    textEdit->setStyleSheet(QString(
        "QTextEdit { background-color: %1; color: %2; border:0; }").arg(
            tabController->m_tail_bg_color, tabController->m_tail_text_color));

    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(textEdit);


    setLayout(mainLayout);
}

void Tab::slotAddContent(QString content) {

    // Dont update tab text if we are on the current tab
    if (tabController->currentIndex() != m_tabIndex) {
        int new_lines = content.count("\n", Qt::CaseInsensitive);
        m_new_lines = new_lines + m_new_lines;

        tabController->setTabText(m_tabIndex,
            QString("[%1] %2").arg(QString::number(m_new_lines), tail->alias));
        tabController->setTabColor(m_tabIndex, QColor(173,0,0));
        tabController->setTabToolTip(m_tabIndex, QString("[%1] %2 (%3)").arg(
            QString::number(m_new_lines), tail->alias, tail->command));
    }

    textEdit->moveCursor(QTextCursor::End);

    textEdit->insertPlainText(content);
    QScrollBar *sb = textEdit->verticalScrollBar();
    sb->setValue(sb->maximum());

    textEdit->moveCursor(QTextCursor::End);
}

void Tab::slotTailFailed(QString message) {

    labelStatus->setText(QString("[<span style=\"color:red;\">Offline</span>]" \
        " <span style=\"color:#121212;\">%1 (%2)</span>").arg(tail->alias, tail->command));
    slotAddContent(message.append("\n"));
}

void Tab::slotCleanTail() {

    textEdit->clear();
}

void Tab::slotVerifyHostKey(QString key) {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
            0,
            QString("The server is unknown. Do you trust the host key?"),
            QString("Public key hash: %1").arg(key),
            QMessageBox::Yes|QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        m_hostkey_verified = true;
    } else {
        m_hostkey_verified = false;
    }
}

void Tab::slotEnterPassword() {

    bool ok;
    m_manual_password = QInputDialog::getText(this,
        tail->alias, tr("Enter password:"), QLineEdit::Password, "", &ok);
}
