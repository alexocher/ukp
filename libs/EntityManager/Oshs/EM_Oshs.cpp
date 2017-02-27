
#include "../Headers/EM_Oshs.h"
#include "../DBConnect/dbconnect.h"

#include <QtSql>

// Запрос ОШС
#define query_SelectOSHSItem        "SELECT suid, parent_suid, fname, sname, level, employee_count FROM oshs.oshsitem"

// добавление/обновление узла
#define  query_InsertOSHSItem       "INSERT INTO oshs.oshsitem "\
                                    " (parent_suid, fname, sname, level, employee_count ) "\
                                    " VALUES "\
                                    " (:parent_suid, :fname, :sname, :level, :employee_count ) "\
                                    " RETURNING suid"

#define  query_UpdateOSHSItem       "UPDATE oshs.oshsitem "\
                                    " SET "\
                                    " parent_suid = :parent_suid, fname = :fname, sname = :sname, level = :level, employee_count = :employee_count "\
                                    " WHERE suid = :suid "
// Удаление узла
#define query_DeleteOSHSItem        "DELETE FROM oshs.oshsitem " \
                                    " WHERE suid IN (%1) "
// Очистка всей ОШС
#define query_DeleteOSHS            "DELETE FROM oshs.oshsitem "


#define query_SelectEsteblishedPost "SELECT type, name, group_type FROM oshs.esteblishedpost"

#define query_UpdateEsteblishedPost "UPDATE oshs.esteblishedpost SET name=:name, group_type=:group_type WHERE type=:id"

#define query_SelectEsteblishedPostAccess "SELECT access_templ FROM oshs.esteblishedpost WHERE type = :type"

#define query_UpdateEsteblishedPostAccess "UPDATE oshs.esteblishedpost SET "\
                                            " access_templ=:access_templ WHERE type = :type"

#define query_SelectEsteblishedPostForItem "SELECT e.type, e.name, e.group_type, l.num "\
                                            " FROM oshs.oshsitem_esteblishedpost AS l " \
                                            " JOIN oshs.esteblishedpost AS e ON e.type = l.esteblishedpost_id " \
                                            " WHERE l.oshsitem_suid = :suid "

#define query_DeleteUserForItem            "DELETE FROM oshs.user_in_oshs "\
                                           " WHERE esteblished_id = :id AND oshsitem_suid = :suid "

#define query_DeleteEsteblishedPostForItem "DELETE FROM oshs.oshsitem_esteblishedpost "\
                                           " WHERE esteblishedpost_id = :id AND oshsitem_suid = :suid "


#define query_SelectEsteblishedPostForItemByID "SELECT esteblishedpost_id FROM oshs.oshsitem_esteblishedpost " \
                                               " WHERE esteblishedpost_id = :id AND oshsitem_suid = :suid "

#define query_InsertEsteblishedPostForItem  "INSERT INTO oshs.oshsitem_esteblishedpost(oshsitem_suid, esteblishedpost_id, num) " \
                                            "VALUES " \
                                            "( :oshsitem_suid, :esteblishedpost_id, :num ) "

#define query_SelectEsteblishedPostAndUserForItem  "SELECT u.login, l.estebleshed_id "\
                                                   " FROM oshs.user_in_oshs AS l " \
                                                   " JOIN oshs.secure AS u ON u.suid = l.user_suid" \
                                                   " WHERE l.oshsitem_suid = :node_suid "
#define query_SelectOshsItemForUser     "SELECT oshsitem_suid FROM oshs.user_in_oshs WHERE user_suid=:user_suid"

#define query_SelectOshsItemForUsers    "SELECT user_suid, oshsitem_suid, estebleshed_id FROM oshs.user_in_oshs WHERE oshsitem_suid IN (%1)"

#define query_SelectOshsItemForUsers2   "SELECT " \
                                        " s.user_suid, s.oshsitem_suid, s.estebleshed_id, e.name, e.group_type "\
                                        " FROM oshs.user_in_oshs AS s" \
                                        " JOIN oshs.esteblishedpost AS e ON e.type = s.estebleshed_id " \
                                        " WHERE s.oshsitem_suid = :oshsitem_suid "
#define query_SelectOshsItemForUsers3   "SELECT " \
                                        " s.user_suid, s.oshsitem_suid "\
                                        " FROM oshs.user_in_oshs AS s" \
                                        " WHERE s.estebleshed_id = :estebleshed_id "


#define query_SelectEsteblishedPostForUser  "SELECT estebleshed_id "\
                                            " FROM oshs.user_in_oshs AS l " \
                                            " WHERE l.user_suid = :user_suid "

#define query_InsertAppoint     "INSERT INTO oshs.user_in_oshs " \
                                " (user_suid,oshsitem_suid,estebleshed_id) " \
                                " VALUES "\
                                " (:user_suid,:oshsitem_suid,:estebleshed_id) "

#define query_DeleteAppoint     "DELETE FROM oshs.user_in_oshs WHERE " \
                                " user_suid=:user_suid AND oshsitem_suid = :oshsitem_suid AND estebleshed_id = :estebleshed_id "




