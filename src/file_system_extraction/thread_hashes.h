#ifndef THREAD_HASH_H
#define THREAD_HASH_H

#include <QObject>
                                   
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_standard.h"      

class thread_hashes : public QObject
{
    Q_OBJECT
public:
    explicit thread_hashes(QObject *parent = nullptr);
    ~thread_hashes();

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);
signals:
    void finished();
    void signal_add_matched_hashes_to_case_tree(QString hashes_db_path, QString source_count_name);
    void signal_add_matched_hashes_for_fs_module_to_case_tree(QStringList hashes_db_path_list);
    void signal_add_all_available_hashes_to_case_tree();
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

public slots:
    void slot_hashset_extraction();

private:

    struct struct_filesystem_record_and_filepath
    {
        QString filesystem_record;
        QString filepath;
    };

    struct hashset_db_data_struct
    {
        QString db_path;
        QString md5_hash_str;
    };

    struct struct_hashes
    {
        QString md5_hash;
        QString sha1_hash;

    };

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    QStringList job_selected_source_count_name_list;

    QList<hashset_db_data_struct> list_hashset_db_data;

    bool bool_hash_matched;

    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;

    qint64 progress_value;

    bool bool_cancel_extraction;

    QSqlDatabase  destination_db;
    QSqlDatabase  in_memory_hashes_db;
    QMutex *mutext_for_fs_main_db;

    bool open_fs_db(QString fs_db_path);
    bool open_and_create_hashes_db(QString hashes_db_path);
    void intermediate_save_data_to_hashes_db(QString signature_database_path);

    QString recon_case_runner_type;

    void copy_data_from_hashes_db_to_fs_db(QString source_count_name);

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_hashset_for_fs_run_module();
    struct_hashes create_md5_and_sha1_hashes(QString file_path);
    recon_helper_standard *recon_helper_standard_obj;

    QList<struct_global_available_hashset_data> available_hashset_data_list;
    QSqlDatabase db_hashset_indexes;
};

#endif //
