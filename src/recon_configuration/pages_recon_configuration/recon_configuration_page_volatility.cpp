#include "recon_configuration_page_volatility.h"
#include "ui_recon_configuration_page_volatility.h"

recon_configuration_page_volatility::recon_configuration_page_volatility(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_volatility)
{
    ui->setupUi(this);

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

recon_configuration_page_volatility::~recon_configuration_page_volatility()
{
    delete ui;
}

void recon_configuration_page_volatility::on_pushButton_open_clicked()
{
    volatility_path.clear();// = QFileDialog::getOpenFileName(this, "Volatility Path", getenv("HOME"), QString("vol.py"));


    file_dialog_obj.setNameFilter(("vol.py"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            volatility_path = selected_files.at(0);
        }
    }


    QFileInfo info(volatility_path);

    if(!info.exists())
        return;

    QString volatility_dir = info.dir().absolutePath();

    if(volatility_dir.isEmpty())
        return;

    QString command = "delete from tbl_volatility";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command, destination_db_file, Q_FUNC_INFO);

    command = "insert into tbl_volatility (volatility_dir) values (?)";
    QStringList args_list;
    args_list << volatility_dir.trimmed();

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, args_list, destination_db_file, Q_FUNC_INFO);

    ui->lineEdit_path->setText(volatility_dir);


}

void recon_configuration_page_volatility::prepare_display()
{
    QString command = "select volatility_dir from tbl_volatility";
    QString volatility_dir = recon_helper_standard_obj->get_string_from_db_by_dbpath(command, 0, destination_db_file, Q_FUNC_INFO);

    ui->lineEdit_path->setText(volatility_dir);
}


void recon_configuration_page_volatility::pub_set_essentials()
{
    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    prepare_display();
}
