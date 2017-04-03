#include "ModelBDF.h"

#include <QDebug>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStack>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>

#include "../DBConnect/BDF/GateBDF.h"

#define KEY_ID          "id"
#define KEY_L10N        "l10n"
#define KEY_REQUIRED    "required"
#define KEY_PROPERTIES  "properties"
#define KEY_DEFINITIONS "definitions"
#define KEY_ITEMS       "items"

#define DEFAULT_LOCALIZATION "ru"

namespace ModelBDF {
namespace Asset {
namespace Consts {
    const char *_UNDEF = "UNDEF";

    const char *_frmKeyId = "_id";
    const char *_frmKeyCoverage = "spatial";
    const char *_frmKeyType = "type";
    const char *_frmKeyFiles = "files";
    const char *_frmKeyCitation = "citation";
    const char *_frmKeyTitle = "title";
    const char *_frmKeyIdentifiers = "identifiers";
    const char *_frmKeyCode = "code";
    const char *_frmKeyDate = "date";
}//Const
}//Asset
}//ModelBDF



ModelBDF::l10n::l10n(){
    ;
}

ModelBDF::l10n::~l10n(){
    QMap<QString,QMap<QString,QString>* >::iterator iter;
    for (iter = _content.begin(); iter!=_content.end() ;++iter) {
        delete (*iter);
    }
}
ModelBDF::l10n *ModelBDF::l10n::Instance(QJsonValue& value){
    if(!value.isObject()) return 0;

    l10n* newObj = new l10n;

    QJsonObject obj = value.toObject();

    foreach (QString key, obj.keys()) {
        QMap<QString,QString> * item = 0;

        QJsonValue v = obj.value(key);
        if(v.isObject()){
            item =  new QMap<QString,QString>();
            QJsonObject o = v.toObject();
            foreach (QString field, o.keys()) {
                QJsonValue local = o.value(field);
                if(local.isString()){
                    (*item)[field] = local.toString();
                }else if(local.isArray()){
                    int i = 1;
                    foreach (QJsonValue it, local.toArray()) {
                        if(it.isString()){
                            (*item)[field+"_"+QString::number(i++)] = it.toString();
                        }else{
                            qDebug()<<"Error while parse l10n content for field: "<<key;
                        }
                    }
                }else{
                    qDebug()<<"Error while parse l10n content for field: "<<key;
                }
            }

        }else{
            qDebug()<<"Error while parse l10n content for key: "<<key;
            continue;
        }

        newObj->_content[key] = item;
    }

    return newObj;
}
const QMap<QString, QString> &ModelBDF::l10n::getDefaultLocalization() const throw(CommonException::ObjNotFoundException){
    if(_content.isEmpty()) throw CommonException::ObjNotFoundException();
    if(_content.contains(DEFAULT_LOCALIZATION)) return *(_content[DEFAULT_LOCALIZATION]);

    return *(_content.first());
}
const QMap<QString, QString> &ModelBDF::l10n::getLocalization(const QString &key) const throw(CommonException::ObjNotFoundException){
    if(!_content.contains(key)) throw CommonException::ObjNotFoundException();
    return *(_content[key]);
}
const QList<QString> ModelBDF::l10n::getKeys() const{
    return _content.keys();
}
// =======================================================================================
ModelBDF::Schema::BaseSchemaObject::BaseSchemaObject(const QString &id, ModelBDF::Schema::ITEM_TYPE type):BaseTreeItem(id,type){
    ;
}
QString ModelBDF::Schema::BaseSchemaObject::getFullID(){
    if(getParent()!=0 && getParent()->getType()!=ROOT_ITEM){
        return ((BaseSchemaObject*)getParent())->getFullID()+"/"+getID();
    }
    return getID();
}
// =======================================================================================
ModelBDF::Schema::SchemaObject::SchemaObject(const QString &id):BaseSchemaObject(id,ModelBDF::Schema::OBJECT_ITEM){
    _localization = 0;
}
ModelBDF::Schema::SchemaObject::~SchemaObject(){
    if(_localization!=0){
        delete _localization;
    }
}
QMap<QString,QString> ModelBDF::Schema::SchemaObject::getPropType(const QString& local){
    QMap<QString,QString> ret;

    TREE_LIST::iterator iter;
    for(iter = begin();iter!=end();++iter){
        // в чилдах ищем объект type
        if( (*iter)->getID().compare("type")==0){
            SchemaObject* item = (SchemaObject*)(*iter);

            QString key;
            QString val;
            if(item->_enums.contains("enum") && !item->_enums["enum"].isEmpty() ){
                key = item->_enums["enum"].at(0);
            }
            if(item->_localization!=0){
                const QMap<QString,QString>& l = item->_localization->getLocalization(local);
                if(l.contains("enum-title_1")){
                    val = l["enum-title_1"];
                }
            }
            if(!key.isNull() && !val.isNull()){
                ret[key] = val;
            }
            break;
        }
    }

    return ret;
}
// =======================================================================================
ModelBDF::Schema::SchemaDic::SchemaDic():BaseTree(new BaseSchemaObject("ROOT_ITEM",ModelBDF::Schema::ROOT_ITEM)){

}

