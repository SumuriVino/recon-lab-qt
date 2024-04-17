#include "snapshots_loader.h"

snapshots_loader::snapshots_loader(QWidget *parent)
{

    timeline_selection_window_obj = new timeline_selection_window(this);
    connect(timeline_selection_window_obj, SIGNAL(signal_time_line(qint64,qint64)), this, SLOT(slot_time_line(qint64,qint64)));

    bool_timeline_is_selected = false;


    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    file_export_dialog_obj = new file_export_dialog(this);
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done(bool,QString)), this, SLOT(slot_export_file_dialog_work_done(bool,QString)));
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done_for_zip(QString,bool,QString)), this, SLOT(slot_export_file_dialog_work_done_for_zip(QString,bool,QString)));


    thread_zip_file_export_obj = new thread_zip_file_export;
    connect(thread_zip_file_export_obj, SIGNAL(finished()), this, SLOT(slot_thread_zip_file_export_finished()));
    connect(file_export_dialog_obj, SIGNAL(signal_export_file_dialog_work_done_for_dmg(bool,QString)), this, SLOT(slot_export_file_dialog_work_done_for_dmg(bool,QString)));

    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    recon_helper_process_obj = new recon_helper_process(this);

}

snapshots_loader::~snapshots_loader()
{

}

void snapshots_loader::pub_set_snapshots_difference_db_path(QString dest_db)
{
    destination_db_path = dest_db;
}


void snapshots_loader::populate_data_in_table()
{

    notes_value_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    record_no_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    bookmark_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);
    timeline_col_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);


    file_name_index = 4;
    file_path_index = 5;
    file_size_index = 6;
    file_extension = 7;
    modification_time_index = 8;
    change_time_index = 9;
    last_access_time_index = 10;
    created_time_index = 11;
    source_count_name_index = 12;


    display_data(destination_db_path);


    create_snapshots_right_click();

}


void snapshots_loader::display_data(QString dest_db_path)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, destination_db_path, QStringList(), Q_FUNC_INFO);

    QString command;
    QString current_tab_name;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + dest_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        return;
    }

    QSqlQuery query_index(destination_db);

    for(int ii = 0  ; ii < display_tab_list.size() ;ii++)
    {

        current_tab_name = display_tab_list.at(ii);
        display_header_name_list = global_csv_reader_class_selected_obj->get_header_list_of_tab(plugin_name ,current_tab_name);

        command = get_command_according_to_table_name(table_name_list.at(ii));

        query_index.prepare(command);

        populate_data_in_tablewidget_for_multitabs(list_tablewidgets.at(ii) , destination_db, query_index);
    }

    destination_db.close();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

QString snapshots_loader::get_command_according_to_table_name(QString table_name)
{

    QString command;
    if(!db_columns_list.isEmpty())
        command = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        command.append(db_columns_list.at(i));
        command.append(QString(","));
    }

    if(command.endsWith(QString(",")))
        command.chop(1);

    command.append(QString(" From '"));
    command.append(table_name + "'");


    return command;

}

