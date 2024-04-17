#include "redefined_result_full_loader.h"

redefined_result_full_loader::redefined_result_full_loader(QWidget *parent) : table_result_loader(parent)
{
    setWindowFlags(Qt::Window);

    timeline_selection_window_obj = new timeline_selection_window(this);
    connect(timeline_selection_window_obj, SIGNAL(signal_time_line(qint64,qint64)), this, SLOT(slot_time_line(qint64,qint64)));

    save_result_dialog_box_obj = new save_result_dialog_box(this);
    connect(save_result_dialog_box_obj, SIGNAL(signal_save_result_name_value(QString)), this, SLOT(slot_save_result_name_value(QString)));

    save_graph_dialog_box_obj = new saved_graphs_dialog_box(this);
    connect(save_graph_dialog_box_obj, SIGNAL(signal_save_graph_name(QString)), this, SLOT(slot_save_graph_name_value(QString)));

    export_dialog_box_1_obj  =  new export_dialog_box_1(this);
    connect(export_dialog_box_1_obj, SIGNAL(signal_save_results_by_export_window(QString , QString, QString)), this, SLOT(slot_export_graph_by_saved_name(QString , QString, QString)));
    connect(export_dialog_box_1_obj, SIGNAL(signal_cancel_export_results()), this, SLOT(slot_export_graphs_cancel_results()));


    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;


}

redefined_result_full_loader::~redefined_result_full_loader()
{

}

void redefined_result_full_loader::pub_set_destination_db_path(QString db_path)
{
    destination_db_redefined_result = db_path;
}

void redefined_result_full_loader::pub_set_graph_on_display()
{
    radiobutton_create_graph_yearly->setChecked(true);

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        set_data_in_configured_accounts_list();
        if(tablewidget_accounts_configure_list->rowCount() > 1)
        {
            emit tablewidget_accounts_configure_list->cellClicked(0,0);

        }
    }else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        label_account_configure_list_link->setHidden(true);
        label_participants_list_link->setHidden(true);
        set_graphs_essentials_for_data_only();
        create_graphs_browsers_history();

    }


    ///do not delete this connection
    connect(tabwidget_redefined_result, SIGNAL(currentChanged(int)),this, SLOT(slot_tabbar_clicked(int)));


}

void redefined_result_full_loader::redefined_result_populate_data_in_table()
{
    redefined_result_bookmark_index = 0;
    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index = 5;
    os_name_index = 6;
    source_count_name_index = 7;
    visit_count_index = 11;

    redefined_result_examiner_notes_index = 12;

    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(redefined_result_examiner_notes_index);

    pub_refresh_horizontal_header();

    display_data(destination_db_redefined_result);

    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);

    mainMenu->clear();

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {
        // For custom right click
        create_right_click_actions(submenu_bookmark, menuList);

        sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
        create_tags_submenu();

        connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
        connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Add_Note)
                 << QString(MACRO_Generic_Right_Click_Remove_Note);
        submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
        create_right_click_actions(submenu_note, menuList);

        // Open With
        QStringList submenuList;
        submenuList << QString(MACRO_CASE_TREE_Plist_Viewer) << QString(MACRO_CASE_TREE_Hex_Viewer) << QString(MACRO_CASE_TREE_Sqlite_Viewer);


        ///----Create Export of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator




        act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);

        create_right_click_actions(mainMenu , menuList);

    }


}

