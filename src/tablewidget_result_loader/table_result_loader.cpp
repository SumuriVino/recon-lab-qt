#include "table_result_loader.h"

table_result_loader::table_result_loader(QWidget *parent) : QWidget(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    //table_result_csv_obj = new csv_reader(this);
    add_notes_to_singal_record_obj = new add_notes(this);
    add_notes_to_bookmarks_obj = new add_notes(this);

    ///---right click actions
    mainMenu = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    add_tags_module_obj = new add_tags(this);
    remove_multiple_tag_selecotor_obj = new remove_multiple_tag_selecotor(this);

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    hashset_display_obj = new hashset_display(this);

    bool_display_table_data_loaded = false;
    total_record_count = 0;
    total_covered_count = 0;
    progressbar_increment_step = 0;

    hashset_storage_obj = new hashset_storage(this);
    connect(hashset_storage_obj, SIGNAL(signal_hashset_storage_workdone_clicked(QStringList, QString)), this, SLOT(slot_hashset_storage_workdone_clicked(QStringList, QString)));

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_main_menu_triggered(QAction*)));
    connect(add_notes_to_bookmarks_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_add_notes_to_bookmark_pushbutton_clicked(QString)));
    connect(add_notes_to_singal_record_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_add_notes_pushbutton_clicked(QString)));

    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_submenu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_remaining_tags_list(QStringList, QString , QString )),this,SLOT(slot_remaining_tags_list(QStringList, QString , QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_tag_list_to_be_removed(QStringList, QString , QString)),this,SLOT(slot_tag_list_to_be_removed(QStringList, QString , QString)));
    connect(this, SIGNAL(signal_add_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    bool_sort_ascending = true;

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false; display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);


    bool_virtual_export = false;

    export_rsync_obj = new export_rsync(this);

    export_dialog_box_2_obj = new export_dialog_box_2(this);

    connect(export_dialog_box_2_obj, SIGNAL(signal_save_results_by_export_window(QString , QString, QString , QString)), this, SLOT(slot_save_results_by_export_as_csv_sqlite(QString , QString, QString, QString)));


}

table_result_loader::~table_result_loader()
{
}

void table_result_loader::contextMenuEvent(QContextMenuEvent *evt)
{
}

void table_result_loader::pub_create_ui()
{

    main_tab_widget_obj = new QTabWidget(this);

    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);

    QStringList total_tablist = global_csv_reader_class_selected_obj->get_total_tab_list(plugin_name);
    if(total_tablist.size() > 0)
    {
        first_tab_name = total_tablist.at(0);
    }
    else
    {
        first_tab_name = "Files";
    }

    main_tab_widget_obj->addTab(m_tablewidget_obj, first_tab_name);

    main_vbox = new QVBoxLayout(this);
    hbox_table_widget_extra_widgets = new QHBoxLayout(this);
    hbox_table_widget_extra_widgets->setContentsMargins(0,0,0,0);
    hbox_table_widget_extra_widgets->setSpacing(0);

    upper_widgets_hbox = new QHBoxLayout(this);

    lineEdit_search = new QLineEdit(this);
    lineEdit_search->setMinimumWidth(200);
    lineEdit_search->setMaximumWidth(220);
    lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,false);

    QFont m_font_lineedit_search = lineEdit_search->font();
    m_font_lineedit_search.setPointSize(12);
    lineEdit_search->setFont(m_font_lineedit_search);

    pushButton_search = new QPushButton(this);
    pushButton_search->setText("Search");
    QFont m_font_pushbutton_search = pushButton_search->font();
    m_font_pushbutton_search.setPointSize(12);
    pushButton_search->setFont(m_font_pushbutton_search);

    pushbutton_show_all = new QPushButton(this);
    pushbutton_show_all->setText("Show All");
    QFont m_font_pushbutton_show_all = pushbutton_show_all->font();
    m_font_pushbutton_show_all.setPointSize(12);
    pushbutton_show_all->setFont(m_font_pushbutton_show_all);

    lineEdit_search->hide();
    pushButton_search->hide();
    pushbutton_show_all->hide();
    pushbutton_show_column = new QPushButton("Show Column",this);
    pushbutton_show_column->hide();
    QFont m_font_pushbutton_column_view= pushbutton_show_column->font();
    m_font_pushbutton_column_view.setPointSize(12);
    pushbutton_show_column->setFont(m_font_pushbutton_column_view);

    upper_widgets_hbox->addWidget(pushbutton_show_column);

    upper_widgets_hbox->addWidget(lineEdit_search);
    upper_widgets_hbox->addWidget(pushButton_search);
    upper_widgets_hbox->addWidget(pushbutton_show_all);
    upper_widgets_hbox->addStretch();
    upper_widgets_hbox->setSpacing(10);
    pushButton_export_as = new QPushButton(this);
    pushButton_export_as->setText(MACRO_Generic_Export_As);
    QFont m_font_pushbutton_export = pushButton_export_as->font();
    m_font_pushbutton_export.setPointSize(12);
    pushButton_export_as->setFont(m_font_pushbutton_export);

    upper_widgets_hbox->addWidget(pushButton_export_as);

    menu_export_as = new QMenu(this);
    QStringList export_menuList;
    export_menuList << QString(MACRO_Generic_Export_As_CSV)
                    << QString(MACRO_Generic_Export_As_SQLite);

    create_right_click_actions(menu_export_as,export_menuList);

    pushButton_export_as->setMenu(menu_export_as);
    connect(menu_export_as, SIGNAL(triggered(QAction*)), this, SLOT(slot_menu_export_as_clicked(QAction*)));

    pushButton_export_as->hide();

    if(plugin_name == QString(MACRO_Plugin_Name_Tag_Search) || plugin_name == QString(MACRO_Tag_Name_Bookmarks))
    {
        pushButton_export_as->show();
    }



    main_vbox->addItem(upper_widgets_hbox);
    hbox_table_widget_extra_widgets->addWidget(main_tab_widget_obj);
    main_vbox->addLayout(hbox_table_widget_extra_widgets);
    main_vbox->setStretch(0,0);
    main_vbox->setStretch(1,1);

    main_vbox->setContentsMargins(4,4,4,4);
    main_vbox->setSpacing(0);

    this->setLayout(main_vbox);

    if(plugin_name == QString(MACRO_Plugin_Name_File_System))
    {
        pushbutton_show_column->show();
    }
    else
    {
        pushbutton_show_column->hide();
    }

    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));

    connect(pushButton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj,SIGNAL(signal_spacebar_quicklook_pressed()),this,SIGNAL(signal_act_quick_look_triggered()));

    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect(lineEdit_search,SIGNAL(returnPressed()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(pushbutton_show_all,SIGNAL(clicked()),this,SLOT(slot_pushbutton_showall_clicked()));

    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));
}

