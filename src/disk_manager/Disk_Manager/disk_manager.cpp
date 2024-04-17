#include "disk_manager.h"
#include "ui_disk_manager.h"

disk_manager::disk_manager(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::disk_manager)
{
    ui->setupUi(this);

    ui->label_disk_arbitration->setText(QString("Disk Arbitration"));
    ui->pushButton_arbitration->setText(QString("Turn OFF"));

    setWindowTitle(QString("Disk Manager"));

    //ui->pushButton_mount_read->hide();
    //ui->pushButton_decrypt->hide();
    //ui->pushButton_unmount->hide();
    ui->pushButton_mount_read_write->hide();
    ui->pushButton_format->hide();

    bool_mount_blocker_process_running = false;

    // pub_display_data();

    ui->tableWidget_disk_list->hideColumn(enum_imager_mount_path);
    ui->tableWidget_disk_list->hideColumn(enum_imager_encryption_status);
    ui->tableWidget_disk_list->hideColumn(enum_imager_device_location);

    ui->tableWidget_disk_list->setColumnWidth(enum_imager_serial_number, 20);
    ui->tableWidget_disk_list->setColumnWidth(enum_imager_device, 80);
    ui->tableWidget_disk_list->setColumnWidth(enum_imager_model, 160);
    ui->tableWidget_disk_list->setColumnWidth(enum_imager_type, 180);


    display_dialog_box_obj_non_aborted = new display_progress_dialog_box(this);
    display_dialog_box_obj_non_aborted->pub_hide_cancel_button();


    recon_helper_standard_obj = new imager_helper_standard(this);

    ui->pushButton_format->setEnabled(false);
    disk_manager_format_disk_obj = new disk_manager_format_disk();
    connect(disk_manager_format_disk_obj, SIGNAL(signal_format_finished(bool)), this, SLOT(slot_format_finished(bool)));

    disk_manager_decrypt_disk_obj = new disk_manager_filevault_decrypt();
    connect(disk_manager_decrypt_disk_obj, SIGNAL(signal_decrypt_finished(bool)), this, SLOT(slot_decrypt_finished(bool)));

    ui->pushButton_decrypt->setEnabled(false);

    timer_tdm_total_seconds_elapsed    = 0;
    timer_tdm_wait_seconds_elapsed     = 0;
    tdm_redecryption_tried_count       = 0;
    tdm_redecryption_max_tryout_count  = 5;

    prepare_dialog_for_tdm_decryption_attempt();

}

disk_manager::~disk_manager()
{
    delete ui;
}

void disk_manager::pub_stop_disk_arbitration()
{
    QProcess::execute("killall", QStringList("disk_arbitration"));

    if(proc_disk_arb.state() == QProcess::Running)
        proc_disk_arb.terminate();

    bool_mount_blocker_process_running = false;
}

void disk_manager::pub_start_disk_arbitration()
{
    if(proc_disk_arb.state() == QProcess::Running)
    {
        bool_mount_blocker_process_running = true;
        return;
    }

    proc_disk_arb.start("./disk_manager/disk_arbitration.app/Contents/MacOS/disk_arbitration");
    proc_disk_arb.waitForStarted();

    bool_mount_blocker_process_running = true;
}

bool disk_manager::pub_get_disk_arbitration_started_status()
{
    return bool_mount_blocker_process_running;
}

