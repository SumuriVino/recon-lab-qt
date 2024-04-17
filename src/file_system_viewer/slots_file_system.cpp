#include "file_system.h"

void file_system::slot_file_system_menu_right_clicked(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
    {
        action_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_remove_bookmarks_triggered();
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
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_In_Window))
    {
        action_open_in_full_triggered();
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
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Hide_Seen_Files))
    {
        bool_Global_hide_seen_files = true;
        current_clicked_action->setEnabled(false);

        QList<QAction *> list = submenu_seen->actions();

        for(int i = 0; i < list.size(); i++)
        {
            if(list.at(i)->text() == QString(MACRO_Generic_Right_Click_Show_Seen_Files))
            {
                list.at(i)->setDisabled(false);
                break;
            }
        }

        set_fs_display(main_db_cmd , current_parent_dir_path , true, false);
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Show_Seen_Files))
    {
        bool_Global_hide_seen_files = false;
        current_clicked_action->setDisabled(true);

        QList<QAction *> list = submenu_seen->actions();

        for(int i = 0; i < list.size(); i++)
        {
            if(list.at(i)->text() == QString(MACRO_Generic_Right_Click_Hide_Seen_Files))
            {
                list.at(i)->setDisabled(false);
                break;
            }
        }

        set_fs_display(main_db_cmd , current_parent_dir_path , true, false);
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Decompress_File))
    {
        action_extract_compressed_file();
    }

}

void file_system::slot_tablewidget_current_cell_changed(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    bool_cell_changed_connection_buildup_for_tablewidget = true;

    tablewidget_general_clicked(currentRow, currentColumn);
}

void file_system::slot_pushButton_back_clicked()
{
    lineEdit_search->clear();

    clear_all_custom_header_widget_entries();

    ///disable back button if can't go more backwards
    if(hlayout_frame_dir_navigation->layout() != NULL && hlayout_frame_dir_navigation->layout()->count() <= 1)
    {
        pushButton_back->setEnabled(false);
        return;
    }

    QString file_path;

    QStringList dir_path_parts = current_parent_dir_path.split("/");
    if(dir_path_parts.isEmpty())
        return;

    dir_path_parts.removeLast();

    file_path = dir_path_parts.join("/");

    QString tmp_virtual_path = fs_extraction_path;
    tmp_virtual_path = tmp_virtual_path.remove(0,file_path.size());
    if(tmp_virtual_path == "/" && !file_path.endsWith("/"))
        file_path.append("/");

    current_parent_dir_path = file_path;

    make_display_for_specified_path(file_path);
}


void file_system::slot_pushButton_next_clicked()
{
    lineEdit_search->clear();

    clear_all_custom_header_widget_entries();

    if(deepest_path.trimmed().isEmpty())
        return;

    if(!deepest_path.contains(current_parent_dir_path.trimmed()))
        return;

    QString str = deepest_path;
    QString path_to_next = current_parent_dir_path.trimmed();
    str = str.remove(0, path_to_next.size());

    if(str.size() > 1)
    {
        if(str.startsWith("/"))
            str.remove(0,1);
    }

    QStringList list = str.split("/", Qt::SkipEmptyParts);
    if(list.isEmpty())
        return;

    if(path_to_next.endsWith("/"))
        path_to_next =  path_to_next + list.at(0);
    else
        path_to_next =  path_to_next + "/" + list.at(0);

    current_parent_dir_path = path_to_next;

    make_display_for_specified_path(path_to_next);

}

void file_system::slot_bookmark_key_pressed_tablewidget()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int current_row = m_file_system_tablewidget_obj->currentRow();
    int current_col = m_file_system_tablewidget_obj->currentColumn();

    if(current_row < 0)
        return;


    if(m_file_system_tablewidget_obj->item(current_row, enum_File_System_display_BOOKMARK)->checkState())
    {
        m_file_system_tablewidget_obj->item(current_row, enum_File_System_display_BOOKMARK)->setCheckState(Qt::Unchecked);
    }
    else
    {
        m_file_system_tablewidget_obj->item(current_row, enum_File_System_display_BOOKMARK)->setCheckState(Qt::Checked);
    }

    update_bookmarks_status_on_checkbox_click(current_row , enum_File_System_display_BOOKMARK);


    //    if(current_row != m_file_system_tablewidget_obj->rowCount() - 1)
    //        m_file_system_tablewidget_obj->setCurrentCell(current_row + 1, 1);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void file_system::slot_arrow_key_pressed()
{
    int row = m_file_system_tablewidget_obj->currentRow();

    if(row < 0)
        return;

    if(m_file_system_tablewidget_obj->isRowHidden(row))
        return;

    tablewidget_general_clicked(m_file_system_tablewidget_obj->currentRow() , m_file_system_tablewidget_obj->currentColumn());
}

void file_system::slot_fs_tablewidget_single_click(int row, int column)
{
    tablewidget_general_clicked(row,column);
}


void file_system::update_bookmarks_status_on_checkbox_click(int row , int column)
{
    if(column != enum_File_System_display_BOOKMARK)
        return;

    int selected_row_no = row;
    if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
        return;

    QStringList selected_record_num_list;
    QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
    selected_record_num_list << record ;



    struct_global_tagging_tag_notes_db_updation obj;
    obj.bookmark = "1";
    obj.category = QString(MACRO_Plugin_Name_File_System);
    obj.record_no = record;
    obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
    obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
    obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
    obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
    obj.table_name = QString("files");
    obj.recon_tag_value = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_TAG_VALUE)->text().trimmed();
    obj.notes = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_NOTES_VALUE)->text().trimmed();
    obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();
    obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
    obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
    obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
    obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
    obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;
    list_st_bookmark_item_list.append(obj);

    if(list_st_bookmark_item_list.isEmpty())
        return;

    bool bool_add_boomark = false;
    if(m_file_system_tablewidget_obj->item(row,enum_File_System_display_BOOKMARK)->checkState() == Qt::Checked)
    {
        bool_add_boomark = true;
    }
    else if(m_file_system_tablewidget_obj->item(row,enum_File_System_display_BOOKMARK)->checkState() == Qt::Unchecked)
    {
        bool_add_boomark = false;
    }

    if(bool_add_boomark)
    {
        bookmarked_records_from_table_display_list << record;

        global_connection_manager_obj->pub_set_bookmark_in_fs_source_db(selected_record_num_list, source_count_name);
        global_connection_manager_obj->pub_add_bookmark_for_file_system(list_st_bookmark_item_list);
    }
    else
    {
        qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(record);
        if(tmp_index >= 0)
            bookmarked_records_from_table_display_list.removeAt(tmp_index);

        global_connection_manager_obj->pub_remove_bookmark_in_fs_source_db(selected_record_num_list, source_count_name);
        global_connection_manager_obj->pub_delete_bookmark_for_file_system(list_st_bookmark_item_list);

    }

}

void file_system::slot_fs_tablewidget_double_click(int row, int column)
{
    if(row < 0)
        return;

    QString file_path =   m_file_system_tablewidget_obj->item(row,enum_File_System_display_FILEPATH)->text().trimmed();
    QString record = m_file_system_tablewidget_obj->item(row, enum_File_System_display_INT)->text().trimmed();

    bool bool_compressed_file_extracted = false;

    bool bool_is_it_file = true;

    if(recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record,destination_db_obj,"files",source_count_name,Q_FUNC_INFO) ? bool_is_it_file = false : bool_is_it_file = true);

    QString cmd_itm = "select decompression_status from files where INT = ?";
    QString uncmp_val = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_itm, QStringList(record), 0, destination_db_obj, Q_FUNC_INFO);
    if(uncmp_val.trimmed() == "1")
    {
        bool_compressed_file_extracted = true;
    }

    if(bool_is_it_file && !bool_compressed_file_extracted)
    {
        m_tablewidget *m_tablewidget_obj = (m_tablewidget*) sender();

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
        return;
    }

    if(!deepest_path.contains(file_path))
    {
        deepest_path = file_path.trimmed();
    }

    current_parent_dir_path = file_path;

    make_display_for_specified_path(file_path);

}

void file_system::slot_sub_menu_open_with_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

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
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        action_open_with_registry_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
    {
        add_applications_to_open_application_menu();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        action_open_with_log_viewer_triggered();
    }
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void file_system::slot_sub_menu_sendToBucket_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        action_send_to_bucket_plist_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        action_send_to_bucket_hex_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        action_send_to_bucket_sqlite_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        action_send_to_bucket_registry_viewer_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        action_send_to_bucket_log_viewer_triggered();
    }
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_bookmark_triggered()
{

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(true);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->checkState() == Qt::Checked)
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        selected_record_num_list << record ;

        m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->setCheckState(Qt::Checked);


        bookmarked_records_from_table_display_list << record;

        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "1";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.recon_tag_value = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_TAG_VALUE)->text().trimmed();

        obj.notes = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_NOTES_VALUE)->text().trimmed();
        obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();




        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();


        list_st_bookmark_item_list.append(obj);

    }


    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    global_connection_manager_obj->pub_set_bookmark_in_fs_source_db(selected_record_num_list, source_count_name);
    global_connection_manager_obj->pub_add_bookmark_for_file_system(list_st_bookmark_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void file_system::action_extract_compressed_file()
{
    recon_static_functions::debug_intensive(" start ", Q_FUNC_INFO);

    int selected_row_no = m_file_system_tablewidget_obj->currentRow();

    if((selected_row_no < 0) || (selected_row_no >= m_file_system_tablewidget_obj->rowCount()))
        return;

    if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
        return;


    QString decom_level = "Decompressed";
    QString itm_val = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_DECOMPRESSION_STATUS)->text().trimmed();
    if(itm_val.trimmed() == QString(decom_level))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "File already decompressed. Do double click to see content.");
        message_dialog_obj->show();
        return;
    }

    QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
    decompress_file_system_obj->pub_set_db_essentials(db_path_filesystem,destination_db_obj);
    decompress_file_system_obj->pub_set_source_count_name(source_count_name);
    decompress_file_system_obj->pub_set_record_no_for_decompression(record);

    decompress_file_system_obj->pub_set_decompress_file_path(m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed());
    decompress_file_system_obj->pub_set_treewidget_case_display_obj(treewidget_case_display_obj);

    QString m_value = decompress_file_system_obj->start_run();

    if(m_value == MACRO_PASS_VALUE_READABLE_QString)
    {
        m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_DECOMPRESSION_STATUS)->setText(decom_level);
        change_textcolor_for_compression_decompression(selected_row_no , true);


        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "Decompression has been done. Do you want to open it?"));
        message_dialog_obj->show();

        recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
        bool_message_box_clicked = false;
        while(1)
        {
            QCoreApplication::processEvents();
            if(bool_message_box_clicked)
                break;
        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

        if(bool_message_box_yes_button_clicked)
        {
            slot_fs_tablewidget_double_click(selected_row_no,enum_File_System_display_FILENAME);
        }

    }

    recon_static_functions::debug_intensive(" end ", Q_FUNC_INFO);

    return;

}

void file_system::action_remove_bookmarks_triggered()
{

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(false);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    m_file_system_tablewidget_obj->setCurrentCell(-1, 0);
    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->checkState() == Qt::Unchecked)
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        selected_record_num_list << record ;

        m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->setCheckState(Qt::Unchecked);


        qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(record);
        if(tmp_index >= 0)
            bookmarked_records_from_table_display_list.removeAt(tmp_index);

        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "0";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");

        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();


        list_st_bookmark_item_list.append(obj);

    }


    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    global_connection_manager_obj->pub_remove_bookmark_in_fs_source_db(selected_record_num_list, source_count_name);
    global_connection_manager_obj->pub_delete_bookmark_for_file_system(list_st_bookmark_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();


}


void file_system::update_bookmarks_in_bookmark_serach(bool updation_status, QList<struct_global_tagging_tag_notes_db_updation> list_obj)
{

    thread_db_updation_bookmark_obj = new thread_db_updation_bookmark;


    qthread_bookmark_db_updation_obj = new QThread;
    thread_db_updation_bookmark_obj->moveToThread(qthread_bookmark_db_updation_obj);


    connect(qthread_bookmark_db_updation_obj, SIGNAL(started()), thread_db_updation_bookmark_obj, SLOT(slot_bookmark_updation_start()));
    connect(thread_db_updation_bookmark_obj, SIGNAL(finished()), qthread_bookmark_db_updation_obj, SLOT(quit()));
    connect(thread_db_updation_bookmark_obj, SIGNAL(finished()), this, SLOT(slot_bookmark_updation_finished()));

    thread_db_updation_bookmark_obj->pub_set_updation_status(updation_status);
    thread_db_updation_bookmark_obj->set_record_value_list(list_obj);


    qthread_bookmark_db_updation_obj->start();

}

