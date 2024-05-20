#include "thread_hashes.h"

thread_hashes::thread_hashes(QObject *parent) : QObject(parent)
{
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_hashes::~thread_hashes()
{

}


void thread_hashes::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_hashes::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_hashes::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}


void thread_hashes::slot_hashset_extraction()
{ // from here hashes extraction starts, we support MD5 and SHA1 hashses and use QT's inbuild function like MD5_init and SHA1_init functions for hashes extraction
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Hashes);


    bool_cancel_extraction = false;
    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        mutext_for_fs_main_db->lock();
        recon_helper_standard_obj->pub_set_filesystem_modules_thread_run_status(QString(MACRO_JobType_Hashes), QString::number(1), Q_FUNC_INFO);
        mutext_for_fs_main_db->unlock();

        extract_hashset_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Hashes);

        //update_record_count_for_hashset_conf_index_db();

        if(db_hashset_indexes.isOpen())
            db_hashset_indexes.close();

        emit finished();
        return;
    }

    QString cmd_fs_db_update = QString("Update files set fs_module_hashset_run_status = '1' Where ");
    int bunch_count = 50;

    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
    {
        emit signal_add_all_available_hashes_to_case_tree();
    }
    else
    {

        for(int count = 0; count < list_target_source_info.size(); count++)
        {
            if(bool_cancel_extraction)
                break;

            struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);
            QString fs_hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/hashes.sqlite");


            if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
                continue;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            list_struct_fs_rec_and_path.clear();


            mutext_for_fs_main_db->lock();
            recon_helper_standard_obj->pub_set_filesystem_modules_thread_run_status(QString(MACRO_JobType_Hashes), QString::number(1), Q_FUNC_INFO);
            mutext_for_fs_main_db->unlock();

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Hashes, target_struct_obj.source_name, true, 0, 0, false);

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");


            if(!open_and_create_hashes_db(fs_hashes_db_path))
                continue;

            mutext_for_fs_main_db->lock();
            if(!open_fs_db(fs_db_path))
            {
                mutext_for_fs_main_db->unlock();
                in_memory_hashes_db.close();
                continue;
            }

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files") , 0, destination_db, Q_FUNC_INFO);

            qint64  total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                destination_db.close();
                mutext_for_fs_main_db->unlock();
                in_memory_hashes_db.close();

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
                command_fs = QString("select INT, id_path from files");
            }
            else
                command_fs = QString("select INT, filepath from files");

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

            in_memory_hashes_db.transaction();
            QSqlQuery insert_query(in_memory_hashes_db);

            qint64 records_count = 0;

            QStringList record_num_list;
            int fs_rec_db_updated_count = 0;

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

                ///do not process same record
                QString tmp_md5_hash_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select md5_hash from files Where filesystem_record = ?"), QStringList(fs_rec_filepath_obj.filesystem_record), 0, in_memory_hashes_db, Q_FUNC_INFO);
                tmp_md5_hash_str = tmp_md5_hash_str.trimmed();
                if(!tmp_md5_hash_str.isEmpty())
                    continue;


                QString file_full_path = target_struct_obj.fs_extraction_source_path + fs_rec_filepath_obj.filepath;

                //                if(!QFileInfo(file_full_path).isFile())
                //                    continue;

                bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
                if(!bool_is_file)
                    continue;


                /// get hash here
                struct_hashes hashes_obj = create_md5_and_sha1_hashes(file_full_path);
                QString file_hash = hashes_obj.md5_hash.trimmed();
                QString file_hash_sha1 = hashes_obj.sha1_hash.trimmed();


                if(file_hash.isEmpty())
                    continue;

                /// Save data for one record

                QString command = QString("insert into files (filesystem_record, md5_hash, sha1_hash) values(?,?,?)");
                insert_query.prepare(command);

                insert_query.addBindValue(fs_rec_filepath_obj.filesystem_record);
                insert_query.addBindValue(file_hash);
                insert_query.addBindValue(file_hash_sha1);
                if(!insert_query.exec())
                {
                    recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                // prepare_record_count_for_hashset_conf_index_db(file_hash);

                ///==================NOTE=======Start=====================================
                ///no need to update because hash can be run on same file mutiple times
                /// because user can add new hashset.
                //record_num_list << fs_rec_filepath_obj.filesystem_record;

                //                fs_rec_db_updated_count++;
                //                if(fs_rec_db_updated_count == bunch_count)
                //                {
                //                    mutext_for_fs_main_db->lock();


                //                    bool bool_list_processed = false;
                //                    QString tmp_cmd;
                //                    int pos = 0;
                //                    for(int count = (record_num_list.size() -1); count >= 0; count--)
                //                    {
                //                        if(bool_cancel_extraction)
                //                            break;

                //                        bool_list_processed = true;
                //                        QString record_no_str = record_num_list.takeAt(count);

                //                        if(pos == 0)
                //                            tmp_cmd = cmd_fs_db_update;

                //                        tmp_cmd.append("INT = ");
                //                        tmp_cmd.append(record_no_str);
                //                        tmp_cmd.append(" OR ");

                //                        pos++;

                //                        if(count == bunch_count || count == 0)
                //                        {
                //                            if(tmp_cmd.endsWith(" OR "))
                //                                tmp_cmd.chop(QString(" OR ").size());

                //                            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, fs_db_path, Q_FUNC_INFO);
                //                            tmp_cmd.clear();

                //                            pos = 0;
                //                        }
                //                    }

                //                    if(bool_list_processed)
                //                    {
                //                        fs_rec_db_updated_count = 0;
                //                        record_num_list.clear();
                //                    }
                //                    mutext_for_fs_main_db->unlock();
                //                }
                ///==================NOTE=======End=====================================

                if(estimated_time.isEmpty() || estimated_time == "00:00:00")
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Hashes, "Calculating...");
                else
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Hashes, estimated_time);

                if(records_count % 1000 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);
                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Hashes, target_struct_obj.source_name, true, records_count, progress_val, false);
                }

                //                if(records_count % 2000 == 0)
                //                {
                //                    intermediate_save_data_to_hashes_db(fs_hashes_db_path);
                //                }

            }

            // intermediate_save_data_to_hashes_db(fs_hashes_db_path);
            in_memory_hashes_db.commit();
            recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_hashes_db, Q_FUNC_INFO);
            in_memory_hashes_db.close();

            mutext_for_fs_main_db->lock();
            copy_data_from_hashes_db_to_fs_db(target_struct_obj.source_count_name);
            mutext_for_fs_main_db->unlock();

            ///==================NOTE=======Start=====================================
            ///no need to update because hash can be run on same file mutiple times
            /// because user can add new hashset.
            ///==================NOTE=======End=====================================

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Hashes, target_struct_obj.source_name, true, records_count, 100, false);

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Hashes, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);


            if(recon_case_runner_type != MACRO_RECON_CASE_RUNNER_TYPE_POST_LAUNCH)
                emit signal_add_matched_hashes_to_case_tree(fs_hashes_db_path, target_struct_obj.source_count_name);
        }

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_POST_LAUNCH)
            emit signal_add_all_available_hashes_to_case_tree();
    }
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Hashes);


    emit finished();
}

