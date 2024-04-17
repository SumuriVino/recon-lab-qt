#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::insert_record_entry_into_tag_search_db_for_file_system(QString record_no, QString tab_name, QString tags_db_path, QString tag_name , QString fs_db_path, QString os_scheme)
{
    QStringList fs_db_path_split_list = fs_db_path.split("/", Qt::SkipEmptyParts);
    QString source_name;

    if(fs_db_path_split_list.size() > 2)
        source_name = fs_db_path_split_list.at(fs_db_path_split_list.size() -2);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(fs_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + fs_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    query.prepare("SELECT " + QString("bookmark,recon_tag_value,notes,filename,filepath,file_size,mime_type,md5_hash,extension,file_parent_path")
                  + " FROM files WHERE INT=?");

    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(!query.next())
    {
        recon_static_functions::app_debug(" --- No Record Exist---",Q_FUNC_INFO);
        return;
    }


    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << record_no << os_scheme;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tags_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QString file_path = query.value(4).toString().trimmed();

        QStringList arg_list;

        arg_list << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << QString("files")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << query.value(2).toString().trimmed() << os_scheme << source_name << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << query.value(3).toString().trimmed() << file_path
                 << query.value(5).toString().trimmed() <<  query.value(6).toString().trimmed() << "";


        QStringList db_columns_list;
        db_columns_list << QString("item0") << QString("item1") << QString("item2") << QString("item3") << QString("item4");

        QString command_insert = get_insert_query_for_tag_search(db_columns_list);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tags_db_path,Q_FUNC_INFO);

        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_File_System) , QString(MACRO_Plugin_File_System_TAB_Name_Files) , tag_name, os_scheme);
    }

    source_db.close();
}

void MainWindow::add_current_record_details_from_file_system_to_tag_search_db(QString tag_name,QString plugin_name,QString tab_name,QString record_no,QString applied_tag_str , QString fs_db_path, QString os_scheme)
{

    // Create DB for every Tags
    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);


    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no << os_scheme;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        insert_record_entry_into_tag_search_db_for_file_system(record_no,tab_name,tag_db_path,tag_name,fs_db_path,os_scheme);
    }
    else
    {
        QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE INT=?";
        QStringList arg_list;
        arg_list << tag_name << INT_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);
    }
}

void MainWindow::update_tag_search_db_and_display_for_remove_tag_for_ram_analysis(QString plugin_name, QString tab_name, QString record, QStringList list_tags_removed, QString os_scheme)
{

    for(int i = 0; i < list_tags_removed.size(); i++)
    {
        QString tag_name = list_tags_removed.at(i);
        QString tag_search_db_name = tag_name;
        tag_search_db_name.replace(" ","_");

        if(tag_search_db_name.isEmpty())
            continue;

        QString tags_result_db_path;
        if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
        else
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_search_db_name + ".sqlite";

        QSqlDatabase destination_db;
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(tags_result_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + tags_result_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QString command = "SELECT INT FROM tags";
        QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,0,destination_db,Q_FUNC_INFO);


        command = "SELECT INT FROM tags WHERE record_no=? AND plugin_name=? AND tab_name=?";
        QStringList arg_list;
        arg_list << record << plugin_name << tab_name ;
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);


        if(!INT_list.isEmpty())
        {
            int INT_pos = INT_list.indexOf(INT_str,0);

            if(INT_pos < 0)
                continue;

            command = "DELETE FROM tags WHERE INT=?";
            arg_list.clear();
            arg_list << INT_str;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);
            common_add_tag_name_into_case_tree(tag_name);
        }

        destination_db.close();
    }
}


