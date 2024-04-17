#include "redefined_result_full_loader.h"


void redefined_result_full_loader::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
}


void redefined_result_full_loader::slot_add_notes_pushbutton_clicked(QString text)
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

    global_connection_manager_obj->pub_set_notes_in_redefined_db(INT_List,plugin_name,text);
    global_connection_manager_obj->pub_add_notes_for_redefined_db(redefined_record_list, text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void redefined_result_full_loader::slot_goto_artifact_source_submenu_clicked(QAction *clicked_action)
{
    QString source_path_goto_source = clicked_action->toolTip();

    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info  st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QString command = "SELECT source_count_name FROM '" + table_name + "' WHERE INT=?";
    QStringList arg_list;
    arg_list << record_no_str;
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    emit signal_go_to_artifact_source_in_file_system_from_loader(source_path_goto_source,fs_db_path,source_count_name);
}

void redefined_result_full_loader::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str)
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

                QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
                QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

                if(plugin_name_str.isEmpty())
                    continue;

                int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

                QString is_bookmarked = "0";
                if(bookmark_index != -1)
                {
                    if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() != 0)
                    {
                        is_bookmarked = "1";
                    }
                }

                QString tmp_plg_name = plugin_name_str;
                tmp_plg_name.replace(" ","_");

                QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
                QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

                tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

                struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

                int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

                if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
                    continue;

                QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
                table_name_str = table_name;

                QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
                parent_name = st_csv_info_obj.plugin_parent_name;

                all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);
                QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

                destination_db_path = plugin_db_file;

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
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_add_files_to_hashset_submenu_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_clicked_action->text() == MACRO_Generic_Right_Click_Create_Hashset)
    {
        return;
    }

    QString dbpath = current_clicked_action->toolTip();

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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        QString bookmark = "0";
        if(bookmark_index != -1)
        {
            if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == 2)
                bookmark = "1";
        }
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            continue;

        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
        table_name_str = table_name;

        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
        parent_name = st_csv_info_obj.plugin_parent_name;

        all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);
        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

        destination_db_path = plugin_db_file;

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

            hashset_storage_obj->insert_file_details_into_hashset_database(complete_path,dbpath,source_count_name);
        }
    }

    QFileInfo info2(dbpath);
    QString dbname = info2.fileName();
    QStringList list_db;

    QString db_str = dbname.remove(".sqlite") + MACRO_RECON_Splitter_1_multiple + dbpath;
    list_db << db_str;

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::update_bookmark_value(QString status,int row, int column)
{
    if(column != 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
    {
        return;
    }


    QStringList INT_List;

    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;



    INT_List << m_tablewidget_obj->item(row, record_no_index)->text();

    struct_global_tagging_redefined_result_record s1;
    s1.INT_record = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(row, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(row, tab_name_index)->text();
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();
    s1.item2 = m_tablewidget_obj->item(row, 8)->text();
    s1.item3 = m_tablewidget_obj->item(row, 9)->text();
    s1.item4 = m_tablewidget_obj->item(row, 10)->text();
    s1.item5 = m_tablewidget_obj->item(row, 11)->text();
    s1.redefined_tab_name = plugin_name;
    s1.redefined_plugin_name = plugin_name;

    redefined_record_list << s1;


    if(status == "1")
    {
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_redefined_db(INT_List ,plugin_name);
        global_connection_manager_obj->pub_add_bookmark_for_redefined_db(redefined_record_list);

    }
    else
    {
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_redefined_db(INT_List, plugin_name);
        global_connection_manager_obj->pub_delete_bookmark_for_redefined_db(INT_List , plugin_name);

    }

}


void redefined_result_full_loader::update_tags_value(QString tag_data, QString colour)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int tag_coulumn_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    if(tag_coulumn_index == -1)
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

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_redefined_result_record s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, 11)->text();
        s1.redefined_tab_name = plugin_name;
        s1.redefined_plugin_name = plugin_name;

        redefined_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_coulumn_index)->setIcon(icon);

    }


    global_connection_manager_obj->pub_set_tag_in_redefined_db(INT_List, plugin_name,tag_data);
    global_connection_manager_obj->pub_add_tag_for_redefined(redefined_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();
}



void redefined_result_full_loader::action_bookmark_triggered()
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
    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

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

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_redefined_result_record s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, 11)->text();
        s1.redefined_tab_name = plugin_name;
        s1.redefined_plugin_name = plugin_name;

        redefined_record_list << s1;


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    }


    global_connection_manager_obj->pub_set_bookmark_in_redefined_db(INT_List ,plugin_name);
    global_connection_manager_obj->pub_add_bookmark_for_redefined_db(redefined_record_list);


    display_loading_progress_bar_non_cancelable_obj->hide();
}



void redefined_result_full_loader::action_remove_bookmark_triggered()
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


        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_redefined_db(INT_List  , plugin_name);
    global_connection_manager_obj->pub_delete_bookmark_for_redefined_db(INT_List  , plugin_name);


    display_loading_progress_bar_non_cancelable_obj->hide();
}


void redefined_result_full_loader::slot_main_table_widget_item_clicked(QTableWidgetItem *item)
{
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
}

void redefined_result_full_loader::slot_tablewidget_goto_record(QString record )
{

    if(record == "")
    {
        QString command = "SELECT INT FROM redefined_result WHERE INT = ?";
        QStringList arg_list;
        arg_list << record;
        record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_redefined_result,Q_FUNC_INFO);
    }

    int select_row = 0;
    bool row_found_in_existin_display = false;

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        QString INT_str = m_tablewidget_obj->item(row,record_no_index)->text();

        if(INT_str == record)
        {
            row_found_in_existin_display = true;
            select_row = row;
            break;
        }
    }

    if(row_found_in_existin_display)
    {
        m_tablewidget_obj->selectRow(select_row);

        QModelIndex index = m_tablewidget_obj->model()->index(select_row, record_no_index);
        m_tablewidget_obj->scrollTo(index, QAbstractItemView::PositionAtCenter);
    }
}

void redefined_result_full_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString keyword = lineEdit_search->text().trimmed();

    searched_keyword_str = keyword;

    if(keyword.trimmed().isEmpty())
        return;

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


    //all data in table view displayed according to timestamp for messenger as well as browser history.
    QString command = cols_nam_list.join(",");
    command = "select " + command + " from '" + table_name + "'";

    QString partial_command = recon_static_functions::get_partial_command_forsearch(cols_nam_list,search_string_list, Q_FUNC_INFO);
    command = command + partial_command + " AND (timestamp  >= ? AND timestamp <= ?) ORDER BY timestamp ASC";


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
    query_index.addBindValue(start_timestamp_int);
    query_index.addBindValue(end_timestamp_int);

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