void snapshots_loader::search_keyword_from_db()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString keyword = lineEdit_search->text().trimmed();

    searched_keyword_str = keyword;

    total_covered_count = 0;
    total_record_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, destination_db_path, QStringList(), Q_FUNC_INFO);


    QStringList search_string_list = recon_static_functions::get_search_string_list(keyword, Q_FUNC_INFO);
    if(search_string_list.size() <= 0)
        return;


    QStringList cols_nm_list =  db_columns_list;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        return;
    }

    QSqlQuery query_index(destination_db);


    for(int ii = 0 ; ii < table_name_list.size() ; ii++)
    {
        QString table_name = table_name_list.at(ii);
        QString command = cols_nm_list.join(",");
        command = "select " + command + " from '" + table_name + "'";
        QString partial_command = recon_static_functions::get_partial_command_forsearch(cols_nm_list,search_string_list, Q_FUNC_INFO);
        command = command + partial_command;

        QString timestamp_column_name;
        if((modification_time_index > 0) && (cols_nm_list.size() > modification_time_index))
            timestamp_column_name = cols_nm_list.at(modification_time_index);

        QString timestamp_partial_cmd = timestamp_column_name +"  >= ? AND "+ timestamp_column_name +" <= ? ORDER BY "+ timestamp_column_name +" ASC";
        if(bool_timeline_is_selected && keyword.trimmed().isEmpty())
        {
            command.clear();
            QString timeline_cmd = cols_nm_list.join(",");
            command  = timeline_cmd = "select " + timeline_cmd + " from '" + table_name + "' where " + timestamp_partial_cmd;
            query_index.prepare(command);
            query_index.addBindValue(timeline_start_time);
            query_index.addBindValue(timeline_end_time);

        }
        else if(bool_timeline_is_selected && !keyword.trimmed().isEmpty())
        {
            command += " AND "+ timestamp_partial_cmd;

            query_index.prepare(command);
            for(int k = 0 ; k < cols_nm_list.size() ; k++)
            {
                if(cols_nm_list.at(k) == "os_scheme_display")
                    continue ;

                for(int i = 0; i < search_string_list.size(); i++)
                {
                    query_index.addBindValue(search_string_list.at(i));
                }
            }
            query_index.addBindValue(timeline_start_time);
            query_index.addBindValue(timeline_end_time);

        }
        else
        {
            query_index.prepare(command);
            for(int k = 0 ; k < cols_nm_list.size() ; k++)
            {
                if(cols_nm_list.at(k) == "os_scheme_display")
                    continue ;

                for(int i = 0; i < search_string_list.size(); i++)
                {
                    query_index.addBindValue(search_string_list.at(i));
                }
            }
        }


        populate_data_in_tablewidget_for_multitabs(list_tablewidgets.at(ii),destination_db,query_index);
    }

    destination_db.close();

    emit signal_clear_all_metadata();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


void snapshots_loader::populate_data_in_tablewidget_for_multitabs(QTableWidget *current_table_widget,QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    current_table_widget->horizontalHeader()->setStretchLastSection(true);

    if(display_header_name_list.isEmpty())
    {
        recon_static_functions::app_debug("display_header_name_list.isEmpty() ----FAILED---- " , Q_FUNC_INFO);
        return ;
    }

    bool_display_table_data_loaded = true;

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_index.executedQuery(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        return ;
    }

    current_table_widget->setRowCount(0);

    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;


    current_table_widget->setColumnWidth(bookmark_position,40);
    current_table_widget->setColumnWidth(notes_icon_index,30);
    current_table_widget->setColumnWidth(file_name_index,200);
    current_table_widget->setColumnWidth(tag_position,30);
    //current_table_widget->setColumnWidth(timeline_col_index,200);
    current_table_widget->setColumnWidth(modification_time_index,200);
    current_table_widget->setColumnWidth(change_time_index,200);
    current_table_widget->setColumnWidth(last_access_time_index,200);
    current_table_widget->setColumnWidth(created_time_index,200);
    current_table_widget->setColumnWidth(file_path_index,300);


    qint64 count = current_table_widget->rowCount();

    while(query_index.next())
    {
        if(count % 100 == 0)
            QCoreApplication::processEvents();

        total_covered_count++;

        loading_display_dialog_based_on_records(total_record_count , total_covered_count ,feature_name);
        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break ;
        }

        current_table_widget->insertRow(count);
        current_table_widget->setRowHeight(count, 22);

        int pos = 0;
        while(pos != display_header_name_list.size())
        {
            QString dataStr_from_db = query_index.value(pos).toString().trimmed();

            if(pos == record_no_index)
            {
                QTableWidgetItem *item_tablewidget_record_no;

                item_tablewidget_record_no = new QTableWidgetItem;
                item_tablewidget_record_no->setData(Qt::EditRole, dataStr_from_db.toLongLong());

                item_tablewidget_record_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                current_table_widget->setItem(count, pos, item_tablewidget_record_no);

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

                current_table_widget->setItem(count, pos, item_tablewidget_bookmark);

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
                current_table_widget->setItem(count, pos, item_tablewidget_notes_icon);

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

                current_table_widget->setItem(count, pos, item_tablewidget_tags);
            }
            else if(pos == blank_column_index)
            {
                QTableWidgetItem *item_tablewidget_blank = new QTableWidgetItem;
                item_tablewidget_blank->setText("");
                current_table_widget->setItem(count, pos, item_tablewidget_blank);
            }
            else if(pos == notes_value_index)
            {
                QTableWidgetItem *item_tablewidget_notes_text = new QTableWidgetItem;
                item_tablewidget_notes_text->setText(query_index.value(notes_value_index).toString().trimmed());
                current_table_widget->setItem(count, pos, item_tablewidget_notes_text);
            }
            else if(pos == modification_time_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                item_tablewidget_int->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db,Q_FUNC_INFO));
                current_table_widget->setItem(count, pos, item_tablewidget_int);
            }
            else if(pos == change_time_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                item_tablewidget_int->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db,Q_FUNC_INFO));
                current_table_widget->setItem(count, pos, item_tablewidget_int);
            }
            else if(pos == last_access_time_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                item_tablewidget_int->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db,Q_FUNC_INFO));
                current_table_widget->setItem(count, pos, item_tablewidget_int);
            }
            else if(pos == created_time_index)
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                item_tablewidget_int->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(dataStr_from_db,Q_FUNC_INFO));
                current_table_widget->setItem(count, pos, item_tablewidget_int);
            }
            else if(pos == file_size_index)
            {
                QTableWidgetItem *item_tablewidget_fl_size;
                item_tablewidget_fl_size = new QTableWidgetItem;
                item_tablewidget_fl_size->setData(Qt::EditRole, dataStr_from_db.toLongLong());
                item_tablewidget_fl_size->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                current_table_widget->setItem(count, pos, item_tablewidget_fl_size);
            }
            else
            {
                QTableWidgetItem *item_tablewidget_int = new QTableWidgetItem;
                item_tablewidget_int->setText(dataStr_from_db);
                current_table_widget->setItem(count, pos, item_tablewidget_int);
            }

            pos ++;
        }
        count ++;
    }

    current_table_widget->hideColumn(notes_value_index);
    current_table_widget->hideColumn(source_count_name_index);

    bool_display_table_data_loaded = false;

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return ;
}

