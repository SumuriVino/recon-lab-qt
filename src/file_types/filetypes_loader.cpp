#include "filetypes_loader.h"

filetypes_loader::filetypes_loader(QWidget *parent) : table_result_loader(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    gallery_viewer_obj = new gallery_viewer(this);

    connect(gallery_viewer_obj, SIGNAL(signal_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_viewer_item_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_clicked(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_invalid_item_clicked()), this, SLOT(slot_gallery_view_invalid_tem_clicked()));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_viewer_item_double_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info)));

    connect(this, SIGNAL(signal_set_bookmark_status_to_gallery_view(bool)), gallery_viewer_obj, SLOT(slot_set_bookmark_status_to_gallery_view(bool)));

    file_export_dialog_obj = new file_export_dialog(this);
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done(bool,QString)), this, SLOT(slot_export_file_dialog_work_done(bool,QString)));
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done_for_zip(QString,bool,QString)), this, SLOT(slot_export_file_dialog_work_done_for_zip(QString,bool,QString)));

    thread_zip_file_export_obj = new thread_zip_file_export;
    connect(thread_zip_file_export_obj, SIGNAL(finished()), this, SLOT(slot_thread_zip_file_export_finished()));


    decompress_file_system_obj = new decompress_file_system(this);
    decompress_file_system_obj->hide();

    export_as_KML_dialog_box_obj = new export_as_KML(this);
    connect(export_as_KML_dialog_box_obj, SIGNAL(signal_cancel_export_as_kml()), this, SLOT(slot_cancel_export_as_KML_report()));
    connect(export_as_KML_dialog_box_obj, SIGNAL(signal_continue_export_as_kml()), this, SLOT(slot_continue_export_as_KML_report()));

    map_bookmarked_records_from_table_display.clear();
    search_files_with_hash_obj = new search_files_with_hash();
    bool_columns_ajust_complete = false;

    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done_for_dmg(bool,QString)), this, SLOT(slot_export_file_dialog_work_done_for_dmg(bool,QString)));

    recon_helper_process_obj = new recon_helper_process(this);

    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

}


filetypes_loader::~filetypes_loader()
{
    if(search_files_with_hash_obj != nullptr)
    {
        delete search_files_with_hash_obj;
    }
}


void filetypes_loader::configure_filetypes_loader(QString result_dir_path , QString filetype , QString case_tree_type_dsply_name, qint64 records_count, QTreeWidgetItem *currentItem)
{ // create the custom ui if file type loader
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    file_type_child   = filetype.trimmed();
    file_type_parent  = case_tree_type_dsply_name.trimmed();

    gallery_viewer_obj->pub_set_file_type_parent_type(case_tree_type_dsply_name.trimmed());
    gallery_viewer_obj->pub_create_ui();

    if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
    {
        main_tab_widget_obj->addTab(gallery_viewer_obj, "Faces");
    }
    else
    {
        main_tab_widget_obj->addTab(gallery_viewer_obj, "Gallery View");
    }


    connect(main_tab_widget_obj, SIGNAL(currentChanged(int)), this, SLOT(slot_main_tab_widget_current_changed(int)));

    //    if(file_type_parent == QString(MACRO_CASE_TREE_CONTENT_SEARCH))
    //    {
    //        prepare_indexed_carved_files_tab_display();
    //        main_tab_widget_obj->addTab(m_table_carved_index_files, QString(MACRO_Plugin_Name_Carved_Files));
    //    }


    tree_case_display_currentItem = currentItem;

    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);


    QHeaderView *header_view_obj =  m_tablewidget_obj->horizontalHeader();
    connect(header_view_obj , SIGNAL(sectionClicked(int)), this , SLOT(slot_table_widget_header_item_clicked(int)));


    disconnect(m_tablewidget_obj, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_main_table_widget_item_clicked(QTableWidgetItem*)));
    connect(m_tablewidget_obj, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_main_table_widget_item_clicked(QTableWidgetItem*)));
    disconnect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));

    total_record_count = records_count;


    pushbutton_search_filters = new QPushButton(QString("Filters"), this);
    menu_search_filters       = new QMenu(this);

    pushbutton_search_filters->hide();
    menu_search_filters->hide();
    pushbutton_search_filters->setMenu(menu_search_filters);


    // upper_widgets_hbox->addWidget(pushbutton_search_filters);

    text_edit_search_filters = new QTextEdit(this);
    text_edit_search_filters->setReadOnly(true);
    hbox_table_widget_extra_widgets->addWidget(text_edit_search_filters);
    text_edit_search_filters->hide();
    label_text_edit_show_link = new QLabel(this);
    label_text_edit_show_link->setText("<a href = "+ QString("Show") + " > " + QString("Searched Filters") + " </a>");
    label_text_edit_show_link->hide();
    QObject::connect(label_text_edit_show_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_link_label_searched_filters(QString)));

    text_edit_search_apple_metadata_filters = new QTextEdit(this);
    text_edit_search_apple_metadata_filters->setReadOnly(true);
    hbox_table_widget_extra_widgets->addWidget(text_edit_search_apple_metadata_filters);
    text_edit_search_apple_metadata_filters->hide();///show only for apple-metadata filters

    label_searched_apple_metadata_filters_show_link = new QLabel(this);
    label_searched_apple_metadata_filters_show_link->setText("<a href = "+ QString("Show") + " > " + QString("Searched Filters") + " </a>");
    label_searched_apple_metadata_filters_show_link->hide();
    QObject::connect(label_searched_apple_metadata_filters_show_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_link_label_apple_metadata_searched_filters(QString)));

    upper_widgets_hbox->insertWidget(2, pushbutton_search_filters);
    upper_widgets_hbox->addWidget(label_text_edit_show_link,1,Qt::AlignRight);
    upper_widgets_hbox->addWidget(label_searched_apple_metadata_filters_show_link,1,Qt::AlignRight);


    QString dest_db_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();

    set_display_db_path(dest_db_dir_path);

    //connect(add_notes_to_singal_record_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_add_notes_pushbutton_clicked(QString)));
    connect(add_notes_to_bookmarks_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_add_notes_to_bookmark_pushbutton_clicked(QString)));

    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_remaining_tags_list(QStringList, QString , QString )),this,SLOT(slot_remaining_tags_list(QStringList, QString , QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_tag_list_to_be_removed(QStringList, QString , QString)),this,SLOT(slot_tag_list_to_be_removed(QStringList, QString , QString)));

    label_link_searched_face = new QLabel(this);
    label_link_searched_face->setText("<a href = "+ QString("Show") + " > " + QString("Searched Face") + " </a>");
    label_link_searched_face->setAlignment( Qt::AlignTop);
    upper_widgets_hbox->addWidget(label_link_searched_face);
    label_link_searched_face->hide();

    QObject::connect(label_link_searched_face, SIGNAL(linkActivated(QString)), this, SLOT(slot_label_link_searched_face_linkactivated(QString)));

    label_picture_searched_face = new QLabel(this);
    label_picture_searched_face->setFixedWidth(345);
    label_picture_searched_face->setFixedHeight(335);
    hbox_table_widget_extra_widgets->addWidget(label_picture_searched_face);
    label_picture_searched_face->setStyleSheet("border: 4px inset grey");
    label_picture_searched_face->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::set_display_db_path(QString db_dir_path)
{
    filetypes_destination_dir_path = db_dir_path;
}


void filetypes_loader::pub_populate_data_in_table()
{ // populate data in file types tables
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    record_no_index   = enum_Record_No_Index ;
    filepath_index    = enum_File_Path_Index  ;
    notes_value_index  = enum_Notes_Value_Index;
    blank_column_index = enum_Blank_Column_Index;


    total_covered_count = 0;




    m_tablewidget_obj->hideColumn(enum_Apple_Latitude_Index);
    m_tablewidget_obj->hideColumn(enum_Apple_Longitude_Index);
    m_tablewidget_obj->hideColumn(enum_WhereFrom_Index);
    m_tablewidget_obj->hideColumn(enum_Inode_No_Index);
    m_tablewidget_obj->hideColumn(enum_Exif_Make_Index);
    m_tablewidget_obj->hideColumn(enum_Exif_Model_Index);
    m_tablewidget_obj->hideColumn(enum_Exif_Latitude_Index);
    m_tablewidget_obj->hideColumn(enum_Exif_Longitude_Index);
    // m_tablewidget_obj->hideColumn(enum_Source_Name_Index);
    m_tablewidget_obj->hideColumn(enum_Apple_Metadata_Column_Index);
    m_tablewidget_obj->hideColumn(enum_Exif_Metadata_Column_Index);
    m_tablewidget_obj->hideColumn(enum_Extension_Index);
    // m_tablewidget_obj->hideColumn(enum_Date_Added_Index);
    m_tablewidget_obj->hideColumn(enum_Source_Count_Name_Index);
    m_tablewidget_obj->hideColumn(enum_Signature_Name_Index);
    m_tablewidget_obj->hideColumn(enum_Signature_Value_Index);
    //  m_tablewidget_obj->hideColumn(enum_Hash_MD5_Index);
    m_tablewidget_obj->hideColumn(enum_Content_search_Number_of_hits);
    m_tablewidget_obj->hideColumn(enum_Content_search_Keyword_Hit);
    m_tablewidget_obj->hideColumn(enum_Content_search_Word_Count);
    m_tablewidget_obj->hideColumn(enum_Content_search_Hit_Locations);
    //m_tablewidget_obj->hideColumn(enum_File_Decompression_Status_Index);


    m_tablewidget_obj->setColumnWidth(enum_Date_Modified_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Date_Change_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Date_Accessed_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Date_Created_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Date_Added_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Content_Creation_Date_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Content_Modification_Date_Index, 220);
    m_tablewidget_obj->setColumnWidth(enum_Last_Used_Date_Index, 220);

    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,0);

    //    disable_widgets(true);
    //    emit signal_disable_case_tree_sidebar(true);
    display_data();
    //    emit signal_disable_case_tree_sidebar(false);
    //    disable_widgets(false);

    for(int count = 0; count < m_tablewidget_obj->columnCount(); count++)
    {

        if(m_tablewidget_obj->isColumnHidden(count))
            continue;

        QString column_label = m_tablewidget_obj->horizontalHeaderItem(count)->text();

        if(column_label.isEmpty())
            continue;

        QAction *action_filters   =  new QAction(column_label , this);
        action_filters->setCheckable(true);

        if(display_header_name_list.at(count).trimmed() == QString("File Name"))
            action_filters->setChecked(true);

        if(file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent)
                && display_header_name_list.at(count).trimmed() == QString("OCR Text"))
        {
            action_filters->setChecked(true);
        }

        menu_search_filters->addAction(action_filters);
    }


    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {
        // For custom right click
        //Bookmark
        submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
        create_right_click_actions(submenu_bookmark, menuList);

        //Tags
        sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
        create_tags_submenu();
        connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
        connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

        //Notes
        menuList.clear();
        menuList   << QString(MACRO_Generic_Right_Click_Add_Note)
                   << QString(MACRO_Generic_Right_Click_Remove_Note);

        submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
        create_right_click_actions(submenu_note, menuList);
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        //Quick look
        act_quick_look = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Quick_look),mainMenu);

        QStringList submenuList;
        submenuList << QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                    << ""
                    << QString(MACRO_CASE_TREE_Plist_Viewer)
                    << QString(MACRO_CASE_TREE_Hex_Viewer)
                    << QString(MACRO_CASE_TREE_Sqlite_Viewer)
                    << QString(MACRO_CASE_TREE_Registry_Viewer)
                    << QString(MACRO_CASE_TREE_Log_Viewer);

        submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
        create_right_click_actions(submenu_open_with , submenuList);

        submenu_open_with_external_application = new QMenu(this);
        connect(submenu_open_with_external_application,SIGNAL(triggered(QAction*)),this,SLOT(slot_sub_menu_open_file_with_other_app_clicked(QAction*)));

        QList <QMenu*> open_with_other_app_menu_list;
        open_with_other_app_menu_list.append(submenu_open_with_external_application);
        recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(open_with_other_app_menu_list, Q_FUNC_INFO);

        act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

        //Copy to Clipboard
        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);
        create_right_click_actions(mainMenu , menuList);

        create_right_click_actions(mainMenu ,QStringList("")); //----Seperator

        submenuList.clear();
        submenuList << QString(MACRO_CASE_TREE_Plist_Viewer)
                    << QString(MACRO_CASE_TREE_Hex_Viewer)
                    << QString(MACRO_CASE_TREE_Sqlite_Viewer)
                    << QString(MACRO_CASE_TREE_Registry_Viewer)
                    << QString(MACRO_CASE_TREE_Log_Viewer);

        submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_send_to_bucket);
        create_right_click_actions(submenu_send_to_bucket , submenuList);

        //Add to text indexing
        act_add_file_to_text_indexing_queue = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue),mainMenu);

        create_right_click_actions(mainMenu ,QStringList("")); //----Seperator

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Carve_Files) << QString(MACRO_Generic_Right_Click_Carve_Data);
        submenu_carve = create_right_click_submenu( QString(MACRO_Generic_Right_Click_Carve) , mainMenu, submenu_carve);
        connect(submenu_carve, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_carve_clicked(QAction*)));
        create_right_click_actions(submenu_carve,menuList);

        submenu_run_fs_modules = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules) , mainMenu , submenu_run_fs_modules);

        connect(submenu_run_fs_modules, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_run_fs_modules_clicked(QAction*)));

        QAction *act_Run_Filesystem_Modules_Apple_Metadata = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Exif_Metadata  = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Exif_Metadata).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Signature_Analysis = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Signature_Analysis).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Hashset = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Hashes).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Face_Analysis = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_OCR = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Weapons = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Fire_Analysis = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis).toLocal8Bit(), this);
        QAction *act_Run_Filesystem_Modules_Skin_Tone_Detection = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection).toLocal8Bit(), this);

        act_Run_Filesystem_Modules_Apple_Metadata->setIcon(QIcon("../icons/Features/apple_metadata.png"));
        act_Run_Filesystem_Modules_Exif_Metadata->setIcon(QIcon("../icons/Features/exif_data.png"));
        act_Run_Filesystem_Modules_Signature_Analysis->setIcon(QIcon("../icons/Features/signature_analysis.png"));
        act_Run_Filesystem_Modules_Hashset->setIcon(QIcon("../icons/Features/hashset.png"));
        act_Run_Filesystem_Modules_Face_Analysis->setIcon(QIcon("../icons/Features/face_analysis.png"));
        act_Run_Filesystem_Modules_OCR->setIcon(QIcon("../icons/Features/optical_character_recognition.png"));
        act_Run_Filesystem_Modules_Weapons->setIcon(QIcon("../icons/Features/weapons.png"));
        act_Run_Filesystem_Modules_Fire_Analysis->setIcon(QIcon("../icons/Features/fire_analysis.png"));
        act_Run_Filesystem_Modules_Skin_Tone_Detection->setIcon(QIcon("../icons/Features/skin_tone_detection.png"));

        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Apple_Metadata);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Exif_Metadata);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Signature_Analysis);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Hashset);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Face_Analysis);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_OCR);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Weapons);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Fire_Analysis);
        submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Skin_Tone_Detection);

        act_face_search = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Face_Search),mainMenu);
        connect(act_face_search, SIGNAL(triggered(bool)), this, SLOT(slot_act_face_search_triggered(bool)));

        act_remove_record = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Remove_Record),mainMenu);
        connect(act_remove_record, SIGNAL(triggered(bool)), this, SLOT(slot_action_right_click_remove_record()));

        create_right_click_actions(mainMenu ,QStringList("")); //----Seperator

        // Hashset
        submenu_add_files_to_hashset_db = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database) , mainMenu , submenu_add_files_to_hashset_db);

        QList <QMenu*> hahses_menu_list;
        hahses_menu_list.append(submenu_add_files_to_hashset_db);
        recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(hahses_menu_list, Q_FUNC_INFO);
        action_search_file_with_same_md5_hash = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash),mainMenu);
        connect(action_search_file_with_same_md5_hash, SIGNAL(triggered()), this, SLOT(slot_action_search_file_with_same_hash_clicked()));

        export_hashes_as_vic_obj = new export_hashes_as_vic(this);
        connect(export_hashes_as_vic_obj,SIGNAL(signal_export_button_clicked()) , this , SLOT(slot_export_hashes_as_vic_clicked())); //only for VIC-2.0

        export_hashes_as_sqlite_obj = new export_hashes_as_sqlite(this);
        connect(export_hashes_as_sqlite_obj,SIGNAL(signal_export_button_clicked()) , this , SLOT(slot_export_hashes_as_sqlite_clicked()));

        export_hashes_as_csv_obj = new export_hashes_as_csv(this);
        connect(export_hashes_as_csv_obj,SIGNAL(signal_export_button_clicked()) , this , SLOT(slot_export_hashes_as_csv_clicked()));

        submenu_export_hashes_as = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As) , mainMenu , submenu_export_hashes_as);
        connect(submenu_export_hashes_as, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_export_hashes_as_clicked(QAction*)));
        QAction *act_export_vic_1_1 = new QAction(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_1).toLocal8Bit(), this);
        QAction *act_export_vic_1_2 = new QAction(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_2).toLocal8Bit(), this);
        QAction *act_export_vic_1_3 = new QAction(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_1_3).toLocal8Bit(), this);
        QAction *act_export_vic_2_0 = new QAction(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_VIC_Version_2_0).toLocal8Bit(), this);
        QAction *act_export_sqlite = new QAction(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_SQLite).toLocal8Bit(), this);
        QAction *act_export_csv = new QAction(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As_CSV).toLocal8Bit(), this);

        submenu_export_hashes_as->addAction(act_export_vic_1_1);
        submenu_export_hashes_as->addAction(act_export_vic_1_2);
        submenu_export_hashes_as->addAction(act_export_vic_1_3);
        submenu_export_hashes_as->addAction(act_export_vic_2_0);
        submenu_export_hashes_as->addAction(act_export_sqlite);
        submenu_export_hashes_as->addAction(act_export_csv);

        create_right_click_actions(mainMenu ,QStringList("")); //----Seperator

        // Export
        act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

        //Export as kml;
        menuList.clear();
        menuList  << QString(MACRO_Generic_Right_Click_Export_as_KML);
        create_right_click_actions(mainMenu , menuList);

        create_right_click_actions(mainMenu ,QStringList("")); //----Seperator

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Mark_As_Seen) << QString(MACRO_Generic_Right_Click_Mark_As_Unseen);
        submenu_seen = create_right_click_submenu( QString(MACRO_Generic_Right_Click_Seen) , mainMenu, submenu_seen);
        create_right_click_actions(submenu_seen,menuList);

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Decompress_File)
                 << QString(MACRO_Generic_Right_Click_Go_To_Source);
        create_right_click_actions(mainMenu , menuList);

        ///----Create Export of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));
        connect(submenu_open_with, SIGNAL(triggered(QAction*)), this, SLOT(slot_open_with_submenu_clicked(QAction*)));
        connect(submenu_send_to_bucket, SIGNAL(triggered(QAction*)), this, SLOT(slot_sendtobucket_submenu_clicked(QAction*)));
        connect(submenu_add_files_to_hashset_db,SIGNAL(triggered(QAction*)),this,SLOT(slot_add_files_to_hashset_submenu_clicked(QAction*)));
    }
    pub_refresh_horizontal_header();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_act_quick_look_triggered()
{ // quick look the data on space bar pressed or from right click action
    //    int row = m_tablewidget_obj->currentRow();

    //    if(row < 0)
    //        return;

    //tablewidget_general_clicked(row,2);
    emit signal_act_quick_look_triggered();
}

void filetypes_loader::slot_act_face_search_triggered(bool status)
{ // face search action from right click triggered to run face search feature on file type records
    int selected_row_no = m_tablewidget_obj->currentRow();

    if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
        return;

    if(m_tablewidget_obj->isRowHidden(selected_row_no))
        return;


    QString record_no_str = get_record_number_from_selected_row(selected_row_no);

    QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

    QString display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_path,source_count_name);
    bool bool_is_image = recon_helper_standard_obj->check_is_it_image_file(file_path,display_file_path,Q_FUNC_INFO);
    if(bool_is_image)
    {
        facefinder_obj->pub_clear_fields();
        bool bool_value_face = facefinder_obj-> extract_cropped_faces(file_path);

        if(bool_value_face)
            facefinder_obj->pub_prepare_display();
    }

}

