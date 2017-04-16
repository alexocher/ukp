#ifndef EM_EXTPROC
#define EM_EXTPROC

#include <QMap>
#include "commondef.h"
#include "enums.h"
#include "exceptions.h"


#define NEW_VERSION 1
class EM_ExtProcDic;


class ENTITYMANAGERSHARED_EXPORT EM_ExtModule:public AbstractEntity{
    friend class EM_ExtProcDic;
protected:
    QString _title;
    QString _path;
    QString _descr;

    bool    _is_system;
    EM_ExtModule();
public:
    ~EM_ExtModule(){;}
    const QString& getTitle() const;
    const QString& getPath() const;
    const QString& getDescr() const;
    bool isSystem() const;
};
// Элемент классификатора внешних процедур
class ENTITYMANAGERSHARED_EXPORT EM_ExtProc:public AbstractEntity{
    friend class EM_ExtProcDic;
protected:
    QList<TExternalModuleType>  _lsModules;  // список кодов внешних модулей
    QString                     _title;      // название
    QMap<int,EM_ExtModule*>     _modules;    // список внешних модулей
protected:
    EM_ExtProc(int id,const QString title);

    void add(EM_ExtModule* m);
    void rem(EM_ExtModule* m);
public:
    ~EM_ExtProc();
    QList<TExternalModuleType> &getModules();
    const QString& getTitle() const;
    int getNum() const;
    const QMap<int,EM_ExtModule*>&  modules() const;    
};

class ENTITYMANAGERSHARED_EXPORT EM_ExtProcDic{
private:
    EM_ExtProcDic() throw(CommonException::OpenDBException,CommonException::SQLException);
    ~EM_ExtProcDic();
    void clear();

    const QString _connect_name;

    EM_ExtProcDic(EM_ExtProcDic const&);
    EM_ExtProcDic& operator= (EM_ExtProcDic const&);    
protected:
    QMap<int,EM_ExtProc*> _map;

    QMap<int,EM_ExtModule*> _mapM;
    QMap<QString,EM_ExtModule*> _mapMs;

    static bool _instflag;
    static EM_ExtProcDic* _instance;      
public:
    static EM_ExtProcDic* getInstance() throw(CommonException::OpenDBException,CommonException::SQLException){
        if(! _instflag){
            _instance = new EM_ExtProcDic();
            _instflag = true;
        }
        return _instance;
    }
    static EM_ExtProcDic& Instance() throw(CommonException::OpenDBException,CommonException::SQLException){
        return *getInstance();
    }
    // тестовый вывод на консоль
    void printDic()const;
    // получить по id
    EM_ExtProc* by(int id) const;
    //
    const EM_ExtModule* by(QString id) const;
    // добавить
    /*!
     * \warning DEPRICATED
     */
    EM_ExtProc* add(const QString& title,const QList<TExternalModuleType> & ls)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException);
    EM_ExtProc* add(const QString& title)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException);
    // удалить
    void rem(EM_ExtProc* proc)throw(CommonException::OpenDBException,CommonException::SQLException);
    void rem()throw(CommonException::OpenDBException,CommonException::SQLException);
    // обновить
    /*!
     * \warning DEPRICATED
     */
    void update(EM_ExtProc* proc,const QString& title,QList<TExternalModuleType> & ls)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException);
    void update(EM_ExtProc* proc,const QString& title)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException);
    // перегрузка словаря
    void reload()throw(CommonException::OpenDBException,CommonException::SQLException);
    // итераторы
    QMap<int,EM_ExtProc*>::const_iterator begin();
    QMap<int,EM_ExtProc*>::const_iterator end();

    QList<EM_ExtModule*> get();
    // операции редактирования
    void add(EM_ExtProc* proc,EM_ExtModule* module) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException);
    void rem(EM_ExtProc* proc,EM_ExtModule* module) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException);
    // добавить новый модуль
    EM_ExtModule* add(const QString& title,const QString& path, const QString& descr, bool system = false)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException);
    // добавить новый модуль и приципить его к процедуре
    EM_ExtModule* add(EM_ExtProc* proc,const QString& title,const QString& path, const QString& descr, bool system = false)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::GenPKException);
    // удалить модуль
    void rem(EM_ExtModule* module)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    void remModule()throw(CommonException::OpenDBException,CommonException::SQLException);
    // обновить
    void update(EM_ExtModule* module,const QString& title,const QString& path, const QString& descr, bool system = false)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException);
};

#endif // EM_EXTPROC