void file_system::slot_bookmark_updation_finished()
{


    if(qthread_bookmark_db_updation_obj->isRunning())
        qthread_bookmark_db_updation_obj->quit();
    if(qthread_bookmark_db_updation_obj != NULL && !qthread_bookmark_db_updation_obj->isRunning())
        delete qthread_bookmark_db_updation_obj;


    if(thread_db_updation_bookmark_obj != NULL)
        delete thread_db_updation_bookmark_obj;


    emit signal_common_add_tag_name_into_case_tree(MACRO_Generic_Tag_Bookmarks);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void file_system::action_add_note_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int current_row = m_file_system_tablewidget_obj->currentRow();
    QString record = m_file_system_tablewidget_obj->item(current_row , enum_File_System_display_INT)->text();

    QString command = QString("select notes from '") + QString("files'") + QString(" WHERE INT = ?");
    QStringList arg_list;
    arg_list << record;
    QString already_added_note = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,db_path_filesystem,Q_FUNC_INFO);

    add_notes_to_singal_record_obj->set_notes(already_added_note);
    add_notes_to_singal_record_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_pushbutton_add_notes_clicked(QString notes_value)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    add_notes_to_singal_record_obj->hide();
    notes_value = notes_value.trimmed();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_tag_notes_db_updation> list_st_notes_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        selected_record_num_list << record ;


        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "1";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.notes = notes_value;
        obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();

        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();

        QIcon iconNotes("../icons/notes.png");

        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_NOTES_ICON)->setIcon(iconNotes);

        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_NOTES_VALUE)->setText(notes_value);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_NOTES_ICON)->setToolTip(notes_value);


        list_st_notes_item_list.append(obj);

    }


    if(list_st_notes_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    global_connection_manager_obj->pub_set_notes_in_fs_source_db(selected_record_num_list, source_count_name, notes_value);
    global_connection_manager_obj->pub_add_notes_for_file_system(list_st_notes_item_list, notes_value);

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_remove_note_triggered()
{

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_tag_notes_db_updation> list_st_notes_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        selected_record_num_list << record ;


        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.notes = "";
        obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();

        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();

        QIcon icon(QString(""));
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_NOTES_ICON)->setIcon(icon);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_NOTES_ICON)->setToolTip("");
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_NOTES_VALUE)->setText("");

        list_st_notes_item_list.append(obj);
    }


    if(list_st_notes_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    global_connection_manager_obj->pub_remove_notes_in_fs_source_db(selected_record_num_list, source_count_name);
    global_connection_manager_obj->pub_delete_notes_for_file_system(list_st_notes_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void file_system::action_add_note_to_bookmark_triggered()
{
    add_notes_to_bookmarks_obj->show();
}

void file_system::slot_pushbutton_add_notes_to_bookmark_clicked(QString text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    add_notes_to_bookmarks_obj->hide();

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString command = "update '" + table_name + "' set notes=? where bookmark" + "='1' AND (notes IS NULL or notes is '')";
    QStringList arg_list;
    arg_list << text;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path_filesystem,Q_FUNC_INFO);

    QIcon iconNotes("../icons/notes.png");

    for(int k = 0 ; k < m_file_system_tablewidget_obj->rowCount(); k++)
    {
        if(k%100 == 0)
            QCoreApplication::processEvents();

        if(m_file_system_tablewidget_obj->item(k, bookmark_index)->checkState())
        {
            m_file_system_tablewidget_obj->item(k, note_icon_index)->setIcon(iconNotes);

            QString record = m_file_system_tablewidget_obj->item(k , enum_File_System_display_INT)->text();

            QStringList applied_tags_on_this_record = get_applied_tags_name_on_current_record(record);
            emit signal_update_notes_value_of_other_display_and_db(record, QString(MACRO_Plugin_Name_File_System), MACRO_Plugin_File_System_TAB_Name_Files, text, applied_tags_on_this_record, os_scheme_display,MACRO_CASE_TREE_Source);

            QString result_note_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

            emit signal_add_notes_from_file_system(record, MACRO_Plugin_File_System_TAB_Name_Files, result_note_db_path, text ,db_path_filesystem, os_scheme_display);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_remove_note_bookmark_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString command = "update '" + table_name + "' set notes=? where bookmark " + "='1'";
    QStringList arg_list;
    arg_list << QString("");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path_filesystem,Q_FUNC_INFO);

    QIcon iconNotes("");

    for(int k = 0 ; k <  m_file_system_tablewidget_obj->rowCount() ; k++)
    {
        if(k%100 == 0)
            QCoreApplication::processEvents();

        if(m_file_system_tablewidget_obj->item(k, bookmark_index)->checkState())
        {
            m_file_system_tablewidget_obj->item(k, note_icon_index)->setIcon(iconNotes);

            QString record = m_file_system_tablewidget_obj->item(k , enum_File_System_display_INT)->text();
            QStringList applied_tags_on_this_record = get_applied_tags_name_on_current_record(record);

            emit signal_update_notes_value_of_other_display_and_db(record, QString(MACRO_Plugin_Name_File_System), MACRO_Plugin_File_System_TAB_Name_Files, "", applied_tags_on_this_record, os_scheme_display, MACRO_CASE_TREE_Source);
            emit signal_update_display_on_remove_notes(QString(MACRO_Plugin_Name_File_System), MACRO_Plugin_File_System_TAB_Name_Files,  record,  "",  os_scheme_display);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_with_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        emit signal_statusbar_progress_bar_show_hide(false , QString(""));

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

    QString src_cnt_name_label = QString(" [" + source_count_name + "]");
    QString hex_display_nm = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;


    QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
    if(list_hex_display_name.contains(hex_display_nm))
    {
        emit signal_show_file_in_viewer_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer, source_count_name);
        emit signal_statusbar_progress_bar_show_hide(false , QString(""));
        return;
    }


    ///check file in bucket
    struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;

    recon_static_functions::clear_variables_hex_viewer_open_file_data(hex_viewer_open_file_data_obj);

    QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    QStringList value_list;
    value_list << hex_display_nm << source_count_name;

    QString tmp_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select file_path from hex_viewer_index Where viewer_display_name = ? AND source_count_name = ?"), value_list, 0, hex_feature_db_path, Q_FUNC_INFO);
    tmp_file_path = tmp_file_path.trimmed();
    if(!tmp_file_path.isEmpty())
    {
        hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
    }
    else
    {
        hex_viewer_open_file_data_obj.bool_is_file_in_bucket = false;
        hex_viewer_open_file_data_obj.hex_feature_db_path.clear();
    }

    hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;
    hex_viewer_open_file_data_obj.complete_file_path  = complete_path;
    hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
    hex_viewer_open_file_data_obj.record_no_str       = record_no;
    hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
    hex_viewer_open_file_data_obj.plugin_name_str     = pluginName;
    hex_viewer_open_file_data_obj.plugin_tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);
    hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
    hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

    emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_with_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        emit signal_statusbar_progress_bar_show_hide(false , QString(""));

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    if(!recon_static_functions::check_is_it_plist_file_by_signature(complete_path, Q_FUNC_INFO))
    {
        emit signal_statusbar_progress_bar_show_hide(false , QString(""));

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not plist file."));
        message_dialog_obj->show();
        return;
    }
    else
    {
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString plist_display_nm = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;


        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(plist_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
            emit signal_statusbar_progress_bar_show_hide(false , QString(""));

            return;
        }

        plist_viewer *plist_viewer_obj;
        plist_viewer_obj = new plist_viewer(this);
        QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;

        plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
        plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
        plist_viewer_obj->pub_set_plist_path(complete_path);
        plist_viewer_obj->pub_set_source_count_name(source_count_name);
        plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        plist_viewer_obj->pub_set_essentials();

        plist_viewer_obj->pub_start_plist_extraction();

        emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";
    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        emit signal_statusbar_progress_bar_show_hide(false , QString(""));

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
    {
        emit signal_statusbar_progress_bar_show_hide(false , QString(""));

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not sqlite file");
        message_dialog_obj->show();
        return;
    }
    else
    {
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo( st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString sqlite_display_nm = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(sqlite_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
            emit signal_statusbar_progress_bar_show_hide(false , QString(""));

            return;
        }

        QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;

        sqlite_viewer_starter *sqlite_viewer_starter_obj;
        sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
        sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
        sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
        sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
        sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
        sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
        sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);

        sqlite_viewer_starter_obj->pub_set_essentials();

        sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_send_to_bucket_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(check_entry_on_row_is_directory(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
            message_dialog_obj->show();
            continue;
        }

        if(!recon_static_functions::check_is_it_plist_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not plist file."));
            message_dialog_obj->show();
            continue;
        }
        else
        {
            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString plist_display_nm = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

            QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
            if(list_plist_display_name.contains(plist_display_nm))
            {
                continue;
            }

            plist_viewer *plist_viewer_obj;
            plist_viewer_obj = new plist_viewer(this);
            QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;

            plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
            plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
            plist_viewer_obj->pub_set_plist_path(complete_path);
            plist_viewer_obj->pub_set_source_count_name(source_count_name);
            plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            plist_viewer_obj->pub_set_essentials();

            plist_viewer_obj->pub_start_plist_extraction();

            emit signal_viewer_extarction_completed_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_send_to_bucket_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;


        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

            emit signal_statusbar_progress_bar_show_hide(false , QString(""));

            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
            message_dialog_obj->show();
            return;
        }

        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString hex_display_nm = display_file_name + " [" + pluginName + " " + record_no + "]" +src_cnt_name_label;

        QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_hex_display_name.contains(hex_display_nm))
        {
            emit signal_statusbar_progress_bar_show_hide(false , QString(""));
            return;
        }



        ///check file in bucket
        struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;

        recon_static_functions::clear_variables_hex_viewer_open_file_data(hex_viewer_open_file_data_obj);

        QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

        hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
        hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;
        hex_viewer_open_file_data_obj.complete_file_path  = complete_path;
        hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
        hex_viewer_open_file_data_obj.record_no_str       = record_no;
        hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
        hex_viewer_open_file_data_obj.plugin_name_str     = pluginName;
        hex_viewer_open_file_data_obj.plugin_tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
        hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

        recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

        emit signal_viewer_extarction_completed_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer);
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void file_system::action_send_to_bucket_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(check_entry_on_row_is_directory(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
            message_dialog_obj->show();
            continue;
        }

        if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" + info.fileName() + "' is not sqlite file");
            message_dialog_obj->show();
            continue;
        }
        else
        {
            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString sqlite_display_nm = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

            QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
            if(list_sqlite_display_name.contains(sqlite_display_nm))
            {
                continue;
            }

            QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;

            sqlite_viewer_starter *sqlite_viewer_starter_obj;
            sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
            sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
            sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
            sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
            sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
            sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
            sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            sqlite_viewer_starter_obj->pub_set_essentials();

            sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

            emit signal_viewer_extarction_completed_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_send_to_bucket_registry_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait....");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(check_entry_on_row_is_directory(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File does not exist");
            message_dialog_obj->show();
            continue;
        }

        if(!recon_static_functions::check_is_it_registry_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not a registry file."));
            message_dialog_obj->show();
            continue;
        }
        else
        {
            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString display_tab_name_str = info.fileName() + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

            QString command = "select display_tab_name from 'tbl_registry_viewer_index'";
            QStringList list_registry_display_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,reg_sqlite_path,Q_FUNC_INFO);
            if(list_registry_display_name.contains(display_tab_name_str))
            {
                continue;
            }

            QString display_tab_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;
            QString reg_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString();
            QString destination_db_path = reg_dir_path + source_count_name;
            QDir dir;
            dir.mkpath(destination_db_path);

            QString tm_file_name = info.fileName();

            if(tm_file_name.contains(" "))
                tm_file_name.replace(" " , "_");
            if(tm_file_name.contains("."))
                tm_file_name.replace("." , "_");


            QString sqlite_name =  tm_file_name + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") + "_" + record_no + ".sqlite";
            destination_db_path = destination_db_path + "/" + sqlite_name;



            ////----do not change order of calling
            registry_viewer *registry_viewer_obj;
            registry_viewer_obj = new registry_viewer(this);
            registry_viewer_obj->pub_set_destination_db_path(destination_db_path);
            registry_viewer_obj->pub_set_case_tree_tab_name(display_tab_name);
            registry_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            registry_viewer_obj->pub_set_source_file_path(complete_path);
            registry_viewer_obj->pub_set_source_count_name(source_count_name);
            registry_viewer_obj->pub_set_registry_destination_path(reg_sqlite_path);
            registry_viewer_obj->pub_set_essentials();

            registry_viewer_obj->pub_parse_windows_registry();

            if(!QFile::exists(destination_db_path))
            {
                message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("Unable to open Registry File ") +  "'" +  info.fileName() + "'");
                message_dialog_obj->show();
                continue;
            }
            emit signal_viewer_extarction_completed_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_send_to_bucket_log_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(check_entry_on_row_is_directory(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString log_file_complete_path = st_globl_recon_file_info_obj.complete_file_path;

        QFileInfo info(log_file_complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>", Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
            message_dialog_obj->show();
            continue;
        }

        QString temp_p = log_file_complete_path;
        temp_p = temp_p.toLower();
        if(!temp_p.endsWith(".log"))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not log file."));
            message_dialog_obj->show();
            continue;
        }
        else
        {
            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString log_display_name = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

            QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(log_viewer_sqlite_path, Q_FUNC_INFO);
            if(list_log_display_name.contains(log_display_name))
            {
                continue;
            }

            log_viewer *log_viewer_obj;
            log_viewer_obj = new log_viewer(this);
            QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;

            log_viewer_obj->pub_set_result_log_viewer_sqlite_path(log_viewer_sqlite_path);
            log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
            log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
            log_viewer_obj->pub_set_source_count_name(source_count_name);
            log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            log_viewer_obj->pub_set_essentials();
            log_viewer_obj->pub_start_log_extraction();

            emit signal_viewer_extarction_completed_table_result(log_display_name,MACRO_CASE_TREE_Log_Viewer);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_with_log_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QString adhoc_log_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

    int selected_row_no = m_file_system_tablewidget_obj->currentRow();

    if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
        return;

    if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
        return;

    if(check_entry_on_row_is_directory(selected_row_no))
        return;

    QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString log_file_complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(log_file_complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File does not exist");
        message_dialog_obj->show();
        return;
    }

    QString temp_p = log_file_complete_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".log"))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not a log file."));
        message_dialog_obj->show();
        return;
    }
    else
    {
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString log_display_name = display_file_name + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

        QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(adhoc_log_sqlite_path, Q_FUNC_INFO);

        if(list_log_display_name.contains(log_display_name))
        {
            emit signal_show_file_in_viewer_table_result(log_display_name,MACRO_CASE_TREE_Log_Viewer, source_count_name);
            emit signal_statusbar_progress_bar_show_hide(false , QString(""));
            return;
        }

        log_viewer *log_viewer_obj;
        log_viewer_obj = new log_viewer(this);
        QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;

        log_viewer_obj->pub_set_result_log_viewer_sqlite_path(adhoc_log_sqlite_path);
        log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
        log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
        log_viewer_obj->pub_set_source_count_name(source_count_name);
        log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        log_viewer_obj->pub_set_essentials();

        log_viewer_obj->pub_start_log_extraction();

        emit signal_show_file_in_viewer_table_result(log_display_name,MACRO_CASE_TREE_Log_Viewer, source_count_name);

    }

    emit signal_statusbar_progress_bar_show_hide(false, QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_with_registry_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait....");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString adhoc_reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(check_entry_on_row_is_directory(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File does not exist");
            message_dialog_obj->show();
            continue;
        }

        if(!recon_static_functions::check_is_it_registry_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not a registry file."));
            message_dialog_obj->show();
            continue;
        }
        else
        {

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString display_tab_name_str = info.fileName() + " [" + pluginName + " " + record_no + "]" + src_cnt_name_label;

            QString command = "select display_tab_name from 'tbl_registry_viewer_index'";

            //// if file alread added
            QString reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
            QStringList list_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,reg_sqlite_path,Q_FUNC_INFO);
            if(list_name.contains(display_tab_name_str))
            {
                emit signal_show_file_in_viewer_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer, source_count_name);
                emit signal_statusbar_progress_bar_show_hide(false , QString(""));
                continue;
            }

            //// if file not added
            QStringList list_plist_display_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,adhoc_reg_sqlite_path,Q_FUNC_INFO);
            if(list_plist_display_name.contains(display_tab_name_str))
            {
                emit signal_show_file_in_viewer_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer, source_count_name);
                emit signal_statusbar_progress_bar_show_hide(false , QString(""));
                continue;
            }

            QString display_tab_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + pluginName + MACRO_RECON_Splitter_1_multiple + table_name + MACRO_RECON_Splitter_1_multiple + record_no;
            QString reg_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString();

            QString destination_db_path_str = reg_dir_path + source_count_name;
            QDir dir;
            dir.mkpath(destination_db_path_str);

            QString tm_file_name = info.fileName();

            if(tm_file_name.contains(" "))
                tm_file_name.replace(" " , "_");
            if(tm_file_name.contains("."))
                tm_file_name.replace("." , "_");


            QString sqlite_name =  tm_file_name + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") + "_" + record_no + ".sqlite";
            destination_db_path_str = destination_db_path_str + "/" + sqlite_name;



            ////----do not change order of calling
            registry_viewer *registry_viewer_obj;
            registry_viewer_obj = new registry_viewer(this);
            registry_viewer_obj->pub_set_destination_db_path(destination_db_path_str);
            registry_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            registry_viewer_obj->pub_set_case_tree_tab_name(display_tab_name);
            registry_viewer_obj->pub_set_source_file_path(complete_path);
            registry_viewer_obj->pub_set_source_count_name(source_count_name);
            registry_viewer_obj->pub_set_registry_destination_path(adhoc_reg_sqlite_path);
            registry_viewer_obj->pub_set_essentials();

            registry_viewer_obj->pub_parse_windows_registry();

            if(!QFile::exists(destination_db_path_str))
            {
                message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("Unable to open Registry File ")+  "'" +  info.fileName() + "'");
                message_dialog_obj->show();
                continue;
            }

            emit signal_show_file_in_viewer_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer, source_count_name);

        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_mark_as_seen_unseen(QString current_action_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    //   emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        QString update_seen_str;
        QString command = QString("Update '") + table_name +  QString("' Set is_seen = ? where INT = ?");

        QStringList value_list;
        if(current_action_text == QString(MACRO_Generic_Right_Click_Mark_As_Unseen))
        {
            update_seen_str = QString::number(0);  // set as unseen
            value_list << update_seen_str << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list,db_path_filesystem, Q_FUNC_INFO);

            change_textcolor_for_seen_unseen(selected_row_no , true); // set default black color for text
            m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_SEEN)->setCheckState(Qt::Unchecked);

        }
        else
        {
            update_seen_str = QString::number(1);  // set as seen
            value_list << update_seen_str << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list, db_path_filesystem, Q_FUNC_INFO);

            change_textcolor_for_seen_unseen(selected_row_no , false); // change text color
            m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_SEEN)->setCheckState(Qt::Checked);
        }

    }

    //   emit signal_statusbar_progress_bar_show_hide(false , QString(""));
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void file_system::loading_display_dialog_based_on_records(qint64 total_records, qint64 processed_count)
{
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    if(processed_count % 100 == 0)
    {
        qint64 temp_count = processed_count;
        float record_percent = ((temp_count*100)/total_records);
        display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));
    }


}

