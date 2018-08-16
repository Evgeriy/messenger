QT       += core gui widgets network sql

TARGET = demo_tcp_server
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../build/lib/ -ltcpserver -ldbmanager

INCLUDEPATH += $$PWD/../../libs/tcpserver
INCLUDEPATH += $$PWD/../../libs/dbmanager

DEPENDPATH  += $$PWD/../../libs/tcpserver
DEPENDPATH  += $$PWD/../../libs/dbmanager