void filetypes_loader::slot_action_right_click_remove_record()
{ // remove record from file type feature, it will remove any particular record from tablwidget which we don't want to see
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);



    QString feature_db_name;
    QString m_file_type_prnt;
    if((file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
            || (file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
            || (file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent))
            || (file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
            || (file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent)))
    {
        m_file_type_prnt = file_type_parent;
        feature_db_name = file_type_parent;
        feature_db_name = feature_db_name.toLower().replace(" ","_").append(".sqlite");
    }
    else
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();

    display_loading_progress_bar_non_cancelable_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList db_path_list;

    for(int ii = selection_model_list.size() - 1; ii >= 0; ii--)
    {
        if(ii % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(ii);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();

        QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);


        QString remove_record_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() +
                source_count_name +"/"+ feature_db_name;

        QString commd_rmv_record = "Delete From files where filesystem_record = '" + record + "' ";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(commd_rmv_record,remove_record_db_path,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("=====Query---not---Executed---FAILED---- " , Q_FUNC_INFO);
            return;
        }

        if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        {
            QString file_system_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();

            QString face_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString() + "/face_info.sqlite";

            QString cmmd_face_id = "Select face_id from all_faces where fs_record = '" + record + "' AND source_count_name = '" + source_count_name + "'";

            QString face_id_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmmd_face_id, 0, face_info_db_path, Q_FUNC_INFO);

            ///Delete record
            QString delete_cmd = "DELETE from all_faces WHERE fs_record = '" + record + "'";

            recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd, face_info_db_path, Q_FUNC_INFO);

            ///Get remaining records
            QString select_cmd = "SELECT fs_record from all_faces WHERE face_id = '" + face_id_str + "'";

            QStringList fs_record_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd, 0, face_info_db_path, Q_FUNC_INFO);

            if(fs_record_list.size() <= 0)
            {
                QString cmmd_remove_cropped_face = "Select file_path from distinct_faces where fs_record = '" + record + "' AND source_count_name = '" + source_count_name + "'";

                QStringList path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmmd_remove_cropped_face, 0, face_info_db_path, Q_FUNC_INFO);
                for(int kk = 0; kk < path_list.size(); kk++)
                {
                    QString final_path = file_system_dir_path + path_list.at(kk);

                    if(QFile(final_path).exists())
                        QFile(final_path).remove();
                }

                QString delete_record = "DELETE from distinct_faces WHERE fs_record = '" + record + "'";
                recon_helper_standard_obj->execute_db_command_by_dbpath(delete_record, face_info_db_path, Q_FUNC_INFO);

            }

            else
            {
                QString update_cmd = "UPDATE distinct_faces SET fs_record = '" + fs_record_list.at(0) + "' WHERE face_id = '" + face_id_str + "'";
                recon_helper_standard_obj->execute_db_command_by_dbpath(update_cmd, face_info_db_path, Q_FUNC_INFO);
            }
        }

        for(int pp = 0 ; pp < st_gallery_view_file_info_list.size() ; pp++)
        {
            struct_global_gallery_view_file_info f1 = st_gallery_view_file_info_list.at(pp);
            {

                if((f1.INT == record)
                        && (f1.source_count_name == source_count_name))
                {
                    st_gallery_view_file_info_list.removeAt(pp);

                }

            }

        }
        gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);
        m_tablewidget_obj->removeRow(selected_row_no);

        //----------------------------------------SkinTone Tag remove start-------------------------------------------------//
        if(file_type_parent == MACRO_CASE_TREE_Skin_Tone_Detection_Parent)
        {
            QString file_system_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();
            QString fs_db_path =file_system_dir_path + source_count_name + "/file_system.sqlite";
            QString command = "SELECT recon_tag_value from files where INT = ?";
            QStringList arg_list;
            arg_list << record;
            QString tag_to_be_removed = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,fs_db_path,Q_FUNC_INFO);
            if(tag_to_be_removed == QString(MACRO_Tag_Name_Skin_Tone))
            {
                QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);
                QString tab_name_str    = QString(MACRO_Plugin_File_System_TAB_Name_Files);

                command = "update files set recon_tag_value = ? where INT = ?";
                arg_list.clear();
                arg_list << "" << record;

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);
                emit signal_table_widget_loader_removed_tags_list(plugin_name_str,tab_name_str,record,QStringList(tag_to_be_removed),"");

            }
        }
        //----------------------------------------SkinTone Tag remove end-------------------------------------------------//
    }

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        QString db_path_list_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + list_target_source_info.at(count).source_count_name + QString("/") + feature_db_name;

        db_path_list << db_path_list_1;
    }

    emit signal_right_click_removed_record_refresh_in_case_tree(db_path_list,m_file_type_prnt);

    emit signal_clear_all_metadata();
    bool_act_remove_record = true;

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::contextMenuEvent(QContextMenuEvent *evt)
{ // create right click actions and menu here
    if(evt == NULL)
        return;

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        menu_carved_index_files_right_click->exec(QCursor::pos());
        return;
    }

    if(main_tab_widget_obj->currentIndex() > 1)
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int current_row = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();


    QList <QAction *> act_list = mainMenu->actions();

    if(act_list.isEmpty())
        return;

    create_add_files_to_hashset_submenu();

    add_applications_to_open_application_menu();

    QAction *act_go_to_source;
    for(int ii = 0 ; ii < act_list.size() ; ii++)
    {
        if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Face_Search))
        {
            act_list.at(ii)->setEnabled(false);
            if(selection_model_list.size() == 1)
            {
                act_list.at(ii)->setEnabled(true);
            }
        }
        else if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Remove_Record))
            act_list.at(ii)->setEnabled(false);
        else if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Go_To_Source))
            act_go_to_source = act_list.at(ii);
        else if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Decompress_File))
        {
            // action should disable for multi selection OR not-supported file

            act_list.at(ii)->setEnabled(false);
            QString f_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), enum_File_Name_Index)->text().trimmed();
            if((selection_model_list.size() == 1) && (f_name.endsWith(".zip",Qt::CaseInsensitive) || f_name.endsWith(".tar",Qt::CaseInsensitive)
                                                      || f_name.endsWith(".tar.gz",Qt::CaseInsensitive) || f_name.endsWith(".tar.bz2",Qt::CaseInsensitive)))
            {
                act_list.at(ii)->setEnabled(true);
            }
        }


        if(file_type_parent == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
        {
            if(file_type_child != QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations))
            {
                //                if(act_list.at(i)->text() == QString(MACRO_Generic_Right_Click_Export_as_KML))
                //                    act_list.at(i)->setEnabled(false);
            }
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Exif_Parent))
        {
            if(file_type_child != QString(MACRO_CASE_TREE_Exif_Category_GPS))
            {
                //                if(act_list.at(i)->text() == QString(MACRO_Generic_Right_Click_Export_as_KML))
                //                    act_list.at(i)->setEnabled(false);
            }
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
        {
            if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue))
                act_list.at(ii)->setEnabled(false);
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Hashset))
        {
            if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database))
                act_list.at(ii)->setEnabled(false);
            else if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash))
                act_list.at(ii)->setEnabled(false);
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        {
            if(file_type_child == QString(MACRO_CASE_TREE_Face_Analysis_Category_Faces))
            {
                if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Face_Search))
                    act_list.at(ii)->setEnabled(true);

                if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Remove_Record))
                    act_list.at(ii)->setEnabled(true);
            }
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Face_Search_PARENT))
        {
            if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Face_Search))
                act_list.at(ii)->setEnabled(false);
        }
        else if((file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent)))
        {
            if(act_list.at(ii)->text() == QString(MACRO_Generic_Right_Click_Remove_Record))
                act_list.at(ii)->setEnabled(true);
        }

    }


    ///Disable actions for FS Modules

    QString fs_record_no       = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), enum_Record_No_Index)->text().trimmed();
    QString src_cnt_nm  = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), enum_Source_Count_Name_Index)->text().trimmed();
    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + src_cnt_nm + "/file_system.sqlite";
    bool bool_record_decompressed = recon_helper_standard_obj->check_record_is_decompressed_by_dbpath(fs_record_no,db_path_filesystem,Q_FUNC_INFO);
    struct_GLOBAL_witness_info_source struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_nm);
    bool bool_source_nested = struct_target_source_info.bool_source_nested;

    foreach (QAction *action, submenu_run_fs_modules->actions())
    {

        //==================Enable Some Right Click Fs Module Only for Single file/Directory Start=================//

        if(action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition)
                || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons)
                || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis)
                || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis)
                || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection))
        {
            action->setEnabled(false);

            if((selection_model_list.size() == 1) && (recon_case_obj->pub_is_source_mime_type_extraction_completed(src_cnt_nm)))
            {
                action->setEnabled(true);
            }
        }
        //==================Enable some Right Click Fs Module Only for Single file/Directory End=================//


        if(((file_type_parent == QString(MACRO_CASE_TREE_Apple_Metadata_Parent)
             && (struct_target_source_info.file_system_type !=  QString(MACRO_FILE_SYSTEM_TYPE_HFS)
                 || struct_target_source_info.file_system_type !=  QString(MACRO_FILE_SYSTEM_TYPE_APFS)))
            || bool_record_decompressed || bool_source_nested)
                && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata))
        {
            action->setDisabled(true);
            //break;
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Exif_Parent) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Exif_Metadata))
        {
            action->setDisabled(true);
            //break;
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_File_Signature_Parent) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Signature_Analysis))
        {
            action->setDisabled(true);
            // break;
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Hashset) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Hashes))
        {
            action->setDisabled(true);
            // break;
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis))
        {
            action->setDisabled(true);
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition))
        {
            action->setDisabled(true);
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons))
        {
            action->setDisabled(true);
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis))
        {
            action->setDisabled(true);
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent) && action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection))
        {
            action->setDisabled(true);
        }
    }

    if(act_go_to_source == NULL)
        return;

    action_search_file_with_same_md5_hash->setEnabled(false);

    if(selection_model_list.size() == 1)
    {
        action_search_file_with_same_md5_hash->setEnabled(true);
        act_go_to_source->setEnabled(true);
        act_quick_look->setEnabled(true);
        act_full_detail_info->setEnabled(true);
    }
    else if(selection_model_list.size() > 1)
    {
        act_go_to_source->setEnabled(false);
        act_quick_look->setEnabled(false);
        act_full_detail_info->setEnabled(false);
    }
    //    else
    //    {
    //        act_go_to_source->setEnabled(true);
    //        act_quick_look->setEnabled(true);
    //        act_full_detail_info->setEnabled(true);
    //    }


    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW
            || main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard)
                    || action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Seen)
                    || action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Unseen)
                    || action->text() == QString(MACRO_Generic_Right_Click_Hide_Seen_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Show_Seen_Files))
            {
                action->setEnabled(true);
            }
        }

    }
    else if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW)
    {
        if(st_gallery_view_file_info_list.size() == 0)
            return;

        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard)
                    || action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Seen)
                    || action->text() == QString(MACRO_Generic_Right_Click_Mark_As_Unseen)
                    || action->text() == QString(MACRO_Generic_Right_Click_Hide_Seen_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Show_Seen_Files))
            {
                action->setEnabled(false);
            }
        }
    }


    struct_global_right_clicks_status_depends_on_file_existance obj;
    obj.plugin_name = MACRO_Plugin_Name_File_System;
    obj.feature_name = QString(MACRO_CASE_TREE_Filetype_Parent);
    obj.tab_name = MACRO_Plugin_File_System_TAB_Name_Files;
    obj.selected_row_count = selection_model_list.size();
    obj.record_no = m_tablewidget_obj->item(current_row,enum_Record_No_Index)->text();
    obj.source_count_name = m_tablewidget_obj->item(current_row,enum_Source_Count_Name_Index)->text();
    recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

    mainMenu->exec(QCursor::pos());

    if(bool_act_remove_record)
    {
        bool_act_remove_record = false;
    }
    else
    {
        tablewidget_general_clicked(current_row , current_column);
    }
}

