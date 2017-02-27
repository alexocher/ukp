#ifndef BASE_SERVER_H
#define BASE_SERVER_H

#include "../../Headers/commondef.h"
#include <QTcpServer>


class BaseServerHolder;
class ENTITYMANAGERSHARED_EXPORT BaseServer : public QTcpServer{
    Q_OBJECT
friend class BaseServerHolder;
protected:
    BaseServer(QObject *parent = 0);

    QMap<unsigned long,QThread*> map;

    QString path2prop;
protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

    void initProp(QString& ip,int &port);

private:

};


class ENTITYMANAGERSHARED_EXPORT BaseServerHolder:public QObject{
    Q_OBJECT
private:
    BaseServer *m_Server;
    static BaseServerHolder *_instance;
    static bool _init;
protected:
    BaseServerHolder(QObject *parent = 0);
    ~BaseServerHolder();
public:
    static BaseServerHolder& Instance();
};

#endif // BASE_SERVER_H
