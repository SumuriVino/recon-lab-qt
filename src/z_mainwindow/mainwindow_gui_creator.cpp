#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::draw_complete_gui()
{

    create_navigation_combobox_bar();
    set_layout_tree_widgets();
    set_layout_1_main_tree_layout_and_case_tab();
    create_file_previewer_tabs();
    set_layout_2_attch_previewer_tab_with_layout_1();

}

void MainWindow::slot_tabwidget_metadata_viewer_tab_changed(int index)
{
    set_metadata_information();
}

void MainWindow::create_navigation_combobox_bar()
{
    hbox_layout_top_combobox_bar = new QHBoxLayout;

    QFont font;
    font.setPixelSize(12);


    pushButton_topbar_previous = new QPushButton(this);
    pushButton_topbar_previous->setIcon(QIcon("../icons/signs/previous.png"));

    QFont m_font = pushButton_topbar_previous->font();
    m_font.setPointSize(11);

    connect(pushButton_topbar_previous,SIGNAL(clicked()),this,SLOT(slot_pushButton_topbar_previous_clicked()));
    pushButton_topbar_previous->setFont(m_font);
    pushButton_topbar_previous->setMinimumSize(25,25);
    pushButton_topbar_previous->setMaximumSize(25,22);
    pushButton_topbar_previous->setStyleSheet("QPushButton{ "
                                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4); "
                                              "border: 0px solid rgb(100,100,100); border-radius: 0px;  color:gray ; border-width: 0px 0px 0px 0px; }"
                                              "QPushButton:hover { background:#767676; color:white;} "
                                              "QPushButton:pressed { color:  black; }");


    pushButton_topbar_next = new QPushButton(this);
    pushButton_topbar_next->setIcon(QIcon("../icons/signs/next.png"));
    connect(pushButton_topbar_next,SIGNAL(clicked()),this,SLOT(slot_pushButton_topbar_next_clicked()));
    pushButton_topbar_next->setFont(m_font);
    pushButton_topbar_next->setMinimumSize(25,25);
    pushButton_topbar_next->setMaximumSize(25,22);
    pushButton_topbar_next->setStyleSheet("QPushButton{ "
                                          "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4); "
                                          "border: 0px solid rgb(100,100,100); border-radius: 0px;  color:gray ; border-width: 0px 4px 0px 0px; }"
                                          "QPushButton:hover { background:#767676; color:white;} "
                                          "QPushButton:pressed { color:  black; }");






    combobox_case_navigation = new QFlatComboBox(this, 1);
    combobox_case_navigation->setDisabled(true);
    combobox_case_navigation->setView(new QListView());
    combobox_case_navigation->setStyleSheet("QComboBox QAbstractItemView::item { min-height: 22px; min-width: 50px; selection-background-color:#5A5A5A; selection-color:black;}");

    //    QListView *l1_view = new QListView();
    //    l1_view->setStyleSheet("QAbstractItemView::item { min-height: 24px; selection-background-color:#CDCDCD; selection-color:black;}");
    //    combobox_case_navigation->setView(l1_view);

    connect(combobox_case_navigation,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_combobox_case_current_text_changed(QString)));
    combobox_case_navigation->setMinimumHeight(25);
    combobox_case_navigation->setMaximumHeight(25);
    combobox_case_navigation->setMaximumWidth(250);
    combobox_case_navigation->setFont(font);

    pushButton_combobox_case_close = new QPushButton(this);
    connect(pushButton_combobox_case_close,SIGNAL(clicked()),this,SLOT(slot_pushButton_combobox_case_close_clicked()));

    pushButton_combobox_case_close->setDisabled(true);
    pushButton_combobox_case_close->setText("");
    pushButton_combobox_case_close->setFont(m_font);

    pushButton_combobox_case_close->setMinimumSize(25,25);
    pushButton_combobox_case_close->setMaximumSize(25,22);
    pushButton_combobox_case_close->setStyleSheet("QPushButton{ "
                                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4); "
                                                  "border: 0px solid rgb(100,100,100); border-radius: 0px;  color:gray ; border-width: 0px 4px 0px 0px; }"
                                                  "QPushButton:hover { background:#767676; color:white;} "
                                                  "QPushButton:pressed { color:  black; }");






    combobox_category_navigation = new QFlatComboBox(this, 0);
    combobox_category_navigation->setView(new QListView());
    combobox_category_navigation->setStyleSheet("QComboBox QAbstractItemView::item { min-height: 22px; min-width: 50px; selection-background-color:#5A5A5A; selection-color:white;}");

    //    QListView *l2_view = new QListView();
    //    l2_view->setStyleSheet("QAbstractItemView::item { min-height: 24px; selection-background-color:#CDCDCD; selection-color:black;}");
    //    combobox_category_navigation->setView(l2_view);
    connect(combobox_category_navigation,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_combobox_category_current_text_changed(QString)));
    combobox_category_navigation->setMinimumHeight(25);
    combobox_category_navigation->setMaximumHeight(25);
    combobox_category_navigation->setMaximumWidth(250);
    combobox_category_navigation->setFont(font);

    pushButton_combobox_category_close = new QPushButton(this);
    connect(pushButton_combobox_category_close,SIGNAL(clicked()),this,SLOT(slot_pushButton_combobox_category_close_clicked()));

    pushButton_combobox_category_close->setFont(m_font);

    pushButton_combobox_category_close->setText("X");

    pushButton_combobox_category_close->setMinimumSize(25,25);
    pushButton_combobox_category_close->setMaximumSize(25,23);
    pushButton_combobox_category_close->setStyleSheet("QPushButton{ "
                                                      "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4); "
                                                      "border: 0px solid rgb(100,100,100); border-radius: 0px;  color:gray ; border-width: 0px 4px 0px 0px; }"
                                                      "QPushButton:hover { background:#767676; color:white;} "
                                                      "QPushButton:pressed { color:  black; }");

    QLabel *placeholder_label = new QLabel(this);
    placeholder_label->setMinimumHeight(25);
    placeholder_label->setMaximumHeight(23);
    placeholder_label->setFixedWidth(400);
    placeholder_label->setStyleSheet("QLabel{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4)}");

    combobox_current_displayed_feature_opened = new QFlatComboBox(this, 0);
    combobox_current_displayed_feature_opened->setView(new QListView());
    combobox_current_displayed_feature_opened->setStyleSheet("QComboBox QAbstractItemView::item { min-height: 22px; min-width: 50px; selection-background-color:#5A5A5A; selection-color:white;}");


    //    QListView *l3_view = new QListView();
    //    l3_view->setStyleSheet("QAbstractItemView::item { min-height: 24px; selection-background-color:#CDCDCD; selection-color:black;}");
    //    combobox_current_displayed_feature_opened->setView(l3_view);


    connect(combobox_current_displayed_feature_opened,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_combobox_feature_current_text_changed(QString )));
    combobox_current_displayed_feature_opened->setMinimumHeight(25);
    combobox_current_displayed_feature_opened->setMaximumHeight(25);


    combobox_current_displayed_feature_opened->setFont(font);
    pushButton_combobox_current_displayed_feature_close  = new QPushButton(this);
    connect(pushButton_combobox_current_displayed_feature_close,SIGNAL(clicked()),this,SLOT(slot_pushButton_combobox_feature_close_clicked()));
    pushButton_combobox_current_displayed_feature_close->setText("X");
    pushButton_combobox_current_displayed_feature_close->setFont(m_font);

    pushButton_combobox_current_displayed_feature_close->setMinimumSize(25,25);
    pushButton_combobox_current_displayed_feature_close->setMaximumSize(25,23);
    pushButton_combobox_current_displayed_feature_close->setStyleSheet("QPushButton{ "
                                                                       "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4); "
                                                                       "border: 0px solid rgb(100,100,100); border-radius: 0px;  color:gray ; border-width: 0px 1px 0px 0px; }"
                                                                       "QPushButton:hover { background:#767676; color:white;} "
                                                                       "QPushButton:pressed { color:  black; }");

    hbox_layout_top_combobox_bar->addWidget(pushButton_topbar_previous);
    hbox_layout_top_combobox_bar->addWidget(pushButton_topbar_next);

    hbox_layout_top_combobox_bar->addWidget(combobox_case_navigation);
    combobox_case_navigation->hide();
    hbox_layout_top_combobox_bar->addWidget(pushButton_combobox_case_close);
    pushButton_combobox_case_close->hide();

    hbox_layout_top_combobox_bar->addWidget(combobox_category_navigation);
    hbox_layout_top_combobox_bar->addWidget(pushButton_combobox_category_close);

    hbox_layout_top_combobox_bar->addWidget(combobox_current_displayed_feature_opened);
    hbox_layout_top_combobox_bar->addWidget(pushButton_combobox_current_displayed_feature_close);

    hbox_layout_top_combobox_bar->addWidget(placeholder_label);

    hbox_layout_top_combobox_bar->setContentsMargins(0,0,0,0);
    hbox_layout_top_combobox_bar->setSpacing(0);

}

