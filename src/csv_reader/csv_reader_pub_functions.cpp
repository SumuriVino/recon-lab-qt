#include "csv_reader.h"


void csv_reader::set_csv_dir_path(QString path)
{ //  set the path of csv directory
    csv_dir_path = path;
    if(!csv_dir_path.endsWith("/"))
        csv_dir_path.append("/");

}


void csv_reader::do_job_plugin_all()
{ // parse csv for plugins name and data
    recon_static_functions::app_debug("start  ",Q_FUNC_INFO);

    csv_info_list_final.clear();
    csv_info_list_full.clear();

    QString csvs_path = csv_dir_path;

    QDirIterator iter(csvs_path,QDirIterator::Subdirectories);
    while(iter.hasNext())
    {
        QCoreApplication::processEvents();
        QFileInfo info(iter.next());

        if(info.isHidden() || info.isSymLink()  || info.isDir())
            continue;

        QString plugin_csv_path = info.filePath();

        parse_plugin_csv(plugin_csv_path);
    }


    if(csv_info_list_final.size() <= 0)
    {
        recon_static_functions::app_debug(" csv_info_list size is 0 or less  -----FAILED---- csv dir path -> " + csv_dir_path,Q_FUNC_INFO);
        return;
    }


    //=============================================//
    // checking that if two or more plugin have same name
    checking_plugins_duplicacy();
    //=============================================//

    recon_static_functions::app_debug("end  ",Q_FUNC_INFO);

}

void csv_reader::do_job_plugin_specific(QString csv_file_path, QString caller_func)
{ //
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    csv_info_list_final.clear();
    csv_info_list_full.clear();

    if(!QFileInfo(csv_file_path).exists())
    {
        recon_static_functions::app_debug("-----FAILED---- csv file not exist -> " + csv_file_path,Q_FUNC_INFO);
        return;
    }

    parse_plugin_csv(csv_file_path);


    if(csv_info_list_final.size() <= 0)
    {
        recon_static_functions::app_debug(" csv_info_list size is 0 or less  -----FAILED---- csv file path -> " + csv_file_path,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);

}

void csv_reader::parse_plugin_csv(QString csv_file_path)
{

    if(!csv_file_path.endsWith(".csv"))
        return;

    clear_fields();

    if(make_master_lists(csv_file_path) == -1)
    {
        recon_static_functions::app_debug(" error while creating master list  -----FAILED----plugin -> " + plugin_name,Q_FUNC_INFO);
        return;

    }
    if(!is_our_masterlist_and_GUItab_size_accurate())
    {
        recon_static_functions::app_debug(" csv and GUI tab size not accurate   -----FAILED----plugin -> " + plugin_name,Q_FUNC_INFO);
        return;
    }

    generate_info();
    fill_value();

}

void csv_reader::checking_plugins_duplicacy()
{
    recon_static_functions::app_debug("start", Q_FUNC_INFO);


    /// Checking that same plugin name is used in more than one csv

    QList<struct_global_csv_reader_full_csv_info> tmp_list = csv_info_list_final;

    for(int i = 0; i < tmp_list.size(); i++)
    {
        QStringList csv_path_list;
        struct_global_csv_reader_full_csv_info obj1 =  tmp_list.takeAt(i);
        QString tmp_plgn_nm = obj1.plugin_name;
        csv_path_list << obj1.plugin_csv_file_path;

        for(int pp = 0; pp < tmp_list.size(); pp++)
        {
            struct_global_csv_reader_full_csv_info obj2 =  tmp_list.at(pp);
            if(obj2.plugin_name.trimmed() == tmp_plgn_nm.trimmed())
            {
                csv_path_list << obj2.plugin_csv_file_path;
                tmp_list.removeAt(pp);
            }
        }

        if(csv_path_list.size() > 1)
        {
            recon_static_functions::app_debug(" ---FAILED--- ---FAILED--- ---FAILED--- ---FAILED--- redefined plugin name : " + tmp_plgn_nm, Q_FUNC_INFO);
            recon_static_functions::app_debug(" ---FAILED--- ---FAILED--- ---FAILED--- ---FAILED--- redefined plugin name : " + tmp_plgn_nm, Q_FUNC_INFO);
            recon_static_functions::app_debug(" ---FAILED--- ---FAILED--- ---FAILED--- ---FAILED--- redefined plugin name : " + tmp_plgn_nm, Q_FUNC_INFO);
            recon_static_functions::app_debug(" ---FAILED--- ---FAILED--- ---FAILED--- ---FAILED--- redefined plugin name : " + tmp_plgn_nm, Q_FUNC_INFO);
            recon_static_functions::app_debug(" ---FAILED--- ---FAILED--- ---FAILED--- ---FAILED--- redefined plugin name : " + tmp_plgn_nm, Q_FUNC_INFO);
            recon_static_functions::app_debug(" ---FAILED--- ---FAILED--- ---FAILED--- ---FAILED--- redefined plugin name : " + tmp_plgn_nm, Q_FUNC_INFO);
            recon_static_functions::app_debug("  csv paths : " + csv_path_list.join("-----"), Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug(" end", Q_FUNC_INFO);

}

struct_global_csv_reader_full_csv_info csv_reader::read_csv_value(QString plugin_name)
{
    struct_global_csv_reader_full_csv_info obj1;
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            return csv_info_list_final.at(i);
        }
    }

    recon_static_functions::app_debug("csv_reader::read_csv_value csv value not found  ---FAILED---   plugin name " + plugin_name,Q_FUNC_INFO);

    return obj1;
}


QList<struct_global_csv_reader_full_csv_info> csv_reader::get_csv_info_list()
{
    return csv_info_list_final;
}


bool csv_reader::is_export_available_in_plugin(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_export_available_QStringList_tablewidget.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_export_available_QStringList_tablewidget.at(tt) == QString("1"))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool csv_reader::is_export_available_in_tab(QString tab_name, QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            int tab_index =  csv_info_list_final.at(i).tab_name_QStringList_tablewidget.indexOf(tab_name,0);

            if(csv_info_list_final.at(i).tab_export_available_QStringList_tablewidget.at(tab_index) == "1")
                return true;
            else
                return false;
        }
    }

    return false;
}

bool csv_reader::is_tab_support_multiple_export(QString tab_name, QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            int tab_index =  csv_info_list_final.at(i).tab_name_QStringList_tablewidget.indexOf(tab_name,0);

            if(csv_info_list_final.at(i).tab_export_criteria_QStringList_tablewidget.at(tab_index) == "2")
                return true;
            else
                return false;
        }
    }

    return false;
}

