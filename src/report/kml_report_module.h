#ifndef KML_REPORT_MODULE_H
#define KML_REPORT_MODULE_H

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


class kml_report_module : public QObject
{
    Q_OBJECT
public:
    explicit kml_report_module(QObject *parent = 0);

    void create_full_report_kml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_screen_report_kml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list);
    void create_tag_report_kml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void set_essentials();
    void set_narad_muni_for_report(narad_muni *mobj);
    void clear_exportfilelist_table();

signals:

public slots:

private slots:


private:

    QList<m_tablewidget *> list_m_tablewidgets;


    narad_muni *narad_muni_for_report_obj;

    QString module_name_str;
    QStringList destination_db_file_list;


    QList<QStringList>tw_data_type_list_tablewidget;


    QList<QStringList>rt_kml_d_name_list;
    QList<QStringList>rt_kml_col_name_list;
    QList<QStringList>rt_kml_order_list;
    QList<QStringList>rt_kml_data_type_list;



    QStringList table_export_available_list_tablewidget;
    QStringList table_name_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QStringList table_kml_report_available_list_tablewidget;
    QStringList table_export_criteria_list_tablewidget;


    void fill_variable_from_narad_muni();
    void set_table_data_kml(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, QXmlStreamWriter *stream_kml, QString destination_db_file);
    void get_export_detail_kml(QSqlDatabase dest_db, QString column, QString tablename);


    export_file_module *export_file_obj;

    report_generic_function *report_generic_function_obj;

    void create_tablewidget_kml_report(QXmlStreamWriter *stream_kml, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked);
    recon_helper_standard *recon_helper_standard_obj;

    QString get_plugin_name();
    void clear_fields();
};

#endif // KML_REPORT_MODULE_H
