#include "sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::load_sqlite_and_prepare_data(QString file_path)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    ///- 1 clear widgwt on reload
    ui->tabWidget_FilterViewSearch->clear();
    ui->tabWidget_main_data_panel->clear();

    list_struct_tablewidget_data_sqlite_viewer.clear();

    ui->textEdit_query_insert->clear();
    ui->tableWidget_query_execution->clear();

    bool_combobox_clear = false;
    ui->comboBox_dbtables_selection->clear();
    bool_combobox_clear = true;

    source_file = file_path;

    ///-2 Copy and recreate sourece file instance at destination to add two coloums in it.

    set_values_obtain_from_sqlite_viewer_index_db(recieved_sqlite_path_from_mainwindow,file_path);



    if(is_loadstate == 1)
    {
        load_savedstate_of_sqlite();
        return;
    }

    ///-3. Create tables and structure list from db.
    QString copiedSourceFile = copied_source_file;
    if(!copiedSourceFile.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
        copiedSourceFile.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());


    QString recreateSourceFile = saved_execute_query_maindb_path;
    if(!recreateSourceFile.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
        recreateSourceFile.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

    list_struct_tablewidget_data_sqlite_viewer = base_core_class_obj->prepare_and_fill_structure_for_main_table_display(source_file,copiedSourceFile,recreateSourceFile);

    ///-4 Display tables name in QCombobox
    QStringList listTableName = base_core_class_obj->pub_get_db_tablename_list();

    ui->comboBox_dbtables_selection->addItems(listTableName);
    set_widgets_disable(false);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::connect_table_filter_view_search_for_cell_clicked(int index)
{
    struct_global_sqlite_viewer_tablewidget_main obj_struct;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > index)
        obj_struct = list_struct_tablewidget_data_sqlite_viewer.at(index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    QTableWidget *tableWidget = obj_struct.tableWidget_FilterViewSearch_ptr;

    //-Disconnect
    disconnect(tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
    disconnect(tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

    //- Connect
    connect(tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
    connect(tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

}

int sqlite_viewer::check_savedstate_and_update_filterview_search()
{
    QString sv_index_db_path = recieved_sqlite_path_from_mainwindow;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(sv_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return 0;
    }

    QSqlQuery query(destination_db);

    QString command = "SELECT viewer_display_name, is_savestate, savedstate_dest_dir_path, recreated_file_path"
                      " FROM sv_index WHERE viewer_display_name = ?";
    query.prepare(command);
    query.addBindValue(sqlite_viewer_display_name_in_tree_and_tab);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query  --------FAILED------ db file - " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return 0;
    }

    QString plugin_record_db_name;
    QString savestate;
    QString savedstate_dbs_path;

    while(query.next())
    {
        plugin_record_db_name = query.value(0).toString();
        savestate = query.value(1).toString();
        savedstate_dbs_path = query.value(2).toString();

        if(!savedstate_dbs_path.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            savedstate_dbs_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        saved_execute_query_maindb_path = query.value(3).toString();
        if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());
    }

    if(savestate == QString::number(1))
    {
        recon_static_functions::app_debug(QString("Updating targeted FilterView db path is ") + savedstate_dbs_path + QString("/FilterViewTable_SavedState.sqlite"),Q_FUNC_INFO);
        filter_view_table_saved_state(savedstate_dbs_path + QString("/FilterViewTable_SavedState.sqlite"));

        destination_db.close();
        return 1;
    }

    destination_db.close();
    return 0;
}

int sqlite_viewer::check_savedstate_and_update_main_data_display()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString sv_index_db_path = recieved_sqlite_path_from_mainwindow;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(sv_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return 0;
    }

    QSqlQuery query(destination_db);

    QString command = "SELECT viewer_display_name, is_savestate, savedstate_dest_dir_path, recreated_file_path FROM"
                      " sv_index WHERE viewer_display_name = ?";
    query.prepare(command);
    query.addBindValue(sqlite_viewer_display_name_in_tree_and_tab);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query  --------FAILED------ db file - " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return 0;
    }

    QString plugin_record_db_name;
    QString savestate;
    QString savedstate_dbs_path;

    while(query.next())
    {
        plugin_record_db_name = query.value(0).toString();
        savestate = query.value(1).toString();
        savedstate_dbs_path = query.value(2).toString();

        saved_execute_query_maindb_path = query.value(3).toString();
    }

    if(savestate == QString::number(1))
    {
        recon_static_functions::app_debug(QString("Updating targeted MainDataDisplay db path is ") + savedstate_dbs_path + QString("/MainDataDisplay_SavedState.sqlite"),Q_FUNC_INFO);
        update_dataIn_DB_main_display_data_table(savedstate_dbs_path + QString("/MainDataDisplay_SavedState.sqlite"));

        destination_db.close();
        return 1;
    }

    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return 0;
}

int sqlite_viewer::check_savedstate_and_update_main_data_display_single_record()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString sv_index_db_path = recieved_sqlite_path_from_mainwindow;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(sv_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return 0;
    }

    QSqlQuery query(destination_db);

    QString command = "SELECT viewer_display_name, is_savestate, savedstate_dest_dir_path, recreated_file_path FROM sv_index WHERE "
                      "viewer_display_name = ?";
    query.prepare(command);
    query.addBindValue(sqlite_viewer_display_name_in_tree_and_tab);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query  --------FAILED------ db file - " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return 0;
    }

    QString plugin_record_db_name;
    QString savestate;
    QString savedstate_dbs_path;

    while(query.next())
    {
        plugin_record_db_name = query.value(0).toString();
        savestate = query.value(1).toString();
        savedstate_dbs_path = query.value(2).toString();

        if(!savedstate_dbs_path.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            savedstate_dbs_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        saved_execute_query_maindb_path = query.value(3).toString();
        if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());
    }

    if(savestate == QString::number(1))
    {
        recon_static_functions::app_debug(QString("Updating targeted MainDataDisplay db path is ") + savedstate_dbs_path + QString("/MainDataDisplay_SavedState.sqlite"),Q_FUNC_INFO);
        update_dataIn_DB_main_display_data_table_single_record(savedstate_dbs_path + QString("/MainDataDisplay_SavedState.sqlite"));

        destination_db.close();
        return 1;
    }

    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return 0;
}