bool csv_reader::is_timeline_available_in_plugin(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.at(tt) == QString("1"))
                {
                    return true;
                }
            }
        }
    }

    return false;
}



bool csv_reader::is_timeline_available_in_tab(QString plugin_name,QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.at(tt) == QString("1"))
                {

                    for(int tb = 0; tb < csv_info_list_final.at(i).tab_name_QStringList_tablewidget.size(); tb++ )
                    {
                        if(csv_info_list_final.at(i).tab_name_QStringList_tablewidget.at(tb) == tab_name && csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.at(tb) == "1")
                        {
                            return true;
                        }
                    }

                }
            }
        }
    }

    return false;
}

bool csv_reader::is_timeline_available_in_table(QString plugin_name,QString table_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.at(tt) == QString("1"))
                {

                    for(int tb = 0; tb < csv_info_list_final.at(i).table_name_QStringList_tablewidget.size(); tb++ )
                    {
                        if(csv_info_list_final.at(i).table_name_QStringList_tablewidget.at(tb) == table_name && csv_info_list_final.at(i).tab_timeline_available_QStringList_tablewidget.at(tb) == "1")
                        {
                            return true;
                        }
                    }

                }
            }
        }
    }

    return false;
}

QStringList csv_reader::get_total_tables_list(QString plugin_name)
{
    QStringList table_list;

    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            table_list =  csv_info_list_final.at(i).overall_table_name_QStringList;
            break;
        }
    }

    return table_list;
}

QStringList csv_reader::get_total_tab_list(QString plugin_name)
{
    QStringList tab_list;

    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            tab_list =  csv_info_list_final.at(i).overall_tab_name_QStringList;
            break;
        }
    }

    return tab_list;

}

QString csv_reader::get_parent_category(QString plugin_name)
{
    QString parent_name;

    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            parent_name =  csv_info_list_final.at(i).plugin_parent_name;
            break;
        }
    }

    return parent_name;
}

QStringList csv_reader::get_export_tab_list(QString plugin_name)
{
    QStringList list_tab;
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_export_available_QStringList_tablewidget.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_export_available_QStringList_tablewidget.at(tt) == QString("1"))
                {
                    list_tab << csv_info_list_final.at(i).tab_name_QStringList_tablewidget.at(tt);
                }
            }

            return list_tab;
        }
    }
    return list_tab;
}

QString csv_reader::get_table_name_according_tab(QString plugin_name, QString tab_name)
{

    QString table_name;

    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            int tab_index =  csv_info_list_final.at(i).overall_tab_name_QStringList.indexOf(tab_name,0);

            if(tab_index == -1)
            {
                return table_name;
            }
            table_name = csv_info_list_final.at(i).overall_table_name_QStringList.at(tab_index);
            break;
        }
    }

    return table_name;

}

QStringList csv_reader::get_full_tables_name_according_tabs(QString plugin_name, QStringList tab_list)
{
    // this function must be private

    QList<struct_global_csv_reader_full_csv_info> csv_info_list = csv_info_list_full;

    QStringList table_list;

    for(int i = 0; i < csv_info_list.size(); i++)
    {
        if(csv_info_list.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int tab_count = 0; tab_count < tab_list.size(); tab_count++)
            {

                int tab_index =  csv_info_list.at(i).overall_tab_name_QStringList.indexOf(tab_list.at(tab_count),0);

                if(tab_index == -1)
                {
                    table_list << "";
                    continue;
                }

                table_list << csv_info_list.at(i).overall_table_name_QStringList.at(tab_index);
            }
            break;
        }
    }
    return table_list;
}

