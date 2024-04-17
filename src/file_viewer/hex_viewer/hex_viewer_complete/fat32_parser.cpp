#include "fat32_parser.h"

fat32_parser::fat32_parser()
{

}

QList<common_structure_for_data_table> fat32_parser::pub_fat32_vbr_data(QByteArray data)
{
    QList<common_structure_for_data_table> list;

    common_structure_for_data_table jump_instruction;
    jump_instruction.offset_dec = "0";
    jump_instruction.offset_hex = "0x00";
    jump_instruction.length = "3";
    jump_instruction.value = data.mid(0, 3).toHex();
    jump_instruction.value_hex = data.mid(0, 3).toHex();
    jump_instruction.description = "Jump Instruction";
    list.append(jump_instruction);


    common_structure_for_data_table oem;
    oem.offset_dec = "3";
    oem.offset_hex = "0x03";
    oem.length = "8";
    oem.value = data.mid(3, 8);
    oem.value_hex = data.mid(3, 8).toHex();
    oem.description = "OEM";
    list.append(oem);


    common_structure_for_data_table bytes_per_sector;
    bytes_per_sector.offset_dec = "11";
    bytes_per_sector.offset_hex = "0x0b";
    bytes_per_sector.length = "2";
    bytes_per_sector.value =  QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(11, 2), LITTLE_ENDIAN_HEX));
    bytes_per_sector.value_hex = data.mid(11, 2).toHex();
    bytes_per_sector.description = "Bytes Per Sector";
    list.append(bytes_per_sector);


    common_structure_for_data_table sectors_per_cluster;
    sectors_per_cluster.offset_dec = "13";
    sectors_per_cluster.offset_hex = "0x0d";
    sectors_per_cluster.length = "1";
    sectors_per_cluster.value =  QString::number(data.mid(13, 1).toHex().toLongLong(0, 16));
    sectors_per_cluster.value_hex = data.mid(13, 1).toHex();
    sectors_per_cluster.description = "Sectors Per Cluster";
    list.append(sectors_per_cluster);

    common_structure_for_data_table reserved_sectors;
    reserved_sectors.offset_dec = "14";
    reserved_sectors.offset_hex = "0x0e";
    reserved_sectors.length = "2";
    reserved_sectors.value =  QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(14, 2), LITTLE_ENDIAN_HEX));
    reserved_sectors.value_hex = data.mid(14, 2).toHex();
    reserved_sectors.description = "Reserved Sectors";
    list.append(reserved_sectors);

    common_structure_for_data_table no_of_fats;
    no_of_fats.offset_dec = "16";
    no_of_fats.offset_hex = "0x10";
    no_of_fats.length = "1";
    no_of_fats.value =  QString::number(data.mid(16, 1).toHex().toLongLong(0, 16));
    no_of_fats.value_hex = data.mid(16, 1).toHex();
    no_of_fats.description = "Number of FAT's";
    list.append(no_of_fats);

    //    common_structure_for_data_table total_sectors;
    //    total_sectors.offset_dec = "19";
    //    total_sectors.offset_hex = "0x13";
    //    total_sectors.length = "2";
    //    total_sectors.value =  QString::number(data.mid(19, 2).toHex().toLongLong(0, 16));
    //    total_sectors.value_hex = data.mid(19, 2).toHex();
    //    total_sectors.description = "Reserved Sectors";
    //    list.append(total_sectors);

    common_structure_for_data_table media_descriptot;
    media_descriptot.offset_dec = "21";
    media_descriptot.offset_hex = "0x15";
    media_descriptot.length = "1";
    media_descriptot.value =  media_desctriptor_type_to_int((unsigned char)data.mid(21, 1).at(0));
    media_descriptot.value_hex = data.mid(21, 1).toHex();
    media_descriptot.description = "Media Descriptor";
    list.append(media_descriptot);


    common_structure_for_data_table sectors_per_fat;
    sectors_per_fat.offset_dec = "36";
    sectors_per_fat.offset_hex = "0x24";
    sectors_per_fat.length = "4";
    sectors_per_fat.value =  QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(36, 4), LITTLE_ENDIAN_HEX));
    sectors_per_fat.value_hex = data.mid(36, 4).toHex();
    sectors_per_fat.description = "Sectors Per FAT";
    list.append(sectors_per_fat);



    common_structure_for_data_table volume_serial_number;
    volume_serial_number.offset_dec = "67";
    volume_serial_number.offset_hex = "0x43";
    volume_serial_number.length = "4";
    volume_serial_number.value =  QString::number(data.mid(67, 4).toHex().toLongLong(0, 16));
    volume_serial_number.value_hex = data.mid(67, 4).toHex();
    volume_serial_number.description = "Volume Serial Number";
    list.append(volume_serial_number);

    common_structure_for_data_table volume_label;
    volume_label.offset_dec = "71";
    volume_label.offset_hex = "0x47";
    volume_label.length = "11";
    volume_label.value =  data.mid(71, 11);;
    volume_label.value_hex = data.mid(71, 11).toHex();
    volume_label.description = "Volume Label";
    list.append(volume_label);


    common_structure_for_data_table fs;
    fs.offset_dec = "82";
    fs.offset_hex = "0x52";
    fs.length = "8";
    fs.value =  data.mid(82, 8);
    fs.value_hex = data.mid(82, 8).toHex();
    fs.description = "File System";
    list.append(fs);

    return list;


}