void file_system::change_textcolor_for_seen_unseen(qint64 current_row , bool is_default_color)
{
    for(int column = 0; column < m_file_system_tablewidget_obj->columnCount(); column++)
    {
        if(m_file_system_tablewidget_obj->item(current_row , column) == NULL)
            continue;

        if(is_default_color)
            m_file_system_tablewidget_obj->item(current_row , column)->setForeground(Qt::black);
        else
            m_file_system_tablewidget_obj->item(current_row , column)->setForeground(Qt::blue);
    }
}

void file_system::change_textcolor_for_compression_decompression(qint64 current_row , bool bool_change_color)
{
    for(int column = 0; column < m_file_system_tablewidget_obj->columnCount(); column++)
    {
        if(m_file_system_tablewidget_obj->item(current_row , column) == NULL)
            continue;

        if(bool_change_color)
            m_file_system_tablewidget_obj->item(current_row , column)->setForeground(Qt::red);
        else
            m_file_system_tablewidget_obj->item(current_row , column)->setForeground(Qt::black);
    }
}

void file_system::action_copy_to_clipboard()
{
    recon_static_functions::copy_tablewidget_data_to_clipboard(m_file_system_tablewidget_obj, Q_FUNC_INFO);
}

void file_system::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;

}

void file_system::slot_sub_menu_add_files_to_hashset_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == MACRO_Generic_Right_Click_Create_Hashset)
        return;

    QString dbpath = current_clicked_action->toolTip();

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(check_entry_on_row_is_directory(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        QString bookmark;
        if(m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_BOOKMARK)->checkState())
            bookmark = "1";

        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
            message_dialog_obj->show();
            return;
        }

        hashset_storage_obj->insert_file_details_into_hashset_database(complete_path,dbpath, source_count_name);
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void file_system::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    hashset_storage_obj->hide();

    ///Adding Menu's Action
    for(int i = 0; i < list_dbs_addmenu.size() ; i++)
    {
        QStringList list = list_dbs_addmenu.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
        if(list.size() != 2)
            continue;

        QString db_name = list.at(0);
        QString db_path = list.at(1);

        if(recon_static_functions::check_existance_of_action_in_menu(submenu_add_files_to_hashset_db, db_name, db_path))
            continue;


        if(callfrom_str == MACRO_Hashset_New_Hash_Created)
        {
            QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

            for(int i = 0; i < selection_model_list.size(); i++)
            {
                if(i % 500 == 0)
                    QCoreApplication::processEvents();

                QModelIndex model_index = selection_model_list.at(i);

                int selected_row_no = model_index.row();

                if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
                    continue;

                if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
                    continue;

                if(check_entry_on_row_is_directory(selected_row_no))
                    continue;


                QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

                QString bookmark;
                if(m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_BOOKMARK)->checkState())
                    bookmark = "1";

                QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

                QFileInfo info(complete_path);
                if(!info.exists())
                {
                    message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
                    message_dialog_obj->show();
                    continue;
                }

                hashset_storage_obj->insert_file_details_into_hashset_database(complete_path,db_path, source_count_name);
            }

            emit signal_update_hashset_submenu_from_file_system(db_name,db_path);
        }
    }


    hashset_storage_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_show_add_file_to_hashset()
{
    hashset_storage_obj->show();
}

void file_system::slot_sub_menu_tags_clicked(QAction* current_clicked_action)
{
    action_submenu_tag_triggered(current_clicked_action->text());
}

void file_system::action_submenu_tag_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;
    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_tag_notes_db_updation> list_st_tag_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        selected_record_num_list << record ;


        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "1";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.notes = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_NOTES_VALUE)->text().trimmed();
        obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();

        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_name, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_ICON)->setIcon(icon);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_ICON)->setToolTip(tag_name);

        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_VALUE)->setText(tag_name);


        list_st_tag_item_list.append(obj);

    }


    if(list_st_tag_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    global_connection_manager_obj->pub_set_tag_in_fs_source_db(selected_record_num_list, source_count_name, tag_name);
    global_connection_manager_obj->pub_add_tag_for_file_system(list_st_tag_item_list, tag_name);

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_show_tag()
{
    add_tags_module_obj->show();
}

void file_system::slot_action_remove_tags_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_tag_notes_db_updation> list_st_tag_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        selected_record_num_list << record ;


        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "1";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.notes = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_NOTES_VALUE)->text().trimmed();
        obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();

        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();

        QIcon icon(QString(""));

        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_ICON)->setIcon(icon);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_ICON)->setToolTip("");

        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_VALUE)->setText("");


        list_st_tag_item_list.append(obj);

    }


    if(list_st_tag_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    global_connection_manager_obj->pub_remove_tag_in_fs_source_db(selected_record_num_list, source_count_name);
    global_connection_manager_obj->pub_delete_tag_for_file_system(list_st_tag_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void file_system::slot_action_add_tags_clicked(QString tag_data , QString colour)
{
    update_tag_status_for_current_record(tag_data,colour);
}

void file_system::slot_sub_menu_open_detailed_info_clicked(QAction *current_clicked_action)
{
}
void file_system::slot_abouttoshow_tags_in_submenu()
{
    display_tags_submenu();
}

void file_system::slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

        emit signal_update_tag_submenu_of_plugins(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_tag_list_after_remove_multiple_tag(QStringList list_tags_remained, QString table_name, QString record)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int current_row = m_file_system_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;


    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();

    QString tags_str = list_tags_remained.join(",");

    QString command = "update '" + table_name + "' set recon_tag_value=? where INT " + "=?";
    QStringList arg_list;
    arg_list << tags_str << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path_filesystem,Q_FUNC_INFO);

    if(tags_str == "")
    {
        QIcon icon(QString(""));
        m_file_system_tablewidget_obj->item(current_row,enum_File_System_display_TAG_ICON)->setIcon(icon);
    }
    else if(tags_str.contains(","))
    {
        QString icon_path = "../icons/tag_icons/multiple.png";
        QIcon icon(icon_path);
        m_file_system_tablewidget_obj->item(current_row,enum_File_System_display_TAG_ICON)->setIcon(icon);
    }
    else if((tags_str != "") && (!tags_str.contains(",")))
    {
        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags_str,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
        color = color.toLower();
        color = color.replace(" ","_");
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(color, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_file_system_tablewidget_obj->item(current_row,enum_File_System_display_TAG_ICON)->setIcon(icon);
    }

    emit signal_update_other_display_and_db_for_remained_tags(pluginName,QString(MACRO_Plugin_File_System_TAB_Name_Files), record_no,list_tags_remained,os_scheme_display);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_in_detach_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    //    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();

    //    QString command = "select filepath from '" + table_name +  "' where INT = ?";
    //    QStringList arg_list;
    //    arg_list << record_no;
    //    QString complete_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,db_path_filesystem,Q_FUNC_INFO);

    //    if(os_naming_scheme == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
    //    {
    //        QString idpath = recon_static_functions::get_fs_complete_id_file_path(fs_extraction_path, complete_path,db_path_filesystem);
    //        QString link_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/Links";
    //        QDir dir(link_dir_path);
    //        if(!dir.exists())
    //        {
    //            dir.mkdir(link_dir_path);
    //        }


    //        QFileInfo info(idpath);
    //        QString current_media_linked_file_ios = link_dir_path + "/" + info.fileName();
    //        QString suffix = recon_static_functions::mining_suffix(complete_path, Q_FUNC_INFO);
    //        if(!suffix.trimmed().isEmpty())
    //        {
    //            current_media_linked_file_ios +=  "." + suffix;
    //        }

    //        QFile::remove(current_media_linked_file_ios);
    //        QFile::link(idpath,current_media_linked_file_ios);

    //        int preview_index_full = get_preview_index_for_current_record(current_media_linked_file_ios);

    //        struct_global_fetch_metadata  struct_metadata_values_obj = global_recon_555_helper_singular_class_obj->fetch_file_system_metadata_for_current_record(record_no, source_count_name, Q_FUNC_INFO);
    //        QString metadata_str = struct_metadata_values_obj.metadata;

    //        if(!idpath.isEmpty())
    //        {
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_linked_file_ios, current_media_linked_file_ios, preview_index_full,QString(""),source_count_name);
    //        }
    //        else
    //        {
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, QString(""), QString(""), preview_index_full,QString(""),source_count_name);
    //        }
    //    }
    //    else
    //    {
    //        complete_path = recon_static_functions::get_complete_file_path(fs_extraction_path, complete_path, Q_FUNC_INFO);

    //        int preview_index_full = get_preview_index_for_current_record(complete_path);

    //        struct_global_fetch_metadata  struct_metadata_values_obj = global_recon_555_helper_singular_class_obj->fetch_file_system_metadata_for_current_record(record_no, source_count_name, Q_FUNC_INFO);
    //        QString metadata_str = struct_metadata_values_obj.metadata;

    //        if(!complete_path.isEmpty())
    //        {
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, complete_path, complete_path, preview_index_full,QString(""),source_count_name);
    //        }
    //        else
    //        {
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, QString(""), QString(""), preview_index_full,QString(""),source_count_name);
    //        }
    //    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::action_open_in_full_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no ,db_path_filesystem,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QString display_file_path = st_globl_recon_file_info_obj.display_file_path;

    int preview_index = get_preview_index_for_current_record(complete_path,display_file_path);

    struct_global_fetch_metadata  struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no, source_count_name, Q_FUNC_INFO);
    QString metadata_str = struct_metadata_values_obj.metadata;


    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    connect(full_detail_info_obj, SIGNAL(signal_add_saved_maps_in_case_tree()), this, SIGNAL(signal_add_saved_maps_in_case_tree()));
    connect(full_detail_info_obj, SIGNAL(signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString, QString)), this, SLOT(slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString, QString)));


    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    full_detail_info_obj->pub_show_full_detail_info(metadata_str,"",preview_index,QString(""),"", record_no, MACRO_Plugin_Name_File_System, "");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

