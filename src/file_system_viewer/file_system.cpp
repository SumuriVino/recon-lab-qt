#include "file_system.h"
#include "ui_file_system.h"

file_system::file_system(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::file_system)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    recon_helper_standard_obj = new recon_helper_standard(this);

    installEventFilter(this);

    main_tab_widget_obj = new QTabWidget(this);
    connect(main_tab_widget_obj, SIGNAL(currentChanged(int)), this, SLOT(slot_main_tab_widget_current_changed(int)));

    gallery_viewer_obj = new gallery_viewer(this);
    gallery_viewer_obj->pub_create_ui();

    connect(gallery_viewer_obj, SIGNAL(signal_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_viewer_item_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_clicked(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_view_invalid_item_clicked()), this, SLOT(slot_gallery_view_invalid_tem_clicked()));
    connect(gallery_viewer_obj, SIGNAL(signal_gallery_viewer_item_double_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info)));

    connect(this, SIGNAL(signal_set_bookmark_status_to_gallery_view(bool)), gallery_viewer_obj, SLOT(slot_set_bookmark_status_to_gallery_view(bool)));

    m_file_system_tablewidget_obj = new m_tablewidget(this);
    //connect(m_file_system_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    m_file_system_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_file_system_tablewidget_obj->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_file_system_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_file_system_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);


    search_files_with_hash_obj = new search_files_with_hash();


    prc_generic = new QProcess(this);
    connect(prc_generic,SIGNAL(finished(int)),this,SLOT(slot_proc_generic_finished(int)));



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

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    pushButton_back = new QPushButton(this);
    pushButton_back->setFont(font);

    pushButton_next = new QPushButton(this);
    pushButton_next->setFont(font);

    pushbutton_show_files_recursively = new QPushButton(this);
    pushbutton_show_files_recursively->setFont(font);
    pushbutton_show_files_recursively->setToolTip("Recursive View");

    //    pushbutton_export_as_csv = new QPushButton("Export as CSV");
    pushbutton_export_as_csv = new QPushButton(MACRO_Generic_Export_As);
    pushbutton_export_as_csv->setFont(font);

    pushbutton_show_column = new QPushButton("Show Column");

    menu_export_as = new QMenu(this);

    QStringList export_menuList;
    export_menuList << QString(MACRO_Generic_Export_As_CSV)
                    << QString(MACRO_Generic_Export_As_SQLite);
    create_right_click_actions(menu_export_as,export_menuList);

    pushbutton_export_as_csv->setMenu(menu_export_as);

    connect(menu_export_as, SIGNAL(triggered(QAction*)), this, SLOT(slot_file_system_export_as_clicked(QAction*)));

    connect(pushButton_back,SIGNAL(clicked()),this,SLOT(slot_pushButton_back_clicked()), Qt::QueuedConnection);
    connect(pushButton_next,SIGNAL(clicked()),this,SLOT(slot_pushButton_next_clicked()), Qt::QueuedConnection);

    connect(pushbutton_show_files_recursively,SIGNAL(clicked(bool)),this,SLOT(slot_show_files_recursively_clicked(bool)));

    //connect(pushbutton_export_as_csv,SIGNAL(clicked(bool)),this,SLOT(slot_on_pushbutton_clicked_export_as_csv(bool)));

    ///---right click actions
    mainMenu = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_file_system_menu_right_clicked(QAction*)));

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark), mainMenu, submenu_bookmark);
    create_right_click_actions(submenu_bookmark,menuList);

    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    connect(this, SIGNAL(signal_add_tags(QString,QString)), this, SLOT(slot_action_add_tags_clicked(QString,QString)));

    menuList.clear();
    menuList << QString("")
             << QString(MACRO_Generic_Right_Click_Add_Note)
             << QString(MACRO_Generic_Right_Click_Remove_Note)
             << QString("");

    submenu_add_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes) , mainMenu , submenu_add_note);
    create_right_click_actions(submenu_add_note,menuList);

    menuList.clear();
    menuList << QString("");
    create_right_click_actions(mainMenu,menuList);

    act_quick_look = create_right_click_action_for_enable_disable(QString(MACRO_Generic_Right_Click_Quick_look),mainMenu , act_quick_look);
    connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));

    //Open With
    QStringList submenuList;
    submenuList << QString(MACRO_Generic_Right_Click_Open_With_External_Application) << ""
                << QString(MACRO_CASE_TREE_Plist_Viewer) << QString(MACRO_CASE_TREE_Hex_Viewer)
                << QString(MACRO_CASE_TREE_Sqlite_Viewer) << QString(MACRO_CASE_TREE_Registry_Viewer)
                << QString(MACRO_CASE_TREE_Log_Viewer);

    submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
    create_right_click_actions(submenu_open_with , submenuList);

    submenu_open_files_with_other_app = new QMenu(this);
    connect(submenu_open_files_with_other_app,SIGNAL(triggered(QAction*)),this,SLOT(slot_sub_menu_open_file_with_other_app_clicked(QAction*)));
    QList <QMenu*> open_with_other_app_menu_list;
    open_with_other_app_menu_list.append(submenu_open_files_with_other_app);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(open_with_other_app_menu_list, Q_FUNC_INFO);

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Open_In_Window);
    menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);
    create_right_click_actions(mainMenu,menuList);
    create_right_click_actions(mainMenu,QStringList(""));

    // Send to Bucket
    submenuList.clear();
    submenuList  << QString(MACRO_CASE_TREE_Plist_Viewer) << QString(MACRO_CASE_TREE_Hex_Viewer)
                 << QString(MACRO_CASE_TREE_Sqlite_Viewer) << QString(MACRO_CASE_TREE_Registry_Viewer)
                 << QString(MACRO_CASE_TREE_Log_Viewer);

    submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_send_to_bucket);
    create_right_click_actions(submenu_send_to_bucket , submenuList);

    connect(submenu_open_with, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_open_with_clicked(QAction*)));
    connect(submenu_send_to_bucket, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_sendToBucket_clicked(QAction*)));

    //text indexing
    act_add_file_to_text_indexing = create_right_click_action_for_enable_disable(QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue),mainMenu , act_add_file_to_text_indexing);
    connect(act_add_file_to_text_indexing, SIGNAL(triggered(bool)), this, SLOT(slot_add_file_to_text_indexing_clicked(bool)));
    create_right_click_actions(mainMenu,QStringList(""));

    //carve file
    menuList.clear();
    menuList << MACRO_Generic_Right_Click_Carve_Files << MACRO_Generic_Right_Click_Carve_Data;
    submenu_carve = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Carve) , mainMenu , submenu_carve);
    connect(submenu_carve, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_carve_clicked(QAction*)));
    create_right_click_actions(submenu_carve,menuList);

    // Run FS Modules
    submenu_run_fs_modules = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules) , mainMenu , submenu_run_fs_modules);

    connect(submenu_run_fs_modules, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_run_fs_modules_clicked(QAction*)));

    QAction *act_Run_Filesystem_Modules_Apple_Metadata = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_Exif_Metadata  = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Exif_Metadata).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_Signature_Analysis = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Signature_Analysis).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_Hashset = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Hashes).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_Face_Analysis = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_OCR = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_Weapons = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_fire_analysis = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis).toLocal8Bit(), this);
    QAction *act_Run_Filesystem_Modules_Skin_Tone_Detection = new QAction(QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection).toLocal8Bit(), this);

    act_Run_Filesystem_Modules_Apple_Metadata->setIcon(QIcon("../icons/Features/apple_metadata.png"));
    act_Run_Filesystem_Modules_Exif_Metadata->setIcon(QIcon("../icons/Features/exif_data.png"));
    act_Run_Filesystem_Modules_Signature_Analysis->setIcon(QIcon("../icons/Features/signature_analysis.png"));
    act_Run_Filesystem_Modules_Hashset->setIcon(QIcon("../icons/Features/hashset.png"));
    act_Run_Filesystem_Modules_Face_Analysis->setIcon(QIcon("../icons/Features/face_analysis.png"));
    act_Run_Filesystem_Modules_OCR->setIcon(QIcon("../icons/Features/optical_character_recognition.png"));
    act_Run_Filesystem_Modules_Weapons->setIcon(QIcon("../icons/Features/weapons.png"));
    act_Run_Filesystem_Modules_fire_analysis->setIcon(QIcon("../icons/Features/fire_analysis.png"));
    act_Run_Filesystem_Modules_Skin_Tone_Detection->setIcon(QIcon("../icons/Features/skin_tone_detection.png"));

    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Apple_Metadata);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Exif_Metadata);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Signature_Analysis);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Hashset);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Face_Analysis);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_OCR);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Weapons);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_fire_analysis);
    submenu_run_fs_modules->addAction(act_Run_Filesystem_Modules_Skin_Tone_Detection);

    act_face_search = create_right_click_action_for_enable_disable(QString(MACRO_Generic_Right_Click_Face_Search),mainMenu , act_face_search);
    connect(act_face_search, SIGNAL(triggered(bool)), this, SLOT(slot_act_face_search_triggered(bool)));

    create_right_click_actions(mainMenu,QStringList(""));


    submenu_add_files_to_hashset_db = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database) , mainMenu , submenu_add_files_to_hashset_db);

    connect(submenu_add_files_to_hashset_db,SIGNAL(triggered(QAction*)),this,SLOT(slot_sub_menu_add_files_to_hashset_clicked(QAction*)));
    QList <QMenu*> hahses_menu_list;
    hahses_menu_list.append(submenu_add_files_to_hashset_db);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(hahses_menu_list, Q_FUNC_INFO);
    action_search_file_with_same_md5_hash = create_right_click_action_for_enable_disable(QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash),mainMenu , action_search_file_with_same_md5_hash);
    connect(action_search_file_with_same_md5_hash, SIGNAL(triggered()), this, SLOT(slot_action_search_file_with_same_hash_clicked()));

    hashset_display_obj = new hashset_display(this);
    hashset_storage_obj = new hashset_storage(this);
    connect(hashset_storage_obj, SIGNAL(signal_hashset_storage_workdone_clicked(QStringList, QString)), this, SLOT(slot_hashset_storage_workdone_clicked(QStringList, QString)));

    //Export as hashes
    export_hashes_as_vic_obj = new export_hashes_as_vic(this);
    connect(export_hashes_as_vic_obj, SIGNAL(signal_export_button_clicked()), this,  SLOT(slot_export_hashes_as_vic_clicked()));

    export_hashes_as_sqlite_obj = new export_hashes_as_sqlite(this);
    connect(export_hashes_as_sqlite_obj,SIGNAL(signal_export_button_clicked()) , this , SLOT(slot_export_hashes_as_sqlite_clicked()));

    export_hashes_as_csv_obj = new export_hashes_as_csv(this);
    connect(export_hashes_as_csv_obj, SIGNAL(signal_export_button_clicked()), this,  SLOT(slot_export_hashes_as_csv_clicked()));

    submenu_export_hashes_as = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As) , mainMenu , submenu_export_hashes_as);
    connect(submenu_export_hashes_as, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_export_hashes_as_vic_clicked(QAction*)));

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

    create_right_click_actions(mainMenu,QStringList(""));

    // Export
    act_export = create_right_click_action_for_enable_disable(QString(MACRO_Generic_Right_Click_Export),mainMenu , act_export);

    create_right_click_actions(mainMenu,QStringList(""));

    //seen
    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Mark_As_Seen) << QString(MACRO_Generic_Right_Click_Mark_As_Unseen);
    menuList << QString(MACRO_Generic_Right_Click_Hide_Seen_Files) << QString(MACRO_Generic_Right_Click_Show_Seen_Files);
    submenu_seen = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Seen) , mainMenu , submenu_seen);
    create_right_click_actions(submenu_seen,menuList);

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Decompress_File);
    create_right_click_actions(mainMenu,menuList);

    act_app_related_file = create_right_click_action_for_enable_disable(QString(MACRO_Generic_Right_Click_List_Related_Files),mainMenu , act_app_related_file);
    connect(act_app_related_file, SIGNAL(triggered(bool)), this, SLOT(slot_act_app_related_files_triggered(bool)));
    app_related_files_obj = new app_related_files;

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    add_notes_to_singal_record_obj = new add_notes(this);
    connect(add_notes_to_singal_record_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_pushbutton_add_notes_clicked(QString)));

    add_notes_to_bookmarks_obj = new add_notes(this);
    connect(add_notes_to_bookmarks_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_pushbutton_add_notes_to_bookmark_clicked(QString)));

    // Tags
    add_tags_module_obj = new add_tags(this);
    remove_multiple_tag_selecotor_obj = new remove_multiple_tag_selecotor(this);


    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_in_actions_menu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_action_add_tags_clicked(QString,QString)));

    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_remaining_tags_list(QStringList, QString , QString)),this,SLOT(slot_tag_list_after_remove_multiple_tag(QStringList, QString , QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_tag_list_to_be_removed(QStringList, QString , QString)),this,SLOT(slot_removed_tags_list_file_system(QStringList, QString , QString)));

    ///******************List of all stringlist that associate to filters*************************************************************************

    list_search_types << MACRO_Generic_Filter_Condition_Name << MACRO_Generic_Filter_Condition_Contents
                      << MACRO_Generic_Filter_Condition_Kind << MACRO_Generic_Filter_Condition_Last_Opened_Date
                      << MACRO_Generic_Filter_Condition_Last_Modified_Date << MACRO_Generic_Filter_Condition_Others;

    list_search_type_names_search_items << MACRO_Generic_Filter_Condition_Matches << MACRO_Generic_Filter_Condition_Contains
                                        << MACRO_Generic_Filter_Condition_Begins_With << MACRO_Generic_Filter_Condition_Ends_With;
    // << MACRO_Generic_Filter_Condition_Is << MACRO_Generic_Filter_Condition_Is_Not;

    list_search_type_kind_search_items  << MACRO_Generic_Filter_Condition_Any << MACRO_Generic_Filter_Condition_Application
                                        << MACRO_Generic_Filter_Condition_Archive << MACRO_Generic_Filter_Condition_Document
                                        << MACRO_Generic_Filter_Condition_Executable << MACRO_Generic_Filter_Condition_Folder
                                        << MACRO_Generic_Filter_Condition_Image << MACRO_Generic_Filter_Condition_Movie
                                        << MACRO_Generic_Filter_Condition_Music << MACRO_Generic_Filter_Condition_Pdf
                                        << MACRO_Generic_Filter_Condition_Presentation << MACRO_Generic_Filter_Condition_Text
                                        << MACRO_Generic_Filter_Condition_Other;

    list_search_type_kind_images_search_items << MACRO_Generic_Filter_Condition_All << MACRO_Generic_Filter_Condition_Jpeg
                                              << MACRO_Generic_Filter_Condition_Tiff << MACRO_Generic_Filter_Condition_Gif
                                              << MACRO_Generic_Filter_Condition_Png << MACRO_Generic_Filter_Condition_Bmp;

    list_search_type_kind_music_search_items << MACRO_Generic_Filter_Condition_All << MACRO_Generic_Filter_Condition_Mp3
                                             << MACRO_Generic_Filter_Condition_Aac << MACRO_Generic_Filter_Condition_Purchased;

    list_search_type_kind_textfiles_search_items << MACRO_Generic_Filter_Condition_All << MACRO_Generic_Filter_Condition_Plain_Text
                                                 << MACRO_Generic_Filter_Condition_Rich_Text << MACRO_Generic_Filter_Condition_Html
                                                 << MACRO_Generic_Filter_Condition_Xml << MACRO_Generic_Filter_Condition_Source_Code;

    list_search_type_date_search_items  << MACRO_Generic_Filter_Condition_Within_Last << MACRO_Generic_Filter_Condition_Exactly
                                        << MACRO_Generic_Filter_Condition_Before << MACRO_Generic_Filter_Condition_After
                                        << MACRO_Generic_Filter_Condition_Today << MACRO_Generic_Filter_Condition_Yesterday
                                        << MACRO_Generic_Filter_Condition_This_Week << MACRO_Generic_Filter_Condition_This_Month
                                        << MACRO_Generic_Filter_Condition_This_Year;

    list_search_type_date_days_search_items  << MACRO_Generic_Filter_Condition_Days << MACRO_Generic_Filter_Condition_Weeks
                                             << MACRO_Generic_Filter_Condition_Months << MACRO_Generic_Filter_Condition_Years;

    list_search_type_options_search_items << MACRO_Generic_Filter_Condition_Any << MACRO_Generic_Filter_Condition_Yes
                                          << MACRO_Generic_Filter_Condition_No;

    list_search_type_size_filters_items << MACRO_Generic_Filter_Condition_KB << MACRO_Generic_Filter_Condition_MB
                                        << MACRO_Generic_Filter_Condition_GB;

    list_search_type_compare_search_items << MACRO_Generic_Filter_Condition_Equal << MACRO_Generic_Filter_Condition_Is_Greater_Than
                                          << MACRO_Generic_Filter_Condition_Is_Less_Than << MACRO_Generic_Filter_Condition_Is_Not;

    list_search_type_system_search_items << MACRO_Generic_Filter_Condition_Are_Not_Included << MACRO_Generic_Filter_Condition_Are_Included;

    list_search_type_visible_search_items << MACRO_Generic_Filter_Condition_Visible_Items << MACRO_Generic_Filter_Condition_Invisible_Items
                                          << MACRO_Generic_Filter_Condition_Visible_Or_Invisible;

    //model_filters = new QStandardItemModel(0, 5);
    vlayout_main = new QVBoxLayout(this);
    hlayout_back_next_button = new QHBoxLayout;
    last_save_index = 0;


    bool_proc_generic_finished = false;
    bool_sort_ascending = true;

    create_gui();


    keywords_list_selector_obj = new keywords_list_selector(this);
    connect(keywords_list_selector_obj, SIGNAL(signal_pushbotton_done_clicked()),this,SLOT(slot_dt_search_keywords_list_selection_done_clicked()));

    font_size_of_spotlight_search_int = 11;
    stylesheet_pushbutton_str = "QPushButton { margin: 1px; border-color: #D0D0D0; border-style: solid; border-radius: 1px; border-width: 1px; color: black;"
                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF); }"
                                "QPushButton:pressed {  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF);  border-color: #000000;}"
                                "QPushButton:checked {  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF);  border-color: #484848;}";


    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    export_dialog_box_3_obj = new export_dialog_box_3(this);
    connect(export_dialog_box_3_obj,SIGNAL(signal_start_write_data_as_csv_sqlite(QString,QString,bool,bool,QString,QString)),this,SLOT(slot_start_write_fs_data_in_csv(QString , QString ,bool, bool ,QString,QString)));

    file_export_dialog_obj = new file_export_dialog(this);
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done(bool,QString)), this, SLOT(slot_export_file_dialog_work_done(bool,QString)));
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done_for_zip(QString,bool,QString)), this, SLOT(slot_export_file_dialog_work_done_for_zip(QString,bool,QString)));

    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done_for_dmg(bool,QString)), this, SLOT(slot_export_file_dialog_work_done_for_dmg(bool,QString)));
    recon_helper_process_obj = new recon_helper_process(this);

    thread_zip_file_export_obj = new thread_zip_file_export;
    connect(thread_zip_file_export_obj, SIGNAL(finished()), this, SLOT(slot_thread_zip_file_export_finished()));



    decompress_file_system_obj = new decompress_file_system(this);
    decompress_file_system_obj->hide();

    export_rsync_obj = new export_rsync(this);
    bookmarked_records_from_table_display_list.clear();
    recursive_view_status = false;

}

