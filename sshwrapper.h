#ifndef SSHWRAPPER_H
#define SSHWRAPPER_H

#include <QObject>
#include <QThread>
#include "libssh/libssh.h"
#include "mainwindow.h"

class Tab;

class SshWrapper : public QThread {

    Q_OBJECT

public:
    ssh_session session;

    explicit SshWrapper(QWidget *parent, s_tail *_tail);
    int connectSsh();
    void disconnectSsh();
    int verifyKnownHost();
    int readData();

    void run();
private:
    Tab *tab;
    s_tail *tail;

signals:
    void signalUpdateContent(QString);
    void signalTailFailed(QString);
    void signalClearTail();
    void signalVerifyHostKey(QString);
    void signalEnterPassword();

public slots:

};

#endif // SSHWRAPPER_H
