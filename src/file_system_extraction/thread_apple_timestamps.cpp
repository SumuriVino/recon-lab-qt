#include "thread_apple_timestamps.h"


thread_apple_timestamps::thread_apple_timestamps(QWidget *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    attributes_actual_key_list << "kMDItemUsedDates" << "kMDItemUseCount"
                               << "kMDItemLastUsedDate" << "kMDItemContentCreationDate"
                               << "kMDItemContentModificationDate" << "kMDItemDateAdded";

    attribute_column_list_in_apple_timestamp_db << "useddates" << "usecount"
                                                << "last_useddate" << "content_creation_date"
                                                << "content_modification_date" << "date_added";

    attribute_column_list_in_fs_db << "kMDItemUsedDates_Used_Dates" << "kMDItemUseCount_Used_Count"
                                   << "kMDItemLastUsedDate_Last_Opened_Date" << "kMDItemContentCreationDate_Created_Date"
                                   << "kMDItemContentModificationDate_Last_Modified_Date" << "kMDItemDateAdded_Date_Added";

}

thread_apple_timestamps::~thread_apple_timestamps()
{

}

void thread_apple_timestamps::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutext_for_fs_main_db = mutex;
}


void thread_apple_timestamps::run()
{ // extract apple timestamps and generate file system timelines from here which runs from file system post launcher
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    emit signal_PBAR_thread_apple_timestamp_started(MACRO_JobType_Extract_Apple_Timestamps);

    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int count = 0; count < list_target_source_info.size(); count++)
    {

        bool bool_need_to_fill_apple_timestamps_from_QMap = false;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        if(!source_selected_source_count_name_list.contains(target_struct_obj.source_count_name) )
            continue;

        if(target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
                && target_struct_obj.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
            continue;

        QString apple_timestamp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + target_struct_obj.source_count_name + QString("/apple_timestamps.sqlite");


        if(recon_case_runner_type == MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE)
        {
            break;
        }
        else
        {
            if(!QFileInfo(target_struct_obj.fs_extraction_source_path).exists())
                continue;

            list_struct_fs_rec_and_path.clear();
            map_apple_metadata_for_recon_logical_image.clear();

            emit signal_PBAR_thread_apple_timestamp_value(MACRO_JobType_Extract_Apple_Timestamps, target_struct_obj.source_name, false, 0, 0, false);

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + target_struct_obj.source_count_name + QString("/file_system.sqlite");

            if(!open_and_create_apple_timestamp_db(apple_timestamp_db_path))
            {
                continue;
            }
            QString apple_metadata_path =   global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();
            QString out_put_path_dir = apple_metadata_path + target_struct_obj.source_count_name + "/";
            QString txt_file_path = out_put_path_dir + MACRO_Spotlight_Extracted_Text_File_Name;
            QString csv_file_pth = out_put_path_dir + MACRO_Spotlight_Extracted_CSV_File_Name;

            struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

            bool_need_to_fill_apple_timestamps_from_QMap = true;
            emit signal_PBAR_thread_apple_timestamp_value(MACRO_JobType_Extract_Apple_Timestamps,("Initializing... " +target_struct_obj.source_name), true, MACRO_Generic_Progress_No_Records_int, -1, false);
            map_apple_metadata_for_recon_logical_image = recon_helper_standard_obj->extract_apple_metadata_from_script(txt_file_path,csv_file_pth,apple_metadata_attributes_name_list);


            if(bool_need_to_fill_apple_timestamps_from_QMap)
            {
                qint64 records_count = 0;

                mutext_for_fs_main_db->lock();

                bool status = open_fs_db(fs_db_path);

                if(!status)
                {
                    in_memory_apple_timestamp_db.close();
                    mutext_for_fs_main_db->unlock();

                    continue;
                }
                QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString("Select count(*) from files") , 0, file_system_db, Q_FUNC_INFO);
                qint64  total_records_num = total_records_str.toLongLong();

                QString command_fs = QString("select INT, filepath from files");
                QSqlQuery query(file_system_db);
                if(!query.exec(command_fs))
                {
                    file_system_db.close();
                    in_memory_apple_timestamp_db.close();
                    mutext_for_fs_main_db->unlock();
                    continue;
                }

                while(query.next())
                {
                    struct_filesystem_record_and_filepath m1;
                    m1.filesystem_record = query.value(0).toString();
                    m1.filepath = query.value(1).toString();
                    list_struct_fs_rec_and_path << m1;
                }

                file_system_db.close();
                mutext_for_fs_main_db->unlock();

                for(int num = 0; num < list_struct_fs_rec_and_path.size(); num++)
                {

                    records_count++;

                    struct_filesystem_record_and_filepath fs_rec_filepath_obj = list_struct_fs_rec_and_path.at(num);

                    struct_global_apple_metadata_parsed_data extended_attr_parsed_data_obj;

                    QString file_full_path = target_struct_obj.fs_extraction_source_path + fs_rec_filepath_obj.filepath;


                    bool file_exist = fill_apple_timestamps_from_script(fs_rec_filepath_obj.filepath ,attributes_actual_key_list , extended_attr_parsed_data_obj);
                    if(!file_exist)
                        continue;

                    extended_attr_parsed_data_obj.filesystem_record = fs_rec_filepath_obj.filesystem_record;

                    /// Save data for one record
                    QString command_str;
                    QStringList value_list;
                    if(extended_attr_parsed_data_obj.extended_attrs_keys_list.size() == extended_attr_parsed_data_obj.extended_attrs_values_list.size())
                    {
                        command_str = QString("INSERT INTO files (filesystem_record,");

                        QString question_marks;

                        for(int num = 0; num < extended_attr_parsed_data_obj.extended_attrs_keys_list.size(); num++)
                        {
                            command_str.append(attribute_column_list_in_apple_timestamp_db.at(num)  + QString(","));

                            question_marks.append("?,");
                        }

                        if(command_str.endsWith(QString(",")))
                            command_str.chop(1);

                        if(question_marks.endsWith(QString(",")))
                            question_marks.chop(1);

                        command_str.append(") values(?,");

                        command_str.append(question_marks);

                        command_str.append(")");

                        value_list.clear();
                        value_list << fs_rec_filepath_obj.filesystem_record << extended_attr_parsed_data_obj.extended_attrs_values_list;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_str , value_list , in_memory_apple_timestamp_db , Q_FUNC_INFO);
                    }


                    if(records_count % 100 == 0)
                    {
                        qint64 progress_val = ((records_count * 100) / total_records_num);
                        if(progress_val > 99)
                            progress_val = 99;

                        emit signal_PBAR_thread_apple_timestamp_value(MACRO_JobType_Extract_Apple_Timestamps, target_struct_obj.source_name, false, records_count, progress_val, false);
                    }


                    if(records_count % 2000 == 0)
                    {
                        intermediate_save_data_to_apple_timestamp_db(apple_timestamp_db_path);
                    }

                }

                intermediate_save_data_to_apple_timestamp_db(apple_timestamp_db_path);
                // in_memory_apple_timestamp_db.commit();
                in_memory_apple_timestamp_db.close();

                mutext_for_fs_main_db->lock();
                copy_data_from_apple_timestamp_to_fs_db(target_struct_obj.source_count_name);
                generate_filesystem_timeline(target_struct_obj.source_count_name);
                mutext_for_fs_main_db->unlock();
                emit signal_PBAR_thread_apple_timestamp_value(MACRO_JobType_Extract_Apple_Timestamps, target_struct_obj.source_name, false, records_count, 100, false);
            }
        }
    }
    emit signal_thread_apple_timestamp_finished();

    emit signal_PBAR_thread_apple_timestamp_finished(MACRO_JobType_Extract_Apple_Timestamps);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

