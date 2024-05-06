#include "csv_list_maker.h"

csv_list_maker::csv_list_maker(QWidget *parent)
{
}


int csv_list_maker::initialize_data(QString file_path)
{ // intilise and validate the csv path and data
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    QString new_path = recon_static_functions::decrypt_file_1(file_path, Q_FUNC_INFO);
    QFile file(new_path);
    if(!file.open(QIODevice::ReadOnly))
    {

        recon_static_functions::app_debug(" file open ------FAILED-----" + new_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(file.errorString(),Q_FUNC_INFO);
        return -1;
    }

    QFileInfo info(new_path);
    csv_file_name = info.fileName();
    csv_file_path = info.filePath();

    QString file_line_list_str = QString::fromLocal8Bit(file.readAll().trimmed());
    file_line_list << file_line_list_str.split("\n");

    file.close();
    file.remove();

    if(parse_plugin_pub_info() == -1)
    {
        recon_static_functions::app_debug(" ---FAILED--- parse_plugin_pub_info() improper output, csv_file_name " + csv_file_name,Q_FUNC_INFO);
        return -1;
    }

    if(extract_tables_start_offset() == -1)
    {
        recon_static_functions::app_debug(" ---FAILED--- extract_tables_start_offset() improper output, csv_file_name " + csv_file_name,Q_FUNC_INFO);
        return -1;
    }

    for(int k = 0 ; k < tables_start_offset_list.size() ; k++)
    {
        parse_table(tables_start_offset_list.at(k));
    }

    set_tab_order();



    clear_variables_computed_info_plugin(st_computed_info_plugin_obj);

    for (int i = 0; i < (csv_table_info_list.size()); i++)
    {
        csv_table_info at_j = csv_table_info_list.at(i);

        st_computed_info_plugin_obj.tab_count_int = i;
        st_computed_info_plugin_obj.tab_name_QStringList << at_j.get_tab_name();
        st_computed_info_plugin_obj.table_name_QStringList << at_j.get_table_name();
        st_computed_info_plugin_obj.tab_order_QStringList << at_j.get_table_order();
    }

    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

    return 0;
}



int csv_list_maker::extract_tables_start_offset()
{ // extract tablenames from csv's.
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    tables_start_offset_list.clear();

    for(int i = 0 ; i < file_line_list.size() ; i++)
    {
        if(file_line_list.at(i).startsWith("OSSTable Name")) //Table start tag in template file
            tables_start_offset_list << i;
    }




    if(tables_start_offset_list.size() <= 0)
    {
        recon_static_functions::app_debug("  ---FAILED--- any 'OSSTable Name' keyword not found, csv_file_name " + csv_file_name,Q_FUNC_INFO);
        recon_static_functions::app_debug(" csv_file_path " + csv_file_path,Q_FUNC_INFO);
        return -1;
    }


    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

    return 0;
}



int csv_list_maker::parse_table(int index)
{ // parse all the tables from csv
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    csv_table_info table_info_obj ;
    table_info_obj.set_table_attribute(file_line_list.at(index+1));

    recon_static_functions::debug_intensive(" table attribute parsed at index " + QString::number(index),Q_FUNC_INFO);


    int start_pos = index+3;


    while(1)
    {
        QString line = file_line_list.at(start_pos);
        if(line.startsWith("END==")) //Table end tag in template file
            break;
        table_info_obj.set_column_attribute(line);
        start_pos++;
    }

    recon_static_functions::debug_intensive(" column attribute parsed ",Q_FUNC_INFO);

    QString visible = table_info_obj.get_table_visibility();
    visible = visible.trimmed();
    if(visible != QString("1"))
    {
        recon_static_functions::debug_intensive(" end with table not visible ",Q_FUNC_INFO);
        return start_pos;
    }

    QStringList sft_list = table_info_obj.get_table_tab_supported_software();
    //    if(!sft_list.contains(software_name))
    //    {
    //        recon_static_functions::debug_intensive(" end with table not ssupported software ",Q_FUNC_INFO);
    //        return start_pos;
    //    }


    csv_table_info_list << table_info_obj;

    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

    return start_pos;
}


void csv_list_maker::set_tab_order()
{ // set tab order from csv
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    csv_table_info temp_order;
    int i, j;

    for (i = 0; i < (csv_table_info_list.size() - 1); ++i)
    {
        if(i%10 == 0)
            QCoreApplication::processEvents();

        for (j = 0; j < csv_table_info_list.size() - 1 - i; ++j )
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            csv_table_info at_j = csv_table_info_list.at(j);
            csv_table_info at_j_1 = csv_table_info_list.at(j+1);

            QString order_tmp_j = at_j.get_table_order();
            QString order_tmp_j_1 = at_j_1.get_table_order();

            if (order_tmp_j.toInt() > order_tmp_j_1.toInt())
            {
                temp_order = at_j_1;
                csv_table_info_list.replace(j+1,at_j);
                csv_table_info_list.replace(j,temp_order);
            }
        }
    }


    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

}

