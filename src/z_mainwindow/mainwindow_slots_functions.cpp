#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::slot_hide_loading_display_dialog_box(bool status)
{
    bool_cancel_loading = true;
}

void MainWindow::slot_super_timeline_done(bool status)
{
    super_timeline_obj->hide();
}

void MainWindow::slot_hashset_finished_or_cancelled_for_rcase(QStringList default_hashset_list, QString source_count)
{
    // common_hashset_finished_or_cancelled_for_rcase(default_hashset_list, source_count);

    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    slot_add_all_available_hashset_in_case_tree();

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

}

void MainWindow::slot_polling_timer_timeout()
{
    //    usb_device_serial_list usb_l;
    usb_device_serial_list_obj.update_usb_list();

    QStringList a_list;
    a_list.clear();
    a_list = usb_device_serial_list_obj.usb_serial_number_list();

    if(a_list.contains(poller))
    {
        uncloseable_wdgt->hide();
        close_counter = 1;
    }
    else
    {
        if(close_counter % 100 == 0)
            exit(0);
        else
            close_counter++;

        uncloseable_wdgt->show();
    }

}

void MainWindow::slot_hashset_finished_or_cancelled(QStringList default_hashset_list)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    slot_add_all_available_hashset_in_case_tree();

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

    return;

    //    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    //    QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    //    QString conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    //    int records_count = 0;

    //    QList<struct_GLOBAL_derived_source_info> list_target_source_info = global_target_sources_info_class_obj->pub_get_general_target_info_structure_Qlist();
    //    for(int count = 0; count < list_target_source_info.size(); count++)
    //    {
    //        struct_GLOBAL_derived_source_info target_struct_obj = list_target_source_info.at(count);
    //        QString source_count_name = target_struct_obj.source_count_name;

    //        QString hashset_run_status_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select Hashes from source_index Where table_name = ?") , QStringList(source_count_name), 0, sources_db_path, Q_FUNC_INFO);

    //        if(hashset_run_status_str.trimmed().isEmpty())
    //            hashset_run_status_str = QString::number(0);

    //        if(hashset_run_status_str == QString::number(0))
    //            continue;

    //        QString file_hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/hashes.sqlite");

    //        QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    //        QSqlDatabase::removeDatabase(connection_naam);
    //        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //        destination_db.setDatabaseName(file_hashes_db_path);
    //        if(!destination_db.open())
    //        {
    //            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
    //            recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            continue;
    //        }

    //        for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    //        {
    //            if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_HASHSET))
    //            {
    //                for(int pp = 0; pp < default_hashset_list.size() ; pp++)
    //                {
    //                    records_count = 0;
    //                    QStringList list = default_hashset_list.at(pp).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
    //                    if(list.size() != 2)
    //                        continue;

    //                    QString db_name = list.at(0).trimmed();
    //                    QString db_path = list.at(1).trimmed();

    //                    QStringList hashes_in_hashset_list;

    //                    QString hash_column_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select column_name From tbl_hashset Where database_path = ?"), QStringList(db_path), 0,conf_db_path ,Q_FUNC_INFO);
    //                    hash_column_str = hash_column_str.trimmed();
    //                    if(hash_column_str.isEmpty())
    //                        continue;

    //                    QString hash_table_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select table_name From tbl_hashset Where database_path = ?"), QStringList(db_path), 0,conf_db_path ,Q_FUNC_INFO);
    //                    hash_table_str = hash_table_str.trimmed();
    //                    if(hash_table_str.isEmpty())
    //                        continue;

    //                    hashes_in_hashset_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select " + hash_column_str + " From " + hash_table_str), 0, db_path, Q_FUNC_INFO);

    //                    bool bool_child_exists = false;
    //                    QTreeWidgetItem *sub_child = NULL;
    //                    for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->childCount(); j++)
    //                    {
    //                        QString tab_text = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j)->text(enum_global_case_tree_display_column_ZERO);
    //                        if(tab_text.contains("("))
    //                            tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());
    //                        tab_text = tab_text.trimmed();

    //                        QString item_db_path = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j)->text(enum_global_case_tree_display_column_THIRD);
    //                        if(tab_text == db_name && item_db_path == db_path)
    //                        {
    //                            sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j);
    //                            bool_child_exists = true;

    //                            break;
    //                        }
    //                    }

    //                    for(int num = 0; num < hashes_in_hashset_list.size(); num++)
    //                    {
    //                        QStringList records_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select filesystem_record From files Where md5_hash = ?"), QStringList(hashes_in_hashset_list.at(num)), 0, destination_db, Q_FUNC_INFO);
    //                        records_count += records_list.size();
    //                    }


    //                    if(bool_child_exists && sub_child != NULL)
    //                    {
    //                        QString title_str = db_name + " (" + QString::number(records_count) + ")";


    //                        sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
    //                        sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(records_count));
    //                    }
    //                    else
    //                    {

    //                        sub_child = new QTreeWidgetItem;
    //                        QString title_str = db_name + " (" + QString::number(records_count) + ")";

    //                        sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
    //                        sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/hashset.png"));
    //                        sub_child->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_HASHSET));
    //                        sub_child->setText(enum_global_case_tree_display_column_THIRD, db_path);
    //                        sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(records_count));

    //                        treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->addChild(sub_child);
    //                    }

    //                }
    //            }
    //        }

    //        destination_db.close();
    //    }

    //    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}

void MainWindow::slot_right_click_keyword_search_finished_or_cancelled(QString search_tag)
{
    common_right_click_keyword_search_finished_or_cancelled(search_tag);
}

void MainWindow::slot_right_click_artifacts_timeline_full(QString timeline_name)
{
    common_right_click_artifacts_timeline_full(timeline_name);
}

void MainWindow::slot_file_search_finished_or_cancelled(QString search_label)
{
    common_file_search_finished_or_cancelled(search_label);
}

void MainWindow::slot_content_search_extraction_finished_or_cancelled(QString search_label)
{
    common_content_search_finished_or_cancelled(search_label);
}

void MainWindow::slot_metadata_clicked(QString metadata, QString searched_keyword)
{
    textedit_detailed_information->clear();
    textedit_detailed_information->appendHtml(metadata);
    highlight_search_text(searched_keyword);
}


void MainWindow::slot_refresh_widgets()
{
    //    av_preview_object->stop_media();
    //    stackwidget_previewer->setCurrentIndex(0);
    //    textedit_detailed_information->clear();
    //    hex_viewer_metadata_obj->setdata("");
    //    text_viewer_aascii_unicode_obj->pub_clear_textview();
    //    tree_detailed_info_apple_metadata->clear();
    //    strings_viewer_obj->pub_clear_data();

    slot_clear_all_metadata();
}

void MainWindow::slot_redefined_result_finished_or_cancelled(QString category_name)
{
    common_add_redefined_result_category_name_in_case_tree(category_name);
}

void MainWindow::slot_right_click_add_to_text_indexing(QString item_text, QString source_count_name)
{

    if(recon_case_obj->pub_is_file_system_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not process 'Text Indexing' until source(s) have been added to case and apple timestamps have been extracted in case of Apple Filesystems.");
        return;
    }

    if(recon_case_obj->pub_is_text_indexing_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Text Indexing' is already running.");
        return;
    }

    source_count_name = source_count_name.trimmed();
    item_text = item_text.trimmed();

    if(source_count_name.isEmpty() || item_text.isEmpty())
        return;

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString file_path;
    if(item_text.startsWith("/"))
        file_path = item_text;
    else
        file_path = QString("/") + item_text;


    if(item_text == struct_source_info.source_name)
        file_path = QString("/");

    QString indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";

    ///========delete previous entry for same path==========///
    QString command_delete = QString("Delete from files Where file_path = ? AND source_count_name = ?");
    QStringList value_list;
    value_list << file_path << source_count_name;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_delete, value_list, indexing_db_path, Q_FUNC_INFO);

    ///insert new entry
    QString command = QString("insert into files (bookmark, fs_record,file_path,source_count_name,status,plugin_name) values(?,?,?,?,?,?)");
    value_list.clear();
    value_list << QString::number(0) << QString("") << file_path << source_count_name << QString::number(0) << QString(MACRO_Plugin_Name_File_System);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);

}

void MainWindow::slot_right_click_disk_hex_view(QString item_text, QString source_count_name, bool bool_is_complete_source)
{
    if(item_text == QString(MACRO_Generic_Right_Click_Disk_Hex_Viewer_Complete))
    {
        if(!source_count_name.isEmpty())
        {
            action_openwith_for_hex_viewer_complete(source_count_name,bool_is_complete_source);
        }
        else
        {
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"No Item selected for HEX view");
            message_dialog_object->show();
        }
    }
    else if(item_text == QString(MACRO_Generic_Right_Click_Disk_Hex_Viewer_Unallocated))
    {
        prepare_thread_for_disk_hex_view_unallocated(source_count_name, bool_is_complete_source);
    }
}

void MainWindow::slot_right_click_view_forensic_image_in_hex_viewer(QString source_count_name, bool bool_is_complete_source)
{
    for(int jj = 0 ; jj < combobox_category_navigation->count(); jj++)
    {
        if(combobox_category_navigation->itemText(jj) == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            QStringList feature_list = center_stackwidget_manager_obj->pub_get_feature_name_list(combobox_category_navigation->itemText(jj));

            for(int jj = 0 ; jj < feature_list.size(); jj++)
            {
                if(feature_list.at(jj).contains(MACRO_Plugin_Forensic_Image_In_Hex_Viewer_TAB_Name))
                {
                    QString hex_viewer_display_name_navigation = feature_list.at(jj);
                    QString unique_name = QString(MACRO_Plugin_Name_Hex_Viewer) + QString(MACRO_RECON_Splitter_1_multiple) + hex_viewer_display_name_navigation;
                    right_click_remove_widget_by_unique_name(unique_name, hex_viewer_display_name_navigation);
                }
            }
        }
    }

    if(bool_is_complete_source)
    {
        right_click_action_view_complete_forensic_image_with_hex_viewer(source_count_name,bool_is_complete_source);
    }
}

