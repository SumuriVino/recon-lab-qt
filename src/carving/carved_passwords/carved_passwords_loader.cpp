#include "carved_passwords_loader.h"

carved_passwords_loader::carved_passwords_loader(QWidget *parent) : table_result_loader(parent)
{
    setWindowFlags(Qt::Window);
    export_dialog_box_1_obj = new export_dialog_box_1(this);
    connect(export_dialog_box_1_obj, SIGNAL(signal_save_results_by_export_window(QString , QString, QString)), this, SLOT(slot_table_data_export_dialog_ok_clicked(QString , QString, QString)));

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
}

carved_passwords_loader::~carved_passwords_loader()
{

}

void carved_passwords_loader::pub_set_carved_passwords_display_db_path(QString db_path)
{
    destination_db_path = db_path;
}


void carved_passwords_loader::populate_data_in_table()
{

    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index = 5;
    password_index = 6 ;
    server_index = 7;
    account_index = 8 ;
    description_index = 9;

    os_name_index = 10;
    source_count_name_index = 11;


    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(plugin_name_index);
    m_tablewidget_obj->hideColumn(tab_name_index);

    pub_refresh_horizontal_header();

    display_data(destination_db_path);

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark, menuList);

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {

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

        act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);

        create_right_click_actions(mainMenu , menuList);
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        create_right_click_actions(mainMenu , QStringList(QString("Create Word List")));

        connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
        connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
        //connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
        connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
        connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
        connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));

    }
}

void carved_passwords_loader::pub_create_ui()
{
    main_tab_widget_obj = new QTabWidget(this);

    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);

    main_tab_widget_obj->addTab(m_tablewidget_obj, MACRO_Plugin_Carved_Passwords_TAB_Name_Passwords);

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

    upper_widgets_hbox->addWidget(lineEdit_search);
    upper_widgets_hbox->addWidget(pushButton_search);
    upper_widgets_hbox->addWidget(pushbutton_show_all);

    upper_widgets_hbox->addStretch();
    upper_widgets_hbox->setSpacing(10);

    main_vbox->addItem(upper_widgets_hbox);
    hbox_table_widget_extra_widgets->addWidget(main_tab_widget_obj);
    main_vbox->addLayout(hbox_table_widget_extra_widgets);
    main_vbox->setStretch(0,0);
    main_vbox->setStretch(1,1);

    main_vbox->setContentsMargins(4,4,4,4);
    main_vbox->setSpacing(0);

    this->setLayout(main_vbox);

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

int carved_passwords_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
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


    int record_no_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int bookmark_position  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    int tag_position     = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
    int count = m_tablewidget_obj->rowCount();

    notes_value_index  = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString,0);
    blank_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString,0);



    while(query_index.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
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

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void carved_passwords_loader::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    if(bool_display_table_data_loaded)
        return;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    int current_row    = m_tablewidget_obj->currentRow();
    int current_column = m_tablewidget_obj->currentColumn();


    if(!m_tablewidget_obj->selectionModel()->selectedRows().isEmpty())
    {
        QList <QAction *> act_list = mainMenu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }
    }

    tablewidget_general_clicked(current_row,current_column);


    mainMenu->exec(QCursor::pos());
}

void carved_passwords_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;


    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    if(record_no_str.trimmed().isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text().trimmed();

    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text().trimmed();



    struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based("carved_password",0,record_no_str,plugin_name,destination_db_path,source_count_name,Q_FUNC_INFO);


    metadata_str = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.source_count_name  = source_count_name;
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = lineEdit_search->text();
    st_set_meta_obj.plugin_name = MACRO_Plugin_Name_Carved_Password;
    st_set_meta_obj.tab_name = tab_name_str;


    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_passwords_loader::action_bookmark_triggered()
{
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
    QList<struct_global_tagging_carved_passwords> carved_passwd_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_carved_passwords s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, 6)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, 7)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item4 = clicked_item_tab_text;

        carved_passwd_record_list << s1;


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    }


    global_connection_manager_obj->pub_set_bookmark_in_carved_password_db(INT_List ,clicked_item_tab_text);
    global_connection_manager_obj->pub_add_bookmark_for_carved_password_db(carved_passwd_record_list);



    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_passwords_loader::action_remove_bookmark_triggered()
{
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
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Unchecked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Unchecked);
    }

    global_connection_manager_obj->pub_remove_bookmark_in_carved_password_db(INT_List,clicked_item_tab_text);
    global_connection_manager_obj->pub_delete_bookmark_for_carved_password_db(INT_List,clicked_item_tab_text);


    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_passwords_loader::update_bookmark_value(QString status,int row, int column)
{
    if(column != 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
    {
        return;
    }


    QStringList INT_List;

    QList<struct_global_tagging_carved_passwords> carved_passwords_record_list;

    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;



    INT_List << m_tablewidget_obj->item(row, record_no_index)->text();

    struct_global_tagging_carved_passwords s1;
    s1.INT_record = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(row, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(row, tab_name_index)->text();
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();
    s1.item0 = m_tablewidget_obj->item(row, 6)->text();
    s1.item1 = m_tablewidget_obj->item(row, 7)->text();
    s1.item2 = m_tablewidget_obj->item(row, 8)->text();
    s1.item3 = m_tablewidget_obj->item(row, 9)->text();
    s1.item4 = clicked_item_tab_text;

    carved_passwords_record_list << s1;


    if(status == "1")
    {

        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_carved_password_db(INT_List ,clicked_item_tab_text);
        global_connection_manager_obj->pub_add_bookmark_for_carved_password_db(carved_passwords_record_list);

    }
    else
    {
        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_carved_password_db(INT_List,clicked_item_tab_text);
        global_connection_manager_obj->pub_delete_bookmark_for_carved_password_db(INT_List , clicked_item_tab_text);
    }
}

void carved_passwords_loader::action_remove_note_triggered()
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

    global_connection_manager_obj->pub_remove_notes_in_carved_password_db(INT_List,clicked_item_tab_text);
    global_connection_manager_obj->pub_delete_notes_for_carved_password_db(INT_List,clicked_item_tab_text);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_passwords_loader::action_remove_note_bookmark_triggered()
{
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

            emit signal_update_display_on_remove_notes(plugin_name,clicked_item_tab_text, record, "", os_name_str);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_passwords_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString searched_keyword = searched_keyword_str;
    QString record_no_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", 0,searched_keyword,"",record_no_str, "","");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void carved_passwords_loader::action_create_word_list_triggered()
{
    QString file_name = QString("Carved Password");
    QString dir_path = getenv("HOME") + QString("/Desktop") + "/LAB_Export";
    export_dialog_box_1_obj->pub_set_name_and_dir_path(file_name,dir_path);

    export_dialog_box_1_obj->show();

}

void carved_passwords_loader::update_tags_value(QString tag_data, QString colour)
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
    QList<struct_global_tagging_carved_passwords> carved_passwd_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_carved_passwords s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item0 = m_tablewidget_obj->item(selected_row_no, 6)->text();
        s1.item1 = m_tablewidget_obj->item(selected_row_no, 7)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item4 = clicked_item_tab_text;

        carved_passwd_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_coulumn_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_set_tag_in_carved_password_db(INT_List, clicked_item_tab_text,tag_data);
    global_connection_manager_obj->pub_add_tag_for_carved_password(carved_passwd_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void carved_passwords_loader::open_window_for_passwords_file(QString file_path)
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
