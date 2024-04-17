#include "notes_search_loader.h"

notes_search_loader::notes_search_loader(QWidget *parent)
{
    setWindowFlags(Qt::Window);

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

notes_search_loader::~notes_search_loader()
{

}

void notes_search_loader::pub_notes_search_set_display_db_path(QString db_path)
{
    destination_db_path_for_note_search = db_path;
}

void notes_search_loader::notes_search_populate_data_in_table()
{

    record_no_index = 0;
    plugin_name_index = 2;
    tab_name_index = 3;
    os_name_index = 4;
    source_count_name_index = 5;
    item_0_index = 6;
    item_1_index = 7;
    file_path_index = 7;
    item_2_index = 8;
    search_db_path_index = 8;
    hex_and_text_viewer_start_offset_index = 8;
    sqlite_viewer_db_table_name_index = 8;
    item_3_index = 9;
    hex_and_text_viewer_end_offset_index = 9;
    item_4_index = 10;
    saved_map_path_index = 10;
    item_5_index = 11;
    plist_viewer_display_name_index = 11;
    item_6_index = 12;
    item_7_index = 13;
    item_8_index = 14;
    item_9_index = 15;
    item_10_index = 16;
    item_11_index = 17;

    item_12_index = 18;
    item_13_index = 19;
    item_14_index = 20;
    item_15_index = 21;

    item_16_index = 22;
    item_17_index = 23;
    item_18_index = 24;
    item_19_index = 25;





    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);

    display_data(destination_db_path_for_note_search);

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Remove_Note);
    submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
    create_right_click_actions(submenu_note, menuList);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    ///----Create Submenu of existing Menu
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator
    act_quick_look = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Quick_look),mainMenu);

    //    //Open With
    QStringList submenuList;
    submenuList << QString(MACRO_CASE_TREE_Plist_Viewer) << QString(MACRO_CASE_TREE_Hex_Viewer)
                << QString(MACRO_CASE_TREE_Sqlite_Viewer)
                << QString(MACRO_CASE_TREE_Log_Viewer);

    submenu_open_with = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Open_With) , mainMenu , submenu_open_with);
    create_right_click_actions(submenu_open_with , submenuList);

    act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard) ;
    create_right_click_actions(mainMenu , menuList);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Send to Bucket
    submenu_send_to_bucket = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Send_To_Bucket) , mainMenu , submenu_send_to_bucket);
    create_right_click_actions(submenu_send_to_bucket , submenuList);
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Hashset
    submenu_add_files_to_hashset_db = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database) , mainMenu , submenu_add_files_to_hashset_db);

    QList <QMenu*> hahses_menu_list;
    hahses_menu_list.append(submenu_add_files_to_hashset_db);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(hahses_menu_list, Q_FUNC_INFO);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    // Export
    act_export = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export),mainMenu);

    sub_menu_export_files =  mainMenu->addMenu(MACRO_Generic_Right_Click_Export_Filesystem_Records);
    action_export_all_file_system_records = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export_All_Filesystem_Records),sub_menu_export_files);
    action_export_selected_file_system_records = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Export_Selected_Filesystem_Records),sub_menu_export_files);
    sub_menu_export_files->addAction(action_export_all_file_system_records);
    sub_menu_export_files->addAction(action_export_selected_file_system_records);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    create_right_click_actions(mainMenu , QStringList(QString(MACRO_Generic_Right_Click_Go_To_Source)));

    submenu_go_to_artifact_source = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Go_To_Artifacts_Source) , mainMenu , submenu_go_to_artifact_source);

    act_go_to_record = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Go_To_Record),mainMenu);

    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));
    connect(submenu_open_with, SIGNAL(triggered(QAction*)), this, SLOT(slot_open_with_submenu_clicked(QAction*)));
    connect(submenu_send_to_bucket, SIGNAL(triggered(QAction*)), this, SLOT(slot_sendtobucket_submenu_clicked(QAction*)));
    connect(submenu_add_files_to_hashset_db,SIGNAL(triggered(QAction*)),this,SLOT(slot_add_files_to_hashset_submenu_clicked(QAction*)));
    connect(submenu_go_to_artifact_source,SIGNAL(triggered(QAction*)),this,SLOT(slot_goto_artifact_source_submenu_clicked(QAction*)));

    connect(action_export_all_file_system_records, SIGNAL(triggered()), this, SLOT(slot_action_export_all_file_system_records_triggered()));
    connect(action_export_selected_file_system_records, SIGNAL(triggered()), this, SLOT(slot_action_export_selected_file_system_records_triggered()));

}

