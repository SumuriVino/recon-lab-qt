#include "ram_analysis_saved_loader.h"

ram_analysis_saved_loader::ram_analysis_saved_loader(QWidget *parent) : table_result_loader(parent)
{
    setWindowFlags(Qt::Window);
}

ram_analysis_saved_loader::~ram_analysis_saved_loader()
{

}


void ram_analysis_saved_loader::pub_set_ram_analysis_saved_display_db_path(QString db_path)
{
    destination_ram_analysis_saved_db_path = db_path;
    destination_db_path = db_path;
}

void ram_analysis_saved_loader::populate_ram_analysis_saved_data_in_table()
{

    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index  = 5;
    os_name_index = 11;



    m_tablewidget_obj->hideColumn(plugin_name_index);
    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(os_name_index);


    m_tablewidget_obj->horizontalHeader()->setSectionsMovable(true);


    display_data(destination_ram_analysis_saved_db_path);

    QStringList menuList;
    menuList << QString("")
             << QString(MACRO_Generic_Right_Click_Add_Bookmark) << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark , menuList);

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
        menuList  << QString("") << QString(MACRO_Generic_Right_Click_Add_Note) << QString(MACRO_Generic_Right_Click_Remove_Note);
        submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
        create_right_click_actions(submenu_note , menuList);

        ///----Create Submenu of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        // Open With
        //        QStringList submenuList;
        //        submenuList << QString(MACRO_CASE_TREE_PLIST_VIEWER) << QString(MACRO_CASE_TREE_HEX_VIEWER) << QString(MACRO_CASE_TREE_SQLITE_VIEWER);

        //submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
        //create_right_click_actions(submenu_open_with , submenuList);

        // Send to Bucket
        //submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_send_to_bucket);
        //create_right_click_actions(submenu_send_to_bucket , submenuList);

        // Action Export
        //act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

        ///----Create Export of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

//        sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
//        create_tags_submenu();



        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard) ;

        create_right_click_actions(mainMenu , menuList);

//        connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
//        connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    }
}


void ram_analysis_saved_loader::contextMenuEvent(QContextMenuEvent *evt)
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

void ram_analysis_saved_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    QString record_no_str   = m_tablewidget_obj->item(row, record_no_index)->text();
    if(record_no_str.trimmed().isEmpty())
        return;


    QString metadata_value_str = recon_helper_standard_obj->fetch_metadata_for_ram_analysis_saved(record_no_str,Q_FUNC_INFO);

    current_media_path = "";
    preview_file_path  = "";
    metadata_str       = metadata_value_str;

    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = metadata_value_str;
    st_set_meta_obj.source_count_name = "";
    st_set_meta_obj.record_no = record_no_str;
    st_set_meta_obj.display_searched_text = searched_keyword_str;
    st_set_meta_obj.plugin_name = plugin_name;
    st_set_meta_obj.tab_name = "";


    emit signal_set_metadata_info(st_set_meta_obj);


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void ram_analysis_saved_loader::update_bookmark_value(QString status,int row, int column)
{
    recon_static_functions::app_debug("-Starts " , Q_FUNC_INFO);

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(!table_name_list.isEmpty())
    {
        QString table_name = table_name_list.at(0);

        if(bookmark_index != -1)
        {
            if(status == QString("1"))
                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Checked);
            else
                m_tablewidget_obj->item(row, bookmark_index)->setCheckState(Qt::Unchecked);
        }

        QString record_no = m_tablewidget_obj->item(row, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(row, plugin_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(row, os_name_index)->text();

        QString bookmark_col_name = db_columns_list.at(bookmark_index);
        QString plugin_col_name = db_columns_list.at(plugin_name_index);
        QString tab_column_name = db_columns_list.at(tab_name_index);
        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString os_scheme_column_name = db_columns_list.at(os_name_index);

        QStringList arg_list;
        QString  command = "UPDATE '" + table_name + "' SET " + bookmark_col_name +  " =?  WHERE " + record_no_column_name + "=? AND " + plugin_col_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?";
        arg_list << status << record_no << plugin_name_str << tab_name_str << os_name_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_ram_analysis_saved_db_path,Q_FUNC_INFO);

        emit signal_change_bookmark_value_in_other_db(record_no,plugin_name_str,tab_name_str,status,QStringList(""),destination_ram_analysis_saved_db_path,os_name_str, feature_name);
    }
    recon_static_functions::app_debug("-Ends " , Q_FUNC_INFO);
}



void ram_analysis_saved_loader::update_notes_value(QString text, int current_row)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_ram_analysis_saved_db_path);

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
        QString tab_name_str = m_tablewidget_obj->item(current_row, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(current_row, os_name_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(current_row, plugin_name_index)->text();

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);
        QString tab_column_name = db_columns_list.at(tab_name_index);
        QString os_scheme_column_name = db_columns_list.at(os_name_index);

        QString command = QString("UPDATE '" + table_name + "' set notes = ? WHERE " + record_no_column_name + "= ? AND " + plugin_column_name + "=? AND " + tab_column_name + "=? AND " + os_scheme_column_name + "=?");
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
        arg_list << record_no << plugin_column_name << tab_name_str << os_name_str;
        QString applied_tags = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

        QStringList applied_tags_list = applied_tags.split(",", Qt::SkipEmptyParts);

        emit signal_change_notes_in_other_db_also(record_no,plugin_name_str,tab_name_str,text,applied_tags_list, "", feature_name);
    }

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void ram_analysis_saved_loader::update_tags_value(QString tag_data, QString colour)
{
    recon_static_functions::debug_intensive("-Starts " , Q_FUNC_INFO);

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
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(selected_row_no, os_name_index)->text();

        QString record_no_column_name = db_columns_list.at(record_no_index);
        QString plugin_column_name = db_columns_list.at(plugin_name_index);
        QString tab_col_name = db_columns_list.at(tab_name_index);
        QString os_scheme_col_name = db_columns_list.at(os_name_index);

        QString table_name;
        if(table_name_list.size() > 0)
            table_name = table_name_list.at(0);

        QString command = QString("update '" + table_name + "' set recon_tag_value=? where " +  record_no_column_name + "= ? AND " + plugin_column_name + "=? AND " + tab_col_name + "=? AND " + os_scheme_col_name + "=?");
        QStringList arg_list;
        arg_list << tag_data << record_no_str << plugin_name_str << tab_name_str << os_name_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_ram_analysis_saved_db_path,Q_FUNC_INFO);
        int co_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
        tablewidget_general_clicked(selected_row_no,co_index);

        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);

        int tag_position = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
        m_tablewidget_obj->item(selected_row_no,tag_position)->setIcon(icon);

        emit signal_add_tag_in_other_locations(tag_data,"0",plugin_name_str,tab_name_str,record_no_str,destination_ram_analysis_saved_db_path, "");

    }

    recon_static_functions::debug_intensive("-Ends " , Q_FUNC_INFO);
}

void ram_analysis_saved_loader::action_remove_note_triggered()
{
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

        QString record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(selected_row_no , os_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no , tab_name_index)->text();

        update_notes_value("",selected_row_no);

        emit signal_update_display_on_remove_notes(plugin_name_str,tab_name_str, record, "", os_name_str);
    }
}

void ram_analysis_saved_loader::action_remove_note_bookmark_triggered()
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
            QString plugin_name_str = m_tablewidget_obj->item(k, plugin_name_index)->text();
            QString tab_name_str = m_tablewidget_obj->item(k, tab_name_index)->text();

            update_notes_value(text,k);

            emit signal_update_display_on_remove_notes(plugin_name_str,tab_name_str, record, text, os_name_str);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}