void MainWindow::update_notes_value_in_notes_search_db_and_display_from_ram_analysis_saved(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_scheme)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    if(notes_text.trimmed().isEmpty())
        return ;

    if(plugin_name != QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        return ;

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString ram_anlysis_saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() +"ram_analysis_saved.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(ram_anlysis_saved_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("Select INT,bookmark,recon_tag_value,notes,result_name,source_name,operating_system,build_version,artifacts,command_output from tbl_saved_result where INT = ?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query select --------FAILED--------- " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("query Error " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(!query.next())
    {
        recon_static_functions::app_debug("--- No Record Exist---",Q_FUNC_INFO);
        source_db.close();
        return;
    }


    QString command_already_record = "SELECT INT FROM notes WHERE plugin_name=? AND tab_name=? AND record_no=?";
    QStringList arg_list;
    arg_list << plugin_name << tab_name << record_no;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list,0,notes_db_path,Q_FUNC_INFO);

    arg_list.clear();
    if(INT_str.isEmpty())
    {
        QString command_insert = "INSERT INTO notes(record_no,bookmark,recon_tag_value,notes,"
                                 "item0,item1,item2,item3,item4,item5,plugin_name,category,tab_name,os_scheme_display)"
                                 "VALUES (?,?,?,?, ?,?,?,?, ?,?,?,?,?,?)";

        ///record no , bookmark , tag , notes , result name , source name , operating system
        /// build version , artifacts , command output
        arg_list << query.value(0).toString()
                 << query.value(1).toString()
                 << query.value(2).toString()
                 << query.value(3).toString()
                 << query.value(4).toString()
                 << query.value(5).toString()
                 << query.value(6).toString()
                 << query.value(7).toString()
                 << query.value(8).toString()
                 << query.value(9).toString()
                 << plugin_name
                 << plugin_name
                 << tab_name
                 << os_scheme;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , plugin_name ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes),os_scheme );
    }
    else
    {
        QString command_update = "UPDATE notes SET notes = ? WHERE record_no=? AND plugin_name = ? AND tab_name = ? AND os_scheme_display = ?";
        arg_list.clear();
        arg_list << notes_text << record_no << plugin_name << tab_name << os_scheme;

        emit signal_update_tags_search_display_for_singal_record(record_no , plugin_name ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes),os_scheme );
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,notes_db_path,Q_FUNC_INFO);
    }
    source_db.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::update_notes_value_in_ram_analysis_saved_db_and_display_from_notes_search(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_scheme)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);
    if(plugin_name != QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        return ;

    QString ram_analysis_saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";
    QString command = "UPDATE tbl_saved_result SET notes = ? WHERE INT = ?";
    QStringList arg_list;
    arg_list << notes_text << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,ram_analysis_saved_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::update_remained_tag_in_ram_analysis_saved_db_and_display(QString plugin_name, QString tab_name, QString record_no, QString tag_name, QString os_scheme)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    if(plugin_name != QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        return ;

    QString ram_analysis_saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";
    QString command = "UPDATE tbl_saved_result SET recon_tag_value = ? WHERE INT = ? AND plugin_name = ? AND tab_name = ? AND os_scheme_display = ?";

    QStringList arg_list;
    arg_list << tag_name << record_no << plugin_name << tab_name << os_scheme;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,ram_analysis_saved_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::update_bookmark_value_in_bookmarks_db_and_display(QString record_no,QString plugin_name,QString tab_name,QString status , QString fs_db_path, QString os_name_str)
{
    QString bookmarks_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    if(status == "1")
    {
        if(plugin_name == QString(MACRO_Plugin_Name_File_System))
        {
            insert_record_entry_into_tag_search_db_for_file_system(record_no,QString(MACRO_Plugin_File_System_TAB_Name_Files),bookmarks_db_path,QString(MACRO_Tag_Name_Bookmarks), fs_db_path, os_name_str);
        }
        else if(plugin_name == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        {
            insert_record_entry_into_bookmarks_db_for_ram_analysis_saved(record_no,plugin_name,tab_name,status,bookmarks_db_path, fs_db_path, os_name_str, QString(MACRO_Tag_Name_Bookmarks));
        }
        else
        {
            insert_value_into_bookmark_db_for_plugin(record_no,plugin_name,tab_name,status, bookmarks_db_path, os_name_str);
        }

    }
    else if(status == "0")
    {
        QSqlDatabase destination_db;
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(bookmarks_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + bookmarks_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QString command = "SELECT INT FROM tags WHERE record_no=? AND plugin_name=? AND tab_name=?";
        QStringList arg_list;
        arg_list << record_no << plugin_name << tab_name;
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

        command = "DELETE FROM tags WHERE INT=?";
        arg_list.clear();
        arg_list << INT_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);
        destination_db.close();
    }
}

void MainWindow::insert_value_into_bookmark_db_for_plugin(QString record_no,QString plugin_name,QString tab_name,QString status,QString tag_db_path, QString os_name_str)
{

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString destination_plugin_dir_path = result_dir_path  + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);


    QStringList notes_arg_list;
    notes_arg_list << record_no;
    QString command_select_notes = "SELECT notes FROM '" + table_name + "' WHERE INT=?";
    QString notes_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_notes,notes_arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);
    QString command_select_tag = "SELECT recon_tag_value, source_count_name FROM '" + table_name + "' WHERE INT=?";
    QString applied_tags_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,notes_arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);
    QString src_cnt_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,notes_arg_list,1,plugins_destination_db_file,Q_FUNC_INFO);

    QStringList cols_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);

    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString ,0);
    QString record_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(record_index);

    int tag_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString ,0);
    QString tag_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(tag_index);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    QString bookmark_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(bookmark_index);

    int scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    QString os_scheme_col_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(scheme_index);

    QStringList current_table_colum_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);

    if(current_table_colum_list.contains(tag_column_name))
    {
        int index_tmp_tag = current_table_colum_list.indexOf(tag_column_name,0);
        current_table_colum_list.removeAt(index_tmp_tag);
    }

    if(current_table_colum_list.contains(record_column_name))
    {
        int index_tmp_record = current_table_colum_list.indexOf(record_column_name,0);
        current_table_colum_list.removeAt(index_tmp_record);
    }

    if(current_table_colum_list.contains(bookmark_column_name))
    {
        int index_tmp_bookmark = current_table_colum_list.indexOf(bookmark_column_name,0);
        current_table_colum_list.removeAt(index_tmp_bookmark);
    }

    if(current_table_colum_list.contains(os_scheme_col_name))
    {
        int index_os_scheme = current_table_colum_list.indexOf(os_scheme_col_name,0);
        current_table_colum_list.removeAt(index_os_scheme);
    }

    QString time_column_name;
    if(cols_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString))
    {
        int time_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        time_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(time_index);
    }

    QString size_column_name;
    if(cols_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString))
    {
        int size_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString,0);
        size_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(size_index);
    }

    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no << os_name_str;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QStringList tags_specific_arg_list;
        tags_specific_arg_list << (QString(MACRO_Plugin_Tag_Search_TAB_Name_Plugin)) << plugin_name << tab_name << table_name
                               << record_no << applied_tags_str << status << notes_str << os_name_str << src_cnt_name <<plugin_name << tab_name;


        QStringList arg_list = get_column_value_list_from_plugin_db(plugins_destination_db_file,record_no,current_table_colum_list,time_column_name,size_column_name,table_name,tags_specific_arg_list);

        QString command_insert = get_insert_query_for_tag_search(current_table_colum_list);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);

        emit signal_update_tags_search_display_for_singal_record(record_no,plugin_name,tab_name,QString(MACRO_Tag_Name_Bookmarks),os_name_str);
    }
}