EM_OSHSItem::EM_OSHSItem(const QString& fullTitle, const QString& shortTitle, int level, int employeeCount, int suid):BaseTreeItem(suid,1),AbstractEntity(suid),EM_InternalState(INTERNAL_STATE_NEW_ITEM){
    _fullTitle = fullTitle;
    _shortTitle = shortTitle;
    _level    = level;
    _employeeCount = employeeCount;
}
void EM_OSHSItem::setInternalState(INTERNAL_STATE state, bool child){
    EM_InternalState::setInternalState(state);
    if(child){
        for(EM_OSHSItem::iterator iter = begin();iter!=end();++iter){
            (*iter)->setInternalState(state,true);
        }
    }
}
void EM_OSHSItem::setModify(){
    if(getInternalState()!=INTERNAL_STATE_NEW_ITEM){
        setInternalState(INTERNAL_STATE_MODIFY_ITEM);
    }
}
EM_OSHSItem* EM_OSHSItem::getParent() const{
    return  (EM_OSHSItem*)BaseTreeItem::getParent();
}
EM_OSHSItem::iterator EM_OSHSItem::begin(){
    return iterator(((BaseTreeItem*)this)->begin());
}
EM_OSHSItem::iterator EM_OSHSItem::end(){
    return iterator(((BaseTreeItem*)this)->end());
}
const QString& EM_OSHSItem::getFullTitle() const{
    return _fullTitle;
}
const QString& EM_OSHSItem::getShortTitle() const{
    return _shortTitle;
}
int EM_OSHSItem::getOSHSLevel() const{
    return _level;
}
int EM_OSHSItem::getEemployeeCount()const{
    return _employeeCount;
}
void EM_OSHSItem::setFullTitle(const QString& value){
    _fullTitle = value;
    setModify();
}
void EM_OSHSItem::setShortTitle(const QString& value){
    _shortTitle = value;
    setModify();
}
void EM_OSHSItem::setEmployeeCount(int value){
    _employeeCount = value;
    setModify();
}
// ------------------------------------------------------------------------------------
EM_OSHSItem* EM_OSHSDic::Instance(const QString& fullTitle, const QString& shortTitle, int level, int employeeCount){
    return new EM_OSHSItem(fullTitle, shortTitle,level,employeeCount,-1);
}
EM_OSHSItem* EM_OSHSDic::getRoot()const{
    return (EM_OSHSItem*) BaseTree::getRoot();
}
#define st_connect_name "OshsItem"
EM_OSHSDic::EM_OSHSDic(EM_OSHSItem* rootItem):BaseTree<int,int>(rootItem),_connect_name(st_connect_name+QUuid::createUuid().toString()){
    //db = getConnection(_connect_name);
    getConnection(_connect_name);
}
EM_OSHSDic::EM_OSHSDic():BaseTree<int,int>(0),_connect_name(st_connect_name+QUuid::createUuid().toString()){
    qDebug()<<_connect_name;
    //db = getConnection(_connect_name);
    getConnection(_connect_name);
}
EM_OSHSDic::~EM_OSHSDic(){
//    QSqlDatabase db = QSqlDatabase::database(_connect_name);
//    if(db.isOpen())db.close();
//    db.removeDatabase(_connect_name);
    QSqlDatabase::removeDatabase(_connect_name);
    qDebug()<<"Close connect: "<<_connect_name;
}
EM_OSHSItem* EM_OSHSDic::add(EM_OSHSItem *parent,const QString& fullTitle, const QString& shortTitle,int employeeCount)throw (CommonException::NullParamException){
    if( parent==0 ) throw CommonException::NullParamException();
    int level = parent->getLevel()+1;
    EM_OSHSItem* newItem = new EM_OSHSItem(fullTitle,shortTitle,level,employeeCount,-1);
    BaseTree::add(parent,newItem);

    return newItem;
}

