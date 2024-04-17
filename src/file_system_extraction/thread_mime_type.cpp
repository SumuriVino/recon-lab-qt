#include "thread_mime_type.h"

thread_mime_type::thread_mime_type(QObject *parent) : QObject(parent)
{
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_mime_type::~thread_mime_type()
{

}

void thread_mime_type::slot_extract_mime_type()
{
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Mime_Type);
    QMimeDatabase m_mime_database;

    bool_cancel_extraction = false;

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        extract_mime_type_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Mime_Type);
        emit finished();
        return;
    }

    QString cmd_fs_db_update = QString("Update files set fs_module_mime_type_analysis_run_status = '1' Where ");
    int bunch_count = 50;

    QStringList mime_db_path_list;

//    magic_t magic_cookie_obj;
//    magic_cookie_obj = magic_open(MAGIC_MIME_TYPE);

//    if (magic_cookie_obj == NULL)
//    {
//        recon_static_functions::app_debug("Unable to initialize MAGIC library, might be ---FAILED--- , we are not extracting all file mime type from MAGIC LIB", Q_FUNC_INFO);
//    }

//    if (magic_load(magic_cookie_obj, "../Resources/features_resources/mime_type/magic") != 0)
//    {
//        recon_static_functions::app_debug("Cannot load MAGIC database, might be ---FAILED--- , we are not extracting all file mime type from MAGIC LIB", Q_FUNC_INFO);
//    }

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_extraction)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);
        QString mime_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/mime.sqlite");

        QString sources_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "/sources_info.sqlite";

        QString select_mime_cmd = "SELECT Mime_Type from fs_status WHERE source_count_name = ?";
        QString mime_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_mime_cmd, QStringList(target_struct_obj.source_count_name),0,sources_info_db_path,Q_FUNC_INFO);


        //Now mime type extraction is hard binded with FS Extraction
        //For load case[Case created before FS + Mime Type hard bind]: If mime type is not extracted then we will extract it.
        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE && mime_status == "1")
        {
            mime_db_path_list << mime_db_path;
        }
        else
        {
            mime_db_path_list << mime_db_path;

            if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
                continue;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            list_struct_fs_rec_and_path.clear();


            //            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, target_struct_obj.source_name, true, 0, 0, false);
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, target_struct_obj.source_name, false, 0, 0, false);

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");

            if(!open_and_create_mime_type_db(mime_db_path))
                continue;

            mutext_for_fs_main_db->lock();
            if(!open_fs_db(fs_db_path))
            {
                in_memory_mime_type_db.close();
                mutext_for_fs_main_db->unlock();

                continue;
            }

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files Where fs_module_mime_type_analysis_run_status='0'") , 0, destination_db, Q_FUNC_INFO);
            qint64  total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                destination_db.close();
                in_memory_mime_type_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

//            qint64 processed_file_count_qint64 = 0;
//            QString estimated_time;
//            qint64 remaining_file_count_for_req_time_qint64;

