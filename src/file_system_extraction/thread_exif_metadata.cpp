#include "thread_exif_metadata.h"

thread_exif_metadata::thread_exif_metadata(QObject *parent) : QObject(parent)
{
    exif_interface_obj = new exif_data_interface;
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard(this);
}

thread_exif_metadata::~thread_exif_metadata()
{
    if(exif_interface_obj != NULL)
        delete exif_interface_obj;
}

void thread_exif_metadata::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_exif_metadata::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_exif_metadata::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_exif_metadata::slot_extract_exif_data()
{ // extract exif metadata from here from post launcher file system features
    // we extract it using objective C classes and it's inbuilt library function which are different for audio,video,documents etc.

    //=================Command============///
    //SELECT filepath FROM files Where exif_data_update_status = '0' ORDER BY INT ASC LIMIT 10 offset 0
    //====================================///

    bool_cancel_extraction = false;
    //    QMimeType var;
    //    QMimeDatabase obj;

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Exif_Metadata);

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        mutext_for_fs_main_db->lock();
        recon_helper_standard_obj->pub_set_filesystem_modules_thread_run_status(QString(MACRO_JobType_Exif_Metadata), QString::number(1), Q_FUNC_INFO);
        mutext_for_fs_main_db->unlock();

        extract_exif_metadata_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Exif_Metadata);
        emit finished();
        return;
    }

    QString cmd_fs_db_update = QString("Update files set fs_module_exif_metadata_run_status = '1' Where ");
    int bunch_count = 50;

    QStringList exif_db_path_list;

    for(int count = 0; count < list_target_source_info.size(); count++)
    {

        if(bool_cancel_extraction)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;


        QString exif_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/exif_metadata.sqlite");


        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            exif_db_path_list << exif_db_path;
            // emit signal_add_exif_to_case_tree(QStringList(exif_db_path));
        }
        else
        {
            exif_db_path_list << exif_db_path;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            mutext_for_fs_main_db->lock();
            recon_helper_standard_obj->pub_set_filesystem_modules_thread_run_status(QString(MACRO_JobType_Exif_Metadata), QString::number(1), Q_FUNC_INFO);
            mutext_for_fs_main_db->unlock();

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");


            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Exif_Metadata, target_struct_obj.source_name, true, 0, 0, false);

            if(!open_and_create_exif_db(exif_db_path))
                continue;

            list_struct_fs_rec_and_path.clear();

            mutext_for_fs_main_db->lock();
            if(!open_fs_db(fs_db_path))
            {
                in_memory_exif_data_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files") , 0, destination_db, Q_FUNC_INFO);
            qint64  total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                destination_db.close();
                in_memory_exif_data_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            qint64 processed_file_count_qint64 = 0;
            QString estimated_time;
            qint64 remaining_file_count_for_req_time_qint64;

            remaining_file_count_for_req_time_qint64 = total_records_num;
            qint64 start_time_epoch = QDateTime::currentSecsSinceEpoch();

            QString command_fs;

            if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                command_fs = QString("select INT, id_path ,filepath, mime_type from files");
            }
            else
            {
                command_fs = QString("select INT, filepath, mime_type from files");
            }

            QSqlQuery query(destination_db);

            if(!query.exec(command_fs))
            {
                destination_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            while(query.next())
            {
                if(bool_cancel_extraction)
                    break;

                if(query.value(1).toString().trimmed().isEmpty())
                    continue;

                ///=========Skip-Files=======///
                QString filepath = query.value(1).toString();
                if(recon_helper_standard_obj->bool_is_permission_restricted_file(filepath, Q_FUNC_INFO))
                    continue;

                struct_filesystem_record_and_filepath m1;
                m1.filesystem_record = query.value(0).toString();
                m1.filepath = filepath;
                m1.display_file_path = query.value("filepath").toString();
                m1.mime_type = query.value("mime_type").toString();

                list_struct_fs_rec_and_path << m1;
            }


            destination_db.close();
            mutext_for_fs_main_db->unlock();

            qint64 records_count = 0;
            qint64 hit_count = 0;

            QStringList record_num_list;
            int fs_rec_db_updated_count = 0;

            in_memory_exif_data_db.transaction();

            for(int num = 0; num < list_struct_fs_rec_and_path.size(); num++)
            {

                if(bool_cancel_extraction)
                    break;

                qint64 current_time_epoch = QDateTime::currentSecsSinceEpoch();
                remaining_file_count_for_req_time_qint64--;
                processed_file_count_qint64++;

                if(current_time_epoch - start_time_epoch >= 5)
                {
                    double avg_image_per_sec = processed_file_count_qint64 / (current_time_epoch - start_time_epoch);
                    qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;

                    qint64 required_time = new_ramining_file_count / avg_image_per_sec;

                    estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");
                    processed_file_count_qint64 = 0;
                    start_time_epoch = current_time_epoch;
                }

                records_count++;

                struct_filesystem_record_and_filepath fs_rec_filepath_obj = list_struct_fs_rec_and_path.at(num);

                QString file_full_path = target_struct_obj.fs_extraction_source_path + fs_rec_filepath_obj.filepath;

                QString display_file_path = fs_rec_filepath_obj.display_file_path;

                bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
                if(!bool_is_file)
                    continue;


                QString mime_type_for_file = fs_rec_filepath_obj.mime_type;

                if(mime_type_for_file.startsWith("audio/")
                        || mime_type_for_file.startsWith("video/")
                        || mime_type_for_file.startsWith("application/pdf")
                        || recon_helper_standard_obj->check_is_it_image_file(file_full_path,display_file_path, Q_FUNC_INFO))
                {
                    hit_count++;
                    struct_global_exif_data_parsed_data exif_data_obj;


                    /// get exif here
                    exif_data_obj.exif_make.clear();
                    exif_data_obj.exif_model.clear();
                    exif_data_obj.exif_author.clear();

                    QString exif_data_str = exif_interface_obj->pub_extract_and_save_exif_data(mime_type_for_file, file_full_path , fs_rec_filepath_obj.filesystem_record.toLongLong(), in_memory_exif_data_db, exif_data_obj,display_file_path);
                    if(!exif_data_str.trimmed().isEmpty())
                    {
                        fs_rec_db_updated_count++;
                        record_num_list << fs_rec_filepath_obj.filesystem_record;
                    }


                    if(fs_rec_db_updated_count == bunch_count)
                    {
                        mutext_for_fs_main_db->lock();

                        QString tmp_cmd;
                        int pos = 0;

                        bool bool_list_processed = false;
                        for(int count = (record_num_list.size() -1); count >= 0; count--)
                        {
                            if(bool_cancel_extraction)
                                break;

                            bool_list_processed = true;

                            QString record_no_str = record_num_list.takeAt(count);

                            if(pos == 0)
                                tmp_cmd = cmd_fs_db_update;

                            tmp_cmd.append("INT = ");
                            tmp_cmd.append(record_no_str);
                            tmp_cmd.append(" OR ");

                            pos++;

                            if(count == bunch_count || count == 0)
                            {
                                if(tmp_cmd.endsWith(" OR "))
                                    tmp_cmd.chop(QString(" OR ").size());

                                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, fs_db_path, Q_FUNC_INFO);
                                tmp_cmd.clear();

                                pos = 0;
                            }
                        }


                        if(bool_list_processed)
                        {
                            fs_rec_db_updated_count = 0;
                            record_num_list.clear();
                        }

                        mutext_for_fs_main_db->unlock();
                    }

                }

                if(estimated_time.isEmpty() || estimated_time == "00:00:00")
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Exif_Metadata, "Calculating...");
                else
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Exif_Metadata, estimated_time);

                if(records_count % 100 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);

                    if(progress_val > 99)
                        progress_val = 99;


                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Exif_Metadata, target_struct_obj.source_name, true, records_count, progress_val, false);
                }

                //                if(hit_count % 1000 == 0)
                //                {
                //                    intermediate_save_data_to_exif_db(exif_db_path);
                //                }


            }
            //  intermediate_save_data_to_exif_db(exif_db_path);
            in_memory_exif_data_db.commit();
            recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_exif_data_db, Q_FUNC_INFO);
            in_memory_exif_data_db.close();

            if(!record_num_list.isEmpty())
            {
                mutext_for_fs_main_db->lock();

                QString tmp_cmd;
                int pos = 0;
                for(int count = (record_num_list.size() -1); count >= 0; count--)
                {
                    if(bool_cancel_extraction)
                        break;

                    QString record_no_str = record_num_list.takeAt(count);

                    if(pos == 0)
                        tmp_cmd = cmd_fs_db_update;

                    tmp_cmd.append("INT = ");
                    tmp_cmd.append(record_no_str);
                    tmp_cmd.append(" OR ");

                    pos++;

                    if(count == bunch_count || count == 0)
                    {
                        if(tmp_cmd.endsWith(" OR "))
                            tmp_cmd.chop(QString(" OR ").size());

                        recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, fs_db_path, Q_FUNC_INFO);
                        tmp_cmd.clear();

                        pos = 0;
                    }
                }

                record_num_list.clear();
                mutext_for_fs_main_db->unlock();
            }
            //emit signal_add_exif_to_case_tree(exif_db_path_list);
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Exif_Metadata, target_struct_obj.source_name, true, records_count, 100, false);

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Exif_Metadata, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);
        }
    }

    emit signal_add_exif_to_case_tree(exif_db_path_list);

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Exif_Metadata);
    emit finished();
}

