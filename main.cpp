#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    a.setApplicationName("tailman");
    a.setOrganizationName("tailman");
    a.setOrganizationDomain("tailman");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MainWindow main;
    main.setWindowIcon(QIcon(":images/network.png"));
    main.show();

    return a.exec();
}
