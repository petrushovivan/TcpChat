#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readMessage.clear();
    udpSocket = new QUdpSocket(this);
    tcpPort = 40000;
    udpPort = 40001;

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &MainWindow::clientNewConnection);

    bool serverOk = server->listen(QHostAddress::Any, tcpPort);

    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::udpSocketReadyRead);

    bool udpOk = udpSocket->bind(udpPort);
    if(!udpOk&&serverOk){
        QMessageBox::information(this, "Внимание!", "Сервер не работает!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Обработка подключения нового клиента
void MainWindow::clientNewConnection()
{
    // Пока есть ожидающие клиентские поключения
    while(server->hasPendingConnections()) {
        // Получаем и настраиваем клиентское подключение
        QTcpSocket *client = server->nextPendingConnection();
        connect(client, &QTcpSocket::readyRead, this, &MainWindow::clientReadyRead);
        connect(client, &QTcpSocket::disconnected, this, &MainWindow::clientDisconnected);

        // Сохраняем сокет подключенного клиента
        clients << client;

        // Отображаем IP подключенного клиента
        ui->clientListWidget->addItem(client->peerAddress().toString());
    }
}

// Обработка приема данных от клиентов
void MainWindow::clientReadyRead()
{
    QByteArray data;
    // Клиентский сокет, который прислал данные
    QTcpSocket *client = (QTcpSocket*)sender();

    readMessage.append(client->readAll());

    while ((unsigned int)readMessage.size() >= sizeof(uint32_t)) { // Проверяем, есть ли заголовок
        // Читаем длину сообщения
        uint32_t dataSize = 0;
        memcpy(&dataSize, readMessage.constData(), sizeof(uint32_t));

        // Проверяем, достаточно ли данных для полного сообщения
        if ((unsigned int)readMessage.size() < sizeof(uint32_t) + dataSize)
            return; // Полные данные еще не получены

        // Извлекаем сообщение
        data = readMessage.mid(sizeof(uint32_t), dataSize);
        readMessage.remove(0, sizeof(uint32_t) + dataSize); // Удаляем обработанные данные
    }

    if(data.contains(" ")){
        // Сообщение от клиента
        QString message = QString::fromUtf8(data);
        QString ip = client->peerAddress().toString();

        // Отображаем сообщение
        QString text = ip + "\n" + message + "\n";
        ui->chatEdit->append(text);

        QByteArray response;
        uint32_t dataSize = data.size();
        response.append((char*)(&dataSize), sizeof(dataSize));
        response.append(data);

        for(QTcpSocket *cli : clients) {
            if(cli != client)
                cli->write(response);
        }
    }
    else{
        QString userName = QString::fromUtf8(data);
        qDebug()<<userName;
        usersNames.append(userName);

        QByteArray users = getUserNamesString().toUtf8();
        QByteArray response;
        uint32_t dataSize = users.size();
        response.append((char*)(&dataSize), sizeof(dataSize));
        response.append(users);
        for(QTcpSocket *cli : clients) {
            cli->write(response);
        }
    }
}

// Обработка отключения клиента
void MainWindow::clientDisconnected()
{
    // Клиентский сокет, который отключился
    QTcpSocket *client = (QTcpSocket*)sender();

    // Удаляем клиента из списков
    int index = clients.indexOf(client);
    if(index >= 0) {
        usersNames.removeAt(index);
        clients.removeAt(index);
        ui->clientListWidget->takeItem(index);
    }
    QByteArray users = getUserNamesString().toUtf8();
    QByteArray response;
    uint32_t dataSize = users.size();
    response.append((char*)(&dataSize), sizeof(dataSize));
    response.append(users);
    for(QTcpSocket *cli : clients) {
        cli->write(response);
    }
}