qint64 fat32_parser::pub_get_fat1_address(QByteArray fat32_vbr)
{
    QByteArray bytes_per_sector = fat32_vbr.mid(0x0b, 2);

    QByteArray reserved_sectors = fat32_vbr.mid(0x0e, 2);

    QByteArray sectors_per_fat = fat32_vbr.mid(0x24, 2);

    qint64 bytes_per_sector_int, reserved_sectors_int, sectors_per_fat_int;

    bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);

    reserved_sectors_int = utility_obj.convert_bytearray_to_number_16bit(reserved_sectors, LITTLE_ENDIAN_HEX);

    sectors_per_fat_int = utility_obj.convert_bytearray_to_number_16bit(sectors_per_fat, LITTLE_ENDIAN_HEX);

    return (bytes_per_sector_int * (reserved_sectors_int));
}

qint64 fat32_parser::pub_get_fat2_address(QByteArray fat32_vbr)
{
    QByteArray bytes_per_sector = fat32_vbr.mid(0x0b, 2);

    QByteArray reserved_sectors = fat32_vbr.mid(0x0e, 2);

    QByteArray sectors_per_fat = fat32_vbr.mid(0x24, 2);

    qint64 bytes_per_sector_int, reserved_sectors_int, sectors_per_fat_int;

    bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);

    reserved_sectors_int = utility_obj.convert_bytearray_to_number_16bit(reserved_sectors, LITTLE_ENDIAN_HEX);

    sectors_per_fat_int = utility_obj.convert_bytearray_to_number_16bit(sectors_per_fat, LITTLE_ENDIAN_HEX);

    return (bytes_per_sector_int * (reserved_sectors_int + sectors_per_fat_int));
}

qint64 fat32_parser::pub_get_root_directory_address(QByteArray fat32_vbr)
{
    QByteArray bytes_per_sector = fat32_vbr.mid(0x0b, 2);

    QByteArray reserved_sectors = fat32_vbr.mid(0x0e, 2);

    QByteArray sectors_per_fat = fat32_vbr.mid(0x24, 2);

    qint64 bytes_per_sector_int, reserved_sectors_int, sectors_per_fat_int;

    bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);

    reserved_sectors_int = utility_obj.convert_bytearray_to_number_16bit(reserved_sectors, LITTLE_ENDIAN_HEX);

    sectors_per_fat_int = utility_obj.convert_bytearray_to_number_16bit(sectors_per_fat, LITTLE_ENDIAN_HEX);

    return (bytes_per_sector_int * (reserved_sectors_int + sectors_per_fat_int + sectors_per_fat_int));
}

qint64 fat32_parser::pub_get_cluster_number(QByteArray data)
{
    QByteArray ar = data.mid(0x1a, 2);

    return utility_obj.convert_bytearray_to_number_16bit_unsigned(ar, LITTLE_ENDIAN_HEX);
}

int fat32_parser::pub_get_bytes_per_sector(QByteArray fat32_vbr)
{
    QByteArray bytes_per_sector = fat32_vbr.mid(0x0b, 2);

    return utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);
}

int fat32_parser::pub_get_sectors_per_cluster(QByteArray fat32_vbr)
{
    QByteArray sector_per_cluster = fat32_vbr.mid(0x0d, 1);

    return sector_per_cluster.toHex().toUInt();
}

int fat32_parser::pub_get_fat_sector_count(QByteArray fat32_vbr)
{
    return (utility_obj.convert_bytearray_to_number_32bit(fat32_vbr.mid(36, 4), LITTLE_ENDIAN_HEX));
}


QString fat32_parser::media_desctriptor_type_to_int(unsigned char val)
{
    QString str;

    if(val == 0xF8)
        str = "Fixed Media";

    if(val == 0xF0)
        str = "Removable Media";

    return str;
}
