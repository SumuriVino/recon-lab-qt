#include "thread_mail_emlx.h"

thread_mail_emlx::thread_mail_emlx(QObject *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);
}

thread_mail_emlx::~thread_mail_emlx()
{

}

void thread_mail_emlx::slot_extract_emlx_files_data()
{
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QString featr_emails_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Email_Parser_Emlx_Mail);

    QStringList email_parser_db_path_list;

    QString email_parser_db_path = featr_emails_parent_path + QString("/email_data.sqlite");
    QString tmp_email_parser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString();
    QString tmp_mail_emlx_db_path = tmp_email_parser_dir_path + "/tmp_mail_emlx.sqlite";

    for(int count = 0; count < list_target_source_info.size(); count++)
    {

        QString account_id_str;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            email_parser_db_path_list << email_parser_db_path;
        }
        else
        {
            email_parser_db_path_list << email_parser_db_path;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Emlx_Mail,target_struct_obj.source_name,false,0,0,false);

            mutex_for_fs_main_db->lock();
            if(!open_fs_db(filesystem_db_path))
            {
                email_parser_db.close();
                mutex_for_fs_main_db->unlock();
                continue;
            }
            // ------Copy email_data.sqlite------Start---
            // copy email_data.sqlite into tmp dir named as tmp_mail_emlx.sqlite
            //  and delete the complete data of it to get the column names

            QStringList cp_args;
            cp_args << email_parser_db_path << tmp_mail_emlx_db_path;

            QProcess proc_cp_email_parser_db;

            proc_cp_email_parser_db.start("cp", cp_args);

            proc_cp_email_parser_db.waitForFinished(-1);

            proc_cp_email_parser_db.close();

            QString delete_cmd = "DELETE FROM emlx_data";
            recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd,tmp_mail_emlx_db_path,Q_FUNC_INFO);

            // ------Copy email_data.sqlite------End---

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("SELECT count(*) FROM files where extension = 'emlx'") , 0, file_system_db, Q_FUNC_INFO);
            qint64 total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                file_system_db.close();
                email_parser_db.close();
                mutex_for_fs_main_db->unlock();
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
                command_fs = QString("select INT, id_path, filepath, extension, is_dir from files Where extension = 'emlx'");
            }
            else
            {
                command_fs = QString("select INT, filepath, extension, is_dir from files Where extension = 'emlx'");
            }

            QString emlx_insert_cmd = "insert into emlx_data  (bookmark,email_type, emlx_subject,emlx_to,emlx_from,emlx_date,emlx_cc,emlx_msg_body,email_attachment,"
                                      "account_id,mbox,source_file,os_scheme_display,source_count_name,message_text,export_file_paths) values(0,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) ";

            QSqlQuery select_query(file_system_db);

            if(!select_query.exec(command_fs))
            {
                file_system_db.close();
                mutex_for_fs_main_db->unlock();
                continue;
            }

            qlist_struct_fs_rec_and_path.clear();

            while(select_query.next())
            {

                QString filepath = select_query.value(1).toString();

                if(filepath.trimmed().isEmpty())
                    continue;

                if(recon_helper_standard_obj->bool_is_permission_restricted_file(filepath, Q_FUNC_INFO))
                    continue;

                struct_filesystem_record_and_filepath m1;
                m1.filesystem_record = select_query.value("INT").toString();
                m1.filepath = filepath;
                m1.extension = select_query.value("extension").toString();
                m1.is_dir = select_query.value("is_dir").toString();

                if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                        || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                {
                    m1.ios_filepath = select_query.value("filepath").toString();
                }

                qlist_struct_fs_rec_and_path << m1;

            }

            file_system_db.close();
            mutex_for_fs_main_db->unlock();

            qint64 records_count = 0;

            QStringList record_num_list;
            int fs_rec_db_updated_count = 0;

            if(!open_and_create_email_parser_db(tmp_mail_emlx_db_path))
            {
                continue;
            }

            QSqlQuery insert_query(email_parser_db);

            for(int num = 0; num < qlist_struct_fs_rec_and_path.size(); num++)
            {

                qint64 current_time_epoch = QDateTime::currentSecsSinceEpoch();
                remaining_file_count_for_req_time_qint64--;
                processed_file_count_qint64++;

                if(current_time_epoch - start_time_epoch >= 3)
                {
                    double avg_image_per_sec = processed_file_count_qint64 / (current_time_epoch - start_time_epoch);
                    qint64 new_remaining_file_count = remaining_file_count_for_req_time_qint64;
                    double required_time = new_remaining_file_count / avg_image_per_sec;

                    estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");
                    start_time_epoch = current_time_epoch;
                    processed_file_count_qint64 = 0;
                }


                records_count++;

                struct_filesystem_record_and_filepath fs_rec_filepath_obj = qlist_struct_fs_rec_and_path.at(num);

                QString filepath = fs_rec_filepath_obj.filepath;

                if(fs_rec_filepath_obj.extension != "emlx")
                    continue;

                emlx_filepath = recon_static_functions::get_complete_file_path(target_struct_obj.virtual_source_path,filepath,Q_FUNC_INFO);
                QString mbox_str;

                if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                        || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                {
                    QString m_emlx_filepath = fs_rec_filepath_obj.ios_filepath;
                    QStringList emlx_path_split_list = m_emlx_filepath.split("/");

                    if(!emlx_path_split_list.isEmpty())
                    {
                        int root_index = -1;
                        if(emlx_path_split_list.contains("V2"))
                            root_index = emlx_path_split_list.indexOf("V2");
                        else if(emlx_path_split_list.contains("V3"))
                            root_index = emlx_path_split_list.indexOf("V3");
                        else if(emlx_path_split_list.contains("V4"))
                            root_index = emlx_path_split_list.indexOf("V4");
                        else if(emlx_path_split_list.contains("V5"))
                            root_index = emlx_path_split_list.indexOf("V5");
                        else if(emlx_path_split_list.contains("V6"))
                            root_index = emlx_path_split_list.indexOf("V6");
                        else if(emlx_path_split_list.contains("V7"))
                            root_index = emlx_path_split_list.indexOf("V7");
                        else if(emlx_path_split_list.contains("V8"))
                            root_index = emlx_path_split_list.indexOf("V8");
                        else if(emlx_path_split_list.contains("V9"))
                            root_index = emlx_path_split_list.indexOf("V9");
                        else if(emlx_path_split_list.contains("V10"))
                            root_index = emlx_path_split_list.indexOf("V10");
                        else
                            root_index = -1;

                        ///get account id

                        if(root_index > 0 && emlx_path_split_list.size() > (root_index + 1))
                        {
                            QString account_id_parent_dir_name = emlx_path_split_list.at(root_index + 1);
                            if(m_emlx_filepath.contains(account_id_parent_dir_name))
                            {
                                QStringList account_id_parent_path_list = m_emlx_filepath.split(account_id_parent_dir_name);
                                QString account_folder_path = account_id_parent_path_list.at(0);
                                account_folder_path += account_id_parent_dir_name;

                                if(map_key_account_folder_path_value_account_id.contains(account_folder_path))
                                {
                                    account_id_str = map_key_account_folder_path_value_account_id.value(account_folder_path);
                                }
                                else
                                {
                                    account_id_str = get_account_id_str(account_folder_path);
                                    map_key_account_folder_path_value_account_id.insert(account_folder_path, account_id_str);
                                }
                            }
                        }

                        ///get mboxes
                        QStringList mbox_index_list;

                        mbox_index_list = get_index_of_mbox_from_path(emlx_path_split_list);

                        for(int count = 0; count < mbox_index_list.size(); count++)
                        {
                            int mbox_index = mbox_index_list.at(count).toInt();
                            mbox_str.append(emlx_path_split_list.at(mbox_index));
                            mbox_str.append(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple));
                        }
                    }
                }
                else
                {
                    QString m_emlx_filepath = emlx_filepath;
                    QStringList emlx_path_split_list = m_emlx_filepath.split("/");

                    if(!emlx_path_split_list.isEmpty())
                    {
                        int root_index = -1;
                        if(emlx_path_split_list.contains("V2"))
                            root_index = emlx_path_split_list.indexOf("V2");
                        else if(emlx_path_split_list.contains("V3"))
                            root_index = emlx_path_split_list.indexOf("V3");
                        else if(emlx_path_split_list.contains("V4"))
                            root_index = emlx_path_split_list.indexOf("V4");
                        else if(emlx_path_split_list.contains("V5"))
                            root_index = emlx_path_split_list.indexOf("V5");
                        else if(emlx_path_split_list.contains("V6"))
                            root_index = emlx_path_split_list.indexOf("V6");
                        else if(emlx_path_split_list.contains("V7"))
                            root_index = emlx_path_split_list.indexOf("V7");
                        else if(emlx_path_split_list.contains("V8"))
                            root_index = emlx_path_split_list.indexOf("V8");
                        else if(emlx_path_split_list.contains("V9"))
                            root_index = emlx_path_split_list.indexOf("V9");
                        else if(emlx_path_split_list.contains("V10"))
                            root_index = emlx_path_split_list.indexOf("V10");
                        else
                            root_index = -1;

                        ///get account id
                        if(root_index > 0 && emlx_path_split_list.size() > (root_index + 1))
                        {
                            QString account_id_parent_dir_name = emlx_path_split_list.at(root_index + 1);
                            if(m_emlx_filepath.contains(account_id_parent_dir_name))
                            {
                                QStringList account_id_parent_path_list = m_emlx_filepath.split(account_id_parent_dir_name);
                                QString account_folder_path = account_id_parent_path_list.at(0);
                                account_folder_path += account_id_parent_dir_name;

                                if(map_key_account_folder_path_value_account_id.contains(account_folder_path))
                                {
                                    account_id_str = map_key_account_folder_path_value_account_id.value(account_folder_path);
                                }
                                else
                                {
                                    account_id_str = get_account_id_str(account_folder_path);
                                    map_key_account_folder_path_value_account_id.insert(account_folder_path, account_id_str);
                                }
                            }
                        }

                        ///get mboxes
                        QStringList mbox_index_list;

                        mbox_index_list = get_index_of_mbox_from_path(emlx_path_split_list);

                        for(int count = 0; count < mbox_index_list.size(); count++)
                        {
                            int mbox_index = mbox_index_list.at(count).toInt();
                            mbox_str.append(emlx_path_split_list.at(mbox_index));
                            mbox_str.append(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple));
                        }
                    }
                }

                if(account_id_str.isEmpty())
                {
                    account_id_str = "Orphan";
                }

                if(mbox_str.endsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)))
                    mbox_str.chop(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple).size());

                if(mbox_str.isEmpty())
                {
                    QFileInfo info(emlx_filepath);
                    mbox_str = info.absoluteDir().dirName();
                }
                emlx_account_id_str = account_id_str.trimmed();
                emlx_mailbox_str = mbox_str.trimmed();
                emlx_filepath = emlx_filepath.trimmed();
                emlx_start_parsing(emlx_filepath);

                emlx_attachment_list.removeAll("");

                emlx_subject_str = emlx_subject_str.trimmed();
                emlx_to_str      = emlx_to_str.trimmed();
                emlx_from_str    = emlx_from_str.trimmed();
                emlx_date_str    = emlx_date_str.trimmed();
                emlx_message_str = emlx_message_str.trimmed();
                emlx_account_id_str = emlx_account_id_str.trimmed();
                emlx_mailbox_str    = emlx_mailbox_str.trimmed();

                insert_query.prepare(emlx_insert_cmd);
                insert_query.addBindValue("Emlx");

                insert_query.addBindValue(emlx_subject_str);
                insert_query.addBindValue(emlx_to_str);
                insert_query.addBindValue(emlx_from_str);
                insert_query.addBindValue(emlx_date_str);
                insert_query.addBindValue(emlx_cc_str);
                insert_query.addBindValue(emlx_message_str);
                insert_query.addBindValue(emlx_attachment_list.join(","));

                insert_query.addBindValue(emlx_account_id_str);
                insert_query.addBindValue(emlx_mailbox_str);

                QString relative_source_file = emlx_filepath;
                if(relative_source_file.startsWith(target_struct_obj.virtual_source_path))
                {
                    relative_source_file.remove(0,target_struct_obj.virtual_source_path.size());
                    if(!relative_source_file.startsWith("/"))
                        relative_source_file.prepend("/");
                }

                relative_source_file    = relative_source_file.trimmed();
                elmx_body_msg_without_text_formatting    = elmx_body_msg_without_text_formatting.trimmed();

                insert_query.addBindValue(relative_source_file);
                insert_query.addBindValue(target_struct_obj.os_scheme_display);
                insert_query.addBindValue(target_struct_obj.source_count_name);
                insert_query.addBindValue(elmx_body_msg_without_text_formatting);


                QString export_file_path ;
                if(emlx_attachment_list.size() != 0)
                {
                    for(int i = 0 ;i < emlx_attachment_list.size() ; i++)
                    {
                        export_file_path.append("/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)+ emlx_attachment_list.at(i));
                        export_file_path.append(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple));
                    }

                    if(export_file_path.endsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)))
                        export_file_path.chop(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple).size());

                    export_file_path = export_file_path.trimmed();

                    insert_query.addBindValue(export_file_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple) + relative_source_file);

                }
                else
                {
                    insert_query.addBindValue(relative_source_file);
                }

                if(!insert_query.exec())
                {
                    recon_static_functions::app_debug("Query insertt ----FAILED---" + insert_query.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error:" + insert_query.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                fs_rec_db_updated_count++;
                record_num_list << fs_rec_filepath_obj.filesystem_record;

                emlx_subject_str.clear();
                emlx_to_str.clear();
                emlx_from_str.clear();
                emlx_date_str.clear();
                emlx_cc_str.clear();
                emlx_message_str.clear();
                emlx_attachment_list.clear();
                elmx_body_msg_without_text_formatting.clear();


                if(estimated_time.isEmpty() || estimated_time == "00:00:00")
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Email_Parser_Emlx_Mail, "Calculating...");
                else
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Email_Parser_Emlx_Mail, estimated_time);


                if(records_count % 100 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);

                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Emlx_Mail,target_struct_obj.source_count_name, false, records_count, progress_val, false);
                }
            }

            recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", email_parser_db,Q_FUNC_INFO);
            email_parser_db.close();

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Emlx_Mail,target_struct_obj.source_name,false,records_count,100,false);
            copy_data_from_tmp_mail_db_to_email_parser_db();

        }
    }

    emit signal_add_mail_emlx_files_to_case_tree(email_parser_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Email_Parser_Emlx_Mail);
    emit finished();
}
void thread_mail_emlx::copy_data_from_tmp_mail_db_to_email_parser_db()
{
    mutex_for_email_parser_db->lock();

    QString featr_emails_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    QString email_parser_db_path = featr_emails_parent_path + QString("email_data.sqlite");

    QString tmp_email_parser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString();
    QString tmp_mail_emlx_db_path = tmp_email_parser_dir_path + "/tmp_mail_emlx.sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(email_parser_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("DB open  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        mutex_for_email_parser_db->unlock();
        return;
    }

    QSqlQuery dest_query(destination_db);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS tmp_mailDB")
            .arg(QDir::toNativeSeparators(tmp_mail_emlx_db_path));
    if(!dest_query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug("query attach   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
        destination_db.close();
        mutex_for_email_parser_db->unlock();
        return;
    }

    QString command_str = QString("INSERT INTO main.emlx_data (bookmark,email_type,emlx_subject,emlx_to,emlx_from,"
                                  "emlx_date,emlx_cc,emlx_msg_body,message_text,email_attachment,account_id,"
                                  "mbox,export_file_paths,os_scheme_display,"
                                  "source_count_name,source_file) SELECT bookmark,email_type,emlx_subject,emlx_to,"
                                  "emlx_from,emlx_date,emlx_cc,emlx_msg_body,message_text,email_attachment,"
                                  "account_id,mbox,export_file_paths,os_scheme_display,"
                                  "source_count_name,source_file FROM tmp_mailDB.emlx_data");

    if(!dest_query.exec(command_str))
    {
        recon_static_functions::app_debug("query update   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
        //no need to return here
    }

    QString command_detach = QString("DETACH DATABASE tmp_mailDB");
    dest_query.exec(command_detach);
    destination_db.close();

    mutex_for_email_parser_db->unlock();

    QFile::remove(tmp_mail_emlx_db_path);
}

QString thread_mail_emlx::get_account_id_str(QString filepath)
{
    QString account_id_str;

    QDirIterator dir_account(filepath,QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    QStringList sent_dir_path_list;

    while(dir_account.hasNext())
    {
        QString eml_filepath = dir_account.next();
        QFileInfo info(eml_filepath);
        QString eml_file = info.fileName();
        if(eml_file.startsWith("Sent",Qt::CaseInsensitive))
        {
            sent_dir_path_list << eml_filepath;
        }
    }

    if(sent_dir_path_list.size() < 1)
    {
        return "";
    }

    for(int ii = 0; ii < sent_dir_path_list.size(); ii++)
    {
        if(!account_id_str.isEmpty())
        {
            break;
        }

        QString sent_dir_path = sent_dir_path_list.at(ii);
        QDirIterator sent_iter(sent_dir_path,QDir::Files,QDirIterator::Subdirectories);

        while(sent_iter.hasNext())
        {
            QString eml_filepath = sent_iter.next();
            QFile file_info(eml_filepath);
            if(!file_info.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("File Open -----FAILED----- " + file_info.fileName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Error :- " + file_info.errorString(),Q_FUNC_INFO);
                continue;
            }

            if(file_info.fileName().endsWith(".emlx"))
            {
                QByteArray arr = file_info.readAll();
                file_info.close();
                if(arr.size() !=  0)
                {
                    account_id_str = emlx_account_id_from_sentmessage(&arr);
                    if(!account_id_str.isEmpty())
                    {
                        break;
                    }
                }
            }
            file_info.close();
        }
    }

    return account_id_str;
}

bool thread_mail_emlx::open_and_create_email_parser_db(QString email_parser_db_path)
{
    QFileInfo info_db(email_parser_db_path);
    if(info_db.exists())
    {
        QString connection_naam = Q_FUNC_INFO;
        QSqlDatabase::removeDatabase(connection_naam);
        email_parser_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        email_parser_db.setDatabaseName(email_parser_db_path);
        if(!email_parser_db.open())
        {
            recon_static_functions::app_debug(" email_parser Open  -------FAILED------ " + email_parser_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(email_parser_db.lastError().text() , Q_FUNC_INFO);
            return false;
        }

        return true;
    }
}

bool thread_mail_emlx::open_fs_db(QString fs_db)
{
    bool db_status = false;

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    file_system_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
    file_system_db.setDatabaseName(fs_db);
    if(!file_system_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + file_system_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + file_system_db.lastError().text(),Q_FUNC_INFO);
    }
    else
    {
        db_status = true;
    }

    return db_status;
}

void thread_mail_emlx::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutex_for_fs_main_db = mutex;
}

void thread_mail_emlx::pub_set_mutex_for_mail_emlx_db(QMutex *mutex)
{
    mutex_for_email_parser_db = mutex;
}

void thread_mail_emlx::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_mail_emlx::pub_set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_mail_emlx::emlx_start_parsing(QString filepath)
{

    QFile myfile(filepath);
    if (!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open ----FAILED---" + myfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + myfile.errorString(),Q_FUNC_INFO);
        return ;
    }

    QByteArray arr = myfile.readAll();
    myfile.close();

    if(arr.size() !=0)
    {
        emlx_to(&arr);
        emlx_from(&arr);
        emlx_subject(&arr);
        emlx_cc(&arr);
        emlx_date(&arr);
        emlx_message_body(&arr);
        emlx_attachment(&arr);
        emlx_partial_attachments_decode();
    }
}


void thread_mail_emlx::emlx_to(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_to] -Start" ,Q_FUNC_INFO);

    QString to;
    QByteArray to_arr;

    to_arr.append(0x0A);
    to_arr.append(0x54);
    to_arr.append(0x6F);
    to_arr.append(0x3A);
    to_arr.append(0x20);

    qint64 start_index = 0 ,end_index = 0;
    start_index = arr->indexOf(to_arr,end_index);

    if(start_index > 0)
    {
        start_index++;

        end_index = arr->indexOf("\n",start_index);
        if(end_index > start_index)
        {
            to = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));
            parser_stop_index = end_index;

            if(to.contains(","))
            {
                to.append(emlx_multiple(arr,parser_stop_index));
                while(1)
                {
                    if(to.endsWith(","))
                    {
                        to.append(emlx_multiple(arr , parser_stop_index));
                    }
                    else
                        break;
                }
            }
        }
    }

    if(to.startsWith(QString("To: ")))
        to.remove(0, QString("To: ").size());

    QStringList multiple_to_list;
    QStringList to_list;

    if(to.contains(","))
    {
        multiple_to_list = to.split(",",Qt::SkipEmptyParts);

        for(int ii = 0; ii < multiple_to_list.size(); ii++)
        {
            QString tmp_str = multiple_to_list.at(ii);
            if(tmp_str.contains("<"))
            {
                QStringList tmp_mail;
                tmp_mail << tmp_str.split("<",Qt::SkipEmptyParts);
                if(tmp_mail.size() > 1)
                {
                    to_list << tmp_mail.at(1);
                }
            }
            else
            {
                to_list << tmp_str;
            }
        }
    }
    else if(to.contains("<"))
    {
        multiple_to_list = to.split("<",Qt::SkipEmptyParts);

        for(int ii = 0; ii < multiple_to_list.size(); ii++)
        {
            QString tmp_str = multiple_to_list.at(ii);
            if(tmp_str.contains("@") && tmp_str.contains(">"))
            {
                to_list << tmp_str;
            }
        }
    }
    else
    {
        to_list << to;
    }

    QString final_to_str;

    for(int ii = 0; ii < to_list.size(); ii++)
    {
        final_to_str.append(to_list.at(ii));
        final_to_str.append(",");
    }

    final_to_str.remove(final_to_str.size()-1,final_to_str.size()-1);


    if(final_to_str.contains('"'))
        final_to_str.remove('"');
    if(final_to_str.contains("<"))
        final_to_str.remove("<");
    if(final_to_str.contains(">"))
        final_to_str.remove(">");

    if(final_to_str.endsWith(","))
        final_to_str.remove(",");

    final_to_str = final_to_str.trimmed();

    emlx_to_str = final_to_str.trimmed();

    recon_static_functions::app_debug("[emlx_to] -End" ,Q_FUNC_INFO);

}

