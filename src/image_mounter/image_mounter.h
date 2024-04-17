#ifndef IMAGE_MOUNTER_H
#define IMAGE_MOUNTER_H

#include <QObject>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"
#include <QFileDialog>
#include "header_identical_1.h"

#include "header_global_structures.h"
#include "message_box/message_dialog.h"
#include "header_global_variables_3.h"                         
#include "progress_bar/progress_bar_window.h"
#include "header_global_variables_4.h"

class image_mounter : public QDialog
{
    Q_OBJECT
public:
    explicit image_mounter(QWidget *parent = 0);
    ~image_mounter();

    void set_image_category(QString image_source_type_display, QString image_source_type_internal);
    int set_image_path(QString path1, QString path2, QString password);
    void unmount_image(QString image_path1);
    void unmount_all_image();

    void unmount_all_fuse();
    QStringList get_mount_path(QString image_path1);
    bool is_image_mounted(QString image_path1);


    QStringList get_all_mount_path();
    void set_progress_bar(progress_bar_window *obj);



    struct_global_image_mounter_image_info pub_get_struct_image_info_last_mounted();

    struct_global_image_mounter_image_info pub_get_struct_image_info_by_image_path(QString image_path);

    void set_list_image_info(QList<struct_global_image_mounter_image_info> list_info);
    void clear_list_image_info();


signals:
    void signal_statusbar_progress_bar_show_hide(bool show_hide, QString message);
    void signal_set_progress_message(QString message);

private slots:
    void slot_prc_generic_with_argument_finished(int);
    void slot_prc_generic_without_argument_finished(int);
    void slot_prc_generic_finished_with_timer(int);

    void slot_timer_timeout();

    void slot_prc_generic_with_argument_mergechannel_finished(int);
    void slot_prc_generic_with_argument_mergechannel_readyread();
private:

    enum enum_fusemount_type
    {
        enum_fuse_type_ewfmount = 0,
        enum_fuse_type_smrawmount,
        enum_fuse_type_vmdkmount,
        enum_fuse_type_vhdimount,
        enum_fuse_type_aff4fuse
    };


    QList<struct_global_image_mounter_image_info> list_struct_image_info;
    struct_global_image_mounter_image_info struct_image_info_obj;


    QString shadow_files_directory;

    bool bool_prc_generic_with_argument;
    QProcess *prc_generic_with_argument;
    bool bool_prc_generic_without_argument;
    QProcess *prc_generic_without_argument;
    bool bool_prc_generic_with_timer;
    QProcess *prc_generic_with_timer;

    bool bool_prc_generic_with_argument_mergechannel;
    QProcess *prc_generic_with_argument_mergechannel;

    QTimer *m_timer;
    bool bool_m_timer_finish;

    bool create_directory_structure();

    int mount_forensic_images();
    int mount_forensic_dmg();
    int mount_forensic_encase_physical();
    int mount_forensic_raw_full();
    int mount_forensic_raw_split();
    int mount_forensic_after_hdiutil(QString out_hdiutil);


    int mount_vault_images();
    int mount_vault_dmg_volume_encrypted();
    int mount_vault_dmg_diskutil_encrypted();
    int mount_vault_encase();
    int mount_vault_raw_full();
    int mount_vault_raw_split();
    int mount_vault_after_hdiutil(QString out_hdiutil);


    int mount_fusion_images();
    int mount_fusion_dmg();
    int mount_fusion_encase();
    int mount_fusion_raw_full();
    int mount_fusion_raw_split();
    int mount_fusion_after_hdiutil(struct_global_image_mounter_image_info obj_ssd);
    int mount_fusion_after_ssd_platter();

    int mount_recon_fs_block_images();
    int mount_recon_fs_block_sparseimages();
    int mount_recon_fs_block_image_after_hdiutil(QString out_hdiutil);

    int mount_rcn_logical_images();
    int mount_rcn_logical_dmg_sparseimage();

    int mount_encase_logical_images();
    int mount_encase_logical_l01();

