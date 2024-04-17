#include "image_mounter.h"

int image_mounter::mount_time_machine_backup_dmg()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    // hdiutil attach -readonly -nomount -noautofsck -noverify image_path -shadow ~/tmp/abc.shadow;
    QStringList args_hdiutil;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" << "-noverify"  << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument_mergechannel("hdiutil",args_hdiutil);

    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    int value = mount_time_machine_img_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_time_machine_img_after_hdiutil(QString out_hdiutil)
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