void MainWindow::set_layout_1_main_tree_layout_and_case_tab()
{
    QWidget *temp_case_tab_widget = new QWidget;
    temp_case_tab_widget->setLayout(layout_vbox_1_attach_casetab_and_metadatatab); // To join layout in Splitter we have place that in temp widget.

    QWidget *widget_tree_combo = new QWidget(this);
    QHBoxLayout *hbox_tree_layout = new QHBoxLayout;

    QLabel *placeholder_label = new QLabel(this);
    placeholder_label->setMinimumHeight(25);
    placeholder_label->setMaximumHeight(23);
    placeholder_label->setFixedWidth(10);
    //placeholder_label->setStyleSheet("QLabel{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 #CCCCCC, stop: 0.51 #BBBBBB, stop:1.0 #999999)}");
    placeholder_label->setStyleSheet("QLabel{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 #C8C8C8, stop: 0.51 #bebebe, stop:1.0 #b4b4b4)}");

    dockwidget_case_tree_obj = new r_dockwidget(this);
    QLabel *label_sumuri_logo = new QLabel(this);
    label_sumuri_logo->setText("SUMURI LLC");
    label_sumuri_logo->setStyleSheet("background-color:  #f1f1f1;");
    QImage img("../icons/sumuri_logo/sumuri_with_lab_logo.png");
    img = img.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    label_sumuri_logo->setPixmap(QPixmap::fromImage(img));
    label_sumuri_logo->show();


    QVBoxLayout *layout_sidebar = new QVBoxLayout;
    layout_sidebar->setContentsMargins(0,0,0,0);
    layout_sidebar->setSpacing(0);
    layout_sidebar->addWidget(treewidget_case_display_obj);
    layout_sidebar->addWidget(label_sumuri_logo);

    QWidget *widget_sidebar = new QWidget(this);
    widget_sidebar->setMaximumWidth(300);
    widget_sidebar->setLayout(layout_sidebar);

    dockwidget_case_tree_obj->setWidget(widget_sidebar);

    //dockwidget_case_tree_obj->setWidget(treewidget_case_display_obj);
    combobox_case_tree_display = new QFlatComboBox(this, 0);
    combobox_case_tree_display->setView(new QListView());
    combobox_case_tree_display->setStyleSheet("QComboBox QAbstractItemView::item { min-height: 30px; min-width: 50px; selection-background-color:#5A5A5A; selection-color:white;}");

    //    QListView *l3_view = new QListView();
    //    l3_view->setStyleSheet("QComboBox QAbstractItemView::item { min-height: 40px; selection-background-color:#CDCDCD; selection-color:black;}");
    //    combobox_category_navigation->setView(l3_view);

    QLineEdit *lineedit_case_tree_search = new QLineEdit(this);
    lineedit_case_tree_search->setClearButtonEnabled(true);
    lineedit_case_tree_search->setFixedHeight(22);
    lineedit_case_tree_search->setAttribute(Qt::WA_MacShowFocusRect, false);
    lineedit_case_tree_search->setStyleSheet("QLineEdit { "
                                             "border: 1px solid gray;"
                                             "border-radius: 10px;"
                                             "};");
    lineedit_case_tree_search->setTextMargins(10,0,0,0);
    connect(lineedit_case_tree_search,SIGNAL(textChanged(QString)),this,SLOT(slot_lineedit_case_tree_search_text_changed(QString)));

    connect(combobox_case_tree_display,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_combobox_case_tree_display_changed(QString)));
    combobox_case_tree_display->setMinimumHeight(25);
    combobox_case_tree_display->setMaximumHeight(25);

    hbox_tree_layout->addWidget(combobox_case_tree_display);
    hbox_tree_layout->addWidget(placeholder_label);
    hbox_tree_layout->setSpacing(0);
    hbox_tree_layout->setContentsMargins(0,0,0,0);

    QVBoxLayout *vbox_tree_layout = new QVBoxLayout;
    vbox_tree_layout->addLayout(hbox_tree_layout);
    vbox_tree_layout->addWidget(lineedit_case_tree_search);
    vbox_tree_layout->setSpacing(0);
    vbox_tree_layout->setContentsMargins(1,0,1,1);

    widget_tree_combo->setLayout(vbox_tree_layout);
    widget_tree_combo->setContentsMargins(0,0,0,0);

    dockwidget_case_tree_obj->setTitleBarWidget(widget_tree_combo);
    //  dockwidget_case_tree_obj->showNormal();

    addDockWidget(Qt::LeftDockWidgetArea, dockwidget_case_tree_obj);

    splitter_main_display = new QSplitter;
    splitter_main_display->setHandleWidth(0);
    splitter_main_display->addWidget(temp_case_tab_widget);
    splitter_main_display->setContentsMargins(1,0,1,0);
    splitter_main_display->setStretchFactor(1,1);

    splitter_main_display->setOrientation(Qt::Horizontal);
    layout_hbox_1_main->addWidget(splitter_main_display);
    layout_hbox_1_main->setSpacing(0);
    layout_hbox_1_main->setContentsMargins(0,0,0,0);
    main_widget->setLayout(layout_hbox_1_main);
}

