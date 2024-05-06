#include "artifacts_timeline_full_loader.h"

artifacts_timeline_full_loader::artifacts_timeline_full_loader(QWidget *parent) : table_result_loader(parent)
{

    index_for_INT_only_for_artifact_timeline = 13;
    message_box_obj = new message_dialog(this);

    save_result_dialog_box_obj = new save_result_dialog_box(this);
    connect(save_result_dialog_box_obj, SIGNAL(signal_save_result_name_value(QString)), this, SLOT(slot_save_result_name_value(QString)));

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);


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

}

artifacts_timeline_full_loader::~artifacts_timeline_full_loader()
{

}
void artifacts_timeline_full_loader::contextMenuEvent(QContextMenuEvent *evt)
{ // create right click menu here
    if(evt == NULL)
        return;

    if(tabwidget_timeline_data_obj->currentIndex() == 0)
    {
        if(bool_check_barset)
        {
            act_show_data_of_current_plugin_barset->setText("show data of " + current_barset_plugin);
            menu_graphs->exec(QCursor::pos());

        }
        return;
    }

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() > 1 || selection_model_list.size() == 0)
    {
        submenu_go_to_artifact_source->setEnabled(false);
        act_go_to_record->setEnabled(false);

    }
    else
    {
        submenu_go_to_artifact_source->setEnabled(true);
        act_go_to_record->setEnabled(true);
    }

    create_action_for_goto_artifact_source_submenu();

    mainMenu->exec(QCursor::pos());

    return;

}

void artifacts_timeline_full_loader::pub_set_display_db_path(QString db_path)
{ // set display db path
    destination_timeline_db_path = db_path;
}

void artifacts_timeline_full_loader::pub_set_saved_events_db_path(QString db_path)
{ // set timeline saved db path
    destination_timeline_saved_db_path = db_path;
}

void artifacts_timeline_full_loader::loading_display_dialog_based_on_records(int total_records, int processed_count)
{ //Show progress count on progress bar
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

}

