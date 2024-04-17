#ifndef THREAD_Fire_Analysis_H
#define THREAD_Fire_Analysis_H

#include <QObject>
#include <QtCore>
#include  <QProcess>

#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_structures.h"

class thread_fire_analysis : public QObject
{
    Q_OBJECT

public:
    explicit thread_fire_analysis(QObject *parent = nullptr);
    ~thread_fire_analysis();

    void pub_set_bool_cancel_extraction(bool status);
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void set_recon_case_runner_type(QString type);
    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);


signals:
    void finished();
    void signal_add_fire_analysis_to_case_tree(QStringList fire_db_path);
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);

public slots:
    void slot_fire_analysis_extraction();

private slots:
    void slot_process_fire_analysis_finished(int status);
    void slot_process_fire_analysis_readyread();
    void slot_timeout_to_update_estimated_time_for_fs_modules();

private:
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    QStringList job_selected_source_count_name_list;

    bool bool_fire_analysis_extraction_process_finished = false;
    bool bool_cancel_extraction_fire_analysis = false;
    bool load_case = false;

    QMutex *mutext_for_fs_main_db;
    QSqlDatabase  destination_db;

    qint64  total_records_num = 0;
    int rec_count = 0;

    QString recon_case_runner_type;
    QString source_name;
    QString source_type;
    QString file_system_db_path;

    recon_helper_standard *recon_helper_standard_obj;
    QProcess *process_fire_analysis;

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_fire_analysis_for_fs_run_module();
    QString cmd_update_fs_run_status;

    qint64 processed_file_count_qint64 = 0;
    qint64 remaining_file_count_for_req_time_qint64 = 0;
    QString estimated_time_QString;

};
#endif // THREAD_Fire_Analysis_H
