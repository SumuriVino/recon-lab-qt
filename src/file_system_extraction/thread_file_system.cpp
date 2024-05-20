#include "thread_file_system.h"

thread_file_system::thread_file_system(QWidget *parent)
{
    timer_records_fs   = new QElapsedTimer();

    total_records_fs        = 0;

    thread_recon_lab_initial_settings_obj = new thread_recon_lab_initial_settings;

    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_file_system::~thread_file_system()
{
    if(timer_records_fs != NULL)
        delete timer_records_fs;

}

void thread_file_system::pub_set_essential()
{
    list_target_general_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < list_target_general_source_info.size(); count++)
    {
        source_count_name = list_target_general_source_info.at(count).source_count_name;
        if(!source_selected_source_count_name_list.contains(source_count_name))
            continue;

        retrieve_paths();
        create_mime_signature_exif_apple_metadata_and_necessary_path();
    }

}

void thread_file_system::pub_set_tsk_fs_process(QProcess *mobj)
{
    process_tsk_fs = mobj;
}

void thread_file_system::pub_set_general_process(QProcess *mobj)
{
    process_general = mobj;

}

void thread_file_system::run()
{ // Here our file system thread works which prepare the main file_system.sqlite database to displays all the data on file system table widget
    // mainly we prepare our file_system.sqlite database here for different different sources like images, zip, tar, ufdr, ios backup etc
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Adding_Source_To_Case);

    bool bool_all_sources_are_ios = true;
    bool bool_ios_finish_signal_emitt = false;
    for(int count = 0; count < list_target_general_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source struct_info_ptr = list_target_general_source_info.at(count);

        if(!source_selected_source_count_name_list.contains(struct_info_ptr.source_count_name) )
            continue;

        if(struct_info_ptr.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            bool_all_sources_are_ios = false;
            break;
        }
    }



    source_count_name.clear();

    QString msg_str;

    for(int count = 0; count < list_target_general_source_info.size(); count++)
    {
        struct_target_info_ptr = list_target_general_source_info.at(count);

        if(!source_selected_source_count_name_list.contains(struct_target_info_ptr.source_count_name) )
            continue;

        if(struct_target_info_ptr.file_system_type.contains(MACRO_FILE_SYSTEM_TYPE_HFS) || struct_target_info_ptr.file_system_type.contains(MACRO_FILE_SYSTEM_TYPE_APFS))
        {
            thread_recon_lab_initial_settings_obj->start();
        }

        source_count_name = struct_target_info_ptr.source_count_name;
        retrieve_paths();
        set_current_source_ios_uid_backup_parser_obj(map_ios_uid_backup_parser_obj->value(source_count_name));

        msg_str = "Updating case tree for - " + struct_target_info_ptr.source_name;

        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, struct_target_info_ptr.source_name, false, 0, -1, true);

        source_path = struct_target_info_ptr.fs_extraction_source_path;

        total_records_fs        = 0;
        list_struct_fs_dir_info.clear();


        if(struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal) || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal))
        {
            source_path = struct_target_info_ptr.source_path_1;
            source_path = source_path.trimmed();
            if(source_path.endsWith("/"))
                source_path.chop(1);

        }

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            list_struct_fs_dir_info = get_list_extracted_struct_fs_dir_info_from_db();
        }
        else
        {
            if(struct_target_info_ptr.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            {
                fill_struct_dir_list_for_ios_uid_backup();
            }
            else if(struct_target_info_ptr.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                fill_struct_dir_list_for_ios_xml_backup();
            }
            else
            {
                // Already sort sources list by iOS ascending order
                // here we reach if non-ios/(iOS and non-ios) sources are selected
                // this check will execute when ios sources are finished
                if(!bool_all_sources_are_ios && !bool_ios_finish_signal_emitt)
                {
                    bool_ios_finish_signal_emitt = true;
                    emit signal_all_ios_sources_extraction_finished();
                }

                // Iterator which iterate source path and fill structure
                iterate_directory_recursively_and_make_dir_info_structure(source_path);
            }

            store_python_script_outputin_result_dir();

            if(recon_static_functions::is_source_supported_for_tsk(struct_target_info_ptr))
            {
                extract_file_system_tsk();

                if(bool_process_tsk_fs_finish_with_desired_output)
                {
                    update_fs_db_for_path_startfrom();
                }
                else
                {
                    extract_file_system_regular();
                }
            }
            else
            {
                extract_file_system_regular();
            }

            QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
            recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , fs_database_path, Q_FUNC_INFO);

            recon_helper_standard_obj->create_db_index_by_db_path("fs_parent", "file_parent_path", "files", fs_database_path, Q_FUNC_INFO);
            recon_helper_standard_obj->create_db_index_by_db_path("fs_int", "INT", "files", fs_database_path, Q_FUNC_INFO);

            if(struct_target_info_ptr.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || struct_target_info_ptr.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
                    || struct_target_info_ptr.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform)
                    || struct_target_info_ptr.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal))
            {
                if(struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
                        || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal))

                {
                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, ("Generating Timeline - " + struct_target_info_ptr.source_name), false, MACRO_Generic_Progress_No_Records_int, -1, true);
                    generate_filesystem_timeline(struct_target_info_ptr.source_count_name);
                }
            }


            if(struct_target_info_ptr.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
            {
                extract_window_registry_files();
                extract_window_ram_images();
            }

        }

        // Load first level dir in case tree
        add_fs_dir_in_case_tree();

        bool_add_file_basic_to_case = false;
        bool_add_internal_sources_to_case = false;

        emit signal_source_extraction_completed_only_for_basic_file_system(source_count_name);
        emit signal_add_file_basic_to_case(fs_database_path);
        emit signal_add_internal_sources_to_case(source_count_name);

        int tmp_count = 0;
        while(1)
        {
            tmp_count++;
            if(tmp_count % 50 == 0)
            {
                QCoreApplication::processEvents();
                tmp_count = 0;
            }

            if(bool_add_file_basic_to_case && bool_add_internal_sources_to_case)
                break;
        }

    }

    if(bool_all_sources_are_ios && !bool_ios_finish_signal_emitt)
    {
        emit signal_all_ios_sources_extraction_finished();
    }

    emit signal_thread_file_system_finished();

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Adding_Source_To_Case);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void thread_file_system::slot_add_file_basic_to_case_finished()
{
    bool_add_file_basic_to_case = true;
}

void thread_file_system::slot_add_internal_sources_to_case_finished()
{
    bool_add_internal_sources_to_case = true;
}

void thread_file_system::slot_process_tsk_fs_custom_status(QString custom_msg)
{ // tsk function of ready read which reads the output when image mounted and file system filling started.
    // we use tsk/sleuthkit for some images to prepare main db file_system.sqlite
    // "\"Process_Start\""
    custom_msg.remove("\"");

    if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Device_Node_Open_Failed))
    {
        recon_static_functions::app_debug("TSKFS Device_Node_Open_Failed ---FAILED--- ", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_File_System_Not_Found_On_Node))
    {
        recon_static_functions::app_debug("TSKFS File_System_Not_Found_On_Node ---FAILED--- ", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_FS_Destination_DB_Open_Failed))
    {
        recon_static_functions::app_debug("TSKFS FS_Destination_DB_Open_Failed ---FAILED--- ", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_FS_Insertion_Query_Failed))
    {
        recon_static_functions::app_debug("TSKFS FS_Insertion_Query_Failed ---FAILED--- ", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Processed_Records))
    {
        QStringList mmlist = custom_msg.split(QString(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Splitter_1));
        if(mmlist.size() >1)
        {
            custom_msg = mmlist.at(1);
            custom_msg = custom_msg.trimmed();

            total_records_fs += custom_msg.toLongLong();
            if(total_records_fs % 500 == 0)
            {
                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, struct_target_info_ptr.source_name, false, total_records_fs, -1, true);
            }
        }
    }


    if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Process_Finished_With_Success))
    {
        bool_process_tsk_fs_finished = true;
        bool_process_tsk_fs_finish_with_desired_output = true;
        recon_static_functions::app_debug("TSKFS Process_Finished_With_Success", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Process_Finished_With_Failure))
    {
        bool_process_tsk_fs_finished = true;
        recon_static_functions::app_debug("TSKFS Process_Finished_With_Failure ---FAILED--- ", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSREADER_COMMON_Process_Finished_With_Crash))
    {
        bool_process_tsk_fs_finished = true;
        recon_static_functions::app_debug("TSKFS Process_Finished_With_Crash ---FAILED--- ", Q_FUNC_INFO);
    }

}

void thread_file_system::update_fs_db_for_path_startfrom()
{ // update file_system.sqlite database for paths start from which means add initial path + FilePathFromDB
    recon_static_functions::app_debug("start " , Q_FUNC_INFO);

    QString prepend_path = struct_target_info_ptr.virtual_source_path;

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(fs_database_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    source_db.transaction();

    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, ("Generating Timeline - " + struct_target_info_ptr.source_name), false, MACRO_Generic_Progress_No_Records_int, -1, true);

    QString command = QString("Update files Set recon_filefrom = '" + QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main) + "'");
    recon_helper_standard_obj->execute_db_command_by_dbreference(command,source_db,Q_FUNC_INFO);

    qint64 total_all_count = recon_helper_standard_obj->get_string_from_db_by_dbreference("select count(*) from files", 0,source_db,Q_FUNC_INFO).toLongLong();

    qint64 total_not_exist_cnt = 0;
    qint64 total_proces_count = 0;

    int bnch = 900;
    qint64 m_start = 1;
    qint64 m_end = bnch;

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QStringList not_existing_file_record_num_list;

        for(int zz = m_start; zz <= m_end; zz++)
        {
            QString cmd = "select filepath from files where INT = ?";

            QString rec = QString::number(zz);

            QString disply_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd, QStringList(rec), 0, source_db, Q_FUNC_INFO).trimmed();

            if(disply_path.isEmpty())
            {
                continue;
            }

            QString cmplt_path = prepend_path + disply_path;

            if(QFileInfo(cmplt_path).isReadable())
            {
                if(!QFileInfo(cmplt_path).isDir())
                    check_for_other_sources(disply_path, disply_path);
            }
            else
            {
                not_existing_file_record_num_list << QString::number(zz);
            }
        }


        if(not_existing_file_record_num_list.size() > 0)
        {
            total_not_exist_cnt += not_existing_file_record_num_list.size();

            QString command = QString("Update files Set recon_filefrom = '" + QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_TSK_Fuse) + "' where ");


            QString tmp_cmd;
            for(int qq = 0; qq < not_existing_file_record_num_list.size(); qq++)
            {
                tmp_cmd.append("INT = ? ");
                tmp_cmd.append("OR ");
            }

            if(tmp_cmd.endsWith("OR "))
                tmp_cmd.chop(QString("OR ").size());

            command = command + tmp_cmd;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,not_existing_file_record_num_list,source_db,Q_FUNC_INFO);
        }



        total_proces_count = m_end;

        if(total_proces_count >= total_all_count)
            break;


        m_start = m_end + 1;

        m_end = m_end + bnch;
        if(m_end > total_all_count)
        {
            m_end = total_all_count;
        }


    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    source_db.commit();
    source_db.close();

    recon_static_functions::app_debug("end " , Q_FUNC_INFO);

    return;

}

