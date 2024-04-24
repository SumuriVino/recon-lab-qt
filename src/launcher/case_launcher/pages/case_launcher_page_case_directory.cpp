#include "case_launcher_page_case_directory.h"
#include "ui_case_launcher_page_case_directory.h"


page_case_directory::page_case_directory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_case_directory)
{
    ui->setupUi(this);

    ui->lineEdit_case_dir_path->setEnabled(false);

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

page_case_directory::~page_case_directory()
{
    delete ui;
}


void page_case_directory::pub_set_output_dir_path() // Here we set the result directory path in global class so we can access it anywhere in the code globally.
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Output_Directory_QString,ui->lineEdit_case_dir_path->text().trimmed());
}

void page_case_directory::pub_set_stored_output_path() //If we have selected the result directory path earlier, so next time it directly show their from database
{
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";

    QString command = QString("select result_path from case_result_info");
    output_dir_path =  recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,db_path,Q_FUNC_INFO);

    QFileInfo info(output_dir_path);

    if(!output_dir_path.isEmpty() && info.exists())
    {
        ui->lineEdit_case_dir_path->setText(output_dir_path);
        emit signal_result_dir_path_selected(output_dir_path);
    }
    else
    {
        emit signal_result_dir_path_selected("");
    }

}

bool page_case_directory::pub_check_output_path_exist() //This checks in the backend that the result directory path exists or not
{
    QString path = ui->lineEdit_case_dir_path->text().trimmed();

    QFileInfo info(path);

    if(info.exists())
        return true;
    else
        return false;

    return false;
}

void page_case_directory::on_pushButton_case_dir_path_clicked() // When we click on browse button to select the result directory path, this function hits
{                                                               // and send that path through signal to save in database later
    // QString home_dir = getenv("HOME");
    QString path;// = QFileDialog::getExistingDirectory(this, "Case", home_dir, QFileDialog::ShowDirsOnly);

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            path = selected_files.at(0);
        }
    }

    if(path.trimmed() == "")
        return;

    if(path.trimmed() != "")
        output_dir_path = path;


    QString fs_type = global_recon_helper_singular_class_obj->get_file_system_of_filepath(path, Q_FUNC_INFO);
    if(fs_type != MACRO_FILE_SYSTEM_TYPE_HFS && fs_type != MACRO_FILE_SYSTEM_TYPE_APFS)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(),"Selected output drive is formatted with '" + fs_type + "'. We strongly recommend to use either 'HFS+' or 'APFS' formatted output drives.");
    }

    ui->lineEdit_case_dir_path->setText(output_dir_path);

    emit signal_result_dir_path_selected(output_dir_path);
}
