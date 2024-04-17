#include "thread_content_search.h"

thread_content_search::thread_content_search(QWidget *parent)
{
    bool_cancel_extraction = false;
    timer_records_fs = new QElapsedTimer();
    total_records = 0;
    total_size_processed = 0;
    size_divided_value = 0;
    thread_index_files_obj = new thread_index_files(this);
    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_content_search::~thread_content_search()
{
    if(timer_records_fs != NULL)
        delete timer_records_fs;
}

void thread_content_search::run()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    total_size_processed = 0;
    size_divided_value = 0;
    total_records = 0;

    timer_records_fs->start();

    list_struct_found_keyword_file_info.clear();

    insert_entry_in_index_db();

    QString destination_db_path = create_content_search_destination_db();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("destination_db Db opening --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    directory_items_count = 0;

    struct struct_global_task_type_and_title t1;
    t1.title = search_label_name;
    t1.type = MACRO_JobType_Content_Search;

    ///here should be number of files in directory - dir_path_to_temp_indexing instead of 100
    //emit signal_set_progress_bar_max_value(t1,100);
    //emit signal_set_progressbar_value(0 , QString(MACRO_JobType_Content_Search));


    if(bool_search_in_temp_mode)
    {
        /* ==================================================================================
         * Note: We are not considering "/" i.e slash only case because we will be working
         * on mounted images most.
         * ==================================================================================
         */


        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
        QString fs_extrcn_source_path = struct_source_info.fs_extraction_source_path;

        QString current_dir_path = dir_path_to_temp_indexing;

        if(current_dir_path.startsWith(fs_extrcn_source_path ))
        {
            current_dir_path.remove(0,fs_extrcn_source_path.size());
        }

        if((!current_dir_path.trimmed().isEmpty() && !current_dir_path.startsWith("/")) || dir_path_to_temp_indexing == fs_extrcn_source_path)
            current_dir_path.prepend("/");


        QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()  + source_count_name_str + "/file_system.sqlite";

        QString command = "select INT from files where filepath LIKE ?";
        QStringList values;
        values << QString(current_dir_path + "%");

        QStringList filesystem_record_list;
        filesystem_record_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,values, 0, dbpath, Q_FUNC_INFO);

        QString is_indexed;

        /// check for index files db empty
        QString index_files_dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()  + source_count_name_str + "/index_files.sqlite";
        command = QString("Select count(*) from files");
        QString record_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command , 0, index_files_dbpath, Q_FUNC_INFO);

        bool is_index_files_db_empy = false;

        if(record_count.isEmpty() || record_count == QString::number(0))
            is_index_files_db_empy = true;

        command = QString("Select is_file_indexed from files Where filesystem_record = ?");

        if(filesystem_record_list.size() > 0 && !is_index_files_db_empy)
        {
            QString record_str = filesystem_record_list.at(1);
            values.clear();
            values << record_str;
            is_indexed = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , values, 0, index_files_dbpath, Q_FUNC_INFO);
        }

        if(is_indexed != "1")
        {
            thread_index_files_obj->pub_create_dt_search_index_for_every_source(source_count_name_str);
            iterate_directory_recursively_and_index_files(dir_path_to_temp_indexing);
            dt_search_interface_obj->pub_set_list_found_word_file_info_ptr(&list_struct_found_keyword_file_info);
        }
    }

    destination_db->transaction();
    search_for_keywords_in_indexed_data(*destination_db);
    destination_db->commit();

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}


