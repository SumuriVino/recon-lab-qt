#include "thread_report_builder.h"

void thread_report_builder::create_report_header()
{

    if(m_report_type == MACRO_REPORT_TYPE_HTML_ADVANCE_QString)
    {
        write_html_advance_report_header();
    }
    else if(m_report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || m_report_type == MACRO_REPORT_TYPE_PDF_QString)
    {
        if(m_report_type == MACRO_REPORT_TYPE_PDF_QString)
        {
            bool_pdf_report_finished = false;

            if(report_filepath.endsWith(".pdf"))
            {
                report_filepath.chop(3); //pdf
                report_filepath.append("html");
            }
        }


        write_html_standard_report_header();


        if(m_report_type == MACRO_REPORT_TYPE_PDF_QString)
        {
            if(selected_plugins_info.size() <= 0)
            {
                QString pdf_filepath = report_filepath;
                if(pdf_filepath.endsWith(".html"))
                {
                    pdf_filepath.chop(4); // html
                    pdf_filepath.append("pdf");
                }
                emit signal_create_pdf_report(report_filepath, pdf_filepath);
            }
        }


    }
    else if(m_report_type == MACRO_REPORT_TYPE_CSV_QString)
    {
        QFile file(report_filepath);
        if(!file.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("FILE Open ------FAILED----- " + file.fileName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + file.errorString(),Q_FUNC_INFO);
            return;
        }

        QTextStream outputfile_csv(&file);
        global_recon_helper_singular_class_obj->write_csv_report_case_details_and_source_info(outputfile_csv ,m_report_scope, narad_muni_for_report_obj);
        file.flush();
        file.close();
    }
    else if(m_report_type == MACRO_REPORT_TYPE_XML_QString)
    {
        QFile file(report_filepath);
        if(!file.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("File Open ---FAILED--- " + file.fileName() ,Q_FUNC_INFO);
            recon_static_functions::app_debug("error  " + file.errorString(),Q_FUNC_INFO);
            return;
        }

        QXmlStreamWriter  stream_xml(&file);
        stream_xml.writeStartElement("RECON_Report");
        global_recon_helper_singular_class_obj->write_xml_report_case_info_and_source_info(&stream_xml,m_report_scope, narad_muni_for_report_obj);
        file.flush();
        file.close();
    }

}


void thread_report_builder::write_html_advance_report_header()
{
    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);

    QFile file(report_filepath);
    if(!file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("file open ----FAILED----" + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString(),Q_FUNC_INFO);
        return;
    }

    QString report_data;

    ///- A. Set Report Header
    report_data += report_html_template_plugin_advance_obj->report_main_set_html_header();

    ///- B.Set Agency Name in main Top bar.
    report_data += report_html_template_plugin_advance_obj->report_main_set_tool_agency_name();

    ///- C. Set Plugins List in Dropdown list of TOP bar
    //    report_data += report_html_template_MAIN_obj->report_main_set_plugins_list_in_topbar_dropdown_list(list_strct_plugins_tab);

    report_html_template_plugin_advance_obj->set_essential();

    report_data += report_html_template_plugin_advance_obj->report_main_set_sidebar_plugins_list(selected_plugins_info);

    ///-E. Set Agency Logo, Name, and TAG Line
    QString tag_line = recon_static_functions::get_app_name();
    report_data += report_html_template_plugin_advance_obj->report_main_set_agency_logo_and_name(logo_path,agency,tag_line);

    report_data += global_recon_helper_singular_class_obj->write_advanced_html_report_case_details_and_source_info(m_report_scope, narad_muni_for_report_obj);

    ///-G End Main Page
    report_data += report_html_template_plugin_advance_obj->report_main_end();

    file.write(report_data.toLocal8Bit().data());
    file.flush();
    file.close();

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);

    return;
}

