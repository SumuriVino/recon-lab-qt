#ifndef RECON_CONFIGURATION_PAGE_EXTERNAL_APPLICATIONS_H
#define RECON_CONFIGURATION_PAGE_EXTERNAL_APPLICATIONS_H

#include <QWidget>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include <QFileDialog>
#include <QFileIconProvider>
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_configuration_page_external_applications;
}

class recon_configuration_page_external_applications : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_external_applications(QWidget *parent = 0);
    ~recon_configuration_page_external_applications();

    void pub_set_caller_type(QString caller_type);

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_remove_clicked();

private:
    Ui::recon_configuration_page_external_applications *ui;

    enum enum_display_column_name
    {
        enum_display_column_application_name = 0,
        enum_display_column_application_path


    };


    QString destination_db_file;

    void display_data(void);

    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // RECON_CONFIGURATION_PAGE_EXTERNAL_APPLICATIONS_H
