#include "csv_table_info.h"


csv_table_info::csv_table_info(QWidget *parent)
{

}

void csv_table_info::set_table_attribute(QString line)
{
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    QStringList line_list = line.split(MACRO_CSV_TEMPLATE_CSV_FILE_SEPRATOR);

    table_name = line_list.at(enum_table_info_TABLE_NAME);
    table_name = table_name.trimmed();

    tab_widget_type = line_list.at(enum_table_info_TAB_WIDGET_TYPE);
    tab_widget_type = tab_widget_type.trimmed();

    tab_name = line_list.at(enum_table_info_TAB_NAME);
    tab_name = tab_name.trimmed();

    tab_visible = line_list.at(enum_table_info_TAB_VISIBILITY);
    tab_visible = tab_visible.trimmed();

    tab_order = line_list.at(enum_table_info_TAB_ORDER);
    tab_order = tab_order.trimmed();


    QString os_str = line_list.at(enum_table_info_TAB_SUPPORTED_OS_LIST);
    tab_supported_os_scheme_list = os_str.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));
    tab_supported_os_scheme_list.removeAll("");
    tab_supported_os_scheme_list.removeDuplicates();

    QString soft_str = line_list.at(enum_table_info_TAB_SUPPORTED_SOFTWARE_LIST);
    tab_supported_software_list = soft_str.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));
    tab_supported_software_list.removeAll("");
    tab_supported_software_list.removeDuplicates();

    log_show_supported = line_list.at(enum_table_info_LOG_SHOW_SUPPORTED);
    log_show_supported = log_show_supported.trimmed();

    root_privileged = line_list.at(enum_table_info_ROOT_PRIVILEGED);
    root_privileged = root_privileged.trimmed();

    user_iterated = line_list.at(enum_table_info_USER_ITERATED);
    user_iterated = user_iterated.trimmed();

    apple_metadata_available = line_list.at(enum_table_info_APPLE_METADATA_AVAILABLE);
    apple_metadata_available = apple_metadata_available.trimmed();

    export_available = line_list.at(enum_table_info_EXPORT_AVAILABLE);
    export_available = export_available.trimmed();


    artifact_common = line_list.at(enum_table_info_ARTIFACT_COMMON);
    artifact_common = artifact_common.trimmed();


    html_report_available = line_list.at(enum_table_info_HTML_REPORT_AVAILABLE);
    html_report_available = html_report_available.trimmed();

    html_report_header_name = line_list.at(enum_table_info_HTML_REPORT_HEADER_NAME);
    html_report_header_name = html_report_header_name.trimmed();

    html_report_header_width = line_list.at(enum_table_info_HTML_REPORT_HEADER_WIDTH);
    html_report_header_width = html_report_header_width.trimmed();


    xml_report_available = line_list.at(enum_table_info_XML_REPORT_AVAILABLE);
    xml_report_available = xml_report_available.trimmed();
    csv_report_available = line_list.at(enum_table_info_CSV_REPORT_AVAILABLE);
    csv_report_available = csv_report_available.trimmed();



    kml_report_available = line_list.at(enum_table_info_KML_REPORT_AVAILABLE);
    kml_report_available = kml_report_available.trimmed();


    global_search_available = line_list.at(enum_table_info_GLOBAL_SEARCH_AVAILABLE);
    global_search_available = global_search_available.trimmed();


    timeline_available = line_list.at(enum_table_info_TIMELINE_AVAILABLE);
    timeline_available = timeline_available.trimmed();


    QString tm_type = line_list.at(enum_table_info_TIMELINE_TIMESTAMP_TYPE);
    timeline_timestamp_type_stringlist = tm_type.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));
    timeline_timestamp_type_stringlist.removeAll("");

    QString tm_col = line_list.at(enum_table_info_TIMELINE_COLUMN_NAME);
    timeline_column_name_stringlist = tm_col.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));
    timeline_column_name_stringlist.removeAll("");


    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

}


QString csv_table_info::get_artifact_common()
{
    return artifact_common;
}

QString csv_table_info::get_table_name()
{
    return table_name;
}

QString csv_table_info::get_table_tab_widget_type()
{
    return tab_widget_type;

}

QString csv_table_info::get_tab_name()
{
    return tab_name;
}

QString csv_table_info::get_table_visibility()
{
    return tab_visible;
}

QString csv_table_info::get_table_order()
{
    return tab_order;
}

QString csv_table_info::get_table_html_report()
{
    return html_report_available;
}
QString csv_table_info::get_apple_metadata()
{
    return apple_metadata_available;
}

QString csv_table_info::get_log_show_supported()
{
    return log_show_supported;
}

QString csv_table_info::get_root_privileged()
{
    return root_privileged;
}

QString csv_table_info::get_user_iterated()
{
    return user_iterated;
}

QString csv_table_info::get_table_xml_report()
{
    return xml_report_available;
}

QString csv_table_info::get_table_csv_report()
{
    return csv_report_available;
}

QString csv_table_info::get_table_kml_report()
{
    return kml_report_available;
}

QStringList csv_table_info::get_table_tab_supported_os()
{
    return tab_supported_os_scheme_list;
}

QStringList csv_table_info::get_table_tab_supported_software()
{
    return tab_supported_software_list;
}


QString csv_table_info::get_table_export_available()
{
    return export_available;
}

QString csv_table_info::get_table_html_header()
{
    return html_report_header_name;
}

QString csv_table_info::get_table_header_column_width_html()
{
    return html_report_header_width;

}


QString csv_table_info::get_table_g_search_available()
{
    return global_search_available;
}

QString csv_table_info::get_table_timeline_available()
{
    return timeline_available;
}

QStringList csv_table_info::get_table_timeline_type()
{
    return timeline_timestamp_type_stringlist;
}

QStringList csv_table_info::get_table_timeline_column_name()
{
    return timeline_column_name_stringlist;
}

void csv_table_info::set_column_attribute(QString line)
{
    csv_column_info col_obj;
    col_obj.set_column_attribute(line);
    col_info_list << col_obj;
}

void csv_table_info::print_table_info()
{
    for(int i = 0 ; i < col_info_list.size() ; i++)
    {
        csv_column_info c1 = col_info_list.at(i);
        c1.print_col_info();
    }
}

QList<csv_column_info> csv_table_info::get_cols_list()
{
    return col_info_list;
}