void MainWindow::right_click_action_view_complete_forensic_image_with_hex_viewer(QString source_count_name,bool bool_is_complete_source)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString recon_file_from_examiner;
    QString recon_fileinfo_path;

    struct_GLOBAL_witness_info_source derived_source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString file_type = recon_static_functions::get_forensic_image_type(derived_source_info_obj.source_path_1);
    if(file_type == MACRO_Target_Disk_ImageType_Apple
            || file_type == MACRO_Target_Disk_ImageType_RAW_FULL
            || file_type == MACRO_Target_Disk_ImageType_ENCASE_LOGICAL)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.source_path_1;
    }
    else if(file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "ewf1";
    }
    else if(file_type == MACRO_Target_Disk_ImageType_VMDK)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "vmdk1";
    }
    else if(file_type == MACRO_Target_Disk_ImageType_VHD)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "vhdi1";
    }
    else
    {
        // for raw images(default)
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.source_path_1;
    }

    QString filename = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(derived_source_info_obj.source_path_1);
    QString pluginName = QString(MACRO_Plugin_Forensic_Image_In_Hex_Viewer_TAB_Name);
    QString hex_viewer_display_name  = filename + " [" + pluginName + "]";

    QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    QStringList value_list;
    value_list << hex_viewer_display_name << source_count_name;

    recon_fileinfo_path = derived_source_info_obj.source_path_1;

    if(recon_fileinfo_path.startsWith(result_dir_path))
    {
        recon_fileinfo_path.remove(0,result_dir_path.size());
    }
    if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
        recon_fileinfo_path.prepend("/");

    recon_file_from_examiner = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path);

    QString tmp_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select file_path from hex_viewer_index Where viewer_display_name = ? AND source_count_name = ?"), value_list, 0, hex_feature_db_path, Q_FUNC_INFO);
    tmp_file_path = tmp_file_path.trimmed();
    if(!tmp_file_path.isEmpty())
    {
        st_hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        st_hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
    }
    else
    {
        st_hex_viewer_open_file_data_obj.bool_is_file_in_bucket = false;
        st_hex_viewer_open_file_data_obj.hex_feature_db_path.clear();
    }

    st_hex_viewer_open_file_data_obj.display_file_path = derived_source_info_obj.source_path_1;
    st_hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_viewer_display_name;
    st_hex_viewer_open_file_data_obj.plugin_name_str = pluginName;
    st_hex_viewer_open_file_data_obj.plugin_tab_name_str = st_hex_viewer_open_file_data_obj.plugin_name_str;
    st_hex_viewer_open_file_data_obj.source_count_name   = derived_source_info_obj.source_count_name;
    st_hex_viewer_open_file_data_obj.recon_file_infopath = recon_fileinfo_path;
    st_hex_viewer_open_file_data_obj.recon_filefrom      = recon_file_from_examiner;

    slot_file_openwith_hex_viewer(st_hex_viewer_open_file_data_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void MainWindow::action_openwith_for_hex_viewer_complete(QString source_count_name,bool bool_is_complete_source)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString recon_file_from_examiner;
    QString recon_fileinfo_path;

    struct_GLOBAL_witness_info_source derived_source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString file_type = recon_static_functions::get_forensic_image_type(derived_source_info_obj.source_path_1);
    if(file_type == MACRO_Target_Disk_ImageType_Apple
            || file_type == MACRO_Target_Disk_ImageType_RAW_FULL
            || file_type == MACRO_Target_Disk_ImageType_ENCASE_LOGICAL)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.source_path_1;
    }
    else if(file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "ewf1";
    }
    else if(file_type == MACRO_Target_Disk_ImageType_VMDK)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "vmdk1";
    }
    else if(file_type == MACRO_Target_Disk_ImageType_VHD)
    {
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "vhdi1";
    }
    else
    {
        // for raw images(default)
        st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.source_path_1;
    }


    QString filename = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(derived_source_info_obj.source_path_1);
    QString pluginName = QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete);
    QString hex_viewer_display_name  = filename + " [" + pluginName + "]" ;

    QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    QStringList value_list;
    value_list << hex_viewer_display_name << source_count_name;

    recon_fileinfo_path = derived_source_info_obj.source_path_1;

    if(recon_fileinfo_path.startsWith(result_dir_path))
    {
        recon_fileinfo_path.remove(0,result_dir_path.size());
    }
    if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
        recon_fileinfo_path.prepend("/");

    recon_file_from_examiner = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path);

    QString tmp_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select file_path from hex_viewer_index Where viewer_display_name = ? AND source_count_name = ?"), value_list, 0, hex_feature_db_path, Q_FUNC_INFO);
    tmp_file_path = tmp_file_path.trimmed();
    if(!tmp_file_path.isEmpty())
    {
        st_hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        st_hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
    }
    else
    {
        st_hex_viewer_open_file_data_obj.bool_is_file_in_bucket = false;
        st_hex_viewer_open_file_data_obj.hex_feature_db_path.clear();
    }

    st_hex_viewer_open_file_data_obj.display_file_path = derived_source_info_obj.source_path_1;
    st_hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_viewer_display_name;
    st_hex_viewer_open_file_data_obj.plugin_name_str = pluginName;
    st_hex_viewer_open_file_data_obj.plugin_tab_name_str = st_hex_viewer_open_file_data_obj.plugin_name_str;
    st_hex_viewer_open_file_data_obj.source_count_name   = derived_source_info_obj.source_count_name;
    st_hex_viewer_open_file_data_obj.recon_file_infopath = recon_fileinfo_path;
    st_hex_viewer_open_file_data_obj.recon_filefrom      = recon_file_from_examiner;

    slot_file_openwith_hex_viewer(st_hex_viewer_open_file_data_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void MainWindow::prepare_thread_for_disk_hex_view_unallocated(QString source_count_name, bool bool_is_complete_source)
{

    if(bool_thread_disk_hex_view_unallocated_running)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Disk Hex View 'Unallocated' is already running.");
        return;
    }

    thread_hex_view_unallocated_obj = new thread_hex_view_unallocated();

    connect(thread_hex_view_unallocated_obj, SIGNAL(finished()), this, SLOT(slot_thread_disk_hex_view_unallocated_finished()));

    connect(thread_hex_view_unallocated_obj, SIGNAL(signal_PBAR_thread_disk_hex_view_unallocated_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_hex_view_unallocated_obj, SIGNAL(signal_PBAR_thread_disk_hex_view_unallocated_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_hex_view_unallocated_obj, SIGNAL(signal_PBAR_thread_disk_hex_view_unallocated_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_disk_hex_view_unallocated_extraction(QString)), this, SLOT(slot_cancel_disk_hex_view_unallocated_extraction(QString)));

    thread_hex_view_unallocated_obj->pub_set_essentials(source_count_name, bool_is_complete_source);

    bool_thread_disk_hex_view_unallocated_running = true;
    thread_hex_view_unallocated_obj->start();

}

void MainWindow::slot_thread_disk_hex_view_unallocated_finished()
{
    bool_thread_disk_hex_view_unallocated_running = false;

    QString source_count_name;
    bool bool_is_complete_disk = false;

    QString current_table_name;

    if(thread_hex_view_unallocated_obj != NULL)
    {
        source_count_name     = thread_hex_view_unallocated_obj->pub_get_source_count_name();
        bool_is_complete_disk = thread_hex_view_unallocated_obj->pub_get_is_complete_disk_status();
        current_table_name    = thread_hex_view_unallocated_obj->pub_get_current_table_name();

        if(thread_hex_view_unallocated_obj->isRunning())
            thread_hex_view_unallocated_obj->quit();

        if(!thread_hex_view_unallocated_obj->isRunning())
            delete thread_hex_view_unallocated_obj;
    }

    source_count_name = source_count_name.trimmed();
    if(source_count_name.isEmpty())
        return;

    ///start hex view here
    prepare_display_for_disk_hex_view(source_count_name, bool_is_complete_disk, true, current_table_name);

}

void MainWindow::slot_cancel_disk_hex_view_unallocated_extraction(QString job_type)
{
    thread_hex_view_unallocated_obj->pub_set_cancel_extraction_status(true);
}


void MainWindow::slot_right_click_remove_story(QString item_text)
{
    item_text = item_text.trimmed();

    if(item_text.isEmpty())
        return;

    ///remove story from database

    QString story_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + "story_board.sqlite";

    QString command = QString("Delete from story_board Where display_name = ?");
    QStringList value_list;
    value_list << item_text;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, story_db_path, Q_FUNC_INFO);

    ///remove story from case tree
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Story_Board))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->childCount(); j++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->child(j)->text(enum_global_case_tree_display_column_ZERO) == item_text)
                {
                    delete treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->child(j);
                    break;
                }
            }
        }
    }

    ///remove from navigation combobox
    QString unique_name = QString(MACRO_CASE_TREE_Story_Board) + QString(MACRO_RECON_Splitter_1_multiple) + item_text;
    right_click_remove_widget_by_unique_name(unique_name, item_text);

}


void MainWindow::slot_pushbutton_status_right_corner_clicked(bool checked)
{
    if(!bool_launcher_work_completed)
        return;

    emit signal_statusbar_button_checked(checked);

    common_pushbutton_status_right_corner_clicked(checked);
}

void MainWindow::slot_remove_category_and_feature_box_for_run_plugin()
{
    remove_selected_plugins_from_display_on_add_source();
}

