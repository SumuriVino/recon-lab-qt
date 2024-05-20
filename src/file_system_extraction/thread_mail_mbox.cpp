#include "thread_mail_mbox.h"

thread_mail_mbox::thread_mail_mbox(QObject *parent) : QObject(parent)
{

}

thread_mail_mbox::~thread_mail_mbox()
{

}

void thread_mail_mbox::slot_extract_mbox_files_data()
{ // This is automated feature which extracts the email data from .mbox files, display the data on email viewer. Usually these files we found in google takeout sources

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QString featr_emails_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();

    destination_atchmnt_path_feature = featr_emails_parent_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments;

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Email_Parser_Mbox_Mail);

    QStringList email_parser_db_path_list;

    QString email_parser_db_path = featr_emails_parent_path + QString("email_data.sqlite");
    QString tmp_email_parser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString();
    QString tmp_email_parser_db_path = tmp_email_parser_dir_path + "/tmp_mail_mbox.sqlite";

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        target_struct_obj = list_target_source_info.at(count);

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

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Mbox_Mail,target_struct_obj.source_name,false,0,0,true);

            mutex_for_fs_main_db->lock();
            if(!open_fs_db(filesystem_db_path))
            {
                mutex_for_fs_main_db->unlock();
                continue;
            }

            QStringList cp_args;
            cp_args << email_parser_db_path << tmp_email_parser_db_path;

            QProcess proc_cp_email_parser_db;

            proc_cp_email_parser_db.start("cp", cp_args);

            proc_cp_email_parser_db.waitForFinished(-1);

            proc_cp_email_parser_db.close();

            QString delete_cmd = "DELETE FROM emlx_data";
            recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd,tmp_email_parser_db_path,Q_FUNC_INFO);

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("SELECT count(*) FROM files where extension = 'mbox'") , 0, file_system_db, Q_FUNC_INFO);
            total_records_num = total_records_str.toLongLong();
            if(total_records_num <= 0)
            {
                file_system_db.close();
                mutex_for_fs_main_db->unlock();
                continue;
            }

            QString command_fs;
            if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                command_fs = QString("select INT, id_path, filepath, extension, is_dir from files Where extension = 'mbox' AND is_dir = 0");
            }
            else
            {
                command_fs = QString("select INT, filepath, extension, is_dir from files Where extension = 'mbox' AND is_dir = 0");
            }

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

                qlist_struct_fs_rec_and_path << m1;

            }

            file_system_db.close();
            mutex_for_fs_main_db->unlock();


            if(!open_tmp_email_parser_db(tmp_email_parser_db_path))
            {
                continue;
            }

            for(int num = 0; num < qlist_struct_fs_rec_and_path.size(); num++)
            {

                struct_filesystem_record_and_filepath fs_rec_filepath_obj = qlist_struct_fs_rec_and_path.at(num);

                QString filepath = fs_rec_filepath_obj.filepath;

                if(fs_rec_filepath_obj.extension != "mbox" && fs_rec_filepath_obj.is_dir == "1")
                    continue;

                mbox_filepath_str = recon_static_functions::get_complete_file_path(target_struct_obj.virtual_source_path,filepath,Q_FUNC_INFO);
                mbox_filepath_str = mbox_filepath_str.trimmed();

                start_mbox_parsing_for_feature(mbox_filepath_str);

                QString select_cmd = "SELECT account_id from emlx_data WHERE source_count_name = '"+ target_struct_obj.source_count_name +"' AND source_file = '" + filepath +"' AND account_id != 'Orphan'";
                QStringList account_id_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd,0,tmp_email_parser_db,Q_FUNC_INFO);
                account_id_list.removeDuplicates();

                QString account_id_str;
                if(!account_id_list.isEmpty())
                    account_id_str = account_id_list.at(0);

                if(!account_id_str.isEmpty())
                {
                    QString update_cmd = "UPDATE emlx_data SET account_id = '" + account_id_str + "' WHERE account_id = 'Orphan' AND source_count_name = '" + target_struct_obj.source_count_name + "' AND source_file = '" + filepath + "'";
                    recon_helper_standard_obj->execute_db_command_by_dbreference(update_cmd,tmp_email_parser_db,Q_FUNC_INFO);
                }
                mbox_attachment_list.removeAll("");

            }

            recon_helper_standard_obj->create_db_index_by_db_reference("fs_parent","filesystem_record", "files", tmp_email_parser_db,Q_FUNC_INFO);
            tmp_email_parser_db.close();

            copy_data_from_tmp_mail_db_to_email_parser_db();

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Mbox_Mail,target_struct_obj.source_name,false,records_count,100,true);

        }
    }
    emit signal_add_mail_mbox_files_to_case_tree(email_parser_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Email_Parser_Mbox_Mail);
    emit finished();
}

