#ifndef THREAD_SKIN_TONE_DETECTION_H
#define THREAD_SKIN_TONE_DETECTION_H

#include <QObject>
#include <QtCore>
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_structures.h"
#include <QProcess>
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include <QMessageBox>
#include "witness_info/witness_info_manager/witness_info_manager.h"
#include <QTimer>

class thread_skin_tone_detection : public QObject
{
    Q_OBJECT
public:
    explicit thread_skin_tone_detection(QObject *parent = nullptr);

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_job_selected_source_count_name_list(QStringList m_list);

    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);

signals:
    void finished();
    void signal_add_skin_tone_detection_to_case_tree(QStringList);
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

    void signal_extract_mime_from_skin_tone_detection_yes_clicked();
    void signal_set_bool_thread_skin_tone_detection_running_status();

public slots:
    void slot_extract_skin_tone_detection();

private slots:
    void slot_process_skin_tone_detection_finished(int status);

    void slot_process_skin_tone_detection_readyread();
    void slot_timeout_to_update_estimated_time_for_fs_modules();

private:

    QStringList job_selected_source_count_name_list;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    bool bool_cancel_extraction = false;
    bool load_case = false;

    QMutex *mutext_for_fs_main_db;

    QSqlDatabase  destination_db;

    bool open_fs_db(QString fs_db_path);

    QString recon_case_runner_type;

    bool bool_skin_tone_extraction_process_finished = false;

    QProcess *process_skin_tone_detection;

    int rec_count = 0;

    qint64  total_records_num = 0;

    QString source_name;

    QString filesystem_db_path;

    recon_helper_standard *recon_helper_standard_obj;

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_skin_tone_detection_for_fs_run_module();
    QString cmd_update_fs_run_status;

    qint64 processed_file_count_qint64 = 0;
    qint64 remaining_file_count_for_req_time_qint64 = 0;
    QString estimated_time;

};

#endif // THREAD_SKIN_TONE_DETECTION_H
