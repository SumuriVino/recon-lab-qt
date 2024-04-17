#include "thread_unified_logs.h"

thread_unified_logs::thread_unified_logs(QObject *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

    prc_log_macos_log_show = new QProcess(this);
    prc_log_macos_log_show->setProcessChannelMode(QProcess::MergedChannels);
    connect(prc_log_macos_log_show, SIGNAL(readyRead()), this, SLOT(slot_apple_macOS_log_show_proc_readyread()));
    connect(prc_log_macos_log_show, SIGNAL(finished(int)), this, SLOT(slot_apple_macOS_log_show_proc_finished()));

    m_timer_log_macos_log_show = new QTimer(this);
    connect(m_timer_log_macos_log_show,SIGNAL(timeout()),this, SLOT(on_slot_timeout_apple_macOS_log_collection_timer()));

}

void thread_unified_logs::slot_extract_unified_logs()
{
    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Unified_Logs);

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    bool_cancel_extraction = false;

    QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    QString index_unified_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";

    QStringList unified_logs_db_path_list;

    for(int mm = 0; mm < list_target_source_info.size(); mm++)
    {

        if(bool_cancel_extraction)
            break;


        target_struct_obj = list_target_source_info.at(mm);

        if(target_struct_obj.os_scheme_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
            continue;

        bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
        if(!bool_src_applicable)
            continue;


        if(recon_case_runner_type != MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            QString src_cnt = target_struct_obj.source_count_name;
            src_cnt = src_cnt.remove(0, QString("Source").size());
            src_cnt.prepend("s");
            QString db_name = src_cnt + "_unified_logs_data.sqlite";
            QString available_file_name = recon_static_functions::get_available_filename(db_name, unified_logs_dir_path, Q_FUNC_INFO);
            QString logs_db_path =  unified_logs_dir_path + "/" + available_file_name;

            unified_logs_db_path_list << logs_db_path;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            extract_log_macos_log_show();
            create_and_insertion_in_logs_db(logs_db_path,available_file_name,index_unified_logs_db_path);

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Unified_Logs, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

        }
        emit signal_add_unified_logs_to_case_tree(target_struct_obj.source_count_name);

    }

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Unified_Logs);
    emit finished();

}

