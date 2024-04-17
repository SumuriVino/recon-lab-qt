#ifndef CHAT_TEMPLATE_CLASS_H
#define CHAT_TEMPLATE_CLASS_H

#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"

class chat_template_class
{
public:
    chat_template_class();
    ~chat_template_class();

    static QString chat_page_start();
    static QString chat_page_end();
    static QString html_table_start();
    static QString html_table_row_data(QString data);
    static QString html_table_end();

    static QString html_left_align(QString data_str);
    static QString html_right_align(QString data_str);
private:


};

#endif // CHAT_TEMPLATE_CLASS_H