void MainWindow::create_file_previewer_tabs()
{
    QSplitter *splitter_metadata_tabs = new QSplitter(Qt::Horizontal);
    splitter_metadata_tabs->setHandleWidth(0);
    //- TAbwidget Metadata Viewer Propertie's
    tabwidget_metadata_viewer->setTabPosition(QTabWidget::TabPosition::South);
    tabwidget_metadata_viewer->setElideMode(Qt::ElideNone);
    tabwidget_metadata_viewer->setDocumentMode(true);
    tabwidget_metadata_viewer->addTab(textedit_detailed_information,QIcon("../icons/Features/detailed_info.png"), MACRO_Metadata_TabName_Detailed_Information);
    tabwidget_metadata_viewer->addTab(textedit_optical_character_recognition_information,QIcon("../icons/Features/optical_character_recognition.png"), MACRO_Metadata_TabName_Optical_Character_Recognition);

    tabwidget_metadata_viewer->addTab(hex_displayer_metadata_obj, QIcon("../icons/Features/hex_viewer.png"), MACRO_Metadata_TabName_Hex_View);
    tabwidget_metadata_viewer->addTab(text_viewer_aascii_unicode_obj,QIcon("../icons/Features/text_viewer.png"),  MACRO_Metadata_TabName_Text_View);
    tabwidget_metadata_viewer->addTab(strings_viewer_obj,QIcon("../icons/Features/strings.png"),  MACRO_Metadata_TabName_Strings);
    tabwidget_metadata_viewer->addTab(tree_exif_metadata_display, QIcon("../icons/Features/exif_data.png"), QString(MACRO_Metadata_TabName_Exif_Meatadta));
    tabwidget_metadata_viewer->addTab(tree_detailed_info_apple_metadata, QIcon("../icons/Features/apple_metadata.png"), MACRO_Metadata_TabName_Apple_Meatadta);
    tabwidget_metadata_viewer->addTab(maps_metadata_obj, QIcon("../icons/Features/maps.png"), QString(MACRO_Metadata_TabName_Maps));
    tabwidget_metadata_viewer->setTabsClosable(false);

    connect(tabwidget_metadata_viewer, SIGNAL(currentChanged(int)), this, SLOT(slot_tabwidget_metadata_viewer_tab_changed(int)));


    //- TAbwidget Image and A/V Viewer Propertie's
    tabwidget_av_and_images_previewer->setTabPosition(QTabWidget::TabPosition::South);
    tabwidget_av_and_images_previewer->setElideMode(Qt::ElideNone);
    tabwidget_av_and_images_previewer->setDocumentMode(true);
    tabwidget_av_and_images_previewer->addTab(stackwidget_previewer,QIcon("../icons/Features/preview.png"),"Preview");
    tabwidget_av_and_images_previewer->setMinimumWidth(350);
    tabwidget_av_and_images_previewer->setTabsClosable(false);

    splitter_metadata_tabs->addWidget(tabwidget_metadata_viewer);
    splitter_metadata_tabs->addWidget(tabwidget_av_and_images_previewer);
    splitter_metadata_tabs->setStretchFactor(0,1);
    splitter_metadata_tabs->setStretchFactor(1,0);
    //    dockwidget_metadata_obj = new r_dockwidget;
    //    dockwidget_metadata_obj->setWidget(splitter_metadata_tabs);

    //    layout_hbox_2_atch_metadata_tabs->addWidget(dockwidget_metadata_obj);

    layout_hbox_2_atch_metadata_tabs->addWidget(splitter_metadata_tabs);
    layout_hbox_2_atch_metadata_tabs->setContentsMargins(1,4,1,1);
    layout_hbox_2_atch_metadata_tabs->setSpacing(4);
}

