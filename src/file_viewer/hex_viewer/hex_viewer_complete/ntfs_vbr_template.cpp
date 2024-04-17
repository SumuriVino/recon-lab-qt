#include "ntfs_vbr_template.h"
#include "ui_ntfs_vbr_template.h"

ntfs_vbr_template::ntfs_vbr_template(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ntfs_vbr_template)
{
    ui->setupUi(this);

    ui->tableWidget->setWordWrap(false);
}

ntfs_vbr_template::~ntfs_vbr_template()
{
    delete ui;
}

void ntfs_vbr_template::pub_set_data(QByteArray vbr_data)
{
    ntfs_vbr_data = vbr_data;

    parse_ntfs_vbr_data();
}

QByteArray ntfs_vbr_template::ntfs_vbr_jump_instruction(QByteArray vbr_data)
{
    QByteArray data = vbr_data.mid(0, 3);

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_oem_id(QByteArray vbr_data)
{
    QByteArray data = vbr_data.mid(3, 8);

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_bytes_per_sector(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x0C));
    data.append(vbr_data.at(0x0B));

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_sectors_per_cluster(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x0D));

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_total_sectors(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x2f));
    data.append(vbr_data.at(0x2e));
    data.append(vbr_data.at(0x2d));
    data.append(vbr_data.at(0x2c));
    data.append(vbr_data.at(0x2b));
    data.append(vbr_data.at(0x2a));
    data.append(vbr_data.at(0x29));
    data.append(vbr_data.at(0x28));

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_mft_starting_extent(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x37));
    data.append(vbr_data.at(0x36));
    data.append(vbr_data.at(0x35));
    data.append(vbr_data.at(0x34));
    data.append(vbr_data.at(0x33));
    data.append(vbr_data.at(0x32));
    data.append(vbr_data.at(0x31));
    data.append(vbr_data.at(0x30));

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_mftmirr_starting_extent(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x3f));
    data.append(vbr_data.at(0x3e));
    data.append(vbr_data.at(0x3d));
    data.append(vbr_data.at(0x3c));
    data.append(vbr_data.at(0x3b));
    data.append(vbr_data.at(0x3a));
    data.append(vbr_data.at(0x39));
    data.append(vbr_data.at(0x38));

    return data;

}

QByteArray ntfs_vbr_template::ntfs_vbr_cluster_per_mft_record(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x40));

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_volume_serial_number(QByteArray vbr_data)
{
    QByteArray data;

    data.append(vbr_data.at(0x4f));
    data.append(vbr_data.at(0x4e));
    data.append(vbr_data.at(0x4d));
    data.append(vbr_data.at(0x4c));
    data.append(vbr_data.at(0x4b));
    data.append(vbr_data.at(0x4a));
    data.append(vbr_data.at(0x49));
    data.append(vbr_data.at(0x48));

    return data;
}

QByteArray ntfs_vbr_template::ntfs_vbr_checksum(QByteArray vbr_data)
{

    return QByteArray("");
}