file_system::~file_system()
{
    if(search_files_with_hash_obj != nullptr)
    {
        delete search_files_with_hash_obj;
    }

    if(app_related_files_obj != nullptr)
        delete app_related_files_obj;

    delete ui;
}

void file_system::slot_act_quick_look_triggered()
{
    emit signal_act_quick_look_triggered();

}

void file_system::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;

    if(yes_no_clicked)
    {
        if(bool_folder_export_done)
        {
            bool_folder_export_done = false;
            point_to_export_dir(folder_export_dir_path);
        }
        else if(bool_zip_export_done)
        {
            bool_zip_export_done =false;
            point_to_export_dir(zip_export_file_path);
        }
        else if(bool_dmg_export_done)
        {
            bool_dmg_export_done = false;

            point_to_export_dir(dmg_export_file_path);
        }
    }

}

void file_system::pub_set_necessary()
{
    hashset_display_obj->pub_set_essentials();
}

void file_system::add_applications_to_open_application_menu()
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

void file_system::open_export_as_FS_csv_file(QString file_path)
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

void file_system::set_source_count_name(QString name)
{
    source_count_name = name;
}

void file_system::pub_set_fs_db_path(QString db_path)
{
    db_path_filesystem = db_path;

    db_path_signature = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/signature.sqlite";
    db_path_mime = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/mime.sqlite";
    db_path_exif_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/exif_metadata.sqlite";
    db_path_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/apple_metadata.sqlite";

}

void file_system::pub_set_face_finder_obj(facefinder *obj)
{
    facefinder_obj = obj;

    facefinder_obj->pub_set_display_loading_progress_bar_obj(display_loading_progress_bar_obj);

    facefinder_obj->pub_set_essentials();
}

void file_system::pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str)
{

    if(bookmarked_records_from_table_display_list.contains(record_no_str))
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(true);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->checkState() == Qt::Checked)
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        if(record_no_str != record)
            continue;


        selected_record_num_list << record ;

        m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->setCheckState(Qt::Checked);



        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "1";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");


        obj.recon_tag_value = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_TAG_VALUE)->text().trimmed();

        obj.notes = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_NOTES_VALUE)->text().trimmed();
        obj.os_scheme_display = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_OS_SCHEME)->text().trimmed();


        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();


        list_st_bookmark_item_list.append(obj);

        m_file_system_tablewidget_obj->selectRow(selected_row_no);
    }


    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    global_connection_manager_obj->pub_set_bookmark_in_fs_source_db(selected_record_num_list, source_count_name);
    global_connection_manager_obj->pub_add_bookmark_for_file_system(list_st_bookmark_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void file_system::pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str)
{

    if(bookmarked_records_from_table_display_list.contains(record_no_str))
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    // Gallery view right click - bookmark
    if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW)
        emit signal_set_bookmark_status_to_gallery_view(false);

    QList<struct_global_tagging_tag_notes_db_updation> list_st_bookmark_item_list;

    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();


    m_file_system_tablewidget_obj->setCurrentCell(-1, 0);
    QStringList selected_record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->checkState() == Qt::Unchecked)
            continue;

        QString record = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_INT)->text().trimmed();
        if(record_no_str != record)
            continue;


        selected_record_num_list << record ;

        m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_BOOKMARK)->setCheckState(Qt::Unchecked);



        struct_global_tagging_tag_notes_db_updation obj;
        obj.bookmark = "0";
        obj.category = QString(MACRO_Plugin_Name_File_System);
        obj.record_no = record;
        obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.csv_plugin_name = QString(MACRO_Plugin_Name_File_System);
        obj.csv_tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
        obj.table_name = QString("files");

        obj.source_count_name = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_SOURCE_COUNT_NAME)->text().trimmed();
        obj.item0 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILENAME)->text().trimmed();
        obj.item1 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILEPATH)->text().trimmed();
        obj.item2 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_FILESIZE)->text().trimmed();
        obj.item3 = m_file_system_tablewidget_obj->item(selected_row_no, enum_File_System_display_MIMETYPE)->text().trimmed();


        list_st_bookmark_item_list.append(obj);

        m_file_system_tablewidget_obj->selectRow(selected_row_no);

    }


    if(list_st_bookmark_item_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    global_connection_manager_obj->pub_remove_bookmark_in_fs_source_db(selected_record_num_list, source_count_name);
    global_connection_manager_obj->pub_delete_bookmark_for_file_system(list_st_bookmark_item_list);

    display_loading_progress_bar_non_cancelable_obj->hide();


}

void file_system::pub_set_global_connection_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
    search_files_with_hash_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
}

void file_system::pub_set_global_recon_file_info_obj(global_recon_file_info *obj)
{
    global_recon_file_info_obj = obj;
}

void file_system::pub_set_recon_case_obj(RECON_case  *obj)
{
    recon_case_obj = obj;
}


void file_system::pub_set_case_treewidget_obj(R_Treewidget *obj)
{
    treewidget_case_display_obj = obj;
}

QString file_system::get_command_for_export_as_csv(bool is_recursive)
{
    QString parent_category = deepest_path;
    parent_category = specified_file_path;
    if(parent_category.startsWith(fs_extraction_path))
        parent_category.remove(0,fs_extraction_path.size());

    if(!parent_category.startsWith("/"))
        parent_category.prepend("/");

    QStringList custom_columns_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
    QStringList bookmarked_ext_attr_columns_list;
    for(int i = 0 ; i < custom_columns_list.size() ; i++)
    {
        QString column_name = custom_columns_list.at(i);
        column_name.prepend("b.");
        bookmarked_ext_attr_columns_list << column_name;
    }

    QString partial_command = bookmarked_ext_attr_columns_list.join(",");
    partial_command = partial_command.trimmed();
    if(partial_command.isEmpty())
        partial_command = ",";
    else
        partial_command.prepend(",").append(",");


    if(partial_command.endsWith(","))
        partial_command.chop(1);


    //    QString  command = "Select "  + QString("a.INT,a.filename, a.filepath,a.file_size,a.file_size,a.mime_type,a.hashset_name,a.hash_md5,a.hash_sha1,a.date_time_last_modified,a.date_time_created"
    //                                            ",a.date_time_last_read , a.kMDItemDateAdded_Date_Added ,a.kMDItemContentCreationDate_Created_Date"
    //                                            ",a.kMDItemContentModificationDate_Last_Modified_Date ,a.kMDItemLastUsedDate_Last_Opened_Date"
    //                                            ", a.kMDItemUseCount_Used_Count") + partial_command +
    //            QString(" from main.files as a LEFT OUTER join apple_metadataDB.files as b ON a.INT = b.filesystem_record where file_parent_path ");


    QString  command = "Select "  + QString("a.INT, a.inode_no, a.filename, a.extension, a.filepath, a.file_size, a.file_size,a.mime_type,a.hashset_name,a.hash_md5,a.hash_sha1,a.decompression_status,a.z_date_time_modified,a.z_date_time_change"
                                            ",a.z_date_time_accessed, a.z_date_time_created , a.kMDItemDateAdded_Date_Added ,a.kMDItemContentCreationDate_Created_Date"
                                            ",a.kMDItemContentModificationDate_Last_Modified_Date ,a.kMDItemLastUsedDate_Last_Opened_Date"
                                            ", a.kMDItemUseCount_Used_Count") + partial_command +
            QString(" from main.files as a LEFT OUTER join apple_metadataDB.files as b ON a.INT = b.filesystem_record where file_parent_path ");

    if(is_recursive || !pushbutton_show_files_recursively->isEnabled())
    {
        command.append("Like '" + parent_category + "%'");
    }
    else
    {
        command.append("= ?");
    }

    return command;
}

