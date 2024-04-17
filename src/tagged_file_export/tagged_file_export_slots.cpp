#include "tagged_file_export/tagged_file_export.h"
#include "ui_tagged_file_export.h"

void tagged_file_export::on_pushButton_open_clicked()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    if(ui->radioButton_export_to_folder->isChecked())
    {
        QString path;

        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                path = selected_files.at(0);
            }

            QString dir_name_with_date_time =  M_VAR_export_dir_name_QString + QString("_") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));
            if(!path.endsWith("/"))
                path.append("/");

            path = path + dir_name_with_date_time;
            ui->lineEdit_di_path->setPlaceholderText(path);

        }

    }
    else if(ui->radioButton_export_to_zip->isChecked())
    {
        QString path;
        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                path = selected_files.at(0);
            }

            QString dir_name_with_date_time = M_VAR_export_dir_name_QString + QString("_") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));
            if(!path.endsWith("/"))
                path.append("/");

            QDir dir ;
            dir.mkpath(path);

            path = path + dir_name_with_date_time + ".zip";
            ui->lineEdit_di_path->setPlaceholderText(path);
        }

    }
    else if(ui->radioButton_export_to_dmg->isChecked())
    {
        QString path;
        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                path = selected_files.at(0);
            }

            QString dir_name_with_date_time = M_VAR_export_dir_name_QString + QString("_") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));
            if(!path.endsWith("/"))
                path.append("/");

            QDir dir ;
            dir.mkpath(path);

            path = path + dir_name_with_date_time + ".dmg";
            ui->lineEdit_di_path->setPlaceholderText(path);
        }
    }

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}


void tagged_file_export::on_pushButton_export_clicked()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    if(!recon_helper_standard_obj->is_any_source_exist())
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Source does not exist.");
        return;
    }

    prepare_selected_tags_list();

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    if(ui->radioButton_export_to_folder->isChecked())
    {
        QString dir_path;

        bool bool_do_keep_dir_structure = false;
        if(ui->checkBox_recursive->isChecked())
            bool_do_keep_dir_structure = true;


        dir_path = ui->lineEdit_di_path->placeholderText();
        dir_path.append("_folder");

        if(!dir_path.endsWith("/"))
            dir_path.append("/");


        export_files_to_folder(bool_do_keep_dir_structure , dir_path.trimmed());
    }
    else if(ui->radioButton_export_to_zip->isChecked())
    {
        QString file_path;
        file_path = ui->lineEdit_di_path->placeholderText();

        QString zip_file_path = file_path;

        QString dir_path = zip_file_path;
        dir_path.chop(QString(".zip").size());

        dir_path.append("_zip");

        export_files_to_zip(zip_file_path , dir_path);
    }
    else if(ui->radioButton_export_to_dmg->isChecked())
    {
        QString dmg_path;

        bool bool_do_keep_dir_structure = false;
        if(ui->checkBox_recursive->isChecked())
            bool_do_keep_dir_structure = true;

        dmg_path = ui->lineEdit_di_path->placeholderText();

        QString tmp_dmg_path = dmg_path;

        QString dmg_dir_path = tmp_dmg_path;
        dmg_dir_path.chop(QString(".dmg").size());

        dmg_dir_path.append("_dmg");

        export_files_to_dmg(bool_do_keep_dir_structure , dmg_dir_path.trimmed(),dmg_path);
    }

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
    return ;
}

void tagged_file_export::on_radioButton_export_to_zip_toggled(bool checked)
{
    ui->lineEdit_di_path->clear();

    if(checked)
    {

        ui->checkBox_recursive->setEnabled(false);
        ui->checkBox_encrypt_zip->setEnabled(true);
    }
    else
    {
        ui->checkBox_recursive->setEnabled(true);
        ui->checkBox_encrypt_zip->setEnabled(false);
    }

    set_zip_file_path();
}

void tagged_file_export::on_radioButton_export_to_folder_toggled(bool checked)
{
    ui->lineEdit_di_path->clear();


    if(checked)
    {
        ui->checkBox_recursive->setEnabled(true);
        ui->checkBox_encrypt_zip->setEnabled(false);
    }
    else
    {
        ui->checkBox_recursive->setEnabled(false);
        ui->checkBox_encrypt_zip->setEnabled(true);
    }

    if(!result_dir_path.endsWith("/"))
        result_dir_path.append("/");


    QString dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString() + M_VAR_export_dir_name_QString + QString("_") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));

    ui->lineEdit_di_path->setPlaceholderText(dir_path);
}

void tagged_file_export::on_radioButton_export_to_dmg_toggled(bool checked)
{
    ui->lineEdit_di_path->clear();

    if(checked)
    {
        ui->checkBox_recursive->setEnabled(true);
        ui->checkBox_encrypt_zip->setEnabled(false);
    }
    else
    {
        ui->checkBox_recursive->setEnabled(false);
        ui->checkBox_encrypt_zip->setEnabled(true);
    }

    if(!result_dir_path.endsWith("/"))
        result_dir_path.append("/");


    QString dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString() + M_VAR_export_dir_name_QString + QString("_") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));

    ui->lineEdit_di_path->setPlaceholderText(dir_path);

}

void tagged_file_export::slot_thread_zip_file_export_finished()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    ui->checkBox_encrypt_zip->setChecked(false);


    if(ui->checkBox_md5->isChecked() || ui->checkBox_sha1->isChecked())
    {
        QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

        for(int count = 0; count < list_target_source_info.size(); count++)
        {
            if(bool_cancel_loading)
                break ;

            struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);

            QString virtual_source_path = obj.virtual_source_path;
            if(!QFileInfo(virtual_source_path).exists())
            {
                recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
                continue ;
            }

            QString source_count_name =  obj.source_count_name;
            if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
                continue ;

            QString hashes_dir;
            hashes_dir = zip_filename;
            if(hashes_dir.endsWith(".zip"))
                hashes_dir.chop(QString(".zip").size());

            QString csv_file_dir = export_dir_file_for_zip + "/" + hashes_dir + "_hashes" + "/";
            QDir dir;
            dir.mkpath(csv_file_dir);
            csv_file_dir += source_count_name;

            write_exported_files_details_in_csv(source_count_name , csv_file_dir);
        }
    }

    if(QFileInfo(exported_zip_dest_file_path).exists() && !bool_cancel_loading)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
        message_dialog_obj->show();
    }

    bool_cancel_loading = false;


    display_loading_progress_bar_obj->hide();


    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}


void tagged_file_export::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();

    export_rsync_obj->pub_cancel_rsync_copy(Q_FUNC_INFO);
    thread_zip_file_export_obj->pub_cancel_export();
}

void tagged_file_export::slot_rejected()
{
    // this slot called with Esc (escape) button press.

    if(lineedit_password_zip_enc->text().isEmpty())
    {
        //QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please enter Password for encryption.");
        ui->checkBox_encrypt_zip->setChecked(false);
        return;
    }
}

void tagged_file_export::slot_message_box_yes_no_clicked(bool status)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    if(status)
    {
        if(ui->radioButton_export_to_zip->isChecked())
        {
            point_to_export_dir(exported_zip_dest_file_path);
        }
        else if(ui->radioButton_export_to_folder->isChecked())
        {
            point_to_export_dir(exported_folder_dir_path);
        }
        else if(ui->radioButton_export_to_dmg->isChecked())
        {
            point_to_export_dir(exported_dmg_path);
        }
    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}
