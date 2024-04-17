#include "table_result_loader.h"

void table_result_loader::slot_right_click_main_menu_triggered(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
    {
        action_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_remove_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Note))
    {
        action_add_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Notes_to_Bookmarks))
    {
        action_add_note_to_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        action_remove_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks))
    {
        action_remove_note_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Export))
    {
        action_export_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record))
    {
        action_go_to_record_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    {
        action_copy_to_clipboard();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Tag))
    {
        slot_act_remove_tags();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Go_To_Source))
    {
        action_goto_source_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Seen))
    {
        action_mark_as_seen_unseen(QString(MACRO_Generic_Right_Click_Mark_As_Seen));
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Unseen))
    {
        action_mark_as_seen_unseen(QString(MACRO_Generic_Right_Click_Mark_As_Unseen));
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove))
    {
        action_remove_record();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_In_Window))
    {
        action_open_full_detailed_info_triggered();
    }

}

void table_result_loader::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;
}

void table_result_loader::slot_act_create_tags()
{
    add_tags_module_obj->show();
}

void table_result_loader::slot_add_tags(QString tag_data , QString colour)
{
    update_tags_value(tag_data,colour);
}

void table_result_loader::slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

        emit signal_update_tag_submenu_of_plugins(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_update_tags_in_actions_menu_without_signal_emit(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_act_remove_tags()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

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

        if(plugin_name_str.isEmpty())
            continue;



        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);
        QString tab_column_name = db_columns_list.at(tab_name_index);
        QString os_scheme_column_name = db_columns_list.at(os_name_index);

        // Because this class support only CSV of one tab only. confirmed by Mj sir
        QString table_name;
        if(table_name_list.size() > 0)
            table_name = table_name_list.at(0);

        QString command = "SELECT recon_tag_value from '" + table_name + "' where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
        QStringList arg_list;
        arg_list << record_no << plugin_name_str << tab_name_str << os_name_str;
        QString tag_to_be_removed = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

        command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
        arg_list.clear();
        arg_list << "" << record_no << plugin_name_str << tab_name_str << os_name_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);


        QIcon icon("");
        int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);


        emit signal_update_tags_to_other_db(plugin_name_str,tab_name_str,record_no,QStringList(tag_to_be_removed),os_name_str);
        emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record_no,QStringList(""),os_name_str);

    }

    //    else
    //    {
    //        int selected_row_no = m_tablewidget_obj->currentRow();

    //        if(selected_row_no < 0)
    //            return;

    //        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


    //        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

    //        if(plugin_name_str.isEmpty())
    //            return;


    //        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
    //        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

    //        QString record_no_column_name = db_columns_list.at(record_no_index);
    //        QString plugin_column_name = db_columns_list.at(plugin_name_index);
    //        QString tab_column_name = db_columns_list.at(tab_name_index);
    //        QString os_scheme_column_name = db_columns_list.at(os_name_index);

    //        // Because this class support only CSV of one tab only. confirmed by Mj sir
    //        QString table_name;
    //        if(table_name_list.size() > 0)
    //            table_name = table_name_list.at(0);

    //        QString command = "SELECT recon_tag_value from '" + table_name + "' where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
    //        QStringList arg_list;
    //        arg_list << record_no << plugin_name_str << tab_name_str << os_name_str;
    //        QString tag_to_be_removed = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

    //        command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
    //        arg_list.clear();
    //        arg_list << "" << record_no << plugin_name_str << tab_name_str << os_name_str;
    //        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

    //        int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    //        tablewidget_general_clicked(selected_row_no,co_index);

    //        QIcon icon("");
    //        int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    //        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);

    //        emit signal_update_tags_to_other_db(plugin_name_str,tab_name_str,record_no,QStringList(tag_to_be_removed),os_name_str);
    //        emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record_no,QStringList(""),os_name_str);
    //    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::slot_remaining_tags_list(QStringList list_tags_remained, QString table_name, QString record)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();


    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;


    QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
    QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();

    QString record_no_column_name = db_columns_list.at(record_no_index);
    QString plugin_column_name = db_columns_list.at(plugin_name_index);
    QString tab_column_name = db_columns_list.at(tab_name_index);
    QString os_scheme_column_name = db_columns_list.at(os_name_index);

    QString tags_str = list_tags_remained.join(",");

    QString command1 = "SELECT recon_tag_value from '" + table_name + "' where INT=?";
    QStringList list;
    list << record;

    QString previous_tag = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command1,list,0, destination_db_path, Q_FUNC_INFO);

    QStringList listoftags = previous_tag.split(",", Qt::SkipEmptyParts);
    if(listoftags.contains(QString(MACRO_Tag_Name_Bookmarks)))
    {
        if(tags_str.trimmed().isEmpty())
            tags_str.append(QString(MACRO_Tag_Name_Bookmarks));
        else
            tags_str.append("," + QString(MACRO_Tag_Name_Bookmarks));

        list_tags_remained << QString(MACRO_Tag_Name_Bookmarks);

    }

    QString command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
    QStringList arg_list;
    arg_list << tags_str << record_no << plugin_name_str << tab_name_str << os_name_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

    int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    tablewidget_general_clicked(current_row,co_index);

    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    if(tags_str == "")
    {
        QIcon icon(QString(""));
        m_tablewidget_obj->item(current_row,tag_position)->setIcon(icon);
    }
    else if(tags_str.contains(","))
    {
        QString icon_path = "../icons/tag_icons/multiple.png";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(current_row,tag_position)->setIcon(icon);
    }
    else if((tags_str != "") && (!tags_str.contains(",")))
    {
        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags_str, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
        color = color.toLower();
        color = color.replace(" ","_");
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(color, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(current_row,tag_position)->setIcon(icon);
    }

    //    QString record_no_int = recon_static_functions::remove_record_no_appended_zero(record_no);

    emit signal_update_tags_to_other_db(plugin_name_str,tab_name_str,record_no,list_tags_remained,os_name_str);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::slot_tablewidget_cell_clicked(int row, int column)
{
    if(row < 0)
        return;

    tablewidget_general_clicked(row,column);
}

void table_result_loader::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading =  cancel_status;
    export_rsync_obj->pub_cancel_rsync_copy(Q_FUNC_INFO);
    display_loading_progress_bar_obj->hide();
}