int file_system::get_preview_index_for_current_record(QString current_media_path,QString display_file_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int preview_index_full;
    preview_index_full = enum_global_preview_stack_widget_BLANK;

    QString mime_type_for_file = mime_database_obj.mimeTypeForFile(current_media_path).name();

    if(recon_helper_standard_obj->check_is_it_image_file(current_media_path,display_file_path, Q_FUNC_INFO))
    {
        preview_index_full = enum_global_preview_stack_widget_IMAGE;
    }
    else if(mime_type_for_file.startsWith("video"))
    {
        preview_index_full = enum_global_preview_stack_widget_Audio_Video;
    }


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return preview_index_full;
}


void file_system::slot_sorting_tablewidget_display(int column_index)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(column_index != enum_File_System_display_BOOKMARK && column_index != enum_File_System_display_NOTES_ICON && column_index != enum_File_System_display_TAG_ICON
            && column_index != enum_File_System_display_SEEN)
    {
        m_file_system_tablewidget_obj->sortByColumn(column_index,Qt::AscendingOrder);
        m_file_system_tablewidget_obj->setSortingEnabled(false);
        return;
    }

    emit signal_fs_refresh_widgets();

    m_file_system_tablewidget_obj->clearSelection();

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    if(m_file_system_tablewidget_obj->rowCount() <= 1)
        return;

    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_file_system_tablewidget_obj->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < m_file_system_tablewidget_obj->rowCount(); row++)
        {
            if(row % 50 == 0)
                QCoreApplication::processEvents();


            QIcon icon_set;
            if(column_index != enum_File_System_display_BOOKMARK)
                icon_set =  m_file_system_tablewidget_obj->item(row, column_index)->icon();

            if(m_file_system_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column_index != enum_File_System_display_BOOKMARK)
                    icon_shiftrow_acs =  m_file_system_tablewidget_obj->item(row_shift_pos_ascending, column_index)->icon();

                if(m_file_system_tablewidget_obj->item(row_shift_pos_ascending, column_index)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_file_system_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_file_system_tablewidget_obj->takeRow(destRow);

                m_file_system_tablewidget_obj->setRow(destRow, sourceItems);

                m_file_system_tablewidget_obj->setRow(sourceRow, destItems);
                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(row % 50 == 0)
                QCoreApplication::processEvents();

            QIcon icon_set;
            if(column_index != enum_File_System_display_BOOKMARK)
                icon_set =  m_file_system_tablewidget_obj->item(row, column_index)->icon();

            if(m_file_system_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column_index != enum_File_System_display_BOOKMARK)
                    icon_shiftrow_desc =  m_file_system_tablewidget_obj->item(row_shift_pos_descending, column_index)->icon();

                if(m_file_system_tablewidget_obj->item(row_shift_pos_descending, column_index)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_file_system_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_file_system_tablewidget_obj->takeRow(destRow);

                m_file_system_tablewidget_obj->setRow(destRow, sourceItems);
                m_file_system_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;

            }
        }

        bool_sort_ascending = true;
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
    return;
}

void file_system::slot_ctrl_e_key_press_on_tablewidget()
{
    action_export_triggered();
}

void file_system::slot_ctrl_a_key_press_on_tablewidget()
{
    //m_file_system_tablewidget_obj->selectAll();

    //    for(int i = 0 ; i < m_file_system_tablewidget_obj->rowCount(); i++)
    //    {
    //        if(!m_file_system_tablewidget_obj->isRowHidden(i))
    //            m_file_system_tablewidget_obj->selectRow(i);
    //    }


}

void file_system::slot_spacebar_quicklook_key_pressed_on_tablewidget()
{
    slot_act_quick_look_triggered();

}

void file_system::slot_removed_tags_list_file_system(QStringList list_removed_tags, QString table_name, QString record_no)
{
    emit signal_file_system_removed_tags_list(pluginName, QString(MACRO_Plugin_File_System_TAB_Name_Files),record_no,list_removed_tags,os_scheme_display);
}

void file_system::slot_update_tags_in_actions_menu_without_signal_emit(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_update_hashset_submenu_of_file_system(QString db_name, QString db_path)
{
    if(submenu_add_files_to_hashset_db == NULL)
        return;

    QAction *act_add_file = new QAction(tr(db_name.toLocal8Bit().data()), this->submenu_add_files_to_hashset_db);
    act_add_file->setToolTip(db_path);

    submenu_add_files_to_hashset_db->addAction(act_add_file);

}

void file_system::slot_go_to_file_system_record(QString filepath , QString record, QString source_c_name)
{

    recon_static_functions::app_debug(" filepath - " + filepath, Q_FUNC_INFO);
    recon_static_functions::app_debug(" record - " + record, Q_FUNC_INFO);
    recon_static_functions::app_debug(" source_c_name - " + source_c_name, Q_FUNC_INFO);

    if(source_c_name != source_count_name)
        return;

    main_tab_widget_obj->setCurrentIndex(0);

    QString dir_path;

    if(record.trimmed().isEmpty())
    {
        QString command = "SELECT INT, file_parent_path FROM files WHERE filepath = ?";
        QStringList arg_list;
        arg_list << filepath;
        record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,db_path_filesystem,Q_FUNC_INFO);

        dir_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,1,db_path_filesystem,Q_FUNC_INFO);

        if(dir_path.trimmed().isEmpty())
        {
            recon_static_functions::app_debug(" ---FAILED--- BLANK DIR PATH FOUND for file : " + filepath, Q_FUNC_INFO);

        }
        make_display_for_specified_path(dir_path);

    }


    int select_row = 0;

    bool row_found_in_existin_display = false;

    for(int row = 0; row < m_file_system_tablewidget_obj->rowCount(); row++)
    {
        QString INT_str = m_file_system_tablewidget_obj->item(row,enum_File_System_display_INT)->text();

        if(INT_str == record)
        {
            row_found_in_existin_display = true;
            select_row = row;
            break;
        }
    }

    if(row_found_in_existin_display)
    {
        m_file_system_tablewidget_obj->blockSignals(true);
        m_file_system_tablewidget_obj->selectRow(select_row);
        m_file_system_tablewidget_obj->blockSignals(false);


        QModelIndex index = m_file_system_tablewidget_obj->model()->index(select_row, enum_File_System_display_INT);
        m_file_system_tablewidget_obj->scrollTo(index, QAbstractItemView::PositionAtCenter);
    }


}

void file_system::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();


    //    /// hide every row first
    //    for(int row = 0; row < m_file_system_tablewidget_obj->rowCount(); row++)
    //    {
    //        if(row % 50 == 0)
    //            QCoreApplication::processEvents();

    //        if(bool_cancel_loading)
    //            break;

    //        m_file_system_tablewidget_obj->hideRow(row);
    //    }

    QString keyword = lineEdit_search->text().trimmed().toLower();


    QList<QAction *> menu_actions_list = menu_search_filters->actions();

    if(menu_actions_list.isEmpty())
    {
        display_loading_progress_bar_obj->hide();
        return;
    }

    st_gallery_view_file_info_list.clear();

    for(int row = 0; row < m_file_system_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool bool_match_found = false;

        for(int count = 0; count < menu_actions_list.size(); count++)
        {
            if(bool_cancel_loading)
                break;


            if(!menu_actions_list.at(count)->isChecked())
                continue;

            qint64 header_index = get_header_column_index(menu_actions_list.at(count)->text());

            if(header_index <= 0)
                continue;

            if(m_file_system_tablewidget_obj->item(row , header_index)->text().trimmed().toLower().contains(keyword))
            {
                bool_match_found = true;

                QString full_filepath;
                QString thumbnail_full_filepath;
                m_file_system_tablewidget_obj->showRow(row);

                QTableWidgetItem *item_filename = m_file_system_tablewidget_obj->item(row, enum_File_System_display_FILENAME);
                QTableWidgetItem *item_filepath = m_file_system_tablewidget_obj->item(row, enum_File_System_display_FILEPATH);
                QTableWidgetItem *item_bk = m_file_system_tablewidget_obj->item(row, enum_File_System_display_BOOKMARK);
                QTableWidgetItem *item_INT = m_file_system_tablewidget_obj->item(row, enum_File_System_display_INT);
                QTableWidgetItem *item_date_last_read = m_file_system_tablewidget_obj->item(row, enum_File_System_display_DATE_ACCESSED);


                if(recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
                {
                    bool bool_is_it_file = false;

                    if(os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                            || os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                    {
                        QStringList list_ios_path;
                        list_ios_path << item_filepath->text();
                        QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference("select id_path from files where filepath LIKE ?" , list_ios_path, 0, destination_db_obj, Q_FUNC_INFO);
                        if(!idpath.isEmpty())
                        {
                            full_filepath = recon_static_functions::get_complete_file_path(fs_extraction_path, idpath, Q_FUNC_INFO);
                            bool_is_it_file = true;
                        }
                    }
                    else
                    {
                        QString fs_path = fs_extraction_path;
                        full_filepath = recon_static_functions::get_complete_file_path(fs_path, item_filepath->text(), Q_FUNC_INFO);
                        bool_is_it_file = QFileInfo(full_filepath).isFile();
                    }

                    if(!bool_is_it_file)
                        break;

                    struct_global_gallery_view_file_info f1;
                    f1.file_name = item_filename->text();
                    f1.file_path = item_filepath->text();
                    f1.thumbnail_file_path = full_filepath;
                    f1.full_filepath = full_filepath;
                    f1.INT = item_INT->text();
                    f1.table_view_row_no = row;
                    f1.bookmark = "0";
                    f1.latitude = "";
                    f1.longitude = "";
                    f1.last_access_timestamp = item_date_last_read->text();
                    f1.source_count_name = source_count_name;
                    f1.index = st_gallery_view_file_info_list.size();

                    if(item_bk->checkState() == Qt::Checked)
                        f1.bookmark = "1";

                    st_gallery_view_file_info_list.append(f1);
                }
                else if(recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(item_filename->text()))
                {
                    QString file_size = m_file_system_tablewidget_obj->item(row, enum_File_System_display_DATE_ACCESSED)->text();
                    bool bool_is_it_file = false;

                    if(os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                            || os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                    {
                        QStringList list_ios_path;
                        list_ios_path << item_filepath->text();
                        QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference("select id_path from files where filepath LIKE ?" , list_ios_path, 0, destination_db_obj, Q_FUNC_INFO);
                        if(!idpath.isEmpty())
                        {
                            full_filepath = recon_static_functions::get_complete_file_path(fs_extraction_path, idpath, Q_FUNC_INFO);
                            bool_is_it_file = true;
                        }
                    }
                    else
                    {
                        QString fs_path = fs_extraction_path;
                        full_filepath = recon_static_functions::get_complete_file_path(fs_path, item_filepath->text(), Q_FUNC_INFO);
                        bool_is_it_file = QFileInfo(full_filepath).isFile();
                    }

                    if(!bool_is_it_file)
                        break;

                    QString file_name = item_filename->text();

                    QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

                    QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";

                    QString fs_record_no = item_INT->text();

                    QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
                    QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                    QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

                    if(!thumb_file_path.isEmpty())
                    {
                        thumbnail_full_filepath = result_filepath + thumb_file_path;
                    }
                    else
                    {
                        file_name.remove(file_name.lastIndexOf("."),file_name.size());
                        video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

                        //                        QString script_path = "../Resources/scripts_persistent/video_frame.py";
                        //                        QString cmd_python_path = global_varibale_python_path;

                        //                        QStringList args;
                        //                        args << script_path << full_filepath << video_thumbnails_path << file_size;

                        QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                        QStringList args;
                        args << full_filepath << video_thumbnails_path << file_size;

                        QProcess process_thumbnails;
                        //                        process_thumbnails.start(cmd_python_path,args);
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

                        thumbnail_full_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();
                        QString thumb_filepath;
                        QStringList list;
                        QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                        QFileInfo file_inf(thumbnail_full_filepath);
                        if(file_inf.exists())
                        {
                            thumb_filepath.clear();
                            thumb_filepath = thumbnail_full_filepath;
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
                    f1.thumbnail_file_path = thumbnail_full_filepath;
                    f1.full_filepath = full_filepath;
                    f1.INT = item_INT->text();
                    f1.table_view_row_no = row;
                    f1.bookmark = "0";
                    f1.latitude = "";
                    f1.longitude = "";
                    f1.last_access_timestamp = item_date_last_read->text();
                    f1.source_count_name = source_count_name;
                    f1.index = st_gallery_view_file_info_list.size();

                    if(item_bk->checkState() == Qt::Checked)
                        f1.bookmark = "1";

                    st_gallery_view_file_info_list.append(f1);

                }
                break;
            }
        }

        if(!bool_match_found)
            m_file_system_tablewidget_obj->hideRow(row);

    }

    bool_did_tableview_display_changed = true;

    emit signal_clear_all_metadata();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void file_system::slot_pushbutton_showall_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    lineEdit_search->clear();

    emit signal_clear_all_metadata();

    m_file_system_tablewidget_obj->setRowCount(0);
    set_fs_display(main_db_cmd , current_parent_dir_path , false, false);


    pushbutton_show_files_recursively->setEnabled(true);

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void file_system::slot_fs_double_clicked_dir_extraction_completed(QString dir_path)
{
    set_fs_display(main_db_cmd , dir_path , true, false);
}

void file_system::slot_dir_navigation_button_hover_enter_or_leave(bool bool_hover_enter_or_leave, m_hovered_pushbutton_with_signal *dir_button)
{
    int text_width = dir_button->fontMetrics().size(Qt::TextShowMnemonic, dir_button->text()).width();
    // int icon_width = dir_button->icon().availableSizes().first().width();

    //    int total_width = icon_width + text_width + 6;
    //    if(bool_hover_enter_or_leave)
    //        dir_button->setMinimumWidth(total_width);
    //    else
    //        dir_button->setMinimumWidth(icon_width);


    int total_width = text_width + 2;
    dir_button->setMinimumWidth(total_width);

}

void file_system::slot_dir_navigator_pushbutton_clicked(QString dir_path)
{
    if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW)
        return;

    if(dir_path.endsWith("/") && dir_path != QString("/"))
        dir_path.chop(1);


    if(dir_path.trimmed().isEmpty())
        return;

    QString file_path_str;
    if(struct_target_source_info.bool_source_nested)
        file_path_str = dir_path;
    else
        file_path_str = global_recon_helper_singular_class_obj->get_complete_file_path(struct_target_source_info.fs_extraction_source_path, dir_path, Q_FUNC_INFO);

    if(!deepest_path.contains(file_path_str))
    {
        deepest_path = file_path_str.trimmed();
    }

    current_parent_dir_path = file_path_str;

    make_display_for_specified_path(file_path_str);

}

void file_system::slot_sub_menu_open_file_with_other_app_clicked(QAction* current_clicked_action)
{

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            continue;
        }

        recon_static_functions::open_file_with_specified_app(complete_path , current_clicked_action->toolTip(), Q_FUNC_INFO);

        // show only first file (Multiselection not supported).
        break;

    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}


void file_system::slot_main_tab_widget_current_changed(int index)
{

    emit signal_clear_all_metadata();

    bool_cell_changed_connection_buildup_for_tablewidget = false;


    QCoreApplication::processEvents();
    if(index == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW && bool_did_tableview_display_changed)
    {
        m_file_system_tablewidget_obj->clearSelection();

        pushbutton_show_files_recursively->hide();
        pushbutton_export_as_csv->hide();
        pushbutton_show_all->hide();
        pushButton_search->hide();
        pushbutton_search_filters_for_non_hfs->hide();
        lineEdit_search->hide();
        pushButton_back->hide();
        pushButton_next->hide();
        pushbutton_show_column->hide();
        frame_dir_navigation->setDisabled(true);

        gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);
        QCoreApplication::processEvents();
        bool_did_tableview_display_changed = false;

    }
    else if(index == enum_File_System_Main_TabWidget_Indexes_TABLE_VIEW)
    {
        m_file_system_tablewidget_obj->clearSelection();

        pushbutton_show_files_recursively->show();
        pushbutton_export_as_csv->show();

        pushbutton_show_all->hide();
        pushButton_search->hide();
        pushbutton_search_filters_for_non_hfs->hide();
        lineEdit_search->hide();
        pushbutton_show_column->show();

        pushButton_back->show();
        pushButton_next->show();
        frame_dir_navigation->setDisabled(false);

        //tablewidget_general_clicked(m_file_system_tablewidget_obj->currentRow() , m_file_system_tablewidget_obj->currentColumn());
    }
    else if(index == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW && !bool_did_tableview_display_changed)
    {
        m_file_system_tablewidget_obj->clearSelection();

        pushbutton_show_files_recursively->hide();
        pushbutton_export_as_csv->hide();
        pushbutton_show_all->hide();
        pushButton_search->hide();
        pushbutton_search_filters_for_non_hfs->hide();
        lineEdit_search->hide();
        pushButton_back->hide();
        pushButton_next->hide();
        pushbutton_show_column->hide();
        frame_dir_navigation->setDisabled(true);

    }
}

void file_system::slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    QString dest_dir_path = export_dir_path;// = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString();

    folder_export_dir_path.clear();
    folder_export_dir_path = export_dir_path + source_count_name + "/";
    bool_folder_export_done = false;

    QStringList list_paths_to_message_box;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        QString display_path = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILEPATH)->text();
        QFileInfo a_info(display_path);
        QString parent_dir = a_info.absolutePath();
        if(bool_keep_directory_structure)
        {
            dest_dir_path = export_dir_path  + source_count_name + "/" + parent_dir + "/";
            QDir mdir;
            mdir.mkpath(dest_dir_path);
        }
        else
        {
            dest_dir_path = export_dir_path  + source_count_name + "/";
            QDir mdir;
            mdir.mkpath(dest_dir_path);
        }

        QString m_file_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text();

        struct_global_export_file_system_record obj;
        obj.record_no = record_no;
        obj.source_count_name = source_count_name;
        obj.display_file_path = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILEPATH)->text();
        obj.displayed_file_name = m_file_name;


        QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbreference(obj , destination_db_obj, Q_FUNC_INFO);
        QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_dir_path,m_file_name , Q_FUNC_INFO);
        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;

    }


    //    for(int i = 0 ; i < list_paths_to_message_box.size(); i++)
    //    {
    //        QProcess proc;

    //        QStringList args;
    //        args << "-R" << "0777" << list_paths_to_message_box.at(i);

    //        proc.execute("chmod", args);

    //        QCoreApplication::processEvents();
    //    }


    display_loading_progress_bar_obj->hide();

    if(list_paths_to_message_box.size() > 0)
    {
        bool_folder_export_done = true;
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}


