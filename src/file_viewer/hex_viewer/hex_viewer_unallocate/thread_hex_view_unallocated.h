#ifndef THREAD_HEX_VIEW_UNALLOCATED_H
#define THREAD_HEX_VIEW_UNALLOCATED_H

#include <QObject>
#include <QMessageBox>
#include <tsk/libtsk.h>
#include "narad_muni/narad_muni.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "witness_info/witness_info_manager/witness_info_manager.h"

struct struct_global_tsk_part_info_unalloc
{
    bool bool_partition_found;
    TSK_VS_PART_INFO *part_info_ptr;
    bool bool_fs_found;
    TSK_FS_INFO *fs_info_ptr;
    qint64 size;
    QString fs_name;
    QList<qint64> list_unalloc_block;
    qint64 current_sector;
};


class thread_hex_view_unallocated : public QThread
{
    Q_OBJECT

public:
    thread_hex_view_unallocated();
    ~thread_hex_view_unallocated();

    void run();

    void pub_set_essentials(QString source_count_str, bool bool_complete_src);
    void pub_set_cancel_extraction_status(bool status);

    QString pub_get_source_count_name();
    bool pub_get_is_complete_disk_status();
    QString pub_get_current_table_name();
signals:
    void signal_PBAR_thread_disk_hex_view_unallocated_started(QString job_type);
    void signal_PBAR_thread_disk_hex_view_unallocated_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_disk_hex_view_unallocated_finished(QString job_type);

private slots:

private:
    QString source_count_name;
    QString source_device_node_str;
    bool bool_is_complete_source = false;

    recon_helper_standard *recon_helper_standard_obj;
    bool bool_cancel_extraction = false;

    QString current_table_name;

    TSK_IMG_INFO *img_info_ptr = NULL;
    TSK_VS_INFO  *vs_info_ptr  = NULL;
    TSK_FS_INFO  *fs_info_ptr  = NULL;

    QString source_db_path;
    QSqlDatabase destination_db;
    QSqlQuery query_destination;
    QString command_insert;
    QByteArray block_data_array;
    struct_GLOBAL_witness_info_source struct_source_info;
    int tsk_start_image_processing(QString image_path_str);
    int tsk_process_volume_filesystem(TSK_IMG_INFO *img_info, TSK_OFF_T start);
    int process_multi_partitioned_unallocated_data();
    int get_unallocated_data_from_partition(TSK_FS_INFO *fs_info_ptr);
    void create_destination_table();
    void insert_destination_index();
    bool check_source_already_processed_completely();
};

#endif // THREAD_HEX_VIEW_UNALLOCATED_H
