#ifndef HTML_STANDARD_REPORT_MODULE_H
#define HTML_STANDARD_REPORT_MODULE_H

#include <QObject>
#include <QWidget>
#include "recon_generic_functions/recon_static_functions.h"
#include "report/report_essentials/report_template_html_standard.h"
#include "header_global_structures.h"
//#include "export/export_for_reports/thread_file_copier_module.h"

#include "report/report_essentials/report_template_html_advance.h"
#include "report/report_essentials/report_status.h"

#include <QTextEdit>
#include <QStackedWidget>
#include "ui_custom_classes/m_tablewidget.h"

#include "export/export_for_reports/export_file.h"
#include "header_global_variables_3.h"
#include "header_global_variables_1.h"
#include "header_report.h"
#include "header_global_variables_2.h"
#include "report/report_essentials/report_generic_function.h"
#include "recon_generic_functions/recon_helper_standard.h"      


class html_standard_report_module : public QObject
{
    Q_OBJECT
public:
    explicit html_standard_report_module(QObject *parent = 0);

    void create_full_report_standard_html(QString file_path,  QStringList selected_tablist, QStringList tabs_export_status_list,QStringList selected_tags_list);
    void create_tag_report_standard_html( QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_screen_report_standard_html(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list);
    void clear_exportfilelist_table(void);

    void set_essentials();
    void set_narad_muni_for_report(narad_muni *mobj);
    void pub_cancel_report();


    void pub_set_bool_for_pdf_report();


signals:

public slots:

private slots:


private:

    export_file_module *export_file_obj;

    narad_muni *narad_muni_for_report_obj;
    QList<m_tablewidget *> list_m_tablewidgets;

    void create_tablewidget_html_report(QFile &file, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked);
    void create_textwidget_html_report(QFile &file, QString selected_tab_name, QString report_scope, QStringList selected_tags_list);
    void create_stackwidget_html_report(QFile &file, QString selected_tab_name, QString report_scope, QStringList selected_tags_list);

    void set_tablewidget_data_new(QFile &html_report_file, int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int sr_no, QString destination_db_file, QSqlDatabase &dest_db);
    QString set_tablewidget_data(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int sr_no, QString destination_db_file, QSqlDatabase &dest_db);
    QString set_textedit_data(QString command, int textedit_col_pos, int sr_no, QString destination_db_file);
    QString set_stack_data(QString command, int textedit_col_pos, int sr_no, QString destination_db_file);

    void get_export_detail_html(QSqlDatabase dest_db, QString column, QString tablename);
    QString export_virtual_file(QString file_path);


    QStringList table_export_available_list_tablewidget;
    QStringList table_name_list_tablewidget;
    QStringList table_html_report_available_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QStringList table_name_list_textedit;
    QStringList html_report_available_list_textedit;
    QStringList tab_name_list_textedit;
    QStringList table_name_list_stack;
    QStringList html_report_available_list_stack;
    QStringList tab_name_list_stack;
    QStringList table_artifact_common_list_tablewidget;
    QStringList table_report_html_header_list_tablewidget;
    QStringList table_report_header_column_width_html_list_tablewidget;
    QStringList table_export_criteria_list_tablewidget;
    QStringList artifact_common_list_textedit;
    QStringList report_html_header_list_textedit;
    QStringList report_header_column_width_html_list_textedit;
    QStringList artifact_common_list_stack;
    QStringList report_html_header_list_stack;
    QStringList report_header_column_width_html_list_stack;



    QList<QStringList>report_html_col_name_list_textedit;
    QList<QStringList>report_html_datatype_list_textedit;
    QList<QStringList>report_html_col_name_list_stack;
    QList<QStringList>report_html_datatype_list_stack;
    QList<QStringList>tw_data_type_list_tablewidget;

    QList<QStringList>rt_html_d_name_list_tablewidget;
    QList<QStringList>rt_html_order_list_tablewidget;
    QList<QStringList>rt_html_data_type_list_tablewidget;
    QList<QStringList>rt_html_col_name_list_tablewidget;


    QStringList destination_db_file_list;
    QString  plugin_name_str;
    QString parent_module_name_str;
    QString reports_dir_path;
    QString destination_file_location;


    void fill_variable_from_narad_muni();
    void clear_fields();


    report_generic_function *report_generic_function_obj;

    QString get_plugin_name();
    QString get_virtual_preview_file(QString virtual_file_path);

    bool bool_cancel_report;
    recon_helper_standard *recon_helper_standard_obj;
    QString emlx_source_file;


    bool bool_export_file_for_pdf_report;

};

#endif //
