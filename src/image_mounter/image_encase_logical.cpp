#include "image_mounter.h"

int image_mounter::mount_encase_logical_images()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QString file_type = struct_image_info_obj.image_format_type;
    int value = 0;
    if(file_type == MACRO_Target_Disk_ImageType_ENCASE_LOGICAL)
    {
        value = mount_encase_logical_l01();
    }


    recon_static_functions::app_debug(" end ",Q_FUNC_INFO);
    return value;
}

int image_mounter::mount_encase_logical_l01()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString mount_point_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString).toString();

    QString available_filename =  recon_static_functions::get_available_filename(("RECON_MNT_" + struct_image_info_obj.image_name_1),mount_point_path, Q_FUNC_INFO);
    QString mount_point    =  mount_point_path + available_filename;

    QDir dir;
    if(!dir.mkpath(mount_point))
    {
        recon_static_functions::app_debug(" mount point creation ---FAILED--- for " + struct_image_info_obj.image_path_1,Q_FUNC_INFO);
        recon_static_functions::app_debug(" mount point  " + mount_point,Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING, "Failed to mount [ERROR-" + QString::number(enum_global_ERROR_1011_Mount_Point_Creation_Encase_Logical_Failed) + "]");
        return -1;
    }

    //ewfmount -f files image.L01 mount_point
    QStringList args_fuse;
    args_fuse << "-f" << "files" << "-X" << "allow_other" << struct_image_info_obj.image_path_1 << mount_point;
    int value = run_fuse_mount(enum_fuse_type_ewfmount,args_fuse);
    if(value == -1)
        return value;


    struct_global_image_mounter_partition_info part_obj;
    recon_static_functions::clear_variables_image_mounting_partiton_info(part_obj);

    part_obj.mount_path  = mount_point;
    part_obj.parent_name = struct_image_info_obj.image_name_1;


    QList<struct_global_image_mounter_partition_info> tmp_part_list;
    tmp_part_list.append(part_obj);

    struct_image_info_obj.list_struct_partition_info = tmp_part_list;

    list_struct_image_info.append(struct_image_info_obj);


    recon_static_functions::app_debug(" end " + QString::number(value),Q_FUNC_INFO);
    return value;
}
