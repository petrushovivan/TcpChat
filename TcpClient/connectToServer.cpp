#include "mainwindow.h"

void MainWindow::connectToRemoteServer() {
    if(connector == nullptr){
        connector = new QUdpSocket(this);

    }
}
