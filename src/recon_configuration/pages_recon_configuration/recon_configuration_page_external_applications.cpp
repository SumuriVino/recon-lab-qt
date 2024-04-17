#include "recon_configuration_page_external_applications.h"
#include "ui_recon_configuration_page_external_applications.h"

recon_configuration_page_external_applications::recon_configuration_page_external_applications(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_external_applications)
{
    ui->setupUi(this);

    recon_static_functions::app_debug("config-external_applications---11", Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_static_functions::app_debug("config-external_applications---22", Q_FUNC_INFO);

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    recon_static_functions::app_debug("config-external_applications---33", Q_FUNC_INFO);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory("/Applications/");
    recon_static_functions::app_debug("config-external_applications---44", Q_FUNC_INFO);



    ui->tableWidget_external_applications->setColumnWidth(enum_display_column_application_name, 220);
    ui->tableWidget_external_applications->setColumnWidth(enum_display_column_application_path, 600);
}

recon_configuration_page_external_applications::~recon_configuration_page_external_applications()
{
    delete ui;
}

void recon_configuration_page_external_applications::pub_set_caller_type(QString caller_type)
{

    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");


    //    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    //        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Specific_Path_QString).toString() + QString("recon_configuration/recon_configuration.sqlite");
    //    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    //        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Specific_Path_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    //    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
    //        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    display_data();
}

void recon_configuration_page_external_applications::display_data()
{

    recon_static_functions::app_debug(" : - START",Q_FUNC_INFO);

    ui->tableWidget_external_applications->setRowCount(0);

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
    QString command = "select application_name, application_path from tbl_external_applications";

    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" Query ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    int row = 0;
    while(query.next())
    {
        QFileInfo info(query.value(1).toString());
        QFileIconProvider icon_provide;
        QIcon icon_path = icon_provide.icon(info);

        QTableWidgetItem *item_app_name, *item_app_path;

        item_app_name = new QTableWidgetItem(query.value(0).toString());
        item_app_name->setIcon(icon_path);
        item_app_path = new QTableWidgetItem(query.value(1).toString());

        ui->tableWidget_external_applications->insertRow(row);
        ui->tableWidget_external_applications->setRowHeight(row, 23);

        ui->tableWidget_external_applications->setItem(row, 0, item_app_name);
        ui->tableWidget_external_applications->setItem(row, 1, item_app_path);

        row++;
    }

//    ui->tableWidget_external_applications->resizeColumnToContents(0);
//    ui->tableWidget_external_applications->resizeColumnToContents(1);

    ui->tableWidget_external_applications->setColumnWidth(enum_display_column_application_name, 220);
    ui->tableWidget_external_applications->setColumnWidth(enum_display_column_application_path, 600);

    recon_static_functions::app_debug(" : - END",Q_FUNC_INFO);
}

void recon_configuration_page_external_applications::on_pushButton_add_clicked()
{
    QString app_path;// = QFileDialog::getOpenFileName(this, "Applications", "/Applications");

    file_dialog_obj.setNameFilter(("*.app"));
    file_dialog_obj.setFileMode(QFileDialog::Directory);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            app_path = selected_files.at(0);
        }
    }
    if(app_path.trimmed() == QString(""))
        return;

    QFileInfo info(app_path);

    QString app_name = info.fileName();

    QString command = "insert into tbl_external_applications (application_name, application_path) values (?,?)";
    QStringList args_list;
    args_list << app_name << app_path;


    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, args_list, destination_db_file, Q_FUNC_INFO);

    display_data();

}

void recon_configuration_page_external_applications::on_pushButton_remove_clicked()
{
    int current_row = ui->tableWidget_external_applications->currentRow();

    if(current_row < 0)
        return;

    QString app_name =  ui->tableWidget_external_applications->item(current_row, 0)->text().trimmed();
    QString app_path =  ui->tableWidget_external_applications->item(current_row, 1)->text().trimmed();

    QString command = "delete from tbl_external_applications where application_name = ? AND application_path = ?";
    QStringList args_list;
    args_list << app_name << app_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, args_list, destination_db_file, Q_FUNC_INFO);

    display_data();

}
