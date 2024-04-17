#include "ntfs_parser.h"
#include <QtCore>

ntfs_parser::ntfs_parser()
{

}

mft_entry ntfs_parser::ntfs_parse_mft_entry(QByteArray data)
{

    mft_entry obj;
    if(data.startsWith(QByteArray("FILE")))
    {
        obj.ok = false;
        return obj;
    }

    obj.ok = true;

    obj.signature = data.mid(0, 4);
    obj.offset_to_fixup_array = data.mid(0x04, 2);
    obj.entries_in_fix_array = data.mid(0x06, 2);
    obj.logfile_sequence_number = data.mid(0x08, 8);
    obj.sequence_count = data.mid(0x10, 2);
    obj.hard_link_count = data.mid(0x12, 2);
    obj.offset_to_first_attribute = data.mid(0x14, 2);
    obj.allocation_status = data.mid(0x16, 2);
    obj.logical_size = data.mid(0x18, 4);
    obj.physical_size = data.mid(0x1c, 4);
    obj.file_refrence_to_base_record = data.mid(0x20, 8);
    obj.next_attribute_identification = data.mid(0x28, 2);



    return obj;
}

QList<common_structure_for_data_table> ntfs_parser::ntfs_mft_entry_file_record_header(QByteArray data)
{
    QList<common_structure_for_data_table> list;

    common_structure_for_data_table signature;
    signature.offset_dec = "0";
    signature.offset_hex = "0x00";
    signature.length = "4";
    signature.value = data.mid(0, 4);
    signature.value_hex = data.mid(0, 4).toHex();
    signature.description = "Signature";
    list.append(signature);


    common_structure_for_data_table offset_to_fix_up_array;
    offset_to_fix_up_array.offset_dec = "4";
    offset_to_fix_up_array.offset_hex = "0x04";
    offset_to_fix_up_array.length = "2";
    offset_to_fix_up_array.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(4, 2), LITTLE_ENDIAN_HEX));
    offset_to_fix_up_array.value_hex = data.mid(4, 2).toHex();
    offset_to_fix_up_array.description = "Offset to fix up array";
    list.append(offset_to_fix_up_array);

    common_structure_for_data_table entried_in_fix_up_array;
    entried_in_fix_up_array.offset_dec = "6";
    entried_in_fix_up_array.offset_hex = "0x06";
    entried_in_fix_up_array.length = "2";
    entried_in_fix_up_array.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(6, 2), LITTLE_ENDIAN_HEX));
    entried_in_fix_up_array.value_hex = data.mid(6, 2).toHex();
    entried_in_fix_up_array.description = "Entried in fix up array";
    list.append(entried_in_fix_up_array);

    common_structure_for_data_table logfile_sequence;
    logfile_sequence.offset_dec = "8";
    logfile_sequence.offset_hex = "0x08";
    logfile_sequence.length = "8";
    logfile_sequence.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(data.mid(8, 8), LITTLE_ENDIAN_HEX));
    logfile_sequence.value_hex = data.mid(8, 8).toHex();
    logfile_sequence.description = "$LogFile Sequence Number";
    list.append(logfile_sequence);

    common_structure_for_data_table sequence_count;
    sequence_count.offset_dec = "16";
    sequence_count.offset_hex = "0x10";
    sequence_count.length = "2";
    sequence_count.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(16, 2), LITTLE_ENDIAN_HEX));
    sequence_count.value_hex = data.mid(16, 2).toHex();
    sequence_count.description = "Sequence Count";
    list.append(sequence_count);


    common_structure_for_data_table hard_link_count;
    hard_link_count.offset_dec = "18";
    hard_link_count.offset_hex = "0x12";
    hard_link_count.length = "2";
    hard_link_count.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(16, 2), LITTLE_ENDIAN_HEX));
    hard_link_count.value_hex = data.mid(16, 2).toHex();
    hard_link_count.description = "Hard Link Count";
    list.append(hard_link_count);

    common_structure_for_data_table offset_to_first_attribute;
    offset_to_first_attribute.offset_dec = "20";
    offset_to_first_attribute.offset_hex = "0x14";
    offset_to_first_attribute.length = "2";
    offset_to_first_attribute.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(20, 2), LITTLE_ENDIAN_HEX));
    offset_to_first_attribute.value_hex = data.mid(20, 2).toHex();
    offset_to_first_attribute.description = "Offset to first attribute";
    list.append(offset_to_first_attribute);

    common_structure_for_data_table allocation_status;
    allocation_status.offset_dec = "22";
    allocation_status.offset_hex = "0x16";
    allocation_status.length = "2";

    if(data.mid(22, 1).at(0) == 0x00)
        allocation_status.value = "Deleted File";
    else if(data.mid(22, 1).at(0) == 0x01)
        allocation_status.value = "Allocated File";
    else if(data.mid(22, 1).at(0) == 0x02)
        allocation_status.value = "Deleted Directory";
    else if(data.mid(22, 1).at(0) == 0x03)
        allocation_status.value = "Allocated Directory";

    allocation_status.value_hex = data.mid(22, 2).toHex();
    allocation_status.description = "Allocation Status";
    list.append(allocation_status);

    common_structure_for_data_table logical_size;
    logical_size.offset_dec = "24";
    logical_size.offset_hex = "0x18";
    logical_size.length = "4";
    logical_size.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(24, 4), LITTLE_ENDIAN_HEX));
    logical_size.value_hex = data.mid(24, 4).toHex();
    logical_size.description = "Logical size of $MFT Record";
    list.append(logical_size);

    common_structure_for_data_table physical_size;
    physical_size.offset_dec = "28";
    physical_size.offset_hex = "0x1C";
    physical_size.length = "4";
    physical_size.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(28, 4), LITTLE_ENDIAN_HEX));
    physical_size.value_hex = data.mid(28, 4).toHex();
    physical_size.description = "Physical size of $MFT Record";
    list.append(physical_size);


    return list;
}

