#include "file_types/filetypes_loader.h"

void filetypes_loader::slot_tableWidget_doubleClicked(int row, int column)
{ // double click on any record, if record is decompressed we can open it in same table widget by double clicking on it

    //- On last column due to crash returned from below lines.
    if(column == (m_tablewidget_obj->columnCount() - 1))
        return;

    m_tablewidget_obj->clearSelection();

    QString uncmp_stat = m_tablewidget_obj->item(row, enum_File_Decompression_Status_Index)->text().trimmed();

    QList <QAction *> act_list = mainMenu->actions();

    if(uncmp_stat == "Decompressed")
    {
        QString src_cnt_name = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index)->text().trimmed();
        QString record_no = m_tablewidget_obj->item(row, enum_Record_No_Index)->text().trimmed();
        QString file_display_path = m_tablewidget_obj->item(row, enum_File_Path_Index)->text().trimmed();
        struct_GLOBAL_witness_info_source st_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_name);
        QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + src_cnt_name + "/file_system.sqlite";

        emit signal_decompressed_files_view(st_source_info.source_name,QString(MACRO_CASE_TREE_Source),file_display_path,record_no, db_path_filesystem, src_cnt_name);

    }
    else
    {
        for(int i = 0 ; i < act_list.size() ; i++)
        {
            if(act_list.at(i)->text().trimmed() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
                continue;
            else
                act_list.at(i)->setDisabled(true);
        }

        m_tablewidget_obj->item(row,column)->setSelected(true);
    }
}

void filetypes_loader::slot_right_click_main_menu_triggered(QAction* current_clicked_action)
{ // right click menu actions
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
    {
        action_bookmark_triggered();
    }
    //    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Bookmark_all))
    //    {
    //        action_bookmark_all_triggered();
    //    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_remove_bookmark_triggered();
    }
    //    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_All_Bookmarks))
    //    {
    //        action_remove_bookmark_all_triggered();
    //    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Note))
    {
        action_add_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Notes_to_Bookmarks))
    {
        //   action_add_note_to_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        action_remove_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks))
    {
        // action_remove_note_bookmark_triggered();
    }
    //    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_all_Notes))
    //    {
    //        action_remove_all_notes_triggered();
    //    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Export))
    {
        action_export_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Go_To_Source))
    {
        action_goto_source_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_In_Window))
    {
        action_open_full_detailed_info_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    {
        action_copy_to_clipboard();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Seen))
    {
        action_mark_as_seen_unseen(QString(MACRO_Generic_Right_Click_Mark_As_Seen));
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Unseen))
    {
        action_mark_as_seen_unseen(QString(MACRO_Generic_Right_Click_Mark_As_Unseen));
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Export_as_KML))
    {
        action_export_as_kml_clicked();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue))
    {
        action_text_indexing_queue_clicked();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Decompress_File))
    {
        action_extract_compressed_file();
    }



}


void filetypes_loader::slot_link_label_searched_filters(QString value)
{ // show/hide the search filters from header
    if(value == "Show")
    {
        label_text_edit_show_link->setText("<a href = "+ QString("Hide") + " > " + QString("Searched Filters") + " </a>");
        text_edit_search_filters->show();
    }
    else if(value == "Hide")
    {
        label_text_edit_show_link->setText("<a href = "+ QString("Show") + " > " + QString("Searched Filters") + " </a>");
        text_edit_search_filters->hide();
    }
}

void filetypes_loader::slot_link_label_apple_metadata_searched_filters(QString value)
{ // show/hide the meradata searched filters
    if(value == "Show")
    {
        label_searched_apple_metadata_filters_show_link->setText("<a href = "+ QString("Hide") + " > " + QString("Searched Filters") + " </a>");
        text_edit_search_apple_metadata_filters->show();
    }
    else if(value == "Hide")
    {
        label_searched_apple_metadata_filters_show_link->setText("<a href = "+ QString("Show") + " > " + QString("Searched Filters") + " </a>");
        text_edit_search_apple_metadata_filters->hide();
    }
}


void filetypes_loader::slot_hide_loading_display_dialog_box(bool cancel_status)
{ // hide loading dialog function
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();

    export_rsync_obj->pub_cancel_rsync_copy(Q_FUNC_INFO);

    thread_zip_file_export_obj->pub_cancel_export();
}

//same right click actions defined below like adding/removing bookmaks,tags,notes and other right click features
void filetypes_loader::action_bookmark_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(true);


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString , 0);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

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

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        source_count_name_list << source_count_name_str;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");

        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_bookmark_item_list.append(obj);

        QString Int_str = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display ,source_count_name_str, Int_str);

    }

    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    global_connection_manager_obj->pub_add_bookmark_for_file_system(list_st_bookmark_item_list);

    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_bookmark_item_list.size(); num++)
        {
            if(list_st_bookmark_item_list.at(num).source_count_name == source_count_name_str)
                INT_list.append(list_st_bookmark_item_list.at(num).record_no);
        }
        global_connection_manager_obj->pub_set_bookmark_in_fs_source_db(INT_list, source_count_name_str);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_remove_bookmark_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(false);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString , 0);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

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

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        source_count_name_list << source_count_name_str;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");

        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_bookmark_item_list.append(obj);


        QString Int_str = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        recon_helper_standard_obj->pub_remove_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display ,source_count_name_str, Int_str);

    }

    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }



    global_connection_manager_obj->pub_delete_bookmark_for_file_system(list_st_bookmark_item_list);


    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_bookmark_item_list.size(); num++)
        {
            if(list_st_bookmark_item_list.at(num).source_count_name == source_count_name_str)
                INT_list.append(list_st_bookmark_item_list.at(num).record_no);
        }
        global_connection_manager_obj->pub_remove_bookmark_in_fs_source_db(INT_list, source_count_name_str);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_bookmark_all_triggered()
{
    // update_bookmark_status_for_all(QString("1"));
}

void filetypes_loader::action_remove_bookmark_all_triggered()
{
    //  update_bookmark_status_for_all(QString("0"));
}

void filetypes_loader::update_bookmark_status_for_all(QString status)
{

    //    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    //    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    //    {
    //        if(row % 100 == 0)
    //            QCoreApplication::processEvents();


    //        QString record_no_str = get_record_number_from_selected_row(row);

    //        QString source_count_name = get_source_count_name_from_selected_row(row);
    //        QString fs_db_path = case_narad_muni->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    //        struct_GLOBAL_derived_source_info struct_info = global_target_m_sources_info_obj->pub_get_target_info_structure_from_list(source_count_name);

    //        QString os_name_str = struct_info.os_naming_scheme;


    //        QString connection_naam = QString(Q_FUNC_INFO);
    //        QSqlDatabase::removeDatabase(connection_naam);
    //        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //        destination_db.setDatabaseName(fs_db_path);

    //        if(!destination_db.open())
    //        {
    //            recon_static_functions::app_debug(" - Destination Db opening ----FAILED---- " + fs_db_path, Q_FUNC_INFO);
    //            recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
    //            continue;
    //        }

    //        int bookmark_index = display_column_data_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_BOOKMARK,0);
    //        QString bookmark_col_name = db_columns_list.at(bookmark_index);

    //        if(status == QString("1"))
    //            m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
    //        else
    //            m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);


    //        QString fs_record_INT = record_no_str;

    //        QString command = QString("UPDATE '") + QString("files") + QString("' SET ") + bookmark_col_name + QString(" = ?") + QString(" WHERE INT = ?");
    //        QStringList arg_list;
    //        arg_list  << status <<  fs_record_INT;

    //        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);

    //        destination_db.close();


    //        QStringList applied_tags_on_this_record = get_applied_tags_name_on_current_record(fs_record_INT , fs_db_path);



    //    }



    //    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_add_note_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    //  QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    m_tablewidget_obj->blockSignals(true);

    //    for(int i = 0; i < selection_model_list.size(); i++)
    //    {
    //        if(i % 10 == 0)
    //            QCoreApplication::processEvents();

    //        QModelIndex model_index = selection_model_list.at(i);

    //        int selected_row_no = model_index.row();

    //        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
    //            continue;

    //        QString record_no_str = get_record_number_from_selected_row(selected_row_no);

    //        QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
    //        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");



    //        QString command = QString("select notes from '") + QString("files'") + QString(" WHERE INT = ?");
    //        QStringList arg_list;
    //        arg_list << record_no_str ;
    //        QString already_added_note = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command,arg_list,0,fs_db_path,Q_FUNC_INFO);

    //        add_notes_to_singal_record_obj->set_notes(already_added_note);
    //        add_notes_to_singal_record_obj->show();

    //    }

    add_notes_to_singal_record_obj->set_notes("");
    add_notes_to_singal_record_obj->show();

    m_tablewidget_obj->blockSignals(false);


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_remove_note_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();



    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;


    QList<struct_global_tagging_tag_notes_db_updation> list_st_notes_item_list;

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

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        source_count_name_list << source_count_name_str;

        QIcon icon("");
        m_tablewidget_obj->item(selected_row_no,enum_Notes_Icon_Index)->setIcon(icon);
        m_tablewidget_obj->item(selected_row_no,enum_Notes_Value_Index)->setText("");



        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_notes_item_list.append(obj);

    }


    if(list_st_notes_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }



    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_notes_item_list.size(); num++)
        {
            if(list_st_notes_item_list.at(num).source_count_name == source_count_name_str)
            {
                INT_list.append(list_st_notes_item_list.at(num).record_no);
            }
        }

        global_connection_manager_obj->pub_remove_notes_in_fs_source_db(INT_list, source_count_name_str);
    }

    global_connection_manager_obj->pub_delete_notes_for_file_system(list_st_notes_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_add_notes_pushbutton_clicked(QString note_value)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    add_notes_to_singal_record_obj->hide();

    note_value = note_value.trimmed();


    if(note_value.isEmpty())
        return;

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        action_carved_index_files_add_note_clicked(note_value);
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;


    QList<struct_global_tagging_tag_notes_db_updation> list_st_notes_item_list;

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

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        source_count_name_list << source_count_name_str;

        QIcon icon("../icons/notes.png");
        m_tablewidget_obj->item(selected_row_no,enum_Notes_Icon_Index)->setIcon(icon);
        m_tablewidget_obj->item(selected_row_no,enum_Notes_Value_Index)->setText(note_value);



        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_notes_item_list.append(obj);

    }


    if(list_st_notes_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_notes_item_list.size(); num++)
        {
            if(list_st_notes_item_list.at(num).source_count_name == source_count_name_str)
            {
                INT_list.append(list_st_notes_item_list.at(num).record_no);
            }
        }

        global_connection_manager_obj->pub_set_notes_in_fs_source_db(INT_list, source_count_name_str, note_value);
    }

    global_connection_manager_obj->pub_add_notes_for_file_system(list_st_notes_item_list, note_value);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


