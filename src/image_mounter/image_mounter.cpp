#include "image_mounter.h"

image_mounter::image_mounter(QWidget *parent) :
    QDialog(parent)
{

    //================================//
    /// DO NOT Delete
    // /dev/disk2  disk node
    // disk2  disk identifier
    // /dev/disk2s2 volume node
    // disk2s2 volume identifier

    // 0 is for sucess and -1 is for failure
    // '-A' is for failure
    //==================================//

    message_dialog_object = new message_dialog;

    prc_generic_with_argument = new QProcess(this);
    connect(prc_generic_with_argument, SIGNAL(finished(int)), this, SLOT(slot_prc_generic_with_argument_finished(int)));

    prc_generic_with_argument_mergechannel = new QProcess(this);
    prc_generic_with_argument_mergechannel->setProcessChannelMode(QProcess::MergedChannels);
    connect(prc_generic_with_argument_mergechannel, SIGNAL(finished(int)), this, SLOT(slot_prc_generic_with_argument_mergechannel_finished(int)));
    connect(prc_generic_with_argument_mergechannel, SIGNAL(readyRead()), this, SLOT(slot_prc_generic_with_argument_mergechannel_readyread()));

    prc_generic_without_argument = new QProcess(this);
    connect(prc_generic_without_argument, SIGNAL(finished(int)), this, SLOT(slot_prc_generic_without_argument_finished(int)));


    prc_generic_with_timer = new QProcess(this);
    connect(prc_generic_with_timer, SIGNAL(finished(int)), this, SLOT(slot_prc_generic_finished_with_timer(int)));

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timer_timeout()));

    struct_global_desktop_info struct_mchne_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
    os_version_full_running_machine = struct_mchne_info_obj.os_version_full;

    check_for_os_fuse_installation();

    list_struct_image_info.clear();

}

image_mounter::~image_mounter()
{

}

void image_mounter::set_list_image_info(QList<struct_global_image_mounter_image_info> list_info)
{
    list_struct_image_info = list_info;
}

void image_mounter::clear_list_image_info()
{
    list_struct_image_info.clear();
}

void image_mounter::set_image_category(QString image_source_type_display, QString image_source_type_internal)
{
    clear_fields();
    struct_image_info_obj.image_source_type_display = image_source_type_display;
    struct_image_info_obj.image_source_type_internal = image_source_type_internal;
}

void image_mounter::clear_fields()
{
    shadow_files_directory.clear();

    recon_static_functions::clear_variables_image_mounting_image_info(struct_image_info_obj);

    core_storage_and_derived_disk_node_list.clear();
    apfs_container_and_derived_disk_node_list.clear();

}

