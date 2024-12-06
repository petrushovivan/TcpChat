#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QList>
#include <QMessageBox>
#include <QNetworkDatagram>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Обработка подключения нового клиента
    void clientNewConnection();

    // Обработка приема данных от клиентов
    void clientReadyRead();

    // Обработка отключения клиента
    void clientDisconnected();

    void udpSocketReadyRead();

private:
    Ui::MainWindow *ui;

    ushort udpPort;
    ushort tcpPort;

    QUdpSocket *udpSocket;

    // TCP-сервер. Позволяет принимать входящие соединения.
    QTcpServer *server;

    // Список подключенных клиентов (сокетов).
    QList<QTcpSocket*> clients;

    QByteArray readMessage;

    QList<QString> usersNames;
};
#endif // MAINWINDOW_H
