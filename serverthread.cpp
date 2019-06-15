#include "serverthread.h"

serverThread::serverThread(int sockDesc, QObject *parent) :
    QThread(parent),
    m_sockDesc(sockDesc)
{

}

serverThread::~serverThread()
{
    m_socket->close();
}

void serverThread::run(void)
{
    m_socket = new MySocket(m_sockDesc);

    if (!m_socket->setSocketDescriptor(m_sockDesc)) {
        return ;
    }

    connect(m_socket, &MySocket::disconnected, this, &serverThread::disconnectToHost);
    connect(m_socket, SIGNAL(dataReady(const QString&, const QByteArray&)),
            this, SLOT(recvDataSlot(const QString&, const QByteArray&)));
    connect(this, SIGNAL(sendData(int, const QByteArray&)),
            m_socket, SLOT(sendData(int, const QByteArray&)));

    this->exec();
}

void serverThread::sendDataSlot(int sockDesc, const QByteArray &data)
{
    if (data.isEmpty()) {
        return ;
    }

    emit sendData(sockDesc, data);
}

void serverThread::recvDataSlot(const QString &ip, const QByteArray &data)
{
    emit dataReady(ip, data);
}

void serverThread::disconnectToHost(void)
{
    emit disconnectTCP(m_sockDesc);
    m_socket->disconnectFromHost();
    this->quit();
}