void MainWindow::slot_remove_plugin_entry_from_display_tree(QStringList plugin_list)
{
    common_remove_plugin_entry_from_display_tree(plugin_list);
}

void MainWindow::common_remove_plugin_entry_from_display_tree(QStringList plugin_list)
{

    remove_category_page_from_stackwidget(MACRO_CASE_TREE_Artifacts);
    remove_plugins_entry_from_tree(plugin_list);

}

void MainWindow::slot_current_plugin_extraction_finished_or_cancelled(QString plugin_name)
{
    common_current_plugin_extraction_finished_or_cancelled(plugin_name);


}


void MainWindow::slot_right_click_global_report_finished_or_cancelled(QStringList report_paths)
{
    common_right_click_global_report_finished_or_cancelled(report_paths);

}

void MainWindow::slot_verifying_source_start(QString name)
{
    emit signal_statusbar_button_checked(true);
    common_pushbutton_status_right_corner_clicked(true);

    task_status_window_obj->set_task_in_taskbar(name,MACRO_JobType_Verifying);
}


void MainWindow::slot_show_progressbar(bool status)
{
    if(status)
        task_status_window_obj->show();
    else
    {
        int running_tasks = task_status_window_obj->get_running_tasks_count();

        if(running_tasks == 0)
            task_status_window_obj->hide();
    }
}

void MainWindow::slot_keyword_search_extraction_start(QString name)
{
    emit signal_statusbar_button_checked(true);
    common_pushbutton_status_right_corner_clicked(true);

    task_status_window_obj->pub_set_source_count_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString());
    task_status_window_obj->set_task_in_taskbar(name,MACRO_JobType_Artifacts_Keyword_Search);
}


void MainWindow::slot_content_search_extraction_start(QString name)
{
    emit signal_statusbar_button_checked(true);
    common_pushbutton_status_right_corner_clicked(true);

    task_status_window_obj->set_task_in_taskbar(name,MACRO_JobType_Content_Search);
}

void MainWindow::slot_show_metadata(bool status)
{
    show_metadata_widget(status);
}

void MainWindow::slot_global_report_extraction_start(QString name)
{
    progress_bar_fixed_in_statusbar_obj->set_status_message("Please wait...");
    progress_bar_fixed_in_statusbar_obj->show();

}

void MainWindow::slot_select_mac_version(QString ver)
{
    mounted_media_os_version = ver;
    bool_os_version_found = true;
}
void MainWindow::slot_generic_process_finished(int)
{
    bool_process_generic = true;
}

void MainWindow::slot_selected_users_list(QStringList list)
{
    selected_user_list_from_mounted_media = list;
    bool_user_list_found = true;
}

void MainWindow::slot_viewer_extraction_finished_or_cancelled(QString result_name, QString viewer_name)
{
    common_viewer_extraction_finish_or_cancelled(result_name,viewer_name);
}


void MainWindow::slot_change_hex_viewer_content_in_metadata_on_row_click(QString file_path, QString source_count_name)
{

    detailed_info_pane_file = file_path;
    detailed_info_pane_file_source_count_name = source_count_name;

    if(tabwidget_metadata_viewer->currentWidget() != hex_displayer_metadata_obj)
    {

        hex_displayer_metadata_obj->pub_set_data(QByteArray(""));
        hex_displayer_metadata_obj->pub_set_source_file_size(0);

        return;
    }

    QFileInfo info(file_path);

    if(!info.exists() ||info.isDir())
    {

        hex_displayer_metadata_obj->pub_set_data(QByteArray(""));
        hex_displayer_metadata_obj->pub_set_source_file_size(0);

        return;
    }

    hex_displayer_metadata_obj->pub_set_global_connection_manager(global_connection_manager_obj);
    hex_displayer_metadata_obj->pub_set_hex_viewer_open_file_data_obj(st_hex_viewer_open_file_data_obj);
    hex_displayer_metadata_obj->pub_set_source_file_size(info.size());
    hex_displayer_metadata_obj->pub_set_source_block_size(512);
    hex_displayer_metadata_obj->pub_set_file_path(file_path,file_path);
}

void MainWindow::slot_change_exif_data_content_on_detatch_click(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index,QString searched_keyword, QString source_count_name)
{
    plugin_name_for_detach_ext_attribute = QString(MACRO_Plugin_Name_File_System);
    table_name_for_detach_ext_attribute  = QString("files");
    //record_number_for_detach_ext_attribute = record_no;


    slot_open_window_for_detach_clicked(metadata_value , hex_file_path , preview_path , preview_index, searched_keyword, source_count_name);
}



void MainWindow::slot_change_apple_metadata_content_on_row_click(QStringList file_path_list, QString source_count_name,QString record_no, QString table_name, QString plugin_name)
{
    return;

    //    recon_static_functions::app_debug("---Starts----",Q_FUNC_INFO);

    //    detailed_info_pane_file_list = file_path_list;
    //    detailed_info_pane_file_source_count_name = source_count_name;
    //    detailed_info_pane_file_record_no = record_no;
    //    detailed_info_pane_file_table_name = table_name;
    //    detailed_info_pane_file_plugin_name = plugin_name;

    //    if(tabwidget_metadata_viewer->currentWidget() != tree_detailed_info_apple_metadata)
    //        return;

    //    plugin_name_for_detach_ext_attribute = plugin_name;
    //    table_name_for_detach_ext_attribute = table_name;
    //    record_number_for_detach_ext_attribute = record_no;

    //    if(file_path_list.isEmpty())
    //    {
    //        tree_detailed_info_apple_metadata->clear();
    //        return;
    //    }

    //    tree_detailed_info_apple_metadata->clear();
    //    QString tmp_plugin_name = plugin_name;


    //    QString destination_db_path;
    //    if(plugin_name == QString(MACRO_Plugin_Name_File_System))
    //        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
    //    else
    //        destination_db_path = QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plugin_name.replace(" ", "_") + "/" + tmp_plugin_name + ".sqlite");


    //    QString command = QString("select bookmarked_extended_attributes from " + table_name + " where INT = ") + record_no;
    //    QString bookmarked_items = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_path,Q_FUNC_INFO);

    //    for(int file_count = 0; file_count < file_path_list.size(); file_count++)
    //    {
    //        QString file_path = file_path_list.at(file_count);

    //        //        struct_GLOBAL_derived_source_info source_info_obj = global_target_m_sources_info_obj->pub_get_target_info_structure_according_source_count_name(source_count_name);

    //        //        if(file_path.startsWith(source_info_obj.virtual_source_path))
    //        //            file_path = file_path.remove(0,source_info_obj.virtual_source_path.size());

    //        if(!file_path.startsWith(QString("/")))
    //            file_path.prepend(QString("/"));


    //        QStringList ext_attribute_list = global_recon_helper_singular_class_obj->get_apple_metadata_from_api(file_path);
    //        if(ext_attribute_list.isEmpty())
    //            continue;

    //        QFileInfo file(file_path);

    //        QTreeWidgetItem *root_item = new QTreeWidgetItem();
    //        root_item->setText(enum_mainwindow_apple_metadata_KEY, file.fileName());
    //        root_item->setText(enum_mainwindow_apple_metadata_Value, "");
    //        tree_detailed_info_apple_metadata->addTopLevelItem(root_item);

    //        for(int i = 0 ; i < ext_attribute_list.size() ; i++)
    //        {
    //            QStringList keyvalue_list = ext_attribute_list.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    //            QString key,value;
    //            if(keyvalue_list.size() > 1)
    //            {
    //                key = keyvalue_list.at(0);
    //                value = keyvalue_list.at(1);
    //            }

    //            if(value.trimmed().isEmpty())
    //                continue;



    //            QTreeWidgetItem *tree_child = new QTreeWidgetItem();
    //            tree_child->setText(enum_mainwindow_apple_metadata_KEY,key);
    //            if(bookmarked_items.contains(key+" :"))
    //                tree_child->setCheckState(enum_mainwindow_apple_metadata_KEY,Qt::Checked);
    //            else
    //                tree_child->setCheckState(enum_mainwindow_apple_metadata_KEY,Qt::Unchecked);


    //            if(value.contains("\n"))
    //            {
    //                QStringList sub_values_list = value.split("\n", Qt::SkipEmptyParts);

    //                int count = 0;
    //                while(count < sub_values_list.size())
    //                {
    //                    QString sub_value = sub_values_list.at(count);

    //                    if(count == 0)
    //                    {
    //                        tree_child->setText(enum_mainwindow_apple_metadata_Value, sub_value);
    //                        tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Table_Name, table_name);
    //                        tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Db_Path, destination_db_path);
    //                        tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Record_no, record_no);
    //                        tree_child->setText(enum_mainwindow_apple_metadata_File_Path, file_path);


    //                        root_item->addChild(tree_child);
    //                    }
    //                    else
    //                    {
    //                        QTreeWidgetItem *tree_sub_child = new QTreeWidgetItem;
    //                        tree_sub_child->setText( enum_mainwindow_apple_metadata_Value, sub_value);

    //                        tree_child->addChild(tree_sub_child);

    //                    }
    //                    count++;
    //                }
    //            }
    //            else
    //            {

    //                tree_child->setText(enum_mainwindow_apple_metadata_Value, value);
    //                tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Table_Name, table_name);
    //                tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Db_Path, destination_db_path);
    //                tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Record_no, record_no);
    //                tree_child->setText(enum_mainwindow_apple_metadata_File_Path, file_path);

    //                root_item->addChild(tree_child);
    //            }

    //        }


    //        break;///so that we need only first file to get extended attributes in case of multiple files attach with single record
    //    }

    //    tree_detailed_info_apple_metadata->expandAll();
    //    tree_detailed_info_apple_metadata->setColumnWidth(enum_mainwindow_apple_metadata_KEY,300);
    //    tree_detailed_info_apple_metadata->setColumnWidth(enum_mainwindow_apple_metadata_Value,400);
    //    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Table_Name,true);
    //    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Db_Path,true);
    //    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Record_no,true);
    //    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_File_Path,true);


    //    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);

}