void MainWindow::update_bookmark_db_and_display_for_current_record(QString tag_name,QString plugin_name,QString tab_name,QString record_no, QString os_scheme)
{
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    if(!QFileInfo(bookmark_db_path).exists())
        return;

    QString command = "UPDATE 'tags' SET recon_tag_value = ? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list;
    arg_list << tag_name << plugin_name << tab_name << record_no << os_scheme;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,bookmark_db_path,Q_FUNC_INFO);

}

void MainWindow::update_notes_value_in_bookmark_db_and_display(QString record_no,QString plugin_name,QString tab_name,QString status, QString os_scheme)
{
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    if(!QFileInfo(bookmark_db_path).exists())
        return;

    QString command = "UPDATE 'tags' SET notes=? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list;
    arg_list << status << plugin_name << tab_name << record_no << os_scheme;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,bookmark_db_path,Q_FUNC_INFO);

}

void MainWindow::update_bookmark_value_in_keyword_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString status, QString os_scheme)
{
    QString keyword_search_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(keyword_search_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + keyword_search_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT table_name,search_tag FROM keyword_search_index");
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QString db_file_name = query.value(0).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + db_file_name + ".sqlite";

        QStringList arg_list;
        QString command = "UPDATE keyword_search SET bookmark=? WHERE plugin=? AND category=? AND record_no=? AND os_scheme_display =?";
        arg_list << status << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);
    }

    destination_db.close();

}

