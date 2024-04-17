#ifndef M_STACKEDWIDGET_H
#define M_STACKEDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>

class m_stackedwidget : public QStackedWidget
{
public:
    m_stackedwidget(QWidget *parent = 0);
    ~m_stackedwidget();
};

#endif // M_STACKEDWIDGET_H
