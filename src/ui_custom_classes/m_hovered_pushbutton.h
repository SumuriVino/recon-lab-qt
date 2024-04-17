#ifndef M_HOVERED_PUSHBUTTON_H
#define M_HOVERED_PUSHBUTTON_H

#include <QPushButton>
#include <QtCore>
#include <QEvent>
#include <QHoverEvent>
#include <QIcon>

class m_hovered_pushButton : public QPushButton
{
public:
    m_hovered_pushButton(QWidget *parent = 0);

protected:

bool event(QEvent *event);

};

#endif // M_HOVERED_PUSHBUTTON_H
