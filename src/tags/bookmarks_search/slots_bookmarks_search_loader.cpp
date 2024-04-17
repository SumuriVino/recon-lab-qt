#include "bookmarks_search_loader.h"

void bookmarks_search_loader::slot_add_notes_pushbutton_clicked(QString text)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    add_notes_to_singal_record_obj->hide();

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

        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();
        QString file_path_str = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

        QString result_note_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        QString is_bookmarked = "0";
        if(bookmark_index != -1)
        {
            if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() != 0)
            {
                is_bookmarked = "1";
            }
        }

        update_notes_value(text,selected_row_no);

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

            emit signal_add_notes_from_result_loader_for_fs(record_no, MACRO_Plugin_File_System_TAB_Name_Files, result_note_db_path, text, fs_db_path, os_name_str);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
        {
            emit signal_change_notes_in_other_db_also(record_no,QString(MACRO_Plugin_Name_Artifacts_Timeline),QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event),text,QStringList(""), os_name_str, feature_name);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR)
                || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR)
                || plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved)
                || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
        {
            emit signal_change_notes_in_other_db_also(record_no,plugin_name_str,tab_name_str,text,QStringList(""), os_name_str, feature_name);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser))
        {

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
        {

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            //            QString tmp_file_path = m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();
            //            if(!tmp_file_path.isEmpty())
            //            {
            //                QStringList tmp_split_list = tmp_file_path.split("/");
            //                if(tmp_split_list.size() >= 5)
            //                {
            //                    QString partial_db_path  = "/" + tmp_split_list.at(1) + QString("/") + tmp_split_list.at(2) +  QString("/") + tmp_split_list.at(3) + QString("/") + tmp_split_list.at(4)+ "/carver_files.sqlite";
            //                    QString carve_files_db_path =  global_recon_555_helper_singular_class_obj->get_complete_file_path_for_relative_filepaths_starts_with_Lab_Features(partial_db_path , Q_FUNC_INFO);
            //                    QString command = "UPDATE '" + QString("files") + "' SET notes = ? " +  " WHERE INT = ?";
            //                    QStringList arg_list;
            //                    arg_list  << text <<  record_no;

            //                    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,carve_files_db_path,Q_FUNC_INFO);
            //                }
            //            }
        }
        else
        {
            global_connection_manager_obj->pub_add_notes_for_plugin_viewer(QStringList(record_no),plugin_name_str,tab_name_str,text);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void bookmarks_search_loader::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str)
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

                QString initial_path,source_count_name;
                if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
                {

                    source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

                    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

                    QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no ,fs_db_path ,"files",source_count_name ,Q_FUNC_INFO);

                    all_path_list << complete_path;

                    tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);
                    table_name_str = "files";
                    parent_name = QString(MACRO_Plugin_Name_File_System);
                    destination_db_path =  fs_db_path;
                }
                else
                {
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
                    initial_path = recon_helper_standard_obj->get_virtual_source_path_of_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);
                    source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

                    destination_db_path = plugin_db_file;
                }

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

void bookmarks_search_loader::slot_add_files_to_hashset_submenu_clicked(QAction* current_clicked_action)
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

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        QString bookmark = "0";
        if(bookmark_index != -1)
        {
            if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == 2)
                bookmark = "1";
        }

        QString initial_path, source_count_name;
        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no ,fs_db_path ,"files",source_count_name ,Q_FUNC_INFO);

            all_path_list << complete_path;

            tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);
            table_name_str = "files";
            parent_name = QString(MACRO_Plugin_Name_File_System);
            destination_db_path =  fs_db_path;
        }
        else
        {
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
            initial_path = recon_helper_standard_obj->get_virtual_source_path_of_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);
            source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);
            destination_db_path = plugin_db_file;
        }

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

            hashset_storage_obj->insert_file_details_into_hashset_database(complete_path,dbpath, source_count_name);
        }
    }

    QFileInfo info2(dbpath);
    QString dbname = info2.fileName();
    QStringList list_db;

    QString db_str = dbname.remove(".sqlite") + MACRO_RECON_Splitter_1_multiple + dbpath;
    list_db << db_str;

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void bookmarks_search_loader::slot_goto_artifact_source_submenu_clicked(QAction *clicked_action)
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