int image_mounter::set_image_path(QString path1,QString path2,QString password)
{
    QFileInfo info1(path1);
    QFileInfo info2(path2);

    if(!info1.exists())
    {
        recon_static_functions::app_debug(" path 1 not exist ---FAILED--- " + info1.filePath(),Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1012_Image_Path1_Not_Exist) + "]");
        return -1;
    }

    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        if(!info2.exists())
        {
            recon_static_functions::app_debug(" path 2 not exist ---FAILED--- " + info2.filePath(),Q_FUNC_INFO);
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1013_Image_Path2_Not_Exist) + "]");
            return -1;
        }
    }

    QString file_type = recon_static_functions::get_forensic_image_type(info1.filePath());

    if(file_type == MACRO_Target_Disk_ImageType_Unsupported)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Image Mounter supports EWF, SMART, RAW and DMG file formats, the selected one isn't. Please select file of these formats");
        //  message_dialog_object->show();
        return -1;
    }
    else if((file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL || file_type == MACRO_Target_Disk_ImageType_RAW_FULL
             || file_type == MACRO_Target_Disk_ImageType_RAW_SPLIT || file_type == MACRO_Target_Disk_ImageType_ENCASE_LOGICAL
             || file_type == MACRO_Target_Disk_ImageType_AFF4)
            && !bool_osx_fuse_installed)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Please install OSX Fuse to mount this image.");
        //   message_dialog_object->show();

        return -1;
    }

    struct_image_info_obj.image_format_type = file_type;
    struct_image_info_obj.image_name_1 = info1.fileName();
    struct_image_info_obj.image_path_1 = info1.filePath();
    struct_image_info_obj.image_name_2 = info2.fileName();
    struct_image_info_obj.image_path_2 = info2.filePath();
    struct_image_info_obj.password = password;

    if(!create_directory_structure())
    {
        recon_static_functions::app_debug(" mount point creation ---FAILED---",Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1001_Mount_Point_Creation_Failed) + "]");
        return -1;
    }

    // prepare_shadow_file_path();

    emit signal_set_progress_message("Initializing...");

    int value = 0;

    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
    {
        value = mount_forensic_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
    {
        value = mount_vault_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        value = mount_fusion_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
    {
        value = mount_recon_fs_block_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
    {
        value = mount_time_machine_backup_dmg();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
    {
        value = mount_optical_disc_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)
    {
        value = mount_rcn_logical_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
    {
        value = mount_encase_logical_images();
    }
    else if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
    {
        value = mount_recon_mac_sharing_mode_images();
    }



    if(value == -1)
    {
        // removing and unmounting all dependancy
        unmount_image(struct_image_info_obj.image_path_1);

        // we are showing only here bcoz progress bar is settled by image mounter parent.
        // In backgroud progress bar must not be there, if we show message by message box
        // so we are showing only msg box here

        message_dialog_object->show();
        return -1;
    }
    else
    {
        /// This function fill the value of free space, Total space after mounting because we know about free
        /// space after mounting
        fill_partition_info_structure_after_mounting();
    }

    return 0;
}


struct_global_image_mounter_image_info image_mounter::extract_disk_info(QString out_hdiutil)
{
    recon_static_functions::app_debug(" start -->> hdiutil output " + out_hdiutil,Q_FUNC_INFO);

    //=================================================================//
    //  /dev/disk4          	GUID_partition_scheme
    //  /dev/disk4s1        	EFI
    //  /dev/disk4s2        	Apple_HFS    //OR   Apple_CoreStorage
    //  /dev/disk4s3        	Apple_Boot
    //
    // OR
    //
    // /dev/disk5
    //
    // OR
    //
    //  /dev/disk6          	GUID_partition_scheme
    // /dev/disk6s1        	EFI
    // /dev/disk6s2        	Apple_APFS
    // /dev/disk7          	EF57347C-0000-11AA-AA11-0030654
    // /dev/disk7s1        	41504653-0000-11AA-AA11-0030654
    // /dev/disk7s2        	41504653-0000-11AA-AA11-0030654
    // /dev/disk7s3        	41504653-0000-11AA-AA11-0030654
    // /dev/disk7s4        	41504653-0000-11AA-AA11-0030654
    //
    // OR
    //
    //    /dev/disk9
    //    /dev/disk10         	EF57347C-0000-11AA-AA11-0030654
    //    /dev/disk10s1       	41504653-0000-11AA-AA11-0030654

    //=================================================================//

    struct_global_image_mounter_image_info obj;
    recon_static_functions::clear_variables_image_mounting_image_info(obj);

    if(out_hdiutil.contains("temporarily unavailable"))
    {
        recon_static_functions::app_debug(" Resource temporarily unavailable    ---FAILED---",Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Resource temporarily unavailable. May be the image has already been mounted from Finder");
        //message_dialog_object->show();
        return obj;
    }

    if(!out_hdiutil.startsWith("/dev/disk"))
    {
        recon_static_functions::app_debug(" dont start with /dev/disk    ---FAILED---",Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1006_Hdiutil_Output_Desired_Pattern_Not_Found) + "]");
        // message_dialog_object->show();

        return obj;
    }

    QString disk_node = parse_disk_node_from_hdiutil_output(out_hdiutil);
    QString disk_name = extract_name_from_node(disk_node);


    obj.disk_identifier_1 = disk_name;
    obj.disk_node_1 = disk_node;

    recon_static_functions::app_debug("Start corestorage_partition_info ", Q_FUNC_INFO);
    QList <struct_global_image_mounter_partition_info> list_1 = parse_corestorage_partition_info_from_hdiutil_output(out_hdiutil);
    if(list_1.size() >= 1)
    {
        obj.list_struct_partition_info = list_1;
    }

    recon_static_functions::app_debug("Start APFS_partition_info ", Q_FUNC_INFO);

    QList <struct_global_image_mounter_partition_info> list_3 = parse_apfs_disk_partition_info_from_hdiutil_output(out_hdiutil);
    if(list_3.size() >= 1)
    {
        obj.list_struct_partition_info << list_3;
    }

    recon_static_functions::app_debug("Start Container_partition_info ", Q_FUNC_INFO);
    QList <struct_global_image_mounter_partition_info> list_4 = parse_apfs_container_partition_info_from_hdiutil_output(out_hdiutil);
    if(list_4.size() >= 1)
    {
        obj.list_struct_partition_info << list_4;
    }

    recon_static_functions::app_debug("Start Rest_partition_info ", Q_FUNC_INFO);
    QList <struct_global_image_mounter_partition_info> list_2 = parse_other_partition_info_from_hdiutil_output(out_hdiutil);
    if(list_2.size() >= 1)
    {
        obj.list_struct_partition_info << list_2;
    }

    return obj;
}

QStringList image_mounter::get_derived_into_disknode_list(QList <struct_global_image_mounter_partition_info> objlist)
{
    QStringList derived_list;
    for(int zz = 0; zz < objlist.size(); zz++)
    {
        struct_global_image_mounter_partition_info obj = objlist.at(zz);
        if(obj.derived_into_node.trimmed() != "")
        {
            derived_list << obj.derived_into_node;
        }
    }

    return derived_list;
}

QMap<QString, int> image_mounter::final_mounting(QList<struct_global_image_mounter_partition_info> list_image_partition_info)
{
    recon_static_functions::app_debug(" start --",Q_FUNC_INFO);
    recon_static_functions::debug_intensive(" list_image_partition_info size == " + QString::number(list_image_partition_info.size()), Q_FUNC_INFO);

    // progress_bar_obj->set_status_message("Mounting...");
    //  emit signal_statusbar_progress_bar_show_hide(true,"Mounting " + struct_image_info_obj.image_name_1 + "...");

    emit signal_set_progress_message("Mounting " + struct_image_info_obj.image_name_1 + "...");

    QMap<QString, int> mymap;

    if(list_image_partition_info.size() < 1)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "No mountable Filesystem found on Image.");
        message_dialog_object->show();

        mymap.insert("",-1);
        return mymap;
    }

    QStringList mount_path_list;
    for(int i = 0 ; i < list_image_partition_info.size(); i++)
    {
        struct_global_image_mounter_partition_info st_partition_info = list_image_partition_info.takeAt(i);

        st_partition_info.mount_path.clear();
        QString volume_node = st_partition_info.parent_node;
        volume_node = volume_node.trimmed();
        QString volume_identfr = st_partition_info.parent_identifier;
        volume_identfr = volume_identfr.trimmed();

        QString mount_point_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString).toString();

        QString mount_point;
        QString available_file;
        if(st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive) || st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
        {
            volume_identfr = st_partition_info.derived_into_identifier;
            volume_identfr = volume_identfr.trimmed();
            st_partition_info.volume_UUID = get_value_of_key_from_diskutil_info_output(volume_identfr,"VolumeUUID",enum_Global_Plist_File_Key_Type_String);

            QString tt_fl_nm = ("RECON_MNT_" + volume_identfr + "_" + struct_image_info_obj.image_name_1);
            tt_fl_nm.replace(" ","_");
            available_file =  recon_static_functions::get_available_filename(tt_fl_nm,mount_point_path, Q_FUNC_INFO);
        }
        else
        {
            QString tt_fl_nm = ("RECON_MNT_" + volume_identfr + "_" + struct_image_info_obj.image_name_1);
            tt_fl_nm.replace(" ","_");
            available_file =  recon_static_functions::get_available_filename(tt_fl_nm,mount_point_path, Q_FUNC_INFO);
        }

        mount_point = mount_point_path + available_file;

        QDir dir;
        if(!dir.mkpath(mount_point))
        {
            recon_static_functions::app_debug(" mount point creation ---FAILED--- for " + volume_node,Q_FUNC_INFO);
            recon_static_functions::app_debug(" mount point  " + mount_point,Q_FUNC_INFO);
            mymap.insert(volume_node,-2);
            continue;
        }

        QString out_mount;
        QStringList arg_mount;

        arg_mount.clear();
        QString m_fs_type = st_partition_info.fs_type.trimmed();

        if(st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive)
                || st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
        {
            volume_node = st_partition_info.derived_into_node.trimmed();
        }




        if(m_fs_type.contains(MACRO_FILE_SYSTEM_TYPE_HFS, Qt::CaseInsensitive))
        {
            arg_mount << "-j"  << "-o" << "rdonly,noowners" << volume_node << mount_point;
            out_mount = run_command_with_argument("mount_hfs",arg_mount);
        }
        else if(m_fs_type.contains(MACRO_FILE_SYSTEM_TYPE_APFS, Qt::CaseInsensitive))
        {
            arg_mount  << "-o" << "rdonly,noowners" << volume_node << mount_point;
            out_mount = run_command_with_argument("mount_apfs",arg_mount);
        }
        else if(m_fs_type.contains(MACRO_FILE_SYSTEM_TYPE_NTFS, Qt::CaseInsensitive))
        {
            arg_mount  << "-o" << "rdonly,noowners,nobrowse" << volume_node << mount_point;
            out_mount = run_command_with_argument("mount_ntfs",arg_mount);
        }
        else if(m_fs_type.contains(MACRO_FILE_SYSTEM_TYPE_LIFS, Qt::CaseInsensitive))
        {
            arg_mount  << "-o" << "rdonly,noowners,nobrowse" << volume_node << mount_point;
            out_mount = run_command_with_argument("mount_lifs",arg_mount);
        }


        if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(mount_point, Q_FUNC_INFO))
        {
            arg_mount.clear();
            arg_mount << "mount" << "readonly" << "-mountPoint" << mount_point << volume_node;
            out_mount = run_command_with_argument("diskutil",arg_mount);


            if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(mount_point, Q_FUNC_INFO))
            {
                arg_mount.clear();
                arg_mount << "-j"  << "-o" << "rdonly,noowners" << volume_node << mount_point;
                out_mount = run_command_with_argument("mount_hfs",arg_mount);


                if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(mount_point, Q_FUNC_INFO))
                {
                    arg_mount.clear();
                    arg_mount  << "-o" << "rdonly,noowners" << volume_node << mount_point;
                    out_mount = run_command_with_argument("mount_apfs",arg_mount);

                    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(mount_point, Q_FUNC_INFO))
                    {
                        // get snapshot label for mounting
                        QString volume_snaps_info_path = QFileInfo(struct_image_info_obj.image_path_1).absoluteDir().absolutePath() + "/volume_snaps_info.rif";
                        bool bool_file_exist = QFileInfo(volume_snaps_info_path).exists();

                        //check for Big Sur ASR Image
                        if(bool_file_exist)
                        {
                            QString snapshot_label_str = get_snapshot_label_for_big_sur(volume_snaps_info_path);
                            if(!snapshot_label_str.isEmpty())
                            {
                                arg_mount.clear();
                                arg_mount  << "-o" << "rdonly,noowners,nobrowse" << "-s" << snapshot_label_str << volume_node << mount_point;
                                out_mount = run_command_with_argument("mount_apfs",arg_mount);
                            }
                        }
                        else
                        {
                            arg_mount.clear();
                            arg_mount  << "-o" << "rdonly,noowners,nobrowse" << volume_node << mount_point;
                            out_mount = run_command_with_argument("mount_ntfs",arg_mount);
                        }

                    }

                    //                    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(mount_point, Q_FUNC_INFO))
                    //                    {
                    //                        arg_mount.clear();
                    //                        arg_mount  << "-o" << "rdonly,noowners,nobrowse" << volume_node << mount_point;
                    //                        out_mount = run_command_with_argument("mount_ntfs",arg_mount);

                    //                    }
                }
            }
        }


        //mount_hfs: error on mount(): error = -1.
        //mount_hfs: Operation not permitted
        //Volume on disk4 failed to mount; if it has a partitioning scheme, use "diskutil mountDisk"
        //Unable to find disk for /dev/disk4s1
        //Mountpoint  does not exist
        if(out_mount.contains("error on mount") || out_mount.contains("Operation not permitted")
                || out_mount.contains("Mountpoint  does not exist") || out_mount.contains("Unable to find disk")
                || out_mount.contains("failed to mount"))
        {
            mymap.insert(volume_node,-3);
            recon_static_functions::app_debug(" mount_hfs/diskutil output ---FAILED--- for " + volume_node ,Q_FUNC_INFO);
            recon_static_functions::app_debug(" argument list --> " + arg_mount.join(","),Q_FUNC_INFO);
            recon_static_functions::app_debug(" output_mount_hfs/diskutil " + out_mount,Q_FUNC_INFO);
            continue;
        }


        if(global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(mount_point, Q_FUNC_INFO))
        {
            mount_path_list << mount_point;
            //            turn_indexing_ON_OFF("on",mount_point);
            mymap.insert(volume_node,0);
            st_partition_info.mount_path = mount_point;
        }
        else
        {
            recon_static_functions::app_debug(" mount point not exist on  ---FAILED--- for volume_node " + volume_node,Q_FUNC_INFO);
            recon_static_functions::app_debug(" mount point not exist on ---FAILED--- for mount_point " + mount_point,Q_FUNC_INFO);
            mymap.insert(volume_node,-4);
        }

        list_image_partition_info.insert(i,st_partition_info);
    }

    if(mount_path_list.size() <= 0)
    {
        recon_static_functions::app_debug(" mount_path_list size is 0 ---FAILED--- for image " + struct_image_info_obj.image_path_1,Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1002_Mounting_All_Partitions_Failed) + "]");
        //  message_dialog_object->show();

        mymap.insert("",-5);
        // do not return;
    }

    struct_image_info_obj.list_struct_partition_info = list_image_partition_info;
    list_struct_image_info.append(struct_image_info_obj);

    recon_static_functions::app_debug(" end " ,Q_FUNC_INFO);

    return mymap;
}


