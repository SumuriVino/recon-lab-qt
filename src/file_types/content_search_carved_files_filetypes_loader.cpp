#include "filetypes_loader.h"

void filetypes_loader::prepare_indexed_carved_files_tab_display()
{
    return;

    QStringList table_header_title_list;
    table_header_title_list << "" << "" << "" << "Record No." << "File Name" << "File Size" << "Extension" << "Number of Hits" << "Keyword Hit"
                            << "Word Count" << "Hit Location" <<"File Path" << "Source Count Name" << "Source Name" << "Notes Value" << "";

    m_table_carved_index_files = new m_tablewidget(this);

    connect(m_table_carved_index_files, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_table_carved_index_files, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_export_carved_index_files_triggered()));
    connect(m_table_carved_index_files,SIGNAL(signal_spacebar_quicklook_pressed()),this,SIGNAL(signal_act_quick_look_triggered()));
    connect(m_table_carved_index_files, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_indexed_carved_files_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_table_carved_index_files->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));


    m_table_carved_index_files->horizontalHeader()->setStretchLastSection(true);
    m_table_carved_index_files->horizontalHeader()->setSectionsMovable(true);
    m_table_carved_index_files->horizontalHeader()->sortIndicatorOrder();
    m_table_carved_index_files->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_table_carved_index_files->setColumnCount(table_header_title_list.size());
    m_table_carved_index_files->setHorizontalHeaderLabels(table_header_title_list);

    m_table_carved_index_files->hideColumn(enum_indexed_carved_files_Word_Count);
    m_table_carved_index_files->hideColumn(enum_indexed_carved_files_Hit_Locations);
    m_table_carved_index_files->hideColumn(enum_indexed_carved_files_Source_Count_Name_Index);
    m_table_carved_index_files->hideColumn(enum_indexed_carved_files_Notes_Value_Index);

    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Bookmarks_Index,  35);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Notes_Icon_Index, 35);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Tag_Icon_Index,   35);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Record_No_Index,  90);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_File_Name_Index,  200);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_File_Size_Index,  100);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Extension_Index,  100);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Number_of_hits,   100);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Keyword_Hit,      100);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_File_Path_Index,  250);
    m_table_carved_index_files->setColumnWidth(enum_indexed_carved_files_Source_Name_Index,200);



    QTableWidgetItem *item_bookmark = new QTableWidgetItem;
    item_bookmark->setIcon(QIcon("../icons/signs/bookmark.png"));
    m_table_carved_index_files->setHorizontalHeaderItem(enum_indexed_carved_files_Bookmarks_Index, item_bookmark);


    menu_carved_index_files_search_filters  = new QMenu(this);
    for(int count = 0; count < m_table_carved_index_files->columnCount(); count++)
    {
        if(m_table_carved_index_files->isColumnHidden(count))
            continue;

        QString column_label = m_table_carved_index_files->horizontalHeaderItem(count)->text();
        if(column_label.isEmpty())
            continue;

        QAction *action_filters   =  new QAction(column_label , this);
        action_filters->setCheckable(true);

        if(table_header_title_list.at(count).trimmed() == QString("File Name"))
            action_filters->setChecked(true);

        menu_carved_index_files_search_filters->addAction(action_filters);
    }

    menu_carved_index_files_right_click = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    menu_carved_index_files_right_click->setFont(m_font);
    connect(menu_carved_index_files_right_click, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_carved_index_files_main_menu_triggered(QAction*)));


    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Bookmark) << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);

    // For custom right click
    create_right_click_actions(menu_carved_index_files_right_click , menuList);

    //Tags
    add_tags_module_carved_index_files_obj = new add_tags(this);
    connect(add_tags_module_carved_index_files_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_submenu_carved_index_files(bool,QString,QString)));
    connect(add_tags_module_carved_index_files_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags_carved_index_files(QString,QString)));

    sub_carved_index_files_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , menu_carved_index_files_right_click , sub_carved_index_files_menu_tags);
    create_tags_submenu_for_carved_index_files();

    connect(sub_carved_index_files_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_carved_index_files_clicked(QAction*)));
    connect(sub_carved_index_files_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu_for_carved_index_files_clicked()));
    connect(this, SIGNAL(signal_add_tags_carved_index_files(QString,QString)), this, SLOT(slot_add_tags_carved_index_files(QString,QString)));

    create_right_click_actions(menu_carved_index_files_right_click , QStringList(QString(""))); /// creating a separator


    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Add_Note) << QString(MACRO_Generic_Right_Click_Remove_Note);
    create_right_click_actions(menu_carved_index_files_right_click , menuList);
    create_right_click_actions(menu_carved_index_files_right_click , QStringList(QString(""))); /// creating a separator

    act_carved_index_files_quick_look = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Quick_look),menu_carved_index_files_right_click);


    //Open With
    QStringList submenuList;
    submenuList << QString(MACRO_CASE_TREE_Plist_Viewer)
                << QString(MACRO_CASE_TREE_Hex_Viewer)
                << QString(MACRO_CASE_TREE_Sqlite_Viewer);


    submenu_carved_index_files_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , menu_carved_index_files_right_click , submenu_carved_index_files_open_with);
    create_right_click_actions(submenu_carved_index_files_open_with , submenuList);

    // Send to Bucket
    submenu_carved_index_files_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , menu_carved_index_files_right_click , submenu_carved_index_files_send_to_bucket);
    create_right_click_actions(submenu_carved_index_files_send_to_bucket , submenuList);

    // Export
    act_carved_index_files_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),menu_carved_index_files_right_click);

    create_right_click_actions(menu_carved_index_files_right_click , QStringList(QString(""))); /// creating a separator


    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Open_With_External_Application)
             << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard) ;

    create_right_click_actions(menu_carved_index_files_right_click , menuList);

    submenu_carved_index_files_open_with_external_application = new QMenu(this);
    connect(submenu_carved_index_files_open_with_external_application,SIGNAL(triggered(QAction*)),this,SLOT(slot_sub_menu_carved_index_files_open_file_with_other_app_clicked(QAction*)));

    QList <QMenu*> open_with_other_app_menu_list;
    open_with_other_app_menu_list.append(submenu_carved_index_files_open_with_external_application);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(open_with_other_app_menu_list, Q_FUNC_INFO);

    add_applications_to_open_application_menu_for_carved_index_files();

    act_carved_index_files_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),menu_carved_index_files_right_click);

    connect(act_carved_index_files_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));
    connect(submenu_carved_index_files_open_with, SIGNAL(triggered(QAction*)), this, SLOT(slot_open_with_submenu_clicked(QAction*)));
    connect(submenu_carved_index_files_send_to_bucket, SIGNAL(triggered(QAction*)), this, SLOT(slot_sendtobucket_submenu_clicked(QAction*)));

}

