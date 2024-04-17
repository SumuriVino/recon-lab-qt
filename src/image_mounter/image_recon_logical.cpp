#include "image_mounter.h"

int image_mounter::mount_rcn_logical_images()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QString file_type = struct_image_info_obj.image_format_type;

    int value = 0;
    if(file_type == MACRO_Target_Disk_ImageType_Apple)
    {
        value = mount_rcn_logical_dmg_sparseimage();
    }


    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);

    return value;

}

int image_mounter::mount_rcn_logical_dmg_sparseimage()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    // hdiutil attach -readonly -nomount -noautofsck -noverify image_path -shadow ~/tmp/abc.shadow;
    QStringList args_hdiutil;
    //args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" << "-noverify"  << struct_image_info_obj.image_path_1 << "-shadow" << struct_image_info_obj.shadow_file_path_1;
    args_hdiutil << "attach" << "-readonly" << "-nomount" <<  "-noautofsck" << "-noverify"  << struct_image_info_obj.image_path_1;

    QString out_hdiutil = run_command_with_argument("hdiutil",args_hdiutil);
    out_hdiutil = check_for_fdisk_mounting_scheme(out_hdiutil, args_hdiutil);
    int value = mount_forensic_after_hdiutil(out_hdiutil);

    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;

}

