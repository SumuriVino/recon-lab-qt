#include "detach_detailed_info.h"
#include "ui_detach_detailed_info.h"

detach_detailed_information::detach_detailed_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::detach_detailed_information)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

}

detach_detailed_information::~detach_detailed_information()
{
    delete ui;
}

void detach_detailed_information::closeEvent(QCloseEvent *eve)
{ // close deattach window if deattach
    emit close_detach_window(this);
}