void thread_mail_emlx::emlx_from(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_from] -Start" ,Q_FUNC_INFO);

    qint64 start_index = 0,end_index = 0;
    QString from;

    start_index = arr->indexOf("From: ",end_index);

    if(start_index > 0)
    {
        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            from = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));
        }

    }
    if(from.startsWith(QString("From: ")))
        from.remove(0, QString("From: ").size());

    QString final_from_str;

    if(from.contains("<"))
    {
        QStringList tmp_from_list;
        tmp_from_list = from.split("<",Qt::SkipEmptyParts);

        for(int ii = 0; ii < tmp_from_list.size(); ii++)
        {
            QString tmp_str = tmp_from_list.at(ii);
            if(tmp_str.contains("@") && tmp_str.contains(">"))
            {
                final_from_str = tmp_str;
            }
        }
    }
    else
    {
        final_from_str = from;
    }


    if(final_from_str.contains('"'))
        final_from_str.remove('"');
    if(final_from_str.contains("<"))
        final_from_str.remove("<");
    if(final_from_str.contains(">"))
        final_from_str.remove(">");

    if(final_from_str.endsWith(","))
        final_from_str.remove(",");

    final_from_str = final_from_str.trimmed();
    emlx_from_str = final_from_str;

    recon_static_functions::app_debug("[emlx_from] -End" ,Q_FUNC_INFO);

}

