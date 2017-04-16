#include "../Headers/EM_ExtProc.h"
#include <QtSql>
#include "../DBConnect/dbconnect.h"

#define query_SelectModule "SELECT suid, title, path, descr, is_system FROM plan.ext_module_dic "
#define query_SelectLink    "SELECT proc_id, module_id FROM plan.ext_proc_link_module "
#define query_SelectLinkTest "SELECT proc_id, module_id FROM plan.ext_proc_link_module WHERE proc_id=:proc_id AND module_id=:module_id"
#define query_DeleteLink    "DELETE FROM plan.ext_proc_link_module WHERE proc_id=:proc_id AND module_id=:module_id"
#define query_InsertLink    "INSERT INTO plan.ext_proc_link_module (proc_id, module_id) VALUES (:proc_id, :module_id) "

#define query_InsertModule "INSERT INTO plan.ext_module_dic (title, path, descr, is_system) VALUES (:title, :path, :descr, :is_system) RETURNING suid"
#define query_DeleteModule "DELETE FROM plan.ext_module_dic WHERE suid=:suid"
#define query_DeleteModuleAll "DELETE FROM plan.ext_module_dic "
#define query_UpdateModule "UPDATE plan.ext_module_dic SET title=:title, path=:path, descr=:descr, is_system=:is_system WHERE suid=:suid"

#define query_Select    "SELECT " \
                        " id, title, ext_modules " \
                        "FROM plan.ext_proc_dic "
#ifdef NEW_VERSION
#define query_Insert    "INSERT INTO plan.ext_module_dic (title) " \
                        "VALUES " \
                        "(:title)" \
                        "RETURNING suid"
#define query_Delete    "DELETE FROM plan.ext_module_dic WHERE suid = :id "
#define query_DeleteAll "DELETE FROM plan.ext_module_dic "
#define query_Update    "UPDATE plan.ext_module_dic SET "\
                        " title=:title " \
                        "WHERE suid = :id"
#else
#define query_Insert    "INSERT INTO plan.ext_proc_dic (id,title,ext_modules) " \
                        "VALUES " \
                        "(:id,:title,:ext_modules)"
#define query_Delete    "DELETE FROM plan.ext_proc_dic WHERE id = :id "
#define query_Update    "UPDATE plan.ext_proc_dic SET "\
                        " title=:title, ext_modules=:ext_modules " \
                        "WHERE id = :id"
#endif
// -------------------------------------------------------------------------------------------
EM_ExtProc::EM_ExtProc(int id,const QString title):AbstractEntity(id){
    _title = title;
}

EM_ExtProc::~EM_ExtProc(){
    ;
}
QList<TExternalModuleType> &EM_ExtProc::getModules(){
    return _lsModules;
}
const QString& EM_ExtProc::getTitle() const{
    return _title;
}
int EM_ExtProc::getNum() const{
    return suid();
}
const QMap<int,EM_ExtModule*>&  EM_ExtProc::modules() const{
    return _modules;
}
void EM_ExtProc::add(EM_ExtModule* m){
    if(m!=0 ){
        if(_modules.contains(m->SUID())){
            delete _modules[m->SUID()];
        }
        _modules[m->SUID()] = m;
    }
}
void EM_ExtProc::rem(EM_ExtModule* m){
    if(m!=0 ){
        if(_modules.contains(m->SUID())){
            _modules.remove(m->SUID());
        }
    }
}

// -------------------------------------------------------------------------------------------
EM_ExtModule::EM_ExtModule():AbstractEntity(-1){
    _is_system = false;
}
const QString& EM_ExtModule::getTitle() const{return _title;}
const QString& EM_ExtModule::getPath() const{return _path;}
const QString& EM_ExtModule::getDescr() const{return _descr;}