void MainWindow::slot_ram_analysis_saved_in_case_tree(QString plugin_name)
{
    common_add_ram_analysis_into_case_tree(plugin_name);

}

void MainWindow::slot_ram_analysis_carved_password_in_case_tree(QString tab_name)
{
    common_add_carved_password_into_case_tree(tab_name);
}


void MainWindow::common_add_ram_analysis_into_case_tree(QString plugin_name)
{

    QTreeWidgetItem *sub_child;
    QString ram_analysis_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";
    int ram_analysis_saved_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, ram_analysis_db_path, QStringList(), Q_FUNC_INFO);

    QString icon_path = "../icons/Features/ram_analysis.png";


    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_ANALYSIS_SAVED)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Ram_Analysis_Saved))
        {


            for(int j = 0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_ANALYSIS_SAVED)->childCount(); j++)
            {
                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_ANALYSIS_SAVED)->child(j);
                sub_child->setText(0, plugin_name + " (" + QString::number(ram_analysis_saved_count) + ")");
                sub_child->setIcon(0,QIcon(icon_path));
                sub_child->setText(1,QString(MACRO_CASE_TREE_Ram_Analysis_Saved));
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                return;
            }


            sub_child = new QTreeWidgetItem;
            sub_child->setText(0, plugin_name + " (" + QString::number(ram_analysis_saved_count) + ")");
            sub_child->setIcon(0,QIcon(icon_path));
            sub_child->setText(1,QString(MACRO_CASE_TREE_Ram_Analysis_Saved));
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_RAM_ANALYSIS_SAVED)->addChild(sub_child);
            break;
        }
    }
}

void MainWindow::common_add_carved_password_into_case_tree(QString tab_name)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_CARVED_PASSWORD)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Carved_Password))
        {
            QString case_display_name = tab_name;
            QString carved_password_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
            QString command = "select relative_db_path from tbl_carved_password_index where label_name = ?";

            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(tab_name),0,carved_password_path,Q_FUNC_INFO);
            QString saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + relative_db_path;
            quint64 total_records =  recon_helper_standard_obj->get_db_total_record_by_dbpath(false, saved_db_path, QStringList("carved_password"), Q_FUNC_INFO);


            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(0, QString(case_display_name + " (" + QString::number(total_records) + ")"));

            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/carved_password.png"));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_Carved_Password);
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_CARVED_PASSWORD)->addChild(sub_child);

            if(!bool_loading_result_operation_running)
            {
                message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "Passwords have been carved. Do you want to open it?"));
                message_dialog_object->show();

                bool_message_box_clicked = false;
                while(1)
                {
                    QCoreApplication::processEvents();
                    if(bool_message_box_clicked)
                        break;
                }
                recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                if(bool_message_box_yes_button_clicked)
                {
                    treewidget_case_display_obj->setCurrentItem(sub_child,enum_global_case_tree_display_column_ZERO);
                    common_treewidget_case_display_double_clicked(sub_child,enum_global_case_tree_display_column_ZERO);
                }
            }

            break;
        }
    }
}