// Here below the parsing code of mbox files which parses subject, to, from, cc etc and other email data, store these info in email_data.sqlite and display on email viewer
void thread_mail_mbox::start_mbox_parsing_for_feature(QString mbox_file_path)
{

    QFile file(mbox_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("[start_mbox_parsing_for_feature] File Open Failed:" + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("[start_mbox_parsing_for_feature] Error:" + file.errorString(),Q_FUNC_INFO);
        return;
    }

    const QString mail_start_str = "From ";

    Status status = ENUM_MAIL_CHECK;

    QString insert_cmd = "insert into emlx_data  (bookmark,email_type, emlx_subject,emlx_to,emlx_from,emlx_date,emlx_cc,emlx_msg_body,email_attachment,"
                         "account_id,mbox,source_file,os_scheme_display,source_count_name,message_text,export_file_paths,preview_file_path,file_raw_bytes) values(0,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) ";

    QString mail_data_str;
    QString line_str;
    QString preview_file_path;

    mbox_subject_str.clear();
    mbox_to_str.clear();
    mbox_from_str.clear();
    mbox_date_str.clear();
    mbox_cc_str.clear();
    mbox_message_str.clear();
    mbox_attachment_list.clear();
    mbox_account_id_str.clear();
    mbox_mailbox_str.clear();
    mbox_body_msg_without_text_formatting.clear();

    while(status != ENUM_MAIL_END)
    {

        switch (status)
        {
        case ENUM_MAIL_CHECK:
            //read one line
            line_str = QString::fromLocal8Bit(file.readLine());

            if(file.atEnd())
            {
                status = ENUM_MAIL_END;
                break;
            }

            //proceed only if it starts with start header
            if (line_str.startsWith(mail_start_str))
                status = ENUM_MAIL_PROCESS;
            break;
        case ENUM_MAIL_PROCESS:
            //add data untill next start appears
            mail_data_str += line_str;

            //read one line
            line_str = QString::fromLocal8Bit(file.readLine());

            if (line_str.startsWith(mail_start_str))
            {

                ///parse data from mbox file
                mail_data_start_parsing_feature(mail_data_str);

                mbox_subject_str = mbox_subject_str.trimmed();
                mbox_to_str      = mbox_to_str.trimmed();
                mbox_from_str    = mbox_from_str.trimmed();
                mbox_date_str    = mbox_date_str.trimmed();
                mbox_message_str = mbox_message_str.trimmed();
                mbox_account_id_str = mbox_account_id_str.trimmed();
                mbox_mailbox_str    = mbox_mailbox_str.trimmed();
                mbox_cc_str  = mbox_cc_str.trimmed();


                if(mbox_mailbox_str == QString("Sent"))
                {
                    QByteArray arr = mail_data_str.toLocal8Bit();
                    mbox_account_id_from_sentmessage(&arr);
                }

                if(mbox_account_id_str.isEmpty())
                {
                    mbox_account_id_str = "Orphan";
                }

                if(mbox_mailbox_str.isEmpty())
                {
                    mbox_mailbox_str = "Orphan";
                }

                QSqlQuery query_insert(tmp_email_parser_db);

                QString relative_source_file = mbox_file_path;
                if(relative_source_file.startsWith(target_struct_obj.virtual_source_path))
                {
                    relative_source_file.remove(0,target_struct_obj.virtual_source_path.size());
                    if(!relative_source_file.startsWith("/"))
                        relative_source_file.prepend("/");
                }
                relative_source_file = relative_source_file.trimmed();

                query_insert.prepare(insert_cmd);
                query_insert.addBindValue("Mbox");

                query_insert.addBindValue(mbox_subject_str);
                query_insert.addBindValue(mbox_to_str);
                query_insert.addBindValue(mbox_from_str);
                query_insert.addBindValue(mbox_date_str);
                query_insert.addBindValue(mbox_cc_str);
                query_insert.addBindValue(mbox_message_str);
                query_insert.addBindValue(mbox_attachment_list.join(","));
                query_insert.addBindValue(mbox_account_id_str);
                query_insert.addBindValue(mbox_mailbox_str);

                mbox_body_msg_without_text_formatting = mbox_body_msg_without_text_formatting.trimmed();

                query_insert.addBindValue(relative_source_file);
                query_insert.addBindValue(target_struct_obj.os_scheme_display);
                query_insert.addBindValue(target_struct_obj.source_count_name);
                query_insert.addBindValue(mbox_body_msg_without_text_formatting);

                QString export_file_path ;
                if(mbox_attachment_list.size() != 0)
                {
                    for(int i = 0 ;i < mbox_attachment_list.size() ; i++)
                    {
                        QString tmp_path = "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments) + mbox_attachment_list.at(i);
                        export_file_path.append(tmp_path);
                        export_file_path.append(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple));

                        if(i == 0)
                            preview_file_path = tmp_path;

                    }

                    if(export_file_path.endsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)))
                        export_file_path.chop(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple).size());

                    export_file_path = export_file_path.trimmed();

                    query_insert.addBindValue(export_file_path);

                }
                else
                {
                    export_file_path = export_file_path.trimmed();

                    query_insert.addBindValue(export_file_path);
                    preview_file_path = export_file_path;
                }

                preview_file_path = preview_file_path.trimmed();
                if(!preview_file_path.isEmpty() && !preview_file_path.startsWith("."))
                    preview_file_path.prepend(".");

                preview_file_path = preview_file_path.trimmed();

                query_insert.addBindValue(preview_file_path);

                query_insert.addBindValue(mail_data_str.toLocal8Bit());

                if(!query_insert.exec())
                {
                    recon_static_functions::app_debug("[start_mbox_parsing_for_feature] Query insert ----FAILED---" + query_insert.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("[start_mbox_parsing_for_feature] Error:" + query_insert.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                records_count++;

                if(records_count % 100 == 0)
                {
                    qint64 progress_val = ((records_count * 100) / total_records_num);

                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Email_Parser_Mbox_Mail,target_struct_obj.source_count_name, false, records_count, progress_val, true);
                }


                mbox_subject_str.clear();
                mbox_to_str.clear();
                mbox_from_str.clear();
                mbox_date_str.clear();
                mbox_cc_str.clear();
                mbox_message_str.clear();
                mbox_attachment_list.clear();
                mbox_body_msg_without_text_formatting.clear();
                mbox_account_id_str.clear();

                mail_data_str.clear();

            }

            if(file.atEnd())
                status = ENUM_MAIL_END;

            break;
        case ENUM_MAIL_END:
            break;

        default:
            status = ENUM_MAIL_END;
            break;
        }
    }

    file.close();

}