void filetypes_loader::update_notes_value(QString text, int current_row)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    text = text.trimmed();



    QString record_no_str = get_record_number_from_selected_row(current_row);

    QString source_count_name = get_source_count_name_from_selected_row(current_row);
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" -Destination Db opening ----FAILED---- " + fs_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }



    //    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES,0);

    QTableWidgetItem *item_tablewidget_bookmark = m_tablewidget_obj->item(current_row,enum_Notes_Icon_Index);

    if(text.isEmpty())
    {
        QIcon iconNotes("");
        item_tablewidget_bookmark->setIcon(iconNotes);
    }
    else
    {
        QIcon iconNotes("../icons/notes.png");
        item_tablewidget_bookmark->setIcon(iconNotes);
    }



    QString fs_record_INT = record_no_str;

    QString command = QString("UPDATE '") + QString("files") + QString("' SET notes = ? ") +  QString(" WHERE INT = ?");
    QStringList arg_list;
    arg_list  << text <<  fs_record_INT;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);

    destination_db.close();



    QStringList applied_tags_on_this_record = get_applied_tags_name_on_current_record(record_no_str , fs_db_path);

    emit signal_update_tag_search_for_notes(record_no_str, QString(MACRO_Plugin_Name_File_System),  QString(MACRO_Plugin_File_System_TAB_Name_Files), text, applied_tags_on_this_record, struct_info.os_scheme_display, MACRO_CASE_TREE_Source);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_add_note_to_bookmark_triggered()
{
    add_notes_to_bookmarks_obj->show();
}

void filetypes_loader::slot_add_notes_to_bookmark_pushbutton_clicked(QString text)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    add_notes_to_bookmarks_obj->hide();
    text = text.trimmed();

    update_notes_value_in_bookmark_for_filetypes(text , false);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_remove_note_bookmark_triggered()
{
    update_notes_value_in_bookmark_for_filetypes(QString("") , true);
}


void filetypes_loader::action_remove_all_notes_triggered()
{
    //    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    //    int bookmark_index = display_column_data_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_BOOKMARK,0);

    //    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    //    {
    //        if(row % 100 == 0)
    //            QCoreApplication::processEvents();


    //        QString fs_record_INT = get_record_number_from_selected_row(row);

    //        QString source_count_name = get_source_count_name_from_selected_row(row);
    //        QString fs_db_path = case_narad_muni->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    //        struct_GLOBAL_derived_source_info struct_info = global_target_m_sources_info_obj->pub_get_target_info_structure_from_list(source_count_name);


    //        QString result_dir = case_narad_muni->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    //        QString connection_naam = QString(Q_FUNC_INFO);
    //        QSqlDatabase::removeDatabase(connection_naam);
    //        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //        destination_db.setDatabaseName(fs_db_path);

    //        if(!destination_db.open())
    //        {
    //            recon_static_functions::app_debug(" -Destination Db opening ----FAILED---- " + fs_db_path, Q_FUNC_INFO);
    //            recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
    //            continue;
    //        }

    //        QString command = QString("UPDATE '") + QString("files") + QString("' SET notes = ? WHERE INT = ?");
    //        QStringList arg_list;
    //        arg_list  << QString("") << fs_record_INT;

    //        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);

    //        destination_db.close();


    //        QTableWidgetItem *item_tablewidget_bookmark = m_tablewidget_obj->item(row,bookmark_index);

    //        QIcon iconNotes("");
    //        item_tablewidget_bookmark->setIcon(iconNotes);


    //        QStringList applied_tags_on_this_record = get_applied_tags_name_on_current_record(fs_record_INT , fs_db_path);


    //    }



    //    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}
void filetypes_loader::action_goto_source_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    qint64 current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString record_no_str = get_record_number_from_selected_row(current_row);

    QString source_count_name = get_source_count_name_from_selected_row(current_row);

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_dsply = struct_info.os_scheme_display;


    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QString command = QString("Select filepath from'") + QString("files") + QString("' where INT = ?");

    QStringList arg_list;
    arg_list  << record_no_str;

    QString filepath =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,fs_db_path,Q_FUNC_INFO);

    QString fs_source_name = struct_info.source_name;
    emit signal_tablewidget_goto_record(fs_source_name,QString(MACRO_CASE_TREE_Source),filepath,record_no_str , fs_db_path, os_schme_dsply);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


void filetypes_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->show();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return;

}

void filetypes_loader::slot_open_with_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_open_with_plist_viewer_triggered();

        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
            action_open_with_plist_viewer_carved_index_files_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW
                || main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        {
            action_open_with_hex_viewer_triggered();
        }

        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
            action_open_with_hex_viewer_carved_index_files_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_open_with_sqlite_viewer_triggered();

        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
            action_open_with_sqlite_viewer_carved_index_files_triggered();

    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_open_with_registry_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
    {
        add_applications_to_open_application_menu();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_open_with_log_viewer_triggered();
    }
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_sendtobucket_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_sendToBucket_plist_viewer_triggered();

        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
            action_sendToBucket_plist_viewer_carved_index_files_triggered();

    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW
                || main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        {
            action_sendToBucket_hex_viewer_triggered();
        }

        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
            action_sendToBucket_hex_viewer_carved_index_files_triggered();

    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_sendToBucket_sqlite_viewer_triggered();

        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
            action_sendToBucket_sqlite_viewer_carved_index_files_triggered();

    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_sendToBucket_registry_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
            action_sendToBucket_log_viewer_triggered();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_add_files_to_hashset_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == MACRO_Generic_Right_Click_Create_Hashset)
    {
        return;
    }

    QString dbpath = current_clicked_action->toolTip();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QString tab_name_str, table_name_str;
        QStringList all_path_list;
        QString parent_name;

        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

        QString bookmark = QString("0");
        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            bookmark = QString("1");



        QString record_no_str = get_record_number_from_selected_row(selected_row_no);

        QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

        all_path_list << complete_path;

        tab_name_str   = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        table_name_str = QString("files");
        parent_name    = QString(MACRO_Plugin_Name_File_System);


        for(int j = 0; j < all_path_list.size(); j++)
        {
            QString complete_path = all_path_list.at(j);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
                message_dialog_obj->show();
                continue;
            }

            if(info.isDir())
                continue;

            hashset_storage_obj->insert_file_details_into_hashset_database(complete_path,dbpath, source_count_name);
        }
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void filetypes_loader::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    hashset_storage_obj->hide();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    ///Adding Menu's Action
    for(int i = 0; i < list_dbs_addmenu.size() ; i++)
    {
        QStringList list = list_dbs_addmenu.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
        if(list.size() != 2)
            continue;

        QString db_name = list.at(0);
        QString db_path = list.at(1);


        if(callfrom_str == MACRO_Hashset_New_Hash_Created)
        {
            QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

            for(int i = 0; i < selection_model_list.size(); i++)
            {
                QString tab_name_str, table_name_str;
                QStringList all_path_list;
                QString parent_name;

                if(i % 100 == 0)
                    QCoreApplication::processEvents();

                QModelIndex model_index = selection_model_list.at(i);

                int selected_row_no = model_index.row();

                if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
                    continue;

                if(m_tablewidget_obj->isRowHidden(selected_row_no))
                    continue;

                QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

                QString bookmark = QString("0");
                if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
                    bookmark = QString("1");


                QString record_no_str = get_record_number_from_selected_row(selected_row_no);

                QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

                QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

                all_path_list << complete_path;

                tab_name_str   = QString(MACRO_Plugin_File_System_TAB_Name_Files);
                table_name_str = QString("files");
                parent_name    = QString(MACRO_Plugin_Name_File_System);


                for(int j = 0; j < all_path_list.size(); j++)
                {
                    QString complete_path = all_path_list.at(j);

                    QFileInfo info(complete_path);
                    if(!info.exists())
                    {
                        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
                        message_dialog_obj->show();
                        continue;
                    }

                    if(info.isDir())
                        continue;

                    hashset_storage_obj->insert_file_details_into_hashset_database(complete_path,db_path,source_count_name);

                }
            }

            emit signal_update_hashset_submenu_from_tablewidget_loader(db_name,db_path);
        }
    }

    hashset_storage_obj->hide();
    recon_static_functions::app_debug(" -Endds " , Q_FUNC_INFO);
}

void filetypes_loader::slot_act_create_hashset()
{
    hashset_storage_obj->show();
}


void filetypes_loader::slot_add_tags(QString tag_data , QString colour)
{
    update_tags_value(tag_data,colour);
}

void filetypes_loader::slot_action_search_file_with_same_hash_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();
    if(current_row < 0)
        return;

    QString run_status_str = recon_helper_standard_obj->pub_get_filesystem_modules_thread_run_status(QString(MACRO_JobType_Hashes), Q_FUNC_INFO);
    if(run_status_str.isEmpty() || run_status_str == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No source has been processed for 'Hashes' yet.");
        return;
    }

    QString source_count_name = get_source_count_name_from_selected_row(current_row);
    struct_GLOBAL_witness_info_source st_globl_recon_file_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QString record_no = get_record_number_from_selected_row(current_row);

    QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

    QString hash_md5 = m_tablewidget_obj->item(current_row,enum_Hash_MD5_Index)->text().trimmed();

    if(hash_md5.isEmpty())
    {
        /// get hashes from db
        QString command = QString("Select hash_md5 from files where INT = ?");
        QStringList vals_list;
        vals_list << record_no;

        hash_md5 = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, vals_list, 0, fs_db_path, Q_FUNC_INFO);
    }


    search_files_with_hash_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
    search_files_with_hash_obj->pub_refresh_tableWidget();
    search_files_with_hash_obj->pub_set_display_loading_progress_bar_obj(display_loading_progress_bar_non_cancelable_obj);
    search_files_with_hash_obj->pub_search_files_having_same_hash(hash_md5, record_no, source_count_name, complete_file_path);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_remaining_tags_list(QStringList list_tags_remained, QString table_name, QString record)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString record_no       = m_tablewidget_obj->item(current_row, record_no_index)->text();
    QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

    QString source_count_name = get_source_count_name_from_selected_row(current_row);
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString os_schme_dsply = struct_info.os_scheme_display;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");


    QString tags_str = list_tags_remained.join(",");

    QString command = QString("update '") + QString("files") + QString("' set recon_tag_value=? where ") + QString(" INT ") + QString("=? ");
    QStringList arg_list;
    arg_list << tags_str << record_no ;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);

    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);

    if(tags_str == "")
    {
        QIcon icon(QString(""));
        m_tablewidget_obj->item(current_row,tag_position)->setIcon(icon);
    }
    else if(tags_str.contains(","))
    {
        QString icon_path = QString("../icons/tag_icons/multiple.png");
        QIcon icon(icon_path);
        m_tablewidget_obj->item(current_row,tag_position)->setIcon(icon);
    }
    else if((tags_str != "") && (!tags_str.contains(",")))
    {
        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags_str,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
        color = color.toLower();
        color = color.replace(" ","_");
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(color, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(current_row,tag_position)->setIcon(icon);
    }


    QString tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);

    emit signal_update_tags_to_other_db(plugin_name_str,tab_name_str,record_no,list_tags_remained,os_schme_dsply);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_tag_list_to_be_removed(QStringList list_removed_tags, QString table_name, QString record_no)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString source_count_name = get_source_count_name_from_selected_row(current_row);
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_dsply = struct_info.os_scheme_display;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);
    QString tab_name_str    = QString(MACRO_Plugin_File_System_TAB_Name_Files);

    emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record_no,list_removed_tags,os_schme_dsply);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void filetypes_loader::slot_act_remove_tags()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();



    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;


    QList<struct_global_tagging_tag_notes_db_updation> list_st_tag_item_list;

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

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        source_count_name_list << source_count_name_str;

        QIcon icon("");
        m_tablewidget_obj->item(selected_row_no,enum_Tag_Icon_Index)->setIcon(icon);



        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_tag_item_list.append(obj);

    }

    if(list_st_tag_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_tag_item_list.size(); num++)
        {
            if(list_st_tag_item_list.at(num).source_count_name == source_count_name_str)
            {
                INT_list.append(list_st_tag_item_list.at(num).record_no);
            }
        }

        global_connection_manager_obj->pub_remove_tag_in_fs_source_db(INT_list, source_count_name_str);
    }

    global_connection_manager_obj->pub_delete_tag_for_file_system(list_st_tag_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}



