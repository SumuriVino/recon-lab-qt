#include "snapshots_loader.h"

void snapshots_loader::slot_on_tab_changed_clicked(int)
{
    emit signal_clear_all_metadata();
}


void snapshots_loader::slot_tablewidget_selection_changed()
{
    recon_static_functions::app_debug(" -Starts- " , Q_FUNC_INFO);

    current_tab_index =  main_tab_widget_obj->currentIndex();
    if(current_tab_index < 0)
        return ;


    m_tablewidget_obj = list_tablewidgets.at(current_tab_index);


    int current_row = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();
    if(current_row < 0 || current_column < 0)
        return ;



    display_tab_name =  main_tab_widget_obj->tabText(current_tab_index);
    table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name ,display_tab_name);


    tablewidget_general_clicked(current_row , current_row);


    recon_static_functions::app_debug(" -Ends- " , Q_FUNC_INFO);
}

void snapshots_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    bool_timeline_is_selected = false;
    search_keyword_from_db();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void snapshots_loader::slot_pushbutton_showall_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    lineEdit_search->clear();

    display_data(destination_db_path);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void snapshots_loader::slot_pushButton_timeline_clicked()
{
    timeline_selection_window_obj->show();
}

void snapshots_loader::slot_time_line(qint64 start_time, qint64 end_time)
{
    timeline_selection_window_obj->hide();

    timeline_start_time =  global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(start_time);
    timeline_end_time =  global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(end_time);

    bool_timeline_is_selected = true;
    search_keyword_from_db();
    slot_custom_table_header_widget_search_field_return_pressed();
}

void snapshots_loader::slot_tablewidget_cell_clicked(int row, int column)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return ;

    if(column != bookmark_position)
        return ;


    current_tab_index =  main_tab_widget_obj->currentIndex();
    if(current_tab_index < 0)
        return ;


    m_tablewidget_obj = list_tablewidgets.at(current_tab_index);


    int current_row = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();
    if(current_row < 0 || current_column < 0)
        return ;



    display_tab_name =  main_tab_widget_obj->tabText(current_tab_index);
    table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name ,display_tab_name);


    if(m_tablewidget_obj->item(row, bookmark_position)->checkState() == Qt::Checked)
    {
        update_bookmark_value("1",row,bookmark_position);
    }
    else
    {
        update_bookmark_value("0",row,bookmark_position);
    }

    recon_static_functions::app_debug("-Ends " , Q_FUNC_INFO);
}

void snapshots_loader::slot_add_notes_pushbutton_clicked(QString text)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();

    text = text.trimmed();


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int current_row = model_index.row();
        if(current_row < 0 || current_row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(current_row))
            continue;



        INT_List << m_tablewidget_obj->item(current_row, record_no_index)->text();

        struct_global_tagging_tag_notes_db_updation s1;
        s1.record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = plugin_name;
        s1.tab_name = display_tab_name;
        s1.source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(current_row, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(current_row, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, file_size_index)->text();;
        s1.item3 = clicked_item_tab_text;


        tagging_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_icon_index)->setIcon(iconNotes);
        m_tablewidget_obj->item(current_row, notes_value_index)->setText(text);

    }


    global_connection_manager_obj->pub_set_notes_in_snapshots_difference_db(INT_List,clicked_item_tab_text,table_name ,text);
    global_connection_manager_obj->pub_add_notes_for_snapshots_difference(tagging_record_list ,text);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("-Ends " , Q_FUNC_INFO);
}

void snapshots_loader::slot_act_remove_tags()
{
    recon_static_functions::app_debug("-Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList INT_List;
    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;


    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


        struct_global_tagging_tag_notes_db_updation s1;
        s1.record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = plugin_name;
        s1.tab_name = display_tab_name;
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item3 = clicked_item_tab_text;

        tagging_record_list << s1;

        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);
    }

    global_connection_manager_obj->pub_remove_tag_in_snapshots_difference_db(INT_List,clicked_item_tab_text,table_name);
    global_connection_manager_obj->pub_delete_tag_for_snapshots_difference(tagging_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug("-Ends " , Q_FUNC_INFO);
}

