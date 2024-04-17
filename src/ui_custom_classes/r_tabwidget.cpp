#include "r_tabwidget.h"

R_Tabwidget::R_Tabwidget(QWidget *parent)
{
    this->setTabsClosable(true);
    this->setContentsMargins(4,3,3,3);

    this->setUsesScrollButtons(true);
    this->setElideMode(Qt::ElideNone);
}

R_Tabwidget::~R_Tabwidget()
{

}

