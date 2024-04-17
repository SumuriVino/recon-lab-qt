#ifndef THREAD_INDEX_FILES_H
#define THREAD_INDEX_FILES_H

#include <QObject>
#include <QMutex>
                                   
#include "header_global_variables_3.h"                         
#include "header_global_structures.h"
#include "dt_search/dt_search_interface.h"
#include "recon_generic_functions/recon_helper_standard.h"      

extern qint64 global_dt_indexing_processed_files_count;

class thread_index_files : public QObject
{
    Q_OBJECT
public:
    explicit thread_index_files(QObject *parent = nullptr);

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_dt_serach_interface_obj(dt_search_interface *dt_search_obj);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_create_dt_search_index_for_every_source(QString source_count_name);
    bool pub_dt_update_index(QString filepath_str);
    void pub_dt_search_index(QString keyword_str);

    void set_recon_case_runner_type(QString type);
    void pub_set_is_running_from_toolbar(bool status);
signals:
    void finished();

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

public slots:
    void slot_extract_dt_search_data();

private slots:
    void slot_on_timer_tick();
private:

    struct struct_filesystem_record_and_filepath
    {
       QString filesystem_record;
       QString filepath;
    };

    bool bool_is_running_from_toolbar;

    bool bool_cancel_extraction;
    dt_search_interface *dt_search_interface_obj;
    QMutex *mutext_for_fs_main_db;
    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;
    QSqlDatabase  destination_db;
    QSqlDatabase  in_memory_index_files_db;

    QStringList whitelist_extensions_list;
    QStringList blacklist_extensions_list;
    qint64 max_filesize_to_index;

    bool open_fs_db(QString fs_db_path);

    void create_dt_search_index_for_every_source(QString source_count_name);
    void index_file_content_using_dt_search(QString file_path);
    void open_and_create_index_files_db(QString index_files_db_path);
    void intermediate_save_data_to_index_files_db(QString index_files_db_path);
    void prepare_whitelist_and_blacklist();

    QString recon_case_runner_type;

    void index_files_from_toolbar_selected_files();
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    QTimer *timer;
    QString current_label_in_task_status;
    qint64 current_source_total_records;

    bool bool_whitelist_check_status;
    bool bool_blacklist_check_status;

    recon_helper_standard *recon_helper_standard_obj;

    void index_files_for_carved_files();
};

#endif //