void artifacts_timeline_full_loader::action_bookmark_triggered()
{ // when we click bookmark on right click
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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


        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    }

    global_connection_manager_obj->pub_set_bookmark_in_artifact_timeline_db(INT_List);
    global_connection_manager_obj->pub_add_bookmark_for_artifact_timeline(timeline_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_full_loader::action_remove_bookmark_triggered()
{ // remove bookmark on click
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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


        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_artifact_timeline_db(INT_List);
    global_connection_manager_obj->pub_delete_bookmark_for_artifact_timeline(timeline_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_full_loader::populate_data_in_table()
{

}

void artifacts_timeline_full_loader::display_data(QString dest_db_path)
{ // display all the data from database and load in tablewidget
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


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }


    db_cmd.append("where plugin = ? AND (");

    for(int tab = 0; tab < bookmarked_plugin_tab_list.size(); tab++)
    {
        db_cmd.append(" category = ? OR ");
    }

    if(db_cmd.endsWith("OR "))
    {
        db_cmd.chop(3);
        db_cmd.append(") AND (");
    }

    for(int tab = 0; tab < bookmarked_timestamp_type_list.size(); tab++)
    {
        db_cmd.append(" stamp_type = ? OR ");
    }

    if(db_cmd.endsWith("OR "))
    {
        db_cmd.chop(3);
        db_cmd.append(")");
    }


    QString command_time = " AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
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

    query_index.addBindValue(bookmarked_plugin_name);
    for(int tab = 0; tab < bookmarked_plugin_tab_list.size(); tab++)
    {
        query_index.addBindValue(bookmarked_plugin_tab_list.at(tab));
    }

    for(int j = 0; j < bookmarked_timestamp_type_list.size(); j++)
    {
        query_index.addBindValue(bookmarked_timestamp_type_list.at(j));
    }

    query_index.addBindValue(QString::number(start_timestamp_utc_qint64));
    query_index.addBindValue(QString::number(end_timestamp_utc_qint64));

    populate_data_in_tablewidget(*destination_db, query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

int artifacts_timeline_full_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{ // load all the data in tablewidget
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
                        icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
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

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void artifacts_timeline_full_loader::set_timeline()
{ // set timeline (what timelines data you want to see)
    start_timestamp_utc_qint64 = 0;
    end_timestamp_utc_qint64 = 0;

    QDateTime start_date_time;
    start_date_time.setTimeSpec(Qt::UTC);
    start_date_time.setDate(datetimeedit_start->date());
    start_date_time.setTime(datetimeedit_start->time());


    QDateTime end_date_time;
    end_date_time.setTimeSpec(Qt::UTC);
    end_date_time.setDate(datetimeedit_end->date());
    end_date_time.setTime(datetimeedit_end->time());


    start_timestamp_utc_qint64 = start_date_time.toSecsSinceEpoch();
    end_timestamp_utc_qint64 = end_date_time.toSecsSinceEpoch();

    //    start_timestamp_int -= case_narad_muni->get_field(MACRO_NARAD_Target_System_Time_Zone_qint64).toLongLong();
    //    end_timestamp_int -= case_narad_muni->get_field(MACRO_NARAD_Target_System_Time_Zone_qint64).toLongLong();


}

void artifacts_timeline_full_loader::pub_create_ui()
{ // create ui of tablewidget
    m_tablewidget_obj = new m_tablewidget();
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    main_vbox = new QVBoxLayout(this);

    main_vbox->setStretch(0,0);
    main_vbox->setStretch(1,1);

    // main_vbox->setContentsMargins(4,4,4,4);

    ///Prepare complete ui for artifact timeline,only m_tablewidget_obj is used of table result loader
    /// otherwise all code is separate for ui of artifacts timeline
    configure_ui_for_timeline();

    this->setLayout(main_vbox);

    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));

    // connect(pushButton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));

    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    //connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    //    connect(lineEdit_search,SIGNAL(returnPressed()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(pushbutton_show_all,SIGNAL(clicked()),this,SLOT(slot_refresh_table_data_on_show_all()));

    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));






}

void artifacts_timeline_full_loader::configure_ui_for_timeline()
{ // configure ui of tablewidget like buttons, menu list on right click etc
    recon_static_functions::app_debug("---Starts----",Q_FUNC_INFO);

    setWindowFlags(Qt::Window);

    label_tab_details = new QLabel(this);
    label_tab_details->setFixedWidth(200);



    QSpacerItem *spacer = new QSpacerItem(50,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout *h_export_layout = new QHBoxLayout(main_vbox->widget());
    h_export_layout->setContentsMargins(0,0,0,0);
    h_export_layout->addWidget(label_tab_details);
    h_export_layout->addSpacerItem(spacer);


    pushbutton_show_all = new QPushButton(this);
    pushbutton_show_all->setText("Show All");
    QFont m_font_pushbutton_show_all = pushbutton_show_all->font();
    m_font_pushbutton_show_all.setPointSize(12);
    pushbutton_show_all->setFont(m_font_pushbutton_show_all);
    h_export_layout->addWidget(pushbutton_show_all);
    h_export_layout->setSpacing(4);

    menu_export_as = new QMenu(this);

    QStringList export_menuList;
    export_menuList << QString(MACRO_Generic_Export_As_CSV)
                    << QString(MACRO_Generic_Export_As_SQLite);
    create_right_click_actions(menu_export_as,export_menuList);

    connect(menu_export_as, SIGNAL(triggered(QAction*)), this, SLOT(slot_timeline_export_as_clicked(QAction*)));

    pushButton_export_table_data = new QPushButton(MACRO_Generic_Export_As , this);
    pushButton_export_table_data->setFixedWidth(100);
    pushButton_export_table_data->setHidden(true);

    pushButton_export_table_data->setMenu(menu_export_as);


    export_dialog_box_2_obj = new export_dialog_box_2(this);
    connect(export_dialog_box_2_obj, SIGNAL(signal_save_results_by_export_window(QString , QString,QString,QString)), this, SLOT(slot_table_data_export_dialog_ok_clicked(QString , QString, QString,QString)));




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

    h_export_layout->addWidget(pushButton_export_table_data);

    h_export_layout->addWidget(pushButton_save);
    h_export_layout->addWidget(pushButton_close_tab);

    main_vbox->addLayout(h_export_layout);

    main_vbox->setContentsMargins(0,0,0,0);
    main_vbox->setSpacing(1);
    main_vbox->setContentsMargins(0,0,0,0);


    tablewidget_timestamp_list = new QTableWidget(this);
    tablewidget_timestamp_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablewidget_timestamp_list->setAlternatingRowColors(true);

    tablewidget_timestamp_list->setGridStyle(Qt::NoPen);
    tablewidget_timestamp_list->setShowGrid(false);
    tablewidget_timestamp_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablewidget_timestamp_list->setSelectionMode(QAbstractItemView::SingleSelection);
    tablewidget_timestamp_list->setAutoScroll(true);
    QFont m_font = font();

    m_font.setPointSize(12);
    setFont(m_font);

    QString stylesheet = QString(
                "QTableView::item:selected:!active {"
                "color:white;"
                "background-color:  #a1c9f7;"
                "}"

                "QTableView::indicator:checked {background-color:black;border: 1px solid #d6d6d6;}"
                "QTableView::indicator:unchecked {background-color:#f1f1f1;border: 1px solid #777777;}"

                "QTableView::indicator {"
                "width: 13px;"
                "height: 13px;}"
                );


    tablewidget_timestamp_list->setStyleSheet(stylesheet);

    treewidget_plugins_list = new m_treewidget(this);
    treewidget_plugins_list->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(treewidget_plugins_list,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_m_treeWidget_plugins_itemClicked(QTreeWidgetItem*,int)));


    ///for Events and Timestamp list Group Set - Start
    prepare_timeline_groupbox();
    ///for Events and Timestamp list Group Set -End

    QHBoxLayout *hbox_Layout_tablewidget_gtimestamp_layout = new QHBoxLayout(this);
    hbox_Layout_tablewidget_gtimestamp_layout->addWidget(groupbox_event_and_timestamp,1);

    ///Layout for Graph View Set start
    radiobutton_create_graph_yearly = new QRadioButton("Yearly",this);
    radiobutton_create_graph_monthly = new QRadioButton("Monthly",this);
    radiobutton_create_graph_daywise = new QRadioButton("Daywise",this);
    radiobutton_create_graph_hourly = new QRadioButton("Hourly",this);

    radiobutton_create_graph_yearly->setFixedWidth(100);
    radiobutton_create_graph_monthly->setFixedWidth(100);
    radiobutton_create_graph_daywise->setFixedWidth(100);
    radiobutton_create_graph_hourly->setFixedWidth(100);

    pushbutton_export_graph = new QPushButton("Export" ,this);
    pushbutton_save_graph = new QPushButton("Save" ,this);

    save_graph_dialog_box_obj = new saved_graphs_dialog_box(this);
    connect(save_graph_dialog_box_obj, SIGNAL(signal_save_graph_name(QString)), this, SLOT(slot_save_graph_name_value(QString)));

    pushbutton_previous = new QPushButton(this);
    pushbutton_next = new QPushButton(this);
    pushbutton_previous->setFixedWidth(50);

    pushbutton_previous->setIcon(QIcon("../icons/signs/previous.png"));
    pushbutton_next->setIcon(QIcon("../icons/signs/next.png"));
    pushbutton_next->setFixedWidth(50);

    label_show_pages = new QLabel(this);
    label_show_pages->setAlignment(Qt::AlignCenter);
    label_show_pages->setFixedWidth(120);

    label_show_date_time = new QLabel(this);
    label_duration = new QLabel(this);
    label_duration->setText("Duration: ");
    label_show_date_time->setFixedWidth(150);

    label_goto_page = new QLabel(this);
    label_goto_page->setText("     Go to Page");
    label_goto_page->setFixedWidth(100);

    pushbutton_go_to_page = new QPushButton("Go",this);
    pushbutton_go_to_page->setMinimumSize(50,25);

    pushbutton_open_in_tableview = new QPushButton("Open in Tableview" , this);
    pushbutton_open_in_tableview->setMinimumSize(140,25);

    linedit_go_to_page = new QLineEdit(this);
    linedit_go_to_page->setFixedWidth(70);

    pushbutton_show_all->hide();


    connect(pushbutton_previous, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_previous_clicked(bool)));
    connect(pushbutton_next, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_next_clicked(bool)));
    connect(pushbutton_go_to_page, SIGNAL(clicked()), this, SLOT(slot_pushbutton_go_to_page_clicked()));
    connect(linedit_go_to_page,SIGNAL(textEdited(QString)),this,SLOT(slot_line_edit_go_to_page_text_changed(QString)));
    connect(linedit_go_to_page,SIGNAL(returnPressed()) , this, SLOT(slot_pushbutton_go_to_page_clicked()));
    connect(pushbutton_open_in_tableview, SIGNAL(clicked()), this, SLOT(slot_pushbutton_open_in_tableview_clicked()));
    connect(radiobutton_create_graph_yearly,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_yearly_clicked(bool)));
    connect(radiobutton_create_graph_monthly,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_monthly_clicked(bool)));
    connect(radiobutton_create_graph_daywise,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_daywise_clicked(bool)));
    connect(radiobutton_create_graph_hourly,SIGNAL(clicked(bool)),this,SLOT(slot_radioButton_hourly_clicked(bool)));
    connect(pushbutton_export_graph, SIGNAL(clicked()), this, SLOT(slot_pushbutton_export_graph_clicked()));
    connect(pushbutton_save_graph, SIGNAL(clicked()), this, SLOT(slot_pushbutton_save_graph_clicked()));


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
    hbox_radio_buttons_layout->addWidget(pushbutton_save_graph);
    hbox_radio_buttons_layout->addWidget(pushbutton_export_graph);
    hbox_radio_buttons_layout->addWidget(pushbutton_open_in_tableview);


    pushbutton_previous->setEnabled(false);
    pushbutton_next->setEnabled(false);

    hbox_radio_buttons_layout->setSpacing(2);


    stackwidget_obj = new QStackedWidget(this);
    QVBoxLayout *vbox_chartview_layout = new QVBoxLayout;
    vbox_chartview_layout->addWidget(stackwidget_obj);

    tabwidget_timeline_data_obj = new QTabWidget(this);
    connect(tabwidget_timeline_data_obj, SIGNAL(tabBarClicked(int)),this, SLOT(slot_tabbar_clicked(int)));

    QScrollArea *scrollarea_obj = new QScrollArea(tabwidget_timeline_data_obj);
    scrollarea_obj->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea_obj->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vbox_chartview_layout->setSizeConstraint(QLayout::SetMinimumSize);

    scrollarea_obj->setWidgetResizable(true);
    QWidget *container_widget = new QWidget;

    vbox_chartview_layout->addLayout(hbox_radio_buttons_layout);
    container_widget->setLayout(vbox_chartview_layout);
    scrollarea_obj->setWidget(container_widget);

    ///Layout for Graph View Set End


    ///--------
    tabwidget_timeline_data_obj->addTab(scrollarea_obj, tr("Graph View"));
    //    tabwidget_timeline_data_obj->addTab(m_tablewidget_obj, tr("Event View"));

    tabwidget_timeline_data_obj->tabBar()->hide();
    ///-----------

    hbox_Layout_tablewidget_gtimestamp_layout->addWidget(tabwidget_timeline_data_obj,1);
    hbox_Layout_tablewidget_gtimestamp_layout->setContentsMargins(0,0,0,0);
    hbox_Layout_tablewidget_gtimestamp_layout->setSpacing(1);
    hbox_Layout_tablewidget_gtimestamp_layout->setContentsMargins(0,0,0,0);

    main_vbox->addLayout(hbox_Layout_tablewidget_gtimestamp_layout);

    tablewidget_timestamp_list->horizontalHeader()->setStretchLastSection(true);

    menu_graphs = new QMenu(this);
    act_show_data_of_current_plugin_barset = new QAction("show data of current plugin",this);
    act_show_all_data = new QAction("show all data",this);

    menu_graphs->addAction(act_show_data_of_current_plugin_barset);
    menu_graphs->addAction(act_show_all_data);
    connect(act_show_data_of_current_plugin_barset, SIGNAL(triggered()), this, SLOT(slot_act_show_data_of_current_plugin_triggered()));
    connect(act_show_all_data, SIGNAL(triggered()), this, SLOT(slot_act_show_all_data_triggered()));

    disable_enable_initial_ui(false);

    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::prepare_timeline_groupbox()
{ // prepare timelines of group box like search plugins widget, scroll area, search timestamps, lineedit search etc

    ///Done for Events and Timestamp list - Start

    tabwidget_timeline_filter_obj = new QTabWidget(this);

    tabwidget_timeline_artifacts_obj = new QTabWidget(this);


    ///---added Event List tab
    checkbox_plugins = new QCheckBox(this);
    checkbox_plugins->setFixedWidth(30);
    checkbox_plugins->setFixedHeight(20);

    lineEdit_search_plugins = new QLineEdit(this);
    connect(lineEdit_search_plugins, SIGNAL(textChanged(QString)),this,SLOT(slot_lineEdit_plugin_search_textChanged(QString)));
    lineEdit_search_plugins->setPlaceholderText("Plugin Search");

    QHBoxLayout *hsearch_lay = new QHBoxLayout(this);
    hsearch_lay->addWidget(checkbox_plugins);
    hsearch_lay->addWidget(lineEdit_search_plugins);
    hsearch_lay->setSpacing(1);
    QVBoxLayout *vbox_lay = new QVBoxLayout(this);
    vbox_lay->addLayout(hsearch_lay);
    vbox_lay->addWidget(treewidget_plugins_list);
    vbox_lay->setSpacing(1);
    connect(checkbox_plugins, SIGNAL(clicked(bool)), this, SLOT(slot_checkBox_plugin_clicked(bool)));


    QScrollArea *scrollarea_plugins_obj = new QScrollArea(tabwidget_timeline_artifacts_obj);

    connect(tabwidget_timeline_artifacts_obj, SIGNAL(tabBarClicked(int)),this, SLOT(slot_sources_tabbar_clicked(int)));


    scrollarea_plugins_obj->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea_plugins_obj->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vbox_lay->setSizeConstraint(QLayout::SetMinimumSize);
    scrollarea_plugins_obj->setWidgetResizable(true);
    QWidget *plugin_container_widget = new QWidget;
    plugin_container_widget->setLayout(vbox_lay);
    scrollarea_plugins_obj->setWidget(plugin_container_widget);

    //  tabwidget_timeline_artifacts_obj->addTab(tablewidget_filesystem_sources, "Sources List");
    tabwidget_timeline_artifacts_obj->addTab(scrollarea_plugins_obj, "Artifacts List");




    vbox_lay->setContentsMargins(1,1,1,1);
    scrollarea_plugins_obj->setContentsMargins(1,1,1,1);

    ///-------------


    ///---added 'Timestamp List' tab

    lineEdit_search_timestamps = new QLineEdit(this);
    connect(lineEdit_search_timestamps, SIGNAL(textChanged(QString)),this,SLOT(slot_lineEdit_timestamp_search_textChanged(QString)));
    lineEdit_search_timestamps->setPlaceholderText("Timestamp Search");

    connect(tablewidget_timestamp_list->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_timestamps_horizontal_header_clicked(int)));

    QVBoxLayout *vbox_time_lay = new QVBoxLayout(this);
    vbox_time_lay->addWidget(lineEdit_search_timestamps);
    vbox_time_lay->addWidget(tablewidget_timestamp_list);
    vbox_time_lay->setSpacing(1);

    QScrollArea *scrollarea_time_obj = new QScrollArea(tabwidget_timeline_filter_obj);
    scrollarea_time_obj->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea_time_obj->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vbox_time_lay->setSizeConstraint(QLayout::SetMinimumSize);
    scrollarea_time_obj->setWidgetResizable(true);
    QWidget *time_container_widget = new QWidget;
    time_container_widget->setLayout(vbox_time_lay);
    scrollarea_time_obj->setWidget(time_container_widget);
    tabwidget_timeline_filter_obj->addTab(scrollarea_time_obj, "Timestamp List");

    vbox_time_lay->setSpacing(1);
    vbox_time_lay->setContentsMargins(1,1,1,1);
    scrollarea_time_obj->setContentsMargins(1,1,1,1);
    ///-------------



    label_artifacts_start_date = new QLabel("Start", this);
    label_artifact_end_date = new QLabel("End", this);

    datetimeedit_start = new QDateTimeEdit(this);
    datetimeedit_end =new QDateTimeEdit(this);

    datetimeedit_start->setFixedWidth(200);
    datetimeedit_end->setFixedWidth(200);




    QString saved_timestamp = recon_helper_standard_obj->get_string_from_db_by_dbpath("select timestamp from saved_events_timestamp", 0,destination_timeline_saved_db_path,Q_FUNC_INFO);

    QStringList list = saved_timestamp.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
    if(list.size() <= 1)
    {
        QDate set_date;
        set_date.setDate(2013,1,1);
        QDateTime start_date_time;
        start_date_time.setTimeSpec(Qt::UTC);
        start_date_time.setDate(set_date);

        datetimeedit_start->setDateTime(start_date_time);
        datetimeedit_end->setDateTime(QDateTime::currentDateTime());
    }
    else
    {

        quint64 start_time = list.at(0).toULongLong();
        quint64 end_time   = list.at(1).toULongLong();

        QDateTime start_dttime = start_dttime.fromSecsSinceEpoch(start_time);
        QDateTime end_dttime = end_dttime.fromSecsSinceEpoch(end_time);

        datetimeedit_start->setDateTime(start_dttime);
        datetimeedit_end->setDateTime(end_dttime);

        //        datetimeedit_start->setDateTime(recon_helper_obj->convert_epoch_to_custom_for_display(QString::number(start_time),Q_FUNC_INFO));
        //        datetimeedit_end->setDateTime(recon_helper_obj->convert_epoch_to_custom_for_display(QString::number(end_time),Q_FUNC_INFO));
    }


    pushbutton_apply = new QPushButton("Apply",this);
    pushbutton_apply->setFixedWidth(150);
    connect(pushbutton_apply, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_apply_clicked(bool)));
    QVBoxLayout *vbox_Layout_artifacts_dates = new QVBoxLayout(this);
    QHBoxLayout *hbox_Layout_artifacts_start_dates = new QHBoxLayout(this);
    QHBoxLayout *hbox_Layout_artifacts_end_dates = new QHBoxLayout(this);


    vbox_Layout_artifacts_dates->addWidget(tabwidget_timeline_artifacts_obj);

    vbox_Layout_artifacts_dates->addWidget(tabwidget_timeline_filter_obj);

    datetimeedit_start->setDisplayFormat("dd-MMM-yyyy hh:mm:ss");
    hbox_Layout_artifacts_start_dates->addWidget(label_artifacts_start_date);
    hbox_Layout_artifacts_start_dates->addWidget(datetimeedit_start);

    datetimeedit_end->setDisplayFormat("dd-MMM-yyyy hh:mm:ss");
    hbox_Layout_artifacts_end_dates->addWidget(label_artifact_end_date);
    hbox_Layout_artifacts_end_dates->addWidget(datetimeedit_end);

    vbox_Layout_artifacts_dates->addLayout(hbox_Layout_artifacts_start_dates);
    vbox_Layout_artifacts_dates->addLayout(hbox_Layout_artifacts_end_dates);


    QSpacerItem *spacer1 = new QSpacerItem(25, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(25, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *hbox_Layout_artifacts_pushbutton_apply = new QHBoxLayout(this);
    hbox_Layout_artifacts_pushbutton_apply->addSpacerItem(spacer1);
    hbox_Layout_artifacts_pushbutton_apply->addWidget(pushbutton_apply);
    hbox_Layout_artifacts_pushbutton_apply->addSpacerItem(spacer2);

    vbox_Layout_artifacts_dates->addLayout(hbox_Layout_artifacts_pushbutton_apply);


    vbox_Layout_artifacts_dates->setSpacing(1);
    hbox_Layout_artifacts_end_dates->setSpacing(1);
    hbox_Layout_artifacts_start_dates->setSpacing(1);


    groupbox_event_and_timestamp = new QGroupBox(this);
    groupbox_event_and_timestamp->setLayout(vbox_Layout_artifacts_dates);
    groupbox_event_and_timestamp->setFixedWidth(290);
    vbox_Layout_artifacts_dates->setContentsMargins(0,0,0,0);


}


void artifacts_timeline_full_loader::display_timestamps_list()
{ // display all the timestamp list
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    tablewidget_timestamp_list->setRowCount(0);

    tablewidget_timestamp_list->setColumnCount(3);
    tablewidget_timestamp_list->verticalHeader()->hide();

    tablewidget_timestamp_list->setColumnWidth(enum_artifacts_timeline_tablewidget_timestamps_list_stamp_type, 140);
    tablewidget_timestamp_list->setColumnWidth(enum_artifacts_timeline_tablewidget_timestamps_list_stamp_name, 300);

    QStringList header;

    header << "Type" << "Name" << "Feature";

    tablewidget_timestamp_list->setHorizontalHeaderLabels(header);
    tablewidget_timestamp_list->horizontalHeaderItem(0)->setIcon(QIcon("../icons/Unchecked.png"));
    tablewidget_timestamp_list->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);


    tablewidget_timestamp_list->hideColumn(2);

    QStringList timestamp_db_list;
    timestamp_db_list << destination_timeline_db_path;
    //    for(int row = 0; row < tablewidget_filesystem_sources->rowCount(); row++)
    //    {

    //        QTableWidgetItem *item_source_count_name = tablewidget_filesystem_sources->item(row , 1);

    //        QString fs_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
    //        QString fs_timeline_db_path = fs_result_dir_path  + "file_system_timeline_full_" + item_source_count_name->text() + ".sqlite";

    //        timestamp_db_list << fs_timeline_db_path;

    //        /// do not remove break
    //        break;

    //    }


    QString fs_compare_str = QString(MACRO_Plugin_Name_File_System);
    fs_compare_str.replace(" " , "_");
    fs_compare_str = fs_compare_str.toLower();

    for(int count = 0; count < timestamp_db_list.size(); count++)
    {

        QString db_path = timestamp_db_list.at(count);

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug("DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
            return;
        }

        QSqlQuery query_list(destination_db);


        QString timestamp_types = recon_helper_standard_obj->get_string_from_db_by_dbpath("select timestamp_type from saved_events_timestamp", 0,destination_timeline_saved_db_path ,Q_FUNC_INFO);
        QStringList bk_stamp_type_list = timestamp_types.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        query_list.prepare("select stamp_type,stamp_name from timeline_data GROUP BY stamp_type ");

        if(!query_list.exec())
        {
            recon_static_functions::app_debug(" query_list.exec()  ------FAILED-----" + destination_timeline_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(query_list.lastError().text() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(query_list.executedQuery(),Q_FUNC_INFO);
            return;
        }

        int row = 0;
        while(query_list.next())
        {

            QTableWidgetItem *item_stamp_type = new QTableWidgetItem;
            item_stamp_type->setText(QString(query_list.value(0).toString()));

            tablewidget_timestamp_list->insertRow(row);
            tablewidget_timestamp_list->setRowHeight(row,21);
            tablewidget_timestamp_list->setItem(row,0,item_stamp_type);

            if(bk_stamp_type_list.contains(query_list.value(0).toString()))
                item_stamp_type->setCheckState(Qt::Checked);
            else
                item_stamp_type->setCheckState(Qt::Unchecked);


            QTableWidgetItem *item_stamp_name = new QTableWidgetItem;
            item_stamp_name->setText(QString(query_list.value(1).toString()));
            tablewidget_timestamp_list->setItem(row,1,item_stamp_name);


            QTableWidgetItem *item_feature = new QTableWidgetItem;


            if(QFileInfo(db_path).fileName().startsWith(fs_compare_str))
                item_feature->setText(QString(MACRO_Plugin_Name_File_System));
            else
            {
                item_feature->setText(QString(""));
                // tablewidget_timestamp_list->hideRow(row);
            }


            tablewidget_timestamp_list->setItem(row,2,item_feature);


            row++;
        }

        destination_db.close();

    }

    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);
}


void artifacts_timeline_full_loader::update_bookmark_status_for_all(QString status)
{ // update bookmark status in database
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" -Destination Db opening ----FAILED---- " + destination_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    QString bookmark_col_name = db_columns_list.at(bookmark_index);

    int timestamp_type_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACTS_TIMELINE_TIMESTAMP_TYPE_QString,0);

    if(timestamp_type_index < 0)
        return;

    QString timestamp_type_column_name = db_columns_list.at(timestamp_type_index);
    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbreference(true, destination_db, QStringList(), Q_FUNC_INFO);
    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;


    if(table_name_list.size() > 0)
    {
        QString table_name = table_name_list.at(0);

        for(int i = 0; i < m_tablewidget_obj->rowCount(); i++)
        {
            if(i % 500 == 0)
                QCoreApplication::processEvents();

            total_covered_count++;

            if(status == QString("1"))
                m_tablewidget_obj->item(i, bookmark_index)->setCheckState(Qt::Checked);
            else
                m_tablewidget_obj->item(i, bookmark_index)->setCheckState(Qt::Unchecked);

            QString record_no = m_tablewidget_obj->item(i, record_no_index)->text();
            QString plugin_name_str = m_tablewidget_obj->item(i, plugin_name_index)->text();
            QString tab_name_str = m_tablewidget_obj->item(i, tab_name_index)->text();
            QString timestamp_type_val = m_tablewidget_obj->item(i,timestamp_type_index)->text();
            QString os_name_str = m_tablewidget_obj->item(i, os_name_index)->text();

            QString record_no_column_name = db_columns_list.at(record_no_index);
            QString plugin_column_name = db_columns_list.at(plugin_name_index);
            QString tab_column_name = db_columns_list.at(tab_name_index);
            QString os_scheme_column_name = db_columns_list.at(os_name_index);

            QString command = "UPDATE '" + table_name + "' SET " + bookmark_col_name + "=?" " WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + timestamp_type_column_name + "=? AND " + os_scheme_column_name + "=?";
            QStringList arg_list;
            arg_list << status << record_no << plugin_name_str << tab_name_str << timestamp_type_val << os_name_str;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);
        }
    }


    destination_db.close();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::update_note_in_notes_serach(QString text_str, int selected_row, QString dest_db_path)
{ // update notes in database for notes search
    if(text_str.trimmed().isEmpty())
        return ;

    QString plugin_name_str = m_tablewidget_obj->item(selected_row, plugin_name_index)->text().trimmed();
    QString record_no_str = m_tablewidget_obj->item(selected_row, record_no_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(selected_row, tab_name_index)->text();
    QString stamp_type_str = m_tablewidget_obj->item(selected_row, stamp_type_index)->text();

    QString source_count_name = m_tablewidget_obj->item(selected_row, source_count_name_index)->text();

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase timeline_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    timeline_db.setDatabaseName(dest_db_path);
    if(!timeline_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + timeline_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + timeline_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QString command  = QString("SELECT record_no,category,table_name,timestamp,item1,item2,plugin,stamp_type,stamp_name, bookmark,recon_tag_value,notes,os_scheme_display from timeline_data WHERE record_no=? AND plugin=? AND category=? AND stamp_type=? AND source_count_name=?");
    QSqlQuery query(timeline_db);
    query.prepare(command);
    query.addBindValue(record_no_str);
    query.addBindValue(plugin_name_str);
    query.addBindValue(tab_name_str);
    query.addBindValue(stamp_type_str);
    query.addBindValue(source_count_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select ------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        timeline_db.close();
        return;
    }
    if(!query.next())
    {
        recon_static_functions::app_debug(" No Record Exist ",Q_FUNC_INFO);
        return ;
    }

    QString command_already_record = "SELECT INT FROM notes WHERE plugin_name=? AND tab_name=? AND record_no=? AND item0=? AND item1=? AND item2=? AND source_count_name=?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                      << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                      << record_no_str
                      << plugin_name_str
                      << tab_name_str
                      << stamp_type_str
                      << source_count_name;

    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,notes_db_path,Q_FUNC_INFO);
    if(INT_str.isEmpty())
    {
        QStringList arg_list;
        QString command_insert = "INSERT INTO notes(plugin_name,category,item0,item1,item2,table_name,tab_name,record_no"
                                 ",item3,item4,item5,item6,item7,bookmark,recon_tag_value,"
                                 "notes,os_scheme_display , source_count_name)"
                                 "VALUES (?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?,?,?,?)";


        ///----note insertion in item1 , item2 , item3 is fixed to make unique records for same kind of timestamp and record no
        arg_list   << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                   << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                   << query.value("plugin").toString()
                   << query.value("category").toString()
                   << stamp_type_str
                   << query.value("table_name").toString()
                   << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                   << query.value("record_no").toString()
                   << global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value("timestamp").toString(),Q_FUNC_INFO)
                   << query.value("item1").toString()
                   << query.value("item2").toString()
                   << query.value("stamp_type").toString()
                   << query.value("stamp_name").toString()
                   << query.value("bookmark").toString()
                   << query.value("recon_tag_value").toString()
                   << query.value("notes").toString()
                   << query.value("os_scheme_display").toString()
                   << source_count_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);
    }
    else
    {

        QString command_update = "UPDATE notes SET notes = ? WHERE record_no=? AND plugin_name = ? AND category = ? AND item0=? AND item1=? AND item2=? AND source_count_name=?";
        QStringList arg_list;
        arg_list << text_str << record_no_str
                 << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                 << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                 << plugin_name_str
                 << tab_name_str
                 << stamp_type_str
                 << source_count_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,notes_db_path,Q_FUNC_INFO);
    }

    timeline_db.close();
}

