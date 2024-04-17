#include "image_mounter.h"


int image_mounter::mount_vault_images()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString file_type = struct_image_info_obj.image_format_type;

    int value = 0;
    if(file_type == MACRO_Target_Disk_ImageType_Apple)
    {
        QFile file(struct_image_info_obj.image_path_1);
        if(!file.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("checking for image is of created by diskutility  or not ---FAILED---",Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1003_Open_Vault_Image_Failed) + "]");
            //  message_dialog_object->show();
            return -1;
        }
        QString hash = file.readLine(6);
        file.close();
        if(hash.trimmed() == "encrc")
        {
            // image which is created by diskutility
            value = mount_vault_dmg_diskutil_encrypted();
        }
        else
        {
            // image of OS (volume is encrypted)
            value = mount_vault_dmg_volume_encrypted();
        }
    }
    else if(file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL)
    {
        value = mount_vault_encase();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_RAW_FULL)
    {
        value = mount_vault_raw_full();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_RAW_SPLIT)
    {
        value = mount_vault_raw_split();
    }

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_vault_dmg_diskutil_encrypted()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QString output = extract_encrypted_disk();
    if(output == "-A")
        return -1;

    int value = mount_forensic_after_hdiutil(output);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}
int image_mounter::mount_vault_dmg_volume_encrypted()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    // hdiutil attach -owners on -nomount -noverify
    QStringList args_hdiutil;
    // args_hdiutil << "attach" << "-owners" << "on" << "-nomount" << "-noverify" << struct_image_info_obj.image_path_1 << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-owners" << "on" << "-nomount" << "-noverify" << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    int value = mount_vault_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_vault_encase()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_ewfmount,args_fuse);
    if(value == -1)
        return value;

    // attach -readonly -nomount -noautofsck -noverify -imagekey diskimage-class=CRawDiskImage (img_obj.osx_fuse_mount_point + "/ewf1")  -shadow  shadow_file_path;
    QStringList args_hdiutil;
    // args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" << "-noverify" << "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/ewf1") << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" << "-noverify" << "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/ewf1");

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_vault_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}

int image_mounter::mount_vault_raw_full()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    // hdiutil attach -owners on -nomount -noverify -imagekey diskimage-class=CRawDiskImage
    QStringList args_hdiutil;
    // args_hdiutil << "attach" << "-owners" << "on" <<  "-nomount" << "-noverify" << "-imagekey" <<   "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_1 << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-owners" << "on" <<  "-nomount" << "-noverify" << "-imagekey" <<   "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    int value = mount_vault_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_vault_raw_split()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_smrawmount,args_fuse);
    if(value == -1)
        return value;

    QStringList args_hdiutil;
    //args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/raw1") << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/smraw1");

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_vault_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}