void filetypes_loader::display_data()
{ // prepare data here to display it on tablewidget from database
    recon_static_functions::app_debug(" -Starts filetype - " + file_type_child + ", source type - " + file_type_parent , Q_FUNC_INFO);

    if(db_columns_list.isEmpty() || table_name_list.isEmpty())
        return;

    clear_all_custom_header_widget_entries();

    st_gallery_view_file_info_list.clear();

    bool_cancel_loading = false;

    if(file_type_parent == MACRO_CASE_TREE_File_Search)
    {
        text_edit_search_filters->setMaximumWidth(200);
        //  label_text_edit_show_link->show();
        set_data_in_text_edit_searched_filter(file_type_child);
    }

    map_bookmarked_records_from_table_display.clear();

    QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_loading)
            break;

        struct_GLOBAL_witness_info_source target_struct_obj = list_target_source_info.at(count);
        QString source_count_name = target_struct_obj.source_count_name;

        if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
            continue ;

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/apple_metadata.sqlite");
        QString exif_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/exif_metadata.sqlite");
        QString mime_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/mime.sqlite");
        QString sign_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/signature.sqlite");
        QString file_hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/hashes.sqlite");
        QString face_analysis_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/face_analysis.sqlite");
        QString optical_character_recognition_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/optical_character_recognition.sqlite");
        QString skin_tone_detection_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/skin_tone_detection.sqlite");
        QString weapons_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/weapons.sqlite");
        QString fire_analysis_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/fire_analysis.sqlite");
        QString griffeye_data_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/griffeye_data.sqlite");

        QString db_cmd = QString("Select ");

        for(int i = 0; i < db_columns_list.size(); i++)
        {
            if((file_type_child == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations) && file_type_parent == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_MIME_Type))
                    || (file_type_parent == QString(MACRO_CASE_TREE_File_Signature_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions))
                    || (file_type_parent == QString(MACRO_CASE_TREE_File_Size_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Apple_Metadata_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Exif_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Hashset))
                    || (file_type_parent == QString(MACRO_CASE_TREE_File_Search))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Face_Search_PARENT))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_Griffeye_Data_Parent))
                    || (file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions)))
            {
                db_cmd.append( QString("a.") + db_columns_list.at(i) + ",");
            }
        }

        if((file_type_child == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations) && file_type_parent == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_MIME_Type))
                || (file_type_parent == QString(MACRO_CASE_TREE_File_Signature_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions))
                || (file_type_parent == QString(MACRO_CASE_TREE_File_Size_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Apple_Metadata_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Exif_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Hashset))
                || (file_type_parent == QString(MACRO_CASE_TREE_File_Search))
                || (file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
                || (file_type_parent == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
                || (file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
                || (file_type_parent == QString(MACRO_CASE_TREE_Face_Search_PARENT))
                || (file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_Griffeye_Data_Parent))
                || (file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions)))
        {
            db_cmd.append(QString("a.is_seen,"));
        }

        QString tmp_db_cmd = db_cmd;

        //        if(file_type_child.contains(QString(" (")))
        //            file_type_child = file_type_child.mid(0 , file_type_child.indexOf(QString(" ("))).trimmed();


        if(file_type_parent == QString(MACRO_CASE_TREE_Exif_Parent))
        {

            if(file_type_child == MACRO_CASE_TREE_Exif_Category_Make_Model)
            {
                m_tablewidget_obj->showColumn(enum_Exif_Make_Index);
                m_tablewidget_obj->showColumn(enum_Exif_Model_Index);
            }
            else if(file_type_child == MACRO_CASE_TREE_Exif_Category_GPS)
            {
                m_tablewidget_obj->showColumn(enum_Exif_Latitude_Index);
                m_tablewidget_obj->showColumn(enum_Exif_Longitude_Index);
            }
            else
            {
                m_tablewidget_obj->showColumn(enum_Exif_Metadata_Column_Index);

                for(int jj = 0; jj < custom_header_widgets_QList.size(); jj++)
                {
                    custom_table_header_widget *header_wdgt_ptr = custom_header_widgets_QList.at(jj);

                    QString current_header_column_name = header_wdgt_ptr->pub_get_column_name();

                    if(current_header_column_name == "Exif Metadata")
                    {
                        header_wdgt_ptr->pub_set_column_name(file_type_child);
                    }
                }
            }


            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(exif_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);


            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);


            if(file_type_child == MACRO_CASE_TREE_Exif_Category_Make_Model)
                tmp_db_cmd.append("c.make,c.model, b.mime_type from main.files as c join filesystemDB.files as a ON c.filesystem_record=a.INT     LEFT OUTER join mimeDB.files as b ON  c.filesystem_record=b.filesystem_record  where (c.make is not null AND c.make != '')");
            else if(file_type_child == MACRO_CASE_TREE_Exif_Category_GPS)
                tmp_db_cmd.append("c.latitude,c.longitude, b.mime_type from main.files as c join filesystemDB.files as a ON c.filesystem_record=a.INT     LEFT OUTER join mimeDB.files as b ON  c.filesystem_record=b.filesystem_record  where ((c.latitude is not null AND c.latitude != '') AND (c.longitude is not null AND c.longitude != ''))");
            else
            {
                QString cmd_str = QString("Select custom_column_name from tbl_exif_attributes Where parent_name = '") + file_type_child + QString("'");
                QString attr_column_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_str, 0, case_db_path, Q_FUNC_INFO);
                tmp_db_cmd.append("c." + attr_column_name + QString(", b.mime_type from main.files as c join filesystemDB.files as a ON c.filesystem_record=a.INT     LEFT OUTER join mimeDB.files as b ON  c.filesystem_record=b.filesystem_record  where ((c.") + attr_column_name + QString(" is not null AND c.") + attr_column_name + QString(" != ''))"));
            }

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Exif_Parent) ;


            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(filesystem_db_dettach_commmand);
            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);
            destination_db.close();

        }
        else if(file_type_child == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations)
                && file_type_parent == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
        {

            m_tablewidget_obj->showColumn(enum_Exif_Latitude_Index);
            m_tablewidget_obj->showColumn(enum_Exif_Longitude_Index);


            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(exif_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);


            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);

            QString apple_db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadataDB")
                    .arg(QDir::toNativeSeparators(apple_metadata_db_path));
            query.exec(apple_db_attach_commmand);

            QString m_cmd = tmp_db_cmd;
            m_cmd.append("c.latitude,c.longitude, b.mime_type from main.files as c join filesystemDB.files as a ON c.filesystem_record=a.INT     LEFT OUTER join mimeDB.files as b ON  c.filesystem_record=b.filesystem_record  where ((c.latitude is not null AND c.latitude != '') AND (c.longitude is not null AND c.longitude != ''))");

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, m_cmd, source_count_name,MACRO_CASE_TREE_Redefined_Result_Category_Locations) ;



            m_cmd = tmp_db_cmd;
            m_cmd.append("d.kMDItemLatitude_Latitude,d.kMDItemLongitude_Longitude, b.mime_type from apple_metadataDB.files as d  join filesystemDB.files as a  ON d.filesystem_record=a.INT     LEFT OUTER join mimeDB.files as b ON  d.filesystem_record=b.filesystem_record  where ((d.kMDItemLatitude_Latitude is not null AND d.kMDItemLatitude_Latitude != '') AND (d.kMDItemLongitude_Longitude is not null AND d.kMDItemLongitude_Longitude != ''))");

            prepare_display_for_filetypes(destination_db , filesystem_db_path, m_cmd, source_count_name,MACRO_CASE_TREE_Redefined_Result_Category_Locations) ;

            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(filesystem_db_dettach_commmand);
            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);
            //            QString apple_db_dettach_commmand = QString("DETACH DATABASE apple_metadataDB");
            //            query.exec(apple_db_dettach_commmand);
            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions))
        {
            m_tablewidget_obj->showColumn(enum_Extension_Index);

            QString case_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
            QString command = QString("Select extension_name From extension_list Where category_name = ?");
            QStringList values;
            values << file_type_child;
            QStringList extn_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, values, 0, case_db_path, Q_FUNC_INFO);

            if(extn_list.isEmpty())
            {
                recon_static_functions::app_debug("---FAILED---  " + file_type_child, Q_FUNC_INFO);
                continue;
            }

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);
            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);




            tmp_db_cmd.append("b.mime_type From main.files as a  LEFT OUTER join mimeDB.files as b  ON a.INT = b.filesystem_record where ");

            QString diff_cmd;
            for(int count = 0; count < extn_list.size(); count++)
            {
                QString extension_str = extn_list.at(count);
                if(extension_str.startsWith("."))
                    extension_str = extension_str.mid(1, extension_str.size()).trimmed();

                //  diff_cmd += " a.extension = '" + extension_str + "' OR ";
                diff_cmd += " a.extension = '" + extension_str + QString("' COLLATE NOCASE") + " OR ";
            }

            diff_cmd.chop(4); // OR
            tmp_db_cmd.append(diff_cmd);
            // tmp_db_cmd.append(" COLLATE NOCASE");

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions) ;

            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);

            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
        {
            m_tablewidget_obj->showColumn(enum_Exif_Make_Index);
            m_tablewidget_obj->showColumn(enum_Exif_Model_Index);
            m_tablewidget_obj->showColumn(enum_Exif_Latitude_Index);
            m_tablewidget_obj->showColumn(enum_Exif_Longitude_Index);
            m_tablewidget_obj->showColumn(enum_Exif_Metadata_Column_Index);

            for(int jj = 0; jj < custom_header_widgets_QList.size(); jj++)
            {
                custom_table_header_widget *header_wdgt_ptr = custom_header_widgets_QList.at(jj);

                QString current_header_column_name = header_wdgt_ptr->pub_get_column_name();

                if(current_header_column_name == "Exif Metadata")
                {
                    header_wdgt_ptr->pub_set_column_name(file_type_child);
                }
            }

            QString db_name = file_type_child;
            db_name = db_name.replace(" ", "_");
            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + db_name + ".sqlite";

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(search_db_path);

            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);


            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);



            QString cmd_str = QString("Select custom_column_name from tbl_exif_attributes Where parent_name = '") + MACRO_Custom_Column_Name_Author + QString("'");
            QString attr_column_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_str, 0, case_db_path, Q_FUNC_INFO);

            tmp_db_cmd.append("c." + attr_column_name + QString(", c.make,c.model,c.latitude,c.longitude,b.mime_type from main.files as c LEFT OUTER join mimeDB.files as b ON c.filesystem_record = b.filesystem_record  join filesystemDB.files as a ON c.filesystem_record = a.INT  where c.source_count_name='" + source_count_name + "'"));


            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Exif_Metadata_Search) ;


            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(filesystem_db_dettach_commmand);
            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);
            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions))
        {
            recon_static_functions::app_debug(" In 1 " + QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions) , Q_FUNC_INFO);

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }


            QSqlQuery query(destination_db);
            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);




            QString extension_str = file_type_child;
            if(extension_str.startsWith("."))
                extension_str = extension_str.mid(1, extension_str.size()).trimmed();

            tmp_db_cmd.append(QString("b.mime_type From main.files as a LEFT OUTER join mimeDB.files as b  ON (a.INT = b.filesystem_record) where (a.extension = '") + extension_str + QString("')"));

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions) ;

            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);

            destination_db.close();

            recon_static_functions::app_debug(" In 2 " + QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions) , Q_FUNC_INFO);

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_MIME_Type))
        {
            m_tablewidget_obj->showColumn(enum_Extension_Index);
            m_tablewidget_obj->hideColumn(enum_Mime_Type_Index);

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }


            tmp_db_cmd = tmp_db_cmd.trimmed();
            if(tmp_db_cmd.endsWith(","))
                tmp_db_cmd.chop(1);
            tmp_db_cmd.append((" From main.files as a where (a.mime_type like '") + file_type_child + QString("/%')"));

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_MIME_Type) ;

            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_File_Size_Parent))
        {
            m_tablewidget_obj->showColumn(enum_Extension_Index);

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }


            QSqlQuery query(destination_db);
            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);


            qint64 bytes_qint64_second = 0, bytes_qint64_first = 0;

            if(file_type_child == QString(MACRO_CASE_TREE_File_Size_Category_50_MB))
            {
                bytes_qint64_first = 0;
                bytes_qint64_second = (50*MACRO_Generic_Value_MB_in_byte_int);
                tmp_db_cmd.append("b.mime_type From main.files as a LEFT OUTER join mimeDB.files as b  ON (a.INT = b.filesystem_record) where ((a.file_size between " + QString::number(bytes_qint64_first) +" AND "+ QString::number(bytes_qint64_second) + ") AND (a.file_size is not null AND a.file_size != ''))");
            }
            else if(file_type_child == QString(MACRO_CASE_TREE_File_Size_Category_250_MB))
            {
                bytes_qint64_first = (50*MACRO_Generic_Value_MB_in_byte_int);
                bytes_qint64_second = (250*MACRO_Generic_Value_MB_in_byte_int);
                tmp_db_cmd.append("b.mime_type From main.files as a LEFT OUTER join mimeDB.files as b  ON (a.INT = b.filesystem_record) where ((a.file_size between " + QString::number(bytes_qint64_first) +" AND "+ QString::number(bytes_qint64_second) + ") AND (a.file_size is not null AND a.file_size != ''))");
            }
            else if(file_type_child == QString(MACRO_CASE_TREE_File_Size_Category_1_GB))
            {
                bytes_qint64_first = (250*MACRO_Generic_Value_MB_in_byte_int);
                bytes_qint64_second = (MACRO_Generic_Value_GB_in_byte_int);
                tmp_db_cmd.append("b.mime_type From main.files as a LEFT OUTER join mimeDB.files as b  ON (a.INT = b.filesystem_record) where ((a.file_size between " + QString::number(bytes_qint64_first) +" AND "+ QString::number(bytes_qint64_second) + ") AND (a.file_size is not null AND a.file_size != ''))");
            }
            else if(file_type_child == QString(MACRO_CASE_TREE_File_Size_Category_Above_1_GB))
            {
                bytes_qint64_second = (MACRO_Generic_Value_GB_in_byte_int);
                tmp_db_cmd.append("b.mime_type From main.files as a LEFT OUTER join mimeDB.files as b  ON (a.INT = b.filesystem_record) where ((a.file_size > "+ QString::number(bytes_qint64_second) +") AND (a.file_size is not null AND a.file_size != ''))");
            }

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();
            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_File_Size_Parent) ;


            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);

            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_File_Signature_Parent))
        {

            QString signature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + source_count_name + QString("/signature.sqlite");

            QString command_str = QString("Select Distinct signature_name from files where signature_name NOTNULL");
            QStringList signature_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0 , signature_db_path , Q_FUNC_INFO);
            QString file_t_str = file_type_child;

            for(int i = 0 ; i < signature_category_list.size() ; i++)
            {
                QString signature_str;
                QString t_sig_name =  signature_category_list.at(i);

                if(t_sig_name == file_t_str)
                    signature_str = t_sig_name;
                else
                    continue ;

                m_tablewidget_obj->showColumn(enum_Signature_Name_Index);
                m_tablewidget_obj->showColumn(enum_Signature_Value_Index);

                QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
                QSqlDatabase::removeDatabase(connection_naam);
                QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                destination_db.setDatabaseName(sign_db_path);
                if(!destination_db.open())
                {
                    recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                    QSqlDatabase::removeDatabase(connection_naam);
                    continue;
                }

                QSqlQuery query(destination_db);
                QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                        .arg(QDir::toNativeSeparators(filesystem_db_path));
                query.exec(fs_db_attach_commmand);

                QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                        .arg(QDir::toNativeSeparators(mime_db_path));
                query.exec(mime_db_attach_commmand);

                //tmp_db_cmd.append("c.signature_name,c.signature_value,b.mime_type From main.files as c LEFT OUTER join filesystemDB.files as a  ON (c.filesystem_record = a.INT) LEFT OUTER join mimeDB.files as b ON (c.filesystem_record = b.filesystem_record) where (c.signature_value is not null AND c.signature_value != '')");

                tmp_db_cmd.append("c.signature_name,c.signature_value,b.mime_type From main.files as c LEFT OUTER join filesystemDB.files as a  ON (c.filesystem_record = a.INT) LEFT OUTER join mimeDB.files as b ON (c.filesystem_record = b.filesystem_record) where (c.signature_value is not null AND c.signature_value != '' AND c.signature_name = '" + signature_str + "'" + QString(")"));

                total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

                prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_File_Signature_Parent) ;

                QString fs_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
                query.exec(fs_db_dettach_commmand);
                QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
                query.exec(mime_db_dettach_commmand);

                destination_db.close();
            }
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Hashset))
        {
            m_tablewidget_obj->showColumn(enum_Hash_MD5_Index);

            m_tablewidget_obj->showColumn(enum_Hash_SHA1_Index);
            m_tablewidget_obj->hideColumn(enum_Hashset_Name_Index);

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(file_hashes_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }
            QSqlQuery query(destination_db);

            QString hashset_db_path  = tree_case_display_currentItem->text(enum_global_case_tree_display_column_THIRD).trimmed();
            QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

            QString command = QString("Select column_name From tbl_hashset Where database_path = ?");
            QStringList value_list;
            value_list << hashset_db_path;
            QString hash_column_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , value_list, 0, case_conf_db, Q_FUNC_INFO);
            hash_column_name = hash_column_name.trimmed();


            command = QString("Select table_name From tbl_hashset Where database_path = ?");
            QString hash_table_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , value_list, 0, case_conf_db, Q_FUNC_INFO);
            hash_table_name = hash_table_name.trimmed();

            QString hashset_db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashsetDB")
                    .arg(QDir::toNativeSeparators(hashset_db_path));
            query.exec(hashset_db_attach_commmand);

            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);

            tmp_db_cmd.append(" c.md5_hash,b.mime_type,c.sha1_hash from main.files as c join hashsetDB." + hash_table_name + " as d ON c.md5_hash=d."+ hash_column_name + " LEFT OUTER join mimeDB.files as b ON  c.filesystem_record=b.filesystem_record   join filesystemDB.files as a where c.filesystem_record=a.INT");


            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name,MACRO_CASE_TREE_Hashset) ;

            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);
            QString hashset_db_dettach_commmand = QString("DETACH DATABASE hashsetDB");
            query.exec(hashset_db_dettach_commmand);
            QString fs_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(fs_db_dettach_commmand);
            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Apple_Metadata_Parent))
        {

            if(file_type_child == MACRO_CASE_TREE_Apple_Metadata_Category_Latitude || file_type_child == MACRO_CASE_TREE_Apple_Metadata_Category_Longitude)
            {
                m_tablewidget_obj->showColumn(enum_Apple_Latitude_Index);
                m_tablewidget_obj->showColumn(enum_Apple_Longitude_Index);
            }
            else
            {
                m_tablewidget_obj->showColumn(enum_Apple_Metadata_Column_Index);

                for(int jj =0 ; jj < custom_header_widgets_QList.size(); jj++)
                {
                    custom_table_header_widget *header_wdgt_ptr = custom_header_widgets_QList.at(jj);

                    QString current_header_column_name = header_wdgt_ptr->pub_get_column_name();

                    if(current_header_column_name == "Apple Metadata")
                    {
                        header_wdgt_ptr->pub_set_column_name(file_type_child);
                    }
                }
            }

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(apple_metadata_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);
            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);

            QString apple_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(apple_db_attach_commmand);


            QString case_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
            QString cmd_str = QString("Select custom_column_name from tbl_apple_attributes Where attribute_name = '") + file_type_child + QString("'");
            QString attr_column_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_str, 0, case_db_path, Q_FUNC_INFO);


            if(file_type_child == MACRO_CASE_TREE_Apple_Metadata_Category_Latitude || file_type_child == MACRO_CASE_TREE_Apple_Metadata_Category_Longitude)
                tmp_db_cmd.append("c.kMDItemLatitude_Latitude,c.kMDItemLongitude_Longitude,b.mime_type From  main.files as c join filesystemDB.files as a   ON c.filesystem_record = a.INT LEFT OUTER join mimeDB.files as b ON c.filesystem_record = b.filesystem_record WHERE ((c.kMDItemLatitude_Latitude is not null AND c.kMDItemLatitude_Latitude != '') OR (c.kMDItemLongitude_Longitude is not null AND c.kMDItemLongitude_Longitude != ''))");
            else
                tmp_db_cmd.append(QString("c.") + attr_column_name + QString(",b.mime_type From main.files as c LEFT OUTER join mimeDB.files as b ON c.filesystem_record = b.filesystem_record  join filesystemDB.files as a ON c.filesystem_record = a.INT  where ((c.") + attr_column_name + QString(" is not null AND c.") + attr_column_name + QString(" != ''))"));

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Apple_Metadata_Parent) ;

            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);
            QString fs_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(fs_db_dettach_commmand);
            destination_db.close();

        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_File_Search))
        {
            m_tablewidget_obj->showColumn(enum_Extension_Index);

            QString db_name = file_type_child;
            db_name = db_name.replace(" ", "_");
            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + db_name + ".sqlite";
            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(search_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }
            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);

            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);


            tmp_db_cmd.append("b.mime_type from main.files as c LEFT OUTER join mimeDB.files as b ON c.filesystem_record = b.filesystem_record  join filesystemDB.files as a ON c.filesystem_record = a.INT  where c.source_count_name='" + source_count_name + "'");

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();


            prepare_display_for_filetypes(destination_db, filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_File_Search) ;

            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);

            QString fs_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(fs_db_dettach_commmand);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
        {
            m_tablewidget_obj->showColumn(enum_Extension_Index);

            m_tablewidget_obj->showColumn(enum_Content_search_Number_of_hits);
            m_tablewidget_obj->showColumn(enum_Content_search_Keyword_Hit);
            m_tablewidget_obj->hideColumn(enum_Hash_MD5_Index);
            m_tablewidget_obj->hideColumn(enum_Hash_SHA1_Index);
            m_tablewidget_obj->hideColumn(enum_Signature_Name_Index);
            m_tablewidget_obj->hideColumn(enum_Hashset_Name_Index);

            QString db_name = file_type_child;
            db_name = db_name.replace(" ", "_");
            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + db_name + ".sqlite";
            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(search_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);
            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);

            QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                    .arg(QDir::toNativeSeparators(mime_db_path));
            query.exec(mime_db_attach_commmand);



            tmp_db_cmd.append(" c.no_of_hits, c.keyword_name, b.mime_type from main.files as c LEFT OUTER join mimeDB.files as b ON c.filesystem_record = b.filesystem_record  join filesystemDB.files as a ON c.filesystem_record = a.INT  where c.source_count_name='" + source_count_name + "'");


            total_record_count = extract_record_count_from_case_tree_display(enum_global_case_tree_display_row_CONTENT_SEARCH, MACRO_CASE_TREE_Content_Search, file_type_child);


            prepare_display_for_filetypes(destination_db, filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Content_Search) ;

            QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
            query.exec(mime_db_dettach_commmand);
            QString fs_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(fs_db_dettach_commmand);

            // prepare_display_for_carved_files_content_search(destination_db);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        {

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS faceDB")
                    .arg(QDir::toNativeSeparators(face_analysis_db_path));
            query.exec(fs_db_attach_commmand);


            tmp_db_cmd = tmp_db_cmd.trimmed();
            if(tmp_db_cmd.endsWith(","))
                tmp_db_cmd.chop(1);

            tmp_db_cmd.append((" From main.files as a join faceDB.files as b ON a.INT = b.filesystem_record"));

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Face_Analysis_PARENT) ;

            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE faceDB");
            query.exec(filesystem_db_dettach_commmand);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Face_Search_PARENT))
        {
            label_link_searched_face->show();

            QString search_dir_name = file_type_child;
            if(search_dir_name.contains("("))
            {
                search_dir_name = search_dir_name.mid(0, search_dir_name.lastIndexOf("(")).trimmed();
            }

            search_dir_name = search_dir_name.replace(" ", "_");

            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString() + search_dir_name + QString("/") + "record.sqlite";

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(search_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }
            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
                    .arg(QDir::toNativeSeparators(filesystem_db_path));
            query.exec(fs_db_attach_commmand);

            tmp_db_cmd.append("a.mime_type from main.files as c LEFT OUTER join filesystemDB.files as a ON c.fs_record_no = a.INT  where c.source_count_name='" + source_count_name + "'");

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();


            prepare_display_for_filetypes(destination_db, filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Face_Search_PARENT) ;

            QString fs_db_dettach_commmand = QString("DETACH DATABASE filesystemDB");
            query.exec(fs_db_dettach_commmand);

            QString select_cmd = "SELECT searched_face_pic_path from filter";

            QString searched_face_pic_path = recon_helper_standard_obj->get_string_from_db_by_dbpath(select_cmd, 0, search_db_path, Q_FUNC_INFO);

            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

            QString searched_face_cmplt_picture_path = recon_static_functions::get_complete_file_path( result_dir_path , searched_face_pic_path, Q_FUNC_INFO);

            QPixmap image(searched_face_cmplt_picture_path);

            label_picture_searched_face->setPixmap(image);
            label_picture_searched_face->setScaledContents(true);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
        {
            m_tablewidget_obj->showColumn(enum_OCR_Text_Index);

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString optical_character_recognition_db_attach_commmand = QString("ATTACH DATABASE '%1' AS optical_character_recognitionDB")
                    .arg(QDir::toNativeSeparators(optical_character_recognition_db_path));
            query.exec(optical_character_recognition_db_attach_commmand);

            tmp_db_cmd = tmp_db_cmd.trimmed();
            if(tmp_db_cmd.endsWith(","))
                tmp_db_cmd.chop(1);

            tmp_db_cmd.append((" From main.files as a join optical_character_recognitionDB.files as b ON a.INT = b.filesystem_record"));

            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Optical_Character_Recognition_Parent) ;

            QString optical_character_recognition_db_dettach_commmand = QString("DETACH DATABASE optical_character_recognitionDB");
            query.exec(optical_character_recognition_db_dettach_commmand);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
        {
            m_tablewidget_obj->showColumn(enum_Extension_Index);
            m_tablewidget_obj->hideColumn(enum_Mime_Type_Index);

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);
            if(file_type_parent == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
            {
                QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS skin_tone_DB")
                        .arg(QDir::toNativeSeparators(skin_tone_detection_db_path));
                query.exec(fs_db_attach_commmand);


                tmp_db_cmd = tmp_db_cmd.trimmed();
                if(tmp_db_cmd.endsWith(","))
                    tmp_db_cmd.chop(1);

                tmp_db_cmd.append((" From main.files as a join skin_tone_DB.files as b ON a.INT = b.filesystem_record where (a.mime_type ='") + file_type_child + QString("')"));
                total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

                prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Skin_Tone_Detection_Parent) ;
            }
            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE skin_tone_DB");
            query.exec(filesystem_db_dettach_commmand);
            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Weapons_Parent))
        {

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS weaponsDB")
                    .arg(QDir::toNativeSeparators(weapons_db_path));
            query.exec(fs_db_attach_commmand);

            tmp_db_cmd = tmp_db_cmd.trimmed();
            if(tmp_db_cmd.endsWith(","))
                tmp_db_cmd.chop(1);

            tmp_db_cmd.append((" From main.files as a join weaponsDB.files as b ON a.INT = b.filesystem_record  WHERE (b.weapon_type='") + file_type_child + QString("')"));
            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Weapons_Parent);

            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE weaponsDB");
            query.exec(filesystem_db_dettach_commmand);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
        {

            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS fire_analysisDB")
                    .arg(QDir::toNativeSeparators(fire_analysis_db_path));
            query.exec(fs_db_attach_commmand);

            tmp_db_cmd = tmp_db_cmd.trimmed();
            if(tmp_db_cmd.endsWith(","))
                tmp_db_cmd.chop(1);

            tmp_db_cmd.append((" From main.files as a join fire_analysisDB.files as b ON a.INT = b.filesystem_record "));
            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db , filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Fire_Analysis_Parent) ;

            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE fire_analysisDB");
            query.exec(filesystem_db_dettach_commmand);

            destination_db.close();
        }
        else if(file_type_parent == QString(MACRO_CASE_TREE_Griffeye_Data_Parent))
        {
            QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(filesystem_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                continue;
            }

            QSqlQuery query(destination_db);

            QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS griffeyeDB")
                    .arg(QDir::toNativeSeparators(griffeye_data_db_path));
            query.exec(fs_db_attach_commmand);

            tmp_db_cmd = tmp_db_cmd.trimmed();
            if(tmp_db_cmd.endsWith(","))
                tmp_db_cmd.chop(1);

            tmp_db_cmd.append((" From main.files as a join griffeyeDB.files as b ON a.INT = b.filesystem_record WHERE (b.griffeye_category='") + file_type_child + QString("')"));
            total_record_count = tree_case_display_currentItem->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();

            prepare_display_for_filetypes(destination_db, filesystem_db_path, tmp_db_cmd, source_count_name, MACRO_CASE_TREE_Griffeye_Data_Parent);

            QString filesystem_db_dettach_commmand = QString("DETACH DATABASE griffeyeDB");
            query.exec(filesystem_db_dettach_commmand);

            destination_db.close();
        }
    }


    bool_cancel_loading = false;
    display_loading_progress_bar_obj->hide();

    gallery_viewer_obj->pub_set_file_list(&st_gallery_view_file_info_list);

    menu_manage_column = new QMenu(this);
    pushbutton_show_column->setMenu(menu_manage_column);

    QFont m_font1;
    m_font1.setPointSize(MACRO_QMenu_Font_Size_int);
    menu_manage_column->setFont(m_font1);
    connect(menu_manage_column, SIGNAL(triggered(QAction*)), this, SLOT(slot_action_manage_column_clicked(QAction*)));


    int col_count;
    col_count = m_tablewidget_obj->columnCount();

    QList<struct_manage_column_info> col_no_and_name_list;
    col_no_and_name_list = get_hidden_column_from_manage_column_db();


    QStringList hidden_column_name_list;
    for(int j = 0; j < col_no_and_name_list.size(); j++)
    {
        struct_manage_column_info obj = col_no_and_name_list.at(j);
        hidden_column_name_list << obj.hid_col_name;

    }

    for(int count = 0 ; count < col_count ; count++)
    {
        if((count == 0) || (count == 1) || (count == 2) || (count == 3) || (count == 4) || (count == (col_count - 1)) || (count == (col_count - 2)))
            continue;

        QString column_text = ((custom_table_header_widget *)(custom_header_widgets_QList.at(count)))->pub_get_column_name();

        if(m_tablewidget_obj->isColumnHidden(count) && !hidden_column_name_list.contains(column_text))
            continue;

        QAction *act  = new QAction(column_text,this->menu_manage_column);
        act->setCheckable(true);

        if(hidden_column_name_list.contains(column_text))
            act->setChecked(false);
        else
            act->setChecked(true);

        menu_manage_column->addAction(act);
    }

    m_tablewidget_obj->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tablewidget_obj->horizontalHeader(),SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slot_table_header_customContextMenuRequested(const QPoint&)));

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::set_apple_metadata_searched_filters_in_textedit()
{

    QString searched_attributes_str;
    QString db_name = file_type_child;
    db_name = db_name.replace(" ", "_");
    QString file_search_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";

    QString command = "select  mdfind_condition_str  from file_search_index where search_db_file_name = ?";
    QStringList mdfind_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,QStringList(db_name),0,file_search_index_db_path,Q_FUNC_INFO);

    QString db_path_apple_attributes = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    //QString attributes_command_str = QString("Select custom_column_name ,attribute_name From tbl_apple_attributes Where bookmark_filter = '1' or bookmark_report = '1'");
    QString attributes_command_str = QString("Select custom_column_name ,attribute_name From tbl_apple_attributes");
    QStringList all_columns_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(attributes_command_str, 0 ,db_path_apple_attributes , Q_FUNC_INFO);
    QStringList selected_display_label_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(attributes_command_str, 1 ,db_path_apple_attributes , Q_FUNC_INFO);

    if(all_columns_list.size() != selected_display_label_list.size())
        return ;

    QStringList displayed_items_list;
    bool selected_type_all = false;

    for(int ii = 0 ; ii < all_columns_list.size() ; ii++)
    {
        QString column_name = all_columns_list.at(ii);

        QString display_column_label = selected_display_label_list.at(ii);

        for(int jj = 0 ; jj < mdfind_list.size() ; jj++)
        {

            QString md_find_command_run = mdfind_list.at(jj);

            if(md_find_command_run.contains("OR"))
                selected_type_all = true;

            if(md_find_command_run.contains(column_name))
                displayed_items_list << display_column_label;

        }
    }
    if(displayed_items_list.size() > 0)
    {
        displayed_items_list.removeDuplicates();
        label_searched_apple_metadata_filters_show_link->show();
        text_edit_search_apple_metadata_filters->setMaximumWidth(200);

        if(selected_type_all)
        {
            searched_attributes_str.append("Filter Type: Any<br><br>");
            searched_attributes_str.append("Searched Attribute(s):<br>");

            for(int kk = 0 ; kk < displayed_items_list.size() ; kk++)
            {
                searched_attributes_str.append(displayed_items_list.at(kk)).append("<br>");
            }
        }
        else
        {
            searched_attributes_str.append("Filter Type: All<br><br>");
            searched_attributes_str.append("Searched Attribute(s):<br>");

            for(int kk = 0 ; kk < displayed_items_list.size() ; kk++)
            {
                searched_attributes_str.append(displayed_items_list.at(kk)).append("<br>");
            }
        }
        text_edit_search_apple_metadata_filters->setText(searched_attributes_str);
    }

    return;
}

