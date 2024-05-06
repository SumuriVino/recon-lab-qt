#include "carved_data_loader.h"

carved_data_loader::carved_data_loader(QWidget *parent) : table_result_loader(parent)
{
    setWindowFlags(Qt::Window);

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    plugin_name = MACRO_Plugin_Name_Carved_Data;
    carved_data_tab_name = MACRO_Plugin_Carved_Data_TAB_Name_Carved_Data;
}

void carved_data_loader::pub_set_carved_data_display_db_path(QString db_path)
{ // set carver display db path
    destination_db_path = db_path;
}

void carved_data_loader::populate_data_in_table()
{ // popualte carved data in table widget
    current_table_name = clicked_item_tab_text;
    category_name = clicked_item_tab_text;

    notes_icon_index  = 1;
    tag_icon_index    = 2;
    record_no_index   = 3;
    plugin_name_index = 4;
    tab_name_index    = 5;
    category_name_index = 6;
    number_of_hits_name_index = 7;
    carved_keyword_index = 8;
    source_plugin_name_index = 9;
    source_tab_name_index = 10;
    source_record_no_index = 11;
    source_filename_index = 12;
    source_filepath_index = 13;
    root_count_name_index = 14;
    source_count_name_index = 15;


    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(root_count_name_index);
    m_tablewidget_obj->hideColumn(source_record_no_index);
    m_tablewidget_obj->hideColumn(source_tab_name_index);
    m_tablewidget_obj->hideColumn(source_plugin_name_index);
    m_tablewidget_obj->hideColumn(plugin_name_index);
    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(category_name_index);
    //    m_tablewidget_obj->hideColumn(source_filepath_index);
    //    m_tablewidget_obj->hideColumn(source_filename_index);


    display_data(destination_db_path);

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);

    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark, menuList);

    //Tags
    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
    create_tags_submenu();

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

    //Notes
    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Add_Note)
             << QString(MACRO_Generic_Right_Click_Remove_Note);

    submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
    create_right_click_actions(submenu_note, menuList);
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);
    create_right_click_actions(mainMenu , menuList);

}