QString image_mounter::extract_corestorage_disk(QString uuid)
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    //   QString disk_str = QString("printf '%s\0' 'osslabs' | diskutil corestorage unlockVolume 4C6066A9-6E8D-4A5A-A8CD-378D8BEF8C6E -stdinpassphrase");
    //Do not use simplified password function.
    //QString disk_str = QString("printf '%s' '") + recon_static_functions::pub_get_simplified_password_for_shell_script(struct_image_info_obj.password, Q_FUNC_INFO) + "' | diskutil corestorage unlockVolume " + uuid + " -stdinpassphrase";
    QString disk_str = QString("printf '%s' '") + struct_image_info_obj.password + "' | diskutil corestorage unlockVolume " + uuid + " -stdinpassphrase";

    //    QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
    //    QString script_file = MACRO_RECON_Temp_DirPath_In_Bundle + available_file;

    QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString script_file = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;

    qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_file,disk_str, Q_FUNC_INFO);
    if(byte_size == -1)
    {
        recon_static_functions::app_debug(" command not filled in file   ---FAILED--- command " + disk_str, Q_FUNC_INFO);
        recon_static_functions::app_debug(" file " + script_file, Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1015_Corestorage_Script_Creation_Failed) + "]");
        //message_dialog_object->show();
        recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

        return "-A";
    }


    // QString out_put = run_command_with_argument("sh",QStringList(script_file));
    QString out_put = run_command_with_argument_mergechannel("sh",QStringList(script_file));

    recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

    QString line;
    if(out_put.contains("Volume successfully unlocked"))
    {
        //Started CoreStorage operation
        // Logical Volume successfully unlocked
        // Logical Volume successfully attached as disk5
        // Core Storage disk: disk5
        // Finished CoreStorage operation

        QString chk_1 = "Logical Volume successfully unlocked";
        QString chk_2 = "Logical Volume successfully attached as";
        if(!out_put.contains(chk_1) || !out_put.contains(chk_2))
        {
            recon_static_functions::app_debug(" Core Storage disk not found  ---FAILED--- " + out_put, Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1016_Corestorage_Volume_Not_Attached1) + "]");
            return "-A";
        }

        line = out_put.split(chk_2).at(1).trimmed().split("\n").at(0).trimmed();
    }
    else if(out_put.contains("already unlocked"))
    {
        // D9EF4C2D-5B77-4695-B3C2-150BFDE7FA38 is already unlocked and is attached as disk5


        QString chk_1 = "is already unlocked and is attached as";

        if(!out_put.contains(chk_1))
        {
            recon_static_functions::app_debug(" Core Storage disk not found  ---FAILED--- " + out_put, Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1017_Corestorage_Volume_Not_Attached2) + "]");
            return "-A";
        }

        QStringList tmp_split_list = out_put.split(chk_1);
        if(tmp_split_list.size() > 1)
        {
            line = tmp_split_list.at(1);
            line = line.trimmed();
            QString spc_val = " ";
            if(line.contains(spc_val))
            {
                line.remove(line.indexOf(spc_val), line.size());
                line = line.trimmed();
            }
        }
    }
    else
    {
        recon_static_functions::app_debug(" Core Storage disk not found  ---FAILED--- " + out_put, Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1018_Core_Storage_Disk_Not_Found) + "]");
        return "-A";
    }


    recon_static_functions::app_debug(" end " + line,Q_FUNC_INFO);
    return line.trimmed(); // disk5

}

QString image_mounter::extract_apfs_disk(QString disk_node)
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    //   QString disk_str = QString("printf '%s\0' 'apple123' | diskutil apfs unlockVolume /dev/disk6s1  -nomount -stdinpassphrase");
    //Do not use simplified password function.
    //QString disk_str = QString("printf '%s' '" + recon_static_functions::pub_get_simplified_password_for_shell_script(struct_image_info_obj.password, Q_FUNC_INFO) + "' |  diskutil apfs unlockVolume " + disk_node + " -nomount -stdinpassphrase");
    QString disk_str = QString("printf '%s' '" + struct_image_info_obj.password + "' |  diskutil apfs unlockVolume " + disk_node + " -nomount -stdinpassphrase");

    //    QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
    //    QString script_file = MACRO_RECON_Temp_DirPath_In_Bundle + available_file;

    QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString script_file = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;

    qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_file,disk_str, Q_FUNC_INFO);
    if(byte_size == -1)
    {
        recon_static_functions::app_debug(" command not filled in file   ---FAILED--- command " + disk_str, Q_FUNC_INFO);
        recon_static_functions::app_debug(" file " + script_file, Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1009_Apfs_Script_Creation_Failed) + "]");
        //message_dialog_object->show();
        recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

        return "-A";
    }

    // QString out_put = run_command_with_argument("sh",QStringList(script_file));
    QString out_put = run_command_with_argument_mergechannel("sh",QStringList(script_file));
    recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);



    //Unlocking any cryptographic user on APFS Volume disk6s2
    //Unlocked but did not mount APFS Volume
    //    if(!out_put.contains("Unlocking any cryptographic user on APFS Volume") || !out_put.contains("Unlocked but did not mount APFS Volume"))
    //    {
    //        recon_static_functions::app_debug(" APFS disk not found  ---FAILED--- " + out_put, Q_FUNC_INFO);
    //        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_110) + "]");
    //        //message_dialog_object->show();
    //        recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

    //        return "-A";
    //    }

    //    QString line = out_put.split("Unlocking any cryptographic user on APFS Volume ").at(1).trimmed().split("\n").at(0).trimmed();

    //    recon_static_functions::app_debug(" end " + line,Q_FUNC_INFO);
    //    return line.trimmed();

    //    Unlocking any cryptographic user on APFS Volume disk7s1
    //    Error unlocking APFS Volume: The given APFS Volume is not locked (-69589)


    //Unlocking any cryptographic user on APFS Volume disk6s2
    //Unlocked but did not mount APFS Volume

    QString line;
    QString spt1 = "Unlocking any cryptographic user on APFS Volume";
    QString spt2 = "Unlocked but did not mount APFS Volume";
    QString spt3 = "Error unlocking APFS Volume: The given APFS Volume is not locked";
    if(out_put.contains(spt1) && out_put.contains(spt2))
    {
        line = out_put.split(spt1).at(1).trimmed().split("\n").at(0).trimmed();
    }
    else if(out_put.contains(spt1) && out_put.contains(spt3))
    {
        line = out_put.split(spt1).at(1).trimmed().split("\n").at(0).trimmed();
    }
    else
    {
        recon_static_functions::app_debug(" APFS disk not found  ---FAILED--- " + out_put, Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1010_Apfs_Disk_Not_Found) + "]");
        //message_dialog_object->show();
        recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

        return "-A";
    }


    recon_static_functions::app_debug(" end " + line,Q_FUNC_INFO);
    return line.trimmed();


}

