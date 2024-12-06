#ifndef USERNAMEFORM_H
#define USERNAMEFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class UserNameForm;
}

class UserNameForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserNameForm(QWidget *parent = nullptr);
    ~UserNameForm();

    signals:
    void sendUserName(QString name);

private slots:
    void on_lineEditName_textChanged();

    void closeEvent(QCloseEvent *e) override;

    void on_pushButtonContinue_clicked();

private:
    Ui::UserNameForm *ui;
};

#endif // USERNAMEFORM_H
