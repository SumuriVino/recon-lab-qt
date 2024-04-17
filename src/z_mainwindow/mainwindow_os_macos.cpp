#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::set_r_case_essentials_macos()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString virtual_source_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString src_type_intrnl = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString();


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString,recon_static_functions::extract_os_version_full_xcode_mm_macOS(virtual_source_path, Q_FUNC_INFO));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString,recon_static_functions::extract_os_version_extended_xcode_mm_macOS(virtual_source_path, Q_FUNC_INFO));

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Build_Version_QString,recon_static_functions::extract_build_version_xcode_mm_macOS(virtual_source_path, Q_FUNC_INFO));


    QList <struct_global_system_account_info> sys_account_list_for_macos = get_system_account_QList_for_macos(virtual_source_path,src_type_intrnl);
    QList<QVariant> qvariant_list;
    qvariant_list.append(QVariant::fromValue(sys_account_list_for_macos));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_System_Account_QList , qvariant_list);

    QStringList sys_users_list = recon_static_functions::get_system_user_list_from_system_account_QList(sys_account_list_for_macos , Q_FUNC_INFO);

    QStringList config_ids = recon_helper_standard_obj->extract_macos_current_login_apple_ids(virtual_source_path, sys_users_list);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, config_ids);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString,recon_static_functions::extract_model_number_xcode_mm_macOS(virtual_source_path, Q_FUNC_INFO));


    struct_global_macOS_global_preferences obj_pref = recon_static_functions::extract_global_preferences_xcode_mm_macOS(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString(), Q_FUNC_INFO);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString,obj_pref.country);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString,obj_pref.city_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString,obj_pref.latitude);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString,obj_pref.longitude);

    QString target_timezone = recon_helper_standard_obj->get_complete_timezone_name_label(recon_static_functions::extract_time_zone_macOS(virtual_source_path, Q_FUNC_INFO),Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(target_timezone,Q_FUNC_INFO);
    if(tmzone_offset == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());

        //We can not set timezone value to 0 because 0 value may corresponds to UTC
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,-1);
    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,target_timezone);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);
    }

    //This is for extract macOS installation timestamps
    QString mounted_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString os_installer_file_path = mounted_path + QString("/private/var/db/.AppleSetupDone");
    if(QFileInfo(os_installer_file_path).exists())
    {
        struct stat fileStat;
        bzero(&fileStat,sizeof(fileStat));
        lstat(os_installer_file_path.toLocal8Bit().data(),&fileStat);      // File Info using stat
        qint64 modification_timestamp = fileStat.st_mtimespec.tv_sec;

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64, modification_timestamp);
    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}


void MainWindow::set_r_case_essentials_otherOS()
{

}

void MainWindow::set_r_case_essentials_gtakeoutos()
{

    QString source_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString() + "/index.html";

    QFile file(source_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(" file open ---FAILED--- " + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QString email_logged;
    QStringList snippet_list ;

    while(!file.atEnd())
    {
        QString line = file.readLine();
        line = line.replace("\r\n ","");

        if(line.contains("Archive for"))
        {
            QString text = line;

            QTextDocument doc ;
            doc.setHtml(text);

            QString snippet = doc.toPlainText();
            snippet = snippet.trimmed();

            snippet_list << snippet.split("\n");

            break;
        }
    }

    for(int ss= 0 ; ss < snippet_list.size() ; ss++)
    {
        QString line  = snippet_list.at(ss);
        if(line.startsWith("Archive for "))
        {
            email_logged = line.remove("Archive for ");
            break;
        }
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, QStringList(email_logged));

}
