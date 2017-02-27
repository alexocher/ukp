#ifndef EM_OSHS_H
#define EM_OSHS_H



#include "commondef.h"
#include "exceptions.h"
#include "defdbconst.h"
#include "enums.h"
#include "BaseTree.h"
#include "EM_AddressBook.h"


#include <QSharedPointer>

// Коды меток доступа
enum ACCESS_TYPES{
    atBDF   = 1,    // BDF
    atOOBGD = 2,    // OOBGD
    atPLAN  = 10,   // Планы
    atDOC   = 20,   // Документы
    atREPORT= 30,   // Отчеты
};


class EM_OSHSDic;
class EM_EsteblishedPost;
class EM_EsteblishedPostEx;
class EM_UserContainer;
/**
 * @brief The EM_OSHSItem class
 * Базавый узел ОШС
 */
class ENTITYMANAGERSHARED_EXPORT EM_OSHSItem:public BaseTreeItem<int,int>,AbstractEntity,EM_InternalState{
    friend class EM_OSHSDic;
private:
    QString     _fullTitle;     // полное название
    QString     _shortTitle;    // короткое название
    int         _level;         // внутренний уровень
    int         _employeeCount; // численность по штату
protected:
    EM_OSHSItem(const QString& fullTitle, const QString& shortTitle,int level,int employeeCount,int suid);
    void setInternalState(INTERNAL_STATE state,bool child=false);
    void setModify();
public:
    ~EM_OSHSItem(){;}

    class iterator{
            friend class EM_OSHSItem;
            iterator(QList<BaseTreeItem*>::iterator el){iter = el;}
        public:
            // конструкторы
            iterator(){;}
            iterator(const iterator &it){iter=it.iter;}
            // присваивание итераторов – генерируется по умочанию
            // сравнение итераторов
            bool operator==(const iterator &it) const{return (iter==it.iter);}
            bool operator!=(const iterator &it) const{return !(iter==it.iter);}
            // продвижение к следующему элементу – только префиксная форма
            iterator& operator++(){
                ++iter;
                return *this;
            }
            // продвижение к предыдущему элементу – только префиксная форма
            iterator& operator--(){
                --iter;
                return *this;
            }
            // получить ссылку на информационную часть
            // работает справа и слева от знака присваивания
            EM_OSHSItem* operator*() const{return (EM_OSHSItem*)*iter;}
        private:
            QList<BaseTreeItem*>::iterator iter;
    };

    // получит родителя
    EM_OSHSItem* getParent() const;
    // итераторы
    iterator begin();
    iterator end();
    // методы доступа
    const QString& getFullTitle() const;     // полное название
    const QString& getShortTitle() const;    // короткое название
    int            getOSHSLevel() const;     // внутренний уровень
    int            getEemployeeCount() const;      // численность по штату

    void setFullTitle(const QString& value);  // полное название
    void setShortTitle(const QString& value);    // короткое название
    void setEmployeeCount(int value); // численность по штату
};

/**
 * @brief The EM_OSHSDic class
 * Словарь ОШС
 */
class ENTITYMANAGERSHARED_EXPORT EM_OSHSDic:public BaseTree<int,int>{
    friend class EM_OSHSItem;
    friend class EM_User;        
private:    
    EM_OSHSDic(EM_OSHSItem* rootItem);

    typedef struct USER_IN_OSHS{
        int _user_suid;
        int _oshs_suid;
        TEstablishedPostType _estebleshed_id;
    }* P_USER_IN_OSHS;
    QList< QSharedPointer<USER_IN_OSHS> > getListUserInOshs(EM_OSHSItem *oshsitem, bool isOneLevel=false);
    static QList< QSharedPointer<EM_UserContainer> > getListUserWithEsteblishedPost(int oshs_suid, const QString& cond)throw(CommonException::OpenDBException,CommonException::SQLException);
protected:
    const QString _connect_name;
    //QSqlDatabase db;
public:
    EM_OSHSDic();
    // Конструирования экземпляра словаря по корневому эементу
    static EM_OSHSDic* Instance(EM_OSHSItem* rootItem)throw (CommonException::NullParamException){
        if(rootItem == 0) throw CommonException::NullParamException();
        return new EM_OSHSDic(rootItem);
    }

