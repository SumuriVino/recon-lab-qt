#include "recon_case.h"

void RECON_case::slot_thread_file_system_finished()
{

    if(process_tsk_fs->state() == QProcess::Running)
        process_tsk_fs->terminate();

    if(process_tsk_fs != NULL && process_tsk_fs->state() == QProcess::NotRunning)
        delete process_tsk_fs;


    QString case_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString attributes_command_str = QString("Select metadata_attribute_name From tbl_apple_attributes");
    apple_all_attribute_name_list.clear();
    apple_all_attribute_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(attributes_command_str, 0 ,case_config_db_path , Q_FUNC_INFO);

    set_apple_metadata_reader_file_system();

    start_apple_timestamps_extraction();
    start_mime_type_extraction();
    start_mail_pst_extraction();
    start_mail_emlx_extraction();
    start_mail_mbox_extraction();

    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_ADD_SOURCE)
    {
        //We dont allow to run FS module at time of Adding new source.
        //So, we have return after completion of file system extraction.
        recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
        int count = 0;
        while(1)
        {
            count++;
            if(count % 50 == 0)
            {
                count = 0;
                QCoreApplication::processEvents();
            }

            if(!bool_thread_file_system_running && !bool_thread_mime_type_running)
            {
                emit signal_filesystem_extraction_finished();
                break;
            }
        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);
        return;
    }

    start_apple_metadata_extraction();
    start_signature_extraction();
    pub_start_hashes_data_extraction();
    start_text_indexing_extraction();
    start_unified_logs_extraction();


    //After Mime Type completion Below process will run.
    //This load case check is used to maintain the Symmetry like other process above and to add the result of Skin Tone detection in case tree.

    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
    {
        start_exif_data_extraction();
        start_face_analysis_extraction();
        start_optical_character_recognition_extraction();
        start_skin_tone_detection_extraction();
        start_weapons_extraction();
        start_fire_analysis_extraction();
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int count = 0;
    while(1)
    {
        count++;
        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }
        if(!bool_thread_file_system_running && !bool_thread_mime_type_running && !bool_thread_apple_metadata_running
                && !bool_thread_signature_running && !bool_thread_exif_data_running && !bool_thread_hashes_running
                && !bool_thread_text_indexing_running && !bool_thread_face_analysis_running && !bool_thread_optical_character_recognition_running
                && !bool_thread_skin_tone_detection_running && !bool_thread_weapons_running && !bool_thread_fire_analysis_running
                && !bool_thread_pst_mail_running && !bool_thread_mail_emlx_files_running && !bool_thread_mbox_mail_running
                && !bool_thread_unified_logs_running)
        {
            emit signal_filesystem_extraction_finished();
            break;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);
}

void RECON_case::slot_tsk_fs_process_data()
{
    QString data_str = QString::fromLocal8Bit(process_tsk_fs->readAllStandardError()).trimmed();

    emit signal_process_tsk_fs_custom_status(data_str);

}

void RECON_case::slot_tsk_fs_finished(int exit_code)
{

    if(exit_code != 0)
    {
        emit signal_process_tsk_fs_custom_status(QString(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Process_Finished_With_Crash));
    }

}

void RECON_case::slot_thread_apple_timestamp_finished()
{
    bool_thread_file_system_running = false;
}

void RECON_case::file_system_extraction_initiator()
{
    if(bool_thread_file_system_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Filesystem already running.");
        return;
    }

    recon_static_functions::app_debug(" start", Q_FUNC_INFO);


    process_tsk_fs   = new QProcess(this);
    process_tsk_fs->setReadChannel(QProcess::StandardOutput);
    connect(process_tsk_fs, SIGNAL(readyReadStandardError()), this, SLOT(slot_tsk_fs_process_data()));
    connect(process_tsk_fs, SIGNAL(finished(int)), this, SLOT(slot_tsk_fs_finished(int)));

    QProcess *process_for_thread   = new QProcess(this);


    thread_file_system_obj = new thread_file_system(this);
    connect(thread_file_system_obj,SIGNAL(signal_thread_file_system_finished()),this,SLOT(slot_thread_file_system_finished()));
    connect(thread_file_system_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_file_system_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_file_system_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_file_system_obj, SIGNAL(signal_add_file_basic_to_case(QString)), this, SLOT(slot_add_file_basic_to_case_tree(QString)));
    connect(thread_file_system_obj, SIGNAL(signal_add_internal_sources_to_case(QString)), this, SLOT(slot_add_internal_sources_to_case(QString)));
    connect(thread_file_system_obj, SIGNAL(signal_add_registry_files(QStringList , QString)), this, SLOT(slot_add_registry_files(QStringList , QString)));
    connect(this, SIGNAL(signal_registry_files_extraction_work_finished()), thread_file_system_obj, SLOT(slot_registry_files_extraction_work_finished()));
    connect(thread_file_system_obj, SIGNAL(signal_all_ios_sources_extraction_finished()), this, SLOT(slot_all_ios_sources_extraction_finished()));
    connect(this, SIGNAL(signal_process_tsk_fs_custom_status(QString)), thread_file_system_obj, SLOT(slot_process_tsk_fs_custom_status(QString)));
    connect(thread_file_system_obj, SIGNAL(signal_source_extraction_completed_only_for_basic_file_system(QString)), this, SLOT(slot_source_extraction_completed_only_for_basic_file_system(QString)));
    connect(this, SIGNAL(signal_add_internal_sources_to_case_from_rcase_finished()), thread_file_system_obj, SLOT(slot_add_internal_sources_to_case_finished()));
    connect(this, SIGNAL(signal_add_file_basic_to_case_from_rcase_finished()), thread_file_system_obj, SLOT(slot_add_file_basic_to_case_finished()));

    thread_file_system_obj->pub_set_tsk_fs_process(process_tsk_fs);
    thread_file_system_obj->pub_set_general_process(process_for_thread);
    thread_file_system_obj->set_bool_cancel_extraction();

    thread_file_system_obj->set_case_treewidget_obj(treewidget_case_display_obj);
    thread_file_system_obj->pub_set_map_ios_uid_backup_parser_obj(map_ios_uid_backup_parser_obj);
    thread_file_system_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_file_system_obj->pub_set_FS_source_selected_source_count_name_list(source_selected_source_count_name_list);
    bool_thread_file_system_running = true;
    thread_file_system_obj->pub_set_essential(); // it will always be just before start
    thread_file_system_obj->start();

    sources_pending_in_queue_for_plugin_extraction << source_selected_source_count_name_list;

    recon_static_functions::app_debug(" end", Q_FUNC_INFO);

}

void RECON_case::clear_case_tree_children(int parent_position)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item_parent = treewidget_case_display_obj->topLevelItem(ii)->child(parent_position);

        if(item_parent == NULL)
            continue;

        while(item_parent->childCount())
        {
            item_parent->removeChild(item_parent->child(0));
        }
    }

}

void RECON_case::clear_case_tree_grand_children(int parent_position, QString feature_name)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item_grand_parent = treewidget_case_display_obj->topLevelItem(ii)->child(parent_position);

        if(item_grand_parent == NULL)
            continue;

        for(int j = 0; j < item_grand_parent->childCount(); j++)
        {
            QTreeWidgetItem *item_parent = item_grand_parent->child(j);
            if(item_parent->text(0).trimmed() == feature_name)
            {
                while(item_parent->childCount())
                    item_parent->removeChild(item_parent->child(0));

            }
        }

    }
}

void RECON_case::slot_recon_config_apply_button_clicked()
{
    display_loading_non_cancelable_progress_bar_obj->pub_set_label_messsge("Please Wait ...");
    display_loading_non_cancelable_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_non_cancelable_progress_bar_obj->show();

    QList<struct_GLOBAL_witness_info_source> source_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    clear_case_tree_children(enum_global_case_tree_display_row_APPLE_METADATA);
    clear_case_tree_children(enum_global_case_tree_display_row_EXIF_METADATA);
    clear_case_tree_children(enum_global_case_tree_display_row_EXTENSIONS);
    clear_case_tree_children(enum_global_case_tree_display_row_HASHSET);
    clear_case_tree_grand_children(enum_global_case_tree_display_row_MIME_TYPES, MACRO_CASE_TREE_MIME_Type);

    recon_helper_standard_obj->update_narad_variable_with_apple_metadata_info(Q_FUNC_INFO);
    recon_helper_standard_obj->update_narad_variable_with_exif_metadata_info(Q_FUNC_INFO);


    /// thread will run if new attributes are added.
    bool bool_need_to_run_apple_metadata_thread = false;

    QString config_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString attributes_command_str = QString("Select metadata_attribute_name From tbl_apple_attributes Where bookmark_filter = '1' or bookmark_report = '1'");
    QStringList attributes_list_latest = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(attributes_command_str, 0 ,config_db_path , Q_FUNC_INFO);

    QString cmd_last_processed_attrs = QString("Select attribute_name From processed_attributes");

    QStringList apple_metadata_db_path_list;
    QStringList exif_metadata_db_path_list;

    for(int i = 0; i < source_list.size(); i++)
    {
        QString source_count_name = source_list.at(i).source_count_name;
        QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/apple_metadata.sqlite";
        apple_metadata_db_path_list << apple_metadata_db_path;

        if(!bool_need_to_run_apple_metadata_thread)
        {
            QStringList attributes_list_last_processed = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_last_processed_attrs, 0 ,apple_metadata_db_path , Q_FUNC_INFO);
            for(int count = 0; count < attributes_list_latest.size(); count++)
            {
                if(!attributes_list_last_processed.contains(attributes_list_latest.at(count)))
                {
                    bool_need_to_run_apple_metadata_thread = true;
                    break;
                }
            }
        }

        QString exif_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/exif_metadata.sqlite";
        exif_metadata_db_path_list << exif_metadata_db_path;


        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
        add_file_extension_by_user_defined_extension_in_case_tree(fs_db_path);


        QString hash_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/hashes.sqlite";
        slot_add_matched_hashes_to_case_tree(hash_db_path, source_count_name);
    }

    if(bool_need_to_run_apple_metadata_thread)
    {
        start_apple_metadata_extraction();
    }
    else
    {
        slot_add_apple_metadata_to_case(apple_metadata_db_path_list);
    }

    slot_add_exif_data_to_case_tree(exif_metadata_db_path_list);

    display_loading_non_cancelable_progress_bar_obj->hide();

}


void RECON_case::slot_job_type_completion_status(QString job_type, QString source_count_name, QString status)
{
    QString column_name = QString(job_type).replace(" ", "_");

    QString command = "update fs_status set " + column_name + "=" + status + " where source_count_name = ?";


    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";


    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, QStringList(source_count_name) , destination_db_path, Q_FUNC_INFO);

}


void RECON_case::slot_remove_entry_for_file_types_based_in_main_stack(QString parent_name, QString child_name)
{
    emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(parent_name, child_name);
}

void RECON_case::pub_start_hashes_data_extraction()
{
    if(bool_thread_hashes_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Hashset already running.");
        return;
    }

    thread_hashes_obj = new thread_hashes;

    thread_hashes_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_hashes_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_hashes_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_hashes);


    thread_hashes_data_extract = new QThread;
    thread_hashes_obj->moveToThread(thread_hashes_data_extract);

    connect(thread_hashes_data_extract, SIGNAL(started()), thread_hashes_obj, SLOT(slot_hashset_extraction()));
    connect(thread_hashes_obj, SIGNAL(finished()), thread_hashes_data_extract, SLOT(quit()));
    connect(thread_hashes_obj, SIGNAL(finished()), this, SLOT(slot_hashes_thread_finish()));
    connect(thread_hashes_obj, SIGNAL(signal_add_matched_hashes_to_case_tree(QString,QString)), this, SLOT(slot_add_matched_hashes_to_case_tree(QString,QString)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);
    connect(thread_hashes_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);
    connect(thread_hashes_obj, SIGNAL(signal_add_all_available_hashes_to_case_tree()), this, SIGNAL(signal_r_case_add_all_available_hashes_to_case_tree()));

    bool_thread_hashes_running = true;

    thread_hashes_data_extract->start();
}

void RECON_case::start_apple_metadata_extraction()
{
    if(bool_thread_apple_metadata_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Apple Metadata already running.");
        return;
    }

    thread_apple_metadata_obj = new thread_apple_metadata;

    process_apple_metadata_full_source = new QProcess(this);
    process_apple_metadata_full_source->setProcessChannelMode(QProcess::MergedChannels);
    connect(process_apple_metadata_full_source, SIGNAL(readyRead()), thread_apple_metadata_obj, SLOT(slot_prc_apple_metadata_reader_for_full_source_readyread()));

    thread_apple_metadata_obj->pub_set_essentials();
    thread_apple_metadata_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_apple_metadata_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_apple_metadata_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_apple_metadata);
    thread_apple_metadata_obj->pub_set_apple_metadata_all_attributes_name_list(apple_all_attribute_name_list);

    thread_apple_metadata_obj->pub_set_apple_metadata_reader_process_for_full_source(process_apple_metadata_full_source);
    thread_extended_attr_extract = new QThread;

    thread_apple_metadata_obj->moveToThread(thread_extended_attr_extract);


    connect(thread_extended_attr_extract, SIGNAL(started()), thread_apple_metadata_obj, SLOT(slot_extract_apple_metadata()));
    connect(thread_apple_metadata_obj, SIGNAL(finished()), thread_extended_attr_extract, SLOT(quit()));
    connect(thread_apple_metadata_obj, SIGNAL(finished()), this, SLOT(slot_apple_metadata_thread_finish()));
    connect(thread_apple_metadata_obj, SIGNAL(signal_add_apple_metadata_to_case(QStringList)), this, SLOT(slot_add_apple_metadata_to_case(QStringList)));
    connect(thread_apple_metadata_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_apple_metadata_running = true;
    thread_extended_attr_extract->start();
}


void RECON_case::save_selected_plugin_tab_info_in_db()
{

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    QStringList m_list_plugins = global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_QStringList).toStringList();

    for(int tt = 0; tt < m_list_plugins.size(); tt++)
    {
        QString plg_nm = m_list_plugins.at(tt);
        QStringList tab_list = recon_static_functions::get_associated_tabs(plg_nm,global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList).toStringList(), Q_FUNC_INFO);

        for(int pp = 0; pp < tab_list.size(); pp++)
        {

            QString tab_name = tab_list.at(pp);
            QString command_check = "select tab_name from tbl_plugin_status where plugin_name = '" + plg_nm + "'";
            if(recon_helper_standard_obj->check_value_existence_in_db_by_dbpath(command_check,0,(tab_name),db_path,Q_FUNC_INFO))
            {
                continue;
            }



            QString command_tabs = QString("insert into tbl_plugin_status(plugin_name,tab_name,run_status) values(?,?,?)");

            QStringList args;
            args << plg_nm << tab_list.at(pp) << "1";

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_tabs,args, db_path,Q_FUNC_INFO);

        }

    }




}