ModelBDF::Schema::SchemaDic::~SchemaDic(){
    ;
}
ModelBDF::Schema::SchemaObject* ModelBDF::Schema::SchemaDic::findEvristicSchema(const QString& value){
    SchemaObject* ret =0 ;

    QString fake = "/"+value+"#";
    if(_schema_cache.contains(fake)){
        ret = (SchemaObject*)_schema_cache[fake];
    }else{
        QList<BaseTreeItem<QString,ITEM_TYPE>*>::iterator iter;
        for(iter = getRoot()->begin();iter!=getRoot()->end();++iter) {
            const QString &id = ((SchemaObject*)(*iter))->getID();
            int pos = id.indexOf(fake);
            if(pos>0){
                ret = ((SchemaObject*)(*iter));
                _schema_cache[fake] = ret;
                break;
            }
        }
    }
    return ret;
}
ModelBDF::Schema::SchemaObject* ModelBDF::Schema::SchemaDic::findEvristicSchema_v2(const QString& value){
    SchemaObject* ret =0 ;
    if(_schema_cache.contains(value)){
        ret = (SchemaObject*)_schema_cache[value];
    }else{
        TREE_LIST::iterator iter;
        for(iter = getRoot()->begin();iter!=getRoot()->end();++iter) {
            SchemaObject* item = ((SchemaObject*)(*iter));
            QMap<QString,QString> mType = item->getPropType(DEFAULT_LOCALIZATION);
            if(mType.contains(value)){
                ret = ((SchemaObject*)(*iter));
                _schema_cache[value] = ret;
                break;
            }
        }
    }
    return ret;
}
const ModelBDF::Schema::BaseSchemaObject *ModelBDF::Schema::SchemaDic::findTopLevel(const QString& schema_id) const{
    BaseSchemaObject* ret =0 ;
    QList<BaseTreeItem<QString,ITEM_TYPE>*>::iterator iter;
    for(iter = getRoot()->begin();iter!=getRoot()->end();++iter) {
        const QString &id = ((BaseSchemaObject*)(*iter))->getID();
        if(id.compare(schema_id)==0){
            ret = ((BaseSchemaObject*)(*iter));
            break;
        }
    }
    return ret;
}
const ModelBDF::Schema::BaseSchemaObject* ModelBDF::Schema::SchemaDic::by(const QString& schema_id,bool short_id ){
    BaseSchemaObject* ret = 0;
    if(_cache.contains(schema_id)){
        ret = _cache[schema_id];
    }else{
        foreach (QString key, _cache.keys()) {
            BaseSchemaObject* item = _cache[key];
            QString id;
            if(short_id){
                id = item->getID();
            }else{
                id = item->getFullID();
            }
            if(id.compare(schema_id)==0){
                ret = item;
                _cache[schema_id] = item;
                break;
            }
        }
    }
    return ret;
}
void ModelBDF::Schema::SchemaDic::load(){
    GateBDF *gate = new GateBDF();
    QByteArray data = gate->getSchemas();
    delete gate;
    parse(data);
}
void ModelBDF::Schema::SchemaDic::load(const QString& file_path)throw (CommonException::FileNotFoundException,CommonException::FileOpenException){
    QFile f(file_path);
    if(!f.exists()){
        throw CommonException::FileNotFoundException(file_path);
    }
    if(!f.open(QFile::ReadOnly)){
        throw CommonException::FileOpenException(file_path,f.errorString());
    }
    QByteArray data = f.readAll();
    f.close();
    parse(data);
}
ModelBDF::Schema::BaseSchemaObject *ModelBDF::Schema::SchemaDic::parse(QJsonObject& obj, const QString key_obj){

    QString id;
    if(obj.contains(KEY_ID)){
        QJsonValue v = obj.value(KEY_ID);
        if(v.isString()){
            id = v.toString();
        }else{
            qDebug()<<"Wrong object format: field <id> not string - skip";
            return 0;
        }
    }else{
        id = key_obj;
    }


    SchemaObject *ret = new SchemaObject(id);

    foreach (QString it, obj.keys()) {
        QJsonValue v = obj.value(it);
        if(it.compare(KEY_ID)==0){
            continue;
        }else if(it.compare(KEY_L10N)==0){
            ret->_localization = l10n::Instance(v);
        }else if(it.compare(KEY_REQUIRED)==0){
            if(v.isArray()){
                foreach (QJsonValue it_require, v.toArray()) {
                    if(it_require.isString()){
                        ret->_required<<it_require.toString();
                    }else{
                        qDebug()<<"Wrong object format: element <required> not string - skip "<<id;
                    }
                }
            }else{
                qDebug()<<"Wrong object format: field <required> not object - skip "<<id;
            }
        }else if(it.compare(KEY_DEFINITIONS)==0 || it.compare(KEY_PROPERTIES)==0){
            QString sufix = "";
            if(it.compare(KEY_DEFINITIONS)==0){
                sufix = it+"/";
            }
            if(v.isObject()){
                QJsonObject o = v.toObject();
                foreach (QString sub_key, o.keys()) {
                    QJsonValue sub_item = o.value(sub_key);
                    if(sub_item.isObject()){
                        QJsonObject sub_obj = sub_item.toObject();
                        BaseSchemaObject* child = parse(sub_obj,sufix+sub_key);
                        if(child!=0){
                            add(ret,child);
                        }else{
                            qDebug()<<"Wrong parse child - skip "<<id;
                        }
                    }else{
                        qDebug()<<"Wrong object format: item <prop OR defines> not object - skip "<<id;
                    }
                }
            }else{
                qDebug()<<"Wrong object format: field <prop OR defines> not object - skip "<<id;
            }
        }else{
            if(v.isString()){
                ret->_fields[it] = v.toString();
            }else if(v.isArray()){
                foreach (QJsonValue enums, v.toArray()) {
                    if(enums.isString()){
                        ret->_enums[it]<<enums.toString();
                    }else if(enums.isObject()){
                        QJsonObject sub_obj = enums.toObject();
                        BaseSchemaObject* child = parse(sub_obj,it);
                        if(child!=0){
                            add(ret,child);
                        }else{
                            qDebug()<<"Wrong parse child - skip "<<it;
                        }
                    }else{
                        qDebug()<<"Wrong object format: element <enums> not string - skip "<<id;
                    }
                }
            }else if(v.isObject()){
                QJsonObject sub_obj = v.toObject();
                BaseSchemaObject* child = parse(sub_obj,it);
                if(child!=0){
                    add(ret,child);
                }else{
                    qDebug()<<"Wrong parse child - skip "<<id;
                }
            }else{

            }
        }

    }
    return ret;
}