void thread_mail_mbox::copy_data_from_tmp_mail_db_to_email_parser_db()
{
    mutex_for_email_parser_db->lock();

    QString featr_emails_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    QString email_parser_db_path = featr_emails_parent_path + QString("email_data.sqlite");

    QString tmp_email_parser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString();
    QString tmp_email_parser_db_path = tmp_email_parser_dir_path + "/tmp_mail_mbox.sqlite";

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
            .arg(QDir::toNativeSeparators(tmp_email_parser_db_path));
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
                                  "mbox,file_raw_bytes,file_name,bookmarked_extended_attributes,"
                                  "bookmarked_exif_data,extended_attributes_key_value,"
                                  "extended_attributes_value_for_search,LINKFILENAME,preview_file_path,"
                                  "export_file_paths,recon_tag_value,notes_icon,tags_icon,os_scheme_display,"
                                  "source_count_name,source_file,notes) SELECT bookmark,email_type,emlx_subject,emlx_to,"
                                  "emlx_from,emlx_date,emlx_cc,emlx_msg_body,message_text,email_attachment,"
                                  "account_id,mbox,file_raw_bytes,file_name,bookmarked_extended_attributes,"
                                  "bookmarked_exif_data,extended_attributes_key_value,"
                                  "extended_attributes_value_for_search,LINKFILENAME,preview_file_path,"
                                  "export_file_paths,recon_tag_value,notes_icon,tags_icon,os_scheme_display,"
                                  "source_count_name,source_file,notes FROM tmp_mailDB.emlx_data");

    if(!dest_query.exec(command_str))
    {
        recon_static_functions::app_debug("query update   ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastQuery(),Q_FUNC_INFO);
    }

    QString command_detach = QString("DETACH DATABASE tmp_mailDB");
    dest_query.exec(command_detach);
    destination_db.close();
    mutex_for_email_parser_db->unlock();

    QFile::remove(tmp_email_parser_db_path);
}

