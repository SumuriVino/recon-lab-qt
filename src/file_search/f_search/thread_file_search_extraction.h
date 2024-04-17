#ifndef THREAD_FILE_SEARCH_H
#define THREAD_FILE_SEARCH_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include "recon_generic_functions/recon_static_functions.h"
#include <QtCore>

#include "header_identical_2.h"
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

class thread_file_search : public QThread
{
    Q_OBJECT

public:
    thread_file_search(QWidget *parent = 0);
    ~thread_file_search();
    void run();
    void cancel_extraction();
    void set_bool_cancel_extraction();

    void set_file_search_label_name(QString name);
    void set_file_name_str_to_search(QString name);
    void set_file_size_str_to_search(QString size_str);
    void set_change_date_str_to_search(QString date_str);
    void set_added_date_str_to_search(QString date_str);
    void set_last_modification_str_to_search(QString date_str);
    void set_last_read_str_to_search(QString date_str);
    void set_creation_date_str_to_search(QString date_str);
    QString pub_get_current_file_search_label();
    void set_radio_button_or_cheched_search(bool value);
    void set_parent_dir_path_for_search(QString dir_path);
    void set_mdfind_condition_str_for_search(QString mdfind_str);
    void set_bool_run_in_full_mode(bool run_mode);
    void set_file_system_current_source_count_name(QString source_count_name);

    void resume_thread();
    void stop_thread();

    void set_mdfind_extracted_output(QString mdfind_output_str);
signals:
    void signal_run_separate_process_for_mdfind_outside_thread(QString, QString);

private:

    bool bool_cancel_extraction;
    QElapsedTimer *timer_records_fs ;
    int total_records;

    QString current_search_db_file_name;
    QString current_search_label_name;
    QString file_name_str_to_search;
    QString file_size_str_to_search;
    QString change_date_str_to_search;
    QString added_date_str_to_search;
    QString last_read_date_str_to_search;
    QString last_modif_date_str;
    QString created_date_str_to_search;
    qint64 current_date_time;
    bool is_radio_all_filter_clicked;

    void insert_file_search_config_into_index_db();
    QString create_file_search_destination_db();
    QString get_query_for_file_size_search_in_db(QString column_name);
    QString get_query_for_date_search_in_db(QString date_val, QString colume_name);
    QString get_additional_query_of_within_last_timestamp(QString val_str, QString colume_name);
    QString get_additional_query_for_exactly_timestamp(QString val_str, QString colume_name);
    QString get_additional_query_for_before_timestamp(QString val_str, QString colume_name);
    QString get_additional_query_for_after_timestamp(QString val_str, QString colume_name);
    QString get_additional_query_for_today_timestamp(QString colume_name);
    QString get_additional_query_for_yesterday_timestamp(QString column_name);
    QString get_additional_query_for_this_week_timestamp(QString column_name);
    QString get_additional_query_for_this_month_timestamp(QString colume_name);
    QString get_additional_query_for_this_year_timestamp(QString colume_name);
    QString get_additional_query_for_between_timestamp(QString val_str, QString colume_name);
    void update_total_records_extracted_in_file_search_index_db();
    QString get_query_for_file_name_search_in_db(QString column_name);

    QString parent_dir_path;
    QString mdfind_condition_str;

    bool bool_run_in_full_mode;
    QString file_system_source_count_name;

    QStringList list_int_with_source_count_name;

    //    void fill_list_for_regular_filters_info_by_logically_or(QString command, QString file_name_query_for_search_in_db, QString file_size_query_to_search_in_db, QString created_date_query_for_search_in_db, QString last_modified_date_query_for_search_in_db, QString last_read_date_query_for_search_in_db, QString added_date_query_for_search_in_db, QSqlQuery &query_select);
    //    void fill_list_for_regular_filters_info_by_logically_and(QString command, QString file_name_query_for_search_in_db, QString file_size_query_to_search_in_db, QString created_date_query_for_search_in_db, QString last_modified_date_query_for_search_in_db, QString last_read_date_query_for_search_in_db, QString added_date_query_for_search_in_db, QSqlQuery &query_select);

    void fill_list_for_spotlight_filters_info(QSqlQuery &query_select, QString fs_extration_path);
    void fill_list_for_apple_metadata_filters_info(QString source_count_name);


    bool pause_thread;
    QMutex mutex_val;
    QWaitCondition waitcondition;

    QString mdfind_searched_putput_str;
    void insert_searched_info_into_file_search_db(QSqlQuery &query_insert);

    QString destination_db_path;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif
