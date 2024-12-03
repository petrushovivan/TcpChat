#ifndef SERVERSCANNER_H
#define SERVERSCANNER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QNetworkDatagram>
#include <QTcpSocket>

class ServerScanner: public QObject
{
    Q_OBJECT
public:
    void connectToServer(QTcpSocket* server, ushort programPort);
    void stopTimer();
    ServerScanner();
signals:
    void userCanSendMessage();

private:
        ushort TcpPort;
        QUdpSocket *connector;
        QTimer *timer;
        ushort programPort;
        QTcpSocket * server;
private slots:
        void connectorReadyRead();
        void discovery();
};

#endif // SERVERSCANNER_H
