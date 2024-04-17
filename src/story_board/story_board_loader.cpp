#include "story_board_loader.h"

story_board_loader::story_board_loader(QWidget *parent)
{

    bool_clear_table_display = false;
    bool_right_click_actions_added = true;

    add_record_with_files = QString(MACRO_Generic_Right_Click_Add_Record_With_Files);
    add_file_to_story     = QString(MACRO_Generic_Right_Click_Add_Files);
    add_record            = QString("Add Record");

    bool_checkbox_blurr_status = false;
    bool_checkbox_censor_skintone_tag_image_status = false;

    global_recon_file_info_obj = new global_recon_file_info(this);

}

story_board_loader::~story_board_loader()
{
}

void story_board_loader::pub_set_tag_icon_path(QString icon_str)
{
    tag_icon_str = icon_str;
}

void story_board_loader::pub_tag_search_set_display_db_path(QString db_path)
{
    destination_db_path_for_tag_search = db_path;
}

void story_board_loader::pub_set_story_board_loader_essential()
{
    examiner_notes_source_path.clear();
}


void story_board_loader::pub_set_examiner_notes_file_path(QString file_path)
{
    examiner_notes_source_path = file_path;
}

void story_board_loader::pub_set_current_tag_name(QString tag_name)
{
    current_tag_name = tag_name;
}

void story_board_loader::pub_set_clear_table_display(bool status)
{
    bool_clear_table_display = status;
}

void story_board_loader::pub_set_bool_for_right_click_actions(bool status)
{
    bool_right_click_actions_added = status;
}

void story_board_loader::pub_set_story_file_name_and_path(QString file_name, QString file_path)
{
    story_file_name = file_name;
    story_file_path = file_path;
}

void story_board_loader::pub_set_storyboard_textedit_class_obj(story_board_textedit *obj)
{
    story_board_textedit_obj = obj;
    story_board_textedit_obj->pub_set_blurr_status(bool_checkbox_blurr_status);

}

void story_board_loader::pub_create_ui()
{
    stacked_widget_obj = new QStackedWidget(this);
    stacked_widget_obj->setFixedWidth(300);

    av_preview_obj = new av_preview(this);
    image_preview_obj = new  image_preview(this);
    blank_obj = new blank(this);

    stacked_widget_obj->addWidget(av_preview_obj);
    stacked_widget_obj->addWidget(image_preview_obj);
    stacked_widget_obj->addWidget(blank_obj);
    stacked_widget_obj->setCurrentIndex(enum_no_preview_index);

    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    main_vbox = new QVBoxLayout(this);

    hbox_table_and_preview = new QHBoxLayout(this);
    hbox_table_and_preview->setSpacing(10);

    m_tablewidget_timeline_obj = new m_tablewidget(this);

    connect(m_tablewidget_timeline_obj, SIGNAL(itemSelectionChanged()), this, SLOT(slot_timeline_tablewidget_selection_changed()));

    m_tablewidget_timeline_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_timeline_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);


    QStringList headerlist;
    headerlist << "Timestamp" << "Type" << "Record No." << "Plugin" << "Category" << "Item 1" << "Item 2" << "Source Count Name" << "";

    custom_header_widgets_QList.clear();

    m_tablewidget_timeline_obj->setColumnCount(headerlist.size());

    QStringList empty_header_stringlist;

    for(int column_count = 0; column_count < headerlist.size(); column_count++)
    {
        QString column_name = headerlist.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;

        custom_table_header_widget_obj->pub_set_column_name(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

        m_tablewidget_timeline_obj->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        empty_header_stringlist << "";

        connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
        connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
        connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));

        custom_header_widgets_timeline_QList.append(custom_table_header_widget_obj);
    }
    m_tablewidget_timeline_obj->setHorizontalHeaderLabels(empty_header_stringlist);



    tabwidget_storyboard_obj = new QTabWidget(this);

    connect(tabwidget_storyboard_obj, SIGNAL(tabBarClicked(int)),this, SLOT(slot_storyboard_tabbar_clicked(int)));

    tabwidget_storyboard_obj->addTab(m_tablewidget_obj, "Items");
    tabwidget_storyboard_obj->addTab(m_tablewidget_timeline_obj, "Timeline");


    hbox_table_and_preview->addWidget(tabwidget_storyboard_obj);
    hbox_table_and_preview->addWidget(stacked_widget_obj);

    main_vbox->addItem(hbox_table_and_preview);

    //main_vbox->addWidget(tabwidget_storyboard_obj);

    main_vbox->setStretch(0,0);
    main_vbox->setStretch(1,1);
    main_vbox->setContentsMargins(4,4,4,4);


    this->setLayout(main_vbox);

    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));

    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect(m_tablewidget_obj,SIGNAL(signal_spacebar_quicklook_pressed()),this,SIGNAL(signal_act_quick_look_triggered()));

    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));
}

void story_board_loader::display_data_from_examiner_notes_file(QString dest_file_path)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

        if(dest_file_path.isNull())
        return;

    if(display_header_name_list.isEmpty())
    {
        recon_static_functions::app_debug("display_header_name_list.isEmpty() ----FAILED---- " , Q_FUNC_INFO);
        return;
    }

    bool_display_table_data_loaded = true;

    if(bool_clear_table_display)
        m_tablewidget_obj->setRowCount(0);


    int count = m_tablewidget_obj->rowCount();

    m_tablewidget_obj->insertRow(count);
    m_tablewidget_obj->setRowHeight(count, 22);

    QTableWidgetItem *item_icon = new QTableWidgetItem;

    QIcon icon("../icons/Features/examiner_space.png");
    item_icon->setIcon(icon);
    m_tablewidget_obj->setItem(count, 0, item_icon);

    QTableWidgetItem *item_record_no = new QTableWidgetItem;
    QTableWidgetItem *item_plugin    = new QTableWidgetItem;
    QTableWidgetItem *item_tab_name  = new QTableWidgetItem;
    QTableWidgetItem *item_source_count_name_str  = new QTableWidgetItem;
    QTableWidgetItem *item_2  = new QTableWidgetItem;


    item_plugin->setText(MACRO_Plugin_Name_Examiner_Space);
    item_tab_name->setText(MACRO_Tag_Name_Examiner_Comments);
    item_record_no->setText("1");
    item_source_count_name_str->setText("");
    item_2->setText(get_examiner_notes_data_for_table_view(dest_file_path.trimmed()));
    item_record_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);


    m_tablewidget_obj->setItem(count, plugin_name_index, item_plugin);
    m_tablewidget_obj->setItem(count, tab_name_index, item_tab_name);
    m_tablewidget_obj->setItem(count, record_no_index, item_record_no);
    m_tablewidget_obj->setItem(count, source_count_name_index, item_source_count_name_str);
    m_tablewidget_obj->setItem(count, item_2_index, item_2);

    bool_display_table_data_loaded = false;

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

    return;
}

QString story_board_loader::get_examiner_notes_data_for_table_view(QString file_path)
{
    QFile examiner_notes_html_QFile;
    examiner_notes_html_QFile.setFileName(file_path);

    if(!examiner_notes_html_QFile.open(QFile::ReadOnly))
    {
        recon_static_functions::app_debug(" File Open Examiner Notes Html -----FAILED-----" + examiner_notes_html_QFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:" + examiner_notes_html_QFile.errorString(), Q_FUNC_INFO);
        return "";
    }

    examiner_notes_html_QFile.seek(0);
    QString read_examiner_notes_html_file = examiner_notes_html_QFile.readAll();
    QTextDocument doc;

    doc.setHtml(read_examiner_notes_html_file);

    QString  examiner_notes_data = doc.toPlainText();
    examiner_notes_data.replace("\n", "<br>");

    return examiner_notes_data;
}

void story_board_loader::display_data(QString dest_db_path)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);



    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")
    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }

    //    if(!db_cmd.isEmpty())
    //        db_cmd.append("notes");


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }

    if(dest_db_path == global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite")
    {
        tag_icon_str.clear();
        command_for_table_display = "Select record_no, record_no, record_no,plugin_name,tab_name,os_scheme_display,source_count_name,item0,item1,item2,item3,item4,item5,item6,item7,item8,item9,item10,item11,item12,item13,item14,item15,item16,item17,item18,item19,notes,recon_tag_value,table_name,recon_tag_value From 'notes'";
    }
    else if(dest_db_path == global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite"
            || dest_db_path == global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite")
    {
        tag_icon_str.clear();
        command_for_table_display = "Select INT, INT, INT,plugin_name,tab_name,blank_column,source_count_name,graph_name,graph_path,timestamp From 'saved_graphs'";
    }
    else
        command_for_table_display = db_cmd;

    destination_db_path = dest_db_path;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(dest_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + dest_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query_index(*destination_db);
    query_index.prepare(command_for_table_display);


    populate_data_in_tablewidget(*destination_db, query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void story_board_loader::populate_data_in_table()
{

    fill_display_essentials(plugin_and_tab_name_list_for_csv);

    tag_icon_index = 1;
    record_no_index   = 2;
    plugin_name_index = 3;
    tab_name_index    = 4;
    os_name_index     = 5;
    source_count_name_index = 6;
    item_0_index = 6;
    item_1_index = 7;
    item_2_index = 8;
    file_path_index   = 8;
    item_3_index = 9;
    screenshot_date_creation_index = 9;
    hex_and_text_viewer_start_offset_index = 9;
    item_4_index = 10;
    hex_and_text_viewer_end_offset_index   = 10;
    item_5_index = 11;
    saved_map_file_path_index = 11;
    item_6_index = 12;
    plist_viewer_display_name_index   = 12;
    item_7_index = 13;
    item_8_index = 14;
    item_9_index = 15;
    item_10_index = 16;
    item_11_index = 17;

    item_12_index = 18;
    item_13_index = 19;
    item_14_index = 20;
    item_15_index = 21;

    item_16_index = 22;
    item_17_index = 23;
    item_18_index = 24;
    item_19_index = 25;

    tag_index = 28;
    sqlite_viewer_db_table_name_index = 29;
    tag_name_index = 30;

    m_tablewidget_obj->hideColumn(tag_name_index);
    m_tablewidget_obj->hideColumn(tag_index);
    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(sqlite_viewer_db_table_name_index);

    m_tablewidget_obj->setColumnWidth(record_no_index, 120);

    //    set_table_for_display(destination_db_path_for_tag_search);

    pub_refresh_horizontal_header();

    display_data(destination_db_path_for_tag_search);

    display_data_from_examiner_notes_file(examiner_notes_source_path);

    if(bool_right_click_actions_added)
    {
        ///quicklook
        act_quick_look = new QAction(mainMenu);
        act_quick_look = mainMenu->addAction(tr(QString(MACRO_Generic_Right_Click_Quick_look).toLocal8Bit().data()));

        QFont m_font;
        m_font.setPointSize(MACRO_QMenu_Font_Size_int);
        act_quick_look->setFont(m_font);

        connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));

        QStringList menuList;
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);

        // For custom right click
        create_right_click_actions(mainMenu , menuList);

        create_right_click_actions(mainMenu , QStringList(""));

        menuList.clear();
        menuList << add_record << add_record_with_files << add_file_to_story;

        // For custom right click
        create_right_click_actions(mainMenu , menuList);
        create_right_click_actions(mainMenu , QStringList("")); //Seperator

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Go_To_Record);
        create_right_click_actions(mainMenu , menuList);
    }
}