void filetypes_loader::slot_right_click_carved_index_files_main_menu_triggered(QAction *action)
{ // right click actions on content search loader
    if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Bookmark))
    {
        action_carved_index_files_bookmark_clicked();
    }
    else if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_carved_index_files_remove_bookmark_clicked();
    }
    else if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Add_Note))
    {
        m_table_carved_index_files->blockSignals(true);
        action_add_note_triggered();
        m_table_carved_index_files->blockSignals(false);
    }
    else if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        action_carved_index_files_remove_note_clicked();
    }
    else if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Export))
    {
        action_export_carved_index_files_triggered();
    }
    else if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    {
        action_copy_to_clipboard_carved_index_files();
    }
    else if(action->text().trimmed() == QString(MACRO_Generic_Right_Click_Open_In_Window))
    {
        action_open_full_detailed_info_carved_index_files_triggered();
    }

}


void filetypes_loader::prepare_display_for_carved_files_content_search(QSqlDatabase &destination_db)
{ // display all the data on tablewidget after carved files on content search
    recon_static_functions::app_debug(" -Starts ", Q_FUNC_INFO);

    m_table_carved_index_files->setRowCount(0);

    QString cmd_select = QString("Select INT,bookmark,file_name,file_path,no_of_hits,keyword_name,total_word_count,hit_locations,index_path,recon_tag_value,notes,source_count_name from files Where plugin_name = ?");
    QSqlQuery query(destination_db);
    query.prepare(cmd_select);
    query.addBindValue(QString(MACRO_Plugin_Name_Carved_Files));
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + cmd_select,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return ;
    }

    // crash free check post
    if(total_record_count <= 0)
        total_record_count = 1;

    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    if(total_record_count > 1500)
    {
        disable_widgets(true);
        emit signal_disable_case_tree_sidebar(true);
    }

    QString recon_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    while(query.next())
    {
        total_covered_count++;
        if(total_covered_count % progressbar_increment_step == 0)
        {
            float record_percent = ((total_covered_count*100)/total_record_count);
            emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
        }

        if(total_covered_count % 100 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
        {
            break ;
        }

        loading_display_dialog_based_on_records(total_record_count , total_covered_count);

        QString INT_str  = query.value("INT").toString().trimmed();
        QString bookmark_str  = query.value("bookmark").toString().trimmed();
        QString file_name_str = query.value("file_name").toString().trimmed();
        QString file_path_str = query.value("file_path").toString().trimmed();
        QString no_of_hits_str   = query.value("no_of_hits").toString().trimmed();
        QString keyword_name_str = query.value("keyword_name").toString().trimmed();
        QString recon_tag_str = query.value("recon_tag_value").toString().trimmed();
        QString notes_str = query.value("notes").toString().trimmed();
        QString source_count_name_str = query.value("source_count_name").toString().trimmed();
        QString word_count_str   = query.value("total_word_count").toString().trimmed();
        QString hit_location_str = query.value("hit_locations").toString().trimmed();

        QString complete_file_path = recon_result_dir + file_path_str;

        QString filesize_str = recon_static_functions::mining_size(complete_file_path, Q_FUNC_INFO);

        int row = m_table_carved_index_files->rowCount();
        m_table_carved_index_files->insertRow(row);
        m_table_carved_index_files->setRowHeight(row, 22);

        QTableWidgetItem *item_bk        = new QTableWidgetItem;
        QTableWidgetItem *item_INT       = new QTableWidgetItem;
        QTableWidgetItem *item_tag_icon  = new QTableWidgetItem;
        QTableWidgetItem *item_filename  = new QTableWidgetItem;
        QTableWidgetItem *item_extension = new QTableWidgetItem;
        QTableWidgetItem *item_filepath  = new QTableWidgetItem;
        QTableWidgetItem *item_file_size = new QTableWidgetItem;
        QTableWidgetItem *item_source_count_name = new QTableWidgetItem;
        QTableWidgetItem *item_source_name  = new QTableWidgetItem;
        QTableWidgetItem *item_no_of_hits   = new QTableWidgetItem;
        QTableWidgetItem *item_keyword_hit  = new QTableWidgetItem;
        QTableWidgetItem *item_notes_icon   = new QTableWidgetItem;
        QTableWidgetItem *item_notes_value  = new QTableWidgetItem;
        QTableWidgetItem *item_word_count   = new QTableWidgetItem;
        QTableWidgetItem *item_hit_location = new QTableWidgetItem;

        QTableWidgetItem *item_blank       = new QTableWidgetItem;

        item_bk->setCheckState(Qt::Unchecked);
        if(bookmark_str == QString("1"))
            item_bk->setCheckState(Qt::Checked);

        item_INT->setData(Qt::EditRole , INT_str.toLongLong());
        item_INT->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);

        item_no_of_hits->setForeground(Qt::red);
        item_keyword_hit->setForeground(Qt::red);

        item_no_of_hits->setData(Qt::EditRole , no_of_hits_str.toDouble());
        item_no_of_hits->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_keyword_hit->setText(keyword_name_str);
        item_extension->setText(recon_helper_standard_obj->mining_suffix(file_path_str, Q_FUNC_INFO));

        if(!notes_str.isEmpty())
        {
            QIcon iconNotes("../icons/notes.png");
            item_notes_icon->setIcon(iconNotes);
        }
        else
        {
            QIcon iconNotes("");
            item_notes_icon->setIcon(iconNotes);
        }

        item_filename->setText(file_name_str);
        item_filepath->setText(file_path_str);

        item_file_size->setData(Qt::EditRole , filesize_str.toLongLong());
        item_file_size->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_source_count_name->setText(source_count_name_str);

        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
        item_source_name->setText(struct_source_info.complete_source_name);

        QString tags_icon_path;
        if(!recon_tag_str.isEmpty())
        {
            if(recon_tag_str.contains(","))
            {
                tags_icon_path = recon_static_functions::get_tags_icon_path("",recon_tag_str, Q_FUNC_INFO);
            }
            else
            {
                QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
                QString color_name = recon_helper_standard_obj->get_colour_name_of_tag_from_db(recon_tag_str,tag_db_path, Q_FUNC_INFO);
                tags_icon_path = recon_static_functions::get_tags_icon_path(color_name , recon_tag_str, Q_FUNC_INFO);
            }
        }
        QIcon icon(tags_icon_path);

        item_tag_icon->setIcon(icon);
        item_notes_value->setText(notes_str);
        item_blank->setText(QString(""));

        item_word_count->setText(word_count_str);
        item_hit_location->setText(hit_location_str);


        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Bookmarks_Index,item_bk);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Record_No_Index,item_INT);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_File_Name_Index,item_filename);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Extension_Index,item_extension);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_File_Path_Index,item_filepath);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_File_Size_Index,item_file_size);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Tag_Icon_Index,item_tag_icon);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Source_Count_Name_Index,item_source_count_name);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Source_Name_Index,item_source_name);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Number_of_hits,item_no_of_hits);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Keyword_Hit,item_keyword_hit);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Notes_Icon_Index,item_notes_icon);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Notes_Value_Index,item_notes_value);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Word_Count,item_word_count);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Hit_Locations,item_hit_location);
        m_table_carved_index_files->setItem(row , enum_indexed_carved_files_Blank_Column_Index,item_blank);


        //        if(item_filename->text().endsWith(".png", Qt::CaseInsensitive) || item_filename->text().endsWith(".jpg", Qt::CaseInsensitive)
        //                || item_filename->text().endsWith(".gif", Qt::CaseInsensitive) || item_filename->text().endsWith(".icns", Qt::CaseInsensitive)
        //                || item_filename->text().endsWith(".tif", Qt::CaseInsensitive) || item_filename->text().endsWith(".svg", Qt::CaseInsensitive)
        //                || item_filename->text().endsWith(".bmp", Qt::CaseInsensitive) || item_filename->text().endsWith(".ppm", Qt::CaseInsensitive)
        //                || item_filename->text().endsWith(".xbm", Qt::CaseInsensitive)|| item_filename->text().endsWith(".jpeg", Qt::CaseInsensitive)
        //                || item_filename->text().endsWith(".heic", Qt::CaseInsensitive))
        //        {
        //            struct_global_gallery_view_file_info f1;
        //            f1.file_name = item_filename->text();
        //            f1.file_path = item_filepath->text();
        //            f1.INT = item_INT->text();
        //            f1.source_count_name = item_source_count_name->text();
        //            f1.table_view_row_no = row;
        //            f1.latitude = QString("");
        //            f1.longitude = QString("");
        //            f1.full_file_path = complete_file_path;
        //            f1.bookmark = "0";
        //            f1.last_access_timestamp = QString("");
       // f1.index = st_gallery_view_file_info_list.size();

        //            if(item_bk->checkState() == Qt::Checked)
        //                f1.bookmark = "1";

        //            st_gallery_view_file_info_list.append(f1);
        //        }
    }

    disable_widgets(false);
    emit signal_disable_case_tree_sidebar(false);

    recon_static_functions::app_debug(" -Ends ", Q_FUNC_INFO);

}


