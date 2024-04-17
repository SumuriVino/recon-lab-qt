#ifndef SQLITE_VIEWER_REPORT_STATUS_H
#define SQLITE_VIEWER_REPORT_STATUS_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"

namespace Ui {
class sqlite_viewer_report_status;
}

class sqlite_viewer_report_status : public QDialog
{
    Q_OBJECT

public:
    explicit sqlite_viewer_report_status(QWidget *parent = 0);
    ~sqlite_viewer_report_status();

    void set_label(QString text);

private:
    Ui::sqlite_viewer_report_status *ui;
};

#endif // SQLITE_VIEWER_REPORT_STATUS_H
