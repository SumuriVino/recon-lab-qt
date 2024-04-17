#ifndef RECON_CASE_H
#define RECON_CASE_H

#include <QObject>
#include <QtCore>
#include <QMessageBox>


#include "artifacts_keyword_search/artifacts_keyword_search_interface.h"
#include "artifacts_keyword_search/k_search/thread_artifacts_keyword_search.h"
#include "global_report_interface/global_report_interface.h"
#include "report/thread_report_builder/thread_report_builder.h"
#include "progress_bar/progress_bar_fixed_in_statusbar.h"
#include "run_plugin/run_plugin.h"
#include "file_search/f_search/thread_file_search_extraction.h"
#include "activity_logs/activity_logs_storage.h"
#include "ui_custom_classes/r_treewidget.h"
#include "file_system_extraction/dt_search/dt_search_interface.h"
#include "content_search/thread_content_search.h"
#include "file_system_extraction/thread_apple_metadata.h"
#include "file_system_extraction/thread_signature.h"
#include "file_system_extraction/thread_exif_metadata.h"
#include "file_system_extraction/thread_mime_type.h"
#include "file_system_extraction/thread_hashes.h"
#include "task_status/task_status_window.h"
#include "file_system_extraction/thread_index_files.h"
#include "file_system_extraction/thread_file_system.h"
#include "file_system_extraction/thread_apple_timestamps.h"
#include "plugin_extraction/qobject_plugin_extraction.h"
#include"file_viewer/registry_viewer/registry_viewer.h"

#include "global_recon_file_info/global_recon_file_info.h"
#include "recon_generic_functions/recon_helper_standard.h"      

#include "file_system_extraction/thread_face_analysis.h"
#include "face_extractor/facefinder.h"
#include "face_extractor/recognize_face.h"
#include "file_system_extraction/thread_optical_character_recognition.h"
#include "file_system_extraction/thread_skin_tone_detection.h"
#include "file_system_extraction/thread_weapons.h"
#include "file_system_extraction/thread_fire_analysis.h"
#include "file_system_extraction/thread_mail_pst.h"
#include "file_system_extraction/thread_mail_emlx.h"
#include "file_system_extraction/thread_mail_mbox.h"
#include "file_system_extraction/thread_unified_logs.h"

class RECON_case : public QWidget
{
    Q_OBJECT
public:
    explicit RECON_case(QWidget *parent = 0);
    ~RECON_case();


    void set_selected_pluginname_list_for_extraction(QStringList);
    void set_progress_bar_object(progress_bar_fixed_in_statusbar *obj);
    void pub_set_task_status_window_object(task_status_window *obj);
    bool is_plugin_selected_for_exraction(QString plugin_name);

    void set_essentials();
    void start_extraction();

    void pub_set_FS_source_selected_source_count_name_list(QStringList m_list);


    void set_ios_parser_map(QMap<QString, ios_uid_backup_parser *> *obj);

    void add_source_in_list_others_folder(QString virtual_source_path, QString target_os_version, QStringList target_users_list);

    void pub_file_search_interface_work_done(struct_global_selected_regular_filters_info struct_selected_regular_filters_info, QString mdfind_condition_str, QString search_label_name, bool bool_run_in_full_mode, QString dir_path_to_search, QString fs_source_count_name);
    void set_case_treewidget_obj(R_Treewidget *obj);

    void pub_set_dt_search_obj(dt_search_interface *obj);
    void pub_content_search_start(QStringList keyword_list, bool bool_search_in_temp_mode, QString dir_path_to_index, QString source_count_name,QString search_label_name);
    void file_system_extraction_initiator();
    void pub_set_file_system_selected_job_source_count_name(struct_global_file_system_post_launcher_job_info obj);

    void pub_start_file_system_post_launcher_jobs();

    void pub_set_recon_case_runner_type(QString type);

    void pub_start_file_indexing_from_toolbar();

    bool pub_is_file_system_thread_running();
    bool pub_is_artifacts_thread_running();

    void save_selected_plugin_tab_info_in_db();

    bool pub_is_text_indexing_thread_running();
    void pub_stop_all_running_threads();

    void pub_set_file_system_thread_not_running();

    void pub_keyword_search_clicked();

