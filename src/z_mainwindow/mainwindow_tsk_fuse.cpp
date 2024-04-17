#include "mainwindow.h"


void MainWindow::slot_tsk_fuse_process_data()
{
    QString custom_msg = QString::fromLocal8Bit(process_tsk_fuse->readAllStandardError()).trimmed();

    // "\"Process_Start\""
    custom_msg.remove("\"");

    if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSFS_COMMON_Device_Node_Open_Failed))
    {
        recon_static_functions::app_debug("TSKFUSE Device_Node_Open_Failed ---FAILED--- ", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSFS_COMMON_File_System_Not_Found_On_Node))
    {
        recon_static_functions::app_debug("TSKFUSE File_System_Not_Found_On_Node ---FAILED--- ", Q_FUNC_INFO);
    }


    if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSFS_COMMON_Process_Finished_With_Success))
    {
        recon_static_functions::app_debug("TSKFUSE Process_Finished_With_Success", Q_FUNC_INFO);
    }
    else if(custom_msg.startsWith(MACRO_GLOBAL_RECON_AND_FSFS_COMMON_Process_Finished_With_Failure))
    {
        recon_static_functions::app_debug("TSKFUSE Process_Finished_With_Failure ---FAILED--- ", Q_FUNC_INFO);
    }

}

void MainWindow::slot_tsk_fuse_finished(int exit_code)
{

    if(exit_code != 0)
    {
        //  recon_static_functions::app_debug(QString(MACRO_GLOBAL_RECON_AND_FSFS_COMMON_Process_Finished_With_Crash) + " ---FAILED--- ", Q_FUNC_INFO);
        recon_static_functions::app_debug("TSKFUSE Process_Finished_With_Crash ---FAILED--- ", Q_FUNC_INFO);
    }

    bool_process_tsk_fuse_finished = true;

}

void MainWindow::update_tsk_mount_path_for_source()
{

    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    bool_process_tsk_fuse_finished = false;

    process_tsk_fuse  = new QProcess(this);
    process_tsk_fuse->setReadChannel(QProcess::StandardOutput);
    connect(process_tsk_fuse, SIGNAL(readyReadStandardError()), this, SLOT(slot_tsk_fuse_process_data()));
    connect(process_tsk_fuse, SIGNAL(finished(int)), this, SLOT(slot_tsk_fuse_finished(int)));

    QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    bool bool_any_source_updated = false;

    //================ TSK Fuse mount start ================//
    for(int pp = 0; pp < list_source_info.size(); pp++)
    {

        struct_GLOBAL_witness_info_source src_obj =  list_source_info.at(pp);
        if(!recon_static_functions::is_source_supported_for_tsk(src_obj))
        {
            continue;
        }


        struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);


        if(!pwd_obj.bool_desktop_pwd_accurate)
            continue;

        QString m_vrtl_path = src_obj.virtual_source_path;

        QString m_node = global_recon_helper_singular_class_obj->get_disk_node_of_mounted_path(m_vrtl_path, Q_FUNC_INFO);
        if(m_node.isEmpty())
            continue;


        QString r_dev = "/dev/";
        m_node = m_node.remove(0, r_dev.size());
        m_node.prepend(r_dev + "r");  // /dev/rdisk3


        QString fuse_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString();
        QString available_file_1 = recon_static_functions::get_available_filename(src_obj.source_count_name,fuse_dir_path, Q_FUNC_INFO);
        QString fuse_mnt_path = fuse_dir_path + available_file_1;
        QDir mdir;
        mdir.mkpath(fuse_mnt_path);

        QString m_pwd = pwd_obj.desktop_password_original;


        QString exec_path = "./fsfs";

        //QString cmd_load = QString("echo '") + m_pwd + "' | sudo -S " + exec_path.replace(" ", "\\ ") + " -o allow_other " + fuse_mnt_path.replace(" ", "\\ ") + " " + m_node;
        QString cmd_load = QString("echo $FORENSOPASSWD")  + " | sudo -S " + exec_path.replace(" ", "\\ ") + " -o allow_other " + fuse_mnt_path.replace(" ", "\\ ") + " " + m_node;

        //        QString available_file_2 = recon_static_functions::get_available_filename("m_tsk_fuse",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
        //        QString script_file = MACRO_RECON_Temp_DirPath_In_Bundle + available_file_2;

        QString available_file_2 = recon_static_functions::get_available_filename("m_tsk_fuse",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
        QString script_file = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file_2;

        qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_file,cmd_load, Q_FUNC_INFO);
        if(byte_size == -1)
            continue;

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("FORENSOPASSWD", m_pwd);

        QStringList args;
        args << script_file;

        process_tsk_fuse->setProcessEnvironment(env);
        process_tsk_fuse->start("bash", args);
        //process_tsk_fuse->start("sh", QStringList(script_file));

        if (!process_tsk_fuse->waitForStarted())
        {
            recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + process_tsk_fuse->errorString() ,Q_FUNC_INFO);
            return;
        }

        recon_static_functions::debug_conditional(" : running111", Q_FUNC_INFO);
        ///forcing thread to wait and read when data available from process.
        int count = 0;
        while(1)
        {
            count++;
            if(count % 100 == 0)
            {
                count = 0;
                QCoreApplication::processEvents();
            }

            if(process_tsk_fuse->state() == QProcess::NotRunning)
            {
                break;
            }

            process_tsk_fuse->waitForReadyRead();
        }
        recon_static_functions::debug_conditional(" : infinite loop cross111", Q_FUNC_INFO);

        recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
        int m_cnt = 0;
        while(1)
        {
            m_cnt++;
            if(m_cnt % 500 == 0)
            {
                m_cnt = 0;
                QCoreApplication::processEvents();
            }
            if(bool_process_tsk_fuse_finished)
            {
                break;
            }

        }
        recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);

        recon_static_functions::remove_safely_blank_dir_OR_file(script_file,Q_FUNC_INFO);



        // to check fuse is mounted or not
        if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(fuse_mnt_path, Q_FUNC_INFO))
            continue;


        fuse_mnt_path.append("/");
        src_obj.fuse_mount_path_tsk = fuse_mnt_path;

        bool_any_source_updated = true;
        list_source_info.removeAt(pp);
        list_source_info.insert(pp, src_obj);

    }
    //================ TSK Fuse mount end ================//


    if(bool_any_source_updated)
        global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info);


    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

}