void ModelBDF::Schema::SchemaDic::parse(const QByteArray &data)throw (JSONFormatException::FormatException){
    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject o = doc.object();
            if(o.contains(KEY_ITEMS)){
                QJsonValue items = o[KEY_ITEMS];
                if(items.isArray()){
                    QJsonArray top_level = items.toArray();
                    int i = 0;
                    foreach (QJsonValue iter, top_level) {
                        if(!iter.isObject()){
                            qDebug()<<"Skip item, wrong object ("<<QString::number(i++)<<")";
                            continue;
                        }
                        QJsonObject obj = iter.toObject();
                        BaseSchemaObject* item = parse(obj,QString());
                        if(item==0){
                            qDebug()<<"Skip item, error while parse ("<<QString::number(i++)<<")";
                            continue;
                        }
                        add(getRoot(),item);
                    }
                }else{
                    throw JSONFormatException::FormatException("Wrong items type (not array)");
                }
            }else{
                throw JSONFormatException::FormatException("Not found items");
            }
        }else{
            throw JSONFormatException::FormatException("Wrong root item type (not object)");
        }
    }else{
        throw JSONFormatException::FormatException(parseError.errorString());
    }
}

void ModelBDF::Schema::SchemaDic::showKeys() const{
    qDebug()<<"Map keys:";
    foreach (QString iter, m_Map.keys()) {
        qDebug()<<iter;
    }
}
void ModelBDF::Schema::SchemaDic::showTopLevel() const{
    typedef  QList<BaseTreeItem<QString,ITEM_TYPE>*> ITER;
    ITER::iterator iter;
    for(iter = getRoot()->begin();iter!=getRoot()->end();++iter) {
        qDebug()<<((BaseSchemaObject*)(*iter))->getID();
    }
}
void ModelBDF::Schema::SchemaDic::printSchemaTree(bool idonly) const{
    qDebug()<<"Schama Tree";
    QStack<BaseSchemaObject*> stack;
    stack.push( (BaseSchemaObject*)getRoot() );
    while(!stack.isEmpty()){
        BaseSchemaObject* cur = stack.pop();


        QString tab = "";
        QString ret = "";
        for(int i=1;i<cur->getLevel();i++){
            ret+="----";
            tab+="    ";
        }
        if(idonly){
            ret+=cur->getID()+" ("+cur->getFullID()+")";
        }else{
            tab+="  ";
            ret+="("+QString::number(cur->getLevel())+") ID: "+cur->getID()+"\r\n";
            if(cur->getType()==ModelBDF::Schema::OBJECT_ITEM){
                SchemaObject* obj = (SchemaObject*)cur;
                if(obj->_localization!=0){
                    try{
                        QMap<QString,QString> loc = obj->_localization->getDefaultLocalization();
                        foreach (QString key, loc.keys()) {
                            ret+=tab+key+": "+loc[key]+"\r\n";
                        }
                    }catch(CommonException::ObjNotFoundException &e){

                    }
                }
                ret+="\r\n";
                foreach (QString fields, obj->_fields.keys()) {
                    ret+=tab+fields+": "+obj->_fields[fields]+"\r\n";
                }
                ret+="\r\n";
                foreach (QString fields, obj->_enums.keys()) {
                    ret+=tab+fields+":";
                    foreach (QString val, obj->_enums[fields]) {
                        ret+=val+", ";
                    }
                    ret+="\r\n";
                }
            }
        }
        qDebug().noquote()<<ret;
        for(QList<BaseTreeItem<QString,ModelBDF::Schema::ITEM_TYPE>*>::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(  (BaseSchemaObject*)(*iter) );
        }
    }
}
QByteArray ModelBDF::Schema::SchemaDic::serialize() const{
    qWarning()<<"SchemaDic: UNSUPPORTED OPERATION";
    return QByteArray();
}
int ModelBDF::Schema::SchemaDic::deserialize(const QByteArray& data){
    if(data.isEmpty()){;}
    qWarning()<<"SchemaDic: UNSUPPORTED OPERATION";
    return -1;
}
// =======================================================================================
ModelBDF::Asset::Citation::Citation(){
    _title = Consts::_UNDEF;
}
ModelBDF::Asset::Citation* ModelBDF::Asset::Citation::create(const QJsonObject&data, Schema::SchemaObject* schema  ){
    if(schema==0){;}
    Citation* ret = new Citation();
    if(data.contains(Consts::_frmKeyCitation)){
        QJsonValue valueCitation = data[Consts::_frmKeyCitation];
        if(valueCitation.isObject()){
            QJsonObject objCitation = valueCitation.toObject();

            // TITLE
            if(objCitation.contains(Consts::_frmKeyTitle)){
                QJsonValue v = objCitation[Consts::_frmKeyTitle];
                if(v.isString()){
                    ret->_title =  v.toString();
                }else{
                    qWarning()<<"Citation property "<<Consts::_frmKeyTitle<<" is not string";
                }
            }else{
                qWarning()<<"Citation is not contain "<<Consts::_frmKeyTitle;
            }
            // IDENTIFIERS
            if(objCitation.contains(Consts::_frmKeyIdentifiers)){
                QJsonValue v = objCitation[Consts::_frmKeyIdentifiers];
                if(v.isArray()){
                    foreach (QJsonValue it, v.toArray()) {
                        try{
                            ret->_code <<  it.toObject()[Consts::_frmKeyCode].toString();
                        }catch(...){
                            qWarning()<<"Something wrong Citation property "<<Consts::_frmKeyIdentifiers;
                        }
                    }
                }else{
                    qWarning()<<"Citation property "<<Consts::_frmKeyIdentifiers<<" is not Array";
                }
            }else{
                qWarning()<<"Citation is not contain "<<Consts::_frmKeyIdentifiers;
            }
            // DATES
            if(objCitation.contains(Consts::_frmKeyDate)){
                QJsonValue v = objCitation[Consts::_frmKeyDate];
                if(v.isObject()){
                    QJsonObject obj = v.toObject();
                    foreach (QString it, obj.keys()) {
                        try{
                            ret->_dates[it] = obj[it].toString();
                        }catch(...){
                            qWarning()<<"Something wrong Citation property "<<Consts::_frmKeyDate;
                        }
                    }
                }else{
                    qWarning()<<"Citation property "<<Consts::_frmKeyDate<<" is not Object";
                }
            }else{
                qWarning()<<"Citation is not contain "<<Consts::_frmKeyDate;
            }
        }else{
            qWarning()<<"Citation is not object";
        }
    }
    return ret;
}
ModelBDF::Asset::Citation::~Citation(){
    ;
}
const QString& ModelBDF::Asset::Citation::getTitle() const{
    return _title;
}
const QStringList &ModelBDF::Asset::Citation::getCode() const{
    return _code;
}
const QMap<QString, QString> &ModelBDF::Asset::Citation::getDates() const{
    return _dates;
}
// =======================================================================================
ModelBDF::Asset::Content::Content(){
    _state = icsOK;
    _citation = 0;
}

