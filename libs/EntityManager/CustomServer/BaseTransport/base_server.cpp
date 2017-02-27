
#include "base_server.h"
#include "base_thread.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QSettings>

namespace BaseServerHolderConst{
    const char* PROP_FILE_NAME = "RPCServer.prop";
    const char* SERVRE_IP = "SERVER_IP";
    const char* SERVER_PORT = "SERVER_PORT";

    const char* SERVER_IP_DEF = "127.0.0.1";
    const int SERVER_PORT_DEF = 6666;
}

// ===================================
BaseServerHolder* BaseServerHolder::_instance = 0;
bool BaseServerHolder::_init = false;

BaseServerHolder::BaseServerHolder(QObject *parent):QObject(parent){
    m_Server = new BaseServer(this);

    qDebug()<<"Init BaseServerHolder";
    int port = 6666;
    QString server_ip = "128.19.10.66";
    // грузим конфиг
    qDebug()<<"Load config";
    m_Server->initProp(server_ip,port);

    // вешаем прослушку
    bool bStart = m_Server->listen(QHostAddress(server_ip),port);
    if(!bStart){
        qDebug()<<"Error while listen socket";
    }else{
        qDebug()<<"BaseServer listen "<<m_Server->serverAddress().toString()<<":"<<QString::number(m_Server->serverPort());
    }


}
BaseServerHolder::~BaseServerHolder(){
    qDebug()<<"Destroy BaseServerHolder";
    //delete m_Server;
}

BaseServerHolder &BaseServerHolder::Instance(){
    if(!_init){
        _instance = new BaseServerHolder();
        _init = true;
    }
    return *_instance;
}
// ===================================
BaseServer::BaseServer(QObject *parent) : QTcpServer(parent){
    path2prop = QCoreApplication::applicationDirPath()+QDir::separator()+QString(BaseServerHolderConst::PROP_FILE_NAME);
}
void BaseServer::initProp(QString& ip,int &port){
    bool bOk = false;
    QFile file(path2prop);
    if(file.exists()){
        QSettings* settings = new QSettings(path2prop,QSettings::IniFormat);

        bool checkContains = settings->contains(BaseServerHolderConst::SERVRE_IP)&settings->contains(BaseServerHolderConst::SERVER_PORT);
        if(checkContains){
            ip = settings->value(BaseServerHolderConst::SERVRE_IP).toString();
            port = settings->value(BaseServerHolderConst::SERVER_PORT).toInt();

            bOk = true;
        }

        delete settings;
    }
    if(!bOk){
        ip = BaseServerHolderConst::SERVER_IP_DEF;
        port = BaseServerHolderConst::SERVER_PORT_DEF;
        QSettings* settings = new QSettings(path2prop,QSettings::IniFormat);
        settings->setValue(BaseServerHolderConst::SERVRE_IP,ip);
        settings->setValue(BaseServerHolderConst::SERVER_PORT,port);
        settings->sync();
        delete settings;
    }

}


void BaseServer::incomingConnection(qintptr socketDescriptor){
    qDebug()<<"Detect incoming connection";
    BaseThread *thread = new BaseThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    map[thread->threadID()] = thread;

}