void bookmarks_search_loader::slot_sort_tablewidget_display(int column_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    return;

    // Because notes has position at 0. Return if clicked column index other then notes
    if(column_index != 0)
    {
        m_tablewidget_obj->sortByColumn(column_index,Qt::AscendingOrder);
        m_tablewidget_obj->setSortingEnabled(false);
        return;
    }

    m_tablewidget_obj->clearSelection();

    if(m_tablewidget_obj->rowCount() <= 1)
        return;

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_tablewidget_obj->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column_index)->icon();

            // Check for checkstate OR iCon
            if(m_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                icon_shiftrow_acs =  m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->icon();

                // If row is already on 0th position then increment and continue
                if(m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                // Shifting of rows from lower to upper
                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column_index)->icon();

            if(m_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;
                icon_shiftrow_desc =  m_tablewidget_obj->item(row_shift_pos_descending, column_index)->icon();

                // If row is already on last position then decrement and continue
                if(m_tablewidget_obj->item(row_shift_pos_descending, column_index)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                // Shifting of rows from upper to lower
                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }

    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void bookmarks_search_loader::slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QString dest_dir_path = export_dir_path;// = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString();

    QStringList list_paths_to_message_box;

    QList<int> rows_list_to_export;

    if(!bool_export_all_file_system_records)
    {
        for(int row = 0; row < selection_model_list.size(); row++)
        {
            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
            if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
                continue;

            rows_list_to_export.append(selected_row_no);
        }

    }
    else
    {
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
                continue;

            QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
            if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
                continue;

            rows_list_to_export.append(row);
        }

    }



    for(int i = 0; i < rows_list_to_export.size(); i++)
    {

        int selected_row_no = rows_list_to_export.at(i);


        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();


        QString display_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();
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


        QString m_file_name = a_info.fileName();

        struct_global_export_file_system_record obj;
        obj.record_no = record_no;
        obj.source_count_name = source_count_name;
        obj.display_file_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();
        obj.displayed_file_name = m_file_name;


        QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj, Q_FUNC_INFO);
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
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_dir_path);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}


void bookmarks_search_loader::slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path,bool bool_is_enc_zip,QString password_enc_zip)
{
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QList<int> rows_list_to_export;

    if(!bool_export_all_file_system_records)
    {
        for(int row = 0; row < selection_model_list.size(); row++)
        {
            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
            if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
                continue;

            rows_list_to_export.append(selected_row_no);
        }

    }
    else
    {
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
                continue;

            QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
            if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
                continue;

            rows_list_to_export.append(row);
        }

    }

    QList<struct_global_file_for_zip_export> files_list_for_zip_export;

    QStringList files_which_do_not_exist;

    for(int i = 0; i < rows_list_to_export.size(); i++)
    {

        int selected_row_no = rows_list_to_export.at(i);

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;



        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        struct_global_file_for_zip_export st_file_info_for_export_obj;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
        QString filename = m_tablewidget_obj->item(selected_row_no, item_0_index)->text().trimmed();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        QString display_file_path = m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();
        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
        if(recon_helper_standard_obj->is_it_dir_record_no_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO))
        {
            QString cmd_2 = "select INT from files where file_parent_path LIKE '" + display_file_path  + "%'";
            QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_2, 0, fs_db_path, Q_FUNC_INFO);
            for(int ii = 0 ; ii < INT_list.size() ; ii++)
            {
                QString relevant_record_no = INT_list.at(ii);
                QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(relevant_record_no,fs_db_path ,"files" ,source_count_name, Q_FUNC_INFO);
                QFileInfo m_file_info(file_path);
                if(!m_file_info.exists())
                {
                    continue ;
                }

                QString cmd = "select filepath from files where INT =" + relevant_record_no;
                QString display_file_path_2 = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd, 0, fs_db_path, Q_FUNC_INFO);

                st_file_info_for_export_obj.source_path = file_path;
                st_file_info_for_export_obj.destination_path = source_count_name + display_file_path_2;

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


void bookmarks_search_loader::slot_thread_zip_file_export_finished()
{
    display_loading_progress_bar_obj->hide();

    if(bool_cancel_loading)
        return;

    bool_zip_export_done = true;

    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) exported to zip successfully. Do you want to open it?");
    message_dialog_obj->show();

}

