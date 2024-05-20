#include "thread_signature.h"

thread_signature::thread_signature(QObject *parent) : QObject(parent)
{
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_signature::~thread_signature()
{
}


void thread_signature::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_signature::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}


void thread_signature::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}


void thread_signature::slot_extract_signature()
{ // Here our Signature analysis feature runs and extract the signatures, we get the signture related info from case configuration db which we shows on post launcher
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    bool_cancel_extraction = false;

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Signature_Analysis);

    bunch_count = 50;
    fs_rec_db_updated_count = 0;
    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        open_config_db();
        prepare_signature_structure_list();

        extract_signature_analysis_for_fs_run_module();

        case_configuration_db.close();

        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Signature_Analysis);
        emit finished();
        return;
    }

    if(recon_case_runner_type != MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE && !job_selected_source_count_name_list.isEmpty())
    {
        open_config_db();
        prepare_signature_structure_list();
    }

    QStringList signature_db_path_list;

    for(int count = 0; count < list_target_source_info.size(); count++)
    {

        if(bool_cancel_extraction)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);
        QString signature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/signature.sqlite");

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            signature_db_path_list << signature_db_path;
        }
        else
        {

            signature_db_path_list << signature_db_path;

            if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
                continue;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;


            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Signature_Analysis, target_struct_obj.source_name, true, 0, 0, false);

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");


            if(!open_and_create_signature_db(signature_db_path))
                continue;


            extract_signatures(fs_db_path, signature_db_path , target_struct_obj, signature_db_path_list);

            //in_memory_signature_data_db.close();
        }
    }

    case_configuration_db.close();

    emit signal_add_all_signatures_to_case_tree(signature_db_path_list);

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Signature_Analysis);
    emit finished();
}

void thread_signature::extract_signatures(QString fs_db_path, QString signature_db_path, struct_GLOBAL_witness_info_source target_struct_obj, QStringList sign_db_path_list)
{ // extraction signature related code
    mutex_for_fs_main_db->lock();
    if(!open_fs_db(fs_db_path))
    {
        in_memory_signature_data_db.close();
        mutex_for_fs_main_db->unlock();
        return;
    }

    list_struct_fs_rec_and_path.clear();


    fs_rec_db_updated_count = 0;
    QString cmd_fs_db_update = QString("Update files set fs_module_signature_analysis_run_status = '1' Where ");

    QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files Where fs_module_signature_analysis_run_status='0'") , 0, destination_db, Q_FUNC_INFO);
    qint64  total_records_num = total_records_str.toLongLong();
    if(total_records_num <= 0)
    {
        in_memory_signature_data_db.close();
        destination_db.close();
        mutex_for_fs_main_db->unlock();
        return;
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
        command_fs = QString("select INT, id_path from files Where fs_module_signature_analysis_run_status='0'");
    }
    else
        command_fs = QString("select INT, filepath from files Where fs_module_signature_analysis_run_status='0'");

    QSqlQuery query(destination_db);

    if(!query.exec(command_fs))
    {
        destination_db.close();
        in_memory_signature_data_db.close();

        mutex_for_fs_main_db->unlock();
        return;
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
    mutex_for_fs_main_db->unlock();

    qint64 records_count = 0;
    in_memory_signature_data_db.transaction();
    QSqlQuery insert_query(in_memory_signature_data_db);

    for(int num = 0; num < list_struct_fs_rec_and_path.size(); num++)
    {
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

        if(bool_cancel_extraction)
            break;

        struct_filesystem_record_and_filepath file_rec_filepath_obj;
        file_rec_filepath_obj = list_struct_fs_rec_and_path.at(num);

        QString extnd_path = target_struct_obj.fs_extraction_source_path + file_rec_filepath_obj.filepath;

        if(!QFileInfo(extnd_path).isFile())
        {
            continue;
        }

        struct_signature_data signature_data_obj;
        signature_data_obj.filesystem_record.clear();
        signature_data_obj.signature_value.clear();
        signature_data_obj.signature_name.clear();


        //        if(/*signature_check_status*/)
        check_file_signature(extnd_path , signature_data_obj);

        if(signature_data_obj.signature_name.trimmed().isEmpty() && signature_data_obj.signature_value.trimmed().isEmpty())
            continue;

        QString command = QString("insert into files (filesystem_record,  signature_name, signature_value) values(?,?,?)");

        insert_query.prepare(command);

        insert_query.addBindValue(file_rec_filepath_obj.filesystem_record);
        insert_query.addBindValue(signature_data_obj.signature_name);
        insert_query.addBindValue(signature_data_obj.signature_value);

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" query insert  --------FAILED------ ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        record_num_list << file_rec_filepath_obj.filesystem_record;
        fs_rec_db_updated_count++;

        if(fs_rec_db_updated_count == bunch_count)
        {
            mutex_for_fs_main_db->lock();

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
            mutex_for_fs_main_db->unlock();
        }

        if(estimated_time.isEmpty() || estimated_time == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Signature_Analysis, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Signature_Analysis, estimated_time);

        if(records_count % 1000 == 0)
        {
            qint64 progress_val = ((records_count * 100) / total_records_num);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Signature_Analysis, target_struct_obj.source_name, true, records_count, progress_val, false);
        }

        //        if(records_count % 2000 == 0)
        //        {
        //            intermediate_save_data_to_signature_db(signature_db_path);
        //        }

    }

    // intermediate_save_data_to_signature_db(signature_db_path);
    in_memory_signature_data_db.commit();
    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_signature_data_db, Q_FUNC_INFO);
    in_memory_signature_data_db.close();

    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Signature_Analysis, target_struct_obj.source_name, true, records_count, 100, false);
    if(!record_num_list.isEmpty())
    {
        mutex_for_fs_main_db->lock();

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

        fs_rec_db_updated_count = 0;
        record_num_list.clear();
        mutex_for_fs_main_db->unlock();
    }

    //emit signal_add_all_signatures_to_case_tree(sign_db_path_list);

    if(!bool_cancel_extraction)
        emit signal_completion_status(MACRO_JobType_Signature_Analysis, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

}