void table_result_loader::slot_tableWidget_doubleClicked(int row, int column)
{
    m_tablewidget *m_tablewidget_obj = (m_tablewidget*) sender();

    //- On last column due to crash returned from below lines.
    if(column == (m_tablewidget_obj->columnCount() - 1))
        return;

    m_tablewidget_obj->clearSelection();

    QList <QAction *> act_list = mainMenu->actions();

    for(int i = 0 ; i < act_list.size() ; i++)
    {
        if(act_list.at(i)->text().trimmed() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
            continue;
        else
            act_list.at(i)->setDisabled(true);
    }

    m_tablewidget_obj->item(row,column)->setSelected(true);
}

void table_result_loader::slot_add_notes_pushbutton_clicked(QString text)
{

}

void table_result_loader::slot_add_notes_to_bookmark_pushbutton_clicked(QString text)
{

}

void table_result_loader::slot_insert_record_in_tablewidget(QString record_str,QString plugin_name,QString tab_name,QString tag_name, QString os_scheme)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(tag_name.isEmpty())
        return;

    if(tag_name != clicked_item_tab_text)
        return;

    QString record_no_column_name = db_columns_list.at(record_no_index);
    QString plugin_column_name = db_columns_list.at(plugin_name_index);
    QString tab_column_name = db_columns_list.at(tab_name_index);
    QString os_scheme_column_name = db_columns_list.at(os_name_index);

    QString table_name;
    if(!table_name_list.isEmpty())
        table_name = table_name_list.at(0);

    QStringList values_list;
    values_list << record_str << plugin_name << tab_name << os_scheme;

    QString db_cmd;
    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }

    if(!db_cmd.isEmpty())
        db_cmd.append("notes");


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);


    db_cmd.append(QString(" From '"));
    db_cmd.append(table_name);
    db_cmd.append("' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?");

    insert_record_in_table_display(destination_db_path,db_cmd,display_header_name_list,values_list);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_open_with_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        action_open_with_plist_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        action_open_with_hex_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        action_open_with_sqlite_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        action_open_with_log_viewer_triggered();
    }
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_sendtobucket_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        action_sendToBucket_plist_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        action_sendToBucket_hex_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        action_sendToBucket_sqlite_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        action_sendToBucket_log_viewer_triggered();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::slot_tags_submenu_clicked(QAction* current_clicked_action)
{
    action_submenu_tag_triggered(current_clicked_action->text());
}

