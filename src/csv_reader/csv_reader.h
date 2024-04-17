#ifndef CSV_READER_H
#define CSV_READER_H

#include <QObject>
#include <QMapIterator>
#include "csv_cutter/csv_list_maker.h"
#include "csv_cutter/csv_column_info.h"
#include "recon_generic_functions/recon_static_functions.h"
#include <QtCore>
#include "header_csv_template.h"
#include "header_identical_1.h"


class csv_reader
{

public:
    explicit csv_reader(QWidget *parent = 0);
    ~csv_reader();

    void set_csv_dir_path(QString path);
    void do_job_plugin_all();
    void do_job_plugin_specific(QString csv_file_path, QString caller_func);

    void set_plugin_and_associated_tabs(QStringList received_plugin_and_tabs_list, QString caller_func);





    struct_global_csv_reader_full_csv_info read_csv_value(QString plugin_name);


    QList<struct_global_csv_reader_full_csv_info> get_csv_info_list();

    bool is_export_available_in_plugin(QString plugin_name);
    bool is_export_available_in_tab(QString tab_name, QString plugin_name);
    bool is_tab_support_multiple_export(QString tab_name, QString plugin_name);
    bool is_timeline_available_in_plugin(QString plugin_name);
    bool is_timeline_available_in_tab(QString plugin_name, QString tab_name);
    bool is_timeline_available_in_table(QString plugin_name, QString table_name);

    QString get_parent_category(QString plugin_name);
    QStringList get_total_tables_list(QString plugin_name);
    QStringList get_total_tab_list(QString plugin_name);
    QStringList get_export_tab_list(QString plugin_name);

    //=========================//
    QString get_table_name_according_tab(QString plugin_name,QString tab_name);
    QStringList get_tables_name_according_tabs(QString plugin_name,QStringList tab_list);
    QStringList get_all_tab_list_associated_with_table(QString plugin_name_str, QString table_name_str);
    QString get_tab_name_associated_with_widget(QString plugin_name_str, QString table_name_str, QString widget_type);

    int get_tab_index_as_per_tabwidget(QString plugin_name_str, QString tab_name_str);
    //=========================//

    QStringList get_native_os_list(QString plugin_name);
    QStringList get_supported_os_scheme_list_of_plugin(QString plugin_name);
    QStringList get_supported_os_scheme_list_of_tab(QString plugin_name, QString tab_name);

    QStringList get_header_list_of_tab(QString plugin_name, QString tab_name);
    QString get_coloum_name_according_header_name(QString plugin_name, QString table_name, QString header_name);



    bool is_tab_type_tablewidget(QString plugin_name, QString tab_name);
    bool is_tab_type_stackwidget(QString plugin_name, QString tab_name);
    bool is_tab_type_textedit(QString plugin_name, QString tab_name);
    bool is_tab_type_chat_view(QString plugin_name, QString tab_name);
    bool is_tab_type_tree_view(QString plugin_name, QString tab_name);
    bool is_tab_type_thumbnail_view(QString plugin_name, QString tab_name);

    bool is_plugin_volatile(QString plugin_name);
    bool is_plugin_available_for_redefined_result(QString plugin_name);
    bool is_plugin_available_for_chat_view(QString plugin_name);

    QString get_chat_view_tab_name(QString plugin_name);
    bool is_plugin_available_for_software(QString m_plugin_name, QString soft_name);
    bool is_plugin_available_for_os(QString m_plugin_name, QString os_name);

    bool is_plugin_log_show_supported(QString plugin_name);


signals:

public slots:

private:

    QString csv_dir_path;
    int make_master_lists(QString csv_path);
    QList<struct_global_csv_reader_table_detail> master_list_tablewidget;
    QList<struct_global_csv_reader_table_detail> master_list_textedit;
    QList<struct_global_csv_reader_table_detail> master_list_stack;
    QList<struct_global_csv_reader_table_detail> master_list_tree;
    QList<struct_global_csv_reader_table_detail> master_list_thumbnail;
    QList<struct_global_csv_reader_table_detail> master_list_chat_viewer;


    bool is_our_masterlist_and_GUItab_size_accurate();
    void generate_info();
    void clear_fields();
    void fill_value();

    void sort_tw_md_rt_list(QStringList &order_list, QStringList &col_list, QStringList &data_type_list, QStringList &display_name_list);
    void sort_tw_md_rt_list(QStringList &order_list, QStringList &col_list, QStringList &data_type_list, QStringList &display_name_list, QStringList &col_width_list);
    void sort_tw_md_rt_list_tree(QStringList &order_list, QStringList &col_list, QStringList &data_type_list, QStringList &display_name_list, QStringList &col_width_list, QStringList &parent_child_list);

    QList<struct_global_csv_reader_full_csv_info>csv_info_list_full;
    QList<struct_global_csv_reader_full_csv_info>csv_info_list_final;

