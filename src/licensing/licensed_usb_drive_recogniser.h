#ifndef LICENSED_USB_DRIVE_RECOGNISER_H
#define LICENSED_USB_DRIVE_RECOGNISER_H

#include <QObject>
#include <QWidget>
#include "recon_generic_functions/recon_static_functions.h"
#include "usb_device_serial_list.h"
#include <QMessageBox>

class licensed_usb_drive_recogniser : public QObject
{
    Q_OBJECT
public:
    explicit licensed_usb_drive_recogniser(QObject *parent = nullptr);
    ~licensed_usb_drive_recogniser();

    void pub_set_licensed_drive_sr_no(QString sr_no);


signals:


private slots:
    void slot_timer();

private:

    QString licensed_drive_sr_no;
    QTimer *m_timer;

};

#endif // LICENSED_USB_DRIVE_RECOGNISER_H
