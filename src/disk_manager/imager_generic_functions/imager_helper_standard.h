#ifndef IMAGER_HELPER_STANDARD_H
#define IMAGER_HELPER_STANDARD_H

#include <QWidget>
#include "imager_static_functions.h"

#include <QProcess>
#include <QtCore>
#include "disk_manager/imager_global_headers.h"


class imager_helper_standard : public QWidget
{
    Q_OBJECT

public:
    imager_helper_standard(QWidget *parent = 0);
    ~imager_helper_standard();

    QString run_command_with_arguments(QString command, QStringList arguments, QString caller_func);
    QString run_command_without_argument(QString command, QString caller_func);
    QString run_command_with_arguments_no_trimmed(QString command, QStringList arguments, QString caller_func);

    void pub_set_bool_process_generic(bool status);

    void pub_terminate_generic_process();
    qint64 pub_get_used_space_using_disk_node(QString disk_node);
    qint64 pub_get_network_mount_path_used_size(QString mount_path);

    QString pub_extract_device_mount_path_using_device_node(QString device_node, QList<struct_disk_info> list_disk_full_info);
    bool pub_write_data_to_file(QString a_data, QString file_path, int mode);
    bool pub_append_file_data_from_another_file(QString first_file_path, QString second_file_path);
    QString pub_get_mount_path_of_network_volume(QString network_path);
    QString pub_extract_disk_node_from_combobox_item_text(QString str);
    QList<network_share_info> pub_get_network_share_info();

    void pub_change_combobox_text_color(QComboBox &combobox, QList<struct_disk_info> list_disk_full_info);
    qint64 pub_get_used_size_using_mount_path(QString mount_path);
    QString get_human_readable_time(QString str, QString caller_func);


    QList<struct_disk_info> extract_all_disk_identifier();
    QList<struct_disk_info> pub_device_list();

    struct_disk_info get_device_struct_info_according_identifier(QString m_identifier);
    QString create_md5(QString file_path, QString caller_func);
    QString create_sha1(QString file_path, QString caller_func);

    QString extract_dev_rdisk_from_combobox_item_text(QString str);

    QString extract_dev_disk_node_from_combobox_item_text(QString str);

    QString extract_device_mount_path_using_device_node(QString device_node);

    QString extract_device_fs_name_using_device_node(QString device_node);

    QString extract_disk_node_from_combobox_item_text(QString str);

    bool write_data_to_file(QString a_data, QString file_path, int mode);

    bool append_file_data_from_another_file(QString first_file_path, QString second_file_path);

    QList<network_share_info> get_network_share_info();

    QString get_mount_path_of_network_volume(QString network_path);

    qint64 get_used_space_using_disk_node(QString disk_node);

    qint64 get_disk_physical_size(QString disk_node);

    QString recon_imger_data_partition_mount_path(QString other_device_mount_path);

    void pub_set_recon_imager_data_partition_mount_path(QString);

    QString pub_get_recon_imager_data_partition_mount_path(void);

    QString get_network_mount_path_acc_share_name(QString share_name, QString ip_address);

    void pub_set_recon_imager_root_disk(QString);

    QString pub_get_recon_imager_root_disk(void);


    qint64 get_network_mount_path_used_size(QString mount_path);

    bool pub_check_is_root_disk(struct_disk_info m_obj);
    QString pub_get_system_profiler_hardware_info();
    QString pub_get_system_profiler_software_info();

    struct_imager_hashes_with_details pub_create_md5_SHA1_with_details(QString file_path);
    QString pub_recon_imger_data_partition_mount_path_during_inital_phase();
    qint64 pub_write_date_into_file(QString file_path, QString our_data, QString caller_func);
    QStringList pub_get_catalina_system_and_data_disk_node_list(struct_disk_info disk_node_obj, QList<struct_disk_info> list_disk_full_info);
    bool pub_check_is_catalina_drive(struct_disk_info disk_node_obj, QList<struct_disk_info> list_disk_full_info);
    bool pub_source_other_than_system_and_data(QString d_node, QList<struct_disk_info> list_disk_full_info);
signals:

private slots:
    void slot_generic_process_finished(int);

private:

    QProcess *process_generic;
    bool bool_process_generic_finished;


    QList<struct_disk_info> list_disk_full_info;

    QStringList image_devices_skipped_list;

    void parse_apfs_device_details(struct_disk_info &st_obj);
    void update_offline_device_details_in_main_disklist();
    void clear_sources_struct_obj(struct_disk_info &st_obj);

    QString recon_imager_data_partition_mount_path;

    QString recon_imager_root_disk;

    QStringList get_apfs_users_id_list(QString disk_node_str);
};

#endif
