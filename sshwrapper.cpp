#include "sshwrapper.h"
#include "tab.h"

#include <errno.h>
#include <QDebug>

SshWrapper::SshWrapper(QWidget *parent, s_tail *_tail) : QThread() {

    tab = qobject_cast<Tab*>(parent);
    tail = _tail;

    session = ssh_new();

    if (session == NULL) {
        qDebug() << "Failed to init ssh object.";
    }

    connect(this, SIGNAL(signalUpdateContent(QString)),
        tab, SLOT(slotAddContent(QString)));
    connect(this, SIGNAL(signalTailFailed(QString)),
        tab, SLOT(slotTailFailed(QString)));
    connect(this, SIGNAL(signalClearTail()),
        tab, SLOT(slotCleanTail()));
    connect(this, SIGNAL(signalVerifyHostKey(QString)),
        tab, SLOT(slotVerifyHostKey(QString)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(signalEnterPassword()),
        tab, SLOT(slotEnterPassword()), Qt::BlockingQueuedConnection);

    ssh_options_set(session, SSH_OPTIONS_HOST,
        tail->username.toUtf8().append("@").append(tail->host.toUtf8()));

    const void *timeout = "1000";
    ssh_options_set(session, SSH_OPTIONS_TIMEOUT, timeout);
}

int SshWrapper::readData() {

    ssh_channel channel;
    int rc;

    channel = ssh_channel_new(session);

    if (channel == NULL)
        return SSH_ERROR;

    rc = ssh_channel_open_session(channel);

    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        return rc;
    }

    rc = ssh_channel_request_exec(channel, tail->command.toUtf8());

    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return rc;
    }

    char buffer[512];
    int nbytes;

    while (tab->m_connected && (nbytes = ssh_channel_read(channel, buffer, sizeof(buffer)-1, 0) > 0)) {
        emit signalUpdateContent(QString(buffer));
        memset(buffer, 0, sizeof(buffer));
    }

    if (nbytes < 0) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return SSH_ERROR;
    }

    if (tab->m_connected) {
        ssh_channel_send_eof(channel);
        ssh_channel_close(channel);
        ssh_channel_free(channel);
    }

    return SSH_OK;
}

int SshWrapper::connectSsh() {

    int ret = ssh_connect(session);

    if (ret != SSH_OK) {
        emit signalTailFailed(QString(ssh_get_error(session)));
        ssh_free(session);
        return -1;
    }

    if (verifyKnownHost() < 0) {
        emit signalTailFailed(QString("Host key was not verified.\n"));
        ssh_free(session);
        return -1;
    }

    if (tail->auth_method.compare("password") == 0) { // Password
        emit signalEnterPassword();
        ret = ssh_userauth_password(session, tail->username.toUtf8(), tab->m_manual_password.toUtf8());
    } else { // Public key
        ret = ssh_userauth_autopubkey(session, NULL);
    }

    if (ret != SSH_AUTH_SUCCESS) {
        emit signalTailFailed(QString("Failed to auhtenticate. (%1)\n").arg(ret));

        if (ret == SSH_AUTH_ERROR) {
            emit signalTailFailed(QString("Error: %1.\n").arg(ssh_get_error(session)));
        }

        disconnectSsh();
        return -1;
    }

    return 0;
}

void SshWrapper::disconnectSsh() {

    tab->m_connected = false;
    ssh_disconnect(session);
}

int SshWrapper::verifyKnownHost() {

    int state, hlen;
    unsigned char *hash = NULL;
    char *hexa;

    state = ssh_is_server_known(session);
    hlen = ssh_get_pubkey_hash(session, &hash);

    if (hlen < 0) {
        return -1;
    }

    switch (state) {
        case SSH_SERVER_KNOWN_OK:
        break;
        case SSH_SERVER_KNOWN_CHANGED:
            emit signalTailFailed(
                QString("Host key for server changed.\nFor security reasons, connection will be stopped\n"));
            free(hash);
            return -1;
        case SSH_SERVER_FOUND_OTHER:
            emit signalTailFailed(
                    QString("The host key for this server was not found but an other type of key exists."
                            "An attacker might change the default server key to"
                            "confuse your client into thinking the key does not exist\n"));
            free(hash);
            return -1;
        case SSH_SERVER_FILE_NOT_FOUND:
            emit signalTailFailed(
                    QString("Could not find known host file.\n"
                            "If you accept the host key here, the file will be"
                            "automatically created.\n"));
        case SSH_SERVER_NOT_KNOWN:
            hexa = ssh_get_hexa(hash, hlen);

            // Call verify host messagebox
            emit signalVerifyHostKey(hexa);
            free(hexa);

            if (!tab->m_hostkey_verified)
            {
                free(hash);
                return -1;
            }

            if (ssh_write_knownhost(session) < 0)
            {
                emit signalTailFailed(
                        QString("Error: %1").arg(strerror(errno)));
                free(hash);
                return -1;
            }
        break;
        case SSH_SERVER_ERROR:
            emit signalTailFailed(
                    QString("Error: %1").arg(ssh_get_error(session)));
            free(hash);
            return -1;
    }

    free(hash);

    return 0;
}

void SshWrapper::run() {

    emit signalUpdateContent(QString("Connecting to %1\n").arg(tail->host));

    if (connectSsh() == 0) {
        tab->labelStatus->setText(QString("[<span style=\"color:green;\">Online</span>]" \
            " <span style=\"color:#121212;\">%1 (%2)</span>").arg(tail->alias, tail->command));
        emit signalClearTail();
        tab->m_connected = true;
        int ret = readData();

        if (!tab->m_closing_tab) {
            emit signalTailFailed(
                     QString("No more data to read. Disconnecting (%1) ...\n").arg(ret));
            disconnectSsh();
            tab->labelStatus->setText(QString("[<span style=\"color:red;\">Offline</span>]" \
                " <span style=\"color:#121212;\">%1 (%2)</span>").arg(tail->alias, tail->command));
        }
    } else {
        quit();
    }
}
