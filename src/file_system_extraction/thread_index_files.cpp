#include "thread_index_files.h"

thread_index_files::thread_index_files(QObject *parent) : QObject(parent)
{
    bool_cancel_extraction = false;
    max_filesize_to_index = 0;
    bool_is_running_from_toolbar = false;

    recon_helper_standard_obj = new recon_helper_standard(this);

}

void thread_index_files::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}


void thread_index_files::prepare_whitelist_and_blacklist()
{ // Here we are preparing extensions which needs to index
    //whitelist inludes documents and blacklist includes disk images

    bool_whitelist_check_status = false;
    bool_blacklist_check_status = false;


    QString result_config_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString selected_index_filter = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select select_status from tbl_selected_text_index_filter"), 0, result_config_db_file, Q_FUNC_INFO);

    /* selected_index_filter is 0 for All
     * selected_index_filter is 1 for WhiteList
     * selected_index_filter is 2 for BlackList
     */

    selected_index_filter = selected_index_filter.trimmed();
    if(selected_index_filter.isEmpty())
        selected_index_filter = QString::number(0);

    if(selected_index_filter == QString::number(1))
    {
        bool_whitelist_check_status = true;

        QString command_str = QString("Select extension From tbl_index_files_whitelist_extension");
        QStringList extensions_from_db_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0, result_config_db_file, Q_FUNC_INFO);

        whitelist_extensions_list << extensions_from_db_list;
    }
    else if(selected_index_filter == QString::number(2))
    {

        bool_blacklist_check_status = true;

        QString command_str = QString("Select extension From tbl_index_files_blacklist_extension");
        QStringList extensions_from_db_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0, result_config_db_file, Q_FUNC_INFO);

        blacklist_extensions_list << extensions_from_db_list;
    }
    else if(selected_index_filter == QString::number(0))
    {
        blacklist_extensions_list.clear();
        whitelist_extensions_list.clear();
    }


    // max_filesize_to_index = global_narad_muni_class_obj->get_field(MACRO_NARAD_Index_Files_Less_Than_Size_InBytes_qint64).toLongLong();


}

void thread_index_files::pub_set_is_running_from_toolbar(bool status)
{
    bool_is_running_from_toolbar = status;
}


void thread_index_files::slot_extract_dt_search_data()
{ // from here, Text indexing starts. we use dt search library for the text indexing which helps us in content search feature
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    whitelist_extensions_list.clear();
    blacklist_extensions_list.clear();
    max_filesize_to_index = 0;

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Text_Indexing);

    if(bool_is_running_from_toolbar)
    {
        index_files_from_toolbar_selected_files();
    }

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Text_Indexing);
    emit finished();

    return;
}

void thread_index_files::pub_set_bool_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_index_files::pub_set_dt_serach_interface_obj(dt_search_interface *dt_search_obj)
{
    dt_search_interface_obj = dt_search_obj;
}

void thread_index_files::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}

