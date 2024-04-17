#include "recon_case.h"

void RECON_case::pub_file_search_interface_work_done(struct_global_selected_regular_filters_info struct_selected_regular_filters_info, QString mdfind_condition_str, QString search_label_name, bool bool_run_in_full_mode, QString dir_path_to_search, QString fs_source_count_name)
{
    recon_static_functions::debug_intensive("before entering in queue file_name " +  struct_selected_regular_filters_info.file_name_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue file_size_str " +  struct_selected_regular_filters_info.file_size_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue change_date_str " +  struct_selected_regular_filters_info.change_date_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue added_date_str " +  struct_selected_regular_filters_info.added_date_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue last_modified_date_str " +  struct_selected_regular_filters_info.last_modified_date_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue last_read_date_str " +  struct_selected_regular_filters_info.last_read_date_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue create_date_str " +  struct_selected_regular_filters_info.created_date_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue file_search_label_name " +  search_label_name,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue bool_is_combined_by_or " +  QString::number(struct_selected_regular_filters_info.bool_is_combined_by_or),Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue dir_path_to_search " +  dir_path_to_search,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue mdfind_condition_str " +  mdfind_condition_str,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue bool_run_in_full_mode " +  bool_run_in_full_mode,Q_FUNC_INFO);
    recon_static_functions::debug_intensive("before entering in queue fs_source_count_name " +  fs_source_count_name,Q_FUNC_INFO);

    bool_thread_file_search_regular_filter_or_apple_metadata_running = true;

    bool_cancel_operation = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    thread_file_search_obj->set_bool_cancel_extraction();
    thread_file_search_obj->set_file_search_label_name(search_label_name);
    thread_file_search_obj->set_file_name_str_to_search(struct_selected_regular_filters_info.file_name_str);
    thread_file_search_obj->set_file_size_str_to_search(struct_selected_regular_filters_info.file_size_str);
    thread_file_search_obj->set_change_date_str_to_search(struct_selected_regular_filters_info.change_date_str);
    thread_file_search_obj->set_added_date_str_to_search(struct_selected_regular_filters_info.added_date_str);
    thread_file_search_obj->set_last_modification_str_to_search(struct_selected_regular_filters_info.last_modified_date_str);
    thread_file_search_obj->set_last_read_str_to_search(struct_selected_regular_filters_info.last_read_date_str);
    thread_file_search_obj->set_creation_date_str_to_search(struct_selected_regular_filters_info.created_date_str);
    thread_file_search_obj->set_radio_button_or_cheched_search(struct_selected_regular_filters_info.bool_is_combined_by_or);
    thread_file_search_obj->set_parent_dir_path_for_search(dir_path_to_search);
    thread_file_search_obj->set_mdfind_condition_str_for_search(mdfind_condition_str);
    thread_file_search_obj->set_bool_run_in_full_mode(bool_run_in_full_mode);
    thread_file_search_obj->set_file_system_current_source_count_name(fs_source_count_name);

    thread_file_search_obj->start();
}

void RECON_case::queued_operation_for_file_search(r_case_struct_queue_2 obj)
{
    thread_file_search_obj->set_bool_cancel_extraction();
    thread_file_search_obj->set_file_search_label_name(obj.struct_file_search_obj.file_search_label_name);
    thread_file_search_obj->set_file_name_str_to_search(obj.struct_file_search_obj.file_name_str);
    thread_file_search_obj->set_file_size_str_to_search(obj.struct_file_search_obj.file_size_str);
    thread_file_search_obj->set_change_date_str_to_search(obj.struct_file_search_obj.change_date_str);
    thread_file_search_obj->set_added_date_str_to_search(obj.struct_file_search_obj.added_date_str);
    thread_file_search_obj->set_last_modification_str_to_search(obj.struct_file_search_obj.last_modification_date_str);
    thread_file_search_obj->set_last_read_str_to_search(obj.struct_file_search_obj.last_read_date_str);
    thread_file_search_obj->set_creation_date_str_to_search(obj.struct_file_search_obj.created_date_str);
    thread_file_search_obj->set_radio_button_or_cheched_search(obj.struct_file_search_obj.is_radio_or_clicked);
    thread_file_search_obj->set_parent_dir_path_for_search(obj.struct_file_search_obj.parent_dir_path);
    thread_file_search_obj->set_mdfind_condition_str_for_search(obj.struct_file_search_obj.mdfind_condition_str);
    thread_file_search_obj->set_bool_run_in_full_mode(obj.struct_file_search_obj.bool_run_in_full_mode);
    thread_file_search_obj->set_file_system_current_source_count_name(obj.struct_file_search_obj.file_system_source_count_name);

    thread_file_search_obj->start();

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_File_Search,MACRO_ACTIVITY_LOGS_TASK_START,obj.struct_file_search_obj.file_search_label_name,date_time,"FileSearch Started...");
}

void RECON_case::slot_thread_global_search_extraction_finished_or_cancelled()
{
    recon_static_functions::app_debug(" search name " + thread_file_search_obj->pub_get_current_file_search_label(),Q_FUNC_INFO);

    display_loading_progress_bar_obj->hide();
    bool_thread_file_search_regular_filter_or_apple_metadata_running = false;

    if(bool_cancel_operation)
    {
        return;
    }

    emit signal_file_search_finished_or_cancelled(thread_file_search_obj->pub_get_current_file_search_label());


    //    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    //    emit signal_activity_logs_update_status(MACRO_JobType_File_Search,MACRO_ACTIVITY_LOGS_TASK_COMPLETED,thread_file_search_obj->pub_get_current_file_search_label(),date_time,"FileSearch Finished...");

    //    struct_global_task_type_and_title s1;
    //    s1.title = thread_file_search_obj->pub_get_current_file_search_label();
    //    s1.type = MACRO_JobType_File_Search;
    //    emit signal_remove_sub_process_status_window_object(s1);
    //    finish_operation_queue_2();
}

void RECON_case::slot_cancel_file_search_thread_extraction(struct_global_task_type_and_title t1)
{
    recon_static_functions::app_debug(" Start ",Q_FUNC_INFO);

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_File_Search,MACRO_ACTIVITY_LOGS_TASK_ABORTED,thread_file_search_obj->pub_get_current_file_search_label() ,date_time,"File Search Cancelled.");

    if(thread_file_search_obj->pub_get_current_file_search_label() == t1.title)
    {
        thread_file_search_obj->cancel_extraction();
    }

    emit signal_remove_sub_process_status_window_object(t1);

    interrupt_queued_operation(t1);
    is_all_task_and_queue_finish();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

void RECON_case::slot_run_mdfind_in_separate_function(QString dir_path, QString mdfind_search_condition_str)
{
    recon_static_functions::app_debug("Start ",Q_FUNC_INFO);

    QString command_mdfind = "mdfind";

    QStringList arguments;
    arguments <<  "-onlyin" << dir_path  << mdfind_search_condition_str;

    QProcess *process_mdfind = new QProcess(this);
    connect(process_mdfind, SIGNAL(finished(int)), this, SLOT(slot_mdfind_process_finished(int)));

    bool_mdfind_process_is_finished = false;
    process_mdfind->start(command_mdfind,arguments);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_mdfind_process_is_finished)
        {
            break;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    QString output = QString::fromLocal8Bit(process_mdfind->readAll().trimmed());

    thread_file_search_obj->set_mdfind_extracted_output(output);
    thread_file_search_obj->resume_thread();

    recon_static_functions::app_debug("End ",Q_FUNC_INFO);
}

void RECON_case::slot_mdfind_process_finished(int exit_code)
{
    bool_mdfind_process_is_finished = true;
}
