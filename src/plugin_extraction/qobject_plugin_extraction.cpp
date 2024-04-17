#include "qobject_plugin_extraction.h"

qobject_plugin_extraction::qobject_plugin_extraction(QObject *parent) : QObject(parent)
{
    bool_plugin_extraction_canceled = false;
    bool_cancel_everything = true;
    recon_helper_standard_obj = new recon_helper_standard(this);

    prc_log_macos_log_show = new QProcess(this);
    prc_log_macos_log_show->setProcessChannelMode(QProcess::MergedChannels);
    connect(prc_log_macos_log_show, SIGNAL(readyRead()), this, SLOT(slot_apple_macOS_log_show_proc_readyread()));
    connect(prc_log_macos_log_show, SIGNAL(finished(int)), this, SLOT(slot_apple_macOS_log_show_proc_finished()));

    recon_helper_process_obj = new recon_helper_process(this);

    m_timer_log_macos_log_show = new QTimer(this);
    connect(m_timer_log_macos_log_show,SIGNAL(timeout()),this, SLOT(on_slot_timeout_apple_macOS_log_collection_timer()));

}

void qobject_plugin_extraction::pub_set_plugin_and_tab_list_for_extraction(QList<struct_global_selected_plugins_info> list)
{
    plugin_and_tab_list = list;
}

void qobject_plugin_extraction::pub_cancel_everything()
{
    bool_plugin_extraction_canceled = true;
    bool_cancel_everything = true;
    plugin_extraction_interface_obj->pub_cancel_extraction();
}

void qobject_plugin_extraction::pub_set_source_count_name_list_for_extraction(QStringList list)
{
    source_selected_source_count_name_list = list;
}

void qobject_plugin_extraction::pub_set_plugin_pointer_obj(QMap<QString, header_plugin_extraction_interface *> obj)
{
    map_plugin_pointer_obj = obj;
}


QStringList qobject_plugin_extraction::remaining_plugin_tab_extraction_for_source(QString source_count_name, QString plugin_name, QStringList tab_list)
{
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/plugin_status.sqlite");


    QString cmd = "select tab_name from plugin_status where plugin_name = ?";

    QStringList already_extracted_tab_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(plugin_name), 0, dest_db_path, Q_FUNC_INFO);


    return recon_static_functions::get_unmatched_varible_list_of_parentlist(tab_list, already_extracted_tab_list, Q_FUNC_INFO);

}


void qobject_plugin_extraction::slot_extract_plugins()
{
    recon_static_functions::app_debug(" Starts - " , Q_FUNC_INFO);


    bool_plugin_extraction_canceled = false;

    bool_cancel_everything = false;
    QList<struct_GLOBAL_witness_info_source> qlist_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    if(qlist_target_source_info.size() <= 0)
    {
        emit finished();
        return;
    }


    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Artifact_Extraction);

    fill_map_redefined_result_category();

