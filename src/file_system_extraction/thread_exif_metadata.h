#ifndef THREAD_EXIF_METADATA_H
#define THREAD_EXIF_METADATA_H

#include <QObject>
#include <QMutex>
                                   
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "exif_data/exif_data_interface.h"
#include "recon_generic_functions/recon_helper_standard.h"      

class thread_exif_metadata : public QObject
{
    Q_OBJECT
public:
    explicit thread_exif_metadata(QObject *parent = nullptr);
    ~thread_exif_metadata();

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);

signals:
    void finished();
    void signal_add_exif_to_case_tree(QStringList exif_db_path_list);

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString estimated_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);


public slots:
    void slot_extract_exif_data();

private:

    struct struct_filesystem_record_and_filepath
    {
       QString filesystem_record;
       QString filepath;
       QString display_file_path;
       QString mime_type;

    };


    QMutex *mutext_for_fs_main_db;

    QStringList job_selected_source_count_name_list;

    bool bool_cancel_extraction;
    exif_data_interface *exif_interface_obj;
    QSqlDatabase  destination_db;
    QSqlDatabase  in_memory_exif_data_db;
    qint64 progress_value;

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;

    bool open_fs_db(QString fs_db_path);
    bool open_and_create_exif_db(QString exif_db_path);
    void intermediate_save_data_to_exif_db(QString exif_database_path);
    QString recon_case_runner_type;

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_exif_metadata_for_fs_run_module();
    recon_helper_standard *recon_helper_standard_obj;

    void create_and_insert_processed_exif_types_list(QString exif_metadata_db_path);

};

#endif //
