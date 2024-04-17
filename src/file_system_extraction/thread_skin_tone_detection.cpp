#include "thread_skin_tone_detection.h"

thread_skin_tone_detection::thread_skin_tone_detection(QObject *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);
}

void thread_skin_tone_detection::slot_extract_skin_tone_detection()
{
    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Skin_Tone_Detection);

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    bool_cancel_extraction = false;

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        extract_skin_tone_detection_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Skin_Tone_Detection);
        emit finished();
        return;
    }

    QStringList skin_tone_db_path_list;

    QString run_by_right_click = "FALSE"; //Do Not Use Macro Here.

    for(int mm = 0; mm < list_target_source_info.size(); mm++)
    {

        if(bool_cancel_extraction)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(mm);

        QString skin_tone_detection_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/skin_tone_detection.sqlite");

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            if(bool_cancel_extraction)
                break;

            skin_tone_db_path_list.append(skin_tone_detection_db_path);
        }
        else
        {
            skin_tone_db_path_list.append(skin_tone_detection_db_path);

            source_name = target_struct_obj.source_name;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
                continue;

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");

            QString tmp_fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString).toString();
            tmp_fs_dir_path.append(target_struct_obj.source_count_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch()));

            QString video_thumbs_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch()) + "video_thumb/";

            QDir thumbs_path_dir;
            thumbs_path_dir.mkpath(video_thumbs_path);

            QDir tmp_dir;
            tmp_dir.mkpath(tmp_fs_dir_path);

            QString tmp_fs_db_path = tmp_fs_dir_path + "/tmp_file_system.sqlite";

            filesystem_db_path = fs_db_path;

            QString os_scheme_internal = target_struct_obj.os_scheme_internal;

            QStringList cp_args;
            cp_args << fs_db_path << tmp_fs_db_path;


            QProcess proc_cp_fs_db;

            proc_cp_fs_db.start("cp", cp_args);

            proc_cp_fs_db.waitForFinished(-1);

            proc_cp_fs_db.close();

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Skin_Tone_Detection, target_struct_obj.source_name, true, 0, 0, false);

            QString virtual_source_path = target_struct_obj.virtual_source_path;


            qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Skin_Tone_Detection_Minimum_File_Size_Bytes_qint64).toLongLong();
            QStringList python_args;

            //Syntax: script_path source_count_name os_scheme_internal virtual_source_path tmp_fs_db_path skin_tone_detection_db_path
            // video_thumbs_path pref_min_file_size_str run_by_right_click display_filepath

            //            python_args << "../Resources/scripts_persistent/skin_tone_detection.py" << target_struct_obj.source_count_name
            //                        << os_scheme_internal << virtual_source_path << tmp_fs_db_path  << skin_tone_detection_db_path
            //                        << video_thumbs_path << QString::number(pref_min_file_size) << run_by_right_click << "";

            python_args << target_struct_obj.source_count_name
                        << os_scheme_internal << virtual_source_path << tmp_fs_db_path  << skin_tone_detection_db_path
                        << video_thumbs_path << QString::number(pref_min_file_size) << run_by_right_click << "";

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) from files Where fs_module_skin_tone_detection_run_status='0'") , 0, tmp_fs_db_path, Q_FUNC_INFO);
            total_records_num = 0;
            total_records_num = total_records_str.toLongLong();

            process_skin_tone_detection = new QProcess(this);

            //            QString python_exe_path = global_varibale_python_path;
            QString skin_tone_detection_exe_path =  "../ZAlreadySigned/pysafe/intelligence_army/skin_tone_detection";

            rec_count = 0;

            process_skin_tone_detection->setProcessChannelMode(QProcess::MergedChannels);
            connect(process_skin_tone_detection, SIGNAL(readyRead()), this, SLOT(slot_process_skin_tone_detection_readyread()));

            //            process_skin_tone_detection->start(python_exe_path,python_args);
            process_skin_tone_detection->start(skin_tone_detection_exe_path,python_args);

            if (!process_skin_tone_detection->waitForStarted())
            {
                recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
                recon_static_functions::app_debug("Error:" + process_skin_tone_detection->errorString() ,Q_FUNC_INFO);
                continue;
            }

            bool_skin_tone_extraction_process_finished = false;
            connect(process_skin_tone_detection, SIGNAL(finished(int)), this, SLOT(slot_process_skin_tone_detection_finished(int)));

            int count = 0;
            while(1)
            {
                count++;
                if(count % 100 == 0)
                {
                    QCoreApplication::processEvents();
                    count = 0;
                }

                if(bool_skin_tone_extraction_process_finished || process_skin_tone_detection->state() == QProcess::NotRunning)
                    break;

                if(bool_cancel_extraction)
                {
                    break;
                }
            }


            //**************Update Tag_DB and FS_DB**************** Start

            QString cmd_select_int = "SELECT filesystem_record FROM files";
            QStringList skin_tone_rec_int_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_select_int,0,skin_tone_detection_db_path,Q_FUNC_INFO);

            QString tag_name = MACRO_Tag_Name_Skin_Tone;
            QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name, Q_FUNC_INFO);

            QString connection_naam_7 = Q_FUNC_INFO + QString("_7");
            QSqlDatabase::removeDatabase(connection_naam_7);
            QSqlDatabase  tmp_fs_db;
            tmp_fs_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_7);
            tmp_fs_db.setDatabaseName(tmp_fs_db_path);

            if(!tmp_fs_db.open())
            {
                recon_static_functions::app_debug("Database open ----FAILED---- " + tmp_fs_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + tmp_fs_db.lastError().text() , Q_FUNC_INFO);
                tmp_fs_db.close();
            }

            QSqlQuery query_select_fs_details(tmp_fs_db);
            if(!skin_tone_rec_int_list.isEmpty())
            {
                mutext_for_fs_main_db->lock();

                for(int ii = 0 ; ii < skin_tone_rec_int_list.size() ; ii++)
                {
                    QString int_rec   = skin_tone_rec_int_list.at(ii);

                    QStringList values;
                    values << tag_name << int_rec;
                    QString cmd_update_fs = "UPDATE files SET recon_tag_value = ? WHERE INT = ?";
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_update_fs, values,fs_db_path,Q_FUNC_INFO);

                    QString cmd_select = "SELECT INT, filename, filepath, source_count_name from files WHERE INT = ?" ;
                    query_select_fs_details.prepare(cmd_select);
                    query_select_fs_details.addBindValue(int_rec);
                    if(!query_select_fs_details.exec())
                    {
                        recon_static_functions::app_debug("select query ----FAILED---- " + query_select_fs_details.executedQuery() , Q_FUNC_INFO);
                        recon_static_functions::app_debug("Error " + query_select_fs_details.lastError().text() , Q_FUNC_INFO);
                        tmp_fs_db.close();
                        continue;
                    }

                    QString record_no;
                    if(query_select_fs_details.next())
                    {
                        record_no = query_select_fs_details.value(0).toString().trimmed();
                        QString filename = query_select_fs_details.value(1).toString().trimmed();
                        QString filepath = query_select_fs_details.value(2).toString().trimmed();
                        QString s_count_name = query_select_fs_details.value(3).toString().trimmed();

                        QString command_insert_tag = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                                                     "source_count_name, item0, item1, csv_plugin_name, csv_tab_name) VALUES (?,?,?,?,?,? ,?,?,?,?,?)";

                        QStringList arg_list;
                        arg_list << QString("File System") << QString("File System") << QString("Files")
                                 << "files" << record_no << tag_name << s_count_name<< filename << filepath << "File System" << "Files" ;

                        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_tag,arg_list,tag_db_path,Q_FUNC_INFO))
                        {
                            recon_static_functions::app_debug(" insert table tags from skin tone detection ---FAILED---" + tag_db_path,Q_FUNC_INFO);
                        }
                    }
                }
                mutext_for_fs_main_db->unlock();
            }

            //*************Update Tag_DB and FS_DB************ End

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Skin_Tone_Detection, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);
        }
    }

    emit signal_add_skin_tone_detection_to_case_tree(skin_tone_db_path_list);

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Skin_Tone_Detection);
    emit finished();
}