void thread_content_search::search_for_keywords_in_indexed_data(QSqlDatabase &destination_db)
{
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString carved_files_dir_name = QString(MACRO_Plugin_Name_Carved_Files).replace(" ", "_");
    QString lab_features_dir_name = QString("Lab_Features");

    qint64 records_count = 0;
    emit signal_set_search_label_with_records_in_progress_window(QString(MACRO_JobType_Content_Search), search_label_name, true, 0, -1, true);

    for(int i = 0; i < keywords_list_to_search.size(); i++)
    {
        if(bool_cancel_extraction)
            break;

        list_struct_found_keyword_file_info.clear();

        QString keyword = keywords_list_to_search.at(i);

        // Search Keyword using DT Search
        thread_index_files_obj->pub_dt_search_index(keyword);

        for(int j = 0; j < list_struct_found_keyword_file_info.size(); j++)
        {
            if(bool_cancel_extraction)
                break;

            struct_global_indexed_keyword_found_file_info struct_file_info = list_struct_found_keyword_file_info.at(j);

            QString source_count_name;
            QStringList tmp_list = struct_file_info.index_path.split("/", Qt::SkipEmptyParts);
            if(tmp_list.size() > 1)
                source_count_name = tmp_list.at(tmp_list.size() - 2);

            ///=========For Carved Files-Start===========///
            if(struct_file_info.file_path.contains(carved_files_dir_name))
            {
                QString filepath =  struct_file_info.file_path;
                int pos = filepath.indexOf(lab_features_dir_name);
                if(pos < 0)
                    pos = 0;
                else
                    pos -= 1;

                filepath = filepath.mid(pos, filepath.size());

                ///==========Check for Record Already Exist==========///
                qint64 no_of_hits = struct_file_info.word_hit_location_list.size();
                QString cmd_check = QString("Select no_of_hits from files Where file_path = ? AND keyword_name = ?");
                QStringList val_list;
                val_list << filepath << keyword;
                QString previous_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_check, val_list, 0, destination_db, Q_FUNC_INFO);
                previous_count = previous_count.trimmed();
                if(!previous_count.isEmpty())
                {
                    no_of_hits += previous_count.toInt();
                    cmd_check = QString("Update files Set no_of_hits = ? Where file_path = ? AND keyword_name = ?");
                    val_list.prepend(QString::number(no_of_hits));
                    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_check,val_list, destination_db, Q_FUNC_INFO))
                    {
                        recon_static_functions::app_debug("Search Db UPDATION --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                    }
                    continue;
                }

                QStringList arg_list;
                arg_list << MACRO_Plugin_Name_Carved_Files << MACRO_Plugin_Carved_Files_TAB_Name_Files << "files"
                         << struct_file_info.file_name << filepath
                         << QString::number(struct_file_info.word_hit_location_list.size()) << keyword
                         << QString::number(struct_file_info.file_total_word_count)
                         << struct_file_info.word_hit_location_list.join(",") << struct_file_info.index_path
                         << source_count_name ;

                QString command = "INSERT INTO files(plugin_name, tab_name, table_name, file_name, file_path, no_of_hits, keyword_name,"
                                  " total_word_count, hit_locations, index_path, source_count_name) VALUES (?,?,?,?,?,?, ?,?,?,?,?)";
                if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list, destination_db, Q_FUNC_INFO))
                {
                    recon_static_functions::app_debug("Search Db INSERTION --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                }

                records_count++;

                emit signal_set_search_label_with_records_in_progress_window(QString(MACRO_JobType_Content_Search), search_label_name, true, records_count, -1, true);

                continue;
            }
            ///=========For Carved Files-End===========///

            struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString file_path = struct_file_info.file_path;

            if(struct_target_info.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                    || struct_target_info.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
            {
                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";


                if(file_path.startsWith(struct_target_info.virtual_source_path))
                    file_path.remove(0, struct_target_info.virtual_source_path.size());

                if(!file_path.isEmpty() && !file_path.startsWith("/"))
                    file_path.prepend("/");

                file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select filepath from files where id_path = ?"), QStringList(file_path), 0, fs_db_path, Q_FUNC_INFO);

                struct_file_info.file_path = file_path;
                struct_file_info.file_name = QFileInfo(file_path).fileName();
            }


            //DT Library Create the file(.ix) which stores the complete filepath of indexed files including mount path
            //RECON Result DT Files Path - /Lab_Features/File_System/Source1/dt_search_index/index_a.ix
            //So to point these file we have to remove the mount path at time of load case
            //To remove the mount path we have to comapre them with different static path because of different RECON LAB Result Versions
            //Thats why we remove the path from the list splitted by '/' from 0 to N.. (depends on RECON LAB Result versions)

            QString mount_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString).toString();

            QString previous_mounting_path = ("/RECON_TMP/RECON_mount_path/RECON_MNT_disk");
            if(file_path.contains(previous_mounting_path))
            {
                QStringList filepath_list;
                filepath_list = file_path.split("/");

                if(filepath_list.size() > 6)
                {
                    for(int count = 0; count < 6 ; count++)
                    {
                        filepath_list.removeAt(0);
                    }
                }
                file_path = filepath_list.join("/");

                file_path = get_filepath_for_tar_rar_gz_and_zip_files(file_path);
            }
            else if(file_path.startsWith(result_dir_path))
            {
                file_path.remove(0, result_dir_path.size());

                //result path is the extracted content part which is created by recon lab for sources such as cellebrite tar, ab andriod etc.
                QString source_path1 = struct_target_info.source_path_1;
                QString source_name = QDir(source_path1).dirName();

                if(source_name.endsWith(".zip",Qt::CaseInsensitive))
                {
                    source_name.chop(QString(".zip").size());
                }
                else if(source_name.endsWith(".ab",Qt::CaseInsensitive))
                {
                    source_name.chop(QString(".ab").size());
                }
                else if(source_name.endsWith(".tar",Qt::CaseInsensitive))
                {
                    source_name.chop(QString(".tar").size());
                }
                else if(source_name.endsWith(".ufdr",Qt::CaseInsensitive))
                {
                    source_name.chop(QString(".ufdr").size());
                }
                source_name = source_name.replace(" ","_");

                if(file_path.contains(source_name))
                {
                    QStringList tmp_filepath = file_path.split(source_name);
                    if(tmp_filepath.size() > 1)
                    {
                        file_path = tmp_filepath.at(1);
                    }
                }

                file_path = get_filepath_for_tar_rar_gz_and_zip_files(file_path);
            }
            else if(file_path.startsWith("/private/var/tmp/RECON_LAB_"))
            {
                //Remove virtual source path
                //file_path Input Path(8)  : /private/var/tmp/RECON_LAB_hawk/1584110933/RECON_mount_path/RECON_MNT_disk14_small_image.E01/images/910116939_builtin_carver.jpg"
                //file_path Output Path : /images/910116939_builtin_carver.jpg

                //OR

                //file_path Input Path(10)  : /private/var/tmp/RECON_LAB_karan/1657523109/RECON_mount_path/RECON_MNT_disk6s2_time_machine_img_June-06.dmg/2022-06-06-151008.previous/Macintosh\ HD\ -\ Data/Users/tester/Downloads/images.jpeg
                //file_path Output Path : /Users/tester/Downloads/images.jpeg

                QStringList file_path_list;
                file_path_list = file_path.split("/");

                if(struct_target_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
                {
                    if(file_path_list.size() > 10)
                    {
                        for(int count = 0; count < 10; count++)
                        {
                            file_path_list.removeAt(0);
                        }
                    }
                }
                else
                {
                    if(file_path_list.size() > 8)
                    {
                        for(int count = 0; count < 8 ; count++)
                        {
                            file_path_list.removeAt(0);
                        }
                    }
                }

                file_path = file_path_list.join("/");

                if(!file_path.startsWith("/"))
                    file_path.prepend("/");

                file_path = get_filepath_for_tar_rar_gz_and_zip_files(file_path);
            }
            else if(file_path.contains(mount_path))
            {
                //Remove virtual source path
                //file_path Input Path  : /private/var/tmp/RECON_hawk/RECON_mount_path/RECON_MNT_disk14_small_image.E01/images/910116939_builtin_carver.jpg"
                //file_path Output Path : /images/910116939_builtin_carver.jpg

                QStringList file_path_list;
                file_path_list = file_path.split("/");

                if(file_path_list.size() > 7)
                {
                    for(int count = 0; count < 7 ; count++)
                    {
                        file_path_list.removeAt(0);
                    }
                }
                file_path = file_path_list.join("/");

                if(!file_path.startsWith("/"))
                    file_path.prepend("/");

                file_path = get_filepath_for_tar_rar_gz_and_zip_files(file_path);
            }
            else
            {
                if(file_path.startsWith(struct_target_info.virtual_source_path))
                    file_path.remove(0, struct_target_info.virtual_source_path.size());

                if(!file_path.startsWith("/"))
                    file_path.prepend("/");

                // dont use with  else if condition
                if(file_path.startsWith(struct_target_info.fs_extraction_source_path))
                    file_path.remove(0, struct_target_info.fs_extraction_source_path.size());

                file_path = get_filepath_for_tar_rar_gz_and_zip_files(file_path);
            }

            if(!file_path.startsWith("/") && !file_path.trimmed().isEmpty())
                file_path.prepend("/");


            QString index_path = struct_file_info.index_path;
            if(index_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
                index_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());

            if(!index_path.startsWith("/"))
                index_path.prepend("/");



            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
            QString command_fs = "select INT from files where filepath = ?";
            QStringList v_args;
            v_args << file_path;
            QString file_system_record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_fs, v_args, 0, fs_db_path, Q_FUNC_INFO);
            file_system_record = file_system_record.trimmed();
            if(file_system_record.isEmpty())
            {
                command_fs = "select INT from files where recon_file_infopath = ?";
                file_system_record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_fs, v_args, 0, fs_db_path, Q_FUNC_INFO);
                file_system_record = file_system_record.trimmed();
            }
            if(file_system_record.isEmpty())
                continue;

            ///==========Check for Record Already Exist==========///
            qint64 no_of_hits = struct_file_info.word_hit_location_list.size();
            QString cmd_check = QString("Select no_of_hits from files Where filesystem_record = ? AND keyword_name = ?");
            QStringList val_list;
            val_list << file_system_record << keyword;
            QString previous_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_check, val_list, 0, destination_db, Q_FUNC_INFO);
            previous_count = previous_count.trimmed();
            if(!previous_count.isEmpty())
            {
                no_of_hits += previous_count.toInt();
                cmd_check = QString("Update files Set no_of_hits = ? Where filesystem_record = ? AND keyword_name = ?");
                val_list.prepend(QString::number(no_of_hits));
                if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_check,val_list, destination_db, Q_FUNC_INFO))
                {
                    recon_static_functions::app_debug("Search Db UPDATION --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                }
                continue;
            }


            QStringList arg_list;
            arg_list << MACRO_Plugin_Name_File_System << MACRO_Plugin_File_System_TAB_Name_Files << "files"
                     << struct_file_info.file_name << file_path
                     << QString::number(struct_file_info.word_hit_location_list.size()) << keyword
                     << QString::number(struct_file_info.file_total_word_count)
                     << struct_file_info.word_hit_location_list.join(",") << index_path
                     << source_count_name << struct_target_info.os_scheme_display;

            QString command = "INSERT INTO files(filesystem_record, plugin_name, tab_name, table_name, file_name, file_path, no_of_hits, keyword_name, total_word_count, hit_locations, index_path, source_count_name, os_scheme_display) VALUES (?,?,?,?,?,?, ?,?,?,?,?,?,?)";
            arg_list.prepend(file_system_record);
            if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list, destination_db, Q_FUNC_INFO))
            {
                recon_static_functions::app_debug("Search Db INSERTION --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
            }

            records_count++;

            emit signal_set_search_label_with_records_in_progress_window(QString(MACRO_JobType_Content_Search), search_label_name, true, records_count, -1, true);

        }
    }

}

