#include "recon_helper_singular.h"


recon_helper_singular::recon_helper_singular(QWidget *parent)
{

    bool_process_generic = false;
    process_generic = new QProcess(this);
    connect(process_generic,SIGNAL(finished(int)),this,SLOT(slot_generic_process_finished(int)));

}

recon_helper_singular::~recon_helper_singular()
{

}


QString recon_helper_singular::get_adhoc_path_for_database(QString main_source_path)
{

    // Input Path : /Users/maruti/Library/Application Support/MobileSync/Backup/64f1899db1dc45affa697c8f1dd00dec6f5ba229/Manifest.db
    // Return Path : /Users/maruti/aaout put/Result_2018-Jan-03T15-13-11/tmp/m_sqlite/Manifest.db


    if(main_source_path.trimmed().isEmpty())
        return QString("");


    QString shm_source_path = main_source_path + "-shm";
    QString wal_source_path = main_source_path + "-wal";

    QFileInfo m_info(main_source_path);

    QString sql_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Miscellenous_In_Result_QString).toString();
    QString  avail_file = recon_static_functions::get_available_filename(m_info.fileName(), sql_path, Q_FUNC_INFO);


    QString main_dest_path = sql_path + avail_file;
    QString shm_dest_path = sql_path + avail_file + "-shm";
    QString wal_dest_path = sql_path + avail_file + "-wal";

    QFileInfo info_temp_check(main_dest_path);
    if(info_temp_check.exists())
    {
        QFile temp_file(main_dest_path);
        temp_file.remove();
    }
    QFile::copy(main_source_path,main_dest_path);

    QFileInfo shm_file_info(shm_source_path);
    QFileInfo wal_file_info(wal_source_path);

    if(shm_file_info.exists())
    {
        QFile::copy(shm_source_path,shm_dest_path);
    }
    if(wal_file_info.exists())
    {
        QFile::copy(wal_source_path,wal_dest_path);
    }

    return main_dest_path;
}


QString recon_helper_singular::get_file_system_initial_path_by_giving_source_count_name(QString source_count_name, QString caller_func)
{
    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString fs_extraction_path = struct_target_info.fs_extraction_source_path;

    if(!fs_extraction_path.isEmpty())
    {
        if(fs_extraction_path.endsWith("/"))
            fs_extraction_path.remove((fs_extraction_path.size() - 1),1);
    }

    return fs_extraction_path;
}



QString recon_helper_singular::pub_get_value_from_source_textinfo_file_only(QString file_path, QString keyword)
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString value;

    QFile m_file(file_path);
    if(!m_file.exists())
        return value;

    if(!m_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug( " text file open ---FAILED--- " + m_file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + m_file.errorString(), Q_FUNC_INFO);
        return value;
    }

    keyword = keyword.trimmed();

    while(!m_file.atEnd())
    {
        QString line = m_file.readLine();

        if(line.contains("="))
        {
            QStringList tmp_list = line.split("=", Qt::SkipEmptyParts);
            if(tmp_list.size() == 2)
            {
                if(tmp_list.at(0).trimmed() == keyword)
                {
                    value = tmp_list.at(1).trimmed();
                    break;
                }
            }
        }

    }

    m_file.close();


    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

    return value;
}


QString recon_helper_singular::get_plist_file_path_after_running_diskutill_info(QString identifier_name, QString result_dir_path , QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    if(identifier_name.isEmpty())
        return "";

    QString temp_plist_path = global_variable_private_var_tmp_Dir_Path_Case_QString;
    QString avail_file = recon_static_functions::get_available_filename("temp_diskutil_info.plist",temp_plist_path, Q_FUNC_INFO);
    temp_plist_path = temp_plist_path +  avail_file;

    QStringList arg_list;
    arg_list << "info" << "-plist" << identifier_name;

    QString output_value = run_command_generic(QString("diskutil") , arg_list , Q_FUNC_INFO);

    recon_static_functions::overwrite_data_into_file(temp_plist_path,output_value,(QString(Q_FUNC_INFO) + caller_func));

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return temp_plist_path;
}

QString recon_helper_singular::get_plist_file_path_after_running_diskutil_apfs_list(QString parent_identifier, QString caller_func)
{
    //   Here we run command
    //   diskutil apfs list -plist disk2 > ~/tmp/123.plist
    //   then return the file path

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    if(parent_identifier.isEmpty())
        return "";

    QString temp_plist_path = global_variable_private_var_tmp_Dir_Path_Case_QString;
    QString avail_file = recon_static_functions::get_available_filename("temp_diskutil_info.plist",temp_plist_path, Q_FUNC_INFO);
    temp_plist_path = temp_plist_path +  avail_file;

    QStringList arg_list;
    arg_list << MACRO_FILE_SYSTEM_TYPE_APFS << "list" << "-plist" << parent_identifier;


    QString output_value = run_command_generic(QString("diskutil") , arg_list , Q_FUNC_INFO);

    recon_static_functions::overwrite_data_into_file(temp_plist_path,output_value,(QString(Q_FUNC_INFO) + caller_func));

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return temp_plist_path;

}

void recon_helper_singular::slot_generic_process_finished(int)
{
    bool_process_generic = true;
}

void recon_helper_singular::slot_export_process_finished(int)
{
    bool_export_process = true;
}


QStringList recon_helper_singular::get_mounted_media_list(QString caller_func)
{
    QStringList mount_media_list =  get_mounted_media_list_from_volume_directory(caller_func);

    mount_media_list << get_mounted_media_from_mount_command(caller_func);

    mount_media_list.removeDuplicates();
    mount_media_list.sort(Qt::CaseInsensitive);


    return mount_media_list;
}

QStringList recon_helper_singular::get_mounted_media_list_from_volume_directory(QString caller_func)
{
    QStringList list;

    QDirIterator iter("/Volumes/");
    while(iter.hasNext())
    {
        QString temp = iter.next();
        QFileInfo info(temp);

        if(!info.isDir())
            continue;

        QDir mdir(temp);

        //Returns the total number of directories and files in the directory.
        if(mdir.count() < 3)
            continue;


        if(info.isSymLink() || info.isHidden() || info.isRoot())
            continue;

        if(temp.endsWith("."))
            continue;

        list << temp.trimmed();
    }

    return list;
}

QStringList recon_helper_singular::get_mounted_media_from_mount_command(QString caller_func)
{
    QStringList list_mounted_media;

    QString data = run_command_without_argument(QString("mount") , caller_func);


    QStringList all_media_list = data.split("\n");

    for(int ii = 0; ii < all_media_list.size(); ii++)
    {
        QString line_text = all_media_list.at(ii).trimmed();



        if(line_text.contains(" (" + QString(MACRO_FILE_SYSTEM_TYPE_NTFS) + ",", Qt::CaseInsensitive)
                || line_text.contains(" (" + QString(MACRO_FILE_SYSTEM_TYPE_HFS) + ",", Qt::CaseInsensitive)
                || line_text.contains(" (" + QString(MACRO_FILE_SYSTEM_TYPE_APFS) + ",", Qt::CaseInsensitive)
                || line_text.contains(" (" + QString(MACRO_FILE_SYSTEM_TYPE_MSDOS) + ",", Qt::CaseInsensitive)
                || line_text.contains(" (" + QString(MACRO_FILE_SYSTEM_TYPE_LIFS) + ",", Qt::CaseInsensitive)
                || line_text.contains(" (" + QString(MACRO_FILE_SYSTEM_TYPE_EXFAT) + ",", Qt::CaseInsensitive))
            // our all custom mount path should include || line_text.contains("ewfmount@osxfuse"))
        {
            if((line_text.contains(" on /") && line_text.contains(" (")) && (!line_text.contains("/private/var/")))
            {
                QStringList list_temp = line_text.split(" on /");

                if(list_temp.size() > 1)
                {
                    QStringList list_name = list_temp.at(1).split(" (");

                    if(list_name.size() >= 1)
                    {
                        list_mounted_media <<  "/" + list_name.at(0).trimmed();
                    }

                }

            }

        }

    }

    return list_mounted_media;
}

QString recon_helper_singular::run_command_generic(QString command, QStringList arguments , QString caller_func)
{
    bool_process_generic = false;

    process_generic->start(command,arguments);

    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic)
            break;
    }
    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + "  : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}

QString recon_helper_singular::get_disk_node_of_mounted_path(QString mounted_path, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    // donot trim received mounted path
    // /dev/disk5s1 on /Volumes/Untitled      (apfs, local, nodev, nosuid, journaled, noowners)

    // Output : /dev/disk5s1


    if(mounted_path.endsWith("/"))
    {
        if(mounted_path != "/")
            mounted_path.chop(QString("/").size());
    }

    QString node_value;

    QProcess proc_mount;
    proc_mount.start("mount");
    proc_mount.waitForFinished(3000);


    QStringList all_media_list = QString::fromLocal8Bit(proc_mount.readAll()).split("\n");

    for(int ii = 0; ii < all_media_list.size(); ii++)
    {
        QString line_val_str = all_media_list.at(ii);

        if(line_val_str.startsWith("ntfs:/"))
        {
            line_val_str.remove(0, QString("ntfs:/").size());
            line_val_str.prepend("/dev");
        }

        int m_index = line_val_str.lastIndexOf(" (");
        if(m_index == -1)
            continue;

        line_val_str = line_val_str.remove(m_index, line_val_str.size());

        QStringList tmp_list = line_val_str.split(" on /");
        if(tmp_list.size() < 2)
            continue;

        QString disk_node_name  = tmp_list.at(0).trimmed();
        QString disk_mount_path = tmp_list.at(1); // donot trim
        disk_mount_path.prepend("/");

        if(disk_mount_path == mounted_path)
        {
            node_value = disk_node_name;
            break;
        }

    }

    if(node_value.endsWith("/"))
        node_value.chop(QString("/").size());

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  end " , Q_FUNC_INFO);

    return node_value;
}

