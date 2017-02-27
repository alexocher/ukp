#include "../HeadersInt/StringConsts.h"
#include "../Headers/EM_BDF.h"
#include  <QCoreApplication>
// -----------------------------------------------------------------------------------------------------
BDF::Asset::Content::CONTENT_TYPE getTypeString(const QString& type){
    BDF::Asset::Content::CONTENT_TYPE ret = BDF::Asset::Content::ctUNDEF;
    if(type.compare(Consts::_ctSEA_CHART)==0){
        ret = BDF::Asset::Content::ctSEA_CHART;
    }else if(type.compare(Consts::_ctBOOK)==0){
        ret = BDF::Asset::Content::ctBOOK;
    }else if(type.compare(Consts::_ctREPORT)==0){
        ret = BDF::Asset::Content::ctREPORT;
    }else if(type.compare(Consts::_ctHYDRO_SHEET)==0){
        ret = BDF::Asset::Content::ctHYDRO_SHEET;
    }
    return ret;
}
QString getStringType(BDF::Asset::Content::CONTENT_TYPE type){
    QString ret = "";
    switch(type){
    case BDF::Asset::Content::ctSEA_CHART: ret=Consts::_ctSEA_CHART; break;
    case BDF::Asset::Content::ctBOOK: ret=Consts::_ctBOOK; break;
    case BDF::Asset::Content::ctREPORT: ret=Consts::_ctREPORT; break;
    case BDF::Asset::Content::ctHYDRO_SHEET: ret=Consts::_ctHYDRO_SHEET; break;
    default:
        ret = "UNDEF";
    }
    return ret;
}
// -----------------------------------------------------------------------------------------------------
BDF::Asset::Content::Content(){
    _type = ctUNDEF;          // тип объекта
}
BDF::Asset::Content::Content(const QJsonObject& obj, const QString& id, const QString &schema){
    // type
    if(obj.contains(Consts::_frmKeyType)){
        QJsonValue v = obj[Consts::_frmKeyType];
        if(!v.isNull() && v.isString()){
            _type = getTypeString(v.toString());
        }else{
            _type = ctUNDEF;
            qDebug()<<"Error parse 'type' in asset id: "<<id;
        }
    }else{
        _type = ctUNDEF;
        qDebug()<<"Asset id: "<<id<<" without 'type'";
    }
    // title
    if(obj.contains(Consts::_frmKeyTitle)){
        QJsonValue v = obj[Consts::_frmKeyTitle];
        if(!v.isNull() && v.isString()){
            _title = v.toString();
        }else{
            qDebug()<<"Error parse 'title' in asset id: "<<id;
        }
    }else{
        qDebug()<<"Asset id: "<<id<<" without 'title'";
    }
    // docs
    if(obj.contains(Consts::_frmKeyDocuments)){
        QJsonValue v = obj[Consts::_frmKeyDocuments];
        if(!v.isNull() && v.isArray()){
            foreach (QJsonValue iter, v.toArray()) {
                if(iter.isString()){
                    _docs<<iter.toString();
                }
            }
        }else{
            qDebug()<<"Error parse 'documents' in asset id: "<<id;
        }
    }else{
        qDebug()<<"Asset id: "<<id<<" without 'documents'";
    }
    // DATE
    if(obj.contains(Consts::_frmKeyDate)){
        QJsonValue v = obj[Consts::_frmKeyDate];
        if(!v.isNull() && v.isObject()){
            QJsonObject o = v.toObject();
            if(o.contains(Consts::_frmKeyPublication)){
                try{
                    QString st = (o[Consts::_frmKeyPublication]).toString();
                    QDate date = QDate::fromString(st, "yyyy-MM-dd");
                    _datemap[dtPUBLICATION] = QDateTime(date);
                }catch(...){
                    qDebug()<<"Error parse date publication in asset id: "<<id;
                }
            }
        }else{
            qDebug()<<"Error parse 'date' in asset id: "<<id;
        }
    }else{
        qDebug()<<"Asset id: "<<id<<" without 'date'";
    }
}
BDF::Asset::Content::~Content(){;}
BDF::Asset::Content::Content(const Content& obj){
    _type = obj._type;
    _docs<<obj._docs;
    _title = obj._title;
    _ident_code = obj._ident_code;
    _datemap = QMap<DATE_TYPE,QDateTime>(obj._datemap);
}
BDF::Asset::Content& BDF::Asset::Content::operator= (Content const& obj){
    Content *ret = new Content();

    ret->_type = obj._type;
    ret->_docs<<obj._docs;
    ret->_title = obj._title;
    ret->_ident_code = obj._ident_code;
    ret->_datemap = QMap<DATE_TYPE,QDateTime>(obj._datemap);

    return *ret;
}

