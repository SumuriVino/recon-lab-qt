#include "gpt_parser.h"

gpt_parser::gpt_parser()
{

}

void gpt_parser::parse(QByteArray data)
{
    gpt_data = data;

}

QList<common_structure_for_data_table> gpt_parser::gpt_protective_mbr_data()
{

    QList<common_structure_for_data_table> list;

    common_structure_for_data_table start_sector;
    start_sector.offset_dec = "8";
    start_sector.offset_hex = "0x08";
    start_sector.length = "4";
    start_sector.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(446 + 8, 4), LITTLE_ENDIAN_HEX));
    gpt_start_sector = utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(446 + 8, 4), LITTLE_ENDIAN_HEX);
    start_sector.value_hex = gpt_data.mid(446 + 8, 4).toHex();
    start_sector.description = "Start Sector";
    list.append(start_sector);

    common_structure_for_data_table partition_size;
    partition_size.offset_dec = "12";
    partition_size.offset_hex = "0x0C";
    partition_size.length = "4";
    partition_size.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(446 + 12, 4), LITTLE_ENDIAN_HEX));
    partition_size.value_hex = gpt_data.mid(446 + 12, 4).toHex();
    partition_size.description = "Partition Size";
    list.append(partition_size);

    return list;
}

QList<common_structure_for_data_table> gpt_parser::gpt_header()
{
    int gpt_header_start_offset = 512;
    QList<common_structure_for_data_table> list;

    common_structure_for_data_table signature;
    signature.offset_dec = "0";
    signature.offset_hex = "0x00";
    signature.length = "8";
    signature.value = QString::fromLocal8Bit(gpt_data.mid(gpt_header_start_offset + 0, 8));
    signature.value_hex = gpt_data.mid(gpt_header_start_offset + 0, 8).toHex();
    signature.description = "Partition Size";
    list.append(signature);

    common_structure_for_data_table rev_number;
    rev_number.offset_dec = "8";
    rev_number.offset_hex = "0x08";
    rev_number.length = "4";
    rev_number.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(gpt_header_start_offset + 8, 4), LITTLE_ENDIAN_HEX));
    rev_number.value_hex = gpt_data.mid(gpt_header_start_offset + 8, 4).toHex();
    rev_number.description = "Revision Number";
    list.append(rev_number);

    common_structure_for_data_table gpt_header_size;
    gpt_header_size.offset_dec = "12";
    gpt_header_size.offset_hex = "0x0C";
    gpt_header_size.length = "4";
    gpt_header_size.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(gpt_header_start_offset + 12, 4), LITTLE_ENDIAN_HEX));
    gpt_header_size.value_hex = gpt_data.mid(gpt_header_start_offset + 12, 4).toHex();
    gpt_header_size.description = "GPT Header Size";
    list.append(gpt_header_size);

    common_structure_for_data_table crc32;
    crc32.offset_dec = "16";
    crc32.offset_hex = "0x10";
    crc32.length = "4";
    crc32.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(gpt_header_start_offset + 16, 4), LITTLE_ENDIAN_HEX));
    crc32.value_hex = gpt_data.mid(gpt_header_start_offset + 16, 4).toHex();
    crc32.description = "CRC32 Checksum";
    list.append(crc32);


    common_structure_for_data_table lba_current_gpt_header_struct;
    lba_current_gpt_header_struct.offset_dec = "24";
    lba_current_gpt_header_struct.offset_hex = "0x18";
    lba_current_gpt_header_struct.length = "8";
    lba_current_gpt_header_struct.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(gpt_header_start_offset + 24, 8), LITTLE_ENDIAN_HEX));
    lba_current_gpt_header_struct.value_hex = gpt_data.mid(gpt_header_start_offset + 24, 8).toHex();
    lba_current_gpt_header_struct.description = "LBA current GPT Header Structure";
    list.append(lba_current_gpt_header_struct);

    common_structure_for_data_table lba_gpt_backup_copy;
    lba_gpt_backup_copy.offset_dec = "32";
    lba_gpt_backup_copy.offset_hex = "0x20";
    lba_gpt_backup_copy.length = "8";
    lba_gpt_backup_copy.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(gpt_header_start_offset + 32, 8), LITTLE_ENDIAN_HEX));
    lba_gpt_backup_copy.value_hex = gpt_data.mid(gpt_header_start_offset + 32, 8).toHex();
    lba_gpt_backup_copy.description = "LBA backup copy of GPT";
    list.append(lba_gpt_backup_copy);


    common_structure_for_data_table lba_start_partition_area;
    lba_start_partition_area.offset_dec = "40";
    lba_start_partition_area.offset_hex = "0x28";
    lba_start_partition_area.length = "8";
    lba_start_partition_area.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(gpt_header_start_offset + 40, 8), LITTLE_ENDIAN_HEX));
    lba_start_partition_area.value_hex = gpt_data.mid(gpt_header_start_offset + 40, 8).toHex();
    lba_start_partition_area.description = "LBA of Start of partition area";
    list.append(lba_start_partition_area);

    common_structure_for_data_table lba_end_partition_area;
    lba_end_partition_area.offset_dec = "48";
    lba_end_partition_area.offset_hex = "0x30";
    lba_end_partition_area.length = "8";
    lba_end_partition_area.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(gpt_header_start_offset + 48, 8), LITTLE_ENDIAN_HEX));
    lba_end_partition_area.value_hex = gpt_data.mid(gpt_header_start_offset + 48, 8).toHex();
    lba_end_partition_area.description = "LBA of End of partition area";
    list.append(lba_end_partition_area);

    common_structure_for_data_table disk_guid;
    disk_guid.offset_dec = "56";
    disk_guid.offset_hex = "0x38";
    disk_guid.length = "16";
    disk_guid.value = gpt_data.mid(gpt_header_start_offset + 56, 16).toHex();
    disk_guid.value_hex = gpt_data.mid(gpt_header_start_offset + 56, 16).toHex();
    disk_guid.description = "DISK GUID";
    list.append(disk_guid);

    common_structure_for_data_table lba_start_partition_tables;
    lba_start_partition_tables.offset_dec = "72";
    lba_start_partition_tables.offset_hex = "0x48";
    lba_start_partition_tables.length = "8";
    lba_start_partition_tables.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(gpt_header_start_offset + 72, 8), LITTLE_ENDIAN_HEX));
    lba_start_partition_tables.value_hex = gpt_data.mid(gpt_header_start_offset + 72, 8).toHex();
    lba_start_partition_tables.description = "LBA of start of partition tables";
    list.append(lba_start_partition_tables);
    partition_table_start_sector_int = utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(gpt_header_start_offset + 72, 8), LITTLE_ENDIAN_HEX);

    common_structure_for_data_table partition_entries_count;
    partition_entries_count.offset_dec = "80";
    partition_entries_count.offset_hex = "0x50";
    partition_entries_count.length = "4";
    partition_entries_count.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(gpt_header_start_offset + 80, 4), LITTLE_ENDIAN_HEX));
    partition_entries_count.value_hex = gpt_data.mid(gpt_header_start_offset + 80, 4).toHex();
    partition_entries_count.description = "Partition entries count";
    list.append(partition_entries_count);

    common_structure_for_data_table partition_entries_size;
    partition_entries_size.offset_dec = "84";
    partition_entries_size.offset_hex = "0x54";
    partition_entries_size.length = "4";
    partition_entries_size.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(gpt_header_start_offset + 84, 4), LITTLE_ENDIAN_HEX));
    partition_entries_size.value_hex = gpt_data.mid(gpt_header_start_offset + 84, 4).toHex();
    partition_entries_size.description = "Partition entry size";
    list.append(partition_entries_size);

    common_structure_for_data_table crc32_partition_table;
    crc32_partition_table.offset_dec = "88";
    crc32_partition_table.offset_hex = "0x58";
    crc32_partition_table.length = "4";
    crc32_partition_table.value = QString::number(utility_obj.convert_bytearray_to_number_32bit(gpt_data.mid(gpt_header_start_offset + 88, 4), LITTLE_ENDIAN_HEX));
    crc32_partition_table.value_hex = gpt_data.mid(gpt_header_start_offset + 88, 4).toHex();
    crc32_partition_table.description = "CRC32 Partition table";
    list.append(crc32_partition_table);


    return list;
}

