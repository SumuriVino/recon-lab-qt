#include "csv_reader.h"

csv_reader::csv_reader(QWidget *parent)
{

}

csv_reader::~csv_reader()
{

}


int csv_reader::make_master_lists(QString csv_path)
{
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    csv_list_maker list_maker_obj;
    if(list_maker_obj.initialize_data(csv_path) == -1)
    {
        recon_static_functions::app_debug(" Error found in initialize_data -------FAILED------ csv path " + csv_path,Q_FUNC_INFO);
        return -1;
    }

    struct_global_csv_reader_pub_info_plugin obj = list_maker_obj.get_plugin_pub_detail();
    plugin_csv_file_name = QFileInfo(csv_path).fileName();
    plugin_csv_file_path = QFileInfo(csv_path).filePath();
    plugin_name = obj.plugin_name;
    plugin_category = obj.plugin_category;
    plugin_supported_os_scheme_list = obj.supported_os_scheme_QStringList;
    plugin_native_os_list = obj.native_os_QStringList;
    plugin_extraction_mode = obj.extraction_mode;
    plugin_available_for_redefined_result = obj.available_for_redefined_result;
    plugin_supported_software_list = obj.supported_software_QStringList;
    plugin_log_show_supported = obj.log_show_supported;
    plugin_root_privileged = obj.root_privileged;
    plugin_user_iterated = obj.dir_iterated;


    struct_global_csv_reader_computed_info_plugin cmpt_obj = list_maker_obj.get_plugin_computed_detail();
    total_tab_count_int = cmpt_obj.tab_count_int;
    tab_order_QStringList = cmpt_obj.tab_order_QStringList;
    total_tab_name_QStringList = cmpt_obj.tab_name_QStringList;
    total_table_name_QStringList = cmpt_obj.table_name_QStringList;

    master_list_tablewidget =  list_maker_obj.get_table_details(MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);
    master_list_textedit = list_maker_obj.get_table_details(MACRO_CSV_TEMPLATE_TAB_TYPE_TEXTEDIT);
    master_list_stack = list_maker_obj.get_table_details(MACRO_CSV_TEMPLATE_TAB_TYPE_STACK);
    master_list_tree = list_maker_obj.get_table_details(MACRO_CSV_TEMPLATE_TAB_TYPE_TREE);
    master_list_thumbnail = list_maker_obj.get_table_details(MACRO_CSV_TEMPLATE_TAB_TYPE_THUMBNAIL);

    if(!global_variable_desktop_silicon_bool)
        master_list_chat_viewer = list_maker_obj.get_table_details(MACRO_CSV_TEMPLATE_TAB_TYPE_CHAT_VIEWER);

    recon_static_functions::debug_intensive(" Plugin Name " + plugin_name,Q_FUNC_INFO);
    recon_static_functions::debug_intensive(" tablewidget count " + QString::number(master_list_tablewidget.size()),Q_FUNC_INFO);
    recon_static_functions::debug_intensive(" textedit count " + QString::number(master_list_textedit.size()),Q_FUNC_INFO);
    recon_static_functions::debug_intensive(" stack count " + QString::number(master_list_tree.size()),Q_FUNC_INFO);
    recon_static_functions::debug_intensive(" thumbnail count " + QString::number(master_list_thumbnail.size()),Q_FUNC_INFO);
    recon_static_functions::debug_intensive(" bubble chat count " + QString::number(master_list_chat_viewer.size()),Q_FUNC_INFO);

    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

    return 1;
}


bool csv_reader::is_our_masterlist_and_GUItab_size_accurate()
{
    int tablewidget_count = master_list_tablewidget.size();
    int textedit_count = master_list_textedit.size();
    int stack_count = master_list_stack.size();

    if(tablewidget_count <= 0 && textedit_count <= 0 && stack_count <= 0)
    {
        recon_static_functions::app_debug(" Could not find any TAB type in csv  ---FAILED--- plugin -> " + plugin_name,Q_FUNC_INFO);
        return false;
    }


    recon_static_functions::debug_intensive(" done plugin name " + plugin_name,Q_FUNC_INFO);

    return true;
}