bool recon_helper_singular::is_it_mounted_path_by_contains_match(QString mounted_path, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    QProcess proc_mount;
    proc_mount.start("mount");
    proc_mount.waitForFinished(3000);

    QStringList all_media_list = QString::fromLocal8Bit(proc_mount.readAll()).split("\n");

    bool src_exist = false;
    for(int ii = 0; ii < all_media_list.size(); ii++)
    {
        QString line_str = all_media_list.at(ii);
        if(line_str.contains(mounted_path))
        {
            src_exist = true;
            break;
        }
    }

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -end " , Q_FUNC_INFO);

    return src_exist;
}

QStringList recon_helper_singular::get_softlink_path(QString dir_path, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -start " , Q_FUNC_INFO);


    //find . -maxdepth 5 -type l
    QStringList argument;
    QString tmp_path = dir_path;
    tmp_path.chop(1);

    //This maxdepth count is according to the mount path directory structure we created for HomeDirectory.
    argument << tmp_path << "-maxdepth" << "5" << "-type" << "l";

    QString complete_path = run_command_generic("find" , argument, Q_FUNC_INFO);
    QStringList soft_path_list;
    soft_path_list = complete_path.split("\n");

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -end " , Q_FUNC_INFO);

    return  soft_path_list;

}

QString recon_helper_singular::get_file_system_of_mounted_path(QString mounted_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    // donot trim received mounted path
    // /dev/disk5s1 on /Volumes/Untitled      (apfs, local, nodev, nosuid, journaled, noowners)

    // output : apfs


    QString m_node = get_disk_node_of_mounted_path(mounted_path, caller_func);

    QString fs_value = get_file_system_of_any_node(m_node, caller_func).trimmed();

    if(fs_value.isEmpty())
    {
        if(mounted_path.endsWith("/"))
        {
            if(mounted_path != "/")
                mounted_path.chop(QString("/").size());
        }


        QProcess proc_mount;
        proc_mount.start("mount");
        proc_mount.waitForFinished(3000);

        QString str_1 = "on " + mounted_path + " (";


        QStringList all_media_list = QString::fromLocal8Bit(proc_mount.readAll()).split("\n");

        for(int i = 0; i < all_media_list.size(); i++)
        {
            QString each_line_str = all_media_list.at(i);

            if(!each_line_str.contains(str_1))
                continue;

            QStringList tmp_list = each_line_str.split(str_1);
            if(tmp_list.size() < 2)
                continue;


            QString fs_line = tmp_list.at(1);
            fs_line.remove(fs_line.indexOf(","), fs_line.size());

            fs_value = fs_line.trimmed();

            break;
        }
    }

    if(fs_value == (MACRO_FILE_SYSTEM_TYPE_LIFS))
        fs_value = MACRO_FILE_SYSTEM_TYPE_NTFS;

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  end " , Q_FUNC_INFO);

    return fs_value;
}

bool recon_helper_singular::is_it_mounted_path_by_exact_match(QString mounted_path, QString caller_func)
{
    QString mnode = get_disk_node_of_mounted_path(mounted_path,(recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO)).trimmed();

    if(mnode.isEmpty())
        return false;
    else
        return true;

}


QStringList recon_helper_singular::get_mounted_volumes_path_with_node_list(QString caller_func)
{

    QStringList path_with_node_list;

    QString mount_output = run_command_without_argument(QString("mount") , caller_func);

    QStringList all_media_list = mount_output.split("\n");

    for(int i = 0; i < all_media_list.size(); i++)
    {
        QString line_val_str = all_media_list.at(i);
        if(line_val_str.startsWith("/dev/"))
        {
            int m_index = line_val_str.lastIndexOf(" (");
            if(m_index == -1)
                continue;

            line_val_str = line_val_str.remove(m_index, line_val_str.size());

            QStringList tmp_list = line_val_str.split(" on /");
            if(tmp_list.size() < 2)
                continue;

            QString disk_node_name  = tmp_list.at(0).trimmed();
            QString disk_mount_path = tmp_list.at(1); // donot trim
            disk_mount_path.prepend("/");

            if(disk_mount_path == "/")
                continue;

            QString val_str = disk_node_name + MACRO_RECON_Splitter_1_multiple + disk_mount_path;

            path_with_node_list << val_str;
        }
    }

    return path_with_node_list;
}

QString recon_helper_singular::run_command_without_argument(QString command , QString caller_func)
{
    bool_process_generic = false;


    process_generic->start(command);

    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic)
            break;
    }
    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}


