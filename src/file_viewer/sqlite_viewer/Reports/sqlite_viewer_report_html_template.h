#ifndef REPORT_HTML_TEMPLATE_H
#define REPORT_HTML_TEMPLATE_H

#include <QtCore>
#include <QtGui>
#include<narad_muni/narad_muni.h>
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"

class sqlite_viewer_report_html_template
{
public:
    sqlite_viewer_report_html_template();
    static QString html_header1();
    static QString html_title(QString title);
    static QString html_header_after_title();
    static QString html_sidebar_start();
    static QString html_sidebar_end();
    static QString html_sidebar_link(QString module_name);
    static QString html_report_main_title(QString icon, QString title_name);
    static QString html_module_header(QString icon_path, QString report_content, QString module_name);
    static QString html_sub_module_header(QString module_name);

    static QString html_top_module_header(QString module_name);
    static QString html_top_link();

    static QString html_table_start();
    static QString html_table_headers(QStringList list,int val);
    static QString html_table_row_data(QStringList list , int val);
    static QString html_table_end();


};

#endif // REPORT_HTML_TEMPLATE_H
