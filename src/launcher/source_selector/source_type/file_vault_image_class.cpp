#include "file_vault_image_class.h"
#include "ui_file_vault_image_class.h"

File_Vault_image_Class::File_Vault_image_Class(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::File_Vault_image_Class)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    ui->lineEdit_vault_password->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_vault_password->setEchoMode(QLineEdit::Password);
    ui->pushButton_show_hide_password->setIcon(QIcon("../icons/signs/eye_open.png"));

}

File_Vault_image_Class::~File_Vault_image_Class()
{
    delete ui;
}


void File_Vault_image_Class::pub_set_id_and_name(int id, QString name)
{
    received_id = id;
    received_name = name;
}

int File_Vault_image_Class::pub_get_id()
{
    return received_id;
}

int File_Vault_image_Class::pub_get_window_height()
{
    return this->minimumHeight();
}


void File_Vault_image_Class::on_pushButton_add_password_clicked()
{
    if(ui->lineEdit_vault_password->text() == "")
        return;

    emit signal_file_vault_password_inserted(ui->lineEdit_vault_password->text().trimmed());
    this->hide();
}

void File_Vault_image_Class::on_pushButton_cancel_clicked()
{
    this->hide();
}


void File_Vault_image_Class::on_pushButton_show_hide_password_clicked(bool checked)
{
    if(checked)
    {
        ui->pushButton_show_hide_password->setIcon(QIcon("../icons/signs/eye_close.png"));
        ui->lineEdit_vault_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pushButton_show_hide_password->setIcon(QIcon("../icons/signs/eye_open.png"));
        ui->lineEdit_vault_password->setEchoMode(QLineEdit::Password);
    }

}