QString image_mounter::extract_encrypted_disk()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    // printf '%s\0' 'osslabs' | hdiutil attach -owners on -nomount -noverify path -stdinpass -shadow shadowpath

    QString image_path = struct_image_info_obj.image_path_1;
    //QString disk_str = QString("printf '%s' '") + struct_image_info_obj.password + "' | hdiutil attach -owners on -nomount -noverify " + image_path.replace(" ","\\ ") + " -stdinpass -shadow " + struct_image_info_obj.shadow_file_path_1;
    //Do not use simplified password function.
    //QString disk_str = QString("printf '%s' '") +  recon_static_functions::pub_get_simplified_password_for_shell_script(struct_image_info_obj.password, Q_FUNC_INFO)  + "' | hdiutil attach -owners on -nomount -noverify " + image_path.replace(" ","\\ ") + " -stdinpass";
    QString disk_str = QString("printf '%s' '") + struct_image_info_obj.password + "' | hdiutil attach -owners on -nomount -noverify " + image_path.replace(" ","\\ ") + " -stdinpass";

    //    QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
    //    QString script_file = MACRO_RECON_Temp_DirPath_In_Bundle + available_file;

    QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    QString script_file = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;

    qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_file,disk_str, Q_FUNC_INFO);
    if(byte_size == -1)
    {
        recon_static_functions::app_debug(" command not filled in file   ---FAILED--- command " + disk_str, Q_FUNC_INFO);
        recon_static_functions::app_debug(" file " + script_file, Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1014_Encryption_Script_Creation_Failed2) + "]");
        //message_dialog_object->show();
        recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

        return "-A";
    }

    QString out_put = run_command_with_argument("sh",QStringList(script_file));
    recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);
    // out_put = check_for_fdisk_mounting_scheme(out_put);

    if(out_put.contains("FDisk_partition_scheme"))// Looks like a device with 4096 block size
        {
        // QString disk_str = QString("printf '%s' '") + struct_image_info_obj.password + "' | hdiutil attach -owners on -nomount -noverify " + image_path.replace(" ","\\ ") + " -stdinpass -shadow " + struct_image_info_obj.shadow_file_path_1 + " -blocksize 4096";
        //Do not use simplified password function.
        //QString disk_str = QString("printf '%s' '") + recon_static_functions::pub_get_simplified_password_for_shell_script(struct_image_info_obj.password, Q_FUNC_INFO)  + "' | hdiutil attach -owners on -nomount -noverify " + image_path.replace(" ","\\ ") + " -stdinpass -blocksize 4096";
        QString disk_str = QString("printf '%s' '") + struct_image_info_obj.password + "' | hdiutil attach -owners on -nomount -noverify " + image_path.replace(" ","\\ ") + " -stdinpass -blocksize 4096";

        //        QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
        //        QString script_file = MACRO_RECON_Temp_DirPath_In_Bundle + available_file;

        QString available_file = recon_static_functions::get_available_filename("script_unlockvolume",global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
        QString script_file = global_variable_private_var_tmp_Dir_Path_Case_QString + available_file;

        qint64 byte_size = recon_static_functions::overwrite_data_into_file(script_file,disk_str, Q_FUNC_INFO);
        if(byte_size == -1)
        {
            recon_static_functions::app_debug(" command not filled in file   ---FAILED--- command " + disk_str, Q_FUNC_INFO);
            recon_static_functions::app_debug(" file " + script_file, Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1008_Encryption_Script_Creation_Failed1) + "]");
            //message_dialog_object->show();
            recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

            return "-A";
        }

        out_put = run_command_with_argument("sh",QStringList(script_file));
        recon_static_functions::remove_safely_blank_dir_OR_file(script_file, Q_FUNC_INFO);

    }

    recon_static_functions::app_debug(" end " + out_put,Q_FUNC_INFO);
    return out_put.trimmed();
}