void bookmarks_search_loader::slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_file_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();

    bool_cancel_loading = false;
    bool_dmg_export_done = false;

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QString dest_path = export_dmg_file_path;

    dmg_export_file_path.clear();

    QStringList list_paths_to_message_box,source_count_name_list;
    QString exported_file_path,parent_dir,tmp_folder_path,available_dmg_image;

    bool bool_is_image_created;

    QString current_timestamp;
    current_timestamp = QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));

    QList<int> rows_list_to_export;

    if(!bool_export_all_file_system_records)
    {
        for(int row = 0; row < selection_model_list.size(); row++)
        {
            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
            if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
                continue;

            rows_list_to_export.append(selected_row_no);
        }
    }
    else
    {
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
                continue;

            QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
            if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
                continue;

            rows_list_to_export.append(row);
        }
    }

    for(int i = 0; i < rows_list_to_export.size(); i++)
    {
        int selected_row_no = rows_list_to_export.at(i);

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();

        QString display_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();
        QFileInfo a_info(display_path);
        QString parent_dir = a_info.absolutePath();
        if(bool_keep_directory_structure)
        {
            dest_path = export_dmg_file_path  + source_count_name + "_" + current_timestamp + "/" + parent_dir + "/";
            QDir mdir;
            mdir.mkpath(dest_path);
        }
        else
        {
            dest_path = export_dmg_file_path  + source_count_name + "_" + current_timestamp + "/";
            QDir mdir;
            mdir.mkpath(dest_path);
        }

        QString m_file_name = a_info.fileName();

        struct_global_export_file_system_record obj;
        obj.record_no = record_no;
        obj.source_count_name = source_count_name;
        obj.display_file_path = display_path;
        obj.displayed_file_name = m_file_name;

        if(!source_count_name_list.contains(source_count_name))
        {
            source_count_name_list << source_count_name;
        }

        QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj , Q_FUNC_INFO);
        QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_path,m_file_name , Q_FUNC_INFO);
        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;
    }

    for( int ii = 0 ; ii < source_count_name_list.size() ; ii++)
    {
        bool_is_image_created = false;
        QString source_count_name = source_count_name_list.at(ii);
        QString folder_path = export_dmg_file_path  + source_count_name + "_" + current_timestamp;

        QDir dir(folder_path);
        if(bool_cancel_loading)
        {
            if(dir.exists())
            {
                dir.removeRecursively();
            }
            break;
        }
        tmp_folder_path = folder_path;
        QString export_dmg_path = tmp_folder_path.append(".dmg");

        available_dmg_image  = recon_static_functions::get_available_filename(export_dmg_path,dest_path, Q_FUNC_INFO);
        dmg_export_file_path = available_dmg_image;

        QStringList args_list_id;
        args_list_id << "create" <<  available_dmg_image << "-fs" << "apfs" << "-srcfolder" << folder_path;

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

void bookmarks_search_loader::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;

    if(yes_no_clicked)
    {
        if(bool_zip_export_done)
        {
            bool_zip_export_done = false;
            point_to_export_dir(zip_export_file_path);
        }
        else if(bool_dmg_export_done)
        {
            bool_dmg_export_done = false;
            point_to_export_dir(dmg_export_file_path);
        }
    }
}

void bookmarks_search_loader::point_to_export_dir(QString path)
{
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void bookmarks_search_loader::action_goto_source_triggered()
{
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    QString file_path;
    QString source_count_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();

    if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
        file_path = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_0_index)->text();
    else
        file_path = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), file_path_index)->text();

    if(file_path.trimmed().isEmpty())
        return ;

    if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        if(file_path.startsWith("/"+ QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features)))
        {
            QString carved_file_plugin_name = MACRO_Plugin_Name_Carved_Files;
            QString remove_str = "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features) + carved_file_plugin_name.replace(" ", "_") + "/" + source_count_name_str + "/";

            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString tmp_str = file_path;
            if(tmp_str.startsWith(remove_str))
            {
                tmp_str.remove(remove_str);
                QStringList tm_list = tmp_str.split("/");
                if(tm_list.size() > 0)
                {
                    QString case_tree_tab_name = tm_list.at(0);
                    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(case_tree_tab_name),0,index_db_path,Q_FUNC_INFO);
                    QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
                    QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select INT from files where recon_file_infopath = ?",QStringList(file_path),0,carved_files_db_path,Q_FUNC_INFO);
                    emit signal_tablewidget_goto_record(MACRO_Plugin_Name_Carved_Files,MACRO_CASE_TREE_Carved_Files,case_tree_tab_name,record_no , QString(""), "");
                }
            }
        }
        else
        {
            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_str + QString("/file_system.sqlite");
            emit signal_go_to_artifact_source_in_file_system_from_loader(file_path,fs_db_path,source_count_name_str);
        }
    }
}