    //---------------------------------------- Computed Info Start--------------------------------//

    int total_tab_count_int;
    QStringList tab_order_QStringList;
    QStringList total_tab_name_QStringList;
    QStringList total_table_name_QStringList;


    //---------------------------------------- Plugin Info Start--------------------------------//

    QString plugin_csv_file_name;
    QString plugin_csv_file_path;
    QString plugin_name;
    QString plugin_category;
    QStringList plugin_native_os_list;
    QStringList plugin_supported_os_scheme_list;
    QString plugin_extraction_mode;
    QString plugin_available_for_redefined_result;
    QStringList plugin_supported_software_list;
    QString plugin_log_show_supported;
    QString plugin_root_privileged;
    QString plugin_user_iterated;

    //---------------------------------------- Table Info Start--------------------------------//
    void generate_tableinfo_tablewidget();
    QStringList table_name_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QList<QStringList> tab_supported_os_scheme_Qlist_tablewidget;
    QList<QStringList> tab_supported_software_Qlist_tablewidget;
    QStringList tab_log_show_supported_list_tablewidget;
    QStringList tab_root_privileged_list_tablewidget;
    QStringList tab_user_iterated_list_tablewidget;
    QStringList tab_apple_metadata_available_list_tablewidget;
    QStringList tab_html_report_available_list_tablewidget;
    QStringList tab_xml_report_available_list_tablewidget;
    QStringList tab_csv_report_available_list_tablewidget;
    QStringList tab_kml_report_available_list_tablewidget;
    QStringList tab_export_available_list_tablewidget;
    QStringList tab_export_criteria_list_tablewidget;
    QStringList tab_report_html_header_list_tablewidget;
    QStringList tab_report_header_column_width_html_list_tablewidget;
    QStringList tab_g_search_available_list_tablewidget;
    QStringList tab_timeline_available_list_tablewidget;
    QList<QStringList> tab_timeline_type_QList_tablewidget;
    QList<QStringList> tab_timeline_column_name_QList_tablewidget;
    QStringList tab_artifact_common_list_tablewidget;

    void generate_tableinfo_textedit();
    QStringList tab_name_list_textedit;
    QStringList table_name_list_textedit;
    QList<QStringList> tab_supported_os_scheme_Qlist_textedit;
    QList<QStringList> tab_supported_software_Qlist_textedit;
    QStringList tab_log_show_supported_list_textedit;
    QStringList tab_root_privileged_list_textedit;
    QStringList tab_user_iterated_list_textedit;
    QStringList tab_html_report_available_list_textedit;
    QStringList tab_xml_report_available_list_textedit;
    QStringList tab_csv_report_available_list_textedit;
    QStringList tab_report_html_header_list_textedit;
    QStringList tab_report_header_column_width_html_list_textedit;
    QStringList tab_artifact_common_list_textedit;

    void generate_tableinfo_stack();
    QStringList tab_name_list_stack;
    QStringList table_name_list_stack;
    QList<QStringList> tab_supported_os_scheme_Qlist_stack;
    QList<QStringList> tab_supported_software_Qlist_stack;
    QStringList tab_log_show_supported_list_stack;
    QStringList tab_root_privileged_list_stack;
    QStringList tab_user_iterated_list_stack;
    QStringList tab_html_report_available_list_stack;
    QStringList tab_xml_report_available_list_stack;
    QStringList tab_csv_report_available_list_stack;
    QStringList tab_report_html_header_list_stack;
    QStringList tab_report_header_column_width_html_list_stack;
    QStringList tab_artifact_common_list_stack;

    void generate_tableinfo_tree();
    QStringList table_name_list_tree;
    QStringList tab_name_list_tree;
    QList<QStringList> tab_supported_os_scheme_Qlist_tree;
    QList<QStringList> tab_supported_software_Qlist_tree;
    QStringList tab_log_show_supported_list_tree;
    QStringList tab_root_privileged_list_tree;
    QStringList tab_user_iterated_list_tree;

    void generate_tableinfo_thumbnail();
    QStringList table_name_list_thumbnail;
    QStringList tab_name_list_thumbnail;
    QList<QStringList> tab_supported_os_scheme_Qlist_thumbnail;
    QList<QStringList> tab_supported_software_Qlist_thumbnail;
    QStringList tab_log_show_supported_list_thumbnail;
    QStringList tab_root_privileged_list_thumbnail;
    QStringList tab_user_iterated_list_thumbnail;

    void generate_tableinfo_chat_viewer();
    QStringList table_name_list_chat_viewer;
    QStringList tab_name_list_chat_viewer;
    QList<QStringList> tab_supported_os_scheme_Qlist_chat_viewer;
    QList<QStringList> tab_supported_software_Qlist_chat_viewer;
    QStringList tab_log_show_supported_list_chat_viewer;
    QStringList tab_root_privileged_list_chat_viewer;
    QStringList tab_user_iterated_list_chat_viewer;