void MainWindow::update_notes_value_in_redefined_result_full_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString notes_text, QString os_scheme)
{
    QString redff_result_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "index_redefined_result.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(redff_result_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + redff_result_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT db_name,display_name FROM redefined_result_index");
    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QString db_file_name = query.value(0).toString();
        QString display_name = query.value(1).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + db_file_name + ".sqlite";

        QString command = "UPDATE redefined_result SET notes=? WHERE plugin=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << notes_text << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }

    destination_db.close();
}

void MainWindow::update_notes_value_in_keyword_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString notes_text, QString os_scheme)
{
    QString keyword_search_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(keyword_search_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + keyword_search_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT table_name,search_tag FROM keyword_search_index");
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QString db_file_name = query.value(0).toString();
        QString display_name = query.value(1).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + db_file_name + ".sqlite";

        QString command = "UPDATE keyword_search SET notes=? WHERE plugin=? AND category=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << notes_text << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }

    destination_db.close();
}

void MainWindow::update_bookmark_value_in_content_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString status, QString os_scheme)
{
    QString contnt_srch_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(contnt_srch_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT search_db_file_name,search_label_name FROM table_index_content_search");
    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
    }

    if(query.next())
    {
        QString display_name = query.value(0).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + display_name + ".sqlite";

        QString command1 = "select recon_tag_value from files WHERE plugin_name=? AND tab_name=? AND INT=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << plugin_name << tab_name << record_no << os_scheme;
        QString tag_value = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command1,arg_list,0,db_path,Q_FUNC_INFO);
        tag_value = recon_static_functions::get_tags_on_add_or_remove_bookmarks(tag_value,status, Q_FUNC_INFO);

        arg_list.clear();
        QString command = "UPDATE files SET bookmark=? , recon_tag_value = ?  WHERE plugin_name=? AND tab_name=? AND INT=? AND os_scheme_display =?";
        arg_list << status << tag_value << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }
    destination_db.close();
}

void MainWindow::update_keyword_search_db_for_current_record(QString tag_name,QString plugin_name,QString tab_name,QString record_no, QString os_scheme)
{
    QString keyword_search_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(keyword_search_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + keyword_search_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT table_name,search_tag FROM keyword_search_index");
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QString db_file_name = query.value(0).toString();
        QString display_name = query.value(1).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + db_file_name + ".sqlite";

        QString command = "UPDATE keyword_search SET recon_tag_value=? WHERE plugin=? AND category=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << tag_name << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }

    destination_db.close();
}

void MainWindow::update_remained_tag_in_content_search_db_and_display(QString tag_name,QString plugin_name,QString tab_name,QString record_no, QString os_scheme)
{
    QString contnt_srch_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(contnt_srch_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT search_db_file_name,search_label_name FROM table_index_content_search");
    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
    }

    if(query.next())
    {

        QString db_file_name = query.value(0).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + db_file_name + ".sqlite";
        QString command = "UPDATE files SET recon_tag_value=? WHERE plugin_name=? AND tab_name=? AND INT=? AND os_scheme_display =?";

        QStringList arg_list;
        arg_list << tag_name << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }
    destination_db.close();
}


void MainWindow::update_tag_search_db_and_display_for_remove_tag(QString plugin_name, QString tab_name, QString record, QStringList list_tags_removed, QString os_scheme)
{
    for(int i =0; i < list_tags_removed.size(); i++)
    {
        QString tag_name = list_tags_removed.at(i);
        QString tag_search_db_name = tag_name;
        tag_search_db_name.replace(" ","_");

        if(tag_search_db_name.isEmpty())
            continue;

        QString tags_result_db_path;
        if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
        else
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_search_db_name + ".sqlite";

        QSqlDatabase destination_db;
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(tags_result_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + tags_result_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QString command = "SELECT INT FROM tags";
        QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,0,destination_db,Q_FUNC_INFO);


        //command = "SELECT INT FROM tags WHERE record_no=? AND plugin_name=? AND tab_name=? AND os_scheme_display =?";
        command = "SELECT INT FROM tags WHERE record_no=? AND plugin_name=? AND tab_name=?";
        QStringList arg_list;
        //arg_list << record << plugin_name << tab_name << os_scheme;
        arg_list << record << plugin_name << tab_name;
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

        if(!INT_list.isEmpty())
        {
            int INT_pos = INT_list.indexOf(INT_str,0);

            if(INT_pos < 0)
                continue;

            command = "DELETE FROM tags WHERE INT=?";
            arg_list.clear();
            arg_list << INT_str;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);
            common_add_tag_name_into_case_tree(tag_name);
        }

        destination_db.close();
    }
}