void EM_OSHSDic::add(EM_OSHSItem *parent,EM_OSHSItem *item) throw (CommonException::NullParamException,OSHSException::DuplicateIDException){
    if( parent==0 || item==0 ) throw CommonException::NullParamException();
    if( find(item->getID())!=0 ) throw OSHSException::DuplicateIDException();
    BaseTree::add(parent,item);
}
void EM_OSHSDic::move(EM_OSHSItem *parent,EM_OSHSItem *item){
    if(item==0) return;
    if(item->getInternalState()!=EM_InternalState::INTERNAL_STATE_NEW_ITEM){
        item->setInternalState(EM_InternalState::INTERNAL_STATE_MODIFY_ITEM);
    }
    BaseTree::move(parent,item);
}
EM_OSHSItem* EM_OSHSDic::find(int id){
    return (EM_OSHSItem*)(BaseTree::find(id));
}
void EM_OSHSDic::addEsteblishedPost(const EM_OSHSItem* node,const EM_EsteblishedPost* item, int num) throw(CommonException::DuplicateObjectException,CommonException::NullParamException,CommonException::SQLException){
    if(node==0 || item==0) throw CommonException::NullParamException();
    if(node->getInternalState()==EM_InternalState::INTERNAL_STATE_NEW_ITEM){
        // Пока просто выходим
        qDebug().noquote()<<"OSHSItem not persist";
        return;
    }
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    // проверка на дубликат
    q->prepare(query_SelectEsteblishedPostForItemByID);
    q->bindValue(":id",item->getType());
    q->bindValue(":suid",node->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    bool bFind = false;
    while(q->next()){
        bFind = true;
        break;
    }
    if(bFind){
        delete q;
        db.close();
        throw CommonException::DuplicateObjectException();
    }

    q->prepare(query_InsertEsteblishedPostForItem);
    q->bindValue(":oshsitem_suid", node->SUID());
    q->bindValue(":esteblishedpost_id", item->getType());
    q->bindValue(":num", num);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
}
void EM_OSHSDic::remEsteblishedPost(const EM_OSHSItem* node,const EM_EsteblishedPost* item) throw(CommonException::NullParamException,CommonException::SQLException){
    if(node==0 || item==0) throw CommonException::NullParamException();
    if(node->getInternalState()==EM_InternalState::INTERNAL_STATE_NEW_ITEM){return;}

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    //
    q->prepare(query_DeleteEsteblishedPostForItem);
    q->bindValue(":id",item->getType());
    q->bindValue(":suid",node->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    q->prepare(query_DeleteUserForItem);
    q->bindValue(":id",item->getType());
    q->bindValue(":suid",node->SUID());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    //
    delete q;
    db.close();
}

QList< QSharedPointer<EM_EsteblishedPostEx> > EM_OSHSDic::getEsteblishedPost(const EM_OSHSItem* node) throw(CommonException::NullParamException,CommonException::SQLException){
    if(node==0) throw CommonException::NullParamException();
    QList< QSharedPointer<EM_EsteblishedPostEx> > ret;

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_SelectEsteblishedPostForItem);
    q->bindValue(":suid",node->SUID());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    const int ind_type = rec.indexOf("type");
    const int ind_name = rec.indexOf("name");
    const int ind_group_type = rec.indexOf("group_type");
    const int ind_num = rec.indexOf("num");

    TEstablishedPostType type;
    QString name;
    TEmployeeType group;
    int num;

    while(q->next()){
        try{
            type = (TEstablishedPostType)q->value(ind_type).toInt();
        }catch(...){
            type = eptNone;
        }
        name = q->value(ind_name).toString();
        try{
            group = (TEmployeeType)q->value(ind_group_type).toInt();
        }catch(...){
            group = eltNone;
        }

        num = q->value(ind_num).toInt();

        ret<<QSharedPointer<EM_EsteblishedPostEx>(new EM_EsteblishedPostEx(type,name,group,num));
    }

    delete q;

    db.close();

    return ret;
}

EM_OSHSItem* EM_OSHSDic::getOshsItemByUser(const EM_User *user)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(user==0) throw CommonException::NullParamException();
    return getOshsItemByUser(user->SUID());
}
EM_OSHSItem* EM_OSHSDic::getOshsItemByUser(int user_suid) throw(CommonException::OpenDBException,CommonException::SQLException){

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectOshsItemForUser);
    q->bindValue(":user_suid",user_suid);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();
    const int ind_oshsitem_suid = rec.indexOf("oshsitem_suid");
    int oshsitem_suid = -1;
    while(q->next()){
        oshsitem_suid = q->value(ind_oshsitem_suid).toInt();
    }
    delete q;
    db.close();

    EM_OSHSItem* findItem = 0;

    QStack<EM_OSHSItem*> stack;
    stack.push(getRoot());
    while(!stack.isEmpty()){
        EM_OSHSItem* cur = stack.pop();
        if(cur->suid()==oshsitem_suid){
            findItem = cur;
            break;
        }
        EM_OSHSItem::iterator iter;
        for(iter=cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }
    }


    return findItem;
}

QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getUserWithEsteblishedPost(const EM_User* user, QList<TEstablishedPostType> &filter,bool onelevel)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(user==0) throw CommonException::NullParamException();
    return getUserWithEsteblishedPost(user->SUID(),filter,onelevel);
}
QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getUserWithEsteblishedPost(const EM_User* user, QList<TEmployeeType> &filter,bool onelevel)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(user==0) throw CommonException::NullParamException();
    return getUserWithEsteblishedPost(user->SUID(),filter,onelevel);
}
QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getUserWithEsteblishedPost(int user_suid, QList<TEstablishedPostType> &filter,bool onelevel)throw(CommonException::OpenDBException,CommonException::SQLException){
    EM_OSHSItem* cur_oshsitem = getOshsItemByUser(user_suid);
    if(cur_oshsitem==0) return QList< QSharedPointer<EM_UserContainer> >();

    EM_AddressBook& addrBook = EM_AddressBook::Instance();
    EM_EsteblishedPostDic& estebBook = EM_EsteblishedPostDic::Instance();

    QList< QSharedPointer<EM_UserContainer> > ret;

    QList< QSharedPointer<EM_OSHSDic::USER_IN_OSHS> > ls =  EM_OSHSDic::getListUserInOshs(cur_oshsitem,onelevel);
    foreach (QSharedPointer<EM_OSHSDic::USER_IN_OSHS> it, ls) {
        if(!filter.isEmpty()){
            if(!filter.contains( it->_estebleshed_id)) continue;
        }
        QSharedPointer<EM_UserContainer> newItem = QSharedPointer<EM_UserContainer>(new EM_UserContainer(addrBook.getUser(it->_user_suid)));
        newItem->add(new EM_EsteblishedPost(*(estebBook.find(it->_estebleshed_id))));

        ret<<newItem;
    }

    return ret;
}
QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getUserWithEsteblishedPost(int user_suid, QList<TEmployeeType> &filter,bool onelevel)throw(CommonException::OpenDBException,CommonException::SQLException){
    EM_OSHSItem* cur_oshsitem = getOshsItemByUser(user_suid);
    if(cur_oshsitem==0) return QList< QSharedPointer<EM_UserContainer> >();

    EM_AddressBook& addrBook = EM_AddressBook::Instance();
    EM_EsteblishedPostDic& estebBook = EM_EsteblishedPostDic::Instance();

    QList< QSharedPointer<EM_UserContainer> > ret;

    QList< QSharedPointer<EM_OSHSDic::USER_IN_OSHS> > ls =  EM_OSHSDic::getListUserInOshs(cur_oshsitem,onelevel);
    foreach (QSharedPointer<EM_OSHSDic::USER_IN_OSHS> it, ls) {
        EM_EsteblishedPost* e = estebBook.find(it->_estebleshed_id);
        if(e==0) continue;

        if(!filter.isEmpty()){
            if(!filter.contains( e->getGroup() )) continue;
        }
        QSharedPointer<EM_UserContainer> newItem = QSharedPointer<EM_UserContainer>(new EM_UserContainer(addrBook.getUser(it->_user_suid)));
        newItem->add(new EM_EsteblishedPost(*e));

        ret<<newItem;
    }

    return ret;
}
QMap<int,int> EM_OSHSDic::getUserIDWithOshsIDbyEsteblishedpost(TEstablishedPostType type,bool reverse) throw(CommonException::OpenDBException,CommonException::SQLException){
    QMap<int,int> ret;
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectOshsItemForUsers3);
    q->bindValue(":estebleshed_id",type);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();
    const int ind_oshsitem_suid = rec.indexOf("oshsitem_suid");
    const int ind_user_suid = rec.indexOf("user_suid");
    int oshsitem_suid = -1;
    int user_suid = -1;
    while(q->next()){
        oshsitem_suid = q->value(ind_oshsitem_suid).toInt();
        user_suid = q->value(ind_user_suid).toInt();
        if(reverse){
            ret[oshsitem_suid] = user_suid;
        }else{
            ret[user_suid] = oshsitem_suid;
        }
    }
    delete q;
    db.close();

    return ret;
}

