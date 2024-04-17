#ifndef RECON_CONFIGURATION_PAGE_SYSTEM_PASSWORD_H
#define RECON_CONFIGURATION_PAGE_SYSTEM_PASSWORD_H

#include <QWidget>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_helper_process.h"    
#include "system_password_asker/system_password_asker.h"
#include "header_global_variables_4.h"

namespace Ui {
class recon_configuration_page_system_password;
}

class recon_configuration_page_system_password : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_system_password(QWidget *parent = 0);
    ~recon_configuration_page_system_password();

    void pub_set_caller_type(QString caller_type);

private slots:
    void on_pushButton_edit_pwd_clicked();

    void slot_edit_password_done(bool pwd_status, QString received_pwd);
private:
    Ui::recon_configuration_page_system_password *ui;

    void display_data();

    QString destination_db_file;

    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;

    system_password_asker *system_password_asker_obj;

};

#endif // RECON_CONFIGURATION_PAGE_SYSTEM_PASSWORD_H
