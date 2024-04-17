#ifndef report_template_html_advance_H
#define report_template_html_advance_H

#include "header_report.h"
#include "header_csv_template.h"
                                   
#include "header_plugin_extraction_interface.h"
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_2.h"


class report_template_html_advance
{
public:
    report_template_html_advance(QObject *parent = 0);
    ~report_template_html_advance();

    //----Generic_functions_for_plugin_report_generation
    void set_essential();

    QString report_end();
    QString report_main_end();
    QString report_set_html_header();
    QString report_main_set_html_header();
    QString report_set_agency_name_and_logo();
    QString report_main_set_tool_agency_name();
    QString report_set_tab_list(QStringList tab_list);
    QString report_drop_down_plugin_list(QStringList plugins_info_list);
    QString report_set_artifacts_source(QStringList list_artifacts_source);
    QString report_set_category_name_in_subnavigation_bar(QString category_name);
    QString report_main_set_sidebar_plugins_list(QStringList m_selected_plugins_info);
    QString report_insert_table_data(QStringList table_data_list, int report_preview);
    QString report_set_plugin_name_and_icon(QString plugin_name, QString parent_category);
    QString report_set_table_header(QStringList table_header_list, QStringList coloum_width_list);
    QString report_set_plugin_name_and_icon_on_no_data_available(QString plugin_name, QString parent_category);
    QString report_set_category_dropdownlist(QStringList category_list, QString current_pg_of_total_pg);
    QString report_main_set_agency_logo_and_name(QString logo_path, QString agency_name, QString tag_line);
    QString report_set_next_previous_pager_buttons(QString previous, QString next, QString first, QString last);

private:

    void clear_fields();
    void fill_variable_from_narad_muni();

};

#endif //
