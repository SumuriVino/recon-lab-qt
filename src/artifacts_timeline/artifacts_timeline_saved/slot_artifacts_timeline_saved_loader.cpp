#include "artifacts_timeline_saved_loader.h"


void artifacts_timeline_saved_loader::slot_act_export_as_csv_triggered()
{

}

void artifacts_timeline_saved_loader::slot_act_export_as_xml_triggered()
{
}

void artifacts_timeline_saved_loader::slot_act_remove_tags()
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


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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


        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_for_saved_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_column_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_remove_tag_in_artifact_timeline_db(INT_List);
    global_connection_manager_obj->pub_delete_tag_for_artifact_timeline(timeline_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}


void artifacts_timeline_saved_loader::slot_add_notes_pushbutton_clicked(QString notes_value)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();

    notes_value = notes_value.trimmed();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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


        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_for_saved_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(selected_row_no, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_artifact_timeline_db(INT_List, notes_value);
    global_connection_manager_obj->pub_add_notes_for_artifact_timeline(timeline_record_list, notes_value);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_saved_loader::slot_main_table_widget_item_clicked(QTableWidgetItem *item)
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

void artifacts_timeline_saved_loader::slot_goto_artifact_source_submenu_clicked(QAction *clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString source_path_goto_source = clicked_action->toolTip();

    source_path_goto_source = source_path_goto_source.trimmed();
    if(source_path_goto_source.isEmpty())
        return;

    QString record_no_str   = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    QString tab_name_str    = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QString command = "SELECT source_count_name FROM '" + table_name + "' WHERE INT=?";
    QStringList arg_list;
    arg_list << record_no_str;
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    emit signal_go_to_artifact_source_in_file_system_from_loader(source_path_goto_source,fs_db_path,source_count_name);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