void thread_file_system::extract_file_system_tsk()
{ // fill file_system.sqlite main database using tsk library in case of HFS NTFS, LIFS, EXFAT file system type images

    recon_static_functions::app_debug("start " , Q_FUNC_INFO);

    bool_process_tsk_fs_finish_with_desired_output = false;
    bool_process_tsk_fs_finished = false;

    struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    if(!pwd_obj.bool_desktop_pwd_accurate)
        return;

    QString m_vrtl_path = struct_target_info_ptr.virtual_source_path;

    QString m_node = global_recon_helper_singular_class_obj->get_disk_node_of_mounted_path(m_vrtl_path, Q_FUNC_INFO).trimmed();
    if(m_node.isEmpty())
        return;

    QString r_dev = "/dev/";
    m_node = m_node.remove(0, r_dev.size());
    m_node.prepend(r_dev + "r");  // /dev/rdisk3

    QString db_path = fs_database_path;
    QString src_name = source_count_name;
    QString m_pwd = pwd_obj.desktop_password_original;

    QString exec_path = "./fs_reader.app/Contents/MacOS/fs_reader";

    //QString cmd_load = QString("echo '") + m_pwd + "' | sudo -S " + exec_path.replace(" ", "\\ ") + " " + db_path.replace(" ", "\\ ") + " " + src_name.replace(" ", "\\ ") + " " + m_node;
    QString cmd_load = QString("echo $FORENSOPASSWD") + " | sudo -S " + exec_path.replace(" ", "\\ ") + " " + db_path.replace(" ", "\\ ") + " " + src_name.replace(" ", "\\ ") + " " + m_node;


    //    QString available_file = recon_static_functions::get_available_filename("tsk_fs_file",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
    //    QString script_file = MACRO_RECON_Temp_DirPath_In_Bundle + available_file;

    QString available_file = recon_static_functions::get_available_filename("tsk_fs_file",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString script_file = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;
    qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_file,cmd_load, Q_FUNC_INFO);
    if(byte_size == -1)
        return;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("FORENSOPASSWD", m_pwd);

    QStringList args;
    args << script_file;

    process_tsk_fs->setProcessEnvironment(env);
    process_tsk_fs->start("bash", args);
    //process_tsk_fs->start("sh", QStringList(script_file));


    if (!process_tsk_fs->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + process_tsk_fs->errorString() ,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running111", Q_FUNC_INFO);
    ///forcing thread to wait and read when data available from process.
    int count = 0;
    while(1)
    {
        count++;
        if(count % 100 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(process_tsk_fs->state() == QProcess::NotRunning)
        {
            break;
        }

        process_tsk_fs->waitForReadyRead();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross111", Q_FUNC_INFO);

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;

    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(bool_process_tsk_fs_finished || process_tsk_fs->state() != QProcess::Running)
        {
            break;
        }

    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);

    recon_static_functions::remove_safely_blank_dir_OR_file(script_file,Q_FUNC_INFO);

    recon_static_functions::app_debug("end " , Q_FUNC_INFO);

}

void thread_file_system::extract_file_system_regular()
{ // fill file_system.sqlite main database in case of image doesn't support by tsk or any other fuse. Then we use regular iterator to prepare the main file system database
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);


    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE");
    destination_db.setDatabaseName(":memory:");
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Database in memory open ---FAILED---" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error: " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_create_table(destination_db);

    QString db_create_command = recon_static_functions::get_filesystem_db_create_command("files");
    query_create_table.prepare(db_create_command);

    recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_reference(destination_db, Q_FUNC_INFO);

    if(!query_create_table.exec())
    {
        recon_static_functions::app_debug(" query memory table create ----FAILED----  "+ destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + db_create_command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(struct_target_info_ptr.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
    {
        extract_fs_info_and_fill_db_for_ios_uid(destination_db);
    }
    else if(struct_target_info_ptr.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        extract_fs_info_and_fill_db_for_ios_xml(destination_db);
    }
    //    else if(struct_target_info_ptr.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform)
    //    {
    //        extract_fs_info_and_fill_db_for_ios_uniform(destination_db);
    //    }
    else
    {
        //        extract_fs_info_and_fill_db_for_regular_source(destination_db);

        if(struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
        {
            extract_fs_info_and_fill_db_for_zip_source(destination_db);
        }
        else
        {
            extract_fs_info_and_fill_db_for_regular_source(destination_db);
        }

    }

    /// Attach from memory to destination
    QString fs_path = fs_database_path;
    if(QFileInfo(fs_database_path).exists())
        QFile::remove(fs_database_path);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(fs_path));
    QSqlQuery query_attach(destination_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
    }


    /// Save to destination from memory
    QSqlQuery query_save(destination_db);
    query_save.prepare(QString("CREATE TABLE filesystemDB.files AS SELECT * from files"));
    if(!query_save.exec())
    {
        recon_static_functions::app_debug(" query memory Save  --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
    }

    destination_db.close();


    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

}



void thread_file_system::extract_window_registry_files()
{ // extract window registry files if find these below path in the image or file system database after prepared
    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    bool_registry_files_extraction_finished = false;

    QStringList registry_file_path_list;
    QString initial_path = source_path;

    registry_file_path_list << initial_path + "Users/Default/NTUSER.DAT";

    registry_file_path_list << initial_path + "Users/DefaultAppPool/NTUSER.DAT";

    for(int ii = 0 ; ii < struct_target_info_ptr.system_user_QStringList.size() ; ii++)
    {
        registry_file_path_list << initial_path + struct_target_info_ptr.system_user_QStringList.at(ii) + "/NTUSER.DAT";
    }

    QStringList dir_list;
    dir_list << initial_path + "Windows/System32/config/"
             << initial_path + "Windows/ServiceProfiles/";

    for(int ii = 0; ii < dir_list.size();ii++)
    {
        QDirIterator dir_iter(dir_list.at(ii), QDirIterator::Subdirectories);
        qint64 counter = 0;
        while (dir_iter.hasNext())
        {
            counter++;

            if(counter % 100 == 0)
                QCoreApplication::processEvents();

            QString file_path = dir_iter.next();
            QFileInfo file_info(file_path);
            if(!file_info.exists())
                continue ;

            if(file_info.isFile())
            {
                if(recon_static_functions::check_is_it_registry_file_by_signature(file_path , Q_FUNC_INFO))
                {
                    registry_file_path_list << file_info.filePath();
                }
            }
        }
    }

    emit signal_add_registry_files(registry_file_path_list, source_count_name);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    qint64 counter = 0;
    while(1)
    {
        counter++;

        if(counter % 100 == 0)
        {
            counter = 0;
            QCoreApplication::processEvents();
        }


        if(bool_registry_files_extraction_finished)
            break;
    }

    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


    recon_static_functions::app_debug(" end", Q_FUNC_INFO);
}

void thread_file_system::extract_window_ram_images()
{ // extract window ram images if find in the image after preparation of file system database
    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    QString source_count_name = struct_target_info_ptr.source_count_name;
    QString root_count_name   = struct_target_info_ptr.root_count_name;

    QString start_point = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);
    QString fs_extraction_src_path = source_path;
    if(fs_extraction_src_path.endsWith("/"))
        fs_extraction_src_path.chop(QString("/").size());

    QStringList file_list;

    //    file_list << "/2018-May-22T12-20-13_ram_image.raw";
    //    file_list << "/suspect.vhd";
    //    file_list << "/2018-May-22T12-20-13.html";

    file_list << "/hiberfil.sys";
    file_list << "/pagefile.sys";
    file_list << "/swapfile.sys";

    for(int ii = 0; ii< file_list.size(); ii++)
    {
        QString display_path = file_list.at(ii);
        QString info_path = display_path;
        QString file_path = fs_extraction_src_path + display_path;
        QFileInfo info(file_path);
        if(!info.exists())
            continue;

        QString m_file_name = info.fileName();
        QString m_file_size = recon_static_functions::mining_size(info.filePath(), Q_FUNC_INFO);

        QString path_rtrvd_srcs_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString).toString() + "ram_images.sqlite";

        QString command_insert = ("insert into tbl_ram_list(bookmark, plugin_name, tab_name, source_type_display, source_type_internal, source_name, source_path, source_size,"
                                  "root_count_name, source_count_name,extraction_status_as_source,recon_filefrom,recon_file_infopath,is_nested,is_decompressed) values (?,?,?,?, ?,?,?,?,?,?,?, ?,?, ?,?)");

        QStringList val_list;
        val_list << QString::number(0)  << QString(MACRO_Plugin_Name_RAM_Images) << QString(MACRO_Plugin_RAM_Images_TAB_Name_Images)
                 << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Display << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal
                 << m_file_name << display_path << m_file_size
                 << root_count_name << source_count_name << QString::number(0) << start_point << info_path << "1" << "0";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert, val_list, path_rtrvd_srcs_db, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" end", Q_FUNC_INFO);

}




void thread_file_system::set_bool_cancel_extraction()
{
    //bool_cancel_extraction = false;
}


void thread_file_system::set_current_source_ios_uid_backup_parser_obj(ios_uid_backup_parser *obj)
{
    ios_uid_backup_parser_obj = obj;

    if(ios_uid_backup_parser_obj != NULL)
        ios_uid_backup_path_str = ios_uid_backup_parser_obj->ios_uid_backup_path;
}

void thread_file_system::pub_set_map_ios_uid_backup_parser_obj(QMap<QString, ios_uid_backup_parser*> *obj)
{
    map_ios_uid_backup_parser_obj = obj;
}

void thread_file_system::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}


void thread_file_system::pub_set_FS_source_selected_source_count_name_list(QStringList m_list)
{
    source_selected_source_count_name_list = m_list;
}


void thread_file_system::set_case_treewidget_obj(R_Treewidget *obj)
{
    case_treewidget_obj = obj;
}



void thread_file_system::retrieve_paths()
{ // set complete path in private variables to use further

    case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString src_cnt_name_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/";

    QDir m_dir;
    m_dir.mkpath(src_cnt_name_dir);


    fs_database_path =  src_cnt_name_dir + QString("file_system.sqlite");

    signature_database_path = src_cnt_name_dir + QString("signature.sqlite");

    mime_database_path = src_cnt_name_dir + QString("mime.sqlite");

    plugin_status_database_path = src_cnt_name_dir + QString("plugin_status.sqlite");

    hashes_database_path = src_cnt_name_dir + QString("hashes.sqlite");

    griffeye_data_database_path = src_cnt_name_dir + QString("griffeye_data.sqlite");

    exif_database_path = src_cnt_name_dir + QString("exif_metadata.sqlite");

    apple_metadata_database_path = src_cnt_name_dir + QString("apple_metadata.sqlite");

    apple_timestamps_database_path = src_cnt_name_dir + QString("apple_timestamps.sqlite");

    face_analysis_database_path = src_cnt_name_dir + QString("face_analysis.sqlite");

    optical_character_recognition_database_path = src_cnt_name_dir + QString("optical_character_recognition.sqlite");

    skin_tone_detection_database_path = src_cnt_name_dir + QString("skin_tone_detection.sqlite");

    weapons_database_path = src_cnt_name_dir + QString("weapons.sqlite");

    fire_analysis_database_path = src_cnt_name_dir + QString("fire_analysis.sqlite");

    video_thumbnails_database_path = src_cnt_name_dir + MACRO_Video_Thumbnails_Dir_Name_QString + QString("video_thumbnails.sqlite");

    QDir mdir;
    mdir.mkpath(src_cnt_name_dir + MACRO_Decompressed_Files_Dir_Name_QString);

    mdir.mkpath(src_cnt_name_dir + MACRO_Censored_Skin_Tone_Images_Dir_Name_QString);

    mdir.mkpath(src_cnt_name_dir + MACRO_Video_Thumbnails_Dir_Name_QString);
}



void thread_file_system::extract_fs_info_and_fill_db_for_ios_uid(QSqlDatabase destination_db)
{ // extract fs info and prepare main file system database for ios backups
    recon_static_functions::debug_intensive("Start " , Q_FUNC_INFO);


    if(ios_uid_backup_parser_obj->os_version_stripped == "7" || ios_uid_backup_parser_obj->os_version_stripped == "8" || ios_uid_backup_parser_obj->os_version_stripped == "9")
    {
        extract_fs_info_and_fill_db_for_ios_uid_7_8_9(destination_db);
    }
    else if(ios_uid_backup_parser_obj->os_version_stripped == "10" || ios_uid_backup_parser_obj->os_version_stripped == "11"
            || ios_uid_backup_parser_obj->os_version_stripped == "12" || ios_uid_backup_parser_obj->os_version_stripped == "13"
            || ios_uid_backup_parser_obj->os_version_stripped == "14" || ios_uid_backup_parser_obj->os_version_stripped == "15"
            || ios_uid_backup_parser_obj->os_version_stripped == "16")
    {
        extract_fs_info_and_fill_db_for_ios_uid_10_11(destination_db);
    }

    recon_static_functions::debug_intensive("End " , Q_FUNC_INFO);
}

void thread_file_system::extract_fs_info_and_fill_db_for_regular_source(QSqlDatabase destination_db)
{ // prepare file system database for regular sources which are not supported by tsk nor ios, uid, xml type sources.
    // Even if we add any unknown source or folder or some some images also in some cases, it's file system will be prepared by this function
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString init_path = source_path;
    if(init_path.endsWith(QString("/")) && init_path != QString("/"))
        init_path.chop(1);


    QSqlQuery query_insert(destination_db);

    if(QFileInfo(init_path).isDir())
    {
        QDirIterator dir_iter(init_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);

        int count = 0;
        while(dir_iter.hasNext())
        {
            count++;
            if(count % 100 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            insert_regular_source_details_in_filesystem_db(dir_iter.next().trimmed() , &query_insert);

        }
    }
    else
    {

        insert_regular_source_details_in_filesystem_db(init_path , &query_insert);
    }


    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}


void thread_file_system::insert_regular_source_details_in_filesystem_db(QString file_path , QSqlQuery *query_insert)
{ // prepare file system for regular sources like any unknown source, folder, file etc or some images also in some cases

    QString fs_extraction_src_path = struct_target_info_ptr.fs_extraction_source_path;
    QString source_count_name = struct_target_info_ptr.source_count_name;
    QString src_type_intrnl = struct_target_info_ptr.source_type_internal;
    QString os_schme_intrnl = struct_target_info_ptr.os_scheme_internal;
    QFileInfo info(file_path);
    if(info.fileName() == QString(".") || info.fileName() == QString(".."))
        return;

    QString is_dir = "0";
    if(info.isDir())
        is_dir = "1";

    bzero(&fileStat,sizeof(fileStat));
    lstat(file_path.toLocal8Bit().data(),&fileStat);      // File Info using stat


    QString file_parent_path = info.dir().absolutePath();

    QString inode_number_str;
    if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)

    {
        inode_number_str = QString::number(fileStat.st_ino);
    }


    if(!fs_extraction_src_path.isEmpty() && fs_extraction_src_path != QString("/"))
    {
        file_path = file_path.remove(file_path.indexOf(fs_extraction_src_path) , fs_extraction_src_path.size());
        if(!file_path.startsWith("/"))
            file_path.prepend("/");

        if(file_parent_path.endsWith("/"))
            file_parent_path.chop(1);

        if(fs_extraction_src_path.endsWith("/"))
            fs_extraction_src_path.chop(1);

        file_parent_path = file_parent_path.remove(file_parent_path.indexOf(fs_extraction_src_path) , fs_extraction_src_path.size());
        if(!file_parent_path.startsWith("/"))
            file_parent_path.prepend("/");
    }


    //    if(file_path == "/MDFJDFDF3454JKK89K"
    //            && (src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal)
    //                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)))
    //    {
    //        return;
    //    }


    ///check for other source types (Mobile Backup/Disk Images)
    if(!info.isDir())
        check_for_other_sources(file_path, file_path);


    QString change_timestamp_str ,modification_timestamp_str,last_read_timestamp_str,created_timestamp_str;

    if(os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
            || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
            || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal))
    {
        if(src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
                || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal))

        {
            qint64 change_timestamp     = fileStat.st_ctimespec.tv_sec;
            qint64 modification_timestamp = fileStat.st_mtimespec.tv_sec;
            qint64 last_read_timestamp    = fileStat.st_atimespec.tv_sec;
            qint64 created_timestamp        = fileStat.st_birthtimespec.tv_sec;

            change_timestamp_str = QString::number(change_timestamp);
            modification_timestamp_str = QString::number(modification_timestamp);
            last_read_timestamp_str = QString::number(last_read_timestamp);
            created_timestamp_str = QString::number(created_timestamp);
        }
        else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)

        {
            if(map_key_filepath_value_recon_logical_image_timestamp.contains(file_path))
            {

                struct_recon_logical_image_posix_timestamp recon_logical_info_obj = map_key_filepath_value_recon_logical_image_timestamp.value(file_path);

                qint64 change_timestamp = recon_logical_info_obj.change_timestamp_qint64;
                qint64 modification_timestamp = recon_logical_info_obj.modify_timestamp_qint64;
                qint64 last_read_timestamp = recon_logical_info_obj.access_timestamp_qint64;
                qint64 created_timestamp = recon_logical_info_obj.create_timestamp_qint64;

                change_timestamp_str = QString::number(change_timestamp);
                modification_timestamp_str = QString::number(modification_timestamp);
                last_read_timestamp_str = QString::number(last_read_timestamp);
                created_timestamp_str = QString::number(created_timestamp);
            }
        }
    }


    QString prmsn_restrict = "0";

    if(recon_helper_standard_obj->bool_is_permission_restricted_file(file_path, Q_FUNC_INFO))
        return;


    QString msize = QString::number(info.size());
    if(is_dir == "1")
    {
        msize.clear();
    }

    query_insert->prepare("insert into files (bookmark,filename, filepath, file_size,extension,file_parent_path,"
                          "z_date_time_change,z_date_time_accessed,z_date_time_modified,z_date_time_created,"
                          "source_count_name,inode_no,extended_attribute_update_status,source_file,exif_data_update_status,permission_restricted_file, is_dir,"
                          "decompression_status, recon_filefrom, recon_file_infopath,"
                          "fs_module_apple_metadata_run_status,fs_module_exif_metadata_run_status,fs_module_signature_analysis_run_status,"
                          "fs_module_mime_type_analysis_run_status,fs_module_hashset_run_status,fs_module_face_analysis_run_status,"
                          "fs_module_optical_character_recognition_run_status, fs_module_weapons_run_status, fs_module_skin_tone_detection_run_status,"
                          "fs_module_fire_analysis_run_status) "
                          "values(0,?,?,?,?,?, ?,?,?,?,? ,?,?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?, ?,?,?)");

    query_insert->addBindValue(info.fileName());
    query_insert->addBindValue(file_path);
    query_insert->addBindValue(msize);
    query_insert->addBindValue(recon_helper_standard_obj->mining_suffix(file_path, Q_FUNC_INFO));
    query_insert->addBindValue(file_parent_path);
    query_insert->addBindValue(change_timestamp_str);
    query_insert->addBindValue(last_read_timestamp_str);
    query_insert->addBindValue(modification_timestamp_str);
    query_insert->addBindValue(created_timestamp_str);
    query_insert->addBindValue(source_count_name);
    query_insert->addBindValue(inode_number_str);
    query_insert->addBindValue(QString::number(0));
    query_insert->addBindValue("");
    query_insert->addBindValue(QString::number(0));
    query_insert->addBindValue(prmsn_restrict);
    query_insert->addBindValue(is_dir);
    query_insert->addBindValue(QString::number(0));
    query_insert->addBindValue(QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main));
    query_insert->addBindValue(file_path);

    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));


    if(!query_insert->exec())
    {
        recon_static_functions::app_debug(" insert query  --------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query_insert->executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert->lastError().text(),Q_FUNC_INFO);
    }

    total_records_fs++;

    if(total_records_fs % 5000 == 0)
    {
        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, struct_target_info_ptr.source_name, false, total_records_fs, -1, true);
    }

}

