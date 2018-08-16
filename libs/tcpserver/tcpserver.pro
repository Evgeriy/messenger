QT       += gui widgets sql
QT       += network
DESTDIR = $$PWD/../../build/lib

TARGET = tcpserver
TEMPLATE = lib

SOURCES += \
        tcpserver.cpp

HEADERS += \
        tcpserver.h \
        tcpserver_global.h 

unix|win32: LIBS += -L$$DESTDIR/ -ldbmanager

INCLUDEPATH += $$PWD/../dbmanager
DEPENDPATH += $$PWD/../dbmanager
