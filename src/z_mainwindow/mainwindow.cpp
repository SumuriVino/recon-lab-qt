#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    if(getuid() == 0)
    //        bool_is_lab_running_from_root = true;
    //    else
    //        bool_is_lab_running_from_root = false;

    global_variable_macOS_appearance_light_mode_bool = get_macos_appearence_mode();
    global_variable_desktop_silicon_bool = get_macos_desktop_processor_chip_arch_type();

    bool_is_lab_running_from_root = false;

    polling_timer = new QTimer(this);
    connect(polling_timer, SIGNAL(timeout()), this, SLOT(slot_polling_timer_timeout()));

    uncloseable_wdgt = new uncloseable(this);

    bool_license_invalidation_value = false;

    bool_apple_metdatata_process_already_running = false;

    process_generic = new QProcess(this);
    connect(process_generic,SIGNAL(finished(int)),this,SLOT(slot_generic_process_finished(int)));

    image_mounter_obj = new image_mounter(this);
    connect(image_mounter_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool,QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool,QString)));
    connect(image_mounter_obj,SIGNAL(signal_set_progress_message(QString)),this,SLOT(slot_set_image_mounter_progress_message(QString)));

    set_narad_and_essentials_for_language_translater();
    fill_recon_essential_info_in_narad_muni();

    create_and_fill_mounting_path_in_narad();

    unmount_previously_mounted_images();

    //This Functions is for safe check of previous source unmounted on machine
    unmount_previous_images_according_to_mount_output();

    unlink_previous_mounted_softlinks();

    bool our_mount_path_exist_2 = global_recon_helper_singular_class_obj->is_it_mounted_path_by_contains_match(global_variable_private_var_tmp_Dir_Path_Parent_QString, Q_FUNC_INFO);
    if(!our_mount_path_exist_2)
    {
        ///===========Create Temprary directory  for files decryption-Start===========///
        QDir dir_tmp_usrname(global_variable_private_var_tmp_Dir_Path_Parent_QString);
        if(dir_tmp_usrname.exists())
        {
            recon_static_functions::do_directory_blank_completely(global_variable_private_var_tmp_Dir_Path_Parent_QString, Q_FUNC_INFO);
            dir_tmp_usrname.mkpath(QString(global_variable_private_var_tmp_Dir_Path_Case_QString));
            create_and_fill_mounting_path_in_narad();
        }
        ///===========Create Temprary directory for files decryption-End===========///
    }


    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Loading ") +  recon_static_functions::get_app_name() + global_lang_tr_obj.get_translated_string("..."));
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    setWindowFlags(Qt::Window|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

    qRegisterMetaType<struct_global_task_type_and_title>("struct_global_task_type_and_title");


    bool_launcher_work_completed = false;

    ui->actionRedefined_Result->setDisabled(true);

    QDir dir;

    license_locator_obj = new license_locator(this);
    connect(license_locator_obj, SIGNAL(signal_waapis_jao()), this, SLOT(slot_wapis_jao()));


    message_dialog_object = new message_dialog(this);
    connect(message_dialog_object,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    progress_bar_fixed_in_statusbar_obj = new progress_bar_fixed_in_statusbar(this);
    ui->statusBar->addWidget(progress_bar_fixed_in_statusbar_obj,0);
    progress_bar_fixed_in_statusbar_obj->hide();


    QString window_title = recon_static_functions::get_app_name() + "-" + recon_static_functions::get_app_version() + recon_static_functions::get_app_build_modified() + recon_static_functions::get_code_timestamp();


    //---------------------------------------------------------------//

    lic_file_not_exists = false;
    lic_integerity_failed = false;

    lic_validate_license_integerity();

    if(lic_file_not_exists)
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        license_locator_obj->set_gui_messages(global_lang_tr_obj.get_translated_string("Please click 'Browse' and locate ") + recon_static_functions::get_app_name() + global_lang_tr_obj.get_translated_string(" license file."));
        license_locator_obj->exec();

        bool_license_invalidation_value = true;
        this->close();

        if(global_variable_license_validation_value_int != 33)
            global_variable_license_validation_value_int = 32;

        return;
    }


    if(lic_integerity_failed)
    {
        lic_validate_license_integerity_newer_from_june_2019();

        if(lic_integerity_failed)
        {
            display_loading_progress_bar_non_cancelable_obj->hide();
            license_locator_obj->set_gui_messages(global_lang_tr_obj.get_translated_string("License file integrity test failed !!! Click 'Browse' to locate ") + recon_static_functions::get_app_name() + global_lang_tr_obj.get_translated_string(" license file."));
            license_locator_obj->exec();
            this->close();

            if(global_variable_license_validation_value_int != 33)
                global_variable_license_validation_value_int = 32;

            return;
        }
    }

    // Create  USB List
    licensed_usb_present = false;
    create_usb_device_list();

    lic_validate_connected_usb_device();

    if(!licensed_usb_present)
    {
        lic_validate_connected_usb_device_newer_from_june_2019();
        if(!licensed_usb_present){
            display_loading_progress_bar_non_cancelable_obj->hide();
            license_locator_obj->set_gui_messages(global_lang_tr_obj.get_translated_string("Please insert licensed ") + recon_static_functions::get_app_name() + global_lang_tr_obj.get_translated_string(" USB OR click 'Browse' to locate ") + recon_static_functions::get_app_name() + global_lang_tr_obj.get_translated_string(" license file."));
            license_locator_obj->exec();

            if(global_variable_license_validation_value_int != 33)
                global_variable_license_validation_value_int = 32;

            return;
        }
    }

    lic_create_module_hash_list();

    extract_purchase_expiry_date();

    if(bool_license_invalidation_value)
    {
        global_variable_license_validation_value_int = 32;
        return;
    }

    bool_is_international_version = false;

    check_for_international();

    check_for_demo_version();

    QString international_version_or_full;

    if(bool_is_international_version)
        international_version_or_full = global_lang_tr_obj.get_translated_string(" - International Version");


    recon_static_functions::set_recon_jaari_smay();
    recon_jaari_smay_date_numeric_qint64 = recon_static_functions::get_recon_jaari_smay();

    if(!is_lic_passed_for_valid_day_remaining())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        license_locator_obj->set_gui_messages(global_lang_tr_obj.get_translated_string("Your license has expired for this version of ") + recon_static_functions::get_app_name() + global_lang_tr_obj.get_translated_string(". Click 'Browse' to locate new license file"));
        license_locator_obj->exec();

        this->close();

        if(global_variable_license_validation_value_int != 33)
            global_variable_license_validation_value_int = 32;
        return;
    }

    network_time_mainwindow = new network_time;

    QString demo_version_string;

    if(is_it_a_demo)
    {

        demo_version_string = global_lang_tr_obj.get_translated_string(" - [Demo]");

        network_time_mainwindow->getSocialNetwork();
        while(!(network_time_mainwindow->is_test_performed()))
        {
            QCoreApplication::processEvents();
        }

        if((network_time_mainwindow->nw_value()))
        {
            display_loading_progress_bar_non_cancelable_obj->hide();
            license_locator_obj->set_gui_messages(global_lang_tr_obj.get_translated_string("Internet Connectivity is required for Demo Version."));
            license_locator_obj->pub_hide_browse_button();
            license_locator_obj->pub_set_cancel_button_text(global_lang_tr_obj.get_translated_string("OK"));
            license_locator_obj->exec();
            this->close();
            global_variable_license_validation_value_int = 32;

            return;
        }



        qint64 expiry_epch = license_expiry_date_numeric_qint64;

        if((network_time_mainwindow->return_string() < license_purchase_date_numeric_qint64) || (network_time_mainwindow->return_string() > expiry_epch))
        {
            display_loading_progress_bar_non_cancelable_obj->hide();
            license_locator_obj->set_gui_messages(global_lang_tr_obj.get_translated_string("Demo version license expired."));
            license_locator_obj->exec();
            this->close();
            global_variable_license_validation_value_int = 32;

            return;
        }

    }

    window_title = window_title + international_version_or_full + " " + demo_version_string;
    setWindowTitle(window_title);

    //---------------------------------------------------------------//



    //================================================================//

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);
    recon_helper_process_android_obj = new recon_helper_process(this);
    recon_helper_process_ios_obj = new recon_helper_process(this);

    facefinder_obj = new facefinder(this);

    unmount_previously_mounted_paths_from_home();

    //================================================================//

    activity_log_storage_obj = new activity_logs_storage(this);
    activity_log_display_obj = new activity_logs_display(this);



    socket_quicklook = new QTcpSocket(this);
    socket_quicklook->connectToHost(QHostAddress::LocalHost, 8765);
    socket_quicklook->waitForConnected(3000);




    ///- RECON interface -(START)
    main_widget = new QWidget(this);

    layout_hbox_1_main = new QHBoxLayout;
    layout_hbox_2_atch_metadata_tabs = new QHBoxLayout;
    
    layout_vbox_1_attach_casetab_and_metadatatab = new QVBoxLayout;
    
    //-Case Tree Sections

    treewidget_case_display_obj = new R_Treewidget(this);
    treewidget_case_display_obj->header()->hide();
    treewidget_case_display_obj->setMinimumWidth(200);
    treewidget_case_display_obj->setMaximumWidth(400);
    treewidget_case_display_obj->setAlternatingRowColors(false);
    treewidget_case_display_obj->setFrameStyle(QFrame::NoFrame);
    treewidget_case_display_obj->setFrameShadow(QFrame::Plain);
    treewidget_case_display_obj->setStyleSheet("QTreeWidget {"
                                               "selection-background-color: #CDCDCD;"
                                               "selection-color: black;"
                                               "}"
                                               "QTreeWidget::item {"
                                               "height: 25px;"
                                               "}"
                                               "QTreeWidget::item:selected:!active {"
                                               "color:black;"
                                               " background-color:  #CDCDCD;"

                                               "}"
                                               "QTreeWidget::indicator:checked {"
                                               "image: url(../icons/mark_item_recursively.png);"
                                               "}"
                                               "QTreeWidget::indicator:unchecked {"
                                               "image: url(../icons/unmark_item_recursively.png);"
                                               "}"
                                               );
    
    treewidget_case_display_obj->setExpandsOnDoubleClick(true);
    treewidget_case_display_obj->setAnimated(false);
    stack_widget_cases_plugins_and_global_items = new m_stackedwidget(this);
    
    //textedit_metadata = new m_textedit(this);
    textedit_detailed_information = new QPlainTextEdit(this);
    textedit_detailed_information->setReadOnly(true);
    textedit_detailed_information->setMinimumHeight(40);

    textedit_optical_character_recognition_information = new QPlainTextEdit(this);
    textedit_optical_character_recognition_information->setReadOnly(true);
    textedit_optical_character_recognition_information->setMinimumHeight(40);

    ///Extended Attributes tab-------------
    tree_detailed_info_apple_metadata = new m_treewidget(this);
    tree_detailed_info_apple_metadata->setColumnCount(6);

    QTreeWidgetItem* header_tree = tree_detailed_info_apple_metadata->headerItem();
    header_tree->setText(enum_mainwindow_apple_metadata_KEY, "Attribute");
    header_tree->setText(enum_mainwindow_apple_metadata_Value, "Value");
    header_tree->setText(enum_mainwindow_apple_metadata_Plugin_Table_Name, "Plugin Table Name");
    header_tree->setText(enum_mainwindow_apple_metadata_Plugin_Db_Path, "Plugin Db Path");
    header_tree->setText(enum_mainwindow_apple_metadata_Plugin_Record_no, "Record no");

    tree_detailed_info_apple_metadata->header()->resizeSection(0, 220);

    connect(tree_detailed_info_apple_metadata, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slot_tree_apple_metadata_item_clicked(QTreeWidgetItem*,int)));
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Table_Name,true);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Db_Path,true);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Record_no,true);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_File_Path,true);

    ///----------

    system_password_asker_obj = new system_password_asker(this);
    connect(system_password_asker_obj, SIGNAL(signal_system_password_asker_workdone(bool,QString)), this, SLOT(slot_system_password_asker_workdone(bool,QString)));


    ///hex viewer
    hex_displayer_metadata_obj = new hex_viewer_complete(this);
    hex_displayer_metadata_obj->pub_downsize_hexviewer_for_metadata_display();
    connect(hex_displayer_metadata_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    //    hex_viewer_metadata_obj = new hex_viewer(this);
    //    hex_viewer_metadata_obj->pub_display_hex_viewer_metadata();
    //    // hex_viewer_metadata_obj->create_ui_full();
    //    hex_viewer_metadata_obj->setMinimumHeight(40);
    //    hex_viewer_metadata_obj->setStyleSheet("QTextEdit{background:#f1f1f1;}");

    //    hex_viewer_metadata_obj->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    //    connect(hex_viewer_metadata_obj,SIGNAL(signal_insert_tag_detail_into_tag_search_db(QString, QString, uint64_t, uint64_t, QString, QString, QString,QString)), this, SLOT(slot_insert_tag_detail_into_tag_search_db_from_hex_viewer(QString, QString, uint64_t, uint64_t, QString, QString, QString,QString)));
    //    connect(hex_viewer_metadata_obj,SIGNAL(signal_remove_tag_entry_from_tag_search_db_and_display(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    //    connect(hex_viewer_metadata_obj,SIGNAL(signal_remove_tag_entry_from_notes_db_and_display(QString,QString,QString,QString,QString)),this,SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));

    //    scroll_area_for_hex = new QScrollArea(this);
    //    scroll_area_for_hex->setWidget(hex_viewer_metadata_obj);
    //    scroll_area_for_hex->setWidgetResizable(true);

    //    text_viewer_metadata_obj = new text_viewer(this);
    //    text_viewer_metadata_obj->setMinimumHeight(40);
    //    text_viewer_metadata_obj->setStyleSheet("QTextEdit{background:#f1f1f1;}");
    //    text_viewer_metadata_obj->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    //  connect(text_viewer_metadata_obj,SIGNAL(signal_insert_tag_detail_into_tag_search_db(QString, QString, uint64_t, uint64_t, QString, QString, QString)), this, SLOT(slot_insert_tag_detail_into_tag_search_db_from_text_viewer(QString, QString, uint64_t, uint64_t, QString, QString, QString)));

    text_viewer_aascii_unicode_obj = new text_viewer_aascii_unicode(this);



    strings_viewer_obj = new strings_viewer(this);
    strings_viewer_obj->pub_set_global_manager_obj(global_connection_manager_obj);

    tree_exif_metadata_display = new m_treewidget(this);
    tree_exif_metadata_display->setColumnCount(5);
    tree_exif_metadata_display->header()->resizeSection(0, 220);
    connect(tree_exif_metadata_display, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slot_tree_exif_metadata_item_clicked(QTreeWidgetItem*,int)));

    recon_static_functions::app_debug("offline maps meta object reached 000", Q_FUNC_INFO);

    maps_metadata_obj = new offline_maps(this);
    recon_static_functions::app_debug("offline maps meta object reached 111", Q_FUNC_INFO);
    maps_metadata_obj->setMinimumHeight(40);
    recon_static_functions::app_debug("offline maps meta object reached 222", Q_FUNC_INFO);
    maps_metadata_obj->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    recon_static_functions::app_debug("offline maps meta object reached 333", Q_FUNC_INFO);
    connect(maps_metadata_obj, SIGNAL(signal_add_saved_maps_in_case_tree()), this, SLOT(slot_add_saved_maps_in_case_tree()));
    connect(maps_metadata_obj, SIGNAL(signal_display_saved_maps(QString)), this, SLOT(slot_display_for_saved_maps(QString)));

    recon_static_functions::app_debug("offline maps meta object crossed ", Q_FUNC_INFO);

    stackwidget_previewer = new m_stackedwidget(this);
    blank_object = new blank(this);
    av_preview_object = new av_preview(this);
    image_preview_object = new image_preview(this);
    
    stackwidget_previewer->addWidget(blank_object);
    stackwidget_previewer->addWidget(image_preview_object);
    stackwidget_previewer->addWidget(av_preview_object);
    
    stackwidget_previewer->setCurrentIndex(0);
    
    tabwidget_av_and_images_previewer = new R_Tabwidget(this);
    //    tabwidget_av_and_images_previewer->setStyleSheet("QTabBar::tab { max-height: 8px; min-height: 8px; color:#EAEAEA; font:  11pt; color:#404040; }"
    //                                                     " QTabBar::tab:hover { color:#000000; } QTabBar::tab:selected { color:#000000;  }" );

    tabwidget_metadata_viewer = new R_Tabwidget(this);
    //    tabwidget_metadata_viewer->setStyleSheet("QTabBar::tab { max-height: 8px; min-height: 8px; color:#EAEAEA; font:  11pt; color:#404040; }"
    //                                             " QTabBar::tab:hover { color:#000000; } QTabBar::tab:selected { color:#000000;  }" );



    QCoreApplication::processEvents();

    draw_complete_gui();
    this->setCentralWidget(main_widget);
    QCoreApplication::processEvents();


    action_add_source = ui->mainToolBar->addAction(QIcon("../icons/toolbar/source.png"), ("Add Source"));
    connect(action_add_source, SIGNAL(triggered()), this, SLOT(slot_action_add_source_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    bool_text_indexing_running = false;

    //    action_run_plugins = ui->mainToolBar->addAction(QIcon("../icons/toolbar/Artifacts.png"), QString("Run Artifacts"));
    //    connect(action_run_plugins, SIGNAL(triggered()), this, SLOT(slot_action_run_plugin_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_ram_analysis = ui->mainToolBar->addAction(QIcon("../icons/toolbar/ram_analysis.png"),QString("RAM Analysis"));
    //    connect(action_ram_analysis, SIGNAL(triggered()), this, SLOT(slot_action_ram_analysis_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);


    //    ui->mainToolBar->addSeparator();


    //    action_artifacts_timeline_full = ui->mainToolBar->addAction(QIcon("../icons/toolbar/timeline_full.png"), QString(MACRO_JobType_Artifacts_Timeline));
    //    connect(action_artifacts_timeline_full, SIGNAL(triggered()), this, SLOT(slot_action_artifacts_timeline_full_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_global_report = ui->mainToolBar->addAction(QIcon("../icons/toolbar/global_report.png"), QString("Global Report"));
    //    connect(action_global_report, SIGNAL(triggered()), this, SLOT(slot_action_global_report_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_export = ui->mainToolBar->addAction(QIcon("../icons/toolbar/fs_export.png"),QString("Tagged File Export"));
    //    connect(action_export, SIGNAL(triggered()), this, SLOT(slot_global_tagged_file_export_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_keyword_search = ui->mainToolBar->addAction(QIcon("../icons/toolbar/artifacts_keyword_search.png"), QString(MACRO_JobType_Artifacts_Keyword_Search));
    //    connect(action_keyword_search, SIGNAL(triggered()), this, SLOT(slot_action_keyword_search_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_content_search = ui->mainToolBar->addAction(QIcon("../icons/toolbar/content_search.png"),QString("Content Search"));
    //    connect(action_content_search, SIGNAL(triggered()), this, SLOT(slot_action_content_search_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_file_search_regular_filters = ui->mainToolBar->addAction(QIcon("../icons/toolbar/file_search.png"),QString(MACRO_CASE_TREE_FILE_SEARCH));
    //    connect(action_file_search_regular_filters, SIGNAL(triggered()), this, SLOT(slot_action_file_search_regular_filters_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_file_search_apple_metadata_filters = ui->mainToolBar->addAction(QIcon("../icons/toolbar/apple_metadata.png"),QString("Apple Metadata Search"));
    //    connect(action_file_search_apple_metadata_filters, SIGNAL(triggered()), this, SLOT(slot_action_file_apple_metadata_filters_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_file_search_exif_metadata_filters = ui->mainToolBar->addAction(QIcon("../icons/toolbar/exif_metadata.png"),QString(MACRO_CASE_TREE_EXIF_METADATA_SEARCH));
    //    connect(action_file_search_exif_metadata_filters,SIGNAL(triggered()), this, SLOT(slot_action_exif_metadata_filters_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_recognize_face = ui->mainToolBar->addAction(QIcon("../icons/toolbar/recognize_face.png"),QString("Recognize Face"));
    //    connect(action_recognize_face,SIGNAL(triggered()), this, SLOT(slot_action_recognize_face_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    ui->mainToolBar->addSeparator();

    //    action_text_indexing = ui->mainToolBar->addAction(QIcon("../icons/toolbar/text_indexing.png"),QString("Text Indexing"));
    //    connect(action_text_indexing, SIGNAL(triggered()), this, SLOT(slot_action_text_indexing_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);


    file_indexing_display_obj = new file_indexing_display();
    connect(file_indexing_display_obj, SIGNAL(signal_done_clicked()), this, SLOT(slot_text_indexing_done_clicked()));


    //    action_super_timeline = ui->mainToolBar->addAction(QIcon("../icons/toolbar/super_timeline.png"), QString("Super Timeline"));
    //    connect(action_super_timeline, SIGNAL(triggered()), this, SLOT(slot_action_super_timeline_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    action_file_system_post_launcher = ui->mainToolBar->addAction(QIcon("../icons/toolbar/processing_status.png"),QString("Processing Status"));
    connect(action_file_system_post_launcher, SIGNAL(triggered()), this, SLOT(slot_action_file_system_post_launcher_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);


    action_recon_configuration_after_launch = ui->mainToolBar->addAction(QIcon("../icons/toolbar/configuration.png"),QString("Configuration"));
    connect(action_recon_configuration_after_launch, SIGNAL(triggered()), this, SLOT(slot_action_recon_configuration_after_launcher_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);


    hashset_display_toolbar_obj = new hashset_display_toolbar(this);
    connect(hashset_display_toolbar_obj, SIGNAL(signal_hashset_toolbar_apply_clicked(QStringList)), this, SLOT(slot_hashset_toolbar_apply_clicked(QStringList)));

    //    action_hashset_from_toolbar = ui->mainToolBar->addAction(QIcon("../icons/toolbar/hashset.png"),QString(MACRO_RECON_CONFIGURATION_HASHSET));
    //    connect(action_hashset_from_toolbar, SIGNAL(triggered()), this, SLOT(slot_action_hashset_from_toolbar_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    export_case_obj = new export_case(this);

    //    action_export_case = ui->mainToolBar->addAction(QIcon("../icons/toolbar/export_case.png"),QString(MACRO_Export_Case_Parent));
    //    connect(action_export_case, SIGNAL(triggered()), this, SLOT(slot_action_export_case_from_toolbar_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    snapshots_obj = new snapshots(this);
    connect(snapshots_obj, SIGNAL(signal_add_snapshot_in_case_tree(QString)), this, SLOT(slot_add_snapshot_in_case_tree(QString)));
    connect(snapshots_obj, SIGNAL(signal_go_to_snapshots_display(QString)), this, SLOT(slot_snapshot_display(QString)));


    //====================================================================================================//
    ui->mainToolBar->addAction(("")); //To maintain toolbar gap space.
    ui->mainToolBar->addAction(("")); //To maintain toolbar gap space.

    QWidget *spacerWidget_1 = new QWidget(this);
    spacerWidget_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget_1->setVisible(true);
    ui->mainToolBar->addWidget(spacerWidget_1);

    action_global_report = ui->mainToolBar->addAction(QIcon("../icons/toolbar/global_report.png"), QString("Global Report"));
    connect(action_global_report, SIGNAL(triggered()), this, SLOT(slot_action_global_report_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    action_story_board = ui->mainToolBar->addAction(QIcon("../icons/toolbar/story_board.png"), QString("Story Board"));
    connect(action_story_board, SIGNAL(triggered()), this, SLOT(slot_action_story_board_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //====================================================================================================//
    ///
    /// - ScreenShot & Quick look code added in the last of the all Actions Code.
    ///

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    ui->mainToolBar->addWidget(spacerWidget);

    examiner_space_obj = new examiner_space(this);
    action_examiner_space = ui->mainToolBar->addAction(QIcon("../icons/toolbar/examiner_space.png"),QString("Examiner Space"));
    connect(action_examiner_space, SIGNAL(triggered()), this, SLOT(slot_action_examiner_space_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    action_screenshot = ui->mainToolBar->addAction(QIcon("../icons/toolbar/screenshot.png"), QString("Screenshot"));
    connect(action_screenshot, SIGNAL(triggered()), this, SLOT(slot_action_screenshot_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    action_screenshot->setDisabled(false);

    action_quick_look = ui->mainToolBar->addAction(QIcon("../icons/toolbar/quicklook.png"), QString("Quick Look"));
    connect(action_quick_look, SIGNAL(triggered()), this, SLOT(slot_action_quick_look_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //    action_activity_logs = ui->mainToolBar->addAction(QIcon("../icons/toolbar/logs.png"), QString("Activity Logs"));
    //    connect(action_activity_logs, SIGNAL(triggered()), this, SLOT(slot_action_activity_logs_triggered()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);


    //    action_story_board = ui->mainToolBar->addAction(QIcon("../icons/toolbar/story_board.png"), QString("Story Board"));
    //    connect(action_story_board, SIGNAL(triggered()), this, SLOT(slot_action_story_board_triggered()));
    //    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    action_show_case_tree_sidebar = ui->mainToolBar->addAction(QIcon("../icons/toolbar/sidebar.png"), QString("Show Sidebar"));
    connect(action_show_case_tree_sidebar, SIGNAL(triggered()), this, SLOT(slot_action_show_case_tree_sidebar()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);


    action_show_detailed_information = ui->mainToolBar->addAction(QIcon("../icons/toolbar/detailed_info.png"), QString("Show Detailed Information"));
    connect(action_show_detailed_information, SIGNAL(triggered()), this, SLOT(slot_action_show_detailed_information()));
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //====================================================================================================//


    //fill_recon_essential_info_in_narad_muni();
    update_examiner_machine_recon_library();

    //====================== start =======================
    ///
    /// Do not change calling order
    /// create_recon_library_structure is using db created in
    /// create_feature_resources_structure fun
    ///
    QCoreApplication::processEvents();

    create_feature_resources_structure();

    QCoreApplication::processEvents();
    create_recon_library_structure();
    QCoreApplication::processEvents();

    //====================  end  =========================

    // new_case_wizard_object = new new_case_wizard(this);
    
    connect(treewidget_case_display_obj,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_treewidget_case_display_single_clicked(QTreeWidgetItem*,int)));
    connect(treewidget_case_display_obj,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slot_treewidget_case_display_double_clicked(QTreeWidgetItem*,int)));
    connect(treewidget_case_display_obj,SIGNAL(itemExpanded(QTreeWidgetItem*)),this,SLOT(slot_treewidget_case_display_expanded(QTreeWidgetItem*)));
    connect(treewidget_case_display_obj,SIGNAL(signal_tree_close_case()),this,SLOT(slot_tree_right_click_close_case()),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_tree_add_source()),this,SLOT(slot_tree_right_click_add_source()),Qt::QueuedConnection);

    task_status_window_obj = new task_status_window(this);
    task_status_window_obj->setGeometry(this->pos().x() + 100, this->pos().y() + this->height() - 180, 360, 203);
    task_status_window_obj->move(QPoint(this->pos().x() + this->width() - 243, this->pos().y() + 7));
    
    connect(task_status_window_obj,SIGNAL(signal_current_item_for_statusbar(QString)),this,SLOT(slot_update_status_bar_message(QString)));
    connect(this,SIGNAL(signal_statusbar_button_checked(bool)),task_status_window_obj,SLOT(slot_mainwindow_statusbar_button_clicked(bool)));

    connect(snapshots_obj, SIGNAL(signal_PBAR_snapshot_fs_extraction_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(snapshots_obj, SIGNAL(signal_PBAR_snapshot_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(snapshots_obj, SIGNAL(signal_PBAR_snapshot_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    pushbutton_statusbar = new QPushButton(this);
    pushbutton_statusbar->setFlat(true);
    
    label_status_bar = new QLabel(this);

    pushbutton_statusbar->setToolTip(global_lang_tr_obj.get_translated_string("Show Statusbar"));
    set_right_cornered_status_window();
    connect(pushbutton_statusbar,SIGNAL(clicked(bool)),this,SLOT(slot_pushbutton_status_right_corner_clicked(bool)),Qt::QueuedConnection);
    

    case_info_details_obj = new case_info_details(this);
    
    load_case_obj = new load_case(this);
    
    connect(this,SIGNAL(signal_load_case_clicked()),load_case_obj,SLOT(slot_load_case_button_clicked()),Qt::QueuedConnection);
    connect(load_case_obj,SIGNAL(signal_loadcase_launched(QString)),this,SLOT(slot_loadcase_launched(QString)),Qt::QueuedConnection);
    

    map_ios_uid_backup_parser_obj = new QMap<QString, ios_uid_backup_parser*>;
    
    // please do not delete this
    QString path = recon_static_functions::get_appilcation_relative_path() + "/Resources/tmp/";
    dir.mkpath(path);
    
    
    show_metadata_widget(false);

    case_wizard_object = new case_wizard(this);
    // connect(case_wizard_object,SIGNAL(signal_case_launched(QString,QList<struct_GLOBAL_derived_source_info>)),this,SLOT(slot_case_launched(QString,QList<struct_GLOBAL_derived_source_info>)));
    connect(case_wizard_object,SIGNAL(signal_case_launched(QString,QList<struct_GLOBAL_witness_info_root>)),this,SLOT(slot_case_launched(QString,QList<struct_GLOBAL_witness_info_root>)));

    case_wizard_object->pub_set_lic_info(customer_name,license_expiry_date_numeric_qint64);
    case_wizard_object->pub_set_qml_on_first_main_page();

    disk_manager_obj = case_wizard_object->pub_get_disk_manager_object();
    disk_manager_obj->pub_stop_disk_arbitration();



    //-Load Case From Launcher
    connect(case_wizard_object,SIGNAL(signal_load_case_clicked(QString)),this,SLOT(slot_launcher_load_Case_clicked(QString)));
    connect(this,SIGNAL(signal_load_case_from_launcher_clicked(QString)),load_case_obj,SLOT(slot_load_case_from_launcher_clicked(QString)),Qt::QueuedConnection);
    
    
    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;



    super_timeline_obj = new super_timeline(this);
    connect(super_timeline_obj, SIGNAL(signal_super_timeline_done(bool)), this, SLOT(slot_super_timeline_done(bool)));

    bool_main_launcher_call = false;
    bool_metadata_show = true;
    

    bool_insert_data_in_stack = false;
    center_stackwidget_manager_obj = new center_stackwidget_manager(this);

    add_tags_module_obj = new add_tags(this);
    detach_window_manager_obj = new detach_window_manager(this);

    bool_update_navigation_list = false;
    bool_combobox_items_added = false;
    center_next_previous_navigation_obj = new center_next_previous_navigation(this);
    connect(this,SIGNAL(signal_insert_new_entry_in_navigation_list(QString)),center_next_previous_navigation_obj,SLOT(slot_insert_new_entry_in_navigation_list(QString)));
    connect(this,SIGNAL(signal_set_center_next_previous_navigation_list_page_unique_name(QString, QString)),center_next_previous_navigation_obj,SLOT(slot_set_center_next_previous_navigation_list_page_unique_name(QString, QString)));
    connect(this,SIGNAL(signal_update_navigation_list_on_combobox_change(QString)),center_next_previous_navigation_obj,SLOT(slot_update_navigation_list_on_combobox_change(QString)));
    connect(this,SIGNAL(signal_remove_navigation_list_entry_on_combobox_pusbutton_close(QString)),center_next_previous_navigation_obj,SLOT(slot_remove_navigation_list_entry_on_combobox_pusbutton_close(QString)));
    connect(center_next_previous_navigation_obj,SIGNAL(signal_set_case_tree_item_bold_on_navigation_button_click(QString)),this,SLOT(slot_set_case_tree_item_bold_on_navigation_click(QString)));



    quick_look_obj = new quick_look(this);
    quick_look_obj->show();
    quick_look_obj->pub_set_preview_closed_status(true);


    bool_quick_look_preview_clicked = false;
    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;

    //connect(ui->menuView, SIGNAL(aboutToShow()), this, SLOT(slot_aboutToShow_menubar_view()));
    ui->actionPlugin_Summary->setEnabled(false);


    bool_case_tree_combobox_filling = false;

    load_case_source_displayer_obj = new load_case_source_displayer(this);
    connect(load_case_source_displayer_obj,SIGNAL(signal_load_case_set_sources_path(QList<struct_GLOBAL_witness_info_source>, bool)), this, SLOT(slot_load_case_set_sources_path(QList<struct_GLOBAL_witness_info_source>, bool)));

    bool_display_load_case_page_ok_clicked = false;
    connect(this,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);

    prc_generic_without_argument = new QProcess(this);
    connect(prc_generic_without_argument, SIGNAL(finished(int)), this, SLOT(slot_prc_generic_without_argument_finished(int)));

    screenshots_obj = new screenshots(this);
    connect(screenshots_obj, SIGNAL(signal_screenshots_work_complete(QString,QString,QString)), this, SLOT(slot_screenshots_work_complete(QString,QString,QString)));
    connect(screenshots_obj, SIGNAL(signal_screenshots_canceled()), this, SLOT(slot_screenshots_canceled()));

    exif_interface_obj = new exif_data_interface(this);

    /// To give focus to a widget
    // setFocus(Qt::ActiveWindowFocusReason);

    content_search_obj = new content_search(this);
    content_search_obj->hide();
    connect(content_search_obj, SIGNAL(signal_content_search_pushbutton_done_clicked(QString)),this,SLOT(slot_content_search_done_clicked(QString)));


    pushbutton_statusbar->setChecked(true);

    file_search_interface_regular_filters_obj = new file_search_interface(this);
    connect(file_search_interface_regular_filters_obj, SIGNAL(signal_start_file_search_thread(struct_global_selected_regular_filters_info, QString, QString, bool, QString, QString)), this, SLOT(slot_start_file_search_thread(struct_global_selected_regular_filters_info, QString, QString, bool, QString, QString)));

    file_search_regular_filters_obj = new file_search_regular_filters(this);
    connect(file_search_regular_filters_obj, SIGNAL(signal_file_search_regular_filter_work_done(QString,QStringList,QString)), this, SLOT(slot_file_search_regular_filter_work_done(QString,QStringList,QString)));

    thread_file_search_regular_filters_obj = new thread_file_search_regular_filters(this);
    connect(thread_file_search_regular_filters_obj, SIGNAL(finished()), this, SLOT(slot_thread_file_search_regular_filter_finish()));

    file_search_regular_filters_progress_bar_obj = new display_loading_progress_bar(this);
    connect(file_search_regular_filters_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_file_search_regular_filters_display(bool)));

    file_search_interface_apple_metadata_filters_obj = new file_search_interface(this);
    connect(file_search_interface_apple_metadata_filters_obj, SIGNAL(signal_start_file_search_thread(struct_global_selected_regular_filters_info, QString, QString, bool, QString, QString)), this, SLOT(slot_start_file_search_thread(struct_global_selected_regular_filters_info, QString, QString, bool, QString, QString)));

    file_system_post_launcher_obj = new file_system_post_launcher(this);
    connect(file_system_post_launcher_obj, SIGNAL(signal_file_system_post_launcher_job_list(struct_global_file_system_post_launcher_job_info)), this, SLOT(slot_file_system_post_launcher_job_list(struct_global_file_system_post_launcher_job_info)));
    connect(file_system_post_launcher_obj, SIGNAL(signal_verifying_source_start(QString)), this, SLOT(slot_verifying_source_start(QString)),Qt::QueuedConnection);
    connect(file_system_post_launcher_obj, SIGNAL(signal_PBAR_source_verifying_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(file_system_post_launcher_obj, SIGNAL(signal_source_verfication_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)), Qt::QueuedConnection);
    connect(file_system_post_launcher_obj, SIGNAL(signal_set_progress_bar_max_value(struct_global_task_type_and_title, int)), task_status_window_obj, SLOT(slot_set_progress_bar_max_value(struct_global_task_type_and_title, int)),Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_source_verification(struct_global_task_type_and_title)), file_system_post_launcher_obj, SIGNAL(signal_cancel_source_verfication(struct_global_task_type_and_title)));


    exif_metadata_filters_obj = new exif_metadata_filters(this);
    connect(exif_metadata_filters_obj, SIGNAL(signal_exif_metadata_filter_work_done(QString,QString,QStringList,QString)),this,SLOT(slot_exif_metadata_filter_work_done(QString,QString,QStringList,QString)));

    thread_exif_metadata_search_obj = new thread_exif_metadata_search(this);
    connect(thread_exif_metadata_search_obj, SIGNAL(finished()), this, SLOT(slot_thread_exif_metadata_filter_finish()));


    connect(thread_exif_metadata_search_obj, SIGNAL(signal_PBAR_thread_exif_metadata_search_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_exif_metadata_search_obj, SIGNAL(signal_PBAR_thread_exif_metadata_search_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_exif_metadata_search_obj, SIGNAL(signal_PBAR_thread_exif_metadata_search_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));


    recon_configuration_after_launch_obj = new recon_configuration(this);

    create_dialog_for_storyboard_file_save();

    check_and_update_services_db_for_recon_lab();

    setUnifiedTitleAndToolBarOnMac(false);

    ui->mainToolBar->raise();

    connect(this, SIGNAL(signal_lic_label_text(QString,qint64,qint64)), case_wizard_object, SLOT(slot_set_license_label_text(QString,qint64,qint64)));
    emit signal_lic_label_text(customer_name, license_purchase_date_numeric_qint64, license_expiry_date_numeric_qint64);

    initial_warning_message_obj = new initial_warning_message(case_wizard_object);


    case_wizard_object->pub_set_window_title(window_title);

    recognize_face_obj = new recognize_face(this);

    create_and_fill_mounting_path_in_narad();

    set_up_global_connection_manager();
    set_up_global_recon_file_info();

    bool_loading_result_operation_running = false;
    bool_need_to_update_result_for_load_case = false;

    tagged_file_export_obj = new tagged_file_export(this);

    display_loading_progress_bar_non_cancelable_obj->hide();

    witness_info_store_obj = new witness_info_store(this);

    root_count_name_list_to_be_process.clear();
    source_count_name_list_to_be_process.clear();

    show_launcher(); // Dont change this line's position.

    //================== Fill Examiner Info after show_launcher() =================//
    //================== becoz password info will be available only after show_launcher() ==============//
    global_desktop_info_class_obj->pub_fill_desktop_info_general(Q_FUNC_INFO);
    struct_global_desktop_info examiner_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
    recon_static_functions::print_examiner_info(examiner_info_obj, Q_FUNC_INFO);
    recon_static_functions::print_examiner_info_for_sumuri(examiner_info_obj, Q_FUNC_INFO);

    polling_timer->start(2000);

}

MainWindow::~MainWindow()
{
    if(bool_license_invalidation_value || global_variable_license_validation_value_int == 44)
        return;

    delete_entries_from_hex_viewer_tmp(hex_viewer_tmp_db_path);

    delete_entries_from_plist_viewer_tmp_db();

    delete_entries_from_mounted_image_db();

    if(center_stackwidget_manager_obj != NULL)
        delete center_stackwidget_manager_obj;

    if(map_ios_uid_backup_parser_obj != NULL)
        delete map_ios_uid_backup_parser_obj;

    if(file_indexing_display_obj != NULL)
        delete file_indexing_display_obj;

    if(recognize_face_obj != NULL)
        delete recognize_face_obj;

    recon_static_functions::close_debug_file();


    system("killall recon_ql_preview_server");

    delete ui;

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(bool_license_invalidation_value || global_variable_license_validation_value_int == 44)
        return;

    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(true);
    progress_bar_fixed_in_statusbar_obj->set_status_message("Please wait...");
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(70);
    progress_bar_fixed_in_statusbar_obj->show();

    stop_rcase_all_threads();

    disk_manager_obj->pub_stop_disk_arbitration();


    QList<struct_GLOBAL_witness_info_source> all_sources_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();


    snapshots_obj->pub_unmount_all_snapshots();

    ///=========Unlink softmount link for Home dir source start========///
    for(int count = 0; count < all_sources_list.size(); count++)
    {
        QString src_type_intrnl = all_sources_list.at(count).source_type_internal;
        if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
        {
            QString virtual_source_path = all_sources_list.at(count).virtual_source_path;

            if(!virtual_source_path.trimmed().isEmpty() && virtual_source_path.endsWith("/"))
                virtual_source_path.chop(1);

            QStringList arg;
            arg << virtual_source_path;
            run_command_generic("unlink",arg);
        }
    }
    ///=========Unlink softmount link for Home dir source end========///

    unmount_previously_mounted_paths_from_home();
    image_mounter_obj->unmount_all_image();
    image_mounter_obj->unmount_all_fuse();

    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(90);


    recon_static_functions::do_directory_blank_completely(global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString).toString(), Q_FUNC_INFO);
    recon_static_functions::do_directory_blank_completely(global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString(), Q_FUNC_INFO);
    recon_static_functions::do_directory_blank_completely(global_variable_private_var_tmp_Dir_Path_Case_QString ,Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(global_variable_private_var_tmp_Dir_Path_Case_QString ,Q_FUNC_INFO);


    QString result_config_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString();
    recon_static_functions::do_directory_blank_completely(result_config_path, Q_FUNC_INFO);


    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(false);
    progress_bar_fixed_in_statusbar_obj->hide();

    quick_look_obj->pub_close_view();
    QApplication::exit(0);

}


void MainWindow::show_launcher()
{
    global_desktop_info_class_obj->pub_update_desktop_password(true, Q_FUNC_INFO);

    struct_global_desktop_info pwdobj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    // check system paswword is changed or not
    if(!global_recon_helper_singular_class_obj->is_system_pwd_accurate(pwdobj.desktop_password_original, Q_FUNC_INFO))
    {
        global_desktop_info_class_obj->pub_update_desktop_password(false, Q_FUNC_INFO);
        system_password_asker_obj->show();
    }
    else
        show_case_wizard();

}


void MainWindow::slot_system_password_asker_workdone(bool pwd_status, QString received_pwd)
{
    system_password_asker_obj->hide();

    if(pwd_status)
    {
        QString file_path_org = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString).toString() + "confidential_original.txt";
        recon_static_functions::overwrite_data_into_file(file_path_org, received_pwd, Q_FUNC_INFO);
        recon_static_functions::encrypt_file_1(file_path_org, Q_FUNC_INFO);

        global_desktop_info_class_obj->pub_update_desktop_password(true, Q_FUNC_INFO);

    }
    else
    {
        global_desktop_info_class_obj->pub_update_desktop_password(false, Q_FUNC_INFO);
    }

    show_case_wizard();

}

void MainWindow::slot_wapis_jao()
{
    bool_license_invalidation_value = true;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    task_status_window_obj->move(QPoint(this->width() - 363, this->height() - 227));
}

void MainWindow::delete_entries_from_mounted_image_db()
{
    QString mounted_image_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "mounted_images/mounted_images_info.sqlite";

    QString command = "DELETE FROM images_info";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,mounted_image_db_path,Q_FUNC_INFO);

    command = "DELETE FROM partition_info";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,mounted_image_db_path,Q_FUNC_INFO);
}

void MainWindow::unmount_previously_mounted_images()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    //=====================Unmount previously TSK FUSE start ====================//
    //    QDirIterator miter(global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString(), QDirIterator::NoIteratorFlags);
    //    while(miter.hasNext())
    //    {
    //        QFileInfo minfo(miter.next());

    //        QString file_name =  minfo.fileName();
    //        QString tsk_path =  minfo.filePath();
    //        if(file_name == (".") || file_name == (".."))
    //            continue;

    //        if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(tsk_path, Q_FUNC_INFO))
    //        {
    //            recon_static_functions::do_directory_blank_completely(tsk_path, Q_FUNC_INFO);
    //            recon_static_functions::remove_safely_blank_dir_OR_file(tsk_path,Q_FUNC_INFO);
    //            continue;
    //        }

    //        QStringList arg;
    //        arg << "umount" << tsk_path;
    //        run_command_generic("diskutil",arg);

    //        // to check mount path is mounted or not
    //        if(global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(tsk_path, Q_FUNC_INFO))
    //            continue;

    //        recon_static_functions::do_directory_blank_completely(tsk_path, Q_FUNC_INFO);
    //        recon_static_functions::remove_safely_blank_dir_OR_file(tsk_path,Q_FUNC_INFO);

    //    }
    //=====================Unmount previously TSK FUSE end ====================//

    //=====================Unmount apfs snapshots start  ====================//

    // snapshots_obj->pub_unmount_all_snapshots();


    QString mounted_image_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "mounted_images/mounted_images_info.sqlite";

    if(!QFile(mounted_image_db_path).exists())
    {
        return;
    }
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(mounted_image_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + mounted_image_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select(destination_db);
    QSqlQuery query_select_partition_info(destination_db);

    query_select.prepare("SELECT COUNT (*) FROM 'partition_info' WHERE mount_path IS NOT NULL AND mount_path is NOT ''");
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query_select  --------FAILED------ db file - " + mounted_image_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query_select.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_select.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
    }

    int count = 0;
    if(query_select.next())
    {
        count = query_select.value(0).toInt();
    }

    if(count <= 0)
    {
        destination_db.close();
        recon_static_functions::app_debug(" end ===",Q_FUNC_INFO);
        return;
    }

    //    QString message = "Please wait unmounting previously mounted images...";
    //    case_wizard_object->pub_set_message_in_label_for_unmounting_and_enable_disable_ui(message, false);

    query_select.prepare("SELECT image_path_1, image_name_1, shadow_file_path_1, disk_node_1, disk_identifier_1,"
                         "osx_fuse_mount_point_1, image_path_2, image_name_2, shadow_file_path_2, disk_node_2, disk_identifier_2,"
                         "osx_fuse_mount_point_2, image_format_type, image_password, image_source_type_display,image_source_type_internal INT FROM images_info");

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query_select  --------FAILED------ db file - " + mounted_image_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query_select.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_select.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
    }

    QList<struct_global_image_mounter_image_info> list_struct_image_info;
    while(query_select.next())
    {
        struct_global_image_mounter_image_info struct_img_info;
        recon_static_functions::clear_variables_image_mounting_image_info(struct_img_info);

        struct_img_info.image_path_1            = query_select.value("image_path_1").toString();
        struct_img_info.image_name_1            = query_select.value("image_name_1").toString();
        struct_img_info.shadow_file_path_1      = query_select.value("shadow_file_path_1").toString();
        struct_img_info.disk_node_1             = query_select.value("disk_node_1").toString();
        struct_img_info.disk_identifier_1       = query_select.value("disk_identifier_1").toString();
        struct_img_info.osx_fuse_mount_point_1  = query_select.value("osx_fuse_mount_point_1").toString();

        struct_img_info.image_path_2            = query_select.value("image_path_2").toString();
        struct_img_info.image_name_2            = query_select.value("image_name_2").toString();
        struct_img_info.shadow_file_path_2      = query_select.value("shadow_file_path_2").toString();
        struct_img_info.disk_node_2             = query_select.value("disk_node_2").toString();
        struct_img_info.disk_identifier_2       = query_select.value("disk_identifier_2").toString();
        struct_img_info.osx_fuse_mount_point_2  = query_select.value("osx_fuse_mount_point_2").toString();

        struct_img_info.image_format_type              = query_select.value("image_format_type").toString();
        struct_img_info.password                = query_select.value("image_password").toString();
        struct_img_info.image_source_type_display          = query_select.value("image_source_type_display").toString();
        struct_img_info.image_source_type_internal          = query_select.value("image_source_type_internal").toString();

        QString int_str = query_select.value(15).toString();

        query_select_partition_info.prepare("SELECT volume_type, volume_total_size_byte_numeric, volume_identifier, volume_node,"
                                            "volume_free_size_byte_numeric, volume_fs_type, derived_into_disk_node,"
                                            "derived_into_disk_identifier, derived_into_volume_name,"
                                            "volume_uuid, mount_path FROM partition_info WHERE image_id = ?");

        query_select_partition_info.addBindValue(int_str);

        if(!query_select_partition_info.exec())
        {
            recon_static_functions::app_debug(" query_select_partition_info  --------FAILED------ db file - " + mounted_image_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query_select_partition_info.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query_select_partition_info.lastError().text(),Q_FUNC_INFO);
        }

        while(query_select_partition_info.next())
        {
            struct_global_image_mounter_partition_info struct_partion_info;
            recon_static_functions::clear_variables_image_mounting_partiton_info(struct_partion_info);

            struct_partion_info.parent_type                     = query_select_partition_info.value(0).toString();
            struct_partion_info.total_size_byte_numeric_QString               = query_select_partition_info.value(1).toString();
            struct_partion_info.parent_identifier               = query_select_partition_info.value(2).toString();
            struct_partion_info.parent_node                     = query_select_partition_info.value(3).toString();
            struct_partion_info.free_size_byte_numeric_QString                = query_select_partition_info.value(4).toString();
            struct_partion_info.fs_type                  = query_select_partition_info.value(5).toString();

            struct_partion_info.derived_into_node          = query_select_partition_info.value(6).toString();
            struct_partion_info.derived_into_identifier    = query_select_partition_info.value(7).toString();
            struct_partion_info.derived_into_name        = query_select_partition_info.value(8).toString();
            struct_partion_info.derived_into_uuid                     = query_select_partition_info.value(9).toString();
            struct_partion_info.mount_path                      = query_select_partition_info.value(10).toString();

            struct_img_info.list_struct_partition_info.append(struct_partion_info);
        }

        list_struct_image_info.append(struct_img_info);
    }

    QString command = "DELETE FROM images_info";
    recon_helper_standard_obj->execute_db_command_by_dbreference(command,destination_db,Q_FUNC_INFO);

    command = "DELETE FROM partition_info";
    recon_helper_standard_obj->execute_db_command_by_dbreference(command,destination_db,Q_FUNC_INFO);

    destination_db.close();

    // image_mounter_obj->set_progress_bar(progress_bar_fixed_in_statusbar_obj);
    image_mounter_obj->set_list_image_info(list_struct_image_info);
    image_mounter_obj->unmount_all_image();
    image_mounter_obj->unmount_all_fuse();
    image_mounter_obj->clear_list_image_info();


    QString tmp_dir = QString(global_variable_private_var_tmp_Dir_Path_Case_QString);
    if(QFileInfo(tmp_dir).exists())
    {
        QDirIterator iter(tmp_dir, QDirIterator::NoIteratorFlags);
        while(iter.hasNext())
        {
            QFile(iter.next()).remove();
        }
    }

    //  case_wizard_object->pub_set_message_in_label_for_unmounting_and_enable_disable_ui("", true);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

QStringList MainWindow::get_mounted_volumes_path_with_node_list()
{
    recon_static_functions::app_debug("get_mounted_volumes_path_with_node_list Start",Q_FUNC_INFO);

    QStringList path_with_node_list;

    QString mount_output = run_command_without_argument("mount");

    QStringList all_media_list = mount_output.split("\n", Qt::SkipEmptyParts);

    for(int i = 0; i < all_media_list.size(); i++)
    {
        QString line_val_str = all_media_list.at(i);
        if(line_val_str.startsWith("/dev/"))
        {
            QStringList splitted_list = line_val_str.split(" (", Qt::SkipEmptyParts);

            QString tmp_str = splitted_list.at(0);

            if(tmp_str.isEmpty())
                continue;

            QStringList tmp_list = tmp_str.split("on", Qt::SkipEmptyParts);

            if(tmp_list.size() < 2)
                continue;

            QString disk_node_name = tmp_list.at(0).trimmed();
            QString disk_mount_path = tmp_list.at(1).trimmed();

            if(disk_mount_path == "/")
                continue;

            QString val_str = disk_node_name + MACRO_RECON_Splitter_1_multiple + disk_mount_path;

            path_with_node_list << val_str;
        }
    }

    recon_static_functions::app_debug("get_mounted_volumes_path_with_node_list End",Q_FUNC_INFO);

    return path_with_node_list;
}

void MainWindow::show_launcher_add_source(QList<struct_GLOBAL_witness_info_root> received_list_root_info)
{
    root_count_name_list_to_be_process.clear();
    source_count_name_list_to_be_process.clear();

    disk_manager_obj->pub_stop_disk_arbitration();

    recon_case_obj = new RECON_case(this);




    connect(recon_configuration_after_launch_obj, SIGNAL(signal_recon_config_apply_button_clicked()), recon_case_obj, SLOT(slot_recon_config_apply_button_clicked()));

    r_case_connections();

    create_recon_necessary_paths();


    if(!global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().trimmed().isEmpty())
    {
        QString agency_logo_path_from_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + QString("icons/agency_logo.png");
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,agency_logo_path_from_result);
    }

    // Create Dt Search Index
    QString path_to_xilib = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + MACRO_Index_Files_Dir_Name_QString;
    DString homeDir, privateDir;
    privateDir = path_to_xilib.toLocal8Bit().data();

    dt_search_interface_obj = new dt_search_interface(cin, cout, homeDir, privateDir);
    dt_search_interface_obj->init();


    // Exit-1
    int progress_val = 50;
    //    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(true);
    //    progress_bar_fixed_in_statusbar_obj->set_status_message("Please wait...");
    //    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_val);
    //    progress_bar_fixed_in_statusbar_obj->show();

    case_info_details_obj->pub_write_case_info_in_textfile_and_db();
    case_info_details_obj->pub_extract_full_caseinfo();

    map_key_src_cnt_name_val_raw_fuse_mnt_path.clear();

    prepare_roots_detail(received_list_root_info);

    global_witness_info_manager_class_obj->pub_fill_witness_info_QList_from_store();
    update_tsk_mount_path_for_source();
    update_raw_fuse_mount_path_for_source();
    update_zip_mount_path_for_source();

    QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    QList<struct_GLOBAL_witness_info_root> list_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

    //====================Print Info Start=================//
    recon_static_functions::print_witness_root_info(list_root_info,root_count_name_list_to_be_process);
    recon_static_functions::print_witness_source_info(list_source_info,source_count_name_list_to_be_process);
    //====================Print Info End=================//


    progress_val = progress_val + 5;
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_val);

    struct_global_file_system_post_launcher_job_info st_obj;

    for(int i = 0; i < list_source_info.size(); i++)
    {
        struct_GLOBAL_witness_info_source struct_source_info = list_source_info.at(i);

        QString source_count_name = struct_source_info.source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Apple_Metadata_Check_Status_bool).toBool())
            st_obj.source_list_apple_metadata << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool).toBool())
            st_obj.source_list_exif_metadata << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Signature_Check_Status_bool).toBool())
            st_obj.source_list_signature_analysis << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Hashset_Check_Status_bool).toBool())
            st_obj.source_list_hashes << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Face_Analysis_Check_Status_bool).toBool())
            st_obj.source_list_face_analysis << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Optical_Character_Recognition_Check_Status_bool).toBool())
            st_obj.source_list_optical_character_recognition << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Skin_Tone_Detection_Check_Status_bool).toBool())
            st_obj.source_list_skin_tone_detection << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Weapons_Check_Status_bool).toBool())
            st_obj.source_list_weapons << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Fire_Analysis_Check_Status_bool).toBool())
            st_obj.source_list_fire_analysis << source_count_name;

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Unified_Logs_Check_Status_bool).toBool())
            st_obj.source_list_unified_logs << source_count_name;

        QString directory_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name;

        QDir dir_fs;
        if(!dir_fs.mkdir(directory_path))
        {
            recon_static_functions::app_debug(" : show_r_case_common_first_launcher - directory create  --FAILED-- ",Q_FUNC_INFO);
        }


    }


    progress_val = progress_val + 5;
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_val);

    copy_resource_files_to_destination();
    progress_val = progress_val + 15;
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_val);

    extract_online_accounts_for_redefined_result();
    progress_val = progress_val + 15;
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_val);

    case_tree_display();

    // do not change position of this boolean, it mean full tree is loaded here, after case_tree_display()
    bool_launcher_work_completed = true;
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(100);
    progress_bar_fixed_in_statusbar_obj->hide();
    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(false);

    recon_case_obj->set_progress_bar_object(progress_bar_fixed_in_statusbar_obj);
    recon_case_obj->pub_set_task_status_window_object(task_status_window_obj);
    recon_case_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    recon_case_obj->set_ios_parser_map(map_ios_uid_backup_parser_obj);
    recon_case_obj->pub_set_file_system_selected_job_source_count_name(st_obj);

    recon_case_obj->set_case_treewidget_obj(treewidget_case_display_obj);
    recon_case_obj->set_essentials();
    recon_case_obj->pub_set_dt_search_obj(dt_search_interface_obj);

    recon_case_obj->pub_set_face_finder_obj(facefinder_obj);
    recon_case_obj->pub_set_recognize_face_obj(recognize_face_obj);
    facefinder_obj->pub_set_task_status_window(task_status_window_obj);

    recon_case_obj->pub_set_recon_case_runner_type(MACRO_RECON_CASE_RUNNER_TYPE_FIRST_LAUNCHER);

    recon_case_obj->set_selected_pluginname_list_for_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_QStringList).toStringList());

    recon_case_obj->save_selected_plugin_tab_info_in_db();

    recon_case_obj->pub_set_FS_source_selected_source_count_name_list(source_count_name_list_to_be_process);

    recon_configuration_after_launch_obj->pub_set_essentials();
    recon_configuration_after_launch_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER);
    recon_configuration_after_launch_obj->pub_initialise_configuration();




    recon_case_obj->start_extraction();
}