void table_result_loader::pub_set_case_tree_obj(R_Treewidget *obj)
{
    treewidget_case_display_obj = obj;
}

void table_result_loader::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void table_result_loader::pub_set_recon_case_obj(RECON_case  *obj)
{
    recon_case_obj = obj;
}

void table_result_loader::pub_set_global_recon_file_info_obj(global_recon_file_info *obj)
{
    global_recon_file_info_obj = obj;
}





void table_result_loader::pub_set_display_tab_name_list(QStringList tabs_list)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    plugin_and_tab_name_list_for_csv.clear();
    QString plugin_and_tab_name;
    plugin_and_tab_name.append(plugin_name);
    plugin_and_tab_name.append(QString(MACRO_RECON_Splitter_1_multiple));

    for(int i =0; i < tabs_list.size(); i++)
    {
        plugin_and_tab_name.append(tabs_list.at(i));
        plugin_and_tab_name.append(QString(MACRO_RECON_Splitter_2_hash_in_brace));
    }

    if(plugin_and_tab_name.endsWith(QString(MACRO_RECON_Splitter_2_hash_in_brace)))
        plugin_and_tab_name.remove(plugin_and_tab_name.size(),QString(MACRO_RECON_Splitter_2_hash_in_brace).size());

    plugin_and_tab_name_list_for_csv << plugin_and_tab_name;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::pub_set_essentials()
{
    result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    hashset_display_obj->pub_set_essentials();
}

void table_result_loader::pub_set_feature_name(QString name)
{
    feature_name = name;
}

void table_result_loader::pub_set_plugin_name(QString name)
{
    plugin_name = name;
}

void table_result_loader::pub_set_clicked_item_tab_text(QString tab_text)
{
    clicked_item_tab_text = tab_text;
}

void table_result_loader::pub_set_csv_dir_path(QString path)
{
    csv_dir_path = path;
}

void table_result_loader::pub_set_table_display_essentials()
{

    fill_display_essentials(plugin_and_tab_name_list_for_csv);

    set_table_header(display_header_name_list , table_headers_width_list);

    connect(m_tablewidget_obj, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_main_table_widget_item_clicked(QTableWidgetItem*)));

}

int table_result_loader::set_table_header(QStringList list_headers , QStringList headersWidth)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(list_headers.isEmpty())
        return -1;


    custom_header_widgets_QList.clear();
    tab_custom_header_widgets_QMap.clear();

    tab_custom_header_widgets_for_redefine_result_Messenger_QMap.clear();
    tab_custom_header_widgets_for_redefine_result_browser_history_QMap.clear();

    if(list_headers.size() != headersWidth.size())
        return -1;

    m_tablewidget_obj->setColumnCount(list_headers.size());

    QStringList empty_header_stringlist;

    for(int column_count = 0; column_count < list_headers.size(); column_count++)
    {
        QString column_name = list_headers.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;

        if(plugin_name == MACRO_CASE_TREE_Artifacts_Keyword_Search)
        {
            custom_table_header_widget_obj->pub_hide_search_field();
            custom_table_header_widget_obj->pub_hide_empty_label();
            m_tablewidget_obj->horizontalHeader()->setMinimumHeight(35);
        }

        custom_table_header_widget_obj->pub_set_column_name(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

        m_tablewidget_obj->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        empty_header_stringlist << "";

        connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
        connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
        connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));

        custom_header_widgets_QList.append(custom_table_header_widget_obj);

        if(column_count ==  enum_tablewidget_column_bookmark && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();

            if(plugin_name == QString(MACRO_Plugin_Name_Tag_Search) || plugin_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
                    || plugin_name == QString(MACRO_Tag_Name_Bookmarks) || plugin_name == QString(MACRO_Plugin_Name_Saved_Maps)
                    || plugin_name == QString(MACRO_Plugin_Name_Screenshots))
            {
                continue;
            }

            QPixmap bookmark("../icons/signs/bookmark.png");
            custom_table_header_widget_obj->pub_set_icon_on_bookmark_or_seen(bookmark);
        }
        else if(column_count ==  enum_tablewidget_column_notes && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        else if(column_count ==  enum_tablewidget_column_tags && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }

        if(feature_name == MACRO_CASE_TREE_Story_Board)
        {
            tab_custom_header_widgets_QMap.insert(feature_name,custom_header_widgets_QList);
        }

        if(feature_name == MACRO_CASE_TREE_Redefined_Result_Parent)
        {
            if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
            {
                tab_custom_header_widgets_for_redefine_result_Messenger_QMap.insert("Event View",custom_header_widgets_QList);
            }

            if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
            {
                tab_custom_header_widgets_for_redefine_result_browser_history_QMap.insert("Event View",custom_header_widgets_QList);
            }
        }
    }

    m_tablewidget_obj->setHorizontalHeaderLabels(empty_header_stringlist);

    for(int ii = 0; ii < headersWidth.size(); ii++)
    {
        if(((custom_table_header_widget *)custom_header_widgets_QList.at(ii))->pub_get_column_name().isEmpty())
        {
            //m_tablewidget_obj->setColumnWidth(ii, headersWidth.at(ii).toInt());
            int column_width = headersWidth.at(ii).toInt();
            column_width = column_width + 10;
            m_tablewidget_obj->setColumnWidth(ii, column_width);
        }
        else
        {
            int column_width = headersWidth.at(ii).toInt();
            column_width = column_width + 25;
            m_tablewidget_obj->setColumnWidth(ii, column_width);
        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return 0;
}


///for creating actions on right click
void table_result_loader::create_right_click_actions(QMenu *mainMenu , QStringList submenuList)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    for(int i = 0; i < submenuList.size(); i++)
    {
        QAction *act_submenu = new QAction(submenuList.at(i).toLocal8Bit(), this);

        if(submenuList.at(i).isEmpty())
            act_submenu->setSeparator(true);

        mainMenu->addAction(act_submenu);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

QAction* table_result_loader::create_and_get_right_click_action(QString submenuStr ,QMenu *mainMenu)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QAction *action = new QAction(mainMenu);
    action = mainMenu->addAction(tr(submenuStr.toLocal8Bit().data()));

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    action->setFont(m_font);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return action;
}

/// for creating menu on right click
QMenu* table_result_loader::create_right_click_submenu(QString submenuStr ,QMenu *mainMenu , QMenu *subMenu)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    subMenu = mainMenu->addMenu(tr(submenuStr.toLocal8Bit().data()));

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    subMenu->setFont(m_font);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return subMenu;
}