bool EM_ExtModule::isSystem() const{return _is_system;}
// -------------------------------------------------------------------------------------------
void parse(QByteArray raw_value,QList<TExternalModuleType> &ret)throw(JSONFormatException::ParseInputJSONException){
    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(raw_value, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("object")){
                QJsonValue object_type = obj["object"];
                if(!object_type.isString()|| object_type.toString().compare("ext_module")!=0){
                    qDebug()<<"parseExtModule: "<<"Wrong object type";
                    throw JSONFormatException::ParseInputJSONException();
                }
                if(!obj.contains("items")|| !obj["items"].isArray()){
                    qDebug()<<"parseExtModule: "<<"Wrong object format";
                    throw JSONFormatException::ParseInputJSONException();
                }
                QJsonArray items = obj["items"].toArray();

                foreach (QVariant iter, items) {
                    if(iter.canConvert(QVariant::Int)){
                        ret<<(TExternalModuleType)iter.toInt();
                    }else{
                        qDebug()<<"parseExtModule: "<<"value in array wrong format";
                    }
                }
            }else{
                qDebug()<<"parseExtModule: "<<"Not found object";
                throw JSONFormatException::ParseInputJSONException();
            }
        }else{
            qDebug()<<"parseExtModule: "<<"Input data not object";
            throw JSONFormatException::ParseInputJSONException();
        }
    }else{
        qDebug()<<"parseExtModule: "<<parseError.errorString();
        throw JSONFormatException::ParseInputJSONException();
    }
}
QByteArray parse(const QList<TExternalModuleType> &ls){
    QString st_template = "{\"object\":\"ext_module\",\"items\":[]}";
    QJsonParseError  parseError;
    QJsonDocument doc;
    if(!ls.isEmpty()){
        QJsonObject obj;
        QJsonArray arr;
        obj["object"] = QJsonValue("ext_module");
        foreach (TExternalModuleType iter,ls){
            arr<< QJsonValue(iter);
        }
        obj["items"] = arr;

        doc.setObject(obj);
    }else{
        doc  = QJsonDocument::fromJson(st_template.toUtf8(), &parseError);
    }
    return doc.toJson(QJsonDocument::Compact);
}
void EM_ExtProcDic::reload()throw(CommonException::OpenDBException,CommonException::SQLException){
#ifdef NEW_VERSION
    qDebug()<<"Reload ExtProcDic NEW VERSION";
    clear();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *q = new QSqlQuery(db);

    //===================================================
    q->prepare(query_SelectModule);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    int ind_suid = rec.indexOf("suid");
    int ind_title = rec.indexOf("title");
    int ind_path = rec.indexOf("path");
    int ind_descr = rec.indexOf("descr");
    int ind_system = rec.indexOf("is_system");


    while(q->next()){
        EM_ExtModule* newItem = new EM_ExtModule();

        newItem->suid(q->value(ind_suid).toInt());
        newItem->_title = q->value(ind_title).toString();
        newItem->_path = q->value(ind_path).toString();
        newItem->_descr = q->value(ind_descr).toString();
        newItem->_is_system = q->value(ind_system).toBool();

        _mapM[newItem->SUID()] = newItem;
        _mapMs[newItem->_path] = newItem;


        EM_ExtProc* newProc = new EM_ExtProc(newItem->SUID(),newItem->getTitle());
        _map[newProc->SUID()] = newProc;
    }

    delete q;
    db.close();
#else
    qDebug()<<"Reload ExtProcDic";
    clear();
    qDebug()<<"Clear ExtProcDic";
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    qDebug()<<"Open db ExtProcDic";
    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_SelectModule);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    int ind_suid = rec.indexOf("suid");
    int ind_title = rec.indexOf("title");
    int ind_path = rec.indexOf("path");
    int ind_descr = rec.indexOf("descr");
    int ind_system = rec.indexOf("is_system");


    while(q->next()){
        EM_ExtModule* newItem = new EM_ExtModule();

        newItem->suid(q->value(ind_suid).toInt());
        newItem->_title = q->value(ind_title).toString();
        newItem->_path = q->value(ind_path).toString();
        newItem->_descr = q->value(ind_descr).toString();
        newItem->_is_system = q->value(ind_system).toBool();

        _mapM[newItem->SUID()] = newItem;
        _mapMs[newItem->_path] = newItem;
    }

    //===================================================
    q->prepare(query_Select);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    rec = q->record();
    int ind_id = rec.indexOf("id");
    ind_title = rec.indexOf("title");
    int ind_json = rec.indexOf("ext_modules");


    int      suid;
    QString  title;
    QString  raw_value;


    while(q->next()){
        suid = q->value(ind_id).toInt();
        title = q->value(ind_title).toString();
        raw_value = q->value(ind_json).toString();
        QList<TExternalModuleType> ls;
        parse(raw_value.toUtf8(),ls);

        EM_ExtProc *newItem = new EM_ExtProc(suid, title);
        newItem->_lsModules<<ls;

        _map[suid] = newItem;
    }

    //===================================================
    q->prepare(query_SelectLink);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    rec = q->record();
    int ind_pid = rec.indexOf("proc_id");
    int ind_mid = rec.indexOf("module_id");

    int pid,mid;
    while(q->next()){
        pid = q->value(ind_pid).toInt();
        mid = q->value(ind_mid).toInt();
        if(_map.contains(pid) && _mapM.contains(mid)){
            _map[pid]->add(_mapM[mid]);
        }
    }

    delete q;
    db.close();
    qDebug()<<"Reload ExtProcDic finished";
