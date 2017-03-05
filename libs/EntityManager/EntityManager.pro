#-------------------------------------------------
#
# Project created by QtCreator 2016-08-04T14:54:32
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = EntityManager
TEMPLATE = lib

DEFINES += ENTITYMANAGER_LIBRARY

SOURCES += \    
    Plan/EM_Plan.cpp \
    Oshs/EM_Oshs.cpp \
    Oshs/EM_AddressBook.cpp \
    Production/EM_Production.cpp \
    Calendar/EM_Calendar.cpp \
    ExtProcedure/EM_ExtProc.cpp \
    Utils/Geom.cpp \
    Utils/helper.cpp \
    Policy/EM_Policy.cpp \
    CustomServer/BaseTransport/base_server.cpp \
    CustomServer/BaseTransport/base_thread.cpp \
    ModelBDF/ModelBDF.cpp \
    Utils/SimpleLogger.cpp

HEADERS += \
    Headers/BaseTree.h \
    Headers/commondef.h \
    Headers/defdbconst.h \
    Headers/EM_Plan.h \
    Headers/EM_Oshs.h \
    Headers/enums.h \
    Headers/exceptions.h \
    Headers/EM_Production.h \
    Headers/EM_AddressBook.h \
    Headers/EM_Calendar.h \
    HeadersInt/StringConsts.h \
    Headers/EM_ExtProc.h \
    Headers/Geom.h \
    Headers/EM_Policy.h \
    HeadersInt/helper.h \
    CustomServer/BaseTransport/base_server.h \
    CustomServer/BaseTransport/base_thread.h \
    ModelBDF/ModelBDF.h \
    Headers/SimpleLogger.h


win32:CONFIG(release, debug|release): {
    LIBS += -L$$PWD/../DBConnect/builds/release/ -lDBConnect

    OBJECTS_DIR = .tmp/obj
    MOC_DIR     = .tmp/moc
    RCC_DIR     = .tmp/qrc

    QMAKE_POST_LINK  = "copy  ..\release\*.dll /B ..\..\release\ /B /Y"
}else:win32:CONFIG(debug, debug|release): {
    LIBS += -L$$PWD/../DBConnect/builds/debug/ -lDBConnect

    OBJECTS_DIR = .tmp/obj
    MOC_DIR     = .tmp/moc
    RCC_DIR     = .tmp/qrc

    QMAKE_POST_LINK  = "copy  .\debug\*.dll /B ..\..\release\ /B /Y" && "copy  .\debug\*.dll /B ..\..\TestApp\build\debug\ /B /Y"
}else:unix {
    OBJECTS_DIR = .tmp/obj
    LIBS += -L../../build/libs -lDBConnect
    QMAKE_POST_LINK  = "mkdir -p ../../build/libs" && "mv -f libEntityManager.* ../../build/libs"

    targ_commit.target = COMMIT_HEADERS
    targ_commit.commands = "cp -f Headers/* ../../include/h"
    QMAKE_EXTRA_TARGETS += targ_commit
    PRE_TARGETDEPS += $$targ_commit.target
}

INCLUDEPATH += $$PWD/../DBConnect
DEPENDPATH += $$PWD/../DBConnect