void snapshots_loader::pub_create_ui()
{
    recon_static_functions::app_debug("-start- " , Q_FUNC_INFO);

    tab_custom_header_snapshot_widgets_QMap.clear();
    fill_display_essentials(plugin_and_tab_name_list_for_csv);

    main_vbox = new QVBoxLayout(this);
    main_tab_widget_obj = new QTabWidget(this);

    display_tab_list = global_csv_reader_class_selected_obj->get_total_tab_list(plugin_name);


    QString current_tab_name;
    QStringList columnheaderlist;
    for(int ii = 0  ; ii < display_tab_list.size() ;ii++)
    {
        current_tab_name = display_tab_list.at(ii);
        columnheaderlist = global_csv_reader_class_selected_obj->get_header_list_of_tab(plugin_name ,current_tab_name);
        m_tablewidget_multitabs = new m_tablewidget(this);

        connect(m_tablewidget_multitabs, SIGNAL(itemSelectionChanged()), this, SLOT(slot_tablewidget_selection_changed()));
        connect(m_tablewidget_multitabs, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
        connect(m_tablewidget_multitabs, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
        connect(m_tablewidget_multitabs,SIGNAL(signal_spacebar_quicklook_pressed()),this,SIGNAL(signal_act_quick_look_triggered()));
        connect(m_tablewidget_multitabs, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
        connect(m_tablewidget_multitabs,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));

        m_tablewidget_multitabs->horizontalHeader()->setStretchLastSection(true);
        m_tablewidget_multitabs->setSelectionMode(QAbstractItemView::ExtendedSelection);

        m_tablewidget_multitabs->setColumnCount(columnheaderlist.size());
        QList<custom_table_header_widget *> custom_header_widgets_snapshot_QList;

        QStringList empty_header_stringlist;

        for(int column_count = 0; column_count < columnheaderlist.size(); column_count++)
        {
            QString column_name = columnheaderlist.at(column_count);

            custom_table_header_widget_obj = new custom_table_header_widget;

            custom_table_header_widget_obj->pub_set_column_name(column_name);
            custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

            m_tablewidget_multitabs->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

            if(column_count ==  enum_tablewidget_column_bookmark && column_name.isEmpty())
            {
                QPixmap bookmark("../icons/signs/bookmark.png");
                custom_table_header_widget_obj->pub_set_icon_on_bookmark_or_seen(bookmark);
                custom_table_header_widget_obj->pushbutton_sort->show();
            }
            else if(column_count ==  enum_tablewidget_column_notes && column_name.isEmpty())
            {
                custom_table_header_widget_obj->pushbutton_sort->show();
            }
            else if(column_count ==  enum_tablewidget_column_tags && column_name.isEmpty())
            {
                custom_table_header_widget_obj->pushbutton_sort->show();
            }

            empty_header_stringlist << "";

            connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
            connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
            connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));

            custom_header_widgets_snapshot_QList.append(custom_table_header_widget_obj);
        }
        m_tablewidget_multitabs->setHorizontalHeaderLabels(empty_header_stringlist);
        tab_custom_header_snapshot_widgets_QMap.insert(ii,custom_header_widgets_snapshot_QList);

        m_tablewidget_multitabs->setSortingEnabled(false);

        main_tab_widget_obj->addTab(m_tablewidget_multitabs, current_tab_name);
        connect(main_tab_widget_obj, SIGNAL(tabBarClicked(int)), this, SLOT(slot_on_tab_changed_clicked(int)));

        list_tablewidgets << m_tablewidget_multitabs;
    }


    hbox_table_widget_extra_widgets = new QHBoxLayout(this);
    hbox_table_widget_extra_widgets->setContentsMargins(0,0,0,0);
    hbox_table_widget_extra_widgets->setSpacing(0);


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



    pushbutton_timeline = new QPushButton("Timeline", this);
    QFont m_font_pushbutton_timeline = pushbutton_timeline->font();
    m_font_pushbutton_timeline.setPointSize(12);
    pushbutton_timeline->setFixedWidth(100);

    pushButton_search->hide();
    lineEdit_search->hide();
    pushbutton_show_all->hide();
    pushbutton_timeline->setEnabled(false);
    checkbox_timeline = new QCheckBox(this);
    checkbox_timeline->setFixedWidth(30);
    connect(checkbox_timeline, SIGNAL(toggled(bool)), this, SLOT(slot_checkbox_timeline_toggled(bool)));

    upper_widgets_hbox = new QHBoxLayout(this);
    upper_widgets_hbox->addWidget(lineEdit_search);
    upper_widgets_hbox->addWidget(checkbox_timeline);
    upper_widgets_hbox->addWidget(pushbutton_timeline);
    upper_widgets_hbox->addWidget(pushButton_search);
    upper_widgets_hbox->addWidget(pushbutton_show_all);

    upper_widgets_hbox->addStretch();
    upper_widgets_hbox->setSpacing(10);

    main_vbox->addItem(upper_widgets_hbox);
    hbox_table_widget_extra_widgets->addWidget(main_tab_widget_obj);

    main_vbox->addItem(hbox_table_widget_extra_widgets);

    main_vbox->setStretch(0,0);
    main_vbox->setStretch(1,1);
    main_vbox->setContentsMargins(4,4,4,4);
    main_vbox->setSpacing(0);

    this->setLayout(main_vbox);


    connect(pushButton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(lineEdit_search,SIGNAL(returnPressed()),this,SLOT(slot_pushbutton_search_clicked()));
    connect(pushbutton_show_all,SIGNAL(clicked()),this,SLOT(slot_pushbutton_showall_clicked()));
    connect(pushbutton_timeline, SIGNAL(clicked()), this, SLOT(slot_pushButton_timeline_clicked()));


    recon_static_functions::app_debug("-end- " , Q_FUNC_INFO);

}

void snapshots_loader::pub_set_selected_snapshot_name()
{
    QString index_dest_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + "saved_snapshots_index.sqlite";
    QString cmd = "Select primary_source ,secondary_source from tbl_saved_index where snapshot_lable_name = ?";

    primary_snapshot   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,QStringList(clicked_item_tab_text),0,index_dest_db,Q_FUNC_INFO);;
    secondary_snapshot = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,QStringList(clicked_item_tab_text),1,index_dest_db,Q_FUNC_INFO);;
}

