#ifndef QOBJECT_PLUGIN_EXTRACTION_H
#define QOBJECT_PLUGIN_EXTRACTION_H

#include "header_plugin_extraction_interface.h"
#include "header_global_variables_1.h"
#include "ios_backup/uid_backup/uid_backup_parser/ios_uid_backup_parser.h"
#include <QObject>
#include <QtCore>
#include "header_identical_2.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include "recon_generic_functions/recon_helper_process.h"
#include "header_global_variables_4.h"

class qobject_plugin_extraction : public QObject
{
    Q_OBJECT

public:
    explicit qobject_plugin_extraction(QObject *parent = nullptr);

    void pub_set_ios_parser_map(QMap<QString, ios_uid_backup_parser *> *obj);
    void set_run_plugin_triggered(bool value);

    void pub_set_plugin_and_tab_list_for_extraction(QList<struct_global_selected_plugins_info> list);

    void pub_cancel_everything();

    void pub_set_source_count_name_list_for_extraction(QStringList list);

    void pub_set_plugin_pointer_obj(QMap <QString, header_plugin_extraction_interface*> obj);

signals:

    void signal_change_display_message_for_plugin_task_progress_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress);
    void signal_insert_entry_in_case_tree_for_redefined_result(QString category_name);
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress);

    void signal_PBAR_plugin_extraction(QString plugin_name, qint64 record_count, QString current_count_and_total_plugins);

    void signal_PBAR_plugin_status_message(QString plugin_name, QString message);

    void signal_PBAR_thread_file_system_finished(QString job_type);

    void finished();

    void signal_plugin_extraction_finished_or_cancelled(QString plugin_name_str);

public slots:

    void slot_extract_plugins();
    void slot_skip_triggered();

private slots:
    void slot_extracted_records_from_plugin(qint64 count);
    void slot_apple_macOS_log_show_proc_readyread();
    void slot_apple_macOS_log_show_proc_finished();
    void on_slot_timeout_apple_macOS_log_collection_timer();


private:

    struct struct_r_case_artifacts_timeline
    {
        QStringList timestamp_applicable_column_list;
        QStringList display_applicable_column_list;
        QStringList display_applicable_column_datatypes_list;
        QString tab_name;
        QString table_name;
        QStringList timestamp_type_list;
    };

    int redefined_result_static_column_count = 0;
    QString target_plugin_name;
    header_plugin_extraction_interface *plugin_extraction_interface_obj;

    bool bool_plugin_extraction_canceled;
    bool bool_cancel_everything;

    QMap<QString, ios_uid_backup_parser*> *map_ios_uid_backup_parser_obj;
    QStringList supported_os_schemes_list;

    void generate_artifacts_timeline(QString currently_running_sources, QStringList selected_tab_list);
    void insert_items_in_artifacts_timeline_full_db(struct_r_case_artifacts_timeline g_timeline_obj, QString source_count_name);
    QString get_query_for_currently_running_source_count_name(QStringList currently_running_sources_list);

    bool bool_run_plugin_triggered;
    void set_plugin_with_tab_after_extraction_in_narad_muni();

    void start_redefined_result_extraction(QString currently_running_sources, QStringList selected_tab_list);
    void fill_redefined_result_selected_plugin_info_list(QSqlDatabase &destination_db, QString category_name, QString category_index);
    bool insert_into_redefined_result_db(QSqlDatabase &plugin_source_db, QString command, QString plugin_name, QString tab_name, QStringList given_column_header_list, QString redf_result_db_path, int index_of_timestamp);

    QMap<QString, QList<struct_global_redefined_result_selected_plugin_and_tab_info>> map_redefinition_category;
    void fill_map_redefined_result_category();


    QList<struct_global_selected_plugins_info> plugin_and_tab_list;

    header_plugin_extraction_interface *find_plugin_ptr_by_pluginname(QString plugin_name);
    QString current_count_and_total_plugins;

    QStringList source_selected_source_count_name_list;
    void save_plugin_tab_status(QString plugin_name_str, QStringList tab_str, QString source_count_name);
    QStringList remaining_plugin_tab_extraction_for_source(QString source_count_name, QString plugin_name, QStringList tab_list);

    QMap <QString, header_plugin_extraction_interface*> map_plugin_pointer_obj;
    recon_helper_standard *recon_helper_standard_obj;

    void empty_current_plugin_on_skip_for_source(QString source_count_name, QStringList tables_list);
    header_plugin_extraction_interface *find_plugin_ptr_for_online_account_plugin(QString plugin_name);
    QString mining_timestamp_name(QString type, QString caller_func);

    void write_apple_macOS_log_archive_data_into_text_file(QString logarchive_file_path, QString file_path);
    QProcess *prc_log_macos_log_show;
    QFile log_macos_log_show_qfile;
    bool bool_log_macos_log_show_error_status;
    bool bool_log_macos_process_finished;
    QTimer *m_timer_log_macos_log_show;
    QString log_macos_log_text_file_path;
    recon_helper_process *recon_helper_process_obj;

    void extract_log_macos_log_show(struct_GLOBAL_witness_info_source source_info_obj);
};

#endif
