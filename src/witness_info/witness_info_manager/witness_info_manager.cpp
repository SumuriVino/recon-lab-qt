#include "witness_info_manager.h"

witness_info_manager::witness_info_manager(QWidget *parent)
{

}

witness_info_manager::~witness_info_manager()
{

}


void witness_info_manager::pub_fill_witness_info_QList_from_store()
{
    recon_static_functions::app_debug("Starts ---- ",Q_FUNC_INFO);

    fill_root_structure_list_from_db();
    fill_root_structure_list_from_text_file();


    fill_source_structure_list_from_db();
    fill_source_structure_list_from_text_file();


    recon_static_functions::app_debug(" Ends ---- ",Q_FUNC_INFO);
}

void witness_info_manager::fill_source_structure_list_from_db()
{
    recon_static_functions::app_debug(" Starts ---- ",Q_FUNC_INFO);

    QString source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(source_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + source_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" " + destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_source_count_name(destination_db);
    query_source_count_name.prepare("SELECT source_count_name FROM fs_status");

    if(!query_source_count_name.exec())
    {
        recon_static_functions::app_debug(" query select source count name ----FAILED---- " + source_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" last error" + query_source_count_name.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" query - " + query_source_count_name.executedQuery() ,Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query_source_count_name.next())
    {

        QString source_count_name = query_source_count_name.value(0).toString();
        bool bool_source_already_added = false;
        for(int i = 0; i < list_witness_info_source.size(); i++)
        {
            struct_GLOBAL_witness_info_source struct_source_info = list_witness_info_source.at(i);
            if(struct_source_info.source_count_name == source_count_name)
            {
                bool_source_already_added = true;
                break;
            }
        }
        if(bool_source_already_added)
            continue;


        QSqlQuery query_select(destination_db);

        query_select.prepare("SELECT os_scheme_display, os_scheme_internal, users_list, source_type_display, source_type_internal, source_name,"
                             "source_main_path, virtual_source_path,  product_type, os_version_extended, os_version_full,"
                             "lattitude, longitude, country, city, display_name, device_name, id_no, guid, product_name, target_type,"
                             "build_version,itune_version, serial_no, phone_no_msisdn, imei, sim_no_iccid, latest_backup_date_readable,"
                             "latest_backup_date_numeric, backup_size_byte_numeric, recon_logical_image, timezone_readable, timezone_numeric,"
                             "source_main_name, volume_type, total_size_byte_numeric,free_size_byte_numeric, used_size_byte_numeric, file_system_type, source_path_1,"
                             "source_path_2_for_fusion, root_count_name, volume_offset, volume_uuid, configured_id, source_hierarchy,"
                             "is_source_decompressed, is_source_nested, source_disk_identifier, installer_timestamp_utc_numeric, product_model,"
                             "bluetooth_device_address, wifi_address, locale_language, role_type, source_evidence_password "
                             " FROM tbl_source_index where source_count_name = ?");


        query_select.addBindValue(source_count_name);

        if(!query_select.exec())
        {
            recon_static_functions::app_debug(" query select source info ----FAILED---- " + source_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error - " + query_select.lastError().text() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(" query - " + query_select.executedQuery() ,Q_FUNC_INFO);
            // continue;  donot do continue
        }

        struct_GLOBAL_witness_info_source struct_source_info;
        recon_static_functions::clear_variables_witness_info_source_structure(struct_source_info);

        struct_source_info.source_count_name = source_count_name;

        if(query_select.next())
        {

            struct_source_info.root_count_name                                   = query_select.value("root_count_name").toString();

            //============== To be called just after filling root_count_name for source -Start==============//

            update_source_structure_for_root_info(struct_source_info);

            //============== To be called just after filling root_count_name for source -End==============//



            struct_source_info.source_disk_identifier                            = query_select.value("source_disk_identifier").toString();

            struct_source_info.os_scheme_display                                 = query_select.value("os_scheme_display").toString();
            struct_source_info.os_scheme_internal                                = query_select.value("os_scheme_internal").toString();

            QString system_user_str = query_select.value("users_list").toString().trimmed();

            struct_source_info.system_account_QList = get_system_account_qlist_for_all_users(system_user_str);
            struct_source_info.system_user_QStringList = recon_static_functions::get_system_user_list_from_system_account_QList(struct_source_info.system_account_QList , Q_FUNC_INFO);

            struct_source_info.source_type_display                               = query_select.value("source_type_display").toString();
            struct_source_info.source_type_internal                              = query_select.value("source_type_internal").toString();
            struct_source_info.source_name                                       = query_select.value("source_name").toString();

            struct_source_info.source_main_path                                  = query_select.value("source_main_path").toString();

            struct_source_info.virtual_source_path                               = query_select.value("virtual_source_path").toString();

            struct_source_info.product_type                                      = query_select.value("product_type").toString();
            struct_source_info.os_version_extended                               = query_select.value("os_version_extended").toString();
            struct_source_info.os_version_full                                   = query_select.value("os_version_full").toString();
            struct_source_info.latitude                                          = query_select.value("lattitude").toString();
            struct_source_info.longitude                                         = query_select.value("longitude").toString();
            struct_source_info.country                                           = query_select.value("country").toString();
            struct_source_info.city                                              = query_select.value("city").toString();
            struct_source_info.display_name                                      = query_select.value("display_name").toString();
            struct_source_info.device_name                                       = query_select.value("device_name").toString();
            struct_source_info.device_id                                             = query_select.value("id_no").toString();
            struct_source_info.guid                                              = query_select.value("guid").toString();
            struct_source_info.product_name                                      = query_select.value("product_name").toString();
            struct_source_info.target_type                                       = query_select.value("target_type").toString();
            struct_source_info.build_version                                     = query_select.value("build_version").toString();
            struct_source_info.itune_version                                     = query_select.value("itune_version").toString();
            struct_source_info.serial_no                                         = query_select.value("serial_no").toString();
            struct_source_info.phone_no_msisdn_QStringList                       = query_select.value("phone_no_msisdn").toString().split(MACRO_RECON_Splitter_6_comma);
            struct_source_info.imei_no_QStringList                               = query_select.value("imei").toString().split(MACRO_RECON_Splitter_6_comma);

            struct_source_info.sim_no_iccid_QStringList                          = query_select.value("sim_no_iccid").toString().split(MACRO_RECON_Splitter_6_comma);
            struct_source_info.latest_backup_date_readable_QString               = query_select.value("latest_backup_date_readable").toString();
            struct_source_info.latest_backup_date_numeric_qint64                 = query_select.value("latest_backup_date_numeric").toLongLong();
            struct_source_info.backup_size_byte_numeric_QString                  = query_select.value("backup_size_byte_numeric").toString();

            struct_source_info.os_installer_timestamp_utc_numeric_qint64         = query_select.value("installer_timestamp_utc_numeric").toLongLong();

            QString live_src_stat                                                = query_select.value("recon_logical_image").toString();
            if(live_src_stat == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                struct_source_info.bool_recon_logical_image = true;
            else
                struct_source_info.bool_recon_logical_image = false;

            struct_source_info.timezone_readable_QString                         = query_select.value("timezone_readable").toString();
            struct_source_info.timezone_numeric_qint64                           = query_select.value("timezone_numeric").toLongLong();

            struct_source_info.source_main_name                                  = query_select.value("source_main_name").toString();

            struct_source_info.image_volume_type                                 = query_select.value("volume_type").toString();

            struct_source_info.total_size_byte_numeric_QString                   = query_select.value("total_size_byte_numeric").toString();
            struct_source_info.free_space_byte_numeric_QString                   = query_select.value("free_size_byte_numeric").toString();
            struct_source_info.used_space_byte_numeric_QString                   = query_select.value("used_size_byte_numeric").toString();

            struct_source_info.file_system_type                                  = query_select.value("file_system_type").toString();
            struct_source_info.source_path_1                                     =  query_select.value("source_path_1").toString();

            struct_source_info.source_path_2_for_fusion                          = query_select.value("source_path_2_for_fusion").toString();

            struct_source_info.image_volume_offset                               = query_select.value("volume_offset").toString();
            struct_source_info.image_volume_uuid                                 = query_select.value("volume_uuid").toString();
            struct_source_info.configured_ids_QStringList                        = query_select.value("configured_id").toString().split(MACRO_RECON_Splitter_6_comma);
            struct_source_info.source_hierarchy                                  = query_select.value("source_hierarchy").toString();


            QString decompressed_stat = query_select.value("is_source_decompressed").toString();
            if(decompressed_stat == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                struct_source_info.bool_source_decompressed = true;
            else
                struct_source_info.bool_source_decompressed = false;


            QString nest_stat = query_select.value("is_source_nested").toString();
            if(nest_stat == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
            {
                struct_source_info.bool_source_nested = true;
            }
            else
                struct_source_info.bool_source_nested = false;


            struct_source_info.product_model                                    = query_select.value("product_model").toString();
            struct_source_info.bluetooth_device_address                         = query_select.value("bluetooth_device_address").toString();
            struct_source_info.wifi_address                                     = query_select.value("wifi_address").toString();
            struct_source_info.locale_language                                  = query_select.value("locale_language").toString();
            struct_source_info.role_type                                        = query_select.value("role_type").toString();

            struct_source_info.source_evidence_password_encrypted               = query_select.value("source_evidence_password").toString();
            struct_source_info.source_evidence_password_plain                   = recon_static_functions::decrypt_string(struct_source_info.source_evidence_password_encrypted, Q_FUNC_INFO);
            struct_source_info.source_evidence_password_modified_symbol_hex     = recon_static_functions::get_modified_password_replace_symbol_with_hex(struct_source_info.source_evidence_password_plain,Q_FUNC_INFO);

            struct_source_info.snapshots_QList = get_apfs_snapshots_QList_for_source(struct_source_info.source_count_name, destination_db);

        }

        //=========================== Additional Insertion Start ===================//
        // donot insert value in upper while loop
        // bcoz we dont save these value in database

        struct_source_info.fs_extraction_source_path                = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj, struct_source_info.source_type_internal, struct_source_info.virtual_source_path, Q_FUNC_INFO);


        if(struct_source_info.bool_source_nested)
        {
            QStringList src_hir_list = struct_source_info.source_hierarchy.split(MACRO_RECON_Splitter_4_forward);
            if(src_hir_list.size() > 1)
                struct_source_info.parent_source_count_name = src_hir_list.at(src_hir_list.size() - 1);
        }

        struct_source_info.complete_source_name             = get_complete_source_name_from_source(struct_source_info);
        //=========================== Additional Insertion end ===================//

        struct_source_info.configured_ids_QStringList.removeAll("");
        struct_source_info.imei_no_QStringList.removeAll("");
        struct_source_info.phone_no_msisdn_QStringList.removeAll("");
        struct_source_info.sim_no_iccid_QStringList.removeAll("");
        struct_source_info.system_user_QStringList.removeAll("");

        struct_source_info.configured_ids_QStringList.removeDuplicates();
        struct_source_info.imei_no_QStringList.removeDuplicates();
        struct_source_info.phone_no_msisdn_QStringList.removeDuplicates();
        struct_source_info.sim_no_iccid_QStringList.removeDuplicates();
        struct_source_info.system_user_QStringList.removeDuplicates();

        list_witness_info_source.append(struct_source_info);

    }


    destination_db.close();

    recon_static_functions::app_debug(" LIST OF TARGET STRUCTURE SIZE -- " + QString::number(list_witness_info_source.size()),Q_FUNC_INFO);
    recon_static_functions::app_debug(" Ends ---- ",Q_FUNC_INFO);
}

void witness_info_manager::fill_source_structure_list_from_text_file()
{
    recon_static_functions::app_debug(" Starts ---- ",Q_FUNC_INFO);

    QString source_index_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_index.txt";

    QFile file(source_index_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ----FAILED---" + source_index_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + file.errorString(),Q_FUNC_INFO);
        return;
    }

    QStringList source_file_name_list;

    while(!file.atEnd())
        source_file_name_list << QString::fromLocal8Bit(file.readLine()).trimmed();

    file.close();


    for(int pp = 0; pp < source_file_name_list.size(); pp++)
    {
        QString saved_src_cnt_name = source_file_name_list.at(pp);
        if(saved_src_cnt_name.endsWith(QString(".txt")))
            saved_src_cnt_name.chop(QString(".txt").size());

        saved_src_cnt_name = saved_src_cnt_name.trimmed();

        bool bool_source_already_added = false;

        int source_pos = 0;
        for(int i = 0; i < list_witness_info_source.size(); i++)
        {
            struct_GLOBAL_witness_info_source struct_source_info = list_witness_info_source.at(i);
            QString source_file_name = struct_source_info.source_count_name;
            if(source_file_name == saved_src_cnt_name)
            {
                bool_source_already_added = true;
                source_pos = i;
                break;
            }
        }

        if(bool_source_already_added)
        {
            int zzz = source_pos;

            struct_GLOBAL_witness_info_source struct_source_info = list_witness_info_source.takeAt(zzz);

            QString source_file_name = struct_source_info.source_count_name;

            //            int count_val = 0;
            //            QString source_count = source_file_name;
            //            if(source_count.startsWith(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag))
            //            {
            //                source_count.remove(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag);
            //                bool ok = false;
            //                count_val = source_count.toInt(&ok,10);
            //                if(ok)
            //                {
            //                    //struct_source_info.source_count_int = count_val;
            //                }
            //            }
            //            if(count_val == 0)
            //            {
            //                recon_static_functions::app_debug("SOURCE COUNT  ----FAILED--- " + source_file_name,Q_FUNC_INFO);
            //                list_witness_info_source.insert(zzz,struct_source_info);
            //                continue;
            //            }

            QString source_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + source_file_name + ".txt";

            QFile file(source_file_path);
            if(!file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("file open ----FAILED---" + source_file_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("Error--- " + file.errorString(),Q_FUNC_INFO);
                list_witness_info_source.insert(zzz,struct_source_info);
                continue;
            }
            while(!file.atEnd())
            {
                QString file_line = file.readLine();

                file_line.replace("\n","");

                QStringList tmp_list = file_line.split(QString(MACRO_RECON_Splitter_8_equal_hash_equal), Qt::SkipEmptyParts);

                if(tmp_list.size() < 2)
                    continue;


                if((tmp_list.at(0) == MACRO_NARAD_Source_Type_Display_QString) && (struct_source_info.source_type_display.isEmpty()))
                {
                    struct_source_info.source_type_display              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Type_Internal_QString) && (struct_source_info.source_type_internal.isEmpty()))
                {
                    struct_source_info.source_type_internal              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Count_Name_QString) && (struct_source_info.root_count_name.isEmpty()))
                {
                    struct_source_info.root_count_name                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Count_Name_QString) && (struct_source_info.source_count_name.isEmpty()))
                {
                    struct_source_info.source_count_name                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Name_QString) && (struct_source_info.source_name.isEmpty()))
                {
                    struct_source_info.source_name                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Main_Path_QString) && (struct_source_info.source_main_path.isEmpty()))
                {
                    struct_source_info.source_main_path           = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Time_Zone_Readable_QString) && (struct_source_info.timezone_readable_QString.isEmpty()))
                {
                    struct_source_info.timezone_readable_QString       = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Time_Zone_Numeric_qint64) && (struct_source_info.timezone_numeric_qint64 == 0))
                {
                    struct_source_info.timezone_numeric_qint64   = tmp_list.at(1).toLongLong();
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_OS_Scheme_Display_QString) && (struct_source_info.os_scheme_display.isEmpty()))
                {
                    struct_source_info.os_scheme_display              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_OS_Scheme_Internal_QString) && (struct_source_info.os_scheme_internal.isEmpty()))
                {
                    struct_source_info.os_scheme_internal              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_System_Account_QList) && (struct_source_info.system_account_QList.isEmpty()))
                {
                    QString system_user_str = tmp_list.at(1).trimmed();

                    struct_source_info.system_account_QList = get_system_account_qlist_for_all_users(system_user_str);
                    struct_source_info.system_user_QStringList = recon_static_functions::get_system_user_list_from_system_account_QList(struct_source_info.system_account_QList , Q_FUNC_INFO);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Product_Type_QString) && (struct_source_info.product_type.isEmpty()))
                {
                    struct_source_info.product_type                 = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_OS_Version_Extended_QString) && (struct_source_info.os_version_extended.isEmpty()))
                {
                    struct_source_info.os_version_extended          = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_OS_Version_Full_QString) && (struct_source_info.os_version_full.isEmpty()))
                {
                    struct_source_info.os_version_full              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Country_QString) && (struct_source_info.country.isEmpty()))
                {
                    struct_source_info.country                      = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_City_QString) && (struct_source_info.city.isEmpty()))
                {
                    struct_source_info.city                         = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Latitude_QString) && (struct_source_info.latitude.isEmpty()))
                {
                    struct_source_info.latitude                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Longitude_QString) && (struct_source_info.longitude.isEmpty()))
                {
                    struct_source_info.longitude                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Build_Version_QString) && (struct_source_info.build_version.isEmpty()))
                {
                    struct_source_info.build_version                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Display_Name_QString) && (struct_source_info.display_name.isEmpty()))
                {
                    struct_source_info.display_name                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Device_Name_QString) && (struct_source_info.device_name.isEmpty()))
                {
                    struct_source_info.device_name                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Id_No_QString) && (struct_source_info.device_id.isEmpty()))
                {
                    struct_source_info.device_id                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_GUID_QString) && (struct_source_info.guid.isEmpty()))
                {
                    struct_source_info.guid                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Product_Name_QString) && (struct_source_info.product_name.isEmpty()))
                {
                    struct_source_info.product_name                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Target_Type_QString) && (struct_source_info.target_type.isEmpty()))
                {
                    struct_source_info.target_type                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_iTune_Version_QString) && (struct_source_info.itune_version.isEmpty()))
                {
                    struct_source_info.itune_version                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Serial_No_QString) && (struct_source_info.serial_no.isEmpty()))
                {
                    struct_source_info.serial_no                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Phone_No_MSISDN_QStringList) && (struct_source_info.phone_no_msisdn_QStringList.isEmpty()))
                {
                    struct_source_info.phone_no_msisdn_QStringList                    = tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Imei_QStringList) && (struct_source_info.imei_no_QStringList.isEmpty()))
                {
                    struct_source_info.imei_no_QStringList                    = tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Sim_No_Iccid_QStringList) && (struct_source_info.sim_no_iccid_QStringList.isEmpty()))
                {
                    struct_source_info.sim_no_iccid_QStringList                    = tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString) && (struct_source_info.latest_backup_date_readable_QString.isEmpty()))
                {
                    struct_source_info.latest_backup_date_readable_QString                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64) && (struct_source_info.latest_backup_date_numeric_qint64 == 0))
                {
                    struct_source_info.latest_backup_date_numeric_qint64                    = tmp_list.at(1).toLongLong();
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString) && (struct_source_info.backup_size_byte_numeric_QString.isEmpty()))
                {
                    struct_source_info.backup_size_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Main_Name_QString) && (struct_source_info.source_main_name.isEmpty()))
                {
                    struct_source_info.source_main_name                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Volume_Type_QString) && (struct_source_info.image_volume_type.isEmpty()))
                {
                    struct_source_info.image_volume_type                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Volume_Offset_QString) && (struct_source_info.image_volume_offset.isEmpty()))
                {
                    struct_source_info.image_volume_offset                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Volume_UUID_QString) && (struct_source_info.image_volume_uuid.isEmpty()))
                {
                    struct_source_info.image_volume_uuid                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString) && (struct_source_info.total_size_byte_numeric_QString.isEmpty()))
                {
                    struct_source_info.total_size_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString) && (struct_source_info.used_space_byte_numeric_QString.isEmpty()))
                {
                    struct_source_info.used_space_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString) && (struct_source_info.free_space_byte_numeric_QString.isEmpty()))
                {
                    struct_source_info.free_space_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_File_System_Type_QString) && (struct_source_info.file_system_type.isEmpty()))
                {
                    struct_source_info.file_system_type                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Path_2_For_Fusion_QString) && (struct_source_info.source_path_2_for_fusion.isEmpty()))
                {
                    struct_source_info.source_path_2_for_fusion                    = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_RECON_Logical_Mode_bool) && (!struct_source_info.bool_recon_logical_image))
                {
                    if(tmp_list.at(1) == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        struct_source_info.bool_recon_logical_image = true;
                    else
                        struct_source_info.bool_recon_logical_image = false;
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Configured_ID_QStringList) && (struct_source_info.configured_ids_QStringList.isEmpty()))
                {
                    struct_source_info.configured_ids_QStringList        = QString(tmp_list.at(1)).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Disk_Identifier_QString)
                        && (struct_source_info.source_disk_identifier.isEmpty()))
                {
                    struct_source_info.source_disk_identifier        = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Role_Type_QString)
                        && (struct_source_info.role_type.isEmpty()))
                {
                    struct_source_info.role_type        = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Path_1_QString) && (struct_source_info.source_path_1.isEmpty()))
                {
                    struct_source_info.source_path_1                =  tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Virtual_Source_Path_QString) && (struct_source_info.virtual_source_path.isEmpty()))
                {
                    struct_source_info.virtual_source_path          =  tmp_list.at(1);
                }
                else if(tmp_list.at(0) == QString(MACRO_NARAD_Source_Nested_bool) && !struct_source_info.bool_source_nested)
                {
                    if(tmp_list.at(1) == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        struct_source_info.bool_source_nested   =  true;
                    else
                        struct_source_info.bool_source_nested   =  false;
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Hierarchy_QString) && (struct_source_info.source_hierarchy.isEmpty()))
                {
                    struct_source_info.source_hierarchy        = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == QString(MACRO_NARAD_Source_Decompressed_bool) && !struct_source_info.bool_source_decompressed)
                {
                    if(tmp_list.at(1) == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        struct_source_info.bool_source_decompressed   =  true;
                    else
                        struct_source_info.bool_source_decompressed   =  false;
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64) && (struct_source_info.os_installer_timestamp_utc_numeric_qint64 == 0))
                {
                    struct_source_info.os_installer_timestamp_utc_numeric_qint64         = tmp_list.at(1).toLongLong();
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Product_Model_QString) && (struct_source_info.product_model.isEmpty()))
                {
                    struct_source_info.product_model         = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Bluetooth_Device_Address_QString) && (struct_source_info.bluetooth_device_address.isEmpty()))
                {
                    struct_source_info.bluetooth_device_address         = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Wifi_Address_QString) && (struct_source_info.wifi_address.isEmpty()))
                {
                    struct_source_info.wifi_address         = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Locale_Language_QString) && (struct_source_info.locale_language.isEmpty()))
                {
                    struct_source_info.locale_language         = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Source_Evidence_Password_Encrypted_QString) && (struct_source_info.source_evidence_password_encrypted.isEmpty()))
                {
                    struct_source_info.source_evidence_password_encrypted         = tmp_list.at(1);
                    struct_source_info.source_evidence_password_plain   = recon_static_functions::decrypt_string(struct_source_info.source_evidence_password_encrypted, Q_FUNC_INFO);
                    struct_source_info.source_evidence_password_modified_symbol_hex     = recon_static_functions::get_modified_password_replace_symbol_with_hex(struct_source_info.source_evidence_password_plain,Q_FUNC_INFO);
                }
            }

            file.close();

            //=========================== Additional Insertion Start ===================//
            // donot insert value in upper while loop
            // bcoz we dont save these value in text file

            update_source_structure_for_root_info(struct_source_info);


            if(struct_source_info.snapshots_QList.isEmpty())
                struct_source_info.snapshots_QList = get_apfs_snapshots_QList_for_source_from_snapshots_plist(struct_source_info.source_count_name);


            if(struct_source_info.fs_extraction_source_path.isEmpty())
            {
                struct_source_info.fs_extraction_source_path                = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj, struct_source_info.source_type_internal, struct_source_info.virtual_source_path, Q_FUNC_INFO);
            }

            if(struct_source_info.parent_source_count_name.isEmpty() && struct_source_info.bool_source_nested)
            {
                QStringList src_hir_list = struct_source_info.source_hierarchy.split(MACRO_RECON_Splitter_4_forward);
                if(src_hir_list.size() > 1)
                    struct_source_info.parent_source_count_name = src_hir_list.at(src_hir_list.size() - 1);
            }

            if(struct_source_info.complete_source_name.isEmpty())
            {
                struct_source_info.complete_source_name = get_complete_source_name_from_source(struct_source_info);
            }
            //=========================== Additional Insertion End ===================//

            struct_source_info.configured_ids_QStringList.removeAll("");
            struct_source_info.imei_no_QStringList.removeAll("");
            struct_source_info.phone_no_msisdn_QStringList.removeAll("");
            struct_source_info.sim_no_iccid_QStringList.removeAll("");
            struct_source_info.system_user_QStringList.removeAll("");

            struct_source_info.configured_ids_QStringList.removeDuplicates();
            struct_source_info.imei_no_QStringList.removeDuplicates();
            struct_source_info.phone_no_msisdn_QStringList.removeDuplicates();
            struct_source_info.sim_no_iccid_QStringList.removeDuplicates();
            struct_source_info.system_user_QStringList.removeDuplicates();

            list_witness_info_source.insert(zzz,struct_source_info);

        }
        else
        {
            int count = 0;

            QString source_count = saved_src_cnt_name;
            if(source_count.startsWith(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag))
            {
                source_count.remove(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag);
                bool ok = false;
                int count_val = source_count.toInt(&ok,10);
                if(ok)
                {
                    count = count_val;
                }
            }
            if(count == 0)
            {
                recon_static_functions::app_debug("SOURCE COUNT  ----FAILED--- " + QString::number(count),Q_FUNC_INFO);
                continue;
            }

            QString source_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + saved_src_cnt_name + ".txt";

            QFile file(source_file_path);
            if(!file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("file open ----FAILED---" + source_file_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("Error--- " + file.errorString(),Q_FUNC_INFO);
                continue;
            }


            bool bool_supportive_source_found = false;
            struct_GLOBAL_witness_info_source struct_source_info;
            recon_static_functions::clear_variables_witness_info_source_structure(struct_source_info);

            while(!file.atEnd())
            {
                QString file_line = file.readLine();

                file_line.replace("\n","");

                QStringList tmp_list = file_line.split(QString(MACRO_RECON_Splitter_8_equal_hash_equal), Qt::SkipEmptyParts);

                if(tmp_list.size() < 2)
                    continue;

                bool_supportive_source_found = true;


                struct_source_info.source_count_name = saved_src_cnt_name.trimmed();


                if(tmp_list.at(0) == MACRO_NARAD_Root_Count_Name_QString)
                {
                    struct_source_info.root_count_name             = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Count_Name_QString)
                {
                    struct_source_info.source_count_name              = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Type_Display_QString)
                {
                    struct_source_info.source_type_display              = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Type_Internal_QString)
                {
                    struct_source_info.source_type_internal              = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Name_QString)
                {
                    struct_source_info.source_name                  = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Main_Path_QString)
                {
                    struct_source_info.source_main_path           = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Time_Zone_Readable_QString)
                {
                    struct_source_info.timezone_readable_QString       = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Time_Zone_Numeric_qint64)
                {
                    struct_source_info.timezone_numeric_qint64   = tmp_list.at(1).toLongLong();
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_OS_Scheme_Display_QString)
                {
                    struct_source_info.os_scheme_display              = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_OS_Scheme_Internal_QString)
                {
                    struct_source_info.os_scheme_internal              = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_System_Account_QList)
                {
                    QString system_user_str = tmp_list.at(1);

                    struct_source_info.system_account_QList = get_system_account_qlist_for_all_users(system_user_str);
                    struct_source_info.system_user_QStringList = recon_static_functions::get_system_user_list_from_system_account_QList(struct_source_info.system_account_QList , Q_FUNC_INFO);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Product_Type_QString)
                {
                    struct_source_info.product_type                 = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_OS_Version_Extended_QString)
                {
                    struct_source_info.os_version_extended          = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_OS_Version_Full_QString)
                {
                    struct_source_info.os_version_full              = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Country_QString)
                {
                    struct_source_info.country                      = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_City_QString)
                {
                    struct_source_info.city                         = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Latitude_QString)
                {
                    struct_source_info.latitude                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Longitude_QString)
                {
                    struct_source_info.longitude                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Virtual_Source_Path_QString)
                {
                    struct_source_info.virtual_source_path                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Build_Version_QString)
                {
                    struct_source_info.build_version                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Display_Name_QString)
                {
                    struct_source_info.display_name                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Device_Name_QString)
                {
                    struct_source_info.device_name                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Id_No_QString)
                {
                    struct_source_info.device_id                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_GUID_QString)
                {
                    struct_source_info.guid                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Product_Name_QString)
                {
                    struct_source_info.product_name                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Target_Type_QString)
                {
                    struct_source_info.target_type                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Build_Version_QString)
                {
                    struct_source_info.build_version                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_iTune_Version_QString)
                {
                    struct_source_info.itune_version                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Serial_No_QString)
                {
                    struct_source_info.serial_no                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Phone_No_MSISDN_QStringList)
                {
                    struct_source_info.phone_no_msisdn_QStringList                    = tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Imei_QStringList)
                {
                    struct_source_info.imei_no_QStringList                    = tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Sim_No_Iccid_QStringList)
                {
                    struct_source_info.sim_no_iccid_QStringList                    = tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString)
                {
                    struct_source_info.latest_backup_date_readable_QString                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64)
                {
                    struct_source_info.latest_backup_date_numeric_qint64                    = tmp_list.at(1).toLongLong();
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString)
                {
                    struct_source_info.backup_size_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Main_Name_QString)
                {
                    struct_source_info.source_main_name                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Volume_Type_QString)
                {
                    struct_source_info.image_volume_type                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Volume_Offset_QString)
                {
                    struct_source_info.image_volume_offset                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Volume_UUID_QString)
                {
                    struct_source_info.image_volume_uuid                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString)
                {
                    struct_source_info.total_size_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString)
                {
                    struct_source_info.used_space_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString)
                {
                    struct_source_info.free_space_byte_numeric_QString                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_File_System_Type_QString)
                {
                    struct_source_info.file_system_type                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Path_1_QString)
                {
                    struct_source_info.source_path_1                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Path_2_For_Fusion_QString)
                {
                    struct_source_info.source_path_2_for_fusion                    = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == QString(MACRO_NARAD_Source_RECON_Logical_Mode_bool))
                {
                    if(tmp_list.at(1) == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        struct_source_info.bool_recon_logical_image   =  true;
                    else
                        struct_source_info.bool_recon_logical_image   =  false;
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Configured_ID_QStringList)
                {
                    struct_source_info.configured_ids_QStringList        = QString(tmp_list.at(1)).split(MACRO_RECON_Splitter_6_comma);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Disk_Identifier_QString)
                {
                    struct_source_info.source_disk_identifier  = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Role_Type_QString)
                {
                    struct_source_info.role_type   = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == QString(MACRO_NARAD_Source_Nested_bool))
                {
                    if(tmp_list.at(1) == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        struct_source_info.bool_source_nested   =  true;
                    else
                        struct_source_info.bool_source_nested   =  false;
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Hierarchy_QString)
                {
                    struct_source_info.source_hierarchy        = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == QString(MACRO_NARAD_Source_Decompressed_bool))
                {
                    if(tmp_list.at(1) == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                        struct_source_info.bool_source_decompressed   =  true;
                    else
                        struct_source_info.bool_source_decompressed   =  false;
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64)
                {
                    struct_source_info.os_installer_timestamp_utc_numeric_qint64         = tmp_list.at(1).toLongLong();
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Product_Model_QString)
                {
                    struct_source_info.product_model         = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Bluetooth_Device_Address_QString)
                {
                    struct_source_info.bluetooth_device_address         = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Wifi_Address_QString)
                {
                    struct_source_info.wifi_address         = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Locale_Language_QString)
                {
                    struct_source_info.locale_language         = tmp_list.at(1);
                }
                else if(tmp_list.at(0) == MACRO_NARAD_Source_Evidence_Password_Encrypted_QString)
                {
                    struct_source_info.source_evidence_password_encrypted         = tmp_list.at(1);
                    struct_source_info.source_evidence_password_plain         = recon_static_functions::decrypt_string(struct_source_info.source_evidence_password_encrypted,Q_FUNC_INFO);
                    struct_source_info.source_evidence_password_modified_symbol_hex     = recon_static_functions::get_modified_password_replace_symbol_with_hex(struct_source_info.source_evidence_password_plain,Q_FUNC_INFO);
                }
            }

            file.close();

            if(!bool_supportive_source_found)
                continue;


            //=========================== Additional Insertion Start ===================//
            // donot insert value in upper while loop
            // bcoz we dont save these value in text file

            update_source_structure_for_root_info(struct_source_info);


            if(struct_source_info.snapshots_QList.isEmpty())
                struct_source_info.snapshots_QList = get_apfs_snapshots_QList_for_source_from_snapshots_plist(struct_source_info.source_count_name);


            struct_source_info.fs_extraction_source_path                = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj, struct_source_info.source_type_internal, struct_source_info.virtual_source_path, Q_FUNC_INFO);

            if(struct_source_info.bool_source_nested)
            {
                QStringList src_hir_list = struct_source_info.source_hierarchy.split(MACRO_RECON_Splitter_4_forward);
                if(src_hir_list.size() > 1)
                    struct_source_info.parent_source_count_name = src_hir_list.at(src_hir_list.size() - 1);
            }

            struct_source_info.complete_source_name                = get_complete_source_name_from_source(struct_source_info);
            //=========================== Additional Insertion End ===================//

            struct_source_info.configured_ids_QStringList.removeAll("");
            struct_source_info.imei_no_QStringList.removeAll("");
            struct_source_info.phone_no_msisdn_QStringList.removeAll("");
            struct_source_info.sim_no_iccid_QStringList.removeAll("");
            struct_source_info.system_user_QStringList.removeAll("");

            struct_source_info.configured_ids_QStringList.removeDuplicates();
            struct_source_info.imei_no_QStringList.removeDuplicates();
            struct_source_info.phone_no_msisdn_QStringList.removeDuplicates();
            struct_source_info.sim_no_iccid_QStringList.removeDuplicates();
            struct_source_info.system_user_QStringList.removeDuplicates();

            list_witness_info_source.append(struct_source_info);

        }

    }


    recon_static_functions::app_debug(" Ends ---- ",Q_FUNC_INFO);
}


QList<struct_GLOBAL_snapshots_info> witness_info_manager::get_apfs_snapshots_QList_for_source(QString source_count_name, QSqlDatabase& sources_db)
{
    source_count_name = source_count_name.trimmed();
    QList<struct_GLOBAL_snapshots_info> apfs_snapshots_list;

    if(source_count_name.isEmpty())
        return apfs_snapshots_list;


    if(!sources_db.tables().contains("snapshots"))
        return apfs_snapshots_list;

    QSqlQuery select_query(sources_db);

    select_query.prepare(QString("Select snapshot_name,fs_run_status From snapshots Where source_count_name = ?"));
    select_query.addBindValue(source_count_name);

    if(!select_query.exec())
    {
        recon_static_functions::app_debug("select query -----FAILED----:"+ select_query.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:"+ select_query.lastError().text(), Q_FUNC_INFO);
        return apfs_snapshots_list;
    }

    while(select_query.next())
    {
        struct_GLOBAL_snapshots_info snapshots_info_obj;
        snapshots_info_obj.fs_run_status.clear();

        snapshots_info_obj.snapshot_actual_name = select_query.value("snapshot_name").toString().trimmed();
        snapshots_info_obj.fs_run_status = select_query.value("fs_run_status").toString().trimmed();
        snapshots_info_obj.source_count_name = source_count_name;

        apfs_snapshots_list.append(snapshots_info_obj);
    }

    return apfs_snapshots_list;
}

QList<struct_GLOBAL_witness_info_source> witness_info_manager::pub_get_source_structure_QList()
{
    return list_witness_info_source;
}

void witness_info_manager::pub_set_updated_source_structure_QList(QList<struct_GLOBAL_witness_info_source> list)
{
    list_witness_info_source = list;
}


struct_GLOBAL_witness_info_root witness_info_manager::pub_get_root_structure_according_root_count_name(QString root_count_name)
{
    struct_GLOBAL_witness_info_root struct_root_info;

    for(int num = 0; num < list_witness_info_root.size(); num++)
    {
        if(list_witness_info_root.at(num).root_count_name == root_count_name){
            struct_root_info = list_witness_info_root.at(num);
            break;
        }
    }

    return struct_root_info;
}

struct_GLOBAL_witness_info_root witness_info_manager::pub_get_root_structure_according_source_count_name(QString source_count_name)
{

    struct_GLOBAL_witness_info_source struct_src_info = pub_get_source_structure_according_source_count_name(source_count_name);

    struct_GLOBAL_witness_info_root struct_root_info = pub_get_root_structure_according_root_count_name(struct_src_info.root_count_name);

    return struct_root_info;

}


QList<struct_GLOBAL_witness_info_root> witness_info_manager::pub_get_root_structure_QList()
{

    return list_witness_info_root;
}


struct_GLOBAL_witness_info_source witness_info_manager::pub_get_source_structure_according_source_count_name(QString source_count_name)
{
    struct_GLOBAL_witness_info_source struct_source_info;

    bool bool_source_found = false;
    for(int num = 0; num < list_witness_info_source.size(); num++)
    {
        if(list_witness_info_source.at(num).source_count_name == source_count_name){
            struct_source_info = list_witness_info_source.at(num);
            bool_source_found = true;
            break;
        }
    }

    return struct_source_info;
}


QString witness_info_manager::get_complete_source_name_from_source(struct_GLOBAL_witness_info_source target_sources_information)
{
    QString first_name = target_sources_information.source_main_name;
    QString last_name = target_sources_information.source_name;

    if(first_name.isEmpty() || last_name.isEmpty())
        return "";

    struct_GLOBAL_witness_info_source prnt_source_obj = pub_get_source_structure_according_source_count_name(target_sources_information.parent_source_count_name);

    QString complete_source_name;
    if(target_sources_information.bool_source_nested)
    {
        if(target_sources_information.bool_source_decompressed)
        {
            if(target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
            {

                QString crnt_cmplt_nm = recon_static_functions::get_complete_file_path(target_sources_information.source_main_path, last_name, Q_FUNC_INFO);

                complete_source_name = recon_static_functions::get_complete_file_path(prnt_source_obj.complete_source_name, crnt_cmplt_nm, Q_FUNC_INFO);
            }
            else if(target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
            {
                complete_source_name = recon_static_functions::get_complete_file_path(prnt_source_obj.complete_source_name, target_sources_information.source_main_path, Q_FUNC_INFO);
            }
        }
        else
        {
            if(target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
            {
                QString crnt_cmplt_nm = recon_static_functions::get_complete_file_path((QString("/") + first_name), last_name, Q_FUNC_INFO);
                complete_source_name = recon_static_functions::get_complete_file_path(prnt_source_obj.complete_source_name, crnt_cmplt_nm, Q_FUNC_INFO);
            }
            else if(target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
            {
                complete_source_name = recon_static_functions::get_complete_file_path(prnt_source_obj.complete_source_name, first_name, Q_FUNC_INFO);
            }
        }
    }
    else
    {
        if(target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
        {
            complete_source_name = recon_static_functions::get_complete_file_path((QString("/") + first_name), last_name, Q_FUNC_INFO);
        }
        else if(target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal
                || target_sources_information.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
        {
            complete_source_name = recon_static_functions::get_complete_file_path("/", first_name, Q_FUNC_INFO);
        }
    }

    return complete_source_name;
}


QString witness_info_manager::get_string_with_addbindvalues_by_dbreference(QString command, QStringList values, int pos, QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString str_gen;

    QSqlQuery query(destination_db);
    query.prepare(command);
    for(int i = 0; i < values.size(); i++)
    {
        query.addBindValue(values.at(i));
    }

    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return str_gen;
    }

    if(query.next())
    {
        str_gen = query.value(pos).toString();
    }

    str_gen = str_gen.trimmed();

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return str_gen;
}

void witness_info_manager::fill_root_structure_list_from_db()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    list_witness_info_root.clear();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() ;
    QString decrypted_passwd_file_path = result_dir_path + ".Root_File.txt";
    QString decrypted_file_path = result_dir_path + "Root_File.txt";

    QString root_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";



    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(root_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error  " + destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QString select_command = "select root_count_name, evidence_type, root_type, source_type_display, source_type_internal, root_name,"
                             "all_source_count_name, prime_source_count_name, nested_source_count_name, root_disk_identifier_1,"
                             "root_disk_node_1, root_disk_identifier_2, root_disk_node_2, root_path_1, root_path_2, root_password,"
                             "root_md5_hash, root_sha1_hash, home_directory_user_name, evidence_number, description "
                             " from tbl_root_index";



    QSqlQuery select_query(destination_db);
    select_query.prepare(select_command);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" select query  --------FAILED------" + root_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug(" error - " + select_query.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" query - " + select_query.executedQuery(), Q_FUNC_INFO);
        return;
    }

    while(select_query.next())
    {
        struct_GLOBAL_witness_info_root st_root_info;
        recon_static_functions::clear_variables_witness_info_root_structure(st_root_info);

        st_root_info.root_count_name = select_query.value("root_count_name").toString();
        st_root_info.evidence_type = select_query.value("evidence_type").toString();
        st_root_info.root_type = select_query.value("root_type").toString();
        st_root_info.source_type_display = select_query.value("source_type_display").toString();
        st_root_info.source_type_internal = select_query.value("source_type_internal").toString();
        st_root_info.root_name = select_query.value("root_name").toString();

        st_root_info.source_count_name_all_QStringList     = select_query.value("all_source_count_name").toString().split(MACRO_RECON_Splitter_6_comma);
        st_root_info.source_count_name_prime_QStringList   = select_query.value("prime_source_count_name").toString().split(MACRO_RECON_Splitter_6_comma);
        st_root_info.source_count_name_nested_QStringList  = select_query.value("nested_source_count_name").toString().split(MACRO_RECON_Splitter_6_comma);

        st_root_info.root_path_1 = select_query.value("root_path_1").toString();
        st_root_info.root_path_2_for_fusion = select_query.value("root_path_2").toString();

        st_root_info.root_disk_identifier_1 = select_query.value("root_disk_identifier_1").toString();
        st_root_info.root_disk_node_1 = select_query.value("root_disk_node_1").toString();
        st_root_info.root_disk_identifier_2_for_fusion = select_query.value("root_disk_identifier_2").toString();
        st_root_info.root_disk_node_2_for_fusion = select_query.value("root_disk_node_2").toString();

        st_root_info.root_recovery_passcode_encrypted = select_query.value("root_recovery_passcode").toString();
        st_root_info.root_recovery_passcode_plain = recon_static_functions::decrypt_string(st_root_info.root_recovery_passcode_encrypted , Q_FUNC_INFO);
        st_root_info.root_evidence_password_encrypted = select_query.value("root_evidence_password").toString();
        st_root_info.root_evidence_password_plain = recon_static_functions::decrypt_string(st_root_info.root_evidence_password_encrypted , Q_FUNC_INFO);

        //        if(!st_root_info.root_password_encrypted.isEmpty())
        //        {
        //            QString root_password_file_path = QString(MACRO_RECON_Temp_DirPath_In_Bundle) + "root_file.txt";
        //            QString decrypted_password = recon_static_functions::get_plain_password_from_encrypted_password(root_password_file_path,st_root_info.root_password_encrypted,Q_FUNC_INFO);
        //            st_root_info.root_password_plain = decrypted_password;
        //        }

        st_root_info.root_hash_md5 = select_query.value("root_md5_hash").toString();
        st_root_info.root_hash_sha1 = select_query.value("root_sha1_hash").toString();

        QString evidence_number_by_examiner = select_query.value("evidence_number").toString();
        st_root_info.evidence_number = evidence_number_by_examiner.replace(MACRO_RECON_Splitter_7_at_rate_in_brace , "\n");

        QString description_by_examiner = select_query.value("description").toString();
        st_root_info.description = description_by_examiner.replace(MACRO_RECON_Splitter_7_at_rate_in_brace , "\n");

        st_root_info.home_directory_user_name = select_query.value("home_directory_user_name").toString();

        list_witness_info_root.append(st_root_info);
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void witness_info_manager::fill_root_structure_list_from_text_file()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    QString root_index_text_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_index.txt";
    QFile root_index_file(root_index_text_file_path);
    if(!root_index_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ----FAILED---" + root_index_text_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + root_index_file.errorString(),Q_FUNC_INFO);
        return;
    }

    QStringList root_file_name_list;
    while(!root_index_file.atEnd())
        root_file_name_list << QString::fromLocal8Bit(root_index_file.readLine()).trimmed();

    root_index_file.close();

    for(int pp = 0; pp < root_file_name_list.size(); pp++)
    {
        QString saved_root_cnt_name = root_file_name_list.at(pp);
        if(saved_root_cnt_name.endsWith(QString(".txt")))
            saved_root_cnt_name.chop(QString(".txt").size());

        saved_root_cnt_name = saved_root_cnt_name.trimmed();

        bool bool_source_already_added = false;

        int source_pos = 0;
        for(int i = 0; i < list_witness_info_root.size(); i++)
        {
            struct_GLOBAL_witness_info_root struct_root_info = list_witness_info_root.at(i);
            QString root_file_name = struct_root_info.root_count_name;
            if(root_file_name == saved_root_cnt_name)
            {
                bool_source_already_added = true;
                source_pos = i;
                break;
            }
        }

        if(bool_source_already_added)
        {

            int zzz = source_pos;


            struct_GLOBAL_witness_info_root struct_root_info = list_witness_info_root.takeAt(zzz);

            QString root_file_name = struct_root_info.root_count_name;

            QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + root_file_name + ".txt";

            QFile root_file(root_file_path);
            if(!root_file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("file open ----FAILED---" + root_file_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("Error--- " + root_file.errorString(),Q_FUNC_INFO);
                list_witness_info_root.insert(zzz,struct_root_info);
                continue;
            }

            while(!root_file.atEnd())
            {
                QString file_line = root_file.readLine();

                file_line.replace("\n","");

                QStringList tmp_list = file_line.split(QString(MACRO_RECON_Splitter_8_equal_hash_equal), Qt::SkipEmptyParts);

                if(tmp_list.size() < 2)
                    continue;

                if((tmp_list.at(0) == MACRO_NARAD_Root_Count_Name_QString) && (struct_root_info.root_count_name.isEmpty()))
                {
                    struct_root_info.root_count_name              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Evidence_Type_QString) && (struct_root_info.evidence_type.isEmpty()))
                {
                    struct_root_info.evidence_type              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Type_QString) && (struct_root_info.root_type.isEmpty()))
                {
                    struct_root_info.root_type                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Type_Display_QString) && (struct_root_info.source_type_display.isEmpty()))
                {
                    struct_root_info.source_type_display                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Type_Internal_QString) && (struct_root_info.source_type_internal.isEmpty()))
                {
                    struct_root_info.source_type_internal                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Name_QString) && (struct_root_info.root_name.isEmpty()))
                {
                    struct_root_info.root_name                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Path_1_QString) && (struct_root_info.root_path_1.isEmpty()))
                {
                    struct_root_info.root_path_1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Path_2_for_fusion_QString) && (struct_root_info.root_path_2_for_fusion.isEmpty()))
                {
                    struct_root_info.root_path_2_for_fusion                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString) && (struct_root_info.root_recovery_passcode_encrypted.isEmpty()))
                {
                    struct_root_info.root_recovery_passcode_encrypted                  = tmp_list.at(1);
                    struct_root_info.root_recovery_passcode_plain                      = recon_static_functions::decrypt_string(struct_root_info.root_recovery_passcode_encrypted , Q_FUNC_INFO);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Evidence_Password_Encrypted_QString) && (struct_root_info.root_evidence_password_encrypted.isEmpty()))
                {
                    struct_root_info.root_evidence_password_encrypted                  = tmp_list.at(1);
                    struct_root_info.root_evidence_password_plain                      = recon_static_functions::decrypt_string(struct_root_info.root_evidence_password_encrypted , Q_FUNC_INFO);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Hash_MD5_QString) && (struct_root_info.root_hash_md5.isEmpty()))
                {
                    struct_root_info.root_hash_md5                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Hash_SHA1_QString) && (struct_root_info.root_hash_sha1.isEmpty()))
                {
                    struct_root_info.root_hash_sha1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Count_Name_All_QStringList) && (struct_root_info.source_count_name_all_QStringList.isEmpty()))
                {
                    struct_root_info.source_count_name_all_QStringList                << tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList) && (struct_root_info.source_count_name_prime_QStringList.isEmpty()))
                {
                    struct_root_info.source_count_name_prime_QStringList                << tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList) && (struct_root_info.source_count_name_nested_QStringList.isEmpty()))
                {
                    struct_root_info.source_count_name_nested_QStringList                << tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Identifier_1_QString) && (struct_root_info.root_disk_identifier_1.isEmpty()))
                {
                    struct_root_info.root_disk_identifier_1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Node_1_QString) && (struct_root_info.root_disk_node_1.isEmpty()))
                {
                    struct_root_info.root_disk_node_1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString) && (struct_root_info.root_disk_identifier_2_for_fusion.isEmpty()))
                {
                    struct_root_info.root_disk_identifier_2_for_fusion                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString) && (struct_root_info.root_disk_node_2_for_fusion.isEmpty()))
                {
                    struct_root_info.root_disk_node_2_for_fusion                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Evidence_Number_QString) && (struct_root_info.evidence_number.isEmpty()))
                {
                    QString evidence_by_examiner =  tmp_list.at(1);
                    struct_root_info.evidence_number  = evidence_by_examiner.replace(MACRO_RECON_Splitter_7_at_rate_in_brace , "\n");
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Description_QString) && (struct_root_info.description.isEmpty()))
                {
                    QString description_by_examiner =  tmp_list.at(1);
                    struct_root_info.description  = description_by_examiner.replace(MACRO_RECON_Splitter_7_at_rate_in_brace , "\n");
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString) && (struct_root_info.home_directory_user_name.isEmpty()))
                {
                    struct_root_info.home_directory_user_name             = tmp_list.at(1);
                }
            }

            list_witness_info_root.insert(zzz,struct_root_info);
        }
        else
        {
            QString root_cnt = saved_root_cnt_name;


            QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + saved_root_cnt_name + ".txt";
            QFile root_file(root_file_path);
            if(!root_file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("file open ----FAILED---" + root_file_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("Error--- " + root_file.errorString(),Q_FUNC_INFO);
                continue;
            }

            struct_GLOBAL_witness_info_root struct_root_info;
            recon_static_functions::clear_variables_witness_info_root_structure(struct_root_info);

            while(!root_file.atEnd())
            {
                QString file_line = root_file.readLine();

                file_line.replace("\n","");

                QStringList tmp_list = file_line.split(QString(MACRO_RECON_Splitter_8_equal_hash_equal), Qt::SkipEmptyParts);
                if(tmp_list.size() < 2)
                    continue;

                if((tmp_list.at(0) == MACRO_NARAD_Root_Count_Name_QString) && (struct_root_info.root_count_name.isEmpty()))
                {
                    struct_root_info.root_count_name              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Evidence_Type_QString) && (struct_root_info.evidence_type.isEmpty()))
                {
                    struct_root_info.evidence_type              = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Type_QString) && (struct_root_info.root_type.isEmpty()))
                {
                    struct_root_info.root_type                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Type_Display_QString) && (struct_root_info.source_type_display.isEmpty()))
                {
                    struct_root_info.source_type_display                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Type_Internal_QString) && (struct_root_info.source_type_internal.isEmpty()))
                {
                    struct_root_info.source_type_internal                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Name_QString) && (struct_root_info.root_name.isEmpty()))
                {
                    struct_root_info.root_name                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Path_1_QString) && (struct_root_info.root_path_1.isEmpty()))
                {
                    struct_root_info.root_path_1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Path_2_for_fusion_QString) && (struct_root_info.root_path_2_for_fusion.isEmpty()))
                {
                    struct_root_info.root_path_2_for_fusion                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString) && (struct_root_info.root_recovery_passcode_encrypted.isEmpty()))
                {
                    struct_root_info.root_recovery_passcode_encrypted                  = tmp_list.at(1);
                    struct_root_info.root_recovery_passcode_plain                      = recon_static_functions::decrypt_string(struct_root_info.root_recovery_passcode_encrypted , Q_FUNC_INFO);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Evidence_Password_Encrypted_QString) && (struct_root_info.root_evidence_password_encrypted.isEmpty()))
                {
                    struct_root_info.root_evidence_password_encrypted                  = tmp_list.at(1);
                    struct_root_info.root_evidence_password_plain                      = recon_static_functions::decrypt_string(struct_root_info.root_evidence_password_encrypted , Q_FUNC_INFO);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Hash_MD5_QString) && (struct_root_info.root_hash_md5.isEmpty()))
                {
                    struct_root_info.root_hash_md5                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Hash_SHA1_QString) && (struct_root_info.root_hash_sha1.isEmpty()))
                {
                    struct_root_info.root_hash_sha1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Count_Name_All_QStringList) && (struct_root_info.source_count_name_all_QStringList.isEmpty()))
                {
                    struct_root_info.source_count_name_all_QStringList                << tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList) && (struct_root_info.source_count_name_prime_QStringList.isEmpty()))
                {
                    struct_root_info.source_count_name_prime_QStringList                << tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList) && (struct_root_info.source_count_name_nested_QStringList.isEmpty()))
                {
                    struct_root_info.source_count_name_nested_QStringList                << tmp_list.at(1).split(MACRO_RECON_Splitter_6_comma);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Identifier_1_QString) && (struct_root_info.root_disk_identifier_1.isEmpty()))
                {
                    struct_root_info.root_disk_identifier_1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Node_1_QString) && (struct_root_info.root_disk_node_1.isEmpty()))
                {
                    struct_root_info.root_disk_node_1                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString) && (struct_root_info.root_disk_identifier_2_for_fusion.isEmpty()))
                {
                    struct_root_info.root_disk_identifier_2_for_fusion                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString) && (struct_root_info.root_disk_node_2_for_fusion.isEmpty()))
                {
                    struct_root_info.root_disk_node_2_for_fusion                  = tmp_list.at(1);
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Evidence_Number_QString) && (struct_root_info.evidence_number.isEmpty()))
                {
                    QString evidence_by_examiner =  tmp_list.at(1);
                    struct_root_info.evidence_number  = evidence_by_examiner.replace(MACRO_RECON_Splitter_7_at_rate_in_brace , "\n");
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Description_QString) && (struct_root_info.description.isEmpty()))
                {
                    QString description_by_examiner =  tmp_list.at(1);
                    struct_root_info.description  = description_by_examiner.replace(MACRO_RECON_Splitter_7_at_rate_in_brace , "\n");
                }
                else if((tmp_list.at(0) == MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString) && (struct_root_info.home_directory_user_name.isEmpty()))
                {
                    struct_root_info.home_directory_user_name             = tmp_list.at(1);
                }
            }


            list_witness_info_root.append(struct_root_info);
        }

    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void witness_info_manager::pub_set_updated_root_structure_QList(QList<struct_GLOBAL_witness_info_root> list)
{
    list_witness_info_root = list;
}