void redefined_result_full_loader::display_data(QString dest_db_path)
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

    if(checkbox_timeline->isChecked())
    {
        QString command_time = " where timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";
        db_cmd.append(command_time);
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

    if(checkbox_timeline->isChecked())
    {
        query_index.addBindValue(QString::number(start_timestamp_int));
        query_index.addBindValue(QString::number(end_timestamp_int));
    }

    populate_data_in_tablewidget(*destination_db,query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void redefined_result_full_loader::pub_create_ui()
{


    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    main_vbox = new QVBoxLayout(this);
    upper_widgets_hbox = new QHBoxLayout(this);

    lineEdit_search = new QLineEdit(this);
    lineEdit_search->hide();
    lineEdit_search->setMinimumWidth(200);
    lineEdit_search->setMaximumWidth(220);
    lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,false);

    QFont m_font_lineedit_search = lineEdit_search->font();
    m_font_lineedit_search.setPointSize(12);
    lineEdit_search->setFont(m_font_lineedit_search);

    pushButton_search = new QPushButton(this);
    pushButton_search->setText("Search");
    pushButton_search->hide();
    QFont m_font_pushbutton_search = pushButton_search->font();
    m_font_pushbutton_search.setPointSize(12);

    pushButton_search->setFixedWidth(100);
    pushButton_search->setFont(m_font_pushbutton_search);

    pushbutton_show_all = new QPushButton(this);
    pushbutton_show_all->setText("Show All");
    pushbutton_show_all->hide();
    QFont m_font_pushbutton_show_all = pushbutton_show_all->font();
    m_font_pushbutton_show_all.setPointSize(12);
    pushbutton_show_all->setFont(m_font_pushbutton_show_all);

    pushbutton_timeline = new QPushButton("Timeline", this);
    connect(pushbutton_timeline, SIGNAL(clicked()), this, SLOT(slot_pushButton_timeline_clicked()));
    pushbutton_timeline->setFixedWidth(100);
    pushbutton_show_all->setFixedWidth(100);

    pushbutton_timeline->setEnabled(false);
    checkbox_timeline = new QCheckBox(this);
    checkbox_timeline->setChecked(false);
    connect(checkbox_timeline, SIGNAL(toggled(bool)), this, SLOT(slot_checkbox_timeline_toggled(bool)));


    QSpacerItem *spacer1 = new QSpacerItem(10,30,QSizePolicy::Fixed,QSizePolicy::Minimum);

    upper_widgets_hbox->addWidget(lineEdit_search);
    upper_widgets_hbox->addWidget(pushButton_search);
    upper_widgets_hbox->addWidget(pushbutton_show_all);

    upper_widgets_hbox->addWidget(checkbox_timeline);
    upper_widgets_hbox->addSpacerItem(spacer1);

    upper_widgets_hbox->addWidget(pushbutton_timeline);


    connect(pushButton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    connect(lineEdit_search,SIGNAL(returnPressed()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(pushbutton_show_all,SIGNAL(clicked()),this,SLOT(slot_pushbutton_showall_clicked()));

    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));

    connect(m_tablewidget_obj, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_main_table_widget_item_clicked(QTableWidgetItem*)));

    main_vbox->setContentsMargins(0,0,0,0);

    configure_ui_for_graphs();
    set_timestamp_from_db_min_max();

}

void redefined_result_full_loader::configure_ui_for_graphs()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    setWindowFlags(Qt::Window);


    QSpacerItem *spacer = new QSpacerItem(50,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout *h_export_layout = new QHBoxLayout(main_vbox->widget());
    h_export_layout->setContentsMargins(0,0,0,0);

    h_export_layout->addLayout(upper_widgets_hbox);
    h_export_layout->addSpacerItem(spacer);

    pushButton_save = new QPushButton(this);
    pushButton_save->setText("Save");
    pushButton_save->setFixedWidth(90);
    pushButton_save->setHidden(true);
    connect(pushButton_save, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_save_clicked(bool)));

    pushButton_close_tab = new QPushButton(this);
    pushButton_close_tab->setText("Close");
    pushButton_close_tab->setFixedWidth(90);
    pushButton_close_tab->setHidden(true);
    connect(pushButton_close_tab, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_close_tab_clicked(bool)));

    h_export_layout->addWidget(pushButton_save);
    h_export_layout->addWidget(pushButton_close_tab);

    label_account_configure_list_link = new QLabel(this);
    label_account_configure_list_link->setText("<a href = "+ QString("Show") + " > " + QString("Accounts") + " </a>");
    QObject::connect(label_account_configure_list_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_link_label_accounts_list(QString)));

    label_participants_list_link = new QLabel(this);
    label_participants_list_link->setText("<a href = "+ QString("Show") + " > " + QString("Participants") + " </a>");
    QObject::connect(label_participants_list_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_link_label_participants_list(QString)));

    label_account_configure_list_link->setHidden(true);
    label_participants_list_link->setHidden(true);

    h_export_layout->addWidget(label_account_configure_list_link);
    h_export_layout->addWidget(label_participants_list_link);

    main_vbox->addLayout(h_export_layout);

    main_vbox->setContentsMargins(0,0,0,0);
    main_vbox->setSpacing(1);
    main_vbox->setContentsMargins(0,0,0,0);


    ///Layout for Graph View Set start
    radiobutton_create_graph_yearly = new QRadioButton("Yearly",this);
    radiobutton_create_graph_monthly = new QRadioButton("Monthly",this);
    radiobutton_create_graph_daywise = new QRadioButton("Daywise",this);
    radiobutton_create_graph_hourly = new QRadioButton("Hourly",this);

    radiobutton_create_graph_yearly->setFixedWidth(100);
    radiobutton_create_graph_monthly->setFixedWidth(100);
    radiobutton_create_graph_daywise->setFixedWidth(100);
    radiobutton_create_graph_hourly->setFixedWidth(100);

    pushbutton_previous = new QPushButton(this);
    pushbutton_next = new QPushButton(this);
    pushbutton_previous->setFixedWidth(50);
    pushbutton_next->setFixedWidth(50);

    pushbutton_previous->setIcon(QIcon("../icons/signs/previous.png"));
    pushbutton_next->setIcon(QIcon("../icons/signs/next.png"));

    pushButton_export_graph = new QPushButton("Export",this) ;
    pushButton_save_graph = new QPushButton("Save",this) ;


    label_show_pages = new QLabel(this);
    label_show_pages->setAlignment(Qt::AlignCenter);
    label_show_pages->setFixedWidth(120);

    label_show_date_time = new QLabel(this);
    QLabel *label_duration = new QLabel(this);
    label_duration->setText("Duration: ");
    label_show_date_time->setFixedWidth(150);


    label_goto_page = new QLabel(this);
    linedit_go_to_page = new QLineEdit(this);
    label_goto_page->setText("     Go to Page");
    label_goto_page->setFixedWidth(100);
    linedit_go_to_page->setFixedWidth(70);

    pushbutton_go_to_page = new QPushButton("Go",this);
    pushbutton_go_to_page->setFixedWidth(50);

    connect(pushbutton_previous, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_previous_clicked(bool)));
    connect(pushbutton_next, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_next_clicked(bool)));
    connect(pushbutton_go_to_page, SIGNAL(clicked()), this, SLOT(slot_pushbutton_go_to_page_clicked()));
    connect(linedit_go_to_page,SIGNAL(textEdited(QString)),this,SLOT(slot_line_edit_go_to_page_text_changed(QString)));
    connect(linedit_go_to_page,SIGNAL(returnPressed()) , this, SLOT(slot_pushbutton_go_to_page_clicked()));
    connect(radiobutton_create_graph_yearly,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_yearly_clicked(bool)));
    connect(radiobutton_create_graph_monthly,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_monthly_clicked(bool)));
    connect(radiobutton_create_graph_daywise,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_daywise_clicked(bool)));
    connect(radiobutton_create_graph_hourly,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_hourly_clicked(bool)));

    connect(pushButton_export_graph, SIGNAL(clicked()), this, SLOT(slot_pushbutton_export_graph_clicked()));
    connect(pushButton_save_graph, SIGNAL(clicked()), this, SLOT(slot_pushButton_save_graph_clicked()));


    QSpacerItem *spacer_at_last = new QSpacerItem(50, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout *hbox_radio_buttons_layout = new QHBoxLayout;

    hbox_radio_buttons_layout->addWidget(radiobutton_create_graph_yearly);
    hbox_radio_buttons_layout->addWidget(radiobutton_create_graph_monthly);
    hbox_radio_buttons_layout->addWidget(radiobutton_create_graph_daywise);
    hbox_radio_buttons_layout->addWidget(radiobutton_create_graph_hourly);
    hbox_radio_buttons_layout->addWidget(label_duration);
    hbox_radio_buttons_layout->addWidget(label_show_date_time);
    hbox_radio_buttons_layout->addWidget(pushbutton_previous);
    hbox_radio_buttons_layout->addWidget(label_show_pages);
    hbox_radio_buttons_layout->addWidget(pushbutton_next);
    hbox_radio_buttons_layout->addWidget(label_goto_page);
    hbox_radio_buttons_layout->addWidget(linedit_go_to_page);
    hbox_radio_buttons_layout->addWidget(pushbutton_go_to_page);
    hbox_radio_buttons_layout->addSpacerItem(spacer_at_last);
    hbox_radio_buttons_layout->addWidget(pushButton_save_graph);
    hbox_radio_buttons_layout->addWidget(pushButton_export_graph);


    pushbutton_previous->setEnabled(false);
    pushbutton_next->setEnabled(false);

    hbox_radio_buttons_layout->setSpacing(2);

    stackwidget_obj = new QStackedWidget(this);
    QVBoxLayout *vbox_chartview_layout = new QVBoxLayout;
    vbox_chartview_layout->addWidget(stackwidget_obj);

    tabwidget_redefined_result = new QTabWidget(this);
    connect(tabwidget_redefined_result, SIGNAL(tabBarClicked(int)),this, SLOT(slot_tabbar_clicked(int)));

    QScrollArea *scrollarea_obj = new QScrollArea(tabwidget_redefined_result);
    scrollarea_obj->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea_obj->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vbox_chartview_layout->setSizeConstraint(QLayout::SetMinimumSize);

    scrollarea_obj->setWidgetResizable(true);
    QWidget *container_widget = new QWidget;

    vbox_chartview_layout->addLayout(hbox_radio_buttons_layout);
    container_widget->setLayout(vbox_chartview_layout);
    container_widget->setContentsMargins(0,0,0,0);
    scrollarea_obj->setWidget(container_widget);

    ///Layout for Graph View Set End


    ///--------
    tabwidget_redefined_result->addTab(m_tablewidget_obj, tr("Event View"));
    tabwidget_redefined_result->addTab(scrollarea_obj, tr("Graph View"));

    chartView_for_pie_or_visit_count_obj = new QChartView(this);
    chartView_for_pie_or_visit_count_obj->setRenderHint(QPainter::Antialiasing);
    //chartView_for_pie_or_visit_count_obj->setRubberBand(QChartView::HorizontalRubberBand);

    pushButton_msg_pie_chart_topurl_export = new QPushButton("Export",this) ;
    connect(pushButton_msg_pie_chart_topurl_export, SIGNAL(clicked()), this, SLOT(slot_pushButton_topurl_export_clicked()));
    pushButton_save_url_pie = new QPushButton("Save",this) ;
    connect(pushButton_save_url_pie, SIGNAL(clicked()), this, SLOT(slot_pushButton_save_url_pie_clicked()));

    QHBoxLayout *hbox_push_buttons_layout = new QHBoxLayout;


    hbox_push_buttons_layout->addStretch(1);
    hbox_push_buttons_layout->addWidget(pushButton_save_url_pie);
    hbox_push_buttons_layout->addWidget(pushButton_msg_pie_chart_topurl_export);


    QVBoxLayout *vbox_topview_layout = new QVBoxLayout;
    vbox_topview_layout->addWidget(chartView_for_pie_or_visit_count_obj);
    vbox_topview_layout->setSizeConstraint(QLayout::SetMinimumSize);


    QScrollArea *scrollarea_2_obj = new QScrollArea(tabwidget_redefined_result);
    scrollarea_2_obj->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea_2_obj->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vbox_topview_layout->setSizeConstraint(QLayout::SetMinimumSize);

    scrollarea_2_obj->setWidgetResizable(true);
    QWidget *container_widget_2 = new QWidget;

    vbox_topview_layout->addLayout(hbox_push_buttons_layout);
    container_widget_2->setLayout(vbox_topview_layout);
    container_widget_2->setContentsMargins(0,0,0,0);
    scrollarea_2_obj->setWidget(container_widget_2);

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        tabwidget_redefined_result->addTab(scrollarea_2_obj, tr("Pie View"));
    else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        tabwidget_redefined_result->addTab(scrollarea_2_obj, tr("Top URLs"));


    ///-----------


    tablewidget_accounts_configure_list = new QTableWidget();
    tablewidget_participants_list = new QTableWidget();

    tablewidget_accounts_configure_list->setHidden(true);
    tablewidget_participants_list->setHidden(true);

    QHBoxLayout *hbox_tabwidgets_layout = new QHBoxLayout(this);
    hbox_tabwidgets_layout->addWidget(tabwidget_redefined_result,1);
    hbox_tabwidgets_layout->addWidget(tablewidget_accounts_configure_list,0);
    hbox_tabwidgets_layout->addWidget(tablewidget_participants_list,0);
    hbox_tabwidgets_layout->setContentsMargins(0,0,0,0);
    hbox_tabwidgets_layout->setSpacing(2);

    hbox_tabwidgets_layout->setContentsMargins(0,0,0,0);

    main_vbox->addLayout(hbox_tabwidgets_layout);

    connect(tablewidget_accounts_configure_list,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_accounts_configures_item_clicked(int,int)));
    // connect(tablewidget_accounts_configure_list, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    tablewidget_accounts_configure_list->horizontalHeader()->setStretchLastSection(true);

    connect(tablewidget_participants_list,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_participants_list_cell_single_clicked(int,int)));
    // connect(tablewidget_participants_list, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    tablewidget_participants_list->horizontalHeader()->setStretchLastSection(true);


    menu_graphs = new QMenu(this);
    act_show_data_of_current_barset = new QAction("show data of current barset",this);
    act_show_all_data = new QAction("show all data",this);

    menu_graphs->addAction(act_show_data_of_current_barset);
    menu_graphs->addAction(act_show_all_data);

    connect(act_show_data_of_current_barset, SIGNAL(triggered()), this, SLOT(slot_act_show_data_of_current_barset_triggered()));
    connect(act_show_all_data, SIGNAL(triggered()), this, SLOT(slot_act_show_all_data_triggered()));


    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);
}

