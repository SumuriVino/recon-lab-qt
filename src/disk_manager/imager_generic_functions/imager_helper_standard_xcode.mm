#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#include "imager_helper_standard.h"

QList<struct_disk_info> imager_helper_standard::extract_all_disk_identifier()
{ // extract all the disk identifiers from system and prepare list
    QProcess::execute("killall diskutil");

    list_disk_full_info.clear();
    image_devices_skipped_list.clear();
    QStringList args;
    args << "list" << "-plist";

    NSString *m_output = run_command_with_arguments("diskutil", args, Q_FUNC_INFO).toNSString();
    NSDictionary *m_plist = [NSPropertyListSerialization
            propertyListWithData:[m_output dataUsingEncoding:NSUTF8StringEncoding]
            options:kNilOptions
            format:NULL
            error:NULL];

    if(m_plist == nil)
    {
        return list_disk_full_info;
    }

    NSString *AllDisks_label = @"AllDisks";
    NSArray *AllDisks_label_arr    = [m_plist objectForKey:AllDisks_label];
    NSObject* my_object = [m_plist objectForKey:AllDisks_label];

    if(AllDisks_label_arr == nil || ![my_object isKindOfClass:[NSArray class]])
    {

        return list_disk_full_info;
    }

    for(NSUInteger ii = 0; ii < [AllDisks_label_arr count]; ii++)
    {
        NSString *identifier_value = [AllDisks_label_arr objectAtIndex:ii];

        struct_disk_info st_obj;

        clear_sources_struct_obj(st_obj);

        st_obj.device_identifier = QString::fromNSString(identifier_value);

        QStringList args_part;
        args_part << "info" << "-plist" << QString::fromNSString(identifier_value);

        NSString *part_output = run_command_with_arguments("diskutil", args_part, Q_FUNC_INFO).toNSString();

        //NSLog(@"=====Disk:%@", part_output);

        NSDictionary *part_plist = [NSPropertyListSerialization
                propertyListWithData:[part_output dataUsingEncoding:NSUTF8StringEncoding]
                options:kNilOptions
                format:NULL
                error:NULL];

        if(part_plist == nil)
        {
            list_disk_full_info.append(st_obj);
            continue;
        }

        ///==========Skip Image Physical disk and its partitions-Start========///
        NSString *MediaName_label = @"MediaName";
        QString MediaName_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(MediaName_label, part_plist, Q_FUNC_INFO);

        if(MediaName_value == QString("Disk Image"))
        {
            //skip image files(Do not append in main disklist)
            image_devices_skipped_list << st_obj.device_identifier;
            continue;
        }
        st_obj.model_MediaName = MediaName_value;


        NSString *parent_whole_disk_label = @"ParentWholeDisk";
        QString parent_whole_disk_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(parent_whole_disk_label, part_plist, Q_FUNC_INFO);
        if(image_devices_skipped_list.contains(parent_whole_disk_value))
        {
            //skip image partitions(Do not append in main disklist)
            continue;
        }

        st_obj.parent_whole_disk = parent_whole_disk_value;
        ///==========Skip Image Physical disk and its partitions-End========///


        NSString *MountPoint_label = @"MountPoint";
        QString MountPoint_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(MountPoint_label, part_plist, Q_FUNC_INFO);
        st_obj.mount_point_MountPoint = MountPoint_value;
        st_obj.is_mounted = false;
        if(!MountPoint_value.trimmed().isEmpty())
        {
            QString mount_mode = QString(mount_mode_ReadOnly);
            NSString *Writable_label = @"Writable";
            qint64 Writable_value = imager_static_functions::get_nsnumber_values_qint64(Writable_label, part_plist, Q_FUNC_INFO);
            if(Writable_value == 1)
            {
                mount_mode = QString(mount_mode_ReadWrite);
            }
            st_obj.mode = mount_mode;
            st_obj.is_mounted = true;
        }

        NSString *FilesystemType_label = @"FilesystemType";
        QString FilesystemType_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(FilesystemType_label, part_plist, Q_FUNC_INFO);
        st_obj.filesystem_FilesystemType = FilesystemType_value;

        NSString *Content_label = @"Content";
        QString Content_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(Content_label, part_plist, Q_FUNC_INFO);
        st_obj.type_Content = Content_value;

        NSString *VolumeName_label = @"VolumeName";
        QString VolumeName_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(VolumeName_label, part_plist, Q_FUNC_INFO);
        st_obj.name_VolumeName = VolumeName_value;

        NSString *TotalSize_label = @"TotalSize";
        qint64 TotalSize_value = imager_static_functions::get_nsnumber_values_qint64(TotalSize_label, part_plist, Q_FUNC_INFO);
        st_obj.total_size_qint64_TotalSize = TotalSize_value;
        st_obj.total_size_human_readable = imager_static_functions::human_readable_size(TotalSize_value, Q_FUNC_INFO);

        NSString *FreeSpace_label = @"FreeSpace";
        qint64 FreeSpace_value = imager_static_functions::get_nsnumber_values_qint64(FreeSpace_label, part_plist, Q_FUNC_INFO);
        st_obj.free_space_qint64_FreeSpace = FreeSpace_value;

        st_obj.is_internal_Internal = false;
        NSString *Internal_label = @"Internal";
        qint64 Internal_value = imager_static_functions::get_nsnumber_values_qint64(Internal_label, part_plist, Q_FUNC_INFO);
        if(Internal_value == 1)
        {
            st_obj.is_internal_Internal = true;
        }

        QStringList derived_from_list;
        NSString *CoreStoragePVs_label = @"CoreStoragePVs";
        NSArray *CoreStoragePVs_arr    = [part_plist objectForKey:CoreStoragePVs_label];
        my_object = [part_plist objectForKey:CoreStoragePVs_label];
        for(NSUInteger tt = 0; tt < [CoreStoragePVs_arr count]; tt++)
        {

            NSDictionary *pvs_dict = [CoreStoragePVs_arr objectAtIndex:tt];
            my_object = [CoreStoragePVs_arr objectAtIndex:tt];
            if(pvs_dict != nil && [my_object isKindOfClass:[NSDictionary class]])
            {
                NSString *CoreStoragePVDisk_label = @"CoreStoragePVDisk";
                QString CoreStoragePVDisk_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(CoreStoragePVDisk_label, pvs_dict, Q_FUNC_INFO);
                derived_from_list << CoreStoragePVDisk_value;

            }
        }
        st_obj.derived_from = derived_from_list.join(",");


        NSString *DiskUUID_label = @"DiskUUID";
        QString DiskUUID_value = imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(DiskUUID_label, part_plist, Q_FUNC_INFO);
        st_obj.disk_uuid = DiskUUID_value;



        st_obj.is_whole_disk = false;
        NSString *whole_disk_label = @"WholeDisk";
        qint64 whole_disk_value = imager_static_functions::get_nsnumber_values_qint64(whole_disk_label, part_plist, Q_FUNC_INFO);
        if(whole_disk_value == 1)
        {
            st_obj.is_whole_disk = true;
        }

        if(st_obj.filesystem_FilesystemType == "apfs")
            parse_apfs_device_details(st_obj);

        list_disk_full_info.append(st_obj);
    }


    update_offline_device_details_in_main_disklist();


    //    NSString *AllDisksAndPartitions_label = @"AllDisksAndPartitions";
    //    NSArray *AllDisksAndPartitions_arr    = [m_plist objectForKey:AllDisksAndPartitions_label];
    //    NSObject* my_object = [m_plist objectForKey:AllDisksAndPartitions_label];

    //    if(AllDisksAndPartitions_arr == nil || ![my_object isKindOfClass:[NSArray class]])
    //    {

    //        return mobj;
    //    }


    //    for(int ii = 0; ii < [AllDisksAndPartitions_arr count]; ii++)
    //    {
    //        NSDictionary *single_disk_dict = [AllDisksAndPartitions_arr objectAtIndex:ii];
    //        my_object = [AllDisksAndPartitions_arr objectAtIndex:ii];
    //        if(single_disk_dict == nil || ![my_object isKindOfClass:[NSDictionary class]])
    //        {
    //            continue;
    //        }

    //        struct_disk_info struct_disk_info_obj;

    //        NSString *disk_identifier_label = @"DeviceIdentifier";
    //        QString disk_identifier_value = recon_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(disk_identifier_label, single_disk_dict, Q_FUNC_INFO);

    //        struct_disk_info_obj.device_identifier = disk_identifier_value;

    //        NSString *Partitions_label = @"Partitions";
    //        NSArray *Partitions_arr    = [single_disk_dict objectForKey:Partitions_label];
    //        my_object = [single_disk_dict objectForKey:Partitions_label];

    //        if(Partitions_arr != nil && [my_object isKindOfClass:[NSArray class]])
    //        {
    //            for(int pp = 0; pp < [Partitions_arr count]; pp++)
    //            {
    //                NSDictionary *single_part_dict = [Partitions_arr objectAtIndex:pp];
    //                my_object = [Partitions_arr objectAtIndex:pp];
    //                if(single_part_dict != nil && [my_object isKindOfClass:[NSDictionary class]])
    //                {
    //                    NSString *part_identifier_label = @"DeviceIdentifier";
    //                    QString part_identifier_value = recon_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(part_identifier_label, single_part_dict, Q_FUNC_INFO);

    //                }

    //            }

    //        }


    //        map_disk_info.insert(disk_identifier_value, struct_disk_info_obj);

    //    }

    return list_disk_full_info;
}

