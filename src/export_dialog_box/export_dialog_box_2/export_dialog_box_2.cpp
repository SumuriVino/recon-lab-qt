#include "export_dialog_box_2.h"
#include "ui_export_dialog_box_2.h"

export_dialog_box_2::export_dialog_box_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_dialog_box_2)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);

    ui->lineEdit_export_path->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_saved_name->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->radioButton_screen_items->setChecked(true);
    if(ui->radioButton_screen_items->isChecked())
    {
        report_scope_str = QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString);
    }

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

export_dialog_box_2::~export_dialog_box_2()
{
    delete ui;
}

void export_dialog_box_2::pub_set_export_type(QString exp_typ_str)
{ // set export type
    export_type_str = exp_typ_str;
}

void export_dialog_box_2::pub_set_exported_file_name_and_dir_path(QString file_name , QString outpur_dir_path)
{ // set exported saved file name and export path
    ui->lineEdit_saved_name->setPlaceholderText(file_name);
    ui->lineEdit_export_path->setPlaceholderText(outpur_dir_path);
}

void export_dialog_box_2::closeEvent(QCloseEvent *)
{ // close to not to export
    emit signal_cancel_export_results();
    ui->radioButton_screen_items->setChecked(true);
}


void export_dialog_box_2::on_pushButton_export_clicked()
{ // export button clicked to export
    QString filename = ui->lineEdit_saved_name->text().trimmed();
    QString output_dir_path = ui->lineEdit_export_path->text().trimmed();

    if(filename.isEmpty())
        filename = ui->lineEdit_saved_name->placeholderText();

    if(output_dir_path.isEmpty())
        output_dir_path = ui->lineEdit_export_path->placeholderText();

    emit signal_save_results_by_export_window(filename,output_dir_path, export_type_str,report_scope_str);

    ui->radioButton_screen_items->setChecked(true);
}

void export_dialog_box_2::on_pushButton_select_path_clicked()
{ // select destination where to export
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

void export_dialog_box_2::on_radioButton_screen_items_toggled(bool checked)
{ // export screen item which are displaying on screen if selected this radio button
    if(ui->radioButton_screen_items->isChecked())
    {
        report_scope_str = QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString);
    }
    else if(ui->radioButton_selected_items->isChecked())
    {
        report_scope_str = QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString);
    }
}
