#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include <QFrame>
#include "disk_manager/display_dialog_box/display_progress_dialog_box.h"
#include "disk_manager/Disk_Manager/Format/disk_manager_format_disk.h"
#include "disk_manager/Disk_Manager/FileVault/disk_manager_filevault_decrypt.h"
#include "disk_manager/imager_generic_functions/imager_static_functions.h"

#include "header_identical_1.h"
#include "header_global_variables_3.h"
#include "header_global_variables_4.h"

#include "QLabel"
#include "QProgressBar"

namespace Ui {
class disk_manager;
}

class disk_manager : public QFrame
{
    Q_OBJECT

public:
    explicit disk_manager(QWidget *parent = 0);
    ~disk_manager();

    void pub_display_data();
    void pub_start_disk_arbitration();
    void pub_stop_disk_arbitration();
    void pub_mount_read_only(QString device_node);
    void pub_mount_read_write(QString device_node);
    void pub_unmount_disk(QString device_node);
    void display_filevaulted_message_with_disk_name();
    void pub_display_data_with_progress_message();
    bool pub_get_disk_arbitration_started_status();
    void pub_set_disk_arbitration_label_status(QString status_str);
private slots:
    void on_tableWidget_disk_list_cellClicked(int row, int column);

    void on_pushButton_refresh_clicked();

    void on_pushButton_mount_read_clicked();
    void on_pushButton_mount_read_write_clicked();
    void on_pushButton_unmount_clicked();


    void on_pushButton_format_clicked();
    void slot_format_finished(bool status);

    void on_pushButton_decrypt_clicked();
    void slot_decrypt_finished(bool status);
    void slot_tdm_decrypt_timer_tick();
    void slot_pushbutton_tdm_cancel_clicked(bool status);
    void on_pushButton_arbitration_clicked();

private:
    Ui::disk_manager *ui;

    enum enum_imager_disk_info
    {
        enum_imager_serial_number,
        enum_imager_device,
        enum_imager_device_location,
        enum_imager_model,
        enum_imager_size,
        enum_imager_type,
        enum_imager_name,
        enum_imager_filesystem,
        enum_imager_derived_from,
        enum_imager_is_encrypted,
        enum_imager_encryption_status,
        enum_imager_mode,
        enum_imager_mount_path,
        enum_imager_blank
    };


    QList<struct_disk_info> list_all_disk_info;


    display_progress_dialog_box *display_dialog_box_obj_non_aborted;

    imager_helper_standard *recon_helper_standard_obj;

    disk_manager_format_disk *disk_manager_format_disk_obj;
    disk_manager_filevault_decrypt *disk_manager_decrypt_disk_obj;

    int tdm_redecryption_tried_count = 0;
    int tdm_redecryption_max_tryout_count = 0;
    int timer_tdm_total_seconds_elapsed = 0;
    int timer_tdm_wait_seconds_elapsed = 0;

    QTimer *timer_tdm_decrypt = NULL;

    bool bool_tdm_decrption_cancel_clicked = false;

    void change_background_color_in_table();
    void change_background_color_in_each_column(int row, QString color_str);

    void refresh_display_with_existing_disklist();

    QDialog *dialog_tdm_decrypt;
    QLabel *label_attempt_value;
    QLabel *label_time_elapsed_value;
    void prepare_dialog_for_tdm_decryption_attempt();

    bool bool_mount_blocker_process_running = false;
    QProcess proc_disk_arb;


};

#endif // DISK_MANAGER_H
