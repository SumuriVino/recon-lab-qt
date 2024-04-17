#include "carved_data_loader.h"

void carved_data_loader::slot_main_table_widget_item_clicked(QTableWidgetItem * item)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);
    if(item == NULL)
        return;

    if(item->row() < 0)
        return;

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

void carved_data_loader::slot_act_remove_tags()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

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


    QStringList INT_List , source_count_name_list , tagging_list_on_record;

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

        source_count_name_list <<m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_icon_index)->setIcon(icon);

        tagging_list_on_record <<  is_tagged_current_record_record(selected_row_no  , record_no , dest_db);
    }
    dest_db.close();

    global_connection_manager_obj->pub_update_tag_export_content_carved_data_db(INT_List,category_name,source_count_name_list , "");
    global_connection_manager_obj->pub_delete_entry_export_content_carved_data_db(INT_List,current_table_name,source_count_name_list,tagging_list_on_record);
    global_connection_manager_obj->pub_remove_tag_in_carved_data_db(INT_List, current_table_name);
    global_connection_manager_obj->pub_delete_tag_for_carved_data(INT_List, category_name,source_count_name_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}

void carved_data_loader::slot_add_notes_pushbutton_clicked(QString text)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();

    text = text.trimmed();

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

    QStringList INT_List, source_count_namelist;
    QList<struct_global_tagging_carved_data> carved_data_record_list;

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

        QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();

        INT_List << record_no;
        QString source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
        source_count_namelist << source_count_name;

        QString bookmark_status;
        if(m_tablewidget_obj->item(current_row, bookmark_index)->checkState() == Qt::Checked)
            bookmark_status = MACRO_TRUE_VALUE_NUMERIC_QString;
        else
            bookmark_status = MACRO_FALSE_VALUE_NUMERIC_QString;


        struct_global_tagging_carved_data s1;
        s1.INT_record = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        s1.category_name = category_name;
        s1.table_name = current_table_name;


        s1.source_count_name = source_count_name;

        s1.source_file_name = m_tablewidget_obj->item(current_row, source_filename_index)->text();
        s1.source_file_path = m_tablewidget_obj->item(current_row, source_filepath_index)->text();
        s1.source_plugin_name = m_tablewidget_obj->item(current_row, source_plugin_name_index)->text();
        s1.source_tab_name = m_tablewidget_obj->item(current_row, source_tab_name_index)->text();
        s1.source_record_no = m_tablewidget_obj->item(current_row, source_record_no_index)->text();
        s1.root_count_name = m_tablewidget_obj->item(current_row, root_count_name_index)->text();
        s1.recon_file_from = recon_file_from;
        s1.recon_file_infopath = recon_file_info_path;


        s1.item0 = m_tablewidget_obj->item(current_row, number_of_hits_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(current_row, carved_keyword_index)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, source_filename_index)->text();
        s1.item3 = m_tablewidget_obj->item(current_row, source_filepath_index)->text();
        s1.item11 = category_name;
        s1.item12 = current_table_name;

        s1.notes_str = text;
        QString tagging_on_current_record = is_tagged_current_record_record(current_row , record_no , dest_db);
        s1.tagging_on_record = tagging_on_current_record;

        carved_data_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_icon_index)->setIcon(iconNotes);
        m_tablewidget_obj->item(current_row, notes_value_index)->setText(text);
    }
    dest_db.close();

    global_connection_manager_obj->pub_add_data_in_export_content_carved_data_db(carved_data_record_list,"");
    global_connection_manager_obj->pub_update_notes_export_content_carved_data_db(INT_List,category_name ,source_count_namelist,text);
    global_connection_manager_obj->pub_set_notes_in_carved_data_db(INT_List, current_table_name,text);
    global_connection_manager_obj->pub_add_notes_for_carved_data_db(carved_data_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_data_loader::slot_goto_screenshot_record(QString record_str)
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

void carved_data_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString keyword = lineEdit_search->text().trimmed();
    if(keyword.isEmpty())
        return;

    QStringList search_string_list = keyword.split(",", Qt::SkipEmptyParts);

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_label_messsge("Searching...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        bool bool_search_item_matched = false;

        if(row % 50 == 0)
            QCoreApplication::processEvents();

        ///column started from 2 intentionally,(first 3 columns bookmark, notes icon and tag icon).
        /// currently 14 columns are in use for display
        for(int col_count = 2 ; col_count < 15; col_count++)
        {
            if(bool_cancel_loading)
                break;

            QString column_data = m_tablewidget_obj->item(row, col_count)->text().trimmed();
            if(column_data.isEmpty())
                continue;

            for(int serach_item_index = 0; serach_item_index < search_string_list.size(); serach_item_index++)
            {
                if(bool_cancel_loading)
                    break;

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

        if(!bool_search_item_matched)
            m_tablewidget_obj->hideRow(row);
    }

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}
