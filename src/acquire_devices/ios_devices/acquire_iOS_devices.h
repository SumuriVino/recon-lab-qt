#ifndef ACQUIRE_IOS_DEVICES_H
#define ACQUIRE_IOS_DEVICES_H

#include <QFrame>
#include <QtCore>
#include <QtGui>
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"
#include <QFileDialog>
#include <QMessageBox>
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_process.h"    
#include "report/report_essentials/report_status.h"



namespace Ui {
class acquire_iOS_devices;
}

class acquire_iOS_devices : public QFrame
{
    Q_OBJECT

public:
    explicit acquire_iOS_devices(QWidget *parent = 0);
    ~acquire_iOS_devices();

    void pub_extract_idevice_info();

    void pub_set_id_and_name(int id, QString name);

    void closeEvent(QCloseEvent *);

signals:
    void signal_attached_ios_devices_selected(QStringList uuid_list);

    void signal_bring_recon_lab_banner_page_in_front();

private slots:

    void on_tableWidget_device_list_cellClicked(int row, int column);

    void on_pushButton_refresh_list_clicked();

    void on_pushButton_acquire_clicked();

    void on_pushButton_back_clicked();

    void slot_proc_acquire_ios_devices_finished(int);

    void slot_proc_acquire_ios_devices_readyread();

    void slot_stop_acquire(bool status);
private:
    Ui::acquire_iOS_devices *ui;

    enum enum_idevice_list {

        enum_idevice_list_bookmark,
        enum_idevice_list_sr_no,
        enum_idevice_list_device_class,
        enum_idevice_list_device_name,
        enum_idevice_list_device_phone,
        enum_idevice_list_device_serial,
        enum_idevice_list_device_uuid,
        enum_idevice_list_blank
    };


    //QMap<QString, QString> key_value_map;

    QList<QMap<QString, QString>> key_value_map_list;

    void prepare_display();

    int received_id;
    QString received_name;

    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;

    QProcess *proc_acquire_ios_devices, *proc_list_devices_id;

    display_loading_progress_bar *display_loading_progress_bar_obj;

    bool bool_device_acquired_cancelled;

    QString acquiring_start_time_string;
    QString destination_location;

    QFileDialog file_dialog_obj;

    void set_gui_essentials();
};

#endif // ACQUIRE_IOS_DEVICES_CLASS_H
