#include "m_textedit.h"

m_textedit::m_textedit(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);

   // setWindowFlags(Qt::Window);
   // setStyleSheet("QTextEdit{background:#f1f1f1;}");
}

m_textedit::~m_textedit()
{

}

void m_textedit::focusOutEvent(QFocusEvent *event)
{
    emit signal_textedit_focusout_event_occured();
}

