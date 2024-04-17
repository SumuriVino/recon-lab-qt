#include "thread_apple_metadata.h"

thread_apple_metadata::thread_apple_metadata(QObject *parent) : QObject(parent)
{
    bool_cancel_extraction = false;

    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_apple_metadata::~thread_apple_metadata()
{

}

void thread_apple_metadata::slot_extract_apple_metadata()
{
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Apple_Metadata);

    bool_cancel_extraction = false;

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        extract_apple_metadata_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Apple_Metadata);
        emit finished();
        return;
    }

    QStringList value_list;
    QStringList del_value_list;

    QString cmd_delete_existing = QString("Delete From files Where filesystem_record = ?");
    QString cmd_fs_db_update    = QString("Update files set fs_module_apple_metadata_run_status = '1' Where ");
    int bunch_count = 50;

    QStringList apple_metadata_db_path_list;

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_extraction)
            break;

        bool bool_need_to_fill_apple_metadata_from_QMap = false;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;

        if(target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
                && target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
        {
            continue;
        }

        QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/apple_metadata.sqlite");


        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            apple_metadata_db_path_list << apple_metadata_db_path;
        }
        else
        {
            apple_metadata_db_path_list << apple_metadata_db_path;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, target_struct_obj.source_name, true, 0, 0, false);

            list_struct_fs_rec_and_path.clear();
            map_apple_metadata_for_recon_logical_image.clear();

            attribute_column_list_final = column_name_list_examiner_selected;
            attributes_name_list_final = attributes_name_list_examiner_selected;

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");

            QString apple_metadata_path =   global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();
            QString out_put_dir_path = apple_metadata_path + target_struct_obj.source_count_name + "/";
            QString txt_file_path = out_put_dir_path + MACRO_Spotlight_Extracted_Text_File_Name;
            QString csv_file_pth = out_put_dir_path + MACRO_Spotlight_Extracted_CSV_File_Name;

            struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, ("Initializing... " + target_struct_obj.source_name), true, MACRO_Generic_Progress_No_Records_int, -1, false);
            attribute_column_list_final = column_name_list_all;
            bool_need_to_fill_apple_metadata_from_QMap = true;
            attributes_name_list_final = attributes_name_list_all;
            map_apple_metadata_for_recon_logical_image = recon_helper_standard_obj->extract_apple_metadata_from_script(txt_file_path,csv_file_pth,apple_metadata_attributes_name_list);


            if(bool_need_to_fill_apple_metadata_from_QMap)
            {
                if(!open_and_create_apple_metadata_db(apple_metadata_db_path))
                    continue;


                recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete from files"), in_memory_apple_metadata_db,Q_FUNC_INFO);

                in_memory_apple_metadata_db.transaction();
                QSqlQuery query_memory_db(in_memory_apple_metadata_db);

                qint64 records_count = 0;

                QStringList record_num_list;
                int fs_rec_db_updated_count = 0;
                mutext_for_fs_main_db->lock();
                bool status = open_fs_db(fs_db_path);
                if(!status)
                {
                    in_memory_apple_metadata_db.close();
                    mutext_for_fs_main_db->unlock();
                    continue;
                }
                QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files") , 0, destination_db, Q_FUNC_INFO);
                qint64  total_records_num = total_records_str.toLongLong();

                if(total_records_num <= 0)
                {
                    destination_db.close();
                    in_memory_apple_metadata_db.close();
                    mutext_for_fs_main_db->unlock();

                    continue;
                }

                qint64 processed_file_count_qint64 = 0;
                qint64 remaining_file_count_for_req_time_qint64;

                remaining_file_count_for_req_time_qint64 = total_records_num;
                qint64 start_time_epoch = QDateTime::currentSecsSinceEpoch();

                QString command_fs = QString("select INT, filepath from files");
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

                    struct_filesystem_record_and_filepath m1;
                    m1.filesystem_record = query.value(0).toString();
                    m1.filepath = query.value(1).toString();

                    if(recon_helper_standard_obj->bool_is_permission_restricted_file(m1.filepath, Q_FUNC_INFO))
                        continue;

                    list_struct_fs_rec_and_path << m1;
                }

                destination_db.close();
                mutext_for_fs_main_db->unlock();


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

                        estimated_time_QString = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");
                        processed_file_count_qint64 = 0;
                        start_time_epoch = current_time_epoch;
                    }
                    records_count++;

                    struct_filesystem_record_and_filepath fs_rec_filepath_obj = list_struct_fs_rec_and_path.at(num);

                    struct_global_apple_metadata_parsed_data extended_attr_parsed_data_obj;

                    QString file_full_path = target_struct_obj.fs_extraction_source_path + fs_rec_filepath_obj.filepath;

                    attribute_column_name_for_logical_image.clear();
                    bool file_exist = fill_apple_metadata_from_script(fs_rec_filepath_obj.filepath ,attributes_name_list_final , extended_attr_parsed_data_obj);
                    if(!file_exist)
                        continue;
                    attribute_column_list_final = attribute_column_name_for_logical_image;

                    extended_attr_parsed_data_obj.filesystem_record = fs_rec_filepath_obj.filesystem_record;

                    /// Save data for one record
                    QString command_str;

                    if(extended_attr_parsed_data_obj.extended_attrs_keys_list.size() == extended_attr_parsed_data_obj.extended_attrs_values_list.size()
                            && attribute_column_list_final.size() == extended_attr_parsed_data_obj.extended_attrs_keys_list.size())
                    {
                        command_str = QString("INSERT INTO files (filesystem_record,");
                        value_list.clear();

                        QString question_marks;

                        for(int num = 0; num < extended_attr_parsed_data_obj.extended_attrs_keys_list.size(); num++)
                        {
                            command_str.append(attribute_column_list_final.at(num)  + QString(","));
                            question_marks.append("?,");
                        }

                        if(command_str.endsWith(QString(",")))
                            command_str.chop(1);

                        if(question_marks.endsWith(QString(",")))
                            question_marks.chop(1);

                        command_str.append(") values(?,");

                        command_str.append(question_marks);

                        command_str.append(")");

                        value_list << fs_rec_filepath_obj.filesystem_record << extended_attr_parsed_data_obj.extended_attrs_values_list;

                        ///insert record
                        query_memory_db.clear();
                        query_memory_db.prepare(command_str);
                        for(int i = 0; i < value_list.size(); i++)
                        {
                            query_memory_db.addBindValue(value_list.at(i));
                        }
                        if(!query_memory_db.exec())
                        {
                            recon_static_functions::app_debug(" insert query  --------FAILED------ db file - " + in_memory_apple_metadata_db.databaseName() ,Q_FUNC_INFO);
                            recon_static_functions::app_debug(" command -  " + query_memory_db.executedQuery(),Q_FUNC_INFO);
                            recon_static_functions::app_debug(" error -  " + query_memory_db.lastError().text(),Q_FUNC_INFO);
                        }


                        fs_rec_db_updated_count++;
                        record_num_list << fs_rec_filepath_obj.filesystem_record;
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

                    if(estimated_time_QString.isEmpty() || estimated_time_QString == "00:00:00")
                        emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, "Calculating...");
                    else
                        emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, estimated_time_QString);

                    if(records_count % 100 == 0)
                    {
                        qint64 progress_val = ((records_count * 100) / total_records_num);
                        if(progress_val > 99)
                            progress_val = 99;

                        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, target_struct_obj.source_name, true, records_count, progress_val, false);

                    }
                }
                in_memory_apple_metadata_db.commit();

                recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_apple_metadata_db, Q_FUNC_INFO);

                in_memory_apple_metadata_db.close();


                create_and_insert_processed_attributes_list(apple_metadata_db_path);


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
                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, target_struct_obj.source_name, true, records_count, 100, false);

                if(!bool_cancel_extraction)
                    emit signal_completion_status(MACRO_JobType_Apple_Metadata, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

            }
        }
    }

    emit signal_add_apple_metadata_to_case(apple_metadata_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Apple_Metadata);
    emit finished();

}