void MainWindow::slot_toolbar_add_source(QList<struct_GLOBAL_witness_info_root> list_root)
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    root_count_name_list_to_be_process.clear();
    source_count_name_list_to_be_process.clear();
    prepare_roots_detail(list_root);
    show_r_case_common_add_source();

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::show_r_case_common_add_source()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    global_witness_info_manager_class_obj->pub_fill_witness_info_QList_from_store();
    update_tsk_mount_path_for_source();
    update_raw_fuse_mount_path_for_source();
    update_zip_mount_path_for_source();

    QList<struct_GLOBAL_witness_info_source> list_all_sources_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    QList<struct_GLOBAL_witness_info_root> list_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

    //====================Print Info Start=================//
    recon_static_functions::print_witness_root_info(list_root_info,root_count_name_list_to_be_process);
    recon_static_functions::print_witness_source_info(list_all_sources_info,source_count_name_list_to_be_process);
    //====================Print Info End=================//

    //-For progress value increase.
    if(progress_bar_fixed_in_statusbar_obj->get_progressbar_value() < 80)
        progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_bar_fixed_in_statusbar_obj->get_progressbar_value() + 10);

    for(int pp = 0; pp < source_count_name_list_to_be_process.size(); pp++)
    {
        QString source_count_name = source_count_name_list_to_be_process.at(pp);
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


        QString directory_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name;

        QDir dir_fs;
        if(!dir_fs.mkdir(directory_path))
        {
            recon_static_functions::app_debug(" : show_r_case_common_first_launcher - directory create  --FAILED-- ",Q_FUNC_INFO);
        }

        common_tree_add_source_from_fs(struct_source_info);

        //-For progress value increase.
        if(progress_bar_fixed_in_statusbar_obj->get_progressbar_value() < 80)
            progress_bar_fixed_in_statusbar_obj->set_progressbar_value(progress_bar_fixed_in_statusbar_obj->get_progressbar_value() + 10);

    }

    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(100);
    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(false);
    progress_bar_fixed_in_statusbar_obj->hide();


    QString plugin_db_path;
    plugin_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    QString command = "select distinct plugin_name from tbl_plugin_status";
    QStringList plugin_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,db_path,Q_FUNC_INFO);


    QStringList plugin_withtab = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList,plugin_withtab);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Selected_Plugins_QStringList,plugin_list);

    extract_online_accounts_for_redefined_result();

    recon_case_obj->set_progress_bar_object(progress_bar_fixed_in_statusbar_obj);
    recon_case_obj->pub_set_task_status_window_object(task_status_window_obj);
    recon_case_obj->set_ios_parser_map(map_ios_uid_backup_parser_obj);
    recon_case_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    recon_case_obj->set_case_treewidget_obj(treewidget_case_display_obj);

    recon_case_obj->set_essentials();
    recon_case_obj->set_selected_pluginname_list_for_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_QStringList).toStringList());

    recon_case_obj->pub_set_FS_source_selected_source_count_name_list(source_count_name_list_to_be_process);
    recon_case_obj->pub_set_recon_case_runner_type(MACRO_RECON_CASE_RUNNER_TYPE_ADD_SOURCE);

    recon_case_obj->start_extraction();


    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Case+ MACRO_RECON_Splitter_1_multiple + MACRO_COMBOBOX_FEATURE_Case_Info;
    comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);
    display_case_tabs(MACRO_COMBOBOX_FEATURE_Case_Info,combobox_case_navigation->currentText(),MACRO_COMBOBOX_FEATURE_Case_Info,"","",Q_FUNC_INFO);
    remove_case_details_from_display_on_add_source();
    remove_selected_plugins_from_display_on_add_source();
    show_metadata_widget(false);

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::slot_right_click_add_source(struct_GLOBAL_witness_info_source st_source_info)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    root_count_name_list_to_be_process.clear();
    source_count_name_list_to_be_process.clear();

    prepare_sources_detail(st_source_info);
    show_r_case_common_add_source();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}


