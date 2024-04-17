#include "saved_graph_loader.h"

saved_graphs_loader::saved_graphs_loader(QWidget *parent) : table_result_loader(parent)
{
    setWindowFlags(Qt::Window);

}

void saved_graphs_loader::pub_set_saved_graphs_display_db_path(QString db_path)
{
    destination_db_path_for_saved_graphs = db_path;
}

void saved_graphs_loader::saved_graphs_populate_data_in_table()
{
    record_no_index   = 3;
    file_path_index   = 7;
    plugin_name_index = 4;
    tab_name_index    = 5;

    creation_date_index = 8;
    blank_column_index = 10;


    m_tablewidget_obj->hideColumn(tab_name_index);
    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    m_tablewidget_obj->hideColumn(bookmark_position);
    m_tablewidget_obj->hideColumn(display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0));

    //    m_tablewidget_obj->hideColumn(tag_index);
    //m_tablewidget_obj->hideColumn(os_name_index);
    //m_tablewidget_obj->hideColumn(source_count_name_index);

    pub_refresh_horizontal_header();

    display_data(destination_db_path_for_saved_graphs);

    QStringList menuList;
    //    menuList << QString(MACRO_Generic_Right_Click_Bookmark)
    //             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks)


    menuList << QString(MACRO_Generic_Right_Click_Add_Note)
             << QString(MACRO_Generic_Right_Click_Remove_Note);
    submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {
        // For custom right click
        create_right_click_actions(submenu_note, menuList);

        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);

        create_right_click_actions(mainMenu , menuList);

        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        //Action Export
         act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

         create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

         create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Remove),mainMenu);
         create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

         connect(submenu_note,SIGNAL(triggered(QAction*)),this,SLOT(slot_notes_submenu_clicked(QAction*)));
    }
}

int saved_graphs_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
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

    while(m_tablewidget_obj->rowCount())
        m_tablewidget_obj->removeRow(0);

    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, destination_db_path, QStringList(), Q_FUNC_INFO);
    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,0);
    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int count = m_tablewidget_obj->rowCount();

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);

    pub_refresh_horizontal_header();

    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        total_covered_count++;
        if(total_record_count > 0)
        {
            if(total_covered_count % progressbar_increment_step == 0)
            {
                float record_percent = ((total_covered_count*100)/total_record_count);
                emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
            }
        }
        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);


        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            /// for bookmarks checkbox
            if(pos == record_no_position)
            {
                QTableWidgetItem *item_tablewidget_record_no;

                item_tablewidget_record_no = new QTableWidgetItem;
                item_tablewidget_record_no->setData(Qt::EditRole, dataStr_from_db.toLongLong());

                item_tablewidget_record_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_record_no);

            }
            else if(pos == bookmark_position)
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
            else if(pos == (tag_position))
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
            else if(pos == creation_date_index)
            {
                QTableWidgetItem *item_tablewidget_creation_datetime = new QTableWidgetItem;
                item_tablewidget_creation_datetime->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db.trimmed() , Q_FUNC_INFO));
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_creation_datetime);
            }
            else if(pos == notes_value_index)
            {
                //- No entry in the table widget item
            }
            else if(pos == file_path_index)
            {
                QString relative_path = dataStr_from_db;

                QTableWidgetItem *item_tablewidget_file_path = new QTableWidgetItem;
                item_tablewidget_file_path->setText(relative_path);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_file_path);

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

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void saved_graphs_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    if(bool_display_table_data_loaded)
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int current_row = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();

    if(current_row < 0)
        return;

    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text();

    if(plugin_name_str.isEmpty())
        return;


    if(!m_tablewidget_obj->selectionModel()->selectedRows().isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }

    //tablewidget_general_clicked(current_row,current_column);

    act_export->setEnabled(false);
    act_full_detail_info->setEnabled(true);

    if(selection_model_list.size() > 1)
    {
        act_full_detail_info->setEnabled(false);
        act_export->setEnabled(true);
    }
    else
    {
        act_export->setEnabled(true);
        act_full_detail_info->setEnabled(true);
    }

    struct_global_right_clicks_status_depends_on_file_existance obj;
    obj.plugin_name = plugin_name_str;
    obj.feature_name = QString(MACRO_CASE_TREE_Graph_Result_Saved);
    obj.tab_name = m_tablewidget_obj->item(current_row,tab_name_index)->text();
    obj.selected_row_count = selection_model_list.size();
    obj.record_no = m_tablewidget_obj->item(current_row,record_no_index)->text();
    recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

    mainMenu->exec(QCursor::pos());
}

