#include "mainwindow.h"

void MainWindow::set_r_case_essentials_androidos()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    android_bkup_path = android_bkup_path.trimmed();

    if(android_bkup_path.isEmpty())
    {
        recon_static_functions::app_debug(" android Backup source path empty -----FAILED-----" + android_bkup_path , Q_FUNC_INFO);
        return;
    }

    if(!android_bkup_path.endsWith("/"))
        android_bkup_path.append("/");


    struct_global_android_backup_detail struct_android_details_obj;
    clear_android_backup_detail_info_structure(struct_android_details_obj);

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString() == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal))
    {
        extract_country_code_for_graykey_android_backup(struct_android_details_obj);
        extract_imei_no_for_graykey_android_backup(struct_android_details_obj);
        extract_iccid_no_for_graykey_android_backup(struct_android_details_obj);
        extract_configured_id_for_graykey_android_backup(struct_android_details_obj);
        extract_bluetooth_address_device_id_for_graykey_android_backup(struct_android_details_obj);
        extract_phone_no_for_graykey_android_backup(struct_android_details_obj);
        extract_product_version_name_model_language_for_graykey_android_backup(struct_android_details_obj);
        extract_serial_no_for_graykey_android_backup(struct_android_details_obj);
        extract_timezone_for_graykey_android_backup(struct_android_details_obj);
    }
    else
    {

        struct_android_details_obj.configured_ids_QStringList = extract_androidos_configured_id();
        struct_android_details_obj.time_zone = extract_androidos_system_timezone();
    }

    struct_android_details_obj.time_zone = recon_helper_standard_obj->get_complete_timezone_name_label(struct_android_details_obj.time_zone,Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(struct_android_details_obj.time_zone,Q_FUNC_INFO);

    if(tmzone_offset == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,-1);
    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,struct_android_details_obj.time_zone);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Country_QString, struct_android_details_obj.country_code);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Imei_QStringList, struct_android_details_obj.imei_number_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList, struct_android_details_obj.phone_number_mssid_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Build_Version_QString, struct_android_details_obj.product_build_version);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Name_QString, struct_android_details_obj.product_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList, struct_android_details_obj.iccid_number_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Model_QString,struct_android_details_obj.product_model);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Locale_Language_QString,struct_android_details_obj.locale_language);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Serial_No_QString,struct_android_details_obj.serial_number);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Configured_ID_QStringList, struct_android_details_obj.configured_ids_QStringList);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString,struct_android_details_obj.bluetooth_address);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Id_No_QString, struct_android_details_obj.device_id);

    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_City_QString, "Static-Karnal");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latitude_QString, 29); // static
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Longitude_QString, 79); // static
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Device_Name_QString, "Static-REdmi");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Display_Name_QString, "Static-Redmi forenso");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,"Static-100");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,"Static-0");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,"Static-200");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString,"Static-May-25");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64, QString("Static-May-25").toLongLong()); // static
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Product_Type_QString, "Static - pixel");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString, "Static - 32GB");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString, "Static - Forenso Phone");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_GUID_QString, "Static - 65023489");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Target_Type_QString, "Static - RnD");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString,"Static-8.0.1");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString ,"Static - 8.0");
    //    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Wifi_Address_QString,"Static - 18:65:90:02:9b:bd");

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

QStringList MainWindow::extract_androidos_configured_id()
{
    QStringList cnfg_id_list;
    QString virtual_source_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString db_path = virtual_source_path + "apps/com.google.android.apps.docs/db/DocList.db";
    if(!QFileInfo(db_path).exists())
    {
        recon_static_functions::app_debug("android config ID file not exist ---FAILED--- " + db_path, Q_FUNC_INFO);
        return cnfg_id_list;
    }

    QString command = "select distinct accountHolderName from Account237";
    cnfg_id_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, db_path, Q_FUNC_INFO);

    return cnfg_id_list;

}