void csv_reader::generate_info()
{

    recon_static_functions::debug_intensive(" generate list tableinfo start, plugin name - " + plugin_name,Q_FUNC_INFO);
    generate_tableinfo_tablewidget();
    generate_tableinfo_textedit();
    generate_tableinfo_stack();
    generate_tableinfo_tree();
    generate_tableinfo_thumbnail();
    generate_tableinfo_chat_viewer();

    recon_static_functions::debug_intensive(" generate list tableinfo end, plugin name - " + plugin_name,Q_FUNC_INFO);


    recon_static_functions::debug_intensive(" generate list columninfo display start, plugin name - " + plugin_name,Q_FUNC_INFO);
    generate_columninfo_display_tablewidget();
    generate_columninfo_display_textedit();
    generate_columninfo_display_stack();
    generate_columninfo_display_tree();
    generate_columninfo_display_chat_viewer();

    recon_static_functions::debug_intensive(" generate list columninfo display end, plugin name - " + plugin_name,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" generate list columninfo detailed info start, plugin name - " + plugin_name,Q_FUNC_INFO);
    generate_columninfo_tablewidget_detailedinfo();
    generate_columninfo_tree_detailedinfo();
    recon_static_functions::debug_intensive(" generate list columninfo detailed info end, plugin name - " + plugin_name,Q_FUNC_INFO);


    recon_static_functions::debug_intensive(" generate list columninfo report  start, plugin name - " + plugin_name,Q_FUNC_INFO);
    generate_columninfo_html_report_tablewidget();
    generate_columninfo_html_report_textedit();
    generate_columninfo_html_report_stack();

    generate_order_list_xml_report();
    generate_order_list_csv_report();
    generate_columninfo_kml_report();
    recon_static_functions::debug_intensive(" generate list columninfo report  end, plugin name - " + plugin_name,Q_FUNC_INFO);


    recon_static_functions::debug_intensive(" generate list columninfo Keyword Search start, plugin name - " + plugin_name,Q_FUNC_INFO);
    generate_order_list_g_search_tablewidget();
    recon_static_functions::debug_intensive(" generate list columninfo Keyword Search end, plugin name - " + plugin_name,Q_FUNC_INFO);


    recon_static_functions::debug_intensive(" generate list columninfo Global Timeline start, plugin name - " + plugin_name,Q_FUNC_INFO);
    generate_order_list_timeline_tablewidget();
    recon_static_functions::debug_intensive(" generate list columninfo Global Timeline end, plugin name - " + plugin_name,Q_FUNC_INFO);

}



void csv_reader::sort_tw_md_rt_list(QStringList &order_list, QStringList &col_list, QStringList &data_type_list, QStringList &display_name_list)
{
    recon_static_functions::debug_intensive("start  ",Q_FUNC_INFO);

    int i, j;
    QString temp_order;
    QString temp_col_name;
    QString temp_display_name;
    QString temp_display_type;

    int size = order_list.size();

    for (i = 0; i < (size - 1); ++i)
    {
        if(i%10 == 0)
            QCoreApplication::processEvents();

        for (j = 0; j < size - 1 - i; ++j )
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            QString order_tmp_j = order_list.at(j);
            QString order_tmp_j_1 = order_list.at(j+1);
            if (order_tmp_j.toInt() > order_tmp_j_1.toInt())
            {
                temp_order = order_tmp_j_1;
                order_list.replace(j+1,order_tmp_j);
                order_list.replace(j,temp_order);

                temp_col_name = col_list.at(j+1);
                col_list.replace(j+1,col_list.at(j));
                col_list.replace(j,temp_col_name);

                temp_display_name = display_name_list.at(j+1);
                display_name_list.replace(j+1,display_name_list.at(j));
                display_name_list.replace(j,temp_display_name);

                temp_display_type = data_type_list.at(j+1);
                data_type_list.replace(j+1,data_type_list.at(j));
                data_type_list.replace(j,temp_display_type);

            }
        }
    }

    recon_static_functions::debug_intensive("end  ",Q_FUNC_INFO);

}