void csv_reader::clear_variables_csv_info(struct_global_csv_reader_full_csv_info &struct_csv_info)
{
    struct_csv_info.overall_tab_count_int = 0;
    struct_csv_info.overall_tab_name_QStringList.clear();
    struct_csv_info.overall_table_name_QStringList.clear();
    struct_csv_info.overall_tab_order_QStringList.clear();

    struct_csv_info.plugin_csv_file_name.clear();
    struct_csv_info.plugin_csv_file_path.clear();
    struct_csv_info.plugin_name.clear();
    struct_csv_info.plugin_parent_name.clear();
    struct_csv_info.plugin_native_os_QStringList.clear();
    struct_csv_info.plugin_supported_os_QStringList.clear();
    struct_csv_info.plugin_extraction_mode.clear();
    struct_csv_info.plugin_log_show_supported.clear();
    struct_csv_info.plugin_root_privileged.clear();
    struct_csv_info.plugin_user_iterated.clear();
    struct_csv_info.plugin_available_for_redefined_result.clear();
    struct_csv_info.plugin_supported_software_QStringList.clear();


    struct_csv_info.tab_count_int_tablewidget = 0;
    struct_csv_info.table_name_QStringList_tablewidget.clear();
    struct_csv_info.tab_name_QStringList_tablewidget.clear();
    struct_csv_info.tab_supported_os_scheme_QList_tablewidget.clear();
    struct_csv_info.tab_supported_software_QList_tablewidget.clear();
    struct_csv_info.tab_log_show_supported_QStringList_tablewidget.clear();
    struct_csv_info.tab_root_privileged_QStringList_tablewidget.clear();
    struct_csv_info.tab_user_iterated_QStringList_tablewidget.clear();
    struct_csv_info.tab_apple_metadata_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_html_report_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_xml_report_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_csv_report_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_kml_report_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_export_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_export_criteria_QStringList_tablewidget.clear();
    struct_csv_info.tab_report_html_header_QStringList_tablewidget.clear();
    struct_csv_info.tab_report_header_column_width_html_QStringList_tablewidget.clear();
    struct_csv_info.tab_g_search_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_timeline_available_QStringList_tablewidget.clear();
    struct_csv_info.tab_timeline_timestamp_type_QList_tablewidget.clear();
    struct_csv_info.tab_timeline_timestamp_column_name_QList_tablewidget.clear();
    struct_csv_info.tab_artifact_common_QStringList_tablewidget.clear();

    struct_csv_info.tab_count_int_textedit = 0;
    struct_csv_info.tab_name_QStringList_textedit.clear();
    struct_csv_info.table_name_QStringList_textedit.clear();
    struct_csv_info.tab_supported_os_scheme_QList_textedit.clear();
    struct_csv_info.tab_supported_software_QList_textedit.clear();
    struct_csv_info.tab_log_show_supported_QStringList_textedit.clear();
    struct_csv_info.tab_root_privileged_QStringList_textedit.clear();
    struct_csv_info.tab_user_iterated_QStringList_textedit.clear();
    struct_csv_info.tab_html_report_available_QStringList_textedit.clear();
    struct_csv_info.tab_xml_report_available_QStringList_textedit.clear();
    struct_csv_info.tab_csv_report_available_QStringList_textedit.clear();
    struct_csv_info.tab_report_html_header_QStringList_textedit.clear();
    struct_csv_info.tab_report_header_column_width_html_QStringList_textedit.clear();
    struct_csv_info.tab_artifact_common_QStringList_textedit.clear();

    struct_csv_info.tab_count_int_stack = 0;
    struct_csv_info.tab_name_QStringList_stack.clear();
    struct_csv_info.table_name_QStringList_stack.clear();
    struct_csv_info.tab_supported_os_scheme_QList_stack.clear();
    struct_csv_info.tab_supported_software_QList_stack.clear();
    struct_csv_info.tab_log_show_supported_QStringList_stack.clear();
    struct_csv_info.tab_root_privileged_QStringList_stack.clear();
    struct_csv_info.tab_user_iterated_QStringList_stack.clear();
    struct_csv_info.tab_html_report_available_QStringList_stack.clear();
    struct_csv_info.tab_xml_report_available_QStringList_stack.clear();
    struct_csv_info.tab_csv_report_available_QStringList_stack.clear();
    struct_csv_info.tab_report_html_header_QStringList_stack.clear();
    struct_csv_info.tab_report_header_column_width_html_QStringList_stack.clear();
    struct_csv_info.tab_artifact_common_QStringList_stack.clear();

    struct_csv_info.tab_count_int_tree = 0;
    struct_csv_info.table_name_QStringList_tree.clear();
    struct_csv_info.tab_name_QStringList_tree.clear();
    struct_csv_info.tab_supported_os_scheme_QList_tree.clear();
    struct_csv_info.tab_supported_software_QList_tree.clear();
    struct_csv_info.tab_log_show_supported_QStringList_tree.clear();
    struct_csv_info.tab_root_privileged_QStringList_tree.clear();
    struct_csv_info.tab_user_iterated_QStringList_tree.clear();

    struct_csv_info.tab_count_int_thumbnail = 0;
    struct_csv_info.table_name_QStringList_thumbnail.clear();
    struct_csv_info.tab_name_QStringList_thumbnail.clear();
    struct_csv_info.tab_supported_os_scheme_QList_thumbnail.clear();
    struct_csv_info.tab_supported_software_QList_thumbnail.clear();
    struct_csv_info.tab_log_show_supported_QStringList_thumbnail.clear();
    struct_csv_info.tab_root_privileged_QStringList_thumbnail.clear();
    struct_csv_info.tab_user_iterated_QStringList_thumbnail.clear();

    struct_csv_info.tab_count_int_chat_viewer = 0;
    struct_csv_info.table_name_QStringList_chat_viewer.clear();
    struct_csv_info.tab_name_QStringList_chat_viewer.clear();
    struct_csv_info.tab_supported_os_scheme_QList_chat_viewer.clear();
    struct_csv_info.tab_supported_software_QList_chat_viewer.clear();
    struct_csv_info.tab_log_show_supported_QStringList_chat_viewer.clear();
    struct_csv_info.tab_root_privileged_QStringList_chat_viewer.clear();
    struct_csv_info.tab_user_iterated_QStringList_chat_viewer.clear();


    //------------------------------------------------------------------------//
    struct_csv_info.display_header_QList_tablewidget.clear();
    struct_csv_info.display_column_name_QList_tablewidget.clear();
    struct_csv_info.display_data_type_QList_tablewidget.clear();
    struct_csv_info.display_col_width_QList_tablewidget.clear();
    struct_csv_info.local_search_column_name_QList_tablewidget.clear();
    struct_csv_info.display_order_QList_tablewidget.clear();

    struct_csv_info.display_column_name_QList_textedit.clear();
    struct_csv_info.display_data_type_QList_textedit.clear();

    struct_csv_info.display_column_name_QList_stack.clear();
    struct_csv_info.display_data_type_QList_stack.clear();

    struct_csv_info.display_header_QList_tree.clear();
    struct_csv_info.display_parent_child_QList_tree.clear();
    struct_csv_info.display_column_name_QList_tree.clear();
    struct_csv_info.display_data_type_QList_tree.clear();
    struct_csv_info.display_col_width_QList_tree.clear();

    struct_csv_info.display_column_name_QList_chat_viewer.clear();
    struct_csv_info.display_data_type_QList_chat_viewer.clear();
    //------------------------------------------------------------------------//


    //------------------------------------------------------------------------//
    struct_csv_info.metadata_header_QList_tablewidget.clear();
    struct_csv_info.metadata_column_name_QList_tablewidget.clear();
    struct_csv_info.metadata_data_type_QList_tablewidget.clear();
    struct_csv_info.metadata_preview_QList_tablewidget.clear();
    struct_csv_info.export_path_column_name_QList_tablewidget.clear();

    struct_csv_info.metadata_header_QList_tree.clear();
    struct_csv_info.metadata_column_name_QList_tree.clear();
    struct_csv_info.metadata_data_type_QList_tree.clear();
    struct_csv_info.metadata_preview_QList_tree.clear();

    //------------------------------------------------------------------------//

    struct_csv_info.report_html_display_name_QList_tablewidget.clear();
    struct_csv_info.report_html_col_name_QList_tablewidget.clear();
    struct_csv_info.report_html_order_QList_tablewidget.clear();
    struct_csv_info.report_html_data_type_QList_tablewidget.clear();

    struct_csv_info.report_html_col_name_QList_textedit.clear();
    struct_csv_info.report_html_datatype_QList_textedit.clear();

    struct_csv_info.report_html_col_name_QList_stack.clear();
    struct_csv_info.report_html_datatype_QList_stack.clear();

    struct_csv_info.report_xml_display_name_QList_tablewidget.clear();
    struct_csv_info.report_xml_col_name_QList_tablewidget.clear();
    struct_csv_info.report_xml_order_QList_tablewidget.clear();
    struct_csv_info.report_xml_data_type_QList_tablewidget.clear();


    struct_csv_info.report_csv_display_name_QList_tablewidget.clear();
    struct_csv_info.report_csv_col_name_QList_tablewidget.clear();
    struct_csv_info.report_csv_order_QList_tablewidget.clear();
    struct_csv_info.report_csv_data_type_QList_tablewidget.clear();


    struct_csv_info.report_kml_display_name_QList_tablewidget.clear();
    struct_csv_info.report_kml_col_name_QList_tablewidget.clear();
    struct_csv_info.report_kml_order_QList_tablewidget.clear();
    struct_csv_info.report_kml_data_type_QList_tablewidget.clear();

    struct_csv_info.g_search_col_name_QList.clear();
    struct_csv_info.g_search_col_order_QList.clear();
    struct_csv_info.g_search_col_data_type_QList.clear();

    struct_csv_info.g_timeline_col_name_QList.clear();
    struct_csv_info.g_timeline_col_data_type_QList.clear();


}

