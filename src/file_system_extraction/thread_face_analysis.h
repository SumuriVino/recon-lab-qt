#ifndef THREAD_FACE_ANALYSIS_H
#define THREAD_FACE_ANALYSIS_H

#include <QObject>
#include <QtCore>
#include "recon_generic_functions/recon_helper_singular.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "face_extractor/facefinder.h"

class thread_face_analysis : public QObject
{
    Q_OBJECT
public:
    explicit thread_face_analysis(QObject *parent = nullptr);

    void set_recon_case_runner_type(QString type);

    void pub_set_job_selected_source_count_name_list(QStringList type);

    void pub_set_bool_cancel_extraction(bool status);

    void pub_set_face_finder_obj(facefinder *obj);

    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list);

signals:

    void finished();

    void signal_add_faces_to_case_tree(QStringList path_list);


    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

    void signal_completion_status(QString job_type, QString source_count_name, QString status);


public slots:
    void slot_extract_faces();

private:

    struct struct_filesystem_record_and_filepath
    {
        QString filesystem_record;
        QString filepath;
        bool is_phy;
        QString mime_type;

    };


    bool open_and_create_face_analysis_db(QString path);

    bool open_fs_db(QString db_path);

    QSqlDatabase in_memory_face_analysis_db;

    QSqlDatabase destination_db;

    recon_helper_singular *recon_helper_singular_obj;
    QString recon_case_runner_type;

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;

    QStringList job_selected_source_count_name_list;

    bool bool_cancel_extraction = false;

    facefinder *facefinder_obj;
    recon_helper_standard *recon_helper_standard_obj;
    QMutex *mutext_for_fs_main_db;
    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void extract_face_analysis_for_fs_run_module();

};

#endif // THREAD_FACE_ANALYSIS_H
