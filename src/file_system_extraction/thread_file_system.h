#ifndef THREAD_FILE_SYSTEM_H
#define THREAD_FILE_SYSTEM_H

#include <QObject>
#include <QtCore>
#include <QWidget>
#include <QThread>
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

#include "recon_generic_functions/recon_static_functions.h"

#include "header_global_structures.h"
#include "header_global_variables_1.h"
#include "ios_backup/uid_backup/uid_backup_parser/ios_uid_backup_parser.h"
#include "exif_data/exif_data_interface.h"
#include "ui_custom_classes/r_treewidget.h"
#include "dt_search/dt_search_interface.h"
#include "thread_recon_lab_initial_settings.h"
#include "rcbuild/build_items_common_for_recon_and_fsreader.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include "ios_backup/xml_backup/xml_backup_parser/ios_xml_backup_parser.h"
#include "header_global_variables_4.h"

class thread_file_system : public QThread
{
    Q_OBJECT

public:
    thread_file_system(QWidget *parent = 0);
    ~thread_file_system();

    void run();

    void set_bool_cancel_extraction();
    void pub_set_FS_source_selected_source_count_name_list(QStringList m_list);
    void set_case_treewidget_obj(R_Treewidget *obj);

    void pub_set_map_ios_uid_backup_parser_obj(QMap<QString, ios_uid_backup_parser *> *obj);

    void set_recon_case_runner_type(QString type);
    void pub_set_essential();

    void pub_set_tsk_fs_process(QProcess *mobj);
    void pub_set_general_process(QProcess *mobj);
signals:
    void signal_remove_file_type_based_entry_from_main_stackwidget(QString parent_name, QString child_name);

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_thread_file_system_finished();
    void signal_add_file_basic_to_case(QString fs_database_path);
    void signal_add_internal_sources_to_case(QString source_cnt_name);
    void signal_add_registry_files(QStringList file_path_list,QString source_count_name);
    void signal_all_ios_sources_extraction_finished();
    void signal_source_extraction_completed_only_for_basic_file_system(QString source_cnt_name);

private slots:
    void slot_registry_files_extraction_work_finished();
    void slot_process_tsk_fs_custom_status(QString custom_msg);
    void slot_add_file_basic_to_case_finished();
    void slot_add_internal_sources_to_case_finished();


private:

    enum enum_ENDIANS {
        enum_little_endian   = 1,
        enum_big_endian      = 2
    };

    struct struct_file_signature_data
    {
        QString signature_name;
        QString signature;
    };

    struct struct_recon_logical_image_posix_timestamp
    {
        qint64 access_timestamp_qint64;
        qint64 change_timestamp_qint64;
        qint64 modify_timestamp_qint64;
        qint64 create_timestamp_qint64;
    };


    QString adhoc_path_doc_revision;
    QString source_path;
    QString fs_database_path;
    QString mime_database_path;
    QString signature_database_path;
    QString hashes_database_path;
    QString griffeye_data_database_path;
    QString exif_database_path;
    QString plugin_status_database_path;
    QString apple_metadata_database_path;
    QString apple_timestamps_database_path;
    QString ios_uid_backup_path_str;
    QString case_db_path;

    QElapsedTimer         *timer_records_fs ;
    ios_uid_backup_parser     *ios_uid_backup_parser_obj;
    R_Treewidget          *case_treewidget_obj;

    struct_GLOBAL_witness_info_source struct_target_info_ptr;
    QList<struct_global_fs_extracted_dir_info> list_struct_fs_dir_info;


    qint64 total_records_fs;

    QList<struct_GLOBAL_witness_info_source> list_target_general_source_info;

    QMap<QString, ios_uid_backup_parser*> *map_ios_uid_backup_parser_obj;

    int iterate_directory_recursively_and_make_dir_info_structure(QString source_path);

    void extract_fs_info_and_fill_db_for_regular_source(QSqlDatabase destination_db);

    void extract_fs_info_and_fill_db_for_ios_uid_10_11(QSqlDatabase destination_db);
    void extract_fs_info_and_fill_db_for_ios_uid_7_8_9(QSqlDatabase destination_db);
    void initiate_dir_insertion_for_fs_in_case_tree(QTreeWidgetItem *item, QString dir_path, QSqlDatabase &filesystem_db);
    void extract_fs_info_and_fill_db_for_ios_uid(QSqlDatabase destination_db);
    void fill_struct_dir_list_for_ios_uid_backup();

    void add_fs_dir_in_case_tree();

    QString get_file_size_records_command(QString column_name, QString file_size_category);
    QList<struct_global_fs_extracted_dir_info> get_list_extracted_struct_fs_dir_info_from_db();
    struct_global_fs_extracted_dir_info get_struct_dir_info_from_list(QString dir_path, QList<struct_global_fs_extracted_dir_info> list_struct_file_system_dir_info);
    void retrieve_paths();

    void create_mime_signature_exif_apple_metadata_and_necessary_path();

    QString create_apple_metadata_db_table_command();

    void set_current_source_ios_uid_backup_parser_obj(ios_uid_backup_parser *obj);
    struct stat fileStat;

    QStringList source_selected_source_count_name_list;

    QString source_count_name;
    QString recon_case_runner_type;

    void insert_regular_source_details_in_filesystem_db(QString file_path, QSqlQuery *query_insert);
    void insert_ios_uid_details_in_filesystem_db(QSqlDatabase destination_db, QString id_path, QString domain_path, QString file_id, QString m_flag);



    void parse_generations_table_data_document_revision();

    void extract_file_system_regular();
    void extract_file_system_tsk();
    void extract_window_registry_files();
    void extract_window_ram_images();
    void update_fs_db_for_path_startfrom();


    thread_recon_lab_initial_settings *thread_recon_lab_initial_settings_obj;

    void generate_filesystem_timeline(QString source_count_name);
    void check_for_other_sources(QString display_path, QString info_path);

    bool bool_registry_files_extraction_finished = false;
    bool bool_process_tsk_fs_finish_with_desired_output = false;
    bool bool_process_tsk_fs_finished = false;
    QProcess *process_tsk_fs;
    QProcess *process_general;
    recon_helper_standard *recon_helper_standard_obj;

    void extract_posix_timestamps_for_recon_logical_images();
    QMap <QString,struct_recon_logical_image_posix_timestamp> map_key_filepath_value_recon_logical_image_timestamp;

    void clear_variables_recon_logical_image_posix_timestamps(thread_file_system::struct_recon_logical_image_posix_timestamp &st_tmestamps);
    void run_python_script_for_apple_metadata();

    bool bool_add_file_basic_to_case = false;
    bool bool_add_internal_sources_to_case = false;

    void store_python_script_outputin_result_dir();

    QString ios_xml_backup_path_str;
    QList<struct_ios_xml_files_info> struct_ios_xml_files_info_qlist;
    void fill_struct_dir_list_for_ios_xml_backup();
    void extract_fs_info_and_fill_db_for_ios_xml(QSqlDatabase destination_db);

    void insert_ios_xml_details_in_filesystem_db(QSqlDatabase destination_db, QString id_path, QString file_path, QString file_size, QString file_id, QString m_flag , struct_ios_xml_tagged_files_timestamp_info obj);

    QString face_analysis_database_path;
    QString optical_character_recognition_database_path;
    QString skin_tone_detection_database_path;

    QString weapons_database_path;
    QString fire_analysis_database_path;

    void extract_fs_info_and_fill_db_for_zip_source(QSqlDatabase destination_db);
    void insert_zip_source_details_in_filesystem_db(QString file_path, QSqlQuery *query_insert);
    QString video_thumbnails_database_path;
};

#endif //