void csv_reader::sort_tw_md_rt_list(QStringList &order_list, QStringList &col_list, QStringList &data_type_list, QStringList &display_name_list, QStringList &col_width_list)
{
    recon_static_functions::debug_intensive("start  ",Q_FUNC_INFO);


    int i, j;
    QString temp_order;
    QString temp_col_name;
    QString temp_display_name;
    QString temp_display_type;
    QString temp_col_width;

    int size = order_list.size();

    for (i = 0; i < (size - 1); ++i)
    {
        if(i%10 == 0)
            QCoreApplication::processEvents();

        for (j = 0; j < size - 1 - i; ++j )
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            QString order_tmp_j = order_list.at(j);
            QString order_tmp_j_1 = order_list.at(j+1);
            if (order_tmp_j.toInt() > order_tmp_j_1.toInt())
            {
                temp_order = order_tmp_j_1;
                order_list.replace(j+1,order_tmp_j);
                order_list.replace(j,temp_order);

                temp_col_name = col_list.at(j+1);
                col_list.replace(j+1,col_list.at(j));
                col_list.replace(j,temp_col_name);

                temp_display_name = display_name_list.at(j+1);
                display_name_list.replace(j+1,display_name_list.at(j));
                display_name_list.replace(j,temp_display_name);

                temp_display_type = data_type_list.at(j+1);
                data_type_list.replace(j+1,data_type_list.at(j));
                data_type_list.replace(j,temp_display_type);

                temp_col_width = col_width_list.at(j+1);
                col_width_list.replace(j+1,col_width_list.at(j));
                col_width_list.replace(j,temp_col_width);

            }
        }
    }

    recon_static_functions::debug_intensive("end  ",Q_FUNC_INFO);

}

void csv_reader::sort_tw_md_rt_list_tree(QStringList &order_list, QStringList &col_list, QStringList &data_type_list, QStringList &display_name_list, QStringList &col_width_list,QStringList &parent_child_list)
{
    recon_static_functions::debug_intensive("start  ",Q_FUNC_INFO);

    int i, j;
    QString temp_order;
    QString temp_col_name;
    QString temp_display_name;
    QString temp_display_type;
    QString temp_col_width;
    QString temp_parent_child;

    int size = order_list.size();

    for (i = 0; i < (size - 1); ++i)
    {
        if(i%10 == 0)
            QCoreApplication::processEvents();

        for (j = 0; j < size - 1 - i; ++j )
        {
            if(j%10 == 0)
                QCoreApplication::processEvents();

            QString order_tmp_j = order_list.at(j);
            QString order_tmp_j_1 = order_list.at(j+1);
            if (order_tmp_j.toInt() > order_tmp_j_1.toInt())
            {
                temp_order = order_tmp_j_1;
                order_list.replace(j+1,order_tmp_j);
                order_list.replace(j,temp_order);

                temp_col_name = col_list.at(j+1);
                col_list.replace(j+1,col_list.at(j));
                col_list.replace(j,temp_col_name);

                temp_display_name = display_name_list.at(j+1);
                display_name_list.replace(j+1,display_name_list.at(j));
                display_name_list.replace(j,temp_display_name);

                temp_display_type = data_type_list.at(j+1);
                data_type_list.replace(j+1,data_type_list.at(j));
                data_type_list.replace(j,temp_display_type);

                temp_col_width = col_width_list.at(j+1);
                col_width_list.replace(j+1,col_width_list.at(j));
                col_width_list.replace(j,temp_col_width);

                temp_parent_child = parent_child_list.at(j+1);
                parent_child_list.replace(j+1,parent_child_list.at(j));
                parent_child_list.replace(j,temp_parent_child);

            }
        }
    }

    recon_static_functions::debug_intensive("end  ",Q_FUNC_INFO);

}



