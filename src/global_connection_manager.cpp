#include "global_connection_manager.h"

global_connection_manager::global_connection_manager(QObject *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

void global_connection_manager::pub_set_bookmark_in_fs_source_db(QStringList INT_list, QString source_count_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_filesystem, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_bookmark_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    pub_delete_bookmark_for_file_system(list_st_global_bookmark_db_updation);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.csv_plugin_name);
        m_query.addBindValue(obj.csv_tab_name);
        m_query.addBindValue(obj.recon_tag_value);
        m_query.addBindValue(obj.notes);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_fs_source_db(QStringList INT_list, QString source_count_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(count % 50 == 0)
            QCoreApplication::processEvents();


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_filesystem, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ?" ;

    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.source_count_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_fs_source_db(QStringList INT_list, QString source_count_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set recon_tag_value ='") + tag_name +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_filesystem, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_fs_source_db(QStringList INT_list, QString source_count_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set recon_tag_value ='") + "" +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_filesystem, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}
void global_connection_manager::pub_add_tag_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);


    delete_file_entry_from_all_tag_DBs(list_st_global_bookmark_db_updation);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.bookmark);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.csv_plugin_name);
        m_query.addBindValue(obj.csv_tab_name);
        m_query.addBindValue(tag_name);
        m_query.addBindValue(obj.notes);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj)
{
    delete_file_entry_from_all_tag_DBs(obj);
}

void global_connection_manager::pub_set_notes_in_fs_source_db(QStringList INT_list, QString source_count_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set notes ='") + notes_value +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_filesystem, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_fs_source_db(QStringList INT_list, QString source_count_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set notes ='") + "" +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_filesystem, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_notes_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    delete_file_entry_from_all_notes_DBs(list_st_global_bookmark_db_updation);

    QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?)";


    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.bookmark);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.csv_plugin_name);
        m_query.addBindValue(notes_value);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj)
{
    delete_file_entry_from_all_notes_DBs(obj);
    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);

}

void global_connection_manager::pub_set_bookmark_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString tmp_cmd;
    int pos = 0;

    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";


    QString command = QString("Update '") + table_name +  "' Set bookmark = '1' Where ";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plugin, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString tmp_cmd;
    int pos = 0;

    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";


    QString command = QString("Update '") + table_name +  "' Set bookmark = '0' Where ";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plugin, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_bookmark_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name)
{
    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";


    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    // Remove previous existense
    pub_delete_bookmark_for_plugin_viewer(INT_list, plugin_name, tab_name, "");

    QStringList cols_data_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);


    QString time_column_name;
    if(cols_data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString))
    {
        int time_index = cols_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        time_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(time_index);
    }

    QString size_column_name;
    if(cols_data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString))
    {
        int size_index = cols_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString,0);
        size_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(size_index);
    }

    QStringList display_column_name_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);

    display_column_name_list.removeAll("os_scheme_display");
    display_column_name_list.removeAll("INT");
    display_column_name_list.removeAll("system_user");
    display_column_name_list.removeAll("bookmark");
    display_column_name_list.removeAll("recon_tag_value");
    display_column_name_list.removeAll("notes");
    display_column_name_list.removeAll("source_file");


    int column_to_be_sent_count = display_column_name_list.size();

    display_column_name_list << "source_count_name";


    QString connection_naam = QString(Q_FUNC_INFO) + "_1";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_plugin = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_plugin.setDatabaseName(db_path_plugin);

    if(!destination_db_plugin.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_plugin.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_plugin.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_plugin(destination_db_plugin);

    connection_naam = QString(Q_FUNC_INFO) + "_2";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_bookmark = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_bookmark.setDatabaseName(bookmark_db_path);

    if(!destination_db_bookmark.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_bookmark.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_bookmark.lastError().text(),Q_FUNC_INFO);
        destination_db_plugin.close();
        return;
    }

    destination_db_bookmark.transaction();

    QSqlQuery query_bookmark(destination_db_bookmark);


    for(int int_count = 0; int_count < INT_list.size(); int_count++)
    {

        QString command_for_plugin = "select " + display_column_name_list.join(",") + " from " + table_name + " where INT = ?";

        query_plugin.prepare(command_for_plugin);
        query_plugin.addBindValue(INT_list.at(int_count));

        if(!query_plugin.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db_plugin.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_plugin.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_plugin.executedQuery(),Q_FUNC_INFO);
            continue;
        }

        if(!query_plugin.next())
            continue;

        QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                          "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5, item6, item7, item8, item9) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?, ?, ?,?,?, ?,?)";


        query_bookmark.clear();
        query_bookmark.prepare(command);
        query_bookmark.addBindValue(QString(MACRO_Plugin_Tag_Search_TAB_Name_Plugin));
        query_bookmark.addBindValue(plugin_name);
        query_bookmark.addBindValue(tab_name);
        query_bookmark.addBindValue(table_name);
        query_bookmark.addBindValue(INT_list.at(int_count));
        query_bookmark.addBindValue("1");
        query_bookmark.addBindValue(query_plugin.value("source_count_name").toString());
        query_bookmark.addBindValue(plugin_name);
        query_bookmark.addBindValue(tab_name);
        query_bookmark.addBindValue(""); //
        query_bookmark.addBindValue(""); //
        query_bookmark.addBindValue(""); //


        query_bookmark.addBindValue(query_plugin.value(0).toString());



        for(int i = 1 ; i < 10; i++)
        {
            if(column_to_be_sent_count > i)
            {
                if(query_plugin.record().field(i).name() == time_column_name)
                {
                    query_bookmark.addBindValue(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_plugin.value(i).toString(), Q_FUNC_INFO));
                }
                else if(query_plugin.record().field(i).name() == size_column_name)
                {
                    query_bookmark.addBindValue(recon_static_functions::human_readable_size(query_plugin.value(i).toString(), Q_FUNC_INFO));
                }
                else
                    query_bookmark.addBindValue(query_plugin.value(i).toString());
            }
            else
                query_bookmark.addBindValue("");

        }


        if(!query_bookmark.exec())
        {
            recon_static_functions::app_debug(" query_bookmark deletion ---FAILED--- " + destination_db_bookmark.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_bookmark.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_bookmark.executedQuery(),Q_FUNC_INFO);
            continue;
        }

    }


    destination_db_bookmark.commit();

    destination_db_bookmark.close();
    destination_db_plugin.close();


    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);

}

void global_connection_manager::pub_delete_bookmark_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString source_count_name)
{
    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO) + "_2";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_bookmark = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_bookmark.setDatabaseName(bookmark_db_path);

    if(!destination_db_bookmark.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_bookmark.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_bookmark.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db_bookmark.transaction();

    QSqlQuery query_bookmark(destination_db_bookmark);

    if(source_count_name == QString(""))
        query_bookmark.prepare("delete from tags where plugin_name = ? AND tab_name = ? AND record_no = ?");
    else
        query_bookmark.prepare("delete from tags where plugin_name = ? AND tab_name = ? AND record_no = ? AND source_count_name = ?");


    for(int int_count = 0; int_count < INT_list.size(); int_count++)
    {
        query_bookmark.addBindValue(plugin_name);
        query_bookmark.addBindValue(tab_name);
        query_bookmark.addBindValue(INT_list.at(int_count));

        if(source_count_name != QString(""))
        {
            query_bookmark.addBindValue(source_count_name);
        }


        if(!query_bookmark.exec())
        {
            recon_static_functions::app_debug(" query_bookmark deletion ---FAILED--- " + destination_db_bookmark.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_bookmark.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_bookmark.executedQuery(),Q_FUNC_INFO);
            continue;
        }

    }


    destination_db_bookmark.commit();

    destination_db_bookmark.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
}

void global_connection_manager::pub_set_tag_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString tmp_cmd;
    int pos = 0;

    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";


    QString command = QString("Update '") + table_name +  "' Set recon_tag_value = '" + tag_name + "'  Where ";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plugin, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString tmp_cmd;
    int pos = 0;

    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";


    QString command = QString("Update '") + table_name +  "' Set recon_tag_value = '' Where ";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plugin, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}
void global_connection_manager::pub_add_tag_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString tag_name)
{
    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);


    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;


    pub_delete_tag_for_plugin_viewer(INT_list, plugin_name, tab_name, "");

    QStringList cols_data_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);


    QString time_column_name;
    if(cols_data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString))
    {
        int time_index = cols_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        time_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(time_index);
    }

    QString size_column_name;
    if(cols_data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString))
    {
        int size_index = cols_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString,0);
        size_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(size_index);
    }

    QStringList display_column_name_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);

    display_column_name_list.removeAll("os_scheme_display");
    display_column_name_list.removeAll("INT");
    display_column_name_list.removeAll("system_user");
    display_column_name_list.removeAll("bookmark");
    display_column_name_list.removeAll("recon_tag_value");
    display_column_name_list.removeAll("notes");
    display_column_name_list.removeAll("source_file");


    int column_to_be_sent_count = display_column_name_list.size();

    display_column_name_list << "source_count_name";


    QString connection_naam = QString(Q_FUNC_INFO) + "_1";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_plugin = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_plugin.setDatabaseName(db_path_plugin);

    if(!destination_db_plugin.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_plugin.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_plugin.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_plugin(destination_db_plugin);

    connection_naam = QString(Q_FUNC_INFO) + "_2";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_tag = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_tag.setDatabaseName(tag_db_path);

    if(!destination_db_tag.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_tag.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_tag.lastError().text(),Q_FUNC_INFO);
        destination_db_plugin.close();
        return;
    }

    destination_db_tag.transaction();

    QSqlQuery query_tag(destination_db_tag);


    for(int int_count = 0; int_count < INT_list.size(); int_count++)
    {

        QString command_for_plugin = "select " + display_column_name_list.join(",") + " from " + table_name + " where INT = ?";

        query_plugin.prepare(command_for_plugin);
        query_plugin.addBindValue(INT_list.at(int_count));

        if(!query_plugin.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db_plugin.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_plugin.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_plugin.executedQuery(),Q_FUNC_INFO);
            continue;
        }

        if(!query_plugin.next())
            continue;

        QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                          "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5, item6, item7, item8, item9) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?, ?, ?,?,?, ?,?)";


        query_tag.clear();
        query_tag.prepare(command);
        query_tag.addBindValue(QString(MACRO_Plugin_Tag_Search_TAB_Name_Plugin));
        query_tag.addBindValue(plugin_name);
        query_tag.addBindValue(tab_name);
        query_tag.addBindValue(table_name);
        query_tag.addBindValue(INT_list.at(int_count));
        query_tag.addBindValue("");
        query_tag.addBindValue(query_plugin.value("source_count_name").toString());
        query_tag.addBindValue(plugin_name);
        query_tag.addBindValue(tab_name);
        query_tag.addBindValue(""); //
        query_tag.addBindValue(""); //
        query_tag.addBindValue(""); //


        query_tag.addBindValue(query_plugin.value(0).toString());



        for(int i = 1 ; i < 10; i++)
        {
            if(column_to_be_sent_count > i)
            {
                if(query_plugin.record().field(i).name() == time_column_name)
                {
                    query_tag.addBindValue(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_plugin.value(i).toString(), Q_FUNC_INFO));
                }
                else if(query_plugin.record().field(i).name() == size_column_name)
                {
                    query_tag.addBindValue(recon_static_functions::human_readable_size(query_plugin.value(i).toString(), Q_FUNC_INFO));
                }
                else
                    query_tag.addBindValue(query_plugin.value(i).toString());
            }
            else
                query_tag.addBindValue("");

        }


        if(!query_tag.exec())
        {
            recon_static_functions::app_debug(" query_bookmark deletion ---FAILED--- " + destination_db_tag.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_tag.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_tag.executedQuery(),Q_FUNC_INFO);
            continue;
        }

    }


    destination_db_tag.commit();

    destination_db_tag.close();
    destination_db_plugin.close();


    emit signal_common_add_tag_name_into_case_tree(tag_name);

}

