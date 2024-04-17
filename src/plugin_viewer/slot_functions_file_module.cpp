#include "plugin_viewer.h"
#include "ui_plugin_viewer.h"

void plugin_viewer::slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1)
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    if(list_tablewidgets.size() <= current_tab_index_only_for_tableview)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index_only_for_tableview);

    if(f1.INT.isEmpty())
    {
        my_tablewidget->clearSelection();
        emit signal_clear_all_metadata();
        return;
    }

    /// control goes to table view tab
    /// now all operations can be done on basis of m_tablewidget_obj

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index_only_for_tableview);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int current_row_selection_int = f1.table_view_row_no;

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(get_plugin_name(),current_tab_text);
    QString src_cnt_name = recon_helper_standard_obj->get_source_count_name_on_current_record(f1.INT, table_name, destination_db_file, Q_FUNC_INFO);

    for(int pp = 0 ; pp < my_tablewidget->rowCount() ; pp++)
    {

        if((f1.INT == my_tablewidget->item(pp,record_index)->text())
                && (f1.source_count_name == src_cnt_name))
        {
            current_row_selection_int = pp;
            break;
        }
    }

    my_tablewidget->selectRow(current_row_selection_int);


    bool bool_chkstate = false;

    if(f1.bookmark == MACRO_TRUE_VALUE_NUMERIC_QString)
        bool_chkstate = true;


    QString INT_str = f1.INT;

    QString tablewdgt_tab_nm =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(get_plugin_name(), table_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);
    if(bool_chkstate)
    {
        my_tablewidget->item(my_tablewidget->currentRow(),my_tablewidget->currentColumn())->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_plugin_viewer_db(QStringList(INT_str), get_plugin_name(), tablewdgt_tab_nm);
        global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(QStringList(INT_str), get_plugin_name(), tablewdgt_tab_nm);
        global_connection_manager_obj->pub_set_bookmarks_in_artifacts_keyword_search_db(QStringList(INT_str), get_plugin_name(), tablewdgt_tab_nm);

    }
    else
    {
        my_tablewidget->item(my_tablewidget->currentRow(),my_tablewidget->currentColumn())->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_plugin_viewer_db(QStringList(INT_str), get_plugin_name(), tablewdgt_tab_nm);
        global_connection_manager_obj->pub_delete_bookmark_for_plugin_viewer(QStringList(INT_str), get_plugin_name(), tablewdgt_tab_nm,"");
        global_connection_manager_obj->pub_remove_bookmarks_in_artifacts_keyword_search_db(QStringList(INT_str), get_plugin_name(), tablewdgt_tab_nm);

    }


}

void plugin_viewer::slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1)
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString record_no =  f1.INT;

    QString tbl_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(get_plugin_name(), current_tab_text);
    QString tab_name =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(get_plugin_name(), tbl_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);
    QString complete_file_path = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name,record_no,destination_db_file).complete_file_path;

    if(QFileInfo(complete_file_path).exists())
        emit signal_plugin_viewer_space_pressed_for_quicklook(complete_file_path);

}

void plugin_viewer::slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    slot_act_export();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}



void plugin_viewer::slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1)
{

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    if(list_tablewidgets.size() <= current_tab_index_only_for_tableview)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index_only_for_tableview);

    if(f1.INT.isEmpty())
    {

        my_tablewidget->clearSelection();

        emit signal_clear_all_metadata();
        return;
    }


    /// control goes to table view tab
    /// now all operations can be done on basis of my_tablewidget

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index_only_for_tableview);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QString table_name = table_name_list_tablewidget.at(current_tab_index_only_for_tableview);
    QString src_cnt_name = recon_helper_standard_obj->get_source_count_name_on_current_record(f1.INT, table_name, destination_db_file, Q_FUNC_INFO);


    int current_row_selection_int = f1.table_view_row_no;
    for(int pp = 0 ; pp < my_tablewidget->rowCount() ; pp++)
    {

        if((f1.INT == my_tablewidget->item(pp,record_index)->text())
                && (f1.source_count_name == src_cnt_name))
        {
            current_row_selection_int = pp;
            break;
        }
    }

    my_tablewidget->selectRow(current_row_selection_int);


}


void plugin_viewer::slot_gallery_view_invalid_tem_clicked()
{
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index_only_for_tableview);

    my_tablewidget->clearSelection();

    emit signal_clear_all_metadata();

}


void plugin_viewer::slot_table_widget_header_item_clicked(int column_index)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    //Note :- Because of new Gallery Viewer no need to do sorting, so returning this function.
    return;

    if(column_index < 0)
        return;


    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;


    if(st_gallery_view_file_info_QList_QList.size() <= 0)
        return;

    QList<struct_global_gallery_view_file_info> *st_gallery_view_file_info_list = st_gallery_view_file_info_QList_QList.at(current_tab_index_only_for_tableview);

    if(st_gallery_view_file_info_list == nullptr)
        return;


    if(list_tablewidgets.size() <= current_tab_index_only_for_tableview)
        return;


    m_tablewidget *my_tablewidget_obj = list_tablewidgets.at(current_tab_index_only_for_tableview);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index_only_for_tableview);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QList<struct_global_gallery_view_file_info>  *sorted_gallery_info_qlist = new QList<struct_global_gallery_view_file_info>;
    sorted_gallery_info_qlist->clear();

    QMap<int ,struct_global_gallery_view_file_info> index_row_qmap;
    index_row_qmap.clear();
    QList<int> row_no_QList;
    row_no_QList.clear();

    QString table_name = table_name_list_tablewidget.at(current_tab_index_only_for_tableview);

    for(int pp = 0 ; pp < st_gallery_view_file_info_list->size() ; pp++)
    {
        struct_global_gallery_view_file_info f1 = st_gallery_view_file_info_list->at(pp);

        for(int qq = 0 ; qq < my_tablewidget_obj->rowCount() ; qq++)
        {
            QString src_cnt_name = recon_helper_standard_obj->get_source_count_name_on_current_record(f1.INT, table_name, destination_db_file, Q_FUNC_INFO);

            if((f1.INT == my_tablewidget_obj->item(qq,record_index)->text())
                    && (f1.source_count_name == src_cnt_name))
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
        sorted_gallery_info_qlist->append(index_row_qmap.value(row_no_QList.at(ii)));
    }

    gallery_viewer *widget = list_thumbnail_viewer_widgets.at(current_tab_index_only_for_tableview);
    widget->pub_set_file_list(sorted_gallery_info_qlist);
    QCoreApplication::processEvents();


    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

}


void plugin_viewer::slot_thread_report_finished_or_cancelled()
{
    display_loading_progress_bar_for_reports_from_plugin_obj->hide();


    if(bool_cancel_loading_reports)
        return;

    thread_report_builder *m_obj = dynamic_cast<thread_report_builder*> (sender());

    QStringList filepath_list = m_obj->pub_get_report_filepath_list();

    for(int i = 0; i < filepath_list.size(); i++)
    {
        QFileInfo info(filepath_list.at(i));

        if(info.exists())
        {
            QString report_dir_path = info.absolutePath();

            QProcess proc;

            QStringList args;
            args << "-R" << "0777" << report_dir_path;;

            proc.execute("chmod", args);

            QCoreApplication::processEvents();
        }
    }

    QString filepath;
    if(filepath_list.size() > 0)
        filepath = filepath_list.at(0);

    report_status obj;
    if(obj.exec())
    {
        QStringList arg;
        arg << filepath;

        QProcess *myProcess = new QProcess(this);
        myProcess->startDetached("open",arg);
    }


}

void plugin_viewer::slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str)
{
    recon_static_functions::app_debug ("[slot_hashset_storage_workdone_clicked]-Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    for(int i = 0; i < list_dbs_addmenu.size() ; i++)
    {
        QStringList list = list_dbs_addmenu.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
        if(list.size() != 2)
            continue;

        QString db_name = list.at(0);
        QString db_path = list.at(1);

        if(recon_static_functions::check_existance_of_action_in_menu(menu_hashset_add_files, db_name, db_path))
            continue;

        QAction *act_add_file = new QAction(tr(db_name.toLocal8Bit().data()), this->menu_hashset_add_files);
        act_add_file->setToolTip(db_path);

        menu_hashset_add_files->addAction(act_add_file);


        if(callfrom_str == MACRO_Hashset_New_Hash_Created)
        {
            int current_tab_index = current_tab_index_only_for_tableview;

            QString table_name;

            QStringList listtabs;
            //listtabs << ui->tabWidget->tabText(current_tab_index);
            listtabs << get_exact_tab_name_from_ui(current_tab_index);
            QStringList table_name_list =  global_csv_reader_class_selected_obj->get_tables_name_according_tabs(get_plugin_name(), listtabs);

            if(table_name_list.size() == 1)
                table_name =  table_name_list.at(0);

            m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index);

            QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index);
            int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

            QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();
            for(int i = 0; i < selection_model_list.size(); i++)
            {
                if(i % 500 == 0)
                    QCoreApplication::processEvents();

                QModelIndex model_index = selection_model_list.at(i);

                int selected_row_no = model_index.row();

                if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
                    continue;

                QString record_no =  my_tablewidget->item(selected_row_no,record_index)->text();

                int table_index = table_name_list_tablewidget.indexOf(table_name);
                QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);
                struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
                QStringList path_list = obj.complete_export_filepaths;
                QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_file, Q_FUNC_INFO);

                if(path_list.size() == 0)
                {
                    hashset_storage_obj->hide();
                    return;
                }

                for(int i = 0 ; i < path_list.size() ; i++)
                {
                    QString file_path = path_list.at(i);
                    QFileInfo info(file_path);
                    if(!info.exists() && (path_list.size() == 1))
                    {
                        recon_static_functions::app_debug(" file does not exist ----" + file_path,Q_FUNC_INFO);
                        hashset_storage_obj->hide();
                        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
                        message_dialog_object->show();

                        continue;
                    }

                    hashset_storage_obj->insert_file_details_into_hashset_database(file_path,db_path,source_count_name);
                }
            }

            emit signal_update_created_hashset_db_into_other_submenu_from_plugin(db_name, db_path);
        }
    }

    hashset_storage_obj->hide();
    recon_static_functions::app_debug ("[slot_hashset_storage_workdone_clicked]-END",Q_FUNC_INFO);
}