void table_result_loader::create_tags_submenu()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_tags == NULL)
        return;

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));


    ///remove tag
    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    sub_menu_tags->addAction(act_remove_tag);

    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags()));
    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                sub_menu_tags->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";
            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);
}

void table_result_loader::fill_display_essentials(QStringList associated_plugins_and_tabs_list)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_header_name_list.clear();
    db_columns_list.clear();
    table_name_list.clear();
    table_headers_width_list.clear();
    table_display_order_list.clear();


    st_result_loader_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);

    ///display tab names

    if(st_result_loader_csv_info_obj.display_header_QList_tablewidget.size() > 0)
    {
        display_header_name_list = st_result_loader_csv_info_obj.display_header_QList_tablewidget.at(0);
        //        if(display_header_name_list.at(display_header_name_list.size() -1) == "")
        //        {
        //            display_header_name_list.removeAt(display_header_name_list.size() -1);
        //        }
    }


    /// tab names in Database
    if(st_result_loader_csv_info_obj.display_column_name_QList_tablewidget.size() > 0)
    {
        db_columns_list = st_result_loader_csv_info_obj.display_column_name_QList_tablewidget.at(0);

        if(db_columns_list.at(db_columns_list.size() -1) == MACRO_DISPLAY_LAST_BLANK_COLUMN)
        {
            db_columns_list.removeAt(db_columns_list.size() -1);
        }

    }

    /// database table name
    table_name_list = st_result_loader_csv_info_obj.table_name_QStringList_tablewidget;

    /// Headers width from CSV
    if(st_result_loader_csv_info_obj.display_col_width_QList_tablewidget.size() > 0)
    {
        table_headers_width_list = st_result_loader_csv_info_obj.display_col_width_QList_tablewidget.at(0);
        //        if(table_headers_width_list.at(table_headers_width_list.size() -2) == "0")
        //        {
        //            table_headers_width_list.removeAt(table_headers_width_list.size() -2);
        //        }
    }

    /// Display Order from CSV
    if(st_result_loader_csv_info_obj.display_order_QList_tablewidget.size() > 0)
        table_display_order_list = st_result_loader_csv_info_obj.display_order_QList_tablewidget.at(0);


    /// Display Order from CSV
    if(st_result_loader_csv_info_obj.display_data_type_QList_tablewidget.size() > 0)
        display_column_data_type_list = st_result_loader_csv_info_obj.display_data_type_QList_tablewidget.at(0);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


