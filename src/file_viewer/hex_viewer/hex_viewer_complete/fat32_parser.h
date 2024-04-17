#ifndef FAT32_PARSER_H
#define FAT32_PARSER_H
#include <QtCore>
#include "utilities_function.h"

class fat32_parser
{
public:
    fat32_parser();

    QList<common_structure_for_data_table> pub_fat32_vbr_data(QByteArray data);

    qint64 pub_get_fat1_address(QByteArray fat32_vbr);
    qint64 pub_get_fat2_address(QByteArray fat32_vbr);
    qint64 pub_get_root_directory_address(QByteArray fat32_vbr);

    qint64 pub_get_cluster_number(QByteArray data);

    int pub_get_bytes_per_sector(QByteArray fat32_vbr);
    int pub_get_sectors_per_cluster(QByteArray fat32_vbr);

    int pub_get_fat_sector_count(QByteArray fat32_vbr);
private:
    utilities_function utility_obj;

    QString media_desctriptor_type_to_int(unsigned char);



};

#endif // FAT32_PARSER_H