#endif
}

void EM_ExtProcDic::add(EM_ExtProc* proc,EM_ExtModule* module) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException){
#ifdef NEW_VERSION
    if(proc==0 || module==0) throw CommonException::NullParamException();
    return;
#else
    if(proc==0 || module==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_SelectLinkTest);
    q->bindValue(":proc_id",proc->SUID());
    q->bindValue(":module_id",module->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    bool bFind = false;
    while(q->next()){
        bFind = true;
    }
    if(!bFind){
        q->prepare(query_InsertLink);
        q->bindValue(":proc_id",proc->SUID());
        q->bindValue(":module_id",module->SUID());
        if(!q->exec()){
            QString mes = q->lastError().text();
            delete q;
            db.close();
            throw CommonException::SQLException(mes);
        }
        proc->add(module);
    }

    delete q;
    db.close();
#endif
}
void EM_ExtProcDic::rem(EM_ExtProc* proc,EM_ExtModule* module) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException){
#ifdef NEW_VERSION
    if(proc==0 || module==0) throw CommonException::NullParamException();
    return;
#else
    if(proc==0 || module==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_DeleteLink);
    q->bindValue(":proc_id",proc->SUID());
    q->bindValue(":module_id",module->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    proc->rem(module);

    delete q;
    db.close();
#endif
}
EM_ExtModule* EM_ExtProcDic::add(const QString& title,const QString& path, const QString& descr, bool system)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException){
#ifdef NEW_VERSION
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_InsertModule);
    q->bindValue(":title",title);
    q->bindValue(":path",path);
    q->bindValue(":descr",descr);
    q->bindValue(":is_system",system);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    int id = -1;
    while(q->next()){
        id = q->value("suid").toInt();
    }
    EM_ExtModule* newItem = new EM_ExtModule();
    newItem->suid(id);
    newItem->_title = title;
    newItem->_path = path;
    newItem->_descr = descr;
    newItem->_is_system = system;

    _mapM[id] = newItem;

    EM_ExtProc* newProc = new EM_ExtProc(id,title);
    _map[id] = newProc;

    delete q;
    db.close();

    return newItem;
#else
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_InsertModule);
    q->bindValue(":title",title);
    q->bindValue(":path",path);
    q->bindValue(":descr",descr);
    q->bindValue(":is_system",system);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    int id = -1;
    while(q->next()){
        id = q->value("suid").toInt();
    }
    EM_ExtModule* newItem = new EM_ExtModule();
    newItem->suid(id);
    newItem->_title = title;
    newItem->_path = path;
    newItem->_descr = descr;
    newItem->_is_system = system;

    _mapM[id] = newItem;

    delete q;
    db.close();

    return newItem;
