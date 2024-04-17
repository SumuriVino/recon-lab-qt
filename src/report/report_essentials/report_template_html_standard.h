#ifndef report_template_html_standard_H
#define report_template_html_standard_H


#include "header_csv_template.h"

                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"

#include "header_report.h"

class report_template_html_standard
{
public:
    report_template_html_standard(QWidget *parent = 0);
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
    static QString html_table_headers(QStringList list, QStringList value);
    static QString html_table_row_data(QStringList list, int value);
    static QString html_table_end();

};

#endif //
