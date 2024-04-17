#include "image_mounter.h"

int image_mounter::mount_forensic_images()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QString file_type = struct_image_info_obj.image_format_type;

    int value = 0;
    if(file_type == MACRO_Target_Disk_ImageType_Apple)
    {
        value = mount_forensic_dmg();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL)
    {
        value = mount_forensic_encase_physical();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_RAW_FULL)
    {
        value = mount_forensic_raw_full();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_RAW_SPLIT)
    {
        value = mount_forensic_raw_split();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_VMDK)
    {
        value = mount_forensic_vmdk();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_VHD)
    {
        value = mount_forensic_vhd();
    }
    else if(file_type == MACRO_Target_Disk_ImageType_AFF4)
    {
        value = mount_forensic_aff4();
    }



    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;
}

int image_mounter::mount_forensic_dmg()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    // hdiutil attach -readonly -nomount -noautofsck -noverify image_path -shadow ~/tmp/abc.shadow;
    QStringList args_hdiutil;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" << "-noverify"  << struct_image_info_obj.image_path_1;

    //QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    QString out_hdiutil = run_command_with_argument_mergechannel("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    int value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}
int image_mounter::mount_forensic_encase_physical()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_ewfmount,args_fuse);
    if(value == -1)
        return value;

    // "attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage  (img_obj.intermediate_mount_point + "/ewf1") -shadow  shadow_file_path;
    QStringList args_hdiutil;
    //args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/ewf1") << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/ewf1");

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);

    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_forensic_raw_full()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    // attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage  image_path -shadow shadow_file_path;
    QStringList args_hdiutil;
    //args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_1 << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    int value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_forensic_raw_split()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_smrawmount,args_fuse);
    if(value == -1)
    {
        return value;
    }

    QStringList args_hdiutil;
    // attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/smraw1");

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}


int image_mounter::mount_forensic_after_hdiutil(QString out_hdiutil)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    if(out_hdiutil.trimmed().isEmpty())
    {
        recon_static_functions::app_debug(" ---FAILED--- hdiutil output is blank ",Q_FUNC_INFO);
    }
    recon_static_functions::debug_intensive(" out_hdiutil == " + out_hdiutil,Q_FUNC_INFO);

    struct_global_image_mounter_image_info obj = extract_disk_info(out_hdiutil);
    struct_image_info_obj.disk_identifier_1 = obj.disk_identifier_1;
    struct_image_info_obj.disk_node_1 = obj.disk_node_1;
    struct_image_info_obj.list_struct_partition_info = obj.list_struct_partition_info;

    if(obj.list_struct_partition_info.size() < 1)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,  "No mountable Filesystem found on Image.");
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


    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_forensic_vmdk()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_vmdkmount,args_fuse);
    if(value == -1)
        return value;

    ///mounting final raw image
    // attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage  image_path -shadow shadow_file_path;
    QString raw_image_path = struct_image_info_obj.osx_fuse_mount_point_1 + "vmdk1";
    QStringList args_hdiutil;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << raw_image_path;

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_forensic_vhd()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << struct_image_info_obj.osx_fuse_mount_point_1;
    int value = run_fuse_mount(enum_fuse_type_vhdimount,args_fuse);
    if(value == -1)
        return value;

    ///mounting final raw image
    // attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage  image_path -shadow shadow_file_path;
    QString raw_image_path = struct_image_info_obj.osx_fuse_mount_point_1 + "vhdi1";
    QStringList args_hdiutil;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << raw_image_path;

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_forensic_aff4()
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    QStringList args_fuse;
    args_fuse << struct_image_info_obj.osx_fuse_mount_point_1 << struct_image_info_obj.image_path_1;
    int value = run_fuse_mount(enum_fuse_type_aff4fuse,args_fuse);
    if(value == -1)
        return value;

    // "attach -readonly -nomount -noautofsck -imagekey diskimage-class=CRawDiskImage  (img_obj.intermediate_mount_point + "/ewf1") -shadow  shadow_file_path;
    QStringList args_hdiutil;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" <<  "-imagekey" <<  "diskimage-class=CRawDiskImage" << (struct_image_info_obj.osx_fuse_mount_point_1 + "/aff4.raw");

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);

    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug("end " + QString::number(value),Q_FUNC_INFO);
    return value;
}


int image_mounter::check_mounting_status(QMap<QString, int> mmap)
{
    for(int i = 0; i < mmap.size(); i++)
    {
        // value having -2,-3,-4 are not considered for check yet now
        if(mmap.value(mmap.key(i)) == -1 || mmap.value(mmap.key(i)) == -5)
        {
            return -1;
        }
    }

    return 0;
}

