#ifndef LOGINSERVICE_H
#define LOGINSERVICE_H

#include <QtCore/qglobal.h>
#include <QWidget>
#if defined(LOGINSERVICE_LIBRARY) && !defined(Q_OS_WIN32)
    #include "../EntityManager/Headers/EM_AddressBook.h"
#else
    #include <EM_AddressBook>
#endif

#if defined(LOGINSERVICE_LIBRARY)
#  define LOGINSERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGINSERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C"{
    EM_User* LOGINSERVICESHARED_EXPORT input2system(int argc, char *argv[],bool admin=false,QWidget *parent=0);
}

#endif // LOGINSERVICE_H