//            remaining_file_count_for_req_time_qint64 = total_records_num;
//            qint64 start_time_epoch = QDateTime::currentSecsSinceEpoch();

            QString command_fs;

            if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                command_fs = QString("select INT, id_path from files Where fs_module_mime_type_analysis_run_status='0'");
            }
            else
                command_fs = QString("select INT, filepath from files Where fs_module_mime_type_analysis_run_status='0'");

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
                list_struct_fs_rec_and_path << m1;
            }

            destination_db.close();
            mutext_for_fs_main_db->unlock();

            in_memory_mime_type_db.transaction();
            QSqlQuery insert_query(in_memory_mime_type_db);

            qint64 records_count = 0;

            QStringList record_num_list;
            int fs_rec_db_updated_count = 0;

            for(int num = 0; num < list_struct_fs_rec_and_path.size(); num++)
            {
                if(bool_cancel_extraction)
                    break;

//                qint64 current_time_epoch = QDateTime::currentSecsSinceEpoch();
//                remaining_file_count_for_req_time_qint64--;
//                processed_file_count_qint64++;

//                if(current_time_epoch - start_time_epoch >= 5)
//                {
//                    double avg_image_per_sec = processed_file_count_qint64 / (current_time_epoch - start_time_epoch);
//                    qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;

//                    qint64 required_time = new_ramining_file_count / avg_image_per_sec;

//                    estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");
//                    processed_file_count_qint64 = 0;
//                    start_time_epoch = current_time_epoch;
//                }

                records_count++;

                struct_filesystem_record_and_filepath fs_rec_filepath_obj = list_struct_fs_rec_and_path.at(num);

                QString file_full_path = target_struct_obj.fs_extraction_source_path + fs_rec_filepath_obj.filepath;

                bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
                if(!bool_is_file)
                    continue;

                //                QString m_mime_type = m_mime_database.mimeTypeForFile(file_full_path, QMimeDatabase::MatchContent).name();

                QString m_mime_type;

                if(file_full_path.endsWith(".mkv"))
                {
                   m_mime_type = "video/x-matroska";
                 //   m_mime_type = magic_file(magic_cookie_obj, file_full_path.toStdString().c_str());
                }
                else
                    m_mime_type = m_mime_database.mimeTypeForFile(file_full_path, QMimeDatabase::MatchContent).name();


                if(m_mime_type.trimmed().isEmpty())
                {
                    if(records_count % 1000 == 0)
                    {
                        qint64 progress_val = ((records_count * 100) / total_records_num);
                        if(progress_val > 99)
                            progress_val = 99;

                        //                        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, target_struct_obj.source_name, true, records_count, progress_val, false);
                        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, target_struct_obj.source_name, false, records_count, progress_val, false);
                    }

                    continue;
                }

                QString command = QString("insert into files (filesystem_record, mime_type) values(?,?)");
                insert_query.prepare(command);

                insert_query.addBindValue(fs_rec_filepath_obj.filesystem_record);
                insert_query.addBindValue(m_mime_type);
                if(!insert_query.exec())
                {
                    recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
                    continue;

                }

                record_num_list << fs_rec_filepath_obj.filesystem_record;
                fs_rec_db_updated_count++;

                if(fs_rec_db_updated_count == bunch_count)
                {
                    mutext_for_fs_main_db->lock();


                    bool bool_list_processed = false;
                    QString tmp_cmd;
                    int pos = 0;
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

//                if(estimated_time.isEmpty() || estimated_time == "00:00:00")
//                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Mime_Type, "Calculating...");
//                else
//                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Mime_Type, estimated_time);

                if(records_count % 1000 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);
                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, target_struct_obj.source_name, false, records_count, progress_val, false);
                    //  intermediate_save_data_to_mime_db(mime_db_path);
                }

            }

            // intermediate_save_data_to_mime_db(mime_db_path);

            in_memory_mime_type_db.commit();
            recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_mime_type_db, Q_FUNC_INFO);
            in_memory_mime_type_db.close();

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

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, target_struct_obj.source_name, false, records_count, 100, false);

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Mime_Type, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

            mutext_for_fs_main_db->lock();
            copy_data_from_mime_to_fs_db(target_struct_obj.source_count_name);

            mutext_for_fs_main_db->unlock();
        }
        emit signal_mime_type_extracted_source(target_struct_obj.source_count_name);

    }

//    magic_close(magic_cookie_obj);

    emit signal_add_mime_types_to_case_tree(mime_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Mime_Type);

    emit finished();

}

