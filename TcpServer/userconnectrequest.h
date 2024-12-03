#ifndef USERCONNECTREQUEST_H
#define USERCONNECTREQUEST_H

#include <QObject>
#include <QUdpSocket>

class UserConnectRequest : public QObject
{
    Q_OBJECT
public:
    UserConnectRequest();
public slots:
    void discoveryReadyRead();
private:
    QUdpSocket *discoverySocket;
};

#endif // USERCONNECTREQUEST_H
