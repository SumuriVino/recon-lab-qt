#include "tt_old_hex_viewer_export_tag_module.h"
#include "ui_tt_old_hex_viewer_export_tag_module.h"

export_tag_module::export_tag_module(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_tag_module)
{
    ui->setupUi(this);
    ui->lineEdit_output_path->setText("");

    ui->lineEdit_output_path->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_output_path->setReadOnly(true);

    QFont myfont;
    myfont.setPointSize(12);
    setFont(myfont);

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

export_tag_module::~export_tag_module()
{
    delete ui;
}

void export_tag_module::pub_set_recon_result_dir_path(QString path)
{
    QString recon_result_path = path;
    recon_result_path = recon_result_path + "/Exported_Files/hex_viewer";
    ui->lineEdit_output_path->setPlaceholderText(recon_result_path);
}

void export_tag_module::pub_clear_line_edit_text()
{
    ui->lineEdit_output_path->setText("");
}

void export_tag_module::on_pushButton_open_clicked()
{
    QString out_put_path;// = QFileDialog::getExistingDirectory(this, "Export Output", getenv("HOME"));

    file_dialog_obj.setFileMode(QFileDialog::DirectoryOnly);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            out_put_path = selected_files.at(0);
        }
    }
    ui->lineEdit_output_path->setText(out_put_path);
}

void export_tag_module::on_pushButton_export_clicked()
{
    QString output_path;
    if(ui->lineEdit_output_path->text().isEmpty())
    {
        output_path = ui->lineEdit_output_path->placeholderText();
    }
    else
    {
        output_path = ui->lineEdit_output_path->text();
    }

    emit export_clicked(output_path);
}
