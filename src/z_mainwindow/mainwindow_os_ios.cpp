#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::set_r_case_essentials_ios_uid()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString ios_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    ios_bkup_path = ios_bkup_path.trimmed();

    if(ios_bkup_path.isEmpty())
    {
        recon_static_functions::app_debug("iOS Backup source path empty -----FAILED-----" + ios_bkup_path , Q_FUNC_INFO);
        return;
    }

    if(!ios_bkup_path.endsWith("/"))
        ios_bkup_path.append("/");

    QString ios_info_path = ios_bkup_path + QString("Info.plist");

    if(!QFileInfo(ios_info_path).exists())
    {
        recon_static_functions::app_debug("iOS Backup info file not exist -----FAILED-----" + ios_info_path , Q_FUNC_INFO);
        return;
    }

    /// extract size in 1 sec of 900 MB iOS Backup
    QString ios_total_size = recon_static_functions::mining_size(QFileInfo(ios_info_path).absoluteDir().absolutePath(), Q_FUNC_INFO);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,ios_total_size);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,"0");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,ios_total_size);

    QString backup_path = QFileInfo(ios_info_path).absolutePath();
    struct_global_ios_uid_backup_detail mobj = recon_static_functions::extract_backup_details_iOS_uid(ios_info_path, Q_FUNC_INFO);
    ios_uid_backup_parser *ios_parser_obj = new ios_uid_backup_parser(this);
    ios_parser_obj->pub_set_backup_path(backup_path);
    ios_parser_obj->pub_set_os_version(mobj.product_version);
    ios_parser_obj->pub_parse_backup();
    ios_uid_backup_parser_obj = ios_parser_obj;

    QString account_db_path = ios_parser_obj->pub_get_artifact_source_file_uid_path("HomeDomain/Library/Accounts/Accounts3.sqlite");
    QString config_id = extract_iOS_configured_id(account_db_path);
    QString pref_path =  ios_parser_obj->pub_get_artifact_source_file_uid_path("HomeDomain/Library/Preferences/com.apple.preferences.datetime.plist");

    QString city_name = recon_static_functions::extract_time_zone_xcode_mm_iOS(pref_path, Q_FUNC_INFO);
    QString timezone_db_path = "../Resources/iOS/CityInfo.db";
    QString tm_command = QString("select timezonename,latitude,longitude,locale_code from cities where name = ?");
    QString tm_zone = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 0, timezone_db_path, Q_FUNC_INFO);
    QString lati = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 1, timezone_db_path, Q_FUNC_INFO);

    QString longi = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 2, timezone_db_path, Q_FUNC_INFO);
    QString cntry = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 3, timezone_db_path, Q_FUNC_INFO);


    tm_zone = recon_helper_standard_obj->get_complete_timezone_name_label(tm_zone,Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(tm_zone,Q_FUNC_INFO);
    if(tmzone_offset == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,-1);

    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,tm_zone);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);
    }


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString, city_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString, cntry);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString, lati);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString, longi);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Device_Name_QString, mobj.device_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Id_No_QString, mobj.target_identifier);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Imei_QStringList, mobj.imei_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString, global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(QString::number(mobj.last_backup_date_numeric_qint64), Q_FUNC_INFO));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64, mobj.last_backup_date_numeric_qint64);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Serial_No_QString, mobj.serial_number);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList, mobj.phone_no_msisdn_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString, mobj.product_type);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString, mobj.product_version);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_iTune_Version_QString, mobj.itune_version);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString, mobj.backup_size_byte_numeric_QString);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Build_Version_QString, mobj.build_version);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Display_Name_QString, mobj.display_name);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString, mobj.display_name);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_GUID_QString, mobj.guid);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Name_QString, mobj.product_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Target_Type_QString, mobj.target_type);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList, mobj.sim_no_iccid_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, QStringList(config_id));


    QStringList list_ver_sepr = mobj.product_version.split(".", Qt::SkipEmptyParts);
    QString os_full_version;

    if(list_ver_sepr.size() > 1)
    {
        os_full_version = list_ver_sepr.at(0) + "." + list_ver_sepr.at(1);
    }
    else
    {
        os_full_version = list_ver_sepr.at(0);
    }


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString, os_full_version);

    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Model_QString, struct_device_info_obj.product_model);
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString, struct_device_info_obj.bluetooth_device_address);
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Wifi_Address_QString, struct_device_info_obj.wifi_address);
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Locale_Language_QString, struct_device_info_obj.locale_language);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

