#include "progress_bar_window.h"
#include "ui_progress_bar_window.h"
#include <QKeyEvent>

progress_bar_window::progress_bar_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progress_bar_window)
{
    ui->setupUi(this);
    set_icon("../icons/sumuri_logo/recon_lab.png");

    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

}

progress_bar_window::~progress_bar_window()
{
    delete ui;
}

void progress_bar_window::keyPressEvent(QKeyEvent *obj)
{
    if(obj->key() == Qt::Key_Escape)
        return;

}

void progress_bar_window::set_status_message(QString msg)
{
    ui->label_status_message->setText(msg);
}

void progress_bar_window::set_window_title(QString title)
{
    setWindowTitle(title);
}

void progress_bar_window::closeEvent(QCloseEvent *)
{
    emit signal_close_window();
}

void progress_bar_window::set_application_modal()
{
    setWindowModality(Qt::ApplicationModal);
}

void progress_bar_window::set_window_modal()
{
    setWindowModality(Qt::WindowModal);

}
void progress_bar_window::set_icon(QString icon_path)
{
    QPixmap p(icon_path); // load pixmap

    // get label dimensions
    int w = ui->label_icon->width();
    int h = ui->label_icon->height();

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label_icon->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
}