header_plugin_extraction_interface *MainWindow::find_plugin_ptr_by_pluginname(QString plugin_name)
{
    QString lib_path = QString("../Artifacts_Libs/libm_") + plugin_name.replace(" ","_").toLower() + ".1.0.0.dylib";
    QFileInfo info(lib_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file not exist 111 ---FAILED---- " + lib_path,Q_FUNC_INFO);
        return NULL;
    }

    QString new_lib_path = recon_static_functions::decrypt_file_1(lib_path, Q_FUNC_INFO);

    QFileInfo info1(new_lib_path);
    if(!info1.exists())
    {
        recon_static_functions::app_debug(" file not exist 222 ---FAILED---- " + new_lib_path,Q_FUNC_INFO);
        return NULL;
    }


    QLibrary module_lib(new_lib_path);
    QCoreApplication::processEvents();

    header_plugin_extraction_interface::entry_point_plugin_exraction entry_point_func = (header_plugin_extraction_interface::entry_point_plugin_exraction)module_lib.resolve("create_object");

    if(entry_point_func == NULL)
    {
        QFile file1(new_lib_path);
        file1.remove();
        recon_static_functions::app_debug(" Pointer entry_point_func NULL  create_object ---FAILED---- " + plugin_name,Q_FUNC_INFO);
        return NULL;
    }

    header_plugin_extraction_interface *ptr = (header_plugin_extraction_interface *)(entry_point_func());

    if(!ptr)
    {
        QFile file1(new_lib_path);
        file1.remove();
        recon_static_functions::app_debug(" Pointer NULL recon_module_interface ---FAILED---- " + plugin_name,Q_FUNC_INFO);
        return NULL;
    }

    QFile file1(new_lib_path);
    file1.remove();

    return ptr;
}