int sqlite_viewer::notes_check_savedstate_and_update_main_data_display(int input_type)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString sv_index_db_path = recieved_sqlite_path_from_mainwindow;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(sv_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return 0;
    }

    QSqlQuery query(destination_db);

    QString command = "SELECT viewer_display_name, is_savestate, savedstate_dest_dir_path, recreated_file_path FROM sv_index WHERE "
                      "viewer_display_name = ?";
    query.prepare(command);
    query.addBindValue(sqlite_viewer_display_name_in_tree_and_tab);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query  --------FAILED------ db file - " + sv_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return 0;
    }

    QString plugin_record_db_name;
    QString savestate;
    QString savedstate_dbs_path;

    while(query.next())
    {
        plugin_record_db_name = query.value(0).toString();
        savestate = query.value(1).toString();
        savedstate_dbs_path = query.value(2).toString();
        if(!savedstate_dbs_path.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            savedstate_dbs_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        saved_execute_query_maindb_path = query.value(3).toString();
        if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

    }

    if(savestate == QString::number(1))
    {
        recon_static_functions::app_debug(QString("Updating targeted MainDataDisplay db path is ") + savedstate_dbs_path + QString("/MainDataDisplay_SavedState.sqlite"),Q_FUNC_INFO);
        notes_update_data_In_DB_main_display_data_table(savedstate_dbs_path + QString("/MainDataDisplay_SavedState.sqlite"),input_type);
        destination_db.close();
        return 1;
    }

    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return 0;
}

