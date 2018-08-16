QT       -= gui
QT       += sql
DESTDIR = $$PWD/../../build/lib

TARGET   = dbmanager
TEMPLATE = lib

HEADERS += \
        databasemanager.h \
        dbmanager_global.h

SOURCES += \
        databasemanager.cpp