void RECON_case::start_mime_type_extraction()
{
    thread_mime_type_obj = new thread_mime_type;

    thread_mime_type_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_mime_type_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_mime_type_obj->pub_set_job_selected_source_count_name_list(source_selected_source_count_name_list);

    thread_mime_extract = new QThread;
    thread_mime_type_obj->moveToThread(thread_mime_extract);

    connect(thread_mime_extract, SIGNAL(started()), thread_mime_type_obj, SLOT(slot_extract_mime_type()));
    connect(thread_mime_type_obj, SIGNAL(finished()), thread_mime_extract, SLOT(quit()));
    connect(thread_mime_type_obj, SIGNAL(finished()), this, SLOT(slot_mime_type_thread_finish()));
    connect(thread_mime_type_obj, SIGNAL(signal_add_mime_types_to_case_tree(QStringList)), this, SLOT(slot_add_mime_type_to_case_tree(QStringList)));
    connect(thread_mime_type_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);
    connect(thread_mime_type_obj, SIGNAL(signal_mime_type_extracted_source(QString)), this, SLOT(slot_source_extraction_mime_type_completed(QString)));

    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    //    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_mime_type_running = true;
    thread_mime_extract->start();

}

void RECON_case::start_exif_data_extraction()
{
    if(bool_thread_exif_data_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Exif metadata already running.");
        return;
    }

    thread_exif_metadata_obj = new thread_exif_metadata;

    thread_exif_metadata_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_exif_metadata_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_exif_metadata_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_exif_metadata);

    thread_exif_data_extract = new QThread;
    thread_exif_metadata_obj->moveToThread(thread_exif_data_extract);

    connect(thread_exif_data_extract, SIGNAL(started()), thread_exif_metadata_obj, SLOT(slot_extract_exif_data()));
    connect(thread_exif_metadata_obj, SIGNAL(finished()), thread_exif_data_extract, SLOT(quit()));
    connect(thread_exif_metadata_obj, SIGNAL(finished()), this, SLOT(slot_exif_metadata_thread_finish()));
    connect(thread_exif_metadata_obj, SIGNAL(signal_add_exif_to_case_tree(QStringList)), this, SLOT(slot_add_exif_data_to_case_tree(QStringList)));
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);
    connect(thread_exif_metadata_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_exif_data_running = true;
    thread_exif_data_extract->start();
}

void RECON_case::start_text_indexing_extraction()
{
    if(bool_thread_text_indexing_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Text indexing already running.");
        return;
    }

    thread_index_files_obj = new thread_index_files;

    thread_index_files_obj->pub_set_dt_serach_interface_obj(dt_search_interface_obj);
    thread_index_files_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    //thread_index_files_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_text_indexing);
    thread_index_files_obj->set_recon_case_runner_type(recon_case_runner_type);

    thread_dt_search_extract = new QThread;
    thread_index_files_obj->moveToThread(thread_dt_search_extract);

    connect(thread_dt_search_extract, SIGNAL(started()), thread_index_files_obj, SLOT(slot_extract_dt_search_data()));
    connect(thread_index_files_obj, SIGNAL(finished()), thread_dt_search_extract, SLOT(quit()));
    connect(thread_index_files_obj, SIGNAL(finished()), this, SLOT(slot_dt_search_thread_finish()));
    connect(thread_index_files_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_index_files_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_index_files_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_index_files_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_text_indexing_running = true;
    thread_dt_search_extract->start();
}

void RECON_case::pub_start_file_indexing_from_toolbar()
{

    if(bool_thread_text_indexing_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Text indexing already running.");
        return;
    }

    thread_index_files_obj = new thread_index_files;

    thread_index_files_obj->pub_set_dt_serach_interface_obj(dt_search_interface_obj);
    thread_index_files_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    //thread_index_files_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_text_indexing);
    thread_index_files_obj->set_recon_case_runner_type(recon_case_runner_type);

    thread_index_files_obj->pub_set_is_running_from_toolbar(true);

    thread_dt_search_extract = new QThread;
    thread_index_files_obj->moveToThread(thread_dt_search_extract);

    connect(thread_dt_search_extract, SIGNAL(started()), thread_index_files_obj, SLOT(slot_extract_dt_search_data()));
    connect(thread_index_files_obj, SIGNAL(finished()), thread_dt_search_extract, SLOT(quit()));
    connect(thread_index_files_obj, SIGNAL(finished()), this, SLOT(slot_dt_search_thread_finish()));
    connect(thread_index_files_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_index_files_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_index_files_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_index_files_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_text_indexing_running = true;
    task_status_window_obj->show();
    thread_dt_search_extract->start();
}

bool RECON_case::pub_is_file_system_thread_running()
{
    return bool_thread_file_system_running;
}

bool RECON_case::pub_is_artifacts_thread_running()
{
    return bool_plugin_extraction_qobject_running;
}

bool RECON_case::pub_is_text_indexing_thread_running()
{
    return bool_thread_text_indexing_running;
}

void RECON_case::pub_stop_all_running_threads()
{
    /// Cancel Apple Metadat thread
    if(bool_thread_apple_metadata_running)
    {
        thread_apple_metadata_obj->pub_set_bool_cancel_extraction(true);
        if(thread_extended_attr_extract != nullptr)
        {
            if(thread_extended_attr_extract->isRunning())
                thread_extended_attr_extract->quit();
        }
        while(1)
        {
            if(bool_thread_apple_metadata_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }

    /// Cancel mime type thread
    if(bool_thread_mime_type_running)
    {
        thread_mime_type_obj->pub_set_bool_cancel_extraction(true);
        while(1)
        {
            if(bool_thread_mime_type_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }

    /// Cancel Exif Thread
    if(bool_thread_exif_data_running)
    {
        thread_exif_metadata_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_exif_data_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }

    /// Cancel Face Extractrion
    if(bool_thread_face_analysis_running)
    {
        thread_face_analysis_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_face_analysis_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }

    }

    /// Cancel OPTICAL CHARACTER RECOGNITION Extractrion
    if(bool_thread_optical_character_recognition_running)
    {
        thread_optical_character_recognition_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_optical_character_recognition_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }

    }

    ///Cancel Skin Tone Detection
    if(bool_thread_skin_tone_detection_running)
    {
        thread_skin_tone_detection_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_skin_tone_detection_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }

    }

    ///Cancel Weapons Extraction
    if(bool_thread_weapons_running)
    {
        thread_weapons_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_weapons_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }

    }
    ///Cancel fire analysis Extraction

    if(bool_thread_fire_analysis_running)
    {
        thread_fire_analysis_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_fire_analysis_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }

    }

    /// Cancel Plugin extraction thread
    if(bool_plugin_extraction_qobject_running)
    {
        qobject_run_multiple_plugins_obj->pub_cancel_everything();

        while(1)
        {
            if(bool_plugin_extraction_qobject_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }
    ///Cancel Email Pst Extraction
    if(bool_thread_pst_mail_running)
    {

        while(1)
        {
            if(bool_thread_pst_mail_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }

    }

    ///Cancel Email Emlx Extraction
    if(bool_thread_mail_emlx_files_running)
    {

        while(1)
        {
            if(bool_thread_mail_emlx_files_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }

    ///Cancel Email Mbox Extraction
    if(bool_thread_mbox_mail_running)
    {

        while(1)
        {
            if(bool_thread_mbox_mail_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }

    ///Cancel Unified Logs Extraction
    if(bool_thread_unified_logs_running)
    {
        thread_unified_logs_obj->pub_set_bool_cancel_extraction(true);

        while(1)
        {
            if(bool_thread_unified_logs_running)
            {
                QCoreApplication::processEvents();
            }
            else
                break;
        }
    }

}

void RECON_case::pub_set_file_system_thread_not_running()
{
    bool_thread_file_system_running = false;
}

void RECON_case::slot_add_apple_metadata_to_case(QStringList db_path_list)
{
    if(!db_path_list.isEmpty())
    {
        //    add_metadata_by_apple_metadata_in_case_tree(db_path);
        add_apple_metadata_in_case_tree(db_path_list);
        // add_locations_apple_meta_in_case_tree(db_path_list);

    }

    add_redefined_result_locations_for_apple_meta_and_exif_in_case_tree();

}

void RECON_case::slot_source_extraction_completed_only_for_basic_file_system(QString source_count_nm)
{
    extracted_source_count_name_list.append(source_count_nm);
    extracted_source_count_name_list.removeDuplicates();

    //pub_set_extracted_source_count_list(QStringList(source_count_nm));
}

void RECON_case::slot_source_extraction_mime_type_completed(QString source_count_nm)
{
    extracted_mime_type_source_count_name_list.append(source_count_nm);
    extracted_mime_type_source_count_name_list.removeDuplicates();
}

void RECON_case::slot_all_ios_sources_extraction_finished()
{
    if(!bool_plugin_extraction_qobject_running)
    {
        start_plugin_extractor_qobject();
    }
}

void RECON_case::slot_add_registry_files(QStringList all_reg_files_path_list ,  QString source_count_name)
{
    parse_registry_files_list(all_reg_files_path_list ,source_count_name);

    emit signal_registry_files_extraction_work_finished();
}

void RECON_case::slot_add_internal_sources_to_case(QString source_cnt_name)
{
    add_mobile_backup_info_in_case_tree(source_cnt_name);
    add_disk_images_info_in_case_tree(source_cnt_name);
    add_window_ram_images_info_in_case_tree(source_cnt_name);

    // emit at last position
    emit signal_add_internal_sources_to_case_from_rcase_finished();
}

void RECON_case::slot_add_file_basic_to_case_tree(QString db_path)
{
    add_file_size_in_case_tree(db_path);
    add_file_extension_by_user_defined_extension_in_case_tree(db_path);

    // emit at last position
    emit signal_add_file_basic_to_case_from_rcase_finished();

}


void RECON_case::start_unified_logs_extraction()
{

    if(bool_thread_unified_logs_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Unified Logs already running.");
        return;
    }

    thread_unified_logs_obj = new thread_unified_logs;

    thread_unified_logs_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_unified_logs_obj->pub_set_recon_case_runner_type(recon_case_runner_type);
    thread_unified_logs_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_unified_logs);

    thread_unified_logs_extraction = new QThread;
    thread_unified_logs_obj->moveToThread(thread_unified_logs_extraction);

    connect(thread_unified_logs_extraction, SIGNAL(started()),thread_unified_logs_obj,SLOT(slot_extract_unified_logs()));
    connect(thread_unified_logs_obj,SIGNAL(finished()),thread_unified_logs_extraction,SLOT(quit()));
    connect(thread_unified_logs_obj, SIGNAL(finished()),this,SLOT(slot_unified_logs_thread_finish()));

    connect(thread_unified_logs_obj, SIGNAL(signal_add_unified_logs_to_case_tree(QString)),this,SLOT(slot_add_unified_log_file_to_case_tree(QString)));
    connect(thread_unified_logs_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)),task_status_window_obj,SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_unified_logs_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)),task_status_window_obj,SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_unified_logs_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)),task_status_window_obj,SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_unified_logs_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_unified_logs_running = true;
    thread_unified_logs_extraction->start();

}

void RECON_case::slot_unified_logs_thread_finish()
{
    bool_thread_unified_logs_running = false;

    if(thread_unified_logs_extraction->isRunning())
        thread_unified_logs_extraction->quit();

    if(thread_unified_logs_extraction != NULL && !thread_unified_logs_extraction->isRunning())
        delete thread_unified_logs_extraction;


    if(thread_unified_logs_obj != NULL)
        delete thread_unified_logs_obj;


    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();
}

void RECON_case::slot_add_unified_log_file_to_case_tree(QString source_count_name)
{
    add_unified_logs_file_in_case_tree(source_count_name);
}

void RECON_case::add_unified_logs_file_in_case_tree(QString source_count_name)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    QString index_unified_logs = unified_logs_dir_path + "/index_unified_logs.sqlite";

    QString cmd = "Select db_name from tbl_index where source_count_name = '" + source_count_name + "'";
    QString db_available_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd,0,index_unified_logs,Q_FUNC_INFO);

    if(db_available_name.isEmpty())
        return;

    QString logs_db_path = unified_logs_dir_path + db_available_name;

    QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) from log_data") , 0, logs_db_path, Q_FUNC_INFO);
    qint64 total_records_num = total_records_str.toLongLong();

    if(total_records_num <= 0)
        return;

    QStringList log_file_list;
    log_file_list << "log";

    struct_GLOBAL_witness_info_source struct_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_UNIFIED_LOGS);

        if(item->text(0) == QString(MACRO_CASE_TREE_Unified_Logs))
        {
            for(int num = 0; num < log_file_list.size(); num++)
            {
                QString child_name_str = struct_src_info.source_name + "." + log_file_list.at(num) + " (" + total_records_str +  ")"+ " " + "[" + source_count_name + "]";
                QTreeWidgetItem *child_item = new QTreeWidgetItem;
                child_item->setText(0, child_name_str);
                child_item->setText(1, QString(MACRO_CASE_TREE_Unified_Logs));

                item->addChild(child_item);
            }
            break;
            item->setExpanded(false);

        }

        item->setExpanded(false);
        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::slot_add_exif_data_to_case_tree(QStringList db_path_list)
{
    if(!db_path_list.isEmpty())
    {
        add_exif_metadata_in_case_tree(db_path_list);
        // add_locations_exif_in_case_tree(db_path_list);

    }

    add_redefined_result_locations_for_apple_meta_and_exif_in_case_tree();
}

void RECON_case::slot_add_matched_hashes_to_case_tree(QString hashes_db_path, QString source_count)
{
    add_matched_hashes_in_case_tree(hashes_db_path, source_count);
}

void RECON_case::slot_PBAR_file_system_job_cancelled_requested(QString job_type)
{
    if(job_type == MACRO_JobType_Apple_Metadata)
    {
        thread_apple_metadata_obj->pub_set_bool_cancel_extraction(true);
        if(bool_thread_apple_metadata_running)
        {
            thread_apple_metadata_obj->pub_set_bool_cancel_extraction(true);
            if(thread_extended_attr_extract != nullptr)
            {
                if(thread_extended_attr_extract->isRunning())
                {
                    thread_extended_attr_extract->exit(0);
                    if(process_apple_metadata_full_source->state() == QProcess::Running)
                    {
                        process_apple_metadata_full_source->kill();

                    }
                }

                while(1)
                {
                    if(bool_thread_apple_metadata_running)
                    {
                        QCoreApplication::processEvents();
                    }
                    else
                        break;
                }

            }

        }
    }
    else if(job_type == MACRO_JobType_Exif_Metadata)
        thread_exif_metadata_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Mime_Type)
        thread_mime_type_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Signature_Analysis)
        thread_signature_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Hashes)
        thread_hashes_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Text_Indexing)
        thread_index_files_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Exif_Metadata_Search)
        emit signal_r_case_thread_exif_metadata_search_cancelled();
    else if(job_type == MACRO_JobType_Face_Analysis)
        thread_face_analysis_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Optical_Character_Recognition)
        thread_optical_character_recognition_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Skin_Tone_Detection)
        thread_skin_tone_detection_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Weapons)
        thread_weapons_obj->pub_set_bool_cancel_extraction(true);
    else if(job_type == MACRO_JobType_Fire_Analysis)
        thread_fire_analysis_obj->pub_set_bool_cancel_extraction(true);
}