QList<struct_disk_info> imager_helper_standard::pub_device_list()
{ // return device lists
    return list_disk_full_info;
}

void imager_helper_standard::parse_apfs_device_details(struct_disk_info &st_obj)
{ // parse apfs device details
    QStringList args;
    args << "apfs" << "list" << "-plist";

    NSString *m_output = run_command_with_arguments("diskutil", args, Q_FUNC_INFO).toNSString();
    NSDictionary *m_plist = [NSPropertyListSerialization
            propertyListWithData:[m_output dataUsingEncoding:NSUTF8StringEncoding]
            options:kNilOptions
            format:NULL
            error:NULL];

    if(m_plist == nil)
        return;

    NSString *AllDisks_label = @"Containers";
    NSArray *AllDisks_label_arr    = [m_plist objectForKey:AllDisks_label];
    NSObject* my_object = [m_plist objectForKey:AllDisks_label];

    if(AllDisks_label_arr == nil || ![my_object isKindOfClass:[NSArray class]])
    {
        return ;
    }

    //NSLog(@"====APFS==:%@", AllDisks_label_arr);

    for(NSDictionary *container_dict in AllDisks_label_arr)
    {
        // APFSContainerUUID
        NSString *apfs_container_uuid_label = @"APFSContainerUUID";
        NSString *apfs_container_uuid_value = [container_dict objectForKey:apfs_container_uuid_label];


        //Fusion
        NSString *apfs_fusion_label = @"Fusion";
        NSNumber *apfs_fusion_value = [container_dict objectForKey:apfs_fusion_label];

        bool is_fusion_disk = false;
        if([apfs_fusion_value doubleValue] == 1)
            is_fusion_disk = true;


        // Containers
        NSString *PhysicalStores_label  = @"PhysicalStores";
        NSArray  *PhysicalStores_array = [container_dict objectForKey:PhysicalStores_label];

        QString apfs_physical_device_id_str;
        QString apfs_physical_device_uuid_str;
        for(NSDictionary *PhysicalStores_dict in PhysicalStores_array)
        {
            // DeviceIdentifier
            NSString *apfs_physical_store_device_id_label = @"DeviceIdentifier";
            NSString *apfs_physical_store_device_id_value = [PhysicalStores_dict objectForKey:apfs_physical_store_device_id_label];
            apfs_physical_device_id_str = QString::fromNSString(apfs_physical_store_device_id_value);

            // DiskUUID
            NSString *apfs_physical_store_disk_uuid_label = @"DiskUUID";
            NSString *apfs_physical_store_disk_uuid_value = [PhysicalStores_dict objectForKey:apfs_physical_store_disk_uuid_label];
            apfs_physical_device_uuid_str = QString::fromNSString(apfs_physical_store_disk_uuid_value);
        }


        // Volumes
        NSString *Volumes_label  = @"Volumes";
        NSArray  *Volumes_array  = [container_dict objectForKey:Volumes_label];

        for(NSDictionary *Volumes_dict in Volumes_array)
        {

            // DeviceIdentifier
            NSString *DeviceIdentifier_label = @"DeviceIdentifier";
            NSString *DeviceIdentifier_value = [Volumes_dict objectForKey:DeviceIdentifier_label];
            QString  apfs_volume_id_str = QString::fromNSString(DeviceIdentifier_value);

            if(st_obj.device_identifier == apfs_volume_id_str && !apfs_volume_id_str.trimmed().isEmpty())
            {

                // APFSVolumeUUID
                NSString *APFSVolumeUUID_label = @"APFSVolumeUUID";
                NSString *APFSVolumeUUID_value = [Volumes_dict objectForKey:APFSVolumeUUID_label];

                // Encryption
                NSString *Encryption_label = @"Encryption";
                NSNumber *apfs_encryption_value = [Volumes_dict objectForKey:Encryption_label];

                bool is_encrypted_disk = false;
                if([apfs_encryption_value doubleValue] == 1)
                    is_encrypted_disk = true;

                // FileVault
                NSString *FileVault_label = @"FileVault";
                NSNumber *apfs_filevault_value = [Volumes_dict objectForKey:FileVault_label];

                bool is_filevault_disk = false;
                if([apfs_filevault_value doubleValue] == 1)
                    is_filevault_disk = true;

                // Locked
                NSString *Locked_label = @"Locked";
                NSNumber *apfs_locked_value = [Volumes_dict objectForKey:Locked_label];

                bool is_locked_disk = false;
                if([apfs_locked_value doubleValue] == 1)
                    is_locked_disk = true;

                // Name
                NSString *Name_label = @"Name";
                NSString *Name_value = [Volumes_dict objectForKey:Name_label];

                // Roles
                NSString *Roles_label = @"Roles";
                NSArray  *Roles_value_arr = [Volumes_dict objectForKey:Roles_label];
                for(NSString *roles_str in Roles_value_arr)
                    st_obj.apfs_volume_roles_list << QString::fromNSString(roles_str);


                st_obj.apfs_physical_container_uuid = QString::fromNSString(apfs_container_uuid_value);
                st_obj.apfs_physical_device_id   = apfs_physical_device_id_str;
                st_obj.derived_from = apfs_physical_device_id_str;
                st_obj.apfs_physical_device_uuid = apfs_physical_device_uuid_str;
                st_obj.apfs_bool_is_physical_fusion = is_fusion_disk;

                st_obj.apfs_volume_uuid = QString::fromNSString(APFSVolumeUUID_value);
                st_obj.apfs_volume_id   = QString::fromNSString(DeviceIdentifier_value);
                st_obj.apfs_bool_is_encrypt  = is_encrypted_disk;
                st_obj.apfs_bool_is_filevault = is_filevault_disk;
                st_obj.apfs_bool_is_locked = is_locked_disk;
                st_obj.apfs_volume_label = QString::fromNSString(Name_value);

                st_obj.bool_is_encrypt = is_encrypted_disk;
                st_obj.bool_is_locked  = is_locked_disk;

                break;
            }
        }
    }

}