void MainWindow::insert_recent_load_case_result_info(QString result_dir_path)
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    if(result_dir_path.trimmed().isEmpty())
        return;

    QString dir_path_without_slash = result_dir_path;
    if(dir_path_without_slash.endsWith("/"))
        dir_path_without_slash.chop(QString("/").size());

    dir_path_without_slash = dir_path_without_slash.trimmed();

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "loaded_results_info.sqlite";
    QString result_d_path_from_db = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select result_dir_path from tbl_loaded_result_info where result_dir_path = ?" , QStringList(dir_path_without_slash) ,0,db_path , Q_FUNC_INFO).trimmed();
    if(result_d_path_from_db == dir_path_without_slash )
    {
        // mean received path is already entered in db
        return;
    }

    QFileInfo file_info(result_dir_path);
    QString case_name = file_info.absoluteDir().dirName();

    QString total_record_str = recon_helper_standard_obj->get_string_from_db_by_dbpath("Select  Count(*)  from tbl_loaded_result_info" ,0,db_path,Q_FUNC_INFO);
    if(total_record_str.toInt() >= 10)
    {
        QString delete_INT_no = recon_helper_standard_obj->get_string_from_db_by_dbpath("Select  MIN(INT)  from tbl_loaded_result_info",0,db_path,Q_FUNC_INFO);
        QString command = "Delete from tbl_loaded_result_info where INT = ?";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, QStringList(delete_INT_no) ,db_path,Q_FUNC_INFO);
    }

    QStringList values_list;
    values_list << case_name << dir_path_without_slash;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("insert into tbl_loaded_result_info(case_name , result_dir_path) Values(?,?)" ,values_list  ,db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::slot_tree_apple_metadata_item_clicked(QTreeWidgetItem *item, int m_column)
{

    if(item->text(enum_mainwindow_apple_metadata_KEY).isEmpty())
        return;

    QString tablename = item->text(enum_mainwindow_apple_metadata_Plugin_Table_Name);
    QString db_path = item->text(enum_mainwindow_apple_metadata_Plugin_Db_Path);
    QString rec_no = item->text(enum_mainwindow_apple_metadata_Plugin_Record_no);
    QString key = item->text(enum_mainwindow_apple_metadata_KEY);
    QString value = item->text(enum_mainwindow_apple_metadata_Value);


    QString command_select = "select  bookmarked_extended_attributes from '" + tablename + "' where INT = ?";
    QStringList Int_value;
    Int_value << rec_no;
    QString stored_ext_attr = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select, Int_value, 0, db_path, Q_FUNC_INFO);

    if(!stored_ext_attr.endsWith("\n"))
        stored_ext_attr.append("\n");



    QString apl_metadata_str = QString(key + " : " + value + "\n");

    if(!item->checkState(enum_mainwindow_apple_metadata_KEY))
    {
        if(stored_ext_attr.contains(apl_metadata_str))
        {
            stored_ext_attr.remove(apl_metadata_str);

            QString command = "update '" + tablename + "' set bookmarked_extended_attributes = ? where INT = ?";
            QStringList list;

            list << stored_ext_attr << rec_no;


            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }
    else
    {

        if(!stored_ext_attr.contains(apl_metadata_str))
        {
            stored_ext_attr +=  QString(key + " : " + value + "\n");

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                stored_ext_attr += item->child(mm)->text( enum_mainwindow_apple_metadata_Value) + "\n";
            }

            QString command = "update '" + tablename + "' set bookmarked_extended_attributes = ? where INT = ?";
            QStringList list;
            list << stored_ext_attr << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }

    QString bookmark_status = QString(MACRO_FALSE_VALUE_NUMERIC_QString);

    bool check_status = check_any_exif_or_apple_metadata_item_selected();
    if(check_status)
        bookmark_status = QString(MACRO_TRUE_VALUE_NUMERIC_QString);

    handle_bookmark_from_detailed_info_apple_metadata_clicked(rec_no,bookmark_status);


}

bool MainWindow::check_any_exif_or_apple_metadata_item_selected()
{
    bool check_status = false;

    if(tree_exif_metadata_display != nullptr)
    {
        for(int count = 0; count < tree_exif_metadata_display->topLevelItemCount(); count++)
        {
            QTreeWidgetItem *child_item = tree_exif_metadata_display->topLevelItem(count);
            if(child_item != nullptr)
            {

                if(child_item->checkState(0) == Qt::Checked)
                {
                    check_status = true;
                    break;
                }
            }
        }
    }

    if(check_status)
        return check_status;


    if(tree_detailed_info_apple_metadata != nullptr)
    {
        for(int pp = 0; pp < tree_detailed_info_apple_metadata->topLevelItemCount(); pp++)
        {
            QTreeWidgetItem *root_item = tree_detailed_info_apple_metadata->topLevelItem(pp);
            if(root_item != nullptr)
            {
                for(int cc = 0; cc < root_item->childCount(); cc++)
                {
                    QTreeWidgetItem *child_item = root_item->child(cc);
                    if(child_item != nullptr)
                    {

                        if(child_item->checkState(0) == Qt::Checked)
                        {
                            check_status = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    return check_status;
}

void MainWindow::slot_case_launched(QString case_nm,QList<struct_GLOBAL_witness_info_root> list_root_info)
{
    launched_case_name = case_nm;
    bool_main_launcher_call = true;
    case_wizard_object->hide();

    task_status_window_obj->hide();
    this->show();

    show_launcher_add_source(list_root_info);

    this->activateWindow();
}

void MainWindow::slot_show_file_in_viewer(QString tab_text,QString viewer_name, QString source_count_name)
{

    if(viewer_name == MACRO_CASE_TREE_Plist_Viewer)
    {
        QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

        ///open file from bucket if in bucket
        QString plist_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
        QStringList value_list;
        value_list << tab_text << source_count_name;
        QString file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select file_path from plist_viewer_index Where viewer_display_name = ? AND source_count_name = ?"), value_list, 0, plist_feature_db_path, Q_FUNC_INFO);
        file_path = file_path.trimmed();
        if(!file_path.isEmpty())
        {
            sqlite_path = plist_feature_db_path;
        }

        display_case_tabs(tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Plist_Viewer,"",sqlite_path,Q_FUNC_INFO);
    }
    else if(viewer_name == MACRO_CASE_TREE_Sqlite_Viewer)
    {
        QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

        display_case_tabs(tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Sqlite_Viewer,"",sqlite_path,Q_FUNC_INFO);
    }
    else if(viewer_name == MACRO_CASE_TREE_Hex_Viewer)
    {
        QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
        ///open file from bucket if in bucket
        QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
        QStringList value_list;
        value_list << tab_text << source_count_name;

        QString file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select file_path from hex_viewer_index Where viewer_display_name = ? AND source_count_name = ?"), value_list, 0, hex_feature_db_path, Q_FUNC_INFO);
        file_path = file_path.trimmed();
        if(!file_path.isEmpty())
        {
            sqlite_path = hex_feature_db_path;
        }

        display_case_tabs(tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Hex_Viewer,"",sqlite_path,Q_FUNC_INFO);
    }
    else if(viewer_name == MACRO_CASE_TREE_Registry_Viewer)
    {


        ///first check at adhoc path
        QString registry_tmp_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
        QStringList value_list;
        value_list << tab_text << source_count_name;

        QString command = QString("Select source_file_path from tbl_registry_viewer_index Where display_tab_name = ? AND source_count_name = ?");
        QString file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, registry_tmp_index_db_path, Q_FUNC_INFO);
        file_path = file_path.trimmed();
        if(!file_path.isEmpty())
        {
            display_case_tabs(tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Registry_Viewer,"",registry_tmp_index_db_path,Q_FUNC_INFO);
        }
        else
        {
            /// then check at actual path
            QString registry_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
            QString file_path_1 = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, registry_index_db_path, Q_FUNC_INFO);
            file_path_1 = file_path_1.trimmed();
            if(!file_path_1.isEmpty())
            {
                display_case_tabs(tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Registry_Viewer,"",registry_index_db_path,Q_FUNC_INFO);
            }
        }

    }
    else if(viewer_name == MACRO_CASE_TREE_Log_Viewer)
    {
        QString log_viewer_adhoc_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString).toString();

        ///open file from bucket if in bucket
        QString log_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";
        QStringList value_list;
        value_list << tab_text << source_count_name;
        QString file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select file_path from log_viewer_index Where viewer_display_name = ? AND source_count_name = ?"), value_list, 0, log_feature_db_path, Q_FUNC_INFO);
        file_path = file_path.trimmed();
        if(!file_path.isEmpty())
        {
            log_viewer_adhoc_db_path = log_feature_db_path;
        }

        display_case_tabs(tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Log_Viewer,"",log_viewer_adhoc_db_path,Q_FUNC_INFO);
    }

}

void MainWindow::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;
}


void MainWindow::slot_new_case_wizard_finishes(int status)
{
    if(status != 1)
        return;

    emit signal_show_r_case_triggered();
}

void MainWindow::slot_decompressed_files_view(QString plugin_name, QString parent_name, QString tab_name, QString record , QString db_path, QString source_count_name)
{

    recon_static_functions::app_debug(" plugin_name - " + plugin_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" parent_name - " + parent_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" tab_name - " + tab_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" record - " + record, Q_FUNC_INFO);
    recon_static_functions::app_debug(" db_path - " + record, Q_FUNC_INFO);
    recon_static_functions::app_debug(" source_count_name - " + source_count_name, Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(parent_name == QString(MACRO_CASE_TREE_Source))
    {
        QFileInfo info_file(tab_name);
        QString dir_path = info_file.absolutePath();

        if(dir_path.endsWith("/") && dir_path.size() > 1)
            dir_path.chop(1);


        plugin_name = plugin_name + " ("  +  struct_target_info.source_main_name  + ")";

        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),parent_name,dir_path,db_path,Q_FUNC_INFO);

        emit signal_goto_file_system_record(tab_name,record, source_count_name);
        emit signal_decompressed_files_view_file_system_record(tab_name);
    }
}

void MainWindow::slot_tablewidget_goto_record(QString plugin_name, QString parent_name, QString tab_name, QString record , QString db_path, QString os_name_str)
{

    recon_static_functions::app_debug(" plugin_name - " + plugin_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" parent_name - " + parent_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" tab_name - " + tab_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" record - " + record, Q_FUNC_INFO);
    recon_static_functions::app_debug(" db_path - " + db_path, Q_FUNC_INFO);
    recon_static_functions::app_debug(" os_name_str - " + os_name_str, Q_FUNC_INFO);

    if(parent_name == QString("Parent RECON Plugin"))
    {
        if(!plugin_name.contains(" ("))
        {
            QString plugin_db_path = get_plugin_db_path_from_result_using_plugin_name(plugin_name);
            qint64 total_records = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, plugin_db_path, QStringList() , Q_FUNC_INFO);
            plugin_name = plugin_name + " (" + QString::number(total_records) + ")";
        }

        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Artifacts),parent_name,"",Q_FUNC_INFO);

        emit signal_goto_plugin_record(tab_name,record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Source))
    {
        QStringList db_path_split_list = db_path.split("/", Qt::SkipEmptyParts);

        QString source_count_name;

        if(db_path_split_list.size() > 2)
            source_count_name = db_path_split_list.at(db_path_split_list.size() - 2);

        QFileInfo info_file(tab_name);
        QString dir_path = info_file.absolutePath();

        if(dir_path.endsWith("/") && dir_path.size() > 1)
            dir_path.chop(1);

        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


        QString src_cnt_name_label = QString(" [" + struct_target_info.source_count_name + "]");
        plugin_name = plugin_name + " ("  +  struct_target_info.source_main_name  + ")" + src_cnt_name_label;

        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),parent_name,dir_path,db_path,Q_FUNC_INFO);

        emit signal_goto_file_system_record(tab_name,record, source_count_name);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Email_Files_Parent))
    {

        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Email_Files_Parent),parent_name,"",Q_FUNC_INFO);
        emit signal_goto_email_files_record(record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Screenshots))
    {
        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Screenshots),parent_name,"",Q_FUNC_INFO);

        emit signal_goto_screenshot_record(record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Graph_Result_Saved))
    {
        QString db_path;
        if(plugin_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
        {
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString()  + "Browser_History.sqlite";
            display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs),parent_name,db_path,Q_FUNC_INFO);
        }
        else if(plugin_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
        {
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
            display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs),parent_name,db_path,Q_FUNC_INFO);
        }
        emit signal_goto_saved_graphs_record(record);

    }
    else if(parent_name == QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph))
    {
        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph),parent_name,"",Q_FUNC_INFO);

        ///because its using screenshot class for display
        emit signal_goto_screenshot_record(record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Saved_Maps))
    {
        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Saved_Maps),parent_name,"",Q_FUNC_INFO);

        ///because its using screenshot class for display
        emit signal_goto_screenshot_record(record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
    {
        display_case_tabs(plugin_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Redefined_Result_Parent),parent_name,"",Q_FUNC_INFO);
        emit signal_goto_redefined_result(record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Carved_Files))
    {
        display_case_tabs(tab_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Carved_Files),parent_name,"",Q_FUNC_INFO);
        emit signal_goto_carved_files_record(record);
    }
    else if(parent_name == QString(MACRO_CASE_TREE_Carved_Password))
    {
        display_case_tabs(tab_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Carved_Password),parent_name,"",Q_FUNC_INFO);
        emit signal_goto_carved_password_record(record);
    }
    else if(parent_name == MACRO_CASE_TREE_Carved_Data)
    {
        display_case_tabs(tab_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Carved_Data),parent_name,"",Q_FUNC_INFO);
        emit signal_goto_carved_data(record);
    }



}

void MainWindow::slot_update_status_bar_message(QString message)
{
    label_status_bar->setText(message);
}

void MainWindow::slot_extracted_records_by_plugin(qint64 records)
{
    if(task_status_window_obj == NULL)
        return;

    task_status_window_obj->set_extracted_records_in_progress_for_artifacts_plugin(records);
}

void MainWindow::slot_change_display_message_of_task_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress)
{
    if(task_status_window_obj == NULL)
        return;

    task_status_window_obj->pub_change_display_message_in_task_progress_bar(task_type,current_plugin_name, message, bool_show_close_button, max_size_of_progress);
}

void MainWindow::slot_set_progressbar_value(int tabs , QString job_title)
{
    if(task_status_window_obj == NULL)
        return;

    task_status_window_obj->set_progressbar_value(tabs , job_title);
}

void MainWindow::slot_progress_count_for_progress_bar(qint64 total_record_count, qint64 record_displayed, float progress_percent)
{
    if(total_record_count == 0)
    {
        progress_bar_fixed_in_statusbar_obj->set_record_count_string("");
        progress_bar_fixed_in_statusbar_obj->set_progressbar_value(int(progress_percent));
        return;
    }

    QString count_str = "[" +QString::number(record_displayed) +" Out of " + QString::number(total_record_count) + "]";

    progress_bar_fixed_in_statusbar_obj->set_record_count_string(count_str);
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(int(progress_percent));
}