void RECON_case::slot_task_status_window_skip_triggered()
{
    if(qobject_run_multiple_plugins_obj != NULL)
        qobject_run_multiple_plugins_obj->slot_skip_triggered();
}

void RECON_case::slot_current_plugin_extraction_finished_or_cancelled(QString plugin_name)
{
    emit signal_current_plugin_extraction_finished_or_cancelled(plugin_name);
}


void RECON_case::slot_add_all_signatures_to_case_tree(QStringList db_path_list)
{
    //emit signal_disable_case_tree_sidebar(true);

    if(!db_path_list.isEmpty())
    {

        add_file_signatures_in_case_tree(db_path_list);

    }

    //emit signal_disable_case_tree_sidebar(false);
}

void RECON_case::slot_add_mime_type_to_case_tree(QStringList db_path_list)
{
    if(!db_path_list.isEmpty())
    {
        add_mime_types_in_case_tree(db_path_list);
    }
}


void RECON_case::slot_apple_metadata_thread_finish()
{
    bool_thread_apple_metadata_running = false;

    if(thread_extended_attr_extract->isRunning())
        thread_extended_attr_extract->quit();

    if(thread_extended_attr_extract != NULL && !thread_extended_attr_extract->isRunning())
        delete thread_extended_attr_extract;

    if(thread_apple_metadata_obj != NULL)
        delete thread_apple_metadata_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();
}

void RECON_case::slot_mime_type_thread_finish()
{

    bool_thread_mime_type_running = false;

    if(thread_mime_extract->isRunning())
        thread_mime_extract->quit();


    if(thread_mime_extract != NULL && !thread_mime_extract->isRunning())
        delete thread_mime_extract;


    if(thread_mime_type_obj != NULL)
        delete thread_mime_type_obj;


    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();


    //Now we are hard bind to mime type extraction with Files System extraction
    //FS Modules based on Mime type will run after its completion.
    //At 'Add Source' time no need to rum mime type based FS modules as we have to run those modules by post launcher only.
    //So here we have check of first launcher runner type only.
    if((recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FIRST_LAUNCHER))
    {
        start_exif_data_extraction();
        start_face_analysis_extraction();
        start_optical_character_recognition_extraction();
        start_skin_tone_detection_extraction();
        start_weapons_extraction();
        start_fire_analysis_extraction();
    }


}

void RECON_case::slot_thread_extract_plugin_finished()
{

    QMapIterator<QString, header_plugin_extraction_interface*> iter(map_plugin_pointer_obj);
    while (iter.hasNext()) {
        iter.next();

        header_plugin_extraction_interface * mmobj = iter.value();
        if(mmobj != NULL)
            delete mmobj;

    }
    map_plugin_pointer_obj.clear();

    if(sources_pending_in_queue_for_plugin_extraction.size() > 0)
    {
        if(qobject_run_multiple_plugins_obj != NULL)
            delete qobject_run_multiple_plugins_obj;

        start_plugin_extractor_qobject();
    }

    bool_plugin_extraction_qobject_running = false;


}


void RECON_case::slot_extracted_progress_bar_value_for_plugin(int value)
{
    emit signal_set_progressbar_value(value , QString(MACRO_JobType_Artifacts));
}

void RECON_case::slot_signature_thread_finish()
{
    bool_thread_signature_running = false;

    if(thread_signature_extract->isRunning())
        thread_signature_extract->quit();

    if(thread_signature_extract != NULL && !thread_signature_extract->isRunning())
        delete thread_signature_extract;

    if(thread_signature_obj != NULL)
        delete thread_signature_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();


}

void RECON_case::start_signature_extraction()
{
    if(bool_thread_signature_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Signature Analysis already running.");
        return;
    }

    thread_signature_obj = new thread_signature;

    thread_signature_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_signature_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_signature_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_signature_analysis);

    thread_signature_extract = new QThread;
    thread_signature_obj->moveToThread(thread_signature_extract);

    connect(thread_signature_extract, SIGNAL(started()), thread_signature_obj, SLOT(slot_extract_signature()));
    connect(thread_signature_obj, SIGNAL(finished()), thread_signature_extract, SLOT(quit()));
    connect(thread_signature_obj, SIGNAL(finished()), this, SLOT(slot_signature_thread_finish()));
    connect(thread_signature_obj, SIGNAL(signal_add_all_signatures_to_case_tree(QStringList)), this, SLOT(slot_add_all_signatures_to_case_tree(QStringList)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);
    connect(thread_signature_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_signature_running = true;
    thread_signature_extract->start();
}

void RECON_case::start_face_analysis_extraction()
{
    if(bool_thread_face_analysis_running)
    {

        QMessageBox::information(this, recon_static_functions::get_app_name(), "Face analysis module already running.");

        return;
    }

    thread_face_analysis_obj = new thread_face_analysis;

    thread_face_analysis_obj->pub_set_face_finder_obj(facefinder_obj);
    thread_face_analysis_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_face_analysis_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_face_analysis_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_face_analysis);

    thread_face_extract = new QThread;
    thread_face_analysis_obj->moveToThread(thread_face_extract);

    connect(thread_face_extract, SIGNAL(started()), thread_face_analysis_obj, SLOT(slot_extract_faces()));
    connect(thread_face_analysis_obj, SIGNAL(finished()), thread_face_extract, SLOT(quit()));
    connect(thread_face_analysis_obj, SIGNAL(finished()), this, SLOT(slot_face_analysis_thread_finished()));
    connect(thread_face_analysis_obj, SIGNAL(signal_add_faces_to_case_tree(QStringList)), this, SLOT(slot_add_faces_to_case_tree(QStringList)));
    connect(thread_face_analysis_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_face_analysis_running = true;
    thread_face_extract->start();
}

void RECON_case::slot_add_faces_to_case_tree(QStringList db_path_list)
{
    if(!db_path_list.isEmpty())
    {
        add_face_analysis_result_in_case_tree(db_path_list,true);
    }
}

void RECON_case::slot_face_analysis_thread_finished()
{
    bool_thread_face_analysis_running = false;

    if(thread_face_extract->isRunning())
        thread_face_extract->quit();

    if(thread_face_extract != NULL && !thread_face_extract->isRunning())
        delete thread_face_extract;


    if(thread_face_analysis_obj != NULL)
        delete thread_face_analysis_obj;


    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

}

void RECON_case::add_face_analysis_result_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh)
{
    QString child_name = QString(MACRO_CASE_TREE_Face_Analysis_Category_Faces);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        QTreeWidgetItem *item_face_extraction = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FACE_ANALYSIS);

        if(!item_face_extraction)
            continue;


        if(item_face_extraction->text(0) == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        {
            int rec_count = 0;

            for(int count = 0; count < db_path_list.size(); count++)
            {
                QString db_path = db_path_list.at(count);
                QString command_str = QString("Select COUNT(*) From files");

                rec_count += recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str , 0, db_path, Q_FUNC_INFO).toInt();

            }

            QString records_count_str = QString::number(rec_count);

            if(item_face_extraction->childCount() > 0)
            {
                QTreeWidgetItem *child_item = item_face_extraction->child(0);

                if(bool_navigation_refresh)
                {
                    QString cat_child_txt = child_item->text(enum_global_case_tree_display_column_ZERO);
                    cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));

                    emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child_item->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                }

                child_item->setText(enum_global_case_tree_display_column_ZERO, child_name + " (" + records_count_str+ ")");
                child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
            }

            else
            {
                QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                child_item_second->setText(enum_global_case_tree_display_column_ZERO, child_name + " (" + records_count_str + ")");

                child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Face_Analysis_PARENT));

                child_item_second->setText(enum_global_case_tree_display_column_SECOND, global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                item_face_extraction->addChild(child_item_second);
            }
        }
    }
}

void RECON_case::slot_add_matched_faces_to_case_tree(QString srch_lbl_name)
{
    QString child_name;

    child_name = srch_lbl_name;
    srch_lbl_name = srch_lbl_name.replace(" ", "_");

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString() + srch_lbl_name + "/" + "record.sqlite";

    QString command_str = QString("Select COUNT(*) From files");

    QString rec_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str , 0, db_path, Q_FUNC_INFO);

    if(rec_count.isEmpty() || rec_count == "0")
    {
        rec_count = "0";
    }

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FACE_SEARCH)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Face_Search_PARENT))
        {
            QTreeWidgetItem *child_item = new QTreeWidgetItem;

            emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child_item->text(enum_global_case_tree_display_column_FIRST), child_item->text(enum_global_case_tree_display_column_ZERO));

            child_item->setText(enum_global_case_tree_display_column_ZERO, child_name + " (" + rec_count + ")");

            child_item->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Face_Search_PARENT));

            child_item->setText(enum_global_case_tree_display_column_SECOND, global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, rec_count);

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FACE_SEARCH)->addChild(child_item);

        }
    }
}

void RECON_case::pub_add_matched_faces_to_case_tree_for_load_case(QString search_label_name)
{
    slot_add_matched_faces_to_case_tree(search_label_name);
}

void RECON_case::start_optical_character_recognition_extraction()
{
    if(bool_thread_optical_character_recognition_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Optical Character Recognition Extraction already running.");
        return;
    }
    thread_optical_character_recognition_obj = new thread_optical_character_recognition;

    thread_optical_character_recognition_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_optical_character_recognition_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_optical_character_recognition_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_optical_character_recognition);

    thread_optical_character_recognition_extract = new QThread;
    thread_optical_character_recognition_obj->moveToThread(thread_optical_character_recognition_extract);

    connect(thread_optical_character_recognition_extract, SIGNAL(started()), thread_optical_character_recognition_obj, SLOT(slot_extract_optical_character_recognition()));
    connect(thread_optical_character_recognition_obj, SIGNAL(finished()), thread_optical_character_recognition_extract, SLOT(quit()));
    connect(thread_optical_character_recognition_obj, SIGNAL(finished()), this, SLOT(slot_optical_character_recognition_thread_finish()));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_add_optical_character_recognition_to_case_tree(QStringList)), this, SLOT(slot_add_optical_character_recognition_to_case_tree(QStringList)));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_optical_character_recognition_running = true;
    thread_optical_character_recognition_extract->start();

}

void RECON_case::slot_add_optical_character_recognition_to_case_tree(QStringList db_path_list)
{
    add_optical_character_recognition_result_in_case_tree(db_path_list, true);
}

void RECON_case::add_optical_character_recognition_result_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh)
{
    QString child_name = QString(MACRO_CASE_TREE_Optical_Character_Recognition_Category_Images);

    int rec_int = 0;
    QString rec_count;

    for(int count = 0; count < db_path_list.size(); count++)
    {
        QString db_path = db_path_list.at(count);

        QString command_str = QString("SELECT COUNT(*) FROM files");
        rec_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str , 0, db_path, Q_FUNC_INFO);

        rec_int += rec_count.toInt();
    }
    if(bool_navigation_refresh)
    {
        if(rec_int == 0)
            return;
    }


    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        QTreeWidgetItem *item_optical_character_recognition = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_OPTICAL_CHARACTER_RECOGNITION);

        if(!item_optical_character_recognition)
            continue;

        if(item_optical_character_recognition->text(0) == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
        {

            if(item_optical_character_recognition->childCount() > 0)
            {
                QTreeWidgetItem *child_item = item_optical_character_recognition->child(0);

                if(bool_navigation_refresh)
                {
                    QString cat_child_txt = child_item->text(enum_global_case_tree_display_column_ZERO);
                    cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));

                    emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child_item->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                }

                child_item->setText(enum_global_case_tree_display_column_ZERO, child_name + "(" + QString::number(rec_int) + ")");
                child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(rec_int));
            }

            else
            {

                QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                child_item_second->setText(enum_global_case_tree_display_column_ZERO, child_name + "(" + QString::number(rec_int) + ")");

                child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent));

                child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(rec_int));

                item_optical_character_recognition->addChild(child_item_second);

            }

        }
    }
}

void RECON_case::slot_optical_character_recognition_thread_finish()
{

    bool_thread_optical_character_recognition_running = false;

    if(thread_optical_character_recognition_extract->isRunning())
        thread_optical_character_recognition_extract->quit();

    if(thread_optical_character_recognition_extract != NULL && !thread_optical_character_recognition_extract->isRunning())
        delete thread_optical_character_recognition_extract;

    if(thread_optical_character_recognition_obj != NULL)
        delete thread_optical_character_recognition_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

}

void RECON_case::start_skin_tone_detection_extraction()
{

    if(bool_thread_skin_tone_detection_running)
    {
        QMessageBox::information(this,recon_static_functions::get_app_name(),"Skin Tone Detection already running.");
        return;
    }


    thread_skin_tone_detection_obj = new thread_skin_tone_detection;
    thread_skin_tone_detection_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_skin_tone_detection_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_skin_tone_detection_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_skin_tone_detection);

    thread_skin_tone_detection_extraction = new QThread;
    thread_skin_tone_detection_obj->moveToThread(thread_skin_tone_detection_extraction);

    connect(thread_skin_tone_detection_extraction, SIGNAL(started()), thread_skin_tone_detection_obj, SLOT(slot_extract_skin_tone_detection()));

    connect(thread_skin_tone_detection_obj, SIGNAL(finished()), thread_skin_tone_detection_extraction, SLOT(quit()));
    connect(thread_skin_tone_detection_obj, SIGNAL(finished()), this, SLOT(slot_skin_tone_detection_thread_finished()));

    connect(thread_skin_tone_detection_obj, SIGNAL(signal_add_skin_tone_detection_to_case_tree(QStringList)), this, SLOT(slot_add_skin_tone_detection_to_case_tree(QStringList)));
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_skin_tone_detection_running = true;
    thread_skin_tone_detection_extraction->start();
}

void RECON_case::slot_add_skin_tone_detection_to_case_tree(QStringList db_path_list)
{
    if(!db_path_list.isEmpty())
    {
        add_skin_tone_detection_result_in_case_tree(db_path_list, true);
    }
}

