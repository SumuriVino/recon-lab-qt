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


void File_Vault_image_Class::pub_set_id_and_name(int id, QString name) //Set id and source name in private variable
{
    received_id = id;
    received_name = name;
}

int File_Vault_image_Class::pub_get_id() //return source id
{
    return received_id;
}

int File_Vault_image_Class::pub_get_window_height() // return window height
{
    return this->minimumHeight();
}


void File_Vault_image_Class::on_pushButton_add_password_clicked() // when we fill password and click add button on file vault image
{
    if(ui->lineEdit_vault_password->text() == "")
        return;

    emit signal_file_vault_password_inserted(ui->lineEdit_vault_password->text().trimmed());
    this->hide();
}

void File_Vault_image_Class::on_pushButton_cancel_clicked() // cancel in case of no password
{
    this->hide();
}


void File_Vault_image_Class::on_pushButton_show_hide_password_clicked(bool checked) //Show/hide password on button after enterning password
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