void recon_helper_singular::write_xml_report_case_info_and_source_info(QXmlStreamWriter *stream_xml, QString report_scope, narad_muni *narad_muni_for_reports)
{
    struct_case_details_values case_details_value_obj = fill_case_detials_values_structure(narad_muni_for_reports);
    struct_case_details_header_lables case_details_lables_obj = fill_case_detials_lables_structure();

    stream_xml->writeStartElement(case_details_lables_obj.Case_Info_header_lable.replace(" ","_"));
    stream_xml->writeStartElement(case_details_lables_obj.Case_Details_header_lable.replace(" ","_"));
    stream_xml->writeTextElement(case_details_lables_obj.app_version_lable.replace(" ","_"),case_details_value_obj.result_created_app_version);
    stream_xml->writeTextElement(case_details_lables_obj.report_scope_lable.replace(" ","_"), report_scope);
    stream_xml->writeTextElement(case_details_lables_obj.case_no_lable.replace(" ","_"), case_details_value_obj.case_no);
    stream_xml->writeTextElement(case_details_lables_obj.case_name_lable.replace(" ","_"), case_details_value_obj.case_name);
    stream_xml->writeTextElement(case_details_lables_obj.location_lable.replace(" ","_"), case_details_value_obj.location);
    stream_xml->writeTextElement(case_details_lables_obj.notes_lable.replace(" ","_"), case_details_value_obj.notes);
    stream_xml->writeEndElement();   // end case Details


    stream_xml->writeStartElement(case_details_lables_obj.Examiner_Details_header_lable.replace(" ","_"));
    stream_xml->writeTextElement(case_details_lables_obj.examiner_lable.replace(" ","_"), case_details_value_obj.examiner);
    stream_xml->writeTextElement(case_details_lables_obj.examiner_phone_lable.replace(" ","_"), case_details_value_obj.examiner_phone);
    stream_xml->writeTextElement(case_details_lables_obj.examiner_email_lable.replace(" ","_"), case_details_value_obj.examiner_email);
    stream_xml->writeTextElement(case_details_lables_obj.agency_lable.replace(" ","_"), case_details_value_obj.agency);
    stream_xml->writeTextElement(case_details_lables_obj.agency_address_lable.replace(" ","_"), case_details_value_obj.agency_address);
    stream_xml->writeTextElement(case_details_lables_obj.user_timezone_string_lable.replace(" ","_"), case_details_value_obj.user_timezone_string);
    stream_xml->writeTextElement(case_details_lables_obj.report_generated_date_time_lable.replace(" ","_"), case_details_value_obj.report_generated_date_time);
    stream_xml->writeTextElement(case_details_lables_obj.machine_timezone_lable.replace(" ","_"), case_details_value_obj.machine_timezone_string);
    stream_xml->writeEndElement();   // end Examiner Details
    stream_xml->writeEndElement();   // end Case Info

    stream_xml->writeStartElement(case_details_lables_obj.Sources_Details_header_lable.replace(" ","_"));

    /// Sources Details
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int i = 0; i < list_target_source_info.size(); i++)
    {

        QString source_count_name           = list_target_source_info.at(i).source_count_name;
        QString complete_source_name        = list_target_source_info.at(i).complete_source_name;

        QString evidence_no                 = list_target_source_info.at(i).evidence_number;
        QString description                 = list_target_source_info.at(i).description;


        QString os_schme_dsply                = list_target_source_info.at(i).os_scheme_display;
        QString os_schme_intrnl                = list_target_source_info.at(i).os_scheme_internal;
        QString os_version_extended         = list_target_source_info.at(i).os_version_extended;
        QString latitude                    = list_target_source_info.at(i).latitude;
        QString longitude                   = list_target_source_info.at(i).longitude;
        QString country                     = list_target_source_info.at(i).country;
        QString city                        = list_target_source_info.at(i).city;
        QString configured_id_str           = list_target_source_info.at(i).configured_ids_QStringList.join(MACRO_RECON_Splitter_6_comma);

        QString src_type_intrnl                 = list_target_source_info.at(i).source_type_internal;
        QString root_type                      = list_target_source_info.at(i).root_type;
        QStringList system_user_list        = list_target_source_info.at(i).system_user_QStringList;
        QString build_version               = list_target_source_info.at(i).build_version;

        QString device_name             = list_target_source_info.at(i).device_name;
        QString device_display_name     = list_target_source_info.at(i).display_name;
        QString device_id               = list_target_source_info.at(i).device_id;
        QString imei_no                 = list_target_source_info.at(i).imei_no_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString phone_no                = list_target_source_info.at(i).phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString serial_no               = list_target_source_info.at(i).serial_no;
        QString iccid_no                = list_target_source_info.at(i).sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma);

        QString product_name                = list_target_source_info.at(i).product_name;
        QString product_model                       = list_target_source_info.at(i).product_model;
        QString product_type                = list_target_source_info.at(i).product_type;
        QString target_type                 = list_target_source_info.at(i).target_type;

        QString itune_version           = list_target_source_info.at(i).itune_version;
        QString latest_backup_date_QString = list_target_source_info.at(i).latest_backup_date_readable_QString;

        QString target_system_timezone_QString = list_target_source_info.at(i).timezone_readable_QString;
        QString file_system_type_str        = list_target_source_info.at(i).file_system_type;
        QString os_installation_timestamps = convert_numeric_to_readable_in_utc_timezone(QString::number(list_target_source_info.at(i).os_installer_timestamp_utc_numeric_qint64) ,Q_FUNC_INFO);

        QString bluetooth_device_address          = list_target_source_info.at(i).bluetooth_device_address;
        QString wifi_address                      = list_target_source_info.at(i).wifi_address;
        QString locale_language                   = list_target_source_info.at(i).locale_language;

        stream_xml->writeStartElement(source_count_name);

        if(!complete_source_name.isEmpty())
            stream_xml->writeTextElement(QString("Source Name").replace(" ","_"), complete_source_name);

        if(!evidence_no.isEmpty())
            stream_xml->writeTextElement(QString("Evidence No").replace(" ","_"), evidence_no);

        if(!root_type.isEmpty())
            stream_xml->writeTextElement(QString("Source Type").replace(" ","_"), root_type);

        if(!os_schme_dsply.isEmpty())
        {
            if((os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    stream_xml->writeTextElement(QString("OS Type").replace(" ","_"), os_schme_dsply);
                }
            }
        }

        if(!file_system_type_str.isEmpty())
        {
            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal)
            {
                stream_xml->writeTextElement(QString("File System").replace(" ","_"), file_system_type_str);
            }
        }

        if(!system_user_list.isEmpty())
        {
            if((os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    stream_xml->writeTextElement(QString("Users").replace(" ","_"), system_user_list.join(", "));
                }
            }
        }

        if(!configured_id_str.isEmpty())
            stream_xml->writeTextElement(QString("Configured ID").replace(" ","_"), configured_id_str);

        if(!device_display_name.isEmpty())
            stream_xml->writeTextElement(QString("Display Name").replace(" ","_"), device_display_name);

        if(!device_name.isEmpty())
            stream_xml->writeTextElement(QString("Device Name").replace(" ","_"), device_name);

        if(!device_id.isEmpty())
            stream_xml->writeTextElement(QString("Device ID").replace(" ","_"), device_id);

        if(!imei_no.isEmpty())
            stream_xml->writeTextElement(QString("Device IMEI").replace(" ","_"), imei_no);

        if(!serial_no.isEmpty())
            stream_xml->writeTextElement(QString("Device Serial No").replace(" ","_"), serial_no);

        if(!iccid_no.isEmpty())
            stream_xml->writeTextElement(QString("Device ICCID").replace(" ","_"), iccid_no);

        if(!os_version_extended.isEmpty())
            stream_xml->writeTextElement(QString("OS Version").replace(" ","_"), os_version_extended);

        if(!phone_no.isEmpty())
            stream_xml->writeTextElement(QString("Phone No.").replace(" ","_"), phone_no);

        if(!product_name.isEmpty())
            stream_xml->writeTextElement(QString("Product Name").replace(" ","_"), product_name);

        if(!product_model.isEmpty())
            stream_xml->writeTextElement(QString("Product Model").replace(" ","_"), product_model);

        if(!product_type.isEmpty())
            stream_xml->writeTextElement(QString("Product Type").replace(" ","_"), product_type);

        if(!target_type.isEmpty())
            stream_xml->writeTextElement(QString("Target Type").replace(" ","_"), target_type);

        if(!itune_version.isEmpty())
            stream_xml->writeTextElement(QString("iTune Version").replace(" ","_"), itune_version);

        if(!latest_backup_date_QString.isEmpty())
            stream_xml->writeTextElement(QString("Backup Date").replace(" ","_"), latest_backup_date_QString);

        if(!build_version.isEmpty())
            stream_xml->writeTextElement(QString("Build Version").replace(" ","_"), build_version);

        if(!country.isEmpty())
            stream_xml->writeTextElement(QString("Country").replace(" ","_"), country);

        if(!city.isEmpty())
            stream_xml->writeTextElement(QString("City").replace(" ","_"), city);

        if(!latitude.isEmpty())
            stream_xml->writeTextElement(QString("Latitude").replace(" ","_"), latitude);

        if(!longitude.isEmpty())
            stream_xml->writeTextElement(QString("Longitude").replace(" ","_"), longitude);

        if(!bluetooth_device_address.isEmpty())
            stream_xml->writeTextElement(QString("Bluetooth Device Address").replace(" ","_"), bluetooth_device_address);

        if(!wifi_address.isEmpty())
            stream_xml->writeTextElement(QString("Wifi Address").replace(" ","_"), wifi_address);

        if(!locale_language.isEmpty())
            stream_xml->writeTextElement(QString("Locale Language").replace(" ","_"), locale_language);

        if(!target_system_timezone_QString.isEmpty())
            stream_xml->writeTextElement(QString("System Time Zone").replace(" ","_"), target_system_timezone_QString);

        if(!os_installation_timestamps.isEmpty())
        {
            stream_xml->writeTextElement(QString("Installer Date").replace(" ","_"), os_installation_timestamps);
        }

        if(!description.isEmpty())
            stream_xml->writeTextElement(QString("Description").replace(" ","_"), description);


        stream_xml->writeEndElement();   // end Source count name
    }


    stream_xml->writeEndElement(); // end source details
}

