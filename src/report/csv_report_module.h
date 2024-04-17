#ifndef CSV_REPORT_MODULE_H
#define CSV_REPORT_MODULE_H

#include <QObject>
#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "header_plugin_extraction_interface.h"
#include "header_csv_template.h"
//#include "export/thread_file_copier_module.h"
                                   
#include "ui_custom_classes/m_tablewidget.h"
#include "export/export_for_reports/export_file.h"
#include "header_global_variables_3.h"
#include "header_global_variables_1.h"
#include "header_report.h"
#include "report/report_essentials/report_generic_function.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


class csv_report_module : public QObject
{
    Q_OBJECT
public:
    explicit csv_report_module(QObject *parent = 0);

    void create_full_report_csv(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_tag_report_csv(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_screen_report_csv(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list);
    void set_narad_muni_for_report(narad_muni *mobj);

    void set_essentials();
    void clear_exportfilelist_table();

    void pub_cancel_report();

signals:

public slots:

private slots:
private:


    QList<m_tablewidget *> list_m_tablewidgets;

    narad_muni *narad_muni_for_report_obj;


    QStringList table_export_available_list_tablewidget;
    QStringList table_name_list_tablewidget;
    QStringList table_csv_report_available_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QStringList table_export_criteria_list_tablewidget;


    QList<QStringList>rt_csv_d_name_list;
    QList<QStringList>rt_csv_col_name_list;
    QList<QStringList>rt_csv_order_list;
    QList<QStringList>rt_csv_data_type_list;



    QStringList table_artifact_common_list_tablewidget;



    QString  plugin_name_str;
    QString parent_module_name_str;
    QStringList destination_db_file_list;


    QList<QStringList>tw_data_type_list_tablewidget;



    QStringList get_export_detail_csv_export(QSqlDatabase dest_db, QString column, QString tablename);
    QString get_export_detail_csv_multiple_export(QSqlDatabase dest_db, QString column, QString tablename);

    void fill_variable_from_narad_muni();


    export_file_module *export_file_obj;


    void set_table_data(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int count, QTextStream &stream_csv, QString destination_db_file, QSqlDatabase &dest_db);


    void create_table_widget_csv_report(QTextStream &stream_csv, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked);

    QString write_users_apple_metadata_in_csv_file(QString extended_attr_str);

    QString get_apple_metadata_key_value_for_csv_report(QString record_no, QString table_name, QString db_path);

    report_generic_function *report_generic_function_obj;

    QString get_plugin_name();
    QStringList csv_custom_col_report_list;
    bool bool_cancel_report;
    bool bool_os_scheme_ios_found;

    recon_helper_standard *recon_helper_standard_obj;

    QString emlx_source_file;
    QString emlx_source_file_value_str;

};

#endif // CSV_REPORT_MODULE_H
