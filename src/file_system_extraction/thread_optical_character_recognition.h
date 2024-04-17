
#ifndef THREAD_OPTICAL_CHARACTER_RECOGNITION_H
#define THREAD_OPTICAL_CHARACTER_RECOGNITION_H

#include <QObject>

#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_standard.h"

class thread_optical_character_recognition : public QObject
{
    Q_OBJECT
public:
    explicit thread_optical_character_recognition(QObject *parent = nullptr);
    ~thread_optical_character_recognition();

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_job_selected_source_count_name_list(QStringList m_list);

    void set_recon_case_runner_type(QString type);

    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);

signals:
    void finished();
    void signal_add_optical_character_recognition_to_case_tree(QStringList);

    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

public slots:
    void slot_extract_optical_character_recognition();

private slots:
    void slot_process_optical_character_recognition_finished(int status);

    void slot_process_optical_character_recognition_readyread();
    void slot_timeout_to_update_estimated_time_for_fs_modules();

private:

    QStringList job_selected_source_count_name_list;

    bool bool_cancel_extraction;
    QMutex *mutext_for_fs_main_db;

    QSqlDatabase  destination_db;

    bool open_fs_db(QString fs_db_path);

    QString recon_case_runner_type;

    bool bool_optical_character_recognition_process_finished = false;

    QList<struct_GLOBAL_witness_info_source> list_target_witness_info;

    QProcess *process_run_optical_character_recognition;

    qint64  rec_count = 0;
    qint64  total_records_num = 0;

    QString source_name;

    QString file_system_db_path;

    recon_helper_standard *recon_helper_standard_obj;
    void copy_data_from_ocr_to_fs_db(QString source_count_name);

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_optical_character_recognition_for_fs_run_module();
    QString cmd_update_fs_run_status;

    qint64 processed_file_count_qint64 = 0;
    qint64 remaining_file_count_for_req_time_qint64 = 0;
    QString estimated_time;
};

#endif // THREAD_optical_character_recognition_EXTRACTION_H