void MainWindow::update_notes_db_and_display_for_remove_tag(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme)
{
    QString notes_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + notes_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command = "SELECT INT FROM notes";
    QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,0,destination_db,Q_FUNC_INFO);

    command = "SELECT INT FROM notes WHERE record_no=? AND plugin_name=? AND tab_name=? AND os_scheme_display =?";
    QStringList arg_list;
    arg_list << record << plugin_name << tab_name << os_scheme;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

    if(!INT_list.isEmpty())
    {
        int INT_pos = INT_list.indexOf(INT_str,0);

        if(INT_pos < 0)
        {
            destination_db.close();
            return;
        }


        command = "DELETE FROM notes WHERE INT=?";
        arg_list.clear();
        arg_list << INT_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    }


    destination_db.close();
}

void MainWindow::update_screenshot_db_and_display(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme)
{
    QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

    QString command = "Update tags set notes = ? WHERE plugin_name = ? AND tab_name = ? AND INT = ?";
    QStringList val_list;
    val_list << note_val << plugin_name << tab_name << record;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,val_list,screenshot_db_path,Q_FUNC_INFO);
}



void MainWindow::update_bookmark_value_in_plugin_db(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_name_str)
{

    if(plugin_name == MACRO_Plugin_Name_File_System) // this function is meant to run only for artifacts plugins
        return;

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;


    if(QFileInfo(plugins_destination_db_file).exists() == false)
        return;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;


    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QStringList col_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);

    int bookmark_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    int record_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QStringList col_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);

    QString bookmark_col_name = col_list.at(bookmark_index);

    QString record_col_name = col_list.at(record_index);

    uint64_t record_int = record_no.toULongLong();

    QString command = "UPDATE '" + table_name + "' SET " + bookmark_col_name + "=? WHERE " + record_col_name + "=?";
    QStringList arg_list;
    arg_list << status << QString::number(record_int);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,plugins_destination_db_file,Q_FUNC_INFO);

}

void MainWindow::update_bookmark_value_in_plist_content_info_db(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_name_str)
{

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

    QString command = "UPDATE plist_content_info SET bookmark=? WHERE record_no=?";
    QStringList arg_list;
    arg_list << status << record_no;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);
    return;

}

void MainWindow::update_bookmark_value_in_tag_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QStringList applied_tags_list, QString os_name_str)
{
    for(int i = 0; i < applied_tags_list.size(); i++)
    {
        QString tag_name_str = applied_tags_list.at(i);
        QString tag_search_db_name = tag_name_str;
        tag_search_db_name.replace(" ","_");

        if(tag_search_db_name.isEmpty())
            continue;

        QString tags_result_db_path;
        if(tag_name_str == QString(MACRO_Tag_Name_Bookmarks))
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
        else
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_search_db_name + ".sqlite";

        QString command = "UPDATE tags SET bookmark=? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << status << plugin_name << tab_name << record_no << os_name_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tags_result_db_path,Q_FUNC_INFO);

    }
}


void MainWindow::update_notes_value_in_plugin_db(QString record_no,QString plugin_name,QString tab_name, QString notes_text, QString os_name_str)
{

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QStringList col_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);
    int record_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QStringList col_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);
    QString record_col_name = col_list.at(record_index);

    uint64_t record_int = record_no.toULongLong();

    QString command = "UPDATE '" + table_name + "' SET notes=? WHERE " + record_col_name + "=?";
    QStringList arg_list;
    arg_list << notes_text << QString::number(record_int);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,plugins_destination_db_file,Q_FUNC_INFO);

    emit signal_update_plugin_display_on_remove_notes(plugin_name, tab_name, record_no, notes_text, os_name_str);
}