void thread_hashes::copy_data_from_hashes_db_to_fs_db(QString source_count_name)
{
    QString hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/hashes.sqlite");


    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/file_system.sqlite");


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

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashesDB")
            .arg(QDir::toNativeSeparators(hashes_db_path));
    if(!dest_query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug("query attach   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QString command_str = QString("Update main.files set ");

    QStringList fs_columns_list;
    fs_columns_list << "hash_md5" << "hash_sha1";

    QStringList hashes_columns_list;
    hashes_columns_list << "md5_hash" << "sha1_hash";

    for(int ii = 0; ii < fs_columns_list.size(); ii++)
    {
        command_str.append(QString(" " + fs_columns_list.at(ii) + " = (Select " + hashes_columns_list.at(ii) + " From hashesDB.files where main.files.INT = hashesDB.files.filesystem_record),"));
    }
    if(command_str.trimmed().endsWith(","))
        command_str.chop(1);

    dest_query.prepare(command_str);
    if(!dest_query.exec())
    {
        recon_static_functions::app_debug("query update   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + dest_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
    }

    QString command_detach = QString("DETACH DATABASE hashesDB");
    dest_query.exec(command_detach);
    destination_db.close();

}

void thread_hashes::intermediate_save_data_to_hashes_db(QString signature_database_path)
{
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(signature_database_path));

    QSqlQuery query_attach(in_memory_hashes_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_insert(in_memory_hashes_db);

    QString command_insert = QString("INSERT INTO filesystemDB.files SELECT * from files");

    query_insert.prepare(command_insert);
    if(!query_insert.exec())
    {
        recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_clear(in_memory_hashes_db);

    QString command_clear = QString("DELETE from files");

    query_clear.prepare(command_clear);
    if(!query_clear.exec())
    {
        recon_static_functions::app_debug(" query memory Delete  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_detach(in_memory_hashes_db);
    QString command_detach = QString("DETACH DATABASE filesystemDB");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }

}

void thread_hashes::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

bool thread_hashes::open_fs_db(QString fs_db_path)
{
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

void thread_hashes::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}


bool thread_hashes::open_and_create_hashes_db(QString hashes_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_hashes_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_hashes_db.setDatabaseName(hashes_db_path);
    if(!in_memory_hashes_db.open())
    {
        recon_static_functions::app_debug(" Hashes DB Open Open  -------FAILED------ " + in_memory_hashes_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_hashes_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    return true;


    //    QFileInfo info_db (hashes_db_path);
    //    if(info_db.exists())
    //    {
    //        if(info_db.size() <= 0 && recon_case_runner_type != QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    //            QFile::remove(hashes_db_path);
    //        else
    //        {
    //            QString connection_naam = Q_FUNC_INFO;
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            in_memory_hashes_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //            in_memory_hashes_db.setDatabaseName(hashes_db_path);
    //            if(!in_memory_hashes_db.open())
    //            {
    //                recon_static_functions::app_debug(" Hashes DB Open Open  -------FAILED------ " + in_memory_hashes_db.databaseName(), Q_FUNC_INFO);
    //                recon_static_functions::app_debug(in_memory_hashes_db.lastError().text() , Q_FUNC_INFO);
    //                return false;
    //            }

    //            return true;
    //        }
    //    }

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    in_memory_hashes_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    in_memory_hashes_db.setDatabaseName(":memory:");
    //    if(!in_memory_hashes_db.open())
    //    {
    //        recon_static_functions::app_debug("DB Open in memory -------FAILED------ " + in_memory_hashes_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(in_memory_hashes_db.lastError().text() , Q_FUNC_INFO);
    //        return false;
    //    }

    //    QString command_str = QString("create table files (INT INTEGER PRIMARY KEY, filesystem_record INTEGER, md5_hash varchar(200) , sha1_hash varchar(200))");

    //    QSqlQuery query_create_table(in_memory_hashes_db);
    //    query_create_table.prepare(command_str);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file " + in_memory_hashes_db.databaseName(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        in_memory_hashes_db.close();
    //        return false;
    //    }

    //    //    if(QFileInfo(hashes_db_path).exists())
    //    //        QFile::remove(hashes_db_path);

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashesDB")
    //            .arg(QDir::toNativeSeparators(hashes_db_path));

    //    QSqlQuery query_attach(in_memory_hashes_db);
    //    query_attach.prepare(db_attach_commmand);
    //    if(!query_attach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_hashes_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_save(in_memory_hashes_db);
    //    query_save.prepare(QString("CREATE TABLE hashesDB.files AS SELECT * from files"));
    //    if(!query_save.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Save  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
    //        return false;
    //    }

    //    QSqlQuery query_detach(in_memory_hashes_db);
    //    query_detach.prepare("DETACH DATABASE hashesDB");
    //    if(!query_detach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Detach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    //        return false;
    //    }

    //    return true;
}

thread_hashes::struct_hashes thread_hashes::create_md5_and_sha1_hashes(QString file_path)
{ // This is the function where we use inbuilt functions and variables for hashes extraction
    struct_hashes hashes_obj;


    unsigned char a_md5[MD5_DIGEST_LENGTH], a_sha1[SHA_DIGEST_LENGTH];
    int i, bytes;
    unsigned char data[1024];

    MD5_CTX md_context;
    MD5_Init(&md_context);

    SHA_CTX sha_context;
    SHA1_Init(&sha_context);

    ///open file and read data
    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {
        return hashes_obj;
    }

    qint64 file_total_size = target_file.size();
    qint64 data_read = 0;
    if(file_total_size <= 0)
        return hashes_obj;


    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        if(bool_cancel_extraction)
            break;


        MD5_Update(&md_context, data, bytes);
        SHA1_Update(&sha_context, data, bytes);

        data_read += bytes;
    }

    target_file.close();


    QByteArray md5_ar;
    MD5_Final(a_md5, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        if(bool_cancel_extraction)
            break;

        md5_ar.append(a_md5[i]);
    }

    QByteArray sha1_ar;
    SHA1_Final(a_sha1, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        if(bool_cancel_extraction)
            break;

        sha1_ar.append(a_sha1[i]);
    }


    hashes_obj.md5_hash  = QString::fromLocal8Bit(md5_ar.toHex());
    hashes_obj.sha1_hash = QString::fromLocal8Bit(sha1_ar.toHex());

    return hashes_obj;

}

void thread_hashes::extract_hashset_for_fs_run_module()
{ // right click action hashes extraction features which also use qt inbuilt MD5_inti and SHA1_init functions for hashes extraction
    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Hashes, QString("Acquiring Files List..."), true, 0, 0, false);

    struct struct_fs_db_update
    {
        QString record_no;
        QString fs_db_path;
    };
    QList <struct_fs_db_update> fs_db_update_struct_list;


    qint64 total_records_num = run_fs_modules_details_list.size();
    qint64 records_count = 0;

    QString cmd_fs_db_update = QString("Update files set fs_module_hashset_run_status = '1' Where ");

    QStringList processed_source_count_list;
    QStringList hashes_db_path_list;

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


        ///do not process same record
        QString tmp_md5_hash_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select md5_hash from files Where filesystem_record = ?"), QStringList(fs_modules_details_obj.fs_record_no), 0, in_memory_hashes_db, Q_FUNC_INFO);
        tmp_md5_hash_str = tmp_md5_hash_str.trimmed();
        if(!tmp_md5_hash_str.isEmpty())
            continue;


        struct_GLOBAL_witness_info_source target_struct_obj   = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(fs_modules_details_obj.source_count_name);

        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;

        QString fs_hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/hashes.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");

        if(in_memory_hashes_db.isOpen() && in_memory_hashes_db.databaseName() == fs_hashes_db_path)
        {
            ///db for same source already open no need to open again
        }
        else
        {
            processed_source_count_list << fs_modules_details_obj.source_count_name;
            hashes_db_path_list << fs_hashes_db_path;

            if(in_memory_hashes_db.isOpen())
            {
                in_memory_hashes_db.commit();
                recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_hashes_db, Q_FUNC_INFO);
                in_memory_hashes_db.close();

                QSqlDatabase::removeDatabase(in_memory_hashes_db.connectionName());
            }
            if(!open_and_create_hashes_db(fs_hashes_db_path))
                continue;

            in_memory_hashes_db.transaction();
        }

        QString file_full_path = fs_modules_details_obj.complete_file_path;

        records_count++;

        bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(file_full_path, Q_FUNC_INFO);
        if(!bool_is_file)
            continue;

        QSqlQuery insert_query(in_memory_hashes_db);

        /// get hash here
        struct_hashes hashes_obj = create_md5_and_sha1_hashes(file_full_path);

        QString file_hash = hashes_obj.md5_hash.trimmed();
        QString file_hash_sha1 = hashes_obj.sha1_hash.trimmed();

        if(file_hash.isEmpty())
            continue;


        /// Save data for one record

        QString command = QString("insert into files (filesystem_record, md5_hash, sha1_hash) values(?,?,?)");
        insert_query.prepare(command);

        insert_query.addBindValue(fs_modules_details_obj.fs_record_no);
        insert_query.addBindValue(file_hash);
        insert_query.addBindValue(file_hash_sha1);
        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
            continue;
        }


        ///==================NOTE=======Start=====================================
        ///no need to update because hash can be run on same file mutiple times
        /// because user can add new hashset.
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
        ///==================NOTE=======End=====================================

        if(estimated_time.isEmpty() || estimated_time == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Hashes, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Hashes, estimated_time);

        if(records_count % 10 == 0)
        {
            qint64 progress_val = ((records_count * 100) / total_records_num);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Hashes, fs_modules_details_obj.examiner_selected_file_or_dir_name, true, records_count, progress_val, false);
        }

    }
    hashes_db_path_list.removeDuplicates();
    processed_source_count_list.removeDuplicates();

    in_memory_hashes_db.commit();
    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_hashes_db, Q_FUNC_INFO);
    in_memory_hashes_db.close();
    QSqlDatabase::removeDatabase(in_memory_hashes_db.connectionName());

    mutext_for_fs_main_db->lock();
    for(int count = 0; count < processed_source_count_list.size(); count++)
        copy_data_from_hashes_db_to_fs_db(processed_source_count_list.at(count));
    mutext_for_fs_main_db->unlock();

    ///==================NOTE=======Start=====================================
    ///no need to update because hash can be run on same file mutiple times
    /// because user can add new hashset.
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
    ///==================NOTE=======End=====================================

    emit signal_add_all_available_hashes_to_case_tree();

    run_fs_modules_details_list.clear();
}