QString image_mounter::extract_name_from_node(QString disk_node)
{
    // Input : /dev/disk3
    // Output : disk3
    QStringList list = disk_node.split("/");

    if(list.size() < 3)
        return QString("");
    else
        return list.at(2); // disk3

}


QList<struct_global_image_mounter_partition_info> image_mounter::parse_other_partition_info_from_hdiutil_output(QString out_put)
{
    //  /dev/disk4          	GUID_partition_scheme
    //  /dev/disk4s1        	EFI
    //  /dev/disk4s2        	Apple_CoreStorage  //OR Apple_HFS
    //  /dev/disk4s3        	Apple_Boot
    //
    // OR
    //
    // /dev/disk5

    QList <struct_global_image_mounter_partition_info> objlist;

    // this is for volume's image
    if(out_put.trimmed().size() < 13) // as according to old RECON
    {
        struct_global_image_mounter_partition_info obj;
        recon_static_functions::clear_variables_image_mounting_partiton_info(obj);

        obj.parent_node = out_put;
        obj.parent_identifier = out_put.remove("/dev/");

        /// Get Plist file path after running diskutil info -plist identifier_name > plist_file_path
        QString temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(out_put, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

        /// Parse Volume Type for node
        obj.parent_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);

        /// Parse Volume Name for Simple Node
        obj.parent_name = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeName", Q_FUNC_INFO);
        obj.volume_offset = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_Number, "PartitionMapPartitionOffset", Q_FUNC_INFO);
        obj.volume_UUID = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeUUID", Q_FUNC_INFO);
        obj.volume_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);
        obj.fs_type = global_recon_helper_singular_class_obj->get_file_system_of_any_node(out_put.trimmed(),Q_FUNC_INFO);

        obj.role_type = recon_static_functions::get_value_of_key_from_diskutil_apfs_list_plist_volume("", "", "Roles", enum_Global_Plist_File_Key_Type_Array, Q_FUNC_INFO);

        /// Remove diskutil info plist file
        QFile temp_file(temp_plist_file_path);
        if(temp_file.exists())
            temp_file.remove();

        objlist.append(obj);

        return objlist; // /dev/disk5
    }

    QString apfs_list_plist_path, parent_identifier;

    QStringList mlist = out_put.split("\n");
    for(int i = 0; i < mlist.size(); i++)
    {
        struct_global_image_mounter_partition_info obj;
        recon_static_functions::clear_variables_image_mounting_partiton_info(obj);

        QString line = mlist.at(i);
        line = line.trimmed();

        if(line.isEmpty())
            continue;

        QStringList tmp_list = line.split(" ");
        QString node_value = tmp_list.at(0);  // /dev/disk4s1
        node_value = node_value.trimmed();
        if(node_value.isEmpty() || tmp_list.size() <= 1)
            continue;

        QString tmp_node = node_value;
        tmp_node.remove(0,9); // removing /dev/disk
        if(!tmp_node.contains("s"))
            continue;

        if(core_storage_and_derived_disk_node_list.contains(node_value))
            continue;

        if(apfs_container_and_derived_disk_node_list.contains(node_value))
            continue;

        obj.parent_node = node_value;

        QString vol_identifier = node_value.remove("/dev/");
        obj.parent_identifier = vol_identifier;

        /// Get Plist file path after running diskutil info -plist identifier_name > plist_file_path
        QString temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(vol_identifier, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

        /// Parse Volume Type for node
        obj.parent_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);

        /// Parse Volume Name for Simple Node
        obj.parent_name = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeName", Q_FUNC_INFO);

        obj.volume_offset = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_Number, "PartitionMapPartitionOffset", Q_FUNC_INFO);
        obj.volume_UUID = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeUUID", Q_FUNC_INFO);
        obj.volume_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);
        QString whole_disk_identifier = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "ParentWholeDisk", Q_FUNC_INFO).trimmed();

        if(parent_identifier != whole_disk_identifier)
        {
            parent_identifier = whole_disk_identifier;
            apfs_list_plist_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutil_apfs_list(whole_disk_identifier, Q_FUNC_INFO);
        }

        obj.role_type = recon_static_functions::get_value_of_key_from_diskutil_apfs_list_plist_volume(apfs_list_plist_path, vol_identifier, "Roles", enum_Global_Plist_File_Key_Type_Array, Q_FUNC_INFO);
        obj.fs_type = global_recon_helper_singular_class_obj->get_file_system_of_any_node(node_value.trimmed(),Q_FUNC_INFO);

        /// Remove diskutil info plist file
        QFile temp_file(temp_plist_file_path);
        if(temp_file.exists())
            temp_file.remove();

        objlist.append(obj);
    }

    return objlist;  //  /dev/disk4s2
}

QList<struct_global_image_mounter_partition_info> image_mounter::parse_corestorage_partition_info_from_hdiutil_output(QString out_put)
{

    //=================================================================//
    //  /dev/disk4          	GUID_partition_scheme
    //  /dev/disk4s1        	EFI
    //  /dev/disk4s2        	Apple_HFS    //OR   Apple_CoreStorage
    //  /dev/disk4s3        	Apple_Boot

    // OR

    // /dev/disk5
    //=================================================================//


    /*=============================== Note for hfs encryption /dev/disk5 Start======================
    This type of case is arise on HFS encrypted image when select core storage partition /dev/disk4s2 rather then full disk
    /dev/disk4 when creating image.
    In this case we haven't any information regarding to this disk because the output return nothing.
    =================================== Note for hfs encryption /dev/disk5 end ======================
    */

    QList <struct_global_image_mounter_partition_info> objlist;

    QStringList cs_volume_identifier_list = extract_corestorage_volume_identifier_list_from_hdiutil_output(out_put);

    QString out_diskutillist = run_command_with_argument("diskutil",QStringList("list"));
    out_diskutillist = out_diskutillist.trimmed();
    recon_static_functions::app_debug(" diskutil output " + (out_diskutillist),Q_FUNC_INFO);

    QString apfs_volume_roles_plist_path, parent_identifier;

    // fill core storage structure
    for(int zz = 0; zz < cs_volume_identifier_list.size() ; zz++)
    {
        struct_global_image_mounter_partition_info obj;
        recon_static_functions::clear_variables_image_mounting_partiton_info(obj);

        QString volume_idntfr = cs_volume_identifier_list.at(zz);
        obj.parent_type = MACRO_Disk_Partition_Type_Apple_CoreStorage;
        obj.parent_identifier = volume_idntfr;
        obj.parent_node = QString("/dev/") + volume_idntfr;

        /// Parse Volume Name for Simple Node
        obj.parent_name = get_value_of_key_from_diskutil_info_output(volume_idntfr, "VolumeName", enum_Global_Plist_File_Key_Type_String);

        QString temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(volume_idntfr, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

        obj.volume_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);

        QString disk_node = extract_derived_into_disknode_from_diskutillist_output(out_diskutillist,volume_idntfr);

        if(disk_node.trimmed() != "")
        {
            obj.derived_into_node = disk_node;
            obj.derived_into_identifier = disk_node.remove(QString("/dev/"));
            obj.derived_into_name = get_value_of_key_from_diskutil_info_output(disk_node, "VolumeName", enum_Global_Plist_File_Key_Type_String);

            QString whole_disk_identifier =  get_value_of_key_from_diskutil_info_output(obj.derived_into_identifier, "ParentWholeDisk", enum_Global_Plist_File_Key_Type_String);

            if(parent_identifier != whole_disk_identifier)
            {
                parent_identifier = whole_disk_identifier;
                apfs_volume_roles_plist_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutil_apfs_list(whole_disk_identifier, Q_FUNC_INFO);
            }

            obj.role_type = recon_static_functions::get_value_of_key_from_diskutil_apfs_list_plist_volume(apfs_volume_roles_plist_path, obj.derived_into_identifier, "Roles", enum_Global_Plist_File_Key_Type_Array, Q_FUNC_INFO);
            obj.fs_type = global_recon_helper_singular_class_obj->get_file_system_of_any_node(disk_node,Q_FUNC_INFO);
        }

        core_storage_and_derived_disk_node_list << obj.parent_node << obj.derived_into_node;

        obj.derived_into_uuid = extract_cs_volume_uuid_from_diskutillist_output(out_diskutillist,volume_idntfr);

        objlist << obj;
    }

    return objlist;
}

QStringList image_mounter::extract_corestorage_volume_identifier_list_from_hdiutil_output(QString out_put)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    QStringList volume_name_list;

    QStringList mlist = out_put.split("\n");
    for(int i = 0; i < mlist.size(); i++)
    {
        QString line = mlist.at(i);
        line = line.trimmed();

        if(line == QString(""))
            continue;

        if(!line.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive))
            continue;

        QStringList tmp_list = line.split(" ");

        QString node = tmp_list.at(0);
        node = node.remove("/dev/");
        node = node.trimmed();

        volume_name_list.append(node);
    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

    return volume_name_list;
}