QString MainWindow::extract_androidos_system_timezone()
{
    QString time_zone;
    QString virtual_source_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString file_path = virtual_source_path + "apps/com.google.android.deskclock/sp/com.google.android.deskclock_preferences.xml";
    if(!QFileInfo(file_path).exists())
    {
        recon_static_functions::app_debug("android timezone file not exist ---FAILED--- " + file_path, Q_FUNC_INFO);
        return time_zone;
    }

    QFile myfile(file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("android timezone file not opened ---FAILED--- " + myfile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(), Q_FUNC_INFO);
        return time_zone;
    }


    QXmlStreamReader xml(&myfile);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartElement)
        {
            QXmlStreamAttributes attributes = xml.attributes();

            if(attributes.value("name").toString() == "home_time_zone")
            {
                time_zone = xml.readElementText();
            }
        }
    }

    myfile.close();

    return time_zone;

}


QString MainWindow::get_extracted_android_ab_file_backup_path()
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    QString source_path1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();

    QString available_file1 = recon_static_functions::get_available_filename("ADB_Android_AB",bkup_location, Q_FUNC_INFO);

    //    QString bkup_path = bkup_location + available_file1;

    //---
    QString source_name = QDir(source_path1).dirName();

    if(source_name.endsWith(".ab",Qt::CaseInsensitive))
    {
        source_name.chop(QString(".ab").size());
    }
    source_name = source_name.replace(" ","_");

    QString bkup_path = bkup_location + available_file1 + "/" + source_name;;
    //---
    QDir dir;
    dir.mkpath(bkup_path);

    //    QString available_file2 = recon_static_functions::get_available_filename("adb_backup.tar",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
    //    QString tar_filepath = MACRO_RECON_Temp_DirPath_In_Bundle + available_file2;

    QString available_file2 = recon_static_functions::get_available_filename("adb_backup.tar",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString tar_filepath = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file2;


    QStringList args_list;

    args_list << "-jar" << "./adb_android/abe.jar" << "unpack" << source_path1 << tar_filepath;

    //    recon_helper_process_android_obj->run_command_with_arguments("./adb_android/java", args_list, Q_FUNC_INFO);
    recon_helper_process_android_obj->run_command_with_arguments("/usr/bin/java", args_list, Q_FUNC_INFO);

    args_list.clear();
    args_list << "-xvf" << tar_filepath << "-C" << bkup_path;
    recon_helper_process_android_obj->run_command_with_arguments("tar", args_list, Q_FUNC_INFO);


    if(QFile(tar_filepath).exists())
        QFile::remove(tar_filepath);


    QProcess proc;
    QStringList args;
    args << "-a" << "L" << bkup_path;
    proc.execute("SetFile", args);

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

    return bkup_path;

}

QString MainWindow::get_extracted_android_graykey_backup_path()
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    QString source_path1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();
    QString available_file_name = recon_static_functions::get_available_filename("GrayKey_Android_Zip",bkup_location, Q_FUNC_INFO);

    QString source_name = QDir(source_path1).dirName();

    if(source_name.endsWith(".zip",Qt::CaseInsensitive))
    {
        source_name.chop(QString(".zip").size());
    }
    source_name = source_name.replace(" ","_");

    decompressed_graykey_android_backup_path = bkup_location + available_file_name + "/" + source_name;
    QString bkup_path = decompressed_graykey_android_backup_path;

    QDir dir;
    dir.mkpath(bkup_path);

    QString graykey_filepath = source_path1;

    //Added timer to show the count of files on progressing bar while decompressing
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this , SLOT(slot_graykey_android_source_decompression_timeout()));
    timer->start(3000);

    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting GrayKey Android Dump! Please wait...(0)");
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

