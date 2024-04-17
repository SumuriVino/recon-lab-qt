#include "redefined_result_saved_loader.h"

void redefined_result_saved_loader::slot_add_notes_pushbutton_clicked(QString text)
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

    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int current_row = model_index.row();
        if(current_row < 0 && current_row >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(current_row, record_no_index)->text();

        struct_global_tagging_redefined_result_record s1;
        s1.INT_record = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, 8)->text();
        s1.item3 = m_tablewidget_obj->item(current_row, 9)->text();
        s1.item4 = m_tablewidget_obj->item(current_row, 10)->text();
        s1.item5 = m_tablewidget_obj->item(current_row, 11)->text();
        s1.redefined_tab_name = plugin_name;
        s1.redefined_plugin_name = plugin_name;

        redefined_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_redefined_db(INT_List, plugin_name,text);
    global_connection_manager_obj->pub_add_notes_for_redefined_db(redefined_record_list, text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}


void redefined_result_saved_loader::slot_act_remove_tags()
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

    global_connection_manager_obj->pub_remove_tag_in_redefined_db(INT_List, plugin_name);
    global_connection_manager_obj->pub_delete_tag_for_redefined(INT_List, plugin_name);

    display_loading_progress_bar_non_cancelable_obj->hide();
}


void redefined_result_saved_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString keyword = lineEdit_search->text().trimmed();
    if(keyword.isEmpty())
        return ;

    searched_keyword_str = keyword;

    QStringList search_string_list = recon_static_functions::get_search_string_list(keyword, Q_FUNC_INFO);
    if(search_string_list.size() <= 0)
    {
        return;
    }

    QString table_name = "redefined_result_saved";
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