void plugin_viewer::slot_hashset_menu_add_file_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug ("[slot_hashset_menu_add_file_clicked]-Starts",Q_FUNC_INFO);

    if(current_clicked_action->text() == MACRO_Generic_Right_Click_Create_Hashset)
    {
        hashset_storage_obj->show();
        return;
    }
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int current_tab_index = current_tab_index_only_for_tableview;

    QString table_name;

    QStringList listtabs;
    //listtabs << ui->tabWidget->tabText(current_tab_index);
    listtabs << get_exact_tab_name_from_ui(current_tab_index);
    QStringList table_name_list =  global_csv_reader_class_selected_obj->get_tables_name_according_tabs(get_plugin_name(), listtabs);

    if(table_name_list.size() == 1)
        table_name =  table_name_list.at(0);

    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;

        QString record_no =  my_tablewidget->item(selected_row_no,record_index)->text();

        int table_index = table_name_list_tablewidget.indexOf(table_name);
        QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);
        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
        QStringList path_list = obj.complete_export_filepaths;
        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_file, Q_FUNC_INFO);

        if(path_list.size() == 0)
        {
            hashset_storage_obj->hide();
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION," Please select an exported record");
            message_dialog_object->show();
            continue;
        }

        for(int i = 0 ; i < path_list.size() ; i++)
        {
            QString file_path = path_list.at(i);

            QFileInfo info(file_path);
            if(!info.exists() && (path_list.size() == 1))
            {
                recon_static_functions::app_debug(" file does not exist ----" + file_path,Q_FUNC_INFO);
                hashset_storage_obj->hide();
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
                message_dialog_object->show();

                continue;
            }

            QString dbpath = current_clicked_action->toolTip();

            QFileInfo info2(dbpath);
            QString dbname = info2.fileName();
            QStringList list_db;

            QString db_str = dbname.remove(".sqlite") + MACRO_RECON_Splitter_1_multiple + dbpath;
            list_db << db_str;

            hashset_storage_obj->insert_file_details_into_hashset_database(file_path,dbpath,source_count_name);
        }
    }

    recon_static_functions::app_debug ("[slot_hashset_menu_add_file_clicked]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_action_search_file_with_same_hash_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString run_status_str = recon_helper_standard_obj->pub_get_filesystem_modules_thread_run_status(QString(MACRO_JobType_Hashes), Q_FUNC_INFO);
    if(run_status_str.isEmpty() || run_status_str == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No source has been processed for 'Hashes' yet.");
        return;
    }

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);
    QString record_no =  my_tablewidget->item(my_tablewidget->currentRow(),2)->text();

    //QString tab_name_str = ui->tabWidget->tabText(current_tab);
    QString tab_name_str = get_exact_tab_name_from_ui(current_tab);
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name_str,record_no,destination_db_file);
    QString source_count_name = st_globl_recon_file_info_obj.source_count_name;
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name +"/file_system.sqlite";


    prepare_record_no_str_and_hash_md5_str_for_search_files_with_same_hash(fs_db_path, st_globl_recon_file_info_obj.display_file_path);


    search_files_with_hash_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
    search_files_with_hash_obj->pub_refresh_tableWidget();
    search_files_with_hash_obj->pub_set_display_loading_progress_bar_obj(display_loading_progress_bar_non_cancelable_obj);
    search_files_with_hash_obj->pub_search_files_having_same_hash(hash_md5_for_search_files_with_same_hash, record_no_for_search_files_with_same_hash, source_count_name, complete_path);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void plugin_viewer::slot_open_in_detach_triggered()
{
    recon_static_functions::app_debug ("[slot_open_in_detach_triggered]-START",Q_FUNC_INFO);


    //    int current_tab_index = ui->tabWidget->currentIndex();
    //    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index);


    //    QString table_name = table_name_list_tablewidget.at(current_tab_index);

    //    QStringList tw_col_type_list = tw_data_type_list_tablewidget.at(current_tab_index);
    //    int INT_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    //    QString INT_str = my_tablewidget->item(my_tablewidget->currentRow(), INT_index)->text();

    //    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(INT_str,table_name,destination_db_file,Q_FUNC_INFO);

    //    emit signal_open_in_detach_clicked(data_prepare_metadata, current_media, preview_file_path, preview_index_full,searched_keyword, source_count_name);

    recon_static_functions::app_debug ("[slot_open_in_detach_triggered]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_full_detail_info_triggered()
{
    recon_static_functions::app_debug ("-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int current_tab_index = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index);

    QString table_name = table_name_list_tablewidget.at(current_tab_index);
    QString rec =  my_tablewidget->item(my_tablewidget->currentRow(),2)->text();


    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    connect(full_detail_info_obj, SIGNAL(signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString, QString)), this, SLOT(slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString, QString)));

    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);
    QString tab_name = tab_name_list_tablewidget.at(current_tab_index);
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name,rec ,destination_db_file);
    QString file_path = st_globl_recon_file_info_obj.complete_file_path;
    QString source_count_name = st_globl_recon_file_info_obj.source_count_name;
    QString display_file_path = st_globl_recon_file_info_obj.display_file_path;

    preview_index_full = get_stack_index_for_preview(file_path,display_file_path);

    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(data_prepare_metadata, file_path, preview_index_full,searched_keyword,source_count_name, rec, get_plugin_name(), table_name);

    preview_index_full = 0;

    full_detail_info_obj->show();

    recon_static_functions::app_debug (" -END",Q_FUNC_INFO);
}

int plugin_viewer::get_stack_index_for_preview(QString current_media_path, QString display_file_path)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QMimeDatabase mime_database_obj;
    int preview_index_full;
    preview_index_full = enum_global_preview_stack_widget_BLANK;

    current_media_path = current_media_path.trimmed();

    if(!current_media_path.isEmpty())
    {
        QFileInfo info(current_media_path);
        if(info.exists())
        {

            QString mime_type_for_file = mime_database_obj.mimeTypeForFile(current_media_path).name();


            if(recon_helper_standard_obj->check_is_it_image_file(current_media_path,display_file_path, Q_FUNC_INFO))
            {
                preview_index_full = enum_global_preview_stack_widget_IMAGE;
            }

            if(mime_type_for_file.startsWith("video/"))
            {
                preview_index_full = enum_global_preview_stack_widget_Audio_Video;
            }
        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return preview_index_full;
}

void plugin_viewer::slot_sort_bookmarked(int index)
{
    return;

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int current_tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= current_tab_index)
        return;


    total_covered_count = 0;
    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;


    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index);

    //    QString search_string;
    //    search_string = "'%"+ ui->lineEdit_search->text().trimmed() +"%'";

    QString search_string;
    QString keyword = ui->lineEdit_search->text().trimmed();
    QStringList search_string_list = recon_static_functions::get_search_string_list(keyword, Q_FUNC_INFO);

    if(index > 1)
    {
        my_tablewidget->sortByColumn(index,Qt::AscendingOrder);
        my_tablewidget->setSortingEnabled(false);
        return;
    }


    if((index == 0) || (index == 1))
    {
        disable_action_widgets(true);

        while(my_tablewidget->rowCount())
            my_tablewidget->removeRow(0);

        QString table_name = table_name_list_tablewidget.at(current_tab_index);
        QString tab_name = tab_name_list_tablewidget.at(current_tab_index);
        QStringList cols_nam_list =  tw_column_name_list_tablewidget.at(current_tab_index);
        QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index);
        QStringList search_cols_list = ms_search_local_list_tablewidget.at(current_tab_index);

        QStringList cols_width_list = tw_col_width_list_tablewidget.at(current_tab_index);

        // adding notes because of notes visibily for display is not set, and we want to show icon of notes on_pushbutton_showall_click
        cols_nam_list.insert(cols_nam_list.size(), "notes");
        cols_type_list.insert(cols_type_list.size(), MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString);

        QString command = cols_nam_list.join(",");
        command = "select " + command + " from " + table_name;

        QString partial_command = recon_static_functions::get_partial_command_forsearch(search_cols_list,search_string_list, Q_FUNC_INFO);
        command = command + partial_command;

        if(bool_sort_bookmarked[current_tab_index])
        {
            if(index == 0)
                command = command + " ORDER BY bookmark DESC";
            else
                command = command + " ORDER BY recon_tag_value DESC";

            int timeline_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
            int notes_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString,0);

            int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
            draw_table(command , current_tab_index, cols_nam_list.size(),cols_type_list, timeline_col_index, notes_col_index,os_scheme_index, cols_width_list,search_cols_list,search_string_list);

            bool_sort_bookmarked[current_tab_index] = false;
        }
        else
        {

            if(index == 0)
                command = command + " ORDER BY bookmark ASC";
            else
                command = command + " ORDER BY recon_tag_value ASC";

            int timeline_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
            int notes_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString,0);

            int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
            draw_table(command , current_tab_index, cols_nam_list.size(),cols_type_list, timeline_col_index, notes_col_index,os_scheme_index, cols_width_list,search_cols_list,search_string_list);

            bool_sort_bookmarked[current_tab_index] = true;
        }
        disable_action_widgets(false);
    }

    display_loading_progress_bar_obj->hide();

}

void plugin_viewer::slot_fixed_last_blank_column(int col_index)
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int current_tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= current_tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index);

    if(tw_header_list_tablewidget.at(current_tab_index).size() == col_index)
        my_tablewidget->horizontalHeader()->setSectionsMovable(false);
    else
        my_tablewidget->horizontalHeader()->setSectionsMovable(true);
}