void thread_apple_metadata::intermediate_save_data_to_apple_metadata_db(QString apple_metadata_database_path)
{

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(apple_metadata_database_path));

    QSqlQuery query_attach(in_memory_apple_metadata_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_insert(in_memory_apple_metadata_db);

    QString command_insert = QString("INSERT INTO filesystemDB.files SELECT * from files");
    query_insert.prepare(command_insert);
    if(!query_insert.exec())
    {
        recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_clear(in_memory_apple_metadata_db);
    QString command_clear = QString("DELETE from files");
    query_clear.prepare(command_clear);
    if(!query_clear.exec())
    {
        recon_static_functions::app_debug(" query memory delete  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_detach(in_memory_apple_metadata_db);
    QString command_detach = QString("DETACH DATABASE filesystemDB");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }
}


bool thread_apple_metadata::open_and_create_apple_metadata_db(QString apple_metadata_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_apple_metadata_db.setDatabaseName(apple_metadata_db_path);
    if(!in_memory_apple_metadata_db.open())
    {
        recon_static_functions::app_debug(" apple metadata Open  -------FAILED------ " + in_memory_apple_metadata_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_apple_metadata_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    return true;


    //    QFileInfo info_db (apple_metadata_db_path);
    //    if(info_db.exists())
    //    {
    //        if(info_db.size() <= 0 && recon_case_runner_type != QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    //            QFile::remove(apple_metadata_db_path);
    //        else
    //        {
    //            QString connection_naam = Q_FUNC_INFO;
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            in_memory_apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //            in_memory_apple_metadata_db.setDatabaseName(apple_metadata_db_path);
    //            if(!in_memory_apple_metadata_db.open())
    //            {
    //                recon_static_functions::app_debug(" apple metadata Open  -------FAILED------ " + in_memory_apple_metadata_db.databaseName(), Q_FUNC_INFO);
    //                recon_static_functions::app_debug(in_memory_apple_metadata_db.lastError().text() , Q_FUNC_INFO);
    //                return false;
    //            }

    //            return true;
    //        }
    //    }

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    in_memory_apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    in_memory_apple_metadata_db.setDatabaseName(":memory:");
    //    if(!in_memory_apple_metadata_db.open())
    //    {
    //        recon_static_functions::app_debug(" exif_data_db Open in memory -------FAILED------ " + in_memory_apple_metadata_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(in_memory_apple_metadata_db.lastError().text() , Q_FUNC_INFO);
    //        return false;
    //    }

    //    QString command_str = create_in_memory_db_table_command();

    //    QSqlQuery query_create_table(in_memory_apple_metadata_db);
    //    query_create_table.prepare(command_str);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file - ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        in_memory_apple_metadata_db.close();
    //        return false;
    //    }

    //    //    if(QFileInfo(apple_metadata_db_path).exists())
    //    //        QFile::remove(apple_metadata_db_path);

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
    //            .arg(QDir::toNativeSeparators(apple_metadata_db_path));

    //    QSqlQuery query_attach(in_memory_apple_metadata_db);
    //    query_attach.prepare(db_attach_commmand);
    //    if(!query_attach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_apple_metadata_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_save(in_memory_apple_metadata_db);
    //    query_save.prepare(QString("CREATE TABLE filesystemDB.files AS SELECT * from files"));
    //    if(!query_save.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Save  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
    //        in_memory_apple_metadata_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_detach(in_memory_apple_metadata_db);
    //    query_detach.prepare("DETACH DATABASE filesystemDB");
    //    if(!query_detach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Detach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_apple_metadata_db.close();
    //        return false;
    //    }

    //    return true;

}

bool thread_apple_metadata::open_fs_db(QString fs_db_path)
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

QString thread_apple_metadata::create_in_memory_db_table_command()
{
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
        if(bool_cancel_extraction)
            break;

        count++;

        if(count%10 == 0)
            QCoreApplication::processEvents();

        QString line = file.readLine();

        if(line.endsWith("\n"))
            line.chop(1);
        if(line.endsWith(",,,"))
            line.chop(3);

        QStringList list = line.split(",,,");

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

    //  file.remove();
    return command_str;

}

void thread_apple_metadata::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_apple_metadata::pub_set_essentials()
{
    set_all_attributes_list();
}


void thread_apple_metadata::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_apple_metadata::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;

}

void thread_apple_metadata::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_apple_metadata::pub_set_apple_metadata_all_attributes_name_list(QStringList attributes_name_list)
{
    apple_metadata_attributes_name_list = attributes_name_list;
}

void thread_apple_metadata::set_all_attributes_list()
{
    QString config_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString attributes_command_str = QString("Select metadata_attribute_name From tbl_apple_attributes Where bookmark_filter = '1' or bookmark_report = '1'");
    attributes_name_list_examiner_selected = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(attributes_command_str, 0 ,config_db_path , Q_FUNC_INFO);

    attributes_command_str = QString("Select custom_column_name From tbl_apple_attributes Where bookmark_filter = '1' or bookmark_report = '1'");
    column_name_list_examiner_selected = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(attributes_command_str, 0, config_db_path, Q_FUNC_INFO);


    QString all_apple_attributes_command_str = QString("Select metadata_attribute_name From tbl_apple_attributes");
    attributes_name_list_all = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(all_apple_attributes_command_str, 0, config_db_path, Q_FUNC_INFO);

    QString all_apple_attributes_columnname_command_str = QString("Select custom_column_name From tbl_apple_attributes");
    column_name_list_all = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(all_apple_attributes_columnname_command_str, 0, config_db_path, Q_FUNC_INFO);

}

void thread_apple_metadata::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_apple_metadata::extract_apple_metadata_for_fs_run_module()
{
    //emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, QString("Initializing......"), true, 0, 0, true);
    struct struct_fs_db_update
    {
        QString record_no;
        QString fs_db_path;
    };
    QList <struct_fs_db_update> fs_db_update_struct_list;

    qint64 total_records_num = run_fs_modules_details_list.size();
    qint64 records_count = 0;

    QString cmd_delete_existing = QString("Delete From files Where filesystem_record = ?");
    QString cmd_fs_db_update    = QString("Update files set fs_module_apple_metadata_run_status = '1' Where ");

    QStringList apple_metadata_db_path_list;
    QStringList delete_val_list;
    QMap <QString,QMap <QString,QMap <QString,QString> > > qmap_srccntname_map_filepath_map_metadata_key_value;

    QStringList apple_metadata_tmp_attrs_list;
    map_apple_metadata_for_recon_logical_image.clear();

    bool bool_need_to_extract_apple_metadata_from_QMap = false;
    bool bool_need_to_extract_apple_metadata_from_reader = false;

    int bunch_count = 50;
    int fs_db_updated_rec_count = 0;

    qint64 processed_file_count_qint64 = 0;
    qint64 remaining_file_count_for_req_time_qint64;

    remaining_file_count_for_req_time_qint64 = total_records_num;
    qint64 start_time_epoch = QDateTime::currentSecsSinceEpoch();

    QStringList source_cnt_name_list_for_reader;
    QStringList virtual_source_path_list;
    QStringList extracted_source_count_name;

    QString apple_metadata_essential_db_path = create_tmp_apple_metdata_essential_db_for_right_click();

    struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString os_version_examiner_machine = pwd_obj.os_version_full;
    if(pwd_obj.bool_desktop_pwd_accurate)
    {
        remaining_file_count_for_req_time_qint64 = run_fs_modules_details_list.size();
        remain_file_count_for_req_time_for_proc_qint64 = run_fs_modules_details_list.size();

        for(int dd = 0; dd < run_fs_modules_details_list.size(); dd++)
        {
            bool_need_to_extract_apple_metadata_from_QMap = false;

            struct_global_run_fs_modules_details fs_modules_details_obj = run_fs_modules_details_list.at(dd);
            QString source_count_name = fs_modules_details_obj.source_count_name;
            struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);

            if(target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
                    && target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
            {
                continue;
            }

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, ("Initializing... " + target_struct_obj.source_name), true, MACRO_Generic_Progress_No_Records_int, -1, false);

            QString apple_metadata_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();
            QString out_put_path = apple_metadata_path + target_struct_obj.source_count_name + "/";
            QString txt_file_path = out_put_path + MACRO_Spotlight_Extracted_Text_File_Name;
            QString csv_file_pth = out_put_path + MACRO_Spotlight_Extracted_CSV_File_Name;

            attribute_column_list_final = column_name_list_all;
            attributes_name_list_final = attributes_name_list_all;
            bool_need_to_extract_apple_metadata_from_QMap = true;
            if(extracted_source_count_name.contains(source_count_name))
                continue;

            apple_metadata_tmp_attrs_list = attributes_name_list_final;
            extracted_source_count_name << source_count_name;
            QMap <QString ,QMap <QString ,QString> > map_path_map_metadata_key_value = recon_helper_standard_obj->extract_apple_metadata_from_script(txt_file_path,csv_file_pth,attributes_name_list_final);
            qmap_srccntname_map_filepath_map_metadata_key_value.insert(source_count_name,map_path_map_metadata_key_value);
        }
    }
    else
    {
        remaining_file_count_for_req_time_qint64 = run_fs_modules_details_list.size();
        remain_file_count_for_req_time_for_proc_qint64 = run_fs_modules_details_list.size();

        for(int dd = 0; dd < run_fs_modules_details_list.size(); dd++)
        {
            bool_need_to_extract_apple_metadata_from_QMap = false;
            struct_global_run_fs_modules_details fs_modules_details_obj = run_fs_modules_details_list.at(dd);
            QString source_count_name = fs_modules_details_obj.source_count_name;
            struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);
            QString apple_metadata_path =   global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();
            QString out_put_path = apple_metadata_path + target_struct_obj.source_count_name + "/";
            QString txt_file_path = out_put_path + MACRO_Spotlight_Extracted_Text_File_Name;
            QString csv_file_pth = out_put_path + MACRO_Spotlight_Extracted_CSV_File_Name;
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, ("Initializing... " + target_struct_obj.source_name), true, MACRO_Generic_Progress_No_Records_int, -1, false);

            attribute_column_list_final = column_name_list_all;
            attributes_name_list_final = attributes_name_list_all;
            bool_need_to_extract_apple_metadata_from_QMap = true;
            if(extracted_source_count_name.contains(source_count_name))
                continue;

            apple_metadata_tmp_attrs_list = attributes_name_list_final;
            extracted_source_count_name << source_count_name;
            QMap <QString ,QMap <QString ,QString> > map_path_map_metadata_key_value = recon_helper_standard_obj->extract_apple_metadata_from_script(txt_file_path,csv_file_pth,attributes_name_list_final);
            qmap_srccntname_map_filepath_map_metadata_key_value.insert(source_count_name,map_path_map_metadata_key_value);
        }
    }

    if(bool_need_to_extract_apple_metadata_from_reader)
    {
        source_cnt_name_list_for_reader.removeDuplicates();
        virtual_source_path_list.removeDuplicates();
        for(int jj = 0; jj < source_cnt_name_list_for_reader.size(); jj++)
        {
            if(bool_cancel_extraction)
                break;

            QString key_source_count_name = source_cnt_name_list_for_reader.at(jj);
            struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(key_source_count_name);

            if(!QFileInfo::exists(target_struct_obj.fs_extraction_source_path))
                continue;

            if(target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
                    && target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
                continue;

            extract_apple_metadata_from_apple_metadata_reader(apple_metadata_essential_db_path,key_source_count_name,virtual_source_path_list.at(jj),QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_metadata_right_click),enum_apple_metadata_runner_status_right_click);
        }
    }

    if(bool_need_to_extract_apple_metadata_from_QMap)
    {
        remaining_file_count_for_req_time_qint64 = run_fs_modules_details_list.size();

        for(int count = 0; count < run_fs_modules_details_list.size(); count++)
        {
            qint64 current_time_epoch = QDateTime::currentSecsSinceEpoch();
            remaining_file_count_for_req_time_qint64--;
            processed_file_count_qint64++;

            if(current_time_epoch - start_time_epoch >= 5)
            {
                double avg_image_per_sec = processed_file_count_qint64 / (current_time_epoch - start_time_epoch);
                qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;

                qint64 required_time = new_ramining_file_count / avg_image_per_sec;

                estimated_time_QString = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");

                processed_file_count_qint64 = 0;
                start_time_epoch = current_time_epoch;
            }

            struct_global_run_fs_modules_details fs_modules_details_obj = run_fs_modules_details_list.at(count);
            struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);
            QString mac_version = target_struct_obj.os_version_extended;
            if(bool_cancel_extraction)
                break;

            if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
                continue;


            if(target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
                    && target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
            {
                continue;
            }

            if(source_cnt_name_list_for_reader.contains(fs_modules_details_obj.source_count_name))
                continue;

            if(recon_helper_standard_obj->bool_is_permission_restricted_file(fs_modules_details_obj.complete_file_path, Q_FUNC_INFO))
                continue;

            QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/apple_metadata.sqlite");

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");


            if(!open_and_create_apple_metadata_db(apple_metadata_db_path))
                continue;

            if(in_memory_apple_metadata_db.isOpen() && in_memory_apple_metadata_db.databaseName() == apple_metadata_db_path)
            {
                ///db for same source already open no need to open again
            }
            else
            {
                apple_metadata_db_path_list << apple_metadata_db_path;

                if(in_memory_apple_metadata_db.isOpen())
                {

                    in_memory_apple_metadata_db.commit();
                    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_apple_metadata_db, Q_FUNC_INFO);
                    in_memory_apple_metadata_db.close();

                    QSqlDatabase::removeDatabase(in_memory_apple_metadata_db.connectionName());


                }

                in_memory_apple_metadata_db.transaction();
            }

            QString file_full_path = fs_modules_details_obj.complete_file_path;

            records_count++;

            struct_global_apple_metadata_parsed_data extended_attr_parsed_data_obj;


            attribute_column_name_for_logical_image.clear();
            map_apple_metadata_for_recon_logical_image = qmap_srccntname_map_filepath_map_metadata_key_value.value(target_struct_obj.source_count_name);

            bool file_exist = fill_apple_metadata_from_script(fs_modules_details_obj.display_filepath ,apple_metadata_tmp_attrs_list , extended_attr_parsed_data_obj);
            if(!file_exist)
                continue;

            attribute_column_list_final = attribute_column_name_for_logical_image;

            extended_attr_parsed_data_obj.filesystem_record = fs_modules_details_obj.fs_record_no;

            /// Save data for one record
            QString command_str;
            QStringList value_list;

            if(extended_attr_parsed_data_obj.extended_attrs_keys_list.size() == extended_attr_parsed_data_obj.extended_attrs_values_list.size()
                    && attribute_column_list_final.size() == extended_attr_parsed_data_obj.extended_attrs_keys_list.size())
            {
                command_str = QString("INSERT INTO files (filesystem_record,");
                value_list.clear();

                QString question_marks;

                for(int num = 0; num < extended_attr_parsed_data_obj.extended_attrs_keys_list.size(); num++)
                {
                    if(bool_cancel_extraction)
                        break;

                    command_str.append(attribute_column_list_final.at(num)  + QString(","));

                    question_marks.append("?,");
                }

                if(command_str.endsWith(QString(",")))
                    command_str.chop(1);

                if(question_marks.endsWith(QString(",")))
                    question_marks.chop(1);

                command_str.append(") values(?,");

                command_str.append(question_marks);

                command_str.append(")");

                value_list << extended_attr_parsed_data_obj.filesystem_record << extended_attr_parsed_data_obj.extended_attrs_values_list;

                delete_val_list.clear();
                delete_val_list << extended_attr_parsed_data_obj.filesystem_record;
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_delete_existing , delete_val_list , in_memory_apple_metadata_db , Q_FUNC_INFO);

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_str , value_list , in_memory_apple_metadata_db , Q_FUNC_INFO);

                struct_fs_db_update fs_db_update_obj;
                fs_db_update_obj.record_no = fs_modules_details_obj.fs_record_no;
                fs_db_update_obj.fs_db_path = fs_db_path;

                fs_db_update_struct_list.append(fs_db_update_obj);

                fs_db_updated_rec_count++;

                if(fs_db_updated_rec_count == bunch_count)
                {
                    mutext_for_fs_main_db->lock();

                    bool bool_list_processed = false;

                    QString tmp_cmd;
                    int pos = 0;
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

                //intermediate_save_data_to_apple_metadata_db(apple_metadata_db_path);
            }

            if(estimated_time_QString.isEmpty() || estimated_time_QString == "00:00:00")
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, "Calculating...");
            else
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, estimated_time_QString);

            if(records_count % 100 == 0)
            {
                int progress_val = ((records_count * 100) / total_records_num);
                if(progress_val > 99)
                    progress_val = 99;

                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, fs_modules_details_obj.examiner_selected_file_or_dir_name, true, records_count, progress_val, false);
            }

        }
    }
    apple_metadata_db_path_list.removeDuplicates();


    //    in_memory_apple_metadata_db.commit();
    //    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_apple_metadata_db, Q_FUNC_INFO);
    //    in_memory_apple_metadata_db.close();
    //    QSqlDatabase::removeDatabase(in_memory_apple_metadata_db.connectionName());

    for(int count = 0; count < apple_metadata_db_path_list.size(); count++)
    {
        if(bool_cancel_extraction)
            break;
        create_and_insert_processed_attributes_list(apple_metadata_db_path_list.at(count));
    }

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





    QStringList aple_mtd_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString aple_mt_db_path_2 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/apple_metadata.sqlite");

        aple_mtd_db_path_list << aple_mt_db_path_2;
    }
    emit signal_add_apple_metadata_to_case(aple_mtd_db_path_list);
    run_fs_modules_details_list.clear();

}