void file_system::set_essentials()
{
    struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_DATE_MODIFIED);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_DATE_CHANGE);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_DATE_ACCESSED);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_DATE_CREATED);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_Date_Added);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_Content_Creation_Date);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_Content_Modification_Date);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_Last_Used_Date);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_Use_Count);


    if(struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal
            || struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)

    {

        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_DATE_MODIFIED);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_DATE_CHANGE);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_DATE_ACCESSED);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_DATE_CREATED);
    }

    if(struct_target_source_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_HFS
            || struct_target_source_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_APFS)
    {
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Date_Added);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Content_Creation_Date);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Content_Modification_Date);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Last_Used_Date);
        m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Use_Count);
    }
    else
    {
        if(struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
        {
            m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Date_Added);
            m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Content_Creation_Date);
            m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Content_Modification_Date);
            m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Last_Used_Date);
            m_file_system_tablewidget_obj->showColumn(enum_File_System_display_Use_Count);
        }
    }


    // m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_DECOMPRESSION_STATUS);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_PERMISSION_RESTRICTED);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_SOURCE_COUNT_NAME);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_NOTES_VALUE);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_TAG_VALUE);
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_OS_SCHEME);


    for(int count = 0; count < m_file_system_tablewidget_obj->columnCount() - 2; count++)
    {
        if(m_file_system_tablewidget_obj->isColumnHidden(count))
            continue;

        if(headerlist.size() <= count)
            continue;

        QString column_label = headerlist.at(count);

        if(column_label.isEmpty())
            continue;

        QAction *action_filters   =  new QAction(column_label , this);
        action_filters->setCheckable(true);

        if(headerlist.at(count).trimmed() == QString("File Name"))
            action_filters->setChecked(true);

        menu_search_filters->addAction(action_filters);
    }

    pushbutton_search_filters_for_non_hfs->setMenu(menu_search_filters);

    virtual_source_path = struct_target_source_info.virtual_source_path;
    fs_extraction_path = struct_target_source_info.fs_extraction_source_path;

    os_naming_scheme_internal = struct_target_source_info.os_scheme_internal;
    os_scheme_display = struct_target_source_info.os_scheme_display;
    source_path = struct_target_source_info.fs_extraction_source_path;
    deepest_path = struct_target_source_info.fs_extraction_source_path;

    case_confg_dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    table_name = QString("files");
    pluginName = QString(MACRO_Plugin_Name_File_System);

    bookmark_index = 0;
    note_icon_index = 1;

    current_parent_dir_path = source_path;

    main_initial_db_cmd = QString("select a.bookmark, a.INT, a.filename, a.extension , a.filepath, a.mime_type, a.z_date_time_modified, a.z_date_time_change, a.z_date_time_accessed, a.z_date_time_created, a.file_size,"
                                  "a.notes, a.recon_tag_value, a.inode_no, a.is_seen,a.permission_restricted_file,a.kMDItemUsedDates_Used_Dates,a.kMDItemUseCount_Used_Count,"
                                  "a.kMDItemLastUsedDate_Last_Opened_Date,a.kMDItemContentCreationDate_Created_Date,a.kMDItemContentModificationDate_Last_Modified_Date,a.kMDItemDateAdded_Date_Added,a.hash_md5,"
                                  "a.hash_sha1,a.decompression_status,a.ocr_text from main.files as a  where (a.file_parent_path ");

    main_db_cmd =  main_initial_db_cmd + QString(" =  ?)");

    display_tags_submenu();

    menu_manage_column = new QMenu(this);

    pushbutton_show_column->setMenu(menu_manage_column);

    QFont m_font1;
    m_font1.setPointSize(MACRO_QMenu_Font_Size_int);
    menu_manage_column->setFont(m_font1);
    connect(menu_manage_column, SIGNAL(triggered(QAction*)), this, SLOT(slot_action_manage_column_clicked(QAction*)));

    int column_count;
    column_count = m_file_system_tablewidget_obj->columnCount();


    QList<struct_manage_column_info> column_no_and_name_list;
    column_no_and_name_list = get_hidden_column_from_manage_column_db();


    QStringList hidden_column_name_list;
    for(int j = 0; j < column_no_and_name_list.size(); j++)
    {
        struct_manage_column_info obj = column_no_and_name_list.at(j);
        hidden_column_name_list << obj.hid_col_name;

    }

    for(int count = 0 ; count < column_count ; count++)
    {
        if((count == 0) || (count == 1) || (count == 2) || (count == 3) || (count == 4) || (count == (column_count - 1)))
            continue;


        if(headerlist.size() <= count)
            continue;

        QString column_text = headerlist.at(count);

        if(m_file_system_tablewidget_obj->isColumnHidden(count) && !hidden_column_name_list.contains(column_text))
            continue;

        QAction *act  = new QAction(column_text,this->menu_manage_column);
        act->setCheckable(true);

        if(hidden_column_name_list.contains(column_text))
            act->setChecked(false);
        else
            act->setChecked(true);

        menu_manage_column->addAction(act);
    }

    m_file_system_tablewidget_obj->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_file_system_tablewidget_obj->horizontalHeader(),SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slot_table_header_customContextMenuRequested(const QPoint&)));

}

QString file_system::pub_get_last_display_path_for_check()
{
    return current_path_for_mainwindow_check;
}


void file_system::make_display_for_specified_path(QString file_path)
{
    current_path_for_mainwindow_check = file_path;
    if(current_path_for_mainwindow_check.startsWith(fs_extraction_path))
    {
        current_path_for_mainwindow_check.remove(0,fs_extraction_path.size());

        if(!current_path_for_mainwindow_check.trimmed().isEmpty() && !current_path_for_mainwindow_check.startsWith("/"))
            current_path_for_mainwindow_check.prepend("/");
    }


    QString t_file_path = file_path;
    QString t_fs_extracraction_path = fs_extraction_path;

    bool bool_is_root = false; // For special check in case if the file reside at the root of FS

    if((t_file_path + "/").trimmed() == t_fs_extracraction_path.trimmed())
        bool_is_root = true;


    if(!file_path.startsWith(fs_extraction_path) && !bool_is_root && !struct_target_source_info.bool_source_nested)
        file_path = global_recon_helper_singular_class_obj->get_complete_file_path(fs_extraction_path,file_path, Q_FUNC_INFO);

    QFont font;
    font.setPointSize(12);

    pushbutton_show_files_recursively->setIcon(QIcon("../icons/signs/recursive_on.png"));
    pushbutton_show_files_recursively->setFont(font);
    pushbutton_show_files_recursively->setToolTip("Recursive View");
    recursive_view_status = false;

    if(!file_path.isEmpty())
    {
        specified_file_path = file_path;

        if(!deepest_path.contains(file_path))
        {
            deepest_path = file_path.trimmed();
        }

        if(bool_is_root)
        {
            current_parent_dir_path = "/";
            file_path = file_path + "/";
        }
        else
        {
            current_parent_dir_path = file_path;

            if(!current_parent_dir_path.startsWith(fs_extraction_path))
                current_parent_dir_path = recon_static_functions::get_complete_file_path(fs_extraction_path,current_parent_dir_path, Q_FUNC_INFO);
        }

        set_fs_display(main_db_cmd , file_path , true , false);
    }
    else
    {
        specified_file_path = file_path;

        current_parent_dir_path = source_path;
        set_fs_display(main_db_cmd , source_path , true, false);
    }

}

void file_system::create_gui()
{
    QFont font;
    font.setPointSize(12);

    pushButton_back->setIcon(QIcon("../icons/signs/previous.png"));
    pushButton_next->setIcon(QIcon("../icons/signs/next.png"));

    pushbutton_show_files_recursively->setIcon(QIcon("../icons/signs/recursive_on.png"));
    pushbutton_show_files_recursively->setFont(font);
    pushbutton_show_files_recursively->setToolTip("Recursive View");

    pushButton_back->setFixedWidth(60);
    pushButton_back->setStyleSheet(stylesheet_pushbutton_str);
    pushButton_back->setFont(font);

    pushButton_next->setFixedWidth(60);
    pushButton_next->setStyleSheet(stylesheet_pushbutton_str);
    pushButton_next->setFont(font);

    pushbutton_search_filters_for_non_hfs = new QPushButton(this);
    pushbutton_search_filters_for_non_hfs->setText("Filters");
    //  pushbutton_search_filters_for_non_hfs->setFixedSize(85,27);
    // pushbutton_search_filters_for_non_hfs->setStyleSheet(stylesheet_pushbutton_str);
    pushbutton_search_filters_for_non_hfs->setFont(font);

    menu_search_filters       = new QMenu(this);
    menu_search_filters->setFont(font);

    lineEdit_search = new QLineEdit(this);
    lineEdit_search->setMinimumWidth(200);
    lineEdit_search->setMaximumWidth(220);
    //  lineEdit_search->setMinimumHeight(27);
    //    lineEdit_search->setContentsMargins(0,6,0,0);
    lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineEdit_search->setFont(font);

    QString stylesheet_lineedit = "QLineEdit { margin: 1px; border-color: #D0D0D0;  border-style: solid; border-radius: 1px; border-width: 1px; color: black;"
                                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF); }";

    // lineEdit_search->setStyleSheet(stylesheet_lineedit);

    pushButton_search = new QPushButton(this);
    //  pushButton_search->setFixedSize(85,27);
    // pushButton_search->setStyleSheet(stylesheet_pushbutton_str);
    pushButton_search->setFont(font);
    pushButton_search->setText("Search");

    pushbutton_show_all = new QPushButton(this);
    //   pushbutton_show_all->setFixedSize(85,27);
    //  pushbutton_show_all->setStyleSheet(stylesheet_pushbutton_str);
    pushbutton_show_all->setFont(font);
    pushbutton_show_all->setText("Show All");


    frame_dir_navigation = new QScrollArea(this);
    frame_dir_navigation->setMaximumHeight(20);

    hlayout_frame_dir_navigation = new QHBoxLayout;
    hlayout_frame_dir_navigation->setContentsMargins(0,0,0,0);
    hlayout_frame_dir_navigation->setSpacing(0);
    hlayout_frame_dir_navigation->setAlignment(Qt::AlignLeft);

    frame_dir_navigation->setLayout(hlayout_frame_dir_navigation);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    hlayout_back_next_button->addWidget(pushButton_back);
    hlayout_back_next_button->addWidget(pushButton_next);
    hlayout_back_next_button->addWidget(pushbutton_show_column);
    hlayout_back_next_button->addWidget(lineEdit_search);
    hlayout_back_next_button->addWidget(pushButton_search);
    hlayout_back_next_button->addWidget(pushbutton_search_filters_for_non_hfs);
    hlayout_back_next_button->addWidget(pushbutton_show_all);
    hlayout_back_next_button->addSpacerItem(spaceitem);
    hlayout_back_next_button->addWidget(pushbutton_export_as_csv);
    hlayout_back_next_button->addWidget(pushbutton_show_files_recursively);
    hlayout_back_next_button->setContentsMargins(1,4,4,0);
    hlayout_back_next_button->setSpacing(6);

    vlayout_main->addLayout(hlayout_back_next_button);
    vlayout_main->addWidget(main_tab_widget_obj);
    main_tab_widget_obj->addTab(m_file_system_tablewidget_obj, "Table View");
    main_tab_widget_obj->addTab(gallery_viewer_obj, "Gallery View");
    //vlayout_main->addWidget(m_file_system_tablewidget_obj);
    vlayout_main->addWidget(frame_dir_navigation);
    vlayout_main->setSpacing(0);
    vlayout_main->setContentsMargins(1,0,1,0);

    setLayout(vlayout_main);

    m_file_system_tablewidget_obj->installEventFilter(this);
    // m_file_system_tablewidget_obj->verticalHeader()->setFixedWidth(50);
    m_file_system_tablewidget_obj->verticalHeader()->setHighlightSections(false);
    m_file_system_tablewidget_obj->horizontalHeader()->setHighlightSections(false);
    m_file_system_tablewidget_obj->horizontalHeader()->stretchLastSection();

    pushButton_back->setEnabled(false);
    pushButton_next->setEnabled(false);

    connect(m_file_system_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_fs_tablewidget_single_click(int,int)));
    connect(m_file_system_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_fs_tablewidget_double_click(int,int)));
    connect(m_file_system_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed()));
    connect(m_file_system_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_file_system_tablewidget_obj,SIGNAL(signal_control_e_key_pressed()),this,SLOT(slot_ctrl_e_key_press_on_tablewidget()));
    connect(m_file_system_tablewidget_obj,SIGNAL(signal_control_a_key_pressed()),this,SLOT(slot_ctrl_a_key_press_on_tablewidget()));
    connect(m_file_system_tablewidget_obj,SIGNAL(signal_spacebar_quicklook_pressed()),this,SLOT(slot_spacebar_quicklook_key_pressed_on_tablewidget()));

    connect((QObject*)m_file_system_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sorting_tablewidget_display(int)));

    //    connect(pushButton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search_clicked()));
    //    connect(lineEdit_search,SIGNAL(returnPressed()),this,SLOT(slot_pushbutton_search_clicked()));
    //    connect(pushbutton_show_all,SIGNAL(clicked()),this,SLOT(slot_pushbutton_showall_clicked()));

    headerlist.clear();
    //    headerlist << "" << "" << "" << "Seen" << "Record No." << "Inode No./File ID" <<"File Name" << "Extension"  << "File Path" << "File Size"
    //               << "Mime Type" << "Hashset Name" << "MD5" << "SHA1"  << "Decompression Status" << "Date Modified"  << "Date Change" << "Date Accessed"
    //               << "Date Added"  << "Content Creation Date"  << "Content Modification Date"  << "Last Used Date" << "Use Count"
    //               << "Permission Restricted" <<"Source Count Name" << "Notes Value" << "Tag Value" << "OS Scheme" << "";

    //    headerlist << "" << "" << "" << "Seen" << "Record No." << "Inode No./File ID" <<"File Name" << "Extension" << "File Size"
    //               << "Date Modified" << "Date Change" << "Date Accessed" << "Date Added"
    //               << "Content Creation Date"  << "Content Modification Date"  << "Last Used Date"
    //               << "Use Count" << "File Path"
    //               << "Mime Type" << "Hashset Name" << "MD5" << "SHA1"  << "Decompression Status"
    //               << "Permission Restricted" <<"Source Count Name" << "Notes Value" << "Tag Value" << "OS Scheme" << "";


    headerlist << "" << "" << "" << "Seen" << "Record No." << "Inode No./File ID" <<"File Name" << "Extension" << "File Size"
               << "Date Modified" << "Date Change" << "Date Accessed" << "Date Created" << "Last Used Date" << "Use Count" << "Date Added"
               << "Content Creation Date"  << "Content Modification Date" << "File Path"
               << "Mime Type" << "Hashset Name" << "MD5" << "SHA1"  << "Decompression Status"
               << "Permission Restricted" << "OCR Text" <<"Source Count Name" << "Notes Value" << "Tag Value"  << "OS Scheme" << "";


    QStringList empty_header_stringlist;
    m_file_system_tablewidget_obj->setColumnCount(headerlist.size());

    for(int column_count = 0; column_count < headerlist.size(); column_count++)
    {
        QString column_name = headerlist.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;
        custom_table_header_widget_obj->pub_set_column_name_for_file_system(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

        m_file_system_tablewidget_obj->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        if(column_count ==  enum_File_System_display_BOOKMARK)
        {
            QPixmap bookmark("../icons/signs/bookmark.png");
            custom_table_header_widget_obj->pub_set_icon_on_bookmark_or_seen(bookmark);
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        else if(column_count == enum_File_System_display_SEEN)
        {
            QPixmap seen("../icons/signs/seen.png");
            custom_table_header_widget_obj->pub_set_icon_on_bookmark_or_seen(seen);
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        else if(column_count == enum_File_System_display_NOTES_ICON || column_count == enum_File_System_display_TAG_ICON)
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }

        empty_header_stringlist << "";

        connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
        connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
        connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));
        custom_header_widgets_QList.append(custom_table_header_widget_obj);
    }
    m_file_system_tablewidget_obj->setHorizontalHeaderLabels(empty_header_stringlist);



    QHeaderView *fs_header_view_obj =  m_file_system_tablewidget_obj->horizontalHeader();
    connect(fs_header_view_obj , SIGNAL(sectionClicked(int)), this , SLOT(slot_table_widget_header_item_clicked(int)));

    for(int i = 0; i < 200; i++)
    {

        m_hovered_pushbutton_with_signal *dir_navigation_button = new m_hovered_pushbutton_with_signal(this);
        connect(dir_navigation_button, SIGNAL(signal_hover_enter_or_leave(bool, m_hovered_pushbutton_with_signal*)),this, SLOT(slot_dir_navigation_button_hover_enter_or_leave(bool, m_hovered_pushbutton_with_signal*)));
        connect(dir_navigation_button, SIGNAL(signal_mouse_click_on_pushbutton(QString)),this,SLOT(slot_dir_navigator_pushbutton_clicked(QString)));

        dir_navigation_button->setContentsMargins(0,0,0,0);
        dir_navigation_button->setFlat(true);
        dir_navigation_button->setStyleSheet("QPushButton { background: transparent;text-align: left;}");

        hlayout_frame_dir_navigation->addWidget(dir_navigation_button);

        dir_navigation_button_array[i] = dir_navigation_button;

        dir_navigation_button->hide();

    }

}