bool thread_apple_timestamps::open_and_create_apple_timestamp_db(QString apple_timestamp_db_path)
{ // open and create apple timestamps database
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_apple_timestamp_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_apple_timestamp_db.setDatabaseName(":memory:");
    if(!in_memory_apple_timestamp_db.open())
    {
        recon_static_functions::app_debug("Open in memory ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_apple_timestamp_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    QString command_str = QString("create table files (INT INTEGER PRIMARY KEY, filesystem_record INTEGER, useddates varchar(200), usecount varchar(200), last_useddate INTEGER, content_creation_date INTEGER, content_modification_date INTEGER, date_added INTEGER)");

    QSqlQuery query_create_table(in_memory_apple_timestamp_db);
    query_create_table.prepare(command_str);

    if(!query_create_table.exec())
    {
        recon_static_functions::app_debug(" query memory table create  ---FAILED---  db file - " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
        in_memory_apple_timestamp_db.close();
        return false;
    }

    if(QFileInfo(apple_timestamp_db_path).exists())
        QFile::remove(apple_timestamp_db_path);

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(apple_timestamp_db_path));

    QSqlQuery query_attach(in_memory_apple_timestamp_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        in_memory_apple_timestamp_db.close();
        return false;
    }

    QSqlQuery query_save(in_memory_apple_timestamp_db);
    query_save.prepare(QString("CREATE TABLE filesystemDB.files AS SELECT * from files"));
    if(!query_save.exec())
    {
        recon_static_functions::app_debug(" query memory Save  ---FAILED---- "  + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_save.lastError().text(),Q_FUNC_INFO);
        in_memory_apple_timestamp_db.close();
        return false;
    }

    QSqlQuery query_detach(in_memory_apple_timestamp_db);
    query_detach.prepare("DETACH DATABASE filesystemDB");
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory Detach  ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        in_memory_apple_timestamp_db.close();
        return false;
    }

    return true;

}

void thread_apple_timestamps::set_essential()
{

}
void thread_apple_timestamps::set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;

}