void file_system::slot_on_pushbutton_clicked_export_as_csv(bool status)
{
    export_dialog_box_3_obj->pub_set_window_title("Export AS CSV");
    export_dialog_box_3_obj->show();
}

void file_system::export_as_csv_for_screen_items(QTextStream &outputfile_csv , struct_GLOBAL_witness_info_source struct_info)
{

    display_loading_progress_bar_obj->pub_set_label_messsge("Please Wait... ");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    qint64 s_number = 0;
    for (int row_count = 0 ; row_count < m_file_system_tablewidget_obj->rowCount() ; row_count++)
    {

        if(row_count % 30 == 0)
            QCoreApplication::processEvents();


        if(m_file_system_tablewidget_obj->isRowHidden(row_count))
            continue ;



        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break;
        }
        s_number++;
        display_loading_progress_bar_obj->pub_set_label_messsge("EXporting CSV... " + QString::number(s_number));




        //        outputfile_csv << "\n";
        //        if(struct_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_APFS || struct_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_HFS)
        //        {
        //            outputfile_csv << "\"" + QString::number(s_number) + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILENAME)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILEPATH)->text()+ "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() +  "\"" + ",";
        //            outputfile_csv << "\""+ recon_static_functions::human_readable_size(m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() , Q_FUNC_INFO) + "\""+ ",";

        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_MIMETYPE)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASHSET_NAME)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_MD5)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_SHA1)->text() + "\"" + ",";


        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_MODIFIED)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CREATED)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_ACCESSED)->text() + "\"" +  ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Date_Added)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Content_Creation_Date)->text() + "\""+ ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Content_Modification_Date)->text() + "\""+ ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Last_Used_Date)->text() + "\"" +",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Use_Count)->text() + "\"" + ",";


        //        }
        //        else
        //        {
        //            outputfile_csv << "\"" + QString::number(s_number) + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILENAME)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILEPATH)->text()+ "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() +  "\"" + ",";
        //            outputfile_csv << "\""+ recon_static_functions::human_readable_size(m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() , Q_FUNC_INFO) + "\""+ ",";

        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_MIMETYPE)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASHSET_NAME)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_MD5)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_SHA1)->text() + "\"" + ",";


        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_MODIFIED)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CREATED)->text() + "\"" + ",";
        //            outputfile_csv << "\""+ m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_ACCESSED)->text() + "\"" +  ",";
        //        }



        outputfile_csv << "\n";
        if(struct_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_APFS || struct_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_HFS)
        {

            export_as_csv_data_list.clear();
            hidden_column_header_data_index_list.clear();
            export_as_csv_data_list << QString::number(s_number)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_INODE_NO)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILENAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_EXTENSION)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILEPATH)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text()
                                    << recon_static_functions::human_readable_size(m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() , Q_FUNC_INFO)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_MIMETYPE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASHSET_NAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_MD5)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_SHA1)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DECOMPRESSION_STATUS)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_MODIFIED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CHANGE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_ACCESSED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CREATED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Date_Added)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Content_Creation_Date)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Content_Modification_Date)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Last_Used_Date)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Use_Count)->text();

            for(int i=0; i < export_as_csv_data_list.size(); i++)
            {
                for(int j=0; j < hidden_column_header_name_index_list.size(); j++)
                {
                    if(i == hidden_column_header_name_index_list.at(j))
                    {
                        hidden_column_header_data_index_list << i;

                    }
                }
            }

            for(int k = hidden_column_header_data_index_list.size()-1; k >= 0; k--)
            {
                export_as_csv_data_list.removeAt(hidden_column_header_data_index_list.at(k));
            }

            for(int i = 0 ; i < export_as_csv_data_list.size() ; i++)
            {
                outputfile_csv << "\"" + export_as_csv_data_list.at(i) + "\"" + ",";
            }
        }
        else
        {


            export_as_csv_data_list.clear();
            hidden_column_header_data_index_list.clear();
            export_as_csv_data_list << QString::number(s_number)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_INODE_NO)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILENAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_EXTENSION)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILEPATH)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text()
                                    << recon_static_functions::human_readable_size(m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() , Q_FUNC_INFO)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_MIMETYPE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASHSET_NAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_MD5)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_SHA1)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DECOMPRESSION_STATUS)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_MODIFIED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CHANGE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_ACCESSED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CREATED)->text();

            for(int i=0; i < export_as_csv_data_list.size(); i++)
            {
                for(int j=0; j < hidden_column_header_name_index_list.size(); j++)
                {
                    if(i == hidden_column_header_name_index_list.at(j))
                    {
                        hidden_column_header_data_index_list << i;
                    }
                }
            }
            for(int k = hidden_column_header_data_index_list.size()-1; k >= 0; k--)
            {
                export_as_csv_data_list.removeAt(hidden_column_header_data_index_list.at(k));
            }
            for(int i = 0 ; i < export_as_csv_data_list.size() ; i++)
            {
                outputfile_csv << "\"" + export_as_csv_data_list.at(i) + "\"" + ",";
            }

        }

    }



    display_loading_progress_bar_obj->hide();

}

void file_system::slot_start_write_fs_data_in_csv(QString file_path , QString size_str , bool bool_recursive_checked_for_csv_only , bool bool_radio_button_screen_item_status, QString info_file_path , QString export_as_type)
{

    export_dialog_box_3_obj->hide();

    prepare_hidden_column_list_from_db();

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QStringList csv_headerlist;

    csv_headerlist.clear();
    hidden_column_header_name_index_list.clear();
    export_as_csv_header_list.clear();

    QString file_size_bytes_column = QString("File Size (Bytes)");
    QString file_size_human_readable = QString("File Size (Units)");

    if(struct_info.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_APFS)
            || struct_info.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_HFS))
    {


        csv_headerlist  << "Sr. No"
                        << "Inode No./File ID"
                        <<"File Name"
                       << "Extension"
                       << "File Path"
                       << file_size_bytes_column
                       << file_size_human_readable
                       << "Mime Type"
                       << "Hashset Name"
                       << "MD5"
                       << "SHA1"
                       << "Decompression Status"

                       << "Date Modified"
                       << "Date Change"
                       << "Date Accessed"
                       << "Date Created"
                       << "Date Added (Apple)"
                       << "Content Creation Date (Apple)"
                       << "Content Modification Date (Apple)"
                       << "Last Used Date (Apple)"
                       << "Use Count";

    }
    else
    {

        csv_headerlist  << "Sr. No"
                        << "Inode No./File ID"
                        <<"File Name"
                       << "Extension"
                       << "File Path"
                       << file_size_bytes_column
                       << file_size_human_readable
                       << "Mime Type"
                       << "Hashset Name"
                       << "MD5"
                       << "SHA1"
                       << "Decompression Status"
                       << "Date Modified"
                       << "Date Change"
                       << "Date Accessed"
                       << "Date Created";
    }

    export_as_csv_header_list.append(csv_headerlist);

    for(int i = 0; i < csv_headerlist.size(); i++)
    {
        for(int j=0; j < hide_column_list.size(); j++)
        {
            if(csv_headerlist.at(i) == hide_column_list.at(j))
            {
                hidden_column_header_name_index_list << i;
            }
        }
    }

    for(int k = hidden_column_header_name_index_list.size()-1; k>=0; k--)
    {
        csv_headerlist.removeAt(hidden_column_header_name_index_list.at(k));
    }


    QStringList custom_col_name_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList).toStringList();
    if(!bool_radio_button_screen_item_status)
    {
        if(struct_info.os_scheme_display == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
        {
            if(custom_col_name_list.size() != 0)
                csv_headerlist << custom_col_name_list;
        }
    }

    prepare_hashset_db_data_list();
    QStringList hashsets_to_be_hidden = get_hashsets_list_to_be_hidden();

    if(export_as_type == QString(MACRO_Generic_Export_As_CSV))
        export_data_as_csv(csv_headerlist,file_path,bool_radio_button_screen_item_status,bool_recursive_checked_for_csv_only, struct_info, hashsets_to_be_hidden,info_file_path);
    if(export_as_type == QString(MACRO_Generic_Export_As_SQLite))
        export_data_as_sqlite(csv_headerlist,file_path,bool_radio_button_screen_item_status,bool_recursive_checked_for_csv_only, struct_info, hashsets_to_be_hidden,info_file_path);

}


