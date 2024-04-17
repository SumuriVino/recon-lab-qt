#include "carved_files_loader.h"

carved_files_loader::carved_files_loader(QWidget *parent) : table_result_loader(parent)
{
    setWindowFlags(Qt::Window);

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    plugin_name = QString(MACRO_Plugin_Name_Carved_Files);
    tab_name_str = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);


    gallery_viewer_obj = new gallery_viewer(this);
    gallery_viewer_obj->pub_create_ui();

    recon_helper_process_obj = new recon_helper_process;

    connect(gallery_viewer_obj, SIGNAL(signal_gallery_viewer_item_double_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_viewer_item_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_clicked(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_invalid_item_clicked()), this, SLOT(slot_gallery_view_invalid_tem_clicked()));

    connect(this, SIGNAL(signal_set_bookmark_status_to_gallery_view(bool)), gallery_viewer_obj, SLOT(slot_set_bookmark_status_to_gallery_view(bool)));

    bookmarked_records_from_table_display_list.clear();

}


void carved_files_loader::pub_set_carved_files_display_db_path(QString db_path)
{
    destination_db_path = db_path;
}

void carved_files_loader::populate_data_in_table()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    bookmark_value_index  = 0;
    notes_icon_index  = 1;
    tag_icon_index    = 2;
    record_no_index   = 3;
    plugin_name_index = 4;
    tab_name_index    = 5;
    os_name_index     = 6;
    source_count_name_index = 7;
    file_name_index   = 8;
    file_path_index   = 9;
    file_size_index   = 11;
    offset_index      = 13;
    notes_value_index  = 28;
    blank_column_index = 29;


    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);

    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);

    m_tablewidget_obj->hideColumn(plugin_name_index);
    m_tablewidget_obj->hideColumn(tab_name_index);


    for(int count = 0; count < m_tablewidget_obj->columnCount(); count++)
    {
        if(m_tablewidget_obj->horizontalHeaderItem(count)->text().trimmed().startsWith("Item"))
            m_tablewidget_obj->hideColumn(count);
    }

    display_data(destination_db_path);


    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark, menuList);

    //Tags
    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
    create_tags_submenu();
    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

    menuList.clear();
    menuList   << QString(MACRO_Generic_Right_Click_Add_Note)
               << QString(MACRO_Generic_Right_Click_Remove_Note);

    submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
    create_right_click_actions(submenu_note, menuList);
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    act_quick_look = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Quick_look),mainMenu);

    QStringList submenuList;
    submenuList << QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                << ""
                << QString(MACRO_CASE_TREE_Plist_Viewer)
                << QString(MACRO_CASE_TREE_Hex_Viewer)
                << QString(MACRO_CASE_TREE_Sqlite_Viewer)
                << QString(MACRO_CASE_TREE_Log_Viewer);

    submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
    create_right_click_actions(submenu_open_with , submenuList);

    submenu_open_files_with_other_app = new QMenu(this);
    connect(submenu_open_files_with_other_app,SIGNAL(triggered(QAction*)),this,SLOT(slot_sub_menu_open_file_with_other_app_clicked(QAction*)));
    QList <QMenu*> open_with_other_app_menu_list;
    open_with_other_app_menu_list.append(submenu_open_files_with_other_app);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(open_with_other_app_menu_list, Q_FUNC_INFO);

    act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);
    create_right_click_actions(mainMenu , menuList);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    submenuList.clear();
    submenuList << QString(MACRO_CASE_TREE_Plist_Viewer)
                << QString(MACRO_CASE_TREE_Hex_Viewer)
                << QString(MACRO_CASE_TREE_Sqlite_Viewer)
                << QString(MACRO_CASE_TREE_Log_Viewer);

    submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_send_to_bucket);
    create_right_click_actions(submenu_send_to_bucket , submenuList);
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Export
    act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

    connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));
    connect(submenu_open_with, SIGNAL(triggered(QAction*)), this, SLOT(slot_open_with_submenu_clicked(QAction*)));
    connect(submenu_send_to_bucket, SIGNAL(triggered(QAction*)), this, SLOT(slot_sendtobucket_submenu_clicked(QAction*)));


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("end " , Q_FUNC_INFO);

}