void MainWindow::update_notes_value_in_plist_content_info_db(QString record_no,QString plugin_name,QString tab_name, QString notes_text, QString os_name_str)
{
    recon_static_functions::app_debug(" Starts", Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

    QString command = "UPDATE plist_content_info SET notes=? WHERE record_no=?";
    QStringList arg_list;
    arg_list << notes_text << record_no;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);

    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);
}
void MainWindow::update_notes_value_in_sqlite_content_info_db(QString record_no,QString plugin_name,QString tab_name, QString notes_text, QString os_name_str)
{
    recon_static_functions::app_debug(" Starts", Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

    QString command = "UPDATE sv_content_info SET notes=? WHERE record_no_in_sqliteviewer=? AND sql_tab_name = ?";
    QStringList arg_list;
    arg_list << notes_text << record_no << tab_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);

    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);
}


void MainWindow::update_bookmark_value_in_notes_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_name_str)
{
    if(plugin_name == QString(MACRO_Plugin_Name_Artifacts_Timeline))
    {
        return ;
    }

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QFileInfo file(db_path);
    if(!file.exists())
    {
        recon_static_functions::app_debug(" ---FAILED--- file not exist " + db_path ,Q_FUNC_INFO);
        return;
    }

    QString command = "UPDATE notes SET bookmark=? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list;
    arg_list << status << plugin_name << tab_name << record_no << os_name_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

}


void MainWindow::update_notes_value_in_tag_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString notes_text,QStringList applied_tags_list, QString os_scheme)
{
    for(int i = 0; i < applied_tags_list.size(); i++)
    {
        QString tag_name_str = applied_tags_list.at(i);
        QString tag_search_db_name = tag_name_str;
        tag_search_db_name.replace(" ","_");

        if(tag_search_db_name.isEmpty())
            continue;

        QString tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_search_db_name + ".sqlite";

        QString command = "UPDATE tags SET notes=? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << notes_text << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tags_result_db_path,Q_FUNC_INFO);

    }
}

void MainWindow::add_tags_value_in_plugin_db(QString tag_name,QString plugin_name,QString tab_name,QString record_no, QString os_name_str)
{

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;


    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QString command = "update '" + table_name + "' set recon_tag_value = ? where INT=?";
    QStringList arg_list;
    arg_list << tag_name << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,plugins_destination_db_file,Q_FUNC_INFO);

}


void MainWindow::add_current_record_details_in_tag_search_db(QString tag_name,QString plugin_name,QString tab_name,QString record_no,QString applied_tag_str, QString os_name_str)
{
    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;


    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    // Create DB for every Tags
    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QStringList notes_arg_list;
    notes_arg_list << record_no;
    QString command_select_notes = "SELECT notes, recon_tag_value, bookmark, source_count_name FROM '" + table_name + "' WHERE INT=?";
    QString notes_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_notes,notes_arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);
    QString applied_tags_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_notes,notes_arg_list,1,plugins_destination_db_file,Q_FUNC_INFO);
    QString bookmark_status_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_notes,notes_arg_list,2,plugins_destination_db_file,Q_FUNC_INFO);
    QString src_cnt_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_notes,notes_arg_list,3,plugins_destination_db_file,Q_FUNC_INFO);


    QStringList cols_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);

    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString record_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(record_index);

    int tag_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    QString tag_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(tag_index);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    QString bookmark_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(bookmark_index);

    int scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    QString os_scheme_col_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(scheme_index);

    QStringList current_table_colum_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);

    if(current_table_colum_list.contains(tag_column_name))
    {
        int index_tmp_tag = current_table_colum_list.indexOf(tag_column_name,0);
        current_table_colum_list.removeAt(index_tmp_tag);
    }

    if(current_table_colum_list.contains(record_column_name))
    {
        int index_tmp_record = current_table_colum_list.indexOf(record_column_name,0);
        current_table_colum_list.removeAt(index_tmp_record);
    }

    if(current_table_colum_list.contains(bookmark_column_name))
    {
        int index_tmp_bookmark = current_table_colum_list.indexOf(bookmark_column_name,0);
        current_table_colum_list.removeAt(index_tmp_bookmark);
    }

    if(current_table_colum_list.contains(os_scheme_col_name))
    {
        int index_os_scheme = current_table_colum_list.indexOf(os_scheme_col_name,0);
        current_table_colum_list.removeAt(index_os_scheme);
    }


    QString time_column_name;
    if(cols_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString))
    {
        int time_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        time_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(time_index);
    }

    QString size_column_name;
    if(cols_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString))
    {
        int size_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString,0);
        size_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(size_index);
    }

    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no << os_name_str;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QStringList tags_specific_arg_list;

        tags_specific_arg_list << (QString(MACRO_Plugin_Tag_Search_TAB_Name_Plugin)) << plugin_name << tab_name << table_name
                               << record_no << applied_tags_str << bookmark_status_str << notes_str << os_name_str << src_cnt_name <<plugin_name << tab_name;

        QStringList arg_list = get_column_value_list_from_plugin_db(plugins_destination_db_file,record_no,current_table_colum_list,time_column_name,size_column_name,table_name,tags_specific_arg_list);

        QString command_insert = get_insert_query_for_tag_search(current_table_colum_list);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);

        emit signal_update_tags_search_display_for_singal_record(record_no,plugin_name,tab_name,tag_name,os_name_str);
    }
    else
    {
        QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE INT=?";
        QStringList arg_list;
        arg_list << applied_tags_str << INT_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);

    }
}

