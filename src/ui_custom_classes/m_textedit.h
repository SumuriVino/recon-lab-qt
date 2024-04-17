#ifndef M_TEXTEDIT_H
#define M_TEXTEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>

class m_textedit : public QTextEdit
{
    Q_OBJECT

public:
    m_textedit(QWidget *parent = 0);
    ~m_textedit();

signals:
    void signal_textedit_focusout_event_occured();

protected:
   void focusOutEvent(QFocusEvent *event);

};

#endif // M_TEXTEDIT_H