QString MainWindow::extract_iOS_configured_id(QString db_path)
{
    QString config_id;

    QString cmd1 = "select ZUSERNAME from ZACCOUNT where ZACCOUNTDESCRIPTION = ?";
    config_id = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd1,QStringList("iCloud"),0, db_path, Q_FUNC_INFO);

    //====================== Do not delete start====================//
    /// this is for extracting all configured/authenticated ids yet
    //    QString cmd2 = "select ZUSERNAME from ZACCOUNT where ZOWNINGBUNDLEID = ?";
    //    config_ids.authenticated_ids = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(cmd2,QStringList("com.apple.AuthKit"),0, source_db, Q_FUNC_INFO);
    //====================== Do not delete end====================//

    return config_id;
}

void MainWindow::set_r_case_essentials_ios_xml()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString ios_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    ios_bkup_path = ios_bkup_path.trimmed();

    if(ios_bkup_path.isEmpty())
    {
        recon_static_functions::app_debug("iOS Backup source path empty -----FAILED-----" + ios_bkup_path , Q_FUNC_INFO);
        return;
    }

    if(!ios_bkup_path.endsWith("/"))
        ios_bkup_path.append("/");

    QString report_xml_file_path = ios_bkup_path + QString("report.xml");

    if(!QFileInfo(report_xml_file_path).exists())
    {
        recon_static_functions::app_debug("iOS Backup report.xml file not exist -----FAILED-----" + report_xml_file_path , Q_FUNC_INFO);
        return;
    }


    ios_xml_backup_parser *ios_parser_obj = new ios_xml_backup_parser(this);
    ios_parser_obj->pub_set_ios_xml_file_path(report_xml_file_path);
    ios_parser_obj->pub_parse_ios_xml_device_info();

    struct_ios_xml_device_info struct_device_info_obj = ios_parser_obj->pub_get_struct_device_info_obj();

    QString timezone_str = struct_device_info_obj.timezone;

    QStringList tmp_list = timezone_str.split(" ",Qt::SkipEmptyParts);
    QString timezone_offset , city_name;
    if(tmp_list.size() > 2)
    {
        timezone_offset = tmp_list.at(0); //(UTC-5:00)
        city_name = tmp_list.at(1); // New_York
        if(city_name.contains("_"))
        {
            city_name = city_name.replace("_"," ");
        }
    }

    QString timezone_db_path = "../Resources/iOS/CityInfo.db";
    QString tm_command = QString("select timezonename,latitude,longitude,locale_code from cities where name = ?");
    QString tm_zone = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 0, timezone_db_path, Q_FUNC_INFO);
    QString latitude = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 1, timezone_db_path, Q_FUNC_INFO);

    QString longitude = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 2, timezone_db_path, Q_FUNC_INFO);
    QString country = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(tm_command,QStringList(city_name), 3, timezone_db_path, Q_FUNC_INFO);

    tm_zone = recon_helper_standard_obj->get_complete_timezone_name_label(tm_zone,Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(tm_zone,Q_FUNC_INFO);
    if(tmzone_offset == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,-1);
    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,tm_zone);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString, city_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString, country);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString, latitude);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString, longitude);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Device_Name_QString, struct_device_info_obj.device_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Id_No_QString, struct_device_info_obj.unique_identifier);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Imei_QStringList,struct_device_info_obj.imei_QStringList);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Serial_No_QString, struct_device_info_obj.serial_number);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString, struct_device_info_obj.product_type);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString, struct_device_info_obj.product_version);

    if(!struct_device_info_obj.display_name.trimmed().isEmpty())
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Display_Name_QString, struct_device_info_obj.display_name);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString, struct_device_info_obj.display_name);
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Name_QString, struct_device_info_obj.product_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList, struct_device_info_obj.sim_no_iccid_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, QStringList(struct_device_info_obj.apple_id));

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Model_QString, struct_device_info_obj.product_model);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList, struct_device_info_obj.phone_no_msisdn_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString, struct_device_info_obj.bluetooth_device_address);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Wifi_Address_QString, struct_device_info_obj.wifi_address);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Locale_Language_QString, struct_device_info_obj.locale_language);

    QString os_full_version;

    if(!struct_device_info_obj.product_version.trimmed().isEmpty())
    {
        QStringList list_ver_sepr = struct_device_info_obj.product_version.split(".", Qt::SkipEmptyParts);
        if(list_ver_sepr.size() > 1)
        {
            os_full_version = list_ver_sepr.at(0) + "." + list_ver_sepr.at(1);
        }
        else
        {
            os_full_version = list_ver_sepr.at(0);
        }
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString, os_full_version);


    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

