#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readMessage.clear();
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::socketConnected);
    connectedToServer = false;
    ui->widgetEmoji->hide();
    ui->sendMessageButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Обработка приема данных от сервера
void MainWindow::socketReadyRead()
{
    readMessage.append(socket->readAll());

    while ((unsigned int)readMessage.size() >= sizeof(uint32_t)) { // Проверяем, есть ли заголовок
        // Читаем длину сообщения
        uint32_t dataSize = 0;
        memcpy(&dataSize, readMessage.constData(), sizeof(uint32_t));

        // Проверяем, достаточно ли данных для полного сообщения
        if ((unsigned int)readMessage.size() < sizeof(uint32_t) + dataSize)
            return; // Полные данные еще не получены

        // Извлекаем сообщение
        QByteArray data = readMessage.mid(sizeof(uint32_t), dataSize);
        readMessage.remove(0, sizeof(uint32_t) + dataSize); // Удаляем обработанные данные

        // Обрабатываем полученное сообщение
        QString message = QString::fromUtf8(data);
        ui->chatEdit->append("Server:");
        ui->chatEdit->append(message);
    }
}

// Обработка отключения от сервера
void MainWindow::socketDisconnected()
{
    ui->sendMessageButton->setEnabled(false);
}

// Обработка подключения к серверу
void MainWindow::socketConnected()
{
    // Дождаться подключения к серверу можно и тут.
    ui->sendMessageButton->setEnabled(true);
}

void MainWindow::on_connectToServerButton_clicked()
{
    // "Координаты" сервера (IP-адрес и порт).
    QString serverIp = ui->serverIpEdit->text();
    QHostAddress serverAddress;

    if(!serverAddress.setAddress(serverIp)){
        QMessageBox::warning(this, "Внимание!", "Введенный вами ip не корректный");
        return;
    }

    bool portIsOk;
    ushort serverPort = ui->serverPortEdit->text().toUShort(&portIsOk);

    if(!portIsOk){
        QMessageBox::warning(this, "Внимание!", "Введенный вами порт не корректный");
        return;
    }

    // Подключаемся к серверу.
    // Может быть долго...

    socket->connectToHost(serverAddress, serverPort);

    // Дождаться подключения к серверу можно и тут.
    // Ожидание (10 сек) подключения ..

    bool connected = socket->waitForConnected(10000);
    ui->sendMessageButton->setEnabled(connected);
    if(!connected) {
        QMessageBox::information(this, "Внимание!", "Подключиться к серверу не удалось");
        socket->disconnectFromHost();
        connectedToServer = false;
    }
    else{
        connectedToServer = true;
    }
}

void MainWindow::on_sendMessageButton_clicked()
{
    // Веденное сообщение
    QString text = ui->sendMessageEdit->toPlainText();
    QByteArray data = text.toUtf8();

    uint32_t dataSize = data.size();

    QByteArray message;
    message.append((char*)(&dataSize), sizeof(dataSize));
    qDebug()<<message;
    message.append(data);

    socket->write(message);
    // Отображаем отправленное сообщение
    ui->chatEdit->append("ME");
    ui->chatEdit->append(text);
}

void MainWindow::on_sendMessageEdit_textChanged()
{
    if(ui->sendMessageEdit->toPlainText().length()>0 && connectedToServer){
        ui->sendMessageButton->setEnabled(true);
    }
    else{
        ui->sendMessageButton->setEnabled(false);
    }
}
