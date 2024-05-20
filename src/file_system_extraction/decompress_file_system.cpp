#include "decompress_file_system.h"

decompress_file_system::decompress_file_system(QWidget *parent) : QWidget(parent)
{

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);


    bool_cancel_loading = false;
    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));

    message_dialog_obj = new message_dialog(this);

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

}

decompress_file_system::~decompress_file_system()
{

}

void decompress_file_system::slot_hide_loading_display_dialog_box(bool status)
{
    bool_cancel_loading = status;
}

void decompress_file_system::pub_set_db_essentials(QString db_path, QSqlDatabase destination_db)
{ // set file system database path
    if(destination_db.isOpen())
    {
        destination_db_obj = destination_db;
    }
    else
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        destination_db_obj = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db_obj.setDatabaseName(db_path);
        if(!destination_db_obj.open())
        {
            recon_static_functions::app_debug(" FS db opening ----FAILED---- " + destination_db_obj.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db_obj.lastError().text(),Q_FUNC_INFO);
        }
    }


}

void decompress_file_system::pub_set_treewidget_case_display_obj(R_Treewidget *obj)
{
    treewidget_case_display_obj = obj;
}

void decompress_file_system::pub_set_decompress_file_path(QString file_path)
{
    decompress_file_path = file_path;

}

void decompress_file_system::pub_set_source_count_name(QString src_cnt_name)
{
    received_source_count_name = src_cnt_name;

}

void decompress_file_system::pub_set_record_no_for_decompression(QString record_no)
{
    received_record_no = record_no;
}