void imager_helper_standard::update_offline_device_details_in_main_disklist()
{ // update offline device details in disk list
    QStringList args;
    args << "list";

    QString output_str = run_command_with_arguments("diskutil", args, Q_FUNC_INFO);

    QStringList line_split_list = output_str.split("\n");

    for(int count = 0; count < line_split_list.size(); count++)
    {
        QString line_str = line_split_list.at(count).trimmed();
        if(line_str.contains(QString("Logical Volume on ")) || (line_str.contains(QString("Logical Volume ")) && !line_str.contains(QString("Logical Volume on "))))
        {
            int lgcl_vlum_index = count;

            QString logical_volume_dev_str = line_split_list.at(lgcl_vlum_index).trimmed();
            QStringList tmp_split_list = logical_volume_dev_str.split(" on ");
            logical_volume_dev_str = tmp_split_list.at(tmp_split_list.size() -1).trimmed();

            if(!logical_volume_dev_str.isEmpty() && logical_volume_dev_str.startsWith("disk"))
            {
                QString line_volume_uuid;
                if(line_split_list.size() > (lgcl_vlum_index + 1))
                    line_volume_uuid = line_split_list.at(lgcl_vlum_index + 1).trimmed();

                QString line_volume_locked;
                if(line_split_list.size() > (lgcl_vlum_index + 2))
                    line_volume_locked = line_split_list.at(lgcl_vlum_index + 2).trimmed();

                bool is_locked = false;
                bool is_encrypted = false;
                if(line_volume_locked == QString("Locked Encrypted"))
                {
                    is_locked    = true;
                    is_encrypted = true;
                }
                else if(line_volume_locked == QString("Unlocked Encrypted"))
                {
                    is_locked    = false;
                    is_encrypted = true;
                }

                for(int count = 0; count < list_disk_full_info.size(); count++)
                {
                    struct_disk_info disk_info_obj = list_disk_full_info.takeAt(count);
                    if(disk_info_obj.device_identifier == logical_volume_dev_str)
                    {
                        disk_info_obj.disk_uuid = line_volume_uuid;
                        disk_info_obj.bool_is_encrypt = is_encrypted;
                        disk_info_obj.bool_is_locked  = is_locked;

                        list_disk_full_info.insert(count, disk_info_obj);
                        break;
                    }
                    list_disk_full_info.insert(count, disk_info_obj);
                }
            }
        }
    }
}