QList<common_structure_for_data_table> ntfs_parser::ntfs_mft_parse_attribute(QByteArray data, ntfs_attribute_and_offset attr)
{
    if(attr.attribute_type == enum_ntfs_mft_standard_attribute)
        return parse_standard_attribute(data, attr);

    if(attr.attribute_type == enum_ntfs_mft_file_name_attribute)
        return parse_filename_attribute(data, attr);


    QList<common_structure_for_data_table> list;

    return list;
}

QList<ntfs_attribute_and_offset> ntfs_parser::get_mft_attributes_list(QByteArray data)
{


    QList<ntfs_attribute_and_offset> list;

    int attribute_offset = utility_obj.convert_bytearray_to_number_16bit(data.mid(20, 2), LITTLE_ENDIAN_HEX);

    while(1)
    {
        int attr_type = attribute_type(data.mid(attribute_offset, 4));

        if(attr_type == enum_ntfs_mft_unknown_attribute || attr_type == enum_ntfs_mft_end_attribute)
            break;

        int attr_size = utility_obj.convert_bytearray_to_number_32bit(data.mid(attribute_offset + 4, 4), LITTLE_ENDIAN_HEX);

        ntfs_attribute_and_offset obj;

        obj.attribute_type = attr_type;
        obj.offset = attribute_offset;
        obj.size = attr_size;

        list.append(obj);
        attribute_offset = attribute_offset + attr_size;
    }

    return list;
}

QString ntfs_parser::attribute_type_to_name(int attr_type)
{
    if(attr_type == enum_ntfs_mft_standard_attribute)
        return "Standard Attribute";

    if(attr_type == enum_ntfs_mft_file_name_attribute)
        return "Filename Attribute";

    return "Unknown";
}

qint64 ntfs_parser::mft_address(QByteArray ntfs_vbr)
{
    if(ntfs_vbr.size() < 512)
        return -1;

    QByteArray bytes_per_sector = ntfs_vbr.mid(0x0B, 2);
    QByteArray sectors_per_cluster = ntfs_vbr.mid(0x0D, 1);
    QByteArray mft_start_extent = ntfs_vbr.mid(0x30, 8);

    qint64 mft_address = 0;

    int bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);
    int sectors_per_cluster_int = sectors_per_cluster.at(0);
    qint64 mft_start_extent_int = utility_obj.convert_bytearray_to_number_64bit(mft_start_extent, LITTLE_ENDIAN_HEX);

    return bytes_per_sector_int * sectors_per_cluster_int * mft_start_extent_int;
}

int ntfs_parser::sector_per_cluster(QByteArray ntfs_vbr)
{
    QByteArray sectors_per_cluster = ntfs_vbr.mid(0x0D, 1);
    return sectors_per_cluster.at(0);
}

int ntfs_parser::sector_size(QByteArray ntfs_vbr)
{
    QByteArray bytes_per_sector = ntfs_vbr.mid(0x0B, 2);

    return utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);
}