QString decompress_file_system::start_run()
{ // There is a feature on right click named as "decompress file" which functonality runs/starts from this function. This right click feature enables on any compressed and
// when we hit this feature, this function will decompress that file and store the filepath, file info like timestamps etc into database. Here we decompress the file using unzip and tar
    // commands. We extract the file parallel to file_system.sqlite and store those extracted file info in same file_system.sqlite database
    recon_static_functions::app_debug(" start " ,Q_FUNC_INFO);

    if(!destination_db_obj.open())
    {
        recon_static_functions::app_debug(" FS db opening ----FAILED---- " + destination_db_obj.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_obj.lastError().text(),Q_FUNC_INFO);
        return MACRO_FAIL_VALUE_READABLE_QString;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString file_full_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(received_record_no,destination_db_obj,"files",received_source_count_name,Q_FUNC_INFO);
    if(!QFileInfo::exists(file_full_path))
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "File does not exist.");
        message_dialog_obj->exec();
        return MACRO_FAIL_VALUE_READABLE_QString;
    }

    QString command = "select filepath from files where INT = ?";
    QString file_display_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,QStringList(received_record_no),0,destination_db_obj,Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source st_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(received_source_count_name);


    QString dir_name = QString(MACRO_Decompressed_Files_Dir_Name_QString);
    if(dir_name.endsWith("/"))
        dir_name.chop(QString("/").size());

    QString extracted_dest_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + received_source_count_name + "/" + dir_name + QFileInfo(file_display_path).absolutePath();

    if(extracted_dest_path.endsWith("/"))
        extracted_dest_path.chop(QString("/").size());

    QString avl_file = recon_static_functions::get_available_filename(QFileInfo(file_display_path).fileName(), extracted_dest_path, Q_FUNC_INFO);
    extracted_dest_path = extracted_dest_path + "/_" + avl_file;


    QDir dir;
    dir.mkpath(extracted_dest_path);

    QStringList margs;
    QString exec_name;

    if(file_display_path.endsWith(".zip",Qt::CaseInsensitive))
    {
        exec_name = "unzip";
        margs << "-u" << file_full_path << "-d" << extracted_dest_path;

    }
    else if(file_display_path.endsWith(".tar",Qt::CaseInsensitive)
            || file_display_path.endsWith(".tar.gz",Qt::CaseInsensitive)
            || file_display_path.endsWith(".tar.bz2",Qt::CaseInsensitive))
    {
        exec_name = "tar";
        margs << "-xf" << file_full_path << "-C" << extracted_dest_path;
    }
    else
    {
        recon_static_functions::app_debug(" ---FAILED--- file is not ending with supportive extensions", Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return MACRO_FAIL_VALUE_READABLE_QString;
    }

    recon_helper_process_obj->run_command_with_arguments(exec_name, margs, Q_FUNC_INFO);

    QString mcmd = "update files set decompression_status = 1 where INT = ?";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(mcmd,QStringList(received_record_no), destination_db_obj, Q_FUNC_INFO);

    QString cmd_max = "select MAX(INT) from files";
    qint64 max_INT = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd_max,0,destination_db_obj, Q_FUNC_INFO).toLongLong();

    destination_db_obj.transaction();
    QSqlQuery u_query(destination_db_obj);

    qint64 fcount = 0;
    QDirIterator dir_iter(extracted_dest_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    while(dir_iter.hasNext())
    {
        fcount++;

        if(fcount % 50 == 0)
            QCoreApplication::processEvents();

        QFileInfo u_info(dir_iter.next());

        QString u_filename = u_info.fileName().trimmed();
        QString u_filepath = u_info.filePath().trimmed();
        if((u_filepath.contains(extracted_dest_path + "/__MACOSX"))
                || (u_filepath.contains(extracted_dest_path + "/.DS_Store"))
                || (u_filename == ".") || (u_filename == ".."))
        {
            continue;
        }

        QString zombie_filepath = u_filepath;
        if(zombie_filepath.startsWith(extracted_dest_path))
            zombie_filepath.remove(0, extracted_dest_path.size());

        zombie_filepath = file_display_path + zombie_filepath;

        QString zombie_parent_path = QFileInfo(zombie_filepath).absolutePath();

        QString filepath_in_result = u_filepath;
        if(filepath_in_result.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
            filepath_in_result.remove(0,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()).size());

        filepath_in_result = filepath_in_result.trimmed();
        if(!filepath_in_result.startsWith("/"))
            filepath_in_result.prepend("/");


        struct stat fileStat;
        bzero(&fileStat,sizeof(fileStat));
        lstat(u_filepath.toLocal8Bit().data(),&fileStat);       // File Info using stat

        QString is_dir = "0";
        if(u_info.isDir())
            is_dir = "1";


        if(!u_info.isDir())
            check_for_other_sources(zombie_filepath, filepath_in_result);


        QString inode_number_str = QString::number(fileStat.st_ino);

        QString change_timestamp_str ,modification_timestamp_str,last_read_timestamp_str,creation_timestamp_str;

        if(st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal
                || st_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
        {
            qint64 change_timestamp     = fileStat.st_ctimespec.tv_sec;
            qint64 modification_timestamp = fileStat.st_mtimespec.tv_sec;
            qint64 last_read_timestamp    = fileStat.st_atimespec.tv_sec;
            qint64 creation_timestamp        = fileStat.st_birthtimespec.tv_sec;

            change_timestamp_str = QString::number(change_timestamp);
            modification_timestamp_str = QString::number(modification_timestamp);
            last_read_timestamp_str = QString::number(last_read_timestamp);
            creation_timestamp_str = QString::number(creation_timestamp);
        }


        QString msize = QString::number(u_info.size());
        if(is_dir == "1")
        {
            msize.clear();
        }

        u_query.prepare("insert into files (INT,bookmark,filename, filepath, file_size,extension,file_parent_path,"
                        "z_date_time_change,z_date_time_accessed,z_date_time_modified,z_date_time_created,"
                        "source_count_name,inode_no,extended_attribute_update_status,source_file,exif_data_update_status,permission_restricted_file, is_dir,"
                        "decompression_status,recon_filefrom,recon_file_infopath,"
                        "fs_module_apple_metadata_run_status,fs_module_exif_metadata_run_status,fs_module_signature_analysis_run_status,"
                        "fs_module_mime_type_analysis_run_status,fs_module_hashset_run_status,fs_module_face_analysis_run_status, fs_module_optical_character_recognition_run_status, fs_module_weapons_run_status, fs_module_skin_tone_detection_run_status, fs_module_fire_analysis_run_status) values(?,0,?,?,?,?,?, ?,?,?,?,? ,?,?,?,?,?,?,?,?, ?,?,?, ?,?,?,?,?, ?,?,?)");

        u_query.addBindValue(QString::number(++max_INT));
        u_query.addBindValue(u_filename);
        u_query.addBindValue(zombie_filepath);
        u_query.addBindValue(msize);
        u_query.addBindValue(recon_helper_standard_obj->mining_suffix(u_filepath, Q_FUNC_INFO));
        u_query.addBindValue(zombie_parent_path);
        u_query.addBindValue(""); // change_timestamp_str
        u_query.addBindValue(""); // last_read_timestamp_str
        u_query.addBindValue(modification_timestamp_str);
        u_query.addBindValue(creation_timestamp_str);//z_date_time_created
        u_query.addBindValue(received_source_count_name);
        u_query.addBindValue(""); // inode_number_str
        u_query.addBindValue(QString::number(0));
        u_query.addBindValue("");
        u_query.addBindValue(QString::number(0));
        u_query.addBindValue("0");
        u_query.addBindValue(is_dir);
        u_query.addBindValue(QString::number(0));
        u_query.addBindValue(QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result));
        u_query.addBindValue(filepath_in_result);
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");
        u_query.addBindValue("0");

        if(!u_query.exec())
        {
            recon_static_functions::app_debug(" insert query  --------FAILED------",Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + u_query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + u_query.lastError().text(),Q_FUNC_INFO);
        }

    }

    destination_db_obj.commit();

    update_record_count_for_file_system_in_case_tree();
    update_record_count_for_mobile_backup_in_case_tree();
    update_record_count_for_disk_images_in_case_tree();


    // FS timeline pending


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

    return MACRO_PASS_VALUE_READABLE_QString;
}

