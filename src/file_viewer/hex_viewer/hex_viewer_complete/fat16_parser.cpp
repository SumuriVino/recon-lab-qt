#include "fat16_parser.h"

fat16_parser::fat16_parser()
{

}

QList<common_structure_for_data_table> fat16_parser::pub_fat16_vbr_data(QByteArray data)
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
    sectors_per_fat.offset_dec = "22";
    sectors_per_fat.offset_hex = "0x16";
    sectors_per_fat.length = "2";
    sectors_per_fat.value =  QString::number(utility_obj.convert_bytearray_to_number_16bit(data.mid(22, 2), LITTLE_ENDIAN_HEX));
    sectors_per_fat.value_hex = data.mid(22, 2).toHex();
    sectors_per_fat.description = "Sectors Per FAT";
    list.append(sectors_per_fat);



    common_structure_for_data_table volume_serial_number;
    volume_serial_number.offset_dec = "39";
    volume_serial_number.offset_hex = "0x27";
    volume_serial_number.length = "4";
    volume_serial_number.value =  QString::number(data.mid(39, 4).toHex().toLongLong(0, 16));
    volume_serial_number.value_hex = data.mid(26, 4).toHex();
    volume_serial_number.description = "Volume Serial Number";
    list.append(volume_serial_number);

    common_structure_for_data_table volume_label;
    volume_label.offset_dec = "43";
    volume_label.offset_hex = "0x2b";
    volume_label.length = "11";
    volume_label.value =  data.mid(43, 11);;
    volume_label.value_hex = data.mid(43, 11).toHex();
    volume_label.description = "Volume Label";
    list.append(volume_label);


    common_structure_for_data_table fs;
    fs.offset_dec = "54";
    fs.offset_hex = "0x36";
    fs.length = "8";
    fs.value =  data.mid(54, 8);
    fs.value_hex = data.mid(54, 8).toHex();
    fs.description = "File System";
    list.append(fs);

    return list;


}
qint64 fat16_parser::pub_get_fat1_address(QByteArray fat16_vbr)
{
    QByteArray bytes_per_sector = fat16_vbr.mid(0x0b, 2);

    QByteArray reserved_sectors = fat16_vbr.mid(0x0e, 2);

    QByteArray sectors_per_fat = fat16_vbr.mid(0x24, 2);

    int bytes_per_sector_int, reserved_sectors_int, sectors_per_fat_int;

    bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);

    reserved_sectors_int = utility_obj.convert_bytearray_to_number_16bit(reserved_sectors, LITTLE_ENDIAN_HEX);

    sectors_per_fat_int = utility_obj.convert_bytearray_to_number_16bit(sectors_per_fat, LITTLE_ENDIAN_HEX);

    return bytes_per_sector_int * (reserved_sectors_int);
}

qint64 fat16_parser::pub_get_fat2_address(QByteArray fat16_vbr)
{
    QByteArray bytes_per_sector = fat16_vbr.mid(0x0b, 2);

    QByteArray reserved_sectors = fat16_vbr.mid(0x0e, 2);

    QByteArray sectors_per_fat = fat16_vbr.mid(0x16, 2);

    int bytes_per_sector_int, reserved_sectors_int, sectors_per_fat_int;

    bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);

    reserved_sectors_int = utility_obj.convert_bytearray_to_number_16bit(reserved_sectors, LITTLE_ENDIAN_HEX);

    sectors_per_fat_int = utility_obj.convert_bytearray_to_number_16bit(sectors_per_fat, LITTLE_ENDIAN_HEX);

    return bytes_per_sector_int * (reserved_sectors_int + sectors_per_fat_int);

}

qint64 fat16_parser::pub_get_root_directory_address(QByteArray fat16_vbr)
{
    QByteArray bytes_per_sector = fat16_vbr.mid(0x0b, 2);

    QByteArray reserved_sectors = fat16_vbr.mid(0x0e, 2);

    QByteArray sectors_per_fat = fat16_vbr.mid(0x16, 2);

    int bytes_per_sector_int, reserved_sectors_int, sectors_per_fat_int;

    bytes_per_sector_int = utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);

    reserved_sectors_int = utility_obj.convert_bytearray_to_number_16bit(reserved_sectors, LITTLE_ENDIAN_HEX);

    sectors_per_fat_int = utility_obj.convert_bytearray_to_number_16bit(sectors_per_fat, LITTLE_ENDIAN_HEX);

    return bytes_per_sector_int * (reserved_sectors_int + sectors_per_fat_int + sectors_per_fat_int);

}

