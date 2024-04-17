#include "m_hovered_pushbutton.h"

m_hovered_pushButton::m_hovered_pushButton(QWidget *parent) :
    QPushButton(parent)
{
    installEventFilter(this);
    // this->setIcon(QIcon("../icons/signs/close.png"));
}


bool m_hovered_pushButton::event(QEvent *event)
{
    if(event->type() == QEvent::Enter)
    {
        //this->setIcon(QIcon("../icons/signs/cancel.png"));

        this->setText("X");

    }
    if(event->type() == QEvent::Leave)
    {
        //this->setIcon(QIcon("../icons/signs/close.png"));
        this->setText("X");
    }

    return QWidget::event(event);
}