void redefined_result_full_loader::loading_display_dialog_based_on_records(int total_records, int processed_count)
{
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

}

int redefined_result_full_loader::populate_data_in_tablewidget_browser_history(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(display_header_name_list.isEmpty())
    {
        recon_static_functions::app_debug("display_header_name_list.isEmpty() ----FAILED---- " , Q_FUNC_INFO);
        return -1;
    }


    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_index.executedQuery(),Q_FUNC_INFO);
        return -1;
    }

    m_tablewidget_obj->setRowCount(0);

    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int timestamp_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
    int count = m_tablewidget_obj->rowCount();

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);


    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    pub_refresh_horizontal_header();

    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
        {
            break;
        }

        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            if(pos == record_no_position || pos == visit_count_index)
            {
                QTableWidgetItem *item_tablewidget_int_value;
                item_tablewidget_int_value = new QTableWidgetItem;
                item_tablewidget_int_value->setData(Qt::EditRole, dataStr_from_db.toLongLong());
                item_tablewidget_int_value->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int_value);
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
                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;
                if(dataStr_from_db.isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_tags->setIcon(icon);
                }
                else
                {
                    QString icon_str = dataStr_from_db;

                    icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                    icon_str = icon_str.toLower();
                    icon_str = icon_str.replace(" ","_");
                    QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                    item_tablewidget_tags->setIcon(icon);
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
            else if(pos == timestamp_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                dataStr_from_db = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db, Q_FUNC_INFO);

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int);
            }
            else if(pos == visit_count_index)
            {
                QTableWidgetItem *item_tablewidget_visit_count_itm;
                item_tablewidget_visit_count_itm = new QTableWidgetItem;
                item_tablewidget_visit_count_itm->setData(Qt::EditRole, dataStr_from_db.toLongLong());
                item_tablewidget_visit_count_itm->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_visit_count_itm);
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

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