void thread_unified_logs::create_and_insertion_in_logs_db(QString unified_logs_db_path,QString db_name1,QString index_unified_logs_db)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    QString log_tmp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString).toString()
            + target_struct_obj.source_count_name + "/";

    QString unified_logs_archive_file_path = log_tmp_dir_path + "unified_logs.logarchive";

    QString unified_logs_json_file_path = log_tmp_dir_path + "unified_logs.json";
    QString src_file_path = QString("/private/var/db/uuidtext") + QString(MACRO_RECON_Splitter_6_comma) + QString("/private/var/db/diagnostics");

    QString create_cmd = "CREATE TABLE log_data(INT INTEGER PRIMARY KEY, bookmark, notes_icon varchar(300), category VARCHAR(1024),"
                         " timestamp_readable VARCHAR(1024), timestamp_epoch INTEGER,process VARCHAR(1024), process_id VARCHAR(1024), process_path VARCHAR(1024),"
                         " process_image_uuid VARCHAR(1024), event_message VARCHAR(1024), event_type VARCHAR(1024),"
                         " message_type VARCHAR(1024), library VARCHAR(1024), library_path VARCHAR(1024), thread_id VARCHAR(1024),"
                         " activity_id VARCHAR(1024), subsystem VARCHAR(1024), boot_uuid VARCHAR(1024), trace_id VARCHAR(1024),"
                         " source_count_name VARCHAR(100), recon_tag_value VARCHAR(1024), notes VARCHAR(1024), os_scheme_display VARCHAR(1024),"
                         " source_file VARCHAR(1024))";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(unified_logs_db_path);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug("DB open  ---FAILED--- " + dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    dest_db.transaction();

    QSqlQuery create_query(dest_db);

    if(!create_query.exec(create_cmd))
    {
        recon_static_functions::app_debug("query attach   ---FAILED--- " + dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + create_query.lastQuery(),Q_FUNC_INFO);
        dest_db.close();
        return;
    }

    QSqlQuery insert_query(dest_db);

    QFile log_json_file(unified_logs_json_file_path);
    if(!log_json_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ----FAILED---" + log_json_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + log_json_file.errorString(),Q_FUNC_INFO);
        return;
    }

    qint64 m_counter = 0;

    QByteArray raw_block_byte_arr;
    QString readable_timestamp;
    QString epoch_timestamp;
    QString process, process_path;
    QStringList process_path_list;
    QString library, library_path;
    QStringList library_path_list;
    QString event_message;
    QString pid;
    QString category;
    QString subsystem;
    QString activity_id;
    QString event_type;
    QString message_type;
    QString thread_id;
    QString trace_id;
    QString boot_uuid;
    QString process_image_uuid;
    qint64 total_count = 0;

    while(!log_json_file.atEnd())
    {
        QString line = log_json_file.readLine();

        if(bool_cancel_extraction)
            break;

        if(line.contains("},{") || log_json_file.atEnd())
        {

            if(!log_json_file.atEnd())
                raw_block_byte_arr.append("}]");

            QJsonArray json_block = QJsonDocument::fromJson(raw_block_byte_arr).array();

            QJsonObject json_key_value_obj;
            foreach (const QJsonValue &obj_values_key, json_block)
            {
                json_key_value_obj = obj_values_key.toObject();

                if(m_counter % 2000 == 0)
                {
                    m_counter = 0;
                    QCoreApplication::processEvents();
                }
            }

            readable_timestamp  = json_key_value_obj["timestamp"].toString().trimmed();

            epoch_timestamp = global_recon_helper_singular_class_obj->convert_timestamp_readable_to_numeric_7(readable_timestamp);

            //Process and Process Path
            process_path  = json_key_value_obj["processImagePath"].toString().trimmed();
            process_path_list = process_path.split("/");

            if(!process_path_list.isEmpty())
            {
                process = process_path_list.last();
            }

            //Library and Library Path
            library_path  = json_key_value_obj["senderImagePath"].toString().trimmed();

            library_path_list = library_path.split("/");

            if(!library_path_list.isEmpty())
            {
                library = library_path_list.last();
            }

            //Parse here for ids
            event_message  = json_key_value_obj["eventMessage"].toString().trimmed();
            pid  = QString::number(json_key_value_obj.value("processID").toDouble());
            category  = json_key_value_obj["category"].toString().trimmed();
            subsystem  = json_key_value_obj["subsystem"].toString().trimmed();
            activity_id  = QString::number(json_key_value_obj.value("activityIdentifier").toDouble());
            event_type  = json_key_value_obj["eventType"].toString().trimmed();
            message_type  = json_key_value_obj["messageType"].toString().trimmed();
            thread_id  = QString::number(json_key_value_obj.value("threadID").toDouble());
            trace_id  = QString::number(json_key_value_obj.value("traceID").toDouble());
            boot_uuid  = json_key_value_obj["bootUUID"].toString().trimmed();
            process_image_uuid  = json_key_value_obj["processImageUUID"].toString().trimmed();

            QString insert_cmd = "INSERT INTO log_data(bookmark,notes_icon,category,timestamp_readable,timestamp_epoch,process,process_id,process_path,"
                                 "process_image_uuid,event_message,event_type,message_type,library,library_path,thread_id,"
                                 "activity_id,subsystem,boot_uuid,trace_id,source_count_name,recon_tag_value,notes,os_scheme_display,"
                                 "source_file) values(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)";

            insert_query.prepare(insert_cmd);

            insert_query.addBindValue(0);
            insert_query.addBindValue(0);
            insert_query.addBindValue(category);
            insert_query.addBindValue(readable_timestamp);
            insert_query.addBindValue(epoch_timestamp);
            insert_query.addBindValue(process);
            insert_query.addBindValue(pid);
            insert_query.addBindValue(process_path);
            insert_query.addBindValue(process_image_uuid);
            insert_query.addBindValue(event_message);
            insert_query.addBindValue(event_type);
            insert_query.addBindValue(message_type);

            insert_query.addBindValue(library);
            insert_query.addBindValue(library_path);
            insert_query.addBindValue(thread_id);
            insert_query.addBindValue(activity_id);
            insert_query.addBindValue(subsystem);
            insert_query.addBindValue(boot_uuid);
            insert_query.addBindValue(trace_id);
            insert_query.addBindValue(target_struct_obj.source_count_name);
            insert_query.addBindValue("");

            insert_query.addBindValue("");
            insert_query.addBindValue(target_struct_obj.os_scheme_display);
            insert_query.addBindValue(src_file_path);

            total_count++;
            if(total_count % 500 == 0)
                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Unified_Logs,("Extracting Unified Logs - " + target_struct_obj.source_name),false,total_count,0,true);

            if(!insert_query.exec())
            {
                recon_static_functions::app_debug(" query select ------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" query: " + insert_query.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" query Error " + insert_query.lastError().text(),Q_FUNC_INFO);
                dest_db.close();
                return;
            }

            raw_block_byte_arr.clear();
            raw_block_byte_arr.append("[{");
            line.clear();

            readable_timestamp.clear();
            epoch_timestamp.clear();
            process.clear(), process_path.clear();
            process_path_list.clear();
            library.clear(), library_path.clear();
            library_path_list.clear();
            event_message.clear();
            pid.clear();
            category.clear();
            subsystem.clear();
            activity_id.clear();
            event_type.clear();
            message_type.clear();
            thread_id.clear();
            trace_id.clear();
            boot_uuid.clear();
            process_image_uuid.clear();
        }

        raw_block_byte_arr.append(line.toStdString());
    }

    log_json_file.close();

    QString command = "Insert into tbl_index (db_name,source_count_name) values(?,?)";
    QStringList values;
    values << db_name1 << target_struct_obj.source_count_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,values,index_unified_logs_db,Q_FUNC_INFO);

    dest_db.commit();
    dest_db.close();

    QFile::remove(unified_logs_json_file_path);
    QFile::remove(unified_logs_archive_file_path);

    recon_static_functions::app_debug("End" , Q_FUNC_INFO);

}