QString ntfs_parser::filename_type_to_string(QByteArray ar)
{
    if(ar.size() < 0)
        return "";

    if(ar.at(0) == 0x00)
        return "POSIX";

    if(ar.at(0) == 0x01)
        return "LFN";

    if(ar.at(0) == 0x02)
        return "SFN";

    if(ar.at(0) == 0x03)
        return "Win32 & DOS";


    return "";
}

qint64 ntfs_parser::get_first_cluster_number(QByteArray ar)
{
    QList<ntfs_attribute_and_offset> attr_list = get_mft_attributes_list(ar);

    foreach (ntfs_attribute_and_offset attr_details, attr_list) {

       if(attr_details.attribute_type == enum_ntfs_mft_data_attribute)
       {
                unsigned char content_type = ar.at(attr_details.offset + 8);

                if(content_type == 0x01)
                {
                    QByteArray offset_to_run_list = ar.mid(attr_details.offset + 0x20, 2);

                    short int run_list_offset_int =  utility_obj.convert_bytearray_to_number_16bit(offset_to_run_list, LITTLE_ENDIAN_HEX);

                    QByteArray run_list_data = ar.mid(attr_details.offset + run_list_offset_int, 10);

                    unsigned char cluster_number_length = run_list_data.at(0);
                    unsigned char cluster_count_length = run_list_data.at(0);
                    cluster_number_length = cluster_number_length >> 4;

                    cluster_count_length = cluster_count_length << 4;
                    cluster_count_length = cluster_count_length >> 4;

                    QByteArray first_cluster_number;

                    QByteArray a_tmp = ar.mid(attr_details.offset + run_list_offset_int + 1 + cluster_count_length, cluster_number_length);

                    for(int i = (a_tmp.size() - 1); i >= 0 ; i--)
                    {
                        first_cluster_number.append(a_tmp.at(i));
                    }

                    return first_cluster_number.toHex().toULongLong(0, 16);
//                    if(cluster_number_length == 2)
//                    {
//                        return utility_obj.convert_bytearray_to_number_16bit_unsigned(first_cluster_number, LITTLE_ENDIAN_HEX);
//                    }
                }
                else
                    return 0;
       }
    }

    return -1;
}

int ntfs_parser::attribute_type(QByteArray ar)
{
    if(ar.at(0) == 0x10 && ar.at(1) == 0x00 && ar.at(2) == 0x00 && ar.at(3) == 0x00 )
        return enum_ntfs_mft_standard_attribute;

    if(ar.at(0) == 0x30 && ar.at(1) == 0x00 && ar.at(2) == 0x00 && ar.at(3) == 0x00 )
        return enum_ntfs_mft_file_name_attribute;

    if(ar.at(0) == (char)0x80 && ar.at(1) == 0x00 && ar.at(2) == 0x00 && ar.at(3) == 0x00 )
        return enum_ntfs_mft_data_attribute;

    if(ar.at(0) == 0x50 && ar.at(1) == 0x00 && ar.at(2) == 0x00 && ar.at(3) == 0x00 )
        return enum_ntfs_mft_security_descriptor_attribute;

    if(ar.at(0) == (char)0xFF && ar.at(1) == (char)0xFF && ar.at(2) == (char)0xFF && ar.at(3) == (char)0xFF )
        return enum_ntfs_mft_end_attribute;

    return enum_ntfs_mft_unknown_attribute;
}