QString image_mounter::extract_derived_into_disknode_from_diskutillist_output(QString out_put,QString cs_volumename)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);


    ///
    /// -------- Do not delete commented code ------
    ///

    //    /dev/disk2 (internal, virtual):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:                  Apple_HFS Macintosh HD           +250.0 GB   disk2
    //                                     Logical Volume on disk0s2, disk1s2
    //                                     D6B9275F-5331-46FC-B819-5A49EEF42D41
    //                                     Unencrypted Fusion Drive

    //    /dev/disk3 (external, physical):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:      GUID_partition_scheme                        *16.0 GB    disk3
    //       1:                        EFI EFI                     209.7 MB   disk3s1
    //       2:                  Apple_HFS Install macOS Sierra    13.5 GB    disk3s2
    //       3:          Apple_CoreStorage little                  1.0 GB     disk3s3
    //       4:          Apple_CoreStorage SMALLE                  1.0 GB     disk3s4

    //    /dev/disk4 (external, virtual):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:                  Apple_HFS SMALLE                 +637.5 MB   disk4
    //                                     Logical Volume on disk3s4
    //                                     97B5EB96-35D5-4249-BD3E-E922333814BE
    //                                     Unlocked Encrypted

    //    /dev/disk5 (external, physical):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:      GUID_partition_scheme                        *16.0 GB    disk5
    //       1:                        EFI EFI                     209.7 MB   disk5s1
    //       2:                  Apple_HFS POWER                   15.7 GB    disk5s2

    //    Offline
    //                                     Logical Volume little on disk3s3
    //                                     FDFA0A1A-4897-4ACB-9733-14A4DCA465C7
    //                                     Locked Encrypted

    //                                     OOOOOOOORRRRRRRRRRR
    //
    //    /dev/disk7 (synthesized):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:      APFS Container Scheme -                      +15.7 GB    disk7
    //                                     Physical Stores disk5s2, disk6s2
    //       1:                APFS Volume Fusion                  116.9 MB   disk7s1

    //==================================================================================================================//

    QString derived_into_disk;
    bool bool_derived_into_found = false;

    QStringList line_list = out_put.split("\n");
    for(int zz = 0; zz < line_list.size(); zz++)
    {
        QString parse_str = " on " + cs_volumename;           // Logical Volume little on disk3s3
        QString parse_str1 = " on " + cs_volumename + ", ";   // Logical Volume on disk0s2, disk1s2
        QString parse_str2 = ", " + cs_volumename;           //  Logical Volume on disk0s2, disk1s2
        QString parse_str3 = " " + cs_volumename + ",";           // Physical Stores disk5s2, disk6s2

        QString m_line = line_list.at(zz).trimmed();

        if(m_line.startsWith("Logical Volume ") && (line_list.at(zz).trimmed().endsWith(parse_str)
                                                    || line_list.at(zz).trimmed().contains(parse_str1)
                                                    || line_list.at(zz).trimmed().endsWith(parse_str2)))
        {
            // for hfs




            bool_derived_into_found = true;

            if(zz <= 0)
                break;

            QString str = line_list.at(zz - 1);
            str = str.trimmed();
            str = str.toLower();

            if(str.startsWith("offline"))
            {
                // do nothing
            }
            else
            {
                if(line_list.size() >= 4)
                {
                    QString str1 = line_list.at(zz - 3);
                    str1 = str1.trimmed();

                    if(str1.startsWith("/dev/disk"))
                    {
                        QStringList tmp_list = str1.split("(");
                        derived_into_disk = tmp_list.at(0).trimmed();
                    }
                }
            }

            break;
        }
        else if(m_line.startsWith("Physical Stores ") && (line_list.at(zz).trimmed().endsWith(parse_str2)
                                                          || line_list.at(zz).trimmed().contains(parse_str3)))
        {
            // for APFS

            int next_line_pos = zz + 1;
            if(next_line_pos >= line_list.size())
                continue;

            QString nxt_ln_val = line_list.at(next_line_pos);
            nxt_ln_val = nxt_ln_val.trimmed();

            QStringList next_line_list = nxt_ln_val.split(" ");
            if(next_line_list.size() <= 2)
                continue;

            QString mm_node = next_line_list.at(next_line_list.size() - 1);

            derived_into_disk = mm_node.trimmed();

            break;

        }

    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

    return derived_into_disk;
}

QString image_mounter::extract_cs_volume_uuid_from_diskutillist_output(QString out_put, QString cs_volumename)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    ///
    /// -------- Do not delete commented code ------
    ///

    //    /dev/disk2 (internal, virtual):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:                  Apple_HFS Macintosh HD           +250.0 GB   disk2
    //                                     Logical Volume on disk0s2, disk1s2
    //                                     D6B9275F-5331-46FC-B819-5A49EEF42D41
    //                                     Unencrypted Fusion Drive

    //    /dev/disk3 (external, physical):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:      GUID_partition_scheme                        *16.0 GB    disk3
    //       1:                        EFI EFI                     209.7 MB   disk3s1
    //       2:                  Apple_HFS Install macOS Sierra    13.5 GB    disk3s2
    //       3:          Apple_CoreStorage little                  1.0 GB     disk3s3
    //       4:          Apple_CoreStorage SMALLE                  1.0 GB     disk3s4

    //    /dev/disk4 (external, virtual):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:                  Apple_HFS SMALLE                 +637.5 MB   disk4
    //                                     Logical Volume on disk3s4
    //                                     97B5EB96-35D5-4249-BD3E-E922333814BE
    //                                     Unlocked Encrypted

    //    /dev/disk5 (external, physical):
    //       #:                       TYPE NAME                    SIZE       IDENTIFIER
    //       0:      GUID_partition_scheme                        *16.0 GB    disk5
    //       1:                        EFI EFI                     209.7 MB   disk5s1
    //       2:                  Apple_HFS POWER                   15.7 GB    disk5s2

    //    Offline
    //                                     Logical Volume little on disk3s3
    //                                     FDFA0A1A-4897-4ACB-9733-14A4DCA465C7
    //                                     Locked Encrypted

    //==================================================================================================================//


    QString volume_uuid;
    bool bool_uuid_found = false;

    QStringList line_list = out_put.split("\n");
    for(int zz = 0; zz < line_list.size(); zz++)
    {
        QString parse_str = " on " + cs_volumename;           // Logical Volume SMALLE on disk3s4
        QString parse_str1 = " on " + cs_volumename + ", ";   // Logical Volume on disk0s2, disk1s2
        QString parse_str2 = ", " + cs_volumename;           // Logical Volume on disk0s2, disk1s2

        if(line_list.at(zz).trimmed().startsWith("Logical Volume ") && (line_list.at(zz).trimmed().endsWith(parse_str)
                                                                        || line_list.at(zz).trimmed().contains(parse_str1)
                                                                        || line_list.at(zz).trimmed().endsWith(parse_str2)))
        {
            bool_uuid_found = true;

            if(line_list.size() > (zz + 1))
                volume_uuid = line_list.at(zz + 1).trimmed();

            break;
        }
    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

    return volume_uuid;
}

QString image_mounter::parse_disk_node_from_hdiutil_output(QString out_put)
{
    //=================================================================//
    //  /dev/disk4          	GUID_partition_scheme
    //  /dev/disk4s1        	EFI
    //  /dev/disk4s2        	Apple_HFS    //OR   Apple_CoreStorage
    //  /dev/disk4s3        	Apple_Boot
    //
    // OR
    //
    // /dev/disk5
    //
    // OR
    //
    //  /dev/disk6          	GUID_partition_scheme
    // /dev/disk6s1        	EFI
    // /dev/disk6s2        	Apple_APFS
    // /dev/disk7          	EF57347C-0000-11AA-AA11-0030654
    // /dev/disk7s1        	41504653-0000-11AA-AA11-0030654
    // /dev/disk7s2        	41504653-0000-11AA-AA11-0030654
    // /dev/disk7s3        	41504653-0000-11AA-AA11-0030654
    // /dev/disk7s4        	41504653-0000-11AA-AA11-0030654

    //=================================================================//

    if(out_put.contains("GUID_partition_scheme"))
    {
        QStringList splitted_out = out_put.split("\n");

        for(int i = 0; i < splitted_out.size(); i++)
        {
            QString str_value = splitted_out.at(i);
            if(str_value.contains("GUID_partition_scheme"))
            {
                str_value.remove("GUID_partition_scheme");
                str_value = str_value.trimmed();
                return str_value;
            }
        }
    }


    QString str = out_put.split("\n").at(0);
    QStringList tmp_list = str.split(" ");

    return tmp_list.at(0); // /dev/disk5

}


