#include "plugin_viewer/plugin_viewer.h"
#include "ui_plugin_viewer.h"


void plugin_viewer::pub_set_global_recon_file_info_obj(global_recon_file_info *obj)
{
    global_recon_file_info_obj = obj;
}

void plugin_viewer::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void plugin_viewer::pub_set_recon_case_obj(RECON_case  *obj)
{
    recon_case_obj = obj;
}



void plugin_viewer::display_hashset_add_files_submenu()
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString command = QString("Select database_path From tbl_hashset");

    QStringList hashset_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, db_path, Q_FUNC_INFO);

    if(hashset_db_path_list.size() == 0)
        return;


    bool bool_recon_hashes_db = false;

    for(int count = 0; count < hashset_db_path_list.size(); count++)
    {
        QString path_str = hashset_db_path_list.at(count);

        QString hash_name_str = QFileInfo(path_str).fileName();
        hash_name_str.remove(QFileInfo(path_str).suffix());

        if(hash_name_str.endsWith("."))
            hash_name_str.chop(1);

        QString software_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select name from software"), 0, path_str, Q_FUNC_INFO);
        software_name = software_name.trimmed();
        if(software_name == QString("RECON"))
        {
            if(!bool_recon_hashes_db)
                menu_hashset_add_files->clear();

            bool_recon_hashes_db = true;
            QAction *action_add_file = new QAction(hash_name_str, menu_hashset_add_files);
            action_add_file->setToolTip(path_str);
            menu_hashset_add_files->addAction(action_add_file);
        }

    }

    hashset_storage_obj->pub_set_essentials();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);

}


void plugin_viewer::set_destination_location()
{

    detached_metadata_viewer_module_popobj->set_window_title(get_plugin_name());
    detached_file_previewer_module_popobj->set_window_title(get_plugin_name());

    add_tags_obj->pub_set_essentials();

    reports_location_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Plugin_In_Result_QString).toString();

    QString temp_path_output;
    //    if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_File_System)
    //        temp_path_output = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Parent_In_Result_QString).toString() + get_plugin_name().replace(" ","_") + "/Source1/";
    if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Hex_Viewer)
        temp_path_output = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + get_plugin_name().replace(" ","_");
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Plist_Viewer)
        temp_path_output = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + get_plugin_name().replace(" ","_");
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Sqlite_Viewer)
        temp_path_output = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + get_plugin_name().replace(" ","_");
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Log_Viewer)
        temp_path_output = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + get_plugin_name().replace(" ","_");
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Screenshots)
    {
        temp_path_output = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString();
        temp_path_output.chop(1); // choping '/'
    }
    else
        temp_path_output = recon_result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + get_plugin_name().replace(" ","_");

    destination_file_location = temp_path_output;

    path_manually_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Plugin_QString).toString();

    QDir dir;
    dir.mkpath(path_manually_export);

    if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Hex_Viewer)
        destination_db_file = destination_file_location + "/hex_viewer_content_info.sqlite";
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Plist_Viewer)
        destination_db_file = destination_file_location + "/plist_viewer_content_info.sqlite";
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Sqlite_Viewer)
        destination_db_file = destination_file_location + "/sqlite_viewer_content_info.sqlite";
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Log_Viewer)
        destination_db_file = destination_file_location + "/log_viewer_content_info.sqlite";
    else if(received_os_naming_scheme_for_feature_only == MACRO_Plugin_Name_Screenshots)
        destination_db_file = destination_file_location + "/screenshots.sqlite";
    else
        destination_db_file = destination_file_location + "/" + get_plugin_name().replace(" ","_") + ".sqlite";

    detach_mdls_dialog_module_popobj->set_db_file_loaction(destination_db_file);
    detach_mdls_dialog_module_popobj->set_window_title(get_plugin_name());

}

