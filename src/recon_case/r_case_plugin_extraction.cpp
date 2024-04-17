#include "recon_case.h"

void RECON_case::start_plugin_extractor_qobject()
{

    if(bool_plugin_extraction_qobject_running)
    {
        return;
    }

    recon_static_functions::app_debug("Starts - ", Q_FUNC_INFO);


    bool bool_should_plugin_run = false;
    for(int zz = 0; zz < sources_pending_in_queue_for_plugin_extraction.size(); zz++)
    {
        struct_GLOBAL_witness_info_source obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(sources_pending_in_queue_for_plugin_extraction.at(zz));
        if(obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal || obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
            continue;

        bool_should_plugin_run = true;
        break;
    }
    if(!bool_should_plugin_run)
        return;


    bool_plugin_extraction_qobject_running = true;

    QStringList final_plugin_list;
    for(int ii = 0; ii < selected_plugins_info_list.size(); ii++)
    {
        final_plugin_list << selected_plugins_info_list.at(ii).plugin_name.trimmed();
    }
    emit signal_remove_plugin_entry_from_display_tree(final_plugin_list);


    qobject_run_multiple_plugins_obj = new qobject_plugin_extraction;

    qobject_run_multiple_plugins_obj->pub_set_plugin_and_tab_list_for_extraction(selected_plugins_info_list);
    qobject_run_multiple_plugins_obj->pub_set_ios_parser_map(map_ios_uid_backup_parser_obj);
    qobject_run_multiple_plugins_obj->set_run_plugin_triggered(bool_run_plugin_triggered);
    qobject_run_multiple_plugins_obj->pub_set_source_count_name_list_for_extraction(sources_pending_in_queue_for_plugin_extraction);

    thread_qobject_run_multiple_plugins = new QThread;
    connect(thread_qobject_run_multiple_plugins, SIGNAL(started()), qobject_run_multiple_plugins_obj, SLOT(slot_extract_plugins()));
    qobject_run_multiple_plugins_obj->moveToThread(thread_qobject_run_multiple_plugins);

    connect(qobject_run_multiple_plugins_obj, SIGNAL(finished()), thread_qobject_run_multiple_plugins, SLOT(quit()));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(finished()), thread_qobject_run_multiple_plugins, SLOT(deleteLater()));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(finished()), this, SLOT(slot_thread_extract_plugin_finished()));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(signal_PBAR_plugin_status_message(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_plugin_extraction_status_message(QString,QString)));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(signal_PBAR_plugin_extraction(QString,qint64,QString)), task_status_window_obj, SLOT(slot_PBAR_plugin_extraction_status(QString,qint64,QString)));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(signal_plugin_extraction_finished_or_cancelled(QString)), this, SLOT(slot_current_plugin_extraction_finished_or_cancelled(QString)));
    connect(qobject_run_multiple_plugins_obj, SIGNAL(signal_insert_entry_in_case_tree_for_redefined_result(QString)), this, SLOT(slot_insert_entry_in_case_tree_for_redefined_result(QString)),Qt::QueuedConnection);

    connect(task_status_window_obj, SIGNAL(signal_skip_triggered()), qobject_run_multiple_plugins_obj, SLOT(slot_skip_triggered()));
    connect(task_status_window_obj, SIGNAL(signal_skip_triggered()), this, SLOT(slot_task_status_window_skip_triggered()));

    if(selected_plugins_info_list.size() >=1)
        emit signal_PBAR_artifacts_loading_started(MACRO_JobType_Artifact_Loading);

    map_plugin_pointer_obj.clear();
    for(int plugin_count = 0; plugin_count < selected_plugins_info_list.size(); plugin_count++)
    {
        emit signal_PBAR_artifacts_loading_value(MACRO_JobType_Artifact_Loading ,QString::number(plugin_count + 1) + "/" + QString::number(selected_plugins_info_list.size()) ,false,MACRO_Generic_Progress_No_Records_int,-1,true);

        struct_global_selected_plugins_info plugin_info = selected_plugins_info_list.at(plugin_count);
        header_plugin_extraction_interface *plugin_obj  = find_plugin_ptr_by_pluginname(plugin_info.plugin_name);
        if(plugin_obj == NULL)
            continue;

        connect(plugin_obj , SIGNAL(signal_extracted_records_of_plugin_overall(qint64)) , qobject_run_multiple_plugins_obj ,SLOT (slot_extracted_records_from_plugin(qint64)), Qt::DirectConnection);
        map_plugin_pointer_obj.insert(plugin_info.plugin_name, plugin_obj);
    }

    if(selected_plugins_info_list.size() >=1)
        emit signal_PBAR_artifacts_loading_finished(MACRO_JobType_Artifact_Loading);

    qobject_run_multiple_plugins_obj->pub_set_plugin_pointer_obj(map_plugin_pointer_obj);

    thread_qobject_run_multiple_plugins->start();

    sources_pending_in_queue_for_plugin_extraction.clear();

    recon_static_functions::app_debug("Ends - ", Q_FUNC_INFO);
}


void RECON_case::slot_change_display_message_of_task_progress_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress)
{
    emit signal_change_display_message_on_task_progress_bar(task_type, current_plugin_name, message, bool_show_close_button, max_size_of_progress);
}


header_plugin_extraction_interface *RECON_case::find_plugin_ptr_by_pluginname(QString plugin_name)
{
    QString lib_path = QString("../Artifacts_Libs/libm_") + plugin_name.replace(" ","_").toLower() + ".1.0.0.dylib";
    QFileInfo info(lib_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file not exist 111 ---FAILED---- " + lib_path,Q_FUNC_INFO);
        return NULL;
    }

    QString new_lib_path = recon_static_functions::decrypt_file_1(lib_path, Q_FUNC_INFO);
    QFileInfo info1(new_lib_path);
    if(!info1.exists())
    {
        recon_static_functions::app_debug(" file not exist 222 ---FAILED---- " + new_lib_path,Q_FUNC_INFO);
        return NULL;
    }


    QLibrary module_lib(new_lib_path);
   // QCoreApplication::processEvents();

    header_plugin_extraction_interface::entry_point_plugin_exraction entry_point_func = (header_plugin_extraction_interface::entry_point_plugin_exraction)module_lib.resolve("create_object");

    if(entry_point_func == NULL)
    {
        QFile file1(new_lib_path);
        file1.remove();
        recon_static_functions::app_debug(" Pointer entry_point_func NULL  create_object ---FAILED---- " + plugin_name,Q_FUNC_INFO);
        return NULL;
    }

    header_plugin_extraction_interface *ptr = (header_plugin_extraction_interface *)(entry_point_func());

    if(!ptr)
    {
        QFile file1(new_lib_path);
        file1.remove();
        recon_static_functions::app_debug(" Pointer NULL recon_module_interface ---FAILED---- " + plugin_name,Q_FUNC_INFO);
        return NULL;
    }

    QFile file1(new_lib_path);
    file1.remove();

    return ptr;
}


void RECON_case::add_source_in_list_others_folder(QString virtual_source_path, QString target_os_version, QStringList target_users_list)
{
    bool_run_plugin_triggered = false;
}

void RECON_case::slot_insert_entry_in_case_tree_for_redefined_result(QString category_name)
{
    emit signal_insert_redefined_result_entry_in_case_tree(category_name);
}
