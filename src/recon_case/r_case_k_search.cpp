#include "recon_case.h"

void RECON_case::pub_keyword_search_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    current_search_name.clear();
    keyword_search_obj->set_initialisation();
    keyword_search_obj->show();
}

void RECON_case::slot_keyword_search_plugin_selection_workdone(QString search_name, QStringList keyword_list, QStringList selected_plugin_with_os_schme_list, bool is_plugin_selected, bool is_files_selected)
{
    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Artifacts_Keyword_Search,MACRO_ACTIVITY_LOGS_TASK_QUEUED,search_name,date_time,"Task Initialised...");

    r_case_struct_queue_2 obj;
    obj.k_search_obj.search_name = search_name;
    obj.k_search_obj.keyword_list = keyword_list;
    obj.k_search_obj.bool_search_on_plugins = is_plugin_selected;
    obj.k_search_obj.bool_search_on_files = is_files_selected;
    obj.k_search_obj.plugin_with_os_scheme_list = selected_plugin_with_os_schme_list;
    obj.operation_type = enum_recon_case_queue_k_search;

    list_struct_queue_2.enqueue(obj);

    emit signal_keyword_search_extraction_start(search_name);

    if(!bool_extraction_running_queue_2)
        start_extraction_queue_2();

    recon_static_functions::app_debug("slot_keyword_search_plugin_selection_workdone selected list -- " + selected_plugin_with_os_schme_list.join(","),Q_FUNC_INFO);

}

void RECON_case::queued_operation_for_k_search(r_case_struct_queue_2 obj)
{

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Artifacts_Keyword_Search,MACRO_ACTIVITY_LOGS_TASK_START,obj.k_search_obj.search_name,date_time,"Task started...");

    current_search_name = obj.k_search_obj.search_name;

    thread_keyword_search_obj->pub_set_search_name(current_search_name);
    thread_keyword_search_obj->pub_set_bool_search_on_plugin(obj.k_search_obj.bool_search_on_plugins);
    thread_keyword_search_obj->pub_set_bool_search_on_files(obj.k_search_obj.bool_search_on_files);
    thread_keyword_search_obj->pub_set_keywords_list_for_search(obj.k_search_obj.keyword_list);
    thread_keyword_search_obj->pub_set_essentials();
    thread_keyword_search_obj->pub_set_plugin_list_for_search(obj.k_search_obj.plugin_with_os_scheme_list);
    struct struct_global_task_type_and_title t1;
    t1.title = current_search_name;
    t1.type = MACRO_JobType_Artifacts_Keyword_Search;


    emit signal_set_progress_bar_max_value(t1,obj.k_search_obj.plugin_with_os_scheme_list.size());

    thread_keyword_search_obj->start();
}


void RECON_case::slot_cancel_keyword_search(struct_global_task_type_and_title t1)
{
    recon_static_functions::app_debug("slot_cancel_keyword_search",Q_FUNC_INFO);

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Artifacts_Keyword_Search, MACRO_ACTIVITY_LOGS_TASK_ABORTED, t1.title ,date_time,"Keyword Search Cancelled.");

    if(current_search_name == t1.title)
    {
        thread_keyword_search_obj->pub_cancel_keyword_search_extraction();
        current_search_name.clear();
        return;
    }

    interrupt_queued_operation(t1);
    is_all_task_and_queue_finish();

}

void RECON_case::slot_thread_keyword_search_finished_or_cancelled()
{
    recon_static_functions::app_debug(" search name" + thread_keyword_search_obj->pub_get_search_name(),Q_FUNC_INFO);
    common_thread_k_search_finished_or_cancelled();
}

void RECON_case::common_thread_k_search_finished_or_cancelled()
{
    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Artifacts_Keyword_Search,MACRO_ACTIVITY_LOGS_TASK_COMPLETED,thread_keyword_search_obj->pub_get_search_name(),date_time,"Task Completed...");

    emit signal_keyword_search_finished_or_cancelled(thread_keyword_search_obj->pub_get_search_name());

    struct_global_task_type_and_title s1;
    s1.title = thread_keyword_search_obj->pub_get_search_name();
    s1.type = MACRO_JobType_Artifacts_Keyword_Search;
    emit signal_remove_sub_process_status_window_object(s1);
    finish_operation_queue_2();
}