void MainWindow::slot_graykey_android_source_decompression_timeout()
{
    QDirIterator iter(decompressed_graykey_android_backup_path,QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    qint64 counter = 0;
    while(iter.hasNext())
    {
        QCoreApplication::processEvents();
        iter.next();
        counter++;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Extracting GrayKey Android Dump! Please wait...(" + QString::number(counter) + ")");
}

void MainWindow::extract_product_version_name_model_language_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString product_info_file_path = android_bkup_path + "system/build.prop" ;

    QFile product_info_file(product_info_file_path);

    if(!product_info_file.exists())
    {
        recon_static_functions::app_debug("graykey android info file not exist ---FAILED--- " + product_info_file_path , Q_FUNC_INFO);
        return;
    }

    if(!product_info_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("graykey android info file not open ---FAILED--- " + product_info_file_path , Q_FUNC_INFO);
        return;
    }

    while(!product_info_file.atEnd())
    {
        QString line_str = QString::fromLocal8Bit(product_info_file.readLine()).trimmed();

        if(line_str.contains("ro.system.build.version.release="))
        {
            mobj.product_build_version = line_str.remove("ro.system.build.version.release=");
        }
        else if(line_str.contains("ro.product.system.brand="))
        {
            mobj.product_name = line_str.remove("ro.product.system.brand=");
        }
        else if(line_str.contains("ro.product.system.model="))
        {
            mobj.product_model = line_str.remove("ro.product.system.model=");
        }
        else if(line_str.contains("ro.product.locale="))
        {
            mobj.locale_language = line_str.remove("ro.product.locale=");
        }
    }
    product_info_file.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::extract_country_code_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString dir_iter_info_path = android_bkup_path + "data/user_de/";

    QDirIterator iter(dir_iter_info_path,QDir::AllDirs | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);
    while(iter.hasNext())
    {
        dir_iter_info_path = iter.next();

        QString sim_info_db_path = dir_iter_info_path + "/com.android.providers.telephony/databases/telephony.db";

        if(!QFileInfo(sim_info_db_path).exists())
        {
            recon_static_functions::app_debug("graykey android info db not exists ---FAILED--- " + sim_info_db_path, Q_FUNC_INFO);
            continue;
        }

        if(mobj.country_code.isEmpty())
        {
            QString cmd_select_country = "select iso_country_code from siminfo";
            mobj.country_code = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_select_country, 0, sim_info_db_path, Q_FUNC_INFO);
        }
    }
    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::extract_phone_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString dir_iter_info_path = android_bkup_path + "data/user_de/";

    QDirIterator iter(dir_iter_info_path,QDir::AllDirs | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);
    while(iter.hasNext())
    {
        dir_iter_info_path = iter.next();

        QString phone_no_path = dir_iter_info_path + "/com.android.server.telecom/files/phone-account-registrar-state.xml";

        QFile phone_no_file(phone_no_path);

        if(!phone_no_file.exists())
        {
            recon_static_functions::app_debug("graykey android backup phone_no file not exists ---FAILED--- " + phone_no_path, Q_FUNC_INFO);
            continue;
        }

        if(!phone_no_file.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("graykey android backup phone_no file not opened ---FAILED--- " + phone_no_path, Q_FUNC_INFO);
            continue;
        }

        while(!phone_no_file.atEnd())
        {
            QString line_str = phone_no_file.readLine().trimmed();
            if(line_str.contains("<subscription_number>"))
            {
                QString number = line_str.remove("<subscription_number>");
                number = number.remove("</subscription_number>");
                if(number.startsWith("tel:"))
                {
                    number = number.remove("tel:");
                    mobj.phone_number_mssid_QStringList << number;
                }
            }
        }


        mobj.phone_number_mssid_QStringList.removeDuplicates();
        phone_no_file.close();
    }

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::extract_imei_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString imei_no_path = android_bkup_path + "data/data/com.sec.imsservice/shared_prefs/ims_user_data_0.xml";
    QFile imei_no_file(imei_no_path);
    if(!imei_no_file.exists())
    {
        recon_static_functions::app_debug("graykey android backup info file not exist ---FAILED--- " + imei_no_path, Q_FUNC_INFO);
        return;
    }

    if(!imei_no_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("graykey android backup info file not open ---FAILED--- " + imei_no_path, Q_FUNC_INFO);
        return;
    }

    while(!imei_no_file.atEnd())
    {
        QString line_str = imei_no_file.readLine().trimmed();
        if(line_str.contains("imei:"))
        {
            QStringList imei_no_list = line_str.split("imei:");

            if(imei_no_list.size() > 1)
            {
                QString imei_no_splitted_list = imei_no_list.at(1);
                QStringList imei_no_splitted_list_2 = imei_no_splitted_list.split("&");
                if(imei_no_splitted_list_2.size() > 0)
                {
                    mobj.imei_number_QStringList << imei_no_splitted_list_2.at(0);
                }
            }
        }
    }
    mobj.imei_number_QStringList.removeDuplicates();
    imei_no_file.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);

}