void disk_manager::pub_display_data()
{

    list_all_disk_info = recon_helper_standard_obj->extract_all_disk_identifier();
    //list_all_disk_info = recon_helper_singular_obj->pub_device_list();

    ui->tableWidget_disk_list->setRowCount(0);

    QCoreApplication::processEvents();

    int row_count = 0;
    for(int disk_count = 0; disk_count < list_all_disk_info.size(); disk_count++)
    {
        QCoreApplication::processEvents();

        struct_disk_info m_obj = list_all_disk_info.at(disk_count);

        QString device_location_str = m_obj.is_internal_Internal ? QString("Internal") : QString("External");
        if(m_obj.is_internal_Internal)
            continue;

        //ui->tableWidget_disk_list->insertRow(disk_count);
        //ui->tableWidget_disk_list->setRowHeight(disk_count, 27);
        ui->tableWidget_disk_list->insertRow(row_count);
        ui->tableWidget_disk_list->setRowHeight(row_count, 27);





        QTableWidgetItem *item_serial = new QTableWidgetItem;
        QTableWidgetItem *item_device = new QTableWidgetItem;
        QTableWidgetItem *item_device_location = new QTableWidgetItem;
        QTableWidgetItem *item_model = new QTableWidgetItem;
        QTableWidgetItem *item_size  = new QTableWidgetItem;
        QTableWidgetItem *item_type  = new QTableWidgetItem;
        QTableWidgetItem *item_name  = new QTableWidgetItem;
        QTableWidgetItem *item_filesystem = new QTableWidgetItem;
        QTableWidgetItem *item_derived_from = new QTableWidgetItem;
        QTableWidgetItem *item_is_encrypted    = new QTableWidgetItem;
        QTableWidgetItem *item_encryption_status = new QTableWidgetItem;
        QTableWidgetItem *item_mode = new QTableWidgetItem;
        QTableWidgetItem *item_mount_path = new QTableWidgetItem;
        QTableWidgetItem *item_blank = new QTableWidgetItem;

        item_device->setText(m_obj.device_identifier);
        item_device_location->setText(device_location_str);
        item_model->setText(m_obj.model_MediaName);
        item_size->setText(m_obj.total_size_human_readable);
        item_size->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        item_type->setText(m_obj.type_Content);
        item_name->setText(m_obj.name_VolumeName);
        item_filesystem->setText(m_obj.filesystem_FilesystemType);
        item_derived_from->setText(m_obj.derived_from);

        if(m_obj.bool_is_locked || m_obj.apfs_bool_is_locked)
            item_is_encrypted->setText(QString("YES"));
        else
            item_is_encrypted->setText(QString("NO"));


        if(m_obj.bool_is_encrypt || m_obj.apfs_bool_is_encrypt)
            item_encryption_status->setText(QString("YES"));
        else
            item_encryption_status->setText(QString("NO"));


        item_mode->setText(m_obj.mode);
        item_mount_path->setText(m_obj.mount_point_MountPoint);
        item_blank->setText("");

        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_serial_number, item_serial);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_device, item_device);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_device_location, item_device_location);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_model, item_model);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_size, item_size);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_type, item_type);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_name, item_name);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_filesystem, item_filesystem);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_derived_from, item_derived_from);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_is_encrypted, item_is_encrypted);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_encryption_status, item_encryption_status);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_mode, item_mode);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_mount_path, item_mount_path);
        //        ui->tableWidget_disk_list->setItem(disk_count, enum_imager_blank, item_blank);

        ui->tableWidget_disk_list->setItem(row_count, enum_imager_serial_number, item_serial);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_device, item_device);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_device_location, item_device_location);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_model, item_model);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_size, item_size);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_type, item_type);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_name, item_name);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_filesystem, item_filesystem);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_derived_from, item_derived_from);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_is_encrypted, item_is_encrypted);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_encryption_status, item_encryption_status);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_mode, item_mode);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_mount_path, item_mount_path);
        ui->tableWidget_disk_list->setItem(row_count, enum_imager_blank, item_blank);

        row_count++;
    }

    ui->tableWidget_disk_list->horizontalHeader()->stretchLastSection();
    if(ui->tableWidget_disk_list->rowCount() > 0)
    {
        ui->tableWidget_disk_list->scrollToItem(ui->tableWidget_disk_list->item(0,0));
        ui->tableWidget_disk_list->selectRow(0);
    }

    change_background_color_in_table();
}

void disk_manager::pub_display_data_with_progress_message()
{
    display_dialog_box_obj_non_aborted->pub_set_label_message("Refreshing device list...");
    display_dialog_box_obj_non_aborted->show();

    pub_display_data();

    display_dialog_box_obj_non_aborted->hide();
}


void disk_manager::change_background_color_in_table()
{
    for(int row = 0; row < ui->tableWidget_disk_list->rowCount(); row++)
    {
        QString dev_node_text = ui->tableWidget_disk_list->item(row, enum_imager_device)->text().trimmed();
        struct_disk_info disk_obj = recon_helper_standard_obj->get_device_struct_info_according_identifier(dev_node_text);

        if(disk_obj.is_whole_disk)
            change_background_color_in_each_column(row, QString(disk_manager_backround_color_Gray));
        if(!disk_obj.derived_from.trimmed().isEmpty())
            change_background_color_in_each_column(row, QString(disk_manager_backround_color_Yellow));
        if(disk_obj.type_Content.trimmed() == QString("Apple_CoreStorage"))
            change_background_color_in_each_column(row, QString(disk_manager_backround_color_Orange));
        if(disk_obj.bool_is_encrypt || disk_obj.apfs_bool_is_encrypt)
            change_background_color_in_each_column(row, QString(disk_manager_backround_color_Olive));
        if(disk_obj.filesystem_FilesystemType.trimmed() == "apfs")
            change_background_color_in_each_column(row, QString(disk_manager_backround_color_Light_Brown));
        if(disk_obj.filesystem_FilesystemType.trimmed() == "apfs" && (disk_obj.bool_is_encrypt ||  disk_obj.apfs_bool_is_encrypt) && !disk_obj.bool_is_locked)
            change_background_color_in_each_column(row, QString(disk_manager_backround_color_Olive));


        QString mount_mode_text = ui->tableWidget_disk_list->item(row, enum_imager_mode)->text().trimmed();
        if(!mount_mode_text.isEmpty())
        {
            if(mount_mode_text == QString(mount_mode_ReadWrite))
            {
                change_background_color_in_each_column(row, QString(disk_manager_backround_color_Red));
            }
            else if(mount_mode_text == QString(mount_mode_ReadOnly))
            {
                change_background_color_in_each_column(row, QString(disk_manager_backround_color_Green));
            }
        }
    }
}

