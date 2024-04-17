#include "thread_report_builder.h"

thread_report_builder::thread_report_builder(QWidget *parent)
{

    recon_helper_standard_obj = new recon_helper_standard(this);

    report_html_template_plugin_advance_obj = new report_template_html_advance(this);

    report_generic_function_obj = new report_generic_function(this);

    html_standard_report_module_obj = new html_standard_report_module(this);
    pdf_report_module_obj = new pdf_report_module(this);
    connect(this, SIGNAL(signal_create_pdf_report(QString,QString)), pdf_report_module_obj, SIGNAL(signal_create_pdf(QString,QString)));
    connect(pdf_report_module_obj, SIGNAL(signal_pdf_finished(QString,bool)), this, SLOT(slot_pdf_report_finished(QString, bool)));
    csv_report_module_obj = new csv_report_module(this);
    xml_report_module_obj = new xml_report_module(this);
    kml_report_module_obj = new kml_report_module(this);
    html_advance_report_module_obj = new html_advance_report_module(this);

    bool_cancel_report = false;
    bool_pdf_report_finished = false;

}

thread_report_builder::~thread_report_builder()
{

}


void thread_report_builder::run()
{
    recon_static_functions::app_debug(" - start",Q_FUNC_INFO);

    for(int kk = 0; kk < report_type_list.size(); kk++)
    {
        if(bool_cancel_report)
            break;

        report_filepath = report_path_list.at(kk);
        m_report_type = report_type_list.at(kk);


        if(report_filepath.trimmed().isEmpty())
            continue;


        QFileInfo info(report_filepath);
        report_dir = info.absoluteDir().absolutePath();

        selected_plugins_info = get_plugins_info_list_after_trimming_tabs_having_no_data();

        create_report_header();

        for(int i = 0; i < selected_plugins_info.size(); i++)
        {
            if(bool_cancel_report)
                break;

            QString value_str = selected_plugins_info.at(i);

            QStringList tmp_list = value_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            if(tmp_list.size() < 2)
                continue;

            QString plugin_name = tmp_list.at(0);
            QStringList tabs_list = recon_static_functions::get_associated_tabs(plugin_name,QStringList(value_str),Q_FUNC_INFO);
            QStringList exp_info_list;
            for(int pp = 0; pp < tabs_list.size(); pp++)
            {
                if(recon_static_functions::get_associated_tabs_with_export(plugin_name, tabs_list.at(pp), QStringList(value_str), Q_FUNC_INFO))
                    exp_info_list <<  "1";
                else
                    exp_info_list <<  "0";
            }


            recon_static_functions::app_debug(" Report Builder selected_tablist " + tabs_list.join("----"), Q_FUNC_INFO);
            recon_static_functions::app_debug(" Report Builder tabs_export_status_list " + exp_info_list.join("----"), Q_FUNC_INFO);
            recon_static_functions::app_debug(" Report Builder report_file_path " + report_filepath, Q_FUNC_INFO);
            recon_static_functions::app_debug(" Report Builder report_type " + m_report_type, Q_FUNC_INFO);
            recon_static_functions::app_debug(" Report Builder report_scope " + m_report_scope, Q_FUNC_INFO);
            recon_static_functions::app_debug(" Report Builder tags_list " + selected_tags_list.join("----"), Q_FUNC_INFO);


            extract_csv_values(plugin_name);
            update_narad_muni(plugin_name);

            if(m_report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString)
            {
                html_standard_report_module_obj->set_essentials();

                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                    html_standard_report_module_obj->create_full_report_standard_html(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                    html_standard_report_module_obj->create_tag_report_standard_html(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
                    html_standard_report_module_obj->create_screen_report_standard_html(report_filepath,tabs_list, exp_info_list);

                html_standard_report_module_obj->clear_exportfilelist_table();
            }
            else if(m_report_type == MACRO_REPORT_TYPE_HTML_ADVANCE_QString)
            {
                html_advance_report_module_obj->set_essentials();

                html_advance_report_module_obj->pub_set_plugins_and_tabs_info(selected_plugins_info);

                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                    html_advance_report_module_obj->create_full_report_advanced_html(report_filepath,tabs_list,exp_info_list,selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                    html_advance_report_module_obj->create_tag_report_advanced_html(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
                    html_advance_report_module_obj->create_screen_report_advanced_html(report_filepath,tabs_list, exp_info_list);

                html_advance_report_module_obj->clear_exportfilelist_table();
            }
            else if(m_report_type == MACRO_REPORT_TYPE_PDF_QString)
            {
                pdf_report_module_obj->set_essentials();

                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                    pdf_report_module_obj->create_full_report_pdf(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                    pdf_report_module_obj->create_tag_report_pdf(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
                    pdf_report_module_obj->create_screen_report_pdf(report_filepath,tabs_list, exp_info_list);

                pdf_report_module_obj->clear_exportfilelist_table();
            }
            else if(m_report_type == MACRO_REPORT_TYPE_CSV_QString)
            {
                if(!report_generic_function_obj->pub_is_tagtype_other_than_screenshot_or_saved_graph(plugin_name) || plugin_name == MACRO_Plugin_Name_Saved_Maps)
                    continue ;

                csv_report_module_obj->set_essentials();

                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                    csv_report_module_obj->create_full_report_csv(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                    csv_report_module_obj->create_tag_report_csv(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
                    csv_report_module_obj->create_screen_report_csv(report_filepath,tabs_list, exp_info_list);

                csv_report_module_obj->clear_exportfilelist_table();
            }
            else if(m_report_type == MACRO_REPORT_TYPE_XML_QString)
            {
                if(!report_generic_function_obj->pub_is_tagtype_other_than_screenshot_or_saved_graph(plugin_name) || plugin_name == MACRO_Plugin_Name_Saved_Maps)
                    continue ;

                xml_report_module_obj->set_essentials();

                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                    xml_report_module_obj->create_full_report_xml(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                    xml_report_module_obj->create_tag_report_xml(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
                    xml_report_module_obj->create_screen_report_xml(report_filepath,tabs_list, exp_info_list);

                xml_report_module_obj->clear_exportfilelist_table();
            }
            else if(m_report_type == MACRO_REPORT_TYPE_KML_QString)
            {
                kml_report_module_obj->set_essentials();

                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                    kml_report_module_obj->create_full_report_kml(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                    kml_report_module_obj->create_tag_report_kml(report_filepath,tabs_list, exp_info_list, selected_tags_list);
                else if(m_report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
                    kml_report_module_obj->create_screen_report_kml(report_filepath,tabs_list, exp_info_list);

                kml_report_module_obj->clear_exportfilelist_table();
            }

        } // for loop plugins


        if(m_report_type == MACRO_REPORT_TYPE_PDF_QString)
        {
            //Need to run executable only once if we have selected more than one artifact/taged items...
            //Thats why run executable outside the loop....

            pdf_report_module_obj->pub_convert_html_to_pdf(report_filepath);

            while(1)
            {
                QCoreApplication::processEvents();

                if(bool_pdf_report_finished || bool_cancel_report)
                    break;
            }
        }
        if(m_report_type == MACRO_REPORT_TYPE_XML_QString)
        {
            QFile file_xml(report_filepath);
            if(!file_xml.open(QIODevice::Append))
            {
                recon_static_functions::app_debug("File Open ------FAILED----- " + file_xml.fileName() ,Q_FUNC_INFO);
                recon_static_functions::app_debug("error  " + file_xml.errorString(),Q_FUNC_INFO);
                continue;
            }

            file_xml.write("\n</RECON_Report>");
            file_xml.flush();
            file_xml.close();
        }

    } // for loop reports



    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);
}


void thread_report_builder::fill_variable_from_narad()
{
    app_version         = narad_muni_for_report_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString();

    case_no             = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_ID_QString).toString();
    case_name           = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    examiner            = narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString();
    examiner_phone      = narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString();
    examiner_email      = narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString();
    agency              = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString();
    agency_address      = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString();
    location            = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Location_QString).toString();
    notes               = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString();
    logo_path           = narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString();
    examiner_selected_timezone_string      = narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_QString).toString();

}


QString thread_report_builder::pub_get_report_name()
{
    return report_name;
}


void thread_report_builder::pub_cancel_report()
{
    bool_cancel_report = true;

    html_advance_report_module_obj->pub_cancel_report();
    html_standard_report_module_obj->pub_cancel_report();
    csv_report_module_obj->pub_cancel_report();
    xml_report_module_obj->pub_cancel_report();
    pdf_report_module_obj->pub_cancel_report();
}

QString thread_report_builder::pub_get_report_password()
{
    return narad_muni_for_report_obj->get_field(MACRO_NARAD_Global_Report_Password_QString).toString();
}

void thread_report_builder::slot_pdf_report_finished(QString html_pah, bool m_srtatus)
{
    bool_pdf_report_finished = true;
}

void thread_report_builder::pub_set_essentials(struct_global_global_report_info obj)
{
    bool_cancel_report = false;
    bool_pdf_report_finished = false;

    m_report_maker = obj.report_maker;
    report_path_list = obj.report_path_list;

    selected_plugins_info = obj.report_plugin_info_list;


    report_name = obj.report_name;

    report_type_list = obj.report_type_list;
    m_report_scope = obj.report_scope;

    //======================================//
    narad_muni_for_report_obj  = obj.narad_muni_report_obj;
    report_generic_function_obj->pub_set_narad_muni_for_report(narad_muni_for_report_obj);
    html_standard_report_module_obj->set_narad_muni_for_report(narad_muni_for_report_obj);
    html_advance_report_module_obj->set_narad_muni_for_report(narad_muni_for_report_obj);
    pdf_report_module_obj->set_narad_muni_for_report(narad_muni_for_report_obj);
    csv_report_module_obj->set_narad_muni_for_report(narad_muni_for_report_obj);
    xml_report_module_obj->set_narad_muni_for_report(narad_muni_for_report_obj);
    kml_report_module_obj->set_narad_muni_for_report(narad_muni_for_report_obj);
    //======================================//


    fill_variable_from_narad();

    selected_tags_list = obj.selected_tags_list;

    recon_result_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

}

QStringList thread_report_builder::pub_get_report_filepath_list()
{
    return report_path_list;
}

void thread_report_builder::extract_csv_values(QString plgn_name)
{

    struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_selected_obj->read_csv_value(plgn_name);


    tab_name_list_textedit  = obj.tab_name_QStringList_textedit;

    tab_name_list_stack  = obj.tab_name_QStringList_stack;
    table_artifact_common_list_tablewidget  = obj.tab_artifact_common_QStringList_tablewidget;
    table_report_html_header_list_tablewidget  = obj.tab_report_html_header_QStringList_tablewidget;

    table_report_header_column_width_html_list_tablewidget  = obj.tab_report_header_column_width_html_QStringList_tablewidget;
    table_export_criteria_list_tablewidget  = obj.tab_export_criteria_QStringList_tablewidget;
    artifact_common_list_textedit  = obj.tab_artifact_common_QStringList_textedit;

    report_html_header_list_textedit  = obj.tab_report_html_header_QStringList_textedit;
    report_header_column_width_html_list_textedit  = obj.tab_report_header_column_width_html_QStringList_textedit;

    artifact_common_list_stack  = obj.tab_artifact_common_QStringList_stack;

    report_html_header_list_stack  = obj.tab_report_html_header_QStringList_stack;
    report_header_column_width_html_list_stack  = obj.tab_report_header_column_width_html_QStringList_stack;


    report_html_col_name_list_textedit  = obj.report_html_col_name_QList_textedit;

    rt_html_data_type_list_tablewidget  = obj.report_html_data_type_QList_tablewidget;

    rt_html_col_name_list_tablewidget  = obj.report_html_col_name_QList_tablewidget;

    report_html_datatype_list_textedit  = obj.report_html_datatype_QList_textedit;

    report_html_col_name_list_stack  = obj.report_html_col_name_QList_stack;

    report_html_datatype_list_stack  = obj.report_html_datatype_QList_stack;
    rt_html_d_name_list_tablewidget  = obj.report_html_display_name_QList_tablewidget;

    rt_html_order_list_tablewidget  = obj.report_html_order_QList_tablewidget;

    rt_csv_data_type_list  = obj.report_csv_data_type_QList_tablewidget;

    rt_csv_col_name_list  = obj.report_csv_col_name_QList_tablewidget;
    rt_csv_d_name_list  = obj.report_csv_display_name_QList_tablewidget;

    rt_csv_order_list  = obj.report_csv_order_QList_tablewidget;

    rt_xml_d_name_list  = obj.report_xml_display_name_QList_tablewidget;

    rt_xml_order_list  = obj.report_xml_order_QList_tablewidget;

    rt_xml_data_type_list  = obj.report_xml_data_type_QList_tablewidget;

    rt_xml_col_name_list  = obj.report_xml_col_name_QList_tablewidget;

    rt_kml_data_type_list  = obj.report_kml_data_type_QList_tablewidget;

    rt_kml_col_name_list  = obj.report_kml_col_name_QList_tablewidget;

    rt_kml_d_name_list  = obj.report_kml_display_name_QList_tablewidget;

    rt_kml_order_list  = obj.report_kml_order_QList_tablewidget;



    table_export_available_list_tablewidget  = obj.tab_export_available_QStringList_tablewidget;

    table_name_list_tablewidget  = obj.table_name_QStringList_tablewidget;

    table_html_report_available_list_tablewidget  = obj.tab_html_report_available_QStringList_tablewidget;

    tab_name_list_tablewidget  = obj.tab_name_QStringList_tablewidget;
    table_name_list_textedit  = obj.table_name_QStringList_textedit;
    html_report_available_list_textedit  = obj.tab_html_report_available_QStringList_textedit;
    table_name_list_stack  = obj.table_name_QStringList_stack;

    html_report_available_list_stack  = obj.tab_html_report_available_QStringList_stack;

    table_csv_report_available_list_tablewidget  = obj.tab_csv_report_available_QStringList_tablewidget;

    table_xml_report_available_list_tablewidget  = obj.tab_xml_report_available_QStringList_tablewidget;

    table_kml_report_available_list_tablewidget  = obj.tab_kml_report_available_QStringList_tablewidget;

    export_path_column_list_tablewidget  = obj.export_path_column_name_QList_tablewidget;
    tw_data_type_list_tablewidget  = obj.display_data_type_QList_tablewidget;





}

QStringList thread_report_builder::get_plugins_info_list_after_trimming_tabs_having_no_data()
{
    QStringList trimmed_plugin_info_list;
    for(int i = 0 ; i < selected_plugins_info.size() ; i++)
    {
        QString value_str = selected_plugins_info.at(i);
        QStringList plugin_and_tab_list = QStringList(value_str);

        for(int itr=0; itr < plugin_and_tab_list.size(); itr++)
        {
            QString temp_str = plugin_and_tab_list.at(itr);
            QStringList plugin_and_tab_nm_list_holder = temp_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
            if(plugin_and_tab_nm_list_holder.size() < 2)
                continue;


            QString plugin_name = plugin_and_tab_nm_list_holder.at(0);
            temp_str = plugin_and_tab_nm_list_holder.at(1);


            extract_csv_values(plugin_name);
            update_narad_muni(plugin_name);

            report_generic_function_obj->pub_set_essentials();

            QStringList all_tabs_info = temp_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

            QStringList tab_list;
            for(int tt = 0; tt < all_tabs_info.size(); tt++)
            {
                QString exp_info = all_tabs_info.at(tt);
                QString tab_with_data = exp_info;
                tab_with_data = tab_with_data.trimmed();
                if(tab_with_data.contains(MACRO_RECON_Splitter_3_dollar_in_brace))
                {
                    tab_with_data.remove(tab_with_data.indexOf(MACRO_RECON_Splitter_3_dollar_in_brace),tab_with_data.size());
                }

                if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    if(global_csv_reader_class_selected_obj->is_tab_type_chat_view(plugin_name,tab_with_data))
                    {
                        if(!selected_tags_list.contains(MACRO_Tag_Name_Bookmarks))
                            continue ;

                        QString chat_viewer_bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
                        qint64 total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true ,chat_viewer_bookmark_db_path ,QStringList("") ,Q_FUNC_INFO);

                        if(total_record_count > 0)
                            tab_list << exp_info;

                        continue;
                    }
                }

                QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name,tab_with_data);


                if(tab_with_data == MACRO_Tag_Name_Examiner_Comments)
                {
                    // For "Examiner Comments" we haven't created db.
                    // So no need to get record count.
                }
                else
                {
                    qint64 record_count = report_generic_function_obj->pub_get_total_record_of_table(table_name,m_report_scope,selected_tags_list);
                    if(record_count <= 0)
                        continue ;
                }

                tab_list << exp_info;
            }

            if(tab_list.size() >= 1)
            {
                trimmed_plugin_info_list << plugin_name + MACRO_RECON_Splitter_1_multiple + tab_list.join(MACRO_RECON_Splitter_2_hash_in_brace);
            }
        }
    }

    return trimmed_plugin_info_list;
}

void thread_report_builder::update_narad_muni(QString plgn_name)
{

    QString m_plugin_name = plgn_name;

    QString destination_file_location;
    if(m_plugin_name == MACRO_Plugin_Name_File_System)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Parent_In_Result_QString).toString() + plgn_name.replace(" ","_");
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Hex_Viewer)
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + plgn_name.replace(" ","_");
    else if(m_plugin_name == MACRO_Plugin_Name_Plist_Viewer)
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + plgn_name.replace(" ","_");
    else if(m_plugin_name == MACRO_Plugin_Name_Sqlite_Viewer)
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + plgn_name.replace(" ","_");
    else if(m_plugin_name == MACRO_Plugin_Name_Log_Viewer)
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + plgn_name.replace(" ","_");
    else if(m_plugin_name == MACRO_Plugin_Name_Unified_Logs)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Screenshots)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Messenger_Full_RR || m_plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR )
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Ram_Analysis_Saved)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Email_Parser)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Artifacts_Timeline)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Saved_Maps)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Strings_Viewer)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Text_Viewer)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Email_Parser_Tag)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Carved_Files)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Carved_Password)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Registry_Viewer)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Mobile_Backup)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Carved_Data)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Snapshots)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString();
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Examiner_Space)
    {
        destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString();
    }
    else
        destination_file_location = recon_result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plgn_name.replace(" ","_");

    if(!destination_file_location.endsWith("/"))
        destination_file_location.append("/");


    QStringList destination_db_file_list;
    if(m_plugin_name == MACRO_Plugin_Name_File_System)
    {
        QList<struct_GLOBAL_witness_info_source> list_target_source_info;
        list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
        for(int i = 0 ; i < list_target_source_info.size() ; i++)
        {
            destination_db_file_list << destination_file_location + list_target_source_info.at(i).source_count_name + "/file_system.sqlite";
        }
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Hex_Viewer)
        destination_db_file_list << destination_file_location + "hex_viewer_content_info.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Plist_Viewer)
        destination_db_file_list << destination_file_location + "plist_viewer_content_info.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Sqlite_Viewer)
        destination_db_file_list << destination_file_location + "sqlite_viewer_content_info.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Log_Viewer)
        destination_db_file_list << destination_file_location + "log_viewer_content_info.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Unified_Logs)
    {
        QString index_db_path = destination_file_location + "index_unified_logs.sqlite";
        QString cmd = "Select db_name from tbl_index";
        QStringList relative_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd,0,index_db_path,Q_FUNC_INFO);

        for(int ii = 0 ; ii < relative_db_path_list.size() ;ii++)
        {
            destination_db_file_list << global_recon_helper_singular_class_obj->get_complete_file_path(destination_file_location,relative_db_path_list.at(ii), Q_FUNC_INFO);
        }
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Screenshots)
        destination_db_file_list << destination_file_location + "screenshots.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        destination_db_file_list << destination_file_location + "Messenger.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        destination_db_file_list << destination_file_location + "Browser_History.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Artifacts_Timeline)
    {
        destination_db_file_list << destination_file_location + "artifacts_timeline_full.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
        destination_db_file_list << destination_file_location + "Browser_History.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
        destination_db_file_list << destination_file_location + "Messenger.sqlite";
    else if(m_plugin_name == MACRO_Plugin_Name_Ram_Analysis_Saved)
    {
        destination_db_file_list << destination_file_location + "ram_analysis_saved.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Email_Parser)
    {
        destination_db_file_list << destination_file_location + "email_data.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
    {
        destination_db_file_list << destination_file_location + "artifacts_timeline_saved_graphs.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Saved_Maps)
    {
        destination_db_file_list << destination_file_location + "saved_maps.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Strings_Viewer)
    {
        destination_db_file_list << destination_file_location + "string_viewer_content_info.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Text_Viewer)
    {
        destination_db_file_list << destination_file_location + "text_viewer_content_info.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Email_Parser_Tag)
    {
        destination_db_file_list << destination_file_location + "email_tag_data.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Carved_Files)
    {
        QString index_db_path = destination_file_location + "carved_files_index.sqlite";
        QString cmd = "Select carver_files_db_path from table_carver_index";
        QStringList relative_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd,0,index_db_path,Q_FUNC_INFO);

        QString result_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        for(int ii = 0 ; ii < relative_db_path_list.size() ;ii++)
        {
            destination_db_file_list << global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path_list.at(ii), Q_FUNC_INFO);
        }
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Carved_Password)
    {
        QString index_db_path = destination_file_location + "carved_password_index.sqlite";
        QString cmd = "Select relative_db_path from tbl_carved_password_index";
        QStringList relative_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd,0,index_db_path,Q_FUNC_INFO);
        QString result_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        for(int ii = 0 ; ii < relative_db_path_list.size() ; ii++)
        {
            destination_db_file_list << global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path_list.at(ii) , Q_FUNC_INFO);
        }
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Registry_Viewer)
    {
        QString index_db_path = destination_file_location + "registry_viewer_index.sqlite";
        QString cmd = "Select relative_db_path from tbl_registry_viewer_index";
        QStringList relative_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd,0,index_db_path,Q_FUNC_INFO);
        QString result_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        for(int ii = 0 ; ii < relative_db_path_list.size() ; ii++)
        {
            destination_db_file_list << global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path_list.at(ii) , Q_FUNC_INFO);
        }

    }
    else if(m_plugin_name == MACRO_Plugin_Name_Mobile_Backup)
    {
        destination_db_file_list << destination_file_location + "mobile_backup.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Carved_Data)
    {
        destination_db_file_list << destination_file_location + "carved_data_content_info.sqlite";
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Snapshots)
    {
        QString index_db_path = destination_file_location + "saved_snapshots_index.sqlite";
        QString cmd = "Select snapshot_lable_name from tbl_saved_index";
        QStringList snapshot_name_str = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd,0,index_db_path,Q_FUNC_INFO);
        for(int ii = 0 ; ii < snapshot_name_str.size() ; ii++)
        {
            destination_db_file_list << destination_file_location + snapshot_name_str.at(ii) + ".sqlite";
        }
    }
    else if(m_plugin_name == MACRO_Plugin_Name_Examiner_Space)
    {
        destination_db_file_list << destination_file_location + "Examiner_Notes.html";
    }
    else
        destination_db_file_list << destination_file_location + plgn_name.replace(" ","_") + ".sqlite";


    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Destination_Db_File_List_QString,destination_db_file_list);


    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Destination_File_Location_QString,destination_file_location);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Plugin_Name_QString,m_plugin_name);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Parent_Module_Name_QString,global_csv_reader_class_selected_obj->get_parent_category(plgn_name));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Selected_Tags_QStringList,selected_tags_list);

    narad_muni_for_report_obj->set_field(MACRO_NARAD_Reports_Dir_Path_QString,report_dir);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Global_Adv_Html_Records_Per_Page_int,50);


    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Tab_Name_Textedit_QStringList,tab_name_list_textedit);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Tab_Name_Stack_QStringList,tab_name_list_stack);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Artifact_Common_Tablewidget_QStringList,table_artifact_common_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Report_Html_Header_Tablewidget_QStringList,table_report_html_header_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Report_Header_Column_Width_Html_Tablewidget_QStringList,table_report_header_column_width_html_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList,table_export_criteria_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Artifact_Common_Textedit_QStringList,artifact_common_list_textedit);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Header_Textedit_QStringList,report_html_header_list_textedit);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Header_Column_Width_Html_Textedit_QStringList,report_header_column_width_html_list_textedit);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Artifact_Common_Stack_QStringList,artifact_common_list_stack);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Header_Stack_QStringList,report_html_header_list_stack);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Header_Column_Width_Html_Stack_QStringList,report_header_column_width_html_list_stack);





    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList,table_export_available_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList,table_name_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Html_Report_Available_Tablewidget_QStringList,table_html_report_available_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList,tab_name_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Name_Textedit_QStringList,table_name_list_textedit);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Report_Available_Textedit_QStringList,html_report_available_list_textedit);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_Name_Stack_QStringList,table_name_list_stack);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Report_Available_Stack_QStringList,html_report_available_list_stack);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_CSV_Report_Available_Tablewidget_QStringList,table_csv_report_available_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_XML_Report_Available_Tablewidget_QStringList,table_xml_report_available_list_tablewidget);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Table_KML_Report_Available_Tablewidget_QStringList,table_kml_report_available_list_tablewidget);


    QList<QVariant> path_list_tablewidget;
    for(int i=0; i< export_path_column_list_tablewidget.size(); i++)
        path_list_tablewidget.append(export_path_column_list_tablewidget.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Export_Path_Tablewidget_QList,path_list_tablewidget);

    QList<QVariant> data_type_list_tablewidget;
    for(int i=0; i< tw_data_type_list_tablewidget.size(); i++)
        data_type_list_tablewidget.append(tw_data_type_list_tablewidget.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList,data_type_list_tablewidget);



    QList<QVariant> html_col_name_list_textedit;
    for(int i=0; i< report_html_col_name_list_textedit.size(); i++)
        html_col_name_list_textedit.append(report_html_col_name_list_textedit.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Col_Name_Textedit_QList,html_col_name_list_textedit);


    QList<QVariant> html_data_type_list_tablewidget;
    for(int i=0; i< rt_html_data_type_list_tablewidget.size(); i++)
        html_data_type_list_tablewidget.append(rt_html_data_type_list_tablewidget.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Data_Type_Tablewidget_QList,html_data_type_list_tablewidget);


    QList<QVariant> html_col_name_list_tablewidget;
    for(int i=0; i< rt_html_col_name_list_tablewidget.size(); i++)
        html_col_name_list_tablewidget.append(rt_html_col_name_list_tablewidget.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Col_Name_Tablewidget_QList,html_col_name_list_tablewidget);


    QList<QVariant> html_datatype_list_textedit;
    for(int i=0; i< report_html_datatype_list_textedit.size(); i++)
        html_datatype_list_textedit.append(report_html_datatype_list_textedit.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Datatype_Textedit_QList,html_datatype_list_textedit);


    QList<QVariant> html_col_name_list_stack;
    for(int i=0; i< report_html_col_name_list_stack.size(); i++)
        html_col_name_list_stack.append(report_html_col_name_list_stack.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Col_Name_Stack_QList,html_col_name_list_stack);


    QList<QVariant> html_datatype_list_stack;
    for(int i=0; i< report_html_datatype_list_stack.size(); i++)
        html_datatype_list_stack.append(report_html_datatype_list_stack.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Datatype_Stack_QList,html_datatype_list_stack);


    QList<QVariant> html_d_name_list_tablewidget;
    for(int i=0; i< rt_html_d_name_list_tablewidget.size(); i++)
        html_d_name_list_tablewidget.append(rt_html_d_name_list_tablewidget.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Display_Name_Tablewidget_QList,html_d_name_list_tablewidget);


    QList<QVariant> html_order_list_tablewidget;
    for(int i=0; i< rt_html_order_list_tablewidget.size(); i++)
        html_order_list_tablewidget.append(rt_html_order_list_tablewidget.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Html_Order_Tablewidget_QList,html_order_list_tablewidget);




    QList<QVariant> csv_data_type_list;
    for(int i=0; i< rt_csv_data_type_list.size(); i++)
        csv_data_type_list.append(rt_csv_data_type_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_CSV_Data_Type_QList,csv_data_type_list);


    QList<QVariant> csv_col_name_list;
    for(int i=0; i< rt_csv_col_name_list.size(); i++)
        csv_col_name_list.append(rt_csv_col_name_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_CSV_Col_Name_QList,csv_col_name_list);



    QList<QVariant> csv_d_name_list;
    for(int i=0; i< rt_csv_d_name_list.size(); i++)
        csv_d_name_list.append(rt_csv_d_name_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_CSV_Display_Name_QList,csv_d_name_list);

    QList<QVariant> csv_order_list;
    for(int i=0; i< rt_csv_order_list.size(); i++)
        csv_order_list.append(rt_csv_order_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_CSV_Order_QList,csv_order_list);




    QList<QVariant> xml_d_name_list;
    for(int i=0; i< rt_xml_d_name_list.size(); i++)
        xml_d_name_list.append(rt_xml_d_name_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_XML_Display_Name_QList,xml_d_name_list);


    QList<QVariant> xml_order_list;
    for(int i=0; i< rt_xml_order_list.size(); i++)
        xml_order_list.append(rt_xml_order_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_XML_Order_QList,xml_order_list);


    QList<QVariant> xml_data_type_list;
    for(int i=0; i< rt_xml_data_type_list.size(); i++)
        xml_data_type_list.append(rt_xml_data_type_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_XML_Data_Type_QList,xml_data_type_list);


    QList<QVariant> xml_col_name_list;
    for(int i=0; i< rt_xml_col_name_list.size(); i++)
        xml_col_name_list.append(rt_xml_col_name_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_XML_Col_Name_QList,xml_col_name_list);




    QList<QVariant> kml_data_type_list;
    for(int i=0; i< rt_kml_data_type_list.size(); i++)
        kml_data_type_list.append(rt_kml_data_type_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_KML_Data_Type_QList,kml_data_type_list);


    QList<QVariant> kml_col_name_list;
    for(int i=0; i< rt_kml_col_name_list.size(); i++)
        kml_col_name_list.append(rt_kml_col_name_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_KML_Col_Name_QList,kml_col_name_list);



    QList<QVariant> kml_d_name_list;
    for(int i=0; i< rt_kml_d_name_list.size(); i++)
        kml_d_name_list.append(rt_kml_d_name_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_KML_Display_Name_QList,kml_d_name_list);


    QList<QVariant> kml_order_list;
    for(int i=0; i< rt_kml_order_list.size(); i++)
        kml_order_list.append(rt_kml_order_list.at(i));
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_KML_Order_QList,kml_order_list);

}