//    qint64 examiner_selected_timezone_epch =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();
//    qint64 examiner_machine_timezone_epch = global_recon_helper_singular_class_obj->struct_default_timezone_details_obj.d_case_created_machine_timezn_numeric_qint64;
//    int examiner_selected_date_format =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt();

    struct_global_desktop_info struct_mchne_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
    QString examinner_machine_os_vrsn_full = struct_mchne_info_obj.os_version_full;

    for(int plugin_count = 0; plugin_count < plugin_and_tab_list.size(); plugin_count++)
    {

        struct_global_selected_plugins_info plugin_info = plugin_and_tab_list.at(plugin_count);

        target_plugin_name = plugin_info.plugin_name;
        plugin_extraction_interface_obj  = find_plugin_ptr_by_pluginname(target_plugin_name);


        supported_os_schemes_list = plugin_info.supported_os_schemes_list;

        if(plugin_extraction_interface_obj == NULL)
            continue;


        current_count_and_total_plugins = QString::number(plugin_count + 1) + "/" + QString::number(plugin_and_tab_list.size());
        emit signal_PBAR_plugin_extraction(target_plugin_name, 0,current_count_and_total_plugins);

        bool_plugin_extraction_canceled = false;

        QStringList  total_table_list;
        total_table_list = plugin_info.total_table_list;


        plugin_extraction_interface_obj->pub_set_essentials();
        plugin_extraction_interface_obj->pub_set_engine_type(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_CALLER_ENGINE_TYPE_RECON_LAB);


        struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_desktop_info desktop_info_obj;
        recon_static_functions::clear_variables_recon_and_artifact_desktop_info(desktop_info_obj);
        desktop_info_obj.os_version_full = examinner_machine_os_vrsn_full;
        desktop_info_obj.python_executable_path = global_varibale_python_path;


        desktop_info_obj.bool_desktop_password_accurate = struct_mchne_info_obj.bool_desktop_pwd_accurate;
        desktop_info_obj.desktop_password_original = struct_mchne_info_obj.desktop_password_original;
        desktop_info_obj.desktop_password_modified_symbol_hex_value = struct_mchne_info_obj.desktop_password_modified_symbol_hex_value;
        plugin_extraction_interface_obj->pub_set_desktop_info(desktop_info_obj);

        struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_general_info gnrl_info_obj;
        recon_static_functions::clear_variables_recon_and_artifact_general_info(gnrl_info_obj);
        gnrl_info_obj.debug_file_path = recon_static_functions::get_debug_filepath_regular();
        gnrl_info_obj.debug_mode_status_bool = global_variable_debug_mode_status_bool;
        gnrl_info_obj.debug_print_in_file_bool = global_variable_debug_print_in_file_bool;
        gnrl_info_obj.debug_print_on_terminal_bool = global_variable_debug_print_on_terminal_bool;
        gnrl_info_obj.debug_encryption_status_bool = global_variable_debug_encryption_status_bool;
        plugin_extraction_interface_obj->pub_set_general_info(gnrl_info_obj);

        plugin_extraction_interface_obj->pub_set_result_directory(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString());
        plugin_extraction_interface_obj->pub_set_extended_attributes_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Apple_Attributes_All_Key_QStringList).toStringList());
        plugin_extraction_interface_obj->pub_create_db_tables();


        recon_static_functions::app_debug(" plugin name " +  target_plugin_name,Q_FUNC_INFO);
        recon_static_functions::app_debug(" supported_os_schemes_list " + supported_os_schemes_list.join(",") ,Q_FUNC_INFO);

        for(int i = 0; i < qlist_target_source_info.size(); i++)
        {
            if(bool_plugin_extraction_canceled)
                break;

            struct_GLOBAL_witness_info_source struct_target_info = qlist_target_source_info.at(i);

            if(!QFileInfo(struct_target_info.virtual_source_path).exists())
                continue;


            if(!source_selected_source_count_name_list.contains(struct_target_info.source_count_name) )
                continue;

            QStringList remaining_tabs = remaining_plugin_tab_extraction_for_source(struct_target_info.source_count_name, target_plugin_name, plugin_info.selected_plugin_tab_names);
            recon_static_functions::app_debug(" remaining_tabs " + remaining_tabs.join("--"),Q_FUNC_INFO);
            if(remaining_tabs.size() <= 0)
                continue;


            if(struct_target_info.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                    && struct_target_info.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                    && struct_target_info.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                    && struct_target_info.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display
                    && struct_target_info.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display)
            {
                continue;
            }

            if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
            {
                // To escape the redundancy in artifact's data.
                // There is redundancy in macOS 10.15 or above source because of system and data partitions.
                // Now we have to extract artifact only from Data partition.

                bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(struct_target_info.source_count_name);
                if(!bool_src_applicable)
                {
                    save_plugin_tab_status(target_plugin_name, remaining_tabs, struct_target_info.source_count_name);
                    continue;
                }
            }

            QString initial_source_path = struct_target_info.virtual_source_path;

            if(!initial_source_path.isEmpty() && (struct_target_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal))
            {
                // /Users/maruti/tmp_sumuri_recon_img_mount/dummy_root/RECON_MNT_disk5s1_user2.dmg/Users/forenso
                // removing softlink user, because plugin need initial path as root(/).

                initial_source_path.remove(initial_source_path.lastIndexOf("/Users"), initial_source_path.size());
            }

            if(initial_source_path.isEmpty())
            {
                recon_static_functions::app_debug(" ---FAILED--- empty virtual source path, " +  struct_target_info.source_count_name,Q_FUNC_INFO);
                continue;
            }
            if(!initial_source_path.endsWith("/"))
                initial_source_path.append("/");


            recon_static_functions::app_debug(" os_scheme_display " + struct_target_info.os_scheme_display,Q_FUNC_INFO);
            recon_static_functions::app_debug(" os_scheme_internal " + struct_target_info.os_scheme_internal,Q_FUNC_INFO);
            recon_static_functions::app_debug(" virtual_source_path " + struct_target_info.virtual_source_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" system_user_list " + struct_target_info.system_user_QStringList.join(","),Q_FUNC_INFO);
            recon_static_functions::app_debug(" os_version_extended " + struct_target_info.os_version_extended,Q_FUNC_INFO);

            struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_source_info cmn_src_obj;
            recon_static_functions::clear_variables_recon_and_artifact_common_source_info(cmn_src_obj);
            extract_log_macos_log_show(struct_target_info);
            cmn_src_obj.macos_logs_unified_log_custom_text_file_path1 = log_macos_log_text_file_path;
            cmn_src_obj.macos_logs_unified_log_actual_path1_QStringList << QString(MACRO_LOG_macOS_SOURCE_DIR_PATH_Diagnostics);
            cmn_src_obj.macos_logs_unified_log_actual_path1_QStringList << QString(MACRO_LOG_macOS_SOURCE_DIR_PATH_UUID);
            cmn_src_obj.regular_source_path = initial_source_path;
            if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
            {
                cmn_src_obj.macos_system_source_path = initial_source_path;
            }

            cmn_src_obj.os_scheme_display = struct_target_info.os_scheme_display;
            cmn_src_obj.os_scheme_internal = struct_target_info.os_scheme_internal;

            for(int pp = 0; pp < struct_target_info.system_account_QList.size(); pp++)
            {
                struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_system_account m_obj;
                recon_static_functions::clear_variables_recon_and_artifact_common_system_account_info(m_obj);

                m_obj.system_user = struct_target_info.system_account_QList.at(pp).system_user;
                m_obj.domain_user = struct_target_info.system_account_QList.at(pp).domain_user;

                cmn_src_obj.system_account_QList.append(m_obj);
            }

            cmn_src_obj.os_version_full = struct_target_info.os_version_full;

            cmn_src_obj.source_type_internal = struct_target_info.source_type_internal;

            if(!struct_target_info.configured_ids_QStringList.isEmpty())
            {
                cmn_src_obj.gtakeoutos_google_takeout_account_id = struct_target_info.configured_ids_QStringList.at(0);
            }
            cmn_src_obj.source_password_original = struct_target_info.source_evidence_password_plain;
            cmn_src_obj.source_password_modified_symbol_hex_value = struct_target_info.source_evidence_password_modified_symbol_hex;

            plugin_extraction_interface_obj->pub_set_source_info(cmn_src_obj);

            plugin_extraction_interface_obj->pub_set_essential_for_recon_lab_extraction(global_csv_reader_class_full_obj->get_tables_name_according_tabs(target_plugin_name, remaining_tabs), total_table_list);


            if(struct_target_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            {
                plugin_extraction_interface_obj->pub_set_ios_uid_backup_pointer(map_ios_uid_backup_parser_obj->value(struct_target_info.source_count_name));
            }

            QString src_cnt_nm = struct_target_info.source_count_name;
            int src_cnt_int = src_cnt_nm.remove(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).toInt();

            plugin_extraction_interface_obj->pub_set_source_count(src_cnt_int);
            plugin_extraction_interface_obj->pub_find_source_file();


            if(bool_cancel_everything)
                return;

            plugin_extraction_interface_obj->pub_extract_artifacts();



            if(bool_plugin_extraction_canceled)
            {

                QStringList tables_list = global_csv_reader_class_full_obj->get_tables_name_according_tabs(target_plugin_name, remaining_tabs);
                empty_current_plugin_on_skip_for_source(struct_target_info.source_count_name,tables_list);
                break;
            }

            save_plugin_tab_status(target_plugin_name, remaining_tabs, struct_target_info.source_count_name);


            set_plugin_with_tab_after_extraction_in_narad_muni();

            emit signal_PBAR_plugin_status_message(target_plugin_name, "Generating Timeline...");
            generate_artifacts_timeline(struct_target_info.source_count_name, remaining_tabs);


            if(global_csv_reader_class_selected_obj->is_plugin_available_for_redefined_result(target_plugin_name))
            {
                emit signal_PBAR_plugin_status_message(target_plugin_name, "Redefining results...");

                start_redefined_result_extraction(struct_target_info.source_count_name, remaining_tabs);
            }


            if(bool_cancel_everything)
                return;

        } // ends source loop



        if(bool_plugin_extraction_canceled)
        {
            plugin_extraction_interface_obj->pub_remove_temporary_asthetics();
            emit signal_plugin_extraction_finished_or_cancelled(target_plugin_name);
            continue;
        }

        plugin_extraction_interface_obj->pub_generate_chat_html_pages();
        plugin_extraction_interface_obj->pub_remove_temporary_asthetics();



        emit signal_plugin_extraction_finished_or_cancelled(target_plugin_name);

    }

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Artifact_Extraction);

    emit finished();
    recon_static_functions::app_debug(" end, plugin name  " + target_plugin_name,Q_FUNC_INFO);
}