void table_result_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString keyword = lineEdit_search->text().trimmed();
    if(keyword.trimmed().isEmpty())
        return;

    searched_keyword_str = keyword;

    QStringList search_string_list = recon_static_functions::get_search_string_list(keyword, Q_FUNC_INFO);
    if(search_string_list.size() <= 0)
    {
        return;
    }

    QString table_name;

    if(table_name_list.size() > 0)
        table_name = table_name_list.at(0);

    QStringList cols_nam_list =  db_columns_list;
    cols_nam_list << "notes";


    QString command = cols_nam_list.join(",");
    command = "select " + command + " from '" + table_name + "'";

    QString partial_command = recon_static_functions::get_partial_command_forsearch(cols_nam_list,search_string_list, Q_FUNC_INFO);
    command = command + partial_command;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query_index(*destination_db);
    query_index.prepare(command);
    for(int k = 0 ; k < cols_nam_list.size() ; k++)
    {
        if(cols_nam_list.at(k) == "os_scheme_display")
            continue ;

        for(int i = 0; i < search_string_list.size(); i++)
        {
            query_index.addBindValue(search_string_list.at(i));
        }
    }

    populate_data_in_tablewidget(*destination_db, query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    emit signal_clear_all_metadata();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}
void table_result_loader::slot_tablewidget_current_cell_changed(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    bool_cell_changed_connection_buildup_for_tablewidget = true;
    tablewidget_general_clicked(currentRow, currentColumn);
}

void table_result_loader::slot_action_export_all_file_system_records_triggered()
{

}

void table_result_loader::slot_action_export_selected_file_system_records_triggered()
{

}

void table_result_loader::slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path)
{

}

void table_result_loader::slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path,bool bool_is_enc_zip,QString password_enc_zip)
{

}

void table_result_loader::slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dir_path)
{

}

void table_result_loader::slot_main_table_widget_item_clicked(QTableWidgetItem *item)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(item == NULL)
        return;

    if(item->row() < 0)
        return;

    if(item->column() != enum_tablewidget_column_bookmark)
        return;

    int current_row = item->row();

    if(current_row < 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index != -1)
    {
        if(item->checkState())
        {
            update_bookmark_value("1",current_row,bookmark_index);
        }
        else
        {
            update_bookmark_value("0",current_row,bookmark_index);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_thread_zip_file_export_finished()
{

}

void table_result_loader::slot_sub_menu_open_file_with_other_app_clicked(QAction *current_clicked_action)
{

}


void table_result_loader::slot_bookmark_key_pressed_tablewidget()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index != -1)
    {
        if(m_tablewidget_obj->item(current_row, bookmark_index)->checkState())
        {
            update_bookmark_value("0",current_row,bookmark_index);
        }
        else
        {
            update_bookmark_value("1",current_row,bookmark_index);
        }
    }

    //    if(current_row != m_tablewidget_obj->rowCount() - 1)
    //    {
    //        m_tablewidget_obj->setCurrentCell(current_row + 1, 1);

    //        tablewidget_general_clicked(m_tablewidget_obj->currentRow(),m_tablewidget_obj->currentColumn());
    //    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::slot_spacebar_quicklook_key_pressed_on_tablewidget()
{
    slot_act_quick_look_triggered();
}

void table_result_loader::slot_process_video_thumbnails_finished(int)
{
    bool_process_thumbnails_finished = true;
}

void table_result_loader::slot_arrow_key_pressed_on_tablewidget()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);


    tablewidget_general_clicked(current_row,co_index);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_control_e_key_pressed_on_tablewidget()
{
    action_export_triggered();
}

void table_result_loader::slot_act_create_hashset()
{
    QFileInfo info(current_media_path);
    if(!info.exists())
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,info.fileName() + " file not exist.");
        message_dialog_obj->show();

        return;
    }

    hashset_storage_obj->show();
}

void table_result_loader::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str)
{

}



void table_result_loader::slot_add_files_to_hashset_submenu_clicked(QAction* current_clicked_action)
{

}


void table_result_loader::slot_open_detailed_info_submenu_clicked(QAction *current_clicked_action)
{
}

void table_result_loader::slot_action_search_file_with_same_hash_clicked()
{

}