int redefined_result_full_loader::populate_data_tablewidget_messenger(QSqlDatabase &received_db_obj,QSqlQuery &query_index)
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

    m_tablewidget_obj->setRowCount(0);

    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    /// this is because the notes column will be at last position
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int timestamp_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
    int count = m_tablewidget_obj->rowCount();


    /// this is because the notes column will be at last position
    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);


    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
        {
            break;
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
            else if(pos == timestamp_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                dataStr_from_db = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db, Q_FUNC_INFO);

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int);
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

int redefined_result_full_loader::populate_data_of_graph_in_tablewidget_messenger(QSqlQuery &query_index)
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


    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    /// this is because the notes column will be at last position
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int timestamp_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
    int count = m_tablewidget_obj->rowCount();


    /// this is because the notes column will be at last position
    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);


    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
        {
            break;
        }

        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

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
                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;

                if(dataStr_from_db.isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_tags->setIcon(icon);
                }
                else
                {
                    QString icon_str = dataStr_from_db;
                    icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                    icon_str = icon_str.toLower();
                    icon_str = icon_str.replace(" ","_");
                    QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                    item_tablewidget_tags->setIcon(icon);
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
            else if(pos == timestamp_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                dataStr_from_db = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db, Q_FUNC_INFO);

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int);
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