void qobject_plugin_extraction::empty_current_plugin_on_skip_for_source(QString source_count_name, QStringList tables_list)
{
    QString plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString();

    QString plugin_name = target_plugin_name;
    plugin_name.replace(" ", "_");
    QString plugins_destination_db_file = plugin_dir_path + plugin_name + "/" + plugin_name + ".sqlite";

    for(int count = 0; count < tables_list.size(); count++)
    {
        QString command = "Delete from " + tables_list.at(count) + " Where source_count_name = ?";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, QStringList(source_count_name), plugins_destination_db_file, Q_FUNC_INFO);
    }
}

void qobject_plugin_extraction::slot_extracted_records_from_plugin(qint64 count)
{
    emit signal_PBAR_plugin_extraction(target_plugin_name, count,current_count_and_total_plugins);
}

void qobject_plugin_extraction::slot_skip_triggered()
{

    bool_plugin_extraction_canceled = true;

    if(plugin_extraction_interface_obj != NULL)
        plugin_extraction_interface_obj->pub_cancel_extraction();
}


header_plugin_extraction_interface *qobject_plugin_extraction::find_plugin_ptr_by_pluginname(QString plugin_name)
{
    recon_static_functions::app_debug(" start " + plugin_name,Q_FUNC_INFO);

    header_plugin_extraction_interface *ptr = NULL;

    QMapIterator<QString, header_plugin_extraction_interface*> iter(map_plugin_pointer_obj);

    while (iter.hasNext())
    {
        iter.next();

        if(iter.key() == plugin_name)
        {
            recon_static_functions::app_debug(" end 1 " + plugin_name,Q_FUNC_INFO);
            return iter.value();
        }
    }

    recon_static_functions::app_debug(" end 2 " + plugin_name,Q_FUNC_INFO);

    return ptr;
}


void qobject_plugin_extraction::pub_set_ios_parser_map(QMap<QString, ios_uid_backup_parser*> *obj)
{
    map_ios_uid_backup_parser_obj = obj;
}

void qobject_plugin_extraction::set_run_plugin_triggered(bool value)
{
    bool_run_plugin_triggered = value;
}

