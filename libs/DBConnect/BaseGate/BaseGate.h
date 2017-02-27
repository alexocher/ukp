#ifndef BASEGATE_H
#define BASEGATE_H

#include <QNetworkAccessManager>
#include "dbconnect.h"


class SHARED_EXPORT BaseGate{
    enum REQUEST_TYPE{
        PUT,GET,POST,DEL
    };
protected:
    enum GATE_TYPE{
        BDF = 1000,
        BGD = 2000
    };

private:
    QString m_url;                      // адрес запросов
    QNetworkAccessManager *m_pManager;  // менеджер сетевого соединения
private:
    // убрали конструктор копирования и оператор присваивания
    BaseGate();
    BaseGate(const BaseGate &o);
    BaseGate &operator=(const BaseGate &o);

    QByteArray _request(REQUEST_TYPE type, const QString &path, const QString &content_type, const QByteArray &data, int &sts);
    QByteArray _request(REQUEST_TYPE type,const QUrl &url,const QString &content_type,const QByteArray &data,int &sts);
    QByteArray _request(REQUEST_TYPE type,QNetworkRequest &request,const QString &content_type,const QByteArray &data,int &sts);

//    BaseGate(const QString& login,const QString& password,const QString& url, const QString& prefix, bool secure);
//    BaseGate(const QString& url, const QString& prefix, bool secure );
protected:
    QByteArray get(const QString &path, const QString &content_type);
    QByteArray get(const QUrl& url,const QString& content_type);
    QByteArray put(const QString &path, const QString &content_type, const QByteArray &data);
    QByteArray del(const QString &path, const QString &content_type);
    QByteArray post(const QString &path, const QString &content_type, const QByteArray &data);

    BaseGate(GATE_TYPE type);
public:
    virtual ~BaseGate();
    // Проверка подключения
    bool isConnect();
    // Показать путь
    const QString& showURL() const;
};

#endif // BASEGATE_H
