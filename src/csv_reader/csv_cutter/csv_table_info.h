#ifndef CSV_TABLE_INFO_H
#define CSV_TABLE_INFO_H

#include "csv_column_info.h"



class csv_table_info
{
public:
    csv_table_info(QWidget *parent = 0);
    void set_table_attribute(QString line);

    QString get_table_name();
    QString get_table_tab_widget_type();
    QString get_tab_name();
    QString get_table_visibility();
    QString get_table_order();
    QString get_table_html_report();
    QString get_table_xml_report();
    QString get_table_csv_report();
    QString get_table_kml_report();

    QStringList get_table_tab_supported_os();
    QStringList get_table_tab_supported_software();
    QString get_apple_metadata();

    QString get_log_show_supported();
    QString get_root_privileged();
    QString get_user_iterated();

    QString get_artifact_common();

    QString get_table_export_available();

    QString get_table_html_header();
    QString get_table_header_column_width_html();

    QString get_table_g_search_available();


    QString get_table_timeline_available();
    QStringList get_table_timeline_type();
    QStringList get_table_timeline_column_name();

    void set_column_attribute(QString line);

    void print_table_info();

    QList<csv_column_info> get_cols_list();


private:

    enum table_info_plugin_table_master{
        enum_table_info_TABLE_NAME = 0,
        enum_table_info_TAB_WIDGET_TYPE,
        enum_table_info_TAB_NAME,
        enum_table_info_TAB_VISIBILITY,
        enum_table_info_TAB_ORDER,

        enum_table_info_TAB_SUPPORTED_OS_LIST,
        enum_table_info_TAB_SUPPORTED_SOFTWARE_LIST,
        enum_table_info_LOG_SHOW_SUPPORTED,
        enum_table_info_ROOT_PRIVILEGED,
        enum_table_info_USER_ITERATED,
        enum_table_info_Col_10,

        enum_table_info_APPLE_METADATA_AVAILABLE,
        enum_table_info_EXPORT_AVAILABLE,
        enum_table_info_ARTIFACT_COMMON,

        enum_table_info_HTML_REPORT_AVAILABLE,
        enum_table_info_Col_15,
        enum_table_info_HTML_REPORT_HEADER_NAME,
        enum_table_info_HTML_REPORT_HEADER_WIDTH,

        enum_table_info_XML_REPORT_AVAILABLE,
        enum_table_info_Col_19,
        enum_table_info_Col_20,
        enum_table_info_Col_21,

        enum_table_info_CSV_REPORT_AVAILABLE,
        enum_table_info_Col_23,
        enum_table_info_Col_24,
        enum_table_info_Col_25,
        enum_table_info_KML_REPORT_AVAILABLE,
        enum_table_info_Col_27,
        enum_table_info_Col_28,
        enum_table_info_Col_29,

        enum_table_info_GLOBAL_SEARCH_AVAILABLE,
        enum_table_info_Col_31,
        enum_table_info_Col_32,

        enum_table_info_TIMELINE_AVAILABLE,
        enum_table_info_TIMELINE_TIMESTAMP_TYPE,
        enum_table_info_TIMELINE_COLUMN_NAME,

        enum_table_info_Col_EXTRA_1,
        enum_table_info_Col_EXTRA_2,
        enum_table_info_Col_EXTRA_3,
        enum_table_info_Col_EXTRA_4,
        enum_table_info_Col_EXTRA_5,
        enum_table_info_Col_EXTRA_6,
        enum_table_info_Col_EXTRA_7,
        enum_table_info_Col_EXTRA_8,
        enum_table_info_Col_EXTRA_9,
        enum_table_info_Col_EXTRA_10,
        enum_table_info_Col_EXTRA_11,
        enum_table_info_Col_EXTRA_12,
        enum_table_info_Col_EXTRA_13,
        enum_table_info_Col_EXTRA_14,
        enum_table_info_Col_EXTRA_15,
        enum_table_info_Col_EXTRA_16,
        enum_table_info_Col_EXTRA_17,
        enum_table_info_Col_EXTRA_18,
        enum_table_info_Col_EXTRA_19,
        enum_table_info_Col_EXTRA_20


    };

    QString table_name;
    QString tab_widget_type;
    QString tab_name;
    QString tab_visible;
    QString tab_order;

    QString log_show_supported;
    QString root_privileged;
    QString user_iterated;

    QString apple_metadata_available;

    QString html_report_available;
    QString xml_report_available;
    QString csv_report_available;
    QString kml_report_available;

    QString export_available;

    QString html_report_header_name;
    QString html_report_header_width;

    QString global_search_available;

    QString timeline_available;
    QStringList timeline_timestamp_type_stringlist;
    QStringList timeline_column_name_stringlist;
    QString artifact_common;

    QStringList tab_supported_os_scheme_list;
    QStringList tab_supported_software_list;


    QList<csv_column_info> col_info_list;
};

#endif //