void plugin_viewer::slot_act_extract_plist_viewer_openwith()
{
    recon_static_functions::app_debug ("[slot_act_extract_plist_viewer_openwith]-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");


    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);
    QString rec =  my_tablewidget->item(my_tablewidget->currentRow(),2)->text();
    //QString tab_name_str = ui->tabWidget->tabText(current_tab);
    QString tab_name_str = get_exact_tab_name_from_ui(current_tab);
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name_str,rec ,destination_db_file);
    QString source_count_name = st_globl_recon_file_info_obj.source_count_name;

    QStringList all_path_list = QStringList(st_globl_recon_file_info_obj.complete_file_path);

    for(int i =0 ; i< all_path_list.size() ; i++)
    {
        QString complete_path = all_path_list.at(i);

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(QString("[slot_act_extract_plist_viewer_openwith] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
            emit signal_statusbar_progress_bar_show_hide(false, "");
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,  "'" +  info.fileName() + "' File not exist.");
            message_dialog_object->show();

            continue;
        }

        if(!recon_static_functions::check_is_it_plist_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            emit signal_statusbar_progress_bar_show_hide(false, "");
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' is not plist file.");
            message_dialog_object->show();
            continue;
        }
        else
        {

            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString plist_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

            QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
            if(list_plist_display_name.contains(plist_display_nm))
            {
                emit signal_show_file_in_viewer_master_frame(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
                continue;
            }


            plist_viewer_obj = new plist_viewer(this);
            //QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + ui->tabWidget->tabText(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
            QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + get_exact_tab_name_from_ui(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
            plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
            plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
            plist_viewer_obj->pub_set_plist_path(complete_path);
            plist_viewer_obj->pub_set_source_count_name(source_count_name);
            plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            plist_viewer_obj->pub_set_essentials();

            plist_viewer_obj->pub_start_plist_extraction();


            emit signal_show_file_in_viewer_master_frame(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");
    recon_static_functions::app_debug ("[slot_act_extract_plist_viewer_openwith]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_extract_log_viewer_openwith()
{
    recon_static_functions::app_debug (" START ",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString adhoc_log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString).toString();

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);
    QString rec =  my_tablewidget->item(my_tablewidget->currentRow(),2)->text();
    QString tab_name_str = get_exact_tab_name_from_ui(current_tab);
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name_str,rec,destination_db_file);
    QString source_count_name = st_globl_recon_file_info_obj.source_count_name;

    QStringList all_path_list = QStringList(st_globl_recon_file_info_obj.complete_file_path);

    for(int i = 0; i < all_path_list.size(); i++)
    {
        QString log_file_complete_path = all_path_list.at(i);

        QFileInfo info(log_file_complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(QString("[slot_act_extract_log_viewer_openwith] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
            emit signal_statusbar_progress_bar_show_hide(false, "");
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,  "'" +  info.fileName() + "' File not exist.");
            message_dialog_object->show();

            continue;
        }

        QString temp_p = log_file_complete_path;
        temp_p = temp_p.toLower();
        if(!temp_p.endsWith(".log"))
        {
            emit signal_statusbar_progress_bar_show_hide(false, "");
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' is not log file.");
            message_dialog_object->show();
            continue;
        }
        else
        {
            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString log_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

            QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(adhoc_log_viewer_sqlite_path, Q_FUNC_INFO);
            if(list_log_display_name.contains(log_display_nm))
            {
                emit signal_show_file_in_viewer_master_frame(log_display_nm,MACRO_CASE_TREE_Log_Viewer, source_count_name);
                continue;
            }

            log_viewer *log_viewer_obj = new log_viewer(this);
            QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + get_exact_tab_name_from_ui(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
            log_viewer_obj->pub_set_result_log_viewer_sqlite_path(adhoc_log_viewer_sqlite_path);
            log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
            log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
            log_viewer_obj->pub_set_source_count_name(source_count_name);
            log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            log_viewer_obj->pub_set_essentials();

            log_viewer_obj->pub_start_log_extraction();

            emit signal_show_file_in_viewer_master_frame(log_display_nm,MACRO_CASE_TREE_Log_Viewer, source_count_name);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");
    recon_static_functions::app_debug (" END ",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_log_viewer_sendtobucket()
{
    recon_static_functions::app_debug (" START ",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        QString rec =  my_tablewidget->item(selected_row_no,record_index)->text();
        QString table_name = table_name_list_tablewidget.at(current_tab);
        int table_index = table_name_list_tablewidget.indexOf(table_name);

        QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);
        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,rec,Q_FUNC_INFO);
        QStringList all_path_list = obj.complete_export_filepaths;
        QStringList display_file_path_list = obj.displayed_file_path;
        QStringList recon_filefrom_list = obj.recon_filefrom_list;
        QStringList recon_file_info_path_list = obj.recon_file_info_path_list;

        for(int i = 0; i < all_path_list.size(); i++)
        {
            QString log_file_complete_path = all_path_list.at(i);
            struct_global_recon_file_info st_globl_recon_file_info_obj;

            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
            }

            QFileInfo info(log_file_complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(QString("[slot_act_log_viewer_sendtobucket] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
                message_dialog_object->show();

                continue;
            }

            QString temp_p = log_file_complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".log"))
            {
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' is not log file.");
                message_dialog_object->show();

                continue;
            }
            else
            {
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString table_name = table_name_list_tablewidget.at(current_tab);
                QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(rec,table_name,destination_db_file,Q_FUNC_INFO);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString log_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

                QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(log_viewer_sqlite_path, Q_FUNC_INFO);
                if(list_log_display_name.contains(log_display_nm))
                {
                    continue;
                }

                log_viewer *log_viewer_obj = new log_viewer(this);
                QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + get_exact_tab_name_from_ui(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;

                log_viewer_obj->pub_set_result_log_viewer_sqlite_path(log_viewer_sqlite_path);
                log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
                log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
                log_viewer_obj->pub_set_source_count_name(source_count_name);
                log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                log_viewer_obj->pub_set_essentials();

                log_viewer_obj->pub_start_log_extraction();

                emit signal_viewer_extarction_completed(log_display_nm,MACRO_CASE_TREE_Log_Viewer);
            }
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug (" END ",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_plist_viewer_sendtobucket()
{
    recon_static_functions::app_debug ("[slot_act_plist_viewer_sendtobucket]-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        QString rec =  my_tablewidget->item(selected_row_no,record_index)->text();
        QString table_name = table_name_list_tablewidget.at(current_tab);
        int table_index = table_name_list_tablewidget.indexOf(table_name);

        QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);
        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,rec,Q_FUNC_INFO);
        QStringList all_path_list = obj.complete_export_filepaths;
        QStringList display_file_path_list = obj.displayed_file_path;
        QStringList recon_filefrom_list = obj.recon_filefrom_list;
        QStringList recon_file_info_path_list = obj.recon_file_info_path_list;

        for(int i =0 ; i< all_path_list.size() ; i++)
        {
            QString complete_path = all_path_list.at(i);
            struct_global_recon_file_info st_globl_recon_file_info_obj;

            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
            }

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(QString("[slot_act_plist_viewer_sendtobucket] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
                message_dialog_object->show();

                continue;
            }

            if(!recon_static_functions::check_is_it_plist_file_by_signature(complete_path, Q_FUNC_INFO))
            {
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' is not plist file.");
                message_dialog_object->show();

                continue;
            }
            else
            {
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString table_name = table_name_list_tablewidget.at(current_tab);
                QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(rec,table_name,destination_db_file,Q_FUNC_INFO);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString plist_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;


                QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
                if(list_plist_display_name.contains(plist_display_nm))
                {
                    continue;
                }

                plist_viewer_obj = new plist_viewer(this);
                //QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + ui->tabWidget->tabText(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
                QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + get_exact_tab_name_from_ui(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;

                plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
                plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
                plist_viewer_obj->pub_set_plist_path(complete_path);
                plist_viewer_obj->pub_set_source_count_name(source_count_name);
                plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                plist_viewer_obj->pub_set_essentials();

                plist_viewer_obj->pub_start_plist_extraction();

                emit signal_viewer_extarction_completed(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer);
            }
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug ("[slot_act_plist_viewer_sendtobucket]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_extract_sqlite_viewer_openwith()
{
    recon_static_functions::app_debug("[slot_act_extract_sqlite_viewer_openwith]-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget*my_tablewidget = list_tablewidgets.at(current_tab);
    QString rec =  my_tablewidget->item(my_tablewidget->currentRow(),2)->text();
    //QString tab_name_str = ui->tabWidget->tabText(current_tab);
    QString tab_name_str = get_exact_tab_name_from_ui(current_tab);
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name_str,rec ,destination_db_file);
    QString source_count_name = st_globl_recon_file_info_obj.source_count_name;

    QStringList all_path_list;
    all_path_list << st_globl_recon_file_info_obj.complete_file_path;

    for(int i =0 ; i< all_path_list.size() ; i++)
    {

        QString complete_path = all_path_list.at(i);

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(QString("[slot_act_extract_sqlite_viewer_openwith] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
            message_dialog_object->show();

            continue;
        }

        if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not sqlite file.");
            message_dialog_object->show();

            continue;
        }
        else
        {
            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString sqlite_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

            QString recon_dir_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";
            QStringList sqlite_viewer_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(recon_dir_sqlite_path, Q_FUNC_INFO);

            if(sqlite_viewer_display_name.size() > 0){

                if(sqlite_viewer_display_name.contains(sqlite_display_nm)){
                    emit signal_openwith_after_sendto_bucket_master_frame(sqlite_viewer_display_name.at(0),MACRO_CASE_TREE_Sqlite_Viewer);
                    break;
                }

            }

            QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
            if(list_sqlite_display_name.contains(sqlite_display_nm))
            {
                emit signal_show_file_in_viewer_master_frame(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
                continue;
            }

            //QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + ui->tabWidget->tabText(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
            QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + get_exact_tab_name_from_ui(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;

            sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
            sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
            sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
            sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(recon_result_dir_path);
            sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
            sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
            sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            sqlite_viewer_starter_obj->pub_set_essentials();

            sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

            emit signal_show_file_in_viewer_master_frame(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");
    recon_static_functions::app_debug ("[slot_act_extract_sqlite_viewer_openwith]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_sqlite_viewer_sendtobucket()
{
    recon_static_functions::app_debug ("[slot_act_sqlite_viewer_sendtobucket]-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;

        QString rec =  my_tablewidget->item(selected_row_no,record_index)->text();
        QString table_name = table_name_list_tablewidget.at(current_tab);
        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(rec,table_name,destination_db_file,Q_FUNC_INFO);

        int table_index = table_name_list_tablewidget.indexOf(table_name);

        QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);
        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,rec,Q_FUNC_INFO);
        QStringList all_path_list = obj.complete_export_filepaths;
        QStringList display_file_path_list = obj.displayed_file_path;
        QStringList recon_filefrom_list = obj.recon_filefrom_list;
        QStringList recon_file_info_path_list = obj.recon_file_info_path_list;

        for(int i =0 ; i< all_path_list.size() ; i++)
        {
            QString complete_path = all_path_list.at(i);

            struct_global_recon_file_info st_globl_recon_file_info_obj;

            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
            }

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(QString("[slot_act_sqlite_viewer_sendtobucket] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
                message_dialog_object->show();

                continue;
            }

            if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
            {
                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" + info.fileName() + "' is not sqlite file.");
                message_dialog_object->show();

                continue;
            }
            else
            {
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString sqlite_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

                QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
                if(list_sqlite_display_name.contains(sqlite_display_nm))
                {
                    continue;
                }

                // QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + ui->tabWidget->tabText(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
                QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + get_plugin_name() + MACRO_RECON_Splitter_1_multiple + get_exact_tab_name_from_ui(current_tab) + MACRO_RECON_Splitter_1_multiple + rec;
                sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
                sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
                sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
                sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(recon_result_dir_path);
                sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
                sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
                sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                sqlite_viewer_starter_obj->pub_set_essentials();

                sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

                emit signal_viewer_extarction_completed(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer);
            }
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");
    recon_static_functions::app_debug ("[slot_act_sqlite_viewer_sendtobucket]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_hex_viewer_openwith()
{
    recon_static_functions::app_debug ("[slot_act_hex_viewer_openwith]-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");


    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);
    QString rec =  my_tablewidget->item(my_tablewidget->currentRow(),2)->text();
    //QString tab_name_str = ui->tabWidget->tabText(current_tab);
    QString tab_name_str  = get_exact_tab_name_from_ui(current_tab);
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),tab_name_str,rec ,destination_db_file);
    QString source_count_name = st_globl_recon_file_info_obj.source_count_name;
    QStringList all_path_list;
    all_path_list << st_globl_recon_file_info_obj.complete_file_path;

    for(int i =0 ; i< all_path_list.size() ; i++)
    {

        QString complete_path = all_path_list.at(i);

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(QString("[slot_act_hex_viewer_openwith] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
            emit signal_statusbar_progress_bar_show_hide(false, "");
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
            message_dialog_object->show();

            continue;
        }

        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);


        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString hex_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

        QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_hex_display_name.contains(hex_display_nm))
        {
            emit signal_show_file_in_viewer_master_frame(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer, source_count_name);
            continue;
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
        hex_viewer_open_file_data_obj.record_no_str       = rec;
        hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
        hex_viewer_open_file_data_obj.plugin_name_str     = get_plugin_name();
        hex_viewer_open_file_data_obj.plugin_tab_name_str = current_tab_text;
        hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
        hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

        emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);


    }

    emit signal_statusbar_progress_bar_show_hide(false, "");
    recon_static_functions::app_debug ("[slot_act_hex_viewer_openwith]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_message_box_yes_no_recived(bool value)
{
    bool_message_box_yes_or_no_receiver = value;
}


void plugin_viewer::slot_act_hex_viewer_sendtobucket()
{
    recon_static_functions::app_debug ("[slot_act_hex_viewer_sendtobucket]-START",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");


    int current_tab = current_tab_index_only_for_tableview;
    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QString table_name = table_name_list_tablewidget.at(current_tab);

    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;

        QString rec =  my_tablewidget->item(selected_row_no,record_index)->text();

        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(rec,table_name,destination_db_file,Q_FUNC_INFO);

        QString table_name = table_name_list_tablewidget.at(current_tab);
        int table_index = table_name_list_tablewidget.indexOf(table_name);

        QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);
        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,rec,Q_FUNC_INFO);
        QStringList all_path_list = obj.complete_export_filepaths;
        QStringList display_file_path_list = obj.displayed_file_path;
        QStringList recon_file_info_path_list = obj.recon_file_info_path_list;
        QStringList recon_filefrom_list = obj.recon_filefrom_list;

        for(int i =0 ; i< all_path_list.size() ; i++)
        {
            QString complete_path = all_path_list.at(i);

            struct_global_recon_file_info st_globl_recon_file_info_obj;

            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
            }

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(QString("[slot_act_hex_viewer_sendtobucket] file does not exist path -->>") + info.filePath() ,Q_FUNC_INFO);
                emit signal_statusbar_progress_bar_show_hide(false, "");

                message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist.");
                message_dialog_object->show();

                continue;
            }

            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString hex_display_nm = display_file_name + " [" + get_plugin_name() + " " + rec + "]" + src_cnt_name_label;

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
            hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
            hex_viewer_open_file_data_obj.record_no_str       = rec;
            hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
            hex_viewer_open_file_data_obj.plugin_name_str     = get_plugin_name();
            hex_viewer_open_file_data_obj.plugin_tab_name_str = current_tab_text;
            hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
            hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

            recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

            emit signal_viewer_extarction_completed(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");
    recon_static_functions::app_debug ("[slot_act_hex_viewer_sendtobucket]-END",Q_FUNC_INFO);
}


void plugin_viewer::slot_act_manage_columns()
{
    recon_static_functions::app_debug ("[slot_act_manage_columns]-START",Q_FUNC_INFO);

    int tab_index = ui->tabWidget->currentIndex();

    QList<custom_table_header_widget *> custom_header_widgets_QList;

    custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(tab_index);

    int column_count = custom_header_widgets_QList.size();

    QStringList cols_to_show_inpopup;
    cols_to_show_inpopup.clear();

    for(int i=0;i < column_count;i++)
    {
        if((i == 0) || (i == 1) || (i == 2))
            continue;

        QString column_text = ((custom_table_header_widget *)(custom_header_widgets_QList.at(i)))->pub_get_column_name();
        if(column_text == QString(Macro_Generic_OS_Scheme))
            continue;

        cols_to_show_inpopup << column_text;
    }

    QList<struct_manage_column_info> column_no_and_name_list;
    column_no_and_name_list.clear();

    column_no_and_name_list = get_hidden_column_from_manage_column_db();

    manage_plugin_columns_obj->popup_hid_col_lst_var.clear();
    manage_plugin_columns_obj->popup_hid_col_name_lst_var.clear();

    for(int i=0;i< column_no_and_name_list.size();i++){
        struct_manage_column_info var = column_no_and_name_list.at(i);

        manage_plugin_columns_obj->popup_hid_col_lst_var << var.hid_col_no;
        manage_plugin_columns_obj->popup_hid_col_name_lst_var << var.hid_col_name;
    }

    manage_plugin_columns_obj->setWindowTitle("Manage Columns");
    manage_plugin_columns_obj->draw_table_manage_plugins_columns(cols_to_show_inpopup);

    manage_plugin_columns_obj->show();

    recon_static_functions::app_debug ("[slot_act_manage_columns]-END",Q_FUNC_INFO);
}


void plugin_viewer::slot_arrow_key_pressed()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    tablewidget_general_clicked(tab_index,my_tablewidget->currentRow(),1);

}

void plugin_viewer::slot_bookmark_key_pressed_tablewidget()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    if(my_tablewidget->item(my_tablewidget->currentRow(), 0)->checkState())
        my_tablewidget->item(my_tablewidget->currentRow(), 0)->setCheckState(Qt::Unchecked);
    else
        my_tablewidget->item(my_tablewidget->currentRow(), 0)->setCheckState(Qt::Checked);

    tablewidget_general_clicked(tab_index,my_tablewidget->currentRow(),0);

}

void plugin_viewer::slot_control_e_key_pressed()
{
    slot_act_export();
}

void plugin_viewer::slot_act_remove_bookmark()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(get_plugin_name(),current_tab_text))
        emit signal_set_bookmark_status_to_gallery_view(false);

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        if(my_tablewidget->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        my_tablewidget->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

        QString INT_str = my_tablewidget->item(selected_row_no, record_index)->text();
        INT_list << INT_str;
        recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display, tab_name, INT_str);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_delete_bookmark_for_plugin_viewer(INT_list, get_plugin_name(), tab_name, "");
    global_connection_manager_obj->pub_remove_bookmarks_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_bookmark_triggered()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(get_plugin_name(),current_tab_text))
        emit signal_set_bookmark_status_to_gallery_view(true);

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        if(my_tablewidget->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        my_tablewidget->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

        QString INT_str = my_tablewidget->item(selected_row_no, record_index)->text();
        INT_list << INT_str;
        recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display, tab_name, INT_str);
    }

    global_connection_manager_obj->pub_set_bookmark_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_set_bookmarks_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_remove_all_bookmark()
{
    //    disable_action_widgets(true);

    //    QString connection_naam = QString(Q_FUNC_INFO);
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    destination_db.setDatabaseName(destination_db_file);

    //    if(!destination_db.open())
    //    {
    //        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
    //        disable_action_widgets(false);
    //        return;
    //    }

    //    QSqlQuery query(destination_db);

    //    int tab_index = ui->tabWidget->currentIndex();
    //    if(list_tablewidgets.size() <= tab_index)
    //        return;

    //    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    //    QStringList col_type_list = tw_data_type_list_tablewidget.at(tab_index);
    //    int bookmark_index = col_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_BOOKMARK,0);
    //    int record_index = col_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_RECORD,0);
    //    QString table_name = table_name_list_tablewidget.at(tab_index);

    //    QString tab_name = tab_name_list_tablewidget.at(tab_index);

    //    QStringList col_list = tw_column_name_list_tablewidget.at(tab_index);
    //    QString bookmark_col_name = col_list.at(bookmark_index);

    //    ///--Progressbar work---START.
    //    total_covered_count = 0;
    //    total_record_count = my_tablewidget->rowCount();
    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;

    //    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,5);
    //    ///--Progressbar work---END.


    //    int os_scheme_index = col_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_OS_SCHEME_NAME,0);

    //    for(int i = 0; i < my_tablewidget->rowCount(); i++)
    //    {
    //        ///--Progressbar work---START.
    //        //        if(i % 500 == 0)
    //        //            QCoreApplication::processEvents();

    //        if(i < 100)
    //        {
    //            if(i % 8 == 0)
    //            {
    //                QCoreApplication::processEvents();
    //            }
    //        }
    //        else
    //        {
    //            if(i % 350 == 0)
    //            {
    //                QCoreApplication::processEvents();
    //            }
    //        }

    //        total_covered_count++;
    //                   if(total_record_count > 0)
    //   {
    // if(total_covered_count % progressbar_increment_step == 0)
    //        {
    //            QCoreApplication::processEvents();

    //            float record_percent = ((total_covered_count*100)/total_record_count);
    //            emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
    //        }
    //}
    //        ///--Progressbar work---End.


    //        os_naming_scheme_of_current_record = my_tablewidget->item(i, os_scheme_index)->text();
    //        my_tablewidget->item(i, bookmark_index)->setCheckState(Qt::Unchecked);

    //        query.prepare(QString("update ") + table_name + " set " + bookmark_col_name + " =0 where INT=?");
    //        query.addBindValue(my_tablewidget->item(i, record_index)->text());

    //        if(!query.exec())
    //        {
    //            recon_static_functions::app_debug("query ----FAILED---- ",Q_FUNC_INFO);
    //            recon_static_functions::app_debug("" + query.lastError().text(),Q_FUNC_INFO);
    //            disable_action_widgets(false);
    //            return;
    //        }

    //        QStringList applied_tags_name_list = get_applied_tags_name_on_current_record(my_tablewidget->item(i, record_index)->text());
    //        emit signal_change_bookmark_status_in_other_db_also(my_tablewidget->item(i, record_index)->text(),get_plugin_name(),tab_name,"0",applied_tags_name_list, destination_db_file, os_naming_scheme_of_current_record,MACRO_CASE_TREE_PLUGINS);
    //    }

    //    disable_action_widgets(false);
    //    destination_db.close();
    //    return;
}

void plugin_viewer::slot_act_add_notes()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    if(my_tablewidget->currentRow() < 0)
        return;

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString table_name = table_name_list_tablewidget.at(tab_index);

    add_notes_dialog_module_popobj1->show();

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("select notes from " + table_name + " where INT=?");
    query.addBindValue(my_tablewidget->item(my_tablewidget->currentRow(), record_index)->text());

    if(!query.exec())
    {
        recon_static_functions::app_debug("query ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    if(query.next())
    {
        add_notes_dialog_module_popobj1->set_notes(query.value(0).toString());
    }

    destination_db.close();
    return;
}


void plugin_viewer::slot_add_notes(QString notes_value)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    add_notes_dialog_module_popobj1->hide();

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    if(notes_value.trimmed().isEmpty())
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        QIcon icon("../icons/notes.png");
        my_tablewidget->item(selected_row_no, bookmark_index)->setIcon(icon);


        INT_list << my_tablewidget->item(selected_row_no, record_index)->text();
    }

    global_connection_manager_obj->pub_set_notes_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name, notes_value );
    global_connection_manager_obj->pub_add_notes_for_plugin_viewer(INT_list, get_plugin_name(), tab_name, notes_value);
    global_connection_manager_obj->pub_set_notes_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name, notes_value);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}


void plugin_viewer::slot_act_add_notes_to_bookmarks()
{
    add_notes_dialog_module_popobj2->show();
}

void plugin_viewer::slot_act_export()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    // this approach has been done by command + e

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int c_tab = current_tab_index_only_for_tableview;
    if(table_export_available_list_tablewidget.at(c_tab) != QString("1"))
        return;

    if(list_tablewidgets.size() <= c_tab)
        return;

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    m_tablewidget *my_tablewidget = list_tablewidgets.at(c_tab);

    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();
    path_manually_export.clear();// =  QFileDialog::getExistingDirectory(this, "Output Directory", path_manually_export, QFileDialog::ShowDirsOnly);


    file_dialog_obj.setFileMode(QFileDialog::Directory);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            path_manually_export = selected_files.at(0);
        }
    }
    if(path_manually_export.trimmed().isEmpty())
    {
        emit signal_statusbar_progress_bar_show_hide(false, "");

        return;
    }

    QString dest_path = path_manually_export;
    bool bool_file_copied  = false;
    QStringList list_paths_to_message_box;

    total_covered_count = 0;
    total_record_count = selection_model_list.size();

    for(int i = 0; i < selection_model_list.size(); i++)
    {

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;

        if(i < 100)
        {
            if(i % 8 == 0)
            {
                QCoreApplication::processEvents();
            }
        }
        else
        {
            if(i % 350 == 0)
            {
                QCoreApplication::processEvents();
            }
        }

        total_covered_count++;
        QString table_name = table_name_list_tablewidget.at(c_tab);
        int table_index = table_name_list_tablewidget.indexOf(table_name);
        QStringList export_col_name_list = export_path_column_list_tablewidget.at(table_index);

        QString record_no =  my_tablewidget->item(selected_row_no,2)->text();
        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(destination_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);

        QStringList all_path_list = obj.complete_export_filepaths;
        QStringList display_f_path_list = obj.displayed_file_path;

        for(int i =0 ; i< all_path_list.size() ; i++)
        {
            if(cancel_exporter_loop)
            {
                recon_static_functions::app_debug(" cancel_exporter_loop 333333333 CANCELLEDDDDDDDDDDDD",Q_FUNC_INFO);
                emit signal_statusbar_progress_bar_show_hide(false, "");
                return;
            }

            QString complete_path = all_path_list.at(i);

            QString display_f_path = display_f_path_list.at(i);
            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path ,dest_path ,QFileInfo(display_f_path).fileName() , Q_FUNC_INFO);


            if(!exported_file_path.trimmed().isEmpty())
            {
                bool_file_copied = true;
                list_paths_to_message_box << exported_file_path;
            }

        }
    }

    if(cancel_exporter_loop)
    {
        emit signal_statusbar_progress_bar_show_hide(false, "");
        return;
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(bool_file_copied)
    {
        message_dialog_object->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_path);
        message_dialog_object->show();
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}


void plugin_viewer::slot_add_notes_to_bookmarks(QString data)
{
    add_notes_dialog_module_popobj2->hide();

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    data = data.trimmed();
    if(data.isEmpty())
        return;



    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);
    QString table_name = table_name_list_tablewidget.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QStringList cols_name_list = tw_column_name_list_tablewidget.at(tab_index);
    QString bookmark_col_name = cols_name_list.at(bookmark_index);



    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QIcon icon("../icons/notes.png");

    QSqlQuery query(destination_db);
    //    query.prepare("update "+ table_name + " set notes=? where (" + bookmark_col_name + " = 1) AND notes IS NULL");

    query.prepare("update "+ table_name + " set notes=? where " + bookmark_col_name + "= '1' AND (notes IS NULL or notes is '')");
    query.addBindValue(data);

    if(!query.exec())
    {
        recon_static_functions::app_debug("query ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString tab_name = tab_name_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);


    QString command= "SELECT Count(*)  from '" + table_name + "' where " + bookmark_col_name + "= '1'";

    total_covered_count = 0;

    total_record_count = recon_helper_standard_obj->get_string_from_db_by_dbreference(command,0,destination_db, Q_FUNC_INFO).toLongLong();
    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;

    //    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,05);

    for(int k = 0 ; k < my_tablewidget->rowCount() ; k++)
    {
        QString  os_nm_scheme = my_tablewidget->item(k, os_scheme_index)->text();
        if(my_tablewidget->item(k, bookmark_index)->checkState())
        {

            if(k % 350 == 0)
            {
                QCoreApplication::processEvents();
            }

            total_covered_count++;
            //            if(total_record_count > 0)
            //            {
            //                if(total_covered_count % progressbar_increment_step == 0)
            //                {
            //                    float record_percent = ((total_covered_count*100)/total_record_count);
            //                    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
            //                }
            //            }

            my_tablewidget->item(k, bookmark_index)->setIcon(icon);

            QStringList applied_tags_name_list = get_applied_tags_name_on_current_record(my_tablewidget->item(k, record_index)->text());

            emit signal_change_notes_value_in_other_db_also(my_tablewidget->item(k, record_index)->text(),get_plugin_name(),tab_name,data,applied_tags_name_list,os_nm_scheme, MACRO_CASE_TREE_Artifacts);

            QString result_note_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

            QString is_bookmarked = "0";
            if(my_tablewidget->item(k, bookmark_index)->checkState() != 0)
            {
                is_bookmarked = "1";
            }
            QString record_no = my_tablewidget->item(k, record_index)->text();
            emit signal_add_notes_added_in_db(is_bookmarked, MACRO_Plugin_Notes_Search_TAB_Name_Plugin, record_no, get_plugin_name(), tab_name, table_name, data.trimmed(), result_note_db_path, os_nm_scheme);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    destination_db.close();
}

void plugin_viewer::slot_time_line(qint64 start_time, qint64 end_time)
{
    timeline_selection_window_obj->hide();
    qint64 adjustment_seconds = case_created_machine_offset - examiner_time_offset;

    timeline_start_time =  global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(start_time);
    timeline_end_time =  global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(end_time);

    on_lineEdit_search_returnPressed();
}

void plugin_viewer::on_checkBox_timeline_toggled(bool checked)
{
    ui->pushButton_timeline->setEnabled(checked);
    on_lineEdit_search_returnPressed();
}

void plugin_viewer::slot_progress_window_module_closed()
{
    cancel_exporter_loop = true;
}

void plugin_viewer::slot_detach_mdls_dialog_window_closed()
{
    bool_detach_mdls_set = false;
}

void plugin_viewer::slot_control_m_key_pressed()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);
    QStringList cols_nam_list =  md_column_name_list_tablewidget.at(tab_index);
    QStringList preview_col_list = md_preview_list_tablewidget.at(tab_index);

    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
    }

    if(preview_index == -1)
        return;

    bool_detach_mdls_set = true;
    detach_mdls_dialog_module_popobj->show();

    tablewidget_general_clicked(tab_index,my_tablewidget->currentRow(),0);

}

void plugin_viewer::slot_control_n_key_pressed()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);
    QStringList cols_nam_list =  md_column_name_list_tablewidget.at(tab_index);
    QStringList preview_col_list = md_preview_list_tablewidget.at(tab_index);

    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
    }

    if(preview_index == -1)
        return;

    bool_full_mdls_set = true;

    tablewidget_general_clicked(tab_index,my_tablewidget->currentRow(),0);

    bool_full_mdls_set = false;
}


void plugin_viewer::on_tabWidget_currentChanged(int index)
{
    if(index < 0)
        return;


    current_media.clear();
    //current_tab_text = ui->tabWidget->tabText(index);
    current_tab_text = get_exact_tab_name_from_ui(index);


    current_tab_index_only_for_tableview = 0;
    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(get_plugin_name(),current_tab_text);
    QString tablewdgt_tab_nm =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(get_plugin_name(), table_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);
    for(int tt = 0; tt < tab_name_list_tablewidget.size(); tt++)
    {
        if(tab_name_list_tablewidget.at(tt) == tablewdgt_tab_nm)
        {
            current_tab_index_only_for_tableview = tt;
            break;
        }
    }

    emit signal_refresh_widgets();

    for(int i =0; i < list_tablewidgets.size(); i++)
    {
        m_tablewidget *my_tablewidget = list_tablewidgets.at(i);
        my_tablewidget->setCurrentCell(-1,0);
    }


    bool thmb_tab = global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(get_plugin_name(), current_tab_text);
    bool chat_tab = global_csv_reader_class_selected_obj->is_tab_type_chat_view(get_plugin_name(), current_tab_text);
    bool tree_tab = global_csv_reader_class_selected_obj->is_tab_type_tree_view(get_plugin_name(), current_tab_text);

    if(thmb_tab || chat_tab || tree_tab)
    {
        ui->checkBox_timeline->setDisabled(true);
        ui->pushButton_timeline->setDisabled(true);
    }
    else
    {
        ui->checkBox_timeline->setDisabled(false);
    }



    if(list_tablewidgets.size() <= index)
        return;


    m_tablewidget_obj_for_manage_column = NULL;
    m_tablewidget_obj_for_manage_column = list_tablewidgets.at(index);


    if(m_tablewidget_obj_for_manage_column == NULL){
        return;
    }

    QList<struct_manage_column_info> hid_col_lst_var;
    hid_col_lst_var.clear();

    hid_col_lst_var = get_hidden_column_from_manage_column_db();

    for(int i=0;i<hid_col_lst_var.size();i++){
        m_tablewidget_obj_for_manage_column->hideColumn(hid_col_lst_var.at(i).hid_col_no);
    }



}

void plugin_viewer::slot_add_tags_to_plugin(QString tag_name , QString colour_name)
{
    common_add_tags_to_plugin( tag_name, colour_name);
}

void plugin_viewer::slot_remove_tags_from_plugin()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        QString icon_path =  "";
        QIcon icon(icon_path);
        my_tablewidget->item(selected_row_no,1)->setIcon(icon);


        INT_list << my_tablewidget->item(selected_row_no, record_index)->text();
    }

    global_connection_manager_obj->pub_remove_tag_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_delete_tag_for_plugin_viewer(INT_list, get_plugin_name(), tab_name, "");
    global_connection_manager_obj->pub_remove_tags_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}


void plugin_viewer::slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        tag_sub_menu->addAction(act_already_saved_tags);

        emit signal_add_current_tag_into_loader_class_sub_menu(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void plugin_viewer::slot_tag_list_after_remove_multiple_tag(QStringList list_tags_remained, QString table_name, QString record)
{
    recon_static_functions::app_debug(" Starts ",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    QString tab_name = tab_name_list_tablewidget.at(tab_index);
    int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);

    QString tags_str = list_tags_remained.join(",");

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    int selected_row_no = my_tablewidget->currentRow();
    QString  os_nm_scheme = my_tablewidget->item(selected_row_no, os_scheme_index)->text();

    if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
        return;

    query.prepare("SELECT recon_tag_value from '" + table_name + "' where INT=?");
    query.addBindValue(record);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select ----FAILED---- " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query select ----FAILED---- " + query.executedQuery(),Q_FUNC_INFO);
        return;
    }

    QString previous_tag;
    if(query.next())
    {
        previous_tag = query.value(0).toString().trimmed();
    }

    QStringList listoftags = previous_tag.split(",", Qt::SkipEmptyParts);
    if(listoftags.contains(QString(MACRO_Tag_Name_Bookmarks)))
    {
        if(tags_str.trimmed().isEmpty())
            tags_str.append(QString(MACRO_Tag_Name_Bookmarks));
        else
            tags_str.append("," + QString(MACRO_Tag_Name_Bookmarks));

        list_tags_remained << QString(MACRO_Tag_Name_Bookmarks);

    }

    if(!previous_tag.contains(","))
        return;

    query.prepare("update '" + table_name + "' set recon_tag_value=? where INT=?");

    if(tags_str == "")
        query.addBindValue(QString(""));
    else
        query.addBindValue(tags_str);

    query.addBindValue(record);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query insert ----FAILED---- " + query.executedQuery(),Q_FUNC_INFO);
        return;
    }

    if(tags_str == "")
    {
        QIcon icon(QString(""));
        my_tablewidget->item(selected_row_no,1)->setIcon(icon);
    }
    else if(tags_str.contains(","))
    {
        QString icon_path = "../icons/tag_icons/multiple.png";
        QIcon icon(icon_path);
        my_tablewidget->item(selected_row_no,1)->setIcon(icon);
    }
    else if((tags_str != "") && (!tags_str.contains(",")))
    {
        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags_str, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(color, Q_FUNC_INFO);
        QIcon icon(icon_path);
        my_tablewidget->item(selected_row_no,1)->setIcon(icon);
    }

    emit signal_update_other_db_and_display_also(get_plugin_name(), tab_name, record, list_tags_remained,os_nm_scheme);


    destination_db.close();

    recon_static_functions::app_debug(" Ends ",Q_FUNC_INFO);
}

void plugin_viewer::slot_abouttoshow_tags_in_submenu()
{
    display_default_tags();
}

void plugin_viewer::slot_selected_tag(QAction* action)
{
    recon_static_functions::app_debug ("[slot_selected_tag] Starts",Q_FUNC_INFO);


    QString tag_name = action->text();

    if(tag_name == MACRO_Generic_Right_Click_Remove_Tag || tag_name == MACRO_Generic_Right_Click_Create_Tag)
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;
    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_obj->tags_db_path,Q_FUNC_INFO);

    emit signal_add_tags(action->text(),colour);

    recon_static_functions::app_debug ("[slot_selected_tag] Ends",Q_FUNC_INFO);

}

void plugin_viewer::slot_show_tag()
{
    recon_static_functions::app_debug ("[slot_show_tag] Starts",Q_FUNC_INFO);


    add_tags_obj->show();


    recon_static_functions::app_debug ("[slot_show_tag] Ends",Q_FUNC_INFO);

    return;
}

void plugin_viewer::slot_act_remove_notes()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        QString icon_path =  "";
        QIcon icon(icon_path);
        my_tablewidget->item(selected_row_no,bookmark_index)->setIcon(icon);


        INT_list << my_tablewidget->item(selected_row_no, record_index)->text();
    }

    global_connection_manager_obj->pub_remove_notes_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_delete_notes_for_plugin_viewer(INT_list, get_plugin_name(), tab_name, "");
    global_connection_manager_obj->pub_remove_notes_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_copy_to_clipboard()
{
    recon_static_functions::app_debug("--Starts---", Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    recon_static_functions::copy_tablewidget_data_to_clipboard(my_tablewidget, Q_FUNC_INFO);

    recon_static_functions::app_debug("--Ends---", Q_FUNC_INFO);
}

void plugin_viewer::slot_act_go_to_mail_triggered()
{
    //    recon_static_functions::app_debug("--Starts---", Q_FUNC_INFO);

    //    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
    //        return;

    //    int tab_index = current_tab_index_only_for_tableview;

    //    if(list_tablewidgets.size() <= tab_index)
    //        return;

    //    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    //    qint64 row = my_tablewidget->currentRow();

    //    if(row < 0 || tab_index < 0)
    //        return;

    //    QString plugin_name = get_plugin_name();

    //    QStringList tw_col_type_list = tw_data_type_list_tablewidget.at(tab_index);

    //    int record_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    //    QString record_no = my_tablewidget->item(row, record_index)->text();

    //    int os_scheme_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    //    QString os_name_str = my_tablewidget->item(row, os_scheme_index)->text();

    //    if(plugin_name == QString(MACRO_Plugin_Name_Gmail))
    //   {
    //        emit signal_tablewidget_go_to_mail(QString(MACRO_CASE_TREE_Email_Files_Category_Google_Mail),QString(MACRO_CASE_TREE_Email_Files_Parent), QString(MACRO_Plugin_Google_Mail_TAB_Name_Google_Mail_Data),record_no , QString(""), os_name_str);
    //    }
    //    else if(plugin_name == QString(MACRO_Plugin_Name_Mail))
    //    {
    //        QString table_name = table_name_list_tablewidget.at(tab_index);
    //        //QString current_tab_text = ui->tabWidget->tabText(tab_index).trimmed();
    //        QString current_tab_text = get_exact_tab_name_from_ui(tab_index);
    //        if(current_tab_text == QString(MACRO_Plugin_Mail_TAB_Name_Messages))
    //        {
    //            QString command = QString("Select emlx_file_path from ") + table_name + QString(" Where INT=?");
    //            QString emlx_path_from_plgn = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(record_no), 0, destination_db_file, Q_FUNC_INFO);

    //            ///get Record No. from EMLX Files
    //            QString emlx_mail_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString() + "email_data.sqlite";
    //            command = QString("Select INT from emlx_data Where source_file = ?");
    //            QString emlx_record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(emlx_path_from_plgn), 0, emlx_mail_db_path, Q_FUNC_INFO);
    //            record_no = emlx_record_no;

    //            emit signal_tablewidget_go_to_mail(QString(MACRO_CASE_TREE_Email_Files_Category_Email_Parser),QString(MACRO_CASE_TREE_Email_Files_Parent), QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data),record_no , QString(""), os_name_str);
    //        }
    //    }

    //    recon_static_functions::app_debug("--Ends---", Q_FUNC_INFO);
}

void plugin_viewer::slot_act_remove_all_notes()
{
    //    int tab_index = ui->tabWidget->currentIndex();
    //    if(list_tablewidgets.size() <= tab_index)
    //        return;

    //    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    //    if(my_tablewidget->currentRow() < 0)
    //        return;

    //    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");


    //    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    //    int record_index = cols_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_RECORD,0);
    //    QString table_name = table_name_list_tablewidget.at(tab_index);

    //    QString tab_name = tab_name_list_tablewidget.at(tab_index);

    //    int os_scheme_index = cols_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_OS_SCHEME_NAME,0);

    //    total_covered_count = 0;
    //    total_record_count = my_tablewidget->rowCount();
    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;

    //    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,05);

    //    for(int k = 0 ; k < my_tablewidget->rowCount(); k++)
    //    {
    //        if(k % 350 == 0)
    //        {
    //            QCoreApplication::processEvents();
    //        }

    //        total_covered_count++;
    //                   if(total_record_count > 0)
    //  {
    // if(total_covered_count % progressbar_increment_step == 0)
    //        {
    //            float record_percent = ((total_covered_count*100)/total_record_count);
    //            emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
    //        }
    //}
    //        QIcon icon(QString(""));

    //        my_tablewidget->item(k, 0)->setIcon(icon);

    //        os_naming_scheme_of_current_record = my_tablewidget->item(k, os_scheme_index)->text();

    //        QStringList applied_tags_name_list = get_applied_tags_name_on_current_record(my_tablewidget->item(k, record_index)->text());
    //        emit signal_change_notes_value_in_other_db_also(my_tablewidget->item(k, record_index)->text(),get_plugin_name(),tab_name,"",applied_tags_name_list, os_naming_scheme_of_current_record, MACRO_CASE_TREE_PLUGINS);

    //        QString record_no = my_tablewidget->item(k, record_index)->text();

    //        QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    //        recon_static_functions::delete_records_from_note_search_db_in_plugin(get_plugin_name(), record_no, tab_name, table_name,destination_db_file, os_naming_scheme_of_current_record, result_dir, Q_FUNC_INFO );
    //    }

    //    QSqlDatabase destination_db;
    //    QString connection_naam = QString(Q_FUNC_INFO);
    //    QSqlDatabase::removeDatabase(connection_naam);
    //    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //    destination_db.setDatabaseName(destination_db_file);

    //    if(!destination_db.open())
    //    {
    //        recon_static_functions::app_debug(" Destination Db opening -----FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
    //        emit signal_statusbar_progress_bar_show_hide(false, "");
    //        return;
    //    }

    //    QSqlQuery query(destination_db);

    //    query.prepare("update '" + table_name + "' set notes=NULL where notes is not null or notes is not ''");

    //    if(!query.exec())
    //    {
    //        recon_static_functions::app_debug("query -----FAILED------ ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug("" + query.lastError().text(),Q_FUNC_INFO);
    //        emit signal_statusbar_progress_bar_show_hide(false, "");
    //        destination_db.close();
    //        return;
    //    }

    //    while(query.next())
    //    {
    //        add_notes_dialog_module_popobj1->set_notes(QString(""));
    //    }

    //    destination_db.close();

    //    emit signal_statusbar_progress_bar_show_hide(false, "");
    //    return;
}

void plugin_viewer::slot_act_remove_notes_from_bookmarks()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    if(my_tablewidget->currentRow() < 0)
        return;

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString table_name = table_name_list_tablewidget.at(tab_index);


    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QString command= "SELECT Count(*)  from '" + table_name + "' where bookmark=1";

    total_covered_count = 0;

    total_record_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_file, Q_FUNC_INFO).toLongLong();
    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;

    //    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,05);

    for(int k = 0 ; k < my_tablewidget->rowCount() ; k++)
    {
        QIcon icon(QString(""));

        QString os_nm_scheme = my_tablewidget->item(k, os_scheme_index)->text();

        if(my_tablewidget->item(k, bookmark_index)->checkState())
        {

            if(k % 350 == 0)
            {
                QCoreApplication::processEvents();
            }

            total_covered_count++;
            //            if(total_record_count > 0)
            //            {
            //                if(total_covered_count % progressbar_increment_step == 0)
            //                {
            //                    float record_percent = ((total_covered_count*100)/total_record_count);
            //                    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
            //                }
            //            }

            my_tablewidget->item(k, bookmark_index)->setIcon(icon);

            QString record_no = my_tablewidget->item(k, record_index)->text();

            QStringList applied_tags_name_list = get_applied_tags_name_on_current_record(my_tablewidget->item(k, record_index)->text());
            emit signal_change_notes_value_in_other_db_also(my_tablewidget->item(k, record_index)->text(),get_plugin_name(),tab_name,"",applied_tags_name_list, os_nm_scheme, MACRO_CASE_TREE_Artifacts);

            //recon_static_functions::delete_records_from_note_search_db_in_plugin(get_plugin_name(), record_no, tab_name, table_name,destination_db_file, os_naming_scheme_of_current_record, result_dir, Q_FUNC_INFO );
            emit signal_update_display_on_remove_notes(get_plugin_name(), tab_name, record_no, "", os_nm_scheme);
        }
    }


    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        emit signal_statusbar_progress_bar_show_hide(false, "");
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("update '" + table_name + "' set notes=? where bookmark=1");

    query.addBindValue(QString(""));
    if(!query.exec())
    {
        recon_static_functions::app_debug("query ----FAILED---- " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("" + query.lastError().text(),Q_FUNC_INFO);
        emit signal_statusbar_progress_bar_show_hide(false, "");
        destination_db.close();
        return;
    }

    while(query.next())
    {
        add_notes_dialog_module_popobj1->set_notes(QString(""));
    }


    destination_db.close();

    emit signal_statusbar_progress_bar_show_hide(false, "");

    return;
}


void plugin_viewer::slot_removed_tags_list(QStringList list_tags_removed, QString table_name, QString record)
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;

    if(tab_name_list_tablewidget.size() <= tab_index)
        return;

    QString tab_name = tab_name_list_tablewidget.at(tab_index);
    QString plugin_name = get_plugin_name();

    QString os_scheme = recon_helper_standard_obj->get_target_os_scheme_display_on_current_record_source(record,table_name,destination_db_file,Q_FUNC_INFO);
    emit signal_remove_current_record_entry_from_tag_search_display(plugin_name,tab_name,record,list_tags_removed,os_scheme);
}