void thread_mail_emlx::emlx_cc(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_cc] -Start" ,Q_FUNC_INFO);

    qint64 start_index = 0,end_index = 0;
    QString emlx_cc;

    start_index = arr->indexOf("Cc: ",end_index);

    if(start_index > 0)
    {
        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            emlx_cc = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));
            parser_stop_index = end_index;

            if(emlx_cc.contains(","))
            {
                emlx_cc.append(emlx_multiple(arr,parser_stop_index));

                while(1)
                {
                    if(emlx_cc.endsWith(","))
                    {
                        emlx_cc.append(emlx_multiple(arr , parser_stop_index));
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

    }
    if(emlx_cc.startsWith(QString("Cc: ")))
        emlx_cc.remove(0, QString("Cc: ").size());

    QStringList multiple_cc_list;
    QStringList cc_list;

    if(emlx_cc.contains(","))
    {
        cc_list = emlx_cc.split(",",Qt::SkipEmptyParts);

        for(int ii = 0; ii < cc_list.size(); ii++)
        {
            QString tmp_str = cc_list.at(ii);
            if(tmp_str.contains("<"))
            {
                QStringList tmp_mail;
                tmp_mail << tmp_str.split("<",Qt::SkipEmptyParts);
                if(tmp_mail.size() > 1)
                {
                    multiple_cc_list << tmp_mail.at(1);
                }
            }
            else
            {
                multiple_cc_list << tmp_str;
            }
        }
    }
    else if(emlx_cc.contains("<"))
    {
        cc_list = emlx_cc.split("<",Qt::SkipEmptyParts);

        for(int ii = 0;ii < cc_list.size(); ii++)
        {
            QString tmp_str = cc_list.at(ii);
            if(tmp_str.contains("@") && tmp_str.contains(">"))
            {
                multiple_cc_list << tmp_str;
            }
        }
    }
    else
    {
        multiple_cc_list << emlx_cc;
    }

    QString final_cc_str;

    for(int ii = 0;ii < multiple_cc_list.size(); ii++)
    {
        final_cc_str.append(multiple_cc_list.at(ii));
        final_cc_str.append(",");
    }

    final_cc_str.remove(final_cc_str.size()-1,final_cc_str.size()-1);

    if(final_cc_str.contains('"'))
        final_cc_str.remove('"');
    if(final_cc_str.contains("<"))
        final_cc_str.remove("<");
    if(final_cc_str.contains(">"))
        final_cc_str.remove(">");
    if(final_cc_str.endsWith(","))
        final_cc_str.remove(",");

    final_cc_str = final_cc_str.trimmed();

    emlx_cc_str = final_cc_str.trimmed();

    recon_static_functions::app_debug("[emlx_cc] -End" ,Q_FUNC_INFO);

}

QString thread_mail_emlx::emlx_account_id_from_sentmessage(QByteArray *arr)
{
    qint64 start_index = 0,end_index = 0;
    QString from;
    QString account_id_str;
    start_index = arr->indexOf("From: ",end_index);

    if(start_index > 0)
    {
        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            from = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));
        }

    }

    if(from.isEmpty())
        return "";

    if(from.startsWith(QString("From: ")))
        from.remove(0, QString("From: ").size());

    QString final_from_str;

    if(from.contains("<"))
    {
        QStringList tmp_from_list;
        tmp_from_list = from.split("<",Qt::SkipEmptyParts);

        for(int ii = 0; ii < tmp_from_list.size(); ii++)
        {
            QString tmp_str = tmp_from_list.at(ii);
            if(tmp_str.contains("@") && tmp_str.contains(">"))
            {
                final_from_str = tmp_str;
            }
        }
    }
    else
    {
        final_from_str = from;
    }


    if(final_from_str.contains('"'))
        final_from_str.remove('"');
    if(final_from_str.contains("<"))
        final_from_str.remove("<");
    if(final_from_str.contains(">"))
        final_from_str.remove(">");

    if(final_from_str.endsWith(","))
        final_from_str.remove(",");

    final_from_str = final_from_str.trimmed();
    account_id_str = final_from_str;

    return account_id_str;
}