void witness_info_manager::update_source_structure_for_root_info(struct_GLOBAL_witness_info_source &struct_src_info)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //this function updates only those values of ROOT in source structure which is not available in source database and text files

    struct_GLOBAL_witness_info_root st_root_info = pub_get_root_structure_according_root_count_name(struct_src_info.root_count_name);


    struct_src_info.evidence_number = st_root_info.evidence_number;
    struct_src_info.description = st_root_info.description;

    struct_src_info.evidence_type = st_root_info.evidence_type;
    struct_src_info.root_type = st_root_info.root_type;
    struct_src_info.root_name = st_root_info.root_name;

    struct_src_info.root_path_1 = st_root_info.root_path_1;
    struct_src_info.root_path_2_for_fusion = st_root_info.root_path_2_for_fusion;

    struct_src_info.root_recovery_passcode_plain = st_root_info.root_recovery_passcode_plain;

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}



QList<struct_global_system_account_info> witness_info_manager::get_system_account_qlist_for_all_users(QString system_user_str)
{

    QList <struct_global_system_account_info> sys_acc_QList;

    if(system_user_str.trimmed().isEmpty())
        return sys_acc_QList;


    QStringList each_account_list = system_user_str.split(MACRO_RECON_Splitter_5_resolution_scope, Qt::SkipEmptyParts);

    for(int pp = 0; pp < each_account_list.size(); pp++)
    {
        QString tmp_str = each_account_list.at(pp);

        QStringList tmp_list = tmp_str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon);

        QString sys_user_str = tmp_list.at(0);
        QString dom_usr_str = tmp_list.at(1);

        sys_user_str.remove(MACRO_System_Account_List_Prefix_SYSTEM_USER);
        dom_usr_str.remove( MACRO_System_Account_List_Prefix_DOMAIN_USER);

        sys_user_str = sys_user_str.trimmed();
        dom_usr_str  = dom_usr_str.trimmed();

        if(sys_user_str.startsWith("'"))
            sys_user_str.remove(0,1);

        if(sys_user_str.endsWith("'"))
            sys_user_str.chop(1);

        if(dom_usr_str.startsWith("'"))
            dom_usr_str.remove(0,1);

        if(dom_usr_str.endsWith("'"))
            dom_usr_str.chop(1);

        struct_global_system_account_info mobj;
        recon_static_functions::clear_variables_system_account_info(mobj);

        mobj.system_user = sys_user_str;
        mobj.domain_user = dom_usr_str;

        sys_acc_QList.append(mobj);
    }

    return sys_acc_QList;

}