QStringList csv_reader::get_tables_name_according_tabs(QString plugin_name, QStringList tab_list)
{

    QStringList table_list;

    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int tab_count = 0; tab_count < tab_list.size(); tab_count++)
            {

                int tab_index =  csv_info_list_final.at(i).overall_tab_name_QStringList.indexOf(tab_list.at(tab_count),0);

                if(tab_index == -1)
                {
                    table_list << "";
                    continue;
                }

                table_list << csv_info_list_final.at(i).overall_table_name_QStringList.at(tab_index);
            }
            break;
        }
    }
    return table_list;
}

void csv_reader::set_plugin_and_associated_tabs(QStringList received_plugin_and_tabs_list, QString caller_func)
{

    if(received_plugin_and_tabs_list.size() <= 0)
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "     ---FAILED---  plugin_and_associated_tabs_list size <=0 ",Q_FUNC_INFO);
        return;
    }

    QList<struct_global_csv_reader_full_csv_info> csv_info_list_selected = csv_info_list_full;



    // ----------------------- Below reading is for selected plugin only -----------//

    for(int plgn_count = 0; plgn_count < received_plugin_and_tabs_list.size(); plgn_count++)
    {

        QString m_plugin_name =  received_plugin_and_tabs_list.at(plgn_count).split(MACRO_RECON_Splitter_1_multiple).at(0);

        QStringList associated_tabs_lst;
        associated_tabs_lst = recon_static_functions::get_associated_tabs(m_plugin_name,received_plugin_and_tabs_list, Q_FUNC_INFO);
        struct_global_csv_reader_full_csv_info obj;

        if(associated_tabs_lst.size() <= 0)
            continue;


        ///  --------------------------- DO NOT DELETE ---------------------
        /// We are not giving option to select view's tabs (Like Chat View, Tree View), but we have to consider them

        QStringList selected_tables_list = get_full_tables_name_according_tabs(m_plugin_name , associated_tabs_lst);

        QStringList matched_tabs_list;
        for(int count = 0; count < selected_tables_list.size(); count++)
        {
            matched_tabs_list << get_all_full_tab_list_associated_with_table(m_plugin_name , selected_tables_list.at(count));
        }

        if(!matched_tabs_list.isEmpty())
        {
            associated_tabs_lst.clear();
            associated_tabs_lst << matched_tabs_list;
        }


        for(int i = 0; i < csv_info_list_selected.size(); i++)
        {
            if(csv_info_list_selected.at(i).plugin_name.trimmed() == m_plugin_name.trimmed())
            {
                obj = csv_info_list_selected.at(i);

                for(int count = obj.tab_name_QStringList_tablewidget.size() -1; count >=0; count--)
                {

                    ///********************************************
                    /// Loop is running backwards to remove entries
                    /// (Dont be confused loop is running on same list and removing from the same).
                    /// *******************************************


                    QString tmp_tab_str   = obj.tab_name_QStringList_tablewidget.at(count);

                    if(!associated_tabs_lst.contains(tmp_tab_str.trimmed()))
                    {
                        //                        obj.overall_tab_name_QStringList.removeAt(count);
                        //                        obj.overall_table_name_QStringList.removeAt(count);

                        int index = obj.overall_tab_name_QStringList.indexOf(tmp_tab_str, 0);

                        obj.overall_tab_count_int--;
                        obj.overall_tab_order_QStringList.removeAt(index);
                        obj.overall_tab_name_QStringList.removeAt(index);
                        obj.overall_table_name_QStringList.removeAt(index);

                        obj.tab_count_int_tablewidget--;
                        obj.table_name_QStringList_tablewidget.removeAt(count);
                        obj.tab_name_QStringList_tablewidget.removeAt(count);
                        obj.tab_supported_os_scheme_QList_tablewidget.removeAt(count);
                        obj.tab_supported_software_QList_tablewidget.removeAt(count);
                        obj.tab_log_show_supported_QStringList_tablewidget.removeAt(count);
                        obj.tab_root_privileged_QStringList_tablewidget.removeAt(count);
                        obj.tab_user_iterated_QStringList_tablewidget.removeAt(count);
                        obj.tab_apple_metadata_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_html_report_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_xml_report_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_csv_report_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_kml_report_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_export_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_export_criteria_QStringList_tablewidget.removeAt(count);
                        obj.tab_report_html_header_QStringList_tablewidget.removeAt(count);
                        obj.tab_report_header_column_width_html_QStringList_tablewidget.removeAt(count);
                        obj.tab_g_search_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_timeline_available_QStringList_tablewidget.removeAt(count);
                        obj.tab_timeline_timestamp_type_QList_tablewidget.removeAt(count);
                        obj.tab_timeline_timestamp_column_name_QList_tablewidget.removeAt(count);
                        obj.tab_artifact_common_QStringList_tablewidget.removeAt(count);

                        obj.display_header_QList_tablewidget.removeAt(count);
                        obj.display_column_name_QList_tablewidget.removeAt(count);
                        obj.display_data_type_QList_tablewidget.removeAt(count);
                        obj.display_col_width_QList_tablewidget.removeAt(count);
                        obj.display_order_QList_tablewidget.removeAt(count);

                        obj.metadata_header_QList_tablewidget.removeAt(count);
                        obj.metadata_column_name_QList_tablewidget.removeAt(count);
                        obj.metadata_data_type_QList_tablewidget.removeAt(count);
                        obj.metadata_preview_QList_tablewidget.removeAt(count);

                        obj.local_search_column_name_QList_tablewidget.removeAt(count);
                        obj.export_path_column_name_QList_tablewidget.removeAt(count);


                        obj.report_html_display_name_QList_tablewidget.removeAt(count);
                        obj.report_html_col_name_QList_tablewidget.removeAt(count);
                        obj.report_html_order_QList_tablewidget.removeAt(count);
                        obj.report_html_data_type_QList_tablewidget.removeAt(count);


                        obj.report_xml_display_name_QList_tablewidget.removeAt(count);
                        obj.report_xml_col_name_QList_tablewidget.removeAt(count);
                        obj.report_xml_order_QList_tablewidget.removeAt(count);
                        obj.report_xml_data_type_QList_tablewidget.removeAt(count);


                        obj.report_csv_display_name_QList_tablewidget.removeAt(count);
                        obj.report_csv_col_name_QList_tablewidget.removeAt(count);
                        obj.report_csv_order_QList_tablewidget.removeAt(count);
                        obj.report_csv_data_type_QList_tablewidget.removeAt(count);


                        obj.report_kml_display_name_QList_tablewidget.removeAt(count);
                        obj.report_kml_col_name_QList_tablewidget.removeAt(count);
                        obj.report_kml_order_QList_tablewidget.removeAt(count);
                        obj.report_kml_data_type_QList_tablewidget.removeAt(count);

                        obj.g_search_col_name_QList.removeAt(count);
                        obj.g_search_col_order_QList.removeAt(count);
                        obj.g_search_col_data_type_QList.removeAt(count);

                        obj.g_timeline_col_name_QList.removeAt(count);
                        obj.g_timeline_col_data_type_QList.removeAt(count);
                    }
                }


                for(int count = obj.tab_name_QStringList_textedit.size() -1; count >=0; count--)
                {

                    /// ********************************************
                    /// Loop is running backwards to remove entries
                    /// (Dont be confused loop is running on same list and removing from the same).
                    /// *******************************************

                    QString tmp_tab_str   = obj.tab_name_QStringList_textedit.at(count);

                    if(!associated_tabs_lst.contains(tmp_tab_str.trimmed()))
                    {

                        int index = obj.overall_tab_name_QStringList.indexOf(tmp_tab_str, 0);

                        obj.overall_tab_count_int--;
                        obj.overall_tab_order_QStringList.removeAt(index);
                        obj.overall_tab_name_QStringList.removeAt(index);
                        obj.overall_table_name_QStringList.removeAt(index);

                        obj.tab_count_int_textedit--;
                        obj.tab_name_QStringList_textedit.removeAt(count);
                        obj.table_name_QStringList_textedit.removeAt(count);
                        obj.tab_supported_os_scheme_QList_textedit.removeAt(count);
                        obj.tab_supported_software_QList_textedit.removeAt(count);
                        obj.tab_log_show_supported_QStringList_textedit.removeAt(count);
                        obj.tab_root_privileged_QStringList_textedit.removeAt(count);
                        obj.tab_user_iterated_QStringList_textedit.removeAt(count);
                        obj.tab_html_report_available_QStringList_textedit.removeAt(count);
                        obj.tab_xml_report_available_QStringList_textedit.removeAt(count);
                        obj.tab_csv_report_available_QStringList_textedit.removeAt(count);
                        obj.tab_report_html_header_QStringList_textedit.removeAt(count);
                        obj.tab_report_header_column_width_html_QStringList_textedit.removeAt(count);
                        obj.tab_artifact_common_QStringList_textedit.removeAt(count);

                        obj.display_column_name_QList_textedit.removeAt(count);
                        obj.display_data_type_QList_textedit.removeAt(count);

                        obj.report_html_col_name_QList_textedit.removeAt(count);
                        obj.report_html_datatype_QList_textedit.removeAt(count);


                    }
                }

                for(int count = obj.tab_name_QStringList_stack.size() -1; count >=0; count--)
                {
                    /// ********************************************
                    /// Loop is running backwards to remove entries
                    /// (Dont be confused loop is running on same list and removing from the same).
                    /// *******************************************


                    QString tmp_tab_str   = obj.tab_name_QStringList_stack.at(count);

                    if(!associated_tabs_lst.contains(tmp_tab_str.trimmed()))
                    {

                        int index = obj.overall_tab_name_QStringList.indexOf(tmp_tab_str, 0);

                        obj.overall_tab_count_int--;
                        obj.overall_tab_order_QStringList.removeAt(index);
                        obj.overall_tab_name_QStringList.removeAt(index);
                        obj.overall_table_name_QStringList.removeAt(index);

                        obj.tab_count_int_stack--;
                        obj.tab_name_QStringList_stack.removeAt(count);
                        obj.table_name_QStringList_stack.removeAt(count);
                        obj.tab_supported_os_scheme_QList_stack.removeAt(count);
                        obj.tab_supported_software_QList_stack.removeAt(count);
                        obj.tab_log_show_supported_QStringList_stack.removeAt(count);
                        obj.tab_root_privileged_QStringList_stack.removeAt(count);
                        obj.tab_user_iterated_QStringList_stack.removeAt(count);
                        obj.tab_html_report_available_QStringList_stack.removeAt(count);
                        obj.tab_xml_report_available_QStringList_stack.removeAt(count);
                        obj.tab_csv_report_available_QStringList_stack.removeAt(count);
                        obj.tab_report_html_header_QStringList_stack.removeAt(count);
                        obj.tab_report_header_column_width_html_QStringList_stack.removeAt(count);
                        obj.tab_artifact_common_QStringList_stack.removeAt(count);

                        obj.display_column_name_QList_stack.removeAt(count);
                        obj.display_data_type_QList_stack.removeAt(count);

                        obj.report_html_col_name_QList_stack.removeAt(count);
                        obj.report_html_datatype_QList_stack.removeAt(count);


                    }
                }

                for(int count = obj.tab_name_QStringList_tree.size() -1; count >=0; count--)
                {

                    /// ********************************************
                    /// Loop is running backwards to remove entries
                    /// (Dont be confused loop is running on same list and removing from the same).
                    /// *******************************************


                    QString tmp_tab_str   = obj.tab_name_QStringList_tree.at(count);

                    if(!associated_tabs_lst.contains(tmp_tab_str.trimmed()))
                    {

                        int index = obj.overall_tab_name_QStringList.indexOf(tmp_tab_str, 0);

                        obj.overall_tab_count_int--;
                        obj.overall_tab_order_QStringList.removeAt(index);
                        obj.overall_tab_name_QStringList.removeAt(index);
                        obj.overall_table_name_QStringList.removeAt(index);


                        obj.tab_count_int_tree--;
                        obj.table_name_QStringList_tree.removeAt(count);
                        obj.tab_name_QStringList_tree.removeAt(count);
                        obj.tab_supported_os_scheme_QList_tree.removeAt(count);
                        obj.tab_supported_software_QList_tree.removeAt(count);
                        obj.tab_log_show_supported_QStringList_tree.removeAt(count);
                        obj.tab_root_privileged_QStringList_tree.removeAt(count);
                        obj.tab_user_iterated_QStringList_tree.removeAt(count);


                        obj.display_header_QList_tree.removeAt(count);
                        obj.display_parent_child_QList_tree.removeAt(count);
                        obj.display_column_name_QList_tree.removeAt(count);
                        obj.display_data_type_QList_tree.removeAt(count);
                        obj.display_col_width_QList_tree.removeAt(count);

                        obj.metadata_header_QList_tree.removeAt(count);
                        obj.metadata_column_name_QList_tree.removeAt(count);
                        obj.metadata_data_type_QList_tree.removeAt(count);
                        obj.metadata_preview_QList_tree.removeAt(count);

                    }
                }


                for(int count = obj.tab_name_QStringList_thumbnail.size() -1; count >=0; count--)
                {

                    /// ********************************************
                    /// Loop is running backwards to remove entries
                    /// (Dont be confused loop is running on same list and removing from the same).
                    /// *******************************************

                    QString tmp_tab_str   = obj.tab_name_QStringList_thumbnail.at(count);

                    if(!associated_tabs_lst.contains(tmp_tab_str.trimmed()))
                    {
                        int index = obj.overall_tab_name_QStringList.indexOf(tmp_tab_str, 0);

                        obj.overall_tab_count_int--;
                        obj.overall_tab_order_QStringList.removeAt(index);
                        obj.overall_tab_name_QStringList.removeAt(index);
                        obj.overall_table_name_QStringList.removeAt(index);

                        obj.tab_count_int_thumbnail--;
                        obj.table_name_QStringList_thumbnail.removeAt(count);
                        obj.tab_name_QStringList_thumbnail.removeAt(count);
                        obj.tab_supported_os_scheme_QList_thumbnail.removeAt(count);
                        obj.tab_supported_software_QList_thumbnail.removeAt(count);
                        obj.tab_log_show_supported_QStringList_thumbnail.removeAt(count);
                        obj.tab_root_privileged_QStringList_thumbnail.removeAt(count);
                        obj.tab_user_iterated_QStringList_thumbnail.removeAt(count);

                    }
                }

                for(int count = obj.tab_name_QStringList_chat_viewer.size() -1; count >=0; count--)
                {
                    /// ********************************************
                    /// Loop is running backwards to remove entries
                    /// (Dont be confused loop is running on same list and removing from the same).
                    /// *******************************************


                    QString tmp_tab_str   = obj.tab_name_QStringList_chat_viewer.at(count);

                    if(!associated_tabs_lst.contains(tmp_tab_str.trimmed()))
                    {
                        int index = obj.overall_tab_name_QStringList.indexOf(tmp_tab_str, 0);

                        obj.overall_tab_count_int--;
                        obj.overall_tab_order_QStringList.removeAt(index);
                        obj.overall_tab_name_QStringList.removeAt(index);
                        obj.overall_table_name_QStringList.removeAt(index);


                        obj.tab_count_int_chat_viewer--;
                        obj.table_name_QStringList_chat_viewer.removeAt(count);
                        obj.tab_name_QStringList_chat_viewer.removeAt(count);
                        obj.tab_supported_os_scheme_QList_chat_viewer.removeAt(count);
                        obj.tab_supported_software_QList_chat_viewer.removeAt(count);
                        obj.tab_log_show_supported_QStringList_chat_viewer.removeAt(count);
                        obj.tab_root_privileged_QStringList_chat_viewer.removeAt(count);
                        obj.tab_user_iterated_QStringList_chat_viewer.removeAt(count);

                        obj.display_column_name_QList_chat_viewer.removeAt(count);
                        obj.display_data_type_QList_chat_viewer.removeAt(count);


                    }
                }

                csv_info_list_selected.removeAt(i);
                csv_info_list_selected.insert(i,obj);

            }
        }

    }



    csv_info_list_final = csv_info_list_selected;

}