ModelBDF::Asset::Content *ModelBDF::Asset::Content::create(const QJsonObject &data,ModelBDF::Schema::SchemaObject *schema){
    Content * ret = new Content();
    // Пытаемся получить тип и название схемы
    QMap<QString,QString> mtype = schema->getPropType(DEFAULT_LOCALIZATION);
    if(!mtype.isEmpty()){
        ret->_type = mtype.firstKey();
        ret->_title_type = mtype.first();
    }else{
        qDebug()<<"Schema type not found";
    }
    // Разгребаем набор файлов при его наличие (!?! в схеме нет)
    if(data.contains(Consts::_frmKeyFiles)){
        QJsonValue v = data[Consts::_frmKeyFiles];
        if(v.isArray()){
            foreach (QJsonValue it, v.toArray()) {
                if(it.isString()){
                    ret->_files<<it.toString();
                }else{
                    qWarning()<<"Files property contains not string";
                }
            }
        }else{
            qWarning()<<"Wrong files property format";
        }
    }else{
        qDebug()<<"Asset without files properties";
    }

    ret->_citation = Citation::create(data,schema);


    return ret;
}

ModelBDF::Asset::Content::~Content(){
    if(_citation != 0) delete _citation;
}

const QString &ModelBDF::Asset::Content::getType() const{
    return _type;
}
const QString &ModelBDF::Asset::Content::getTitleType() const{
    return _title_type;
}
const QStringList& ModelBDF::Asset::Content::getFiles() const{
    return _files;
}
const ModelBDF::Asset::Citation* ModelBDF::Asset::Content::getCitation() const{
    return _citation;
}
// =======================================================================================
ModelBDF::Asset::EM_Asset::EM_Asset(const QJsonObject &obj, QSharedPointer<Schema::SchemaDic> dic){
    _id = "";
    _coverage = 0;
    _content = 0;
    _interanl_state = EM_Asset::SYNC_ASSET;
    _schema = "";
    // id
    if(obj.contains(Consts::_frmKeyId)){
        QJsonValue v = obj[Consts::_frmKeyId];
        if(v.isString()){
            _id = v.toString();
        }else{
            _interanl_state = EM_Asset::BAD_ASSET;
        }
    }else{
        _interanl_state = EM_Asset::BAD_ASSET;
    }
    // покрытие
    if(obj.contains(Consts::_frmKeyCoverage)){
        QJsonValue v = obj[Consts::_frmKeyCoverage];
        if(v.isObject()){
            Geom::GeometryFactory *gf = new Geom::GeometryFactory();
            try{
                _coverage = gf->createPolygon(v.toObject());
            }catch(JSONFormatException::FormatException &e){
                qDebug()<<"Asset: "<<_id<<" - "<<e.getMessage();
            }
        }else{
            _interanl_state = EM_Asset::BAD_ASSET;
            qDebug()<<"Asset: "<<_id<<" - not conteins object "<< Consts::_frmKeyCoverage;
        }
    }else{
        _interanl_state = EM_Asset::BAD_ASSET;
        qDebug()<<"Asset: "<<_id<<" - not conteins key "<< Consts::_frmKeyCoverage;
    }
    // парсим контент при наличие словаря схем и найденной схемы
    if(!dic.isNull()){
        Schema::SchemaObject* schema = 0;
        // пытаемся получить схему
        if(obj.contains(Consts::_frmKeyType)){
            QJsonValue v = obj[Consts::_frmKeyType];
            if(v.isString()){
                schema = dic->findEvristicSchema_v2(v.toString());
            }else{
                _interanl_state = EM_Asset::BAD_ASSET;
                qDebug()<<"Asset: "<<_id<<" - not conteins string "<< Consts::_frmKeyType;
            }
        }else{
            _interanl_state = EM_Asset::BAD_ASSET;
            qDebug()<<"Asset: "<<_id<<" - not conteins key "<< Consts::_frmKeyType;
        }

        if(schema!=0){
            _schema = schema->getID();
            _content = Content::create(obj,schema);
        }else{
            qDebug()<<"Schema not found for asset with id: "<<_id;
            _interanl_state = EM_Asset::BAD_ASSET;
        }
    }else{
        qDebug()<<"Schema dic is null";
        _interanl_state = EM_Asset::BAD_ASSET;
    }

}

