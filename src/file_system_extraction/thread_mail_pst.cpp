#include "thread_mail_pst.h"

thread_mail_pst::thread_mail_pst(QObject *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);
}

thread_mail_pst::~thread_mail_pst()
{

}

void thread_mail_pst::slot_pst_mail_extraction()
{
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Email_Parser_Pst_Mail);
    QString tmp_email_parser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString();
    QString tmp_mail_pst_db_path = tmp_email_parser_dir_path + "/tmp_mail_pst.sqlite";

    QStringList pst_mail_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");

        QString pst_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString()
                + QString("/email_data.sqlite");

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            pst_mail_db_path_list << (pst_db_path);
        }
        else
        {
            pst_mail_db_path_list << (pst_db_path);

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
            {
                continue;
            }

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            qlist_struct_pst_mail_file.clear();
            QStringList cp_args;
            cp_args << pst_db_path << tmp_mail_pst_db_path;

            QProcess proc_cp_email_parser_db;

            proc_cp_email_parser_db.start("cp", cp_args);

            proc_cp_email_parser_db.waitForFinished(-1);

            proc_cp_email_parser_db.close();

            QString delete_cmd = "DELETE FROM emlx_data";
            recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd,tmp_mail_pst_db_path,Q_FUNC_INFO);

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Pst_Mail, target_struct_obj.source_name, false, 0, 0, false);

            mutext_for_fs_main_db->lock();
            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase file_system_main_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            file_system_main_db.setDatabaseName(filesystem_db_path);
            if(!file_system_main_db.open())
            {
                recon_static_functions::app_debug("fs_db open ------FAILED------ " + file_system_main_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Error :- " + file_system_main_db.lastError().text(),Q_FUNC_INFO);
                mutext_for_fs_main_db->unlock();
                continue;
            }

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files Where extension = 'pst'") , 0, file_system_main_db, Q_FUNC_INFO);
            qint64  total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                file_system_main_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            qint64 processed_file_count_qint64 = 0;
            QString estimated_time;
            qint64 remaining_file_count_for_req_time_qint64;

            remaining_file_count_for_req_time_qint64 = total_records_num;
            qint64 start_time_epoch = QDateTime::currentSecsSinceEpoch();

            QString select_cmd_fs;
            if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                select_cmd_fs = QString("SELECT INT, id_path, filename, source_count_name from files WHERE extension = 'pst'");
            }
            else
            {
                select_cmd_fs = QString("SELECT INT, filepath, filename, source_count_name from files WHERE extension = 'pst'");
            }

            QSqlQuery query_select(file_system_main_db);
            query_select.prepare(select_cmd_fs);
            if(!query_select.exec())
            {
                recon_static_functions::app_debug("query_select -----FAILED----- " + query_select.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Error :- " + query_select.lastError().text(),Q_FUNC_INFO);
                file_system_main_db.close();
                mutext_for_fs_main_db->unlock();
                continue;
            }

            while(query_select.next())
            {

                if(query_select.value(1).toString().trimmed().isEmpty())
                    continue;

                if(recon_helper_standard_obj->bool_is_permission_restricted_file(query_select.value(1).toString(), Q_FUNC_INFO))
                    continue;

                struct_mail_pst_file_details pst_mail_obj;

                pst_mail_obj.record_no = query_select.value("INT").toString();
                pst_mail_obj.filepath = query_select.value(1).toString();
                pst_mail_obj.file_name = query_select.value("filename").toString();
                pst_mail_obj.source_count_name = query_select.value("source_count_name").toString();

                qlist_struct_pst_mail_file << pst_mail_obj;
            }

            file_system_main_db.close();
            mutext_for_fs_main_db->unlock();

            qint64 records_count = 0;
            QStringList record_num_list;

            if(qlist_struct_pst_mail_file.size() == 0)
                continue;


            for(int ii = 0; ii < qlist_struct_pst_mail_file.size(); ii++)
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

                struct_mail_pst_file_details pst_mail_obj = qlist_struct_pst_mail_file.at(ii);

                QString record_num = pst_mail_obj.record_no;
                QString file_path = pst_mail_obj.filepath;

                QString source_cnt_name = pst_mail_obj.source_count_name;

                QString complete_tmp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString()
                        + source_cnt_name + "/Pst_" + record_num;

                QString virtual_source_path = target_struct_obj.virtual_source_path;
                QString os_scheme = target_struct_obj.os_scheme_display;

                QString full_filepath = recon_static_functions::get_complete_file_path(virtual_source_path,file_path,Q_FUNC_INFO);

                QDir dir(complete_tmp_dir_path);
                if(!dir.exists())
                {
                    dir.mkpath(complete_tmp_dir_path);

                    QString path = "./readpst/readpst";
                    QProcess process_pst;
                    QStringList args_list;
                    args_list  << "-e" << "-o" << complete_tmp_dir_path << full_filepath;

                    process_pst.start(path, args_list);
                    process_pst.waitForFinished();

                    while(1)
                    {

                        QCoreApplication::processEvents();
                        if(process_pst.state() == QProcess::NotRunning)
                        {
                            break;
                        }
                    }

                    process_pst.close();
                }

                QString connection_naam_1 = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam_1);
                QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_1);
                destination_db.setDatabaseName(tmp_mail_pst_db_path);
                if(!destination_db.open())
                {
                    recon_static_functions::app_debug("Destination Db opening -----FAILED----- " + destination_db.databaseName(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error :- " + destination_db.lastError().text(),Q_FUNC_INFO);
                    QSqlDatabase::removeDatabase(connection_naam);
                    return;
                }

                QSqlQuery query(destination_db);
                QString insert_cmd_str = "INSERT INTO emlx_data (bookmark, email_type, emlx_subject, emlx_to, emlx_from, emlx_date,"
                                         "emlx_cc, emlx_msg_body, email_attachment, account_id, mbox,"
                                         "file_raw_bytes, file_name, source_file, os_scheme_display, source_count_name, message_text, export_file_paths) "
                                         "VALUES(0,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?,?,?,?)";
                eml_account_id_str.clear();
                eml_account_id_str = pub_get_account_id_str(complete_tmp_dir_path);

                if(eml_account_id_str.isEmpty())
                {
                    eml_account_id_str = "Orphan";
                }

                QDirIterator pst_dir(complete_tmp_dir_path,QStringList() << "*.eml" , QDir::Files, QDirIterator::Subdirectories);
                while(pst_dir.hasNext())
                {

                    bool_account_id = true;

                    eml_filepath = pst_dir.next();
                    if(eml_filepath.endsWith(".eml"))
                    {
                        QString file_complete_path = eml_filepath;
                        QStringList list_split_path = file_complete_path.split("/",Qt::SkipEmptyParts);

                        QFileInfo file_info(eml_filepath);
                        QString folder_name = file_info.absoluteDir().dirName();

                        eml_mailbox_str = folder_name.trimmed();
                        eml_filepath = eml_filepath.trimmed();

                        eml_start_parsing(eml_filepath);

                        eml_file_bytes = eml_file_bytes.trimmed();
                        eml_attachment_list.removeAll("");

                        eml_to_str = eml_to_str.trimmed();
                        eml_from_str = eml_from_str.trimmed();
                        eml_subject_str = eml_subject_str.trimmed();
                        eml_date_str    = eml_date_str.trimmed();
                        eml_message_str = eml_message_str.trimmed();
                        eml_mailbox_str    = eml_mailbox_str.trimmed();
                        eml_account_id_str = eml_account_id_str.trimmed();

                        query.prepare(insert_cmd_str);
                        query.addBindValue("Pst");

                        query.addBindValue(eml_subject_str);
                        query.addBindValue(eml_to_str);
                        query.addBindValue(eml_from_str);
                        query.addBindValue(eml_date_str);
                        query.addBindValue(eml_cc_str);
                        query.addBindValue(eml_message_str);
                        query.addBindValue(eml_attachment_list.join(","));

                        query.addBindValue(eml_account_id_str);
                        query.addBindValue(eml_mailbox_str);
                        query.addBindValue(eml_file_bytes);
                        query.addBindValue(pst_mail_obj.file_name);

                        QString relative_source_file = full_filepath;
                        if(relative_source_file.startsWith(virtual_source_path))
                        {
                            relative_source_file.remove(0,virtual_source_path.size());
                            if(!relative_source_file.startsWith("/"))
                                relative_source_file.prepend("/");
                        }

                        relative_source_file = relative_source_file.trimmed();
                        elm_body_msg_without_text_formatting  = elm_body_msg_without_text_formatting.trimmed();

                        query.addBindValue(relative_source_file);
                        query.addBindValue(os_scheme);
                        query.addBindValue(target_struct_obj.source_count_name);
                        query.addBindValue(elm_body_msg_without_text_formatting);

                        QString export_file_path;
                        if(eml_attachment_list.size() != 0)
                        {
                            for(int ii = 0; ii < eml_attachment_list.size(); ii++)
                            {
                                export_file_path.append("/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments) + eml_attachment_list.at(ii));
                                export_file_path.append(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple));
                            }

                            if(export_file_path.endsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)))
                                export_file_path.chop(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple).size());

                            export_file_path = export_file_path.trimmed();

                            query.addBindValue(export_file_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple) + relative_source_file);
                        }
                        else
                        {
                            query.addBindValue(relative_source_file);
                        }

                        if(!query.exec())
                        {
                            recon_static_functions::app_debug("Query Create -----FAILED----- " + query.executedQuery(),Q_FUNC_INFO);
                            recon_static_functions::app_debug("Error :- " + query.lastError().text(),Q_FUNC_INFO);
                            continue;
                        }

                        eml_subject_str.clear();
                        eml_to_str.clear();
                        eml_from_str.clear();
                        eml_date_str.clear();
                        eml_cc_str.clear();
                        eml_message_str.clear();
                        eml_attachment_list.clear();
                        elm_body_msg_without_text_formatting.clear();
                        eml_file_bytes.clear();
                    }
                }

                destination_db.commit();
                destination_db.close();

                if(estimated_time.isEmpty() || estimated_time == "00:00:00")
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Email_Parser_Pst_Mail, "Calculating...");
                else
                    emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Email_Parser_Pst_Mail, estimated_time);

                if(records_count % 1 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);
                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Pst_Mail, target_struct_obj.source_name, false, records_count, progress_val, false);
                }
            }

            copy_data_from_tmp_mail_db_to_email_parser_db();
        }
    }

    emit signal_add_pst_mail_to_case_tree(pst_mail_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Email_Parser_Pst_Mail);
    emit finished();
}
void thread_mail_pst::copy_data_from_tmp_mail_db_to_email_parser_db()
{
    mutext_for_mail_pst_main_db->lock();

    QString featr_emails_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    QString email_parser_db_path = featr_emails_parent_path + QString("email_data.sqlite");

    QString tmp_email_parser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString();
    QString tmp_mail_pst_db_path = tmp_email_parser_dir_path + "/tmp_mail_pst.sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(email_parser_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("DB open  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        mutext_for_mail_pst_main_db->unlock();
        return;
    }

    QSqlQuery dest_query(destination_db);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS tmp_mailDB")
            .arg(QDir::toNativeSeparators(tmp_mail_pst_db_path));
    if(!dest_query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug("query attach   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
        destination_db.close();
        mutext_for_mail_pst_main_db->unlock();
        return;
    }

    QString command_str = QString("INSERT INTO main.emlx_data (bookmark,email_type,emlx_subject,emlx_to,emlx_from,"
                                  "emlx_date,emlx_cc,emlx_msg_body,message_text,email_attachment,account_id,"
                                  "mbox,file_raw_bytes,file_name,export_file_paths,os_scheme_display,"
                                  "source_count_name,source_file) SELECT bookmark,email_type,emlx_subject,emlx_to,"
                                  "emlx_from,emlx_date,emlx_cc,emlx_msg_body,message_text,email_attachment,"
                                  "account_id,mbox,file_raw_bytes,file_name,export_file_paths,os_scheme_display,"
                                  "source_count_name,source_file FROM tmp_mailDB.emlx_data");

    if(!dest_query.exec(command_str))
    {
        recon_static_functions::app_debug("query update   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
    }

    QString command_detach = QString("DETACH DATABASE tmp_mailDB");
    dest_query.exec(command_detach);
    destination_db.close();

    mutext_for_mail_pst_main_db->unlock();

    QFile::remove(tmp_mail_pst_db_path);
}

void thread_mail_pst::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_mail_pst::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_mail_pst::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_mail_pst::pub_set_mutex_for_emlx_db(QMutex *mutex)
{
    mutext_for_mail_pst_main_db = mutex;
}

QString thread_mail_pst::pub_get_account_id_str(QString dir_path)
{
    bool_account_id = false;
    account_id_str.clear();

    QDirIterator dir_account(dir_path,QDir::Files,QDirIterator::Subdirectories);

    while(dir_account.hasNext())
    {
        QString eml_filepath = dir_account.next();
        if(eml_filepath.contains("MeContact") || eml_filepath.contains("Sent Items"))
        {
            QFile file_info(eml_filepath);
            if(!file_info.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("File Open -----FAILED----- " + file_info.fileName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Error :- " + file_info.errorString(),Q_FUNC_INFO);
                return account_id_str;
            }

            if(eml_filepath.contains("MeContact"))
            {
                while(!file_info.atEnd())
                {
                    QString line_data = file_info.readLine();
                    if(line_data.startsWith("EMAIL:"))
                    {
                        QString eml_owner_id = line_data;
                        account_id_str.clear();
                        account_id_str = eml_owner_id.remove("EMAIL:");
                        file_info.close();
                        break;
                    }
                }
            }
            else
            {
                if(file_info.fileName().endsWith(".eml"))
                {
                    QByteArray arr = file_info.readAll();
                    file_info.close();
                    if(arr.size() !=  0)
                    {
                        eml_extract_from(&arr);
                    }
                }
            }

            file_info.close();
        }
    }
    return account_id_str;
}

void thread_mail_pst::eml_start_parsing(QString filepath)
{
    QFile myfile(filepath);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Opening -----FAILED----- " + myfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error :- " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }

    QByteArray arr = myfile.readAll();
    myfile.close();

    if(arr.size() != 0)
    {
        eml_file_bytes = arr;

        eml_extract_to(&arr);
        eml_extract_from(&arr);
        eml_extract_subject(&arr);
        eml_extract_cc(&arr);
        eml_extract_date(&arr);
        eml_extract_message_body(&arr);
        eml_extract_attachment(&arr);
    }
}


void thread_mail_pst::eml_extract_to(QByteArray *arr)
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
                to.append(eml_multiple(arr,parser_stop_index));
                while(1)
                {
                    if(to.endsWith(","))
                    {
                        to.append(eml_multiple(arr , parser_stop_index));
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

    eml_to_str = final_to_str.trimmed();

    recon_static_functions::app_debug("[emlx_to] -End" ,Q_FUNC_INFO);

}

void thread_mail_pst::eml_extract_from(QByteArray *arr)
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
            if(tmp_str.contains(">"))
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
    eml_from_str = final_from_str;

    if(!bool_account_id)
    {
        if(!from.contains("MAILER-DAEMON"))
        {
            int start_index = from.indexOf("<");
            QString mid_str = from.mid(start_index);

            mid_str = mid_str.remove("<");
            account_id_str= mid_str.remove(">");
        }
    }
    recon_static_functions::app_debug("[emlx_from] -End" ,Q_FUNC_INFO);

}

void thread_mail_pst::eml_extract_cc(QByteArray *arr)
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
                emlx_cc.append(eml_multiple(arr,parser_stop_index));

                while(1)
                {
                    if(emlx_cc.endsWith(","))
                    {
                        emlx_cc.append(eml_multiple(arr , parser_stop_index));
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

    eml_cc_str = final_cc_str.trimmed();

    recon_static_functions::app_debug("[emlx_cc] -End" ,Q_FUNC_INFO);

}

void thread_mail_pst::eml_extract_subject(QByteArray *arr)
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
    eml_subject_str = subject;
}

void thread_mail_pst::eml_extract_date(QByteArray *arr)
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
                eml_date_str = QString::number(epoch_timestamp);
            }
            else
                eml_date_str.clear();
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
                eml_date_str = QString::number(epoch_timestamp);
            }
            else
                eml_date_str.clear();
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
                    eml_date_str = QString::number(epoch_timestamp);
                else
                    eml_date_str.clear();

            }
            else
                eml_date_str.clear();
        }
    }
    else
        eml_date_str.clear();
}

