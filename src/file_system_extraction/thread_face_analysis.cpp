#include "thread_face_analysis.h"

thread_face_analysis::thread_face_analysis(QObject *parent) : QObject(parent)
{
    facefinder_obj = new facefinder();
    recon_helper_standard_obj = new recon_helper_standard(this);

}

void thread_face_analysis::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_face_analysis::pub_set_job_selected_source_count_name_list(QStringList type)
{
    job_selected_source_count_name_list = type;
}

void thread_face_analysis::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_face_analysis::pub_set_face_finder_obj(facefinder *obj)
{
    facefinder_obj = obj;
}

void thread_face_analysis::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_face_analysis::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_face_analysis::slot_extract_faces()
{ // extract face from here
    // we use dlib library and it's functions for face extraction
    // here we run face extraction on records, save faces paths in the face_analysis.sqlite and update run status in main file_system.sqlite to avoid duplicacy
    bool_cancel_extraction = false;

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    if(recon_case_runner_type != MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        emit signal_PBAR_thread_file_system_started(MACRO_JobType_Face_Analysis);

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        extract_face_analysis_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Face_Analysis);
        emit finished();
        return;
    }


    QString cmd_fs_db_update = QString("Update files set fs_module_face_analysis_run_status = '1' Where ");
    int bunch_count = 50;
    QStringList face_db_path_list;

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_extraction)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        QString face_analysis_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/face_analysis.sqlite");

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            face_db_path_list << face_analysis_db_path;
        }
        else
        {

            face_db_path_list << face_analysis_db_path;

            if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
                continue;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            list_struct_fs_rec_and_path.clear();

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");


            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Face_Analysis, target_struct_obj.source_name, true, 0, 0, false);

            if(!open_and_create_face_analysis_db(face_analysis_db_path))
                continue;

            list_struct_fs_rec_and_path.clear();
            mutext_for_fs_main_db->lock();
            if(!open_fs_db(fs_db_path))
            {
                in_memory_face_analysis_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files Where fs_module_face_analysis_run_status='0'") , 0, destination_db, Q_FUNC_INFO);
            qint64  total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                destination_db.close();
                in_memory_face_analysis_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            QString command_fs;
            if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                command_fs = QString("select INT, id_path, file_size, mime_type from files Where fs_module_face_analysis_run_status='0'");
            }
            else
            {
                command_fs = QString("select INT, filepath, file_size, mime_type from files Where fs_module_face_analysis_run_status='0'");
            }

            QSqlQuery query(destination_db);

            if(!query.exec(command_fs))
            {
                destination_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Face_Analysis_Minimum_File_Size_Bytes_qint64).toLongLong();

            while(query.next())
            {
                if(bool_cancel_extraction)
                    break;

                if(query.value(1).toString().trimmed().isEmpty())
                    continue;

                ///=========Skip-Files=======///
                qint64 file_size =  query.value("file_size").toLongLong();

                if(file_size <= pref_min_file_size )
                    continue;

                QString filepath = query.value(1).toString();
                if(recon_helper_standard_obj->bool_is_permission_restricted_file(filepath, Q_FUNC_INFO))
                    continue;


                struct_filesystem_record_and_filepath m1;
                m1.filesystem_record = query.value(0).toString();
                m1.filepath = filepath;
                m1.mime_type = query.value("mime_type").toString();

                list_struct_fs_rec_and_path << m1;
            }

            destination_db.close();
            mutext_for_fs_main_db->unlock();

            qint64 records_count = 0;
            qint64 hit_count = 0;

            QStringList record_num_list;
            int fs_rec_db_updated_count = 0;

            QSqlQuery insert_query(in_memory_face_analysis_db);

            for(int num = 0; num < list_struct_fs_rec_and_path.size(); num++)
            {
                if(bool_cancel_extraction)
                    break;

                records_count++;

                struct_filesystem_record_and_filepath fs_rec_filepath_obj = list_struct_fs_rec_and_path.at(num);

                QString file_full_path = target_struct_obj.fs_extraction_source_path + fs_rec_filepath_obj.filepath;

                bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
                if(!bool_is_file)
                    continue;


                QString mime_type_for_file = fs_rec_filepath_obj.mime_type;

                //                if(mime_type_for_file.startsWith("image/") && (mime_type_for_file.contains("jpeg", Qt::CaseInsensitive) || mime_type_for_file.contains("jpg", Qt::CaseInsensitive) || mime_type_for_file.contains("png", Qt::CaseInsensitive)) && !QFileInfo(file_full_path).fileName().startsWith(".") )
                if(mime_type_for_file.startsWith("image/") && (mime_type_for_file.contains("jpeg", Qt::CaseInsensitive) || mime_type_for_file.contains("jpg", Qt::CaseInsensitive)) && !QFileInfo(file_full_path).fileName().startsWith(".") )
                {
                    hit_count++;

                    QImageReader reader(file_full_path);

                    QImage img = reader.read();

                    if(img.isNull())
                        continue;

                    bool bool_face_status = facefinder_obj->check_is_image_have_face(file_full_path, fs_rec_filepath_obj.filesystem_record, target_struct_obj.source_count_name);

                    if(bool_face_status)
                    {
                        QString command = QString("insert into files (filesystem_record) values(?)");

                        insert_query.prepare(command);
                        insert_query.addBindValue(fs_rec_filepath_obj.filesystem_record);

                        if(!insert_query.exec())
                        {

                            recon_static_functions::app_debug(" query insert  --------FAILED------ ",Q_FUNC_INFO);
                            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
                            continue;
                        }
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

                if(records_count % 100 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);

                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Face_Analysis, target_struct_obj.source_name, true, records_count, progress_val, false);
                }
            }

            recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_face_analysis_db, Q_FUNC_INFO);
            in_memory_face_analysis_db.close();

            if(!record_num_list.isEmpty())
            {
                mutext_for_fs_main_db->lock();

                QString tmp_cmd;
                int pos = 0;
                for(int count = (record_num_list.size() -1); count >= 0; count--)
                {
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
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Face_Analysis, target_struct_obj.source_name, true, records_count, 100, false);

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Face_Analysis, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);
        }
    }

    emit signal_add_faces_to_case_tree(face_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Face_Analysis);
    emit finished();
}

