#include "disk_manager_format_disk.h"
#include "ui_disk_manager_format_disk.h"

disk_manager_format_disk::disk_manager_format_disk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disk_manager_format_disk)
{
    ui->setupUi(this);


    QString title_str = QString("Disk Manager")  + QString(" - ") + QString("Format");

    setWindowTitle(title_str);

    setWindowFlags (Qt::CustomizeWindowHint | Qt::WindowTitleHint  | Qt::WindowMaximizeButtonHint);

    setFixedHeight(185);
    setFixedWidth(500);

    bool_is_device_whole_disk = false;

    ui->lineEdit_label->setAttribute(Qt::WA_MacShowFocusRect, false);


    recon_helper_standard_obj = new imager_helper_standard(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->hide();


    QStringList filesystem_list;
    filesystem_list << "HFS+" << "APFS" << "ExFAT";

    ui->comboBox_format->addItems(filesystem_list);
    ui->comboBox_format->setCurrentIndex(0);



}

disk_manager_format_disk::~disk_manager_format_disk()
{
    delete ui;
}

void disk_manager_format_disk::pub_set_device_details(struct_disk_info selected_dev_obj)
{ // set device/disks details on widget
    setWindowModality(Qt::ApplicationModal);


    ui->lineEdit_label->clear();
    ui->label_status->clear();

    ui->comboBox_format->setCurrentIndex(0);


    QString dev_str = selected_dev_obj.device_identifier;
    if(!dev_str.trimmed().isEmpty())
    {

        ui->label_status->setText(QString("Selected Device '") + dev_str + QString("'"));


        if(!dev_str.startsWith("/dev/"))
            dev_str.prepend("/dev/");

        selected_device_str = dev_str;
    }
    else
    {
        QMessageBox::warning(this, QString("Disk Manager"), QString("Device not found."));
        hide();
        return;
    }

    bool_is_device_whole_disk = selected_dev_obj.is_whole_disk;
    selected_device_fs_type   = selected_dev_obj.filesystem_FilesystemType;

    selected_dev_parent_whole = selected_dev_obj.apfs_physical_device_id;
}

void disk_manager_format_disk::disable_enable_ui_on_format_clicked(bool status)
{ // disable/enable ui on format button clicked
    ui->pushButton_format->setEnabled(status);
    ui->comboBox_format->setEnabled(status);
    ui->lineEdit_label->setEnabled(status);
    ui->pushButton_cancel->setEnabled(status);


    if(!status)
        ui->progressBar->show();
    else
        ui->progressBar->hide();

}


void disk_manager_format_disk::on_pushButton_format_clicked()
{ // format button clicked. Right now this one is hide

    ///=======Prepare Format essentials======///
    QString label_str = ui->lineEdit_label->text().trimmed();
    if(label_str.isEmpty())
    {
        QMessageBox::warning(this, QString("Disk Manager"), QString("Please enter the File System Label."));
        return;
    }

    disable_enable_ui_on_format_clicked(false);

    QString selected_fs_type = ui->comboBox_format->currentText().trimmed();
    if(selected_fs_type == QString("HFS+"))
    {
        selected_fs_type = QString("JHFS+");
    }
    else if(selected_fs_type == QString("ExFAT"))
    {
        selected_fs_type = QString("exfat");
    }
    else if(selected_fs_type == QString("APFS"))
    {
        selected_fs_type = QString("apfs");
    }
    else
        selected_fs_type = QString("JHFS+");

    QString status_str = QString("Formatting '") + selected_device_str + QString("' as '") + label_str + QString("'");
    ui->label_status->setText(status_str);

    QStringList args_list;
    if(selected_device_fs_type == "apfs")
    {
        ///changed to Physical container
        selected_device_str = selected_dev_parent_whole;
        args_list << "apfs" << "deleteContainer" << selected_device_str;
        recon_helper_standard_obj->run_command_with_arguments("diskutil", args_list, Q_FUNC_INFO);
    }

    wipe_filesystem_before_format();

    ///=======Format======///
    QString erase_str = "eraseVolume";
    if(bool_is_device_whole_disk)
        erase_str = "eraseDisk";

    args_list.clear();
    args_list << erase_str << selected_fs_type << label_str << selected_device_str;

    QString output_str = recon_helper_standard_obj->run_command_with_arguments("diskutil", args_list, Q_FUNC_INFO);

    QStringList unmount_args;

    if(bool_is_device_whole_disk)
        unmount_args << "unmountDisk" << selected_device_str;
    else
        unmount_args << "unmount" << selected_device_str;

    recon_helper_standard_obj->run_command_with_arguments("diskutil", unmount_args, Q_FUNC_INFO);

    disable_enable_ui_on_format_clicked(true);

    if(output_str.contains("Finished erase on"))
    {
        emit signal_format_finished(true);
        return;
    }
    else
    {
        emit signal_format_finished(false);
        return;
    }
}

void disk_manager_format_disk::wipe_filesystem_before_format()
{
    QStringList args_list;
    args_list << "if=/dev/zero" << selected_device_str << "bufsz=1024"<< "cnt=200";
    recon_helper_standard_obj->run_command_with_arguments("./dc3dd", args_list, Q_FUNC_INFO);
}

void disk_manager_format_disk::on_pushButton_cancel_clicked()
{
    hide();
}
