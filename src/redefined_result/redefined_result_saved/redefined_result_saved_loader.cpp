#include "redefined_result_saved_loader.h"

redefined_result_saved_loader::redefined_result_saved_loader(QWidget *parent) : table_result_loader(parent)
{

}

redefined_result_saved_loader::~redefined_result_saved_loader()
{

}
void redefined_result_saved_loader::pub_set_destination_db_path(QString db_path)
{
    destination_redefined_result_saved_db_path = db_path;
    destination_db_path = db_path;
}


void redefined_result_saved_loader::contextMenuEvent(QContextMenuEvent *evt)
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

    //    submenu_open_with->setEnabled(false);
    //    submenu_send_to_bucket->setEnabled(false);
    //    act_export->setEnabled(false);
    act_full_detail_info->setEnabled(true);


    if(selection_model_list.size() > 1)
    {
        act_full_detail_info->setEnabled(false);
        //        act_export->setEnabled(true);
    }
    else if(selection_model_list.size() == 0)
    {
        submenu_go_to_artifact_source->setEnabled(false);
        act_go_to_record->setEnabled(false);
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
            //            act_export->setEnabled(true);
            //            submenu_send_to_bucket->setEnabled(true);
            //            submenu_open_with->setEnabled(true);
        }
    }

    mainMenu->exec(QCursor::pos());
}

void redefined_result_saved_loader::tablewidget_general_clicked(int row,int column)
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


    QString db_path;
    if(plugin_name == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
        db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Browser_History.sqlite";
    else if(plugin_name == QString(MACRO_Plugin_Name_Messenger_Full_RR))
        db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "Messenger.sqlite";

    struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_features_plugins("redefined_result",0,record_no,plugin_name,db_path, "" ,struct_info, Q_FUNC_INFO);


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


void redefined_result_saved_loader::redefined_saved_result_populate_data_in_table()
{
    main_tab_widget_obj->setTabText(0, "Items");

    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index = 5;
    os_name_index = 6;
    source_count_name_index = 7;
    visit_count_index = 11;


    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);

    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);


    display_data(destination_redefined_result_saved_db_path);

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


        ///----Create Submenu of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        // Open With
        QStringList submenuList;
        submenuList << QString(MACRO_CASE_TREE_Plist_Viewer) << QString(MACRO_CASE_TREE_Hex_Viewer) << QString(MACRO_CASE_TREE_Sqlite_Viewer);

        //        submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
        //        create_right_click_actions(submenu_open_with , submenuList);

        // Send to Bucket
        //        submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_open_with);
        //        create_right_click_actions(submenu_send_to_bucket , submenuList);

        // Action Export
        //        act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

        ///----Create Export of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator


        act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard);
        create_right_click_actions(mainMenu , menuList);

        connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
        connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
        connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
        connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
        connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
        connect(pushButton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search_clicked()));

    }
}


void redefined_result_saved_loader::display_data(QString dest_db_path)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    main_tab_widget_obj->setTabText(0, "Items");


    QString db_path_redefined_result;
    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        db_path_redefined_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() +"Browser_History.sqlite";
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        db_path_redefined_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString()+"Messenger.sqlite";



    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")
    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append("b.");
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);


    db_cmd.append(" From main.redefined_result_saved as a join redefinedDB.redefined_result as b on a.INT=b.INT");

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

    QString redefined_result_db_attach_commmand = QString("ATTACH DATABASE '%1' AS redefinedDB")
            .arg(QDir::toNativeSeparators(db_path_redefined_result));
    query_index.exec(redefined_result_db_attach_commmand);

    query_index.prepare(command_for_table_display);


    populate_data_in_tablewidget(*destination_db,query_index);

    QString redefined_result_db_dettach_commmand = QString("DETACH DATABASE redefinedDB");
    query_index.exec(redefined_result_db_dettach_commmand);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

int redefined_result_saved_loader::populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        populate_data_in_tablewidget_browser_history(received_db_obj, query_index);
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        populate_data_in_tablewidget_messenger(received_db_obj, query_index);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

