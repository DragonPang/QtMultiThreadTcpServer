#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>

#include <QDebug>

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(int socket, QObject *parent = 0);

signals:
    void revData(QString, QByteArray);
public slots:
    void recvData();
    void sendMsg(QByteArray msg, int id);

private:
    int socketDescriptor;
};

#endif // MYSOCKET_H