void MainWindow::slot_go_to_artifact_source(QString artifact_source_path, QString fs_db_path, QString source_count_name)
{
    struct_GLOBAL_witness_info_source struct_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, global_lang_tr_obj.get_translated_string("Source not extracted."));
        message_dialog_object->show();
        return;
    }


    QString source_name = struct_src_info.source_name;
    QString source_parent_name = struct_src_info.source_main_name;

    QString src_cnt_name_label = QString(" [" + struct_src_info.source_count_name + "]");
    QString display_name = source_name + " (" + source_parent_name + ")" + src_cnt_name_label;


    QString artifact_tmp_path = artifact_source_path;

    if(!artifact_tmp_path.startsWith("/") && !artifact_tmp_path.isEmpty())
        artifact_tmp_path.prepend("/");

    QString command = "SELECT INT FROM files WHERE filepath = ?";
    QStringList arg_list;
    arg_list << artifact_tmp_path;


    QString record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,fs_db_path,Q_FUNC_INFO);

    if(record.isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"Artifact source file is not extracted.");
        message_dialog_object->show();
        return;
    }

    display_case_tabs(display_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Source),"0",fs_db_path,Q_FUNC_INFO);   /* Here the argument 0 is
                                                                                               for insert please wait
                                                                                               in FS display while
                                                                                               extraction for this
                                                                                               clicked dir takes place */

    artifact_source_path = recon_static_functions::get_complete_file_path(struct_src_info.virtual_source_path, artifact_source_path, Q_FUNC_INFO);


    QFileInfo info_file(artifact_source_path);
    QString parent_dir_path = info_file.absolutePath();

    if(parent_dir_path.endsWith("/") && parent_dir_path.size() > 1)
        parent_dir_path.chop(1);


    emit signal_goto_file_system_record(artifact_tmp_path,"", source_count_name);
}



void MainWindow::slot_set_image_mounter_progress_message(QString message)
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge(message);
}

void MainWindow::slot_statusbar_progressbar_show_hide(bool value, QString message)
{
    if(value)
    {
        progress_bar_fixed_in_statusbar_obj->set_status_message(message);
        progress_bar_fixed_in_statusbar_obj->show();
    }
    else
    {
        progress_bar_fixed_in_statusbar_obj->hide();
    }
}

void MainWindow::slot_quick_look_preview_item_email_files(QString source_path)
{
    if(source_path.trimmed().isEmpty())
        return;

    slot_quick_look_preview_item(source_path);

}

void MainWindow::slot_action_quick_look_triggered()
{

    if(!bool_launcher_work_completed)
        return;


    if(bool_is_lab_running_from_root)
    {
        QString filepath = QString("__START__") + QString(MACRO_RECON_Splitter_1_multiple) + quick_look_file_preview_path;

        socket_quicklook->write(filepath.toLocal8Bit().data());
        socket_quicklook->flush();

        return;
    }

    if(quick_look_obj == NULL)
        return;

    quick_look_obj->pub_set_preview_closed_status(false);
    quick_look_obj->pub_set_preview_filepath(quick_look_file_preview_path);

}

void MainWindow::slot_quick_look_preview_item(QString source_path)
{


    quick_look_file_preview_path = source_path;

    if(bool_is_lab_running_from_root)
    {
        socket_quicklook->write(source_path.toLocal8Bit().data());
        socket_quicklook->flush();
        return;
    }


    if(quick_look_obj->pub_get_preview_closed_status())
    {
        return;
    }

    quick_look_obj->pub_set_preview_filepath(source_path);

}


void MainWindow::slot_right_click_remove_selected_item()
{
    common_right_click_remove_selected_clicked();
}

void MainWindow::slot_plugin_viewer_space_pressed_for_quicklook(QString filepath)
{
    quick_look_file_preview_path = filepath;

    slot_action_quick_look_triggered();
}

void MainWindow::slot_aboutToShow_menubar_view()
{
    //    if(combobox_category_navigation->currentText() == MACRO_CASE_TREE_PLUGINS)
    //    {
    //        ui->actionPlugin_Summary->setEnabled(true);
    //    }else
    //    {
    //        ui->actionPlugin_Summary->setEnabled(false);
    //    }

}

void MainWindow::slot_insert_tag_detail_into_tag_search_db_from_hex_viewer(QString file_path, QString tag_name, uint64_t start_index, uint64_t end_index, QString record_no, QString hex_viewer_display_name, QString source_count_name, QString selected_data_str)
{
    recon_static_functions::app_debug("slot_insert_tag_detail_into_tag_search_db_from_hex_viewer start",Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QFileInfo info(file_path);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                      "os_scheme_display,source_count_name, item0, item1, item2, item3, item4,csv_plugin_name,csv_tab_name) VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?, ?,?)";

    QStringList arg_list;
    arg_list << QString(MACRO_Plugin_Name_Hex_Viewer) << QString(MACRO_Plugin_Name_Hex_Viewer) << hex_viewer_display_name
             << "hex_content_info" << record_no << tag_name << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display)
             << source_count_name<< info.fileName() << file_path << QString::number(start_index) << QString::number(end_index) << selected_data_str
             << QString(MACRO_Plugin_Name_Hex_Viewer) <<QString(MACRO_Plugin_Name_Hex_Viewer);

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tag_db_path,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" insert table tags from hex viewer ---FAILED---" + tag_db_path,Q_FUNC_INFO);
    }

    emit signal_update_tags_search_display_for_singal_record(record_no,QString(MACRO_Plugin_Name_Hex_Viewer),hex_viewer_display_name,tag_name,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display));

    common_add_tag_name_into_case_tree(tag_name);
    update_story_board_display_on_adding_tags();

    recon_static_functions::app_debug("slot_insert_tag_detail_into_tag_search_db_from_hex_viewer end",Q_FUNC_INFO);
}



void MainWindow::slot_insert_tag_detail_into_tag_search_db_from_sqlite_viewer(QString file_path, QString tag_name,  QString record_no, QString sqlite_viewer_display_name, QString source_count_name,QString sqliteviewer_db_table_name_of_tag, QString executed_query, QString sql_tab_name)
{
    //    recon_static_functions::app_debug("slot_insert_tag_detail_into_tag_search_db_from_sqlite_viewer start",Q_FUNC_INFO);


    //    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString(),Q_FUNC_INFO);


    //    QFileInfo info(file_path);

    //    QString tab_name =  sql_tab_name;
    //    tab_name = tab_name.remove(tab_name.indexOf("-"),tab_name.size());

    //    if(tab_name == QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB))
    //    {
    //        QStringList arg_previous_list;
    //        arg_previous_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sql_tab_name << record_no << tag_name;
    //        QString INT_str = global_recon_555_helper_singular_class_obj->get_string_from_db_with_addbindvalue_by_dbpath("select INT from tags where plugin_name = ? AND tab_name = ? AND record_no = ? AND recon_tag_value = ?  ", arg_previous_list, 0, tag_db_path, Q_FUNC_INFO);

    //        if(INT_str.isEmpty())
    //        {
    //            QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
    //                              "os_scheme_display,source_count_name, item0, item1, item2,item3,item4,csv_plugin_name,csv_tab_name) VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?, ?,?)";

    //            QStringList arg_list;
    //            arg_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sql_tab_name
    //                     << "sv_content_info" << record_no << tag_name << QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_otherOS) << source_count_name
    //                     << info.fileName() << file_path << sqliteviewer_db_table_name_of_tag << sqlite_viewer_display_name << executed_query.replace(";", "\n")
    //                     << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer);

    //            if(!recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,tag_db_path,Q_FUNC_INFO))
    //            {
    //                recon_static_functions::app_debug(" insert table tags from hex viewer ---FAILED---" + tag_db_path,Q_FUNC_INFO);
    //            }

    //            emit signal_update_tags_search_display_for_singal_record(record_no,QString(MACRO_Plugin_Name_Sqlite_Viewer),sql_tab_name,tag_name,QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_otherOS));
    //        }
    //    }
    //    else
    //    {
    //        QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
    //                          "os_scheme_display,source_count_name, item0, item1, item2,item3,item4,csv_plugin_name,csv_tab_name) VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?)";

    //        QStringList arg_list;
    //        arg_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sql_tab_name
    //                 << "sv_content_info" << record_no << tag_name << QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_otherOS) << source_count_name
    //                 << info.fileName() << file_path << sqliteviewer_db_table_name_of_tag<<sqlite_viewer_display_name << executed_query.replace(";", "\n")
    //                 << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer);

    //        if(!recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,tag_db_path,Q_FUNC_INFO))
    //        {
    //            recon_static_functions::app_debug(" insert table tags from hex viewer ---FAILED---" + tag_db_path,Q_FUNC_INFO);
    //        }

    //        emit signal_update_tags_search_display_for_singal_record(record_no,QString(MACRO_Plugin_Name_Sqlite_Viewer),sql_tab_name,tag_name,QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_otherOS));
    //    }

    //    common_add_tag_name_into_case_tree(tag_name);
    //    update_story_board_display_on_adding_tags();

    recon_static_functions::app_debug("slot_insert_tag_detail_into_tag_search_db_from_sqlite_viewer end",Q_FUNC_INFO);
}

void MainWindow::slot_prc_generic_without_argument_finished(int)
{
    bool_prc_generic_without_argument = true;
}

void MainWindow::slot_screenshots_canceled()
{
    action_screenshot->setDisabled(false);

    screenshots_obj->hide();
}