void sqlite_viewer::active_spacebar_bookmark_and_metadat_show(struct_global_sqlite_viewer_tablewidget_main obj_main)
{
    m_tablewidget *table_obj = obj_main.tableWidget_MainDataDisplay_ptr;

    //-dis connect if connected.
    disconnect(table_obj,SIGNAL(signal_arrow_key_pressed()),this,SLOT(slot_arrow_key_pressed_on_table()));
    disconnect(table_obj,SIGNAL(signal_bookmarked_key_pressed()),this,SLOT(slot_bookmark_key_pressed_tablewidget()));

    //-connect
    connect(table_obj,SIGNAL(signal_arrow_key_pressed()),this,SLOT(slot_arrow_key_pressed_on_table()));
    connect(table_obj,SIGNAL(signal_bookmarked_key_pressed()),this,SLOT(slot_bookmark_key_pressed_tablewidget()));

    obj_main.tableWidget_MainDataDisplay_ptr->installEventFilter(this);
}

void sqlite_viewer::action_submenu_tag_triggered_for_sqlite_viewer(QString tag_name)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);

    emit signal_add_tags_sqlite_viewer(tag_name,colour);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void sqlite_viewer::add_tags_to_sqlite_viewer(QString tag_name , QString colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;

    int column_count = tableWidget->columnCount();

    QModelIndexList selection_model_list = tableWidget->selectionModel()->selectedRows();
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= tableWidget->rowCount())
            continue;

        if(tableWidget->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = tableWidget->item(selected_row_no,2)->text();

        QStringList text_data = get_data_for_current_record(tableWidget);

        //----- Update Exported Content.

        QString status;
        if(tableWidget->item(selected_row_no,0)->checkState() == Qt::Checked)
            status = "1";
        else if(tableWidget->item(selected_row_no,0)->checkState() == Qt::Unchecked)
            status = "0";

        QString sql_tab_name = QString(QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + current_tab_name );

        current_sql_tab = sql_tab_name;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_name, Q_FUNC_INFO);

        QIcon icon(icon_path);
        tableWidget->item(selected_row_no,enum_coloumn_Sqlite_viewer_Tag_icon)->setIcon(icon);
        tableWidget->item(selected_row_no,(column_count -3))->setText(tag_name);

        int column_count = tableWidget->columnCount();
        notes_text_on_record = tableWidget->item(selected_row_no,(column_count -2))->text();
        create_and_insert_exported_content_info_in_db(selected_row_no ,record_no_str ,tag_name, sqlite_viewer_display_name_in_tree_and_tab, text_data, sql_tab_name , status);

        QStringList arg_list;
        QString destination_db_path = saved_state_dir_path + "/" + main_datadisplay_table_sqlite_name();
        QString command = QString("UPDATE " + current_tab_name + " SET RECON_Examiner_tags = ? where RECON_INT = ?");
        arg_list << tag_name << record_no_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , arg_list,destination_db_path , Q_FUNC_INFO);



    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void sqlite_viewer::delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag , QString record_no , QString plugin_name ,QString sql_tab_name)
{
    QString current_tag_name = current_tag;
    QString tag_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QString command = "Select tag_db_name from tag_search_index";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command , 0 ,tag_index_db_path, Q_FUNC_INFO);

    for(int i = 0 ; i <  tag_db_name_list.size(); i++)
    {


        QString db_name = tag_db_name_list.at(i);
        if(db_name == current_tag_name)
            continue ;

        ///1.---delete record from previous tag db
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + db_name + ".sqlite";
        QStringList arg_list;
        command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ?";
        arg_list << record_no << plugin_name << sql_tab_name ;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);


        ///2.---refresh case tree tag
        emit signal_common_add_tag_name_into_case_tree(tag_db_name_list.at(i));
    }

}

