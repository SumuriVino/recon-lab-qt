#ifndef HTML_ADVANCE_REPORT_MODULE_H
#define HTML_ADVANCE_REPORT_MODULE_H

#include <QObject>
#include <QWidget>

#include "header_report.h"
#include "export/export_for_reports/export_file.h"
#include "header_csv_template.h"
                                   
#include "header_global_structures.h"
//#include "export/thread_file_copier_module.h"
#include "header_plugin_extraction_interface.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "report/report_essentials/report_generic_function.h"
#include "report/report_essentials/report_template_html_advance.h"
#include "report/report_essentials/report_template_html_standard.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

class html_advance_report_module : public QObject
{
    Q_OBJECT
public:
    explicit html_advance_report_module(QObject *parent = 0);

    void set_essentials();
    void clear_exportfilelist_table();
    void set_narad_muni_for_report(narad_muni *mobj);

    void pub_set_plugins_and_tabs_info(QStringList plugins_info);

    void create_tag_report_advanced_html(QString file_path,QStringList selected_tablist,QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_full_report_advanced_html(QString file_path,QStringList selected_tablist,QStringList tabs_export_status_list,QStringList selected_tags_list);
    void create_screen_report_advanced_html(QString file_path,QStringList selected_tablist,QStringList tabs_export_status_list);

    void pub_cancel_report();
signals:

public slots:

private slots:

private:

    narad_muni *narad_muni_for_report_obj;
    QStringList selected_tabs_list;
    QStringList tab_name_list_stack;
    QStringList table_name_list_stack;
    QStringList selected_plugins_info;
    QStringList tab_name_list_textedit;
    QStringList table_name_list_textedit;
    QStringList tab_name_list_tablewidget;
    QStringList artifact_common_list_stack;
    QStringList table_name_list_tablewidget;
    QStringList artifact_common_list_textedit;
    QStringList report_html_header_list_stack;
    QStringList report_html_header_list_textedit;
    QStringList table_artifact_common_list_tablewidget;
    QStringList table_export_criteria_list_tablewidget;
    QStringList table_export_available_list_tablewidget;
    QStringList table_report_html_header_list_tablewidget;
    QStringList table_html_report_available_list_textedit;
    QStringList report_header_column_width_html_list_stack;
    QStringList table_html_report_available_list_stackwidget;
    QStringList table_html_report_available_list_tablewidget;
    QStringList report_header_column_width_html_list_textedit;
    QStringList table_report_header_column_width_html_list_tablewidget;

    QList<QStringList>tw_data_type_list_tablewidget;
    QList<QStringList>rt_html_order_list_tablewidget;
    QList<QStringList>report_html_col_name_list_stack;
    QList<QStringList>report_html_datatype_list_stack;
    QList<QStringList>rt_html_d_name_list_tablewidget;
    QList<QStringList>rt_html_col_name_list_tablewidget;
    QList<QStringList>rt_html_data_type_list_tablewidget;
    QList<QStringList>report_html_col_name_list_textedit;
    QList<QStringList>report_html_datatype_list_textedit;

    int max_record_per_page;

    QString reports_dir_path;
    QString recon_result_dir_path;
    QString parent_module_name_str;
    QString destination_file_location;
    QString  module_name_str;
    QStringList destination_db_file_list;


    export_file_module *export_file_obj;
    report_generic_function *report_generic_function_obj;
    report_template_html_advance *report_html_template_plugin_advance_obj;


    QString get_plugin_name();
    QString sub_report_no_data_available();
    QString export_virtual_file(QString file_path);
    QString sub_report_create_headers(int i_tab, QStringList header_name_list, QStringList header_width_list);
    QString sub_report_topbar_to_tab_header(QStringList list_artifacts_source, QString tab_name, QString current_pg_of_total_pg);
    QString sub_report_create_tablewidget_table_headers(int i_tab, QString is_export_available, QString is_multiple_export, bool tab_export_checked);

    QStringList collect_artifacts_source_list(QString report_type, QStringList selected_tags_list);

    void clear_fields();
    void fill_variable_from_narad_muni();

    void get_export_detail_html_final(QString column, QString tablename, bool bool_multiple_export, QString destination_db_file);

    void create_tablewidget_html_advanced_report(QString path , QString selected_tab_name, QString report_scope, QStringList selected_tags_list , bool tab_export_checked);
    void create_textedit_html_advanced_report(QString path , QString selected_tab_name, QString report_scope, QStringList selected_tags_list);
    void create_stackwidget_html_advanced_report(QString path , QString selected_tab_name, QString report_scope, QStringList selected_tags_list);

    void write_data_for_html_report_advance_full(QString report_scope, QString path, int i_tab, QStringList selected_tags_list, bool tab_export_checked);

    QString set_table_data(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int sr_no, QString destination_db_file , QSqlDatabase &dest_db);
    QString set_stack_data(QString command, int textedit_col_pos, QString report_type, QString report_scope, int sr_no, QString destination_db_file);
    QString set_textedit_data(QString command, int textedit_col_pos, QString report_type, QString report_scope, int sr_no, QString destination_db_file);

    QString get_virtual_preview_file(QString virtual_file_path);

    bool bool_cancel_report;
    recon_helper_standard *recon_helper_standard_obj;

    void write_data_for_html_report_advance_tags(QString report_scope, QString path, int i_tab, QStringList selected_tags_list, bool tab_export_checked);
    QString emlx_source_file;

};

#endif //
