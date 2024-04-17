#include "recon_configuration_page_examiner_details.h"
#include "ui_recon_configuration_page_examiner_details.h"

recon_configuration_page_examiner_details::recon_configuration_page_examiner_details(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_examiner_details)
{
    ui->setupUi(this);

    recon_static_functions::app_debug(" examiner_details - 1 start", Q_FUNC_INFO);

    ui->lineEdit_agency_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner_email->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_examiner_phone->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_company_logo->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->textEdit_agency_address->setReadOnly(false);

    recon_static_functions::app_debug(" examiner_details - 2", Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard(this);

    recon_static_functions::app_debug(" examiner_details - 3", Q_FUNC_INFO);

    recon_helper_process_obj = new recon_helper_process(this);

    recon_static_functions::app_debug(" examiner_details - 4", Q_FUNC_INFO);

    QObject::connect(ui->textEdit_agency_address,SIGNAL(signal_textedit_focusout_event_occured()),this,SLOT(slot_textedit_agency_focusout_occured()));

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

    recon_static_functions::app_debug(" examiner_details - 5 end", Q_FUNC_INFO);

}

recon_configuration_page_examiner_details::~recon_configuration_page_examiner_details()
{
    delete ui;
}

void recon_configuration_page_examiner_details::on_pushButton_company_logo_clicked()
{
    recon_static_functions::app_debug(" :  - Starts",Q_FUNC_INFO);

  //  QString home_dir = getenv("HOME");
    QString fileName;// = QFileDialog::getOpenFileName(this,  tr("Select Logo"), home_dir, tr("Image Files (*.png *.jpg *.jpeg)"));

    file_dialog_obj.setNameFilter(("*.png *.jpg *.jpeg"));
     file_dialog_obj.setFileMode(QFileDialog::ExistingFile);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            fileName = selected_files.at(0);
        }
    }


    if(fileName == QString(""))
        return;

   // QFile file1("../icons/agency_logo.png");
    QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
    QFile file1(agency_logo_path);
    file1.remove();

    if(!fileName.endsWith(".png"))
    {
        QStringList args;
      //  args << "-s" << "format" << "png" << fileName << "--out" << "../icons/agency_logo.png";
        args << "-s" << "format" << "png" << fileName << "--out" << agency_logo_path;

        recon_helper_process_obj->run_command_with_arguments("sips", args, Q_FUNC_INFO);
    }
    else
    {
        QFile file(fileName);
        //file.copy("../icons/agency_logo.png");
        file.copy(agency_logo_path);

    }

    //QPixmap pixmap("../icons/agency_logo.png");
    QPixmap pixmap(agency_logo_path);
    int w = ui->label_company_logo->width();
    int h = ui->label_company_logo->height();
    ui->label_company_logo->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

    recon_static_functions::app_debug(" : - END",Q_FUNC_INFO);
}

void recon_configuration_page_examiner_details::pub_pushButton_update_examiner_info()
{
    recon_static_functions::app_debug(" : - START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command_delete = QString("delete from agency_details");
    if(!recon_helper_standard_obj->execute_db_command_by_dbreference(command_delete,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" delete ------FAILED------- ",Q_FUNC_INFO);
    }

    QString command_insert = "insert into 'agency_details' (agency_name, agency_address, examiner, examiner_phone, examiner_email, date_format) values (?,?,?,?,?,?)";
    QStringList list_values;
    list_values << ui->lineEdit_agency_name->text().trimmed() << ui->textEdit_agency_address->toPlainText().trimmed();
    list_values << ui->lineEdit_examiner->text().trimmed() << ui->lineEdit_examiner_phone->text().trimmed();
    list_values << ui->lineEdit_examiner_email->text().trimmed() << "1";
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,list_values,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" agency_details query ------FAILED------- ",Q_FUNC_INFO);
    }
    destination_db.close();
    recon_static_functions::app_debug(": END",Q_FUNC_INFO);
}

void recon_configuration_page_examiner_details::display_data_on_page()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";

   // QPixmap pixmap("../icons/agency_logo.png");
    QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
    QPixmap pixmap(agency_logo_path);
    int w = ui->label_company_logo->width();
    int h = ui->label_company_logo->height();
    ui->label_company_logo->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    if(!query.exec("select agency_name, agency_address, examiner, examiner_phone, examiner_email from agency_details"))
    {
        recon_static_functions::app_debug(" agency_details query ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    if(query.next())
    {
        ui->lineEdit_agency_name->setText(query.value(0).toString());
        ui->textEdit_agency_address->setPlainText(query.value(1).toString());
        ui->lineEdit_examiner->setText(query.value(2).toString());
        ui->lineEdit_examiner_phone->setText(query.value(3).toString());
        ui->lineEdit_examiner_email->setText(query.value(4).toString());
    }
    destination_db.close();
    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);
}


void recon_configuration_page_examiner_details::pub_initialise_examiner_details()
{
    display_data_on_page();

}

void recon_configuration_page_examiner_details::on_lineEdit_examiner_editingFinished()
{
    pub_pushButton_update_examiner_info();
}

void recon_configuration_page_examiner_details::on_lineEdit_examiner_phone_editingFinished()
{
    pub_pushButton_update_examiner_info();
}

void recon_configuration_page_examiner_details::on_lineEdit_examiner_email_editingFinished()
{
    pub_pushButton_update_examiner_info();
}

void recon_configuration_page_examiner_details::on_lineEdit_agency_name_editingFinished()
{
    pub_pushButton_update_examiner_info();
}

void recon_configuration_page_examiner_details::slot_textedit_agency_focusout_occured()
{

    pub_pushButton_update_examiner_info();
}
