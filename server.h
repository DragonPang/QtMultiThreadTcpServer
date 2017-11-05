#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

#include "serverthread.h"
#include "dialog.h"

class Dialog;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

public:
    QList<int> socketList;

private:
    void incomingConnection(int socketDescriptor);

private slots:

private:
    Dialog *dialog;

};

#endif // SERVER_H
