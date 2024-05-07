#include "export_rsync.h"

export_rsync::export_rsync(QObject *parent)
{
    proc_rsync = new QProcess(this);
    connect(proc_rsync, SIGNAL(finished(int)), this, SLOT(slot_proc_rsync_finished(int)));

    bool_cancel_rsync_copy = false;
    bool_proc_rsync_finished = false;

    recon_helper_standard_obj = new recon_helper_standard(this);
}

export_rsync::~export_rsync()
{

}

void export_rsync::slot_proc_rsync_finished(int)
{
    bool_proc_rsync_finished = true;
}

void export_rsync::pub_cancel_rsync_copy(QString caller_func)
{ // cancel rsync copy
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    bool_cancel_rsync_copy = true;


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);
}



void export_rsync::set_rsync_copy_essentials(QString caller_func)
{ // set rsync copy essentials
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    bool_cancel_rsync_copy = false;

    bool_proc_rsync_finished = false;

    QString temp_dir_path_to_export_ios_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Dir_Export_In_Result_QString).toString();
    if(QFileInfo(temp_dir_path_to_export_ios_dir).exists())
    {
        recon_static_functions::do_directory_blank_completely(temp_dir_path_to_export_ios_dir , caller_func);
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
}

QString export_rsync::pub_copy_file_using_rsync(QString src_path, QString dest_path, int arg_status)
{ // copy file using rsync lib which is bit fast & don't give any warning or error as compare to manually copy
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    if(QFileInfo(dest_path).exists())
        return dest_path;


    QString exported_file_path;

    QProcess *proc_rsync = new QProcess(this);
    connect(proc_rsync, SIGNAL(finished(int)), this, SLOT(slot_proc_rsync_finished(int)));

    QStringList arg_list;


    if(arg_status == enum_global_rsync_args_type_general)
        arg_list << "-a" << src_path  << dest_path;
    else if(arg_status == enum_global_rsync_args_type_links_and_recursive)
        arg_list << "-a" << "--no-links" << "-rEpt" << src_path  << dest_path;

    exported_file_path = dest_path;
    bool_proc_rsync_finished = false;
    proc_rsync->start("rsync", arg_list);
    if (!proc_rsync->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + proc_rsync->errorString() ,Q_FUNC_INFO);
        return "" ;
    }
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_proc_rsync_finished ||  proc_rsync->state() != QProcess::Running)
            break;
    }

    recon_static_functions::app_debug("End" , Q_FUNC_INFO);
    return exported_file_path;

}

QString export_rsync::pub_copy_file_using_rsync_for_export_as_folder(QString src_path, QString dest_location , QString displayed_file_name , QString caller_func)
{ // copy as a folder using rsync
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QString exported_file_path;
    if(bool_cancel_rsync_copy)
        return exported_file_path;

    QFileInfo source_path_info(src_path);
    if(!source_path_info.exists())
        return exported_file_path;


    if(!dest_location.endsWith("/"))
        dest_location.append("/");

    QString available_file =  recon_static_functions::get_available_filename(displayed_file_name,dest_location, Q_FUNC_INFO);
    QString available_path = dest_location + available_file;
    available_path = dest_location + available_file;
    QString destination_file_path = available_path;


    QString command;
    QString source_file_path = src_path;

    QStringList arg_list;
    command = "rsync";

    if(source_path_info.isDir())
    {
        arg_list << "-a" << source_file_path.append("/.") << destination_file_path;
        exported_file_path = destination_file_path;
    }
    else
    {
        arg_list << "-a" << source_file_path  << destination_file_path;
        exported_file_path = destination_file_path;
    }



    bool_proc_rsync_finished = false;

    proc_rsync->start(command, arg_list);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(bool_proc_rsync_finished)
            break;

        if(bool_cancel_rsync_copy)
            proc_rsync->terminate();

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    return exported_file_path;
}

QString export_rsync::pub_get_source_path_to_export_fs_record_by_dbreference(struct_global_export_file_system_record rcv_obj , QSqlDatabase &destination_db, QString caller_func)
{ // get complete source path to export from fs record
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    QString filepath_to_export;

    QString parent_path = rcv_obj.display_file_path;
    QString source_count_name = rcv_obj.source_count_name;
    QString record_no = rcv_obj.record_no;


    struct_GLOBAL_witness_info_source target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_intrnl = target_source_info.os_scheme_internal;

    if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
    {
        if(recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no,destination_db ,"files" ,source_count_name ,Q_FUNC_INFO))
        {
            QString temp_dir_path_to_export_ios_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Dir_Export_In_Result_QString).toString();

            QString command = "select INT from files where file_parent_path LIKE '" + parent_path + "%'";
            QStringList INT_List =  recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command, 0, destination_db, Q_FUNC_INFO);

            QString create_dir_path ;
            int counter = 0;
            for(qint64 ii = 0 ; ii < INT_List.size(); ii++)
            {
                if(bool_cancel_rsync_copy)
                    break;

                if(counter % 50 == 0)
                {
                    counter = 0;
                    QCoreApplication::processEvents();
                }

                QString relevant_record_no = INT_List.at(ii);
                QString command = "select filepath , filename from files where INT =" + relevant_record_no;
                QString display_file_path =  recon_helper_standard_obj->get_string_from_db_by_dbreference(command, 0,destination_db, Q_FUNC_INFO);

                create_dir_path = temp_dir_path_to_export_ios_dir + display_file_path;

                if(recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(relevant_record_no,destination_db,"files",source_count_name,Q_FUNC_INFO))
                {
                    QDir mk_path_dir;
                    mk_path_dir.mkpath(create_dir_path);
                }
                else
                {
                    QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(relevant_record_no ,destination_db ,"files",source_count_name ,Q_FUNC_INFO);
                    QFileInfo file_info(file_path);
                    if(!file_info.exists())
                        continue ;


                    QFileInfo m_info(create_dir_path);
                    QDir m_dir;
                    m_dir.mkpath(m_info.absolutePath());


                    QString proc_cmd = "rsync";
                    QStringList arg_list;
                    arg_list << "-a" << file_path << create_dir_path;
                    QProcess proc_rsync ;
                    proc_rsync.execute(proc_cmd , arg_list);


                    if(bool_cancel_rsync_copy)
                        proc_rsync.terminate();

                }
            }

            filepath_to_export = temp_dir_path_to_export_ios_dir + parent_path;
        }
        else
        {
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,"files",source_count_name,Q_FUNC_INFO);
            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end 111 ", Q_FUNC_INFO);
                return filepath_to_export;
            }

            filepath_to_export = complete_path;

        }

    }
    else
    {
        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,"files",source_count_name,Q_FUNC_INFO);
        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end 111 ", Q_FUNC_INFO);
            return filepath_to_export;
        }
        filepath_to_export = complete_path;
    }
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);

    return filepath_to_export;
}

QString export_rsync::pub_get_source_path_to_export_fs_record_by_dbpath(struct_global_export_file_system_record obj , QString caller_func)
{ // get complete source from fs record for exporting
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    QString filepath_to_export;

    QString destination_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + obj.source_count_name + "/file_system.sqlite";
    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return "";
    }


    filepath_to_export = pub_get_source_path_to_export_fs_record_by_dbreference(obj ,*destination_db, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);

    return filepath_to_export;
}