int redefined_result_full_loader::populate_data_of_graph_in_tablewidget_browser_history(QSqlQuery &query_index)
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


    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    /// this is because the notes column will be at last position
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int timestamp_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
    int count = m_tablewidget_obj->rowCount();


    /// this is because the notes column will be at last position
    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);


    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
        {
            break;
        }

        m_tablewidget_obj->insertRow(count);
        m_tablewidget_obj->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            if(pos == record_no_position || pos == visit_count_index)
            {
                QTableWidgetItem *item_tablewidget_int_value;
                item_tablewidget_int_value = new QTableWidgetItem;
                item_tablewidget_int_value->setData(Qt::EditRole, dataStr_from_db.toLongLong());
                item_tablewidget_int_value->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int_value);
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
                QTableWidgetItem *item_tablewidget_tags = new QTableWidgetItem;
                if(dataStr_from_db.isEmpty())
                {
                    QIcon icon("");
                    item_tablewidget_tags->setIcon(icon);
                }
                else
                {
                    QString icon_str = dataStr_from_db;
                    icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                    icon_str = icon_str.toLower();
                    icon_str = icon_str.replace(" ","_");
                    QIcon icon("../icons/tag_icons/"+ icon_str +".png");
                    item_tablewidget_tags->setIcon(icon);
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
            else if(pos == timestamp_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                dataStr_from_db = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db, Q_FUNC_INFO);

                item_tablewidget_int->setText(dataStr_from_db);
                m_tablewidget_obj->setItem(count, pos, item_tablewidget_int);
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

void redefined_result_full_loader::prepare_sqlquery_for_piegraph_tablewidget_to_save_data(QSqlQuery &query_index, QString account_name_str)
{

    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }

    db_cmd.append("where ((sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)) AND ");

    QString command_time = " timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
    command_for_table_display = db_cmd;


    query_index.prepare(command_for_table_display);


    query_index.addBindValue(account_name_str);
    query_index.addBindValue(current_barset_participant);
    query_index.addBindValue(current_barset_participant);
    query_index.addBindValue(account_name_str);

    query_index.addBindValue(QString::number(start_timestamp_int));
    query_index.addBindValue(QString::number(end_timestamp_int));


}

void redefined_result_full_loader::prepare_sqlquery_for_all_graphs_tablewidget_to_save_data(QSqlQuery &query_index, QString account_name_str , QString graph_type_str)
{
    QString db_cmd;


    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }

    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }


    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        if(tablewidget_accounts_configure_list->currentItem()->text() == Macro_Redefined_Result_Messenger_All_Accounts)
        {
            db_cmd.append("where ");
        }else
        {
            db_cmd.append("where ((sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)) AND ");
        }
    }
    else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        db_cmd.append("where ");
    }


    QString command_type =  graph_type_str + " = ? ";
    db_cmd.append(command_type) ;

    QString command_time = " AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
    command_for_table_display = db_cmd;

    query_index.prepare(command_for_table_display);


    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        if(tablewidget_accounts_configure_list->currentItem()->text() != Macro_Redefined_Result_Messenger_All_Accounts)
        {
            query_index.addBindValue(account_name_str);
            query_index.addBindValue(current_barset_participant);
            query_index.addBindValue(current_barset_participant);
            query_index.addBindValue(account_name_str);
        }
    }

    QString category_str = current_barset_category_str;

    if(category_str.endsWith(" hr"))
        category_str.chop(QString(" hr").size());

    query_index.addBindValue(category_str);

    query_index.addBindValue(QString::number(start_timestamp_int));
    query_index.addBindValue(QString::number(end_timestamp_int));


}

QString redefined_result_full_loader::get_graph_type()
{
    QString graph_type;
    if(radiobutton_create_graph_yearly->isChecked())
        graph_type = "year_str";

    if(radiobutton_create_graph_monthly->isChecked())
        graph_type = "month_year";

    if(radiobutton_create_graph_daywise->isChecked())
        graph_type = "day_month_year";

    if(radiobutton_create_graph_hourly->isChecked())
        graph_type = "hour_day_month_year";

    return graph_type;
}

