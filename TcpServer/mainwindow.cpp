#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readMessage.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    if(ui->startButton->text() == "Старт"){
        // Создаем и настраиваем сервер
        server = new QTcpServer(this);
        connect(server, &QTcpServer::newConnection, this, &MainWindow::clientNewConnection);

        bool portOk;
        ushort port = ui->listenPortEdit->text().toUShort(&portOk);
        if(!portOk){
            QMessageBox::warning(this, "Внимание!", "Введенный вами порт не корректный");
            delete server;
            server = nullptr;
            return;
        }

        // Запускаем прослушивание порта сервером для ожидания клиентских подключений
        bool serverOk = server->listen(QHostAddress::Any, port);
        if(!serverOk){
            QMessageBox::warning(this, "Внимание!", server->errorString());
            delete server;
            server = nullptr;
            return;
        }
        ui->listenPortEdit->setReadOnly(true);
        ui->startButton->setText("Стоп");
    }
    else{
        if(server!=nullptr){
            delete server;
            server = nullptr;
        }
        ui->listenPortEdit->setReadOnly(false);
        ui->startButton->setText("Старт");
    }
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

    // Сообщение от клиента
    QString message = QString::fromUtf8(data);
    // IP адрес клиента, отправившего сообщение
    QString ip = client->peerAddress().toString();

    // Отображаем сообщение
    QString text = ip + "\n" + message + "\n";
    ui->chatEdit->append(text);

    for(QTcpSocket *cli : clients){
        if(cli != client)
            cli->write(text.toUtf8());
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
        clients.removeAt(index);
        ui->clientListWidget->takeItem(index);
    }
}
