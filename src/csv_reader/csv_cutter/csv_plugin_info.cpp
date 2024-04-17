#include "csv_plugin_info.h"

csv_plugin_info::csv_plugin_info(QObject *parent) : QObject(parent)
{

}

csv_plugin_info::~csv_plugin_info()
{

}

void csv_plugin_info::set_plugin_attribute(QString line)
{
    recon_static_functions::debug_intensive("start ",Q_FUNC_INFO);

    QStringList line_list = line.split(MACRO_CSV_TEMPLATE_CSV_FILE_SEPRATOR);

    plugin_name = line_list.at(enum_plugin_pub_info_plugin_name);
    plugin_name = plugin_name.trimmed();

    plugin_category = line_list.at(enum_plugin_pub_info_plugin_category);
    plugin_category = plugin_category.trimmed();


    QString os_schemes = line_list.at(enum_plugin_pub_info_plugin_supported_os_scheme);
    if(os_schemes.contains(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR)))
    {
        supported_os_scheme_list = os_schemes.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));
    }
    else
    {
        supported_os_scheme_list = os_schemes.split("-");
    }

    QString ntv_os = line_list.at(enum_plugin_pub_info_plugin_native_os);
    if(ntv_os.contains(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR)))
    {
        native_os_list = ntv_os.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));
    }
    else
    {
        native_os_list = ntv_os.split("-");
    }

    extraction_mode = line_list.at(enum_plugin_pub_info_plugin_extraction_mode);
    extraction_mode = extraction_mode.trimmed();

    available_for_redefined_result = line_list.at(enum_plugin_pub_info_plugin_available_for_redefined_result);
    available_for_redefined_result = available_for_redefined_result.trimmed();


    QString sft_str = line_list.at(enum_plugin_pub_info_plugin_supported_software_list);
    supported_software_list = sft_str.split(QString(MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR));


    log_show_supported = line_list.at(enum_plugin_pub_info_plugin_log_show_supported);
    log_show_supported = log_show_supported.trimmed();


    root_privileged = line_list.at(enum_plugin_pub_info_plugin_root_privileged);
    root_privileged = root_privileged.trimmed();

    user_iterated = line_list.at(enum_plugin_pub_info_plugin_user_iterated);
    user_iterated = user_iterated.trimmed();

    recon_static_functions::debug_intensive("end ",Q_FUNC_INFO);

}

QString csv_plugin_info::get_plugin_name()
{
    return plugin_name;
}

QString csv_plugin_info::get_plugin_category()
{
    return plugin_category;
}

QStringList csv_plugin_info::get_supported_os_scheme()
{
    return supported_os_scheme_list;
}

QStringList csv_plugin_info::get_native_os()
{
    return native_os_list;
}

QString csv_plugin_info::get_extraction_mode()
{
    return extraction_mode;
}

QString csv_plugin_info::get_redefined_result_availability()
{
    return available_for_redefined_result;
}

QStringList csv_plugin_info::get_supported_software()
{
    return supported_software_list;
}

QString csv_plugin_info::get_log_show_status()
{
    return log_show_supported;
}

QString csv_plugin_info::get_root_privileged()
{
    return root_privileged;
}

QString csv_plugin_info::get_user_iterated()
{
    return user_iterated;
}