void disk_manager::change_background_color_in_each_column(int row , QString color_str)
{
    color_str = color_str.trimmed();
    if(color_str.isEmpty())
        return;

    // e50000 F08080
    QColor back_color;
    if(color_str == QString(disk_manager_backround_color_Red))
        back_color = QColor(color_Red);
    else if(color_str == QString(disk_manager_backround_color_Green))
        back_color = QColor(color_Green);
    else if(color_str == QString(disk_manager_backround_color_Gray))
        back_color = QColor(Qt::gray);
    else if(color_str == QString(disk_manager_backround_color_Yellow))
        back_color = QColor(Qt::yellow);
    else if(color_str == QString(disk_manager_backround_color_Orange))
        back_color = QColor(255,165,0);
    else if(color_str == QString(disk_manager_backround_color_Light_Brown))
        back_color = QColor(181,101,29);
    else if(color_str == QString(disk_manager_backround_color_Olive))
        back_color = QColor(color_Olive);

    for(int column = 0; column < ui->tableWidget_disk_list->columnCount(); column++)
    {
        ui->tableWidget_disk_list->item(row, column)->setBackground(back_color);
    }
}

void disk_manager::on_tableWidget_disk_list_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    ///format button disable/enable
    //    QString device_location = ui->tableWidget_disk_list->item(row, enum_imager_device_location)->text();
    //    if(device_location == QString("Internal"))
    //        ui->pushButton_format->setDisabled(true);
    //    else
    //        ui->pushButton_format->setDisabled(false);

    ///Mount-R button disable/enable
    QString mount_path_str = ui->tableWidget_disk_list->item(row, enum_imager_mount_path)->text().trimmed();
    if(mount_path_str.isEmpty())
        ui->pushButton_mount_read->setEnabled(true);
    else
        ui->pushButton_mount_read->setEnabled(false);


    ///decrypt button disable/enable
    QString encrypted_str = ui->tableWidget_disk_list->item(row, enum_imager_is_encrypted)->text();
    if(encrypted_str.trimmed() == QString("YES"))
    {
        ui->pushButton_decrypt->setEnabled(true);

        ui->pushButton_mount_read->setEnabled(false);
    }
    else
        ui->pushButton_decrypt->setEnabled(false);
}

void disk_manager::on_pushButton_unmount_clicked()
{
    int row = ui->tableWidget_disk_list->currentRow();

    if(row < 0)
        return;

    QString device_node = ui->tableWidget_disk_list->item(row, enum_imager_device)->text();
    pub_unmount_disk(device_node);
}

void disk_manager::on_pushButton_refresh_clicked()
{
    display_dialog_box_obj_non_aborted->pub_set_label_message("Refreshing device list...");
    display_dialog_box_obj_non_aborted->show();

    //list_all_disk_info = recon_helper_singular_obj->extract_all_disk_identifier();
    pub_display_data();

    display_dialog_box_obj_non_aborted->hide();
    //display_filevaulted_message_with_disk_name();
}

void disk_manager::on_pushButton_mount_read_clicked()
{
    int row = ui->tableWidget_disk_list->currentRow();

    if(row < 0)
        return;

    QString device_node = ui->tableWidget_disk_list->item(row, enum_imager_device)->text();
    display_dialog_box_obj_non_aborted->pub_set_label_message("Please wait...");
    display_dialog_box_obj_non_aborted->show();

    pub_mount_read_only(device_node);

    pub_display_data();
    display_dialog_box_obj_non_aborted->hide();

}

void disk_manager::refresh_display_with_existing_disklist()
{
    display_dialog_box_obj_non_aborted->pub_set_label_message("Refreshing device list...");
    display_dialog_box_obj_non_aborted->show();
    pub_display_data();
    display_dialog_box_obj_non_aborted->hide();
    //display_filevaulted_message_with_disk_name();
}

void disk_manager::on_pushButton_mount_read_write_clicked()
{
    int row = ui->tableWidget_disk_list->currentRow();

    if(row < 0)
        return;

    QString device_node = ui->tableWidget_disk_list->item(row, enum_imager_device)->text();

    pub_mount_read_write(device_node);

}