void csv_reader::clear_fields()
{

    total_tab_count_int = 0;

    tab_order_QStringList.clear();
    total_tab_name_QStringList.clear();
    total_table_name_QStringList.clear();


    plugin_csv_file_name.clear();
    plugin_csv_file_path.clear();
    plugin_name.clear();
    plugin_category.clear();
    plugin_supported_os_scheme_list.clear();
    plugin_extraction_mode.clear();
    plugin_available_for_redefined_result.clear();
    plugin_supported_software_list.clear();
    plugin_log_show_supported.clear();
    plugin_root_privileged.clear();
    plugin_user_iterated.clear();

    master_list_tablewidget.clear();
    master_list_textedit.clear();
    master_list_stack.clear();
    master_list_tree.clear();
    master_list_thumbnail.clear();
    master_list_chat_viewer.clear();

    table_name_list_tablewidget.clear();
    tab_name_list_tablewidget.clear();
    tab_supported_os_scheme_Qlist_tablewidget.clear();
    tab_supported_software_Qlist_tablewidget.clear();
    tab_log_show_supported_list_tablewidget.clear();
    tab_root_privileged_list_tablewidget.clear();
    tab_user_iterated_list_tablewidget.clear();
    tab_apple_metadata_available_list_tablewidget.clear();
    tab_html_report_available_list_tablewidget.clear();
    tab_xml_report_available_list_tablewidget.clear();
    tab_csv_report_available_list_tablewidget.clear();
    tab_kml_report_available_list_tablewidget.clear();
    tab_export_available_list_tablewidget.clear();
    tab_export_criteria_list_tablewidget.clear();
    tab_report_html_header_list_tablewidget.clear();
    tab_report_header_column_width_html_list_tablewidget.clear();
    tab_g_search_available_list_tablewidget.clear();
    tab_timeline_available_list_tablewidget.clear();
    tab_timeline_type_QList_tablewidget.clear();
    tab_timeline_column_name_QList_tablewidget.clear();
    tab_artifact_common_list_tablewidget.clear();

    tab_name_list_textedit.clear();
    table_name_list_textedit.clear();
    tab_supported_os_scheme_Qlist_textedit.clear();
    tab_supported_software_Qlist_textedit.clear();
    tab_log_show_supported_list_textedit.clear();
    tab_root_privileged_list_textedit.clear();
    tab_user_iterated_list_textedit.clear();
    tab_html_report_available_list_textedit.clear();
    tab_xml_report_available_list_textedit.clear();
    tab_csv_report_available_list_textedit.clear();
    tab_report_html_header_list_textedit.clear();
    tab_report_header_column_width_html_list_textedit.clear();
    tab_artifact_common_list_textedit.clear();

    tab_name_list_stack.clear();
    table_name_list_stack.clear();
    tab_supported_os_scheme_Qlist_stack.clear();
    tab_supported_software_Qlist_stack.clear();
    tab_log_show_supported_list_stack.clear();
    tab_root_privileged_list_stack.clear();
    tab_user_iterated_list_stack.clear();
    tab_html_report_available_list_stack.clear();
    tab_xml_report_available_list_stack.clear();
    tab_csv_report_available_list_stack.clear();
    tab_report_html_header_list_stack.clear();
    tab_report_header_column_width_html_list_stack.clear();
    tab_artifact_common_list_stack.clear();

    table_name_list_tree.clear();
    tab_name_list_tree.clear();
    tab_supported_os_scheme_Qlist_tree.clear();
    tab_supported_software_Qlist_tree.clear();
    tab_log_show_supported_list_tree.clear();
    tab_root_privileged_list_tree.clear();
    tab_user_iterated_list_tree.clear();

    table_name_list_thumbnail.clear();
    tab_name_list_thumbnail.clear();
    tab_supported_os_scheme_Qlist_thumbnail.clear();
    tab_supported_software_Qlist_thumbnail.clear();
    tab_log_show_supported_list_thumbnail.clear();
    tab_root_privileged_list_thumbnail.clear();
    tab_user_iterated_list_thumbnail.clear();

    table_name_list_chat_viewer.clear();
    tab_name_list_chat_viewer.clear();
    tab_supported_os_scheme_Qlist_chat_viewer.clear();
    tab_supported_software_Qlist_chat_viewer.clear();
    tab_log_show_supported_list_chat_viewer.clear();
    tab_root_privileged_list_chat_viewer.clear();
    tab_user_iterated_list_chat_viewer.clear();


    display_header_list_tablewidget.clear();
    display_column_name_list_tablewidget.clear();
    display_data_type_list_tablewidget.clear();
    display_col_width_list_tablewidget.clear();
    ms_search_local_list_tablewidget.clear();
    display_filtered_order_list_tablewidget.clear();

    display_column_name_list_textedit.clear();
    display_data_type_list_textedit.clear();

    display_column_name_list_stack.clear();
    display_data_type_list_stack.clear();

    display_header_list_tree.clear();
    display_parent_child_list_tree.clear();
    display_column_name_list_tree.clear();
    display_data_type_list_tree.clear();
    display_col_width_list_tree.clear();


    display_column_name_list_chat_viewer.clear();
    display_data_type_list_chat_viewer.clear();





    md_header_list_tablewidget.clear();
    md_column_name_list_tablewidget.clear();
    md_data_type_list_tablewidget.clear();
    md_preview_column_list_tablewidget.clear();
    export_path_column_list_tablewidget.clear();

    md_header_list_tree.clear();
    md_column_name_list_tree.clear();
    md_data_type_list_tree.clear();
    md_preview_list_tree.clear();


    rt_html_d_name_list_tablewidget.clear();
    rt_html_col_name_list_tablewidget.clear();
    rt_html_order_list_tablewidget.clear();
    rt_html_data_type_list_tablewidget.clear();

    report_html_col_name_list_textedit.clear();
    report_html_datatype_list_textedit.clear();

    report_html_col_name_list_stack.clear();
    report_html_datatype_list_stack.clear();

    rt_xml_d_name_list.clear();
    rt_xml_col_name_list.clear();
    rt_xml_order_list.clear();
    rt_xml_data_type_list.clear();


    rt_csv_d_name_list.clear();
    rt_csv_col_name_list.clear();
    rt_csv_order_list.clear();
    rt_csv_data_type_list.clear();


    rt_kml_d_name_list.clear();
    rt_kml_col_name_list.clear();
    rt_kml_order_list.clear();
    rt_kml_data_type_list.clear();

    g_search_col_name_list.clear();
    g_search_col_order_list.clear();
    g_search_col_data_type_list.clear();

    g_timeline_col_name_list.clear();
    g_timeline_col_data_type_list.clear();

}

