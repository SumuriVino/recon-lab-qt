#include "artifacts_keyword_search_loader.h"


artifacts_keyword_search_loader::artifacts_keyword_search_loader(QWidget *parent) : table_result_loader(parent)
{
    bookmarked_records_from_table_display_list.clear();

}

artifacts_keyword_search_loader::~artifacts_keyword_search_loader()
{

}

void artifacts_keyword_search_loader::configure_keyword_search_loader(QString result_dir_path, QString item_text,qint64 time_offset , QString search_name)
{
    setWindowFlags(Qt::Window);

    lineEdit_search->hide();
    pushButton_search->hide();
    pushbutton_show_all->hide();

    main_tab_widget_obj->setTabText(0, "Items");

    tablewidget_keywords_list = new QTableWidget(this);

    tablewidget_keywords_list->setFixedWidth(200);

    QHBoxLayout *h_link_layout = new QHBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(25,10,QSizePolicy::Expanding,QSizePolicy::Minimum);

    link_label = new QLabel(this);
    link_label->setText("<a href = "+ QString("Show") + " > " + QString("Searched Keywords") + " </a>");
    QObject::connect(link_label, SIGNAL(linkActivated(QString)), this, SLOT(slot_link_label_keywords_list(QString)));


    h_link_layout->addSpacerItem(spacer);
    h_link_layout->addWidget(link_label);


    h_link_layout->addSpacing(2);
    h_link_layout->setContentsMargins(3,3,3,3);


    keyword_search_name = search_name;

    get_creation_timestamp(search_name , result_dir_path);
    //create_keywordsearch_info_on_top_display();


    //main_vbox->addLayout(hbox_search_info_layout);
    upper_widgets_hbox->addLayout(h_link_layout);


    connect(tablewidget_keywords_list,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tablewidget_keywords_list_item_doubleclicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_main_table_widget_item_clicked(QTableWidgetItem*)));

    //connect(tablewidget_keywords_list, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));

    tablewidget_keywords_list->horizontalHeader()->setStretchLastSection(true);

    //h_tree_layout->addWidget(m_tablewidget_obj,1);
    //    h_tree_layout->addWidget(tablewidget_keywords_list,0);

    hbox_table_widget_extra_widgets->addWidget(tablewidget_keywords_list,0);

    //    h_tree_layout->setSpacing(3);
    //    h_tree_layout->setContentsMargins(0,0,0,0);

    display_searched_keywords_table(search_name , result_dir_path);


    //    while(main_vbox->count())
    //        main_vbox->removeItem(main_vbox->itemAt(0));

    //    main_vbox->addLayout(hbox_search_info_layout);
    //    main_vbox->addLayout(h_link_layout);
    //    main_vbox->addLayout(hbox_table_widget_extra_widgets);


    //main_vbox->addLayout(h_tree_layout);

    main_vbox->setSpacing(2);
    main_vbox->setContentsMargins(1,0,1,0);

    tablewidget_keywords_list->hide();


    QString search_db_name = search_name;
    search_db_name = search_db_name.trimmed().replace(" ","_");
    search_db_name.replace("-","_");
    search_db_name.replace(".","_");
    search_db_name.append(".sqlite");

    /// set dest db path for K-Search
    set_display_db_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + search_db_name);

}

void artifacts_keyword_search_loader::set_display_db_path(QString db_path)
{
    keyword_search_detination_db_path = db_path;
}

void artifacts_keyword_search_loader::pub_populate_data_in_table()
{
    //set_csv_for_display(plugin_and_tab_name_list_for_csv);

    bookmark_index = 0;
    notes_icon_index = 1;
    tag_index  = 2;
    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index = 5;
    os_name_index = 7;
    keyword_hit_index = 10;

    m_tablewidget_obj->hideColumn(os_name_index);
    display_data(keyword_search_detination_db_path);

    QStringList menuList;

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {
        // do your work here
    }


}

void artifacts_keyword_search_loader::create_keywordsearch_info_on_top_display()
{
    hbox_search_info_layout = new QHBoxLayout;
    label_keywordsearch_name = new QLabel;
    QString text = QString("<b>Keyword Search Name: </b>" + keyword_search_name);
    label_keywordsearch_name->setText(text);

    QSpacerItem *spacer = new QSpacerItem(150,15,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);

    QLabel *label_creation_timestamp = new QLabel;
    QString tmp_str = "<b>Creation Timestamp(UTC): </b>" + timeline_creation_time;
    label_creation_timestamp->setText(tmp_str);

    hbox_search_info_layout->addWidget(label_keywordsearch_name);
    hbox_search_info_layout->addSpacerItem(spacer);
    hbox_search_info_layout->addWidget(pushbutton_show_all);
    hbox_search_info_layout->addSpacerItem(spacer);
    hbox_search_info_layout->addWidget(label_creation_timestamp);


}