bool thread_face_analysis::open_and_create_face_analysis_db(QString path)
{
    QFileInfo info_db (path);
    if(info_db.exists())
    {
        QString connection_naam = Q_FUNC_INFO;
        QSqlDatabase::removeDatabase(connection_naam);
        in_memory_face_analysis_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        in_memory_face_analysis_db.setDatabaseName(path);
        if(!in_memory_face_analysis_db.open())
        {
            recon_static_functions::app_debug(" face_analysis_db Open  -------FAILED------ " + in_memory_face_analysis_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(in_memory_face_analysis_db.lastError().text() , Q_FUNC_INFO);
            return false;
        }

        return true;
    }

}

bool thread_face_analysis::open_fs_db(QString db_path)
{
    bool db_status = false;

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
    }
    else
        db_status = true;


    return db_status;
}

void thread_face_analysis::extract_face_analysis_for_fs_run_module()
{ // run face extraction from right click action on file system records
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

    qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Face_Analysis_Minimum_File_Size_Bytes_qint64).toLongLong();

    QString cmd_fs_db_update = QString("Update files set fs_module_face_analysis_run_status = '1' Where ");


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

        if(current_time_epoch - start_time_epoch >= 10)
        {
            double avg_image_per_sec = processed_file_count_qint64 / (current_time_epoch - start_time_epoch);
            qint64 new_remaining_file_count = remaining_file_count_for_req_time_qint64;
            double required_time = new_remaining_file_count / avg_image_per_sec;

            estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");
            start_time_epoch = current_time_epoch;
            processed_file_count_qint64 = 0;
        }

        struct_global_run_fs_modules_details fs_modules_details_obj = run_fs_modules_details_list.at(count);

        ///=========Skip-Files=======///
        if(recon_helper_standard_obj->bool_is_permission_restricted_file(fs_modules_details_obj.complete_file_path, Q_FUNC_INFO))
            continue;


        struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);

                if(records_count == 0)
                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Face_Analysis, target_struct_obj.source_name, true, 0, 0, false);

        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;


        QString face_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/face_analysis.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");


        if(in_memory_face_analysis_db.isOpen() && in_memory_face_analysis_db.databaseName() == face_db_path)
        {
            ///db for same source already open no need to open again
        }
        else
        {
            if(in_memory_face_analysis_db.isOpen())
            {
                in_memory_face_analysis_db.commit();
                recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_face_analysis_db, Q_FUNC_INFO);
                in_memory_face_analysis_db.close();

                QSqlDatabase::removeDatabase(in_memory_face_analysis_db.connectionName());
            }
            if(!open_and_create_face_analysis_db(face_db_path))
                continue;

            in_memory_face_analysis_db.transaction();
        }

        records_count++;

        QString file_full_path = fs_modules_details_obj.complete_file_path;

        bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
        if(!bool_is_file)
            continue;

        QFileInfo file_info(file_full_path);
        qint64 file_size = file_info.size();

        if(file_size < pref_min_file_size)
            continue;


        var = obj.mimeTypeForFile(file_full_path);
        QString mime_type_for_file = var.name();

        if(mime_type_for_file.startsWith("image/") && (mime_type_for_file.contains("jpeg", Qt::CaseInsensitive) || mime_type_for_file.contains("jpg", Qt::CaseInsensitive) || mime_type_for_file.contains("png", Qt::CaseInsensitive)) && !QFileInfo(file_full_path).fileName().startsWith(".") )
        {

            QImageReader reader(file_full_path);

            QImage img = reader.read();

            if(img.isNull())
                continue;

            bool bool_face_status = facefinder_obj->check_is_image_have_face(file_full_path, fs_modules_details_obj.fs_record_no, target_struct_obj.source_count_name);

            if(bool_face_status)
            {
                QString command = QString("insert into files (filesystem_record) values(?)");
                QSqlQuery insert_query(in_memory_face_analysis_db);

                insert_query.prepare(command);
                insert_query.addBindValue(fs_modules_details_obj.fs_record_no);

                if(!insert_query.exec())
                {

                    recon_static_functions::app_debug(" query insert  --------FAILED------ ",Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
                    continue;
                }
            }

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

        }

        if(estimated_time.isEmpty() || estimated_time == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Face_Analysis, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Face_Analysis, estimated_time);

        if(records_count % 20== 0)
        {
            qint64 progress_val = ((records_count * 100) / total_records_num);

            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Face_Analysis, target_struct_obj.source_name, true, records_count, progress_val, false);
        }
    }


    in_memory_face_analysis_db.commit();
    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_face_analysis_db, Q_FUNC_INFO);
    in_memory_face_analysis_db.close();
    QSqlDatabase::removeDatabase(in_memory_face_analysis_db.connectionName());


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


    QStringList face_analysis_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString face_analysis_db_path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/face_analysis.sqlite");


        face_analysis_db_path_list << face_analysis_db_path_1;
    }

    emit signal_add_faces_to_case_tree(face_analysis_db_path_list);

    run_fs_modules_details_list.clear();

}