void thread_mail_pst::eml_extract_message_body(QByteArray *arr)
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
                        bool encoded_text = eml_check_text_encoded(arr);

                        qint64 content_start_index = 0 , content_end_index = 0;

                        content_start_index = arr->indexOf("\n\n", end_index);

                        ///check again for encoded text
                        if(!encoded_text)
                            encoded_text = eml_failsafe_text_encoded_check(content_start_index,arr);

                        if(content_start_index > end_index)
                        {
                            content_end_index = arr->indexOf("\n--", content_start_index);

                            if(content_end_index > content_start_index)
                            {
                                message_body_str = QString::fromLocal8Bit(arr->mid(content_start_index , (content_end_index - content_start_index))).trimmed();

                                if(encoded_text)
                                    message_body_str = QString::fromLocal8Bit(eml_decode_base64(message_body_str.toLocal8Bit()));

                                message_body_str = eml_normalize_characters(message_body_str);
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

    eml_message_str = message_body_str;
    elm_body_msg_without_text_formatting = temp_str;

}

void thread_mail_pst::eml_extract_attachment(QByteArray *arr)
{
    if(!email_attachment_exists(arr))
        eml_attachment_list << QString("");

    eml_attachment_list.clear();

    QStringList filelist = email_attachment_filelist(arr);

    for(int ii = 0; ii < filelist.size(); ii++)
    {
        QString filename = filelist.at(ii);
        qint64 filename_index = 0, data_start_index = 0 ,data_end_index=0;

        filename_index = arr->indexOf(filename.toStdString(),0);

        if(filename_index > 0)
        {
            data_start_index = arr->indexOf("\n\n",filename_index);

            if(data_start_index>filename_index)
            {
                data_end_index = arr->indexOf("\n--",data_start_index);

                if(data_end_index > data_start_index)
                {
                    QByteArray encoded_arr = arr->mid(data_start_index,(data_end_index-data_start_index));

                    filename.remove("filename=");
                    filename.remove("\"");

                    eml_attachment_output(filename,encoded_arr);

                    eml_attachment_list << filename;
                }
            }
        }
    }
}

QString thread_mail_pst::eml_multiple(QByteArray *arr,qint64 end_index)
{
    qint64 next_index = 0;
    QString multiple;

    end_index ++;
    next_index = arr->indexOf("\n",end_index);
    multiple = QString::fromLocal8Bit(arr->mid(end_index,(next_index-end_index)));
    parser_stop_index = next_index;
    return multiple;

}

bool thread_mail_pst::check_for_multipart(QByteArray *arr)
{
    qint64 start_index = 0;

    start_index = arr->indexOf("Content-Type: multipart" , 0);

    if(start_index > 0)
        return true;

    return false;
}

bool thread_mail_pst::eml_check_text_encoded(QByteArray *arr)
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

bool thread_mail_pst::eml_failsafe_text_encoded_check(qint64 text_start_index,QByteArray *arr)
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

QByteArray thread_mail_pst::eml_decode_base64(QByteArray data_arr)
{
    if(data_arr.size() == 0)
        return data_arr;

    data_arr =  QByteArray::fromBase64(data_arr);

    return data_arr;
}

QString thread_mail_pst::eml_normalize_characters(QString data_str)
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

bool thread_mail_pst::email_attachment_exists(QByteArray *arr)
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

QStringList thread_mail_pst::email_attachment_filelist(QByteArray *arr)
{
    QStringList filelist;

    qint64 start_index = 0,end_index = 0;

    while(start_index != -1)
    {
        start_index = arr->indexOf("filename=",end_index);
        if(start_index > 0)
        {
            end_index = arr->indexOf("\n",start_index);

            filelist << QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index))).trimmed();
        }
    }

    return filelist;
}

int thread_mail_pst::eml_attachment_output(QString filename,QByteArray arr)
{
    QByteArray data_decode = eml_decode_base64(arr);

    QString pst_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    destination_atchmnt_loc_eml_files = pst_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments;

    if(data_decode.size() != 0)
    {
        QString file_name = recon_static_functions::get_available_filename(filename,destination_atchmnt_loc_eml_files,Q_FUNC_INFO);
        QString file_path = destination_atchmnt_loc_eml_files + file_name;

        QFile myfile(file_path);
        if(!myfile.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("File open -----FAILED----- " + myfile.fileName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Error :- " + myfile.errorString(),Q_FUNC_INFO);
            return -1;
        }

        myfile.write(data_decode,data_decode.size());

        myfile.flush();
        myfile.close();
    }

    return 0;
}
