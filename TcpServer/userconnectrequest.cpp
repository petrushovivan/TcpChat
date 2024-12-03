#include "userconnectrequest.h"

UserConnectRequest::UserConnectRequest()
{
    discoverySocket = new QUdpSocket();
    connect(discoverySocket, &QUdpSocket::readyRead, this, &UserConnectRequest::discoveryReadyRead);

}
