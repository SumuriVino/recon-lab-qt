#ifndef RECON_HELPER_SINGULAR_H
#define RECON_HELPER_SINGULAR_H

#include <QtCore>
#include <QWidget>
#include <QObject>
#include <QFileIconProvider>

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "header_global_structures.h"
#include "header_identical_2.h"
#include "header_csv_template.h"

#include "header_global_variables_1.h"
#include "header_global_variables_2.h"



class recon_helper_singular : public QObject
{
    Q_OBJECT

public:
    recon_helper_singular(QWidget *parent = 0);
    ~recon_helper_singular();

    qint64 convert_nsdate_to_utc_numeric(NSDate *ns_date_field, NSObject *my_object, QString caller_func);

    QString convert_numeric_to_readable_in_selected_timezone(QString received_date_time, QString caller_func);
    QString convert_numeric_to_readable_in_utc_timezone(QString received_date_time, QString caller_func);
    QString convert_numeric_to_readable_export_time(QString received_date_time, QString caller_func);
    QString convert_numeric_to_readable_for_tablewidget(QString received_date_time, QString caller_func);

    qint64 convert_UTC_numeric_to_case_timezone_numeric(qint64);

    QString convert_safari_time_to_human_readable(QString received_time, QString caller_func);
    QString convert_webkit_chrome_time_to_human_readable(QString received_time, QString caller_func);

    QString convert_raw_data_to_ascii_convertor(QByteArray data_byte_array, QString caller_func);
    int hexToInt(char c);
    int hexToLetter(char c, char d);
    QString convert_raw_data_to_unicode_convertor(QByteArray data_byte_array, QString caller_func);

    QString get_file_system_initial_path_by_giving_source_count_name(QString source_count_name, QString caller_func);

    void fill_apple_metadata_from_api(QString filepath, QStringList keys_list, struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj, QString caller_func);
    QString get_plist_file_path_after_running_diskutill_info(QString identifier_name, QString result_dir_path, QString caller_func);
    QString get_plist_file_path_after_running_diskutil_apfs_list(QString parent_identifier, QString caller_func);

    QStringList get_mounted_media_list(QString caller_func);

    QStringList get_mounted_volumes_path_with_node_list(QString caller_func);
    QString get_disk_node_of_mounted_path(QString mounted_path, QString caller_func);

    QString get_file_system_of_any_node(QString dsk_node, QString caller_func);
    QString get_file_system_of_mounted_path(QString mounted_path, QString caller_func);
    QString get_file_system_of_filepath(QString source_path, QString caller_func);

    bool is_it_mounted_path_by_exact_match(QString mounted_path, QString caller_func);
    bool is_it_mounted_path_by_contains_match(QString mounted_path, QString caller_func);

    QStringList get_softlink_path(QString dir_path, QString caller_func);

    QString convert_user_formatted_date_to_numeric(QString timestamp_str, QString format_str, QString caller_fun);
    QString convert_user_formatted_datetime_to_numeric(QString timestamp_str, QString format_str, QString caller_fun);
    QString extract_date_from_datetime(qint64 m_epch);

    QString get_adhoc_path_for_database(QString filepath);

    QStringList get_apple_metadata_from_api(QString m_file_path);

    void write_xml_report_case_info_and_source_info(QXmlStreamWriter* stream, QString report_scope , narad_muni *narad_muni_for_reports);
    QString write_html_case_info_and_source_info(QString report_scope , narad_muni *narad_muni_for_reports);
    QString write_html_only_case_info(QString report_scope, narad_muni *narad_muni_for_reports);

    void write_csv_report_case_details_and_source_info(QTextStream &outputfile_csv, QString report_scope , narad_muni *narad_muni_for_reports);

    QString write_advanced_html_report_case_details_and_source_info(QString report_scope, narad_muni *narad_muni_for_reports);

    QString get_complete_file_path(QString target_path, QString filepath, QString caller_func);

    QIcon get_qicon_of_source(struct_GLOBAL_witness_info_source struct_source_info);
    QIcon get_qicon_of_root(struct_GLOBAL_witness_info_root struct_root_info);

    bool is_it_recon_custom_picture_plugin(QString plugin_name);

    void pub_set_added_timezone_details(QList <struct_global_added_timezone_details> added_list_obj);
    void pub_set_default_timezone(struct_global_default_timezone_details default_obj);

    QString pub_get_value_from_source_textinfo_file_only(QString file_path, QString keyword);

    void pub_remove_info_from_snapshots_plist_for_source(QString source_count_name);

    bool is_system_pwd_accurate(QString m_pwd_orgl, QString caller_func);


    void pub_fill_destination_drive_info(QString caller_fun);
    struct_global_destination_drive_info get_saved_destination_drive_info(QString caller_func);

    struct_global_default_timezone_details struct_default_timezone_details_obj;

    qint64 convert_timestamp_readable_to_numeric_1(QString m_datetime);
    qint64 convert_timestamp_readable_to_numeric_2(QString timestamp_str, QString caller_func);
    qint64 convert_timestamp_readable_to_numeric_3(QString timestamp_str, QString caller_func);
    qint64 convert_timestamp_readable_to_numeric_4(QString timestamp_str, QString caller_func);
    qint64 convert_timestamp_readable_to_numeric_5(QString timestamp_str, QString caller_func);
    qint64 convert_timestamp_readable_to_numeric_6(QString timestamp_str, QString caller_func);

    QString convert_timestamp_readable_to_numeric_7(QString date_time);
    int convert_timezone_to_seconds(QString tm_zone);



private slots:
    void slot_generic_process_finished(int);

    void slot_export_process_finished(int);

private:


    struct_global_destination_drive_info st_destination_drive_info;


    QProcess *process_generic;
    bool bool_process_generic;

    void set_blank_apple_metadata(QStringList keys_list, struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj);

    QStringList get_mounted_media_list_from_volume_directory(QString caller_func);
    QStringList get_mounted_media_from_mount_command(QString caller_func);

    QString run_command_generic(QString command, QStringList arguments, QString caller_func);
    QString run_command_without_argument(QString command, QString caller_func);


    struct struct_case_details_values
    {
        QString result_created_app_version;
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
        QString user_timezone_string;
        QString report_generated_date_time;
        QString machine_timezone_string;

    };

    struct struct_case_details_header_lables
    {

        QString RECON_Report_header_lable;
        QString Case_Info_header_lable;
        QString Case_Details_header_lable;
        QString Examiner_Details_header_lable;
        QString Sources_Details_header_lable;

        QString app_version_lable;
        QString case_no_lable;
        QString report_scope_lable;
        QString case_name_lable;
        QString examiner_lable;
        QString examiner_phone_lable;
        QString examiner_email_lable;
        QString agency_lable;
        QString agency_address_lable;
        QString location_lable;
        QString notes_lable;
        QString user_timezone_string_lable;
        QString report_generated_date_time_lable;
        QString machine_timezone_lable;

    };

    struct struct_case_details_values fill_case_detials_values_structure(narad_muni *narad_muni_for_reports);

    struct struct_case_details_header_lables fill_case_detials_lables_structure();


    QList <struct_global_added_timezone_details> added_timezone_details_qlist;


    bool bool_export_process;

};

#endif