void table_result_loader::create_default_right_click()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark , menuList);

    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
    create_tags_submenu();

    menuList.clear();
    menuList << QString("") << QString(MACRO_Generic_Right_Click_Add_Note)
             << QString(MACRO_Generic_Right_Click_Remove_Note);

    submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
    create_right_click_actions(submenu_note , menuList);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    act_quick_look = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Quick_look),mainMenu);

    // Open With
    QStringList submenuList;
    submenuList << QString(MACRO_CASE_TREE_Plist_Viewer)
                << QString(MACRO_CASE_TREE_Hex_Viewer)
                << QString(MACRO_CASE_TREE_Sqlite_Viewer)
                << QString(MACRO_CASE_TREE_Log_Viewer);

    submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
    create_right_click_actions(submenu_open_with , submenuList);

    act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);

    create_right_click_actions(mainMenu , menuList);

    // Send to Bucket
    create_right_click_actions(mainMenu , QStringList(""));

    submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_send_to_bucket);
    create_right_click_actions(submenu_send_to_bucket , submenuList);

    //---------------------------------------------------------

    ///----Create Submenu of existing Menu
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Hashset
    submenu_add_files_to_hashset_db = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database) , mainMenu , submenu_add_files_to_hashset_db);
    QList <QMenu*> hahses_menu_list;
    hahses_menu_list.append(submenu_add_files_to_hashset_db);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(hahses_menu_list, Q_FUNC_INFO);
    create_add_files_to_hashset_submenu();

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Action Export
    act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

    ///----Create Export of existing Menu
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    submenu_go_to_artifact_source = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Go_To_Artifacts_Source) , mainMenu , submenu_go_to_artifact_source);
    act_go_to_record = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Go_To_Record), mainMenu);

    connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));
    connect(submenu_open_with, SIGNAL(triggered(QAction*)), this, SLOT(slot_open_with_submenu_clicked(QAction*)));
    connect(submenu_send_to_bucket, SIGNAL(triggered(QAction*)), this, SLOT(slot_sendtobucket_submenu_clicked(QAction*)));
    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    connect(submenu_add_files_to_hashset_db,SIGNAL(triggered(QAction*)),this,SLOT(slot_add_files_to_hashset_submenu_clicked(QAction*)));
    connect(submenu_go_to_artifact_source,SIGNAL(triggered(QAction*)),this,SLOT(slot_goto_artifact_source_submenu_clicked(QAction*)));

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

int table_result_loader::get_stack_index_for_preview(QString current_media_path,QString display_file_path)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int preview_index_full = 0;
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
                if(info.size() > (100 * 1024 * 1024)) // 100MB 104857600 bytes
                    return preview_index_full;

                preview_index_full = enum_global_preview_stack_widget_IMAGE;
            }
            else if(mime_type_for_file.startsWith("video/") || mime_type_for_file.trimmed().startsWith("audio/"))
            {
                preview_index_full = enum_global_preview_stack_widget_Audio_Video;
            }
        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return preview_index_full;
}

void table_result_loader::tablewidget_general_clicked(int row,int column)
{

}

int table_result_loader::insert_record_in_table_display(QString database_path , QString db_cmd , QStringList tab_list_from_csv, QStringList addbindvalue_list)
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

    /// this is because the notes column will be at last position
    int notes_position = tab_list_from_csv.count();

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
                if(query_index.value(notes_position).toString().trimmed().isEmpty())
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

void table_result_loader::update_notes_value(QString text, int current_row)
{

}



void table_result_loader::update_tags_value(QString tag_data,QString colour)
{}
//{
//    recon_static_functions::debug_intensive(" -Starts " , Q_FUNC_INFO);

//    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

//    for(int i = 0; i < selection_model_list.size(); i++)
//    {
//        if(i % 10 == 0)
//            QCoreApplication::processEvents();

//        QModelIndex model_index = selection_model_list.at(i);

//        int selected_row_no = model_index.row();

//        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
//            continue;

//        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

//        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

//        if(plugin_name_str.isEmpty())
//            continue;



//        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
//        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

//        QString record_no_column_name = db_columns_list.at(record_no_index);
//        QString plugin_column_name = db_columns_list.at(plugin_name_index);
//        QString tab_column_name = db_columns_list.at(tab_name_index);
//        QString os_scheme_column_name = db_columns_list.at(os_name_index);

//        QString table_name;
//        if(table_name_list.size() > 0)
//            table_name = table_name_list.at(0);

//        QString command = "SELECT recon_tag_value from '" + table_name + "' where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
//        QStringList arg_list;
//        arg_list << record_no_str << plugin_name_str << tab_name_str << os_name_str;
//        QString previous_tag = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);


//        previous_tag = previous_tag.trimmed();

//        if(!previous_tag.isEmpty())
//        {
//            QStringList temp_taglist = previous_tag.split(",");
//            if(temp_taglist.contains(tag_data))
//                continue;
//        }

//        QString added_tag_name = tag_data;
//        if(!previous_tag.isEmpty())
//        {
//            added_tag_name.append(",");
//            added_tag_name.append(previous_tag);
//        }

//        command.clear();
//        command = "update '" + table_name + "' set recon_tag_value=? where " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
//        arg_list.clear();
//        arg_list << added_tag_name << record_no_str << plugin_name_str << tab_name_str << os_name_str;
//        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

//        int co_index = display_column_data_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_RECORD,0);
//        tablewidget_general_clicked(selected_row_no,co_index);

//        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,added_tag_name, Q_FUNC_INFO);

//        QIcon icon(icon_path);
//        int tag_position = display_column_data_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_TAG,0);
//        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);

//        if(plugin_name_str == QString(MACRO_File_System_Plugin_Name))
//        {
//            QString result_dir_path = case_narad_muni->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

//            int source_index = MACRO_Source_Count_Name_Column_Index_In_Tag_Search_int;
//            QString source = m_tablewidget_obj->item(selected_row_no, source_index)->text();


//            QString fs_record_INT = record_no_str;

//            QString command = "UPDATE '" + QString("files") + "' SET recon_tag_value = ? " +  " WHERE INT = ?";
//            QStringList arg_list;
//            arg_list  << added_tag_name <<  fs_record_INT;

//            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);

//            emit signal_add_tag_in_other_locations(added_tag_name,"0",plugin_name_str,tab_name_str,record_no_str , fs_db_path, os_name_str);
//        }
//        else
//        {
//            emit signal_add_tag_in_other_locations(added_tag_name,"0",plugin_name_str,tab_name_str,record_no_str,destination_db_path, os_name_str);
//        }
//    }

//    recon_static_functions::debug_intensive(" -Ends " , Q_FUNC_INFO);
//}

