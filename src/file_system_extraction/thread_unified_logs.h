#ifndef THREAD_UNIFIED_LOGS_H
#define THREAD_UNIFIED_LOGS_H

#include <QObject>
#include <QtCore>
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_1.h"
#include "header_identical_2.h"
#include "header_global_variables_2.h"
#include "recon_generic_functions/recon_helper_process.h"
#include "header_global_variables_4.h"

class thread_unified_logs : public QObject
{
    Q_OBJECT
public:
    explicit thread_unified_logs(QObject *parent = nullptr);

    //===
    //    ~thread_unified_logs();
    //====

    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void pub_set_recon_case_runner_type(QString type);

private:
    QStringList job_selected_source_count_name_list;
    bool bool_cancel_extraction = false;
    QMutex *mutex_for_fs_main_db;
    QString recon_case_runner_type;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    void extract_log_macos_log_show();
    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;
    QString log_macos_log_json_file_path;
    QFile log_macos_log_show_qfile;
    QProcess *prc_log_macos_log_show;
    bool bool_log_macos_log_show_error_status;
    bool bool_log_macos_process_finished;
    QTimer *m_timer_log_macos_log_show;
    struct_GLOBAL_witness_info_source target_struct_obj;
    void write_apple_macOS_log_archive_data_into_json_file(QString logarchive_file_path, QString file_path);
    void create_and_insertion_in_logs_db(QString logs_db_path, QString db_name1,QString index_unified_logs_db);
signals:
    void finished();
    void signal_add_unified_logs_to_case_tree(QString);
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_completion_status(QString job_type, QString source_count_name, QString status);

private slots:
    void slot_extract_unified_logs();

    void slot_apple_macOS_log_show_proc_readyread();
    void slot_apple_macOS_log_show_proc_finished();
    void on_slot_timeout_apple_macOS_log_collection_timer();
};

#endif // THREAD_UNIFIED_LOGS_H
