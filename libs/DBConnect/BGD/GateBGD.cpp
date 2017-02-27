#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "GateBGD.h"

#define CONTENT_TYPE_JSON       "application/json"

GateBGD::~GateBGD(){
    ;
}
GateBGD::GateBGD():BaseGate(BaseGate::BGD){
    ;
}

GateBGD::BGD_REQUEST_STATUS_CODE GateBGD::parse_ok(const QByteArray &a){
    BGD_REQUEST_STATUS_CODE ret = BGD_REQUEST_STATUS_CODE_ERROR;
    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(a, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("status")){
                QString ok = obj["status"].toString();
                if(ok.compare("ok")==0){
                    ret = BGD_REQUEST_STATUS_CODE_OK;
                }
            }
        }
    }
    return ret;
}


QByteArray GateBGD::version(){
    return get("",CONTENT_TYPE_JSON);
}
QByteArray GateBGD::branches(){
    return get("/branches",CONTENT_TYPE_JSON);
}
QByteArray GateBGD::branch(const QString& _branch){
    return get("/branch/"+_branch,CONTENT_TYPE_JSON);
}
QByteArray GateBGD::branchContent(const QString& _branch){
    return get("/branch/"+_branch+"/content",CONTENT_TYPE_JSON);
}
QByteArray GateBGD::mainBranch(){
    return branch("main");
}
QByteArray GateBGD::mainBranchContent(){
    return branchContent("main");
}
QByteArray GateBGD::commit(const QByteArray& data){
    return post("/commit",CONTENT_TYPE_JSON,data);
}
QByteArray GateBGD::log(const QString& _branch){
    return get("/branch/"+_branch+"/log",CONTENT_TYPE_JSON);
}
QByteArray GateBGD::newBranch(const QString& _branch){
    QString data = "{\"branch\":\""+_branch+"\"}";
    return put("/branch/"+_branch+"/main",CONTENT_TYPE_JSON,data.toUtf8());
}
QByteArray GateBGD::delBranch(const QString& _branch){
    return del("/branch/"+_branch,CONTENT_TYPE_JSON);
}
QByteArray GateBGD::diff(const QString& _branch1,const QString& _branch2){
    return get("/diff?from="+_branch1+"&to="+_branch2,CONTENT_TYPE_JSON);
}
QByteArray GateBGD::diff(const QString& _branch){
    return diff("main",_branch);
}