    int mount_recon_mac_sharing_mode_images();
    int mount_recon_mac_sharing_mode_dmg_sparseimage();

    int mount_time_machine_backup_dmg();
    int mount_time_machine_img_after_hdiutil(QString out_hdiutil);


    struct_global_image_mounter_image_info extract_disk_info(QString out_hdiutil);

    QMap<QString, int> final_mounting(QList<struct_global_image_mounter_partition_info> list_image_partition_info);
    int check_mounting_status(QMap<QString, int> mmap);


    QList<struct_global_image_mounter_partition_info> parse_other_partition_info_from_hdiutil_output(QString out_put);
    QString parse_disk_node_from_hdiutil_output(QString out_put);
    QString extract_name_from_node(QString disk_node);

    QList <struct_global_image_mounter_partition_info> parse_corestorage_partition_info_from_hdiutil_output(QString out_put);
    QStringList extract_corestorage_volume_identifier_list_from_hdiutil_output(QString out_put);
    QString extract_derived_into_disknode_from_diskutillist_output(QString out_put, QString cs_volumename);
    QString extract_cs_volume_uuid_from_diskutillist_output(QString out_put, QString cs_volumename);


    QStringList extract_updated_volume_name_list(QStringList volume_list, QList <struct_global_image_mounter_partition_info> objlist);


    QString run_command_with_argument(QString command, QStringList arguments);
    QString run_command_with_argument_mergechannel(QString command, QStringList arguments);
    QString run_command_with_timer(QString command, QStringList arguments,int interval);
    QString run_command_without_argument(QString command);
    void check_for_os_fuse_installation();
    bool bool_osx_fuse_installed;

    void prepare_shadow_file_path();

    QString extract_fused_node(QString ssd_volume,QString platter_volume);
    QStringList extract_volume_uuid_list(QStringList lg_volume);
    QString extract_volume_uuid(QString lg_volume);

    QString extract_corestorage_disk(QString uuid);
    QString extract_encrypted_disk();
    void clear_fields();
    void turn_indexing_ON_OFF(QString value, QString mount_path);


    int run_fuse_mount(int fuse_type, QStringList m_argument);

    progress_bar_window *progress_bar_obj;

    message_dialog *message_dialog_object;
    QStringList get_derived_into_disknode_list(QList<struct_global_image_mounter_partition_info> objlist);

    QStringList core_storage_and_derived_disk_node_list;
    QStringList apfs_container_and_derived_disk_node_list;
    void fill_partition_info_structure_after_mounting();

    QString get_value_of_key_from_diskutil_info_output(QString identifier_name, QString key_name, int m_datatype);
    QStringList extract_apfs_parent_identifier_list_from_hdiutil_output(QString out_put);
    QList<struct_global_image_mounter_partition_info> parse_apfs_disk_partition_info_from_hdiutil_output(QString out_put);
    QList<struct_global_image_mounter_partition_info> parse_apfs_container_partition_info_from_hdiutil_output(QString out_put);
    void extract_n_fill_derived_apfs_partitions_info_from_diskutillist_output(QString diskutil_list_output, QString apfs_volumename, QList<struct_global_image_mounter_partition_info> &list_partition_info);
    QStringList get_encrypted_apfs_volume_disk_node();
    QString extract_apfs_disk(QString disk_node);

    QString os_version_full_running_machine;
    QString check_for_fdisk_mounting_scheme(QString hdiutil_output, QStringList hdiutil_args);
    QString get_volume_name_using_diskutil_info(QString device_node);
    QString get_apfs_container_identifier(QString diskutil_output_str, QString apfs_identifier_str);
    int mount_forensic_vmdk();
    int mount_forensic_vhd();
    int mount_forensic_aff4();

    int mount_optical_disc_images();
    int mount_optical_iso_cdr();

    QByteArray merge_channel_prc_readall_array;
    bool bool_is_it_installer_dmg;

    QString get_snapshot_label_for_big_sur(QString filepath);
};

#endif // IMAGE_MOUNTER_H
