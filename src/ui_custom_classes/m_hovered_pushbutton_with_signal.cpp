#include "m_hovered_pushbutton_with_signal.h"

m_hovered_pushbutton_with_signal::m_hovered_pushbutton_with_signal(QWidget *parent)
{
    installEventFilter(this);
}

m_hovered_pushbutton_with_signal::~m_hovered_pushbutton_with_signal()
{
}

bool m_hovered_pushbutton_with_signal::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Enter)
    {
        emit signal_hover_enter_or_leave(true,this);
    }
    else if(event->type() == QEvent::Leave)
    {
        emit signal_hover_enter_or_leave(false,this);
    }
    else if(event->type() == QEvent::MouseButtonDblClick)
    {
        emit signal_mouse_click_on_pushbutton(navigation_path);
    }

    return QWidget::event(event);
}

void m_hovered_pushbutton_with_signal::pub_set_navigation_path_on_pushsbutton(QString path)
{
    navigation_path = path;
}
