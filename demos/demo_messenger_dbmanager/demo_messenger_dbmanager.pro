QT       += core gui widgets sql

TARGET = demo_messenger_dbmanager
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../build/lib/ -ldbmanager

INCLUDEPATH += $$PWD/../../libs/dbmanager
DEPENDPATH += $$PWD/../../libs/dbmanager
