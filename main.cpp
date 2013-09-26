#include <QApplication>

#if QT_VERSION < 0x050000
#include <QTextCodec>
#endif

#include "mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    a.setApplicationName("tailman");
    a.setOrganizationName("tailman");
    a.setOrganizationDomain("tailman");

    #if QT_VERSION < 0x050000
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    #endif

    MainWindow main;
    main.setWindowIcon(QIcon(":images/network.png"));
    main.show();

    return a.exec();
}
