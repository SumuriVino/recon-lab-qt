#include "usb_device_serial_list.h"


usb_device_serial_list::usb_device_serial_list(QWidget *parent)
{

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

}

QStringList usb_device_serial_list::usb_serial_number_list()
{
    return usb_serial_stringlist;
}

void usb_device_serial_list::update_usb_list()
{

    usb_serial_stringlist = usb_parser_API();

    usb_serial_stringlist << usb_parser_CLI();

    usb_serial_stringlist.removeDuplicates();
    usb_serial_stringlist.sort();

}


QStringList usb_device_serial_list::usb_parser_CLI()
{
    QStringList args ;
    args << "SPUSBDataType";

    QString sysusb_out = recon_helper_process_obj->run_command_with_arguments("system_profiler", args,Q_FUNC_INFO);

    QStringList m_list = sysusb_out.split("\n",Qt::SkipEmptyParts);

    QStringList fnl_list;
    for(int ii = 0; ii < m_list.size(); ii++)
    {
        QString str = m_list.at(ii);

        if(str.contains("Serial Number:"))
        {
            str = str.trimmed();
            if(str.startsWith("Serial Number:"))
            {
                str.remove(0,QString("Serial Number:").size());
                fnl_list << str.trimmed();
            }
        }
    }


    return fnl_list;

}
