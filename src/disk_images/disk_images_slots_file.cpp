#include "disk_images_loader.h"

void disk_images_loader::slot_right_click_main_menu_triggered(QAction *current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_As_Source))
    {
        action_extract_source_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
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
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        action_remove_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    {
        action_copy_to_clipboard();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Tag))
    {
        slot_act_remove_tags();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_In_Window))
    {
        action_open_full_detailed_info_triggered();
    }

}

void disk_images_loader::slot_add_notes_pushbutton_clicked(QString text)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();
    text = text.trimmed();

    int notes_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    if(notes_column_index == -1)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList INT_List;
    QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int current_row = model_index.row();
        if(current_row < 0 && current_row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(current_row))
            continue;


        INT_List << m_tablewidget_obj->item(current_row, record_no_index)->text();

        struct_global_tagging_disk_images s1;

        s1.INT_record = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();

        s1.item0 = m_tablewidget_obj->item(current_row, 7)->text();
        s1.item1 = m_tablewidget_obj->item(current_row, 8)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, file_path_index)->text();

        rtrvd_srcs_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_disk_images_db(INT_List,text);
    global_connection_manager_obj->pub_add_notes_for_disk_images_db(rtrvd_srcs_record_list, text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void disk_images_loader::slot_main_table_widget_item_clicked(QTableWidgetItem *item)
{
    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(item == NULL)
        return;

    if(item->row() < 0)
        return;

    if(item->column() != bookmark_index)
        return;

    int current_row = item->row();

    if(current_row < 0)
        return;


    m_tablewidget_obj->blockSignals(true);

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

    m_tablewidget_obj->blockSignals(false);


}

void disk_images_loader::slot_act_remove_tags()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    int tag_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    if(tag_column_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList INT_List;

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

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_column_index)->setIcon(icon);
    }

    global_connection_manager_obj->pub_remove_tag_in_disk_images_db(INT_List);
    global_connection_manager_obj->pub_delete_tag_for_disk_images(INT_List);

    display_loading_progress_bar_non_cancelable_obj->hide();
}
