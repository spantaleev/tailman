#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include "tabcontroller.h"

class MainWindow : public QMainWindow {

    Q_OBJECT
public:
    MainWindow();
    TabController *tabController;

    QSettings m_settings;

private:
    void createUI();
    void saveWindowSettings();
    void restoreWindowSettings();
    void closeEvent(QCloseEvent* event);

public slots:
    void slotOpenSettings();
    void slotOpenAbout();

};

#endif // MAINWINDOW_H
