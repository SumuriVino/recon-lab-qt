#include "home_directory_class.h"
#include "ui_home_directory_class.h"

Home_Directory_Class::Home_Directory_Class(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Home_Directory_Class)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    ui->lineEdit_home_dir_user_name->setAttribute(Qt::WA_MacShowFocusRect,false);


}

Home_Directory_Class::~Home_Directory_Class()
{
    delete ui;
}

void Home_Directory_Class::pub_set_id_and_name(int id, QString name) //set reveieve id and received name
{
    received_id = id;
    received_name = name;
}

int Home_Directory_Class::pub_get_id() // receive id return
{
    return received_id;
}

int Home_Directory_Class::pub_get_window_height() //return window minimum height
{
    return this->minimumHeight();
}

void Home_Directory_Class::on_pushButton_add_username_clicked() //after adding username, click on add button
{
    if(ui->lineEdit_home_dir_user_name->text() == "")
        return;

    emit signal_home_directory_user_name_inserted(ui->lineEdit_home_dir_user_name->text().trimmed());
    this->hide();
}

void Home_Directory_Class::on_pushButton_cancel_clicked() //if don't want to add username, cancel button
{
    this->hide();
}