    void pub_start_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    bool pub_get_apple_metadata_run_status();
    bool pub_get_exif_metadata_run_status();
    bool pub_get_signature_analysis_run_status();
    bool pub_get_mime_type_analysis_run_status();
    bool pub_get_hashset_run_status();
    bool pub_get_face_analysis_run_status();
    void pub_start_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);

    bool pub_get_optical_character_recognition_run_status();
    bool pub_get_weapons_run_status();
    bool pub_get_fire_analysis_run_status();
    bool pub_get_skin_tone_detection_run_status();

    void pub_start_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_hashset(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_weapons(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void pub_start_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);

    void pub_start_hashes_data_extraction();

    QMutex *pub_get_fs_db_exclusive_mutex();
    void pub_set_global_recon_file_info_obj(global_recon_file_info *obj);

    bool pub_is_source_extraction_completed(QString source_count_name);
    void pub_rcase_extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name, QString virtual_source_path, QStringList apple_metadata_att_list, QStringList column_name_list);

    void pub_set_face_finder_obj(facefinder *obj);
    void pub_set_recognize_face_obj(recognize_face *obj);
    void pub_add_matched_faces_to_case_tree_for_load_case(QString search_label_name);
    bool pub_is_source_mime_type_extraction_completed(QString source_count_name);

public slots:
    void slot_right_clicked_artifacts_timeline_full();
    void slot_right_click_global_report_clicked();
    void slot_right_click_run_plugins_clicked();

signals:
    void signal_remove_sub_process_status_window_object(struct struct_global_task_type_and_title);
    void signal_current_plugin_extraction_finished_or_cancelled(QString plugin_name);
    void signal_keyword_search_finished_or_cancelled(QString search_tag);
    void signal_artifacts_timeline_full_clicked(QString timeline_name);
    void signal_global_report_finished_or_cancelled(QStringList report_paths);
    void signal_show_progressbar(bool);
    void signal_keyword_search_extraction_start(QString);
    void signal_global_report_extraction_start(QString);
    void signal_set_progress_bar_max_value(struct_global_task_type_and_title, int value);
    void signal_extracted_records_by_plugin(qint64 extracted_records);
    void signal_remove_hashset_entry_from_status_window(struct struct_global_task_type_and_title);
    void signal_redefined_result_finished_or_cancelled(QString category_name);
    void signal_change_display_message_on_task_progress_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress);
    void signal_file_search_finished_or_cancelled(QString search_label);
    void signal_set_progressbar_value(int extracted_tabs , QString job_title);
    void signal_activity_logs_update_status(QString category, QString status_MACRO, QString taskname, qint64 current_date_time, QString comments);
    void signal_extraction_of_interrupted_dir_done_for_case_tree_fs(QString clicked_dir_name, QString clicked_source_count_name, QString dir_path, QString source_count_name);
    void signal_content_search_extraction_start(QString search_label_name);
    void signal_content_search_extraction_finished_or_cancelled(QString label_name);
    void signal_insert_redefined_result_entry_in_case_tree(QString category_name);
    void signal_remove_entry_file_type_based_in_stack_and_refresh_widget(QString parent_name, QString child_name);
    void signal_remove_category_and_feature_box_for_run_plugin();
    void signal_disable_case_tree_sidebar(bool status);

    void signal_remove_plugin_entry_from_display_tree(QStringList plugin_list);
    void signal_update_records_for_hashset_in_case_tree(QStringList hashset_list, QString source_count);
    void signal_filesystem_extraction_finished();
    void signal_email_files_case_tree_add_finished();
    void signal_google_mail_files_case_tree_add_finished();
    void signal_registry_files_extraction_work_finished();
    void signal_process_tsk_fs_custom_status(QString custom_msg);
    void signal_r_case_add_all_available_hashes_to_case_tree();
    void signal_add_file_basic_to_case_from_rcase_finished();
    void signal_add_internal_sources_to_case_from_rcase_finished();
    void signal_set_search_label_with_records_in_progress_window_content_search(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_r_case_thread_exif_metadata_search_cancelled();


    void signal_process_apple_timestamps_status(QString custom_msg);
    void signal_process_apple_metadata_status(QString custom_msg);
    void signal_prc_apple_metadata_prc_left_click_finished();

    void signal_PBAR_artifacts_loading_started(QString job_type);
    void signal_PBAR_artifacts_loading_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress, bool bool_infinite_process);
    void signal_PBAR_artifacts_loading_finished(QString job_type);
    void signal_add_tags_in_case_tree(QString);

