#ifndef XML_REPORT_MODULE_H
#define XML_REPORT_MODULE_H

#include <QObject>
#include <QWidget>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "header_plugin_extraction_interface.h"
#include "header_csv_template.h"
#include "header_report.h"
//#include "export/thread_file_copier_module.h"
                                   
#include "ui_custom_classes/m_tablewidget.h"
#include "header_global_variables_3.h"
#include "export/export_for_reports/export_file.h"
#include "header_global_variables_1.h"
#include "report/report_essentials/report_generic_function.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include <QTextDocument>



class xml_report_module : public QObject
{
    Q_OBJECT
public:
    explicit xml_report_module(QObject *parent = 0);

    void create_full_report_xml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_tag_report_xml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_screen_report_xml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list);
    void set_essentials();
    void set_narad_muni_for_report(narad_muni *mobj);


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
    QStringList table_xml_report_available_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QStringList table_artifact_common_list_tablewidget;
    QStringList table_export_criteria_list_tablewidget;

    QList<QStringList>rt_xml_d_name_list;
    QList<QStringList>rt_xml_col_name_list;
    QList<QStringList>rt_xml_order_list;
    QList<QStringList>rt_xml_data_type_list;

    QString  module_name_str;
    QStringList destination_db_file_list;


    QList<QStringList>tw_data_type_list_tablewidget;


    void fill_variable_from_narad_muni();


    export_file_module *export_file_obj;

    void set_data_xml(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int count, QXmlStreamWriter *stream_xml, QString destination_db_file , QSqlDatabase &dest_db);

    void create_tablewidget_xml_report(QXmlStreamWriter *stream_xml, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked);

    void write_users_apple_metadata_in_xml_file(QString extended_attr_str, QXmlStreamWriter *stream_xml);

    QString get_apple_metadata_key_value_for_xml_report(QString record_no, QString table_name, QString db_path);
    report_generic_function *report_generic_function_obj;

    void set_export_detail_xml(QSqlDatabase dest_db, QXmlStreamWriter *stream_xml, QString column, QString table_name);
    QString get_plugin_name();

    bool bool_cancel_report;
    recon_helper_standard *recon_helper_standard_obj;
    QString emlx_source_file;
    QString emlx_source_file_value;

};

#endif // XML_REPORT_MODULE_H