void artifacts_timeline_full_loader::update_tags_value(QString tag_data, QString colour)
{ // update tags value in database
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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


        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_column_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_set_tag_in_artifact_timeline_db(INT_List, tag_data);
    global_connection_manager_obj->pub_add_tag_for_artifact_timeline(timeline_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_full_loader::slot_act_remove_tags()
{ //action remove tags
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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


        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_column_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_remove_tag_in_artifact_timeline_db(INT_List);
    global_connection_manager_obj->pub_delete_tag_for_artifact_timeline(timeline_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_full_loader::delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag , QString record_no , QString plugin_name , QString tab_name ,QString stamp_type_str, QString source_count_name)
{ // delete tags from tag search database while updating tags in database except current record/tag
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
        QStringList arg_values;
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + db_name + ".sqlite";
        QString select_cmd = "Select INT from tags where record_no = ? AND plugin_name = ? AND category = ?  AND item0=? AND item1=? AND item2=? AND source_count_name=?";
        arg_values << record_no
                   << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                   << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                   << plugin_name
                   << tab_name
                   << stamp_type_str
                   << source_count_name;

        QString int_str =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,arg_values,0,db_path,Q_FUNC_INFO);
        if(int_str == "0")
            continue;

        QStringList arg_list;
        command = "Delete from tags where record_no = ? AND plugin_name = ? AND category = ?  AND item0=? AND item1=? AND item2=? AND source_count_name=?";
        arg_list << record_no
                 << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                 << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                 << plugin_name
                 << tab_name
                 << stamp_type_str
                 << source_count_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

        ///2.---refresh case tree tag
        emit signal_common_add_tag_name_into_case_tree(db_name);
    }

}

void artifacts_timeline_full_loader::update_tags_in_tag_serach_db(QString current_tag , QString dest_db_path)
{ // update tags in tags db when perform action on right click

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(current_tag,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase timeline_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    timeline_db.setDatabaseName(dest_db_path);
    if(!timeline_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + timeline_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + timeline_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

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


        QString record_no_str = m_tablewidget_obj->item(selected_row_no,record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no,plugin_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no,tab_name_index)->text();
        QString stamp_type_str = m_tablewidget_obj->item(selected_row_no,stamp_type_index)->text();

        QString source_count_name = m_tablewidget_obj->item(selected_row_no,source_count_name_index)->text();

        QString command  = QString("SELECT record_no,category,table_name,timestamp,item1,item2,plugin,stamp_type,stamp_name, bookmark,recon_tag_value,notes,os_scheme_display from timeline_data WHERE record_no=? AND plugin=? AND category=? AND stamp_type=? AND source_count_name=?");
        QSqlQuery query(timeline_db);
        query.prepare(command);
        query.addBindValue(record_no_str);
        query.addBindValue(plugin_name_str);
        query.addBindValue(tab_name_str);
        query.addBindValue(stamp_type_str);
        query.addBindValue(source_count_name);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query select ------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
            timeline_db.close();
            return;
        }
        if(!query.next())
        {
            recon_static_functions::app_debug(" No Record Exist ",Q_FUNC_INFO);
            continue;
        }

        QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND item1=? AND item2=? AND item3=? AND source_count_name=?";
        QStringList arg_list_for_tags;
        arg_list_for_tags << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                          << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                          << record_no_str
                          << plugin_name_str
                          << tab_name_str
                          << stamp_type_str
                          <<source_count_name;

        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);
        if(INT_str.isEmpty())
        {
            QStringList arg_list;
            QString command_insert = "INSERT INTO tags(plugin_name,category,item0,item1,item2,table_name,tab_name,record_no"
                                     ",item3,item4,item5,item6,item7,bookmark,recon_tag_value,"
                                     "notes,os_scheme_display,csv_plugin_name,csv_tab_name , source_count_name)"
                                     "VALUES (?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?,?,?,?,?,?)";


            ///----note insertion in item1 , item2 , item3 is fixed to make unique records for same kind of timestamp and record no
            arg_list   << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                       << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                       << query.value("plugin").toString()
                       << query.value("category").toString()
                       << stamp_type_str
                       << query.value("table_name").toString()
                       << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                       << query.value("record_no").toString()
                       << global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value("timestamp").toString(),Q_FUNC_INFO)
                       << query.value("item1").toString()
                       << query.value("item2").toString()
                       << query.value("stamp_type").toString()
                       << query.value("stamp_name").toString()
                       << query.value("bookmark").toString()
                       << query.value("recon_tag_value").toString()
                       << query.value("notes").toString()
                       << query.value("os_scheme_display").toString()
                       << QString(MACRO_Plugin_Name_Artifacts_Timeline_Full)
                       << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Timeline)
                       << source_count_name;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);
        }
        else
        {

            QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE record_no=? AND plugin_name = ? AND category = ? AND item0=? AND item1=? AND item2=? AND source_count_name=?";

            QStringList arg_list;
            arg_list << current_tag << record_no_str
                     << QString(MACRO_Plugin_Name_Artifacts_Timeline)
                     << QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event)
                     << plugin_name_str
                     << tab_name_str
                     << stamp_type_str
                     <<source_count_name;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);
        }

    }

    timeline_db.close();



}

