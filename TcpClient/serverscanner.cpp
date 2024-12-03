#include "serverscanner.h"

ServerScanner::ServerScanner()
{
    timer = new QTimer();
    connector = new QUdpSocket();
    TcpPort = 40001;
}

void ServerScanner::connectToServer(QTcpSocket* server, ushort programPort)
{
    this->server = server;
    this->programPort = programPort;
    connector->bind();
    connect(connector, &QUdpSocket::readyRead, this, &ServerScanner::connectorReadyRead);
    connect(timer, &QTimer::timeout, this, &ServerScanner::discovery);
    timer->start(500);
    discovery();
}

void ServerScanner::stopTimer()
{
    if(timer->isActive())
        timer->stop();
}

void ServerScanner::connectorReadyRead()
{
    timer->stop();
    QNetworkDatagram datagram = connector->receiveDatagram();
    server->connectToHost(datagram.senderAddress(), TcpPort);
}

void ServerScanner::discovery(){
    QByteArray request("Ping");
    connector->writeDatagram(request, QHostAddress::Broadcast, programPort);
}
