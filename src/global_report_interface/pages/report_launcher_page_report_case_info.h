#ifndef PAGE_REPORT_CASE_INFO_H
#define PAGE_REPORT_CASE_INFO_H

#include <QDialog>
#include <QFileDialog>

                                   
#include "recon_generic_functions/recon_helper_standard.h"
namespace Ui {
class page_report_case_info;
}

class page_report_case_info : public QDialog
{
    Q_OBJECT

public:
    explicit page_report_case_info(QWidget *parent = 0);
    ~page_report_case_info();
    void set_narad_muni_for_report(narad_muni*);
    void pub_initialise_case_info();

    void pub_set_case_info_narad_muni();
private slots:
    void on_pushButton_company_logo_clicked();
private:
    Ui::page_report_case_info *ui;

    narad_muni *narad_muni_for_report_obj;

    void fill_case_info();

    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;
};

#endif // PAGE_REPORT_CASE_INFO_H
