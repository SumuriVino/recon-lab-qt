#ifndef PAGE_EXAMINER_DETAILS_H
#define PAGE_EXAMINER_DETAILS_H

#include <QWidget>
#include <QFileInfo>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_helper_process.h"    

#include "header_identical_1.h"
                                   


namespace Ui {
class recon_configuration_page_examiner_details;
}

class recon_configuration_page_examiner_details : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_examiner_details(QWidget *parent = 0);
    ~recon_configuration_page_examiner_details();

    void pub_initialise_examiner_details();
    void pub_pushButton_update_examiner_info();


private slots:
    void on_pushButton_company_logo_clicked();

    void on_lineEdit_examiner_editingFinished();

    void on_lineEdit_examiner_phone_editingFinished();

    void on_lineEdit_examiner_email_editingFinished();

    void on_lineEdit_agency_name_editingFinished();

    void slot_textedit_agency_focusout_occured();

private:
    Ui::recon_configuration_page_examiner_details *ui;

    void display_data_on_page();
    QString destination_db_file;

    recon_helper_standard *recon_helper_standard_obj;

    QFileDialog file_dialog_obj;
    recon_helper_process *recon_helper_process_obj;

};

#endif // PAGE_EXAMINER_DETAILS_H