void MainWindow::extract_online_accounts_for_redefined_result()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    if(list_target_source_info.size() <= 0)
    {
        recon_static_functions::app_debug("list_target_source_info.size() " + QString::number(list_target_source_info.size()),Q_FUNC_INFO);
        return;
    }

    QString plugin_name_str = "Online Accounts";

    plugin_extraction_interface_obj = find_plugin_ptr_by_pluginname(plugin_name_str);
    if(plugin_extraction_interface_obj == NULL)
    {
        recon_static_functions::app_debug("-----FAILED-----to extract plugin - Online Accounts",Q_FUNC_INFO);
        recon_static_functions::app_debug(" failed  Please check rcbuild, may be rcbuild is of another project",Q_FUNC_INFO);
        return;
    }

    plugin_extraction_interface_obj->pub_set_engine_type(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_CALLER_ENGINE_TYPE_RECON_LAB);


    QStringList selected_table_list;
    QStringList total_table_list;

    selected_table_list << "accounts";
    total_table_list << "accounts";

    plugin_extraction_interface_obj->pub_set_essential_for_recon_lab_extraction(selected_table_list, total_table_list);

    plugin_extraction_interface_obj->pub_set_result_directory(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString());

    plugin_extraction_interface_obj->pub_set_clear_db(true);

    plugin_extraction_interface_obj->pub_set_extended_attributes_list(QStringList(""));
    plugin_extraction_interface_obj->pub_create_db_tables();

    QStringList supported_os_schme_dsply_QStringList;
    supported_os_schme_dsply_QStringList  << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                                          << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                                          << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                                          << MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display;

    for(int i = 0; i < list_target_source_info.size(); i++)
    {
        struct_GLOBAL_witness_info_source struct_target_info = list_target_source_info.at(i);

        if(!supported_os_schme_dsply_QStringList.contains(struct_target_info.os_scheme_display))
            continue;


        if(!QFileInfo(struct_target_info.virtual_source_path).exists())
            continue;

        QString initial_source_path = struct_target_info.virtual_source_path;
        if(!initial_source_path.endsWith("/"))
            initial_source_path.append("/");


        struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_source_info cmn_src_obj;
        recon_static_functions::clear_variables_recon_and_artifact_common_source_info(cmn_src_obj);
        cmn_src_obj.regular_source_path = initial_source_path;
        cmn_src_obj.os_scheme_display = struct_target_info.os_scheme_display;
        cmn_src_obj.os_scheme_internal = struct_target_info.os_scheme_internal;

        for(int pp = 0; pp < struct_target_info.system_account_QList.size(); pp++)
        {
            struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_system_account m_obj;
            recon_static_functions::clear_variables_recon_and_artifact_common_system_account_info(m_obj);

            m_obj.system_user = struct_target_info.system_account_QList.at(pp).system_user;
            m_obj.domain_user = struct_target_info.system_account_QList.at(pp).domain_user;

            cmn_src_obj.system_account_QList.append(m_obj);
        }

        cmn_src_obj.source_type_internal = struct_target_info.source_type_internal;

        plugin_extraction_interface_obj->pub_set_source_info(cmn_src_obj);

        if(struct_target_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
        {
            plugin_extraction_interface_obj->pub_set_ios_uid_backup_pointer(map_ios_uid_backup_parser_obj->value(struct_target_info.source_count_name));
        }

        QString src_cnt_nm = struct_target_info.source_count_name;
        int src_cnt_int = src_cnt_nm.remove(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).toInt();

        plugin_extraction_interface_obj->pub_set_source_count(src_cnt_int);

        plugin_extraction_interface_obj->pub_find_source_file();
        plugin_extraction_interface_obj->pub_extract_artifacts();
    }

    plugin_extraction_interface_obj->pub_remove_temporary_asthetics();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}


void MainWindow::update_raw_fuse_mount_path_for_source()
{
    bool bool_any_source_updated = false;
    QList<struct_GLOBAL_witness_info_source> list_all_sources_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QList<struct_GLOBAL_witness_info_source> final_list_all_sources_info ;
    for(int i = 0; i < list_all_sources_info.size(); i++)
    {
        struct_GLOBAL_witness_info_source struct_source_info = list_all_sources_info.at(i);
        QString source_count_name = struct_source_info.source_count_name;

        QString osx_mount_point;
        QMapIterator<QString, QString> itr(map_key_src_cnt_name_val_raw_fuse_mnt_path);
        while (itr.hasNext())
        {
            itr.next();
            if(itr.key() == source_count_name)
            {
                bool_any_source_updated = true;
                osx_mount_point = itr.value();
            }
        }

        struct_source_info.fuse_mount_path_raw = osx_mount_point;
        final_list_all_sources_info.append(struct_source_info);
    }

    if(bool_any_source_updated)
        global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(final_list_all_sources_info);

}

void MainWindow::update_zip_mount_path_for_source()
{
    //Don't need to call this function for load case as now for GrayKey Backup(As timestamps are filled in fs db at time of add source)
    //In future, If recquired to call for other sources you can call it for load case.
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    bool bool_any_source_updated = false;

    for(int pp = 0; pp < list_source_info.size(); pp++)
    {

        struct_GLOBAL_witness_info_source src_obj =  list_source_info.at(pp);
        if(src_obj.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal
                && src_obj.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
        {
            continue;
        }

        QString src_zip_path = src_obj.source_path_1;

        QString fuse_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString();
        QString available_file_1 = recon_static_functions::get_available_filename(src_obj.source_count_name,fuse_dir_path, Q_FUNC_INFO);
        QString fuse_mnt_path = fuse_dir_path + available_file_1;
        QDir mdir;
        mdir.mkpath(fuse_mnt_path);

        QString exec_path = "./fuse_zip_pie/fuse_zip_pie";

        QStringList args;
        args << "-r" << src_zip_path << fuse_mnt_path;

        recon_helper_process_obj->run_command_with_arguments(exec_path, args, Q_FUNC_INFO);


        // to check fuse is mounted or not
        if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(fuse_mnt_path, Q_FUNC_INFO))
            continue;

        // /ROOT is used because when we mount zip file using its zip fuse then it add ROOT Dir in mounted path
        // /mnt/ROOT/private/var/tmp
        fuse_mnt_path.append("/");


        src_obj.fuse_mount_path_zip = fuse_mnt_path + "ROOT/";

        bool_any_source_updated = true;
        list_source_info.removeAt(pp);
        list_source_info.insert(pp, src_obj);

    }

    if(bool_any_source_updated)
        global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info);

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

}

void MainWindow::clear_case_info_narad_variables()
{
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool, false);

    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64,0);
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_int,0);
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64,0);


    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_All_Key_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_All_Custom_Column_Name_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Key_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Custom_Column_Name_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_All_Key_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_All_Custom_Column_Name_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Key_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Custom_Column_Name_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList,QStringList());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Report_Custom_Column_Name_QStringList,QStringList());


    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Debug_File_Path_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Plugin_Result_Dir_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Name_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Name_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Phone_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Email_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Address_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_ID_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Name_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Location_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Notes_QString,QString());
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_Updated_For_Versions_QString,QString());


}

void MainWindow::clear_source_narad_variables_partial()
{

    QList<QVariant> qlist_variant;

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool,false);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, QStringList());

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_System_Account_QList, qlist_variant);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Imei_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList,QStringList());

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64,0);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,0);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Count_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Offset_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_UUID_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Virtual_Source_Path_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Scheme_Display_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Build_Version_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Device_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Id_No_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Serial_No_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_iTune_Version_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Display_Name_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_GUID_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Name_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Target_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Model_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Wifi_Address_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Locale_Language_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Role_Type_QString,QString());

}

void MainWindow::clear_root_narad_variables_all()
{

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Count_Name_All_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList,QStringList());


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Evidence_Number_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Description_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Count_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Evidence_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Type_Display_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Type_Internal_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Path_1_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Path_2_for_fusion_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Hash_MD5_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Hash_SHA1_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_1_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_1_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString,QString());

}

void MainWindow::clear_root_narad_variables_partial()
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Count_Name_QString,QString());
}


void MainWindow::clear_source_narad_variables_all()
{

    QList<QVariant> qlist_variant;

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Nested_bool,false);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Decompressed_bool,false);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool,false);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64,0);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,0);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64, 0);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList,QStringList());

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_System_Account_QList, qlist_variant);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Imei_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList,QStringList());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList,QStringList());

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RUNTIME_Root_Recovery_Passcode_Plain_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString,QString());

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Count_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Hierarchy_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Scheme_Display_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Type_Display_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Type_Internal_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_1_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Main_Path_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Virtual_Source_Path_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Offset_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_UUID_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Device_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Id_No_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_GUID_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Target_Type_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Build_Version_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_iTune_Version_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Serial_No_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Display_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Main_Name_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Model_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Wifi_Address_QString, QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Locale_Language_QString,QString());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Role_Type_QString,QString());

}


QList <struct_global_system_account_info> MainWindow::get_system_account_QList_for_macos(QString virtual_source_path, QString src_type_intrnl)
{
    QList <struct_global_system_account_info> list_systm_account_info ;
    list_systm_account_info.clear();


    if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
    {
        QList <struct_global_system_account_info> nrd_list_sys_acnt_info ;
        nrd_list_sys_acnt_info.clear();

        QList <QVariant> qvariant_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_System_Account_QList).toList();

        for(int pp = 0; pp < qvariant_list.size(); pp++)
        {
            QVariant var = qvariant_list.at(pp);

            if(var.canConvert<QList<struct_global_system_account_info>>())
            {
                nrd_list_sys_acnt_info = var.value<QList<struct_global_system_account_info>>();
            }
        }

        list_systm_account_info = nrd_list_sys_acnt_info;

    }
    else
    {
        QStringList system_user_list = recon_static_functions::get_system_user_list_of_macOS(virtual_source_path, src_type_intrnl, Q_FUNC_INFO);

        // ============= Preparing System Account QList Start==========//
        for(int pp = 0; pp < system_user_list.size() ; pp++)
        {
            struct_global_system_account_info m_obj;
            recon_static_functions::clear_variables_system_account_info(m_obj);

            m_obj.system_user = system_user_list.at(pp);
            m_obj.domain_user = "";

            list_systm_account_info.append(m_obj);

        }
        // ============= Preparing System Account QList End==========//

    }

    return list_systm_account_info;

}




QString MainWindow::soft_mount(QString path,QString username)
{
    recon_static_functions::app_debug("soft_mount start",Q_FUNC_INFO);

    // Input User Name : user2
    // Input Path: /Users/maruti/RECON_mount_path/RECON_MNT_disk5s1_user2.dmg/
    // Return Path : /Users/maruti/tmp_sumuri_recon_img_mount/dummy_root/RECON_MNT_disk5s1_user2.dmg/


    QProcess proc;
    QStringList args;
    args << "-a" << "L" << path;
    proc.execute("SetFile", args);


    // New Path = /Users/maruti/tmp_sumuri_recon_img_mount/dummy_root/RECON_MNT_disk5s1_user2.dmg
    QString new_path;
    new_path = create_available_path_for_soft_mounting(path);



    // softlink_path : /Users/maruti/tmp_sumuri_recon_img_mount/dummy_root/RECON_MNT_disk5s1_user2.dmg/Users/forenso
    QString softlink_path = new_path + "/Users/";
    QDir dir;
    dir.mkpath(softlink_path);
    softlink_path.append(username);


    QStringList arg;
    arg << "-s" << path << softlink_path;

    run_command_generic("ln",arg);

    if(!softlink_path.trimmed().isEmpty() && !softlink_path.endsWith("/"))
        softlink_path.append("/");

    recon_static_functions::app_debug("soft_mount end " + new_path,Q_FUNC_INFO);

    return softlink_path;
}

QString MainWindow::create_available_path_for_soft_mounting(QString path)
{
    if(path.endsWith("/"))
        path.chop(1);

    QString recon_mnt_pth = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Dummy_Root_QString).toString();

    QFileInfo info(path);
    QString new_path = recon_mnt_pth + recon_static_functions::get_available_filename(info.fileName(),recon_mnt_pth, Q_FUNC_INFO);
    QDir dir;
    dir.mkpath(new_path);

    return new_path;
}

QString MainWindow::run_command_generic(QString command, QStringList arguments)
{
    bool_process_generic = false;
    process_generic->start(command,arguments);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic)
            break;
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}

QString MainWindow::run_command_without_argument(QString command)
{
    bool_prc_generic_without_argument = false;
    prc_generic_without_argument->start(command);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_prc_generic_without_argument)
            break;
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(prc_generic_without_argument->readAll().trimmed());
}

QString MainWindow::extract_volume_name_from_mount_path(QString path)
{

    // Input path : /Users/maruti/RECON_mount_path/RECON_MNT_disk5s1_user2.dmg/

    QStringList args;
    args << "info" << path;

    QString str =  run_command_generic("diskutil",args);

    QStringList list = str.split("\n", Qt::SkipEmptyParts);

    for(int i = 0; i < list.size(); i++)
    {
        QString tt = list.at(i);
        tt =  tt.trimmed();

        if(tt.startsWith("Volume Name:"))
        {
            tt.remove("Volume Name:");
            tt = tt.trimmed();

            if(tt != QString(""))
                return tt;
        }
    }

    return QString("dummy");
}