qint64 fat16_parser::pub_get_cluster_number(QByteArray data)
{
    QByteArray ar = data.mid(0x1a, 2);

    return utility_obj.convert_bytearray_to_number_16bit_unsigned(ar, LITTLE_ENDIAN_HEX);
}

int fat16_parser::pub_get_bytes_per_sector(QByteArray fat_vbr)
{
    QByteArray bytes_per_sector = fat_vbr.mid(0x0b, 2);

    return utility_obj.convert_bytearray_to_number_16bit(bytes_per_sector, LITTLE_ENDIAN_HEX);
}

int fat16_parser::pub_get_sectors_per_cluster(QByteArray fat_vbr)
{
    QByteArray sector_per_cluster = fat_vbr.mid(0x0d, 1);

    return sector_per_cluster.toHex().toUInt();
}

int fat16_parser::pub_get_fat_sector_count(QByteArray fat_vbr)
{
    return  utility_obj.convert_bytearray_to_number_16bit(fat_vbr.mid(22, 2), LITTLE_ENDIAN_HEX);
}


QList<common_structure_for_data_table> fat16_parser::pub_SFN_data(QByteArray data)
{
    QList<common_structure_for_data_table> list;

    common_structure_for_data_table filename;
    filename.offset_dec = "0";
    filename.offset_hex = "0x00";
    filename.length = "8";
    filename.value = data.mid(0, 8);
    filename.value_hex = data.mid(0, 8).toHex();
    filename.description = "Filename";
    list.append(filename);

    common_structure_for_data_table extension;
    extension.offset_dec = "8";
    extension.offset_hex = "0x08";
    extension.length = "3";
    extension.value = data.mid(8, 3);
    extension.value_hex = data.mid(8, 3).toHex();
    extension.description = "File Extension";
    list.append(extension);


    common_structure_for_data_table attr;
    attr.offset_dec = "11";
    attr.offset_hex = "0x0b";
    attr.length = "1";
    attr.value = attribute_name((unsigned char)data.mid(11, 1).at(0));
    attr.value_hex = data.mid(11, 1).toHex();
    attr.description = "File Attribute";
    list.append(attr);

    common_structure_for_data_table millisecond;
    millisecond.offset_dec = "13";
    millisecond.offset_hex = "0x0d";
    millisecond.length = "1";
    millisecond.value = QString::number(data.mid(13, 1).toHex().toShort(0,16));
    millisecond.value_hex = data.mid(13, 1).toHex();
    millisecond.description = "Millisecond stamp";
    list.append(millisecond);

    common_structure_for_data_table file_creation_time;
    file_creation_time.offset_dec = "14";
    file_creation_time.offset_hex = "0x0e";
    file_creation_time.length = "2";
    file_creation_time.value = fat_time(data.mid(14, 2));
    file_creation_time.value_hex = data.mid(14, 2).toHex();
    file_creation_time.description = "File creation time";
    list.append(file_creation_time);

    common_structure_for_data_table file_creation_date;
    file_creation_date.offset_dec = "16";
    file_creation_date.offset_hex = "0x10";
    file_creation_date.length = "2";
    file_creation_date.value = fat_date(data.mid(16, 2));
    file_creation_date.value_hex = data.mid(16, 2).toHex();
    file_creation_date.description = "File creation date";
    list.append(file_creation_date);

    common_structure_for_data_table file_access_date;
    file_access_date.offset_dec = "18";
    file_access_date.offset_hex = "0x12";
    file_access_date.length = "2";
    file_access_date.value = fat_date(data.mid(18, 2));
    file_access_date.value_hex = data.mid(18, 2).toHex();
    file_access_date.description = "Last accessed date";
    list.append(file_access_date);

    common_structure_for_data_table cluster_high_word;
    cluster_high_word.offset_dec = "20";
    cluster_high_word.offset_hex = "0x14";
    cluster_high_word.length = "2";
    //cluster_high_word.value = fat_date(data.mid(20, 2));
    cluster_high_word.value_hex = data.mid(20, 2).toHex();
    cluster_high_word.description = "Cluster high word";
    list.append(cluster_high_word);

    common_structure_for_data_table last_write_time;
    last_write_time.offset_dec = "22";
    last_write_time.offset_hex = "0x16";
    last_write_time.length = "2";
    last_write_time.value = fat_time(data.mid(22, 2));
    last_write_time.value_hex = data.mid(22, 2).toHex();
    last_write_time.description = "Last write time";
    list.append(last_write_time);

    common_structure_for_data_table last_write_date;
    last_write_date.offset_dec = "24";
    last_write_date.offset_hex = "0x18";
    last_write_date.length = "2";
    last_write_date.value = fat_date(data.mid(24, 2));
    last_write_date.value_hex = data.mid(24, 2).toHex();
    last_write_date.description = "Last write date";
    list.append(last_write_date);

    common_structure_for_data_table cluster_low_word;
    cluster_low_word.offset_dec = "26";
    cluster_low_word.offset_hex = "0x1a";
    cluster_low_word.length = "2";
    //cluster_high_word.value = fat_date(data.mid(20, 2));
    cluster_low_word.value_hex = data.mid(26, 2).toHex();
    cluster_low_word.description = "Cluster low word";
    list.append(cluster_low_word);

    common_structure_for_data_table file_size;
    file_size.offset_dec = "28";
    file_size.offset_hex = "0x1C";
    file_size.length = "2";
    file_size.value =  QString::number(utility_obj.convert_bytearray_to_number_32bit(data.mid(28, 4), LITTLE_ENDIAN_HEX));
    file_size.value_hex = data.mid(28, 2).toHex();
    file_size.description = "File size";
    list.append(file_size);

    return list;
}