void carved_files_loader::pub_create_custom_ui()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);


    main_tab_widget_obj->addTab(gallery_viewer_obj, "Gallery View");

    connect(main_tab_widget_obj, SIGNAL(currentChanged(int)), this, SLOT(slot_main_tab_widget_current_changed(int)));
    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));

    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);

    QHeaderView *carved_file_header_view_obj = m_tablewidget_obj->horizontalHeader();
    connect(carved_file_header_view_obj , SIGNAL(sectionClicked(int)), this , SLOT(slot_table_widget_header_item_clicked(int)));


    recon_static_functions::app_debug("end " , Q_FUNC_INFO);
}

int carved_files_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(display_header_name_list.isEmpty())
    {
        recon_static_functions::app_debug("display_header_name_list.isEmpty() ----FAILED---- " , Q_FUNC_INFO);
        return -1;
    }

    bool_display_table_data_loaded = true;
    st_gallery_view_file_info_list.clear();

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


    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int bookmark_position  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position     = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int row_count = m_tablewidget_obj->rowCount();

    notes_value_index  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);
    bool_did_tableview_display_changed = true;

    while(query_index.next())
    {
        if(row_count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        m_tablewidget_obj->insertRow(row_count);
        m_tablewidget_obj->setRowHeight(row_count, 22);

        QString bkmrk_stat = MACRO_FALSE_VALUE_NUMERIC_QString;
        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            if(pos == record_no_position || pos == offset_index || pos == file_size_index)
            {
                QTableWidgetItem *item_tablewidget_int_values;
                item_tablewidget_int_values = new QTableWidgetItem;
                item_tablewidget_int_values->setData(Qt::EditRole, dataStr_from_db.toLongLong());
                item_tablewidget_int_values->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_int_values);
            }
            else if(pos == bookmark_position)
            {
                QTableWidgetItem *item_tablewidget_bookmark;

                item_tablewidget_bookmark = new QTableWidgetItem;
                item_tablewidget_bookmark->setCheckState(Qt::Unchecked);

                if(dataStr_from_db == QString::number(1))
                {
                    bkmrk_stat = MACRO_TRUE_VALUE_NUMERIC_QString;
                    item_tablewidget_bookmark->setCheckState(Qt::Checked);
                }
                else
                    item_tablewidget_bookmark->setCheckState(Qt::Unchecked);

                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_bookmark);

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
                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_notes_icon);

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

                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_tags);
            }
            else if(pos == blank_column_index)
            {
                QTableWidgetItem *item_tablewidget_blank = new QTableWidgetItem;
                item_tablewidget_blank->setText("");
                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_blank);
            }
            else if(pos == notes_value_index)
            {
                //- No entry in the table widget item
            }
            else if(pos == file_path_index)
            {
                QString relative_path = dataStr_from_db;

                QTableWidgetItem *item_tablewidget_file_path = new QTableWidgetItem;
                item_tablewidget_file_path->setText(relative_path);
                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_file_path);
            }
            else
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(row_count, pos, item_tablewidget_int);
            }

            pos ++;
        }

        ///fill structure for gallery view
        QString file_name  = m_tablewidget_obj->item(row_count ,file_name_index)->text();
        QString file_size = m_tablewidget_obj->item(row_count ,file_size_index)->text();
        QString file_path = m_tablewidget_obj->item(row_count ,file_path_index)->text();
        QString int_value = m_tablewidget_obj->item(row_count ,record_no_index)->text();
        QString source_count_name = m_tablewidget_obj->item(row_count ,source_count_name_index)->text();

        QString file_full_path;
        file_full_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(int_value,received_db_obj,"files",source_count_name,Q_FUNC_INFO);
        QString thumbnail_file_full_path;

        if(recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(file_name))
        {
            QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

            QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";

            QString fs_record_no = int_value;

            QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
            QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

            if(!thumb_file_path.isEmpty())
            {
                thumbnail_file_full_path = result_filepath + thumb_file_path;
            }
            else
            {
                file_name.remove(file_name.lastIndexOf("."),file_name.size());
                video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

                //                QString script_path = "../Resources/scripts_persistent/video_frame.py";
                //                QString cmd_python_path = global_varibale_python_path;

                //                QStringList args;
                //                args << script_path << file_full_path << video_thumbnails_path << file_size;

                QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                QStringList args;
                args << file_full_path << video_thumbnails_path << file_size;

                QProcess process_thumbnails;

                //                process_thumbnails.start(cmd_python_path,args);
                process_thumbnails.start(video_frame_exe_path,args);

                if(!process_thumbnails.waitForStarted())
                {
                    recon_static_functions::app_debug("process_thumbnails -----FAILED-----",Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error :- " + process_thumbnails.errorString() ,Q_FUNC_INFO);
                    continue;
                }

                bool_process_thumbnails_finished = false;
                connect(&process_thumbnails, SIGNAL(finished(int)), this, SLOT(slot_process_video_thumbnails_finished(int)));

                int count;
                while(1)
                {
                    count++;
                    if(count % 100 == 0)
                    {
                        QCoreApplication::processEvents();
                        count = 0;
                    }

                    if(bool_process_thumbnails_finished || process_thumbnails.state() == QProcess::NotRunning)
                    {
                        process_thumbnails.close();
                        break;
                    }
                }

                thumbnail_file_full_path = QFileInfo(video_thumbnails_path).absoluteFilePath();

                QString thumb_filepath;
                QStringList list;
                QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                QFileInfo thumbnail_file_existence(thumbnail_file_full_path);
                if(thumbnail_file_existence.exists())
                {
                    thumb_file_path.clear();
                    thumb_filepath = thumbnail_file_full_path;
                    thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                    if(!thumb_filepath.startsWith("/"))
                    {
                        thumb_filepath = thumb_filepath.prepend("/");
                    }

                    list.clear();
                    list << fs_record_no << thumb_filepath;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                }
                else
                {
                    thumb_filepath.clear();
                    QFile tmp_file("../Resources/features_resources/video_thumbnail/default_video_thumbnail.png");
                    tmp_file.copy(video_thumbnails_path);
                    thumb_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();
                    thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                    if(!thumb_filepath.startsWith("/"))
                    {
                        thumb_filepath = thumb_filepath.prepend("/");
                    }

                    list.clear();
                    list << fs_record_no << thumb_filepath;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                }
            }

            struct_global_gallery_view_file_info f1;
            f1.file_name = file_name;
            f1.file_path = file_path;
            f1.INT = int_value;
            f1.bookmark = bkmrk_stat;
            f1.source_count_name = source_count_name;
            f1.thumbnail_file_path = thumbnail_file_full_path;
            f1.full_filepath = file_full_path;
            f1.index = st_gallery_view_file_info_list.size();
            f1.table_view_row_no = row_count;

            st_gallery_view_file_info_list.append(f1);
        }
        else if (recon_helper_standard_obj->is_gallery_view_suppoprted_file(file_name, Q_FUNC_INFO))
        {
            struct_global_gallery_view_file_info f1;
            f1.file_name = file_name;
            f1.file_path = file_path;
            f1.INT = int_value;
            f1.bookmark = bkmrk_stat;
            f1.source_count_name = source_count_name;
            f1.thumbnail_file_path = file_full_path;
            f1.full_filepath = file_full_path;
            f1.index = st_gallery_view_file_info_list.size();
            f1.table_view_row_no = row_count;

            st_gallery_view_file_info_list.append(f1);
        }

        row_count ++;
    }

    bookmarked_records_from_table_display_list = recon_helper_standard_obj->pub_get_records_bookmarked_from_table_display_by_db_refrence(received_db_obj, "files", "INT", Q_FUNC_INFO);

    m_tablewidget_obj->hideColumn(notes_value_index);
    bool_display_table_data_loaded = false;

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void carved_files_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    if(bool_display_table_data_loaded)
        return;

    //    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
    //        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int current_row    = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();

    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text();

    if(plugin_name_str.trimmed().isEmpty())
        return;


    if(!m_tablewidget_obj->selectionModel()->selectedRows().isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }

    add_applications_to_open_application_menu();

    act_export->setEnabled(false);
    act_full_detail_info->setEnabled(true);

    if(selection_model_list.size() > 1)
    {
        act_full_detail_info->setEnabled(false);
        act_export->setEnabled(true);
    }
    else
    {
        act_export->setEnabled(true);
        act_full_detail_info->setEnabled(true);
    }


    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_TABLE_VIEW)
    {
        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
            {
                action->setEnabled(true);
                break;
            }
        }
    }
    else if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
    {
        if(st_gallery_view_file_info_list.size() == 0)
            return;

        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
            {
                action->setEnabled(false);
                break;
            }
        }
    }



    struct_global_right_clicks_status_depends_on_file_existance obj;
    obj.feature_name = QString(MACRO_CASE_TREE_Carved_Files);
    obj.plugin_name = plugin_name_str;
    obj.tab_name = m_tablewidget_obj->item(current_row,tab_name_index)->text();
    obj.selected_row_count = selection_model_list.size();
    obj.record_no = m_tablewidget_obj->item(current_row,record_no_index)->text();
    obj.arguments_values_list.clear();
    obj.arguments_values_list << clicked_item_tab_text;
    recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

    mainMenu->exec(QCursor::pos());
}

