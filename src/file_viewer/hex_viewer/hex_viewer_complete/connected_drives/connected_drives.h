#ifndef CONNECTED_DRIVES_H
#define CONNECTED_DRIVES_H
#include <QtCore>
#define MAX_LENGTH 260

//#include <windows.h>

//#include <stdio.h>
//#include <time.h>		// time()
//#include <process.h>	// _getpid()
//#include <wincrypt.h>


struct physical_device {
  QString physical_node;
  qint64 size;
};


class connected_drives
{
public:
    connected_drives();

    void pub_extract_device_list();
    QList<physical_device> pub_get_device_list();

private:
    physical_device list_physical_device(QString sstr);
    physical_device list_logical_device(QString sstr);
    QList<physical_device> physical_device_list;
};

#endif // CONNECTED_DRIVES_H
