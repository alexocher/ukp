#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T17:09:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoginService
TEMPLATE = lib

DEFINES += LOGINSERVICE_LIBRARY

SOURCES += loginservice.cpp \
            LoginDialog.cpp

HEADERS += loginservice.h \
            LoginDialog.h

win32:CONFIG(release, debug|release): {
    LIBS += -L$$PWD/../EntityManager/builds/release/ -lEntityManager

    OBJECTS_DIR = ./obj
    QMAKE_POST_LINK  = "copy  ..\release\*.dll /B ..\..\release\ /B /Y"

    RESOURCES += ../resources/resources.qrc
}
win32:CONFIG(debug, debug|release): {
    LIBS += -L$$PWD/../EntityManager/builds/debug/ -lEntityManager

    OBJECTS_DIR = ./obj
    QMAKE_POST_LINK  = "copy  .\debug\*.dll /B ..\..\release\ /B /Y"

    RESOURCES += ../resources/resources.qrc
}
unix:!macx {
    OBJECTS_DIR = .tmp/obj
    LIBS += -L../../build/libs -lEntityManager
    QMAKE_POST_LINK  = "mkdir -p ../../build/libs" && "mv -f libLoginService.* ../../build/libs"

    targ_commit.target = COMMIT_HEADERS
    targ_commit.commands = "cp -f ./loginservice.h ../../include/h"
    QMAKE_EXTRA_TARGETS += targ_commit
    PRE_TARGETDEPS += $$targ_commit.target
}