QList<common_structure_for_data_table> ntfs_parser::parse_standard_attribute(QByteArray data, ntfs_attribute_and_offset attr)
{
    QList<common_structure_for_data_table> list;

    int current_offset = attr.offset;

    common_structure_for_data_table attr_identifier;
    attr_identifier.offset_dec = "0";
    attr_identifier.offset_hex = "0x00";
    attr_identifier.length = "4";
    attr_identifier.value = data.mid(current_offset, 4).toHex();
    attr_identifier.value_hex = data.mid(current_offset, 4).toHex();
    attr_identifier.description = "Attribute Identifier";
    list.append(attr_identifier);


    common_structure_for_data_table attr_length;
    attr_length.offset_dec = "4";
    attr_length.offset_hex = "0x04";
    attr_length.length = "4";
    attr_length.value =  QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(current_offset + 4 , 4), LITTLE_ENDIAN_HEX));
    attr_length.value_hex = data.mid(current_offset + 4, 4).toHex();
    attr_length.description = "Attribute Length";
    list.append(attr_length);

    common_structure_for_data_table content_flag;
    content_flag.offset_dec = "8";
    content_flag.offset_hex = "0x08";
    content_flag.length = "1";
    if(data.mid(current_offset + 8, 1).at(0) == 0x00)
        content_flag.value = "Resident";
    else
        content_flag.value = "Non Resident";

    content_flag.value_hex = data.mid(current_offset + 8, 1).toHex();
    content_flag.description = "Content Flag";
    list.append(content_flag);



    common_structure_for_data_table length_stream_name;
    length_stream_name.offset_dec = "9";
    length_stream_name.offset_hex = "0x09";
    length_stream_name.length = "1";
    length_stream_name.value = QString::number(data.mid(current_offset + 9, 1).toHex().toInt(0, 16));
    length_stream_name.value_hex = data.mid(current_offset + 9, 1).toHex();
    length_stream_name.description = "Length of stream name";
    list.append(length_stream_name);

    common_structure_for_data_table offset_stream_name;
    offset_stream_name.offset_dec = "10";
    offset_stream_name.offset_hex = "0x0A";
    offset_stream_name.length = "2";
    offset_stream_name.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(current_offset + 10, 2), LITTLE_ENDIAN_HEX));
    offset_stream_name.value_hex = data.mid(current_offset + 10, 2).toHex();
    offset_stream_name.description = "Offset to the stream name";
    list.append(offset_stream_name);



    common_structure_for_data_table size_of_content;
    size_of_content.offset_dec = "16";
    size_of_content.offset_hex = "0x10";
    size_of_content.length = "4";
    size_of_content.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(current_offset + 16, 4), LITTLE_ENDIAN_HEX));
    size_of_content.value_hex = data.mid(current_offset + 16, 4).toHex();
    size_of_content.description = "Size of content";
    list.append(size_of_content);


    common_structure_for_data_table offset_to_content;
    int offset_to_attr_content ;
    offset_to_content.offset_dec = "20";
    offset_to_content.offset_hex = "0x14";
    offset_to_content.length = "2";
    offset_to_content.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(current_offset + 20, 2), LITTLE_ENDIAN_HEX));
    offset_to_attr_content = current_offset +  utility_obj.convert_bytearray_to_number_16bit(data.mid(current_offset + 20, 2), LITTLE_ENDIAN_HEX);
    offset_to_content.value_hex = data.mid(current_offset + 20, 2).toHex();
    offset_to_content.description = "Offset to the content";
    list.append(offset_to_content);

    QByteArray m_ar;

    common_structure_for_data_table create_time;
    create_time.offset_dec = QString::number(offset_to_attr_content + 0);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 0);
    create_time.offset_hex = "0x" + m_ar.toHex();
    create_time.length = "8";
    create_time.value = utility_obj.convert_ntfs_time_to_unix_epoch_string((data.mid(offset_to_attr_content + 0, 8)));
    create_time.value_hex = data.mid(offset_to_attr_content + 0, 8).toHex();
    create_time.description = "Create Time";
    list.append(create_time);


    common_structure_for_data_table modified_time;
    modified_time.offset_dec = QString::number(offset_to_attr_content + 8);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 8);
    modified_time.offset_hex = "0x" + m_ar.toHex();
    modified_time.length = "8";
    modified_time.value = utility_obj.convert_ntfs_time_to_unix_epoch_string((data.mid(offset_to_attr_content + 8, 8)));
    modified_time.value_hex = data.mid(offset_to_attr_content + 8, 8).toHex();
    modified_time.description = "Modified Time";
    list.append(modified_time);

    common_structure_for_data_table mft_modified_time;
    mft_modified_time.offset_dec = QString::number(offset_to_attr_content + 16);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 16);
    mft_modified_time.offset_hex = "0x" + m_ar.toHex();
    mft_modified_time.length = "8";
    mft_modified_time.value = utility_obj.convert_ntfs_time_to_unix_epoch_string((data.mid(offset_to_attr_content + 16, 8)));
    mft_modified_time.value_hex = data.mid(offset_to_attr_content + 16, 8).toHex();
    mft_modified_time.description = "$MFT Modified Time";
    list.append(mft_modified_time);

    common_structure_for_data_table access_time;
    access_time.offset_dec = QString::number(offset_to_attr_content + 24);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 24);
    access_time.offset_hex = "0x" + m_ar.toHex();
    access_time.length = "8";
    access_time.value = utility_obj.convert_ntfs_time_to_unix_epoch_string((data.mid(offset_to_attr_content + 24, 8)));
    access_time.value_hex = data.mid(offset_to_attr_content + 24, 8).toHex();
    access_time.description = "Last Accessed Time";
    list.append(access_time);

    common_structure_for_data_table file_type_flag;
    file_type_flag.offset_dec = QString::number(offset_to_attr_content + 32);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 32);
    file_type_flag.offset_hex = "0x" + m_ar.toHex();
    file_type_flag.length = "4";
    file_type_flag.value = standard_attr_flags_to_string(data.mid(offset_to_attr_content + 32, 4));
    file_type_flag.value_hex = data.mid(offset_to_attr_content + 32, 4).toHex();
    file_type_flag.description = "File Type";
    list.append(file_type_flag);


    return list;
}