void notes_search_loader::contextMenuEvent(QContextMenuEvent *evt)
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

    create_add_files_to_hashset_submenu();

    // tablewidget_general_clicked(current_row,current_column);



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
        act_export->setEnabled(true);
        act_full_detail_info->setEnabled(false);
        act_quick_look->setEnabled(false);
    }
    else
    {
        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            act_export->setEnabled(true);
            submenu_send_to_bucket->setEnabled(true);
            submenu_open_with->setEnabled(true);
            submenu_add_files_to_hashset_db->setEnabled(true);
            act_go_to_record->setEnabled(true);
            act_quick_look->setEnabled(true);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            act_export->setEnabled(false);
            sub_menu_export_files->setDisabled(true);
            submenu_send_to_bucket->setEnabled(false);
            submenu_open_with->setEnabled(false);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots)
                || plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
        {
            submenu_send_to_bucket->setEnabled(false);
            submenu_open_with->setEnabled(false);
            submenu_add_files_to_hashset_db->setEnabled(false);
            act_quick_look->setEnabled(true);
            act_export->setEnabled(true);
            act_go_to_record->setEnabled(true);
            sub_menu_export_files->setDisabled(true);
            action_export_selected_file_system_records->setDisabled(true);

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
                )
        {
            sub_menu_export_files->setDisabled(true);
            action_export_selected_file_system_records->setDisabled(true);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
        {
            act_go_to_record->setEnabled(true);
            //            submenu_send_to_bucket->setEnabled(false);
            //            submenu_open_with->setEnabled(false);
            //            submenu_add_files_to_hashset_db->setEnabled(false);
            //            submenu_search_file_with_same_md5_hash->setEnabled(false);
            //            act_quick_look->setEnabled(true);
            //            act_export->setEnabled(true);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR) )
        {
            act_go_to_record->setEnabled(true);
            submenu_send_to_bucket->setEnabled(false);
            submenu_open_with->setEnabled(false);
            submenu_add_files_to_hashset_db->setEnabled(false);

            act_export->setEnabled(true);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        {

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            sub_menu_export_files->setEnabled(false);
            act_go_to_record->setEnabled(true);
            submenu_send_to_bucket->setEnabled(true);
            submenu_open_with->setEnabled(true);
            submenu_add_files_to_hashset_db->setEnabled(false);
            act_export->setEnabled(true);
            act_quick_look->setEnabled(true);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
        {
            act_go_to_record->setEnabled(true);
            sub_menu_export_files->setEnabled(false);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
        {
            sub_menu_export_files->setEnabled(false);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
        {
            sub_menu_export_files->setEnabled(false);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
        {
            act_go_to_record->setEnabled(true);
            sub_menu_export_files->setEnabled(false);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
        {
            sub_menu_export_files->setEnabled(false);
            sub_menu_export_files->setEnabled(false);
            submenu_add_files_to_hashset_db->setEnabled(false);
            act_go_to_record->setEnabled(true);
            act_export->setEnabled(true);
            act_quick_look->setEnabled(true);
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
                act_quick_look->setEnabled(true);
            }

            submenu_go_to_artifact_source->setEnabled(true);
            act_go_to_record->setEnabled(true);
        }
    }

    enable_disable_go_to_source(current_row, plugin_name_str);

    //    QList<QAction *> menu_list_go_to_source = mainMenu->actions();
    //    for(int count = 0; count < menu_list_go_to_source.size(); count++)
    //    {
    //        QAction *act = menu_list_go_to_source.at(count);
    //        if(act->text() == QString(MACRO_Generic_Right_Click_Go_To_Source))
    //        {
    //            if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
    //                    || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
    //                    || plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer)
    //                    || plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
    //                act->setEnabled(true);
    //            else
    //                act->setEnabled(false);
    //        }
    //    }

    if(plugin_name_str != QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History) && plugin_name_str != QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        create_action_for_goto_artifact_source_submenu();
    }

    QString source_count_name = m_tablewidget_obj->item(current_row,source_count_name_index)->text();
    QString record_no = m_tablewidget_obj->item(current_row,record_no_index)->text();
    struct_global_right_clicks_status_depends_on_file_existance obj;
    obj.plugin_name = plugin_name_str;
    obj.feature_name = QString(MACRO_CASE_TREE_Notes_Parent);
    obj.tab_name = m_tablewidget_obj->item(current_row, tab_name_index)->text();
    obj.selected_row_count = selection_model_list.size();
    obj.record_no = record_no;
    obj.source_count_name = source_count_name;
    if(plugin_name_str == MACRO_Plugin_Name_Carved_Files)
    {
        obj.arguments_values_list.clear();
        obj.arguments_values_list << m_tablewidget_obj->item(current_row,item_3_index)->text();
    }
    else if(plugin_name_str == MACRO_Plugin_Name_Email_Parser)
    {
        obj.arguments_values_list.clear();
        obj.arguments_values_list << m_tablewidget_obj->item(current_row,item_9_index)->text();
    }
    else if(plugin_name_str == MACRO_Plugin_Name_Snapshots)
    {
        obj.arguments_values_list = QStringList(m_tablewidget_obj->item(current_row,item_3_index)->text());
    }
    recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

    if(plugin_name_str != QString(MACRO_Plugin_Name_File_System))
    {
        sub_menu_export_files->setDisabled(true);
    }

    mainMenu->exec(QCursor::pos());
}

void notes_search_loader::tablewidget_general_clicked(int row,int column)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
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

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(row, tab_name_index)->text();
    QString source_count_name = m_tablewidget_obj->item(row, source_count_name_index)->text();
    struct_GLOBAL_witness_info_source target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    //QString os_name_str = target_source_info.os_scheme_display;

    QString searched_text = searched_keyword_str;

    QString metadata_value_str,final_plugin_name, finaltab_name;


    final_plugin_name = plugin_name_str;
    finaltab_name = tab_name_str;


    QString metadata_db_path;
    if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_file_system_metadata_for_current_record(record_no,source_count_name, Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_hex_viewer(record_no,source_count_name, Q_FUNC_INFO);
        metadata_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";
        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    {
        QString filepath = m_tablewidget_obj->item(row, file_path_index)->text();
        QString db_table_name = m_tablewidget_obj->item(row, sqlite_viewer_db_table_name_index)->text();

        QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";
        QString command = "SELECT recon_tag_value FROM sv_content_info WHERE file_path = ?  AND record_no_in_sqliteviewer = ? AND db_table_name = ? AND sql_tab_name = ?";
        QStringList val_list;
        val_list << filepath << record_no << db_table_name << tab_name_str;
        QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

        struct_global_fetch_metadata struct_metadata_values_obj;
        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString source_name_str = struct_target_info.source_name;

        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_sqlite_viewer_tag_entry(filepath,tag_name,record_no,db_table_name, source_name_str,Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
    {
        QString filepath = m_tablewidget_obj->item(row, file_path_index)->text();
        QString viewer_display_name = m_tablewidget_obj->item(row, plist_viewer_display_name_index)->text();

        QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

        QString command = "SELECT recon_tag_value FROM plist_content_info WHERE file_path = ? AND viewer_display_name =  ? AND record_no = ?";
        QStringList val_list;
        val_list << filepath << viewer_display_name << record_no;
        QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

        struct_global_fetch_metadata struct_metadata_values_obj;

        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString source_name_str = struct_target_info.source_name;
        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_plist_viewer_tag_entry(filepath,tag_name,viewer_display_name, record_no,source_name_str, Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer))
    {
        QString filepath = m_tablewidget_obj->item(row, file_path_index)->text();
        QString viewer_display_name = m_tablewidget_obj->item(row, item_5_index)->text();
        QString tagged_data = m_tablewidget_obj->item(row, item_2_index)->text();

        QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";

        QString command = "SELECT recon_tag_value FROM log_content_info WHERE file_path = ? AND viewer_display_name = ? AND record_no = ? AND tagged_data = ?";

        QStringList val_list;
        val_list << filepath << viewer_display_name << record_no << tagged_data;
        QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,exported_content_info_db,Q_FUNC_INFO);

        struct_global_fetch_metadata struct_metadata_values_obj;
        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_log_viewer_tag_entry(filepath, tag_name, viewer_display_name, record_no, source_count_name, tagged_data, Q_FUNC_INFO);

        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
    {
        metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
        struct_global_fetch_metadata struct_metadata_values_obj;
        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_screenshots_tag_entry(m_tablewidget_obj->item(row, file_path_index)->text(),record_no,Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR)||plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
    {

        QString temp_tab_name = tab_name_str;
        temp_tab_name.replace(" ","_");

        struct_global_fetch_metadata struct_metadata_values_obj;

        struct_global_csv_reader_full_csv_info md_csv_redaer_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
        QString command;
        if(md_csv_redaer_obj.metadata_column_name_QList_tablewidget.size() > 0)
        {
            QStringList db_cols_nam_list =  md_csv_redaer_obj.metadata_column_name_QList_tablewidget.at(0);
            command = db_cols_nam_list.join(",");
            command = "select " + command + " from redefined_result where INT = '" + record_no + "'";
        }


        QString metadata_str;
        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + plugin_name_str + "\n<br>";
        metadata_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + tab_name_str + "\n<br>";

        struct_GLOBAL_witness_info_source struct_target_info;
        struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        struct_metadata_values_obj  = recon_helper_standard_obj->fetch_metadata_for_features_plugins("",0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + temp_tab_name +".sqlite" ,command,struct_target_info,Q_FUNC_INFO);
        metadata_str += struct_metadata_values_obj.metadata;

        metadata_value_str = metadata_str;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
    {
        struct_global_fetch_metadata struct_metadata_values_obj;
        QString stamp_type_tm = m_tablewidget_obj->item(row,item_5_index)->text();

        QString plugin_name = m_tablewidget_obj->item(row,item_0_index)->text();
        QString tab_name = m_tablewidget_obj->item(row,item_1_index)->text();

        struct_global_csv_reader_full_csv_info md_csv_redaer_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
        QString command;
        QString timeline_table_name;
        timeline_table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Timeline));
        if(md_csv_redaer_obj.metadata_column_name_QList_tablewidget.size() > 0)
        {
            QStringList db_cols_nam_list =  md_csv_redaer_obj.metadata_column_name_QList_tablewidget.at(0);
            command = db_cols_nam_list.join(",");
            command = "select " + command + " from " + timeline_table_name + " where record_no = '" + record_no +"'" + " AND stamp_type = '" +  stamp_type_tm + "'" + " AND plugin = '" +  plugin_name + "'" + " AND category = '" +  tab_name + "'";
        }


        QString metadata_str;
        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + plugin_name_str + "\n<br>";
        metadata_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + tab_name_str + "\n<br>";

        struct_GLOBAL_witness_info_source struct_target_info;
        struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        if(tab_name_str == QString(MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event))
        {

            struct_metadata_values_obj  = recon_helper_standard_obj->fetch_metadata_for_features_plugins(timeline_table_name,0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite" ,command,struct_target_info,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;
        }
        else
        {
            struct_metadata_values_obj  = recon_helper_standard_obj->fetch_metadata_for_features_plugins(timeline_table_name,0,record_no,plugin_name_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + tab_name_str.replace(" ", "_") + ".sqlite", command,struct_target_info,Q_FUNC_INFO);
            metadata_str += struct_metadata_values_obj.metadata;
        }

        metadata_value_str = metadata_str;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
    {
        QString metadata_str;

        QString all_notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(all_notes_db_path);
        QSqlQuery query_select(destination_db);

        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" ---FAILED--- to open DB" + all_notes_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QString command = QString("select INT,plugin_name,item0,item1,item2,item3,item4,item5,recon_tag_value,notes,tab_name from notes where record_no = ? AND tab_name = ? AND plugin_name = ?");
        query_select.prepare(command);
        query_select.addBindValue(record_no);
        query_select.addBindValue(tab_name_str);
        query_select.addBindValue(plugin_name_str);

        if(!query_select.exec())
        {
            recon_static_functions::app_debug("  ---FAILED--- select query",Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select.executedQuery(),Q_FUNC_INFO);
            destination_db.close();
            return;
        }

        while(query_select.next())
        {
            ///item0=result name    ; item1=source name ; item2=operating system name;
            ///item3=build version  ; item4=artifacts   ; item5=output command

            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br>" ;
            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + query_select.value(1).toString() + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + query_select.value(10).toString() + "\n<br>";
            metadata_str += "<br>";
            metadata_str += "<b>" + QObject::tr("Item0") + ":</b>" +QString("\t") + query_select.value(2).toString() + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Item1") + ":</b>" +QString("\t") + query_select.value(3).toString() + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Item2") + ":</b>" +QString("\t") + query_select.value(4).toString() + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Item3") + ":</b>" +QString("\t") + query_select.value(5).toString() + "\n<br>";
            metadata_str += "<b>" + QObject::tr("Item4") + ":</b>" +QString("\t") + query_select.value(6).toString() + "\n<br>";

            QString command_output = "<b>" + QObject::tr("Item5") + ":</b>" +QString("\t") + query_select.value(7).toString() + "\n<br>";
            if(command_output.contains("\n\n"))
                command_output.replace("\n\n","<br><br>");


            metadata_str += command_output;

            QString tag_text = "";
            QStringList list_tags = query_select.value(8).toString().split(",", Qt::SkipEmptyParts);
            for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
            {
                if(list_tags.at(tag_i).isEmpty())
                    continue;

                QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
            }

            if(tag_text.endsWith(","))
                tag_text.chop(1);

            metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";
            metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(9).toString() +"<br>" + "</font>\n";
        }

        metadata_value_str = metadata_str;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger)||plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        QString db_path;
        if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
            db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() +  "Browser_History.sqlite" ;
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
            db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() +   "Messenger.sqlite";

        metadata_db_path = db_path;
        struct_global_fetch_metadata struct_metadata_values_obj;
        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_graph(record_no,plugin_name_str,Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";

        QString filepath = m_tablewidget_obj->item(row, file_path_index)->text();

        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_timeline_graph_entry(filepath, record_no,Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_maps_entry(m_tablewidget_obj->item(row, file_path_index)->text(), record_no,Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(m_tablewidget_obj->item(row,9)->text()),0,index_db_path,Q_FUNC_INFO);

        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_carved_files(carved_files_db_path, record_no,Q_FUNC_INFO);
        metadata_value_str = struct_metadata_values_obj.metadata;
        metadata_db_path = carved_files_db_path;

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
    {
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
        QString cmd_2 = "select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?";
        QString result_tab_name = m_tablewidget_obj->item(row,17)->text();
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(result_tab_name),0,index_db_path,Q_FUNC_INFO);
        QString registry_viewer_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based("tbl_registry_tree",0,record_no,plugin_name_str,registry_viewer_db_path,source_count_name,Q_FUNC_INFO);
        QString metadata_str;
        metadata_str += struct_metadata_values_obj.metadata;
        metadata_value_str = metadata_str;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
    {
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
        QString cmd_2 = "select relative_db_path from tbl_carved_password_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(m_tablewidget_obj->item(row,10)->text()),0,index_db_path,Q_FUNC_INFO);
        QString carved_password_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based("carved_password",0,record_no,plugin_name_str,carved_password_db_path,source_count_name,Q_FUNC_INFO);
        QString metadata_str;
        metadata_str += struct_metadata_values_obj.metadata;

        metadata_value_str = metadata_str;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
    {
        QString mobile_backup_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , MACRO_Plugin_Mobile_Backup_TAB_Name_Backup);
        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name, 0, record_no, plugin_name_str, mobile_backup_db_path, source_count_name, Q_FUNC_INFO);

        QString metadata_str;
        metadata_str += struct_metadata_values_obj.metadata;
        metadata_value_str = metadata_str;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
    {
        QString table_name = m_tablewidget_obj->item(row , item_12_index)->text();
        metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";
        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name,0,record_no,plugin_name_str,metadata_db_path,source_count_name,Q_FUNC_INFO);
        QString metadata_str;
        metadata_str += struct_metadata_values_obj.metadata;
        metadata_value_str = metadata_str;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        QString snapshot_label_name = m_tablewidget_obj->item(row , item_3_index)->text();
        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , tab_name_str);
        metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";

        struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based(table_name,0,record_no,plugin_name_str,metadata_db_path,source_count_name,Q_FUNC_INFO);
        QString metadata_str;

        metadata_str += struct_metadata_values_obj.metadata;
        metadata_value_str = metadata_str;
    }
    else
    {
        struct_global_fetch_metadata struct_metadata_values_obj;
        struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_of_plugin(plugin_name_str,tab_name_str,record_no,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

        final_plugin_name = plugin_name_str;
        finaltab_name = tab_name_str;
        metadata_value_str = struct_metadata_values_obj.metadata;

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
        QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;
        if(!QFileInfo(plugins_destination_db_file).exists())
            return ;

        metadata_db_path = plugins_destination_db_file;

    }

    metadata_str       = metadata_value_str;

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = metadata_value_str;
    st_set_meta_obj.db_file_path = metadata_db_path;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = searched_text;
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = tab_name_str;


    emit signal_set_metadata_info(st_set_meta_obj);
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_open_full_detailed_info_triggered()
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

    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();

    struct_global_recon_file_info st_globl_recon_file_info_obj;
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_hex_viewer(record_no,source_count_name);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path , source_count_name);
    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str, current_media_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,"");
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str, current_media_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,"");
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(preview_file_path);
    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str,  preview_file_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,"");
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR)
    //            || plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(preview_file_path);
    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str, preview_file_path, preview_index_full,searched_keyword,source_count_name,record_no, plugin_name_str,"");

    //    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();
        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_EMLX_File) || plugin_name_str == QString(MACRO_Plugin_Name_Google_Mail))
    //    {
    //        int preview_index_full = 0;
    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,source_count_name, record_no, plugin_name_str, "");
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password) || plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer)
    //            ||plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
    //    {
    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", 0,searched_keyword,source_count_name,record_no, plugin_name_str,"");
    //    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_screenshots(record_no);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_timeline_graphs(record_no);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_saved_maps(record_no);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_browser_histroy_saved_graphs(record_no);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_messenger_saved_graphs(record_no);
    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
    //    {
    //        full_detail_info_obj->pub_show_full_detail_info(metadata_str,"", 0,searched_keyword,source_count_name,record_no, "","");
    //    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        QString snapshot_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow() , item_3_index)->text();
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_snapshots_by_db_path(record_no,tab_name_str,db_path,source_count_name);
    }
    else
    {
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str ,record_no,plugin_db_file);

    }

    QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    QString display_file_path =  st_globl_recon_file_info_obj.display_file_path;
    int preview_index_full = get_stack_index_for_preview(complete_file_path ,display_file_path);
    full_detail_info_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    full_detail_info_obj->pub_set_metadata_values_obj(st_set_meta_obj);
    full_detail_info_obj->pub_show_full_detail_info(metadata_str, "", preview_index_full,searched_keyword,"",record_no, plugin_name_str,"");

    full_detail_info_obj->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_open_detach_detailed_info_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    //    if(m_tablewidget_obj->currentRow() < 0)
    //        return;

    //    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    //    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    //    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    //    QString searched_keyword = searched_keyword_str;

    //    if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots) || plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    //    {
    //        int preview_index_full = 1;

    //        QString filepath = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), file_path_index)->text();
    //        if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    //        {
    //            QString recon_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    //            if(!filepath.startsWith(recon_result_dir))
    //                filepath = recon_static_functions::get_complete_file_path(recon_result_dir, filepath, Q_FUNC_INFO);
    //        }

    //        preview_index_full = get_stack_index_for_preview(filepath);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, filepath, filepath, preview_index_full,searched_keyword,"");

    //        recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    //        return;

    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
    //    {

    //        int preview_index_full = 1;

    //        QString filepath = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), file_path_index)->text();

    //        if(!filepath.isEmpty())
    //        {
    //            struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_for_saved_maps_entry(filepath, record_no,Q_FUNC_INFO);
    //            filepath = struct_metadata_values_obj.preview_file_path;

    //            preview_index_full = get_stack_index_for_preview(filepath);

    //            emit signal_open_in_detach_clicked_table_result(metadata_str, filepath, filepath, preview_index_full,searched_keyword,"");
    //        }
    //        recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    //        return;
    //    }

    //    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();

    //    if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, current_media_path, preview_index_full,searched_keyword,source_count_name);
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //        QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(),source_count_name_index)->text();

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, current_media_path, preview_index_full,searched_keyword, source_count_name);
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, current_media_path, preview_index_full,searched_keyword,source_count_name);
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(current_media_path);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, current_media_path, preview_index_full,searched_keyword,source_count_name);
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(preview_file_path);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, preview_file_path, preview_index_full,searched_keyword,source_count_name);
    //    }
    //    else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR) )
    //    {
    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(preview_file_path);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, preview_file_path, preview_index_full,searched_keyword,source_count_name);
    //    }
    //    else
    //    {
    //        QString tmp_plg_name = plugin_name_str;
    //        tmp_plg_name.replace(" ","_");

    //        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    //        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    //        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    //        QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();
    //        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    //        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
    //            return;

    //        QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    //        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

    //        int preview_index_full = 0;
    //        preview_index_full = get_stack_index_for_preview(preview_file_path);

    //        emit signal_open_in_detach_clicked_table_result(metadata_str, current_media_path, preview_file_path, preview_index_full,searched_keyword,source_count_name);

    //    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::update_notes_value(QString text, int current_row)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(current_row < 0)
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- " + destination_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("   " + destination_db.lastError().text() , Q_FUNC_INFO);
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

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            QString source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();

            QStringList m_list;
            m_list << record_no;

            global_connection_manager_obj->pub_remove_notes_in_fs_source_db(m_list, source_count_name);

            struct_global_tagging_tag_notes_db_updation obj;
            obj.source_count_name = source_count_name;
            obj.tab_name = QString(MACRO_Plugin_File_System_TAB_Name_Files);
            obj.record_no = record_no;
            obj.plugin_name = QString(MACRO_Plugin_Name_File_System);


            QList<struct_global_tagging_tag_notes_db_updation> list_st_tag_item_list;
            list_st_tag_item_list.append(obj);

            global_connection_manager_obj->pub_delete_notes_for_file_system(list_st_tag_item_list);

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            QString command_select_tag = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=?";
            QStringList arg_list;
            arg_list << record_no << plugin_name_str;
            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,arg_list,0,destination_db_path,Q_FUNC_INFO);

            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

            QString start_offset = m_tablewidget_obj->item(current_row, hex_and_text_viewer_start_offset_index)->text();
            QString end_offset = m_tablewidget_obj->item(current_row, hex_and_text_viewer_end_offset_index)->text();
            QString filepath = m_tablewidget_obj->item(current_row, file_path_index)->text();

            QString command = "UPDATE 'hex_content_info' SET notes = ? WHERE file_path = ? AND recon_tag_value = ? AND start_index =  ? AND end_index = ?";
            arg_list.clear();
            arg_list  << text <<  filepath << tag_name << start_offset << end_offset;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);
            destination_db.close();

            QString source_count_str = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
            struct_global_tagging_tag_notes_db_updation obj;
            obj.plugin_name = plugin_name_str;
            obj.tab_name = tab_name_str;
            obj.item1 = filepath;
            obj.item2 = start_offset;
            obj.item3 = end_offset;
            obj.source_count_name = source_count_str;

            QList<struct_global_tagging_tag_notes_db_updation> list_st_tag_item_list;
            list_st_tag_item_list.append(obj);
            global_connection_manager_obj->pub_delete_notes_for_hex_viewer_db(list_st_tag_item_list);
            global_connection_manager_obj->pub_set_notes_in_tags_search_for_hex_viewer(list_st_tag_item_list);

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
        {
            global_connection_manager_obj->pub_remove_notes_in_redefined_db(QStringList(record_no),plugin_name_str);
            global_connection_manager_obj->pub_delete_notes_for_redefined_db(QStringList(record_no),plugin_name_str);

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots) || plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History) || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger)
                || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            global_connection_manager_obj->pub_remove_notes_in_saved_graphs_and_screenshot_db(QStringList(record_no) , plugin_name_str);
            global_connection_manager_obj->pub_delete_notes_for_saved_graphs_and_screenshot_db(QStringList(record_no) , plugin_name_str);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
        {

            QString plugin_name = m_tablewidget_obj->item(current_row,item_0_index)->text();
            QString tab_name = m_tablewidget_obj->item(current_row,item_1_index)->text();
            QString stamp_type_tm = m_tablewidget_obj->item(current_row,item_5_index)->text();
            QString source_count_name = m_tablewidget_obj->item(current_row,source_count_name_index)->text();

            struct_global_tagging_artifact_timeline_record obj;
            obj.plugin_record_no = record_no;
            obj.plugin_name = plugin_name;
            obj.tab_name = tab_name;
            obj.item5 = stamp_type_tm;
            obj.source_count_name = source_count_name;

            QList<struct_global_tagging_artifact_timeline_record> artifact_timeline_record_list;

            artifact_timeline_record_list.append(obj);
            global_connection_manager_obj->pub_remove_notes_in_artifact_timeline_db_from_outside(artifact_timeline_record_list);
            global_connection_manager_obj->pub_delete_notes_for_artifact_timeline(artifact_timeline_record_list);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString result_tab_name = m_tablewidget_obj->item(current_row,9)->text();
            global_connection_manager_obj->pub_remove_notes_in_carved_files_db(QStringList(record_no),result_tab_name);
            global_connection_manager_obj->pub_delete_notes_for_carved_files_db(QStringList(record_no),result_tab_name);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
        {
            QString result_tab_name = m_tablewidget_obj->item(current_row,10)->text();
            global_connection_manager_obj->pub_remove_notes_in_carved_password_db(QStringList(record_no),result_tab_name);
            global_connection_manager_obj->pub_delete_notes_for_carved_password_db(QStringList(record_no),result_tab_name);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer))
        {
            QString result_tab_name = m_tablewidget_obj->item(current_row,17)->text();
            global_connection_manager_obj->pub_remove_notes_in_registry_viewer_db(QStringList(record_no),result_tab_name);
            global_connection_manager_obj->pub_delete_notes_for_registry_viewer_db(QStringList(record_no),result_tab_name);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup))
        {
            global_connection_manager_obj->pub_remove_notes_in_Mobile_Backup_db(QStringList(record_no));
            global_connection_manager_obj->pub_delete_notes_for_Mobile_Backup_db(QStringList(record_no));
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            QString db_table_name = m_tablewidget_obj->item(current_row, 16)->text();
            QString filepath = m_tablewidget_obj->item(current_row, file_path_index)->text();
            QString result_tab_name = m_tablewidget_obj->item(current_row,17)->text();

            global_connection_manager_obj->pub_delete_or_update_record_export_content_sqlite_viewer_db(QStringList(record_no) ,db_table_name , filepath ,tab_name_str ,"");
            global_connection_manager_obj->pub_remove_notes_in_sqlite_viewer_db(QStringList(record_no),result_tab_name ,db_table_name,tab_name_str);
            global_connection_manager_obj->pub_delete_notes_for_sqlite_viewer_db(QStringList(record_no),result_tab_name ,db_table_name,tab_name_str);
            global_connection_manager_obj->pub_set_notes_in_tags_search_for_sqlite_viewer(QStringList(record_no),tab_name_str,db_table_name,result_tab_name,text);
            global_connection_manager_obj->pub_set_notes_in_bookmark_search_for_sqlite_viewer(QStringList(record_no),tab_name_str,db_table_name,result_tab_name,text);
        }
        else if(plugin_name_str == MACRO_Plugin_Name_Plist_Viewer)
        {
            QString viewer_display_name = m_tablewidget_obj->item(current_row, item_5_index)->text();
            QString plist_db_table_name = m_tablewidget_obj->item(current_row, 12)->text();
            QString source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
            QString file_path = m_tablewidget_obj->item(current_row, file_path_index)->text();
            QString plist_content_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
            QStringList arglist;
            QString cmd = "select bookmark , recon_tag_value from plist_content_info where viewer_display_name = ? AND file_path = ? AND source_count_name = ? AND record_no = ?";
            arglist << viewer_display_name << file_path << source_count_name << record_no;

            QString bookmark_status =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,arglist,0,plist_content_db,Q_FUNC_INFO);
            QString tag_text =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,arglist,1,plist_content_db,Q_FUNC_INFO);

            if(bookmark_status == QString("0") && tag_text.trimmed().isEmpty())
                global_connection_manager_obj->pub_delete_entry_export_content_plist_viewer_db(QStringList(record_no) ,viewer_display_name);
            else
                global_connection_manager_obj->pub_update_tag_export_content_plist_viewer_db(QStringList(record_no) ,viewer_display_name);

            global_connection_manager_obj->pub_remove_notes_in_plist_viewer_db(QStringList(record_no) ,plist_db_table_name);
            global_connection_manager_obj->pub_delete_notes_for_plist_viewer(QStringList(record_no) ,viewer_display_name);
            global_connection_manager_obj->pub_set_notes_in_bookmark_search_for_plist_viewer(QStringList(record_no),viewer_display_name ,text);
            global_connection_manager_obj->pub_set_notes_in_tags_search_for_plist_viewer(QStringList(record_no),viewer_display_name ,text);
        }
        else if(plugin_name_str == MACRO_Plugin_Name_Log_Viewer)
        {
            QString viewer_display_name = m_tablewidget_obj->item(current_row, item_5_index)->text();
            QString tagged_data_str = m_tablewidget_obj->item(current_row, item_2_index)->text();

            global_connection_manager_obj->pub_update_notes_export_content_log_viewer_db(QStringList(record_no), viewer_display_name, text, tagged_data_str);
            global_connection_manager_obj->pub_delete_notes_for_log_viewer(QStringList(record_no) ,viewer_display_name, tagged_data_str);
            global_connection_manager_obj->pub_set_notes_in_tags_search_for_log_viewer(QStringList(record_no),viewer_display_name, text, tagged_data_str);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
        {
            QString source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
            QString category_name = m_tablewidget_obj->item(current_row, item_11_index)->text();
            QString db_table_name = m_tablewidget_obj->item(current_row, item_12_index)->text();

            QString export_content_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString() + "carved_data_content_info.sqlite";

            QString cmd = "select bookmark , recon_tag_value from tbl_carved_data where record_no = ? AND category_name = ? AND source_count_name = ?";

            QStringList arg_list;
            arg_list << record_no << category_name << source_count_name;
            QString tagging_on_record;

            QString bookmark_value =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,arg_list,0,export_content_db_path,Q_FUNC_INFO);
            QString tag_name =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,arg_list,1,export_content_db_path,Q_FUNC_INFO);
            if(bookmark_value.trimmed() == MACRO_FALSE_VALUE_NUMERIC_QString && tag_name.trimmed().isEmpty())
                tagging_on_record = MACRO_FALSE_VALUE_NUMERIC_QString;
            else
                tagging_on_record = MACRO_TRUE_VALUE_NUMERIC_QString;


            QString notes_value = "";
            global_connection_manager_obj->pub_update_notes_export_content_carved_data_db(QStringList(record_no),category_name,QStringList(source_count_name),notes_value);
            global_connection_manager_obj->pub_delete_entry_export_content_carved_data_db(QStringList(record_no),category_name ,QStringList(source_count_name) , QStringList(tagging_on_record));
            global_connection_manager_obj->pub_remove_notes_in_carved_data_db(QStringList(record_no),db_table_name);
            global_connection_manager_obj->pub_delete_notes_for_carved_data_db(QStringList(record_no),category_name ,QStringList(source_count_name));
            return;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
        {
            QList<struct_global_tagging_tag_notes_db_updation> tagging_record_list;

            QString snapshot_label_name = m_tablewidget_obj->item(current_row, item_3_index)->text();

            struct_global_tagging_tag_notes_db_updation s1;
            s1.record_no = record_no;
            s1.plugin_name = plugin_name_str;
            s1.tab_name = tab_name_str;
            s1.source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();
            s1.item3 = snapshot_label_name;

            tagging_record_list << s1;


            global_connection_manager_obj->pub_remove_notes_in_snapshots_difference_db(QStringList(record_no) ,snapshot_label_name,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str));
            global_connection_manager_obj->pub_delete_notes_for_snapshots_difference(tagging_record_list);
            return ;
        }
        else
        {

            global_connection_manager_obj->pub_remove_notes_in_plugin_viewer_db(QStringList(record_no), plugin_name_str, tab_name_str);
            global_connection_manager_obj->pub_delete_notes_for_plugin_viewer(QStringList(record_no), plugin_name_str, tab_name_str, QString(""));

            emit signal_change_notes_in_other_db_also(record_no,plugin_name_str,tab_name_str,text,applied_tags_list, os_name_str, feature_name);

            global_connection_manager_obj->pub_remove_notes_in_artifacts_keyword_search_db(QStringList(record_no),plugin_name_str,tab_name_str);
        }
    }

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void notes_search_loader::action_sendToBucket_hex_viewer_triggered()
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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj;
        QStringList display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;
        bool bool_is_record_for_artifacts = false;

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);
            QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << QString(complete_path + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            //            QString start_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_start_offset_index)->text();
            //            QString end_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_end_offset_index)->text();
            //            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            //            QString record_no_column_name = db_columns_list.at(record_no_index);
            //            QString plugin_column_name = db_columns_list.at(plugin_name_index);

            //            QString table_name = table_name_list.at(0);     // Because Loader supports only one tab in CSV

            //            QString command_select_tag = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=?";
            //            QStringList arg_list;
            //            arg_list << record_no << plugin_name_str;
            //            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,arg_list,0,destination_db_path,Q_FUNC_INFO);

            //            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

            //            QString command = "SELECT content_file_path from hex_content_info WHERE file_path = ? AND recon_tag_value = ? AND start_index =  ? AND end_index = ?";
            //            QStringList values_list;
            //            values_list << filepath << tag_name << start_offset << end_offset;

            //            QString content_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,values_list,0,exported_content_info_db,Q_FUNC_INFO);
            //            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            //            if(!content_file_path.isEmpty() && !content_file_path.startsWith(result_dir_path))
            //                content_file_path.prepend(result_dir_path);

            //            all_path_list << content_file_path + MACRO_RECON_Splitter_1_multiple + source_count_name;

            //            QString block_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,values_list,0,exported_content_info_db,Q_FUNC_INFO);

            //            all_path_list << block_file_path + MACRO_RECON_Splitter_1_multiple + source_count_name;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            // It is intentionally left blank to execute the else part only for plugins
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
            QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
            QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << QString(complete_file_path + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }
        else
        {
            QString tmp_plg_name = plugin_name_str;
            tmp_plg_name.replace(" ","_");

            QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
            QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";
            if(!QFileInfo(plugin_db_file).exists())
                continue;

            struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

            int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

            if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
                continue;

            QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
            QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

            struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
            bool_is_record_for_artifacts = true;
            QStringList tmp_list = obj.complete_export_filepaths;
            display_file_path_list = obj.displayed_file_path;
            recon_file_info_path_list = obj.recon_file_info_path_list;
            recon_filefrom_list = obj.recon_filefrom_list;

            QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

            for(int j = 0; j < tmp_list.size(); j++)
            {
                all_path_list << QString(tmp_list.at(j) + MACRO_RECON_Splitter_1_multiple + source_count_name);
            }
        }

        for(int j = 0 ; j < all_path_list.size() ; j++)
        {
            QString complete_path_str = all_path_list.at(j);

            QStringList tmp_list = complete_path_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(tmp_list.size() < 2)
                continue;

            if(bool_is_record_for_artifacts)
            {
                if(all_path_list.size() == display_file_path_list.size())
                {
                    st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
                }
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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_sendToBucket_sqlite_viewer_triggered()
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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();
        tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj;
        QStringList display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;
        bool bool_is_record_for_artifacts = false;

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);
            QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << complete_path  + QString(MACRO_RECON_Splitter_1_multiple) + source_count_name;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            //            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();

            //            QString start_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_start_offset_index)->text();
            //            QString end_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_end_offset_index)->text();
            //            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            //            QString record_no_column_name = db_columns_list.at(record_no_index);
            //            QString plugin_column_name = db_columns_list.at(plugin_name_index);

            //            QString table_name = table_name_list.at(0);     // Because Loader supports only one Tab in CSV

            //            QString command_select_tag = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=?";
            //            QStringList arg_list;
            //            arg_list << record_no << plugin_name_str;
            //            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,arg_list,0,destination_db_path,Q_FUNC_INFO);

            //            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

            //            QString command = "SELECT content_file_path from hex_content_info WHERE file_path = ? AND recon_tag_value = ? AND start_index =  ? AND end_index = ?";
            //            QStringList values_list;
            //            values_list << filepath << tag_name << start_offset << end_offset;
            //            QString block_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,values_list,0,exported_content_info_db,Q_FUNC_INFO);

            //            all_path_list << block_file_path + QString(MACRO_RECON_Splitter_1_multiple) + source_count_name;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            // It is intentionally left blank to execute the else part only for plugins
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
            QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
            QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << QString(complete_file_path + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }
        else
        {
            QString tmp_plg_name = plugin_name_str;
            tmp_plg_name.replace(" ","_");

            QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
            QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";
            if(!QFileInfo(plugin_db_file).exists())
                continue;

            tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

            struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

            int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

            if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
                continue;

            QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
            QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
            QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

            struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
            bool_is_record_for_artifacts = true;
            QStringList tmp_list = obj.complete_export_filepaths;
            display_file_path_list = obj.displayed_file_path;
            recon_file_info_path_list = obj.recon_file_info_path_list;
            recon_filefrom_list = obj.recon_filefrom_list;
            for(int j = 0; j < tmp_list.size(); j++)
            {
                all_path_list << QString(tmp_list.at(j) + MACRO_RECON_Splitter_1_multiple + source_count_name);
            }
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

            if(bool_is_record_for_artifacts)
            {
                if(all_path_list.size() == display_file_path_list.size())
                {
                    st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
                }
            }

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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_sendToBucket_plist_viewer_triggered()
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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj;
        QStringList display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;
        bool bool_is_record_for_artifacts = false;

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);
            QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << complete_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            //            QString start_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_start_offset_index)->text();
            //            QString end_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_end_offset_index)->text();
            //            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            //            QString record_no_column_name = db_columns_list.at(record_no_index);
            //            QString plugin_column_name = db_columns_list.at(plugin_name_index);

            //            QString table_name = table_name_list.at(0);     // Because Loader supports only one Tab in CSV

            //            QString command_select_tag = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=?";
            //            QStringList arg_list;
            //            arg_list << record_no << plugin_name_str;
            //            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,arg_list,0,destination_db_path,Q_FUNC_INFO);

            //            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

            //            QString command = "SELECT content_file_path from hex_content_info WHERE file_path = ? AND recon_tag_value = ? AND start_index =  ? AND end_index = ?";
            //            QStringList values_list;
            //            values_list << filepath << tag_name << start_offset << end_offset;
            //            QString block_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,values_list,0,exported_content_info_db,Q_FUNC_INFO);

            //            all_path_list << block_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            // It is intentionally left blank to execute the else part only for plugins
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
            QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
            QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << QString(complete_file_path + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }
        else
        {
            QString tmp_plg_name = plugin_name_str;
            tmp_plg_name.replace(" ","_");

            QString destination_plugin_dir_path = result_dir_path  + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
            QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";
            if(!QFileInfo(plugin_db_file).exists())
                continue ;

            struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

            int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

            if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
                continue;

            QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
            QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

            if(source_count_name.trimmed().isEmpty())
                source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

            struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
            bool_is_record_for_artifacts = true;
            all_path_list = obj.complete_export_filepaths;
            display_file_path_list = obj.displayed_file_path;
            recon_filefrom_list = obj.recon_filefrom_list;
            recon_file_info_path_list = obj.recon_file_info_path_list;
        }

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

            if(bool_is_record_for_artifacts)
            {
                if(all_path_list.size() == display_file_path_list.size())
                {
                    st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
                }
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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    if(!message_list.isEmpty())
    {
        message_dialog_obj->pub_set_multiple_message_list(MACRO_MSG_INFORMATION,message_list);
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_sendToBucket_log_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

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

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();

        struct_global_recon_file_info st_globl_recon_file_info_obj;
        QStringList display_file_path_list;
        QStringList recon_file_info_path_list;
        QStringList recon_filefrom_list;
        bool bool_is_record_for_artifacts = false;

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);
            QString complete_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << complete_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
                || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            // It is intentionally left blank to execute the else part only for plugins
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

            QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
            QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
            QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

            st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
            QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
            all_path_list << QString(complete_file_path + MACRO_RECON_Splitter_1_multiple + source_count_name);
        }
        else
        {
            QString tmp_plg_name = plugin_name_str;
            tmp_plg_name.replace(" ","_");

            QString destination_plugin_dir_path = result_dir_path  + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
            QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";
            if(!QFileInfo(plugin_db_file).exists())
                continue ;

            struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

            int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

            if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
                continue;

            QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
            QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);

            if(source_count_name.trimmed().isEmpty())
                source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);

            struct_global_artifacts_export_fileinfo obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file,export_col_name_list,table_name,record_no,Q_FUNC_INFO);
            bool_is_record_for_artifacts = true;
            all_path_list = obj.complete_export_filepaths;
            display_file_path_list = obj.displayed_file_path;
            recon_filefrom_list = obj.recon_filefrom_list;
            recon_file_info_path_list = obj.recon_file_info_path_list;
        }

        for(int i = 0; i< all_path_list.size(); i++)
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

            if(bool_is_record_for_artifacts)
            {
                if(all_path_list.size() == display_file_path_list.size())
                {
                    st_globl_recon_file_info_obj.display_file_path = display_file_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_info_path = recon_file_info_path_list.at(i);
                    st_globl_recon_file_info_obj.recon_file_from = recon_filefrom_list.at(i);
                }
            }

            QString temp_p = complete_path;
            temp_p = temp_p.toLower();
            if(!temp_p.endsWith(".log"))
            {
                message_list << QString("'") +  info.fileName() + QString("' is not log file.");
                continue;
            }
            else
            {
                QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

                QString src_cnt_name_label = QString(" [" + source_count_name + "]");
                QString log_display_nm = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

                QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(log_viewer_sqlite_path, Q_FUNC_INFO);
                if(list_log_display_name.contains(log_display_nm))
                {
                    continue;
                }

                log_viewer *log_viewer_obj;
                log_viewer_obj = new log_viewer(this);
                QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

                log_viewer_obj->pub_set_result_log_viewer_sqlite_path(log_viewer_sqlite_path);
                log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
                log_viewer_obj->pub_set_log_file_path(complete_path);
                log_viewer_obj->pub_set_source_count_name(source_count_name);
                log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
                log_viewer_obj->pub_set_essentials();

                log_viewer_obj->pub_start_log_extraction();

                emit signal_viewer_extarction_completed_table_result(log_display_nm,MACRO_CASE_TREE_Log_Viewer);
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

void notes_search_loader::action_open_with_log_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(m_tablewidget_obj->currentRow() < 0)
        return;

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString adhoc_log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString complete_log_file_path;
    struct_global_recon_file_info st_globl_recon_file_info_obj;

    if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);
        complete_log_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    {
        // It is intentionally left blank to execute the else part only for plugins
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
        complete_log_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else
    {
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        if(!QFileInfo(plugin_db_file).exists())
            return;

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str,record_no,plugin_db_file);
        complete_log_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }

    QFileInfo info(complete_log_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        emit signal_statusbar_progress_bar_show_hide(false, "");
        return;
    }

    QString temp_p = complete_log_file_path;
    temp_p = temp_p.toLower();
    if(!temp_p.endsWith(".log"))
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "'" +  info.fileName() + "' is not log file.");
        message_dialog_obj->show();
    }
    else
    {
        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

        QString src_cnt_name_label = QString(" [" + source_count_name + "]");
        QString log_file_display_name = display_file_name + " [" + plugin_name_str + " " + record_no + "]" + src_cnt_name_label;

        QStringList list_log_display_name = recon_helper_standard_obj->get_log_displayname_list_from_result_db(adhoc_log_viewer_sqlite_path, Q_FUNC_INFO);
        if(list_log_display_name.contains(log_file_display_name))
        {
            emit signal_show_file_in_viewer_table_result(log_file_display_name, MACRO_CASE_TREE_Log_Viewer, source_count_name);
        }
        else
        {
            log_viewer *log_viewer_obj;
            log_viewer_obj = new log_viewer(this);
            QString log_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + plugin_name_str + MACRO_RECON_Splitter_1_multiple + tab_name_str + MACRO_RECON_Splitter_1_multiple + record_no;

            log_viewer_obj->pub_set_result_log_viewer_sqlite_path(adhoc_log_viewer_sqlite_path);
            log_viewer_obj->pub_set_log_viewer_name(log_view_table_name);
            log_viewer_obj->pub_set_log_file_path(complete_log_file_path);
            log_viewer_obj->pub_set_source_count_name(source_count_name);
            log_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
            log_viewer_obj->pub_set_essentials();

            log_viewer_obj->pub_start_log_extraction();

            emit signal_show_file_in_viewer_table_result(log_file_display_name,MACRO_CASE_TREE_Log_Viewer, source_count_name);
        }
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_open_with_plist_viewer_triggered()
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

    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString complete_file_path;
    struct_global_recon_file_info st_globl_recon_file_info_obj;

    if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        complete_file_path = current_media_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);

        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    {
        // It is intentionally left blank to execute the else part only for plugins
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else
    {
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        if(!QFileInfo(plugin_db_file).exists())
            return ;

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str,record_no,plugin_db_file);
        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }

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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_open_with_hex_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString complete_file_path;
    struct_global_recon_file_info st_globl_recon_file_info_obj;

    if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        complete_file_path = current_media_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);

        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    {
        // It is intentionally left blank to execute the else part only for plugins
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else
    {
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        if(!QFileInfo(plugin_db_file).exists())
            return ;

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str,record_no,plugin_db_file);
        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }

    QFileInfo info(complete_file_path);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" file does not exist path -->>",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"'" +  info.fileName() + "' File not exist.");
        message_dialog_obj->show();
        emit signal_statusbar_progress_bar_show_hide(false, "");
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

        hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        hex_viewer_open_file_data_obj.hex_feature_db_path = hex_feature_db_path;
        hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;
        hex_viewer_open_file_data_obj.complete_file_path  = complete_file_path;
        hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
        hex_viewer_open_file_data_obj.record_no_str       = record_no;
        hex_viewer_open_file_data_obj.source_count_name   = source_count_name;
        hex_viewer_open_file_data_obj.plugin_name_str     = plugin_name_str;
        hex_viewer_open_file_data_obj.plugin_tab_name_str = tab_name_str;
        hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
        hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

        recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);

        emit signal_file_openwith_hex_viewer(hex_viewer_open_file_data_obj);

    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_open_with_sqlite_viewer_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;


    QString source_count_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), source_count_name_index)->text();
    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString complete_file_path;
    struct_global_recon_file_info st_globl_recon_file_info_obj;

    if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        complete_file_path = current_media_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(record_no,fs_db_path,source_count_name);

        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
    {
        // It is intentionally left blank to execute the else part only for plugins
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QString carved_file_label_name = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), item_3_index)->text();

        QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd_2 = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_2,QStringList(carved_file_label_name),0,index_db_path,Q_FUNC_INFO);
        QString carved_files_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(record_no,carved_files_db_path,source_count_name);
        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }
    else
    {
        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";

        if(!QFileInfo(plugin_db_file).exists())
            return ;

        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name_str,tab_name_str,record_no,plugin_db_file);
        complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
    }

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

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_export_triggered()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

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
        QString source_count_name_str = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
        {
            list_paths_to_message_box << export_file_system_record(record_no,selected_row_no);
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer))
        {
            QString start_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_start_offset_index)->text();
            QString end_offset = m_tablewidget_obj->item(selected_row_no, hex_and_text_viewer_end_offset_index)->text();
            QString filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();

            QString record_no_column_name = db_columns_list.at(record_no_index);
            QString plugin_column_name = db_columns_list.at(plugin_name_index);

            QString table_name = table_name_list.at(0);     // Because Loader suppoet only one Tab in CSV

            QString command_select_tag = "SELECT recon_tag_value FROM '" + table_name + "' WHERE " + record_no_column_name + "=? AND " + plugin_column_name + "=?";
            QStringList arg_list;
            arg_list << record_no << plugin_name_str;
            QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select_tag,arg_list,0,destination_db_path,Q_FUNC_INFO);

            QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

            QString command = "SELECT content_file_path from hex_content_info WHERE file_path = ? AND recon_tag_value = ? AND start_index =  ? AND end_index = ?";
            QStringList values_list;
            values_list << filepath << tag_name << start_offset << end_offset;

            QString content_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,values_list,0,exported_content_info_db,Q_FUNC_INFO);
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            if(!content_file_path.isEmpty() && !content_file_path.startsWith(result_dir_path))
                content_file_path.prepend(result_dir_path);

            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(content_file_path,dest_path,QFileInfo(exported_file_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
        {
            QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no, db_path ,"tags",source_count_name_str,Q_FUNC_INFO);

            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
        {
            QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no, db_path ,"saved_graphs",source_count_name_str,Q_FUNC_INFO);
            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
        {
            QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no, db_path ,"saved_graphs",source_count_name_str,Q_FUNC_INFO);
            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
        {
            QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no, db_path ,"tags",source_count_name_str,Q_FUNC_INFO);
            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no, db_path ,"tags",source_count_name_str,Q_FUNC_INFO);
            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer) || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer))
        {
            // It is intentionally left blank to execute the else part only for plugins
        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString relative_filepath = m_tablewidget_obj->item(selected_row_no, file_path_index)->text();
            QString complete_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_filepath , Q_FUNC_INFO);

            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path,dest_path,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
                list_paths_to_message_box << exported_file_path;

        }
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
        {
            QString source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
            QString tab_name_str = m_tablewidget_obj->item(selected_row_no,tab_name_index)->text();
            QString snapshot_label_name = m_tablewidget_obj->item(selected_row_no,item_3_index)->text();

            QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str , tab_name_str);
            QString snapshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_label_name +".sqlite";

            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no,snapshot_db_path,table_name,source_count_name,Q_FUNC_INFO);
            QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path ,dest_path ,QFileInfo(complete_path).fileName() , Q_FUNC_INFO);
            if(!exported_file_path.trimmed().isEmpty())
            {
                list_paths_to_message_box << exported_file_path;
            }

        }
        else
        {
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
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

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
    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"selected records does not have any file to export");
        message_dialog_obj->show();
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

