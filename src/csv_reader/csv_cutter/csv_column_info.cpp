#include "csv_column_info.h"

csv_column_info::csv_column_info(QWidget *parent)
{
}

QString csv_column_info::get_export_path()
{
    return col_export_path;
}

void csv_column_info::set_column_attribute(QString line)
{
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    QStringList line_list = line.split(MACRO_CSV_TEMPLATE_CSV_FILE_SEPRATOR);

    col_name = line_list.at(enum_Column_Info_COLUMN_NAME);
    col_name = col_name.trimmed();

    col_parent_child_type = line_list.at(enum_Column_Info_PARENT_CHILD_TYPE);
    col_parent_child_type = col_parent_child_type.trimmed();

    col_tw_name = line_list.at(enum_Column_Info_TW_NAME);
    col_tw_name = col_tw_name.trimmed();

    col_tw_visible = line_list.at(enum_Column_Info_TW_VISIBILITY);
    col_tw_visible = col_tw_visible.trimmed();

    col_tw_order = line_list.at(enum_Column_Info_TW_ORDER);
    col_tw_order = col_tw_order.trimmed();

    col_tw_col_width = line_list.at(enum_Column_Info_TW_COL_WIDTH);
    col_tw_col_width = col_tw_col_width.trimmed();


    col_tw_data_type = line_list.at(enum_Column_Info_TW_DATA_TYPE);
    col_tw_data_type = col_tw_data_type.trimmed();


    col_md_name = line_list.at(enum_Column_Info_MD_NAME);
    col_md_name = col_md_name.trimmed();

    col_md_visible = line_list.at(enum_Column_Info_MD_VISIBLITY);
    col_md_visible = col_md_visible.trimmed();

    col_md_order = line_list.at(enum_Column_Info_MD_ORDER);
    col_md_order = col_md_order.trimmed();

    col_md_data_type = line_list.at(enum_Column_Info_MD_DATA_TYPE);
    col_md_data_type = col_md_data_type.trimmed();

    col_md_preview = line_list.at(enum_Column_Info_MD_PREVIEW);
    col_md_preview = col_md_preview.trimmed();

    col_export_path = line_list.at(enum_Column_Info_EXPORT_PATH);
    col_export_path = col_export_path.trimmed();

    col_search_local_field = line_list.at(enum_Column_Info_SEARCH_LOCAL_FIELD);
    col_search_local_field = col_search_local_field.trimmed();




    col_html_report_name = line_list.at(enum_Column_Info_HTML_REPORT_NAME);
    col_html_report_name = col_html_report_name.trimmed();

    col_html_report_visible = line_list.at(enum_Column_Info_HTML_REPORT_VISIBLE);
    col_html_report_visible = col_html_report_visible.trimmed();

    col_html_report_order = line_list.at(enum_Column_Info_HTML_REPORT_ORDER);
    col_html_report_order = col_html_report_order.trimmed();

    col_html_report_data_type = line_list.at(enum_Column_Info_HTML_REPORT_DATA_TYPE);
    col_html_report_data_type = col_html_report_data_type.trimmed();




    col_xml_report__name = line_list.at(enum_Column_Info_XML_REPORT_NAME);
    col_xml_report__name = col_xml_report__name.trimmed();

    col_xml_report_visible = line_list.at(enum_Column_Info_XML_REPORT_VISIBLE);
    col_xml_report_visible = col_xml_report_visible.trimmed();

    col_xml_report_order = line_list.at(enum_Column_Info_XML_REPORT_ORDER);
    col_xml_report_order = col_xml_report_order.trimmed();

    col_xml_report_data_type = line_list.at(enum_Column_Info_XML_REPORT_DATA_TYPE);
    col_xml_report_data_type = col_xml_report_data_type.trimmed();


    col_csv_report_name = line_list.at(enum_Column_Info_CSV_REPORT_NAME);
    col_csv_report_name = col_csv_report_name.trimmed();

    col_csv_report_visible = line_list.at(enum_Column_Info_CSV_REPORT_VISIBLE);
    col_csv_report_visible = col_csv_report_visible.trimmed();

    col_csv_report_order = line_list.at(enum_Column_Info_CSV_REPORT_ORDER);
    col_csv_report_order = col_csv_report_order.trimmed();

    col_csv_report_data_type = line_list.at(enum_Column_Info_CSV_REPORT_DATA_TYPE);
    col_csv_report_data_type = col_csv_report_data_type.trimmed();

    col_kml_report_name = line_list.at(enum_Column_Info_KML_REPORT_NAME);
    col_kml_report_name = col_kml_report_name.trimmed();

    col_kml_report_visible = line_list.at(enum_Column_Info_KML_REPORT_VISIBLE);
    col_kml_report_visible = col_kml_report_visible.trimmed();

    col_kml_report_order = line_list.at(enum_Column_Info_KML_REPORT_ORDER);
    col_kml_report_order = col_kml_report_order.trimmed();

    col_kml_report_data_type = line_list.at(enum_Column_Info_KML_REPORT_DATA_TYPE);
    col_kml_report_data_type = col_kml_report_data_type.trimmed();

    col_global_search_visible = line_list.at(enum_Column_Info_GLOBAl_SEARCH_VISIBLE);
    col_global_search_visible = col_global_search_visible.trimmed();

    col_global_search_order = line_list.at(enum_Column_Info_GLOBAl_SEARCH_ORDER);
    col_global_search_order = col_global_search_order.trimmed();

    col_global_search_data_type = line_list.at(enum_Column_Info_GLOBAl_SEARCH_DATA_TYPE);
    col_global_search_data_type = col_global_search_data_type.trimmed();

    col_timeline_visible = line_list.at(enum_Column_Info_TIMELINE_VISIBLE);
    col_timeline_visible = col_timeline_visible.trimmed();

    col_timeline_order = line_list.at(enum_Column_Info_TIMELINE_ORDER);
    col_timeline_order = col_timeline_order.trimmed();

    col_timeline_data_type = line_list.at(enum_Column_Info_TIMELINE_DATA_TYPE);
    col_timeline_data_type = col_timeline_data_type.trimmed();


    recon_static_functions::debug_intensive(" end ",Q_FUNC_INFO);

}