void MainWindow::extract_serial_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString serial_no_path =  android_bkup_path + "efs/sec_efs/SVC";

    QFile serial_no_file(serial_no_path);

    if(!serial_no_file.exists())
    {
        recon_static_functions::app_debug("graykey android info filepath not exist ---FAILED--- " + serial_no_path , Q_FUNC_INFO);
        return;
    }

    if(!serial_no_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("graykey android info filepath not open ---FAILED--- " + serial_no_path , Q_FUNC_INFO);
        return;
    }

    bool bool_serial_no_found = false;

    while(!serial_no_file.atEnd())
    {
        if(bool_serial_no_found)
        {
            break;
        }

        QString line_str = serial_no_file.readLine().trimmed();
        if(line_str.contains("serialNumber"))
        {
            QStringList line_str_list = line_str.split(",",Qt::SkipEmptyParts);
            for(int jj = 0; jj < line_str_list.size(); jj++)
            {
                QString info = line_str_list.at(jj).trimmed();
                if(info.contains("serialNumber"))
                {
                    QStringList info_list = info.split(":");
                    if(info_list.size() > 1)
                    {
                        QString sr_no = info_list.at(1).trimmed();
                        mobj.serial_number = sr_no.remove("\"");
                        bool_serial_no_found = true;
                        break;
                    }
                }
            }
        }
    }
    serial_no_file.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::extract_iccid_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString iccid_no_path = android_bkup_path + "data/data/com.google.android.apps.messaging/shared_prefs/sim_state_tracker.xml";

    QFile iccid_no_file(iccid_no_path);
    if(!iccid_no_file.exists())
    {
        recon_static_functions::app_debug("graykey android backup iccid info file not exist ---FAILED--- " + iccid_no_path, Q_FUNC_INFO);
        return;
    }

    if(!iccid_no_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("graykey android backup iccid info file not open ---FAILED--- " + iccid_no_path, Q_FUNC_INFO);
        return;
    }

    while(!iccid_no_file.atEnd())
    {
        QString line_str = iccid_no_file.readLine().trimmed();
        if(line_str.contains("sim_id"))
        {
            QStringList iccid_no_list = line_str.split("\"sim_id\">");
            if(iccid_no_list.size() > 1)
            {
                QString iccid_no_splitted_list = iccid_no_list.at(1);
                mobj.iccid_number_QStringList << iccid_no_splitted_list.remove("</string>").trimmed();
            }
        }
    }
    mobj.iccid_number_QStringList.removeDuplicates();
    iccid_no_file.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);

}

void MainWindow::extract_configured_id_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString dir_iter_info_path = android_bkup_path + "data/system_de/";

    QDirIterator iter(dir_iter_info_path,QDir::AllDirs | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);
    while(iter.hasNext())
    {
        dir_iter_info_path = iter.next();

        QString sim_info_db_path = dir_iter_info_path + "/accounts_de.db";

        if(!QFileInfo(sim_info_db_path).exists())
        {
            continue;
        }

        QString cmd_select_cnfg_id = "select name from accounts where type = 'com.google'";
        mobj.configured_ids_QStringList << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_select_cnfg_id, 0, sim_info_db_path, Q_FUNC_INFO);

    }
    mobj.configured_ids_QStringList.removeDuplicates();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);

}

