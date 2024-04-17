#include "recon_configuration_page_system_password.h"
#include "ui_recon_configuration_page_system_password.h"

recon_configuration_page_system_password::recon_configuration_page_system_password(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_system_password)
{
    ui->setupUi(this);


    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

    system_password_asker_obj = new system_password_asker(this);
    connect(system_password_asker_obj, SIGNAL(signal_system_password_asker_workdone(bool,QString)), this, SLOT(slot_edit_password_done(bool,QString)));

    QIcon icon_edit(QPixmap("../icons/signs/edit.png"));

    ui->lineEdit_password->setReadOnly(true);

    ui->pushButton_edit_pwd->setIcon(icon_edit);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

recon_configuration_page_system_password::~recon_configuration_page_system_password()
{
    delete ui;
}

void recon_configuration_page_system_password::on_pushButton_edit_pwd_clicked()
{

    system_password_asker_obj->pub_set_essentials();
    system_password_asker_obj->pub_set_window_modality();
    system_password_asker_obj->pub_set_label_msg("Enter admin password.");
    system_password_asker_obj->show();

}

void recon_configuration_page_system_password::pub_set_caller_type(QString caller_type)
{

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    display_data();

}

void recon_configuration_page_system_password::display_data()
{
    // showing **********
    ui->lineEdit_password->setText("12345678");
}


void recon_configuration_page_system_password::slot_edit_password_done(bool pwd_status, QString received_pwd)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    system_password_asker_obj->hide();

    if(pwd_status)
    {
        QString file_path_org = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString).toString() + "confidential_original.txt";
        recon_static_functions::overwrite_data_into_file(file_path_org, received_pwd, Q_FUNC_INFO);
        recon_static_functions::encrypt_file_1(file_path_org, Q_FUNC_INFO);

        global_desktop_info_class_obj->pub_update_desktop_password(true, Q_FUNC_INFO);

    }
    else
    {
        global_desktop_info_class_obj->pub_update_desktop_password(false, Q_FUNC_INFO);
    }
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}
