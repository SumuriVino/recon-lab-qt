#ifndef THREAD_CONTENT_SEARCH_H
#define THREAD_CONTENT_SEARCH_H

#include <QObject>
#include <QWidget>
#include <QtCore>
#include <QThread>
#include "file_system_extraction/dt_search/dt_search_interface.h"
#include "header_global_structures.h"
#include "header_global_variables_1.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <QFileIconProvider>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "file_system_extraction/thread_index_files.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

class thread_content_search : public QThread
{
    Q_OBJECT

public:
    thread_content_search(QWidget *parent=0);
    ~thread_content_search();

    void pub_set_dt_search_obj(dt_search_interface *obj);
    void pub_set_bool_cancel_extraction();
    void cancel_extraction();
    void run();
    void pub_set_keywords_list_to_search(QStringList list);
    void pub_set_search_label_name(QString name);
    QString pub_get_search_label_name();
    void pub_set_bool_search_in_temp_mode(bool bool_run_temp);
    void pub_set_dir_path_to_temp_index(QString path);
    void pub_set_source_count_name(QString name);

signals:
    void signal_total_size_processed_of_index_search(int size, QString source_count_name);
    void signal_set_progress_bar_max_value(struct_global_task_type_and_title, int value);
    void signal_set_progressbar_value(int total_files , QString job_title);
    void signal_set_search_label_with_records_in_progress_window(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);

private:

    dt_search_interface *dt_search_interface_obj;
    bool bool_cancel_extraction;
    QStringList keywords_list_to_search;
    QString search_label_name;
    bool bool_search_in_temp_mode;
    QString dir_path_to_temp_indexing;
    void insert_entry_in_index_db();
    QString search_db_file_name;
    QString source_count_name_str;

    QString create_content_search_destination_db();

    QList<struct_global_indexed_keyword_found_file_info> list_struct_found_keyword_file_info;

    void iterate_directory_recursively_and_index_files(QString source_path);

    QElapsedTimer *timer_records_fs ;
    int total_records;
    qint64 total_size_processed;
    int size_divided_value;
    int directory_items_count;

    void search_for_keywords_in_indexed_data(QSqlDatabase &destination_db);
    thread_index_files *thread_index_files_obj;
    recon_helper_standard *recon_helper_standard_obj;

    QString get_filepath_for_tar_rar_gz_and_zip_files(QString file_path);

};

#endif //