#define CONN_OSHS_ITEM_STATIC "OshsItemS"
QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getListUserWithEsteblishedPost(int oshs_suid, const QString& cond)throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase _db;
    if(QSqlDatabase::contains(CONN_OSHS_ITEM_STATIC)){
        _db = QSqlDatabase::database(CONN_OSHS_ITEM_STATIC);
    }else{
        _db = getConnection(CONN_OSHS_ITEM_STATIC);
    }
    if(!_db.open()) throw CommonException::OpenDBException(_db.lastError().text());
    QSqlQuery *q = new QSqlQuery(_db);

    q->prepare(query_SelectOshsItemForUsers2 + cond);
    q->bindValue(":oshsitem_suid",oshs_suid);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        _db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();
    //const int ind_oshsitem_suid = rec.indexOf("oshsitem_suid");
    const int ind_user_suid = rec.indexOf("user_suid");
    const int ind_estebleshed_id = rec.indexOf("estebleshed_id");
    const int ind_estebleshed_name = rec.indexOf("name");
    const int ind_estebleshed_group = rec.indexOf("group_type");

    EM_AddressBook& book = EM_AddressBook::Instance();


    QList< QSharedPointer<EM_UserContainer> > ret;
    while(q->next()){
        EM_User* user = book.getUser(q->value(ind_user_suid).toInt());

        QSharedPointer<EM_UserContainer> newItem = QSharedPointer<EM_UserContainer>(new EM_UserContainer(user));

        EM_EsteblishedPost* es = new EM_EsteblishedPost(
                    (TEstablishedPostType)q->value(ind_estebleshed_id).toInt(),
                    q->value(ind_estebleshed_name).toString(),
                    (TEmployeeType)q->value(ind_estebleshed_group).toInt()
                    );
        newItem->add(es);

        ret<<newItem;
    }

    delete q;
    _db.close();
    //_db.removeDatabase(local_connect_name);
    return ret;
}

QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getListUserWithEsteblishedPost(int oshs_suid, QList<TEstablishedPostType> &filter,bool onelevel)throw(CommonException::OpenDBException,CommonException::SQLException){
    QString cond_add="";
    if(!filter.isEmpty()){
        QStringList sl;
        foreach(TEstablishedPostType n, filter) sl << QString("%1").arg(n);
        cond_add = " AND estebleshed_id IN ( "+sl.join(",")+" )";
    }
    if(onelevel){
        ;
    }
    return getListUserWithEsteblishedPost(oshs_suid, cond_add);
}
QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getListUserWithEsteblishedPost(int oshs_suid, QList<TEmployeeType> &filter,bool onelevel)throw(CommonException::OpenDBException,CommonException::SQLException){
    QString cond_add="";
    if(!filter.isEmpty()){
        QStringList sl;
        foreach(TEmployeeType n, filter) sl << QString("%1").arg(n);
        cond_add = " AND group_type IN ( "+sl.join(",")+" )";
    }
    if(onelevel){
        ;
    }
    return getListUserWithEsteblishedPost(oshs_suid, cond_add);
}
QList< QSharedPointer<EM_OSHSDic::USER_IN_OSHS> > EM_OSHSDic::getListUserInOshs(EM_OSHSItem* oshsitem,bool isOneLevel){
    QString cond = "";
    if(isOneLevel){
        cond = QString::number(oshsitem->suid());
    }else{
        QStack<EM_OSHSItem*> stack;
        stack.push(oshsitem);
        while(!stack.isEmpty()){
            EM_OSHSItem* cur = stack.pop();
            if(!cond.isEmpty()){
                cond+=", "+QString::number(cur->suid());
            }else{
                cond+=QString::number(cur->suid());
            }
            EM_OSHSItem::iterator iter;
            for(iter=cur->begin();iter!=cur->end();++iter){
                stack.push(*iter);
            }
        }
    }
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(QString(query_SelectOshsItemForUsers).arg(cond));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();
    const int ind_oshsitem_suid = rec.indexOf("oshsitem_suid");
    const int ind_user_suid = rec.indexOf("user_suid");
    const int ind_estebleshed_id = rec.indexOf("estebleshed_id");

    QList< QSharedPointer<EM_OSHSDic::USER_IN_OSHS> > ret;
    while(q->next()){

        QSharedPointer<EM_OSHSDic::USER_IN_OSHS> newItem = QSharedPointer<EM_OSHSDic::USER_IN_OSHS>(new USER_IN_OSHS);
        newItem->_user_suid = q->value(ind_user_suid).toInt();
        newItem->_oshs_suid = q->value(ind_oshsitem_suid).toInt();
        newItem->_estebleshed_id = (TEstablishedPostType)q->value(ind_estebleshed_id).toInt();

        ret<<newItem;
    }

    delete q;
    db.close();

    return ret;
}