void ntfs_vbr_template::parse_ntfs_vbr_data()
{

    bool ok;
    ui->tableWidget->insertRow(enum_jmp_instruction);
    ui->tableWidget->setItem(enum_jmp_instruction, 0, new QTableWidgetItem("0x00"));
    ui->tableWidget->setItem(enum_jmp_instruction, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(enum_jmp_instruction, 2, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(enum_jmp_instruction, 3, new QTableWidgetItem(QString(ntfs_vbr_jump_instruction(ntfs_vbr_data).toHex())));
    ui->tableWidget->setItem(enum_jmp_instruction, 4, new QTableWidgetItem("Jump Instruction"));

    ui->tableWidget->insertRow(enum_oem_id);
    ui->tableWidget->setItem(enum_oem_id, 0, new QTableWidgetItem("0x03"));
    ui->tableWidget->setItem(enum_oem_id, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(enum_oem_id, 2, new QTableWidgetItem("8"));
    ui->tableWidget->setItem(enum_oem_id, 3, new QTableWidgetItem(QString(ntfs_vbr_oem_id(ntfs_vbr_data))));
    ui->tableWidget->setItem(enum_oem_id, 4, new QTableWidgetItem("OEM ID"));

    ui->tableWidget->insertRow(enum_bytes_per_sector);
    ui->tableWidget->setItem(enum_bytes_per_sector, 0, new QTableWidgetItem("0x0B"));
    ui->tableWidget->setItem(enum_bytes_per_sector, 1, new QTableWidgetItem("11"));
    ui->tableWidget->setItem(enum_bytes_per_sector, 2, new QTableWidgetItem("2"));
    ui->tableWidget->setItem(enum_bytes_per_sector, 3, new QTableWidgetItem(QString::number(ntfs_vbr_bytes_per_sector(ntfs_vbr_data).toHex().toLongLong(0, 16))));
    ui->tableWidget->setItem(enum_bytes_per_sector, 4, new QTableWidgetItem("Bytes Per Sector"));

    ui->tableWidget->insertRow(enum_sectors_per_cluster);
    ui->tableWidget->setItem(enum_sectors_per_cluster, 0, new QTableWidgetItem("0x0D"));
    ui->tableWidget->setItem(enum_sectors_per_cluster, 1, new QTableWidgetItem("13"));
    ui->tableWidget->setItem(enum_sectors_per_cluster, 2, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(enum_sectors_per_cluster, 3, new QTableWidgetItem(QString::number(ntfs_vbr_sectors_per_cluster(ntfs_vbr_data).toHex().toLongLong(0, 16))));
    ui->tableWidget->setItem(enum_sectors_per_cluster, 4, new QTableWidgetItem("Sectors Per Cluster"));

    ui->tableWidget->insertRow(enum_total_sectors);
    ui->tableWidget->setItem(enum_total_sectors, 0, new QTableWidgetItem("0x28"));
    ui->tableWidget->setItem(enum_total_sectors, 1, new QTableWidgetItem("40"));
    ui->tableWidget->setItem(enum_total_sectors, 2, new QTableWidgetItem("8"));
    ui->tableWidget->setItem(enum_total_sectors, 3, new QTableWidgetItem(QString::number(ntfs_vbr_total_sectors(ntfs_vbr_data).toHex().toLongLong(0, 16))));
    ui->tableWidget->setItem(enum_total_sectors, 4, new QTableWidgetItem("Total Sectors"));


    ui->tableWidget->insertRow(enum_mft);
    ui->tableWidget->setItem(enum_mft, 0, new QTableWidgetItem("0x30"));
    ui->tableWidget->setItem(enum_mft, 1, new QTableWidgetItem("48"));
    ui->tableWidget->setItem(enum_mft, 2, new QTableWidgetItem("8"));
    ui->tableWidget->setItem(enum_mft, 3, new QTableWidgetItem(QString::number(ntfs_vbr_mft_starting_extent(ntfs_vbr_data).toHex().toLongLong(0, 16))));
    ui->tableWidget->setItem(enum_mft, 4, new QTableWidgetItem("$MFT starting extent"));

    ui->tableWidget->insertRow(enum_mftmirr);
    ui->tableWidget->setItem(enum_mftmirr, 0, new QTableWidgetItem("0x38"));
    ui->tableWidget->setItem(enum_mftmirr, 1, new QTableWidgetItem("56"));
    ui->tableWidget->setItem(enum_mftmirr, 2, new QTableWidgetItem("8"));
    ui->tableWidget->setItem(enum_mftmirr, 3, new QTableWidgetItem(QString::number(ntfs_vbr_mftmirr_starting_extent(ntfs_vbr_data).toHex().toLongLong(0, 16))));
    ui->tableWidget->setItem(enum_mftmirr, 4, new QTableWidgetItem("$MFTMirr starting extent"));

    ui->tableWidget->insertRow(enum_clusters_per_mft);
    ui->tableWidget->setItem(enum_clusters_per_mft, 0, new QTableWidgetItem("0x40"));
    ui->tableWidget->setItem(enum_clusters_per_mft, 1, new QTableWidgetItem("64"));
    ui->tableWidget->setItem(enum_clusters_per_mft, 2, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(enum_clusters_per_mft, 3, new QTableWidgetItem(QString::number(ntfs_vbr_cluster_per_mft_record(ntfs_vbr_data).toHex().toLongLong(0, 16))));
    ui->tableWidget->setItem(enum_clusters_per_mft, 4, new QTableWidgetItem("Clusters per $MFT"));


}
