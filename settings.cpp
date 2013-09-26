#include "settings.h"
#include "mainwindow.h"
#include <QDialogButtonBox>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDebug>
#include <QMessageBox>

Settings::Settings(QWidget *parent) :
    QDialog(parent) {

    mainwindow = qobject_cast<MainWindow*>(parent);
    m_activeListId = -1;
    m_statusSaving = false;
    createUI();
}

void Settings::createUI() {

    setWindowTitle("Settings");

    tailList = new QListWidget(this);
    tailList->setMaximumWidth(200);
    tailList->setSortingEnabled(true);
    loadTails();

    connect(tailList, SIGNAL(currentRowChanged(int)),
            this, SLOT(slotItemChanged(int)));
    connect(tailList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slotItemClicked(QListWidgetItem*)));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonClose = buttonBox->addButton(tr("Close"), QDialogButtonBox::AcceptRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    QPushButton *buttonCreate = new QPushButton("Create new tail", this);
    connect(buttonCreate, SIGNAL(clicked()), this, SLOT(slotCreateNew()));

    QLabel *labelAlias = new QLabel("Alias:", this);
    editAlias = new QLineEdit(this);

    QLabel *labelAutostart = new QLabel("Autostart:", this);
    editAutostart = new QCheckBox(this);

    QLabel *labelHost = new QLabel("Host:", this);
    editHost = new QLineEdit(this);

    QLabel *labelPort = new QLabel("Port:", this);
    editPort = new QLineEdit(this);

    QLabel *labelAuthentication = new QLabel("Authentication:", this);
    editAuthentication = new QComboBox(this);
    editAuthentication->addItem("Password", QString("password"));
    editAuthentication->addItem("Public key", QString("pubkey"));

    QLabel *labelUsername = new QLabel("Username:", this);
    editUsername = new QLineEdit(this);

    QLabel *labelPath = new QLabel("Tail path:", this);
    editPath = new QLineEdit(this);
    connect(editPath, SIGNAL(textEdited(QString)), this, SLOT(slotUpdateCommand(QString)));

    QLabel *labelEditCommand = new QLabel("Edit command:", this);
    editEditCommand = new QCheckBox(this);
    connect(editEditCommand, SIGNAL(toggled(bool)), this, SLOT(slotEditCommandToggled(bool)));

    QLabel *labelCommand = new QLabel("Command:", this);
    editCommand = new QLineEdit(this);
    editCommand->setReadOnly(true);
    editCommand->setStyleSheet("background-color:#DBDBDB;");

    QDialogButtonBox *buttonBoxForm = new QDialogButtonBox(this);
    buttonSave = buttonBoxForm->addButton(tr("Save"), QDialogButtonBox::ApplyRole);
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveSettings()));
    buttonDelete = buttonBoxForm->addButton(tr("Delete"), QDialogButtonBox::DestructiveRole);
    buttonDelete->setEnabled(false);
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(slotDeleteTail()));
    buttonDelete->setStyleSheet("color:red;");

    QGridLayout *formLayout = new QGridLayout;
    formLayout->addWidget(labelAlias, 0, 0);
    formLayout->addWidget(editAlias, 0, 1);
    formLayout->addWidget(labelAutostart, 1, 0);
    formLayout->addWidget(editAutostart, 1, 1);
    formLayout->addWidget(labelHost, 2, 0);
    formLayout->addWidget(editHost, 2, 1);
    formLayout->addWidget(labelPort, 3, 0);
    formLayout->addWidget(editPort, 3, 1);
    formLayout->addWidget(labelAuthentication, 4, 0);
    formLayout->addWidget(editAuthentication, 4, 1);
    formLayout->addWidget(labelUsername, 5, 0);
    formLayout->addWidget(editUsername, 5, 1);
    formLayout->addWidget(labelPath, 6, 0);
    formLayout->addWidget(editPath, 6, 1);
    formLayout->addWidget(labelEditCommand, 7, 0);
    formLayout->addWidget(editEditCommand, 7, 1);
    formLayout->addWidget(labelCommand, 8, 0);
    formLayout->addWidget(editCommand, 8, 1);
    formLayout->addWidget(buttonBoxForm, 9, 1);
    formLayout->setSpacing(10);

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->addWidget(tailList, 0, 0, 1, 1);
    mainGridLayout->addLayout(formLayout, 0, 1, 1, 1);
    mainGridLayout->addWidget(buttonBox, 1, 1, 1, 1);
    mainGridLayout->addWidget(buttonCreate, 1, 0, 1, 1);
    mainGridLayout->setColumnStretch(1, 2);
    mainGridLayout->setSpacing(20);


    setLayout(mainGridLayout);
    setMinimumSize(640, 480);
}

void Settings::loadSettings(s_tail *_tail) {

    buttonDelete->setEnabled(true);
    editAlias->setText(_tail->alias);

    if (_tail->autostart) {
        editAutostart->setCheckState(Qt::Checked);
    } else {
        editAutostart->setCheckState(Qt::Unchecked);
    }

    editHost->setText(_tail->host);
    editPort->setText(QString::number(_tail->port));

    if (_tail->auth_method.compare("pubkey") == 0) {
        int index = editAuthentication->findData(QString("pubkey"));
        if (index != -1) {
            editAuthentication->setCurrentIndex(index);
        }
    } else if (_tail->auth_method.compare("password") == 0) {
        int index = editAuthentication->findData(QString("password"));
        if (index != -1) {
            editAuthentication->setCurrentIndex(index);
        }
    }

    editUsername->setText(_tail->username);
    editPath->setText(_tail->tail_path);
    editCommand->setText(_tail->command);
}

