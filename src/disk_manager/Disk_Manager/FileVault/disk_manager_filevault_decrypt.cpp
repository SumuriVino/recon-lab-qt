#include "disk_manager_filevault_decrypt.h"
#include "ui_disk_manager_filevault_decrypt.h"

disk_manager_filevault_decrypt::disk_manager_filevault_decrypt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disk_manager_filevault_decrypt)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint  | Qt::WindowMaximizeButtonHint);


    QString title_str =  QString("Disk Manager") + QString(" - ") + QString("Decrypt");
    setWindowTitle(title_str);

    setFixedHeight(155);
    setFixedWidth(500);

    ui->lineEdit_password_or_recovery_key->setAttribute(Qt::WA_MacShowFocusRect, false);

    recon_helper_standard_obj = new imager_helper_standard(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->hide();

}

disk_manager_filevault_decrypt::~disk_manager_filevault_decrypt()
{
    delete ui;
}

struct_disk_info disk_manager_filevault_decrypt::pub_get_selected_device_details()
{
    return selected_dev_obj;
}

QString disk_manager_filevault_decrypt::pub_get_last_run_script_path_for_decrypt()
{
   return last_run_script_path;
}

void disk_manager_filevault_decrypt::pub_set_device_details(struct_disk_info dev_obj)
{ // set default device/disks details if connected or not
    selected_dev_obj = dev_obj;

    setWindowModality(Qt::ApplicationModal);

    ui->label_apfs_users->hide();
    ui->comboBox_apfs_users->hide();
    ui->comboBox_apfs_users->clear();
    ui->lineEdit_password_or_recovery_key->clear();
    ui->label_status->clear();

    on_pushButton_password_clicked(false);

    QString dev_str = selected_dev_obj.device_identifier;
    if(!dev_str.trimmed().isEmpty())
    {
        ui->label_status->setText(QString("Selected Device '") + dev_str + QString("'"));

        if(!dev_str.startsWith("/dev/"))
            dev_str.prepend("/dev/");

        selected_device_str = dev_str;
    }
    else
    {
        QMessageBox::warning(this,  QString("Disk Manager"), QString("Device not found."));
        hide();
        return;
    }


    selected_device_uuid = selected_dev_obj.disk_uuid;

    selected_device_fs_type = selected_dev_obj.filesystem_FilesystemType;
    if(selected_device_fs_type == "apfs")
    {
        selected_device_uuid = selected_dev_obj.apfs_physical_device_uuid;

        ui->label_apfs_users->show();
        ui->comboBox_apfs_users->show();

        QStringList apfs_users_list = get_apfs_users_id_list(selected_device_str);
        ui->comboBox_apfs_users->addItems(apfs_users_list);

    }


}

void disk_manager_filevault_decrypt::pub_stop_decryption()
{ // stop decryption
    recon_helper_standard_obj->pub_set_bool_process_generic(true);
}


void disk_manager_filevault_decrypt::on_pushButton_decrypt_clicked()
{
// decrypt any encrypted disk if knows password, decrypt button clicked
    ui->pushButton_decrypt->setEnabled(false);
    ///=======Prepare Decrypt essentials======///
    //password_or_recovery_key_str = ui->lineEdit_password_or_recovery_key->text().trimmed();
    password_or_recovery_key_str = ui->lineEdit_password_or_recovery_key->text();


    if(password_or_recovery_key_str.isEmpty())
    {
        QString tmp_str;
        if(ui->radioButton_password->isChecked())
            tmp_str = "Password";
        else if(ui->radioButton_recovery_key->isChecked())
            tmp_str = "Recovery Key";


        QMessageBox::warning(this, QString("Disk Manager"), QString("Please enter the '") + tmp_str + QString("' for decryption."));
        ui->pushButton_decrypt->setEnabled(true);

        return;
    }

    if(ui->comboBox_apfs_users->count() > 0)
        apfs_selected_user = ui->comboBox_apfs_users->currentText().trimmed();
    else
        apfs_selected_user.clear();


//    password_or_recovery_key_str.replace("\\","\\\\");
//    password_or_recovery_key_str.replace("!","\\!");
//    password_or_recovery_key_str.replace("(","\\(");
//    password_or_recovery_key_str.replace(")","\\)");
//    password_or_recovery_key_str.replace("&","\\&");
//    password_or_recovery_key_str.replace("$","\\$");
//    password_or_recovery_key_str.replace("|","\\|");
//    password_or_recovery_key_str.replace("\"","\\\"");
//    password_or_recovery_key_str.replace("<","\\<");
//    password_or_recovery_key_str.replace(">","\\>");
//    password_or_recovery_key_str.replace("'","\\'");
//    password_or_recovery_key_str.replace(";","\\;");
//    password_or_recovery_key_str.replace("`","\\`");
    password_or_recovery_key_str = recon_static_functions::pub_get_simplified_password_for_shell_script(password_or_recovery_key_str, Q_FUNC_INFO);


    QString script_path = get_decrypt_script_path();

    last_run_script_path = script_path;

    disable_enable_ui_on_decrypt_clicked(false);

    QStringList args_list;
    args_list << script_path;

    QString output_str = recon_helper_standard_obj->run_command_with_arguments("sh", args_list, Q_FUNC_INFO);

    if(output_str.contains("Logical Volume successfully unlocked") || output_str.contains("Unlocked but did not mount APFS Volume"))
    {
        emit signal_decrypt_finished(true);
        return;
    }
    else
    {
        emit signal_decrypt_finished(false);
        return;
    }
}

