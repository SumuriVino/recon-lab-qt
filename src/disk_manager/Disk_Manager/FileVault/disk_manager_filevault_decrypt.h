#ifndef DISK_MANAGER_FILEVAULT_DECRYPT_H
#define DISK_MANAGER_FILEVAULT_DECRYPT_H

#include <QFrame>
#include <QtCore>
#include <QMessageBox>
#include "header_identical_1.h"

#include "recon_generic_functions/recon_static_functions.h"
#include "disk_manager/imager_generic_functions/imager_helper_standard.h"

namespace Ui {
class disk_manager_filevault_decrypt;
}

class disk_manager_filevault_decrypt : public QDialog
{
    Q_OBJECT

public:
    explicit disk_manager_filevault_decrypt(QWidget *parent = 0);
    ~disk_manager_filevault_decrypt();


    void pub_set_device_details(struct_disk_info dev_obj);
    QString pub_get_last_run_script_path_for_decrypt();
    struct_disk_info pub_get_selected_device_details();
    void pub_try_decryption_target_disk_mode_using_timer(struct_disk_info selected_dev_obj, QString last_run_scrpt_path);
    void pub_enable_ui_on_finish(bool status);
    void pub_stop_decryption();
signals:
    void signal_decrypt_finished(bool status);

private slots:
    void on_pushButton_decrypt_clicked();

    void on_pushButton_password_clicked(bool checked);

    void on_pushButton_cancel_clicked();

private:
    Ui::disk_manager_filevault_decrypt *ui;

    QString selected_device_str;
    QString selected_device_uuid;
    QString selected_device_fs_type;
    QString password_or_recovery_key_str;

    QString apfs_selected_user;
    imager_helper_standard *recon_helper_standard_obj;
    struct_disk_info selected_dev_obj;
    QString last_run_script_path;

    void disable_enable_ui_on_decrypt_clicked(bool status);
    QString get_decrypt_script_path();
    QStringList get_apfs_users_id_list(QString disk_node_str);
};

#endif // DISK_MANAGER_FILEVAULT_DECRYPT_H
