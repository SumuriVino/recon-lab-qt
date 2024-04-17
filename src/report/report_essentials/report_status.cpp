#include "report_status.h"
#include "ui_report_status.h"

report_status::report_status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::report_status)
{
    ui->setupUi(this);

    setWindowTitle(recon_static_functions::get_app_name());

    ui->label->setText("Report Generated. Do you want to open this?");
}

report_status::~report_status()
{
    delete ui;
}

void report_status::set_label(QString text)
{
    ui->label->setText(text);
}