void file_system::slot_add_file_to_text_indexing_clicked(bool status)
{
    Q_UNUSED(status);

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

    QString command = QString("insert into files (bookmark, fs_record,file_path,source_count_name,status,plugin_name) values(?,?,?,?,?,?)");

    QStringList value_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();
        QString file_path = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILEPATH)->text();
        QString source_count_name = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_SOURCE_COUNT_NAME)->text();

        value_list.clear();
        value_list << QString::number(0) << record_no << file_path << source_count_name << QString::number(0) << QString(MACRO_Plugin_Name_File_System);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void file_system::slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip,QString password_enc_zip)
{
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

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

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        struct_global_file_for_zip_export st_file_info_for_export_obj;

        QString record_no_str = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        QString filename = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();

        QString display_file_path =  m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();

        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no_str,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

        if(recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no_str,destination_db_obj,"files",source_count_name,Q_FUNC_INFO))
        {
            QString cmd_2 = "select INT from files where file_parent_path LIKE ?";
            QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(cmd_2,QStringList(QString(display_file_path  + "%")), 0, destination_db_obj, Q_FUNC_INFO);
            for(int ii = 0 ; ii < INT_list.size() ; ii++)
            {
                QString relevant_record_no = INT_list.at(ii);
                QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(relevant_record_no,destination_db_obj ,"files" ,source_count_name, Q_FUNC_INFO);
                QFileInfo m_file_info(file_path);
                if(!m_file_info.exists())
                {
                    continue ;
                }

                QString cmd = "select filepath from files where INT =" + relevant_record_no ;
                QString displ_path = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, destination_db_obj, Q_FUNC_INFO);

                st_file_info_for_export_obj.source_path = file_path;
                st_file_info_for_export_obj.destination_path = source_count_name + displ_path;

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


    display_loading_progress_bar_obj->show();
    zip_export_file_path = export_zip_file_path;
    bool_zip_export_done = false;


    thread_zip_file_export_obj->pub_set_essentials(export_zip_file_path, files_list_for_zip_export, bool_is_enc_zip, password_enc_zip);
    thread_zip_file_export_obj->start();
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void file_system::slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_file_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();

    bool_cancel_loading = false;

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    QString dest_dir_path = export_dmg_file_path;

    dmg_export_file_path.clear();

    QStringList list_paths_to_message_box;
    QString exported_file_path,parent_dir,new_folder_path;

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

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        QString display_path = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILEPATH)->text();

        QFileInfo a_info(display_path);
        parent_dir = a_info.absolutePath();
        if(bool_keep_directory_structure)
        {
            dest_dir_path = export_dmg_file_path  + source_count_name  + QString("_") + current_timestamp + "/" + parent_dir + "/"  ;

            QDir mdir;
            mdir.mkpath(dest_dir_path);
        }
        else
        {
            dest_dir_path = export_dmg_file_path  + source_count_name + QString("_") + current_timestamp + "/";
            QDir mdir;
            mdir.mkpath(dest_dir_path);
        }

        QString m_file_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text();

        struct_global_export_file_system_record obj;
        obj.record_no = record_no;
        obj.source_count_name = source_count_name;
        obj.display_file_path = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILEPATH)->text();
        obj.displayed_file_name = m_file_name;

        QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbreference(obj , destination_db_obj, Q_FUNC_INFO);
        exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_dir_path,m_file_name , Q_FUNC_INFO);

        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;
    }

    bool bool_is_image_created = false;

    QString folder_path = dest_dir_path;
    QString export_dmg_path = export_dmg_file_path + source_count_name + QString("_") + current_timestamp;
    export_dmg_path.append(".dmg");

    QString available_dmg_image  = recon_static_functions::get_available_filename(export_dmg_path,dest_dir_path, Q_FUNC_INFO);
    dmg_export_file_path = available_dmg_image;

    new_folder_path = export_dmg_file_path + source_count_name + QString("_") + current_timestamp;

    QStringList args_list_id;
    args_list_id << "create" <<  available_dmg_image << "-fs" << "apfs" << "-srcfolder" << new_folder_path;

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

    QDir dir(new_folder_path);
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

    display_loading_progress_bar_obj->hide();

    if(bool_is_image_created && QFile(available_dmg_image).exists())
    {
        bool_dmg_export_done = true;
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void file_system::slot_thread_zip_file_export_finished()
{
    display_loading_progress_bar_obj->hide();

    if(bool_cancel_loading)
        return;

    bool_zip_export_done = true;

    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
    message_dialog_obj->show();
}


void file_system::slot_sub_menu_carve_clicked(QAction *current_clicked_action)
{
    QString action = current_clicked_action->text();

    if(action == QString(MACRO_Generic_Right_Click_Carve_Files))
    {
        action_carve_files_clicked();
    }
    else if(action == QString(MACRO_Generic_Right_Click_Carve_Data))
    {
        action_carve_data_clicked();
    }
}

void file_system::action_carve_files_clicked()
{
    QString complete_file_path = get_current_complete_file_path();
    if(complete_file_path.trimmed().isEmpty())
        return;

    emit signal_carve_files_clicked(source_count_name, complete_file_path, false);
}

void file_system::action_carve_data_clicked()
{
    QString complete_file_path = get_current_complete_file_path();
    if(complete_file_path.trimmed().isEmpty())
        return;


    QString record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();

    emit signal_carve_data_clicked(source_count_name, complete_file_path, QString(MACRO_Plugin_Name_File_System), QString(MACRO_Plugin_File_System_TAB_Name_Files), record_no);
}

void file_system::slot_act_face_search_triggered(bool status)
{

    int selected_row_no = m_file_system_tablewidget_obj->currentRow();

    if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
        return;

    if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
        return;


    QString record_no_str = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

    QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,db_path_filesystem,"files",source_count_name,Q_FUNC_INFO);

    QString display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_path,source_count_name);

    bool bool_is_image = recon_helper_standard_obj->check_is_it_image_file(file_path,display_file_path,Q_FUNC_INFO);

    if(bool_is_image)
    {

        facefinder_obj->pub_clear_fields();
        bool bool_value_face = facefinder_obj-> extract_cropped_faces(file_path);

        if(bool_value_face)
            facefinder_obj->pub_prepare_display();
    }


}

void file_system::slot_sub_menu_run_fs_modules_clicked(QAction* current_clicked_action)
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

void file_system::action_run_fs_modules_apple_metadata_clicked()
{
    bool_run_fs_modules_apple_metadata_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_apple_metadata_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_apple_metadata_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Apple Metadata' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_apple_metadata_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_apple_metadata(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Apple Metadata' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_exif_metadata_clicked()
{

    bool_run_fs_modules_exif_metadata_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_exif_metadata_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_exif_metadata_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Exif Metadata' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_exif_metadata_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Exif_Metadata); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_exif_metadata(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Exif Metadata' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_signature_analysis_clicked()
{

    bool_run_fs_modules_signature_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_signature_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_signature_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Signature Analysis' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_signature_analysis_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Signature_Analysis); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_signature_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Signature' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_mime_type_analysis_clicked()
{

    bool_run_fs_modules_mime_type_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_mime_type_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_mime_type_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Mime Type Analysis' already running.");
        return;
    }

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_mime_type_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Mime Type' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_hashset_clicked()
{

    bool_run_fs_modules_hashset_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_hashset_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_hashset_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Hashes Analysis' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_hashset_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Hashes); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_hashset(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Hashes' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_face_analysis_clicked()
{

    bool_run_fs_modules_face_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_face_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_face_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Face Analysis' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_face_analysis_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_face_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Face Analysis' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_optical_character_recognition_clicked()
{

    bool_run_fs_modules_optical_character_recognition_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_optical_character_recognition_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_optical_character_recognition_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Optical Character Recognition' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_optical_character_recognition_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_optical_character_recognition(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Optical Character Recognition' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_weapons_clicked()
{

    bool_run_fs_modules_weapons_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_weapons_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_weapons_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Weapons' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_weapons_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_weapons(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Weapons' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_fire_analysis_clicked()
{

    bool_run_fs_modules_fire_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_fire_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_fire_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Fire Analysis' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_fire_analysis_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_fire_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Fire Analysis' has already been extracted.");
        return;
    }
}

void file_system::action_run_fs_modules_skin_tone_detection_clicked()
{

    bool_run_fs_modules_skin_tone_detection_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_skin_tone_detection_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_skin_tone_detection_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Skin Tone Detection' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_skin_tone_detection_run_status"), MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_skin_tone_detection(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Skin Tone Detection' has already been extracted.");
        return;
    }
}


void file_system::prepare_fs_run_module_files_list(QString status_column_name, QString feature_name)
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    run_fs_modules_details_list.clear();
    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString sou_cnt_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();

        struct_GLOBAL_witness_info_source source_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(sou_cnt_name);

        QString os_scheme_internal = source_obj.os_scheme_internal;
        QString virtual_source_path = source_obj.virtual_source_path;
        QString fs_record_no       = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();

        bool bool_is_dir = recon_helper_standard_obj->pub_check_path_is_directory_by_fs_dbreference(complete_file_path,fs_record_no, destination_db_obj, Q_FUNC_INFO);

        //FS module which run by script; Script itself recognize record is dir or file and dir is same.
        //Like ocr, weapons etc,
        //Here we pass fs module script record in if else file scope.

        if(bool_is_dir
                && (feature_name == MACRO_Generic_Right_Click_Run_Filesystem_Modules_Exif_Metadata
                    || feature_name == MACRO_Generic_Right_Click_Run_Filesystem_Modules_Signature_Analysis
                    || feature_name == MACRO_Generic_Right_Click_Run_Filesystem_Modules_Hashes
                    || feature_name == MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata
                    || feature_name == MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis))
        {
            if((os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                || os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
            {
                QString select_cmd = "SELECT INT, id_path, filepath from files WHERE filepath LIKE '" + display_filepath + "/%'";

                QStringList rec_no_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd, 0, destination_db_obj, Q_FUNC_INFO);

                QStringList id_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd, 1, destination_db_obj, Q_FUNC_INFO);

                QStringList file_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd, 2, destination_db_obj, Q_FUNC_INFO);

                for(int ii = 0; ii < rec_no_list.size(); ii++)
                {
                    QString fs_record_no = rec_no_list.at(ii);
                    QString id_path = id_path_list.at(ii);

                    QString file_path = file_path_list.at(ii);

                    QString complete_path = virtual_source_path + id_path;

                    QString filename_str  = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_path);
                    struct_global_run_fs_modules_details struct_run_fs_modules_details_obj;
                    struct_run_fs_modules_details_obj.fs_record_no       = fs_record_no;
                    struct_run_fs_modules_details_obj.display_filepath   = file_path;
                    struct_run_fs_modules_details_obj.complete_file_path = complete_path;
                    struct_run_fs_modules_details_obj.source_count_name  = source_count_name;
                    struct_run_fs_modules_details_obj.examiner_selected_file_or_dir_name = filename_str;
                    run_fs_modules_details_list.append(struct_run_fs_modules_details_obj);

                }
            }
            else
            {
                QString early_run_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select ") + status_column_name + QString(" from files where INT = ?") , QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
                ///no need to run again already ran for this file
                if(early_run_status.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                    continue;

                QString filename_str  = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_file_path);
                struct_global_run_fs_modules_details struct_run_fs_modules_details_obj;
                struct_run_fs_modules_details_obj.fs_record_no       = fs_record_no;
                struct_run_fs_modules_details_obj.display_filepath   = display_filepath;
                struct_run_fs_modules_details_obj.complete_file_path = complete_file_path;
                struct_run_fs_modules_details_obj.source_count_name  = source_count_name;
                struct_run_fs_modules_details_obj.examiner_selected_file_or_dir_name = filename_str;
                run_fs_modules_details_list.append(struct_run_fs_modules_details_obj);


                QStringList value_list;
                QString val_str = display_filepath;
                val_str.append("%");
                value_list << val_str << QString("0");

                ///Run only for files not processed
                QStringList record_no_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select INT From files Where file_parent_path like ? AND ") + status_column_name + QString(" = ?") , value_list, 0, destination_db_obj, Q_FUNC_INFO);
                if(record_no_list.isEmpty())
                    continue;

                ///get all files/directories in selected directory
                for(int count = 0; count < record_no_list.size(); count++)
                {
                    if(count % 50 == 0)
                        QCoreApplication::processEvents();

                    fs_record_no   = record_no_list.at(count);

                    QString early_run_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select ") + status_column_name + QString(" from files where INT = ?") , QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);

                    ///no need to run again already ran for this file
                    if(early_run_status.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        continue;

                    complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);


                    bool bool_is_dir_in_loop = recon_helper_standard_obj->pub_check_path_is_directory_by_fs_dbreference(complete_file_path,fs_record_no, destination_db_obj, Q_FUNC_INFO);
                    if(bool_is_dir_in_loop)
                        continue;


                    display_filepath  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select filepath From files Where INT = ?"), QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);


                    QString filename_str  = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_file_path);

                    struct_global_run_fs_modules_details struct_run_fs_modules_details_obj;

                    struct_run_fs_modules_details_obj.fs_record_no       = fs_record_no;
                    struct_run_fs_modules_details_obj.display_filepath   = display_filepath;
                    struct_run_fs_modules_details_obj.complete_file_path = complete_file_path;
                    struct_run_fs_modules_details_obj.source_count_name  = source_count_name;
                    struct_run_fs_modules_details_obj.examiner_selected_file_or_dir_name = filename_str;

                    run_fs_modules_details_list.append(struct_run_fs_modules_details_obj);
                }
            }
        }
        else
        {
            QString early_run_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select ") + status_column_name + QString(" from files where INT = ?") , QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);

            ///no need to run again already ran for this file
            if(early_run_status.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                continue;

            QString filename_str  = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_file_path);

            struct_global_run_fs_modules_details struct_run_fs_modules_details_obj;

            struct_run_fs_modules_details_obj.fs_record_no       = fs_record_no;
            struct_run_fs_modules_details_obj.display_filepath   = display_filepath;
            struct_run_fs_modules_details_obj.complete_file_path = complete_file_path;
            struct_run_fs_modules_details_obj.source_count_name  = source_count_name;
            struct_run_fs_modules_details_obj.examiner_selected_file_or_dir_name   = filename_str;

            run_fs_modules_details_list.append(struct_run_fs_modules_details_obj);
        }

    }

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void file_system::slot_r_case_apple_metadata_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_apple_metadata_status = status;
}

