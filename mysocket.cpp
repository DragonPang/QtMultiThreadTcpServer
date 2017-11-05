#include "mysocket.h"

MySocket::MySocket(int socketDescriptor, QObject *parent)
    : QTcpSocket(parent), socketDescriptor(socketDescriptor)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
}

void MySocket::sendMsg(QByteArray msg, int id)
{
    if (id == socketDescriptor)
    {
        write(msg);
    }
}

void MySocket::recvData()
{
    QByteArray data;
    QString peerAddr;

    data = readAll();

    QString temp = peerAddress().toString();
    peerAddr = temp.remove(0, 7);

    emit revData(peerAddr, data);
}