BDF::Asset::Content::CONTENT_TYPE BDF::Asset::Content::getType() const{
    return _type;
}
const QList<QString>& BDF::Asset::Content::getDocs() const{
    return _docs;
}
const QString& BDF::Asset::Content::getTitle() const{
    return _title;
}
const QString& BDF::Asset::Content::getIdentCode() const{
    return _ident_code;
}
const QMap<BDF::Asset::Content::DATE_TYPE,QDateTime>& BDF::Asset::Content::getDate() const{
    return _datemap;
}
// -----------------------------------------------------------------------------------------------------
BDF::Asset::EM_Asset::EM_Asset(const QJsonObject &obj){
    _id = "";
    _coverage = 0;
    _content = 0;
    _interanl_state = BDF::Asset::EM_Asset::SYNC_ASSET;
    _schema = Consts::_valueStrDefault;    
    // id
    if(obj.contains(Consts::_frmKeyId)){
        QJsonValue v = obj[Consts::_frmKeyId];
        if(v.isString()){
            _id = v.toString();
        }else{
            _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
        }
    }else{
        _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
    }
    // покрытие
    if(obj.contains(Consts::_frmKeyCoverage)){
        QJsonValue v = obj[Consts::_frmKeyCoverage];
        if(!v.isNull()){
            Geom::GeometryFactory *gf = new Geom::GeometryFactory();
            try{
                _coverage = gf->createPolygon(obj,true);
            }catch(JSONFormatException::FormatException &e){
                qDebug()<<"Asset: "<<_id<<" - "<<e.getMessage();
            }
        }
    }else{
        _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
    }
    // shema
    if(obj.contains(Consts::_frmKeySchema)){
        QJsonValue v = obj[Consts::_frmKeySchema];
        if(v.isString()){
            _schema = v.toString();
        }else{
            _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
        }
    }else{
        _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
    }
    // content
    if(obj.contains(Consts::_frmKeyContent)){
        QJsonValue v = obj[Consts::_frmKeyContent];
        if(v.isObject()){
            _content = new Content(v.toObject(),_id,_schema);
        }else{
            _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
        }
    }else{
        _interanl_state = BDF::Asset::EM_Asset::BAD_ASSET;
    }

}

BDF::Asset::EM_Asset::~EM_Asset(){
    if(_coverage!=0){
        delete _coverage;
    }   
    if(_content!=0){
        delete _content;
    }
}
BDF::Asset::EM_Asset::INTERNAL_ASSET_STATE BDF::Asset::EM_Asset::internal_state()const{
    return _interanl_state;
}
Geom::Geometry* BDF::Asset::EM_Asset::coverage() const{
    return _coverage;
}
BDF::Asset::Content* BDF::Asset::EM_Asset::content() const{
    return _content;
}
const QString& BDF::Asset::EM_Asset::id() const{
    return _id;
}
// -----------------------------------------------------------------------------------------------------


char* conv(const QString &src){
    std::string _tmp = src.toStdString();
    char* ret = new char [_tmp.size()+1];
    strcpy( ret, _tmp.c_str() );
    return ret;
}
const BDF::Asset::EM_Asset* BDF::Asset::EM_AssetDic::by(const QString& id)const{
    EM_Asset* ret = 0;
    if(_mAsset.contains(id)){
        ret = _mAsset[id];
    }
    return ret;
}
QList<const BDF::Asset::EM_Asset*> BDF::Asset::EM_AssetDic::find(Geom::Polygon* coverage) const{
    QList<const EM_Asset*> ret;

    foreach (EM_Asset* iter, _mAsset.values()) {
        if(iter->coverage()==0 || iter->coverage()->isEmpty() ) continue;
        if(coverage->contains(iter->coverage())){
            ret<<iter;
        }
    }

    return ret;
}
void BDF::Asset::EM_AssetDic::sync(bool store)throw (BDFException::ParseInputJSONException){
    if(!store){
        foreach (EM_Asset* iter, _mAsset.values()) {
            delete iter;
        }
        _mAsset.clear();
        _mAsset = loadAssetsFromBDF();
    }else{
        // TODO: ревлизация сохранения и удаления данных
    }
}
void BDF::Asset::EM_AssetDic::print(){
    foreach (QString iter, _mAsset.keys()) {
        qDebug() << iter;
    }
}

