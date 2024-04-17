#include "recon_case.h"

void RECON_case::pub_content_search_start(QStringList keyword_list, bool bool_search_in_temp_mode, QString dir_path_to_index, QString source_count_name, QString search_label_name)
{
    recon_static_functions::app_debug("Search before entering in queue keywords list " +  keyword_list.join(","),Q_FUNC_INFO);

    QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";


    search_label_name = search_label_name.trimmed();
    if(search_label_name.isEmpty())
    {
        QString command = "SELECT COUNT(*) FROM table_index_content_search";
        QString value = recon_helper_standard_obj->get_string_from_db_by_dbpath(command, 0, index_db_path, Q_FUNC_INFO);
        search_label_name = "Index search " + QString::number(value.toInt() + 1);
    }
    search_label_name.replace(" ", "_");

    QString content_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();

    search_label_name.append(".sqlite");
    search_label_name = recon_static_functions::get_available_filename(search_label_name, content_search_dir, Q_FUNC_INFO);

    if(search_label_name.endsWith(".sqlite"))
        search_label_name.chop(QString(".sqlite").size());


    r_case_struct_queue_2 obj;
    obj.struct_content_search_obj.keywords_list = keyword_list;
    obj.struct_content_search_obj.label_name = search_label_name;
    obj.struct_content_search_obj.bool_run_in_temp_mode = bool_search_in_temp_mode;
    obj.struct_content_search_obj.dir_path_to_temp_index = dir_path_to_index;
    obj.struct_content_search_obj.source_count_name = source_count_name;

    obj.operation_type = enum_recon_case_queue_content_search;

    list_struct_queue_2.enqueue(obj);

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Content_Search,MACRO_ACTIVITY_LOGS_TASK_QUEUED,search_label_name,date_time,"Content Search Queued...");

    emit signal_content_search_extraction_start(search_label_name);

    if(!bool_extraction_running_queue_2)
        start_extraction_queue_2();
}

void RECON_case::queued_operation_for_content_search(r_case_struct_queue_2 obj)
{
    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Content_Search,MACRO_ACTIVITY_LOGS_TASK_START,obj.struct_content_search_obj.label_name,date_time,"Content Search Started...");

    thread_content_search_obj->pub_set_bool_cancel_extraction();
    thread_content_search_obj->pub_set_dt_search_obj(dt_search_interface_obj);
    thread_content_search_obj->pub_set_search_label_name(obj.struct_content_search_obj.label_name);
    thread_content_search_obj->pub_set_bool_search_in_temp_mode(obj.struct_content_search_obj.bool_run_in_temp_mode);
    thread_content_search_obj->pub_set_dir_path_to_temp_index(obj.struct_content_search_obj.dir_path_to_temp_index);
    thread_content_search_obj->pub_set_source_count_name(obj.struct_content_search_obj.source_count_name);
    thread_content_search_obj->pub_set_keywords_list_to_search(obj.struct_content_search_obj.keywords_list);
    thread_content_search_obj->start();
}

void RECON_case::slot_thread_indexed_k_search_extraction_finished_or_cancelled()
{
    recon_static_functions::app_debug("search Label Name  " + thread_content_search_obj->pub_get_search_label_name(),Q_FUNC_INFO);

    emit signal_content_search_extraction_finished_or_cancelled(thread_content_search_obj->pub_get_search_label_name());

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Content_Search,MACRO_ACTIVITY_LOGS_TASK_COMPLETED,thread_content_search_obj->pub_get_search_label_name(),date_time,"Content Search Finished...");

    struct_global_task_type_and_title s1;
    s1.title = thread_content_search_obj->pub_get_search_label_name();
    s1.type = MACRO_JobType_Content_Search;
    emit signal_remove_sub_process_status_window_object(s1);
    finish_operation_queue_2();
}

void RECON_case::slot_cancel_content_search(struct_global_task_type_and_title t1)
{
    recon_static_functions::app_debug("Start ",Q_FUNC_INFO);

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_Content_Search,MACRO_ACTIVITY_LOGS_TASK_ABORTED,thread_content_search_obj->pub_get_search_label_name(),date_time,"Content Search Cancelled...");

    if(thread_content_search_obj->pub_get_search_label_name() == t1.title)
    {
        thread_content_search_obj->cancel_extraction();
    }

    emit signal_remove_sub_process_status_window_object(t1);

    interrupt_queued_operation(t1);
    is_all_task_and_queue_finish();

    recon_static_functions::app_debug("End ",Q_FUNC_INFO);
}
