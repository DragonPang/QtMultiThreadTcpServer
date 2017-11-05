#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QtNetwork>
#include <QDebug>

#include "mysocket.h"

class Socket;

class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(int socketDes, QObject *parent = 0);
    ~serverThread();
    void run();

public:
    int socketDescriptor;    

signals:
    void revData(QString, QByteArray);
    void sendDat(QByteArray data, int id);
    void disconnectTCP(int );

private slots:   
    void sendData(QByteArray data, int id);
    void recvData(QString, QByteArray);
    void disconnectToHost();

private:
    MySocket *socket;

};

#endif // SERVERTHREAD_H
