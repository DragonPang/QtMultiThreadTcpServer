#include <QMessageBox>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog), count(0), timer(new QTimer)
{
    ui->setupUi(this);

    ui->loopStopBtn->setDisabled(true);
    ui->num->setText(QString("%1").arg(count));

    server = new Server(this);

    server->listen(QHostAddress::Any, 8712);

    connect(ui->sendBtn, SIGNAL(clicked()), this, SLOT(sendMsg()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearMsg()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendLoopMessage()));
    connect(ui->loopStopBtn, SIGNAL(clicked(bool)), this, SLOT(stopLoopSend()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showConnection()
{
    count++;

    /* add socket object that join in */
    ui->objectBox->addItem(QString("%1").arg(server->socketList.last()));

    /* change connect number while connection is connecting */
    ui->num->setText(QString("%1").arg(count));
}

void Dialog::showDisconnection(int socketDescriptor)
{
    count--;

    /* remove disconnect socketdescriptor from list */
    server->socketList.removeAll(socketDescriptor);

    /* reload combobox */
    ui->objectBox->clear();

    for (int i = 0; i < server->socketList.size(); i++) {
         ui->objectBox->addItem(QString("%1").arg(server->socketList.at(i)));
    }

    //change connect number while connection is disconnecting
    ui->num->setText(QString("%1").arg(count));
}

int charToHex(char c)
{
    if ((c >= '0') && (c <= '9')) {
        return c - 0x30;
    } else if ((c >= 'A') && (c <= 'F')) {
        return c - 'A' + 10;
    } else if ((c >= 'a') && (c <= 'f')) {
        return c - 'a' + 10;
    }

    return (-1);
}

void Dialog::sendMsg()
{
    /* if send message is null return */
    if (ui->sendMsg->text() == "") {
        QMessageBox::information(NULL,
                        tr("注意"),
                        tr("发送内容不能为空！"),
                        QMessageBox::Yes);

        return ;
    }

    if (ui->loopCheckBox->isChecked()) {
        int time_period = ui->periodLineEdit->text().toInt();

        timer->setInterval(time_period);

        timer->start();

        ui->sendBtn->setDisabled(true);
        ui->loopStopBtn->setDisabled(false);

        return;
    }

    /* whether send hex data */
    if (ui->hexCheckBox->isChecked()) {
        QString temp = ui->sendMsg->text();
        int temp_value;
        QByteArray data;

        QStringList list = temp.split(" ");
        foreach (QString str, list) {
            if (str.left(2) == "0x") {
                str = str.right(2);
            }
            temp_value = (charToHex(str.at(0).toLatin1()) << 4) + charToHex(str.at(1).toLatin1());
            data.append(temp_value);
        }
    } else {
        /* send original data */
        emit sendData(ui->sendMsg->text().toLocal8Bit(), ui->objectBox->currentText().toInt());
    }

    ui->sendMsg->setText("");
}

void stringToHtmlFilter(QString &str)
{
   str.replace("&","&amp;");
   str.replace(">","&gt;");
   str.replace("<","&lt;");
   str.replace("\"","&quot;");
   str.replace("\'","&#39;");
   str.replace(" ","&nbsp;");
   str.replace("\n","<br>");
   str.replace("\r","<br>");
}

void stringToHtml(QString &str, QColor color)
{
    QByteArray data;

    if (str.isEmpty()) {
        return;
    }

    data.append(color.red());
    data.append(color.green());
    data.append(color.blue());
    QString strColor(data.toHex());

    str = QString("<span style=\" color:#%1;\">%2</span>").arg(strColor).arg(str);
}

void Dialog::revData(QString peerAddr, QByteArray data)
{
    int i = 0;
    QString msg;

    if (ui->hexCheckBox->isChecked()) {
        QString dataString;

        foreach (QChar c, data.toHex()) {
            if (i % 2) {
                dataString += QString("%1 ").arg(c);
            } else {
                dataString += QString("0x%1").arg(c);
            }

            i++;
        }

        if (ui->addrCheckBox->isChecked()) {
            ui->msg->append(dataString);
            return ;
        }
        msg = dataString;
    } else {
        if (ui->addrCheckBox->isChecked()) {
            ui->msg->append(QString::fromLocal8Bit(data));
            return ;
        }

        msg = QString::fromLocal8Bit(data);
    }

    if (ui->circleCheckBox->isChecked()) {
        emit sendData(data, ui->objectBox->currentText().toInt());
    }

    peerAddr.insert(peerAddr.size(), ": ");
    stringToHtmlFilter(peerAddr);
    stringToHtml(peerAddr, QColor(255, 0, 0));

    msg.prepend(peerAddr);
    ui->msg->append(msg);
}

void Dialog::clearMsg()
{
    ui->msg->clear();
}

void Dialog::sendLoopMessage()
{
    emit sendData(ui->sendMsg->text().toLocal8Bit(), ui->objectBox->currentText().toInt());
}

void Dialog::stopLoopSend()
{
    timer->stop();
    ui->sendBtn->setDisabled(false);
    ui->loopStopBtn->setDisabled(true);
}
