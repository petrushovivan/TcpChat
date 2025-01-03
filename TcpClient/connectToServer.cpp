#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::discovery() {
    if(canStart){
    QByteArray request("Ping");
    udpSocket->writeDatagram(request, QHostAddress::Broadcast, udpPort);
    }
}

void MainWindow::udpSocketReadyRead() {
    timer->stop();
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    socket->connectToHost(datagram.senderAddress(), tcpPort);

    bool connected = socket->waitForConnected(10000);
    ui->sendMessageButton->setEnabled(connected);
    if(!connected) {
        ui->frameConnection->setStyleSheet("background-color: red;");
        socket->disconnectFromHost();
        connectedToServer = false;
    }
    else{
        connectedToServer = true;
        ui->frameConnection->setStyleSheet("background-color: green;");
    }
}
