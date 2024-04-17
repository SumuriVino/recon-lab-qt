#ifndef THREAD_APPLE_TIMESTAMPS_H
#define THREAD_APPLE_TIMESTAMPS_H

#include <QObject>
#include <QtCore>
#include <QWidget>
#include <QThread>
#include "recon_generic_functions/recon_static_functions.h"

#include "header_global_structures.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include "rcbuild/build_items_common_for_recon_and_apple_metadata_reader.h"
#include "header_global_variables_4.h"

class thread_apple_timestamps : public QThread
{
    Q_OBJECT



public:
    thread_apple_timestamps(QWidget *parent = 0);
    ~thread_apple_timestamps();


    void run();


    void set_essential();

    void set_recon_case_runner_type(QString type);
    void pub_set_FS_source_selected_source_count_name_list(QStringList m_list);

    void pub_set_mutex_for_fs_main_db(QMutex *mutex);

    void pub_set_apple_timestamps_reader_process(QProcess *mobj);

    void pub_set_apple_metadata_all_attributes_name_list(QStringList attributes_name_list);
public slots:

signals:


    void signal_PBAR_thread_apple_timestamp_started(QString job_type);
    void signal_PBAR_thread_apple_timestamp_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_apple_timestamp_finished(QString job_type);
    void signal_thread_apple_timestamp_finished();

private slots:
    void slot_prc_apple_timestamps_readyread();
private:

    struct struct_filesystem_record_and_filepath
    {
        QString filesystem_record;
        QString filepath;
    };


    QString recon_case_runner_type;
    QStringList source_selected_source_count_name_list;

    QSqlDatabase  file_system_db;
    QSqlDatabase  in_memory_apple_timestamp_db;

    QList<struct_filesystem_record_and_filepath> list_struct_fs_rec_and_path;

    bool open_and_create_apple_timestamp_db(QString apple_timestamp_db_path);
    bool open_fs_db(QString fs_db_path);
    void intermediate_save_data_to_apple_timestamp_db(QString apple_timestamp_database_path);

    void copy_data_from_apple_timestamp_to_fs_db(QString source_count_name);

    QMutex *mutext_for_fs_main_db;


    QStringList attributes_actual_key_list;
    QStringList attribute_column_list_in_apple_timestamp_db;
    QStringList attribute_column_list_in_fs_db;
    recon_helper_standard *recon_helper_standard_obj;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    void generate_filesystem_timeline(QString source_count_name);

    QMap <QString ,QMap <QString ,QString> > map_apple_metadata_for_recon_logical_image;
    bool fill_apple_timestamps_from_script(QString file_path, QStringList keys_list , struct_global_apple_metadata_parsed_data  &extended_attr_parsed_data_obj);
    QStringList apple_metadata_attributes_name_list;
    QProcess *process_apple_timestamps_reader;
    void extract_apple_timestamps_from_apple_metadata_reader(QStringList file_path, QString source_cnt_name, QString virtual_source_path);
    void copy_tmp_apple_timestamps_to_actual_apple_timestamps_db(QString tmp_fs_db_path, QString actual_db_path);
};

#endif // THREAD_APPLE_TIMESTAMPS_H