QString thread_content_search::get_filepath_for_tar_rar_gz_and_zip_files(QString file_path)
{
    //RECON_LAB.zip/RECON_LAB/z_mainwindow/load_case"  = if path contains .zip/,.rar/ or .tar/
    //than remove path after .zip/, .rar/ or .tar/ to display
    if(file_path.contains(".zip/"))
    {
        QStringList tmp_filepath_list = file_path.split(".zip/");
        if(tmp_filepath_list.size() > 0)
        {
            QString tmp_filepath_str = tmp_filepath_list.at(0);
            file_path = tmp_filepath_str + ".zip";
        }
    }
    else if(file_path.contains(".rar/"))
    {
        QStringList tmp_filepath_list = file_path.split(".rar/");
        if(tmp_filepath_list.size() > 0)
        {
            QString tmp_filepath_str = tmp_filepath_list.at(0);
            file_path = tmp_filepath_str + ".rar";
        }
    }
    else if(file_path.contains(".tar/"))
    {
        QStringList tmp_filepath_list = file_path.split(".tar/");
        if(tmp_filepath_list.size() > 0)
        {
            QString tmp_filepath_str = tmp_filepath_list.at(0);
            file_path = tmp_filepath_str + ".tar";
        }
    }
    else if(file_path.contains(".gz/"))
    {
        QStringList tmp_filepath_list = file_path.split(".gz/");
        if(tmp_filepath_list.size() > 0)
        {
            QString tmp_filepath_str = tmp_filepath_list.at(0);
            file_path = tmp_filepath_str + ".gz";
        }
    }
    else
    {
        //else path does't ends with .zip/, .rar/ or .tar/ than no need to do anything
        file_path = file_path;
    }

    return file_path;
}

