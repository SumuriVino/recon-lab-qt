#ifndef THREAD_EXIF_METADATA_SEARCH_H
#define THREAD_EXIF_METADATA_SEARCH_H

#include <QThread>
#include <QObject>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_2.h"


class thread_exif_metadata_search : public QThread
{
    Q_OBJECT

public:
    thread_exif_metadata_search(QWidget *parent = 0);


    void run();

    void pub_set_search_command_sources_search_label(QString command,QString where_statement, QStringList src_list, QString srch_label);

    void pub_cancel_extraction();

signals:
    void signal_PBAR_thread_exif_metadata_search_started(QString job_type);
    void signal_PBAR_thread_exif_metadata_search_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_exif_metadata_search_finished(QString job_type);


private:

    QString search_command;
    QString where_condition_command;

    QString search_label;
    QStringList sources_list;
    QString search_db_name;
    QString destination_db_path;

    bool bool_cancel_extraction = false;
    int processed_records = 0;
    qint64  total_records_num = 0;

    void insert_file_search_config_into_index_db();
    QString create_file_search_destination_db();
    void update_total_records_extracted_in_file_search_index_db();

    recon_helper_standard *recon_helper_standard_obj;


};

#endif // THREAD_EXIF_METADATA_SEARCH_H