void thread_report_builder::write_html_standard_report_header()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    QFile file(report_filepath);
    if(!file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" -- FILE Open ------FAILED----- " + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--error-- " + file.errorString(),Q_FUNC_INFO);
        return;
    }


    file.write(report_template_html_standard::html_header1().toLocal8Bit().data());
    file.write(report_template_html_standard::html_title("Standard HTML REPORT").toLocal8Bit().data());
    file.write(report_template_html_standard::html_header_after_title().toLocal8Bit().data());
    file.write(report_template_html_standard::html_sidebar_start().toLocal8Bit().data());



    for(int i = 0; i < selected_plugins_info.size(); i++)
    {
        QString value_str = selected_plugins_info.at(i);

        QStringList tmp_list = value_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            continue;

        QString plugin_name = tmp_list.at(0);
        QStringList selected_tablist = recon_static_functions::get_associated_tabs(plugin_name,QStringList(value_str),Q_FUNC_INFO);

        extract_csv_values(plugin_name);
        update_narad_muni(plugin_name);
        report_generic_function_obj->pub_set_essentials();

        if(m_report_maker == MACRO_REPORT_MAKER_PLUGIN_int)
        {

            if(m_report_scope != MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString && global_csv_reader_class_selected_obj->is_plugin_available_for_chat_view(plugin_name))
            {
                if(m_report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    file.write(report_template_html_standard::html_sidebar_link(global_csv_reader_class_selected_obj->get_chat_view_tab_name(plugin_name)).toLocal8Bit().data());

                }
                else if(m_report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QString chat_bk_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
                    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select INT from chat_viewer_info where plugin_name = ?", QStringList(plugin_name),0,chat_bk_db_path,Q_FUNC_INFO);
                    if(!INT_str.trimmed().isEmpty())
                    {
                        file.write(report_template_html_standard::html_sidebar_link(global_csv_reader_class_selected_obj->get_chat_view_tab_name(plugin_name)).toLocal8Bit().data());
                    }

                }
            }


            for(int pp = 0; pp < selected_tablist.size(); pp++)
            {

                QString tab_name = selected_tablist.at(pp);
                if(global_csv_reader_class_selected_obj->is_tab_type_chat_view(plugin_name , tab_name))
                    continue ;

                if(report_generic_function_obj->pub_is_data_available_in_tab(QStringList(tab_name), m_report_scope, selected_tags_list))
                {
                    QString tblname = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, selected_tablist.at(pp));



                    if(table_name_list_tablewidget.contains(tblname))
                    {
                        int table_index = table_name_list_tablewidget.indexOf(tblname);
                        QString is_report_available = table_html_report_available_list_tablewidget.at(table_index);
                        if(is_report_available != QString("1"))
                            continue;

                        QString tab_name = tab_name_list_tablewidget.at(table_index);
                        file.write(report_template_html_standard::html_sidebar_link(tab_name).toLocal8Bit().data());
                    }
                    else if(table_name_list_textedit.contains(tblname))
                    {
                        int table_index = table_name_list_textedit.indexOf(tblname);
                        QString is_report_available = html_report_available_list_textedit.at(table_index);
                        if(is_report_available != QString("1"))
                            continue;

                        QString tab_name = tab_name_list_textedit.at(table_index);
                        file.write(report_template_html_standard::html_sidebar_link(tab_name).toLocal8Bit().data());
                    }
                    else  if(table_name_list_stack.contains(tblname))
                    {
                        int table_index = table_name_list_stack.indexOf(tblname);
                        QString is_report_available = html_report_available_list_stack.at(table_index);
                        if(is_report_available != QString("1"))
                            continue;

                        QString tab_name = tab_name_list_stack.at(table_index);
                        file.write(report_template_html_standard::html_sidebar_link(tab_name).toLocal8Bit().data());
                    }

                }

            }
        }
        else
            file.write(report_template_html_standard::html_sidebar_link(plugin_name).toLocal8Bit().data());
    }



    file.write(report_template_html_standard::html_sidebar_end().toLocal8Bit().data());
    file.write(report_template_html_standard::html_report_main_title("./resources/agency_logo.png",narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString()).toLocal8Bit().data());
    file.write(global_recon_helper_singular_class_obj->write_html_case_info_and_source_info(m_report_scope, narad_muni_for_report_obj).toLocal8Bit().data());

    file.flush();
    file.close();

    recon_static_functions::app_debug("- End",Q_FUNC_INFO);
}