void snapshots_loader::slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QString dest_dir_path = export_dir_path;

    QStringList list_paths_to_message_box;

    QList<int> rows_list_to_export;


    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QModelIndex model_index = selection_model_list.at(row);
        int selected_row_no = model_index.row();
        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        rows_list_to_export.append(selected_row_no);
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
        obj.display_file_path = display_path;
        obj.displayed_file_name = m_file_name;


        QString source_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,destination_db_path,table_name,source_count_name,Q_FUNC_INFO);


        QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_dir_path,m_file_name , Q_FUNC_INFO);
        if(!exported_file_path.trimmed().isEmpty())
            list_paths_to_message_box << exported_file_path;

    }



    display_loading_progress_bar_obj->hide();

    if(list_paths_to_message_box.size() > 0)
    {
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_dir_path);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void snapshots_loader::slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path,bool bool_is_enc_zip,QString password_enc_zip)
{
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    file_export_dialog_obj->hide();
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QList<struct_global_file_for_zip_export> files_list_for_zip_export;

    QStringList files_which_do_not_exist;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();
        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        struct_global_file_for_zip_export st_file_info_for_export_obj;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();
        QString filename = m_tablewidget_obj->item(selected_row_no, file_name_index)->text().trimmed();
        QString source_count_name_str = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();

        QString display_file_path =  m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();

        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,destination_db_path,table_name,source_count_name_str,Q_FUNC_INFO);

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            files_which_do_not_exist << filename;
            continue;
        }
        else
        {
            st_file_info_for_export_obj.source_path = complete_path;
            st_file_info_for_export_obj.destination_path = source_count_name_str + display_file_path;
            files_list_for_zip_export.append(st_file_info_for_export_obj);
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

    zip_export_file_path.clear();
    zip_export_file_path = export_zip_file_path;
    bool_zip_export_done = false;

    thread_zip_file_export_obj->pub_set_essentials(export_zip_file_path, files_list_for_zip_export, bool_is_enc_zip, password_enc_zip);
    thread_zip_file_export_obj->start();
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void snapshots_loader::slot_thread_zip_file_export_finished()
{
    display_loading_progress_bar_obj->hide();

    if(bool_cancel_loading)
        return;

    bool_zip_export_done = true;

    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) exported to zip successfully. Do you want to open it?");
    message_dialog_obj->show();

}

void snapshots_loader::slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_file_path)
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
    dmg_export_file_path.clear();

    QString dest_path = export_dmg_file_path;

    QStringList list_paths_to_message_box,source_count_name_list;
    QString exported_file_path,parent_dir,tmp_folder_path,available_dmg_image;

    bool bool_is_image_created;

    QString current_timestamp;
    current_timestamp = QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();

        QString display_file_path =  m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();

        QFileInfo a_info(display_file_path);
        parent_dir = a_info.absolutePath();
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

        QString m_file_name = m_tablewidget_obj->item(selected_row_no, file_name_index)->text();

        struct_global_export_file_system_record obj;
        obj.record_no = record_no_str;
        obj.source_count_name = source_count_name;
        obj.display_file_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();
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

void snapshots_loader::slot_message_box_yes_no_clicked(bool yes_no_clicked)
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