#endif
}
EM_ExtModule* EM_ExtProcDic::add(EM_ExtProc* proc,const QString& title,const QString& path, const QString& descr,bool system)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException){
#ifdef NEW_VERSION
    if(proc==0) throw CommonException::NullParamException();
    return add(title,path,descr,system);
#else
    if(proc==0) throw CommonException::NullParamException();

    EM_ExtModule* module = add(title,path,descr,system);
    add(proc,module);
    return module;
#endif
}

void EM_ExtProcDic::rem(EM_ExtModule* module)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
#ifdef NEW_VERSION
    if(module==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    _mapM.remove(module->SUID());

    if(_map.contains(module->SUID())){
        delete _map[module->SUID()];
        _map.remove(module->SUID() );
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_DeleteModule);
    q->bindValue(":suid",module->SUID());

    delete module;

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
#else
    if(module==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    _mapM.remove(module->SUID());
    foreach (EM_ExtProc* iter, _map.values()) {
        iter->rem(module);
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_DeleteModule);
    q->bindValue(":suid",module->SUID());

    delete module;

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
#endif
}
void EM_ExtProcDic::remModule()throw(CommonException::OpenDBException,CommonException::SQLException){
#ifdef NEW_VERSION
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }


    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_DeleteModuleAll);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();

    clear();
#else
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    foreach (EM_ExtModule* it, _mapM.values()) {
        foreach (EM_ExtProc* iter, _map) {
            iter->rem(it);
        }
        delete it;
    }
    _mapM.clear();

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_DeleteModuleAll);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
#endif
}
void EM_ExtProcDic::update(EM_ExtModule* module,const QString& title,const QString& path, const QString& descr,bool system)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException){
    if(module==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    //===================================================
    q->prepare(query_UpdateModule);
    q->bindValue(":title",title);
    q->bindValue(":path",path);
    q->bindValue(":descr",descr);
    q->bindValue(":is_system",system);
    q->bindValue(":suid",module->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    module->_title = title;
    module->_path = path;
    module->_descr = descr;
    module->_is_system = system;

    delete q;
    db.close();
}
EM_ExtProc* EM_ExtProcDic::add(const QString& title)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException){
    QList<TExternalModuleType> ls;
    return add(title,ls);
}

EM_ExtProc* EM_ExtProcDic::add(const QString& title,const QList<TExternalModuleType> & ls)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException){    
#ifdef NEW_VERSION
    if(ls.isEmpty()){;}

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Insert);
    q->bindValue(":title",title);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    int id = -1;
    while(q->next()){
        id = q->value("suid").toInt();
    }
    EM_ExtProc* newItem = 0;
    if(id>0){
        EM_ExtProc* newItem = new EM_ExtProc(id,title);
        _map[id] = newItem;
    }else{
        LogObserv *log = new LogObserv();
        log->err("Ошибка при регистрации внешнего модуля");
    }
    delete q;
    db.close();

    return newItem;
#else
    int id = -1;
    for(int i=1;i<0x7FFFFFFF;i++){
        if(!_map.contains(i)){
            id = i;
            break;
        }
    }
    if(id<0){
        throw CommonException::GenPKException();
    }

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Insert);
    q->bindValue(":id",id);
    q->bindValue(":title",title);
    q->bindValue(":ext_modules",QString::fromUtf8(parse(ls)));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    EM_ExtProc* newItem = new EM_ExtProc(id,title);
    newItem->_lsModules<<ls;
    _map[id] = newItem;

    delete q;
    db.close();

    return newItem;
