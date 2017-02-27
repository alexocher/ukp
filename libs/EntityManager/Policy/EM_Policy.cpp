#include "../Headers/EM_Policy.h"
#include <QSql>
#include "../DBConnect/dbconnect.h"

#define field_title "_title"
#define field_descr "_descr"
#define field_object_id "_object_id"
#define field_isRead "_isRead"
#define field_isWrite "_isWrite"
#define field_id "_id"
#define field_pid "_pid"


#define query_SelectPolicy      "SELECT suid,parent_suid,object_id,title,descr,is_read,is_write FROM policy.policy_group"
#define query_UpdatePolicyMove  "UPDATE policy.policy_group SET parent_suid=:parent_suid WHERE suid=:suid"
#define query_DeletePolicyItem  "DELETE FROM policy.policy_group WHERE suid IN (%1) "
#define query_UpdatePolicyItem  "UPDATE policy.policy_group SET object_id=:object_id,title=:title,descr=:descr,is_read=:is_read,is_write=:is_write WHERE suid=:suid"
#define query_InsertPolicyItem  "INSERT INTO policy.policy_group (object_id,title,descr,is_read,is_write,parent_suid) VALUES (:object_id,:title,:descr,:is_read,:is_write,:parent_suid) RETURNING suid"
#define query_SelectPolicyForUser "SELECT policy_suid FROM policy.policy_user WHERE user_suid=:user_suid "
#define query_SelectPolicyForUser2 "SELECT policy_suid FROM policy.policy_user WHERE user_suid=:user_suid AND policy_suid IN(%1)"
#define query_SelectPolicyForUser3  "SELECT g.suid FROM policy.policy_user AS u "\
                                    " JOIN policy.policy_group AS g ON u.policy_suid = g.suid AND g.object_id = :head_id "\
                                    " WHERE u.user_suid = :user_suid "\
                                    " UNION "\
                                    " SELECT g1.suid "\
                                    " FROM policy.policy_user AS u "\
                                    " JOIN policy.policy_group AS g ON g.object_id = :head_id "\
                                    " JOIN policy.policy_group AS g1 ON g1.parent_suid = g.suid AND u.policy_suid = g1.suid AND g1.object_id = 'SID_ALL_ACCESS' "\
                                    " WHERE u.user_suid=:user_suid "
#define query_InsertPolicyUser  "INSERT INTO policy.policy_user (user_suid,policy_suid) VALUES (:user_suid,:policy_suid)"
#define query_DeletePolicyUser  "DELETE FROM policy.policy_user WHERE user_suid=:user_suid AND policy_suid=:policy_suid"
#define query_SelectPolicyUser  "SELECT user_suid FROM policy.policy_user WHERE policy_suid=:policy_suid"
// =======================================================
EM_Policy::EM_Policy(int suid,const QString& title, const QString& descr,const QString& objectID,bool isRead,bool isWrite):BaseTreeItem(suid,1),AbstractEntity(suid){
    _title = title;
    _descr = descr;
    _object_id = objectID;
    _isRead = isRead;
    _isWrite = isWrite;
}