void file_system::pub_insert_entry_info_frame_dir_navigator(QString file_path)
{
    recon_static_functions::app_debug("start ", Q_FUNC_INFO);

    QString file_path_str = file_path;


    if(file_path_str.startsWith(fs_extraction_path))
        file_path_str.remove(0,fs_extraction_path.size());

    if(!file_path_str.startsWith("/"))
        file_path_str.prepend("/");

    if(file_path.size() > 1 && file_path.endsWith("/"))
        file_path.chop(1);

    QStringList splitted_path_list = file_path_str.split("/", Qt::SkipEmptyParts);
    splitted_path_list.removeAll("");


    for(int i = 0; i < 200; i++)
    {
        dir_navigation_button_array[i]->hide();
    }

    QString extraction_path = struct_target_source_info.fs_extraction_source_path;
    if(extraction_path.endsWith("/"))
        extraction_path.chop(QString("/").size());

    QFileInfo info(extraction_path);
    splitted_path_list.prepend(info.fileName());


    QString dir_initial_path = "/";
    for(int i = 0; i < splitted_path_list.size(); i++)
    {
        QString dir_name = splitted_path_list.at(i);
        m_hovered_pushbutton_with_signal *dir_navigation_button = dir_navigation_button_array[i];
        if(i == 0)
        {
            if(struct_target_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal))
                dir_navigation_button->setText(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File) + " ›");        // Press alt + shift + 4 and print ›
            else if(struct_target_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal))
                dir_navigation_button->setText(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage) + " ›");        // Press alt + shift + 4 and print ›
            else
                dir_navigation_button->setText(struct_target_source_info.source_name + " ›");        // Press alt + shift + 4 and print ›


            dir_navigation_button->pub_set_navigation_path_on_pushsbutton(dir_initial_path);
        }
        else
        {
            dir_navigation_button->setText(dir_name + " ›");

            dir_initial_path.append(dir_name);
            dir_initial_path.append("/");
            dir_navigation_button->pub_set_navigation_path_on_pushsbutton(dir_initial_path);
        }

        QIcon icon_path_dir;
        if(i == 0)
        {
            struct_GLOBAL_witness_info_source struct_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
            icon_path_dir = global_recon_helper_singular_class_obj->get_qicon_of_source(struct_src_info);

        }
        else
        {
            QString mpath = dir_initial_path;
            if(mpath.endsWith("/"))
                mpath.chop(QString("/").size());

            QString cmd_itm = "select INT from files where filepath = ?";
            QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_itm, QStringList(mpath), 0, destination_db_obj, Q_FUNC_INFO);

            icon_path_dir = recon_helper_standard_obj->get_qicon_of_record_no_by_dbreference(record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        }

        dir_navigation_button->setIcon(icon_path_dir);
        dir_navigation_button->show();
    }

    recon_static_functions::app_debug("end ", Q_FUNC_INFO);

}