bool thread_mail_mbox::open_tmp_email_parser_db(QString email_parser_db_path)
{
    QFileInfo info_db(email_parser_db_path);
    if(info_db.exists())
    {
        QString connection_naam = Q_FUNC_INFO;
        QSqlDatabase::removeDatabase(connection_naam);
        tmp_email_parser_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        tmp_email_parser_db.setDatabaseName(email_parser_db_path);
        if(!tmp_email_parser_db.open())
        {
            recon_static_functions::app_debug(" email_parser Open  -------FAILED------ " + tmp_email_parser_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(tmp_email_parser_db.lastError().text() , Q_FUNC_INFO);
            return false;
        }

        return true;
    }
}

bool thread_mail_mbox::open_fs_db(QString fs_db)
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

void thread_mail_mbox::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutex_for_fs_main_db = mutex;
}

void thread_mail_mbox::pub_set_mutex_for_mail_mbox_db(QMutex *mutex)
{
    mutex_for_email_parser_db = mutex;
}

void thread_mail_mbox::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_mail_mbox::pub_set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_mail_mbox::mail_data_start_parsing_feature(QString data_str)
{

    if(data_str.size() !=0)
    {
        QByteArray arr = data_str.toLocal8Bit();

        emlx_mbox(&arr);
        emlx_to(&arr);
        emlx_from(&arr);
        emlx_subject(&arr);
        emlx_cc(&arr);
        emlx_date(&arr);
        emlx_message_body(&arr);
        emlx_attachment_feature(&arr);
    }
}

void thread_mail_mbox::emlx_mbox(QByteArray *arr)
{
    qint64 start_index = 0,end_index = 0;
    QString mbox;

    start_index = arr->indexOf("X-Gmail-Labels: ",end_index);

    if(start_index > 0)
    {
        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            mbox = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));

        }

    }

    if(mbox.contains("X-Gmail-Labels: "))
    {
        mbox.remove("X-Gmail-Labels: ");

        if(mbox.contains("Trash"))
        {
            mbox = "Trash";
        }
        else if(mbox.contains("Archived"))
        {
            mbox = "Archived";
        }
        else if(mbox.contains("Spam"))
        {
            mbox = "Spam";
        }
        else if(mbox.contains("Inbox"))
        {
            mbox = "Inbox";
        }
        else if(mbox.contains("Sent"))
        {
            mbox = "Sent";
        }
        else if(mbox.contains("Draft"))
        {
            mbox = "Draft";
        }
        else if(mbox.contains("Chat"))
        {
            mbox = "Orphan";
        }
        else
        {
            QStringList splitted_mbox;
            if(mbox.contains(","))
            {
                splitted_mbox = mbox.split(",");
                mbox = splitted_mbox.at(0);
            }
        }
    }

    mbox_mailbox_str = mbox;
}

void thread_mail_mbox::emlx_to(QByteArray *arr)
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

    mbox_to_str = final_to_str.trimmed();

    recon_static_functions::app_debug("[emlx_to] -End" ,Q_FUNC_INFO);

}