void filetypes_loader::slot_open_detailed_info_submenu_clicked(QAction *current_clicked_action)
{
}



void filetypes_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        action_pushbutton_search_carved_index_files_clicked();
        return;
    }

    QList<QAction *> menu_actions_list = menu_search_filters->actions();
    if(menu_actions_list.isEmpty())
        return;

    if(lineEdit_search->text().trimmed() == QString(""))
        return;

    QString keyword = lineEdit_search->text().trimmed();
    QStringList search_string_list = keyword.split(",", Qt::SkipEmptyParts);


    st_gallery_view_file_info_list.clear();

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();



    QCoreApplication::processEvents();
    /// hide every row first
    //    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    //    {
    //        m_tablewidget_obj->hideRow(row);
    //    }




    QList<int> columns_list_to_search;
    QStringList column_name_list_to_search;

    for(int i = 0; i < menu_actions_list.size(); i++)
    {
        if(menu_actions_list.at(i)->isChecked())
        {
            column_name_list_to_search.append(menu_actions_list.at(i)->text());
        }
    }

    for(int i_col = 0; i_col < column_name_list_to_search.size(); i_col++)
    {

        for(int m_col_count = 0; m_tablewidget_obj->columnCount(); m_col_count++)
        {
            if(m_tablewidget_obj->horizontalHeaderItem(m_col_count)->text() == column_name_list_to_search.at(i_col))
            {
                columns_list_to_search.append(m_col_count);
                break;
            }
        }

    }


    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        bool bool_search_item_matched = false;

        if(row % 500 == 0)
            QCoreApplication::processEvents();

        for(int col_count = 0 ; col_count < columns_list_to_search.size(); col_count++)
        {
            QString column_data = m_tablewidget_obj->item(row, columns_list_to_search.at(col_count))->text().trimmed();

            for(int serach_item_index = 0; serach_item_index < search_string_list.size(); serach_item_index++)
            {
                keyword = search_string_list.at(serach_item_index);

                if(column_data.contains(keyword, Qt::CaseInsensitive))
                {
                    bool_search_item_matched = true;
                    break;
                }


            }
            if(bool_search_item_matched)
                break;

        }
        if(bool_search_item_matched)
        {
            m_tablewidget_obj->showRow(row);
            QTableWidgetItem *item_filename = m_tablewidget_obj->item(row, enum_File_Name_Index);

            QString file_size = m_tablewidget_obj->item(row, enum_File_Size_Index)->text();


            if(recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(item_filename->text()))
            {
                QTableWidgetItem *item_source_count = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index);
                QTableWidgetItem *item_filepath = m_tablewidget_obj->item(row, enum_File_Path_Index);
                QTableWidgetItem *item_INT = m_tablewidget_obj->item(row, enum_Record_No_Index);
                QTableWidgetItem *item_bk = m_tablewidget_obj->item(row, enum_Bookmarks_Index);
                QTableWidgetItem *exif_latitu = m_tablewidget_obj->item(row, enum_Exif_Latitude_Index);
                QTableWidgetItem *exif_longit = m_tablewidget_obj->item(row, enum_Exif_Longitude_Index);
                QTableWidgetItem *item_date_accessed = m_tablewidget_obj->item(row, enum_Date_Accessed_Index);

                struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item_source_count->text());
                QString tmp_path = item_filepath->text();
                QString file_full_path;
                QString thumbnail_file_full_path;
                bool bool_is_it_file = false;

                if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                        || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                {
                    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                            + struct_info.source_count_name + "/file_system.sqlite";

                    QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(tmp_path), 0, fs_db_path, Q_FUNC_INFO);
                    if(!idpath.isEmpty())
                    {
                        file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, idpath, Q_FUNC_INFO);
                        bool_is_it_file = true;
                    }
                }
                else
                {
                    file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, tmp_path, Q_FUNC_INFO);
                    bool_is_it_file = QFileInfo(file_full_path).isFile();
                }

                if(!bool_is_it_file)
                    continue;

                QString file_name = item_filename->text();
                QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + struct_info.source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

                QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";
                QString fs_record_no = item_INT->text();

                QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
                QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

                if(!thumb_file_path.isEmpty())
                {
                    thumbnail_file_full_path = result_filepath + thumb_file_path;
                }
                else
                {
                    file_name.remove(file_name.lastIndexOf("."),file_name.size());
                    video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

                    //                    QString script_path = "../Resources/scripts_persistent/video_frame.py";
                    //                    QString cmd_python_path = global_varibale_python_path;

                    //                    QStringList args;
                    //                    args << script_path << file_full_path << video_thumbnails_path << file_size;
                    //

                    QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                    QStringList args;
                    args << file_full_path << video_thumbnails_path << file_size;

                    QProcess process_thumbnails;
                    //                    process_thumbnails.start(cmd_python_path,args);
                    process_thumbnails.start(video_frame_exe_path,args);
                    if(!process_thumbnails.waitForStarted())
                    {
                        recon_static_functions::app_debug("process_thumbnails -----FAILED----- ",Q_FUNC_INFO);
                        recon_static_functions::app_debug("Error :- " + process_thumbnails.errorString() ,Q_FUNC_INFO);
                        continue;
                    }

                    bool_process_thumbnails_finished = false;
                    connect(&process_thumbnails, SIGNAL(finished(int)), this, SLOT(slot_process_video_thumbnails_finished(int)));

                    int count;
                    while(1)
                    {
                        count++;
                        if(count % 100 == 0)
                        {
                            QCoreApplication::processEvents();
                            count = 0;
                        }

                        if(bool_process_thumbnails_finished || process_thumbnails.state() == QProcess::NotRunning)
                        {
                            process_thumbnails.close();
                            break;
                        }
                    }

                    thumbnail_file_full_path = QFileInfo(video_thumbnails_path).absoluteFilePath();
                    QString thumb_filepath;
                    QStringList list;
                    QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                    QFileInfo thumbnail_file_existence(thumbnail_file_full_path);
                    if(thumbnail_file_existence.exists())
                    {
                        thumb_filepath.clear();
                        thumb_filepath = thumbnail_file_full_path;
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                    }
                    else
                    {
                        thumb_filepath.clear();
                        QFile tmp_file("../Resources/features_resources/video_thumbnail/default_video_thumbnail.png");
                        tmp_file.copy(video_thumbnails_path);
                        thumb_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                    }
                }

                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();
                f1.file_path = item_filepath->text();
                f1.INT = item_INT->text();
                f1.table_view_row_no = row;
                f1.thumbnail_file_path = thumbnail_file_full_path;
                f1.full_filepath = file_full_path;
                f1.bookmark = "0";
                f1.latitude = exif_latitu->text();
                f1.longitude = exif_longit->text();
                f1.source_count_name = item_source_count->text();
                f1.last_access_timestamp = item_date_accessed->text();
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);

            }
            else if (recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
            {
                QTableWidgetItem *item_source_count = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index);
                QTableWidgetItem *item_filepath = m_tablewidget_obj->item(row, enum_File_Path_Index);
                QTableWidgetItem *item_INT = m_tablewidget_obj->item(row, enum_Record_No_Index);
                QTableWidgetItem *item_bk = m_tablewidget_obj->item(row, enum_Bookmarks_Index);
                QTableWidgetItem *exif_latitu = m_tablewidget_obj->item(row, enum_Exif_Latitude_Index);
                QTableWidgetItem *exif_longit = m_tablewidget_obj->item(row, enum_Exif_Longitude_Index);
                QTableWidgetItem *item_date_accessed = m_tablewidget_obj->item(row, enum_Date_Accessed_Index);

                struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item_source_count->text());
                QString tmp_path = item_filepath->text();
                QString file_full_path;
                bool bool_is_it_file = false;

                if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                        || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                {
                    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                            + struct_info.source_count_name + "/file_system.sqlite";

                    QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(tmp_path), 0, fs_db_path, Q_FUNC_INFO);
                    if(!idpath.isEmpty())
                    {
                        file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, idpath, Q_FUNC_INFO);
                        bool_is_it_file = true;
                    }
                }
                else
                {
                    file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, tmp_path, Q_FUNC_INFO);
                    bool_is_it_file = QFileInfo(file_full_path).isFile();
                }

                if(!bool_is_it_file)
                    continue;

                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();
                f1.file_path = item_filepath->text();
                f1.INT = item_INT->text();
                f1.table_view_row_no = row;
                f1.thumbnail_file_path = file_full_path;
                f1.full_filepath = file_full_path;
                f1.bookmark = "0";
                f1.latitude = exif_latitu->text();
                f1.longitude = exif_longit->text();
                f1.source_count_name = item_source_count->text();
                f1.last_access_timestamp = item_date_accessed->text();
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }

            //            else
        }
        else
            m_tablewidget_obj->hideRow(row);
    }


    bool_did_tableview_display_changed = true;

    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);

    emit signal_clear_all_metadata();

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        slot_main_tab_widget_current_changed(enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW);


    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_pushbutton_showall_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        total_covered_count = 0;
        m_table_carved_index_files->blockSignals(true);
        lineEdit_search->clear();
        m_table_carved_index_files->setRowCount(0);
        action_pushbutton_show_all_carved_index_files_clicked();
        m_table_carved_index_files->blockSignals(true);
        return;
    }

    m_tablewidget_obj->blockSignals(true);
    lineEdit_search->clear();
    m_tablewidget_obj->setRowCount(0);
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();
    QCoreApplication::processEvents();

    total_covered_count = 0;
    display_data();
    display_loading_progress_bar_obj->hide();

    m_tablewidget_obj->blockSignals(false);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);


    return;
    //===================
    st_gallery_view_file_info_list.clear();

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    lineEdit_search->clear();


    /// show every row first
    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {

        if(row % 500 == 0)
            QCoreApplication::processEvents();

        m_tablewidget_obj->showRow(row);

        QTableWidgetItem *item_filename = m_tablewidget_obj->item(row, enum_File_Name_Index);

        QString file_size = m_tablewidget_obj->item(row, enum_File_Size_Index)->text();

        if(recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(item_filename->text()))
        {
            QTableWidgetItem *item_source_count = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index);
            QTableWidgetItem *item_filepath = m_tablewidget_obj->item(row, enum_File_Path_Index);
            QTableWidgetItem *item_INT = m_tablewidget_obj->item(row, enum_Record_No_Index);
            QTableWidgetItem *item_bk = m_tablewidget_obj->item(row, enum_Bookmarks_Index);
            QTableWidgetItem *exif_latitu = m_tablewidget_obj->item(row, enum_Exif_Latitude_Index);
            QTableWidgetItem *exif_longit = m_tablewidget_obj->item(row, enum_Exif_Longitude_Index);

            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item_source_count->text());
            QString tmp_path = item_filepath->text();
            QString file_full_path;
            QString thumbnail_file_full_path;
            bool bool_is_it_file = false;

            if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                        + struct_info.source_count_name + "/file_system.sqlite";
                QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(tmp_path), 0, fs_db_path, Q_FUNC_INFO);
                if(!idpath.isEmpty())
                {
                    file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, idpath, Q_FUNC_INFO);
                    bool_is_it_file = true;
                }
            }
            else
            {
                file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, tmp_path, Q_FUNC_INFO);
                bool_is_it_file = QFileInfo(file_full_path).isFile();
            }

            if(!bool_is_it_file)
                continue;

            QString file_name = item_filename->text();

            QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + struct_info.source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

            QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";

            QString fs_record_no = item_INT->text();

            QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
            QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

            if(!thumb_file_path.isEmpty())
            {
                thumbnail_file_full_path = result_filepath + thumb_file_path;
            }
            else
            {
                file_name.remove(file_name.lastIndexOf("."),file_name.size());
                video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

                //                QString script_path = "../Resources/scripts_persistent/video_frame.py";
                //                QString cmd_python_path = global_varibale_python_path;

                //                QStringList args;
                //                args << script_path << file_full_path << video_thumbnails_path << file_size;

                QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                QStringList args;
                args << file_full_path << video_thumbnails_path << file_size;

                QProcess process_thumbnails;
                //                process_thumbnails.start(cmd_python_path,args);
                process_thumbnails.start(video_frame_exe_path,args);
                if(!process_thumbnails.waitForStarted())
                {
                    recon_static_functions::app_debug("process_thumbnails -----FAILED----- ",Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error:" + process_thumbnails.errorString() ,Q_FUNC_INFO);
                    continue;
                }

                bool_process_thumbnails_finished = false;
                connect(&process_thumbnails, SIGNAL(finished(int)), this, SLOT(slot_process_video_thumbnails_finished(int)));

                int count;
                while(1)
                {
                    count++;
                    if(count % 100 == 0)
                    {
                        QCoreApplication::processEvents();
                        count = 0;
                    }

                    if(bool_process_thumbnails_finished || process_thumbnails.state() == QProcess::NotRunning)
                    {
                        process_thumbnails.close();
                        break;
                    }
                }

                thumbnail_file_full_path = QFileInfo(video_thumbnails_path).absoluteFilePath();
                QString thumb_filepath;
                QStringList list;
                QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                QFileInfo thumbnail_file_existence(file_full_path);
                if(thumbnail_file_existence.exists())
                {
                    thumb_filepath.clear();
                    thumb_filepath = thumbnail_file_full_path;
                    thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                    if(!thumb_filepath.startsWith("/"))
                    {
                        thumb_filepath = thumb_filepath.prepend("/");
                    }

                    list.clear();
                    list << fs_record_no << thumb_filepath;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                }
                else
                {
                    thumb_filepath.clear();
                    QFile tmp_file("../Resources/features_resources/video_thumbnail/default_video_thumbnail.png");
                    tmp_file.copy(video_thumbnails_path);
                    thumb_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();
                    thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                    if(!thumb_filepath.startsWith("/"))
                    {
                        thumb_filepath = thumb_filepath.prepend("/");
                    }

                    list.clear();
                    list << fs_record_no << thumb_filepath;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                }
            }

            struct_global_gallery_view_file_info f1;
            f1.file_name = item_filename->text();
            f1.file_path = item_filepath->text();
            f1.INT = item_INT->text();
            f1.table_view_row_no = row;
            f1.thumbnail_file_path = thumbnail_file_full_path;
            f1.full_filepath = file_full_path;
            f1.bookmark = "0";
            f1.latitude = exif_latitu->text();
            f1.longitude = exif_longit->text();
            f1.source_count_name = item_source_count->text();
            f1.index = st_gallery_view_file_info_list.size();


            if(item_bk->checkState() == Qt::Checked)
                f1.bookmark = "1";

            st_gallery_view_file_info_list.append(f1);
        }
        else if(recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
        {
            QTableWidgetItem *item_source_count = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index);
            QTableWidgetItem *item_filepath = m_tablewidget_obj->item(row, enum_File_Path_Index);
            QTableWidgetItem *item_INT = m_tablewidget_obj->item(row, enum_Record_No_Index);
            QTableWidgetItem *item_bk = m_tablewidget_obj->item(row, enum_Bookmarks_Index);
            QTableWidgetItem *exif_latitu = m_tablewidget_obj->item(row, enum_Exif_Latitude_Index);
            QTableWidgetItem *exif_longit = m_tablewidget_obj->item(row, enum_Exif_Longitude_Index);

            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item_source_count->text());
            QString tmp_path = item_filepath->text();
            QString file_full_path;
            bool bool_is_it_file = false;

            if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                        + struct_info.source_count_name + "/file_system.sqlite";
                QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(tmp_path), 0, fs_db_path, Q_FUNC_INFO);
                if(!idpath.isEmpty())
                {
                    file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, idpath, Q_FUNC_INFO);
                    bool_is_it_file = true;
                }
            }
            else
            {
                file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, tmp_path, Q_FUNC_INFO);
                bool_is_it_file = QFileInfo(file_full_path).isFile();
            }

            if(!bool_is_it_file)
                continue;

            struct_global_gallery_view_file_info f1;
            f1.file_name = item_filename->text();
            f1.file_path = item_filepath->text();
            f1.INT = item_INT->text();
            f1.table_view_row_no = row;
            f1.thumbnail_file_path = file_full_path;
            f1.full_filepath = file_full_path;
            f1.bookmark = "0";
            f1.latitude = exif_latitu->text();
            f1.longitude = exif_longit->text();
            f1.source_count_name = item_source_count->text();
            f1.index = st_gallery_view_file_info_list.size();


            if(item_bk->checkState() == Qt::Checked)
                f1.bookmark = "1";

            st_gallery_view_file_info_list.append(f1);
        }
    }


    bool_did_tableview_display_changed = true;

    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);

    display_loading_progress_bar_obj->hide();



    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        slot_main_tab_widget_current_changed(enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_mark_as_seen_unseen(QString clicked_action_text)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");


        QString update_seen_str;
        QString command = QString("Update 'files' Set is_seen = ? where INT = ?");

        QStringList value_list;
        if(clicked_action_text == QString(MACRO_Generic_Right_Click_Mark_As_Unseen))
        {
            update_seen_str = QString::number(0);  // set as unseen
            value_list << update_seen_str << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list, fs_db_path, Q_FUNC_INFO);

            change_background_for_seen_unseen(selected_row_no , true); // set default black color for text

        }
        else
        {
            update_seen_str = QString::number(1);  // set as seen
            value_list << update_seen_str << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list, fs_db_path, Q_FUNC_INFO);

            change_background_for_seen_unseen(selected_row_no , false); // change text color
        }

        QString fs_record_INT = record_no;
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::change_background_for_seen_unseen(qint64 current_row , bool is_default_color)
{
    if(current_row < 0)
        return;

    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        if(m_tablewidget_obj->item(current_row , column) == NULL)
            continue;

        if(is_default_color)
            m_tablewidget_obj->item(current_row , column)->setForeground(Qt::black);
        else
            m_tablewidget_obj->item(current_row , column)->setForeground(Qt::blue);
    }

}

