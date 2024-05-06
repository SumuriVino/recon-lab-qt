#include "display_loading_progress_bar.h"
#include "ui_display_loading_progress_bar.h"

display_loading_progress_bar::display_loading_progress_bar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::display_loading_progress_bar)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window| Qt::FramelessWindowHint);

    setWindowTitle(recon_static_functions::get_app_name());
    setWindowModality(Qt::ApplicationModal);
    this->move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());

}

display_loading_progress_bar::~display_loading_progress_bar()
{
    delete ui;
}

void display_loading_progress_bar::pub_set_window_modal(enum Qt::WindowModality wdt)
{ //set modality so window don't go behinde ofg previous window
    setWindowModality(wdt);
}

void display_loading_progress_bar::closeEvent(QCloseEvent *close)
{ // close progress bar window
    emit signal_close_loading_display_dialog(true);
}

void display_loading_progress_bar::pub_set_label_messsge(QString label_text)
{ // set message in dialog
    ui->label->setText(label_text);
}


void display_loading_progress_bar::pub_set_hidden_cancel_button(bool status)
{ // cancel button hide or not status

    ui->pushButton_canel->setHidden(status);
}

void display_loading_progress_bar::pub_set_progress_bar_title(QString title_str)
{ // progress bar title
    setWindowTitle(title_str);
}


void display_loading_progress_bar::pub_set_progress_bar_definite_value(int value)
{ // weather it has definite value
    ui->progressBar_records->setMinimum(0);
    ui->progressBar_records->setMaximum(100);

    ui->progressBar_records->setValue(value);
}

void display_loading_progress_bar::pub_set_progress_bar_indefinite_value()
{ // if it has indifinte value and needs to run unlimited
    ui->progressBar_records->setMinimum(0);
    ui->progressBar_records->setMaximum(0);

}

void display_loading_progress_bar::on_pushButton_canel_clicked()
{ // cancel button performs
    emit signal_close_loading_display_dialog(true);
}