void artifacts_timeline_full_loader::update_notes_value(QString text, int current_row)
{ // update notes value in db from right click

    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);

    QTableWidgetItem *item_tablewidget_notes_icon = m_tablewidget_obj->item(current_row,notes_icon_index);



    QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();
    QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
    QString stamp_type_str = m_tablewidget_obj->item(current_row, stamp_type_index)->text();
    QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();

    QString record_no_column_name = db_columns_list.at(record_no_index);
    QString plugin_col_name = db_columns_list.at(plugin_name_index);
    QString table_col_name = db_columns_list.at(tab_name_index);
    QString os_col_name = db_columns_list.at(os_name_index);


    QString table_name = table_name_list.at(0);

    QString timeline_db_path;
    QString timeline_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
    if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString source_count_str = m_tablewidget_obj->item(current_row, source_count_name_index)->text().trimmed();
        timeline_db_path = timeline_result_dir + "file_system_timeline_full_" + source_count_str + ".sqlite";
    }
    else
    {
        timeline_db_path = timeline_result_dir + "artifacts_timeline_full.sqlite";
    }

    QString command = "update '" + table_name + "' set notes =? WHERE " + record_no_column_name + "=? AND " + plugin_col_name + "=? AND " + table_col_name + "=? AND " + os_col_name + "=? AND stamp_type=?";
    QStringList arg_list;
    arg_list << text << record_no << plugin_name_str << tab_name_str << os_name_str << stamp_type_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,timeline_db_path,Q_FUNC_INFO);


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

    update_note_in_notes_serach(text,current_row,timeline_db_path);

    emit signal_common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);

    //    emit signal_update_notes_status_in_tab_generated_by_timeline_graph(record_no,plugin_name_str,tab_name_str,text,QString(MACRO_Plugin_Name_Artifacts_Timeline),os_name_str);
    //    emit signal_update_tags_status_in_tab_generated_by_timeline_graph(record_no,plugin_name_str,tab_name_str,QString(MACRO_Plugin_Name_Artifacts_Timeline),applied_tags,os_name_str);
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void artifacts_timeline_full_loader::tablewidget_general_clicked(int row,int column)
{ // table widget randomly click on any record. Can get info of that record
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if((row < 0) || (column < 0) || (m_tablewidget_obj == NULL))
        return;

    if(!m_tablewidget_obj->selectionModel()->selectedRows(column).isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }

    QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text();
    QString record_no_timeline_INT = m_tablewidget_obj->item(row, index_for_INT_only_for_artifact_timeline)->text();

    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();
    QString os_name_str = m_tablewidget_obj->item(row, os_name_index)->text();


    QString searched_text = searched_keyword_str;
    struct_global_fetch_metadata struct_metadata_values_obj;

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString timeline_table_name;
    timeline_table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name,(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Timeline));
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins(timeline_table_name,0,record_no_timeline_INT,plugin_name,destination_timeline_db_path, "" ,struct_info, Q_FUNC_INFO);

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index != -1)
    {
        if(column == bookmark_index)
        {
            if(m_tablewidget_obj->item(row, bookmark_index)->checkState() == Qt::Checked)
                update_bookmark_value(QString("1"),row,bookmark_index);
            else if(m_tablewidget_obj->item(row, bookmark_index)->checkState() == Qt::Unchecked)
                update_bookmark_value(QString("0"),row,bookmark_index);
        }
    }

    current_media_path = "";
    preview_file_path  = "";
    metadata_str       = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = searched_text;
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = tab_name_str;

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::action_open_with_plist_viewer_triggered()
{ // action open with plist viewer click from right click
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

    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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
    if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
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

void artifacts_timeline_full_loader::action_open_with_hex_viewer_triggered()
{ // open with hex view from right click for hex analysis for that particluar record
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString os_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), os_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString source_count_name;
    QString complete_file_path;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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


        struct_global_artifacts_export_fileinfo st_globl_recon_file_info_obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);

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

        emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);
    }


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::action_open_with_sqlite_viewer_triggered()
{ // open that record with sqlite viewer for sql analysis
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString os_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), os_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString source_count_name;
    QString complete_file_path;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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

