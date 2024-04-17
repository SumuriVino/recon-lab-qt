#include "ram_analysis.h"
#include "ui_ram_analysis.h"


void ram_analysis::slot_ram_analysis_saved_result_in_file_by_saved_name(QString given_name , QString selected_dir_path, QString export_type_str)
{

    export_dialog_box_1_obj->hide();
    ui->pushButton_export->setEnabled(true);
    ui->pushButton_save->setEnabled(true);

    QString saved_name_str = given_name;
    if(saved_name_str.contains(" "))
        saved_name_str.replace(" ","_");

    if(!selected_dir_path.endsWith("/"))
        selected_dir_path.append("/");

    QString available_file =  recon_static_functions::get_available_filename(saved_name_str + ".txt", selected_dir_path, Q_FUNC_INFO);
    QString available_path = selected_dir_path + available_file;

    saved_ram_analaysis_in_file_on_export_button_clicked(available_path);

}

void ram_analysis::slot_ram_analysis_cancel_save_results()
{
    ui->pushButton_export->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
}

void ram_analysis::slot_ram_analysis_cancel_export_results()
{
    ui->pushButton_export->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
}

void ram_analysis::slot_ram_analysis_saved_result_in_db_by_saved_name(QString saved_name)
{
    save_result_dialog_box_obj->hide();

    ui->pushButton_save->setEnabled(true);

    ui->pushButton_export->setEnabled(true);

    QStringList values_list;

    QString saved_rm_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";
    QString command = QString("select result_name from tbl_saved_result where result_name = ?");
    values_list << saved_name;
    command = QString("select result_name from tbl_saved_result");

    QStringList already_saved_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, saved_rm_db_path, Q_FUNC_INFO);

    QString new_name = recon_static_functions::get_available_stringname(saved_name, already_saved_list, Q_FUNC_INFO);

    saved_ram_analaysis_in_db_on_save_button_clicked(new_name);

}
