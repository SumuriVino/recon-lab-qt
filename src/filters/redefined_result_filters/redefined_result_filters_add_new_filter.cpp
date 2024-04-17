#include "redefined_result_filters_add_new_filter.h"
#include "ui_redefined_result_filters_add_new_filter.h"

redefined_result_filters_add_new_filter::redefined_result_filters_add_new_filter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::redefined_result_filters_add_new_filter)
{
    ui->setupUi(this);
    setWindowTitle("Filter Name");
    ui->lineEdit_filter_name->setClearButtonEnabled(true);
    ui->lineEdit_filter_name->setAttribute(Qt::WA_MacShowFocusRect,false);
}

redefined_result_filters_add_new_filter::~redefined_result_filters_add_new_filter()
{
    delete ui;
}

void redefined_result_filters_add_new_filter::refresh_filters_list_on_add_new_filter(QStringList filters_list)
{
    ui->lineEdit_filter_name->clear();
    ui->lineEdit_filter_name->setStyleSheet("");

    redefined_result_filters_list.clear();
    redefined_result_filters_list = filters_list;
}

void redefined_result_filters_add_new_filter::on_pushButton_save_clicked()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QString filter_by_user = ui->lineEdit_filter_name->text();

    if(filter_by_user.isEmpty())
    {
        ui->lineEdit_filter_name->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        ui->label_message->setText("enter filter.");
        return;
    }

    if(redefined_result_filters_list.contains(filter_by_user)){
        ui->lineEdit_filter_name->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        ui->label_message->setText("filter already exist.");
        return;
    }

    emit signal_add_new_filter_in_tablewidget(ui->lineEdit_filter_name->text());
    close();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);

}

void redefined_result_filters_add_new_filter::on_lineEdit_filter_name_textChanged(const QString &arg1)
{
    if(!arg1.trimmed().isEmpty())
    {
        ui->lineEdit_filter_name->setStyleSheet("");
        ui->label_message->clear();
    }
}
