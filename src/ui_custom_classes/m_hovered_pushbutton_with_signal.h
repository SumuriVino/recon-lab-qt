#ifndef M_HOVERED_PUSHBUTTON_WITH_SIGNAL_H
#define M_HOVERED_PUSHBUTTON_WITH_SIGNAL_H

#include <QtCore>
#include <QPushButton>
#include <QObject>

class m_hovered_pushbutton_with_signal : public QPushButton
{
    Q_OBJECT

    bool eventFilter(QObject *obj, QEvent *event);
public:
    m_hovered_pushbutton_with_signal(QWidget *parent=0);

    ~m_hovered_pushbutton_with_signal();
    void pub_set_navigation_path_on_pushsbutton(QString path);

signals:
    void signal_hover_enter_or_leave(bool enter_or_leave, m_hovered_pushbutton_with_signal *obj_val);
    void signal_mouse_click_on_pushbutton(QString dir_path);

private:

    QString navigation_path; // In File System - Dir navigation path

};

#endif // M_HOVERED_PUSHBUTTON_WITH_SIGNAL_H