void thread_content_search::iterate_directory_recursively_and_index_files(QString source_path)
{
    return;

    if(!source_path.endsWith("/"))
        source_path.append("/");

    DIR *dir;
    struct dirent *dirent_obj;
    struct stat fileStat;
    bzero(&fileStat,sizeof(fileStat));
    if ((dir = opendir (source_path.toLocal8Bit().data())) != NULL)
    {
        while ((dirent_obj = readdir (dir)) != NULL)
        {
            if(bool_cancel_extraction)
            {
                closedir (dir);
                return;
            }

            QString path_inside_directory = recon_static_functions::get_complete_file_path(source_path ,QString(dirent_obj->d_name), Q_FUNC_INFO);

            if(QFileInfo(path_inside_directory).fileName() == QString(".") || QFileInfo(path_inside_directory).fileName() == QString(".."))
                continue;

            if(!lstat(path_inside_directory.toLocal8Bit().data(),&fileStat))
            {
                directory_items_count++;
                if(S_ISDIR(fileStat.st_mode))
                {
                    iterate_directory_recursively_and_index_files(path_inside_directory);
                }
                else
                {
                    total_records++;
                    total_size_processed += fileStat.st_size;

                    if(timer_records_fs->elapsed() > MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Extracted_Record_Timer_Interval_int)
                    {
                        timer_records_fs->restart();
                    }

                    if((total_size_processed / MACRO_File_System_Total_Size_Processed_Byte_int) > size_divided_value)
                    {
                        int size_processed = total_size_processed / MACRO_File_System_Total_Size_Processed_Byte_int;
                        emit signal_total_size_processed_of_index_search(size_processed, source_count_name_str);
                        size_divided_value++;
                    }

                    bool bool_update_index = thread_index_files_obj->pub_dt_update_index(path_inside_directory);

                    if(!bool_update_index)
                        recon_static_functions::app_debug("index file content -------FAILED------ " + path_inside_directory, Q_FUNC_INFO);
                    else
                        recon_static_functions::app_debug("Successfully update index for " + path_inside_directory, Q_FUNC_INFO);                }
            }
        }

        closedir (dir);
    }
    else
    {
        recon_static_functions::debug_intensive(" Directory Open -----FAILED---- " + source_path, Q_FUNC_INFO);
    }
}