void filetypes_loader::slot_tablewidget_indexed_carved_files_current_cell_changed(int cur_row, int cur_col, int pre_row, int pre_col)
{
    Q_UNUSED(pre_row);
    Q_UNUSED(pre_col);

    tablewidget_indexed_carved_files_general_clicked(cur_row,cur_col);
}

void filetypes_loader::tablewidget_indexed_carved_files_general_clicked(int row, int column)
{ // randomly clicked on any record of carved files data to get the index of data or their column data
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    QString record_no   = m_table_carved_index_files->item(row, enum_indexed_carved_files_Record_No_Index)->text();
    QString file_path   =  m_table_carved_index_files->item(row,enum_indexed_carved_files_File_Path_Index)->text().trimmed();
    QString source_count_name = m_table_carved_index_files->item(row, enum_indexed_carved_files_Source_Name_Index)->text();
    QString content_srch_data = m_table_carved_index_files->item(row,enum_indexed_carved_files_Keyword_Hit)->text().trimmed();


    QString db_name = file_type_child;
    db_name = db_name.replace(" ", "_");

    QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + db_name + ".sqlite";

    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_content_search(record_no,search_db_path,Q_FUNC_INFO);

    QString recon_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    current_media_path = recon_result_dir + file_path;
    preview_file_path  = recon_result_dir + file_path;
    metadata_str       = struct_metadata_values_obj.metadata;


    QString carver_db_path   = get_carver_db_path_from_file_path(file_path);
    QString record_no_carved_files  = get_carver_record_no_from_filepath(file_path);

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.db_file_path  = carver_db_path;
    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no_carved_files;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.content_searched_text = content_srch_data;
    st_set_meta_obj.plugin_name = QString(MACRO_Plugin_Name_Carved_Files);
    st_set_meta_obj.tab_name    = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


void filetypes_loader::slot_sub_menu_carved_index_files_open_file_with_other_app_clicked(QAction* current_clicked_action)
{ // right click action and open any other record with other system app

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

        QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
        QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        recon_static_functions::open_file_with_specified_app(complete_path , current_clicked_action->toolTip(), Q_FUNC_INFO);

        // show only first file (Multiselection not supported).
        break;

    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void filetypes_loader::add_applications_to_open_application_menu_for_carved_index_files()
{ // function not in use

    QString config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    QStringList application_list;
    QString cmd_str = QString("Select application_path From tbl_external_applications");
    application_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_str , 0, config_db_path, Q_FUNC_INFO);


    if(!application_list.isEmpty())
    {
        QFileIconProvider icon_provide;

        QList<QAction*> actions_list = menu_carved_index_files_right_click->actions();

        for(int count = 0; count < actions_list.size(); count++)
        {
            QAction *act_submenu =  actions_list.at(count);

            if(act_submenu->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
            {
                submenu_carved_index_files_open_with_external_application->clear();

                for(int num = 0; num < application_list.size(); num++)
                {
                    QAction *act_submenu_app =  new QAction(QFileInfo(application_list.at(num)).fileName().toLocal8Bit(), this);
                    act_submenu_app->setIcon(icon_provide.icon(QFileInfo(application_list.at(num))));
                    submenu_carved_index_files_open_with_external_application->addAction(act_submenu_app);
                    submenu_carved_index_files_open_with_external_application->setToolTip(application_list.at(num));
                }

                act_submenu->setMenu(submenu_carved_index_files_open_with_external_application);
                break;
            }

        }
    }
    else
    {
        submenu_open_with_external_application->clear();

        QList<QAction*> actions_list = mainMenu->actions();

        for(int count = 0; count < actions_list.size(); count++)
        {
            QAction *act_submenu =  actions_list.at(count);

            if(act_submenu->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
            {
                QList <QMenu*> open_with_other_app_menu_list;
                open_with_other_app_menu_list.append(submenu_open_with_external_application);
                recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(open_with_other_app_menu_list, Q_FUNC_INFO);
                act_submenu->setMenu(submenu_open_with_external_application);
                break;
            }
        }
    }

}

void filetypes_loader::action_carved_index_files_bookmark_clicked()
{ // add bookmark on carved data
    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_carved_files> carved_files_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Bookmarks_Index)->checkState() == Qt::Checked)
            continue;

        QString record_no_str = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Record_No_Index)->text();
        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString current_carver_label = get_carver_label_from_filepath(filepath_str);

        struct_global_tagging_carved_files s1;
        s1.INT_record  = get_carver_record_no_from_filepath(filepath_str);
        s1.plugin_name = QString(MACRO_Plugin_Name_Carved_Files);
        s1.tab_name    = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);
        s1.source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text();
        s1.item0 = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Name_Index)->text();
        s1.item1 = filepath_str;
        s1.item2 = record_no_str;
        s1.item3 = current_carver_label;

        carved_files_record_list << s1;
        m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Bookmarks_Index)->setCheckState(Qt::Checked);

        global_connection_manager_obj->pub_set_bookmark_in_carved_index_files_db(QStringList(record_no_str) ,file_type_child);
    }

    global_connection_manager_obj->pub_add_bookmark_for_carved_files_db(carved_files_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void filetypes_loader::action_carved_index_files_remove_bookmark_clicked()
{ // remove bookmark on carved data
    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

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

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Bookmarks_Index)->checkState() == Qt::Unchecked)
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        INT_List << m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Record_No_Index)->text();

        m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Bookmarks_Index)->setCheckState(Qt::Unchecked);


        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString current_carver_label = get_carver_label_from_filepath(filepath_str);

        QString carver_record_no = get_carver_record_no_from_filepath(filepath_str);
        global_connection_manager_obj->pub_delete_bookmark_for_carved_files_db(QStringList(carver_record_no),current_carver_label);

    }

    global_connection_manager_obj->pub_remove_bookmark_in_carved_index_files_db(INT_List,file_type_child);


    display_loading_progress_bar_non_cancelable_obj->hide();

}

