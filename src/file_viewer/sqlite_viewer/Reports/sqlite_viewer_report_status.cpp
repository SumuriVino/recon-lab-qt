#include "sqlite_viewer_report_status.h"
#include "ui_sqlite_viewer_report_status.h"

sqlite_viewer_report_status::sqlite_viewer_report_status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sqlite_viewer_report_status)
{
    ui->setupUi(this);

    setWindowTitle(recon_static_functions::get_sqlite_viewer_name());

    ui->label->setText("Report Generated. Do you want to open this?");
}

sqlite_viewer_report_status::~sqlite_viewer_report_status()
{
    delete ui;
}

void sqlite_viewer_report_status::set_label(QString text)
{
    ui->label->setText(text);
}