void story_board_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL || bool_display_table_data_loaded)
        return;

    struct_global_right_clicks_status_depends_on_file_existance obj;

    if(m_tablewidget_obj->currentRow() >=0)
    {
        QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();

        QList <QAction *> action_list = mainMenu->actions();

        for(int count = 0; count < action_list.size(); count++)
        {
            QAction *act = action_list.at(count);

            if(act->text() == QString(MACRO_Generic_Right_Click_Go_To_Record))
            {
                act->setEnabled(true);
                if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
                    act->setEnabled(false);
                else
                    act->setEnabled(true);


                break;
            }
            else if(act->text() == add_file_to_story)
            {

                if((plugin_name_str == MACRO_Plugin_Name_Sqlite_Viewer
                    || plugin_name_str == MACRO_Plugin_Name_Hex_Viewer
                    //                    || plugin_name_str == MACRO_Plugin_Name_Text_Viewer
                    //                    ||  plugin_name_str == MACRO_Plugin_Name_Strings_Viewer
                    ||  plugin_name_str == MACRO_Plugin_Name_Plist_Viewer
                    || plugin_name_str == MACRO_Plugin_Name_Log_Viewer
                    ||  plugin_name_str == MACRO_Plugin_Name_Messenger_Full_RR
                    ||  plugin_name_str == MACRO_Plugin_Name_Browser_History_Full_RR
                    ||  plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline)
                    //||  plugin_name_str == QString(MACRO_Plugin_Name_EMLX_File)
                    //||  plugin_name_str == QString(MACRO_Plugin_Name_Google_Mail)
                    ))
                {
                    act->setEnabled(false);
                }
                else
                    act->setEnabled(true);

                break;
            }

        }

    }
    else
        return;



    if(tabwidget_storyboard_obj->currentIndex() == 1)
    {

        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == add_file_to_story)
            {
                action->setEnabled(false);
                break;
            }
        }

        QModelIndexList selection_rows_list = m_tablewidget_timeline_obj->selectionModel()->selectedRows();

        obj.selected_row_count = selection_rows_list.size();

        for(int count = 0; count < selection_rows_list.size(); count++)
        {
            QModelIndex model_index = selection_rows_list.at(count);

            QString record_no_str = m_tablewidget_timeline_obj->item(model_index.row(), enum_timeline_record_no_column)->text().trimmed();
            QString plugin_str    = m_tablewidget_timeline_obj->item(model_index.row(), enum_timeline_plugin_column)->text().trimmed();
            QString category_str  = m_tablewidget_timeline_obj->item(model_index.row(), enum_timeline_category_column)->text().trimmed();
            QString source_count_name  = m_tablewidget_timeline_obj->item(model_index.row(), enum_timeline_source_count_name_column)->text().trimmed();

            obj.source_count_name = source_count_name;
            obj.plugin_name = plugin_str;
            obj.tab_name = category_str;
            obj.record_no = record_no_str;


            for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
            {
                if(m_tablewidget_obj->item(row, record_no_index)->text().trimmed() == record_no_str &&
                        m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed() == plugin_str &&
                        m_tablewidget_obj->item(row, tab_name_index)->text().trimmed() == category_str &&
                        m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed() == source_count_name)
                {
                    m_tablewidget_obj->selectRow(row);
                    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);
                    break;
                }
            }
        }
    }
    else
    {
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
        if(selection_model_list.size() < 1)
            return;


        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == add_file_to_story)
            {
                action->setEnabled(true);
                break;
            }
        }

        QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

        obj.plugin_name = plugin_name_str;
        obj.selected_row_count = m_tablewidget_obj->selectionModel()->selectedRows().size();
        obj.source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text().trimmed();
        obj.tab_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text().trimmed();
        obj.record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text().trimmed();
        if(plugin_name_str == MACRO_Plugin_Name_Carved_Files)
        {
            obj.arguments_values_list.clear();
            obj.arguments_values_list << m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_4_index)->text().trimmed();
        }
        else if(plugin_name_str == MACRO_Plugin_Name_Snapshots)
        {
            obj.arguments_values_list = QStringList(m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),item_4_index)->text());
        }


        if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story)
                {
                    action->setEnabled(false);
                    break;
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
                || plugin_name_str == QString(MACRO_Plugin_Name_Screenshots)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
                || plugin_name_str == QString(MACRO_Plugin_Name_Examiner_Space))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story || action->text() == add_record_with_files)
                {
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser)
                || plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser_Tag))
        {
            obj.arguments_values_list.clear();
            obj.arguments_values_list << m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_10_index)->text().trimmed();

            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story || action->text() == add_record_with_files)
                {
                    //do not break
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved)
                || plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story || action->text() == add_record_with_files)
                {
                    action->setEnabled(false);
                }
                if(action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record))
                {
                    QString filepath_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), file_path_index)->text().trimmed();
                    if(filepath_str.startsWith("/"+ QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features)))
                    {
                        ////note::record from carved files in viewers
                        action->setDisabled(true);
                    }
                    else
                    {
                        action->setEnabled(true);
                    }
                }

                if(plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved) &&
                        (action->text() == QString(MACRO_Generic_Right_Click_Quick_look) || action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record)))
                {
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story
                        || action->text() == add_record_with_files
                        )
                {
                    action->setEnabled(false);
                }
                if(action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record))
                {
                  QString tab_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text().trimmed();
                  if(tab_name.contains(MACRO_Plugin_Forensic_Image_In_Hex_Viewer_TAB_Name))
                  {
                      action->setEnabled(false);
                  }
                  else
                  {
                      action->setEnabled(true);
                  }
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story
                        || action->text() == add_record_with_files
                        || action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record)
                        ||action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
                {
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Unified_Logs))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story
                        || action->text() == add_record_with_files
                        || action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record)
                        ||action->text() == QString(MACRO_Generic_Right_Click_Quick_look)
                        ||action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
                {
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story
                        || action->text() == add_record_with_files
                        || action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record)
                        ||action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
                {
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR)
                ||plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR) )
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story
                        || action->text() == add_record_with_files
                        ||action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
                {
                    action->setEnabled(false);
                }
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
        {
            foreach (QAction *action, mainMenu->actions())
            {
                if(action->text() == add_file_to_story
                        ||action->text() == add_record_with_files
                        ||action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
                {
                    action->setEnabled(false);
                }
            }
        }

    }

    obj.feature_name = QString(MACRO_CASE_TREE_Story_Board);
    recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

    mainMenu->exec(QCursor::pos());
}

struct_global_recon_file_info story_board_loader::get_filepath_for_av()
{
    QString media_file_path;


    struct_global_recon_file_info st_glbl_rcn_fl_info_obj;


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    // works only for single selection
    //    if(selection_model_list.size() != 1)
    //        return QString("");
    if(selection_model_list.size() != 1)
        return st_glbl_rcn_fl_info_obj;

    QModelIndex model_index = selection_model_list.at(0);

    int selected_row_no = model_index.row();

    if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
        return st_glbl_rcn_fl_info_obj;


    QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();


    if(plugin_name_str.isEmpty())
        return st_glbl_rcn_fl_info_obj;

    QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

    if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    {
        ///sqlite file is not previewable
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        ///tagged hex data for a file is not previewable
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
    {
        ///plist file is not previewable
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer))
    {
        ///log file is not previewable
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Unified_Logs))
    {
        ///unified log file is not previewable
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString source_count_name ;
        if(tabwidget_storyboard_obj->currentIndex() == 0)
        {
            source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        }
        else if(tabwidget_storyboard_obj->currentIndex() == 1)
        {
            source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
        }

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
        //QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,fs_db_path,"files",source_count_name,Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
    {
        //        QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
        //        QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,screenshot_db_path,"tags","",Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_screenshots(record_no);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        //        QString tmln_svd_grph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
        //        QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,tmln_svd_grph_db_path,"tags","",Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_timeline_graphs(record_no);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        //        QString svd_maps_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
        //        QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,svd_maps_db_path,"tags","",Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_saved_maps(record_no);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        //QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        //QString file_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed() , Q_FUNC_INFO);
        QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_4_index)->text();
        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser) || plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser_Tag))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        //        QString svd_maps_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
        //        QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,svd_maps_db_path,"saved_graphs","",Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_messenger_saved_graphs(record_no);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        // QString svd_maps_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
        //QString cmplt_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,svd_maps_db_path,"saved_graphs","",Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_browser_histroy_saved_graphs(record_no);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR)
            || plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer))
    {
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        //QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
        //QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,"text_content_info",source_count_name,Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_text_viewer(record_no,source_count_name);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
    {
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        //QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
        //QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,"string_content_info",source_count_name,Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_string_viewer(record_no,source_count_name);
        return st_glbl_rcn_fl_info_obj;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no,tab_name_index)->text();
        QString snapshot_label_name = m_tablewidget_obj->item(selected_row_no,item_4_index)->text();

        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , tab_name_str);
        QString snapshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";

        //QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,snapshot_db_path,table_name,source_count_name,Q_FUNC_INFO);
        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_snapshots_by_db_path(record_no,snapshot_db_path ,tab_name_str, source_count_name);
        return st_glbl_rcn_fl_info_obj;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Examiner_Space))
    {
        return st_glbl_rcn_fl_info_obj;
    }
    else
    {

        //        QString tmp_plg_name = plugin_name_str;
        //        tmp_plg_name.replace(" ","_");

        //        QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
        //        QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

        //        if(!QFileInfo(plugins_destination_db_file).exists())
        //            return st_glbl_rcn_fl_info_obj;

        //        struct_global_fetch_metadata struct_metadata_values_obj;
        //        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_of_plugin(plugin_name_str,tab_name_str,record_no,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), os_name_str,Q_FUNC_INFO);

        //        media_file_path = struct_metadata_values_obj.file_path;

        //        bool bool_virtual_export = struct_metadata_values_obj.bool_virtual_export;
        //        if(!bool_virtual_export && !media_file_path.trimmed().isEmpty())
        //        {
        //            QString source_count_name_str;
        //            if(tabwidget_storyboard_obj->currentIndex() == 0)
        //            {
        //                source_count_name_str = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        //            }
        //            else if(tabwidget_storyboard_obj->currentIndex() == 1)
        //            {
        //                source_count_name_str = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
        //            }

        //            struct_GLOBAL_derived_source_info struct_target_info = global_target_sources_info_class_obj->pub_get_derived_info_structure_according_source_count_name(source_count_name_str);

        //            if(struct_target_info.os_naming_scheme == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
        //            {
        //                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_str +  "/file_system.sqlite";
        //                media_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(media_file_path), 0, fs_db_path, Q_FUNC_INFO);
        //            }

        //            if(!media_file_path.startsWith(struct_target_info.virtual_source_path))
        //            {
        //                media_file_path = recon_static_functions::get_complete_file_path(struct_target_info.virtual_source_path, media_file_path, Q_FUNC_INFO);
        //            }

        //        }

        st_glbl_rcn_fl_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_file_exist_any(plugin_name_str,tab_name_str ,record_no);

        return st_glbl_rcn_fl_info_obj;

    }

    return st_glbl_rcn_fl_info_obj;
}

