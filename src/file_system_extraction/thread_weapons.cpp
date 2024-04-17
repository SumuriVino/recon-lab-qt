#include "thread_weapons.h"

thread_weapons::thread_weapons(QObject *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);
}

thread_weapons::~thread_weapons()
{

}
void thread_weapons::slot_weapons_extraction()
{
    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Weapons);

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    if(recon_case_runner_type == QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE))
    {
        extract_weapons_for_fs_run_module();
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Weapons);
        emit finished();
        return;
    }

    QStringList weapons_db_path_list;

    QString run_by_right_click = "FALSE"; //Do Not Use Macro Here.

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_extraction_weapons)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/file_system.sqlite");

        QString weapons_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/weapons.sqlite");

        file_system_db_path = filesystem_db_path;

        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            if(bool_cancel_extraction_weapons)
                break;

            weapons_db_path_list << (weapons_db_path);
        }
        else
        {
            weapons_db_path_list << (weapons_db_path);

            if(!job_selected_source_count_name_list.contains(target_struct_obj.source_count_name))
            {
                continue;
            }

            bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(target_struct_obj.source_count_name);
            if(!bool_src_applicable)
                continue;

            QString tmp_fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Weapons_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch());

            QDir tmp_dir;
            tmp_dir.mkpath(tmp_fs_dir_path);

            QString tmp_fs_db_path = tmp_fs_dir_path + "/tmp_file_system.sqlite";

            QStringList cp_args;
            cp_args << filesystem_db_path << tmp_fs_db_path;

            source_name = target_struct_obj.source_name;

            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Weapons, target_struct_obj.source_name, true, 0, 0, false);

            QProcess proc_cp_fs_db;
            proc_cp_fs_db.start("cp", cp_args);
            proc_cp_fs_db.waitForFinished(-1);
            proc_cp_fs_db.close();

            QString source_count_name = target_struct_obj.source_count_name;
            QString os_scheme_internal = target_struct_obj.os_scheme_internal;
            QString virtual_source_path = target_struct_obj.virtual_source_path;

            qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Weapons_Minimum_File_Size_Bytes_qint64).toLongLong();

            QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("SELECT count(*) FROM files WHERE fs_module_weapons_run_status = '0'") , 0, tmp_fs_db_path, Q_FUNC_INFO);
            total_records_num = 0;
            total_records_num = total_records_str.toLongLong();

            //            QString cmd_python_path = global_varibale_python_path;

            rec_count = 0;

            //            QString yolo_weight_path = "../Resources/scripts_persistent/weapon_fire_yolov3.weights";
            //            QString yolo_cfg_path = "../Resources/scripts_persistent/weapon_fire_yolov3.cfg";
            //            QString objects_path = "../Resources/scripts_persistent/weapon_fire_obj.names";

            QString yolo_weight_path = "../ZAlreadySigned/pysafe/intelligence_army/weapon_fire_yolov3.weights";
            QString yolo_cfg_path = "../ZAlreadySigned/pysafe/intelligence_army/weapon_fire_yolov3.cfg";
            QString objects_path = "../ZAlreadySigned/pysafe/intelligence_army/weapon_fire_obj.names";

            process_weapons = new QProcess(this);
            process_weapons->setProcessChannelMode(QProcess::MergedChannels);
            connect(process_weapons, SIGNAL(readyRead()), this, SLOT(slot_process_weapons_readyread()));

            QStringList python_args;

            //Syntax: script_path source_count_name os_scheme_internal virtual_source_path tmp_fs_db_path weapons_db_path
            // yolo_weight_path yolo_cfg_path objects_path pref_min_file_size_str run_by_right_click display_file_path category_gun category_rifles

            //            python_args << "../Resources/scripts_persistent/weapons.py" << source_count_name << os_scheme_internal << virtual_source_path
            //                        << tmp_fs_db_path << weapons_db_path << yolo_weight_path << yolo_cfg_path << objects_path
            //                        << QString::number(pref_min_file_size) << run_by_right_click << ""
            //                        << QString(MACRO_CASE_TREE_Weapons_Category_Gun) << QString(MACRO_CASE_TREE_Weapons_Category_Rifles);


            //            process_weapons->start(cmd_python_path,python_args);

            python_args  << source_count_name << os_scheme_internal << virtual_source_path
                         << tmp_fs_db_path << weapons_db_path << yolo_weight_path << yolo_cfg_path << objects_path
                         << QString::number(pref_min_file_size) << run_by_right_click << ""
                         << QString(MACRO_CASE_TREE_Weapons_Category_Gun) << QString(MACRO_CASE_TREE_Weapons_Category_Rifles);

            QString weapons_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/weapons";
            process_weapons->start(weapons_exe_path,python_args);

            if (!process_weapons->waitForStarted())
            {
                recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
                recon_static_functions::app_debug("Error:" + process_weapons->errorString() ,Q_FUNC_INFO);
                continue;
            }

            bool_weapons_extraction_process_finished = false;
            connect(process_weapons, SIGNAL(finished(int)), this, SLOT(slot_process_weapons_finished(int)));

            int count = 0;
            while(1)
            {
                count++;
                if(count % 100 == 0)
                {
                    QCoreApplication::processEvents();
                    count = 0;
                }

                if(bool_weapons_extraction_process_finished || process_weapons->state() == QProcess::NotRunning)
                    break;

                if(bool_cancel_extraction_weapons)
                {
                    break;
                }
            }

            if(!bool_cancel_extraction_weapons)
                emit signal_completion_status(MACRO_JobType_Weapons, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);
        }
    }
    emit signal_add_weapons_to_case_tree(weapons_db_path_list);
    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Weapons);
    emit finished();
}