void thread_apple_timestamps::pub_set_FS_source_selected_source_count_name_list(QStringList m_list)
{
    source_selected_source_count_name_list = m_list;

}
void thread_apple_timestamps::pub_set_apple_metadata_all_attributes_name_list(QStringList attributes_name_list)
{
    apple_metadata_attributes_name_list = attributes_name_list;
}

bool thread_apple_timestamps::open_fs_db(QString fs_db_path)
{ // open file system database here
    recon_static_functions::app_debug(" -Starts" ,Q_FUNC_INFO);

    bool db_status = false;

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    file_system_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    file_system_db.setDatabaseName(fs_db_path);

    if(!file_system_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + file_system_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + file_system_db.lastError().text(),Q_FUNC_INFO);
    }
    else
        db_status = true;

    recon_static_functions::app_debug(" -Ends" ,Q_FUNC_INFO);

    return db_status;
}

void thread_apple_timestamps::intermediate_save_data_to_apple_timestamp_db(QString apple_timestamp_database_path)
{ // save data in apple timestamps database
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(apple_timestamp_database_path));

    QSqlQuery query_attach(in_memory_apple_timestamp_db);
    query_attach.prepare(db_attach_commmand);
    if(!query_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_attach.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_insert(in_memory_apple_timestamp_db);

    QString command_insert = QString("INSERT INTO filesystemDB.files SELECT * from files");
    query_insert.prepare(command_insert);
    if(!query_insert.exec())
    {
        recon_static_functions::app_debug(" query memory insert  ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_clear(in_memory_apple_timestamp_db);
    QString command_clear = QString("DELETE from files");
    query_clear.prepare(command_clear);
    if(!query_clear.exec())
    {
        recon_static_functions::app_debug(" query memory delete  ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_clear.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_detach(in_memory_apple_timestamp_db);
    QString command_detach = QString("DETACH DATABASE filesystemDB");
    query_detach.prepare(command_detach);
    if(!query_detach.exec())
    {
        recon_static_functions::app_debug(" query memory detach  ---FAILED--- " + in_memory_apple_timestamp_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
        return;
    }
}

void thread_apple_timestamps::copy_data_from_apple_timestamp_to_fs_db(QString source_count_name)
{ // copy all the timestmaps data from apple timestamos database to main file system database

    QString apple_timestamp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/apple_timestamps.sqlite");


    recon_helper_standard_obj->create_db_index_by_db_path("fs_parent", "filesystem_record", "files", apple_timestamp_db_path, Q_FUNC_INFO);


    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/file_system.sqlite");


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

    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_timestampsDB")
            .arg(QDir::toNativeSeparators(apple_timestamp_db_path));
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
        command_str.append(QString(" " + attribute_column_list_in_fs_db.at(ii) + " = (Select " + attribute_column_list_in_apple_timestamp_db.at(ii) + " From apple_timestampsDB.files where main.files.INT = apple_timestampsDB.files.filesystem_record),"));
    }
    if(command_str.trimmed().endsWith(","))
        command_str.chop(1);

    while(1)
    {
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

    QString command_detach = QString("DETACH DATABASE apple_timestampsDB");
    dest_query.exec(command_detach);
    destination_db.close();


}

void thread_apple_timestamps::generate_filesystem_timeline(QString source_count_name)
{ // generate file system timelines here and save the info in tmp database and later transfer all the data in file system database
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QString fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + source_count_name + ".sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_timeline_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery dest_query(destination_db);

    QString columns_str = " (bookmark INT,INT INTEGER PRIMARY KEY,plugin varchar(30),category varchar(30), table_name varchar(30), "
                          "record_no INTEGER, stamp_name varchar(30), stamp_type varchar(30),timestamp INTEGER,item1 varchar(300), item2 varchar(300),os_scheme_display varchar(300),source_count_name varchar(300),"
                          "loaded varchar(10), year_str varchar(100), month_year varchar(100), day_month_year varchar(100), hour_day_month_year varchar(100),recon_tag_value varchar(30), notes varchar(100),blank_column varchar(100),"
                          "notes_icon varchar(100),LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000))";

    QString command_str = "create table IF NOT EXISTS timeline_data" + columns_str;

    dest_query.prepare(command_str);

    if(!dest_query.exec())
    {
        recon_static_functions::app_debug(" create query(dest) --------FAILED------ " + dest_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
        return;
    }


    ///===== temp database ========
    QString tmp_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "tmp_fs_timeline.sqlite";
    QSqlDatabase::removeDatabase(connection_naam + "_1");
    QSqlDatabase tmp_destination_db = QSqlDatabase::addDatabase("QSQLITE", (connection_naam + "_1"));

    tmp_destination_db.setDatabaseName(tmp_timeline_db_path);
    if(!tmp_destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening(tmp fs) --------FAILED------ " + tmp_destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + tmp_destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(tmp_destination_db);

    command_str = "create table tmp_timeline_data" + columns_str;

    query.prepare(command_str);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" create query(tmp) --------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    ///==Attach temp with main database for copy
    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS timelineDB")
            .arg(QDir::toNativeSeparators(tmp_timeline_db_path));
    if(!dest_query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug(" attach temp to main --------FAILED------ " + dest_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    ///===== ============= ========



    /// attach filesystem database to temp database
    QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    db_attach_commmand = QString("ATTACH DATABASE '%1' AS fs_timelineDB")
            .arg(QDir::toNativeSeparators(filesystem_db_path));
    if(!query.exec(db_attach_commmand))
    {
        recon_static_functions::app_debug(" FS Db Attach --------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QStringList timestamp_name_list;
    timestamp_name_list << "Content Creation Date" << "Content Modification Date" << "Date Added" << "Last Used";

    for(int num = 0; num < timestamp_name_list.size(); num++)
    {

        QString timestamp_name = timestamp_name_list.at(num);
        QString timestamp_type , timestamp_column_name;

        if(timestamp_name == QString("Content Creation Date"))
        {
            timestamp_type = QString("CNCRT");
            timestamp_column_name = QString("kMDItemContentCreationDate_Created_Date");
        }
        else if(timestamp_name == QString("Content Modification Date"))
        {
            timestamp_type = QString("CNMOD");
            timestamp_column_name = QString("kMDItemContentModificationDate_Last_Modified_Date");
        }
        else if(timestamp_name == QString("Date Added"))
        {
            timestamp_type = QString("DTADD");
            timestamp_column_name = QString("kMDItemDateAdded_Date_Added");
        }
        else if(timestamp_name == QString("Last Used"))
        {
            timestamp_type = QString("LUSED");
            timestamp_column_name = QString("kMDItemLastUsedDate_Last_Opened_Date");
        }
        else
        {
            timestamp_type.clear();
            timestamp_column_name.clear();
        }

        if(timestamp_column_name.isEmpty())
            continue;

        command_str = QString("Insert into tmp_timeline_data (record_no, timestamp, item1, item2, source_count_name,"
                              "bookmark) ");
        command_str.append(QString("Select INT,") + timestamp_column_name + QString(",filename,filepath,source_count_name,bookmark From files Where ") + timestamp_column_name + " is not null AND " + timestamp_column_name + " != ''");

        if(!query.exec(command_str))
        {
            recon_static_functions::app_debug(" query execute (insert and select)--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        command_str = QString("Update tmp_timeline_data set plugin = ?,category = ?, table_name = ? , stamp_name = ? , stamp_type = ?");

        query.prepare(command_str);
        query.addBindValue(QString(MACRO_Plugin_Name_File_System));           //plugin
        query.addBindValue(QString(MACRO_Plugin_File_System_TAB_Name_Files)); //category
        query.addBindValue(QString("files"));                 //table_name
        query.addBindValue(timestamp_name);           //stamp_name
        query.addBindValue(timestamp_type);           //stamp_type

        if(!query.exec())
        {
            recon_static_functions::app_debug(" query execute (update)--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        }


        /// save info into main db from temp db
        command_str = QString("Insert into timeline_data (bookmark,plugin,category,table_name,record_no, stamp_name,stamp_type,timestamp, item1, item2, source_count_name) ");
        command_str.append(QString("Select bookmark,plugin,category,table_name,record_no, stamp_name,stamp_type,timestamp, item1, item2, source_count_name From tmp_timeline_data"));
        if(!dest_query.exec(command_str))
        {
            recon_static_functions::app_debug(" query execute (insert and select main)--------FAILED------ " + dest_query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_query.lastError().text(),Q_FUNC_INFO);
        }


        /// delete everything from temp db
        command_str = QString("Delete From tmp_timeline_data");
        if(!query.exec(command_str))
        {
            recon_static_functions::app_debug(" query execute (delete)--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        }



    }

    destination_db.close();

    tmp_destination_db.close();
    recon_static_functions::remove_safely_blank_dir_OR_file(tmp_timeline_db_path , Q_FUNC_INFO);

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}

bool thread_apple_timestamps::fill_apple_timestamps_from_script(QString file_path, QStringList keys_list, struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj)
{ // file apple timestamps in structure from map
    QMap <QString , QString > tmp_qmap;
    tmp_qmap =  map_apple_metadata_for_recon_logical_image.value(file_path);
    if(tmp_qmap.isEmpty())
    {
        return false;
    }

    for(int bb = 0; bb < keys_list.size();  bb++)
    {
        QString keys = keys_list.at(bb);

        QString values = tmp_qmap.value(keys);

        extended_attr_parsed_data_obj.extended_attrs_values_list << values;
        extended_attr_parsed_data_obj.extended_attrs_keys_list << keys;
    }

    return true;

}

void thread_apple_timestamps::pub_set_apple_timestamps_reader_process(QProcess *mobj)
{
    process_apple_timestamps_reader = mobj;

}

void thread_apple_timestamps::extract_apple_timestamps_from_apple_metadata_reader(QStringList file_path_list,QString source_cnt_name,QString virtual_source_path)
{ // extract apple timestammps from apple_metadata_reader but not in use right now
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);
    QString file_system_apple_timestamps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString).toString();


    QString exec_path = QDir::currentPath() + "/apple_metadata_reader.app/Contents/MacOS/apple_metadata_reader";

    QString apple_timestamps_attr_names_str = attributes_actual_key_list.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);
    QString apple_timestamps_column_name = attribute_column_list_in_apple_timestamp_db.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);

    QString file_paths_str = file_path_list.join(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Main);

    QString m_flag = QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_extraction_timestamps);

    struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString system_pswd = pwd_obj.desktop_password_original;
    bool bool_script_run_with_root_prevs = false;

    QString cmd_load;

    if(pwd_obj.bool_desktop_pwd_accurate)
    {
        //        cmd_load = QString("echo '") + system_pswd + "' | sudo -S " + exec_path.replace(" ", "\\ ") + " " + "\"" + virtual_source_path + "\"" +  " " + "\"" + file_paths_str + "\"" +" " + "\"" + file_system_apple_timestamps + "\""
        //                + " " + "\"" + apple_timestamps_attr_names_str + "\"" + " " +  "\"" + apple_timestamps_column_name + "\"" + " " +  "\"" + source_cnt_name + "\"" + " " +  "\"" + m_flag + "\"";


        cmd_load = QString("echo $FORENSOPASSWD") + " | sudo -S " + exec_path.replace(" ", "\\ ") + " " + "\"" + virtual_source_path + "\"" +  " " + "\"" + file_paths_str + "\"" +" " + "\"" + file_system_apple_timestamps + "\""
                + " " + "\"" + apple_timestamps_attr_names_str + "\"" + " " +  "\"" + apple_timestamps_column_name + "\"" + " " +  "\"" + source_cnt_name + "\"" + " " +  "\"" + m_flag + "\"";

        bool_script_run_with_root_prevs = true;
    }
    else
    {
        cmd_load = exec_path.replace(" ", "\\ ") + " " + "\"" + virtual_source_path + "\"" +  " " + "\"" + file_paths_str + "\"" +" " + "\"" + file_system_apple_timestamps + "\""
                + " " + "\"" + apple_timestamps_attr_names_str + "\"" + " " +  "\"" + apple_timestamps_column_name + "\"" + " " +  "\"" + source_cnt_name + "\"" + " " +  "\"" + m_flag + "\"";
    }

    QString available_file = recon_static_functions::get_available_filename("apple_timestamps",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString script_path = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;
    qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_path,cmd_load, Q_FUNC_INFO);
    if(byte_size == -1)
        return;

    if(bool_script_run_with_root_prevs)
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("FORENSOPASSWD", system_pswd);

        QStringList args;
        args << script_path;

        process_apple_timestamps_reader->setProcessEnvironment(env);
        process_apple_timestamps_reader->start("bash", args);

    }
    else
    {
        process_apple_timestamps_reader->start("sh", QStringList(script_path));
    }
    if (!process_apple_timestamps_reader->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + process_apple_timestamps_reader->errorString() ,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running111", Q_FUNC_INFO);

    while(1)
    {

        process_apple_timestamps_reader->waitForReadyRead();
        if(process_apple_timestamps_reader->state() == QProcess::NotRunning)
        {
            break;
        }
    }

    QString tmp_apple_timestamps_db_path = file_system_apple_timestamps + source_cnt_name + QString("/apple_timestamps.sqlite");
    QString actual_apple_timestamp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_cnt_name + QString("/apple_timestamps.sqlite");
    copy_tmp_apple_timestamps_to_actual_apple_timestamps_db(tmp_apple_timestamps_db_path,actual_apple_timestamp_db_path);

    mutext_for_fs_main_db->lock();
    copy_data_from_apple_timestamp_to_fs_db(source_cnt_name);
    generate_filesystem_timeline(source_cnt_name);
    mutext_for_fs_main_db->unlock();

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

void thread_apple_timestamps::slot_prc_apple_timestamps_readyread()
{ // parent function not in use
    QString prc_output = process_apple_timestamps_reader->readAll();
    prc_output.remove("\"");
    QString record_str = QString(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Record);
    if(prc_output.startsWith(record_str))
    {
        QString record_count = prc_output;
        record_count.remove(0,record_str.size());
        record_count = record_count.trimmed();
        if(record_count.isEmpty())
            return;
        QStringList tmp_list = record_count.split(MACRO_GLOBAL_RECON_AND_APPLE_METADATA_READER_COMMON_Splitter_Ampersand);
        QString source_cnt_name = tmp_list.at(0).trimmed();
        QString process_records = tmp_list.at(1).trimmed();
        QString total_records = tmp_list.at(2).trimmed();

        qint64 process_record_cnt = process_records.toLongLong();
        qint64 total_record_cnt = total_records.toLongLong();
        if(total_record_cnt > 0)
        {
            struct_GLOBAL_witness_info_source target_struct_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);

            int progress_val = (int)((process_record_cnt * 100) / total_record_cnt);
            if(progress_val > 99)
                progress_val = 99;

            emit signal_PBAR_thread_apple_timestamp_value(MACRO_JobType_Extract_Apple_Timestamps, target_struct_obj.source_name, false, process_record_cnt, progress_val, false);
        }
    }
}


void thread_apple_timestamps::copy_tmp_apple_timestamps_to_actual_apple_timestamps_db(QString tmp_timestamps_db_path,QString actual_db_path)
{ // parent function not in use
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    //    QString connection_naam = Q_FUNC_INFO;
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    QSqlDatabase tmp_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    tmp_db.setDatabaseName(tmp_fs_db_path);
    //    if(!tmp_db.open())
    //    {
    //        recon_static_functions::app_debug("Open in memory ---FAILED--- " + tmp_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(tmp_db.lastError().text() , Q_FUNC_INFO);
    //    }

    //    QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_timestamps_db")
    //            .arg(QDir::toNativeSeparators(actual_db_path));

    //    QSqlQuery attach_query(tmp_db);
    //    attach_query.prepare(db_attach_commmand);
    //    if(!attach_query.exec())
    //    {
    //        recon_static_functions::app_debug(" Attach query(tmp) --------FAILED------ " + attach_query.executedQuery(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + attach_query.lastError().text(),Q_FUNC_INFO);
    //    }


    //    QSqlQuery insert_query(tmp_db);

    //    attribute_column_list_in_apple_timestamp_db << "useddates" << "usecount"
    //                                                << "last_useddate" << "content_creation_date"
    //                                                << "content_modification_date" << "date_added";

    //    //QString insert_cmd = "INSERT into apple_timestamps_db.files (Select * from main.files)";
    //    QString insert_cmd = QString("Insert into apple_timestamps_db.files (filesystem_record, usecount, last_useddate, content_creation_date, content_modification_date,"
    //                                 "date_added) ");
    //    insert_cmd.append(QString("Select filesystem_record,usecount,last_useddate,content_creation_date,content_modification_date,date_added From main.files"));



    //    insert_query.prepare(insert_cmd);
    //    if(!insert_query.exec())
    //    {
    //        recon_static_functions::app_debug(" Insert query(tmp) --------FAILED------ " + insert_query.executedQuery(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
    //    }
    //    tmp_db.close();
    //====================================================================================================================================//

    QFile actual_file(actual_db_path);
    if(actual_file.exists())
    {
        actual_file.remove();
    }

    if(QFile(tmp_timestamps_db_path).exists())
    {
        QFile::copy(tmp_timestamps_db_path,actual_db_path);
    }

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}
