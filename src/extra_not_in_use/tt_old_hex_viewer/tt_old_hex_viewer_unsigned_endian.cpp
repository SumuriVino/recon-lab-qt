#include "tt_old_hex_viewer.h"
#include "ui_tt_old_hex_viewer.h"

void hex_viewer::slot_cur_hex(QByteArray data_64_bit_arr)
{
    if(data_64_bit_arr.isEmpty())
        return;

    _8_bit_array = data_64_bit_arr.mid(0,1);
    _16_bit_array = data_64_bit_arr.mid(0,2);
    _32_bit_array = data_64_bit_arr.mid(0,4);

    show_mac_timestamp(_32_bit_array);
    show_unix_timestamp(_32_bit_array);

    if(checkBox_unsigned->isChecked())
    {
        show_binary_value(_8_bit_array);
        show_8_bit_signed_value(_8_bit_array, true);
        show_16_bit_signed_value(_16_bit_array,true);
        show_32_bit_signed_value(_32_bit_array,true);
    }
    else
    {
        show_binary_value(_8_bit_array);
        show_8_bit_signed_value(_8_bit_array, false);
        show_16_bit_signed_value(_16_bit_array,false);
        show_32_bit_signed_value(_32_bit_array,false);
    }
}

void hex_viewer::on_checkBox_unsigned_clicked(bool checked)
{
    if(_8_bit_array.isEmpty())
        return;

    if(_16_bit_array.isEmpty())
        return;

    if(_32_bit_array.isEmpty())
        return;

    if(!checkBox_big_endian->isChecked())
    {
        show_binary_value(_8_bit_array);
        show_8_bit_signed_value(_8_bit_array, checked);
        show_16_bit_signed_value(_16_bit_array,checked);
        show_32_bit_signed_value(_32_bit_array, checked);
    }
    else
    {
        _16_bit_converted_array = endian_converter(_16_bit_array);
        _32_bit_converted_array = endian_converter(_32_bit_array);

        show_binary_value(_8_bit_array);
        show_8_bit_signed_value(_8_bit_array, checked);
        show_16_bit_signed_value(_16_bit_converted_array,checked);
        show_32_bit_signed_value(_32_bit_converted_array, checked);
    }
}

void hex_viewer::on_checkBox_big_endian_clicked(bool checked)
{
    if(_8_bit_array.isEmpty())
        return;

    if(_16_bit_array.isEmpty())
        return;

    if(_32_bit_array.isEmpty())
        return;

    if(endian_checker() == MACRO_Hex_Viewer_Little_Endian_int)
    {
        if(checked)
        {
            _16_bit_converted_array = endian_converter(_16_bit_array);
            _32_bit_converted_array = endian_converter(_32_bit_array);

            show_mac_timestamp(_32_bit_converted_array);
            show_unix_timestamp(_32_bit_converted_array);

            if(checkBox_unsigned->isChecked())
            {
                show_16_bit_signed_value(_16_bit_converted_array,true);
                show_32_bit_signed_value(_32_bit_converted_array, true);
            }
            else
            {
                show_16_bit_signed_value(_16_bit_converted_array,false);
                show_32_bit_signed_value(_32_bit_converted_array, false);
            }
        }
        else
        {
            show_mac_timestamp(_32_bit_array);
            show_unix_timestamp(_32_bit_array);

            if(checkBox_unsigned->isChecked())
            {
                show_16_bit_signed_value(_16_bit_array,true);
                show_32_bit_signed_value(_32_bit_array, true);
            }
            else
            {
                show_16_bit_signed_value(_16_bit_array,false);
                show_32_bit_signed_value(_32_bit_array, false);
            }
        }
    }
    else if(endian_checker() == MACRO_Hex_Viewer_Big_Endian_int)
    {
        if(checked)
        {
            show_mac_timestamp(_32_bit_array);
            show_unix_timestamp(_32_bit_array);

            if(checkBox_unsigned->isChecked())
            {
                show_16_bit_signed_value(_16_bit_array,true);
                show_32_bit_signed_value(_32_bit_array, true);
            }
            else
            {
                show_16_bit_signed_value(_16_bit_array,false);
                show_32_bit_signed_value(_32_bit_array, false);
            }
        }
        else
        {
            _16_bit_converted_array = endian_converter(_16_bit_array);
            _32_bit_converted_array = endian_converter(_32_bit_array);

            show_mac_timestamp(_32_bit_converted_array);
            show_unix_timestamp(_32_bit_converted_array);

            if(checkBox_unsigned->isChecked())
            {
                show_16_bit_signed_value(_16_bit_converted_array,true);
                show_32_bit_signed_value(_32_bit_converted_array, true);
            }
            else
            {
                show_16_bit_signed_value(_16_bit_converted_array,false);
                show_32_bit_signed_value(_32_bit_converted_array, false);
            }
        }
    }
}