void thread_apple_metadata::create_and_insert_processed_attributes_list(QString apple_metadata_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(apple_metadata_db_path);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug("db open ------FAILED-------" + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db.lastError().text() ,Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    destination_db.transaction();
    QString command_str = QString("create table IF NOT EXISTS processed_attributes (INT INTEGER PRIMARY KEY, attribute_name varchar(100))");

    recon_helper_standard_obj->execute_db_command_by_dbreference(command_str, destination_db, Q_FUNC_INFO);

    recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete from processed_attributes"), destination_db, Q_FUNC_INFO);

    QString command = QString("Insert into processed_attributes(attribute_name) Values(?)");
    for(int count = 0; count < attributes_name_list_final.size(); count++)
    {
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, QStringList(attributes_name_list_final.at(count)), destination_db, Q_FUNC_INFO);
    }

    destination_db.commit();
    destination_db.close();
}

bool thread_apple_metadata::fill_apple_metadata_from_script(QString file_path, QStringList keys_list, struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj)
{
    QMap <QString , QString > tmp_qmap;
    tmp_qmap =  map_apple_metadata_for_recon_logical_image.value(file_path);
    if(tmp_qmap.isEmpty())
    {
        return false;
    }

    QMapIterator <QString , QString > map_iterator(tmp_qmap);

    while(map_iterator.hasNext())
    {
        map_iterator.next();
        QString key_str = map_iterator.key();

        int pos = keys_list.indexOf(key_str);

        attribute_column_name_for_logical_image << column_name_list_all.at(pos);

        extended_attr_parsed_data_obj.extended_attrs_keys_list << key_str;
        extended_attr_parsed_data_obj.extended_attrs_values_list << map_iterator.value();
    }

    return true;

}

