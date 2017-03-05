
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = ukpplan
TEMPLATE = lib

DEFINES += UKPPLAN_LIBRARY

HEADERS += ../../include/h/defukpplan.h \
           ../../include/h/tappplan.h \
           ../../include/h/tmoduleplans.h \
           ../../include/h/tabstractplanelement.h \
           ../../include/h/tabstractplan.h \
           ../../include/h/tgenplan.h \
           ../../include/h/tcarryplan.h \
           ../../include/h/tcarrytask.h \
           ../../include/h/tcarryprocedure.h \
           ../../include/h/tcarrywork.h \
           ../../include/h/tcalendartable.h


SOURCES += src/modules/classes/tabstractplanelement.cpp \
           src/modules/classes/tabstractplan.cpp \
           src/modules/classes/tgenplan.cpp \
           src/modules/classes/tcarryplan.cpp \
           src/modules/classes/tcarrytask.cpp \
           src/modules/classes/tcarryprocedure.cpp \
           src/modules/classes/tcarrywork.cpp \
           src/modules/proj/planfunction.cpp

SOURCES += src/modules/proj/tappplan.cpp \
           src/modules/proj/tmoduleplans.cpp

SOURCES += src/modules/qext/tcalendartable.cpp

unix {
    UI_DIR      = .tmp/ui
    OBJECTS_DIR = .tmp/obj
    MOC_DIR     = .tmp/moc
    INCLUDEPATH += . ../../include
    LIBS += -L../../build/libs -lukpcommon
    QMAKE_POST_LINK  = "mkdir -p ../../build/libs" && "mv -f libukpplan.* ../../build/libs"
}

win32 {
    INCLUDEPATH += . ../../include
    LIBS += -L../../build/libs -lukpcommon -lEntityManager
    QMAKE_POST_LINK  = mkdir "..\..\build\libs" & \
                       for %%i in ("debug\libukpplan.a","debug\ukpplan.dll") do copy "%%i" "..\..\build\libs\*.*" /y & \
                       copy "debug\ukpplan.dll" "..\..\apps\ukpcarryplan\debug\*.*" /y
}

