#include "dbconnect.h"
#include <QUuid>
QSqlDatabase getConnection(const QString& connectionname){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",connectionname);
    db.setDatabaseName("moderndb");
    db.setUserName("modern");
    db.setPassword("modern");
    db.setHostName("dbmodern");

    return db;
}

QString generateConnectionCustomName(const QString& connectionname){
    return connectionname+QUuid::createUuid().toString();
}
QString generateConnectionName(){
    return QUuid::createUuid().toString();
}