void filetypes_loader::change_textcolor_for_compression_decompression(qint64 current_row , bool bool_change_color)
{
    if(current_row < 0)
        return;

    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        if(m_tablewidget_obj->item(current_row , column) == NULL)
            continue;

        if(bool_change_color)
            m_tablewidget_obj->item(current_row , column)->setForeground(Qt::red);
        else
            m_tablewidget_obj->item(current_row , column)->setForeground(Qt::black);
    }
}

void filetypes_loader::action_export_as_kml_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList list_paths_not_available_for_kml;


    ///make list of files if file does not contain longitude and latitude.
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);
        int row = model_index.row();
        if(row < 0 || row > m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        QString display_file_path =  m_tablewidget_obj->item(row,enum_File_Path_Index)->text().trimmed();
        QString latitude;
        QString longitude;
        if(!display_file_path.trimmed().isEmpty())
        {
            QString source_count_name =   m_tablewidget_obj->item(row,enum_Source_Count_Name_Index)->text().trimmed();
            QString record_no =   m_tablewidget_obj->item(row,enum_Record_No_Index)->text().trimmed();

            QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + source_count_name + "/file_system.sqlite";

            QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path_filesystem,"files",source_count_name ,Q_FUNC_INFO);


            QStringList my_list;
            my_list << "Latitude";
            my_list << "Longitude";

            QMap<QString, QString> map_obj =  exif_data_image_files_obj->pub_extract_specific_attributes(complete_file_path, my_list);
            QMap <QString, QString>::iterator mapiter;
            for(mapiter = map_obj.begin(); mapiter != map_obj.end() ; mapiter++)
            {
                if(mapiter.key() == "Latitude")
                {
                    latitude = mapiter.value();
                }
                else if(mapiter.key() == "Longitude")
                {
                    longitude = mapiter.value();
                }
            }


            if(!display_file_path.startsWith("/"))
                display_file_path.prepend("/");

            if(latitude.trimmed().isEmpty() && longitude.trimmed().isEmpty())
                list_paths_not_available_for_kml << display_file_path;
        }

    }
    ///make list of files if file does not contain longitude and latitude.


    if(list_paths_not_available_for_kml.size() == 0)
    {
        QStringList list_paths_to_message_box = make_kml_report();
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, QString(""));
        message_dialog_obj->show();
    }
    else if(list_paths_not_available_for_kml.size() > 0)
    {
        export_as_KML_dialog_box_obj->pub_set_lable(list_paths_not_available_for_kml);
        export_as_KML_dialog_box_obj->show();
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_text_indexing_queue_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

    QString command = QString("insert into files (bookmark, fs_record,file_path,source_count_name,status,plugin_name) values(?,?,?,?,?,?)");

    QStringList value_list;
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

        QString record_no = m_tablewidget_obj->item(selected_row_no,enum_Record_No_Index)->text();
        QString file_path = m_tablewidget_obj->item(selected_row_no,enum_File_Path_Index)->text();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no,enum_Source_Count_Name_Index)->text();

        value_list.clear();
        value_list << QString::number(0) << record_no << file_path << source_count_name << QString::number(0) << QString(MACRO_Plugin_Name_File_System);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
    }


}

void filetypes_loader::slot_main_tab_widget_current_changed(int index)
{
    emit signal_clear_all_metadata();
    bool_cell_changed_connection_buildup_for_tablewidget = false;

    lineEdit_search->clear();

    QCoreApplication::processEvents();
    if(index == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW && bool_did_tableview_display_changed)
    {

        m_tablewidget_obj->clearSelection();
        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_search_filters->hide();
        pushbutton_show_all->hide();
        pushbutton_show_column->hide();

        gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);
        QCoreApplication::processEvents();
        bool_did_tableview_display_changed = false;

    }
    else if(index == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
    {
        m_tablewidget_obj->clearSelection();
        pushbutton_search_filters->setMenu(menu_search_filters);

        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_search_filters->hide();
        pushbutton_show_all->hide();
        pushbutton_show_column->show();

        //tablewidget_general_clicked(m_tablewidget_obj->currentRow() , m_tablewidget_obj->currentColumn());
    }
    else if(index == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW && !bool_did_tableview_display_changed)
    {

        m_tablewidget_obj->clearSelection();
        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_search_filters->hide();
        pushbutton_show_all->hide();
        pushbutton_show_column->hide();
    }
    else if(index == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        pushbutton_search_filters->setMenu(menu_carved_index_files_search_filters);

        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_search_filters->hide();
        pushbutton_show_all->hide();

        pushbutton_show_column->show();
    }

}

