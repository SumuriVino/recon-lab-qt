#include "progress_bar_fixed_in_statusbar.h"
#include "ui_progress_bar_fixed_in_statusbar.h"

progress_bar_fixed_in_statusbar::progress_bar_fixed_in_statusbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progress_bar_fixed_in_statusbar)
{
    ui->setupUi(this);

    QString icon_path = "../icons/signs/cancel.png";
    QPixmap pixmap(icon_path); // load pixmap

    int w = ui->pushButton_close->width();
    int h = ui->pushButton_close->height();
    ui->pushButton_close->setIcon(QIcon(pixmap.scaled(w,h,Qt::KeepAspectRatio)));

    ui->pushButton_close->hide();

}

progress_bar_fixed_in_statusbar::~progress_bar_fixed_in_statusbar()
{
    delete ui;
}

void progress_bar_fixed_in_statusbar::set_status_message(QString msg)
{
    ui->label_status_message->setText(msg);
}

void progress_bar_fixed_in_statusbar::set_progressbar_size(QSize siz)
{
    // ui->label->setMinimumWidth(siz.width());
}

void progress_bar_fixed_in_statusbar::on_pushButton_close_clicked()
{
    emit signal_close_window();
}

void progress_bar_fixed_in_statusbar::set_progressbar_value(int value)
{
    ui->progressBar->setValue(value);
}

void progress_bar_fixed_in_statusbar::set_record_count_string(QString count_str)
{
    ui->label_record_displayed_from_tota_record->setText(count_str);
}

void progress_bar_fixed_in_statusbar::set_move_progressbar_in_left_corner(bool value)
{
    if(value)
        ui->label_record_displayed_from_tota_record->hide();
    else
        ui->label_record_displayed_from_tota_record->show();

    ui->progressBar->setFixedWidth(350);
}

int progress_bar_fixed_in_statusbar::get_progressbar_value()
{
    return ui->progressBar->value();
}