QString recon_helper_singular::write_html_case_info_and_source_info(QString report_scope, narad_muni *narad_muni_for_reports)
{

    ///----------Case Details
    struct_case_details_values case_details_value_obj = fill_case_detials_values_structure(narad_muni_for_reports);
    struct_case_details_header_lables case_details_lables_obj = fill_case_detials_lables_structure();

    QString temp = "</div>  </div> <div class=\"stack\"><h2 class=\"caption\" id=\"\"><span class=\" s14\" id=\"\">Case Details </span>  </h2>  </div>";

    temp +=  "<table width = \"100%\" border=\"1\" align=\"center\" cellpadding=\"1\" cellspacing=\"0\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">" ;
    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.app_version_lable + "</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.result_created_app_version + "</td>  </tr>";

    if(!report_scope.trimmed().isEmpty())
    {
        report_scope.replace("_"," ");
        temp += " <tr>  <td class = \"wrapped\"> <strong> "+ case_details_lables_obj.report_scope_lable + "</strong> </td>  <td class = \"wrapped\">";
        temp += report_scope + "</td>  </tr>";
    }

    temp += " <tr>  <td class = \"wrapped\"> <strong>" + case_details_lables_obj.case_no_lable + "</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.case_no + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.case_name_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.case_name + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.location_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.location + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.notes_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.notes + "</td>  </tr>";

    temp += "<tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.examiner_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.examiner+ "</td>  </tr> ";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.examiner_phone_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.examiner_phone+ "</td>  </tr> ";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.examiner_email_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.examiner_email+ "</td>  </tr> ";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.agency_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.agency + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.agency_address_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.agency_address + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.user_timezone_string_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.user_timezone_string + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.report_generated_date_time_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.report_generated_date_time + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.machine_timezone_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.machine_timezone_string + "</td>  </tr> </table>";

    /// Sources Details

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    temp += "<div class=\"stack\"><h2 class=\"caption\" id=\"\"><span class=\" s14\" id=\"\">"+ case_details_lables_obj.Sources_Details_header_lable + " </span>  </h2>  </div>";

    for(int i = 0; i < list_target_source_info.size(); i++)
    {

        QString source_count_name           = list_target_source_info.at(i).source_count_name;
        QString complete_source_name                 = list_target_source_info.at(i).complete_source_name;

        QString evidence_no           = list_target_source_info.at(i).evidence_number;
        QString description                 = list_target_source_info.at(i).description;


        QString os_schme_dsply            = list_target_source_info.at(i).os_scheme_display;
        QString os_schme_intrnl            = list_target_source_info.at(i).os_scheme_internal;

        QString os_version_extended         = list_target_source_info.at(i).os_version_extended;
        QString latitude                    = list_target_source_info.at(i).latitude;
        QString longitude                   = list_target_source_info.at(i).longitude;
        QString country                     = list_target_source_info.at(i).country;
        QString city                        = list_target_source_info.at(i).city;

        QString src_type_intrnl                 = list_target_source_info.at(i).source_type_internal;
        QString root_type            = list_target_source_info.at(i).root_type;
        QString configured_id_str           = list_target_source_info.at(i).configured_ids_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QStringList system_user_list        = list_target_source_info.at(i).system_user_QStringList;
        QString build_version               = list_target_source_info.at(i).build_version;

        QString device_name             = list_target_source_info.at(i).device_name;
        QString device_display_name     = list_target_source_info.at(i).display_name;
        QString device_id               = list_target_source_info.at(i).device_id;
        QString imei_no                 = list_target_source_info.at(i).imei_no_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString phone_no                = list_target_source_info.at(i).phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString serial_no               = list_target_source_info.at(i).serial_no;
        QString iccid_no                = list_target_source_info.at(i).sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma);

        QString product_name                = list_target_source_info.at(i).product_name;
        QString product_model                     = list_target_source_info.at(i).product_model;
        QString product_type                = list_target_source_info.at(i).product_type;
        QString bluetooth_device_address          = list_target_source_info.at(i).bluetooth_device_address;
        QString wifi_address                      = list_target_source_info.at(i).wifi_address;
        QString locale_language                   = list_target_source_info.at(i).locale_language;

        QString target_type                 = list_target_source_info.at(i).target_type;
        QString itune_version           = list_target_source_info.at(i).itune_version;
        QString latest_backup_date_QString = list_target_source_info.at(i).latest_backup_date_readable_QString;
        QString target_system_timezone_QString = list_target_source_info.at(i).timezone_readable_QString;
        QString file_system_type_str        = list_target_source_info.at(i).file_system_type;
        QString os_installation_timestamps = convert_numeric_to_readable_in_utc_timezone(QString::number(list_target_source_info.at(i).os_installer_timestamp_utc_numeric_qint64) ,Q_FUNC_INFO);

        temp +=  "<table width = \"100%\" border=\"1\" align=\"center\" cellpadding=\"1\" cellspacing=\"0\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">" ;

        QStringList headers_list;
        headers_list << source_count_name << "";

        QStringList size_list;
        size_list << "75" << "75";

        for(int i = 0; i < headers_list.size(); i++)
        {
            temp += "<th width=\"" + size_list.at(i) + "%\" >" + headers_list.at(i) + "</th>";
        }


        if(!complete_source_name.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Source Name</strong> </td>  <td class = \"wrapped\">";
            temp += complete_source_name + "</td>  </tr>";
        }

        if(!evidence_no.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Evidence No</strong> </td>  <td class = \"wrapped\">";
            temp += evidence_no + "</td>  </tr>";
        }
        if(!root_type.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Source Type</strong> </td>  <td class = \"wrapped\">";
            temp += root_type + "</td>  </tr>";
        }

        if(!os_schme_dsply.isEmpty())
        {
            if((os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    temp += " <tr>  <td class = \"wrapped\"> <strong>OS Type</strong> </td>  <td class = \"wrapped\">";
                    temp += os_schme_dsply + "</td>  </tr>";
                }
            }
        }

        if(!file_system_type_str.isEmpty())
        {
            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal)
            {
                temp += " <tr>  <td class = \"wrapped\"> <strong>File System</strong> </td>  <td class = \"wrapped\">";
                temp += file_system_type_str + "</td>  </tr>";
            }
        }

        if(!system_user_list.isEmpty())
        {
            if((os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    temp += " <tr>  <td class = \"wrapped\"> <strong>User(s)</strong> </td>  <td class = \"wrapped\">";
                    temp += system_user_list.join(", ") + "</td>  </tr>";
                }
            }
        }

        if(!configured_id_str.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Configured ID</strong> </td>  <td class = \"wrapped\">";
            temp += configured_id_str + "</td>  </tr>";
        }

        if(!device_display_name.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Display Name</strong> </td>  <td class = \"wrapped\">";
            temp += device_display_name + "</td>  </tr>";
        }

        if(!device_name.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Device Name</strong> </td>  <td class = \"wrapped\">";
            temp += device_name + "</td>  </tr>";
        }

        if(!device_id.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Device ID</strong> </td>  <td class = \"wrapped\">";
            temp += device_id + "</td>  </tr>";
        }

        if(!imei_no.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Device IMEI</strong> </td>  <td class = \"wrapped\">";
            temp += imei_no + "</td>  </tr>";
        }

        if(!serial_no.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Device Serial No</strong> </td>  <td class = \"wrapped\">";
            temp += serial_no + "</td>  </tr>";
        }

        if(!iccid_no.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Device ICCID</strong> </td>  <td class = \"wrapped\">";
            temp += iccid_no + "</td>  </tr>";
        }

        if(!os_version_extended.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>OS Version</strong> </td>  <td class = \"wrapped\">";
            temp += os_version_extended + "</td>  </tr>";
        }

        if(!phone_no.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Phone No.</strong> </td>  <td class = \"wrapped\">";
            temp += phone_no + "</td>  </tr>";
        }

        if(!product_name.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Product Name</strong> </td>  <td class = \"wrapped\">";
            temp += product_name + "</td>  </tr>";
        }

        if(!product_model.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Product Model</strong> </td>  <td class = \"wrapped\">";
            temp += product_model + "</td>  </tr>";
        }

        if(!product_type.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Product Type</strong> </td>  <td class = \"wrapped\">";
            temp += product_type + "</td>  </tr>";
        }

        if(!target_type.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Target Type</strong> </td>  <td class = \"wrapped\">";
            temp += target_type + "</td>  </tr>";
        }

        if(!itune_version.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>iTune Version</strong> </td>  <td class = \"wrapped\">";
            temp += itune_version + "</td>  </tr>";
        }

        if(!latest_backup_date_QString.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Backup Date</strong> </td>  <td class = \"wrapped\">";
            temp += latest_backup_date_QString + "</td>  </tr>";
        }

        if(!build_version.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Build Version</strong> </td>  <td class = \"wrapped\">";
            temp += build_version + "</td>  </tr>";
        }

        if(!country.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Country</strong> </td>  <td class = \"wrapped\">";
            temp += country + "</td>  </tr>";
        }

        if(!city.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>City</strong> </td>  <td class = \"wrapped\">";
            temp += city + "</td>  </tr>";
        }

        if(!latitude.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Latitude</strong> </td>  <td class = \"wrapped\">";
            temp += latitude + "</td>  </tr>";
        }

        if(!longitude.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Longitude</strong> </td>  <td class = \"wrapped\">";
            temp += longitude + "</td>  </tr>";
        }

        if(!bluetooth_device_address.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Bluetooth Device Address</strong> </td>  <td class = \"wrapped\">";
            temp += bluetooth_device_address + "</td>  </tr>";
        }

        if(!wifi_address.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Wifi Address</strong> </td>  <td class = \"wrapped\">";
            temp += wifi_address + "</td>  </tr>";
        }

        if(!locale_language.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Locale Language</strong> </td>  <td class = \"wrapped\">";
            temp += locale_language + "</td>  </tr>";
        }

        if(!target_system_timezone_QString.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>System Time Zone</strong> </td>  <td class = \"wrapped\">";
            temp += target_system_timezone_QString + "</td>  </tr>";
        }

        if(!os_installation_timestamps.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Installer Date</strong> </td>  <td class = \"wrapped\">";
            temp += os_installation_timestamps + "</td>  </tr>";
        }

        if(!description.isEmpty())
        {
            temp += " <tr>  <td class = \"wrapped\"> <strong>Description</strong> </td>  <td class = \"wrapped\">";
            temp += description + "</td>  </tr>";
        }


        temp += "</table> \n<br> <br>";

    }

    return temp;
}

QString recon_helper_singular::write_html_only_case_info(QString report_scope, narad_muni *narad_muni_for_reports)
{
    ///----------Case Details
    struct_case_details_values case_details_value_obj = fill_case_detials_values_structure(narad_muni_for_reports);
    struct_case_details_header_lables case_details_lables_obj = fill_case_detials_lables_structure();

    QString temp = "</div>  </div> <div class=\"stack\"><h2 class=\"caption\" id=\"\"><span class=\" s14\" id=\"\">Case Details </span>  </h2>  </div>";

    temp +=  "<table width = \"100%\" border=\"1\" align=\"center\" cellpadding=\"1\" cellspacing=\"0\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">" ;
    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.app_version_lable + "</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.result_created_app_version + "</td>  </tr>";

    if(!report_scope.trimmed().isEmpty())
    {
        report_scope.replace("_"," ");
        temp += " <tr>  <td class = \"wrapped\"> <strong> "+ case_details_lables_obj.report_scope_lable + "</strong> </td>  <td class = \"wrapped\">";
        temp += report_scope + "</td>  </tr>";
    }

    temp += " <tr>  <td class = \"wrapped\"> <strong>" + case_details_lables_obj.case_no_lable + "</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.case_no + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.case_name_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.case_name + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.location_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.location + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.notes_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.notes + "</td>  </tr>";

    temp += "<tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.examiner_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.examiner+ "</td>  </tr> ";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.examiner_phone_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.examiner_phone+ "</td>  </tr> ";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.examiner_email_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.examiner_email+ "</td>  </tr> ";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.agency_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.agency + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.agency_address_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.agency_address + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.user_timezone_string_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.user_timezone_string + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.report_generated_date_time_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.report_generated_date_time + "</td>  </tr>";

    temp += " <tr>  <td class = \"wrapped\"> <strong>"+ case_details_lables_obj.machine_timezone_lable +"</strong> </td>  <td class = \"wrapped\">";
    temp += case_details_value_obj.machine_timezone_string + "</td>  </tr> </table>";

    return temp;
}

