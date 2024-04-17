#ifndef CSV_COLUMN_INFO_H
#define CSV_COLUMN_INFO_H

#include <QtCore>

#include "header_csv_template.h"
#include "recon_generic_functions/recon_static_functions.h"

class csv_column_info
{
public:
    csv_column_info(QWidget *parent = 0);

    void set_column_attribute(QString line);

    QString get_column_name();
    QString get_column_parent_child_type();
    QString get_column_tw_display_name();
    QString get_column_tw_visibility();
    QString get_column_tw_order();
    QString get_column_tw_col_width();
    QString get_column_tw_data_type();

    QString get_column_md_name();
    QString get_column_md_visiblity();
    QString get_column_md_order();
    QString get_column_md_data_type();
    QString get_search_local_field();
    QString get_md_preview();

    QString get_html_name();
    QString get_html_visible();
    QString get_html_order();
    QString get_html_report_data_type();


    QString get_xml_name();
    QString get_xml_visible();
    QString get_xml_order();
    QString get_xml_report_data_type();

    QString get_csv_name();
    QString get_csv_visible();
    QString get_csv_order();
    QString get_csv_report_data_type();

    QString get_kml_report_data_type();
    QString get_kml_order();
    QString get_kml_visible();
    QString get_kml_name();

    QString get_g_search_visible();
    QString get_g_search_order();
    QString get_g_search_data_type();

    QString get_timeline_visible();
    QString get_timeline_order();
    QString get_timeline_data_type();

    void print_col_info();

    QString get_export_path();

private:


    enum Column_Info_Plugin_column{

        enum_Column_Info_COLUMN_NAME,
        enum_Column_Info_PARENT_CHILD_TYPE,
        enum_Column_Info_TW_NAME,
        enum_Column_Info_TW_VISIBILITY,
        enum_Column_Info_TW_ORDER,
        enum_Column_Info_TW_COL_WIDTH,
        enum_Column_Info_TW_DATA_TYPE,

        enum_Column_Info_MD_NAME,
        enum_Column_Info_MD_VISIBLITY,
        enum_Column_Info_MD_ORDER,
        enum_Column_Info_MD_DATA_TYPE,
        enum_Column_Info_MD_PREVIEW,
        enum_Column_Info_EXPORT_PATH,

        enum_Column_Info_SEARCH_LOCAL_FIELD,

        enum_Column_Info_HTML_REPORT_NAME,
        enum_Column_Info_HTML_REPORT_VISIBLE,
        enum_Column_Info_HTML_REPORT_ORDER,
        enum_Column_Info_HTML_REPORT_DATA_TYPE,

        enum_Column_Info_XML_REPORT_NAME,
        enum_Column_Info_XML_REPORT_VISIBLE,
        enum_Column_Info_XML_REPORT_ORDER,
        enum_Column_Info_XML_REPORT_DATA_TYPE,

        enum_Column_Info_CSV_REPORT_NAME,
        enum_Column_Info_CSV_REPORT_VISIBLE,
        enum_Column_Info_CSV_REPORT_ORDER,
        enum_Column_Info_CSV_REPORT_DATA_TYPE,
        enum_Column_Info_KML_REPORT_NAME,
        enum_Column_Info_KML_REPORT_VISIBLE,
        enum_Column_Info_KML_REPORT_ORDER,
        enum_Column_Info_KML_REPORT_DATA_TYPE,

        enum_Column_Info_GLOBAl_SEARCH_VISIBLE,
        enum_Column_Info_GLOBAl_SEARCH_ORDER,
        enum_Column_Info_GLOBAl_SEARCH_DATA_TYPE,


        enum_Column_Info_TIMELINE_VISIBLE,
        enum_Column_Info_TIMELINE_ORDER,
        enum_Column_Info_TIMELINE_DATA_TYPE,

        enum_Column_Info_EXTRA_1,
        enum_Column_Info_EXTRA_2,
        enum_Column_Info_EXTRA_3,
        enum_Column_Info_EXTRA_4,
        enum_Column_Info_EXTRA_5,
        enum_Column_Info_EXTRA_6,
        enum_Column_Info_EXTRA_7,
        enum_Column_Info_EXTRA_8,
        enum_Column_Info_EXTRA_9,
        enum_Column_Info_EXTRA_10,
        enum_Column_Info_EXTRA_11,
        enum_Column_Info_EXTRA_12,
        enum_Column_Info_EXTRA_13,
        enum_Column_Info_EXTRA_14,
        enum_Column_Info_EXTRA_15,
        enum_Column_Info_EXTRA_16,
        enum_Column_Info_EXTRA_17,
        enum_Column_Info_EXTRA_18,
        enum_Column_Info_EXTRA_19,
        enum_Column_Info_EXTRA_20

    };

    QString col_name;
    QString col_parent_child_type;
    QString col_tw_name;
    QString col_tw_visible;
    QString col_tw_order;
    QString col_tw_col_width;
    QString col_tw_data_type;

    QString col_md_name;
    QString col_md_visible;
    QString col_md_order;
    QString col_md_data_type;

    QString col_search_local_field;
    QString col_md_preview;
    QString col_export_path;
    QString col_html_report_name;
    QString col_html_report_visible;
    QString col_html_report_order;
    QString col_html_report_data_type;

    QString col_xml_report__name;
    QString col_xml_report_visible;
    QString col_xml_report_order;
    QString col_xml_report_data_type;

    QString col_csv_report_name;
    QString col_csv_report_visible;
    QString col_csv_report_order;
    QString col_csv_report_data_type;


    QString col_kml_report_name;
    QString col_kml_report_visible;
    QString col_kml_report_order;
    QString col_kml_report_data_type;

    QString col_global_search_visible;
    QString col_global_search_order;
    QString col_global_search_data_type;

    QString col_timeline_visible;
    QString col_timeline_order;
    QString col_timeline_data_type;

};

#endif //