void hex_viewer::show_32_bit_signed_value(const QByteArray &data_arr, bool checked)
{
    if(checked)
    {
        uint32_t int_32_unsigned = data_arr.toHex().toLongLong(0,16);
        //        lineEdit_32_bit->setText(QString::number(int_32_unsigned));
        m_tablewidget_conversion_obj->item(enum_32_bit,1)->setText(QString::number(int_32_unsigned));
    }
    else if(!checked)
    {
        qint32 int_val_32 = data_arr.toHex().toLongLong(0,16);
        //        lineEdit_32_bit->setText(QString::number(int_val_32));
        m_tablewidget_conversion_obj->item(enum_32_bit,1)->setText(QString::number(int_val_32));
    }
}

void hex_viewer::show_16_bit_signed_value(const QByteArray &data_arr, bool checked)
{
    if(checked)
    {
        uint16_t _16_bit_unsigned = data_arr.toHex().toLongLong(0,16);
        //        lineEdit_16_bit->setText(QString::number(_16_bit_unsigned));
        m_tablewidget_conversion_obj->item(enum_16_bit,1)->setText(QString::number(_16_bit_unsigned));
    }
    else if(!checked)
    {
        qint16 int_val_16 = data_arr.toHex().toLongLong(0,16);
        //        lineEdit_16_bit->setText(QString::number(int_val_16));
        m_tablewidget_conversion_obj->item(enum_16_bit,1)->setText(QString::number(int_val_16));
    }
}

void hex_viewer::show_8_bit_signed_value(const QByteArray &data_arr, bool checked)
{
    if(checked)
    {
        uint8_t _unsigned_8_bit = data_arr.toHex().toInt(0,16);
        //        lineEdit_8_bit->setText(QString::number(_unsigned_8_bit));
        m_tablewidget_conversion_obj->item(enum_8_bit,1)->setText(QString::number(_unsigned_8_bit));
    }
    else if(!checked)
    {
        qint8 int_val_8 = data_arr.toHex().toInt(0,16);
        //        lineEdit_8_bit->setText(QString::number(int_val_8));
        m_tablewidget_conversion_obj->item(enum_8_bit,1)->setText(QString::number(int_val_8));
    }
}

void hex_viewer::show_mac_timestamp(QByteArray data_arr)
{
    if(data_arr.size() < 4)
    {
        m_tablewidget_conversion_obj->item(enum_mac_timestamp,1)->setText("");
        return;
    }

    if(data_arr.at(0) == '\0' && data_arr.at(1) == '\0' && data_arr.at(2) == '\0' && data_arr.at(3) == '\0')
    {
        m_tablewidget_conversion_obj->item(enum_mac_timestamp,1)->setText("");
        return;
    }

    uint32_t int_timestamp = data_arr.toHex().toULongLong(0,16);
    int_timestamp += 978307200;

    QString mac_time_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(QString::number(int_timestamp), Q_FUNC_INFO);
    //    lineEdit_mac_timestamp->setText(mac_time_str);
    m_tablewidget_conversion_obj->item(enum_mac_timestamp,1)->setText(mac_time_str);
}

void hex_viewer::show_unix_timestamp(QByteArray data_arr)
{
    if(data_arr.size() < 4)
    {
        m_tablewidget_conversion_obj->item(enum_unix_timestamp,1)->setText("");
        return;
    }

    if(data_arr.at(0) == '\0' && data_arr.at(1) == '\0' && data_arr.at(2) == '\0' && data_arr.at(3) == '\0')
    {
        m_tablewidget_conversion_obj->item(enum_unix_timestamp,1)->setText("");
        return;
    }

    uint32_t int_timestamp = data_arr.toHex().toULongLong(0,16);

    QString unix_time_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(QString::number(int_timestamp), Q_FUNC_INFO);
    //    lineEdit_unix_timestamp->setText(mac_time_str);
    m_tablewidget_conversion_obj->item(enum_unix_timestamp,1)->setText(unix_time_str);
}

void hex_viewer::show_binary_value(const QByteArray &data_arr)
{
    //    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QBitArray bit_arr;
    bit_arr.resize(8 * data_arr.count());
    for(int j=0;j<data_arr.count();++j)                     							// Converting to hex to binary
    {
        for(int b=0;b<8;++b)
        {
            bit_arr.setBit((j * 8) + b, data_arr.at(j) & (1<<(7 - b)));
        }
    }

    QString binary;
    bool value = false;
    for(int i = 0; i < bit_arr.size(); i++)
    {
        if(i % 4 == 0)
            binary.append(" ");

        if(bit_arr.at(i) == value)
            binary.append("0");
        else if(bit_arr.at(i) == !value)
            binary.append("1");
    }

    binary = binary.trimmed();

    //    lineEdit_binary->setText(binary);
    m_tablewidget_conversion_obj->item(enum_binary,1)->setText(binary);

    //    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

int hex_viewer::endian_checker()
{
    union
    {
        int i;
        char c[sizeof(int)];
    } x;

    x.i = 1;

    if(x.c[0] == 1)
        return MACRO_Hex_Viewer_Little_Endian_int;
    else
        return MACRO_Hex_Viewer_Big_Endian_int;

}

QByteArray hex_viewer::endian_converter(QByteArray data)
{
    //    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QByteArray converted_arr;

    int arr_size = data.size();

    for(int i = arr_size; i > 0; i--)
    {
        converted_arr.append(data.mid(i-1,1));
    }

    //    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

    return converted_arr;
}