void RECON_case::add_skin_tone_detection_result_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QStringList mime_type_list;

    for(int count = 0; count < db_path_list.size(); count++)
    {
        if(bool_cancel_operation)
            break;

        QString db_path = db_path_list.at(count);
        QString command_str = QString("Select Distinct mime From files");
        mime_type_list += recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0, db_path, Q_FUNC_INFO);

        if(mime_type_list.isEmpty())
        {
            continue;
        }
        mime_type_list.removeDuplicates();
    }

    QStringList mime_category_list;
    for(int count = 0; count < mime_type_list.size(); count++)
    {
        QString mime_str = mime_type_list.at(count);

        if(mime_str.contains("/"))
        {
            mime_category_list << mime_str.split("/", Qt::SkipEmptyParts).at(0);
        }

    }


    ///Adding All MIME Category as Parent first
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        QTreeWidgetItem *item_skin_tone = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SKIN_TONE_DETECTION);

        if(!item_skin_tone)
            continue;


        if(item_skin_tone->text(0) == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
        {


            for(int pos = 0; pos < mime_category_list.size(); pos++)
            {
                QString mime_category_str = mime_category_list.at(pos);
                mime_category_str = mime_category_str.trimmed();


                if(mime_category_str.isEmpty())
                    continue;


                bool bool_is_child_found = false;
                int child_position = -1;


                for(int count = 0; count < item_skin_tone->childCount(); count++)
                {


                    QTreeWidgetItem *child_item = item_skin_tone->child(count);
                    QString category_name_str  = child_item->text(0).trimmed();
                    if(mime_category_str == category_name_str)
                    {
                        child_position = count;
                        bool_is_child_found = true;
                        break;
                    }

                }



                if(!bool_is_child_found)
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, mime_category_str);
                    item_skin_tone->addChild(child_item_second);
                }
            }
        }

        if(bool_navigation_refresh)
        {
            item_skin_tone->setExpanded(false);
        }

        item_skin_tone->sortChildren(0 , Qt::AscendingOrder);
        break;
    }


    ///Adding MIME types under Skin Tone detection Parent.
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        QTreeWidgetItem *item_skin_tone = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SKIN_TONE_DETECTION);

        if(!item_skin_tone)
            continue;

        if(item_skin_tone->text(0) == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
        {

            for(int pos = 0; pos < mime_type_list.size(); pos++)
            {
                QString mime_type_str = mime_type_list.at(pos);
                mime_type_str = mime_type_str.trimmed();
                if(mime_type_str.isEmpty())
                    continue;

                bool bool_is_child_found = false;
                int item_category_child_position = -1;
                QTreeWidgetItem *item_category = NULL;

                for(int count = 0; count < item_skin_tone->childCount(); count++)
                {

                    item_category = item_skin_tone->child(count);

                    QString mime_category_str  = item_category->text(0);
                    mime_category_str = mime_category_str.trimmed();

                    ///parent category found
                    if(mime_type_str.startsWith(mime_category_str))
                    {
                        /// check if child already exists
                        for(int child_count = 0; child_count < item_category->childCount(); child_count++)
                        {
                            QTreeWidgetItem *child_item = item_category->child(child_count);
                            QString child_text_str  = child_item->text(0);
                            child_text_str = child_text_str.remove(child_text_str.indexOf("("), child_text_str.size()).trimmed();

                            if(child_text_str == mime_type_str)
                            {
                                item_category_child_position = child_count;
                                bool_is_child_found = true;

                                break;
                            }
                        }
                        break;
                    }
                }


                if(bool_is_child_found)
                {
                    if(item_category_child_position >= 0 && item_category_child_position < item_category->childCount())
                    {
                        QTreeWidgetItem *child = item_category->child(item_category_child_position);

                        if(bool_navigation_refresh)
                        {
                            QString cat_child_txt = child->text(enum_global_case_tree_display_column_ZERO);
                            cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));
                            emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                        }
                    }
                }

                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE mime = ?");

                QStringList value_list;
                value_list << mime_type_str;

                int records_count = 0;
                for(int countt = 0; countt < db_path_list.size(); countt++)
                {
                    if(bool_cancel_operation)
                        break;
                    QString db_path1 = db_path_list.at(countt);
                    records_count += recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(db_cmd, value_list ,0 , db_path1, Q_FUNC_INFO).toInt();
                }

                QString records_count_str = QString::number(records_count);
                QString item_text_with_records;
                item_text_with_records.clear();

                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  mime_type_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  mime_type_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(item_category_child_position >= 0 && item_category_child_position < item_category->childCount())
                    {
                        QTreeWidgetItem *child_item = item_category->child(item_category_child_position);
                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent));

                    child_item_second->setText(enum_global_case_tree_display_column_SECOND, global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item_category->addChild(child_item_second);
                }
            }

        }

        if(bool_navigation_refresh)
        {
            item_skin_tone->setExpanded(false);
        }

        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::slot_skin_tone_detection_thread_finished()
{
    bool_thread_skin_tone_detection_running = false;

    if(thread_skin_tone_detection_extraction->isRunning())
        thread_skin_tone_detection_extraction->quit();

    if(thread_skin_tone_detection_extraction != NULL && !thread_skin_tone_detection_extraction->isRunning())
        delete thread_skin_tone_detection_extraction;

    if(thread_skin_tone_detection_obj != NULL)
        delete thread_skin_tone_detection_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

    emit signal_add_tags_in_case_tree(MACRO_Tag_Name_Skin_Tone);
}

void RECON_case::start_weapons_extraction()
{
    if(bool_thread_weapons_running)
    {
        QMessageBox::information(this,recon_static_functions::get_app_name(),"Weapons Extraction already running.");
        return;
    }

    thread_weapons_obj = new thread_weapons;

    thread_weapons_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_weapons_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_weapons_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_weapons);

    thread_weapons_extraction = new QThread;
    thread_weapons_obj->moveToThread(thread_weapons_extraction);

    connect(thread_weapons_extraction, SIGNAL(started()), thread_weapons_obj, SLOT(slot_weapons_extraction()));
    connect(thread_weapons_obj, SIGNAL(finished()), thread_weapons_extraction, SLOT(quit()));
    connect(thread_weapons_obj, SIGNAL(finished()), this, SLOT(slot_weapons_thread_finished()));
    connect(thread_weapons_obj, SIGNAL(signal_add_weapons_to_case_tree(QStringList)), this, SLOT(slot_add_weapons_to_case_tree(QStringList)));
    connect(thread_weapons_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_weapons_running = true;
    thread_weapons_extraction->start();

}

void RECON_case::slot_add_weapons_to_case_tree(QStringList db_path_list)
{
    add_weapons_in_case_tree(db_path_list, true);
}

void RECON_case::add_weapons_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString category_rifles = QString(MACRO_CASE_TREE_Weapons_Category_Rifles);
    QString category_gun = QString(MACRO_CASE_TREE_Weapons_Category_Gun);

    QStringList weapons_list;
    weapons_list << category_gun << category_rifles;

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)

            QCoreApplication::processEvents();

        QTreeWidgetItem *item_weapons = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_WEAPONS);

        if(item_weapons->text(0) == QString(MACRO_CASE_TREE_Weapons_Parent))
        {
            QString item_text_with_records;

            for(int jj = 0; jj < weapons_list.size(); jj++)
            {
                if(jj % 100 == 0)

                    QCoreApplication::processEvents();

                QString final_rec_count;

                QString weapons_type_str = weapons_list.at(jj);

                qint64 existing_records_for_display = 0;

                bool bool_is_child_found = false;

                int child_position = -1;

                for(int kk = 0; kk < item_weapons->childCount(); kk++)
                {
                    if(kk % 100 == 0)

                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item_weapons->child(kk);

                    if(child_item->text(0).startsWith(weapons_type_str))
                    {
                        child_position = kk;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_weapons->childCount())
                    {
                        QTreeWidgetItem *child = item_weapons->child(child_position);

                        QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();

                        existing_records_for_display = existing_records_str.toInt();

                        if(bool_navigation_refresh)
                        {
                            QString cat_child_txt = child->text(enum_global_case_tree_display_column_ZERO);
                            cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));

                            emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                        }
                    }
                }

                int rec_cnt = 0 ;

                for(int ll = 0; ll < db_path_list.size(); ll++)
                {
                    QString cmd_get_records = "SELECT count(*) from files where weapon_type = ? ";

                    //                   rec_cnt += recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_get_records,0,db_path_list.at(ll),Q_FUNC_INFO).toInt();
                    rec_cnt += recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_get_records,QStringList(weapons_type_str),0,db_path_list.at(ll),Q_FUNC_INFO).toInt();

                }

                final_rec_count = QString::number(rec_cnt);

                item_text_with_records.clear();

                if(final_rec_count.isEmpty())
                {
                    item_text_with_records =  weapons_type_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  weapons_type_str + QString(" (") + final_rec_count + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_weapons->childCount())
                    {
                        QTreeWidgetItem *child_item = item_weapons->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, final_rec_count);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Weapons_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_SECOND, global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, final_rec_count);
                    item_weapons->addChild(child_item_second);
                }
            }
        }

        if(bool_navigation_refresh)
        {
            item_weapons->setExpanded(false);
        }

        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::slot_weapons_thread_finished()
{
    bool_thread_weapons_running = false;

    if(thread_weapons_extraction->isRunning())
        thread_weapons_extraction->quit();

    if(thread_weapons_extraction != NULL && !thread_weapons_extraction->isRunning())
        delete thread_weapons_extraction;

    if(thread_weapons_obj != NULL)
        delete thread_weapons_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

    display_loading_progress_bar_obj->hide();

}

void RECON_case::start_fire_analysis_extraction()
{

    if(bool_thread_fire_analysis_running)
    {
        QMessageBox::information(this,recon_static_functions::get_app_name(),"Fire Analysis Extraction already running.");
        return;
    }

    thread_fire_analysis_obj = new thread_fire_analysis;

    thread_fire_analysis_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_fire_analysis_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_fire_analysis_obj->pub_set_job_selected_source_count_name_list(st_file_system_post_launcher_job_info_obj.source_list_fire_analysis);

    thread_fire_analysis_extraction = new QThread;
    thread_fire_analysis_obj->moveToThread(thread_fire_analysis_extraction);

    connect(thread_fire_analysis_extraction, SIGNAL(started()), thread_fire_analysis_obj, SLOT(slot_fire_analysis_extraction()));
    connect(thread_fire_analysis_obj, SIGNAL(finished()), thread_fire_analysis_extraction, SLOT(quit()));
    connect(thread_fire_analysis_obj, SIGNAL(finished()), this, SLOT(slot_fire_analysis_thread_finished()));
    connect(thread_fire_analysis_obj, SIGNAL(signal_add_fire_analysis_to_case_tree(QStringList)), this, SLOT(slot_add_fire_analysis_to_case_tree(QStringList)));
    connect(thread_fire_analysis_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_fire_analysis_running = true;
    thread_fire_analysis_extraction->start();
}

void RECON_case::slot_add_fire_analysis_to_case_tree(QStringList db_path_list)
{
    add_fire_analysis_in_case_tree(db_path_list, true);
}

void RECON_case::add_fire_analysis_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString child_name = QString(MACRO_CASE_TREE_Fire_Analysis_Category_Fire);

    int final_rec_count = 0;
    QString rec_count;

    for(int count = 0; count < db_path_list.size(); count++)
    {
        QString db_path = db_path_list.at(count);

        QString command_str = QString("SELECT COUNT(*) FROM files");
        rec_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str , 0, db_path, Q_FUNC_INFO);

        final_rec_count += rec_count.toInt();
    }

    if(bool_navigation_refresh)
    {
        if(final_rec_count == 0)
            return;
    }

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        QTreeWidgetItem *item_fire_analysis = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FIRE_ANALYSIS);

        if(!item_fire_analysis)
            continue;

        if(item_fire_analysis->text(0) == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
        {

            if(item_fire_analysis->childCount() > 0)
            {
                QTreeWidgetItem *child_item = item_fire_analysis->child(0);

                if(bool_navigation_refresh)
                {
                    QString cat_child_txt = child_item->text(enum_global_case_tree_display_column_ZERO);
                    cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));

                    emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child_item->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                }

                child_item->setText(enum_global_case_tree_display_column_ZERO, child_name + " (" + QString::number(final_rec_count) + ")");
                child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(final_rec_count));
            }

            else
            {
                QTreeWidgetItem *child_item_second = new QTreeWidgetItem;

                child_item_second->setText(enum_global_case_tree_display_column_ZERO, child_name + " (" + QString::number(final_rec_count) + ")");
                child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Fire_Analysis_Parent));
                child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(final_rec_count));
                item_fire_analysis->addChild(child_item_second);
            }
        }
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::slot_fire_analysis_thread_finished()
{
    bool_thread_fire_analysis_running = false;

    if(thread_fire_analysis_extraction->isRunning())
        thread_fire_analysis_extraction->quit();

    if(thread_fire_analysis_extraction != NULL && !thread_fire_analysis_extraction->isRunning())
        delete thread_fire_analysis_extraction;

    if(thread_fire_analysis_obj != NULL)
        delete thread_fire_analysis_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

    display_loading_progress_bar_obj->hide();

}

void RECON_case::slot_right_click_remove_record_refresh_in_case_tree(QStringList db_path_list, QString file_type_parent)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
    {
        add_optical_character_recognition_result_in_case_tree(db_path_list,false);
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
    {
        add_skin_tone_detection_result_in_case_tree(db_path_list,false);
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent))
    {
        add_weapons_in_case_tree(db_path_list,false);
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
    {
        add_face_analysis_result_in_case_tree(db_path_list,false);
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
    {
        add_fire_analysis_in_case_tree(db_path_list,false);
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void RECON_case::add_mime_types_in_case_tree(QStringList db_path_list)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QStringList mime_category_list;
    QStringList mime_type_list;

    for(int count = 0; count < db_path_list.size(); count++)
    {
        QString db_path = db_path_list.at(count);

        QString command_str = QString("Select mime_type From files");
        mime_type_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0, db_path, Q_FUNC_INFO);

        if(mime_type_list.isEmpty())
            continue;

        mime_type_list.removeDuplicates();


        for(int count = 0; count < mime_type_list.size(); count++)
        {
            QString mime_str = mime_type_list.at(count);

            if(mime_str.contains("/"))
            {
                mime_category_list << mime_str.split("/", Qt::SkipEmptyParts).at(0);
            }

        }

    }
    ///Adding All MIME Category as Parent first
    ///

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        QTreeWidgetItem *item_mime_types = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_MIME_TYPES);

        if(!item_mime_types)
            continue;

        if(item_mime_types->text(0) == QString(MACRO_CASE_TREE_MIME_Type))
        {
            QString mime_type_str;

            for(int pos = 0; pos < mime_category_list.size(); pos++)
            {
                mime_type_str = mime_category_list.at(pos);
                mime_type_str = mime_type_str.trimmed();

                if(mime_type_str.isEmpty())
                    continue;

                bool bool_is_child_found = false;
                int item_category_child_position = -1;
                QTreeWidgetItem *item_category = NULL;

                for(int count = 0; count < item_mime_types->childCount(); count++)
                {
                    item_category = item_mime_types->child(count);
                    QString mime_category_str  = item_category->text(0).trimmed();

                    if(mime_category_str.startsWith(mime_type_str))
                    {
                        mime_category_str = mime_category_str.remove(mime_category_str.indexOf("("), mime_category_str.size()).trimmed();

                        if(mime_type_str == mime_category_str)
                        {
                            item_category_child_position = count;
                            bool_is_child_found = true;
                            break;
                        }
                    }
                }

                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE mime_type like '" + mime_type_str + "%'");

                qint64 total_records_count_int = 0;
                for(int ll = 0; ll < db_path_list.size(); ll++)
                {
                    total_records_count_int += recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd, 0, db_path_list.at(ll), Q_FUNC_INFO).toInt();
                }
                QString records_count_str =  QString::number(total_records_count_int);

                QString item_text_with_records;
                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  mime_type_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  mime_type_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(item_category_child_position >= 0 && item_category_child_position < item_mime_types->childCount())
                    {
                        QTreeWidgetItem *child = item_mime_types->child(item_category_child_position);
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));

                        QTreeWidgetItem *existing_child_item = item_mime_types->child(item_category_child_position);
                        existing_child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        existing_child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    }
                }
                else
                {
                    QTreeWidgetItem *new_child_item = new QTreeWidgetItem;
                    new_child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    new_child_item->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_MIME_Type));
                    new_child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item_mime_types->addChild(new_child_item);
                }
            }
            item_mime_types->setExpanded(false);
            item_mime_types->sortChildren(0 , Qt::AscendingOrder);
            break;
        }
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}


