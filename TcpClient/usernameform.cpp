#include "usernameform.h"
#include "ui_usernameform.h"

UserNameForm::UserNameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserNameForm)
{
    ui->setupUi(this);
    ui->pushButtonContinue->setEnabled(false);
}

UserNameForm::~UserNameForm()
{
    delete ui;
}

void UserNameForm::on_lineEditName_textChanged()
{
    QString text = ui->lineEditName->text();
    if(text.length()>2 && !text.contains(" ")){
        ui->pushButtonContinue->setEnabled(true);
    }
    else{
        ui->pushButtonContinue->setEnabled(false);
    }
}

void UserNameForm::closeEvent(QCloseEvent *e)
{
    if(ui->lineEditName->text().isEmpty()){
        int response = QMessageBox::information(this, "Внимание!", "Если вы закроете это окно то вам будет выдано имя по умолчанию!"
                                                , QMessageBox::Yes, QMessageBox::No);
        if(response == QMessageBox::Yes){
            ui->lineEditName->setText("User");
            on_pushButtonContinue_clicked();
        }
        else{
            e->ignore();
        }
    }
}

void UserNameForm::on_pushButtonContinue_clicked()
{
    if(ui->lineEditName->text().contains("$")){
        QMessageBox::information(this, "Внимание!", "Знак $ - служебный, его использовать в имени нельзя");
    }
    else{
        emit sendUserName(ui->lineEditName->text());
        this->close();
        this->deleteLater();
    }
}
