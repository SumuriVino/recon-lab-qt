#include "saved_graph_loader.h"

void saved_graphs_loader::slot_act_remove_tags()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    if(selection_model_list.size() > 1)
    {
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
            QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

            if(plugin_name_str.isEmpty())
                continue;


            QString record_no_column_name = db_columns_list.at(record_no_index);
            QString plugin_column_name = db_columns_list.at(plugin_name_index);
            QString table_name;
            if(table_name_list.size() > 0)
                table_name = table_name_list.at(0);

            QString command = "SELECT recon_tag_value from '" + table_name + "' where " + record_no_column_name + "=? AND " + plugin_column_name + "=? ";
            QStringList arg_list;
            arg_list << record_no << plugin_name_str ;
            QString tag_to_be_removed = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);


            command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=? AND " + plugin_column_name + "=? " ;
            arg_list.clear();
            arg_list << "" << record_no << plugin_name_str ;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

            int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

            tablewidget_general_clicked(selected_row_no,co_index);

            QIcon icon("");
            int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
            m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);


            emit signal_update_tags_to_other_db(plugin_name_str,tab_name_str,record_no,QStringList(tag_to_be_removed),"");
            emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record_no,QStringList(""),"");

        }
    }
    else
    {
        int selected_row_no = m_tablewidget_obj->currentRow();

        if(selected_row_no < 0)
            return;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        if(plugin_name_str.isEmpty())
            return;


      //  QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);
        //QString tab_column_name = db_columns_list.at(tab_name_index);

        // Because this class support only CSV of one tab only. confirmed by Mj sir
        QString table_name;
        if(table_name_list.size() > 0)
            table_name = table_name_list.at(0);

        QString command = "SELECT recon_tag_value from '" + table_name + "' where " + record_no_column_name + "=? AND " + plugin_column_name + "=? ";
        QStringList arg_list;
        arg_list << record_no << plugin_name_str ;
        QString tag_to_be_removed = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

        command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=? AND " + plugin_column_name + "=? ";
         arg_list.clear();
        arg_list << "" << record_no << plugin_name_str ;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

        int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
        tablewidget_general_clicked(selected_row_no,co_index);


        QIcon icon("");
        int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);

        emit signal_update_tags_to_other_db(plugin_name_str,tab_name_str,record_no,QStringList(tag_to_be_removed),"");
        emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record_no,QStringList(""),"");
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void saved_graphs_loader::slot_add_notes_pushbutton_clicked(QString text)
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

    QList<struct_global_tagging_saved_graph_record> saved_graph_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int current_row = model_index.row();
        if(current_row < 0 && current_row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(current_row))
            continue;

        INT_List << m_tablewidget_obj->item(current_row, record_no_index)->text();

        struct_global_tagging_saved_graph_record s1;
        s1.INT_record = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(current_row, 6)->text(); /// graph name
        s1.item1 = m_tablewidget_obj->item(current_row , file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, creation_date_index)->text();

        saved_graph_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_saved_graphs_and_screenshot_db(INT_List,plugin_name,text);
    global_connection_manager_obj->pub_add_notes_for_saved_graphs_and_screenshot_db(saved_graph_record_list, text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void saved_graphs_loader::slot_goto_saved_graphs_record(QString record_str)
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