private slots:

    void slot_keyword_search_plugin_selection_workdone(QString search_name, QStringList keyword_list, QStringList selected_plugin_with_os_schme_list, bool is_plugin_selected, bool is_files_selected);
    void slot_thread_keyword_search_finished_or_cancelled();
    void slot_thread_global_report_finished_or_cancelled();
    void slot_pluginlist_from_run_plugin();
    void slot_global_report_plugin_selection_workdone(narad_muni*mobj,QStringList report_plugin_info_list, QString rpt_name, QStringList rpt_file_path_list,QStringList rpt_type_list, QString rpt_scope,QStringList selctd_tags_list);
    void slot_thread_file_system_finished();
    void slot_thread_apple_timestamp_finished();
    void slot_extracted_records_from_plugin(qint64 total_records_extracted);
    void slot_thread_global_search_extraction_finished_or_cancelled();
    void slot_cancel_file_search_thread_extraction(struct_global_task_type_and_title t1);

    void slot_change_display_message_of_task_progress_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress);

    void slot_thread_indexed_k_search_extraction_finished_or_cancelled();
    void slot_cancel_content_search(struct_global_task_type_and_title t1);
    void slot_insert_entry_in_case_tree_for_redefined_result(QString category_name);

    void slot_run_mdfind_in_separate_function(QString dir_path, QString mdfind_search_condition_str);
    void slot_mdfind_process_finished(int exit_code);
    void slot_remove_entry_for_file_types_based_in_main_stack(QString parent_name, QString child_name);
    void slot_apple_metadata_thread_finish();
    void slot_mime_type_thread_finish();
    void slot_thread_extract_plugin_finished();


    void slot_signature_thread_finish();
    void slot_extracted_progress_bar_value_for_plugin(int value);
    void slot_add_mime_type_to_case_tree(QStringList db_path_list);
    void slot_add_apple_metadata_to_case(QStringList db_path_list);
    void slot_exif_metadata_thread_finish();
    void slot_add_exif_data_to_case_tree(QStringList db_path_list);
    void slot_hashes_thread_finish();
    void slot_dt_search_thread_finish();

    void slot_add_matched_hashes_to_case_tree(QString hashes_db_path, QString source_count);

    void slot_PBAR_file_system_job_cancelled_requested(QString job_type);


    void slot_task_status_window_skip_triggered();

    void slot_current_plugin_extraction_finished_or_cancelled(QString plugin_name);


public slots:
    void slot_cancel_plugin_extraction(struct_global_task_type_and_title t1);
    void slot_cancel_keyword_search(struct_global_task_type_and_title t1);


    void slot_recon_config_apply_button_clicked();
private slots:
    void slot_job_type_completion_status(QString job_type, QString source_count_name, QString status);

    void slot_add_file_basic_to_case_tree(QString db_path);
    void slot_add_all_signatures_to_case_tree(QStringList db_path_list);
    void slot_hide_loading_display_dialog_box(bool cancel_status);
    void slot_add_internal_sources_to_case(QString source_cnt_name);
    void slot_add_registry_files(QStringList all_reg_files_path_list, QString source_count_name);

    void slot_add_matched_hashes_for_fs_module_to_case_tree(QStringList hashset_db_path_list);
    void slot_all_ios_sources_extraction_finished();
    void slot_tsk_fs_process_data();
    void slot_tsk_fs_finished(int exit_code);
    void slot_source_extraction_completed_only_for_basic_file_system(QString source_count_nm);
    void slot_report_encrypt_prcoess(int);

    void slot_prc_apple_timestamps_mergechannel_readyread();
    void slot_prc_apple_metadata_mergechannel_readyread();
    void slot_prc_apple_metadata_prc_left_click_finished(int status);

    void slot_face_analysis_thread_finished();
    void slot_add_faces_to_case_tree(QStringList db_path_list);
    void slot_add_matched_faces_to_case_tree(QString srch_lbl_name);
    void slot_add_optical_character_recognition_to_case_tree(QStringList db_path_list);
    void slot_optical_character_recognition_thread_finish();
    void slot_skin_tone_detection_thread_finished();
    void slot_add_skin_tone_detection_to_case_tree(QStringList db_path_list);
    void slot_weapons_thread_finished();
    void slot_add_weapons_to_case_tree(QStringList db_path_list);
    void slot_fire_analysis_thread_finished();
    void slot_add_fire_analysis_to_case_tree(QStringList db_path_list);
    void slot_right_click_remove_record_refresh_in_case_tree(QStringList db_path_list, QString file_type_parent);
    void slot_source_extraction_mime_type_completed(QString source_count_nm);

    void slot_add_emails_data_in_case_tree(QStringList db_path_list);
    void slot_pst_mail_thread_finished();
    void slot_mail_emlx_thread_finish();
    void slot_mail_mbox_thread_finish();

    void slot_unified_logs_thread_finish();
    void slot_add_unified_log_file_to_case_tree(QString source_count_name);