void MainWindow::extract_bluetooth_address_device_id_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString dir_iter_info_path = android_bkup_path + "data/system/users/";

    QDirIterator iter(dir_iter_info_path,QDir::AllDirs | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);

    while(iter.hasNext())
    {
        dir_iter_info_path = iter.next();

        QString bluetooth_address_device_id_path = dir_iter_info_path + "/settings_secure.xml";

        QFile device_info_file(bluetooth_address_device_id_path);
        if(!device_info_file.exists())
        {
            continue;
        }

        if(!device_info_file.open(QIODevice::ReadOnly))
        {
            continue;
        }

        while(!device_info_file.atEnd())
        {
            QString line_str = device_info_file.readLine().trimmed();

            if(line_str.contains("bluetooth_address"))
            {
                QStringList bluetooth_address_no_list = line_str.split("bluetooth_address\" value=\"");
                if(bluetooth_address_no_list.size() > 1)
                {
                    QString bluetooth_address_splitted_list = bluetooth_address_no_list.at(1);
                    QStringList bluetooth_address = bluetooth_address_splitted_list.split("\" package");
                    if(bluetooth_address.size() > 0 && mobj.bluetooth_address.isEmpty())
                    {
                        mobj.bluetooth_address = bluetooth_address.at(0);
                    }
                }
            }

            if(line_str.contains("android_id") && bluetooth_address_device_id_path.contains("/users/0/settings_secure.xml"))
            {
                QStringList device_id_list = line_str.split("android_id\" value=\"");
                if(device_id_list.size() > 1)
                {
                    QString device_id_splitted_list = device_id_list.at(1);
                    QStringList device_id = device_id_splitted_list.split("\" package");
                    mobj.device_id = device_id.at(0);
                }
            }
        }

        device_info_file.close();
    }
    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::extract_timezone_for_graykey_android_backup(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString android_bkup_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
    QString timezone_no_path1 = android_bkup_path + "data/property/persistent_properties";

    QFile timezone_no_file1(timezone_no_path1);

    if(timezone_no_file1.open(QIODevice::ReadOnly))
    {

        while(!timezone_no_file1.atEnd())
        {
            QString line_str = timezone_no_file1.readLine().trimmed();
            if(line_str.contains("persist.sys.timezone"))
            {
                QString timezone = line_str.remove("persist.sys.timezone");
                mobj.time_zone = timezone.trimmed();
                break;
            }
        }
        timezone_no_file1.close();
    }

    if(!mobj.time_zone.isEmpty())
    {
        return;
    }

    QString timezone_no_path2 = android_bkup_path + "/data/data/com.google.android.gms/shared_prefs/reminders_preferences.xml";

    QFile timezone_no_file2(timezone_no_path2);
    if(!timezone_no_file2.exists())
    {
        recon_static_functions::app_debug("graykey android backup timeone info file not exist ---FAILED--- " + timezone_no_path2, Q_FUNC_INFO);
        return;
    }

    if(!timezone_no_file2.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("graykey android backup timezone info file not open ---FAILED--- " + timezone_no_path2, Q_FUNC_INFO);
        return;
    }

    while(!timezone_no_file2.atEnd())
    {
        QString line_str = timezone_no_file2.readLine().trimmed();
        if(line_str.contains("last_timezone_id"))
        {
            QStringList tmp_timezone_list = line_str.split("last_timezone_id\">");
            if(tmp_timezone_list.size() > 1)
            {
                QString time_zone = tmp_timezone_list.at(1);
                time_zone = time_zone.remove("</string>");
                mobj.time_zone = time_zone;
                break;
            }
        }
    }
    timezone_no_file2.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);

}

void MainWindow::clear_android_backup_detail_info_structure(struct_global_android_backup_detail &mobj)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    mobj.product_build_version.clear();
    mobj.product_model.clear();
    mobj.product_name.clear();
    mobj.configured_ids_QStringList.clear();
    mobj.country_code.clear();
    mobj.iccid_number_QStringList.clear();
    mobj.imei_number_QStringList.clear();
    mobj.locale_language.clear();
    mobj.phone_number_mssid_QStringList.clear();
    mobj.time_zone.clear();
    mobj.serial_number.clear();
    mobj.device_id.clear();
    mobj.bluetooth_address.clear();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}