void qobject_plugin_extraction::start_redefined_result_extraction(QString currently_running_sources, QStringList selected_tab_list)
{
    recon_static_functions::app_debug("Start " + target_plugin_name,Q_FUNC_INFO);


    struct_global_redefined_result_selected_plugin_and_tab_info struct_info;
    bool bool_plugin_and_tab_found = false;
    QString category_str;
    QString tmp_category_str;
    QMapIterator<QString, QList<struct_global_redefined_result_selected_plugin_and_tab_info>> map_itr(map_redefinition_category);
    while(map_itr.hasNext())
    {
        map_itr.next();

        QString map_key = map_itr.key();
        QStringList tmp_list = map_key.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
        if(tmp_list.size() < 1)
            continue;

        category_str =   tmp_list.at(0); // Browser History
        tmp_category_str =  category_str;
        tmp_category_str.replace(" ","_");

        QList<struct_global_redefined_result_selected_plugin_and_tab_info> list_selected_plugin_info = map_itr.value();
        for(int i = 0; i < list_selected_plugin_info.size(); i++)
        {
            struct_info = list_selected_plugin_info.at(i);

            if(struct_info.plugin_name == target_plugin_name && selected_tab_list.contains(struct_info.tab_name))
            {
                bool_plugin_and_tab_found = true;
                break;
            }
        }

        if(bool_plugin_and_tab_found)
            break;
    }



    if(!bool_plugin_and_tab_found)
        return;

    QString online_category_command = "select category from accounts";
    QString online_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + "Online_Accounts/Online_Accounts.sqlite";
    QStringList messanger_plugin_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(online_category_command,0,online_db_path,Q_FUNC_INFO);
    messanger_plugin_list.removeDuplicates();

    if(!messanger_plugin_list.contains(target_plugin_name) && category_str == MACRO_Redefined_Result_Messenger)
        return;

    QString plugin_name = struct_info.plugin_name;
    QString tmp_plg_name = struct_info.plugin_name.replace(" ","_");
    QString tab_name = struct_info.tab_name;
    QStringList selected_header_list = struct_info.selected_columns_of_tab_list;
    QStringList given_column_name_list = struct_info.given_columns_name_list;

    QString plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString();
    QString plugins_destination_db_file = plugin_dir_path + tmp_plg_name + "/" + tmp_plg_name + ".sqlite";
    QFile check_exist(plugins_destination_db_file);
    if(!check_exist.exists())
        return;


    QString red_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + tmp_category_str + ".sqlite";

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);



    QStringList data_type_list , db_columns_list ,display_header_list;
    QString table_name ;
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);


    if(tab_index >= 0)
    {
        table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

        db_columns_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);
        data_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);

        display_header_list = st_csv_info_obj.display_header_QList_tablewidget.at(tab_index);
        //display header list must have sender and receiver not (from and to) from plugin as display column
        if(display_header_list.size() != db_columns_list.size())
            return;
    }
    else
    {
        return;
    }

    QString bookmark_col_name ;
    int bookmark_index = data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index >= 0)
        bookmark_col_name = db_columns_list.at(bookmark_index);

    QString record_no_col_name;
    int record_no_index = data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    if(record_no_index >= 0)
        record_no_col_name = db_columns_list.at(record_no_index);


    QString os_scheme_col_name;
    int os_index = data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    if(os_index >= 0)
        os_scheme_col_name = db_columns_list.at(os_index);


    QString timestamp_col_name ;
    int timestamp_index = data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
    if(timestamp_index >= 0)
        timestamp_col_name = db_columns_list.at(timestamp_index);

    QString recon_tag_col_name = "recon_tag_value";
    QString notes_col_name = "notes";
    QString source_count_column_name = "source_count_name";

    QSqlDatabase plugin_source_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    plugin_source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    plugin_source_db.setDatabaseName(plugins_destination_db_file);
    if(!plugin_source_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + plugin_source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + plugin_source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString artifact_source_column_name;
    struct_GLOBAL_witness_info_source target_sources_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(currently_running_sources);

    if((target_sources_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            && (plugin_source_db.record(table_name).contains("artifacts_relation_source_file")))
    {
        artifact_source_column_name = "artifacts_relation_source_file";
    }
    else
    {
        artifact_source_column_name = "source_file";
    }


    QStringList selected_column_list;
    selected_column_list << bookmark_col_name << record_no_col_name << recon_tag_col_name << notes_col_name << artifact_source_column_name << os_scheme_col_name << source_count_column_name;

    redefined_result_static_column_count = selected_column_list.size();

    for(int j = 0; j < selected_header_list.size(); j++)
    {
        QString header_name = selected_header_list.at(j);
        for(int k = 0; k < display_header_list.size(); k++)
        {
            QString name = display_header_list.at(k);
            if(name == header_name)
            {
                QString column_name = db_columns_list.at(k);
                selected_column_list << column_name;
            }
        }
    }


    int index_of_timestamp = selected_column_list.indexOf(timestamp_col_name,0);

    QString command = selected_column_list.join(",");
    command = "SELECT " + command + " FROM " + table_name + " Where source_count_name ='" + currently_running_sources + "'";

    bool bool_is_extraction_done = insert_into_redefined_result_db(plugin_source_db,command,plugin_name,tab_name,given_column_name_list, red_result_db_path ,index_of_timestamp);
    if(bool_is_extraction_done)
        emit signal_insert_entry_in_case_tree_for_redefined_result(category_str);


    plugin_source_db.close();

    recon_static_functions::app_debug("end " + target_plugin_name,Q_FUNC_INFO);

}

bool qobject_plugin_extraction::insert_into_redefined_result_db(QSqlDatabase &plugin_source_db,QString command,QString plugin_name, QString tab_name, QStringList given_column_name_list, QString redefined_result_dbpath , int index_of_timestamp)
{
    recon_static_functions::app_debug("Start" + target_plugin_name,Q_FUNC_INFO);

    //  extract_online_account_for_all_added_source();


    QSqlQuery query_select_plugin(plugin_source_db);
    query_select_plugin.prepare(command);
    if(!query_select_plugin.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " + plugin_source_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_select_plugin.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_select_plugin.executedQuery(),Q_FUNC_INFO);
        return false;
    }

    QString connection_naam;
    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    destination_db.setDatabaseName(redefined_result_dbpath);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + redefined_result_dbpath,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return false;
    }

    destination_db.transaction();

    QSqlQuery query_insert(destination_db);




    QStringList values_list;
    for(int i = 0; i < given_column_name_list.size(); i++)
    {
        values_list << "?";
    }

    QString selected_columns ;
    QString selected_values_question_mark ;
    if(!given_column_name_list.isEmpty())
    {
        selected_columns = given_column_name_list.join(",");
        selected_values_question_mark = values_list.join(",");
    }


    QString command_insert = "INSERT INTO redefined_result( plugin, tab_name, os_scheme_display, source_count_name, bookmark,"
                             "record_no, recon_tag_value, notes, source_file, " + selected_columns + ","
                                                                                                     "year_str, month_year, day_month_year, hour_day_month_year)"
                                                                                                     " VALUES (?,?,?,?,?,?,?,?,?," + selected_values_question_mark + ",?,?,?,?)";


    while(query_select_plugin.next())
    {

        query_insert.prepare(command_insert);
        query_insert.addBindValue(plugin_name);
        query_insert.addBindValue(tab_name);
        query_insert.addBindValue(query_select_plugin.value(5).toString());//5
        query_insert.addBindValue(query_select_plugin.value(6).toString());//6
        query_insert.addBindValue(query_select_plugin.value(0).toString());//0
        query_insert.addBindValue(query_select_plugin.value(1).toString());//1
        query_insert.addBindValue(query_select_plugin.value(2).toString());//2
        query_insert.addBindValue(query_select_plugin.value(3).toString());//3
        query_insert.addBindValue(query_select_plugin.value(4).toString());//4


        ///select plugin specific columns that will start after index 6.
        for(int i = 0; i < given_column_name_list.size(); i++)
        {
            query_insert.addBindValue(query_select_plugin.value(i + redefined_result_static_column_count).toString());
        }

        qint64 date_int = query_select_plugin.value(index_of_timestamp).toULongLong();
        qint64 tmp_date_int = date_int;

        ///for creating category list of graphs
        tmp_date_int += global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();

        QDateTime dttime;
        dttime.toUTC();
        dttime = dttime.fromSecsSinceEpoch(tmp_date_int, Qt::UTC);
        QString yearr = QString::number(dttime.date().year());
        QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
        QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");
        QString mnth_year = QString(dttime.date().toString("MMM") + "-" + QString::number(dttime.date().year()));

        query_insert.addBindValue(yearr);
        query_insert.addBindValue(mnth_year);
        query_insert.addBindValue(day_mnth_year);
        query_insert.addBindValue(hour_day_mnth_year);

        //---------

        if(!query_insert.exec())
        {
            recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Query -  " + query_insert.executedQuery(),Q_FUNC_INFO);
        }
    }

    destination_db.commit();

    destination_db.close();


    recon_static_functions::app_debug("Ends" + target_plugin_name,Q_FUNC_INFO);

    return true;

}

