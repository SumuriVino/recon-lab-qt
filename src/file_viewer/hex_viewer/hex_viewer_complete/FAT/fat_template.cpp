#include "fat_template.h"
#include "ui_fat_template.h"

FAT_Template::FAT_Template(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FAT_Template)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(1, 200);
}

FAT_Template::~FAT_Template()
{
    delete ui;
}

void FAT_Template::pub_set_fat_data(QByteArray ar)
{

    ui->tableWidget->setRowCount(0);

    int row = 0;

    QTableWidgetItem *item_cluster_number;
    QTableWidgetItem *item_cluster_status;

    if(fat_type == 16)
    {
        for(int i = 0; i < ar.size(); i = i+2)
        {
            item_cluster_number = new QTableWidgetItem;
            item_cluster_status = new QTableWidgetItem;

            ui->tableWidget->insertRow(row);
            item_cluster_number->setText(QString::number(row));

            QByteArray cluster_data = ar.mid(i, 2);

            if(cluster_data.toHex().toUpper() == "FFFF" || cluster_data.toHex().toUpper() == "F8FF")
            {
                item_cluster_status->setText("End of file");
            }
            else if(cluster_data.toHex().toUpper() == "0000" )
            {
                item_cluster_status->setText("Unallocated");
            }
            else if(cluster_data.toHex().toUpper() == "F7FF" )
            {
                item_cluster_status->setText("Bad Cluster");
            }
            else {
               qint16 next_cluster = utilities_obj.convert_bytearray_to_number_16bit(cluster_data, LITTLE_ENDIAN_HEX);

               item_cluster_status->setText("Allocated. Next cluster is " + QString::number(next_cluster));
            }

            ui->tableWidget->setItem(row, 0, item_cluster_number);
            ui->tableWidget->setItem(row, 1, item_cluster_status);

            row++;

        }
    }


    if(fat_type == 32)
    {
        for(int i = 0; i < ar.size(); i = i+4)
        {
            item_cluster_number = new QTableWidgetItem;
            item_cluster_status = new QTableWidgetItem;

            ui->tableWidget->insertRow(row);
            item_cluster_number->setText(QString::number(row));

            QByteArray cluster_data = ar.mid(i, 4);

            if(cluster_data.toHex().toUpper() == "FFFFFF0F" || cluster_data.toHex().toUpper() == "FFFFFFF8"
                    || cluster_data.toHex().toUpper() == "F8FFFF0F" || cluster_data.toHex().toUpper() == "FFFFFFFF")
            {
                item_cluster_status->setText("End of file");
            }
            else if(cluster_data.toHex().toUpper() == "00000000" )
            {
                item_cluster_status->setText("Unallocated");
            }
            else if(cluster_data.toHex().toUpper() == "FFFFFF7F" )
            {
                item_cluster_status->setText("Bad Cluster");
            }
            else {
               quint32 next_cluster = utilities_obj.convert_bytearray_to_number_32bit_unsigned(cluster_data, LITTLE_ENDIAN_HEX);

               item_cluster_status->setText("Allocated. Next cluster is " + QString::number(next_cluster));
            }

            ui->tableWidget->setItem(row, 0, item_cluster_number);
            ui->tableWidget->setItem(row, 1, item_cluster_status);

            row++;

        }
    }

}

void FAT_Template::pub_set_fat_type(int type)
{
    fat_type = type;
}
