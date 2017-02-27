#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H
#include <QMap>


#include "commondef.h"
#include "exceptions.h"

///////////////////////////////////////////////////////////////////////////////
// Классы адресной книги

class EM_AddressBook;
/**
 * @brief The Role class
 * Роль в системе
 */
class ENTITYMANAGERSHARED_EXPORT EM_Role:public AbstractEntity{
    friend class EM_AddressBook;
public:
    // коды ролей, для внутреннего использования
    enum ROLE_TYPE{
       ROLE_TYPE_TESTER = 0,        // тестовая
       ROLE_TYPE_ADMIN  = 10,       // администратор
       ROLE_TYPE_MANAGER= 20,       // управляющий
       ROLE_TYPE_WORKER = 66,       // работник
       ROLE_TYPE_DAEMON = 666,      // для програмной авторизации
    };

private:
    QString     m_sId;      // идентификатор
    QString     m_sTitle;   // название
    QString     m_sDescr;   // описание (или набор атрибутов в формате JSON)
    ROLE_TYPE   m_Type;     // код
protected:
    /**
     * @brief Role - закрытый конструктор
     * @param id - идентификатор роли (должен быть уникальным)
     * @param title - визуальное описание роли
     * @param descr - дополнительные атрибуты в формате JSON
     */
    EM_Role(const QString& id,const QString& title,const QString& descr,ROLE_TYPE type);

    void setID(QString const &v);
    void setTitle(QString const &v);
    void setDescr(QString const &v);
public:
    /**
     * @brief getID
     * @return идентификатор
     */
    QString const &getID() const;
    /**
     * @brief getTitle
     * @return визуальное представление
     */
    QString const &getTitle() const;
    /**
     * @brief getDescr
     * @return дополнительные атрибуты
     */
    QString const &getDescr() const;
    /**
     * @brief getType
     * @return код роли
     */
    ROLE_TYPE getType() const;
    QString toString() const;
};
/**
 * @brief The User class
 * Пользователь в системе
 */
class ENTITYMANAGERSHARED_EXPORT EM_User:public AbstractEntity{
    friend class EM_AddressBook;
private:
    QString m_sLogin;
    QString m_sPassword;
    EM_Role* m_Role;
    QString m_sDescr;
protected:
    /**
     * @brief User - закрытый конструктор
     * @param login - логин
     * @param password - пароль (возможно хеш)
     * @param role - указатель на роль
     * @param descr - дополнительные атрибуты в формате JSON
     */
    EM_User(const QString& login, const QString& password,EM_Role* role,const QString& descr);

    void setLogin(QString const &v);
    void setPassword(QString const &v);
    void setRole(EM_Role *v);
    void setDescr(QString const &v);
public:
    /**
     * @brief getLogin
     * @return логин
     */
    QString const &getLogin() const;
    /**
     * @brief getPassword
     * @return пароль
     */
    QString const &getPassword() const;
    /**
     * @brief getRole
     * @return Роль
     */
    EM_Role *getRole() const;
    /**
     * @brief getDescr
     * @return  дополнительные атрибуты
     */
    QString const &getDescr() const;
};


/**
 * @brief The AddressBook class
 * Адресная книга
 */
class ENTITYMANAGERSHARED_EXPORT EM_AddressBook{
private:
    QMap<QString,EM_Role*> mRoles; // таблица ролей
    QMap<QString,EM_User*> mUsers; // таблица пользователей
    QMap<int,EM_User*> mUsersSUID;

    const QString  _connect_name;

    bool bLogin;

    EM_AddressBook()throw(CommonException::OpenDBException,CommonException::SQLException); // прячим конструктор
    ~EM_AddressBook();
    void clear();

    EM_AddressBook(EM_AddressBook const&);
    EM_AddressBook& operator= (EM_AddressBook const&);

    static bool _instflag;
    static EM_AddressBook* _instance;
public:
    static EM_AddressBook* getInstance() throw(CommonException::OpenDBException,CommonException::SQLException){
        if(! _instflag){
            _instance = new EM_AddressBook();
            _instflag = true;
        }
        return _instance;
    }

    static EM_AddressBook& Instance() throw(CommonException::OpenDBException,CommonException::SQLException){
        return *getInstance();
    }


    QList<EM_Role*> getRoles() const; // получить все роли
    QList<EM_User*> getUsers() const; // получить всех пользователей

    EM_Role* getRole(const QString& id) const; // получить роль по идентификатору
    EM_Role* getRole(EM_Role::ROLE_TYPE type) const; // получить роль по идентификатору
    EM_User* getUser(const QString& login) const; // получить пользователя по логину
    EM_User* getUser(int user_suid) const; // получить пользователя по логину

    EM_User* addNewUser(const QString& login,const QString& password,EM_Role* role,const QString& descr) throw(AddressBookException::UserExistException,CommonException::NullParamException,CommonException::SQLException); // добавить новую роль
    EM_Role* addNewRole(const QString& id,const QString& title,const QString& descr,EM_Role::ROLE_TYPE type) throw(AddressBookException::RoleExistException,CommonException::SQLException); // добавить нового пользователя

    void updateUser(EM_User* user, const QString& login,const QString& password,EM_Role* role,const QString& descr)throw(CommonException::NullParamException,CommonException::SQLException); // изменить пользователя
    void updateRole(EM_Role* role,const QString& id,const QString& title,const QString& descr)throw(CommonException::SQLException); // изменить роль

    void removeRole(EM_Role* role)throw (CommonException::NullParamException, AddressBookException::RoleUseException,CommonException::SQLException); // удалить роль
    void removeUser(EM_User* user) throw(CommonException::NullParamException,CommonException::SQLException); // удалить пользователя

    void removeAll()throw(CommonException::SQLException); // удалить все   

    // Логинимся
    const EM_User* login(const QString& login, const QString& password);
    bool isLogin() const;
};



#endif // ADDRESSBOOK_H