#endif
}
void EM_ExtProcDic::rem(EM_ExtProc *proc)throw(CommonException::OpenDBException,CommonException::SQLException){
#ifdef NEW_VERSION
    if(!_map.contains(proc->SUID())) return;
    delete _map[proc->SUID()];
    _map.remove(proc->SUID());
    if(_mapM.contains(proc->SUID())){
        delete _mapM[proc->SUID()];
        _mapM.remove(proc->SUID());
    }

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Delete);
    q->bindValue(":id",proc->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
#else
    if(!_map.contains(proc->SUID())) return;
    delete _map[proc->SUID()];
    _map.remove(proc->SUID());

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Delete);
    q->bindValue(":id",proc->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
#endif
}
void EM_ExtProcDic::rem()throw(CommonException::OpenDBException,CommonException::SQLException){
#ifdef NEW_VERSION
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteAll);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    clear();

    delete q;
    db.close();
#else
    QVariantList ls;
    foreach (int i, _map.keys()) {
        ls<<i;
    }
    clear();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Delete);
    q->bindValue(":id",ls);
    if(!q->execBatch()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
#endif
}
void EM_ExtProcDic::update(EM_ExtProc* proc,const QString& title)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException){
    QList<TExternalModuleType> ls;
    update(proc,title,ls);
}

void EM_ExtProcDic::update(EM_ExtProc* proc,const QString& title,QList<TExternalModuleType> & ls)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException){
    if(!_map.contains(proc->SUID())) throw CommonException::ObjNotFoundException();
#ifdef NEW_VERSION
    if(ls.isEmpty()){;}
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Update);
    q->bindValue(":id",proc->SUID());
    q->bindValue(":title",title);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();

    EM_ExtProc *newItem = _map[proc->SUID()];
    newItem->_title = title;
#else
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_Update);
    q->bindValue(":id",proc->SUID());
    q->bindValue(":title",title);
    q->bindValue(":ext_modules",QString::fromUtf8(parse(ls)));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();


    EM_ExtProc *newItem = _map[proc->SUID()];
    newItem->_lsModules.clear();
    newItem->_lsModules<<ls;
    newItem->_title = title;
#endif
}

EM_ExtProcDic::EM_ExtProcDic()throw(CommonException::OpenDBException,CommonException::SQLException):_connect_name(generateConnectionCustomName("ExtProcDic")){
   qDebug()<<"Create ExtProcDic: "<<_connect_name;
   getConnection(_connect_name);
   reload();
}
EM_ExtProcDic::~EM_ExtProcDic(){
    clear();
    QSqlDatabase::removeDatabase(_connect_name);
}
void EM_ExtProcDic::clear(){
#ifndef NEW_VERSION
    foreach (EM_ExtProc* iter, _map.values()) {
        delete iter;
    }    
#endif
    foreach (EM_ExtModule* iter, _mapM.values()) {
        delete iter;
    }
    foreach (EM_ExtProc* iter, _map.values()) {
        delete iter;
    }
    _mapM.clear();
    _map.clear();
    _mapMs.clear();
}

void EM_ExtProcDic::printDic()const{
    foreach (EM_ExtProc* iter, _map.values()){
        qDebug().noquote() << iter->getTitle();
    }
    qDebug().noquote()<< "========================";
    foreach (EM_ExtModule* iter, _mapM.values()){
        qDebug().noquote() << iter->getTitle();
    }
}
QMap<int,EM_ExtProc*>::const_iterator EM_ExtProcDic::begin(){return _map.begin();}
QMap<int,EM_ExtProc*>::const_iterator EM_ExtProcDic::end(){return _map.end();}
QList<EM_ExtModule*> EM_ExtProcDic::get(){
    return _mapM.values();
}
EM_ExtProc* EM_ExtProcDic::by(int id) const{
    EM_ExtProc* ret = 0;
    if(_map.contains(id)){
        ret = _map[id];
    }
    return ret;
}
const EM_ExtModule* EM_ExtProcDic::by(QString id) const{
    if(_mapMs.contains(id)){
        return _mapMs[id];
    }
    return 0;
}
bool EM_ExtProcDic::_instflag = false;
EM_ExtProcDic* EM_ExtProcDic::_instance = 0;