QString thread_mail_mbox::emlx_multiple(QByteArray *arr,qint64 end_index)
{
    recon_static_functions::app_debug("[emlx_multiple] -Start" ,Q_FUNC_INFO);

    qint64 next_index = 0;
    QString multiple;

    end_index ++;
    next_index = arr->indexOf("\n",end_index);
    multiple = QString::fromLocal8Bit(arr->mid(end_index,(next_index-end_index)));
    parser_stop_index = next_index;

    recon_static_functions::app_debug("[emlx_multiple] -End" ,Q_FUNC_INFO);

    return multiple;

}

void thread_mail_mbox::emlx_from(QByteArray *arr)
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
    mbox_from_str = final_from_str;

    recon_static_functions::app_debug("[emlx_from] -End" ,Q_FUNC_INFO);

}

void thread_mail_mbox::emlx_subject(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_subject] -Start" ,Q_FUNC_INFO);

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
    if(subject.startsWith(QString("Subject: ")))
        subject.remove(0, QString("Subject: ").size());

    mbox_subject_str = subject.trimmed();

    recon_static_functions::app_debug("[emlx_subject] -End" ,Q_FUNC_INFO);

}

void thread_mail_mbox::emlx_cc(QByteArray *arr)
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

    mbox_cc_str = final_cc_str.trimmed();

    recon_static_functions::app_debug("[emlx_cc] -End" ,Q_FUNC_INFO);

}

void thread_mail_mbox::emlx_date(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_date] -Start" ,Q_FUNC_INFO);

    qint64 start_index = 0,end_index = 0;
    QString date;

    start_index = arr->indexOf("Date: ",end_index);

    if(start_index > 0)
    {
        end_index = arr->indexOf("\n",start_index);

        if(end_index > start_index)
        {
            date = QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index)));
        }

    }

    if(!date.trimmed().isEmpty())
        date.remove(0, QString("Date: ").size());

    QString time_str_full = date.trimmed();
    QString time_str , offset_str;
    QStringList time_split_list;

    qint64 epoch_timestamp = 0;

    if(time_str_full.contains("+"))
    {
        // Thu, 20 Sep 2018 11:01:27 +0530

        time_split_list = time_str_full.split("+");
        time_str = time_split_list.at(0).trimmed();
        offset_str = time_split_list.at(1).trimmed();

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

        epoch_timestamp -= hour_offset + min_offset;

    }
    else if(time_str_full.contains("-"))
    {
        // Thu, 20 Sep 2018 11:01:27 -0530

        time_split_list = time_str_full.split("-");
        time_str = time_split_list.at(0).trimmed();
        offset_str = time_split_list.at(1).trimmed();

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

        epoch_timestamp += hour_offset + min_offset;
    }
    else
    {
        // Thu, 20 Sep 2018 11:01:27

        time_str = time_str_full.trimmed();

        QString format = "ddd, d MMM yyyy hh:mm:ss";
        QDateTime valid = QDateTime::fromString(time_str, format);
        valid.setTimeSpec(Qt::UTC);
        epoch_timestamp = valid.toSecsSinceEpoch();
    }

    mbox_date_str = QString::number(epoch_timestamp).trimmed();


    recon_static_functions::app_debug("[emlx_date] -End" ,Q_FUNC_INFO);

}