bool image_mounter::create_directory_structure()
{
    shadow_files_directory = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Shadow_File_QString).toString();

    QDir dir;
    if(!dir.mkpath(shadow_files_directory))
        return false;

    QString fuse_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString();

    QString available_file1 = recon_static_functions::get_available_filename(struct_image_info_obj.image_name_1, fuse_path, Q_FUNC_INFO);
    QString mount_point1 = fuse_path + available_file1 + "/";
    if(!dir.mkpath(mount_point1))
        return false;
    struct_image_info_obj.osx_fuse_mount_point_1 = mount_point1;

    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        QString available_file2 = recon_static_functions::get_available_filename(struct_image_info_obj.image_name_2, fuse_path, Q_FUNC_INFO);

        QString mount_point2 = fuse_path + available_file2 + "/";
        if(!dir.mkpath(mount_point2))
            return false;
        struct_image_info_obj.osx_fuse_mount_point_2 = mount_point2;
    }


    return true;
}

void image_mounter::slot_prc_generic_with_argument_finished(int)
{
    bool_prc_generic_with_argument = true;
}

void image_mounter::slot_prc_generic_with_argument_mergechannel_finished(int)
{
    bool_prc_generic_with_argument_mergechannel = true;
}

void image_mounter::slot_prc_generic_without_argument_finished(int)
{
    bool_prc_generic_without_argument = true;

}
void image_mounter::slot_prc_generic_finished_with_timer(int)
{
    bool_prc_generic_with_timer = true;
}
void image_mounter::slot_timer_timeout()
{
    bool_m_timer_finish = true;
    m_timer->stop();

}

QString image_mounter::run_command_with_argument(QString command, QStringList arguments)
{
    bool_prc_generic_with_argument = false;
    prc_generic_with_argument->start(command,arguments);
    if(!prc_generic_with_argument->waitForStarted())
    {
        recon_static_functions::app_debug(" waitForStarted ---FAILED---  " + prc_generic_with_argument->errorString(), Q_FUNC_INFO);
        return "";
    }

    // recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_prc_generic_with_argument || (prc_generic_with_argument->state() == QProcess::NotRunning))
        {
            break;
        }
    }
    // recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(prc_generic_with_argument->readAll().trimmed());
}

QString image_mounter::run_command_with_argument_mergechannel(QString command, QStringList arguments)
{
    bool_prc_generic_with_argument_mergechannel = false;
    prc_generic_with_argument_mergechannel->start(command,arguments);
    if(!prc_generic_with_argument_mergechannel->waitForStarted())
    {
        recon_static_functions::app_debug(" waitForStarted ---FAILED---  " + prc_generic_with_argument_mergechannel->errorString(), Q_FUNC_INFO);
        return "";
    }

    // waitForReadyRead() is used for images(like installer image),
    // which needs keyboard input for further mounting/processing..
    prc_generic_with_argument_mergechannel->waitForReadyRead(60000);



    // recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_prc_generic_with_argument_mergechannel || (prc_generic_with_argument_mergechannel->state() == QProcess::NotRunning))
        {
            break;
        }
        else if(bool_is_it_installer_dmg || (prc_generic_with_argument_mergechannel->state() == QProcess::Running))
        {
            prc_generic_with_argument_mergechannel->kill();
            break;
        }
    }
    // recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    // return QString::fromLocal8Bit(prc_generic_with_argument_mergechannel->readAll().trimmed());
    return QString::fromLocal8Bit(merge_channel_prc_readall_array.trimmed());

}

QString image_mounter::run_command_without_argument(QString command)
{
    bool_prc_generic_without_argument = false;
    prc_generic_without_argument->start(command);
    if(!prc_generic_without_argument->waitForStarted())
    {
        recon_static_functions::app_debug(" waitForStarted ---FAILED---  " + prc_generic_without_argument->errorString(), Q_FUNC_INFO);
        return "";
    }

    //recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_prc_generic_without_argument || (prc_generic_without_argument->state() == QProcess::NotRunning))
            break;
    }
    //  recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(prc_generic_without_argument->readAll().trimmed());
}


QString image_mounter::run_command_with_timer(QString command, QStringList arguments, int interval)
{
    bool_prc_generic_with_timer = false;
    bool_m_timer_finish = false;

    prc_generic_with_timer->start(command,arguments);
    if(!prc_generic_with_timer->waitForStarted())
    {
        recon_static_functions::app_debug(" waitForStarted ---FAILED---  " + prc_generic_with_timer->errorString(), Q_FUNC_INFO);
        return "";
    }

    m_timer->setInterval(interval);
    m_timer->start();

    //recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_prc_generic_with_timer)
        {
            break;
        }
        if(bool_m_timer_finish  || (prc_generic_with_timer->state() == QProcess::NotRunning))
            break;
    }
    //recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    m_timer->stop();

    return QString::fromLocal8Bit(prc_generic_with_timer->readAll().trimmed());
}

void image_mounter::turn_indexing_ON_OFF(QString value,QString mount_path)
{
    QStringList args;
    args << "-i" << value << mount_path;

    bool_m_timer_finish = false;
    m_timer->setInterval(10000);
    m_timer->start();


    //recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        QString out =  run_command_with_argument("mdutil",args);

        // Error: Index is already changing state.  Please try again in a moment.
        if(!out.trimmed().contains("Error"))
        {
            m_timer->stop();
            break;
        }

        if(bool_m_timer_finish)
            break;
    }
    // recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    m_timer->stop();

}

void image_mounter::unmount_all_image()
{
    for(int pos = list_struct_image_info.size() - 1; pos >= 0; pos--)
    {
        unmount_image(list_struct_image_info.at(pos).image_path_1);
    }
}

void image_mounter::unmount_all_fuse()
{
    QString target_dir_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString();

    QDirIterator iter_1(target_dir_1);

    while(iter_1.hasNext())
    {
        QString f_path = iter_1.next();

        QFileInfo m_info(f_path);

        if(m_info.isDir())
        {
            if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(f_path, Q_FUNC_INFO))
                continue;

            QProcess proc_unmount;
            proc_unmount.start("diskutil" , QStringList() << "unmount" <<  "force" << f_path);
            proc_unmount.waitForReadyRead(10000);
            proc_unmount.waitForFinished(10000);
        }
    }

}

