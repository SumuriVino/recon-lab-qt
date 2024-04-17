#ifndef USB_DEVICE_SERIAL_LIST_H
#define USB_DEVICE_SERIAL_LIST_H
#include<stdio.h>
//#include <usb.h>
#include <QtCore>
#include "header_plugin_extraction_interface.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_helper_process.h"


class usb_device_serial_list : public QObject
{
public:
    usb_device_serial_list(QWidget *parent = 0);
    QStringList usb_serial_number_list();
    void update_usb_list();


private:
    QStringList usb_serial_stringlist;
    QStringList usb_parser_API();
    QStringList usb_parser_CLI();

    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;

};

#endif // USB_DEVICE_SERIAL_LIST_H
