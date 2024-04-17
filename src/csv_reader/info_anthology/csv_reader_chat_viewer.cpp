#include "csv_reader/csv_reader.h"

void csv_reader::generate_tableinfo_chat_viewer()
{
    for(int i = 0 ; i < master_list_chat_viewer.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        struct_global_csv_reader_table_detail struct_obj = master_list_chat_viewer.at(i);

        table_name_list_chat_viewer << struct_obj.table_name;
        tab_name_list_chat_viewer << struct_obj.tab_name;

        tab_supported_os_scheme_Qlist_chat_viewer<< struct_obj.supported_os_scheme_QStringList;
        tab_supported_software_Qlist_chat_viewer << struct_obj.supported_software_QStringList;

        tab_log_show_supported_list_chat_viewer << struct_obj.log_show_supported;
        tab_root_privileged_list_chat_viewer << struct_obj.root_privileged;
        tab_user_iterated_list_chat_viewer << struct_obj.dir_iterated;

    }

}

void csv_reader::generate_columninfo_display_chat_viewer()
{
    for(int i = 0 ; i < master_list_chat_viewer.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_chat_viewer.at(i);
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

            if((visible != QString("1")))
                continue;

            header_list << col_struct_obj.col_tw_name;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_tw_order;
            data_type_list << col_struct_obj.col_tw_data_type;
            tmp_col_width_list << col_struct_obj.col_tw_col_width;

        }

        sort_tw_md_rt_list(order_list, col_name_list, data_type_list, header_list, tmp_col_width_list);

        display_column_name_list_chat_viewer << col_name_list;
        display_data_type_list_chat_viewer << data_type_list;

    }

}