void qobject_plugin_extraction::fill_map_redefined_result_category()
{
    QString configuration_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString).toString() + "redefined_result_filters.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(configuration_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + configuration_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT INT,category_name FROM category");
    if(!query.exec())
    {
        recon_static_functions::app_debug("query.exec() --------FAILED------ " + configuration_db_path ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QString category_INT = query.value("INT").toString();
        QString category_name = query.value("category_name").toString();

        fill_redefined_result_selected_plugin_info_list(destination_db, category_name, category_INT);
    }

    destination_db.close();
}

void qobject_plugin_extraction::fill_redefined_result_selected_plugin_info_list(QSqlDatabase &destination_db, QString category_name, QString category_index)
{
    recon_static_functions::debug_intensive("Start",Q_FUNC_INFO);

    QList<struct_global_redefined_result_selected_plugin_and_tab_info> list_redefined_result_selected_plugin_and_tab_info;
    struct_global_redefined_result_selected_plugin_and_tab_info struct_info;

    QSqlQuery query_select_plugin(destination_db);
    query_select_plugin.prepare("SELECT INT, plugin_tab_name FROM plugin_tab WHERE category_INT = ?");
    query_select_plugin.addBindValue(category_index);
    if(!query_select_plugin.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_select_plugin.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_select_plugin.executedQuery(),Q_FUNC_INFO);
        return;
    }

    while(query_select_plugin.next())
    {
        QString plugin_id = query_select_plugin.value("INT").toString();
        QString plugin_with_tab_name = query_select_plugin.value("plugin_tab_name").toString();

        if(plugin_with_tab_name.contains(MACRO_RECON_Splitter_5_resolution_scope))
        {
            QStringList tmp_list = plugin_with_tab_name.split(MACRO_RECON_Splitter_5_resolution_scope, Qt::SkipEmptyParts);

            if(tmp_list.size() < 2)
                continue;

            QString plugin_name = tmp_list.at(0);
            QString tab_name = tmp_list.at(1);

            struct_info.plugin_name = plugin_name;
            struct_info.tab_name = tab_name;
        }

        struct_info.plugin_INT = plugin_id;

        QString command = "SELECT column_name FROM column_details WHERE category_INT = ? AND plugin_tab_INT = ?";
        QStringList arg_list;
        arg_list << category_index << plugin_id;

        QStringList selected_column_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

        struct_info.selected_columns_of_tab_list = selected_column_list;


        command = "SELECT header_name FROM column_details WHERE category_INT = ? AND plugin_tab_INT = ?";
        QStringList given_columns_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

        QStringList selected_column_name_list;
        for(int i = 0; i < given_columns_name_list.size(); i++)
        {
            QString colume_name = given_columns_name_list.at(i);

            colume_name.replace(" ","_");
            colume_name = colume_name.toLower();

            selected_column_name_list << colume_name;
        }

        struct_info.given_columns_name_list = selected_column_name_list;

        list_redefined_result_selected_plugin_and_tab_info.append(struct_info);
    }

    QString key = category_name + MACRO_RECON_Splitter_1_multiple + category_index;

    map_redefinition_category.insert(key, list_redefined_result_selected_plugin_and_tab_info);

    recon_static_functions::debug_intensive("End",Q_FUNC_INFO);
}