void artifacts_keyword_search_loader::get_creation_timestamp(QString search_name , QString destination_db_file)
{
    recon_static_functions::app_debug(" : STARTS",Q_FUNC_INFO);

    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }


    QSqlQuery query_index(destination_db);
    query_index.prepare("select creation_timestamp from 'keyword_search_index' where search_tag = ?");
    query_index.addBindValue(search_name);
    if(!query_index.exec())
    {
        recon_static_functions::app_debug(" query_index.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_index.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    if(query_index.next())
    {
        //-----set time to UTC
        QDateTime currentDT;
        currentDT.setTimeSpec(Qt::UTC);
        currentDT.setSecsSinceEpoch(query_index.value(0).toInt());

        timeline_creation_time = currentDT.toString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString());
        //-----

    }


    destination_db.close();

    recon_static_functions::app_debug(" : End",Q_FUNC_INFO);

}

int artifacts_keyword_search_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
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

            if(pos == record_no_index)
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

    bookmarked_records_from_table_display_list = recon_helper_standard_obj->pub_get_records_bookmarked_from_table_display_by_db_refrence(received_db_obj, "keyword_search", "INT", Q_FUNC_INFO);

    m_tablewidget_obj->hideColumn(notes_value_index);
    bool_display_table_data_loaded = false;

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void artifacts_keyword_search_loader::display_searched_keywords_table(QString search_name , QString result_dir)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString feature_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();

    QString keyword_search_index_db_path = feature_path + "artifacts_keyword_search.sqlite";

    QString tmp_search_name = search_name;
    tmp_search_name.replace(" ", "_");
    tmp_search_name.replace("-", "_");

    QString keyword_search_db_path = feature_path + tmp_search_name + ".sqlite";

    while (tablewidget_keywords_list->rowCount() > 0)
    {
        tablewidget_keywords_list->removeRow(0);
    }

    tablewidget_keywords_list->setColumnCount(1);
    tablewidget_keywords_list->verticalHeader()->hide();
    QStringList header;
    header << "Keywords";
    tablewidget_keywords_list->setHorizontalHeaderLabels(header);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(keyword_search_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + keyword_search_index_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return ;
    }

    QSqlQuery query_list(destination_db);

    query_list.prepare("select search_keywords from 'keyword_search_index' where search_tag = ?");
    query_list.addBindValue(search_name);

    if(!query_list.exec())
    {
        recon_static_functions::app_debug(" query_list.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_list.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(query_list.executedQuery(),Q_FUNC_INFO);
        return;
    }

    if(query_list.next())
    {
        QStringList list_keywords = query_list.value(0).toString().split("--", Qt::SkipEmptyParts);

        all_keywords_str = list_keywords.join(",");
        searched_keyword_str = all_keywords_str;

        list_keywords.insert(0,"Show All");

        for(int i = 0; i < list_keywords.size(); i++)
        {
            if(i == 0)
            {
                quint64 total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(false, keyword_search_db_path,QStringList("keyword_search"), Q_FUNC_INFO);

                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                item->setText(QString("Show all (" + QString::number(total_record_count) + ")"));

                //item->setBackground(QColor("#434343"));
                // item->setForeground(QColor("#ffffff"));
                tablewidget_keywords_list->insertRow(0);
                tablewidget_keywords_list->setRowHeight(0,21);
                tablewidget_keywords_list->setItem(0,0,item);

            }else
            {

                QString command = "SELECT Count(*)  from keyword_search where keyword_hit =?";
                QStringList value_list;
                value_list << list_keywords.value(i);

                quint64 searched_record_count = recon_helper_standard_obj->get_intvalue_from_db_with_addbindvalues_by_dbpath(command, keyword_search_db_path, 0 , value_list, Q_FUNC_INFO);

                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

                QString keyword_searched = list_keywords.value(i);
                item->setText(QString(keyword_searched.trimmed() + " (" + QString::number(searched_record_count) + ")"));

                tablewidget_keywords_list->insertRow(i);
                tablewidget_keywords_list->setRowHeight(i,21);
                tablewidget_keywords_list->setItem(i,0,item);
            }
        }

    }

    destination_db.close();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::contextMenuEvent(QContextMenuEvent *evt)
{

    if(evt == NULL)
        return;

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
    submenu_open_with->setEnabled(false);
    submenu_send_to_bucket->setEnabled(false);
    act_export->setEnabled(false);
    submenu_add_files_to_hashset_db->setEnabled(false);
    submenu_go_to_artifact_source->setEnabled(false);
    act_go_to_record->setEnabled(false);
    act_full_detail_info->setEnabled(true);
    act_quick_look->setEnabled(false);

    if(selection_model_list.size() > 1)
    {
        act_full_detail_info->setEnabled(false);
        act_export->setEnabled(true);
        act_quick_look->setEnabled(false);
    }
    else if(selection_model_list.size() == 0)
    {
        submenu_go_to_artifact_source->setEnabled(false);
        act_go_to_record->setEnabled(false);
        act_quick_look->setEnabled(false);
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
            act_export->setEnabled(true);
            submenu_send_to_bucket->setEnabled(true);
            submenu_open_with->setEnabled(true);
            submenu_add_files_to_hashset_db->setEnabled(true);
        }

        act_quick_look->setEnabled(true);
        submenu_go_to_artifact_source->setEnabled(true);
        act_go_to_record->setEnabled(true);
    }

    create_action_for_goto_artifact_source_submenu();

    mainMenu->exec(QCursor::pos());
}