int image_mounter::mount_vault_after_hdiutil(QString out_hdiutil)
{
    recon_static_functions::app_debug(" start out_hdiutil output - " + out_hdiutil,Q_FUNC_INFO);

    struct_global_image_mounter_image_info obj = extract_disk_info(out_hdiutil);

    struct_image_info_obj.disk_identifier_1 = obj.disk_identifier_1;
    struct_image_info_obj.disk_node_1 = obj.disk_node_1;


    QStringList list_encrypted_identifiers = get_encrypted_apfs_volume_disk_node();


    bool bool_cs_drive = false;
    bool bool_apfs_container = false;
    QString first_cs_uuid;
    for(int tt = 0; tt < obj.list_struct_partition_info.size(); tt++)
    {

        if(obj.list_struct_partition_info.at(tt).parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive))
        {
            if(obj.list_struct_partition_info.at(tt).derived_into_node.trimmed().isEmpty())
            {
                bool_cs_drive = true;
                first_cs_uuid = obj.list_struct_partition_info.at(tt).derived_into_uuid.trimmed();
                break;
            }
        }
        else if(obj.list_struct_partition_info.at(tt).parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
        {

            if(!list_encrypted_identifiers.contains(obj.list_struct_partition_info.at(tt).derived_into_identifier))
                continue;

            bool_apfs_container = true;

            //  this is only for single volume, we are not supporting multiple volume in vault
            QString core_disk_name = extract_apfs_disk(obj.list_struct_partition_info.at(tt).derived_into_node);



            //If decryption not succsessfully so we not move to the others partitions the image have
            // thats why use return -1 it means mounting failed
            if(core_disk_name == "-A")
                return -1;

        }

    }


    if(bool_cs_drive)
    {
        if(first_cs_uuid.trimmed().isEmpty())
        {
            recon_static_functions::app_debug(" not found any uuid  ---FAILED---" + first_cs_uuid,Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1007_Vault_UID_Not_Found) + "]");
            //message_dialog_object->show();
            return -1;
        }

        //  this is only for single volume, we are not supporting multiple volume in vault
        QString core_disk_name = extract_corestorage_disk(first_cs_uuid);

        if(core_disk_name == "-A")
            return -1;

        // update structure with  dev node
        for(int tt = 0; tt < obj.list_struct_partition_info.size(); tt++)
        {
            struct_global_image_mounter_partition_info tmp_obj = obj.list_struct_partition_info.at(tt);

            if(tmp_obj.derived_into_uuid.trimmed() == first_cs_uuid)
            {
                /// Parse Volume Name for Derived Node
                tmp_obj.derived_into_name = get_value_of_key_from_diskutil_info_output(core_disk_name, "VolumeName", enum_Global_Plist_File_Key_Type_String);

                tmp_obj.derived_into_identifier = core_disk_name;
                tmp_obj.derived_into_node = core_disk_name.prepend("/dev/");

                obj.list_struct_partition_info.removeAt(tt);
                obj.list_struct_partition_info.insert(tt,tmp_obj);
                break;
            }
        }
    }



    if(obj.list_struct_partition_info.size() < 1)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,  "No mountable Filesystem found on Image");
        //message_dialog_object->show();
        return -1;
    }

    struct_image_info_obj.list_struct_partition_info = obj.list_struct_partition_info;

    recon_static_functions::app_debug(" -----BELOW PRINTED INFO IS BEFORE IMAGE MOUNTING-----", Q_FUNC_INFO);
    QList<struct_global_image_mounter_image_info> tt_list;
    tt_list.append(struct_image_info_obj);
    recon_static_functions::print_image_mounting_info_on_terminal(tt_list, Q_FUNC_INFO);
    recon_static_functions::app_debug(" ------ABOVE PRINTED INFO IS BEFORE IMAGE MOUNTING------", Q_FUNC_INFO);


    QMap<QString, int> mmap = final_mounting(struct_image_info_obj.list_struct_partition_info);
    int value = check_mounting_status(mmap);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}



