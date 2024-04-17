#ifndef REPORT_GENERIC_FUNCTION_H
#define REPORT_GENERIC_FUNCTION_H

#include <QObject>
#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_report.h"
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


class report_generic_function : public QObject
{
    Q_OBJECT
public:
    explicit report_generic_function(QObject *parent = nullptr);


    void pub_set_narad_muni_for_report(narad_muni *mobj);
    void pub_set_essentials(void);
    void pub_get_only_tags_record_of_table_including_multiple(QString cmd, QStringList &record_no_list, QString destination_db_file);

    bool pub_is_data_available_in_table(QString table_name, QString report_scope, QStringList selected_tags_list);
    bool pub_is_data_available_in_tab(QStringList tab_name, QString report_scope, QStringList selected_tags_list);
    bool pub_is_data_available_in_examiner_notes();
    bool pub_check_datatype_exist_in_datatypelist(const char *searched_data, QStringList data_type_list);
    bool pub_is_column_of_apple_attribute(QString column_name);
    bool pub_is_tagtype_other_than_screenshot_or_saved_graph(QString plugin_name);

    qint64 pub_get_total_record_of_table(QString table_name, QString report_scope, QStringList tags_list);

    QString pub_get_fs_command(QString fs_record_no);
    QString pub_get_hex_viewer_format_for_html_report(QString hex_bytes);
    QString pub_export_virtual_file(QString file_path, QString plugin_name, QString destination_file_location, QString reports_dir_path);

    QStringList pub_get_artifacts_source_list(QString table, QString report_scope, QStringList selected_tags_list_for_report);

    QStringList html_report_file_system_signauture_key_list;
    QStringList html_report_file_system_Mime_key_list;

signals:

public slots:

private:

    narad_muni *narad_muni_for_report_obj;

    QString plugin_name;
    QStringList destination_db_file_list;

    void fill_variable_from_narad_muni();
    recon_helper_standard *recon_helper_standard_obj;


};

#endif