void qobject_plugin_extraction::generate_artifacts_timeline(QString currently_running_sources, QStringList selected_tab_list)
{

    struct_r_case_artifacts_timeline g_timeline_obj;

    struct_global_csv_reader_full_csv_info csv_obj =  global_csv_reader_class_selected_obj->read_csv_value(target_plugin_name);

    if(!global_csv_reader_class_selected_obj->is_timeline_available_in_plugin(target_plugin_name))
        return;

    for(int j = 0; j < selected_tab_list.size(); j++)
    {
        if(!global_csv_reader_class_selected_obj->is_timeline_available_in_tab(target_plugin_name,selected_tab_list.at(j)))
            continue;

        int tab_index =  csv_obj.overall_tab_name_QStringList.indexOf(selected_tab_list.at(j),0);

        if(tab_index != -1)
        {
            QString tablename = csv_obj.table_name_QStringList_tablewidget.at(tab_index);
            QString tab_name = csv_obj.tab_name_QStringList_tablewidget.at(tab_index);
            QStringList timestamp_type = csv_obj.tab_timeline_timestamp_type_QList_tablewidget.at(tab_index);
            QStringList timestamp_column_name = csv_obj.tab_timeline_timestamp_column_name_QList_tablewidget.at(tab_index);
            QStringList db_column_name_list = csv_obj.g_timeline_col_name_QList.at(tab_index);
            QStringList db_column_datatype_list = csv_obj.g_timeline_col_data_type_QList.at(tab_index);

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
            if(timestamp_column_name.size() != timestamp_type.size())
            {
                recon_static_functions::app_debug("Timeline (Artifacts) ----FAILED---- " + target_plugin_name, Q_FUNC_INFO);
                recon_static_functions::app_debug("Timeline (Category) ----FAILED---- " + tab_name, Q_FUNC_INFO);
                recon_static_functions::app_debug("Timeline (timestamp_type) ----FAILED---- " + QString::number(timestamp_type.size()), Q_FUNC_INFO);
                recon_static_functions::app_debug("Timeline (timestamp_column_name) ----FAILED---- " + QString::number(timestamp_column_name.size()), Q_FUNC_INFO);
            }
#endif


            g_timeline_obj.tab_name = tab_name;
            g_timeline_obj.table_name = tablename;
            g_timeline_obj.display_applicable_column_list = db_column_name_list;
            g_timeline_obj.display_applicable_column_datatypes_list = db_column_datatype_list;
            g_timeline_obj.timestamp_applicable_column_list = timestamp_column_name;
            g_timeline_obj.timestamp_type_list = timestamp_type;

            insert_items_in_artifacts_timeline_full_db(g_timeline_obj, currently_running_sources);

        }
    }
}

