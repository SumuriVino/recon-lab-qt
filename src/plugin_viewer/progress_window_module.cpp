#include "progress_window_module.h"
#include "ui_progress_window_module.h"

progress_window_module::progress_window_module(const QString &title, QWidget *parent) :
    QDialog(parent), w_title(title),
    ui(new Ui::progress_window_module)
{
    ui->setupUi(this);

    set_icon("../icons/sumuri_logo/recon_lab.png");
}

progress_window_module::~progress_window_module()
{
    delete ui;
}

void progress_window_module::set_status_message(QString data)
{
    ui->label->setText(data);
}

void progress_window_module::closeEvent(QCloseEvent *)
{
    emit signal_progress_window_module_closed();
}

void progress_window_module::set_module_name(QString name)
{
    QString set_title = name + " - Progress Window";
    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));
}

void progress_window_module::set_icon(QString icon_path)
{
    QPixmap p(icon_path); // load pixmap

    // get label dimensions
    int w = ui->label_icon->width();
    int h = ui->label_icon->height();

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label_icon->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
}