int EM_Policy::getSize() const{
    return BaseTreeItem::getSize();
}
EM_Policy* EM_Policy::getParent() const{
    return (EM_Policy*)BaseTreeItem::getParent();
}
EM_Policy::iterator EM_Policy::begin(){
    return iterator(((BaseTreeItem*)this)->begin());
}
EM_Policy::iterator EM_Policy::end(){
    return iterator(((BaseTreeItem*)this)->end());
}
const QString& EM_Policy::getTitle() const{
    return _title;
}
const QString& EM_Policy::getDescr() const{
    return _descr;
}
const QString& EM_Policy::getObjectID() const{
    return _object_id;
}
bool EM_Policy::isRead() const{
    return _isRead;
}
bool EM_Policy::isWrite() const{
    return _isWrite;
}
// ==================================================================================
EM_PolicyDic* EM_PolicyDic::_instance = 0;
bool EM_PolicyDic::_instflag = false;
EM_PolicyDic::EM_PolicyDic():BaseTree<int,int>(0),_connect_name("PolicyDic"+QUuid::createUuid().toString()){
    _isLoad = false;
    qDebug()<<_connect_name;
    getConnection(_connect_name);
}
EM_PolicyDic &EM_PolicyDic::Instance(){
    if(!_instflag){
        _instflag = true;
        _instance = new EM_PolicyDic();
    }
    return *_instance;
}
EM_PolicyDic::~EM_PolicyDic(){
    QSqlDatabase::removeDatabase(_connect_name);
    qDebug()<<"Destroy EM_PolicyDic";
}
EM_Policy *EM_PolicyDic::getRoot() const{
    return (EM_Policy*)BaseTree::getRoot();
}
EM_OPERATION_RETURNED_STATUS EM_PolicyDic::fromDB()throw (CommonException::OpenDBException,CommonException::SQLException){
    if(_isLoad) return opOK;
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    EM_Policy* fakeRoot = 0;

    // структура для обработки не связанных ключей
    typedef struct KEYS{
        int key;
        int pkey;
    }* PKEYS;

    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(query_SelectPolicy);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    //
//    if(getRoot()!=0) delete getRoot();
//    _mapID.clear();

    int q_size = q->size();
    if(q_size<1) {
        fakeRoot =  add(-1,"Словарь политик доступа", "Корневой элемент", "-1", true, true);
        setRootItem(fakeRoot);
        delete q;
        db.close();
        return opOK;
    }


    int l_suid;
    int l_parent_suid;
    QString l_title;
    QString l_descr;
    QString l_objid;
    bool    l_read;
    bool    l_write;


    // индексы столбцов
    QSqlRecord rec = q->record();
    const int ind_suid = rec.indexOf("suid");
    const int ind_parent_suid = rec.indexOf("parent_suid");
    const int ind_title = rec.indexOf("title");
    const int ind_descr = rec.indexOf("descr");
    const int ind_objid = rec.indexOf("object_id");
    const int ind_read  = rec.indexOf("is_read");
    const int ind_write = rec.indexOf("is_write");

    QMap<int,EM_Policy*> mTree;


    int keys_size = 0;
    PKEYS keys = new KEYS[q_size];

    EM_Policy *root = 0;

    while(q->next()){

        l_suid = q->value(ind_suid).toInt();
        l_parent_suid = q->value(ind_parent_suid).toInt();
        l_title = q->value(ind_title).toString();
        l_descr = q->value(ind_descr).toString();
        l_objid = q->value(ind_objid).toString();
        l_read = q->value(ind_read).toBool();
        l_write = q->value(ind_write).toBool();


        int pkey = l_parent_suid;
        int key = l_suid;


        EM_Policy *newItem = new EM_Policy(key,l_title, l_descr,l_objid,l_read,l_write);
        _mapID[l_objid] = newItem;

        mTree[key] = newItem;
        if(pkey==-1){
            root = newItem;
        }else{
            if(mTree.contains(pkey)){
                EM_Policy *par = mTree[pkey];
                BaseTree::add(par,newItem);
            }else{
                keys[keys_size].key = key;
                keys[keys_size].pkey = pkey;
                keys_size++;
            }
        }
    }
    delete q;
    db.close();

    // если есть бессвязные элементы дополняем
    for(int i=0;i<keys_size;i++){
        // TODO: надо бы проверить на существование
        EM_Policy *par = mTree[keys[i].pkey];
        EM_Policy *item = mTree[keys[i].key];


        BaseTree::add(par,item);
    }
    if(root==0) root = fakeRoot;

    setRootItem(root);

    delete [] keys;
    _isLoad = true;
    return opOK;

}
EM_Policy * EM_PolicyDic::add(int parent_suid, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_InsertPolicyItem);
    q->bindValue(":object_id",objectID);
    q->bindValue(":title",title);
    q->bindValue(":descr",descr);
    q->bindValue(":is_read",isRead);
    q->bindValue(":is_write",isWrite);
    q->bindValue(":parent_suid",parent_suid);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    int newID = -1;
    while(q->next()){
        newID = q->value("suid").toInt();
    }

    delete q;
    db.close();

    return new EM_Policy(newID,title, descr, objectID, isRead, isWrite);
}

EM_Policy * EM_PolicyDic::add(EM_Policy *parent, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(parent==0) throw CommonException::NullParamException();

    EM_Policy* newItem = add(parent->suid(),title, descr, objectID, isRead, isWrite);

    BaseTree::add(parent,newItem);

    return newItem;

}
EM_Policy * EM_PolicyDic::add(const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::OpenDBException,CommonException::SQLException){
    if(!_isLoad){
        fromDB();
    }
    EM_Policy* parent = 0;
    if(getRoot()==0){
         parent = add(-1,"Словарь политик доступа", "Корневой элемент", "-1", true, true);
         setRootItem(parent);
    }else{
        parent = getRoot();
    }

    EM_Policy* newItem = add(parent->suid(),title, descr, objectID, isRead, isWrite);

    BaseTree::add(parent,newItem);

    return newItem;
}

