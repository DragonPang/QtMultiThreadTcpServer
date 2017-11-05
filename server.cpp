#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    /* get current dialog object */
    dialog = dynamic_cast<Dialog *>(parent);
}

Server::~Server()
{

}

void Server::incomingConnection(int socketDescriptor)
{
    socketList.append(socketDescriptor);

    serverThread *thread = new serverThread(socketDescriptor, 0);

    connect(thread, SIGNAL(started()), dialog, SLOT(showConnection()));
    connect(thread, SIGNAL(disconnectTCP(int)), dialog, SLOT(showDisconnection(int)));
    connect(thread, SIGNAL(revData(QString, QByteArray)), dialog, SLOT(revData(QString, QByteArray)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(dialog, SIGNAL(sendData(QByteArray, int)), thread, SLOT(sendData(QByteArray, int)));

    thread->start();
}