void filetypes_loader::tablewidget_general_clicked(int row,int column)
{ // randomly clicked on any record of tablewidget to get the record number or any other info about the records
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    if(row >= m_tablewidget_obj->rowCount())
        return;

    if(!m_tablewidget_obj->selectionModel()->selectedRows(column).isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }

    }

    QString record_no = m_tablewidget_obj->item(row, enum_Record_No_Index)->text();

    QString source_count_name = m_tablewidget_obj->item(row, enum_Source_Count_Name_Index)->text();

    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no,source_count_name, Q_FUNC_INFO);


    QString file_path   =  m_tablewidget_obj->item(row,enum_File_Path_Index)->text().trimmed();


    QString content_srch_data;
    if(file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
    {
        content_srch_data = m_tablewidget_obj->item(row,enum_Content_search_Keyword_Hit)->text().trimmed();
    }

    current_media_path = file_path;
    preview_file_path = file_path;
    metadata_str       = struct_metadata_values_obj.metadata;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.db_file_path = fs_db_path;
    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.content_searched_text = content_srch_data;
    st_set_meta_obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
    st_set_meta_obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);

    emit signal_set_metadata_info(st_set_meta_obj);



    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

QString filetypes_loader::get_record_number_from_selected_row(int selected_row_no)
{
    return m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
}

QString filetypes_loader::get_source_count_name_from_selected_row(int selected_row_no)
{
    return m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();
}


void filetypes_loader::update_bookmark_value(QString status,int row, int column)
{ // update bookmark value according to add/remove
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString , 0);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    int selected_row_no = row;

    if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
        return;

    //    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    //    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    //    display_loading_progress_bar_non_cancelable_obj->show();

    QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

    if(status == "1")
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    else
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);

    QString Int_str = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
    struct_global_tagging_tag_notes_db_updation obj;
    obj.category = QString(MACRO_Plugin_Name_File_System);
    obj.record_no = Int_str;
    obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
    obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
    obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
    obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
    obj.table_name = QString("files");

    obj.source_count_name = source_count_name_str;
    obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
    obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
    obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

    list_st_bookmark_item_list.append(obj);


    QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();

    if(status == "1")
    {
        recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display ,source_count_name, Int_str);
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_fs_source_db(QStringList(Int_str), source_count_name_str);
        global_connection_manager_obj->pub_add_bookmark_for_file_system(list_st_bookmark_item_list);

    }
    else
    {
        recon_helper_standard_obj->pub_remove_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display, source_count_name, Int_str);
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_fs_source_db(QStringList(Int_str), source_count_name_str);
        global_connection_manager_obj->pub_delete_bookmark_for_file_system(list_st_bookmark_item_list);
    }

    //    display_loading_progress_bar_non_cancelable_obj->hide();


}



void filetypes_loader::update_notes_value_in_bookmark_for_filetypes(QString notes_text , bool is_remove_note)
{ // update notes value on bookmarked record for file types loader
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    notes_text = notes_text.trimmed();


    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    for(int row = 0 ; row < m_tablewidget_obj->rowCount() ; row++)
    {
        if(row % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        if(m_tablewidget_obj->item(row, bookmark_index)->checkState())
        {

            QString fs_record_INT = get_record_number_from_selected_row(row);

            QString source_count_name = get_source_count_name_from_selected_row(row);
            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

            struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


            if(is_remove_note)
            {
                emit signal_update_display_on_remove_notes(QString(MACRO_Plugin_Name_File_System), QString(MACRO_Plugin_File_System_TAB_Name_Files), fs_record_INT, QString(""), struct_info.os_scheme_display);
            }


            if(notes_text.isEmpty())
            {
                QIcon icon_notes("");
                m_tablewidget_obj->item(row, bookmark_index)->setIcon(icon_notes);
            }
            else
            {
                QIcon icon_notes("../icons/notes.png");
                m_tablewidget_obj->item(row, bookmark_index)->setIcon(icon_notes);
            }



            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(fs_db_path);

            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" -Destination Db opening ----FAILED---- " + fs_db_path , Q_FUNC_INFO);
                recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
                continue;
            }



            QString command = "update '" + QString("files") + QString("' set notes = ? where INT = ?");

            QStringList arg_list;
            arg_list  << notes_text << fs_record_INT;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,fs_db_path,Q_FUNC_INFO);


            destination_db.close();



            QStringList applied_tags_on_this_record = get_applied_tags_name_on_current_record(fs_record_INT , fs_db_path);

            emit signal_update_tag_search_for_notes(fs_record_INT, QString(MACRO_Plugin_Name_File_System), QString(MACRO_Plugin_File_System_TAB_Name_Files), notes_text, applied_tags_on_this_record, struct_info.os_scheme_display,MACRO_CASE_TREE_Source);

            QString result_note_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";


            if(!is_remove_note)
            {
                emit signal_add_notes_from_file_system(fs_record_INT, QString(MACRO_Plugin_File_System_TAB_Name_Files), result_note_db_path, notes_text ,fs_db_path, struct_info.os_scheme_display);
            }


        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}



void filetypes_loader::action_open_with_plist_viewer_triggered()
{ // open with plist viewer for plist viewer analysis of any record
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

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
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString plist_display_nm = display_file_name + QString(" [") + QString(MACRO_Plugin_Name_File_System) + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(plist_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
            return;
        }

        plist_viewer *plist_viewer_obj;
        plist_viewer_obj = new plist_viewer(this);
        QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_File_System) + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no_str;

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
// same right click actions are given which are all right click action features which we usually create in any loader, easily understand with function name
void filetypes_loader::action_open_with_hex_viewer_triggered()
{ // open with hex view clicked from right click menu
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;


    QFileInfo info(complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

    QString src_cnt_name_label = QString(" [" + source_count_name + "]");
    QString hex_display_nm = display_file_name + QString(" [") + QString(MACRO_Plugin_Name_File_System) + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

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
    hex_viewer_open_file_data_obj.record_no_str       = record_no_str;
    hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
    hex_viewer_open_file_data_obj.plugin_name_str     = QString(MACRO_Plugin_Name_File_System);
    hex_viewer_open_file_data_obj.plugin_tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);
    hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
    hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

    emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

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
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString sqlite_display_nm = display_file_name + QString(" [") + QString(MACRO_Plugin_Name_File_System) + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(sqlite_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);

            return;
        }

        QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_File_System) + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no_str;

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

void filetypes_loader::action_open_with_log_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString adhoc_log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString).toString();

    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
    if(st_globl_recon_file_info_obj.bool_file_is_dir)
        return;

    QString log_file_complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(log_file_complete_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File not exist");
        message_dialog_obj->show();
        return;
    }

    QString temp_p = log_file_complete_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".log"))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not log file.");
        message_dialog_obj->show();
        return;
    }
    else
    {
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString log_display_name = display_file_name + QString(" [") + QString(MACRO_Plugin_Name_File_System) + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

        QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(adhoc_log_viewer_sqlite_path, Q_FUNC_INFO);
        if(list_log_display_name.contains(log_display_name))
        {
            emit signal_show_file_in_viewer_table_result(log_display_name,MACRO_CASE_TREE_Log_Viewer, source_count_name);
            return;
        }

        log_viewer *log_viewer_obj;
        log_viewer_obj = new log_viewer(this);
        QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_File_System) + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no_str;

        log_viewer_obj->pub_set_result_log_viewer_sqlite_path(adhoc_log_viewer_sqlite_path);
        log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
        log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
        log_viewer_obj->pub_set_source_count_name(source_count_name);
        log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        log_viewer_obj->pub_set_essentials();

        log_viewer_obj->pub_start_log_extraction();

        emit signal_show_file_in_viewer_table_result(log_display_name,MACRO_CASE_TREE_Log_Viewer, source_count_name);
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_sendToBucket_log_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;

    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no ,fs_db_path,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString log_file_complete_path = st_globl_recon_file_info_obj.complete_file_path;
        all_path_list << log_file_complete_path;

        for(int pos = 0; pos < all_path_list.size(); pos++)
        {
            QString log_file_complete_path = all_path_list.at(pos);

            QFileInfo info(log_file_complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            QString temp_p = log_file_complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".log"))
            {
                message_list << QString("'" +  info.fileName() + "' is not log file.");
                continue;
            }
            else
            {
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString log_display_name = display_file_name + QString(" [") + plugin_name_str + QString(" ") + record_no + QString("]") + src_cnt_name_label;

                QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(log_viewer_sqlite_path, Q_FUNC_INFO);
                if(list_log_display_name.contains(log_display_name))
                {
                    continue;
                }

                log_viewer *log_viewer_obj;
                log_viewer_obj = new log_viewer(this);
                QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no;

                log_viewer_obj->pub_set_result_log_viewer_sqlite_path(log_viewer_sqlite_path);
                log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
                log_viewer_obj->pub_set_log_file_path(log_file_complete_path);
                log_viewer_obj->pub_set_source_count_name(source_count_name);
                log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                log_viewer_obj->pub_set_essentials();

                log_viewer_obj->pub_start_log_extraction();

                emit signal_viewer_extarction_completed_table_result(log_display_name,MACRO_CASE_TREE_Log_Viewer);
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

void filetypes_loader::action_sendToBucket_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;

    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

        QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no ,fs_db_path,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

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

                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString plist_display_nm = display_file_name+ QString(" [") + plugin_name_str + QString(" ") + record_no + QString("]") + src_cnt_name_label;

                QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
                if(list_plist_display_name.contains(plist_display_nm))
                {
                    continue;
                }

                plist_viewer *plist_viewer_obj;
                plist_viewer_obj = new plist_viewer(this);
                QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + QString("files") + MACRO_RECON_Splitter_1_multiple + record_no;

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


void filetypes_loader::action_sendToBucket_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;

    for(int row = 0; row < selection_model_list.size(); row++)
    {
        QString tab_name_str;
        QStringList all_path_list;

        if(row % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(row);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        // QString plugin_name_str = file_type_child;
        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

        QString record_no_str = get_record_number_from_selected_row(selected_row_no);

        QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        tab_name_str  = QString(MACRO_Plugin_File_System_TAB_Name_Files);
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


            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString hex_display_nm = display_file_name + QString(" [") + plugin_name_str + QString(" ") + record_no_str + QString("]") + src_cnt_name_label;

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
            hex_viewer_open_file_data_obj.record_no_str       = record_no_str;
            hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
            hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name_str;
            hex_viewer_open_file_data_obj.plugin_tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);
            hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
            hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

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

void filetypes_loader::action_sendToBucket_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QStringList all_path_list;

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);
        QString tab_name_str;

        QString record_no_str = get_record_number_from_selected_row(selected_row_no);

        QString source_count_name = get_source_count_name_from_selected_row(selected_row_no);
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

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
                tab_name_str = QString(MACRO_Plugin_File_System_TAB_Name_Files);

                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString sqlite_display_nm = display_file_name + QString(" [") + plugin_name_str + QString(" ") + record_no + QString("]") + src_cnt_name_label;

                QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
                if(list_sqlite_display_name.contains(sqlite_display_nm))
                {
                    continue;
                }

                QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

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

void filetypes_loader::action_sendToBucket_registry_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait....");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QString reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no,enum_Record_No_Index)->text();
        QString source_count_name_str = m_tablewidget_obj->item(selected_row_no,enum_Source_Count_Name_Index)->text();
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_str + QString("/file_system.sqlite");
        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no ,fs_db_path,source_count_name_str);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            continue;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File does not exist");
            message_dialog_obj->show();
            continue;
        }

        if(!recon_static_functions::check_is_it_registry_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not a registry file."));
            message_dialog_obj->show();
            continue;
        }
        else
        {

            QString src_cnt_name_label = QString(" [" + source_count_name_str + "]");
            QString display_tab_name_str = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;


            QString command = "select display_tab_name from 'tbl_registry_viewer_index'";
            QStringList list_registry_display_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,reg_sqlite_path,Q_FUNC_INFO);
            if(list_registry_display_name.contains(display_tab_name_str))
            {
                continue;
            }

            QString tab_name_str  = QString(MACRO_Plugin_File_System_TAB_Name_Files);
            QString display_tab_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;
            QString reg_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString();
            QString destination_db_path = reg_dir_path + source_count_name_str;
            QDir dir;
            dir.mkpath(destination_db_path);

            QString tm_file_name = info.fileName();

            if(tm_file_name.contains(" "))
                tm_file_name.replace(" " , "_");
            if(tm_file_name.contains("."))
                tm_file_name.replace("." , "_");



            QString sqlite_name =  tm_file_name + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") + "_" + record_no + ".sqlite";
            destination_db_path = destination_db_path + "/" + sqlite_name;



            ////----do not change order of calling
            registry_viewer *registry_viewer_obj;
            registry_viewer_obj = new registry_viewer(this);
            registry_viewer_obj->pub_set_destination_db_path(destination_db_path);
            registry_viewer_obj->pub_set_case_tree_tab_name(display_tab_name);
            registry_viewer_obj->pub_set_source_file_path(complete_path);
            registry_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            registry_viewer_obj->pub_set_source_count_name(source_count_name_str);
            registry_viewer_obj->pub_set_registry_destination_path(reg_sqlite_path);
            registry_viewer_obj->pub_set_essentials();

            registry_viewer_obj->pub_parse_windows_registry();

            if(!QFile::exists(destination_db_path))
            {
                message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("Unable to open Registry File ") +  "'" +  info.fileName() + "'");
                message_dialog_obj->show();
                continue;
            }
            emit signal_viewer_extarction_completed_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void filetypes_loader::action_open_with_registry_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait....");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString adhoc_reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;


        QString record_no = m_tablewidget_obj->item(selected_row_no,enum_Record_No_Index)->text();
        QString source_count_name = m_tablewidget_obj->item(selected_row_no,enum_Source_Count_Name_Index)->text();
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no ,fs_db_path,source_count_name);
        if(st_globl_recon_file_info_obj.bool_file_is_dir)
            return;

        QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
        QString plugin_name_str = QString(MACRO_Plugin_Name_File_System);
        QFileInfo info(complete_path);
        if(!info.exists())
        {
            recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' File does not exist");
            message_dialog_obj->show();
            continue;
        }

        if(!recon_static_functions::check_is_it_registry_file_by_signature(complete_path, Q_FUNC_INFO))
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("'" +  info.fileName() + "' is not a registry file."));
            message_dialog_obj->show();
            continue;
        }
        else
        {

            QString src_cnt_name_label = QString(" [" + source_count_name + "]");
            QString display_tab_name_str = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

            QString command = "select display_tab_name from 'tbl_registry_viewer_index'";


            //// if file alread added
            QString reg_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
            QStringList list_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,reg_sqlite_path,Q_FUNC_INFO);
            if(list_name.contains(display_tab_name_str))
            {
                emit signal_show_file_in_viewer_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer, source_count_name);
                continue;
            }

            //// if file not added
            QStringList list_plist_display_name = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,adhoc_reg_sqlite_path,Q_FUNC_INFO);
            if(list_plist_display_name.contains(display_tab_name_str))
            {
                emit signal_show_file_in_viewer_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer, source_count_name);
                continue;
            }

            QString tab_name_str  = QString(MACRO_Plugin_File_System_TAB_Name_Files);
            QString display_tab_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;
            QString reg_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString();

            QString destination_db_path_str = reg_dir_path + source_count_name;
            QDir dir;
            dir.mkpath(destination_db_path_str);

            QString tm_file_name = info.fileName();

            if(tm_file_name.contains(" "))
                tm_file_name.replace(" " , "_");
            if(tm_file_name.contains("."))
                tm_file_name.replace("." , "_");


            QString sqlite_name =  tm_file_name + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") + "_" + record_no + ".sqlite";
            destination_db_path_str = destination_db_path_str + "/" + sqlite_name;



            ////----do not change order of calling
            registry_viewer *registry_viewer_obj;
            registry_viewer_obj = new registry_viewer(this);
            registry_viewer_obj->pub_set_destination_db_path(destination_db_path_str);
            registry_viewer_obj->pub_set_case_tree_tab_name(display_tab_name);
            registry_viewer_obj->pub_set_source_file_path(complete_path);
            registry_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            registry_viewer_obj->pub_set_source_count_name(source_count_name);
            registry_viewer_obj->pub_set_registry_destination_path(adhoc_reg_sqlite_path);
            registry_viewer_obj->pub_set_essentials();

            registry_viewer_obj->pub_parse_windows_registry();

            if(!QFile::exists(destination_db_path_str))
            {
                message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, QString("Unable to open Registry File ") +  "'" +  info.fileName() + "'");
                message_dialog_obj->show();
                continue;
            }
            emit signal_show_file_in_viewer_table_result(display_tab_name_str,MACRO_CASE_TREE_Registry_Viewer, source_count_name);

        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void filetypes_loader::update_tags_value(QString tag_data,QString colour)
{ // update tags value in database weather tags is added or removed pr if added what color is it
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList source_count_name_list;


    QList<struct_global_tagging_tag_notes_db_updation> list_st_tag_item_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString source_count_name_str = get_source_count_name_from_selected_row(selected_row_no);

        source_count_name_list << source_count_name_str;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,enum_Tag_Icon_Index)->setIcon(icon);



        struct_global_tagging_tag_notes_db_updation obj;
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.source_count_name = source_count_name_str;
        obj.item0 = m_tablewidget_obj->item(selected_row_no, enum_File_Name_Index)->text().trimmed();
        obj.item1 = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();
        obj.item2 = m_tablewidget_obj->item(selected_row_no, enum_Date_Accessed_Index)->text().trimmed();

        list_st_tag_item_list.append(obj);

    }

    if(list_st_tag_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    source_count_name_list.removeDuplicates();

    for(int count = 0; count < source_count_name_list.size(); count++)
    {
        QString source_count_name_str = source_count_name_list.at(count);

        QStringList INT_list;

        for(int num = 0; num < list_st_tag_item_list.size(); num++)
        {
            if(list_st_tag_item_list.at(num).source_count_name == source_count_name_str)
            {
                INT_list.append(list_st_tag_item_list.at(num).record_no);
            }
        }

        global_connection_manager_obj->pub_set_tag_in_fs_source_db(INT_list, source_count_name_str, tag_data);
    }

    global_connection_manager_obj->pub_add_tag_for_file_system(list_st_tag_item_list, tag_data);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void filetypes_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no_str ,fs_db_path,source_count_name);
    QString complete_path = st_globl_recon_file_info_obj.complete_file_path;

    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    int preview_index = get_stack_index_for_preview(complete_path ,display_file_path);

    struct_global_fetch_metadata  struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no_str, source_count_name, Q_FUNC_INFO);
    QString metadata_str = struct_metadata_values_obj.metadata;


    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_add_saved_maps_in_case_tree()), this, SIGNAL(signal_add_saved_maps_in_case_tree()));
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    connect(full_detail_info_obj, SIGNAL(signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString, QString)), this, SLOT(slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString, QString)));

    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str,"",preview_index,QString(""),"", record_no_str, MACRO_Plugin_Name_File_System, "");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void filetypes_loader::action_open_detach_detailed_info_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    //    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    //    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());

    //    struct_GLOBAL_derived_source_info struct_info = global_target_sources_info_class_obj->pub_get_derived_info_structure_according_source_count_name(source_count_name);

    //    QString os_name_str = struct_info.os_naming_scheme;

    //    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    //    QString filepath = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), enum_File_Path_Index)->text();

    //    QStringList pathlist;
    //    pathlist << filepath;


    //    QString inital_path = global_recon_helper_singular_class_obj->get_file_system_initial_path_by_giving_source_count_name(source_count_name, Q_FUNC_INFO);

    //    QString searched_keyword = searched_keyword_str;

    //    if(os_name_str == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
    //    {

    //        QString idpath = current_media_path;
    //        QString current_media_linked_file_ios = preview_file_path;

    //        struct_GLOBAL_derived_source_info struct_target_info;
    //        struct_target_info = global_target_sources_info_class_obj->pub_get_derived_info_structure_according_source_count_name(source_count_name);

    //        struct_global_fetch_metadata struct_metadata_values_obj;
    //        struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no_str,source_count_name, Q_FUNC_INFO);

    //        QString fs_metadata_str = struct_metadata_values_obj.metadata;

    //        int preview_index_full;// = get_stack_index_for_preview(current_media_linked_file_ios);

    //        if(filepath.size() > 0)
    //        {
    //            emit signal_open_in_detach_clicked_table_result(fs_metadata_str, idpath, current_media_linked_file_ios, preview_index_full,searched_keyword,source_count_name);

    //        }
    //        else
    //        {
    //            emit signal_open_in_detach_clicked_table_result(fs_metadata_str, QString("") , current_media_linked_file_ios, preview_index_full,searched_keyword,source_count_name);
    //        }

    //    }
    //    else
    //    {

    //        filepath.prepend(inital_path);

    //        struct_GLOBAL_derived_source_info struct_target_info;
    //        struct_target_info = global_target_sources_info_class_obj->pub_get_derived_info_structure_according_source_count_name(source_count_name);

    //        struct_global_fetch_metadata struct_metadata_values_obj;
    //        struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no_str,source_count_name, Q_FUNC_INFO);

    //        QString fs_metadata_str = struct_metadata_values_obj.metadata;


    //        int preview_index_full;// = get_stack_index_for_preview(filepath);

    //        if(filepath.size() > 0)
    //        {
    //            emit signal_open_in_detach_clicked_table_result(fs_metadata_str, filepath, filepath, preview_index_full,searched_keyword,source_count_name);
    //        }
    //        else
    //        {
    //            emit signal_open_in_detach_clicked_table_result(fs_metadata_str, QString("") , filepath, preview_index_full,searched_keyword,source_count_name);
    //        }

    //    }



    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