void file_system::set_fs_display(QString received_command , QString parent_dir_path , bool clear_previous_data, bool is_recursive_display)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    if(received_command.isEmpty())
        return;

    clear_all_custom_header_widget_entries();
    // ======================

    QMap<QString, QString> tag_key_value_map;

    QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString command = "select tag_name, selected_colour from 'tags'";
    QStringList tag_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, tag_db_path, Q_FUNC_INFO);
    QStringList color_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 1, tag_db_path, Q_FUNC_INFO);

    if(tag_list.size() == color_list.size())
    {
        for(int i = 0; i < tag_list.size(); i++)
        {
            tag_key_value_map.insert(tag_list.at(i), color_list.at(i));
        }
    }
    // ======================

    //=================Check for columns hidden by manage columns -  ===========///

    for(int col= 0; col < m_file_system_tablewidget_obj->columnCount(); col++)
    {
        m_file_system_tablewidget_obj->showColumn(col);
    }

    QList<struct_manage_column_info> column_no_and_name_list;
    column_no_and_name_list = get_hidden_column_from_manage_column_db();
    for(int i= 0; i < column_no_and_name_list.size(); i++)
    {
        struct_manage_column_info var = column_no_and_name_list.at(i);
        m_file_system_tablewidget_obj->hideColumn(var.hid_col_no);
    }
    //=================


    QString dir_navigate_path = parent_dir_path;

    if(parent_dir_path.startsWith(fs_extraction_path))
        parent_dir_path.remove(0,fs_extraction_path.size());

    if(!parent_dir_path.startsWith("/"))
        parent_dir_path.prepend("/");


    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_filesystem);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }
    destination_db_obj = destination_db;

    if(is_recursive_display)
        parent_dir_path.append("%");


    if(clear_previous_data)
        pub_insert_entry_info_frame_dir_navigator(dir_navigate_path);

    QSqlQuery query_select(destination_db);
    query_select.prepare(received_command);
    query_select.addBindValue(parent_dir_path);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query_select execution---FAILED----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        return;
    }


    if(clear_previous_data)
    {
        m_file_system_tablewidget_obj->setRowCount(0);
    }

    QString temp_command ;
    QString  total_records_str;
    if(is_recursive_display)
    {
        temp_command = QString("Select  count(*) from files where file_parent_path Like ?");
    }
    else
    {
        temp_command = QString("Select  count(*) from files where file_parent_path = ?");
    }

    total_records_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(temp_command , QStringList(parent_dir_path), 0 , destination_db , Q_FUNC_INFO);

    int row = 0;
    st_gallery_view_file_info_list.clear();

    prepare_hashset_db_data_list();

    QStringList hashsets_to_be_hidden = get_hashsets_list_to_be_hidden();

    //    QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    //    QStringList hashset_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select database_path from tbl_hashset"), 0, case_conf_db, Q_FUNC_INFO);

    //    hashset_db_data_list.clear();
    //    for(int count = 0; count < hashset_db_path_list.size(); count++)
    //    {
    //        struct_global_hashset_db_data hashset_obj;

    //        hashset_obj.hashset_db_path = hashset_db_path_list.at(count);
    //        hashset_obj.hashet_table_name  = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select table_name from tbl_hashset Where database_path = ?"), QStringList(hashset_obj.hashset_db_path), 0, case_conf_db, Q_FUNC_INFO);
    //        hashset_obj.hashet_column_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select column_name from tbl_hashset Where database_path = ?"), QStringList(hashset_obj.hashset_db_path), 0, case_conf_db, Q_FUNC_INFO);

    //        QString hash_name = QFileInfo(hashset_obj.hashset_db_path).fileName();
    //        hash_name.remove(QFileInfo(hashset_obj.hashset_db_path).suffix());
    //        if(hash_name.endsWith("."))
    //            hash_name.chop(1);

    //        hashset_obj.hash_name_str = hash_name;

    //        hashset_db_data_list.append(hashset_obj);
    //    }


    bool bool_highlight_user_opened_files = global_narad_muni_class_obj->get_field(MACRO_NARAD_Preferences_User_Opened_Files_bool).toBool();

    while (query_select.next())
    {
        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break;
        }

        if(row % 300 == 0 && row > 0)
        {
            QCoreApplication::processEvents();

            qint64 total_records_count = total_records_str.toLongLong();
            qint64 precessed_records_count = row;
            loading_display_dialog_based_on_records(total_records_count,precessed_records_count);
        }
        if(row == 300)
            display_loading_progress_bar_obj->show();

        ///get matched hashset names
        QString md5_hash_str  = query_select.value("hash_md5").toString();
        QString sha1_hash_str = query_select.value("hash_sha1").toString();

        QStringList matched_hashset_name_list;
        for(int num = 0; num < hashset_db_data_list.size(); num++)
        {
            struct_global_hashset_db_data hashset_obj = hashset_db_data_list.at(num);
            QString command_str = "Select " + hashset_obj.hashet_column_name + " From " + hashset_obj.hashet_table_name + " Where " + hashset_obj.hashet_column_name + " = ?";

            QString matched_hash_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, QStringList(md5_hash_str), 0, hashset_obj.hashset_db_path, Q_FUNC_INFO);
            matched_hash_str = matched_hash_str.trimmed();

            if(!matched_hash_str.isEmpty())
                matched_hashset_name_list << hashset_obj.hash_name_str;
        }


        /// hide record for hidden hashset
        if(check_fs_record_hidden_for_hashset(hashsets_to_be_hidden, matched_hashset_name_list))
            continue;


        m_file_system_tablewidget_obj->insertRow(row);
        m_file_system_tablewidget_obj->setRowHeight(row,22);



        /// do not delete, it is sample query type

        // bookmark,INT,filename, extension ,filepath,mime_type,z_date_time_modified,
        // z_date_time_change,z_date_time_accessed,z_date_time_created,file_size,notes,recon_tag_value, ""

        QTableWidgetItem *item_seen = new  QTableWidgetItem;
        QTableWidgetItem *item_bk = new QTableWidgetItem;
        QTableWidgetItem *item_notes_icon = new QTableWidgetItem;
        QTableWidgetItem *item_INT = new QTableWidgetItem;
        QTableWidgetItem *item_inode_no = new QTableWidgetItem;
        QTableWidgetItem *item_tag_icon = new QTableWidgetItem;
        QTableWidgetItem *item_filename = new QTableWidgetItem;
        QTableWidgetItem *item_extension = new QTableWidgetItem;
        QTableWidgetItem *item_filepath = new QTableWidgetItem;
        QTableWidgetItem *item_mimetype = new QTableWidgetItem;
        QTableWidgetItem *item_hashset_name = new QTableWidgetItem;
        QTableWidgetItem *item_hash_md5  = new QTableWidgetItem;
        QTableWidgetItem *item_hash_sha1 = new QTableWidgetItem;
        QTableWidgetItem *item_decompression_stat = new QTableWidgetItem;
        QTableWidgetItem *item_date_lastmodified = new QTableWidgetItem;
        QTableWidgetItem *item_date_change = new QTableWidgetItem;
        QTableWidgetItem *item_date_last_read = new QTableWidgetItem;
        QTableWidgetItem *item_date_created = new QTableWidgetItem;
        QTableWidgetItem *item_file_size = new QTableWidgetItem;
        QTableWidgetItem *item_source_count_name = new QTableWidgetItem;
        QTableWidgetItem *item_prmsn = new QTableWidgetItem;
        QTableWidgetItem *item_kMDItemUseCount = new QTableWidgetItem;
        QTableWidgetItem *item_kMDItemLastUsedDate = new QTableWidgetItem;
        QTableWidgetItem *item_kMDItemContentCreationDate = new QTableWidgetItem;
        QTableWidgetItem *item_kMDItemContentModificationDate = new QTableWidgetItem;
        QTableWidgetItem *item_kMDItemDateAdded = new QTableWidgetItem;
        QTableWidgetItem *item_notes_value = new QTableWidgetItem;
        QTableWidgetItem *item_tag_value = new QTableWidgetItem;
        QTableWidgetItem *item_os_scheme = new QTableWidgetItem;
        QTableWidgetItem *item_blank_column = new QTableWidgetItem;
        QTableWidgetItem *item_ocr_text = new QTableWidgetItem;

        item_seen->setCheckState(Qt::Unchecked);
        if(query_select.value("is_seen").toString()=="1")
            item_seen->setCheckState(Qt::Checked);

        item_bk->setCheckState(Qt::Unchecked);
        if(query_select.value("bookmark").toString() == "1")
            item_bk->setCheckState(Qt::Checked);

        QString notes_str = query_select.value("notes").toString().trimmed();
        if(!notes_str.isEmpty())
        {
            QIcon iconNotes("../icons/notes.png");
            item_notes_icon->setIcon(iconNotes);
            item_notes_icon->setToolTip(notes_str);
        }
        item_notes_value->setText(notes_str);
        item_os_scheme->setText(os_scheme_display);

        qint64 record_no_value = query_select.value("INT").toLongLong();
        QString file_path =  query_select.value("filepath").toString();
        QString full_filepath;
        QString thumbnail_full_filepath;

        bool bool_is_it_file = false;

        QString fs_record_no;
        QIcon icon_path = recon_helper_standard_obj->get_qicon_of_record_no_by_dbreference(QString::number(record_no_value),destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        item_filename->setIcon(icon_path);

        bool bool_dir_record = recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(QString::number(record_no_value),destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
        if(!bool_dir_record)
        {
            fs_record_no = QString::number(record_no_value);
            full_filepath = recon_helper_standard_obj->get_file_infopath_by_dbreference(QString::number(record_no_value),destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
            bool_is_it_file = true;
        }

        /*================= Do Not Remove - Start ===============
         * Note: If u dont want to show folder/directories in display
         * when show files recursive clicked.
         */
        //        if(is_recursive_display)
        //        {
        //            if(QFileInfo(tmp_filepath).isDir())
        //            {
        //                m_file_system_tablewidget_obj->removeRow(row);
        //                continue;
        //            }
        //        }
        //================= Do Not Remove - End ===============


        item_hashset_name->setText(matched_hashset_name_list.join(","));

        item_INT->setData(Qt::EditRole , record_no_value);
        item_INT->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);


        QString uncmp_stat    = query_select.value("decompression_status").toString();

        QString decom_level = "Decompressed";
        if(uncmp_stat == MACRO_TRUE_VALUE_NUMERIC_QString)
            uncmp_stat = decom_level;
        else
            uncmp_stat = "";

        item_hash_md5->setText(md5_hash_str);
        item_hash_sha1->setText(sha1_hash_str);
        item_decompression_stat->setText(uncmp_stat);



        if(!query_select.value("inode_no").toString().isEmpty())
        {
            item_inode_no->setData(Qt::EditRole , query_select.value("inode_no").toLongLong());
            item_inode_no->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        }
        else
            item_inode_no->setText(QString(""));

        item_filename->setText(query_select.value("filename").toString());
        item_filepath->setText(file_path);

        item_extension->setText(query_select.value("extension").toString());

        item_ocr_text->setText(query_select.value("ocr_text").toString());

        QString mime_type_str = query_select.value("mime_type").toString();
        item_mimetype->setText(mime_type_str);


        item_mimetype->setText(query_select.value("mime_type").toString());
        item_date_lastmodified->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("z_date_time_modified").toString(), Q_FUNC_INFO));
        item_date_change->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("z_date_time_change").toString(), Q_FUNC_INFO));
        item_date_last_read->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("z_date_time_accessed").toString(), Q_FUNC_INFO));
        item_date_created->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("z_date_time_created").toString(), Q_FUNC_INFO));

        QString date_lused_str = query_select.value("kMDItemLastUsedDate_Last_Opened_Date").toString();
        item_kMDItemLastUsedDate->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(date_lused_str, Q_FUNC_INFO));


        item_kMDItemContentCreationDate->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("kMDItemContentCreationDate_Created_Date").toString(), Q_FUNC_INFO));
        item_kMDItemContentModificationDate->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("kMDItemContentModificationDate_Last_Modified_Date").toString(), Q_FUNC_INFO));
        item_kMDItemDateAdded->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value("kMDItemDateAdded_Date_Added").toString(), Q_FUNC_INFO));
        item_source_count_name->setText(source_count_name);

        QString usecount_str = query_select.value("kMDItemUseCount_Used_Count").toString().trimmed();
        if(usecount_str.isEmpty())
            item_kMDItemUseCount->setText(usecount_str);
        else
        {
            item_kMDItemUseCount->setData(Qt::EditRole , query_select.value("kMDItemUseCount_Used_Count").toLongLong());
            item_kMDItemUseCount->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        }

        item_prmsn->setText(query_select.value("permission_restricted_file").toString());

        if(bool_dir_record)
        {
            item_file_size->setText("--");
        }
        else
        {
            item_file_size->setData(Qt::EditRole , query_select.value("file_size").toLongLong());
        }
        item_file_size->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        item_blank_column->setText("");

        item_tag_value->setText(query_select.value("recon_tag_value").toString().trimmed());
        QString tags_icon_path;
        if(query_select.value("recon_tag_value").toString().trimmed().contains(","))
        {
            tags_icon_path = recon_static_functions::get_tags_icon_path("",query_select.value("recon_tag_value").toString().trimmed(), Q_FUNC_INFO);
        }
        else
        {
            QString color_name;
            QMapIterator<QString, QString> iter_tag(tag_key_value_map);
            while (iter_tag.hasNext()) {
                iter_tag.next();


                if(iter_tag.key() == query_select.value("recon_tag_value").toString().trimmed())
                {
                    color_name = iter_tag.value();
                    break;
                }
            }

            tags_icon_path = recon_static_functions::get_tags_icon_path(color_name,query_select.value("recon_tag_value").toString().trimmed(), Q_FUNC_INFO);
        }

        QIcon icon(tags_icon_path);
        item_tag_icon->setIcon(icon);
        item_tag_icon->setToolTip(item_tag_value->text());

        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_SEEN,item_seen);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_BOOKMARK,item_bk);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_NOTES_ICON,item_notes_icon);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_TAG_ICON,item_tag_icon);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_INT,item_INT);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_INODE_NO,item_inode_no);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_FILENAME,item_filename);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_EXTENSION,item_extension);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_FILEPATH,item_filepath);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_MIMETYPE,item_mimetype);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_HASHSET_NAME,item_hashset_name);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_HASH_MD5,item_hash_md5);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_HASH_SHA1,item_hash_sha1);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_DECOMPRESSION_STATUS,item_decompression_stat);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_DATE_MODIFIED,item_date_lastmodified);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_DATE_CHANGE,item_date_change);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_DATE_ACCESSED,item_date_last_read);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_DATE_CREATED,item_date_created);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_Date_Added,item_kMDItemDateAdded);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_Content_Creation_Date,item_kMDItemContentCreationDate);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_Content_Modification_Date,item_kMDItemContentModificationDate);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_Last_Used_Date,item_kMDItemLastUsedDate);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_Use_Count,item_kMDItemUseCount);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_SOURCE_COUNT_NAME,item_source_count_name);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_PERMISSION_RESTRICTED,item_prmsn);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_FILESIZE,item_file_size);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_NOTES_VALUE,item_notes_value);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_TAG_VALUE,item_tag_value);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_OCR_TEXT,item_ocr_text);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_OS_SCHEME,item_os_scheme);
        m_file_system_tablewidget_obj->setItem(row,enum_File_System_display_BLANK_COLUMN,item_blank_column);



        QString is_seen_str = query_select.value("is_seen").toString().trimmed();
        if(is_seen_str == QString::number(1))
            change_textcolor_for_seen_unseen(row , false);


        if(item_decompression_stat->text() == QString(decom_level))
            change_textcolor_for_compression_decompression(row , true);

        if(is_seen_str == QString::number(1) && bool_Global_hide_seen_files)
            m_file_system_tablewidget_obj->hideRow(row);





        if(bool_highlight_user_opened_files)
        {
            bool bool_used_count_available = false;
            usecount_str = usecount_str.trimmed();

            if(!usecount_str.isEmpty() && usecount_str.toInt() > 0)
                bool_used_count_available = true;

            if(!date_lused_str.isEmpty() && bool_used_count_available)
                change_row_background_color_for_last_used_date(row);
        }



        if(bool_is_it_file)
        {

            //            if(mime_type_str.startsWith("image/") || recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
            //            {
            //                struct_global_gallery_view_file_info f1;
            //                f1.file_name = item_filename->text();

            //                f1.file_path = item_filepath->text();
            //                f1.full_file_path = full_filepath;
            //                f1.INT = item_INT->text();
            //                f1.table_view_row_no = row;
            //                f1.bookmark = "0";
            //                f1.latitude = "";
            //                f1.longitude = "";
            //                f1.last_access_timestamp = item_date_last_read->text();
            //                f1.source_count_name = source_count_name;
            //                f1.index = st_gallery_view_file_info_list.size();

            //                if(item_bk->checkState() == Qt::Checked)
            //                    f1.bookmark = "1";

            //                st_gallery_view_file_info_list.append(f1);
            //            }

            QString file_name = item_filename->text();
            QString file_size = item_file_size->text();

            if(mime_type_str.startsWith("video/") || recon_helper_standard_obj->is_gallery_view_suppoprted_video_file(file_name))
            {

                QString video_thumbnails_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + MACRO_Video_Thumbnails_Dir_Name_QString;

                QString video_thumbnails_db_path = video_thumbnails_dir_path + "video_thumbnails.sqlite";

                QString select_cmd = "select thumb_filepath from files where filesystem_record = ?";
                QString result_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString thumb_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(fs_record_no),0,video_thumbnails_db_path,Q_FUNC_INFO);

                if(!thumb_file_path.isEmpty())
                {
                    thumbnail_full_filepath = result_filepath + thumb_file_path;
                }
                else
                {
                    file_name.remove(file_name.lastIndexOf("."),file_name.size());
                    video_thumbnails_dir_path = video_thumbnails_dir_path + file_name + "_" + fs_record_no + ".png";

                    //                    QString thumbnail_script_path = "../Resources/scripts_persistent/video_frame.py";
                    //                    QString cmd_python_path = global_varibale_python_path;

                    //                    QStringList args;
                    //                    args << thumbnail_script_path << full_filepath << video_thumbnails_dir_path << file_size;

                    QString video_frame_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/video_frame";

                    QStringList args;
                    args << full_filepath << video_thumbnails_dir_path << file_size;

                    QProcess process_thumbnails;
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
                    thumbnail_full_filepath = QFileInfo(video_thumbnails_dir_path).absoluteFilePath();
                    QFileInfo thumb_file_existence(thumbnail_full_filepath);

                    QString thumb_filepath;
                    QStringList list;
                    QString insert_cmd = "insert into files(filesystem_record, thumb_filepath) values(?,?)";

                    if(thumb_file_existence.exists())
                    {
                        thumb_filepath = thumbnail_full_filepath;
                        thumb_filepath = thumb_filepath.remove(0,result_filepath.size());

                        if(!thumb_filepath.startsWith("/"))
                        {
                            thumb_filepath = thumb_filepath.prepend("/");
                        }

                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnails_db_path,Q_FUNC_INFO);
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

                        list << fs_record_no << thumb_filepath;

                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd,list,video_thumbnails_db_path,Q_FUNC_INFO);
                    }
                }

                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();

                f1.file_path = item_filepath->text();
                f1.thumbnail_file_path = thumbnail_full_filepath;
                f1.full_filepath = full_filepath;
                f1.INT = item_INT->text();
                f1.table_view_row_no = row;
                f1.bookmark = "0";
                f1.latitude = "";
                f1.longitude = "";
                f1.last_access_timestamp = item_date_last_read->text();
                f1.source_count_name = source_count_name;
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }
            else if(mime_type_str.startsWith("image/") || recon_helper_standard_obj->is_gallery_view_suppoprted_file(item_filename->text(), Q_FUNC_INFO))
            {
                struct_global_gallery_view_file_info f1;
                f1.file_name = item_filename->text();

                f1.file_path = item_filepath->text();
                f1.thumbnail_file_path = full_filepath;
                f1.full_filepath = full_filepath;
                f1.INT = item_INT->text();
                f1.table_view_row_no = row;
                f1.bookmark = "0";
                f1.latitude = "";
                f1.longitude = "";
                f1.last_access_timestamp = item_date_last_read->text();
                f1.source_count_name = source_count_name;
                f1.index = st_gallery_view_file_info_list.size();

                if(item_bk->checkState() == Qt::Checked)
                    f1.bookmark = "1";

                st_gallery_view_file_info_list.append(f1);
            }
        }

        row++; // Do not change this position of row.
    }


    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_SEEN,40);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_BOOKMARK,40);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_NOTES_ICON,30);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_TAG_ICON,30);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_INT,115);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_INODE_NO,145);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_FILENAME,255);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_EXTENSION,105);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_FILEPATH,425);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_MIMETYPE,165);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_HASHSET_NAME,165);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_DATE_MODIFIED,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_DATE_CHANGE,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_DATE_ACCESSED,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_DATE_CREATED,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_Date_Added,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_Content_Creation_Date,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_Content_Modification_Date,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_Last_Used_Date,245);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_Use_Count,95);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_SOURCE_COUNT_NAME,175);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_PERMISSION_RESTRICTED,175);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_FILESIZE,105);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_BLANK_COLUMN,105);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_DECOMPRESSION_STATUS,195);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_HASH_MD5,285);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_HASH_SHA1,325);
    m_file_system_tablewidget_obj->setColumnWidth(enum_File_System_display_OCR_TEXT,245);
    m_file_system_tablewidget_obj->horizontalHeader()->stretchLastSection();
    m_file_system_tablewidget_obj->hideColumn(enum_File_System_display_PERMISSION_RESTRICTED);

    set_buttons();

    emit signal_fs_refresh_widgets();

    //    QString mime_db_dettach_commmand = QString("DETACH DATABASE mimeDB");
    //    query.exec(mime_db_dettach_commmand);

    bookmarked_records_from_table_display_list = recon_helper_standard_obj->pub_get_records_bookmarked_from_table_display_by_db_refrence(destination_db, "files", "INT", Q_FUNC_INFO);

    bool_did_tableview_display_changed = true;

    //=================Check for columns hidden by manage columns -  ===========///
    QList<struct_manage_column_info> column_no_and_name_list_1;
    column_no_and_name_list_1 = get_hidden_column_from_manage_column_db();
    for(int i= 0; i < column_no_and_name_list_1.size(); i++)
    {
        struct_manage_column_info var = column_no_and_name_list_1.at(i);
        m_file_system_tablewidget_obj->hideColumn(var.hid_col_no);
    }

    refresh_horizontal_header();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);

}