void thread_signature::intermediate_save_data_to_signature_db(QString signature_database_path)
{
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(signature_database_path));

    QSqlQuery query_attach(in_memory_signature_data_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_insert(in_memory_signature_data_db);

    QString command_insert = QString("INSERT INTO filesystemDB.files SELECT * from files");
    query_insert.prepare(command_insert);
    if(!query_insert.exec())
    {
        recon_static_functions::app_debug(" query memory insert  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_clear(in_memory_signature_data_db);

    QString command_clear = QString("DELETE from files");
    query_clear.prepare(command_clear);
    if(!query_clear.exec())
    {
        recon_static_functions::app_debug(" query memory Delete  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }
    QSqlQuery query_detach(in_memory_signature_data_db);
    QString command_detach = QString("DETACH DATABASE filesystemDB");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory Detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }

}

bool thread_signature::open_and_create_signature_db(QString signature_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_signature_data_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_signature_data_db.setDatabaseName(signature_db_path);
    if(!in_memory_signature_data_db.open())
    {
        recon_static_functions::app_debug(" signature_data_db DB Open  -------FAILED------ " + in_memory_signature_data_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_signature_data_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    return true;


    //    QFileInfo info_db (signature_db_path);
    //    if(info_db.exists())
    //    {
    //        if(info_db.size() <= 0 && recon_case_runner_type != QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    //            QFile::remove(signature_db_path);
    //        else
    //        {
    //            QString connection_naam = Q_FUNC_INFO;
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            in_memory_signature_data_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //            in_memory_signature_data_db.setDatabaseName(signature_db_path);
    //            if(!in_memory_signature_data_db.open())
    //            {
    //                recon_static_functions::app_debug(" signature_data_db DB Open  -------FAILED------ " + in_memory_signature_data_db.databaseName(), Q_FUNC_INFO);
    //                recon_static_functions::app_debug(in_memory_signature_data_db.lastError().text() , Q_FUNC_INFO);
    //                return false;
    //            }

    //            return true;
    //        }
    //    }

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);

    //    in_memory_signature_data_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    in_memory_signature_data_db.setDatabaseName(":memory:");
    //    if(!in_memory_signature_data_db.open())
    //    {
    //        recon_static_functions::app_debug(" signature_data_db Open in memory -------FAILED------ " + in_memory_signature_data_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(in_memory_signature_data_db.lastError().text() , Q_FUNC_INFO);
    //        return false;
    //    }

    //    QString command_str = QString("create table files (bookmark INTEGER, filesystem_record INTEGER, signature_name varchar(200), signature_value varchar(200))");

    //    QSqlQuery query_create_table(in_memory_signature_data_db);
    //    query_create_table.prepare(command_str);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file - ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        in_memory_signature_data_db.close();
    //        return false;
    //    }

    //    //    if(QFileInfo(signature_db_path).exists())
    //    //        QFile::remove(signature_db_path);

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
    //            .arg(QDir::toNativeSeparators(signature_db_path));

    //    QSqlQuery query_attach(in_memory_signature_data_db);
    //    query_attach.prepare(db_attach_commmand);
    //    if(!query_attach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_signature_data_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_save(in_memory_signature_data_db);
    //    query_save.prepare(QString("CREATE TABLE filesystemDB.files AS SELECT * from files"));
    //    if(!query_save.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Save  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
    //        in_memory_signature_data_db.close();
    //        return false;
    //    }

    //    QSqlQuery query_detach(in_memory_signature_data_db);
    //    query_detach.prepare("DETACH DATABASE filesystemDB");
    //    if(!query_detach.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory Detach  --------FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    //        in_memory_signature_data_db.close();
    //        return false;
    //    }

    //    return true;

}


bool thread_signature::open_fs_db(QString fs_db_path)
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

void thread_signature::open_config_db()
{
    QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    case_configuration_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    case_configuration_db.setDatabaseName(case_db_path);
    if(!case_configuration_db.open())
    {
        recon_static_functions::app_debug(" case_configuration_db Open -------FAILED------ " + case_configuration_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(case_configuration_db.lastError().text() , Q_FUNC_INFO);
    }

}

void thread_signature::pub_set_bool_cancel_extraction(struct_global_task_type_and_title t1)
{
    if(t1.type == QString(MACRO_JobType_Signature_Analysis))
        bool_cancel_extraction = true;
}

void thread_signature::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_signature::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutex_for_fs_main_db = mutex;
}

void thread_signature::prepare_signature_structure_list()
{ // prepare signature qlist where this case configuration database plays role
    all_signatures_list.clear();
    QString command_str = QString("Select header from tbl_signature_list Where signature_status = '1'");
    all_signatures_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_str , 0 , case_configuration_db, Q_FUNC_INFO);

    for(int num = 0; num < all_signatures_list.size(); num++)
    {
        if(bool_cancel_extraction)
            break;

        if(all_signatures_list.at(num).trimmed().isEmpty())
            continue;

        signature_match_data signature_match_obj;

        command_str = QString("Select offset from tbl_signature_list Where header = ?");
        QString offset_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_str , QStringList(all_signatures_list.at(num)), 0 , case_configuration_db, Q_FUNC_INFO);

        if(!offset_str.trimmed().isEmpty())
        {
            if(offset_str.startsWith("0x"))
                offset_str = offset_str.mid(2, offset_str.size());

            bool ok = false;
            uint offset_decimal = offset_str.toUInt(&ok,16);

            offset_str = offset_str.number(offset_decimal);
        }


        signature_match_obj.signature_value   = all_signatures_list.at(num);
        signature_match_obj.signature_offset = offset_str;

        command_str = QString("Select label from tbl_signature_list Where header =  ?");
        signature_match_obj.signature_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_str , QStringList(all_signatures_list.at(num)), 0 , case_configuration_db, Q_FUNC_INFO);

        signature_match_data_list.append(signature_match_obj);

    }

}

void thread_signature::check_file_signature(QString filepath, struct_signature_data &signature_data_obj)
{
    if(filepath.isEmpty())
    {
        return;
    }

    QFile myfile(filepath);
    if(!myfile.exists())
        return;

    if (!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(" -File Open ------FAILED----- : "  + filepath, Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error---- " + myfile.errorString() , Q_FUNC_INFO);
        return;
    }


    for(int count = 0; count < signature_match_data_list.size(); count++)
    {
        signature_match_data signature_match_obj = signature_match_data_list.at(count);

        QString signature_str = signature_match_obj.signature_value;

        QString offset_str = signature_match_obj.signature_offset;

        if(signature_match_obj.signature_value.isEmpty())
            continue;

        myfile.seek(0);

        qint64 bytes_to_read = signature_str.size();

        bool bool_is_signature_hex = false;

        if(signature_str.startsWith(QString("0x")))
        {
            bool_is_signature_hex = true;
            signature_str = signature_str.mid(2, signature_str.size() - 2);
            bytes_to_read = signature_str.size() / 2;
        }



        if(bytes_to_read <= 0)
            continue;


        offset_str = offset_str.trimmed();

        if(!offset_str.isEmpty())
        {
            bool ok;
            myfile.seek(offset_str.toInt(&ok, 10));
        }


        ///
        ///  Do Not use trimm here...(because there could be null bytes)
        ///


        QByteArray data_read = myfile.read(bytes_to_read);

        bool m_status = false;
        if(bool_is_signature_hex)
        {
            if(signature_str.toLower() == QString::fromLocal8Bit(data_read.toHex()).toLower())
            {
                m_status = true;
            }
        }
        else
        {
            if(signature_str.toLower() == QString::fromLocal8Bit(data_read).toLower())
            {
                m_status = true;
            }
        }

        if(m_status)
        {
            signature_data_obj.signature_value = signature_match_obj.signature_value;
            signature_data_obj.signature_name = signature_match_obj.signature_name;
            break;

        }

    }
    myfile.close();
    return;
}


void thread_signature::extract_signature_analysis_for_fs_run_module()
{
    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Apple_Metadata, QString("Acquiring Files List..."), true, 0, 0, false);

    struct struct_fs_db_update
    {
        QString record_no;
        QString fs_db_path;
    };
    QList <struct_fs_db_update> fs_db_update_struct_list;

    fs_rec_db_updated_count = 0;

    qint64 total_records_num = run_fs_modules_details_list.size();
    qint64 records_count = 0;

    QString cmd_fs_db_update = QString("Update files set fs_module_signature_analysis_run_status = '1' Where ");

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

            qint64 required_time = new_ramining_file_count/avg_image_per_sec;

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


        QString signature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/signature.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");


        if(in_memory_signature_data_db.isOpen() && in_memory_signature_data_db.databaseName() == signature_db_path)
        {
            ///db for same source already open no need to open again
        }
        else
        {

            if(in_memory_signature_data_db.isOpen())
            {
                in_memory_signature_data_db.commit();
                recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_signature_data_db, Q_FUNC_INFO);
                in_memory_signature_data_db.close();

                QSqlDatabase::removeDatabase(in_memory_signature_data_db.connectionName());
            }
            if(!open_and_create_signature_db(signature_db_path))
                continue;

            in_memory_signature_data_db.transaction();

        }
        QString file_full_path = fs_modules_details_obj.complete_file_path;
        records_count++;

        if(!QFileInfo(file_full_path).isFile())
        {
            continue;
        }

        struct_signature_data signature_data_obj;
        signature_data_obj.filesystem_record.clear();
        signature_data_obj.signature_value.clear();
        signature_data_obj.signature_name.clear();

        check_file_signature(file_full_path , signature_data_obj);

        if(signature_data_obj.signature_name.trimmed().isEmpty() && signature_data_obj.signature_value.trimmed().isEmpty())
            continue;


        QSqlQuery insert_query(in_memory_signature_data_db);

        QString command = QString("insert into files (filesystem_record,  signature_name, signature_value) values(?,?,?)");

        insert_query.prepare(command);

        insert_query.addBindValue(fs_modules_details_obj.fs_record_no);
        insert_query.addBindValue(signature_data_obj.signature_name);
        insert_query.addBindValue(signature_data_obj.signature_value);

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" query insert  --------FAILED------ ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        struct_fs_db_update fs_db_update_obj;
        fs_db_update_obj.record_no = fs_modules_details_obj.fs_record_no;
        fs_db_update_obj.fs_db_path = fs_db_path;

        fs_db_update_struct_list.append(fs_db_update_obj);

        fs_rec_db_updated_count++;
        if(fs_rec_db_updated_count == bunch_count)
        {
            mutex_for_fs_main_db->lock();

            bool bool_list_processed = false;

            QString tmp_cmd;
            int pos = 0;
            for(int count = (fs_db_update_struct_list.size() -1); count >= 0; count--)
            {
                struct_fs_db_update fs_db_obj = fs_db_update_struct_list.takeAt(count);

                bool_list_processed = true;

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
                fs_rec_db_updated_count = 0;
                fs_db_update_struct_list.clear();
            }
            mutex_for_fs_main_db->unlock();
        }

        // intermediate_save_data_to_signature_db(signature_db_path);

        if(estimated_time.isEmpty() || estimated_time == "00:00:00")
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Signature_Analysis, "Calculating...");
        else
            emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Signature_Analysis, estimated_time);

        if(records_count % 1000 == 0)
        {
            qint64 progress_val = ((records_count * 100) / total_records_num);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Signature_Analysis, fs_modules_details_obj.examiner_selected_file_or_dir_name, true, records_count, progress_val, false);
        }

    }


    in_memory_signature_data_db.commit();
    recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", in_memory_signature_data_db, Q_FUNC_INFO);
    in_memory_signature_data_db.close();
    QSqlDatabase::removeDatabase(in_memory_signature_data_db.connectionName());

    if(!fs_db_update_struct_list.isEmpty())
    {
        mutex_for_fs_main_db->lock();
        QString tmp_cmd;
        int pos = 0;
        for(int count = (fs_db_update_struct_list.size() -1); count >= 0; count--)
        {
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
        fs_rec_db_updated_count = 0;
        mutex_for_fs_main_db->unlock();
    }

    QStringList signature_analysis_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString sign_db_path_2 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/signature.sqlite");

        signature_analysis_db_path_list << sign_db_path_2;
    }


    emit signal_add_all_signatures_to_case_tree(signature_analysis_db_path_list);

    run_fs_modules_details_list.clear();

}
