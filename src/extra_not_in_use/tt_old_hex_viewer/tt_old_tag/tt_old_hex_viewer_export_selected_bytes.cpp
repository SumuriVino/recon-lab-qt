#include "tt_old_hex_viewer_export_selected_bytes.h"
#include "ui_tt_old_hex_viewer_export_selected_bytes.h"

hex_viewer_export_selected_bytes::hex_viewer_export_selected_bytes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hex_viewer_export_selected_bytes)
{
    ui->setupUi(this);
    ui->lineEdit_file_name->setText("");
    ui->lineEdit_file_path->setText("");

    ui->lineEdit_file_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_file_path->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->lineEdit_file_path->setReadOnly(true);

    QString name =QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") ;
    ui->lineEdit_file_name->setPlaceholderText(name);

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

hex_viewer_export_selected_bytes::~hex_viewer_export_selected_bytes()
{
    delete ui;
}

void hex_viewer_export_selected_bytes::pub_set_essentials()
{
    QString result_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Hex_Viewer_QString).toString();
    ui->lineEdit_file_path->setPlaceholderText(result_path);
}

void hex_viewer_export_selected_bytes::pub_clear_line_edit_text()
{
    ui->lineEdit_file_name->setText("");
    ui->lineEdit_file_path->setText("");
}

void hex_viewer_export_selected_bytes::on_pushButton_open_clicked()
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
    ui->lineEdit_file_path->setText(out_put_path);
}

void hex_viewer_export_selected_bytes::on_pushButton_save_clicked()
{
    QString file_name,file_path;
    if(ui->lineEdit_file_name->text().isEmpty())
    {
        file_name = ui->lineEdit_file_name->placeholderText();
    }
    else
    {
        file_name = ui->lineEdit_file_name->text();
    }

    if(ui->lineEdit_file_path->text().isEmpty())
    {
        file_path = ui->lineEdit_file_path->placeholderText();
    }
    else
    {
        file_path = ui->lineEdit_file_path->text();
    }

    emit signal_save_exported_file_click(file_name,file_path);
}

void hex_viewer_export_selected_bytes::on_pushButton_cancel_clicked()
{
    this->hide();
}