QString csv_column_info::get_column_name()
{
    return col_name;
}

QString csv_column_info::get_column_parent_child_type()
{
    return col_parent_child_type;

}

QString csv_column_info::get_column_tw_display_name()
{
    return col_tw_name;
}

QString csv_column_info::get_column_tw_visibility()
{
    return col_tw_visible;
}

QString csv_column_info::get_column_tw_order()
{
    return col_tw_order;
}

QString csv_column_info::get_column_tw_col_width()
{
    return col_tw_col_width;
}


QString csv_column_info::get_column_tw_data_type()
{
    return col_tw_data_type;
}

QString csv_column_info::get_column_md_name()
{
    return col_md_name;
}

QString csv_column_info::get_column_md_visiblity()
{
    return col_md_visible;
}

QString csv_column_info::get_column_md_order()
{
    return col_md_order;
}

QString csv_column_info::get_column_md_data_type()
{
    return col_md_data_type;
}

QString csv_column_info::get_search_local_field()
{
    return col_search_local_field;
}

QString csv_column_info::get_md_preview()
{
    return col_md_preview;
}

QString csv_column_info::get_html_name()
{
    return col_html_report_name;
}

QString csv_column_info::get_html_visible()
{
    return col_html_report_visible;
}

QString csv_column_info::get_html_order()
{
    return col_html_report_order;
}

QString csv_column_info::get_html_report_data_type()
{
    return col_html_report_data_type;
}



QString csv_column_info::get_xml_name()
{
    return col_xml_report__name;
}

QString csv_column_info::get_xml_visible()
{
    return col_xml_report_visible;
}

QString csv_column_info::get_xml_order()
{
    return col_xml_report_order;
}

QString csv_column_info::get_xml_report_data_type()
{
    return col_xml_report_data_type;
}





QString csv_column_info::get_csv_name()
{
    return col_csv_report_name;
}

QString csv_column_info::get_csv_visible()
{
    return col_csv_report_visible;
}

QString csv_column_info::get_csv_order()
{
    return col_csv_report_order;
}

QString csv_column_info::get_csv_report_data_type()
{
    return col_csv_report_data_type;
}

QString csv_column_info::get_kml_name()
{
    return col_kml_report_name;
}

QString csv_column_info::get_g_search_visible()
{
  return col_global_search_visible;
}

QString csv_column_info::get_g_search_order()
{
    return col_global_search_order;

}

QString csv_column_info::get_g_search_data_type()
{
    return col_global_search_data_type;

}

QString csv_column_info::get_kml_visible()
{
    return col_kml_report_visible;
}

QString csv_column_info::get_kml_order()
{
    return col_kml_report_order;
}

QString csv_column_info::get_kml_report_data_type()
{
    return col_kml_report_data_type;
}

QString csv_column_info::get_timeline_visible()
{
    return col_timeline_visible;
}

QString csv_column_info::get_timeline_order()
{
    return col_timeline_order;
}

QString csv_column_info::get_timeline_data_type()
{
    return col_timeline_data_type;
}



void csv_column_info::print_col_info()
{
}
