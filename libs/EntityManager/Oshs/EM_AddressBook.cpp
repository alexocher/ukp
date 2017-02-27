#include "../Headers/EM_AddressBook.h"
#include "../DBConnect/dbconnect.h"
#include <QtSql>

#define query_SelectRole    "SELECT "\
                            "   r.id AS role_id "\
                            "   ,r.title AS role_title "\
                            "   ,r.descr AS role_descr "\
                            "   ,r.type AS role_type " \
                            "   ,r.suid AS role_suid "\
                            " FROM oshs.role AS r"
#define query_Select    "SELECT "\
                        "   s.login AS login"\
                        "   ,s.password AS password "\
                        "   ,s.suid AS secure_suid "\
                        "   ,r.id AS role_id "\
                        "   ,r.title AS role_title "\
                        "   ,r.descr AS role_descr "\
                        "   ,r.type AS role_type " \
                        "   ,r.suid AS role_suid "\
                        "   ,u.descr AS user_descr "\
                        " FROM oshs.users AS u"\
                        " JOIN oshs.secure AS s ON u.secure_id = s.suid "\
                        " JOIN oshs.role AS r ON u.role_id = r.suid"

#define query_InsertSecure  "INSERT INTO oshs.secure (login, password) VALUES (:LOGIN, :PASSWORD) RETURNING suid"
#define query_InsertUser    "INSERT INTO oshs.users (role_id, secure_id,descr) VALUES (:role_id, :secure_id,:descr)"

#define query_UpdateSecure  "UPDATE oshs.secure SET login=:LOGIN, password = :PASSWORD WHERE suid = :suid "
#define query_UpdateUser    "UPDATE oshs.users SET role_id = :role_id, descr = :descr WHERE secure_id = :suid AND role_id = :old_role_id"
#define query_DeleteSecure  "DELETE FROM oshs.secure WHERE suid = :suid"

#define query_InsertRole    "INSERT INTO oshs.role (id, title, descr, type) VALUES (:ID, :TITLE, :DESCR, :TYPE) RETURNING suid"
#define query_UpdateRole    "UPDATE oshs.role SET id = :id, title=:title, descr = :descr WHERE suid = :suid"
#define query_DeleteRole    "DELETE FROM oshs.role WHERE suid = :suid"

#define query_DeleteAllRole    "DELETE FROM oshs.role"
#define query_DeleteAllSecure    "DELETE FROM oshs.secure"


#define MAIN_ADMIN "admin"
// --------------------------------------------------------------------------------
EM_Role::EM_Role(const QString& id, const QString& title, const QString& descr, ROLE_TYPE type):AbstractEntity(-1){
    m_sId = id;
    m_sTitle = title;
    m_sDescr = descr;
    m_Type = type;
}
QString const &EM_Role::getID() const{return m_sId;}
QString const &EM_Role::getTitle() const{return m_sTitle;}
QString const &EM_Role::getDescr() const{return m_sDescr;}
EM_Role::ROLE_TYPE EM_Role::getType() const{return m_Type;}
QString EM_Role::toString() const{return m_sId + " - "+ m_sTitle;}

void EM_Role::setID(QString const &v){m_sId=v;}
void EM_Role::setTitle(QString const &v){m_sTitle=v;}
void EM_Role::setDescr(QString const &v){m_sDescr=v;}
// --------------------------------------------------------------------------------
EM_User::EM_User(const QString& login, const QString& password,EM_Role* role,const QString& descr):AbstractEntity(-1){
    m_sLogin = login;
    m_sPassword = password;
    if(role==0){
        throw CommonException::NullParamException();
    }
    m_Role = role;
    m_sDescr = descr;
}
QString const &EM_User::getLogin() const{return m_sLogin;}
QString const &EM_User::getPassword() const{return m_sPassword;}
EM_Role* EM_User::getRole() const{return m_Role;}
QString const &EM_User::getDescr() const{return m_sDescr;}