void thread_apple_metadata::extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name,QString virtual_source_path,QString m_flag, int runner_type)
{

    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString file_system_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();

    QString exec_path = QDir::currentPath() + "/apple_metadata_reader.app/Contents/MacOS/apple_metadata_reader";

    QTimer *timer_obj = new QTimer(this);
    connect(timer_obj,SIGNAL(timeout()),this,SLOT(slot_timeout_to_update_estimated_time_for_fs_modules()));
    timer_obj->start(5000);

    QString apple_metadata_attr_names_str,apple_metadata_column_name;
    apple_metadata_attr_names_str = attributes_name_list_examiner_selected.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);
    apple_metadata_column_name = apple_metadata_column_name = column_name_list_examiner_selected.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);

    if(m_flag == QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_metadata_left_click))
    {
        apple_metadata_attr_names_str = attributes_name_list_all.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);
        apple_metadata_column_name = column_name_list_all.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);
    }


    struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString system_pswd = pwd_obj.desktop_password_original;
    bool bool_script_run_with_root_prevs = false;

    QString cmd_load;
    if(pwd_obj.bool_desktop_pwd_accurate)
    {
        //cmd_load = QString("echo '") + system_pswd + "' | sudo -S " + exec_path.replace(" ", "\\ ") + " " + "\"" + virtual_source_path + "\"" +  " " + "\"" + file_paths_str + "\"" +" " + "\"" + file_system_apple_metadata + "\""
        //           + " " + "\"" + apple_metadata_attr_names_str + "\"" + " " +  "\"" + apple_metadata_column_name + "\"" + " " +  "\"" + source_cnt_name + "\"" + " " +  "\"" + m_flag + "\"";

        cmd_load = QString("echo $FORENSOPASSWD")   + " | sudo -S " + exec_path.replace(" ", "\\ ") + " " + "\"" + virtual_source_path + "\"" +  " " + "\"" + file_paths_str + "\"" +" " + "\"" + file_system_apple_metadata + "\""
                + " " + "\"" + apple_metadata_attr_names_str + "\"" + " " +  "\"" + apple_metadata_column_name + "\"" + " " +  "\"" + source_cnt_name + "\"" + " " +  "\"" + m_flag + "\"";

        bool_script_run_with_root_prevs = true;
    }
    else
    {
        cmd_load = exec_path.replace(" ", "\\ ") + " " + "\"" + virtual_source_path + "\"" +  " " + "\"" + file_paths_str + "\"" +" " + "\"" + file_system_apple_metadata + "\""
                + " " + "\"" + apple_metadata_attr_names_str + "\"" + " " +  "\"" + apple_metadata_column_name + "\"" + " " +  "\"" + source_cnt_name + "\"" + " " +  "\"" + m_flag + "\"";
    }

    QString available_file = recon_static_functions::get_available_filename("apple_metadata",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString script_path = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;
    qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_path,cmd_load, Q_FUNC_INFO);


    if(byte_size == -1)
        return;

    QProcess *prc_mdtada;
    prc_mdtada = nullptr;
    if(runner_type == enum_apple_metadata_runner_status_full_source)
    {
        prc_mdtada = process_apple_metadata_reader_for_full_source;
    }
    else if(runner_type == enum_apple_metadata_runner_status_right_click)
    {
        prc_mdtada = process_apple_metadata_reader_for_right_click;
    }
    else if(runner_type == enum_apple_metadata_runner_status_left_click)
    {
        prc_mdtada = process_apple_metadata_reader_for_left_click;
    }



    if(prc_mdtada->state() == QProcess::NotRunning)
    {
        if(bool_script_run_with_root_prevs)
        {
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("FORENSOPASSWD", system_pswd);

            QStringList args;
            args << script_path;

            prc_mdtada->setProcessEnvironment(env);
            prc_mdtada->start("bash", args);

        }
        else
        {
            prc_mdtada->start("sh", QStringList(script_path));
        }
        if (!prc_mdtada->waitForStarted())
        {
            recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + prc_mdtada->errorString() ,Q_FUNC_INFO);
            return;
        }

    }
    recon_static_functions::debug_conditional(" : running111", Q_FUNC_INFO);

    while(1)
    {
        prc_mdtada->waitForReadyRead();

        if(prc_mdtada->state() == QProcess::NotRunning)
        {
            break;
        }
    }

    timer_obj->stop();

    if(m_flag != QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_metadata_left_click))
    {
        struct_GLOBAL_witness_info_source target_struct_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);

        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata,MACRO_APPLE_METADATA_Finalize_Message, true, -1, -1, true);

        QString tmp_apple_timestamps_db_path = file_system_apple_metadata + source_cnt_name + QString("/apple_metadata.sqlite");
        QString actual_apple_timestamp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + source_cnt_name + QString("/apple_metadata.sqlite");

        QString tmp_file_system_db_path = file_system_apple_metadata + source_cnt_name + QString("/file_system.sqlite");
        QString actual_file_system_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + source_cnt_name + QString("/file_system.sqlite");

        copy_tmp_apple_metadata_to_actual_apple_timestamps_db(tmp_apple_timestamps_db_path,actual_apple_timestamp_db_path);

        if(!bool_cancel_extraction)
        {
            if(m_flag == QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_metadata_full_source))
            {
                emit signal_completion_status(MACRO_JobType_Apple_Metadata, source_cnt_name, Macro_JobType_Completion_status_done);
            }
        }
        mutext_for_fs_main_db->lock();
        update_filesystem_db_for_fs_run_module_status(tmp_file_system_db_path,actual_file_system_db_path);
        mutext_for_fs_main_db->unlock();

    }
    if(m_flag == QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_metadata_right_click))
    {
        QFile file(file_paths_str);
        file.remove();
    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void thread_apple_metadata::pub_set_apple_metadata_reader_process_for_full_source(QProcess *mobj)
{
    process_apple_metadata_reader_for_full_source = mobj;
}
void thread_apple_metadata::pub_set_apple_metadata_reader_process_for_right_click(QProcess *mobj)
{
    process_apple_metadata_reader_for_right_click = mobj;
}

void thread_apple_metadata::pub_set_apple_metadata_reader_process_for_left_click(QProcess *mobj)
{
    process_apple_metadata_reader_for_left_click = mobj;
}


void thread_apple_metadata::slot_prc_apple_metadata_reader_for_full_source_readyread()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString prc_output = process_apple_metadata_reader_for_full_source->readAll();
    prc_output.remove("\"");

    if(prc_output.startsWith(QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Record)))
    {
        QString record_count = prc_output;
        record_count.remove(0,QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Record).size());
        record_count = record_count.trimmed();
        if(record_count.isEmpty())
            return;
        QStringList tmp_list = record_count.split(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Ampersand);

        QString source_cnt_name = tmp_list.at(0).trimmed();
        QString process_records = tmp_list.at(1).trimmed();
        QString total_records = tmp_list.at(2).trimmed();


        int process_record_cnt = process_records.toInt();
        qint64 total_record_cnt = total_records.toLongLong();

        processed_file_count_for_proc_qint64++;
        remain_file_count_for_req_time_for_proc_qint64--;

        if(estimated_time_for_proc.isEmpty() || estimated_time_for_proc == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, estimated_time_QString);

        if(total_record_cnt > 0)
        {
            struct_GLOBAL_witness_info_source target_struct_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);

            int progress_val = (int)((process_record_cnt * 100) / total_record_cnt);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, target_struct_obj.source_name, true, process_record_cnt, progress_val, false);
        }
    }
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void thread_apple_metadata::slot_prc_apple_metadata_reader_for_right_click_readyread()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString prc_output = process_apple_metadata_reader_for_right_click->readAll();
    prc_output.remove("\"");

    if(prc_output.startsWith(QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Record)))
    {
        QString record_count = prc_output;
        record_count.remove(0,QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Record).size());
        record_count = record_count.trimmed();
        if(record_count.isEmpty())
            return;
        QStringList tmp_list = record_count.split(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Ampersand);

        QString source_file_path = tmp_list.at(0).trimmed();
        QString process_records = tmp_list.at(1).trimmed();
        QString total_records = tmp_list.at(2).trimmed();

        QString source_file_name = QFileInfo(source_file_path).fileName();
        int process_record_cnt = process_records.toInt();
        qint64 total_record_cnt = total_records.toLongLong();

        processed_file_count_for_proc_qint64++;
        remain_file_count_for_req_time_for_proc_qint64--;

        if(estimated_time_for_proc.isEmpty() || estimated_time_for_proc == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Apple_Metadata, estimated_time_QString);

        if(total_record_cnt > 0)
        {

            int progress_val = (int)((process_record_cnt * 100) / total_record_cnt);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, source_file_name, true, process_record_cnt, progress_val, false);
        }
    }
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void thread_apple_metadata::copy_tmp_apple_metadata_to_actual_apple_timestamps_db(QString tmp_db_path,QString actual_db_path)
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    QSqlDatabase tmp_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    tmp_db.setDatabaseName(tmp_db_path);
    //    if(!tmp_db.open())
    //    {
    //        recon_static_functions::app_debug("db open ---FAILED--- " + tmp_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(tmp_db.lastError().text() , Q_FUNC_INFO);
    //    }

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadata_db")
    //            .arg(QDir::toNativeSeparators(actual_db_path));

    //    QSqlQuery attach_query(tmp_db);
    //    attach_query.prepare(db_attach_commmand);
    //    if(!attach_query.exec())
    //    {
    //        recon_static_functions::app_debug(" Attach query(tmp) --------FAILED------ " + attach_query.executedQuery(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + attach_query.lastError().text(),Q_FUNC_INFO);
    //    }


    //    QSqlQuery insert_query(tmp_db);

    //    QString insert_command = QString("Insert into apple_metadata_db.files (filesystem_record,");

    //    QString command_str;
    //    for(int num = 0; num < attribute_column_list_final.size(); num++)
    //    {
    //        command_str.append(attribute_column_list_final.at(num)  + QString(","));
    //    }

    //    if(command_str.endsWith(","))
    //        command_str.chop(1);


    //    QString final_cmd =  insert_command + command_str + ") select filesystem_record," + command_str + " from main.files";

    //    recon_helper_standard_obj->execute_db_command_by_dbreference(final_cmd,tmp_db,Q_FUNC_INFO);

    //    QSqlQuery query_detach(tmp_db);
    //    QString command_detach = QString("DETACH DATABASE apple_metadata_db");
    //    query_detach.prepare(command_detach);
    //    if(!query_detach.exec())
    //    {
    //        recon_static_functions::app_debug(" query  detach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    //        return;
    //    }

    //    tmp_db.close();
    //====================================================================================================================================//
    QFile actual_file(actual_db_path);
    if(actual_file.exists())
    {
        actual_file.remove();
    }

    if(QFile(tmp_db_path).exists())
    {
        QFile::copy(tmp_db_path,actual_db_path);
    }

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}


