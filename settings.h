#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QComboBox>
#include "mainwindow.h"
#include "tabcontroller.h"

#define DEFAULT_CMD_TAIL "tail -f "

class Settings : public QDialog {

    Q_OBJECT
public:
    explicit Settings(QWidget *parent = 0);

private:
    void createUI();
    void loadSettings(s_tail *_tail);
    void loadTails();
    void saveTails();
    int m_activeListId;
    bool m_statusSaving;
    MainWindow *mainwindow;
    QListWidget *tailList;


    QPushButton *buttonClose;
    QPushButton *buttonSave;
    QPushButton *buttonDelete;

    QLineEdit *editAlias;
    QCheckBox *editAutostart;
    QLineEdit *editHost;
    QLineEdit *editPort;
    QLineEdit *editUsername;
    QComboBox *editAuthentication;
    QLineEdit *editPath;
    QLineEdit *editCommand;
    QCheckBox *editEditCommand;

signals:

private slots:
    void slotItemChanged(int);
    void slotItemClicked(QListWidgetItem*);
    void slotCreateNew();
    void slotSaveSettings();
    void slotDeleteTail();
    void slotUpdateCommand(QString);
    void slotEditCommandToggled(bool);

public slots:
    void accept();

};

#endif // SETTINGS_H