void thread_mail_emlx::emlx_subject(QByteArray *arr)
{
    qint64 start_index = 0,end_index = 0;
    QString subject;

    start_index = arr->indexOf("Subject: ",end_index);

    if(start_index > 0)
    {

        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            subject = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));

        }

    }
    subject.remove("Subject: ");
    emlx_subject_str = subject;

}

void thread_mail_emlx::emlx_date(QByteArray *arr)
{
    qint64 start_index = 0,end_index = 0;
    QString date;

    start_index = arr->indexOf("Date: ",end_index);

    if(start_index > 0)
    {
        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            date = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));

            date.remove("Date: ");
            date.remove(date.size()-6,5);

            ///replace double space by single space
            if(date.contains(",  "))
                date.replace(",  ", ", ");

        }
    }

    date = date.trimmed();
    if(date.endsWith(")"))
        date.chop(1);

    if(!date.isEmpty())
    {
        QStringList time_split_list;
        QString offset_str,time_str;

        qint64 epoch_timestamp = 0;

        if(date.contains("+"))
        {
            //Fri, 28 Dec 2012 12:34:50 +0800 (PST
            time_split_list = date.split("+");
            time_str   = time_split_list.at(0).trimmed();
            offset_str = time_split_list.at(1).trimmed();


            //remove (PST
            if(offset_str.contains(" ("))
            {
                time_split_list.clear();
                time_split_list = offset_str.split(" (");
                offset_str = offset_str.at(0);
                offset_str = offset_str.trimmed();
            }
            //Fri, 28 Dec 2012 12:34:50 +0800


            QString format = "ddd, d MMM yyyy hh:mm:ss";
            QDateTime valid = QDateTime::fromString(time_str, format);
            valid.setTimeSpec(Qt::UTC);
            epoch_timestamp = valid.toSecsSinceEpoch();

            qint64 hour_offset = 0;
            QString hr_str;
            ///first two digits are hours
            for(int count = 0; count <= 1; count++)
            {
                hr_str.append(offset_str.at(count));
            }
            hr_str = hr_str.trimmed();
            hour_offset = hr_str.toInt() * 3600;

            qint64 min_offset = 0;
            QString min_str;
            ///last two digits are hours
            for(int count = 2; count <= 3; count++)
            {
                min_str.append(offset_str.at(count));
            }
            min_str = min_str.trimmed();
            min_offset = min_str.toInt() * 60;

            if(epoch_timestamp > 0)
            {
                epoch_timestamp -= (hour_offset + min_offset);
                emlx_date_str = QString::number(epoch_timestamp);
            }
            else
                emlx_date_str.clear();
        }
        else if(date.contains("-"))
        {
            //Fri, 28 Dec 2012 12:34:50 -0800 (PST

            time_split_list = date.split("-");
            time_str = time_split_list.at(0).trimmed();
            offset_str = time_split_list.at(1).trimmed();

            //remove (PST
            if(offset_str.contains(" ("))
            {
                time_split_list.clear();
                time_split_list = offset_str.split(" (");
                offset_str = offset_str.at(0);
                offset_str = offset_str.trimmed();
            }
            //Fri, 28 Dec 2012 12:34:50 -0800


            QString format = "ddd, d MMM yyyy hh:mm:ss";
            QDateTime valid = QDateTime::fromString(time_str, format);
            valid.setTimeSpec(Qt::UTC);
            epoch_timestamp = valid.toSecsSinceEpoch();

            qint64 hour_offset = 0;
            QString hr_str;
            ///first two digits are hours
            for(int count = 0; count <= 1; count++)
            {
                hr_str.append(offset_str.at(count));
            }
            hr_str = hr_str.trimmed();
            hour_offset = hr_str.toInt() * 3600;

            qint64 min_offset = 0;
            QString min_str;
            ///last two digits are hours
            for(int count = 2; count <= 3; count++)
            {
                min_str.append(offset_str.at(count));
            }
            min_str = min_str.trimmed();
            min_offset = min_str.toInt() * 60;

            if(epoch_timestamp > 0)
            {
                epoch_timestamp += (hour_offset + min_offset);
                emlx_date_str = QString::number(epoch_timestamp);
            }
            else
                emlx_date_str.clear();

        }
        else
        {
            QString format;
            QString sec_str;
            time_split_list = date.split(":");
            sec_str = time_split_list.at(time_split_list.size() -1);

            if(date.contains(","))
            {
                // Thu, 20 Sep 2018 11:01:275
                // or
                // Thu, 20 Sep 2018 11:01:27

                if(sec_str.count() == 2)
                    format  = "ddd, d MMM yyyy hh:mm:ss";
                else if(sec_str.count() == 3)
                    format  = "ddd, d MMM yyyy hh:mm:zzz";

                //remove extra space after comma(",)
                //Thu,  20 Sep 2018 11:01:275
                time_split_list.clear();
                time_split_list = date.split(",");
                date = time_split_list.at(0).trimmed() + ", " + time_split_list.at(1).trimmed();
            }
            else
            {
                // 20 Sep 2018 11:01:275
                // or
                // 20 Sep 2018 11:01:27

                if(sec_str.count() == 2)
                    format  = "d MMM yyyy hh:mm:ss";
                else if(sec_str.count() == 3)
                    format  = "d MMM yyyy hh:mm:zzz";
            }

            if(!format.trimmed().isEmpty())
            {
                QDateTime valid = QDateTime::fromString(date, format);
                valid.setTimeSpec(Qt::UTC);
                epoch_timestamp = valid.toSecsSinceEpoch();

                if(epoch_timestamp > 0)
                    emlx_date_str = QString::number(epoch_timestamp);
                else
                    emlx_date_str.clear();

            }
            else
                emlx_date_str.clear();
        }



    }
    else
        emlx_date_str.clear();




    ///=======Older work for timestamp - Start=======///
    //QStringList tmp_list_1 = date.split(",",Qt::SkipEmptyParts);

    //    int date_index = 1;

    //    for(int i=0;i<tmp_list_1.size();i++)
    //    {
    //        QString tmp_str = tmp_list_1.at(i);
    //        if(i==date_index)
    //        {
    //            emlx_date_str.append(tmp_str);

    //            date_index +=2;
    //        }
    //    }
    ///=======Older work for timestamp - End=======///

}

