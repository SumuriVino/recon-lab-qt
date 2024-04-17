#include "online_maps_dialog.h"
#include "ui_online_maps_dialog.h"

online_maps_dialog::online_maps_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::online_maps_dialog)
{
    ui->setupUi(this);

    view = new QWebEngineView(this);

    mainVbox = new QVBoxLayout;

    mainVbox->addWidget(view);

    setLayout(mainVbox);

}

online_maps_dialog::~online_maps_dialog()
{
    delete ui;
}

void online_maps_dialog::closeEvent(QCloseEvent *)
{
    emit signal_online_maps_closed();
}


void online_maps_dialog::pub_show_map(QString current_url_str)
{
   view->setUrl(current_url_str);
}
