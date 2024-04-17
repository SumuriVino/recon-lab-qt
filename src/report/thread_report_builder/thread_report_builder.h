#ifndef THREAD_REPORT_BUILDER_REPORT_H
#define THREAD_REPORT_BUILDER_REPORT_H

#include <QObject>
#include <QThread>
#include <signal.h>

#include "header_identical_1.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_static_functions.h"

                                   
#include "progress_bar/progress_bar_fixed_in_statusbar.h"

#include "report/report_essentials/report_template_html_advance.h"
#include "header_global_variables_1.h"
#include "report/report_essentials/report_generic_function.h"
#include "report/html_standard_report_module.h"
#include "report/pdf_report_module.h"
#include "report/csv_report_module.h"
#include "report/xml_report_module.h"
#include "report/kml_report_module.h"
#include "report/html_advance_report_module.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "ui_display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"



class thread_report_builder : public QThread
{
    Q_OBJECT
public:
    thread_report_builder(QWidget *parent = 0);
    ~thread_report_builder();
    void run();

    QString pub_get_report_name();

    void pub_set_essentials(struct_global_global_report_info obj);
    QStringList pub_get_report_filepath_list();

    void pub_cancel_report();

    QString pub_get_report_password();
signals:


    void signal_create_pdf_report(QString html_pah, QString pdf_path);

private slots:

    void slot_pdf_report_finished(QString html_pah, bool m_srtatus);

private:

    QString m_report_type, m_report_scope;

    int m_report_maker;
    QStringList report_type_list;
    QString report_filepath;
    QString report_dir;
    QStringList report_path_list;

    void write_html_standard_report_header();
    void write_html_advance_report_header();


    narad_muni *narad_muni_for_report_obj;


    QString  report_name;


    QStringList selected_tags_list;

    QString recon_result_dir_path;

    void fill_variable_from_narad();
    ///----------Case Details
    QString app_version;
    QString case_no;
    QString case_name;
    QString examiner;
    QString examiner_phone;
    QString examiner_email;
    QString agency;
    QString agency_address;
    QString location;
    QString notes;
    QString logo_path;
    QString examiner_selected_timezone_string;



    report_template_html_advance *report_html_template_plugin_advance_obj;

    QStringList selected_plugins_info;
    void create_report_header();

    report_generic_function *report_generic_function_obj;



    recon_helper_standard *recon_helper_standard_obj;


    QStringList tab_name_list_textedit;
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



    QStringList table_export_available_list_tablewidget;
    QStringList table_name_list_tablewidget;
    QStringList table_html_report_available_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QStringList table_name_list_textedit;
    QStringList html_report_available_list_textedit;
    QStringList table_name_list_stack;
    QStringList html_report_available_list_stack;
    QStringList table_csv_report_available_list_tablewidget;

    QStringList table_xml_report_available_list_tablewidget;
    QStringList table_kml_report_available_list_tablewidget;

    QList<QStringList>export_path_column_list_tablewidget;

    QList<QStringList>tw_data_type_list_tablewidget;






    QList<QStringList>report_html_col_name_list_textedit;

    QList<QStringList>rt_html_data_type_list_tablewidget;

    QList<QStringList>rt_html_col_name_list_tablewidget;


    QList<QStringList>report_html_datatype_list_textedit;

    QList<QStringList>report_html_col_name_list_stack;

    QList<QStringList>report_html_datatype_list_stack;

    QList<QStringList>rt_html_d_name_list_tablewidget;
    QList<QStringList>rt_html_order_list_tablewidget;
    QList<QStringList>rt_csv_data_type_list;

    QList<QStringList>rt_csv_col_name_list;

    QList<QStringList>rt_csv_d_name_list;
    QList<QStringList>rt_csv_order_list;
    QList<QStringList>rt_xml_d_name_list;

    QList<QStringList>rt_xml_order_list;
    QList<QStringList>rt_xml_data_type_list;

    QList<QStringList>rt_xml_col_name_list;

    QList<QStringList>rt_kml_data_type_list;

    QList<QStringList>rt_kml_col_name_list;

    QList<QStringList>rt_kml_d_name_list;

    QList<QStringList>rt_kml_order_list;


    void update_narad_muni(QString plgn_name);
    void extract_csv_values(QString plgn_name);


    html_standard_report_module *html_standard_report_module_obj;
    pdf_report_module *pdf_report_module_obj;
    csv_report_module *csv_report_module_obj;
    xml_report_module *xml_report_module_obj;
    kml_report_module *kml_report_module_obj;
    html_advance_report_module *html_advance_report_module_obj;


    bool bool_cancel_report;
    bool bool_pdf_report_finished;

    QStringList get_plugins_info_list_after_trimming_tabs_having_no_data();
};


#endif //