void disk_manager::on_pushButton_format_clicked()
{
    int row = ui->tableWidget_disk_list->currentRow();
    if(row < 0)
        return;

    //    QString device_node = ui->tableWidget_disk_list->item(row, enum_imager_device)->text();

    //    struct_disk_info selected_dev_obj = recon_helper_singular_obj->get_device_struct_info_according_identifier(device_node);

    QString device_node = ui->tableWidget_disk_list->item(row, enum_imager_device)->text();
    struct_disk_info selected_dev_obj = recon_helper_standard_obj->get_device_struct_info_according_identifier(device_node);

    ///===========Check for Root Disk===========///
    if(recon_helper_standard_obj->pub_check_is_root_disk(selected_dev_obj))
    {
        QMessageBox::warning(this, QString("Disk Manager"), "Can not format RECON Imager disk.");
        return;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr(""),
                               "You are about to format '" + device_node + "' , Do you want to proceed ?",
                               QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No)
    {
        return;
    }

    disk_manager_format_disk_obj->pub_set_device_details(selected_dev_obj);
    disk_manager_format_disk_obj->show();
}

void disk_manager::slot_format_finished(bool status)
{
    disk_manager_format_disk_obj->hide();
    if(status)
    {
        on_pushButton_refresh_clicked();
        QMessageBox::warning(this, QString("Disk Manager"), "Format Process Completed.");
    }
    else
    {
        QMessageBox::warning(this, QString("Disk Manager"), "Format Failed, Something went wrong.");
    }

}

void disk_manager::on_pushButton_decrypt_clicked()
{
    int row = ui->tableWidget_disk_list->currentRow();
    if(row < 0)
        return;

    QString device_node = ui->tableWidget_disk_list->item(row, enum_imager_device)->text();
    struct_disk_info selected_dev_obj = recon_helper_standard_obj->get_device_struct_info_according_identifier(device_node);

    if(selected_dev_obj.disk_uuid.trimmed().isEmpty() && selected_dev_obj.filesystem_FilesystemType != "apfs")
    {
        QMessageBox::warning(this, QString("Disk Manager"), "Encrypted Device UUID Not Found.");
        return;
    }

    bool_tdm_decrption_cancel_clicked = false;
    disk_manager_decrypt_disk_obj->pub_set_device_details(selected_dev_obj);
    disk_manager_decrypt_disk_obj->show();
}

void disk_manager::slot_decrypt_finished(bool status)
{
    if(status)
    {
        /// check for TDM (if not decrypted try decrypt in background for 5 minutes)
        struct_disk_info selected_dev_before_refresh_obj = disk_manager_decrypt_disk_obj->pub_get_selected_device_details();
        QString last_run_script_path = disk_manager_decrypt_disk_obj->pub_get_last_run_script_path_for_decrypt();

        //refresh disklist
        list_all_disk_info = recon_helper_standard_obj->extract_all_disk_identifier();

        if(bool_tdm_decrption_cancel_clicked)
        {
            if(timer_tdm_decrypt != nullptr)
            {
                if(timer_tdm_decrypt->isActive())
                    timer_tdm_decrypt->stop();

                delete timer_tdm_decrypt;
                timer_tdm_decrypt = nullptr;
            }

            dialog_tdm_decrypt->hide();
        }
        else
        {
            struct_disk_info selected_dev_after_refresh_obj = recon_helper_standard_obj->get_device_struct_info_according_identifier(selected_dev_before_refresh_obj.device_identifier);
            struct_disk_info selected_dev_parent_obj = recon_helper_standard_obj->get_device_struct_info_according_identifier(selected_dev_after_refresh_obj.parent_whole_disk);

            if(selected_dev_parent_obj.model_MediaName.trimmed() == QString(Macro_Target_Disk_Mode_Model_Media_Name)
                    && selected_dev_after_refresh_obj.filesystem_FilesystemType.toLower() == QString("apfs")
                    && selected_dev_after_refresh_obj.apfs_bool_is_locked)
            {

                if(timer_tdm_decrypt == nullptr)
                {
                    timer_tdm_total_seconds_elapsed = 0;
                    tdm_redecryption_tried_count   = 0;
                    timer_tdm_wait_seconds_elapsed = 0;
                    timer_tdm_decrypt = new QTimer(this);
                    connect(timer_tdm_decrypt,SIGNAL(timeout()), this, SLOT(slot_tdm_decrypt_timer_tick()));

                    disk_manager_decrypt_disk_obj->pub_enable_ui_on_finish(true);
                    disk_manager_decrypt_disk_obj->hide();

                    ///not decryted yet, try in bakground
                    timer_tdm_decrypt->start(1000);
                    dialog_tdm_decrypt->show();

                }
                else
                {
                    ///wait for one minute for next try
                    if(timer_tdm_wait_seconds_elapsed < Macro_Timer_Counter_10_Seconds_int)
                    {
                        int count = 0;
                        while(timer_tdm_wait_seconds_elapsed < Macro_Timer_Counter_10_Seconds_int)
                        {
                            count++;
                            if(count % 25 == 0)
                            {
                                count = 0;
                                QCoreApplication::processEvents();
                            }
                        }
                        timer_tdm_wait_seconds_elapsed = 0;
                    }
                }

                tdm_redecryption_tried_count += 1;

                label_attempt_value->setText(QString::number(tdm_redecryption_tried_count));
                disk_manager_decrypt_disk_obj->pub_try_decryption_target_disk_mode_using_timer(selected_dev_after_refresh_obj, last_run_script_path);
                return;
            }
            else if(selected_dev_parent_obj.model_MediaName.trimmed() == QString(Macro_Target_Disk_Mode_Model_Media_Name)
                    && selected_dev_after_refresh_obj.filesystem_FilesystemType.toLower() == QString("apfs")
                    && !selected_dev_after_refresh_obj.apfs_bool_is_locked)
            {
                /// if decryption status 'True' appears at any moment
                if(timer_tdm_decrypt != nullptr)
                {
                    if(timer_tdm_decrypt->isActive())
                        timer_tdm_decrypt->stop();

                    delete timer_tdm_decrypt;
                }

                dialog_tdm_decrypt->hide();
                ///do not use return
            }
        }

        disk_manager_decrypt_disk_obj->pub_enable_ui_on_finish(true);
        disk_manager_decrypt_disk_obj->hide();

        ///refresh display
        refresh_display_with_existing_disklist();

        if(bool_tdm_decrption_cancel_clicked)
        {
            QMessageBox::warning(this, QString("Disk Manager"), "Decrypt Process Cancelled.");
        }
        else
            QMessageBox::warning(this, QString("Disk Manager"), "Decrypt Process Completed.");
    }
    else
    {
        disk_manager_decrypt_disk_obj->pub_enable_ui_on_finish(true);
        disk_manager_decrypt_disk_obj->hide();

        QMessageBox::warning(this, QString("Disk Manager"), "Decrypt Failed, Something went wrong.");
    }

}

