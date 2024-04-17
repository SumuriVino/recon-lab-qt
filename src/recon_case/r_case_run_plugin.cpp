#include "recon_case.h"

void RECON_case::slot_right_click_run_plugins_clicked()
{
    common_right_click_run_plugins_clicked();
}
void RECON_case::common_right_click_run_plugins_clicked()
{
    recon_static_functions::app_debug("common_right_click_run_plugins_clicked ",Q_FUNC_INFO);



    if(bool_plugin_extraction_qobject_running)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Extracting Artifacts' is already running.");

        return;
    }



    run_plugin_obj = new run_plugin(this);
    run_plugin_obj->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(run_plugin_obj,SIGNAL(signal_plugin_list_from_run_plugin()),this,SLOT(slot_pluginlist_from_run_plugin()));


    run_plugin_obj->start_work();


    run_plugin_obj->show();

}

void RECON_case::slot_pluginlist_from_run_plugin()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    run_plugin_obj->close();

    bool_run_plugin_triggered = true;

    save_selected_plugin_tab_info_in_db();

    make_struct_plugin_extraction_list_for_run_plugin();

    sources_pending_in_queue_for_plugin_extraction.clear();

    QList<struct_GLOBAL_witness_info_source> mobj = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int pp = 0; pp < mobj.size(); pp++)
        sources_pending_in_queue_for_plugin_extraction << mobj.at(pp).source_count_name;

    start_plugin_extractor_qobject();


    bool_run_plugin_triggered = false;

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

    return;
}


void RECON_case::make_struct_plugin_extraction_list_for_run_plugin()
{
    selected_plugins_info_list.clear();

    QStringList m_list_plugins = global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_QStringList).toStringList();

    for(int tt = 0; tt < m_list_plugins.size(); tt++)
    {
        QString plg_nm = m_list_plugins.at(tt);
        QStringList tab_list = recon_static_functions::get_associated_tabs(plg_nm,global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList).toStringList(), Q_FUNC_INFO);


        struct_global_selected_plugins_info s1;
        s1.plugin_name = plg_nm;
        s1.selected_plugin_tab_names = tab_list;
        s1.extraction_completed = false;
        s1.plugin_selected_table_list = global_csv_reader_class_selected_obj->get_tables_name_according_tabs(plg_nm,tab_list);
        s1.total_table_list = global_csv_reader_class_full_obj->get_total_tables_list(plg_nm);
        s1.supported_os_schemes_list = global_csv_reader_class_selected_obj->get_supported_os_scheme_list_of_plugin(plg_nm);

        selected_plugins_info_list.append(s1);

    }

}

void RECON_case::make_struct_plugin_extraction_list_for_first_and_add_source()
{

    recon_static_functions::app_debug(" start",Q_FUNC_INFO);


    selected_plugins_info_list.clear();

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";


    QString cmd_plg = ("select Distinct plugin_name from tbl_plugin_status");
    QStringList plgn_list =  recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_plg, 0, db_path, Q_FUNC_INFO);


    for(int mm = 0; mm < plgn_list.size(); mm++)
    {
        QString plugin_name = plgn_list.at(mm);
        QString cmd_tab = ("select tab_name from tbl_plugin_status where plugin_name = ?");
        QStringList tabs_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd_tab,QStringList(plugin_name), 0, db_path, Q_FUNC_INFO);

        struct_global_selected_plugins_info s1;
        s1.plugin_name = plugin_name;
        s1.selected_plugin_tab_names = tabs_list;
        s1.extraction_completed = false;
        s1.plugin_selected_table_list = global_csv_reader_class_selected_obj->get_tables_name_according_tabs(plugin_name,tabs_list);
        s1.total_table_list = global_csv_reader_class_full_obj->get_total_tables_list(plugin_name);
        s1.supported_os_schemes_list = global_csv_reader_class_selected_obj->get_supported_os_scheme_list_of_plugin(plugin_name);

        selected_plugins_info_list.append(s1);



    }



    return;

}

void RECON_case::slot_cancel_plugin_extraction(struct struct_global_task_type_and_title t1)
{
    recon_static_functions::app_debug("pslot_cancel_plugin_extraction] Start, plugin - " + t1.title,Q_FUNC_INFO);


    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_plugin_list->tryAcquire(1))
            break;
        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    if(current_plugin_being_extracted == t1.title)
    {

        bool_cancel_plugin_extraction_click = true;

        qint64 date_time = QDateTime::currentMSecsSinceEpoch();
        emit signal_activity_logs_update_status(MACRO_JobType_Artifacts,MACRO_ACTIVITY_LOGS_TASK_ABORTED,t1.title,date_time,"Plugin Cancelled...");

        return;
    }


    for(int i = 0 ; i < selected_plugins_info_list.size(); i++)
    {
        struct_global_selected_plugins_info p1 = selected_plugins_info_list.at(i);

        //        if(p1.plugin_name == t1.title && p1.extraction_completed == false)
        //        {
        //            selected_plugins_info_list.removeAt(i);
        //            break;
        //        }

        if(p1.plugin_name == t1.title && p1.extraction_completed == false)
        {
            qint64 date_time = QDateTime::currentMSecsSinceEpoch();
            emit signal_activity_logs_update_status(MACRO_JobType_Artifacts,MACRO_ACTIVITY_LOGS_TASK_ABORTED,t1.title,date_time,"Plugin Cancelled...");

            selected_plugins_info_list.removeAt(i);
            break;
        }

    }

    interrupt_queued_operation(t1);

    semaphore_plugin_list->release(1);
    recon_static_functions::app_debug(" end, plugin - " + t1.title,Q_FUNC_INFO);
}



void RECON_case::plugin_null_ptr_found()
{
    struct struct_global_task_type_and_title t1;
    t1.title = current_plugin_being_extracted;
    t1.type = MACRO_JobType_Artifacts;
    emit signal_remove_sub_process_status_window_object(t1);

    for(int i = 0 ; i < selected_plugins_info_list.size(); i++)
    {
        struct_global_selected_plugins_info p1 = selected_plugins_info_list.at(i);

        if(p1.plugin_name == t1.title && p1.extraction_completed == false)
        {
            selected_plugins_info_list.removeAt(i);
            break;
        }
    }

}


void RECON_case::slot_extracted_records_from_plugin(qint64 total_records_extracted)
{
    emit signal_extracted_records_by_plugin(total_records_extracted);
}