void global_connection_manager::pub_delete_tag_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString source_count_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);



    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ?" ;

        for(int tt = 0; tt < INT_list.size(); tt++)
        {
            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(plugin_name);
            m_query.addBindValue(tab_name);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }

        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString tmp_cmd;
    int pos = 0;

    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";


    QString command = QString("Update '") + table_name +  "' Set notes = '" + notes_value + "'  Where ";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plugin, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString tmp_cmd;
    int pos = 0;

    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);
    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";

    QString command = QString("Update '") + table_name +  "' Set notes = '' Where ";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plugin, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}
void global_connection_manager::pub_add_notes_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString notes_value)
{
    pub_delete_notes_for_plugin_viewer(INT_list,plugin_name, tab_name, "");


    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);

    QString db_path_plugin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str + "/" + plugin_name_str + ".sqlite";

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";


    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;


    QStringList cols_data_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);


    QString time_column_name;
    if(cols_data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString))
    {
        int time_index = cols_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        time_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(time_index);
    }

    QString size_column_name;
    if(cols_data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString))
    {
        int size_index = cols_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString,0);
        size_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(size_index);
    }

    QStringList display_column_name_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);

    display_column_name_list.removeAll("os_scheme_display");
    display_column_name_list.removeAll("INT");
    display_column_name_list.removeAll("system_user");
    display_column_name_list.removeAll("bookmark");
    display_column_name_list.removeAll("recon_tag_value");
    display_column_name_list.removeAll("notes");
    display_column_name_list.removeAll("source_file");


    int column_to_be_sent_count = display_column_name_list.size();

    display_column_name_list << "source_count_name";


    QString connection_naam = QString(Q_FUNC_INFO) + "_1";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_plugin = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_plugin.setDatabaseName(db_path_plugin);

    if(!destination_db_plugin.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_plugin.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_plugin.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_plugin(destination_db_plugin);

    connection_naam = QString(Q_FUNC_INFO) + "_2";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_tag = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_tag.setDatabaseName(notes_db_path);

    if(!destination_db_tag.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_tag.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_tag.lastError().text(),Q_FUNC_INFO);
        destination_db_plugin.close();
        return;
    }

    destination_db_tag.transaction();

    QSqlQuery query_tag(destination_db_tag);


    for(int int_count = 0; int_count < INT_list.size(); int_count++)
    {

        QString command_for_plugin = "select " + display_column_name_list.join(",") + " from " + table_name + " where INT = ?";

        query_plugin.prepare(command_for_plugin);
        query_plugin.addBindValue(INT_list.at(int_count));

        if(!query_plugin.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db_plugin.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_plugin.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_plugin.executedQuery(),Q_FUNC_INFO);
            continue;
        }

        if(!query_plugin.next())
            continue;

        QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                          "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5, item6, item7, item8, item9) VALUES (?,?,?, ?,?,?, ?,?,?, ?, ?,?,?,?, ?, ?,?,?, ?,?)";


        query_tag.clear();
        query_tag.prepare(command);
        query_tag.addBindValue(QString(MACRO_Plugin_Tag_Search_TAB_Name_Plugin));
        query_tag.addBindValue(plugin_name);
        query_tag.addBindValue(tab_name);
        query_tag.addBindValue(table_name);
        query_tag.addBindValue(INT_list.at(int_count));
        query_tag.addBindValue("");
        query_tag.addBindValue(query_plugin.value("source_count_name").toString());
        query_tag.addBindValue(""); //
        query_tag.addBindValue(notes_value); //
        query_tag.addBindValue(""); //


        query_tag.addBindValue(query_plugin.value(0).toString());



        for(int i = 1 ; i < 10; i++)
        {
            if(column_to_be_sent_count > i)
            {
                if(query_plugin.record().field(i).name() == time_column_name)
                {
                    query_tag.addBindValue(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_plugin.value(i).toString(), Q_FUNC_INFO));
                }
                else if(query_plugin.record().field(i).name() == size_column_name)
                {
                    query_tag.addBindValue(recon_static_functions::human_readable_size(query_plugin.value(i).toString(), Q_FUNC_INFO));
                }
                else
                    query_tag.addBindValue(query_plugin.value(i).toString());
            }
            else
                query_tag.addBindValue("");

        }


        if(!query_tag.exec())
        {
            recon_static_functions::app_debug(" query_bookmark deletion ---FAILED--- " + destination_db_tag.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_tag.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_tag.executedQuery(),Q_FUNC_INFO);
            continue;
        }

    }


    destination_db_tag.commit();

    destination_db_tag.close();
    destination_db_plugin.close();


    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);

}

void global_connection_manager::pub_delete_notes_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString source_count_name)
{
    QString plugin_name_str = plugin_name;
    plugin_name_str = plugin_name_str.replace(" ", "_");

    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO) + "_2";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db_notes = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db_notes.setDatabaseName(dest_db_path);

    if(!destination_db_notes.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db_notes.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db_notes.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db_notes.transaction();

    QSqlQuery query_notes(destination_db_notes);

    if(source_count_name == QString(""))
        query_notes.prepare("delete from notes where plugin_name = ? AND tab_name = ? AND record_no = ?");
    else
        query_notes.prepare("delete from notes where plugin_name = ? AND tab_name = ? AND record_no = ? AND source_count_name = ?");


    for(int int_count = 0; int_count < INT_list.size(); int_count++)
    {
        query_notes.addBindValue(plugin_name);
        query_notes.addBindValue(tab_name);
        query_notes.addBindValue(INT_list.at(int_count));

        if(source_count_name != QString(""))
        {
            query_notes.addBindValue(source_count_name);
        }


        if(!query_notes.exec())
        {
            recon_static_functions::app_debug(" query_notes deletion ---FAILED--- " + destination_db_notes.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query_notes.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + query_notes.executedQuery(),Q_FUNC_INFO);
            continue;
        }

    }


    destination_db_notes.commit();

    destination_db_notes.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
}