int csv_list_maker::parse_plugin_pub_info()
{ // parse plugin info from csv
    recon_static_functions::debug_intensive("start " ,Q_FUNC_INFO);


    clear_variables_pub_info_plugin(st_pub_info_plugin_obj);

    int plugin_info_offset = -1;

    for(int i = 0 ; i < file_line_list.size() ; i++)
    {
        if(file_line_list.at(i).startsWith("OSSPlugin Name"))
        {
            plugin_info_offset = i;
            break;
        }
    }
    if(plugin_info_offset == -1)
    {
        recon_static_functions::app_debug("  ---FAILED--- 'OSSPlugin Name' keyword not found, csv_file_name " + csv_file_name,Q_FUNC_INFO);
        recon_static_functions::app_debug("csv_file_path " + csv_file_path,Q_FUNC_INFO);
        return -1;
    }
    csv_plugin_info plugin_pub_info_obj;
    plugin_pub_info_obj.set_plugin_attribute(file_line_list.at(plugin_info_offset+1));

    st_pub_info_plugin_obj.plugin_name =  plugin_pub_info_obj.get_plugin_name();
    st_pub_info_plugin_obj.plugin_category =  plugin_pub_info_obj.get_plugin_category();
    st_pub_info_plugin_obj.native_os_QStringList =  plugin_pub_info_obj.get_native_os();
    st_pub_info_plugin_obj.supported_os_scheme_QStringList =  plugin_pub_info_obj.get_supported_os_scheme();
    st_pub_info_plugin_obj.extraction_mode =  plugin_pub_info_obj.get_extraction_mode();
    st_pub_info_plugin_obj.available_for_redefined_result =  plugin_pub_info_obj.get_redefined_result_availability();
    st_pub_info_plugin_obj.supported_software_QStringList =  plugin_pub_info_obj.get_supported_software();
    st_pub_info_plugin_obj.log_show_supported =  plugin_pub_info_obj.get_log_show_status();
    st_pub_info_plugin_obj.root_privileged =  plugin_pub_info_obj.get_root_privileged();
    st_pub_info_plugin_obj.dir_iterated =  plugin_pub_info_obj.get_user_iterated();

    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

    return 0;
}

void csv_list_maker::clear_variables_pub_info_plugin(struct_global_csv_reader_pub_info_plugin &struct_pub_plgn_info)
{ // clear structure
    struct_pub_plgn_info.plugin_name.clear();
    struct_pub_plgn_info.plugin_category.clear();
    struct_pub_plgn_info.native_os_QStringList.clear();
    struct_pub_plgn_info.supported_os_scheme_QStringList.clear();
    struct_pub_plgn_info.extraction_mode.clear();
    struct_pub_plgn_info.available_for_redefined_result.clear();
    struct_pub_plgn_info.supported_software_QStringList.clear();
    struct_pub_plgn_info.log_show_supported.clear();
    struct_pub_plgn_info.root_privileged.clear();
    struct_pub_plgn_info.dir_iterated.clear();
}

void csv_list_maker::clear_variables_computed_info_plugin(struct_global_csv_reader_computed_info_plugin &struct_computed_plgn_info)
{ // clear structure
    struct_computed_plgn_info.tab_count_int = 0;

    struct_computed_plgn_info.tab_name_QStringList.clear();
    struct_computed_plgn_info.tab_order_QStringList.clear();
    struct_computed_plgn_info.table_name_QStringList.clear();
}

struct_global_csv_reader_pub_info_plugin csv_list_maker::get_plugin_pub_detail()
{ // return plugin info structure object
    return st_pub_info_plugin_obj;
}

struct_global_csv_reader_computed_info_plugin csv_list_maker::get_plugin_computed_detail()
{ //return plugin tab details structure object
    return st_computed_info_plugin_obj;
}