void plugin_viewer::pub_display_data()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    remove_extra_widgets();

    recon_static_functions::app_debug(" initialize array  start, plugin name - " + get_plugin_name(),Q_FUNC_INFO);
    initialize_tablewidget_array();
    initialize_textedit_array();
    initialize_stack_array();
    initialize_tree_array();
    initialise_chat_viewer_array();
    initialize_thumbnail_viewer_array();
    recon_static_functions::app_debug(" initialize array  end, plugin name - " + get_plugin_name(),Q_FUNC_INFO);

    if(ui->tabWidget->count() >= 1)
        ui->tabWidget->setCurrentIndex(0);

    recon_static_functions::app_debug(" draw header  start, plugin name - " + get_plugin_name(),Q_FUNC_INFO);
    draw_headers_tablewidget();
    draw_headers_tree();
    recon_static_functions::app_debug(" draw header  end, plugin name - " + get_plugin_name(),Q_FUNC_INFO);

    //-------------------------------------------------------------------------------------------------------------------------//

    recon_static_functions::app_debug(" populate data  start, plugin name - " + get_plugin_name(),Q_FUNC_INFO);
    populate_data_in_tablewidgets();
    populate_data_in_textedit();
    populate_data_in_stack();
    populate_data_in_tree();
    populate_data_in_thumbnail_viewer();
    populate_data_in_chat_viewer();


    recon_static_functions::app_debug(" populate data  end, plugin name - " + get_plugin_name(),Q_FUNC_INFO);

    //-------------------------------------------------------------------------------------------------------------------------//



    recon_static_functions::app_debug(" - end",Q_FUNC_INFO);

}


void plugin_viewer::slot_goto_record(QString tab_name, QString rec)
{

    for(int ii = 0; ii < ui->tabWidget->count(); ii++)
    {
        QString tb_name = get_exact_tab_name_from_ui(ii);
        if(tab_name.trimmed() == tb_name)
        {
            ui->tabWidget->setCurrentIndex(ii);


            m_tablewidget *my_table = list_tablewidgets.at(ii);

            for(int pp = 0; pp < my_table->rowCount(); pp++)
            {
                if(rec == my_table->item(pp,2)->text().trimmed())
                {
                    //                    my_table->setAutoScroll(true);

                    my_table->setCurrentCell(pp, 0);
                    tablewidget_general_clicked(ii,pp,2);
                    my_table->scrollToItem(my_table->item(pp,2));

                    break;
                }

            }
            break;
        }
    }

}

struct_global_advance_report_plugins_tab plugin_viewer::pub_get_tabs_info(QString report_scope,QStringList selected_tags_list)
{
    struct_global_advance_report_plugins_tab obj_strc;

    //    html_adv_report_obj->set_plugins_tab_info(list_all_plugins_tabs_info);
    //    obj_strc = html_adv_report_obj->get_tabs_info(report_scope,selected_tags_list);

    return obj_strc;
}

void plugin_viewer::pub_set_all_plugins_tabs_info(QList<struct_global_advance_report_plugins_tab> list_strct)
{
    list_all_plugins_tabs_info = list_strct;
}


void plugin_viewer::pub_set_plugin_name(QString name)
{
    module_plugin_name = name;
}

void plugin_viewer::pub_set_display_default_tags()
{
    display_default_tags();

}