// изменение узла (сразу сейвится в базу)
EM_OPERATION_RETURNED_STATUS EM_PolicyDic::update(EM_Policy *item, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(item==0) throw CommonException::NullParamException();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_UpdatePolicyItem);
    q->bindValue(":object_id",objectID);
    q->bindValue(":title",title);
    q->bindValue(":descr",descr);
    q->bindValue(":is_read",isRead);
    q->bindValue(":is_write",isWrite);
    q->bindValue(":suid",item->suid());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();

    item->_title = title;
    item->_descr = descr;
    item->_object_id = objectID;
    item->_isRead = isRead;
    item->_isWrite = isWrite;

    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_PolicyDic::rem(EM_Policy *item)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(item==0) throw CommonException::NullParamException();

    QStringList lsCond;
    QString cond = "";
    QStack<EM_Policy*> stack;
    stack.push( item );
    while(!stack.isEmpty()){
        EM_Policy* cur = stack.pop();

        lsCond<<QString::number(cur->suid());
        for(EM_Policy::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    cond = lsCond.join(",");
    q->prepare(QString(query_DeletePolicyItem).arg(cond));


    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();

    BaseTree::rem(item);
    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_PolicyDic::move(EM_Policy *parent,EM_Policy *item)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(parent==0 || item==0) throw CommonException::NullParamException();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(query_UpdatePolicyMove);
    q->bindValue(":parent_suid",parent->suid());
    q->bindValue(":suid",item->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();

    BaseTree::move(parent,item);

    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_PolicyDic::addUser(EM_Policy* policy,const EM_User* user)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(policy==0 || user==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_DeletePolicyUser);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":policy_suid",policy->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }


    q->prepare(query_InsertPolicyUser);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":policy_suid",policy->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();


    return opOK;
}
QList<EM_User*> EM_PolicyDic::getUser(const EM_Policy* policy)const  throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(policy==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_SelectPolicyUser);
    q->bindValue(":policy_suid",policy->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    EM_AddressBook &addr = EM_AddressBook::Instance();
    QList<EM_User*> ret;
    QSqlRecord rec = q->record();
    const int idx_suid = rec.indexOf("user_suid");
    while(q->next()){
        int suid = q->value(idx_suid).toInt();
        EM_User* user = addr.getUser(suid);
        if(user==0){
            qDebug().noquote()<<"Warning!!!! User with suid: "<<QString::number(suid)<<" not found";
            continue;
        }
        ret<<user;
    }

    delete q;
    db.close();
    return ret;
}

EM_OPERATION_RETURNED_STATUS EM_PolicyDic::remUser(EM_Policy* policy,const EM_User* user)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(policy==0 || user==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_DeletePolicyUser);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":policy_suid",policy->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();


    return opOK;
}
bool EM_PolicyDic::isAccess(const EM_User* user,const QString& head_id) const throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(user==0) throw new CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_SelectPolicyForUser3);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":head_id",head_id);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    bool ret = false;
    while(q->next()){
        ret = true;
    }

    delete q;
    db.close();


    return ret;

}
// получить все политики для пользователя по идентификатору
QMap<QString,QSharedPointer<EM_PolicyDic::ACCESS> > EM_PolicyDic::getPolicy(const EM_User* user,const QString& id)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    QMap<QString,QSharedPointer<ACCESS> > ret;
    if(user==0) throw CommonException::NullParamException();

    if(!_isLoad){
        fromDB();
    }

    if(!_mapID.contains(id)){
        return ret;
    }

    EM_Policy* node = _mapID[id];

    QStringList cond;
    EM_Policy::iterator iter;
    QStack<EM_Policy*> stack;
    stack.push(node);
    while(!stack.isEmpty()){
        EM_Policy* cur = stack.pop();

        cond<<QString::number(cur->SUID());

        for(iter = cur->begin(); iter!=cur->end(); ++iter ){
            stack.push(*iter);
        }
    }

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(QString(query_SelectPolicyForUser2).arg(cond.join(", ")));
    q->bindValue(":user_suid",user->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    const int ind_suid = rec.indexOf("policy_suid");
    while(q->next()){
        int suid = q->value(ind_suid).toInt();
        EM_Policy* item = (EM_Policy*)find(suid);
        if(item==0){
            qDebug()<<"Warning!!! not found policy with suid: "<<QString::number(suid);
            continue;
        }
        QSharedPointer<ACCESS> pt = QSharedPointer<ACCESS>(new P_ACCESS);
        pt->read=item->isRead();
        pt->write=item->isWrite();
        ret[item->getObjectID()] = pt;
    }

    delete q;
    db.close();


    return ret;
}
// получить все политики для пользователя
QMap<QString,QSharedPointer<EM_PolicyDic::ACCESS> > EM_PolicyDic::getPolicy(const EM_User* user)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    QMap<QString,QSharedPointer<ACCESS> > ret;

    if(user==0) throw CommonException::NullParamException();

    if(!_isLoad){
        fromDB();
    }

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(query_SelectPolicyForUser);
    q->bindValue(":user_suid",user->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

//    QStack<EM_Policy*> stack;
    QSqlRecord rec = q->record();
    const int ind_suid = rec.indexOf("policy_suid");
    while(q->next()){
        int suid = q->value(ind_suid).toInt();
        EM_Policy* item = (EM_Policy*)find(suid);
        if(item==0){
            qDebug()<<"Warning!!! not found policy with suid: "<<QString::number(suid);
            continue;
        }
        QSharedPointer<ACCESS> pt = QSharedPointer<ACCESS>(new P_ACCESS);
        pt->read=item->isRead();
        pt->write=item->isWrite();
        ret[item->getObjectID()] = pt;

        /*stack.push(item);
        while(!stack.isEmpty()){
            EM_Policy* cur = stack.pop();

            QSharedPointer<ACCESS> pt;
            if(!ret.contains(item->getObjectID())){
                pt = QSharedPointer<ACCESS>(new P_ACCESS);
                pt->read=false;
                pt->write=false;
                ret[item->getObjectID()] = pt;
            }else{
                pt = ret[item->getObjectID()];
            }
            pt->read|=cur->isRead();
            pt->write|=cur->isWrite();
            for(EM_Policy::iterator it=cur->begin();it!=cur->end();++it){
                stack.push(*it);
            }
        }*/
    }

    delete q;
    db.close();


    return ret;
}