void disk_manager_filevault_decrypt::disable_enable_ui_on_decrypt_clicked(bool status)
{ // disable ui of decrypt button on particular scenario
    ui->pushButton_decrypt->setEnabled(status);
    ui->lineEdit_password_or_recovery_key->setEnabled(status);

    if(!status)
        ui->progressBar->show();
    else
        ui->progressBar->hide();

    ui->pushButton_password->setEnabled(status);

}

void disk_manager_filevault_decrypt::pub_enable_ui_on_finish(bool status)
{ // enable ui of decrypt button after completion
    disable_enable_ui_on_decrypt_clicked(status);
}

QString disk_manager_filevault_decrypt::get_decrypt_script_path()
{ // get script of path decrypt which runs on decrypt button clicked
    QString tmp_dir = QString(global_variable_private_var_tmp_Dir_Path_Case_QString);
    if(!QFileInfo(tmp_dir).exists())
    {
        QDir dir;
        dir.mkpath(tmp_dir);
    }

    QString script_path = tmp_dir + "tmp_recon_imager.sh";
    QFile file_script(script_path);
    if(file_script.exists())
        file_script.remove();

    if(!file_script.open(QIODevice::WriteOnly))
    {
        return QString("");
    }

    QStringList script_data_list;
    if(selected_device_fs_type == "apfs")
    {
        if(selected_device_str.isEmpty())
        {
            script_data_list << QString("printf")   << QString("'%s'") << QString("\"") + password_or_recovery_key_str + QString("\"") << QString("|")
                             << QString("diskutil") << QString("apfs") << QString("unlockVolume") << selected_device_str
                             << QString("-nomount") << QString("-stdinpassphrase");
        }
        else
        {
            script_data_list << QString("printf")   << QString("'%s'") << QString("\"") + password_or_recovery_key_str + QString("\"") << QString("|")
                             << QString("diskutil") << QString("apfs") << QString("unlockVolume") << selected_device_str
                             << QString("-user") << apfs_selected_user
                             << QString("-nomount") << QString("-stdinpassphrase");
        }
    }
    else
    {
        script_data_list << QString("printf")   << QString("'%s'") << QString("\"") + password_or_recovery_key_str + QString("\"") << QString("|")
                         << QString("diskutil") << QString("cs") << QString("unlockVolume") << selected_device_uuid
                         << QString("-stdinpassphrase");

    }

    QString script_data_str = script_data_list.join(" ");
    file_script.write(script_data_str.toLocal8Bit(),script_data_str.toLocal8Bit().size());

    file_script.flush();
    file_script.close();

    return script_path;
}



void disk_manager_filevault_decrypt::on_pushButton_password_clicked(bool checked)
{ // password see/hide button clicked after decrypt button clicked

    if(checked)
    {
        ui->pushButton_password->setIcon(QIcon("../icons/signs/eye_close.png"));
        ui->lineEdit_password_or_recovery_key->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pushButton_password->setIcon(QIcon("../icons/signs/eye_open.png"));
        ui->lineEdit_password_or_recovery_key->setEchoMode(QLineEdit::Password);
    }
}

void disk_manager_filevault_decrypt::on_pushButton_cancel_clicked()
{ // cancel button clicked while decryption
    hide();
}

QStringList disk_manager_filevault_decrypt::get_apfs_users_id_list(QString disk_node_str)
{ // get apfs users id lists

    disk_node_str = disk_node_str.trimmed();

    QStringList user_id_list;
    if(disk_node_str.isEmpty())
        return user_id_list;

    QStringList args_list;
    args_list << "apfs" << "listUsers" << disk_node_str;

    QProcess process;
    process.start("diskutil", args_list);

    process.waitForFinished();

    QByteArray output_arr = process.readAllStandardOutput();

    QStringList output_split_list = QString::fromLocal8Bit(output_arr, output_arr.size()).split("\n");

    for(int count = 0; count < output_split_list.size(); count++)
    {
        QString tmp_split_str = output_split_list.at(count);
        if(tmp_split_str.startsWith("+--"))
        {
            tmp_split_str.remove("+--");
            tmp_split_str = tmp_split_str.trimmed();

            user_id_list << tmp_split_str;
        }
    }


    return user_id_list;
}

void disk_manager_filevault_decrypt::pub_try_decryption_target_disk_mode_using_timer(struct_disk_info selected_dev_obj, QString last_run_scrpt_path)
{ // decryption another method using timer
    last_run_script_path = last_run_scrpt_path;

    disable_enable_ui_on_decrypt_clicked(false);

    QStringList args_list;
    args_list << last_run_scrpt_path;

    QString output_str = recon_helper_standard_obj->run_command_with_arguments("sh", args_list, Q_FUNC_INFO);

    disable_enable_ui_on_decrypt_clicked(true);

    ui->pushButton_decrypt->setEnabled(true);

    if(output_str.contains("Logical Volume successfully unlocked") || output_str.contains("Unlocked but did not mount APFS Volume"))
    {
        emit signal_decrypt_finished(true);
        return;
    }
    else
    {
        emit signal_decrypt_finished(false);
        return;
    }
}
