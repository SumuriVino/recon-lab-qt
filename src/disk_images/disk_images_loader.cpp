#include "disk_images_loader.h"

disk_images_loader::disk_images_loader(QWidget *parent) : table_result_loader(parent)
{
    source_evidencer_obj = new source_evidencer(this);
    bookmarked_records_from_table_display_list.clear();

}

disk_images_loader::~disk_images_loader()
{

}

void disk_images_loader::pub_populate_data_in_table()
{ // populate data in disk images loader where we show the disk images(dmg,sparse etc) if found in current source
    destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";

    bookmark_col_index = 0;
    notes_col_index    = 1;
    tag_col_index      = 2;

    record_no_index   = 3;
    plugin_name_index = 4;
    tab_name_index    = 5;
    src_type_dsply_index = 6;
    file_name_index = 7;

    file_size_index  = 9;
    file_path_index   = 10;
    extraction_status_index = 11;
    source_nested_index = 12;
    source_decompressed_index = 13;
    root_count_name_index   = 14;
    source_count_name_index = 15;


    m_tablewidget_obj->hideColumn(bookmark_col_index);

    ///Done intentionally, for a blank column at start
    //m_tablewidget_obj->hideColumn(notes_col_index);
    m_tablewidget_obj->hideColumn(tag_col_index);


    m_tablewidget_obj->hideColumn(plugin_name_index);
    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(src_type_dsply_index);
    m_tablewidget_obj->hideColumn(extraction_status_index);
    m_tablewidget_obj->hideColumn(root_count_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(source_nested_index);
    m_tablewidget_obj->hideColumn(source_decompressed_index);

    pub_refresh_horizontal_header();
    display_data(destination_db_path);

    //    QStringList menuList;
    //    menuList << MACRO_Generic_Right_Click_Add_As_Source
    //             << ""
    //             << QString(MACRO_Generic_Right_Click_Bookmark)
    //             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks)
    //             << QString(MACRO_Generic_Right_Click_Add_Note)
    //             << QString(MACRO_Generic_Right_Click_Remove_Note);

    QStringList menuList;
    menuList << MACRO_Generic_Right_Click_Add_As_Source;

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {
        create_right_click_actions(mainMenu , menuList);
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator
    }

}

int  disk_images_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{ // populate disk images data in tablewidget
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(display_header_name_list.isEmpty())
    {
        recon_static_functions::app_debug("display_header_name_list.isEmpty() ----FAILED---- " , Q_FUNC_INFO);
        return -1;
    }

    bool_display_table_data_loaded = true;

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_index.executedQuery(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        return -1;
    }

    m_tablewidget_obj->setRowCount(0);

    total_covered_count = 0;
    total_record_count  = 0;
    total_record_count  = recon_helper_standard_obj->get_db_total_record_by_dbreference(true, received_db_obj, QStringList(), Q_FUNC_INFO);
    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int count = m_tablewidget_obj->rowCount();

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);

    while(query_index.next())
    {
        if(count % 10 == 0)
        {
            QCoreApplication::processEvents();
        }

        total_covered_count++;

        loading_display_dialog_based_on_records(total_record_count , total_covered_count ,feature_name);
        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break ;
        }

        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            if(pos == record_no_index)
            {
                QTableWidgetItem *item_tablewidget_record_no;

                item_tablewidget_record_no = new QTableWidgetItem;
                item_tablewidget_record_no->setData(Qt::EditRole, dataStr_from_db.toLongLong());

                item_tablewidget_record_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_record_no);

            }
            else if(pos == bookmark_position)
            {
                QTableWidgetItem *item_tablewidget_bookmark;

                item_tablewidget_bookmark = new QTableWidgetItem;
                item_tablewidget_bookmark->setCheckState(Qt::Unchecked);


                if(dataStr_from_db == QString::number(1))
                    item_tablewidget_bookmark->setCheckState(Qt::Checked);
                else
                    item_tablewidget_bookmark->setCheckState(Qt::Unchecked);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_bookmark);

            }
            else if(pos == notes_icon_index)
            {
                QTableWidgetItem *item_tablewidget_notes_icon;

                item_tablewidget_notes_icon = new QTableWidgetItem;

                if(query_index.value(notes_value_index).toString().trimmed().isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_notes_icon->setIcon(icon);
                }
                else
                {
                    QIcon iconNotes("../icons/notes.png");
                    item_tablewidget_notes_icon->setIcon(iconNotes);
                }
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_notes_icon);

            }
            else if(pos == (tag_position))
            {
                ///for tags

                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;

                if(dataStr_from_db.isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_tags->setIcon(icon);
                }
                else
                {
                    QString icon_str = dataStr_from_db;

                    if(icon_str.contains(","))
                    {
                        icon_str = "multiple";
                        QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                        item_tablewidget_tags->setIcon(icon);
                    }
                    else
                    {
                        icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        icon_str = icon_str.toLower();
                        icon_str = icon_str.replace(" ","_");
                        QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                        item_tablewidget_tags->setIcon(icon);
                    }
                }

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_tags);
            }
            else if(pos == blank_column_index)
            {
                QTableWidgetItem *item_tablewidget_blank = new QTableWidgetItem;
                item_tablewidget_blank->setText("");
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_blank);
            }
            else if(pos == notes_value_index)
            {
                //- No entry in the table widget item
            }
            else if(pos == file_size_index)
            {
                QTableWidgetItem *item_tablewidget_file_size;

                item_tablewidget_file_size = new QTableWidgetItem;
                item_tablewidget_file_size->setData(Qt::EditRole, dataStr_from_db.toLongLong());

                item_tablewidget_file_size->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_file_size);

            }
            else
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int);
            }

            pos ++;
        }
        count ++;
    }


    bookmarked_records_from_table_display_list = recon_helper_standard_obj->pub_get_records_bookmarked_from_table_display_by_db_refrence(received_db_obj, "tbl_sources_list", "INT", Q_FUNC_INFO);

    m_tablewidget_obj->hideColumn(notes_value_index);
    bool_display_table_data_loaded = false;

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void disk_images_loader::contextMenuEvent(QContextMenuEvent *evt)
{ // right click menu creation
    if(evt == NULL)
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;



    foreach (QAction *action, mainMenu->actions())
    {
        if(action->text() == QString(MACRO_Generic_Right_Click_Add_As_Source))
        {
            // action should disable if source path not exist or multi selection

            if(selection_model_list.size() > 1)
            {
                action->setEnabled(false);
                break;
            }

            QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
            QString disk_images_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
            QString source_cnt_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
            QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,disk_images_db_path,"tbl_sources_list",source_cnt_name,Q_FUNC_INFO);

            if(!QFileInfo(cmplt_file_path).exists())
                action->setEnabled(false);
            else
                action->setEnabled(true);

            break;
        }
    }

    mainMenu->exec(QCursor::pos());

}

