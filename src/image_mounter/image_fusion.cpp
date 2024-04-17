#include "image_mounter.h"

int image_mounter::mount_fusion_images()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QString file_type = struct_image_info_obj.image_format_type;

    int value = 0;
    if(file_type == MACRO_Target_Disk_ImageType_Apple)
    {
        value = mount_fusion_dmg();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL)
    {
        value = mount_fusion_encase();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_RAW_FULL)
    {
        value = mount_fusion_raw_full();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_RAW_SPLIT)
    {
        value = mount_fusion_raw_split();
    }

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}
int image_mounter::mount_fusion_dmg()
{
    recon_static_functions::app_debug(" stqrt",Q_FUNC_INFO);

    // hdiutil attach -readonly -nomount -noverify -imagekey diskimage-class=CRawDiskImage
    QStringList ssd_args;
    ssd_args  << "attach" << "-readonly" << "-nomount" << "-noverify" << "-imagekey" << "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument("hdiutil",ssd_args);
    out_hdiutil = out_hdiutil.trimmed();

    struct_global_image_mounter_image_info obj1 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_1 = obj1.disk_identifier_1;
    struct_image_info_obj.disk_node_1 = obj1.disk_node_1;



    QStringList platter_args;
    platter_args  << "attach" << "-readonly" << "-nomount" << "-noverify" << "-imagekey" << "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_2;

    out_hdiutil = run_command_with_argument("hdiutil",platter_args);
    out_hdiutil = out_hdiutil.trimmed();

    struct_global_image_mounter_image_info obj2 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_2 = obj2.disk_identifier_1;
    struct_image_info_obj.disk_node_2 = obj2.disk_node_1;


    int value = mount_fusion_after_hdiutil(obj1);


    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}

int image_mounter:: mount_fusion_encase()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_ewfmount,args_fuse);
    if(value == -1)
        return value;

    QString fs_mnt_pth1 = struct_image_info_obj.osx_fuse_mount_point_1;
    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(fs_mnt_pth1, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("FUSE Mount SSD ---FAILED--- " + struct_image_info_obj.osx_fuse_mount_point_1, Q_FUNC_INFO);
        return -1;
    }

    QStringList ssd_args;
    // hdiutil attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage
    ssd_args << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/ewf1");
    QString out_hdiutil = run_command_with_argument("hdiutil",ssd_args);

    struct_global_image_mounter_image_info obj1 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_1 = obj1.disk_identifier_1;
    struct_image_info_obj.disk_node_1 = obj1.disk_node_1;


    QStringList args_fuse2;
    args_fuse2 << "-X" << "allow_other" << struct_image_info_obj.image_path_2 << struct_image_info_obj.osx_fuse_mount_point_2;
    value = run_fuse_mount(enum_fuse_type_ewfmount,args_fuse2);
    if(value == -1)
        return value;

    QString fs_mnt_pth2 = struct_image_info_obj.osx_fuse_mount_point_2;
    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(fs_mnt_pth2, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("FUSE Mount platter ---FAILED--- " + struct_image_info_obj.osx_fuse_mount_point_2, Q_FUNC_INFO);
        return -1;
    }

    QStringList platter_args;
    platter_args << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_2 + "/ewf1");
    out_hdiutil = run_command_with_argument("hdiutil",platter_args);


    struct_global_image_mounter_image_info obj2 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_2 = obj2.disk_identifier_1;
    struct_image_info_obj.disk_node_2 = obj2.disk_node_1;


    value = mount_fusion_after_hdiutil(obj1);
    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}

int image_mounter::mount_fusion_raw_full()
{
    recon_static_functions::app_debug(" stqrt",Q_FUNC_INFO);

    // hdiutil attach -readonly -nomount -noverify -imagekey diskimage-class=CRawDiskImage
    QStringList ssd_args;
    ssd_args  << "attach" << "-readonly" << "-nomount" << "-noverify" << "-imagekey" << "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument("hdiutil",ssd_args);
    out_hdiutil = out_hdiutil.trimmed();

    struct_global_image_mounter_image_info obj1 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_1 = obj1.disk_identifier_1;
    struct_image_info_obj.disk_node_1 = obj1.disk_node_1;


    QStringList platter_args;
    platter_args  << "attach" << "-readonly" << "-nomount" << "-noverify" << "-imagekey" << "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_2;

    out_hdiutil = run_command_with_argument("hdiutil",platter_args);
    out_hdiutil = out_hdiutil.trimmed();


    struct_global_image_mounter_image_info obj2 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_2 = obj2.disk_identifier_1;
    struct_image_info_obj.disk_node_2 = obj2.disk_node_1;

    int value = mount_fusion_after_hdiutil(obj1);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}