void thread_apple_metadata::update_filesystem_db_for_fs_run_module_status(QString tmp_fs_db_path ,QString actual_fs_db_path)
{

    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase tmp_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    tmp_db.setDatabaseName(tmp_fs_db_path);
    if(!tmp_db.open())
    {
        recon_static_functions::app_debug("Db open ---FAILED--- " + tmp_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(tmp_db.lastError().text() , Q_FUNC_INFO);
    }
    QString select_query_1 = "select INT from main.files where fs_module_apple_metadata_run_status = 1";
    QStringList recod_no_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_query_1,0,tmp_db,Q_FUNC_INFO);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystem_db")
            .arg(QDir::toNativeSeparators(actual_fs_db_path));

    QSqlQuery attach_query(tmp_db);
    attach_query.prepare(db_attach_commmand);
    if(!attach_query.exec())
    {
        recon_static_functions::app_debug(" Attach query(tmp) --------FAILED------ " + attach_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + attach_query.lastError().text(),Q_FUNC_INFO);
    }


    QSqlQuery insert_query(tmp_db);
    QSqlQuery select_query(tmp_db);

    QString update_command = QString("Update filesystem_db.files set fs_module_apple_metadata_run_status = ? where INT = ?");
    //QString select_query_cmd = "Select fs_module_apple_metadata_run_status from main.files where INT = ?";
    //QStringList val_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_query_cmd,0,tmp_db,Q_FUNC_INFO);
    for(int jj = 0; jj < recod_no_list.size(); jj++)
    {
        QString record_no = recod_no_list.at(jj);
        QStringList val_list;
        val_list << "1" << record_no;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_command,val_list,tmp_db,Q_FUNC_INFO);
    }

    QSqlQuery query_detach(tmp_db);
    QString command_detach = QString("DETACH DATABASE filesystem_db");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query  detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    tmp_db.close();

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

