#include "display_progress_dialog_box.h"
#include "ui_display_progress_dialog_box.h"

display_progress_dialog_box::display_progress_dialog_box(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::display_progress_dialog_box)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window| Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    this->move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());

}

display_progress_dialog_box::~display_progress_dialog_box()
{
    delete ui;
}

void display_progress_dialog_box::pub_hide_cancel_button()
{
    ui->pushButton_cancel->hide();
}

void display_progress_dialog_box::pub_show_cancel_button()
{
    ui->pushButton_cancel->show();
}

void display_progress_dialog_box::pub_disable_cancel_button(bool status)
{
    ui->pushButton_cancel->setDisabled(status);
}

void display_progress_dialog_box::pub_set_label_message(QString str)
{
    ui->label_message->setText(str);
}

void display_progress_dialog_box::on_pushButton_cancel_clicked()
{
    emit signal_cancel_button_clicked();
}

void display_progress_dialog_box::pub_initialize_progress_bar()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
}


void display_progress_dialog_box::pub_set_progres_value(int percent)
{
    ui->progressBar->setValue(percent);
}
