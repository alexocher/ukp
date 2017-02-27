
#include "base_thread.h"

#include <QtNetwork>
#include <QDebug>

BaseThread::BaseThread(int socketDescriptor, QObject *parent): QThread(parent), socketDescriptor(socketDescriptor){
}

void BaseThread::run(){
    qDebug()<<"Get new connect..";
    QTcpSocket tcpSocket;

    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    // обработка
    qDebug()<<"Answer";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_5);
//    out << (quint16)0;
    out << "Wellcome to hell";
//    out.device()->seek(0);
//    out << (quint16)(block.size() - sizeof(quint16));


    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}

unsigned long BaseThread::threadID(){
    return (unsigned long)currentThreadId();
}