QStringList filetypes_loader::get_applied_tags_name_on_current_record(QString record_no , QString fs_db_path)
{ // get tag value from current selected record from tablewidget

    QStringList tag_name_list;

    record_no = record_no.trimmed();

    if(record_no.isEmpty())
        return tag_name_list;


    QString command = QString("SELECT recon_tag_value FROM '") + QString("files") + QString("' WHERE INT=?");
    QStringList arg_list;
    arg_list << record_no;
    QString applied_tag = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,fs_db_path,Q_FUNC_INFO);

    if(!applied_tag.isEmpty())
    {
        tag_name_list << applied_tag;
    }

    return tag_name_list;
}

void filetypes_loader::prepare_display_for_filetypes(QSqlDatabase &destination_db , QString fs_db_path, QString received_cmd, QString source_count_name, QString m_caller)
{ // prepare display for file types from database and set everything on tablewidget and tablewidget header

    recon_static_functions::app_debug(" -Starts ", Q_FUNC_INFO);


    QSqlQuery query(destination_db);
    query.prepare(received_cmd);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + received_cmd,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return ;
    }

    QSqlDatabase fs_db;
    if(destination_db.databaseName() == fs_db_path)
    {
        fs_db = destination_db;
    }
    else
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        fs_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        fs_db.setDatabaseName(fs_db_path);
        if(!fs_db.open())
        {
            recon_static_functions::app_debug(" fiel system ----FAILED---- " + fs_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + fs_db.lastError().text(),Q_FUNC_INFO);
            // donot use return or continue
        }
    }


    QList<struct_manage_column_info> column_no_and_name_list;
    column_no_and_name_list = get_hidden_column_from_manage_column_db();
    for(int i= 0; i < column_no_and_name_list.size(); i++)
    {
        struct_manage_column_info var = column_no_and_name_list.at(i);
        m_tablewidget_obj->hideColumn(var.hid_col_no);
    }


    // crash free check post
    if(total_record_count <= 0)
        total_record_count = 1;


    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;


    QString source_count_from_fs_db_path = source_count_name;
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_from_fs_db_path);

    int column_count = query.record().count();

    int last_index = column_count - 1;
    int second_last_index  = column_count - 2;
    int third_last_index   = column_count - 3;
    int fourth_last_index  = column_count - 4;
    int fifth_last_index   = column_count - 5;
    int sixth_last_index   = column_count - 6;

    //    if(total_record_count > 1500)
    //    {
    //        disable_widgets(true);
    //        emit signal_disable_case_tree_sidebar(true);
    //    }


    prepare_hashset_db_data_list();
    QStringList hashsets_to_be_hidden = get_hashsets_list_to_be_hidden();

    bool bool_highlight_user_opened_files = global_narad_muni_class_obj->get_field(MACRO_NARAD_Preferences_User_Opened_Files_bool).toBool();

    pub_refresh_horizontal_header();

    while(query.next())
    {
        /// This order of values is manged from CSV(Filetypes.csv). If order
        /// changed from CSV make respective changes here

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

        QStringList matched_hashset_name_list;

        QString md5_str  = query.value(enum_Hash_MD5_Index).toString().trimmed();
        QString sha1_str = query.value(enum_Hash_SHA1_Index).toString().trimmed();

        ///get matched hashset names
        for(int num = 0; num < hashset_db_data_list.size(); num++)
        {
            struct_global_hashset_db_data hashset_obj = hashset_db_data_list.at(num);
            QString command_str = "Select " + hashset_obj.hashet_column_name + " From " + hashset_obj.hashet_table_name + " Where " + hashset_obj.hashet_column_name + " = ?";

            QString matched_hash_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, QStringList(md5_str), 0, hashset_obj.hashset_db_path, Q_FUNC_INFO);
            matched_hash_str = matched_hash_str.trimmed();

            if(!matched_hash_str.isEmpty())
                matched_hashset_name_list << hashset_obj.hash_name_str;
        }
        /// hide record for hidden hashset
        if(check_fs_record_hidden_for_hashset(hashsets_to_be_hidden, matched_hashset_name_list))
            continue;


        QString bookmark_str    = query.value(enum_Bookmarks_Index).toString().trimmed();
        QString recon_tag_str   = query.value(enum_Tag_Icon_Index).toString().trimmed();
        QString INT_str         = query.value(enum_Record_No_Index).toString().trimmed();
        QString filename_str    = query.value(enum_File_Name_Index).toString().trimmed();
        QString extension_str   = query.value(enum_Extension_Index).toString().trimmed();
        QString appl_latitude_str    = query.value(third_last_index).toString().trimmed();
        QString appl_longitude_str   = query.value(second_last_index).toString().trimmed();
        QString wherefrom_str   = query.value(second_last_index).toString().trimmed();
        //        QString exif_metadata_str    = query.value(second_last_index).toString().trimmed();

        //        QString make_str     = query.value(third_last_index).toString().trimmed();
        //        QString model_str    = query.value(second_last_index).toString().trimmed();
        QString make_str ,model_str,exif_metadata_str;
        if(m_caller == MACRO_CASE_TREE_Exif_Metadata_Search)
        {
            exif_metadata_str    = query.value(sixth_last_index).toString().trimmed();
            make_str     = query.value(fifth_last_index).toString().trimmed();
            model_str    = query.value(fourth_last_index).toString().trimmed();

        }
        else
        {
            exif_metadata_str    = query.value(second_last_index).toString().trimmed();
            make_str     = query.value(third_last_index).toString().trimmed();
            model_str    = query.value(second_last_index).toString().trimmed();
        }

        QString exif_latitude_str     = query.value(third_last_index).toString().trimmed();
        QString exif_longitude_str    = query.value(second_last_index).toString().trimmed();

        QString content_search_no_of_hits_str    = query.value(third_last_index).toString().trimmed();
        QString content_search_keyword_hit_str   = query.value(second_last_index).toString().trimmed();

        QString filepath_str    = query.value(enum_File_Path_Index).toString().trimmed();
        QString filesize_str    = query.value(enum_File_Size_Index).toString().trimmed();
        QString mime_type_str   = query.value(enum_Mime_Type_Index).toString().trimmed();
        QString date_modified_str = query.value(enum_Date_Modified_Index).toString().trimmed();
        QString date_change_str   = query.value(enum_Date_Change_Index).toString().trimmed();
        QString date_accessed_str = query.value(enum_Date_Accessed_Index).toString().trimmed();
        QString date_created_str = query.value(enum_Date_Created_Index).toString().trimmed();
        QString date_added_str = query.value(enum_Date_Added_Index).toString().trimmed();
        QString date_ccd_str = query.value(enum_Content_Creation_Date_Index).toString().trimmed();

        QString date_cmd_str = query.value(enum_Content_Modification_Date_Index).toString().trimmed();
        QString date_lused_str = query.value(enum_Last_Used_Date_Index).toString().trimmed();
        QString use_count_str = query.value(enum_Use_Count_Index).toString().trimmed();
        QString source_name_str  = query.value(enum_Source_Name_Index).toString().trimmed();
        QString source_count_name_str = query.value(enum_Source_Count_Name_Index).toString().trimmed();
        QString signature_name_str    = query.value(third_last_index).toString().trimmed();
        QString signature_value_str         = query.value(second_last_index).toString().trimmed();
        QString file_uncomprsn_str    = query.value(enum_File_Decompression_Status_Index).toString().trimmed();
        QString ocr_text_str    = query.value(enum_OCR_Text_Index).toString().trimmed();


        QString notes_str             = query.value(enum_Notes_Value_Index).toString().trimmed();
        QString apple_metadata_str    = query.value(second_last_index).toString().trimmed();



        QString column_name = query.record().fieldName(second_last_index);

        if(column_name.contains("Date", Qt::CaseInsensitive))
        {
            apple_metadata_str =  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(apple_metadata_str, Q_FUNC_INFO);
        }

        QString blank_str             = "";


        int row = m_tablewidget_obj->rowCount();
        m_tablewidget_obj->insertRow(row);
        m_tablewidget_obj->setRowHeight(row, 22);

        QTableWidgetItem *item_bk        = new QTableWidgetItem;
        QTableWidgetItem *item_INT       = new QTableWidgetItem;
        QTableWidgetItem *item_tag_icon      = new QTableWidgetItem;
        QTableWidgetItem *item_filename  = new QTableWidgetItem;
        QTableWidgetItem *item_extension  = new QTableWidgetItem;
        QTableWidgetItem *item_filepath  = new QTableWidgetItem;
        QTableWidgetItem *item_mimetype  = new QTableWidgetItem;
        QTableWidgetItem *item_date_modified = new QTableWidgetItem;
        QTableWidgetItem *item_date_change      = new QTableWidgetItem;
        QTableWidgetItem *item_date_accessed    = new QTableWidgetItem;
        QTableWidgetItem *item_date_created  = new QTableWidgetItem;

        QTableWidgetItem *item_date_added    = new QTableWidgetItem;
        QTableWidgetItem *item_date_ccd    = new QTableWidgetItem;
        QTableWidgetItem *item_date_cmd    = new QTableWidgetItem;
        QTableWidgetItem *item_date_lused    = new QTableWidgetItem;
        QTableWidgetItem *item_use_count    = new QTableWidgetItem;

        QTableWidgetItem *item_file_size         = new QTableWidgetItem;
        QTableWidgetItem *item_source_count_name      = new QTableWidgetItem;
        QTableWidgetItem *item_source_name       = new QTableWidgetItem;
        QTableWidgetItem *item_file_signature_value    = new QTableWidgetItem;
        QTableWidgetItem *item_file_signature_name    = new QTableWidgetItem;
        QTableWidgetItem *item_apple_latitude    = new QTableWidgetItem;
        QTableWidgetItem *item_apple_longitude   = new QTableWidgetItem;
        QTableWidgetItem *item_wherefrom   = new QTableWidgetItem;
        QTableWidgetItem *item_make   = new QTableWidgetItem;
        QTableWidgetItem *item_model  = new QTableWidgetItem;
        QTableWidgetItem *item_exif_latitude = new QTableWidgetItem;
        QTableWidgetItem *item_exif_longitude = new QTableWidgetItem;
        QTableWidgetItem *item_notes_icon = new QTableWidgetItem;
        QTableWidgetItem *item_notes_value = new QTableWidgetItem;
        QTableWidgetItem *item_md5 = new QTableWidgetItem;
        QTableWidgetItem *item_sha1 = new QTableWidgetItem;
        QTableWidgetItem *item_hashset_name = new QTableWidgetItem;
        QTableWidgetItem *item_apple_metadata_value = new QTableWidgetItem;
        QTableWidgetItem *item_exif_metadata_value = new QTableWidgetItem;
        QTableWidgetItem *item_no_of_hits = new QTableWidgetItem;
        QTableWidgetItem *item_keyword_hit = new QTableWidgetItem;
        QTableWidgetItem *item_fileuncomprsn_status  = new QTableWidgetItem;
        QTableWidgetItem *item_blank = new QTableWidgetItem;
        QTableWidgetItem *item_ocr_text = new QTableWidgetItem;

        if(m_caller == MACRO_CASE_TREE_Exif_Parent)
        {
            item_exif_metadata_value->setForeground(Qt::red);
            item_make->setForeground(Qt::red);
            item_model->setForeground(Qt::red);
            item_exif_latitude->setForeground(Qt::red);
            item_exif_longitude->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Exif_Metadata_Search)
        {
            item_exif_metadata_value->setForeground(Qt::red);
            item_make->setForeground(Qt::red);
            item_model->setForeground(Qt::red);
            item_exif_latitude->setForeground(Qt::red);
            item_exif_longitude->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Redefined_Result_Category_Locations)
        {
            item_exif_latitude->setForeground(Qt::red);
            item_exif_longitude->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions || m_caller == MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions)
        {
            item_extension->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_MIME_Type)
        {
            item_mimetype->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_File_Size_Parent)
        {
            item_file_size->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_File_Signature_Parent)
        {
            item_file_signature_name->setForeground(Qt::red);
            item_file_signature_value->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Apple_Metadata_Parent)
        {
            item_apple_metadata_value->setForeground(Qt::red);

            item_apple_latitude->setForeground(Qt::red);
            item_apple_longitude->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Hashset)
        {
            item_md5->setForeground(Qt::red);
            item_sha1->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Griffeye_Data_Parent)
        {
            item_md5->setForeground(Qt::red);
            item_sha1->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Content_Search)
        {
            item_no_of_hits->setForeground(Qt::red);
            item_keyword_hit->setForeground(Qt::red);
        }
        else if(m_caller == MACRO_CASE_TREE_Optical_Character_Recognition_Parent)
        {
            item_ocr_text->setForeground(Qt::red);
        }

        item_make->setText(make_str);
        item_model->setText(model_str);
        //        item_exif_latitude->setText(exif_latitude_str);
        //        item_exif_longitude->setText(exif_longitude_str);

        item_exif_latitude->setData(Qt::EditRole , exif_latitude_str.toDouble());
        item_exif_latitude->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);

        item_exif_longitude->setData(Qt::EditRole , exif_longitude_str.toDouble());
        item_exif_longitude->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);

        QString decom_level = "Decompressed";
        if(file_uncomprsn_str == MACRO_TRUE_VALUE_NUMERIC_QString)
            file_uncomprsn_str = decom_level;
        else
            file_uncomprsn_str = "";

        item_fileuncomprsn_status->setText(file_uncomprsn_str);

        if(file_type_child == "Used Count")
        {
            item_apple_metadata_value->setData(Qt::EditRole , apple_metadata_str.toLongLong());
            item_apple_metadata_value->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        }
        else
        {
            item_apple_metadata_value->setText(apple_metadata_str);
        }

        item_exif_metadata_value->setText(exif_metadata_str);

        item_apple_latitude->setData(Qt::EditRole , appl_latitude_str.toDouble());
        item_apple_latitude->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_apple_longitude->setData(Qt::EditRole , appl_longitude_str.toDouble());
        item_apple_longitude->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_wherefrom->setText(wherefrom_str);


        item_no_of_hits->setData(Qt::EditRole , content_search_no_of_hits_str.toDouble());
        item_no_of_hits->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_keyword_hit->setText(content_search_keyword_hit_str);

        item_extension->setText(extension_str);

        item_file_signature_name->setText(signature_name_str);
        item_file_signature_value->setText(signature_value_str);

        item_bk->setCheckState(Qt::Unchecked);
        if(bookmark_str == QString("1"))
        {
            item_bk->setCheckState(Qt::Checked);
            recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display,source_count_name, INT_str);
        }



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


        item_INT->setData(Qt::EditRole , INT_str.toLongLong());

        item_INT->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);

        item_filename->setText(filename_str);
        item_filepath->setText(filepath_str);

        item_mimetype->setText(mime_type_str);
        item_md5->setText(md5_str);
        item_sha1->setText(sha1_str);
        item_hashset_name->setText(matched_hashset_name_list.join(","));

        item_date_modified->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_modified_str, Q_FUNC_INFO));
        item_date_change->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_change_str, Q_FUNC_INFO));
        item_date_accessed->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_accessed_str, Q_FUNC_INFO));
        item_date_created->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_created_str, Q_FUNC_INFO));
        item_date_added->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_added_str, Q_FUNC_INFO));
        item_date_ccd->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_ccd_str, Q_FUNC_INFO));
        item_date_cmd->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_cmd_str, Q_FUNC_INFO));
        item_date_lused->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_lused_str, Q_FUNC_INFO));

        item_ocr_text->setText(ocr_text_str);

        if(use_count_str.isEmpty())
            item_use_count->setText(use_count_str);
        else
        {
            item_use_count->setData(Qt::EditRole , use_count_str.toLongLong());
            item_use_count->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        }

        bool bool_dir_record = recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(INT_str,fs_db,"files",source_name_str,Q_FUNC_INFO);

        if(bool_dir_record)
        {
            item_file_size->setText("--");
        }
        else
        {
            item_file_size->setData(Qt::EditRole , filesize_str.toLongLong());
        }
        item_file_size->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_source_count_name->setText(source_name_str);
        item_source_name->setText(struct_info.complete_source_name);

        QString file_full_path;
        QString thumbnail_file_full_path;

        bool bool_is_it_file = false;
        QString fs_record_no;

        QIcon icon_path = recon_helper_standard_obj->get_qicon_of_record_no_by_dbreference(INT_str,fs_db,"files",source_name_str,Q_FUNC_INFO);
        item_filename->setIcon(icon_path);

        if(!bool_dir_record)
        {
            file_full_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(INT_str,fs_db,"files",source_name_str,Q_FUNC_INFO);
            fs_record_no = INT_str;
            bool_is_it_file = true;
        }


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
        item_blank->setText(blank_str);

        item_tag_icon->setToolTip(recon_tag_str);
        item_notes_icon->setToolTip(notes_str);

        m_tablewidget_obj->setItem(row , enum_Bookmarks_Index,item_bk);
        m_tablewidget_obj->setItem(row , enum_Record_No_Index,item_INT);
        m_tablewidget_obj->setItem(row , enum_File_Name_Index,item_filename);
        m_tablewidget_obj->setItem(row , enum_Extension_Index,item_extension);
        m_tablewidget_obj->setItem(row , enum_File_Path_Index,item_filepath);
        m_tablewidget_obj->setItem(row , enum_Mime_Type_Index,item_mimetype);
        m_tablewidget_obj->setItem(row , enum_Date_Modified_Index,item_date_modified);
        m_tablewidget_obj->setItem(row , enum_Date_Change_Index,item_date_change);
        m_tablewidget_obj->setItem(row , enum_Date_Accessed_Index,item_date_accessed);
        m_tablewidget_obj->setItem(row , enum_Date_Created_Index,item_date_created);
        m_tablewidget_obj->setItem(row , enum_Date_Added_Index,item_date_added);
        m_tablewidget_obj->setItem(row , enum_Content_Creation_Date_Index,item_date_ccd);
        m_tablewidget_obj->setItem(row , enum_Content_Modification_Date_Index,item_date_cmd);
        m_tablewidget_obj->setItem(row , enum_Last_Used_Date_Index,item_date_lused);
        m_tablewidget_obj->setItem(row , enum_Use_Count_Index,item_use_count);

        m_tablewidget_obj->setItem(row , enum_File_Size_Index,item_file_size);
        m_tablewidget_obj->setItem(row , enum_Hashset_Name_Index,item_hashset_name);
        m_tablewidget_obj->setItem(row , enum_Hash_MD5_Index,item_md5);
        m_tablewidget_obj->setItem(row , enum_Hash_SHA1_Index,item_sha1);
        m_tablewidget_obj->setItem(row , enum_Tag_Icon_Index,item_tag_icon);
        m_tablewidget_obj->setItem(row , enum_Source_Count_Name_Index,item_source_count_name);
        m_tablewidget_obj->setItem(row , enum_Source_Name_Index,item_source_name);
        m_tablewidget_obj->setItem(row , enum_Signature_Name_Index,item_file_signature_name);
        m_tablewidget_obj->setItem(row , enum_Signature_Value_Index,item_file_signature_value);
        m_tablewidget_obj->setItem(row , enum_Apple_Latitude_Index,item_apple_latitude);
        m_tablewidget_obj->setItem(row , enum_Apple_Longitude_Index,item_apple_longitude);
        m_tablewidget_obj->setItem(row , enum_WhereFrom_Index,item_wherefrom);

        m_tablewidget_obj->setItem(row , enum_Content_search_Number_of_hits,item_no_of_hits);
        m_tablewidget_obj->setItem(row , enum_Content_search_Keyword_Hit,item_keyword_hit);

        m_tablewidget_obj->setItem(row , enum_Exif_Make_Index,item_make);
        m_tablewidget_obj->setItem(row , enum_Exif_Model_Index,item_model);
        m_tablewidget_obj->setItem(row , enum_Exif_Latitude_Index,item_exif_latitude);
        m_tablewidget_obj->setItem(row , enum_Exif_Longitude_Index,item_exif_longitude);
        m_tablewidget_obj->setItem(row , enum_Notes_Icon_Index,item_notes_icon);
        m_tablewidget_obj->setItem(row , enum_Notes_Value_Index,item_notes_value);
        m_tablewidget_obj->setItem(row , enum_Apple_Metadata_Column_Index,item_apple_metadata_value);
        m_tablewidget_obj->setItem(row , enum_Exif_Metadata_Column_Index,item_exif_metadata_value);
        m_tablewidget_obj->setItem(row , enum_File_Decompression_Status_Index,item_fileuncomprsn_status);
        m_tablewidget_obj->setItem(row , enum_Blank_Column_Index,item_blank);
        m_tablewidget_obj->setItem(row , enum_OCR_Text_Index, item_ocr_text);

        m_tablewidget_obj->setColumnWidth(enum_Apple_Metadata_Column_Index, 200);

        int pos_seen = enum_Notes_Value_Index + 1;
        QString is_seen_str = query.value(pos_seen).toString().trimmed();

        if(is_seen_str == QString::number(1))
            change_background_for_seen_unseen(row , false);

        if(item_fileuncomprsn_status->text() == QString(decom_level))
            change_textcolor_for_compression_decompression(row , true);


        if(bool_highlight_user_opened_files)
        {
            bool bool_used_count_available = false;
            use_count_str = use_count_str.trimmed();

            if(!use_count_str.isEmpty() && use_count_str.toInt() > 0)
                bool_used_count_available = true;

            if(!date_lused_str.isEmpty() && bool_used_count_available)
                change_row_background_color_for_last_used_date(row);
        }

        if(bool_is_it_file)
        {
            if(mime_type_str.startsWith("image/") || recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
            {
                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();
                f1.file_path = item_filepath->text();
                f1.INT = item_INT->text();
                f1.source_count_name = item_source_count_name->text();
                f1.table_view_row_no = row;
                f1.latitude = exif_latitude_str;
                f1.longitude = exif_longitude_str;
                f1.thumbnail_file_path = file_full_path;
                f1.full_filepath = file_full_path;
                f1.bookmark = "0";
                f1.last_access_timestamp = item_date_accessed->text();
                f1.index = st_gallery_view_file_info_list.size();


                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }
            else if(mime_type_str.startsWith("video/") ||recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(item_filename->text()))
            {
                QString file_size = item_file_size->text();

                QString video_thumbnails_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

                QString video_thumbnail_db_path = video_thumbnails_dir_path + "video_thumbnails.sqlite";

                QString file_name = item_filename->text();

                QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
                QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnail_db_path,Q_FUNC_INFO);

                if(!thumb_file_path.isEmpty())
                {
                    thumbnail_file_full_path = result_filepath + thumb_file_path;
                }
                else
                {
                    file_name.remove(file_name.lastIndexOf("."),file_name.size());
                    video_thumbnails_dir_path = video_thumbnails_dir_path + file_name + "_" + fs_record_no + ".png";

                    //                    QString thumb_script_path = "../Resources/scripts_persistent/video_frame.py";
                    //                    QString cmd_python_path = global_varibale_python_path;

                    //                    QProcess process_thumbnails;

                    //                    QStringList args;
                    //                    args << thumb_script_path << file_full_path << video_thumbnails_dir_path << file_size;

                    QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                    QProcess process_thumbnails;

                    QStringList args;
                    args << file_full_path << video_thumbnails_dir_path << file_size;

                    //                    process_thumbnails.start(cmd_python_path,args);
                    process_thumbnails.start(video_frame_exe_path,args);
                    if(!process_thumbnails.waitForStarted())
                    {
                        recon_static_functions::app_debug("process_thumbnails -----FAILED----- ",Q_FUNC_INFO);
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


                    thumbnail_file_full_path = QFileInfo(video_thumbnails_dir_path).absoluteFilePath();
                    QFileInfo thumb_file_info(thumbnail_file_full_path);
                    QString thumb_filepath;
                    QStringList list;
                    QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";
                    if(thumb_file_info.exists())
                    {
                        thumb_filepath.clear();
                        thumb_filepath = thumbnail_file_full_path;
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_db_path,Q_FUNC_INFO);
                    }
                    else
                    {
                        thumb_filepath.clear();
                        QFile tmp_file("../Resources/features_resources/video_thumbnail/default_video_thumbnail.png");
                        tmp_file.copy(video_thumbnails_dir_path);
                        thumb_filepath = QFileInfo(video_thumbnails_dir_path).absoluteFilePath();
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list.clear();
                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnail_db_path,Q_FUNC_INFO);
                    }
                }

                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();

                f1.file_path = item_filepath->text();
                f1.thumbnail_file_path = thumbnail_file_full_path;
                f1.full_filepath = file_full_path;
                f1.INT = item_INT->text();
                f1.table_view_row_no = row;
                f1.bookmark = "0";
                f1.latitude = "";
                f1.longitude = "";
                f1.last_access_timestamp = item_date_accessed->text();
                f1.source_count_name = source_count_name;
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }
        }
    }

    bool_did_tableview_display_changed = true;

    QList<struct_manage_column_info> column_no_and_name_list_1;
    column_no_and_name_list_1 = get_hidden_column_from_manage_column_db();
    for(int i= 0; i < column_no_and_name_list_1.size(); i++)
    {
        struct_manage_column_info var = column_no_and_name_list_1.at(i);
        m_tablewidget_obj->hideColumn(var.hid_col_no);
    }

    //    disable_widgets(false);
    //    emit signal_disable_case_tree_sidebar(false);

    // mean fs_db is opened in this function
    if(destination_db.databaseName() != fs_db_path)
    {
        fs_db.close();
    }

    if(!bool_columns_ajust_complete)
    {
        bool_columns_ajust_complete = true;
        adjust_important_columns_according_to_feature_in_table_display();
    }

    recon_static_functions::app_debug(" -Ends ", Q_FUNC_INFO);

}