void snapshots_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    if(bool_display_table_data_loaded)
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int row = m_tablewidget_obj->currentRow();

    foreach (QAction *action, mainMenu->actions())
    {
        if(action->text() == QString(MACRO_Generic_Right_Click_Go_To_Source))
        {
            if(display_tab_name == MACRO_Plugin_Snapshots_TAB_Name_Modified_Files)
                action->setEnabled(true);
            else if(display_tab_name == MACRO_Plugin_Snapshots_TAB_Name_Deleted_Files)
                action->setEnabled(false);

            break;
        }
    }


    struct_global_right_clicks_status_depends_on_file_existance obj;
    obj.feature_name = plugin_name;
    obj.plugin_name = plugin_name;
    obj.record_no = m_tablewidget_obj->item(row,record_no_index)->text();
    obj.source_count_name = m_tablewidget_obj->item(row,source_count_name_index)->text();
    obj.arguments_values_list = QStringList(clicked_item_tab_text);
    obj.tab_name = display_tab_name;
    recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj,mainMenu,Q_FUNC_INFO);


    mainMenu->exec(QCursor::pos());
}

void snapshots_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" -Start- " , Q_FUNC_INFO);


    if(m_tablewidget_obj == NULL)
        return ;


    if(row < 0 || column < 0)
        return;



    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow() ,record_no_index)->text();
    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow() ,source_count_name_index)->text();


    struct_global_fetch_metadata metadata_obj;

    metadata_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name, current_tab_index,record_no,plugin_name,destination_db_path,source_count_name,Q_FUNC_INFO);

    metadata_str = metadata_obj.metadata;


    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = metadata_str;
    st_set_meta_obj.db_file_path = destination_db_path;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = plugin_name;
    st_set_meta_obj.tab_name = display_tab_name;

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug("-end- " , Q_FUNC_INFO);
}