void carved_data_loader::display_data(QString dest_db_path)
{ // display the data on tablewidget from database
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    m_tablewidget_obj->setRowCount(0);

    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")
    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }

    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    db_cmd.append(QString(" From '") + current_table_name + "'");


    command_for_table_display = db_cmd;
    destination_db_path = dest_db_path;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(dest_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + dest_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }


    QString command = "select distinct recon_filefrom , recon_file_infopath from '" + current_table_name + "'";
    QSqlQuery sql_query = recon_helper_standard_obj->get_executed_sqlquery_from_db_by_dbreference(command,*destination_db,Q_FUNC_INFO);
    recon_file_from = sql_query.value("recon_filefrom").toString();
    recon_file_info_path = sql_query.value("recon_file_infopath").toString();

    //recon_file_from = recon_helper_standard_obj->get_string_from_db_by_dbreference(command,0,*destination_db,Q_FUNC_INFO);
    //recon_file_info_path = recon_helper_standard_obj->get_string_from_db_by_dbreference(command,1,*destination_db,Q_FUNC_INFO);


    QSqlQuery query_index(*destination_db);
    query_index.prepare(command_for_table_display);


    populate_data_in_tablewidget(*destination_db, query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

int carved_data_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{ // populate data in tablewidget from database
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(display_header_name_list.isEmpty())
    {
        recon_static_functions::app_debug("display_header_name_list.isEmpty() ----FAILED---- " , Q_FUNC_INFO);
        return -1;
    }

    bool_display_table_data_loaded = true;

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_index.executedQuery(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        return -1;
    }

//    while(m_tablewidget_obj->rowCount())
//        m_tablewidget_obj->removeRow(0);

    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbreference(false, received_db_obj, QStringList(current_table_name), Q_FUNC_INFO);
    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    record_no_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    tag_icon_index  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    bookmark_index  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    notes_value_index  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);

    int count = m_tablewidget_obj->rowCount();

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);

    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        total_covered_count++;

        loading_display_dialog_based_on_records(total_record_count , total_covered_count ,feature_name);
        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break ;
        }

        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            /// for bookmarks checkbox
            if(pos == record_no_index)
            {
                QTableWidgetItem *item_tablewidget_record_no;

                item_tablewidget_record_no = new QTableWidgetItem;
                item_tablewidget_record_no->setData(Qt::EditRole, dataStr_from_db.toLongLong());

                item_tablewidget_record_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_record_no);

            }
            else if(pos == bookmark_index)
            {
                QTableWidgetItem *item_tablewidget_bookmark;

                item_tablewidget_bookmark = new QTableWidgetItem;
                item_tablewidget_bookmark->setCheckState(Qt::Unchecked);


                if(dataStr_from_db == QString::number(1))
                    item_tablewidget_bookmark->setCheckState(Qt::Checked);
                else
                    item_tablewidget_bookmark->setCheckState(Qt::Unchecked);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_bookmark);

            }
            else if(pos == notes_icon_index)
            {
                QTableWidgetItem *item_tablewidget_notes_icon;

                item_tablewidget_notes_icon = new QTableWidgetItem;

                if(query_index.value(notes_value_index).toString().trimmed().isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_notes_icon->setIcon(icon);
                }
                else
                {
                    QIcon iconNotes("../icons/notes.png");
                    item_tablewidget_notes_icon->setIcon(iconNotes);
                }
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_notes_icon);

            }
            else if(pos == (tag_icon_index))
            {
                ///for tags

                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;

                if(dataStr_from_db.isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_tags->setIcon(icon);
                }
                else
                {
                    QString icon_str = dataStr_from_db;

                    if(icon_str.contains(","))
                    {
                        icon_str = "multiple";
                        QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                        item_tablewidget_tags->setIcon(icon);
                    }
                    else
                    {
                        icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        icon_str = icon_str.toLower();
                        icon_str = icon_str.replace(" ","_");
                        QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                        item_tablewidget_tags->setIcon(icon);
                    }
                }

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_tags);
            }
            else if(pos == blank_column_index)
            {
                QTableWidgetItem *item_tablewidget_blank = new QTableWidgetItem;
                item_tablewidget_blank->setText("");
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_blank);
            }
            else if(pos == notes_value_index)
            {
                QTableWidgetItem *item_tbl_notes = new QTableWidgetItem;
                item_tbl_notes->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tbl_notes);
            }
            else if(pos == number_of_hits_name_index)
            {
                QTableWidgetItem *item_tablewidget_hits_count;

                item_tablewidget_hits_count = new QTableWidgetItem;
                item_tablewidget_hits_count->setData(Qt::EditRole, dataStr_from_db.toLongLong());

                item_tablewidget_hits_count->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_hits_count);
            }
            else
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int);
            }

            pos ++;
        }
        count ++;
    }

    m_tablewidget_obj->hideColumn(notes_value_index);
    bool_display_table_data_loaded = false;

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void carved_data_loader::contextMenuEvent(QContextMenuEvent *evt)
{ // right click menu creation
    if(evt == NULL)
        return;

    if(bool_display_table_data_loaded)
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int current_row    = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();

    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
    if(plugin_name_str.trimmed().isEmpty())
        return;


    if(!m_tablewidget_obj->selectionModel()->selectedRows().isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }


    act_full_detail_info->setEnabled(true);

    if(selection_model_list.size() > 1)
    {
        act_full_detail_info->setEnabled(false);
    }
    else
    {
        act_full_detail_info->setEnabled(true);
    }


    mainMenu->exec(QCursor::pos());
}

