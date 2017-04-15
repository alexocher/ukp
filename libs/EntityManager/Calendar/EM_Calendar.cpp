#include "../Headers/EM_Calendar.h"
#include "../DBConnect/dbconnect.h"
#include <QtSql>

#define query_Cond      " (int_begin>=to_timestamp(:int_begin)::timestamp AND int_begin<=to_timestamp(:int_end)::timestamp) " \
                        " AND " \
                        " (int_end>=to_timestamp(:int_begin)::timestamp AND int_end<=to_timestamp(:int_end)::timestamp) "

#define query_InsertCalendarUser "INSERT INTO oshs.calendar_user "\
                                 " (user_suid,int_begin,int_end,title,descr,type) "\
                                 " VALUES " \
                                 " (:user_suid,to_timestamp(:int_begin)::timestamp,to_timestamp(:int_end)::timestamp,:title,:descr,:type) "\
                                 " RETURNING suid "
#define query_InsertCalendar     "INSERT INTO oshs.calendar "\
                                 " (int_begin,int_end,title,descr) "\
                                 " VALUES " \
                                 " (to_timestamp(:int_begin)::timestamp,to_timestamp(:int_end)::timestamp,:title,:descr) "\
                                 " RETURNING suid "
#define query_DeleteCalendar     "DELETE FROM oshs.calendar WHERE " \
                                 " (int_begin>=to_timestamp(:int_begin)::timestamp AND int_begin<=to_timestamp(:int_end)::timestamp) " \
                                 " AND " \
                                 " (int_end>=to_timestamp(:int_begin)::timestamp AND int_end<=to_timestamp(:int_end)::timestamp) "
#define query_DeleteCalendarUser "DELETE FROM oshs.calendar_user WHERE " \
                                 " (int_begin>=to_timestamp(:int_begin)::timestamp AND int_begin<=to_timestamp(:int_end)::timestamp) " \
                                 " AND " \
                                 " (int_end>=to_timestamp(:int_begin)::timestamp AND int_end<=to_timestamp(:int_end)::timestamp) " \
                                 " AND user_suid=:user_suid "
#define query_DeleteCalendarSUID "DELETE FROM oshs.calendar WHERE suid=:suid"
#define query_DeleteCalendarUserSUID "DELETE FROM oshs.calendar_user WHERE suid=:suid"

#define query_SelectCalendarUser "SELECT suid, user_suid,int_begin,int_end,title,descr,type "\
                                 "FROM oshs.calendar_user WHERE "\
                                 " (int_begin>=to_timestamp(:int_begin)::timestamp AND int_begin<=to_timestamp(:int_end)::timestamp) " \
                                 " AND " \
                                 " (int_end>=to_timestamp(:int_begin)::timestamp AND int_end<=to_timestamp(:int_end)::timestamp) " \
                                 " AND user_suid=:user_suid "
#define query_SelectCalendar     "SELECT suid, int_begin,int_end,title,descr "\
                                 "FROM oshs.calendar WHERE "\
                                 " (int_begin>=to_timestamp(:int_begin)::timestamp AND int_begin<=to_timestamp(:int_end)::timestamp) " \
                                 " AND " \
                                 " (int_end>=to_timestamp(:int_begin)::timestamp AND int_end<=to_timestamp(:int_end)::timestamp) "

