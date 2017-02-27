#ifndef EM_POLICY
#define EM_POLICY

#include <QSharedPointer>
#include "BaseTree.h"
#include "exceptions.h"
#include "EM_AddressBook.h"


/*!
 * \brief Идентификаторы системных модулей
 */
#define sidALL_ACCESS           "SID_ALL_ACCESS"

#define sidADDRESS_BOOK         "SID_ADDRESS_BOOK"
#define sidAPP_LAUNCH_CONFIG    "SID_APP_LAUNCH_CONFIG"
#define sidDOCFLOW_EDITOR       "SID_DOCFLOW_EDITOR"
#define sidDOCFLOW              "SID_DOCFLOW"
#define sidOSHS_EDITOR          "SID_OSHS_EDITOR"
#define sidPOLICY_EDITOR        "SID_POLICY_EDITOR"
#define sidVIEW_ASSET           "SID_VIEW_ASSET"
#define sidYEAR_PLAN            "SID_YEAR_PLAN"
#define sidCARRY_PLAN           "SID_CARRY_PLAN"



class EM_PolicyDic;

/*!
 * \brief Класс описывает политику доступа к объекту
 */
class ENTITYMANAGERSHARED_EXPORT EM_Policy:public BaseTreeItem<int,int>,AbstractEntity{
    friend class EM_PolicyDic;
protected:
    QString     _title;     // название
    QString     _descr;     // доп атрибуты

    QString     _object_id; // объект контроля

    bool        _isRead;    // квалификатор доступа по чтению
    bool        _isWrite;   // квалификатор доступа по записи

protected:
    EM_Policy(int suid, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite);

public:
    ~EM_Policy(){;}

    class iterator{
            friend class EM_Policy;
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
            EM_Policy* operator*() const{return (EM_Policy*)*iter;}
        private:
            QList<BaseTreeItem*>::iterator iter;
    };

    int getSize() const;
    // получит родителя
    EM_Policy* getParent() const;
    // итераторы
    iterator begin();
    iterator end();
    // гетеры
    const QString& getTitle() const;
    const QString& getDescr() const;
    const QString& getObjectID() const;

    bool isRead() const;
    bool isWrite() const;
};
/*!
 * \brief Управление политиками
 */
class ENTITYMANAGERSHARED_EXPORT EM_PolicyDic:public BaseTree<int,int>{
private:
    QMap<QString,EM_Policy*> _mapID;

    const QString _connect_name;
    static EM_PolicyDic* _instance;
    static bool _instflag;

    bool _isLoad;
    EM_PolicyDic();
protected:
    EM_Policy * add(int parent_suid, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
public:
    typedef struct ACCESS{
        bool read;
        bool write;
    }P_ACCESS;

    /*!
     * \brief Получить экзумпляр словаря
     * \return объект словаря
     */
    static EM_PolicyDic& Instance();
    ~EM_PolicyDic();
    // загрузить из БД
    EM_OPERATION_RETURNED_STATUS fromDB()throw (CommonException::OpenDBException,CommonException::SQLException);
    // корневой элемент
    EM_Policy* getRoot()const;
    // добавление в словарь (сразу сейвится в базу)
    EM_Policy * add(EM_Policy *parent, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    EM_Policy * add(const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::OpenDBException,CommonException::SQLException);
    // изменение узла (сразу сейвится в базу)
    EM_OPERATION_RETURNED_STATUS update(EM_Policy *item, const QString& title, const QString& descr, const QString& objectID, bool isRead, bool isWrite)throw (CommonException::NullParamException,CommonException::OpenDBException,CommonException::SQLException);
    // удаление элемента (сразу сейвится в базу)
    EM_OPERATION_RETURNED_STATUS rem(EM_Policy *item)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // перемещение элемента (сразу сейвится в базу)
    EM_OPERATION_RETURNED_STATUS move(EM_Policy *parent,EM_Policy *item)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // вывод на консоль дерева
    void printOshsTree() const;
    // получить все политики для пользователя
    QMap<QString,QSharedPointer<ACCESS> > getPolicy(const EM_User* user)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // получить все политики для пользователя по идентификатору
    QMap<QString,QSharedPointer<ACCESS> > getPolicy(const EM_User* user,const QString& id)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // addUser
    EM_OPERATION_RETURNED_STATUS addUser(EM_Policy* policy,const EM_User* user)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // remUser
    EM_OPERATION_RETURNED_STATUS remUser(EM_Policy* policy,const EM_User* user)throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // getUser
    QList<EM_User*> getUser(const EM_Policy* policy)const  throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
    // проверка доступа юзера к системной палитике
    bool isAccess(const EM_User* user,const QString& head_id)const throw (CommonException::OpenDBException,CommonException::SQLException,CommonException::NullParamException);
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

#endif // EM_POLICY