ModelBDF::Asset::EM_Asset::~EM_Asset(){
    if(_coverage!=0){
        delete _coverage;
    }
    if(_content!=0){
        delete _content;
    }
}
ModelBDF::Asset::EM_Asset::INTERNAL_ASSET_STATE ModelBDF::Asset::EM_Asset::internal_state()const{
    return _interanl_state;
}
Geom::Geometry* ModelBDF::Asset::EM_Asset::coverage() const{
    return _coverage;
}
ModelBDF::Asset::Content* ModelBDF::Asset::EM_Asset::content() const{
    return _content;
}
const QString& ModelBDF::Asset::EM_Asset::id() const{
    return _id;
}
// =======================================================================================
bool ModelBDF::Asset::EM_AssetDic::_instflag = false;
ModelBDF::Asset::EM_AssetDic* ModelBDF::Asset::EM_AssetDic::_instance = 0;

const ModelBDF::Asset::EM_Asset* ModelBDF::Asset::EM_AssetDic::by(const QString& id)const{
    EM_Asset* ret = 0;
    if(_mAsset.contains(id)){
        ret = _mAsset[id];
    }
    return ret;
}
QList<const ModelBDF::Asset::EM_Asset*> ModelBDF::Asset::EM_AssetDic::find(Geom::Polygon* coverage) const{
    QList<const EM_Asset*> ret;

    foreach (EM_Asset* iter, _mAsset.values()) {
        if(iter->coverage()==0 || iter->coverage()->isEmpty() ) continue;
        if(coverage->contains(iter->coverage())){
            ret<<iter;
        }
    }

    return ret;
}
void ModelBDF::Asset::EM_AssetDic::loadAssetsFromFile(const QString& filename, QSharedPointer<Schema::SchemaDic> dic) throw (BDFException::ParseInputJSONException){
    foreach (EM_Asset* iter, _mAsset.values()) {
        delete iter;
    }
    _mAsset.clear();
    QFile f(filename);
    if(f.exists()){

        f.open(QIODevice::ReadOnly);

        _mAsset = parseAssets(f.readAll(),dic);

        f.close();
    }else{
        qDebug()<<"File <"<<filename<<"> not exist";
    }
}
void ModelBDF::Asset::EM_AssetDic::sync(QSharedPointer<Schema::SchemaDic> dic, bool store)throw (BDFException::ParseInputJSONException){
    if(!store){
        foreach (EM_Asset* iter, _mAsset.values()) {
            delete iter;
        }
        _mAsset.clear();
        _mAsset = loadAssetsFromBDF(dic);
    }else{
        // TODO: ревлизация сохранения и удаления данных
    }
}
void ModelBDF::Asset::EM_AssetDic::print(){
    foreach (QString iter, _mAsset.keys()) {
        EM_Asset* a = _mAsset[iter];
        //qDebug() << a->id() << " [files: "<<a->content()->getFiles().join(", ")<<"]";
        const Citation* cit = a->content()->getCitation();
        qDebug() << cit->getTitle()<<"[publication: "<<cit->getDates()["publication"]<<"]" ;
    }
}