void file_system::slot_r_case_exif_metadata_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_exif_metadata_status = status;
}


void file_system::slot_r_case_signature_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_signature_analysis_status = status;
}

void file_system::slot_r_case_mime_type_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_mime_type_analysis_status = status;
}

void file_system::slot_r_case_hashset_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_hashset_status = status;
}

void file_system::slot_r_case_face_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_face_analysis_status = status;
}

void file_system::slot_r_case_optical_character_recognition_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_optical_character_recognition_status = status;
}

void file_system::slot_r_case_weapons_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_weapons_status = status;
}

void file_system::slot_r_case_fire_analysis_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_fire_analysis_status = status;
}

void file_system::slot_r_case_skin_tone_detection_running_status(bool status)
{
    bool_run_fs_modules_status = true;
    bool_run_fs_modules_skin_tone_detection_status = status;
}

void file_system::slot_sub_menu_export_hashes_as_vic_clicked(QAction* current_clicked_action)
{

    QString export_hashes_as = current_clicked_action->text().trimmed();
    QString vic_version;

    bool bool_any_export_type_found = false;

    if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_1))
    {
        vic_version = QString("1.1");
        bool_any_export_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_2))
    {
        vic_version = QString("1.2");
        bool_any_export_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_3))
    {
        vic_version = QString("1.3");
        bool_any_export_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_2_0))
    {
        vic_version = QString("2.0");
        bool_any_export_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_SQLite))
    {
        bool_any_export_type_found = true;
    }
    else if(export_hashes_as == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_CSV))
    {
        bool_any_export_type_found = true;
    }

    else
        bool_any_export_type_found = false;

    export_hashes_as = export_hashes_as.trimmed();

    if(export_hashes_as.isEmpty() || !bool_any_export_type_found)
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

        export_as_vic_details_list.clear();

        QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

        for(int i = 0; i < selection_model_list.size(); i++)
        {
            if(i % 50 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(i);
            int selected_row_no = model_index.row();

            if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString fs_record_no       = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
            QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
            QString display_filepath   = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();

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
                QStringList record_no_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select INT From files Where file_parent_path like ?"), value_list, 0, destination_db_obj, Q_FUNC_INFO);

                ///get all files/directories in selected directory
                for(int count = 0; count < record_no_list.size(); count++)
                {
                    if(count % 50 == 0)
                        QCoreApplication::processEvents();

                    fs_record_no       = record_no_list.at(count);
                    complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

                    ///get all files(only) in selected directory
                    if(QFileInfo(complete_file_path).isDir())
                        continue;

                    display_filepath   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select filepath From files Where INT = ?"), QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);

                    struct_global_export_as_hashes_details struct_export_vic_details_obj;

                    struct_export_vic_details_obj.fs_record_no       = fs_record_no;
                    struct_export_vic_details_obj.display_filepath   = display_filepath;
                    struct_export_vic_details_obj.display_filename   = QFileInfo(display_filepath).fileName();
                    struct_export_vic_details_obj.complete_file_path = complete_file_path;
                    struct_export_vic_details_obj.source_count_name  = source_count_name;
                    struct_export_vic_details_obj.examiner_selected_file_or_dir_name = info_cmplt.fileName();

                    struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
                    struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 1, destination_db_obj, Q_FUNC_INFO);
                    struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 2, destination_db_obj, Q_FUNC_INFO);
                    struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 3, destination_db_obj, Q_FUNC_INFO);

                    export_as_vic_details_list.append(struct_export_vic_details_obj);
                }
            }
            else
            {
                struct_global_export_as_hashes_details struct_export_vic_details_obj;

                struct_export_vic_details_obj.fs_record_no       = fs_record_no;
                struct_export_vic_details_obj.display_filepath   = display_filepath;
                struct_export_vic_details_obj.display_filename  = display_filename;
                struct_export_vic_details_obj.complete_file_path = complete_file_path;
                struct_export_vic_details_obj.source_count_name  = source_count_name;
                struct_export_vic_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

                struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
                struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 1, destination_db_obj, Q_FUNC_INFO);
                struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 2, destination_db_obj, Q_FUNC_INFO);
                struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 3, destination_db_obj, Q_FUNC_INFO);

                export_as_vic_details_list.append(struct_export_vic_details_obj);
            }
        }
        display_loading_progress_bar_non_cancelable_obj->hide();

        export_hashes_as_vic_obj->pub_set_vic_version(vic_version);
        export_hashes_as_vic_obj->pub_set_vic_export_dir_path(destination_location);
        export_hashes_as_vic_obj->pub_set_export_file_details(export_as_vic_details_list);
        if(vic_version.contains("1.1") ||vic_version.contains("1.2"))
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
        //VIC 2.0 is used here to get user selected name of vic file
        //Here we show the dialog box to get that name instead of filedialog
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

void file_system::prepare_hidden_column_list_from_db()
{
    hide_column_list.clear();
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
            return;
        }

        QSqlQuery query(destination_db);

        QString command;
        command = QString("select hidden_column_name from hidden_columns where plugin_name=") + QString("'") + QString(MACRO_Plugin_Name_File_System) + QString("'");
        query.prepare(command);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query selection of hidden cols on load case tree ---FAILED---",Q_FUNC_INFO);
            recon_static_functions::app_debug("  error " + query.lastError().text(),Q_FUNC_INFO);
        }

        while(query.next())
        {
            QString list = query.value(0).toString();

            if(list == "File Size")
            {
                list.clear();
                list = "File Size (Bytes)";
                hide_column_list << list << "File Size (Units)";
            }
            else if (list == "Date Added")
            {
                list.clear();
                list = "Date Added(Apple)";
                hide_column_list << list;
            }
            else if (list == "Content Creation Date")
            {
                list.clear();
                list = "Content Creation Date(Apple)";
                hide_column_list << list;
            }
            else if (list == "Content Modification Date")
            {
                list.clear();
                list = "Content Modification Date(Apple)";
                hide_column_list << list;
            }
            else if (list == "Last Used Date")
            {
                list.clear();
                list = "Last Used Date(Apple)";
                hide_column_list << list;
            }
            else
            {
                hide_column_list << list;
            }
        }

        destination_db.close();
    }
    hide_column_list.removeDuplicates();
}

void file_system::slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status)
{
    if(bookmark_status == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        pub_remove_main_record_bookmark_from_detailed_info_triggered(record_str);
    }
    else if(bookmark_status == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
    {
        pub_bookmark_main_record_from_detailed_info_triggered(record_str);
    }
}

void file_system::slot_action_search_file_with_same_hash_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int current_row = m_file_system_tablewidget_obj->currentRow();
    if(current_row < 0)
        return;

    QString run_status_str = recon_helper_standard_obj->pub_get_filesystem_modules_thread_run_status(QString(MACRO_JobType_Hashes), Q_FUNC_INFO);
    if(run_status_str.isEmpty() || run_status_str == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No source has been processed for 'Hashes' yet.");
        return;
    }

    QString record_no = m_file_system_tablewidget_obj->item(current_row,enum_File_System_display_INT)->text();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db_obj,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QString hash_md5 = m_file_system_tablewidget_obj->item(current_row,enum_File_System_display_HASH_MD5)->text().trimmed();

    if(hash_md5.isEmpty())
    {
        /// get hashes from db
        QString command = QString("Select hash_md5 from files where INT = ?");
        QStringList vals_list;
        vals_list << record_no;

        hash_md5 = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command, vals_list, 0, destination_db_obj, Q_FUNC_INFO);
    }


    search_files_with_hash_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
    search_files_with_hash_obj->pub_refresh_tableWidget();
    search_files_with_hash_obj->pub_set_display_loading_progress_bar_obj(display_loading_progress_bar_non_cancelable_obj);
    search_files_with_hash_obj->pub_search_files_having_same_hash(hash_md5, record_no, source_count_name, complete_path);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_file_system_export_as_clicked(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Export_As_CSV))
    {
        export_dialog_box_3_obj->pub_set_export_as_type(QString(MACRO_Generic_Export_As_CSV));
        export_dialog_box_3_obj->pub_set_window_title(QString(MACRO_Generic_Export_As) + QString(" ") + QString(MACRO_Generic_Export_As_CSV));

        QString file_name = QString(MACRO_Plugin_Name_File_System);
        QString file_path = getenv("HOME") + QString("/Desktop");
        export_dialog_box_3_obj->pub_set_file_name_and_file_path(file_name,file_path);


        export_dialog_box_3_obj->show();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Export_As_SQLite))
    {
        export_dialog_box_3_obj->pub_set_export_as_type(QString(MACRO_Generic_Export_As_SQLite));
        export_dialog_box_3_obj->pub_set_window_title(QString(MACRO_Generic_Export_As) + QString(" ") + QString(MACRO_Generic_Export_As_SQLite));

        QString file_name = QString(MACRO_Plugin_Name_File_System);
        QString file_path = getenv("HOME") + QString("/Desktop");
        export_dialog_box_3_obj->pub_set_file_name_and_file_path(file_name,file_path);


        export_dialog_box_3_obj->show();
    }
}

void file_system::slot_export_hashes_as_sqlite_clicked()
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

    export_as_vic_details_list.clear();
    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();

        QFileInfo info_cmplt(complete_file_path);

        QFileInfo file_info(display_filepath);
        QString display_filename;
        display_filename = file_info.fileName();
        if(QFileInfo(complete_file_path).isDir())
            continue;
        else
        {
            struct_global_export_as_hashes_details struct_export_vic_details_obj;

            struct_export_vic_details_obj.fs_record_no       = fs_record_no;
            struct_export_vic_details_obj.display_filepath   = display_filepath;
            struct_export_vic_details_obj.display_filename  = display_filename;
            struct_export_vic_details_obj.complete_file_path = complete_file_path;
            struct_export_vic_details_obj.source_count_name  = source_count_name;
            struct_export_vic_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

            struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 1, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 2, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 3, destination_db_obj, Q_FUNC_INFO);

            export_as_vic_details_list.append(struct_export_vic_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    export_hashes_as_sqlite_obj->pub_set_export_file_details(export_as_vic_details_list);
    export_hashes_as_sqlite_obj->pub_start_export_hashes_as_sqlite();
}

void file_system::slot_export_hashes_as_csv_clicked()
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

    export_as_vic_details_list.clear();
    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();

        QFileInfo info_cmplt(complete_file_path);

        QFileInfo file_info(display_filepath);
        QString display_filename;
        display_filename = file_info.fileName();

        if(QFileInfo(complete_file_path).isDir())
            continue;

        else
        {
            struct_global_export_as_hashes_details struct_export_vic_details_obj;

            struct_export_vic_details_obj.fs_record_no       = fs_record_no;
            struct_export_vic_details_obj.display_filepath   = display_filepath;
            struct_export_vic_details_obj.display_filename  = display_filename;
            struct_export_vic_details_obj.complete_file_path = complete_file_path;
            struct_export_vic_details_obj.source_count_name  = source_count_name;
            struct_export_vic_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

            struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 1, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 2, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 3, destination_db_obj, Q_FUNC_INFO);

            export_as_vic_details_list.append(struct_export_vic_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    export_hashes_as_csv_obj->pub_set_export_file_details(export_as_vic_details_list);
    export_hashes_as_csv_obj->pub_start_export_hashes_as_csv();
}

