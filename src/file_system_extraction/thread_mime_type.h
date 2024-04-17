#ifndef THREAD_MIME_TYPE_H
#define THREAD_MIME_TYPE_H

#include <QObject>
                                   
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_standard.h"      
//#include "magic.h"

class thread_mime_type : public QObject
{
    Q_OBJECT
public:
    explicit thread_mime_type(QObject *parent = nullptr);
    ~thread_mime_type();

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_job_selected_source_count_name_list(QStringList m_list);

    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);
signals:
    void finished();
    void signal_add_mime_types_to_case_tree(QStringList mime_db_path_list);

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);

    //    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);
    void signal_mime_type_extracted_source(QString src_cnt_name);

public slots:
    void slot_extract_mime_type();

private:

    struct struct_filesystem_record_and_filepath
    {
       QString filesystem_record;
       QString filepath;
    };


    QStringList job_selected_source_count_name_list;
    QStringList attributes_list_all , attribute_column_list;

    bool bool_cancel_extraction;
    qint64 progress_value;
    QMutex *mutext_for_fs_main_db;

    QSqlDatabase  destination_db;
    QSqlDatabase  in_memory_mime_type_db;
    recon_helper_standard *recon_helper_standard_obj;

    QList<struct_global_apple_metadata_parsed_data> extended_attr_data_list;
    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    bool open_fs_db(QString fs_db_path);
    bool open_and_create_mime_type_db(QString mime_db_path);
    void intermediate_save_data_to_mime_db(QString mime_database_path);

    QString recon_case_runner_type;

    void copy_data_from_mime_to_fs_db(QString source_count_name);

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_mime_type_for_fs_run_module();

};

#endif //