void::imager_helper_standard::clear_sources_struct_obj(struct_disk_info &st_obj)
{ // clear structure
    st_obj.device_identifier.clear();
    st_obj.model_MediaName.clear();
    st_obj.total_size_qint64_TotalSize = 0;
    st_obj.total_size_human_readable.clear();
    st_obj.type_Content.clear();
    st_obj.name_VolumeName.clear();
    st_obj.filesystem_FilesystemType.clear();
    st_obj.derived_from.clear();
    st_obj.mode.clear();
    st_obj.mount_point_MountPoint.clear();
    st_obj.is_internal_Internal = false;
    st_obj.free_space_qint64_FreeSpace = 0;
    st_obj.is_mounted = false;

    st_obj.disk_uuid.clear();
    st_obj.bool_is_encrypt = false;
    st_obj.bool_is_locked  = false;

    st_obj.is_whole_disk = false;
    st_obj.parent_whole_disk.clear();

    st_obj.apfs_physical_container_uuid.clear();
    st_obj.apfs_physical_device_id.clear();
    st_obj.apfs_physical_device_uuid.clear();
    st_obj.apfs_bool_is_physical_fusion = false;

    st_obj.apfs_volume_id.clear();
    st_obj.apfs_volume_uuid.clear();
    st_obj.apfs_volume_label.clear();

    st_obj.apfs_volume_roles_list.clear();

    st_obj.apfs_bool_is_encrypt   = false;
    st_obj.apfs_bool_is_filevault = false;
    st_obj.apfs_bool_is_locked    = false;
}