void MainWindow::case_tree_display()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);


    global_csv_reader_class_full_obj->set_csv_dir_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString).toString());
    global_csv_reader_class_full_obj->do_job_plugin_all();

    //========== selected tab csv start========//
    // copy values into selected obj from full obj
    // avoiding again parsing of csv
    *global_csv_reader_class_selected_obj = *global_csv_reader_class_full_obj;
    global_csv_reader_class_selected_obj->set_plugin_and_associated_tabs(global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList).toStringList(), Q_FUNC_INFO);
    //========== selected tab csv end========//


    QTreeWidgetItem *parent_item = new QTreeWidgetItem;
    parent_item->setText(enum_global_case_tree_display_column_ZERO,MACRO_CASE_TREE_Case + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    parent_item->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/case_info.png"));
    parent_item->setText(enum_global_case_tree_display_column_FIRST,MACRO_COMBOBOX_FEATURE_Case_Info);
    parent_item->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

    common_treewidget_case_display_single_clicked(parent_item,0);

    QTreeWidgetItem *child_item_1 = new QTreeWidgetItem;
    child_item_1->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Source));
    child_item_1->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/Source.png"));
    child_item_1->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_1->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Parent_Source);
    parent_item->addChild(child_item_1);


    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int i = 0; i < list_target_source_info.size(); i++)
    {
        struct_GLOBAL_witness_info_source struct_info = list_target_source_info.at(i);
        insert_source_entry_in_source_case_tree(child_item_1, struct_info);
    }


    QTreeWidgetItem *child_item_mobile_bkp = new QTreeWidgetItem;
    child_item_mobile_bkp->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Mobile_Backup_Parent));
    child_item_mobile_bkp->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/mobile_backup.png"));
    child_item_mobile_bkp->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_mobile_bkp->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Mobile_Backup_Parent));
    parent_item->addChild(child_item_mobile_bkp);

    QTreeWidgetItem *child_item_rtrvd_srcs = new QTreeWidgetItem;
    child_item_rtrvd_srcs->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Disk_Images_Parent));
    child_item_rtrvd_srcs->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/disk_images.png"));
    child_item_rtrvd_srcs->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_rtrvd_srcs->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Disk_Images_Parent));
    parent_item->addChild(child_item_rtrvd_srcs);


    QTreeWidgetItem *child_items_ram_images = new QTreeWidgetItem;
    child_items_ram_images->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_RAM_Images));
    child_items_ram_images->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/ram_images.png"));
    child_items_ram_images->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_RAM_Images));
    child_items_ram_images->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Ram_Images_Parent);
    parent_item->addChild(child_items_ram_images);


    QTreeWidgetItem *child_item_apple_metadata = new QTreeWidgetItem;
    child_item_apple_metadata->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Apple_Metadata_Parent));
    child_item_apple_metadata->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/apple_metadata.png"));
    child_item_apple_metadata->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_apple_metadata->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Apple_Metadata_Parent));
    parent_item->addChild(child_item_apple_metadata);

    QTreeWidgetItem *child_item_exif = new QTreeWidgetItem;
    child_item_exif->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Exif_Parent));
    child_item_exif->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/exif_data.png"));
    child_item_exif->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_exif->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Exif_Parent));
    parent_item->addChild(child_item_exif);

    QTreeWidgetItem *child_item_mime_types = new QTreeWidgetItem;
    child_item_mime_types->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_MIME_Type));
    child_item_mime_types->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/mime_type.png"));
    child_item_mime_types->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_mime_types->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_MIME_Type));
    parent_item->addChild(child_item_mime_types);

    QTreeWidgetItem *child_item_face_extraction = new QTreeWidgetItem;
    child_item_face_extraction->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Face_Analysis_PARENT));
    child_item_face_extraction->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/face_analysis.png"));
    child_item_face_extraction->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_face_extraction->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Face_Analysis_PARENT));
    parent_item->addChild(child_item_face_extraction);

    QTreeWidgetItem *child_item_face_search = new QTreeWidgetItem;
    child_item_face_search->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Face_Search_PARENT));
    child_item_face_search->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/face_search.png"));
    child_item_face_search->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_face_search->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Face_Search_PARENT));
    parent_item->addChild(child_item_face_search);

    QTreeWidgetItem *child_item_optical_character_recognition = new QTreeWidgetItem;
    child_item_optical_character_recognition ->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent));
    child_item_optical_character_recognition ->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/optical_character_recognition.png"));
    child_item_optical_character_recognition ->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_optical_character_recognition ->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent));
    parent_item->addChild(child_item_optical_character_recognition );

    QTreeWidgetItem *child_item_skin_tone_detection = new QTreeWidgetItem;
    child_item_skin_tone_detection->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent));
    child_item_skin_tone_detection->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/skin_tone_detection.png"));
    child_item_skin_tone_detection->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_skin_tone_detection->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent));
    parent_item->addChild(child_item_skin_tone_detection);

    QTreeWidgetItem *child_item_weapons_types = new QTreeWidgetItem;
    child_item_weapons_types->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Weapons_Parent));
    child_item_weapons_types->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/weapons.png"));
    child_item_weapons_types->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_weapons_types->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Weapons_Parent));
    parent_item->addChild(child_item_weapons_types);

    QTreeWidgetItem *child_item_fire_types = new QTreeWidgetItem;
    child_item_fire_types->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Fire_Analysis_Parent));
    child_item_fire_types->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/fire_analysis.png"));
    child_item_fire_types->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_fire_types->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Fire_Analysis_Parent));
    parent_item->addChild(child_item_fire_types);

    QTreeWidgetItem *child_item_extensions = new QTreeWidgetItem;
    child_item_extensions->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_File_Extensions));
    child_item_extensions->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/extensions.png"));
    child_item_extensions->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_extensions->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_File_Extensions));
    parent_item->addChild(child_item_extensions);


    QTreeWidgetItem *child_item_file_signature = new QTreeWidgetItem;
    child_item_file_signature->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_File_Signature_Parent));
    child_item_file_signature->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/file_signatures.png"));
    child_item_file_signature->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_file_signature->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_File_Signature_Parent));
    parent_item->addChild(child_item_file_signature);

    QTreeWidgetItem *child_item_file_size = new QTreeWidgetItem;
    child_item_file_size->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_File_Size_Parent));
    child_item_file_size->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/file_size.png"));
    child_item_file_size->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_file_size->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_File_Size_Parent));
    parent_item->addChild(child_item_file_size);

    QTreeWidgetItem *child_item_unified_log_viewer = new QTreeWidgetItem;
    child_item_unified_log_viewer->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Unified_Logs));
    child_item_unified_log_viewer->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/unified_logs.png"));
    child_item_unified_log_viewer->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Unified_Logs));
    child_item_unified_log_viewer->setText(enum_global_case_tree_display_column_SECOND,"Parent Unified Logs");
    parent_item->addChild(child_item_unified_log_viewer);

    QTreeWidgetItem *child_item_snapshots = new QTreeWidgetItem;
    child_item_snapshots->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Snapshots));
    child_item_snapshots->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/snapshots.png"));
    child_item_snapshots->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_snapshots->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Snapshots_Parent);
    parent_item->addChild(child_item_snapshots);

    QTreeWidgetItem *child_item_hashset = new QTreeWidgetItem;
    child_item_hashset->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Hashset));
    child_item_hashset->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/hashset.png"));
    child_item_hashset->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_hashset->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Hashset));
    parent_item->addChild(child_item_hashset);

    QTreeWidgetItem *child_item_griffeye_data = new QTreeWidgetItem;
    child_item_griffeye_data->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Griffeye_Data_Parent));
    child_item_griffeye_data->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/griffeye.png"));
    child_item_griffeye_data->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_griffeye_data->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Griffeye_Data_Parent));
    parent_item->addChild(child_item_griffeye_data);

    QTreeWidgetItem *child_item_email_files = new QTreeWidgetItem;
    child_item_email_files->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Email_Files_Parent));
    child_item_email_files->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/email_files.png"));
    child_item_email_files->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_email_files->setText(enum_global_case_tree_display_column_SECOND,"Parent " + QString(MACRO_CASE_TREE_Email_Files_Parent));
    parent_item->addChild(child_item_email_files);

    QTreeWidgetItem *child_item_2_macos = new QTreeWidgetItem;
    child_item_2_macos->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Artifacts));
    child_item_2_macos->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/Artifacts.png"));
    child_item_2_macos->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_2_macos->setText(enum_global_case_tree_display_column_SECOND,"Parent Plugins");
    parent_item->addChild(child_item_2_macos);


    QTreeWidgetItem *child_item_k_srch = new QTreeWidgetItem;
    child_item_k_srch->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Artifacts_Keyword_Search));
    child_item_k_srch->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/artifacts_keyword_search.png"));
    child_item_k_srch->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Artifacts_Keyword_Search));
    child_item_k_srch->setText(enum_global_case_tree_display_column_SECOND,"Parent Artifacts Keyword Search");
    parent_item->addChild(child_item_k_srch);

    QTreeWidgetItem *child_item_file_search = new QTreeWidgetItem;
    child_item_file_search->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_File_Search));
    child_item_file_search->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/file_search.png"));
    child_item_file_search->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_File_Search));
    child_item_file_search->setText(enum_global_case_tree_display_column_SECOND,"Parent File Search");
    parent_item->addChild(child_item_file_search);


    QTreeWidgetItem *child_item_exif_metadata_search = new QTreeWidgetItem;
    child_item_exif_metadata_search->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Exif_Metadata_Search));
    child_item_exif_metadata_search->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/exif_metadata_search.png"));
    child_item_exif_metadata_search->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Exif_Metadata_Search));
    child_item_exif_metadata_search->setText(enum_global_case_tree_display_column_SECOND,"Parent Exif Metadata Search");
    parent_item->addChild(child_item_exif_metadata_search);

    QTreeWidgetItem *child_item_cntnt_search = new QTreeWidgetItem;
    child_item_cntnt_search->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Content_Search));
    child_item_cntnt_search->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/content_search.png"));
    child_item_cntnt_search->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Content_Search));
    child_item_cntnt_search->setText(enum_global_case_tree_display_column_SECOND,"Parent Content Search");
    parent_item->addChild(child_item_cntnt_search);

    QTreeWidgetItem *child_item_g_tline = new QTreeWidgetItem;
    child_item_g_tline->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Artifacts_Timeline));
    child_item_g_tline->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/timeline_full.png"));
    child_item_g_tline->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Artifacts_Timeline));
    child_item_g_tline->setText(enum_global_case_tree_display_column_SECOND,"Parent Timeline");
    parent_item->addChild(child_item_g_tline);


    QTreeWidgetItem *child_item_bucket = new QTreeWidgetItem;
    child_item_bucket->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Bucket));
    child_item_bucket->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/bucket.png"));
    child_item_bucket->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Bucket));
    child_item_bucket->setText(enum_global_case_tree_display_column_SECOND,"Parent Bucket");
    parent_item->addChild(child_item_bucket);

    QTreeWidgetItem *child_item_plist = new QTreeWidgetItem;
    child_item_plist->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Plist_Viewer));
    child_item_plist->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/plist_viewer.png"));
    child_item_plist->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Plist_Viewer));
    child_item_plist->setText(enum_global_case_tree_display_column_SECOND,"Parent Plist Viewer");
    child_item_bucket->addChild(child_item_plist);


    QTreeWidgetItem *child_item_hex = new QTreeWidgetItem;
    child_item_hex->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Hex_Viewer));
    child_item_hex->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/hex_viewer.png"));
    child_item_hex->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Hex_Viewer));
    child_item_hex->setText(enum_global_case_tree_display_column_SECOND,"Parent Hex Viewer");
    child_item_bucket->addChild(child_item_hex);

    QTreeWidgetItem *child_item_sqlite = new QTreeWidgetItem;
    child_item_sqlite->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Sqlite_Viewer));
    child_item_sqlite->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/sqlite_viewer.png"));
    child_item_sqlite->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Sqlite_Viewer));
    child_item_sqlite->setText(enum_global_case_tree_display_column_SECOND,"Parent SQLite Viewer");
    child_item_bucket->addChild(child_item_sqlite);

    QTreeWidgetItem *child_item_registry = new QTreeWidgetItem;
    child_item_registry->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Registry_Viewer));
    child_item_registry->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/registry_viewer.png"));
    child_item_registry->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Registry_Viewer));
    child_item_registry->setText(enum_global_case_tree_display_column_SECOND,"Parent Registry Viewer");
    child_item_bucket->addChild(child_item_registry);

    QTreeWidgetItem *child_item_log = new QTreeWidgetItem;
    child_item_log->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Log_Viewer));
    child_item_log->setIcon(enum_global_case_tree_display_column_ZERO, QIcon("../icons/Features/log_viewer.png"));
    child_item_log->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Log_Viewer));
    child_item_log->setText(enum_global_case_tree_display_column_SECOND,"Parent Log Viewer");
    child_item_bucket->addChild(child_item_log);

    QTreeWidgetItem *child_item_tags = new QTreeWidgetItem;
    child_item_tags->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Tags));
    child_item_tags->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/tags.png"));
    child_item_tags->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Tags));
    child_item_tags->setText(enum_global_case_tree_display_column_SECOND,"Parent Tags");
    parent_item->addChild(child_item_tags);

    QTreeWidgetItem *child_item_notes = new QTreeWidgetItem;
    child_item_notes->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Notes_Parent));
    child_item_notes->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/notes.png"));
    child_item_notes->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Notes_Parent));
    child_item_notes->setText(enum_global_case_tree_display_column_SECOND,"Parent Notes");
    parent_item->addChild(child_item_notes);

    QTreeWidgetItem *child_item_redefined_result = new QTreeWidgetItem;
    child_item_redefined_result->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Redefined_Result_Parent));
    child_item_redefined_result->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/redefined_result.png"));
    child_item_redefined_result->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Redefined_Result_Parent));
    child_item_redefined_result->setText(enum_global_case_tree_display_column_SECOND,"Parent Redefined Result");
    parent_item->addChild(child_item_redefined_result);

    QTreeWidgetItem *child_item_redefined_result_saved = new QTreeWidgetItem;
    child_item_redefined_result_saved->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Redefined_Result_Saved_Parent));
    child_item_redefined_result_saved->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/redefined_result.png"));
    child_item_redefined_result_saved->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_redefined_result_saved->setText(enum_global_case_tree_display_column_SECOND,"Parent Redefined Result Saved");
    parent_item->addChild(child_item_redefined_result_saved);

    QTreeWidgetItem *child_item_screenshots = new QTreeWidgetItem;
    child_item_screenshots->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Screenshots));
    child_item_screenshots->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/screenshots.png"));
    child_item_screenshots->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Screenshots));
    child_item_screenshots->setText(enum_global_case_tree_display_column_SECOND,"Parent Screenshots");
    parent_item->addChild(child_item_screenshots);


    QTreeWidgetItem *child_item_storyboard = new QTreeWidgetItem;
    child_item_storyboard->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Story_Board));
    child_item_storyboard->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/story_board.png"));
    child_item_storyboard->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Story_Board));
    child_item_storyboard->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Story_Board_Parent);
    parent_item->addChild(child_item_storyboard);


    QTreeWidgetItem *child_items_password_carved = new QTreeWidgetItem;
    child_items_password_carved->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Carved_Password));
    child_items_password_carved->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/carved_password.png"));
    child_items_password_carved->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Carved_Password));
    child_items_password_carved->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Carved_Password_Parent);
    parent_item->addChild(child_items_password_carved);

    QTreeWidgetItem *child_items_carved_files = new QTreeWidgetItem;
    child_items_carved_files->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Carved_Files));
    child_items_carved_files->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/carved_files.png"));
    child_items_carved_files->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Carved_Files));
    child_items_carved_files->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Carved_Files_Parent);
    parent_item->addChild(child_items_carved_files);

    QTreeWidgetItem *child_items_carved_data = new QTreeWidgetItem;
    child_items_carved_data->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Carved_Data));
    child_items_carved_data->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/carved_data.png"));
    child_items_carved_data->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Carved_Data));
    child_items_carved_data->setText(enum_global_case_tree_display_column_SECOND,MACRO_CASE_TREE_Carved_Data_Parent);
    parent_item->addChild(child_items_carved_data);

    QTreeWidgetItem *child_item_ram_analysis_saved = new QTreeWidgetItem;
    child_item_ram_analysis_saved->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Ram_Analysis_Saved));
    child_item_ram_analysis_saved->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/ram_analysis.png"));
    child_item_ram_analysis_saved->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_ram_analysis_saved->setText(enum_global_case_tree_display_column_SECOND,"Parent Ram Analysis Saved");
    parent_item->addChild(child_item_ram_analysis_saved);

    QTreeWidgetItem *child_item_graph_result_saved = new QTreeWidgetItem;
    child_item_graph_result_saved->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Graph_Result_Saved));
    child_item_graph_result_saved->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/graphs.png"));
    child_item_graph_result_saved->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_graph_result_saved->setText(enum_global_case_tree_display_column_SECOND,"Parent Graph Result Saved");
    parent_item->addChild(child_item_graph_result_saved);

    QTreeWidgetItem *child_item_saved_maps = new QTreeWidgetItem;
    child_item_saved_maps->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Saved_Maps));
    child_item_saved_maps->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/locations.png"));
    child_item_saved_maps->setText(enum_global_case_tree_display_column_FIRST,"");
    child_item_saved_maps->setText(enum_global_case_tree_display_column_SECOND,"Parent Saved Maps");
    parent_item->addChild(child_item_saved_maps);

    treewidget_case_display_obj->addTopLevelItem(parent_item);
    //treewidget_case_display_obj->expandAll();
    parent_item->setExpanded(true);

    treewidget_case_display_obj->hideColumn(enum_global_case_tree_display_column_FIRST);
    treewidget_case_display_obj->hideColumn(enum_global_case_tree_display_column_SECOND);
    treewidget_case_display_obj->hideColumn(enum_global_case_tree_display_column_THIRD);
    treewidget_case_display_obj->hideColumn(enum_global_case_tree_display_column_RECORD_COUNT);
    treewidget_case_display_obj->hideColumn(enum_global_case_tree_display_column_ROOT_COUNT_NAME);

    case_tree_widget_combobox_list_display();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}





void MainWindow::create_redefined_result_db_table_and_insert_into_index_table()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);


    QString configuration_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString).toString() + "redefined_result_filters.sqlite";
    QString command_cat = "SELECT INT,category_name FROM category";
    QStringList cat_INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_cat,0,configuration_db_path,Q_FUNC_INFO);
    QStringList cat_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_cat,1,configuration_db_path,Q_FUNC_INFO);



    for(int i_cat = 0; i_cat < cat_INT_list.size(); i_cat++)
    {
        QString tmp_INT_str = cat_INT_list.at(i_cat);
        QString tmp_category_str = cat_name_list.at(i_cat);
        tmp_category_str.replace(" ","_");

        QString redef_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + tmp_category_str + ".sqlite";
        QFileInfo info(redef_result_db_path);
        if(info.exists())
            continue;

        QString command_select = "SELECT header_name FROM result_headers WHERE category_INT = ?";
        QStringList arg_list;
        arg_list << tmp_INT_str;
        QStringList given_column_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_select,arg_list,0,configuration_db_path,Q_FUNC_INFO);

        QString selected_column_query;
        for(int i = 0; i < given_column_name_list.size(); i++)
        {
            QString colume_name = given_column_name_list.at(i);

            colume_name.replace(" ","_");
            colume_name = colume_name.toLower();

            if(colume_name == "timestamp")
                selected_column_query += colume_name + " INTEGER,";
            else
                selected_column_query += colume_name + " VARCHAR(2048),";
        }

        if(selected_column_query.endsWith(","))
            selected_column_query.remove(selected_column_query.size() - 1,1);


        QString query_create = "CREATE TABLE redefined_result(INT INTEGER PRIMARY KEY, bookmark, plugin VARCHAR(100),"
                               "tab_name VARCHAR(100), os_scheme_display VARCHAR(100), record_no VARCHAR(100),"
                               "recon_tag_value VARCHAR(100),notes VARCHAR(100),source_file VARCHAR(2048),notes_icon varchar(300), source_count_name varchar(300),"
                               "year_str varchar(100), month_year varchar(100), day_month_year varchar(100), hour_day_month_year varchar(100),LINKFILENAME VARCHAR(1000),blank_column varchar(100), ";



        query_create += selected_column_query + ")";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(query_create,redef_result_db_path,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("query create  ---FAILED---" + query_create,Q_FUNC_INFO);
        }

        QString redefine_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "index_redefined_result.sqlite";
        QString command_insert = "INSERT INTO redefined_result_index (display_name, db_name, creation_timestamp) VALUES (?,?,?)";
        arg_list.clear();
        arg_list << cat_name_list.at(i_cat) << tmp_category_str << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch());

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,redefine_index_db_path,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("query Indert Into INDEX  ---FAILED---" + query_create,Q_FUNC_INFO);
        }

    }
    recon_static_functions::app_debug("end",Q_FUNC_INFO);

}

