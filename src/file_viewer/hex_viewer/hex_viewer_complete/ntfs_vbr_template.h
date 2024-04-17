#ifndef NTFS_VBR_TEMPLATE_H
#define NTFS_VBR_TEMPLATE_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class ntfs_vbr_template;
}

class ntfs_vbr_template : public QDialog
{
    Q_OBJECT

public:
    explicit ntfs_vbr_template(QWidget *parent = 0);
    ~ntfs_vbr_template();

    void pub_set_data(QByteArray vbr_data);

private:
    Ui::ntfs_vbr_template *ui;

    enum enum_ntfs_vbr_row{
        enum_jmp_instruction,
        enum_oem_id,
        enum_bytes_per_sector,
        enum_sectors_per_cluster,
        enum_total_sectors,
        enum_mft,
        enum_mftmirr,
        enum_clusters_per_mft,
        enum_volume_serial
    };


    QByteArray ntfs_vbr_jump_instruction(QByteArray vbr_data);
    QByteArray ntfs_vbr_oem_id(QByteArray vbr_data);
    QByteArray ntfs_vbr_bytes_per_sector(QByteArray vbr_data);
    QByteArray ntfs_vbr_sectors_per_cluster(QByteArray vbr_data);
    QByteArray ntfs_vbr_total_sectors(QByteArray vbr_data);
    QByteArray ntfs_vbr_mft_starting_extent(QByteArray vbr_data);
    QByteArray ntfs_vbr_mftmirr_starting_extent(QByteArray vbr_data);
    QByteArray ntfs_vbr_cluster_per_mft_record(QByteArray vbr_data);
    QByteArray ntfs_vbr_volume_serial_number(QByteArray vbr_data);
    QByteArray ntfs_vbr_checksum(QByteArray vbr_data);

    void parse_ntfs_vbr_data();

    QByteArray ntfs_vbr_data;
};

#endif // NTFS_VBR_TEMPLATE_H