int image_mounter::mount_fusion_raw_split()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);


    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_smrawmount,args_fuse);
    if(value == -1)
        return value;

    QString fs_mnt_pth1 = struct_image_info_obj.osx_fuse_mount_point_1;
    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(fs_mnt_pth1, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("FUSE Mount SSD ---FAILED--- " + struct_image_info_obj.osx_fuse_mount_point_1, Q_FUNC_INFO);
        return -1;
    }

    QStringList ssd_args;
    // hdiutil attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage
    ssd_args << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/smraw1");
    QString out_hdiutil = run_command_with_argument("hdiutil",ssd_args);

    struct_global_image_mounter_image_info obj1 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_1 = obj1.disk_identifier_1;
    struct_image_info_obj.disk_node_1 = obj1.disk_node_1;


    QStringList args_fuse2;
    args_fuse2 << "-X" << "allow_other" << struct_image_info_obj.image_path_2 << struct_image_info_obj.osx_fuse_mount_point_2;
    value = run_fuse_mount(enum_fuse_type_smrawmount,args_fuse2);
    if(value == -1)
        return value;


    QString fs_mnt_pth2 = struct_image_info_obj.osx_fuse_mount_point_2;
    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(fs_mnt_pth2, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("FUSE Mount platter ---FAILED--- " + struct_image_info_obj.osx_fuse_mount_point_2, Q_FUNC_INFO);
        return -1;
    }

    QStringList platter_args;
    platter_args << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_2 + "/smraw1");
    out_hdiutil = run_command_with_argument("hdiutil",platter_args);



    struct_global_image_mounter_image_info obj2 = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_2 = obj2.disk_identifier_1;
    struct_image_info_obj.disk_node_2 = obj2.disk_node_1;


    value = mount_fusion_after_hdiutil(obj1);
    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}

int image_mounter::mount_fusion_after_hdiutil(struct_global_image_mounter_image_info obj_ssd)
{
    QString out_diskutillist = run_command_with_argument("diskutil",QStringList("list"));
    out_diskutillist = out_diskutillist.trimmed();

    recon_static_functions::debug_intensive(out_diskutillist, Q_FUNC_INFO);

    //  fill structure with disk node
    QList <struct_global_image_mounter_partition_info> list_struct_partition_info;
    if(obj_ssd.list_struct_partition_info.size() > 0)
    {
        for(int tt = 0; tt < obj_ssd.list_struct_partition_info.size(); tt++)
        {
            struct_global_image_mounter_partition_info tmp_obj = obj_ssd.list_struct_partition_info.at(tt);
            tmp_obj.derived_into_uuid = extract_cs_volume_uuid_from_diskutillist_output(out_diskutillist,tmp_obj.parent_identifier);
            QString disk_node = extract_derived_into_disknode_from_diskutillist_output(out_diskutillist,tmp_obj.parent_identifier);


            if(!disk_node.trimmed().isEmpty())
            {
                tmp_obj.derived_into_node = disk_node;
                tmp_obj.derived_into_identifier = disk_node.remove("/dev/");

                /// Parse Volume Name for Derived Node
                tmp_obj.derived_into_name = get_value_of_key_from_diskutil_info_output(disk_node, "VolumeName", enum_Global_Plist_File_Key_Type_String);

                obj_ssd.list_struct_partition_info.removeAt(tt);
                obj_ssd.list_struct_partition_info.insert(tt,tmp_obj);
            }
        }
    }

    //  list_struct_partition_info << obj_platter.list_struct_partition_info;
    list_struct_partition_info << obj_ssd.list_struct_partition_info;

    struct_image_info_obj.list_struct_partition_info = list_struct_partition_info;

    if(struct_image_info_obj.list_struct_partition_info.size() < 1)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,  "No mountable Filesystem found on Image");
        //  message_dialog_object->show();
        return -1;
    }

    recon_static_functions::app_debug(" -----BELOW PRINTED INFO IS BEFORE IMAGE MOUNTING-----", Q_FUNC_INFO);
    QList<struct_global_image_mounter_image_info> tt_list;
    tt_list.append(struct_image_info_obj);
    recon_static_functions::print_image_mounting_info_on_terminal(tt_list, Q_FUNC_INFO);
    recon_static_functions::app_debug(" ------ABOVE PRINTED INFO IS BEFORE IMAGE MOUNTING------", Q_FUNC_INFO);

    QMap<QString, int> mmap = final_mounting(struct_image_info_obj.list_struct_partition_info);

    int value = check_mounting_status(mmap);

    return value;
}