void recon_helper_singular::write_csv_report_case_details_and_source_info(QTextStream &outputfile_csv,QString report_scope , narad_muni *narad_muni_for_reports)
{
    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);

    struct_case_details_values case_details_value_obj = fill_case_detials_values_structure(narad_muni_for_reports);
    struct_case_details_header_lables case_details_lables_obj = fill_case_detials_lables_structure();

    outputfile_csv << "\n";
    outputfile_csv << case_details_lables_obj.app_version_lable << "," << QString("\"") + case_details_value_obj.result_created_app_version + QString("\"") << "\n" ;
    outputfile_csv << case_details_lables_obj.report_scope_lable << "," << QString("\"") + report_scope + QString("\"") << "\n" ;
    outputfile_csv << "\n";

    outputfile_csv << case_details_lables_obj.Case_Details_header_lable  <<"\n" ;
    outputfile_csv << case_details_lables_obj.case_no_lable << "," << QString("\"") + case_details_value_obj.case_no + QString("\"") << "\n" ;
    outputfile_csv << case_details_lables_obj.case_name_lable << "," << QString("\"") + case_details_value_obj.case_name + QString("\"") << "\n" ;
    outputfile_csv << case_details_lables_obj.location_lable << "," << QString("\"") + case_details_value_obj.location + QString("\"")  <<"\n" ;
    outputfile_csv << case_details_lables_obj.notes_lable << "," << QString("\"") +case_details_value_obj.notes + QString("\"")  <<"\n" ;

    outputfile_csv << case_details_lables_obj.Examiner_Details_header_lable  <<"\n" ;
    outputfile_csv << case_details_lables_obj.examiner_lable << "," << QString("\"") + case_details_value_obj.examiner + QString("\"") <<"\n" ;
    outputfile_csv << case_details_lables_obj.examiner_phone_lable << "," << QString("\"") + case_details_value_obj.examiner_phone + QString("\"") <<"\n" ;
    outputfile_csv << case_details_lables_obj.examiner_email_lable << "," << QString("\"") + case_details_value_obj.examiner_email + QString("\"") <<"\n" ;
    outputfile_csv << case_details_lables_obj.agency_lable << "," << QString("\"") + case_details_value_obj.agency + QString("\"")  <<"\n" ;
    outputfile_csv << case_details_lables_obj.agency_address_lable << "," << QString("\"") + case_details_value_obj.agency_address + QString("\"")  <<"\n" ;
    outputfile_csv << case_details_lables_obj.user_timezone_string_lable << "," << QString("\"") + case_details_value_obj.user_timezone_string + QString("\"") <<"\n" ;
    outputfile_csv << case_details_lables_obj.report_generated_date_time_lable << "," << QString("\"") + case_details_value_obj.report_generated_date_time + QString("\"") <<"\n" ;
    outputfile_csv << case_details_lables_obj.machine_timezone_lable << "," << QString("\"") + case_details_value_obj.machine_timezone_string + QString("\"") <<"\n" ;
    outputfile_csv << "\n";

    /// Sources Details
    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    outputfile_csv << case_details_lables_obj.Sources_Details_header_lable  <<"\n" ;

    for(int i = 0; i < list_target_source_info.size(); i++)
    {

        QString source_count_name                      = list_target_source_info.at(i).source_count_name;
        QString complete_source_name                 = list_target_source_info.at(i).complete_source_name;

        QString evidence_no           = list_target_source_info.at(i).evidence_number;
        QString description                 = list_target_source_info.at(i).description;

        QString os_schme_dsply            = list_target_source_info.at(i).os_scheme_display;
        QString os_schme_intrnl            = list_target_source_info.at(i).os_scheme_internal;
        QString os_version_extended         = list_target_source_info.at(i).os_version_extended;
        QString latitude                    = list_target_source_info.at(i).latitude;
        QString longitude                   = list_target_source_info.at(i).longitude;
        QString country                     = list_target_source_info.at(i).country;
        QString city                        = list_target_source_info.at(i).city;

        QString src_type_intrnl                 = list_target_source_info.at(i).source_type_internal;
        QString root_type            = list_target_source_info.at(i).root_type;
        QString configured_id_str           = list_target_source_info.at(i).configured_ids_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QStringList system_user_list        = list_target_source_info.at(i).system_user_QStringList;
        QString build_version               = list_target_source_info.at(i).build_version;

        QString device_name             = list_target_source_info.at(i).device_name;
        QString device_display_name     = list_target_source_info.at(i).display_name;
        QString device_id               = list_target_source_info.at(i).device_id;
        QString imei_no                 = list_target_source_info.at(i).imei_no_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString phone_no                = list_target_source_info.at(i).phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma);

        QString serial_no               = list_target_source_info.at(i).serial_no;
        QString iccid_no                = list_target_source_info.at(i).sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma);

        QString product_name                = list_target_source_info.at(i).product_name;
        QString product_model                     = list_target_source_info.at(i).product_model;
        QString product_type                = list_target_source_info.at(i).product_type;
        QString bluetooth_device_address          = list_target_source_info.at(i).bluetooth_device_address;
        QString wifi_address                      = list_target_source_info.at(i).wifi_address;
        QString locale_language                   = list_target_source_info.at(i).locale_language;

        QString target_type                 = list_target_source_info.at(i).target_type;

        QString itune_version           = list_target_source_info.at(i).itune_version;
        QString latest_backup_date_QString = list_target_source_info.at(i).latest_backup_date_readable_QString;

        QString target_system_timezone_QString = list_target_source_info.at(i).timezone_readable_QString;
        QString file_system_type_str        = list_target_source_info.at(i).file_system_type;
        QString os_installation_timestamps = convert_numeric_to_readable_in_utc_timezone(QString::number(list_target_source_info.at(i).os_installer_timestamp_utc_numeric_qint64) ,Q_FUNC_INFO);

        outputfile_csv << QString("\"") + source_count_name + QString("\"")  <<"\n" ;

        if(!complete_source_name.isEmpty())
            outputfile_csv << (QObject::tr("Source Name")) << "," << QString("\"") + complete_source_name + QString("\"")  <<"\n" ;

        if(!evidence_no.isEmpty())
            outputfile_csv << (QObject::tr("Evidence No")) << "," << QString("\"") + evidence_no + QString("\"")  <<"\n" ;

        if(!root_type.isEmpty())
            outputfile_csv << (QObject::tr("Source Type")) << "," << QString("\"") + root_type + QString("\"")  <<"\n" ;

        if(!os_schme_dsply.isEmpty())
        {
            if((os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    outputfile_csv << (QObject::tr("OS Type")) << "," << QString("\"") + os_schme_dsply + QString("\"")  <<"\n" ;
                }
            }
        }

        if(!file_system_type_str.isEmpty())
        {
            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal)
            {
                outputfile_csv << (QObject::tr("File System")) << "," << QString("\"") + file_system_type_str + QString("\"")  <<"\n" ;
            }
        }

        if(!system_user_list.isEmpty())
        {
            if((os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    outputfile_csv << (QObject::tr("User(s)")) << "," << QString("\"") + system_user_list.join(", ") + QString("\"")  <<"\n" ;
                }
            }
        }

        if(!configured_id_str.isEmpty())
            outputfile_csv << (QObject::tr("Configured ID")) << "," << QString("\"") + configured_id_str + QString("\"")  <<"\n" ;

        if(!device_display_name.isEmpty())
            outputfile_csv << (QObject::tr("Display Name")) << "," << QString("\"") + device_display_name + QString("\"")  <<"\n" ;

        if(!device_name.isEmpty())
            outputfile_csv << (QObject::tr("Device Name")) << "," << QString("\"") + device_name + QString("\"")  <<"\n" ;

        if(!device_id.isEmpty())
            outputfile_csv << (QObject::tr("Device ID")) << "," << QString("\"") + device_id + QString("\"")  <<"\n" ;

        if(!imei_no.isEmpty())
            outputfile_csv << (QObject::tr("Device IMEI")) << "," << QString("\"") + imei_no + QString("\"")  <<"\n" ;

        if(!serial_no.isEmpty())
            outputfile_csv << (QObject::tr("Device Serial No")) << "," << QString("\"") + serial_no + QString("\"")  <<"\n" ;

        if(!iccid_no.isEmpty())
            outputfile_csv << (QObject::tr("Device ICCID")) << "," << QString("\"") + iccid_no + QString("\"")  <<"\n" ;

        if(!os_version_extended.isEmpty())
            outputfile_csv << (QObject::tr("OS Version")) << "," << QString("\"") + os_version_extended + QString("\"")  <<"\n" ;

        if(!phone_no.isEmpty())
            outputfile_csv << (QObject::tr("Phone No.")) << "," << QString("\"") + phone_no + QString("\"")  <<"\n" ;

        if(!product_name.isEmpty())
            outputfile_csv << (QObject::tr("Product Name")) << "," << QString("\"") + product_name + QString("\"")  <<"\n" ;

        if(!product_model.isEmpty())
            outputfile_csv << (QObject::tr("Product Model")) << "," << QString("\"") + product_model + QString("\"")  <<"\n" ;

        if(!product_type.isEmpty())
            outputfile_csv << (QObject::tr("Product Type")) << "," << QString("\"") + product_type + QString("\"")  <<"\n" ;

        if(!target_type.isEmpty())
            outputfile_csv << (QObject::tr("Target Type")) << "," << QString("\"") + target_type + QString("\"")  <<"\n" ;

        if(!itune_version.isEmpty())
            outputfile_csv << (QObject::tr("iTune Version")) << "," << QString("\"") + itune_version + QString("\"")  <<"\n" ;

        if(!latest_backup_date_QString.isEmpty())
            outputfile_csv << (QObject::tr("Backup Date")) << "," << QString("\"") + latest_backup_date_QString + QString("\"")  <<"\n" ;

        if(!build_version.isEmpty())
            outputfile_csv << (QObject::tr("Build Version")) << "," << QString("\"") + build_version + QString("\"")  <<"\n" ;

        if(!country.isEmpty())
            outputfile_csv << (QObject::tr("Country")) << "," << QString("\"") + country + QString("\"")  <<"\n" ;

        if(!city.isEmpty())
            outputfile_csv << (QObject::tr("City")) << "," << QString("\"") + city + QString("\"")  <<"\n" ;

        if(!latitude.isEmpty())
            outputfile_csv << (QObject::tr("Latitude")) << "," << QString("\"") + latitude + QString("\"")  <<"\n" ;

        if(!longitude.isEmpty())
            outputfile_csv << (QObject::tr("Longitude")) << "," << QString("\"") + longitude + QString("\"")  <<"\n" ;

        if(!bluetooth_device_address.isEmpty())
            outputfile_csv << (QObject::tr("Bluetooth Device Address")) << "," << QString("\"") + bluetooth_device_address + QString("\"")  <<"\n" ;

        if(!wifi_address.isEmpty())
            outputfile_csv << (QObject::tr("Wifi Address")) << "," << QString("\"") + wifi_address + QString("\"")  <<"\n" ;

        if(!locale_language.isEmpty())
            outputfile_csv << (QObject::tr("Locale Language")) << "," << QString("\"") + locale_language + QString("\"")  <<"\n" ;

        if(!target_system_timezone_QString.isEmpty())
            outputfile_csv << (QObject::tr("System Time Zone")) << "," << QString("\"") + target_system_timezone_QString + QString("\"")  <<"\n" ;

        if(!os_installation_timestamps.isEmpty())
        {
            outputfile_csv << (QObject::tr("Installer Date")) << "," << QString("\"") + os_installation_timestamps + QString("\"")  <<"\n" ;
        }

        if(!description.isEmpty())
            outputfile_csv << (QObject::tr("Description")) << "," << QString("\"") + description + QString("\"")  <<"\n" ;

        outputfile_csv << "\n";
    }

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}