void snapshots_loader::create_snapshots_right_click()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);

    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
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

    ///----Create Submenu of existing Menu
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator


    // Action Quicklook
    act_quick_look = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Quick_look),mainMenu);

    act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);


    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard) ;
    create_right_click_actions(mainMenu , menuList);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Action Export
    act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    create_right_click_actions(mainMenu , QStringList(QString(MACRO_Generic_Right_Click_Go_To_Source)));

    connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void snapshots_loader::update_tags_value(QString tag_data,QString colour)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;
    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;


        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_tag_notes_db_updation s1;
        s1.record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = plugin_name;
        s1.tab_name = display_tab_name;
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, file_size_index)->text();;
        s1.item3 = clicked_item_tab_text;
        s1.csv_plugin_name = plugin_name;
        s1.csv_tab_name = display_tab_name;

        tagging_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);

    }

    global_connection_manager_obj->pub_set_tag_in_snapshots_difference_db(INT_List,clicked_item_tab_text,table_name,tag_data);
    global_connection_manager_obj->pub_add_tag_for_snapshots_difference(tagging_record_list ,tag_data);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void snapshots_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString searched_keyword = searched_keyword_str;

    QString source_count_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_snapshots_by_db_path(record_no_str,display_tab_name,destination_db_path,source_count_name_str);
    QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;

    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);


    int preview_index_full = 1;
    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    preview_index_full = get_stack_index_for_preview(complete_file_path ,display_file_path);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"",record_no_str, plugin_name,"");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    file_export_dialog_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    //    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    //    global_recon_555_helper_singular_class_obj->pub_cancel_rsync_copy(false,Q_FUNC_INFO);

    //    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    //    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    //    display_loading_progress_bar_non_cancelable_obj->show();

    //    QString dest_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();

    //    QStringList list_paths_to_message_box;


    //    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    //    for(int i = 0; i < selection_model_list.size(); i++)
    //    {
    //        if(i % 100 == 0)
    //            QCoreApplication::processEvents();

    //        QModelIndex model_index = selection_model_list.at(i);

    //        int selected_row_no = model_index.row();

    //        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
    //            continue;

    //        if(m_tablewidget_obj->isRowHidden(selected_row_no))
    //            continue;


    //        QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow() ,record_no_index)->text();
    //        QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow() ,source_count_name_index)->text();
    //        QString complete_filepath = global_recon_555_helper_singular_class_obj->get_file_infopath_by_dbpath(record_no,destination_db_path,table_name,source_count_name,Q_FUNC_INFO);

    //        QStringList all_path_list;
    //        all_path_list << complete_filepath;


    //        for(int i =0 ; i< all_path_list.size() ; i++)
    //        {
    //            QString complete_path = all_path_list.at(i);

    //            QString exported_file_path = global_recon_555_helper_singular_class_obj->pub_copy_file_using_rsync(complete_path,dest_path ,QFileInfo(complete_path).fileName(),Q_FUNC_INFO);
    //            if(!exported_file_path.trimmed().isEmpty())
    //                list_paths_to_message_box << exported_file_path;

    //        }
    //    }

    //    if(list_paths_to_message_box.size() > 0)
    //    {
    //        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box);
    //        message_dialog_obj->show();
    //    }
    //    else
    //    {
    //        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"selected records does not have any file to export");
    //        message_dialog_obj->show();
    //    }

    //    display_loading_progress_bar_non_cancelable_obj->hide();

    //    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::action_bookmark_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList INT_List;
    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_position)->checkState() == Qt::Checked)
            continue;


        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_tag_notes_db_updation s1;
        s1.record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = plugin_name;
        s1.tab_name = display_tab_name;
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, file_name_index)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, file_size_index)->text();;
        s1.item3 = clicked_item_tab_text;
        s1.csv_plugin_name = plugin_name;
        s1.csv_tab_name = display_tab_name;

        tagging_record_list << s1;
        m_tablewidget_obj->item(selected_row_no, bookmark_position)->setCheckState(Qt::Checked);
    }

    global_connection_manager_obj->pub_set_bookmark_in_snapshots_difference_db(INT_List,clicked_item_tab_text,table_name);
    global_connection_manager_obj->pub_add_bookmark_for_snapshots_difference_system(tagging_record_list);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::action_remove_bookmark_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

    QStringList INT_List;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_position)->checkState() == Qt::Unchecked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_tag_notes_db_updation s1;
        s1.record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = plugin_name;
        s1.tab_name = display_tab_name;
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item3 = clicked_item_tab_text;

        tagging_record_list << s1;

        m_tablewidget_obj->item(selected_row_no, bookmark_position)->setCheckState(Qt::Unchecked);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_snapshots_difference_db(INT_List ,clicked_item_tab_text,table_name);
    global_connection_manager_obj->pub_delete_bookmark_for_snapshots_difference(tagging_record_list);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::update_bookmark_value(QString status,int row, int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(column != 0)
        return;

    QStringList INT_List;

    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;


    INT_List << m_tablewidget_obj->item(row, record_no_index)->text();

    struct_global_tagging_tag_notes_db_updation s1;
    s1.record_no = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = plugin_name;
    s1.tab_name = display_tab_name;
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();
    s1.item0 = m_tablewidget_obj->item(row, file_name_index)->text();
    s1.item1 = m_tablewidget_obj->item(row, file_path_index)->text();
    s1.item2 = m_tablewidget_obj->item(row, file_size_index)->text();;
    s1.item3 = clicked_item_tab_text;
    s1.csv_plugin_name = plugin_name;
    s1.csv_tab_name = display_tab_name;

    tagging_record_list << s1;

    if(status == "1")
    {
        m_tablewidget_obj->item(row, bookmark_position)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_snapshots_difference_db(INT_List,clicked_item_tab_text,table_name);
        global_connection_manager_obj->pub_add_bookmark_for_snapshots_difference_system(tagging_record_list);

    }
    else
    {
        m_tablewidget_obj->item(row, bookmark_position)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_snapshots_difference_db(INT_List ,clicked_item_tab_text,table_name);
        global_connection_manager_obj->pub_delete_bookmark_for_snapshots_difference(tagging_record_list);
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void snapshots_loader::action_add_note_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    m_tablewidget_obj->blockSignals(true);

    add_notes_to_singal_record_obj->set_notes("");
    add_notes_to_singal_record_obj->show();

    m_tablewidget_obj->blockSignals(false);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void snapshots_loader::action_remove_note_triggered()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

    QStringList INT_List;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 || selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


        struct_global_tagging_tag_notes_db_updation s1;
        s1.record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = plugin_name;
        s1.tab_name = display_tab_name;
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item3 = clicked_item_tab_text;

        tagging_record_list << s1;

        QIcon iconNotes("");
        m_tablewidget_obj->item(selected_row_no, notes_icon_index)->setIcon(iconNotes);

    }


    global_connection_manager_obj->pub_remove_notes_in_snapshots_difference_db(INT_List,clicked_item_tab_text,table_name);
    global_connection_manager_obj->pub_delete_notes_for_snapshots_difference(tagging_record_list);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}






