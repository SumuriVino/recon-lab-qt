#include "csv_reader/csv_reader.h"

void csv_reader::generate_tableinfo_tree()
{
    for(int i = 0 ; i < master_list_tree.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        struct_global_csv_reader_table_detail struct_obj = master_list_tree.at(i);

        table_name_list_tree << struct_obj.table_name;
        tab_name_list_tree << struct_obj.tab_name;

        tab_supported_os_scheme_Qlist_tree << struct_obj.supported_os_scheme_QStringList;
        tab_supported_software_Qlist_tree << struct_obj.supported_software_QStringList;
        tab_log_show_supported_list_tree << struct_obj.log_show_supported;
        tab_root_privileged_list_tree << struct_obj.root_privileged;
        tab_user_iterated_list_tree << struct_obj.dir_iterated;

    }
}


void csv_reader::generate_columninfo_display_tree()
{
    for(int i = 0 ; i < master_list_tree.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tree.at(i);
        QList<struct_global_csv_reader_cols_detail> col_list =  table_struct_obj.cols_QList;

        QStringList header_list;
        QStringList col_name_list;
        QStringList order_list;
        QStringList data_type_list;
        QStringList tmp_col_width_list;

        QStringList col_parent_child_type;


        for(int j = 0 ; j < col_list.size() ; j++)
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();


            struct_global_csv_reader_cols_detail col_struct_obj = col_list.at(j);
            QString visible = col_struct_obj.col_tw_visible;

            if(visible != QString("1"))
                continue;

            header_list << col_struct_obj.col_tw_name;
            col_parent_child_type << col_struct_obj.col_parent_child_type;
            col_name_list << col_struct_obj.col_name;
            order_list << col_struct_obj.col_tw_order;
            data_type_list << col_struct_obj.col_tw_data_type;
            tmp_col_width_list << col_struct_obj.col_tw_col_width;


        }


        sort_tw_md_rt_list_tree(order_list, col_name_list, data_type_list, header_list, tmp_col_width_list ,col_parent_child_type);

        display_header_list_tree << header_list;
        display_parent_child_list_tree << col_parent_child_type;
        display_column_name_list_tree << col_name_list;
        display_data_type_list_tree << data_type_list;
        display_col_width_list_tree << tmp_col_width_list;
    }

}

void csv_reader::generate_columninfo_tree_detailedinfo()
{
    for(int i = 0 ; i < master_list_tree.size() ; i++)
    {
        struct_global_csv_reader_table_detail table_struct_obj = master_list_tree.at(i);
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
                continue;

            md_col_display_name_list << col_struct_obj.col_md_name;
            md_col_name_list << col_struct_obj.col_name;
            md_order_list << col_struct_obj.col_md_order;
            md_type_list << col_struct_obj.col_md_data_type;

            if(col_struct_obj.col_md_preview == QString("1"))
                tmp_md_preview_col_list << col_struct_obj.col_name;

        }

        sort_tw_md_rt_list(md_order_list, md_col_name_list, md_type_list, md_col_display_name_list);

        md_header_list_tree << md_col_display_name_list;
        md_column_name_list_tree << md_col_name_list;
        md_data_type_list_tree << md_type_list;
        md_preview_list_tree << tmp_md_preview_col_list;

    }

}