void filetypes_loader::action_carved_index_files_add_note_clicked(QString note_text)
{ // add note clicked from right click action menu
    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_carved_files> carved_files_record_list;

    QStringList int_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int current_row = model_index.row();
        if(current_row < 0 && current_row >= m_table_carved_index_files->rowCount())
            continue;

        QString record_no_str = m_table_carved_index_files->item(current_row, enum_indexed_carved_files_Record_No_Index)->text();
        QString filepath_str  = m_table_carved_index_files->item(current_row, enum_indexed_carved_files_File_Path_Index)->text();
        QString current_carver_label = get_carver_label_from_filepath(filepath_str);

        struct_global_tagging_carved_files s1;
        s1.INT_record  = get_carver_record_no_from_filepath(filepath_str);
        s1.plugin_name = QString(MACRO_Plugin_Name_Carved_Files);
        s1.tab_name    = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);
        s1.source_count_name = m_table_carved_index_files->item(current_row, enum_indexed_carved_files_Source_Count_Name_Index)->text();
        s1.item0 = m_table_carved_index_files->item(current_row, enum_indexed_carved_files_File_Name_Index)->text();
        s1.item1 = filepath_str;
        s1.item2 = record_no_str;
        s1.item3 = current_carver_label;

        carved_files_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_table_carved_index_files->item(current_row, enum_indexed_carved_files_Notes_Icon_Index)->setIcon(iconNotes);

        int_list << record_no_str;
    }
    global_connection_manager_obj->pub_set_notes_in_carved_index_files_db(int_list, file_type_child,note_text);
    global_connection_manager_obj->pub_add_notes_for_carved_files_db(carved_files_record_list, note_text);

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void filetypes_loader::action_carved_index_files_remove_note_clicked()
{ // remove note clicked from right click action
    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

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
        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        INT_List << m_table_carved_index_files->item(selected_row_no, record_no_index)->text();

        QIcon iconNotes("");
        m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Notes_Icon_Index)->setIcon(iconNotes);


        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString current_carver_label = get_carver_label_from_filepath(filepath_str);
        QString carver_record_str = get_carver_record_no_from_filepath(filepath_str);

        global_connection_manager_obj->pub_delete_notes_for_carved_files_db(QStringList(carver_record_str),current_carver_label);
    }

    global_connection_manager_obj->pub_remove_notes_in_carved_index_files_db(INT_List,file_type_child);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void filetypes_loader::action_open_with_plist_viewer_carved_index_files_triggered()
{ // open the record with plist viewer for further analysis
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

    int selected_row_no = m_table_carved_index_files->currentRow();

    QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
    QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

    QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);

    QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    QString temp_p = complete_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not plist file.");
        message_dialog_obj->show();
        return;
    }
    else
    {

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString plist_display_nm = info.fileName() + QString(" [") + QString(MACRO_Plugin_Name_Carved_Files) + QString(" ") + record_no_carved_files_str + QString("]") + src_cnt_name_label;

        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(plist_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
            return;
        }

        plist_viewer *plist_viewer_obj;
        plist_viewer_obj = new plist_viewer(this);
        QString plist_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_Carved_Files) + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Carved_Files_TAB_Name_Files) + MACRO_RECON_Splitter_1_multiple + record_no_carved_files_str;

        plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
        plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
        plist_viewer_obj->pub_set_plist_path(complete_path);
        plist_viewer_obj->pub_set_source_count_name(source_count_name);
        plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        plist_viewer_obj->pub_set_essentials();

        plist_viewer_obj->pub_start_plist_extraction();

        emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_open_with_hex_viewer_carved_index_files_triggered()
{ // open the particular record with hex viewer for hex view analysis
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    int selected_row_no = m_table_carved_index_files->currentRow();

    QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
    QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

    QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
    QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }


    QString src_cnt_name_label = QString(" [" + source_count_name + "]");
    QString hex_display_nm = info.fileName() + QString(" [") + QString(MACRO_Plugin_Name_Carved_Files) + QString(" ") + record_no_carved_files_str + QString("]") + src_cnt_name_label;

    QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
    if(list_hex_display_name.contains(hex_display_nm))
    {
        emit signal_show_file_in_viewer_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer, source_count_name);
        return;
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
    hex_viewer_open_file_data_obj.record_no_str       = record_no_carved_files_str;
    hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
    hex_viewer_open_file_data_obj.plugin_name_str     = QString(MACRO_Plugin_Name_Carved_Files);
    hex_viewer_open_file_data_obj.plugin_tab_name_str = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);
    hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
    hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

    emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_open_with_sqlite_viewer_carved_index_files_triggered()
{ // open the particular record with sqlite viewer for sqlite viewer annalysis
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    int selected_row_no = m_table_carved_index_files->currentRow();

    QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
    QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

    QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
    QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();

        return;
    }

    if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not sqlite file.");
        message_dialog_obj->show();
        return;
    }
    else
    {

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString sqlite_display_nm = info.fileName() + QString(" [") + QString(MACRO_Plugin_Name_Carved_Files) + QString(" ") + record_no_carved_files_str + QString("]") + src_cnt_name_label;

        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(sqlite_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
            return;
        }

        QString sqlite_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_Carved_Files) + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Carved_Files_TAB_Name_Files) + MACRO_RECON_Splitter_1_multiple + record_no_carved_files_str;

        sqlite_viewer_starter *sqlite_viewer_starter_obj;
        sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
        sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
        sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
        sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
        sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
        sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
        sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        sqlite_viewer_starter_obj->pub_set_essentials();

        sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_sendToBucket_plist_viewer_carved_index_files_triggered()
{ // send bucket submenu on right click action and select plist viewer for further analysis
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    QStringList message_list;


    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

        QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
        QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        all_path_list << complete_path;

        for(int pos = 0 ; pos< all_path_list.size() ; pos++)
        {
            QString complete_path = all_path_list.at(pos);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            QString temp_p = complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
            {
                message_list << QString("'" +  info.fileName() + "' is not plist file.");
                continue;
            }
            else
            {

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString plist_display_nm = info.fileName() + QString(" [") + QString(MACRO_Plugin_Name_Carved_Files) + QString(" ") + record_no_carved_files_str + QString("]") + src_cnt_name_label;

                QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
                if(list_plist_display_name.contains(plist_display_nm))
                {
                    continue;
                }

                plist_viewer *plist_viewer_obj;
                plist_viewer_obj = new plist_viewer(this);
                QString plist_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_Carved_Files) + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Carved_Files_TAB_Name_Files) + MACRO_RECON_Splitter_1_multiple + record_no_carved_files_str;

                plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
                plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
                plist_viewer_obj->pub_set_plist_path(complete_path);
                plist_viewer_obj->pub_set_source_count_name(source_count_name);
                plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                plist_viewer_obj->pub_set_essentials();

                plist_viewer_obj->pub_start_plist_extraction();

                emit signal_viewer_extarction_completed_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer);
            }
        }
    }


    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_sendToBucket_hex_viewer_carved_index_files_triggered()
{ // send to bucket as a hex viewer for further analysis
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    QStringList message_list;


    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

        QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
        QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

        struct_global_recon_file_info struct_global_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
        QString complete_path = struct_global_recon_file_info_obj.complete_file_path;
        all_path_list << QString(complete_path + MACRO_RECON_Splitter_1_multiple + source_count_name);

        for(int j = 0 ; j < all_path_list.size() ; j++)
        {
            QString complete_path_str = all_path_list.at(j);

            QStringList tmp_list = complete_path_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(tmp_list.size() < 2)
                continue;

            QString complete_path = tmp_list.at(0);
            QString source_count_name = tmp_list.at(1);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>" + complete_path,Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;


            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString hex_display_nm = info.fileName() + QString(" [") + QString(MACRO_Plugin_Name_Carved_Files) + QString(" ") + record_no_carved_files_str + QString("]") + src_cnt_name_label;

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
            hex_viewer_open_file_data_obj.display_file_path   = struct_global_recon_file_info_obj.display_file_path;
            hex_viewer_open_file_data_obj.record_no_str       = record_no_carved_files_str;
            hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
            hex_viewer_open_file_data_obj.plugin_name_str     = QString(MACRO_Plugin_Name_Carved_Files);
            hex_viewer_open_file_data_obj.plugin_tab_name_str = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);
            hex_viewer_open_file_data_obj.recon_filefrom      = struct_global_recon_file_info_obj.recon_file_from;
            hex_viewer_open_file_data_obj.recon_file_infopath = struct_global_recon_file_info_obj.recon_file_info_path;

            recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

            emit signal_viewer_extarction_completed_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer);
        }
    }

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_sendToBucket_sqlite_viewer_carved_index_files_triggered()
{ // send to bucket from submenu and select sqlite viewer for sqlite analysis
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    QStringList message_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QStringList all_path_list;

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

        QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
        QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
        all_path_list << complete_path;

        for(int i = 0 ; i< all_path_list.size() ; i++)
        {
            QString full_path1 = all_path_list.at(i);

            QFileInfo info(full_path1);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            if(!recon_static_functions::check_is_it_sqlite_file_by_signature(full_path1, Q_FUNC_INFO))
            {
                message_list << QString("'" + info.fileName() + QString("' is not sqlite file."));
                continue;
            }
            else
            {

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString sqlite_display_nm = info.fileName() + QString(" [") + QString(MACRO_Plugin_Name_Carved_Files) + QString(" ") + record_no_carved_files_str + QString("]") + src_cnt_name_label;


                QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
                if(list_sqlite_display_name.contains(sqlite_display_nm))
                {
                    continue;
                }

                QString sqlite_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_Carved_Files) + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Carved_Files_TAB_Name_Files) + MACRO_RECON_Splitter_1_multiple + record_no_carved_files_str;

                sqlite_viewer_starter *sqlite_viewer_starter_obj;
                sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
                sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
                sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
                sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
                sqlite_viewer_starter_obj->pub_set_sqlite_path(full_path1);
                sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
                sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                sqlite_viewer_starter_obj->pub_set_essentials();

                sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

                emit signal_viewer_extarction_completed_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer);
            }
        }
    }

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_export_carved_index_files_triggered()
{ // export any particular record
    action_export_carved_index_files_triggered();
}