void artifacts_keyword_search_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();
    QString os_name_str = m_tablewidget_obj->item(row, os_name_index)->text();


    struct_global_fetch_metadata struct_metadata_values_obj;
    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_of_plugin(plugin_name_str,tab_name_str,record_no,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);


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

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    metadata_str       = struct_metadata_values_obj.metadata;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = struct_metadata_values_obj.metadata;
    st_set_meta_obj.db_file_path = plugins_destination_db_file;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = searched_keyword_str;
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = tab_name_str;

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_open_full_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();

    QString searched_keyword = searched_keyword_str;

    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));
    connect(full_detail_info_obj, SIGNAL(signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString, QString)), this, SLOT(slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString, QString)));

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

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str,record_no ,plugin_db_file);
    QString file_path = st_globl_recon_file_info_obj.complete_file_path;
    int preview_index_full = 0;
    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    preview_index_full = get_stack_index_for_preview(file_path ,display_file_path);

    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"", record_no, plugin_name_str, "");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_open_detach_detailed_info_triggered()
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

void artifacts_keyword_search_loader::create_action_for_goto_artifact_source_submenu()
{
    if(m_tablewidget_obj->currentRow() < 0 || m_tablewidget_obj->currentRow() >= m_tablewidget_obj->rowCount())
        return;

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    submenu_go_to_artifact_source->clear();

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

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
    else
    {
        QFileInfo info_file(artifact_source);

        QAction *act_go_to_actifact = new QAction(info_file.fileName(), this);
        act_go_to_actifact->setToolTip(artifact_source);
        submenu_go_to_artifact_source->addAction(act_go_to_actifact);
    }
}

void artifacts_keyword_search_loader::action_go_to_record_triggered()
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

void artifacts_keyword_search_loader::action_remove_note_triggered()
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    m_tablewidget_obj->setCurrentCell(-1,0);
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
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        if(plugin_name_str.isEmpty())
            continue;


        QIcon icon("");
        m_tablewidget_obj->item(selected_row_no, notes_icon_index)->setIcon(icon);

        global_connection_manager_obj->pub_remove_notes_in_artifacts_keyword_search_db(QStringList(record_no),plugin_name_str, tab_name_str);
        global_connection_manager_obj->pub_remove_notes_in_plugin_viewer_db(QStringList(record_no),plugin_name_str, tab_name_str);
        global_connection_manager_obj->pub_delete_notes_for_plugin_viewer(QStringList(record_no),plugin_name_str, tab_name_str, "");
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

}


