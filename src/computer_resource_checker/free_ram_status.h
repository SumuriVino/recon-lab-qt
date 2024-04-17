#ifndef FREE_RAM_STATUS_H
#define FREE_RAM_STATUS_H

#include <QObject>
#include "recon_generic_functions/recon_helper_standard.h"

class free_ram_status : public QObject
{
    Q_OBJECT
public:
    explicit free_ram_status(QObject *parent = nullptr);

    qint64 pub_get_free_ram_status();

signals:

public slots:

private:

};

#endif // FREE_RAM_STATUS_H
