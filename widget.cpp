#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //标题
    this->setWindowTitle("客户端");
    ui->pushButton->setDefault(true);
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->lineEdit->hide();
    ui->listWidget->hide();

    ui->label->show();
    ui->lineEdit_2->show();
    ui->pushButton_4->show();

}


void Widget::on_pushButton_2_clicked()
{
    QString ipAddress = "127.0.0.1";
    int port = 8010;

    if(!status)
    {
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, SIGNAL(connected()),
        this, SLOT(tcpConnected()));
        connect(tcpSocket, SIGNAL(disconnected()),
        this, SLOT(tcpDisconnected()));
        connect(tcpSocket, SIGNAL(readyRead()),
        this, SLOT(dataReceived()));
        tcpSocket->connectToHost(ipAddress, port);
        status = true;
    }
    else
    {
        int length = 0;
        QString message = userName + tr(":Leave Chat Room");
        length = tcpSocket->write(message.toLatin1(), message.length());

        if(length != message.length())
        {
            return;
        }

        tcpSocket->disconnectFromHost();
        status = false;
    }
}

void Widget::tcpConnected()
{
    int length = 0;
    ui->pushButton_2->setText("退出连接");
    QString message = userName + tr(":Enter Chat Room");
    length = tcpSocket->write(message.toLatin1(), message.length());

    if(length != message.length())
    {
        return;
    }
}

void Widget::tcpDisconnected()
{
    ui->pushButton_2->setText("连接服务");
    ui->lineEdit_2->show();
    ui->label->show();
    ui->pushButton_4->show();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->lineEdit->hide();
    ui->listWidget->hide();
}

void Widget::dataReceived()
{
    while(tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(), datagram.length());
        QString message = datagram.data();
        ui->listWidget->addItem(message);
    }
}

void Widget::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        return;
    }
    QString message = userName + tr(":") + ui->lineEdit->text();
    tcpSocket->write(message.toLatin1(), message.length());
    ui->lineEdit->clear();
}

void Widget::on_pushButton_3_clicked()
{
    ui->listWidget->clear();
}

void Widget::on_pushButton_4_clicked()
{
    if(ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::warning(this, tr("warning!!"),
                             tr("please input you Name!"));
        return;
    }
    else
    {
        userName = ui->lineEdit_2->text();
        ui->lineEdit_2->hide();
        ui->label->hide();
        ui->pushButton_4->hide();
        ui->pushButton->show();
        ui->pushButton_2->show();
        ui->pushButton_3->show();
        ui->lineEdit->show();
        ui->listWidget->show();
    }
}


Widget::~Widget()
{
    delete ui;
}