void thread_weapons::slot_process_weapons_readyread()
{
    QByteArray byte_arr_proc_out = process_weapons->readAll();
    QString str_proc_out = QString::fromLocal8Bit(byte_arr_proc_out);
    QStringList list = str_proc_out.split("\n");
    QStringList rec_cnt_list ;

    for(int ii = 0; ii < list.size(); ii++)
    {
        if(bool_cancel_extraction_weapons)
        {
            mutext_for_fs_main_db->unlock();
            return;
        }

        QString str_data = list.at(ii);
        if(str_data.startsWith("INT"))
        {
            processed_file_count_qint64++;
            remaining_file_count_for_req_time_qint64--;

            if(estimated_time.isEmpty() || estimated_time == "00:00:00")
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Weapons, "Calculating...");
            else
                emit signal_PBAR_thread_file_system_required_time(MACRO_JobType_Weapons, estimated_time);

            QString tmp_line = str_data;

            QString cnt_str = tmp_line.remove("INT");

            rec_cnt_list << cnt_str;

            QString last_rec_int_value = rec_cnt_list.last();

            qint64 last_rec_int = last_rec_int_value.toInt();
            rec_count++;

            if(rec_count % 20 == 0 || last_rec_int == total_records_num )
            {
                qint64 progress_val = ((rec_count * 100) / total_records_num);

                if(progress_val > 99)
                    progress_val = 99;

                emit signal_PBAR_thread_file_system_value(MACRO_JobType_Weapons, source_name, true, rec_count, progress_val, false);
            }


            if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE)
            {
                cmd_update_fs_run_status.append("INT ="+ cnt_str + " OR ");
            }

            else
            {
                if( rec_count % 100 == 0 || last_rec_int == total_records_num)
                {
                    mutext_for_fs_main_db->lock();

                    QString cmd = "UPDATE files SET fs_module_weapons_run_status = '1' WHERE INT <= ?";

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,QStringList(last_rec_int_value),file_system_db_path,Q_FUNC_INFO);

                    rec_cnt_list.clear();

                    mutext_for_fs_main_db->unlock();
                }
            }
        }
    }
}