QList<QList<common_structure_for_data_table> > gpt_parser::gpt_partition_tables()
{
    int gpt_partition_tables_start_offset = partition_table_start_sector_int * 512;
    QList<QList<common_structure_for_data_table>> master_list;

    int partition_table_offset = gpt_partition_tables_start_offset;

    for(int i = 0 ; i < 128; i++)
    {
        QList<common_structure_for_data_table> list;

        partition_table_offset = i * 128 + partition_table_offset;

        common_structure_for_data_table partition_type_guid;
        partition_type_guid.offset_dec = "00";
        partition_type_guid.offset_hex = "0x00";
        partition_type_guid.length = "16";
        partition_type_guid.value_hex = gpt_data.mid(partition_table_offset + 0, 16).toHex();
        partition_type_guid.description = "Partition type GUID";
        list.append(partition_type_guid);

        if(is_partition_entry_blank(gpt_data.mid(partition_table_offset + 0, 16)))
            continue;

        common_structure_for_data_table unique_partition_guid;
        unique_partition_guid.offset_dec = "16";
        unique_partition_guid.offset_hex = "0x10";
        unique_partition_guid.length = "16";
        unique_partition_guid.value_hex = gpt_data.mid(partition_table_offset + 16, 16).toHex();
        unique_partition_guid.description = "Unique Partition GUID";
        list.append(unique_partition_guid);

        common_structure_for_data_table start_lba;
        start_lba.offset_dec = "32";
        start_lba.offset_hex = "0x20";
        start_lba.length = "8";
        start_lba.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(partition_table_offset + 32, 8), LITTLE_ENDIAN_HEX));
        start_lba.value_hex = gpt_data.mid(partition_table_offset + 32, 8).toHex();
        start_lba.description = "Start LBA";
        list.append(start_lba);

        common_structure_for_data_table end_lba;
        end_lba.offset_dec = "40";
        end_lba.offset_hex = "0x28";
        end_lba.length = "8";
        end_lba.value = QString::number(utility_obj.convert_bytearray_to_number_64bit(gpt_data.mid(partition_table_offset + 40, 8), LITTLE_ENDIAN_HEX));
        end_lba.value_hex = gpt_data.mid(partition_table_offset + 40, 8).toHex();
        end_lba.description = "End LBA";
        list.append(end_lba);


        common_structure_for_data_table partition_name;
        partition_name.offset_dec = "56";
        partition_name.offset_hex = "0x38";
        partition_name.length = "72";
        partition_name.value = gpt_data.mid(partition_table_offset + 56, 72);
        partition_name.value_hex = gpt_data.mid(partition_table_offset + 56, 72).toHex();
        partition_name.description = "Partition Name";
        list.append(partition_name);


        master_list.append(list);

    }
    return master_list;
}

bool gpt_parser::is_partition_entry_blank(QByteArray ar)
{

    for(int i = 0; i < 16; i++)
    {
        if(ar.at(i) != 0)
            return false;
    }

    return true;
}