void RECON_case::add_file_signatures_in_case_tree(QStringList db_path_list)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase case_config_db =  QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    case_config_db.setDatabaseName(conf_db_path);
    if(!case_config_db.open())
    {
        recon_static_functions::app_debug("Case Config db open ---FAILED--- " + case_config_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + case_config_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    //QStringList selected_signature_category_int_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select INT From tbl_signature_category Where category_status = ?") , QStringList("1"), 0, conf_db_path, Q_FUNC_INFO);
    QStringList selected_signature_category_int_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select INT From tbl_signature_category Where category_status = ?") , QStringList("1"), 0, case_config_db, Q_FUNC_INFO);

    QStringList signature_category_list;
    for(int ii = 0; ii < db_path_list.size(); ii++)
    {
        QString db_path_1 = db_path_list.at(ii);
        QString command_str = QString("Select Distinct signature_name from files where signature_name NOTNULL");
        signature_category_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0 , db_path_1 , Q_FUNC_INFO);
    }

    signature_category_list.removeDuplicates();

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item_signature = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FILE_SIGNATURE);

        if(item_signature->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_File_Signature_Parent))
        {

            QString item_text_with_records;

            QString signature_str;

            for(int num = 0; num < signature_category_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();


                signature_str = signature_category_list.at(num);

                //QString signature_category_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select category from tbl_signature_list Where label = ?"), QStringList(signature_str), 0, conf_db_path, Q_FUNC_INFO);
                QString signature_category_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select category from tbl_signature_list Where label = ?"), QStringList(signature_str), 0, case_config_db, Q_FUNC_INFO);
                signature_category_str = signature_category_str.trimmed();

                if(signature_category_str.isEmpty() || !selected_signature_category_int_list.contains(signature_category_str))
                    continue;

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item_signature->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item_signature->child(i);

                    QString tab_text = child_item->text(0);
                    if(tab_text.contains("("))
                        tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());

                    tab_text = tab_text.trimmed();

                    if(tab_text == signature_str)
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_signature->childCount())
                    {
                        QTreeWidgetItem *child = item_signature->child(child_position);
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));
                    }
                }

                QString command = QString("Select count(signature_value) From files Where signature_value is not null AND signature_value != ''" " AND signature_name = '" + signature_str + "'");

                qint64 records_count_int = 0;
                for(int count = 0; count < db_path_list.size(); count++)
                {
                    QString db_path_2 = db_path_list.at(count);
                    records_count_int += recon_helper_standard_obj->get_string_from_db_by_dbpath(command, 0,  db_path_2 , Q_FUNC_INFO).toInt();
                }
                QString records_count_str =  QString::number(records_count_int);

                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  signature_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  signature_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_signature->childCount())
                    {
                        QTreeWidgetItem *child_item = item_signature->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_File_Signature_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item_signature->addChild(child_item_second);
                }
            }
        }

        item_signature->setExpanded(false);
        break;
    }
    case_config_db.close();


    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::add_file_extensions_by_all_extensions_in_case_tree(QString fs_db_path)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }


    QSqlQuery query_select(destination_db);
    query_select.prepare(QString("Select extension,count(extension) from files group by extension"));
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query_select.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_select.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QStringList extension_list_with_records;
    while(query_select.next())
    {
        extension_list_with_records << query_select.value(0).toString() + QString(MACRO_RECON_Splitter_1_multiple) + query_select.value(1).toString();
    }
    destination_db.close();



    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item_ext_type_parent = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_EXTENSIONS);



        if(item_ext_type_parent->text(0) == QString(MACRO_CASE_TREE_File_Extensions))
        {

            QString item_text_with_records;
            QTreeWidgetItem *item_child_all_extension = NULL;
            for(int count = 0; count < item_ext_type_parent->childCount(); count++)
            {
                if(count % 100 == 0)
                    QCoreApplication::processEvents();

                if(item_ext_type_parent->child(count)->text(0) == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions))
                {
                    item_child_all_extension =  item_ext_type_parent->child(count);
                }

            }

            for(int pos = 0; pos < extension_list_with_records.size(); pos++)
            {

                if(pos % 100 == 0)
                    QCoreApplication::processEvents();

                QString extension_with_record_str = extension_list_with_records.at(pos);

                QStringList extension_split_list;
                extension_split_list = extension_with_record_str.split(QString(MACRO_RECON_Splitter_1_multiple), Qt::SkipEmptyParts);

                if(extension_split_list.size() < 2)
                    continue;

                QString suffix_str = extension_split_list.at(0);
                suffix_str = suffix_str.trimmed();

                if(suffix_str.isEmpty())
                    continue;

                qint64  records_count_qit64 = extension_split_list.at(1).toInt();

                bool bool_is_child_found = false;
                int child_position = -1;


                if(item_child_all_extension == NULL)
                {
                    item_child_all_extension = new QTreeWidgetItem;
                    item_child_all_extension->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions));
                    item_child_all_extension->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_File_Extensions));
                    item_ext_type_parent->addChild(item_child_all_extension);
                }
                else
                {
                    for(int count = 0; count < item_child_all_extension->childCount(); count++)
                    {
                        if(count % 100 == 0)
                            QCoreApplication::processEvents();

                        QTreeWidgetItem *child_item = item_child_all_extension->child(count);
                        QString text_str  = child_item->text(0);
                        QString title_str = text_str.mid(0 , text_str.indexOf("("));
                        title_str = title_str.trimmed();


                        if(title_str == suffix_str)
                        {
                            child_position = count;
                            bool_is_child_found = true;
                            break;
                        }

                    }


                    if(bool_is_child_found)
                    {
                        if(child_position >= 0 && child_position < item_child_all_extension->childCount())
                        {

                            QTreeWidgetItem *child = item_child_all_extension->child(child_position);
                            QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();
                            records_count_qit64 += existing_records_str.toInt();
                        }
                    }

                }



                QString records_count_str =  QString::number(records_count_qit64);
                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  suffix_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  suffix_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_child_all_extension->childCount())
                    {
                        QTreeWidgetItem *child_item = item_child_all_extension->child(child_position);
                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    item_child_all_extension->addChild(child_item_second);
                }

            }

            item_ext_type_parent->setExpanded(false);

            //item_ext_type_parent->sortChildren(0 , Qt::AscendingOrder);

            break;

        }

    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}


void RECON_case::add_file_size_in_case_tree(QString db_path)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);


    QStringList filesize_category_list;

    filesize_category_list << QString(MACRO_CASE_TREE_File_Size_Category_250_MB)
                           << QString(MACRO_CASE_TREE_File_Size_Category_1_GB)
                           << QString(MACRO_CASE_TREE_File_Size_Category_Above_1_GB);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FILE_SIZE);

        if(item->text(0) == QString(MACRO_CASE_TREE_File_Size_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < filesize_category_list.size(); num++)
            {

                QString records_count_str;
                qint64 records_count_qint64 = 0;

                QString filesize_title_str = filesize_category_list.at(num);


                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(filesize_title_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();
                        records_count_qint64 += existing_records_str.toInt();
                    }
                }

                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE ");

                qint64 bytes_qint64_second = 0, bytes_qint64_first = 0;

                if(filesize_title_str == QString(MACRO_CASE_TREE_File_Size_Category_50_MB))
                {
                    bytes_qint64_first = 0;
                    bytes_qint64_second = (50*MACRO_Generic_Value_MB_in_byte_int);
                    db_cmd.append(QString("file_size between "+ QString::number(bytes_qint64_first) +" AND "+ QString::number(bytes_qint64_second) +" AND file_size is not null AND file_size != ''")) ;
                }
                else if(filesize_title_str == QString(MACRO_CASE_TREE_File_Size_Category_250_MB))
                {
                    bytes_qint64_first = (50*MACRO_Generic_Value_MB_in_byte_int);
                    bytes_qint64_second = (250*MACRO_Generic_Value_MB_in_byte_int);
                    db_cmd.append(QString("file_size between "+ QString::number(bytes_qint64_first) +" AND "+ QString::number(bytes_qint64_second) +" AND file_size is not null AND file_size != ''")) ;
                }
                else if(filesize_title_str == QString(MACRO_CASE_TREE_File_Size_Category_1_GB))
                {
                    bytes_qint64_first = (250*MACRO_Generic_Value_MB_in_byte_int);
                    bytes_qint64_second = MACRO_Generic_Value_GB_in_byte_int;
                    db_cmd.append(QString("file_size between "+QString::number(bytes_qint64_first) +" AND "+ QString::number(bytes_qint64_second) +" AND file_size is not null AND file_size != ''")) ;
                }
                else if(filesize_title_str == QString(MACRO_CASE_TREE_File_Size_Category_Above_1_GB))
                {
                    bytes_qint64_second = MACRO_Generic_Value_GB_in_byte_int;
                    db_cmd.append(QString("file_size > " + QString::number(bytes_qint64_second) +" AND file_size is not null AND file_size != ''")) ;
                }

                records_count_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO);
                records_count_qint64 += records_count_str.toInt();
                records_count_str =  QString::number(records_count_qint64);

                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  filesize_title_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filesize_title_str + QString(" (") + records_count_str + QString(")");
                }


                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_File_Size_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item->addChild(child_item_second);
                }

            }

            item->setExpanded(false);
            break;
        }
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::parse_registry_files_list(QStringList complete_file_path_list, QString source_count_name)
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    struct_GLOBAL_witness_info_source st_obj =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString initial_path = st_obj.virtual_source_path;

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(fs_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return ;
    }

    QString plugin_name = QString(MACRO_Plugin_Name_File_System);
    QString reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";

    for(int ii = 0 ; ii < complete_file_path_list.size() ;ii++)
    {
        QString complete_path = complete_file_path_list.at(ii);
        if(!QFileInfo(complete_path).exists())
            continue;


        QString display_file_path = complete_path;

        if(display_file_path.startsWith(initial_path))
            display_file_path = display_file_path.remove(0, initial_path.size());


        if(!display_file_path.startsWith("/"))
            display_file_path.prepend("/");


        QString command_1 = "select INT from files where filepath = ?";
        QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_1,QStringList(display_file_path),0,source_db,Q_FUNC_INFO);
        if(record_no.trimmed().isEmpty())
            continue;

        QFileInfo file_info(complete_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString display_tab_name_str = file_info.fileName() + " [" + plugin_name + " " + record_no + "]" + src_cnt_name_label;

        QString command_2 = "select display_tab_name from 'tbl_registry_viewer_index'";
        QStringList list_registry_display_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_2,0,reg_sqlite_path,Q_FUNC_INFO);
        if(list_registry_display_name.contains(display_tab_name_str))
            continue;


        QString display_tab_name = file_info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + "files" + MACRO_RECON_Splitter_1_multiple + record_no;
        QString reg_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString();
        QString destination_db_path = reg_dir_path + source_count_name;
        QDir dir;
        dir.mkpath(destination_db_path);

        QString tm_file_name = file_info.fileName();
        if(tm_file_name.contains(" "))
            tm_file_name.replace(" " , "_");
        if(tm_file_name.contains("."))
            tm_file_name.replace("." , "_");


        QString sqlite_name = tm_file_name + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") + "_" + record_no + ".sqlite";
        destination_db_path = destination_db_path + "/" + sqlite_name;

        struct_global_recon_file_info struct_global_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no,source_db,source_count_name);

        ////----do not change order of calling
        registry_viewer *registry_viewer_obj;
        registry_viewer_obj = new registry_viewer(this);
        registry_viewer_obj->pub_set_destination_db_path(destination_db_path);
        registry_viewer_obj->pub_set_case_tree_tab_name(display_tab_name);
        registry_viewer_obj->pub_set_source_file_path(complete_path);
        registry_viewer_obj->pub_set_source_count_name(source_count_name);
        registry_viewer_obj->pub_set_registry_destination_path(reg_sqlite_path);
        registry_viewer_obj->pub_set_recon_file_info(struct_global_recon_file_info_obj);
        registry_viewer_obj->pub_set_essentials();

        registry_viewer_obj->pub_parse_windows_registry();

        if(!QFile::exists(destination_db_path))
            continue;

        add_registry_file_in_bucket_registry_viewer(display_tab_name_str);
    }

    source_db.close();

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);
}

void RECON_case::add_registry_file_in_bucket_registry_viewer(QString display_tab_name_str)
{
    QString icon_path = "../icons/Features/sub_registry_viewer.png";
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_BUCKET)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Bucket))
        {
            for(int cc = 0; cc < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_BUCKET)->childCount() ; cc++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_BUCKET)->child(cc)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Registry_Viewer))
                {
                    QTreeWidgetItem *sub_child = new QTreeWidgetItem;
                    sub_child->setText(enum_global_case_tree_display_column_ZERO, display_tab_name_str);
                    sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
                    sub_child->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_Registry_Viewer);
                    sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_BUCKET)->child(cc)->addChild(sub_child);
                    break;
                }
            }
        }
    }

}

void RECON_case::add_mobile_backup_info_in_case_tree(QString source_count_name)
{
    recon_static_functions::app_debug("start " , Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";

    QStringList exif_types_list  =  QStringList(MACRO_CASE_TREE_Mobile_Backup_iOS_Backup);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)
            QCoreApplication::processEvents();


        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_MOBILE_BACKUP);


        if(item->text(0) == QString(MACRO_CASE_TREE_Mobile_Backup_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < exif_types_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();

                QString records_count;
                qint64 records_for_display = 0;

                QString filetype_str = exif_types_list.at(num);

                qint64 existing_records_for_display = 0;

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(filetype_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();
                        existing_records_for_display = existing_records_str.toInt();
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));
                    }
                }


                QString db_cmd = QString("SELECT COUNT(*) FROM tbl_backup_list WHERE source_type_internal = ? AND source_count_name = ?");

                if(filetype_str == QString(MACRO_CASE_TREE_Mobile_Backup_iOS_Backup))
                {
                    records_for_display = 0;

                    QStringList m_args;
                    m_args << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal << source_count_name;
                    records_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(db_cmd, m_args , 0 , db_path , Q_FUNC_INFO);
                    records_for_display += records_count.toInt();
                }

                records_for_display += existing_records_for_display;
                records_count =  QString::number(records_for_display);

                if(records_count.isEmpty())
                {
                    item_text_with_records =  filetype_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filetype_str + QString(" (") + records_count + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);

                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Mobile_Backup_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);

                    item->addChild(child_item_second);
                }
            }

        }

        item->setExpanded(false);
        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void RECON_case::pub_set_global_recon_file_info_obj(global_recon_file_info *obj)
{
    global_recon_file_info_obj = obj;
}

