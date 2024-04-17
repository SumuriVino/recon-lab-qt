#include "m_hovered_pushbutton_round_close.h"

m_hovered_pushbutton_round_close::m_hovered_pushbutton_round_close(QWidget *parent) :
    QPushButton(parent)
{
    installEventFilter(this);
    this->setIcon(QIcon("../icons/signs/close.png"));

}

bool m_hovered_pushbutton_round_close::event(QEvent *event)
{
    if(event->type() == QEvent::Enter)
    {
        this->setIcon(QIcon("../icons/signs/cancel.png"));
    }
    if(event->type() == QEvent::Leave)
    {
        this->setIcon(QIcon("../icons/signs/close.png"));
    }

    return QWidget::event(event);
}