void thread_file_system::extract_fs_info_and_fill_db_for_zip_source(QSqlDatabase destination_db)
{ // prepare file system database for zip sources like graykey ios backup and graykey android backup

    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString init_path = source_path;
    if(init_path.endsWith(QString("/")) && init_path != QString("/"))
        init_path.chop(1);


    QSqlQuery query_insert(destination_db);

    QDirIterator dir_iter(init_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    int count = 0;
    while(dir_iter.hasNext())
    {
        count++;
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
            count = 0;
        }

        //        insert_uniform_source_details_in_filesystem_db(dir_iter.next().trimmed() , &query_insert);
        insert_zip_source_details_in_filesystem_db(dir_iter.next().trimmed() , &query_insert);

    }

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);
}

void thread_file_system::insert_zip_source_details_in_filesystem_db(QString file_path , QSqlQuery *query_insert)
{ // prepare file system databsae and run iterator on zip type sources like graykey

    QString fs_extraction_src_path = struct_target_info_ptr.fs_extraction_source_path;
    QString source_count_name = struct_target_info_ptr.source_count_name;
    QString zip_fuse_mount_path = struct_target_info_ptr.fuse_mount_path_zip;
    QString vrtl_src_path = struct_target_info_ptr.virtual_source_path;

    QFileInfo info(file_path);
    if(info.fileName() == QString(".") || info.fileName() == QString(".."))
        return;

    QString is_dir = "0";
    if(info.isDir())
        is_dir = "1";

    QString actual_file_path = file_path.mid(vrtl_src_path.size(), file_path.size());
    QString file_path_by_fuse = recon_static_functions::get_complete_file_path(zip_fuse_mount_path,actual_file_path, Q_FUNC_INFO);

    QString file_parent_path = info.dir().absolutePath();

    if(!fs_extraction_src_path.isEmpty() && fs_extraction_src_path != QString("/"))
    {
        file_path = file_path.remove(file_path.indexOf(fs_extraction_src_path) , fs_extraction_src_path.size());
        if(!file_path.startsWith("/"))
            file_path.prepend("/");

        if(file_parent_path.endsWith("/"))
            file_parent_path.chop(1);

        if(fs_extraction_src_path.endsWith("/"))
            fs_extraction_src_path.chop(1);

        file_parent_path = file_parent_path.remove(file_parent_path.indexOf(fs_extraction_src_path) , fs_extraction_src_path.size());
        if(!file_parent_path.startsWith("/"))
            file_parent_path.prepend("/");
    }

    QString change_timestamp_str ,modification_timestamp_str,last_read_timestamp_str,creation_timestamp_str;

    qint64 change_timestamp     = 0;
    qint64 modification_timestamp = 0;
    qint64 last_read_timestamp    = 0;
    qint64 created_timestamp = 0;

    // Timestamp is extracted from the fuse mount point and other informations are extracted from the decompressed backup location.
    // existence is checked because if fuse is not mounted then the timestams must be zero.
    QFileInfo file_info(file_path_by_fuse);
    if(file_info.exists() && !zip_fuse_mount_path.isEmpty())
    {
        bzero(&fileStat,sizeof(fileStat));
        lstat(file_path_by_fuse.toLocal8Bit().data(),&fileStat);      // File Info using stat

        change_timestamp     = fileStat.st_ctimespec.tv_sec;
        modification_timestamp = fileStat.st_mtimespec.tv_sec;
        last_read_timestamp    = fileStat.st_atimespec.tv_sec;
        created_timestamp = fileStat.st_birthtimespec.tv_sec;
    }

    change_timestamp_str = QString::number(change_timestamp);
    modification_timestamp_str = QString::number(modification_timestamp);
    last_read_timestamp_str = QString::number(last_read_timestamp);
    creation_timestamp_str = QString::number(created_timestamp);

    QString prmsn_restrict = "0";

    if(recon_helper_standard_obj->bool_is_permission_restricted_file(file_path, Q_FUNC_INFO))
        return;


    QString msize = QString::number(info.size());
    if(is_dir == "1")
    {
        msize.clear();
    }

    query_insert->prepare("insert into files (bookmark,filename, filepath, file_size,extension,file_parent_path,"
                          "z_date_time_change,z_date_time_accessed,z_date_time_modified,z_date_time_created,"
                          "source_count_name,inode_no,extended_attribute_update_status,source_file,exif_data_update_status,permission_restricted_file, is_dir,"
                          "decompression_status, recon_filefrom, recon_file_infopath,"
                          "fs_module_apple_metadata_run_status,fs_module_exif_metadata_run_status,fs_module_signature_analysis_run_status,"
                          "fs_module_mime_type_analysis_run_status,fs_module_hashset_run_status,fs_module_face_analysis_run_status,"
                          "fs_module_optical_character_recognition_run_status, fs_module_weapons_run_status, fs_module_skin_tone_detection_run_status,"
                          "fs_module_fire_analysis_run_status) "
                          "values(0,?,?,?,?,?, ?,?,?,?,? ,?,?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?, ?,?,?)");

    query_insert->addBindValue(info.fileName());
    query_insert->addBindValue(file_path);
    query_insert->addBindValue(msize);
    query_insert->addBindValue(recon_helper_standard_obj->mining_suffix(file_path, Q_FUNC_INFO));
    query_insert->addBindValue(file_parent_path);
    query_insert->addBindValue(change_timestamp_str);
    query_insert->addBindValue(last_read_timestamp_str);
    query_insert->addBindValue(modification_timestamp_str);
    query_insert->addBindValue(creation_timestamp_str);
    query_insert->addBindValue(source_count_name);
    query_insert->addBindValue(""); //inode number
    query_insert->addBindValue(QString::number(0));
    query_insert->addBindValue("");
    query_insert->addBindValue(QString::number(0));
    query_insert->addBindValue(prmsn_restrict);
    query_insert->addBindValue(is_dir);
    query_insert->addBindValue(QString::number(0));
    query_insert->addBindValue(QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main));
    query_insert->addBindValue(file_path);

    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));
    query_insert->addBindValue(QString("0"));


    if(!query_insert->exec())
    {
        recon_static_functions::app_debug(" insert query  --------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query_insert->executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert->lastError().text(),Q_FUNC_INFO);
    }

    total_records_fs++;

    if(total_records_fs % 5000 == 0)
    {
        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, struct_target_info_ptr.source_name, false, total_records_fs, -1, true);
    }

}

