#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T11:06:38
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = DBConnect
TEMPLATE = lib

DEFINES += LIBRARY

SOURCES += dbconnect.cpp \
    BDF/GateBDF.cpp \
    BGD/GateBGD.cpp \
    BaseGate/BaseGate.cpp
HEADERS += dbconnect.h\
    BDF/GateBDF.h \
    BGD/GateBGD.h \
    BaseGate/BaseGate.h

win32:CONFIG(release, debug|release): {
    OBJECTS_DIR = ./obj
    QMAKE_POST_LINK  = "copy  ..\release\*.dll /B ..\..\release\ /B /Y"
}
win32:CONFIG(debug, debug|release): {
    OBJECTS_DIR = ./obj
    QMAKE_POST_LINK  = "copy  .\debug\*.dll /B ..\..\release\ /B /Y" && "copy  .\debug\*.dll /B ..\..\TestApp\build\debug\ /B /Y"
}
unix: !macx {
    OBJECTS_DIR = .tmp/obj
    QMAKE_POST_LINK  = "mkdir -p ../../build/libs" && "mv -f libDBConnect.* ../../build/libs"
}
