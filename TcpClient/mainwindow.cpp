#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);
    timer = new QTimer(this);
    userNameForm = new UserNameForm();

    connect(userNameForm, &UserNameForm::sendUserName, this, &MainWindow::setUserName);

    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::udpSocketReadyRead);
    connect(timer, &QTimer::timeout, this, &MainWindow::discovery);
    discovery();

    tcpPort = 40000;
    udpPort = 40001;
    canStart = false;
    connectedToServer = false;
    readMessage.clear();
    timer->start(500);

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::socketConnected);

    ui->widgetEmoji->hide();
    userNameForm->show();
    ui->sendMessageButton->setEnabled(false);
    ui->frameConnection->setStyleSheet("background-color: red;");
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
        QStringList list = message.split(" ");
        QString name = list.at(list.size()-1);
        list.pop_back();

        message.clear();
        for(QString s:list){
            message+=s;
        }
        ui->chatEdit->append(name);
        ui->chatEdit->append(message);
    }
}

// Обработка отключения от сервера
void MainWindow::socketDisconnected()
{
    ui->frameConnection->setStyleSheet("background-color: red;");
    ui->sendMessageButton->setEnabled(false);
    connectedToServer = false;
    timer->start(500);
}

// Обработка подключения к серверу
void MainWindow::socketConnected()
{
    ui->sendMessageButton->setEnabled(true);
}

void MainWindow::on_sendMessageButton_clicked()
{
    // Веденное сообщение
    QString text = ui->sendMessageEdit->toPlainText();
    QString name = ui->lineEditName->text();
    QByteArray data = (text + " " + name).toUtf8();

    uint32_t dataSize = data.size();

    QByteArray message;
    message.append((char*)(&dataSize), sizeof(dataSize));
    qDebug()<<message;
    message.append(data);

    socket->write(message);
    // Отображаем отправленное сообщение
    ui->chatEdit->append("ME");
    ui->chatEdit->append(text);
    ui->sendMessageEdit->setText("");
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