int redefined_result_saved_loader::populate_data_in_tablewidget_browser_history(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
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


int redefined_result_saved_loader::populate_data_in_tablewidget_messenger(QSqlDatabase &received_db_obj, QSqlQuery &query_index)
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

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

void redefined_result_saved_loader::action_sendToBucket_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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

        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();
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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void redefined_result_saved_loader::action_sendToBucket_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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
        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

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

            //            QString hex_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

            //            hex_viewer *hex_viewer_obj;
            //            hex_viewer_obj = new hex_viewer(this);
            //            hex_viewer_obj->pub_set_hex_viewer_name(hex_view_table_name);
            //            hex_viewer_obj->pub_set_outsqlite_path(sqlite_path);
            //            hex_viewer_obj->pub_set_essentials();
            //            hex_viewer_obj->pub_set_hex_path(complete_path);
            //            hex_viewer_obj->pub_set_global_connection_manager(global_connection_manager_obj);
            //            hex_viewer_obj->pub_set_source_count_name(source_count_name);

            //            hex_viewer_obj->pub_set_plugin_name(plugin_name_str);
            //            hex_viewer_obj->pub_set_tab_name(tab_name_str);
            //            hex_viewer_obj->pub_set_record_no(record_no);
            //            hex_viewer_obj->pub_start_hex_extraction();


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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::action_sendToBucket_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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

        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::action_open_full_detailed_info_triggered()
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
    QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

    QStringList all_path_list = recon_helper_standard_obj->get_export_path_list_of_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,result_dir_path,Q_FUNC_INFO);
    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_intrnl = struct_target_info.os_scheme_internal;
    int preview_index_full = 0;

    if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        preview_index_full = get_stack_index_for_preview(preview_file_path , "");
    }
    else
        preview_index_full = get_stack_index_for_preview(current_media_path , "");

    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);

    if(all_path_list.size() > 0)
    {
        QString export_file_path = all_path_list.at(0);

        if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
        {
            full_detail_info_obj->pub_show_full_detail_info(metadata_str, preview_file_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,table_name);
        }
        else
            full_detail_info_obj->pub_show_full_detail_info(metadata_str, current_media_path, preview_index_full,searched_keyword,source_count_name,record_no,plugin_name_str,table_name);
    }
    else
    {

        if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
        {
            full_detail_info_obj->pub_show_full_detail_info(metadata_str, preview_file_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,table_name);
        }
        else
            full_detail_info_obj->pub_show_full_detail_info(metadata_str,  current_media_path, preview_index_full,searched_keyword,source_count_name,record_no,plugin_name_str,table_name);
    }

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::action_remove_note_triggered()
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

void redefined_result_saved_loader::action_remove_note_bookmark_triggered()
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

void redefined_result_saved_loader::action_open_detach_detailed_info_triggered()
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

void redefined_result_saved_loader::action_open_with_plist_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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
        emit signal_statusbar_progress_bar_show_hide(false, "");
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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::action_open_with_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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
        emit signal_statusbar_progress_bar_show_hide(false, "");
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
        //        QString hex_view_table_name = info.fileName() + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

        //        hex_viewer *hex_viewer_obj;
        //        hex_viewer_obj = new hex_viewer(this);
        //        hex_viewer_obj->pub_set_hex_viewer_name(hex_view_table_name);
        //        hex_viewer_obj->pub_set_outsqlite_path(sqlite_path);
        //        hex_viewer_obj->pub_set_essentials();
        //        hex_viewer_obj->pub_set_hex_path(complete_file_path);
        //        hex_viewer_obj->pub_set_global_connection_manager(global_connection_manager_obj);
        //        hex_viewer_obj->pub_set_source_count_name(source_count_name);

        //        hex_viewer_obj->pub_set_plugin_name(plugin_name_str);
        //        hex_viewer_obj->pub_set_tab_name(tab_name_str);
        //        hex_viewer_obj->pub_set_record_no(record_no);

        //        hex_viewer_obj->pub_start_hex_extraction();

        //        emit signal_show_file_in_viewer_table_result(hex_display_nm,MACRO_CASE_TREE_HEX_VIEWER, source_count_name);

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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
    QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

    struct_global_csv_reader_full_csv_info  st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

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
        emit signal_statusbar_progress_bar_show_hide(false, "");
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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::action_export_triggered()
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

        QString destination_plugin_dir_path = result_dir_path  + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
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

    emit signal_statusbar_progress_bar_show_hide(false, "");

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



