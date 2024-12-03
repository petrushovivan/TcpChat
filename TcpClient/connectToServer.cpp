#include "mainwindow.h"

void MainWindow::createUDPSocket(){
    connector = new QUdpSocket(this);

    connector->bind();
    connect(connector, &QUdpSocket::readyRead, this, &MainWindow::connectorReadyRead);
    connector->bind();

    connect(&timer, &QTimer::timeout, this, &MainWindow::discovery);

    timer.start(500);

    discovery();
}

void MainWindow::connectorReadyRead(){
    QUdpSocket * server = (QUdpSocket *)sender();
    server->peerAddress().toString();
}

void MainWindow::discovery(){
    QByteArray request("Ping");
    connector->writeDatagram(request, QHostAddress::Broadcast, UDP_DISCOVERY_PORT);
}

void MainWindow::connectToRemoteServer() {

    // Подключаемся к серверу.
    // Может быть долго...

    // Дождаться подключения к серверу можно и тут.
    // Ожидание (10 сек) подключения ..

    bool connected = socket->waitForConnected(10000);
    if(!connected) {
        QMessageBox::information(this, "Внимание!", "Подключиться к серверу не удалось");
        socket->disconnectFromHost();
        connectedToServer = false;
    }
    else{
        connectedToServer = true;
    }
}