void EM_User::setLogin(QString const &v){m_sLogin=v;}
void EM_User::setPassword(QString const &v){m_sPassword = v;}
void EM_User::setRole(EM_Role *v){m_Role = v;}
void EM_User::setDescr(QString const &v){m_sDescr = v;}
// --------------------------------------------------------------------------------
bool EM_AddressBook::isLogin() const{
    return bLogin;
}
const EM_User* EM_AddressBook::login(const QString& login, const QString& password){
    EM_User* ret = 0;
    if(mUsers.contains(login)){
        EM_User* user = mUsers[login];
        if(user->getPassword().compare(password)==0){
            ret = user;
            bLogin = true;
        }else{
            bLogin = false;
        }
    }
    return ret;
}
EM_AddressBook::EM_AddressBook()throw(CommonException::OpenDBException,CommonException::SQLException):_connect_name(generateConnectionCustomName("AddressBook")){
    bLogin = false;
    QSqlDatabase db = getConnection(_connect_name);
    if(!db.open()){
        throw CommonException::OpenDBException(db.lastError().text());
    }

    QSqlQuery *q = new QSqlQuery(db);

    // Загружаем роли
    q->prepare(query_SelectRole);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec1 = q->record();
    const int ind_r1_id = rec1.indexOf("role_id");
    const int ind_r1_suid = rec1.indexOf("role_suid");
    const int ind_r1_title = rec1.indexOf("role_title");
    const int ind_r1_descr = rec1.indexOf("role_descr");
    const int ind_r1_type = rec1.indexOf("role_type");
    while(q->next()){
        QString id = q->value(ind_r1_id).toString();
        EM_Role *role = new EM_Role(
                    id,
                    q->value(ind_r1_title).toString(),
                    q->value(ind_r1_descr).toString(),
                    (EM_Role::ROLE_TYPE)q->value(ind_r1_type).toInt()
                    );
        role->suid(q->value(ind_r1_suid).toInt());
        mRoles[id] = role;
     }



    // загружаем пользователей
    q->prepare(query_Select);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    QSqlRecord rec = q->record();
    const int ind_r_id = rec.indexOf("role_id");
    const int ind_r_suid = rec.indexOf("role_suid");
    const int ind_r_title = rec.indexOf("role_title");
    const int ind_r_descr = rec.indexOf("role_descr");
    const int ind_r_type = rec.indexOf("role_type");
    const int ind_s_suid = rec.indexOf("secure_suid");
    const int ind_s_login = rec.indexOf("login");
    const int ind_s_password = rec.indexOf("password");
    const int ind_u_descr = rec.indexOf("user_descr");


    QString role_id;
    long role_suid;
    QString role_title;
    QString role_descr;
    EM_Role::ROLE_TYPE role_type;
    long secure_suid;
    QString login;
    QString password;
    QString descr;

    bool bAdminFind = false;
    while(q->next()){
        role_id = q->value(ind_r_id).toString();
        EM_Role *role = 0;
        if(mRoles.contains(role_id)){
            role = mRoles[role_id];
        }else{
            role_suid = q->value(ind_r_suid).toInt();
            role_title = q->value(ind_r_title).toString();
            role_descr = q->value(ind_r_descr).toString();
            role_type =(EM_Role::ROLE_TYPE)q->value(ind_r_type).toInt();

            role = new EM_Role(role_id,role_title,role_descr,role_type);
            role->suid(role_suid);
            mRoles[role_id] = role;
        }

        secure_suid = q->value(ind_s_suid).toInt();
        login = q->value(ind_s_login).toString();
        password = q->value(ind_s_password).toString();
        descr = q->value(ind_u_descr).toString();

        EM_User *newUser = new EM_User(login,password,role,descr);
        newUser->suid(secure_suid);
        mUsers[login] = newUser;
        mUsersSUID[secure_suid] = newUser;

        if(login.compare(MAIN_ADMIN)==0){
            bAdminFind = true;
        }
    }
    // добавить админа при его отсутствии
    if(!bAdminFind){
        // TODO
    }

    delete q;
    db.close();
    qDebug()<<QString("Init AddressBook");
}
EM_User* EM_AddressBook::addNewUser(const QString& login,const QString& password,EM_Role* role,const QString& descr)throw(AddressBookException::UserExistException,CommonException::NullParamException,CommonException::SQLException){
    if(role==0){
        throw CommonException::NullParamException();
    }
    if(mUsers.contains(login)){
        throw AddressBookException::UserExistException();
    }

    //////////
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_InsertSecure);
    q->bindValue(":LOGIN",login);
    q->bindValue(":PASSWORD",password);

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
    //

    q->prepare(query_InsertUser);
    q->bindValue(":role_id",role->suid());
    q->bindValue(":secure_id",newId);
    q->bindValue(":descr",descr);

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    EM_User* newUser = new EM_User(login, password,role,descr);
    newUser->suid(newId);
    mUsers[login] = newUser;
    mUsersSUID[newId] = newUser;

    delete q;
    db.close();
    return newUser;
}

EM_Role* EM_AddressBook::addNewRole(const QString& id,const QString& title,const QString& descr,EM_Role::ROLE_TYPE type) throw(AddressBookException::RoleExistException,CommonException::SQLException){
    if(mRoles.contains(id)){
        throw AddressBookException::RoleExistException();
    }
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_InsertRole);
    q->bindValue(":ID",id);
    q->bindValue(":TITLE",title);
    q->bindValue(":DESCR",descr);
    q->bindValue(":TYPE",type);
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
    EM_Role* role = new EM_Role(id,title,descr,type);
    role->suid(newId);
    mRoles[id] = role;

    delete q;
    db.close();
    return role;
}
EM_AddressBook::~EM_AddressBook(){    
    clear();
    QSqlDatabase::removeDatabase(_connect_name);
    qDebug()<<QString("Kill AddressBook");    
}
void EM_AddressBook::clear(){
    for(QMap<QString,EM_User*>::iterator iter = mUsers.begin();iter!=mUsers.end();++iter){
        delete (*iter);
    }
    mUsers.clear();
    mUsersSUID.clear();
    for(QMap<QString,EM_Role*>::iterator iter = mRoles.begin();iter!=mRoles.end();++iter){
        delete (*iter);
    }
    mUsers.clear();
}