void thread_exif_metadata::intermediate_save_data_to_exif_db(QString exif_database_path)
{ // save the data into exif database
    QString db_attach_command = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(exif_database_path));

    ///attach
    QSqlQuery query_attach(in_memory_exif_data_db);
    query_attach.prepare(db_attach_command);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    ///insert
    QSqlQuery query_insert(in_memory_exif_data_db);
    QString command_insert = QString("INSERT INTO filesystemDB.files SELECT * from files");
    query_insert.prepare(command_insert);
    if(!query_insert.exec())
    {
        recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        return;
    }

    ///delete
    QSqlQuery query_clear(in_memory_exif_data_db);
    QString command_clear = QString("DELETE from files");
    query_clear.prepare(command_clear);
    if(!query_clear.exec())
    {
        recon_static_functions::app_debug(" query memory clear  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }

    ///detach
    QSqlQuery query_detach(in_memory_exif_data_db);
    QString command_detach = QString("DETACH DATABASE filesystemDB");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }

}


void thread_exif_metadata::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_exif_metadata::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

bool thread_exif_metadata::open_fs_db(QString fs_db_path)
{ // open file system database
    bool db_status = false;

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
    }
    else
        db_status = true;


    return db_status;
}

bool thread_exif_metadata::open_and_create_exif_db(QString exif_db_path)
{ // open and create exif database

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_exif_data_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_exif_data_db.setDatabaseName(exif_db_path);
    if(!in_memory_exif_data_db.open())
    {
        recon_static_functions::app_debug(" exif_data_db Open  -------FAILED------ " + in_memory_exif_data_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_exif_data_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    return true;


    //    QFileInfo info_db (exif_db_path);
    //    if(info_db.exists())
    //    {
    //        if(info_db.size() <= 0 && recon_case_runner_type != QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    //            QFile::remove(exif_db_path);
    //        else
    //        {
    //            QString connection_naam = Q_FUNC_INFO;
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            in_memory_exif_data_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //            in_memory_exif_data_db.setDatabaseName(exif_db_path);
    //            if(!in_memory_exif_data_db.open())
    //            {
    //                recon_static_functions::app_debug(" exif_data_db Open  -------FAILED------ " + in_memory_exif_data_db.databaseName(), Q_FUNC_INFO);
    //                recon_static_functions::app_debug(in_memory_exif_data_db.lastError().text() , Q_FUNC_INFO);
    //                return false;
    //            }

    //            return true;
    //        }
    //    }

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    in_memory_exif_data_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    in_memory_exif_data_db.setDatabaseName(":memory:");
    //    if(!in_memory_exif_data_db.open())
    //    {
    //        recon_static_functions::app_debug(" exif_data_db Open in memory -------FAILED------ " + in_memory_exif_data_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(in_memory_exif_data_db.lastError().text() , Q_FUNC_INFO);
    //        return false;
    //    }

    //    QString command_str = QString("create table files (filesystem_record INTEGER,exif_data varchar(1024),make varchar(500),model varchar(500),author varchar(500),latitude INTEGER, longitude INTEGER,"
    //                                  " item1 varchar(500), item2 varchar(500), item3 varchar(500), item4 varchar(500), item5 varchar(500), item6 varchar(500), item7 varchar(500), item8 varchar(500), item9 varchar(500), item10 varchar(500))");

    //    QSqlQuery query_create_table(in_memory_exif_data_db);
    //    query_create_table.prepare(command_str);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file - ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        in_memory_exif_data_db.close();
    //        return false;
    //    }

    //    QString exif_database_path = exif_db_path;
    //    //    if(QFileInfo(exif_database_path).exists())
    //    //        QFile::remove(exif_database_path);

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
    //            .arg(QDir::toNativeSeparators(exif_database_path));

    //    QSqlQuery query_attach(in_memory_exif_data_db);
    //    query_attach.prepare(db_attach_commmand);
    //    if(!query_attach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_exif_data_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_save(in_memory_exif_data_db);
    //    query_save.prepare(QString("CREATE TABLE filesystemDB.files AS SELECT * from files"));
    //    if(!query_save.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Save  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
    //        in_memory_exif_data_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_detach(in_memory_exif_data_db);
    //    query_detach.prepare(QString("DETACH DATABASE filesystemDB"));
    //    if(!query_detach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Detach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_exif_data_db.close();
    //        return false;
    //    }

    //    return true;

}

void thread_exif_metadata::extract_exif_metadata_for_fs_run_module()
{ // extract exif data using right click action on file sysetm
    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Exif_Metadata, QString("Acquiring Files List..."), true, 0, 0, false);

    struct struct_fs_db_update
    {
        QString record_no;
        QString fs_db_path;
    };
    QList <struct_fs_db_update> fs_db_update_struct_list;


    QMimeType var;
    QMimeDatabase obj;

    qint64 total_records_num = run_fs_modules_details_list.size();
    qint64 records_count = 0;

    QString cmd_fs_db_update = QString("Update files set fs_module_exif_metadata_run_status = '1' Where ");


    int bunch_count = 50;
    int fs_db_updated_rec_count = 0;

    qint64 processed_file_count_qint64 = 0;
    QString estimated_time;
    qint64 remaining_file_count_for_req_time_qint64;

    remaining_file_count_for_req_time_qint64 = total_records_num;
    qint64 start_time_epoch = QDateTime::currentSecsSinceEpoch();

    for(int count = 0; count < run_fs_modules_details_list.size(); count++)
    {
        if(bool_cancel_extraction)
            break;

        qint64 current_time_epoch = QDateTime::currentSecsSinceEpoch();
        remaining_file_count_for_req_time_qint64--;
        processed_file_count_qint64++;

        if(current_time_epoch - start_time_epoch >= 5)
        {
            double avg_image_per_sec = processed_file_count_qint64 / (current_time_epoch - start_time_epoch);
            qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;

            qint64 required_time = new_ramining_file_count / avg_image_per_sec;

            estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");
            processed_file_count_qint64 = 0;
            start_time_epoch = current_time_epoch;
        }

        struct_global_run_fs_modules_details fs_modules_details_obj = run_fs_modules_details_list.at(count);

        ///=========Skip-Files=======///
        if(recon_helper_standard_obj->bool_is_permission_restricted_file(fs_modules_details_obj.complete_file_path, Q_FUNC_INFO))
            continue;


        struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);


        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;


        QString exif_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/exif_metadata.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");


        if(in_memory_exif_data_db.isOpen() && in_memory_exif_data_db.databaseName() == exif_db_path)
        {
            ///db for same source already open no need to open again
        }
        else
        {
            if(in_memory_exif_data_db.isOpen())
            {
                in_memory_exif_data_db.commit();
                recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_exif_data_db, Q_FUNC_INFO);
                in_memory_exif_data_db.close();

                QSqlDatabase::removeDatabase(in_memory_exif_data_db.connectionName());
            }
            if(!open_and_create_exif_db(exif_db_path))
                continue;

            in_memory_exif_data_db.transaction();
        }

        records_count++;

        QString file_full_path = fs_modules_details_obj.complete_file_path;

        QString display_file_path =  fs_modules_details_obj.display_filepath;

        bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
        if(!bool_is_file)
            continue;


        var = obj.mimeTypeForFile(file_full_path);
        QString mime_type_for_file = var.name();

        if(mime_type_for_file.startsWith("audio/")
                || mime_type_for_file.startsWith("video/")
                || mime_type_for_file.startsWith("application/pdf")
                || recon_helper_standard_obj->check_is_it_image_file(file_full_path,display_file_path, Q_FUNC_INFO))
        {
            struct_global_exif_data_parsed_data exif_data_obj;

            /// get exif here
            exif_data_obj.exif_make.clear();
            exif_data_obj.exif_model.clear();
            exif_data_obj.exif_author.clear();

            QString exif_data_str = exif_interface_obj->pub_extract_and_save_exif_data(mime_type_for_file, file_full_path , fs_modules_details_obj.fs_record_no.toLongLong(), in_memory_exif_data_db, exif_data_obj,display_file_path);

            if(!exif_data_str.trimmed().isEmpty())
            {
                struct_fs_db_update fs_db_update_obj;
                fs_db_update_obj.record_no = fs_modules_details_obj.fs_record_no;
                fs_db_update_obj.fs_db_path = fs_db_path;

                fs_db_update_struct_list.append(fs_db_update_obj);
            }

            fs_db_updated_rec_count++;
            if(fs_db_updated_rec_count == bunch_count)
            {
                mutext_for_fs_main_db->lock();

                QString tmp_cmd;
                int pos = 0;

                bool bool_list_processed = false;
                for(int count = (fs_db_update_struct_list.size() -1); count >= 0; count--)
                {
                    if(bool_cancel_extraction)
                        break;

                    bool_list_processed = true;

                    struct_fs_db_update fs_db_obj = fs_db_update_struct_list.takeAt(count);

                    QString record_no_str = fs_db_obj.record_no;

                    if(pos == 0)
                        tmp_cmd = cmd_fs_db_update;

                    tmp_cmd.append("INT = ");
                    tmp_cmd.append(record_no_str);
                    tmp_cmd.append(" OR ");

                    pos++;

                    if(count == bunch_count || count == 0)
                    {
                        if(tmp_cmd.endsWith(" OR "))
                            tmp_cmd.chop(QString(" OR ").size());

                        recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, fs_db_path, Q_FUNC_INFO);
                        tmp_cmd.clear();

                        pos = 0;
                    }
                }

                if(bool_list_processed)
                {
                    fs_db_updated_rec_count = 0;
                    fs_db_update_struct_list.clear();
                }
                mutext_for_fs_main_db->unlock();
            }

            // intermediate_save_data_to_exif_db(exif_db_path);
        }

        if(estimated_time.isEmpty() || estimated_time == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Exif_Metadata, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Exif_Metadata, estimated_time);

        if(records_count % 100 == 0)
        {
            qint64 progress_val = ((records_count * 100) / total_records_num);

            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Exif_Metadata, fs_modules_details_obj.examiner_selected_file_or_dir_name, true, records_count, progress_val, false);
        }
    }


    in_memory_exif_data_db.commit();
    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_exif_data_db, Q_FUNC_INFO);
    in_memory_exif_data_db.close();
    QSqlDatabase::removeDatabase(in_memory_exif_data_db.connectionName());


    if(!fs_db_update_struct_list.isEmpty())
    {
        mutext_for_fs_main_db->lock();
        QString tmp_cmd;
        int pos = 0;
        for(int count = (fs_db_update_struct_list.size() -1); count >= 0; count--)
        {
            if(bool_cancel_extraction)
                break;

            struct_fs_db_update fs_db_obj = fs_db_update_struct_list.takeAt(count);

            QString record_no_str = fs_db_obj.record_no;

            if(pos == 0)
                tmp_cmd = cmd_fs_db_update;

            tmp_cmd.append("INT = ");
            tmp_cmd.append(record_no_str);
            tmp_cmd.append(" OR ");

            pos++;

            if(count == bunch_count || count == 0)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());

                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, fs_db_obj.fs_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();

                pos = 0;
            }
        }
        fs_db_update_struct_list.clear();
        mutext_for_fs_main_db->unlock();
    }


    QStringList exif_metadata_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString exif_db_path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/exif_metadata.sqlite");


        exif_metadata_db_path_list << exif_db_path_1;
    }

    emit signal_add_exif_to_case_tree(exif_metadata_db_path_list);

    run_fs_modules_details_list.clear();

}
