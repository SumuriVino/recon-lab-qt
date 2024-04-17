#include "report_launcher_page_report_case_info.h"
#include "ui_report_launcher_page_report_case_info.h"

page_report_case_info::page_report_case_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::page_report_case_info)
{
    ui->setupUi(this);
    ui->lineEdit_agency->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_case_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_case_no->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_location->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_company_logo->setAttribute(Qt::WA_MacShowFocusRect,false);

    recon_helper_standard_obj = new recon_helper_standard(this);

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));

}

page_report_case_info::~page_report_case_info()
{
    delete ui;
}

void page_report_case_info::set_narad_muni_for_report(narad_muni *obj)
{
    narad_muni_for_report_obj = obj;

}

void page_report_case_info::pub_initialise_case_info()
{
    fill_case_info();
}


void page_report_case_info::pub_set_case_info_narad_muni()
{
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Case_Agency_Name_QString, ui->lineEdit_agency->text());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Case_Name_QString, ui->lineEdit_case_name->text());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Case_Notes_QString, ui->textEdit_case_notes->toPlainText());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Case_ID_QString, ui->lineEdit_case_no->text());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Examiner_Name_QString, ui->lineEdit_examiner->text());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Case_Location_QString, ui->lineEdit_location->text());

}

void page_report_case_info::fill_case_info()
{
    QPixmap pixmap(narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString()); // load pixmap

    int w = ui->label_company_logo->width();
    int h = ui->label_company_logo->height();
    ui->label_company_logo->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

    ui->lineEdit_agency->setText(narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString());
    ui->lineEdit_case_name->setText(narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    ui->lineEdit_case_no->setText(narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_ID_QString).toString());
    ui->lineEdit_examiner->setText(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString());
    ui->lineEdit_location->setText(narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Location_QString).toString());
    ui->textEdit_case_notes->setText(narad_muni_for_report_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString());
}

void page_report_case_info::on_pushButton_company_logo_clicked()
{

    file_dialog_obj.setNameFilter(("*.png"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);

    QString filepath;
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            filepath = selected_files.at(0);

            if(filepath == QString(""))
                return;

            QPixmap pixmap(filepath); // load pixmap
            int w = ui->label_company_logo->width();
            int h = ui->label_company_logo->height();
            ui->label_company_logo->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

            narad_muni_for_report_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,filepath);

        }
    }


}


