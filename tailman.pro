TEMPLATE = app
TARGET = tailman

QT += core
QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += .
INCLUDEPATH += .

LIBS += -lssh

SOURCES += main.cpp \
    mainwindow.cpp \
    tabcontroller.cpp \
    tab.cpp \
    sshwrapper.cpp \
    addtab.cpp \
    settings.cpp \
    about.cpp

HEADERS += \
    mainwindow.h \
    tabcontroller.h \
    tab.h \
    sshwrapper.h \
    libssh/libssh.h \
    addtab.h \
    settings.h \
    about.h

RESOURCES += \
    images.qrc

target.path = /usr/bin
INSTALLS += target