void filetypes_loader::adjust_important_columns_according_to_feature_in_table_display()
{ // we can adjust columns series acoording to us by moving them first to last or anywhere else and update them in the  database also
    if(file_type_parent == QString(MACRO_CASE_TREE_Exif_Parent))
    {

        if(file_type_child == MACRO_CASE_TREE_Exif_Category_Make_Model)
        {
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Make_Index, (enum_File_Size_Index + 1));
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Model_Index, (enum_File_Size_Index + 2));
        }
        else if(file_type_child == MACRO_CASE_TREE_Exif_Category_GPS)
        {
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Latitude_Index, (enum_File_Size_Index + 1));
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Longitude_Index, (enum_File_Size_Index + 2));
        }
        else
        {
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Metadata_Column_Index, (enum_File_Size_Index + 1));
        }
    }
    else if(file_type_child == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations) && file_type_parent == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Latitude_Index, (enum_File_Size_Index + 1));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Longitude_Index, (enum_File_Size_Index + 2));
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions))
    {


    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Metadata_Column_Index, (enum_File_Size_Index + 1));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Make_Index, (enum_File_Size_Index + 2));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Model_Index, (enum_File_Size_Index + 3));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Latitude_Index, (enum_File_Size_Index + 4));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Exif_Longitude_Index, (enum_File_Size_Index + 5));

    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions))
    {

    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_MIME_Type))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Mime_Type_Index, (enum_File_Size_Index + 1));
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_File_Size_Parent))
    {

    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_File_Signature_Parent))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Signature_Name_Index, (enum_File_Size_Index + 1));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Signature_Value_Index, (enum_File_Size_Index + 2));
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Hashset))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Hash_MD5_Index, (enum_File_Size_Index + 1));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Hash_SHA1_Index, (enum_File_Size_Index + 2));
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Apple_Metadata_Parent))
    {

        if(file_type_child == MACRO_CASE_TREE_Apple_Metadata_Category_Latitude || file_type_child == MACRO_CASE_TREE_Apple_Metadata_Category_Longitude)
        {
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Apple_Latitude_Index, (enum_File_Size_Index + 1));
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Apple_Longitude_Index, (enum_File_Size_Index + 2));
        }
        else
        {
            m_tablewidget_obj->horizontalHeader()->moveSection(enum_Apple_Metadata_Column_Index, (enum_File_Size_Index + 1));
        }
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_File_Search))
    {
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Content_Search))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Content_search_Number_of_hits, (enum_File_Size_Index + 1));
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_Content_search_Keyword_Hit, (enum_File_Size_Index + 2));
    }
    else if(file_type_parent == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
    {
        m_tablewidget_obj->horizontalHeader()->moveSection(enum_OCR_Text_Index, (enum_File_Size_Index + 1));
    }


}

bool filetypes_loader::check_record_already_exists_in_display(QString filename_str , QString INT_str, QString source_count_name)
{
    bool record_found = false;

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        if(m_tablewidget_obj->item(row , enum_Record_No_Index)->text() == INT_str
                && m_tablewidget_obj->item(row , enum_File_Name_Index)->text() == filename_str
                && m_tablewidget_obj->item(row , enum_Source_Count_Name_Index)->text() == source_count_name)
        {
            record_found = true;
            break;
        }
    }

    return record_found;
}

qint64 filetypes_loader::get_header_column_index(QString header)
{

    int index = -1;
    for(int count = 0; count < display_header_name_list.size(); count++)
    {
        if(header == display_header_name_list.at(count).trimmed())
        {
            index = count;
            break;
        }
    }

    return index;
}

QString filetypes_loader::get_file_size_display_command(QString file_size_category)
{ // prepare file size display command to display file size of any record on tablewidget

    QString command_size = QString(" Where file_size ") ;

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");


    QString command_str = QString("Select greater_than_size, greater_than_size_unit, less_than_size, less_than_size_unit, start_size, start_size_unit, end_size, end_size_unit From file_size_filters Where filter_name = ?");

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        command_size.clear();
        return command_size;
    }

    QSqlQuery query(destination_db);
    query.prepare(command_str);
    query.addBindValue(file_size_category);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command_str,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        command_size.clear();
        return command_size;
    }


    while(query.next())
    {

        QString greater_than_size_str  = query.value(0).toString();
        QString greater_than_size_unit_str = query.value(1).toString();

        QString less_than_size_str = query.value(2).toString();
        QString less_than_size_unit_str = query.value(3).toString();

        QString between_start_size_str = query.value(4).toString();
        QString between_start_size_unit_str = query.value(5).toString();

        QString between_end_size_str = query.value(6).toString();
        QString between_end_size_unit_str = query.value(7).toString();

        ///greater than
        if(!greater_than_size_str.isEmpty() && less_than_size_str.isEmpty())
        {
            if(greater_than_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                qint64 start_size_num = 0;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(greater_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    start_size_num = greater_than_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;

                command_size.append(" > " + QString::number(start_size_num));

            }
            else
            {
                command_size.append(" > " + QString::number(greater_than_size_str.toInt()));
            }

        }

        ///less than
        if(!less_than_size_str.isEmpty() && greater_than_size_str.isEmpty())
        {
            if(less_than_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                qint64 end_size_num = 0;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(less_than_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    end_size_num = less_than_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;

                command_size.append(" < " + QString::number(end_size_num));
            }
            else
            {
                command_size.append(" < " + QString::number(less_than_size_str.toInt()));
            }
        }

        ///in between
        if(!between_start_size_str.isEmpty() && !between_end_size_str.isEmpty())
        {

            qint64 start_size_num = 0;
            if(between_start_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(between_start_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    start_size_num = between_start_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;

            }
            else
            {
                start_size_num = between_start_size_str.toInt();
            }


            qint64 end_size_num = 0;
            if(between_end_size_unit_str != QString(MACRO_Generic_Filter_Condition_Bytes))
            {
                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_KB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_KB_in_byte_int;

                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_MB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_MB_in_byte_int;

                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_GB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_GB_in_byte_int;

                if(between_end_size_unit_str == QString(MACRO_Generic_Filter_Condition_TB))
                    end_size_num = between_end_size_str.toInt() * MACRO_Generic_Value_TB_in_byte_int;
            }
            else
            {
                end_size_num = between_end_size_str.toInt();
            }

            command_size.append(" between " + QString::number(start_size_num) + QString(" AND ") + QString::number(end_size_num));
        }

    }

    destination_db.close();

    return command_size.trimmed();
}

bool filetypes_loader::attach_fs_db_in_memory(QString fs_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    in_memory_fs_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    in_memory_fs_db.setDatabaseName(":memory:");
    if(!in_memory_fs_db.open())
    {
        recon_static_functions::app_debug(" temp Open in memory -------FAILED------ " + in_memory_fs_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(in_memory_fs_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }



    QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(fs_db_path));

    QSqlQuery query_fs_attach(in_memory_fs_db);
    query_fs_attach.prepare(fs_db_attach_commmand);
    if(!query_fs_attach.exec())
    {
        recon_static_functions::app_debug(" query memory attach  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_fs_attach.lastError().text(),Q_FUNC_INFO);
        in_memory_fs_db.close();
        return false;
    }





    //    QString command_str = QString("Create table files as select * from filesystemDB.files");

    //    QSqlQuery query_create_table(in_memory_fs_db);
    //    query_create_table.prepare(command_str);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create  --------FAILED------ db file - ",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + command_str,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        in_memory_fs_db.close();
    //        return false;
    //    }


    return true;

}

qint64 filetypes_loader::extract_record_count_from_case_tree_display(int feature_position, QString feature_name, QString sub_feature_name)
{ // extract the record counts when we expand or click on case tree data which displays the data names and records with it's numbers

    qint64 record_count = 0;

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(feature_position);
        if(!item)
            continue;


        if(item->text(enum_global_case_tree_display_column_ZERO).trimmed() == QString(feature_name).trimmed())
        {

            for(int i = 0; i < item->childCount(); i++)
            {
                QTreeWidgetItem *child_item = item->child(i);

                QString t_sub_feature_name = child_item->text(enum_global_case_tree_display_column_ZERO);
                t_sub_feature_name.remove(t_sub_feature_name.indexOf("("), t_sub_feature_name.size());
                t_sub_feature_name = t_sub_feature_name.trimmed();

                if(t_sub_feature_name == sub_feature_name.trimmed())
                {
                    record_count = child_item->text(enum_global_case_tree_display_column_RECORD_COUNT).toLongLong();
                    break;
                }
            }
            break;
        }
    }

    return record_count;

}

void filetypes_loader::set_data_in_text_edit_searched_filter(QString clicked_text)
{ // set by default data in text edit search filter so we can search accordingly
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString file_search_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(file_search_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + file_search_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        return;
    }

    QSqlQuery query_select(destination_db);
    query_select.prepare("SELECT entered_file_name, entered_file_size, entered_creation_date_config, entered_modification_date_config,"
                         "entered_added_date_config, entered_last_used_date_config, is_radio_button_or_clicked, mdfind_condition_str "
                         "FROM file_search_index WHERE search_label_name = ?");
    query_select.addBindValue(clicked_text);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query_select.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_select.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Query -  " + query_select.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QString display_str;
    while(query_select.next())
    {
        QString local_display_str = QString("<b><font color = \"blue\">Regular Filters : </font></b><br>");

        QString entered_file_name = query_select.value(0).toString();
        QString entered_file_size = query_select.value(1).toString();
        QString entered_creation_date = query_select.value(2).toString();
        QString entered_modification_date = query_select.value(3).toString();
        QString entered_added_date = query_select.value(4).toString();
        QString entered_last_used_date = query_select.value(5).toString();
        bool bool_is_radio_AND = query_select.value(6).toBool();
        QString selected_mdfind_str = query_select.value(7).toString();

        // For File Name
        if(!entered_file_name.isEmpty())
        {
            QStringList tmp_list = entered_file_name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(tmp_list.size() > 1)
            {
                QString filter_entry = tmp_list.at(0);
                QString filter_type = tmp_list.at(1);

                display_str += "<b>" + QObject::tr("File Name (") + filter_type + ") " + ":</b>" + QString("\t") + filter_entry +"<br>";
            }
        }

        // For File Size
        if(!entered_file_size.isEmpty())
        {
            QStringList tmp_list = entered_file_size.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(tmp_list.size() > 1)
            {
                QString filter_type = tmp_list.at(0);
                QString secon_str = tmp_list.at(1);

                QStringList sec_temp_list = secon_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

                if(sec_temp_list.size() > 1)
                {
                    QString size_value = sec_temp_list.at(0);
                    QString third_tem_str = sec_temp_list.at(1);

                    if(third_tem_str.contains(MACRO_RECON_Splitter_3_dollar_in_brace))
                    {
                        QStringList third_temp_list = third_tem_str.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

                        if(third_temp_list.size() > 1)
                        {
                            QString secon_size_value = third_temp_list.at(0);
                            QString suffix = third_temp_list.at(1);
                            display_str += "<b>" + QObject::tr("File Size (") + filter_type + ") " + ":</b>" + QString("\t") + size_value + " - " + secon_size_value + " " + suffix + "<br>";
                        }
                    }
                    else
                    {
                        display_str += "<b>" + QObject::tr("File Size (") + filter_type + ") " + ":</b>" + QString("\t") + size_value + " " + third_tem_str +"<br>";
                    }
                }
            }
        }

        // For Creation Date
        display_str += get_text_edit_string_for_dates(entered_creation_date, "Creation date");

        // For Last Modification Date
        display_str += get_text_edit_string_for_dates(entered_modification_date, "Last modified date");

        // For Added Date
        display_str += get_text_edit_string_for_dates(entered_added_date, "Added date");

        // For Added Date
        display_str += get_text_edit_string_for_dates(entered_last_used_date, "Last read date");

        // For And/OR Combination
        if(!display_str.isEmpty())
        {
            if(bool_is_radio_AND)
                display_str += "<b>" + QObject::tr("Combined by ") + ":</b>" + QString("\t") + "AND" +"<br>";
            else
                display_str += "<b>" + QObject::tr("Combined by ") + ":</b>" + QString("\t") + "OR" +"<br>";
        }

        if(!display_str.isEmpty())
            display_str.prepend(local_display_str);

        /// Spotlight Search Filters Display
        display_str += get_text_edit_display_str_for_spotlight_filters(selected_mdfind_str);
    }

    if(display_str.startsWith("<br>"))
        display_str.remove(0,4);

    text_edit_search_filters->setText(display_str);
    destination_db.close();

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

}

QString filetypes_loader::get_widget_type_name_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator)
{
    QString display_str;

    if(joined_operator == "==")
    {
        if(selected_value.startsWith("*") && selected_value.endsWith("*"))
        {
            QString tmp_value_str = selected_value;
            tmp_value_str.chop(1);
            tmp_value_str.remove(0,1);
            display_str += "<b>" + attr_display_name + QString(" (contains) : ") + "</b>" + tmp_value_str + "<br>";
        }
        else if(selected_value.startsWith("*") && !selected_value.endsWith("*"))
        {
            QString tmp_value_str = selected_value;
            tmp_value_str.remove(0,1);
            display_str += "<b>" + attr_display_name + QString(" (ends with) : ") + "</b>" + tmp_value_str + "<br>";
        }
        else if(!selected_value.startsWith("*") && selected_value.endsWith("*"))
        {
            QString tmp_value_str = selected_value;
            tmp_value_str.chop(1);
            display_str += "<b>" + attr_display_name + QString(" (begins with) : ") + "</b>" + tmp_value_str + "<br>";
        }
        else
            display_str += "<b>" + attr_display_name + QString(" (matches) : ") + "</b>" + selected_value + "<br>";
    }
    else if(joined_operator == "!=")
    {
        display_str += "<b>" + attr_display_name + QString(" (is not) : ") + "</b>" + selected_value + "<br>";
    }

    return display_str;
}

QString filetypes_loader::get_text_edit_string_for_dates(QString entered_date_str, QString header_name)
{
    QString display_str;
    if(!entered_date_str.isEmpty())
    {
        QStringList tmp_list = entered_date_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(tmp_list.size() > 1)
        {
            QString filter_type = tmp_list.at(0);
            QString secon_str = tmp_list.at(1);

            if(secon_str.contains(MACRO_RECON_Splitter_2_hash_in_brace))
            {
                QStringList sec_temp_list = secon_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

                if(sec_temp_list.size() > 1)
                {
                    QString first_date_value = sec_temp_list.at(0);
                    QString second_date_val = sec_temp_list.at(1);

                    if(filter_type == MACRO_Generic_Filter_Condition_Between)
                        display_str += "<b>" + header_name + " (" + filter_type + ") " + ":</b>" + QString("\t") + first_date_value + " - " + second_date_val + "<br>";
                    else
                        display_str += "<b>" + header_name + " (" + filter_type + ") " + ":</b>" + QString("\t") + first_date_value + " " + second_date_val + "<br>";
                }
            }
            else
            {
                display_str += "<b>" + header_name + " (" + filter_type + ") " + ":</b>" + QString("\t") + secon_str + "<br>";
            }
        }
    }

    return display_str;
}

QString filetypes_loader::get_text_edit_display_str_for_spotlight_filters(QString selected_mdfind_str)
{
    QString display_str;
    QString local_display_str = QString("<br><b><font color = \"blue\">Spotlight Filters : </font></b><br>");

    QString case_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(case_config_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open---FAILED----" + case_config_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug( destination_db.lastError().text(), Q_FUNC_INFO);
        return "";
    }

    if(!selected_mdfind_str.isEmpty())
    {
        QStringList splitted_list = selected_mdfind_str.split("&&", Qt::SkipEmptyParts);

        for(int i = 0; i < splitted_list.size(); i++)
        {
            QString extended_attr_name;
            QString joined_operator;
            QString selected_value;

            QString selected_mdfind_arg = splitted_list.at(i);

            selected_mdfind_arg = selected_mdfind_arg.trimmed();

            if(selected_mdfind_arg.contains("\""))
            {
                QStringList tmp_list = selected_mdfind_arg.split("\"", Qt::SkipEmptyParts);

                if(tmp_list.size() > 1)
                {
                    QString attr_with_operator_str = tmp_list.at(0);
                    attr_with_operator_str = attr_with_operator_str.trimmed();

                    selected_value = tmp_list.at(1);

                    QStringList splited_attr_list = attr_with_operator_str.split(" ");
                    if(splited_attr_list.size() > 1)
                    {
                        extended_attr_name = splited_attr_list.at(0);
                        joined_operator = splited_attr_list.at(1);
                    }
                }
            }
            else
            {
                QStringList tmp_splitted_list = selected_mdfind_arg.split(" ");

                if(tmp_splitted_list.size() > 2)
                {
                    extended_attr_name = tmp_splitted_list.at(0);
                    joined_operator = tmp_splitted_list.at(1);
                    selected_value = tmp_splitted_list.at(2);
                }
            }

            QString attr_display_name;
            if(extended_attr_name == "kMDItemContentTypeTree")
            {
                attr_display_name = "Kind";
            }
            else
            {
                QString command = "SELECT attribute_name FROM tbl_apple_attributes WHERE metadata_attribute_name = ?";
                QStringList arg_list;
                arg_list << extended_attr_name;
                attr_display_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command, arg_list, 0, destination_db, Q_FUNC_INFO);
            }

            QString command = "SELECT widget_type FROM tbl_apple_attributes WHERE attribute_name = ?";
            QStringList arg_list;
            arg_list << attr_display_name;
            QString widget_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command, arg_list, 0, destination_db, Q_FUNC_INFO);

            int widget_tupe_int = widget_type.toInt();
            switch(widget_tupe_int)
            {
            case 0:
                //  Widget Type Name
                display_str += get_widget_type_name_filter_display_string(selected_value, attr_display_name, joined_operator);
                break;
            case 1:
                break;
            case 2:
                //  Widget Type Range
                display_str += get_widget_type_range_filter_display_string(selected_value, attr_display_name, joined_operator);
                break;
            case 3:
                //  Widget Type Duraion
                display_str += get_widget_type_duration_filter_display_string(selected_value, attr_display_name, joined_operator);
                break;
            case 4:
                //  Widget Type Size
                display_str += get_widget_type_size_filter_display_string(selected_value, attr_display_name, joined_operator);
                break;
            case 5:
                break;
            case 6:
                //  Widget Type Kind
                display_str += get_widget_type_kind_filter_display_string(selected_value, attr_display_name, joined_operator);
                break;
            case 7:
                //  Widget Type Content
                display_str += "<b>" + attr_display_name + " : </b>" + selected_value + "<br>";
                break;
            case 8:
                //  Widget Type Date
                display_str += get_widget_type_date_filter_display_string(selected_value, attr_display_name, joined_operator);
                break;
            }
        }
    }

    if(!display_str.isEmpty())
        display_str.prepend(local_display_str);

    destination_db.close();

    return display_str;
}

