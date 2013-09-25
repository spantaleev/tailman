#ifndef TABCONTROLLER_H
#define TABCONTROLLER_H

#include <QTabWidget>
#include <QSettings>

struct s_tail {
    bool autostart;
    QString alias;
    QString host;
    int port;
    QString auth_method;
    QString username;
    QString tail_path;
    QString command;
};

class TabController : public QTabWidget {

    Q_OBJECT
public:
    explicit TabController(QWidget *parent = 0);

    QList<s_tail*> tailList;
    QString m_tail_bg_color;
    QString m_tail_text_color;

    void loadTails();
    void openAutostartTails();
    void setTabColor(int, QColor);

private:
    QSettings m_settings;

signals:

public slots:
    void slotNewTab();


private slots:
    void slotTabChanged(int index);
    void slotTabClose(int index);

};

#endif // TABCONTROLLER_H