QStringList file_system::get_hashsets_list_to_be_hidden()
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

void file_system::prepare_hashset_db_data_list()
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

bool file_system::check_fs_record_hidden_for_hashset(QStringList &hashes_hidden_list, QStringList matched_hashset_list)
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

void file_system::set_buttons()
{
    pushButton_next->setEnabled(true);
    pushButton_back->setEnabled(true);

    if(deepest_path == current_parent_dir_path.trimmed())
        pushButton_next->setEnabled(false);

    if(current_parent_dir_path.trimmed() == source_path)
        pushButton_back->setEnabled(false);
}


QString file_system::extract_command_type(QString attr)
{
    QString db_Path = case_confg_dbpath;

    QString command = QString("select data_type from tbl_apple_attributes where metadata_attribute_name = ?");
    QStringList list;
    list << attr;

    return recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,0,db_Path,Q_FUNC_INFO);
}

void file_system::create_right_click_actions(QMenu *mainMenu , QStringList submenuList)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    for(int i = 0; i < submenuList.size(); i++)
    {
        QAction *act_submenu = new QAction(submenuList.at(i).toLocal8Bit(), this);

        if(submenuList.at(i).isEmpty())
            act_submenu->setSeparator(true);

        mainMenu->addAction(act_submenu);
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

bool file_system::eventFilter(QObject *obj, QEvent *event)
{
    //    if(obj == m_file_system_tablewidget_obj)
    //    {
    if(event->type() == QEvent::ContextMenu)
    {

        QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();
        if(selection_model_list.size() < 1)
            return false;

        add_applications_to_open_application_menu();

        display_hashset_add_files_submenu();

        if(selection_model_list.size() == 1)
        {
            int current_row = m_file_system_tablewidget_obj->currentRow();
            int current_column = m_file_system_tablewidget_obj->currentColumn();

            tablewidget_general_clicked(current_row,current_column);
        }

        if(check_entry_on_row_is_directory(m_file_system_tablewidget_obj->currentRow()))
        {
            submenu_open_with->setEnabled(false);
            submenu_send_to_bucket->setEnabled(false);
            submenu_add_files_to_hashset_db->setEnabled(false);
            action_search_file_with_same_md5_hash->setEnabled(false);
            act_quick_look->setEnabled(true);
        }
        else
        {
            submenu_open_with->setEnabled(true);
            submenu_send_to_bucket->setEnabled(true);
            submenu_add_files_to_hashset_db->setEnabled(true);
            if(selection_model_list.size() > 1)
            {
                action_search_file_with_same_md5_hash->setEnabled(false);
            }
            else
            {
                action_search_file_with_same_md5_hash->setEnabled(true);
            }

            act_quick_look->setEnabled(true);
        }


        QString complete_path = get_current_complete_file_path();

        struct_global_right_clicks_status_depends_on_file_existance obj;
        obj.feature_name = QString(MACRO_CASE_TREE_Source);
        obj.plugin_name = MACRO_Plugin_Name_File_System;
        obj.tab_name = MACRO_Plugin_File_System_TAB_Name_Files;
        obj.selected_row_count = selection_model_list.size();
        obj.record_no = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text();
        obj.source_count_name = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_SOURCE_COUNT_NAME)->text();
        recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

        bool status = false;

        if(QFileInfo(complete_path).isFile())
            status = true;

        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Carve_Files))
            {
                action->setEnabled(status);
            }

            if(action->text() == QString(MACRO_Generic_Right_Click_Face_Search))
            {
                action->setEnabled(false);
                if(selection_model_list.size() == 1)
                {
                    action->setEnabled(status);
                }
            }

        }

        foreach (QAction *action, mainMenu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Decompress_File))
            {
                // action should disable for multi selection OR not-supported file

                action->setEnabled(false);
                if((selection_model_list.size() > 1))
                    break;

                QString f_name = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(), enum_File_System_display_FILENAME)->text().trimmed();
                if(f_name.endsWith(".zip",Qt::CaseInsensitive) || f_name.endsWith(".tar",Qt::CaseInsensitive)
                        || f_name.endsWith(".tar.gz",Qt::CaseInsensitive) || f_name.endsWith(".tar.bz2",Qt::CaseInsensitive))
                {
                    action->setEnabled(true);
                }

                break;
            }
        }

        ///Disable if current file is decompressed
        // bool is_decompressed = check_selected_row_is_decompressed();
        bool bool_record_decompressed = recon_helper_standard_obj->check_record_is_decompressed_by_dbreference(m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),enum_File_System_display_INT)->text(),destination_db_obj,Q_FUNC_INFO);
        bool bool_source_nested = struct_target_source_info.bool_source_nested;
        foreach (QAction *action, submenu_run_fs_modules->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Apple_Metadata))
            {
                if((struct_target_source_info.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_HFS)
                    || struct_target_source_info.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_APFS))
                        && (!bool_source_nested && !bool_record_decompressed))
                {
                    action->setEnabled(true);
                }
                else
                    action->setEnabled(false);

            }


            if(action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Optical_Character_Recognition)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Weapons)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Fire_Analysis)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Face_Analysis)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules_Skin_Tone_Detection))
            {
                action->setEnabled(false);
                if((selection_model_list.size() == 1) && (recon_case_obj->pub_is_source_mime_type_extraction_completed(source_count_name)))
                {
                    action->setEnabled(true);
                }
            }

        }


        if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_TABLE_VIEW)
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
                else if(action->text() == QString(MACRO_Generic_Right_Click_List_Related_Files))
                {
                    if(selection_model_list.size() == 1)
                    {
                        QString f_name = m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(), enum_File_System_display_FILENAME)->text().trimmed();

                        if(f_name.endsWith(".app") && os_scheme_display ==  QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
                        {
                            action->setEnabled(true);
                        }
                        else
                            action->setEnabled(false);
                    }
                    else
                        action->setEnabled(false);
                }
            }

        }
        else if(main_tab_widget_obj->currentIndex() == enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW)
        {
            if(st_gallery_view_file_info_list.size() == 0)
                return false;

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

        bool bool_src_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(source_count_name);
        if(!bool_src_applicable)
        {
            submenu_run_fs_modules->setEnabled(false);
        }

        mainMenu->exec(QCursor::pos());

        return true;
    }

    //        return false;
    //    }

    return false;
}

QMenu* file_system::create_right_click_submenu(QString submenuStr ,QMenu *mainMenu , QMenu *subMenu)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    subMenu = mainMenu->addMenu(tr(submenuStr.toLocal8Bit().data()));

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    subMenu->setFont(m_font);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return subMenu;
}

bool file_system::check_entry_on_row_is_directory(int selected_row)
{
    if(selected_row < 0 || selected_row >= m_file_system_tablewidget_obj->rowCount())
        return false;

    QString record_no_str = m_file_system_tablewidget_obj->item(selected_row,enum_File_System_display_INT)->text();

    bool bool_dir_record = recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no_str,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);
    if(bool_dir_record)
        return true;


    return false;


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_filesystem);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    QString record_no = m_file_system_tablewidget_obj->item(selected_row,enum_File_System_display_INT)->text();

    QStringList export_col_name_list;
    export_col_name_list << "filepath";

    QString cmd_mime_type = "select mime_type from '" + table_name +  "' where INT = ?";

    QString cmd_info ;

    if(os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || os_naming_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        cmd_info = "select id_path from '" + table_name +  "' where INT = ?";
    }
    else
        cmd_info = "select filepath from '" + table_name +  "' where INT = ?";


    QStringList arg_list;
    arg_list << record_no;
    QString type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_mime_type,arg_list,0,destination_db,Q_FUNC_INFO);
    QString temp_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_info,arg_list,0,destination_db,Q_FUNC_INFO);
    QString complete_file_path = recon_static_functions::get_complete_file_path(fs_extraction_path, temp_path, Q_FUNC_INFO);

    destination_db.close();

    QFileInfo info(complete_file_path);

    if(type == QString("inode/directory") || info.isDir())
        return true;
    else
        return false;
}

QAction* file_system::create_right_click_action_for_enable_disable(QString submenuStr ,QMenu *mainMenu , QAction *action)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    action = new QAction(mainMenu);
    action = mainMenu->addAction(tr(submenuStr.toLocal8Bit().data()));

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    action->setFont(m_font);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return action;
}

void file_system::display_hashset_add_files_submenu()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString command = QString("Select database_path From tbl_hashset");

    QStringList hashset_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, db_path, Q_FUNC_INFO);

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

void file_system::display_tags_submenu()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_show_tag()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_show_tag()));

    ///remove tag
    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    sub_menu_tags->addAction(act_remove_tag);

    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_action_remove_tags_clicked()));
    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_action_remove_tags_clicked()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = add_tags_module_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path);

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

            icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
            QIcon icon(icon_path);

            act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void file_system::update_tag_status_for_current_record(QString tag_data,QString colour)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_filesystem);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();


    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_file_system_tablewidget_obj->rowCount())
            continue;

        if(m_file_system_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no_str = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();

        QString command;
        command = "update '" + table_name + "' set recon_tag_value = ? where INT = ?";
        QStringList arg_list;
        arg_list << tag_data << record_no_str ;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_ICON)->setIcon(icon);
        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_ICON)->setToolTip(tag_data);

        m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_TAG_VALUE)->setText(tag_data);

        emit signal_add_tag_in_other_locations(tag_data,"0",pluginName,QString(MACRO_Plugin_File_System_TAB_Name_Files),record_no_str,db_path_filesystem,os_scheme_display);
    }

    destination_db.close();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}