void thread_mail_mbox::emlx_message_body(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_message_body] -Start" ,Q_FUNC_INFO);

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

                        QByteArray tmp_arr;
                        tmp_arr.append(0x0A);
                        tmp_arr.append(0x0D);
                        tmp_arr.append(0x0A);

                        qint64 content_start_index = 0 , content_end_index = 0;

                        content_start_index = arr->indexOf(tmp_arr, end_index);

                        ///check again for encoded text
                        if(!encoded_text)
                            encoded_text = emlx_failsafe_text_encoded_check(content_start_index,arr);

                        if(content_start_index > end_index)
                        {
                            content_start_index += tmp_arr.size();
                            tmp_arr.append("------=");

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
                }
            }
        }
    }
    else
    {
        bool encoded_text_bool = check_text_encoded(arr);
        bool content_type_available = false;

        qint64 tmp_start_index = arr->indexOf("Content-Type: ", end_index);
        if(tmp_start_index > 0)
        {
            content_type_available = true;
            QByteArray tmp_start_arr;
            tmp_start_arr.append(0x0A);
            tmp_start_arr.append(0x0D);
            tmp_start_arr.append(0x0A);

            start_index = arr->indexOf(tmp_start_arr, tmp_start_index);

            if(start_index > tmp_start_index)
            {
                start_index += tmp_start_arr.size();
                end_index = arr->indexOf(tmp_start_arr , start_index);

                if(end_index > start_index)
                {
                    message_body_str = QString::fromLocal8Bit(arr->mid(start_index , (end_index - start_index))).trimmed();

                    if(encoded_text_bool)
                        message_body_str = QString::fromLocal8Bit(QByteArray::fromBase64(arr->mid(start_index , (end_index - start_index))).trimmed());


                    temp_str = message_body_str;
                }
            }
        }


        if(!content_type_available)
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
    }

    mbox_message_str = message_body_str;
    mbox_body_msg_without_text_formatting = temp_str;

    recon_static_functions::app_debug("[emlx_message_body] -End" ,Q_FUNC_INFO);

}

void thread_mail_mbox::emlx_attachment_feature(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_attachment_feature] -Start" ,Q_FUNC_INFO);

    if(!email_attachment_exists(arr))
        mbox_attachment_list << QString("");

    mbox_attachment_list.clear();

    QStringList filelist = email_attachment_filelist(arr);

    for(int i = 0; i < filelist.size(); i++)
    {
        QString filename = filelist.at(i);

        qint64 filename_index = 0,data_start_index = 0,data_end_index=0;

        filename_index = arr->indexOf(filename.toStdString(),0);

        if(filename_index > 0)
        {
            QByteArray tmp_arr;
            tmp_arr.append(0x0D);
            tmp_arr.append(0x0A);
            tmp_arr.append(0x0D);
            tmp_arr.append(0x0A);

            data_start_index = arr->indexOf(tmp_arr,filename_index);

            if(data_start_index > filename_index)
            {
                data_start_index += tmp_arr.size();
                data_end_index = arr->indexOf("==",data_start_index);

                if(data_end_index > data_start_index)
                {
                    QByteArray encoded_arr = arr->mid(data_start_index,(data_end_index-data_start_index));

                    filename.remove("filename=");
                    filename.remove("\"");

                    filename = recon_static_functions::get_available_filename(filename, destination_atchmnt_path_feature, Q_FUNC_INFO);

                    emlx_attachment_output(filename,encoded_arr,destination_atchmnt_path_feature);

                    mbox_attachment_list << filename;
                }
            }
        }
    }

    recon_static_functions::app_debug("[emlx_attachment_feature] -End" ,Q_FUNC_INFO);

}

void thread_mail_mbox::mbox_account_id_from_sentmessage(QByteArray *arr)
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
    from.remove("From: ");

    QStringList tmp_list = from.split("<",Qt::SkipEmptyParts );

    for(int i=0;i<tmp_list.size();i++)
    {
        if(tmp_list.size() > 1)
        {
            QString tmp_account = tmp_list.at(1);
            account_id_str = tmp_account.remove(">");
        }
    }

    mbox_account_id_str = account_id_str;
}

bool thread_mail_mbox::check_for_multipart(QByteArray *arr)
{
    recon_static_functions::app_debug("[check_for_multipart] -Start" ,Q_FUNC_INFO);

    qint64 start_index = 0;

    start_index = arr->indexOf("Content-Type: multipart" , 0);

    if(start_index > 0)
        return true;

    recon_static_functions::app_debug("[check_for_multipart] -End" ,Q_FUNC_INFO);

    return false;
}

bool thread_mail_mbox::emlx_check_text_encoded(QByteArray *arr)
{
    recon_static_functions::app_debug("[emlx_check_text_encoded] -Start" ,Q_FUNC_INFO);

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
            recon_static_functions::app_debug("[emlx_check_text_encoded] -End" ,Q_FUNC_INFO);

            return true;
        }

    }

    recon_static_functions::app_debug("[emlx_check_text_encoded] -End" ,Q_FUNC_INFO);

    return false;
}