void disk_manager::slot_tdm_decrypt_timer_tick()
{
    timer_tdm_total_seconds_elapsed += 1;
    timer_tdm_wait_seconds_elapsed  += 1;

    QString time_str = recon_helper_standard_obj->get_human_readable_time(QString::number(timer_tdm_total_seconds_elapsed), Q_FUNC_INFO);
    label_time_elapsed_value->setText(time_str);

}

void disk_manager::pub_mount_read_only(QString device_node)
{
    if(device_node.trimmed().isEmpty())
        return;

    struct_disk_info source_disk_obj_for_device_node;
    QString catalina_device_node;
    source_disk_obj_for_device_node = recon_helper_standard_obj->get_device_struct_info_according_identifier(device_node);

    bool bool_source_other_than_system_and_data = false;
    bool bool_is_source_catalina = false;
    bool bool_is_running_from_diskutil = false;

    bool_source_other_than_system_and_data = recon_helper_standard_obj->pub_source_other_than_system_and_data(device_node,recon_helper_standard_obj->pub_device_list());

    if(!bool_source_other_than_system_and_data)
    {
        bool_is_source_catalina = recon_helper_standard_obj->pub_check_is_catalina_drive(source_disk_obj_for_device_node, recon_helper_standard_obj->pub_device_list());
    }

    if(bool_is_source_catalina)
        recon_static_functions::app_debug(QString(" -bool_is_source_catalina-") + QString("YES"), Q_FUNC_INFO);
    else
        recon_static_functions::app_debug(QString(" -bool_is_source_catalina-") + QString("NO"), Q_FUNC_INFO);


    if(bool_is_source_catalina)
    {
        bool_is_running_from_diskutil = true;
        QStringList catalina_system_and_data_disk_node_list = recon_helper_standard_obj->pub_get_catalina_system_and_data_disk_node_list(source_disk_obj_for_device_node, recon_helper_standard_obj->pub_device_list());

        QStringList catalina_device_mounted_node_list;
        QStringList catalina_device_fs_node_list;

        for(int ii = 0; ii < catalina_system_and_data_disk_node_list.size(); ii++)
        {
            QString catalina_device_node = catalina_system_and_data_disk_node_list.at(ii);

            QString mount_path_for_catalina_partition = recon_helper_standard_obj->extract_device_mount_path_using_device_node(catalina_device_node);
            if(mount_path_for_catalina_partition.trimmed() != NOMOUNT)
            {
                catalina_device_mounted_node_list << catalina_device_node;
                continue;
            }

            QString fs_name_for_catalina_partition = recon_helper_standard_obj->extract_device_fs_name_using_device_node(catalina_device_node);
            if(!(fs_name_for_catalina_partition.contains("hfs", Qt::CaseInsensitive) || fs_name_for_catalina_partition.contains("apfs", Qt::CaseInsensitive) || fs_name_for_catalina_partition.contains("ntfs", Qt::CaseInsensitive) || fs_name_for_catalina_partition.contains("exfat", Qt::CaseInsensitive)))
            {
                catalina_device_fs_node_list << catalina_device_node;
                continue;
            }

            QString available_dir = recon_static_functions::get_available_filename("mnt", MOUNT_POINT_PARENT_DIR, Q_FUNC_INFO);

            QString mount_path = QString(MOUNT_POINT_PARENT_DIR) + "/" + available_dir;
            QDir dir;
            dir.mkpath(mount_path);
            if(!catalina_device_node.startsWith("/dev"))
                catalina_device_node = "/dev/" + catalina_device_node;

            QStringList args_for_catalina;

            args_for_catalina << QString("diskutil") << QString("mount") << QString("readonly") << catalina_device_node;

            QString tmp_dir = QString(global_variable_private_var_tmp_Dir_Path_Case_QString);
            if(!QFileInfo(tmp_dir).exists())
            {
                QDir dir;
                dir.mkpath(tmp_dir);
            }

            QString script_path = tmp_dir + "tmp_recon_mount.sh";

            QString script_data_str = args_for_catalina.join(" ");

            recon_helper_standard_obj->pub_write_date_into_file(script_path, script_data_str, Q_FUNC_INFO);

            args_for_catalina.clear();
            args_for_catalina << script_path;

            bool bool_was_arbitration_running = bool_mount_blocker_process_running;
            if(bool_was_arbitration_running && bool_is_running_from_diskutil)
            {
                pub_stop_disk_arbitration();
            }

            recon_helper_standard_obj->run_command_with_arguments("./sudoer/RECON", args_for_catalina, Q_FUNC_INFO);

            if(bool_was_arbitration_running && bool_is_running_from_diskutil)
            {
                pub_start_disk_arbitration();
            }
        }

        if(!catalina_device_mounted_node_list.empty())
        {
            QString temp_sperator = QString(" , ");
            QString temp_node_str = catalina_device_mounted_node_list.join(temp_sperator);
            if(temp_node_str.endsWith(temp_sperator))
            {
                temp_node_str.chop(temp_sperator.size());
            }
            temp_node_str = temp_node_str.trimmed();
            QMessageBox::warning(this, QString("Disk Manager"), temp_node_str +  QString(" ") + QString("is already mounted."));
            return;
        }
        if(!catalina_device_fs_node_list.empty())
        {
            QString temp_sperator = QString(" , ");
            QString temp_node_str = catalina_device_fs_node_list.join(temp_sperator);
            if(temp_node_str.endsWith(temp_sperator))
            {
                temp_node_str.chop(temp_sperator.size());
            }
            temp_node_str = temp_node_str.trimmed();
            QMessageBox::warning(this, QString("Disk Manager"), QString("The")  + QString(" ") + temp_node_str + QString(" ") + QString("does not contain HFS, APFS or NTFS filesystem. Can not mount the device."));
            return;
        }
        return;
    }
    else
    {
        QString mount_path = recon_helper_standard_obj->extract_device_mount_path_using_device_node(device_node);
        if(mount_path.trimmed() != NOMOUNT)
        {
            QMessageBox::warning(this, QString("Disk Manager"), QString("Device is already mounted"));
            return;
        }

        QString fs_name = recon_helper_standard_obj->extract_device_fs_name_using_device_node(device_node);

        if(!(fs_name.contains("hfs", Qt::CaseInsensitive) || fs_name.contains("apfs", Qt::CaseInsensitive) || fs_name.contains("ntfs", Qt::CaseInsensitive) || fs_name.contains("exfat", Qt::CaseInsensitive)))
        {
            QMessageBox::warning(this, QString("Disk Manager"), QString("The selected device does not contain HFS, APFS or ExFAT filesystem. Can not mount the device."));
            return;
        }

        QString mount_dir = getenv("HOME") + QString(MACRO_Disk_Manager_Temp_Mount_DirName);
        QString available_dir = recon_static_functions::get_available_filename("mnt", mount_dir, Q_FUNC_INFO);


        mount_path = mount_dir + available_dir;
        QDir dir;
        dir.mkpath(mount_path);

        QString mount_exec;

        if(fs_name.contains("hfs", Qt::CaseInsensitive))
            mount_exec = "mount_hfs";
        else if(fs_name.contains("apfs", Qt::CaseInsensitive))
            mount_exec = "mount_apfs";
        else if(fs_name.contains("exfat", Qt::CaseInsensitive))
            mount_exec = "mount_exfat";
        else if(fs_name.contains("ntfs", Qt::CaseInsensitive))
            mount_exec = "mount_ntfs";
        else if(fs_name.contains("msdos", Qt::CaseInsensitive))
            mount_exec = "mount_msdos";


        QString new_device_node = device_node;

        if(!new_device_node.startsWith("/dev"))
            new_device_node = "/dev/" + new_device_node;

        QStringList args;
        struct_global_desktop_info struct_mchne_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
        QString mac_version = struct_mchne_info_obj.os_version_full;


        bool_is_running_from_diskutil = true;
        args << QString("diskutil") << QString("mount") << QString("readOnly") << new_device_node;


        QString tmp_dir = QString(global_variable_private_var_tmp_Dir_Path_Case_QString);
        if(!QFileInfo(tmp_dir).exists())
        {
            QDir dir;
            dir.mkpath(tmp_dir);
        }

        QString script_path = tmp_dir + "tmp_recon_mount.sh";

        QString script_data_str = args.join(" ");

        qint64 ret_val = recon_helper_standard_obj->pub_write_date_into_file(script_path, script_data_str, Q_FUNC_INFO);
        if(ret_val <= 0)
        {
            return;
        }

        args.clear();
        args << script_path;

        bool bool_was_arbitration_running = bool_mount_blocker_process_running;

        if(bool_was_arbitration_running && bool_is_running_from_diskutil)
        {
            pub_stop_disk_arbitration();
        }
        recon_helper_standard_obj->run_command_with_arguments("./sudoer/RECON", args, Q_FUNC_INFO);

        if(bool_was_arbitration_running && bool_is_running_from_diskutil)
        {
            pub_start_disk_arbitration();
        }
    }
}

