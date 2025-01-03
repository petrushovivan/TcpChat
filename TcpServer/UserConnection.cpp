#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::udpSocketReadyRead(){
    while(udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        qDebug() << "Pong" << datagram.senderAddress();
        QByteArray response("Pong");
        udpSocket->writeDatagram(datagram.makeReply(response));
    }
}

QString MainWindow::getUserNamesString()
{
    QString result = "";
    for(QString s: usersNames){
        result += s + "$";
    }
    return result;
}