void saved_graphs_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    QString filepath        = m_tablewidget_obj->item(row, file_path_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text().trimmed();

    if(record_no_str.trimmed().isEmpty() || filepath.trimmed().isEmpty())
        return;

    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_graph(record_no_str,plugin_name,Q_FUNC_INFO);
    metadata_str       = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.db_file_path = destination_db_path;
    st_set_meta_obj.source_count_name = "";
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = plugin_name;
    st_set_meta_obj.tab_name = tab_name_str;

    emit signal_set_metadata_info(st_set_meta_obj);


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void saved_graphs_loader::update_tags_value(QString tag_data,QString colour)
{
    recon_static_functions::debug_intensive(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;


        QString record_no_column_name = db_columns_list.at(record_no_index);

        QString table_name;
        if(table_name_list.size() > 0)
            table_name = table_name_list.at(0);

        QString command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=?";
        QStringList arg_list;
        arg_list << tag_data << record_no_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);


        int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
        tablewidget_general_clicked(selected_row_no,co_index);
        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);

        QIcon icon(icon_path);
        int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);

        emit signal_add_tag_in_other_locations(tag_data,"0",plugin_name_str,tab_name_str,record_no_str,destination_db_path, "");

    }

    recon_static_functions::debug_intensive(" -Ends " , Q_FUNC_INFO);
}

void saved_graphs_loader::update_notes_value(QString text, int current_row)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- " + destination_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    if(table_name_list.size() > 0)
    {
        int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);

        QString table_name = table_name_list.at(0);     // Because Loader support only one tab in CSV

        QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();

        if(plugin_name_str.isEmpty())
            return;

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);

        QString command = "UPDATE '" + table_name + "' set notes=? WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? ";
        QStringList arg_list;
        arg_list << text << record_no << plugin_name_str ;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);

        QTableWidgetItem *item_tablewidget_notes_icon = m_tablewidget_obj->item(current_row,notes_icon_index);
        if(notes_icon_index != -1)
        {
            item_tablewidget_notes_icon = m_tablewidget_obj->item(current_row,notes_icon_index);
            if(text.isEmpty())
            {
                QIcon iconNotes("");
                item_tablewidget_notes_icon->setIcon(iconNotes);
            }
            else
            {
                QIcon iconNotes("../icons/notes.png");
                item_tablewidget_notes_icon->setIcon(iconNotes);
            }
        }

        QStringList tmp_list;
        emit signal_change_notes_in_other_db_also(record_no,plugin_name_str,tab_name_str,text, tmp_list , "" , feature_name);

    }

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void saved_graphs_loader::action_remove_note_triggered()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int notes_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    if(notes_column_index == -1)
        return;


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QIcon iconNotes("");
        m_tablewidget_obj->item(selected_row_no, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_remove_notes_in_saved_graphs_and_screenshot_db(INT_List,plugin_name);
    global_connection_manager_obj->pub_delete_notes_for_saved_graphs_and_screenshot_db(INT_List,plugin_name);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void saved_graphs_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    //QString source_count_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    QString searched_keyword = searched_keyword_str;

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    struct_global_recon_file_info st_globl_recon_file_info_obj ;
    if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_messenger_saved_graphs(record_no_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_browser_histroy_saved_graphs(record_no_str);
    }
    QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;

    int preview_index_full = 1;
    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    preview_index_full = get_stack_index_for_preview(complete_file_path ,display_file_path);

    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"",record_no_str,plugin_name_str,"");
    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void saved_graphs_loader::action_open_detach_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

//    if(m_tablewidget_obj->currentRow() < 0)
//        return;

//    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
//    QString filepath = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), file_path_index)->text();

//    QString searched_keyword = searched_keyword_str;
//    if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History) || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
//    {
//        int preview_index_full = 1;
//        preview_index_full = get_stack_index_for_preview(filepath);

//        emit signal_open_in_detach_clicked_table_result(metadata_str, filepath, filepath, preview_index_full,searched_keyword,"");


//    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void saved_graphs_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    QString dest_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();

    QStringList list_paths_to_message_box;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

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

        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,destination_db_path,"saved_graphs","" ,Q_FUNC_INFO);
        if(plugin_name_str.isEmpty())
            continue;

        QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path ,QFileInfo(complete_path).fileName(),Q_FUNC_INFO);
        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;

    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(list_paths_to_message_box.size() > 0)
    {
//        for(int i = 0 ; i < list_paths_to_message_box.size(); i++)
//        {
//            QProcess proc;

//            QStringList args;
//            args << "-R" << "0777" << list_paths_to_message_box.at(i);

//            proc.execute("chmod", args);

//            QCoreApplication::processEvents();
//        }

        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_path);
        message_dialog_obj->show();
    }else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"selected records does not have any file to export");
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void saved_graphs_loader::update_bookmark_value(QString status,int row, int column)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    QString bookmark_col_name = db_columns_list.at(bookmark_index);

    if(!table_name_list.isEmpty())
    {
        QString table_name = table_name_list.at(0);

        if(bookmark_index != -1)
        {
            if(status == QString("1"))
                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
            else
                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        }

        QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();

        QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();

        if(plugin_name_str.isEmpty())
            return;


        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);

        QStringList arg_list;
        QString command = "UPDATE '" + table_name + "' SET " + bookmark_col_name + "=? WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=?";
        arg_list << status << record_no << plugin_name_str ;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

        emit signal_change_bookmark_value_in_other_db(record_no,plugin_name_str,tab_name_str,status,QStringList(""),destination_db_path, "", feature_name);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}