bool thread_mail_mbox::emlx_failsafe_text_encoded_check(qint64 text_start_index,QByteArray *arr)
{

    recon_static_functions::app_debug("[emlx_check_text_encoded] -Start" ,Q_FUNC_INFO);

    if(text_start_index <= 0)
    {
        recon_static_functions::app_debug("[emlx_check_text_encoded] -End" ,Q_FUNC_INFO);

        return false;
    }

    qint64 text_end_index = text_start_index - 6;

    QString encode_text_str = QString::fromLocal8Bit(arr->mid(text_end_index ,(text_start_index - text_end_index))).trimmed();

    if(encode_text_str == QString("base64") || encode_text_str == QString("Base64"))
    {
        recon_static_functions::app_debug("[emlx_check_text_encoded] -End" ,Q_FUNC_INFO);
        return true;
    }
    else
    {
        recon_static_functions::app_debug("[emlx_check_text_encoded] -End" ,Q_FUNC_INFO);
        return false;
    }

}

QByteArray thread_mail_mbox::emlx_decode_base64(QByteArray data_arr)
{
    recon_static_functions::app_debug("[emlx_decode_base64] -Start" ,Q_FUNC_INFO);

    if(data_arr.size() == 0)
        return data_arr;

    data_arr =  QByteArray::fromBase64(data_arr);

    recon_static_functions::app_debug("[emlx_decode_base64] -End" ,Q_FUNC_INFO);

    return data_arr;
}

QString thread_mail_mbox::emlx_normalize_characters(QString data_str)
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

bool thread_mail_mbox::email_attachment_exists(QByteArray *arr)
{
    recon_static_functions::app_debug("[email_attachment_exists] -Start" ,Q_FUNC_INFO);

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
            recon_static_functions::app_debug("[email_attachment_exists] -End" ,Q_FUNC_INFO);

            return true;
        }
    }

    recon_static_functions::app_debug("[email_attachment_exists] -End" ,Q_FUNC_INFO);

    return false;
}

QStringList thread_mail_mbox::email_attachment_filelist(QByteArray *arr)
{
    recon_static_functions::app_debug("[email_attachment_filelist] -Start" ,Q_FUNC_INFO);

    QStringList filelist;

    qint64 start_index = 0,end_index = 0;

    while(start_index != -1)
    {
        start_index = arr->indexOf("filename=",end_index);
        if(start_index >= 0)
        {
            end_index = arr->indexOf("\n",start_index);
            filelist << QString::fromLocal8Bit(arr->mid(start_index,(end_index-start_index))).trimmed();
        }
    }

    recon_static_functions::app_debug("[email_attachment_filelist] -End" ,Q_FUNC_INFO);

    return filelist;

}

int thread_mail_mbox::emlx_attachment_output(QString filename,QByteArray arr, QString dest_dir_path)
{
    recon_static_functions::app_debug("[emlx_attachment_output] -Start" ,Q_FUNC_INFO);

    QByteArray data_decode = emlx_decode_base64(arr);
    if(data_decode.size() > 0)
    {
        QString file_path = dest_dir_path + filename;

        QFile myfile(file_path);

        if(!myfile.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("[emlx_attachment_output] File Open Failed:" + myfile.fileName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("[emlx_attachment_output] Error:" + myfile.errorString(),Q_FUNC_INFO);
            return -1;
        }

        myfile.write(data_decode,data_decode.size());

        myfile.flush();
        myfile.close();
    }

    recon_static_functions::app_debug("[emlx_attachment_output] -End" ,Q_FUNC_INFO);

    return 0;
}

bool thread_mail_mbox::check_text_encoded(QByteArray *arr)
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    qint64 start_encode_index = 0;

    start_encode_index = arr->indexOf("Content-Transfer-Encoding: BASE64", 0);

    if(start_encode_index > 0)
    {
        recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

        return true;
    }
    else
    {
        start_encode_index = arr->indexOf("Content-Transfer-Encoding: base64", 0);

        if(start_encode_index > 0)
        {
            recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

            return true;
        }

    }

    recon_static_functions::app_debug("[emlx_check_text_encoded] -End" ,Q_FUNC_INFO);

    return false;
}