int redefined_result_full_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        populate_data_in_tablewidget_browser_history(received_db_obj, query_index);
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        populate_data_tablewidget_messenger(received_db_obj, query_index);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void redefined_result_full_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    if(tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_stacked_graph)
    {
        if(bool_check_barset)
        {
            if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
            {

                if(tablewidget_participants_list->currentItem()->text() != Macro_Redefined_Result_Messenger_All_Participants)
                {
                    act_show_all_data->setVisible(false);
                }else
                {
                    act_show_all_data->setVisible(true);

                }

                if(tablewidget_accounts_configure_list->currentItem()->text() == Macro_Redefined_Result_Messenger_All_Accounts)
                {
                    act_show_data_of_current_barset->setText("show data");
                    act_show_all_data->setVisible(false);

                }else
                {
                    act_show_data_of_current_barset->setText("show data of " + current_barset_participant);
                    act_show_all_data->setVisible(true);
                }

                if((tablewidget_participants_list->currentItem()->text() != Macro_Redefined_Result_Messenger_All_Participants) &&(tablewidget_accounts_configure_list->currentItem()->text() != Macro_Redefined_Result_Messenger_All_Accounts) )
                    act_show_all_data->setVisible(false);

            }else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
            {
                act_show_data_of_current_barset->setText("show data");
                act_show_all_data->setVisible(false);

            }

            menu_graphs->exec(QCursor::pos());
        }
        return;
    }

    if(tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_pie_or_visit_count_graph)
    {
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
            act_show_data_of_current_barset->setText("show data of " + current_barset_participant);
        else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
            act_show_data_of_current_barset->setText("show data of " + current_clicked_domain);

        act_show_all_data->setVisible(false);
        //menu_graphs->exec(QCursor::pos()); // Fix this part. This part shows same right click on all pie slices.
        return;
    }

    if(bool_display_table_data_loaded)
        return;


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int current_row = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();
    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text();

    if(plugin_name_str.isEmpty())
        return;

    if(!m_tablewidget_obj->selectionModel()->selectedRows().isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }

    tablewidget_general_clicked(current_row,current_column);

    // these are common
    //submenu_open_with->setEnabled(false);
    //submenu_send_to_bucket->setEnabled(false);
    //act_export->setEnabled(false);
    //    submenu_add_files_to_hashset_db->setEnabled(false);
    //    submenu_search_file_with_same_md5_hash->setEnabled(false);
    //    submenu_go_to_artifact_source->setEnabled(false);
    //act_go_to_record->setEnabled(false);

    act_full_detail_info->setEnabled(true);


    if(selection_model_list.size() > 1)
    {
        act_full_detail_info->setEnabled(false);
        //act_export->setEnabled(true);
    }
    else if(selection_model_list.size() == 0)
    {
        //submenu_go_to_artifact_source->setEnabled(false);
        // act_go_to_record->setEnabled(false);
    }
    else
    {

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
        QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            return;

        if(st_csv_info_obj.tab_export_available_QStringList_tablewidget.at(tab_index) == QString("1"))
        {
            //act_export->setEnabled(true);
            //submenu_send_to_bucket->setEnabled(true);
            //submenu_open_with->setEnabled(true);
            //            submenu_add_files_to_hashset_db->setEnabled(true);
            //            submenu_search_file_with_same_md5_hash->setEnabled(true);
        }

        // submenu_go_to_artifact_source->setEnabled(true);
        // act_go_to_record->setEnabled(true);
    }

    //create_action_for_goto_artifact_source_submenu();

    mainMenu->exec(QCursor::pos());
}

