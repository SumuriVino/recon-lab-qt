#include "usb_device_serial_list_module.h"

QStringList usb_parser_module();

usb_device_serial_list_module::usb_device_serial_list_module(QWidget *parent)
{
}

QStringList usb_device_serial_list_module::usb_serial_number_list()
{
    return usb_serial_stringlist;
}

void usb_device_serial_list_module::update_usb_list()
{

    usb_serial_stringlist = usb_parser_module();

}