    //---------------------------------------- Table Info end--------------------------------//

    //---------------------------------------- Display start--------------------------------//
    void generate_columninfo_display_tablewidget();
    QList<QStringList>display_header_list_tablewidget;
    QList<QStringList>display_column_name_list_tablewidget;
    QList<QStringList>display_data_type_list_tablewidget;
    QList<QStringList>display_col_width_list_tablewidget;
    QList<QStringList>ms_search_local_list_tablewidget;
    QList<QStringList>display_filtered_order_list_tablewidget;

    void generate_columninfo_display_textedit();
    QList<QStringList>display_column_name_list_textedit;
    QList<QStringList>display_data_type_list_textedit;

    void generate_columninfo_display_stack();
    QList<QStringList>display_column_name_list_stack;
    QList<QStringList>display_data_type_list_stack;

    void generate_columninfo_display_tree();
    QList<QStringList>display_header_list_tree;
    QList<QStringList>display_parent_child_list_tree;
    QList<QStringList>display_column_name_list_tree;
    QList<QStringList>display_data_type_list_tree;
    QList<QStringList>display_col_width_list_tree;


    void generate_columninfo_display_chat_viewer();
    QList<QStringList>display_column_name_list_chat_viewer;
    QList<QStringList>display_data_type_list_chat_viewer;

    //---------------------------------------- Display end--------------------------------//

    //---------------------------------------- Detailed Info Start--------------------------------//
    void generate_columninfo_tablewidget_detailedinfo();
    QList<QStringList>md_header_list_tablewidget;
    QList<QStringList>md_column_name_list_tablewidget;
    QList<QStringList>md_data_type_list_tablewidget;
    QList<QStringList>md_preview_column_list_tablewidget;
    QList<QStringList>export_path_column_list_tablewidget;

    void generate_columninfo_tree_detailedinfo();
    QList<QStringList>md_header_list_tree;
    QList<QStringList>md_column_name_list_tree;
    QList<QStringList>md_data_type_list_tree;
    QList<QStringList>md_preview_list_tree;

    //---------------------------------------- Detailed Info End--------------------------------//

    //---------------------------------------- Report start--------------------------------//
    void generate_columninfo_html_report_tablewidget();
    QList<QStringList>rt_html_d_name_list_tablewidget;
    QList<QStringList>rt_html_col_name_list_tablewidget;
    QList<QStringList>rt_html_order_list_tablewidget;
    QList<QStringList>rt_html_data_type_list_tablewidget;

    void generate_columninfo_html_report_textedit();
    QList<QStringList>report_html_col_name_list_textedit;
    QList<QStringList>report_html_datatype_list_textedit;

    void generate_columninfo_html_report_stack();
    QList<QStringList>report_html_col_name_list_stack;
    QList<QStringList>report_html_datatype_list_stack;

    void generate_order_list_xml_report();
    QList<QStringList>rt_xml_d_name_list;
    QList<QStringList>rt_xml_col_name_list;
    QList<QStringList>rt_xml_order_list;
    QList<QStringList>rt_xml_data_type_list;


    void generate_order_list_csv_report();
    QList<QStringList>rt_csv_d_name_list;
    QList<QStringList>rt_csv_col_name_list;
    QList<QStringList>rt_csv_order_list;
    QList<QStringList>rt_csv_data_type_list;


    void generate_columninfo_kml_report();
    QList<QStringList>rt_kml_d_name_list;
    QList<QStringList>rt_kml_col_name_list;
    QList<QStringList>rt_kml_order_list;
    QList<QStringList>rt_kml_data_type_list;
    //---------------------------------------- Report end--------------------------------//


    //---------------------------------------- Keyword Search start--------------------------------//
    void generate_order_list_g_search_tablewidget();
    QList<QStringList>g_search_col_name_list;
    QList<QStringList>g_search_col_order_list;
    QList<QStringList>g_search_col_data_type_list;
    //---------------------------------------- Keyword Search end--------------------------------//

    //---------------------------------------- Global Timeline start--------------------------------//
    void generate_order_list_timeline_tablewidget();
    QList<QStringList>g_timeline_col_name_list;
    QList<QStringList>g_timeline_col_data_type_list;
    //---------------------------------------- Global Timeline end--------------------------------//



    //----------------------------------------- Tab Specific Extraction Work----------------------//



    void parse_plugin_csv(QString csv_file_path);

    void checking_plugins_duplicacy(void);

    QStringList get_all_full_tab_list_associated_with_table(QString plugin_name_str, QString table_name_str);
    QStringList get_full_tables_name_according_tabs(QString plugin_name, QStringList tab_list);

    void clear_variables_csv_info(struct_global_csv_reader_full_csv_info &struct_csv_info);

};

#endif // CSV_READER_H