void thread_mail_emlx::emlx_message_body(QByteArray *arr)
{
    QString message_body_str = "";
    QString temp_str = "";

    qint64 start_index = 0, end_index = 0;

    if(check_for_multipart(arr))
    {
        while(start_index != -1)
        {
            start_index = arr->indexOf("Content-Type: ", end_index);

            if(start_index > 0)
            {
                end_index = arr->indexOf(";" , start_index);

                if(end_index > start_index)
                {
                    QString content_type = "";

                    content_type = QString::fromLocal8Bit(arr->mid(start_index , (end_index - start_index))).trimmed();

                    if(content_type == "Content-Type: text/plain" || content_type == "Content-Type: TEXT/PLAIN")
                    {

                        bool encoded_text = emlx_check_text_encoded(arr);


                        qint64 content_start_index = 0 , content_end_index = 0;

                        content_start_index = arr->indexOf("\n\n", end_index);

                        ///check again for encoded text
                        if(!encoded_text)
                            encoded_text = emlx_failsafe_text_encoded_check(content_start_index,arr);


                        if(content_start_index > end_index)
                        {

                            content_end_index = arr->indexOf("\n--", content_start_index);


                            if(content_end_index > content_start_index)
                            {
                                message_body_str = QString::fromLocal8Bit(arr->mid(content_start_index , (content_end_index - content_start_index))).trimmed();


                                if(encoded_text)
                                    message_body_str = QString::fromLocal8Bit(emlx_decode_base64(message_body_str.toLocal8Bit()));

                                message_body_str = emlx_normalize_characters(message_body_str);
                                temp_str = message_body_str;
                            }
                        }
                    }



                    if(content_type == "Content-Type: text/html")
                    {

                        qint64 content_start_index = 0 , content_end_index = 0;

                        content_start_index = arr->indexOf("<html>", end_index);

                        if(content_start_index > end_index)
                        {
                            content_end_index = arr->indexOf("</html>", content_start_index);

                            if(content_end_index > content_start_index)
                            {

                                content_end_index += 7;

                                QByteArray message_html_data_arr = arr->mid(content_start_index , (content_end_index - content_start_index)).trimmed();

                                ///----Need to reconfirm this(it is removing '= ')
                                QByteArray remove_tagged_arr;
                                remove_tagged_arr.append(0x3D);
                                remove_tagged_arr.append(0x0A);

                                message_html_data_arr.replace(remove_tagged_arr,"");
                                ///--------------

                                message_body_str = QString::fromLocal8Bit(message_html_data_arr).trimmed();
                            }
                        }

                    }
                }
            }


        }

    }
    else
    {
        ///for normal message
        start_index = arr->indexOf("\n\n", end_index);
        if(start_index > 0)
        {
            end_index = arr->indexOf("<?xml" , start_index);

            if(end_index > start_index)
            {
                message_body_str = QString::fromLocal8Bit(arr->mid(start_index , (end_index - start_index))).trimmed();
                temp_str = message_body_str;

            }

        }
    }

    emlx_message_str = message_body_str;
    elmx_body_msg_without_text_formatting = temp_str;

}

