#include "thread_optical_character_recognition.h"

thread_optical_character_recognition::thread_optical_character_recognition(QObject *parent) : QObject(parent)
{
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard;

}

thread_optical_character_recognition::~thread_optical_character_recognition()
{

}

void thread_optical_character_recognition::slot_extract_optical_character_recognition()
{ // This the feature where we extract OCR data. We use ocr_text_extractor binary for OCR data Extraction. We have seperate c++ code for ocr_text_extractor
    // we run this ocr_text_extractor using QProcess with some arguments

    list_target_witness_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Optical_Character_Recognition);

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        extract_optical_character_recognition_for_fs_run_module();

        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Optical_Character_Recognition);
        emit finished();
        return;
    }

    bool_cancel_extraction = false;

    QStringList ocr_db_path_list;

    QString run_by_right_click = "FALSE"; //Do Not Use Macro Here.

    for(int count = 0; count < list_target_witness_info.size(); count++)
    {
        if(bool_cancel_extraction)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_witness_info.at(count);

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");

        QString ocr_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/optical_character_recognition.sqlite");

        file_system_db_path = filesystem_db_path;

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            if(bool_cancel_extraction)
                break;

            ocr_db_path_list << ocr_db_path;

        }
        else
        {
            ocr_db_path_list << ocr_db_path;

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
            {
                continue;
            }

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            QString tmp_fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Optical_Character_Recognition_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch());

            QDir tmp_dir;
            tmp_dir.mkpath(tmp_fs_dir_path);

            QString tmp_fs_db_path = tmp_fs_dir_path + "/tmp_fs_copy_optical_character_recognition.sqlite";

            QStringList cp_args;
            cp_args << filesystem_db_path << tmp_fs_db_path;

            source_name = target_struct_obj.source_name;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Optical_Character_Recognition, target_struct_obj.source_name, true, 0, 0, false);

            QProcess proc_cp_fs_db;

            proc_cp_fs_db.start("cp", cp_args);

            proc_cp_fs_db.waitForFinished(-1);

            proc_cp_fs_db.close();

            QString virtual_source_path = target_struct_obj.virtual_source_path;

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) from files Where fs_module_optical_character_recognition_run_status = '0'") , 0, tmp_fs_db_path, Q_FUNC_INFO);
            total_records_num = 0;
            total_records_num = total_records_str.toLongLong();


            rec_count = 0;

            process_run_optical_character_recognition = new QProcess(this);
            process_run_optical_character_recognition->setProcessChannelMode(QProcess::MergedChannels);
            connect(process_run_optical_character_recognition, SIGNAL(readyRead()), this, SLOT(slot_process_optical_character_recognition_readyread()));

            qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Optical_Character_Recognition_Minimum_File_Size_Bytes_qint64).toLongLong();

            QString cmd = "./ocr_text_extractor";
            QStringList proc_args;

            //Syntax: source_count_name os_scheme_internal virtual_source_path tmp_fs_db_path ocr_db_path pref_min_file_size_str
            //run_by_right_click display_file_path

            proc_args << target_struct_obj.source_count_name << target_struct_obj.os_scheme_internal << virtual_source_path << tmp_fs_db_path << ocr_db_path << QString::number(pref_min_file_size) << run_by_right_click << "";

            process_run_optical_character_recognition->start(cmd,proc_args);
            process_run_optical_character_recognition->waitForStarted();

            bool_optical_character_recognition_process_finished = false;
            connect(process_run_optical_character_recognition, SIGNAL(finished(int)), this, SLOT(slot_process_optical_character_recognition_finished(int)));

            int countt = 0;
            while(1)
            {
                countt++;
                if(countt % 50 == 0)
                {
                    countt = 0;
                    QCoreApplication::processEvents();
                }

                if(bool_optical_character_recognition_process_finished || process_run_optical_character_recognition->state() == QProcess::NotRunning)
                    break;

                if(bool_cancel_extraction)
                {
                    break;
                }
            }

            copy_data_from_ocr_to_fs_db(target_struct_obj.source_count_name);

            if(!bool_cancel_extraction)
                emit signal_completion_status(MACRO_JobType_Optical_Character_Recognition, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);
        }
    }

    emit signal_add_optical_character_recognition_to_case_tree(ocr_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Optical_Character_Recognition);
    emit finished();

}