void image_mounter::unmount_image(QString image_path)
{
    if(image_path.trimmed().isEmpty())
        return;

    recon_static_functions::app_debug("start " + image_path,Q_FUNC_INFO);

    int pos = -1;
    for(int pp = 0; pp < list_struct_image_info.size(); pp++)
    {
        struct_global_image_mounter_image_info obj = list_struct_image_info.at(pp);
        if(obj.image_path_1 == image_path)
        {
            pos = pp;
            break;
        }
    }

    if(pos != -1)
    {
        struct_global_image_mounter_image_info obj = list_struct_image_info.at(pos);

        for(int i = obj.list_struct_partition_info.size() - 1; i >= 0; i--)
        {
            QStringList args_diskutil;

            QString volume_node;
            struct_global_image_mounter_partition_info st_partition_info = obj.list_struct_partition_info.at(i);

            if(obj.image_format_type == MACRO_Target_Disk_ImageType_ENCASE_LOGICAL)
            {
                volume_node = st_partition_info.mount_path;
            }
            else if(st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive) || st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
            {
                volume_node = st_partition_info.derived_into_node;
            }
            else
            {
                volume_node = st_partition_info.parent_node;
            }

            volume_node = volume_node.trimmed();

            args_diskutil << "unmount" << "force" << volume_node;

            run_command_with_argument("diskutil",args_diskutil);
        }


        for(int i = obj.list_struct_partition_info.size() - 1; i >= 0; i--)
        {
            QStringList args_hdiutil;

            QString volume_node;
            struct_global_image_mounter_partition_info st_partition_info = obj.list_struct_partition_info.at(i);
            if(st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive) || st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
            {
                volume_node = st_partition_info.derived_into_node;
            }
            else
            {
                volume_node = st_partition_info.parent_node;
            }

            volume_node = volume_node.trimmed();

            args_hdiutil << "eject" << volume_node;

            run_command_with_argument("hdiutil",args_hdiutil);
        }



        if(obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
        {
            QStringList args_hdiutil2;
            args_hdiutil2 << "eject" << obj.disk_node_2;
            run_command_with_timer("hdiutil",args_hdiutil2,20000); // for break fusion unmounting
        }

        QStringList args_hdiutil;
        args_hdiutil << "eject" << obj.disk_node_1;
        if(obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
            run_command_with_timer("hdiutil",args_hdiutil,20000); // for break fusion unmounting
        else
            run_command_with_argument("hdiutil",args_hdiutil);

        if(obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
        {
            QStringList args_fuse_mount_point2;
            args_fuse_mount_point2 << "eject" << obj.osx_fuse_mount_point_2;
            run_command_with_argument("hdiutil",args_fuse_mount_point2);

            recon_static_functions::remove_safely_blank_dir_OR_file(obj.osx_fuse_mount_point_2, Q_FUNC_INFO);
            recon_static_functions::remove_safely_blank_dir_OR_file(obj.shadow_file_path_2, Q_FUNC_INFO);
        }

        QStringList args_fuse_mount_point;
        args_fuse_mount_point << "eject" << obj.osx_fuse_mount_point_1;
        run_command_with_argument("hdiutil",args_fuse_mount_point);


        recon_static_functions::remove_safely_blank_dir_OR_file(obj.osx_fuse_mount_point_1, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(obj.shadow_file_path_1, Q_FUNC_INFO);

        for(int zz = 0; zz < obj.list_struct_partition_info.size(); zz++)
        {
            recon_static_functions::remove_safely_blank_dir_OR_file(obj.list_struct_partition_info.at(zz).mount_path, Q_FUNC_INFO);
        }

        list_struct_image_info.removeAt(pos);
    }

    // if obj is not inserted in list(inserted at last point of mounting) -- if image is not mounting accurate and properly -- then this command do clean all
    for(int i = struct_image_info_obj.list_struct_partition_info.size() - 1; i >= 0; i--)
    {
        QStringList args_diskutil;

        QString volume_node;
        struct_global_image_mounter_partition_info st_partition_info = struct_image_info_obj.list_struct_partition_info.at(i);

        if(st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive) || st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
        {
            volume_node = st_partition_info.derived_into_node;
        }
        else
        {
            volume_node = st_partition_info.parent_node;
        }

        volume_node = volume_node.trimmed();

        args_diskutil << "unmount" << "force" << volume_node;
        run_command_with_argument("diskutil",args_diskutil);
    }

    for(int i = struct_image_info_obj.list_struct_partition_info.size() - 1; i >= 0; i--)
    {
        QStringList args_hdiutil;

        QString volume_node;
        struct_global_image_mounter_partition_info st_partition_info = struct_image_info_obj.list_struct_partition_info.at(i);
        if(st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage,Qt::CaseInsensitive) || st_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
        {
            volume_node = st_partition_info.derived_into_node;
        }
        else
        {
            volume_node = st_partition_info.parent_node;
        }

        volume_node = volume_node.trimmed();

        args_hdiutil << "eject" << volume_node;
        run_command_with_argument("hdiutil",args_hdiutil);
    }


    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        QStringList pr_node2;
        pr_node2 << "eject" << struct_image_info_obj.disk_node_2;
        run_command_with_timer("hdiutil",pr_node2,20000); // for break fusion unmounting
    }


    QStringList pr_node;
    pr_node << "eject" << struct_image_info_obj.disk_node_1;
    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
        run_command_with_timer("hdiutil",pr_node,20000); // for break fusion unmounting
    else
        run_command_with_argument("hdiutil",pr_node);


    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {

        QStringList args_fuse_mount_point2;
        args_fuse_mount_point2 << "eject" << struct_image_info_obj.osx_fuse_mount_point_2;
        run_command_with_argument("hdiutil",args_fuse_mount_point2);

        recon_static_functions::remove_safely_blank_dir_OR_file(struct_image_info_obj.osx_fuse_mount_point_2, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(struct_image_info_obj.shadow_file_path_2, Q_FUNC_INFO);

    }

    QStringList args_fuse_mount_point;
    args_fuse_mount_point << "eject" << struct_image_info_obj.osx_fuse_mount_point_1;
    run_command_with_argument("hdiutil",args_fuse_mount_point);

    recon_static_functions::remove_safely_blank_dir_OR_file(struct_image_info_obj.osx_fuse_mount_point_1, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(struct_image_info_obj.shadow_file_path_1, Q_FUNC_INFO);

    for(int zz = 0; zz < struct_image_info_obj.list_struct_partition_info.size(); zz++)
    {
        recon_static_functions::remove_safely_blank_dir_OR_file(struct_image_info_obj.list_struct_partition_info.at(zz).mount_path, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void image_mounter::check_for_os_fuse_installation()
{
    bool_osx_fuse_installed = false;

    QDirIterator iter("/usr/local/lib", QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);

    while(iter.hasNext())
    {
        QString str = iter.next();

        if(str.contains("libosxfuse"))
        {
            bool_osx_fuse_installed = true;
            break;
        }
    }
}

void image_mounter::prepare_shadow_file_path()
{
    QString available_file = recon_static_functions::get_available_filename(struct_image_info_obj.image_name_1, shadow_files_directory, Q_FUNC_INFO);
    struct_image_info_obj.shadow_file_path_1 = shadow_files_directory + available_file + ".shadow";
    struct_image_info_obj.shadow_file_path_1  = struct_image_info_obj.shadow_file_path_1.trimmed();

    if(struct_image_info_obj.image_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        available_file = recon_static_functions::get_available_filename(struct_image_info_obj.image_name_2, shadow_files_directory, Q_FUNC_INFO);
        struct_image_info_obj.shadow_file_path_2 = shadow_files_directory + available_file + ".shadow";
        struct_image_info_obj.shadow_file_path_2  = struct_image_info_obj.shadow_file_path_2.trimmed();

    }
}
bool image_mounter::is_image_mounted(QString image_path)
{
    QStringList mount_path_list;
    for(int pp = 0; pp < list_struct_image_info.size(); pp++)
    {
        struct_global_image_mounter_image_info obj = list_struct_image_info.at(pp);
        if(obj.image_path_1 == image_path)
        {
            for(int i = 0; i < obj.list_struct_partition_info.size(); i++)
            {
                mount_path_list << obj.list_struct_partition_info.at(i).mount_path;
            }

            if(mount_path_list.size() <= 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    return false;
}

QStringList image_mounter::get_all_mount_path()
{
    QStringList mount_path_list;
    for(int pp = 0; pp < list_struct_image_info.size(); pp++)
    {
        struct_global_image_mounter_image_info obj = list_struct_image_info.at(pp);

        for(int i = 0; i < obj.list_struct_partition_info.size(); i++)
        {
            mount_path_list << obj.list_struct_partition_info.at(i).mount_path;
        }
    }

    return mount_path_list;
}

void image_mounter::set_progress_bar(progress_bar_window *obj)
{
    progress_bar_obj = obj;
}

struct_global_image_mounter_image_info image_mounter::pub_get_struct_image_info_last_mounted()
{
    struct_global_image_mounter_image_info tmp_struct_image_info_obj = struct_image_info_obj;

    QList<struct_global_image_mounter_partition_info> list_struct_partition_info;

    for(int i = 0 ; i < tmp_struct_image_info_obj.list_struct_partition_info.size(); i++)
    {
        struct_global_image_mounter_partition_info t1 = tmp_struct_image_info_obj.list_struct_partition_info.at(i);

        if(!t1.mount_path.trimmed().isEmpty())
        {
            list_struct_partition_info.append(t1);
        }
    }

    tmp_struct_image_info_obj.list_struct_partition_info = list_struct_partition_info;

    return tmp_struct_image_info_obj;
}

struct_global_image_mounter_image_info image_mounter::pub_get_struct_image_info_by_image_path(QString image_path)
{
    struct_global_image_mounter_image_info img_obj;
    recon_static_functions::clear_variables_image_mounting_image_info(img_obj);

    for(int pp = 0; pp < list_struct_image_info.size(); pp++)
    {
        struct_global_image_mounter_image_info obj = list_struct_image_info.at(pp);
        if(obj.image_path_1 == image_path)
        {
            img_obj = obj;
            break;
        }
    }


    struct_global_image_mounter_image_info tmp_struct_image_info_obj = img_obj;
    QList<struct_global_image_mounter_partition_info> list_struct_partition_info;
    for(int i = 0 ; i < tmp_struct_image_info_obj.list_struct_partition_info.size(); i++)
    {
        struct_global_image_mounter_partition_info t1 = tmp_struct_image_info_obj.list_struct_partition_info.at(i);

        if(!t1.mount_path.trimmed().isEmpty())
        {
            list_struct_partition_info.append(t1);
        }
    }

    tmp_struct_image_info_obj.list_struct_partition_info = list_struct_partition_info;

    return tmp_struct_image_info_obj;
}

QStringList image_mounter::get_mount_path(QString image_path)
{
    QStringList mount_path_list;
    for(int pp = 0; pp < list_struct_image_info.size(); pp++)
    {
        struct_global_image_mounter_image_info obj = list_struct_image_info.at(pp);
        if(obj.image_path_1 == image_path)
        {
            for(int i = 0; i < obj.list_struct_partition_info.size(); i++)
            {
                mount_path_list << obj.list_struct_partition_info.at(i).mount_path;
            }

            break;
        }
    }

    return mount_path_list;
}

int image_mounter::run_fuse_mount(int fuse_type, QStringList args)
{

    recon_static_functions::debug_intensive("fuse mount INFO == " + args.join("----"),Q_FUNC_INFO);

    //    QStringList args;
    //    args << "-X" << "allow_other" << image_path << fuse_point;



    QString exec_path;
    if(fuse_type == enum_fuse_type_ewfmount)
    {
        exec_path = "./ewfmount";
    }
    else if(fuse_type == enum_fuse_type_smrawmount)
    {
        exec_path = "./smrawmount";
    }
    else if(fuse_type == enum_fuse_type_vmdkmount)
    {
        exec_path = "./vmdkmount";
    }
    else if(fuse_type == enum_fuse_type_vhdimount)
    {
        exec_path = "./vhdimount";
    }
    else if(fuse_type == enum_fuse_type_aff4fuse)
    {
        exec_path = "./aff4_fuse/aff4fuse";
    }


    QFile file_check(exec_path);
    if(!file_check.exists() || exec_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug("Executable,it does not Exist -----FAILED----" + exec_path, Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1005_Fuse_Executable_Not_Exist) + "]");
        return -1;
    }

    QString out_fusemount =  run_command_with_argument(exec_path,args);
    out_fusemount = out_fusemount.trimmed();
    recon_static_functions::app_debug(" fuse mount output " + out_fusemount,Q_FUNC_INFO);

    if(out_fusemount.contains("is itself on a OSXFUSE volume"))
    {
        recon_static_functions::app_debug(" itself on a OSXFUSE volume   ---FAILED---  ",Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1004_Fuse_Already_Mounted) + "]");
        //  message_dialog_object->show();
        return -1;
    }

    return 0;
}

void image_mounter::fill_partition_info_structure_after_mounting()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);


    for(int ii = 0; ii < struct_image_info_obj.list_struct_partition_info.size(); ii++)
    {
        struct_global_image_mounter_partition_info struct_partition_info = struct_image_info_obj.list_struct_partition_info.takeAt(ii);

        QString temp_plist_file_path;

        if(struct_partition_info.parent_type.toLower() == QString(MACRO_Disk_Partition_Type_Apple_CoreStorage).toLower()
                || struct_partition_info.parent_type.toLower() == QString(MACRO_Disk_Partition_Type_Apple_APFS).toLower())
        {
            temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(struct_partition_info.derived_into_identifier, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);
        }
        else
        {
            temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(struct_partition_info.parent_identifier, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);
        }

        /// Parse Volume Size for Simple Node
        struct_partition_info.total_size_byte_numeric_QString = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_Number, "TotalSize", Q_FUNC_INFO).trimmed();

        /// Parse Volume Free Space for Simple Node
        struct_partition_info.free_size_byte_numeric_QString = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_Number, "FreeSpace", Q_FUNC_INFO).trimmed();

        /// Remove diskutil info plist file
        QFile temp_file(temp_plist_file_path);
        if(temp_file.exists())
            temp_file.remove();

        struct_image_info_obj.list_struct_partition_info.insert(ii,struct_partition_info);
    }

    recon_static_functions::app_debug(" end ",Q_FUNC_INFO);
}

QString image_mounter::get_value_of_key_from_diskutil_info_output(QString identifier_name, QString key_name, int m_datatype)
{
    /* Here we run command
     * diskutil info -plist disk2s2 > ~/tmp/123.plist
     * then match the key name from the output and then return the value of that key
    */

    recon_static_functions::app_debug(" start" + identifier_name + "--" + key_name,Q_FUNC_INFO);

    if(identifier_name.isEmpty())
        return "";

    if(key_name.isEmpty())
        return "";

    QString value;

    QString temp_plist_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Miscellenous_In_Result_QString).toString() + "temp_diskutil_info_" + identifier_name + ".plist";

    QStringList arg_list;
    arg_list << "info" << "-plist" << identifier_name;
    QString output_value = run_command_with_argument("diskutil",arg_list);

    recon_static_functions::overwrite_data_into_file(temp_plist_path,output_value,Q_FUNC_INFO);

    value = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_path,m_datatype, key_name, Q_FUNC_INFO);


    recon_static_functions::remove_safely_blank_dir_OR_file(temp_plist_path, Q_FUNC_INFO);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

    return value;
}

QStringList image_mounter::extract_apfs_parent_identifier_list_from_hdiutil_output(QString out_put)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    QStringList volume_name_list;

    QStringList mlist = out_put.split("\n");
    for(int i = 0; i < mlist.size(); i++)
    {
        QString line = mlist.at(i);
        line = line.trimmed();

        if(line == QString(""))
            continue;

        if(!line.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
            continue;

        QStringList tmp_list = line.split(" ");

        QString node = tmp_list.at(0);
        node = node.remove("/dev/");
        node = node.trimmed();

        volume_name_list.append(node);
    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

    return volume_name_list;
}

QList<struct_global_image_mounter_partition_info> image_mounter::parse_apfs_disk_partition_info_from_hdiutil_output(QString out_put)
{
    QList <struct_global_image_mounter_partition_info> objlist;

    QStringList parent_identifier_list = extract_apfs_parent_identifier_list_from_hdiutil_output(out_put);

    QString out_diskutillist = run_command_with_argument("diskutil",QStringList("list"));
    out_diskutillist = out_diskutillist.trimmed();

    // fill core storage structure
    for(int zz = 0; zz < parent_identifier_list.size() ; zz++)
    {
        extract_n_fill_derived_apfs_partitions_info_from_diskutillist_output(out_diskutillist,parent_identifier_list.at(zz), objlist);
    }

    return objlist;
}

void image_mounter::extract_n_fill_derived_apfs_partitions_info_from_diskutillist_output(QString diskutil_list_output, QString apfs_identifier, QList <struct_global_image_mounter_partition_info> &list_partition_info)
{
    ///
    /// -------- Do not delete commented code ------
    ///
    /*
 * /dev/disk5 (disk image):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        +120.0 GB   disk5
   1:                        EFI EFI                     209.7 MB   disk5s1
   2:                 Apple_APFS Container disk6         119.8 GB   disk5s2

/dev/disk6 (synthesized):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      APFS Container Scheme -                      +119.8 GB   disk6
                                 Physical Store disk5s2
   1:                APFS Volume FORENSO_10_13_Beta      18.3 GB    disk6s1
   2:                APFS Volume Preboot                 55.7 MB    disk6s2
   3:                APFS Volume Recovery                1.0 GB     disk6s3
   4:                APFS Volume VM                      20.5 KB    disk6s4
 */

    //==================================================================================================================//


    recon_static_functions::app_debug("start",Q_FUNC_INFO);


    QString derived_into_disk = get_apfs_container_identifier(diskutil_list_output , apfs_identifier);

    if(derived_into_disk.isEmpty())
        return;


    QStringList line_list = diskutil_list_output.split("\n");

    QString apfs_volume_roles_plist_path, parent_identifier;

    //- Now we are parsing and insert into partition info structure list by using derived into disk node
    for(int zz = 0; zz < line_list.size(); zz++)
    {
        QString line = line_list.at(zz);
        if(line.contains("APFS Volume ") && line.contains(derived_into_disk + "s"))
        {
            QStringList splitted_list = line.split("APFS Volume ");

            if(splitted_list.size() > 1)
            {
                QString second_str = splitted_list.at(1);
                QStringList tmp_list = second_str.split(" ");

                tmp_list.removeAll("");

                if(tmp_list.size() > 3)
                {
                    struct_global_image_mounter_partition_info obj;
                    recon_static_functions::clear_variables_image_mounting_partiton_info(obj);

                    QString apfs_partition_size;
                    QString apfs_partition_disk_identifier = tmp_list.at(tmp_list.size() - 1);
                    QString apfs_partition_name = get_volume_name_using_diskutil_info(apfs_partition_disk_identifier);

                    obj.parent_type = MACRO_Disk_Partition_Type_Apple_APFS;
                    obj.derived_into_identifier = apfs_partition_disk_identifier;

                    QString temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(apfs_partition_disk_identifier, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

                    obj.volume_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);
                    obj.derived_into_node = "/dev/" + apfs_partition_disk_identifier;
                    obj.parent_identifier = apfs_identifier;
                    obj.parent_name = apfs_partition_name;
                    obj.parent_node = "/dev/" + apfs_identifier;
                    obj.total_size_byte_numeric_QString = apfs_partition_size;


                    obj.fs_type = global_recon_helper_singular_class_obj->get_file_system_of_any_node(apfs_partition_disk_identifier, Q_FUNC_INFO);

                    QString whole_disk_identifier =  get_value_of_key_from_diskutil_info_output(obj.derived_into_identifier, "ParentWholeDisk", enum_Global_Plist_File_Key_Type_String);

                    if(parent_identifier != whole_disk_identifier)
                    {
                        parent_identifier = whole_disk_identifier;
                        apfs_volume_roles_plist_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutil_apfs_list(whole_disk_identifier, Q_FUNC_INFO);
                    }

                    obj.role_type = recon_static_functions::get_value_of_key_from_diskutil_apfs_list_plist_volume(apfs_volume_roles_plist_path, obj.derived_into_identifier, "Roles",enum_Global_Plist_File_Key_Type_Array,Q_FUNC_INFO);

                    list_partition_info << obj;

                    QString derived_disk_node = "/dev/" + apfs_partition_disk_identifier;
                    core_storage_and_derived_disk_node_list << obj.parent_node << derived_disk_node;
                }
            }
        }
    }


    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}

QString image_mounter::get_apfs_container_identifier(QString diskutil_output_str , QString apfs_identifier_str)
{


    /// 2:                 Apple_APFS Container disk6         119.8 GB   disk5s2

    /// Input: disk5s2
    /// Return:  disk6

    //---------------------------------------------//

    QString derived_into_disk;
    QStringList line_list = diskutil_output_str.split("\n");
    for(int zz = 0; zz < line_list.size(); zz++)
    {
        QString line = line_list.at(zz);

        line = line.trimmed();

        if(line.contains("Apple_APFS Container") && line.endsWith(apfs_identifier_str))
        {
            QStringList list_1 = line.split("Apple_APFS Container ");
            if(list_1.size() >1)
            {
                QString temp_derived_str = list_1.at(1);

                QStringList list_2 = temp_derived_str.split(" ");

                if(list_2.size() > 1)
                {
                    derived_into_disk = list_2.at(0);
                    derived_into_disk = derived_into_disk.trimmed();
                    break;
                }
            }

        }
    }

    return derived_into_disk;
}


QString image_mounter::check_for_fdisk_mounting_scheme(QString hdiutil_output, QStringList hdiutil_args)
{
    recon_static_functions::app_debug("Start ",Q_FUNC_INFO);

    hdiutil_output = hdiutil_output.trimmed();
    recon_static_functions::app_debug("Recieved hdiutil output --  " + hdiutil_output,Q_FUNC_INFO);

    if(hdiutil_output.contains("Windows"))
        return hdiutil_output;

    //    if(hdiutil_output.contains("FDisk_partition_scheme") && (os_version_full_running_machine.contains("10.13")
    //                                                             || os_version_full_running_machine.contains("10.14")
    //                                                             || os_version_full_running_machine.contains("10.15")
    //                                                             || os_version_full_running_machine.contains("10.16")
    //                                                             || os_version_full_running_machine.startsWith("11")
    //                                                             || os_version_full_running_machine.startsWith("12")))// Looks like a device with 4096 block size

    if(hdiutil_output.contains("FDisk_partition_scheme"))// Looks like a device with 4096 block size
    {
        QStringList t_list = hdiutil_output.split(" ");
        if(t_list.size() > 1)
        {
            QString t_node_to_remove = t_list.at(0);

            QStringList t_args;
            t_args << "eject" << "-force" << t_node_to_remove ;

            //hdiutil eject -force /dev/disk6
            //"disk6" ejected.

            QString t_out = run_command_with_argument("hdiutil", t_args);
            t_out = t_out.trimmed();

            if(!t_out.endsWith("ejected."))
            {
                t_args.clear();
                t_args <<  "eject"  << t_node_to_remove ;

                //diskutil eject /dev/disk5
                // Disk /dev/disk5 ejected

                t_out = run_command_with_argument("diskutil", t_args);
            }
        }
        hdiutil_args << "-blocksize" << "4096";

        hdiutil_output = run_command_with_argument("hdiutil",hdiutil_args);
        hdiutil_output = hdiutil_output.trimmed();

        recon_static_functions::app_debug("hdiutil output After blocksize -- " + hdiutil_output,Q_FUNC_INFO);
    }
    else
    {
        recon_static_functions::app_debug("Not contain FDisk_partition_scheme ",Q_FUNC_INFO);
    }

    return hdiutil_output;
}

QList<struct_global_image_mounter_partition_info> image_mounter::parse_apfs_container_partition_info_from_hdiutil_output(QString out_put)
{

    /*
        /dev/disk9
        OR
        /dev/disk10         	EF57347C-0000-11AA-AA11-0030654
        /dev/disk10s1       	41504653-0000-11AA-AA11-0030654
    */

    recon_static_functions::app_debug("Start", Q_FUNC_INFO);
    QList <struct_global_image_mounter_partition_info> objlist;


    QStringList hdutil_outpt_in_list = out_put.split("\n");
    QString parent_disk_node = parse_disk_node_from_hdiutil_output(out_put);
    QString apfs_volume_roles_plist_path, parent_identifier;
    QString temp_parent_node_file_path, prnt_identifier;

    for(int ii = 0; ii < hdutil_outpt_in_list.size(); ii++)
    {
        struct_global_image_mounter_partition_info obj;
        recon_static_functions::clear_variables_image_mounting_partiton_info(obj);

        QString line = hdutil_outpt_in_list.at(ii);
        line = line.trimmed();

        if(line.isEmpty())
            continue;

        QStringList tmp_list = line.split(" ");
        QString node_value = tmp_list.at(0);  // /dev/disk10s1
        node_value = node_value.trimmed();
        if(node_value.isEmpty() || tmp_list.size() <= 1)
            continue;

        QString tmp_node = node_value;
        tmp_node.remove(0,9); // removing /dev/disk
        if(!tmp_node.contains("s"))
            continue;

        if(core_storage_and_derived_disk_node_list.contains(node_value))
            continue;

        obj.fs_type = global_recon_helper_singular_class_obj->get_file_system_of_any_node(node_value,Q_FUNC_INFO);


        if(!obj.fs_type.contains(MACRO_FILE_SYSTEM_TYPE_APFS, Qt::CaseSensitive))
            continue;

        //------------------- parent_type start--------------------
        /// Parse Volume Type for node
        //We don't receive parent type from output so we manually Initialize parent type to Apple_APFS
        //After this all the procedure will be same as we do in APFS Decryption

        obj.parent_type = MACRO_Disk_Partition_Type_Apple_APFS;
        //------------------- parent_type end--------------------

        obj.parent_node = parent_disk_node;

        obj.derived_into_node = node_value;

        obj.derived_into_identifier = node_value.remove(QString("/dev/"));

        QString vol_identifier = parent_disk_node.remove("/dev/");
        obj.parent_identifier = vol_identifier;

        /// Get Plist file path after running diskutil info -plist identifier_name > plist_file_path
        QString temp_derived_node_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(obj.derived_into_identifier, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

        obj.volume_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);

        if(prnt_identifier != vol_identifier)
        {
            prnt_identifier = vol_identifier;
            temp_parent_node_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(obj.parent_identifier, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);
        }

        /// Parse Volume Name for Simple Node
        obj.parent_name = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_parent_node_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeName", Q_FUNC_INFO);

        obj.derived_into_name = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeName", Q_FUNC_INFO);

        obj.volume_offset = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_Number, "PartitionMapPartitionOffset", Q_FUNC_INFO);

        obj.volume_UUID = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_String, "VolumeUUID", Q_FUNC_INFO);


        obj.total_size_byte_numeric_QString = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_Number, "TotalSize", Q_FUNC_INFO).trimmed();

        obj.free_size_byte_numeric_QString = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_Number, "FreeSpace", Q_FUNC_INFO).trimmed();

        QString whole_disk_identifier = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_derived_node_file_path,enum_Global_Plist_File_Key_Type_String, "ParentWholeDisk", Q_FUNC_INFO).trimmed();

        if(parent_identifier != whole_disk_identifier)
        {
            parent_identifier = whole_disk_identifier;
            apfs_volume_roles_plist_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutil_apfs_list(whole_disk_identifier, Q_FUNC_INFO);
        }

        obj.role_type = recon_static_functions::get_value_of_key_from_diskutil_apfs_list_plist_volume(apfs_volume_roles_plist_path, obj.derived_into_identifier, "Roles", enum_Global_Plist_File_Key_Type_Array, Q_FUNC_INFO);

        /// Remove diskutil info plist file
        QFile temp_derived_file(temp_derived_node_file_path);
        if(temp_derived_file.exists())
            temp_derived_file.remove();

        QFile temp_parent_file(temp_parent_node_file_path);
        if(temp_parent_file.exists())
            temp_parent_file.remove();

        apfs_container_and_derived_disk_node_list << obj.parent_node << obj.derived_into_node;

        objlist.append(obj);
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return objlist;
}