bool RECON_case::pub_is_source_extraction_completed(QString source_count_name)
{
    recon_static_functions::app_debug("start " , Q_FUNC_INFO);


    if(extracted_source_count_name_list.contains(source_count_name))
    {
        recon_static_functions::app_debug("end " , Q_FUNC_INFO);
        return true;
    }

    recon_static_functions::app_debug("end " , Q_FUNC_INFO);
    return false;
}

bool RECON_case::pub_is_source_mime_type_extraction_completed(QString source_count_name)
{
    recon_static_functions::app_debug("start " , Q_FUNC_INFO);


    if(extracted_mime_type_source_count_name_list.contains(source_count_name))
    {
        recon_static_functions::app_debug("end " , Q_FUNC_INFO);
        return true;
    }

    recon_static_functions::app_debug("end " , Q_FUNC_INFO);
    return false;
}

void RECON_case::add_window_ram_images_info_in_case_tree(QString source_count_name)
{
    recon_static_functions::app_debug("start " , Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString).toString() + "ram_images.sqlite";

    QTreeWidgetItem *sub_child;
    qint64 total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, db_path, QStringList(), Q_FUNC_INFO);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_IMAGES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_RAM_Images))
        {
            for(int j = 0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_IMAGES)->childCount(); j++)
            {
                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_IMAGES)->child(j);
                sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_Plugin_Name_RAM_Images) + " (" + QString::number(total_record_count) + ")");
                sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_RAM_Images));
                sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT,QString::number(total_record_count));
                return;
            }

            sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_Plugin_Name_RAM_Images) + " (" + QString::number(total_record_count) + ")");
            sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_RAM_Images));
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT,QString::number(total_record_count));

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_IMAGES)->addChild(sub_child);

            break;
        }
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void RECON_case::add_file_extension_by_user_defined_extension_in_case_tree(QString fs_db_path)
{

    QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam =  QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(case_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("case db open ---FAILED--- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString connection_naam2 = QString::number(QDateTime::currentMSecsSinceEpoch()) + "2";
    QSqlDatabase::removeDatabase(connection_naam2);
    QSqlDatabase file_system_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam2);
    file_system_db.setDatabaseName(fs_db_path);
    if(!file_system_db.open())
    {
        recon_static_functions::app_debug("fs db open ---FAILED--- " + file_system_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + file_system_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_EXTENSIONS);


        if(item->text(0) == QString(MACRO_CASE_TREE_File_Extensions))
        {
            QString command_str = QString("Select category_name From extension_category");
            QStringList extension_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_str , 0 , destination_db , Q_FUNC_INFO);

            if(!extension_category_list.isEmpty())
            {

                QTreeWidgetItem *item_child_user_defined_ext = NULL;
                for(int count = 0; count < item->childCount(); count++)
                {
                    if(count % 100 == 0)
                        QCoreApplication::processEvents();

                    if(item->child(count)->text(0) == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions))
                    {
                        item_child_user_defined_ext =  item->child(count);
                    }

                }

                if(item_child_user_defined_ext == NULL)
                {
                    item_child_user_defined_ext = new QTreeWidgetItem;
                    item_child_user_defined_ext->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions));
                    //item_child_user_defined_ext->setIcon(enum_global_case_tree_display_column_FIRST, QIcon("../icons/Features/extensions.png"));
                    item_child_user_defined_ext->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_File_Extensions));
                    item->addChild(item_child_user_defined_ext);
                }

                QString item_text_with_records;
                for(int num = 0; num < extension_category_list.size(); num++)
                {
                    if(num % 100 == 0)
                        QCoreApplication::processEvents();

                    QString records_count_str;
                    qint64 records_count_qint64 = 0;

                    QString category_name_str = extension_category_list.at(num);

                    bool bool_is_child_found = false;
                    int child_position = -1;

                    for(int count = 0; count < item_child_user_defined_ext->childCount(); count++)
                    {
                        if(count % 100 == 0)
                            QCoreApplication::processEvents();

                        QString category_text_with_count = item_child_user_defined_ext->child(count)->text(0);
                        QString dsply_category_name = category_text_with_count.remove(category_text_with_count.indexOf("("), category_text_with_count.size());
                        dsply_category_name = dsply_category_name.trimmed();
                        if(dsply_category_name.trimmed() == category_name_str.trimmed())
                        {
                            child_position = count;
                            bool_is_child_found = true;
                            break;
                        }
                    }


                    if(bool_is_child_found)
                    {
                        if(child_position >= 0 && child_position < item_child_user_defined_ext->childCount())
                        {
                            QTreeWidgetItem *child = item_child_user_defined_ext->child(child_position);
                            QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();
                            records_count_qint64 += existing_records_str.toInt();
                        }
                    }


                    QString command = QString("Select extension_name From extension_list Where category_name = ?");
                    QStringList values;
                    values << category_name_str;
                    QStringList extn_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command, values, 0, destination_db, Q_FUNC_INFO);


                    QString column_name = QString("extension");
                    QStringList main_type_list = extn_list;
                    for(int count = 0; count < main_type_list.size(); count++)
                    {
                        if(count % 100 == 0)
                            QCoreApplication::processEvents();

                        QString extension_str = main_type_list.at(count);
                        if(extension_str.isEmpty())
                            continue;

                        if(extension_str.startsWith("."))
                            extension_str = extension_str.mid(1, extension_str.size()).trimmed();

                        command_str = QString("SELECT COUNT(*) FROM files WHERE ") + column_name + QString("= '") + extension_str + QString("' COLLATE NOCASE");;

                        records_count_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(command_str, 0, file_system_db, Q_FUNC_INFO);

                        records_count_qint64 += records_count_str.toInt();
                    }

                    records_count_str =  QString::number(records_count_qint64);


                    if(records_count_str.isEmpty())
                    {
                        item_text_with_records =  extension_category_list.at(num) + QString(" (0)");
                    }
                    else
                    {
                        item_text_with_records =  extension_category_list.at(num) + QString(" (") + records_count_str + QString(")");
                    }

                    if(bool_is_child_found)
                    {
                        if(child_position >= 0 && child_position < item_child_user_defined_ext->childCount())
                        {
                            QTreeWidgetItem *child_item = item_child_user_defined_ext->child(child_position);

                            child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                            child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                        }
                    }
                    else
                    {
                        QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                        child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        //child_item_second->setIcon(enum_global_case_tree_display_column_FIRST,QIcon("../icons/Features/extensions.png"));
                        child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions));
                        child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                        item_child_user_defined_ext->addChild(child_item_second);
                    }
                }

                item->setExpanded(false);
            }

            break;

        }
    }

    file_system_db.close();

    destination_db.close();
}

void RECON_case::add_disk_images_info_in_case_tree(QString source_count_name)
{
    recon_static_functions::app_debug("start " , Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";

    QStringList child_list  =  QStringList(MACRO_CASE_TREE_Disk_Images_Category_Images);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 10 == 0)
            QCoreApplication::processEvents();


        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_DISK_IMAGES);

        if(item->text(0) == QString(MACRO_CASE_TREE_Disk_Images_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < child_list.size(); num++)
            {
                if(num % 10 == 0)
                    QCoreApplication::processEvents();

                QString records_count;
                qint64 records_for_display = 0;

                QString child_str = child_list.at(num);

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    if(i % 10 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(child_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));
                    }
                }


                QString db_cmd = QString("SELECT COUNT(*) FROM tbl_sources_list");

                if(child_str == QString(MACRO_CASE_TREE_Disk_Images_Category_Images))
                {
                    records_for_display = 0;
                    records_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd, 0 , db_path , Q_FUNC_INFO);
                    records_for_display += records_count.toInt();
                }

                records_count =  QString::number(records_for_display);

                if(records_count.isEmpty())
                {
                    item_text_with_records =  child_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  child_str + QString(" (") + records_count + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Disk_Images_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);

                    item->addChild(child_item_second);
                }
            }

        }

        item->setExpanded(false);
        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void RECON_case::add_metadata_by_apple_metadata_in_case_tree(QString db_path)
{
    //    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);


    //    QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    //    QStringList advance_filetypes_list;
    //    advance_filetypes_list << QString(MACRO_CASE_TREE_Metadata_Category_Geotags)
    //                           << QString(MACRO_CASE_TREE_Metadata_Category_Wherefrom);

    //    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    //    {
    //        if(ii % 100 == 0)
    //            QCoreApplication::processEvents();

    //        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_METADATA);

    //        if(item->text(0) == QString(MACRO_CASE_TREE_Metadata_Parent))
    //        {
    //            QString item_text_with_records;

    //            for(int num = 0; num < advance_filetypes_list.size(); num++)
    //            {
    //                if(num % 100 == 0)
    //                    QCoreApplication::processEvents();

    //                QString records_count;
    //                qint64 records_for_display = 0;

    //                QString filetype_str = advance_filetypes_list.at(num);

    //                qint64 existing_records_for_display = 0;

    //                bool bool_is_child_found = false;
    //                int child_position = -1;
    //                for(int i = 0; i < item->childCount(); i++)
    //                {
    //                    if(i % 100 == 0)
    //                        QCoreApplication::processEvents();

    //                    QTreeWidgetItem *child_item = item->child(i);

    //                    if(child_item->text(0).startsWith(filetype_str))
    //                    {
    //                        child_position = i;
    //                        bool_is_child_found = true;
    //                        break;
    //                    }
    //                }

    //                if(bool_is_child_found)
    //                {
    //                    if(child_position >= 0 && child_position < item->childCount())
    //                    {
    //                        QTreeWidgetItem *child = item->child(child_position);
    //                        QString existing_records_str = child->text(2).trimmed();
    //                        existing_records_for_display = existing_records_str.toInt();
    //                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(1), child->text(0));

    //                    }
    //                }

    //                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE ");

    //                if(filetype_str == QString(MACRO_CASE_TREE_Metadata_Category_Geotags))
    //                {
    //                    records_for_display = 0;

    //                    db_cmd.append(QString("kMDItemLatitude_Latitude is not null AND kMDItemLatitude_Latitude != '' OR kMDItemLongitude_Longitude is not null AND kMDItemLongitude_Longitude != ''")) ;

    //                    records_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO);

    //                    records_for_display += records_count.toInt();
    //                }
    //                else if(filetype_str == QString(MACRO_CASE_TREE_Metadata_Category_Wherefrom))
    //                {
    //                    records_for_display = 0;

    //                    db_cmd.append(QString("kMDItemWhereFroms_Where_From is not null AND kMDItemWhereFroms_Where_From != ''")) ;

    //                    records_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO);

    //                    records_for_display += records_count.toInt();
    //                }

    //                records_for_display += existing_records_for_display;
    //                records_count =  QString::number(records_for_display);
    //                if(records_count.isEmpty())
    //                {
    //                    item_text_with_records =  filetype_str + QString(" (0)");
    //                }
    //                else
    //                {
    //                    item_text_with_records =  filetype_str + QString(" (") + records_count + QString(")");
    //                }

    //                QString icon_name_str = filetype_str + QString(".png");

    //                if(bool_is_child_found)
    //                {
    //                    if(child_position >= 0 && child_position < item->childCount())
    //                    {
    //                        QTreeWidgetItem *child_item = item->child(child_position);

    //                        child_item->setText(enum_global_case_tree_display_column_FIRST, item_text_with_records);
    //                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);
    //                    }
    //                }
    //                else
    //                {
    //                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
    //                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, item_text_with_records);
    //                    child_item_second->setIcon(enum_global_case_tree_display_column_FIRST,QIcon("../icons/Features/" + icon_name_str));
    //                    child_item_second->setText(enum_global_case_tree_display_column_SECOND, QString(MACRO_CASE_TREE_Metadata_Parent));
    //                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);

    //                    item->addChild(child_item_second);
    //                }

    //            }

    //            item->setExpanded(false);
    //            break;
    //        }
    //    }

    //    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}



void RECON_case::slot_exif_metadata_thread_finish()
{
    bool_thread_exif_data_running = false;

    if(thread_exif_data_extract->isRunning())
        thread_exif_data_extract->quit();

    if(thread_exif_data_extract != NULL && !thread_exif_data_extract->isRunning())
        delete thread_exif_data_extract;

    if(thread_exif_metadata_obj != NULL)
        delete thread_exif_metadata_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

}

void RECON_case::slot_dt_search_thread_finish()
{

    bool_thread_text_indexing_running = false;

    if(thread_dt_search_extract->isRunning())
        thread_dt_search_extract->quit();

    if(thread_dt_search_extract != NULL && !thread_dt_search_extract->isRunning())
        delete thread_dt_search_extract;

    if(thread_index_files_obj != NULL)
        delete thread_index_files_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

}

void RECON_case::add_exif_metadata_in_case_tree(QStringList db_path_list)
{

    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase case_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    case_config_db.setDatabaseName(case_db_path);
    if(!case_config_db.open())
    {
        recon_static_functions::app_debug("Case config db open ---FAILED--- " + case_config_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + case_config_db.lastError().text() , Q_FUNC_INFO);
        return;
    }


    QString command = QString("Select parent_name From tbl_exif_attributes Where bookmark_filter = ? group by parent_name");
    QStringList value_list;
    value_list << QString("1");
    //QStringList exif_types_list  =  recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list , 0 , case_db_path , Q_FUNC_INFO);
    QStringList exif_types_list  =  recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command, value_list , 0 , case_config_db , Q_FUNC_INFO);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();


        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_EXIF_METADATA);


        if(item->text(0) == QString(MACRO_CASE_TREE_Exif_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < exif_types_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();


                QString filetype_str = exif_types_list.at(num);

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(filetype_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));
                    }
                }

                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE ");

                if(filetype_str == QString(MACRO_CASE_TREE_Exif_Category_Make_Model))
                {
                    db_cmd.append(QString("make is not null AND make != '' AND model is not null AND model != ''")) ;
                }
                else if(filetype_str == QString(MACRO_CASE_TREE_Exif_Category_GPS))
                {
                    db_cmd.append(QString("(latitude is not null AND latitude != '') AND (longitude is not null AND longitude != '')")) ;
                }
                else
                {
                    QString tmp_cmd = QString("Select custom_column_name from tbl_exif_attributes Where parent_name = '") + filetype_str + QString("'");
                    //QString custom_column_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(tmp_cmd, 0, case_db_path, Q_FUNC_INFO);
                    QString custom_column_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(tmp_cmd, 0, case_config_db, Q_FUNC_INFO);

                    db_cmd.append(custom_column_str + QString(" is not null AND ") + custom_column_str + QString(" != ''"));
                }

                qint64 records_count_qint64 = 0;
                for(int count = 0; count < db_path_list.size(); count++)
                {
                    QString db_path = db_path_list.at(count);
                    records_count_qint64 += recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO).toInt();
                }

                QString records_count_str =  QString::number(records_count_qint64);

                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  filetype_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filetype_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Exif_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item->addChild(child_item_second);
                }
            }
        }

        item->setExpanded(false);
        break;
    }
    case_config_db.close();

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void RECON_case::add_redefined_result_locations_for_apple_meta_and_exif_in_case_tree()
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QStringList types_list;
    types_list << MACRO_CASE_TREE_Redefined_Result_Category_Locations;


    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QStringList db_path_list;
    for(int dd = 0; dd < list_target_source_info.size(); dd++)
    {
        struct_GLOBAL_witness_info_source target_info_struct_obj = list_target_source_info.at(dd);
        QString source_count_name =  target_info_struct_obj.source_count_name;
        if(!pub_is_source_extraction_completed(source_count_name))
            continue;


        QString exif_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/exif_metadata.sqlite");
        QString apple_m_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/apple_metadata.sqlite");

        db_path_list << exif_db_path << apple_m_db_path;
    }



    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();


        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT);
        if(item->text(0) == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < types_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();

                QString filetype_str = types_list.at(num);

                qint64 existing_records_for_display = 0;

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(filetype_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();
                        existing_records_for_display = existing_records_str.toInt();


                    }
                }

                QString db_cmd_for_apple_m = QString("SELECT COUNT(*) FROM files WHERE ");
                QString db_cmd_exif = QString("SELECT COUNT(*) FROM files WHERE ");
                if(filetype_str == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations))
                {
                    db_cmd_for_apple_m.append(QString("(kMDItemLatitude_Latitude is not null AND kMDItemLatitude_Latitude != '') AND (kMDItemLongitude_Longitude is not null AND kMDItemLongitude_Longitude != '')")) ;
                    db_cmd_exif.append(QString("(latitude is not null AND latitude != '') AND (longitude is not null AND longitude != '')")) ;
                }

                QString db_cmd;
                qint64 records_count_qint64 = 0;
                for(int count = 0; count < db_path_list.size(); count++)
                {
                    QString db_path = db_path_list.at(count);
                    if(db_path.endsWith("exif_metadata.sqlite"))
                    {
                        db_cmd = db_cmd_exif;
                    }
                    else if(db_path.endsWith("apple_metadata.sqlite"))
                    {
                        db_cmd = db_cmd_for_apple_m;
                    }

                    records_count_qint64 += recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO).toInt();
                }

                QString records_count_str =  QString::number(records_count_qint64);
                if(records_count_str.isEmpty() || records_count_qint64 == 0)
                {
                    item_text_with_records =  filetype_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filetype_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);
                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Redefined_Result_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item->addChild(child_item_second);
                }
            }
        }

        item->setExpanded(false);
        break;
    }


    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::add_locations_exif_in_case_tree(QStringList db_path_list)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QStringList types_list;
    types_list << MACRO_CASE_TREE_Redefined_Result_Category_Locations;

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();


        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT);


        if(item->text(0) == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < types_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();


                QString filetype_str = types_list.at(num);

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(filetype_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));
                    }
                }

                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE ");

                if(filetype_str == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations))
                {
                    db_cmd.append(QString("(latitude is not null AND latitude != '') AND (longitude is not null AND longitude != '')")) ;
                }


                qint64 records_count_qint64 = 0;
                for(int count = 0; count < db_path_list.size(); count++)
                {
                    QString db_path = db_path_list.at(count);
                    records_count_qint64 += recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO).toInt();
                }

                QString records_count_str =  QString::number(records_count_qint64);

                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  filetype_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filetype_str + QString(" (") + records_count_str + QString(")");
                }



                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);
                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Redefined_Result_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    if(records_count_str != "0")
                        item->addChild(child_item_second);
                }
            }
        }

        item->setExpanded(false);
        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void RECON_case::add_matched_hashes_in_case_tree(QString hashes_db_path, QString source_count)
{
    recon_static_functions::app_debug(" Start " + hashes_db_path, Q_FUNC_INFO);

    QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString cmd_1 = "select database_path from tbl_hashset where bookmark = 1";
    QStringList active_hashset_databases = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_1, 0, case_conf_db, Q_FUNC_INFO);

    QStringList hashset_list;
    for(int count = 0; count < active_hashset_databases.size(); count++)
    {
        QFileInfo info (active_hashset_databases.at(count));
        QString db_name = info.fileName();

        db_name = db_name.remove(info.suffix());
        if(db_name.endsWith("."))
            db_name.chop(1);

        hashset_list << db_name + QString(MACRO_RECON_Splitter_1_multiple) + info.filePath();
    }

    emit signal_update_records_for_hashset_in_case_tree(hashset_list, source_count);


    recon_static_functions::app_debug(" ends " + hashes_db_path, Q_FUNC_INFO);

    return;





    //    QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    //    QString cmd_1 = "select database_path from tbl_hashset where bookmark = 1";

    //    QStringList active_hashset_databases = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_1, 0, case_conf_db, Q_FUNC_INFO);

    //    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    //    {
    //        if(ii % 100 == 0)
    //            QCoreApplication::processEvents();

    //        QTreeWidgetItem *item_hashset = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET);

    //        if(!item_hashset)
    //            continue;

    //        for(int hh = 0; hh < active_hashset_databases.size(); hh++)
    //        {
    //            QString hashset_path = active_hashset_databases.at(hh);
    //            QString hashset_name = QFileInfo(hashset_path).baseName();

    //            int child_position = 0;
    //            bool bool_is_child_found = false;

    //            for(int i = 0; i < item_hashset->childCount(); i++)
    //            {

    //                QTreeWidgetItem *child_item = item_hashset->child(i);
    //                QString child_name = child_item->text(enum_global_case_tree_display_column_ZERO);
    //                child_name = child_name.remove(child_name.indexOf("("), child_name.size()).trimmed();

    //                if(child_name == hashset_name)
    //                {
    //                    child_position = i;
    //                    bool_is_child_found = true;
    //                    break;
    //                }
    //            }


    //            int count = recon_helper_standard_obj->get_db_total_record_by_dbpath(0, hashset_path, QStringList(), Q_FUNC_INFO);
    //            if(count < 0)
    //                count = 0;

    //            QString records_count = QString::number(count);

    //            QString item_text_with_records =  hashset_name + QString(" (") + records_count + ")";

    //            if(bool_is_child_found)
    //            {
    //                if(child_position >= 0 && child_position < item_hashset->childCount())
    //                {
    //                    QTreeWidgetItem *child_item = item_hashset->child(child_position);

    //                    child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
    //                    child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);
    //                }

    //            }
    //            else
    //            {
    //                QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
    //                child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
    //                child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_HASHSET));
    //                child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);
    //                child_item_second->setText(enum_global_case_tree_display_column_THIRD, hashset_path);

    //                item_hashset->addChild(child_item_second);
    //            }


    //        }

    //        break;
    //    }
}

