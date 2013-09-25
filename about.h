#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

#define VERSION "0.1 - 0924"

class About : public QDialog
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);

private:
    void createUI();

signals:

public slots:
    void accept();
};

#endif // ABOUT_H