void file_system::slot_export_hashes_as_vic_clicked()
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait, Preparing Files List...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString cmd_tstmp = QString("Select z_date_time_modified, z_date_time_change, z_date_time_accessed, z_date_time_created from files Where INT = ?");

    export_as_vic_details_list.clear();
    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        QString display_filepath   = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();

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
            QStringList record_no_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select INT From files Where file_parent_path like ?"), value_list, 0, destination_db_obj, Q_FUNC_INFO);

            ///get all files/directories in selected directory
            for(int count = 0; count < record_no_list.size(); count++)
            {
                if(count % 50 == 0)
                    QCoreApplication::processEvents();

                fs_record_no       = record_no_list.at(count);
                complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(fs_record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

                ///get all files(only) in selected directory
                if(QFileInfo(complete_file_path).isDir())
                    continue;

                display_filepath   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select filepath From files Where INT = ?"), QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);

                struct_global_export_as_hashes_details struct_export_vic_details_obj;

                struct_export_vic_details_obj.fs_record_no       = fs_record_no;
                struct_export_vic_details_obj.display_filepath   = display_filepath;
                struct_export_vic_details_obj.display_filename   = QFileInfo(display_filepath).fileName();
                struct_export_vic_details_obj.complete_file_path = complete_file_path;
                struct_export_vic_details_obj.source_count_name  = source_count_name;
                struct_export_vic_details_obj.examiner_selected_file_or_dir_name = info_cmplt.fileName();

                struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
                struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 1, destination_db_obj, Q_FUNC_INFO);
                struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 2, destination_db_obj, Q_FUNC_INFO);
                struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 3, destination_db_obj, Q_FUNC_INFO);

                export_as_vic_details_list.append(struct_export_vic_details_obj);
            }
        }
        else
        {
            struct_global_export_as_hashes_details struct_export_vic_details_obj;

            struct_export_vic_details_obj.fs_record_no       = fs_record_no;
            struct_export_vic_details_obj.display_filepath   = display_filepath;
            struct_export_vic_details_obj.display_filename  = display_filename;
            struct_export_vic_details_obj.complete_file_path = complete_file_path;
            struct_export_vic_details_obj.source_count_name  = source_count_name;
            struct_export_vic_details_obj.examiner_selected_file_or_dir_name   = info_cmplt.fileName();

            struct_export_vic_details_obj.modif_timestamp  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 0, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.change_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 1, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.last_acces_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 2, destination_db_obj, Q_FUNC_INFO);
            struct_export_vic_details_obj.create_timestamp = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_tstmp, QStringList(fs_record_no), 3, destination_db_obj, Q_FUNC_INFO);

            export_as_vic_details_list.append(struct_export_vic_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    export_hashes_as_vic_obj->pub_set_export_file_details(export_as_vic_details_list);
    export_hashes_as_vic_obj->pub_start_vic_export_2_0();
}

void file_system::slot_act_app_related_files_triggered(bool status)
{
    int selected_row_no = m_file_system_tablewidget_obj->currentRow();

    if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
        return;

    if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
        return;


    QString file_name = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILENAME)->text();
    QString file_path = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_FILEPATH)->text();

    QString cmplt_file_path = recon_static_functions::get_complete_file_path(virtual_source_path, file_path,Q_FUNC_INFO);
    QString app_info_plist_file_path = cmplt_file_path + "/Contents/info.plist";

    QString app_identifier;
    if(QFileInfo::exists(app_info_plist_file_path))
    {
        app_identifier = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(app_info_plist_file_path, enum_Global_Plist_File_Key_Type_String,"Bundle identifier", Q_FUNC_INFO);

        if(app_identifier.trimmed().isEmpty())
        {
            app_identifier = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(app_info_plist_file_path, enum_Global_Plist_File_Key_Type_String,"CFBundleIdentifier", Q_FUNC_INFO);
        }

        QStringList tmp_list = app_identifier.split(".");
        if(tmp_list.size() == 3)
        {
            app_identifier = tmp_list.at(tmp_list.size() - 1);
        }
        else
            app_identifier = file_name;

    }

    if(app_identifier.trimmed().isEmpty())
        app_identifier = file_name;

    QCoreApplication::processEvents();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->show();

    connect(app_related_files_obj,SIGNAL(signal_related_files_tablewidget_goto_record(QString,QString,QString)),this,SLOT(slot_go_to_file_system_record(QString,QString,QString)),Qt::QueuedConnection);

    app_related_files_obj->pub_set_source_count_name(source_count_name);
    bool is_file_list_avail = app_related_files_obj->pub_set_data_in_table(db_path_filesystem, app_identifier, file_name);

    display_loading_progress_bar_non_cancelable_obj->hide();

    if(is_file_list_avail)
    {
        app_related_files_obj->setWindowFlags(Qt::Window);
        app_related_files_obj->show();
        app_related_files_obj->activateWindow();
    }
    else
    {
        QMessageBox::information(this,recon_static_functions::get_app_name(),"No file(s) found");
    }

}

void file_system::slot_process_video_thumbnails_finished(int)
{
    bool_process_thumbnails_finished = true;
}

void file_system::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QList<struct_search_column_text_for_file_system> search_text_col_QList;

    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            bool_stop_sorting_tags_bookmark_notes_after_search = true;

            struct_search_column_text_for_file_system struct_search_column_text_for_file_system_obj;
            struct_search_column_text_for_file_system_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_file_system_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_file_system_obj);
        }
    }
    bool_cancel_loading = false;
    if(search_text_col_QList.size() < 1)
    {

        if(recursive_view_status)
        {
            recursive_view_status = false;
            slot_show_files_recursively_clicked(false);
        }
        else
        {
            slot_pushbutton_showall_clicked();
        }

        return;
    }

    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    st_gallery_view_file_info_list.clear();

    all_column_text_search(search_text_col_QList);

    bool_did_tableview_display_changed = true;

    refresh_horizontal_header();

    emit signal_clear_all_metadata();
    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void file_system::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(column != enum_File_System_display_BOOKMARK && column != enum_File_System_display_NOTES_ICON && column != enum_File_System_display_TAG_ICON
            && column != enum_File_System_display_SEEN)
    {
        if(column_sort_order == 0)
            m_file_system_tablewidget_obj->sortByColumn(column, Qt::DescendingOrder);
        else
            m_file_system_tablewidget_obj->sortByColumn(column, Qt::AscendingOrder);

        refresh_horizontal_header();
        return;
    }

    if(bool_stop_sorting_tags_bookmark_notes_after_search)
        return;

    emit signal_fs_refresh_widgets();

    m_file_system_tablewidget_obj->clearSelection();

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    if(m_file_system_tablewidget_obj->rowCount() <= 1)
        return;

    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_file_system_tablewidget_obj->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < m_file_system_tablewidget_obj->rowCount(); row++)
        {
            if(row % 20 == 0)
                QCoreApplication::processEvents();

            if(m_file_system_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }
            else
            {
                row_shift_pos_ascending = row;
                break;
            }
        }
    }
    else
    {
        for(qint64 row = m_file_system_tablewidget_obj->rowCount() -1; row > 0; row--)
        {
            if(row % 20 == 0)
                QCoreApplication::processEvents();

            if(m_file_system_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }
            else
            {
                row_shift_pos_descending = row;
                break;
            }
        }
    }

    qint64  rw_start = row_shift_pos_ascending;
    qint64  rw_end = row_shift_pos_descending + 1; //add 1 for last row.


    if(bool_sort_ascending)
    {
        for(qint64 row = rw_start; row < rw_end; row++)
        {

            if(row < row_shift_pos_ascending)
            {
                continue;
            }

            if(m_file_system_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            int destination_row;
            for(qint64 ii = rw_start; ii <= row_shift_pos_ascending; ii++)
            {
                if(!m_file_system_tablewidget_obj->isRowHidden(ii))
                {
                    destination_row = ii;
                }
            }



            if(row % 50 == 0)
                QCoreApplication::processEvents();

            QIcon icon_set;
            if(column != enum_File_System_display_BOOKMARK)
                icon_set =  m_file_system_tablewidget_obj->item(row, column)->icon();

            if(m_file_system_tablewidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column != enum_File_System_display_BOOKMARK)
                    icon_shiftrow_acs =  m_file_system_tablewidget_obj->item(row_shift_pos_ascending, column)->icon();

                if(m_file_system_tablewidget_obj->item(row_shift_pos_ascending, column)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;


                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_file_system_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_file_system_tablewidget_obj->takeRow(destRow);

                m_file_system_tablewidget_obj->setRow(destRow, sourceItems);

                m_file_system_tablewidget_obj->setRow(sourceRow, destItems);
                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = rw_start; row < rw_end; row++)
        {
            if(row > row_shift_pos_descending)
            {
                continue;
            }

            if(m_file_system_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            if(row % 50 == 0)
                QCoreApplication::processEvents();

            QIcon icon_set;
            if(column != enum_File_System_display_BOOKMARK)
                icon_set =  m_file_system_tablewidget_obj->item(row, column)->icon();

            if(m_file_system_tablewidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column != enum_File_System_display_BOOKMARK)
                    icon_shiftrow_desc =  m_file_system_tablewidget_obj->item(row_shift_pos_descending, column)->icon();

                if(m_file_system_tablewidget_obj->item(row_shift_pos_descending, column)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                QList<QTableWidgetItem*> sourceItems = m_file_system_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_file_system_tablewidget_obj->takeRow(destRow);

                m_file_system_tablewidget_obj->setRow(destRow, sourceItems);
                m_file_system_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }
        bool_sort_ascending = true;
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    refresh_horizontal_header();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::all_column_text_search(QList<struct_search_column_text_for_file_system> search_text_all_column_list)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    st_gallery_view_file_info_list.clear();

    for(int row = 0; row < m_file_system_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break;
        }

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool hide_row = false;

        for(int column_count = 0; column_count < search_text_all_column_list.size(); column_count++)
        {
            struct_search_column_text_for_file_system struct_search_column_text_for_file_system_obj = search_text_all_column_list.at(column_count);
            if(column_count % 10 == 0)
                QCoreApplication::processEvents();

            if(!(m_file_system_tablewidget_obj->item(row, struct_search_column_text_for_file_system_obj.column_number)->text().contains(struct_search_column_text_for_file_system_obj.search_text, Qt::CaseInsensitive)))
            {

                hide_row = true;
                break;
            }
        }

        if(hide_row)
        {
            m_file_system_tablewidget_obj->hideRow(row);
        }
        else
        {
            m_file_system_tablewidget_obj->showRow(row);
            prepare_gallery_view_list(row);
        }
    }

    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void file_system::prepare_gallery_view_list(int row)
{
    QString full_filepath;

    QTableWidgetItem *item_INT = m_file_system_tablewidget_obj->item(row, enum_File_System_display_INT);
    QTableWidgetItem *item_filename = m_file_system_tablewidget_obj->item(row, enum_File_System_display_FILENAME);
    QTableWidgetItem *item_filepath = m_file_system_tablewidget_obj->item(row, enum_File_System_display_FILEPATH);
    QTableWidgetItem *item_bk = m_file_system_tablewidget_obj->item(row, enum_File_System_display_BOOKMARK);
    QTableWidgetItem *item_date_last_read = m_file_system_tablewidget_obj->item(row, enum_File_System_display_DATE_ACCESSED);

    bool bool_is_it_file = false;

    if(os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        QStringList list_ios_path;
        list_ios_path << item_filepath->text();
        QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference("select id_path from files where filepath LIKE ?" , list_ios_path, 0, destination_db_obj, Q_FUNC_INFO);
        if(!idpath.isEmpty())
        {
            full_filepath = recon_static_functions::get_complete_file_path(fs_extraction_path, idpath, Q_FUNC_INFO);
            bool_is_it_file = true;
        }
    }
    else
    {
        QString fs_path = fs_extraction_path;
        full_filepath = recon_static_functions::get_complete_file_path(fs_path, item_filepath->text(), Q_FUNC_INFO);
        bool_is_it_file = QFileInfo(full_filepath).isFile();
    }

    if(!bool_is_it_file)
        return;

    if(recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
    {

        struct_global_gallery_view_file_info f1;
        f1.file_name = item_filename->text();

        f1.file_path = item_filepath->text();
        f1.thumbnail_file_path = full_filepath;
        f1.full_filepath = full_filepath;
        f1.INT = item_INT->text();
        f1.table_view_row_no = row;
        f1.bookmark = "0";
        f1.latitude = "";
        f1.longitude = "";
        f1.last_access_timestamp = item_date_last_read->text();
        f1.source_count_name = source_count_name;
        f1.index = st_gallery_view_file_info_list.size();

        if(item_bk->checkState() == Qt::Checked)
            f1.bookmark = "1";

        st_gallery_view_file_info_list.append(f1);
    }
    else if(recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(item_filename->text()))
    {
        QString file_size = m_file_system_tablewidget_obj->item(row, enum_File_System_display_DATE_ACCESSED)->text();
        QString file_name = item_filename->text();

        QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

        QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";

        QString fs_record_no = item_INT->text();

        QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
        QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

        if(!thumb_file_path.isEmpty())
        {
            full_filepath = result_filepath + thumb_file_path;
        }
        else
        {
            file_name.remove(file_name.lastIndexOf("."),file_name.size());
            video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

            //            QString script_path = "../Resources/scripts_persistent/video_frame.py";
            //            QString cmd_python_path = global_varibale_python_path;

            //            QStringList args;
            //            args << script_path << full_filepath << video_thumbnails_path << file_size;

            QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

            QStringList args;
            args << full_filepath << video_thumbnails_path << file_size;

            QProcess process_thumbnails;

            //            process_thumbnails.start(cmd_python_path,args);
            process_thumbnails.start(video_frame_exe_path,args);

            if(!process_thumbnails.waitForStarted())
            {
                recon_static_functions::app_debug("process_thumbnails -----FAILED----- ",Q_FUNC_INFO);
                recon_static_functions::app_debug("Error :- " + process_thumbnails.errorString() ,Q_FUNC_INFO);
                return;
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

            full_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();

            QString thumb_filepath;
            QStringList list;
            QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

            QFileInfo file_inf(full_filepath);
            if(file_inf.exists())
            {
                thumb_filepath.clear();
                thumb_filepath = full_filepath;
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
        f1.thumbnail_file_path = full_filepath;
        f1.full_filepath = full_filepath;
        f1.INT = item_INT->text();
        f1.table_view_row_no = row;
        f1.bookmark = "0";
        f1.latitude = "";
        f1.longitude = "";
        f1.last_access_timestamp = item_date_last_read->text();
        f1.source_count_name = source_count_name;
        f1.index = st_gallery_view_file_info_list.size();

        if(item_bk->checkState() == Qt::Checked)
            f1.bookmark = "1";

        st_gallery_view_file_info_list.append(f1);
    }
}
