#ifndef REPORT_STATUS_H
#define REPORT_STATUS_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"
namespace Ui {
class report_status;
}

class report_status : public QDialog
{
    Q_OBJECT

public:
    explicit report_status(QWidget *parent = 0);
    ~report_status();

    void set_label(QString text);

private:
    Ui::report_status *ui;
};

#endif // REPORT_STATUS_H
