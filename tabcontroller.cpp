#include "tabcontroller.h"
#include "tab.h"
#include "addtab.h"
#include <QTabBar>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QThread>

TabController::TabController(QWidget *parent) :
    QTabWidget(parent) {

    m_tail_bg_color = "";
    m_tail_text_color = "";

    setTabsClosable(true);

    // Load all tails from config
    loadTails();

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabClose(int)));
}

void TabController::setTabColor(int index, QColor color) {

    tabBar()->setTabTextColor(index, color);
}

void TabController::slotTabChanged(int index) {

    if (index >= 0) {
        Tab* tab = qobject_cast<Tab*>(widget(index));
        tab->m_new_lines = 0;
        setTabText(index, QString("[%1] %2").arg(QString::number(0), tab->tail->alias));
        setTabColor(index, Qt::black);
        setTabToolTip(index, QString("[%1] %2 (%3)").arg(QString::number(0), tab->tail->alias, tab->tail->command));
    }
}

void TabController::slotNewTab() {

    AddTab *addNewTab = new AddTab(this);

    if (addNewTab->exec()) {
        int i;
        Tab *tab = new Tab(this, tailList.at(addNewTab->m_tail_index));
        i = addTab(tab,
                   QString("[0] %1").arg(tailList.at(addNewTab->m_tail_index)->alias));
        tab->m_tabIndex = i;

        setCurrentIndex(i);
    }

    delete addNewTab;
}

void TabController::loadTails() {

    tailList.clear();
    int size = m_settings.beginReadArray("tails");

    for (int i = 0; i < size; ++i) {
        m_settings.setArrayIndex(i);

        s_tail *tail = new s_tail();
        tail->autostart = m_settings.value("autostart").toBool();
        tail->alias = m_settings.value("alias").toString();
        tail->host = m_settings.value("host").toString();
        tail->port = m_settings.value("port").toInt();
        tail->auth_method = m_settings.value("auth_method").toString();
        tail->username = m_settings.value("username").toString();
        tail->tail_path = m_settings.value("tail_path").toString();
        tail->command = m_settings.value("command").toString();

        tailList.append(tail);
    }

    m_settings.endArray();
}

void TabController::openAutostartTails() {

    // Open all the tails that should be autostarted
    foreach (s_tail *tail, tailList) {
        if (tail->autostart) {
            int i;
            Tab *tab = new Tab(this, tail);
            i = addTab(tab,
                       QString("[0] %1").arg(tail->alias));

            tab = qobject_cast<Tab*>(widget(i));
            tab->m_tabIndex = i;
        }
    }
}

void TabController::slotTabClose(int index) {

    Tab* tab = qobject_cast<Tab*>(widget(index));
    tab->m_closing_tab = true;

    if (tab->m_connected == true) {
        tab->sshWrapper->disconnectSsh();
    }

    tab->sshWrapper->exit(0);

    // Wait for thread fo finish
    while(tab->sshWrapper->isRunning()) {
        usleep(1000);
    }

    removeTab(index);
    delete tab;
}