void MainWindow::delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag , QString record_no , QString plugin_name , QString tab_name)
{
    QString current_tag_name = current_tag;
    QString tag_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QString command = "Select tag_db_name from tag_search_index";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command , 0 ,tag_index_db_path, Q_FUNC_INFO);

    for(int i = 0 ; i <  tag_db_name_list.size(); i++)
    {

        QString db_name = tag_db_name_list.at(i);
        if(db_name == current_tag_name)
            continue ;

        ///1.---delete record from previous tag db
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + db_name + ".sqlite";
        QStringList arg_list;
        command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ?";
        arg_list << record_no << plugin_name << tab_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);



        ///2.---refresh case tree tag
        common_add_tag_name_into_case_tree(db_name);

    }

}

void MainWindow::copy_configurations_in_result(QString destpath)
{
    recon_static_functions::app_debug(" : [copy_case_config_in_result]  Starts",Q_FUNC_INFO);

    QString lib_case_config = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString()  + "case_configuration";

    QString result_case_conf_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString();

    QFile::copy(lib_case_config + "/case_configuration.sqlite", result_case_conf_path + "case_configuration.sqlite");


    recon_static_functions::do_directory_blank_completely(lib_case_config, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(lib_case_config, Q_FUNC_INFO);

    QString redfn_result_filter_path_in_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString).toString();

    QString redefined_result_dir_path_in_resources = "../Resources/features_resources/redefined_result_filters";

    if(!recon_static_functions::copy_directory(redefined_result_dir_path_in_resources,redfn_result_filter_path_in_result_dir, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" : [copy_case_config_in_result]  - RECON redefine result copy dir --FAILED-- ",Q_FUNC_INFO);
    }


    recon_static_functions::app_debug(" : [copy_case_config_in_result]  Ends",Q_FUNC_INFO);
}

void MainWindow::create_feature_resources_structure()
{
    recon_static_functions::app_debug(" : [create_feature_resources_structure] - Starts",Q_FUNC_INFO);

#ifndef MACRO_RECON_STATIC_NOTARISE_ESSENTIALS
    return;
#endif

    bool bool_need_to_update_db = false;
    if(!bool_need_to_update_db)
        return;


    QDir dir;

    QString features_configuration_db_file = QString("../Resources/features_resources");
    dir.mkdir(features_configuration_db_file);
    features_configuration_db_file.append("/features_configuration.sqlite");

    QCoreApplication::processEvents();

    QFile file_feature_config(features_configuration_db_file);
    if(!file_feature_config.exists())
    {
        QString cmd_ext_mime_type = QString("CREATE TABLE tbl_mime_type(bookmark INTEGER,INT INTEGER PRIMARY KEY, category_name VARCHAR(500), mime_type VARCHAR(500))");

        QString cmd_file_signature_category = "CREATE TABLE tbl_signature_category (INT INTEGER PRIMARY KEY,category varchar(100),category_status VARCHAR(100))";
        QString cmd_file_signature_list = "CREATE TABLE tbl_signature_list (INT INTEGER PRIMARY KEY,category varchar(100),label varchar(100),header varchar(100),signature_status VARCHAR(100),offset VARCHAR(100))";
        QString cmd_tags_colours = "CREATE TABLE colors (INT INTEGER PRIMARY KEY,color_name varchar(100),color_hex varchar(100))";

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase feature_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        feature_config_db.setDatabaseName(features_configuration_db_file);

        if(feature_config_db.open())
        {
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_ext_mime_type,feature_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_file_signature_category,feature_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_file_signature_list,feature_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_tags_colours,feature_config_db,Q_FUNC_INFO);

            insert_default_values_in_features_resources_db(feature_config_db);

            feature_config_db.close();
        }
        else
        {
            recon_static_functions::app_debug("Destination Db opening features configuration --------FAILED------ " + features_configuration_db_file,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + feature_config_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug(" : [create_feature_resources_structure] - Ends",Q_FUNC_INFO);

}

void MainWindow::fill_recon_essential_info_in_narad_muni()
{
    QString rcn_lib_path = getenv("HOME") + QString("/Library/") + recon_static_functions::get_app_name().trimmed().replace(" ", "_") + "/";
    QDir mdir;
    mdir.mkpath(rcn_lib_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString,rcn_lib_path);

    QString rcn_cnfndtl_path = rcn_lib_path + "confidential/";
    mdir.mkpath(rcn_cnfndtl_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString,rcn_cnfndtl_path);


    if(bool_is_lab_running_from_root)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString()
                                               + recon_static_functions::get_recon_library_version().replace("." , "_") + QString("_Admin/"));
    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString()
                                               + recon_static_functions::get_recon_library_version().replace("." , "_") + QString("/"));
    }


    QString lib_version_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString).toString();

    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString,lib_version_parent_path + "recon_configuration/");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_App_Configuration_QString,lib_version_parent_path + "app_configuration/");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString,lib_version_parent_path + "result_configuration/");

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_App_Version_QString,recon_static_functions::get_app_version());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_Build_Version_QString,recon_static_functions::get_app_build_regular());

    ///For New Case Only(Previous versions are added while load case updation)
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_Updated_For_Versions_QString,recon_static_functions::get_app_version());


    QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
    if(!QFileInfo(agency_logo_path).exists())
    {
        QFile::copy("../icons/agency_logo.png", agency_logo_path);
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString, agency_logo_path);

}

void MainWindow::create_recon_case_result_file(QString dest_path)
{
    recon_static_functions::app_debug(" : create_recon_case_result_file - Starts",Q_FUNC_INFO);

    QDir dir;

    QString temp_path_to_loadcase = dest_path + "Result.recon/";
    dir.mkpath(temp_path_to_loadcase);

    QFile file(temp_path_to_loadcase + "caseresult");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        recon_static_functions::app_debug(" : create_recon_case_result_file - load case file open ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(" : create_recon_case_result_file error " + file.errorString(),Q_FUNC_INFO);
        return;
    }
    file.write("This file is for " + recon_static_functions::get_app_name().toLocal8Bit()  + " - [macOS] loadcase note.");
    file.flush();
    file.close();

    recon_static_functions::app_debug(" : create_recon_case_result_file - Ends",Q_FUNC_INFO);
}

void MainWindow::copy_resource_files_to_destination()
{
    recon_static_functions::app_debug(" : copy_resource_files_to_destination - Starts",Q_FUNC_INFO);

    QDir dir;
    QString dest_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "icons";
    dir.mkdir(dest_dir);

    QDirIterator iter1("../icons");
    while(iter1.hasNext())
    {
        QString temp_file_name = iter1.next();

        QFileInfo info(temp_file_name);

        QFile file(temp_file_name);
        file.copy(dest_dir + "/" + info.fileName());
    }

    dest_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Chat_View_In_Result_QString).toString();
    QFile::copy(recon_static_functions::get_appilcation_relative_path() + "/Resources/graphics/html_reports/css/chat_template.css" , dest_dir + "/chat_template.css");

    dest_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString();
    recon_static_functions::copy_directory( recon_static_functions::get_appilcation_relative_path() + "/Resources/CSVS/Plugins_CSV/",dest_dir, Q_FUNC_INFO);

    dest_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString();
    recon_static_functions::copy_directory( recon_static_functions::get_appilcation_relative_path() + "/Resources/CSVS/Features_CSV/",dest_dir, Q_FUNC_INFO);

    recon_static_functions::app_debug(" : copy_resource_files_to_destination - Ends",Q_FUNC_INFO);
}

void MainWindow::highlight_search_text(QString pattern)
{
    textedit_detailed_information->moveCursor(QTextCursor::Start);

    pattern = pattern.trimmed();

    if(pattern.size() < 1)
        return;

    QStringList patternlist;

    if(pattern.contains(","))
    {
        QStringList keyword_list;
        keyword_list = pattern.split(",", Qt::SkipEmptyParts);

        for(int i = 0; i < keyword_list.size(); i++)
        {
            QString temp_string = keyword_list.at(i);
            temp_string = temp_string.trimmed();
            if(temp_string == "")
            {
                continue;
            }

            patternlist.append(temp_string);
        }
    }
    else
        patternlist.append(pattern);

    QTextCharFormat fmt;
    fmt.setBackground(QColor("#D59B3F"));


    QTextCursor cursor(textedit_detailed_information->document());

    QString text = textedit_detailed_information->toPlainText().toLower();

    for(int i = 0; i < patternlist.size(); i++)
    {
        QString pattern = patternlist.at(i);
        cursor.setCharFormat(fmt);
        int length = pattern.size();

        int index = 0;
        while (index >= 0)
        {

            index = text.indexOf(pattern.toLower(), index);
            if(index < 0)
            {
                break;
            }

            cursor.setPosition(index, QTextCursor::MoveAnchor);
            cursor.setPosition(index + length, QTextCursor::KeepAnchor);

            index = index + length;

            cursor.setCharFormat(fmt);
        }
    }

    textedit_optical_character_recognition_information->moveCursor(QTextCursor::Start);
    QTextCursor ocr_cursor(textedit_optical_character_recognition_information->document());
    QString ocr_text = textedit_optical_character_recognition_information->toPlainText().toLower();

    for(int i = 0; i < patternlist.size(); i++)
    {
        QString pattern = patternlist.at(i);
        ocr_cursor.setCharFormat(fmt);
        int length = pattern.size();

        int index = 0;
        while (index >= 0)
        {
            index = ocr_text.indexOf(pattern.toLower(), index);
            if(index < 0)
            {
                break;
            }

            ocr_cursor.setPosition(index, QTextCursor::MoveAnchor);
            ocr_cursor.setPosition(index + length, QTextCursor::KeepAnchor);

            index = index + length;

            ocr_cursor.setCharFormat(fmt);
        }
    }

}

void MainWindow::remove_plugins_entry_from_tree(QStringList plugin_list)
{
    recon_static_functions::debug_intensive(" start ",Q_FUNC_INFO);

    for(int zz = 0; zz < plugin_list.size(); zz++)
    {
        QString plugin_name = plugin_list.at(zz);

        QTreeWidgetItemIterator m_iter(treewidget_case_display_obj);
        QList<QTreeWidgetItem*> clist ;
        while(*m_iter)
        {

            if((*m_iter)->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_CASE_TREE_Artifacts))
            {
                QString text_str = recon_static_functions::remove_record_count_from_feature_name((*m_iter)->text(enum_global_case_tree_display_column_ZERO));

                if(plugin_name == text_str)
                    clist.append(*m_iter);
            }

            ++m_iter;
        }

        if(!clist.isEmpty())
        {
            for(int count = 0; count < clist.size(); count++)
            {
                if(clist.at(count)->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_CASE_TREE_Artifacts))
                {
                    QTreeWidgetItem *item = clist.at(count);


                    /// clearing existing elements from tree
                    delete item->parent()->takeChild(item->parent()->indexOfChild(item));

                }
            }
        }

    }

    recon_static_functions::debug_intensive(" end" ,Q_FUNC_INFO);
}

void MainWindow::common_current_plugin_extraction_finished_or_cancelled(QString plugin_name)
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    QString plugins_db_dir_path;
    QString plugin_db_name = plugin_name;
    plugin_db_name = plugin_db_name.replace(" ","_");

    plugins_db_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_db_name + "/" + plugin_db_name + ".sqlite";

    if(!QFileInfo::exists(plugins_db_dir_path))
        return ;

    if(recon_helper_standard_obj->get_db_total_record_by_dbpath(false, plugins_db_dir_path, global_csv_reader_class_selected_obj->get_total_tables_list(plugin_name), Q_FUNC_INFO) <= 0)
        return;


    int enum_case_child_value = enum_global_case_tree_display_row_ARTIFACTS;
    QString Macro_plugins_type = MACRO_CASE_TREE_Artifacts;

    // common_remove_plugin_entry_from_display_tree(QStringList(plugin_name));

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->text(enum_global_case_tree_display_column_ZERO) == QString(Macro_plugins_type))
        {

            QString parent_cat = global_csv_reader_class_selected_obj->get_parent_category(plugin_name);
            bool parent_found = false;
            int parent_pos = 0;
            for(int pp = 0; pp < treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->childCount(); pp++)
            {

                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->text(enum_global_case_tree_display_column_ZERO) == parent_cat)
                {
                    parent_found  = true;
                    parent_pos = pp;
                }
            }


            if(!parent_found)
            {
                QTreeWidgetItem *item_category_of_plugins = new QTreeWidgetItem;
                item_category_of_plugins->setText(0, parent_cat);
                item_category_of_plugins->setText(1,"");
                item_category_of_plugins->setText(2, "Parent RECON Plugin");
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->addChild(item_category_of_plugins);

                for(int pp = 0; pp < treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->childCount(); pp++)
                {
                    if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->child(pp)->text(enum_global_case_tree_display_column_ZERO) == parent_cat)
                    {
                        parent_pos = pp;
                    }
                }
            }

            QTreeWidgetItem *item_plugin_name = new QTreeWidgetItem;
            QString icon_path = plugin_name;


            icon_path = "../icons/plugins/" + icon_path.replace(" ","_") + ".png";


            QStringList total_tablist =  global_csv_reader_class_selected_obj->get_total_tab_list(plugin_name);

            qint64 total_records = 0;

            QStringList total_tables_list = global_csv_reader_class_selected_obj->get_total_tables_list(plugin_name);

            for(int i = 0; i < total_tablist.size() ; i++)
            {
                if(global_csv_reader_class_selected_obj->is_tab_type_chat_view(plugin_name, total_tablist.at(i)) || global_csv_reader_class_selected_obj->is_tab_type_tree_view(plugin_name, total_tablist.at(i)) || global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(plugin_name, total_tablist.at(i)))
                    continue;


                if(global_csv_reader_class_selected_obj->is_tab_type_textedit(plugin_name, total_tablist.at(i)) || global_csv_reader_class_selected_obj->is_tab_type_stackwidget(plugin_name, total_tablist.at(i)))
                {
                    total_records += recon_helper_standard_obj->get_db_total_record_by_dbpath(false, plugins_db_dir_path, QStringList(total_tables_list.at(i)), Q_FUNC_INFO);

                }else
                {

                    total_records += recon_helper_standard_obj->get_db_total_record_by_dbpath(false, plugins_db_dir_path, QStringList(total_tables_list.at(i)), Q_FUNC_INFO);
                }

            }


            item_plugin_name->setIcon(0,QIcon(icon_path));
            item_plugin_name->setText(0, plugin_name + " (" + QString::number(total_records) + ")");
            item_plugin_name->setText(1, QString(MACRO_CASE_TREE_Artifacts));
            item_plugin_name->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            item_plugin_name->setText(enum_global_case_tree_display_column_RECORD_COUNT,QString::number(total_records));

            item_plugin_name->setDisabled(false);

            //            if(!bool_is_data_available)
            //            {
            //                item_plugin_name->setForeground(0,Qt::red);
            //            }


            bool plugin_alreay_exist = false;
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->child(parent_pos)->childCount(); j++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->child(parent_pos)->child(j)->text(enum_global_case_tree_display_column_ZERO) == QString(plugin_name + " (" + QString::number(total_records) + ")"))
                {
                    plugin_alreay_exist = true;
                    break;
                }
            }

            if(!plugin_alreay_exist)
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_case_child_value)->child(parent_pos)->addChild(item_plugin_name);

            break;
        }
    }

    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);
}

void MainWindow::common_pushbutton_status_right_corner_clicked(bool checked)
{
    if(!checked)
    {
        task_status_window_obj->hide();
        pushbutton_statusbar->setToolTip("Show Statusbar");
    }
    else
    {
        task_status_window_obj->show();
        pushbutton_statusbar->setToolTip("Hide Statusbar");
    }
}


void MainWindow::set_device_mounting()
{
    QString src_type_dsply = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Display_QString).toString();
    QString src_type_intrnl = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString();

    QString source_path1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    QString source_path2 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString).toString();
    QString password = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_RUNTIME_Root_Recovery_Passcode_Plain_QString).toString();


    image_mounter_obj->set_image_category(src_type_dsply,src_type_intrnl);

    if(image_mounter_obj->set_image_path(source_path1, source_path2, password) == -1)
    {
        image_mounter_obj->unmount_image(source_path1);
        recon_static_functions::app_debug("---FAILED-- mounting",Q_FUNC_INFO);
        recon_static_functions::app_debug("PATH1  " + source_path1,Q_FUNC_INFO);
        recon_static_functions::app_debug("PATH2  " + source_path2,Q_FUNC_INFO);
        return;
    }

    QStringList mount_path_list = image_mounter_obj->get_mount_path(source_path1);
    if(mount_path_list.size() <= 0)
    {
        recon_static_functions::app_debug("Mount path found ---FAILED---  " + source_path1,Q_FUNC_INFO);
        image_mounter_obj->unmount_image(source_path1);
        return;
    }

    insert_current_mounted_image_info_in_db();
}

void MainWindow::set_current_tree_item_highlighted(QString case_name, QString category_text , QString feature_name)
{
    recon_static_functions::app_debug(" -START " , Q_FUNC_INFO);


    recon_static_functions::app_debug(" -END " , Q_FUNC_INFO);

}

void MainWindow::delete_entry_of_note_from_case_tree()
{
}

void MainWindow::delete_empty_tags_from_case_tree_display()
{

}

QString MainWindow::find_os_scheme_if_os_is_installed_into_source(QString mount_path)
{
    recon_static_functions::app_debug(" START ",Q_FUNC_INFO);

    QString os_scheme;

    if(is_it_winos_compatible_source(mount_path))
    {
        os_scheme = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display) + MACRO_RECON_Splitter_5_resolution_scope + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal);
    }
    else if(is_it_macos_compatible_source(mount_path))
    {
        os_scheme = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display) + MACRO_RECON_Splitter_5_resolution_scope + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal);
    }

    recon_static_functions::app_debug(" END ",Q_FUNC_INFO);

    return os_scheme;
}

bool MainWindow::is_it_macos_compatible_source(QString mount_path)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    bool bool_maos_compatible = false;

    QString file1 = mount_path + "/Library/Preferences/.GlobalPreferences.plist";
    QString file2 = mount_path + "/System/Library/CoreServices/SystemVersion.plist";
    QString file3 = mount_path + "/Library/Preferences/SystemConfiguration/preferences.plist";
    QString file4 = mount_path + "/Library/Preferences/com.apple.SoftwareUpdate.plist";
    QString file5 = mount_path + "/System/Library/CoreServices/BridgeVersion.plist";
    QString file6 = mount_path + "/System/Library/CoreServices/Applications/About This Mac.app/Contents/Info.plist";


    if(QFileInfo(file1).exists() && QFileInfo(file2).exists() && QFileInfo(file3).exists())
    {
        bool_maos_compatible = true;
    }
    else if(QFileInfo(file1).exists() && QFileInfo(file3).exists() && QFileInfo(file4).exists())
    {
        bool_maos_compatible = true;
    }
    else if(QFileInfo(file2).exists() && QFileInfo(file5).exists() && QFileInfo(file6).exists())
    {
        bool_maos_compatible = true;
    }

    recon_static_functions::app_debug(" ends with " + QString::number(bool_maos_compatible),Q_FUNC_INFO);

    return bool_maos_compatible;
}

