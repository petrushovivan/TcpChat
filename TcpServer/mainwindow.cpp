#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
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
    ui->startButton->setEnabled(false);
    // Обработать результат listen
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
    // Клиентский сокет, который прислал данные
    QTcpSocket *client = (QTcpSocket*)sender();

    // Читаем данные от клиента.
    // TODO: FIX ME
    // Так как TCP ориентирован на потоковую передачу данных, то такой подход
    // не будет работать всегда (например, для больших сообщений).
    // Здесь требуется реализовать протокол прикладного уровня и алгоритм получения
    // фиксированных идентифицируемых сообщений (будет продемонстрировано в следующий раз).
    QByteArray buffer = client->readAll();

    // Сообщение от клиента
    QString message = QString::fromUtf8(buffer);
    // IP адрес клиента, отправившего сообщение
    QString ip = client->peerAddress().toString();

    // Отображаем сообщение
    QString text = ip + "\n" + message + "\n";
    ui->chatEdit->append(text);

    // Пересылаем сообщение остальным клиентам
    // TODO: FIX ME
    // Так как TCP ориентирован на потоковую передачу данных, то такой подход
    // не будет работать всегда (например, для больших сообщений).
    // Здесь требуется реализовать протокол прикладного уровня и алгоритм формирования
    // фиксированных идентифицируемых сообщений (будет продемонстрировано в следующий раз).
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