void story_board_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    stacked_widget_obj->setCurrentIndex(enum_no_preview_index);

    if(row < 0 || column < 0)
        return;

    QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text();

    if(!m_tablewidget_obj->selectionModel()->selectedRows(column).isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();


        for(int i = 0 ; i < act_list.size() ; i++)
        {

            if((plugin_name_str == MACRO_Plugin_Name_Sqlite_Viewer ||
                plugin_name_str == MACRO_Plugin_Name_Hex_Viewer ||
                plugin_name_str == MACRO_Plugin_Name_Text_Viewer ||
                plugin_name_str == MACRO_Plugin_Name_Strings_Viewer ||
                plugin_name_str == MACRO_Plugin_Name_Plist_Viewer ||
                plugin_name_str == MACRO_Plugin_Name_Log_Viewer ||
                plugin_name_str == MACRO_Plugin_Name_Messenger_Full_RR ||
                plugin_name_str == MACRO_Plugin_Name_Browser_History_Full_RR) && act_list.at(i)->text() == add_file_to_story)
            {
                act_list.at(i)->setEnabled(false);

            }
            else
            {
                act_list.at(i)->setEnabled(true);
            }
        }
    }


    struct_global_recon_file_info st_glbl_rcn_fl_info_obj = get_filepath_for_av();
    QString file_path = st_glbl_rcn_fl_info_obj.complete_file_path;
    QString display_file_path_str = st_glbl_rcn_fl_info_obj.display_file_path;

    emit signal_quick_look_preview_filepath_from_table_result_loader(file_path);

    QFileInfo info(file_path);
    if(info.exists())
    {
        QString previewed_path = "";
        previewed_path = file_path;

        QString file_mime_type = mime_database_obj.mimeTypeForFile(previewed_path).name();

        if(file_mime_type.startsWith("video/"))
        {
            stacked_widget_obj->setCurrentIndex(enum_audio_video_preview_index);
            av_preview_obj->load_source(previewed_path);
        }
        else if (file_mime_type.startsWith("audio/"))
        {
            stacked_widget_obj->setCurrentIndex(enum_audio_video_preview_index);
            av_preview_obj->load_source(previewed_path);
        }
        else if(recon_helper_standard_obj->check_is_it_image_file(previewed_path,display_file_path_str, Q_FUNC_INFO))
        {
            stacked_widget_obj->setCurrentIndex(enum_image_preview_index);
            image_preview_obj->set_image_by_path(previewed_path);
        }
        else
        {
            stacked_widget_obj->setCurrentIndex(enum_no_preview_index);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void story_board_loader::slot_right_click_main_menu_triggered(QAction* current_clicked_action)
{

    if(current_clicked_action->text() == add_record_with_files)
    {
        action_add_record_to_story_with_files_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    {
        action_copy_to_clipboard();
    }
    else if(current_clicked_action->text() == add_file_to_story)
    {
        action_add_file_to_story_triggered();
    }
    else if(current_clicked_action->text() == add_record)
    {
        action_add_record_to_story_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record))
    {
        action_goto_record_triggered();
    }

}

void story_board_loader::action_add_record_to_story_with_files_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    bool_cancel_loading = false;
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Adding records to story");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    metadata_str.clear();
    QString story_data;

    ///for timeline tab
    if(tabwidget_storyboard_obj->currentIndex() == 1)
    {

        QModelIndexList selection_model_list = m_tablewidget_timeline_obj->selectionModel()->selectedRows();

        for(int i = 0; i < selection_model_list.size(); i++)
        {

            bool bool_header_inserted = false;

            metadata_str.clear();

            if(i % 50 == 0)
                QCoreApplication::processEvents();

            if(bool_cancel_loading)
                break;

            QModelIndex model_index = selection_model_list.at(i);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 || selected_row_no >= m_tablewidget_timeline_obj->rowCount())
                continue;

            if(m_tablewidget_timeline_obj->isRowHidden(selected_row_no))
                continue;

            //artifacts Plugin/Tab name
            QString record_no   = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_record_no_column)->text().trimmed();
            QString artifact_name_str   = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_plugin_column)->text().trimmed();
            QString artifact_tab_str    = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_category_column)->text().trimmed();
            QString source_count_name;
            if(tabwidget_storyboard_obj->currentIndex() == 0)
            {
                source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            }
            else if(tabwidget_storyboard_obj->currentIndex() == 1)
            {
                source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
            }



            story_data += create_html_table_start();

            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString copied_file_path;
            QString display_file_path;
            QString timestamp_str    = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_timestamp_column)->text().trimmed();
            QString timestamp_type_str = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_type_column)->text().trimmed();
            timestamp_type_str.append(QString("(") + m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_type_column)->toolTip() + QString(")"));

            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Source Name") + ":</b>" +QString("\t") + struct_info.complete_source_name +"<br>" ;
            metadata_str += "<br>";

            metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + artifact_name_str + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + artifact_tab_str + "\n<br>";
            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Timestamp") + ":</b>" +QString("\t") +  timestamp_str + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Timestamp Type") + ":</b>" +QString("\t") +  timestamp_type_str + "\n<br>";
            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Item1") + ":</b>" +QString("\t") + m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_item_1_column)->text().trimmed()+ "\n<br>";
            metadata_str += "<b>" + QObject::tr("Item2") + ":</b>" +QString("\t") + m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_item_2_column)->text().trimmed() + "\n<br>";
            metadata_str += "<br>";

            if(artifact_name_str == QString(MACRO_Plugin_Name_File_System))
            {
                struct_global_export_file_system_record obj;
                obj.record_no = record_no;
                obj.source_count_name = source_count_name;
                QString file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj, Q_FUNC_INFO);

                copied_file_path = file_path;
                display_file_path = copied_file_path;
                current_media_path = copied_file_path;
            }
            else
            {

                current_media_path.clear();


                QStringList all_path_list , display_file_path_list;
                QList <struct_global_recon_file_info> recon_file_info_qlist = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_all_file(artifact_name_str,artifact_tab_str,record_no);
                for(int pp = 0 ; pp < recon_file_info_qlist.size() ; pp++)
                {
                    struct_global_recon_file_info st_recon_file_info_obj = recon_file_info_qlist.at(pp);
                    QString complete_file_path = st_recon_file_info_obj.complete_file_path;
                    //                    if(!QFileInfo(complete_file_path).exists())
                    //                        continue ;

                    QString label_file_path = st_recon_file_info_obj.display_file_path;
                    display_file_path_list << label_file_path;
                    all_path_list << complete_file_path;
                }

                QStringList list_header;
                //                if(all_path_list.size() == 0)
                //                    list_header << QString("Detailed Information");
                //                else
                //                    list_header << QString("Detailed Information") << QString("File Preview");

                list_header << QString("Detailed Information") << QString("File Preview");


                if(!bool_header_inserted)
                {
                    story_data += create_html_table_header(list_header);
                    bool_header_inserted = true;
                }

                QStringList raw_data_list;
                raw_data_list << metadata_str << all_path_list.join(MACRO_RECON_Splitter_1_multiple);


                story_data += create_html_table_rows_data_for_for_multiple_files(raw_data_list , display_file_path_list);
                story_data += create_html_table_end();
                story_data += "<br></br>";
                emit signal_add_record_to_story_board_by_loader(story_data,current_media_path);
                story_data.clear();
                continue ;

            }

            QString base_64_data;

            if(!bool_header_inserted)
            {
                QStringList list_header;

                if(current_media_path.isEmpty())
                {
                    current_media_path.clear();

                    list_header << QString("Detailed Information");
                }
                else
                {
                    if(recon_helper_standard_obj->check_is_it_image_file(current_media_path,display_file_path,Q_FUNC_INFO))
                    {
                        if(bool_checkbox_censor_skintone_tag_image_status)
                        {
                            QString tag_name = recon_helper_standard_obj->get_tag_name_of_record_no_by_source_count_name(record_no, source_count_name);

                            if(tag_name == MACRO_Tag_Name_Skin_Tone)
                            {

                                QString censored_images_Dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                                        + source_count_name + QString("/") + QString(MACRO_Censored_Skin_Tone_Images_Dir_Name_QString);

                                QString filename_actl = QFileInfo(current_media_path).fileName();
                                QString m_file_name = record_no + "_" + filename_actl;
                                m_file_name = recon_static_functions::get_available_filename(m_file_name, censored_images_Dir_path,Q_FUNC_INFO);
                                QString dest_file_path = censored_images_Dir_path + m_file_name;

                                current_media_path = recon_helper_standard_obj->get_censored_filepath(current_media_path,dest_file_path);

                            }
                        }

                        base_64_data = get_image_hex_data(current_media_path);
                        QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                        QString exported_file_path =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(copied_file_path,resource_dir_path,QFileInfo(display_file_path).fileName() , Q_FUNC_INFO);
                        current_media_path = exported_file_path;

                    }
                    else
                    {
                        QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                        QString exported_file_path =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(copied_file_path,resource_dir_path,QFileInfo(display_file_path).fileName() , Q_FUNC_INFO);

                        QString filename = QFileInfo(exported_file_path).fileName();

                        current_media_path = "./Resources/" + filename;
                    }

                    list_header << QString("Detailed Information") << QString("File Preview");
                }

                story_data += create_html_table_header(list_header);

                bool_header_inserted = true;
            }


            QStringList row_data_list;
            row_data_list << metadata_str << current_media_path;

            story_data += create_html_table_rows_data_for_general_files(row_data_list, base_64_data);

            story_data += create_html_table_end();

            story_data += "<br></br>";

            emit signal_add_record_to_story_board_by_loader(story_data,current_media_path);

            story_data.clear();

            current_media_path.clear();

        }

        emit signal_save_data();
        display_loading_progress_bar_obj->hide();


        return;
    }


    ///for table display tab
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        metadata_str.clear();

        bool bool_header_inserted = false;

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();

        if(plugin_name_str.isEmpty())
            continue;

        QString source_count_name;
        if(tabwidget_storyboard_obj->currentIndex() == 0)
        {
            source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        }
        else if(tabwidget_storyboard_obj->currentIndex() == 1)
        {
            source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
        }

        struct_GLOBAL_witness_info_source struct_target_info_final_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        QString copied_file_path;

        story_data += create_html_table_start();

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        current_media_path.clear();
        QString display_file_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();

        if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text(); // 7

            QString sqlite_data = m_tablewidget_obj->item(selected_row_no, 11)->text();
            sqlite_data = sqlite_data.replace("\n", "<br>");

            metadata_str.clear();

            metadata_str += "<b>" + QString("Plugin") + ":</b>" +QString("\t")+ QString(MACRO_Plugin_Name_Sqlite_Viewer) +"<br>" ;
            metadata_str += "<b>" + QString("File Path") + ":</b>" +QString("\t")+ filepath +"<br>" ;
            metadata_str += "<b>" + QString("Data") + ":</b>" +QString("\t")+ sqlite_data +"<br>" ;
            current_media_path = "";

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Unified_Logs))
        {
            QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
            QString unified_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";
            QString destination_db_path;
            QString command = QString("SELECT db_name from tbl_index where source_count_name = ?");
            QStringList values;
            values << source_count_name;
            QString unified_logs_db = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, values,0, unified_logs_db_path, Q_FUNC_INFO);

            destination_db_path = unified_logs_dir_path + unified_logs_db;

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_unified_logs_tag_entry(record_no,source_count_name,Q_FUNC_INFO);

            QString metadata_value_str = struct_metadata_values_obj.metadata;

            current_media_path = QString("");

            metadata_str = metadata_value_str;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
        {

            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString artifact_name_str   = m_tablewidget_obj->item(selected_row_no, 7)->text().trimmed();
            QString artifact_tab_str    = m_tablewidget_obj->item(selected_row_no, 8)->text().trimmed();
            QString stamp_type = m_tablewidget_obj->item(selected_row_no, 12)->text().trimmed();
            QString command = QString("select record_no,timestamp,stamp_type,plugin,category,item1,item2,source_file,recon_tag_value,notes from timeline_data where record_no = '"+ record_no + "' AND stamp_type = '"+ stamp_type + "' AND plugin = '"+ artifact_name_str + "' AND category = '"+ artifact_tab_str + "'");

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite", command ,struct_info, Q_FUNC_INFO);


            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = "";

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_hex_viewer(record_no,source_count_name,Q_FUNC_INFO);
            QString metadata_value_str = struct_metadata_values_obj.metadata;
            current_media_path = QString("");
            metadata_str = metadata_value_str;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
        {
            QString content_file_name = m_tablewidget_obj->item(selected_row_no, item_4_index)->text();

            struct_GLOBAL_witness_info_source struct_target_info;
            struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
            struct_global_csv_reader_full_csv_info md_csv_redaer_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
            QString table_name =  global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str);
            QString command;
            if(md_csv_redaer_obj.metadata_column_name_QList_tablewidget.size() > 0)
            {
                QStringList db_cols_nam_list =  md_csv_redaer_obj.metadata_column_name_QList_tablewidget.at(0);
                command = db_cols_nam_list.join(",");
                command = "select " + command + " from  '"+ table_name + "' where INT = '" + record_no +"'" + " AND content_file_name = '" + content_file_name + "'";
            }

            struct_global_fetch_metadata struct_metadata_values_obj;
            QString db_path;
            QString file_path;
            if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
                file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,table_name,source_count_name,Q_FUNC_INFO);
                struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("" ,0,record_no,plugin_name_str,db_path,command,struct_target_info, Q_FUNC_INFO);

            }
            else if(plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
                file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,table_name,source_count_name,Q_FUNC_INFO);
                struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("" ,0,record_no,plugin_name_str,db_path,command,struct_target_info, Q_FUNC_INFO);
            }
            copied_file_path = file_path;

            current_media_path = file_path;
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
        {

            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
            QString viewer_display_name = m_tablewidget_obj->item(selected_row_no, plist_viewer_display_name_index)->text();
            //            QString source_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

            QString command = "SELECT recon_tag_value FROM plist_content_info WHERE file_path = ? AND viewer_display_name =  ? AND record_no = ?";
            QStringList val_list;
            val_list << filepath << viewer_display_name << record_no;
            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

            struct_global_fetch_metadata struct_metadata_values_obj;

            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_plist_viewer_tag_entry(filepath,tag_name,viewer_display_name, record_no,source_count_name,Q_FUNC_INFO);

            QString metadata_value_str = struct_metadata_values_obj.metadata;

            current_media_path = QString("");
            metadata_str = metadata_value_str;

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer))
        {
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
            QString viewer_display_name = m_tablewidget_obj->item(selected_row_no, item_6_index)->text();
            QString tagged_data = m_tablewidget_obj->item(selected_row_no, item_3_index)->text();

            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";

            QString command = "SELECT recon_tag_value FROM log_content_info WHERE file_path = ? AND viewer_display_name = ? AND record_no = ? AND tagged_data = ?";

            QStringList val_list;
            val_list << filepath << viewer_display_name << record_no << tagged_data;
            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_log_viewer_tag_entry(filepath,tag_name,viewer_display_name, record_no,source_count_name, tagged_data, Q_FUNC_INFO);

            QString metadata_value_str = struct_metadata_values_obj.metadata;

            current_media_path = QString("");

            metadata_str = metadata_value_str;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            QString display_file_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();

            struct_global_export_file_system_record obj;
            obj.record_no = record_no;
            obj.source_count_name = source_count_name;
            obj.display_file_path = display_file_path;
            obj.displayed_file_name = QFileInfo(display_file_path).fileName();

            QString filepath = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj, Q_FUNC_INFO);

            copied_file_path = filepath;

            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no,source_count_name, Q_FUNC_INFO);

            current_media_path = filepath;
            metadata_str       = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_screenshots_tag_entry(m_tablewidget_obj->item(selected_row_no, file_path_index)->text(),record_no,Q_FUNC_INFO);
            QString metadata_value_str = struct_metadata_values_obj.metadata;
            current_media_path = struct_metadata_values_obj.preview_file_path;
            metadata_str = metadata_value_str;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_timeline_graph_entry(m_tablewidget_obj->item(selected_row_no, file_path_index)->text(),record_no,Q_FUNC_INFO);
            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = struct_metadata_values_obj.preview_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_maps_entry(m_tablewidget_obj->item(selected_row_no, file_path_index)->text(),record_no,Q_FUNC_INFO);
            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = struct_metadata_values_obj.preview_file_path;
        }
        //        else if(plugin_name_str == QString(MACRO_Plugin_Name_EMLX_File))
        //        {
        //        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("redefined_result",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite", "" ,struct_info, Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = "";
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("redefined_result",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString()  + "Browser_History.sqlite", "" ,struct_info, Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = "";
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        {
            metadata_str = recon_helper_standard_obj->fetch_metadata_for_ram_analysis_saved(record_no,Q_FUNC_INFO);
            current_media_path = QString("");
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            current_media_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed(), Q_FUNC_INFO);
            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(m_tablewidget_obj->item(selected_row_no , 10)->text()),0,index_db_path,Q_FUNC_INFO);

            QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_carved_files(carved_files_db_path, record_no,Q_FUNC_INFO);
            metadata_str = struct_metadata_values_obj.metadata;

            copied_file_path = current_media_path;

        }
        //        else if(plugin_name_str == QString(MACRO_Plugin_Name_Google_Mail))
        //        {
        //        }
        else if(plugin_name_str == MACRO_Plugin_Name_Snapshots)
        {
            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            QString snapshot_label_name = m_tablewidget_obj->item(selected_row_no,item_4_index)->text();

            QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , tab_name_str);
            QString snapshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";

            current_media_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,snapshot_db_path,table_name,source_count_name,Q_FUNC_INFO);
            copied_file_path = current_media_path;

            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name,0,record_no,plugin_name_str,snapshot_db_path,source_count_name,Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;

        }
        else
        {
            struct_global_fetch_metadata struct_metadata_values_obj;

            QString tmp_plg_name = plugin_name_str;
            tmp_plg_name.replace(" ","_");
            QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
            QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;
            if(!QFileInfo(plugins_destination_db_file).exists())
                continue ;

            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_of_plugin(plugin_name_str,tab_name_str,record_no,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(),Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;


            current_media_path.clear();

            QStringList all_path_list , display_file_path_list;
            QList <struct_global_recon_file_info> recon_file_info_qlist = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_all_file(plugin_name_str,tab_name_str,record_no);

            for(int pp = 0 ; pp < recon_file_info_qlist.size() ; pp++)
            {
                struct_global_recon_file_info st_recon_file_info_obj = recon_file_info_qlist.at(pp);

                QString complete_file_path = st_recon_file_info_obj.complete_file_path;
                //                if(!QFileInfo(complete_file_path).exists())
                //                    continue ;

                QString label_file_path = st_recon_file_info_obj.display_file_path;
                display_file_path_list << label_file_path;
                all_path_list << complete_file_path;
            }

            QStringList list_header;
            //            if(all_path_list.size() == 0)
            //                list_header << QString("Detailed Information");
            //            else
            //                list_header << QString("Detailed Information") << QString("File Preview");
            list_header << QString("Detailed Information") << QString("File Preview");


            if(!bool_header_inserted)
            {
                story_data += create_html_table_header(list_header);
                bool_header_inserted = true;
            }

            QStringList raw_data_list;
            raw_data_list << metadata_str << all_path_list.join(MACRO_RECON_Splitter_1_multiple);


            story_data += create_html_table_rows_data_for_for_multiple_files(raw_data_list , display_file_path_list);
            story_data += create_html_table_end();
            story_data += "<br></br>";
            story_data.replace("\n", "<br>");
            emit signal_add_record_to_story_board_by_loader(story_data,current_media_path);
            story_data.clear();
            continue ;

        }



        if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            QStringList list_header;
            list_header << QString("Detailed Information");
            story_data += create_html_table_header(list_header);
            bool_header_inserted = true;


            QString map_filepath = m_tablewidget_obj->item(selected_row_no, saved_map_file_path_index)->text().trimmed();

            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();


            QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
            QString rec_command = "Select INT from files where filepath = ?";
            QStringList arg;
            arg << filepath;

            QString record_no   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(rec_command,arg ,0,db_path_filesystem,Q_FUNC_INFO);
            filepath =   recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path_filesystem,"files",source_count_name,Q_FUNC_INFO);


            if(!map_filepath.isEmpty())
            {
                map_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + map_filepath;
                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, map_filepath, filepath);

                copied_file_path = map_filepath;
                QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                filepath =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(filepath,resource_dir_path,QFileInfo(display_file_path).fileName(),Q_FUNC_INFO);

                QString filename = QFileInfo(map_filepath).fileName();
                filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                map_filepath =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(map_filepath,resource_dir_path,filename,Q_FUNC_INFO);


                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, map_filepath, filepath ,plugin_name_str);

            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved))
        {
            QStringList list_header;
            list_header << QString("Detailed Information");
            story_data += create_html_table_header(list_header);
            bool_header_inserted = true;

            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            if(!filepath.isEmpty())
            {
                filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + filepath;
                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), filepath);
                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), filepath,plugin_name_str);
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
        {
            QStringList list_header;
            list_header << QString("Detailed Information");
            story_data += create_html_table_header(list_header);
            bool_header_inserted = true;

            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            if(!filepath.isEmpty())
            {
                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), filepath);
                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), filepath,plugin_name_str);
            }
        }
        else
        {
            QString base_64_data;
            if(!bool_header_inserted)
            {
                QStringList list_header;

                if(current_media_path.isEmpty())
                {
                    current_media_path.clear();

                    list_header << QString("Detailed Information");
                }
                else
                {
                    if(recon_helper_standard_obj->check_is_it_image_file(current_media_path,display_file_path,Q_FUNC_INFO))
                    {
                        if(bool_checkbox_censor_skintone_tag_image_status)
                        {
                            QString tag_name = recon_helper_standard_obj->get_tag_name_of_record_no_by_source_count_name(record_no, source_count_name);

                            if(tag_name == MACRO_Tag_Name_Skin_Tone)
                            {

                                QString censored_images_Dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                                        + source_count_name + QString("/") + QString(MACRO_Censored_Skin_Tone_Images_Dir_Name_QString);

                                QString filename_actl = QFileInfo(current_media_path).fileName();
                                QString m_file_name = record_no + "_" + filename_actl;
                                m_file_name = recon_static_functions::get_available_filename(m_file_name, censored_images_Dir_path,Q_FUNC_INFO);
                                QString dest_file_path = censored_images_Dir_path + m_file_name;

                                current_media_path = recon_helper_standard_obj->get_censored_filepath(current_media_path,dest_file_path);

                            }
                        }

                        base_64_data = get_image_hex_data(current_media_path);

                        QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                        QString exported_file_path =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(copied_file_path,resource_dir_path,QFileInfo(display_file_path).fileName(),Q_FUNC_INFO);
                        current_media_path = exported_file_path;
                    }
                    else
                    {
                        if(bool_cancel_loading)
                            continue ;

                        QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                        QString exported_file_path =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(copied_file_path,resource_dir_path,QFileInfo(display_file_path).fileName(),Q_FUNC_INFO);
                        QString filename = QFileInfo(exported_file_path).fileName();

                        current_media_path = "./Resources/" + filename;
                    }

                    list_header << QString("Detailed Information") << QString("File Preview");
                }

                story_data += create_html_table_header(list_header);

                bool_header_inserted = true;
            }


            QStringList row_data_list;
            row_data_list << metadata_str << current_media_path;

            story_data += create_html_table_rows_data_for_general_files(row_data_list, base_64_data);

            story_data += create_html_table_end();
        }

        story_data += "<br></br>";
        story_data.replace("\n", "<br>");

        emit signal_add_record_to_story_board_by_loader(story_data,current_media_path);

        story_data.clear();


    }

    display_loading_progress_bar_obj->hide();

    emit signal_save_data();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void story_board_loader::action_add_record_to_story_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Adding records to story...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    metadata_str.clear();
    QString story_data;

    ///for timeline tab
    if(tabwidget_storyboard_obj->currentIndex() == 1)
    {

        QModelIndexList selection_model_list = m_tablewidget_timeline_obj->selectionModel()->selectedRows();

        for(int i = 0; i < selection_model_list.size(); i++)
        {
            metadata_str.clear();

            if(i % 50 == 0)
                QCoreApplication::processEvents();

            if(bool_cancel_loading)
                break ;

            QModelIndex model_index = selection_model_list.at(i);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 || selected_row_no >= m_tablewidget_timeline_obj->rowCount())
                continue;

            if(m_tablewidget_timeline_obj->isRowHidden(selected_row_no))
                continue;

            //artifacts Plugin/Tab name
            QString artifact_name_str   = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_plugin_column)->text().trimmed();
            QString artifact_tab_str    = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_category_column)->text().trimmed();
            QString source_count_name;
            if(tabwidget_storyboard_obj->currentIndex() == 0)
            {
                source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            }
            else if(tabwidget_storyboard_obj->currentIndex() == 1)
            {
                source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
            }


            story_data += create_html_table_start();

            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString timestamp_str    = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_timestamp_column)->text().trimmed();
            QString timestamp_type_str = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_type_column)->text().trimmed();
            timestamp_type_str.append(QString("(") + m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_type_column)->toolTip() + QString(")"));

            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Source Name") + ":</b>" +QString("\t") + struct_info.complete_source_name +"<br>" ;
            metadata_str += "<br>";

            metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + artifact_name_str + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + artifact_tab_str + "\n<br>";
            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Timestamp") + ":</b>" +QString("\t") +  timestamp_str + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Timestamp Type") + ":</b>" +QString("\t") +  timestamp_type_str + "\n<br>";
            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Item1") + ":</b>" +QString("\t") + m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_item_1_column)->text().trimmed()+ "\n<br>";
            metadata_str += "<b>" + QObject::tr("Item2") + ":</b>" +QString("\t") + m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_item_2_column)->text().trimmed() + "\n<br>";
            metadata_str += "<br>";

            current_media_path = "";

            QStringList row_data_list;
            row_data_list << metadata_str << current_media_path;

            story_data += create_html_table_rows_data_for_general_files(row_data_list, QString(""));
            story_data += create_html_table_end();

            story_data += "<br></br>";

            emit signal_add_record_to_story_board_by_loader(story_data,current_media_path);

            story_data.clear();
        }

        emit signal_save_data();

        display_loading_progress_bar_obj->hide();

        return;
    }

    ///for table display tab
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        metadata_str.clear();
        bool bool_header_inserted = false;

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break ;

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();

        if(plugin_name_str.isEmpty())
            continue;


        story_data += create_html_table_start();

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        QString source_count_name;
        if(tabwidget_storyboard_obj->currentIndex() == 0)
        {
            source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        }
        else if(tabwidget_storyboard_obj->currentIndex() == 1)
        {
            source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
        }

        if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text(); // 7
            QString db_table_name = m_tablewidget_obj->item(selected_row_no, sqlite_viewer_db_table_name_index)->text();
            //            QString source_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            QString sqlite_data = m_tablewidget_obj->item(selected_row_no, 11)->text();
            sqlite_data = sqlite_data.replace("\n", "<br>");

            metadata_str.clear();

            metadata_str += "<b>" + QString("Plugin") + ":</b>" +QString("\t")+ QString(MACRO_Plugin_Name_Sqlite_Viewer) +"<br>" ;
            metadata_str += "<b>" + QString("File Path") + ":</b>" +QString("\t")+ filepath +"<br>" ;
            metadata_str += "<b>" + QString("Data") + ":</b>" +QString("\t")+ sqlite_data +"<br>" ;
            current_media_path = "";


            //            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

            //            QString command = "SELECT recon_tag_value FROM sv_content_info WHERE file_path = ?  AND record_no_in_sqliteviewer = ? AND db_table_name = ?";
            //            QStringList val_list;
            //            val_list << filepath << record_no << db_table_name;
            //            QString tag_name = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);


            //            struct_global_fetch_metadata struct_metadata_values_obj;
            //            struct_metadata_values_obj = recon_helper_obj->fetch_metadata_for_sqlite_viewer_tag_entry(filepath,tag_name,record_no,db_table_name,source_name,Q_FUNC_INFO);

            //            QString metadata_value_str = struct_metadata_values_obj.metadata;
            //            QString block_file_path = struct_metadata_values_obj.file_path;


            //            current_media_path = block_file_path;
            //            metadata_str = metadata_value_str;

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
        {

            //artifacts Plugin/Tab name
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString artifact_name_str   = m_tablewidget_obj->item(selected_row_no, 7)->text().trimmed();
            QString artifact_tab_str    = m_tablewidget_obj->item(selected_row_no, 8)->text().trimmed();
            QString stamp_type = m_tablewidget_obj->item(selected_row_no, 12)->text().trimmed();
            QString command = QString("select record_no,timestamp,stamp_type,plugin,category,item1,item2,source_file,recon_tag_value,notes from timeline_data where record_no = '"+ record_no + "' AND stamp_type = '"+ stamp_type + "' AND plugin = '"+ artifact_name_str + "' AND category = '"+ artifact_tab_str + "'");



            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite", command ,struct_info, Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = "";

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_hex_viewer(record_no,source_count_name,Q_FUNC_INFO);
            QString metadata_value_str = struct_metadata_values_obj.metadata;
            QString block_file_path = struct_metadata_values_obj.file_path;
            current_media_path = block_file_path;
            metadata_str = metadata_value_str;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
        {
            QString content_file_name = m_tablewidget_obj->item(selected_row_no, item_4_index)->text();
            struct_GLOBAL_witness_info_source struct_target_info;
            struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
            struct_global_csv_reader_full_csv_info md_csv_redaer_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
            QString command;
            QString table_name =  global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str);
            if(md_csv_redaer_obj.metadata_column_name_QList_tablewidget.size() > 0)
            {
                QStringList db_cols_nam_list =  md_csv_redaer_obj.metadata_column_name_QList_tablewidget.at(0);
                command = db_cols_nam_list.join(",");
                command = "select " + command + " from  '"+ table_name + "' where INT = '" + record_no +"'" + " AND content_file_name = '" + content_file_name + "'";
            }

            struct_global_fetch_metadata struct_metadata_values_obj;
            QString db_path;
            QString file_path;
            if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
                file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,table_name,source_count_name,Q_FUNC_INFO);
                struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("" ,0,record_no,plugin_name_str,db_path,command,struct_target_info, Q_FUNC_INFO);
            }
            else if(plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
                file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,table_name,source_count_name,Q_FUNC_INFO);
                struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("" ,0,record_no,plugin_name_str,db_path,command,struct_target_info, Q_FUNC_INFO);
            }

            current_media_path = file_path;
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
        {

            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
            QString viewer_display_name = m_tablewidget_obj->item(selected_row_no, plist_viewer_display_name_index)->text();
            //            QString source_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

            QString command = "SELECT recon_tag_value FROM plist_content_info WHERE file_path = ? AND viewer_display_name =  ? AND record_no = ?";
            QStringList val_list;
            val_list << filepath << viewer_display_name << record_no;
            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

            struct_global_fetch_metadata struct_metadata_values_obj;

            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_plist_viewer_tag_entry(filepath,tag_name,viewer_display_name, record_no,source_count_name,Q_FUNC_INFO);

            QString metadata_value_str = struct_metadata_values_obj.metadata;

            current_media_path = filepath;
            metadata_str = metadata_value_str;

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer))
         {
             QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
             QString viewer_display_name = m_tablewidget_obj->item(selected_row_no, item_6_index)->text();
             QString tagged_data = m_tablewidget_obj->item(selected_row_no, item_3_index)->text();

             QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";

             QString command = "SELECT recon_tag_value FROM log_content_info WHERE file_path = ? AND viewer_display_name = ? AND record_no = ? AND tagged_data = ?";

             QStringList val_list;
             val_list << filepath << viewer_display_name << record_no << tagged_data;
             QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

             struct_global_fetch_metadata struct_metadata_values_obj;
             struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_log_viewer_tag_entry(filepath,tag_name,viewer_display_name, record_no,source_count_name, tagged_data, Q_FUNC_INFO);

             QString metadata_value_str = struct_metadata_values_obj.metadata;

             current_media_path = filepath;
             metadata_str = metadata_value_str;
         }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no,source_count_name, Q_FUNC_INFO);

            current_media_path = "";
            metadata_str       = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_screenshots_tag_entry(m_tablewidget_obj->item(selected_row_no, file_path_index)->text(),record_no,Q_FUNC_INFO);
            current_media_path = struct_metadata_values_obj.preview_file_path;
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Examiner_Space))
        {
            QString examiner_data = recon_helper_standard_obj->pub_fetch_examiner_comments_data();

            metadata_str.clear();
            metadata_str += "<b>" + QString("Plugin") + ":</b>" +QString("\t")+ QString(MACRO_Plugin_Name_Examiner_Space) + "<br>";
            metadata_str += "<b>" + QString("TAB Name") + ":</b>" +QString("\t")+ QString(MACRO_Tag_Name_Examiner_Comments) + "<br><br>";

            metadata_str += examiner_data;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_timeline_graph_entry(m_tablewidget_obj->item(selected_row_no, file_path_index)->text(),record_no,Q_FUNC_INFO);
            current_media_path = struct_metadata_values_obj.preview_file_path;
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_maps_entry(m_tablewidget_obj->item(selected_row_no, file_path_index)->text(),record_no,Q_FUNC_INFO);
            current_media_path = struct_metadata_values_obj.preview_file_path;
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser))
        {
            QString emlx_source_file = m_tablewidget_obj->item(selected_row_no, 16)->text().trimmed();

            if(emlx_source_file.isEmpty())
                return;

            QString metadata_value_str;

            QString emlx_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString() + "email_data.sqlite";
            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(emlx_db_path);
            QSqlQuery query_select(destination_db);

            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" ---FAILED--- to open DB" + emlx_db_path,Q_FUNC_INFO);
                recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                return;
            }


            QString command = QString("select INT,emlx_to,emlx_from,emlx_subject,emlx_date,emlx_msg_body,email_attachment,source_count_name From emlx_data Where source_file = ? AND INT = ?");
            query_select.prepare(command);
            query_select.addBindValue(emlx_source_file);
            query_select.addBindValue(record_no);

            if(!query_select.exec())
            {
                recon_static_functions::app_debug("  ---FAILED--- select query",Q_FUNC_INFO);
                recon_static_functions::app_debug(query_select.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(query_select.executedQuery(),Q_FUNC_INFO);
                destination_db.close();
                return;
            }

            QString source_file_emlx = emlx_source_file;

            while(query_select.next())
            {

                QString source_count_name = query_select.value(7).toString().trimmed();

                struct_GLOBAL_witness_info_source struct_target_info;
                struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


                metadata_value_str += "<b>" + QObject::tr("Source") + ":</b>" +QString("\t") + struct_target_info.complete_source_name +"<br>" ;
                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + plugin_name_str + "\n<br>";
                metadata_value_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + tab_name_str + "\n<br>";
                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br>" ;
                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("To") + ":</b>" +QString("\t") + query_select.value(1).toString() + "\n<br>";
                metadata_value_str += "<b>" + QObject::tr("From") + ":</b>" +QString("\t") + query_select.value(2).toString() + "\n<br>";
                metadata_value_str += "<b>" + QObject::tr("Subject") + ":</b>" +QString("\t") + query_select.value(3).toString() + "\n<br>";
                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("Date") + ":</b>" +QString("\t") + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_select.value(4).toString(), Q_FUNC_INFO) + "\n<br>";
                //                metadata_value_str += "<br>";
                //                metadata_value_str += "<b>" + QObject::tr("Message Body") + ":</b>" +QString("\t<br><br>") + query_select.value(5).toString() + "\n<br>";
                //                metadata_value_str += "<br>";
                QString emlx_filename = QFileInfo(source_file_emlx).fileName();
                QString eml_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Eml_Files/";

                QDir eml_dir(eml_dir_path);
                if(!eml_dir.exists())
                {
                    eml_dir.mkdir(eml_dir_path);
                }

                QString complete_emlx_path = struct_target_info.virtual_source_path + source_file_emlx;

                emlx_filename = recon_static_functions::get_available_filename(emlx_filename, eml_dir_path, Q_FUNC_INFO);
                QString file_copy_path = eml_dir_path + emlx_filename;

                QFile::copy(complete_emlx_path, file_copy_path);
                QString relative_path = "./Eml_Files/" + recon_helper_standard_obj->pub_get_html_formated_filename(QFileInfo(file_copy_path).fileName(),Q_FUNC_INFO);

                metadata_value_str += "<br>";
                metadata_value_str +=  "<b>" + QObject::tr("Source File") + " : </b>";
                metadata_value_str += QString("<a href='") + relative_path + QString("'>") + source_file_emlx + QString("</a>");
                metadata_value_str += "<br>";

                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("Message Body") + ":</b>" +QString("\t<br><br>") + query_select.value(5).toString() + "\n<br>";
                metadata_value_str += "<br>";

                QString attachment_str = query_select.value(6).toString().trimmed();

                if(!attachment_str.isEmpty())
                {
                    if(attachment_str.contains(","))
                    {
                        QStringList attachment_split_list;
                        attachment_split_list = attachment_str.split(",");

                        for(int count = 0; count < attachment_split_list.size(); count++)
                        {
                            QString attachment_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString() + "Attachments/" + attachment_split_list.at(count);

                            if(count == 0)
                            {
                                if(QFileInfo(attachment_path).exists())
                                    metadata_value_str += "<b>" + QObject::tr("Attachments") + ":</b>" + QString("\t") + "\n<br>";
                            }

                            if(QFileInfo(attachment_path).exists())
                            {
                                if(recon_helper_standard_obj->check_is_it_image_file(attachment_path,attachment_path,Q_FUNC_INFO))
                                {
                                    QImage img(attachment_path);

                                    img = img.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                                    QByteArray arr;
                                    QBuffer buf(&arr);
                                    img.save(&buf, "JPG" );

                                    metadata_value_str += QString("<img src=data:image/png;base64,") + QString::fromLocal8Bit(arr.toBase64())  + " width=\"140\" height=\"150\" >";
                                }
                                else
                                {
                                    QString filename = QFileInfo(attachment_path).fileName();
                                    QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";

                                    filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                                    QString file_copy_path = resource_dir_path + filename;

                                    QFile::copy(attachment_path, file_copy_path);
                                    //attachment_path = "./Resources/" + filename;
                                    QString relative_path = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(QFileInfo(file_copy_path).fileName(),Q_FUNC_INFO);

                                    metadata_value_str += "<br>";
                                    //metadata_value_str +=  QString("<a href='") + attachment_path + QString("'>") + QFileInfo(attachment_path).fileName() + QString("</a>");
                                    metadata_value_str +=  QString("<a href='") + relative_path + QString("'>") + QFileInfo(attachment_path).fileName() + QString("</a>");
                                    metadata_value_str += "<br>";
                                    metadata_value_str += "<br>";
                                }
                            }
                        }
                    }
                    else
                    {
                        QString attachment_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString() + "Attachments/" + attachment_str;

                        if(QFileInfo(attachment_path).exists())
                        {
                            metadata_value_str += "<b>" + QObject::tr("Attachments") + ":</b>" + QString("\t") + "\n<br>";

                            if(recon_helper_standard_obj->check_is_it_image_file(attachment_path,attachment_path,Q_FUNC_INFO))
                            {
                                QImage img(attachment_path);

                                img = img.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                                QByteArray arr;
                                QBuffer buf(&arr);
                                img.save(&buf, "JPG" );

                                metadata_value_str += QString("<img src=data:image/png;base64,") + QString::fromLocal8Bit(arr.toBase64())  + " width=\"140\" height=\"150\" >";
                            }
                            else
                            {
                                QString filename = QFileInfo(attachment_path).fileName();
                                QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";

                                filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                                QString file_copy_path = resource_dir_path + filename;


                                QFile::copy(attachment_path, file_copy_path);
                                //attachment_path = "./Resources/" + filename;
                                QString relative_path = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(QFileInfo(file_copy_path).fileName(),Q_FUNC_INFO);

                                metadata_value_str += "<br>";
                                //metadata_value_str +=  QString("<a href='") + attachment_path + QString("'>") + QFileInfo(attachment_path).fileName() + QString("</a>");
                                metadata_value_str +=  QString("<a href='") + relative_path + QString("'>") + QFileInfo(attachment_path).fileName() + QString("</a>");
                                metadata_value_str += "<br>";
                                metadata_value_str += "<br>";
                            }
                        }
                    }
                }
            }
            destination_db.close();
            metadata_str = metadata_value_str;
            current_media_path.clear();
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser_Tag))
        {
            QString metadata_value_str;
            QString current_m_path;
            QString preview_f_path;

            QString tag_name = m_tablewidget_obj->item(selected_row_no, 12)->text().trimmed();
            QString exported_file_name = m_tablewidget_obj->item(selected_row_no, 10)->text().trimmed();

            QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_name +".sqlite";
            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(tag_db_path);
            QSqlQuery query_select(destination_db);

            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" ---FAILED--- to open DB" + tag_db_path,Q_FUNC_INFO);
                recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                return;
            }

            QString command = "select  INT , plugin_name, tab_name ,item0 ,item1 ,item2 ,item3,item4 ,recon_tag_value ,notes, item9,source_count_name from tags where record_no = ? AND tab_name = ? AND item3 = ?";

            query_select.prepare(command);
            query_select.addBindValue(record_no);
            query_select.addBindValue(tab_name_str);
            query_select.addBindValue(exported_file_name);

            if(!query_select.exec())
            {
                recon_static_functions::app_debug("  ---FAILED--- select query",Q_FUNC_INFO);
                recon_static_functions::app_debug(query_select.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(query_select.executedQuery(),Q_FUNC_INFO);
                destination_db.close();
                return;
            }

            while(query_select.next())
            {

                QString source_count_name = query_select.value("source_count_name").toString();

                struct_GLOBAL_witness_info_source struct_target_info;
                struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

                metadata_value_str += "<b>" + QObject::tr("Source") + ":</b>" +QString("\t") + struct_target_info.complete_source_name +"<br>" ;

                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br>" ;
                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value("item0").toString() + "\n<br>";
                metadata_value_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + query_select.value("item1").toString() + "\n<br>";
                metadata_value_str += "<b>" + QObject::tr("File Size") + ":</b>" +QString("\t") + recon_static_functions::human_readable_size(query_select.value("item4").toString(), Q_FUNC_INFO) + "\n<br>";
                metadata_value_str += "<br>";
                metadata_value_str += "<b>" + QObject::tr("Tagged Text") + ":</b>" +QString("\t") + query_select.value("item2").toString() + "\n<br>";



                QString tag_text = "";
                QStringList list_tags = query_select.value("recon_tag_value").toString().split(",", Qt::SkipEmptyParts);

                if(list_tags.isEmpty())
                    list_tags << tag_name;

                for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                {
                    if(list_tags.at(tag_i).isEmpty())
                        continue;

                    QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                    tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                }

                if(tag_text.endsWith(","))
                    tag_text.chop(1);

                metadata_value_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";

                current_m_path = query_select.value("item9").toString();
                preview_f_path = query_select.value("item1").toString();
            }

            destination_db.close();

            current_media_path.clear();
            metadata_str =  metadata_value_str;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("redefined_result",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString()  + "Messenger.sqlite", "" ,struct_info, Q_FUNC_INFO);


            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = "";
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
        {

            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("redefined_result",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite", "" ,struct_info, Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;
            current_media_path = "";
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        {
            metadata_str = recon_helper_standard_obj->fetch_metadata_for_ram_analysis_saved(record_no,Q_FUNC_INFO);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            current_media_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed(), Q_FUNC_INFO);
            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(m_tablewidget_obj->item(selected_row_no , 10)->text()),0,index_db_path,Q_FUNC_INFO);

            QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_carved_files(carved_files_db_path, record_no,Q_FUNC_INFO);
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
            QString cmd_2 = "select relative_db_path from tbl_carved_password_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(m_tablewidget_obj->item(selected_row_no,11)->text()),0,index_db_path,Q_FUNC_INFO);
            QString carved_password_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based("carved_password",0,record_no,plugin_name_str,carved_password_db_path,source_count_name,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
        {
            struct_global_fetch_metadata struct_metadata_values_obj;
            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_graph(record_no,plugin_name_str,Q_FUNC_INFO);
            current_media_path = struct_metadata_values_obj.preview_file_path;
            metadata_str = struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
            QString cmd_2 = "select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(m_tablewidget_obj->item(selected_row_no,18)->text()),0,index_db_path,Q_FUNC_INFO);
            QString registry_viewer_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based("tbl_registry_tree",0,record_no,plugin_name_str,registry_viewer_db_path,source_count_name,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();
            QString mobile_backup_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
            QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , MACRO_Plugin_Mobile_Backup_TAB_Name_Backup);
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name,0,record_no,plugin_name_str,mobile_backup_db_path,source_count_name,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no , source_count_name_index)->text();
            QString table_name = m_tablewidget_obj->item(selected_row_no , item_13_index)->text();
            QString carved_data_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name,0,record_no,plugin_name_str,carved_data_db_path,source_count_name,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
        {
            QString tab_name_str = m_tablewidget_obj->item(selected_row_no,tab_name_index)->text();
            QString snapshot_label_name = m_tablewidget_obj->item(selected_row_no,item_4_index)->text();
            QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , tab_name_str);

            QString snapshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";
            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name,0,record_no,plugin_name_str,snapshot_db_path,source_count_name,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;

        }
        else
        {
            struct_global_fetch_metadata struct_metadata_values_obj;

            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_of_plugin(plugin_name_str,tab_name_str,record_no,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

            metadata_str = struct_metadata_values_obj.metadata;
        }

        /// because only record will be added(no file)
        current_media_path.clear();
        if(!bool_header_inserted)
        {
            QStringList list_header;

            list_header << QString("Detailed Information");

            story_data += create_html_table_header(list_header);

            bool_header_inserted = true;
        }


        if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            QString map_filepath = m_tablewidget_obj->item(selected_row_no, saved_map_file_path_index)->text().trimmed();

            if(!map_filepath.isEmpty())
            {
                map_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + map_filepath;
                QString filename = QFileInfo(map_filepath).fileName();
                QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";

                filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                QString file_copy_path = resource_dir_path + filename;

                QFile::copy(map_filepath, file_copy_path);

                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, file_copy_path, QString(""));
                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, file_copy_path, QString(""),plugin_name_str);
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
        {
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            if(!filepath.isEmpty())
            {
                filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + filepath;
                QString filename = QFileInfo(filepath).fileName();
                QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";

                filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                QString file_copy_path = resource_dir_path + filename;

                QFile::copy(filepath, file_copy_path);

                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), file_copy_path);
                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), file_copy_path,plugin_name_str);
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
        {
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            if(!filepath.isEmpty())
            {
                QString filename = QFileInfo(filepath).fileName();
                QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";

                filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                QString file_copy_path = resource_dir_path + filename;

                QFile::copy(filepath, file_copy_path);

                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), file_copy_path);
                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), file_copy_path,plugin_name_str);
            }
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
        {
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
            if(!filepath.isEmpty())
            {
                QString recon_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                if(!filepath.startsWith(recon_result_dir_path))
                    filepath = recon_static_functions::get_complete_file_path(recon_result_dir_path, filepath, Q_FUNC_INFO);

                QString filename = QFileInfo(filepath).fileName();
                QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                filename = recon_static_functions::get_available_filename(filename, resource_dir_path, Q_FUNC_INFO);
                QString file_copy_path = resource_dir_path + filename;
                QFile::copy(filepath, file_copy_path);
                //story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), file_copy_path);
                story_data += create_html_table_rows_data_for_our_created_files(metadata_str, QString(""), file_copy_path,plugin_name_str);
            }
        }
        else
        {
            QStringList row_data_list;
            row_data_list << metadata_str << current_media_path;

            story_data += create_html_table_rows_data_for_general_files(row_data_list, QString(""));
        }

        story_data += create_html_table_end();

        story_data += "<br></br>";
        story_data.replace("\n", "<br>");

        emit signal_add_record_to_story_board_by_loader(story_data,current_media_path);

        story_data.clear();
    }

    emit signal_save_data();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void story_board_loader::action_add_file_to_story_triggered()
{
    bool_cancel_loading = false;
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Adding records to story...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    metadata_str.clear();

    ///for timeline tab
    if(tabwidget_storyboard_obj->currentIndex() == 1)
    {
        m_tablewidget_obj->clearSelection();

        QModelIndexList selection_model_list = m_tablewidget_timeline_obj->selectionModel()->selectedRows();

        for(int i = 0; i < selection_model_list.size(); i++)
        {
            if(i % 100 == 0)
                QCoreApplication::processEvents();

            if(bool_cancel_loading)
                break ;

            QModelIndex model_index = selection_model_list.at(i);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 || selected_row_no >= m_tablewidget_timeline_obj->rowCount())
                continue;

            if(m_tablewidget_timeline_obj->isRowHidden(selected_row_no))
                continue;

            QString record_no_timeline       = m_tablewidget_timeline_obj->item(selected_row_no, 2)->text().trimmed();
            QString plugin_name_str_timeline = m_tablewidget_timeline_obj->item(selected_row_no, 3)->text().trimmed();
            QString tab_name_str_timeline    = m_tablewidget_timeline_obj->item(selected_row_no, 4)->text().trimmed();
            QString source_count_name_timeline    = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_source_count_name_column)->text().trimmed();

            for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
            {
                QString record_no = m_tablewidget_obj->item(row, record_no_index)->text().trimmed();
                QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
                QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text().trimmed();
                QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();

                if(record_no == record_no_timeline && plugin_name_str == plugin_name_str_timeline && tab_name_str_timeline == tab_name_str && source_count_name_timeline == source_count_name)
                {
                    m_tablewidget_obj->selectRow(row);
                    break;
                }
            }
        }
    }


    ///for table display tab
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
        {
            display_loading_progress_bar_obj->hide();
            emit signal_on_cancel_pushbutton();
            break;
        }

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();


        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        QString source_count_name;
        if(tabwidget_storyboard_obj->currentIndex() == 0)
        {
            source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        }
        else if(tabwidget_storyboard_obj->currentIndex() == 1)
        {
            source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
        }

        struct_GLOBAL_witness_info_source struct_target_info;
        struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        QString ios_link_label_path;

        current_media_path.clear();

        if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
        {


        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer))
        {


        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            int path_index   = file_path_index;
            QString f_label_name = m_tablewidget_obj->item(selected_row_no, path_index)->text();
            QString display_file_path = m_tablewidget_obj->item(selected_row_no,file_path_index)->text();

            struct_global_export_file_system_record obj;
            obj.record_no = record_no;
            obj.source_count_name = source_count_name;
            obj.display_file_path = display_file_path;
            obj.displayed_file_name = QFileInfo(display_file_path).fileName();

            QString file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj , Q_FUNC_INFO);

            ios_link_label_path = f_label_name;

            struct_global_fetch_metadata struct_metadata_values_obj;

            struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no,source_count_name, Q_FUNC_INFO);

            current_media_path = file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
        {
            QString db_path;
            QString file_path;
            if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
                file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,"text_content_info",source_count_name,Q_FUNC_INFO);
            }
            else if(plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
                file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path,"string_content_info",source_count_name,Q_FUNC_INFO);
            }

            ios_link_label_path = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            current_media_path =  file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
        {
            current_media_path = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
        {
            QString tmln_svd_grph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
            current_media_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,tmln_svd_grph_db_path,"tags","",Q_FUNC_INFO);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        {
            // intentionally blank
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString file_path = m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();
            file_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,file_path , Q_FUNC_INFO);
            current_media_path = file_path;
            ios_link_label_path = m_tablewidget_obj->item(selected_row_no, file_path_index)->text().trimmed();
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
        {
            QString tab_name_str = m_tablewidget_obj->item(selected_row_no,tab_name_index)->text();
            QString snapshot_label_name = m_tablewidget_obj->item(selected_row_no,item_4_index)->text();
            QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , tab_name_str);
            QString snapshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,snapshot_db_path,table_name,source_count_name,Q_FUNC_INFO);
            current_media_path = complete_path;

        }
        else
        {
            //            struct_global_fetch_metadata struct_metadata_values_obj;
            //            struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_of_plugin(plugin_name_str,tab_name_str,record_no,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), os_name_str,Q_FUNC_INFO);

            //            bool bool_virtual_export = struct_metadata_values_obj.bool_virtual_export;
            //            current_media_path = struct_metadata_values_obj.file_path;

            //            ios_link_label_path = struct_metadata_values_obj.file_path;

            //            if(!bool_virtual_export && !current_media_path.trimmed().isEmpty())
            //            {
            //                //                QString source_count_name_str  = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            //                if(struct_target_info.os_naming_scheme == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
            //                {
            //                    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name +  "/file_system.sqlite";
            //                    current_media_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(current_media_path), 0, fs_db_path, Q_FUNC_INFO);
            //                }

            //                if(!current_media_path.startsWith(struct_target_info.virtual_source_path))
            //                {
            //                    current_media_path = recon_static_functions::get_complete_file_path(struct_target_info.virtual_source_path, current_media_path, Q_FUNC_INFO);
            //                }
            //            }


            QList <struct_global_recon_file_info> recon_file_info_qlist = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_all_file(plugin_name_str,tab_name_str,record_no);
            for(int pp = 0 ; pp < recon_file_info_qlist.size() ; pp++)
            {
                struct_global_recon_file_info st_recon_file_info_obj = recon_file_info_qlist.at(pp);

                QString complete_file_path = st_recon_file_info_obj.complete_file_path;
                if(!QFileInfo(complete_file_path).exists())
                    continue;

                QString label_file_path = st_recon_file_info_obj.display_file_path;
                QString filename_actl = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(label_file_path);

                story_board_textedit_obj->pub_add_file_to_story_board(filename_actl,complete_file_path, bool_checkbox_blurr_status,label_file_path);
            }
            continue;

        }

        QString filename_actl = QFileInfo(current_media_path).fileName();

        if(struct_target_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                || struct_target_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
        {
            filename_actl = QFileInfo(ios_link_label_path).fileName();
        }
        //  QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";

        //  QString filename_vrtl = recon_static_functions::get_available_filename(filename_actl, resource_dir_path, Q_FUNC_INFO);
        //  QString file_copy_path = resource_dir_path + filename_vrtl;

        QString label_file_path = current_media_path;
        label_file_path = label_file_path.trimmed();
        QString rslt_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        if(label_file_path.startsWith(struct_target_info.virtual_source_path))
            label_file_path = label_file_path.remove(0,struct_target_info.virtual_source_path.size());
        else if(label_file_path.startsWith(rslt_dir_path))
            label_file_path = label_file_path.remove(0,rslt_dir_path.size());

        if(!ios_link_label_path.trimmed().isEmpty())
            label_file_path = ios_link_label_path;

        if(!label_file_path.isEmpty() && !label_file_path.startsWith("/"))
            label_file_path.prepend("/");

        if(bool_checkbox_censor_skintone_tag_image_status)
        {
            QString tag_name = recon_helper_standard_obj->get_tag_name_of_record_no_by_source_count_name(record_no, source_count_name);

            if(tag_name == MACRO_Tag_Name_Skin_Tone)
            {
                QString censored_images_Dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                        + source_count_name + QString("/") + QString(MACRO_Censored_Skin_Tone_Images_Dir_Name_QString);


                if(recon_helper_standard_obj->check_is_it_image_file(current_media_path ,label_file_path, Q_FUNC_INFO))
                {
                    QString m_file_name = record_no + "_" + filename_actl;
                    m_file_name = recon_static_functions::get_available_filename(m_file_name, censored_images_Dir_path,Q_FUNC_INFO);
                    QString dest_file_path = censored_images_Dir_path + m_file_name;

                    current_media_path = recon_helper_standard_obj->get_censored_filepath(current_media_path,dest_file_path);
                }

            }
        }

        //        if(!global_recon_555_helper_singular_class_obj->check_is_it_image_file(current_media_path , Q_FUNC_INFO))
        //        {
        //            if(QFileInfo(current_media_path).isFile())
        //            {
        //                QFile::copy(current_media_path, file_copy_path);
        //            }
        //            else
        //            {

        //                display_loading_progress_bar_obj->pub_set_label_messsge("Copying Files...");
        //                display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
        //                display_loading_progress_bar_obj->show();


        //                display_loading_progress_bar_obj->hide();
        //            }

        //            current_media_path = file_copy_path;
        //        }


        //emit signal_add_file_to_story_board_by_loader(filename_actl,current_media_path, bool_checkbox_blurr_status,label_file_path);
        story_board_textedit_obj->pub_add_file_to_story_board(filename_actl,current_media_path, bool_checkbox_blurr_status,label_file_path);

    }
    emit signal_save_data();

    display_loading_progress_bar_obj->hide();

}