void decompress_file_system::check_for_other_sources(QString display_path, QString info_path)
{ // Here we check for other sources like if found any ios backup in source then store that info in mobile_backup.sqlite OR if found any image than store that info in disk_images.sqlite
    // so we can show those sources in case tree(mobile backup/disk image section)
    recon_static_functions::app_debug(" start " ,Q_FUNC_INFO);

    QString start_point = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString init_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    struct_GLOBAL_witness_info_source st_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(received_source_count_name);

    QString source_count_name = st_source_info.source_count_name;
    QString root_count_name   = st_source_info.root_count_name;

    QFileInfo info_ios(global_recon_helper_singular_class_obj->get_complete_file_path(init_path,info_path,Q_FUNC_INFO));

    QString m_file_name = info_ios.fileName().trimmed();

    if(m_file_name == ("Manifest.db") || m_file_name == ("Manifest.mbdb"))
    {
        ///iOS uid backups
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
                                               "itunes_version,source_size,source_name,source_path,build_version,display_name,guid,product_name,target_type,iccid, source_type_display,"
                                               "source_type_internal,extraction_status_as_source,source_count_name,root_count_name, recon_filefrom, recon_file_infopath,is_nested,is_decompressed)"
                                               "values (?,?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?,?,?)");

                        QStringList atr_list;

                        atr_list << "0" << QString(MACRO_Plugin_Name_Mobile_Backup) << QString(MACRO_Plugin_Mobile_Backup_TAB_Name_Backup) << mobj.device_name << mobj.target_identifier << mobj.imei_QStringList.join(MACRO_RECON_Splitter_6_comma) << QString::number(mobj.last_backup_date_numeric_qint64) << mobj.serial_number;
                        atr_list << mobj.phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma) << mobj.product_type << mobj.product_version << mobj.itune_version << mobj.backup_size_byte_numeric_QString;
                        atr_list << QFileInfo(display_path).fileName() << QFileInfo(display_path).absolutePath() << mobj.build_version << mobj.display_name << mobj.guid << mobj.product_name;
                        atr_list << mobj.target_type << mobj.sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma) << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Display << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal << "0" << source_count_name << root_count_name;
                        atr_list << start_point << QFileInfo(info_path).absolutePath() << "1" << "1";

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
        /*Note:
         * source type is static as 'Forensic Image' for now.
         * It needs to be generic.
         */

        QString dsk_img_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";

        QString command_insert = ("insert into tbl_sources_list(bookmark, plugin_name, tab_name, source_type_display, source_type_internal, source_name,source_extension, source_path, source_size,"
                                  "root_count_name, source_count_name,extraction_status_as_source,recon_filefrom,recon_file_infopath,is_nested,is_decompressed) "
                                  "values (?,?,?,?,?, ?,?,?,?,?,?,?, ?,?,?,?)");

        QStringList val_list;
        val_list << QString::number(0)  << QString(MACRO_Plugin_Name_Disk_Images) << QString(MACRO_Plugin_Disk_Images_TAB_Name_Images)
                 << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Display) << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                 << m_file_name << recon_helper_standard_obj->mining_suffix(display_path, Q_FUNC_INFO) << display_path << recon_static_functions::mining_size(info_ios.filePath(), Q_FUNC_INFO)
                 << root_count_name << source_count_name << QString::number(0) << start_point << info_path << "1" << "1";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert, val_list, dsk_img_db, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

}