void table_result_loader::create_add_files_to_hashset_submenu()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString command = QString("Select database_path From tbl_hashset");

    QStringList hashset_db_path_list;
    hashset_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, db_path, Q_FUNC_INFO);

    if(hashset_db_path_list.size() == 0)
        return;

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);


    bool bool_recon_hashes_db = false;

    for(int count = 0; count < hashset_db_path_list.size(); count++)
    {
        QString path_str = hashset_db_path_list.at(count);

        QString hash_name_str = QFileInfo(path_str).fileName();
        hash_name_str.remove(QFileInfo(path_str).suffix());

        if(hash_name_str.endsWith("."))
            hash_name_str.chop(1);

        QString software_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select name from software"), 0, path_str, Q_FUNC_INFO);
        software_name = software_name.trimmed();
        if(software_name == QString("RECON"))
        {
            if(!bool_recon_hashes_db)
                submenu_add_files_to_hashset_db->clear();

            bool_recon_hashes_db = true;
            QAction *action_add_file = new QAction(hash_name_str, submenu_add_files_to_hashset_db);
            action_add_file->setToolTip(path_str);
            submenu_add_files_to_hashset_db->addAction(action_add_file);
        }

    }

    hashset_storage_obj->pub_set_essentials();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);

}

QStringList table_result_loader::export_file_system_record(QString record_no,int current_row)
{

}

void table_result_loader::create_action_for_goto_artifact_source_submenu()
{

}

void table_result_loader::display_data(QString dest_db_path)
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

int table_result_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
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
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, destination_db_path, QStringList(), Q_FUNC_INFO);
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

    pub_refresh_horizontal_header();

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

void table_result_loader::update_bookmark_value(QString status,int row, int column)
{}
//{
//    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

//    int bookmark_index = display_column_data_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_BOOKMARK,0);
//    QString bookmark_col_name = db_columns_list.at(bookmark_index);

//    if(!table_name_list.isEmpty())
//    {
//        // Because this class support only CSV of one tab only. confirmed by Mj sir
//        QString table_name = table_name_list.at(0);

//        if(bookmark_index != -1)
//        {
//            if(status == QString("1"))
//                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
//            else
//                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
//        }

//        QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();

//        QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();

//        if(plugin_name_str.isEmpty())
//            return;



//        QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();
//        QString os_name_str = m_tablewidget_obj->item(row, os_name_index)->text();

//        QString record_no_column_name = db_columns_list.at(record_no_index);
//        QString plugin_column_name = db_columns_list.at(plugin_name_index);
//        QString tab_column_name = db_columns_list.at(tab_name_index);
//        QString os_scheme_column_name = db_columns_list.at(os_name_index);

//        if(plugin_name_str == MACRO_Plist_Viewer_Plugin_Name)
//        {
//            QString recon_result_dir_path = case_narad_muni->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

//            QString exported_content_info_db = case_narad_muni->get_field(MACRO_NARAD_Content_Export_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

//            QString command = "UPDATE plist_content_info SET bookmark=? WHERE record_no=?";
//            QStringList arg_list;
//            arg_list << status << record_no;

//            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);


//            command = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
//            arg_list.clear();
//            arg_list << record_no << plugin_name_str << tab_name_str << os_name_str;
//            QString applied_tags = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

//            QStringList applied_tags_list = applied_tags.split(",");
//            emit signal_change_bookmark_value_in_other_db(record_no,plugin_name_str,tab_name_str,status,applied_tags_list,exported_content_info_db, os_name_str, feature_name);
//            return;
//        }

//        QString command = "UPDATE '" + table_name + "' SET " + bookmark_col_name + "=?" " WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
//        QStringList arg_list;
//        arg_list << status << record_no << plugin_name_str << tab_name_str << os_name_str;

//        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

//        command = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
//        arg_list.clear();
//        arg_list << record_no << plugin_name_str << tab_name_str << os_name_str;
//        QString applied_tags = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

//        QStringList applied_tags_list = applied_tags.split(",");

//        if(plugin_name_str == QString(MACRO_File_System_Plugin_Name))
//        {
//            QString result_dir_path = case_narad_muni->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

//            int source_index = MACRO_Source_Count_Name_Column_Index_In_Tag_Search_int;

//            QString source = m_tablewidget_obj->item(row, source_index)->text();


//            QString fs_record_INT = record_no;

//            command = "UPDATE '" + QString("files") + "' SET " + bookmark_col_name + " = ?" " WHERE INT = ?";
//            QStringList arg_list;
//            arg_list  << status <<  fs_record_INT;

//            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);


//            emit signal_change_bookmark_value_in_other_db(record_no,plugin_name_str,tab_name_str,status,applied_tags_list,fs_db_path, os_name_str, feature_name);
//            return;
//        }

//        emit signal_change_bookmark_value_in_other_db(record_no,plugin_name_str,tab_name_str,status,applied_tags_list,destination_db_path, os_name_str, feature_name);

//    }

//    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
//}

QString table_result_loader::get_column_text_from_selected_row(qint64 selected_row_no, qint64 col_no)
{
    return m_tablewidget_obj->item(selected_row_no, col_no)->text().trimmed();

}