void thread_mail_emlx::emlx_attachment(QByteArray *arr)
{

    if(!email_attachment_exists(arr))
        emlx_attachment_list << QString("");

    emlx_attachment_list.clear();

    QStringList filelist = email_attachment_filelist(arr);

    for(int i = 0; i < filelist.size(); i++)
    {
        QString filename = filelist.at(i);
        qint64 filename_index = 0,data_start_index = 0,data_end_index=0;

        filename_index = arr->indexOf(filename.toStdString(),0);

        if(filename_index > 0)
        {
            data_start_index = arr->indexOf("\n\n",filename_index);

            if(data_start_index>filename_index)
            {
                data_end_index = arr->indexOf("\n--",data_start_index);

                if(data_end_index>data_start_index)
                {
                    QByteArray encoded_arr = arr->mid(data_start_index,(data_end_index-data_start_index));

                    filename.remove("filename=");
                    filename.remove("\"");

                    emlx_attachment_output(filename,encoded_arr);

                    emlx_attachment_list << filename;
                }
            }
        }
    }
}

void thread_mail_emlx::emlx_partial_attachments_decode()
{
    QStringList partial_attach_files_list;

    partial_attach_files_list.clear();

    QFileInfo emlx_info(emlx_filepath);

    if(emlx_info.fileName().contains(".partial"))
        partial_attach_files_list = partial_attachment_files(emlx_info.fileName());

    QStringList attached_filenames_list;
    attached_filenames_list = emlx_attachment_list;

    if(attached_filenames_list.size() == partial_attach_files_list.size())
    {
        for(int i = 0; i < partial_attach_files_list.size(); i++)
        {

            QFile file(partial_attach_files_list.at(i));

            if (!file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("[emlx_partial_attachments_decode]File Open Error: " + file.fileName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(file.errorString(),Q_FUNC_INFO);
                continue;
            }

            QByteArray data_arr = file.readAll();

            file.close();

            emlx_attachment_output(attached_filenames_list.at(i) , data_arr);

        }
    }

}

QString thread_mail_emlx::emlx_multiple(QByteArray *arr,qint64 end_index)
{
    qint64 next_index = 0;
    QString multiple;

    end_index ++;
    next_index = arr->indexOf("\n",end_index);
    multiple = QString::fromLocal8Bit(arr->mid(end_index,(next_index-end_index)));
    parser_stop_index = next_index;
    return multiple;

}

bool thread_mail_emlx::check_for_multipart(QByteArray *arr)
{
    qint64 start_index = 0;

    start_index = arr->indexOf("Content-Type: multipart" , 0);

    if(start_index > 0)
        return true;

    return false;
}

bool thread_mail_emlx::emlx_check_text_encoded(QByteArray *arr)
{
    qint64 start_encode_index = 0;

    QByteArray tmp_arr;
    tmp_arr.append(0x0A);


    QString fnl_str = "Content-Transfer-Encoding: BASE64" + QString::fromLocal8Bit(tmp_arr) + "Content-Type: TEXT/PLAIN;";
    start_encode_index = arr->indexOf(fnl_str.toStdString(), 0);

    if(start_encode_index > 0)
    {
        return true;
    }
    else
    {

        QString fnl_str = "Content-Transfer-Encoding: base64" + QString::fromLocal8Bit(tmp_arr) + "Content-Type: text/plain;";
        start_encode_index = arr->indexOf(fnl_str.toStdString(), 0);

        if(start_encode_index > 0)
        {
            return true;
        }

    }

    return false;
}

bool thread_mail_emlx::emlx_failsafe_text_encoded_check(qint64 text_start_index,QByteArray *arr)
{
    if(text_start_index <= 0)
        return false;

    qint64 text_end_index = text_start_index - 6;

    QString encode_text_str = QString::fromLocal8Bit(arr->mid(text_end_index ,(text_start_index - text_end_index))).trimmed();

    if(encode_text_str == QString("base64") || encode_text_str == QString("Base64"))
        return true;
    else
        return false;

}

QByteArray thread_mail_emlx::emlx_decode_base64(QByteArray data_arr)
{
    if(data_arr.size() == 0)
        return data_arr;

    data_arr =  QByteArray::fromBase64(data_arr);

    return data_arr;
}

QString thread_mail_emlx::emlx_normalize_characters(QString data_str)
{

    if(data_str.size() == 0)
        return data_str;


    data_str.replace("=E2=80=9C" , "\"");
    data_str.replace("=E2=80=9D" , "\"");
    data_str.replace("=E2=80=99" , "'");

    data_str.replace("=\n" , "");

    data_str.replace("=E2=80=94" , "-");

    data_str = QString::fromLocal8Bit(QByteArray::fromPercentEncoding(data_str.toLocal8Bit()));

    return data_str;
}

bool thread_mail_emlx::email_attachment_exists(QByteArray *arr)
{
    qint64 start_encode_index =0;

    start_encode_index= arr->indexOf("Content-Transfer-Encoding: base64",0);

    if(start_encode_index > 0)
    {
        return true;
    }
    else
    {
        start_encode_index = arr->indexOf("Content-Transfer-Encoding: BASE64",0);
        if(start_encode_index > 0)
        {
            return true;
        }
    }
    return false;
}

QStringList thread_mail_emlx::email_attachment_filelist(QByteArray *arr)
{
    QStringList filelist;

    qint64 start_index = 0,end_index = 0;

    while(start_index != -1)
    {
        start_index = arr->indexOf("filename=",end_index);
        if(start_index>0)
        {
            end_index = arr->indexOf("\n",start_index);

            filelist << QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index))).trimmed();

        }
    }

    return filelist;

}

