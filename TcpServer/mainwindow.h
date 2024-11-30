#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMessageBox>

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
    void on_startButton_clicked();

    // Обработка подключения нового клиента
    void clientNewConnection();

    // Обработка приема данных от клиентов
    void clientReadyRead();

    // Обработка отключения клиента
    void clientDisconnected();

private:
    Ui::MainWindow *ui;

    // TCP-сервер. Позволяет принимать входящие соединения.
    QTcpServer *server;

    // Список подключенных клиентов (сокетов).
    QList<QTcpSocket*> clients;
};
#endif // MAINWINDOW_H