qint64 RECON_case::get_matched_hahses_count_from_hashset(QString hashes_db_path, QString hashset_path)
{
    qint64 matched_count = 0;

    QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString command = QString("Select column_name From tbl_hashset Where database_path = ?");
    QStringList value_list;
    value_list << hashset_path;
    QString hash_column_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , value_list, 0, case_conf_db, Q_FUNC_INFO);
    hash_column_name = hash_column_name.trimmed();

    if(hash_column_name.isEmpty())
        return matched_count;

    command = QString("Select table_name From tbl_hashset Where database_path = ?");
    QString hash_table_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , value_list, 0, case_conf_db, Q_FUNC_INFO);
    hash_table_name = hash_table_name.trimmed();

    if(hash_table_name.isEmpty())
        return matched_count;




    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase hash_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    hash_db.setDatabaseName(hashes_db_path);
    if(!hash_db.open())
    {
        recon_static_functions::app_debug("DB Open -------FAILED------ " + hash_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(hash_db.lastError().text() , Q_FUNC_INFO);
        return matched_count;
    }

    QString db_attach_commmand_hashset = QString("ATTACH DATABASE '%1' AS hashsetDB")
            .arg(QDir::toNativeSeparators(hashset_path));

    QSqlQuery query_hashset(hash_db);
    query_hashset.prepare(db_attach_commmand_hashset);
    if(!query_hashset.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_hashset.lastError().text(),Q_FUNC_INFO);
        hash_db.close();
        return matched_count;
    }

    QString main_command = "select count(a.md5_hash) from main.files as a join hashsetDB."+  hash_table_name +" as b ON a.md5_hash=b." + hash_column_name;

    query_hashset.prepare(main_command);

    if(!query_hashset.exec())
    {
        recon_static_functions::app_debug(" query --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_hashset.lastError().text(),Q_FUNC_INFO);
        hash_db.close();
        return matched_count;
    }

    if(query_hashset.next())
    {
        matched_count =  query_hashset.value(0).toLongLong();
    }


    QString command_detach = QString("DETACH DATABASE hashsetDB");
    query_hashset.prepare(command_detach);
    if(!query_hashset.exec())
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_hashset.lastError().text(),Q_FUNC_INFO);
        return matched_count;
    }


    return matched_count;

}

void RECON_case::start_apple_timestamps_extraction()
{
    thread_apple_timestamps_obj = new thread_apple_timestamps(this);

    process_apple_timestamps = new QProcess(this);
    process_apple_timestamps->setProcessChannelMode(QProcess::MergedChannels);
    connect(process_apple_timestamps, SIGNAL(readyRead()), thread_apple_timestamps_obj, SLOT(slot_prc_apple_timestamps_readyread()));
    //connect(this, SIGNAL(signal_process_apple_timestamps_status(QString)), thread_apple_timestamps_obj, SLOT(slot_prc_generic_with_argument_mergechannel_readyread(QString)));

    connect(thread_apple_timestamps_obj,SIGNAL(signal_thread_apple_timestamp_finished()),this,SLOT(slot_thread_apple_timestamp_finished()));
    // connect(thread_apple_timestamps_obj, SIGNAL(signal_remove_file_type_based_entry_from_main_stackwidget(QString, QString)), this, SLOT(slot_remove_entry_for_file_types_based_in_main_stack(QString, QString)));
    connect(thread_apple_timestamps_obj, SIGNAL(signal_PBAR_thread_apple_timestamp_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_apple_timestamps_obj, SIGNAL(signal_PBAR_thread_apple_timestamp_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_apple_timestamps_obj, SIGNAL(signal_PBAR_thread_apple_timestamp_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    //connect(thread_apple_timestamps_obj, SIGNAL(signal_add_file_extension_to_case(QString)), this, SLOT(slot_add_file_extensions_to_case_tree(QString)));
    //thread_apple_timestamps_obj->set_case_treewidget_obj(treewidget_case_display_obj);
    // thread_apple_timestamps_obj->pub_set_map_ios_backup_parser_obj(map_ios_backup_parser_obj);
    thread_apple_timestamps_obj->pub_set_apple_timestamps_reader_process(process_apple_timestamps);
    thread_apple_timestamps_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_apple_timestamps_obj->pub_set_apple_metadata_all_attributes_name_list(apple_all_attribute_name_list);
    thread_apple_timestamps_obj->pub_set_FS_source_selected_source_count_name_list(source_selected_source_count_name_list);
    //   bool_thread_file_system_running = true;

    thread_apple_timestamps_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_apple_timestamps_obj->start();

}

void RECON_case::slot_hashes_thread_finish()
{
    bool_thread_hashes_running = false;

    if(thread_hashes_data_extract->isRunning())
        thread_hashes_data_extract->quit();

    if(thread_hashes_data_extract != NULL && !thread_hashes_data_extract->isRunning())
        delete thread_hashes_data_extract;

    if(thread_hashes_obj != NULL)
        delete thread_hashes_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

}

void RECON_case::add_apple_metadata_in_case_tree(QStringList db_path_list)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase case_db =  QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    case_db.setDatabaseName(case_db_path);
    if(!case_db.open())
    {
        recon_static_functions::app_debug("Case db open ---FAILED--- " + case_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + case_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString command = QString("Select attribute_name From tbl_apple_attributes Where bookmark_filter = ?");
    QStringList value_list;
    value_list << QString("1");


    QStringList apple_attr_title_list , apple_attr_final_case_tree_list;
    //apple_attr_title_list  =  recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list , 0 , case_db_path , Q_FUNC_INFO);
    apple_attr_title_list  =  recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command, value_list , 0 , case_db , Q_FUNC_INFO);

    apple_attr_final_case_tree_list << apple_attr_title_list;


    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item_apple_metadata = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_APPLE_METADATA);

        if(item_apple_metadata->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Apple_Metadata_Parent))
        {

            QList<QTreeWidgetItem *> list_children_to_delete;

            for(int c_count = 0; c_count < item_apple_metadata->childCount(); c_count++)
            {
                QString c_child_name = item_apple_metadata->child(c_count)->text(enum_global_case_tree_display_column_ZERO).trimmed();

                c_child_name = c_child_name.remove(c_child_name.indexOf("("), c_child_name.size()).trimmed();

                if(!apple_attr_final_case_tree_list.contains(c_child_name))
                    list_children_to_delete << item_apple_metadata->child(c_count);
            }



            for(int m_count = 0; m_count < list_children_to_delete.size(); m_count++)
            {
                item_apple_metadata->removeChild(list_children_to_delete.at(m_count));
            }


            QString item_text_with_records;

            for(int num = 0; num < apple_attr_final_case_tree_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();


                QString metadata_title_str = apple_attr_final_case_tree_list.at(num);


                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item_apple_metadata->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item_apple_metadata->child(i);

                    if(child_item->text(0) == metadata_title_str)
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_apple_metadata->childCount())
                    {
                        QTreeWidgetItem *child = item_apple_metadata->child(child_position);
                        emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), child->text(enum_global_case_tree_display_column_ZERO));
                    }
                }

                QString db_cmd = QString("Select custom_column_name from tbl_apple_attributes Where attribute_name = '") + metadata_title_str + QString("'");
                //QString custom_column_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd, 0, case_db_path, Q_FUNC_INFO);
                QString custom_column_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(db_cmd, 0, case_db, Q_FUNC_INFO);

                db_cmd = QString("SELECT COUNT(*) FROM files WHERE ") + custom_column_str + QString(" is not null AND ") + custom_column_str + QString(" != ''");

                qint64 records_count_qint64 = 0;
                for(int cc = 0; cc < db_path_list.size(); cc++)
                {
                    QString db_path = db_path_list.at(cc);
                    records_count_qint64 += recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO).toInt();
                }
                QString records_count_str =  QString::number(records_count_qint64);

                if(records_count_str == "0")
                    continue;

                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  metadata_title_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  metadata_title_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_apple_metadata->childCount())
                    {
                        QTreeWidgetItem *child_item = item_apple_metadata->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Apple_Metadata_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item_apple_metadata->addChild(child_item_second);
                }
            }
        }

        item_apple_metadata->setExpanded(false);
        break;
    }

    case_db.close();

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::add_locations_apple_meta_in_case_tree(QStringList db_path_list)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QStringList types_list;
    types_list << MACRO_CASE_TREE_Redefined_Result_Category_Locations;



    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(ii % 100 == 0)
            QCoreApplication::processEvents();


        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT);


        if(item->text(0) == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < types_list.size(); num++)
            {
                if(num % 100 == 0)
                    QCoreApplication::processEvents();

                QString filetype_str = types_list.at(num);

                qint64 existing_records_for_display = 0;

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(i);

                    if(child_item->text(0).startsWith(filetype_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child = item->child(child_position);
                        QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();
                        existing_records_for_display = existing_records_str.toInt();
                    }
                }

                QString db_cmd = QString("SELECT COUNT(*) FROM files WHERE ");

                if(filetype_str == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations))
                {
                    db_cmd.append(QString("(kMDItemLatitude_Latitude is not null AND kMDItemLatitude_Latitude != '') AND (kMDItemLongitude_Longitude is not null AND kMDItemLongitude_Longitude != '')")) ;
                }

                qint64 records_count_qint64 = 0;
                for(int count = 0; count < db_path_list.size(); count++)
                {
                    QString db_path = db_path_list.at(count);
                    records_count_qint64 += recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO).toInt();
                }

                QString records_count_str =  QString::number(records_count_qint64);
                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  filetype_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filetype_str + QString(" (") + records_count_str + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);
                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Redefined_Result_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                    item->addChild(child_item_second);
                }
            }
        }

        item->setExpanded(false);
        break;
    }


    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::slot_add_matched_hashes_for_fs_module_to_case_tree(QStringList hashset_db_path_list)
{
    recon_static_functions::app_debug(" Start ", Q_FUNC_INFO);

    QString conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString cmd_1 = "select database_path from tbl_hashset where bookmark = 1";
    QStringList active_hashset_databases = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_1, 0, conf_db_path, Q_FUNC_INFO);

    QStringList default_hashset_list;
    for(int count = 0; count < active_hashset_databases.size(); count++)
    {
        QFileInfo info (active_hashset_databases.at(count));
        QString db_name = info.fileName();

        db_name = db_name.remove(info.suffix());
        if(db_name.endsWith("."))
            db_name.chop(1);

        default_hashset_list << db_name + QString(MACRO_RECON_Splitter_1_multiple) + info.filePath();
    }

    int records_count = 0;

    for(int count = 0; count < hashset_db_path_list.size(); count++)
    {
        QString file_hashes_db_path = hashset_db_path_list.at(count);

        QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(file_hashes_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            continue;
        }

        for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
        {
            if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Hashset))
            {
                for(int pp = 0; pp < default_hashset_list.size() ; pp++)
                {
                    records_count = 0;
                    QStringList list = default_hashset_list.at(pp).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
                    if(list.size() != 2)
                        continue;

                    QString db_name = list.at(0).trimmed();
                    QString db_path = list.at(1).trimmed();

                    QString hash_column_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select column_name From tbl_hashset Where database_path = ?"), QStringList(db_path), 0,conf_db_path ,Q_FUNC_INFO);
                    hash_column_str = hash_column_str.trimmed();
                    if(hash_column_str.isEmpty())
                        continue;

                    QString hash_table_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select table_name From tbl_hashset Where database_path = ?"), QStringList(db_path), 0,conf_db_path ,Q_FUNC_INFO);
                    hash_table_str = hash_table_str.trimmed();
                    if(hash_table_str.isEmpty())
                        continue;

                    QStringList hashes_in_hashset_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select " + hash_column_str + " From " + hash_table_str), 0, db_path, Q_FUNC_INFO);

                    bool bool_child_exists = false;
                    QTreeWidgetItem *sub_child = NULL;
                    for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->childCount(); j++)
                    {
                        QString tab_text = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j)->text(enum_global_case_tree_display_column_ZERO);
                        if(tab_text.contains("("))
                            tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());
                        tab_text = tab_text.trimmed();

                        QString item_db_path = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j)->text(enum_global_case_tree_display_column_THIRD);

                        if(tab_text == db_name && item_db_path == db_path)
                        {
                            sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j);
                            bool_child_exists = true;

                            break;
                        }
                    }

                    for(int num = 0; num < hashes_in_hashset_list.size(); num++)
                    {
                        QStringList records_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select filesystem_record From files Where md5_hash = ?"), QStringList(hashes_in_hashset_list.at(num)), 0, destination_db, Q_FUNC_INFO);
                        records_count += records_list.size();
                    }


                    if(bool_child_exists && sub_child != NULL)
                    {
                        QString previous_records = sub_child->text(enum_global_case_tree_display_column_RECORD_COUNT);
                        records_count += previous_records.toInt();

                        QString title_str = db_name + " (" + QString::number(records_count) + ")";


                        sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
                        sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(records_count));
                    }
                    else
                    {

                        sub_child = new QTreeWidgetItem;
                        QString title_str = db_name + " (" + QString::number(records_count) + ")";

                        sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
                        sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/hashset.png"));
                        sub_child->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Hashset));
                        sub_child->setText(enum_global_case_tree_display_column_THIRD, db_path);
                        sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(records_count));

                        treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->addChild(sub_child);
                    }
                }
            }
        }

        destination_db.close();

    }


    recon_static_functions::app_debug(" ends " , Q_FUNC_INFO);
}