void carved_files_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    QString filepath        = m_tablewidget_obj->item(row, file_path_index)->text();

    if(record_no_str.trimmed().isEmpty() || filepath.trimmed().isEmpty())
        return;


    QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
    if(plugin_name_str.isEmpty())
        return;

    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();

    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_carved_files(destination_db_path, record_no_str,Q_FUNC_INFO);

    metadata_str       = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name  = source_count_name;
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = QString(MACRO_Plugin_Carved_Files_TAB_Name_Files);
    st_set_meta_obj.db_file_path = destination_db_path;

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_files_loader::update_tags_value(QString tag_data,QString colour)
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
    QList<struct_global_tagging_carved_files> carved_files_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_carved_files s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, offset_index)->text();
        s1.item3 = clicked_item_tab_text;

        carved_files_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_coulumn_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_set_tag_in_carved_files_db(INT_List, clicked_item_tab_text,tag_data);
    global_connection_manager_obj->pub_add_tag_for_carved_files(carved_files_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_files_loader::update_notes_value(QString text, int current_row)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- " + destination_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    if(table_name_list.size() > 0)
    {
        int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);

        QString table_name = table_name_list.at(0);     // Because Loader support only one tab in CSV

        QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            return;

        QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);
        QString tab_column_name = db_columns_list.at(tab_name_index);

        QString command = "UPDATE '" + table_name + "' set notes=? WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=?";
        QStringList arg_list;
        arg_list << text << record_no << plugin_name_str << tab_name_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);

        QTableWidgetItem *item_tablewidget_notes_icon = m_tablewidget_obj->item(current_row,notes_icon_index);
        if(notes_icon_index != -1)
        {
            item_tablewidget_notes_icon = m_tablewidget_obj->item(current_row,notes_icon_index);
            if(text.isEmpty())
            {
                QIcon iconNotes("");
                item_tablewidget_notes_icon->setIcon(iconNotes);
            }
            else
            {
                QIcon iconNotes("../icons/notes.png");
                item_tablewidget_notes_icon->setIcon(iconNotes);
            }
        }

        QStringList tmp_list;
        emit signal_change_notes_in_other_db_also(record_no,plugin_name_str,tab_name_str,text, tmp_list , os_name_str, feature_name);

    }

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_remove_note_triggered()
{
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

    global_connection_manager_obj->pub_remove_notes_in_carved_files_db(INT_List,clicked_item_tab_text);
    global_connection_manager_obj->pub_delete_notes_for_carved_files_db(INT_List,clicked_item_tab_text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}


void carved_files_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString searched_keyword = searched_keyword_str;

    QString source_count_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    connect(full_detail_info_obj, SIGNAL(signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString, QString)), this, SLOT(slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString, QString)));


    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no_str,destination_db_path,source_count_name_str);
    QString filepath = st_globl_recon_file_info_obj.complete_file_path;
    int preview_index_full = 1;
    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    preview_index_full = get_stack_index_for_preview(filepath ,display_file_path);
    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"",record_no_str,plugin_name,"");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_open_detach_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    //    if(m_tablewidget_obj->currentRow() < 0)
    //        return;

    //    QString filepath = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), file_path_index)->text();
    //    QString searched_keyword = searched_keyword_str;

    //    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text().trimmed();
    //    struct_GLOBAL_derived_source_info struct_source_info = global_target_sources_info_class_obj->pub_get_derived_info_structure_according_source_count_name(source_count_name);
    //    filepath = struct_source_info.fs_extraction_source_path + filepath;

    //    int preview_index_full = 1;
    //    preview_index_full = get_stack_index_for_preview(filepath);

    //    emit signal_open_in_detach_clicked_table_result(metadata_str, filepath, filepath, preview_index_full,searched_keyword,"");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_files_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString dest_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();

    QStringList list_paths_to_message_box;

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

        QString relative_filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();
        if(relative_filepath.isEmpty())
            continue;
        QString source_count_name_str = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text().trimmed();

        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name,tab_name_str);
        QString complete_filepath = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,destination_db_path,table_name,source_count_name_str,Q_FUNC_INFO);

        QStringList all_path_list;
        all_path_list << complete_filepath;


        for(int i =0 ; i< all_path_list.size() ; i++)
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