void table_result_loader::export_data_as_csv(QString filepath)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);
    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        if(selection_model_list.size() <= 0)
        {
            display_loading_progress_bar_obj->hide();
            QMessageBox::information(this,"Export","Please select at least one item");
            return;
        }
    }

    QFile file(filepath);
    if (!file.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("File open -----FAILED----" + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + file.errorString() , Q_FUNC_INFO);
        return;
    }


    /// write data to CSV file
    QStringList header_list;

    /// creating file header
    header_list << "Sr. No.";

    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        QString title_str = m_tablewidget_obj->horizontalHeaderItem(column)->text();

        title_str = title_str.trimmed();
        if(title_str.isEmpty() || column == source_count_name_index || column == os_name_index)
            continue;

        header_list << title_str;
    }

    QString header_str = header_list.join(",");

    file.write(header_str.toLocal8Bit());
    file.write(QString("\n").toLocal8Bit());

    QStringList values_list ;
    QString value_str;

    QStringList output_csv_values_list;

    /// writing data to file
    int count = 0;

    ///Export Screen items
    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            count++;
            if(count % 200 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(m_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            QString sr_no_str = QString::number(row + 1);

            QString record_str = m_tablewidget_obj->item(row , enum_column_record_no_index)->text().trimmed();
            QString plugin_str = m_tablewidget_obj->item(row , enum_column_plugin_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(row , enum_column_category_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(row , enum_column_item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(row , enum_column_item_2_index)->text().trimmed();
            QString item_3_str = m_tablewidget_obj->item(row , enum_column_item_3_index)->text().trimmed();
            QString item_4_str = m_tablewidget_obj->item(row , enum_column_item_4_index)->text().trimmed();
            QString item_5_str = m_tablewidget_obj->item(row , enum_column_item_5_index)->text().trimmed();
            QString item_6_str = m_tablewidget_obj->item(row , enum_column_item_6_index)->text().trimmed();
            QString item_7_str = m_tablewidget_obj->item(row , enum_column_item_7_index)->text().trimmed();
            QString item_8_str = m_tablewidget_obj->item(row , enum_column_item_8_index)->text().trimmed();
            QString item_9_str = m_tablewidget_obj->item(row , enum_column_item_9_index)->text().trimmed();
            QString item_10_str = m_tablewidget_obj->item(row , enum_column_item_10_index)->text().trimmed();
            QString item_11_str = m_tablewidget_obj->item(row , enum_column_item_11_index)->text().trimmed();
            QString item_12_str = m_tablewidget_obj->item(row , enum_column_item_12_index)->text().trimmed();
            QString item_13_str = m_tablewidget_obj->item(row , enum_column_item_13_index)->text().trimmed();
            QString item_14_str = m_tablewidget_obj->item(row , enum_column_item_14_index)->text().trimmed();
            QString item_15_str = m_tablewidget_obj->item(row , enum_column_item_15_index)->text().trimmed();
            QString item_16_str = m_tablewidget_obj->item(row , enum_column_item_16_index)->text().trimmed();
            QString item_17_str = m_tablewidget_obj->item(row , enum_column_item_17_index)->text().trimmed();
            QString item_18_str = m_tablewidget_obj->item(row , enum_column_item_18_index)->text().trimmed();
            QString item_19_str = m_tablewidget_obj->item(row , enum_column_item_19_index)->text().trimmed();
            QString item_20_str = m_tablewidget_obj->item(row , enum_column_item_20_index)->text().trimmed();


            values_list.clear();
            values_list << sr_no_str << record_str << plugin_str  << category_str << item_1_str << item_2_str
                        << item_3_str << item_4_str << item_5_str << item_6_str << item_7_str << item_8_str
                        << item_9_str << item_10_str << item_11_str << item_12_str << item_13_str << item_14_str
                        << item_15_str << item_16_str << item_17_str << item_18_str << item_19_str << item_20_str;

            output_csv_values_list.clear();

            for(int ii = 0; ii < values_list.size() ;ii++)
            {
                QString val_str = values_list.at(ii);
                //This is for string delimiter
                val_str.prepend("\"");
                val_str.append("\"");
                output_csv_values_list << val_str;
            }

            output_csv_values_list << QString("\n");

            value_str = output_csv_values_list.join(",");

            file.write(value_str.toLocal8Bit());
        }
    }
    else if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        ///Export Selected Items
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        for(int row = 0; row < selection_model_list.size(); row++)
        {
            count++;
            if(count % 200 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
                continue;

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString sr_no_str = QString::number(row + 1);

            QString record_str = m_tablewidget_obj->item(selected_row_no , enum_column_record_no_index)->text().trimmed();
            QString plugin_str = m_tablewidget_obj->item(selected_row_no , enum_column_plugin_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(selected_row_no , enum_column_category_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_2_index)->text().trimmed();
            QString item_3_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_3_index)->text().trimmed();
            QString item_4_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_4_index)->text().trimmed();
            QString item_5_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_5_index)->text().trimmed();
            QString item_6_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_6_index)->text().trimmed();
            QString item_7_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_7_index)->text().trimmed();
            QString item_8_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_8_index)->text().trimmed();
            QString item_9_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_9_index)->text().trimmed();
            QString item_10_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_10_index)->text().trimmed();
            QString item_11_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_11_index)->text().trimmed();
            QString item_12_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_12_index)->text().trimmed();
            QString item_13_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_13_index)->text().trimmed();
            QString item_14_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_14_index)->text().trimmed();
            QString item_15_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_15_index)->text().trimmed();
            QString item_16_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_16_index)->text().trimmed();
            QString item_17_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_17_index)->text().trimmed();
            QString item_18_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_18_index)->text().trimmed();
            QString item_19_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_19_index)->text().trimmed();
            QString item_20_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_20_index)->text().trimmed();

            values_list.clear();
            values_list << sr_no_str << record_str << plugin_str  << category_str << item_1_str << item_2_str
                        << item_3_str << item_4_str << item_5_str << item_6_str << item_7_str << item_8_str
                        << item_9_str << item_10_str << item_11_str << item_12_str << item_13_str << item_14_str
                        << item_15_str << item_16_str << item_17_str << item_18_str << item_19_str << item_20_str;

            output_csv_values_list.clear();

            for(int ii = 0; ii < values_list.size() ;ii++)
            {
                QString val_str = values_list.at(ii);
                //This is for string delimiter
                val_str.prepend("\"");
                val_str.append("\"");
                output_csv_values_list << val_str;
            }

            output_csv_values_list << QString("\n");

            value_str = output_csv_values_list.join(",");

            file.write(value_str.toLocal8Bit());
        }

    }

    file.flush();
    file.close();

    display_loading_progress_bar_obj->hide();

    if(!filepath.isEmpty())
        open_exported_file(filepath);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void table_result_loader::export_data_as_sqlite(QString filepath)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);
    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        if(selection_model_list.size() <= 0)
        {
            display_loading_progress_bar_obj->hide();
            QMessageBox::information(this,"Export","Please select at least one item");
            return;
        }
    }

    /// creating file header
    QStringList header_list;
    header_list << "Sr. No.";
    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        QString title_str = m_tablewidget_obj->horizontalHeaderItem(column)->text();

        title_str = title_str.trimmed();
        if(title_str.isEmpty() || column == source_count_name_index || column == os_name_index)
            continue;

        header_list << title_str;
    }


    ///=========Command Table Create and Insert-Start=========//
    QString col_str;
    QString cmd_table_create = "Create Table 'files' (";
    QString cmd_insert = QString("Insert into 'files' (");
    QString cmd_insert_val = QString(" Values(");

    for(int num = 0 ; num < header_list.size(); num++)
    {
        col_str = header_list.at(num);

        col_str.replace(".", "");
        col_str.replace(" ", "_");
        col_str.replace("/", "_");
        col_str.replace("(", "_");
        col_str.replace(")", "");
        col_str.replace("__", "_");

        cmd_table_create.append(col_str);
        cmd_table_create.append(" varchar(1024),");

        cmd_insert.append(col_str);
        cmd_insert.append(",");
        cmd_insert_val.append("?,");

    }
    if(cmd_table_create.endsWith(","))
        cmd_table_create.chop(1);
    cmd_table_create.append(")");

    if(cmd_insert.endsWith(","))
        cmd_insert.chop(1);
    cmd_insert.append(")");

    if(cmd_insert_val.endsWith(","))
        cmd_insert_val.chop(1);
    cmd_insert_val.append(")");

    cmd_insert.append(cmd_insert_val);
    ///=========Command Table Create and Insert-Start=========//

    QString connection_naam_exp_db = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam_exp_db);
    QSqlDatabase dest_export_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_exp_db);
    dest_export_db.setDatabaseName(filepath);
    if(!dest_export_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + dest_export_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + dest_export_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_exp_db);
        return;
    }

    QSqlQuery qry_export_db(dest_export_db);

    qry_export_db.prepare(cmd_table_create);

    if(!qry_export_db.exec())
    {
        recon_static_functions::app_debug(" table create command --------FAILED------ " + qry_export_db.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + qry_export_db.lastError().text(),Q_FUNC_INFO);

        dest_export_db.close();
        QSqlDatabase::removeDatabase(connection_naam_exp_db);
        return;
    }

    dest_export_db.transaction();

    int count = 0;

    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        ///Export Screen Items
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            count++;
            if(count % 200 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(m_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            QString sr_no_str = QString::number(row + 1);

            QString record_str   = m_tablewidget_obj->item(row , enum_column_record_no_index)->text().trimmed();
            QString plugin_str   = m_tablewidget_obj->item(row , enum_column_plugin_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(row , enum_column_category_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(row , enum_column_item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(row , enum_column_item_2_index)->text().trimmed();
            QString item_3_str = m_tablewidget_obj->item(row , enum_column_item_3_index)->text().trimmed();
            QString item_4_str = m_tablewidget_obj->item(row , enum_column_item_4_index)->text().trimmed();

            QString item_5_str = m_tablewidget_obj->item(row , enum_column_item_5_index)->text().trimmed();
            QString item_6_str = m_tablewidget_obj->item(row , enum_column_item_6_index)->text().trimmed();
            QString item_7_str = m_tablewidget_obj->item(row , enum_column_item_7_index)->text().trimmed();
            QString item_8_str = m_tablewidget_obj->item(row , enum_column_item_8_index)->text().trimmed();

            QString item_9_str = m_tablewidget_obj->item(row , enum_column_item_9_index)->text().trimmed();
            QString item_10_str = m_tablewidget_obj->item(row , enum_column_item_10_index)->text().trimmed();
            QString item_11_str = m_tablewidget_obj->item(row , enum_column_item_11_index)->text().trimmed();
            QString item_12_str = m_tablewidget_obj->item(row , enum_column_item_12_index)->text().trimmed();

            QString item_13_str = m_tablewidget_obj->item(row , enum_column_item_13_index)->text().trimmed();
            QString item_14_str = m_tablewidget_obj->item(row , enum_column_item_14_index)->text().trimmed();
            QString item_15_str = m_tablewidget_obj->item(row , enum_column_item_15_index)->text().trimmed();
            QString item_16_str = m_tablewidget_obj->item(row , enum_column_item_16_index)->text().trimmed();

            QString item_17_str = m_tablewidget_obj->item(row , enum_column_item_17_index)->text().trimmed();
            QString item_18_str = m_tablewidget_obj->item(row , enum_column_item_18_index)->text().trimmed();
            QString item_19_str = m_tablewidget_obj->item(row , enum_column_item_19_index)->text().trimmed();
            QString item_20_str = m_tablewidget_obj->item(row , enum_column_item_20_index)->text().trimmed();

            qry_export_db.prepare(cmd_insert);

            qry_export_db.addBindValue(sr_no_str);
            qry_export_db.addBindValue(record_str);
            qry_export_db.addBindValue(plugin_str);
            qry_export_db.addBindValue(category_str);

            qry_export_db.addBindValue(item_1_str);
            qry_export_db.addBindValue(item_2_str);
            qry_export_db.addBindValue(item_3_str);
            qry_export_db.addBindValue(item_4_str);
            qry_export_db.addBindValue(item_5_str);
            qry_export_db.addBindValue(item_6_str);
            qry_export_db.addBindValue(item_7_str);

            qry_export_db.addBindValue(item_8_str);
            qry_export_db.addBindValue(item_9_str);
            qry_export_db.addBindValue(item_10_str);
            qry_export_db.addBindValue(item_11_str);
            qry_export_db.addBindValue(item_12_str);
            qry_export_db.addBindValue(item_13_str);
            qry_export_db.addBindValue(item_14_str);

            qry_export_db.addBindValue(item_15_str);
            qry_export_db.addBindValue(item_16_str);
            qry_export_db.addBindValue(item_17_str);
            qry_export_db.addBindValue(item_18_str);
            qry_export_db.addBindValue(item_19_str);
            qry_export_db.addBindValue(item_20_str);

            if(!qry_export_db.exec())
            {
                recon_static_functions::app_debug("insert command --------FAILED------ " + qry_export_db.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + qry_export_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }
    }
    else if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        ///Export Selected Items
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        for(int row = 0; row < selection_model_list.size(); row++)
        {
            if(row % 100 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
                continue;

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString sr_no_str = QString::number(row + 1);

            QString record_str = m_tablewidget_obj->item(selected_row_no , enum_column_record_no_index)->text().trimmed();
            QString plugin_str = m_tablewidget_obj->item(selected_row_no , enum_column_plugin_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(selected_row_no , enum_column_category_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_2_index)->text().trimmed();
            QString item_3_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_3_index)->text().trimmed();
            QString item_4_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_4_index)->text().trimmed();

            QString item_5_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_5_index)->text().trimmed();
            QString item_6_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_6_index)->text().trimmed();
            QString item_7_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_7_index)->text().trimmed();
            QString item_8_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_8_index)->text().trimmed();

            QString item_9_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_9_index)->text().trimmed();
            QString item_10_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_10_index)->text().trimmed();
            QString item_11_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_11_index)->text().trimmed();
            QString item_12_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_12_index)->text().trimmed();

            QString item_13_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_13_index)->text().trimmed();
            QString item_14_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_14_index)->text().trimmed();
            QString item_15_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_15_index)->text().trimmed();
            QString item_16_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_16_index)->text().trimmed();

            QString item_17_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_17_index)->text().trimmed();
            QString item_18_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_18_index)->text().trimmed();
            QString item_19_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_19_index)->text().trimmed();
            QString item_20_str = m_tablewidget_obj->item(selected_row_no , enum_column_item_20_index)->text().trimmed();

            qry_export_db.prepare(cmd_insert);

            qry_export_db.addBindValue(sr_no_str);
            qry_export_db.addBindValue(record_str);
            qry_export_db.addBindValue(plugin_str);
            qry_export_db.addBindValue(category_str);

            qry_export_db.addBindValue(item_1_str);
            qry_export_db.addBindValue(item_2_str);
            qry_export_db.addBindValue(item_3_str);
            qry_export_db.addBindValue(item_4_str);
            qry_export_db.addBindValue(item_5_str);
            qry_export_db.addBindValue(item_6_str);
            qry_export_db.addBindValue(item_7_str);

            qry_export_db.addBindValue(item_8_str);
            qry_export_db.addBindValue(item_9_str);
            qry_export_db.addBindValue(item_10_str);
            qry_export_db.addBindValue(item_11_str);
            qry_export_db.addBindValue(item_12_str);
            qry_export_db.addBindValue(item_13_str);
            qry_export_db.addBindValue(item_14_str);

            qry_export_db.addBindValue(item_15_str);
            qry_export_db.addBindValue(item_16_str);
            qry_export_db.addBindValue(item_17_str);
            qry_export_db.addBindValue(item_18_str);
            qry_export_db.addBindValue(item_19_str);
            qry_export_db.addBindValue(item_20_str);


            if(!qry_export_db.exec())
            {
                recon_static_functions::app_debug("insert command --------FAILED------ " + qry_export_db.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + qry_export_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }

    }

    dest_export_db.commit();
    dest_export_db.close();

    display_loading_progress_bar_obj->hide();

    if(!filepath.isEmpty())
        open_exported_file(filepath);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void table_result_loader::open_exported_file(QString file_path)
{
    report_status obj;
    if(obj.exec())
    {
        QStringList arg;
        arg << file_path;

        QProcess *myProcess = new QProcess(this);
        myProcess->startDetached("open",arg);
    }
}

void table_result_loader::pub_refresh_horizontal_header()
{
    m_tablewidget_obj->horizontalScrollBar()->setValue(m_tablewidget_obj->horizontalScrollBar()->value() + 10);
    m_tablewidget_obj->horizontalScrollBar()->setValue(m_tablewidget_obj->horizontalScrollBar()->value() - 10);
    m_tablewidget_obj->horizontalHeader()->hide();
    m_tablewidget_obj->horizontalHeader()->show();
}

void table_result_loader::pub_clear_all_custom_header_widget_entries()
{
    for(int ii = 0; ii < custom_header_widgets_QList.size(); ii++)
    {
        custom_header_widgets_QList.at(ii)->pub_clear_line_edit_search_field();
    }

    pub_refresh_horizontal_header();
}
