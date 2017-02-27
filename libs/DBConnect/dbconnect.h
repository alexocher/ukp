#ifndef DBCONNECT_H
#define DBCONNECT_H
#include <QtSql>
#include <QtCore/qglobal.h>

#if defined(LIBRARY)
#  define SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" {
    SHARED_EXPORT QSqlDatabase getConnection(const QString& connectionname);

    SHARED_EXPORT QString generateConnectionCustomName(const QString& connectionname);
    SHARED_EXPORT QString generateConnectionName();
}
#endif // DBCONNECT_H