void Settings::saveTails() {

    // Save to config
    mainwindow->m_settings.remove("tails");
    mainwindow->m_settings.beginWriteArray("tails");

    for (int i = 0; i < mainwindow->tabController->tailList.size(); ++i) {
        mainwindow->m_settings.setArrayIndex(i);
        mainwindow->m_settings.setValue("autostart", mainwindow->tabController->tailList.at(i)->autostart);
        mainwindow->m_settings.setValue("alias", mainwindow->tabController->tailList.at(i)->alias);
        mainwindow->m_settings.setValue("host", mainwindow->tabController->tailList.at(i)->host);
        mainwindow->m_settings.setValue("port", mainwindow->tabController->tailList.at(i)->port);
        mainwindow->m_settings.setValue("auth_method", mainwindow->tabController->tailList.at(i)->auth_method);
        mainwindow->m_settings.setValue("username", mainwindow->tabController->tailList.at(i)->username);
        mainwindow->m_settings.setValue("tail_path", mainwindow->tabController->tailList.at(i)->tail_path);
        mainwindow->m_settings.setValue("command", mainwindow->tabController->tailList.at(i)->command);
    }

    mainwindow->m_settings.endArray();
}

void Settings::loadTails() {

    tailList->clear();

    for (int i=0; i<mainwindow->tabController->tailList.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem(mainwindow->tabController->tailList.at(i)->alias, tailList);
        item->setData(Qt::UserRole, QVariant(i));
        tailList->addItem(item);
    }
}

void Settings::slotItemChanged(int currentRow) {

    if (!m_statusSaving && m_activeListId != currentRow) {
        m_activeListId = currentRow;
        slotEditCommandToggled(false);

        loadSettings(mainwindow->tabController->tailList.at(
            tailList->item(m_activeListId)->data(Qt::UserRole).toInt()));
    }
}

void Settings::slotItemClicked(QListWidgetItem *item)
{
    slotItemChanged(tailList->row(item));
}

void Settings::slotSaveSettings() {

    m_statusSaving = true;

    // Check for input errors
    if (editAlias->text().length() == 0) {
        QMessageBox::critical(this, "Error",
            "No alias entered", QMessageBox::Ok);
        m_statusSaving = false;
        return;
    }
    if (editCommand->text().length() == 0) {
        QMessageBox::critical(this, "Error",
            "No command entered", QMessageBox::Ok);
        m_statusSaving = false;
        return;
    }
    if (editHost->text().length() == 0) {
        QMessageBox::critical(this, "Error",
            "No host entered", QMessageBox::Ok);
        m_statusSaving = false;
        return;
    }
    if (editPath->text().length() == 0) {
        QMessageBox::critical(this, "Error",
            "No tail path entered", QMessageBox::Ok);
        m_statusSaving = false;
        return;
    }
    if (editPort->text().length() == 0) {
        QMessageBox::critical(this, "Error",
            "No port entered", QMessageBox::Ok);
        m_statusSaving = false;
        return;
    }
    if (editUsername->text().length() == 0) {
        QMessageBox::critical(this, "Error",
            "No username entered", QMessageBox::Ok);
        m_statusSaving = false;
        return;
    }

    s_tail *tail = (m_activeListId >= 0) ?
            mainwindow->tabController->tailList.at(
                tailList->item(m_activeListId)->data(Qt::UserRole).toInt())
            : new s_tail();

    tail->autostart = editAutostart->isChecked();
    tail->alias = editAlias->text();
    tail->host = editHost->text();
    tail->port = editPort->text().toInt();
    tail->auth_method = editAuthentication->itemData(editAuthentication->currentIndex()).toString();
    tail->username = editUsername->text();
    tail->tail_path = editPath->text();
    tail->command = editCommand->text();

    if (m_activeListId < 0) { // New
        mainwindow->tabController->tailList.append(tail);
    }

    QMessageBox::information(this, "Information",
        "Tail saved!", QMessageBox::Ok);

    saveTails();
    loadTails();
    m_statusSaving = false;

    // Clear
    slotCreateNew();
}

void Settings::slotDeleteTail() {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete",
                "Are you sure that you would like to delete this tail?",
                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_statusSaving = true;
        mainwindow->tabController->tailList.removeAt(
            tailList->item(m_activeListId)->data(Qt::UserRole).toInt());
        saveTails();
        loadTails();
        slotCreateNew();
        m_statusSaving = false;
    }
}

void Settings::slotCreateNew() {

    if (m_activeListId >= 0) {
        tailList->item(m_activeListId)->setSelected(false);
    }

    slotEditCommandToggled(false);
    buttonDelete->setEnabled(false);
    m_activeListId = -1;

    editAlias->setText("");
    editAutostart->setCheckState(Qt::Unchecked);
    editHost->setText("");
    editPort->setText("");
    editAuthentication->setCurrentIndex(0);
    editUsername->setText("");
    editPath->setText("");
    editCommand->setText("");
}

void Settings::slotUpdateCommand(QString path) {

    if (path.length() > 0) {
        editCommand->setText(QString(DEFAULT_CMD_TAIL).append(path));
    } else {
        editCommand->setText("");
    }
}

void Settings::slotEditCommandToggled(bool checked) {

    if (checked) {
        editCommand->setReadOnly(false);
        editCommand->setStyleSheet("");
        editEditCommand->setChecked(true);
    } else {
        editCommand->setReadOnly(true);
        editCommand->setStyleSheet("background-color:#DBDBDB;");
        editEditCommand->setChecked(false);
    }
}

void Settings::accept() {

    QDialog::accept();
    this->hide();
}