void thread_optical_character_recognition::slot_process_optical_character_recognition_readyread()
{ // while running ocr we get output here which we use that data to display the progress to user

    QByteArray byte_arr_proc_out = process_run_optical_character_recognition->readAll();

    QString str_proc_out = QString::fromLocal8Bit(byte_arr_proc_out);

    QStringList list = str_proc_out.split("\n");

    QStringList rec_cnt_list;

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
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Optical_Character_Recognition, "Calculating...");
            else
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Optical_Character_Recognition, estimated_time);

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

                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Optical_Character_Recognition, source_name, true, rec_count, progress_val, false);
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

                    QString cmd_update_fs_run_status = "Update files set fs_module_optical_character_recognition_run_status = '1' WHERE INT <= ?";

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_update_fs_run_status, QStringList(last_rec_int_value), file_system_db_path, Q_FUNC_INFO);

                    rec_cnt_list.clear();

                    mutext_for_fs_main_db->unlock();
                    break;
                }
            }
        }
    }
}

bool thread_optical_character_recognition::open_fs_db(QString fs_db_path)
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


void thread_optical_character_recognition::slot_process_optical_character_recognition_finished(int status)
{ // When OCR Extraction finished we update run status in main file_system.sqlite database
    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE)
    {
        if(cmd_update_fs_run_status.endsWith("OR "))
            cmd_update_fs_run_status.chop(QString("OR ").size());

        mutext_for_fs_main_db->lock();
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_update_fs_run_status, file_system_db_path, Q_FUNC_INFO);
        mutext_for_fs_main_db->unlock();
    }

    bool_optical_character_recognition_process_finished = true;
    process_run_optical_character_recognition->close();
}

