#include "export_dialog_box_1.h"
#include "ui_export_dialog_box_1.h"

export_dialog_box_1::export_dialog_box_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_dialog_box_1)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);

    ui->lineEdit_export_path->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_saved_name->setAttribute(Qt::WA_MacShowFocusRect, false);

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

export_dialog_box_1::~export_dialog_box_1()
{
    delete ui;
}

void export_dialog_box_1::pub_set_export_type(QString exp_typ_str)
{ // set export type
    export_type_str = exp_typ_str;
}

void export_dialog_box_1::pub_set_name_and_dir_path(QString given_name, QString output_dir_path)
{ // set saved name and export path directory
    ui->lineEdit_saved_name->setPlaceholderText(given_name);
    ui->lineEdit_export_path->setPlaceholderText(output_dir_path);
}

void export_dialog_box_1::closeEvent(QCloseEvent *)
{ // close to not to export
    emit signal_cancel_export_results();
}


void export_dialog_box_1::on_pushButton_OK_clicked()
{ // on button ok clicked
    QString filename = ui->lineEdit_saved_name->text().trimmed();
    QString filepath = ui->lineEdit_export_path->text().trimmed();

    if(filename.isEmpty())
        filename = ui->lineEdit_saved_name->placeholderText();

    if(filepath.isEmpty())
        filepath = ui->lineEdit_export_path->placeholderText();

    emit signal_save_results_by_export_window(filename,filepath, export_type_str);
}

void export_dialog_box_1::on_pushButton_select_path_clicked()
{ // select destination path
    QString path;// = QFileDialog::getExistingDirectory(this,recon_static_functions::get_application_name(),getenv("HOME"));

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

    ui->lineEdit_export_path->setText(path);
}
