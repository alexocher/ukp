#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "GateBDF.h"

#define CONTENT_TYPE_JSON       "application/json"
#define CONTENT_TYPE_FORN_DATA  "multipart/form-data"
#define CONTENT_TYPE_TEXT       "text/plain"
GateBDF::~GateBDF(){}
GateBDF::GateBDF():BaseGate(BaseGate::BDF){
    ;
}
GateBDF::BDF_REQUEST_STATUS_CODE GateBDF::parse_ok(const QByteArray &a){
    BDF_REQUEST_STATUS_CODE ret = BDF_REQUEST_STATUS_CODE_ERROR;
    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(a, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("status")){
                QString ok = obj["status"].toString();
                if(ok.compare("ok")==0){
                    ret = BDF_REQUEST_STATUS_CODE_OK;
                }
            }
        }
    }
    return ret;
}

QByteArray GateBDF::getSchemas(){
    return get("/schemas",CONTENT_TYPE_JSON);
}
QByteArray GateBDF::putSchema(const QByteArray &schema){
    return put("/schemas",CONTENT_TYPE_JSON,schema);
}
QByteArray GateBDF::getSchemaByID(const QString &schema_id){
    return get("/schemas/"+schema_id,CONTENT_TYPE_JSON);
}
QByteArray GateBDF::putSchemaByID(const QString &schema_id, const QByteArray &schema){
    return put("/schemas/"+schema_id,CONTENT_TYPE_JSON,schema);
}
QByteArray GateBDF::delSchemaByID(const QString &schema_id){
    return del("/schemas/"+schema_id,CONTENT_TYPE_JSON);
}
QByteArray GateBDF::getRegions(){
    return get("/regions",CONTENT_TYPE_JSON);
}
QByteArray GateBDF::getAssets(){
    return get("/assets",CONTENT_TYPE_JSON);
}
QByteArray GateBDF::putAssets(const QByteArray& asset){
    return put("/assets",CONTENT_TYPE_JSON,asset);
}
QByteArray GateBDF::getAssetsBySpatialCoverage(const QByteArray& coverage){
    QString param = showURL()+"/assets/spatialcoverage?geojson="+QString(coverage);

    qDebug()<<param;
    QUrl url = QUrl::fromEncoded(param.toUtf8());
    return get(url,CONTENT_TYPE_JSON);
}
QByteArray GateBDF::getAssetsByMetaData(const QString& key, const QString& value){
    return get("/assets/metadata/"+key+"/"+value,CONTENT_TYPE_JSON);
}
QByteArray GateBDF::getAssetByID(const QString &id){
    return get("/assets/"+id,CONTENT_TYPE_JSON);
}
QByteArray GateBDF::putAssetByID(const QString &id, const QByteArray &a){
    return put("/assets/"+id,CONTENT_TYPE_JSON,a);
}
QByteArray GateBDF::delAssetByID(const QString &id){
    return del("/assets/"+id,CONTENT_TYPE_JSON);
}
QByteArray GateBDF::getFilesForAsset(const QString& asset_id){
    return get("/assets/"+asset_id+"/files",CONTENT_TYPE_JSON);
}
QByteArray GateBDF::getDataByID(const QString &asset_id, const QString &data_id){
    return get("/assets/"+asset_id+"/files/"+data_id,CONTENT_TYPE_FORN_DATA);
}
QByteArray GateBDF::postDataByID(const QString &asset_id, const QString &data_id, const QByteArray &data){
    return post("/assets/"+asset_id+"/files/"+data_id,CONTENT_TYPE_FORN_DATA,data);
}
QByteArray GateBDF::delDataByID(const QString& asset_id,const QString& data_id){
    return del("/assets/"+asset_id+"/files/"+data_id,CONTENT_TYPE_JSON);
}
QString GateBDF::getNewID(){
    QString s = get("/new_id",CONTENT_TYPE_TEXT);
    if(s.isEmpty()) return 0;
    return s.mid(1,s.length()-2);
}


GateBDF::BDF_REQUEST_STATUS_CODE GateBDF::_putDataByID(const QString &asset_id, const QString &data_id, const QByteArray &a){
    return parse_ok(postDataByID(asset_id, data_id, a));
}
GateBDF::BDF_REQUEST_STATUS_CODE GateBDF::_putAssetByID(const QString &id, const QByteArray &a){
    return parse_ok(putAssetByID(id,a));
}
GateBDF::BDF_REQUEST_STATUS_CODE GateBDF::_delAssetByID(const QString &id){
    return parse_ok(delAssetByID(id));
}
GateBDF::BDF_REQUEST_STATUS_CODE GateBDF::_delDataByID(const QString& asset_id,const QString& data_id){
    return parse_ok(delDataByID(asset_id,data_id));
}