bool MainWindow::is_it_winos_compatible_source(QString mount_path)
{

    bool bool_winos_compatible = false;

    bool is_soft_exists = false, is_sam_exists = false, is_system_exists = false, is_security_exists = false;

    QString software_reg_path = mount_path + "/Windows/System32/config/SOFTWARE";
    QString sam_reg_path = mount_path + "/Windows/System32/config/SAM";
    QString system_reg_path = mount_path + "/Windows/System32/config/SYSTEM";
    QString security_reg_path = mount_path + "/Windows/System32/config/SECURITY";

    QFileInfo info_soft(software_reg_path);
    if(info_soft.exists())
        is_soft_exists = true;

    QFileInfo info_sam(sam_reg_path);
    if(info_sam.exists())
        is_sam_exists = true;

    QFileInfo info_system(system_reg_path);
    if(info_system.exists())
        is_system_exists = true;

    QFileInfo info_security(security_reg_path);
    if(info_security.exists())
        is_security_exists = true;

    if(is_soft_exists && is_sam_exists && is_system_exists && is_security_exists)
        bool_winos_compatible = true;


    return bool_winos_compatible;
}

QStringList MainWindow::get_apple_metadata_key_value_for_file_system(QString record_no, QString source_count_name)
{
    QStringList ext_attribute_list;

    QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/apple_metadata.sqlite";

    QStringList list_ext_attributes_column = global_narad_muni_class_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Custom_Column_Name_QStringList).toStringList();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(apple_metadata_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return ext_attribute_list;
    }


    QStringList values_list;
    QSqlQuery query(destination_db);


    query.prepare("select " + list_ext_attributes_column.join(",") + " from files where filesystem_record = ?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query execution ----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error-----" + query.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Command " + query.executedQuery(), Q_FUNC_INFO);
        return ext_attribute_list;
    }

    while(query.next())
    {
        for(int i = 0 ; i < list_ext_attributes_column.size() ; i++)
        {
            QString column_name = query.record().fieldName(i);

            if(column_name.contains("Date", Qt::CaseInsensitive))
            {
                values_list << global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query.value(i).toString(), Q_FUNC_INFO);
            }
            else
                values_list << query.value(i).toString();
        }

    }

    destination_db.close();

    if(list_ext_attributes_column.size() != values_list.size())
        return ext_attribute_list;


    for(int i = 0 ; i < list_ext_attributes_column.size() ; i++)
    {
        QString key = list_ext_attributes_column.at(i);
        QString value = values_list.at(i);

        key.remove(key.indexOf("_"),key.size());

        if(value.trimmed().isEmpty())
            continue;

        ext_attribute_list << QString(key + MACRO_RECON_Splitter_1_multiple + value);
    }


    return ext_attribute_list;
}

void MainWindow::add_current_record_details_from_content_search_to_tag_search_db(QString tag_name,QString plugin_name,QString tab_name,QString record_no,QString applied_tag_str , QString index_db_path, QString os_scheme)
{
    // Create DB for every Tags
    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);


    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no << os_scheme;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        insert_record_entry_into_tag_search_db_for_content_search(record_no, plugin_name, tab_name, "",tag_db_path, index_db_path, os_scheme ,tag_name);
    }
    else
    {
        QString command_update = "UPDATE tags SET recon_tag_value=? WHERE INT=?";
        QStringList arg_list;
        arg_list << tag_name << INT_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);
    }
}

void MainWindow::add_tag_in_database_for_screenshots(QString tag_name , QString is_emitted_from_plugin, QString plugin_name_str, QString tab_name_str, QString record_no, QString dest_path,  QString os_name)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(dest_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + dest_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,notes,item0,item1,item2 FROM tags WHERE INT=?");
    query.addBindValue(record_no);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }
    if(!query.next())
    {
        recon_static_functions::app_debug(" --- No Record Exist---",Q_FUNC_INFO);
        return;
    }

    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=?";
    QStringList arg_list_for_tags;

    arg_list_for_tags << plugin_name_str << tab_name_str << record_no;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QStringList arg_list;
        QString command_insert = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,bookmark,csv_plugin_name,csv_tab_name,"
                                 "notes,os_scheme_display,source_count_name,item0,item1,item2) VALUES (?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?)";

        arg_list << QString(MACRO_Plugin_Name_Screenshots) << plugin_name_str << tab_name_str << "screenshots"
                 << record_no << query.value(1).toString().trimmed() << "0"<< QString(MACRO_Plugin_Name_Screenshots) << QString(MACRO_Plugin_Name_Screenshots)
                 << "" << os_name << ""
                 << query.value(3).toString().trimmed() << query.value(4).toString().trimmed() << global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(5).toString().trimmed(), Q_FUNC_INFO);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);

        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_Screenshots) , QString(MACRO_Plugin_Name_Screenshots) , tag_name, os_name);

    }else
    {

        QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE INT=?";
        QStringList arg_list;
        arg_list << tag_name << INT_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);

    }

    source_db.close();
    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

bool MainWindow::is_sumuri_live_imager_plugin_list_file_present(QString mount_path)
{
    // MDFJDFDF3454JKK89K
    QFileInfo info(mount_path + "MDFJDFDF3454JKK89K");
    if(info.exists())
    {
        // recon_static_functions::app_debug("Is it Live Image? --->>> YES", Q_FUNC_INFO);
        return true;
    }
    else
    {
        //   recon_static_functions::app_debug("Is it Live Image? --->>> NO", Q_FUNC_INFO);
        return false;
    }

    return false;
}

void MainWindow::check_and_update_services_db_for_recon_lab()
{
    QString user_name = qgetenv("USER");
    if (user_name.isEmpty())
        user_name = qgetenv("USERNAME");

    user_name = user_name.trimmed();

    if(user_name.isEmpty())
        return;

    QString destination_db_file = "/Users/" + user_name + "/Library/Application Support/com.apple.TCC/TCC.db";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return ;
    }


    QSqlQuery query(destination_db);

    QStringList services_list;
    services_list << "kTCCServiceAddressBook" << "kTCCServiceReminders"  << "kTCCServiceCalendar";

    QString app_name = QString(MACRO_APP_NAME_FROM_INFO_PLIST);

    QString command_entry_exist = QString("Select allowed From access Where service = ?  AND client = ?");
    QString command_update = QString("Update access Set client_type = ?,allowed = ?,prompt_count = ? Where service = ?  AND client = ?");
    QString command_insert = QString("Insert into access (service,client,client_type,allowed,prompt_count) Values (?,?,?,?,?)");

    for(int count = 0; count < services_list.size(); count++)
    {
        /// Check entry exist
        query.prepare(command_entry_exist);
        query.addBindValue(services_list.at(count));
        query.addBindValue(app_name);

        if(!query.exec())
        {
            recon_static_functions::app_debug(" query(Select)  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
            continue;
        }


        /// if exist then update
        if(query.next())
        {
            query.prepare(command_update);

            query.addBindValue(QString("0"));
            query.addBindValue(QString("1"));
            query.addBindValue(QString("1"));

            query.addBindValue(services_list.at(count));
            query.addBindValue(app_name);

            if(!query.exec())
            {
                recon_static_functions::app_debug(" query(Update)  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }
        else
        {
            /// if not exist then insert

            query.prepare(command_insert);

            query.addBindValue(services_list.at(count));
            query.addBindValue(app_name);
            query.addBindValue(QString("0"));
            query.addBindValue(QString("1"));
            query.addBindValue(QString("1"));

            if(!query.exec())
            {
                recon_static_functions::app_debug(" query(Insert)  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }

    }

    destination_db.close();

}


void MainWindow::loading_display_dialog_based_on_records(qint64 total_records, qint64 processed_count)
{
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

}

void MainWindow::unmount_previously_mounted_paths_from_home()
{

    //    QStringList mounted_media_list = global_recon_helper_singular_class_obj->get_mounted_media_list(Q_FUNC_INFO);
    //    QStringList args_list;
    //    for(int count = 0; count < mounted_media_list.size(); count++)
    //    {
    //        QString path_to_unmount = mounted_media_list.at(count);
    //        if(path_to_unmount.contains("/RECON_TMP/"))
    //        {
    //            args_list.clear();
    //            args_list << "unmount" << "force" << path_to_unmount;
    //            QProcess process_mount_cmd;
    //            process_mount_cmd.start("diskutil", args_list);

    //            process_mount_cmd.waitForFinished();
    //        }
    //    }

    //    QString target_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString();

    //    QDirIterator iter(target_dir, QDirIterator::NoIteratorFlags);

    //    while(iter.hasNext())
    //    {
    //        QString f_path = iter.next();

    //        QFileInfo m_info(f_path);

    //        if(m_info.fileName() == "." || m_info.fileName() == "..")
    //            continue;

    //        if(m_info.isDir())
    //        {
    //            if(!global_recon_helper_singular_class_obj->is_it_mounted_path(f_path, Q_FUNC_INFO))
    //                continue;

    //            args_list.clear();
    //            args_list << "unmount" << "force" << f_path;
    //            QProcess process_mount_cmd;
    //            process_mount_cmd.start("diskutil", args_list);

    //            process_mount_cmd.waitForFinished();

    //        }
    //    }


    //    QString tmp_dir = QString(MACRO_RECON_Temp_DirPath_In_Bundle);
    //    if(QFileInfo(tmp_dir).exists())
    //    {
    //        QDirIterator iter(tmp_dir, QDirIterator::NoIteratorFlags);
    //        while(iter.hasNext())
    //        {
    //            QFile(iter.next()).remove();
    //        }
    //    }

}


void MainWindow::set_narad_and_essentials_for_language_translater()
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_User_Selected_Language_int,enum_global_language_translater_english);

    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_User_Selected_Language_int,enum_global_language_translater_original);
    //    if(QFile("../Resources/ln_english").exists())
    //    {
    //        recon_static_functions::app_debug("language translater ENGLISH found", Q_FUNC_INFO);
    //    }
    //    else if(QFile("../Resources/ln_chinese").exists())
    //    {
    //        recon_static_functions::app_debug("language translater CHINESE found", Q_FUNC_INFO);
    //        global_narad_muni_class_obj->set_field(MACRO_NARAD_User_Selected_Language_int,enum_global_language_translater_chinese);
    //    }
    //    else
    //    {
    //        recon_static_functions::app_debug(" ---FAILED--- language translater resources file Not found", Q_FUNC_INFO);
    //    }

    global_lang_tr_obj.set_essentials();

}

void MainWindow::prepare_available_hashsets_data_list()
{
    available_hashset_data_list.clear();

    QString hashset_conf_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString() + "hashset_index.sqlite";

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase db_hashset_indexes = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    db_hashset_indexes.setDatabaseName(hashset_conf_index_db_path);
    if(!db_hashset_indexes.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + hashset_conf_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + db_hashset_indexes.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query_select(db_hashset_indexes);

    query_select.prepare("Select saved_table_name,md5_hash_column, recon_filefrom,recon_file_infopath,records_count from hashset_indexes Where show_files = ?");

    query_select.addBindValue(QString(MACRO_TRUE_VALUE_NUMERIC_QString));

    if(!query_select.exec())
    {
        recon_static_functions::app_debug("query execute -----FAILED----" + query_select.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + query_select.lastError().text(), Q_FUNC_INFO);
        db_hashset_indexes.close();
        return;
    }

    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    int count = 0;

    while(query_select.next())
    {

        count++;
        if(count % 10 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        struct_global_available_hashset_data hashset_data_obj;

        hashset_data_obj.saved_table_name = query_select.value("saved_table_name").toString().trimmed();
        hashset_data_obj.md5_hash_column  = query_select.value("md5_hash_column").toString().trimmed();

        hashset_data_obj.recon_filefrom = query_select.value("recon_filefrom").toString().trimmed();
        hashset_data_obj.recon_file_infopath = query_select.value("recon_file_infopath").toString().trimmed();

        hashset_data_obj.hashes_db_path = hashset_data_obj.recon_file_infopath;
        if(hashset_data_obj.recon_filefrom == recon_file_from_result)
            hashset_data_obj.hashes_db_path = result_dir_path + hashset_data_obj.recon_file_infopath;

        hashset_data_obj.records_count = query_select.value("records_count").toString().trimmed().toLongLong();

        available_hashset_data_list.append(hashset_data_obj);
    }
    db_hashset_indexes.close();
}


void MainWindow::slot_add_all_available_hashset_in_case_tree()
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    ///remove widget from feature comboboxes
    int tree_count = 0;
    int item_count = 0;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        tree_count++;
        if(tree_count % 10 == 0)
        {
            tree_count = 0;
            QCoreApplication::processEvents();
        }

        QTreeWidgetItem *item_hashset_parent = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET);
        if(item_hashset_parent->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Hashset))
        {
            for(int jj = 0; jj < item_hashset_parent->childCount(); jj++)
            {
                item_count++;
                if(item_count % 5 == 0)
                {
                    item_count = 0;
                    QCoreApplication::processEvents();
                }

                QTreeWidgetItem *item_hashset_child =  item_hashset_parent->child(jj);

                QString current_tab_name = item_hashset_child->text(enum_global_case_tree_display_column_ZERO);
                QString tab_text_str     = current_tab_name.mid(0,current_tab_name.indexOf("(")).trimmed();

                QString category      = item_hashset_child->text(enum_global_case_tree_display_column_FIRST);
                QString unique_string = category + MACRO_RECON_Splitter_1_multiple + tab_text_str;

                right_click_remove_widget_by_unique_name(unique_string, tab_text_str);
            }
        }
    }

    ///remove all previous items
    int item_remove_count = 0;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        item_remove_count++;
        if(item_remove_count % 5 == 0)
        {
            item_remove_count = 0;
            QCoreApplication::processEvents();
        }

        qDeleteAll(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->takeChildren());
    }

    prepare_available_hashsets_data_list();

    QString sources_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
    QString cmd_already_run = QString("Select source_count_name From fs_status");

    QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString(cmd_already_run), 0, sources_conf_db_path, Q_FUNC_INFO);
    source_count_name_list.removeAll("");
    ///update record count
    tree_count = 0;
    item_count = 0;
    for(int count = (available_hashset_data_list.size() -1); count >= 0; count--)
    {
        tree_count++;
        if(tree_count % 10 == 0)
        {
            tree_count = 0;
            QCoreApplication::processEvents();
        }

        struct_global_available_hashset_data hashset_data_obj = available_hashset_data_list.takeAt(count);

        for(int count = 0; count < source_count_name_list.size(); count++)
        {
            item_count++;
            if(item_count % 5 == 0)
            {
                item_count = 0;
                QCoreApplication::processEvents();
            }

            QString hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_list.at(count) + "/hashes.sqlite";

            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(hashes_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + hashes_db_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            QSqlQuery query_index(destination_db);

            QString hashset_db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashsetDB")
                    .arg(QDir::toNativeSeparators(hashset_data_obj.hashes_db_path));
            query_index.exec(hashset_db_attach_commmand);

            QString cmd_str = QString("Select count(*) From main.files as a join hashsetDB.") + hashset_data_obj.saved_table_name + (" as b on a.md5_hash=b.") + hashset_data_obj.md5_hash_column;
            QString record_cnt_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd_str, 0, destination_db, Q_FUNC_INFO);
            record_cnt_str = record_cnt_str.trimmed();
            if(!record_cnt_str.isEmpty())
            {
                hashset_data_obj.records_count += record_cnt_str.toLongLong();
            }

            QString hashset_db_detach_commmand = QString("DETACH DATABASE hashsetDB");
            query_index.exec(hashset_db_detach_commmand);

            destination_db.close();
        }

        available_hashset_data_list.insert(count, hashset_data_obj);
    }

    add_griffeye_data_category_according_to_available_hashset_list();
    /// create new tree items
    QList <QTreeWidgetItem*>hashset_tree_items_list;
    item_count = 0;
    for(int count = 0; count < available_hashset_data_list.size(); count++)
    {
        item_count++;
        if(item_count % 5 == 0)
        {
            item_count = 0;
            QCoreApplication::processEvents();
        }

        struct_global_available_hashset_data hashset_data_obj = available_hashset_data_list.at(count);

        QString tmp_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(hashset_data_obj.hashes_db_path);

        tmp_str.remove(".sqlite");
        tmp_str = tmp_str.trimmed();

        QString records_cnt_str = QString::number(hashset_data_obj.records_count);

        QTreeWidgetItem *sub_child = new QTreeWidgetItem;
        QString title_str = tmp_str + " (" + records_cnt_str + ")";

        sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
        sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/hashset.png"));
        sub_child->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Hashset));
        sub_child->setText(enum_global_case_tree_display_column_THIRD, hashset_data_obj.hashes_db_path);
        sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_cnt_str);

        hashset_tree_items_list.append(sub_child);
    }

    ///add items in case tree
    tree_count = 0;
    item_count = 0;
    if(hashset_tree_items_list.size() > 0)
    {
        for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
        {
            tree_count++;
            if(tree_count % 5 == 0)
            {
                tree_count = 0;
                QCoreApplication::processEvents();
            }

            if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Hashset))
            {
                for(int count = 0; count < hashset_tree_items_list.size(); count++)
                {
                    item_count++;
                    if(item_count % 5 == 0)
                    {
                        item_count = 0;
                        QCoreApplication::processEvents();
                    }

                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->addChild(hashset_tree_items_list.at(count));
                }
            }
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}

void MainWindow::create_and_fill_mounting_path_in_narad()
{
    QString user_name_dir_path = global_variable_private_var_tmp_Dir_Path_Case_QString;

    QDir mount_path_dir;


    //Source_mount_Path_Dir
    QString images_mount_path = user_name_dir_path + "RECON_mount_path" + "/";
    mount_path_dir.mkpath(images_mount_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString,images_mount_path);


    //Fuse_Path_Dir
    QString fuse_mount_path =user_name_dir_path + "fuse_path" + "/";
    mount_path_dir.mkpath(fuse_mount_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString,fuse_mount_path);


    //APFS_Snapshot_dir_path
    QString apfs_snapshot_mount_path =user_name_dir_path + "apfs_snapshots" + "/";
    mount_path_dir.mkpath(apfs_snapshot_mount_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Mount_Point_APFS_Snapshot_Dir_QString,apfs_snapshot_mount_path);


    //Dummy_Root_Dir_path
    QString soft_root_path = user_name_dir_path + "soft_root" + "/";
    mount_path_dir.mkpath(soft_root_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Mount_Point_Dummy_Root_QString,soft_root_path);


    QString shadow_file_path = user_name_dir_path + "shadow_file" + "/";
    mount_path_dir.mkpath(shadow_file_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Mount_Point_Shadow_File_QString,shadow_file_path);

    //full_disk_access_path
    QString full_disk_access_path = user_name_dir_path + "full_disk_access" + "/";
    mount_path_dir.mkpath(full_disk_access_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_VAR_TMP_Point_Full_Disk_Access_QString,full_disk_access_path);
}