void table_result_loader::slot_pushbutton_showall_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    m_tablewidget_obj->blockSignals(true);


    while(m_tablewidget_obj->rowCount())
        m_tablewidget_obj->removeRow(0);

    searched_keyword_str.clear();
    lineEdit_search->clear();

    emit signal_clear_all_metadata();

    display_data(destination_db_path);


    m_tablewidget_obj->blockSignals(false);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void table_result_loader::slot_tag_list_to_be_removed(QStringList list_removed_tags, QString table_name, QString record_no)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();
    if(plugin_name_str.isEmpty())
        return;


    QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text().trimmed();



    QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();

    emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record_no,list_removed_tags,os_name_str);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_sort_tablewidget_display(int column_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    return;

    if(m_tablewidget_obj == NULL)
        return;

    //Return if clicked column index other then Bookmark, Tags or Notes
    if(column_index != enum_tablewidget_column_bookmark && column_index != enum_tablewidget_column_notes && column_index != enum_tablewidget_column_tags)
    {
        m_tablewidget_obj->sortByColumn(column_index,Qt::AscendingOrder);
        m_tablewidget_obj->setSortingEnabled(false);
        return;
    }

    m_tablewidget_obj->clearSelection();

    if(m_tablewidget_obj->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_tablewidget_obj->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->item(row, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_tablewidget_column_bookmark)
                icon_set =  m_tablewidget_obj->item(row, column_index)->icon();



            m_tablewidget_obj->item(row, column_index)->checkState();

            // Check for checkstate OR iCon
            if(m_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column_index != enum_tablewidget_column_bookmark)
                    icon_shiftrow_acs =  m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->icon();

                // If row is already on 0th position then increment and continue
                if(m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                // Shifting of rows from lower to upper
                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);


                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(m_tablewidget_obj->item(row, column_index) == NULL)
                continue;

            if(m_tablewidget_obj->item(row_shift_pos_descending, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_tablewidget_column_bookmark)
                icon_set =  m_tablewidget_obj->item(row, column_index)->icon();

            if(m_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column_index != enum_tablewidget_column_bookmark)
                    icon_shiftrow_desc =  m_tablewidget_obj->item(row_shift_pos_descending, column_index)->icon();

                // If row is already on last position then decrement and continue
                if(m_tablewidget_obj->item(row_shift_pos_descending, column_index)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                // Shifting of rows from upper to lower
                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_update_hashset_submenu(QString db_name, QString db_path)
{

    if(submenu_add_files_to_hashset_db == NULL)
        return;

    QAction *act_add_file = new QAction(tr(db_name.toLocal8Bit().data()), this->submenu_add_files_to_hashset_db);
    act_add_file->setToolTip(db_path);

    submenu_add_files_to_hashset_db->addAction(act_add_file);

}

void table_result_loader::slot_goto_artifact_source_submenu_clicked(QAction *clicked_action)
{

}

void table_result_loader::slot_act_quick_look_triggered()
{
    emit signal_act_quick_look_triggered();
}

void table_result_loader::slot_goto_screenshot_record(QString record_str)
{

}

void table_result_loader::slot_abouttoshow_tags_in_submenu()
{
    create_tags_submenu();
}

void table_result_loader::slot_add_file_to_text_indexing_queue_clicked()
{
}

void table_result_loader::slot_menu_export_as_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_Generic_Export_As_CSV))
    {
        QString file_name = QString("Tag_Items");
        QString lab_export_dir_path = getenv("HOME") + QString("/Desktop") + "/LAB_Export";
        export_dialog_box_2_obj->pub_set_exported_file_name_and_dir_path(file_name, lab_export_dir_path);
        export_dialog_box_2_obj->pub_set_export_type(QString(MACRO_Generic_Export_As_CSV));

        export_dialog_box_2_obj->show();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Export_As_SQLite))
    {
        QString file_name = QString("Tag_Items");
        QString lab_export_dir_path = getenv("HOME") + QString("/Desktop") + "/LAB_Export";
        export_dialog_box_2_obj->pub_set_exported_file_name_and_dir_path(file_name, lab_export_dir_path);
        export_dialog_box_2_obj->pub_set_export_type(QString(MACRO_Generic_Export_As_SQLite));

        export_dialog_box_2_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_save_results_by_export_as_csv_sqlite(QString file_name , QString selected_dir_path, QString export_type_str , QString report_scope)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    export_dialog_box_2_obj->hide();

    file_name = file_name.trimmed();
    selected_dir_path = selected_dir_path.trimmed();

    report_scope_str = report_scope;


    if(file_name.isEmpty() || selected_dir_path.isEmpty())
        return;

    if(!selected_dir_path.endsWith("/"))
        selected_dir_path.append("/");

    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QString filepath;
    if(export_type_str == QString(MACRO_Generic_Export_As_CSV))
    {
        filepath = selected_dir_path + file_name + ".csv";

        QString available_filename = recon_static_functions::get_available_filename(QFileInfo(filepath).fileName() , selected_dir_path, Q_FUNC_INFO);
        filepath = selected_dir_path + available_filename;

        export_data_as_csv(filepath);
    }
    else if(export_type_str == QString(MACRO_Generic_Export_As_SQLite))
    {
        filepath = selected_dir_path + file_name + ".sqlite";

        QString available_filename = recon_static_functions::get_available_filename(QFileInfo(filepath).fileName() , selected_dir_path, Q_FUNC_INFO);
        filepath = selected_dir_path + available_filename;

        export_data_as_sqlite(filepath);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    QList<struct_search_column_text_for_file_type> search_text_col_QList;

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_file_type struct_search_column_text_for_file_type_obj;

            struct_search_column_text_for_file_type_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_file_type_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_file_type_obj);
            bool_stop_sorting_tags_bookmark_notes_after_search = true;
        }
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    if(search_text_col_QList.size() < 1)
    {
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(row % 20 == 0)
                QCoreApplication::processEvents();

            m_tablewidget_obj->showRow(row);
        }
        bool_stop_sorting_tags_bookmark_notes_after_search = false;

        display_loading_progress_bar_obj->hide();
        return;
    }

    all_column_text_search(search_text_col_QList);

    pub_refresh_horizontal_header();

    emit signal_clear_all_metadata();
    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void table_result_loader::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void table_result_loader::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj == NULL)
        return;

    if((column != enum_tablewidget_column_bookmark || !custom_header_widgets_QList.at(column)->pub_get_column_name().isEmpty())
            && (column != enum_tablewidget_column_notes || !custom_header_widgets_QList.at(column)->pub_get_column_name().isEmpty())
            && (column != enum_tablewidget_column_tags || !custom_header_widgets_QList.at(column)->pub_get_column_name().isEmpty()))
    {

        if(column_sort_order == 0)
            m_tablewidget_obj->sortItems(column, Qt::DescendingOrder);
        else
            m_tablewidget_obj->sortItems(column, Qt::AscendingOrder);

        pub_refresh_horizontal_header();
        return;

    }

    if(bool_stop_sorting_tags_bookmark_notes_after_search)
        return;


    m_tablewidget_obj->clearSelection();

    if(m_tablewidget_obj->rowCount() <= 1)
        return;

    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_tablewidget_obj->rowCount() - 1;

    if(column_sort_order == 1)
    {
        for(qint64 row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column)->icon();

            if(m_tablewidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                icon_shiftrow_acs =  m_tablewidget_obj->item(row_shift_pos_ascending, column)->icon();

                if(m_tablewidget_obj->item(row_shift_pos_ascending, column)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_ascending++;
            }
        }
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            if(m_tablewidget_obj->item(row, column) == NULL)
                continue;

            if(m_tablewidget_obj->item(row_shift_pos_descending, column) == NULL)
                continue;

            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column)->icon();

            if(m_tablewidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                icon_shiftrow_desc =  m_tablewidget_obj->item(row_shift_pos_descending, column)->icon();

                if(m_tablewidget_obj->item(row_shift_pos_descending, column)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }
    }


    pub_refresh_horizontal_header();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void table_result_loader::all_column_text_search(QList<struct_search_column_text_for_file_type> search_text_all_column_list)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool hide_row = false;

        for(int column_count = 0; column_count < search_text_all_column_list.size(); column_count++)
        {
            struct_search_column_text_for_file_type struct_search_column_text_for_file_type_obj = search_text_all_column_list.at(column_count);

            if(!(m_tablewidget_obj->item(row, struct_search_column_text_for_file_type_obj.column_number)->text().trimmed().contains(struct_search_column_text_for_file_type_obj.search_text, Qt::CaseInsensitive)))
            {
                hide_row = true;

                break;
            }
        }

        if(hide_row)
        {
            m_tablewidget_obj->hideRow(row);
        }
        else
        {
            m_tablewidget_obj->showRow(row);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}