QStringList notes_search_loader::export_file_system_record(QString record_no,int current_row)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QStringList list_paths_to_message_box;

    record_no = record_no.trimmed();

    if(record_no.isEmpty() || current_row < 0)
        return list_paths_to_message_box;

    QString dest_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();

    QString source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();

    QString displayed_file_path = m_tablewidget_obj->item(current_row, file_path_index)->text();

    QString m_file_name = QFileInfo(displayed_file_path).fileName();
    struct_global_export_file_system_record obj;
    obj.record_no = record_no;
    obj.source_count_name = source_count_name;
    obj.display_file_path = displayed_file_path;
    obj.displayed_file_name = m_file_name;


    QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj, Q_FUNC_INFO);
    QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path,dest_path,m_file_name , Q_FUNC_INFO);
    if(!exported_file_path.trimmed().isEmpty())
        list_paths_to_message_box << exported_file_path;

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
    return list_paths_to_message_box;
}

void notes_search_loader::create_action_for_goto_artifact_source_submenu()
{
    if(m_tablewidget_obj->currentRow() < 0 || m_tablewidget_obj->currentRow() >= m_tablewidget_obj->rowCount())
        return;

    QString record_no = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text().trimmed();

    if(plugin_name_str.isEmpty())
        return;

    QString tab_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();
    QString os_schme_dsply = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), os_name_index)->text();

    if(plugin_name_str == QString(MACRO_Plugin_Name_File_System) || plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Screenshots) ||plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline)
            || plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR)
            || plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved) || plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
            || plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps) || plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files)
            || plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password) || plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup) || plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data)
            || plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
        return;
    else
    {
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

        if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon)
                ||artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
        {
            QStringList source_list;
            if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
            else
                source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);


            for(int i = 0; i < source_list.size(); i++)
            {
                QString source_path = source_list.at(i);
                QFileInfo info_file(source_path);

                QAction *act_go_to_actifact = new QAction(QObject::tr(info_file.fileName().toLocal8Bit().data()), this);
                act_go_to_actifact->setToolTip(source_path);
                submenu_go_to_artifact_source->addAction(act_go_to_actifact);
            }
        }
        else
        {
            QFileInfo info_file(artifact_source);

            QAction *act_go_to_actifact = new QAction(QObject::tr(info_file.fileName().toLocal8Bit().data()), this);
            act_go_to_actifact->setToolTip(artifact_source);
            submenu_go_to_artifact_source->addAction(act_go_to_actifact);
        }
    }
}