void thread_unified_logs::extract_log_macos_log_show()
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    log_macos_log_json_file_path.clear();

    if(target_struct_obj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
    {
        QString logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString).toString()
                + target_struct_obj.source_count_name + "/";
        QDir dir;
        dir.mkpath(logs_dir_path);

        QString log_file_path = logs_dir_path + "/unified_logs.json";

        QFileInfo file(log_file_path);
        if(file.exists())
        {
            log_macos_log_json_file_path = log_file_path;
        }
        else
        {
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Unified_Logs,target_struct_obj.source_name,false,-1,0,true);

            QString log_archive_path = logs_dir_path + "unified_logs.logarchive";
            QString command = "cp";

            bool bool_any_log_src_exist = false;
            QString diagnostic_source_dir_path = recon_static_functions::get_complete_file_path(target_struct_obj.virtual_source_path, QString(MACRO_LOG_macOS_SOURCE_DIR_PATH_Diagnostics), Q_FUNC_INFO);
            if(QDir(diagnostic_source_dir_path).exists())
            {
                bool_any_log_src_exist = true;
                QStringList diagnostic_arguments;
                diagnostic_arguments  << "-r" << "-f" << diagnostic_source_dir_path << log_archive_path;

                recon_helper_process_obj->run_command_with_arguments(command, diagnostic_arguments ,Q_FUNC_INFO);
            }

            QString uuid_source_dir_path = recon_static_functions::get_complete_file_path(target_struct_obj.fs_extraction_source_path, QString(MACRO_LOG_macOS_SOURCE_DIR_PATH_UUID), Q_FUNC_INFO);
            if(QDir(uuid_source_dir_path).exists())
            {
                bool_any_log_src_exist = true;
                QStringList uuid_arguments;
                uuid_arguments  << "-r" << "-f" << uuid_source_dir_path << log_archive_path;
                recon_helper_process_obj->run_command_with_arguments(command, uuid_arguments ,Q_FUNC_INFO);
            }

            struct_global_desktop_info struct_mchne_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
            QString m_version = struct_mchne_info_obj.os_version_full;

            QString info_plist_file_path = QString("../Resources/log_archive/Info.plist");
            if(QFile(info_plist_file_path).exists())
            {
                QStringList info_plist_arguments;
                info_plist_arguments << info_plist_file_path << log_archive_path;

                recon_helper_process_obj->run_command_with_arguments(command, info_plist_arguments ,Q_FUNC_INFO);
            }

            if(bool_any_log_src_exist)
            {
                log_macos_log_json_file_path = log_file_path;
                write_apple_macOS_log_archive_data_into_json_file(log_archive_path ,log_macos_log_json_file_path);
                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Unified_Logs,target_struct_obj.source_name,false,-1,0,true);
            }
        }
    }

    recon_static_functions::app_debug("End" , Q_FUNC_INFO);

}