QString MainWindow::get_extracted_cellebrite_ios_tar_backup_path()
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);


    QString source_path1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();

    QString available_file_name = recon_static_functions::get_available_filename("Backup_Cellebrite_Tar",bkup_location, Q_FUNC_INFO);
    decompressed_cellebrite_ios_tar_backup_path = bkup_location + available_file_name;
    QString bkup_path = decompressed_cellebrite_ios_tar_backup_path;

    QDir dir;
    dir.mkpath(bkup_path);

    QString tar_filepath = source_path1;

    //Added timer to show the count of files on progressing bar while decompressing
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this , SLOT(slot_cellebrite_ios_tar_source_decompression_timeout()));
    timer->start(3000);

    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting Cellebrite iOS Backup! Please wait...(0)");
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList args_list;

    args_list << "-xvf" << tar_filepath << "-C" << bkup_path;
    recon_helper_process_ios_obj->run_command_with_arguments("tar", args_list, Q_FUNC_INFO);

    QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(bkup_path.startsWith(result_dir))
    {
        //Chmod is used becuase some file/folder do not have permission to traverse.
        //Also We dont extract timstamps for Cellebrite iOS tar.

        args_list.clear();
        args_list << "-R" << "+r" << bkup_path;
        recon_helper_process_ios_obj->run_command_with_arguments("chmod", args_list, Q_FUNC_INFO);
    }

    QProcess proc;
    QStringList args;
    args << "-a" << "L" << bkup_path;
    proc.execute("SetFile", args);

    display_loading_progress_bar_non_cancelable_obj->hide();
    timer->stop();

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

    return bkup_path;

}

QString MainWindow::get_extracted_cellebrite_ios_ufdr_backup_path()
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);


    QString source_path1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();

    QString available_file_name = recon_static_functions::get_available_filename("Backup_Cellebrite_UFDR",bkup_location, Q_FUNC_INFO);

    QString bkup_path = bkup_location + available_file_name;
    QDir dir;
    dir.mkpath(bkup_path);

    bkup_path = bkup_path.trimmed();

    QString tar_filepath = source_path1;

    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting Cellebrite iOS Backup! Please wait...");
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList args_list;

    args_list << "-xvf" << tar_filepath << "-C" << bkup_path;
    recon_helper_process_ios_obj->run_command_with_arguments("tar", args_list, Q_FUNC_INFO);

    QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QProcess proc;
    QStringList args;
    args << "-a" << "L" << bkup_path;
    proc.execute("SetFile", args);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

    return bkup_path;

}

QString MainWindow::get_extracted_ios_graykey_backup_path()
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    QString source_path1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();

    QString available_file_name = recon_static_functions::get_available_filename("GrayKey_iOS_Zip",bkup_location, Q_FUNC_INFO);

    QString source_name = QDir(source_path1).dirName();

    if(source_name.endsWith(".zip",Qt::CaseInsensitive))
    {
        source_name.chop(QString(".zip").size());
    }
    source_name = source_name.replace(" ","_");

    decompressed_graykey_ios_backup_path = bkup_location + available_file_name + "/" + source_name;
    QString bkup_path = decompressed_graykey_ios_backup_path;

    QDir dir;
    dir.mkpath(bkup_path);

    QString graykey_filepath = source_path1;

    //Added timer to show the count of files on progressing bar while decompressing
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this , SLOT(slot_graykey_ios_source_decompression_timeout()));
    timer->start(3000);

    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting GrayKey iOS Dump! Please wait...(0)");
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList args_list;

    args_list << "-xvf" << graykey_filepath << "-C" << bkup_path;
    recon_helper_process_ios_obj->run_command_with_arguments("tar", args_list, Q_FUNC_INFO);

    timer->stop();

    QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    if(bkup_path.startsWith(result_dir))
    {
        //chmod is used becuase some file/folder do not have permission to traverse(decompressed backup location).
        //As we are calculating timestamp from fuse parallely so there will be no moifications in the timestamps(mount point).
        //Decompressed backup location and mount point will never be same.
        //So we can use chmod to change permission for this source.

        args_list.clear();
        args_list << "-R" << "+rx" << bkup_path;
        recon_helper_process_ios_obj->run_command_with_arguments("chmod", args_list, Q_FUNC_INFO);
    }

    QProcess proc;
    QStringList args;
    args << "-a" << "L" << bkup_path;
    proc.execute("SetFile", args);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

    return bkup_path;

}

