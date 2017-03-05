
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = ukpcommon
TEMPLATE = lib

DEFINES += UKPCOMMON_LIBRARY

HEADERS += ../../include/h/defukpcommon.h \
           ../../include/h/defconst.h \
           ../../include/h/defdbconst.h \
           ../../include/h/defmacro.h \
           ../../include/h/defpictures.h \
           ../../include/h/common.h \
           ../../include/h/enums.h \
           ../../include/h/general.h \
           ../../include/h/tident.h \
           ../../include/h/geod_base.h \
           ../../include/h/qptrlist.h \
           ../../include/h/qtools.h \
           ../../include/h/mdisubwindow.h \
           ../../include/h/tgantgraphicsview.h \
           ../../include/h/tabstractobject.h \
           ../../include/h/tabstractapp.h \
           ../../include/h/tabstractmodule.h \
           ../../include/h/tmoduleemployees.h \
           ../../include/h/tmoduleproductions.h \
           ../../include/h/tmoduleunits.h \
           ../../include/h/tunit.h \
           ../../include/h/temployee.h \
           ../../include/h/tproduction.h \
           ../../include/h/tabstractattachment.h \
           ../../include/h/tdocument.h \
           ../../include/h/tbdfdata.h \
           ../../include/h/toobgddata.h \
           ../../include/h/tselfdrawwidget.h \
           ../../include/h/ttabledelegate.h \
           ../../include/h/ttablewidgetitemdata.h \
           ../../include/h/tworkcalendar.h \
           ../../include/h/convertEnums.h

SOURCES += src/modules/etc/common.cpp \
           src/modules/etc/general.cpp \
           src/modules/etc/tident.cpp \
           src/modules/etc/geod_base.cpp \
           src/modules/etc/convertEnums.cpp

SOURCES += src/modules/classes/tabstractobject.cpp \
           src/modules/classes/temployee.cpp \
           src/modules/classes/tunit.cpp \
           src/modules/classes/tproduction.cpp \
           src/modules/classes/tworkcalendar.cpp

SOURCES += src/modules/classes/tabstractattachment.cpp \
           src/modules/classes/tdocument.cpp \
           src/modules/classes/tbdfdata.cpp \
           src/modules/classes/toobgddata.cpp

SOURCES += src/modules/proj/tabstractapp.cpp \
           src/modules/proj/tabstractmodule.cpp \
           src/modules/proj/tmoduleemployees.cpp \
           src/modules/proj/tmoduleproductions.cpp \
           src/modules/proj/tmoduleunits.cpp

SOURCES += src/modules/qext/qtools.cpp \
           src/modules/qext/selfdrawwidget.cpp \
           src/modules/qext/tabledelegate.cpp \
           src/modules/qext/tablewidgetitemdata.cpp \
           src/modules/qext/mdisubwindow.cpp \
           src/modules/qext/tgantgraphicsview.cpp

unix {
    UI_DIR      = .tmp/ui
    OBJECTS_DIR = .tmp/obj
    MOC_DIR     = .tmp/moc
    #DEFINES += WORK_WITH_LOCAL_DATA
    INCLUDEPATH += . ../../include
    LIBS += -L../../build/libs -lEntityManager
    QMAKE_POST_LINK  = "mkdir -p ../../build/libs" && "mv -f libukpcommon.* ../../build/libs"
}

win32 {
    #DEFINES += WORK_WITH_LOCAL_DATA
    INCLUDEPATH += . ../../include
    LIBS += -L../../build/libs -lEntityManager
    QMAKE_POST_LINK  = mkdir "..\..\build\libs" & \
                       for %%i in ("debug\libukpcommon.a","debug\ukpcommon.dll") do copy "%%i" "..\..\build\libs\*.*" /y & \
                       copy "debug\ukpcommon.dll" "..\..\apps\ukpcarryplan\debug\*.*" /y
}