void thread_skin_tone_detection::slot_process_skin_tone_detection_finished(int status)
{
    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE)
    {
        if(cmd_update_fs_run_status.endsWith("OR "))
            cmd_update_fs_run_status.chop(QString("OR ").size());

        mutext_for_fs_main_db->lock();
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_update_fs_run_status, filesystem_db_path, Q_FUNC_INFO);
        mutext_for_fs_main_db->unlock();
    }

    bool_skin_tone_extraction_process_finished = true;
    process_skin_tone_detection->close();
}

void thread_skin_tone_detection::slot_process_skin_tone_detection_readyread()
{
    QByteArray arr_proc_out = process_skin_tone_detection->readAll();

    QString str_proc_out = QString::fromLocal8Bit(arr_proc_out);

    QStringList list = str_proc_out.split("\n");

    QStringList rec_cnt_list ;


    for(int ii = 0; ii < list.size(); ii++)
    {

        if(bool_cancel_extraction)
        {
            mutext_for_fs_main_db->unlock();
            return;
        }

        QString str_line_data = list.at(ii);

        if(str_line_data.startsWith("INT"))
        {
            processed_file_count_qint64++;
            remaining_file_count_for_req_time_qint64--;

            if(estimated_time.isEmpty() || estimated_time == "00:00:00")
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Skin_Tone_Detection, "Calculating...");
            else
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Skin_Tone_Detection, estimated_time);

            QString tmp_line = str_line_data;

            QString cnt_str = tmp_line.remove("INT ");
            rec_cnt_list << cnt_str;

            QString last_rec_int_value = rec_cnt_list.last();
            qint64 last_rec_int = last_rec_int_value.toInt();

            rec_count++;

            if(rec_count % 20 == 0 || last_rec_int == total_records_num)
            {
                qint64 progress_val = ((rec_count * 100) / total_records_num);

                if(progress_val > 99)
                    progress_val = 99;

                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Skin_Tone_Detection, source_name, true, rec_count, progress_val, false);
            }

            if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE)
            {
                cmd_update_fs_run_status.append("INT ="+ cnt_str + " OR ");
            }
            else
            {
                if(rec_count % 100 == 0 || last_rec_int == total_records_num)
                {
                    mutext_for_fs_main_db->lock();

                    QString cmd = "UPDATE files SET fs_module_skin_tone_detection_run_status = '1' WHERE INT <= ?";
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, QStringList(last_rec_int_value),filesystem_db_path,Q_FUNC_INFO);
                    rec_cnt_list.clear();

                    mutext_for_fs_main_db->unlock();
                }
            }
        }
    }
}