#define query_DeleteBeginWork    "DELETE FROM stuff.param_dic WHERE id =:id"
#define query_InsertBeginWork    "INSERT INTO stuff.param_dic (id,value,format,comment) VALUES (:id,:value,:format,:comment)  "
#define query_SelectBeginWork    "SELECT value,format,comment FROM stuff.param_dic WHERE id=:id "
// ------------------------------------------------------------------------------
EM_CalendarUserItem::EM_CalendarUserItem(QDateTime begin, QDateTime end, QString title, QString   descr,TWorkPeriodType type):EM_CalendarItem(begin, end, title, descr){
    _type = type;
}
TWorkPeriodType EM_CalendarUserItem::getType() const{return _type;}
// ------------------------------------------------------------------------------
EM_CalendarItem::EM_CalendarItem(QDateTime begin, QDateTime end, QString title, QString   descr):AbstractEntity(-1){
    _begin = begin;
    _end = end;
    _title = title;
    _descr = descr;
}
const QDateTime& EM_CalendarItem::getBeginDate() const{return _begin;}
const QDateTime& EM_CalendarItem::getEndDate() const{return _end;}
const QString& EM_CalendarItem::getTitle() const{return _title;}
const QString& EM_CalendarItem::getDescr() const{return _descr;}
// ------------------------------------------------------------------------------
QSharedPointer<EM_CalendarItem> EM_CalendarDic::Instance(const QDateTime& begin,const QDateTime& end,const QString& title,const QString& descr){
    return QSharedPointer<EM_CalendarItem>(new EM_CalendarItem(begin, end, title, descr));
}
QSharedPointer<EM_CalendarUserItem> EM_CalendarDic::Instance(const QDateTime& begin,const QDateTime& end,const QString& title,const QString& descr, TWorkPeriodType type){
    return QSharedPointer<EM_CalendarUserItem>(new EM_CalendarUserItem(begin, end, title, descr,type));
}
void EM_CalendarDic::add(QSharedPointer<EM_CalendarUserItem> newItem,const EM_User* user)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(newItem.isNull() || user==0) throw CommonException::NullParamException();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_InsertCalendarUser);

    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":int_begin",newItem->getBeginDate().toTime_t());
    q->bindValue(":int_end",newItem->getEndDate().toTime_t());
    q->bindValue(":title",newItem->getTitle());
    q->bindValue(":descr",newItem->getDescr());
    q->bindValue(":type",newItem->getType());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    int newId = -1;
    while(q->next()){
        newId = q->value(0).toInt();
    }
    newItem->suid(newId);

    delete q;
    db.close();
}
void EM_CalendarDic::add(QSharedPointer<EM_CalendarItem> newItem)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(newItem.isNull()) throw CommonException::NullParamException();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_InsertCalendar);

    q->bindValue(":int_begin",newItem->getBeginDate().toTime_t());
    q->bindValue(":int_end",newItem->getEndDate().toTime_t());
    q->bindValue(":title",newItem->getTitle());
    q->bindValue(":descr",newItem->getDescr());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    int newId = -1;
    while(q->next()){
        newId = q->value(0).toInt();
    }
    newItem->suid(newId);

    delete q;
    db.close();
}
void EM_CalendarDic::rem(const QDateTime& begin,const QDateTime& end,const EM_User* user)throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    if(user==0){
        q->prepare(query_DeleteCalendar);
    }else{
        q->prepare(query_DeleteCalendarUser);
        q->bindValue(":user_suid",user->SUID());
    }
    q->bindValue(":int_begin",begin.toTime_t());
    q->bindValue(":int_end",end.toTime_t());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
}
void EM_CalendarDic::rem(int suid,const QString& mes)throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(mes);
    q->bindValue(":suid",suid);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
}

void EM_CalendarDic::rem(QSharedPointer<EM_CalendarItem> item)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(item.isNull()) throw CommonException::NullParamException();
    rem(item->SUID(),query_DeleteCalendarSUID);
}
void EM_CalendarDic::rem(QSharedPointer<EM_CalendarUserItem> item)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(item.isNull()) throw CommonException::NullParamException();
    rem(item->SUID(),query_DeleteCalendarUserSUID);
}
QList< QSharedPointer<EM_CalendarItem> > EM_CalendarDic::get(const QDateTime& begin,const QDateTime& end)throw(CommonException::OpenDBException,CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectCalendar);

    q->bindValue(":int_begin",begin.toTime_t());
    q->bindValue(":int_end",end.toTime_t());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    const int ind_suid = rec.indexOf("suid");
    const int ind_int_begin = rec.indexOf("int_begin");
    const int ind_int_end = rec.indexOf("int_end");
    const int ind_title = rec.indexOf("title");
    const int ind_descr = rec.indexOf("descr");

    QList< QSharedPointer<EM_CalendarItem> > ret;
    while(q->next()){
        QSharedPointer<EM_CalendarItem> newItem = Instance(
                    q->value(ind_int_begin).toDateTime(),
                    q->value(ind_int_end).toDateTime(),
                    q->value(ind_title).toString(),
                    q->value(ind_descr).toString()
                    );
        newItem->suid(q->value(ind_suid).toInt());
        ret<<newItem;
    }


    delete q;
    db.close();

    return ret;
}