void plugin_viewer::slot_update_tag_submenu(bool status, QString new_tag, QString tag_colour)
{
    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        tag_sub_menu->addAction(act_already_saved_tags);
    }
}


void plugin_viewer::slot_update_plugin_hashset_submenu(QString db_name, QString db_path)
{
    QAction *act_add_file = new QAction(tr(db_name.toLocal8Bit().data()), this->menu_hashset_add_files);
    act_add_file->setToolTip(db_path);

    menu_hashset_add_files->addAction(act_add_file);

}
void plugin_viewer::slot_submenu_go_to_artifact_triggered(QAction* clicked_action)
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString source_path = clicked_action->toolTip();

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QString table_name = table_name_list_tablewidget.at(tab_index);


    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString record_no = my_tablewidget->item(my_tablewidget->currentRow() , record_index)->text();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    QString cmd = QString("Select source_count_name From '") + table_name + QString("' Where INT = ?");

    query.prepare(cmd);
    query.addBindValue(record_no);


    if(!query.exec())
    {
        recon_static_functions::app_debug(" query " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query insert ----FAILED---- " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();

        return;
    }

    QString source_cnt_nm_str;
    if(query.next())
    {
        source_cnt_nm_str = query.value(0).toString();
    }

    destination_db.close();


    if(!source_cnt_nm_str.isEmpty())
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_cnt_nm_str +"/file_system.sqlite";
        emit signal_go_to_artifact_source_in_file_system(source_path, fs_db_path, source_cnt_nm_str);
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);

}