void carved_data_loader::tablewidget_general_clicked(int row,int column)
{ // generally clicked on tablewidget anywhere or any record
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    if(record_no_str.trimmed().isEmpty())
        return;

    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();

    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(current_table_name ,0,record_no_str,plugin_name,destination_db_path,source_count_name,Q_FUNC_INFO);
    current_media_path = "";
    preview_file_path  = "";
    metadata_str       = struct_metadata_values_obj.metadata;


    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.db_file_path = destination_db_path;
    st_set_meta_obj.source_count_name  = source_count_name;
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = plugin_name;
    st_set_meta_obj.tab_name = carved_data_tab_name;

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_data_loader::update_tags_value(QString tag_name,QString colour)
{ // update tags value
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List , source_count_name_list;
    QList<struct_global_tagging_carved_data> carved_data_record_list;

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    dest_db.setDatabaseName(destination_db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + dest_db.lastError().text() , Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << record_no;

        QString source_count_name_str = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        source_count_name_list << source_count_name_str;


        struct_global_tagging_carved_data s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.category_name = category_name;
        s1.table_name = current_table_name;
        s1.source_count_name = source_count_name_str;

        s1.source_file_name = m_tablewidget_obj->item(selected_row_no, source_filename_index)->text();
        s1.source_file_path = m_tablewidget_obj->item(selected_row_no, source_filepath_index)->text();
        s1.source_plugin_name = m_tablewidget_obj->item(selected_row_no, source_plugin_name_index)->text();
        s1.source_tab_name = m_tablewidget_obj->item(selected_row_no, source_tab_name_index)->text();
        s1.source_record_no = m_tablewidget_obj->item(selected_row_no, source_record_no_index)->text();
        s1.root_count_name = m_tablewidget_obj->item(selected_row_no, root_count_name_index)->text();
        s1.recon_file_from = recon_file_from;
        s1.recon_file_infopath = recon_file_info_path;

        s1.item0 = m_tablewidget_obj->item(selected_row_no, number_of_hits_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, carved_keyword_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, source_filename_index)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, source_filepath_index)->text();

        s1.item11 = category_name;
        s1.item12 = current_table_name;

        s1.notes_str = m_tablewidget_obj->item(selected_row_no, notes_value_index)->text();

        QString tagging_on_current_record = is_tagged_current_record_record(selected_row_no , record_no , dest_db);
        s1.tagging_on_record = tagging_on_current_record;

        carved_data_record_list << s1;


        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_name, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_icon_index)->setIcon(icon);
    }
    dest_db.close();


    global_connection_manager_obj->pub_add_data_in_export_content_carved_data_db(carved_data_record_list,tag_name);
    global_connection_manager_obj->pub_update_tag_export_content_carved_data_db(INT_List,category_name ,source_count_name_list,tag_name);
    global_connection_manager_obj->pub_set_tag_in_carved_data_db(INT_List, current_table_name,tag_name);
    global_connection_manager_obj->pub_add_tag_for_carved_data(carved_data_record_list,tag_name);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_data_loader::action_remove_note_triggered()
{ // remove note action clicked from reight click menu
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    dest_db.setDatabaseName(destination_db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + dest_db.lastError().text() , Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QStringList INT_List;
    QStringList source_count_namelist;
    QStringList tagging_list_on_record;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << record_no;

        source_count_namelist << m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        QIcon iconNotes("");
        m_tablewidget_obj->item(selected_row_no, notes_icon_index)->setIcon(iconNotes);
        m_tablewidget_obj->item(selected_row_no, notes_value_index)->setText("");

        tagging_list_on_record <<  is_tagged_current_record_record(selected_row_no  , record_no , dest_db);
    }
    dest_db.close();
    global_connection_manager_obj->pub_delete_entry_export_content_carved_data_db(INT_List,category_name ,source_count_namelist , tagging_list_on_record);
    global_connection_manager_obj->pub_update_notes_export_content_carved_data_db(INT_List,category_name ,source_count_namelist,"");
    global_connection_manager_obj->pub_remove_notes_in_carved_data_db(INT_List,current_table_name);
    global_connection_manager_obj->pub_delete_notes_for_carved_data_db(INT_List,category_name , source_count_namelist);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_data_loader::action_open_full_detailed_info_triggered()
{ // open full detail info action clicked
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString searched_keyword = searched_keyword_str;
    QString source_count_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str,"", 0,searched_keyword,source_count_name_str,record_no_str, "","");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_data_loader::action_bookmark_triggered()
{ // action add bookmark clicked from right click menu
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    dest_db.setDatabaseName(destination_db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + dest_db.lastError().text() , Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    QStringList INT_List , source_count_name_list;
    QList<struct_global_tagging_carved_data> carved_data_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();


        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no =  m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << record_no;
        QString source_count_name_str = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        source_count_name_list << source_count_name_str;

        struct_global_tagging_carved_data s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.category_name = category_name;
        s1.table_name = current_table_name;
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        s1.source_file_name = m_tablewidget_obj->item(selected_row_no, source_filename_index)->text();
        s1.source_file_path = m_tablewidget_obj->item(selected_row_no, source_filepath_index)->text();
        s1.source_plugin_name = m_tablewidget_obj->item(selected_row_no, source_plugin_name_index)->text();
        s1.source_tab_name = m_tablewidget_obj->item(selected_row_no, source_tab_name_index)->text();
        s1.source_record_no = m_tablewidget_obj->item(selected_row_no, source_record_no_index)->text();
        s1.root_count_name = m_tablewidget_obj->item(selected_row_no, root_count_name_index)->text();
        s1.recon_file_from = recon_file_from;
        s1.recon_file_infopath = recon_file_info_path;

        s1.item0 = m_tablewidget_obj->item(selected_row_no, number_of_hits_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, carved_keyword_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, source_filename_index)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, source_filepath_index)->text();

        s1.item11 = category_name;
        s1.item12 = current_table_name;

        s1.bookmark_status = MACRO_TRUE_VALUE_NUMERIC_QString;
        s1.notes_str = m_tablewidget_obj->item(selected_row_no, notes_value_index)->text();

        QString tagging_on_current_record = is_tagged_current_record_record(selected_row_no, record_no , dest_db);
        s1.tagging_on_record = tagging_on_current_record;
        carved_data_record_list << s1;
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

    }
    dest_db.close();

    global_connection_manager_obj->pub_add_data_in_export_content_carved_data_db(carved_data_record_list,"");
    global_connection_manager_obj->pub_update_bookmark_export_content_carved_data_db(INT_List,category_name,source_count_name_list,MACRO_TRUE_VALUE_NUMERIC_QString);

    global_connection_manager_obj->pub_set_bookmark_in_carved_data_db(INT_List,current_table_name);
    global_connection_manager_obj->pub_add_bookmark_for_carved_data_db(carved_data_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_data_loader::update_bookmark_value(QString status,int row, int column)
{ // update bookmark value in tags weather add(1) or remove(0)

    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    if(column != 0)
        return;

    QList<struct_global_tagging_carved_data> carved_data_record_list;
    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;

    QString record_no =  m_tablewidget_obj->item(row, record_no_index)->text();
    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();


    struct_global_tagging_carved_data s1;
    s1.INT_record = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(row, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(row, tab_name_index)->text();
    s1.category_name = category_name;
    s1.table_name = current_table_name;
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();

    s1.source_file_name = m_tablewidget_obj->item(row, source_filename_index)->text();
    s1.source_file_path = m_tablewidget_obj->item(row, source_filepath_index)->text();
    s1.source_plugin_name = m_tablewidget_obj->item(row, source_plugin_name_index)->text();
    s1.source_tab_name = m_tablewidget_obj->item(row, source_tab_name_index)->text();
    s1.source_record_no = m_tablewidget_obj->item(row, source_record_no_index)->text();
    s1.root_count_name = m_tablewidget_obj->item(row, root_count_name_index)->text();
    s1.recon_file_from = recon_file_from;
    s1.recon_file_infopath = recon_file_info_path;

    s1.item0 = m_tablewidget_obj->item(row, number_of_hits_name_index)->text();
    s1.item1 = m_tablewidget_obj->item(row, carved_keyword_index)->text();
    s1.item2 = m_tablewidget_obj->item(row, source_filename_index)->text();
    s1.item3 = m_tablewidget_obj->item(row, source_filepath_index)->text();


    s1.item11 = category_name;
    s1.item12 = current_table_name;

    s1.bookmark_status = status;
    s1.notes_str = m_tablewidget_obj->item(row, notes_value_index)->text();

    QString tagging_on_current_record;
    QString cmd = QString("select recon_tag_value from '" + current_table_name +"' where INT = '" + record_no + "'");
    if(!m_tablewidget_obj->item(row, notes_value_index)->text().isEmpty())
        tagging_on_current_record = MACRO_TRUE_VALUE_NUMERIC_QString;
    else if(!recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd ,0,destination_db_path,Q_FUNC_INFO).trimmed().isEmpty())
        tagging_on_current_record = MACRO_TRUE_VALUE_NUMERIC_QString;

    s1.tagging_on_record = tagging_on_current_record;
    carved_data_record_list << s1;

    if(status == MACRO_TRUE_VALUE_NUMERIC_QString)
    {
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_add_data_in_export_content_carved_data_db(carved_data_record_list,"");
        global_connection_manager_obj->pub_update_bookmark_export_content_carved_data_db(QStringList(record_no),category_name,QStringList(source_count_name) , status);
        global_connection_manager_obj->pub_set_bookmark_in_carved_data_db(QStringList(record_no) ,current_table_name);
        global_connection_manager_obj->pub_add_bookmark_for_carved_data_db(carved_data_record_list);
    }
    else
    {
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_update_bookmark_export_content_carved_data_db(QStringList(record_no),category_name,QStringList(source_count_name) , status);
        global_connection_manager_obj->pub_delete_entry_export_content_carved_data_db(QStringList(record_no),category_name ,QStringList(source_count_name),QStringList(tagging_on_current_record));
        global_connection_manager_obj->pub_remove_bookmark_in_carved_data_db(QStringList(record_no),current_table_name);
        global_connection_manager_obj->pub_delete_bookmark_for_carved_data_db(QStringList(record_no),category_name ,QStringList(source_count_name));
    }

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

}

void carved_data_loader::action_add_note_triggered()
{ // add note action clicked from menu
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    m_tablewidget_obj->blockSignals(true);

    add_notes_to_singal_record_obj->set_notes("");
    add_notes_to_singal_record_obj->show();

    m_tablewidget_obj->blockSignals(false);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void carved_data_loader::action_remove_bookmark_triggered()
{ // remove bookmark clicked from menu
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    dest_db.setDatabaseName(destination_db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + dest_db.lastError().text() , Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QStringList INT_List , source_count_name_list;
    QStringList tagging_list_on_record ;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        INT_List << record_no;
        source_count_name_list << m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
        tagging_list_on_record << is_tagged_current_record_record(selected_row_no , record_no , dest_db);
    }

    dest_db.close();

    global_connection_manager_obj->pub_update_bookmark_export_content_carved_data_db(INT_List,category_name,source_count_name_list,MACRO_FALSE_VALUE_NUMERIC_QString);
    global_connection_manager_obj->pub_delete_entry_export_content_carved_data_db(INT_List,category_name ,source_count_name_list, tagging_list_on_record);
    global_connection_manager_obj->pub_remove_bookmark_in_carved_data_db(INT_List,current_table_name);
    global_connection_manager_obj->pub_delete_bookmark_for_carved_data_db(INT_List,category_name ,source_count_name_list);


    display_loading_progress_bar_non_cancelable_obj->hide();
}

QString carved_data_loader::is_tagged_current_record_record(int selected_row_no , QString record_no , QSqlDatabase &dest_db)
{ // check if current record is tagged or not
    QString cmd = QString("select recon_tag_value from '" + current_table_name +"' where INT = '" + record_no + "'");

//    QSqlQuery sql_query = recon_helper_standard_obj->get_executed_sqlquery_from_db_by_dbreference(command,*destination_db,Q_FUNC_INFO);
//    QString rcn_tag = sql_query.value("recon_tag_value").toString().trimmed();

    if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
        return MACRO_TRUE_VALUE_NUMERIC_QString;
    else if(!m_tablewidget_obj->item(selected_row_no, notes_value_index)->text().isEmpty())
        return MACRO_TRUE_VALUE_NUMERIC_QString;
    else if(!recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd ,0,dest_db,Q_FUNC_INFO).trimmed().isEmpty())
        return MACRO_TRUE_VALUE_NUMERIC_QString;
    else
        return MACRO_FALSE_VALUE_NUMERIC_QString;
}
