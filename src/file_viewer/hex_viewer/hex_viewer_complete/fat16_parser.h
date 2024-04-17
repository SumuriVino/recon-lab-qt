#ifndef FAT16_PARSER_H
#define FAT16_PARSER_H
#include <QtCore>
#include "utilities_function.h"

class fat16_parser
{
public:
    fat16_parser();
    QList<common_structure_for_data_table> pub_fat16_vbr_data(QByteArray data);

    qint64 pub_get_fat1_address(QByteArray fat16_vbr);
    qint64 pub_get_fat2_address(QByteArray fat16_vbr);
    qint64 pub_get_root_directory_address(QByteArray fat16_vbr);

    QList<common_structure_for_data_table> pub_SFN_data(QByteArray sfn_data);


    qint64 pub_get_cluster_number(QByteArray data);
    int pub_get_bytes_per_sector(QByteArray fat_vbr);
    int pub_get_sectors_per_cluster(QByteArray fat_vbr);

    int pub_get_fat_sector_count(QByteArray fat_vbr);
private:
    utilities_function utility_obj;

    QString media_desctriptor_type_to_int(unsigned char);

    QString attribute_name(unsigned char val);

    QString fat_time(QByteArray ar);

    QString fat_date(QByteArray ar);

};

#endif // FAT16_PARSER_H
