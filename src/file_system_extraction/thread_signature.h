#ifndef THREAD_SIGNATURE_H
#define THREAD_SIGNATURE_H

#include <QObject>
#include <QMutex>
                                   
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_standard.h"      


class thread_signature : public QObject
{
    Q_OBJECT
public:
    explicit thread_signature(QObject *parent = nullptr);
    ~thread_signature();


    void pub_set_bool_cancel_extraction(struct_global_task_type_and_title t1);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);


    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);
signals:
    void finished();
    void signal_add_all_signatures_to_case_tree(QStringList sign_db_path_list);

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString estimated_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

public slots:
    void slot_extract_signature();

private:

    struct struct_filesystem_record_and_filepath
    {
       QString filesystem_record;
       QString filepath;
    };
    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;

    struct struct_signature_data
    {
        QString filesystem_record;
        QString signature_value;
        QString signature_name;
    };

    struct signature_match_data
    {
        QString signature_value;
        QString signature_name;
        QString signature_offset;
    };
    QList <signature_match_data> signature_match_data_list;

    QStringList job_selected_source_count_name_list;

    bool bool_cancel_extraction;
    qint64 progress_value;
    QStringList all_signatures_list;

    QMimeDatabase obj;
    QSqlDatabase  destination_db;
    QSqlDatabase  case_configuration_db;
    QSqlDatabase  in_memory_signature_data_db;
    QMutex *mutex_for_fs_main_db;
    recon_helper_standard *recon_helper_standard_obj;

    bool open_fs_db(QString fs_db_path);
    void extract_signatures(QString fs_db_path, QString signature_db_path, struct_GLOBAL_witness_info_source target_struct_obj, QStringList sign_db_path_list);
    void open_config_db();
    void prepare_signature_structure_list();
    void intermediate_save_data_to_signature_db(QString signature_database_path);
    bool open_and_create_signature_db(QString signature_db_path);
    void check_file_signature(QString filepath, struct_signature_data &signature_data_obj);

    QString recon_case_runner_type;

    int bunch_count = 0;
    int fs_rec_db_updated_count = 0;
    QStringList record_num_list;
    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_signature_analysis_for_fs_run_module();
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

};

#endif //