// Загрузка всех данных из БДФ
QMap<QString,ModelBDF::Asset::EM_Asset*> ModelBDF::Asset::EM_AssetDic::loadAssetsFromBDF(QSharedPointer<Schema::SchemaDic> dic)throw (BDFException::ParseInputJSONException){
    QSharedPointer<GateBDF> gate =  QSharedPointer<GateBDF>(new GateBDF());

    return parseAssets(gate->getAssets(),dic);
}

QMap<QString,ModelBDF::Asset::EM_Asset*> ModelBDF::Asset::EM_AssetDic::parseAssets(QByteArray data,QSharedPointer<Schema::SchemaDic> dic)throw (BDFException::ParseInputJSONException){
    QMap<QString,EM_Asset*> ret;

    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject o = doc.object();
            if(o.contains(KEY_ITEMS)){
                QJsonValue items = o[KEY_ITEMS];
                if(items.isArray()){
                    QJsonArray arr = items.toArray();
                    int count = 0;

                    if(dic.isNull()){
                        dic = QSharedPointer<Schema::SchemaDic>(new Schema::SchemaDic());
                        dic->load();
                    }

                    foreach (QJsonValue asset_data, arr) {
                        if(asset_data.isObject()){
                            EM_Asset* newItem = new  EM_Asset( asset_data.toObject(), dic );
                            if(newItem->internal_state()==ModelBDF::Asset::EM_Asset::BAD_ASSET){
                                qDebug()<<"Error while parse asset #: "<<QString::number(count);
                            }
                            ret[newItem->id()] = newItem;
                        }else{
                            qDebug()<<"Asset data #: "<<QString::number(count)<<" wrong";
                        }
                        count++;
                    }
                }else{
                    qDebug()<<"loadAssetsFromBDF: "<<"Input items data not array";
                    throw BDFException::ParseInputJSONException();
                }
            }else{
                qDebug()<<"loadAssetsFromBDF: "<<"Input data not contains items ";
                throw BDFException::ParseInputJSONException();
            }
        }else{
            qDebug()<<"loadAssetsFromBDF: "<<"Input root data not object";
            throw BDFException::ParseInputJSONException();
        }
    }else{
        qDebug()<<"loadAssetsFromBDF: "<<parseError.errorString();
        throw BDFException::ParseInputJSONException();
    }

    return ret;
}