void filetypes_loader::slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1)
{
    slot_act_quick_look_triggered();
}

void filetypes_loader::slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    file_export_dialog_obj->show();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

}


void filetypes_loader::slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString record_no = f1.INT;

    if(record_no.isEmpty())
    {
        bool_cell_changed_connection_buildup_for_tablewidget = false;
        m_tablewidget_obj->clearSelection();
        emit signal_clear_all_metadata();
        return;
    }

    /// control goes to table view tab
    /// now all operations can be done on basis of m_tablewidget_obj

    int current_row_selection_int = f1.table_view_row_no;

    for(int pp = 0 ; pp < m_tablewidget_obj->rowCount() ; pp++)
    {
        if((f1.INT == m_tablewidget_obj->item(pp,enum_Record_No_Index)->text())
                && (f1.source_count_name == m_tablewidget_obj->item(pp,enum_Source_Count_Name_Index)->text()))
        {
            current_row_selection_int = pp;
            break;
        }
    }

    m_tablewidget_obj->selectRow(current_row_selection_int);

    if(!bool_cell_changed_connection_buildup_for_tablewidget)
        m_tablewidget_obj->cellClicked(current_row_selection_int,enum_Bookmarks_Index);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info f1)
{
    QString record_no =   f1.INT;
    if(record_no.isEmpty())
        return;

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(f1.source_count_name);
    QString file_full_path;

    if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        QString display_file_path = f1.file_path;
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + struct_info.source_count_name + "/file_system.sqlite";

        QString cache_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString).toString() + f1.source_count_name;
        QString unique_filename_str = f1.source_count_name + "_" + record_no + "_" + QFileInfo(display_file_path).fileName();

        QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(display_file_path), 0, fs_db_path, Q_FUNC_INFO);
        if(!idpath.isEmpty())
        {
            file_full_path = cache_dir_path + "/" + unique_filename_str;
        }
    }
    else
    {
        file_full_path = f1.full_filepath;
    }

    QStringList argss;
    argss << file_full_path;

    recon_helper_process_obj->run_command_with_arguments("open",argss,Q_FUNC_INFO);

}

void filetypes_loader::slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1)
{

    if(f1.INT.isEmpty())
    {
        m_tablewidget_obj->clearSelection();
        emit signal_clear_all_metadata();
        return;
    }

    /// control goes to table view tab
    /// now all operations can be done on basis of m_tablewidget_obj

    int current_row_selection_int = f1.table_view_row_no;
    for(int pp = 0 ; pp < m_tablewidget_obj->rowCount() ; pp++)
    {

        if((f1.INT == m_tablewidget_obj->item(pp,enum_Record_No_Index)->text())
                && (f1.source_count_name == m_tablewidget_obj->item(pp,enum_Source_Count_Name_Index)->text()))
        {
            current_row_selection_int = pp;
            break;
        }
    }


    m_tablewidget_obj->selectRow(current_row_selection_int);

    bool bool_chkstate = false;

    if(f1.bookmark == MACRO_TRUE_VALUE_NUMERIC_QString)
        bool_chkstate = true;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bool_chkstate)
    {
        m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),m_tablewidget_obj->currentColumn())->setCheckState(Qt::Checked);
        update_bookmark_value("1",m_tablewidget_obj->currentRow(),bookmark_index);
    }
    else
    {
        m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),m_tablewidget_obj->currentColumn())->setCheckState(Qt::Unchecked);
        update_bookmark_value("0",m_tablewidget_obj->currentRow(),bookmark_index);
    }

}

void filetypes_loader::slot_gallery_view_invalid_tem_clicked()
{
    m_tablewidget_obj->clearSelection();
    emit signal_clear_all_metadata();
}


void filetypes_loader::slot_table_widget_header_item_clicked(int column_index)
{ // tabelwidget header item clicked to get the value and index of columns
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    if(column_index < 0)
        return;


    if(st_gallery_view_file_info_list.size() <= 0)
        return;


    QList<struct_global_gallery_view_file_info>  sorted_gallery_info_qlist;

    QMap<int ,struct_global_gallery_view_file_info> index_row_qmap;
    index_row_qmap.clear();
    QList<int> row_no_QList;
    row_no_QList.clear();


    for(int pp = 0 ; pp < st_gallery_view_file_info_list.size() ; pp++)
    {
        struct_global_gallery_view_file_info f1 = st_gallery_view_file_info_list.at(pp);
        for(int qq = 0 ; qq < m_tablewidget_obj->rowCount() ; qq++)
        {

            if((f1.INT == m_tablewidget_obj->item(qq,enum_Record_No_Index)->text())
                    && (f1.source_count_name == m_tablewidget_obj->item(qq,enum_Source_Count_Name_Index)->text()))
            {
                int row_no = qq;

                if(!row_no_QList.contains(row_no))
                {
                    row_no_QList.append(row_no);
                    index_row_qmap.insert(row_no , f1);
                }

            }
        }

    }


    std::sort(row_no_QList.begin(),row_no_QList.end());

    for(int ii = 0 ; ii < row_no_QList.size() ; ii++)
    {
        sorted_gallery_info_qlist.append(index_row_qmap.value(row_no_QList.at(ii)));
    }


    st_gallery_view_file_info_list.clear();

    st_gallery_view_file_info_list = sorted_gallery_info_qlist;

    bool_did_tableview_display_changed = true;

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

}



void filetypes_loader::slot_main_table_widget_item_clicked(QTableWidgetItem * item)
{ // click on main tablewidget item or on any record to get the bookmakr value and update accordingly in bookmark database
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);
    if(item == NULL)
        return;

    if(item->row() < 0)
        return;

    if(item->column() != enum_Bookmarks_Index)
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

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

}

void filetypes_loader::slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path)
{ // export file from right click
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QStringList list_paths_to_message_box;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString dest_path; //   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();


        QString record_no_str = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();

        QString display_file_path =  m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        QFileInfo a_info(display_file_path);
        QString parent_dir = a_info.absolutePath();
        if(bool_keep_directory_structure)
        {
            dest_path = export_dir_path  + source_count_name + "/" + parent_dir + "/";
            QDir mdir;
            mdir.mkpath(dest_path);
        }
        else
        {
            dest_path = export_dir_path  + source_count_name + "/";
            QDir mdir;
            mdir.mkpath(dest_path);
        }

        QString m_file_name = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text();

        struct_global_export_file_system_record obj;
        obj.record_no = record_no_str;
        obj.source_count_name = source_count_name;
        obj.display_file_path = m_tablewidget_obj->item(selected_row_no,enum_File_Path_Index)->text();
        obj.displayed_file_name = m_file_name;


        QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj , Q_FUNC_INFO);
        QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_path,m_file_name , Q_FUNC_INFO);
        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;

    }


    display_loading_progress_bar_obj->hide();

    if(list_paths_to_message_box.size() > 0)
    {
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box , export_dir_path);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path,bool bool_is_enc_zip,QString password_enc_zip)
{ // export file or any record as a zip file
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    bool_zip_export_done = false;

    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QList<struct_global_file_for_zip_export> files_list_for_zip_export;

    QStringList files_which_do_not_exist;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        struct_global_file_for_zip_export st_file_info_for_export_obj;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        QString filename = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        QString display_file_path = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
        if(recon_helper_standard_obj->is_it_dir_record_no_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO))
        {
            QString cmd_2 = "select INT from files where file_parent_path LIKE ?";
            QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(QString(display_file_path  + "%")), 0, fs_db_path, Q_FUNC_INFO);
            for(int ii = 0 ; ii < INT_list.size() ; ii++)
            {
                QString relevant_record_no = INT_list.at(ii);
                QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(relevant_record_no,fs_db_path ,"files" ,source_count_name, Q_FUNC_INFO);
                QFileInfo m_file_info(file_path);
                if(!m_file_info.exists())
                {
                    continue ;
                }

                QString cmd = "select filepath from files where INT =" + relevant_record_no;
                QString display_file_path_2 = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd, 0, fs_db_path, Q_FUNC_INFO);

                st_file_info_for_export_obj.source_path = file_path;
                st_file_info_for_export_obj.destination_path = source_count_name + display_file_path_2;

                files_list_for_zip_export.append(st_file_info_for_export_obj);

            }
        }
        else
        {
            QFileInfo info(complete_path);
            if(!info.exists())
            {
                files_which_do_not_exist << filename;
                continue;
            }
            else
            {
                st_file_info_for_export_obj.source_path = complete_path;
                st_file_info_for_export_obj.destination_path = source_count_name + display_file_path;
                files_list_for_zip_export.append(st_file_info_for_export_obj);

            }
        }
    }



    if(files_which_do_not_exist.size() > 0)
    {

        display_loading_progress_bar_obj->hide();

        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr(""),
                                   files_which_do_not_exist.join("\n") + "\ndo not exist. Do you want to continue ?",
                                   QMessageBox::Ok | QMessageBox::Cancel);
        if (ret != QMessageBox::Ok)
        {
            return;
        }
    }

    zip_export_file_path = export_zip_file_path;

    display_loading_progress_bar_obj->show();


    thread_zip_file_export_obj->pub_set_essentials(export_zip_file_path, files_list_for_zip_export, bool_is_enc_zip, password_enc_zip);
    thread_zip_file_export_obj->start();
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_file_path)
{ // export file or any reocrd as a dmg file
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    bool_dmg_export_done = false;

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QString dest_path;

    dmg_export_file_path.clear();

    bool bool_is_image_created;

    QStringList list_paths_to_message_box,source_count_name_list;
    QString exported_file_path,parent_dir,tmp_folder_path,available_dmg_image;

    QString current_timestamp;
    current_timestamp = QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();

        QString display_file_path =  m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();

        QFileInfo a_info(display_file_path);
        parent_dir = a_info.absolutePath();
        if(bool_keep_directory_structure)
        {
            dest_path = export_dmg_file_path  + source_count_name + "_" + current_timestamp + "/" + parent_dir + "/";
            QDir mdir;
            mdir.mkpath(dest_path);
        }
        else
        {
            dest_path = export_dmg_file_path  + source_count_name + "_" + current_timestamp + "/";
            QDir mdir;
            mdir.mkpath(dest_path);
        }

        QString m_file_name = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text();
        struct_global_export_file_system_record obj;
        obj.record_no = record_no_str;
        obj.source_count_name = source_count_name;
        obj.display_file_path = m_tablewidget_obj->item(selected_row_no,enum_File_Path_Index)->text();
        obj.displayed_file_name = m_file_name;

        if(!source_count_name_list.contains(source_count_name))
        {
            source_count_name_list << source_count_name;
        }

        QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj , Q_FUNC_INFO);
        QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_path,m_file_name , Q_FUNC_INFO);

        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;
    }

    for( int ii = 0 ; ii < source_count_name_list.size() ; ii++)
    {
        if(bool_cancel_loading)
            break;

        bool_is_image_created = false;

        QString source_count_name = source_count_name_list.at(ii);

        QString folder_path = export_dmg_file_path  + source_count_name + "_" + current_timestamp;
        tmp_folder_path = folder_path;
        QString export_dmg_path = tmp_folder_path.append(".dmg");

        available_dmg_image  = recon_static_functions::get_available_filename(export_dmg_path,dest_path, Q_FUNC_INFO);
        dmg_export_file_path = available_dmg_image;

        QStringList args_list_id;
        args_list_id << "create" <<  available_dmg_image << "-fs" << "apfs" << "-srcfolder" << folder_path;

        if(!bool_cancel_loading)
        {

            QString list_data = recon_helper_process_obj->run_command_with_arguments("hdiutil", args_list_id, Q_FUNC_INFO);

            if(!list_data.contains("created:"))
            {
                message_dialog_obj->pub_set_message("File Export", MACRO_MSG_WARNING ,"Failed To Export Files");
                message_dialog_obj->show();
            }
            else
            {
                bool_is_image_created = true;
            }
        }

        QDir dir(folder_path);
        if(dir.exists())
        {
            dir.removeRecursively();
        }

        if(bool_cancel_loading && QFile(available_dmg_image).exists())
        {
            QFile file(available_dmg_image);
            if(file.exists())
            {
                file.remove();
            }
        }
    }

    display_loading_progress_bar_obj->hide();

    if(bool_is_image_created && QFile(available_dmg_image).exists())
    {
        bool_dmg_export_done = true;
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_thread_zip_file_export_finished()
{ // zip file exported successfully
    display_loading_progress_bar_obj->hide();

    if(bool_cancel_loading)
        return;

    bool_zip_export_done = true;
    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) exported to zip successfully. Do you want to open it?");
    message_dialog_obj->show();

}