void thread_optical_character_recognition::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_optical_character_recognition::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_optical_character_recognition::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_optical_character_recognition::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_optical_character_recognition::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_optical_character_recognition::extract_optical_character_recognition_for_fs_run_module()
{ // This is OCR Extraction feature on right click to run on the file system else approach is same as we run OCR from pre launcher

    cmd_update_fs_run_status = "Update files set fs_module_optical_character_recognition_run_status = '1' WHERE ";
    bool_cancel_extraction = false;

    QString run_by_right_click = "TRUE"; //Do Not Use Macro Here.

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


        QString ocr_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + sou_cnt_name + QString("/optical_character_recognition.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + sou_cnt_name + QString("/file_system.sqlite");

        QString tmp_fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Optical_Character_Recognition_In_Result_QString).toString()
                + sou_cnt_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch());

        QDir tmp_dir;
        tmp_dir.mkpath(tmp_fs_dir_path);

        QString tmp_fs_db_path = tmp_fs_dir_path + "/tmp_fs_copy_optical_character_recognition.sqlite";

        QStringList cp_args;
        cp_args << fs_db_path << tmp_fs_db_path;

        QProcess proc_cp_fs_db;

        proc_cp_fs_db.start("cp", cp_args);

        proc_cp_fs_db.waitForFinished(-1);

        proc_cp_fs_db.close();

        file_system_db_path = fs_db_path;

        source_name = source_obj.source_name;

        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Optical_Character_Recognition, source_obj.source_name, true, 0, 0, false);

        qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Optical_Character_Recognition_Minimum_File_Size_Bytes_qint64).toLongLong();

        QString cmd = "./ocr_text_extractor";
        QStringList proc_args;

        //Syntax: source_count_name os_scheme_internal virtual_source_path tmp_fs_db_path ocr_db_path pref_min_file_size_str
        //run_by_right_click display_file_path

        proc_args << sou_cnt_name << os_scheme_internal << virtual_source_path << tmp_fs_db_path << ocr_db_path << QString::number(pref_min_file_size) << QString(run_by_right_click) << display_file_path;

        QString command = "SELECT is_dir from files WHERE filepath = ?";

        QString is_dir_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(display_file_path), 0, tmp_fs_db_path, Q_FUNC_INFO);

        QString cmd_select_count;
        if(is_dir_str == "0")
        {
            cmd_select_count = "Select count(*) from files Where fs_module_optical_character_recognition_run_status = '0' AND filepath = '" + display_file_path + "'";
        }
        else
        {
            if(!display_file_path.endsWith("/"))
                display_file_path.append("/");

            cmd_select_count = "Select count(*) from files Where fs_module_optical_character_recognition_run_status = '0' AND filepath LIKE '" + display_file_path + "%'";

        }

        QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_select_count , 0, tmp_fs_db_path, Q_FUNC_INFO);

        total_records_num = 0;
        total_records_num = total_records_str.toLongLong();

        remaining_file_count_for_req_time_qint64 = total_records_num;
        QTimer *timer_obj = new QTimer(this);
        connect(timer_obj,SIGNAL(timeout()),this,SLOT(slot_timeout_to_update_estimated_time_for_fs_modules()));
        timer_obj->start(10000);

        rec_count = 0;

        process_run_optical_character_recognition = new QProcess(this);
        process_run_optical_character_recognition->setProcessChannelMode(QProcess::MergedChannels);
        connect(process_run_optical_character_recognition, SIGNAL(readyRead()), this, SLOT(slot_process_optical_character_recognition_readyread()));

        process_run_optical_character_recognition->start(cmd,proc_args);
        process_run_optical_character_recognition->waitForStarted();

        bool_optical_character_recognition_process_finished = false;
        connect(process_run_optical_character_recognition, SIGNAL(finished(int)), this, SLOT(slot_process_optical_character_recognition_finished(int)));

        int countt = 0;
        while(1)
        {
            countt++;
            if(countt % 50 == 0)
            {
                countt = 0;
                QCoreApplication::processEvents();
            }

            if(bool_optical_character_recognition_process_finished || process_run_optical_character_recognition->state() == QProcess::NotRunning)
                break;

            if(bool_cancel_extraction)
            {
                break;
            }
        }

        copy_data_from_ocr_to_fs_db(sou_cnt_name);

        timer_obj->stop();
    }

    QStringList ocr_db_path_list;
    for(int counttt = 0; counttt < list_target_witness_info.size(); counttt++)
    {
        QString ocr_db_path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_witness_info.at(counttt).source_count_name + QString("/optical_character_recognition.sqlite");


        ocr_db_path_list << ocr_db_path_1;
    }

    emit signal_add_optical_character_recognition_to_case_tree(ocr_db_path_list);

}

void thread_optical_character_recognition::copy_data_from_ocr_to_fs_db(QString source_count_name)
{ // Here we copy the data from optical_character_recognition.sqlite to main file_system.sqlite database

    QString ocr_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/optical_character_recognition.sqlite");


    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/file_system.sqlite");


    QStringList attribute_column_list_in_ocr_db;
    attribute_column_list_in_ocr_db << "ocr_text";

    QStringList attribute_column_list_in_fs_db;
    attribute_column_list_in_fs_db << "ocr_text";


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

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS ocrDB")
            .arg(QDir::toNativeSeparators(ocr_db_path));
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
        command_str.append(QString(" " + attribute_column_list_in_fs_db.at(ii) + " = (Select " + attribute_column_list_in_ocr_db.at(ii) + " From ocrDB.files where main.files.INT = ocrDB.files.filesystem_record),"));
    }
    if(command_str.trimmed().endsWith(","))
        command_str.chop(1);

    bool_cancel_extraction = false;

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



    QString command_detach = QString("DETACH DATABASE ocrDB");
    dest_query.exec(command_detach);
    destination_db.close();


}

void thread_optical_character_recognition::slot_timeout_to_update_estimated_time_for_fs_modules()
{ // timer approach which tells the estimated completion time of OCR Extraction
    double avg_image_per_sec = processed_file_count_qint64 / 5;

    qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;
    double required_time = new_ramining_file_count / avg_image_per_sec;

    estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");

    processed_file_count_qint64 = 0;
}
