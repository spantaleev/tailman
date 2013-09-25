#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>

#include "sshwrapper.h"
#include "mainwindow.h"
#include "tabcontroller.h"

class Tab : public QWidget {

    Q_OBJECT
public:
    explicit Tab(QWidget *parent, s_tail *_tail);
    ~Tab();

    SshWrapper *sshWrapper;
    QTextEdit *textEdit;
    int m_tabIndex;
    bool m_connected;
    bool m_closing_tab;
    s_tail *tail;
    QLabel *labelStatus;
    bool m_hostkey_verified;
    QString m_manual_password;
    int m_new_lines;

private:
    void createUI();
    TabController *tabController;

signals:

public slots:
    void slotAddContent(QString content);
    void slotTailFailed(QString message);
    void slotCleanTail();
    void slotVerifyHostKey(QString key);
    void slotEnterPassword();
};

#endif // TAB_H