void sqlite_viewer::add_tags_to_executed_query(QString tag_name , QString colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QTableWidget *m_tableWidget_obj = (QTableWidget*)ui->tableWidget_query_execution;

    QModelIndexList selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= m_tableWidget_obj->rowCount())
            continue;

        if(m_tableWidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = QString::number(selected_row_no+1);

        QStringList text_data = get_data_for_current_record(m_tableWidget_obj);


        //----- Update Exported Content.

        QString sql_tab_name = QString(QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB) + "-" + current_tab_name );
        //        QString status;
        //        if(m_tableWidget_obj->item(selected_row_no,0)->checkState() == Qt::Checked)
        //            status = "1";
        //        else if(m_tableWidget_obj->item(selected_row_no,0)->checkState() == Qt::Unchecked)
        //            status = "0";

        //        int column_count = m_tableWidget_obj->columnCount();
        //        notes_text_on_record = m_tableWidget_obj->item(selected_row_no,(column_count -2))->text();

        current_sql_tab = sql_tab_name;

        ///Note::-on execute query notes and bookamrk are not enabled
        QString status = "0";
        tag_name_for_execute_tab = tag_name;

        create_and_insert_exported_content_info_in_db(selected_row_no ,record_no_str ,tag_name, sqlite_viewer_display_name_in_tree_and_tab, text_data, sql_tab_name , status);


        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_name, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tableWidget_obj->item(selected_row_no,enum_coloumn_sqlite_viewer_executed_query_Tag_icon)->setIcon(icon);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void sqlite_viewer::sqlite_viewer_export_db_sanity_check(QString file_path, int REPORT_CONTENT_FLAG)
{
    recon_static_functions::app_debug("starts] -START ",Q_FUNC_INFO);

    export_db_destination_path.clear();
    export_db_destination_path = sqlite_viewer_export_db_path + "/" + recon_static_functions::get_sqlite_viewer_name().replace(" ","_") + "_exports.db";

    QFile export_db_check(export_db_destination_path);
    if(export_db_check.exists())
    {
        recon_static_functions::app_debug("export db exists and now deleting all records",Q_FUNC_INFO);

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(export_db_destination_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug("export db open --FAILED--- (if exists)",Q_FUNC_INFO);
        }
        else
        {
            QSqlQuery dest_query(destination_db);
            if(!dest_query.exec("delete from exportfilelist"))
            {
                recon_static_functions::app_debug("delete query ---FAILED-----",Q_FUNC_INFO);
                recon_static_functions::app_debug(dest_query.lastError().text(),Q_FUNC_INFO);
            }

            destination_db.close();
        }
    }
    else
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(export_db_destination_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug("export db open --FAILED--- (else)",Q_FUNC_INFO);
        }
        else
        {

            QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
            recon_helper_standard_obj->execute_db_command_by_dbreference(export_file_cmd , destination_db, Q_FUNC_INFO);

            destination_db.close();
        }
    }
    // Now get table names which have export enabled from filterview //


    for(int i = 0; i < ui->comboBox_dbtables_selection->count(); i++)
    {

        struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(i);

        m_tablewidget*tableWidget_main_data_display = struct_main_object.tableWidget_MainDataDisplay_ptr;

        QTableWidget*tableWidget_filter_view_search = struct_main_object.tableWidget_FilterViewSearch_ptr;
        QTableWidget*tableWidget_column_selection = struct_main_object.tableWidget_columns_selection_ptr;

        QString mytable_name = struct_main_object.table_name;

        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
        {
            struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
            if(struct_main_object.table_name != mytable_name)
            {
                continue;
            }
        }

        export_table_name.clear();
        export_table_name = struct_main_object.table_name;
        export_columns_name_list.clear();

        for(int iter=0; iter < tableWidget_filter_view_search->rowCount(); iter++)
        {
            QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget((iter) ,2))->currentText());
            if(recivedComboboxText != "Select Conversion")
            {
                if(recivedComboboxText == "File")
                {
                    export_columns_name_list << tableWidget_filter_view_search->item(iter,0)->text();
                }
            }
        }//tableWidgetFilterViewSearch

        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
        {
            bool bool_check_bookmark = false;
            for(int row = 0; row < tableWidget_main_data_display->rowCount(); row++)
            {
                if(tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Unchecked)
                    continue;
                else
                    bool_check_bookmark = true;
            }

            if(!bool_check_bookmark)
            {
                export_columns_name_list.clear();
                export_table_name.clear();
                continue;
            }
        }

        if(export_columns_name_list.size() > 0)
        {
            export_file_fun(REPORT_CONTENT_FLAG,export_table_name,file_path);
        }

        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
        {
            break;
        }

    }//comboBox_dbtables_selection
    recon_static_functions::app_debug("END ",Q_FUNC_INFO);
}