void MainWindow::update_remained_tags_in_plugin_db(QString tag_name,QString plugin_name,QString tab_name,QString record_no, QString os_scheme)
{
    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file;
    QString table_name;
    if(plugin_name == QString(MACRO_Plugin_Name_Screenshots))
    {
        plugins_destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

        table_name = "tags";
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Plist_Viewer))
    {
        plugins_destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

        table_name = tab_name;

        //=========================== Close plist viewer on remove tag from 'Tag display'   - Start ===========================//
        QString command = QString("select viewer_display_name from plist_viewer_index where db_table_name = ?");

        QStringList list_values;
        list_values << table_name;
        QString viewer_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, list_values,0, plugins_destination_db_file, Q_FUNC_INFO);

        slot_remove_entry_file_type_based_in_stack_and_refresh_widget(MACRO_CASE_TREE_Plist_Viewer,viewer_name);
        //=========================== Close plist viewer on remove tag from 'Tag display'   - End ===========================//

    }
    else
    {
        plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;
        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            return;

        table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    }

    QString command = "update '" + table_name + "' set recon_tag_value =NULL where INT=?";
    QStringList arg_list;
    arg_list  << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,plugins_destination_db_file,Q_FUNC_INFO);
}


QStringList MainWindow::get_column_value_list_from_plugin_db(QString plugin_db_file,QString record_val,QStringList current_table_colum_list, QString timestamp_column_name, QString file_size_column_name,QString table_name,QStringList tag_search_arg_value_list)
{
    recon_static_functions::debug_intensive(" -START",Q_FUNC_INFO);

    QStringList arg_list;
    int time_column_index_in_other_list = 0,size_column_index = 0;
    if(current_table_colum_list.contains(timestamp_column_name))
    {
        time_column_index_in_other_list = current_table_colum_list.indexOf(timestamp_column_name,0);
    }

    if(current_table_colum_list.contains(file_size_column_name))
    {
        size_column_index = current_table_colum_list.indexOf(file_size_column_name,0);
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(plugin_db_file);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + plugin_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error" + source_db.lastError().text(),Q_FUNC_INFO);
        return arg_list;
    }

    QSqlQuery query(source_db);
    QString column_names_str = current_table_colum_list.join(",");
    query.prepare("SELECT " + column_names_str + " FROM '" + table_name + "' WHERE INT=?");
    query.addBindValue(record_val);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return arg_list;
    }

    for(int i = 0; i < tag_search_arg_value_list.size(); i++)
    {
        arg_list << tag_search_arg_value_list.at(i);
    }

    while(query.next())
    {
        for(int i = 0; i < current_table_colum_list.size(); i++)
        {
            if(current_table_colum_list.contains(timestamp_column_name))
            {
                if(i == time_column_index_in_other_list)
                {
                    QString time_epch = query.value(i).toString();
                    QString after_conversion = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(time_epch, Q_FUNC_INFO);
                    arg_list << after_conversion;
                    continue;
                }
            }

            if(current_table_colum_list.contains(file_size_column_name))
            {
                if(i == size_column_index)
                {
                    QString size_without_convert = query.value(i).toString();
                    QString size_after_convert = recon_static_functions::convert_bytes_to_kilobytes_for_display(size_without_convert, Q_FUNC_INFO);

                    arg_list << size_after_convert;
                    continue;
                }
            }

            arg_list << query.value(i).toString();
        }
    }

    source_db.close();

    recon_static_functions::debug_intensive(" -End",Q_FUNC_INFO);

    return arg_list;
}