void thread_unified_logs::write_apple_macOS_log_archive_data_into_json_file(QString logarchive_file_path , QString file_path)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    log_macos_log_show_qfile.setFileName(file_path);

    if(!log_macos_log_show_qfile.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" file open ----FAILED---- " + log_macos_log_show_qfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error - " + log_macos_log_show_qfile.errorString(),Q_FUNC_INFO);
        return;
    }

    QStringList a_tmp_args;
    a_tmp_args << "-cr" << logarchive_file_path;

    QProcess::execute("xattr", a_tmp_args);

    QStringList arguments;
    arguments << "show" << QString("--archive") << logarchive_file_path << "--force" << "--style" << "json";
    QString command;
    command = "log" ;

    bool_log_macos_process_finished = false;
    bool_log_macos_log_show_error_status = false;

    prc_log_macos_log_show->start(command , arguments);
    m_timer_log_macos_log_show->setInterval(1000);
    m_timer_log_macos_log_show->start();

    if(!prc_log_macos_log_show->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start_1",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + prc_log_macos_log_show->errorString() ,Q_FUNC_INFO);
        log_macos_log_show_qfile.close();
        return;
    }


    int m_cnt = 0;

    while(1)
    {
        m_cnt++;
        if(m_cnt % 50 == 0 )
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }

        if(bool_cancel_extraction)
        {
            if(prc_log_macos_log_show->state() == QProcess::Running)
                prc_log_macos_log_show->terminate();
        }

        if(bool_log_macos_process_finished || prc_log_macos_log_show->state() != QProcess::Running)
        {
            break;
        }
    }

    ///Start Process again Because Sometimes the logarchive file not open or missing metadata error occured.

    if(bool_log_macos_log_show_error_status)
    {
        bool_log_macos_process_finished = false;
        prc_log_macos_log_show->start(command , arguments);

        bool_log_macos_log_show_error_status = false;

        m_timer_log_macos_log_show->setInterval(1000);
        m_timer_log_macos_log_show->start();

        if(!prc_log_macos_log_show->waitForStarted())
        {
            recon_static_functions::app_debug("----FAILED---- Process Start_2",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + prc_log_macos_log_show->errorString() ,Q_FUNC_INFO);
            log_macos_log_show_qfile.close();
            return;
        }

        while(1)
        {
            m_cnt++;
            if(m_cnt % 50 == 0)
            {
                m_cnt = 0;
                QCoreApplication::processEvents();
            }

            if(bool_cancel_extraction)
            {
                if(prc_log_macos_log_show->state() == QProcess::Running)
                    prc_log_macos_log_show->terminate();
            }

            if(bool_log_macos_process_finished || prc_log_macos_log_show->state() != QProcess::Running)
            {
                break;
            }
        }
    }

    log_macos_log_show_qfile.flush();
    log_macos_log_show_qfile.close();

    recon_static_functions::app_debug("End" , Q_FUNC_INFO);
}

void thread_unified_logs::slot_apple_macOS_log_show_proc_readyread()
{

    QByteArray apple_macOS_log_show_data = prc_log_macos_log_show->readAll();

    if(apple_macOS_log_show_data.contains("Could not open log archive") || apple_macOS_log_show_data.contains("missing metadata"))
    {
        bool_log_macos_log_show_error_status = true;
        return;
    }

    if(!apple_macOS_log_show_data.isEmpty())
    {
        log_macos_log_show_qfile.write(apple_macOS_log_show_data);
    }

}

void thread_unified_logs::on_slot_timeout_apple_macOS_log_collection_timer()
{
    QFileInfo file_info(log_macos_log_json_file_path);
    qint64 file_size_qint = file_info.size();
    if(file_size_qint != 0)
    {
        QString file_size = recon_static_functions::human_readable_size(file_size_qint,Q_FUNC_INFO);
        if(file_size.toInt() > 0)
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Unified_Logs,(target_struct_obj.source_name + " - Collecting Unified Logs... " + QString::number(file_size.toDouble()) + "GB"),false,-1,0,true);
        else
            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Unified_Logs,(target_struct_obj.source_name + " - Collecting Unified Logs... "),false,-1,0,true);

    }
}

void thread_unified_logs::slot_apple_macOS_log_show_proc_finished()
{
    m_timer_log_macos_log_show->stop();
    bool_log_macos_process_finished = true;
}

void thread_unified_logs::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutex_for_fs_main_db = mutex;
}

void thread_unified_logs::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_unified_logs::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_unified_logs::pub_set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}