QString csv_reader::get_coloum_name_according_header_name(QString plugin_name, QString table_name, QString header_name)
{
    QString db_column_name;
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            int table_index =  csv_info_list_final.at(i).overall_table_name_QStringList.indexOf(table_name,0);
            if(table_index == -1)
                return db_column_name;


            QStringList header_list = csv_info_list_final.at(i).display_header_QList_tablewidget.at(table_index);
            int header_index = header_list.indexOf(header_name);
            if(header_index != -1)
                db_column_name =  csv_info_list_final.at(i).display_column_name_QList_tablewidget.at(table_index).at(header_index);

            break;
        }
    }

    return db_column_name;
}

bool csv_reader::is_tab_type_tablewidget(QString plugin_name, QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_tablewidget.size(); tt++)
            {
                if(csv_info_list_final.at(i).tab_name_QStringList_tablewidget.at(tt) == tab_name)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


QStringList csv_reader::get_header_list_of_tab(QString plugin_name, QString tab_name)
{
    recon_static_functions::debug_intensive("Start ",Q_FUNC_INFO);

    QStringList header_list;

    for(int itr = 0; itr < csv_info_list_final.size(); itr++)
    {
        if(csv_info_list_final.at(itr).plugin_name.trimmed() == plugin_name.trimmed())
        {
            int index_no = csv_info_list_final.at(itr).tab_name_QStringList_tablewidget.indexOf(tab_name,0);
            if(index_no != -1)
                header_list = csv_info_list_final.at(itr).display_header_QList_tablewidget.at(index_no);

            break;
        }
    }

    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

    return header_list;
}

QStringList csv_reader::get_supported_os_scheme_list_of_plugin(QString plugin_name)
{
    QStringList os_list;
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            os_list = csv_info_list_final.at(i).plugin_supported_os_QStringList;
            return os_list;
        }
    }
    return os_list;
}

