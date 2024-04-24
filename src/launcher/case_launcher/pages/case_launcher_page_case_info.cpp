#include "case_launcher_page_case_info.h"
#include "ui_case_launcher_page_case_info.h"

page_case_info::page_case_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_case_info)
{
    ui->setupUi(this);

    ui->lineEdit_agency->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_case_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_case_no->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_location->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->textEdit_case_notes->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner_email->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner_phone->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->textEdit_agency_address->setAttribute(Qt::WA_MacShowFocusRect,false);


    // ^ Assert position at the start of the line
    // * Match any character any number of times
    // $ end
    QRegularExpression rx("^[a-zA-Z0-9_ -]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->lineEdit_case_name->setValidator(validator);

    QRegularExpression rx1("^[a-zA-Z0-9_-]*$");
    QValidator *validator1 = new QRegularExpressionValidator(rx1, this);
    ui->lineEdit_case_no->setValidator(validator1);
    recon_helper_standard_obj = new recon_helper_standard(this);


    ui->label_case_no->setText(global_lang_tr_obj.get_translated_string("Case No.") + "*");
    ui->label_case_name->setText(global_lang_tr_obj.get_translated_string("Case Name") + "*");
    ui->label_location->setText(global_lang_tr_obj.get_translated_string("Location"));
    ui->label_case_note->setText(global_lang_tr_obj.get_translated_string("Case Notes"));
    ui->label_examiner->setText(global_lang_tr_obj.get_translated_string("Examiner") + "*");
    ui->label_examiner_phone->setText(global_lang_tr_obj.get_translated_string("Examiner Phone"));
    ui->label_examiner_email->setText(global_lang_tr_obj.get_translated_string("Examiner Email"));
    ui->label_agency->setText(global_lang_tr_obj.get_translated_string("Agency"));
    ui->label_agency_address->setText(global_lang_tr_obj.get_translated_string("Agency Address"));

}

page_case_info::~page_case_info()
{
    delete ui;
}

void page_case_info::pub_set_case_info() // Here we set case id, case name,examiner name etc details in global class so we can save it in database and use it in anywhere in code
{                                       // Functions hits initially when recon lab asks from user to fill all these details
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_ID_QString,ui->lineEdit_case_no->text().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Name_QString,ui->lineEdit_case_name->text().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Location_QString,ui->lineEdit_location->text().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Notes_QString,ui->textEdit_case_notes->toPlainText().trimmed());

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Name_QString,ui->lineEdit_examiner->text().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Phone_QString,ui->lineEdit_examiner_phone->text().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Email_QString,ui->lineEdit_examiner_email->text().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Address_QString,ui->textEdit_agency_address->toPlainText().trimmed());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Name_QString,ui->lineEdit_agency->text().trimmed());


    //-Here update the CAse info.
    update_case_info_in_db();
}

void page_case_info::pub_set_initialise_case_info() // Some information of examiner we only takes one time like agaency name, examiner email etc, next time we use to get it from database
{
    display_case_details_data();
}


void page_case_info::on_lineEdit_case_no_textChanged(const QString &arg1) // If user modify case no then these info again update in database accordingly
{
    if(ui->lineEdit_case_name->text().trimmed() == "" || ui->lineEdit_case_no->text().trimmed() == "" || ui->lineEdit_examiner->text().trimmed() == "")
        emit signal_case_info_essentials_filled("");
    else
        emit signal_case_info_essentials_filled(ui->lineEdit_case_name->text().trimmed());
}

void page_case_info::on_lineEdit_case_name_textChanged(const QString &arg1) // If user modify case Name then these info again update in database accordingly
{
    if(ui->lineEdit_case_name->text().trimmed() == "" || ui->lineEdit_case_no->text().trimmed() == "" || ui->lineEdit_examiner->text().trimmed() == "")
        emit signal_case_info_essentials_filled("");
    else
        emit signal_case_info_essentials_filled(ui->lineEdit_case_name->text().trimmed());
}

void page_case_info::display_case_details_data() //If user has already filled info via configuration, then it will show their directly from database
{
    recon_static_functions::app_debug(" START" ,Q_FUNC_INFO);

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }


    QSqlQuery query_plugin(destination_db);
    QString command_plugin = QString("select agency_name,examiner,examiner_phone,examiner_email,agency_address from agency_details");

    query_plugin.prepare(command_plugin);
    if(!query_plugin.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_plugin,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_plugin.lastError().text(),Q_FUNC_INFO);
        return;
    }

    if(query_plugin.next())
    {
        ui->lineEdit_agency->setText(query_plugin.value(0).toString());
        ui->lineEdit_examiner->setText(query_plugin.value(1).toString());
        ui->lineEdit_examiner_phone->setText(query_plugin.value(2).toString());
        ui->lineEdit_examiner_email->setText(query_plugin.value(3).toString());
        ui->textEdit_agency_address->setText(query_plugin.value(4).toString());
    }

    destination_db.close();
    recon_static_functions::app_debug(" - END" ,Q_FUNC_INFO);
}

void page_case_info::update_case_info_in_db() // This update all the examiner details in database
{
    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";

    QString command_plugin = QString("update agency_details set agency_name = ?,examiner =?,examiner_phone = ?,examiner_email = ?,agency_address = ?");
    QStringList list_values;
    list_values << ui->lineEdit_agency->text().trimmed();
    list_values << ui->lineEdit_examiner->text().trimmed();
    list_values << ui->lineEdit_examiner_phone->text().trimmed();
    list_values << ui->lineEdit_examiner_email->text().trimmed();
    list_values << ui->textEdit_agency_address->toPlainText().trimmed();

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_plugin,list_values,destination_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" Updation  --------FAILED------  ",Q_FUNC_INFO);
    }
}

void page_case_info::on_lineEdit_examiner_textChanged(const QString &arg1) // If user modify case examiner name then these info again update in database accordingly
{
    if(ui->lineEdit_case_name->text().trimmed() == "" || ui->lineEdit_case_no->text().trimmed() == "" || ui->lineEdit_examiner->text().trimmed() == "")
        emit signal_case_info_essentials_filled("");
    else
        emit signal_case_info_essentials_filled(ui->lineEdit_case_name->text().trimmed());

}
