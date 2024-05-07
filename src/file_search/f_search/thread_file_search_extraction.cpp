#include "thread_file_search_extraction.h"

thread_file_search::thread_file_search(QWidget *parent)
{
    bool_cancel_extraction = false;
    timer_records_fs = new QElapsedTimer();
    pause_thread = false;

    recon_helper_standard_obj = new recon_helper_standard(this);

    total_records = 0;
}

thread_file_search::~thread_file_search()
{
    if(timer_records_fs != NULL)
        delete timer_records_fs;
}

void thread_file_search::run()
{ // thread of file search start from here, it will start searching and store that info in database. Also progress bar, counter for display we show from here
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    bool_cancel_extraction = false;

    list_int_with_source_count_name.clear();

    total_records = 0;

    timer_records_fs->start();

    insert_file_search_config_into_index_db();

    destination_db_path = create_file_search_destination_db();

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

    QSqlQuery query_insert(*destination_db);
    query_insert.prepare("INSERT INTO files(filesystem_record, source_count_name) VALUES (?,?)");


    //    QString file_name_query_for_search_in_db            = get_query_for_file_name_search_in_db("filename");
    //    QString file_size_query_to_search_in_db             = get_query_for_file_size_search_in_db("file_size");
    //    QString created_date_query_for_search_in_db         = get_query_for_date_search_in_db(created_date_str_to_search, "date_created");
    //    QString last_modified_date_query_for_search_in_db   = get_query_for_date_search_in_db(last_modif_date_str, "date_last_modified");
    //    QString last_read_date_query_for_search_in_db       = get_query_for_date_search_in_db(last_read_date_str_to_search, "date_last_read");
    //    QString added_date_query_for_search_in_db           = get_query_for_date_search_in_db(added_date_str_to_search, "date_added");

    if(bool_run_in_full_mode)
    {
        QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
        for(int i = 0; i < list_source_info.size(); i++)
        {
            if(bool_cancel_extraction)
            {
                break;
            }

            struct_GLOBAL_witness_info_source struct_info = list_source_info.at(i);
            QString source_name = struct_info.source_count_name;

            QString source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_name + "/file_system.sqlite";

            QSqlDatabase::removeDatabase(connection_naam + source_name);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + source_name);
            source_db.setDatabaseName(source_db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("source_db Db opening --------FAILED------ " + source_db_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + source_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            QSqlQuery query_select(source_db);

            QString command = "SELECT INT, source_count_name FROM files WHERE ";

            //            if(is_radio_all_filter_clicked)
            //            {
            //                fill_list_for_regular_filters_info_by_logically_and(command, file_name_query_for_search_in_db, file_size_query_to_search_in_db, created_date_query_for_search_in_db, last_modified_date_query_for_search_in_db, last_read_date_query_for_search_in_db, added_date_query_for_search_in_db, query_select);
            //            }
            //            else
            //            {
            //                fill_list_for_regular_filters_info_by_logically_or(command, file_name_query_for_search_in_db, file_size_query_to_search_in_db, created_date_query_for_search_in_db, last_modified_date_query_for_search_in_db, last_read_date_query_for_search_in_db, added_date_query_for_search_in_db, query_select);
            //            }

            fill_list_for_apple_metadata_filters_info(source_name);

            source_db.close();
        }
    }
    else
    {
        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(file_system_source_count_name);

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + struct_info.source_count_name + "/file_system.sqlite";

        QSqlDatabase::removeDatabase(connection_naam + struct_info.source_count_name);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + struct_info.source_count_name);
        source_db.setDatabaseName(fs_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug("source_db Db opening --------FAILED------ " + fs_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }
        else
        {
            QSqlQuery query_select(source_db);

            QString command = "SELECT INT,source_count_name FROM files WHERE file_parent_path LIKE '" + parent_dir_path + "%' AND ";

            //            if(is_radio_all_filter_clicked)
            //            {
            //                fill_list_for_regular_filters_info_by_logically_and(command, file_name_query_for_search_in_db, file_size_query_to_search_in_db, created_date_query_for_search_in_db, last_modified_date_query_for_search_in_db, last_read_date_query_for_search_in_db, added_date_query_for_search_in_db, query_select);
            //            }
            //            else
            //            {
            //                fill_list_for_regular_filters_info_by_logically_or(command, file_name_query_for_search_in_db, file_size_query_to_search_in_db, created_date_query_for_search_in_db, last_modified_date_query_for_search_in_db, last_read_date_query_for_search_in_db, added_date_query_for_search_in_db, query_select);
            //            }

            fill_list_for_spotlight_filters_info(query_select, struct_info.fs_extraction_source_path);

            source_db.close();
        }
    }

    insert_searched_info_into_file_search_db(query_insert);

    update_total_records_extracted_in_file_search_index_db();

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}

void thread_file_search::fill_list_for_apple_metadata_filters_info(QString source_count_name)
{ // fill list for apple metadata filters into destination database
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    if(mdfind_condition_str.trimmed().isEmpty())
        return ;

    QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/apple_metadata.sqlite";

    QString command;
    command.append("select filesystem_record from files where " + mdfind_condition_str);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    apple_metadata_db.setDatabaseName(apple_metadata_db_path);
    if(!apple_metadata_db.open())
    {
        recon_static_functions::app_debug(" apple_metadata db open ----FAILED---" + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + apple_metadata_db.lastError().text(),Q_FUNC_INFO);
    }

    QSqlQuery query_select(apple_metadata_db);

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug("Query ---FAILED--- " + apple_metadata_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error  " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query  " + query_select.executedQuery() , Q_FUNC_INFO);
        apple_metadata_db.close();
        return;
    }

    QSqlDatabase::removeDatabase(connection_naam + "1");
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + "1");
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + destination_db.lastError().text(),Q_FUNC_INFO);
        apple_metadata_db.close();
        return;
    }

    QString insert_command = "insert into files (filesystem_record, source_count_name) values (?,?)";
    QStringList args;
    while(query_select.next())
    {
        args.clear();
        args << query_select.value(0).toString() << source_count_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_command, args, destination_db, Q_FUNC_INFO );
    }



    destination_db.close();
    apple_metadata_db.close();

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

}