QString MainWindow::get_insert_query_for_tag_search(QStringList current_table_colum_list)
{
    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,bookmark,"
                      "notes,os_scheme_display,source_count_name,csv_plugin_name, csv_tab_name,";
    QString value_str = " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,";


    for(int i =0; i < current_table_colum_list.size(); i++)
    {
        command.append("item" + QString::number(i));
        command.append(",");

        value_str.append("?");
        value_str.append(",");
    }

    if(command.endsWith(","))
        command.chop(1);

    if(value_str.endsWith(","))
        value_str.chop(1);

    command.append(")");
    value_str.append(")");

    command.append(value_str);

    return command;
}


void MainWindow::update_remained_tags_in_tag_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme)
{
    QString remained_tags_str = list_remained_tags.join(",");

    for(int i = 0; i < list_remained_tags.size(); i++)
    {
        QString tag_name = list_remained_tags.at(i);
        QString tag_search_db_name = tag_name;
        tag_search_db_name.replace(" ","_");

        if(tag_name.isEmpty())
            continue;

        QString tags_result_db_path;
        if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
        else
            tags_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_search_db_name + ".sqlite";

        QString command = "delete from tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list  << plugin_name << tab_name << record_no << os_scheme;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tags_result_db_path,Q_FUNC_INFO);
        slot_common_add_tag_name_into_case_tree(tag_name);

    }
}



QString MainWindow::get_insert_query_for_notes_search(QStringList current_table_colum_list, QString tablename)
{

    QString command = "INSERT INTO '" + tablename + "'(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,bookmark,"
                                                    "notes,os_scheme_display,source_count_name,";
    QString value_str = " VALUES (?,?,?,?,?,?,?,?,?,?,";

    for(int i =0; i < current_table_colum_list.size(); i++)
    {
        command.append("item" + QString::number(i));
        command.append(",");

        value_str.append("?");
        value_str.append(",");
    }

    if(command.endsWith(","))
        command.chop(1);

    if(value_str.endsWith(","))
        value_str.chop(1);

    command.append(")");
    value_str.append(")");

    command.append(value_str);

    return command;
}

void MainWindow::insert_record_entry_into_tag_search_db_for_content_search(QString record_no, QString plugin_name, QString tab_name, QString status, QString tag_db_path, QString indexed_db_path, QString os_name_str, QString tag_name)
{
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(indexed_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + indexed_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark, recon_tag_value, notes, file_name, file_path, no_of_hits, keyword_name,"
                  "hit_locations, source_count_name FROM files WHERE INT=?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(!query.next())
    {
        recon_static_functions::app_debug("--- No Record Exist---",Q_FUNC_INFO);
        source_db.close();
        return;
    }


    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no << os_name_str;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QString file_path = query.value(4).toString().trimmed();

        QString source_db_path = indexed_db_path;
        if(source_db_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
            source_db_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());

        if(!source_db_path.startsWith("/"))
            source_db_path.prepend("/");

        QStringList arg_list;
        arg_list << QString(MACRO_Plugin_Name_File_System) << plugin_name << tab_name << QString("files")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << query.value(2).toString().trimmed() << os_name_str << query.value(8).toString().trimmed()
                 << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_Name_File_System)
                 << query.value(3).toString().trimmed() << file_path << source_db_path << query.value(5).toString().trimmed()
                 << query.value(6).toString().trimmed() << query.value(7).toString().trimmed();

        QStringList db_columns_list;
        db_columns_list << QString("item0") << QString("item1") << QString("item2") << QString("item3") << QString("item4") << QString("item5");

        QString command_insert = get_insert_query_for_tag_search(db_columns_list);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);

        emit signal_update_tags_search_display_for_singal_record(record_no , plugin_name, tab_name, tag_name, os_name_str);
    }

    source_db.close();
}