bool thread_skin_tone_detection::open_fs_db(QString fs_db_path)
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

void thread_skin_tone_detection::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_skin_tone_detection::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_skin_tone_detection::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_skin_tone_detection::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}


void thread_skin_tone_detection::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_skin_tone_detection::extract_skin_tone_detection_for_fs_run_module()
{
    cmd_update_fs_run_status = "Update files set fs_module_skin_tone_detection_run_status = '1' WHERE ";
    bool_cancel_extraction = false;

    QString run_by_right_click = "TRUE"; //Do Not Macro Here.

    for(int ii = 0; ii < run_fs_modules_details_list.size(); ii++)
    {
        if(bool_cancel_extraction)
            break;

        struct_global_run_fs_modules_details details_obj = run_fs_modules_details_list.at(ii);

        QString sou_cnt_name = details_obj.source_count_name;
        QString display_file_path = details_obj.display_filepath;

        struct_GLOBAL_witness_info_source source_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(sou_cnt_name);

        QString virtual_source_path = source_obj.virtual_source_path;
        QString os_scheme_internal = source_obj.os_scheme_internal;

        QString skin_tone_detection_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + sou_cnt_name + QString("/skin_tone_detection.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + sou_cnt_name + QString("/file_system.sqlite");

        QString tmp_fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString).toString();
        tmp_fs_dir_path.append(sou_cnt_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch()));

        QString video_thumbs_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString).toString()
                + sou_cnt_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch()) + "video_thumb/";

        QDir thumbs_path_dir;
        thumbs_path_dir.mkpath(video_thumbs_path);

        QDir tmp_dir;
        tmp_dir.mkpath(tmp_fs_dir_path);

        QString tmp_fs_db_path = tmp_fs_dir_path + "/tmp_file_system.sqlite";

        QStringList cp_args;
        cp_args << fs_db_path << tmp_fs_db_path;

        QProcess proc_cp_fs_db;

        proc_cp_fs_db.start("cp", cp_args);

        proc_cp_fs_db.waitForFinished(-1);

        proc_cp_fs_db.close();

        filesystem_db_path = fs_db_path;

        source_name = source_obj.source_name;

        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Skin_Tone_Detection, source_obj.source_name, true, 0, 0, false);

        qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Skin_Tone_Detection_Minimum_File_Size_Bytes_qint64).toLongLong();

        // script_path source_count_name os_scheme_internal virtual_source_path tmp_fs_db_path skin_tone_detection_db_path
        // video_thumbs_path pref_min_file_size_str run_by_right_click display_file_path

        QStringList python_args;

        //        python_args << "../Resources/scripts_persistent/skin_tone_detection.py" << sou_cnt_name
        //                    << os_scheme_internal << virtual_source_path << tmp_fs_db_path  << skin_tone_detection_db_path
        //                    << video_thumbs_path << QString::number(pref_min_file_size) << run_by_right_click
        //                    << display_file_path;

        python_args << sou_cnt_name
                    << os_scheme_internal << virtual_source_path << tmp_fs_db_path  << skin_tone_detection_db_path
                    << video_thumbs_path << QString::number(pref_min_file_size) << run_by_right_click
                    << display_file_path;

        QString command = "SELECT is_dir from files WHERE filepath = ?";

        QString is_dir_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(display_file_path), 0, tmp_fs_db_path, Q_FUNC_INFO);

        QString cmd_select_count;
        if(is_dir_str == "0")
        {
            cmd_select_count = "Select count(*) from files Where fs_module_skin_tone_detection_run_status = '0' AND filepath = '" + display_file_path + "'";
        }
        else
        {
            if(!display_file_path.endsWith("/"))
                display_file_path.append("/");

            cmd_select_count = "Select count(*) from files Where fs_module_skin_tone_detection_run_status = '0' AND filepath LIKE '" + display_file_path + "%'";

        }

        QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_select_count , 0, tmp_fs_db_path, Q_FUNC_INFO);

        total_records_num = 0;
        total_records_num = total_records_str.toLongLong();

        remaining_file_count_for_req_time_qint64 = total_records_num;

        QTimer *timer_obj = new QTimer(this);
        connect(timer_obj,SIGNAL(timeout()),this,SLOT(slot_timeout_to_update_estimated_time_for_fs_modules()));
        timer_obj->start(5000);

        rec_count = 0;

        process_skin_tone_detection = new QProcess(this);

        //        QString python_exe_path = global_varibale_python_path;
        QString skin_tone_detection_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/skin_tone_detection";

        process_skin_tone_detection->setProcessChannelMode(QProcess::MergedChannels);
        connect(process_skin_tone_detection, SIGNAL(readyRead()), this, SLOT(slot_process_skin_tone_detection_readyread()));

        //        process_skin_tone_detection->start(python_exe_path,python_args);
        process_skin_tone_detection->start(skin_tone_detection_exe_path,python_args);

        if (!process_skin_tone_detection->waitForStarted())
        {
            recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + process_skin_tone_detection->errorString() ,Q_FUNC_INFO);
            continue;
        }

        bool_skin_tone_extraction_process_finished = false;
        connect(process_skin_tone_detection, SIGNAL(finished(int)), this, SLOT(slot_process_skin_tone_detection_finished(int)));

        int count = 0;
        while(1)
        {
            count++;
            if(count % 100 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(bool_skin_tone_extraction_process_finished || process_skin_tone_detection->state() == QProcess::NotRunning)
                break;

            if(bool_cancel_extraction)
            {
                break;
            }
        }

        timer_obj->stop();

        //**************Update Tag_DB and FS_DB**************** Start

        QString cmd_select_int = "SELECT filesystem_record FROM files";
        QStringList skin_tone_rec_int_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_select_int,0,skin_tone_detection_db_path,Q_FUNC_INFO);

        QString tag_name = MACRO_Tag_Name_Skin_Tone;
        QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name, Q_FUNC_INFO);

        QString connection_naam_7 = Q_FUNC_INFO + QString("_7");
        QSqlDatabase::removeDatabase(connection_naam_7);
        QSqlDatabase  tmp_fs_db;
        tmp_fs_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_7);
        tmp_fs_db.setDatabaseName(tmp_fs_db_path);

        if(!tmp_fs_db.open())
        {
            recon_static_functions::app_debug("Database open ----FAILED---- " + tmp_fs_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + tmp_fs_db.lastError().text() , Q_FUNC_INFO);
            tmp_fs_db.close();
        }

        QSqlQuery query_select_fs_details(tmp_fs_db);
        if(!skin_tone_rec_int_list.isEmpty())
        {
            mutext_for_fs_main_db->lock();

            for(int ii = 0 ; ii < skin_tone_rec_int_list.size() ; ii++)
            {
                QString int_rec   = skin_tone_rec_int_list.at(ii);

                QStringList values;
                values << tag_name << int_rec;
                QString cmd_update_fs = "UPDATE files SET recon_tag_value = ? WHERE INT = ?";
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_update_fs, values,fs_db_path,Q_FUNC_INFO);

                QString cmd_select = "SELECT INT, filename, filepath, source_count_name from files WHERE INT = ?" ;
                query_select_fs_details.prepare(cmd_select);
                query_select_fs_details.addBindValue(int_rec);
                if(!query_select_fs_details.exec())
                {
                    recon_static_functions::app_debug("select query ----FAILED---- " + query_select_fs_details.executedQuery() , Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error " + query_select_fs_details.lastError().text() , Q_FUNC_INFO);
                    tmp_fs_db.close();
                    continue;
                }

                QString record_no;
                if(query_select_fs_details.next())
                {
                    record_no = query_select_fs_details.value(0).toString().trimmed();
                    QString filename = query_select_fs_details.value(1).toString().trimmed();
                    QString filepath = query_select_fs_details.value(2).toString().trimmed();
                    QString s_count_name = query_select_fs_details.value(3).toString().trimmed();

                    QString cmd_slct_tag_recrd = "select count(*) from tags where record_no = ? and source_count_name = ?";
                    qint64 exist_rec_cnt = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_slct_tag_recrd,QStringList() << record_no << s_count_name, 0,tag_db_path,Q_FUNC_INFO).toInt();

                    if(exist_rec_cnt > 0)
                    {
                        continue;
                    }

                    QString command_insert_tag = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                                                 "source_count_name, item0, item1, csv_plugin_name, csv_tab_name) VALUES (?,?,?,?,?,? ,?,?,?,?,?)";

                    QStringList arg_list;
                    arg_list << QString("File System") << QString("File System") << QString("Files")
                             << "files" << record_no << tag_name << s_count_name<< filename << filepath << "File System" << "Files" ;

                    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_tag,arg_list,tag_db_path,Q_FUNC_INFO))
                    {
                        recon_static_functions::app_debug(" insert table tags from skin tone detection ---FAILED---" + tag_db_path,Q_FUNC_INFO);
                    }
                }
            }
            mutext_for_fs_main_db->unlock();
        }

        //*************Update Tag_DB and FS_DB************ End
    }

    QStringList skin_tone_db_path_list;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString skin_tone_db_path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/skin_tone_detection.sqlite");

        skin_tone_db_path_list << skin_tone_db_path_1;
    }

    emit signal_add_skin_tone_detection_to_case_tree(skin_tone_db_path_list);

}

void thread_skin_tone_detection::slot_timeout_to_update_estimated_time_for_fs_modules()
{
    double avg_image_per_sec = processed_file_count_qint64 / 5;

    qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;
    double required_time = new_ramining_file_count / avg_image_per_sec;

    estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");

    processed_file_count_qint64 = 0;
}
