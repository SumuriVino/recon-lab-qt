#ifndef M_HOVERED_PUSHBUTTON_ROUND_CLOSE_H
#define M_HOVERED_PUSHBUTTON_ROUND_CLOSE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QtCore>
#include <QEvent>
#include <QHoverEvent>
#include <QIcon>

class m_hovered_pushbutton_round_close : public QPushButton
{
public:
    m_hovered_pushbutton_round_close(QWidget *parent = 0);

protected:

    bool event(QEvent *event);
};

#endif // M_HOVERED_PUSHBUTTON_ROUND_CLOSE_H