void disk_manager::pub_mount_read_write(QString device_node)
{
    if(device_node.trimmed().isEmpty())
        return;

    QString mount_path = recon_helper_standard_obj->extract_device_mount_path_using_device_node(device_node);

    if(mount_path.trimmed() != NOMOUNT)
    {
        QMessageBox::warning(this, QString("Disk Manager"), QString("Device is already mounted"));
        return;
    }

    QString fs_name = recon_helper_standard_obj->extract_device_fs_name_using_device_node(device_node);

    if(!(fs_name.contains("hfs", Qt::CaseInsensitive) || fs_name.contains("apfs", Qt::CaseInsensitive) || fs_name.contains("exfat", Qt::CaseInsensitive)))
    {
        QMessageBox::warning(this, QString("Disk Manager"), QString("The selected device does not contain HFS, APFS or ExFAT filesystem. Can not mount the device."));
        return;
    }

    display_dialog_box_obj_non_aborted->pub_set_label_message(QString("Please wait while mounting ") + device_node + " ...");
    display_dialog_box_obj_non_aborted->show();


    QString mount_dir = QString(MACRO_Disk_Manager_Temp_Mount_DirName);
    QString available_dir = recon_static_functions::get_available_filename("mnt", mount_dir, Q_FUNC_INFO);

    mount_path = mount_dir + available_dir;
    QDir dir;
    dir.mkpath(mount_path);

    QString mount_exec;

    if(fs_name.contains("hfs", Qt::CaseInsensitive))
        mount_exec = "mount_hfs";
    else if(fs_name.contains("apfs", Qt::CaseInsensitive))
        mount_exec = "mount_apfs";
    else if(fs_name.contains("exfat", Qt::CaseInsensitive))
        mount_exec = "mount_exfat";

    QString new_device_node = device_node;

    if(!new_device_node.startsWith("/dev"))
        new_device_node = "/dev/" + new_device_node;

    QStringList args;
    args << new_device_node  << mount_path;

    recon_helper_standard_obj->run_command_with_arguments(mount_exec, args, Q_FUNC_INFO);
    //recon_helper_singular_obj->extract_all_disk_identifier();
    pub_display_data();
    //on_pushButton_refresh_clicked();
    display_dialog_box_obj_non_aborted->hide();
}