ModelBDF::Asset::EM_AssetDic *ModelBDF::Asset::EM_AssetDic::Instance(){
    if(! _instflag){
        _instance = new EM_AssetDic();
        _instflag = true;
    }
    return _instance;
}


// Реализауия методов словаря
ModelBDF::Asset::EM_AssetDic::EM_AssetDic(){
}

ModelBDF::Asset::EM_AssetDic::~EM_AssetDic(){
    foreach (EM_Asset* iter, _mAsset.values()) {
        delete iter;
    }
}
// ===========================================================================================================
ModelBDF::Region::EM_RegionItem::EM_RegionItem(const QString id, const QString& type, const QString& name, const QString& url):BaseTreeItem(id,-1){
    _type = type; // группирование
    _name = name;
    _url = url;
}
ModelBDF::Region::EM_RegionItem::EM_RegionItem(const QString id):BaseTreeItem(id,-1){

}
const QString& ModelBDF::Region::EM_RegionItem::getType() const{
    return _type;
}
const QString& ModelBDF::Region::EM_RegionItem::getName() const{
    return _name;
}
const QString& ModelBDF::Region::EM_RegionItem::getURL() const{
    return _url;
}
ModelBDF::Region::EM_RegionItem::~EM_RegionItem(){
    ;
}
// ===========================================================================================================
ModelBDF::Region::EM_Regions::EM_Regions():BaseTree(0){
    ;
}
ModelBDF::Region::EM_Regions::~EM_Regions(){
    ;
}
void ModelBDF::Region::EM_Regions::parse(const QByteArray& data)throw (JSONFormatException::FormatException){
    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isArray()){
            QJsonArray top_level = doc.array();
            int i = 0;
            EM_RegionItem* rootitem = new EM_RegionItem("-000000000000000000","root","root","/");
            setRootItem(rootitem);

            QMap<QString,EM_RegionItem*> mTree;
            EM_RegionItem* item = 0;
            EM_RegionItem* pitem = 0;
            foreach (QJsonValue iter, top_level) {
                if(!iter.isObject()){
                    qDebug()<<"Skip item, wrong object ("<<QString::number(i++)<<")";
                    continue;
                }
                QJsonObject obj = iter.toObject();
                // --------
                item = 0;
                pitem = 0;
                try{
                    QString type = obj["type"].toString();
                    QString name = obj["name"].toString();
                    QString url = obj["_url"].toString();
                    QString id = obj["_id"].toString();
                    QString pid;
                    bool is_top = true;
                    if(obj.contains("parentId")){
                        pid = obj["parentId"].toString();
                        is_top = false;
                    }
                    if(mTree.contains(id)){
                        item = mTree[id];
                        item->_name = name;
                        item->_type = type;
                        item->_url = url;
                    }else{
                        item = new EM_RegionItem(id, type, name, url);
                        mTree[id] = item;
                    }


                    if(_mapType.contains(type)){
                        _mapType[type]<<item;
                    }else{
                        _mapType[type] = QList<EM_RegionItem*>();
                        _mapType[type]<<item;
                        _types<<type;
                    }

                    if(is_top){
                        pitem = rootitem;
                    }else{
                        if(mTree.contains(pid)){
                            pitem = mTree[pid];
                        }else{
                            pitem = new EM_RegionItem(pid);
                            mTree[pid] = pitem;
                        }
                    }


                    //add(pitem,item);
                }catch(...){
                    qDebug()<<"Skip item, error while parse ("<<QString::number(i++)<<")";
                    continue;
                }

                // ---------

                add(pitem,item);
            }
        }else{
            throw JSONFormatException::FormatException("Wrong root item type (not array)");
        }
    }else{
        throw JSONFormatException::FormatException(parseError.errorString());
    }
}
QList<ModelBDF::Region::EM_RegionItem*>& ModelBDF::Region::EM_Regions::byType(const QString& type)throw (CommonException::ObjNotFoundException){
    if(!_mapType.contains(type)) throw CommonException::ObjNotFoundException();
    return _mapType[type];
}
QList<QString> ModelBDF::Region::EM_Regions::types(){
    return _mapType.keys();
}
