#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QNetworkDatagram>
#include <QStringList>

#include "usernameform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // Обработка приема данных от сервера
    void socketReadyRead();
    // Обработка отключения от сервера
    void socketDisconnected();
    // Обработка подключения к серверу
    void socketConnected();
    // Желательно реализовавть еще и обработку ошибок сокета
    // void socketError(...);
private slots:
    void on_sendMessageButton_clicked();

    void on_pushButtonSmiley_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_sendMessageEdit_textChanged();

    void discovery();

    void udpSocketReadyRead();

    void setUserName(QString userName);

private:
    Ui::MainWindow *ui;

    UserNameForm* userNameForm;

    ushort udpPort;
    QUdpSocket *udpSocket;
    QTimer *timer;

    ushort tcpPort;
    QByteArray readMessage;
    bool connectedToServer;
    bool canStart;
    QTcpSocket *socket; // Клиентский сокет
};
#endif // MAINWINDOW_H
