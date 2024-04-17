#include "utilities_function.h"

utilities_function::utilities_function()
{

}

qint32 utilities_function::convert_bytearray_to_number_32bit(QByteArray ar, int order)
{
    if(ar.size() != 4)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        qint32 value = ar.toHex().toUInt(0, 16);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;
        mar.append(ar.at(3));
        mar.append(ar.at(2));
        mar.append(ar.at(1));
        mar.append(ar.at(0));

        qint32 value = mar.toHex().toUInt(0, 16);

        return value;
    }

    return 0;
}

qint16 utilities_function::convert_bytearray_to_number_16bit(QByteArray ar, int order)
{
    if(ar.size() != 2)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        qint16 value = ar.toHex().toUShort(0, 16);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;

        mar.append(ar.at(1));
        mar.append(ar.at(0));

        qint16 value = mar.toHex().toUShort(0, 16);

        return value;
    }

    return 0;

}

qint64 utilities_function::convert_bytearray_to_number_64bit(QByteArray ar, int order)
{
    if(ar.size() != 8)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        qint64 value = ar.toHex().toLongLong(0, 16);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;

        mar.append(ar.at(7));
        mar.append(ar.at(6));
        mar.append(ar.at(5));
        mar.append(ar.at(4));
        mar.append(ar.at(3));
        mar.append(ar.at(2));
        mar.append(ar.at(1));
        mar.append(ar.at(0));

        qint64 value = mar.toHex().toULongLong(0, 16);

        return value;
    }

    return 0;

}

qint32 utilities_function::convert_bytearray_to_number_24_bit(QByteArray ar, int order)
{

    int high_value = 16777216;

    int actual_value = ar.toHex().toLong(0, 16);


    if(actual_value & 0x00800000){
        return actual_value - high_value;
    }
    else {
        return actual_value ;
    }

//    struct my_packed_data
//    {
//        int   my_3_byte_int        : 24;
//          char      rest_for_other_scrap : 8;
//    };

//    my_packed_data m1;

//    //m1.my_3_byte_int = 0x92 + (0x92 << 8) + (0x92 << 16);

//    if(ar.size() != 3)
//        return 0;

//    int ch1 = (unsigned char)ar.at(0);
//    int ch2 = (unsigned char)ar.at(1);
//    int ch3 = (unsigned char)ar.at(2);

//    if(order == BIG_ENDIAN_HEX)
//    {
//        qint32 value;
//        m1.my_3_byte_int = ch1 + (ch2 << 8) + (ch3 << 16);

//        value = m1.my_3_byte_int;
//        return value;
//    }

//    if(order == LITTLE_ENDIAN_HEX)
//    {
//        qint32 value;
//        m1.my_3_byte_int = ch3 + (ch2 << 8) + (ch1 << 16);

//        value = m1.my_3_byte_int;
//        return value;

//    }

    return 0;

}

QString utilities_function::convert_8bit_to_binary(char ch)
{
    QString binddata;

    unsigned char mak = 1;

    for(int i =0; i < 8; i++)
    {
        if((mak & ch))
        {
            binddata.prepend("1");
        }
        else {
            binddata.prepend("0");
        }

        mak = (unsigned )(mak << 1);
    }

    return binddata;
}

unsigned int utilities_function::convert_bytearray_to_number_32bit_unsigned(QByteArray ar, int order)
{
    if(ar.size() != 4)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        unsigned int value = ar.toHex().toUInt(0, 16);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;
        mar.append(ar.at(3));
        mar.append(ar.at(2));
        mar.append(ar.at(1));
        mar.append(ar.at(0));

        unsigned int value = mar.toHex().toUInt(0, 16);

        return value;
    }

    return 0;

}

unsigned short utilities_function::convert_bytearray_to_number_16bit_unsigned(QByteArray ar, int order)
{
    if(ar.size() != 2)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        unsigned short value = ar.toHex().toUShort(0, 16);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;

        mar.append(ar.at(1));
        mar.append(ar.at(0));

        unsigned short value = mar.toHex().toUShort(0, 16);

        return value;
    }

    return 0;

}

unsigned short utilities_function::convert_bytearray_to_number_24bit_unsigned(QByteArray ar, int order)
{
    if(ar.size() != 4)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        unsigned short value = ar.toHex().toUShort(0, 24);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;

        mar.append(ar.at(1));
        mar.append(ar.at(0));

        unsigned short value = mar.toHex().toUShort(0, 24);

        return value;
    }

    return 0;

}

unsigned long long utilities_function::convert_bytearray_to_number_64bit_unsigned(QByteArray ar, int order)
{

    if(ar.size() != 8)
        return 0;

    if(order == BIG_ENDIAN_HEX)
    {
        unsigned long long value = ar.toHex().toULongLong(0, 16);

        return value;
    }

    if(order == LITTLE_ENDIAN_HEX)
    {
        QByteArray mar;

        mar.append(ar.at(7));
        mar.append(ar.at(6));
        mar.append(ar.at(5));
        mar.append(ar.at(4));
        mar.append(ar.at(3));
        mar.append(ar.at(2));
        mar.append(ar.at(1));
        mar.append(ar.at(0));

        unsigned long long value = mar.toHex().toULongLong(0, 16);

        return value;
    }

    return 0;
}

qint64 utilities_function::convert_ntfs_time_to_unix_epoch(QByteArray ar)
{
    if(ar.size() != 8)
        return 0;

    qint64 windows_timestamp = convert_bytearray_to_number_64bit(ar, LITTLE_ENDIAN_HEX);

    qint64 filetime_posix = 11644473600000 * 10000;
    return qint64(((windows_timestamp - filetime_posix)))/10000000;
}

QString utilities_function::convert_ntfs_time_to_unix_epoch_string(QByteArray ar)
{
    QDateTime dt;
    dt.setSecsSinceEpoch(Qt::UTC);

    dt.setMSecsSinceEpoch(convert_ntfs_time_to_unix_epoch(ar));

    return dt.toString("MM-dd-yyyy_hh:mm:ss");
}


QString utilities_function::fat_time(QByteArray trr)
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

QString utilities_function::fat_date(QByteArray ar)
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
