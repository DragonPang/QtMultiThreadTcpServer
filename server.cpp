#include "dialog.h"
#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    /* get current dialog object */
    m_dialog = dynamic_cast<Dialog *>(parent);
}

Server::~Server()
{

}

void Server::incomingConnection(int sockDesc)
{
    m_socketList.append(sockDesc);

    serverThread *thread = new serverThread(sockDesc);

    m_dialog->showConnection(sockDesc);

    connect(thread, SIGNAL(disconnectTCP(int)), this, SLOT(clientDisconnected(int)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(dataReady(const QString&, const QByteArray&)),
            m_dialog, SLOT(recvData(const QString&, const QByteArray&)));

    connect(m_dialog, SIGNAL(sendData(int, const QByteArray&)),
            thread, SLOT(sendDataSlot(int, const QByteArray&)));

    thread->start();
}

void Server::clientDisconnected(int sockDesc)
{
    m_dialog->showDisconnection(sockDesc);
}