QStringList file_system::get_applied_tags_name_on_current_record(QString record_no)
{
    QStringList tag_name_list;
    record_no = record_no.trimmed();
    if(record_no.isEmpty())
        return tag_name_list;

    QString command = "SELECT recon_tag_value FROM '" + table_name + "' WHERE INT=?";
    QStringList arg_list;
    arg_list << record_no;
    QString applied_tag = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,db_path_filesystem,Q_FUNC_INFO);

    if(!applied_tag.isEmpty())
    {
        tag_name_list << applied_tag;
    }

    return tag_name_list;
}

void file_system::tablewidget_general_clicked(int row,int column)
{
    if(row < 0) // just in case safe check
        return;


    if(!m_file_system_tablewidget_obj->selectionModel()->selectedRows(column).isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }

    QString file_path =   m_file_system_tablewidget_obj->item(row,enum_File_System_display_FILEPATH)->text().trimmed();
    QString record    =   m_file_system_tablewidget_obj->item(row,enum_File_System_display_INT)->text().trimmed();

    struct_global_fetch_metadata  struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record, source_count_name, Q_FUNC_INFO);

    m_file_system_tablewidget_obj->blockSignals(true);

    update_bookmarks_status_on_checkbox_click(row , column);

    m_file_system_tablewidget_obj->blockSignals(false);
    QModelIndexList selection_model_list = m_file_system_tablewidget_obj->selectionModel()->selectedRows();

    if(column ==  enum_File_System_display_SEEN)
    {
        m_file_system_tablewidget_obj->blockSignals(true);

        m_file_system_tablewidget_obj->selectRow(row);

        m_file_system_tablewidget_obj->blockSignals(false);

        if(m_file_system_tablewidget_obj->item(row,enum_File_System_display_SEEN)->checkState() == Qt::Unchecked)
        {
            action_mark_as_seen_unseen(MACRO_Generic_Right_Click_Mark_As_Unseen);
        }
        else if(m_file_system_tablewidget_obj->item(row,enum_File_System_display_SEEN)->checkState() == Qt::Checked)
        {
            action_mark_as_seen_unseen(MACRO_Generic_Right_Click_Mark_As_Seen);
        }
    }

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.db_file_path = db_path_filesystem;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = QString(MACRO_Plugin_Name_File_System);
    st_set_meta_obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);

    emit signal_set_metadata_info(st_set_meta_obj);

}

QString file_system::extract_attribute_name(QString attr)
{
    attr = attr.trimmed();
    if(attr.isEmpty())
        return QString("");

    QString command = QString("select attribute_name from tbl_apple_attributes where metadata_attribute_name = ?");
    QStringList list;
    list << attr;

    return recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,0,case_confg_dbpath,Q_FUNC_INFO);
}

qint64 file_system::get_header_column_index(QString header)
{
    int index = -1;
    for(int count = 0; count < headerlist.size(); count++)
    {
        if(header == headerlist.at(count).trimmed())
        {
            index = count;
            break;
        }
    }

    return index;
}

QString file_system::get_recon_filefrom_of_selected_row()
{
    qint64 selected_row_no = m_file_system_tablewidget_obj->currentRow();
    if(selected_row_no < 0)
        return QString("");

    QString record_no     = m_file_system_tablewidget_obj->item(selected_row_no,enum_File_System_display_INT)->text();
    QString file_from_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select recon_filefrom from files Where INT = ?"), QStringList(record_no),0,destination_db_obj,Q_FUNC_INFO);

    file_from_str = file_from_str.trimmed();

    return file_from_str;
}

bool file_system::check_selected_row_is_decompressed()
{
    qint64 selected_row_no = m_file_system_tablewidget_obj->currentRow();
    if(selected_row_no < 0)
        return false;

    QString file_from_str = get_recon_filefrom_of_selected_row();
    if(file_from_str.isEmpty())
        return false;

    int file_from_num = file_from_str.toInt();

    if(file_from_num == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result)
        return true;

    return false;
}

QString file_system::get_current_complete_file_path()
{
    ///returns complete path of current row

    int selected_row = m_file_system_tablewidget_obj->currentRow();
    if(selected_row < 0)
        return QString("");

    QString record_no = m_file_system_tablewidget_obj->item(selected_row,enum_File_System_display_INT)->text();
    QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db_obj,"files",source_count_name,Q_FUNC_INFO);

    return complete_path;
}