void disk_images_loader::tablewidget_general_clicked(int row, int column)
{ // generally clicked on any record of tablewidget
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return ;

    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    if(record_no_str.trimmed().isEmpty())
        return;

    QString prnt_source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();
    QString filepath          = m_tablewidget_obj->item(row, file_path_index)->text();

    QString mtable = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, first_tab_name);
    struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(mtable, 0, record_no_str, plugin_name, destination_db_path, prnt_source_count_name, Q_FUNC_INFO);

    filepath = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,destination_db_path,mtable,prnt_source_count_name,Q_FUNC_INFO);

    current_media_path = filepath;
    preview_file_path  = filepath;
    metadata_str       = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name = "";
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = QString(MACRO_Plugin_Name_Disk_Images);
    st_set_meta_obj.tab_name = "";

    emit signal_set_metadata_info(st_set_meta_obj);


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void disk_images_loader::action_extract_source_triggered()
{ // action extract source on right click of disk images. Add that image as a source
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();

    if(current_row < 0 || current_row >= m_tablewidget_obj->rowCount()
            || current_column < 0 || current_column >= m_tablewidget_obj->columnCount())
        return;


    if(recon_case_obj->pub_is_file_system_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not add new source until previous source(s) have been added to case and apple timestamps have been extracted in case of Apple Filesystems.");
        return;
    }

    if(recon_case_obj->pub_get_mime_type_analysis_run_status())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not add new source until previous source(s) have been added.");
        return;
    }

    QString running_stat = "2";
    if(check_running_status == running_stat)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not add new source until previous source(s) have been added to case and apple timestamps have been extracted in case of Apple Filesystems.");
        return;
    }

    QString extraction_status =  m_tablewidget_obj->item(current_row, extraction_status_index)->text().trimmed();
    if(extraction_status == MACRO_TRUE_VALUE_NUMERIC_QString)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Source has been already extracted.");
        return;
    }

    m_tablewidget_obj->item(current_row, extraction_status_index)->setText(running_stat);

    QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
    QString source_cnt_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
    QString filepath = m_tablewidget_obj->item(current_row, file_path_index)->text();
    QString file_name = m_tablewidget_obj->item(current_row,file_name_index)->text();

    struct_GLOBAL_witness_info_source tgt_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);


    QString root_cnt_name = tgt_obj.root_count_name;
    QString root_type = tgt_obj.root_type;
    QString root_src_name = tgt_obj.root_name;
    QString root_path = tgt_obj.root_path_1;
    QString src_hierarchy_str = tgt_obj.source_hierarchy + (MACRO_RECON_Splitter_4_forward) + source_cnt_name;

    bool is_nested = false;
    if(m_tablewidget_obj->item(current_row, source_nested_index)->text() == MACRO_TRUE_VALUE_NUMERIC_QString)
        is_nested = true;

    bool is_decomprs = false;
    if(m_tablewidget_obj->item(current_row, source_decompressed_index)->text() == MACRO_TRUE_VALUE_NUMERIC_QString)
        is_decomprs = true;


    QString display_file_path = "/.../" + file_name;
    source_evidencer_obj->set_source_message(tgt_obj.complete_source_name + display_file_path);

    QString evidence_no = tgt_obj.evidence_number;
    QString description = tgt_obj.description;
    source_evidencer_obj->pub_set_evidence_no_and_description(evidence_no,description);
    source_evidencer_obj->pub_disable_evidence_no_and_description_widgets(true);

    source_evidencer_obj->pub_hide_cancel_button(false);
    source_evidencer_obj->exec();
    bool check_evidence_window_status = source_evidencer_obj->pub_check_evidence_opertion_status();
    if(!check_evidence_window_status)
    {
        return;
    }

    QString disk_images_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,disk_images_db_path,"tbl_sources_list",source_cnt_name,Q_FUNC_INFO);

    /*Note: Source type is static for now.
     * It needs to be generic.
     */

    check_running_status  =  m_tablewidget_obj->item(current_row, extraction_status_index)->text().trimmed();

    QString src_type_dsply = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Display);
    QString src_type_intrnal = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal);

    struct_global_source_evidencer mobj = source_evidencer_obj->get_evicencer_info();
    struct_GLOBAL_witness_info_source struct_source_info;
    recon_static_functions::clear_variables_witness_info_source_structure(struct_source_info);

    struct_source_info.root_count_name = root_cnt_name;
    struct_source_info.root_type = root_type;
    struct_source_info.root_name = root_src_name;
    struct_source_info.root_path_1 = root_path;

    struct_source_info.source_type_display = src_type_dsply;
    struct_source_info.source_type_internal = src_type_intrnal;
    struct_source_info.source_path_1 = cmplt_file_path;
    struct_source_info.source_path_2_for_fusion = "";
    struct_source_info.source_main_path = filepath;
    struct_source_info.source_main_name = QFileInfo(filepath).fileName();

    struct_source_info.root_recovery_passcode_plain = tgt_obj.root_recovery_passcode_plain;
    struct_source_info.source_evidence_password_plain = mobj.source_password_by_examiner;
    struct_source_info.source_evidence_password_encrypted = recon_static_functions::encrypt_string(mobj.source_password_by_examiner,Q_FUNC_INFO);

    struct_source_info.evidence_number = mobj.evidence_by_examiner;
    struct_source_info.description = mobj.description_by_examiner;
    struct_source_info.bool_source_nested = is_nested;
    struct_source_info.bool_source_decompressed = is_decomprs;
    struct_source_info.source_hierarchy = src_hierarchy_str;

    emit signal_add_source_for_extraction(struct_source_info);

    bool bool_processed_ok = false;
    int mcount = 0;
    while(1)
    {
        mcount++;
        if(mcount % 50 == 0)
        {
            QCoreApplication::processEvents();
            mcount = 0;
        }

        if(!recon_case_obj->pub_is_file_system_thread_running())
        {
            QString cmd = "update tbl_sources_list set extraction_status_as_source = 1 where INT = ?";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, QStringList(record_no), destination_db_path, Q_FUNC_INFO);

            m_tablewidget_obj->item(current_row, extraction_status_index)->setText("1");
            check_running_status = m_tablewidget_obj->item(current_row, extraction_status_index)->text();
            bool_processed_ok = true;
            break;
        }

    }

    if(!bool_processed_ok)
    {
        m_tablewidget_obj->item(current_row, extraction_status_index)->setText("0");
    }


    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

}