void MainWindow::set_layout_tree_widgets()
{
    QStringList list_case_headers;
    list_case_headers << "Cases" <<"Type" << "Top Level Item" << "" << "Record Count" << "Root COunt Name";
    treewidget_case_display_obj->setColumnCount(6);
    treewidget_case_display_obj->setHeaderLabels(list_case_headers);
    treewidget_case_display_obj->setColumnWidth(enum_global_case_tree_display_column_ZERO,500);
}

void MainWindow::create_adhoc_paths()
{
    QString adhoc_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + QString("tmp/");
    QDir dir;
    dir.mkpath(adhoc_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Parent_In_Result_QString, adhoc_dir);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Hex_viewer_In_Result_QString, (adhoc_dir + "hex_viewer/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Plist_viewer_In_Result_QString,  (adhoc_dir + "plist_viewer/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString,  (adhoc_dir + "registry_viewer/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Log_viewer_In_Result_QString,  (adhoc_dir + "log_viewer/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Optical_Character_Recognition_In_Result_QString,  (adhoc_dir + "optical_character_recognition/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Dir_Export_In_Result_QString, (adhoc_dir + "ios_dir_export/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Apple_Metadata_In_Result_QString, (adhoc_dir + "apple_metadata/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Preview_Links_In_Result_QString, (adhoc_dir + "Preview_links/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Data_In_Result_QString, (adhoc_dir + "carved_data/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Files_In_Result_QString, (adhoc_dir + "carved_files/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Miscellenous_In_Result_QString, (adhoc_dir + "miscellenous/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString, (adhoc_dir + "ios_preview_cache/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Recognize_Faces_In_Result_QString,  (adhoc_dir + "recognize_faces/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString,  (adhoc_dir + "skin_tone_detection/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Weapons_In_Result_QString, (adhoc_dir + "weapons/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Fire_Analysis_In_Result_QString, (adhoc_dir + "fire_analysis/"));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString,(adhoc_dir + "Emails/"));

    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Weapons_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Fire_Analysis_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Recognize_Faces_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Hex_viewer_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Plist_viewer_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Log_viewer_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Optical_Character_Recognition_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Dir_Export_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Apple_Metadata_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Preview_Links_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Data_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Files_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Miscellenous_In_Result_QString).toString());
    dir.mkpath(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString).toString());


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString, (global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Hex_viewer_In_Result_QString).toString() + QString("hex_viewer.sqlite")));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString, (global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Plist_viewer_In_Result_QString).toString() + QString("plist_viewer.sqlite")));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Database_Registry_Viewer_In_Result_QString, (global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString() + QString("registry_viewer_index.sqlite")));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString, (global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Log_viewer_In_Result_QString).toString() + QString("log_viewer.sqlite")));

    hex_viewer_tmp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    plist_viewer_tmp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();
    registry_viewer_tmp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Registry_Viewer_In_Result_QString).toString();
    log_viewer_tmp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString).toString();
}

