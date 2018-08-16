QT       -= gui
QT       += network
DESTDIR = $$PWD/../../build/lib

TARGET = tcpclient
TEMPLATE = lib

SOURCES += \
        tcpclient.cpp

HEADERS += \
        tcpclient.h \
        tcpclient_global.h 