void thread_apple_metadata::pub_extract_apple_metadata_from_apple_metadata_reader_on_file_click(QString file_paths_str, QString source_cnt_name,QString virtual_source_path,QStringList apple_metadata_att_list,QStringList column_name_list)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    attributes_name_list_all = apple_metadata_att_list;
    column_name_list_all = column_name_list;
    extract_apple_metadata_from_apple_metadata_reader(file_paths_str, source_cnt_name, virtual_source_path,QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_metadata_left_click),enum_apple_metadata_runner_status_left_click);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void thread_apple_metadata::insert_source_info_in_apple_metadata_essential_db_for_right_click_extraction(QString fs_record_no,QString display_file_path,QString source_count_name)
{
    QString tmp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Apple_Metadata_In_Result_QString).toString();

    QDir carve_dir(tmp_dir_path);
    if(!carve_dir.exists())
        carve_dir.mkpath(tmp_dir_path);

    QString apple_metadata_essential_db_file = tmp_dir_path + "apple_metadata_essential.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    apple_metadata_db.setDatabaseName(apple_metadata_essential_db_file);
    if(!apple_metadata_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + apple_metadata_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query(apple_metadata_db);

    apple_metadata_db.transaction();
    query.prepare("Insert into files(fs_record_no,display_filepath,source_count_name)values(?,?,?)");
    query.addBindValue(fs_record_no);
    query.addBindValue(display_file_path);
    query.addBindValue(source_count_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(source_count_name)  --------FAILED------ db file - " + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
    }

    apple_metadata_db.commit();
    apple_metadata_db.close();
}


