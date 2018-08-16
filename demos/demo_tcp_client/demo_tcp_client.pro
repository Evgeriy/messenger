QT       += core gui widgets network

TARGET = demo_tcp_client
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../build/lib/ -ltcpclient

INCLUDEPATH += $$PWD/../../libs/tcpclient
DEPENDPATH += $$PWD/../../libs/tcpclient
