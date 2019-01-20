#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>

#include <QDebug>

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(int sockDesc, QObject *parent = Q_NULLPTR);
    ~MySocket();

signals:
    void dataReady(const QString &ip, const QByteArray &data);

public slots:
    void recvData(void);
    void sendData(int id, const QByteArray &data);

private:
    int m_sockDesc;
};

#endif // MYSOCKET_H