QList<common_structure_for_data_table> ntfs_parser::parse_filename_attribute(QByteArray data, ntfs_attribute_and_offset attr)
{
    QList<common_structure_for_data_table> list;

    int current_offset = attr.offset;

    common_structure_for_data_table attr_identifier;
    attr_identifier.offset_dec = "0";
    attr_identifier.offset_hex = "0x00";
    attr_identifier.length = "4";
    attr_identifier.value = data.mid(current_offset, 4).toHex();
    attr_identifier.value_hex = data.mid(current_offset, 4).toHex();
    attr_identifier.description = "Attribute Identifier";
    list.append(attr_identifier);


    common_structure_for_data_table attr_length;
    attr_length.offset_dec = "4";
    attr_length.offset_hex = "0x04";
    attr_length.length = "4";
    attr_length.value =  QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(current_offset + 4 , 4), LITTLE_ENDIAN_HEX));
    attr_length.value_hex = data.mid(current_offset + 4, 4).toHex();
    attr_length.description = "Attribute Length";
    list.append(attr_length);

    common_structure_for_data_table content_flag;
    content_flag.offset_dec = "8";
    content_flag.offset_hex = "0x08";
    content_flag.length = "1";
    if(data.mid(current_offset + 8, 1).at(0) == 0x00)
        content_flag.value = "Resident";
    else
        content_flag.value = "Non Resident";

    content_flag.value_hex = data.mid(current_offset + 8, 1).toHex();
    content_flag.description = "Content Flag";
    list.append(content_flag);


    common_structure_for_data_table length_stream_name;
    length_stream_name.offset_dec = "9";
    length_stream_name.offset_hex = "0x09";
    length_stream_name.length = "1";
    length_stream_name.value = QString::number(data.mid(current_offset + 9, 1).toHex().toInt(0, 16));
    length_stream_name.value_hex = data.mid(current_offset + 9, 1).toHex();
    length_stream_name.description = "Length of stream name";
    list.append(length_stream_name);

    common_structure_for_data_table offset_stream_name;
    offset_stream_name.offset_dec = "10";
    offset_stream_name.offset_hex = "0x0A";
    offset_stream_name.length = "2";
    offset_stream_name.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(current_offset + 10, 2), LITTLE_ENDIAN_HEX));
    offset_stream_name.value_hex = data.mid(current_offset + 10, 2).toHex();
    offset_stream_name.description = "Offset to the stream name";
    list.append(offset_stream_name);



    common_structure_for_data_table size_of_content;
    size_of_content.offset_dec = "16";
    size_of_content.offset_hex = "0x10";
    size_of_content.length = "4";
    size_of_content.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(current_offset + 16, 4), LITTLE_ENDIAN_HEX));
    size_of_content.value_hex = data.mid(current_offset + 16, 4).toHex();
    size_of_content.description = "Size of content";
    list.append(size_of_content);


    common_structure_for_data_table offset_to_content;
    int offset_to_attr_content ;
    offset_to_content.offset_dec = "20";
    offset_to_content.offset_hex = "0x14";
    offset_to_content.length = "2";
    offset_to_content.value = QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(current_offset + 20, 2), LITTLE_ENDIAN_HEX));
    offset_to_attr_content = current_offset +  utility_obj.convert_bytearray_to_number_16bit(data.mid(current_offset + 20, 2), LITTLE_ENDIAN_HEX);
    offset_to_content.value_hex = data.mid(current_offset + 20, 2).toHex();
    offset_to_content.description = "Offset to the content";
    list.append(offset_to_content);

    QByteArray m_ar;

    common_structure_for_data_table parent_mft_record_number;
    parent_mft_record_number.offset_dec = QString::number(offset_to_attr_content + 0);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 0);
    parent_mft_record_number.offset_hex = "0x" + m_ar.toHex();
    parent_mft_record_number.length = "6";
    parent_mft_record_number.value = QString::number(utility_obj.convert_bytearray_to_number_64bit_unsigned(data.mid(offset_to_attr_content + 0, 6).append((char)0).append((char)0), LITTLE_ENDIAN_HEX));
    parent_mft_record_number.value_hex = data.mid(offset_to_attr_content + 0, 6).toHex();
    parent_mft_record_number.description = "Parent MFT Record Number";
    list.append(parent_mft_record_number);

    common_structure_for_data_table parent_directory_seq;
    parent_directory_seq.offset_dec = QString::number(offset_to_attr_content + 6);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 6);
    parent_directory_seq.offset_hex = "0x" + m_ar.toHex();
    parent_directory_seq.length = "2";
    parent_directory_seq.value = QString::number(utility_obj.convert_bytearray_to_number_16bit_unsigned(data.mid(offset_to_attr_content + 6, 2), LITTLE_ENDIAN_HEX));
    parent_directory_seq.value_hex = data.mid(offset_to_attr_content + 6, 2).toHex();
    parent_directory_seq.description = "Parent Directory Sequence Number";
    list.append(parent_directory_seq);

    common_structure_for_data_table filename_length;
    filename_length.offset_dec = QString::number(offset_to_attr_content + 64);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 64);
    filename_length.offset_hex = "0x" + m_ar.toHex();
    filename_length.length = "1";
    int filename_length_int = data.mid(offset_to_attr_content + 64, 1).toHex().toUInt(0, 16);
    filename_length.value = QString::number(data.mid(offset_to_attr_content + 64, 1).toHex().toLong(0, 16));
    filename_length.value_hex = data.mid(offset_to_attr_content + 64, 1).toHex();
    filename_length.description = "Filename Length";
    list.append(filename_length);

    common_structure_for_data_table filename_type;
    filename_type.offset_dec = QString::number(offset_to_attr_content + 65);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 65);
    filename_type.offset_hex = "0x" + m_ar.toHex();
    filename_type.length = "1";
    filename_type.value = filename_type_to_string(data.mid(offset_to_attr_content + 65, 1));
    filename_type.value_hex = data.mid(offset_to_attr_content + 65, 1).toHex();
    filename_type.description = "Filename Type";
    list.append(filename_type);


    common_structure_for_data_table filename;
    filename.offset_dec = QString::number(offset_to_attr_content + 66);
    m_ar.clear(); m_ar.append(offset_to_attr_content + 66);
    filename.offset_hex = "0x" + m_ar.toHex();
    filename.length = QString::number(filename_length_int);
    filename.value = QString::fromUtf16((ushort *)data.mid(offset_to_attr_content + 66, filename_length_int * 2).data(), filename_length_int);
    filename.value_hex = data.mid(offset_to_attr_content + 66, filename_length_int).toHex();
    filename.description = "Filename";
    list.append(filename);


    return list;
}