void MainWindow::set_layout_2_attch_previewer_tab_with_layout_1()
{

    splitter_case_tab_and_metadata_display = new QSplitter(Qt::Vertical);
    splitter_case_tab_and_metadata_display->setHandleWidth(4);

    QWidget *widget_metadat_tabs = new QWidget;
    widget_metadat_tabs->setLayout(layout_hbox_2_atch_metadata_tabs);

    QWidget *widget_combobox_topbar =  new QWidget(this);
    widget_combobox_topbar->setLayout(hbox_layout_top_combobox_bar);

    //    splitter_case_tab_and_metadata_display->addWidget(widget_combobox_topbar);
    //    splitter_case_tab_and_metadata_display->addWidget(stack_widget_cases_plugins_and_global_items);
    // splitter_case_tab_and_metadata_display->addWidget(widget_metadat_tabs);

    QVBoxLayout *vbox_layout = new QVBoxLayout;

    vbox_layout->setContentsMargins(0,0,0,0);
    vbox_layout->setSpacing(0);
    vbox_layout->addWidget(widget_combobox_topbar);
    vbox_layout->addWidget(stack_widget_cases_plugins_and_global_items);
    vbox_layout->setStretch(0,0);
    vbox_layout->setStretch(1,1);
    QWidget *widget = new QWidget;
    widget->setLayout(vbox_layout);

    //    splitter_case_tab_and_metadata_display->insertWidget(MACRO_CASE_DISPLAY_WIDGET_COMBOBOX_TOPBAR_int,widget_combobox_topbar);
    splitter_case_tab_and_metadata_display->insertWidget(MACRO_CASE_DISPLAY_WIDGET_PLUGIN_AND_GLOBAL_ITEMS_int,widget);
    splitter_case_tab_and_metadata_display->insertWidget(MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int,widget_metadat_tabs);


    splitter_case_tab_and_metadata_display->setContentsMargins(0,0,0,0);
    splitter_case_tab_and_metadata_display->setStretchFactor(0,0);
    splitter_case_tab_and_metadata_display->setStretchFactor(1,1);
    splitter_case_tab_and_metadata_display->setStretchFactor(2,0);

    show_metadata_widget(false);

    layout_vbox_1_attach_casetab_and_metadatatab->addWidget(splitter_case_tab_and_metadata_display);
    layout_vbox_1_attach_casetab_and_metadatatab->addSpacing(2);
    layout_vbox_1_attach_casetab_and_metadatatab->setContentsMargins(0,0,0,0);


}

void MainWindow::set_right_cornered_status_window()
{
    QWidget *widget = new QWidget(this);

    QFont font_s;
    font_s.setPixelSize(10);
    label_status_bar = new QLabel(this);
    label_status_bar->setFont(font_s);

    QHBoxLayout *hbox = new QHBoxLayout;

    pushbutton_statusbar->setFixedSize(15,15);
    pushbutton_statusbar->setIcon(QIcon("../icons/signs/taskbar.png"));
    pushbutton_statusbar->setIconSize(QSize(12,12));
    pushbutton_statusbar->setChecked(true);
    pushbutton_statusbar->setCheckable(true);

    hbox->addWidget(label_status_bar);
    hbox->addWidget(pushbutton_statusbar);
    hbox->setContentsMargins(0,0,20,0);

    widget->setLayout(hbox);

    ui->statusBar->addPermanentWidget(widget,0);
    ui->statusBar->setMaximumHeight(24);

    common_pushbutton_status_right_corner_clicked(true);
}
