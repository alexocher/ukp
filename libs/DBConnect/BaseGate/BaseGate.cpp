#include <QNetworkRequest>
#include <QNetworkReply>
#include "BaseGate.h"


//const char *templ_https = "https://%1:%2@%3/api";
//const char *templ_http = "http://%1:%2@%3/api";

//const char *templ_https_public = "https://%1/api";
//const char *templ_http_public = "http://%1/api";

const char *CONNECT_NAME = "GATE_CONNECT";

BaseGate::~BaseGate(){
    if(m_pManager!=0){delete m_pManager;}
}
//BaseGate::BaseGate(const QString &login, const QString& password, const QString& url , const QString &prefix, bool secure){
//    m_url = QString((secure?templ_https:templ_http)+prefix /*"https://%1:%2@%3/api"*/).arg(login).arg(password).arg(url);
//    m_pManager = new QNetworkAccessManager();
//    //qDebug()<<m_url;
//}
//BaseGate::BaseGate(const QString& url, const QString& prefix, bool secure ){
//    m_url = QString((secure?templ_https_public:templ_http_public)+prefix).arg(url);
//    m_pManager = new QNetworkAccessManager();
//}
BaseGate::BaseGate(GATE_TYPE type){
    QSqlDatabase db;
    if(QSqlDatabase::contains(CONNECT_NAME)){
        db = QSqlDatabase::database(CONNECT_NAME);
    }else{
        db = getConnection(CONNECT_NAME);
    }
    bool bOK = false;
    m_pManager = new QNetworkAccessManager();
    if(db.open()){
        QSqlQuery* q = new QSqlQuery(db);

        q->prepare("SELECT host,login,password FROM stuff.gate_cfg WHERE db_type = :type ");
        q->bindValue(":type",type);
        if(q->exec()){
            while(q->next()){
                m_url = q->value("host").toString();
                bOK = true;
            }
        }else{
            qDebug()<<"Error while exec query: "<<q->lastError();
        }
        delete q;
        db.close();
    }else{
        qDebug()<<"Error while open db: "<<db.lastError();
    }

    if(!bOK){
        qDebug()<<"Error configure gate to db "<<QString::number(type);
    }
    qDebug()<<"Check GATE: "<<m_url;
}

bool BaseGate::isConnect(){
    int sts;
    _request(GET,"",QString(),QByteArray(),sts);
    if(sts==200) return true;
    return false;
}
const QString& BaseGate::showURL() const{
    return m_url;
}
QByteArray BaseGate::_request(REQUEST_TYPE type, const QUrl &url, const QString &content_type, const QByteArray &data, int &sts){
    QNetworkRequest request = QNetworkRequest(url);
    return _request(type,request,content_type,data,sts);
}
QByteArray BaseGate::_request(REQUEST_TYPE type,const QString &path,const QString &content_type,const QByteArray &data,int &sts){
    QNetworkRequest request = QNetworkRequest(QUrl(m_url+path));
    return _request(type,request,content_type,data,sts);
}
QByteArray BaseGate::_request(REQUEST_TYPE type,QNetworkRequest &request,const QString &content_type,const QByteArray &data,int &sts){
    if(!content_type.isEmpty() && !content_type.isNull() ){
        request.setHeader(QNetworkRequest::ContentTypeHeader, content_type);
    }

    QNetworkReply  *reply;
    switch(type){
    case GET:
        reply = m_pManager->get(request);
        break;
    case DEL:
        reply = m_pManager->deleteResource(request);
        break;
    case POST:
        reply = m_pManager->post(request,data);
        break;
    case PUT:
        reply = m_pManager->put(request,data);
        break;
    }


    QEventLoop eventLoop;
    QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray ret_data = 0;
    if(reply!=0){
        ret_data = reply->readAll();

        QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        if ( statusCode.isValid() ){
            sts = statusCode.toInt();
        }

        delete reply;
    }
    return ret_data;
}
QByteArray BaseGate::get(const QUrl& url,const QString& content_type){
    int sts;
    return _request(GET,url,content_type,QByteArray(),sts);
}
QByteArray BaseGate::get(const QString& path,const QString& content_type){
    int sts;
    return _request(GET,path,content_type,QByteArray(),sts);
//    QNetworkReply  *reply = m_pManager->get(QNetworkRequest(QUrl(m_url+f)));
//    QEventLoop eventLoop;
//    QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
//    eventLoop.exec();
//    QByteArray data = 0;
//    if(reply!=0){
//        data = reply->readAll();
//        delete reply;
//    }
//    return data;
}
QByteArray BaseGate::del(const QString& path,const QString& content_type){
    int sts;
    return _request(DEL,path,content_type,QByteArray(),sts);
//    QNetworkReply  *reply = m_pManager->deleteResource(QNetworkRequest(QUrl(m_url+f)));
//    QEventLoop eventLoop;
//    QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
//    eventLoop.exec();
//    QByteArray data = 0;
//    if(reply!=0){
//        data = reply->readAll();
//        delete reply;
//    }
//    return data;
}
QByteArray BaseGate::post(const QString &path, const QString &content_type, const QByteArray &data){
    int sts;
    return _request(POST,path,content_type,data,sts);
}
QByteArray BaseGate::put(const QString& path,const QString& content_type, const QByteArray& data){
    int sts;
    return _request(PUT,path,content_type,data,sts);
//    QNetworkReply  *reply = m_pManager->put(QNetworkRequest(QUrl(m_url+f)),data);
//    QEventLoop eventLoop;
//    QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
//    eventLoop.exec();
//    QByteArray data = 0;
//    if(reply!=0){
//        data = reply->readAll();
//        delete reply;
//    }
//    return data;
}