QList< QSharedPointer<EM_UserContainer> > EM_OSHSDic::getAppoint(const EM_OSHSItem* node) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(node==0) throw CommonException::NullParamException();

    QList< QSharedPointer<EM_UserContainer> > ret;

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(query_SelectEsteblishedPostAndUserForItem);
    q->bindValue(":node_suid",node->suid());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    int ind_login = rec.indexOf("login");
    int ind_estebleshed_id = rec.indexOf("estebleshed_id");

    QString login;
    TEstablishedPostType estebleshed_id;

    EM_AddressBook& addrBook = EM_AddressBook::Instance();
    EM_EsteblishedPostDic& estebBook = EM_EsteblishedPostDic::Instance();

    QMap<QString,QSharedPointer<EM_UserContainer> > mData;


    while(q->next()){
        login = q->value(ind_login).toString();
        estebleshed_id = (TEstablishedPostType)q->value(ind_estebleshed_id).toInt();

        QSharedPointer<EM_UserContainer> pt;
        if(!mData.contains(login)){
            EM_User* user = addrBook.getUser(login);
            if(user==0){
                qDebug()<<"User with login "<<login<<" not found";
                continue;
            }
            pt = QSharedPointer<EM_UserContainer>(new EM_UserContainer(user) );
            mData[login] = pt;
            ret<<pt;
        }else{
            pt = mData[login];
        }

        const EM_EsteblishedPost* item = estebBook.by(estebleshed_id);
        if(item==0){
            qDebug()<<"EsteblishedPost with id "<<QString::number(estebleshed_id)<<" not found";
            continue;
        }
        pt->add( new EM_EsteblishedPost(*item) );
    }

    delete q;
    db.close();

    return ret;
}

// Полуить список должностей пользователя
QList< QSharedPointer<EM_EsteblishedPost> > EM_OSHSDic::getEsteblishedPostForUser(const EM_User* user) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(user==0) throw CommonException::NullParamException();

    QList< QSharedPointer<EM_EsteblishedPost> > ret;

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(query_SelectEsteblishedPostForUser);
    q->bindValue(":user_suid",user->SUID());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    int ind_estebleshed_id = rec.indexOf("estebleshed_id");

    TEstablishedPostType estebleshed_id;

    EM_EsteblishedPostDic& estebBook = EM_EsteblishedPostDic::Instance();
    while(q->next()){

        estebleshed_id = (TEstablishedPostType)q->value(ind_estebleshed_id).toInt();

        const EM_EsteblishedPost* item = estebBook.by(estebleshed_id);
        if(item==0){
            qDebug()<<"EsteblishedPost with id "<<QString::number(estebleshed_id)<<" not found";
            continue;
        }
        ret<< QSharedPointer<EM_EsteblishedPost>(new EM_EsteblishedPost(*item));
    }

    delete q;
    db.close();

    return ret;
}

void EM_OSHSDic::remAppoint(const EM_OSHSItem* node,const EM_User* user, const EM_EsteblishedPost* esteblished) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(node==0 || user==0 || esteblished==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteAppoint);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":oshsitem_suid",node->suid());
    q->bindValue(":estebleshed_id",esteblished->getType());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
}

void EM_OSHSDic::addAppoint(const EM_OSHSItem* node,const EM_User* user, const EM_EsteblishedPost* esteblished, bool check )throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(node==0 || user==0 || esteblished==0) throw CommonException::NullParamException();


    if(check){
        QList< QSharedPointer<EM_EsteblishedPostEx> > ls = getEsteblishedPost(node);

        bool bFind = false;
        foreach (QSharedPointer<EM_EsteblishedPostEx> item, ls) {
            if(item->getType()==esteblished->getType()){
                bFind = true;
                break;
            }
        }
        if(!bFind){
            throw OSHSException::EsteblishedTypeException();
        }
    }

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_InsertAppoint);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":oshsitem_suid",node->suid());
    q->bindValue(":estebleshed_id",esteblished->getType());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
}