void MainWindow::set_r_case_essentials_ios_uniform()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString ios_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    ios_bkup_path = ios_bkup_path.trimmed();

    if(ios_bkup_path.isEmpty())
    {
        recon_static_functions::app_debug(" iOS uniform Backup source path empty -----FAILED-----" + ios_bkup_path , Q_FUNC_INFO);
        return;
    }

    if(!ios_bkup_path.endsWith("/"))
        ios_bkup_path.append("/");


    struct_global_ios_uniform_backup_detail mobj;

    recon_static_functions::clear_variables_ios_uniform_backup_structure(mobj);

    recon_static_functions::get_graykey_device_name_and_timezone_plist(ios_bkup_path,mobj,Q_FUNC_INFO);
    recon_static_functions::get_graykey_product_type_and_country_code_plist(ios_bkup_path,mobj,Q_FUNC_INFO);
    recon_static_functions::get_graykey_imei_phone_and_iccid_no_plist(ios_bkup_path,mobj,Q_FUNC_INFO);
    recon_static_functions::get_graykey_serial_no_plist(ios_bkup_path,mobj,Q_FUNC_INFO);
    recon_static_functions::get_graykey_device_guid_plist(ios_bkup_path,mobj,Q_FUNC_INFO);
    recon_static_functions::get_graykey_product_vrsn_build_model_plist(ios_bkup_path,mobj,Q_FUNC_INFO);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,"0");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,"0");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,"0");

    QString zone_name = mobj.time_zone;
    QString tm_zone = recon_helper_standard_obj->get_complete_timezone_name_label(zone_name, Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(tm_zone,Q_FUNC_INFO);
    if(tmzone_offset == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString(tm_zone));
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);

    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,tm_zone);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);
    }


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString, "");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString, mobj.country_code);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString, "");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString, "");

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Device_Name_QString,mobj.device_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Id_No_QString, "");


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Imei_QStringList, mobj.imei_number_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString,"");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64, 0);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Serial_No_QString, mobj.serial_number);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList, mobj.phone_number_mssid_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString, mobj.product_type);
    QString os_version_ext = mobj.product_version;
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString, os_version_ext);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_iTune_Version_QString, "");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString, 0);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Build_Version_QString, mobj.product_build_version);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Display_Name_QString, mobj.display_name);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString, mobj.display_name);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_GUID_QString, mobj.device_guid);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Name_QString, mobj.product_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Target_Type_QString, "");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList, mobj.iccid_number_QStringList);

    QString config_id_db_path = ios_bkup_path + "private/var/mobile/Library/Accounts/Accounts3.sqlite";
    QString config_id = extract_iOS_configured_id(config_id_db_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, QStringList(config_id));

    QStringList list_ver_sepr = os_version_ext.split(".", Qt::SkipEmptyParts);
    QString os_full_version;

    if(!mobj.product_version.trimmed().isEmpty())
    {
        if(list_ver_sepr.size() > 1)
        {
            os_full_version = list_ver_sepr.at(0) + "." + list_ver_sepr.at(1);
        }
        else
        {
            os_full_version = list_ver_sepr.at(0);
        }
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString, os_full_version);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Model_QString, mobj.product_model);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString, "");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Wifi_Address_QString, "");
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Locale_Language_QString, mobj.locale_language);



    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);


}

void MainWindow::slot_graykey_ios_source_decompression_timeout()
{
    QDirIterator iter(decompressed_graykey_ios_backup_path,QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    qint64 counter = 0;
    while(iter.hasNext())
    {
        QCoreApplication::processEvents();
        iter.next();
        counter++;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting GrayKey iOS Dump! Please wait...(" + QString::number(counter) + ")");
}

void MainWindow::slot_cellebrite_ios_tar_source_decompression_timeout()
{
    QDirIterator iter(decompressed_cellebrite_ios_tar_backup_path,QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    qint64 counter = 0;
    while(iter.hasNext())
    {
        QCoreApplication::processEvents();
        iter.next();
        counter++;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting Cellebrite iOS Backup! Please wait...(" + QString::number(counter) + ")");

}