    ~EM_OSHSDic();
    // Создать элемент словаря
    EM_OSHSItem* Instance(const QString& fullTitle, const QString& shortTitle,int level,int employeeCount);
    // удалить весь словарь из БД
    EM_OPERATION_RETURNED_STATUS removeFromDB()throw (CommonException::OpenDBException,CommonException::SQLException);
    // сохранить в БД
    EM_OPERATION_RETURNED_STATUS toDB()throw (CommonException::OpenDBException,CommonException::SQLException);
    // загрузить из БД
    EM_OPERATION_RETURNED_STATUS fromDB()throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::AllocateMemoryException);
    // корневой элемент
    EM_OSHSItem* getRoot()const;
    // добавление нового элемента
    void add(EM_OSHSItem *parent,EM_OSHSItem *item) throw (CommonException::NullParamException,OSHSException::DuplicateIDException);
    EM_OSHSItem* add(EM_OSHSItem *parent,const QString& fullTitle, const QString& shortTitle,int employeeCount)throw (CommonException::NullParamException);
    // удаление элемента
    EM_OPERATION_RETURNED_STATUS rem(EM_OSHSItem *item)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // перемещение элемента
    void move(EM_OSHSItem *parent,EM_OSHSItem *item);
    // вывод на консоль дерева (название узлов и подчиненность)
    void printOshsTree() const;
    // поиск элемента по идентификатору
    EM_OSHSItem* find(int id);
    // назначить пользователя на должность
    void addAppoint(const EM_OSHSItem* node,const EM_User* user, const EM_EsteblishedPost* esteblished,bool check=false) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // снять пользователя с должности
    void remAppoint(const EM_OSHSItem* node,const EM_User* user, const EM_EsteblishedPost* esteblished) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // получить список пользователей с должностями
    QList< QSharedPointer<EM_UserContainer> > getAppoint(const EM_OSHSItem* node) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // Полуить список должностей пользователя
    QList< QSharedPointer<EM_EsteblishedPost> > getEsteblishedPostForUser(const EM_User* user) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // Список должностей узла
    QList< QSharedPointer<EM_EsteblishedPostEx> > getEsteblishedPost(const EM_OSHSItem* node) throw(CommonException::NullParamException,CommonException::SQLException);
    // Добвавить должность в ОШС
    void addEsteblishedPost(const EM_OSHSItem* node,const EM_EsteblishedPost* item, int num) throw(CommonException::DuplicateObjectException,CommonException::NullParamException,CommonException::SQLException);
    // Удалить должность из ОШС
    void remEsteblishedPost(const EM_OSHSItem* node,const EM_EsteblishedPost* item) throw(CommonException::NullParamException,CommonException::SQLException);
    // Получить департамент пользователя
    EM_OSHSItem* getOshsItemByUser(const EM_User* user) throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    EM_OSHSItem* getOshsItemByUser(int user_suid) throw(CommonException::OpenDBException,CommonException::SQLException);
    // получить список людей с должностями по фильтру
    QList< QSharedPointer<EM_UserContainer> > getUserWithEsteblishedPost(const EM_User* user, QList<TEstablishedPostType> &filter,bool onelevel=false)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    QList< QSharedPointer<EM_UserContainer> > getUserWithEsteblishedPost(const EM_User* user, QList<TEmployeeType> &filter,bool onelevel=false)throw(CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    QList< QSharedPointer<EM_UserContainer> > getUserWithEsteblishedPost(int user_suid, QList<TEstablishedPostType> &filter,bool onelevel=false)throw(CommonException::OpenDBException,CommonException::SQLException);
    QList< QSharedPointer<EM_UserContainer> > getUserWithEsteblishedPost(int user_suid, QList<TEmployeeType> &filter,bool onelevel=false)throw(CommonException::OpenDBException,CommonException::SQLException);

    static QList< QSharedPointer<EM_UserContainer> > getListUserWithEsteblishedPost(int oshs_suid, QList<TEstablishedPostType> &filter,bool onelevel=false)throw(CommonException::OpenDBException,CommonException::SQLException);
    static QList< QSharedPointer<EM_UserContainer> > getListUserWithEsteblishedPost(int oshs_suid, QList<TEmployeeType> &filter,bool onelevel=false)throw(CommonException::OpenDBException,CommonException::SQLException);

    QMap<int,int> getUserIDWithOshsIDbyEsteblishedpost(TEstablishedPostType type,bool reverse = false) throw(CommonException::OpenDBException,CommonException::SQLException);

    /*!
     * \brief Сериалтзация
     * \return массив байт
     */
    QByteArray serialize() const;
    /*!
     * \brief Десериализация
     * \param data - массив байт
     * \return код статуса
     */
    int deserialize(const QByteArray& data);
};