void artifacts_timeline_full_loader::action_export_triggered()
{ // action export button click from right click menu to export that record
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


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;


        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::action_sendToBucket_plist_viewer_triggered()
{ // send to bucket button from menu for plist viewer
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


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;


        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString source_count_name;
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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
            if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
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

void artifacts_timeline_full_loader::action_sendToBucket_hex_viewer_triggered()
{ // send to bucket for hex viewer analysis using menu
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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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


            struct_global_artifacts_export_fileinfo st_globl_recon_file_info_obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
            ///check file in bucket
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

void artifacts_timeline_full_loader::action_sendToBucket_sqlite_viewer_triggered()
{ // send to bucket for sqlite viewer analysis from menu
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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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

void artifacts_timeline_full_loader::action_open_full_detailed_info_triggered()
{ // open full detail info of that particlar record from right click menu
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

    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;


    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    int preview_index_full = 0;
    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str,record_no ,plugin_db_file);
    QString file_path = st_globl_recon_file_info_obj.complete_file_path;

    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    preview_index_full = get_stack_index_for_preview(file_path ,display_file_path);

    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);

    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"", record_no, plugin_name_str, "");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::action_remove_note_triggered()
{ //remove tag of any record from right click menu
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

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

        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,notes_column_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_remove_notes_in_artifact_timeline_db(INT_List);
    global_connection_manager_obj->pub_delete_notes_for_artifact_timeline(timeline_record_list);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_full_loader::action_remove_note_bookmark_triggered()
{ // remove note bookmark clicked
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    add_notes_to_bookmarks_obj->hide();
    QString text = "";

    int i  = m_tablewidget_obj->rowCount();
    for(int k = 0 ; k < i ; k++)
    {
        if(k % 300 == 0)
        {
            QCoreApplication::processEvents();
        }

        int icon_notes_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
        int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        if(m_tablewidget_obj->item(k, bookmark_position)->checkState())
        {
            if(text.isEmpty())
            {
                QIcon icon_notes("");
                m_tablewidget_obj->item(k, icon_notes_position)->setIcon(icon_notes);
            }
            else
            {
                QIcon icon_notes("../icons/notes.png");
                m_tablewidget_obj->item(k, icon_notes_position)->setIcon(icon_notes);
            }

            QString record = m_tablewidget_obj->item(k, record_no_index)->text();
            QString os_name_str = m_tablewidget_obj->item(k , os_name_index)->text();

            update_notes_value(text,k);

            emit signal_update_display_on_remove_notes(QString(MACRO_Plugin_Name_Artifacts_Timeline),QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event), record, "", os_name_str);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::action_open_detach_detailed_info_triggered()
{ // not in use
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

    //    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    //    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    //    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    //    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();
    //    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);


    //    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
    //        return;

    //    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
    //    QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);


    //    QStringList all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);
    //    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

    //    int preview_index_full = 0;
    //    if(os_name_str == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
    //        preview_index_full = get_stack_index_for_preview(preview_file_path);
    //    else
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //    if(all_path_list.size() > 0)
    //    {
    //        QString export_file_path = all_path_list.at(0);

    //        if(os_name_str == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, export_file_path, preview_file_path, preview_index_full,searched_keyword,source_count_name);
    //        else
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, export_file_path, current_media_path, preview_index_full,searched_keyword,source_count_name);
    //    }
    //    else
    //    {
    //        if(os_name_str == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, "", preview_file_path, preview_index_full,searched_keyword,source_count_name);
    //        else
    //            emit signal_open_in_detach_clicked_table_result(metadata_str, "", current_media_path, preview_index_full,searched_keyword,source_count_name);
    //    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::set_bookmarked_timestamp_type_list()
{ // after filter apply to display bookmarks data, it will show bookmark timestamp type list
    recon_static_functions::app_debug("", Q_FUNC_INFO);

    bookmarked_timestamp_type_list.clear();
    bookmarked_timestamp_name_list.clear();

    for(int i = 0; i < tablewidget_timestamp_list->rowCount(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        if(tablewidget_timestamp_list->item(i, enum_artifacts_timeline_tablewidget_timestamps_list_stamp_type)->checkState())
        {
            QString stamp_type = tablewidget_timestamp_list->item(i, enum_artifacts_timeline_tablewidget_timestamps_list_stamp_type)->text();
            QString stamp_name = tablewidget_timestamp_list->item(i, enum_artifacts_timeline_tablewidget_timestamps_list_stamp_name)->text();

            bookmarked_timestamp_type_list << stamp_type;
            bookmarked_timestamp_name_list << stamp_name;

        }

    }


    if(bookmarked_timestamp_type_list.isEmpty())
        return;

    recon_helper_standard_obj->execute_db_command_by_dbpath("delete from saved_events_timestamp", destination_timeline_saved_db_path,Q_FUNC_INFO);

    // saving in db with timezone, when gui loads from db - then qt converted it into UTC
    QString timestamp = QString::number(datetimeedit_start->dateTime().toSecsSinceEpoch()) + MACRO_RECON_Splitter_1_multiple + QString::number(datetimeedit_end->dateTime().toSecsSinceEpoch());
    QStringList list;
    list << timestamp << bookmarked_timestamp_type_list.join(MACRO_RECON_Splitter_1_multiple) << bookmarked_timestamp_name_list.join(MACRO_RECON_Splitter_1_multiple);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("insert into saved_events_timestamp(timestamp,timestamp_type, timestamp_name) values(?,?,?)", list ,destination_timeline_saved_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("", Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::display_data_of_graph_tablewidget(QString dest_db_path)
{// display data of graph tablewidget
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    bookmark_column_index = 0;
    notes_column_index    = 1;
    tag_column_index      = 2;
    timestamp_index = 3;
    stamp_type_index = 4;
    record_no_index = 5;
    plugin_name_index = 6;
    tab_name_index = 7;
    os_name_index = 8;
    item_1_index = 9;
    item_2_index = 10;

    source_count_name_index = 12;
    index_for_INT_only_for_artifact_timeline = 13;


    m_tablewidget_obj->hideColumn(source_count_name_index);

    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(index_for_INT_only_for_artifact_timeline);


    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")
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

    db_cmd.append("where plugin = ? AND (");

    for(int tab = 0; tab < bookmarked_plugin_tab_list.size(); tab++)
    {
        db_cmd.append(" category = ? OR ");
    }

    if(db_cmd.endsWith("OR "))
    {
        db_cmd.chop(3);
        db_cmd.append(") AND (");
    }

    for(int tab = 0; tab < bookmarked_timestamp_type_list.size(); tab++)
    {
        db_cmd.append(" stamp_type = ? OR ");
    }

    if(db_cmd.endsWith("OR "))
    {
        db_cmd.chop(3);
        db_cmd.append(")");
    }


    QString time_start_str , time_end_str;
    if(radiobutton_create_graph_yearly->isChecked())
    {
        //2017
        QString start_year = current_barset_time_str; // by default starts from Jan 01
        QString end_year   = QString("31.12.") + current_barset_time_str;

        QDateTime date_time;
        date_time = QDateTime::fromString(start_year, "yyyy");
        date_time.setTimeSpec(Qt::UTC);
        qint64 start_epch = date_time.toSecsSinceEpoch();
        if(start_epch < start_timestamp_utc_qint64)
            start_epch = start_timestamp_utc_qint64;

        date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
        date_time.setTimeSpec(Qt::UTC);
        qint64 end_epch  = date_time.toSecsSinceEpoch();
        if(end_epch > end_timestamp_utc_qint64)
            end_epch = end_timestamp_utc_qint64;


        time_start_str = QString::number(start_epch);
        time_end_str = QString::number(end_epch);
    }


    if(radiobutton_create_graph_monthly->isChecked())
    {
        // Jan-2017
        QString start_year = QString("01-") + current_barset_time_str;
        QString end_year   = QString("31-12-") + current_barset_time_str;

        QStringList year_split_list = current_barset_time_str.split("-");
        QString year_str;
        if(year_split_list.size() > 1)
            year_str = year_split_list.at(1);

        bool is_leap_year = QDate::isLeapYear(year_str.toInt());

        if(current_barset_time_str.startsWith("Feb-") && is_leap_year)
            end_year   = QString("29-") + current_barset_time_str;
        else if(current_barset_time_str.startsWith("Feb-") && !is_leap_year)
            end_year   = QString("28-") + current_barset_time_str;
        else
        {
            if(current_barset_time_str.startsWith("Jan-") || current_barset_time_str.startsWith("Mar-") || current_barset_time_str.startsWith("May-") || current_barset_time_str.startsWith("Jul-") || current_barset_time_str.startsWith("Aug-")
                    || current_barset_time_str.startsWith("Oct-") || current_barset_time_str.startsWith("Dec-"))
            {
                end_year = QString("31-") + current_barset_time_str;
            }
            else
            {
                end_year = QString("30-") + current_barset_time_str;
            }
        }

        QDateTime date_time;
        date_time = QDateTime::fromString(start_year, "dd-MMM-yyyy");
        date_time.setTimeSpec(Qt::UTC);
        qint64 start_epch = date_time.toSecsSinceEpoch();
        if(start_epch < start_timestamp_utc_qint64)
            start_epch = start_timestamp_utc_qint64;

        date_time = QDateTime::fromString(end_year, "dd-MMM-yyyy");
        date_time.setTimeSpec(Qt::UTC);
        qint64 end_epch  = date_time.toSecsSinceEpoch();
        if(end_epch > end_timestamp_utc_qint64)
            end_epch = end_timestamp_utc_qint64;

        time_start_str = QString::number(start_epch);
        time_end_str = QString::number(end_epch);
    }

    if(radiobutton_create_graph_daywise->isChecked())
    {
        QString start_year = current_barset_time_str + QString(" 00:00:00");
        QString end_year   = current_barset_time_str + QString(" 23:59:59");

        QDateTime date_time;
        date_time = QDateTime::fromString(start_year, "ddd dd-MMM-yyyy hh:mm:ss");
        date_time.setTimeSpec(Qt::UTC);
        qint64 start_epch = date_time.toSecsSinceEpoch();
        if(start_epch < start_timestamp_utc_qint64)
            start_epch = start_timestamp_utc_qint64;


        date_time = QDateTime::fromString(end_year, "ddd dd-MMM-yyyy hh:mm:ss");
        date_time.setTimeSpec(Qt::UTC);
        qint64 end_epch  = date_time.toSecsSinceEpoch();
        if(end_epch > end_timestamp_utc_qint64)
            end_epch = end_timestamp_utc_qint64;


        time_start_str = QString::number(start_epch);
        time_end_str = QString::number(end_epch);

    }

    if(radiobutton_create_graph_hourly->isChecked())
    {
        // Fri 02-Jun-2017 10
        QString start_year = current_barset_time_str + QString(":00:00");
        QString end_year   = current_barset_time_str + QString(":59:59");

        QDateTime date_time;
        date_time = QDateTime::fromString(start_year, "ddd dd-MMM-yyyy hh:mm:ss");
        date_time.setTimeSpec(Qt::UTC);
        qint64 start_epch = date_time.toSecsSinceEpoch();
        if(start_epch < start_timestamp_utc_qint64)
            start_epch = start_timestamp_utc_qint64;


        date_time = QDateTime::fromString(end_year, "ddd dd-MMM-yyyy hh:mm:ss");
        date_time.setTimeSpec(Qt::UTC);
        qint64 end_epch  = date_time.toSecsSinceEpoch();
        if(end_epch > end_timestamp_utc_qint64)
            end_epch = end_timestamp_utc_qint64;

        time_start_str = QString::number(start_epch);
        time_end_str = QString::number(end_epch);
    }


    QString command_time = " AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
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
    query_index.addBindValue(bookmarked_plugin_name);
    for(int tab = 0; tab < bookmarked_plugin_tab_list.size(); tab++)
    {
        query_index.addBindValue(bookmarked_plugin_tab_list.at(tab));
    }

    for(int j = 0; j < bookmarked_timestamp_type_list.size(); j++)
    {
        query_index.addBindValue(bookmarked_timestamp_type_list.at(j));
    }

    query_index.addBindValue(time_start_str);
    query_index.addBindValue(time_end_str);

    populate_data_of_graph_in_tablewidget(query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

int artifacts_timeline_full_loader::populate_data_of_graph_in_tablewidget(QSqlQuery &query_index)
{ // populate all the data of graph  in tablewidget from database
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj == NULL)
        return -1;

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

    //        while(m_tablewidget_saved_by_graph_obj->rowCount())
    //            m_tablewidget_saved_by_graph_obj->removeRow(0);

    total_covered_count = 0;
    total_record_count = 0;


    QString db_cmd = "select COUNT(*) From '" + table_name_list.at(0) + "'";
    db_cmd.append(" where timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");

    QStringList list;
    list << QString::number(start_timestamp_utc_qint64) << QString::number(end_timestamp_utc_qint64);

    total_record_count = recon_helper_standard_obj->get_intvalue_from_db_with_addbindvalues_by_dbpath(db_cmd, destination_db_path,0,list, Q_FUNC_INFO);


    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    int bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int timestamp_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
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
                        icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(dataStr_from_db,(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
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

                if(dataStr_from_db.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    dataStr_from_db = dataStr_from_db.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");
                }
                else if(dataStr_from_db.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                {
                    dataStr_from_db = dataStr_from_db.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");
                }

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

int artifacts_timeline_full_loader::set_graphs_data_tables_header()
{ // set headers of graphs data
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(table_headers_width_list.isEmpty())
        return -1;


    if(display_header_name_list.size() != table_headers_width_list.size())
        return -1;

    custom_header_widgets_QList.clear();

    m_tablewidget_obj->setColumnCount(display_header_name_list.size());

    QStringList empty_header_stringlist;

    for(int column_count = 0; column_count < display_header_name_list.size(); column_count++)
    {
        QString column_name = display_header_name_list.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;

        custom_table_header_widget_obj->pub_set_column_name(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

        m_tablewidget_obj->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        empty_header_stringlist << "";

        connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
        connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
        connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));


        if(column_count ==  enum_tablewidget_column_bookmark && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();

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
        custom_header_widgets_QList.append(custom_table_header_widget_obj);

    }
    m_tablewidget_obj->setHorizontalHeaderLabels(empty_header_stringlist);

    tab_custom_header_widgets_QMap.insert(new_tab_name,custom_header_widgets_QList);

    for(int i = 0; i < display_header_name_list.size(); i++)
    {
        if(((custom_table_header_widget *)custom_header_widgets_QList.at(i))->pub_get_column_name().isEmpty())
        {
            //m_tablewidget_obj->setColumnWidth(i , table_headers_width_list.at(i).toInt());
            int column_width = table_headers_width_list.at(i).toInt();
            column_width = column_width + 2;
            m_tablewidget_obj->setColumnWidth(i, column_width);
        }
        else
        {
            int column_width = table_headers_width_list.at(i).toInt();
            column_width = column_width + 25;
            m_tablewidget_obj->setColumnWidth(i, column_width);
        }

        if(table_headers_width_list.at(i).size() == 0)
            m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    }

    pub_refresh_horizontal_header();



    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return 0;
}

void artifacts_timeline_full_loader::saved_db_info(QString db_name)
{ // save db name and tab name in seperate db to get info later
    QString  db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + "index_artifacts_timeline.sqlite";

    QString tab_name = db_name;
    if(tab_name.endsWith(".sqlite"))
        tab_name.chop(7);

    QSqlDatabase saved_db_info;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    saved_db_info = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    saved_db_info.setDatabaseName(db_path);
    if(!saved_db_info.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_timeline_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + saved_db_info.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_qry(saved_db_info);

    insert_qry.prepare("insert into saved_timeline (db_name,tab_name) values(?,?)");
    insert_qry.addBindValue(db_name);
    insert_qry.addBindValue(tab_name);
    if(!insert_qry.exec())
    {
        recon_static_functions::app_debug("insert_qry.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + insert_qry.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + insert_qry.executedQuery(),Q_FUNC_INFO);
        saved_db_info.close();
        return ;
    }

    saved_db_info.close();
}

void artifacts_timeline_full_loader::update_bookmark_value(QString status,int selected_row_no, int column)
{ // update bookmark value in db to display on click
    if(column != 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

    if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
        return;



    //    if( status == "1" && m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
    //        return;

    //    if( status == "0" && m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
    //        return;

    INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

    struct_global_tagging_artifact_timeline_record s1;
    s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
    s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
    s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
    s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
    s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
    s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

    timeline_record_list << s1;

    if(status == "1")
    {
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_artifact_timeline_db(INT_List);
        global_connection_manager_obj->pub_add_bookmark_for_artifact_timeline(timeline_record_list);

    }
    else
    {
        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_artifact_timeline_db(INT_List);
        global_connection_manager_obj->pub_delete_bookmark_for_artifact_timeline(timeline_record_list);

    }



}

void artifacts_timeline_full_loader::disable_enable_initial_ui(bool status)
{ // disable/enable initial ui or accroding to status (when needs to disable/enable)
    radiobutton_create_graph_yearly->setEnabled(status);
    radiobutton_create_graph_monthly->setEnabled(status);
    radiobutton_create_graph_daywise->setEnabled(status);
    radiobutton_create_graph_hourly->setEnabled(status);

    pushbutton_next->setEnabled(status);
    pushbutton_previous->setEnabled(status);

    pushbutton_go_to_page->setEnabled(status);
    pushbutton_export_graph->setEnabled(status);
    pushbutton_open_in_tableview->setEnabled(status);
    pushbutton_save_graph->setEnabled(status);

    linedit_go_to_page->setEnabled(status);
    label_duration->setHidden(!status);
}

void artifacts_timeline_full_loader::action_go_to_record_triggered()
{ // when we click on go to record button from menu to see that file from file system

    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    qint64 current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
    {
        recon_static_functions::app_debug(" -(1)Ends " , Q_FUNC_INFO);
        return;
    }

    QString record_no_str = get_column_text_from_selected_row(current_row,record_no_index);
    QString tab_name = get_column_text_from_selected_row(current_row,tab_name_index);
    QString plugin_name = get_column_text_from_selected_row(current_row,plugin_name_index);
    QString os_name_str = get_column_text_from_selected_row(current_row,os_name_index);

    QString artifacts_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString()  + QString("artifacts_timeline_full.sqlite");

    emit signal_tablewidget_goto_record(plugin_name,QString("Parent RECON Plugin"),tab_name,record_no_str , artifacts_timeline_db_path, os_name_str);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::create_action_for_goto_artifact_source_submenu()
{ // go to artifact source file from right click on click
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    qint64 current_row = m_tablewidget_obj->currentRow();

    if(current_row < 0)
    {
        recon_static_functions::app_debug(" -(1)Ends " , Q_FUNC_INFO);
        return;
    }

    submenu_go_to_artifact_source->clear();

    QString record_no = get_column_text_from_selected_row(current_row,record_no_index);
    QString plugin_name_str = get_column_text_from_selected_row(current_row,plugin_name_index);
    if(plugin_name_str.isEmpty() || record_no.isEmpty())
        return;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    QString tab_name_str = get_column_text_from_selected_row(current_row,tab_name_index);
    if(tab_name_str.isEmpty())
        return;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QString os_schme_intrnl = recon_helper_standard_obj->get_target_os_scheme_internal_on_current_record_source(record_no, table_name, plugins_destination_db_file, Q_FUNC_INFO);

    QString command;

    if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
        command = "SELECT artifacts_relation_source_file FROM '" + table_name + "' WHERE INT=?" + " GROUP BY artifacts_relation_source_file";
    else
        command = "SELECT source_file FROM '" + table_name + "' WHERE INT=?" + " GROUP BY source_file";

    QStringList arg_list;
    arg_list << record_no;
    QString artifact_source = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);


    if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
    {
        QStringList source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

        for(int i = 0; i < source_list.size(); i++)
        {
            QString source_path = source_list.at(i);

            QFileInfo info_file(source_path);

            QAction *act_go_to_actifact = new QAction(info_file.fileName(), this);
            act_go_to_actifact->setToolTip(source_path);
            submenu_go_to_artifact_source->addAction(act_go_to_actifact);
        }
    }
    else if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
    {
        QStringList source_list = artifact_source.split(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple), Qt::SkipEmptyParts);

        for(int i = 0; i < source_list.size(); i++)
        {
            QString source_path = source_list.at(i);

            QFileInfo info_file(source_path);

            QAction *act_go_to_actifact = new QAction(info_file.fileName(), this);
            act_go_to_actifact->setToolTip(source_path);
            submenu_go_to_artifact_source->addAction(act_go_to_actifact);
        }
    }
    else
    {

        QFileInfo info_file(artifact_source);

        QAction *act_go_to_actifact = new QAction(info_file.fileName(), this);
        act_go_to_actifact->setToolTip(artifact_source);
        submenu_go_to_artifact_source->addAction(act_go_to_actifact);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

