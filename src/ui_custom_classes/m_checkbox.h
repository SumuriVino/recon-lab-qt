#ifndef M_CHECKBOX_H
#define M_CHECKBOX_H

#include <QObject>
#include <QWidget>
#include <QCheckBox>

class m_checkbox : public QCheckBox
{
public:
    m_checkbox(QWidget *parent = 0);
    ~m_checkbox();
};

#endif // M_CHECKBOX_H
