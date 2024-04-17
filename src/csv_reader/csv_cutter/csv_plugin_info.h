#ifndef CSV_PLUGIN_INFO_H
#define CSV_PLUGIN_INFO_H

#include <QObject>
#include <QWidget>
#include "header_csv_template.h"
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"


class csv_plugin_info : public QObject
{
    Q_OBJECT
public:
    explicit csv_plugin_info(QObject *parent = 0);
    ~csv_plugin_info();

    void set_plugin_attribute(QString line);
    QString get_plugin_name();
    QString get_plugin_category();
    QStringList get_supported_os_scheme();
    QStringList get_native_os();
    QString get_extraction_mode();
    QString get_redefined_result_availability();
    QStringList get_supported_software();
    QString get_log_show_status();
    QString get_root_privileged();
    QString get_user_iterated();



signals:

public slots:


private:

    enum enum_plugin_pub_info
    {
        enum_plugin_pub_info_plugin_name,
        enum_plugin_pub_info_plugin_category,
        enum_plugin_pub_info_plugin_native_os,
        enum_plugin_pub_info_plugin_supported_os_scheme,
        enum_plugin_pub_info_plugin_extraction_mode,
        enum_plugin_pub_info_plugin_available_for_redefined_result,
        enum_plugin_pub_info_plugin_supported_software_list,
        enum_plugin_pub_info_plugin_log_show_supported,
        enum_plugin_pub_info_plugin_root_privileged,
        enum_plugin_pub_info_plugin_user_iterated
    };


    QString plugin_name;
    QString plugin_category;
    QStringList native_os_list;
    QStringList supported_os_scheme_list;
    QString extraction_mode;
    QString available_for_redefined_result;
    QStringList supported_software_list;
    QString log_show_supported;
    QString root_privileged;
    QString user_iterated;

};

#endif
