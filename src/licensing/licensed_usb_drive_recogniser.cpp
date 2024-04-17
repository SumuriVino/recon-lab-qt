#include "licensed_usb_drive_recogniser.h"

licensed_usb_drive_recogniser::licensed_usb_drive_recogniser(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer;
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));
    m_timer->setInterval(300);
    m_timer->start();

}

licensed_usb_drive_recogniser::~licensed_usb_drive_recogniser()
{

}

void licensed_usb_drive_recogniser::pub_set_licensed_drive_sr_no(QString sr_no)
{

    licensed_drive_sr_no = sr_no;
}

void licensed_usb_drive_recogniser::slot_timer()
{
    m_timer->stop();

    return;


    usb_device_serial_list l1;
    l1.update_usb_list();
    QStringList attached_usb_list = l1.usb_serial_number_list();
    if(!attached_usb_list.contains(licensed_drive_sr_no))
    {
        m_timer->stop();

    }

}