bool thread_index_files::open_fs_db(QString fs_db_path)
{ // open main file_system.sqlite database
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

void thread_index_files::pub_create_dt_search_index_for_every_source(QString source_count_name)
{ // create dt search text indexing for the source
    create_dt_search_index_for_every_source(source_count_name);
}

bool thread_index_files::pub_dt_update_index(QString filepath_str)
{ // update dt search indexing when we try to hit content search, that time this function will get call.
    bool status = dt_search_interface_obj->updateIndex(0, filepath_str);

    return status;
}

void thread_index_files::pub_dt_search_index(QString keyword_str)
{ // when we search any keyword using content search, The keyword will be here in keyword_str variable and dt search will search that the index of it using below function
    dt_search_interface_obj->searchIndex(0,keyword_str);
}

void thread_index_files::create_dt_search_index_for_every_source(QString source_count_name)
{ // create dt search index for particular source/source_count_name
    QString path_of_index_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/dt_search_index/";
    QFile file(path_of_index_dir);
    if(file.exists())
        return;

    QDir dir_index;
    if(dir_index.mkpath(path_of_index_dir))
    {
        bool bool_create_index = dt_search_interface_obj->createIndex(true, path_of_index_dir);
        if(!bool_create_index)
            recon_static_functions::app_debug("DT Search Create index -------FAILED------ " + path_of_index_dir, Q_FUNC_INFO);
        else
            recon_static_functions::app_debug("DT Search Create index successfully done " + path_of_index_dir, Q_FUNC_INFO);
    }
}

void thread_index_files::index_file_content_using_dt_search(QString file_path)
{ //index any particular file by file using dt search
    bool bool_update_index = dt_search_interface_obj->updateIndex(0 , "@", file_path);

    if(!bool_update_index)
        recon_static_functions::app_debug("index file content -------FAILED------ " + file_path, Q_FUNC_INFO);
    else
        recon_static_functions::app_debug("Successfully update index for " + file_path, Q_FUNC_INFO);
}

void thread_index_files::open_and_create_index_files_db(QString index_files_db_path)
{ // function not in use
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_index_files_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_index_files_db.setDatabaseName(":memory:");
    if(!in_memory_index_files_db.open())
    {
        recon_static_functions::app_debug(" index files db Open in memory -------FAILED------ " + in_memory_index_files_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_index_files_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString command_str = QString("create table files (filesystem_record varchar(300),is_file_indexed INTEGER)");

    QSqlQuery query_create_table(in_memory_index_files_db);
    query_create_table.prepare(command_str);

    if(!query_create_table.exec())
    {
        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
        in_memory_index_files_db.close();
        return;
    }

    QString index_files_database_path = index_files_db_path;
    if(QFileInfo(index_files_database_path).exists())
        QFile::remove(index_files_database_path);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(index_files_database_path));

    QSqlQuery query_attach(in_memory_index_files_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        in_memory_index_files_db.close();
        return;
    }

    QSqlQuery query_save(in_memory_index_files_db);
    query_save.prepare(QString("CREATE TABLE filesystemDB.files AS SELECT * from files"));
    if(!query_save.exec())
    {
        recon_static_functions::app_debug(" query memory Save  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
    }

    QSqlQuery query_detach(in_memory_index_files_db);
    if(!query_detach.exec("DETACH DATABASE filesystemDB"))
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
    }

}

void thread_index_files::intermediate_save_data_to_index_files_db(QString index_files_db_path)
{ // function not in use
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(index_files_db_path));

    QSqlQuery query_attach(in_memory_index_files_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_insert(in_memory_index_files_db);

    QString command_insert = QString("INSERT INTO filesystemDB.files SELECT * from files");
    if(!query_insert.exec(command_insert))
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_clear(in_memory_index_files_db);

    QString command_clear = QString("DELETE from files");
    if(!query_clear.exec(command_clear))
    {
        recon_static_functions::app_debug(" query memory delete  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_detach(in_memory_index_files_db);
    QString command_detach = QString("DETACH DATABASE filesystemDB");
    if(!query_detach.exec(command_detach))
    {
        recon_static_functions::app_debug(" query memory detach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }

}

//Before Aug-23-2019. Don't Delete this
//void thread_index_files::index_files_from_toolbar_selected_files()
//{

//    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

//    QString command = QString("Select source_count_name From files Where status = ? group by source_count_name");
//    QStringList value_list;
//    value_list << QString::number(0);

//    QStringList source_count_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, indexing_db_path,Q_FUNC_INFO);
//    source_count_list.removeDuplicates();

//    for(int num = 0; num < source_count_list.size(); num++)
//    {
//        if(bool_cancel_extraction)
//            break;

//        QString source_count_name = source_count_list.at(num);

//        struct_GLOBAL_derived_source_info target_struct_obj = target_info_obj->pub_get_target_info_structure_according_source_count_name(source_count_name);

//        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, 0, 0);

//        create_dt_search_index_for_every_source(target_struct_obj.source_count_name);

//        value_list.clear();
//        value_list << QString::number(0) << source_count_name;


//        command = QString("Select file_path From files Where status = ? AND source_count_name = ?");
//        QStringList file_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, indexing_db_path,Q_FUNC_INFO);

//        //If root dir is selected then process all files
//        bool bool_is_running_on_root = false;

//        QString total_records_str;
//        qint64  total_records_num = 0;

//        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

//        if(target_struct_obj.os_naming_scheme == QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS))
//        {
//            QStringList tmp_filepath_list;
//            if(file_path_list.contains(QString("/")))
//            {
//                bool_is_running_on_root = true;

//                QStringList id_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select id_path from files"), 0, fs_db_path, Q_FUNC_INFO);

//                for(int num = 0; num < id_path_list.size(); num++)
//                {
//                    if(bool_cancel_extraction)
//                        break;

//                    QString id_path_str = id_path_list.at(num).trimmed();

//                    if(id_path_str.isEmpty())
//                        continue;

//                    tmp_filepath_list << id_path_str;
//                    total_records_num += num;
//                }
//            }
//            else
//            {
//                for(int count = 0; count < file_path_list.size(); count++)
//                {
//                    if(bool_cancel_extraction)
//                        break;

//                    QString file_path_str = file_path_list.at(count).trimmed();
//                    if(file_path_str.isEmpty())
//                        continue;

//                    //check for directory/file
//                    QString flag_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(QString("Select flags From files Where filepath = ?"), QStringList(file_path_str), 0, fs_db_path, Q_FUNC_INFO);
//                    flag_str = flag_str.trimmed();

//                    if(flag_str.isEmpty())
//                        continue;

//                    if(flag_str == QString::number(2))
//                    {
//                        //directory
//                        QStringList id_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select id_path from files Where filepath LIKE '" + file_path_str + QString("%'")), 0, fs_db_path, Q_FUNC_INFO);

//                        for(int num = 0; num < id_path_list.size(); num++)
//                        {
//                            if(bool_cancel_extraction)
//                                break;

//                            QString id_path_str = id_path_list.at(num).trimmed();

//                            if(id_path_str.isEmpty())
//                                continue;

//                            tmp_filepath_list << id_path_str + QString(MACRO_RECON_Splitter) + file_path_str;
//                            total_records_num += num;

//                        }
//                    }
//                    else
//                    {
//                        //file
//                        QString id_path_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(QString("Select id_path from files Where filepath = ?"), QStringList(file_path_str), 0, fs_db_path, Q_FUNC_INFO);
//                        id_path_str =  id_path_str.trimmed();
//                        if(id_path_str.isEmpty())
//                            continue;

//                        tmp_filepath_list << id_path_str + QString(MACRO_RECON_Splitter) + file_path_str;
//                        total_records_num += 1;
//                    }
//                }
//            }
//            file_path_list.clear();
//            file_path_list = tmp_filepath_list;

//        }
//        else
//        {
//            if(file_path_list.contains(QString("/")))
//            {
//                file_path_list.clear();
//                value_list.clear();

//                command = QString("Select filepath From files Where file_parent_path = ?");
//                value_list << QString("/");
//                file_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, fs_db_path,Q_FUNC_INFO);

//                bool_is_running_on_root = true;

//                total_records_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(QString("Select count(*) from files Where source_count_name = ?") , QStringList(source_count_name), 0, fs_db_path, Q_FUNC_INFO);
//                total_records_num = total_records_str.toLongLong();

//            }
//            else
//            {
//                for(int count = 0; count < file_path_list.size(); count++)
//                {
//                    if(bool_cancel_extraction)
//                        break;

//                    QString file_path_str = file_path_list.at(count).trimmed();
//                    if(file_path_str.isEmpty())
//                        continue;

//                    QString file_full_path = recon_static_functions::get_complete_file_path(target_struct_obj.fs_extraction_source_path, file_path_str, Q_FUNC_INFO);

//                    if(QFileInfo(file_full_path).isDir())
//                    {
//                        value_list.clear();
//                        value_list << file_path_str << source_count_name;
//                        total_records_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(QString("Select count(*) from files Where file_parent_path = ? AND source_count_name = ?") , value_list, 0, fs_db_path, Q_FUNC_INFO);
//                        total_records_num += total_records_str.toLongLong();
//                    }
//                    else
//                    {
//                        total_records_num += 1;
//                    }
//                }
//            }
//        }




//        if(bool_is_running_on_root)
//            command = QString("Update files set status = ? Where source_count_name = ?");
//        else
//            command = QString("Update files set status = ? Where file_path = ? AND source_count_name = ?");

//        qint64 records_count = 0;

//        for(int count = 0; count < file_path_list.size(); count++)
//        {
//            if(bool_cancel_extraction)
//                break;


//            QString file_path_str = file_path_list.at(count);
//            if(file_path_str.trimmed().isEmpty())
//                continue;

//            QStringList path_split_list;
//            if(file_path_str.contains(QString(MACRO_RECON_Splitter)))
//            {
//                path_split_list = file_path_str.split(QString(MACRO_RECON_Splitter));

//                // for indexing
//                file_path_str = path_split_list.at(0);
//            }


//            QString file_full_path = recon_static_functions::get_complete_file_path(target_struct_obj.fs_extraction_source_path, file_path_str, Q_FUNC_INFO);


//            if(!QFileInfo(file_full_path).exists())
//                continue;
//            else
//            {
//                ///to iterate recursively
//                if(QFileInfo(file_full_path).isDir())
//                {
//                    QDirIterator it(file_full_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories );
//                    while (it.hasNext())
//                    {
//                        if(bool_cancel_extraction)
//                            break;

//                        records_count++;


//                        QString filepath = it.next();
//                        if(filepath.endsWith(".") || filepath.endsWith(".."))
//                            continue;

//                        index_file_content_using_dt_search(filepath);


//                        if(records_count % 10 == 0)
//                        {
//                            qint64 progress_val = ((records_count * 100) / total_records_num);
//                            if(progress_val > 99)
//                                progress_val = 99;

//                            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, records_count, progress_val);
//                        }
//                    }

//                    if(!bool_is_running_on_root)
//                    {
//                        value_list.clear();

//                        if(bool_cancel_extraction)
//                            value_list << QString::number(0) << file_path_str << source_count_name;
//                        else
//                            value_list << QString::number(1) << file_path_str << source_count_name;

//                        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
//                    }
//                }
//                else
//                {
//                    records_count++;

//                    index_file_content_using_dt_search(file_full_path);

//                    if(path_split_list.size() > 1)
//                    {
//                        // for db updation
//                        file_path_str =  path_split_list.at(1);
//                    }

//                    if(!bool_is_running_on_root)
//                    {
//                        value_list.clear();

//                        if(bool_cancel_extraction)
//                            value_list << QString::number(0) << file_path_str << source_count_name;
//                        else
//                            value_list << QString::number(1) << file_path_str << source_count_name;

//                        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
//                    }

//                    if(records_count % 10 == 0)
//                    {
//                        qint64 progress_val = ((records_count * 100) / total_records_num);
//                        if(progress_val > 99)
//                            progress_val = 99;

//                        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, records_count, progress_val);
//                    }
//                }
//            }

//        }

//        if(bool_is_running_on_root)
//        {
//            value_list.clear();

//            if(bool_cancel_extraction)
//                value_list << QString::number(0) <<  source_count_name;
//            else
//                value_list << QString::number(1) <<  source_count_name;

//            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
//        }

//        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, records_count, 100);

//        if(!bool_cancel_extraction)
//            emit signal_completion_status(MACRO_JobType_Text_Indexing, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

//    }


//    return;







//    //======================================//

//   // Before Aug-16-2018 code

////    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

////    QString command = QString("Select source_count_name From files Where status = ? group by source_count_name");
////    QStringList value_list;
////    value_list << QString::number(0);

////    QStringList source_count_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, indexing_db_path,Q_FUNC_INFO);
////    source_count_list.removeDuplicates();

////    for(int num = 0; num < source_count_list.size(); num++)
////    {
////        if(bool_cancel_extraction)
////            break;

////        QString source_count_name = source_count_list.at(num);

////        struct_GLOBAL_derived_source_info target_struct_obj = target_info_obj->pub_get_target_info_structure_according_source_count_name(source_count_name);

////        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, 0, 0);

////        create_dt_search_index_for_every_source(target_struct_obj.source_count_name);

////        value_list.clear();
////        value_list << QString::number(0) << source_count_name;


////        command = QString("Select file_path From files Where status = ? AND source_count_name = ?");
////        QStringList file_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, indexing_db_path,Q_FUNC_INFO);

////        //If root dir is selected then process all files
////        bool bool_is_running_on_root = false;

////        QString total_records_str;
////        qint64  total_records_num = 0;

////        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
////        if(file_path_list.contains(QString("/")))
////        {
////            file_path_list.clear();
////            value_list.clear();

////            command = QString("Select filepath From files Where file_parent_path = ?");
////            value_list << QString("/");
////            file_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, fs_db_path,Q_FUNC_INFO);

////            bool_is_running_on_root = true;

////            total_records_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(QString("Select count(*) from files Where source_count_name = ?") , QStringList(source_count_name), 0, fs_db_path, Q_FUNC_INFO);
////            total_records_num = total_records_str.toLongLong();

////        }
////        else
////        {
////            for(int count = 0; count < file_path_list.size(); count++)
////            {
////                if(bool_cancel_extraction)
////                    break;

////                QString file_path_str = file_path_list.at(count).trimmed();
////                if(file_path_str.isEmpty())
////                    continue;

////                QString file_full_path = recon_static_functions::get_complete_file_path(target_struct_obj.fs_extraction_source_path, file_path_str, Q_FUNC_INFO);

////                if(QFileInfo(file_full_path).isDir())
////                {
////                    value_list.clear();
////                    value_list << file_path_str << source_count_name;
////                    total_records_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(QString("Select count(*) from files Where file_parent_path = ? AND source_count_name = ?") , value_list, 0, fs_db_path, Q_FUNC_INFO);
////                    total_records_num += total_records_str.toLongLong();
////                }
////                else
////                {
////                    total_records_num += 1;
////                }
////            }
////        }




////        if(bool_is_running_on_root)
////            command = QString("Update files set status = ? Where source_count_name = ?");
////        else
////            command = QString("Update files set status = ? Where file_path = ? AND source_count_name = ?");

////        qint64 records_count = 0;

////        for(int count = 0; count < file_path_list.size(); count++)
////        {
////            if(bool_cancel_extraction)
////                break;


////            QString file_path_str = file_path_list.at(count);
////            if(file_path_str.trimmed().isEmpty())
////                continue;

////            QString file_full_path = recon_static_functions::get_complete_file_path(target_struct_obj.fs_extraction_source_path, file_path_str, Q_FUNC_INFO);


////            if(!QFileInfo(file_full_path).exists())
////                continue;
////            else
////            {
////                ///to iterate recursively
////                if(QFileInfo(file_full_path).isDir())
////                {
////                    QDirIterator it(file_full_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories );
////                    while (it.hasNext())
////                    {
////                        if(bool_cancel_extraction)
////                            break;

////                        records_count++;


////                        QString filepath = it.next();
////                        if(filepath.endsWith(".") || filepath.endsWith(".."))
////                            continue;

////                        index_file_content_using_dt_search(filepath);


////                        if(records_count % 10 == 0)
////                        {
////                            qint64 progress_val = ((records_count * 100) / total_records_num);
////                            if(progress_val > 99)
////                                progress_val = 99;

////                            emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, records_count, progress_val);
////                        }
////                    }

////                    if(!bool_is_running_on_root)
////                    {
////                        value_list.clear();

////                        if(bool_cancel_extraction)
////                            value_list << QString::number(0) << file_path_str << source_count_name;
////                        else
////                            value_list << QString::number(1) << file_path_str << source_count_name;



////                        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
////                    }

////                }
////                else
////                {

////                    records_count++;

////                    index_file_content_using_dt_search(file_full_path);

////                    if(!bool_is_running_on_root)
////                    {
////                        value_list.clear();

////                        if(bool_cancel_extraction)
////                            value_list << QString::number(0) << file_path_str << source_count_name;
////                        else
////                            value_list << QString::number(1) << file_path_str << source_count_name;

////                        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
////                    }

////                    if(records_count % 10 == 0)
////                    {
////                        qint64 progress_val = ((records_count * 100) / total_records_num);
////                        if(progress_val > 99)
////                            progress_val = 99;

////                        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, records_count, progress_val);
////                    }
////                }
////            }

////        }

////        if(bool_is_running_on_root)
////        {
////            value_list.clear();
////            value_list << QString::number(1) <<  source_count_name;
////            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
////        }

////        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, records_count, 100);

////        if(!bool_cancel_extraction)
////            emit signal_completion_status(MACRO_JobType_Text_Indexing, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

////    }


////    return;

//}


void thread_index_files::index_files_from_toolbar_selected_files()
{ // Run Text Indexing on those files which we have selected

    global_dt_indexing_processed_files_count = 0;

    int files_chunk_size = 50000; //fifty thousands

    prepare_whitelist_and_blacklist();
    //    bool bool_whitelist_check_status = global_narad_muni_class_obj->get_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool).toBool();
    //    bool bool_blacklist_check_status = global_narad_muni_class_obj->get_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool).toBool();

    bool_cancel_extraction = false;
    dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);

    index_files_for_carved_files();

    if(bool_cancel_extraction)
        return;

    ///get source count from selected files
    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

    QString command = QString("Select source_count_name From files Where status = ? AND plugin_name = ? group by source_count_name");
    QStringList value_list;
    value_list << QString::number(0) << QString(MACRO_Plugin_Name_File_System);

    QStringList source_count_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, indexing_db_path,Q_FUNC_INFO);
    source_count_list.removeDuplicates();

    for(int num = 0; num < source_count_list.size(); num++)
    {
        if(bool_cancel_extraction)
            break;

        QString source_count_name = source_count_list.at(num);

        struct_GLOBAL_witness_info_source target_struct_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
            continue;

        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, target_struct_obj.source_name, true, 0, 0, false);

        create_dt_search_index_for_every_source(target_struct_obj.source_count_name);

        value_list.clear();
        value_list << QString::number(0) << source_count_name;

        command = QString("Select file_path From files Where status = ? AND source_count_name = ?");
        QStringList file_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, indexing_db_path,Q_FUNC_INFO);

        //If root dir is selected then process all files
        bool bool_is_running_on_root = false;

        QString total_records_str;
        qint64  total_records_num = 0;

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        if(target_struct_obj.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                || target_struct_obj.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
        {
            QStringList tmp_filepath_list;

            /*======================Note(iOS)==============================
             * if selected files already contains root for same source,
             * then no need to process all files simultaneously, just
             * process root only, all files will already be included in root.
             */

            if(file_path_list.contains(QString("/")))
            {
                bool_is_running_on_root = true;

                QStringList id_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select id_path from files"), 0, fs_db_path, Q_FUNC_INFO);

                for(int num = 0; num < id_path_list.size(); num++)
                {
                    if(bool_cancel_extraction)
                        break;

                    QString id_path_str = id_path_list.at(num).trimmed();

                    if(id_path_str.isEmpty())
                        continue;

                    tmp_filepath_list << id_path_str;
                    total_records_num += num;
                }
            }
            else
            {
                //Process individual directory/files
                for(int count = 0; count < file_path_list.size(); count++)
                {
                    if(bool_cancel_extraction)
                        break;

                    QString file_path_str = file_path_list.at(count).trimmed();
                    if(file_path_str.isEmpty())
                        continue;

                    //check for directory/file
                    QString flag_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select flags From files Where filepath = ?"), QStringList(file_path_str), 0, fs_db_path, Q_FUNC_INFO);
                    flag_str = flag_str.trimmed();
                    if(flag_str.isEmpty())
                        continue;

                    if(flag_str == QString::number(2))
                    {
                        //directory
                        QStringList id_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select id_path from files Where filepath LIKE ?") , QStringList(file_path_str + QString("%")), 0, fs_db_path, Q_FUNC_INFO);

                        bool bool_any_entry_found = false;
                        for(int num = 0; num < id_path_list.size(); num++)
                        {
                            if(bool_cancel_extraction)
                                break;

                            QString id_path_str = id_path_list.at(num).trimmed();

                            if(id_path_str.isEmpty())
                                continue;

                            bool_any_entry_found = true;

                            tmp_filepath_list << id_path_str + QString(MACRO_RECON_Splitter_1_multiple) + file_path_str;
                            total_records_num += num;

                        }

                        if(!bool_any_entry_found)
                            tmp_filepath_list << file_path_str;
                    }
                    else
                    {
                        //file

                        total_records_num += 1;

                        QString id_path_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select id_path from files Where filepath = ?"), QStringList(file_path_str), 0, fs_db_path, Q_FUNC_INFO);
                        id_path_str =  id_path_str.trimmed();
                        if(id_path_str.isEmpty())
                            id_path_str = file_path_str;

                        tmp_filepath_list << id_path_str + QString(MACRO_RECON_Splitter_1_multiple) + file_path_str;
                    }
                }
            }


            file_path_list.clear();
            file_path_list = tmp_filepath_list;
        }
        else
        {
            /*======================Note(Non-iOS)==============================
             * if selected files already contains root for same source,
             * then no need to process all files simultaneously, just
             * process root only, all files will already be included in root.
             */

            if(file_path_list.contains(QString("/")))
            {
                file_path_list.clear();
                value_list.clear();

                command = QString("Select filepath From files Where file_parent_path = ?");
                value_list << QString("/");
                file_path_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, fs_db_path,Q_FUNC_INFO);

                bool_is_running_on_root = true;

                total_records_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select count(*) from files Where source_count_name = ?") , QStringList(source_count_name), 0, fs_db_path, Q_FUNC_INFO);
                total_records_num = total_records_str.toLongLong();

            }
            else
            {
                //Process individual directory/files
                for(int count = 0; count < file_path_list.size(); count++)
                {
                    if(bool_cancel_extraction)
                        break;

                    QString file_path_str = file_path_list.at(count).trimmed();
                    if(file_path_str.isEmpty())
                        continue;

                    QString file_full_path = recon_static_functions::get_complete_file_path(target_struct_obj.fs_extraction_source_path, file_path_str, Q_FUNC_INFO);

                    bool bool_is_dir = recon_helper_standard_obj->pub_check_path_is_directory_by_stat(file_full_path, Q_FUNC_INFO);
                    if(bool_is_dir)
                    {
                        value_list.clear();
                        value_list << file_path_str << source_count_name;
                        total_records_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select count(*) from files Where file_parent_path = ? AND source_count_name = ?") , value_list, 0, fs_db_path, Q_FUNC_INFO);
                        total_records_num += total_records_str.toLongLong();
                    }
                    else
                    {
                        total_records_num += 1;
                    }
                }
            }
        }


        if(bool_is_running_on_root)
            command = QString("Update files set status = ? Where source_count_name = ?");
        else
            command = QString("Update files set status = ? Where file_path = ? AND source_count_name = ?");


        ///create text file of all selected files
        QString tmp_index_filepath_list_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() +  QString("Index_Files/dt_index_filepath.txt");

        QFile dt_file(tmp_index_filepath_list_file);
        if(dt_file.exists())
            dt_file.remove();

        if(!dt_file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            recon_static_functions::app_debug(" File Open -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
            continue;
        }

        ///=========================

        int tmp_pos = 0;
        total_records_num = 0;

        QStringList chunk_list;
        for(int count = 0; count < file_path_list.size(); count++)
        {
            if(bool_cancel_extraction)
                break;

            chunk_list << file_path_list.at(count);
            tmp_pos++;

            ///devide files in chunks of specified size
            if(tmp_pos == files_chunk_size || count == (file_path_list.size() - 1))
            {
                for(int count = 0; count < chunk_list.size(); count++)
                {
                    if(bool_cancel_extraction)
                        break;

                    qint64 records_count = 0;

                    QString file_path_str = chunk_list.at(count);
                    if(file_path_str.trimmed().isEmpty())
                        continue;

                    QStringList path_split_list;
                    if(file_path_str.contains(QString(MACRO_RECON_Splitter_1_multiple)))
                    {
                        path_split_list = file_path_str.split(QString(MACRO_RECON_Splitter_1_multiple));

                        // for indexing
                        file_path_str = path_split_list.at(0);
                    }

                    QString file_full_path;
                    QString record_no;

                    if(target_struct_obj.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                            || target_struct_obj.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                    {
                        record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select INT from files Where id_path = ?"), QStringList(file_path_str), 0, fs_db_path, Q_FUNC_INFO);
                    }
                    else
                        record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select INT from files Where filepath = ?"), QStringList(file_path_str), 0, fs_db_path, Q_FUNC_INFO);

                    if(!record_no.trimmed().isEmpty() && record_no.toLongLong() > 0)
                        file_full_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);


                    ///for ios empty directory
                    if(!QFileInfo(file_full_path).exists())
                    {
                        value_list.clear();
                        if(bool_cancel_extraction)
                            value_list << QString::number(0) << file_path_str << source_count_name;
                        else
                            value_list << QString::number(1) << file_path_str << source_count_name;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);

                        continue;
                    }


                    ///extract files from directories and write in text file
                    bool bool_is_dir = recon_helper_standard_obj->pub_check_path_is_directory_by_stat(file_full_path, Q_FUNC_INFO);

                    if(bool_is_dir)
                    {
                        if(!bool_is_running_on_root)
                        {
                            dt_file.close();
                            dt_file.setFileName(tmp_index_filepath_list_file);
                            if(dt_file.exists())
                                dt_file.remove();

                            if(!dt_file.open(QIODevice::WriteOnly | QIODevice::Append))
                            {
                                recon_static_functions::app_debug(" File Open -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
                                recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
                                continue;
                            }
                        }

                        ///works for root(/) or selected directories
                        int tmp_cnt = 0;
                        QDirIterator it(file_full_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
                        while (it.hasNext())
                        {
                            if(bool_cancel_extraction)
                                break;

                            QString filepath = it.next();

                            QString filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(filepath);
                            bool bool_is_file = recon_helper_standard_obj->pub_check_path_is_file_by_stat(filepath, Q_FUNC_INFO);

                            if(filename_str == (".") || filename_str == ("..") || !bool_is_file)
                                continue;

                            QString m_suffix = recon_helper_standard_obj->mining_suffix(filepath, Q_FUNC_INFO);

                            if(!m_suffix.startsWith(".") && !m_suffix.trimmed().isEmpty())
                                m_suffix.prepend(".");

                            tmp_cnt++;

                            if(bool_whitelist_check_status)
                            {
                                //index these files only
                                if(whitelist_extensions_list.contains(m_suffix))
                                {
                                    records_count++;
                                    dt_file.write(filepath.toLocal8Bit().data());
                                    dt_file.write(QString("\n").toLocal8Bit().data());
                                }
                            }
                            else if(bool_blacklist_check_status)
                            {
                                //do not index these files
                                if(!blacklist_extensions_list.contains(m_suffix))
                                {
                                    records_count++;
                                    dt_file.write(filepath.toLocal8Bit().data());
                                    dt_file.write(QString("\n").toLocal8Bit().data());
                                }
                            }
                            else
                            {
                                records_count++;
                                dt_file.write(filepath.toLocal8Bit().data());
                                dt_file.write(QString("\n").toLocal8Bit().data());
                            }

                            ///process files in chunks(not for root(/))
                            if(tmp_cnt == files_chunk_size && !bool_is_running_on_root)
                            {
                                dt_file.flush();
                                dt_file.close();

                                current_label_in_task_status = target_struct_obj.source_name + QString(" - ")  + recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(file_full_path);
                                current_source_total_records = records_count;
                                slot_on_timer_tick();

                                timer = new QTimer(this);
                                connect(timer, SIGNAL(timeout()), this, SLOT(slot_on_timer_tick()));
                                timer->start(1000);

                                dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);

                                index_file_content_using_dt_search(dt_file.fileName());

                                timer->stop();
                                if(timer != NULL)
                                    delete timer;

                                value_list.clear();

                                if(bool_cancel_extraction)
                                    value_list << QString::number(0) << file_path_str << source_count_name;
                                else
                                    value_list << QString::number(1) << file_path_str << source_count_name;

                                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);


                                dt_file.setFileName(tmp_index_filepath_list_file);
                                if(dt_file.exists())
                                    dt_file.remove();

                                if(!dt_file.open(QIODevice::WriteOnly | QIODevice::Append))
                                {
                                    recon_static_functions::app_debug(" File Open -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
                                    recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
                                    continue;
                                }
                            }
                        }

                        total_records_num += records_count;

                        ///process remaining files that were less than chunk size
                        if(!bool_is_running_on_root)
                        {
                            dt_file.flush();
                            dt_file.close();

                            global_dt_indexing_processed_files_count = 0;

                            current_label_in_task_status = target_struct_obj.source_name + QString(" - ")  + recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(file_full_path);
                            current_source_total_records = records_count;
                            slot_on_timer_tick();

                            timer = new QTimer(this);
                            connect(timer, SIGNAL(timeout()), this, SLOT(slot_on_timer_tick()));
                            timer->start(1000);

                            dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);

                            index_file_content_using_dt_search(dt_file.fileName());

                            timer->stop();
                            if(timer != NULL)
                                delete timer;

                            value_list.clear();

                            if(bool_cancel_extraction)
                                value_list << QString::number(0) << file_path_str << source_count_name;
                            else
                                value_list << QString::number(1) << file_path_str << source_count_name;

                            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);

                        }
                    }
                    else
                    {
                        ///write filepaths in text file if not directory.
                        if(!bool_is_running_on_root)
                        {
                            dt_file.close();
                            dt_file.setFileName(tmp_index_filepath_list_file);
                            if(dt_file.exists())
                                dt_file.remove();

                            if(!dt_file.open(QIODevice::WriteOnly | QIODevice::Append))
                            {
                                recon_static_functions::app_debug(" File Open -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
                                recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
                                continue;
                            }
                        }

                        QString filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(file_full_path);

                        QString full_path_for_suffix;

                        if(target_struct_obj.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                                || target_struct_obj.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                        {
                            QString id_path = file_full_path;
                            if(id_path.startsWith(target_struct_obj.fs_extraction_source_path))
                                id_path.remove(0, target_struct_obj.fs_extraction_source_path.size());

                            QString filepath_str_ios = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select filepath from files where id_path = ?"), QStringList(id_path), 0, fs_db_path, Q_FUNC_INFO);
                            full_path_for_suffix = filepath_str_ios;
                        }
                        else
                        {
                            full_path_for_suffix = file_full_path;
                        }

                        QString m_suffix = recon_helper_standard_obj->mining_suffix(full_path_for_suffix, Q_FUNC_INFO);

                        if(!m_suffix.startsWith(".") && !m_suffix.trimmed().isEmpty())
                            m_suffix.prepend(".");


                        if(bool_whitelist_check_status)
                        {
                            //index these files only
                            if(whitelist_extensions_list.contains(m_suffix))
                            {
                                records_count++;
                                dt_file.write(file_full_path.toLocal8Bit().data());
                                dt_file.write(QString("\n").toLocal8Bit().data());
                            }
                        }
                        else if(bool_blacklist_check_status)
                        {
                            //do not index these files
                            if(!blacklist_extensions_list.contains(m_suffix))
                            {
                                records_count++;
                                dt_file.write(file_full_path.toLocal8Bit().data());
                                dt_file.write(QString("\n").toLocal8Bit().data());
                            }
                        }
                        else
                        {
                            records_count++;
                            dt_file.write(file_full_path.toLocal8Bit().data());
                            dt_file.write(QString("\n").toLocal8Bit().data());
                        }

                        total_records_num += records_count;

                        ///Process non root in chunks
                        if(!bool_is_running_on_root)
                        {
                            dt_file.flush();
                            dt_file.close();

                            if(path_split_list.size() > 1)
                            {
                                // for db updation
                                file_path_str =  path_split_list.at(1);
                            }

                            global_dt_indexing_processed_files_count = 0;

                            current_label_in_task_status = target_struct_obj.source_name + QString(" - ") + filename_str;
                            current_source_total_records = records_count;
                            slot_on_timer_tick();

                            timer = new QTimer(this);
                            connect(timer, SIGNAL(timeout()), this, SLOT(slot_on_timer_tick()));
                            timer->start(1000);

                            dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);

                            index_file_content_using_dt_search(dt_file.fileName());

                            timer->stop();
                            if(timer != NULL)
                                delete timer;

                            value_list.clear();

                            if(bool_cancel_extraction)
                                value_list << QString::number(0) << file_path_str << source_count_name;
                            else
                                value_list << QString::number(1) << file_path_str << source_count_name;

                            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
                        }
                    }
                }


                chunk_list.clear();
                tmp_pos = 0;
            }
        }

        ///=========================


        ///Process root only in chunks
        if(bool_is_running_on_root)
        {
            dt_file.flush();
            dt_file.close();

            global_dt_indexing_processed_files_count = 0;
            current_label_in_task_status = target_struct_obj.source_name + QString(" - ") + QString("/");
            current_source_total_records = total_records_num;
            slot_on_timer_tick();

            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(slot_on_timer_tick()));
            timer->start(1000);

            dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);

            ///====Create temporary chunk file====///
            QString tmp_chunk_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() +  QString("Index_Files/dt_tmp_chunk_filepath.txt");
            QFile dt_chunk_file(tmp_chunk_file_path);
            if(dt_chunk_file.exists())
                dt_chunk_file.remove();

            if(!dt_chunk_file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                recon_static_functions::app_debug(" File Open(chunk) -----FAILED-----:" + dt_chunk_file.fileName(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error:" + dt_chunk_file.errorString(), Q_FUNC_INFO);
                continue;
            }

            dt_file.setFileName(dt_file.fileName());
            if(!dt_file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug(" File Open(read) -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
                continue;
            }

            int tmp_cnt = 0;
            while(!dt_file.atEnd())
            {

                QByteArray line_arr = dt_file.readLine();

                dt_chunk_file.write(line_arr);
                dt_chunk_file.flush();

                tmp_cnt++;
                if(tmp_cnt == files_chunk_size)
                {
                    dt_chunk_file.close();
                    index_file_content_using_dt_search(dt_chunk_file.fileName());


                    ///clear all previous data
                    tmp_cnt = 0;

                    dt_chunk_file.setFileName(tmp_chunk_file_path);
                    if(dt_chunk_file.exists())
                        dt_chunk_file.remove();

                    if(!dt_chunk_file.open(QIODevice::WriteOnly | QIODevice::Append))
                    {
                        recon_static_functions::app_debug(" File Open(reopen_chunk) -----FAILED-----:" + dt_chunk_file.fileName(), Q_FUNC_INFO);
                        recon_static_functions::app_debug(" Error:" + dt_chunk_file.errorString(), Q_FUNC_INFO);
                        continue;
                    }
                }
            }
            dt_file.close();


            ///index remaining files
            index_file_content_using_dt_search(dt_chunk_file.fileName());

            ///delete temp chunk file
            dt_chunk_file.remove();

            timer->stop();
            if(timer != NULL)
                delete timer;

            value_list.clear();

            if(bool_cancel_extraction)
                value_list << QString::number(0) <<  source_count_name;
            else
                value_list << QString::number(1) <<  source_count_name;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
        }


        ///delete dt all filepath txt file
        dt_file.remove();

        if(!bool_cancel_extraction && bool_is_running_on_root)
            emit signal_completion_status(MACRO_JobType_Text_Indexing, target_struct_obj.source_count_name, Macro_JobType_Completion_status_done);

    }

}

void thread_index_files::slot_on_timer_tick()
{ // set timer to display that how much percentage of text indexing completed
    if(bool_cancel_extraction)
    {
        dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);
    }

    qint64 records_count = dt_search_interface_obj->pub_get_processed_files_count();

    if(current_source_total_records < 1)
        return;

    qint64 progress_val = ((records_count * 100) / current_source_total_records);
    if(progress_val > 99)
        progress_val = 99;

    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, current_label_in_task_status, true, records_count, progress_val, false);
}


void thread_index_files::index_files_for_carved_files()
{ // Text Indexing runs on selected file for carved files extraction
    ///get source count from selected files
    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";


    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(indexing_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Indexing Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_indexing(destination_db);

    query_indexing.prepare("Select fs_record,file_path,source_count_name from files Where status = ? AND plugin_name = ?");
    query_indexing.addBindValue(QString(MACRO_FALSE_VALUE_NUMERIC_QString));
    query_indexing.addBindValue(QString(MACRO_Plugin_Name_Carved_Files));

    if(!query_indexing.exec())
    {
        recon_static_functions::app_debug("query exec --------FAILED------ " + query_indexing.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_indexing.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;

    }

    struct struct_carved_files_indexing
    {
        QString filepath;
        QString complete_filepath;
        QString source_count_name;
    };
    QList <struct_carved_files_indexing> carved_files_indexing_list;


    QString command_update = QString("Update files set status = ? Where file_path = ?");
    QSqlQuery query_update(destination_db);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QStringList processed_source_count_list;
    while(query_indexing.next())
    {
        emit signal_PBAR_thread_file_system_value(MACRO_JobType_Text_Indexing, QString(MACRO_Plugin_Name_Carved_Files), true, 0, 0, false);

        QString file_path = query_indexing.value("file_path").toString().trimmed();
        QString source_count_name = query_indexing.value("source_count_name").toString().trimmed();

        QString complete_file_path = result_dir_path + file_path;

        if(!processed_source_count_list.contains(source_count_name))
        {
            processed_source_count_list << source_count_name;
            create_dt_search_index_for_every_source(source_count_name);
        }

        struct_carved_files_indexing files_obj;
        files_obj.filepath = file_path;
        files_obj.complete_filepath = complete_file_path;
        files_obj.source_count_name = source_count_name;

        carved_files_indexing_list.append(files_obj);
    }

    ///create text file of all selected files
    QString tmp_index_filepath_list_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() +  QString("Index_Files/dt_index_filepath.txt");

    QFile dt_file(tmp_index_filepath_list_file);
    if(dt_file.exists())
        dt_file.remove();

    if(!dt_file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        recon_static_functions::app_debug(" File Open -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
        return;
    }

    ///=========================

    int tmp_pos = 0;
    qint64  total_records_num = 0;
    int files_chunk_size = 50000; //fifty thousands

    global_dt_indexing_processed_files_count = 0;

    QList <struct_carved_files_indexing> carved_files_indexing_chunk_list;
    for(int count_a = 0; count_a < carved_files_indexing_list.size(); count_a++)
    {
        if(bool_cancel_extraction)
            break;

        carved_files_indexing_chunk_list.append(carved_files_indexing_list.at(count_a));
        tmp_pos++;

        ///devide files in chunks of specified size
        if(tmp_pos == files_chunk_size || count_a == (carved_files_indexing_list.size() - 1))
        {
            for(int count_b = 0; count_b < carved_files_indexing_chunk_list.size(); count_b++)
            {
                if(bool_cancel_extraction)
                    break;

                qint64 records_count = 0;

                struct_carved_files_indexing files_obj = carved_files_indexing_chunk_list.at(count_b);

                QString file_path_str = files_obj.complete_filepath;

                QString m_suffix = recon_helper_standard_obj->mining_suffix(file_path_str, Q_FUNC_INFO);

                if(!m_suffix.startsWith(".") && !m_suffix.trimmed().isEmpty())
                    m_suffix.prepend(".");

                if(bool_whitelist_check_status)
                {
                    //index these files only
                    if(whitelist_extensions_list.contains(m_suffix))
                    {
                        records_count++;
                        dt_file.write(file_path_str.toLocal8Bit().data());
                        dt_file.write(QString("\n").toLocal8Bit().data());
                    }
                }
                else if(bool_blacklist_check_status)
                {
                    //do not index these files
                    if(!blacklist_extensions_list.contains(m_suffix))
                    {
                        records_count++;
                        dt_file.write(file_path_str.toLocal8Bit().data());
                        dt_file.write(QString("\n").toLocal8Bit().data());
                    }
                }
                else
                {
                    records_count++;
                    dt_file.write(file_path_str.toLocal8Bit().data());
                    dt_file.write(QString("\n").toLocal8Bit().data());
                }

                total_records_num += records_count;

                query_update.prepare(command_update);
                query_update.addBindValue(QString(MACRO_TRUE_VALUE_NUMERIC_QString));
                query_update.addBindValue(files_obj.filepath);

                if(!query_update.exec())
                {
                    recon_static_functions::app_debug("update query exec --------FAILED------ " + query_update.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("error -  " + query_update.lastError().text(),Q_FUNC_INFO);
                }

            }

            dt_file.flush();
            dt_file.close();

            //current_label_in_task_status = target_struct_obj.source_name + QString(" - ") + filename_str;
            current_label_in_task_status = QString(MACRO_Plugin_Name_Carved_Files);
            current_source_total_records = carved_files_indexing_list.size();
            slot_on_timer_tick();

            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(slot_on_timer_tick()));
            timer->start(1000);

            dt_search_interface_obj->pub_set_bool_cancel_extraction(bool_cancel_extraction);

            index_file_content_using_dt_search(dt_file.fileName());

            timer->stop();
            if(timer != NULL)
                delete timer;

            carved_files_indexing_chunk_list.clear();
            tmp_pos = 0;

            dt_file.setFileName(tmp_index_filepath_list_file);
            if(dt_file.exists())
                dt_file.remove();

            if(!dt_file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                recon_static_functions::app_debug(" File Open -----FAILED-----:" + dt_file.fileName(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error:" + dt_file.errorString(), Q_FUNC_INFO);
                continue;
            }
        }
    }

    ///=========================

    ///delete dt all filepath txt file
    dt_file.flush();
    dt_file.close();
    dt_file.remove();

    destination_db.close();

}

