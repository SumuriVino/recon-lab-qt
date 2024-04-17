#ifndef THREAD_APPLE_METADATA_H
#define THREAD_APPLE_METADATA_H

#include <QObject>

#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "rcbuild/build_items_common_for_recon_and_apple_metadata_reader.h"
#include "header_global_variables_4.h"

class thread_apple_metadata : public QObject
{
    Q_OBJECT
public:
    explicit thread_apple_metadata(QObject *parent = nullptr);
    ~thread_apple_metadata();

    void pub_set_essentials();
    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_job_selected_source_count_name_list(QStringList m_list);

    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);
    void pub_set_apple_metadata_all_attributes_name_list(QStringList attributes_name_list);
    void pub_extract_apple_metadata_from_apple_metadata_reader_on_file_click(QString file_paths_str, QString source_cnt_name, QString virtual_source_path, QStringList apple_metadata_att_list, QStringList column_name_list);
    void pub_set_apple_metadata_reader_process_for_full_source(QProcess *mobj);
    void pub_set_apple_metadata_reader_process_for_right_click(QProcess *mobj);
    void pub_set_apple_metadata_reader_process_for_left_click(QProcess *mobj);
signals:
    void finished();
    void signal_add_apple_metadata_to_case(QStringList apple_db_path_list);

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

public slots:
    void slot_extract_apple_metadata();


    void slot_prc_apple_metadata_reader_for_right_click_readyread();
private slots:
    void slot_prc_apple_metadata_reader_for_full_source_readyread();
    void slot_timeout_to_update_estimated_time_for_fs_modules();

private:

    struct struct_filesystem_record_and_filepath
    {
        QString filesystem_record;
        QString filepath;
    };

    enum apple_metadata_runner_status
    {
        enum_apple_metadata_runner_status_full_source = 0,
        enum_apple_metadata_runner_status_right_click,
        enum_apple_metadata_runner_status_left_click
    };

    QStringList job_selected_source_count_name_list;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    QStringList attribute_column_name_for_logical_image;

    QStringList attributes_name_list_final , attribute_column_list_final;
    QStringList attributes_name_list_examiner_selected,column_name_list_examiner_selected;
    QStringList attributes_name_list_all,column_name_list_all;

    bool bool_cancel_extraction;
    qint64 progress_value;
    QMutex *mutext_for_fs_main_db;

    QSqlDatabase  destination_db;
    QSqlDatabase  in_memory_apple_metadata_db;

    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;

    QString create_in_memory_db_table_command();
    void set_all_attributes_list();
    bool open_fs_db(QString fs_db_path);
    bool open_and_create_apple_metadata_db(QString apple_metadata_db_path);
    void intermediate_save_data_to_apple_metadata_db(QString apple_metadata_database_path);

    QString recon_case_runner_type;

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_apple_metadata_for_fs_run_module();
    recon_helper_standard *recon_helper_standard_obj;

    void create_and_insert_processed_attributes_list(QString apple_metadata_db_path);

    QMap <QString ,QMap <QString ,QString> > map_apple_metadata_for_recon_logical_image;
    bool fill_apple_metadata_from_script(QString file_path, QStringList keys_list, struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj);
    QStringList apple_metadata_attributes_name_list;

    void extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name, QString virtual_source_path, QString m_flag, int runner_type);

    QProcess *process_apple_metadata_reader_for_full_source;
    QProcess *process_apple_metadata_reader_for_left_click;
    QProcess *process_apple_metadata_reader_for_right_click;

    void copy_tmp_apple_metadata_to_actual_apple_timestamps_db(QString tmp_fs_db_path, QString actual_db_path);
    void update_filesystem_db_for_fs_run_module_status(QString tmp_fs_db_path, QString actual_fs_db_path);

    void insert_source_info_in_apple_metadata_essential_db_for_right_click_extraction(QString fs_record_no, QString display_file_path, QString source_count_name);
    QString create_tmp_apple_metdata_essential_db_for_right_click();

    QString estimated_time_for_proc;
    QString estimated_time_QString;
    qint64 processed_file_count_for_proc_qint64 = 0;
    qint64 remain_file_count_for_req_time_for_proc_qint64 = 0;

};

#endif //
