#ifndef USB_DEVICE_SERIAL_LIST_module_H
#define USB_DEVICE_SERIAL_LIST_module_H

#include<stdio.h>
//#include <usb.h>
#include <QtCore>


class usb_device_serial_list_module
{
public:
    usb_device_serial_list_module(QWidget *parent = 0);
    QStringList usb_serial_number_list();
    void update_usb_list();



private:
    QStringList usb_serial_stringlist;
};

#endif // USB_DEVICE_SERIAL_LIST_H