void filetypes_loader::action_export_carved_index_files_triggered()
{ // export any particlar record file action triggered
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString dest_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();

    QStringList list_paths_to_message_box;

    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;


        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

        QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
        QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
        QString complete_filepath = st_globl_recon_file_info_obj.complete_file_path;

        QStringList all_path_list;
        all_path_list << complete_filepath;


        for(int i = 0 ; i< all_path_list.size() ; i++)
        {
            QString complete_path = all_path_list.at(i);

            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path ,QFileInfo(complete_path).fileName(),Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;

        }
    }


    if(list_paths_to_message_box.size() > 0)
    {
        for(int i = 0 ; i < list_paths_to_message_box.size(); i++)
        {
            QProcess proc;

            QStringList args;
            args << "-R" << "0777" << list_paths_to_message_box.at(i);

            proc.execute("chmod", args);

            QCoreApplication::processEvents();
        }

        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_path);
        message_dialog_obj->show();
    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"selected records does not have any file to export");
        message_dialog_obj->show();
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void filetypes_loader::create_tags_submenu_for_carved_index_files()
{ // create tags from right click action to create our own color tag
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_carved_index_files_menu_tags == NULL)
        return;

    sub_carved_index_files_menu_tags->clear();

    ///add new tag
    act_create_tag_carved_index_files = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag_carved_index_files->setIcon(icon_create);
    sub_carved_index_files_menu_tags->addAction(act_create_tag_carved_index_files);

    disconnect(act_create_tag_carved_index_files, SIGNAL(triggered()), this, SLOT(slot_act_create_tags_carved_index_files()));
    connect(act_create_tag_carved_index_files, SIGNAL(triggered()), this, SLOT(slot_act_create_tags_carved_index_files()));


    ///remove tag
    act_remove_tag_carved_index_files = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag_carved_index_files->setIcon(icon_remove);
    sub_carved_index_files_menu_tags->addAction(act_remove_tag_carved_index_files);

    disconnect(act_remove_tag_carved_index_files, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags_carved_index_files()));
    connect(act_remove_tag_carved_index_files, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags_carved_index_files()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_carved_index_files_menu_tags->addAction(separator_main);

    add_tags_module_carved_index_files_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_carved_index_files_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                sub_carved_index_files_menu_tags->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";
            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            act_already_saved_tags_carved_index_files = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags_carved_index_files->setIcon(icon);
            sub_carved_index_files_menu_tags->addAction(act_already_saved_tags_carved_index_files);
        }
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);
}

