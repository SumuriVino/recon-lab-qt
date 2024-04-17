#ifndef CSV_LIST_MAKER_H
#define CSV_LIST_MAKER_H


#include <QtCore>
#include "csv_table_info.h"
#include "csv_plugin_info.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"



class csv_list_maker
{
public:
    csv_list_maker(QWidget *parent = 0);

    int initialize_data(QString file_path);
    QList<struct_global_csv_reader_table_detail>get_table_details(QString tab_type);
    struct_global_csv_reader_pub_info_plugin get_plugin_pub_detail();
    struct_global_csv_reader_computed_info_plugin get_plugin_computed_detail();

private:
    QStringList file_line_list;
    QList<int>tables_start_offset_list;
    QList<csv_table_info> csv_table_info_list;

    int extract_tables_start_offset();
    int parse_table(int index);
    void set_tab_order();
    int parse_plugin_pub_info();

    struct_global_csv_reader_pub_info_plugin st_pub_info_plugin_obj;
    struct_global_csv_reader_computed_info_plugin st_computed_info_plugin_obj;
    QString csv_file_name;
    QString csv_file_path;

    void clear_variables_pub_info_plugin(struct_global_csv_reader_pub_info_plugin &struct_pub_plgn_info);
    void clear_variables_computed_info_plugin(struct_global_csv_reader_computed_info_plugin &struct_computed_plgn_info);

};

#endif // CSV_LIST_MAKER_H
