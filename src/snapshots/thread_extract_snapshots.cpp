#include "thread_extract_snapshots.h"

thread_extract_snapshots::thread_extract_snapshots(QWidget *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    total_records_fs  = 0;

}

thread_extract_snapshots::~thread_extract_snapshots()
{
    if(recon_helper_standard_obj != NULL)
        delete recon_helper_standard_obj;
}


void thread_extract_snapshots::pub_set_source_count_name(QString str)
{
    source_count_name = str;
}

void thread_extract_snapshots::pub_set_selected_snapshot_name(QString snap_str, QString db_name)
{
    selected_snapshot_str = snap_str;
    snapshot_fs_db_name   = db_name;
}

void thread_extract_snapshots::pub_set_snapshot_thread_running_status(bool status)
{
    bool_snapshot_thread_running_status = status;
}

void thread_extract_snapshots::pub_set_snapshot_mount_path(QString str)
{
    snapshot_mount_path = str;
}


void thread_extract_snapshots::run()
{
    recon_static_functions::app_debug(" -Start" , Q_FUNC_INFO);

    source_count_name = source_count_name.trimmed();
    if(source_count_name.isEmpty())
        return;

    total_records_fs  = 0;

    bool_snapshot_thread_running_status = true;

    struct_target_info_ptr = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(!QFileInfo(snapshot_mount_path).exists())
        return;

    emit signal_PBAR_thread_snapshot_fs_extraction_started(MACRO_JobType_Analysing_Snapshot);

    set_essentials();

    extract_filesystem_for_snapshot(snapshot_mount_path);

    emit signal_PBAR_thread_snapshot_file_system_finished(MACRO_JobType_Analysing_Snapshot);

    recon_static_functions::app_debug(" -End" , Q_FUNC_INFO);

}

void thread_extract_snapshots::set_essentials()
{
    snapshot_fs_dir_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_File_System_In_Result_QString).toString() + source_count_name + QString("/");

    if(!QDir(snapshot_fs_dir_path).exists())
    {
        QDir tmp_dir;
        tmp_dir.mkdir(snapshot_fs_dir_path);
    }


}

void thread_extract_snapshots::extract_filesystem_for_snapshot(QString snap_mount_path)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    ///create snapshot fs db
    QString snap_fs_db_name =  snapshot_fs_db_name + ".sqlite";
    snapshot_fs_db_path = snapshot_fs_dir_path + snap_fs_db_name;


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

    if(!query_create_table.exec())
    {
        recon_static_functions::app_debug(" query memory table create ----FAILED----  "+ destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + db_create_command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    iterate_and_insert_snapshot_filesystem_data(destination_db, snap_mount_path);

    /// Attach from memory to destination
    QString fs_path = snapshot_fs_db_path;
    if(QFileInfo(snapshot_fs_db_path).exists())
        QFile::remove(snapshot_fs_db_path);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS snap_filesystemDB")
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
    query_save.prepare(QString("CREATE TABLE snap_filesystemDB.files AS SELECT * from files"));
    if(!query_save.exec())
    {
        recon_static_functions::app_debug(" query memory Save  --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
    }

    destination_db.close();

    QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
    recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , fs_path, Q_FUNC_INFO);


    emit signal_thread_custom_finished();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

}

void thread_extract_snapshots::iterate_and_insert_snapshot_filesystem_data(QSqlDatabase& destination_db, QString snap_mount_path)
{
    QSqlQuery query_insert(destination_db);

    QString fs_extraction_src_path = snap_mount_path;
    QDirIterator dir_iter(snap_mount_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    int count = 0;
    while(dir_iter.hasNext())
    {
        count++;
        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        QString file_path = dir_iter.next().trimmed();

        QFileInfo info(file_path);
        if(info.fileName() == QString(".") || info.fileName() == QString(".."))
            continue;

        QString is_dir = "0";
        if(info.isDir())
            is_dir = "1";

        bzero(&fileStat,sizeof(fileStat));
        lstat(file_path.toLocal8Bit().data(),&fileStat);       // File Info using stat

        QString file_parent_path = info.dir().absolutePath();
        QString inode_number_str = QString::number(fileStat.st_ino);

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


        //         ///check for other source types (Mobile Backup/Disk Images)
        //         if(!info.isDir())
        //             check_for_other_sources(file_path, file_path);

        QString change_timestamp_str ,modification_timestamp_str,last_read_timestamp_str,creation_timestamp_str;

        if(struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
                || struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)

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


        QString prmsn_restrict = "0";


        if(recon_helper_standard_obj->bool_is_permission_restricted_file(file_path, Q_FUNC_INFO))
            continue;

        QString msize = QString::number(info.size());
        if(is_dir == "1")
            msize.clear();

        query_insert.prepare("insert into files (bookmark,filename, filepath, file_size,extension,file_parent_path,"
                             "z_date_time_change,z_date_time_accessed,z_date_time_modified,z_date_time_created,"
                             "source_count_name,inode_no,extended_attribute_update_status,source_file,exif_data_update_status,permission_restricted_file, is_dir,"
                             "decompression_status, recon_filefrom, recon_file_infopath,"
                             "fs_module_apple_metadata_run_status,fs_module_exif_metadata_run_status,fs_module_signature_analysis_run_status,"
                             "fs_module_mime_type_analysis_run_status,fs_module_hashset_run_status,fs_module_face_analysis_run_status,"
                             "fs_module_optical_character_recognition_run_status, fs_module_weapons_run_status, fs_module_skin_tone_detection_run_status,"
                             "fs_module_fire_analysis_run_status) values(0,?,?,?,?,?, ?,?,?,?,? ,?,?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?, ?,?,?)");

        query_insert.addBindValue(info.fileName());
        query_insert.addBindValue(file_path);
        query_insert.addBindValue(msize);
        query_insert.addBindValue(recon_helper_standard_obj->mining_suffix(file_path, Q_FUNC_INFO));
        query_insert.addBindValue(file_parent_path);
        query_insert.addBindValue(change_timestamp_str);
        query_insert.addBindValue(last_read_timestamp_str);
        query_insert.addBindValue(modification_timestamp_str);
        query_insert.addBindValue(creation_timestamp_str);
        query_insert.addBindValue(source_count_name);
        query_insert.addBindValue(inode_number_str);
        query_insert.addBindValue(QString::number(0));
        query_insert.addBindValue("");
        query_insert.addBindValue(QString::number(0));
        query_insert.addBindValue(prmsn_restrict);
        query_insert.addBindValue(is_dir);
        query_insert.addBindValue(QString::number(0));
        query_insert.addBindValue(QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Snapshot));
        query_insert.addBindValue(file_path);

        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));
        query_insert.addBindValue(QString("0"));

        if(!query_insert.exec())
        {
            recon_static_functions::app_debug(" insert query  --------FAILED------",Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query_insert.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        }

        total_records_fs++;

        if(total_records_fs % 1000 == 0)
        {
            emit signal_PBAR_thread_snapshot_file_system_value(MACRO_JobType_Analysing_Snapshot, struct_target_info_ptr.source_name, false, total_records_fs, -1, true);
        }
    }
}