QString plugin_viewer::get_plugin_name()
{
    return module_plugin_name;
}

void plugin_viewer::slot_table_header_customContextMenuRequested(const QPoint &points)
{
    return;

    int column_count;
    column_count = m_tablewidget_obj_for_manage_column->columnCount();

    menu_manage_column->clear();

    QList<struct_manage_column_info> column_no_and_name_list;
    column_no_and_name_list = get_hidden_column_from_manage_column_db();

    for(int i=0;i < column_count;i++)
    {
        if((i == 0) || (i == 1) || (i == 2)  || (i == (column_count - 1)))
            continue;

        if(m_tablewidget_obj_for_manage_column->horizontalHeaderItem(i)->text() == QString(Macro_Generic_OS_Scheme))
            continue;

        QAction *act = new QAction(m_tablewidget_obj_for_manage_column->horizontalHeaderItem(i)->text(),this->menu_manage_column);
        act->setCheckable(true);
        act->setChecked(true);

        for(int j = 0; j < column_no_and_name_list.size(); j++)
        {
            struct_manage_column_info obj = column_no_and_name_list.at(j);
            if(act->text() == obj.hid_col_name)
                act->setCheckable(false);

        }
        menu_manage_column->addAction(act);
    }

    menu_manage_column->exec(QCursor::pos());

    recon_static_functions::app_debug ("[slot_act_manage_columns]-END",Q_FUNC_INFO);
}