void artifacts_keyword_search_loader::action_sendToBucket_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList message_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QStringList all_path_list;
        QStringList  display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;

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

        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
        QStringList tmp_list = obj.complete_export_filepaths;
        display_file_path_list = obj.displayed_file_path;
        recon_file_info_path_list = obj.recon_file_info_path_list;
        recon_filefrom_list = obj.recon_filefrom_list;

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

            struct_global_recon_file_info st_globl_recon_file_info_obj;
            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
            }

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

            QString hex_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;


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
            hex_viewer_open_file_data_obj.record_no_str       = record_no;
            hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
            hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name_str;
            hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;
            hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
            hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

            recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

            emit signal_viewer_extarction_completed_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_sendToBucket_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList message_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QString tab_name_str;
        QStringList all_path_list;
        QStringList  display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;

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

        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
        QStringList tmp_list = obj.complete_export_filepaths;
        display_file_path_list = obj.displayed_file_path;
        recon_file_info_path_list = obj.recon_file_info_path_list;
        recon_filefrom_list = obj.recon_filefrom_list;
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

            struct_global_recon_file_info st_globl_recon_file_info_obj;
            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
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
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString sqlite_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;


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
                sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_path);
                sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
                sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                sqlite_viewer_starter_obj->pub_set_essentials();

                sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

                emit signal_viewer_extarction_completed_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer);
            }
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_sendToBucket_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList message_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QStringList all_path_list;
        QStringList  display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;

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

        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
        all_path_list = obj.complete_export_filepaths;
        display_file_path_list = obj.displayed_file_path;
        recon_file_info_path_list = obj.recon_file_info_path_list;
        recon_filefrom_list     = obj.recon_filefrom_list;


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

            struct_global_recon_file_info st_globl_recon_file_info_obj;
            if(all_path_list.size() == display_file_path_list.size())
            {
                st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
            }

            QString temp_p = complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".plist") && !recon_static_functions::is_it_plist_file(temp_p, Q_FUNC_INFO))
            {
                message_list << QString("'") +  info.fileName() + QString("' is not plist file.");
                continue;
            }
            else
            {
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString plist_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

                QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
                if(list_plist_display_name.contains(plist_display_nm))
                {
                    continue;
                }

                plist_viewer *plist_viewer_obj;
                plist_viewer_obj = new plist_viewer(this);
                QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

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

    display_loading_progress_bar_non_cancelable_obj->hide();

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_open_with_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str ,tab_name_str,record_no,plugin_db_file);
    source_count_name = st_globl_recon_file_info_obj.source_count_name;
    complete_file_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        display_loading_progress_bar_non_cancelable_obj->hide();
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
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString plist_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(plist_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
        }
        else
        {
            plist_viewer *plist_viewer_obj;
            plist_viewer_obj = new plist_viewer(this);
            QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

            plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
            plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
            plist_viewer_obj->pub_set_plist_path(complete_file_path);
            plist_viewer_obj->pub_set_source_count_name(source_count_name);
            plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            plist_viewer_obj->pub_set_essentials();

            plist_viewer_obj->pub_start_plist_extraction();

            emit signal_show_file_in_viewer_table_result(plist_display_nm,MACRO_CASE_TREE_Plist_Viewer, source_count_name);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_open_with_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str ,tab_name_str,record_no,plugin_db_file);

    source_count_name = st_globl_recon_file_info_obj.source_count_name;
    complete_file_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

    QString src_cnt_name_label = QString(" [" + source_count_name + "]");
    QString hex_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;


    QStringList list_hex_display_name = recon_helper_standard_obj->get_hex_namelist_from_db(sqlite_path, Q_FUNC_INFO);
    if(list_hex_display_name.contains(hex_display_nm))
    {
        emit signal_show_file_in_viewer_table_result(hex_display_nm,MACRO_CASE_TREE_Hex_Viewer, source_count_name);
    }
    else
    {

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
        hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
        hex_viewer_open_file_data_obj.record_no_str       = record_no;
        hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
        hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name_str;
        hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;
        hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
        hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

        emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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

    struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str ,tab_name_str,record_no,plugin_db_file);
    source_count_name = st_globl_recon_file_info_obj.source_count_name;
    complete_file_path = st_globl_recon_file_info_obj.complete_file_path;

    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    if(!recon_static_functions::check_is_it_sqlite_file_by_signature(complete_file_path, Q_FUNC_INFO))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' is not sqlite file.");
        message_dialog_obj->show();
    }
    else
    {
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);
        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString sqlite_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;


        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(sqlite_display_nm))
        {
            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
        }
        else
        {
            QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

            sqlite_viewer_starter *sqlite_viewer_starter_obj;
            sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
            sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
            sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
            sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(result_dir_path);
            sqlite_viewer_starter_obj->pub_set_sqlite_path(complete_file_path);
            sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
            sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            sqlite_viewer_starter_obj->pub_set_essentials();

            sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

            emit signal_show_file_in_viewer_table_result(sqlite_display_nm,MACRO_CASE_TREE_Sqlite_Viewer, source_count_name);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            continue;

        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

        struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
        QStringList all_path_list = obj.complete_export_filepaths;
        QStringList display_file_path_list = obj.displayed_file_path;

        for(int i =0 ; i< all_path_list.size() ; i++)
        {
            QString complete_path = all_path_list.at(i);
            QString display_file_path = display_file_path_list.at(i);

            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(display_file_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;

        }

    }

    display_loading_progress_bar_non_cancelable_obj->hide();


    if(list_paths_to_message_box.size() > 0)
    {
        //        for(int i = 0 ; i < list_paths_to_message_box.size(); i++)
        //        {
        //            QProcess proc;

        //            QStringList args;
        //            args << "-R" << "0777" << list_paths_to_message_box.at(i);

        //            proc.execute("chmod", args);

        //            QCoreApplication::processEvents();
        //        }
        message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, dest_path);
        message_dialog_obj->show();
    }else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"selected records does not have any file to export");
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_remove_note_bookmark_triggered()
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
            QString plugin_name_str = m_tablewidget_obj->item(k, plugin_name_index)->text().trimmed();

            if(plugin_name_str.isEmpty())
                continue;

            QString tab_name_str = m_tablewidget_obj->item(k, tab_name_index)->text();
            QString os_name_str = m_tablewidget_obj->item(k , os_name_index)->text();

            update_notes_value(text,k);
            emit signal_update_display_on_remove_notes(plugin_name_str, tab_name_str, record, "", os_name_str);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::update_tags_value(QString tag_data,QString colour)
{
    recon_static_functions::debug_intensive(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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

        QString record_no_str = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        if(plugin_name_str.isEmpty())
            continue;


        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_index)->setIcon(icon);

        global_connection_manager_obj->pub_set_tags_in_artifacts_keyword_search_db(QStringList(record_no_str), plugin_name_str, tab_name_str, tag_data );
        global_connection_manager_obj->pub_set_tag_in_plugin_viewer_db(QStringList(record_no_str), plugin_name_str, tab_name_str, tag_data );
        global_connection_manager_obj->pub_add_tag_for_plugin_viewer(QStringList(record_no_str),plugin_name_str, tab_name_str, tag_data);
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::debug_intensive(" -Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::update_bookmark_value(QString status,int row, int column)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(!table_name_list.isEmpty())
    {
        // Because this class support only CSV of one tab only. confirmed by Mj sir

        if(bookmark_index != -1)
        {
            if(status == QString("1"))
                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
            else
                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        }

        QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text().trimmed();
        QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();
        if(plugin_name_str.isEmpty())
            return;

        if(status == "1")
        {
            bookmarked_records_from_table_display_list << record_no;
            global_connection_manager_obj->pub_set_bookmarks_in_artifacts_keyword_search_db(QStringList(record_no), plugin_name_str, tab_name_str);
            global_connection_manager_obj->pub_set_bookmark_in_plugin_viewer_db(QStringList(record_no), plugin_name_str, tab_name_str);
            global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(QStringList(record_no), plugin_name_str, tab_name_str);
        }
        else if(status == "0")
        {
            qint64 tmp_index = bookmarked_records_from_table_display_list.indexOf(record_no);
            if(tmp_index >= 0)
                bookmarked_records_from_table_display_list.removeAt(tmp_index);

            global_connection_manager_obj->pub_remove_bookmark_in_plugin_viewer_db(QStringList(record_no), plugin_name_str, tab_name_str);
            global_connection_manager_obj->pub_delete_bookmark_for_plugin_viewer(QStringList(record_no), plugin_name_str, tab_name_str ,"");
            global_connection_manager_obj->pub_remove_bookmarks_in_artifacts_keyword_search_db(QStringList(record_no), plugin_name_str, tab_name_str);
        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void artifacts_keyword_search_loader::action_bookmark_triggered()
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
        if(bookmark_index != -1)
        {
            update_bookmark_value("1",selected_row_no,bookmark_index);
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void artifacts_keyword_search_loader::pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str)
{

    if(bookmarked_records_from_table_display_list.contains(record_no_str))
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        if(record_no != record_no_str)
            continue;

        if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() == Qt::Checked)
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
        if(bookmark_index != -1)
        {
            update_bookmark_value("1",selected_row_no,bookmark_index);
        }

        m_tablewidget_obj->selectRow(selected_row_no);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void artifacts_keyword_search_loader::pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str)
{
    if(bookmarked_records_from_table_display_list.contains(record_no_str))
        return;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        if(record_no != record_no_str)
            continue;


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
        if(bookmark_index != -1)
        {
            update_bookmark_value("0",selected_row_no,bookmark_index);
        }

        m_tablewidget_obj->selectRow(selected_row_no);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void artifacts_keyword_search_loader::slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status)
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