void thread_mime_type::copy_data_from_mime_to_fs_db(QString source_count_name)
{

    QString mime_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/mime.sqlite");


    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/file_system.sqlite");


    //    recon_helper_standard_obj->create_db_index("fs_parent","filesystem_record", "files", mime_db_path, Q_FUNC_INFO);



    QStringList attribute_column_list_in_mime_db;
    attribute_column_list_in_mime_db << "mime_type";

    QStringList attribute_column_list_in_fs_db;
    attribute_column_list_in_fs_db << "mime_type";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("DB open  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery dest_query(destination_db);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
            .arg(QDir::toNativeSeparators(mime_db_path));
    if(!dest_query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug("query attach   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }


    QString command_str = QString("Update main.files set ");
    for(int ii = 0; ii < attribute_column_list_in_fs_db.size(); ii++)
    {
        command_str.append(QString(" " + attribute_column_list_in_fs_db.at(ii) + " = (Select " + attribute_column_list_in_mime_db.at(ii) + " From mimeDB.files where main.files.INT = mimeDB.files.filesystem_record),"));
    }
    if(command_str.trimmed().endsWith(","))
        command_str.chop(1);



    while(1)
    {
        if(bool_cancel_extraction)
            break;

        bool bool_query_execute = true;
        if(!dest_query.exec(command_str))
        {
            recon_static_functions::app_debug("query update   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
            bool_query_execute = false;
        }
        if(bool_query_execute)
            break;
    }



    QString command_detach = QString("DETACH DATABASE mimeDB");
    dest_query.exec(command_detach);
    destination_db.close();

}

void thread_mime_type::intermediate_save_data_to_mime_db(QString mime_database_path)
{
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
            .arg(QDir::toNativeSeparators(mime_database_path));

    QSqlQuery query_attach(in_memory_mime_type_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_insert(in_memory_mime_type_db);

    QString command_insert = QString("INSERT INTO mimeDB.files SELECT * from files");
    query_insert.prepare(command_insert);
    if(!query_insert.exec())
    {
        recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_clear(in_memory_mime_type_db);
    QString command_clear = QString("DELETE from files");
    query_clear.prepare(command_clear);
    if(!query_clear.exec())
    {
        recon_static_functions::app_debug(" query memory delete  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_detach(in_memory_mime_type_db);
    QString command_detach = QString("DETACH DATABASE mimeDB");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }
}


bool thread_mime_type::open_and_create_mime_type_db(QString mime_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_mime_type_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_mime_type_db.setDatabaseName(mime_db_path);
    if(!in_memory_mime_type_db.open())
    {
        recon_static_functions::app_debug(" exif_data_db Open  -------FAILED------ " + in_memory_mime_type_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_mime_type_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    return true;





    //    QFileInfo info_db (mime_db_path);
    //    if(info_db.exists())
    //    {
    //        if(info_db.size() <= 0 && recon_case_runner_type != QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    //            QFile::remove(mime_db_path);
    //        else
    //        {
    //            QString connection_naam = Q_FUNC_INFO;
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            in_memory_mime_type_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //            in_memory_mime_type_db.setDatabaseName(mime_db_path);
    //            if(!in_memory_mime_type_db.open())
    //            {
    //                recon_static_functions::app_debug(" exif_data_db Open  -------FAILED------ " + in_memory_mime_type_db.databaseName(), Q_FUNC_INFO);
    //                recon_static_functions::app_debug(in_memory_mime_type_db.lastError().text() , Q_FUNC_INFO);
    //                return false;
    //            }

    //            return true;
    //        }
    //    }

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    in_memory_mime_type_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    in_memory_mime_type_db.setDatabaseName(":memory:");
    //    if(!in_memory_mime_type_db.open())
    //    {
    //        recon_static_functions::app_debug(" Open in memory  ---FAILED----" + in_memory_mime_type_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(in_memory_mime_type_db.lastError().text() , Q_FUNC_INFO);
    //        return false;
    //    }

    //    QString command_str = QString("create table files (bookmark INTEGER, filesystem_record INTEGER, mime_type varchar(200))");

    //    QSqlQuery query_create_table(in_memory_mime_type_db);
    //    query_create_table.prepare(command_str);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file - ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        in_memory_mime_type_db.close();
    //        return false;
    //    }

    //    //    if(QFileInfo(mime_db_path).exists())
    //    //        QFile::remove(mime_db_path);

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
    //            .arg(QDir::toNativeSeparators(mime_db_path));

    //    QSqlQuery query_attach(in_memory_mime_type_db);
    //    query_attach.prepare(db_attach_commmand);
    //    if(!query_attach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_mime_type_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_save(in_memory_mime_type_db);
    //    query_save.prepare(QString("CREATE TABLE mimeDB.files AS SELECT * from files"));
    //    if(!query_save.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Save  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
    //        in_memory_mime_type_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_detach(in_memory_mime_type_db);
    //    query_detach.prepare("DETACH DATABASE mimeDB");
    //    if(!query_detach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Detach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_mime_type_db.close();
    //        return false;
    //    }

    //    return true;

}


bool thread_mime_type::open_fs_db(QString fs_db_path)
{
    recon_static_functions::app_debug(" -Starts" ,Q_FUNC_INFO);

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

    recon_static_functions::app_debug(" -Ends" ,Q_FUNC_INFO);

    return db_status;
}

void thread_mime_type::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}


void thread_mime_type::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_mime_type::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_mime_type::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_mime_type::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_mime_type::extract_mime_type_for_fs_run_module()
{
    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, QString("Acquiring Files List..."), true, 0, 0, false);

    struct struct_fs_db_update
    {
        QString record_no;
        QString fs_db_path;
    };
    QList <struct_fs_db_update> fs_db_update_struct_list;


    int fs_db_updated_rec_count = 0;
    int bunch_count = 50;


    QMimeDatabase m_mime_database;

    qint64 total_records_num = run_fs_modules_details_list.size();
    qint64 records_count = 0;

    QString cmd_fs_db_update = QString("Update files set fs_module_mime_type_analysis_run_status = '1' Where ");

    QStringList processed_source_count_list;

    for(int count = 0; count < run_fs_modules_details_list.size(); count++)
    {
        if(bool_cancel_extraction)
            break;

        struct_global_run_fs_modules_details fs_modules_details_obj = run_fs_modules_details_list.at(count);

        ///=========Skip-Files=======///
        if(recon_helper_standard_obj->bool_is_permission_restricted_file(fs_modules_details_obj.complete_file_path, Q_FUNC_INFO))
            continue;

        struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);
        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;

        QString mime_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/mime.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");

        if(in_memory_mime_type_db.isOpen() && in_memory_mime_type_db.databaseName() == mime_db_path)
        {
            ///db for same source already open no need to open again
        }
        else
        {
            processed_source_count_list << fs_modules_details_obj.source_count_name;

            if(in_memory_mime_type_db.isOpen())
            {
                in_memory_mime_type_db.commit();
                recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_mime_type_db, Q_FUNC_INFO);
                in_memory_mime_type_db.close();

                QSqlDatabase::removeDatabase(in_memory_mime_type_db.connectionName());
            }
            if(!open_and_create_mime_type_db(mime_db_path))
                continue;

            in_memory_mime_type_db.transaction();
        }

        QString file_full_path = fs_modules_details_obj.complete_file_path;

        records_count++;

        bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
        if(!bool_is_file)
        {
            if(records_count % 1000 == 0)
            {
                qint64 progress_val = ((records_count * 100) / total_records_num);
                if(progress_val > 99)
                    progress_val = 99;

                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, fs_modules_details_obj.examiner_selected_file_or_dir_name, true, records_count, progress_val, false);
            }

            continue;
        }

        //        if(!QFileInfo(file_full_path).isFile())
        //            continue;


        QString m_mime_type = m_mime_database.mimeTypeForFile(file_full_path, QMimeDatabase::MatchContent).name();

        QSqlQuery insert_query(in_memory_mime_type_db);

        QString command = QString("insert into files (filesystem_record, mime_type) values(?,?)");
        insert_query.prepare(command);

        insert_query.addBindValue(fs_modules_details_obj.fs_record_no);
        insert_query.addBindValue(m_mime_type);
        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
            continue;
        }
        //  intermediate_save_data_to_mime_db(mime_db_path);

        struct_fs_db_update fs_db_update_obj;
        fs_db_update_obj.record_no = fs_modules_details_obj.fs_record_no;
        fs_db_update_obj.fs_db_path = fs_db_path;

        fs_db_update_struct_list.append(fs_db_update_obj);

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

        if(records_count % 1000 == 0)
        {
            qint64 progress_val = ((records_count * 100) / total_records_num);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Mime_Type, fs_modules_details_obj.examiner_selected_file_or_dir_name, true, records_count, progress_val, false);
        }
    }

    processed_source_count_list.removeDuplicates();

    in_memory_mime_type_db.commit();
    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_mime_type_db, Q_FUNC_INFO);
    in_memory_mime_type_db.close();
    QSqlDatabase::removeDatabase(in_memory_mime_type_db.connectionName());

    mutext_for_fs_main_db->lock();
    for(int count = 0; count < processed_source_count_list.size(); count++)
        copy_data_from_mime_to_fs_db(processed_source_count_list.at(count));
    mutext_for_fs_main_db->unlock();


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

    QStringList mime_type_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString mime_db_path_2 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/mime.sqlite");

        mime_type_db_path_list << mime_db_path_2;
    }

    emit signal_add_mime_types_to_case_tree(mime_type_db_path_list);

    run_fs_modules_details_list.clear();
}