void filetypes_loader::slot_act_create_tags_carved_index_files()
{
    add_tags_module_carved_index_files_obj->show();
}

void filetypes_loader::slot_act_remove_tags_carved_index_files()
{ // remove tags clicked from right click menu
    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList INT_carved_files_List;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        if(m_table_carved_index_files->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Record_No_Index)->text();

        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString current_carver_label = get_carver_label_from_filepath(filepath_str);


        QString carved_files_record = get_carver_record_no_from_filepath(filepath_str);


        QString icon_path =  "";
        QIcon icon(icon_path);
        m_table_carved_index_files->item(selected_row_no,enum_indexed_carved_files_Tag_Icon_Index)->setIcon(icon);

        global_connection_manager_obj->pub_remove_tag_in_carved_index_files_db(QStringList(record_no_str), file_type_child);
        global_connection_manager_obj->pub_delete_tag_for_carved_files(QStringList(carved_files_record), current_carver_label);
    }


    display_loading_progress_bar_non_cancelable_obj->hide();

}

void filetypes_loader::slot_update_tags_submenu_carved_index_files(bool status, QString new_tag, QString tag_colour)
{ // create our own color tags and  set it on submenu
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags_carved_index_files = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags_carved_index_files->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags_carved_index_files);

        emit signal_update_tag_submenu_of_plugins(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_add_tags_carved_index_files(QString tag_data , QString colour)
{ // update tags submenu and update in database accordingly we perform action using tags
    update_tags_value_carved_index_files(tag_data,colour);
}

void filetypes_loader::update_tags_value_carved_index_files(QString tag_data,QString colour)
{ // update tags value in database when we add/remove tag on any record
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_table_carved_index_files->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_carved_files> carved_files_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_table_carved_index_files->rowCount())
            continue;

        QString record_no_str = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Record_No_Index)->text();
        QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
        QString current_carver_label = get_carver_label_from_filepath(filepath_str);

        struct_global_tagging_carved_files s1;
        s1.INT_record  = get_carver_record_no_from_filepath(filepath_str);
        s1.plugin_name = QString(MACRO_Plugin_Name_Carved_Files);
        s1.tab_name    = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);
        s1.source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text();
        s1.item0 = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Name_Index)->text();
        s1.item1 = filepath_str;
        s1.item2 = record_no_str;
        s1.item3 = current_carver_label;

        carved_files_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_table_carved_index_files->item(selected_row_no,enum_indexed_carved_files_Tag_Icon_Index)->setIcon(icon);

        global_connection_manager_obj->pub_set_tags_in_carved_index_files_db(QStringList(record_no_str), file_type_child,tag_data);

    }

    global_connection_manager_obj->pub_add_tag_for_carved_files(carved_files_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_tags_submenu_carved_index_files_clicked(QAction* current_clicked_action)
{
    action_submenu_tag_carved_index_files_triggered(current_clicked_action->text());
}

void filetypes_loader::slot_abouttoshow_tags_in_submenu_for_carved_index_files_clicked()
{
    create_tags_submenu_for_carved_index_files();
}

void filetypes_loader::action_submenu_tag_carved_index_files_triggered(QString tag_name)
{ // display default color tags on submenu of right click tags action
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_module_carved_index_files_obj->tags_db_path,Q_FUNC_INFO);

    emit signal_add_tags_carved_index_files(tag_name,colour);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_copy_to_clipboard_carved_index_files()
{ // copy to clipboard feature on right click
    recon_static_functions::app_debug("--Starts---", Q_FUNC_INFO);

    recon_static_functions::copy_tablewidget_data_to_clipboard(m_table_carved_index_files, Q_FUNC_INFO);

    recon_static_functions::app_debug("--Ends---", Q_FUNC_INFO);
}

void filetypes_loader::action_open_full_detailed_info_carved_index_files_triggered()
{ // open full detail info metadata of any record to get the more info about it
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int selected_row_no = m_table_carved_index_files->currentRow();

    if(selected_row_no < 0)
        return;

    QString searched_keyword = searched_keyword_str;

    QString filepath_str  = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_File_Path_Index)->text();
    QString record_no_carved_files_str = get_carver_record_no_from_filepath(filepath_str);

    QString carver_db_path = get_carver_db_path_from_file_path(filepath_str);
    QString source_count_name = m_table_carved_index_files->item(selected_row_no, enum_indexed_carved_files_Source_Count_Name_Index)->text().trimmed();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_carved_files_str ,carver_db_path,source_count_name);
    QString complete_filepath = st_globl_recon_file_info_obj.complete_file_path;

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    int preview_index_full = 1;
    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    preview_index_full = get_stack_index_for_preview(complete_filepath ,display_file_path);
    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"",record_no_carved_files_str,QString(MACRO_Plugin_Name_Carved_Files),"");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void filetypes_loader::action_pushbutton_search_carved_index_files_clicked()
{ // search any file from display data loader
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QList<QAction *> menu_actions_list = menu_carved_index_files_search_filters->actions();
    if(menu_actions_list.isEmpty())
        return;

    QString keyword = lineEdit_search->text().trimmed();
    if(keyword.isEmpty())
        return;

    QStringList search_string_list = keyword.split(",", Qt::SkipEmptyParts);


    QList<int> columns_list_to_search;
    QStringList column_name_list_to_search;

    for(int i = 0; i < menu_actions_list.size(); i++)
    {
        if(menu_actions_list.at(i)->isChecked())
        {
            column_name_list_to_search.append(menu_actions_list.at(i)->text());
        }
    }

    for(int i_col = 0; i_col < column_name_list_to_search.size(); i_col++)
    {
        for(int m_col_count = 0; m_table_carved_index_files->columnCount(); m_col_count++)
        {
            if(m_table_carved_index_files->horizontalHeaderItem(m_col_count)->text() == column_name_list_to_search.at(i_col))
            {
                columns_list_to_search.append(m_col_count);
                break;
            }
        }
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    for(int row = 0; row < m_table_carved_index_files->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        bool bool_search_item_matched = false;

        if(row % 50 == 0)
            QCoreApplication::processEvents();

        for(int col_count = 0 ; col_count < columns_list_to_search.size(); col_count++)
        {
            if(bool_cancel_loading)
                break;

            QString column_data = m_table_carved_index_files->item(row, columns_list_to_search.at(col_count))->text().trimmed();
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
            m_table_carved_index_files->hideRow(row);
    }

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_pushbutton_show_all_carved_index_files_clicked()
{ // show all/ refresh button clicked for by default filter selection and display the data on tablewidget
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString db_name = file_type_child;
    db_name = db_name.replace(" ", "_");
    QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + db_name + ".sqlite";
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(search_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    prepare_display_for_carved_files_content_search(destination_db);

    destination_db.close();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

QString filetypes_loader::get_carver_label_from_filepath(QString file_path)
{ // get carver laber which we give name while searching in line edit
    QStringList filepath_split_list  = file_path.split("/");
    QString carved_files_label ;

    if(filepath_split_list.size() > 4)
    {
        //input:   /Lab_Features/Carved_Files/Source1/BUILTIN_06-06-2019-21-00-22/sqlite/0_100/2287460948_builtin_carver.sqlite
        //output:  BUILTIN_06-06-2019-21-00-22
        carved_files_label =  filepath_split_list.at(4);
    }

    return carved_files_label;
}

QString filetypes_loader::get_carver_record_no_from_filepath(QString file_path)
{ // get carver record number from file path
    QString carver_db_path = get_carver_db_path_from_file_path(file_path);

    QString command = QString("Select INT from files Where item1 = ?");
    QString record_no_carved_files  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(file_path), 0, carver_db_path, Q_FUNC_INFO);

    return record_no_carved_files;
}

QString filetypes_loader::get_carver_db_path_from_file_path(QString file_path)
{ // get the complete database path of carver from filepath
    QStringList filepath_split_list  = file_path.split("/");
    QString carved_files_db_dir_path ;

    if(filepath_split_list.size() > 4)
    {
        //input=:   /Lab_Features/Carved_Files/Source1/BUILTIN_06-06-2019-21-00-22/sqlite/0_100/2287460948_builtin_carver.sqlite
        //output:  /Lab_Features/Carved_Files/Source1/BUILTIN_06-06-2019-21-00-22
        carved_files_db_dir_path = filepath_split_list.at(0) + "/" + filepath_split_list.at(1) + "/" + filepath_split_list.at(2) + "/"
                + filepath_split_list.at(3) + "/" + filepath_split_list.at(4);
    }


    QString carver_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + carved_files_db_dir_path + QString("/carver_files.sqlite");

    return carver_db_path;
}

bool filetypes_loader::check_is_carved_files_content_search_record(QString record_str)
{ // checking if if carve file has content search record
    bool bool_is_carved_files_record = false;

    if(file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
    {
        QString db_name = file_type_child;
        db_name = db_name.replace(" ", "_");

        QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + db_name + ".sqlite";

        QString command = QString("Select plugin_name from files where INT = ?");
        QString plugin_name_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(record_str), 0, search_db_path, Q_FUNC_INFO);

        if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
            bool_is_carved_files_record = true;
    }

    return bool_is_carved_files_record;
}