void MainWindow::slot_screenshots_work_complete(QString filepath, QString notes, QString tags)
{
    // No need to save as relative path,
    // bcoz user can save anywhere

    QFileInfo file(filepath);
    if(file.exists())
    {

        QDateTime date_created = file.birthTime();

        qint64 epch_time = date_created.toSecsSinceEpoch();

        QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

        QString command = "insert into tags(plugin_name,category, tab_name,item0, item1, item2, notes, recon_tag_value, os_scheme_display,bookmark,recon_filefrom , recon_file_infopath) values(?,?,?,?,?,?,?,?,?,?,?,?)";
        QString recon_file_from_examiner = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path) ;
        QStringList values;
        values << QString(MACRO_Plugin_Name_Screenshots)<< QString(MACRO_Plugin_Name_Screenshots)
               << QString(MACRO_Plugin_Screenshots_TAB_Name_Screenshots) << file.fileName() <<  filepath
               << QString::number(epch_time) << notes << QString(MACRO_Tag_Name_Screenshots)
               << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display << "" << recon_file_from_examiner << filepath;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values,screenshot_db_path,Q_FUNC_INFO);

        ///Update Record No value from INT
        command = QString("Select INT From tags Where item1=?");
        values.clear();
        values << filepath;

        QString record_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, values ,0 , screenshot_db_path,Q_FUNC_INFO);

        command = QString("UPDATE tags set record_no = ? Where INT=?");
        values.clear();
        values << record_str << record_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values,screenshot_db_path,Q_FUNC_INFO);

        common_add_screenshots_into_case_tree(MACRO_Plugin_Name_Screenshots);

        QString command_2 = "select INT from tags where item0 = ? AND item1 = ? AND notes = ?";
        QStringList values_2;
        values_2 << file.fileName()<< filepath << notes;

        QString int_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_2,values_2,0, screenshot_db_path, Q_FUNC_INFO );
        QString result_note_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

        if(!notes.trimmed().isEmpty())
            slot_add_notes_added_in_database_for_screenshot(int_str, QString(MACRO_Plugin_Name_Screenshots), filepath, "", result_note_db_path, notes);

        emit signal_update_tags_search_display_for_singal_record(int_str , QString(MACRO_Plugin_Name_Screenshots) ,QString(MACRO_Plugin_Name_Screenshots) , QString(MACRO_Plugin_Name_Screenshots) , QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display));
    }

    action_screenshot->setDisabled(false);

    screenshots_obj->hide();

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("Screenshot has been saved. Do you want to open it?")));
    message_dialog_object->show();

    recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
    int proc_counter = 0;
    while(1)
    {
        proc_counter++;
        if(proc_counter % 100 == 0)
        {
            proc_counter = 0;
            QCoreApplication::processEvents();
        }
        if(bool_message_box_clicked)
            break ;

    }

    if(bool_message_box_yes_button_clicked)
    {
        prepare_display_for_screenshots(MACRO_CASE_TREE_Screenshots);
    }

    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

}

void MainWindow::slot_saved_graph_category_name(QString category_name)
{
    common_add_saved_graph_category_name_in_case_tree(category_name);
}

void MainWindow::slot_set_case_tree_item_bold_on_navigation_click(QString text)
{

    if(text.trimmed().isEmpty())
        return;


    if(text.contains(QString(MACRO_RECON_Splitter_1_multiple)))
    {
        QStringList navigation_text_split_list = text.split(QString(MACRO_RECON_Splitter_1_multiple), Qt::SkipEmptyParts);

        if(navigation_text_split_list.size() > 2)
        {

            QString case_text = navigation_text_split_list.at(0);
            QString parent_item_text = navigation_text_split_list.at(1);
            QString item_text = navigation_text_split_list.at(2);



            /// iterating through all items of Case Tree for setting
            /// selected item bold

            QFont font;
            font.setBold(false);
            QTreeWidgetItemIterator m_iter(treewidget_case_display_obj);


            if(parent_item_text == QString(MACRO_CASE_TREE_Case))
            {

                while(*m_iter)
                {
                    if ((*m_iter)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Case) + case_text)
                    {
                        font.setBold(true);
                        (*m_iter)->setFont(0,font);
                    }
                    else
                    {
                        font.setBold(false);
                        (*m_iter)->setFont(0,font);
                    }

                    ++m_iter;
                }

            }
            else if(parent_item_text == QString(MACRO_CASE_TREE_Source))
            {

                while(*m_iter)
                {
                    if ((*m_iter)->text(enum_global_case_tree_display_column_ZERO) == item_text)
                    {
                        font.setBold(true);
                        (*m_iter)->setFont(0,font);
                    }
                    else
                    {
                        font.setBold(false);
                        (*m_iter)->setFont(0,font);
                    }

                    ++m_iter;
                }

            }
            else
            {
                while(*m_iter)
                {
                    if ((*m_iter)->text(enum_global_case_tree_display_column_ZERO) == item_text && (*m_iter)->text(enum_global_case_tree_display_column_FIRST) == parent_item_text)
                    {
                        font.setBold(true);
                        (*m_iter)->setFont(0,font);
                    }
                    else
                    {
                        font.setBold(false);
                        (*m_iter)->setFont(0,font);
                    }

                    ++m_iter;
                }
            }


        }
    }
}


void MainWindow::slot_content_search_done_clicked(QString search_label)
{
    QStringList keyword_list;
    keyword_list << recon_helper_standard_obj->get_keyword_list_for_search_from_db(Q_FUNC_INFO);

    recon_case_obj->pub_content_search_start(keyword_list, false, QString(""), QString(""),search_label);

}

void MainWindow::slot_index_search_on_file_system_current_dir(QStringList keyword_list, bool index_search_in_temp_mode, QString dir_path_to_iterate, QString source_count_name)
{
    recon_case_obj->pub_content_search_start(keyword_list, index_search_in_temp_mode, dir_path_to_iterate, source_count_name,QString(""));
}


void MainWindow::slot_file_system_post_launcher_job_list(struct_global_file_system_post_launcher_job_info obj)
{
    file_system_post_launcher_obj->hide();
    recon_case_obj->pub_set_file_system_selected_job_source_count_name(obj);
    recon_case_obj->pub_start_file_system_post_launcher_jobs();

}

void MainWindow::slot_disable_case_tree_sidebar(bool status)
{
    disable_case_tree_sidebar(status);
}


void MainWindow::slot_start_file_search_thread(struct_global_selected_regular_filters_info struct_search_filters_info, QString mdfind_condition, QString search_label_name, bool bool_run_in_full_mode, QString dir_path_to_search, QString fs_source_count_name)
{
    recon_case_obj->pub_file_search_interface_work_done(struct_search_filters_info, mdfind_condition, search_label_name, bool_run_in_full_mode, dir_path_to_search, fs_source_count_name);
}


void MainWindow::slot_remove_entry_file_type_based_in_stack_and_refresh_widget(QString parent_name, QString child_name)
{
    QString unique_pattrn = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + MACRO_RECON_Splitter_1_multiple + parent_name + MACRO_RECON_Splitter_1_multiple + child_name;

    //- Remove Widget fromstack.
    for(int i = (stack_widget_cases_plugins_and_global_items->count() -1) ; i >= 0; i--)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        if(widget_ptr->pub_get_case_name() != global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString())
            continue;

        if(widget_ptr->pub_get_widget_unique_name() == unique_pattrn)
        {
            stack_widget_cases_plugins_and_global_items->removeWidget(widget_ptr);
            center_stackwidget_manager_obj->pub_remove_entry_feature_form_list(unique_pattrn);
            slot_refresh_widgets();
            break;
        }
    }

    int plugins_entry_index = 0;
    for(int i = 0; i < combobox_category_navigation->count(); i++)
    {
        if(combobox_category_navigation->itemText(i) == parent_name)
        {
            plugins_entry_index = i;
            break;
        }
    }

    if(combobox_category_navigation->itemText(plugins_entry_index) == parent_name)
    {
        for(int i = 0; i < combobox_current_displayed_feature_opened->count(); i++)
        {
            if(combobox_current_displayed_feature_opened->itemText(i) == child_name)
            {
                combobox_current_displayed_feature_opened->removeItem(i);
                break;
            }
        }
    }

    //-Remove From Category Combobox.
    QStringList features_list = center_stackwidget_manager_obj->pub_get_feature_name_list(parent_name);
    if(features_list.isEmpty())
    {
        for(int i = 0; i < combobox_category_navigation->count(); i++)
        {
            if(combobox_category_navigation->itemText(i) == parent_name)
            {
                combobox_category_navigation->removeItem(i);
                break;
            }
        }
    }
}


