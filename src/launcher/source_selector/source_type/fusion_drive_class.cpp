#include "fusion_drive_class.h"
#include "ui_fusion_drive_class.h"

Fusion_Drive_Class::Fusion_Drive_Class(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Fusion_Drive_Class)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

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

Fusion_Drive_Class::~Fusion_Drive_Class()
{
    delete ui;
}

void Fusion_Drive_Class::pub_set_id_and_name(int id, QString name) //set id and received name/source name in private variable
{
    received_id = id;
    received_name = name;
}

int Fusion_Drive_Class::pub_get_id() // return received id
{
    return received_id;
}

int Fusion_Drive_Class::pub_get_window_height() // return height of window
{
    return this->minimumHeight();
}


void Fusion_Drive_Class::on_pushButton_add_fusion_drives_clicked() // add fusion image after adding both image in line edit, ssd or platter
{
    if(ui->lineEdit_fusion_path_ssd->text().trimmed().isEmpty() || ui->lineEdit_fusion_path_platter->text().trimmed().isEmpty())
        return;

    QStringList list_path;
    list_path << ui->lineEdit_fusion_path_ssd->text().trimmed();
    list_path << ui->lineEdit_fusion_path_platter->text().trimmed();

    emit signal_fusion_drives_selected(list_path);
    this->hide();
}

void Fusion_Drive_Class::on_pushButton_ssd_clicked() // button to add ssd image
{


    QString file_path;
    file_dialog_obj.setNameFilter(("*.dmg *.DMG *.raw *.RAW *.dd *.DD *.e01 *.E01 *.ex01 *.Ex01 *.s01 *.S01 *.000 *.001 *.00001"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            file_path = selected_files.at(0);
        }
    }
    if(file_path.isEmpty())
        return;

    ui->lineEdit_fusion_path_ssd->setText(file_path);


}

void Fusion_Drive_Class::on_pushButton_platter_clicked() //button to add platter image
{

    QString file_path;
    file_dialog_obj.setNameFilter(("*.dmg *.DMG *.raw *.RAW *.dd *.DD *.e01 *.E01 *.ex01 *.Ex01 *.s01 *.S01 *.000 *.001 *.00001"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            file_path = selected_files.at(0);
        }
    }
    if(file_path.isEmpty())
        return;

    ui->lineEdit_fusion_path_platter->setText(file_path);

}

void Fusion_Drive_Class::on_pushButton_cancel_clicked() // cancel if don't want to add fusion image
{
    this->hide();
}