void notes_search_loader::action_go_to_record_triggered()
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
    QString os_schme_dsply = m_tablewidget_obj->item(current_row, os_name_index)->text();
    QString file_path = m_tablewidget_obj->item(current_row, file_path_index)->text().trimmed();

    if(plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
            )
        return;

    if(plugin_name_str ==  QString(MACRO_Plugin_Name_File_System))
    {
        QString source_count_name = m_tablewidget_obj->item(current_row, source_count_name_index)->text();

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString source_name = struct_info.source_name;
        QString display_name = source_name;
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        emit signal_tablewidget_goto_record(display_name,QString(MACRO_CASE_TREE_Source),file_path,record_no , fs_db_path, os_schme_dsply);
    }
    else if(plugin_name_str ==  QString(MACRO_Plugin_Name_Artifacts_Timeline))
    {

        //artifacts Plugin/Tab name
        plugin_name_str = m_tablewidget_obj->item(current_row, item_0_index)->text().trimmed();
        tab_name_str    = m_tablewidget_obj->item(current_row, item_1_index)->text().trimmed();

        QString artifacts_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";
        QString command_str = QString("Select source_count_name from timeline_data Where record_no = ? AND plugin = ? AND category = ?");
        QStringList value_list;
        value_list << record_no << plugin_name_str << tab_name_str;

        QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, value_list, 0, artifacts_timeline_db_path, Q_FUNC_INFO);
        source_count_name = source_count_name.trimmed();
        if(source_count_name.isEmpty())
            return;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        emit signal_tablewidget_goto_record(plugin_name_str,QString("Parent RECON Plugin"),tab_name_str,record_no , QString(""), struct_info.os_scheme_display);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Saved_Maps,tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Artifacts_Timeline_Graph,tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Screenshots,tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)||plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger) || plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Graph_Result_Saved,tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR) || plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Redefined_Result_Parent,tab_name_str,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        emit signal_tablewidget_goto_record(plugin_name_str,MACRO_CASE_TREE_Carved_Files,m_tablewidget_obj->item(current_row, 9)->text().trimmed(),record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password))
    {
        QString result_tab_name = m_tablewidget_obj->item(current_row,10)->text();
        emit signal_tablewidget_goto_record(plugin_name_str,QString(MACRO_CASE_TREE_Carved_Password),result_tab_name,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data))
    {
        QString tab_name = m_tablewidget_obj->item(current_row,item_11_index)->text();
        emit signal_tablewidget_goto_record(plugin_name_str,QString(MACRO_CASE_TREE_Carved_Data),tab_name,record_no , QString(""), os_schme_dsply);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        QString snap_temperred_name = m_tablewidget_obj->item(current_row,item_3_index)->text();
        emit signal_tablewidget_goto_record_snapshots(tab_name_str,snap_temperred_name, record_no ,QString(MACRO_CASE_TREE_Snapshots));
    }
    else
        emit signal_tablewidget_goto_record(plugin_name_str,QString("Parent RECON Plugin"),tab_name_str,record_no , QString(""), os_schme_dsply);



    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void notes_search_loader::action_remove_note_triggered()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    m_tablewidget_obj->blockSignals(true);


    for(int i = 0; i < selection_model_list.size(); i++)
    {

        if(i % 20 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        QString tab_name_str = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        QString os_name_str = m_tablewidget_obj->item(selected_row_no , os_name_index)->text();

        update_notes_value("",selected_row_no);

    }

    for(int i = (selection_model_list.size() - 1); i >= 0; i--)
    {
        m_tablewidget_obj->removeRow(selection_model_list.at(i).row());
    }

    m_tablewidget_obj->blockSignals(false);


}
