#include "csv_reader/csv_reader.h"

void csv_reader::generate_tableinfo_tablewidget()
{

    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        struct_global_csv_reader_table_detail struct_obj = master_list_tablewidget.at(i);

        table_name_list_tablewidget << struct_obj.table_name;
        tab_name_list_tablewidget << struct_obj.tab_name;

        tab_supported_os_scheme_Qlist_tablewidget << struct_obj.supported_os_scheme_QStringList;
        tab_supported_software_Qlist_tablewidget << struct_obj.supported_software_QStringList;
        tab_log_show_supported_list_tablewidget << struct_obj.log_show_supported;
        tab_root_privileged_list_tablewidget << struct_obj.root_privileged;
        tab_user_iterated_list_tablewidget << struct_obj.dir_iterated;

        tab_apple_metadata_available_list_tablewidget << struct_obj.apple_metadata_available;

        tab_html_report_available_list_tablewidget << struct_obj.html_report_available;
        tab_xml_report_available_list_tablewidget << struct_obj.xml_report_available;
        tab_csv_report_available_list_tablewidget << struct_obj.csv_report_available;
        tab_kml_report_available_list_tablewidget << struct_obj.kml_report_available;


        QStringList list = struct_obj.table_export_available.split(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR);

        if(list.at(0).trimmed() == "")
        {
            tab_export_available_list_tablewidget << "";
            tab_export_criteria_list_tablewidget << "";
        }
        else if(list.at(0).trimmed() == "1")
        {
            tab_export_available_list_tablewidget << list.at(0);
            if(list.size() >= 2)
            {
                tab_export_criteria_list_tablewidget << list.at(1);
            }
            else
                tab_export_criteria_list_tablewidget << "";

        }

        tab_report_html_header_list_tablewidget << struct_obj.table_html_header;
        tab_report_header_column_width_html_list_tablewidget << struct_obj.table_header_column_width_html;

        tab_g_search_available_list_tablewidget << struct_obj.table_g_search_available;

        tab_timeline_available_list_tablewidget << struct_obj.table_timeline_available;
        tab_timeline_type_QList_tablewidget << struct_obj.table_timeline_timestamp_type_QStringList;
        tab_timeline_column_name_QList_tablewidget << struct_obj.table_timeline_timestamp_column_name_QStringList;

        tab_artifact_common_list_tablewidget << struct_obj.table_artifact_common;
    }

}

void csv_reader::generate_columninfo_display_tablewidget()
{

    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;
        QStringList tmp_search_local_list;
        QStringList tmp_col_width_list;

        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();


            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_tw_visible;

            if(col_struct_obj.col_search_local == QString("1"))
                tmp_search_local_list << col_struct_obj.col_name;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_tw_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_tw_order;
            data_type_list << col_struct_obj.col_tw_data_type;
            tmp_col_width_list << col_struct_obj.col_tw_col_width;


        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list, tmp_col_width_list);

        display_header_list_tablewidget << header_list;
        display_column_name_list_tablewidget << col_name_list;
        display_data_type_list_tablewidget << data_type_list;
        ms_search_local_list_tablewidget << tmp_search_local_list;
        display_col_width_list_tablewidget << tmp_col_width_list;
        display_filtered_order_list_tablewidget << order_list;

    }

}

void csv_reader::generate_columninfo_tablewidget_detailedinfo()
{
    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList md_col_display_name_list;
        QStringList md_col_name_list;
        QStringList md_order_list;
        QStringList md_type_list;
        QStringList tmp_md_preview_col_list;
        QStringList tmp_export_path;

        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_md_visible;


            if(col_struct_obj.col_export_path == QString("1"))
            {
                tmp_export_path << col_struct_obj.col_name;
            }

            if(visible != QString("1"))
            {
                    continue;
            }

            md_col_display_name_list << col_struct_obj.col_md_name;
            md_col_name_list << col_struct_obj.col_name;
            md_order_list << col_struct_obj.col_md_order;
            md_type_list << col_struct_obj.col_md_data_type;


            if(col_struct_obj.col_md_preview == QString("1"))
                tmp_md_preview_col_list << col_struct_obj.col_name;

        }

        sort_tw_md_rt_list(md_order_list, md_col_name_list, md_type_list, md_col_display_name_list);

        md_header_list_tablewidget << md_col_display_name_list;
        md_column_name_list_tablewidget << md_col_name_list;
        md_data_type_list_tablewidget << md_type_list;
        md_preview_column_list_tablewidget << tmp_md_preview_col_list;

        export_path_column_list_tablewidget << tmp_export_path;

    }

}

void csv_reader::generate_columninfo_html_report_tablewidget()
{
    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;


        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_report_html_visible;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_report_html_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_report_html_order;
            data_type_list << col_struct_obj.col_report_html_data_type;

        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list);

        rt_html_d_name_list_tablewidget << header_list;
        rt_html_col_name_list_tablewidget << col_name_list;
        rt_html_data_type_list_tablewidget << data_type_list;
        rt_html_order_list_tablewidget << order_list;

    }

}
void csv_reader::generate_order_list_xml_report()
{
    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;


        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_report_xml_visible;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_report_xml_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_report_xml_order;
            data_type_list << col_struct_obj.col_report_xml_data_type;
        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list);

        rt_xml_d_name_list << header_list;
        rt_xml_col_name_list << col_name_list;
        rt_xml_data_type_list << data_type_list;
        rt_xml_order_list << order_list;
    }

}
void csv_reader::generate_order_list_csv_report()
{
    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;

        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_report_csv_visible;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_report_csv_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_report_csv_order;
            data_type_list << col_struct_obj.col_report_csv_data_type;
        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list);

        rt_csv_d_name_list << header_list;
        rt_csv_col_name_list << col_name_list;
        rt_csv_data_type_list << data_type_list;
        rt_csv_order_list << order_list;
    }

}
void csv_reader::generate_columninfo_kml_report()
{
    for(int i = 0 ; i < master_list_tablewidget.size(); i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;


        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_report_kml_visible;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_report_kml_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_report_kml_order;
            data_type_list << col_struct_obj.col_report_kml_data_type;

        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list);

        rt_kml_d_name_list << header_list;
        rt_kml_col_name_list << col_name_list;
        rt_kml_data_type_list << data_type_list;
        rt_kml_order_list << order_list;


    }

}

void csv_reader::generate_order_list_g_search_tablewidget()
{
    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);

        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;

        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_g_search_visible;

            if(visible != QString("1"))
                continue;

            header_list << "";
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_g_search_order;
            data_type_list << col_struct_obj.col_g_search_data_type;
        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list);

        g_search_col_name_list << col_name_list;
        g_search_col_order_list << order_list;
        g_search_col_data_type_list << data_type_list;
    }

}

void csv_reader::generate_order_list_timeline_tablewidget()
{
    for(int i = 0 ; i < master_list_tablewidget.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tablewidget.at(i);

        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;

        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_g_timeline_visible;

            if(visible != QString("1"))
                continue;

            header_list << "";
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_g_timeline_order;
            data_type_list << col_struct_obj.col_g_timeline_data_type;
        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list);

        g_timeline_col_name_list << col_name_list;
        g_timeline_col_data_type_list << data_type_list;
    }

}


