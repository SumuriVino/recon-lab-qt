#include "blank.h"
#include "ui_blank.h"

blank::blank(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::blank)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
}

blank::~blank()
{
    delete ui;
}

void blank::pub_set_label_text(QString text)
{
    ui->label->setText(text);
}
