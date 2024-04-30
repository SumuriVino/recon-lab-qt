#include "artifacts_keyword_search_loader.h"

void artifacts_keyword_search_loader::slot_link_label_keywords_list(QString text)
{ // hide and show keyword list of artifacts
    if(text == "Show")
    {
        link_label->setText("<a href = "+ QString("Hide") + " > " + QString("Searched Keywords") + " </a>");
        tablewidget_keywords_list->show();
    }
    else if(text == "Hide")
    {
        link_label->setText("<a href = "+ QString("Show") + " > " + QString("Searched Keywords") + " </a>");
        tablewidget_keywords_list->hide();
    }
}

void artifacts_keyword_search_loader::slot_tablewidget_keywords_list_item_doubleclicked(int row, int coloum)
{ // show keyword search list in tablewidget on double click
    QString keyword_to_show = tablewidget_keywords_list->item(row,coloum)->text().trimmed();

    if(keyword_to_show.contains( "("))
        keyword_to_show = keyword_to_show.remove(keyword_to_show.lastIndexOf(" ("), keyword_to_show.size());

    keyword_to_show = keyword_to_show.trimmed();

    emit signal_clear_all_metadata();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    int count = 1;

    if(row == 0) /// for show all
    {
        searched_keyword_str = all_keywords_str;
        for(int i = 0; i < m_tablewidget_obj->rowCount(); i++)
        {
            m_tablewidget_obj->showRow(i);
            m_tablewidget_obj->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(count++)));
        }
    }
    else
    {
        searched_keyword_str = keyword_to_show;
        for(int i = 0; i < m_tablewidget_obj->rowCount(); i++)
        {
            m_tablewidget_obj->showRow(i);

            QString keyword_hit = m_tablewidget_obj->item(i,keyword_hit_index)->text().trimmed();

            if(keyword_to_show != keyword_hit)
            {
                m_tablewidget_obj->hideRow(i);
            }
            else
            {
                m_tablewidget_obj->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(count++)));
            }
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void artifacts_keyword_search_loader::slot_add_notes_pushbutton_clicked(QString text) // add notes on right click
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    add_notes_to_singal_record_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);

    text = text.trimmed();

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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        if(plugin_name_str.isEmpty())
            continue;


        QIcon icon("../icons/notes.png");
        m_tablewidget_obj->item(selected_row_no, notes_icon_index)->setIcon(icon);

        global_connection_manager_obj->pub_set_notes_in_artifacts_keyword_search_db(QStringList(record_no),plugin_name_str, tab_name_str, text);
        global_connection_manager_obj->pub_set_notes_in_plugin_viewer_db(QStringList(record_no),plugin_name_str, tab_name_str, text );
        global_connection_manager_obj->pub_add_notes_for_plugin_viewer(QStringList(record_no),plugin_name_str, tab_name_str, text);
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::slot_goto_artifact_source_submenu_clicked(QAction *clicked_action) // go to artifact source file on right click
{
    QString source_path_goto_source = clicked_action->toolTip();

    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

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
}

void artifacts_keyword_search_loader::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str) //store in hashset on right click
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


                if(m_tablewidget_obj->isRowHidden(selected_row_no))
                            continue;

                QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
                QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

                if(plugin_name_str.isEmpty())
                    continue;

                QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();
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

                QString destination_plugin_dir_path = result_dir_path  + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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

                QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

                all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);

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
    recon_static_functions::app_debug(" Endds " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::slot_add_files_to_hashset_submenu_clicked(QAction* current_clicked_action) // add files to hashset to match or get duplicate files in right click
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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
                    continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();
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

        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);
        all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);
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


void artifacts_keyword_search_loader::slot_add_notes_to_bookmark_pushbutton_clicked(QString text) // add notes to bookmarks on right click
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    add_notes_to_bookmarks_obj->hide();
    text = text.trimmed();

    int i  = m_tablewidget_obj->rowCount();
    for(int k = 0 ; k < i ; k++)
    {
        if(k % 300 == 0)
        {
            QCoreApplication::processEvents();
        }

        int icon_notes_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
        int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        if(m_tablewidget_obj->item(k, bookmark_position)->checkState())
        {
            if(text.isEmpty())
            {
                QIcon icon_notes("");
                m_tablewidget_obj->item(k, icon_notes_position)->setIcon(icon_notes);
            }
            else
            {
                QIcon icon_notes("../icons/notes.png");
                m_tablewidget_obj->item(k, icon_notes_position)->setIcon(icon_notes);
            }

            QString record_no = m_tablewidget_obj->item(k, record_no_index)->text();
            QString os_name_str = m_tablewidget_obj->item(k, os_name_index)->text();

            QString tab_name_str = m_tablewidget_obj->item(k, tab_name_index)->text();

            QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString result_note_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";


            QString plugin_name_str = m_tablewidget_obj->item(k, plugin_name_index)->text().trimmed();

            if(plugin_name_str.isEmpty())
                continue;

            update_notes_value(text,k);

            struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

            int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

            QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

            emit signal_add_notes_from_result_loader_for_plugin("1", MACRO_Plugin_Notes_Search_TAB_Name_Plugin, record_no, plugin_name_str, tab_name_str, table_name, text, result_note_db_path, os_name_str);

        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::slot_act_quick_look_triggered() // quick look the record in right click menu
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_act_quick_look_triggered();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::slot_act_remove_tags() // remove tags action on right click
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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


        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        if(plugin_name_str.isEmpty())
            continue;


        QIcon icon("");
        m_tablewidget_obj->item(selected_row_no,tag_index)->setIcon(icon);

        global_connection_manager_obj->pub_remove_tag_in_plugin_viewer_db(QStringList(record_no), plugin_name_str, tab_name_str);
        global_connection_manager_obj->pub_delete_tag_for_plugin_viewer(QStringList(record_no),plugin_name_str, tab_name_str,"");
        global_connection_manager_obj->pub_remove_tags_in_artifacts_keyword_search_db(QStringList(record_no),plugin_name_str, tab_name_str);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::slot_main_table_widget_item_clicked(QTableWidgetItem *item) //click on any recored of table widget
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

//    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

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
