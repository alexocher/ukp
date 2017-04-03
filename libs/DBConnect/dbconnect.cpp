#include "dbconnect.h"
#include <QUuid>
#include <QMap>

const char* _DBNAME   = "db_name";
const char* _USERNAME = "user_name";
const char* _PASSWORD = "password";
const char* _HOST     = "host";
const char* _PORT     = "port";

void  read_config(QMap<QString,QString> &data){
    QString path_cfg = QCoreApplication::applicationDirPath()+QDir::separator()+"dbcfg.ini";

    QSettings *s = new QSettings(path_cfg,QSettings::IniFormat);

    QString _dbname;
    QString _username;
    QString _password;
    QString _host;
    QString _port;

    s->beginGroup("DBAccessConfig");
    _dbname = s->value(_DBNAME).toString();
    _username = s->value(_USERNAME).toString();
    _password = s->value(_PASSWORD).toString();
    _host = s->value(_HOST).toString();
    _port = s->value(_PORT).toString();
    s->endGroup();

    // если проблемы с конфигом
    if(_dbname.isNull() || _dbname.isEmpty()){
        _dbname     = "moderndb";
        _username   = "postgres";
        _password   = "postgres";
        _host       = "kpi1.demo.kt";
        _port       = "5432";

        s->beginGroup("DBAccessConfig");
        s->setValue(_DBNAME,_dbname);
        s->setValue(_USERNAME,_username);
        s->setValue(_PASSWORD,_password);
        s->setValue(_HOST,_host);
        s->setValue(_PORT,_port);
        s->endGroup();

        s->sync();
    }

    delete s;

    data[_DBNAME] = _dbname;
    data[_USERNAME] = _username;
    data[_PASSWORD] = _password;
    data[_HOST] = _host;
    data[_PORT] = _port;

}



QSqlDatabase getConnection(const QString& connectionname){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",connectionname);
//    db.setDatabaseName("moderndb");
//    db.setUserName("modern");
//    db.setPassword("modern");
//    db.setHostName("dbmodern");
    // !!!TODO:
    QMap<QString,QString> param;
    read_config(param);

    db.setDatabaseName(param[_DBNAME]);
    db.setUserName(param[_USERNAME]);
    db.setPassword(param[_PASSWORD]);
    db.setHostName(param[_HOST]);
    db.setPort(param[_PORT].toInt());



    return db;
}

QString generateConnectionCustomName(const QString& connectionname){
    return connectionname+QUuid::createUuid().toString();
}
QString generateConnectionName(){
    return QUuid::createUuid().toString();
}