int thread_mail_emlx::emlx_attachment_output(QString filename,QByteArray arr)
{
    QByteArray data_decode = emlx_decode_base64(arr);

    QString emlx_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    destination_atchmnt_loc_emlx_files = emlx_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments;

    if(data_decode.size() !=0)
    {
        filename = recon_static_functions::get_available_filename(filename,destination_atchmnt_loc_emlx_files,Q_FUNC_INFO);
        QString file_path = destination_atchmnt_loc_emlx_files + filename;

        QFile myfile(file_path);

        if(!myfile.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("[emlx_attachment_output]File Open Error: " + myfile.fileName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(myfile.errorString(),Q_FUNC_INFO);
            return -1;
        }

        myfile.write(data_decode,data_decode.size());

        myfile.flush();
        myfile.close();
    }
    else
    {
        QString attachment_dir;

        QString tmp_attach_dir = emlx_filepath;
        if(tmp_attach_dir.contains("/Messages/"))
        {
            tmp_attach_dir = tmp_attach_dir.replace("/Messages/" , "/Attachments/");

            if(tmp_attach_dir.endsWith(".emlx"))
                attachment_dir =  tmp_attach_dir.remove(tmp_attach_dir.lastIndexOf(".emlx"), QString(".emlx").size());
            if(attachment_dir.endsWith(".partial"))
                attachment_dir = attachment_dir.remove(attachment_dir.lastIndexOf(".partial"), QString(".partial").size());
        }

        QString attachment_file_path = extract_non_emlx_attachments(filename, attachment_dir);

        if(QFile::exists(attachment_file_path))
            if(QFileInfo(attachment_file_path).isDir())
                recon_static_functions::copy_directory(attachment_file_path , destination_atchmnt_loc_emlx_files + filename,Q_FUNC_INFO);
            else
                QFile::copy(attachment_file_path , destination_atchmnt_loc_emlx_files + filename);

    }

    return 0;
}

QStringList thread_mail_emlx::partial_attachment_files(QString filename)
{
    QStringList partial_attachments_paths_list;

    qint64 filename_max_index = 0;
    filename_max_index = filename.indexOf(".");

    if(filename_max_index <= 0)
        return partial_attachments_paths_list;

    filename = filename.mid(0 , filename_max_index);

    QString current_directory = QFileInfo(emlx_filepath).filePath().remove(QFileInfo(emlx_filepath).fileName());

    QDirIterator it(current_directory, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        QFileInfo files_info(it.next());

        if(files_info.fileName() == "." || files_info.fileName() == "..")
            continue;

        if(files_info.fileName().startsWith(filename) && files_info.fileName().endsWith("emlxpart"))
            partial_attachments_paths_list << files_info.filePath();

    }

    return partial_attachments_paths_list;
}

QStringList thread_mail_emlx::get_index_of_mbox_from_path(QStringList filepath_list)
{
    QStringList mboxes_index_list;

    for(int count = 0; count < filepath_list.size(); count++)
    {
        if(filepath_list.at(count).endsWith(".mbox"))
        {
            mboxes_index_list << QString::number(count);
        }
    }

    return mboxes_index_list;
}

QString thread_mail_emlx::extract_non_emlx_attachments(QString filename, QString dir_path)
{

    //    QString temp_initial_path = regular_source_path;

    //    if(temp_initial_path.endsWith("/"))
    //        temp_initial_path.chop(1);

    //    dir_path = temp_initial_path + dir_path ;

    QDirIterator iter(dir_path, QDirIterator::Subdirectories);


    while(iter.hasNext())
    {
        //        if(bool_cancel_extraction)
        //            return "";

        QString iter_path_str = iter.next();
        QFileInfo file_info(iter_path_str);

        if(file_info.fileName() == QString(".") || file_info.fileName() == QString(".."))
            continue;


        if(file_info.fileName() == filename)
        {
            QString file_path = iter_path_str;
            return file_path;
        }
        else if(filename.endsWith(".zip"))
        {
            QString file_name_search = filename;
            file_name_search = file_name_search.remove(file_name_search.lastIndexOf(".zip"), QString(".zip").size());

            if(file_info.fileName() == file_name_search)
            {
                QString file_path = iter_path_str;
                return file_path;
            }
        }
    }

    return QString("");

}