void qobject_plugin_extraction::insert_items_in_artifacts_timeline_full_db(struct_r_case_artifacts_timeline g_timeline_obj, QString source_count_name)
{
    recon_static_functions::debug_intensive(" Starts,  plugin name -- " + target_plugin_name,Q_FUNC_INFO);


    QString plugin_name = target_plugin_name;

    QString source_plugin_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_name.replace(" ", "_") + "/" + plugin_name.replace(" ", "_") + ".sqlite";
    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening-----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" --Error-- " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();

    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    source_db.setDatabaseName(source_plugin_db_file);
    if(!source_db.open())
    {
        recon_static_functions::app_debug(" Source Db opening-----FAILED-----", Q_FUNC_INFO);
        recon_static_functions::app_debug(" --Error-- " + source_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(source_plugin_db_file,Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QSqlQuery query_select(source_db);

    //1.source_file contains file path with uid name in case of iOS
    //2.artifacts_relation_source_file contains file path with domain name in case of iOS,macOS Home Directory but blank in other cases
    QString src_file_column_nm_in_plugin;
    struct_GLOBAL_witness_info_source target_sources_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if((target_sources_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            && (source_db.record(g_timeline_obj.table_name).contains("artifacts_relation_source_file")))
    {
        src_file_column_nm_in_plugin = "artifacts_relation_source_file";
    }
    else
    {
        src_file_column_nm_in_plugin = "source_file";
    }

    for(int i = 0 ; i < g_timeline_obj.timestamp_applicable_column_list.size() ; i++)
    {
        QString column_name = g_timeline_obj.timestamp_applicable_column_list.at(i);

        query_select.prepare("select source_count_name ,os_scheme_display,"+ src_file_column_nm_in_plugin +"," + column_name + "," + g_timeline_obj.display_applicable_column_list.join(",") +  " from " + g_timeline_obj.table_name + " WHERE source_count_name = ?");
        query_select.addBindValue(source_count_name);
        if(!query_select.exec())
        {
            recon_static_functions::app_debug(" query_select execution-----FAILED-----" + source_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" --Error--" + query_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select.executedQuery(),Q_FUNC_INFO);
            continue;
        }

        while(query_select.next())
        {
            QString source_name = query_select.value(0).toString();
            QString os_scheme =  query_select.value(1).toString();
            QString source_file =  query_select.value(2).toString();
            QString timestamp_value =  query_select.value(3).toString();
            timestamp_value = timestamp_value.trimmed();

            if(timestamp_value.isEmpty())
                continue;

            QString command_2 = "insert into timeline_data(plugin, category,table_name, record_no,stamp_name, stamp_type, item1, item2, timestamp,os_scheme_display,loaded, year_str, month_year, day_month_year, hour_day_month_year,source_count_name,source_file)values(?,?,?,?,?,?,  ?,?,?,?,?,?,  ?,?,?,?,?)";

            QString timestamp_type_name = mining_timestamp_name(g_timeline_obj.timestamp_type_list.at(i), Q_FUNC_INFO);
            QString item1, item2;

            item1 = query_select.value(6).toString();
            item2 = query_select.value(7).toString();

            if(g_timeline_obj.display_applicable_column_datatypes_list.at(2) == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                item1 = recon_static_functions::human_readable_size(item1, Q_FUNC_INFO);
            }
            if(g_timeline_obj.display_applicable_column_datatypes_list.at(3) == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                item2 = recon_static_functions::human_readable_size(item2, Q_FUNC_INFO);
            }


            qint64 tmp_date_int = timestamp_value.toULongLong();
            tmp_date_int += global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();

            QDateTime dttime;
            dttime.toUTC();
            dttime = dttime.fromSecsSinceEpoch(tmp_date_int, Qt::UTC);

            QString yearr = QString::number(dttime.date().year());
            QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
            QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");
            QString mnth_year = QString(dttime.date().toString("MMM") + "-" + QString::number(dttime.date().year()));

            QStringList list;
            list << target_plugin_name << g_timeline_obj.tab_name << g_timeline_obj.table_name << query_select.value(5).toString().trimmed();
            list << timestamp_type_name << g_timeline_obj.timestamp_type_list.at(i) << item1;
            list << item2 << timestamp_value << os_scheme << QString::number(0) << yearr << mnth_year << day_mnth_year << hour_day_mnth_year;
            list << source_name << source_file;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_2, list, destination_db, Q_FUNC_INFO);

        }

    }
    destination_db.commit();

    destination_db.close();
    source_db.close();
    recon_static_functions::debug_intensive(" end,  plugin name -- " + target_plugin_name,Q_FUNC_INFO);
}


void qobject_plugin_extraction::set_plugin_with_tab_after_extraction_in_narad_muni()
{
    QStringList total_extracted_plugin_with_extracted_tab_list = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Selected_Plugins_With_Tab_After_Extraction_QStringList,total_extracted_plugin_with_extracted_tab_list);

    global_csv_reader_class_selected_obj->set_plugin_and_associated_tabs(total_extracted_plugin_with_extracted_tab_list, Q_FUNC_INFO);
}

QString qobject_plugin_extraction::mining_timestamp_name(QString type, QString caller_func)
{
    QString command = "select stamp_name from global_timestamp where stamp_type = '" + type + "'";
    QString db_path = "../Resources/global_timestamp.sqlite";
    return recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,db_path,Q_FUNC_INFO);
}

QString qobject_plugin_extraction::get_query_for_currently_running_source_count_name(QStringList currently_running_sources_list)
{
    QString query_str;
    for(int i = 0; i < currently_running_sources_list.size(); i++)
    {
        query_str.append(" source_count_name = ? OR");
    }

    if(query_str.endsWith(" OR"))
        query_str.chop(3);

    return query_str;
}

void qobject_plugin_extraction::save_plugin_tab_status(QString plugin_name_str, QStringList tab_str, QString source_count_name)
{

    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/plugin_status.sqlite");

    for(int i = 0; i < tab_str.size(); i++)
    {
        QString tab_name = tab_str.at(i);

        if(tab_name.trimmed() == QString(""))
            continue;

        QString command = "insert into plugin_status (plugin_name, tab_name) values(?,?)";
        QStringList arg_list;
        arg_list << plugin_name_str << tab_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, arg_list, dest_db_path, Q_FUNC_INFO);

    }

}


header_plugin_extraction_interface *qobject_plugin_extraction::find_plugin_ptr_for_online_account_plugin(QString plugin_name)
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
    QCoreApplication::processEvents();

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

void qobject_plugin_extraction::on_slot_timeout_apple_macOS_log_collection_timer()
{
    QFileInfo file_info(log_macos_log_text_file_path);
    qint64 file_size_qint = file_info.size();
    if(file_size_qint != 0)
    {
        QString file_size = recon_static_functions::human_readable_size(file_size_qint,Q_FUNC_INFO);
        emit signal_PBAR_plugin_status_message("Collecting Logs" , file_size);
    }
}
void qobject_plugin_extraction::slot_apple_macOS_log_show_proc_readyread()
{

    QByteArray apple_macOS_log_show_data = prc_log_macos_log_show->readAll();

    if(apple_macOS_log_show_data.contains("Could not open log archive") || apple_macOS_log_show_data.contains("missing metadata"))
    {
        bool_log_macos_log_show_error_status = true;
        return;
    }

    if(!apple_macOS_log_show_data.isEmpty())
    {
        log_macos_log_show_qfile.write(apple_macOS_log_show_data);
    }

}

void qobject_plugin_extraction::slot_apple_macOS_log_show_proc_finished()
{
    m_timer_log_macos_log_show->stop();
    bool_log_macos_process_finished = true;
}