void plugin_viewer::pub_set_essentials()
{
    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    forward_set_essentials();

    clear_csv_lists();

    ui->checkBox_export->setEnabled(false);
    ui->checkBox_timeline->setEnabled(false);


    debug_file_path = recon_static_functions::get_debug_filepath_regular();
    recon_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    set_destination_location();




    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    received_date_format_from_pub = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt();

    examiner_time_offset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();
    examiner_time_zone = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_QString).toString();
    case_created_machine_offset = global_recon_helper_singular_class_obj->struct_default_timezone_details_obj.d_case_created_machine_timezn_numeric_qint64;


    struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_selected_obj->read_csv_value(get_plugin_name());

    native_os_list = obj.plugin_native_os_QStringList;
    supported_os_scheme_list = obj.plugin_supported_os_QStringList;
    parent_module_name = obj.plugin_parent_name;

    total_tabs_list = obj.overall_tab_name_QStringList;

    table_name_list_tablewidget  = obj.table_name_QStringList_tablewidget;
    tab_name_list_tablewidget  = obj.tab_name_QStringList_tablewidget;
    table_apple_metadata_available_list_tablewidget  = obj.tab_apple_metadata_available_QStringList_tablewidget;
    table_html_report_available_list_tablewidget  = obj.tab_html_report_available_QStringList_tablewidget;
    table_xml_report_available_list_tablewidget  = obj.tab_xml_report_available_QStringList_tablewidget;
    table_csv_report_available_list_tablewidget  = obj.tab_csv_report_available_QStringList_tablewidget;
    table_kml_report_available_list_tablewidget  = obj.tab_kml_report_available_QStringList_tablewidget;
    table_export_available_list_tablewidget  = obj.tab_export_available_QStringList_tablewidget;
    table_g_search_available_list_tablewidget  = obj.tab_g_search_available_QStringList_tablewidget;
    table_timeline_available_list_tablewidget  = obj.tab_timeline_available_QStringList_tablewidget;

    tab_name_list_textedit  = obj.tab_name_QStringList_textedit;
    table_name_list_textedit  = obj.table_name_QStringList_textedit;
    html_report_available_list_textedit  = obj.tab_html_report_available_QStringList_textedit;
    xml_report_available_list_textedit  = obj.tab_xml_report_available_QStringList_textedit;
    csv_report_available_list_textedit  = obj.tab_csv_report_available_QStringList_textedit;

    tab_name_list_stack  = obj.tab_name_QStringList_stack;
    table_name_list_stack  = obj.table_name_QStringList_stack;
    html_report_available_list_stack  = obj.tab_html_report_available_QStringList_stack;
    xml_report_available_list_stack  = obj.tab_xml_report_available_QStringList_stack;
    csv_report_available_list_stack  = obj.tab_csv_report_available_QStringList_stack;

    table_name_list_tree  = obj.table_name_QStringList_tree;
    tab_name_list_tree  = obj.tab_name_QStringList_tree;

    table_name_list_thumbnail  = obj.table_name_QStringList_thumbnail;
    tab_name_list_thumbnail  = obj.tab_name_QStringList_thumbnail;

    table_name_list_chat_viewer  = obj.table_name_QStringList_chat_viewer;
    tab_name_list_chat_viewer  = obj.tab_name_QStringList_chat_viewer;


    tw_header_list_tablewidget  = obj.display_header_QList_tablewidget;
    tw_column_name_list_tablewidget  = obj.display_column_name_QList_tablewidget;
    tw_data_type_list_tablewidget  = obj.display_data_type_QList_tablewidget;
    tw_col_width_list_tablewidget  = obj.display_col_width_QList_tablewidget;
    tw_col_order_list_tablewidget = obj.display_order_QList_tablewidget;
    ms_search_local_list_tablewidget  = obj.local_search_column_name_QList_tablewidget;

    column_name_list_textedit  = obj.display_column_name_QList_textedit;
    data_type_list_textedit  = obj.display_data_type_QList_textedit;

    column_name_list_stack  = obj.display_column_name_QList_stack;
    data_type_list_stack  = obj.display_data_type_QList_stack;

    header_list_tree  = obj.display_header_QList_tree;
    parent_child_list_tree  = obj.display_parent_child_QList_tree;
    column_name_list_tree  = obj.display_column_name_QList_tree;
    data_type_list_tree  = obj.display_data_type_QList_tree;
    col_width_list_tree  = obj.display_col_width_QList_tree;


    display_column_name_list_chat_viewer  = obj.display_column_name_QList_chat_viewer;
    display_data_type_list_chat_viewer  = obj.display_data_type_QList_chat_viewer;

    md_header_list_tablewidget  = obj.metadata_header_QList_tablewidget;
    md_column_name_list_tablewidget  = obj.metadata_column_name_QList_tablewidget;
    md_data_type_list_tablewidget  = obj.metadata_data_type_QList_tablewidget;
    md_preview_list_tablewidget  = obj.metadata_preview_QList_tablewidget;

    export_path_column_list_tablewidget  = obj.export_path_column_name_QList_tablewidget;

    md_header_list_tree  = obj.metadata_header_QList_tree;
    md_column_name_list_tree  = obj.metadata_column_name_QList_tree;
    md_data_type_list_tree  = obj.metadata_data_type_QList_tree;
    md_preview_list_tree  = obj.metadata_preview_QList_tree;





    g_search_col_name_list  = obj.g_search_col_name_QList;
    g_search_col_order_list  = obj.g_search_col_order_QList;
    g_search_col_data_type_list  = obj.g_search_col_data_type_QList;

    tm_col_name_list  = obj.g_timeline_col_name_QList;
    tm_col_data_type_list  = obj.g_timeline_col_data_type_QList;


    for(int pp = 0; pp < table_export_available_list_tablewidget.size(); pp++)
    {
        if(table_export_available_list_tablewidget.at(pp).trimmed() == QString("1"))
        {
            ui->checkBox_export->setEnabled(true);
            break;
        }
    }

    for(int pp = 0; pp < table_timeline_available_list_tablewidget.size(); pp++)
    {
        if(table_timeline_available_list_tablewidget.at(pp).trimmed() == QString("1"))
        {
            ui->checkBox_timeline->setEnabled(true);
            break;
        }
    }


    recon_static_functions::app_debug(" end", Q_FUNC_INFO);

}