void csv_reader::fill_value()
{
    struct_global_csv_reader_full_csv_info obj;

    clear_variables_csv_info(obj);

    obj.plugin_csv_file_name = plugin_csv_file_name;
    obj.plugin_csv_file_path = plugin_csv_file_path;

    obj.overall_tab_name_QStringList = total_tab_name_QStringList;
    obj.overall_table_name_QStringList = total_table_name_QStringList;
    obj.overall_tab_count_int = total_tab_count_int;
    obj.overall_tab_order_QStringList = tab_order_QStringList;


    obj.plugin_name = plugin_name;
    obj.plugin_parent_name = plugin_category;
    obj.plugin_native_os_QStringList = plugin_native_os_list;
    obj.plugin_supported_os_QStringList = plugin_supported_os_scheme_list;
    obj.plugin_extraction_mode = plugin_extraction_mode;
    obj.plugin_available_for_redefined_result = plugin_available_for_redefined_result;
    obj.plugin_supported_software_QStringList = plugin_supported_software_list;
    obj.plugin_log_show_supported = plugin_log_show_supported;
    obj.plugin_root_privileged = plugin_root_privileged;
    obj.plugin_user_iterated = plugin_user_iterated;


    obj.tab_count_int_tablewidget = tab_name_list_tablewidget.size();
    obj.table_name_QStringList_tablewidget = table_name_list_tablewidget;
    obj.tab_name_QStringList_tablewidget = tab_name_list_tablewidget;
    obj.tab_supported_os_scheme_QList_tablewidget = tab_supported_os_scheme_Qlist_tablewidget;
    obj.tab_supported_software_QList_tablewidget = tab_supported_software_Qlist_tablewidget;
    obj.tab_log_show_supported_QStringList_tablewidget = tab_log_show_supported_list_tablewidget;
    obj.tab_root_privileged_QStringList_tablewidget = tab_root_privileged_list_tablewidget;
    obj.tab_user_iterated_QStringList_tablewidget = tab_user_iterated_list_tablewidget;

    obj.tab_apple_metadata_available_QStringList_tablewidget = tab_apple_metadata_available_list_tablewidget;
    obj.tab_html_report_available_QStringList_tablewidget = tab_html_report_available_list_tablewidget;
    obj.tab_xml_report_available_QStringList_tablewidget = tab_xml_report_available_list_tablewidget;
    obj.tab_csv_report_available_QStringList_tablewidget = tab_csv_report_available_list_tablewidget;
    obj.tab_kml_report_available_QStringList_tablewidget = tab_kml_report_available_list_tablewidget;
    obj.tab_export_available_QStringList_tablewidget = tab_export_available_list_tablewidget;
    obj.tab_export_criteria_QStringList_tablewidget = tab_export_criteria_list_tablewidget;
    obj.tab_report_html_header_QStringList_tablewidget = tab_report_html_header_list_tablewidget;
    obj.tab_report_header_column_width_html_QStringList_tablewidget = tab_report_header_column_width_html_list_tablewidget;
    obj.tab_g_search_available_QStringList_tablewidget = tab_g_search_available_list_tablewidget;
    obj.tab_timeline_available_QStringList_tablewidget = tab_timeline_available_list_tablewidget;
    obj.tab_timeline_timestamp_type_QList_tablewidget = tab_timeline_type_QList_tablewidget;
    obj.tab_timeline_timestamp_column_name_QList_tablewidget = tab_timeline_column_name_QList_tablewidget;
    obj.tab_artifact_common_QStringList_tablewidget = tab_artifact_common_list_tablewidget;

    obj.tab_count_int_textedit = tab_name_list_textedit.size();
    obj.tab_name_QStringList_textedit = tab_name_list_textedit;
    obj.table_name_QStringList_textedit = table_name_list_textedit;
    obj.tab_supported_os_scheme_QList_textedit = tab_supported_os_scheme_Qlist_textedit;
    obj.tab_supported_software_QList_textedit = tab_supported_software_Qlist_textedit;
    obj.tab_log_show_supported_QStringList_textedit = tab_log_show_supported_list_textedit;
    obj.tab_root_privileged_QStringList_textedit = tab_root_privileged_list_textedit;
    obj.tab_user_iterated_QStringList_textedit = tab_user_iterated_list_textedit;
    obj.tab_html_report_available_QStringList_textedit = tab_html_report_available_list_textedit;
    obj.tab_xml_report_available_QStringList_textedit = tab_xml_report_available_list_textedit;
    obj.tab_csv_report_available_QStringList_textedit = tab_csv_report_available_list_textedit;
    obj.tab_report_html_header_QStringList_textedit = tab_report_html_header_list_textedit;
    obj.tab_report_header_column_width_html_QStringList_textedit = tab_report_header_column_width_html_list_textedit;
    obj.tab_artifact_common_QStringList_textedit = tab_artifact_common_list_textedit;

    obj.tab_count_int_stack = tab_name_list_stack.size();
    obj.tab_name_QStringList_stack = tab_name_list_stack;
    obj.table_name_QStringList_stack = table_name_list_stack;
    obj.tab_supported_os_scheme_QList_stack = tab_supported_os_scheme_Qlist_stack;
    obj.tab_supported_software_QList_stack = tab_supported_software_Qlist_stack;
    obj.tab_log_show_supported_QStringList_stack = tab_log_show_supported_list_stack;
    obj.tab_root_privileged_QStringList_stack = tab_root_privileged_list_stack;
    obj.tab_user_iterated_QStringList_stack = tab_user_iterated_list_stack;
    obj.tab_html_report_available_QStringList_stack = tab_html_report_available_list_stack;
    obj.tab_xml_report_available_QStringList_stack = tab_xml_report_available_list_stack;
    obj.tab_csv_report_available_QStringList_stack = tab_csv_report_available_list_stack;
    obj.tab_report_html_header_QStringList_stack = tab_report_html_header_list_stack;
    obj.tab_report_header_column_width_html_QStringList_stack = tab_report_header_column_width_html_list_stack;
    obj.tab_artifact_common_QStringList_stack = tab_artifact_common_list_stack;

    obj.tab_count_int_tree = tab_name_list_tree.size();
    obj.tab_name_QStringList_tree = tab_name_list_tree;
    obj.table_name_QStringList_tree = table_name_list_tree;
    obj.tab_supported_os_scheme_QList_tree = tab_supported_os_scheme_Qlist_tree;
    obj.tab_supported_software_QList_tree = tab_supported_software_Qlist_tree;
    obj.tab_log_show_supported_QStringList_tree = tab_log_show_supported_list_tree;
    obj.tab_root_privileged_QStringList_tree = tab_root_privileged_list_tree;
    obj.tab_user_iterated_QStringList_tree = tab_user_iterated_list_tree;

    obj.tab_count_int_thumbnail = tab_name_list_thumbnail.size();
    obj.tab_name_QStringList_thumbnail = tab_name_list_thumbnail;
    obj.table_name_QStringList_thumbnail = table_name_list_thumbnail;
    obj.tab_supported_os_scheme_QList_thumbnail = tab_supported_os_scheme_Qlist_thumbnail;
    obj.tab_supported_software_QList_thumbnail = tab_supported_software_Qlist_thumbnail;
    obj.tab_log_show_supported_QStringList_thumbnail = tab_log_show_supported_list_thumbnail;
    obj.tab_root_privileged_QStringList_thumbnail = tab_root_privileged_list_thumbnail;
    obj.tab_user_iterated_QStringList_thumbnail = tab_user_iterated_list_thumbnail;


    obj.tab_count_int_chat_viewer = tab_name_list_chat_viewer.size();
    obj.tab_name_QStringList_chat_viewer = tab_name_list_chat_viewer;
    obj.table_name_QStringList_chat_viewer = table_name_list_chat_viewer;
    obj.tab_supported_os_scheme_QList_chat_viewer = tab_supported_os_scheme_Qlist_chat_viewer;
    obj.tab_supported_software_QList_chat_viewer = tab_supported_software_Qlist_chat_viewer;
    obj.tab_log_show_supported_QStringList_chat_viewer = tab_log_show_supported_list_chat_viewer;
    obj.tab_root_privileged_QStringList_chat_viewer = tab_root_privileged_list_chat_viewer;
    obj.tab_user_iterated_QStringList_chat_viewer = tab_user_iterated_list_chat_viewer;

    obj.display_header_QList_tablewidget = display_header_list_tablewidget;
    obj.display_column_name_QList_tablewidget = display_column_name_list_tablewidget;
    obj.display_data_type_QList_tablewidget = display_data_type_list_tablewidget;
    obj.display_col_width_QList_tablewidget = display_col_width_list_tablewidget;
    obj.local_search_column_name_QList_tablewidget = ms_search_local_list_tablewidget;
    obj.display_order_QList_tablewidget = display_filtered_order_list_tablewidget;

    obj.display_column_name_QList_textedit = display_column_name_list_textedit;
    obj.display_data_type_QList_textedit = display_data_type_list_textedit;

    obj.display_column_name_QList_stack = display_column_name_list_stack;
    obj.display_data_type_QList_stack = display_data_type_list_stack;

    obj.display_header_QList_tree = display_header_list_tree;
    obj.display_parent_child_QList_tree = display_parent_child_list_tree;
    obj.display_column_name_QList_tree = display_column_name_list_tree;
    obj.display_data_type_QList_tree = display_data_type_list_tree;
    obj.display_col_width_QList_tree = display_col_width_list_tree;


    obj.display_column_name_QList_chat_viewer = display_column_name_list_chat_viewer;
    obj.display_data_type_QList_chat_viewer= display_data_type_list_chat_viewer;

    obj.metadata_header_QList_tablewidget = md_header_list_tablewidget;
    obj.metadata_column_name_QList_tablewidget = md_column_name_list_tablewidget;
    obj.metadata_data_type_QList_tablewidget = md_data_type_list_tablewidget;
    obj.metadata_preview_QList_tablewidget = md_preview_column_list_tablewidget;
    obj.export_path_column_name_QList_tablewidget = export_path_column_list_tablewidget;

    obj.metadata_header_QList_tree = md_header_list_tree;
    obj.metadata_column_name_QList_tree = md_column_name_list_tree;
    obj.metadata_data_type_QList_tree = md_data_type_list_tree;
    obj.metadata_preview_QList_tree = md_preview_list_tree;


    obj.report_html_display_name_QList_tablewidget = rt_html_d_name_list_tablewidget;
    obj.report_html_col_name_QList_tablewidget = rt_html_col_name_list_tablewidget;
    obj.report_html_order_QList_tablewidget = rt_html_order_list_tablewidget;
    obj.report_html_data_type_QList_tablewidget = rt_html_data_type_list_tablewidget;

    obj.report_html_col_name_QList_textedit = report_html_col_name_list_textedit;
    obj.report_html_datatype_QList_textedit = report_html_datatype_list_textedit;

    obj.report_html_col_name_QList_stack = report_html_col_name_list_stack;
    obj.report_html_datatype_QList_stack = report_html_datatype_list_stack;

    obj.report_xml_display_name_QList_tablewidget = rt_xml_d_name_list;
    obj.report_xml_col_name_QList_tablewidget = rt_xml_col_name_list;
    obj.report_xml_order_QList_tablewidget = rt_xml_order_list;
    obj.report_xml_data_type_QList_tablewidget = rt_xml_data_type_list;


    obj.report_csv_display_name_QList_tablewidget = rt_csv_d_name_list;
    obj.report_csv_col_name_QList_tablewidget = rt_csv_col_name_list;
    obj.report_csv_order_QList_tablewidget = rt_csv_order_list;
    obj.report_csv_data_type_QList_tablewidget = rt_csv_data_type_list;


    obj.report_kml_display_name_QList_tablewidget = rt_kml_d_name_list;
    obj.report_kml_col_name_QList_tablewidget = rt_kml_col_name_list;
    obj.report_kml_order_QList_tablewidget = rt_kml_order_list;
    obj.report_kml_data_type_QList_tablewidget = rt_kml_data_type_list;

    obj.g_search_col_name_QList = g_search_col_name_list;
    obj.g_search_col_order_QList = g_search_col_order_list;
    obj.g_search_col_data_type_QList = g_search_col_data_type_list;

    obj.g_timeline_col_name_QList = g_timeline_col_name_list;
    obj.g_timeline_col_data_type_QList = g_timeline_col_data_type_list;

    csv_info_list_full << obj;
    csv_info_list_final << obj;

}