void thread_weapons::slot_process_weapons_finished(int status)
{
    if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE)
    {
        if(cmd_update_fs_run_status.endsWith("OR "))
            cmd_update_fs_run_status.chop(QString("OR ").size());

        mutext_for_fs_main_db->lock();
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_update_fs_run_status, file_system_db_path, Q_FUNC_INFO);
        mutext_for_fs_main_db->unlock();
    }

    bool_weapons_extraction_process_finished = true;
    process_weapons->close();
}

void thread_weapons::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}

void thread_weapons::pub_set_job_selected_source_count_name_list(QStringList m_list)
{
    job_selected_source_count_name_list = m_list;
}

void thread_weapons::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction_weapons = status;
}

void thread_weapons::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

void thread_weapons::pub_set_fs_run_module_file_info_list(QList<struct_global_run_fs_modules_details> fs_modules_list)
{
    run_fs_modules_details_list = fs_modules_list;
}

void thread_weapons::extract_weapons_for_fs_run_module()
{
    cmd_update_fs_run_status = "Update files set fs_module_weapons_run_status = '1' WHERE ";
    bool_cancel_extraction_weapons = false;

    QString run_by_right_click = "TRUE"; //Do Not Use Macro Here.

    for(int ii = 0; ii < run_fs_modules_details_list.size(); ii++)
    {
        if(bool_cancel_extraction_weapons)
            break;

        struct_global_run_fs_modules_details details_obj = run_fs_modules_details_list.at(ii);

        QString sou_cnt_name = details_obj.source_count_name;
        QString display_file_path = details_obj.display_filepath;

        struct_GLOBAL_witness_info_source source_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(sou_cnt_name);

        QString virtual_source_path = source_obj.virtual_source_path;
        QString os_scheme_internal = source_obj.os_scheme_internal;

        QString weapons_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + sou_cnt_name + QString("/weapons.sqlite");

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + sou_cnt_name + QString("/file_system.sqlite");

        QString tmp_fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Weapons_In_Result_QString).toString()
                + sou_cnt_name + "/" + QString::number(QDateTime::currentSecsSinceEpoch());

        QDir tmp_dir;
        tmp_dir.mkpath(tmp_fs_dir_path);

        QString tmp_fs_db_path = tmp_fs_dir_path + "/tmp_file_system.sqlite";

        QStringList cp_args;
        cp_args << fs_db_path << tmp_fs_db_path;

        QProcess proc_cp_fs_db;

        proc_cp_fs_db.start("cp", cp_args);

        proc_cp_fs_db.waitForFinished(-1);

        proc_cp_fs_db.close();

        file_system_db_path = fs_db_path;

        source_name = source_obj.source_name;

        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Weapons, source_obj.source_name, true, 0, 0, false);

        qint64 pref_min_file_size = global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Weapons_Minimum_File_Size_Bytes_qint64).toLongLong();

        //        QString yolo_weight_path = "../Resources/scripts_persistent/weapon_fire_yolov3.weights";
        //        QString yolo_cfg_path = "../Resources/scripts_persistent/weapon_fire_yolov3.cfg";
        //        QString objects_path = "../Resources/scripts_persistent/weapon_fire_obj.names";

        QString yolo_weight_path = "../ZAlreadySigned/pysafe/intelligence_army/weapon_fire_yolov3.weights";
        QString yolo_cfg_path = "../ZAlreadySigned/pysafe/intelligence_army/weapon_fire_yolov3.cfg";
        QString objects_path = "../ZAlreadySigned/pysafe/intelligence_army/weapon_fire_obj.names";

        QString command = "SELECT is_dir from files WHERE filepath = ?";

        QString is_dir_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(display_file_path), 0, tmp_fs_db_path, Q_FUNC_INFO);

        QString cmd_select_count;
        if(is_dir_str == "0")
        {
            cmd_select_count = "Select count(*) from files Where fs_module_weapons_run_status = '0' AND filepath = '" + display_file_path + "'";
        }
        else
        {
            if(!display_file_path.endsWith("/"))
                display_file_path.append("/");

            cmd_select_count = "Select count(*) from files Where fs_module_weapons_run_status = '0' AND filepath LIKE '" + display_file_path + "%'";

        }

        QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_select_count , 0, tmp_fs_db_path, Q_FUNC_INFO);

        total_records_num = 0;
        total_records_num = total_records_str.toLongLong();

        remaining_file_count_for_req_time_qint64 = total_records_num;

        QTimer *timer_obj = new QTimer(this);
        connect(timer_obj,SIGNAL(timeout()),this,SLOT(slot_timeout_to_update_estimated_time_for_fs_modules()));
        timer_obj->start(5000);

        rec_count = 0;

        //        QString cmd_python_path = global_varibale_python_path;

        process_weapons = new QProcess(this);
        process_weapons->setProcessChannelMode(QProcess::MergedChannels);
        connect(process_weapons, SIGNAL(readyRead()), this, SLOT(slot_process_weapons_readyread()));

        //Syntax: script_path source_count_name os_scheme_internal virtual_source_path tmp_fs_db_path weapons_db_path
        // yolo_weight_path yolo_cfg_path objects_path pref_min_file_size_str run_by_right_click display_file_path  category_gun category_rifles

        QStringList python_args;
        //        python_args << "../Resources/scripts_persistent/weapons.py" << sou_cnt_name << os_scheme_internal << virtual_source_path
        //                    << tmp_fs_db_path << weapons_db_path << yolo_weight_path << yolo_cfg_path << objects_path
        //                    << QString::number(pref_min_file_size) << QString(run_by_right_click)
        //                    << display_file_path
        //                    << QString(MACRO_CASE_TREE_Weapons_Category_Gun) << QString(MACRO_CASE_TREE_Weapons_Category_Rifles);

        //        process_weapons->start(cmd_python_path,python_args);

        python_args << sou_cnt_name << os_scheme_internal << virtual_source_path
                    << tmp_fs_db_path << weapons_db_path << yolo_weight_path << yolo_cfg_path << objects_path
                    << QString::number(pref_min_file_size) << QString(run_by_right_click)
                    << display_file_path
                    << QString(MACRO_CASE_TREE_Weapons_Category_Gun) << QString(MACRO_CASE_TREE_Weapons_Category_Rifles);

        QString weapons_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/weapons";

        process_weapons->start(weapons_exe_path,python_args);

        if (!process_weapons->waitForStarted())
        {
            recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + process_weapons->errorString() ,Q_FUNC_INFO);
            continue;
        }

        bool_weapons_extraction_process_finished = false;
        connect(process_weapons, SIGNAL(finished(int)), this, SLOT(slot_process_weapons_finished(int)));

        int count = 0;
        while(1)
        {
            count++;
            if(count % 100 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(bool_weapons_extraction_process_finished || process_weapons->state() == QProcess::NotRunning)
                break;

            if(bool_cancel_extraction_weapons)
            {
                break;
            }
        }
        timer_obj->stop();
    }

    QStringList weapons_db_path_list;
    for(int countt = 0; countt < list_target_source_info.size(); countt++)
    {
        QString weapons_db_path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(countt).source_count_name + QString("/weapons.sqlite");

        weapons_db_path_list << weapons_db_path_1;
    }

    emit signal_add_weapons_to_case_tree(weapons_db_path_list);
}

void thread_weapons::slot_timeout_to_update_estimated_time_for_fs_modules()
{
    double avg_image_per_sec = processed_file_count_qint64 / 5;

    qint64 new_ramining_file_count = remaining_file_count_for_req_time_qint64;
    double required_time = new_ramining_file_count / avg_image_per_sec;

    estimated_time = QTime::fromMSecsSinceStartOfDay(required_time * 1000).toString("hh:mm:ss");

    processed_file_count_qint64 = 0;
}