QStringList csv_reader::get_native_os_list(QString plugin_name)
{
    QStringList os_list;
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            os_list = csv_info_list_final.at(i).plugin_native_os_QStringList;
            return os_list;
        }
    }
    return os_list;
}

bool csv_reader::is_tab_type_textedit(QString plugin_name,QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_textedit.size() ; tt++)
            {

                if(csv_info_list_final.at(i).tab_name_QStringList_textedit.at(tt) == tab_name)
                {
                    return true;
                }

            }
        }
    }

    return false;
}

bool csv_reader::is_plugin_available_for_chat_view(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.size() >= 1)
                {
                    return true;
                }

            }
        }
    }

    return false;
}

QString csv_reader::get_chat_view_tab_name(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.size() >= 1)
                {
                    // now supporting only single chat view per plugin
                    return csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.at(0);
                }

            }
        }
    }

    return "";
}

bool csv_reader::is_plugin_available_for_software(QString m_plugin_name, QString soft_name)
{

    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == m_plugin_name.trimmed())
        {
            if(csv_info_list_final.at(i).plugin_supported_software_QStringList.contains(soft_name))
                return true;
            else
                return false;
        }
    }

    return false;

}

bool csv_reader::is_plugin_available_for_os(QString m_plugin_name, QString os_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == m_plugin_name.trimmed())
        {
            if(csv_info_list_final.at(i).plugin_supported_os_QStringList.contains(os_name))
                return true;
            else
                return false;
        }
    }

    return false;

}