void redefined_result_full_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    if(row < 0 || column < 0)
        return;

    QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text();
    if(plugin_name_str.isEmpty())
        return;

    QString searched_text = searched_keyword_str;
    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();


    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("redefined_result",0,record_no,plugin_name,destination_db_redefined_result, "" ,struct_info, Q_FUNC_INFO);


    current_media_path = "";
    preview_file_path  = "";
    metadata_str       = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = searched_text;
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = "";


    emit signal_set_metadata_info(st_set_meta_obj);


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    QString os_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), os_name_index)->text();

    QString searched_keyword = searched_keyword_str;

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

    int preview_index_full = 0;
    preview_index_full = get_stack_index_for_preview(preview_file_path,"");

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, preview_file_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,table_name);

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_open_detach_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    //    if(m_tablewidget_obj->currentRow() < 0)
    //        return;

    //    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    //    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    //    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    //    QString os_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), os_name_index)->text();

    //    QString searched_keyword = searched_keyword_str;
    //    QString tmp_plg_name = plugin_name_str;
    //    tmp_plg_name.replace(" ","_");

    //    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    //    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    //    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    //    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();
    //    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);


    //    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
    //        return;

    //    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    //    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

    //    int preview_index_full = 0;
    //    preview_index_full = get_stack_index_for_preview(preview_file_path);

    //    emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, preview_file_path, preview_index_full,searched_keyword,source_count_name);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::create_action_for_goto_artifact_source_submenu()
{
    if(m_tablewidget_obj->currentRow() < 0 || m_tablewidget_obj->currentRow() >= m_tablewidget_obj->rowCount())
        return;

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();
    QString os_schme_dsply = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), os_name_index)->text();

    submenu_go_to_artifact_source->clear();

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QString command = "SELECT source_file FROM '" + table_name + "' WHERE INT=?" + " GROUP BY source_file";
    QStringList arg_list;
    arg_list << record_no;
    QString artifact_source = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);

    if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon)
            || artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
    {
        QStringList source_list;

        if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
        {
            source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
        }
        else
        {
            source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
        }

        for(int i = 0; i < source_list.size(); i++)
        {
            QString source_path = source_list.at(i);

            if(os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display) || os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display))
            {
                QString target_initial_path = recon_helper_standard_obj->get_virtual_source_path_of_current_record(record_no , table_name,  plugins_destination_db_file,Q_FUNC_INFO);
                source_path = recon_static_functions::get_complete_file_path(target_initial_path, source_path, Q_FUNC_INFO);
            }

            QFileInfo info_file(source_path);

            QAction *act_go_to_actifact = new QAction(QObject::tr(info_file.fileName().toLocal8Bit().data()), this);
            act_go_to_actifact->setToolTip(source_path);
            submenu_go_to_artifact_source->addAction(act_go_to_actifact);
        }
    }
    else
    {
        if(os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display) || os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display))
        {
            QString target_initial_path = recon_helper_standard_obj->get_virtual_source_path_of_current_record(record_no , table_name,  plugins_destination_db_file,Q_FUNC_INFO);
            artifact_source = recon_static_functions::get_complete_file_path(target_initial_path, artifact_source, Q_FUNC_INFO);
        }

        QFileInfo info_file(artifact_source);

        QAction *act_go_to_actifact = new QAction(QObject::tr(info_file.fileName().toLocal8Bit().data()), this);
        act_go_to_actifact->setToolTip(artifact_source);
        submenu_go_to_artifact_source->addAction(act_go_to_actifact);
    }
}

void redefined_result_full_loader::action_go_to_record_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
        return;

    QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
    QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();

    emit signal_tablewidget_goto_record(plugin_name_str,QString("Parent RECON Plugin"),tab_name_str,record_no , QString(""), os_name_str);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void redefined_result_full_loader::action_remove_note_triggered()
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

    global_connection_manager_obj->pub_remove_notes_in_redefined_db(INT_List,plugin_name);
    global_connection_manager_obj->pub_delete_notes_for_redefined_db(INT_List,plugin_name);

    display_loading_progress_bar_non_cancelable_obj->hide();
}