class EM_EsteblishedPostDic;
// Класс Тип штатной должности
class ENTITYMANAGERSHARED_EXPORT EM_EsteblishedPost{
    friend class EM_EsteblishedPostDic;
    friend class EM_OSHSDic;
private:
    TEstablishedPostType _type;             // тип
protected:
    QString _name;                          // название
    TEmployeeType _group;                   // группа
protected:
    EM_EsteblishedPost(TEstablishedPostType type,const QString& name,TEmployeeType group);
    EM_EsteblishedPost(const EM_EsteblishedPost& o);
public:
    virtual ~EM_EsteblishedPost(){;}

    TEstablishedPostType getType() const;
    virtual const QString& getName() const;
    TEmployeeType getGroup() const;
};
// контейнер: данные о пользователе и его должностях
class ENTITYMANAGERSHARED_EXPORT EM_UserContainer{
    friend class EM_OSHSDic;
private:
    EM_User* _user;
    QList<EM_EsteblishedPost*> _esteblished;
protected:
    EM_UserContainer(EM_User* user);
    void add(EM_EsteblishedPost* item);
public:
    ~EM_UserContainer();
    // получить пользователя
    const EM_User* getUser() const;
    // получить список должностей
    QList<EM_EsteblishedPost*> getEsteblishedPost() const;
};
// класс с дополнением численности в подразделении
class ENTITYMANAGERSHARED_EXPORT EM_EsteblishedPostEx:public EM_EsteblishedPost{
private:
    int _num;
public:
    EM_EsteblishedPostEx(TEstablishedPostType type,const QString& name,TEmployeeType group,int num);
    ~EM_EsteblishedPostEx(){;}
    int getNum() const;
    //const QString& getName() const;
};
// словарь штатного расписания
class ENTITYMANAGERSHARED_EXPORT EM_EsteblishedPostDic{
private:
    QMap<TEstablishedPostType,EM_EsteblishedPost*> mItem;

    const QString _connect_name;

    EM_EsteblishedPostDic()throw(CommonException::OpenDBException,CommonException::SQLException); // прячим конструктор
    ~EM_EsteblishedPostDic();
    void clear();

    EM_EsteblishedPostDic(EM_EsteblishedPostDic const&);
    EM_EsteblishedPostDic& operator= (EM_EsteblishedPostDic const&);

    // переменные управления синглтоном
    static bool _instflag;
    static EM_EsteblishedPostDic* _instance;
public:
    static EM_EsteblishedPostDic* getInstance() throw(CommonException::OpenDBException,CommonException::SQLException){
        if(! _instflag){
            _instance = new EM_EsteblishedPostDic();
            _instflag = true;
        }
        return _instance;
    }
    // получение словаря
    static EM_EsteblishedPostDic& Instance() throw(CommonException::OpenDBException,CommonException::SQLException){
        return *getInstance();
    }

    EM_EsteblishedPost* Instance(TEstablishedPostType type,QString name,TEmployeeType group);
    // итераторы
    QMap<TEstablishedPostType,EM_EsteblishedPost*>::iterator begin();
    QMap<TEstablishedPostType,EM_EsteblishedPost*>::iterator end();

    QList<EM_EsteblishedPost *> getEsteblishedPost() const;
    // поиск по типу
    const EM_EsteblishedPost* by(TEstablishedPostType type) const;
    EM_EsteblishedPost* find(TEstablishedPostType type) const;
    // получить метки доступа
    QList<ACCESS_TYPES>  getAccessList(TEstablishedPostType type)throw(CommonException::OpenDBException,CommonException::SQLException);
    // Задать иетки доступа
    void setAccessList(TEstablishedPostType type,const QList<ACCESS_TYPES> &ls)throw(CommonException::OpenDBException,CommonException::SQLException);
    // редактирование названия и группы
    void update(TEstablishedPostType id,const QString& title, TEmployeeType group)throw(CommonException::OpenDBException,CommonException::SQLException,CommonException::ObjNotFoundException);
};


#endif // EM_OSHS_H
