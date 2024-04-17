#ifndef DISK_MANAGER_FORMAT_DISK_H
#define DISK_MANAGER_FORMAT_DISK_H

#include <QFrame>
#include <QtCore>
#include <QMessageBox>
#include "header_identical_1.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "disk_manager/imager_generic_functions/imager_helper_standard.h"

namespace Ui {
class disk_manager_format_disk;
}

class disk_manager_format_disk : public QDialog
{
    Q_OBJECT

public:
    explicit disk_manager_format_disk(QWidget *parent = 0);
    ~disk_manager_format_disk();

    void pub_set_device_details(struct_disk_info selected_dev_obj);
signals:
    void signal_format_finished(bool status);

private slots:
    void on_pushButton_format_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::disk_manager_format_disk *ui;

    QString selected_device_str;
    QString selected_device_fs_type;
    QString selected_dev_parent_whole;



    bool bool_is_device_whole_disk;

    imager_helper_standard *recon_helper_standard_obj;



    void disable_enable_ui_on_format_clicked(bool status);
    void wipe_filesystem_before_format();
};

#endif // DISK_MANAGER_FORMAT_DISK_H