void plugin_viewer::clear_csv_lists()
{

    table_name_list_tablewidget.clear();
    tab_name_list_tablewidget.clear();
    table_apple_metadata_available_list_tablewidget.clear();
    table_html_report_available_list_tablewidget.clear();
    table_xml_report_available_list_tablewidget.clear();
    table_csv_report_available_list_tablewidget.clear();
    table_kml_report_available_list_tablewidget.clear();
    table_export_available_list_tablewidget.clear();
    table_timeline_available_list_tablewidget.clear();

    tab_name_list_textedit.clear();
    table_name_list_textedit.clear();
    html_report_available_list_textedit.clear();
    xml_report_available_list_textedit.clear();
    csv_report_available_list_textedit.clear();

    tab_name_list_stack.clear();
    table_name_list_stack.clear();
    html_report_available_list_stack.clear();
    xml_report_available_list_stack.clear();
    csv_report_available_list_stack.clear();

    table_name_list_tree.clear();
    tab_name_list_tree.clear();

    table_name_list_thumbnail.clear();
    tab_name_list_thumbnail.clear();

    tw_header_list_tablewidget.clear();
    tw_column_name_list_tablewidget.clear();
    tw_data_type_list_tablewidget.clear();
    tw_col_width_list_tablewidget.clear();
    tw_col_order_list_tablewidget.clear();
    ms_search_local_list_tablewidget.clear();

    column_name_list_textedit.clear();
    data_type_list_textedit.clear();

    column_name_list_stack.clear();
    data_type_list_stack.clear();

    header_list_tree.clear();
    parent_child_list_tree.clear();
    column_name_list_tree.clear();
    data_type_list_tree.clear();
    col_width_list_tree.clear();

    md_header_list_tablewidget.clear();
    md_column_name_list_tablewidget.clear();
    md_data_type_list_tablewidget.clear();
    md_preview_list_tablewidget.clear();
    export_path_column_list_tablewidget.clear();

    md_header_list_tree.clear();
    md_column_name_list_tree.clear();
    md_data_type_list_tree.clear();
    md_preview_list_tree.clear();


    tm_col_name_list.clear();
    tm_col_data_type_list.clear();

}

void plugin_viewer::forward_set_essentials()
{
    hashset_storage_obj->pub_set_essentials();
}
