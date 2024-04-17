#include "r_dockwidget.h"

r_dockwidget::r_dockwidget(QWidget *parent)
{

}

r_dockwidget::~r_dockwidget()
{

}

void r_dockwidget::closeEvent(QCloseEvent *eve)
{
    this->hide();
}