// get total record from file_system.sqlite and update total records display in case tree
void decompress_file_system::update_record_count_for_file_system_in_case_tree()
{
    recon_static_functions::app_debug(" start " ,Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source st_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(received_source_count_name);

    QTreeWidgetItem *item = NULL;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Source))
        {
            for(int pp = 0; pp < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->childCount(); pp++)
            {
                bool bool_item_found = false;

                if((treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->child(pp)->text(enum_global_case_tree_display_column_FIRST) == MACRO_CASE_TREE_Parent_Source))
                {
                    for(int kk = 0; kk < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->child(pp)->childCount(); kk++)
                    {
                        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->child(pp)->child(kk)->text(enum_global_case_tree_display_column_FIRST) == received_source_count_name)
                        {
                            item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->child(pp)->child(kk);
                            bool_item_found = true;
                            break;
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


    // item->setDisabled(false);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + received_source_count_name + "/file_system.sqlite";
    QString total_records = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) from files"), 0, fs_db_path, Q_FUNC_INFO);
    item->setText(enum_global_case_tree_display_column_ZERO , st_source_info.source_name + " (" + total_records + ")");
    item->setText(enum_global_case_tree_display_column_RECORD_COUNT ,total_records);



    //    QString decompressed_file_path;
    //    decompressed_file_path = decompress_file_path;

    //    if(!decompressed_file_path.startsWith("/"))
    //        decompressed_file_path.prepend("/");

    //    QTreeWidgetItemIterator iter(treewidget_case_display_obj);

    //    while (*iter)
    //    {

    //        QString dir_name = (*iter)->text(enum_global_case_tree_display_column_ZERO);
    //        QStringList tm_list =  dir_name.split(" (");

    //        QString child_text_without_record;
    //        if(tm_list.size() >= 1)
    //            child_text_without_record = dir_name.split(" (").at(0);

    //        QString dir_path ;
    //        if((*iter)->text(enum_global_case_tree_display_column_FIRST) == received_source_count_name)
    //        {
    //            QStringList dir_name_list;
    //            dir_name_list = decompressed_file_path.split("/");
    //            for(int ii = 0 ; ii < dir_name_list.size(); ii++)
    //            {

    //                QString dir_to_be_compared = dir_name_list.at(ii);
    //                dir_path.append(dir_to_be_compared).append("/");

    //                if(!dir_path.startsWith("/"))
    //                    dir_path.prepend("/");

    //                if(dir_path.endsWith(".zip"))
    //                    dir_path.chop(QString(".zip").size());


    //                if(dir_to_be_compared == child_text_without_record)
    //                {

    //                    QString command = QString("Select count(*) from files where file_parent_path ='" + dir_path +"'");
    //                    QString record_count_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(command, 0, fs_db_path, Q_FUNC_INFO);
    //                    QString title_str = child_text_without_record + " (" + record_count_str + ")";

    //                    if(dir_path.endsWith("/"))
    //                        dir_path.chop(QString("/").size());


    //                    QTreeWidgetItem *current_child_item = NULL;
    //                    current_child_item = dynamic_cast<QTreeWidgetItem *>(*iter);


    //                    current_child_item->setText(enum_global_case_tree_display_column_ZERO,title_str);
    //                    current_child_item->setText(enum_global_case_tree_display_column_THIRD,dir_path);
    //                    current_child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT ,record_count_str);

    //                }

    //            }

    //        }
    //        ++iter;
    //    }

    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

}
// Get total number of mibile backups we found from source from mobile_backup.sqlite database and show in case tree (Mobile Backup section)
void decompress_file_system::update_record_count_for_mobile_backup_in_case_tree()
{
    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

    QTreeWidgetItem *item = NULL;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_MOBILE_BACKUP)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Mobile_Backup_Parent))
        {
            for(int pp = 0; pp < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_MOBILE_BACKUP)->childCount(); pp++)
            {
                bool bool_item_found = false;

                if((treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_MOBILE_BACKUP)->child(pp)->text(enum_global_case_tree_display_column_FIRST) == MACRO_CASE_TREE_Mobile_Backup_Parent))
                {
                    item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_MOBILE_BACKUP)->child(pp);
                    bool_item_found = true;
                    break;
                }

                if(bool_item_found)
                    break;
            }

            break;
        }
    }
    if(item == NULL)
        return;


    QString ios_bakup_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
    qint64 total_record_count =  recon_helper_standard_obj->get_db_total_record_by_dbpath(true,ios_bakup_db_path,QStringList(""),Q_FUNC_INFO);

    QString item_text_with_records =  MACRO_CASE_TREE_Mobile_Backup_iOS_Backup + QString(" (") + QString::number(total_record_count) + QString(")");
    item->setText(enum_global_case_tree_display_column_ZERO,item_text_with_records);
    item->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(total_record_count));

    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

}