EM_OPERATION_RETURNED_STATUS EM_OSHSDic::fromDB() throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::AllocateMemoryException){

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }


    // структура для обработки не связанных ключей
    typedef struct KEYS{
        int key;
        int pkey;
    }* PKEYS;

    QSqlQuery *q = new QSqlQuery(db);


    q->prepare(query_SelectOSHSItem);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    int q_size = q->size();
    if(q_size<1) {
        return opOSHS_EMPTY;
    }

    int l_suid;
    int l_parent_suid;
    QString l_fname;
    QString l_sname;
    int l_level;
    int l_employee_count;


    // индексы столбцов
    QSqlRecord rec = q->record();
    int ind_suid = rec.indexOf("suid");
    int ind_parent_suid = rec.indexOf("parent_suid");
    int ind_fname = rec.indexOf("fname");
    int ind_sname = rec.indexOf("sname");    
    int ind_level  = rec.indexOf("level");
    int ind_employee_count  = rec.indexOf("employee_count");

    QMap<int,EM_OSHSItem*> mTree;


    int keys_size = 0;
    PKEYS keys = (PKEYS)malloc(sizeof(KEYS)*q_size);
    if(keys==0) throw CommonException::AllocateMemoryException();

    EM_OSHSItem *root = 0;

    while(q->next()){

        l_suid = q->value(ind_suid).toInt();
        l_parent_suid = q->value(ind_parent_suid).toInt();
        l_fname = q->value(ind_fname).toString();
        l_sname = q->value(ind_sname).toString();
        l_level = q->value(ind_level).toInt();
        l_employee_count = q->value(ind_employee_count).toInt();


        int pkey = l_parent_suid;
        int key = l_suid;


        EM_OSHSItem *newItem = new EM_OSHSItem(l_fname, l_sname,l_level,l_employee_count,key);
        newItem->setInternalState(EM_InternalState::INTERNAL_STATE_COMMIT_ITEM);
        mTree[key] = newItem;
        if(pkey==-1){
            root = newItem;
        }else{
            if(mTree.contains(pkey)){
                EM_OSHSItem *par = mTree[pkey];
//                par->addChild( newItem );
//                newItem->setParent( par );
                add(par,newItem);
            }else{
                keys[keys_size].key = key;
                keys[keys_size].pkey = pkey;
                keys_size++;
            }
        }
    }

    // если есть бессвязные элементы дополняем
    for(int i=0;i<keys_size;i++){
        // TODO: надо бы проверить на существование
        EM_OSHSItem *par = mTree[keys[i].pkey];
        EM_OSHSItem *item = mTree[keys[i].key];

//        par->addChild( item );
//        item->setParent( par );
        add(par,item);
    }

    setRootItem(root);

    free(keys);


    delete q;
    db.close();
    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_OSHSDic::toDB()throw (CommonException::OpenDBException,CommonException::SQLException){
    if(getRoot()==0) return opOSHS_EMPTY;

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
//    db.transaction();
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }
    QSqlQuery *qInsert = new QSqlQuery(db);
    QSqlQuery *qUpdate = new QSqlQuery(db);

    qInsert->prepare(query_InsertOSHSItem);
    qUpdate->prepare(query_UpdateOSHSItem);


    QStack<EM_OSHSItem*> stack;
    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_OSHSItem* cur = stack.pop();
        // выполняем обновления базы только для модифицированных и новых узлов
        if(cur->getInternalState()!=EM_InternalState::INTERNAL_STATE_COMMIT_ITEM){
            bool isNewItem = (cur->getInternalState()==EM_InternalState::INTERNAL_STATE_NEW_ITEM);

            QSqlQuery *pq = 0;
            if(isNewItem){
                pq = qInsert;
            }else{
                pq = qUpdate;
            }
            // ================================================================================
            // сейвим узел и ссылку на родителя
            if(cur->getParent()!=0){
                pq->bindValue(":parent_suid", (cur->getParent())->suid());
            }else{
                pq->bindValue(":parent_suid", -1);
            }

            pq->bindValue(":fname", cur->getFullTitle());
            pq->bindValue(":sname", cur->getShortTitle());
            pq->bindValue(":level", cur->getOSHSLevel());
            pq->bindValue(":employee_count", cur->getEemployeeCount());

            if(!isNewItem){
                pq->bindValue(":suid",cur->suid());
            }

            if(!pq->exec()){
                QString mes = pq->lastError().text();
                delete qInsert;
                delete qUpdate;
                db.close();
                throw CommonException::SQLException(mes);
            }

            if(isNewItem){
                int newId = -1;
                while(pq->next()){
                    newId = pq->value(0).toInt();
                }
                cur->suid(newId);
            }

            // ================================================================================
            cur->setInternalState(EM_InternalState::INTERNAL_STATE_COMMIT_ITEM); // признак сейва
        }
        for(EM_OSHSItem::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }



    delete qInsert;
    delete qUpdate;
