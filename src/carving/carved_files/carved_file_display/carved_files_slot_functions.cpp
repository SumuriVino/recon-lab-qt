#include "carved_files_loader.h"


void carved_files_loader::action_bookmark_triggered()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(true);

    QStringList INT_List;
    QList<struct_global_tagging_carved_files> carved_files_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        QString tmp_rec_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << tmp_rec_str;
        bookmarked_records_from_table_display_list << tmp_rec_str;

        struct_global_tagging_carved_files s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, offset_index)->text();
        s1.item3 = clicked_item_tab_text;

        carved_files_record_list << s1;
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    }

    global_connection_manager_obj->pub_set_bookmark_in_carved_files_db(INT_List ,clicked_item_tab_text);
    global_connection_manager_obj->pub_add_bookmark_for_carved_files_db(carved_files_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_files_loader::action_remove_bookmark_triggered()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(false);

    QStringList INT_List;
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

        QString tmp_rec_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << tmp_rec_str;
        qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(tmp_rec_str);
        if(tmp_index >= 0)
            bookmarked_records_from_table_display_list.removeAt(tmp_index);


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_carved_files_db(INT_List,clicked_item_tab_text);
    global_connection_manager_obj->pub_delete_bookmark_for_carved_files_db(INT_List,clicked_item_tab_text);


    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_files_loader::slot_main_table_widget_item_clicked(QTableWidgetItem * item)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);
    if(item == NULL)
        return;

    if(item->row() < 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(item->column() != bookmark_index)
        return;

    int current_row = item->row();

    if(current_row < 0)
        return;


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

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void carved_files_loader::slot_act_remove_tags()
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

    global_connection_manager_obj->pub_remove_tag_in_carved_files_db(INT_List, clicked_item_tab_text);
    global_connection_manager_obj->pub_delete_tag_for_carved_files(INT_List, clicked_item_tab_text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_files_loader::slot_add_notes_pushbutton_clicked(QString text)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();

    text = text.trimmed();

    int notes_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    if(notes_column_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_carved_files> carved_files_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int current_row = model_index.row();
        if(current_row < 0 && current_row >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(current_row, record_no_index)->text();

        struct_global_tagging_carved_files s1;
        s1.INT_record = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(current_row, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(current_row, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, offset_index)->text();
        s1.item3 = clicked_item_tab_text;

        carved_files_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_carved_files_db(INT_List, clicked_item_tab_text,text);
    global_connection_manager_obj->pub_add_notes_for_carved_files_db(carved_files_record_list, text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_files_loader::slot_act_quick_look_triggered()
{
    emit signal_act_quick_look_triggered();
}

void carved_files_loader::action_open_with_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

    int selected_row_no = m_tablewidget_obj->currentRow();

    QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
    QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_str ,destination_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    QString temp_p = complete_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not plist file.");
        message_dialog_obj->show();
        return;
    }
    else
    {

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString plist_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(plist_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
            return;
        }

        plist_viewer *plist_viewer_obj;
        plist_viewer_obj = new plist_viewer(this);
        QString plist_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no_str;

        plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
        plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
        plist_viewer_obj->pub_set_plist_path(complete_path);
        plist_viewer_obj->pub_set_source_count_name(source_count_name);
        plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);

        plist_viewer_obj->pub_set_essentials();

        plist_viewer_obj->pub_start_plist_extraction();

        emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_open_with_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    int selected_row_no = m_tablewidget_obj->currentRow();

    QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
    QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_str ,destination_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }


    QString src_cnt_name_label = QString(" [" + source_count_name + "]");
    QString hex_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;


    QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
    if(list_hex_display_name.contains(hex_display_nm))
    {
        emit signal_show_file_in_viewer_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer, source_count_name);
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
    hex_viewer_open_file_data_obj.record_no_str       = record_no_str;
    hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
    hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name;
    hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;
    hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
    hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

    emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void carved_files_loader::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    int selected_row_no = m_tablewidget_obj->currentRow();

    QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
    QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_str ,destination_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();

        return;
    }

    if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not sqlite file.");
        message_dialog_obj->show();
        return;
    }
    else
    {

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString sqlite_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(sqlite_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
            return;
        }

        QString sqlite_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no_str;

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


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void carved_files_loader::action_open_with_log_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString adhoc_log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString).toString();

    int selected_row_no = m_tablewidget_obj->currentRow();

    QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
    QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_str,destination_db_path,source_count_name);
    QString log_file_complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(log_file_complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    QString temp_p = log_file_complete_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".log"))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not log file.");
        message_dialog_obj->show();
        return;
    }
    else
    {
        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString log_file_display_name = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

        QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(adhoc_log_viewer_sqlite_path, Q_FUNC_INFO);
        if(list_log_display_name.contains(log_file_display_name))
        {
            emit signal_show_file_in_viewer_table_result(log_file_display_name, MACRO_CASE_TREE_Log_Viewer, source_count_name);
            return;
        }

        log_viewer *log_viewer_obj;
        log_viewer_obj = new log_viewer(this);
        QString log_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no_str;

        log_viewer_obj->pub_set_result_log_viewer_sqlite_path(adhoc_log_viewer_sqlite_path);
        log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
        log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
        log_viewer_obj->pub_set_source_count_name(source_count_name);
        log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        log_viewer_obj->pub_set_essentials();
        log_viewer_obj->pub_start_log_extraction();

        emit signal_show_file_in_viewer_table_result(log_file_display_name, MACRO_CASE_TREE_Log_Viewer, source_count_name);
    }
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_sendToBucket_log_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;

    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no ,destination_db_path,source_count_name);
        QString log_file_complete_path = st_globl_recon_file_info_obj.complete_file_path;

        all_path_list << log_file_complete_path;

        for(int pos = 0; pos < all_path_list.size(); pos++)
        {
            QString complete_path = all_path_list.at(pos);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            QString temp_p = complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".log"))
            {
                message_list << QString("'" +  info.fileName() + "' is not log file.");
                continue;
            }
            else
            {
                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString log_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no + QString("]") + src_cnt_name_label;

                QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(log_viewer_sqlite_path, Q_FUNC_INFO);
                if(list_log_display_name.contains(log_display_nm))
                {
                    continue;
                }

                log_viewer *log_viewer_obj;
                log_viewer_obj = new log_viewer(this);
                QString log_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

                log_viewer_obj->pub_set_result_log_viewer_sqlite_path(log_viewer_sqlite_path);
                log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
                log_viewer_obj->pub_set_log_file_path(complete_path);
                log_viewer_obj->pub_set_source_count_name(source_count_name);
                log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                log_viewer_obj->pub_set_essentials();
                log_viewer_obj->pub_start_log_extraction();

                emit signal_viewer_extarction_completed_table_result(log_display_nm,MACRO_CASE_TREE_Log_Viewer);
            }
        }
    }


    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_sendToBucket_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;


    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no ,destination_db_path,source_count_name);
        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        all_path_list << complete_path;

        for(int pos = 0 ; pos< all_path_list.size() ; pos++)
        {
            QString complete_path = all_path_list.at(pos);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            QString temp_p = complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
            {
                message_list << QString("'" +  info.fileName() + "' is not plist file.");
                continue;
            }
            else
            {
                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString plist_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no + QString("]") + src_cnt_name_label;

                QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
                if(list_plist_display_name.contains(plist_display_nm))
                {
                    continue;
                }

                plist_viewer *plist_viewer_obj;
                plist_viewer_obj = new plist_viewer(this);
                QString plist_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

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
    }


    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_sendToBucket_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;


    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
        struct_global_recon_file_info struct_global_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_str ,destination_db_path,source_count_name);
        QString complete_path = struct_global_recon_file_info_obj.complete_file_path;
        all_path_list << QString(complete_path + MACRO_RECON_Splitter_1_multiple + source_count_name);

        for(int j = 0 ; j < all_path_list.size() ; j++)
        {
            QString complete_path_str = all_path_list.at(j);

            QStringList tmp_list = complete_path_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(tmp_list.size() < 2)
                continue;

            QString complete_path = tmp_list.at(0);
            QString source_count_name = tmp_list.at(1);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>" + complete_path,Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString hex_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;


            QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
            if(list_hex_display_name.contains(hex_display_nm))
            {
                continue;
            }

            ///check file in bucket
            struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;

            recon_static_functions::clear_variables_hex_viewer_open_file_data(hex_viewer_open_file_data_obj);

            QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

            hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
            hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
            hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;
            hex_viewer_open_file_data_obj.complete_file_path  = complete_path;
            hex_viewer_open_file_data_obj.display_file_path   = struct_global_recon_file_info_obj.display_file_path;
            hex_viewer_open_file_data_obj.record_no_str       = record_no_str;
            hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
            hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name;
            hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;
            hex_viewer_open_file_data_obj.recon_filefrom      = struct_global_recon_file_info_obj.recon_file_from;
            hex_viewer_open_file_data_obj.recon_file_infopath = struct_global_recon_file_info_obj.recon_file_info_path;

            recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

            emit signal_viewer_extarction_completed_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer);
        }
    }

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_sendToBucket_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QStringList all_path_list;

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no ,destination_db_path,source_count_name);
        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
        all_path_list << complete_path;

        for(int i = 0 ; i< all_path_list.size() ; i++)
        {
            QString full_path1 = all_path_list.at(i);

            QFileInfo info(full_path1);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            if(!recon_static_functions::check_is_it_sqlite_file_by_signature(full_path1, Q_FUNC_INFO))
            {
                message_list << QString("'" + info.fileName() + QString("' is not sqlite file."));
                continue;
            }
            else
            {

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString sqlite_display_nm = info.fileName() + QString(" [") + plugin_name + QString(" ") + record_no + QString("]") + src_cnt_name_label;

                QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
                if(list_sqlite_display_name.contains(sqlite_display_nm))
                {
                    continue;
                }

                QString sqlite_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

                sqlite_viewer_starter *sqlite_viewer_starter_obj;
                sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
                sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
                sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
                sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
                sqlite_viewer_starter_obj->pub_set_sqlite_path(full_path1);
                sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
                sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                sqlite_viewer_starter_obj->pub_set_essentials();

                sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

                emit signal_viewer_extarction_completed_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer);
            }
        }
    }

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::slot_sub_menu_open_file_with_other_app_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString source_count_name_str =  m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        QString record_no_str =  m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name,tab_name_str);
        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,destination_db_path,table_name,source_count_name_str,Q_FUNC_INFO);

        recon_static_functions::open_file_with_specified_app(complete_path , current_clicked_action->toolTip(), Q_FUNC_INFO);

        // show only first file (Multiselection not supported).
        break;

    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void carved_files_loader::slot_open_with_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
    {
        add_applications_to_open_application_menu();
    }
    else if(current_clicked_action->text() == QString(MACRO_CASE_TREE_Plist_Viewer))
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

