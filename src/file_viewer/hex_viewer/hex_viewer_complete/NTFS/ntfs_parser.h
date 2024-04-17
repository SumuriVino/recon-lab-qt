#ifndef NTFS_PARSER_H
#define NTFS_PARSER_H
#include <QtCore>
#include "file_viewer/hex_viewer/hex_viewer_complete/utilities_function.h"

class ntfs_parser
{
public:
    ntfs_parser();

    mft_entry ntfs_parse_mft_entry(QByteArray data);

    QList<common_structure_for_data_table> ntfs_mft_entry_file_record_header(QByteArray data);

    QList<common_structure_for_data_table> ntfs_mft_parse_attribute(QByteArray data, ntfs_attribute_and_offset attr);

    QList<ntfs_attribute_and_offset> get_mft_attributes_list(QByteArray data);

    QString attribute_type_to_name(int attr_type);

    qint64 mft_address(QByteArray ntfs_vbr);

    int sector_per_cluster(QByteArray ntfs_vbr);

    int sector_size(QByteArray ntfs_vbr);

    QString filename_type_to_string(QByteArray ar);

    qint64 get_first_cluster_number(QByteArray ar);

private:
    utilities_function utility_obj;


    int attribute_type(QByteArray ar);

    QList<common_structure_for_data_table> parse_standard_attribute(QByteArray data, ntfs_attribute_and_offset attr);
    QList<common_structure_for_data_table> parse_filename_attribute(QByteArray data, ntfs_attribute_and_offset attr);


    QString standard_attr_flags_to_string(QByteArray ar);

};

#endif // NTFS_PARSER_H
