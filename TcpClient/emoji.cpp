#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButtonSmiley_clicked()
{
    if(ui->widgetEmoji->isHidden()){
        ui->widgetEmoji->show();
    }
    else{
        ui->widgetEmoji->hide();
    }
}

void MainWindow::setUserName(QString userName) {
    ui->lineEditName->setText(userName);
}

void MainWindow::on_pushButton_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton->text());
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_2->text());
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_3->text());
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_4->text());
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_5->text());
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_6->text());
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_7->text());
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_8->text());
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_9->text());
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_10->text());
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_11->text());
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->sendMessageEdit->moveCursor(QTextCursor::End);
    ui->sendMessageEdit->insertPlainText(ui->pushButton_12->text());
}