void thread_file_system::fill_struct_dir_list_for_ios_xml_backup()
{ // fill structure directory list of backup information in case of xml backup sources like ufdr
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString ios_xml_bkup_path = struct_target_info_ptr.virtual_source_path;

    ios_xml_bkup_path = ios_xml_bkup_path.trimmed();

    if(ios_xml_bkup_path.isEmpty())
    {
        recon_static_functions::app_debug("iOS XML Backup source path empty -----FAILED-----" + ios_xml_bkup_path , Q_FUNC_INFO);
        return;
    }

    if(!ios_xml_bkup_path.endsWith("/"))
        ios_xml_bkup_path.append("/");

    QString report_xml_file_path = ios_xml_bkup_path + QString("report.xml");

    if(!QFileInfo(report_xml_file_path).exists())
    {
        recon_static_functions::app_debug("iOS XML Backup report.xml file not exist -----FAILED-----" + report_xml_file_path , Q_FUNC_INFO);
        return;
    }

    ios_xml_backup_path_str = ios_xml_bkup_path;

    struct_ios_xml_files_info_qlist.clear();
    ios_xml_backup_parser *ios_xml_backup_parser_obj = new ios_xml_backup_parser(this);
    ios_xml_backup_parser_obj->pub_set_ios_xml_file_path(report_xml_file_path);
    ios_xml_backup_parser_obj->pub_parse_ios_xml_files_info();
    struct_ios_xml_files_info_qlist = ios_xml_backup_parser_obj->pub_get_struct_files_info_qlist();

    if(struct_ios_xml_files_info_qlist.size() <= 0)
    {
        recon_static_functions::app_debug("Files info do not exist -----FAILED-----" + QString::number(struct_ios_xml_files_info_qlist.size()) , Q_FUNC_INFO);
        return;
    }

    for(int pp = 0; pp < struct_ios_xml_files_info_qlist.size(); pp++)
    {
        struct_ios_xml_files_info struct_ios_xml_files_info_obj = struct_ios_xml_files_info_qlist.at(pp);

        QString domain_path = struct_ios_xml_files_info_obj.domain_path;
        QFileInfo info(domain_path);
        QStringList dirs_path_list = info.absolutePath().split("/");
        dirs_path_list.removeAll("");

        QString ios_xml_temp_str;
        if(!dirs_path_list.isEmpty())
        {
            for(int pp = 0; pp < dirs_path_list.size() ; pp ++)
            {
                QString filename = dirs_path_list.at(pp).trimmed();

                if(filename.isEmpty())
                    continue;

                ios_xml_temp_str.append(filename);
                ios_xml_temp_str.append("/");

                QString fullpath = ios_xml_temp_str;

                if(fullpath.trimmed().isEmpty())
                    continue;

                if(!fullpath.startsWith("/"))
                    fullpath.prepend("/");

                if(fullpath.endsWith("/"))
                    fullpath.chop(1);

                QFileInfo info_file(fullpath);

                bool bool_dir_path_already_inserted = false;
                for (int qq = 0 ; qq < list_struct_fs_dir_info.size() ; qq ++)
                {
                    // dir_path = list_struct_fs_dir_info.at(qq).dir_path;
                    QString dir_name = list_struct_fs_dir_info.at(qq).dir_name;

                    // if(fullpath.startsWith(dir_path))
                    // {
                    QString temp_str = fullpath;
                    QStringList dirs_list = temp_str.split("/");
                    dirs_list.removeAll("");

                    QString parent_dir_name;
                    if(dirs_list.size() > 0)
                    {
                        parent_dir_name = dirs_list.at(0);

                        if(parent_dir_name.trimmed() == dir_name)
                        {
                            bool_dir_path_already_inserted = true;
                            break;
                        }
                    }

                    // }
                }

                if(bool_dir_path_already_inserted)
                    continue;

                struct_global_fs_extracted_dir_info struct_info;
                struct_info.dir_path = fullpath;
                struct_info.dir_name = info_file.fileName();

                list_struct_fs_dir_info.append(struct_info);
            }
        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void thread_file_system::extract_fs_info_and_fill_db_for_ios_xml(QSqlDatabase destination_db)
{ // prepare file_system.sqlite main database for xml sources like cellebrite ufdr backup
    recon_static_functions::debug_intensive("Start " , Q_FUNC_INFO);

    //------------------------ Inserting root level files i.e. report.xml etc. - Starts ---------------------------//

    QDirIterator iter(ios_xml_backup_path_str , QDirIterator::NoIteratorFlags);
    while(iter.hasNext())
    {
        QFileInfo minfo(iter.next());

        if(!minfo.isFile())
            continue;

        QString filename = minfo.fileName();
        QString filepath = minfo.filePath();
        QString m_flag = "1"; // for file
        QString preview_id_path,file_size;

        QFile check_id_exist(filepath);
        if(check_id_exist.exists())
        {
            preview_id_path = filepath;

            if(!preview_id_path.isEmpty())
            {
                if(preview_id_path.startsWith(ios_xml_backup_path_str))
                    preview_id_path.remove(0,ios_xml_backup_path_str.size());

                if(!preview_id_path.startsWith("/"))
                    preview_id_path.prepend("/");
            }

            bzero(&fileStat,sizeof(fileStat));
            lstat(filepath.toLocal8Bit().data(),&fileStat);       // File Info using stat
            file_size = QString::number(fileStat.st_size);
        }

        QString file_id = filename;
        QString display_file_path = preview_id_path;

        qint64 change_timestamp       = fileStat.st_ctimespec.tv_sec;
        qint64 modification_timestamp   = fileStat.st_mtimespec.tv_sec;
        qint64 last_read_timestamp      = fileStat.st_atimespec.tv_sec;
        qint64 creation_timestamp = fileStat.st_birthtimespec.tv_sec;

        struct_ios_xml_tagged_files_timestamp_info timestamp_info_obj;
        timestamp_info_obj.change_timestamp_numeric_qint64  = change_timestamp;
        timestamp_info_obj.modify_timestamp_numeric_qint64    = modification_timestamp;
        timestamp_info_obj.access_timestamp_numeric_qint64    = last_read_timestamp;
        timestamp_info_obj.create_timestamp_numeric_qint64     = creation_timestamp;

        insert_ios_xml_details_in_filesystem_db(destination_db ,preview_id_path, display_file_path, file_size , file_id, m_flag , timestamp_info_obj);
    }


    //------------------------ Inserting root level files i.e. report.xml etc. - Ends ---------------------------//

    for(int ii = 0 ; ii < struct_ios_xml_files_info_qlist.size() ; ii++)
    {
        struct_ios_xml_files_info struct_ios_xml_files_info_obj = struct_ios_xml_files_info_qlist.at(ii);

        QString domain_path = struct_ios_xml_files_info_obj.domain_path;

        bool domain_orphan_file = false;
        if(domain_path.endsWith("/"))
        {
            domain_orphan_file = true;
        }

        //------------------------Forcefully set parent as root for orphan files - Starts ----------------------//
        //--- like domain path = /Instagram/ but id_path = /Files/Images/abc.jpg
        //--- then abc.jpg will be displayed under root(/) instead of /Instagram/
        //--- took hint from Cellebrite Reader Application ---//

        if(domain_orphan_file)
        {
            struct_ios_xml_tagged_files_metadata_section_file_info metadata_files_info_obj = struct_ios_xml_files_info_obj.struct_tagged_files_metadata_section_file_info_obj;
            QString id_path;

            id_path = metadata_files_info_obj.preview_file_path;

            QFileInfo info_id(id_path);
            QString filename = info_id.fileName();

            QString filepath = QString("/" + filename);
            QString m_flag = "1";  // for files

            QString select_command = QString("select count(*) from files where fileID = ?");
            QStringList values ;
            values << struct_ios_xml_files_info_obj.file_id;

            QString count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_command , values, 0 ,destination_db,Q_FUNC_INFO);

            qint64 cnt_qint64 = count.toLongLong();

            if(cnt_qint64 <= 0)
            {
                struct_ios_xml_tagged_files_timestamp_info timestamp_info_obj;
                timestamp_info_obj.change_timestamp_numeric_qint64  = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.change_timestamp_numeric_qint64;
                timestamp_info_obj.modify_timestamp_numeric_qint64    = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.modify_timestamp_numeric_qint64;
                timestamp_info_obj.access_timestamp_numeric_qint64    = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.access_timestamp_numeric_qint64;
                timestamp_info_obj.create_timestamp_numeric_qint64     = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.create_timestamp_numeric_qint64;

                insert_ios_xml_details_in_filesystem_db(destination_db ,id_path, filepath, QString::number(struct_ios_xml_files_info_obj.size_byte) , struct_ios_xml_files_info_obj.file_id, m_flag , timestamp_info_obj);

            }
        }

        //------------------------Forcefully set parent as root for orphan files - Ends ----------------------//

        QFileInfo info(domain_path);
        QStringList dirs_path_list = info.absolutePath().split("/");
        dirs_path_list.removeAll("");

        //------------------------insert record for Dirs only - Starts---------------------------//

        QString ios_xml_temp_str;
        if(!dirs_path_list.isEmpty())
        {
            for(int pp = 0; pp < dirs_path_list.size() ; pp ++)
            {
                QString filename = dirs_path_list.at(pp).trimmed();
                QString m_flag = "2"; // for directory

                if(filename.isEmpty())
                {
                    ios_xml_temp_str.append("/");
                    continue;
                }

                ios_xml_temp_str.append(filename);
                ios_xml_temp_str.append("/");

                QString filepath = ios_xml_temp_str;
                if(!filepath.startsWith("/"))
                    filepath.prepend("/");

                if(filepath.endsWith("/"))
                    filepath.chop(1);

                QString select_command = QString("select count(*) from files where filepath = ?");
                QStringList values ;
                values << filepath;

                QString count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_command , values, 0 ,destination_db,Q_FUNC_INFO);

                qint64 cnt_qint64 = count.toLongLong();

                if(cnt_qint64 <= 0)
                {
                    struct_ios_xml_tagged_files_timestamp_info timestamp_info_obj;
                    timestamp_info_obj.change_timestamp_numeric_qint64  = 0;
                    timestamp_info_obj.modify_timestamp_numeric_qint64    = 0;
                    timestamp_info_obj.access_timestamp_numeric_qint64    = 0;
                    timestamp_info_obj.create_timestamp_numeric_qint64     = 0;
                    insert_ios_xml_details_in_filesystem_db(destination_db ,"", filepath, "" , "", m_flag ,timestamp_info_obj);

                }
            }
        }

        //------------------------insert record for Dirs only - Ends---------------------------//



        //------------------------insert record for files only - Starts---------------------------//

        if(!info.fileName().trimmed().isEmpty())
        {
            QString filename = info.fileName();
            QString filepath = info.filePath();
            QString m_flag = "1";  // for files

            QString select_command = QString("select count(*) from files where fileID = ?");
            QStringList values ;
            values << struct_ios_xml_files_info_obj.file_id;

            QString count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_command , values, 0 ,destination_db,Q_FUNC_INFO);

            qint64 cnt_qint64 = count.toLongLong();

            if(cnt_qint64 <= 0)
            {
                struct_ios_xml_tagged_files_metadata_section_file_info metadata_files_info_obj = struct_ios_xml_files_info_obj.struct_tagged_files_metadata_section_file_info_obj;
                QString id_path;

                id_path = metadata_files_info_obj.preview_file_path;

                struct_ios_xml_tagged_files_timestamp_info timestamp_info_obj;
                timestamp_info_obj.change_timestamp_numeric_qint64  = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.change_timestamp_numeric_qint64;
                timestamp_info_obj.modify_timestamp_numeric_qint64    = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.modify_timestamp_numeric_qint64;
                timestamp_info_obj.access_timestamp_numeric_qint64    = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.access_timestamp_numeric_qint64;
                timestamp_info_obj.create_timestamp_numeric_qint64     = struct_ios_xml_files_info_obj.struct_tagged_files_timestamp_info_obj.create_timestamp_numeric_qint64;

                insert_ios_xml_details_in_filesystem_db(destination_db ,id_path, filepath, QString::number(struct_ios_xml_files_info_obj.size_byte) , struct_ios_xml_files_info_obj.file_id, m_flag ,timestamp_info_obj);

            }

        }

        //------------------------insert record for files only - Ends---------------------------//

    }


    recon_static_functions::debug_intensive("End " , Q_FUNC_INFO);
}

void thread_file_system::insert_ios_xml_details_in_filesystem_db(QSqlDatabase destination_db, QString id_path, QString display_file_path ,QString file_size, QString file_id, QString m_flag , struct_ios_xml_tagged_files_timestamp_info obj)
{ // insertion starts for xml backups in case of cellebrite ios ufdr backups
    recon_static_functions::debug_intensive("Start ", Q_FUNC_INFO);

    QSqlQuery insert_into_destination(destination_db);

    if(!display_file_path.startsWith("/"))
        display_file_path.prepend("/");

    if(display_file_path.endsWith("/"))
        display_file_path.chop(1);

    QFileInfo info(display_file_path);

    QString preview_id_path = id_path;

    QString is_dir = "0";

    if(m_flag == "2")
        is_dir = "1";

    if(is_dir == "1")
    {
        file_size.clear();
    }

    struct_ios_xml_tagged_files_timestamp_info access_info_obj = obj;

    QString  change_timestamp_str = QString::number(access_info_obj.change_timestamp_numeric_qint64);
    QString  modification_timestamp_str = QString::number(access_info_obj.modify_timestamp_numeric_qint64);
    QString  last_read_timestamp_str = QString::number(access_info_obj.access_timestamp_numeric_qint64);
    QString creation_timestamp_str = QString::number(access_info_obj.create_timestamp_numeric_qint64);


    insert_into_destination.prepare("INSERT INTO files(bookmark, filename, filepath, extension, file_parent_path, file_size, source_count_name,"
                                    "fileID, id_path, flags, permission_restricted_file, is_dir, decompression_status, recon_filefrom,"
                                    "recon_file_infopath, z_date_time_change, z_date_time_accessed,"
                                    "z_date_time_modified, z_date_time_created, fs_module_apple_metadata_run_status, fs_module_exif_metadata_run_status,"
                                    "fs_module_signature_analysis_run_status, fs_module_mime_type_analysis_run_status,fs_module_hashset_run_status,"
                                    "fs_module_face_analysis_run_status, fs_module_optical_character_recognition_run_status,fs_module_weapons_run_status,"
                                    "fs_module_skin_tone_detection_run_status, fs_module_fire_analysis_run_status)"
                                    " VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)");


    insert_into_destination.addBindValue("0");
    insert_into_destination.addBindValue(info.fileName());
    insert_into_destination.addBindValue(display_file_path);
    insert_into_destination.addBindValue(recon_helper_standard_obj->mining_suffix(preview_id_path, Q_FUNC_INFO));
    insert_into_destination.addBindValue(info.absolutePath());

    insert_into_destination.addBindValue(file_size);
    insert_into_destination.addBindValue(source_count_name);
    insert_into_destination.addBindValue(file_id);
    insert_into_destination.addBindValue(preview_id_path);
    insert_into_destination.addBindValue(m_flag);

    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(is_dir);
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main));
    insert_into_destination.addBindValue(preview_id_path);

    insert_into_destination.addBindValue(change_timestamp_str);
    insert_into_destination.addBindValue(last_read_timestamp_str);
    insert_into_destination.addBindValue(modification_timestamp_str);
    insert_into_destination.addBindValue(creation_timestamp_str);
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));


    if(!insert_into_destination.exec())
    {
        recon_static_functions::app_debug(" query insert_into_destination ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error- " + insert_into_destination.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" QUERY- " + insert_into_destination.executedQuery(),Q_FUNC_INFO);
    }

    total_records_fs++;
    if(total_records_fs % 100 == 0)
    {
        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, struct_target_info_ptr.source_name, false, total_records_fs, -1, true);
    }

}