QString thread_apple_metadata::create_tmp_apple_metdata_essential_db_for_right_click()
{
    QString tmp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Apple_Metadata_In_Result_QString).toString();

    QDir tmp_dir(tmp_dir_path);
    if(!tmp_dir.exists())
        tmp_dir.mkpath(tmp_dir_path);

    QString apple_metadata_essential_db_file = tmp_dir_path + "apple_metadata_essential.sqlite";

    if(QFile(apple_metadata_essential_db_file).exists())
        QFile::remove(apple_metadata_essential_db_file);

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    apple_metadata_db.setDatabaseName(apple_metadata_essential_db_file);
    if(!apple_metadata_db.open())
    {
        recon_static_functions::app_debug(" apple_metadata_db  opening --------FAILED------ " + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + apple_metadata_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return apple_metadata_essential_db_file;
    }

    QSqlQuery query(apple_metadata_db);

    QString command = "create table files (INT INTEGER PRIMARY KEY, fs_record_no varchar(100), display_filepath varchar(1000), source_count_name varchar(100))";
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        apple_metadata_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return apple_metadata_essential_db_file;
    }

    return apple_metadata_essential_db_file;
}

void thread_apple_metadata::slot_timeout_to_update_estimated_time_for_fs_modules()
{
    double avg_image_per_sec = processed_file_count_for_proc_qint64 / 5;

    qint64 new_ramining_file_count = remain_file_count_for_req_time_for_proc_qint64;
    double required_time = new_ramining_file_count / avg_image_per_sec;

    estimated_time_for_proc = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");

    processed_file_count_for_proc_qint64 = 0;
}