void redefined_result_full_loader::update_notes_value(QString text, int current_row)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();

    text = text.trimmed();

    int notes_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    if(notes_column_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        if(current_row < 0 && current_row >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(current_row, record_no_index)->text();

        struct_global_tagging_redefined_result_record s1;
        s1.INT_record = m_tablewidget_obj->item(current_row, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(current_row, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(current_row, 8)->text();
        s1.item3 = m_tablewidget_obj->item(current_row, 9)->text();
        s1.item4 = m_tablewidget_obj->item(current_row, 10)->text();
        s1.item5 = m_tablewidget_obj->item(current_row, 11)->text();
        s1.redefined_tab_name = plugin_name;
        s1.redefined_plugin_name = plugin_name;

        redefined_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(current_row, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_redefined_db(INT_List,plugin_name,text);
    global_connection_manager_obj->pub_add_notes_for_redefined_db(redefined_record_list, text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void redefined_result_full_loader::action_sendToBucket_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();


        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            continue;

        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

        QStringList tmp_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);

        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

        for(int j = 0; j < tmp_list.size(); j++)
        {
            all_path_list << QString(tmp_list.at(j) + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }

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
            QString hex_display_nm = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

            QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
            if(list_hex_display_name.contains(hex_display_nm))
            {
                continue;
            }


            ///check file in bucket
            struct_global_artifacts_export_fileinfo st_globl_recon_file_info_obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
            struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;

            recon_static_functions::clear_variables_hex_viewer_open_file_data(hex_viewer_open_file_data_obj);

            QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

            hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
            hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
            hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;
            hex_viewer_open_file_data_obj.complete_file_path  = complete_path;

            if(st_globl_recon_file_info_obj.displayed_file_path.size() > 0)
                hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.displayed_file_path.at(0);

            hex_viewer_open_file_data_obj.record_no_str       = record_no;
            hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
            hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name_str;
            hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;

            if(st_globl_recon_file_info_obj.recon_filefrom_list.size() > 0)
                hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_filefrom_list.at(0);

            if(st_globl_recon_file_info_obj.recon_file_info_path_list.size() > 0)
                hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path_list.at(0);

            recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

            emit signal_viewer_extarction_completed_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer);

        }
    }


    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_sendToBucket_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList message_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QString tab_name_str;
        QStringList all_path_list;

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;


        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            continue;

        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

        QStringList tmp_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);
        for(int j = 0; j < tmp_list.size(); j++)
        {
            all_path_list << QString(tmp_list.at(j) + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }

        for(int i = 0; i < all_path_list.size(); i++)
        {
            QStringList temp_list = all_path_list.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(temp_list.size() < 2)
                continue;

            QString complete_path = temp_list.at(0);
            QString source_count_name = temp_list.at(1);

            QFileInfo info(complete_path);
            if(!info.exists())
            {
                recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
                message_list << QString("'" +  info.fileName() + "' File not exist.");
                continue;
            }

            if(info.isDir())
                continue;

            if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_path, Q_FUNC_INFO))
            {
                message_list << QString("'" +  info.fileName() + "' is not sqlite file.");
                continue;
            }
            else
            {

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString sqlite_display_nm = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

                QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
                if(list_sqlite_display_name.contains(sqlite_display_nm))
                {
                    continue;
                }

                QString sqlite_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

                sqlite_viewer_starter *sqlite_viewer_starter_obj;
                sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
                sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
                sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
                sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
                sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
                sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
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

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_sendToBucket_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString source_count_name;

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            continue;

        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

        source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_path,Q_FUNC_INFO);

        all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);


        for(int i =0 ; i< all_path_list.size() ; i++)
        {
            QString complete_path = all_path_list.at(i);

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
            if(!temp_p.endsWith(".plist"))
            {
                message_list << QString("'") +  info.fileName() + QString("' is not plist file.");
                continue;
            }
            else
            {

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString plist_display_nm = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

                QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
                if(list_plist_display_name.contains(plist_display_nm))
                {
                    continue;
                }

                plist_viewer *plist_viewer_obj;
                plist_viewer_obj = new plist_viewer(this);
                QString plist_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

                plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
                plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
                plist_viewer_obj->pub_set_plist_path(complete_path);
                plist_viewer_obj->pub_set_source_count_name(source_count_name);
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

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_open_with_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString source_count_name;
    QString complete_file_path;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_path,Q_FUNC_INFO);

    QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

    QStringList all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);

    if(all_path_list.size() > 0)
        complete_file_path = all_path_list.at(0);


    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        return;
    }

    QString temp_p = complete_file_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".plist"))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' is not plist file.");
        message_dialog_obj->show();
    }
    else
    {

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString plist_display_nm = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(plist_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
        }
        else
        {
            plist_viewer *plist_viewer_obj;
            plist_viewer_obj = new plist_viewer(this);
            QString plist_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

            plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
            plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
            plist_viewer_obj->pub_set_plist_path(complete_file_path);
            plist_viewer_obj->pub_set_source_count_name(source_count_name);
            plist_viewer_obj->pub_set_essentials();

            plist_viewer_obj->pub_start_plist_extraction();

            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
        }
    }


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_open_with_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString source_count_name;
    QString complete_file_path;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_path,Q_FUNC_INFO);

    QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

    QStringList all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);

    if(all_path_list.size() > 0)
        complete_file_path = all_path_list.at(0);


    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        return;
    }


    QString src_cnt_name_label = QString(" [" + source_count_name + "]");
    QString hex_display_nm = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

    QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
    if(list_hex_display_name.contains(hex_display_nm))
    {
        emit signal_show_file_in_viewer_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer, source_count_name);
    }
    else
    {


        ///check file in bucket
        struct_global_artifacts_export_fileinfo st_globl_recon_file_info_obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
        struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;

        recon_static_functions::clear_variables_hex_viewer_open_file_data(hex_viewer_open_file_data_obj);

        QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

        hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
        hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;
        hex_viewer_open_file_data_obj.complete_file_path  = complete_file_path;

        if(st_globl_recon_file_info_obj.displayed_file_path.size() > 0)
            hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.displayed_file_path.at(0);

        hex_viewer_open_file_data_obj.record_no_str       = record_no;
        hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
        hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name_str;
        hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;

        if(st_globl_recon_file_info_obj.recon_filefrom_list.size() > 0)
            hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_filefrom_list.at(0);

        if(st_globl_recon_file_info_obj.recon_file_info_path_list.size() > 0)
            hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path_list.at(0);

        recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

        emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);

    }


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString source_count_name;
    QString complete_file_path;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_path,Q_FUNC_INFO);

    QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

    QStringList all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);

    if(all_path_list.size() > 0)
        complete_file_path = all_path_list.at(0);


    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        return;
    }

    if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_file_path, Q_FUNC_INFO))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not sqlite file.");
        message_dialog_obj->show();
    }
    else
    {

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString sqlite_display_nm = info.fileName() + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(sqlite_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
        }
        else
        {
            QString sqlite_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

            sqlite_viewer_starter *sqlite_viewer_starter_obj;
            sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
            sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
            sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
            sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
            sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_file_path);
            sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
            sqlite_viewer_starter_obj->pub_set_essentials();

            sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
        }
    }


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_full_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
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

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            continue;

        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

        QStringList all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);

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
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_path);
        message_dialog_obj->show();
    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"selected records does not have any file to export");
        message_dialog_obj->show();

    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