void snapshots_loader::point_to_export_dir(QString path)
{
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void snapshots_loader::slot_goto_snapshots_record(QString record_str, QString tab_name)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    record_str = record_str.trimmed();
    tab_name = tab_name.trimmed();
    if(record_str.isEmpty() || tab_name.isEmpty())
        return;

    for(int i = 0; i < main_tab_widget_obj->count(); i++)
    {
        if(tab_name.trimmed() == main_tab_widget_obj->tabText(i).trimmed())
        {
            main_tab_widget_obj->setCurrentIndex(i);
            m_tablewidget_obj = list_tablewidgets.at(i);
            for(int row_count = 0; row_count < m_tablewidget_obj->rowCount(); row_count++)
            {
                if(record_str == m_tablewidget_obj->item(row_count,record_no_index)->text().trimmed())
                {
                    m_tablewidget_obj->setAutoScroll(true);
                    m_tablewidget_obj->setCurrentCell(row_count, 0);
                    m_tablewidget_obj->selectRow(row_count);
                    tablewidget_general_clicked(row_count,record_no_index);
                    break;
                }
            }
            break;
        }
    }
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}
void snapshots_loader::action_goto_source_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    qint64 current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString display_file_path = m_tablewidget_obj->item(current_row ,file_path_index)->text();
    QString source_count_name = m_tablewidget_obj->item(current_row ,source_count_name_index)->text();

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
    QString command = QString("Select INT from'") + QString("files") + QString("' where filepath = ?");
    QStringList arg_list;
    arg_list  << display_file_path;
    QString fs_record_no =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,fs_db_path,Q_FUNC_INFO);
    if(fs_record_no.trimmed().isEmpty())
    {
        return ;
        recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
    }

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_dsply = struct_info.os_scheme_display;
    QString fs_source_name = struct_info.source_name;
    emit signal_tablewidget_goto_record(fs_source_name,QString(MACRO_CASE_TREE_Source),display_file_path,fs_record_no , fs_db_path, os_schme_dsply);


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void snapshots_loader::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);
    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    QList<struct_search_column_text_for_file_type> search_text_col_QList;

    int tab_index = main_tab_widget_obj->currentIndex();

    if(tab_custom_header_snapshot_widgets_QMap.size() <= 1)
        return;

    custom_header_widgets_snapshot_QList = tab_custom_header_snapshot_widgets_QMap.value(tab_index);

    for(int i = 0 ; i < custom_header_widgets_snapshot_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_snapshot_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_file_type struct_search_column_text_for_file_type_obj;
            struct_search_column_text_for_file_type_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_file_type_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_file_type_obj);
            bool_stop_sorting_tags_bookmark_notes_after_search = true;
        }
    }

    for(int ii = 0 ; ii < list_tablewidgets.size() ; ii++)
    {
        if(main_tab_widget_obj->currentIndex() == ii)
        {
            current_table_widget = list_tablewidgets.at(ii);
        }
    }

    if(search_text_col_QList.size() < 1)
    {
        bool_cancel_loading = false;
        display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_obj->show();

        for(int row = 0; row < current_table_widget->rowCount(); row++)
        {
            current_table_widget->showRow(row);
        }
        bool_stop_sorting_tags_bookmark_notes_after_search = false;

        display_loading_progress_bar_obj->hide();
        return;
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    all_column_text_search(search_text_col_QList);

    refresh_horizontal_header();

    emit signal_clear_all_metadata();
    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void snapshots_loader::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    m_tablewidget *current_table_widget;

    for(int ii = 0 ; ii < list_tablewidgets.size() ; ii++)
    {
        if(main_tab_widget_obj->currentIndex() == ii)
        {
            current_table_widget = list_tablewidgets.at(ii);
        }
    }

    if(column != enum_tablewidget_column_bookmark && column != enum_tablewidget_column_notes && column != enum_tablewidget_column_tags)
    {
        if(column_sort_order == 0)
            current_table_widget->sortItems(column, Qt::DescendingOrder);
        else
            current_table_widget->sortItems(column, Qt::AscendingOrder);

        refresh_horizontal_header();
        return;
    }

    if(bool_stop_sorting_tags_bookmark_notes_after_search)
        return;

    current_table_widget->clearSelection();

    if(current_table_widget->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = current_table_widget->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < current_table_widget->rowCount(); row++)
        {
            if(current_table_widget->isRowHidden(row))
            {
                continue;
            }

            if(current_table_widget->item(row, column) == NULL)
                continue;

            QIcon icon_set;
            if(column != enum_tablewidget_column_bookmark)
                icon_set =  current_table_widget->item(row, column)->icon();



            current_table_widget->item(row, column)->checkState();

            if(current_table_widget->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column != enum_tablewidget_column_bookmark)
                    icon_shiftrow_acs =  current_table_widget->item(row_shift_pos_ascending, column)->icon();

                if(current_table_widget->item(row_shift_pos_ascending, column)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                QList<QTableWidgetItem*> sourceItems = current_table_widget->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_table_widget->takeRow(destRow);

                current_table_widget->setRow(destRow, sourceItems);
                current_table_widget->setRow(sourceRow, destItems);

                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(current_table_widget->isRowHidden(row))
            {
                continue;
            }

            if(current_table_widget->item(row, column) == NULL)
                continue;

            if(current_table_widget->item(row_shift_pos_descending, column) == NULL)
                continue;

            QIcon icon_set;
            if(column != enum_tablewidget_column_bookmark)
                icon_set =  current_table_widget->item(row, column)->icon();

            if(current_table_widget->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column != enum_tablewidget_column_bookmark)
                    icon_shiftrow_desc =  current_table_widget->item(row_shift_pos_descending, column)->icon();


                if(current_table_widget->item(row_shift_pos_descending, column)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                QList<QTableWidgetItem*> sourceItems = current_table_widget->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_table_widget->takeRow(destRow);

                current_table_widget->setRow(destRow, sourceItems);
                current_table_widget->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::all_column_text_search(QList<struct_search_column_text_for_file_type> search_text_all_column_list)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    for(int row = 0; row < current_table_widget->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool hide_row = false;

        for(int column_count = 0; column_count < search_text_all_column_list.size(); column_count++)
        {
            struct_search_column_text_for_file_type struct_search_column_text_for_file_type_obj = search_text_all_column_list.at(column_count);

            if(!(current_table_widget->item(row, struct_search_column_text_for_file_type_obj.column_number)->text().contains(struct_search_column_text_for_file_type_obj.search_text, Qt::CaseInsensitive)))
            {
                hide_row = true;
                break;
            }
        }

        if(hide_row)
        {
            current_table_widget->hideRow(row);
        }
        else
        {
            current_table_widget->showRow(row);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::refresh_horizontal_header()
{
    m_tablewidget_multitabs->horizontalHeader()->hide();
    m_tablewidget_multitabs->horizontalHeader()->show();
    m_tablewidget_multitabs->horizontalScrollBar()->setValue(m_tablewidget_multitabs->horizontalScrollBar()->value() + 10);
    m_tablewidget_multitabs->horizontalScrollBar()->setValue(m_tablewidget_multitabs->horizontalScrollBar()->value() - 10);
}

void snapshots_loader::clear_all_custom_header_widget_entries()
{
    for(int ii = 0; ii < custom_header_widgets_snapshot_QList.size(); ii++)
    {
        custom_header_widgets_snapshot_QList.at(ii)->pub_clear_line_edit_search_field();
    }

    refresh_horizontal_header();
}
void snapshots_loader::slot_checkbox_timeline_toggled(bool checked)
{
    pushbutton_timeline->setEnabled(checked);

    if(checked)
    {
        search_keyword_from_db();
    }
    else
    {
        bool_timeline_is_selected = false;
        slot_pushbutton_showall_clicked();
    }

    slot_custom_table_header_widget_search_field_return_pressed();
}
