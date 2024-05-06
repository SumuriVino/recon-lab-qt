#include "m_custom_source_button.h"

m_custom_source_button::m_custom_source_button(QWidget *parent):
    QPushButton(parent)
{
    bool_expandded = false;

}

void m_custom_source_button::pub_set_source_id(int id)
{
    source_id = id;
}

void m_custom_source_button::pub_set_source_name(QString name)
{
    source_name = name;
}

void m_custom_source_button::pub_set_os_scheme(QString scheme)
{
    os_scheme = scheme;
}

//QString m_custom_source_button::pub_get_os_scheme()
//{

//}

void m_custom_source_button::mousePressEvent(QMouseEvent *event)
{ // mouse pressed event which expand area to display source info
    if(bool_expandded)
    {
        emit signal_source_button_clicked(false,source_id,source_name,os_scheme);
        bool_expandded = false;
    }
    else
    {
        bool_expandded = true;
        emit signal_source_button_clicked(true,source_id,source_name,os_scheme);
    }

}

void m_custom_source_button::mouseDoubleClickEvent(QMouseEvent *event)
{
//    if(bool_expandded)
//    {
//        emit signal_source_button_clicked(false,source_id,source_name);
//        bool_expandded = false;
//    }
//    else
//    {
//        bool_expandded = true;
//        emit signal_source_button_clicked(true,source_id,source_name);
//    }
}