void carved_files_loader::slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1)
{
    slot_act_quick_look_triggered();
}

void carved_files_loader::slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    action_export_triggered();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}



void carved_files_loader::slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1)
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

    int record_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    for(int pp = 0 ; pp < m_tablewidget_obj->rowCount() ; pp++)
    {
        if((f1.INT == m_tablewidget_obj->item(pp,record_index)->text())
                && (f1.source_count_name == m_tablewidget_obj->item(pp ,source_count_name_index)->text()))
        {
            current_row_selection_int = pp;
            break;
        }
    }

    m_tablewidget_obj->selectRow(current_row_selection_int);


    if(!bool_cell_changed_connection_buildup_for_tablewidget)
        m_tablewidget_obj->cellClicked(current_row_selection_int ,bookmark_value_index);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void carved_files_loader::slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info f1)
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

void carved_files_loader::slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(f1.INT.isEmpty())
    {
        m_tablewidget_obj->clearSelection();
        emit signal_clear_all_metadata();
        return;
    }

    /// control goes to table view tab
    /// now all operations can be done on basis of m_tablewidget_obj

    int record_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int current_row_selection_int = f1.table_view_row_no;
    for(int pp = 0 ; pp < m_tablewidget_obj->rowCount() ; pp++)
    {

        if((f1.INT == m_tablewidget_obj->item(pp,record_index)->text())
                && (f1.source_count_name == m_tablewidget_obj->item(pp ,source_count_name_index)->text()))
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

    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}