QString filetypes_loader::get_widget_type_date_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator)
{
    QString display_str;

    if(selected_value.startsWith("$time.iso(") && joined_operator == "==")
    {
        QString value_str = selected_value;
        value_str.replace("$time.iso(", "");
        value_str.chop(1);

        display_str += "<b>" + attr_display_name + QString(" (Exactly) : ") + "</b>" + value_str + "<br>";
    }
    else if(selected_value.startsWith("$time.iso(") && joined_operator == "<=")
    {
        QString value_str = selected_value;
        value_str.replace("$time.iso(", "");
        value_str.chop(1);

        display_str += "<b>" + attr_display_name + QString(" (Before) : ") + "</b>" + value_str + "<br>";
    }
    else if(selected_value.startsWith("$time.iso(") && joined_operator == ">=")
    {
        QString value_str = selected_value;
        value_str.replace("$time.iso(", "");
        value_str.chop(1);

        display_str += "<b>" + attr_display_name + QString(" (After) : ") + "</b>" + value_str + "<br>";
    }
    else if(selected_value.startsWith("$time.today") && joined_operator == "<=")
    {
        display_str += "<b>" + attr_display_name + QString(" : ") + "</b>" + QString("Today") + "<br>";
    }
    else if(selected_value.startsWith("$time.yesterday") && joined_operator == "<=")
    {
        display_str += "<b>" + attr_display_name + QString(" : ") + "</b>" + QString("Yesterday") + "<br>";
    }
    else if(selected_value.startsWith("$time.this_week") && joined_operator == "<=")
    {
        display_str += "<b>" + attr_display_name + QString(" : ") + "</b>" + QString("This Week") + "<br>";
    }
    else if(selected_value.startsWith("$time.this_month") && joined_operator == "<=")
    {
        display_str += "<b>" + attr_display_name + QString(" : ") + "</b>" + QString("This Month") + "<br>";
    }
    else if(selected_value.startsWith("$time.this_year") && joined_operator == "<=")
    {
        display_str += "<b>" + attr_display_name + QString(" : ") + "</b>" + QString("This Year") + "<br>";
    }
    else if(selected_value.startsWith("$time.today(") && joined_operator == ">=")
    {
        QString value_str = selected_value;
        value_str.replace("$time.today(", "");
        value_str.chop(1);

        if(value_str.startsWith("-"))
            value_str.remove(0,1);

        display_str += "<b>" + attr_display_name + QString(" (within last) : ") + "</b>" + value_str + " days" + "<br>";
    }
    else if(selected_value.startsWith("$time.this_month(") && joined_operator == ">=")
    {
        QString value_str = selected_value;
        value_str.replace("$time.this_month(", "");
        value_str.chop(1);

        if(value_str.startsWith("-"))
            value_str.remove(0,1);

        display_str += "<b>" + attr_display_name + QString(" (within last) : ") + "</b>" + value_str + " months" + "<br>";
    }
    else if(selected_value.startsWith("$time.this_year(") && joined_operator == ">=")
    {
        QString value_str = selected_value;
        value_str.replace("$time.this_year(", "");
        value_str.chop(1);

        if(value_str.startsWith("-"))
            value_str.remove(0,1);

        display_str += "<b>" + attr_display_name + QString(" (within last) : ") + "</b>" + value_str + " years" + "<br>";
    }

    return display_str;
}

void filetypes_loader::disable_widgets(bool status)
{
    lineEdit_search->setDisabled(status);
    pushbutton_search_filters->setDisabled(status);
    pushbutton_show_all->setDisabled(status);
    pushButton_search->setDisabled(status);
}

void filetypes_loader::loading_display_dialog_based_on_records(int total_records, int processed_count)
{


    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

    if(processed_count > 10 && !bool_cancel_loading)
        display_loading_progress_bar_obj->show();

    if(total_records <= processed_count)
        display_loading_progress_bar_obj->hide();

}

QString filetypes_loader::get_widget_type_kind_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator)
{
    QString display_str;

    if(selected_value == "public.item" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Any" + "<br>";
    else if(selected_value == "*PDF*" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> PDF" + "<br>";
    else if(selected_value == "*archive" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Archive" + "<br>";
    else if((selected_value == "public.executable" && joined_operator == "==") || (selected_value == "public.executable" && joined_operator == "!=") || (selected_value == "com.apple.mach-o-binary" &&  joined_operator == "!="))
        display_str += "<b>" + attr_display_name + " : </b> Executable" + "<br>";
    else if(selected_value == "*presentation" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Presentation" + "<br>";
    else if(selected_value == "*movie" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Movie" + "<br>";
    else if(selected_value == "*image" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Image (All)" + "<br>";
    else if(selected_value == "JPEG image" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Image (JPEG)" + "<br>";
    else if(selected_value == "TIFF image" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Image (TIFF)" + "<br>";
    else if(selected_value == "Portable Network Graphics image" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Image (PNG)" + "<br>";
    else if(selected_value == "Graphics Interchange Format (GIF)" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Image (GIF)" + "<br>";
    else if(selected_value == "Windows bitmap image" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Image (BMP)" + "<br>";
    else if(selected_value == "*audio" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Audio (All)" + "<br>";
    else if(selected_value == "MP3 audio" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Audio (MP3)" + "<br>";
    else if(selected_value == "AAC audio" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Audio (AAC)" + "<br>";
    else if(selected_value == "purchased*" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Audio (Purchased)" + "<br>";
    else if((selected_value == "public.text" && joined_operator == "==") || (selected_value == "public.object-code" && joined_operator == "!="))
        display_str += "<b>" + attr_display_name + " : </b> Text (All)" + "<br>";
    else if(selected_value == "public.plain-text" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Text (Plain Text)" + "<br>";
    else if(selected_value == "HTML document" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Text (HTML)" + "<br>";
    else if(selected_value == "Rich Text Document" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Rich Text Document" + "<br>";
    else if(selected_value == "public.xml" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Text (XML)" + "<br>";
    else if(selected_value == "public.source-code" && joined_operator == "==")
        display_str += "<b>" + attr_display_name + " : </b> Text (Source Code)" + "<br>";
    else
        display_str += "<b>" + attr_display_name + " : </b> " + selected_value + "<br>";

    return display_str;
}

QString filetypes_loader::get_widget_type_size_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator)
{
    QString display_str;

    if(joined_operator == "==")
        display_str += "<b>" + attr_display_name + QString(" (equal) : ") + "</b>" + recon_static_functions::human_readable_size(selected_value,Q_FUNC_INFO) + "<br>";
    else if(joined_operator == ">")
        display_str += "<b>" + attr_display_name + QString(" (greater than) : ") + "</b>" + recon_static_functions::human_readable_size(selected_value,Q_FUNC_INFO) + "<br>";
    else if(joined_operator == "<")
        display_str += "<b>" + attr_display_name + QString(" (less than) : ") + "</b>" + recon_static_functions::human_readable_size(selected_value,Q_FUNC_INFO) + "<br>";
    else if(joined_operator == "!=")
        display_str += "<b>" + attr_display_name + QString(" (not equal) : ") + "</b>" + recon_static_functions::human_readable_size(selected_value,Q_FUNC_INFO) + "<br>";

    return display_str;
}

QString filetypes_loader::get_widget_type_duration_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator)
{
    QString display_str;

    if(joined_operator == "==")
        display_str += "<b>" + attr_display_name + QString(" (equal) : ") + "</b>" + recon_static_functions::get_human_readable_time(selected_value, Q_FUNC_INFO) + "<br>";
    else if(joined_operator == ">")
        display_str += "<b>" + attr_display_name + QString(" (greater than) : ") + "</b>" + recon_static_functions::get_human_readable_time(selected_value, Q_FUNC_INFO) + "<br>";
    else if(joined_operator == "<")
        display_str += "<b>" + attr_display_name + QString(" (less than) : ") + "</b>" + recon_static_functions::get_human_readable_time(selected_value, Q_FUNC_INFO) + "<br>";
    else if(joined_operator == "!=")
        display_str += "<b>" + attr_display_name + QString(" (not equal) : ") + "</b>" + recon_static_functions::get_human_readable_time(selected_value, Q_FUNC_INFO) + "<br>";

    return display_str;
}


QString filetypes_loader::get_widget_type_range_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator)
{
    QString display_str;

    if(joined_operator == "==")
        display_str += "<b>" + attr_display_name + QString(" (equal) : ") + "</b>" + selected_value + "<br>";
    else if(joined_operator == ">")
        display_str += "<b>" + attr_display_name + QString(" (greater than) : ") + "</b>" + selected_value + "<br>";
    else if(joined_operator == "<")
        display_str += "<b>" + attr_display_name + QString(" (less than) : ") + "</b>" + selected_value + "<br>";
    else if(joined_operator == "!=")
        display_str += "<b>" + attr_display_name + QString(" (not equal) : ") + "</b>" + selected_value + "<br>";

    return display_str;
}

void filetypes_loader::add_applications_to_open_application_menu()
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
                submenu_open_with_external_application->clear();

                for(int num = 0; num < application_list.size(); num++)
                {
                    QAction *act_submenu_app =  new QAction(QFileInfo(application_list.at(num)).fileName().toLocal8Bit(), this);
                    act_submenu_app->setIcon(icon_provide.icon(QFileInfo(application_list.at(num))));
                    submenu_open_with_external_application->addAction(act_submenu_app);
                    submenu_open_with_external_application->setToolTip(application_list.at(num));
                }


                act_submenu->setMenu(submenu_open_with_external_application);
                break;
            }

        }
    }
    else
    {
        submenu_open_with_external_application->clear();

        QList<QAction*> actions_list = submenu_open_with->actions();

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

QString filetypes_loader::get_current_complete_file_path()
{
    QString record_no_str = get_record_number_from_selected_row(m_tablewidget_obj->currentRow());

    QString source_count_name = get_source_count_name_from_selected_row(m_tablewidget_obj->currentRow());
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db_path,"files",source_count_name,Q_FUNC_INFO);

    return complete_path;
}

void filetypes_loader::action_extract_compressed_file()
{
    recon_static_functions::debug_intensive(" start ", Q_FUNC_INFO);

    int selected_row_no = m_tablewidget_obj->currentRow();

    if((selected_row_no < 0) || (selected_row_no >= m_tablewidget_obj->rowCount()))
        return;

    if(m_tablewidget_obj->isRowHidden(selected_row_no))
        return;



    QString decom_level = "Decompressed";
    QString itm_val = m_tablewidget_obj->item(selected_row_no, enum_File_Decompression_Status_Index)->text().trimmed();
    if(itm_val.trimmed() == QString(decom_level))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "File already decompressed. Do double click to see content.");
        message_dialog_obj->show();
        return;
    }


    QString record = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
    QString source_count_name = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();
    QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + "/file_system.sqlite";

    decompress_file_system_obj->pub_set_db_essentials(db_path_filesystem, QSqlDatabase());
    decompress_file_system_obj->pub_set_source_count_name(source_count_name);
    decompress_file_system_obj->pub_set_record_no_for_decompression(record);

    decompress_file_system_obj->pub_set_decompress_file_path(m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed());
    decompress_file_system_obj->pub_set_treewidget_case_display_obj(treewidget_case_display_obj);

    QString m_value = decompress_file_system_obj->start_run();

    if(m_value == MACRO_PASS_VALUE_READABLE_QString)
    {
        m_tablewidget_obj->item(selected_row_no, enum_File_Decompression_Status_Index)->setText(decom_level);
        change_textcolor_for_compression_decompression(selected_row_no , true);

        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "Decompression has been done. Do you want to open it?"));
        message_dialog_obj->show();

        recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
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
            struct_GLOBAL_witness_info_source st_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
            QString file_display_path = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

            emit signal_decompressed_files_view(st_source_info.source_name,QString(MACRO_CASE_TREE_Source),file_display_path,record, db_path_filesystem, source_count_name);
        }

    }


    recon_static_functions::debug_intensive(" end ", Q_FUNC_INFO);

}

QStringList filetypes_loader::make_kml_report()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList list_paths_to_message_box;
    for(int i = 0; i < selection_model_list.size(); i++)
    {

        QModelIndex model_index = selection_model_list.at(i);

        int row = model_index.row();

        if(row < 0 || row > m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        QString display_file_path =  m_tablewidget_obj->item(row,enum_File_Path_Index)->text().trimmed();

        QString latitude;
        QString longitude;

        if(!display_file_path.trimmed().isEmpty())
        {
            QString source_count_name = m_tablewidget_obj->item(row,enum_Source_Count_Name_Index)->text();
            QString record_no =   m_tablewidget_obj->item(row,enum_Record_No_Index)->text().trimmed();
            QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + source_count_name + "/file_system.sqlite";

            QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,db_path_filesystem,"files",source_count_name ,Q_FUNC_INFO);


            QStringList my_list;
            my_list << "Latitude";
            my_list << "Longitude";

            QMap<QString, QString> map_obj =  exif_data_image_files_obj->pub_extract_specific_attributes(complete_file_path, my_list);
            QMap <QString, QString>::iterator mapiter;
            for(mapiter = map_obj.begin(); mapiter != map_obj.end() ; mapiter++)
            {
                if(mapiter.key() == "Latitude")
                {
                    latitude = mapiter.value();
                }
                else if(mapiter.key() == "Longitude")
                {
                    longitude = mapiter.value();
                }
            }


            if(!display_file_path.startsWith("/"))
                display_file_path.prepend("/");


            if(latitude.trimmed().isEmpty() && longitude.trimmed().isEmpty())
                continue ;

        }


        QString full_cordinates = longitude + "," + latitude + ",0";
        QString file_name = QFileInfo(display_file_path).fileName() + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")) + QString(".kml");
        QString kml_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString();
        file_name = recon_static_functions::get_available_filename(file_name , kml_file_path , Q_FUNC_INFO);
        kml_file_path = kml_file_path + file_name;


        QFile file(kml_file_path);
        if(!file.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug(" file open -----FAILED---" + file.fileName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(" error" + file.errorString() , Q_FUNC_INFO);
            continue;
        }

        list_paths_to_message_box << kml_file_path;



        QString firstline = QString("<?xml version=") + "\"" + "1.0" + "\"" + " encoding=" + "\""+ "UTF-8" +"\"" + "?>";
        QString secondline = QString("<kml xmlns=")+ "\""+ "http://www.opengis.net/kml/2.2"+ "\"" + ">";
        QString new_line = "\n";

        file.write(firstline.toLocal8Bit());
        file.write(new_line.toLocal8Bit());
        file.write(secondline.toLocal8Bit());
        file.write(new_line.toLocal8Bit());

        QString buffer;
        QXmlStreamWriter stream_kml(&buffer);
        stream_kml.setAutoFormatting(true);
        stream_kml.writeStartElement("Document"); //Document Start


        ///============Do Not Remove===============

        //    &lt;b &gt;Record No.: &lt;/b &gt;11&lt;br /&gt;
        //    &lt;b &gt;System Account: &lt;/b &gt;nokia&lt;br /&gt;&lt;br /&gt;

        //    &lt;b &gt;File Name: &lt;/b &gt;photo.JPG&lt;br /&gt;
        //    &lt;b &gt;File Path: &lt;/b &gt;/Users/nokia/tmp/test_data_files/geotags/photo.JPG&lt;br /&gt;
        //    &lt;b &gt;File Size: &lt;/b &gt;356.38 KB (364928 bytes)&lt;br /&gt;&lt;br /&gt;

        //    &lt;b &gt;Latitude: &lt;/b &gt;+46.94700000000000&lt;br /&gt;
        //    &lt;b &gt;Longitude: &lt;/b &gt;+7.45916666666666&lt;br /&gt;
        //    &lt;b &gt;Altitude: &lt;/b &gt;+501.21674876847288260251 Mtrs.&lt;br /&gt;&lt;br /&gt;

        //    &lt;b &gt;GPS Datestamp: &lt;/b &gt;Sep-27-2010 00:00:00&lt;br /&gt;&lt;br /&gt;

        //    &lt;b &gt;Date Added: &lt;/b &gt;Jun-20-2018 12:00:42&lt;br /&gt;
        //    &lt;b &gt;Content Creation Date: &lt;/b &gt;Sep-27-2010 18:48:42&lt;br /&gt;
        //    &lt;b &gt;Content Modification Date: &lt;/b &gt;Sep-27-2010 18:48:42&lt;br /&gt;
        //    &lt;b &gt;FS Creation Date: &lt;/b &gt;Nov-26-2016 11:59:54&lt;br /&gt;
        //    &lt;b &gt;FS Content Change Date: &lt;/b &gt;Nov-26-2016 11:59:54&lt;br /&gt;&lt;br /&gt;

        //    &lt;b &gt;Examiner Notes: &lt;/b &gt;<font color = "blue">hello_note</font>&lt;br /&gt;

        ///===========================


        QString final_data;

        QString m_plgn_info = QString("&lt;b &gt;") + "Plugin" + ": &lt;/b &gt;";
        m_plgn_info += QString(MACRO_Plugin_Name_File_System) + "&lt;br /&gt;";
        m_plgn_info += QString("&lt;b &gt;") + "Category" + ": &lt;/b &gt;";
        m_plgn_info += QString(MACRO_Plugin_File_System_TAB_Name_Files) + "&lt;br /&gt;" + "&lt;br /&gt;";
        final_data.append(m_plgn_info);

        /// record no
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Record_No_Index)->text().trimmed() + QString(":")); // record_no title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Record_No_Index)->text().trimmed());// record_no value
        final_data.append("&lt;br /&gt;");
        final_data.append("&lt;br /&gt;");



        /// filename
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_File_Name_Index)->text().trimmed() + QString(":")); // filename title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_File_Name_Index)->text().trimmed());// filename value
        final_data.append("&lt;br /&gt;");


        /// filesize
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_File_Size_Index)->text().trimmed() + QString(":")); // filesize title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_File_Size_Index)->text().trimmed());// filesize value
        final_data.append("&lt;br /&gt;");


        /// Mime Type
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Mime_Type_Index)->text().trimmed() + QString(":")); // Mime Type title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Mime_Type_Index)->text().trimmed());// Mime Type value
        final_data.append("&lt;br /&gt;");
        final_data.append("&lt;br /&gt;");


        /// File Path
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_File_Path_Index)->text().trimmed() + QString(":")); // File Path title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_File_Path_Index)->text().trimmed());// File Path value
        final_data.append("&lt;br /&gt;");
        final_data.append("&lt;br /&gt;");


        /// Latitude
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Apple_Latitude_Index)->text().trimmed() + QString(":")); // Latitude title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Apple_Latitude_Index)->text().trimmed());// Latitude value
        final_data.append("&lt;br /&gt;");

        /// Longitude
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Apple_Longitude_Index)->text().trimmed() + QString(":")); // Longitude title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Apple_Longitude_Index)->text().trimmed());// Longitude value
        final_data.append("&lt;br /&gt;");
        final_data.append("&lt;br /&gt;");

        /// Date Modified
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Date_Modified_Index)->text().trimmed() + QString(":")); // Date Modified title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Date_Modified_Index)->text().trimmed());// Date Modified value
        final_data.append("&lt;br /&gt;");


        /// Date Accessed
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Date_Accessed_Index)->text().trimmed() + QString(":")); // Date Accessed title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Date_Accessed_Index)->text().trimmed());// Date Accessed value
        final_data.append("&lt;br /&gt;");
        final_data.append("&lt;br /&gt;");

        /// Date Created
        final_data.append("&lt;b &gt;");
        final_data.append(m_tablewidget_obj->horizontalHeaderItem(enum_Date_Created_Index)->text().trimmed() + QString(":")); // Date Created title
        final_data.append(" &lt;/b &gt;");
        final_data.append(m_tablewidget_obj->item(row,enum_Date_Created_Index)->text().trimmed());// Date Created value
        final_data.append("&lt;br /&gt;");
        final_data.append("&lt;br /&gt;");

        /// Notes
        final_data.append("&lt;b &gt;");
        final_data.append("Examiner Notes:"); // Notes title
        final_data.append("&lt;/b &gt;<font color = blue>");
        final_data.append(m_tablewidget_obj->item(row,enum_Notes_Value_Index)->text().trimmed().trimmed());// Notes value
        final_data.append("</font>&lt;br /&gt;");


        stream_kml.writeStartElement("Placemark");
        stream_kml.writeTextElement("name", QFileInfo(display_file_path).fileName());
        stream_kml.writeTextElement("description", final_data);
        stream_kml.writeStartElement("Point");
        stream_kml.writeTextElement("coordinates",full_cordinates);
        stream_kml.writeEndElement(); //point end
        stream_kml.writeEndElement(); //placemark end

        ///===========================

        stream_kml.writeEndElement();
        file.write(buffer.toLocal8Bit());
        file.write("</kml>");
        file.flush();
        file.close();

    }

    return list_paths_to_message_box;

}
void filetypes_loader::prepare_hashset_db_data_list()
{
    hashset_db_data_list.clear();

    QString hashset_conf_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString() + "hashset_index.sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(hashset_conf_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + hashset_conf_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query_select(destination_db);

    query_select.prepare("Select saved_db_name,saved_table_name,md5_hash_column, recon_filefrom,recon_file_infopath from hashset_indexes");

    if(!query_select.exec())
    {
        recon_static_functions::app_debug("query execute -----FAILED----" + query_select.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + query_select.lastError().text(), Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString result_conf_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString();
    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    while(query_select.next())
    {
        QString saved_db_name = query_select.value("saved_db_name").toString().trimmed();
        QString saved_table_name = query_select.value("saved_table_name").toString().trimmed();
        QString md5_hash_column = query_select.value("md5_hash_column").toString().trimmed();

        QString recon_filefrom = query_select.value("recon_filefrom").toString().trimmed();
        QString recon_file_infopath = query_select.value("recon_file_infopath").toString().trimmed();

        QString hashes_db_path = recon_file_infopath;
        if(recon_filefrom == recon_file_from_result)
        {
            if(hashes_db_path.startsWith(QString(MACRO_CASE_TREE_Hashset)))
                hashes_db_path = result_conf_dir_path + recon_file_infopath;
            else
                hashes_db_path = result_dir_path + recon_file_infopath;
        }

        struct_global_hashset_db_data hashset_obj;

        hashset_obj.hashset_db_path    = hashes_db_path;
        hashset_obj.hashet_table_name  = saved_table_name;
        hashset_obj.hashet_column_name = md5_hash_column;

        QString hash_name = saved_db_name;
        hash_name.remove(".sqlite");
        hash_name = hash_name.trimmed();

        hashset_obj.hash_name_str = hash_name;

        hashset_db_data_list.append(hashset_obj);

    }

    destination_db.close();
}

QStringList filetypes_loader::get_hashsets_list_to_be_hidden()
{
    QString hashset_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString() + "hashset_index.sqlite";

    QStringList saved_hashes_db_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select saved_db_name From hashset_indexes Where hide_files = ?"), QStringList(QString(MACRO_TRUE_VALUE_NUMERIC_QString)), 0, hashset_index_db_path, Q_FUNC_INFO);

    QStringList hashset_title_list;

    for(int count = 0; count < saved_hashes_db_list.size(); count++)
    {
        QString hash_db_name = saved_hashes_db_list.at(count);
        hash_db_name.remove(".sqlite");

        hashset_title_list << hash_db_name.trimmed();
    }

    hashset_title_list.removeAll("");
    hashset_title_list.removeDuplicates();

    return hashset_title_list;
}

bool filetypes_loader::check_fs_record_hidden_for_hashset(QStringList &hashes_hidden_list, QStringList matched_hashset_list)
{
    bool bool_hide_record = false;
    for(int count = 0; count < matched_hashset_list.size(); count++)
    {
        if(hashes_hidden_list.contains(matched_hashset_list.at(count)))
        {
            bool_hide_record = true;
            break;
        }
    }

    return bool_hide_record;
}

void filetypes_loader::action_run_fs_modules_apple_metadata_clicked()
{

    bool_run_fs_modules_apple_metadata_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_apple_metadata_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_apple_metadata_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Apple Metadata' already running."));
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_apple_metadata_run_status")); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_apple_metadata(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Apple Metadata' has already been extracted."));
        return;
    }
}