private:

    enum enum_recon_case_queue_operation
    {
        enum_recon_case_queue_run_plugin,
        enum_recon_case_queue_k_search,
        enum_recon_case_queue_g_timeline,
        enum_recon_case_queue_g_report,
        enum_recon_case_queue_redefined_result,
        enum_recon_case_queue_file_search,
        enum_recon_case_queue_file_system,
        enum_recon_case_queue_content_search,
        enum_recon_case_queue_hashset
    };

    struct r_case_struct_run_plugin
    {
        QStringList plugin_list;
    };

    struct r_case_struct_k_search
    {
        QString search_name;
        QStringList keyword_list;
        bool bool_search_on_plugins;
        bool bool_search_on_files;
        QStringList plugin_with_os_scheme_list;
    };


    struct r_case_struct_g_timeline
    {
        QString timeline_name;
        QStringList plugins_list_with_os_name;
        qint64 start_timestamp;
        qint64 end_timestamp;
    };


    struct r_case_struct_redefined_result
    {
        QString category_name;
        QString category_index;
    };

    struct r_case_struct_file_search
    {
        QString file_search_label_name;
        QString file_name_str;
        QString file_size_str;
        QString change_date_str;
        QString added_date_str;
        QString last_modification_date_str;
        QString last_read_date_str;
        QString created_date_str;
        QString parent_dir_path;
        QString mdfind_condition_str;
        QString file_system_source_count_name;  // Because this field will use only FS Current dir search mode not in full search mode
        bool bool_run_in_full_mode;
        bool is_radio_or_clicked;
    };

    struct r_case_struct_content_search
    {
        QString label_name;
        QStringList keywords_list;
        bool bool_run_in_temp_mode;
        QString dir_path_to_temp_index;
        QString source_count_name;
    };

    struct struct_file_system_extraction_queue
    {
        // Entry for Load case only
        bool bool_run_fs_thread_for_load_case;

        // Thsese entries only for extract already added sources
        bool bool_extract_already_added_sources;
        QStringList already_added_source_count_name_list;
    };

    struct r_case_struct_queue_1
    {
        int operation_type;
        struct_file_system_extraction_queue struct_file_system_obj;
    };

    struct r_case_struct_queue_2
    {
        int operation_type;
        r_case_struct_run_plugin run_plugin_obj;
        r_case_struct_k_search k_search_obj;
        r_case_struct_g_timeline g_timeline_obj;
        struct_global_global_report_info g_report_obj;
        r_case_struct_redefined_result redefined_result_obj;
        r_case_struct_file_search struct_file_search_obj;
        r_case_struct_content_search struct_content_search_obj;

    };



    QString target_media_path;
    QString mount_path;
    int target_media_type;
    QString output_directory;
    QStringList list_selected_plugins;
    QStringList list_extracted_plugins;

    QString OS_version;

    QString system_timezone_name;
    qint64 system_timezone_offset_seconds;

    QString user_timezone_name;
    qint64 user_timezone_offset_seconds;

    void plugin_null_ptr_found();

    QList<struct_global_selected_plugins_info> selected_plugins_info_list;

    QString current_plugin_being_extracted;
    QString current_search_name;
    QString current_report_name;
    header_plugin_extraction_interface *find_plugin_ptr_by_pluginname(QString plugin_name);

    QSemaphore *semaphore_plugin_list;


    QStringList received_plugin_list_for_current_extraction;

    artifacts_keyword_search_interface *keyword_search_obj;
    global_report_interface *global_report_interface_obj;

    thread_artifacts_keyword_search *thread_keyword_search_obj;
    run_plugin *run_plugin_obj;

    thread_report_builder *thread_report_builder_obj;
    bool bool_cancel_operation;
    header_plugin_extraction_interface *plugin_extraction_interface_obj;

    void common_right_click_run_plugins_clicked();
    void common_thread_k_search_finished_or_cancelled();

    QQueue <r_case_struct_queue_2> list_struct_queue_2;
    void start_extraction_queue_2();
    void finish_operation_queue_2();
    void interrupt_queued_operation(struct_global_task_type_and_title obj);
    bool bool_extraction_running_queue_2;

    void queued_operation_for_k_search(r_case_struct_queue_2 obj);
    void queued_operation_for_g_report(r_case_struct_queue_2 obj);

    QStringList extract_already_exist_plugins_in_queued_list(QStringList list);

    void common_global_report_plugin_selection_workdone(narad_muni*mobj, QStringList report_plugin_info_list, QString rpt_name, QStringList rpt_file_path_list,QStringList rpt_type_list, QString rpt_scope,QStringList selctd_tags_list);

    int count_for_report;

    progress_bar_fixed_in_statusbar *progress_bar_obj;

    task_status_window *task_status_window_obj;

    bool bool_thread_hashset_cancel_finish;

    bool is_all_task_and_queue_finish();


    bool bool_run_plugin_triggered;

    QMap<QString, ios_uid_backup_parser*> *map_ios_uid_backup_parser_obj;

    QStringList finished_thread_hashset_source_count_name_list;

    QString current_redefined_result_category_name;

    void make_struct_plugin_extraction_list_for_first_and_add_source();
    void make_struct_plugin_extraction_list_for_run_plugin();

    QStringList supported_os_schemes_list_for_current_plugin;
    bool bool_cancel_plugin_extraction_click;

    QStringList extracted_source_count_name_list;
    void queued_operation_for_file_search(r_case_struct_queue_2 obj);
    thread_file_search *thread_file_search_obj;

    QQueue <r_case_struct_queue_1> list_struct_queue_1;
    bool bool_extraction_running_queue_1;


    QString finished_fs_source_count_name;

    thread_file_system *thread_file_system_obj;
    thread_apple_timestamps *thread_apple_timestamps_obj;
    thread_apple_metadata *thread_apple_metadata_obj;
    thread_mime_type *thread_mime_type_obj;
    QThread *thread_extended_attr_extract;
    QThread *thread_mime_extract;

    activity_logs_storage *activity_log_storage_obj;

    R_Treewidget *treewidget_case_display_obj;


    dt_search_interface *dt_search_interface_obj;
    thread_content_search *thread_content_search_obj;

    void queued_operation_for_content_search(r_case_struct_queue_2 obj);

    QQueue <r_case_struct_queue_1> list_struct_queue_3;
    bool bool_extraction_running_queue_3;
    void start_extraction_queue_3();


    QString finished_hashset_source_count_name;
    void finish_operation_queue_3();

    bool bool_extract_already_added_sources;
    bool bool_mdfind_process_is_finished;



    bool bool_add_plugin_to_case_tree_and_activity_log;

    void start_apple_metadata_extraction();
    void start_mime_type_extraction();
    thread_signature *thread_signature_obj;
    QThread *thread_signature_extract;
    void start_signature_extraction();
    void add_mime_types_in_case_tree(QStringList db_path_list);
    void add_file_signatures_in_case_tree(QStringList db_path_list);
    bool bool_file_system_run;
    void add_apple_metadata_in_case_tree(QStringList db_path_list);
    void add_metadata_by_apple_metadata_in_case_tree(QString db_path);
    void add_locations_apple_meta_in_case_tree(QStringList db_path_list);

    thread_exif_metadata *thread_exif_metadata_obj;
    QThread *thread_exif_data_extract;
    void start_exif_data_extraction();
    void add_exif_metadata_in_case_tree(QStringList db_path_list);
    void add_locations_exif_in_case_tree(QStringList db_path_list);

    thread_hashes *thread_hashes_obj;
    QThread *thread_hashes_data_extract;

    QMutex *mutex_exclusive_for_fs_main_db;

    thread_index_files *thread_index_files_obj;
    QThread *thread_dt_search_extract;
    void start_text_indexing_extraction();
    void add_matched_hashes_in_case_tree(QString hashes_db_path, QString source_count);

    struct_global_file_system_post_launcher_job_info st_file_system_post_launcher_job_info_obj;
    void add_file_extension_by_user_defined_extension_in_case_tree(QString fs_db_path);
    void add_file_extensions_by_all_extensions_in_case_tree(QString fs_db_path);


    QStringList source_selected_source_count_name_list;

    void clear_case_tree_children(int parent_position);
    void clear_case_tree_grand_children(int parent_position, QString feature_name);

    QString recon_case_runner_type;
    qint64 get_matched_hahses_count_from_hashset(QString hashes_db_path, QString hashset_path);

    bool bool_thread_mime_type_running = false;
    bool bool_thread_apple_metadata_running= false;
    bool bool_thread_signature_running = false;
    bool bool_thread_exif_data_running = false;
    bool bool_thread_hashes_running = false;
    bool bool_thread_text_indexing_running = false;
    bool bool_thread_file_system_running = false;
    bool bool_thread_file_search_regular_filter_or_apple_metadata_running = false;



    void start_apple_timestamps_extraction();
    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_non_cancelable_progress_bar_obj;

    void start_plugin_extractor_qobject();
    qobject_plugin_extraction *qobject_run_multiple_plugins_obj;
    QThread *thread_qobject_run_multiple_plugins;

    bool bool_plugin_extraction_qobject_running;
    QStringList sources_pending_in_queue_for_plugin_extraction;

    QMap <QString, header_plugin_extraction_interface*> map_plugin_pointer_obj;

    void add_file_size_in_case_tree(QString db_path);



    void add_mobile_backup_info_in_case_tree(QString source_count_name);
    void add_disk_images_info_in_case_tree(QString source_count_name);
    void add_registry_file_in_bucket_registry_viewer(QString display_tab_name_str);
    void parse_registry_files_list(QStringList complete_file_path_list, QString source_count_name);
    void add_window_ram_images_info_in_case_tree(QString source_count_name);

    QProcess *process_tsk_fs;
    global_recon_file_info *global_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

    QProcess *process_apple_timestamps;
    QProcess *process_apple_metadata_full_source;
    QProcess *process_apple_metadata_file_click;
    QProcess *process_apple_metadata_right_click;
    thread_apple_metadata *thread_apple_metadata_file_click_obj;


    void add_redefined_result_locations_for_apple_meta_and_exif_in_case_tree();

    QProcess *report_encrypt_prcoess;
    bool bool_report_encrypt_process;
    QStringList apple_all_attribute_name_list;
    void copy_filesystem_dir_to_extracted_content_dir();
    void set_apple_metadata_reader_file_system();

    void start_face_analysis_extraction();
    bool bool_thread_face_analysis_running = false;
    thread_face_analysis *thread_face_analysis_obj;
    facefinder *facefinder_obj;
    recognize_face *recognize_face_obj;
    void add_face_analysis_result_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh);
    QThread *thread_face_extract;

    bool bool_thread_optical_character_recognition_running = false;
    QThread *thread_optical_character_recognition_extract;
    thread_optical_character_recognition *thread_optical_character_recognition_obj;

    void start_optical_character_recognition_extraction();
    void add_optical_character_recognition_result_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh);

    void start_skin_tone_detection_extraction();
    bool bool_thread_skin_tone_detection_running = false;
    void add_skin_tone_detection_result_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh);
    thread_skin_tone_detection *thread_skin_tone_detection_obj;
    QThread *thread_skin_tone_detection_extraction;

    void start_weapons_extraction();
    void add_weapons_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh);
    bool bool_thread_weapons_running = false;
    thread_weapons *thread_weapons_obj;
    QThread *thread_weapons_extraction;

    void start_fire_analysis_extraction();
    void add_fire_analysis_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh);
    bool bool_thread_fire_analysis_running = false;
    thread_fire_analysis *thread_fire_analysis_obj;
    QThread *thread_fire_analysis_extraction;

    QStringList extracted_mime_type_source_count_name_list;

    bool bool_thread_pst_mail_running = false;
    void start_mail_pst_extraction();
    void add_emails_data_in_case_tree(QStringList db_path_list , bool bool_navigation_refresh);
    thread_mail_pst *thread_pst_mail_obj;
    QThread *thread_pst_mail_extraction;
    QMutex *mutex_exclusive_for_email_db;

    bool bool_thread_mail_emlx_files_running = false;
    void start_mail_emlx_extraction();
    thread_mail_emlx *thread_mail_emlx_obj;
    QThread *thread_mail_emlx_extraction;

    bool bool_thread_mbox_mail_running = false;
    void start_mail_mbox_extraction();
    thread_mail_mbox *thread_mail_mbox_obj;
    QThread *thread_mail_mbox_extraction;

    void start_unified_logs_extraction();
    bool bool_thread_unified_logs_running = false;
    thread_unified_logs *thread_unified_logs_obj;
    QThread *thread_unified_logs_extraction;
    void add_unified_logs_file_in_case_tree(QString source_count_name);

};

#endif
