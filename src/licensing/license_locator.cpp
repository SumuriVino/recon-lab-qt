#include "license_locator.h"
#include "ui_license_locator.h"

license_locator::license_locator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::license_locator)
{
    ui->setupUi(this);

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
   // file_dialog_obj.setFilter(QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System);
   // file_dialog_obj.setViewMode(QFileDialog::Detail);

    recon_helper_standard_obj =  new recon_helper_standard(this);

}

license_locator::~license_locator()
{
    delete ui;
}

void license_locator::closeEvent(QCloseEvent *)
{
    emit signal_waapis_jao();
}

void license_locator::set_executer_type(QString m_type)
{

    //executer_type = m_type;

    //set_gui_messages();
}

void license_locator::set_gui_messages(QString msg)
{

    ui->label_message->setText(msg);
}

void license_locator::pub_hide_browse_button()
{
    ui->pushButton_browse->hide();
}

void license_locator::pub_set_cancel_button_text(QString str)
{
    ui->pushButton_cancel->setText(str);
}

void license_locator::on_pushButton_cancel_clicked()
{
    emit signal_waapis_jao();
    close();
}

void license_locator::on_pushButton_browse_clicked()
{

    QString license_file;

    file_dialog_obj.setNameFilter(("license"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);


    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            license_file = selected_files.at(0);
        }
    }


    if(license_file.trimmed() == QString(""))
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select license file.");
        return;
    }


    QString dir = QDir::currentPath();

    QString license_path = recon_helper_standard_obj->pub_get_app_license_path();
    recon_static_functions::remove_safely_blank_dir_OR_file(license_path, Q_FUNC_INFO);

    QFile file(license_file);
    file.copy(license_path);

    emit signal_waapis_jao();

    global_variable_license_validation_value_int = 33;
    close();

}