void story_board_loader::action_goto_record_triggered()
{
    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString source_count_name;
    if(tabwidget_storyboard_obj->currentIndex() == 0)
    {
        source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
    }
    else if(tabwidget_storyboard_obj->currentIndex() == 1)
    {
        source_count_name = m_tablewidget_timeline_obj->item(m_tablewidget_timeline_obj->currentRow(), enum_timeline_source_count_name_column)->text();
    }

    struct_GLOBAL_witness_info_source struct_info;
    QString os_schme_dsply;
    if(!source_count_name.isEmpty())
    {
        struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        os_schme_dsply  = struct_info.os_scheme_display;
    }
    QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();

    if(plugin_name_str ==  QString(MACRO_Plugin_Name_File_System) || plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
    {
        QString source_name = struct_info.source_name;

        QString display_name = source_name ;

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        QString filepath_str = m_tablewidget_obj->item(current_row , file_path_index)->text();

        if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
        {
            QString temp_file_path = filepath_str.remove(struct_info.fs_extraction_source_path);

            QString command = QString("Select INT from files Where filepath = ?");
            QStringList value_list;
            value_list << temp_file_path;
            record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, fs_db_path, Q_FUNC_INFO);
        }

        emit signal_tablewidget_goto_record(display_name,QString(MACRO_CASE_TREE_Source),filepath_str,record_no , fs_db_path, os_schme_dsply);
    }
    else if(plugin_name_str ==  QString(MACRO_Plugin_Name_Screenshots))
    {
        emit signal_tablewidget_goto_record(QString(MACRO_Plugin_Name_Screenshots),QString(QString(MACRO_CASE_TREE_Screenshots)),tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str ==  QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        emit signal_tablewidget_goto_record(QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph),QString(QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)),tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str ==  QString(MACRO_Plugin_Name_Saved_Maps))
    {
        emit signal_tablewidget_goto_record(QString(MACRO_Plugin_Name_Saved_Maps),QString(QString(MACRO_Plugin_Name_Saved_Maps)),tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser) || plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser_Tag))
    {
        emit signal_tablewidget_goto_record(QString(MACRO_CASE_TREE_Email_Files_Category_Email_Parser),QString(MACRO_CASE_TREE_Email_Files_Parent),tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
            ||plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        emit signal_tablewidget_goto_record(QString(plugin_name_str),QString(MACRO_CASE_TREE_Graph_Result_Saved),tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
    {
        //artifacts Plugin/Tab name
        QString artifact_name_str   = m_tablewidget_obj->item(current_row, 7)->text().trimmed();
        QString artifact_tab_str    = m_tablewidget_obj->item(current_row, 8)->text().trimmed();

        QString artifacts_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";
        QString command_str = QString("Select source_count_name from timeline_data Where record_no = ? AND plugin = ? AND category = ?");
        QStringList value_list;
        value_list << record_no << artifact_name_str << artifact_tab_str;

        QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, value_list, 0, artifacts_timeline_db_path, Q_FUNC_INFO);
        source_count_name = source_count_name.trimmed();
        if(source_count_name.isEmpty())
            return;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        emit signal_tablewidget_goto_record(artifact_name_str,QString("Parent RECON Plugin"),artifact_tab_str,record_no , QString(""), struct_info.os_scheme_display);

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Carved_Files,m_tablewidget_obj->item(current_row, 10)->text().trimmed(),record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
    {
        QString result_tab_name_str = m_tablewidget_obj->item(current_row,11)->text();
        emit signal_tablewidget_goto_record(plugin_name_str,QString(MACRO_CASE_TREE_Carved_Password),result_tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Redefined_Result_Parent,tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
    {
        QString result_tab_name_str = m_tablewidget_obj->item(current_row,item_12_index)->text();
        emit signal_tablewidget_goto_record(plugin_name_str,QString(MACRO_CASE_TREE_Carved_Data),result_tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        QString snp_temperred_name = m_tablewidget_obj->item(current_row,item_4_index)->text();
        emit signal_tablewidget_goto_record_snapshots(tab_name_str,snp_temperred_name ,record_no ,QString(MACRO_CASE_TREE_Snapshots));
    }
    else
        emit signal_tablewidget_goto_record(plugin_name_str,QString("Parent RECON Plugin"),tab_name_str,record_no , QString(""), os_schme_dsply);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void story_board_loader::slot_tableWidget_doubleClicked(int row, int column)
{
    m_tablewidget *m_tablewidget_obj = (m_tablewidget*) sender();

    //- On last column due to crash returned from below lines.
    if(column == (m_tablewidget_obj->columnCount() - 1))
        return;

    m_tablewidget_obj->clearSelection();

    QList <QAction *> act_list = mainMenu->actions();

    for(int i = 0 ; i < act_list.size() ; i++)
    {
        if(act_list.at(i)->text().trimmed() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
            continue;
        else
            act_list.at(i)->setDisabled(true);
    }

    m_tablewidget_obj->item(row,column)->setSelected(true);
}

void story_board_loader::pub_story_board_search_clicked(QString search_text)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    searched_keyword_str = search_text;
    QStringList search_string_list = search_text.split(",", Qt::SkipEmptyParts);

    emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

    QTableWidget *current_table_widget;

    if(tabwidget_storyboard_obj->currentIndex() == 0)
        current_table_widget = m_tablewidget_obj;
    else if(tabwidget_storyboard_obj->currentIndex() == 1)
        current_table_widget = m_tablewidget_timeline_obj;
    else
        return;


    /// hide every row first
    for(int row = 0; row < current_table_widget->rowCount(); row++)
    {
        current_table_widget->hideRow(row);
    }


    for(int count = 0; count < search_string_list.size(); count++)
    {
        search_text = search_string_list.at(count);

        for(int row = 0; row < current_table_widget->rowCount(); row++)
        {
            int column = 0;
            while(column != current_table_widget->columnCount())
            {
                QTableWidgetItem *item = current_table_widget->item(row , column);

                if(item == NULL)
                {
                    column++;
                    continue;
                }
                else if(item->text().trimmed().isEmpty())
                {
                    column++;
                    continue;
                }
                else
                {
                    if(item->text().toLower().contains(search_text.toLower()))
                        current_table_widget->showRow(row);

                    column++;
                }

            }
        }
    }

    if(tabwidget_storyboard_obj->currentIndex() == 0)
        populate_data_in_timeline_display();


    emit signal_clear_all_metadata();
    emit signal_statusbar_progress_bar_show_hide(false , QString(""));

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void story_board_loader::pub_story_board_show_all_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    m_tablewidget_obj->setRowCount(0);
    searched_keyword_str.clear();

    emit signal_show_all_clicked();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

int story_board_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
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

    if(bool_clear_table_display)
        m_tablewidget_obj->setRowCount(0);


    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(0, destination_db_path, QStringList(), Q_FUNC_INFO);
    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,0);
    int record_no_position = record_no_index;

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int count = m_tablewidget_obj->rowCount();

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);

    pub_refresh_horizontal_header();

    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        total_covered_count++;
        if(total_record_count > 0)
        {
            if(total_covered_count % progressbar_increment_step == 0)
            {
                float record_percent = ((total_covered_count*100)/total_record_count);
                emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
            }
        }
        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            /// for feature icons
            if(pos == 0)
            {
                //                QTableWidgetItem *item_icon = new QTableWidgetItem;
                //                QIcon icon(current_icon_path_str);
                //                item_icon->setIcon(icon);
                //                m_tablewidget_obj->setItem(count, pos, item_icon);

                QTableWidgetItem *item_tablewidget = new QTableWidgetItem;

                if(current_icon_path_str.endsWith(QString(MACRO_Tag_Name_Bookmarks).replace(" " , "_").toLower() + QString(".png")))
                {
                    QChar uni_char(0x25C9);
                    item_tablewidget = new QTableWidgetItem(QString().append(uni_char));
                }
                else
                {
                    QIcon icon(current_icon_path_str);
                    item_tablewidget->setIcon(icon);
                }
                m_tablewidget_obj->setItem(count,pos,item_tablewidget);


            }
            else if(pos == record_no_position)
            {
                /// for bookmarks checkbox
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
                {
                    QIcon iconNotes("../icons/tag_icons/bookmarks.png");
                    item_tablewidget_bookmark->setIcon(iconNotes);

                    item_tablewidget_bookmark->setCheckState(Qt::Checked);
                }
                else
                {
                    QIcon icon("");
                    item_tablewidget_bookmark->setIcon(icon);
                    item_tablewidget_bookmark->setCheckState(Qt::Unchecked);
                }

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

                QIcon icon(tag_icon_str);
                item_tablewidget_tags->setIcon(icon);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_tags);
            }
            else if(pos == blank_column_index)
            {
                QTableWidgetItem *item_tablewidget_blank = new QTableWidgetItem;
                item_tablewidget_blank->setText("");
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_blank);
            }
            else if(pos == screenshot_date_creation_index)
            {
                if(query_index.value(plugin_name_index).toString().trimmed() == QString(MACRO_Plugin_Name_Screenshots)
                        || query_index.value(plugin_name_index).toString().trimmed() == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
                        || query_index.value(plugin_name_index).toString().trimmed() == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger)
                        ||query_index.value(plugin_name_index).toString().trimmed() == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
                    dataStr_from_db = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db.trimmed() , Q_FUNC_INFO);


                QTableWidgetItem *item_tablewidget_blank = new QTableWidgetItem;
                item_tablewidget_blank->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_blank);
            }
            else if(pos == file_path_index)
            {
                if(query_index.value(plugin_name_index).toString().trimmed() == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
                {
                    QString tmp_str = dataStr_from_db.trimmed();
                    QString recon_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                    if(!tmp_str.startsWith(recon_result_dir))
                        tmp_str = recon_static_functions::get_complete_file_path(recon_result_dir, tmp_str, Q_FUNC_INFO);

                    QString tmln_saved_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString();
                    if(tmp_str.startsWith(tmln_saved_dir))
                    {
                        tmp_str.remove(0 , tmln_saved_dir.size());
                        dataStr_from_db = tmp_str;
                    }
                }

                QTableWidgetItem *item_tablewidget = new QTableWidgetItem;
                item_tablewidget->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget);
            }
            else if(pos == tag_icon_index)
            {

                //                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;
                //                QIcon icon(tag_icon_str);
                //                item_tablewidget_tags->setIcon(icon);

                //                m_tablewidget_obj->setItem(count, pos, item_tablewidget_tags);

                QChar uni_char_1(0x25CF);
                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem(QString().append(uni_char_1));

                QTableWidgetItem *item_tags = new QTableWidgetItem();

                item_tags = item_tablewidget_tags;

                if(tag_icon_str.endsWith("red.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(Qt::red));
                }
                else if(tag_icon_str.endsWith("blue.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(Qt::blue));
                }
                else if(tag_icon_str.endsWith("yellow.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(Qt::yellow));
                }
                else if(tag_icon_str.endsWith("green.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(Qt::green));
                }
                else if(tag_icon_str.endsWith("aqua.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(0,255,255));
                }
                else if(tag_icon_str.endsWith("black.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(0,0,0));
                }
                else if(tag_icon_str.endsWith("fuchsia.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(255,0,255));
                }
                else if(tag_icon_str.endsWith("gray.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(128,128,128));
                }
                else if(tag_icon_str.endsWith("lime.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(0,255,0));
                }
                else if(tag_icon_str.endsWith("maroon.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(128,0,0));
                }
                else if(tag_icon_str.endsWith("navy_blue.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(0,0,128));
                }
                else if(tag_icon_str.endsWith("olive.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(128,128,0));
                }
                else if(tag_icon_str.endsWith("purple.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(128,0,128));
                }
                else if(tag_icon_str.endsWith("silver.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(220,220,220));
                }
                else if(tag_icon_str.endsWith("teal.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(0,128,128));
                }
                else if(tag_icon_str.endsWith("skin.png"))
                {
                    item_tablewidget_tags->setForeground(QColor(229,194,192));
                }
                else
                {
                    QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;
                    item_tags = item_tablewidget_tags;
                    QIcon icon(tag_icon_str);
                    item_tablewidget_tags->setIcon(icon);
                }

                QFont item_font = item_tags->font();
                item_font.setPointSize(25);
                item_tags->setFont(item_font);
                m_tablewidget_obj->setItem(count,pos,item_tags);




            }
            else if(pos == notes_value_index)
            {
                //- No entry in the table widget item
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

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}


void story_board_loader::slot_storyboard_tabbar_clicked(int index)
{
    if(index != 0)
    {
        stacked_widget_obj->setCurrentIndex(enum_no_preview_index);
    }
}

void story_board_loader::slot_checkbox_blurr_status(bool status)
{
    bool_checkbox_blurr_status = status;
    story_board_textedit_obj->pub_set_blurr_status(status);

}

void story_board_loader::slot_tags_combobox_index_changed()
{
    stacked_widget_obj->setCurrentIndex(enum_no_preview_index);
}

void story_board_loader::populate_data_in_timeline_display()
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    m_tablewidget_timeline_obj->hideColumn(enum_timeline_source_count_name_column);

    m_tablewidget_timeline_obj->setSortingEnabled(false);
    m_tablewidget_timeline_obj->setRowCount(0);

    QString plugin_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";
    QStringList timeline_db_path_list;
    timeline_db_path_list << plugin_timeline_db_path;

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);

        QString fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + target_struct_obj.source_count_name + ".sqlite";

        if(!QFileInfo(fs_timeline_db_path).exists())
            continue;

        timeline_db_path_list << fs_timeline_db_path;
    }

    qint64 timeline_row = 0;
    QString command_str;

    for(int count = 0; count < timeline_db_path_list.size(); count++)
    {

        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(timeline_db_path_list.at(count));
        if(!source_db.open())
        {
            recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        QSqlQuery query(source_db);


        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {

            if(m_tablewidget_obj->isRowHidden(row))
                continue;

            QString plugin_str    = m_tablewidget_obj->item(row , plugin_name_index)->text().trimmed();
            QString tab_name_str  = m_tablewidget_obj->item(row , tab_name_index)->text().trimmed();
            QString record_no_str = m_tablewidget_obj->item(row , record_no_index)->text().trimmed();
            QString source_count_name = m_tablewidget_obj->item(row , source_count_name_index)->text().trimmed();

            if(record_no_str.isEmpty() || plugin_str.isEmpty() || tab_name_str.isEmpty())
                continue;

            command_str = QString("Select stamp_name,stamp_type,timestamp,item1,item2 , source_count_name From timeline_data Where "
                                  "plugin=? AND category=? AND record_no=? AND source_count_name = ?");


            query.prepare(command_str);

            query.addBindValue(plugin_str);
            query.addBindValue(tab_name_str);
            query.addBindValue(record_no_str);
            query.addBindValue(source_count_name);

            if(!query.exec())
            {
                recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
                continue;
            }

            while(query.next())
            {


                QString timestamp_str  =  query.value(2).toString().trimmed();

                if(timestamp_str.trimmed().isEmpty() || timestamp_str.trimmed() == QString::number(0))
                    continue;

                timestamp_str  =  global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(timestamp_str, Q_FUNC_INFO);

                QString stamp_name_str =  query.value(0).toString().trimmed();
                QString stamp_type_str =  query.value(1).toString().trimmed();
                QString item_1_str     =  query.value(3).toString().trimmed();
                QString item_2_str     =  query.value(4).toString().trimmed();

                QString source_count_name_str     =  query.value("source_count_name").toString().trimmed();

                m_tablewidget_timeline_obj->insertRow(timeline_row);
                m_tablewidget_timeline_obj->setRowHeight(timeline_row, 22);

                QTableWidgetItem *item_stamp_type = new QTableWidgetItem;
                QTableWidgetItem *item_timestamp  = new QTableWidgetItem;
                QTableWidgetItem *item_item_1 = new QTableWidgetItem;
                QTableWidgetItem *item_item_2 = new QTableWidgetItem;

                QTableWidgetItem *item_record_no = new QTableWidgetItem;
                QTableWidgetItem *item_plugin    = new QTableWidgetItem;
                QTableWidgetItem *item_category  = new QTableWidgetItem;
                QTableWidgetItem *item_source_count_name_str  = new QTableWidgetItem;

                item_stamp_type->setText(stamp_type_str);
                item_stamp_type->setToolTip(stamp_name_str);

                item_timestamp->setText(timestamp_str);
                item_item_1->setText(item_1_str);
                item_item_2->setText(item_2_str);

                item_plugin->setText(plugin_str);
                item_category->setText(tab_name_str);
                item_record_no->setText(record_no_str);
                item_source_count_name_str->setText(source_count_name_str);


                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_type_column, item_stamp_type);
                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_timestamp_column, item_timestamp);
                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_item_1_column, item_item_1);
                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_item_2_column, item_item_2);

                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_plugin_column, item_plugin);
                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_category_column, item_category);
                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_record_no_column, item_record_no);
                m_tablewidget_timeline_obj->setItem(timeline_row, enum_timeline_source_count_name_column, item_source_count_name_str);

                timeline_row++;

            }
        }

        source_db.close();
    }

    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_type_column , 100);
    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_timestamp_column , 200);
    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_item_1_column , 200);
    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_item_2_column , 200);
    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_plugin_column , 160);
    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_category_column , 160);
    m_tablewidget_timeline_obj->setColumnWidth(enum_timeline_record_no_column , 100);

    m_tablewidget_timeline_obj->setSortingEnabled(false);


    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

}

void story_board_loader::slot_sort_tablewidget_display(int column_index)
{
    return;

    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    if(m_tablewidget_obj == NULL)
        return;


    ///record column index is 2
    if(column_index != 0 && column_index != 1)
    {
        m_tablewidget_obj->sortByColumn(column_index,Qt::AscendingOrder);
        m_tablewidget_obj->setSortingEnabled(false);
        return;
    }

    m_tablewidget_obj->clearSelection();
    if(m_tablewidget_obj->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_tablewidget_obj->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->item(row, column_index) == NULL)
                continue;

            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column_index)->icon();

            if((icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                icon_shiftrow_acs =  m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->icon();

                // If row is already on 0th position then increment and continue
                if((icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
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
            if(m_tablewidget_obj->item(row, column_index) == NULL)
                continue;

            if(m_tablewidget_obj->item(row_shift_pos_descending, column_index) == NULL)
                continue;

            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column_index)->icon();

            if((icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                icon_shiftrow_desc =  m_tablewidget_obj->item(row_shift_pos_descending, column_index)->icon();

                // If row is already on last position then decrement and continue
                if((icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
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


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void story_board_loader::pub_set_feature_icon_path(QString icon_str)
{
    current_icon_path_str =  icon_str;

}

void story_board_loader::slot_act_quick_look_triggered()
{
    emit signal_act_quick_look_triggered();
}


void story_board_loader::slot_timeline_tablewidget_selection_changed()
{
    if(tabwidget_storyboard_obj->currentIndex() == 1)
    {
        m_tablewidget_obj->clearSelection();

        QModelIndexList selection_model_list = m_tablewidget_timeline_obj->selectionModel()->selectedRows();

        if(selection_model_list.size() > 1)
            return;

        for(int i = 0; i < selection_model_list.size(); i++)
        {
            if(i % 100 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(i);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tablewidget_timeline_obj->rowCount())
                continue;

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString record_no_timeline       = m_tablewidget_timeline_obj->item(selected_row_no, 2)->text().trimmed();
            QString plugin_name_str_timeline = m_tablewidget_timeline_obj->item(selected_row_no, 3)->text().trimmed();
            QString tab_name_str_timeline    = m_tablewidget_timeline_obj->item(selected_row_no, 4)->text().trimmed();
            QString source_count_name_timeline   = m_tablewidget_timeline_obj->item(selected_row_no, enum_timeline_source_count_name_column)->text().trimmed();

            for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
            {
                QString record_no = m_tablewidget_obj->item(row, record_no_index)->text().trimmed();
                QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
                QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text().trimmed();
                QString source_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();

                if(record_no == record_no_timeline && plugin_name_str == plugin_name_str_timeline && tab_name_str_timeline == tab_name_str && source_count_name_timeline == source_name)
                {
                    m_tablewidget_obj->selectRow(row);
                    break;
                }
            }
        }

        tablewidget_general_clicked(m_tablewidget_obj->currentRow(), m_tablewidget_obj->currentColumn());
    }
}

QByteArray story_board_loader::get_image_hex_data(QString filepath)
{
    QByteArray data_arr;

    if(filepath.trimmed().isEmpty())
        return data_arr;

    QImage img(filepath);
    img = img.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if(!bool_checkbox_blurr_status)
    {
        QByteArray arr;
        QBuffer buf(&arr);
        img.save(&buf, "JPG" );
        data_arr = arr.toBase64();
        return data_arr;
    }
    else
    {

        QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;

        effect->setBlurRadius(8);
        int extent = 0;

        QGraphicsScene scene;
        QGraphicsPixmapItem item;
        item.setPixmap(QPixmap::fromImage(img));
        item.setGraphicsEffect(effect);
        scene.addItem(&item);
        QImage res(img.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
        res.fill(Qt::transparent);
        QPainter ptr(&res);
        scene.render(&ptr, QRectF(), QRectF(-extent, -extent, img.width()+extent*2, img.height()+extent*2));

        QByteArray arr;
        QBuffer buf(&arr);
        res.save(&buf, "JPG" );
        data_arr = arr.toBase64();

        delete effect;

        return data_arr;
    }

}

void story_board_loader::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();
    export_rsync_obj->pub_cancel_rsync_copy(Q_FUNC_INFO);

}

void story_board_loader::slot_checkBox_censor_skintone_tag_images_clicked(bool status)
{
    bool_checkbox_censor_skintone_tag_image_status = status;
}

void story_board_loader::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tabwidget_storyboard_obj->currentIndex() == 0)
    {
        custom_header_widgets_QList.clear();
        current_table_widget = m_tablewidget_obj;
        custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(MACRO_CASE_TREE_Story_Board);
    }
    else if(tabwidget_storyboard_obj->currentIndex() == 1)
    {
        custom_header_widgets_QList.clear();
        current_table_widget = m_tablewidget_timeline_obj;
        custom_header_widgets_QList = custom_header_widgets_timeline_QList;
    }

    QList<struct_search_column_text_for_story_board> search_text_col_QList;


    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_story_board obj;
            obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            obj.column_number = i;

            search_text_col_QList.append(obj);
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

        display_loading_progress_bar_obj->hide();
        return;
    }

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

void story_board_loader::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void story_board_loader::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tabwidget_storyboard_obj->currentIndex() == 0)
    {
        current_table_widget = m_tablewidget_obj;
    }
    else if(tabwidget_storyboard_obj->currentIndex() == 1)
    {
        current_table_widget = m_tablewidget_timeline_obj;
    }

    if(column_sort_order == 0)
        current_table_widget->sortItems(column, Qt::DescendingOrder);
    else
        current_table_widget->sortItems(column, Qt::AscendingOrder);

    current_table_widget->horizontalHeader()->hide();
    current_table_widget->horizontalHeader()->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void story_board_loader::all_column_text_search(QList<struct_search_column_text_for_story_board> search_text_all_column_list)
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
            struct_search_column_text_for_story_board obj = search_text_all_column_list.at(column_count);

            if(!(current_table_widget->item(row, obj.column_number)->text().trimmed().contains(obj.search_text, Qt::CaseInsensitive)))
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
