#include "../Headers/SimpleLogger.h"
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QTextCodec>

const char* CONF_NAME = "LogerConfig.ini";
const char* CONF_LOGNAME = "ukp.log";

const char* CONF_GR = "LoggerConfiguration";
const char* CONF_P1 = "path2log";
const char* CONF_P2 = "max_size";


SimpleLogger* SimpleLogger::_instance = 0;
bool SimpleLogger::_isInst = false;

SimpleLogger *SimpleLogger::Instance(){
    if(!_isInst){
        _instance = new SimpleLogger();
        _isInst = true;
    }
    return _instance;
}

SimpleLogger::SimpleLogger(){
    read_config();
}
void SimpleLogger::read_config(){
    QString path_cfg = QCoreApplication::applicationDirPath()+QDir::separator()+CONF_NAME;

    QSettings *s = new QSettings(path_cfg,QSettings::IniFormat);

    QString path;

    s->beginGroup(CONF_GR);
    s->value(CONF_P1,path);
    s->value(CONF_P2,_maxlogsize);
    s->endGroup();

    // если проблемы с конфигом
    if(path.isNull() && path.isEmpty()){
        path = QCoreApplication::applicationDirPath()+QDir::separator()+CONF_LOGNAME;
        _maxlogsize = 50; // в Мб

        s->beginGroup(CONF_GR);
        s->setValue(CONF_P1,path);
        s->setValue(CONF_P2,_maxlogsize);
        s->endGroup();

        s->sync();
    }

    delete s;

    _flog = new QFile(path);
    bool bOpen = false;
    if(_flog->exists()){
        bOpen = _flog->open(QIODevice::Append|QIODevice::Text);
    }else{
        bOpen = _flog->open(QIODevice::WriteOnly|QIODevice::Text);
    }
    if(!bOpen){
        qDebug().noquote()<<"Error open log file";
    }


}
void SimpleLogger::write_log(SimpleLogger::SEVERITY type,const QString& mes){
    qDebug().noquote()<<"LOG: "<<mes;
    QString stype;
    switch(type){
    case SimpleLogger::WARNING: stype = "WARNING"; break;
    case SimpleLogger::ERROR: stype = "ERROR"; break;
    default:
        stype = "INFO";
    }
    QTextStream _slog(_flog);
    _slog.setCodec(QTextCodec::codecForName("UTF-8"));
    _slog<<"("<<QDateTime::currentDateTime().toString()<<") ["<<stype<<"] "<<mes<<"\r\n";
    _slog.flush();
}
SimpleLogger::~SimpleLogger(){
    _flog->close();
    delete _flog;
}
void SimpleLogger::warn(QString mes){write_log(SimpleLogger::WARNING,mes);}
void SimpleLogger::err(QString mes){write_log(SimpleLogger::ERROR,mes);}
void SimpleLogger::log(QString mes){write_log(SimpleLogger::INFO,mes);}