void MainWindow::slot_storyboard_save_clicked()
{


    QString default_story_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString();

    QString story_name = lineedit_storyboard_filename->text().trimmed();

    slot_storyboard_cancel_clicked();

    QString current_story_dir_path = recon_static_functions::get_available_filename(story_name, default_story_path, Q_FUNC_INFO);

    story_name = current_story_dir_path;

    current_story_dir_path = default_story_path + "/" + current_story_dir_path;

    QDir dir_current_story(current_story_dir_path);
    if(!dir_current_story.exists())
    {
        dir_current_story.mkdir(current_story_dir_path);
    }

    QString resources_dir_path = current_story_dir_path + "/Resources";

    QDir dir_story_resources(resources_dir_path);
    if(!dir_story_resources.exists())
    {
        dir_story_resources.mkdir(resources_dir_path);
    }

    QString filepath = current_story_dir_path + QString("/") + story_name + QString(".html");

    QFileInfo file_check(filepath);
    if(file_check.exists())
    {
        recon_static_functions::app_debug("File Already Exist ",Q_FUNC_INFO);
        return;
    }


    QFile file(filepath);
    if(!file.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("File Creation -------FAILED------ :" + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(file.errorString() , Q_FUNC_INFO);
        return;
    }
    file.close();

    QString storyboard_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + "story_board.sqlite";
    QString filename = file_check.fileName();
    QString suffix = recon_helper_standard_obj->mining_suffix(filename, Q_FUNC_INFO);

    QString display_name = filename;
    if(display_name.endsWith(QString("." + suffix)))
    {
        display_name.remove(display_name.lastIndexOf(QString("." + suffix)), display_name.size());
    }

    if(filepath.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
        filepath.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());

    QString recon_file_infopath;
    recon_file_infopath = filepath;
    if(!recon_file_infopath.trimmed().isEmpty() && !recon_file_infopath.startsWith("/"))
        recon_file_infopath.prepend("/");

    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString command = "insert into story_board(category, filename, filepath,display_name,recon_filefrom,recon_file_infopath) values(?,?,?,?,?,?)";

    QStringList values;
    values << QString(MACRO_Plugin_Name_Story_Board)<<  file_check.fileName() << filepath << display_name << recon_file_from_result << recon_file_infopath;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values,storyboard_db_path,Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    common_add_story_board_into_case_tree(display_name);

    display_case_tabs(display_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Story_Board,"",storyboard_db_path,Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void MainWindow::slot_lineedit_filename_storyboard_text_changed(QString text_str)
{
    pushbutton_create_storyboard->setEnabled(false);

    QString current_text = text_str.trimmed();

    if(current_text.isEmpty())
        return;

    pushbutton_create_storyboard->setEnabled(true);

}

void MainWindow::slot_storyboard_cancel_clicked()
{
    dialog_save_storyboard->hide();
    lineedit_storyboard_filename->setText(QString(""));
    pushbutton_create_storyboard->setEnabled(false);

}

void MainWindow::slot_save_tabs_in_artifacts_timleine_saved_case_tree(QString tab_name)
{
    common_add_artifacts_timeline_name_into_case_tree(tab_name);
}

void MainWindow::slot_save_tabs_in_redefined_result_saved_case_tree(QString tab_name, QString plugin_name)
{
    common_add_redefined_saved_result_name_into_case_tree(tab_name , plugin_name);
}


void MainWindow::slot_insert_tag_detail_into_tag_search_db_from_text_viewer(QString file_path, QString tag_name, uint64_t start_index, uint64_t end_index, QString record_no, QString hex_viewer_display_name, QString source_count_name)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QFileInfo info(file_path);

    QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                      "os_scheme_display,source_count_name, item0, item1, item2, item3, item4,csv_plugin_name,csv_tab_name) VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?, ?,?)";

    QStringList arg_list;
    arg_list << QString(MACRO_Plugin_Name_Text_Viewer) << QString(MACRO_Plugin_Name_Text_Viewer) << hex_viewer_display_name
             << "text_content_info" << record_no << tag_name << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display)
             << source_count_name<< info.fileName() << file_path << QString::number(start_index) << QString::number(end_index) << ""
             << QString(MACRO_Plugin_Name_Text_Viewer) <<QString(MACRO_Plugin_Name_Text_Viewer);

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tag_db_path,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" insert table tags from text viewer ---FAILED---" + tag_db_path,Q_FUNC_INFO);
    }

    emit signal_update_tags_search_display_for_singal_record(record_no,QString(MACRO_Plugin_Name_Text_Viewer),hex_viewer_display_name,tag_name,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display));

    common_add_tag_name_into_case_tree(tag_name);
    update_story_board_display_on_adding_tags();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

QString MainWindow::get_plugin_db_path_from_result_using_plugin_name(QString plugin_name)
{
    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

    return plugins_destination_db_file;
}

void MainWindow::handle_bookmark_from_detailed_info_apple_metadata_clicked(QString record_no_str, QString bookmark_status)
{

    Container_QWidget *widget = get_currenly_loaded_display_obj();
    if(widget == nullptr)
        return;

    QLayout  *main_layout = widget->layout();
    if(main_layout == nullptr)
        return;

    QVariant *current_widget = (QVariant *)main_layout->itemAt(0)->widget();
    if(current_widget == nullptr)
        return;

    QString current_category = widget->pub_get_category_name();

    QString category_name = widget->pub_get_category_name();
    QString widget_name   = widget->pub_get_feature_widget_name();


    if(category_name == QString(MACRO_CASE_TREE_Artifacts))
    {
        //for plugins only
        plugin_viewer *plugin_viewer_obj = (plugin_viewer *)current_widget;
        if(plugin_viewer_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                plugin_viewer_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                plugin_viewer_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }

    }
    else if(category_name == QString(MACRO_CASE_TREE_Mobile_Backup_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Disk_Images_Parent))
    {
        disk_images_loader *disk_images_loader_obj = (disk_images_loader *)current_widget;
        if(disk_images_loader_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                disk_images_loader_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                disk_images_loader_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Artifacts_Keyword_Search))
    {
        artifacts_keyword_search_loader *keyword_search_loader_obj = (artifacts_keyword_search_loader *)current_widget;
        if(keyword_search_loader_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                keyword_search_loader_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                keyword_search_loader_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions)
            || category_name == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions)
            || category_name == QString(MACRO_CASE_TREE_MIME_Type)
            || category_name == QString(MACRO_CASE_TREE_File_Size_Parent)
            || category_name == QString(MACRO_CASE_TREE_Apple_Metadata_Parent)
            || category_name == QString(MACRO_CASE_TREE_File_Search)
            || category_name == QString(MACRO_CASE_TREE_Content_Search)
            || category_name == QString(MACRO_CASE_TREE_File_Signature_Parent)
            || category_name == QString(MACRO_CASE_TREE_Hashset)
            || (category_name == QString(MACRO_CASE_TREE_Redefined_Result_Parent) && widget_name.contains(QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations)))
            || category_name == QString(MACRO_CASE_TREE_Exif_Parent)
            || category_name == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
    {
        filetypes_loader *filetypes_obj = (filetypes_loader *)current_widget;
        if(filetypes_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                filetypes_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                filetypes_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Source))
    {
        file_system *file_system_obj = (file_system *)current_widget;
        if(file_system_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                file_system_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                file_system_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Artifacts_Timeline))
    {
        artifacts_timeline_saved_loader *artifacts_timeline_saved_loader_obj = (artifacts_timeline_saved_loader *)current_widget;
        if(artifacts_timeline_saved_loader_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                artifacts_timeline_saved_loader_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                artifacts_timeline_saved_loader_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Tags))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Notes_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_Redefined_Result_Browser_History)
            || category_name == QString(MACRO_Redefined_Result_Messenger))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Screenshots))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Story_Board))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Ram_Analysis_Saved))
    {
        //not available
    }
    else if(category_name == QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs)
            || category_name == QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Saved_Maps))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Email_Files_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Carved_Files))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Carved_Password))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Carved_Data))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_RAM_Images))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Snapshots))
    {
        //not available
    }
    else
    {
        ///(unknown feature type)
    }

}

void MainWindow::handle_bookmark_from_detailed_info_exif_metadata_clicked(QString record_no_str, QString bookmark_status)
{
    Container_QWidget *widget = get_currenly_loaded_display_obj();
    if(widget == nullptr)
        return;

    QLayout  *main_layout = widget->layout();
    if(main_layout == nullptr)
        return;

    QVariant *current_widget = (QVariant *)main_layout->itemAt(0)->widget();
    if(current_widget == nullptr)
        return;

    QString current_category = widget->pub_get_category_name();

    QString category_name = widget->pub_get_category_name();
    QString widget_name   = widget->pub_get_feature_widget_name();


    if(category_name == QString(MACRO_CASE_TREE_Artifacts))
    {
        //for plugins only
        plugin_viewer *plugin_viewer_obj = (plugin_viewer *)current_widget;
        if(plugin_viewer_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                plugin_viewer_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                plugin_viewer_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }

    }
    else if(category_name == QString(MACRO_CASE_TREE_Mobile_Backup_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Disk_Images_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Artifacts_Keyword_Search))
    {
        artifacts_keyword_search_loader *keyword_search_loader_obj = (artifacts_keyword_search_loader *)current_widget;
        if(keyword_search_loader_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                keyword_search_loader_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                keyword_search_loader_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions)
            || category_name == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions)
            || category_name == QString(MACRO_CASE_TREE_MIME_Type)
            || category_name == QString(MACRO_CASE_TREE_File_Size_Parent)
            || category_name == QString(MACRO_CASE_TREE_Apple_Metadata_Parent)
            || category_name == QString(MACRO_CASE_TREE_File_Search)
            || category_name == QString(MACRO_CASE_TREE_Content_Search)
            || category_name == QString(MACRO_CASE_TREE_File_Signature_Parent)
            || category_name == QString(MACRO_CASE_TREE_Hashset)
            || (category_name == QString(MACRO_CASE_TREE_Redefined_Result_Parent) && widget_name.contains(QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations)))
            || category_name == QString(MACRO_CASE_TREE_Exif_Parent)
            || category_name == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
    {
        filetypes_loader *filetypes_obj = (filetypes_loader *)current_widget;
        if(filetypes_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                filetypes_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                filetypes_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Source))
    {
        file_system *file_system_obj = (file_system *)current_widget;
        if(file_system_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
            {
                file_system_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            }
            else
            {

                file_system_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
            }
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Artifacts_Timeline))
    {
        artifacts_timeline_saved_loader *artifacts_timeline_saved_loader_obj = (artifacts_timeline_saved_loader *)current_widget;
        if(artifacts_timeline_saved_loader_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                artifacts_timeline_saved_loader_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                artifacts_timeline_saved_loader_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Tags))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Notes_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_Redefined_Result_Browser_History)
            || category_name == QString(MACRO_Redefined_Result_Messenger))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Screenshots))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Story_Board))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Ram_Analysis_Saved))
    {
        //not available
    }
    else if(category_name == QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs)
            || category_name == QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Saved_Maps))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Email_Files_Parent))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Carved_Files))
    {
        carved_files_loader *carved_files_loader_obj = (carved_files_loader *)current_widget;
        if(carved_files_loader_obj != nullptr)
        {
            if(bookmark_status == QString("1"))
                carved_files_loader_obj->pub_bookmark_main_record_from_detailed_info_triggered(record_no_str);
            else
                carved_files_loader_obj->pub_remove_main_record_bookmark_from_detailed_info_triggered(record_no_str);
        }
    }
    else if(category_name == QString(MACRO_CASE_TREE_Carved_Password))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Carved_Data))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_RAM_Images))
    {
        //not available
    }
    else if(category_name == QString(MACRO_CASE_TREE_Snapshots))
    {
        //not available
    }
    else
    {
        ///(unknown feature type)
    }

}