QList<struct_global_csv_reader_table_detail> csv_list_maker::get_table_details(QString tab_type)
{ // get below table details from csv
    recon_static_functions::debug_intensive("start " + QString::number(csv_table_info_list.size()),Q_FUNC_INFO);



    QList<struct_global_csv_reader_table_detail> list;

    for(int y = 0 ; y < csv_table_info_list.size() ; y++)
    {
        if(y%10 == 0)
            QCoreApplication::processEvents();

        csv_table_info table = csv_table_info_list.at(y);
        if(table.get_table_tab_widget_type() != tab_type)
            continue;

        struct_global_csv_reader_table_detail struct_obj;
        struct_obj.table_name = table.get_table_name();
        struct_obj.table_tab_widget_type = table.get_table_tab_widget_type();
        struct_obj.tab_name = table.get_tab_name();
        struct_obj.supported_os_scheme_QStringList = table.get_table_tab_supported_os();
        struct_obj.supported_software_QStringList = table.get_table_tab_supported_software();
        struct_obj.log_show_supported = table.get_log_show_supported();
        struct_obj.root_privileged = table.get_root_privileged();
        struct_obj.dir_iterated = table.get_user_iterated();
        struct_obj.apple_metadata_available = table.get_apple_metadata();

        struct_obj.html_report_available = table.get_table_html_report();
        struct_obj.xml_report_available = table.get_table_xml_report();
        struct_obj.csv_report_available = table.get_table_csv_report();
        struct_obj.kml_report_available = table.get_table_kml_report();

        struct_obj.table_export_available = table.get_table_export_available();

        struct_obj.table_html_header = table.get_table_html_header();
        struct_obj.table_header_column_width_html = table.get_table_header_column_width_html();

        struct_obj.table_g_search_available = table.get_table_g_search_available();

        struct_obj.table_timeline_available = table.get_table_timeline_available();
        struct_obj.table_timeline_timestamp_type_QStringList = table.get_table_timeline_type();
        struct_obj.table_timeline_timestamp_column_name_QStringList = table.get_table_timeline_column_name();


        struct_obj.table_artifact_common = table.get_artifact_common();


        QList<csv_column_info> cols_list;
        cols_list = table.get_cols_list();

        QList<struct_global_csv_reader_cols_detail> table_cols_list;

        for(int i = 0 ; i < cols_list.size() ; i++)
        {
            if(i%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail cols_obj;
            csv_column_info column = cols_list.at(i);

            cols_obj.col_name = column.get_column_name();
            cols_obj.col_parent_child_type = column.get_column_parent_child_type();
            cols_obj.col_tw_name = column.get_column_tw_display_name();
            cols_obj.col_tw_order = column.get_column_tw_order();
            cols_obj.col_tw_col_width = column.get_column_tw_col_width();
            cols_obj.col_tw_visible = column.get_column_tw_visibility();
            cols_obj.col_tw_data_type = column.get_column_tw_data_type();

            cols_obj.col_md_name = column.get_column_md_name();
            cols_obj.col_md_order = column.get_column_md_order();
            cols_obj.col_md_data_type = column.get_column_md_data_type();
            cols_obj.col_md_visible = column.get_column_md_visiblity();
            cols_obj.col_search_local = column.get_search_local_field();
            cols_obj.col_md_preview = column.get_md_preview();

            cols_obj.col_export_path = column.get_export_path();

            cols_obj.col_report_html_name = column.get_html_name();
            cols_obj.col_report_html_visible = column.get_html_visible();
            cols_obj.col_report_html_order = column.get_html_order();
            cols_obj.col_report_html_data_type = column.get_html_report_data_type();

            cols_obj.col_report_xml_name = column.get_xml_name();
            cols_obj.col_report_xml_visible = column.get_xml_visible();
            cols_obj.col_report_xml_order = column.get_xml_order();
            cols_obj.col_report_xml_data_type = column.get_xml_report_data_type();


            cols_obj.col_report_csv_name = column.get_csv_name();
            cols_obj.col_report_csv_visible = column.get_csv_visible();
            cols_obj.col_report_csv_order = column.get_csv_order();
            cols_obj.col_report_csv_data_type = column.get_csv_report_data_type();

            cols_obj.col_report_kml_name = column.get_kml_name();
            cols_obj.col_report_kml_visible = column.get_kml_visible();
            cols_obj.col_report_kml_order = column.get_kml_order();
            cols_obj.col_report_kml_data_type = column.get_kml_report_data_type();

            cols_obj.col_g_search_visible = column.get_g_search_visible();
            cols_obj.col_g_search_order = column.get_g_search_order();
            cols_obj.col_g_search_data_type = column.get_g_search_data_type();

            cols_obj.col_g_timeline_visible = column.get_timeline_visible();
            cols_obj.col_g_timeline_order = column.get_timeline_order();
            cols_obj.col_g_timeline_data_type = column.get_timeline_data_type();

            table_cols_list << cols_obj;
        }

        struct_obj.cols_QList = table_cols_list;

        list << struct_obj;

    }


    recon_static_functions::debug_intensive("end " ,Q_FUNC_INFO);

    return list;
}