// Get total disk images number from disk_images.sqlite database and show that in case tree (Disk Images Section)
void decompress_file_system::update_record_count_for_disk_images_in_case_tree()
{
    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

    QTreeWidgetItem *item = NULL;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_DISK_IMAGES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Disk_Images_Parent))
        {
            for(int pp = 0; pp < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_DISK_IMAGES)->childCount(); pp++)
            {
                bool bool_item_found = false;
                if((treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_DISK_IMAGES)->child(pp)->text(enum_global_case_tree_display_column_FIRST) == MACRO_CASE_TREE_Disk_Images_Parent))
                {
                    item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_DISK_IMAGES)->child(pp);
                    bool_item_found = true;
                    break;
                }

                if(bool_item_found)
                    break;
            }

            break;
        }
    }
    if(item == NULL)
        return;


    QString disk_images_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    qint64 total_record_count =  recon_helper_standard_obj->get_db_total_record_by_dbpath(true,disk_images_db_path,QStringList(""),Q_FUNC_INFO);

    QString item_text_with_records =  MACRO_CASE_TREE_Disk_Images_Category_Images + QString(" (") + QString::number(total_record_count) + QString(")");

    item->setText(enum_global_case_tree_display_column_ZERO,item_text_with_records);
    item->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(total_record_count));

    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);
}