bool csv_reader::is_plugin_log_show_supported(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            if(csv_info_list_final.at(i).plugin_log_show_supported == "1")
                return true;
            else
                return false;
        }
    }

    return false;

}

bool csv_reader::is_tab_type_chat_view(QString plugin_name,QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_name_QStringList_chat_viewer.at(tt) == tab_name)
                {
                    return true;
                }

            }
        }
    }

    return false;
}

bool csv_reader::is_tab_type_tree_view(QString plugin_name,QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_tree.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_name_QStringList_tree.at(tt) == tab_name)
                {
                    return true;
                }

            }
        }
    }

    return false;
}

bool csv_reader::is_tab_type_thumbnail_view(QString plugin_name,QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_thumbnail.size() ; tt++)
            {
                if(csv_info_list_final.at(i).tab_name_QStringList_thumbnail.at(tt) == tab_name)
                {
                    return true;
                }

            }
        }
    }

    return false;
}

bool csv_reader::is_plugin_volatile(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            if(csv_info_list_final.at(i).plugin_extraction_mode == "Volatile")
                return true;
            else
                return false;
        }
    }

    return false;
}

bool csv_reader::is_plugin_available_for_redefined_result(QString plugin_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            if(csv_info_list_final.at(i).plugin_available_for_redefined_result == "1")
                return true;
            else
                return false;
        }
    }

    return false;
}


bool csv_reader::is_tab_type_stackwidget(QString plugin_name,QString tab_name)
{
    for(int i = 0; i < csv_info_list_final.size(); i++)
    {
        if(csv_info_list_final.at(i).plugin_name.trimmed() == plugin_name.trimmed())
        {
            for(int  tt = 0 ; tt < csv_info_list_final.at(i).tab_name_QStringList_stack.size() ; tt++)
            {

                if(csv_info_list_final.at(i).tab_name_QStringList_stack.at(tt) == tab_name)
                {
                    return true;
                }

            }
        }
    }

    return false;
}

QStringList csv_reader::get_all_full_tab_list_associated_with_table(QString plugin_name_str , QString table_name_str)
{
    // this function must be private

    QStringList associate_tabs_list;

    QList<struct_global_csv_reader_full_csv_info>backup_csv_info_list = csv_info_list_full;

    for(int i = 0; i < backup_csv_info_list.size(); i++)
    {
        if(backup_csv_info_list.at(i).plugin_name.trimmed() == plugin_name_str.trimmed())
        {

            struct_global_csv_reader_full_csv_info obj;
            obj = backup_csv_info_list.at(i);

            for(int pp = obj.overall_tab_name_QStringList.size() -1; pp >=0; pp--)
            {
                QString tmp_table_name_str = obj.overall_table_name_QStringList.at(pp);

                if(table_name_str == tmp_table_name_str)
                    associate_tabs_list << obj.overall_tab_name_QStringList.at(pp);

            }

            return associate_tabs_list;
        }
    }

    return associate_tabs_list;

}

QStringList csv_reader::get_all_tab_list_associated_with_table(QString plugin_name_str , QString table_name_str)
{
    QStringList associate_tabs_list;

    QList<struct_global_csv_reader_full_csv_info>backup_csv_info_list = csv_info_list_final;

    for(int i = 0; i < backup_csv_info_list.size(); i++)
    {
        if(backup_csv_info_list.at(i).plugin_name.trimmed() == plugin_name_str.trimmed())
        {

            struct_global_csv_reader_full_csv_info obj;
            obj = backup_csv_info_list.at(i);

            for(int pp = obj.overall_tab_name_QStringList.size() -1; pp >=0; pp--)
            {
                QString tmp_table_name_str = obj.overall_table_name_QStringList.at(pp);

                if(table_name_str == tmp_table_name_str)
                    associate_tabs_list << obj.overall_tab_name_QStringList.at(pp);

            }

            return associate_tabs_list;
        }
    }

    return associate_tabs_list;

}

QString csv_reader::get_tab_name_associated_with_widget(QString plugin_name_str , QString table_name_str , QString widget_type)
{
    QString associate_tabs;

    QList<struct_global_csv_reader_full_csv_info>backup_csv_info_list = csv_info_list_final;

    for(int ii = 0; ii < backup_csv_info_list.size(); ii++)
    {
        if(backup_csv_info_list.at(ii).plugin_name.trimmed() == plugin_name_str.trimmed())
        {

            struct_global_csv_reader_full_csv_info obj = backup_csv_info_list.at(ii);

            if(widget_type == MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET)
            {
                for(int  tt = 0 ; tt < obj.table_name_QStringList_tablewidget.size() ; tt++)
                {
                    if(obj.table_name_QStringList_tablewidget.at(tt) == table_name_str)
                    {
                        associate_tabs = obj.tab_name_QStringList_tablewidget.at(tt);
                        break;
                    }
                }
            }
            else if(widget_type == MACRO_CSV_TEMPLATE_TAB_TYPE_TEXTEDIT)
            {
                for(int  tt = 0 ; tt < obj.table_name_QStringList_textedit.size() ; tt++)
                {
                    if(obj.table_name_QStringList_textedit.at(tt) == table_name_str)
                    {
                        associate_tabs = obj.tab_name_QStringList_textedit.at(tt);
                        break;
                    }
                }
            }
            else if(widget_type == MACRO_CSV_TEMPLATE_TAB_TYPE_STACK)
            {
                for(int  tt = 0 ; tt < obj.table_name_QStringList_stack.size() ; tt++)
                {
                    if(obj.table_name_QStringList_stack.at(tt) == table_name_str)
                    {
                        associate_tabs = obj.tab_name_QStringList_stack.at(tt);
                        break;
                    }
                }
            }
            else if(widget_type == MACRO_CSV_TEMPLATE_TAB_TYPE_TREE)
            {
                for(int  tt = 0 ; tt < obj.table_name_QStringList_tree.size() ; tt++)
                {
                    if(obj.table_name_QStringList_tree.at(tt) == table_name_str)
                    {
                        associate_tabs = obj.tab_name_QStringList_tree.at(tt);
                        break;
                    }
                }
            }
            else if(widget_type == MACRO_CSV_TEMPLATE_TAB_TYPE_THUMBNAIL)
            {
                for(int  tt = 0 ; tt < obj.table_name_QStringList_thumbnail.size() ; tt++)
                {
                    if(obj.table_name_QStringList_thumbnail.at(tt) == table_name_str)
                    {
                        associate_tabs = obj.tab_name_QStringList_thumbnail.at(tt);
                        break;
                    }
                }
            }
            else if(widget_type == MACRO_CSV_TEMPLATE_TAB_TYPE_CHAT_VIEWER)
            {
                for(int  tt = 0 ; tt < obj.table_name_QStringList_chat_viewer.size() ; tt++)
                {
                    if(obj.table_name_QStringList_chat_viewer.at(tt) == table_name_str)
                    {
                        associate_tabs = obj.tab_name_QStringList_chat_viewer.at(tt);
                        break;
                    }
                }
            }

            break;
        }
    }

    return associate_tabs;

}

