#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Dialog;
}

class Server;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = Q_NULLPTR);
    ~Dialog();

signals:
    void sendData(int id, const QByteArray &data);

public slots:
    void recvData(const QString &ip, const QByteArray &data);

private slots:
    void showConnection(int sockDesc);
    void showDisconnection(int sockDesc);
    void sendHexData(void);
    void sendDataSlot(void);
    void clearData(void);
    void startLoopSend(void);
    void stopLoopSend(void);

private:
    Ui::Dialog *ui;
    friend class Server;

    QTimer *m_timer;
    Server *m_server;

    int m_count;
};

#endif // DIALOG_H
