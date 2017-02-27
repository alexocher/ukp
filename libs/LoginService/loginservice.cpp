#include "loginservice.h"
#include <QDebug>
#include <QInputDialog>
#include "LoginDialog.h"
#include "../EntityManager/Headers/SimpleLogger.h"

EM_User*  input2system(int argc, char *argv[], bool admin,QWidget *parent){
    QString login;
    QString password;
    QString userid;

    const int x = 180;

    int i = 0;
    while(i<argc){
        if(QString(argv[i]).compare("-login")==0){
            login = QString(argv[i+1]);
            i+=2;
        }
        if(QString(argv[i]).compare("-password")==0){
            password = QString(argv[i+1]);
            i+=2;
        }
        if(QString(argv[i]).compare("-userid")==0){
            userid = QString(argv[i+1]);
            i+=2;
        }
        i++;
    }

    LogObserv* _log = new LogObserv();

    EM_AddressBook &addr = EM_AddressBook::Instance();
    EM_User*  ret = 0;
    if(!userid.isEmpty()){
        try{
            int user_id = userid.toInt();
            EM_User* user = addr.getUser(user_id);
            if(user!=0){
                if(admin && user->getRole()->getType()==EM_Role::ROLE_TYPE_ADMIN){
                    ret = user;
                }else if(!admin){
                    ret = user;
                }
            }else{
                QString mes = "User with id: "+userid+" not found";
                qDebug().noquote()<<mes;
                _log->warn(mes);
            }
        }catch(...){
            QString mes = "Error while parse user id: "+userid;
            qDebug().noquote()<<mes;
            _log->warn(mes);
        }
    }

    if(ret==0){
        int enter_ct = 0;
        do{
            const EM_User* user = addr.login(login,password);
            if(user!=0){
                if(admin && user->getRole()->getType()==EM_Role::ROLE_TYPE_ADMIN){
                    ret = addr.getUser(user->SUID());
                    break;
                }else if(!admin){
                    ret = addr.getUser(user->SUID());
                    break;
                }
            }
            if(enter_ct>0){
                _log->warn("Неудачная попытка входа в систему с логином: "+login);
            }
            LoginDialog dlg(parent);
            dlg.setWindowIcon(QIcon(":/icons/images/Main.png"));
            dlg.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
            dlg.setWindowTitle(admin?"Регистрация администратора":"Регистрация пользователя");
            dlg.setFixedSize(1.5*x,x);
            dlg.exec();
            if(!dlg.isOK()){
                break;
            }
            login = dlg.getLogin();
            password = dlg.getPassword();
            enter_ct++;
            // можно воткнуть счетчик попыток авторизации
        }while(true);
    }


    if(ret==0){
        _log->warn("Неудачная попытка входа в систему");
    }else{
        _log->log("Вход в систему пользователя: "+ret->getLogin());
    }
    delete _log;

    return ret;
}