QString recon_helper_singular::write_advanced_html_report_case_details_and_source_info(QString report_scope , narad_muni *narad_muni_for_reports)
{
    struct_case_details_values case_details_value_obj = fill_case_detials_values_structure(narad_muni_for_reports);
    struct_case_details_header_lables case_details_lables_obj = fill_case_detials_lables_structure();

    QString report_data = QString("<ul class=\"nav nav-tabs\" class =\"col-sm-1\" >"
                                  "<li class=\"active\"><a data-toggle=\"tab\" href=\"#casedetail\">"+ case_details_lables_obj.Case_Details_header_lable +"</a></li>");

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int i = 0; i < list_target_source_info.size(); i++)
    {
        QString source_table_name = list_target_source_info.at(i).source_count_name;
        report_data += QString("<li><a data-toggle=\"tab\" href=\"#" + source_table_name + "\">" + source_table_name + "</a></li>");
    }

    report_data += QString("</ul>");

    /// Case Details
    report_data += QString("<!-- Inserted Tab Content below -->"
                           "<div class=\"tab-content\">"
                           "<div id=\"casedetail\" class=\"tab-pane fade in active\">"
                           "<br>"

                           "<div class=\"table-responsive\">"
                           "<table class=\"table table-hover table-bordered table-condensed table table-striped\">"
                           "<tbody>"
                           "<tr>"
                           "<td width=\"25%\"><b>" +
                           case_details_lables_obj.app_version_lable + "</b></td>"+
                           "<td>" + case_details_value_obj.result_created_app_version + "</td>"
                                                                                        "</tr>"+
                           "<td width=\"25%\"><b>" + case_details_lables_obj.report_scope_lable +"</b></td>"+
                           "<td>" + report_scope + "</td>"
                                                   "</tr>"
                                                   " <tr>"+
                           " <td width=\"25%\"> <b> "+
                           case_details_lables_obj.case_no_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.case_no + "</td>"
                                                                     "</tr>"
                                                                     "<tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.case_name_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.case_name + "</td>"
                                                                       "</tr>"
                                                                       "<tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.location_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.location + "</td>"
                                                                      "</tr>"
                                                                      "<tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.notes_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.notes + "</td>"
                                                                   "</tr>"
                                                                   "<tr>"+
                           "<td width=\"25%\"><b>" +
                           case_details_lables_obj.examiner_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.examiner + "</td>"
                                                                      "</tr>"
                                                                      "</tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.examiner_phone_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.examiner_phone + "</td>"
                                                                            "</tr>"
                                                                            "</tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.examiner_email_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.examiner_email + "</td>"
                                                                            "</tr>"
                                                                            "</tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.agency_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.agency + "</td>"
                                                                    "</tr>"
                                                                    "</tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.agency_address_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.agency_address + "</td>"
                                                                            "</tr>"
                                                                            "</tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.user_timezone_string_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.user_timezone_string + "</td>"
                                                                                  "</tr>"
                                                                                  "</tr>"
                                                                                  "</tr>"+
                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.report_generated_date_time_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.report_generated_date_time + "</td>"
                                                                                        "</tr>"
                                                                                        "</tr>"
                                                                                        "</tr>"+

                           "<td width=\"25%\"><b>"+
                           case_details_lables_obj.machine_timezone_lable +"</b></td>"+
                           "<td>" + case_details_value_obj.machine_timezone_string + "</td>"
                                                                                     "</tr>"
                                                                                     "</tr>");



    report_data += "</tr>"
                   "</tbody>"
                   "</table>"
                   "</div>"
                   "</div>";

    /// Sources Details



    for(int i = 0; i < list_target_source_info.size(); i++)
    {

        QString complete_source_name        = list_target_source_info.at(i).complete_source_name;
        QString evidence_no           = list_target_source_info.at(i).evidence_number;
        QString description                 = list_target_source_info.at(i).description;

        QString source_table_name           = list_target_source_info.at(i).source_count_name;
        QString os_scheme_dsply            = list_target_source_info.at(i).os_scheme_display;
        QString os_scheme_intrnl            = list_target_source_info.at(i).os_scheme_internal;
        QString os_version_extended         = list_target_source_info.at(i).os_version_extended;
        QString latitude                    = list_target_source_info.at(i).latitude;
        QString longitude                   = list_target_source_info.at(i).longitude;
        QString country                     = list_target_source_info.at(i).country;
        QString city                        = list_target_source_info.at(i).city;

        QString file_system_type_str        = list_target_source_info.at(i).file_system_type;
        QString src_type_intrnl                 = list_target_source_info.at(i).source_type_internal;
        QString root_type            = list_target_source_info.at(i).root_type;
        QString configured_id_str           = list_target_source_info.at(i).configured_ids_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QStringList system_user_list        = list_target_source_info.at(i).system_user_QStringList;
        QString build_version               = list_target_source_info.at(i).build_version;
        QString device_name             = list_target_source_info.at(i).device_name;
        QString device_display_name     = list_target_source_info.at(i).display_name;
        QString device_id               = list_target_source_info.at(i).device_id;
        QString imei_no                 = list_target_source_info.at(i).imei_no_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString phone_no                = list_target_source_info.at(i).phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString serial_no               = list_target_source_info.at(i).serial_no;
        QString iccid_no                = list_target_source_info.at(i).sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma);
        QString product_name                = list_target_source_info.at(i).product_name;
        QString product_model                       = list_target_source_info.at(i).product_model;
        QString product_type                = list_target_source_info.at(i).product_type;
        QString target_type                 = list_target_source_info.at(i).target_type;
        QString itune_version           = list_target_source_info.at(i).itune_version;
        QString latest_backup_date_QString = list_target_source_info.at(i).latest_backup_date_readable_QString;
        QString target_system_timezone_QString = list_target_source_info.at(i).timezone_readable_QString;
        QString os_installation_timestamps  = convert_numeric_to_readable_in_utc_timezone(QString::number(list_target_source_info.at(i).os_installer_timestamp_utc_numeric_qint64) ,Q_FUNC_INFO);

        QString bluetooth_device_address          = list_target_source_info.at(i).bluetooth_device_address;
        QString wifi_address                      = list_target_source_info.at(i).wifi_address;
        QString locale_language                   = list_target_source_info.at(i).locale_language;


        report_data += QString("<div id=\"" + source_table_name + "\" class=\"tab-pane fade\">"
                                                                  "<br>"
                                                                  "<div class=\"table-responsive\">"
                                                                  "<table class=\"table table-hover table-bordered table-condensed table table-striped\">"
                                                                  "<tbody>"
                                                                  "<tr>");


        if(!complete_source_name.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Source Name</b></td>"
                                   "<td>" + complete_source_name  +"</td>"
                                                                   "</tr>"
                                                                   "<tr>");
        }

        if(!evidence_no.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Evidence No</b></td>"
                                   "<td>" +  evidence_no +"</td>"
                                                          "</tr>"
                                                          "<tr>");
        }

        if(!root_type.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b> Source Type </b></td>"
                                   "<td>"+ root_type  +"</td>"
                                                       "</tr>"
                                                       "<tr>");
        }

        if(!os_scheme_dsply.isEmpty())
        {
            if((os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    report_data += QString("<td width=\"25%\"> <b>OS Type</b></td>"
                                           "<td>"+ os_scheme_dsply +"</td>"
                                                                    "</tr>"
                                                                    "<tr>"
                                                                    "<tr>");
                }
            }
        }

        if(!file_system_type_str.isEmpty())
        {
            if(os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal)
            {
                report_data += QString("<td width=\"25%\"> <b> File System</b></td>"
                                       "<td>"+ file_system_type_str +"</td>"
                                                                     "</tr>"
                                                                     "<tr>");
            }
        }

        if(!system_user_list.isEmpty())
        {
            if((os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                    || os_scheme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
            {
                if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
                {
                    report_data += QString("<td width=\"25%\"><b>User(s)</b></td>"
                                           "<td>" + system_user_list.join(", ") + "</td>"
                                                                                  "</tr>"
                                                                                  "<tr>");
                }
            }
        }

        if(!configured_id_str.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Configured ID</b></td>"
                                   "<td>" + configured_id_str  +"</td>"
                                                                "</tr>"
                                                                "<tr>");
        }

        if(!device_display_name.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Display Name</b></td>"
                                   "<td>"+ device_display_name +"</td>"
                                                                "</tr>"
                                                                "<tr>"
                                                                "<tr>");
        }

        if(!device_name.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Device Name</b></td>"
                                   "<td>"+ device_name +"</td>"
                                                        "</tr>"
                                                        "<tr>"
                                                        "<tr>");
        }

        if(!device_id.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Device ID</b></td>"
                                   "<td>"+ device_id +"</td>"
                                                      "</tr>"
                                                      "<tr>"
                                                      "<tr>");
        }

        if(!imei_no.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Device IMEI</b></td>"
                                   "<td>"+ imei_no +"</td>"
                                                    "</tr>"
                                                    "<tr>"
                                                    "<tr>");
        }

        if(!serial_no.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Device Serial No.</b></td>"
                                   "<td>"+ serial_no +"</td>"
                                                      "</tr>"
                                                      "<tr>"
                                                      "<tr>");
        }

        if(!iccid_no.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Device ICCID</b></td>"
                                   "<td>"+ iccid_no +"</td>"
                                                     "</tr>"
                                                     "<tr>"
                                                     "<tr>");
        }

        if(!os_version_extended.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>OS Version</b></td>"
                                   "<td>"+ os_version_extended +"</td>"
                                                                "</tr>"
                                                                "<tr>"
                                                                "<tr>");

        }

        if(!phone_no.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Phone No.</b></td>"
                                   "<td>"+ phone_no +"</td>"
                                                     "</tr>"
                                                     "<tr>"
                                                     "<tr>");
        }

        if(!product_name.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Product Name</b></td>"
                                   "<td>"+ product_name +"</td>"
                                                         "</tr>"
                                                         "<tr>"
                                                         "<tr>");
        }

        if(!product_model.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Product Model</b></td>"
                                   "<td>"+ product_model +"</td>"
                                                          "</tr>"
                                                          "<tr>"
                                                          "<tr>");
        }

        if(!product_type.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b> Product Type</b></td>"
                                   "<td>"+ product_type +"</td>"
                                                         "<tr>");
        }

        if(!target_type.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Target Type</b></td>"
                                   "<td>"+ target_type +"</td>"
                                                        "</tr>"
                                                        "<tr>"
                                                        "<tr>");
        }

        if(!itune_version.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>iTune Version</b></td>"
                                   "<td>"+ itune_version +"</td>"
                                                          "</tr>"
                                                          "<tr>"
                                                          "<tr>");
        }

        if(!latest_backup_date_QString.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Backup Date</b></td>"
                                   "<td>"+ latest_backup_date_QString +"</td>"
                                                                       "</tr>"
                                                                       "<tr>"
                                                                       "<tr>");
        }

        if(!build_version.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Build Version</b></td>"
                                   "<td>"+ build_version +"</td>"
                                                          "</tr>"
                                                          "<tr>"
                                                          "<tr>");
        }

        if(!country.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Country</b></td>"
                                   "<td>"+ country +"</td>"
                                                    "</tr>"
                                                    "<tr>"
                                                    "<tr>");
        }

        if(!city.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>City</b></td>"
                                   "<td>"+ city +"</td>"
                                                 "</tr>"
                                                 "<tr>"
                                                 "<tr>");
        }

        if(!latitude.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Latitude</b></td>"
                                   "<td>"+ latitude +"</td>"
                                                     "</tr>"
                                                     "<tr>"
                                                     "<tr>");
        }

        if(!longitude.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Longitude</b></td>"
                                   "<td>"+ longitude +"</td>"
                                                      "</tr>"
                                                      "<tr>"
                                                      "<tr>");
        }

        if(!bluetooth_device_address.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Bluetooth Device Address</b></td>"
                                   "<td>"+ bluetooth_device_address +"</td>"
                                                                     "</tr>"
                                                                     "<tr>"
                                                                     "<tr>");
        }

        if(!wifi_address.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Wifi Address</b></td>"
                                   "<td>"+ wifi_address +"</td>"
                                                         "</tr>"
                                                         "<tr>"
                                                         "<tr>");
        }

        if(!locale_language.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>Locale Language</b></td>"
                                   "<td>"+ locale_language +"</td>"
                                                            "</tr>"
                                                            "<tr>"
                                                            "<tr>");
        }

        if(!target_system_timezone_QString.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b>System Time Zone</b></td>"
                                   "<td>"+ target_system_timezone_QString +"</td>"
                                                                           "</tr>"
                                                                           "<tr>"
                                                                           "<tr>");

        }

        if(!os_installation_timestamps.isEmpty())
        {
            report_data += QString("<td width=\"25%\"> <b> Installer Date </b></td>"
                                   "<td>"+ os_installation_timestamps  +"</td>"
                                                                        "</tr>"
                                                                        "<tr>");
        }

        if(!description.isEmpty())
        {
            report_data +=  QString("<td width=\"25%\"> <b> Description </b></td>"
                                    "<td>"+ description  + "</td>"
                                                           "</tr>"
                                                           "</tbody>"
                                                           "</table>"
                                                           "</div>"
                                                           "</div>");
        }


    }
    report_data += QString("</div>"
                           "</div>"
                           "</div>"
                           "</div>");

    return report_data;
}