void filetypes_loader::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{ // open or locate the exported directory path pop window appear, go to that directory path on yes clicked to see exported file
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;

    if(yes_no_clicked)
    {
        if(bool_zip_export_done)
        {
            bool_zip_export_done = false;
            point_to_export_dir(zip_export_file_path);
        }
        else if(bool_dmg_export_done)
        {
            bool_dmg_export_done = false;
            point_to_export_dir(dmg_export_file_path);
        }
    }
}

void filetypes_loader::point_to_export_dir(QString path)
{ // point to destination path where file exported
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void filetypes_loader::slot_sub_menu_open_file_with_other_app_clicked(QAction* current_clicked_action)
{ // open file with any other application

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text();

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

        QFileInfo info(file_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            continue;
        }

        recon_static_functions::open_file_with_specified_app(file_path , current_clicked_action->toolTip(), Q_FUNC_INFO);

        break;

    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}


void filetypes_loader::slot_table_header_customContextMenuRequested(const QPoint &points)
{
    return;
    menu_manage_column->exec(QCursor::pos());
}


void filetypes_loader::slot_action_manage_column_clicked(QAction* current_clicked_action)
{ // manage column clicked to manage the columns like which one is to hide or which one to show on table widget header
    current_clicked_action->setChecked(false);

    QString manage_column_db_path;
    manage_column_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString() + "manage_columns.sqlite";

    bool hidden_col_db_exist = false;

    QFile file(manage_column_db_path);
    if(file.exists())
    {
        hidden_col_db_exist = true;
    }

    if(m_tablewidget_obj != NULL)
    {
        for(int count = 0 ; count < m_tablewidget_obj->columnCount() ; count++)
        {
            if(((custom_table_header_widget *)custom_header_widgets_QList.at(count))->pub_get_column_name() == current_clicked_action->text())
            {
                if(m_tablewidget_obj->isColumnHidden(count) && current_clicked_action->isChecked() == false)
                {
                    m_tablewidget_obj->showColumn(count);
                    current_clicked_action->setChecked(true);

                    if(hidden_col_db_exist)
                    {
                        QString connection_naam = QString(Q_FUNC_INFO);
                        QSqlDatabase::removeDatabase(connection_naam);
                        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                        destination_db.setDatabaseName(manage_column_db_path);

                        if(!destination_db.open())
                        {
                            recon_static_functions::app_debug(" : plugin hidden columns Db opening ------FAILED------- ",Q_FUNC_INFO);
                            recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                            return;
                        }

                        QSqlQuery query(destination_db);
                        QString command;
                        command = QString("delete from hidden_columns where hidden_columns_nos=") + QString::number(count) + QString(" AND plugin_name='") + QString(MACRO_Plugin_Name_File_System) + QString("'") + QString(" AND tab_name='") + QString(MACRO_Plugin_File_System_TAB_Name_Files) + QString("'") + QString(" AND file_type_category='") + file_type_child + QString("'");

                        query.prepare(command);
                        if(!query.exec())
                        {
                            recon_static_functions::app_debug(" [slot_of_master_frame_invoked_from_manage_columns] query deletion ---FAILED---",Q_FUNC_INFO);
                            recon_static_functions::app_debug(" [slot_of_master_frame_invoked_from_manage_columns] error " + query.lastError().text(),Q_FUNC_INFO);
                        }
                        destination_db.close();
                    }
                }
                else
                {
                    m_tablewidget_obj->hideColumn(count);

                    if(hidden_col_db_exist)
                    {
                        QString connection_naam = QString(Q_FUNC_INFO);
                        QSqlDatabase::removeDatabase(connection_naam + "_1");
                        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + "_1");
                        destination_db.setDatabaseName(manage_column_db_path);

                        if(!destination_db.open())
                        {
                            recon_static_functions::app_debug(" : plugin hidden columns Db opening ------FAILED------- ",Q_FUNC_INFO);
                            recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                            return;
                        }

                        QSqlQuery query(destination_db);


                        query.prepare("insert into hidden_columns (plugin_name, tab_name, file_type_category, hidden_columns_nos, hidden_column_name) values(?,?,?,?,?)");

                        query.addBindValue(QString(MACRO_Plugin_Name_File_System));
                        query.addBindValue(QString(MACRO_Plugin_File_System_TAB_Name_Files));
                        query.addBindValue(file_type_child);
                        query.addBindValue(QString::number(count));
                        query.addBindValue(((custom_table_header_widget *)custom_header_widgets_QList.at(count))->pub_get_column_name());

                        if(!query.exec())
                        {
                            recon_static_functions::app_debug(" [slot_manage_column_clicked] query insertion ---FAILED---",Q_FUNC_INFO);
                            recon_static_functions::app_debug(" [slot_manage_column_clicked] error " + query.lastError().text(),Q_FUNC_INFO);
                        }
                        destination_db.close();
                    }
                }
            }
        }
    }
    clear_all_custom_header_widget_entries();
    pub_refresh_horizontal_header();
}

QList<filetypes_loader::struct_manage_column_info> filetypes_loader::get_hidden_column_from_manage_column_db()
{ // get hidden columns from database
    QList<struct_manage_column_info> hidden_col_lst;
    QString manage_column_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString() + "manage_columns.sqlite";

    QFile file(manage_column_db_path);
    if(file.exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(manage_column_db_path);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" : plugin hidden columns Db opening on load case tree------FAILED------- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
            return hidden_col_lst;
        }

        QSqlQuery query(destination_db);

        QString command;
        command = QString("select * from hidden_columns where tab_name=") + QString("'") + QString(MACRO_Plugin_File_System_TAB_Name_Files) + QString("'") + QString(" AND plugin_name=") + QString("'") + QString(MACRO_Plugin_Name_File_System) + QString("'") + QString(" AND file_type_category=") + QString("'") + file_type_child + QString("'");

        query.prepare(command);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query selection of hidden cols on load case tree ---FAILED---",Q_FUNC_INFO);
            recon_static_functions::app_debug("  error " + query.lastError().text(),Q_FUNC_INFO);
        }

        while(query.next()){

            struct_manage_column_info obj;

            obj.hid_col_no = query.value(2).toInt();
            obj.hid_col_name = query.value(3).toString();

            hidden_col_lst << obj;
        }

        destination_db.close();

    }
    return hidden_col_lst;
}

void filetypes_loader::pub_set_essentials()
{
    result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    hashset_display_obj->pub_set_essentials();
}

void filetypes_loader::pub_set_face_finder_obj(facefinder *obj)
{
    facefinder_obj = obj;

    facefinder_obj->pub_set_display_loading_progress_bar_obj(display_loading_progress_bar_obj);

    facefinder_obj->pub_set_essentials();
}

void filetypes_loader::slot_sub_menu_carve_clicked(QAction *current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Carve_Files))
    {
        action_carve_files_clicked();
    }
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Carve_Data))
    {
        action_carve_data_clicked();
    }
}

void filetypes_loader::action_carve_files_clicked()
{
    QString complete_file_path = get_current_complete_file_path();
    if(complete_file_path.trimmed().isEmpty())
        return;

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());

    emit signal_carve_files_clicked(source_count_name, complete_file_path, false);
}

void filetypes_loader::action_carve_data_clicked()
{
    QString complete_file_path = get_current_complete_file_path();
    if(complete_file_path.trimmed().isEmpty())
        return;

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),enum_Record_No_Index)->text();

    emit signal_carve_data_clicked(source_count_name, complete_file_path, QString(MACRO_Plugin_Name_File_System), QString(MACRO_Plugin_File_System_TAB_Name_Files), record_no);

}


void filetypes_loader::slot_cancel_export_as_KML_report()
{
    export_as_KML_dialog_box_obj->hide();
}

void filetypes_loader::slot_continue_export_as_KML_report()
{

    export_as_KML_dialog_box_obj->hide();

    QStringList list_paths_to_message_box = make_kml_report();

    if(list_paths_to_message_box.size() > 0)
    {
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, QString(""));
        message_dialog_obj->show();
    }

}