void redefined_result_saved_loader::update_bookmark_value(QString status,int row, int column)
{
    if(column != 0)
        return;

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    if(bookmark_index == -1)
    {
        return;
    }


    QStringList INT_List;

    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

    if(row < 0 || row >= m_tablewidget_obj->rowCount())
        return;



    INT_List << m_tablewidget_obj->item(row, record_no_index)->text();

    struct_global_tagging_redefined_result_record s1;
    s1.INT_record = m_tablewidget_obj->item(row, record_no_index)->text();
    s1.plugin_name = m_tablewidget_obj->item(row, plugin_name_index)->text();
    s1.tab_name = m_tablewidget_obj->item(row, tab_name_index)->text();
    s1.source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();
    s1.item2 = m_tablewidget_obj->item(row, 8)->text();
    s1.item3 = m_tablewidget_obj->item(row, 9)->text();
    s1.item4 = m_tablewidget_obj->item(row, 10)->text();
    s1.item5 = m_tablewidget_obj->item(row, 11)->text();
    s1.redefined_tab_name = plugin_name;
    s1.redefined_plugin_name = plugin_name;

    redefined_record_list << s1;


    if(status == "1")
    {

        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
        global_connection_manager_obj->pub_set_bookmark_in_redefined_db(INT_List ,plugin_name);
        global_connection_manager_obj->pub_add_bookmark_for_redefined_db(redefined_record_list);

    }
    else
    {

        m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        global_connection_manager_obj->pub_remove_bookmark_in_redefined_db(INT_List,plugin_name);
        global_connection_manager_obj->pub_delete_bookmark_for_redefined_db(INT_List , plugin_name);

    }

}

void redefined_result_saved_loader::update_notes_value(QString text, int current_row)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_redefined_result_saved_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    if(table_name_list.size() > 0)
    {
        int notes_icon_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString,0);
        QString table_name = table_name_list.at(0);     // Because Loader support only one tab in CSV

        QString record_no = m_tablewidget_obj->item(current_row, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            return;

        QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);
        QString tab_column_name = db_columns_list.at(tab_name_index);
        QString os_scheme_column_name = db_columns_list.at(os_name_index);

        QString command = "UPDATE '" + table_name + "' set notes=? WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
        QStringList arg_list;
        arg_list << text << record_no << plugin_name_str << tab_name_str << os_name_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db,Q_FUNC_INFO);

        QTableWidgetItem *item_tablewidget_notes_icon = m_tablewidget_obj->item(current_row,notes_icon_index);

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

        command = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
        arg_list.clear();
        arg_list << record_no << plugin_name_str << tab_name_str << os_name_str;
        QString applied_tags = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

        QStringList applied_tags_list = applied_tags.split(",", Qt::SkipEmptyParts);


        emit signal_change_notes_in_other_db_also(record_no,plugin_name,clicked_item_tab_text,text,applied_tags_list, os_name_str, feature_name);

    }

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void redefined_result_saved_loader::update_tags_value(QString tag_data, QString colour)
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

    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();

        struct_global_tagging_redefined_result_record s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, 11)->text();
        s1.redefined_tab_name = plugin_name;
        s1.redefined_plugin_name = plugin_name;

        redefined_record_list << s1;

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_coulumn_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_set_tag_in_redefined_db(INT_List, plugin_name,tag_data);
    global_connection_manager_obj->pub_add_tag_for_redefined(redefined_record_list, tag_data);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void redefined_result_saved_loader::action_bookmark_triggered()
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
    QList<struct_global_tagging_redefined_result_record> redefined_record_list;

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

        struct_global_tagging_redefined_result_record s1;
        s1.INT_record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 8)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, 11)->text();
        s1.redefined_tab_name = plugin_name;
        s1.redefined_plugin_name = plugin_name;

        redefined_record_list << s1;


        m_tablewidget_obj->item(selected_row_no, bookmark_index)->setCheckState(Qt::Checked);
    }


    global_connection_manager_obj->pub_set_bookmark_in_redefined_db(INT_List ,plugin_name);
    global_connection_manager_obj->pub_add_bookmark_for_redefined_db(redefined_record_list);



    display_loading_progress_bar_non_cancelable_obj->hide();
}

void redefined_result_saved_loader::action_remove_bookmark_triggered()
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

    global_connection_manager_obj->pub_remove_bookmark_in_redefined_db(INT_List , plugin_name);
    global_connection_manager_obj->pub_delete_bookmark_for_redefined_db(INT_List , plugin_name);


    display_loading_progress_bar_non_cancelable_obj->hide();
}
