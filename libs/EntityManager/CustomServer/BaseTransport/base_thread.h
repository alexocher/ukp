#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include <QThread>
#include <QTcpSocket>

class BaseThread : public QThread{
    Q_OBJECT

public:
    BaseThread(int socketDescriptor, QObject *parent);

    void run() Q_DECL_OVERRIDE;

    unsigned long threadID();
signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;



};


#endif // BASE_THREAD_H
