#ifndef ADDTAB_H
#define ADDTAB_H

#include <QDialog>
#include <QListWidgetItem>
#include "tabcontroller.h"

class AddTab : public QDialog {

    Q_OBJECT
public:
    explicit AddTab(QWidget *parent = 0);

    TabController *tabController;
    int m_tail_index;

private:
    void createUI();

    QPushButton *buttonAdd;

signals:

public slots:
    void accept();
    void reject();

private slots:
    void slotItemChanged(QListWidgetItem*, QListWidgetItem*);

};

#endif // ADDTAB_H
