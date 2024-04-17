#include "thread_artifacts_keyword_search.h"

thread_artifacts_keyword_search::thread_artifacts_keyword_search(QWidget *parent)
{
    bool_cancel_search = false;

    timer_records_k_search = new QElapsedTimer();
    total_records = 0;

    recon_helper_standard_obj = new recon_helper_standard(this);

}


thread_artifacts_keyword_search::~thread_artifacts_keyword_search()
{
    if(timer_records_k_search != NULL)
        delete timer_records_k_search;

}

void thread_artifacts_keyword_search::run()
{
    timer_records_k_search->start();

    search_in_plugins();

}


void thread_artifacts_keyword_search::pub_set_plugin_list_for_search(QStringList list)
{
    list_plugins_for_search = list;
}

void thread_artifacts_keyword_search::pub_set_keywords_list_for_search(QStringList keyword_list)
{
    list_search_keyword = keyword_list;
}


void thread_artifacts_keyword_search::pub_set_essentials()
{
    result_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    destination_search_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";

    insert_index_table_entry();
    create_search_table();

}

void thread_artifacts_keyword_search::search_in_plugins()
{
    recon_static_functions::app_debug(" -START -",Q_FUNC_INFO);

    total_records = 0;

    bool_cancel_search = false;

    for(int i = 0; i < list_plugins_for_search.size(); i++)
    {
        //-1. Itearation for Ever plugin
        if(bool_cancel_search)
        {
            return;
        }

        list_table.clear();list_tab.clear();list_search_on_column.clear();list_display_columns.clear();
        plugin_name = list_plugins_for_search.at(i);


        struct_global_csv_reader_full_csv_info obj =  global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
        QStringList selected_tab_list = recon_static_functions::get_associated_tabs(plugin_name,(global_narad_muni_class_obj->get_field(MACRO_NARAD_Keyword_Search_Selected_Plugins_And_Tabs_QStringList).toStringList()), Q_FUNC_INFO);



        for(int j = 0; j < selected_tab_list.size(); j++)
        {
            for(int  pp = 0 ; pp < obj.tab_name_QStringList_tablewidget.size() ; pp++)
            {
                if(bool_cancel_search)
                {
                    emit signal_increase_progressbar_of_keyword_search(i+1 , QString(MACRO_JobType_Artifacts_Keyword_Search));
                    return;
                }

                if(obj.tab_g_search_available_QStringList_tablewidget.at(pp) != "1")
                    continue;

                if(obj.tab_name_QStringList_tablewidget.at(pp) != selected_tab_list.at(j))
                    continue;

                list_table << obj.table_name_QStringList_tablewidget.at(pp);
                list_tab << obj.tab_name_QStringList_tablewidget.at(pp);


                list_search_on_column << obj.local_search_column_name_QList_tablewidget.at(pp);

                list_display_columns << obj.g_search_col_name_QList.at(pp);

            }
        }

        create_paths_for_plugins();
        //        save_current_search();
        save_current_search_with_the_keyword_hit();
        emit signal_increase_progressbar_of_keyword_search(i+1 , QString(MACRO_JobType_Artifacts_Keyword_Search));

    }

    recon_static_functions::app_debug(" -END -",Q_FUNC_INFO);

}

void thread_artifacts_keyword_search::pub_set_search_name(QString name)
{
    search_name = name;
}

QString thread_artifacts_keyword_search::pub_get_search_name()
{
    return search_name;
}

void thread_artifacts_keyword_search::pub_cancel_keyword_search_extraction()
{
    bool_cancel_search = true;
}

void thread_artifacts_keyword_search::create_paths_for_plugins()
{
    QString pl_name = plugin_name;

    destination_file_location = result_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + pl_name.replace(" ","_");
    source_plugins_db_file = destination_file_location + "/" + pl_name.replace(" ","_") + ".sqlite";
}

void thread_artifacts_keyword_search::create_search_table()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString table_name = search_name;
    table_name = table_name.trimmed().replace(" ","_");
    table_name.replace("-","_");
    table_name.replace(".","_");


    QString key_search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + table_name + ".sqlite";

    current_k_search_db_path = key_search_db_path;

    QString command = "create table keyword_search (INT INTEGER PRIMARY KEY, search_tag varchar(30), searched_keywords varchar(30), plugin varchar(30), category varchar(30), table_name varchar(30), "
                      "os_scheme_display varchar(100),record_no INTEGER, bookmark varchar(2),item1 varchar(300), item2 varchar(300), item3 varchar(300) ,keyword_hit varchar(100), source_count_name varchar(100), recon_tag_value varchar(100), notes varchar(100),notes_icon varchar(100))";


    recon_helper_standard_obj->execute_db_command_by_dbpath(command,key_search_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}