void MainWindow::update_snapshot_mount_path_for_load_case()
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int rr = 0; rr < list_source_info.size(); rr++)
    {
        struct_GLOBAL_witness_info_source st_source =  list_source_info.at(rr);

        QList<struct_GLOBAL_snapshots_info> snp_Qlist = st_source.snapshots_QList;
        if(snp_Qlist.isEmpty())
            continue;

        for(int ss =0; ss < snp_Qlist.size(); ss++)
        {
            struct_GLOBAL_snapshots_info snp_obj = snp_Qlist.at(ss);

            if(snp_obj.fs_run_status != "1")
                continue;

            QString selected_snapshot_str = snp_obj.snapshot_actual_name;
            /// mount selected snapshot
            QString snapshot_mount_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_APFS_Snapshot_Dir_QString).toString();

            QDir tmp_dir;
            tmp_dir.mkpath(snapshot_mount_dir);

            QString available_mount_point = recon_static_functions::get_available_filename("snapshot_mount", snapshot_mount_dir, Q_FUNC_INFO);

            QString snapshot_mount_path = snapshot_mount_dir + available_mount_point;

            QDir mdir;
            mdir.mkpath(snapshot_mount_path);

            // mount -t apfs -o -s=com.apple.TimeMachine.2019-03-15-124935 / ~/tmp/snap_mount/
            QStringList args_list;

            args_list << "-t" << MACRO_FILE_SYSTEM_TYPE_APFS << "-o" << QString("-s=") + selected_snapshot_str << st_source.virtual_source_path << snapshot_mount_path ;
            recon_helper_process_obj->run_command_with_arguments("mount", args_list, Q_FUNC_INFO);

            if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(snapshot_mount_path, Q_FUNC_INFO))
                continue;

            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
            {
                snapshot_mount_path = recon_helper_standard_obj->get_new_mount_path_of_time_machine_backup_image_and_snapshots(snapshot_mount_path,QString(MACRO_Mount_Path_Time_Machine_Backup_Image_Snapshots));
            }

            snp_obj.snapshot_mount_path = snapshot_mount_path;

            snp_Qlist.removeAt(ss);
            snp_Qlist.insert(ss,snp_obj);

        }

        st_source.snapshots_QList = snp_Qlist;

        list_source_info.removeAt(rr);
        list_source_info.insert(rr,st_source);
    }

    global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info);

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

void MainWindow::unmount_previous_images_according_to_mount_output()
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QProcess proc_mount;
    proc_mount.start("mount");
    proc_mount.waitForFinished(6000);

    QStringList all_media_list = QString::fromLocal8Bit(proc_mount.readAll()).split("\n");

    for(int ii = 0; ii < all_media_list.size(); ii++)
    {
        QString line_str = all_media_list.at(ii);
        if(line_str.contains(global_variable_private_var_tmp_Dir_Path_Parent_QString))
        {
            int m_index = line_str.lastIndexOf(" (");
            if(m_index == -1)
                continue;

            line_str = line_str.remove(m_index, line_str.size());

            QStringList tmp_list = line_str.split(" on /");
            if(tmp_list.size() < 2)
                continue;

            QString disk_mount_path = tmp_list.at(1).trimmed();
            disk_mount_path.prepend("/");

            if(QFile(disk_mount_path).exists())
            {
                QStringList args_list;
                args_list << "umount" << "force" << disk_mount_path;
                run_command_generic("diskutil" , args_list);
            }


            if(QFile(disk_mount_path).exists())
            {
                QString disk_node = tmp_list.at(0).trimmed();

                QStringList args_list;
                args_list  << disk_node;
                run_command_generic("umount" , args_list);
            }

        }
    }

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

}

void MainWindow::unlink_previous_mounted_softlinks()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QStringList soft_link_path_list = global_recon_helper_singular_class_obj->get_softlink_path(global_variable_private_var_tmp_Dir_Path_Parent_QString, Q_FUNC_INFO);

    for(int jj = 0; jj < soft_link_path_list.size(); jj++)
    {
        QString soft_link_path = soft_link_path_list.at(jj);

        if(QFile(soft_link_path).exists())
        {
            run_command_generic("unlink",QStringList(soft_link_path));
        }
    }
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}


void MainWindow::prepare_feature_export_case_essentials()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString conf_path_case_config = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    ////Internet History
    QString cmd = "SELECT name from plugins where category = ?";
    QStringList values;
    values << "Web Browsers";
    QStringList browsers_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Internet_History , browsers_list);

    ////Messengers
    cmd = "SELECT name from plugins where category = ?";
    values.clear();
    values << "Messenger";
    QStringList messenger_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Messages , messenger_list);

    ////Email Artifacts
    cmd = "SELECT name from plugins where category = ?";
    values.clear();
    values << "Email Artifacts";
    QStringList email_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Emails , email_list);

    ////Remaining Plugins List
    cmd = "SELECT name from plugins where category != ? AND category != ? AND category != ?";
    values.clear();
    values << "Email Artifacts" << "Messenger" << "Web Browsers";
    QStringList other_artifacts_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Other_Artifacts , other_artifacts_list);

    ////Images
    QStringList images_ext_list;

    cmd = "SELECT extension_name from extension_list where category_name = ?";
    values.clear();
    values << MACRO_Export_Case_Category_Image;
    images_ext_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Image , images_ext_list);

    ////Multimedia
    QStringList multimedia_list;

    cmd = "SELECT extension_name from extension_list where category_name = ? OR category_name = ?";
    values.clear();
    values << "Audio" << "Video";
    multimedia_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Multimedia , multimedia_list);

    ////Documents
    QStringList documents_list;

    cmd = "SELECT extension_name from extension_list where category_name = ?";
    values.clear();
    values << MACRO_Export_Case_Category_Document;
    documents_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Document , documents_list);

    ////Tagged Items
    QStringList tags_list ;
    tags_list << QString(MACRO_Tag_Name_Bookmarks);

    cmd = "SELECT tag_name from tags";
    tags_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd, 0, conf_path_case_config, Q_FUNC_INFO);
    fill_export_case_status_database(MACRO_Export_Case_Category_Tags , tags_list);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void MainWindow::fill_export_case_status_database(QString category_name , QStringList list_items)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    for(int ii = 0; ii < list_items.size(); ii++)
    {
        QString insert_cmd = "INSERT into tbl_export_status (category_name, list_name, check_status) VALUES(?,?,?)";
        QStringList values;

        int check_status = 1;
        if(category_name == QString(MACRO_Export_Case_Category_Other_Artifacts))
        {
            check_status = 0;
        }

        values << category_name << list_items.at(ii) << QString::number(check_status);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, export_status_db_path, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void MainWindow::add_griffeye_data_category_according_to_available_hashset_list()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(available_hashset_data_list.size() < 0)
        return;

    for(int count = 0; count < available_hashset_data_list.size(); count++)
    {
        struct_global_available_hashset_data hashset_data_obj = available_hashset_data_list.at(count);

        QString hashes_db_path = QString(hashset_data_obj.hashes_db_path);

        QString select_cmd = QString("SELECT md5_hash, griffeye_category FROM saved_hashsets WHERE griffeye_category IS NOT NULL");

        QStringList md5_hash_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd,0,hashes_db_path,Q_FUNC_INFO);
        QStringList griffeye_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd,1,hashes_db_path,Q_FUNC_INFO);

        if(!griffeye_category_list.isEmpty())
        {
            add_rec_with_category_name_into_griffeye_db(griffeye_category_list,md5_hash_list);
        }
    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

}

void MainWindow::add_rec_with_category_name_into_griffeye_db(QStringList griffeye_category, QStringList md5_str)
{
    QString sources_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
    QString cmd_already_run = QString("Select source_count_name From fs_status");

    QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString(cmd_already_run), 0, sources_conf_db_path, Q_FUNC_INFO);

    source_count_name_list.removeAll("");
    QString filesystem_record;
    QStringList griffeye_data_db_path_list;
    QStringList griffeye_category_list;
    griffeye_category_list << griffeye_category;

    griffeye_category_list.removeAll("");
    if(griffeye_category_list.size() < 1)
        return;

    for(int kk = 0; kk < source_count_name_list.size(); kk++)
    {
        QString griffeye_data_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + source_count_name_list.at(kk) + "/griffeye_data.sqlite";

        griffeye_data_db_path_list << griffeye_data_db_path;
        griffeye_data_db_path_list.removeDuplicates();


        QString hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_list.at(kk) + "/hashes.sqlite";
        for(int kk = 0; kk < griffeye_category_list.size(); kk++)
        {
            if(griffeye_category_list.at(kk).trimmed().isEmpty())
                continue;

            QString command = "SELECT filesystem_record FROM files WHERE md5_hash = ?";
            QStringList arg_list;
            arg_list << md5_str.at(kk);
            filesystem_record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,hashes_db_path,Q_FUNC_INFO);

            if(filesystem_record.isEmpty())
            {
                continue;
            }

            QString insert_command = QString("INSERT INTO files (filesystem_record, md5_hash, griffeye_category) VALUES(?,?,?)");

            QStringList arg_value_list;
            arg_value_list << filesystem_record << md5_str.at(kk) << griffeye_category.at(kk);
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_command,arg_value_list,griffeye_data_db_path,Q_FUNC_INFO);
        }
    }


    common_add_category_name_into_case_tree_griffeye(griffeye_category_list,griffeye_data_db_path_list);

}


void MainWindow::common_add_category_name_into_case_tree_griffeye(QStringList category_name_list, QStringList griffeye_db_path_list)
{
    recon_static_functions::debug_intensive(" start", Q_FUNC_INFO);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(ii % 100 == 0)

            QCoreApplication::processEvents();

        QTreeWidgetItem *item_griffeye = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_GRIFFEYE_DATA);

        if(item_griffeye->text(0) == QString(MACRO_CASE_TREE_Griffeye_Data_Parent))
        {
            QString item_text_with_records;

            for(int jj = 0; jj < category_name_list.size(); jj++)
            {
                if(jj % 100 == 0)

                    QCoreApplication::processEvents();

                QString final_rec_count;

                QString griffeye_type_str = category_name_list.at(jj);
                if(griffeye_type_str.trimmed().isEmpty())
                    continue;

                qint64 existing_records_for_display = 0;

                bool bool_is_child_found = false;

                int child_position = -1;

                for(int kk = 0; kk < item_griffeye->childCount(); kk++)
                {
                    if(kk % 100 == 0)

                        QCoreApplication::processEvents();

                    QTreeWidgetItem *child_item = item_griffeye->child(kk);

                    if(child_item->text(0).startsWith(griffeye_type_str))
                    {
                        child_position = kk;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_griffeye->childCount())
                    {
                        QTreeWidgetItem *child = item_griffeye->child(child_position);

                        QString existing_records_str = child->text(enum_global_case_tree_display_column_RECORD_COUNT).trimmed();

                        existing_records_for_display = existing_records_str.toInt();

                        QString cat_child_txt = child->text(enum_global_case_tree_display_column_ZERO);
                        cat_child_txt = cat_child_txt.remove(cat_child_txt.indexOf(" ("), cat_child_txt.indexOf(")"));

                        slot_remove_entry_file_type_based_in_stack_and_refresh_widget(child->text(enum_global_case_tree_display_column_FIRST), cat_child_txt);
                    }
                }

                int rec_cnt = 0 ;

                for(int ll = 0; ll < griffeye_db_path_list.size(); ll++)
                {
                    QString cmd_get_records = "SELECT count(*) from files where griffeye_category = ? ";
                    rec_cnt += recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_get_records,QStringList(griffeye_type_str),0,griffeye_db_path_list.at(ll),Q_FUNC_INFO).toInt();
                }

                final_rec_count = QString::number(rec_cnt);

                item_text_with_records.clear();

                if(final_rec_count.isEmpty())
                {
                    item_text_with_records =  griffeye_type_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  griffeye_type_str + QString(" (") + final_rec_count + QString(")");
                }

                if(bool_is_child_found)
                {
                    if(child_position >= 0 && child_position < item_griffeye->childCount())
                    {
                        QTreeWidgetItem *child_item = item_griffeye->child(child_position);

                        child_item->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                        child_item->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/griffeye.png"));
                        child_item->setText(enum_global_case_tree_display_column_RECORD_COUNT, final_rec_count);
                    }
                }
                else
                {
                    QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                    child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                    child_item_second->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/griffeye.png"));
                    child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Griffeye_Data_Parent));
                    child_item_second->setText(enum_global_case_tree_display_column_SECOND, global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                    child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, final_rec_count);
                    item_griffeye->addChild(child_item_second);
                }
            }
        }

        break;
    }
    //==============


    recon_static_functions::debug_intensive(" end", Q_FUNC_INFO);
}

bool MainWindow::get_macos_appearence_mode()
{
    recon_static_functions::app_debug(" Start ",Q_FUNC_INFO);

    QStringList args;
    args << "read" << "-g" << "AppleInterfaceStyle";

    QProcess prc;
    prc.start("defaults", args);
    prc.waitForFinished();
    while(1)
    {
        QCoreApplication::processEvents();
        if(prc.state() == QProcess::NotRunning)
        {
            break;
        }
    }

    QString macOS_mode_str = QString::fromLocal8Bit(prc.readAll().trimmed());
    bool is_mac_light_mode = false;
    if(macOS_mode_str == "Dark")
    {
        is_mac_light_mode = false;
    }
    else
    {
        is_mac_light_mode = true;
    }

    recon_static_functions::app_debug("End ",Q_FUNC_INFO);

    return is_mac_light_mode;
}

bool MainWindow::get_macos_desktop_processor_chip_arch_type()
{
    recon_static_functions::app_debug(" Start ",Q_FUNC_INFO);

    QProcess prc;
    prc.start("arch");
    prc.waitForFinished();
    while(1)
    {
        QCoreApplication::processEvents();
        if(prc.state() == QProcess::NotRunning)
        {
            break;
        }
    }

    QString arch_type = QString::fromLocal8Bit(prc.readAll().trimmed());
    bool is_desktop_silicon = false;
    if(arch_type == "arm64")
    {
        is_desktop_silicon = true;
    }
    else
    {
        is_desktop_silicon = false;
    }

    recon_static_functions::app_debug("End ",Q_FUNC_INFO);

    return is_desktop_silicon;
}

bool MainWindow::validate_full_disk_access_functionality()
{
    QString tcc_db_system_path = QString("/Library/Application Support/com.apple.TCC/TCC.db");

    QFile source_file(tcc_db_system_path);
    if(!source_file.exists())
    {
        return false;
    }

    QString tcc_db_adhoc_path = copy_tcc_db_to_adhoc_path(tcc_db_system_path);

    bool bool_full_disk_access = check_full_disk_access(tcc_db_system_path, tcc_db_adhoc_path);

    return bool_full_disk_access;
}

QString MainWindow::copy_tcc_db_to_adhoc_path(QString source_db_path)
{
    QString full_disk_access_adhoc_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_VAR_TMP_Point_Full_Disk_Access_QString).toString();

    QString dest_tcc_db_path = recon_helper_standard_obj->get_adhoc_path_for_database_using_cp_without_sudo(source_db_path, full_disk_access_adhoc_dir_path);

    if(!QFileInfo::exists(dest_tcc_db_path))
    {
        struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
        QString system_pswd = pwd_obj.desktop_password_original;

        dest_tcc_db_path = recon_helper_standard_obj->get_adhoc_path_for_database_using_cp_with_sudo(source_db_path, system_pswd, full_disk_access_adhoc_dir_path, Q_FUNC_INFO);
    }

    return dest_tcc_db_path;
}

bool MainWindow::check_full_disk_access(QString source_db_path, QString tcc_db_adoch_path)
{
    bool bool_full_disk_access = false;

    //This for internal use only, so full disk access popup will not disturb at development level
    QDir m_dir;
    if(!m_dir.exists("../_CodeSignature"))
    {
        return true;
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    QFile tcc_db_adoch_file(tcc_db_adoch_path);

    if(tcc_db_adoch_file.exists())
    {
        source_db.setDatabaseName(tcc_db_adoch_path);

        if(!source_db.open())
        {
            recon_static_functions::app_debug("Temporary TCC Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error - " + source_db.lastError().text() , Q_FUNC_INFO);

            source_db.setDatabaseName(source_db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("System TCC Database open 1 ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error - " + source_db.lastError().text() , Q_FUNC_INFO);
                return bool_full_disk_access;
            }
        }
    }
    else
    {
        source_db.setDatabaseName(source_db_path);

        if(!source_db.open())
        {
            recon_static_functions::app_debug("System TCC Database open 2 ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error - " + source_db.lastError().text() , Q_FUNC_INFO);
            return bool_full_disk_access;
        }
    }

    QSqlQuery query_select(source_db);

    query_select.prepare("select client,csreq from access where service = ?");
    query_select.addBindValue("kTCCServiceSystemPolicyAllFiles");

    if(!query_select.exec())
    {
        recon_static_functions::app_debug("select query execute ----FAILED---- " + query_select.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        source_db.commit();
        source_db.close();
        return bool_full_disk_access;
    }

    while(query_select.next())
    {
        QString client = query_select.value("client").toString();

        QByteArray csreq = query_select.value("csreq").toByteArray();


        if(client.endsWith("RECON-LAB") && csreq.contains("M2UAN8S5M3"))
        {
            bool_full_disk_access = true;
            break;
        }
    }

    source_db.commit();
    source_db.close();
    return bool_full_disk_access;
}

void MainWindow::show_message_widget_for_full_disk_access()
{
    QWidget *message_widget_full_disk_access = new QWidget();
    message_widget_full_disk_access->setFixedHeight(200);
    message_widget_full_disk_access->setFixedWidth(650);
    message_widget_full_disk_access->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    message_widget_full_disk_access->setWindowTitle("RECON LAB - Full Disk Access");
    //    message_widget_full_disk_access->setContentsMargins(0,0,0,0);


    QLabel *label_message_full_disk_access = new QLabel(message_widget_full_disk_access);
    QString text_message_full_disk_access = QString("            RECON LAB has detected that Full Disk Access has not been granted to the application. To grant\n"
                                                    "\tFull Disk Access, Use the following menu path and click the checkbox next to RECON LAB:\n\n"
                                                    "\t\t\t AppleMenu > Security and Privacy > Full Disk Access\n\n"
                                                    "            Note: In order to update RECON LAB's Full Disk Access, The application will require a restart.");

    label_message_full_disk_access->setText(text_message_full_disk_access);

    QPushButton *pushButton_okay_full_disk_access = new QPushButton(message_widget_full_disk_access);
    connect(pushButton_okay_full_disk_access,SIGNAL(clicked(bool)),this,SLOT(slot_pushbutton_okay_full_disk_access_clicked(bool)));
    pushButton_okay_full_disk_access->setText("Okay");
    pushButton_okay_full_disk_access->setMaximumWidth(60);

    QSpacerItem *horizontal_spacer;
    horizontal_spacer = new QSpacerItem(QSizePolicy::Minimum, QSizePolicy::Expanding);

    QHBoxLayout *hlayout_pushbutton_okay = new QHBoxLayout();
    hlayout_pushbutton_okay->addItem(horizontal_spacer);
    hlayout_pushbutton_okay->addWidget(pushButton_okay_full_disk_access);
    hlayout_pushbutton_okay->addItem(horizontal_spacer);


    QVBoxLayout* vertical_layout = new QVBoxLayout(message_widget_full_disk_access);
    vertical_layout->addWidget(label_message_full_disk_access);
    vertical_layout->addLayout(hlayout_pushbutton_okay);
    vertical_layout->setContentsMargins(0,0,0,10);

    message_widget_full_disk_access->show();
    message_widget_full_disk_access->activateWindow();
}

void MainWindow::slot_pushbutton_okay_full_disk_access_clicked(bool click)
{
    recon_static_functions::do_directory_blank_completely(global_variable_private_var_tmp_Dir_Path_Parent_QString, Q_FUNC_INFO);

    exit(0);
}