void disk_manager::pub_unmount_disk(QString device_node)
{
    if(device_node.trimmed().isEmpty())
        return;

    QString mount_path = recon_helper_standard_obj->extract_device_mount_path_using_device_node(device_node);
    if(mount_path.trimmed() == NOMOUNT)
    {
        QMessageBox::warning(this, QString("Disk Manager"), QString("Device is not mounted"));
        return;
    }

    display_dialog_box_obj_non_aborted->pub_set_label_message(QString("Unmounting ") + device_node + " ...");
    display_dialog_box_obj_non_aborted->show();

    QStringList args;
    args << "unmount" << device_node;

    recon_helper_standard_obj->run_command_with_arguments("diskutil", args, Q_FUNC_INFO);

    //recon_helper_singular_obj->extract_all_disk_identifier();
    pub_display_data();
    display_dialog_box_obj_non_aborted->hide();
}

void disk_manager::display_filevaulted_message_with_disk_name()
{
    QStringList filevalued_disk_list;
    for(int row = 0; row < ui->tableWidget_disk_list->rowCount(); row++)
    {
        QString fv_status_str = ui->tableWidget_disk_list->item(row , enum_imager_is_encrypted)->text().trimmed();
        if(fv_status_str == QString("YES"))
        {
            QString fv_dev_str = ui->tableWidget_disk_list->item(row , enum_imager_device)->text().trimmed();
            filevalued_disk_list << fv_dev_str;
        }
    }

    filevalued_disk_list.removeDuplicates();
    filevalued_disk_list.removeAll(QString(""));

    if(!filevalued_disk_list.isEmpty())
    {
        QString red_str = QString("<P><FONT COLOR='#FF0000'>") + filevalued_disk_list.join(" , ") + QString("</FONT></P>");
        QMessageBox::information(this, QString("Disk Manager"),  QString("FileVault is enabled on the following disks...") + red_str);
    }
}

