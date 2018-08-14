#-------------------------------------------------
#
# Project created by QtCreator 2018-08-09T11:45:55
#
#-------------------------------------------------

QT       += gui widgets sql
QT       += network

TARGET = tcpserver
TEMPLATE = lib

DEFINES += TCPSERVER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        tcpserver.cpp

HEADERS += \
        tcpserver.h \
        tcpserver_global.h 

unix {
    target.path = $$OUT_PWD/../dbmanager
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dbmanager/release/ -ldbmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dbmanager/debug/ -ldbmanager
else:unix: LIBS += -L$$OUT_PWD/../dbmanager/ -ldbmanager

INCLUDEPATH += $$PWD/../dbmanager
DEPENDPATH += $$PWD/../dbmanager
