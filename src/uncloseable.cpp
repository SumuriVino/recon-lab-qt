#include "uncloseable.h"
#include "ui_uncloseable.h"

uncloseable::uncloseable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uncloseable)
{
    ui->setupUi(this);

    //setWindowFlags(Qt::Popup);
}

uncloseable::~uncloseable()
{
    delete ui;
}

void uncloseable::closeEvent(QCloseEvent *)
{
    exit(0);
}

void uncloseable::on_pushButton_quit_clicked()
{
    exit(0);
}