void qobject_plugin_extraction::write_apple_macOS_log_archive_data_into_text_file(QString logarchive_file_path , QString file_path)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    log_macos_log_show_qfile.setFileName(file_path);

    if(!log_macos_log_show_qfile.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" file open ----FAILED---- " + log_macos_log_show_qfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error - " + log_macos_log_show_qfile.errorString(),Q_FUNC_INFO);
        return;
    }

    QStringList a_tmp_args;
    a_tmp_args << "-cr" << logarchive_file_path;

    QProcess::execute("xattr", a_tmp_args);

    QStringList arguments;
    arguments << "show" << QString("--archive") << logarchive_file_path << "--force";
    QString command;
    command = "log" ;

    bool_log_macos_process_finished = false;
    bool_log_macos_log_show_error_status = false;
    prc_log_macos_log_show->start(command , arguments);
    m_timer_log_macos_log_show->setInterval(1000);
    m_timer_log_macos_log_show->start();

    if(!prc_log_macos_log_show->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start_1",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + prc_log_macos_log_show->errorString() ,Q_FUNC_INFO);
        log_macos_log_show_qfile.close();
        return;
    }


    int m_cnt = 0;

    while(1)
    {
        m_cnt++;
        if(m_cnt % 50 == 0 )
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(bool_log_macos_process_finished || prc_log_macos_log_show->state() != QProcess::Running)
        {
            break;
        }
    }

    ///Start Process again Because Sometimes the logarchive file not open or missing metadata error occured.

    if(bool_log_macos_log_show_error_status)
    {
        bool_log_macos_process_finished = false;
        prc_log_macos_log_show->start(command , arguments);

        bool_log_macos_log_show_error_status = false;

        m_timer_log_macos_log_show->setInterval(1000);
        m_timer_log_macos_log_show->start();

        if(!prc_log_macos_log_show->waitForStarted())
        {
            recon_static_functions::app_debug("----FAILED---- Process Start_2",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + prc_log_macos_log_show->errorString() ,Q_FUNC_INFO);
            log_macos_log_show_qfile.close();
            return;
        }

        while(1)
        {
            m_cnt++;
            if(m_cnt % 50 == 0)
            {
                m_cnt = 0;
                QCoreApplication::processEvents();
            }
            if(bool_log_macos_process_finished || prc_log_macos_log_show->state() != QProcess::Running)
            {
                break;
            }
        }
    }

    log_macos_log_show_qfile.flush();
    log_macos_log_show_qfile.close();

    recon_static_functions::app_debug("End" , Q_FUNC_INFO);
}

void qobject_plugin_extraction::extract_log_macos_log_show(struct_GLOBAL_witness_info_source source_info_obj)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);


    log_macos_log_text_file_path.clear();

    if((source_info_obj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
            && global_csv_reader_class_selected_obj->is_plugin_log_show_supported(target_plugin_name))
    {
        QString apple_log_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString).toString();

        QString destination_path =  apple_log_dir_path + source_info_obj.source_count_name + "/";
        QDir dir;
        dir.mkpath(destination_path);

        QString log_file_path = destination_path + "/log.txt";

        QFileInfo file(log_file_path);
        if(file.exists())
        {
            log_macos_log_text_file_path = log_file_path;
        }
        else
        {
            emit signal_PBAR_plugin_status_message("Extracting Logs","please wait...");

            QString log_archive_path = destination_path + "log.logarchive";
            QString command = "cp";

            bool bool_any_log_src_exist = false;
            QString diagnostic_source_dir_path = recon_static_functions::get_complete_file_path(source_info_obj.virtual_source_path, QString(MACRO_LOG_macOS_SOURCE_DIR_PATH_Diagnostics), Q_FUNC_INFO);
            if(QDir(diagnostic_source_dir_path).exists())
            {
                bool_any_log_src_exist = true;
                QStringList diagnostic_arguments;
                diagnostic_arguments  << "-r" << "-f" << diagnostic_source_dir_path << log_archive_path;

                recon_helper_process_obj->run_command_with_arguments(command, diagnostic_arguments ,Q_FUNC_INFO);
            }

            QString uuid_source_dir_path = recon_static_functions::get_complete_file_path(source_info_obj.fs_extraction_source_path, QString(MACRO_LOG_macOS_SOURCE_DIR_PATH_UUID), Q_FUNC_INFO);
            if(QDir(uuid_source_dir_path).exists())
            {
                bool_any_log_src_exist = true;
                QStringList uuid_arguments;
                uuid_arguments  << "-r" << "-f" << uuid_source_dir_path << log_archive_path;
                recon_helper_process_obj->run_command_with_arguments(command, uuid_arguments ,Q_FUNC_INFO);
            }

            struct_global_desktop_info struct_mchne_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
            QString m_version = struct_mchne_info_obj.os_version_full;

            QString info_plist_file_path = QString("../Resources/log_archive/") + "Info.plist";
            if(QFile(info_plist_file_path).exists())
            {
                QStringList info_plist_arguments;
                info_plist_arguments << info_plist_file_path << log_archive_path;

                recon_helper_process_obj->run_command_with_arguments(command, info_plist_arguments ,Q_FUNC_INFO);
            }

            if(bool_any_log_src_exist)
            {
                log_macos_log_text_file_path = log_file_path;
                write_apple_macOS_log_archive_data_into_text_file(log_archive_path ,log_macos_log_text_file_path);

                // log timer in above code updating the status
                // so need to re-update for plugin message
                emit signal_PBAR_plugin_extraction(target_plugin_name, 0,current_count_and_total_plugins);
            }

        }
    }



    recon_static_functions::app_debug("End" , Q_FUNC_INFO);

}