//    db.commit();
    db.close();
    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_OSHSDic::removeFromDB()throw (CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteOSHS);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    // выставляем признак всем узлам
    if(getRoot()!=0){
        getRoot()->setInternalState(EM_InternalState::INTERNAL_STATE_NEW_ITEM,true);
    }

    delete q;
    db.close();
    return opOK;
}
EM_OPERATION_RETURNED_STATUS EM_OSHSDic::rem(EM_OSHSItem *item)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException){
    if(item==0) throw CommonException::NullParamException();

    QStringList lsCond;
    QString cond = "";
    QStack<EM_OSHSItem*> stack;
    stack.push( item );
    while(!stack.isEmpty()){
        EM_OSHSItem* cur = stack.pop();

        if(cur->getInternalState()!=EM_InternalState::INTERNAL_STATE_NEW_ITEM){
            lsCond<<QString::number(cur->suid());
//            if(!cond.isEmpty()){
//                cond+=", "+QString::number(cur->suid());
//            }else{
//                cond+=QString::number(cur->suid());
//            }
        }

        for(EM_OSHSItem::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);
    cond = lsCond.join(",");
    q->prepare(QString(query_DeleteOSHSItem).arg(cond));


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
void EM_OSHSDic::printOshsTree() const{
    qDebug()<<"OSHS Tree";
    QStack<EM_OSHSItem*> stack;
    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_OSHSItem* cur = stack.pop();
        QString ret = "";
        for(int i=1;i<cur->getLevel();i++){
            ret+="----";
        }
        ret+=" "+cur->getShortTitle()+" ("+cur->getID()+")";
        qDebug().noquote()<<ret;

        for(EM_OSHSItem::iterator iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }

    }
}
QByteArray EM_OSHSDic::serialize() const{
    QJsonArray a;

    EM_OSHSItem::iterator iter;
    QStack<EM_OSHSItem*> stack;
    stack.push( getRoot() );
    while(!stack.isEmpty()){
        EM_OSHSItem* cur = stack.pop();

        QJsonObject o;
        //o[field_title] = QJsonValue(cur->_title);

        a.append(o);


        for(iter = cur->begin();iter!=cur->end();++iter){
            stack.push(*iter);
        }
    }

    return QJsonDocument(a).toBinaryData();
}
int EM_OSHSDic::deserialize(const QByteArray& data){
    if(data.isNull()) return -1;

    int ret = 1;

    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isArray()){
        QMap<int,EM_OSHSItem*> mP;
        QMap<int,EM_OSHSItem*> m;
        foreach (QJsonValue iter, doc.object()) {
            if(iter.isObject()){
                QJsonObject o = iter.toObject();
                EM_OSHSItem* newItem =0;/* new EM_OSHSItem(
                            o[field_id].toInt(),
                            o[field_title].toString(),
                            o[field_descr].toString(),
                            o[field_object_id].toString(),
                            o[field_isRead].toBool(),
                            o[field_isWrite].toBool()
                            );*/
                int pid = 0;// = o[field_pid].toInt();
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

                EM_OSHSItem* item = mP[pid];
                EM_OSHSItem* p_item = m[pid];

                add(p_item,item);
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
// ------------------------------------------------------------------------------------
EM_UserContainer::EM_UserContainer(EM_User* user){
    _user = user;
}
void EM_UserContainer::add(EM_EsteblishedPost* item){
    if(item!=0){
        _esteblished<<item;
    }
}
EM_UserContainer::~EM_UserContainer(){
    foreach(EM_EsteblishedPost* iter, _esteblished){
        delete iter;
    }
}
const EM_User* EM_UserContainer::getUser() const{
    return _user;
}
QList<EM_EsteblishedPost*> EM_UserContainer::getEsteblishedPost() const{
    return _esteblished;
}

// ------------------------------------------------------------------------------------
EM_EsteblishedPost::EM_EsteblishedPost(TEstablishedPostType type, const QString &name, TEmployeeType group){
    _type = type;
    _name = name;
    _group = group;
}
EM_EsteblishedPost::EM_EsteblishedPost(const EM_EsteblishedPost& o){
    _type = o.getType();
    _name = o.getName();
    _group = o.getGroup();
}
TEstablishedPostType EM_EsteblishedPost::getType() const{return _type;}
const QString& EM_EsteblishedPost::getName() const{return _name;}
TEmployeeType EM_EsteblishedPost::getGroup() const{return _group;}
// ------------------------------------------------------------------------------------
EM_EsteblishedPostEx::EM_EsteblishedPostEx(TEstablishedPostType type, const QString &name, TEmployeeType group, int num):EM_EsteblishedPost(type,name,group){
    _num=num;
}
int EM_EsteblishedPostEx::getNum() const{return _num;}
//const QString& EM_EsteblishedPostEx::getName() const{return EM_EsteblishedPost::getName();}
// ------------------------------------------------------------------------------------
EM_EsteblishedPostDic::EM_EsteblishedPostDic()throw(CommonException::OpenDBException,CommonException::SQLException):_connect_name(generateConnectionCustomName("EsteblishedPostDic")){
    QSqlDatabase db = getConnection(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_SelectEsteblishedPost);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec =q->record();

    const int ind_type = rec.indexOf("type");
    const int ind_name = rec.indexOf("name");
    const int ind_group_type = rec.indexOf("group_type");

    TEstablishedPostType type;
    QString name;
    TEmployeeType group;

    while(q->next()){
        try{
            type = (TEstablishedPostType)q->value(ind_type).toInt();
        }catch(...){
            type = eptNone;
        }
        name = q->value(ind_name).toString();
        try{
            group = (TEmployeeType)q->value(ind_group_type).toInt();
        }catch(...){
            group = eltNone;
        }

        EM_EsteblishedPost* newItem = new EM_EsteblishedPost(type,name,group);
        mItem[type] = newItem;
    }

    delete q;

}
EM_EsteblishedPostDic::~EM_EsteblishedPostDic(){
    clear();
    QSqlDatabase::removeDatabase(_connect_name);
}
void EM_EsteblishedPostDic::clear(){
    QMap<TEstablishedPostType,EM_EsteblishedPost*>::iterator iter;
    for(iter = mItem.begin();iter!=mItem.end();++iter){
        delete (*iter);
    }
    mItem.clear();
}
EM_EsteblishedPost* EM_EsteblishedPostDic::Instance(TEstablishedPostType type,QString name,TEmployeeType group){
    return new EM_EsteblishedPost(type,name,group);
}

QMap<TEstablishedPostType,EM_EsteblishedPost*>::iterator EM_EsteblishedPostDic::begin(){
    return mItem.begin();
}
QMap<TEstablishedPostType,EM_EsteblishedPost*>::iterator EM_EsteblishedPostDic::end(){
    return mItem.end();
}
QList<EM_EsteblishedPost *> EM_EsteblishedPostDic::getEsteblishedPost() const{
    return mItem.values();
}
const EM_EsteblishedPost* EM_EsteblishedPostDic::by(TEstablishedPostType type) const{
    EM_EsteblishedPost* ret = 0;
    if(mItem.contains(type)){
        ret = mItem[type];
    }
    return ret;
}
EM_EsteblishedPost* EM_EsteblishedPostDic::find(TEstablishedPostType type) const{
    EM_EsteblishedPost* ret = 0;
    if(mItem.contains(type)){
        ret = mItem[type];
    }
    return ret;
}
QList<ACCESS_TYPES> parse(QByteArray raw_value)throw(JSONFormatException::ParseInputJSONException){
    QList<ACCESS_TYPES> ret;

    QJsonParseError  parseError;
    QJsonDocument doc = QJsonDocument::fromJson(raw_value, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("object")){
                QJsonValue object_type = obj["object"];
                if(!object_type.isString()|| object_type.toString().compare("access_template")!=0){
                    qDebug()<<"parseAccessList: "<<"Wrong object type";
                    throw JSONFormatException::ParseInputJSONException();
                }
                if(!obj.contains("items")|| !obj["items"].isArray()){
                    qDebug()<<"parseAccessList: "<<"Wrong object format";
                    throw JSONFormatException::ParseInputJSONException();
                }
                QJsonArray items = obj["items"].toArray();

                foreach (QVariant iter, items) {
                    if(iter.canConvert(QVariant::Int)){
                        ret<<(ACCESS_TYPES)iter.toInt();
                    }else{
                        qDebug()<<"parseAccessList: "<<"value in array wrong format";
                    }
                }
            }else{
                qDebug()<<"parseAccessList: "<<"Not found object";
                throw JSONFormatException::ParseInputJSONException();
            }
        }else{
            qDebug()<<"parseAccessList: "<<"Input data not object";
            throw JSONFormatException::ParseInputJSONException();
        }
    }else{
        qDebug()<<"parseAccessList: "<<parseError.errorString();
        throw JSONFormatException::ParseInputJSONException();
    }

    return ret;
}
QByteArray parse(const QList<ACCESS_TYPES> &ls){
    QString st_template = "{\"object\":\"access_template\",\"items\":[]}";
    QJsonParseError  parseError;
    QJsonDocument doc;
    if(!ls.isEmpty()){
        QJsonObject obj;
        QJsonArray arr;
        obj["object"] = QJsonValue("access_template");
        foreach (ACCESS_TYPES iter,ls){
            arr<< QJsonValue(iter);
        }
        obj["items"] = arr;

        doc.setObject(obj);
    }else{
        doc  = QJsonDocument::fromJson(st_template.toUtf8(), &parseError);
    }
    return doc.toJson(QJsonDocument::Compact);
}

QList<ACCESS_TYPES>  EM_EsteblishedPostDic::getAccessList(TEstablishedPostType type)throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_SelectEsteblishedPostAccess);
    q->bindValue(":type",type);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();

    QList<ACCESS_TYPES> ret;
    while(q->next()){
        QString raw_value = q->value(rec.indexOf("access_templ")).toString();
        ret = parse(raw_value.toUtf8());
    }
    delete q;
    db.close();
    return ret;
}
void EM_EsteblishedPostDic::update(TEstablishedPostType id,const QString& title, TEmployeeType group)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException){
    if(!mItem.contains(id)) throw CommonException::ObjNotFoundException();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_UpdateEsteblishedPost);
    q->bindValue(":name",title);
    q->bindValue(":group_type",group);
    q->bindValue(":type",id);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    EM_EsteblishedPost* item = mItem[id];
    item->_group = group;
    item->_name = title;

    delete q;
    db.close();
}

void EM_EsteblishedPostDic::setAccessList(TEstablishedPostType type,const QList<ACCESS_TYPES> &ls)throw(CommonException::OpenDBException,CommonException::SQLException){

    QByteArray value = parse(ls);


    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_UpdateEsteblishedPostAccess);
    q->bindValue(":type",type);
    q->bindValue(":access_templ",QString::fromUtf8(value));
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    delete q;
    db.close();
}

bool EM_EsteblishedPostDic::_instflag = false;
EM_EsteblishedPostDic* EM_EsteblishedPostDic::_instance = 0;
// ------------------------------------------------------------------------------------