// Загрузка всех данных из БДФ
QMap<QString,BDF::Asset::EM_Asset*> BDF::Asset::EM_AssetDic::loadAssetsFromBDF()throw (BDFException::ParseInputJSONException){
    QMap<QString,BDF::Asset::EM_Asset*> ret;

    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(_mGateBDF->getAssets(), &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains(Consts::_frmKeyAssets)){
                QJsonValue root_v = obj[Consts::_frmKeyAssets];
                if(root_v.isObject()){
                    QJsonObject root_o = root_v.toObject();
                    foreach (QString assetiter_id, root_o.keys()) {
                        EM_Asset* newItem = new  EM_Asset( root_o[assetiter_id].toObject() );
                        if(newItem->internal_state()==BDF::Asset::EM_Asset::BAD_ASSET){
                            qDebug()<<"Error while parse asset with id: "<<assetiter_id;
                        }
                        ret[assetiter_id] = newItem;
                    }
                }else{
                    qDebug()<<"loadAssetsFromBDF: "<<"Root is not object";
                    throw BDFException::ParseInputJSONException();
                }
            }else{
                qDebug()<<"loadAssetsFromBDF: "<<"Not found object - "<<Consts::_frmKeyAssets;
                throw BDFException::ParseInputJSONException();
            }
        }else{
            qDebug()<<"loadAssetsFromBDF: "<<"Input data not object";
            throw BDFException::ParseInputJSONException();
        }
    }else{
        qDebug()<<"loadAssetsFromBDF: "<<parseError.errorString();
        throw BDFException::ParseInputJSONException();
    }

    return ret;
}


// Реализауия методов словаря
BDF::Asset::EM_AssetDic::EM_AssetDic(){
    // Загружаем файл конфигурации, в случае ошибки перем данные по умолчанию
    P_BDF_PROP prop = new BDF_PROP;
    prop->login = 0;
    prop->pass = 0;
    prop->url = 0;

    loadProp(prop);
    //
    _mGateBDF = 0;
    if(prop->login!=0){
        _mGateBDF = new GateBDF(prop->login,prop->pass,prop->url);
    }else{
        qDebug()<<"Error with BDF prop";
    }
}
void BDF::Asset::EM_AssetDic::loadProp(P_BDF_PROP prop){
    if(prop==0) throw CommonException::NullParamException();

    bool bOk = false;
    QString path = QCoreApplication::applicationDirPath() + QDir::separator()+QString(Consts::AssetDicProp);
    QFile file(path);
    if(file.exists()){
        QSettings* settings = new QSettings(path,QSettings::IniFormat);

        bool checkContains = settings->contains(Consts::_KEY_BDF_LOGIN)&settings->contains(Consts::_KEY_BDF_PASS)&settings->contains(Consts::_KEY_BDF_URL);
        if(checkContains){
            prop->login = conv(settings->value(Consts::_KEY_BDF_LOGIN).toString());
            prop->pass = conv(settings->value(Consts::_KEY_BDF_PASS).toString());
            prop->url = conv(settings->value(Consts::_KEY_BDF_URL).toString());

            bOk = true;
        }

        delete settings;
    }

    // все плохо создаем новый
    if(!bOk){
        qDebug()<<"Create default property file: "+path;

        QSettings* settings = new QSettings(path,QSettings::IniFormat);
        settings->setValue(Consts::_KEY_BDF_LOGIN,"archive");
        settings->setValue(Consts::_KEY_BDF_PASS,"yoyHoiHeldun3Ovo");
        settings->setValue(Consts::_KEY_BDF_URL,"archive-dev.krontech.org");
        settings->sync();
        delete settings;
    }



}

void BDF::Asset::EM_AssetDic::initConnectProperties(const QString& login,const QString& password,const QString& url){
    QString path = QCoreApplication::applicationDirPath()+QDir::separator()+QString(Consts::AssetDicProp);
    QSettings* settings = new QSettings(path,QSettings::IniFormat);
    settings->setValue(Consts::_KEY_BDF_LOGIN,login);
    settings->setValue(Consts::_KEY_BDF_PASS,password);
    settings->setValue(Consts::_KEY_BDF_URL,url);
    settings->sync();
    delete settings;
}

bool BDF::Asset::EM_AssetDic::isConnectProperties(){
    P_BDF_PROP prop = new BDF_PROP;
    prop->login = 0;
    prop->pass = 0;
    prop->url = 0;

    loadProp(prop);

    if(prop->login == 0) return false;

    return true;
}

BDF::Asset::EM_AssetDic::~EM_AssetDic(){
    if(_mGateBDF!=0) delete _mGateBDF;
    foreach (EM_Asset* iter, _mAsset.values()) {
        delete iter;
    }
}
bool BDF::Asset::EM_AssetDic::_instflag = false;
BDF::Asset::EM_AssetDic* BDF::Asset::EM_AssetDic::_instance=0;