recon_helper_singular::struct_case_details_values recon_helper_singular::fill_case_detials_values_structure(narad_muni *narad_muni_for_reports)
{
    struct_case_details_values obj;

    obj.result_created_app_version = narad_muni_for_reports->get_field(MACRO_NARAD_Result_App_Version_QString).toString();

    obj.case_no = narad_muni_for_reports->get_field(MACRO_NARAD_Case_ID_QString).toString();
    obj.case_name = narad_muni_for_reports->get_field(MACRO_NARAD_Case_Name_QString).toString();
    obj.location = narad_muni_for_reports->get_field(MACRO_NARAD_Case_Location_QString).toString();
    obj.notes = narad_muni_for_reports->get_field(MACRO_NARAD_Case_Notes_QString).toString();
    obj.examiner = narad_muni_for_reports->get_field(MACRO_NARAD_Examiner_Name_QString).toString();
    obj.examiner_phone = narad_muni_for_reports->get_field(MACRO_NARAD_Examiner_Phone_QString).toString();
    obj.examiner_email = narad_muni_for_reports->get_field(MACRO_NARAD_Examiner_Email_QString).toString();
    obj.agency = narad_muni_for_reports->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString();
    obj.agency_address = narad_muni_for_reports->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString();
    obj.user_timezone_string = narad_muni_for_reports->get_field(MACRO_NARAD_Examiner_Selected_Timezone_QString).toString();
    obj.report_generated_date_time =  convert_numeric_to_readable_in_selected_timezone(QString::number(QDateTime::currentSecsSinceEpoch()) ,Q_FUNC_INFO);
    obj.machine_timezone_string = narad_muni_for_reports->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();

    return obj;
}


recon_helper_singular::struct_case_details_header_lables recon_helper_singular::fill_case_detials_lables_structure()
{
    struct_case_details_header_lables obj;


    obj.RECON_Report_header_lable = QString("RECON Report");
    obj.Case_Info_header_lable=  QString("Case Info");
    obj.Case_Details_header_lable = QString("Case Details");
    obj.Examiner_Details_header_lable = QString("Examiner Details");
    obj.Sources_Details_header_lable = QString("Sources Details");


    obj.app_version_lable = QString("RECON Version");
    obj.case_no_lable = QString("Case No.");
    obj.case_name_lable = QString("Case Name");
    obj.report_scope_lable = QString("Report Scope");
    obj.examiner_lable = QString("Examiner");
    obj.examiner_phone_lable = QString("Examiner Phone");
    obj.examiner_email_lable = QString("Examiner Email");
    obj.agency_lable = QString("Agency Name");
    obj.agency_address_lable = QString("Agency Address");
    obj.location_lable = QString("Location");
    obj.notes_lable = QString("Case Notes");
    obj.user_timezone_string_lable = QString("User Selected Time Zone");
    obj.report_generated_date_time_lable = QString ("Report Generated Time");
    obj.machine_timezone_lable = QString("Report Generated Machine Time Zone");

    return obj;

}

QString recon_helper_singular::get_complete_file_path(QString target_path, QString filepath, QString caller_func)
{
    target_path = target_path.trimmed();
    if(!target_path.isEmpty() && !target_path.endsWith("/"))
    {
        target_path.append("/");
    }

    filepath = filepath.trimmed();
    if(!filepath.isEmpty() && !filepath.startsWith("/"))
    {
        filepath.prepend("/");
    }

    if(target_path.endsWith("/") && filepath.startsWith("/"))
        target_path.chop(1);

    return QString(target_path + filepath);
}



void recon_helper_singular::pub_set_added_timezone_details(QList<struct_global_added_timezone_details> added_list_obj)
{
    added_timezone_details_qlist.clear();
    added_timezone_details_qlist = added_list_obj;
}

void recon_helper_singular::pub_set_default_timezone(struct_global_default_timezone_details default_obj)
{
    struct_default_timezone_details_obj = default_obj;
}


