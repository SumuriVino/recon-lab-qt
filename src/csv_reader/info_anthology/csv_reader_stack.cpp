#include "csv_reader/csv_reader.h"

void csv_reader::generate_tableinfo_stack()
{
    for(int i = 0 ; i < master_list_stack.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        struct_global_csv_reader_table_detail struct_obj = master_list_stack.at(i);

        table_name_list_stack << struct_obj.table_name;
        tab_name_list_stack << struct_obj.tab_name;
        tab_supported_os_scheme_Qlist_stack << struct_obj.supported_os_scheme_QStringList;
        tab_supported_software_Qlist_stack << struct_obj.supported_software_QStringList;
        tab_log_show_supported_list_stack << struct_obj.log_show_supported;
        tab_root_privileged_list_stack << struct_obj.root_privileged;
        tab_user_iterated_list_stack << struct_obj.dir_iterated;

        tab_html_report_available_list_stack << struct_obj.html_report_available;
        tab_xml_report_available_list_stack << struct_obj.xml_report_available;
        tab_csv_report_available_list_stack << struct_obj.csv_report_available;


        tab_report_html_header_list_stack << struct_obj.table_html_header;
        tab_report_header_column_width_html_list_stack << struct_obj.table_header_column_width_html;

        tab_artifact_common_list_stack << struct_obj.table_artifact_common;
    }

}

void csv_reader::generate_columninfo_display_stack()
{

    for(int i = 0 ; i < master_list_stack.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_stack.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;
        QStringList tmp_col_width_list;

        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();


            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_tw_visible;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_tw_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_tw_order;
            data_type_list << col_struct_obj.col_tw_data_type;
            tmp_col_width_list << col_struct_obj.col_tw_col_width;


        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list, tmp_col_width_list);

        display_column_name_list_stack << col_name_list;
        display_data_type_list_stack << data_type_list;
    }

}
void csv_reader::generate_columninfo_html_report_stack()
{
    for(int i = 0 ; i < master_list_stack.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_stack.at(i);
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

        report_html_col_name_list_stack << col_name_list;
        report_html_datatype_list_stack << data_type_list;


    }

}
