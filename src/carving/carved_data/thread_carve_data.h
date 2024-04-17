#ifndef THREAD_CARVE_DATA_H
#define THREAD_CARVE_DATA_H

#include <QObject>
#include <QtCore>
                                   
#include "header_global_structures.h"
#include "recon_configuration/recon_configuration.h"
#include "recon_generic_functions/recon_helper_standard.h"      

class thread_carve_data : public QThread
{
    Q_OBJECT

public:
    thread_carve_data(QWidget *parent = 0);
    ~thread_carve_data();

    void run();
    void pub_set_process_obj(QProcess *obj);
    void pub_cancel_extraction(bool status);
    void pub_set_essentials(QString src_cnt_name, QString source_plugin_name_str, QString source_tab_name_str, QString source_record_no_str, QString source_file_path);
signals:
    void signal_PBAR_thread_carved_data_started(QString job_type);
    void signal_PBAR_thread_carved_data_finished(QString job_type);
    void signal_PBAR_thread_carve_data_processing_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);

private:

    QString source_filepath;
    QString source_filename;
    QString source_plugin_name;
    QString source_tab_name;
    QString source_count_name;
    QString source_record_no;
    QString source_filepath_for_display;
    QString recon_file_from;
    QString recon_file_infopath;

    QProcess *process_carve_data;

    bool bool_cancel_extraction = false;

    struct_GLOBAL_witness_info_source st_source_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

    void parse_bulk_extractor_txt_files(QStringList txt_files_list);
};

#endif // THREAD_CARVE_DATA_H