void disk_manager::prepare_dialog_for_tdm_decryption_attempt()
{
    dialog_tdm_decrypt = new QDialog(this);
    dialog_tdm_decrypt->setFixedSize(QSize(400, 125));
    dialog_tdm_decrypt->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    dialog_tdm_decrypt->setModal(Qt::ApplicationModal);

    QString title_str = QString("Disk Manager") + QString(" - ") + QString("Decrypt");

    dialog_tdm_decrypt->setWindowTitle(title_str);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *main_layout = new QVBoxLayout();

    QLabel *label_attempt_title = new QLabel("Attempt Count : ", this);
    label_attempt_value = new QLabel("", this);
    label_attempt_value->setText("0");

    label_attempt_title->setFont(font_s);
    label_attempt_value->setFont(font_s);

    QSpacerItem *spacer_labels = new QSpacerItem(15,10,QSizePolicy::Expanding,QSizePolicy::Maximum);

    QLabel *label_time_elapsed_title = new QLabel("Time Elapsed : ", this);
    label_time_elapsed_value = new QLabel("", this);

    label_time_elapsed_title->setFont(font_s);
    label_time_elapsed_value->setFont(font_s);
    label_time_elapsed_value->setText("00:00");


    QHBoxLayout *layout_top_labels = new QHBoxLayout();

    layout_top_labels->addWidget(label_attempt_title);
    layout_top_labels->addWidget(label_attempt_value);
    layout_top_labels->addSpacerItem(spacer_labels);
    layout_top_labels->addWidget(label_time_elapsed_title);
    layout_top_labels->addWidget(label_time_elapsed_value);


    QHBoxLayout *layout_progress = new QHBoxLayout();

    QProgressBar *progress_bar_tdm_decrypt = new QProgressBar(this);
    progress_bar_tdm_decrypt->setMinimum(0);
    progress_bar_tdm_decrypt->setMaximum(0);

    progress_bar_tdm_decrypt->setStyleSheet("QProgressBar {\ntext-align: top;\nborder-bottom-right-radius: 0px;\n"
                                            "border-bottom-left-radius: 0px;\nbackground:#cdcdcd;\n"
                                            "color:#1a590f;\n\n}\nQProgressBar::chunk {\n\n/* background: #35b31f; */\n\n"
                                            "background: #15A9ED;\nborder-bottom-right-radius: 0px;\n"
                                            "border-bottom-left-radius: 0px;\n\n}");

    progress_bar_tdm_decrypt->setAttribute(Qt::WA_MacShowFocusRect,false);
    progress_bar_tdm_decrypt->setFixedHeight(10);

    layout_progress->addWidget(progress_bar_tdm_decrypt);


    QHBoxLayout *layout_cancel_tdm = new QHBoxLayout();

    QPushButton *pushbutton_tdm_cancel = new QPushButton("Cancel", this);
    pushbutton_tdm_cancel->setFont(font_s);
    pushbutton_tdm_cancel->setFixedWidth(80);
    pushbutton_tdm_cancel->setAttribute(Qt::WA_MacShowFocusRect,false);

    connect(pushbutton_tdm_cancel, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_tdm_cancel_clicked(bool)));

    QSpacerItem *spacer_left  = new QSpacerItem(15,10,QSizePolicy::Expanding,QSizePolicy::Maximum);
    QSpacerItem *spacer_right = new QSpacerItem(15,10,QSizePolicy::Expanding,QSizePolicy::Maximum);

    layout_cancel_tdm->addSpacerItem(spacer_left);
    layout_cancel_tdm->addWidget(pushbutton_tdm_cancel);
    layout_cancel_tdm->addSpacerItem(spacer_right);

    main_layout->addLayout(layout_top_labels);
    main_layout->addLayout(layout_progress);
    main_layout->addLayout(layout_cancel_tdm);
    main_layout->setContentsMargins(5,0,5,2);

    dialog_tdm_decrypt->setLayout(main_layout);

}

void disk_manager::slot_pushbutton_tdm_cancel_clicked(bool status)
{
    Q_UNUSED(status)
    bool_tdm_decrption_cancel_clicked = true;

    disk_manager_decrypt_disk_obj->pub_stop_decryption();

}

void disk_manager::on_pushButton_arbitration_clicked()
{
    if(bool_mount_blocker_process_running)
    {
        pub_stop_disk_arbitration();
        pub_set_disk_arbitration_label_status(QString("Turn OFF"));
    }
    else
    {
        pub_start_disk_arbitration();
        pub_set_disk_arbitration_label_status(QString("Turn ON"));
    }
}

void disk_manager::pub_set_disk_arbitration_label_status(QString status_str)
{
    ui->pushButton_arbitration->setText(status_str);
}