void file_system::point_to_export_dir(QString path)
{
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void file_system::export_data_as_csv(QStringList csv_headerlist, QString file_path, bool bool_radio_button_screen_item_status, bool bool_recursive_checked_for_csv_only, struct_GLOBAL_witness_info_source struct_info, QStringList hashsets_to_be_hidden, QString info_file_path)
{
    QString csv_file_path = file_path;
    QFile m_file;
    m_file.setFileName(csv_file_path);
    if(!m_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED---" + m_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + m_file.errorString() , Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&m_file);



    ///---------csv headers
    for(int i = 0 ; i < csv_headerlist.size() ; i++)
    {
        //  outputfile_csv << csv_headerlist.at(i) + ",";
        outputfile_csv << "\"" + csv_headerlist.at(i) + "\"" + ",";
    }
    ///---------csv headers

    QString screen_item;
    QString parent_category;
    QString is_recurcive;

    if(bool_radio_button_screen_item_status)
    {
        screen_item = "Screen Items";
        export_as_csv_for_screen_items(outputfile_csv , struct_info);
    }
    else
    {

        QString command  = get_command_for_export_as_csv(bool_recursive_checked_for_csv_only);
        parent_category = deepest_path;
        parent_category = specified_file_path;
        if(parent_category.startsWith(fs_extraction_path))
            parent_category.remove(0,fs_extraction_path.size());

        if(!parent_category.startsWith("/"))
            parent_category.prepend("/");



        QString fs_db_path = db_path_filesystem;
        QString file_system_dir_path = db_path_filesystem;
        file_system_dir_path.chop(QString("file_system.sqlite").size()); // "file_system.sqlite"
        QString apple_meta_db_path = file_system_dir_path + "apple_metadata.sqlite";

        QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(fs_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            m_file.close();
            return;
        }


        QSqlQuery query_select(destination_db);
        QString apple_meta_db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadataDB")
                .arg(QDir::toNativeSeparators(apple_meta_db_path));
        query_select.exec(apple_meta_db_attach_commmand);


        QString total_record_count;
        if(struct_info.os_scheme_display == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
        {
            if(bool_recursive_checked_for_csv_only || !pushbutton_show_files_recursively->isEnabled())
            {
                is_recurcive = "YES";
                query_select.prepare(command);
                QString cmd = QString("Select  count(*) from files where file_parent_path Like ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(parent_category + "%") ,0,fs_db_path , Q_FUNC_INFO);
            }
            else
            {
                is_recurcive = "NO";
                query_select.prepare(command);
                query_select.addBindValue(parent_category);
                QString cmd = QString("Select count(*) from files where file_parent_path = ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(parent_category),0,fs_db_path , Q_FUNC_INFO);
            }

        }
        else
        {
            if(bool_recursive_checked_for_csv_only || !pushbutton_show_files_recursively->isEnabled())
            {
                query_select.prepare(command);
                QString cmd = QString("Select  count(*) from files where file_parent_path Like ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,QStringList(parent_category + "%") ,0,fs_db_path , Q_FUNC_INFO);
            }
            else
            {
                query_select.prepare(command);
                query_select.addBindValue(parent_category);
                QString cmd = QString("Select count(*) from files where file_parent_path = ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(parent_category),0,fs_db_path , Q_FUNC_INFO);
            }
        }
        if(!query_select.exec())
        {
            recon_static_functions::app_debug(" query_select execution---FAILED----" + destination_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select.lastError().text(), Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select.executedQuery(), Q_FUNC_INFO);

            QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
            query_select.exec(apple_meta_db_detach_commmand);
            m_file.close();
            destination_db.close();
            return;
        }


        int record_count = 0;
        int S_number = 1;

        display_loading_progress_bar_obj->pub_set_label_messsge("Please Wait... ");
        display_loading_progress_bar_obj->show();

        while(query_select.next())
        {

            if(record_count % 300 == 0)
                QCoreApplication::processEvents();

            if(bool_cancel_loading)
            {
                bool_cancel_loading = false;
                break;
            }


            QString label_text = QString("Exporting CSV...  ") + QString::number(record_count) + "/" + total_record_count;
            display_loading_progress_bar_obj->pub_set_label_messsge(label_text);
            if(record_count > 0)
            {
                int temp_count = record_count;
                float record_percent = ((temp_count*100)/total_record_count.toInt(0));
                display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));
            }

            ////-----------data
            outputfile_csv << "\n";
            int counter = 0;

            bool bool_hashset_hidden_record = false;

            //while(counter != csv_headerlist.size())
            while(counter != export_as_csv_header_list.size())
            {

                if(hidden_column_header_name_index_list.contains(counter))
                {
                    counter++;
                    continue;
                }

                QString human_readable_size_str;
                QString column_value;
                QString column_name = query_select.record().fieldName(counter);
                if(counter == 0)
                {
                    column_value = QString::number(S_number);
                }
                else if(column_name.contains("size", Qt::CaseInsensitive))
                {
                    qint64 size_in_qint64 = query_select.value(counter).toLongLong();
                    column_value = QString::number(size_in_qint64);
                    human_readable_size_str = recon_static_functions::human_readable_size(size_in_qint64 , Q_FUNC_INFO);
                }
                else if(column_name.contains("Date", Qt::CaseInsensitive))
                {
                    column_value =  global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value(counter).toString(), Q_FUNC_INFO);
                }
                else if(column_name == QString("hashset_name"))
                {
                    QString md5_hash_str = query_select.value(counter + 1).toString();
                    ///get matched hashset names
                    QStringList matched_hashset_name_list;
                    for(int num = 0; num < hashset_db_data_list.size(); num++)
                    {
                        struct_global_hashset_db_data hashset_obj = hashset_db_data_list.at(num);
                        QString command_str = "Select " + hashset_obj.hashet_column_name + " From " + hashset_obj.hashet_table_name + " Where " + hashset_obj.hashet_column_name + " = ?";

                        QString matched_hash_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, QStringList(md5_hash_str), 0, hashset_obj.hashset_db_path, Q_FUNC_INFO);
                        matched_hash_str = matched_hash_str.trimmed();

                        if(!matched_hash_str.isEmpty())
                            matched_hashset_name_list << hashset_obj.hash_name_str;
                    }
                    column_value = matched_hashset_name_list.join(",");

                    /// hide record for hidden hashset
                    if(check_fs_record_hidden_for_hashset(hashsets_to_be_hidden, matched_hashset_name_list))
                    {
                        bool_hashset_hidden_record = true;
                        break;
                    }


                }
                else
                    column_value = query_select.value(counter).toString() ;

                if(counter == 6)
                {
                    column_value = human_readable_size_str;
                }


                ///-----append data in text stream to write in csv file
                outputfile_csv <<  "\"" + column_value + "\"" + ",";

                counter++;

            }

            if(bool_hashset_hidden_record)
                continue;

            ////-----------data
            S_number++;
            record_count++;
        }


        QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
        query_select.exec(apple_meta_db_detach_commmand);


        destination_db.close();
        m_file.close();

        display_loading_progress_bar_obj->hide();

    }

    write_source_file_data_for_export_as_csv_and_sqlite(info_file_path, struct_info, parent_category, is_recurcive, screen_item);

    open_export_as_FS_csv_file(csv_file_path);

}

void file_system::export_data_as_sqlite(QStringList col_headerlist, QString file_path, bool bool_radio_button_screen_item_status, bool bool_recursive_checked_for_csv_only, struct_GLOBAL_witness_info_source struct_info, QStringList hashsets_to_be_hidden,QString info_file_path)
{
    QString export_db_path = file_path;

    QString connection_naam_exp_db = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam_exp_db);
    QSqlDatabase dest_export_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_exp_db);
    dest_export_db.setDatabaseName(export_db_path);
    if(!dest_export_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + dest_export_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + dest_export_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_exp_db);
        return;
    }





    ///=========Command Table Create and Insert-Start=========//
    QString col_str;
    QString cmd_table_create = "Create Table 'files' (";
    QString cmd_insert = QString("Insert into 'files' (");
    QString cmd_insert_val = QString(" Values(");
    for(int num = 0 ; num < col_headerlist.size() ; num++)
    {
        col_str = col_headerlist.at(num);
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

    qry_export_db.clear();
    dest_export_db.transaction();

    QString screen_item;
    QString parent_category;
    QString is_recurcive;


    display_loading_progress_bar_obj->pub_set_label_messsge("Please Wait... ");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    if(bool_radio_button_screen_item_status)
    {
        screen_item = "Screen Items";
        export_as_sqlite_for_screen_items(dest_export_db , struct_info, cmd_insert);
    }
    else
    {

        QSqlQuery qry_insert(dest_export_db);


        QString command  = get_command_for_export_as_csv(bool_recursive_checked_for_csv_only);
        parent_category = deepest_path;
        parent_category = specified_file_path;
        if(parent_category.startsWith(fs_extraction_path))
            parent_category.remove(0,fs_extraction_path.size());

        if(!parent_category.startsWith("/"))
            parent_category.prepend("/");


        QString fs_db_path = db_path_filesystem;
        QString file_system_dir_path = db_path_filesystem;
        file_system_dir_path.chop(QString("file_system.sqlite").size()); // "file_system.sqlite"
        QString apple_meta_db_path = file_system_dir_path + "apple_metadata.sqlite";

        QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase filesystem_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        filesystem_db.setDatabaseName(fs_db_path);
        if(!filesystem_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + filesystem_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + filesystem_db.lastError().text(),Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            dest_export_db.commit();
            dest_export_db.close();
            return;
        }


        QSqlQuery query_fs_select(filesystem_db);
        QString apple_meta_db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadataDB")
                .arg(QDir::toNativeSeparators(apple_meta_db_path));
        query_fs_select.exec(apple_meta_db_attach_commmand);


        QString total_record_count;
        if(struct_info.os_scheme_display == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
        {
            if(bool_recursive_checked_for_csv_only || !pushbutton_show_files_recursively->isEnabled())
            {
                is_recurcive = "YES";
                query_fs_select.prepare(command);
                QString cmd = QString("Select  count(*) from files where file_parent_path Like ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(parent_category + "%") ,0,fs_db_path , Q_FUNC_INFO);
            }
            else
            {
                is_recurcive = "NO";
                query_fs_select.prepare(command);
                query_fs_select.addBindValue(parent_category);
                QString cmd = QString("Select count(*) from files where file_parent_path = ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(parent_category),0,fs_db_path , Q_FUNC_INFO);
            }

        }
        else
        {
            if(bool_recursive_checked_for_csv_only || !pushbutton_show_files_recursively->isEnabled())
            {
                query_fs_select.prepare(command);
                QString cmd = QString("Select  count(*) from files where file_parent_path Like ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,QStringList(parent_category + "%") ,0,fs_db_path , Q_FUNC_INFO);
            }
            else
            {
                query_fs_select.prepare(command);
                query_fs_select.addBindValue(parent_category);
                QString cmd = QString("Select count(*) from files where file_parent_path = ?");
                total_record_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(parent_category),0,fs_db_path , Q_FUNC_INFO);
            }
        }
        if(!query_fs_select.exec())
        {
            recon_static_functions::app_debug(" query_select execution---FAILED----" + filesystem_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(query_fs_select.lastError().text(), Q_FUNC_INFO);
            recon_static_functions::app_debug(query_fs_select.executedQuery(), Q_FUNC_INFO);

            QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
            query_fs_select.exec(apple_meta_db_detach_commmand);
            dest_export_db.commit();
            dest_export_db.close();
            filesystem_db.close();
            return;
        }


        int record_count = 0;
        int S_number = 1;

        while(query_fs_select.next())
        {

            if(record_count % 300 == 0)
                QCoreApplication::processEvents();

            if(bool_cancel_loading)
            {
                bool_cancel_loading = false;
                break;
            }


            QString label_text = QString("Exporting SQLite...  ") + QString::number(record_count) + "/" + total_record_count;
            display_loading_progress_bar_obj->pub_set_label_messsge(label_text);
            if(record_count > 0)
            {
                int temp_count = record_count;
                float record_percent = ((temp_count*100)/total_record_count.toInt(0));
                display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));
            }

            int counter = 0;
            bool bool_hashset_hidden_record = false;

            qry_insert.prepare(cmd_insert);

            while(counter != col_headerlist.size())
            {
                if(hidden_column_header_name_index_list.contains(counter))
                {
                    counter++;
                    continue;
                }

                QString human_readable_size_str;
                QString column_value;
                QString column_name = query_fs_select.record().fieldName(counter);
                if(counter == 0)
                {
                    column_value = QString::number(S_number);
                }
                else if(column_name.contains("size", Qt::CaseInsensitive))
                {
                    qint64 size_in_qint64 = query_fs_select.value(counter).toLongLong();
                    column_value = QString::number(size_in_qint64);
                    human_readable_size_str = recon_static_functions::human_readable_size(size_in_qint64 , Q_FUNC_INFO);
                }
                else if(column_name.contains("Date", Qt::CaseInsensitive))
                {
                    column_value =  global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_fs_select.value(counter).toString(), Q_FUNC_INFO);
                }
                else if(column_name == QString("hashset_name"))
                {
                    QString md5_hash_str = query_fs_select.value(counter + 1).toString();
                    ///get matched hashset names
                    QStringList matched_hashset_name_list;
                    for(int num = 0; num < hashset_db_data_list.size(); num++)
                    {
                        struct_global_hashset_db_data hashset_obj = hashset_db_data_list.at(num);
                        QString command_str = "Select " + hashset_obj.hashet_column_name + " From " + hashset_obj.hashet_table_name + " Where " + hashset_obj.hashet_column_name + " = ?";

                        QString matched_hash_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, QStringList(md5_hash_str), 0, hashset_obj.hashset_db_path, Q_FUNC_INFO);
                        matched_hash_str = matched_hash_str.trimmed();

                        if(!matched_hash_str.isEmpty())
                            matched_hashset_name_list << hashset_obj.hash_name_str;
                    }
                    column_value = matched_hashset_name_list.join(",");

                    /// hide record for hidden hashset
                    if(check_fs_record_hidden_for_hashset(hashsets_to_be_hidden, matched_hashset_name_list))
                    {
                        bool_hashset_hidden_record = true;
                        break;
                    }


                }
                else
                    column_value = query_fs_select.value(counter).toString() ;

                if(counter == 6)
                {
                    column_value = human_readable_size_str;
                }


                qry_insert.addBindValue(column_value);

                counter++;

            }

            if(!qry_insert.exec())
            {
                recon_static_functions::app_debug("insert query ----FAILED---- " + qry_insert.executedQuery(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + qry_insert.lastError().text(), Q_FUNC_INFO);
                continue;
            }

            if(bool_hashset_hidden_record)
                continue;

            S_number++;
            record_count++;

        }


        QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
        query_fs_select.exec(apple_meta_db_detach_commmand);


        filesystem_db.close();

    }

    dest_export_db.commit();
    dest_export_db.close();
    QSqlDatabase::removeDatabase(connection_naam_exp_db);

    write_source_file_data_for_export_as_csv_and_sqlite(info_file_path, struct_info, parent_category, is_recurcive, screen_item);

    display_loading_progress_bar_obj->hide();

    open_export_as_FS_csv_file(export_db_path);

}

void file_system::write_source_file_data_for_export_as_csv_and_sqlite(QString info_file_path, struct_GLOBAL_witness_info_source struct_info, QString parent_category, QString is_recurcive, QString screen_item)
{
    ///------------source info on text file
    QFile source_info_file ;
    source_info_file.setFileName(info_file_path);
    QByteArray write_data_arr;
    if(!source_info_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" File Opened ---Failed--- " + source_info_file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error " + source_info_file.errorString() , Q_FUNC_INFO);
        return ;
    }


    write_data_arr.append(QString("Source Name : ").append(struct_info.complete_source_name).append("\n").toLocal8Bit());
    if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        write_data_arr.append(QString(MACRO_Generic_Display_Name).append(struct_info.source_name).append("\n").toLocal8Bit());


    //    write_data_arr.append(QString("Evidence No. : ").append(struct_info.evidence_by_examiner).append("\n").toLocal8Bit());
    //    write_data_arr.append(QString("Description : ").append(struct_info.description_by_examiner).append("\n").toLocal8Bit());


    if(!parent_category.trimmed().isEmpty())
        write_data_arr.append(QString("Directory Name : ").append(parent_category).append("\n").toLocal8Bit());

    if(!is_recurcive.trimmed().isEmpty())
        write_data_arr.append(QString("Is Recursive : ").append(is_recurcive).append("\n").toLocal8Bit());

    if(!screen_item.trimmed().isEmpty())
        write_data_arr.append(QString("CSV Type: ").append(screen_item).append("\n").toLocal8Bit());



    source_info_file.write(write_data_arr);
    source_info_file.flush();
    source_info_file.close();
    ///------------source info on text file

}

void file_system::export_as_sqlite_for_screen_items(QSqlDatabase &dest_exp_db , struct_GLOBAL_witness_info_source struct_info, QString insert_command_str)
{
    QSqlQuery qry_insert(dest_exp_db);

    qint64 s_number = 0;
    for (int row_count = 0 ; row_count < m_file_system_tablewidget_obj->rowCount() ; row_count++)
    {

        if(row_count % 30 == 0)
            QCoreApplication::processEvents();


        if(m_file_system_tablewidget_obj->isRowHidden(row_count))
            continue ;

        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break;
        }
        s_number++;
        display_loading_progress_bar_obj->pub_set_label_messsge("Exporting SQLite... " + QString::number(s_number));

        if(struct_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_APFS || struct_info.file_system_type == MACRO_FILE_SYSTEM_TYPE_HFS)
        {

            export_as_csv_data_list.clear();
            hidden_column_header_data_index_list.clear();
            export_as_csv_data_list << QString::number(s_number)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_INODE_NO)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILENAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_EXTENSION)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILEPATH)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text()
                                    << recon_static_functions::human_readable_size(m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() , Q_FUNC_INFO)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_MIMETYPE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASHSET_NAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_MD5)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_SHA1)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DECOMPRESSION_STATUS)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_MODIFIED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CHANGE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_ACCESSED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CREATED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Date_Added)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Content_Creation_Date)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Content_Modification_Date)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Last_Used_Date)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_Use_Count)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_OCR_TEXT)->text();

            for(int i=0; i < export_as_csv_data_list.size(); i++)
            {
                for(int j=0; j < hidden_column_header_name_index_list.size(); j++)
                {
                    if(i == hidden_column_header_name_index_list.at(j))
                    {
                        hidden_column_header_data_index_list << i;
                    }
                }
            }

            for(int k = hidden_column_header_data_index_list.size()-1; k >= 0; k--)
            {
                export_as_csv_data_list.removeAt(hidden_column_header_data_index_list.at(k));
            }

            qry_insert.prepare(insert_command_str);

            for(int i = 0 ; i < export_as_csv_data_list.size() ; i++)
            {
                qry_insert.addBindValue(export_as_csv_data_list.at(i));
            }

            if(!qry_insert.exec())
            {
                recon_static_functions::app_debug("insert query 1 ----FAILED---- " + qry_insert.executedQuery(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + qry_insert.lastError().text(), Q_FUNC_INFO);
                continue;
            }
        }
        else
        {


            export_as_csv_data_list.clear();
            hidden_column_header_data_index_list.clear();
            export_as_csv_data_list << QString::number(s_number)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_INODE_NO)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILENAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_EXTENSION)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILEPATH)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text()
                                    << recon_static_functions::human_readable_size(m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_FILESIZE)->text() , Q_FUNC_INFO)
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_MIMETYPE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASHSET_NAME)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_MD5)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_HASH_SHA1)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DECOMPRESSION_STATUS)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_MODIFIED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CHANGE)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_ACCESSED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_DATE_CREATED)->text()
                                    << m_file_system_tablewidget_obj->item(row_count , enum_File_System_display_OCR_TEXT)->text();

            for(int i=0; i < export_as_csv_data_list.size(); i++)
            {
                for(int j=0; j < hidden_column_header_name_index_list.size(); j++)
                {
                    if(i == hidden_column_header_name_index_list.at(j))
                    {
                        hidden_column_header_data_index_list << i;
                    }
                }
            }
            for(int k = hidden_column_header_data_index_list.size()-1; k >= 0; k--)
            {
                export_as_csv_data_list.removeAt(hidden_column_header_data_index_list.at(k));
            }
            qry_insert.prepare(insert_command_str);

            for(int i = 0 ; i < export_as_csv_data_list.size() ; i++)
            {
                qry_insert.addBindValue(export_as_csv_data_list.at(i));
            }

            if(!qry_insert.exec())
            {
                recon_static_functions::app_debug("insert query 2 ----FAILED---- " + qry_insert.executedQuery(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + qry_insert.lastError().text(), Q_FUNC_INFO);
                continue;
            }
        }
    }
}

void file_system::change_row_background_color_for_last_used_date(qint64 current_row)
{
    if(current_row < 0)
        return;


    //QColor back_color = QString("#FFFF66"); //light yellow
    //QColor back_color = QString("#FFFF80"); //light yellow
    //QColor back_color = QString("#FFFF90"); //light yellow
    QColor back_color = QString("#FFFF99"); //light yellow

    for(int column = 0; column < m_file_system_tablewidget_obj->columnCount(); column++)
    {
        if(m_file_system_tablewidget_obj->item(current_row , column) == nullptr)
            continue;

        m_file_system_tablewidget_obj->item(current_row , column)->setBackground(back_color);
    }

}

void file_system::refresh_horizontal_header()
{
    m_file_system_tablewidget_obj->horizontalScrollBar()->setValue(m_file_system_tablewidget_obj->horizontalScrollBar()->value() + 10);
    m_file_system_tablewidget_obj->horizontalScrollBar()->setValue(m_file_system_tablewidget_obj->horizontalScrollBar()->value() - 10);
    m_file_system_tablewidget_obj->horizontalHeader()->hide();
    m_file_system_tablewidget_obj->horizontalHeader()->show();
}

void file_system::clear_all_custom_header_widget_entries()
{
    for(int ii = 0; ii < custom_header_widgets_QList.size(); ii++)
    {
        custom_header_widgets_QList.at(ii)->pub_clear_line_edit_search_field();
    }

    refresh_horizontal_header();
}