void disk_images_loader::action_bookmark_triggered()
{ // action add bookmark clicked
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << record;
        bookmarked_records_from_table_display_list << record;

        struct_global_tagging_disk_images s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        s1.item0 = m_tablewidget_obj->item(selected_row_no, 7)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

        rtrvd_srcs_record_list << s1;
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    }


    global_connection_manager_obj->pub_set_bookmark_in_disk_images_db(INT_List);
    global_connection_manager_obj->pub_add_bookmark_for_disk_images_db(rtrvd_srcs_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void disk_images_loader::action_remove_bookmark_triggered()
{ // action remove bookmark clicked
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        INT_List << record;

        qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(record);
        if(tmp_index >= 0)
            bookmarked_records_from_table_display_list.removeAt(tmp_index);

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_disk_images_db(INT_List);
    global_connection_manager_obj->pub_delete_bookmark_for_disk_images_db(INT_List);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void disk_images_loader::action_remove_note_triggered()
{ // action remove note clicked
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int notes_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    if(notes_column_index == -1)
        return;

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

        QIcon iconNotes("");
        m_tablewidget_obj->item(selected_row_no, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_remove_notes_in_disk_images_db(INT_List);
    global_connection_manager_obj->pub_delete_notes_for_disk_images_db(INT_List);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void disk_images_loader::update_bookmark_value(QString status,int row, int column)
{ // update bookmark value in database according to status
    if(column != 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
        return;


    QStringList INT_List;

    QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list;

    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;

    QString record_str = m_tablewidget_obj->item(row, record_no_index)->text();
    INT_List << record_str;

    struct_global_tagging_disk_images s1;
    s1.INT_record = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(row, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(row, tab_name_index)->text();
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();

    s1.item0 = m_tablewidget_obj->item(row, 7)->text();
    s1.item1 = m_tablewidget_obj->item(row, 8)->text();
    s1.item2 = m_tablewidget_obj->item(row, file_path_index)->text();

    rtrvd_srcs_record_list << s1;

    if(status == "1")
    {
        bookmarked_records_from_table_display_list << record_str;
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_disk_images_db(INT_List);
        global_connection_manager_obj->pub_add_bookmark_for_disk_images_db(rtrvd_srcs_record_list);
    }
    else
    {
        qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(record_str);
        if(tmp_index >= 0)
            bookmarked_records_from_table_display_list.removeAt(tmp_index);

        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_disk_images_db(INT_List);
        global_connection_manager_obj->pub_delete_bookmark_for_disk_images_db(INT_List);
    }

}

void disk_images_loader::update_tags_value(QString tag_data, QString colour)
{ // update tags value according to status
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int tag_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    if(tag_column_index == -1)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list;

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

        struct_global_tagging_disk_images s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        s1.item0 = m_tablewidget_obj->item(selected_row_no, 7)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

        rtrvd_srcs_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_column_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_set_tag_in_disk_images_db(INT_List,tag_data);
    global_connection_manager_obj->pub_add_tag_for_disk_images(rtrvd_srcs_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void disk_images_loader::action_open_full_detailed_info_triggered()
{ // action open in full detail info metadata clicked
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString searched_keyword = searched_keyword_str;
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    connect(full_detail_info_obj, SIGNAL(signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString, QString)), this, SLOT(slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString, QString)));


    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", 0,searched_keyword,"",record_no_str, "","");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void disk_images_loader::pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str)
{ // action bookmark from main data (metaedata) clicked
    if(bookmarked_records_from_table_display_list.contains(record_no_str))
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString tmp_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        if(tmp_record != record_no_str)
            continue;

        INT_List << tmp_record;

        struct_global_tagging_disk_images s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

        s1.item0 = m_tablewidget_obj->item(selected_row_no, 7)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

        rtrvd_srcs_record_list << s1;
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);

        m_tablewidget_obj->selectRow(selected_row_no);

    }


    global_connection_manager_obj->pub_set_bookmark_in_disk_images_db(INT_List);
    global_connection_manager_obj->pub_add_bookmark_for_disk_images_db(rtrvd_srcs_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void disk_images_loader::pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str)
{ // remove bookmark from main data(metadata) clicked
    if(bookmarked_records_from_table_display_list.contains(record_no_str))
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString tmp_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        if(tmp_record != record_no_str)
            continue;

        INT_List << tmp_record;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

        m_tablewidget_obj->selectRow(selected_row_no);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_disk_images_db(INT_List);
    global_connection_manager_obj->pub_delete_bookmark_for_disk_images_db(INT_List);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void disk_images_loader::slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status)
{ // handle bookmark on apple metadata, exif etc
    if(bookmark_status == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        pub_remove_main_record_bookmark_from_detailed_info_triggered(record_str);
    }
    else if(bookmark_status == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
    {
        pub_bookmark_main_record_from_detailed_info_triggered(record_str);
    }
}