EM_Role* EM_AddressBook::getRole(const QString& id) const{
    EM_Role* ret = 0;
    if(mRoles.contains(id)){
        ret = mRoles[id];
    }
    return ret;
}
EM_Role* EM_AddressBook::getRole(EM_Role::ROLE_TYPE type) const{
    EM_Role* ret = 0;
    foreach (EM_Role* item, mRoles.values()) {
        if(item->getType()==type){
            ret = item;
            break;
        }
    }
    return ret;
}
EM_User* EM_AddressBook::getUser(const QString& login) const{
    EM_User* ret = 0;
    if(mUsers.contains(login)){
        ret = mUsers[login];
    }
    return ret;
}
EM_User* EM_AddressBook::getUser(int user_suid) const{
    EM_User* ret = 0;
    if(mUsersSUID.contains(user_suid)){
        ret = mUsersSUID[user_suid];
    }
    return ret;
}
QList<EM_User*> EM_AddressBook::getUsers() const{
    QList<EM_User*> ret;
    for(QMap<QString,EM_User*>::const_iterator iter = mUsers.begin();iter!=mUsers.end();++iter){
        if( (*iter)->getLogin().compare(MAIN_ADMIN)==0 ) continue;
        ret.operator <<(*iter);
    }
    return ret;
}
QList<EM_Role*> EM_AddressBook::getRoles() const{
    QList<EM_Role*> ret;
    for(QMap<QString,EM_Role*>::const_iterator iter = mRoles.begin();iter!=mRoles.end();++iter){
        if( (*iter)->getType()==EM_Role::ROLE_TYPE_DAEMON || (*iter)->getType()==EM_Role::ROLE_TYPE_TESTER ) continue;
        ret.operator <<(*iter);
    }
    return ret;
}

void EM_AddressBook::updateUser(EM_User* user, const QString& login,const QString& password,EM_Role* role,const QString& descr)throw(CommonException::NullParamException,CommonException::SQLException){
    if(role==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);

    q->prepare(query_UpdateSecure);
    q->bindValue(":LOGIN",login);
    q->bindValue(":PASSWORD",password);
    q->bindValue(":suid",user->suid());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    q->prepare(query_UpdateUser);
    q->bindValue(":role_id",role->suid());
    q->bindValue(":descr",descr);
    q->bindValue(":suid",user->suid());
    q->bindValue(":old_role_id",user->getRole()->suid());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }


    user->setLogin(login);
    user->setPassword(password);
    user->setRole(role);
    user->setDescr(descr);

    delete q;
    db.close();
}
void EM_AddressBook::updateRole(EM_Role* role,const QString& id,const QString& title,const QString& descr)throw(CommonException::SQLException){

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_UpdateRole);
    q->bindValue(":id",id);
    q->bindValue(":title",title);
    q->bindValue(":descr",descr);
    q->bindValue(":suid",role->suid());

    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }

    role->setID(id);
    role->setTitle(title);
    role->setDescr(descr);
    delete q;
    db.close();
}

void EM_AddressBook::removeRole(EM_Role* role)throw (CommonException::NullParamException,AddressBookException::RoleUseException,CommonException::SQLException){
    if(role==0) throw CommonException::NullParamException();

    QMap<QString,EM_User*>::const_iterator iter;
    for (iter = mUsers.begin();iter!=mUsers.end();++iter) {
        if(role->suid()==(*iter)->getRole()->suid()){
            throw AddressBookException::RoleUseException();
        }
    }
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteRole);
    q->bindValue(":suid",role->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
    mRoles.remove(role->getID());
    delete role;
}

void EM_AddressBook::removeUser(EM_User* user)throw (CommonException::NullParamException,CommonException::SQLException){
    if(user==0) throw CommonException::NullParamException();

    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteSecure);
    q->bindValue(":suid",user->suid());
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
    mUsers.remove(user->getLogin());
    delete user;
}

void EM_AddressBook::removeAll()throw(CommonException::SQLException){
    QSqlDatabase db = QSqlDatabase::database(_connect_name);
    if(!db.open())throw CommonException::OpenDBException(db.lastError().text());
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(query_DeleteAllRole);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    q->prepare(query_DeleteAllSecure);
    if(!q->exec()){
        QString mes = q->lastError().text();
        delete q;
        db.close();
        throw CommonException::SQLException(mes);
    }
    delete q;
    db.close();
    clear();

}
bool EM_AddressBook::_instflag = false;
EM_AddressBook* EM_AddressBook::_instance = 0;

