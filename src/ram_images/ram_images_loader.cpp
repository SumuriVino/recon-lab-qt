#include "ram_images_loader.h"

ram_images_loader::ram_images_loader(QWidget *parent) : table_result_loader(parent)
{
    plugin_name = QString(MACRO_Plugin_Name_RAM_Images);
    csv_tab_name = QString(MACRO_Plugin_RAM_Images_TAB_Name_Images);
}

ram_images_loader::~ram_images_loader()
{

}

void ram_images_loader::populate_data_in_table()
{

    bookmark_index = 0;
    notes_icon_index = 1;
    recon_tag_icon_index = 2;
    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index = 5;

    source_type_display_index = 6;
    source_name_index = 7;
    source_path_index = 8;
    source_size_index = 9;
    extraction_status_index = 10;

    source_nested_index = 11;
    source_decompressed_status_index = 12;
    recon_file_from_index = 13;
    recon_file_path_index = 14;
    root_count_name_index = 15;

    source_count_name_index = 16;
    notes_text_index = 17;
    blank_column_index = 18;




    m_tablewidget_obj->hideColumn(plugin_name_index);
    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(source_type_display_index);
    m_tablewidget_obj->hideColumn(bookmark_index);
    m_tablewidget_obj->hideColumn(recon_tag_icon_index);
    m_tablewidget_obj->hideColumn(notes_icon_index);
    m_tablewidget_obj->hideColumn(notes_text_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(root_count_name_index);
    m_tablewidget_obj->hideColumn(extraction_status_index);
    m_tablewidget_obj->hideColumn(source_decompressed_status_index);
    m_tablewidget_obj->hideColumn(source_nested_index);
    m_tablewidget_obj->hideColumn(extraction_status_index);
    m_tablewidget_obj->hideColumn(recon_file_from_index);
    m_tablewidget_obj->hideColumn(recon_file_path_index);




    display_data(destination_db_path);


    mainMenu->clear();
    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Carve_Data)
             << QString(MACRO_Generic_Right_Click_Carve_Files);

    submenu_carve = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Carve),mainMenu,submenu_carve);
    create_right_click_actions(submenu_carve , menuList);
}

void ram_images_loader::pub_set_ram_images_display_db_path(QString dest_db_path)
{
    destination_db_path = dest_db_path;
}


int ram_images_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
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

    while(m_tablewidget_obj->rowCount())
        m_tablewidget_obj->removeRow(0);

    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbreference(true, received_db_obj, QStringList(), Q_FUNC_INFO);
    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int count = m_tablewidget_obj->rowCount();

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);

    while(query_index.next())
    {
        if(count % 100 == 0)
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

            /// for bookmarks checkbox
            if(pos == record_no_position)
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
            else if(pos == source_size_index)
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

    m_tablewidget_obj->hideColumn(notes_value_index);
    bool_display_table_data_loaded = false;

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void ram_images_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    if(m_tablewidget_obj->currentRow() < 0 || m_tablewidget_obj->currentColumn() < 0)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;



    foreach (QAction *action, mainMenu->actions())
    {
        if(action->text() == QString(MACRO_Generic_Right_Click_Carve_Data)
                || (action->text() == QString(MACRO_Generic_Right_Click_Carve_Files)))
        {
            // action should disable if source path not exist or multi selection
            if(selection_model_list.size() > 1)
            {
                action->setEnabled(false);
                continue;
            }

            QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
            QString source_cnt_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
            QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,destination_db_path,db_table_name,source_cnt_name,Q_FUNC_INFO);

            if(!QFileInfo(cmplt_file_path).exists())
                action->setEnabled(false);
            else
                action->setEnabled(true);

        }
    }


    mainMenu->exec(QCursor::pos());

}

void ram_images_loader::tablewidget_general_clicked(int row, int column)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return ;

    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    if(record_no_str.trimmed().isEmpty())
        return;

    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();

    db_table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, csv_tab_name);
    struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(db_table_name, 0, record_no_str, plugin_name, destination_db_path, source_count_name, Q_FUNC_INFO);
    metadata_str = struct_metadata_values_obj.metadata;


    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name  = source_count_name;
    st_set_meta_obj.db_file_path = destination_db_path;
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = plugin_name;
    st_set_meta_obj.tab_name = csv_tab_name;


    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void ram_images_loader::action_extract_carved_data_triggered()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);


    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),record_no_index)->text();
    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),source_count_name_index)->text();

    QString complete_path =  recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,destination_db_path,db_table_name,source_count_name ,Q_FUNC_INFO);
    if(complete_path.trimmed().isEmpty() || !QFileInfo(complete_path).exists())
        return ;


    emit signal_carve_data_clicked(source_count_name, complete_path, plugin_name, csv_tab_name, record_no);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void ram_images_loader::action_extract_carved_files_triggered()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);


    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),record_no_index)->text();
    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),source_count_name_index)->text();

    QString complete_path =  recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,destination_db_path,db_table_name,source_count_name ,Q_FUNC_INFO);
    if(complete_path.trimmed().isEmpty() || !QFileInfo(complete_path).exists())
        return ;


    emit signal_carve_files_clicked(source_count_name, complete_path, false);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}



void ram_images_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString searched_keyword = searched_keyword_str;
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));

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