void global_connection_manager::pub_set_bookmark_in_artifact_timeline_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update timeline_data Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_artifact_timeline = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_artifact_timeline, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_artifact_timeline_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update timeline_data Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_artifact_timeline = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_artifact_timeline, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_artifact_timeline_db_from_outside(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString command = QString("Update timeline_data Set bookmark = '0' Where record_no = ? AND plugin = ? AND category = ? AND stamp_type = ?");

    QString artifact_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(artifact_timeline_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    m_query.prepare(command);

    for(int count = 0; count < timeline_record_list.size(); count++)
    {
        QString record_no = timeline_record_list.at(count).plugin_record_no;
        QString plugin = timeline_record_list.at(count).plugin_name;
        QString tab = timeline_record_list.at(count).tab_name;
        QString stamp_type = timeline_record_list.at(count).item5;

        m_query.addBindValue(record_no);
        m_query.addBindValue(plugin);
        m_query.addBindValue(tab);
        m_query.addBindValue(stamp_type);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }


    }

    destination_db.close();
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void global_connection_manager::pub_add_bookmark_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    pub_delete_bookmark_for_artifact_timeline(timeline_record_list);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5) VALUES (?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < timeline_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_artifact_timeline_record obj = timeline_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event);
        m_query.addBindValue(MACRO_Plugin_Name_Artifacts_Timeline);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event);
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_record_no);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(MACRO_Plugin_Name_Artifacts_Timeline);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Timeline);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND item0 = ? AND item1 = ? AND item5 = ? AND source_count_name = ?";


    for(int tt = 0; tt < timeline_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_artifact_timeline_record obj = timeline_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);

        m_query.addBindValue(obj.plugin_record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.source_count_name);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_artifact_timeline_db(QStringList INT_list, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update timeline_data Set recon_tag_value ='") + tag_name +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString artifact_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_timeline_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_artifact_timeline_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update timeline_data Set recon_tag_value = '' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_artifact_timeline = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_artifact_timeline, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_artifact_timeline_db_from_outside(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString command = QString("Update timeline_data Set recon_tag_value = '' Where record_no = ? AND plugin = ? AND category = ? AND stamp_type = ?");

    QString artifact_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(artifact_timeline_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    m_query.prepare(command);

    for(int count = 0; count < timeline_record_list.size(); count++)
    {
        QString record_no = timeline_record_list.at(count).plugin_record_no;
        QString plugin = timeline_record_list.at(count).plugin_name;
        QString tab = timeline_record_list.at(count).tab_name;
        QString stamp_type = timeline_record_list.at(count).item5;

        m_query.addBindValue(record_no);
        m_query.addBindValue(plugin);
        m_query.addBindValue(tab);
        m_query.addBindValue(stamp_type);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }


    }

    destination_db.close();
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}
void global_connection_manager::pub_add_tag_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    pub_delete_tag_for_artifact_timeline(timeline_record_list);

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5) VALUES (?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < timeline_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_artifact_timeline_record obj = timeline_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event);
        m_query.addBindValue(MACRO_Plugin_Name_Artifacts_Timeline);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event);
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_record_no);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(MACRO_Plugin_Name_Artifacts_Timeline);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Timeline);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND item0 = ? AND item1 = ? AND item5 = ? AND source_count_name = ?";


        for(int tt = 0; tt < timeline_record_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            struct_global_tagging_artifact_timeline_record obj = timeline_record_list.at(tt);

            m_query.clear();
            m_query.prepare(command);

            m_query.addBindValue(obj.plugin_record_no);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.item5);
            m_query.addBindValue(obj.source_count_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_artifact_timeline_db(QStringList INT_list, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update timeline_data Set notes ='") + notes_value +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString artifact_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_timeline_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_artifact_timeline_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update timeline_data Set notes = '' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_artifact_timeline = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_artifact_timeline, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_artifact_timeline_db_from_outside(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString command = QString("Update timeline_data Set notes = '' Where record_no = ? AND plugin = ? AND category = ? AND stamp_type = ?");

    QString artifact_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()
            + "artifacts_timeline_full.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(artifact_timeline_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    m_query.prepare(command);

    for(int count = 0; count < timeline_record_list.size(); count++)
    {
        QString record_no = timeline_record_list.at(count).plugin_record_no;
        QString plugin = timeline_record_list.at(count).plugin_name;
        QString tab = timeline_record_list.at(count).tab_name;
        QString stamp_type = timeline_record_list.at(count).item5;

        m_query.addBindValue(record_no);
        m_query.addBindValue(plugin);
        m_query.addBindValue(tab);
        m_query.addBindValue(stamp_type);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }


    }

    destination_db.close();
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_notes_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    pub_delete_notes_for_artifact_timeline(timeline_record_list);

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5) VALUES ( ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < timeline_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_artifact_timeline_record obj = timeline_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event);
        m_query.addBindValue(MACRO_Plugin_Name_Artifacts_Timeline);
        m_query.addBindValue(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event);
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_record_no);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record> timeline_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND item0 = ? AND item1 = ? AND item5 = ? AND source_count_name = ?";


    for(int tt = 0; tt < timeline_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_artifact_timeline_record obj = timeline_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);

        m_query.addBindValue(obj.plugin_record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.source_count_name);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_bookmark_in_redefined_db(QStringList INT_list, QString plugin_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update redefined_result Set bookmark = '1' Where ");


    QString  db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";



    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_redefined_result, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_add_bookmark_for_redefined_db(QList<struct_global_tagging_redefined_result_record> redefined_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < redefined_record_list.size(); tt++)
    {
        struct_global_tagging_redefined_result_record obj = redefined_record_list.at(tt);
        pub_delete_bookmark_for_redefined_db(QStringList(obj.INT_record) , obj.redefined_plugin_name);
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5) VALUES (?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < redefined_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_redefined_result_record obj = redefined_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue(obj.redefined_plugin_name);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.redefined_plugin_name);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_redefined_db(QStringList INT_list, QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(plugin_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_remove_bookmark_in_redefined_db(QStringList INT_list , QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update redefined_result Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;



    QString  db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";




    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_redefined_result, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}


void global_connection_manager::pub_set_tag_in_redefined_db(QStringList INT_list, QString plugin_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update redefined_result Set recon_tag_value ='") + tag_name +  "' Where ";


    QString  db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_redefined_result, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_redefined_db(QStringList INT_list, QString plugin_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update redefined_result Set recon_tag_value = '' Where ");


    QString  db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_redefined_result, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_redefined(QList<struct_global_tagging_redefined_result_record> redefined_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < redefined_record_list.size(); tt++)
    {
        struct_global_tagging_redefined_result_record obj = redefined_record_list.at(tt);
        pub_delete_tag_for_redefined(QStringList(obj.INT_record),obj.redefined_plugin_name);
    }




    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5) VALUES (?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < redefined_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_redefined_result_record obj = redefined_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue(obj.redefined_plugin_name);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.redefined_plugin_name);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_redefined(QStringList INT_list, QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(plugin_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_redefined_db(QStringList INT_list, QString plugin_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update redefined_result Set notes ='") + notes_value +  "' Where ";

    QString  db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_redefined_result, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_redefined_db(QStringList INT_list, QString plugin_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update redefined_result Set notes = '' Where ");
    QString  db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_redefined_result, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_redefined_db(QList<struct_global_tagging_redefined_result_record> redefined_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < redefined_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_redefined_result_record obj_1 = redefined_record_list.at(tt);

        pub_delete_notes_for_redefined_db(QStringList(obj_1.INT_record) , obj_1.redefined_plugin_name);
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < redefined_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_redefined_result_record obj = redefined_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue(obj.redefined_plugin_name);
        m_query.addBindValue(obj.redefined_tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_redefined_db(QStringList INT_list, QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(plugin_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::delete_file_entry_from_all_tag_DBs(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);



    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ?" ;

        for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
        {
            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);
            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(obj.record_no);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.source_count_name);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }

        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::delete_file_entry_from_all_notes_DBs(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QCoreApplication::processEvents();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);

    QString command = "Delete from notes where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ?" ;

    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.source_count_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();
    destination_db.close();


    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void global_connection_manager::pub_set_notes_in_saved_graphs_and_screenshot_db(QStringList INT_list, QString plugin_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command ;

    QString db_path_saved_graphs_path;
    if(plugin_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
    {
        command = QString("Update saved_graphs Set notes ='") + notes_value +  "' Where ";
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
    {
        command = QString("Update saved_graphs Set notes ='") + notes_value +  "' Where ";
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
    {
        command = QString("Update tags Set notes ='") + notes_value +  "' Where ";
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Screenshots)
    {
        command = QString("Update tags Set notes ='") + notes_value +  "' Where ";
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Saved_Maps)
    {
        command = QString("Update tags Set notes ='") + notes_value +  "' Where ";
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
    }


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_saved_graphs_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_saved_graphs_and_screenshot_db(QStringList INT_list, QString plugin_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command;
    int bunch_count = 50;

    QString  db_path_saved_graphs_path;
    if(plugin_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
    {
        command = QString("Update saved_graphs Set notes = '' Where ");
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
    {
        command = QString("Update saved_graphs Set notes = '' Where ");
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
    {
        command = QString("Update tags Set notes = '' Where ");
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Screenshots)
    {
        command = QString("Update tags Set notes = '' Where ");
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
    }
    else if(plugin_name == MACRO_Plugin_Name_Saved_Maps)
    {
        command = QString("Update tags Set notes = '' Where ");
        db_path_saved_graphs_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
    }


    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_saved_graphs_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_saved_graphs_and_screenshot_db(QList<struct_global_tagging_saved_graph_record> saved_graphs_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < saved_graphs_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_saved_graph_record obj_1 = saved_graphs_record_list.at(tt);

        pub_delete_notes_for_saved_graphs_and_screenshot_db(QStringList(obj_1.INT_record) , obj_1.plugin_name);
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,item4) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?,?)";


    for(int tt = 0; tt < saved_graphs_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_saved_graph_record obj = saved_graphs_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_saved_graphs_and_screenshot_db(QStringList INT_list, QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(plugin_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_data_in_export_content_string_viewer_and_text_viewer_db(QList<struct_global_tagging_string_viewer_record> string_viewer_record_list , QString tag_name , QString plugin_name)
{

    QString exported_content_info_db;
    QString command;
    if(plugin_name == MACRO_Plugin_Name_Text_Viewer)
    {
        exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
        command = ("INSERT INTO text_content_info(INT,bookmark , file_name, file_path,file_size,"
                   "recon_tag_value,color_hex,content_file_name, source_count_name,text_data,source_file,export_file_path,recon_filefrom,recon_file_infopath) VALUES(?,?,?,?,?,?,?,?,?, ?,?,?,?,?)");
    }
    else if(plugin_name == MACRO_Plugin_Name_Strings_Viewer)
    {
        exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
        command= ("INSERT INTO string_content_info(INT,bookmark , file_name, file_path,file_size,"
                  "recon_tag_value,color_hex,content_file_name, source_count_name,string_data,source_file,export_file_path,recon_filefrom,recon_file_infopath) VALUES(?,?,?,?,?,?,?,?,?, ?,?,?,?,?)");
    }
    else if(plugin_name == MACRO_Plugin_Email_Parser_TAB_Name_Email_Data)
    {
        exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString() + "email_tag_data.sqlite";

        command= ("INSERT INTO emlx_content_info(record_no,bookmark,file_name, file_path,file_size,"
                  "recon_tag_value,color_hex,content_file_name, source_count_name,text_data,source_file,export_file_path,recon_filefrom,recon_file_infopath) VALUES(?,?,?,?,?,?,?,?,?, ?,?,?,?,?)");

    }




    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);
    for(int tt = 0; tt < string_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_string_viewer_record obj = string_viewer_record_list.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("0");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(tag_name);
        m_query.addBindValue(obj.hex_color);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.export_file_path);
        m_query.addBindValue(obj.recon_filefrom);
        m_query.addBindValue(obj.recon_file_infopath);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();

}

void global_connection_manager::pub_delete_tag_export_content_string_viewer_and_text_viewer_db(QStringList INT_list, QString content_file_name , QString plugin_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    if(plugin_name == MACRO_Plugin_Name_Text_Viewer)
    {
        exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
        command = QString("Delete from text_content_info Where ");
    }
    else if(plugin_name == MACRO_Plugin_Name_Strings_Viewer)
    {
        exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
        command = QString("Delete from string_content_info Where ");
    }

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("content_file_name = '");
        tmp_cmd.append(content_file_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_string_viewer_and_text_viewer(QList<struct_global_tagging_string_viewer_record> string_viewer_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4) VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < string_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_string_viewer_record obj = string_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_string_viewer_and_text_viewer(QStringList INT_list, QString content_file_name, QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ? AND item3 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(plugin_name);
            m_query.addBindValue(content_file_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void global_connection_manager::pub_add_data_in_export_content_plist_viewer_db(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list, QString tag_name, QString notes_text)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);


    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {
        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);
        pub_delete_entry_export_content_plist_viewer_db(QStringList(obj.INT_record),obj.item5);
    }



    QString exported_content_info_db;
    QString command;

    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
    command = QString("INSERT INTO 'plist_content_info'( record_no,bookmark,key,value, type, keypath,file_name, file_path, file_size, recon_tag_value,"
                      "color_hex, source_count_name, viewer_display_name, notes, source_file ,recon_filefrom ,recon_file_infopath) VALUES(?,?,?,?,?,?,? ,?,?,?,?,?, ?,?, ?,?,?)");


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);
    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue(obj.bookmark_status);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.keypath);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.file_size);
        m_query.addBindValue(tag_name);
        m_query.addBindValue(obj.color_hex);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(notes_text);
        m_query.addBindValue(obj.source_file);
        m_query.addBindValue(obj.recon_file_from);
        m_query.addBindValue(obj.recon_file_infopath);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();
    destination_db.close();

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_plist_viewer_db( QStringList INT_list , QString table_name , QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '" +  table_name + "' Set recon_tag_value ='") + tag_name +  "' Where ";

    QString  db_path_plist_viewer;
    db_path_plist_viewer   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plist_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_plist_viewer_db(QStringList INT_list, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '" + table_name + "' Set recon_tag_value = '' Where ");


    QString  db_path_plist_viewer_db_path;
    db_path_plist_viewer_db_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plist_viewer_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_delete_entry_export_content_plist_viewer_db(QStringList INT_list, QString display_tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
    command = QString("delete from plist_content_info WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("viewer_display_name = '");
        tmp_cmd.append(display_tab_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_update_tag_export_content_plist_viewer_db(QStringList INT_list, QString display_tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
    command = QString("update plist_content_info Set recon_tag_value = '' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("viewer_display_name = '");
        tmp_cmd.append(display_tab_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_plist_viewer(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {
        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);
        pub_delete_tag_for_plist_viewer(QStringList(obj.INT_record),obj.item5);
    }


    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4 , item5 , item6 , item7) VALUES (?,?,?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?)";


    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.notes_text);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.plist_table_name);
        m_query.addBindValue(obj.bookmark_status);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();

    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_plist_viewer(QStringList INT_list, QString display_tab_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ? AND item5 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(MACRO_Plugin_Name_Plist_Viewer);
            m_query.addBindValue(display_tab_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_bookmark_in_plist_viewer_db(QStringList INT_list, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '" +  table_name + "' Set bookmark = '1' Where ");

    QString  db_path_plist_viewer;
    db_path_plist_viewer   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plist_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_plist_viewer_db(QStringList INT_list, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '" +  table_name + "' Set bookmark = '0' Where ");

    QString  db_path_plist_viewer;
    db_path_plist_viewer   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plist_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_update_bookmark_export_content_plist_viewer_db(QStringList INT_list, QString display_tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
    command = QString("update plist_content_info Set bookmark = '0' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("viewer_display_name = '");
        tmp_cmd.append(display_tab_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_bookmark_for_plist_viewer(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {
        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);
        pub_delete_bookmark_for_plist_viewer(QStringList(obj.INT_record),obj.item5);
    }

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display"
                      ",item0,item1,item2,item3, item4 , item5 , item6 , item7) "
                      "VALUES (?,?,?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?)";


    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.notes_text);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.plist_table_name);
        m_query.addBindValue(obj.tag_status);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();

    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_plist_viewer(QStringList INT_list, QString display_tab_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND plugin_name = ? AND item5 = ?";
    m_query.prepare(command);
    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();


        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Plist_Viewer);
        m_query.addBindValue(display_tab_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_data_in_export_content_log_viewer_db(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist, QString tag_name, QString plugin_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString log_viewer_exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";

    QString insert_command = (" INSERT INTO log_content_info(bookmark, record_no, tagged_data, file_name, file_path,"
                              " recon_filefrom, recon_file_infopath, file_size, recon_tag_value,"
                              " color_value, source_count_name, viewer_display_name, notes, source_file)"
                              " VALUES(0,?,?,?,?,?,?,?, ?,?,?,?,?,?)");

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(log_viewer_exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_query(destination_db);
    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_log_viewer_record obj = log_viewer_record_qlist.at(tt);

        insert_query.clear();
        insert_query.prepare(insert_command);

        insert_query.addBindValue(obj.INT_record);
        insert_query.addBindValue(obj.tagged_data_str);
        insert_query.addBindValue(obj.log_file_name);
        insert_query.addBindValue(obj.source_file_path);
        insert_query.addBindValue(obj.recon_file_from);
        insert_query.addBindValue(obj.recon_file_infopath);

        insert_query.addBindValue(obj.file_size_str);
        insert_query.addBindValue(tag_name);
        insert_query.addBindValue(obj.colour_value);
        insert_query.addBindValue(obj.source_count_name);
        insert_query.addBindValue(obj.viewer_display_name);
        insert_query.addBindValue(obj.notes_text);
        insert_query.addBindValue(obj.source_file_path);

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + insert_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + insert_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_data_in_tags_db_for_log_viewer(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_query_tags(destination_db);

    QString command = " INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      " source_count_name, csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5, item6)"
                      " VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?,?)";

    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_log_viewer_record obj = log_viewer_record_qlist.at(tt);

        insert_query_tags.clear();
        insert_query_tags.prepare(command);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue(obj.plugin_name);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue(obj.INT_record);
        insert_query_tags.addBindValue("1");
        insert_query_tags.addBindValue(obj.source_count_name);
        insert_query_tags.addBindValue(obj.plugin_name);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue(obj.log_file_name);
        insert_query_tags.addBindValue(obj.source_file_path);
        insert_query_tags.addBindValue(obj.tagged_data_str);
        insert_query_tags.addBindValue(obj.item3);
        insert_query_tags.addBindValue(obj.file_size_str);

        insert_query_tags.addBindValue(obj.viewer_display_name);
        insert_query_tags.addBindValue(obj.log_table_name);

        if(!insert_query_tags.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + insert_query_tags.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + insert_query_tags.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_log_viewer_db(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist, QString table_name , QString tag_name, QString colour_value, QString destination_db_path)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug( destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_query(destination_db);
    QString insert_command = "insert into '" + table_name +"'(record_no, tagged_data, cursor_start_index, cursor_end_index,"
                                                           " filepath, recon_tag_value, colour_value) values(?,?,?,?, ?,?,?)";

    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_log_viewer_record strct_log_viewer_rec_obj = log_viewer_record_qlist.at(tt);

        insert_query.clear();
        insert_query.prepare(insert_command);
        insert_query.addBindValue(strct_log_viewer_rec_obj.INT_record);
        insert_query.addBindValue(strct_log_viewer_rec_obj.tagged_data_str); //tagged_data
        insert_query.addBindValue(QString::number(strct_log_viewer_rec_obj.cursor_start_index));

        insert_query.addBindValue(QString::number(strct_log_viewer_rec_obj.cursor_end_index));
        insert_query.addBindValue(strct_log_viewer_rec_obj.source_file_path);
        insert_query.addBindValue(tag_name);
        insert_query.addBindValue(colour_value);

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" insert_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + insert_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Executed query " + insert_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();
    destination_db.close();

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_log_viewer_db(QStringList INT_list, QString table_name, QString tagged_data_str, QString current_tag)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString delete_command = "Delete from '" + table_name + "' WHERE ";

    QString log_viewer_db_path;
    log_viewer_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

    QString tmp_dlt_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_dlt_cmd = delete_command;

        tmp_dlt_cmd.append("record_no = ");
        tmp_dlt_cmd.append(record_no);
        tmp_dlt_cmd.append(" AND ");
        tmp_dlt_cmd.append("tagged_data = '");
        tmp_dlt_cmd.append(tagged_data_str);
        tmp_dlt_cmd.append("'");
        tmp_dlt_cmd.append(" AND ");
        tmp_dlt_cmd.append("recon_tag_value = '");
        tmp_dlt_cmd.append(current_tag);
        tmp_dlt_cmd.append("'");
        tmp_dlt_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_dlt_cmd.endsWith(" OR "))
                tmp_dlt_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_dlt_cmd, log_viewer_db_path, Q_FUNC_INFO);
            tmp_dlt_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_update_tag_export_content_log_viewer_db(QStringList INT_list, QString display_tab_name, QString tagged_data_str)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db;
    exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";
    command = QString("update log_content_info Set recon_tag_value = '' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("viewer_display_name = '");
        tmp_cmd.append(display_tab_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" AND ");
        tmp_cmd.append("tagged_data = '");
        tmp_cmd.append(tagged_data_str);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}


void global_connection_manager::pub_update_notes_export_content_log_viewer_db(QStringList INT_list, QString display_tab_name, QString notes_value, QString tagged_data_str)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString update_command;
    QString exported_content_info_db;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";
    update_command = QString("UPDATE log_content_info Set notes = '" + notes_value + "' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = update_command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("viewer_display_name = '");
        tmp_cmd.append(display_tab_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" AND ");
        tmp_cmd.append("tagged_data = '");
        tmp_cmd.append(tagged_data_str);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_notes_for_log_viewer(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_log_viewer_record obj_1 = log_viewer_record_qlist.at(tt);

        pub_delete_notes_for_log_viewer(QStringList(obj_1.INT_record) , obj_1.viewer_display_name, obj_1.tagged_data_str);
    }


    QSqlQuery m_query(destination_db);

    QString insert_command = " INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,"
                             " source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,"
                             " item2, item3, item4, item5, item6, item7)"
                             " VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?)";


    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_log_viewer_record obj = log_viewer_record_qlist.at(tt);

        m_query.clear();
        m_query.prepare(insert_command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.notes_text);
        m_query.addBindValue("");
        m_query.addBindValue(obj.log_file_name);
        m_query.addBindValue(obj.source_file_path);
        m_query.addBindValue(obj.tagged_data_str);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.file_size_str);
        m_query.addBindValue(obj.viewer_display_name);
        m_query.addBindValue(obj.log_table_name);
        m_query.addBindValue(obj.tag_status);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_log_viewer(QStringList INT_list, QString display_tab_name, QString tagged_data_str)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery delete_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ? AND item5 = ? AND item2 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        delete_query.clear();
        delete_query.prepare(command);
        delete_query.addBindValue(INT_list.at(tt));
        delete_query.addBindValue(MACRO_Plugin_Name_Log_Viewer);
        delete_query.addBindValue(display_tab_name);
        delete_query.addBindValue(tagged_data_str);

        if(!delete_query.exec())
        {
            recon_static_functions::app_debug(" delete_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + delete_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + delete_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_tags_search_for_log_viewer(QStringList INT_list, QString display_tab_name, QString notes_value, QString tagged_data)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString select_command = "SELECT tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_command, 1, tag_index_db, Q_FUNC_INFO);

    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {
        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;

        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery update_query(destination_db);

        QString command = "update tags set notes = ?  where record_no = ? AND plugin_name = ? AND item5 = ? AND item2 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {
            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            update_query.clear();
            update_query.prepare(command);
            update_query.addBindValue(notes_value);
            update_query.addBindValue(INT_list.at(tt));
            update_query.addBindValue(MACRO_Plugin_Name_Log_Viewer);
            update_query.addBindValue(display_tab_name);
            update_query.addBindValue(tagged_data);

            if(!update_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + update_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + update_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_bookmark_in_carved_password_db(QStringList INT_list , QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_carved_password_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_passwords = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString command = QString("Update carved_password Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_passwords, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_carved_password_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_carved_password_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_passwords = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString command = QString("Update carved_password Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;



    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_passwords, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_bookmark_for_carved_password_db(QList<struct_global_tagging_carved_passwords> carved_passwords_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < carved_passwords_record_list.size(); tt++)
    {
        struct_global_tagging_carved_passwords obj = carved_passwords_record_list.at(tt);
        pub_delete_bookmark_for_carved_password_db(QStringList(obj.INT_record),obj.item4);
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,item4) VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_passwords_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_passwords obj = carved_passwords_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_carved_password_db(QStringList INT_list , QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND item4 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(tab_label_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_set_tag_in_carved_password_db(QStringList INT_list, QString tab_label_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;


    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_carved_password_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_passwords = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString command = QString("Update carved_password Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_passwords, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_carved_password_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update carved_password Set recon_tag_value = '' Where ");
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_carved_password_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_password = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_password, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_carved_password(QList<struct_global_tagging_carved_passwords> carved_passwords_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < carved_passwords_record_list.size(); tt++)
    {
        struct_global_tagging_carved_passwords obj = carved_passwords_record_list.at(tt);
        pub_delete_tag_for_carved_password(QStringList(obj.INT_record),obj.item4);
    }



    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,item4) VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_passwords_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_passwords obj = carved_passwords_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }



    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_carved_password(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND item4 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(tab_label_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_carved_password_db(QStringList INT_list, QString tab_label_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update carved_password Set notes ='") + notes_value +  "' Where ";

    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_carved_password_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_password = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_password, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_carved_password_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update carved_password Set notes = '' Where ");

    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_carved_password_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_password = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_password, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_carved_password_db(QList<struct_global_tagging_carved_passwords> carved_password_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < carved_password_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_carved_passwords obj_1 = carved_password_record_list.at(tt);
        pub_delete_notes_for_carved_password_db(QStringList(obj_1.INT_record) , obj_1.item4);
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4) VALUES (?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_password_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_passwords obj = carved_password_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_carved_password_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND item4 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(tab_label_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_bookmark_in_carved_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString command = QString("Update files Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_carved_files_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString command = QString("Update files Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;



    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_bookmark_for_carved_files_db(QList<struct_global_tagging_carved_files> carved_files_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < carved_files_record_list.size(); tt++)
    {
        struct_global_tagging_carved_files obj = carved_files_record_list.at(tt);
        pub_delete_bookmark_for_carved_files_db(QStringList(obj.INT_record),obj.item3);
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_files_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_files obj = carved_files_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_carved_files_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND item3 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(tab_label_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_carved_files_db(QStringList INT_list, QString tab_label_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;


    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString command = QString("Update files Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_carved_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update files Set recon_tag_value = '' Where ");
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_carved_files(QList<struct_global_tagging_carved_files> carved_files_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < carved_files_record_list.size(); tt++)
    {
        struct_global_tagging_carved_files obj = carved_files_record_list.at(tt);
        pub_delete_tag_for_carved_files(QStringList(obj.INT_record),obj.item3);
    }



    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_files_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_files obj = carved_files_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }



    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_carved_files(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND item3 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(tab_label_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_carved_files_db(QStringList INT_list, QString tab_label_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update files Set notes ='") + notes_value +  "' Where ";

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_carved_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update files Set notes = '' Where ");

    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_carved_files_db(QList<struct_global_tagging_carved_files> carved_files_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < carved_files_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_carved_files obj_1 = carved_files_record_list.at(tt);
        pub_delete_notes_for_carved_files_db(QStringList(obj_1.INT_record) , obj_1.item3);
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_files_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_files obj = carved_files_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_carved_files_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND item3 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(tab_label_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_set_bookmark_in_registry_viewer_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    if(relative_db_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("End ", Q_FUNC_INFO);
        return ;
    }

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_registry_viewer = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString command = QString("Update tbl_registry_tree Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_registry_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_registry_viewer_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    if(relative_db_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("End ", Q_FUNC_INFO);
        return ;
    }

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_registry_viewer = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString command = QString("Update tbl_registry_tree Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;



    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_registry_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_bookmark_for_registry_viewer_db(QList<struct_global_tagging_registry_viewer> registry_viewer_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < registry_viewer_record_list.size(); tt++)
    {
        struct_global_tagging_registry_viewer obj = registry_viewer_record_list.at(tt);
        pub_delete_bookmark_for_registry_viewer_db(QStringList(obj.INT_record),obj.item11);
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,"
                      "item4,item5,item6,item7,item11) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?)";


    for(int tt = 0; tt < registry_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_registry_viewer obj = registry_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");

        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item11);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_registry_viewer_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND item11 = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(tab_label_name);
        m_query.addBindValue(QString(MACRO_Plugin_Name_Registry_Viewer));

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_registry_viewer_db(QStringList INT_list, QString tab_label_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update tbl_registry_tree Set notes ='") + notes_value +  "' Where ";

    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    if(relative_db_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("End ", Q_FUNC_INFO);
        return ;
    }

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_registry_viewer = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_registry_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_registry_viewer_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update tbl_registry_tree Set notes = '' Where ");

    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    if(relative_db_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("End ", Q_FUNC_INFO);
        return ;
    }
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_registry_viewer = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_registry_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_registry_viewer_db(QList<struct_global_tagging_registry_viewer> registry_viewer_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < registry_viewer_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_registry_viewer obj_1 = registry_viewer_record_list.at(tt);
        pub_delete_notes_for_registry_viewer_db(QStringList(obj_1.INT_record) , obj_1.item11);
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,"
                      "item4 , item5 ,item6 , item7 , item11) VALUES (?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,? ,?,?,?,?,?)";


    for(int tt = 0; tt < registry_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_registry_viewer obj = registry_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item11);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_registry_viewer_db(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND item11 = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(tab_label_name);
        m_query.addBindValue(MACRO_Plugin_Name_Registry_Viewer);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_set_tag_in_registry_viewer_db(QStringList INT_list, QString tab_label_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;


    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    if(relative_db_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("End ", Q_FUNC_INFO);
        return ;
    }

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_registry_viewer = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString command = QString("Update tbl_registry_tree Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_registry_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_registry_viewer_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update tbl_registry_tree Set recon_tag_value = '' Where ");
    QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QString cmd = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);
    if(relative_db_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("End ", Q_FUNC_INFO);
        return ;
    }
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path_registry_viewer = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_registry_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_registry_viewer(QList<struct_global_tagging_registry_viewer> registry_viewer_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < registry_viewer_record_list.size(); tt++)
    {
        struct_global_tagging_registry_viewer obj = registry_viewer_record_list.at(tt);
        pub_delete_tag_for_registry_viewer(QStringList(obj.INT_record),obj.item11);
    }



    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,"
                      "item4 , item5, item6, item7, item11) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,? ,?,?,?,?,?)";


    for(int tt = 0; tt < registry_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_registry_viewer obj = registry_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item11);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }



    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_registry_viewer(QStringList INT_list, QString tab_label_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND item11 = ? AND plugin_name = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(tab_label_name);
            m_query.addBindValue(MACRO_Plugin_Name_Registry_Viewer);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void global_connection_manager::pub_set_bookmark_in_Mobile_Backup_db(QStringList INT_list )
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString db_path_mobile_backup  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
    QString command = QString("Update tbl_backup_list Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_mobile_backup, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_Mobile_Backup_db(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString db_path_mobile_backup  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";

    QString command = QString("Update tbl_backup_list Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;



    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_mobile_backup, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_bookmark_for_Mobile_Backup_db(QList<struct_global_tagging_mobile_backup> mobile_backup_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < mobile_backup_record_list.size(); tt++)
    {
        struct_global_tagging_mobile_backup obj = mobile_backup_record_list.at(tt);
        pub_delete_bookmark_for_Mobile_Backup_db(QStringList(obj.INT_record));
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,item4,"
                      "item5 ,item6 ,item7 ,item8 ,item9 ,item10 ,item11 ,item12) VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,? ,?,?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < mobile_backup_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_mobile_backup obj = mobile_backup_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item8);
        m_query.addBindValue(obj.item9);
        m_query.addBindValue(obj.item10);
        m_query.addBindValue(obj.item11);
        m_query.addBindValue(obj.item12);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_Mobile_Backup_db(QStringList INT_list )
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Mobile_Backup);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_set_tag_in_Mobile_Backup_db(QStringList INT_list, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString db_path_mobile_backup  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";

    QString command = QString("Update tbl_backup_list Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_mobile_backup, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_Mobile_Backup_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString db_path_mobile_backup  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
    QString command = QString("Update tbl_backup_list Set recon_tag_value = '' Where ");

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_mobile_backup, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_Mobile_Backup(QList<struct_global_tagging_mobile_backup> mobile_backup_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < mobile_backup_record_list.size(); tt++)
    {
        struct_global_tagging_mobile_backup obj = mobile_backup_record_list.at(tt);
        pub_delete_tag_for_Mobile_Backup(QStringList(obj.INT_record));
    }



    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3,item4,"
                      "item5,item6,item7,item8,item9,item10,item11,item12) VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,? , ?,?,?,? , ?,?,?,?)";


    for(int tt = 0; tt < mobile_backup_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_mobile_backup obj = mobile_backup_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item8);
        m_query.addBindValue(obj.item9);
        m_query.addBindValue(obj.item10);
        m_query.addBindValue(obj.item11);
        m_query.addBindValue(obj.item12);


        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }



    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_Mobile_Backup(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(MACRO_Plugin_Name_Mobile_Backup);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}



void global_connection_manager::pub_set_notes_in_Mobile_Backup_db(QStringList INT_list, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString db_path_mobile_backup  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
    QString command = QString("Update tbl_backup_list Set notes ='") + notes_value +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_mobile_backup, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_Mobile_Backup_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString db_path_mobile_backup  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
    QString command = QString("Update tbl_backup_list Set notes = '' Where ");

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_mobile_backup, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_Mobile_Backup_db(QList<struct_global_tagging_mobile_backup> mobile_backup_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < mobile_backup_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_mobile_backup obj_1 = mobile_backup_record_list.at(tt);
        pub_delete_notes_for_Mobile_Backup_db(QStringList(obj_1.INT_record));
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4,"
                      "item5,item6,item7,item8,item9,item10,item11,item12) VALUES (?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,? ,?,?,?,? ,?,?,?,?)";


    for(int tt = 0; tt < mobile_backup_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_mobile_backup obj = mobile_backup_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item8);
        m_query.addBindValue(obj.item9);
        m_query.addBindValue(obj.item10);
        m_query.addBindValue(obj.item11);
        m_query.addBindValue(obj.item12);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_Mobile_Backup_db(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Mobile_Backup);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_set_bookmark_in_disk_images_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString rtrvd_srcs_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    QString command = QString("Update tbl_sources_list Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, rtrvd_srcs_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_bookmark_for_disk_images_db(QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < rtrvd_srcs_record_list.size(); tt++)
    {
        struct_global_tagging_disk_images obj = rtrvd_srcs_record_list.at(tt);
        pub_delete_bookmark_for_disk_images_db(QStringList(obj.INT_record));
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2"
                      ") VALUES (?,?,?,?,? ,?,?,?,?,?, ?,?,?,?,?)";


    for(int tt = 0; tt < rtrvd_srcs_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_disk_images obj = rtrvd_srcs_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);

        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Executed query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_disk_images_db(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Disk_Images);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Executed query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_disk_images_db(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString rtrvd_srcs_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";

    QString command = QString("Update tbl_sources_list Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, rtrvd_srcs_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_remove_notes_in_disk_images_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString rtrvd_srcs_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    QString command = QString("Update tbl_sources_list Set notes = '' Where ");

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, rtrvd_srcs_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_delete_notes_for_disk_images_db(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Disk_Images);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_disk_images_db(QStringList INT_list, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString rtrvd_srcs_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";

    QString command = QString("Update tbl_sources_list Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, rtrvd_srcs_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_tag_for_disk_images(QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < rtrvd_srcs_record_list.size(); tt++)
    {
        struct_global_tagging_disk_images obj = rtrvd_srcs_record_list.at(tt);
        pub_delete_tag_for_disk_images(QStringList(obj.INT_record));
    }



    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2"
                      ") VALUES (?,?,?,?,? ,?,?,?,?,? ,?,?,?,?,?)";


    for(int tt = 0; tt < rtrvd_srcs_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_disk_images obj = rtrvd_srcs_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);


        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }



    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_disk_images(QStringList INT_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(MACRO_Plugin_Name_Disk_Images);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_disk_images_db(QStringList INT_list, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString rtrvd_srcs_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    QString command = QString("Update tbl_sources_list Set notes ='") + notes_value +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, rtrvd_srcs_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_notes_for_disk_images_db(QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < rtrvd_srcs_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_disk_images obj_1 = rtrvd_srcs_record_list.at(tt);
        pub_delete_notes_for_disk_images_db(QStringList(obj_1.INT_record));
    }


    QSqlQuery m_query(destination_db);


    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2"
                      ") VALUES (?,?,?,?,? ,?,?,?,?,? ,?,?,?)";


    for(int tt = 0; tt < rtrvd_srcs_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_disk_images obj = rtrvd_srcs_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_disk_images_db(QStringList INT_list)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString rtrvd_srcs_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    QString command = QString("Update tbl_sources_list Set recon_tag_value = '' Where ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, rtrvd_srcs_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_plist_viewer_db(QStringList INT_list, QString table_name , QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '" + table_name +"' Set notes ='") + notes_value +  "' Where ";

    QString  db_path_plist_viewer;
    db_path_plist_viewer   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plist_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_plist_viewer_db(QStringList INT_list, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '" + table_name + "' Set notes = '' Where ");
    QString  db_path_plist_viewer;
    db_path_plist_viewer   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_plist_viewer, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_update_notes_export_content_plist_viewer_db(QStringList INT_list, QString display_tab_name , QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
    command = QString("update plist_content_info Set notes = '"+ notes_value +"' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("viewer_display_name = '");
        tmp_cmd.append(display_tab_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_plist_viewer(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_plist_viewer_record obj_1 = plist_viewer_record_list.at(tt);

        pub_delete_notes_for_plist_viewer(QStringList(obj_1.INT_record) , obj_1.item5);
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, "
                      "item4, item5,item6 , item7)"
                      " VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?,?)";


    for(int tt = 0; tt < plist_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_plist_viewer_record obj = plist_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue(obj.bookmark_status);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.notes_text);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.plist_table_name);
        m_query.addBindValue(obj.tag_status);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_plist_viewer(QStringList INT_list, QString display_tab_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ? AND item5 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Plist_Viewer);
        m_query.addBindValue(display_tab_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_sqlite_viewer_db(QStringList INT_list, QString tab_label_name, QString table_name , QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString sqlite_lab_feature_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString();

    QString index_db_path  = sqlite_lab_feature_dir_path + "sqlite_viewer.sqlite";
    QString cmd = "Select savedstate_dest_dir_path from sv_index where viewer_display_name = ?";
    QString current_dir = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name) ,0 ,index_db_path ,Q_FUNC_INFO);
    QString dest_db_path = sqlite_lab_feature_dir_path + current_dir + "/" + "MainDataDisplay_SavedState.sqlite";
    QString command = QString("Update '" + table_name + "' Set RECON_Examiner_Notes ='") + notes_value +  "' Where ";
    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("RECON_INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, dest_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_sqlite_viewer_db(QStringList INT_list ,QString tab_label_name ,QString table_name , QString selected_sql_tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString sqlite_lab_feature_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString();
    QString index_db_path  = sqlite_lab_feature_dir_path + "sqlite_viewer.sqlite";
    QString cmd = "Select savedstate_dest_dir_path from sv_index where viewer_display_name = ?";
    QString current_dir = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name) ,0 ,index_db_path ,Q_FUNC_INFO);
    QString dest_db_path = sqlite_lab_feature_dir_path + current_dir + "/" + "MainDataDisplay_SavedState.sqlite";
    QString command = QString("Update '" + table_name + "' Set RECON_Examiner_Notes = '' Where ");

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("RECON_INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, dest_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_sqlite_viewer_db(QList<struct_global_tagging_sqlite_viewer> sqlite_viewer_record_list, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < sqlite_viewer_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_sqlite_viewer obj_1 = sqlite_viewer_record_list.at(tt);
        pub_delete_notes_for_sqlite_viewer_db(QStringList(obj_1.INT_record) ,obj_1.item11 , obj_1.item10 , obj_1.tab_name);
    }


    QSqlQuery m_query(destination_db);
    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4,"
                      "item5,item6,item7,item8,item9,item10,item11) VALUES (?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,? ,?,?,?,? ,?,?,?,?)";


    for(int tt = 0; tt < sqlite_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_sqlite_viewer obj = sqlite_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(notes_value);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.item5);
        m_query.addBindValue(obj.item6);
        m_query.addBindValue(obj.item7);
        m_query.addBindValue(obj.item8);
        m_query.addBindValue(obj.item9);
        m_query.addBindValue(obj.item10);
        m_query.addBindValue(obj.item11);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_sqlite_viewer_db(QStringList INT_list, QString tab_label_name, QString table_name, QString selected_sql_tab_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ? AND tab_name = ? AND item10 = ? AND item11 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Sqlite_Viewer);
        m_query.addBindValue(selected_sql_tab_name);
        m_query.addBindValue(table_name);
        m_query.addBindValue(tab_label_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_or_update_record_export_content_sqlite_viewer_db(QStringList INT_List, QString table_name,  QString file_path , QString tab_name_str, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString sqlite_content_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";
    QStringList arg_list;
    QString command;
    command = "select bookmark , recon_tag_value , notes from sv_content_info where file_path = ? AND record_no_in_sqliteviewer = ? AND db_table_name = ? AND sql_tab_name = ?";
    arg_list << file_path << INT_List << table_name << tab_name_str ;
    QString bookmark_value =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,sqlite_content_db,Q_FUNC_INFO);
    QString tag_name =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,1,sqlite_content_db,Q_FUNC_INFO);

    if(bookmark_value == QString("0") && tag_name.trimmed().isEmpty())
        command = QString("delete from sv_content_info WHERE ");
    else
        command = QString("update sv_content_info set notes = '" + notes_value +"' WHERE ");

    QString tmp_cmd;
    int pos = 0;
    int bunch_count = 50;
    for(int count = 0; count < INT_List.size(); count++)
    {
        QString record_no = INT_List.at(count);


        if(pos == 0)
            tmp_cmd = command;


        tmp_cmd.append("record_no_in_sqliteviewer = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");

        tmp_cmd.append("file_path = '");
        tmp_cmd.append(file_path);
        tmp_cmd.append("'");

        tmp_cmd.append(" AND ");
        tmp_cmd.append("db_table_name = '");
        tmp_cmd.append(table_name);
        tmp_cmd.append("'");

        tmp_cmd.append(" AND ");
        tmp_cmd.append("sql_tab_name = '");
        tmp_cmd.append(tab_name_str);
        tmp_cmd.append("'");

        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_List.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, sqlite_content_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_tags_search_for_plist_viewer(QStringList INT_list, QString display_tab_name, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "update tags set notes = ?  where record_no = ? AND plugin_name = ? AND item5 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(notes_value);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(MACRO_Plugin_Name_Plist_Viewer);
            m_query.addBindValue(display_tab_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_bookmark_search_for_plist_viewer(QStringList INT_list, QString display_tab_name, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "update tags set notes = ? where record_no = ? AND plugin_name = ? AND item5 = ?";
    m_query.prepare(command);
    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();


        m_query.addBindValue(notes_value);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Plist_Viewer);
        m_query.addBindValue(display_tab_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_tags_search_for_sqlite_viewer(QStringList INT_list, QString selected_sql_tab_name, QString table_name ,QString tab_label_name , QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "update tags set notes = '"+ notes_value +"'where record_no = ? AND plugin_name = ? AND tab_name = ? AND item2 = ? AND item3 = ?";

        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(MACRO_Plugin_Name_Sqlite_Viewer);
            m_query.addBindValue(selected_sql_tab_name);
            m_query.addBindValue(table_name);
            m_query.addBindValue(tab_label_name);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_bookmark_search_for_sqlite_viewer(QStringList INT_list, QString selected_sql_tab_name, QString table_name, QString tab_label_name, QString notes_value)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "update tags set notes = '"+ notes_value +"'where record_no = ? AND plugin_name = ? AND tab_name = ? AND item2 = ? AND item3 = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Sqlite_Viewer);
        m_query.addBindValue(selected_sql_tab_name);
        m_query.addBindValue(table_name);
        m_query.addBindValue(tab_label_name);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_bookmark_in_carved_data_db(QStringList INT_list,  QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString db_path_carved_data  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    QString command = QString("Update '" + table_name +"' Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_data, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_carved_data_db(QStringList INT_list, QString table_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString db_path_carved_data  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    QString command = QString("Update '"+ table_name +"' Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;



    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_data, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_update_bookmark_export_content_carved_data_db(QStringList INT_list, QString category_name, QStringList source_count_namelist , QString bookmark_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";
    command = QString("update tbl_carved_data Set bookmark = '"+ bookmark_value + "' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("category_name = '");
        tmp_cmd.append(category_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" AND ");
        tmp_cmd.append("source_count_name = '");
        tmp_cmd.append(source_count_namelist.at(count));
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_bookmark_for_carved_data_db(QList<struct_global_tagging_carved_data> carved_data_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {
        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);
        pub_delete_bookmark_for_carved_data_db(QStringList(obj.INT_record),obj.category_name , QStringList(obj.source_count_name));
    }


    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,"
                      "os_scheme_display,item0,item1,item2,item3 , item11 , item12) VALUES (?,?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        m_query.addBindValue(obj.item11);
        m_query.addBindValue(obj.item12);


        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_carved_data_db(QStringList INT_list, QString category_name , QStringList source_count_name_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from tags where record_no = ? AND plugin_name = ? AND category = ? AND source_count_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Carved_Data);
        m_query.addBindValue(category_name);
        m_query.addBindValue(source_count_name_list.at(tt));
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_entry_export_content_carved_data_db(QStringList INT_list , QString category_name , QStringList source_count_namelist,QStringList is_tagded_record)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString export_content_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(export_content_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString delete_command = "delete from tbl_carved_data where record_no = ? AND category_name = ? AND source_count_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        QString tagged_str = is_tagded_record.at(tt);
        if(tagged_str == MACRO_TRUE_VALUE_NUMERIC_QString)
        {
            continue ;
        }


        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(delete_command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(category_name);
        m_query.addBindValue(source_count_namelist.at(tt));
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_carved_data_db(QStringList INT_list, QString table_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '" + table_name +  "' Set notes ='") + notes_value +  "' Where ";
    QString db_path_carved_data  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";
    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_data, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_carved_data_db(QStringList INT_list, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '"+ table_name +"' Set notes = '' Where ");
    QString db_path_carved_data  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_data, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_update_notes_export_content_carved_data_db(QStringList INT_list, QString category_name, QStringList source_count_namelist , QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";
    command = QString("update tbl_carved_data Set notes = '"+ notes_value +"' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("category_name = '");
        tmp_cmd.append(category_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" AND ");
        tmp_cmd.append("source_count_name = '");
        tmp_cmd.append(source_count_namelist.at(count));
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_notes_for_carved_data_db(QList<struct_global_tagging_carved_data> carved_data_record_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();
        struct_global_tagging_carved_data obj_1 = carved_data_record_list.at(tt);
        pub_delete_notes_for_carved_data_db(QStringList(obj_1.INT_record) , obj_1.category_name , QStringList(obj_1.source_count_name));
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,recon_tag_value,notes,"
                      "os_scheme_display,item0,item1,item2,item3 , item11 , item12) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.notes_str);
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item11);
        m_query.addBindValue(obj.item12);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_carved_data_db(QStringList INT_list, QString category_name, QStringList source_count_name_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where record_no = ? AND plugin_name = ? AND category = ? AND source_count_name = ?";

    for(int tt = 0; tt < INT_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(INT_list.at(tt));
        m_query.addBindValue(MACRO_Plugin_Name_Carved_Data);
        m_query.addBindValue(category_name);
        m_query.addBindValue(source_count_name_list.at(tt));
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_carved_data_db(QStringList INT_list, QString table_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString db_path_carved_data  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    QString command = QString("Update '" +  table_name + "' Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_data, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_carved_data_db(QStringList INT_list, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update '" + table_name + "' Set recon_tag_value = '' Where ");
    QString db_path_carved_data  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";
    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_data, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_update_tag_export_content_carved_data_db(QStringList INT_list, QString category_name, QStringList source_count_namelist , QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString command;
    QString exported_content_info_db ;
    exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";
    command = QString("update tbl_carved_data Set recon_tag_value = '" + tag_name +"' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("record_no = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("category_name = '");
        tmp_cmd.append(category_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" AND ");
        tmp_cmd.append("source_count_name = '");
        tmp_cmd.append(source_count_namelist.at(count));
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");


        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_add_tag_for_carved_data(QList<struct_global_tagging_carved_data> carved_data_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {
        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);
        pub_delete_tag_for_carved_data(QStringList(obj.INT_record),obj.category_name,QStringList(obj.source_count_name));
    }



    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,"
                      "os_scheme_display,item0,item1,item2,item3 , item11 , item12) VALUES (?,?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        m_query.addBindValue(obj.item11);
        m_query.addBindValue(obj.item12);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }



    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_carved_data(QStringList INT_list, QString category_name, QStringList source_count_name_list)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ? AND category = ? AND source_count_name = ?";

        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(MACRO_Plugin_Name_Carved_Data);
            m_query.addBindValue(category_name);
            m_query.addBindValue(source_count_name_list.at(tt));

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_delete_notes_for_hex_viewer_db(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(notes_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "delete from notes where plugin_name = ? AND tab_name = ? AND source_count_name = ? AND item1 = ?"
                      "AND item2 = ? AND item3 = ?";

    for(int tt = 0; tt < list_st_global_tagging_db_updation.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_tagging_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_tags_search_for_hex_viewer(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "update tags set notes = '' where plugin_name = ? AND tab_name = ? AND source_count_name = ? "
                          "AND item1 = ? AND item2 = ? AND item3 = ?";

        for(int tt = 0; tt < list_st_global_tagging_db_updation.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();


            struct_global_tagging_tag_notes_db_updation obj = list_st_global_tagging_db_updation.at(tt);
            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.source_count_name);
            m_query.addBindValue(obj.item1);
            m_query.addBindValue(obj.item2);
            m_query.addBindValue(obj.item3);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_tags_for_hex_viewer(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation , QString tag_name)
{
    recon_static_functions::app_debug("start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    pub_delete_tags_for_hex_viewer(list_st_global_tagging_db_updation,tag_name);

    QSqlQuery m_query(destination_db);
    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                      "os_scheme_display,source_count_name, item0, item1, item2, item3, item4,csv_plugin_name,"
                      "csv_tab_name) VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?, ?,?)";

    for(int tt = 0; tt < list_st_global_tagging_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_tagging_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.recon_tag_value);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(obj.csv_plugin_name);
        m_query.addBindValue(obj.csv_tab_name);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    emit signal_common_add_tag_name_into_case_tree(tag_name);

    recon_static_functions::app_debug("end ", Q_FUNC_INFO);
}

void global_connection_manager::pub_delete_tags_for_hex_viewer(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    //    QString command = "select tag_db_name, tag_name from tag_search_index";

    //QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    //QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);

    //    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    //    {
    //QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

    QString tag_db_name  = tag_name.trimmed();
    tag_db_name = tag_db_name.replace(" ", "_") + ".sqlite";;
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name;

    QFileInfo info(dest_db_path);

    if(info.exists() == false)
        return;


    QCoreApplication::processEvents();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);


    QString command = "delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND "
                      "source_count_name = ? AND item2 = ? AND item3 = ?";



    for(int tt = 0; tt < list_st_global_tagging_db_updation.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_tagging_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.source_count_name);

        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" ERROR " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }


    }
    destination_db.commit();
    destination_db.close();


    //    emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    emit signal_common_add_tag_name_into_case_tree(tag_name);
    //}

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_data_in_export_content_carved_data_db(QList<struct_global_tagging_carved_data> carved_data_record_list, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command_create = QString("create table IF NOT EXISTS 'tbl_carved_data'"
                                     " (bookmark INTEGER,INT INTEGER PRIMARY KEY,record_no varchar(100),plugin_name varchar(100), tab_name varchar(100) , category_name varchar(100),number_of_hits varchar(100),carved_keyword varchar(500),"
                                     "source_plugin_name varchar(100),source_tab_name varchar(100),source_record_no varchar(100),source_file_name varchar(100),source_file_path varchar(500),"
                                     "root_count_name varchar(100), source_count_name varchar(100),"
                                     "notes_icon varchar(1000),recon_tag_value varchar(1000),notes varchar(1000),recon_filefrom varchar(10),recon_file_infopath varchar(500),blank_column varchar(10))");



    QString export_content_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command_create,export_content_db_path ,Q_FUNC_INFO);


    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {
        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);
        pub_delete_entry_export_content_carved_data_db(QStringList(obj.INT_record),obj.table_name , QStringList(obj.source_count_name) , QStringList(obj.tagging_on_record));
    }

    QString exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";
    QString command = QString("INSERT INTO tbl_carved_data(record_no ,bookmark,plugin_name , tab_name , category_name, number_of_hits,carved_keyword,"
                              "source_plugin_name,source_tab_name,source_record_no,source_file_name,source_file_path,"
                              "root_count_name,source_count_name , recon_filefrom , recon_file_infopath , recon_tag_value , notes)"
                              " VALUES(?,?,?,?,?,? ,?,?,?,?,? ,?,?,?,?,? ,?,?)");

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);
    for(int tt = 0; tt < carved_data_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_carved_data obj = carved_data_record_list.at(tt);

        if(obj.tagging_on_record == MACRO_TRUE_VALUE_NUMERIC_QString)
        {
            continue ;
        }

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue(obj.bookmark_status);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.category_name);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.source_plugin_name);
        m_query.addBindValue(obj.source_tab_name);
        m_query.addBindValue(obj.source_record_no);
        m_query.addBindValue(obj.source_file_name);
        m_query.addBindValue(obj.source_file_path);
        m_query.addBindValue(obj.root_count_name);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.recon_file_from);
        m_query.addBindValue(obj.recon_file_infopath);
        m_query.addBindValue(tag_name);
        m_query.addBindValue(obj.notes_str);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();
    destination_db.close();

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString artifact_keyword_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString index_db_path = artifact_keyword_search_dir + "artifacts_keyword_search.sqlite";
    QString command_1 = "select table_name from keyword_search_index";
    QStringList db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,index_db_path,Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update keyword_search Set notes ='' Where ");
    int pos = 0;


    for(int db_count = 0; db_count < db_name_list.size(); db_count++)
    {
        QString db_name = db_name_list.at(db_count);
        QString artifact_keyword_search_db_path = artifact_keyword_search_dir + db_name.trimmed() +".sqlite";

        QString tmp_cmd;
        for(int count = 0; count < INT_list.size(); count++)
        {
            QString record_no = INT_list.at(count);


            if(pos == 0)
                tmp_cmd = command;

            tmp_cmd.append("record_no = ");
            tmp_cmd.append(record_no);
            tmp_cmd.append(" AND ");
            tmp_cmd.append("plugin = '");
            tmp_cmd.append(plugin_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" AND ");
            tmp_cmd.append("category = '");
            tmp_cmd.append(tab_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" OR ");

            pos++;

            if(pos == bunch_count || count >= INT_list.size() - 1)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());


                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_keyword_search_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();
                pos = 0;

                QCoreApplication::processEvents();
            }

        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_remove_tags_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString artifact_keyword_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString index_db_path = artifact_keyword_search_dir + "artifacts_keyword_search.sqlite";
    QString command_1 = "select table_name from keyword_search_index";
    QStringList db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,index_db_path,Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update keyword_search Set recon_tag_value ='' Where ");
    int pos = 0;


    for(int db_count = 0; db_count < db_name_list.size(); db_count++)
    {
        QString db_name = db_name_list.at(db_count);
        QString artifact_keyword_search_db_path = artifact_keyword_search_dir + db_name.trimmed() +".sqlite";

        QString tmp_cmd;
        for(int count = 0; count < INT_list.size(); count++)
        {
            QString record_no = INT_list.at(count);


            if(pos == 0)
                tmp_cmd = command;

            tmp_cmd.append("record_no = ");
            tmp_cmd.append(record_no);
            tmp_cmd.append(" AND ");
            tmp_cmd.append("plugin = '");
            tmp_cmd.append(plugin_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" AND ");
            tmp_cmd.append("category = '");
            tmp_cmd.append(tab_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" OR ");

            pos++;

            if(pos == bunch_count || count >= INT_list.size() - 1)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());


                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_keyword_search_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();
                pos = 0;

                QCoreApplication::processEvents();
            }

        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_remove_bookmarks_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString artifact_keyword_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString index_db_path = artifact_keyword_search_dir + "artifacts_keyword_search.sqlite";
    QString command_1 = "select table_name from keyword_search_index";
    QStringList db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,index_db_path,Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update keyword_search Set bookmark ='0' Where ");
    int pos = 0;


    for(int db_count = 0; db_count < db_name_list.size(); db_count++)
    {
        QString db_name = db_name_list.at(db_count);
        QString artifact_keyword_search_db_path = artifact_keyword_search_dir + db_name.trimmed() +".sqlite";

        QString tmp_cmd;
        for(int count = 0; count < INT_list.size(); count++)
        {
            QString record_no = INT_list.at(count);


            if(pos == 0)
                tmp_cmd = command;

            tmp_cmd.append("record_no = ");
            tmp_cmd.append(record_no);
            tmp_cmd.append(" AND ");
            tmp_cmd.append("plugin = '");
            tmp_cmd.append(plugin_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" AND ");
            tmp_cmd.append("category = '");
            tmp_cmd.append(tab_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" OR ");

            pos++;

            if(pos == bunch_count || count >= INT_list.size() - 1)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());


                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_keyword_search_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();
                pos = 0;

                QCoreApplication::processEvents();
            }

        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_notes_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name , QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString artifact_keyword_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString index_db_path = artifact_keyword_search_dir + "artifacts_keyword_search.sqlite";
    QString command_1 = "select table_name from keyword_search_index";
    QStringList db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,index_db_path,Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update keyword_search Set notes ='" + notes_value +"' Where ");
    int pos = 0;


    for(int db_count = 0; db_count < db_name_list.size(); db_count++)
    {
        QString db_name = db_name_list.at(db_count);
        QString artifact_keyword_search_db_path = artifact_keyword_search_dir + db_name.trimmed() +".sqlite";

        QString tmp_cmd;
        for(int count = 0; count < INT_list.size(); count++)
        {
            QString record_no = INT_list.at(count);


            if(pos == 0)
                tmp_cmd = command;

            tmp_cmd.append("record_no = ");
            tmp_cmd.append(record_no);
            tmp_cmd.append(" AND ");
            tmp_cmd.append("plugin = '");
            tmp_cmd.append(plugin_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" AND ");
            tmp_cmd.append("category = '");
            tmp_cmd.append(tab_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" OR ");

            pos++;

            if(pos == bunch_count || count >= INT_list.size() - 1)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());


                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_keyword_search_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();
                pos = 0;

                QCoreApplication::processEvents();
            }

        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_tags_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString artifact_keyword_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString index_db_path = artifact_keyword_search_dir + "artifacts_keyword_search.sqlite";
    QString command_1 = "select table_name from keyword_search_index";
    QStringList db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,index_db_path,Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update keyword_search Set recon_tag_value ='" + tag_name +"' Where ");
    int pos = 0;


    for(int db_count = 0; db_count < db_name_list.size(); db_count++)
    {
        QString db_name = db_name_list.at(db_count);
        QString artifact_keyword_search_db_path = artifact_keyword_search_dir + db_name.trimmed() +".sqlite";

        QString tmp_cmd;
        for(int count = 0; count < INT_list.size(); count++)
        {
            QString record_no = INT_list.at(count);


            if(pos == 0)
                tmp_cmd = command;

            tmp_cmd.append("record_no = ");
            tmp_cmd.append(record_no);
            tmp_cmd.append(" AND ");
            tmp_cmd.append("plugin = '");
            tmp_cmd.append(plugin_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" AND ");
            tmp_cmd.append("category = '");
            tmp_cmd.append(tab_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" OR ");

            pos++;

            if(pos == bunch_count || count >= INT_list.size() - 1)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());


                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_keyword_search_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();
                pos = 0;

                QCoreApplication::processEvents();
            }

        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_bookmarks_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    QString artifact_keyword_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString index_db_path = artifact_keyword_search_dir + "artifacts_keyword_search.sqlite";
    QString command_1 = "select table_name from keyword_search_index";
    QStringList db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,index_db_path,Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update keyword_search Set bookmark ='1' Where ");
    int pos = 0;


    for(int db_count = 0; db_count < db_name_list.size(); db_count++)
    {
        QString db_name = db_name_list.at(db_count);
        QString artifact_keyword_search_db_path = artifact_keyword_search_dir + db_name.trimmed() +".sqlite";

        QString tmp_cmd;
        for(int count = 0; count < INT_list.size(); count++)
        {
            QString record_no = INT_list.at(count);


            if(pos == 0)
                tmp_cmd = command;

            tmp_cmd.append("record_no = ");
            tmp_cmd.append(record_no);
            tmp_cmd.append(" AND ");
            tmp_cmd.append("plugin = '");
            tmp_cmd.append(plugin_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" AND ");
            tmp_cmd.append("category = '");
            tmp_cmd.append(tab_name);
            tmp_cmd.append("'");
            tmp_cmd.append(" OR ");

            pos++;

            if(pos == bunch_count || count >= INT_list.size() - 1)
            {
                if(tmp_cmd.endsWith(" OR "))
                    tmp_cmd.chop(QString(" OR ").size());


                recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, artifact_keyword_search_db_path, Q_FUNC_INFO);
                tmp_cmd.clear();
                pos = 0;

                QCoreApplication::processEvents();
            }

        }
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_bookmark_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name , QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '" + table_name + "' Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_snapshot_diff = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshots_label_name +".sqlite";


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_snapshot_diff, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name , QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '" + table_name + "' Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;

    QString db_path_snapshot_diff = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshots_label_name +".sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(count % 50 == 0)
            QCoreApplication::processEvents();


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_snapshot_diff, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_bookmark_for_snapshots_difference_system(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    pub_delete_bookmark_for_snapshots_difference(list_st_global_bookmark_db_updation);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.csv_plugin_name);
        m_query.addBindValue(obj.csv_tab_name);
        m_query.addBindValue(obj.recon_tag_value);
        m_query.addBindValue(obj.notes);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_bookmark_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ? AND item3 = ?" ;

    for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.item3);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}


void global_connection_manager::pub_set_tag_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name, QString table_name , QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '"+ table_name +"' Set recon_tag_value ='") + tag_name +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_snapshot_diff = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshots_label_name +".sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_snapshot_diff, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_tag_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '"+ table_name +"' Set recon_tag_value ='") + "" +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_snapshot_diff = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshots_label_name +".sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_snapshot_diff, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_tag_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tag_db_updation, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);


    pub_delete_tag_for_snapshots_difference(list_st_global_tag_db_updation);


    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) "
                      "VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


    for(int tt = 0; tt < list_st_global_tag_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_tag_db_updation.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.bookmark);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.csv_plugin_name);
        m_query.addBindValue(obj.csv_tab_name);
        m_query.addBindValue(tag_name);
        m_query.addBindValue(obj.notes);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tag_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);



    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ? AND item3 = ?" ;

        for(int tt = 0; tt < list_st_global_tag_db_updation.size(); tt++)
        {
            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            struct_global_tagging_tag_notes_db_updation obj = list_st_global_tag_db_updation.at(tt);
            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(obj.record_no);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.source_count_name);
            m_query.addBindValue(obj.item3);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }

        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void global_connection_manager::pub_set_notes_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name, QString table_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '"+ table_name +"' Set notes ='") + notes_value +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_snapshot_diff = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshots_label_name +".sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_snapshot_diff, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name, QString table_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;
    QString command = QString("Update '"+ table_name +"' Set notes ='") + "" +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    QString db_path_snapshot_diff = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshots_label_name +".sqlite";

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_snapshot_diff, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_add_notes_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_notes_db_updation , QString notes_text)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    pub_delete_notes_for_snapshots_difference(list_st_global_notes_db_updation);

    QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,notes,os_scheme_display,item0,item1,item2,item3) "
                      "VALUES (?,?,?,?, ?,?,?,?, ?,?,?,?, ?)";


    for(int tt = 0; tt < list_st_global_notes_db_updation.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_notes_db_updation.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.category);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.table_name);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.bookmark);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(notes_text);
        m_query.addBindValue(obj.os_scheme_display);
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_notes_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_notes_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QCoreApplication::processEvents();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    QSqlQuery m_query(destination_db);

    QString command = "Delete from notes where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ? AND item3 = ?" ;

    for(int tt = 0; tt < list_st_global_notes_db_updation.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_tag_notes_db_updation obj = list_st_global_notes_db_updation.at(tt);
        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.record_no);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.item3);
        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.commit();
    destination_db.close();

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void global_connection_manager::pub_set_bookmark_in_carved_index_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;

    QString content_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    QString index_db_path  = content_search_dir_path + "index_content_search.sqlite";
    QString cmd = "Select search_db_file_name from table_index_content_search where search_label_name = ?";
    QString db_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);

    if(!db_name_str.isEmpty() && !db_name_str.endsWith(".sqlite"))
        db_name_str.append(".sqlite");

    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(content_search_dir_path, db_name_str , Q_FUNC_INFO);

    QString command = QString("Update files Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_bookmark_in_carved_index_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;

    QString content_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    QString index_db_path  = content_search_dir_path + "index_content_search.sqlite";
    QString cmd = "Select search_db_file_name from table_index_content_search where search_label_name = ?";
    QString db_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);

    if(!db_name_str.isEmpty() && !db_name_str.endsWith(".sqlite"))
        db_name_str.append(".sqlite");

    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(content_search_dir_path, db_name_str , Q_FUNC_INFO);

    QString command = QString("Update files Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_notes_in_carved_index_files_db(QStringList INT_list, QString tab_label_name, QString notes_value)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString content_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    QString index_db_path  = content_search_dir_path + "index_content_search.sqlite";
    QString cmd = "Select search_db_file_name from table_index_content_search where search_label_name = ?";
    QString db_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);

    if(!db_name_str.isEmpty() && !db_name_str.endsWith(".sqlite"))
        db_name_str.append(".sqlite");

    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(content_search_dir_path, db_name_str , Q_FUNC_INFO);

    QString command = QString("Update files Set notes ='") + notes_value +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_notes_in_carved_index_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update files Set notes = '' Where ");

    QString content_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    QString index_db_path  = content_search_dir_path + "index_content_search.sqlite";
    QString cmd = "Select search_db_file_name from table_index_content_search where search_label_name = ?";
    QString db_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);

    if(!db_name_str.isEmpty() && !db_name_str.endsWith(".sqlite"))
        db_name_str.append(".sqlite");

    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(content_search_dir_path, db_name_str , Q_FUNC_INFO);

    QString tmp_cmd;
    int pos = 0;


    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}



void global_connection_manager::pub_set_tags_in_carved_index_files_db(QStringList INT_list, QString tab_label_name, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString content_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    QString index_db_path  = content_search_dir_path + "index_content_search.sqlite";
    QString cmd = "Select search_db_file_name from table_index_content_search where search_label_name = ?";
    QString db_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);

    if(!db_name_str.isEmpty() && !db_name_str.endsWith(".sqlite"))
        db_name_str.append(".sqlite");

    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(content_search_dir_path, db_name_str , Q_FUNC_INFO);

    QString command = QString("Update files Set recon_tag_value ='") + tag_name +  "' Where ";


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_remove_tag_in_carved_index_files_db(QStringList INT_list, QString tab_label_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;

    QString content_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    QString index_db_path  = content_search_dir_path + "index_content_search.sqlite";
    QString cmd = "Select search_db_file_name from table_index_content_search where search_label_name = ?";
    QString db_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd , QStringList(tab_label_name),0,index_db_path , Q_FUNC_INFO);

    if(!db_name_str.isEmpty() && !db_name_str.endsWith(".sqlite"))
        db_name_str.append(".sqlite");

    QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(content_search_dir_path, db_name_str , Q_FUNC_INFO);

    QString command = QString("Update files Set recon_tag_value ='' Where ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, db_path_carved_files, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_set_files_list_having_same_hash(QString source_name, QString source, QString file_path, QString record_number, QString fs_db_path, QString os_scheme)
{
    emit signal_go_to_source_for_search_file_with_same_hash(source_name, source, file_path, record_number, fs_db_path, os_scheme);
}

void global_connection_manager::pub_add_tag_for_emlx_viewer_raw_data(QList<struct_global_tagging_string_viewer_record> string_viewer_record_list, QString tag_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery m_query(destination_db);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4,item5) VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?)";


    for(int tt = 0; tt < string_viewer_record_list.size(); tt++)
    {

        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_string_viewer_record obj = string_viewer_record_list.at(tt);

        m_query.clear();
        m_query.prepare(command);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue(obj.INT_record);
        m_query.addBindValue("1");
        m_query.addBindValue(obj.source_count_name);
        m_query.addBindValue(obj.plugin_name);
        m_query.addBindValue(obj.tab_name);
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue("");
        m_query.addBindValue(obj.item0);
        m_query.addBindValue(obj.item1);
        m_query.addBindValue(obj.item2);
        m_query.addBindValue(obj.item3);
        m_query.addBindValue(obj.item4);
        m_query.addBindValue(tag_name);

        if(!m_query.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_delete_tag_export_content_emlx_raw_tag_db(QStringList INT_list, QString content_file_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);
    int bunch_count = 50;


    QString exported_content_info_db =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString() + "email_tag_data.sqlite";

    QString command = QString("Delete from emlx_content_info Where ");


    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);


        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" AND ");
        tmp_cmd.append("content_file_name = '");
        tmp_cmd.append(content_file_name);
        tmp_cmd.append("'");
        tmp_cmd.append(" OR ");



        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, exported_content_info_db, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);


}

void global_connection_manager::pub_delete_tag_for_emlx_raw_tag(QStringList INT_list, QString content_file_name, QString plugin_name)
{

    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);


    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "delete from tags where record_no = ? AND plugin_name = ? AND item3 = ?";
        for(int tt = 0; tt < INT_list.size(); tt++)
        {

            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(INT_list.at(tt));
            m_query.addBindValue(plugin_name);
            m_query.addBindValue(content_file_name);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }


        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::pub_add_data_in_tags_db_for_unified_logs(QList<struct_global_tagging_unified_logs_record> log_viewer_record_qlist, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    delete_file_entry_from_all_tag_DBs_for_unified_logs(log_viewer_record_qlist);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_query_tags(destination_db);

    QString command = " INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      " source_count_name, csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,"
                      " item2,item3, item4, item5, item6)"
                      " VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?,?)";

    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_unified_logs_record obj = log_viewer_record_qlist.at(tt);

        insert_query_tags.clear();
        insert_query_tags.prepare(command);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue(obj.plugin_name);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue(obj.INT_record);
        insert_query_tags.addBindValue("1");
        insert_query_tags.addBindValue(obj.source_count_name);
        insert_query_tags.addBindValue(obj.plugin_name);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue("");

        insert_query_tags.addBindValue(obj.category);
        insert_query_tags.addBindValue(obj.timestamp);
        insert_query_tags.addBindValue(obj.process);
        insert_query_tags.addBindValue(obj.event_type);
        insert_query_tags.addBindValue(obj.event_message);

        insert_query_tags.addBindValue(obj.viewer_display_name);
        insert_query_tags.addBindValue(obj.log_table_name);

        if(!insert_query_tags.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + insert_query_tags.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + insert_query_tags.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_set_tag_in_unified_logs_db(QStringList INT_list, QString tag_name,QString destination_db_path)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;
    QString command = QString("Update log_data Set recon_tag_value ='") + tag_name +  "' Where ";

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, destination_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_remove_data_in_tags_db_for_unified_logs(QList<struct_global_tagging_unified_logs_record> log_viewer_record_qlist, QString tag_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tag_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_query_tags(destination_db);

    QString command = " INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                      " source_count_name, csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3, item4, item5, item6)"
                      " VALUES (?,?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?,?,?)";

    for(int tt = 0; tt < log_viewer_record_qlist.size(); tt++)
    {
        if(tt % 50 == 0)
            QCoreApplication::processEvents();

        struct_global_tagging_unified_logs_record obj = log_viewer_record_qlist.at(tt);

        insert_query_tags.clear();
        insert_query_tags.prepare(command);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue(obj.plugin_name);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue(obj.INT_record);
        insert_query_tags.addBindValue("1");
        insert_query_tags.addBindValue(obj.source_count_name);
        insert_query_tags.addBindValue(obj.plugin_name);
        insert_query_tags.addBindValue(obj.tab_name);
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue("");
        insert_query_tags.addBindValue("");

        insert_query_tags.addBindValue(obj.category);
        insert_query_tags.addBindValue(obj.timestamp);
        insert_query_tags.addBindValue(obj.process);
        insert_query_tags.addBindValue(obj.event_type);
        insert_query_tags.addBindValue(obj.event_message);

        insert_query_tags.addBindValue(obj.viewer_display_name);
        insert_query_tags.addBindValue(obj.log_table_name);

        if(!insert_query_tags.exec())
        {
            recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + insert_query_tags.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" EXecuted query " + insert_query_tags.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }


    destination_db.close();

    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

}

void global_connection_manager::pub_update_tag_unified_logs_db(QStringList INT_list,QString source_count_name)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    int bunch_count = 50;

    QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    QString unified_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";
    QString destination_db_path;
    QString command = QString("SELECT db_name from tbl_index where source_count_name = ?");
    QStringList values;
    values << source_count_name;
    QString unified_logs_db = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, values,0, unified_logs_db_path, Q_FUNC_INFO);

    destination_db_path = unified_logs_dir_path + unified_logs_db;

    command.clear();
    command = QString("update log_data Set recon_tag_value = '' WHERE ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = 0; count < INT_list.size(); count++)
    {
        QString record_no = INT_list.at(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no);
        tmp_cmd.append(" OR ");

        pos++;

        if(pos == bunch_count || count >= INT_list.size() - 1)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());


            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, destination_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();
            pos = 0;

            QCoreApplication::processEvents();
        }

    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void global_connection_manager::delete_file_entry_from_all_tag_DBs_for_unified_logs(QList<struct_global_tagging_unified_logs_record> list_st_global_bookmark_db_updation)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString command = "select tag_db_name, tag_name from tag_search_index";

    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_index_db, Q_FUNC_INFO);
    QStringList tag_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_index_db, Q_FUNC_INFO);
    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";

        QFileInfo info(dest_db_path);

        if(info.exists() == false)
            continue;


        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(dest_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        destination_db.transaction();
        QSqlQuery m_query(destination_db);

        QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ?" ;

        for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
        {
            if(tt % 50 == 0)
                QCoreApplication::processEvents();

            struct_global_tagging_unified_logs_record obj = list_st_global_bookmark_db_updation.at(tt);
            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(obj.INT_record);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.source_count_name);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }

        destination_db.commit();
        destination_db.close();

        emit signal_common_add_tag_name_into_case_tree(tag_name_list.at(db_count));
    }
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}