void filetypes_loader::slot_sub_menu_export_hashes_as_clicked(QAction* current_clicked_action)
{

    QString export_hashes_as = current_clicked_action->text().trimmed();
    QString vic_version;

    bool bool_any_hash_type_found = false;

    if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_1))
    {
        vic_version = QString("1.1");
        bool_any_hash_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_2))
    {
        vic_version = QString("1.2");
        bool_any_hash_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_3))
    {
        vic_version = QString("1.3");
        bool_any_hash_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_2_0))
    {
        vic_version = QString("2.0");
        bool_any_hash_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_SQLite))
    {
        bool_any_hash_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_CSV))
    {
        bool_any_hash_type_found = true;
    }
    else
        bool_any_hash_type_found = false;

    export_hashes_as = export_hashes_as.trimmed();

    if(export_hashes_as.isEmpty() || !bool_any_hash_type_found)
        return;

    if(export_hashes_as.contains(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_1)
            || export_hashes_as.contains(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_2)
            || export_hashes_as.contains(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_3))
    {
        QString destination_location;
        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                destination_location = selected_files.at(0);
            }
        }
        if(destination_location.trimmed() == QString(""))
            return;

        display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
        display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_non_cancelable_obj->show();

        QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

        export_as_hashes_details_list.clear();
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        for(int i = 0; i < selection_model_list.size(); i++)
        {
            if(i % 50 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(i);
            int selected_row_no = model_index.row();

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString fs_record_no       = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
            QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text();

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
            QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
            QString display_filepath   = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

            QFileInfo info_cmplt(complete_file_path);

            QFileInfo file_info(display_filepath);
            QString display_filename;
            display_filename = file_info.fileName();

            if(QFileInfo(complete_file_path).isDir())
            {
                QStringList value_list;
                QString val_str = display_filepath;
                val_str.append("%");
                value_list << val_str;

                ///Run only for files not processed
                QStringList record_no_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select INT From files Where file_parent_path like ?"), value_list, 0, fs_db_path, Q_FUNC_INFO);

                ///get all files/directories in selected directory
                for(int count = 0; count < record_no_list.size(); count++)
                {
                    if(count % 50 == 0)
                        QCoreApplication::processEvents();

                    fs_record_no       = record_no_list.at(count);
                    complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

                    ///get all files(only) in selected directory
                    if(QFileInfo(complete_file_path).isDir())
                        continue;

                    display_filepath   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select filepath From files Where INT = ?"), QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);

                    struct_global_export_as_hashes_details struct_export_vic_details_obj;

                    struct_export_vic_details_obj.fs_record_no       = fs_record_no;
                    struct_export_vic_details_obj.display_filepath   = display_filepath;
                    struct_export_vic_details_obj.complete_file_path = complete_file_path;
                    struct_export_vic_details_obj.source_count_name  = source_count_name;
                    struct_export_vic_details_obj.examiner_selected_file_or_dir_name = info_cmplt.fileName();

                    struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);
                    struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 1, fs_db_path, Q_FUNC_INFO);
                    struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 2, fs_db_path, Q_FUNC_INFO);
                    struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 3, fs_db_path, Q_FUNC_INFO);

                    export_as_hashes_details_list.append(struct_export_vic_details_obj);
                }
            }
            else
            {
                struct_global_export_as_hashes_details struct_export_vic_details_obj;

                struct_export_vic_details_obj.fs_record_no       = fs_record_no;
                struct_export_vic_details_obj.display_filepath   = display_filepath;
                struct_export_vic_details_obj.display_filename   = display_filename;
                struct_export_vic_details_obj.complete_file_path = complete_file_path;
                struct_export_vic_details_obj.source_count_name  = source_count_name;
                struct_export_vic_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

                struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);
                struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 1, fs_db_path, Q_FUNC_INFO);
                struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 2, fs_db_path, Q_FUNC_INFO);
                struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 3, fs_db_path, Q_FUNC_INFO);

                export_as_hashes_details_list.append(struct_export_vic_details_obj);
            }
        }

        display_loading_progress_bar_non_cancelable_obj->hide();

        export_hashes_as_vic_obj->pub_set_vic_version(vic_version);
        export_hashes_as_vic_obj->pub_set_vic_export_dir_path(destination_location);
        export_hashes_as_vic_obj->pub_set_export_file_details(export_as_hashes_details_list);
        if(vic_version == QString("1.1") || vic_version == QString("1.2"))
        {
            export_hashes_as_vic_obj->pub_start_vic_export_general();
        }
        else
        {
            export_hashes_as_vic_obj->pub_start_vic_export_1_3();
        }
    }
    else if(export_hashes_as.contains(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_2_0)))
    {
        export_hashes_as_vic_obj->pub_set_vic_version(vic_version);
        export_hashes_as_vic_obj->pub_set_essential();
        export_hashes_as_vic_obj->show();
    }
    else if(export_hashes_as.contains(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_SQLite)))
    {
        export_hashes_as_sqlite_obj->pub_set_essential();
        export_hashes_as_sqlite_obj->show();
    }
    else if(export_hashes_as.contains(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_CSV)))
    {
        export_hashes_as_csv_obj->pub_set_essential();
        export_hashes_as_csv_obj->show();
    }
}

void filetypes_loader::slot_sub_menu_run_fs_modules_clicked(QAction* current_clicked_action)
{
    if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata))
    {
        action_run_fs_modules_apple_metadata_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Exif_Metadata))
    {
        action_run_fs_modules_exif_metadata_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Signature_Analysis))
    {
        action_run_fs_modules_signature_analysis_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Hashes))
    {
        action_run_fs_modules_hashset_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis))
    {
        action_run_fs_modules_face_analysis_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition))
    {
        action_run_fs_modules_optical_character_recognition_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons))
    {
        action_run_fs_modules_weapons_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis))
    {
        action_run_fs_modules_fire_analysis_clicked();
    }
    else if(current_clicked_action->text().trimmed() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection))
    {
        action_run_fs_modules_skin_tone_detection_clicked();
    }

}

void filetypes_loader::slot_r_case_apple_metadata_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_apple_metadata_status = status;
}

void filetypes_loader::slot_r_case_exif_metadata_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_exif_metadata_status = status;
}

void filetypes_loader::slot_r_case_signature_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_signature_analysis_status = status;
}

void filetypes_loader::slot_r_case_mime_type_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_mime_type_analysis_status = status;
}

void filetypes_loader::slot_r_case_hashset_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_hashset_status = status;
}

void filetypes_loader::slot_r_case_face_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_face_analysis_status = status;
}

void filetypes_loader::slot_r_case_optical_character_recognition_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_optical_character_recognition_status = status;
}

void filetypes_loader::slot_r_case_weapons_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_weapons_status = status;
}

void filetypes_loader::slot_r_case_fire_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_fire_analysis_status = status;
}

void filetypes_loader::slot_r_case_skin_tone_detection_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_skin_tone_detection_status = status;
}

void filetypes_loader::pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str)
{ // add bookmark from meta data information
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(true);


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString , 0);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        QString tmp_rec_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        if(tmp_rec_no != record_no_str)
            continue;


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        if(recon_helper_standard_obj->pub_check_bookmarked_record_in_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display ,source_count_name_str, record_no_str))
            continue;

        source_count_name_list << source_count_name_str;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = tmp_rec_no;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");

        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_bookmark_item_list.append(obj);

        m_tablewidget_obj->selectRow(selected_row_no);


    }

    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    global_connection_manager_obj->pub_add_bookmark_for_file_system(list_st_bookmark_item_list);

    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_bookmark_item_list.size(); num++)
        {
            if(list_st_bookmark_item_list.at(num).source_count_name == source_count_name_str)
                INT_list.append(list_st_bookmark_item_list.at(num).record_no);
        }
        global_connection_manager_obj->pub_set_bookmark_in_fs_source_db(INT_list, source_count_name_str);


    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str)
{ // remove the bookmark from metadata information
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(false);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString , 0);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        QString tmp_rec_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        if(tmp_rec_no != record_no_str)
            continue;


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        if(recon_helper_standard_obj->pub_check_bookmarked_record_in_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display,source_count_name_str, record_no_str))
            continue;

        source_count_name_list << source_count_name_str;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = tmp_rec_no;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");

        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_bookmark_item_list.append(obj);

        m_tablewidget_obj->selectRow(selected_row_no);

    }

    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }



    global_connection_manager_obj->pub_delete_bookmark_for_file_system(list_st_bookmark_item_list);


    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_bookmark_item_list.size(); num++)
        {
            if(list_st_bookmark_item_list.at(num).source_count_name == source_count_name_str)
                INT_list.append(list_st_bookmark_item_list.at(num).record_no);
        }
        global_connection_manager_obj->pub_remove_bookmark_in_fs_source_db(INT_list, source_count_name_str);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}
void filetypes_loader::slot_export_hashes_as_sqlite_clicked()
{ // export hashes as a sqlite database clicked from right click
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

    export_as_hashes_details_list.clear();
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text();

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

        QFileInfo info_cmplt(complete_file_path);

        QFileInfo file_info(display_filepath);
        QString display_filename;
        display_filename = file_info.fileName();

        if(QFileInfo(complete_file_path).isDir())
            continue;
        //
        else
        {
            struct_global_export_as_hashes_details struct_export_hashes_details_obj;

            struct_export_hashes_details_obj.fs_record_no       = fs_record_no;
            struct_export_hashes_details_obj.display_filepath   = display_filepath;
            struct_export_hashes_details_obj.display_filename  = display_filename;
            struct_export_hashes_details_obj.complete_file_path = complete_file_path;
            struct_export_hashes_details_obj.source_count_name  = source_count_name;
            struct_export_hashes_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

            struct_export_hashes_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);
            struct_export_hashes_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 1, fs_db_path, Q_FUNC_INFO);
            struct_export_hashes_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 2, fs_db_path, Q_FUNC_INFO);
            struct_export_hashes_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 3, fs_db_path, Q_FUNC_INFO);
            export_as_hashes_details_list.append(struct_export_hashes_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    export_hashes_as_sqlite_obj->pub_set_export_file_details(export_as_hashes_details_list);
    export_hashes_as_sqlite_obj->pub_start_export_hashes_as_sqlite();
}

void filetypes_loader::slot_export_hashes_as_csv_clicked()
{ // export hashes as a csv report clicked
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

    export_as_hashes_details_list.clear();
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text();

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

        QFileInfo info_cmplt(complete_file_path);

        QFileInfo file_info(display_filepath);
        QString display_filename;
        display_filename = file_info.fileName();


        if(QFileInfo(complete_file_path).isDir())
            continue;


        else
        {
            struct_global_export_as_hashes_details struct_export_hashes_details_obj;

            struct_export_hashes_details_obj.fs_record_no       = fs_record_no;
            struct_export_hashes_details_obj.display_filepath   = display_filepath;
            struct_export_hashes_details_obj.display_filename  = display_filename;
            struct_export_hashes_details_obj.complete_file_path = complete_file_path;
            struct_export_hashes_details_obj.source_count_name  = source_count_name;
            struct_export_hashes_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

            struct_export_hashes_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);
            struct_export_hashes_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 1, fs_db_path, Q_FUNC_INFO);
            struct_export_hashes_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 2, fs_db_path, Q_FUNC_INFO);
            struct_export_hashes_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 3, fs_db_path, Q_FUNC_INFO);
            export_as_hashes_details_list.append(struct_export_hashes_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    export_hashes_as_csv_obj->pub_set_export_file_details(export_as_hashes_details_list);
    export_hashes_as_csv_obj->pub_start_export_hashes_as_csv();
}