void carved_files_loader::slot_gallery_view_invalid_tem_clicked()
{
    m_tablewidget_obj->clearSelection();
    emit signal_clear_all_metadata();
}

void carved_files_loader::slot_table_widget_header_item_clicked(int column_index)
{
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

    int record_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    for(int pp = 0 ; pp < st_gallery_view_file_info_list.size() ; pp++)
    {
        struct_global_gallery_view_file_info f1 = st_gallery_view_file_info_list.at(pp);
        for(int qq = 0 ; qq < m_tablewidget_obj->rowCount() ; qq++)
        {
            if((f1.INT == m_tablewidget_obj->item(qq,record_index)->text())
                    && (f1.source_count_name == m_tablewidget_obj->item(qq ,source_count_name_index)->text()))
            {

                int row_no = qq;
                row_no_QList.append(row_no);

                index_row_qmap.insert(row_no , f1);

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


void carved_files_loader::slot_main_tab_widget_current_changed(int index)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);
    emit signal_clear_all_metadata();
    QCoreApplication::processEvents();

    bool_cell_changed_connection_buildup_for_tablewidget = false;

    if(index == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW && bool_did_tableview_display_changed)
    {
        m_tablewidget_obj->clearSelection();
        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_show_all->hide();
        bool_did_tableview_display_changed = false;

        gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);
        QCoreApplication::processEvents();

    }
    else if(index == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW && !bool_did_tableview_display_changed)
    {
        m_tablewidget_obj->clearSelection();
        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_show_all->hide();

    }
    else if(index == enum_carved_File_TabWidget_Indexes_TABLE_VIEW )
    {
        m_tablewidget_obj->clearSelection();

        lineEdit_search->hide();
        pushButton_search->hide();
        pushbutton_show_all->hide();
    }
    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}
void carved_files_loader::slot_pushbutton_showall_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait....");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_obj->show();

    m_tablewidget_obj->blockSignals(true);
    while(m_tablewidget_obj->rowCount())
        m_tablewidget_obj->removeRow(0);

    searched_keyword_str.clear();
    lineEdit_search->clear();


    st_gallery_view_file_info_list.clear();

    emit signal_clear_all_metadata();

    display_data(destination_db_path);
    bool_did_tableview_display_changed = true;

    m_tablewidget_obj->blockSignals(false);

    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);
    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
        slot_main_tab_widget_current_changed(enum_carved_File_TabWidget_Indexes_GALLERY_VIEW);

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void carved_files_loader::slot_goto_screenshot_record(QString record_str)
{
    record_str = record_str.trimmed();

    if(record_str.isEmpty())
        return;

    for(int count = 0; count < m_tablewidget_obj->rowCount(); count++)
    {
        QString record_no = m_tablewidget_obj->item(count, record_no_index)->text().trimmed();

        if(record_no == record_str)
        {
            m_tablewidget_obj->selectRow(count);
            break;
        }

    }
}

