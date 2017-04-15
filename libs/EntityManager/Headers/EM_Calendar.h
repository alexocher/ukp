#ifndef EM_CALENDAR
#define EM_CALENDAR


#include "commondef.h"
#include "exceptions.h"
#include "enums.h"
#include "EM_AddressBook.h"
#include <QDateTime>
#include <QSharedPointer>
class EM_CalendarDic;

// Элементарная запись календаря
class ENTITYMANAGERSHARED_EXPORT EM_CalendarItem:public AbstractEntity{
    friend class EM_CalendarDic;
private:
    QDateTime _begin;   // дата/время начала
    QDateTime _end;     // дата/время завершения
    QString   _title;   // название
    QString   _descr;   // дополнительные атрибуты
protected:
    EM_CalendarItem(QDateTime begin, QDateTime end, QString title, QString   descr);
public:
    virtual ~EM_CalendarItem(){;}
    // функции доступа к полям
    const QDateTime& getBeginDate() const;
    const QDateTime& getEndDate() const;
    const QString& getTitle() const;
    const QString& getDescr() const;
};
// Календарь для пользователя
class ENTITYMANAGERSHARED_EXPORT EM_CalendarUserItem: public EM_CalendarItem{
    friend class EM_CalendarDic;
private:
    TWorkPeriodType _type; // тип интервала
protected:
    EM_CalendarUserItem(QDateTime begin, QDateTime end, QString title, QString   descr,TWorkPeriodType type);
public:
    ~EM_CalendarUserItem(){;}
    // функции доступа к полям
    TWorkPeriodType getType() const;

};
// Класс работы с календарями
class ENTITYMANAGERSHARED_EXPORT EM_CalendarDic{
private:
    EM_CalendarDic();
    ~EM_CalendarDic();

    EM_CalendarDic(EM_CalendarDic const&);
    EM_CalendarDic& operator= (EM_CalendarDic const&);

    const QString _connect_name;

    static bool _instflag;
    static EM_CalendarDic* _instance;

    void setParam(int id,const QString& value, int format, const QString& comment);
    QString getParam(int id);
protected:
    void rem(int suid,const QString& mes)throw(CommonException::OpenDBException,CommonException::SQLException);
public:
    static EM_CalendarDic* getInstance(){
        if(! _instflag){
            _instance = new EM_CalendarDic();
            _instflag = true;
        }
        return _instance;
    }
    static EM_CalendarDic& Instance() {
        return *getInstance();
    }
    // создание элементов
    QSharedPointer<EM_CalendarItem> Instance(const QDateTime& begin, const QDateTime& end,const QString& title,const QString&   descr);
    QSharedPointer<EM_CalendarUserItem> Instance(const QDateTime& begin, const QDateTime& end,const QString& title,const QString&   descr, TWorkPeriodType type);
    // добавить интервал
    void add(QSharedPointer<EM_CalendarUserItem> newItem,const EM_User* user)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    void add(QSharedPointer<EM_CalendarItem> newItem)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // удалить интервалы (если user==0 удаляем из годового плана)
    void rem(const QDateTime& begin, const QDateTime& end,const EM_User* user=0)throw(CommonException::OpenDBException,CommonException::SQLException);
    void rem(QSharedPointer<EM_CalendarItem> item)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    void rem(QSharedPointer<EM_CalendarUserItem> item)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // получить элементы плана
    QList< QSharedPointer<EM_CalendarItem> > get(const QDateTime& begin, const QDateTime& end)throw(CommonException::OpenDBException,CommonException::SQLException);
    QList< QSharedPointer<EM_CalendarUserItem> > get(const QDateTime& begin, const QDateTime& end,const EM_User* user)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    QList< QSharedPointer<EM_CalendarUserItem> > get(const QDateTime& begin, const QDateTime& end,int user_suid)throw(AddressBookException::UserNotFoundException,CommonException::OpenDBException,CommonException::SQLException);

    // время начала рабочего дня
    const QTime getBeginWork();
    void setBeginWork(const QTime& v);
    // продолжительность рабочего дня
    //const QTime getDurationWork() const;
    //void setDurationWork(const QTime& v) const;
};

#endif // EM_CALENDAR