void thread_file_system::create_mime_signature_exif_apple_metadata_and_necessary_path()
{ // create some by default databases for file system modules featuees

    if(!QFileInfo(fs_database_path).exists())
    {
        QString fs_db_command = recon_static_functions::get_filesystem_db_create_command("files");
        recon_helper_standard_obj->execute_db_command_by_dbpath(fs_db_command, fs_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(fs_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(hashes_database_path).exists())
    {
        QString command_hashes = QString("create table files (INT INTEGER PRIMARY KEY, filesystem_record INTEGER, md5_hash varchar(200), sha1_hash varchar(200))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_hashes, hashes_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(hashes_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(signature_database_path).exists())
    {
        QString command_sign = QString("create table files (bookmark INTEGER, filesystem_record INTEGER,signature_name varchar(200), signature_value varchar(200))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_sign, signature_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(signature_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(mime_database_path).exists())
    {
        QString command_mime = QString("create table files (bookmark INTEGER, filesystem_record INTEGER,mime_type varchar(200))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_mime, mime_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(mime_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(exif_database_path).exists())
    {
        QString command_exif = QString("create table files (filesystem_record INTEGER,exif_data varchar(1024),make varchar(500),model varchar(500),author varchar(500),latitude INTEGER,longitude INTEGER,"
                                       " item1 varchar(500), item2 varchar(500), item3 varchar(500), item4 varchar(500), item5 varchar(500), item6 varchar(500), item7 varchar(500), item8 varchar(500), item9 varchar(500), item10 varchar(500))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_exif, exif_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(exif_database_path, Q_FUNC_INFO);
    }


    if(!QFileInfo(apple_timestamps_database_path).exists())
    {
        QString command_apl_timestmp = QString("create table files (INT INTEGER PRIMARY KEY, filesystem_record INTEGER, useddates varchar(200), usecount varchar(200), last_useddate INTEGER, content_creation_date INTEGER, content_modification_date INTEGER, date_added INTEGER)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_apl_timestmp, apple_timestamps_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(apple_timestamps_database_path, Q_FUNC_INFO);
    }


    if(!QFileInfo(plugin_status_database_path).exists())
    {
        QString command_plugin_status = QString("create table plugin_status ( plugin_name varchar(100), tab_name varchar(200), is_completed INTEGER)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_plugin_status, plugin_status_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(plugin_status_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(apple_metadata_database_path).exists())
    {
        QString command_apple_metadata = create_apple_metadata_db_table_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_apple_metadata, apple_metadata_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(apple_metadata_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(face_analysis_database_path).exists())
    {
        QString command_face_analysis = QString("create table files (filesystem_record INTEGER)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_face_analysis, face_analysis_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(face_analysis_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(optical_character_recognition_database_path).exists())
    {
        QString command_ocr = QString("create table files (filesystem_record INTEGER, ocr_text varchar(5096))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_ocr, optical_character_recognition_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(optical_character_recognition_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(skin_tone_detection_database_path).exists())
    {
        QString command_skin_tone_detection = QString("create table files (filesystem_record INTEGER, mime varchar(100))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_skin_tone_detection, skin_tone_detection_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(skin_tone_detection_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(weapons_database_path).exists())
    {
        QString command_weapons = QString("create table files (filesystem_record INTEGER , weapon_type varchar(100))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_weapons, weapons_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(weapons_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(fire_analysis_database_path).exists())
    {
        QString command_fire_analysis = QString("create table files (filesystem_record INTEGER )");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_fire_analysis, fire_analysis_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(fire_analysis_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(video_thumbnails_database_path).exists())
    {
        QString command_video_thumbnails = QString("create table files (filesystem_record INTEGER PRIMARY KEY , thumb_filepath varchar(1024))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_video_thumbnails, video_thumbnails_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(video_thumbnails_database_path, Q_FUNC_INFO);
    }

    if(!QFileInfo(griffeye_data_database_path).exists())
    {
        QString command_griffeye_data = QString("Create table files (filesystem_record INTEGER PRIMARY KEY, md5_hash varchar(300), griffeye_category varchar(200))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_griffeye_data, griffeye_data_database_path, Q_FUNC_INFO);
        recon_helper_standard_obj->pub_change_database_mode_to_wal_by_db_path(griffeye_data_database_path, Q_FUNC_INFO);
    }

}

QString thread_file_system::create_apple_metadata_db_table_command()
{ // create by default apple metadata database table, it's columns we extracts from apple_attributes_list.csv and then prepare final command to create columns of apple metadata
    QString command_str = "CREATE TABLE files (filesystem_record INTEGER";

    QString apl_attributes_csv_path = QString("../Resources/apple_attributes/apple_attributes_list.csv");
    // apl_attributes_csv_path = recon_static_functions::decrypt_library(apl_attributes_csv_path, Q_FUNC_INFO);

    QFile file(apl_attributes_csv_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open-------FAILED------ " + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error :" + file.errorString() , Q_FUNC_INFO);
        // file.remove();
        return QString("");
    }

    file.readLine(); // Column Title Line
    int count = 0;

    while(!file.atEnd())
    {
        count++;

        if(count%10 == 0)
            QCoreApplication::processEvents();

        QString line = file.readLine();

        if(line.endsWith("\n"))
            line.chop(1);
        if(line.endsWith(",,,"))
            line.chop(3);

        QStringList list = line.split(",,,", Qt::SkipEmptyParts);

        QString attribute_name, description, widget_type, kmditem_command,command_type;

        if(list.size() > 0)
        {
            attribute_name = list.at(0);
            attribute_name.remove("\"");
        }
        if(list.size() > 1)
        {
            description = list.at(1);
            description.remove("\"");
        }
        if(list.size() > 2)
        {
            widget_type = list.at(2);
            widget_type.remove("\"");
        }
        if(list.size() > 3)
        {
            kmditem_command = list.at(3);
            kmditem_command.remove("\"");
        }

        if(list.size() > 4)
        {
            command_type = list.at(4);
            command_type.remove("\"");
        }

        kmditem_command = kmditem_command.trimmed();
        if(kmditem_command == QString("TYPE YOUR OWN QUERY"))
            continue;

        QString attribute_str = attribute_name;
        QString column_name = kmditem_command + "_" + attribute_str.replace(" ", "_");

        if(widget_type == QString::number(MACRO_Apple_Attributes_Widget_Type_REAL_VALUE_int)
                || widget_type == QString::number(MACRO_Apple_Attributes_Widget_Type_SECONDS_VALUE_int)) // for value in flot e.g. 20.05 secods
        {
            command_str.append(QString(",") + column_name + QString(" REAL"));
        }
        else if(widget_type == QString::number(MACRO_Apple_Attributes_Widget_Type_Timestamp_int)) // for timestamp type
        {
            command_str.append(QString(",") + column_name + QString(" INTEGER"));
        }
        else
        {
            command_str.append(QString(",") + column_name + QString(" VARCHAR(200)"));
        }

        if(command_str.endsWith(","))
            command_str.chop(1);

    }
    file.close();
    command_str.append(")");

    // file.remove();

    return command_str;

}


void thread_file_system::extract_fs_info_and_fill_db_for_ios_uid_7_8_9(QSqlDatabase destination_db)
{ // extract and prepare main database file_system.sqlite in case of iOS backup 7,8,9
    recon_static_functions::debug_intensive(" start " , Q_FUNC_INFO);

    // inserting    "Info.plist", "Manifest.plist" etc. static files
    QStringList static_file_list;
    static_file_list << "Info.plist" << "Manifest.mbdb" << "Manifest.plist" << "Status.plist";
    for(int yy = 0; yy < static_file_list.size(); yy++)
    {
        QString m_id = static_file_list.at(yy);
        QString m_id_path = ios_uid_backup_path_str  + "/" + m_id;
        if(!QFileInfo(m_id_path).exists())
            continue;

        QString m_flag = "1"; // for file
        QString m_dmn_path =  m_id;

        insert_ios_uid_details_in_filesystem_db(destination_db, m_id_path, m_dmn_path, m_id, m_flag);
    }


    for(int zz = 0; zz < ios_uid_backup_parser_obj->list_node_entry_for_ios_7_8_9.size(); zz++)
    {

        struct_global_main_node_entry_for_ios_7_8_9 dmn_obj = ios_uid_backup_parser_obj->list_node_entry_for_ios_7_8_9.at(zz);

        QString m_dmn_path =  dmn_obj.node_domain;
        if(m_dmn_path.trimmed().isEmpty())
            continue;

        QString m_id = dmn_obj.node_fileID;
        QString m_id_path = ios_uid_backup_path_str  + "/" + m_id;
        QString m_flag = dmn_obj.flags;

        insert_ios_uid_details_in_filesystem_db(destination_db, m_id_path, m_dmn_path, m_id, m_flag);


        for(int pp = 0; pp < dmn_obj.list_subdomain.size(); pp++)
        {
            struct_global_sub_node_entry_for_ios_7_8_9 sub_obj = dmn_obj.list_subdomain.at(pp);
            if(sub_obj.sub_domain.trimmed().isEmpty())
                continue;

            QString s_id = sub_obj.sub_fileID;
            QString s_id_path = ios_uid_backup_path_str  + "/" + s_id;
            QString s_dmn_path =  sub_obj.sub_domain + "/" + sub_obj.sub_filename;

            QString s_flag = dmn_obj.flags;

            insert_ios_uid_details_in_filesystem_db(destination_db, s_id_path, s_dmn_path, s_id, s_flag);

        }

    }

    recon_static_functions::debug_intensive("End " , Q_FUNC_INFO);
}

void thread_file_system::extract_fs_info_and_fill_db_for_ios_uid_10_11(QSqlDatabase destination_db)
{ // prepare file_system.sqlite main database for ios backup 10,11
    recon_static_functions::debug_intensive(" start " , Q_FUNC_INFO);


    QStringList file_name_list ;
    file_name_list << "Info.plist" << "Manifest.db" << "Manifest.plist" << "Status.plist";

    for(int pp = 0; pp < file_name_list.size() ; pp++)
    {
        QString m_id = file_name_list.at(pp);
        QString m_id_path = ios_uid_backup_path_str + m_id;
        QString m_flag = "1"; // for file
        QString m_dmn_path =  m_id;

        insert_ios_uid_details_in_filesystem_db(destination_db, m_id_path, m_dmn_path, m_id, m_flag);
    }




    for(int zz = 0; zz < ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.size(); zz++)
    {
        struct_global_main_node_entry_for_ios_10_11 dmn_obj = ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.at(zz);
        if(dmn_obj.domain_name.trimmed().isEmpty())
            continue;

        QString m_dmn_path =  dmn_obj.domain_name + "/" + dmn_obj.file_absolutepath_without_domain;


        QString m_id = QString::fromLocal8Bit(dmn_obj.file_id);
        QString file_id_parent_dir_name = m_id.left(2);

        QString m_id_path = ios_uid_backup_path_str + m_id;

        if(!QFile::exists(m_id_path))
        {
            m_id_path = ios_uid_backup_path_str + file_id_parent_dir_name + "/" + m_id;
        }


        QString m_flag = dmn_obj.flags;

        insert_ios_uid_details_in_filesystem_db(destination_db, m_id_path, m_dmn_path, m_id, m_flag);


    }

    recon_static_functions::debug_intensive("END " , Q_FUNC_INFO);
}

void thread_file_system::insert_ios_uid_details_in_filesystem_db(QSqlDatabase destination_db, QString id_path, QString domain_path, QString file_id, QString m_flag)
{ // insertion ios uid details in file_system.sqlite in case of iOS backups

    recon_static_functions::debug_intensive("Start ", Q_FUNC_INFO);

    QSqlQuery insert_into_destination(destination_db);


    if(!domain_path.startsWith("/"))
        domain_path.prepend("/");

    if(domain_path.endsWith("/"))
        domain_path.chop(1);


    QFileInfo file_dmn(domain_path);

    QString preview_id_path,file_size;

    QFile check_id_exist(id_path);
    if(check_id_exist.exists())
    {

        preview_id_path = id_path;
        if(!preview_id_path.isEmpty())
        {
            if(preview_id_path.startsWith(ios_uid_backup_path_str))
                preview_id_path.remove(0,ios_uid_backup_path_str.size());

            if(!preview_id_path.startsWith("/"))
                preview_id_path.prepend("/");
        }


        bzero(&fileStat,sizeof(fileStat));
        lstat(id_path.toLocal8Bit().data(),&fileStat);       // File Info using stat
        file_size = QString::number(fileStat.st_size);

    }

    QString is_dir = "0";

    if(m_flag == "2")
        is_dir = "1";


    if(is_dir == "1")
    {
        file_size.clear();
    }


    insert_into_destination.prepare("INSERT INTO files(bookmark, filename, filepath, extension,  file_parent_path,"
                                    "file_size, source_count_name, fileID, id_path, flags,"
                                    "permission_restricted_file, is_dir, decompression_status, recon_filefrom, recon_file_infopath,"
                                    "fs_module_apple_metadata_run_status,fs_module_exif_metadata_run_status,fs_module_signature_analysis_run_status,"
                                    "fs_module_mime_type_analysis_run_status,fs_module_hashset_run_status,fs_module_face_analysis_run_status,"
                                    "fs_module_optical_character_recognition_run_status, fs_module_weapons_run_status, fs_module_skin_tone_detection_run_status,"
                                    "fs_module_fire_analysis_run_status)"
                                    " VALUES (?,?,?,?,  ?,?,?,?,  ?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?)");

    insert_into_destination.addBindValue("0");
    insert_into_destination.addBindValue(file_dmn.fileName());
    insert_into_destination.addBindValue(domain_path);
    insert_into_destination.addBindValue(recon_helper_standard_obj->mining_suffix(domain_path, Q_FUNC_INFO));
    insert_into_destination.addBindValue(file_dmn.absolutePath());
    insert_into_destination.addBindValue(file_size);
    insert_into_destination.addBindValue(source_count_name);

    insert_into_destination.addBindValue(file_id);
    insert_into_destination.addBindValue(preview_id_path);
    insert_into_destination.addBindValue(m_flag);
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(is_dir);
    insert_into_destination.addBindValue(QString::number(0));
    insert_into_destination.addBindValue(QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main));
    insert_into_destination.addBindValue(preview_id_path);

    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));
    insert_into_destination.addBindValue(QString("0"));


    if(!insert_into_destination.exec())
    {
        recon_static_functions::app_debug(" query insert_into_destination ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error- " + insert_into_destination.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" QUERY- " + insert_into_destination.executedQuery(),Q_FUNC_INFO);
    }

    total_records_fs++;
    if(total_records_fs % 100 == 0)
    {
        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, struct_target_info_ptr.source_name, false, total_records_fs, -1, true);
    }

}



void thread_file_system::fill_struct_dir_list_for_ios_uid_backup()
{ // fill structure directory list for iOS backups
    recon_static_functions::debug_intensive("Start ", Q_FUNC_INFO);

    if(ios_uid_backup_parser_obj->os_version_stripped == "7" || ios_uid_backup_parser_obj->os_version_stripped == "8" || ios_uid_backup_parser_obj->os_version_stripped == "9")
    {
        for(int i = 0; i < ios_uid_backup_parser_obj->list_node_entry_for_ios_7_8_9.size(); i++)
        {

            if(ios_uid_backup_parser_obj->list_node_entry_for_ios_7_8_9.at(i).file_size != 0)
                continue;

            QString fullpath =  ios_uid_backup_parser_obj->list_node_entry_for_ios_7_8_9.at(i).node_domain;

            if(fullpath.trimmed().isEmpty())
                continue;

            if(!fullpath.startsWith("/"))
                fullpath.prepend("/");

            if(fullpath.endsWith("/"))
                fullpath.chop(1);


            QFileInfo info_file(fullpath);

            struct_global_fs_extracted_dir_info struct_info;
            struct_info.dir_path = fullpath;
            struct_info.dir_name = info_file.fileName();
            list_struct_fs_dir_info.append(struct_info);

        }
    }
    else
    {
        for(int i = 0; i < ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.size(); i++)
        {

            if(ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.at(i).flags != "2")
                continue;

            if(ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.at(i).domain_name.trimmed().isEmpty())
                continue;

            if(!ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.at(i).file_absolutepath_without_domain.trimmed().isEmpty())
            {
                continue;
            }


            QString fullpath =  ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.at(i).domain_name + "/" + ios_uid_backup_parser_obj->list_node_entry_for_ios_10_11.at(i).file_absolutepath_without_domain;

            if(!fullpath.startsWith("/"))
                fullpath.prepend("/");

            if(fullpath.endsWith("/"))
                fullpath.chop(1);



            QFileInfo info_file(fullpath);

            struct_global_fs_extracted_dir_info struct_info;
            struct_info.dir_path = fullpath;
            struct_info.dir_name = info_file.fileName();
            list_struct_fs_dir_info.append(struct_info);
        }
    }

    recon_static_functions::debug_intensive("End ", Q_FUNC_INFO);
}

int thread_file_system::iterate_directory_recursively_and_make_dir_info_structure(QString source_path)
{// iterate directory recusrsivly for FS directory open function
    recon_static_functions::debug_intensive("Start " + source_path, Q_FUNC_INFO);

    if(source_path.size() > 1)
    {
        if(source_path.endsWith("/"))
            source_path.chop(1);
    }

    DIR *dir;
    struct dirent *dirent_obj;
    struct stat fileStat;
    if ((dir = opendir (source_path.toLocal8Bit().data())) != NULL)
    {
        while ((dirent_obj = readdir (dir)) != NULL)
        {


            QString path_inside_directory = source_path + QString("/") + QString(dirent_obj->d_name);

            if(QFileInfo(path_inside_directory).fileName() == QString(".") || QFileInfo(path_inside_directory).fileName() == QString(".."))
                continue;

            bzero(&fileStat,sizeof(fileStat));
            if(!lstat(path_inside_directory.toLocal8Bit().data(),&fileStat))
            {
                //                if(S_ISDIR(fileStat.st_mode) || S_ISLNK(fileStat.st_mode))
                if(S_ISDIR(fileStat.st_mode))
                {
                    struct_global_fs_extracted_dir_info struct_info;
                    struct_info.dir_name = QString(dirent_obj->d_name);

                    QString path_without_initial = path_inside_directory;
                    if(path_without_initial.startsWith(struct_target_info_ptr.fs_extraction_source_path))
                        path_without_initial.remove(0,struct_target_info_ptr.fs_extraction_source_path.size());

                    if(!path_without_initial.startsWith("/"))
                        path_without_initial.prepend("/");

                    struct_info.dir_path = path_without_initial;


                    if(recon_helper_standard_obj->bool_is_permission_restricted_file(struct_info.dir_path, Q_FUNC_INFO))
                        continue;

                    list_struct_fs_dir_info.append(struct_info);
                }
            }
        }

        closedir (dir);
    }
    else
    {
        recon_static_functions::debug_intensive(" FS Directory Open -----FAILED---- " + source_path, Q_FUNC_INFO);
        return EXIT_FAILURE;
    }

    recon_static_functions::debug_intensive("End " , Q_FUNC_INFO);
}


void thread_file_system::add_fs_dir_in_case_tree()
{ // Add File System directory in case tree to see and expand the directories and files in case tree
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    int enum_case_child_value = enum_global_case_tree_display_row_SOURCES;
    QString case_source_val = MACRO_CASE_TREE_Source;

    QTreeWidgetItem *item = NULL;
    for(int ii = 0; ii < case_treewidget_obj->topLevelItemCount(); ii++)
    {
        if(case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->text(enum_global_case_tree_display_column_ZERO) == QString(case_source_val))
        {
            for(int pp = 0; pp < case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->childCount(); pp++)
            {
                bool bool_item_found = false;

                if(case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->text(enum_global_case_tree_display_column_FIRST) == source_count_name)
                {
                    QString records_str = case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->text(enum_global_case_tree_display_column_RECORD_COUNT);
                    if(records_str.trimmed().isEmpty())
                    {
                        item = case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp);
                        bool_item_found = true;
                    }
                }
                else if((case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->text(enum_global_case_tree_display_column_FIRST) == MACRO_CASE_TREE_Parent_Source))
                {
                    if(struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
                            || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)

                    {
                        QTreeWidgetItem *image_parent_item = case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp);

                        if(image_parent_item->text(enum_global_case_tree_display_column_FIRST) == MACRO_CASE_TREE_Parent_Source)
                        {
                            for(int kk = 0; kk < image_parent_item->childCount(); kk++)
                            {
                                QTreeWidgetItem *image_child_item = image_parent_item->child(kk);

                                if(image_child_item->childCount() > 0)
                                {
                                    for(int zzz = 0; zzz < image_child_item->childCount(); zzz++)
                                    {
                                        QTreeWidgetItem *final_item = image_child_item->child(zzz);

                                        if(final_item->text(enum_global_case_tree_display_column_FIRST) == source_count_name)
                                        {
                                            QString records_str = final_item->text(enum_global_case_tree_display_column_RECORD_COUNT);
                                            if(records_str.trimmed().isEmpty())
                                            {
                                                item = final_item;
                                                bool_item_found = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    QTreeWidgetItem *final_item = image_child_item;

                                    if(final_item->text(enum_global_case_tree_display_column_FIRST) == source_count_name)
                                    {
                                        QString records_str = final_item->text(enum_global_case_tree_display_column_RECORD_COUNT);
                                        if(records_str.trimmed().isEmpty())
                                        {
                                            item = final_item;
                                            bool_item_found = true;
                                            break;
                                        }
                                    }
                                }



                            }
                        }

                    }
                    else
                    {
                        for(int kk = 0; kk < case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->childCount(); kk++)
                        {
                            if(case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->child(kk)->text(enum_global_case_tree_display_column_FIRST) == source_count_name)
                            {
                                QString records_str = case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->child(kk)->text(enum_global_case_tree_display_column_RECORD_COUNT);
                                if(records_str.trimmed().isEmpty())
                                {
                                    item = case_treewidget_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->child(kk);
                                    bool_item_found = true;
                                    break;
                                }
                            }
                        }

                    }
                }

                if(bool_item_found)
                    break;
            }

            break;
        }
    }

    if(item == NULL)
        return;


    item->setDisabled(false);


    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase filesytem_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    filesytem_db.setDatabaseName(fs_db_path);
    if(!filesytem_db.open())
    {
        recon_static_functions::app_debug("fs db open ---FAILED--- " + filesytem_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + filesytem_db.lastError().text() , Q_FUNC_INFO);
        return;
    }


    //QString total_records = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) from files"), 0, fs_db_path, Q_FUNC_INFO);
    QString total_records = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files"), 0, filesytem_db, Q_FUNC_INFO);

    QString src_name = item->text(enum_global_case_tree_display_column_ZERO);
    item->setText(enum_global_case_tree_display_column_ZERO , src_name + " (" + total_records + ")");
    item->setText(enum_global_case_tree_display_column_RECORD_COUNT ,total_records);

    for(int i = 0; i < list_struct_fs_dir_info.size(); i++)
    {
        struct_global_fs_extracted_dir_info struct_dir_info = list_struct_fs_dir_info.at(i);

        if(struct_dir_info.dir_path.size() == 1 && struct_dir_info.dir_path.startsWith("/") && struct_dir_info.dir_path.endsWith("/"))
            continue;

        initiate_dir_insertion_for_fs_in_case_tree(item, struct_dir_info.dir_path, filesytem_db);
    }

    filesytem_db.close();


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void thread_file_system::initiate_dir_insertion_for_fs_in_case_tree(QTreeWidgetItem *item, QString dir_path, QSqlDatabase &filesystem_db)
{ // set fs directory in case tree with records
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    /// Because it will always be a file.No need to do the following work.
    if(struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal) || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal))
        return;

    QString dir_temp_path = dir_path;

    if(dir_temp_path.startsWith(source_path))
        dir_temp_path.remove(0,source_path.size());

    QStringList splitted_list = dir_temp_path.split("/", Qt::SkipEmptyParts);

    if(splitted_list.size() <= 0)
        return;

    splitted_list.removeAll("");

    QTreeWidgetItem *parent_item = NULL;
    QString dir_name;
    for(int i = 0; i < splitted_list.size(); i++)
    {
        dir_name = splitted_list.takeAt(i);
        for(int j = 0; j < item->childCount(); j++)
        {
            if(item->child(j)->text(enum_global_case_tree_display_column_ZERO) == dir_name)
            {
                parent_item = item->child(j);
                break;
            }
        }

        break;
    }

    if(parent_item == NULL)
    {
        QFileIconProvider icon_provide;

        QIcon  icon_path;
        icon_path = icon_provide.icon(QFileIconProvider::Folder);

        QString fs_dir_path = dir_path;

        //        if(!fs_dir_path.isEmpty() && !fs_dir_path.endsWith("/"))
        //            fs_dir_path.append("/");

        if(fs_dir_path.trimmed() != QString("/") && fs_dir_path.endsWith("/"))
            fs_dir_path.chop(1);

        //   fs_dir_path.append(QString("%"));

        //QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        // QString total_records = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select count(*) from files Where filepath LIKE ?"), QStringList(fs_dir_path), 0, fs_db_path, Q_FUNC_INFO);
        //QString total_records = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select count(*) from files Where file_parent_path = ?"), QStringList(fs_dir_path), 0, fs_db_path, Q_FUNC_INFO);
        QString total_records = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select count(*) from files Where file_parent_path = ?"), QStringList(fs_dir_path), 0, filesystem_db, Q_FUNC_INFO);

        QString title_str = dir_name + " (" + total_records + ")";

        QTreeWidgetItem *item_val = new QTreeWidgetItem;
        item_val->setIcon(enum_global_case_tree_display_column_ZERO, icon_path);
        //item_val->setText(enum_global_case_tree_display_column_ZERO, dir_name);
        item_val->setText(enum_global_case_tree_display_column_ZERO, title_str);
        item_val->setText(enum_global_case_tree_display_column_FIRST,struct_target_info_ptr.source_count_name);
        item_val->setText(enum_global_case_tree_display_column_SECOND, QString(MACRO_CASE_TREE_Parent_Source));
        item_val->setText(enum_global_case_tree_display_column_THIRD,dir_path);
        item_val->setText(enum_global_case_tree_display_column_RECORD_COUNT ,total_records);
        item_val->setText(enum_global_case_tree_display_column_ROOT_COUNT_NAME,struct_target_info_ptr.root_count_name);

        QTreeWidgetItem *item_blank_child_val = new QTreeWidgetItem;
        item_blank_child_val->setIcon(enum_global_case_tree_display_column_ZERO, icon_path);
        item_blank_child_val->setText(enum_global_case_tree_display_column_ZERO, QString(""));
        item_blank_child_val->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_File_System_Blank_Tree_Item_Title));
        item_val->addChild(item_blank_child_val);

        item->addChild(item_val);
    }


    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

}

struct_global_fs_extracted_dir_info thread_file_system::get_struct_dir_info_from_list(QString dir_path, QList<struct_global_fs_extracted_dir_info> list_struct_file_system_dir_info)
{
    struct_global_fs_extracted_dir_info struct_dir_info;

    for(int num = 0; num < list_struct_file_system_dir_info.size(); num++)
    {
        if(list_struct_file_system_dir_info.at(num).dir_path == dir_path)
        {
            struct_dir_info = list_struct_file_system_dir_info.at(num);
            break;
        }
    }

    return struct_dir_info;
}


QList<struct_global_fs_extracted_dir_info> thread_file_system::get_list_extracted_struct_fs_dir_info_from_db()
{ // we return list_struct_fs_dir_info qlist which contains the file system info related to directory name and directory path
    QList<struct_global_fs_extracted_dir_info> list_struct_fs_dir_info;

    QString fs_extracted_dir_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + struct_target_info_ptr.source_count_name + "/file_system.sqlite";

    QString command_str = QString("Select filepath,INT from files Where file_parent_path = ?");

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(fs_extracted_dir_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + fs_extracted_dir_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);

        if(destination_db != NULL)
            delete destination_db;

        QSqlDatabase::removeDatabase(connection_naam);
        return list_struct_fs_dir_info;
    }

    QSqlQuery query_dir(*destination_db);

    query_dir.prepare(command_str);
    query_dir.addBindValue(QString("/"));

    if(!query_dir.exec())
    {
        recon_static_functions::app_debug("query_dir  --------FAILED------db file - " + fs_extracted_dir_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + query_dir.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_dir.lastError().text(),Q_FUNC_INFO);

        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        return list_struct_fs_dir_info;
    }

    while(query_dir.next())
    {

        QFileInfo info(struct_target_info_ptr.fs_extraction_source_path + query_dir.value(0).toString());

        QString rec_no = query_dir.value(1).toString();
        if(!recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(rec_no,*destination_db, "files", source_count_name,Q_FUNC_INFO))
            continue;


        QString dir_name = info.fileName();
        QString dir_path = query_dir.value(0).toString();

        struct_global_fs_extracted_dir_info struct_dirs_info;
        struct_dirs_info.dir_name = dir_name;
        struct_dirs_info.dir_path = dir_path;

        list_struct_fs_dir_info.append(struct_dirs_info);

    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    return list_struct_fs_dir_info;
}


QString thread_file_system::get_file_size_records_command(QString column_name , QString file_size_category)
{ // Prepare the file size commands to extract file size of each reocrds

    QString command_size = QString("SELECT COUNT(*) FROM files WHERE ") + column_name;

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    QString command_str = QString("Select greater_than_size, greater_than_size_unit, less_than_size, less_than_size_unit, start_size, start_size_unit, end_size, end_size_unit From file_size_filters Where filter_name = ?");

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        command_size.clear();
        return command_size;
    }

    QSqlQuery query(destination_db);
    query.prepare(command_str);
    query.addBindValue(file_size_category);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command_str,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        command_size.clear();
        return command_size;
    }

    int count = 0;
    while(query.next())
    {
        count++;

        if(count % 100 == 0)
            QCoreApplication::processEvents();

        QString greater_than_size_str  = query.value(0).toString();
        QString greater_than_size_unit_str = query.value(1).toString();

        QString less_than_size_str = query.value(2).toString();
        QString less_than_size_unit_str = query.value(3).toString();

        QString between_start_size_str = query.value(4).toString();
        QString between_start_size_unit_str = query.value(5).toString();

        QString between_end_size_str = query.value(6).toString();
        QString between_end_size_unit_str = query.value(7).toString();

        ///greater than
        if(!greater_than_size_str.isEmpty() && less_than_size_str.isEmpty())
        {
            if(greater_than_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                qint64 start_size_num = 0;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;

                command_size.append(" > " + QString::number(start_size_num));

            }
            else
            {
                command_size.append(" > " + QString::number(greater_than_size_str.toInt()));
            }

        }

        ///less than
        if(!less_than_size_str.isEmpty() && greater_than_size_str.isEmpty())
        {
            if(less_than_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                qint64 end_size_num = 0;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;

                command_size.append(" < " + QString::number(end_size_num));
            }
            else
            {
                command_size.append(" < " + QString::number(less_than_size_str.toInt()));
            }
        }

        ///in between
        if(!between_start_size_str.isEmpty() && !between_end_size_str.isEmpty())
        {

            qint64 start_size_num = 0;
            if(between_start_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;

            }
            else
            {
                start_size_num = between_start_size_str.toInt();
            }


            qint64 end_size_num = 0;
            if(between_end_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;
            }
            else
            {
                end_size_num = between_end_size_str.toInt();
            }

            command_size.append(" between " + QString::number(start_size_num) + QString(" AND ") + QString::number(end_size_num));
        }

    }

    destination_db.close();
    return command_size.trimmed();
}


void thread_file_system::generate_filesystem_timeline(QString source_count_name)
{ // generate file system timelines
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + source_count_name + ".sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase final_destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    final_destination_db.setDatabaseName(fs_timeline_db_path);
    if(!final_destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + final_destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + final_destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery dest_query(final_destination_db);

    QString columns_str = " (bookmark INT,INT INTEGER PRIMARY KEY,plugin varchar(30),category varchar(30), table_name varchar(30), "
                          "record_no INTEGER, stamp_name varchar(30), stamp_type varchar(30),timestamp INTEGER,item1 varchar(300), item2 varchar(300),os_scheme_display varchar(300),source_count_name varchar(300),"
                          "loaded varchar(10), year_str varchar(100), month_year varchar(100), day_month_year varchar(100), hour_day_month_year varchar(100),recon_tag_value varchar(30), notes varchar(100),blank_column varchar(100),"
                          "notes_icon varchar(100),LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000))";

    QString command_str = "create table timeline_data" + columns_str;
    dest_query.prepare(command_str);
    if(!dest_query.exec())
    {
        recon_static_functions::app_debug(" create query(dest) --------FAILED------ " + dest_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
        final_destination_db.close();
        return;
    }


    ///===== temp database ========
    QString tmp_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "tmp_fs_timeline.sqlite";
    QSqlDatabase::removeDatabase(connection_naam + "_1");
    QSqlDatabase tmp_destination_db = QSqlDatabase::addDatabase("QSQLITE", (connection_naam + "_1"));
    tmp_destination_db.setDatabaseName(tmp_timeline_db_path);
    if(!tmp_destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening(tmp fs) --------FAILED------ " + tmp_destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + tmp_destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(tmp_timeline_db_path , Q_FUNC_INFO);
        final_destination_db.close();
        return;
    }

    QSqlQuery query(tmp_destination_db);

    command_str = "create table tmp_timeline_data" + columns_str;
    query.prepare(command_str);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" create query(tmp) --------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(tmp_timeline_db_path , Q_FUNC_INFO);
        final_destination_db.close();
        return;
    }

    ///==Attach temp with main database for copy
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS timelineDB")
            .arg(QDir::toNativeSeparators(tmp_timeline_db_path));
    if(!dest_query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug(" attach temp to main --------FAILED------ " + dest_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(tmp_timeline_db_path , Q_FUNC_INFO);
        final_destination_db.close();
        return;
    }



    /// attach filesystem database to temp database
    QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    db_attach_commmand = QString("ATTACH DATABASE '%1' AS fs_timelineDB")
            .arg(QDir::toNativeSeparators(filesystem_db_path));
    if(!query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug(" FS Db Attach --------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(tmp_timeline_db_path , Q_FUNC_INFO);
        final_destination_db.close();
        return;
    }


    QStringList timestamp_name_list;

    timestamp_name_list << "Change Time" << "Modification Time" << "Last Access Time" << "Creation Time";

    for(int num = 0; num < timestamp_name_list.size(); num++)
    {

        QString timestamp_name = timestamp_name_list.at(num);
        QString timestamp_type , timestamp_column_name;

        if(timestamp_name == QString("Change Time"))
        {
            timestamp_type = QString("CHANG");
            timestamp_column_name = QString("z_date_time_change");
        }
        else if(timestamp_name == QString("Modification Time"))
        {
            timestamp_type = QString("MODIF");
            timestamp_column_name = QString("z_date_time_modified");
        }
        else if(timestamp_name == QString("Last Access Time"))
        {
            timestamp_type = QString("LACCS");
            timestamp_column_name = QString("z_date_time_accessed");
        }
        else if(timestamp_name == QString("Creation Time"))
        {
            timestamp_type = QString("CREAT");
            timestamp_column_name = QString("z_date_time_created");
        }
        else
        {
            timestamp_type.clear();
            timestamp_column_name.clear();
        }

        if(timestamp_column_name.isEmpty())
            continue;

        command_str = QString("Insert into tmp_timeline_data (record_no, timestamp, item1, item2, source_count_name,"
                              "bookmark) ");
        command_str.append(QString("Select INT,") + timestamp_column_name + QString(",filename,filepath,source_count_name,bookmark From files Where ") + timestamp_column_name + " is not null AND " + timestamp_column_name + " != ''");

        if(!query.exec(command_str))
        {
            recon_static_functions::app_debug(" query execute (insert and select)--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        command_str = QString("Update tmp_timeline_data set plugin = ?,category = ?, table_name = ? , stamp_name = ? , stamp_type = ?");

        query.prepare(command_str);
        query.addBindValue(QString(MACRO_Plugin_Name_File_System));           //plugin
        query.addBindValue(QString(MACRO_Plugin_File_System_TAB_Name_Files)); //category
        query.addBindValue(QString("files"));                 //table_name
        query.addBindValue(timestamp_name);           //stamp_name
        query.addBindValue(timestamp_type);           //stamp_type

        if(!query.exec())
        {
            recon_static_functions::app_debug(" query execute (update)--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);

        }

        /// save info into main db from temp db
        command_str = QString("Insert into timeline_data (bookmark,plugin,category,table_name,record_no, stamp_name,stamp_type,timestamp, item1, item2, source_count_name) ");
        command_str.append(QString("Select bookmark,plugin,category,table_name,record_no, stamp_name,stamp_type,timestamp, item1, item2, source_count_name From tmp_timeline_data"));

        if(!dest_query.exec(command_str))
        {
            recon_static_functions::app_debug(" query execute (insert and select main)--------FAILED------ " + dest_query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
        }


        /// delete everything from temp db
        command_str = QString("Delete From tmp_timeline_data");
        if(!query.exec(command_str))
        {
            recon_static_functions::app_debug(" query execute (delete)--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        }

    }

    final_destination_db.close();
    tmp_destination_db.close();
    recon_static_functions::remove_safely_blank_dir_OR_file(tmp_timeline_db_path , Q_FUNC_INFO);


    recon_helper_standard_obj->create_db_index_by_db_path("data_rec", "record_no", "timeline_data", fs_timeline_db_path, Q_FUNC_INFO);
    recon_helper_standard_obj->create_db_index_by_db_path("data_time", "timestamp", "timeline_data", fs_timeline_db_path, Q_FUNC_INFO);


    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

void thread_file_system::check_for_other_sources(QString display_path, QString info_path)
{ // Here we check other sources if found in current source like any dmg or any ios backup. Later we display the dmg in Disk Images section in case tree and iOS backup in Mobile Backup
    // section in case tree
    QString source_count_name = struct_target_info_ptr.source_count_name;
    QString root_count_name   = struct_target_info_ptr.root_count_name;

    QString start_point = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);
    QString fs_extraction_src_path = source_path;


    QString complete_path ;
    if(struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal))
    {
        complete_path = fs_extraction_src_path;
    }
    else
    {
        complete_path = fs_extraction_src_path + info_path;
    }

    QFileInfo info_ios(complete_path);

    QString m_file_name = info_ios.fileName().trimmed();
    if(m_file_name == ("Manifest.db") || m_file_name == ("Manifest.mbdb"))
    {
        ///iOS backups

        QString ios_dir_path = info_ios.absolutePath() + QString("/");
        if(QFileInfo::exists(ios_dir_path + "Info.plist"))
        {
            if(QFileInfo::exists(ios_dir_path + "Manifest.plist"))
            {
                if(QFileInfo::exists(ios_dir_path + "Status.plist"))
                {
                    struct_global_ios_uid_backup_detail mobj = recon_static_functions::extract_backup_details_iOS_uid(ios_dir_path + "Info.plist", Q_FUNC_INFO);
                    if(!mobj.product_version.trimmed().isEmpty() && !mobj.build_version.trimmed().isEmpty() && !mobj.serial_number.trimmed().isEmpty())
                    {
                        if(ios_dir_path.endsWith("/"))
                            ios_dir_path.chop(1);

                        QString path_ios_bckp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";

                        QString command_que = ("insert into tbl_backup_list(bookmark, plugin_name , tab_name , device_name,device_id,imei,"
                                               "last_backup_date, phone_serial_number,phone_number, product_type, product_version,"
                                               "itunes_version,source_size,"
                                               "source_name,source_path,build_version,display_name,guid,product_name,target_type,iccid, source_type_display,source_type_internal,"
                                               "extraction_status_as_source,source_count_name,root_count_name,"
                                               "recon_filefrom, recon_file_infopath,is_nested,is_decompressed"
                                               ") values (?,?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?, ?,?)");

                        QStringList atr_list;

                        atr_list << "0" << QString(MACRO_Plugin_Name_Mobile_Backup) << QString(MACRO_Plugin_Mobile_Backup_TAB_Name_Backup) << mobj.device_name << mobj.target_identifier << mobj.imei_QStringList.join(MACRO_RECON_Splitter_6_comma) << QString::number(mobj.last_backup_date_numeric_qint64) << mobj.serial_number;
                        atr_list << mobj.phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma) << mobj.product_type << mobj.product_version << mobj.itune_version << mobj.backup_size_byte_numeric_QString;
                        atr_list << QFileInfo(display_path).fileName() << QFileInfo(display_path).absolutePath() << mobj.build_version << mobj.display_name << mobj.guid << mobj.product_name;
                        atr_list << mobj.target_type << mobj.sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma) << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Display << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal << "0" << source_count_name << root_count_name;
                        atr_list << start_point << QFileInfo(info_path).absolutePath() << "1" << "0";

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_que, atr_list, path_ios_bckp, Q_FUNC_INFO);

                    }
                }
            }
        }
    }
    else if(m_file_name.endsWith(".dmg",Qt::CaseInsensitive) || m_file_name.endsWith(".vmdk",Qt::CaseInsensitive)
            || m_file_name.endsWith(".vdi",Qt::CaseInsensitive) || m_file_name.endsWith(".vhd",Qt::CaseInsensitive))
    {
        ///Disk Images
        /* Note:
                                         * source type is static as 'Forensic Image' for now.
                                         * It needs to be generic.
                                         */

        QString path_rtrvd_srcs_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";

        QString command_insert = ("insert into tbl_sources_list(bookmark, plugin_name, tab_name, source_type_display, source_type_internal, source_name, source_path,source_extension, source_size,"
                                  "root_count_name, source_count_name,extraction_status_as_source,recon_filefrom,recon_file_infopath,is_nested,is_decompressed) "
                                  "values (?,?,?,?, ?,?,?,?,?,?,?, ?,?,?,?,?)");

        QStringList val_list;
        val_list << QString::number(0)  << QString(MACRO_Plugin_Name_Disk_Images) << QString(MACRO_Plugin_Disk_Images_TAB_Name_Images)
                 << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Display << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                 << m_file_name << display_path << recon_helper_standard_obj->mining_suffix(complete_path, Q_FUNC_INFO) << recon_static_functions::mining_size(info_ios.filePath(), Q_FUNC_INFO)
                 << root_count_name << source_count_name << QString::number(0) << start_point << info_path << "1" << "0";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert, val_list, path_rtrvd_srcs_db, Q_FUNC_INFO);
    }

}

void thread_file_system::extract_posix_timestamps_for_recon_logical_images()
{ // extract posix timestamps for recon logical images
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);


    map_key_filepath_value_recon_logical_image_timestamp.clear();


    //    QString src_path = struct_target_info_ptr.source_path_1;
    //    QDir dir_path_str = QFileInfo(src_path).absoluteDir();
    //    src_path = dir_path_str.absolutePath();

    QString src_path = QFileInfo(struct_target_info_ptr.source_path_1).absolutePath();

    QString source_db_file_path = recon_static_functions::get_complete_file_path(src_path, QString(MACRO_GLOBAL_RECON_RECON_LOGICAL_FILE_INFO_NAME), Q_FUNC_INFO);
    if(!QFileInfo(source_db_file_path).exists())
    {
        recon_static_functions::app_debug("File_exist -----FAILED------" + source_db_file_path, Q_FUNC_INFO);
        return;
    }

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(source_db_file_path);
    if (!source_db.open())
    {
        recon_static_functions::app_debug("db open ------FAILED-------" + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + source_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery pragma_query(source_db);

    QString command_pragma = "PRAGMA TABLE_INFO(files)";
    if(!pragma_query.exec(command_pragma))
    {
        recon_static_functions::app_debug("command_pragma -----FAILED----- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(pragma_query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    QStringList cols_list;

    while(pragma_query.next())
    {
        cols_list << pragma_query.value(1).toString();
    }
    QString command;
    bool contain_column_change_timestamp = false;
    if(cols_list.contains("change_timestamps"))
    {
        contain_column_change_timestamp = true;
        command = "select file_path,access_timestamps,create_timestamps,modify_timestamps,change_timestamps from files";
    }
    else
    {
        command = "select file_path,access_timestamps,create_timestamps,modify_timestamps from files";
    }


    QSqlQuery select_query(source_db);
    select_query.prepare(command);

    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + source_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("exe query " + select_query.executedQuery() , Q_FUNC_INFO);
        source_db.close();
        return;
    }

    while(select_query.next())
    {
        struct_recon_logical_image_posix_timestamp recon_logical_info;
        clear_variables_recon_logical_image_posix_timestamps(recon_logical_info);

        QString file_path = select_query.value("file_path").toString();

        qint64 tm_access = select_query.value("access_timestamps").toLongLong();
        recon_logical_info.access_timestamp_qint64 = tm_access;

        if(contain_column_change_timestamp)
        {
            qint64 tm_change= select_query.value("change_timestamps").toLongLong();
            recon_logical_info.change_timestamp_qint64 = tm_change;
        }

        qint64 tm_create = select_query.value("create_timestamps").toLongLong();
        recon_logical_info.create_timestamp_qint64 = tm_create;

        qint64 tm_modify = select_query.value("modify_timestamps").toLongLong();
        recon_logical_info.modify_timestamp_qint64 = tm_modify;

        map_key_filepath_value_recon_logical_image_timestamp.insert(file_path ,recon_logical_info);
    }

    source_db.close();
    recon_static_functions::app_debug("End" , Q_FUNC_INFO);

}

void thread_file_system::slot_registry_files_extraction_work_finished()
{
    bool_registry_files_extraction_finished = true;
}

void thread_file_system::clear_variables_recon_logical_image_posix_timestamps(thread_file_system::struct_recon_logical_image_posix_timestamp &st_tmestamps)
{
    st_tmestamps.access_timestamp_qint64 = 0;
    st_tmestamps.change_timestamp_qint64 = 0;
    st_tmestamps.modify_timestamp_qint64 = 0;
    st_tmestamps.create_timestamp_qint64 = 0;
}

void thread_file_system::run_python_script_for_apple_metadata()
{ // Here we run python script/executable spotlight_parser which runs on store.db(present in paralled of mounted image in hidden folder on root location)
    // using this we create two files in our result directory "spotlight-store_data.txt", "spotlight-store_fullpaths.csv" which we use to extract apple metadata
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    // If this script run successfully then two files will create named as:
    // MACRO_Spotlight_Extracted_Text_File_Name          "spotlight-store_data.txt"
    // MACRO_Spotlight_Extracted_CSV_File_Name           "spotlight-store_fullpaths.csv"

    QString store_db_file_path;
    QString out_put_path;

    QString db_file_name = "store.db";

    QString spotlight_storev2_path = struct_target_info_ptr.virtual_source_path + QString(MACRO_Spotlight_Source_Store_V2_Dir_Path_4);
    QDirIterator dir_iter(spotlight_storev2_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    while(dir_iter.hasNext())
    {
        QString file_path = dir_iter.next();
        QFileInfo file_info(file_path);
        if(file_info.fileName() == db_file_name)
        {
            store_db_file_path = file_info.filePath();
            break;
        }
    }

    if(store_db_file_path.isEmpty())
    {
        QString spotlight_storev2_path_3 = struct_target_info_ptr.virtual_source_path + QString(MACRO_Spotlight_Source_Store_V2_Dir_Path_3);
        QDirIterator dir_iter_3(spotlight_storev2_path_3, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
        while(dir_iter_3.hasNext())
        {
            QString file_path_3 = dir_iter_3.next();
            QFileInfo file_info_3(file_path_3);
            if(file_info_3.fileName() == db_file_name)
            {
                store_db_file_path = file_info_3.filePath();
                break;
            }
        }
    }

    if(store_db_file_path.isEmpty())
    {
        QString spotlight_storev2_path_2 = struct_target_info_ptr.virtual_source_path + QString(MACRO_Spotlight_Source_Store_V2_Dir_Path_2);
        QDirIterator dir_iter_2(spotlight_storev2_path_2, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
        while(dir_iter_2.hasNext())
        {
            QString file_path_2 = dir_iter_2.next();
            QFileInfo file_info_2(file_path_2);
            if(file_info_2.fileName() == db_file_name)
            {
                store_db_file_path = file_info_2.filePath();
                break;
            }
        }
    }

    if(store_db_file_path.isEmpty())
    {
        QString spotlight_storev2_path_1 = struct_target_info_ptr.virtual_source_path + QString(MACRO_Spotlight_Source_Store_V2_Dir_Path_1);
        QDirIterator dir_iter_1(spotlight_storev2_path_1, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
        while(dir_iter_1.hasNext())
        {
            QString file_path_1 = dir_iter_1.next();
            QFileInfo file_info_1(file_path_1);
            if(file_info_1.fileName() == db_file_name)
            {
                store_db_file_path = file_info_1.filePath();
                break;
            }
        }
    }

    if(!QFileInfo(store_db_file_path).exists())
    {
        recon_static_functions::app_debug("recon logical store.db File_exist------FAILED------- " + struct_target_info_ptr.source_count_name ,Q_FUNC_INFO);
        return;
    }

    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Adding_Source_To_Case, ("Extracting Spotlight Database - " + struct_target_info_ptr.source_name), false, MACRO_Generic_Progress_No_Records_int, -1, true);

    QString apple_metadata_path =   global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();
    out_put_path = apple_metadata_path + struct_target_info_ptr.source_count_name + "/";
    QDir dir;
    dir.mkpath(out_put_path);

    //    QString script_path = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Scripts_Persistent_Dir_Path) + "spotlight_parser.py";

    //    QString executable_file_path_str = global_varibale_python_path;

    QString spotlight_executable_file_path_str = "../ZAlreadySigned/pysafe/intelligence_army/spotlight_parser";

    QStringList arguments;

    //    arguments << script_path << store_db_file_path << out_put_path;
    //    if(QFileInfo(executable_file_path_str).exists())
    //    {
    //        recon_helper_standard_obj->run_command_with_arguments_process_obj(process_general, executable_file_path_str, arguments, Q_FUNC_INFO);
    //    }

    arguments << store_db_file_path << out_put_path;

    if(QFileInfo(spotlight_executable_file_path_str).exists())
    {
        recon_helper_standard_obj->run_command_with_arguments_process_obj(process_general, spotlight_executable_file_path_str, arguments, Q_FUNC_INFO);
    }


    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

void thread_file_system::store_python_script_outputin_result_dir()
{ // store python script output(These two files "spotlight-store_fullpaths.csv", "spotlight-store_data.txt")  in result directory
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);
    struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    if(struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal))
    {
        run_python_script_for_apple_metadata();
    }
    else if(struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
            || struct_target_info_ptr.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal))
    {
        extract_posix_timestamps_for_recon_logical_images();
        run_python_script_for_apple_metadata();
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}


