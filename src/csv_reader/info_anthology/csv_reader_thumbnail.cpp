#include "csv_reader/csv_reader.h"

void csv_reader::generate_tableinfo_thumbnail()
{

    for(int i = 0 ; i < master_list_thumbnail.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        struct_global_csv_reader_table_detail struct_obj = master_list_thumbnail.at(i);

        table_name_list_thumbnail << struct_obj.table_name;
        tab_name_list_thumbnail << struct_obj.tab_name;

        tab_supported_os_scheme_Qlist_thumbnail << struct_obj.supported_os_scheme_QStringList;
        tab_supported_software_Qlist_thumbnail << struct_obj.supported_software_QStringList;
        tab_log_show_supported_list_thumbnail << struct_obj.log_show_supported;
        tab_root_privileged_list_thumbnail << struct_obj.root_privileged;
        tab_user_iterated_list_thumbnail << struct_obj.dir_iterated;

    }
}