int carved_files_loader::insert_record_in_table_display(QString database_path , QString db_cmd , QStringList tab_list_from_csv, QStringList addbindvalue_list)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(tab_list_from_csv.isEmpty())
        return -1;

    destination_db_path = database_path;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(database_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + database_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return -1;
    }

    QSqlQuery query_index(destination_db);

    query_index.prepare(db_cmd);


    for(int i = 0; i < addbindvalue_list.size(); i++)
    {
        query_index.addBindValue(addbindvalue_list.at(i));
    }


    if(!query_index.exec())
    {
        recon_static_functions::app_debug(" query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return -1;
    }

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int notes_icon_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int timestamp_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);

    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int count = m_tablewidget_obj->rowCount();

    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != tab_list_from_csv.size())
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
            else if(pos == notes_icon_position)
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
            { ///for tags

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
            else if(pos == timestamp_index)
            {
                QTableWidgetItem *item_tablewidget_timestamp;

                item_tablewidget_timestamp = new QTableWidgetItem;
                item_tablewidget_timestamp->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_index.value(timestamp_index).toString(),Q_FUNC_INFO));

                m_tablewidget_obj->setItem(count, pos, item_tablewidget_timestamp);
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

    destination_db.close();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return 0;
}

void carved_files_loader::update_bookmark_value(QString status,int row, int column)
{
    if(column != 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
    {
        return;
    }


    QStringList INT_List;

    QList<struct_global_tagging_carved_files> carved_files_record_list;

    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;



    QString tmp_rec_str = m_tablewidget_obj->item(row, record_no_index)->text();
    INT_List << tmp_rec_str;

    struct_global_tagging_carved_files s1;
    s1.INT_record = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(row, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(row, tab_name_index)->text();
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();
    s1.item0 = m_tablewidget_obj->item(row, file_name_index)->text();
    s1.item1 = m_tablewidget_obj->item(row, file_path_index)->text();
    s1.item2 = m_tablewidget_obj->item(row, offset_index)->text();
    s1.item3 = clicked_item_tab_text;

    carved_files_record_list << s1;


    if(status == "1")
    {

        bookmarked_records_from_table_display_list << tmp_rec_str;
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_carved_files_db(INT_List ,clicked_item_tab_text);
        global_connection_manager_obj->pub_add_bookmark_for_carved_files_db(carved_files_record_list);

    }
    else
    {
        qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(tmp_rec_str);
        if(tmp_index >= 0)
            bookmarked_records_from_table_display_list.removeAt(tmp_index);

        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_carved_files_db(INT_List,clicked_item_tab_text);
        global_connection_manager_obj->pub_delete_bookmark_for_carved_files_db(INT_List , clicked_item_tab_text);
    }
}


void carved_files_loader::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString keyword = lineEdit_search->text().trimmed();
    if(keyword.isEmpty())
        return;

    QStringList search_string_list = keyword.split(",", Qt::SkipEmptyParts);

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    st_gallery_view_file_info_list.clear();
    bool_did_tableview_display_changed = true;

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        bool bool_search_item_matched = false;

        if(row % 50 == 0)
            QCoreApplication::processEvents();

        ///column started from 2 intentionally,(first 3 columns bookmark, notes icon and tag icon).
        /// currently 14 columns are in use for display
        for(int col_count = 2 ; col_count < 15; col_count++)
        {
            if(bool_cancel_loading)
                break;

            QString column_data = m_tablewidget_obj->item(row, col_count)->text().trimmed();
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

        if(bool_search_item_matched)
        {
            QString int_value = m_tablewidget_obj->item(row ,record_no_index)->text();
            QString file_path = m_tablewidget_obj->item(row ,file_path_index)->text();
            QString file_name  = m_tablewidget_obj->item(row ,file_name_index)->text();
            QString source_count_name = m_tablewidget_obj->item(row ,source_count_name_index)->text();
            QString full_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(int_value,destination_db_path,"files",source_count_name,Q_FUNC_INFO);
            QString file_size = m_tablewidget_obj->item(row ,file_size_index)->text();

            QString thumbnail_full_file_path;

            if(recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(file_name))
            {
                QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

                QString video_thumbnail_database_path = video_thumbnails_path + "video_thumbnails.sqlite";

                QString fs_record_no = int_value;

                QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
                QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_database_path,Q_FUNC_INFO);

                if(!thumb_file_path.isEmpty())
                {
                    thumbnail_full_file_path = result_filepath + thumb_file_path;
                }
                else
                {
                    file_name.remove(file_name.lastIndexOf("."),file_name.size());
                    video_thumbnails_path = video_thumbnails_path + file_name + "_" + fs_record_no + ".png";

                    //                    QString script_path = "../Resources/scripts_persistent/video_frame.py";
                    //                    QString cmd_python_path = global_varibale_python_path;

                    //                    QStringList args;
                    //                    args << script_path << full_file_path << video_thumbnails_path << file_size;

                    QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                    QStringList args;
                    args << full_file_path << video_thumbnails_path << file_size;

                    QProcess process_thumbnails;

                    //                    process_thumbnails.start(cmd_python_path,args);
                    process_thumbnails.start(video_frame_exe_path,args);

                    if(!process_thumbnails.waitForStarted())
                    {
                        recon_static_functions::app_debug("process_thumbnails -----FAILED-----",Q_FUNC_INFO);
                        recon_static_functions::app_debug("Error :- " + process_thumbnails.errorString() ,Q_FUNC_INFO);
                        continue;
                    }

                    bool_process_thumbnails_finished = false;
                    connect(&process_thumbnails, SIGNAL(finished(int)), this, SLOT(slot_process_video_thumbnails_finished(int)));

                    int count;
                    while(1)
                    {
                        count++;
                        if(count % 100 == 0)
                        {
                            QCoreApplication::processEvents();
                            count = 0;
                        }

                        if(bool_process_thumbnails_finished || process_thumbnails.state() == QProcess::NotRunning)
                        {
                            process_thumbnails.close();
                            break;
                        }
                    }

                    thumbnail_full_file_path = QFileInfo(video_thumbnails_path).absoluteFilePath();

                    QString thumb_filepath;
                    QStringList list;
                    QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                    QFileInfo thumbnail_file_existence(thumbnail_full_file_path);

                    if(thumbnail_file_existence.exists())
                    {
                        thumb_file_path.clear();
                        thumb_filepath = thumbnail_full_file_path;
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                    }
                    else
                    {
                        thumb_filepath.clear();
                        QFile tmp_file("../Resources/features_resources/video_thumbnail/default_video_thumbnail.png");
                        tmp_file.copy(video_thumbnails_path);
                        thumb_filepath = QFileInfo(video_thumbnails_path).absoluteFilePath();
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_database_path,Q_FUNC_INFO);
                    }
                }

                struct_global_gallery_view_file_info f1;
                f1.file_name = file_name;
                f1.file_path = file_path;
                f1.INT = int_value;
                f1.bookmark = "0";
                f1.source_count_name = source_count_name;
                f1.thumbnail_file_path = thumbnail_full_file_path;
                f1.full_filepath = full_file_path;
                f1.index = st_gallery_view_file_info_list.size();
                f1.table_view_row_no = row;
                if(m_tablewidget_obj->item(row , bookmark_value_index)->checkState() == Qt::Checked)
                {
                    f1.bookmark = "1";
                }

                st_gallery_view_file_info_list.append(f1);
            }
            else if (recon_helper_standard_obj->is_gallery_view_suppoprted_file(file_name, Q_FUNC_INFO))
            {
                struct_global_gallery_view_file_info f1;
                f1.file_name = file_name;
                f1.file_path = file_path;
                f1.INT = int_value;
                f1.bookmark = "0";
                f1.source_count_name = source_count_name;
                f1.thumbnail_file_path = full_file_path;
                f1.full_filepath = full_file_path;
                f1.index = st_gallery_view_file_info_list.size();
                f1.table_view_row_no = row;
                if(m_tablewidget_obj->item(row , bookmark_value_index)->checkState() == Qt::Checked)
                {
                    f1.bookmark = "1";
                }

                st_gallery_view_file_info_list.append(f1);
            }
        }
        else
        {
            m_tablewidget_obj->hideRow(row);

        }
    }


    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);
    emit signal_clear_all_metadata();
    if(main_tab_widget_obj->currentIndex() == enum_carved_File_TabWidget_Indexes_GALLERY_VIEW)
        slot_main_tab_widget_current_changed(enum_carved_File_TabWidget_Indexes_GALLERY_VIEW);

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void carved_files_loader::add_applications_to_open_application_menu()
{
    QString config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    QStringList application_list;
    QString cmd_str = QString("Select application_path From tbl_external_applications");
    application_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_str , 0, config_db_path, Q_FUNC_INFO);

    if(!application_list.isEmpty())
    {
        QFileIconProvider icon_provide;

        QList<QAction*> actions_list = submenu_open_with->actions();

        for(int count = 0; count < actions_list.size(); count++)
        {
            QAction *act_submenu =  actions_list.at(count);

            if(act_submenu->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
            {
                submenu_open_files_with_other_app->clear();

                for(int num = 0; num < application_list.size(); num++)
                {
                    QAction *act_submenu_app =  new QAction(QFileInfo(application_list.at(num)).fileName().toLocal8Bit(), this);
                    act_submenu_app->setIcon(icon_provide.icon(QFileInfo(application_list.at(num))));
                    submenu_open_files_with_other_app->addAction(act_submenu_app);
                    submenu_open_files_with_other_app->setToolTip(application_list.at(num));
                }

                act_submenu->setMenu(submenu_open_files_with_other_app);
                break;
            }

        }
    }
    else
    {
        submenu_open_files_with_other_app->clear();

        QList<QAction*> actions_list = submenu_open_with->actions();

        for(int count = 0; count < actions_list.size(); count++)
        {
            QAction *act_submenu =  actions_list.at(count);

            if(act_submenu->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application))
            {
                QList <QMenu*> open_with_other_app_menu_list;
                open_with_other_app_menu_list.append(submenu_open_files_with_other_app);
                recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(open_with_other_app_menu_list, Q_FUNC_INFO);
                act_submenu->setMenu(submenu_open_files_with_other_app);
                break;
            }
        }
    }

}

void carved_files_loader::action_add_note_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    m_tablewidget_obj->blockSignals(true);

    add_notes_to_singal_record_obj->set_notes("");
    add_notes_to_singal_record_obj->show();

    m_tablewidget_obj->blockSignals(false);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void carved_files_loader::slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status)
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
