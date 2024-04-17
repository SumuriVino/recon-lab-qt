#ifndef ACQUIRE_ANDROID_DEVICES_H
#define ACQUIRE_ANDROID_DEVICES_H

#include <QFrame>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>


#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_helper_process.h"

#include "display_loading_progress_bar/display_loading_progress_bar.h"

namespace Ui {
class acquire_android_devices;
}

class acquire_android_devices : public QFrame
{
    Q_OBJECT

public:
    explicit acquire_android_devices(QWidget *parent = 0);
    ~acquire_android_devices();


    void closeEvent(QCloseEvent *);

signals:
    void signal_bring_recon_lab_banner_page_in_front();

private slots:

    void on_tableWidget_device_list_cellClicked(int row_number, int column);
    void on_pushButton_refresh_list_clicked();
    void on_pushButton_acquire_clicked();
    void on_pushButton_back_clicked();

    void slot_stop_acquire(bool status);

    void slot_proc_acquire_making_abfile_finished(int);

    void slot_proc_acquire_making_abfile_readyread();
    void slot_proc_acquire_making_ab_to_tar_readyread();
    void slot_proc_acquire_making_ab_to_tar_finished(int);
    void slot_proc_acquire_decompress_tar_readyread();
    void slot_proc_acquire_decompress_tar_finished(int);
private:
    Ui::acquire_android_devices *ui;

    enum enum_andrd_device_list
    {
        enum_andrd_device_model_name,
        enum_andrd_device_manufacturer,
        enum_andrd_device_brand,
        enum_andrd_device_serial_no,
        enum_andrd_device_blank
    };

    QString M_VAR_DEVICE_INFO_QString_model_QString;
    QString M_VAR_DEVICE_INFO_QString_manufacturer_QString;
    QString M_VAR_DEVICE_INFO_QString_brand_QString;
    QString M_VAR_DEVICE_INFO_QString_iccid_QString;
    QString M_VAR_DEVICE_INFO_QString_imei1_QString;
    QString M_VAR_DEVICE_INFO_QString_imei2_QString;
    QString M_VAR_DEVICE_INFO_QString_serial_no_QString;
    QString M_VAR_DEVICE_INFO_QString_sys_timezone_QString;

    display_loading_progress_bar *display_loading_progress_bar_obj;

    QFileDialog file_dialog_obj;

    QList<QMap<QString, QString>> key_value_map_list;

    void extract_device_info(void);
    void display_data(void);
    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;

    QStringList extract_device_srl_no_list(void);

    bool bool_device_acquired_cancelled = false;
    bool bool_acquire_making_abfile_sucess = false;
    bool bool_acquire_making_ab_to_tar_sucess = false;
    bool bool_acquire_decompress_tar_sucess = false;
    QString acquiring_start_time_string;
    QProcess *proc_acquire_making_abfile;
    QProcess *proc_acquire_making_ab_to_tar;
    QProcess *proc_acquire_decompress_tar;

    QString destination_location;

    QString making_ab_file();
    QString making_ab_to_tar(QString ab_filepath);
    void acquire_decompress_tar(QString tar_filepath);

};

#endif // ACQUIRE_ANDROID_DEVICES_H