void thread_artifacts_keyword_search::save_current_search()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(source_plugins_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + source_plugins_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase global_srch_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    global_srch_db.setDatabaseName(current_k_search_db_path);
    if(!global_srch_db.open())
    {
        recon_static_functions::app_debug(" global_srch_db Db opening --------FAILED------ " + current_k_search_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + global_srch_db.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QSqlQuery insert_query(global_srch_db);

    QString db_table_name = "keyword_search";
    db_table_name = db_table_name.trimmed().replace(" ","_");


    for(int  i = 0 ; i < list_table.size() ; i++)
    {
        if(bool_cancel_search)
        {
            destination_db.close();
            global_srch_db.close();
            return;
        }

        QString table_name = list_table.at(i);
        QString tab_name = list_tab.at(i);

        QStringList search_oncols_list = list_search_on_column.at(i);
        QStringList cols_nam_list =  list_display_columns.at(i);

        QString command = cols_nam_list.join(",");
        command = "select " + command + " from " + table_name;

        QString partial_command = get_partial_command_forsearch(search_oncols_list,list_search_keyword);
        command = command + partial_command;


        QSqlQuery query_select(destination_db);
        query_select.prepare(command);
        for(int k = 0 ; k < search_oncols_list.size() ; k++)
        {
            if(bool_cancel_search)
            {
                destination_db.close();
                global_srch_db.close();
                return;
            }

            for(int i = 0; i < list_search_keyword.size(); i++)
            {
                if(bool_cancel_search)
                {
                    destination_db.close();
                    global_srch_db.close();
                    return;
                }

                QString keyword_str = list_search_keyword.at(i);
                QString temp_string = "%" + keyword_str.trimmed() + "%";

                query_select.addBindValue(temp_string);
            }
        }


        if(!query_select.exec())
        {
            recon_static_functions::app_debug(" query execution -------------------------FAILED------------ ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error- " + query_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(command,Q_FUNC_INFO);
            return;
        }

        int count = 0;
        while(query_select.next())
        {
            if(bool_cancel_search)
            {
                destination_db.close();
                global_srch_db.close();
                return;
            }

            if(count % 500 == 0)
                QCoreApplication::processEvents();

            insert_query.prepare("insert into '" + db_table_name + "' (search_tag,searched_keywords,record_no,bookmark,plugin,category,table_name,item1,item2,item3) values (?,?,?,?,?, ?,?,?,?,?)");
            insert_query.addBindValue(search_name);
            insert_query.addBindValue(list_search_keyword.join("--"));
            insert_query.addBindValue(query_select.value(1).toString()); // rec
            insert_query.addBindValue("0");
            insert_query.addBindValue(plugin_name);
            insert_query.addBindValue(tab_name);
            insert_query.addBindValue(table_name);
            insert_query.addBindValue(query_select.value(2).toString());
            insert_query.addBindValue(query_select.value(3).toString());
            insert_query.addBindValue(query_select.value(4).toString());
            if(!insert_query.exec())
            {
                recon_static_functions::app_debug(" insert_query execution -----FAILED----- ",Q_FUNC_INFO);
                recon_static_functions::app_debug(" error- " + insert_query.lastError().text(),Q_FUNC_INFO);
            }

            count++;
        }
    }

    destination_db.close();
    global_srch_db.close();
    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void thread_artifacts_keyword_search::save_current_search_with_the_keyword_hit()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(source_plugins_db_file);


    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + source_plugins_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase global_srch_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    global_srch_db.setDatabaseName(current_k_search_db_path);
    if(!global_srch_db.open())
    {
        recon_static_functions::app_debug(" global_srch_db Db opening --------FAILED------ " + current_k_search_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + global_srch_db.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QSqlQuery insert_query(global_srch_db);

    QString db_table_name = "keyword_search";
    db_table_name = db_table_name.trimmed().replace(" ","_");
    db_table_name.replace("-","_");
    db_table_name.replace(".","_");


    for(int li= 0; li < list_search_keyword.size(); li++)
    {
        //A.-Iteration for every Keyword Search

        if(bool_cancel_search)
        {
            destination_db.close();
            global_srch_db.close();
            return;
        }

        //*Taken this stringlist because we can use already build Functions and code code snippet.
        QStringList keyword_for_search;
        QString keyword = list_search_keyword.at(li);
        keyword_for_search << keyword.trimmed();


        for(int  i = 0 ; i < list_table.size() ; i++)
        {
            if(bool_cancel_search)
            {
                destination_db.close();
                global_srch_db.close();
                return;
            }

            QString table_name = list_table.at(i);
            QString tab_name = list_tab.at(i);

            QStringList search_oncols_list = list_search_on_column.at(i);
            QStringList cols_nam_list =  list_display_columns.at(i);

            QString command = cols_nam_list.join(",");

            command = "select recon_tag_value,os_scheme_display," + command + " ,source_count_name , bookmark , notes from '" + table_name + "'";

            QString partial_command = get_partial_command_forsearch(search_oncols_list,keyword_for_search);
            command = command + partial_command;

            QSqlQuery query_select(destination_db);
            query_select.prepare(command);
            for(int k = 0 ; k < search_oncols_list.size() ; k++)
            {
                if(bool_cancel_search)
                {
                    destination_db.close();
                    global_srch_db.close();
                    return;
                }

                for(int i = 0; i < keyword_for_search.size(); i++)
                {
                    if(bool_cancel_search)
                    {
                        destination_db.close();
                        global_srch_db.close();
                        return;
                    }

                    QString temp_string = "%" + keyword_for_search.at(i) + "%";
                    query_select.addBindValue(temp_string);
                }
            }

            if(!query_select.exec())
            {
                recon_static_functions::app_debug(" query execution -------------------------FAILED------------ ",Q_FUNC_INFO);
                recon_static_functions::app_debug(" error- " + query_select.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(command,Q_FUNC_INFO);
                return;
            }

            int count = 0;
            while(query_select.next())
            {
                if(bool_cancel_search)
                {
                    destination_db.close();
                    global_srch_db.close();
                    return;
                }

                if(count % 500 == 0)
                    QCoreApplication::processEvents();

                ///-------------

                insert_query.prepare("insert into '" + db_table_name + "' (search_tag,searched_keywords,record_no,bookmark,plugin,category,table_name,os_scheme_display,item1,item2,item3,keyword_hit,recon_tag_value,source_count_name , notes ) values (?,?,?,?,?,?,?, ?,?,?,?,?,?,?, ?)");
                insert_query.addBindValue(search_name);
                insert_query.addBindValue(list_search_keyword.join("--"));
                insert_query.addBindValue(query_select.value(3).toString()); // rec
                insert_query.addBindValue(query_select.value("bookmark").toString());
                insert_query.addBindValue(plugin_name);
                insert_query.addBindValue(tab_name);
                insert_query.addBindValue(table_name);
                insert_query.addBindValue(query_select.value(1).toString());////add

                insert_query.addBindValue(query_select.value(4).toString());
                insert_query.addBindValue(query_select.value(5).toString());
                insert_query.addBindValue(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value(6).toString(), Q_FUNC_INFO));
                insert_query.addBindValue(keyword);


                insert_query.addBindValue(query_select.value(0).toString());
                insert_query.addBindValue(query_select.value("source_count_name").toString());
                insert_query.addBindValue(query_select.value("notes").toString());

                if(!insert_query.exec())
                {
                    recon_static_functions::app_debug(" insert_query execution -----FAILED----- ",Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error- " + insert_query.lastError().text(),Q_FUNC_INFO);
                }

                ///--------

                count++;
                total_records++;

                if(timer_records_k_search->elapsed() > MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Extracted_Record_Timer_Interval_int){

                    timer_records_k_search->restart();

                }

            }
        }
    }


    destination_db.close();
    global_srch_db.close();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

QString thread_artifacts_keyword_search::get_partial_command_forsearch(QStringList search_cols_list, QStringList search_string_list)
{

    QString partial_command;

    if(search_cols_list.size() > 0)
    {
        partial_command = " where ";

        for(int k = 0 ; k < search_cols_list.size() ; k++)
        {
            for(int j = 0; j < search_string_list.size() ; j++)
            {
                QString searched_keyword = search_cols_list.at(k);
                partial_command  = partial_command + searched_keyword.trimmed() + " LIKE ? OR ";
            }

        }

        partial_command.remove(partial_command.size() - 4 , 4); // OR
    }

    return partial_command;
}


void thread_artifacts_keyword_search::pub_set_bool_search_on_plugin(bool val)
{
    bool_search_on_plugin = val;
}

void thread_artifacts_keyword_search::pub_set_bool_search_on_files(bool val)
{
    bool_search_on_files = val;
}

void thread_artifacts_keyword_search::insert_index_table_entry()
{
    recon_static_functions::app_debug(" - Start ",Q_FUNC_INFO);

    QSqlDatabase global_srch_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    global_srch_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    global_srch_db.setDatabaseName(destination_search_db_file);

    if(!global_srch_db.open())
    {
        recon_static_functions::app_debug(" global_srch_db Db opening --------FAILED------ " + destination_search_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + global_srch_db.lastError().text(),Q_FUNC_INFO);
        global_srch_db.close();
        return;
    }

    QString table_name = search_name;
    table_name = table_name.trimmed().replace(" ","_");
    table_name.replace("-","_");
    table_name.replace(".","_");

    qint64 currentTimestamp = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();

    QSqlQuery query_index(global_srch_db);
    query_index.prepare("insert into 'keyword_search_index' (search_tag,table_name,search_keywords,creation_timestamp) values (?,?,?,?)");
    query_index.addBindValue(search_name);
    query_index.addBindValue(table_name);
    query_index.addBindValue(list_search_keyword.join("--"));
    query_index.addBindValue(currentTimestamp);
    if(!query_index.exec())
    {
        recon_static_functions::app_debug(" query_index execution -------------------------FAILED------------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error- " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query_index.executedQuery(),Q_FUNC_INFO);
        return;
    }

    global_srch_db.close();
    recon_static_functions::app_debug("- END ",Q_FUNC_INFO);
}