void filetypes_loader::action_run_fs_modules_exif_metadata_clicked()
{

    bool_run_fs_modules_exif_metadata_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_exif_metadata_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_exif_metadata_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Exif Metadata' already running."));
        return;
    }


    prepare_fs_run_module_files_list(QString("fs_module_exif_metadata_run_status")); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_exif_metadata(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Exif Metadata' has already been extracted."));
        return;
    }
}

void filetypes_loader::action_run_fs_modules_signature_analysis_clicked()
{

    bool_run_fs_modules_signature_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_signature_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_signature_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Signature Analysis' already running."));
        return;
    }


    prepare_fs_run_module_files_list(QString("fs_module_signature_analysis_run_status")); //db column name
    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_signature_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Signature' has already been extracted."));
        return;
    }
}

void filetypes_loader::action_run_fs_modules_mime_type_analysis_clicked()
{

    bool_run_fs_modules_mime_type_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_mime_type_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_mime_type_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Mime Type Analysis' already running."));
        return;
    }


    prepare_fs_run_module_files_list(QString("fs_module_mime_type_analysis_run_status")); //db column name
    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_mime_type_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Mime Type' has already been extracted."));
        return;
    }
}

void filetypes_loader::action_run_fs_modules_hashset_clicked()
{

    bool_run_fs_modules_hashset_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_hashset_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_hashset_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Hashes Analysis' already running."));
        return;
    }


    prepare_fs_run_module_files_list(QString("fs_module_hashset_run_status")); //db column name
    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_hashset(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("'Hashes' has already been extracted."));
        return;
    }
}

void filetypes_loader::action_run_fs_modules_face_analysis_clicked()
{

    bool_run_fs_modules_face_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_face_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_face_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Face Analysis' already running.");
        return;
    }


    prepare_fs_run_module_files_list(QString("fs_module_face_analysis_run_status")); //db column name
    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_face_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Face Analysis' has already been extracted.");
        return;
    }
}

void filetypes_loader::action_run_fs_modules_optical_character_recognition_clicked()
{

    bool_run_fs_modules_optical_character_recognition_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_optical_character_recognition_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_optical_character_recognition_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Optical Character Recognition' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_optical_character_recognition_run_status")); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_optical_character_recognition(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Optical Character Recognition' has already been extracted.");
        return;
    }
}

void filetypes_loader::action_run_fs_modules_weapons_clicked()
{

    bool_run_fs_modules_weapons_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_weapons_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_weapons_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Weapons' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_weapons_run_status")); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_weapons(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Weapons' has already been extracted.");
        return;
    }
}

void filetypes_loader::action_run_fs_modules_fire_analysis_clicked()
{

    bool_run_fs_modules_fire_analysis_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_fire_analysis_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_fire_analysis_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Fire Analysis' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_fire_analysis_run_status")); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_fire_analysis(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Fire Analysis' has already been extracted.");
        return;
    }
}

void filetypes_loader::action_run_fs_modules_skin_tone_detection_clicked()
{

    bool_run_fs_modules_skin_tone_detection_status = false;
    bool_run_fs_modules_status = false;

    emit signal_run_fs_module_skin_tone_detection_already_running_status();

    int count = 0;
    while(1)
    {
        count++;

        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(bool_run_fs_modules_status)
            break;
    }

    if(bool_run_fs_modules_skin_tone_detection_status)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Skin Tone Detection' already running.");
        return;
    }

    prepare_fs_run_module_files_list(QString("fs_module_skin_tone_detection_run_status")); //db column name

    if(run_fs_modules_details_list.size() > 0)
        emit signal_run_fs_module_skin_tone_detection(run_fs_modules_details_list);
    else
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "'Skin Tone Detection' has already been extracted.");
        return;
    }
}

void filetypes_loader::slot_bookmark_key_pressed_tablewidget()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
    {
        int current_row = m_table_carved_index_files->currentRow();
        if(current_row < 0)
            return;

        if(m_table_carved_index_files->item(current_row, enum_indexed_carved_files_Bookmarks_Index)->checkState())
        {
            action_carved_index_files_remove_bookmark_clicked();
        }
        else
        {
            action_carved_index_files_bookmark_clicked();
        }
        return;
    }



    int current_row = m_tablewidget_obj->currentRow();
    if(current_row < 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index != -1)
    {
        if(m_tablewidget_obj->item(current_row, bookmark_index)->checkState())
        {
            update_bookmark_value("0",current_row,bookmark_index);
        }
        else
        {
            update_bookmark_value("1",current_row,bookmark_index);
        }
    }


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void filetypes_loader::slot_sort_tablewidget_display(int column_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    return;

    m_tablewidget *current_table = NULL;

    if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW)
        current_table = m_tablewidget_obj;
    else if(main_tab_widget_obj->currentIndex() == enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES)
        current_table = m_table_carved_index_files;

    if(current_table == NULL)
        return;

    //Return if clicked column index other then Bookmark, Tags or Notes
    if(column_index != enum_tablewidget_column_bookmark && column_index != enum_tablewidget_column_notes && column_index != enum_tablewidget_column_tags)
    {
        current_table->sortByColumn(column_index,Qt::AscendingOrder);
        current_table->setSortingEnabled(false);
        return;
    }

    current_table->clearSelection();

    if(current_table->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = current_table->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < current_table->rowCount(); row++)
        {
            if(current_table->item(row, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_tablewidget_column_bookmark)
                icon_set =  current_table->item(row, column_index)->icon();



            current_table->item(row, column_index)->checkState();

            // Check for checkstate OR iCon
            if(current_table->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column_index != enum_tablewidget_column_bookmark)
                    icon_shiftrow_acs =  current_table->item(row_shift_pos_ascending, column_index)->icon();

                // If row is already on 0th position then increment and continue
                if(current_table->item(row_shift_pos_ascending, column_index)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = current_table->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_table->takeRow(destRow);

                // Shifting of rows from lower to upper
                current_table->setRow(destRow, sourceItems);
                current_table->setRow(sourceRow, destItems);


                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(current_table->item(row, column_index) == NULL)
                continue;

            if(current_table->item(row_shift_pos_descending, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_tablewidget_column_bookmark)
                icon_set =  current_table->item(row, column_index)->icon();

            if(current_table->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column_index != enum_tablewidget_column_bookmark)
                    icon_shiftrow_desc =  current_table->item(row_shift_pos_descending, column_index)->icon();

                // If row is already on last position then decrement and continue
                if(current_table->item(row_shift_pos_descending, column_index)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = current_table->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_table->takeRow(destRow);

                // Shifting of rows from upper to lower
                current_table->setRow(destRow, sourceItems);
                current_table->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void filetypes_loader::prepare_fs_run_module_files_list(QString status_column_name)
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait, Preparing Files List..."));
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    run_fs_modules_details_list.clear();
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString fs_record_no       = m_tablewidget_obj->item(selected_row_no, enum_Record_No_Index)->text().trimmed();
        QString source_count_name  = m_tablewidget_obj->item(selected_row_no, enum_Source_Count_Name_Index)->text().trimmed();
        QString db_path_filesystem = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + source_count_name + "/file_system.sqlite";

        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,db_path_filesystem,"files",source_count_name ,Q_FUNC_INFO);

        QString display_filepath   = m_tablewidget_obj->item(selected_row_no, enum_File_Path_Index)->text().trimmed();

        bool bool_is_dir = recon_helper_standard_obj->pub_check_path_is_directory_by_fs_dbpath(complete_file_path,fs_record_no, db_path_filesystem, Q_FUNC_INFO);
        if(bool_is_dir)
        {
            QStringList value_list;
            QString val_str = display_filepath;
            val_str.append("%");
            value_list << val_str << QString("0");

            ///Run only for files not processed
            QStringList record_no_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select INT From files Where file_parent_path like ? AND ") + status_column_name + QString(" = ?") , value_list, 0, db_path_filesystem, Q_FUNC_INFO);

            ///get all files/directories in selected directory
            for(int count = 0; count < record_no_list.size(); count++)
            {
                if(count % 50 == 0)
                    QCoreApplication::processEvents();

                fs_record_no   = record_no_list.at(count);

                QString early_run_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select ") + status_column_name + QString(" from files where INT = ?") , QStringList(fs_record_no), 0, db_path_filesystem, Q_FUNC_INFO);

                ///no need to run again already ran for this file
                if(early_run_status.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                    continue;

                complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(fs_record_no,db_path_filesystem,"files",source_count_name,Q_FUNC_INFO);

                ///get all files(only) in selected directory
                //                if(QFileInfo(complete_file_path).isDir())
                //                    continue;

                bool bool_is_dir_in_loop = recon_helper_standard_obj->pub_check_path_is_directory_by_fs_dbpath(complete_file_path,fs_record_no, db_path_filesystem, Q_FUNC_INFO);
                if(bool_is_dir_in_loop)
                    continue;

                display_filepath  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select filepath From files Where INT = ?"), QStringList(fs_record_no), 0, db_path_filesystem, Q_FUNC_INFO);

                QString filename_str  = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_file_path);

                struct_global_run_fs_modules_details struct_run_fs_modules_details_obj;

                struct_run_fs_modules_details_obj.fs_record_no       = fs_record_no;
                struct_run_fs_modules_details_obj.display_filepath   = display_filepath;
                struct_run_fs_modules_details_obj.complete_file_path = complete_file_path;
                struct_run_fs_modules_details_obj.source_count_name  = source_count_name;
                struct_run_fs_modules_details_obj.examiner_selected_file_or_dir_name = filename_str;

                run_fs_modules_details_list.append(struct_run_fs_modules_details_obj);

            }
        }
        else
        {
            QString early_run_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select ") + status_column_name + QString(" from files where INT = ?") , QStringList(fs_record_no), 0, db_path_filesystem, Q_FUNC_INFO);

            ///no need to run again already ran for this file
            if(early_run_status.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                continue;

            QString filename_str  = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_file_path);

            struct_global_run_fs_modules_details struct_run_fs_modules_details_obj;

            struct_run_fs_modules_details_obj.fs_record_no       = fs_record_no;
            struct_run_fs_modules_details_obj.display_filepath   = display_filepath;
            struct_run_fs_modules_details_obj.complete_file_path = complete_file_path;
            struct_run_fs_modules_details_obj.source_count_name  = source_count_name;
            struct_run_fs_modules_details_obj.examiner_selected_file_or_dir_name   = filename_str;

            run_fs_modules_details_list.append(struct_run_fs_modules_details_obj);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void filetypes_loader::slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status)
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

void filetypes_loader::change_row_background_color_for_last_used_date(qint64 current_row)
{
    if(current_row < 0)
        return;


    QColor back_color = QString("#FFFF66"); //light yellow

    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        if(m_tablewidget_obj->item(current_row , column) == nullptr)
            continue;

        m_tablewidget_obj->item(current_row , column)->setBackground(back_color);
    }

}

void filetypes_loader::slot_label_link_searched_face_linkactivated(QString label_text)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(label_text == "Show")
    {
        label_link_searched_face->setText("<a href = "+ QString("Hide") + " > " + QString("Searched Face") + " </a>");
        label_picture_searched_face->show();
    }
    else if(label_text == "Hide")
    {
        label_link_searched_face->setText("<a href = "+ QString("Show") + " > " + QString("Searched Face") + " </a>");
        label_picture_searched_face->hide();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void filetypes_loader::slot_process_video_thumbnails_finished(int)
{
    bool_process_thumbnails_finished = true;
    //process_thumbnails->close();
}

void filetypes_loader::clear_all_custom_header_widget_entries()
{
    for(int ii = 0; ii < custom_header_widgets_QList.size(); ii++)
    {
        custom_header_widgets_QList.at(ii)->pub_clear_line_edit_search_field();
    }

    pub_refresh_horizontal_header();
}
