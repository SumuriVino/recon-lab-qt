#include "connected_drive_list_popup.h"
#include "ui_connected_drive_list_popup.h"

connected_drive_list_popup::connected_drive_list_popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connected_drive_list_popup)
{
    ui->setupUi(this);
}

connected_drive_list_popup::~connected_drive_list_popup()
{
    delete ui;
}

void connected_drive_list_popup::pub_extract_device_list()
{
    ui->tableWidget->setRowCount(0);
    connected_drives obj;

    obj.pub_extract_device_list();

    QList<physical_device> list = obj.pub_get_device_list();

    int row = 0;
    foreach(physical_device device, list){

        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(device.physical_node));
        if(device.physical_node.startsWith("\\"))
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem("Physical Device"));
        else
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem("Mounted Volume"));

        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(device.size)));

        row++;
    }
}

void connected_drive_list_popup::on_pushButton_cancel_clicked()
{
    hide();
}

void connected_drive_list_popup::on_pushButton_open_clicked()
{
    if(ui->tableWidget->currentRow() < 0)
        return;

    struct_device_info info;
    info.device_name = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
    info.device_size = ui->tableWidget->item(ui->tableWidget->currentRow(), 2)->text().toLongLong();

    emit signal_selected_device(info);

    hide();
}