void EM_PolicyDic::printOshsTree() const{
    qDebug()<<"OSHS Tree";
    QStack<EM_Policy*> stack;
    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_Policy* cur = stack.pop();
        QString ret = "";
        for(int i=1;i<cur->getLevel();i++){
            ret+="----";
        }
        ret+=" "+cur->getTitle()+" ("+cur->getObjectID()+")";
        qDebug().noquote()<<ret;

        for(EM_Policy::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }
}

QByteArray EM_PolicyDic::serialize() const{
    QJsonArray a;

    EM_Policy::iterator iter;
    QStack<EM_Policy*> stack;
    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_Policy* cur = stack.pop();

        QJsonObject o;
        o[field_title] = QJsonValue(cur->_title);
        o[field_descr] = QJsonValue(cur->_descr);
        o[field_object_id] = QJsonValue(cur->_object_id);
        o[field_isRead] = QJsonValue(cur->_isRead);
        o[field_isWrite] = QJsonValue(cur->_isWrite);

        o[field_id] = QJsonValue(cur->SUID());
        int pid = cur->getParent()==0?-1:cur->getParent()->SUID();
        o[field_pid] = QJsonValue(pid);

        a.append(o);


        for(iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }
    }

    return QJsonDocument(a).toBinaryData();
}
int EM_PolicyDic::deserialize(const QByteArray& data){
    if(data.isNull()) return -1;

    int ret = 1;

    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isArray()){
        QMap<int,EM_Policy*> mP;
        QMap<int,EM_Policy*> m;
        foreach (QJsonValue iter, doc.object()) {
            if(iter.isObject()){
                QJsonObject o = iter.toObject();
                EM_Policy* newItem = new EM_Policy(
                            o[field_id].toInt(),
                            o[field_title].toString(),
                            o[field_descr].toString(),
                            o[field_object_id].toString(),
                            o[field_isRead].toBool(),
                            o[field_isWrite].toBool()
                            );
                int pid = o[field_pid].toInt();
                mP[pid] = newItem;
                m[newItem->SUID()] = newItem;
            }else{
                qWarning()<<"Wrong item";
            }
        }
        // ===================================
        if(mP.contains(-1)){
            delete m_RootItem;
            m_Map.clear();

            m_RootItem = mP[-1];
            m_Map[-1] = m_RootItem;
            foreach (int pid, mP.keys()) {
                if(pid==-1) continue;

                EM_Policy* item = mP[pid];
                EM_Policy* p_item = m[pid];

                BaseTree::add(p_item,item);
            }

        }else{
            qDeleteAll(m.values());

            qWarning()<<"Not found root";
            ret = -3;
        }
    }else{
        qWarning()<<"Wrong data format";
        ret = -2;
    }

    return ret;
}
