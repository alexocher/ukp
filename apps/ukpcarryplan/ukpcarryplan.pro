
QT       += core gui

#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ukpcarryplan
TEMPLATE = app

UI_DIR      = .tmp/ui
OBJECTS_DIR = .tmp/obj
MOC_DIR     = .tmp/moc

HEADERS += src/modules/proj/tappcarryplan.h

HEADERS += src/forms/fmainukpcarryplan.h \
           src/forms/fdialog_nnm.h \
           src/forms/wgantdiagramm.h \
           src/forms/wcompanycalendar.h \
           src/forms/wemployeecalendar.h \
           src/forms/worgshtat.h \
           src/forms/wplan.h \
           src/forms/wproduction.h \
           src/forms/wtemplate.h

SOURCES += src/modules/proj/tappcarryplan.cpp

SOURCES += src/main.cpp \
           src/forms/fmainukpcarryplan.cpp \
           src/forms/fdialog_nnm.cpp \
           src/forms/wgantdiagramm.cpp \
           src/forms/wcompanycalendar.cpp \
           src/forms/wemployeecalendar.cpp \
           src/forms/worgshtat.cpp \
           src/forms/wproduction.cpp \
           src/forms/wplan.cpp \
           src/forms/wtemplate.cpp

FORMS   += src/forms/fmainukpcarryplan.ui \
           src/forms/fdialog_nnm.ui \
           src/forms/wgantdiagramm.ui \
           src/forms/wcompanycalendar.ui \
           src/forms/wemployeecalendar.ui \
           src/forms/worgshtat.ui \
           src/forms/wplan.ui \
           src/forms/wproduction.ui \
           src/forms/wtemplate.ui

RESOURCES += src/res.qrc

unix {
     INCLUDEPATH += src/modules ../../include
     LIBS += -L../../build/libs -lukpcommon -lukpplan -lDBConnect -lEntityManager -lLoginService
}

win32 {
     INCLUDEPATH += src/modules ../../include
     LIBS += -L../../build/libs -lukpcommon -lukpplan -lDBConnect -lEntityManager -lLoginService
}