void carved_files_loader::slot_add_file_to_text_indexing_queue_clicked()
{

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    if(selection_model_list.size() > 0)
    {
        display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
        display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_non_cancelable_obj->show();


        QString text_indexing_queue_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

        QString connection_naam = Q_FUNC_INFO;
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

        destination_db.setDatabaseName(text_indexing_queue_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug("Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            return ;
        }

        QString command_str = QString("Insert into files (bookmark,fs_record,file_path,source_count_name,status,plugin_name) values(?,?,?, ?,?,?)");

        int count = 0;
        QStringList values_list;
        for(int i = 0; i < selection_model_list.size(); i++)
        {
            count++;
            if(count % 30 == 0)
            {
                count = 0;
                QCoreApplication::processEvents();
            }

            values_list.clear();

            QModelIndex model_index = selection_model_list.at(i);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
                continue;

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            values_list << QString::number(0);//bookmark

            QString record_no_str =  m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
            values_list << record_no_str; //fs_record

            QString filepath_str =  m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
            values_list << filepath_str; //file_path

            QString source_count_name_str =  m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            values_list << source_count_name_str; //source_count_name

            values_list << QString::number(0); //status
            values_list << QString(MACRO_Plugin_Name_Carved_Files); //plugin_name

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_str, values_list, destination_db, Q_FUNC_INFO);
        }

        destination_db.close();

        display_loading_progress_bar_non_cancelable_obj->hide();

    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void carved_files_loader::pub_bookmark_main_record_from_detailed_info_triggered(QString rec_no_str)
{

    if(bookmarked_records_from_table_display_list.contains(rec_no_str))
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(true);

    QStringList INT_List;
    QList<struct_global_tagging_carved_files> carved_files_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        QString tmp_rec_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        if(tmp_rec_str == rec_no_str)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        INT_List << tmp_rec_str;

        struct_global_tagging_carved_files s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, offset_index)->text();
        s1.item3 = clicked_item_tab_text;

        carved_files_record_list << s1;
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

        m_tablewidget_obj->selectRow(selected_row_no);

    }

    global_connection_manager_obj->pub_set_bookmark_in_carved_files_db(INT_List ,clicked_item_tab_text);
    global_connection_manager_obj->pub_add_bookmark_for_carved_files_db(carved_files_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_files_loader::pub_remove_main_record_bookmark_from_detailed_info_triggered(QString rec_no_str)
{
    if(bookmarked_records_from_table_display_list.contains(rec_no_str))
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(false);

    QStringList INT_List;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        QString tmp_rec_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        if(tmp_rec_str == rec_no_str)
            continue;

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;


        INT_List << tmp_rec_str;


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

        m_tablewidget_obj->selectRow(selected_row_no);

    }

    global_connection_manager_obj->pub_remove_bookmark_in_carved_files_db(INT_List,clicked_item_tab_text);
    global_connection_manager_obj->pub_delete_bookmark_for_carved_files_db(INT_List,clicked_item_tab_text);


    display_loading_progress_bar_non_cancelable_obj->hide();
}


