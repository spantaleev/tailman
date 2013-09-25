#include "addtab.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>

AddTab::AddTab(QWidget *parent) :
    QDialog(parent) {

    tabController = qobject_cast<TabController*>(parent);
    m_tail_index = 0;

    createUI();
}

void AddTab::createUI() {

    setFixedSize(350,450);
    setWindowTitle("Add tail");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QListWidget *tailList = new QListWidget(this);
    tailList->setSortingEnabled(true);

    for (int i=0; i<tabController->tailList.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(tabController->tailList.at(i)->alias);
        item->setData(Qt::UserRole, QVariant(i));
        tailList->addItem(item);
    }

    QLabel *labelAddTail = new QLabel(tr("<strong>Add tail:</strong>"), this);
    labelAddTail->setBuddy(tailList);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonAdd = buttonBox->addButton(tr("Add tail"), QDialogButtonBox::AcceptRole);
    buttonAdd->setEnabled(false);
    buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addWidget(labelAddTail);
    mainLayout->addWidget(tailList);
    mainLayout->addWidget(buttonBox);

    connect(tailList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
        this, SLOT(slotItemChanged(QListWidgetItem*, QListWidgetItem*)));

    setLayout(mainLayout);
}

void AddTab::slotItemChanged(QListWidgetItem *item, QListWidgetItem *itemOld) {

    buttonAdd->setEnabled(true);
    m_tail_index = item->data(Qt::UserRole).toInt();
}

void AddTab::accept() {

    QDialog::accept();
    this->hide();
}

void AddTab::reject() {

    QDialog::reject();
    this->hide();
}