void image_mounter::slot_prc_generic_with_argument_mergechannel_readyread()
{
    merge_channel_prc_readall_array.clear();
    bool_is_it_installer_dmg = false;

    merge_channel_prc_readall_array = prc_generic_with_argument_mergechannel->readAll();
    if(!merge_channel_prc_readall_array.isEmpty())
    {
        merge_channel_prc_readall_array = merge_channel_prc_readall_array.trimmed();

        if(merge_channel_prc_readall_array.startsWith("Unlock"))
        {
            bool_is_it_installer_dmg = false;
        }

        else if(!merge_channel_prc_readall_array.startsWith("/dev/disk"))
        {
            bool_is_it_installer_dmg = true;
        }
    }
}

QString image_mounter::get_snapshot_label_for_big_sur(QString filepath)
{
    recon_static_functions::app_debug("start", Q_FUNC_INFO);

    QString snapshot_label_str;
    QFile m_file(filepath);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(" File open ---FAILED---  " + m_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:  " + m_file.errorString(), Q_FUNC_INFO);
        return snapshot_label_str;
    }

    while (!m_file.atEnd())
    {
        QString line_str = m_file.readLine().trimmed();
        if(line_str.startsWith("Snapshot:"))
        {
            line_str.replace("Snapshot:", "");
            snapshot_label_str = line_str.trimmed();
            break;
        }
    }
    m_file.close();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return  snapshot_label_str;
}