QString fat16_parser::media_desctriptor_type_to_int(unsigned char val)
{
    QString str;

    if(val == 0xF8)
        str = "Fixed Media";

    if(val == 0xF0)
        str = "Removable Media";

    return str;
}

QString fat16_parser::attribute_name(unsigned char val)
{

    QString attr;
    if(val & 0x01)
        attr.append("Read Only ");

    if(val & 0x02)
        attr.append("Hidden ");

    if(val & 0x04)
        attr.append("System ");

    if(val & 0x08)
        attr.append("Volume Label ");

    if(val & 0x10)
        attr.append("Directory ");

    if(val & 0x20)
        attr.append("Archive On ");

    return attr;
}

QString fat16_parser::fat_time(QByteArray trr)
{

    unsigned char first_byte = (unsigned char)trr.at(1);
    unsigned char second_byte = (unsigned char)trr.at(0);


    unsigned char hours = first_byte;

    hours= hours >> 3;


    unsigned char minutes_first_part = first_byte;
    unsigned char minutes_second_part = second_byte;

    minutes_first_part = minutes_first_part << 5;

    minutes_second_part = minutes_second_part >> 5;

    unsigned char minuts = minutes_first_part >> 2 | minutes_second_part ;

    unsigned char seconds = second_byte;
    seconds = seconds >> 3;
    seconds = seconds * 2;

    return (QString::number(hours) + ":" + QString::number(minuts) + ":" + QString::number(seconds));

}

QString fat16_parser::fat_date(QByteArray ar)
{
    unsigned char first_byte = (unsigned char)ar.at(1);
    unsigned char second_byte = (unsigned char)ar.at(0);


    unsigned char year_1 = first_byte;

    year_1 = year_1 >> 1;

    unsigned short int year = year_1 + 1980;

    unsigned char month_1 = first_byte;
    month_1 = month_1 << 7;

    unsigned char month_2 = second_byte;
    month_2 = month_2 >> 5;

    unsigned char month;
    month = month_1 >> 4 | month_2;

    unsigned char day = second_byte;
    day = day & (unsigned char)0x1F;

    QString month_str = QString::number(month);
    if(month < 10)
        month_str.prepend("0");

    return (QString::number(year) + "-" + month_str + "-" + QString::number(day));


}