void RECON_case::slot_prc_apple_timestamps_mergechannel_readyread()
{
    QString prc_output = process_apple_timestamps->readAll();
    emit signal_process_apple_timestamps_status(prc_output);
}

void RECON_case::slot_prc_apple_metadata_mergechannel_readyread()
{
    QString prc_output = process_apple_metadata_full_source->readAll();
    emit signal_process_apple_metadata_status(prc_output);
}

void RECON_case::copy_filesystem_dir_to_extracted_content_dir()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString file_system_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();

    QString file_system_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();
    QString file_system_apple_timestamps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString).toString();


    //Copy filesystem directory for apple timestamps
    QProcess process_copy;
    QStringList values_list;
    values_list << "-rf" << file_system_dir_path << file_system_apple_timestamps;

    process_copy.start("cp", values_list);
    if(!process_copy.waitForStarted(3000))
    {
        recon_static_functions::app_debug("Copy process -----FAILED-----", Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int counts = 0;
    while(1)
    {
        counts++;
        if(counts == 50)
        {
            counts = 0;
            QCoreApplication::processEvents();
        }
        if(process_copy.state() == QProcess::NotRunning)
            break;
    }

    //Copy filesystem directory for apple metadata
    values_list.clear();
    values_list << "-rf" << file_system_dir_path << file_system_apple_metadata;
    process_copy.start("cp", values_list);
    if(!process_copy.waitForStarted(3000))
    {
        recon_static_functions::app_debug("Copy process -----FAILED-----", Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int count = 0;
    while(1)
    {
        count++;
        if(count == 50)
        {
            count = 0;
            QCoreApplication::processEvents();
        }
        if(process_copy.state() == QProcess::NotRunning)
            break;
    }
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void RECON_case::pub_rcase_extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name,QString virtual_source_path,QStringList apple_metadata_att_list,QStringList column_name_list)
{

    thread_apple_metadata_file_click_obj->pub_extract_apple_metadata_from_apple_metadata_reader_on_file_click(file_paths_str,source_cnt_name,virtual_source_path,apple_metadata_att_list,column_name_list);
}


void RECON_case::set_apple_metadata_reader_file_system()
{

    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FIRST_LAUNCHER)
    {
        copy_filesystem_dir_to_extracted_content_dir();
    }
    else if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_ADD_SOURCE)
    {
        QString extracted_content_dir_path =   global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Parent_QString).toString();
        QString file_system_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();
        QString file_system_apple_timestamps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString).toString();
        recon_static_functions::do_directory_blank_completely(file_system_apple_timestamps,Q_FUNC_INFO);
        recon_static_functions::do_directory_blank_completely(file_system_apple_metadata,Q_FUNC_INFO);
        copy_filesystem_dir_to_extracted_content_dir();
    }
}

void RECON_case::start_mail_pst_extraction()
{
    thread_pst_mail_obj = new thread_mail_pst();

    thread_pst_mail_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_pst_mail_obj->pub_set_mutex_for_emlx_db(mutex_exclusive_for_email_db);
    thread_pst_mail_obj->set_recon_case_runner_type(recon_case_runner_type);
    thread_pst_mail_obj->pub_set_job_selected_source_count_name_list(source_selected_source_count_name_list);

    thread_pst_mail_extraction = new QThread;
    thread_pst_mail_obj->moveToThread(thread_pst_mail_extraction);

    connect(thread_pst_mail_extraction, SIGNAL(started()), thread_pst_mail_obj, SLOT(slot_pst_mail_extraction()));
    connect(thread_pst_mail_obj, SIGNAL(finished()), thread_pst_mail_extraction, SLOT(quit()));
    connect(thread_pst_mail_obj, SIGNAL(finished()), this, SLOT(slot_pst_mail_thread_finished()));
    connect(thread_pst_mail_obj, SIGNAL(signal_add_pst_mail_to_case_tree(QStringList)), this, SLOT(slot_add_emails_data_in_case_tree(QStringList)));
    connect(thread_pst_mail_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_pst_mail_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_pst_mail_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_pst_mail_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_pst_mail_running = true;
    thread_pst_mail_extraction->start();
}

void RECON_case::slot_add_emails_data_in_case_tree(QStringList db_path_list)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    if(!db_path_list.isEmpty())
    {
        add_emails_data_in_case_tree(db_path_list,true);
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void RECON_case::add_emails_data_in_case_tree(QStringList db_path_list, bool bool_navigation_refresh)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QStringList pst_filetypes_list;
    pst_filetypes_list << MACRO_CASE_TREE_Email_Files_Category_Email_Parser;

    QString records_count;
    for(int ii = 0 ; ii < db_path_list.size(); ii++)
    {
        QString db_path = db_path_list.at(ii);
        QString db_cmd = QString("SELECT COUNT(*) FROM emlx_data");
        records_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , db_path , Q_FUNC_INFO);
    }

    if(bool_navigation_refresh)
    {
        if(records_count == "0")
            return;
    }

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)
            QCoreApplication::processEvents();

        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_EMAIL_FILES);

        if(item->text(0) == QString(MACRO_CASE_TREE_Email_Files_Parent))
        {
            QString item_text_with_records;

            for(int num = 0; num < pst_filetypes_list.size(); num++)
            {
                QString filetype_str = pst_filetypes_list.at(num);

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int ii = 0; ii < item->childCount(); ii++)
                {
                    if(ii % 100 == 0)
                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item->child(ii);

                    if(child_item->text(0).startsWith(filetype_str))
                    {
                        child_position = ii;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(records_count.isEmpty())
                {
                    item_text_with_records =  filetype_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  filetype_str + QString(" (") + records_count + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item->childCount())
                    {
                        QTreeWidgetItem *child_item = item->child(child_position);

                        if(bool_navigation_refresh)
                        {
                            QString cat_child_txt = child_item->text(enum_global_case_tree_display_column_ZERO);
                            cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));

                            emit signal_remove_entry_file_type_based_in_stack_and_refresh_widget(child_item->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                        }

                        child_item->setText(0, item_text_with_records);
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(0, item_text_with_records);
                    child_item_second->setText(1, QString(MACRO_CASE_TREE_Email_Files_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count);

                    item->addChild(child_item_second);
                }
            }
        }

        item->setExpanded(false);
        break;
    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void RECON_case::slot_pst_mail_thread_finished()
{
    bool_thread_pst_mail_running = false;

    if(thread_pst_mail_extraction->isRunning())
        thread_pst_mail_extraction->quit();

    if(thread_pst_mail_extraction != NULL && !thread_pst_mail_extraction->isRunning())
        delete thread_pst_mail_extraction;

    if(thread_pst_mail_obj != NULL)
        delete thread_pst_mail_obj;

    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();
}

void RECON_case::start_mail_emlx_extraction()
{
    thread_mail_emlx_obj = new thread_mail_emlx;

    thread_mail_emlx_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_mail_emlx_obj->pub_set_mutex_for_mail_emlx_db(mutex_exclusive_for_email_db);
    thread_mail_emlx_obj->pub_set_recon_case_runner_type(recon_case_runner_type);
    thread_mail_emlx_obj->pub_set_job_selected_source_count_name_list(source_selected_source_count_name_list);

    thread_mail_emlx_extraction = new QThread;
    thread_mail_emlx_obj->moveToThread(thread_mail_emlx_extraction);

    connect(thread_mail_emlx_extraction, SIGNAL(started()),thread_mail_emlx_obj,SLOT(slot_extract_emlx_files_data()));
    connect(thread_mail_emlx_obj,SIGNAL(finished()),thread_mail_emlx_extraction,SLOT(quit()));
    connect(thread_mail_emlx_obj, SIGNAL(finished()),this,SLOT(slot_mail_emlx_thread_finish()));
    connect(thread_mail_emlx_obj, SIGNAL(signal_add_mail_emlx_files_to_case_tree(QStringList)),this,SLOT(slot_add_emails_data_in_case_tree(QStringList)));
    connect(thread_mail_emlx_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)),task_status_window_obj,SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_mail_emlx_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)),task_status_window_obj,SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_mail_emlx_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)),task_status_window_obj,SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_mail_emlx_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_mail_emlx_files_running = true;
    thread_mail_emlx_extraction->start();

}

void RECON_case::slot_mail_emlx_thread_finish()
{
    bool_thread_mail_emlx_files_running = false;

    if(thread_mail_emlx_extraction->isRunning())
        thread_mail_emlx_extraction->quit();

    if(thread_mail_emlx_extraction != NULL && !thread_mail_emlx_extraction->isRunning())
        delete thread_mail_emlx_extraction;


    if(thread_mail_emlx_obj != NULL)
        delete thread_mail_emlx_obj;


    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();
}

void RECON_case::start_mail_mbox_extraction()
{
    thread_mail_mbox_obj = new thread_mail_mbox;

    thread_mail_mbox_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_mail_mbox_obj->pub_set_mutex_for_mail_mbox_db(mutex_exclusive_for_email_db);
    thread_mail_mbox_obj->pub_set_recon_case_runner_type(recon_case_runner_type);
    thread_mail_mbox_obj->pub_set_job_selected_source_count_name_list(source_selected_source_count_name_list);

    thread_mail_mbox_extraction = new QThread;
    thread_mail_mbox_obj->moveToThread(thread_mail_mbox_extraction);

    connect(thread_mail_mbox_extraction, SIGNAL(started()),thread_mail_mbox_obj,SLOT(slot_extract_mbox_files_data()));
    connect(thread_mail_mbox_obj,SIGNAL(finished()),thread_mail_mbox_extraction,SLOT(quit()));
    connect(thread_mail_mbox_obj, SIGNAL(finished()),this,SLOT(slot_mail_mbox_thread_finish()));
    connect(thread_mail_mbox_obj, SIGNAL(signal_add_mail_mbox_files_to_case_tree(QStringList)),this,SLOT(slot_add_emails_data_in_case_tree(QStringList)));
    connect(thread_mail_mbox_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)),task_status_window_obj,SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_mail_mbox_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)),task_status_window_obj,SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_mail_mbox_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)),task_status_window_obj,SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_mbox_mail_running = true;
    thread_mail_mbox_extraction->start();

}

void RECON_case::slot_mail_mbox_thread_finish()
{
    bool_thread_mbox_mail_running = false;

    if(thread_mail_mbox_extraction->isRunning())
        thread_mail_mbox_extraction->quit();

    if(thread_mail_mbox_extraction != NULL && !thread_mail_mbox_extraction->isRunning())
        delete thread_mail_mbox_extraction;


    if(thread_mail_mbox_obj != NULL)
        delete thread_mail_mbox_obj;


    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();
}