QList< QSharedPointer<EM_CalendarUserItem> > EM_CalendarDic::get(const QDateTime& begin,const QDateTime& end,const EM_User* user)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException){
    if(user==0) throw CommonException::NullParamException();
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectCalendarUser);
    q->bindValue(":user_suid",user->SUID());
    q->bindValue(":int_begin",begin.toTime_t());
    q->bindValue(":int_end",end.toTime_t());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    const int ind_suid = rec.indexOf("suid");
    const int ind_int_begin = rec.indexOf("int_begin");
    const int ind_int_end = rec.indexOf("int_end");
    const int ind_title = rec.indexOf("title");
    const int ind_descr = rec.indexOf("descr");
    const int ind_type = rec.indexOf("type");

    TWorkPeriodType type;

    QList< QSharedPointer<EM_CalendarUserItem> > ret;
    while(q->next()){
        try{
            type = (TWorkPeriodType)q->value(ind_type).toInt();
        }catch(...){
            type = wptNone;
        }
        QSharedPointer<EM_CalendarUserItem> newItem = Instance(
                    q->value(ind_int_begin).toDateTime(),
                    q->value(ind_int_end).toDateTime(),
                    q->value(ind_title).toString(),
                    q->value(ind_descr).toString(),
                    type
                    );
        newItem->suid(q->value(ind_suid).toInt());
        ret<<newItem;
    }


    delete q;
    db.close();

    return ret;
}





#define CODE_BEGIN_WORK 10
#define FORMAT_TIME 3
#define CODE_DURATION_WORK 20
// время начала рабочего дня
const QTime EM_CalendarDic::getBeginWork(){
    QString v = getParam(CODE_BEGIN_WORK);
    QTime ret = QTime::fromString(v);
    if(!ret.isValid()){
        ret.setHMS(9,0,0);
        setBeginWork(ret);
    }
    return ret;
}
void EM_CalendarDic::setBeginWork(const QTime& v){
    if(v.isNull() || !v.isValid() ) return;
    setParam(CODE_BEGIN_WORK,v.toString(), FORMAT_TIME, "Начало рабочего дня");
}
QString EM_CalendarDic::getParam(int id){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_SelectBeginWork);
    q->bindValue(":id",id);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    QSqlRecord rec = q->record();
    const int ind_v = rec.indexOf("value");

    QString ret;
    while(q->next()){
        ret = q->value(ind_v).toString();
    }

    delete q;
    db.close();

    return ret;
}

void EM_CalendarDic::setParam(int id,const QString& value, int format, const QString& comment){


    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open()) throw CommonException::OpenDBException(db.lastError().text());

    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteBeginWork);
    q->bindValue(":id",id);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }


    q->prepare(query_InsertBeginWork);
    q->bindValue(":id",id);
    q->bindValue(":value",value);
    q->bindValue(":format",format);
    q->bindValue(":comment",comment);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }



    delete q;
    db.close();
}

QList< QSharedPointer<EM_CalendarUserItem> > EM_CalendarDic::get(const QDateTime& begin, const QDateTime& end,int user_suid)throw(AddressBookException::UserNotFoundException,CommonException::OpenDBException,CommonException::SQLException){
    EM_AddressBook &addr = EM_AddressBook::Instance();
    EM_User *user = addr.getUser(user_suid);
    if(user==0) throw AddressBookException::UserNotFoundException();
    return get(begin,end,user);
}
bool EM_CalendarDic::_instflag = false;
EM_CalendarDic* EM_CalendarDic::_instance = 0;

EM_CalendarDic::EM_CalendarDic():_connect_name(generateConnectionCustomName("CalendarDic")){
    getConnection(_connect_name);
}
EM_CalendarDic::~EM_CalendarDic(){
    QSqlDatabase::removeDatabase(_connect_name);
}