void thread_content_search::pub_set_bool_cancel_extraction()
{
    bool_cancel_extraction = false;
}

void thread_content_search::cancel_extraction()
{
    bool_cancel_extraction = true;
}


void thread_content_search::pub_set_keywords_list_to_search(QStringList list)
{
    keywords_list_to_search = list;
}

void thread_content_search::pub_set_search_label_name(QString name)
{
    search_label_name = name;
}

void thread_content_search::pub_set_source_count_name(QString name)
{
    source_count_name_str = name;
}

void thread_content_search::pub_set_bool_search_in_temp_mode(bool bool_run_temp)
{
    bool_search_in_temp_mode = bool_run_temp;
}

void thread_content_search::pub_set_dir_path_to_temp_index(QString path)
{
    dir_path_to_temp_indexing = path;
}

QString thread_content_search::pub_get_search_label_name()
{
    return search_label_name;
}

void thread_content_search::pub_set_dt_search_obj(dt_search_interface *obj)
{
    dt_search_interface_obj = obj;
    dt_search_interface_obj->pub_set_list_found_word_file_info_ptr(&list_struct_found_keyword_file_info);
    thread_index_files_obj->pub_set_dt_serach_interface_obj(dt_search_interface_obj);
}

void thread_content_search::insert_entry_in_index_db()
{
    search_db_file_name = search_label_name;
    search_db_file_name = search_db_file_name.replace(" ", "_");

    QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";
    QString command = "INSERT INTO table_index_content_search(search_label_name, search_db_file_name, keywords_list,"
                      "db_creation_timestamp) "
                      "VALUES (?,?,?,?)";

    QStringList arg_list;
    arg_list << search_label_name << search_db_file_name << keywords_list_to_search.join(",") << QString::number(QDateTime::currentMSecsSinceEpoch()/1000);
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, arg_list, index_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("Search Index Db insertion --------FAILED------ " + index_db_path,Q_FUNC_INFO);
    }
}

QString thread_content_search::create_content_search_destination_db()
{
    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + search_db_file_name + ".sqlite";

    QString command = "CREATE TABLE files(INT INTEGER PRIMARY KEY, bookmark varchar(100), filesystem_record INTEGER, plugin_name varchar(500), tab_name varchar(500), table_name varchar(500), os_scheme_display varchar(500), file_name VARCHAR(1024), file_path VARCHAR(1024), no_of_hits INTEGER, keyword_name VARCHAR(1024), total_word_count INTEGER, hit_locations VARCHAR(1024), index_path VARCHAR(1024), source_count_name varchar(100), recon_tag_value varchar(100), notes varchar(100),notes_icon varchar(100))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, destination_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("Search Db creation --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        return "";
    }

    return destination_db_path;
}
