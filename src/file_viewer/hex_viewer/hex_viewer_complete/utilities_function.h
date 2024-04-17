#ifndef UTILITIES_FUNCTION_H
#define UTILITIES_FUNCTION_H

#include <QtCore>
#include "common_header.h"

class utilities_function
{
public:
    utilities_function();
    qint32 convert_bytearray_to_number_32bit(QByteArray ar, int order);
    qint16 convert_bytearray_to_number_16bit(QByteArray ar, int order);
    qint64 convert_bytearray_to_number_64bit(QByteArray ar, int order);

    qint32 convert_bytearray_to_number_24_bit(QByteArray ar, int order);

    QString convert_8bit_to_binary(char ch);

    unsigned int convert_bytearray_to_number_32bit_unsigned(QByteArray ar, int order);
    unsigned short convert_bytearray_to_number_16bit_unsigned(QByteArray ar, int order);
    unsigned short convert_bytearray_to_number_24bit_unsigned(QByteArray ar, int order);
    unsigned long long convert_bytearray_to_number_64bit_unsigned(QByteArray ar, int order);

    qint64 convert_ntfs_time_to_unix_epoch(QByteArray ar);
    QString convert_ntfs_time_to_unix_epoch_string(QByteArray ar);

    QString fat_time(QByteArray trr);
    QString fat_date(QByteArray ar);
};

#endif // UTILITIES_FUNCTION_H
