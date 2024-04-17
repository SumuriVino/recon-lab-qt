#include "export_as_kml.h"
#include "ui_export_as_kml.h"
#include "recon_generic_functions/recon_static_functions.h"


export_as_KML::export_as_KML(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_as_KML)
{
    ui->setupUi(this);


    setWindowFlags(Qt::Window| Qt::FramelessWindowHint);

    setWindowTitle(recon_static_functions::get_app_name());
    setWindowModality(Qt::ApplicationModal);


}

export_as_KML::~export_as_KML()
{
    delete ui;

}

void export_as_KML::pub_set_lable(QStringList filepath_list)
{

    ui->textEdit_files->setReadOnly(true);

    if(filepath_list.size() == 1)
        ui->label_kml->setText("This File can not be exported AS KML");
    else
        ui->label_kml->setText("These Files can not be exported AS KML");


    QString lable_str;
    for(int ii = 0 ; ii < filepath_list.size() ; ii++)
    {
        lable_str.append(filepath_list.at(ii)).append("\n");
    }

    ui->textEdit_files->setText(lable_str);

}

void export_as_KML::on_pushButton_continue_clicked()
{
    emit signal_continue_export_as_kml();
}

void export_as_KML::on_pushButton_cancel_clicked()
{

    emit signal_cancel_export_as_kml();
}
