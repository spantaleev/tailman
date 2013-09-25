#include "mainwindow.h"
#include "tabcontroller.h"
#include "tab.h"
#include "addtab.h"
#include "about.h"
#include "settings.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolBar>
#include <QIcon>
#include <QAction>

MainWindow::MainWindow() {

    createUI();
}

void MainWindow::createUI() {

    restoreWindowSettings();
    setWindowTitle(tr("TailMan"));

    QToolBar *toolBar = addToolBar("toolbar");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(24, 24));
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    tabController = new TabController(this);
    tabController->m_tail_bg_color = m_settings.value("color_bg", "#121212").toString();
    tabController->m_tail_text_color = m_settings.value("color_text", "#00BD0D").toString();
    tabController->openAutostartTails();

    QAction *actionOpenSettings = toolBar->addAction(QIcon(":images/network.png"), "Manage tails");
    connect(actionOpenSettings, SIGNAL(triggered()), this, SLOT(slotOpenSettings()));

    toolBar->addSeparator();

    QAction *actionAddTab = toolBar->addAction(QIcon(":images/plus.png"), "Open tail");
    connect(actionAddTab, SIGNAL(triggered()), tabController, SLOT(slotNewTab()));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    QAction *actionHelp = toolBar->addAction(QIcon(":images/help.png"), "About");
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slotOpenAbout()));

    QWidget *centralWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabController);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::saveWindowSettings() {

    m_settings.setValue("pos", pos());
    m_settings.setValue("size", size());
}

void MainWindow::restoreWindowSettings() {

    QPoint pos = m_settings.value("pos", QPoint(0, 0)).toPoint();
    QSize size = m_settings.value("size", QSize(640, 480)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::closeEvent(QCloseEvent *event) {

    saveWindowSettings();
    event->accept();
}

void MainWindow::slotOpenSettings() {

    Settings *settings = new Settings(this);
    settings->exec();

    delete settings;
}

void MainWindow::slotOpenAbout() {

    About *about = new About(this);
    about->exec();

    delete about;
}
