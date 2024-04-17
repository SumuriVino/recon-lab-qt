#include "save_result_dialog_box.h"
#include "ui_save_result_dialog_box.h"

save_result_dialog_box::save_result_dialog_box(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::save_result_dialog_box)
{
    ui->setupUi(this);

    ui->lineEdit_name->setAttribute(Qt::WA_MacShowFocusRect, false);

}

save_result_dialog_box::~save_result_dialog_box()
{
    delete ui;
}

void save_result_dialog_box::pub_set_name(QString set_name)
{
    set_save_text = set_name;
    ui->lineEdit_name->setPlaceholderText(set_save_text);
}


void save_result_dialog_box::on_pushButton_save_clicked()
{
    QString file_name = ui->lineEdit_name->text().trimmed() ;
    if(file_name.trimmed().isEmpty())
        file_name = ui->lineEdit_name->placeholderText().trimmed();


    emit signal_save_result_name_value(file_name);
}

void save_result_dialog_box::closeEvent(QCloseEvent *)
{
    emit signal_cancel_save_results();
}
