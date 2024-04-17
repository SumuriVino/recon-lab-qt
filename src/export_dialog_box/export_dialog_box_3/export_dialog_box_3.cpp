#include "export_dialog_box_3.h"
#include "ui_export_dialog_box_3.h"

export_dialog_box_3::export_dialog_box_3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_dialog_box_3)
{
    ui->setupUi(this);

    ui->radioButton_screen_items->setChecked(true);
    //setWindowFlags(Qt::Window| Qt::FramelessWindowHint);

    setWindowModality(Qt::ApplicationModal);

    // setWindowFlags(Qt::Dialog|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowCloseButtonHint);

    ui->lineEdit_file_name->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_selected_path->setAttribute(Qt::WA_MacShowFocusRect, false);

    //    ui->comboBox_size_units->addItem(MACRO_Generic_Filter_Condition_Bytes);
    //    ui->comboBox_size_units->addItem(MACRO_Generic_Filter_Condition_KB);
    //    ui->comboBox_size_units->addItem(MACRO_Generic_Filter_Condition_MB);
    //    ui->comboBox_size_units->addItem(MACRO_Generic_Filter_Condition_GB);
    //    ui->comboBox_size_units->addItem(MACRO_Generic_Filter_Condition_TB);

    //    ui->lineEdit_file_name->setPlaceholderText("File System");
    //    QString home_dir = getenv("HOME");
    //    QString csv_dir_path = home_dir + "/Desktop";
    //    ui->lineEdit_selected_path->setPlaceholderText(csv_dir_path);

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

export_dialog_box_3::~export_dialog_box_3()
{
    delete ui;
}

void export_dialog_box_3::pub_set_window_title(QString w_title_name)
{
    setWindowTitle(w_title_name);
}

void export_dialog_box_3::pub_set_export_as_type(QString type_str)
{
    export_as_type = type_str;

}

void export_dialog_box_3::pub_set_file_name_and_file_path(QString file_name,QString output_dir_path)
{
    ui->lineEdit_file_name->setPlaceholderText(file_name.trimmed());
    ui->lineEdit_selected_path->setPlaceholderText(output_dir_path.trimmed());
}

void export_dialog_box_3::on_pushButton_export_clicked()
{

    QString csv_dir_path;
    if(ui->lineEdit_selected_path->text().trimmed().isEmpty())
        csv_dir_path = ui->lineEdit_selected_path->placeholderText();
    else
        csv_dir_path = ui->lineEdit_selected_path->text();


    QString file_name ;
    if(ui->lineEdit_file_name->text().trimmed().isEmpty())
        file_name = ui->lineEdit_file_name->placeholderText().replace(" ", "_");
    else
        file_name = ui->lineEdit_file_name->text().replace(" ", "_");



    csv_dir_path = csv_dir_path + "/" + file_name;

    QString csv_file_name;

    if(export_as_type == QString(MACRO_Generic_Export_As_CSV))
        csv_file_name = file_name + ".csv";
    else if(export_as_type == QString(MACRO_Generic_Export_As_SQLite))
        csv_file_name = file_name + ".sqlite";


    QString new_name = recon_static_functions::get_available_filename(csv_file_name, csv_dir_path , Q_FUNC_INFO);
    file_path = csv_dir_path + "/" + new_name;


    QString source_info_file_name = recon_static_functions::get_available_filename("source_info.txt", csv_dir_path , Q_FUNC_INFO);
    QString info_file_path = csv_dir_path + "/" + source_info_file_name;

    if(ui->radioButton_current_dir->isChecked() == true && ui->checkBox_recursive->checkState() == Qt::Checked)
    {
        emit signal_start_write_data_as_csv_sqlite(file_path ,"",true , false , info_file_path,export_as_type);
    }
    else if(ui->radioButton_screen_items->isChecked() == true)
    {
        emit signal_start_write_data_as_csv_sqlite(file_path , "",false ,true , info_file_path,export_as_type);
    }
    else if(ui->radioButton_current_dir->isChecked() == true && ui->checkBox_recursive->checkState() == Qt::Unchecked)
    {
        emit signal_start_write_data_as_csv_sqlite(file_path , "",false ,false , info_file_path,export_as_type);
    }
}

void export_dialog_box_3::on_pushButton_select_export_path_clicked()
{
    QString export_path;// = QFileDialog::getExistingDirectory(this,"",getenv("HOME"));

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            export_path = selected_files.at(0);
        }
    }

    if(export_path.trimmed() == QString(""))
        return;

    file_path = export_path;
    ui->lineEdit_selected_path->setText(export_path);

}


void export_dialog_box_3::on_radioButton_current_dir_clicked()
{

    if(ui->radioButton_screen_items->isChecked())
        ui->radioButton_screen_items->setDisabled(true);

}

void export_dialog_box_3::on_radioButton_screen_items_clicked()
{
    //    if(ui->radioButton_current_dir->isChecked())
    //        ui->radioButton_current_dir->setDisabled(true);

    //    if(ui->checkBox_recursive->checkState() == Qt::Checked)
    //        ui->checkBox_recursive->setCheckState(Qt::Unchecked);

}

void export_dialog_box_3::on_checkBox_recursive_clicked()
{

    //    if(ui->radioButton_screen_items->isChecked())
    //        ui->radioButton_screen_items->setDisabled(true);


}

void export_dialog_box_3::on_radioButton_screen_items_toggled(bool checked)
{
    if(checked)
        ui->checkBox_recursive->setEnabled(false);
}

void export_dialog_box_3::on_radioButton_current_dir_toggled(bool checked)
{
    if(checked)
        ui->checkBox_recursive->setEnabled(true);
}