void filetypes_loader::slot_export_hashes_as_vic_clicked()
{ // export hashes in the form of vic
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

    export_as_hashes_details_list.clear();
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text();

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

        QFileInfo info_cmplt(complete_file_path);

        QFileInfo file_info(display_filepath);
        QString display_filename;
        display_filename = file_info.fileName();

        if(QFileInfo(complete_file_path).isDir())
        {
            QStringList value_list;
            QString val_str = display_filepath;
            val_str.append("%");
            value_list << val_str;

            ///Run only for files not processed
            QStringList record_no_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select INT From files Where file_parent_path like ?"), value_list, 0, fs_db_path, Q_FUNC_INFO);

            ///get all files/directories in selected directory
            for(int count = 0; count < record_no_list.size(); count++)
            {
                if(count % 50 == 0)
                    QCoreApplication::processEvents();

                fs_record_no       = record_no_list.at(count);
                complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

                ///get all files(only) in selected directory
                if(QFileInfo(complete_file_path).isDir())
                    continue;

                display_filepath   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select filepath From files Where INT = ?"), QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);

                struct_global_export_as_hashes_details struct_export_vic_details_obj;

                struct_export_vic_details_obj.fs_record_no       = fs_record_no;
                struct_export_vic_details_obj.display_filepath   = display_filepath;
                struct_export_vic_details_obj.complete_file_path = complete_file_path;
                struct_export_vic_details_obj.source_count_name  = source_count_name;
                struct_export_vic_details_obj.examiner_selected_file_or_dir_name = info_cmplt.fileName();

                struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);
                struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 1, fs_db_path, Q_FUNC_INFO);
                struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 2, fs_db_path, Q_FUNC_INFO);
                struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 3, fs_db_path, Q_FUNC_INFO);

                export_as_hashes_details_list.append(struct_export_vic_details_obj);
            }
        }
        else
        {
            struct_global_export_as_hashes_details struct_export_vic_details_obj;

            struct_export_vic_details_obj.fs_record_no       = fs_record_no;
            struct_export_vic_details_obj.display_filepath   = display_filepath;
            struct_export_vic_details_obj.display_filename   = display_filename;
            struct_export_vic_details_obj.complete_file_path = complete_file_path;
            struct_export_vic_details_obj.source_count_name  = source_count_name;
            struct_export_vic_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

            struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 0, fs_db_path, Q_FUNC_INFO);
            struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 1, fs_db_path, Q_FUNC_INFO);
            struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 2, fs_db_path, Q_FUNC_INFO);
            struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tstmp, QStringList(fs_record_no), 3, fs_db_path, Q_FUNC_INFO);

            export_as_hashes_details_list.append(struct_export_vic_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    export_hashes_as_vic_obj->pub_set_export_file_details(export_as_hashes_details_list);
    export_hashes_as_vic_obj->pub_start_vic_export_2_0();
}


void filetypes_loader::slot_custom_table_header_widget_search_field_return_pressed()
{ // search anything from cusetom search header by pressing enter
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    QList<struct_search_column_text_for_file_type_loader> search_text_col_QList;

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_file_type_loader struct_search_column_text_for_file_type_loader_obj;
            struct_search_column_text_for_file_type_loader_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_file_type_loader_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_file_type_loader_obj);
            bool_stop_sorting_tags_bookmark_notes_after_search = true;
        }
    }

    if(search_text_col_QList.size() < 1)
    {
        slot_pushbutton_showall_clicked();
        return;
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    all_column_text_search(search_text_col_QList);

    bool_did_tableview_display_changed = true;

    pub_refresh_horizontal_header();

    emit signal_clear_all_metadata();
    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_custom_table_header_widget_search_field_text_changed()
{ // search on text change automatic without ptessing anything
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void filetypes_loader::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{ // sorting clicked from custom tableiwidget header
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    m_tablewidget *current_widget = NULL;

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
        current_widget = m_tablewidget_obj;
    else if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
        current_widget = m_table_carved_index_files;

    if(current_widget == NULL)
        return;

    if(column != enum_tablewidget_column_bookmark && column != enum_tablewidget_column_notes && column != enum_tablewidget_column_tags)
    {
        if(column_sort_order == 0)
            current_widget->sortItems(column, Qt::DescendingOrder);
        else
            current_widget->sortItems(column, Qt::AscendingOrder);

        pub_refresh_horizontal_header();
        return;
    }

    if(bool_stop_sorting_tags_bookmark_notes_after_search)
        return;

    current_widget->clearSelection();

    if(current_widget->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = current_widget->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < current_widget->rowCount(); row++)
        {
            if(current_widget->isRowHidden(row))
            {
                continue;
            }

            if(current_widget->item(row, column) == NULL)
                continue;

            QIcon icon_set;
            if(column != enum_tablewidget_column_bookmark)
                icon_set =  current_widget->item(row, column)->icon();



            current_widget->item(row, column)->checkState();

            // Check for checkstate OR iCon
            if(current_widget->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column != enum_tablewidget_column_bookmark)
                    icon_shiftrow_acs =  current_widget->item(row_shift_pos_ascending, column)->icon();

                // If row is already on 0th position then increment and continue
                if(current_widget->item(row_shift_pos_ascending, column)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = current_widget->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_widget->takeRow(destRow);

                // Shifting of rows from lower to upper
                current_widget->setRow(destRow, sourceItems);
                current_widget->setRow(sourceRow, destItems);


                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(current_widget->isRowHidden(row))
            {
                continue;
            }

            if(current_widget->item(row, column) == NULL)
                continue;

            if(current_widget->item(row_shift_pos_descending, column) == NULL)
                continue;

            QIcon icon_set;
            if(column != enum_tablewidget_column_bookmark)
                icon_set =  current_widget->item(row, column)->icon();

            if(current_widget->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column != enum_tablewidget_column_bookmark)
                    icon_shiftrow_desc =  current_widget->item(row_shift_pos_descending, column)->icon();


                // If row is already on last position then decrement and continue
                if(current_widget->item(row_shift_pos_descending, column)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = current_widget->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_widget->takeRow(destRow);

                // Shifting of rows from upper to lower
                current_widget->setRow(destRow, sourceItems);
                current_widget->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }

    pub_refresh_horizontal_header();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void filetypes_loader::all_column_text_search(QList<struct_search_column_text_for_file_type_loader> search_text_all_column_list)
{ // text search from all columns
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    st_gallery_view_file_info_list.clear();

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool hide_row = false;

        for(int column_count = 0; column_count < search_text_all_column_list.size(); column_count++)
        {
            struct_search_column_text_for_file_type_loader struct_search_column_text_for_file_type_loader_obj = search_text_all_column_list.at(column_count);

            if(!(m_tablewidget_obj->item(row, struct_search_column_text_for_file_type_loader_obj.column_number)->text().contains(struct_search_column_text_for_file_type_loader_obj.search_text, Qt::CaseInsensitive)))
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

            QString full_filepath;

            QTableWidgetItem *item_filename = m_tablewidget_obj->item(row, enum_File_Name_Index);
            QTableWidgetItem *item_filepath = m_tablewidget_obj->item(row, enum_File_Path_Index);
            QTableWidgetItem *item_INT = m_tablewidget_obj->item(row, enum_Record_No_Index);
            QTableWidgetItem *item_bk = m_tablewidget_obj->item(row, enum_Bookmarks_Index);
            QTableWidgetItem *exif_latitu = m_tablewidget_obj->item(row, enum_Exif_Latitude_Index);
            QTableWidgetItem *exif_longit = m_tablewidget_obj->item(row, enum_Exif_Longitude_Index);
            QTableWidgetItem *item_date_accessed = m_tablewidget_obj->item(row, enum_Date_Accessed_Index);

            QString item_source_count  = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index)->text().trimmed();
            QString file_size = m_tablewidget_obj->item(row, enum_File_Size_Index)->text();
            QString mime_type_str   = m_tablewidget_obj->item(row, enum_Mime_Type_Index)->text().trimmed();

            bool bool_is_it_file = false;

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + item_source_count + QString("/file_system.sqlite");
            bool bool_dir_record = recon_helper_standard_obj->is_it_dir_record_no_by_dbpath(item_INT->text(),fs_db_path,"files",item_source_count,Q_FUNC_INFO);

            if(!bool_dir_record)
            {
                full_filepath = recon_helper_standard_obj->get_file_infopath_by_dbpath(item_INT->text(),fs_db_path,"files",item_source_count,Q_FUNC_INFO);
                bool_is_it_file = true;
            }

            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item_source_count);
            QString tmp_path = item_filepath->text();
            QString file_full_path;

            if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
            {
                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                        + struct_info.source_count_name + "/file_system.sqlite";

                QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(tmp_path), 0, fs_db_path, Q_FUNC_INFO);
                if(!idpath.isEmpty())
                {
                    file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, idpath, Q_FUNC_INFO);
                    bool_is_it_file = true;
                }
            }
            else
            {
                file_full_path = recon_static_functions::get_complete_file_path(struct_info.fs_extraction_source_path, tmp_path, Q_FUNC_INFO);
                bool_is_it_file = QFileInfo(file_full_path).isFile();
            }

            if(!bool_is_it_file)
                continue;

            if(mime_type_str.startsWith("image/") || recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
            {
                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();
                f1.file_path = item_filepath->text();
                f1.INT = item_INT->text();
                f1.source_count_name = item_source_count;
                f1.table_view_row_no = row;
                f1.latitude = "";
                f1.longitude = "";
                f1.thumbnail_file_path = full_filepath;
                f1.full_filepath = full_filepath;
                f1.bookmark = "0";
                f1.last_access_timestamp = item_date_accessed->text();
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }
            else if(mime_type_str.startsWith("video/") || recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(item_filename->text()))
            {
                QString file_name = item_filename->text();
                QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + struct_info.source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

                QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";
                QString fs_record_no = item_INT->text();

                QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
                QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

                if(!thumb_file_path.isEmpty())
                {
                    file_full_path = result_filepath + thumb_file_path;
                }
                else
                {
                    file_name.remove(file_name.lastIndexOf("."),file_name.size());
                    video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

                    //                    QString script_path = "../Resources/scripts_persistent/video_frame.py";
                    //                    QString cmd_python_path = global_varibale_python_path;

                    //                    QStringList args;
                    //                    args << script_path << file_full_path << video_thumbnails_path << file_size;

                    QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                    QStringList args;
                    args << file_full_path << video_thumbnails_path << file_size;

                    QProcess process_thumbnails;
                    //                    process_thumbnails.start(cmd_python_path,args);
                    process_thumbnails.start(video_frame_exe_path,args);
                    if(!process_thumbnails.waitForStarted())
                    {
                        recon_static_functions::app_debug("process_thumbnails -----FAILED----- ",Q_FUNC_INFO);
                        recon_static_functions::app_debug("Error :- " + process_thumbnails.errorString() ,Q_FUNC_INFO);
                        continue;
                    }

                    bool_process_thumbnails_finished = false;
                    connect(&process_thumbnails, SIGNAL(finished(int)), this, SLOT(slot_process_video_thumbnails_finished(int)));

                    int count;
                    while(1)
                    {
                        count++;
                        if(count % 100 == 0)
                        {
                            QCoreApplication::processEvents();
                            count = 0;
                        }

                        if(bool_process_thumbnails_finished || process_thumbnails.state() == QProcess::NotRunning)
                        {
                            process_thumbnails.close();
                            break;
                        }
                    }

                    file_full_path = QFileInfo(video_thumbnails_path).absoluteFilePath();

                    QString thumb_filepath;
                    QStringList list;
                    QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                    QFileInfo thumbnail_file_existence(file_full_path);
                    if(thumbnail_file_existence.exists())
                    {
                        thumb_filepath.clear();
                        thumb_filepath = file_full_path;
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                    }
                    else
                    {
                        thumb_filepath.clear();
                        QFile tmp_file("../Resources/features_resources/video_thumbnail/default_video_thumbnail.png");
                        tmp_file.copy(video_thumbnails_path);
                        thumb_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                    }
                }

                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();
                f1.file_path = item_filepath->text();
                f1.INT = item_INT->text();
                f1.table_view_row_no = row;
                f1.thumbnail_file_path = full_filepath;
                f1.full_filepath = full_filepath;
                f1.bookmark = "0";
                f1.latitude = exif_latitu->text();
                f1.longitude = exif_longit->text();
                f1.source_count_name = item_source_count;
                f1.last_access_timestamp = item_date_accessed->text();
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }
        }
    }
    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
        slot_main_tab_widget_current_changed(enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}
