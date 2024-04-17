#include "table_result_loader.h"



void table_result_loader::action_remove_bookmark_triggered()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    m_tablewidget_obj->setCurrentCell(-1, 0);
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

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        if(bookmark_index != -1)
        {
            update_bookmark_value("0",selected_row_no,bookmark_index);
        }
    }
    display_loading_progress_bar_non_cancelable_obj->hide();


}
void table_result_loader::action_add_note_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    add_notes_to_singal_record_obj->set_notes("");
    add_notes_to_singal_record_obj->show();


    return;


    if(table_name_list.size() > 0)
    {
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

            // Because this class support only CSV of one tab only. confirmed by Mj sir
            QString table_name = table_name_list.at(0);

            QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
            QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

            if(plugin_name_str.isEmpty())
                continue;

            QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
            QString record_no_column_name = db_columns_list.at(record_no_index);
            QString plugin_column_name = db_columns_list.at(plugin_name_index);
            QString tab_column_name = db_columns_list.at(tab_name_index);

            QString command = "select notes from '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=?";
            QStringList arg_list;
            arg_list << record_no << plugin_name_str << tab_name_str;
            QString already_added_note = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

            add_notes_to_singal_record_obj->set_notes(already_added_note);

            add_notes_to_singal_record_obj->show();

        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::action_remove_note_triggered()
{

}

void table_result_loader::action_add_note_to_bookmark_triggered()
{
    add_notes_to_bookmarks_obj->show();
}

void table_result_loader::action_remove_note_bookmark_triggered()
{
}

void table_result_loader::action_bookmark_triggered()
{
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

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
        if(bookmark_index != -1)
        {
            update_bookmark_value("1",selected_row_no,bookmark_index);
        }
    }
}

void table_result_loader::action_sendToBucket_plist_viewer_triggered()
{

}

void table_result_loader::action_sendToBucket_hex_viewer_triggered()
{

}

void table_result_loader::action_sendToBucket_sqlite_viewer_triggered()
{

}

void table_result_loader::action_open_with_plist_viewer_triggered()
{

}

void table_result_loader::action_open_with_hex_viewer_triggered()
{

}


void table_result_loader::action_open_with_log_viewer_triggered()
{

}

void table_result_loader::action_sendToBucket_log_viewer_triggered()
{

}

void table_result_loader::action_open_with_sqlite_viewer_triggered()
{

}

void table_result_loader::action_export_triggered()
{

}

void table_result_loader::action_copy_to_clipboard()
{
    recon_static_functions::app_debug("--Starts---", Q_FUNC_INFO);

    recon_static_functions::copy_tablewidget_data_to_clipboard(m_tablewidget_obj, Q_FUNC_INFO);

    recon_static_functions::app_debug("--Ends---", Q_FUNC_INFO);
}

void table_result_loader::loading_display_dialog_based_on_records(int total_records, int processed_count , QString title_str)
{

    if(total_records == processed_count)
    {
        display_loading_progress_bar_obj->hide();
    }
    else if(processed_count >= 10)
    {
        display_loading_progress_bar_obj->pub_set_progress_bar_title(title_str);

        QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
        display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

        int temp_count = processed_count;
        float record_percent = ((temp_count*100)/total_records);
        display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

        if(processed_count == 10)
            display_loading_progress_bar_obj->show();
    }
    else
    {
        display_loading_progress_bar_obj->hide();
    }


}

void table_result_loader::action_submenu_tag_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);

    emit signal_add_tags(tag_name,colour);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::action_open_detach_detailed_info_triggered()
{

}

void table_result_loader::action_go_to_record_triggered()
{

}


void table_result_loader::action_open_full_detailed_info_triggered()
{

}
void table_result_loader::action_mark_as_seen_unseen(QString clicked_action_text)
{

}

void table_result_loader::action_goto_source_triggered()
{

}

void table_result_loader::action_remove_record()
{

    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    m_tablewidget_obj->blockSignals(true);
    QStringList record_no_list;
    for(int i = selection_model_list.size() - 1; i >= 0; i--)
    {

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        record_no_list << m_tablewidget_obj->item(selected_row_no,record_no_index)->text();

        m_tablewidget_obj->removeRow(selected_row_no);

    }
    m_tablewidget_obj->blockSignals(false);


    for(int rc = 0 ; rc < record_no_list.size() ; rc++)
    {

        QString table_name;

        if(table_name_list.size() >= 1)
        {
            table_name = table_name_list.at(0);
            QString  record_num_str = record_no_list.at(rc);
            QStringList arg_list;
            QString command = "DELETE FROM "+ table_name + " WHERE INT=?";
            arg_list << record_num_str ;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);
        }

    }


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);

    emit signal_common_add_tag_name_into_case_tree(plugin_name);
}


void table_result_loader::enable_disable_go_to_source(int current_row , QString feature_plugin_name)
{
    recon_static_functions::app_debug(" starts", Q_FUNC_INFO);

    QString tab_name_str = m_tablewidget_obj->item(current_row , tab_name_index)->text();

    QList<QAction *> menu_list_go_to_source = mainMenu->actions();
    for(int count = 0; count < menu_list_go_to_source.size(); count++)
    {
        QAction *act = menu_list_go_to_source.at(count);
        if(act->text() == QString(MACRO_Generic_Right_Click_Go_To_Source))
        {
            if(feature_plugin_name == QString(MACRO_Plugin_Name_Sqlite_Viewer)
                    || feature_plugin_name == QString(MACRO_Plugin_Name_Plist_Viewer)
                    || feature_plugin_name == QString(MACRO_Plugin_Name_Registry_Viewer)
                    || feature_plugin_name == QString(MACRO_Plugin_Name_Log_Viewer)
                    || feature_plugin_name == QString(MACRO_Plugin_Name_Strings_Viewer)
                    || feature_plugin_name == QString(MACRO_Plugin_Name_Text_Viewer)
                    )

            {
                act->setEnabled(true);
            }
            else if(feature_plugin_name == MACRO_Plugin_Name_Hex_Viewer)
            {
                if(tab_name_str.contains(MACRO_Plugin_Forensic_Image_In_Hex_Viewer_TAB_Name))
                    act->setEnabled(false);
                else
                    act->setEnabled(true);
            }
            else if(feature_plugin_name == MACRO_Plugin_Name_Snapshots)
            {
                if(tab_name_str == MACRO_Plugin_Snapshots_TAB_Name_Modified_Files)
                    act->setEnabled(true);
                else if(tab_name_str == MACRO_Plugin_Snapshots_TAB_Name_Deleted_Files)
                    act->setEnabled(false);
            }
            else
            {
                act->setDisabled(true);
            }

            break;
        }

    }

    recon_static_functions::app_debug(" ends", Q_FUNC_INFO);
}