QIcon recon_helper_singular::get_qicon_of_source(struct_GLOBAL_witness_info_source struct_source_info)
{
    struct_GLOBAL_witness_info_source mobj = struct_source_info;

    QFileIconProvider icon_provide;
    QIcon icon_path = icon_provide.icon(QFileIconProvider::Drive);


    if(mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
    {
        return QIcon("../icons/os/macOS.png");
    }
    else if(mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
    {
        return QIcon("../icons/os/iOS.png");
    }
    else if(mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
    {
        return QIcon("../icons/os/winOS.png");
    }
    else if(mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display)
    {
        return QIcon("../icons/os/google_takeoutOS.png");
    }
    else if(mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
    {
        return QIcon("../icons/os/androidOS.png");
    }
    else if(mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display)
    {
        if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
        {
            return QIcon("../icons/source/mounted_volumes.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal) && (mobj.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if((mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal))
        {
            return QIcon("../icons/source/partition.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
        {
            return QIcon("../icons/source/folder.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal)
        {
            return QIcon("../icons/source/folder.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)
        {
            return QIcon("../icons/source/folder.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
        {
            return QIcon("../icons/source/time_machine_backup_image.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
        {
            return QIcon("../icons/source/time_machine_backup_folder.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
        {
            return QIcon("../icons/source/ram.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
        {
            return QIcon("../icons/source/macos_home_directory.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal)
        {
            return QIcon("../icons/source/google_takeout_backup.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
        {
            return QIcon("../icons/source/android_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
        {
            return QIcon("../icons/source/android_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
        {
            return QIcon("../icons/source/android_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            return QIcon("../icons/source/ios_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
        {
            return QIcon("../icons/source/ios_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
        {
            return QIcon("../icons/source/ios_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
        {
            return QIcon("../icons/source/ios_backup_general.png");
        }
        else if(mobj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
        {
            return QIcon("../icons/source/file.png");
        }
    }

    return icon_path;
}



bool recon_helper_singular::is_it_recon_custom_picture_plugin(QString plugin_name)
{

    if(plugin_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
            || plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
            || plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger)
            || plugin_name == QString(MACRO_Plugin_Name_Screenshots)
            || plugin_name == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        return true;
    }


    return false;
}


QIcon recon_helper_singular::get_qicon_of_root(struct_GLOBAL_witness_info_root struct_root_info)
{

    QFileIconProvider icon_provide;
    QIcon icon_path = icon_provide.icon(QFileIconProvider::Drive);


    if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes)
    {
        return QIcon("../icons/root/mounted_volumes.png");
        // return icon_provide.icon(QFileIconProvider::Drive);
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage)
    {
        return QIcon("../icons/root/image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage)
    {
        return QIcon("../icons/root/image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
    {
        return QIcon("../icons/root/image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage)
    {
        return QIcon("../icons/root/recon_fs_block_image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup)
    {
        return QIcon("../icons/root/ios_backup_general.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup)
    {
        return QIcon("../icons/root/ios_backup_general.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyBackup)
    {
        return QIcon("../icons/root/graykey_backup.png");
    }

    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout)
    {
        return QIcon("../icons/root/google_takeout_backup.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup)
    {
        return QIcon("../icons/root/android_backup_general.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder)
    {
        return QIcon("../icons/root/time_machine_backup_folder.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage)
    {
        return QIcon("../icons/root/time_machine_backup_image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage)
    {
        return QIcon("../icons/root/ram.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory)
    {
        return QIcon("../icons/root/macos_home_directory.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder)
    {
        return QIcon("../icons/root/folder.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File)
    {
        return QIcon("../icons/root/file.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage)
    {
        return QIcon("../icons/root/optical_disc_image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage)
    {
        return QIcon("../icons/root/recon_logical_image.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode)
    {
        return QIcon("../icons/root/recon_mac_sharing_mode.png");
    }
    else if(struct_root_info.root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage)
    {
        return QIcon("../icons/root/encase_logical_image.png");
    }


    return icon_path;

}

QString recon_helper_singular::get_file_system_of_any_node(QString dsk_node, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    // /dev/disk5s1 on /Volumes/Untitled      (apfs, local, nodev, nosuid, journaled, noowners)
    // //recon@192.168.0.111/ProjectManager on /Volumes/ProjectManager (smbfs, nodev, nosuid, mounted by forenso)

    // output : apfs


    QString fs_value;

    QString temp_plist_file_path = get_plist_file_path_after_running_diskutill_info(dsk_node, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), recon_static_functions::prepare_callerfun(caller_func));
    fs_value = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "FilesystemType", recon_static_functions::prepare_callerfun(caller_func)).trimmed();

    QFile temp_file(temp_plist_file_path);
    if(temp_file.exists())
        temp_file.remove();

    if(fs_value.isEmpty())
    {
        QProcess proc_mount;
        proc_mount.start("mount");
        proc_mount.waitForFinished(3000);


        QString str_1 = dsk_node + " on";

        QStringList all_media_list = QString::fromLocal8Bit(proc_mount.readAll()).split("\n");

        for(int i = 0; i < all_media_list.size(); i++)
        {
            QString line_val_str = all_media_list.at(i);

            if(!line_val_str.startsWith(str_1))
                continue;

            int m_index = line_val_str.lastIndexOf(" (");
            if(m_index == -1)
            {
                break;
            }

            line_val_str = line_val_str.remove(0, (m_index + 2)); // " ("

            line_val_str.remove(line_val_str.indexOf(","), line_val_str.size());

            fs_value = line_val_str.trimmed();

        }
    }

    if(fs_value == (MACRO_FILE_SYSTEM_TYPE_LIFS))
        fs_value = MACRO_FILE_SYSTEM_TYPE_NTFS;

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  end " , Q_FUNC_INFO);

    return fs_value;
}


QString recon_helper_singular::get_file_system_of_filepath(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  start " , Q_FUNC_INFO);


    /// Get all Mounted volume disk node with mount path splitted by MACRO_RECON_SPLITTER
    QStringList mounted_path_with_node_list =  get_mounted_volumes_path_with_node_list(Q_FUNC_INFO);


    QString node_value;
    bool is_path_found = false;
    for(int pp = 0; pp < mounted_path_with_node_list.size(); pp++)
    {
        QString tmp_str = mounted_path_with_node_list.at(pp);
        QStringList tmp_list = tmp_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            continue;

        if(source_path.startsWith(tmp_list.at(1)))
        {
            node_value = tmp_list.at(0);
            is_path_found = true;
            break;
        }
    }


    if(!is_path_found)
    {
        node_value = get_disk_node_of_mounted_path(QString("/") , Q_FUNC_INFO);
    }


    QString fs_type = get_file_system_of_any_node(node_value, Q_FUNC_INFO);

    if(fs_type == (MACRO_FILE_SYSTEM_TYPE_LIFS))
        fs_type = MACRO_FILE_SYSTEM_TYPE_NTFS;

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  end " , Q_FUNC_INFO);

    return fs_type;
}


bool recon_helper_singular::is_system_pwd_accurate(QString m_pwd_orgl, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    bool mvalue = false;
    QStringList args;
    args << "-u" << QString(getenv("USER")).trimmed() << "-P" << m_pwd_orgl << "." << "-delete" << "/Users/xvhfjdhvig8fer7kewho8476rhjfhef8gfyz";
    QString pwd_status = run_command_generic("dscl", args, Q_FUNC_INFO);
    if(pwd_status.contains("delete: Invalid Path"))
    {
        mvalue = true;
    }

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return mvalue;
}




void recon_helper_singular::pub_fill_destination_drive_info(QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    recon_static_functions::clear_variables_destination_drive(st_destination_drive_info);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QStorageInfo storage_info(result_dir_path);


    st_destination_drive_info.label_name = storage_info.name();
    st_destination_drive_info.volume_name = storage_info.displayName();
    st_destination_drive_info.device_disk_node = storage_info.device();
    st_destination_drive_info.drive_path = storage_info.rootPath();
    st_destination_drive_info.block_size_numeric_int = storage_info.blockSize();


    st_destination_drive_info.file_system_type = storage_info.fileSystemType();
    st_destination_drive_info.available_size_byte_at_case_uptime_numeric_qint64 = storage_info.bytesAvailable();
    st_destination_drive_info.total_size_byte_numeric_qint64 = storage_info.bytesTotal();
    st_destination_drive_info.bool_read_only = storage_info.isReadOnly();

    st_destination_drive_info.bool_ready_to_work = storage_info.isReady();
    st_destination_drive_info.bool_system_root_volume = storage_info.isRoot();
    st_destination_drive_info.bool_valid_drive = storage_info.isValid();


    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
}



struct_global_destination_drive_info recon_helper_singular::get_saved_destination_drive_info(QString caller_func)
{
    return st_destination_drive_info;
}

QString recon_helper_singular::convert_raw_data_to_ascii_convertor(QByteArray data_byte_array, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QByteArray text_byte_array = QByteArray::fromHex(data_byte_array);
    text_byte_array.data();
    QString ascii_str;

    for(int ii = 0; ii < text_byte_array.size(); ii += Global_Variable_hex_viewer_BYTES_PER_LINE_int)
    {
        QByteArray ascii = text_byte_array.mid(ii,Global_Variable_hex_viewer_BYTES_PER_LINE_int);

        for(int idx = 0; idx < ascii.size(); idx++)
        {
            if(((char)ascii[idx]<0x20) || ((char)ascii[idx]>0x7e))
            {
                ascii_str += QString(".");
            }
            else
            {
                ascii_str += QString(ascii.at(idx));
            }
        }

        QString hex_bytes_with_space;
        QByteArray hex_values = text_byte_array.mid(ii,Global_Variable_hex_viewer_BYTES_PER_LINE_int);
        for(int idx = 0 ; idx < hex_values.size(); idx++)
        {
            QByteArray byte_arr = hex_values.mid(idx,1);
            hex_bytes_with_space += QString::fromLocal8Bit(byte_arr.toHex().toUpper()) + QString("\t");
        }

        if(hex_bytes_with_space.size() < 48)
        {
            int diff = (48 - hex_bytes_with_space.size());

            for(int j = 0; j < diff; j++)
            {
                hex_bytes_with_space.append("&nbsp;\t");
            }

        }
    }

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return ascii_str;
}

int recon_helper_singular::hexToInt(char c)
{
    int first = c / 16 - 3;
    int second = c % 16;
    int result = first*10 + second;
    if (result > 9) result--;
    return result;
}

int recon_helper_singular::hexToLetter(char c, char d)
{
    int high = hexToInt(c) * 16;
    int low = hexToInt(d);

    return high+low;
}

QString recon_helper_singular::convert_raw_data_to_unicode_convertor(QByteArray data_byte_array, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    const char* hexChar = data_byte_array;
    int length = strlen(hexChar);
    char buf = 0;

    QString sample_str;
    QString final_str;
    for(int ii = 0; ii < length; ii++)
    {
        if(ii % 2 != 0)
        {
            sample_str = static_cast<char>(hexToLetter(buf,hexChar[ii]));
            final_str.append(sample_str);
        }
        else
        {
            buf = hexChar[ii];
        }
    }

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return final_str;

}