int csv_reader::get_tab_index_as_per_tabwidget(QString plugin_name_str, QString tab_name_str)
{
    int tab_index = -1;

    QList<struct_global_csv_reader_full_csv_info>backup_csv_info_list = csv_info_list_final;

    for(int ii = 0; ii < backup_csv_info_list.size(); ii++)
    {
        if(backup_csv_info_list.at(ii).plugin_name.trimmed() == plugin_name_str.trimmed())
        {
            struct_global_csv_reader_full_csv_info obj = backup_csv_info_list.at(ii);

            if(is_tab_type_tablewidget(plugin_name_str,tab_name_str))
            {
                for(int  tt = 0 ; tt < obj.tab_name_QStringList_tablewidget.size() ; tt++)
                {
                    if(obj.tab_name_QStringList_tablewidget.at(tt) == tab_name_str)
                    {
                        tab_index = tt;
                        break;
                    }
                }
            }
            else if(is_tab_type_textedit(plugin_name_str,tab_name_str))
            {
                for(int  tt = 0 ; tt < obj.tab_name_QStringList_textedit.size() ; tt++)
                {
                    if(obj.tab_name_QStringList_textedit.at(tt) == tab_name_str)
                    {
                        tab_index = tt;
                        break;
                    }
                }
            }
            else if(is_tab_type_stackwidget(plugin_name_str,tab_name_str))
            {
                for(int  tt = 0 ; tt < obj.tab_name_QStringList_stack.size() ; tt++)
                {
                    if(obj.tab_name_QStringList_stack.at(tt) == tab_name_str)
                    {
                        tab_index = tt;
                        break;
                    }
                }
            }
            else if(is_tab_type_tree_view(plugin_name_str,tab_name_str))
            {
                for(int  tt = 0 ; tt < obj.tab_name_QStringList_tree.size() ; tt++)
                {
                    if(obj.tab_name_QStringList_tree.at(tt) == tab_name_str)
                    {
                        tab_index = tt;
                        break;
                    }
                }
            }
            else if(is_tab_type_thumbnail_view(plugin_name_str,tab_name_str))
            {
                for(int  tt = 0 ; tt < obj.tab_name_QStringList_thumbnail.size() ; tt++)
                {
                    if(obj.tab_name_QStringList_thumbnail.at(tt) == tab_name_str)
                    {
                        tab_index = tt;
                        break;
                    }
                }
            }
            else if(is_tab_type_chat_view(plugin_name_str,tab_name_str))
            {
                for(int  tt = 0 ; tt < obj.tab_name_QStringList_chat_viewer.size() ; tt++)
                {
                    if(obj.tab_name_QStringList_chat_viewer.at(tt) == tab_name_str)
                    {
                        tab_index = tt;
                        break;
                    }
                }
            }

            break;
        }
    }

    return tab_index;
}

QStringList csv_reader::get_supported_os_scheme_list_of_tab(QString plugin_name, QString tab_name)
{
    QStringList tab_supported_os_list;

    for(int ii = 0; ii < csv_info_list_final.size(); ii++)
    {
        struct_global_csv_reader_full_csv_info csv_rdr_obj = csv_info_list_final.at(ii);

        if(csv_rdr_obj.plugin_name == plugin_name)
        {
            if(is_tab_type_tablewidget(plugin_name, tab_name))
            {
                int yy = csv_rdr_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
                tab_supported_os_list = csv_rdr_obj.tab_supported_os_scheme_QList_tablewidget.at(yy);
            }
            else if(is_tab_type_textedit(plugin_name,tab_name))
            {
                int yy = csv_rdr_obj.tab_name_QStringList_textedit.indexOf(tab_name);
                tab_supported_os_list = csv_rdr_obj.tab_supported_os_scheme_QList_textedit.at(yy);
            }
            else if(is_tab_type_stackwidget(plugin_name,tab_name))
            {
                int yy = csv_rdr_obj.tab_name_QStringList_stack.indexOf(tab_name);
                tab_supported_os_list = csv_rdr_obj.tab_supported_os_scheme_QList_stack.at(yy);
            }
            else if(is_tab_type_tree_view(plugin_name,tab_name))
            {
                int yy = csv_rdr_obj.tab_name_QStringList_tree.indexOf(tab_name);
                tab_supported_os_list = csv_rdr_obj.tab_supported_os_scheme_QList_tree.at(yy);
            }
            else if(is_tab_type_thumbnail_view(plugin_name,tab_name))
            {
                int yy = csv_rdr_obj.tab_name_QStringList_thumbnail.indexOf(tab_name);
                tab_supported_os_list = csv_rdr_obj.tab_supported_os_scheme_QList_thumbnail.at(yy);
            }
            else if(is_tab_type_chat_view(plugin_name,tab_name))
            {
                int yy = csv_rdr_obj.tab_name_QStringList_chat_viewer.indexOf(tab_name);
                tab_supported_os_list = csv_rdr_obj.tab_supported_os_scheme_QList_chat_viewer.at(yy);
            }

            break;
        }
    }

    return  tab_supported_os_list;
}