QString ntfs_parser::standard_attr_flags_to_string(QByteArray ar)
{
    QStringList flag_string_list;

    if(ar.at(0) & 0x01)
        flag_string_list.append("Read Only");

    if(ar.at(0) & 0x02)
        flag_string_list.append("Hidden File");


    if(ar.at(0) & 0x04)
        flag_string_list.append("System File");


    if(ar.at(0) & 0x20)
        flag_string_list.append("Archive");


    if(ar.at(0) & 0x40)
        flag_string_list.append("Device");


    if(ar.at(0) & 0x80)
        flag_string_list.append("Normal");


    if(ar.at(1) & 0x01)
        flag_string_list.append("Temporary");

    if(ar.at(1) & 0x02)
        flag_string_list.append("Sparse File");

    if(ar.at(1) & 0x04)
        flag_string_list.append("Reparse Point");

    if(ar.at(1) & 0x08)
        flag_string_list.append("Compressed File");


    if(ar.at(1) & 0x10)
        flag_string_list.append("Offline");


    if(ar.at(1) & 0x20)
        flag_string_list.append("Content not indexed");


    if(ar.at(1) & 0x40)
        flag_string_list.append("Encrypted");


    if(flag_string_list.size() == 0)
        return QString("");

    return flag_string_list.join(",");

}