void plugin_viewer::slot_act_quick_look_triggered()
{
    emit signal_act_quick_look_triggered();

}

void plugin_viewer::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();

}

void plugin_viewer::slot_hide_progressing_bar_on_cancel_loading_report(bool cancel_status)
{
    bool_cancel_loading_reports = true;

    thread_report_builder_obj->pub_cancel_report();
    display_loading_progress_bar_for_reports_from_plugin_obj->hide();

}


void plugin_viewer::slot_action_manage_column_clicked(QAction* current_clicked_action)
{
    int index = menu_manage_column->actions().indexOf(current_clicked_action);
    current_clicked_action->setChecked(false);

    int checkstate;
    if(current_clicked_action->isCheckable())
    {
        checkstate = Qt::Unchecked;
    }
    else
    {
        checkstate = Qt::Checked;
    }
    slot_manage_column_clicked(index,checkstate);
}

void plugin_viewer::pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);


    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(get_plugin_name(),current_tab_text))
        emit signal_set_bookmark_status_to_gallery_view(true);

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        my_tablewidget->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

        QString tmp_record_no = my_tablewidget->item(selected_row_no, record_index)->text();

        if(recon_helper_standard_obj->pub_check_bookmarked_record_in_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display,tab_name,record_no_str))
            return;

        if(record_no_str != tmp_record_no)
            continue;

        INT_list << tmp_record_no;

        my_tablewidget->selectRow(selected_row_no);

    }

    global_connection_manager_obj->pub_set_bookmark_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_set_bookmarks_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void plugin_viewer::pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(get_plugin_name(),current_tab_text))
        emit signal_set_bookmark_status_to_gallery_view(false);

    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString tab_name = tab_name_list_tablewidget.at(tab_index);


    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;


        if(my_tablewidget->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        my_tablewidget->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

        QString tmp_record_no = my_tablewidget->item(selected_row_no, record_index)->text();

        if(recon_helper_standard_obj->pub_check_bookmarked_record_in_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display,tab_name,record_no_str))
            return;

        if(record_no_str != tmp_record_no)
            continue;

        INT_list << tmp_record_no;

        my_tablewidget->selectRow(selected_row_no);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_delete_bookmark_for_plugin_viewer(INT_list, get_plugin_name(), tab_name, "");
    global_connection_manager_obj->pub_remove_bookmarks_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name);

    disable_action_widgets(false);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void plugin_viewer::slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status)
{
    if(bookmark_status == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        pub_remove_main_record_bookmark_from_detailed_info_triggered(record_str);
    }
    else if(bookmark_status == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
    {
        pub_bookmark_main_record_from_detailed_info_triggered(record_str);
    }
}

void plugin_viewer::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);
    bool_sort_with_search = false;

    QList<struct_plugin_search_column_text> search_text_col_QList;

    int tab_index = ui->tabWidget->currentIndex();

    if(tab_custom_header_widgets_QMap.size() <= tab_index)
        return;

    QList<custom_table_header_widget *> custom_header_widgets_QList;

    custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(tab_index);

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_plugin_search_column_text struct_plugin_search_column_text_obj;
            struct_plugin_search_column_text_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_plugin_search_column_text_obj.column_number = i;
            search_text_col_QList.append(struct_plugin_search_column_text_obj);
        }
    }

    for(int ii = 0 ; ii < list_tablewidgets.size() ; ii++)
    {
        if(ui->tabWidget->currentIndex() == ii)
        {
            current_table_widget = list_tablewidgets.at(ii);
            break;
        }
    }


    if(search_text_col_QList.size() < 1)
    {
        on_pushButton_showall_clicked();
        bool_cancel_loading = false;
        display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_obj->show();

        for(int row = 0; row < current_table_widget->rowCount(); row++)
        {
            current_table_widget->showRow(row);
        }

        display_loading_progress_bar_obj->hide();
        return;
    }
    on_pushButton_showall_clicked();
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();


    all_column_text_search(search_text_col_QList);

    current_table_widget->horizontalHeader()->hide();
    current_table_widget->horizontalHeader()->show();

    emit signal_clear_all_metadata();
    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void plugin_viewer::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void plugin_viewer::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    for(int ii = 0 ; ii < list_tablewidgets.size() ; ii++)
    {
        if(ui->tabWidget->currentIndex() == ii)
        {
            current_table_widget = list_tablewidgets.at(ii);
            break;
        }
    }

    if(column != enum_Plugin_display_Bookmarks && column != enum_Plugin_display_Notes_Icon)
    {
        if(column_sort_order == 0)
            current_table_widget->sortItems(column, Qt::DescendingOrder);
        else
            current_table_widget->sortItems(column, Qt::AscendingOrder);

        current_table_widget->horizontalHeader()->hide();
        current_table_widget->horizontalHeader()->show();
        return;
    }

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int current_tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= current_tab_index)
        return;

    total_covered_count = 0;

    QString keyword = ui->lineEdit_search->text().trimmed();
    QStringList search_string_list = recon_static_functions::get_search_string_list(keyword, Q_FUNC_INFO);

    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    if((column == 0) || (column == 1))
    {
        disable_action_widgets(true);

        current_table_widget->setRowCount(0);

        QString table_name = table_name_list_tablewidget.at(current_tab_index);
        QStringList cols_nam_list =  tw_column_name_list_tablewidget.at(current_tab_index);
        QStringList cols_type_list = tw_data_type_list_tablewidget.at(current_tab_index);
        QStringList search_cols_list = ms_search_local_list_tablewidget.at(current_tab_index);

        search_cols_list.clear();
        search_string_list.clear();
        QList<custom_table_header_widget *> custom_header_widgets_QList;

        custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(current_tab_index);
        bool_sort_with_search = false;
        for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
        {
            custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

            if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
            {
                search_string_list << custom_table_header_widget_obj->pub_get_search_text().trimmed();

                QString coln_name =  tw_column_name_list_tablewidget.at(current_tab_index).at(tw_header_list_tablewidget.at(current_tab_index).indexOf(custom_table_header_widget_obj->pub_get_column_name()));
                search_cols_list << coln_name;
                bool_sort_with_search = true;
            }
        }


        QStringList cols_width_list;
        QStringList tmp_cols_width_list = tw_col_width_list_tablewidget.at(current_tab_index);

        for(int jj = 0; jj < tmp_cols_width_list.size(); jj++)
        {
            if(cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString
                    || cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString
                    || cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {
                cols_width_list << tmp_cols_width_list.at(jj);
            }
            else
            {
                int cols_width = tmp_cols_width_list.at(jj).toInt();
                cols_width = cols_width + 25;
                cols_width_list << QString::number(cols_width);
            }
        }

        cols_nam_list.insert(cols_nam_list.size(), "notes");
        cols_type_list.insert(cols_type_list.size(), MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString);

        QString command = cols_nam_list.join(",");
        command = "select " + command + " from " + table_name;

        QString partial_command;
        if(search_cols_list.size() > 0)
        {
            partial_command = " where ";

            for(int k = 0 ; k < search_cols_list.size() ; k++)
            {
                if(search_cols_list.at(k) == "os_scheme_display")
                    continue ;

                partial_command  = partial_command + search_cols_list.at(k) + " LIKE '%"+search_string_list.at(k)+"%' AND ";

            }

            partial_command.chop(QString(" AND ").size()); // AND
        }
        command = command + partial_command;


        if(bool_sort_bookmarked[current_tab_index])
        {

            if(column == 0)
                command = command + " ORDER BY bookmark DESC";
            else
                command = command + " ORDER BY recon_tag_value DESC";

            int timeline_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
            int notes_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString,0);

            int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
            draw_table(command , current_tab_index, cols_nam_list.size(),cols_type_list, timeline_col_index, notes_col_index,os_scheme_index, cols_width_list,search_cols_list,search_string_list);

            bool_sort_bookmarked[current_tab_index] = false;
        }
        else
        {
            if(column == 0)
                command = command + " ORDER BY bookmark ASC";
            else
                command = command + " ORDER BY recon_tag_value ASC";

            int timeline_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
            int notes_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString,0);

            int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
            draw_table(command , current_tab_index, cols_nam_list.size(),cols_type_list, timeline_col_index, notes_col_index,os_scheme_index, cols_width_list,search_cols_list,search_string_list);

            bool_sort_bookmarked[current_tab_index] = true;
        }
        disable_action_widgets(false);
    }

    display_loading_progress_bar_obj->hide();


    current_table_widget->horizontalHeader()->hide();
    current_table_widget->horizontalHeader()->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void plugin_viewer::all_column_text_search(QList<struct_plugin_search_column_text> search_text_all_column_list)
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
            struct_plugin_search_column_text struct_plugin_search_column_text_obj = search_text_all_column_list.at(column_count);

            if(!(current_table_widget->item(row, struct_plugin_search_column_text_obj.column_number)->text().contains(struct_plugin_search_column_text_obj.search_text, Qt::CaseInsensitive)))
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
