#include "hexviewer_progress_bar.h"
#include "ui_hexviewer_progress_bar.h"

hexviewer_progress_bar::hexviewer_progress_bar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hexviewer_progress_bar)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
}

hexviewer_progress_bar::~hexviewer_progress_bar()
{
    delete ui;
}