void thread_file_search::insert_searched_info_into_file_search_db(QSqlQuery &query_insert)
{ // insert the searched data in file search database
    for(int i = 0; i < list_int_with_source_count_name.size(); i++)
    {
        if(bool_cancel_extraction)
            break;

        QString value_str = list_int_with_source_count_name.at(i);

        QStringList str_list = value_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(str_list.size() > 1)
        {
            QString int_str = str_list.at(0);
            QString source_count_name_str = str_list.at(1);

            query_insert.addBindValue(int_str);
            query_insert.addBindValue(source_count_name_str);
            if(!query_insert.exec())
            {
                recon_static_functions::app_debug(" command -  " + query_insert.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
            }

            total_records++;
            if(timer_records_fs->elapsed() > MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Extracted_Record_Timer_Interval_int)
            {
                timer_records_fs->restart();
            }
        }
    }
}

void thread_file_search::fill_list_for_spotlight_filters_info(QSqlQuery &query_select, QString fs_extration_path)
{ // fill list for spotlight filters info
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString combined_path = recon_static_functions::get_complete_file_path(fs_extration_path, parent_dir_path, Q_FUNC_INFO);

    stop_thread();

    emit signal_run_separate_process_for_mdfind_outside_thread(combined_path, mdfind_condition_str);

    mutex_val.lock();
    if(pause_thread)
        waitcondition.wait(&mutex_val); // in this place, your thread will stop to execute until someone calls resume

    mutex_val.unlock();

    QStringList list_of_searched_files_path = mdfind_searched_putput_str.split("\n", Qt::SkipEmptyParts);

    for(int i = 0; i < list_of_searched_files_path.size(); i++)
    {
        if(bool_cancel_extraction)
        {
            break;
        }

        QString file_path = list_of_searched_files_path.at(i);

        if(file_path.startsWith(fs_extration_path))
            file_path.remove(0, fs_extration_path.size());

        if(!file_path.startsWith("/"))
            file_path.prepend("/");

        QString command = "SELECT INT, source_count_name FROM files WHERE filepath = ?";
        query_select.prepare(command);
        query_select.addBindValue(file_path);
        if(!query_select.exec())
        {
            recon_static_functions::app_debug("command -  " + query_select.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + query_select.lastError().text(),Q_FUNC_INFO);
            return;
        }

        while(query_select.next())
        {
            if(bool_cancel_extraction)
            {
                break;
            }

            QString int_str = query_select.value(0).toString();
            QString source_name = query_select.value(1).toString();

            QString str_value = int_str + MACRO_RECON_Splitter_1_multiple + source_name;

            if(list_int_with_source_count_name.contains(str_value))
                continue;

            list_int_with_source_count_name.append(str_value);
        }
    }

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}



//void thread_file_search::fill_list_for_regular_filters_info_by_logically_and(QString command, QString file_name_query_for_search_in_db, QString file_size_query_to_search_in_db, QString created_date_query_for_search_in_db, QString last_modified_date_query_for_search_in_db, QString last_read_date_query_for_search_in_db, QString added_date_query_for_search_in_db, QSqlQuery &query_select)
//{
//    /// File Name
//    QString file_name_query_without_where = file_name_query_for_search_in_db;
//    if(file_name_query_without_where.contains("WHERE"))
//        file_name_query_without_where.replace("WHERE", "");

//    file_name_query_without_where = file_name_query_without_where.trimmed();
//    if(!file_name_query_without_where.isEmpty())
//    {
//        command.append(file_name_query_without_where + " AND ");
//    }

//    /// File Size
//    QString file_size_query_without_where = file_size_query_to_search_in_db;
//    if(file_size_query_without_where.contains("WHERE"))
//        file_size_query_without_where.replace("WHERE", "");

//    file_size_query_without_where = file_size_query_without_where.trimmed();

//    if(!file_size_query_without_where.isEmpty())
//    {
//        command.append(file_size_query_without_where + " AND ");
//    }

//    /// Create Date
//    QString create_date_query_without_where = created_date_query_for_search_in_db;
//    if(create_date_query_without_where.contains("WHERE"))
//        create_date_query_without_where.replace("WHERE", "");

//    create_date_query_without_where = create_date_query_without_where.trimmed();

//    if(!create_date_query_without_where.isEmpty())
//    {
//        command.append(create_date_query_without_where + " AND ");
//    }

//    /// Last Modified Date
//    QString last_modified_date_query_without_where = last_modified_date_query_for_search_in_db;
//    if(last_modified_date_query_without_where.contains("WHERE"))
//        last_modified_date_query_without_where.replace("WHERE", "");

//    last_modified_date_query_without_where = last_modified_date_query_without_where.trimmed();

//    if(!last_modified_date_query_without_where.isEmpty())
//    {
//        command.append(last_modified_date_query_without_where + " AND ");
//    }

//    /// Last Read Date
//    QString last_read_date_query_without_where = last_read_date_query_for_search_in_db;
//    if(last_read_date_query_without_where.contains("WHERE"))
//        last_read_date_query_without_where.replace("WHERE", "");

//    last_read_date_query_without_where = last_read_date_query_without_where.trimmed();

//    if(!last_read_date_query_without_where.isEmpty())
//    {
//        command.append(last_read_date_query_without_where + " AND ");
//    }

//    /// Added Date
//    QString added_date_query_without_where = added_date_query_for_search_in_db;
//    if(added_date_query_without_where.contains("WHERE"))
//        added_date_query_without_where.replace("WHERE", "");

//    added_date_query_without_where = added_date_query_without_where.trimmed();

//    if(!added_date_query_without_where.isEmpty())
//    {
//        command.append(added_date_query_without_where + " AND ");
//    }

//    command = command.trimmed();

//    if(command.endsWith("AND"))
//        command.chop(3);

//    command = command.trimmed();

//    query_select.prepare(command);
//    if(!query_select.exec())
//    {
//        recon_static_functions::app_debug("command -  " + query_select.executedQuery(),Q_FUNC_INFO);
//        recon_static_functions::app_debug("error -  " + query_select.lastError().text(),Q_FUNC_INFO);
//        return;
//    }

//    while(query_select.next())
//    {
//        if(bool_cancel_extraction)
//        {
//            break;
//        }

//        QString int_str = query_select.value(0).toString();
//        QString source_name = query_select.value(1).toString();

//        QString str_value = int_str + MACRO_RECON_Splitter_1_multiple + source_name;

//        list_int_with_source_count_name.append(str_value);
//    }
//}

//void thread_file_search::fill_list_for_regular_filters_info_by_logically_or(QString command, QString file_name_query_for_search_in_db, QString file_size_query_to_search_in_db, QString created_date_query_for_search_in_db, QString last_modified_date_query_for_search_in_db, QString last_read_date_query_for_search_in_db, QString added_date_query_for_search_in_db, QSqlQuery &query_select)
//{
//    bool bool_search_fields_selected = false;

//    /// File Name
//    QString file_name_query_without_where = file_name_query_for_search_in_db;
//    if(file_name_query_without_where.contains("WHERE"))
//        file_name_query_without_where.replace("WHERE", "");

//    file_name_query_without_where = file_name_query_without_where.trimmed();
//    if(!file_name_query_without_where.isEmpty())
//    {
//        bool_search_fields_selected = true;
//        command.append(file_name_query_without_where + " OR ");
//    }

//    /// File Size
//    QString file_size_query_without_where = file_size_query_to_search_in_db;
//    if(file_size_query_without_where.contains("WHERE"))
//        file_size_query_without_where.replace("WHERE", "");

//    file_size_query_without_where = file_size_query_without_where.trimmed();
//    if(!file_size_query_without_where.isEmpty())
//    {
//        bool_search_fields_selected = true;
//        command.append(file_size_query_without_where + " OR ");
//    }

//    /// Create Date
//    QString create_date_query_without_where = created_date_query_for_search_in_db;
//    if(create_date_query_without_where.contains("WHERE"))
//        create_date_query_without_where.replace("WHERE", "");

//    create_date_query_without_where = create_date_query_without_where.trimmed();

//    if(!create_date_query_without_where.isEmpty())
//    {
//        bool_search_fields_selected = true;
//        command.append(create_date_query_without_where + " OR ");
//    }

//    /// Last Modified Date
//    QString last_modified_date_query_without_where = last_modified_date_query_for_search_in_db;
//    if(last_modified_date_query_without_where.contains("WHERE"))
//        last_modified_date_query_without_where.replace("WHERE", "");

//    last_modified_date_query_without_where = last_modified_date_query_without_where.trimmed();

//    if(!last_modified_date_query_without_where.isEmpty())
//    {
//        bool_search_fields_selected = true;
//        command.append(last_modified_date_query_without_where + " OR ");
//    }

//    /// Last Read Date
//    QString last_read_date_query_without_where = last_read_date_query_for_search_in_db;
//    if(last_read_date_query_without_where.contains("WHERE"))
//        last_read_date_query_without_where.replace("WHERE", "");

//    last_read_date_query_without_where = last_read_date_query_without_where.trimmed();

//    if(!last_read_date_query_without_where.isEmpty())
//    {
//        bool_search_fields_selected = true;
//        command.append(last_read_date_query_without_where + " OR ");
//    }

//    /// Added Date
//    QString added_date_query_without_where = added_date_query_for_search_in_db;
//    if(added_date_query_without_where.contains("WHERE"))
//        added_date_query_without_where.replace("WHERE", "");

//    added_date_query_without_where = added_date_query_without_where.trimmed();

//    if(!added_date_query_without_where.isEmpty())
//    {
//        bool_search_fields_selected = true;
//        command.append(added_date_query_without_where + " OR ");
//    }

//    if(!bool_search_fields_selected)
//        return;

//    command = command.trimmed();

//    if(command.endsWith("OR"))
//        command.chop(3);

//    command = command.trimmed();

//    query_select.prepare(command);
//    if(!query_select.exec())
//    {
//        recon_static_functions::app_debug("command -  " + query_select.executedQuery(),Q_FUNC_INFO);
//        recon_static_functions::app_debug("error -  " + query_select.lastError().text(),Q_FUNC_INFO);
//        return;
//    }

//    while(query_select.next())
//    {
//        if(bool_cancel_extraction)
//        {
//            break;
//        }

//        QString int_str = query_select.value(0).toString();
//        QString source_name = query_select.value(1).toString();

//        QString str_value = int_str + MACRO_RECON_Splitter_1_multiple + source_name;

//        list_int_with_source_count_name.append(str_value);
//    }
//}

void thread_file_search::update_total_records_extracted_in_file_search_index_db()
{ // update total records extracted in file search index db
    recon_static_functions::app_debug("Start  " + current_search_label_name,Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";

    QString command = "UPDATE file_search_index SET total_records_extracted = ? WHERE search_label_name = ?";
    QStringList arg_list;
    arg_list << QString::number(total_records) << current_search_label_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("end " + current_search_label_name,Q_FUNC_INFO);
}

QString thread_file_search::get_query_for_date_search_in_db(QString date_val, QString colume_name)
{ // get query for date search in db
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString command;

    if(date_val.contains(MACRO_RECON_Splitter_1_multiple))
    {
        QStringList tmp_str_1 = date_val.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(tmp_str_1.size() >= 2)
        {
            if(tmp_str_1.at(0).contains(MACRO_Generic_Filter_Condition_Between))
            {
                command = get_additional_query_for_between_timestamp(tmp_str_1.at(1), colume_name);
            }
            else if(tmp_str_1.at(0).contains(MACRO_Generic_Filter_Condition_Within_Last))
            {
                command = get_additional_query_of_within_last_timestamp(tmp_str_1.at(1), colume_name);
            }
            else if(tmp_str_1.at(0).contains(MACRO_Generic_Filter_Condition_Exactly))
            {
                command = get_additional_query_for_exactly_timestamp(tmp_str_1.at(1), colume_name);
            }
            else if(tmp_str_1.at(0).contains(MACRO_Generic_Filter_Condition_Before))
            {
                command = get_additional_query_for_before_timestamp(tmp_str_1.at(1), colume_name);
            }
            else if(tmp_str_1.at(0).contains(MACRO_Generic_Filter_Condition_After))
            {
                command = get_additional_query_for_after_timestamp(tmp_str_1.at(1), colume_name);
            }
        }
    }
    else if(date_val.contains(MACRO_Generic_Filter_Condition_Today))
    {
        command = get_additional_query_for_today_timestamp(colume_name);
    }
    else if(date_val.contains(MACRO_Generic_Filter_Condition_Yesterday))
    {
        command = get_additional_query_for_yesterday_timestamp(colume_name);
    }
    else if(date_val.contains(MACRO_Generic_Filter_Condition_This_Week))
    {
        command = get_additional_query_for_this_week_timestamp(colume_name);
    }
    else if(date_val.contains(MACRO_Generic_Filter_Condition_This_Month))
    {
        command = get_additional_query_for_this_month_timestamp(colume_name);
    }
    else if(date_val.contains(MACRO_Generic_Filter_Condition_This_Year))
    {
        command = get_additional_query_for_this_year_timestamp(colume_name);
    }

    return command;

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}
// function name clearly tells below what's there work
QString thread_file_search::get_additional_query_for_this_year_timestamp(QString colume_name)
{ // get additional query for this year timestamp
    QString command;

    QDate current_date = QDate::currentDate();
    int current_month_int = current_date.toString("MM").toInt();
    int current_date_int = current_date.toString("dd").toInt();

    QDate previous_date = current_date.addMonths(-(current_month_int - 1)).addDays(-(current_date_int - 1));

    QString curr_date_str = current_date.toString("dd.MM.yyyy");
    QString prev_date_str = previous_date.toString("dd.MM.yyyy");

    qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
    qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

    command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + colume_name + " <= '" + QString::number(msec_val_curr_date) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_this_month_timestamp(QString colume_name)
{
    QString command;

    QDate current_date = QDate::currentDate();
    int current_int = current_date.toString("dd").toInt();

    QDate previous_date = current_date.addDays(-(current_int - 1));

    QString curr_date_str = current_date.toString("dd.MM.yyyy");
    QString prev_date_str = previous_date.toString("dd.MM.yyyy");

    qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
    qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

    command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + colume_name + " <= '" + QString::number(msec_val_curr_date) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_this_week_timestamp(QString column_name)
{
    QString command;

    QDate current_date = QDate::currentDate();
    QDate previous_date = current_date.addDays(-7);

    QString curr_date_str = current_date.toString("dd.MM.yyyy");
    QString prev_date_str = previous_date.toString("dd.MM.yyyy");

    qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
    qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

    command = "WHERE " + column_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + column_name + " <= '" + QString::number(msec_val_curr_date) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_yesterday_timestamp(QString column_name)
{
    QString command;

    QDate previous_date = QDate::currentDate().addDays(-1);

    QString prev_date_str = previous_date.toString("dd.MM.yyyy");

    qint64 msec_val_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

    command = "WHERE " + column_name + " = '" + QString::number(msec_val_date) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_today_timestamp(QString colume_name)
{
    QString command;

    QString val_str = QDate::currentDate().toString("dd/MM/yyyy");

    qint64 msec_val_date = QDateTime::fromString(val_str, "dd/MM/yyyy").toSecsSinceEpoch();

    command = "WHERE " + colume_name + " = '" + QString::number(msec_val_date) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_exactly_timestamp(QString val_str, QString colume_name)
{
    QString command;

    const QDateTime dateTime1 = QDateTime::currentDateTime();
    const QDateTime dateTime2 = QDateTime(dateTime1.date(), dateTime1.time(), Qt::UTC);
    int current_machine_offset =   dateTime1.secsTo(dateTime2);
    int current_case_offset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toInt();

    qint64 msec_val_date = QDateTime::fromString(val_str, "dd/MM/yyyy").toSecsSinceEpoch() + (current_machine_offset + current_case_offset);

    qint64 round_off_value_date_first =  msec_val_date % (24*60*60);

    command = "WHERE " + colume_name + " = '" + QString::number(msec_val_date - round_off_value_date_first) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_before_timestamp(QString val_str, QString colume_name)
{
    QString command;

    const QDateTime dateTime1 = QDateTime::currentDateTime();
    const QDateTime dateTime2 = QDateTime(dateTime1.date(), dateTime1.time(), Qt::UTC);
    int current_machine_offset =   dateTime1.secsTo(dateTime2);
    int current_case_offset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toInt();

    qint64 msec_val_date = QDateTime::fromString(val_str, "dd/MM/yyyy").toSecsSinceEpoch() + (current_machine_offset + current_case_offset);

    qint64 round_off_value_date_first =  msec_val_date % (24*60*60);

    command = "WHERE " + colume_name + " < '" + QString::number(msec_val_date - round_off_value_date_first) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_after_timestamp(QString val_str, QString colume_name)
{
    QString command;


    const QDateTime dateTime1 = QDateTime::currentDateTime();
    const QDateTime dateTime2 = QDateTime(dateTime1.date(), dateTime1.time(), Qt::UTC);
    int current_machine_offset =   dateTime1.secsTo(dateTime2);
    int current_case_offset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toInt();

    qint64 msec_val_date = QDateTime::fromString(val_str, "dd/MM/yyyy").toSecsSinceEpoch() + (current_machine_offset + current_case_offset);

    qint64 round_off_value_date_first =  msec_val_date % (24*60*60);

    command = "WHERE " + colume_name + " > '" + QString::number(msec_val_date - round_off_value_date_first) + "'";

    return command;
}

QString thread_file_search::get_additional_query_for_between_timestamp(QString val_str, QString colume_name)
{
    QString command;

    QStringList tmp_list_2 = val_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

    if(tmp_list_2.size() < 2)
        return command;

    QString first_date = tmp_list_2.at(0);
    QString second_date = tmp_list_2.at(1);

    const QDateTime dateTime1 = QDateTime::currentDateTime();
    const QDateTime dateTime2 = QDateTime(dateTime1.date(), dateTime1.time(), Qt::UTC);
    int current_machine_offset =   dateTime1.secsTo(dateTime2);
    int current_case_offset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toInt();

    qint64 msec_val_date_first = QDateTime::fromString(first_date, "dd/MM/yyyy").toSecsSinceEpoch() + (current_machine_offset + current_case_offset);
    qint64 msec_val_date_second = QDateTime::fromString(second_date, "dd/MM/yyyy").toSecsSinceEpoch() + (current_machine_offset + current_case_offset);

    //    qint64 round_off_value_date_first =  msec_val_date_first % (24*60*60); // Make it at 12:00
    //    qint64 round_off_value_date_second =  msec_val_date_second % (24*60*60); // Make it at 12:00
    qint64 round_off_value_date_first =  msec_val_date_first % (24*60*60); // Make it at 12:00
    qint64 round_off_value_date_second =  msec_val_date_second % (24*60*60); // Make it at 12:00





    command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_date_first - round_off_value_date_first) + "' AND " + colume_name + " <= '" + QString::number(msec_val_date_second - round_off_value_date_second) + "'";

    return command;
}

QString thread_file_search::get_additional_query_of_within_last_timestamp(QString val_str, QString colume_name)
{
    QString command;

    QString second_val = val_str;
    QStringList tmp_list_2 = second_val.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

    if(tmp_list_2.size() < 2)
        return command;

    if(tmp_list_2.at(1).contains(MACRO_Generic_Filter_Condition_Days))
    {
        qint64 days_count = tmp_list_2.at(0).toLongLong();

        QDate current_date = QDate::currentDate();
        QDate previous_date = current_date.addDays(-days_count);

        QString curr_date_str = current_date.toString("dd.MM.yyyy");
        QString prev_date_str = previous_date.toString("dd.MM.yyyy");

        qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
        qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

        command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + colume_name + " <= '" + QString::number(msec_val_curr_date) + "'";
    }
    else if(tmp_list_2.at(1).contains(MACRO_Generic_Filter_Condition_Weeks))
    {
        qint64 weeks_count = tmp_list_2.at(0).toLongLong();
        qint64 days_count = weeks_count * 7;

        QDate current_date = QDate::currentDate();
        QDate previous_date = current_date.addDays(-days_count);

        QString curr_date_str = current_date.toString("dd.MM.yyyy");
        QString prev_date_str = previous_date.toString("dd.MM.yyyy");

        qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
        qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

        command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + colume_name + " <= '" + QString::number(msec_val_curr_date) + "'";
    }
    else if(tmp_list_2.at(1).contains(MACRO_Generic_Filter_Condition_Months))
    {
        qint64 months_count = tmp_list_2.at(0).toLongLong();

        QDate current_date = QDate::currentDate();
        QDate previous_date = current_date.addMonths(-months_count);

        QString curr_date_str = current_date.toString("dd.MM.yyyy");
        QString prev_date_str = previous_date.toString("dd.MM.yyyy");

        qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
        qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

        command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + colume_name + " <= '" + QString::number(msec_val_curr_date) + "'";
    }
    else if(tmp_list_2.at(1).contains(MACRO_Generic_Filter_Condition_Years))
    {
        qint64 years_count = tmp_list_2.at(0).toLongLong();

        QDate current_date = QDate::currentDate();
        QDate previous_date = current_date.addYears(-years_count);

        QString curr_date_str = current_date.toString("dd.MM.yyyy");
        QString prev_date_str = previous_date.toString("dd.MM.yyyy");

        qint64 msec_val_curr_date = QDateTime::fromString(curr_date_str, "dd.MM.yyyy").toSecsSinceEpoch();
        qint64 msec_val_previous_date = QDateTime::fromString(prev_date_str, "dd.MM.yyyy").toSecsSinceEpoch();

        command = "WHERE " + colume_name + " >= '" + QString::number(msec_val_previous_date) + "' AND " + colume_name + " <= '" + QString::number(msec_val_curr_date) + "'";
    }

    return command;
}

QString thread_file_search::get_query_for_file_name_search_in_db(QString column_name)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString command;

    QStringList tmp_list = file_name_str_to_search.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(tmp_list.size() < 2)
        return command;

    QString file_name = tmp_list.at(0);
    QString filter_value = tmp_list.at(1);

    if(file_name.isEmpty())
        return command;

    if(filter_value == MACRO_Generic_Filter_Condition_Matches)
    {
        command = "WHERE " + column_name + " = '" + file_name + "'";
    }
    else if(filter_value == MACRO_Generic_Filter_Condition_Contains)
    {
        command = "WHERE " + column_name + " LIKE '%" + file_name + "%'";
    }
    else if(filter_value == MACRO_Generic_Filter_Condition_Begins_With)
    {
        command = "WHERE " + column_name + " LIKE '" + file_name + "%'";
    }
    else if(filter_value == MACRO_Generic_Filter_Condition_Ends_With)
    {
        command = "WHERE " + column_name + " LIKE '%" + file_name + "'";
    }
    else if(filter_value == MACRO_Generic_Filter_Condition_Is)
    {
        command = "WHERE " + column_name + " = '" + file_name + "'";
    }
    else if(filter_value == MACRO_Generic_Filter_Condition_Is_Not)
    {
        command = "WHERE " + column_name + " NOT LIKE '" + file_name + "'";
    }

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

    return command;
}

QString thread_file_search::get_query_for_file_size_search_in_db(QString column_name)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString command;

    QStringList tmp_list = file_size_str_to_search.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(tmp_list.size() < 2)
        return command;

    if(tmp_list.at(0) == MACRO_Generic_Filter_Condition_Between)
    {
        //46(##)23523[$$]Bytes
        QString second_str = tmp_list.at(1);

        QStringList tmp_list = second_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            return command;

        QString first_size_val = tmp_list.at(0);
        QString third_str_to_split = tmp_list.at(1);

        QStringList tmp_list_2 = third_str_to_split.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

        if(tmp_list_2.size() < 2)
            return command;

        QString second_size_val = tmp_list_2.at(0);
        QString sufix_entered = tmp_list_2.at(1);

        QString first_val_to_search = recon_static_functions::convert_human_readable_size_to_bytes(first_size_val.toLongLong(),sufix_entered, Q_FUNC_INFO);
        QString second_val_to_search = recon_static_functions::convert_human_readable_size_to_bytes(second_size_val.toLongLong(),sufix_entered, Q_FUNC_INFO);

        command = "WHERE " + column_name + " >= '" + first_val_to_search + "' AND " + column_name + " <= '" + second_val_to_search + "'";

    }
    else if(tmp_list.at(0) == MACRO_Generic_Filter_Condition_Is_Equal)
    {
        //244(##)Bytes
        QString second_str = tmp_list.at(1);

        QStringList tmp_list = second_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            return command;

        QString first_size_val = tmp_list.at(0);
        QString suffix_val = tmp_list.at(1);

        QString val_to_search = recon_static_functions::convert_human_readable_size_to_bytes(first_size_val.toLongLong(),suffix_val, Q_FUNC_INFO);

        command = "WHERE " + column_name + " = '" + val_to_search + "'";
    }
    else if(tmp_list.at(0) == MACRO_Generic_Filter_Condition_Is_Greater_Than)
    {
        //244(##)Bytes
        QString second_str = tmp_list.at(1);

        QStringList tmp_list = second_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            return command;

        QString first_size_val = tmp_list.at(0);
        QString suffix_val = tmp_list.at(1);

        QString val_to_search = recon_static_functions::convert_human_readable_size_to_bytes(first_size_val.toLongLong(),suffix_val, Q_FUNC_INFO);

        command = "WHERE " + column_name + " >= '" + val_to_search + "'";
    }
    else if(tmp_list.at(0) == MACRO_Generic_Filter_Condition_Is_Less_Than)
    {
        //244(##)Bytes
        QString second_str = tmp_list.at(1);

        QStringList tmp_list = second_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            return command;

        QString first_size_val = tmp_list.at(0);
        QString suffix_val = tmp_list.at(1);

        QString val_to_search = recon_static_functions::convert_human_readable_size_to_bytes(first_size_val.toLongLong(),suffix_val, Q_FUNC_INFO);

        command = "WHERE " + column_name + " <= '" + val_to_search + "'";
    }

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

    return command;
}

QString thread_file_search::create_file_search_destination_db()
{
    destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + current_search_db_file_name + ".sqlite";

    QString command = "CREATE TABLE files(INT INTEGER PRIMARY KEY, filesystem_record INTEGER, source_count_name VARCHAR(1024))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, destination_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("File Search Db creation --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        return "";
    }

    return destination_db_path;
}

void thread_file_search::insert_file_search_config_into_index_db()
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    current_search_db_file_name = current_search_label_name;
    current_search_db_file_name.replace(" ", "_");

    QString file_search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";
    QString command = "INSERT INTO file_search_index(search_label_name, search_db_file_name, entered_file_name,"
                      "entered_file_size, entered_creation_date_config, entered_modification_date_config,"
                      "entered_added_date_config, entered_last_used_date_config, db_creation_timestamp,"
                      "is_radio_button_or_clicked, mdfind_condition_str) "
                      "VALUES (?,?,?,?,?, ?,?,?,?,?, ?)";

    QString radio_clicked;
    if(is_radio_all_filter_clicked)
        radio_clicked = "true";
    else
        radio_clicked = "false";

    QStringList arg_list;
    arg_list << current_search_label_name << current_search_db_file_name << file_name_str_to_search << file_size_str_to_search
             << change_date_str_to_search << last_modif_date_str << added_date_str_to_search << last_read_date_str_to_search
             << QString::number(current_date_time) << radio_clicked << mdfind_condition_str;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, arg_list, file_search_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("File Search Index Db insertion --------FAILED------ " + file_search_db_path,Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);
}

void thread_file_search::cancel_extraction()
{
    bool_cancel_extraction = true;
}

void thread_file_search::set_bool_cancel_extraction()
{
    bool_cancel_extraction = false;
}

void thread_file_search::set_file_search_label_name(QString name)
{
    current_search_label_name = name;
    current_date_time = QDateTime::currentMSecsSinceEpoch();
}

void thread_file_search::set_file_name_str_to_search(QString name)
{
    file_name_str_to_search = name;
}

void thread_file_search::set_file_size_str_to_search(QString size_str)
{
    file_size_str_to_search = size_str;
}

void thread_file_search::set_change_date_str_to_search(QString date_str)
{
    change_date_str_to_search = date_str;
}

void thread_file_search::set_added_date_str_to_search(QString date_str)
{
    added_date_str_to_search = date_str;
}

void thread_file_search::set_last_modification_str_to_search(QString date_str)
{
    last_modif_date_str = date_str;
}

void thread_file_search::set_last_read_str_to_search(QString date_str)
{
    last_read_date_str_to_search = date_str;
}

void thread_file_search::set_creation_date_str_to_search(QString date_str)
{
    created_date_str_to_search = date_str;
}

void thread_file_search::set_radio_button_or_cheched_search(bool value)
{
    is_radio_all_filter_clicked = value;
}

void thread_file_search::set_parent_dir_path_for_search(QString dir_path)
{
    parent_dir_path = dir_path;
}

void thread_file_search::set_mdfind_condition_str_for_search(QString mdfind_str)
{
    mdfind_condition_str = mdfind_str;
}

void thread_file_search::set_bool_run_in_full_mode(bool run_mode)
{
    bool_run_in_full_mode = run_mode;
}

void thread_file_search::set_file_system_current_source_count_name(QString source_count_name)
{
    file_system_source_count_name = source_count_name;
}

QString thread_file_search::pub_get_current_file_search_label()
{
    return current_search_label_name;
}

void thread_file_search::resume_thread()
{
    pause_thread = false;
    waitcondition.wakeAll();
}

void thread_file_search::stop_thread()
{
    pause_thread = true;
}

void thread_file_search::set_mdfind_extracted_output(QString mdfind_output_str)
{
    mdfind_searched_putput_str = mdfind_output_str;
}
