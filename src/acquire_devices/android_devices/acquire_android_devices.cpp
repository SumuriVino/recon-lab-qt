#include "acquire_android_devices.h"
#include "ui_acquire_android_devices.h"

/*
 This Class is all about Acquiring the android backup, We have disabled this function from recon lab because it starts failing for lot of companies mobile,
as every company has different security features. So, This needs research and only here if we can optimise it or not
 */

acquire_android_devices::acquire_android_devices(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::acquire_android_devices)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    ui->tableWidget_properties->setColumnWidth(0, 350);

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    connect(display_loading_progress_bar_obj, SIGNAL(signal_close_loading_display_dialog(bool)), this, SLOT(slot_stop_acquire(bool)));

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


    M_VAR_DEVICE_INFO_QString_model_QString = "Model";
    M_VAR_DEVICE_INFO_QString_manufacturer_QString = "manufacturer";
    M_VAR_DEVICE_INFO_QString_brand_QString = "Brand";
    M_VAR_DEVICE_INFO_QString_iccid_QString = "ICCID";
    M_VAR_DEVICE_INFO_QString_imei1_QString = "IMEI1";
    M_VAR_DEVICE_INFO_QString_imei2_QString = "IMEI2";
    M_VAR_DEVICE_INFO_QString_serial_no_QString = "Serial No";
    M_VAR_DEVICE_INFO_QString_sys_timezone_QString = "Time Zone";


    proc_acquire_making_abfile = new QProcess(this);
    connect(proc_acquire_making_abfile, SIGNAL(finished(int)), SLOT(slot_proc_acquire_making_abfile_finished(int)));
    connect(proc_acquire_making_abfile, SIGNAL(readyRead()), this, SLOT(slot_proc_acquire_making_abfile_readyread()));

    proc_acquire_making_ab_to_tar = new QProcess(this);
    connect(proc_acquire_making_ab_to_tar, SIGNAL(finished(int)), SLOT(slot_proc_acquire_making_ab_to_tar_finished(int)));
    connect(proc_acquire_making_ab_to_tar, SIGNAL(readyRead()), this, SLOT(slot_proc_acquire_making_ab_to_tar_readyread()));

    proc_acquire_decompress_tar = new QProcess(this);
    connect(proc_acquire_decompress_tar, SIGNAL(finished(int)), SLOT(slot_proc_acquire_decompress_tar_finished(int)));
    connect(proc_acquire_decompress_tar, SIGNAL(readyRead()), this, SLOT(slot_proc_acquire_decompress_tar_readyread()));

}

acquire_android_devices::~acquire_android_devices()
{
    delete ui;
}



void acquire_android_devices::closeEvent(QCloseEvent *)
{

}

void acquire_android_devices::on_tableWidget_device_list_cellClicked(int row_number, int column)
{

    if(row_number < 0)
        return;

    ui->tableWidget_properties->setRowCount(0);

    QMap<QString, QString> key_value_map;
    key_value_map = key_value_map_list.at(row_number);

    QMapIterator<QString, QString> iter(key_value_map);

    int row = 0;
    while (iter.hasNext())
    {
        iter.next();

        ui->tableWidget_properties->insertRow(row);
        ui->tableWidget_properties->setRowHeight(row, 22);

        QTableWidgetItem *item_key = new QTableWidgetItem(iter.key());
        item_key->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QTableWidgetItem *item_value = new QTableWidgetItem(iter.value());

        ui->tableWidget_properties->setItem(row, 0, item_key);

        ui->tableWidget_properties->setItem(row, 1, item_value);

        row++;

    }

}

void acquire_android_devices::slot_stop_acquire(bool status)
{
    bool_device_acquired_cancelled = true;
    display_loading_progress_bar_obj->hide();
}

void acquire_android_devices::extract_device_info()
{

    recon_static_functions::app_debug("start", Q_FUNC_INFO);

    key_value_map_list.clear();


    QStringList device_id_list = extract_device_srl_no_list();

    if(device_id_list.size() <= 0)
    {
        recon_static_functions::app_debug("---FAILED--- No android device found", Q_FUNC_INFO);
        return;
    }

    QString cmd_list_id = "./android_devices/adb";

    for(int pp = 0; pp < device_id_list.size(); pp++)
    {
        QStringList args_list_id;
        args_list_id << "-s" << device_id_list.at(pp)  << "shell" << "getprop";

        QString  list_data = recon_helper_process_obj->run_command_with_arguments(cmd_list_id, args_list_id, Q_FUNC_INFO);

        QStringList single_device_attr_list = list_data.split("\n", Qt::SkipEmptyParts);

        QMap<QString, QString> key_value_map;
        for(int uuid = 0; uuid < single_device_attr_list.size(); uuid++)
        {
            QString ones_info = single_device_attr_list.at(uuid).trimmed();
            QStringList list_first = ones_info.split(": [");
            if(list_first.size() != 2)
                continue;

            QString m_key, m_value;

            QString part_1 = list_first.at(0).trimmed();
            QString part_2 = list_first.at(1).trimmed();
            if(part_2.endsWith("]"))
                part_2.chop(QString("]").size());

            bool bool_related_info_found = false;

            if(part_1 == ("[ro.product.model]"))
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_model_QString;
                m_value = part_2;
            }
            else if(part_1 == "[ro.product.manufacturer]")
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_manufacturer_QString;
                m_value = part_2;
            }
            else if(part_1 == "[ro.product.brand]")
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_brand_QString;
                m_value = part_2;
            }
            else if(part_1 == "[persist.radio.iccid]")
            {
                // [persist.radio.iccid]: [89911100001033298059,<null>]

                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_iccid_QString;

                //            part_2.remove(",<null>");
                //            part_2.remove("<null>");

                m_value = part_2.trimmed();
            }
            else if(part_1 == "[ro.ril.oem.imei1]")
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_imei1_QString;
                m_value = part_2;
            }
            else if(part_1 == "[ro.ril.oem.imei2]")
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_imei2_QString;
                m_value = part_2;
            }
            else if(part_1 == "[ro.serialno]")
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_serial_no_QString;
                m_value = part_2;
            }
            else if(part_1 == "[persist.sys.timezone]")
            {
                bool_related_info_found = true;
                m_key = M_VAR_DEVICE_INFO_QString_sys_timezone_QString;
                m_value = part_2;
            }

            if(bool_related_info_found)
            {
                key_value_map.insert(m_key, m_value);

            }

        }

        key_value_map_list.append(key_value_map);
    }

    display_data();

    recon_static_functions::app_debug("end", Q_FUNC_INFO);




    //    recon_static_functions::app_debug("start", Q_FUNC_INFO);

    //    QString cmd_list_id = "./android_devices/adb";
    //    QStringList args_list_id;
    //    args_list_id << "devices" << "-l";

    //    key_value_map_list.clear();

    //    QString  list_data = recon_helper_process_obj->run_command_with_arguments(cmd_list_id, args_list_id, Q_FUNC_INFO);
    //    QStringList device_id_list = list_data.split("\n", Qt::SkipEmptyParts);

    //    device_id_list.removeOne("List of devices attached");

    //    //List of devices attached
    //    //17185cba7cf4           device usb:4194304X product:rolex model:Redmi_4A device:rolex transport_id:9

    //    for(int uuid = 0; uuid < device_id_list.size(); uuid++)
    //    {
    //        QString ones_info = device_id_list.at(uuid).trimmed();
    //        QStringList list_first = ones_info.split("   device ");

    //        if(list_first.size() != 2)
    //            continue;


    //        QString part_1 = list_first.at(0).trimmed();
    //        QString part_2 = list_first.at(1).trimmed();


    //        QStringList m_list = part_2.split(" ");
    //        QMap<QString, QString> key_value_map;

    //        for(int i = 0; i < m_list.size(); i++)
    //        {
    //            QString line = m_list.at(i);

    //            QStringList key_value_list = line.split(":");

    //            if(key_value_list.size() != 2)
    //                continue;

    //            key_value_map.insert(key_value_list.at(0), key_value_list.at(1));
    //        }
    //        key_value_map.insert("serial", part_1);
    //        key_value_map_list.append(key_value_map);
    //    }

    //    display_data();

    //    recon_static_functions::app_debug("end", Q_FUNC_INFO);

}

void acquire_android_devices::display_data()
{
    recon_static_functions::app_debug("start", Q_FUNC_INFO);


    ui->tableWidget_device_list->setRowCount(0);

    int row;

    for(int i = 0; i < key_value_map_list.size(); i++)
    {

        row = i;

        ui->tableWidget_device_list->insertRow(row);
        ui->tableWidget_device_list->setRowHeight(row, 22);


        QString m_model, m_manufctr, m_brand, m_serial;
        QTableWidgetItem *item_model, *item_manuftr, *item_brand, *item_serial_no, *item_blank;

        QMap<QString, QString> key_value_map;
        key_value_map = key_value_map_list.at(i);

        QMapIterator<QString, QString> iter(key_value_map);

        while (iter.hasNext())
        {
            iter.next();

            if(iter.key() == M_VAR_DEVICE_INFO_QString_serial_no_QString)
                m_serial = iter.value();

            if(iter.key() == M_VAR_DEVICE_INFO_QString_manufacturer_QString)
                m_manufctr = iter.value();

            if(iter.key() == M_VAR_DEVICE_INFO_QString_model_QString)
                m_model = iter.value();

            if(iter.key() == M_VAR_DEVICE_INFO_QString_brand_QString)
                m_brand = iter.value();

        }

        item_model = new QTableWidgetItem(m_model);
        item_manuftr = new QTableWidgetItem(m_manufctr);
        item_brand = new QTableWidgetItem(m_brand);
        item_serial_no = new QTableWidgetItem(m_serial);
        item_blank = new QTableWidgetItem;



        ui->tableWidget_device_list->setItem(row, enum_andrd_device_model_name, item_model);
        ui->tableWidget_device_list->setItem(row, enum_andrd_device_manufacturer, item_manuftr);
        ui->tableWidget_device_list->setItem(row, enum_andrd_device_brand, item_brand);
        ui->tableWidget_device_list->setItem(row, enum_andrd_device_serial_no, item_serial_no);
        ui->tableWidget_device_list->setItem(row, enum_andrd_device_blank, item_blank);

        //ui->tableWidget_device_list->resizeColumnsToContents();
    }

    recon_static_functions::app_debug("end", Q_FUNC_INFO);


}

QStringList acquire_android_devices::extract_device_srl_no_list()
{
    recon_static_functions::app_debug("start", Q_FUNC_INFO);

    // List of devices attached
    // 17185cba7cf4	device
    // 35df4ppo	device


    QString cmd_list_id = "./android_devices/adb";

    QString  list_data = recon_helper_process_obj->run_command_with_arguments(cmd_list_id, QStringList("devices"), Q_FUNC_INFO);

    QStringList device_id_list = list_data.split("\n", Qt::SkipEmptyParts);

    device_id_list.removeOne("List of devices attached");


    QStringList mmlist;
    for(int uuid = 0; uuid < device_id_list.size(); uuid++)
    {
        QString ones_info = device_id_list.at(uuid).trimmed();
        QStringList list_first = ones_info.split("\t");

        if(list_first.size() != 2)
            continue;


        mmlist << list_first.at(0).trimmed();
    }


    recon_static_functions::app_debug("end", Q_FUNC_INFO);

    return mmlist;
}


void acquire_android_devices::on_pushButton_refresh_list_clicked()
{
    ui->label_message->clear();
    ui->tableWidget_device_list->setRowCount(0);
    ui->tableWidget_properties->setRowCount(0);

    extract_device_info();

}

void acquire_android_devices::on_pushButton_acquire_clicked()
{
    if(ui->tableWidget_device_list->rowCount() < 1)
    {
        QMessageBox::warning(this, "Error", "Click 'Refresh' to list connected android devices.");
        return;
    }

    if(ui->tableWidget_device_list->currentRow() < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a device from the list.");
        return;
    }


    destination_location.clear();

    file_dialog_obj.setFileMode(QFileDialog::Directory);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            destination_location = selected_files.at(0);
        }
    }
    if(destination_location.trimmed().isEmpty())
        return;

    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    bool_device_acquired_cancelled = false;

    display_loading_progress_bar_obj->show();

    QString serial_no =  ui->tableWidget_device_list->item(ui->tableWidget_device_list->currentRow(), enum_andrd_device_serial_no)->text();
    QString cdt = QDateTime::currentDateTimeUtc().toString("MMM-dd-yyyy-hh-mm-ss");
    QString fnl_dir = recon_static_functions::get_available_filename((serial_no+"-"+ cdt), destination_location, Q_FUNC_INFO);
    destination_location = destination_location + "/" + fnl_dir + "/";
    QDir mdir;
    mdir.mkpath(destination_location);

    acquiring_start_time_string = QDateTime::currentDateTimeUtc().toString("MMM-dd-yyyy hh:mm:ss") + " +00:00";


    recon_helper_process_obj->run_command_with_arguments("./android_devices/adb", QStringList("kill-server"), Q_FUNC_INFO);
    recon_helper_process_obj->run_command_with_arguments("./android_devices/adb", QStringList("start-server"), Q_FUNC_INFO);

    QString ab_file = making_ab_file();
    if(!bool_acquire_making_abfile_sucess || bool_device_acquired_cancelled)
    {
        recon_static_functions::app_debug("----FAILED---- making ab file",Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }
    recon_static_functions::app_debug("crossed making ab file",Q_FUNC_INFO);

    QString tar_file = making_ab_to_tar(ab_file);
    if(!bool_acquire_making_ab_to_tar_sucess || bool_device_acquired_cancelled)
    {
        recon_static_functions::app_debug("----FAILED---- making ab to tar ",Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }
    recon_static_functions::app_debug("crossed making ab to tar",Q_FUNC_INFO);

    if(bool_device_acquired_cancelled)
    {
        recon_static_functions::app_debug("----FAILED---- aborted",Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }


    acquire_decompress_tar(tar_file);
    if(!bool_acquire_decompress_tar_sucess || bool_device_acquired_cancelled)
    {
        recon_static_functions::app_debug("----FAILED---- tar decompression",Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }

    display_loading_progress_bar_obj->hide();

    QMessageBox::information(this, "Info", "Device has been acquired successfully.");

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

}

QString acquire_android_devices::making_ab_file()
{
    //./adb backup -apk -shared -all -f ~/tmp/1/backup.ab


    QString serial_no = ui->tableWidget_device_list->item(ui->tableWidget_device_list->currentRow(), enum_andrd_device_serial_no)->text();
//    QString of_file = recon_static_functions::get_available_filename("backup.ab", MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
//    of_file = MACRO_RECON_Temp_DirPath_In_Bundle + of_file;

    QString of_file = recon_static_functions::get_available_filename("backup.ab", global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    of_file = global_variable_private_var_tmp_Dir_Path_Case_QString + of_file;

    QString cmd = "./android_devices/adb";
    QStringList arg_list;
    arg_list << "-s" << serial_no << "backup" << "-apk" << "-shared" << "-all" << "-f" << of_file;

    bool_acquire_making_abfile_sucess = false;

    proc_acquire_making_abfile->start(cmd, arg_list);
    if (!proc_acquire_making_abfile->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + proc_acquire_making_abfile->errorString() ,Q_FUNC_INFO);
        return "" ;
    }

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(bool_acquire_making_abfile_sucess || proc_acquire_making_abfile->state() != QProcess::Running)
        {
            break;
        }

        if(bool_device_acquired_cancelled)
        {
            if(proc_acquire_making_abfile->state() == QProcess::Running)
                proc_acquire_making_abfile->terminate();

            break;
        }
    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);


    return of_file;
}

QString acquire_android_devices::making_ab_to_tar(QString ab_filepath)
{
    //sudo java -jar abe.jar unpack backup1.ab backup1.tar

//    QString of_file = recon_static_functions::get_available_filename("backup.tar", MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
//    of_file = MACRO_RECON_Temp_DirPath_In_Bundle + of_file;
    QString of_file = recon_static_functions::get_available_filename("backup.tar", global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    of_file = global_variable_private_var_tmp_Dir_Path_Case_QString + of_file;


    QString cmd = "java";
    QStringList arg_list;
    arg_list << "-jar" << "./android_devices/abe.jar" << "unpack" << ab_filepath << of_file;


    bool_acquire_making_ab_to_tar_sucess = false;

    proc_acquire_making_ab_to_tar->start(cmd, arg_list);
    if(!proc_acquire_making_ab_to_tar->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + proc_acquire_making_ab_to_tar->errorString() ,Q_FUNC_INFO);
        return "";
    }

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(bool_acquire_making_ab_to_tar_sucess || proc_acquire_making_ab_to_tar->state() != QProcess::Running)
        {
            break;
        }

        if(bool_device_acquired_cancelled)
        {
            if(proc_acquire_making_ab_to_tar->state() == QProcess::Running)
                proc_acquire_making_ab_to_tar->terminate();

            break;
        }

    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);


    return of_file;

}

void acquire_android_devices::acquire_decompress_tar(QString tar_filepath)
{
    // tar -xvf ~/tmp/1/backup1.tar -C ~/tmp/2/

    QString serial_no =  ui->tableWidget_device_list->item(ui->tableWidget_device_list->currentRow(), enum_andrd_device_serial_no)->text();
    //    QString cdt = QDateTime::currentDateTimeUtc().toString("MMM-dd-yyyy-hh-mm-ss");
    //    QString fnl_dir = recon_static_functions::get_available_filename((serial_no+"-"+ cdt), destination_location, Q_FUNC_INFO);
    //    destination_location = destination_location + "/" + fnl_dir + "/";
    QDir mdir;
    //    mdir.mkpath(destination_location);

    QString destin_path = destination_location + serial_no;
    mdir.mkpath(destin_path);

//    QString of_file = recon_static_functions::get_available_filename("backup.ab", MACRO_RECON_Temp_DirPath_In_Bundle, Q_FUNC_INFO);
//    of_file = MACRO_RECON_Temp_DirPath_In_Bundle + of_file;

    QString of_file = recon_static_functions::get_available_filename("backup.ab", global_variable_private_var_tmp_Dir_Path_Case_QString, Q_FUNC_INFO);
    of_file = global_variable_private_var_tmp_Dir_Path_Case_QString + of_file;

    QString cmd = "tar";
    QStringList arg_list;
    arg_list << "-xvf" << tar_filepath << "-C" << destin_path;

    bool_acquire_decompress_tar_sucess = false;

    proc_acquire_decompress_tar->start(cmd, arg_list);
    if (!proc_acquire_decompress_tar->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + proc_acquire_decompress_tar->errorString() ,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(bool_acquire_decompress_tar_sucess || proc_acquire_decompress_tar->state() != QProcess::Running)
        {
            break;
        }
        if(bool_device_acquired_cancelled)
        {
            if(proc_acquire_decompress_tar->state() == QProcess::Running)
                proc_acquire_decompress_tar->terminate();

            break;
        }

    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);


    return;

}

void acquire_android_devices::slot_proc_acquire_making_abfile_finished(int)
{
    bool_acquire_making_abfile_sucess= true;
}

void acquire_android_devices::slot_proc_acquire_making_abfile_readyread()
{
    QByteArray arr = proc_acquire_making_abfile->readLine();

    QString str = QString::fromLocal8Bit(arr);

    //    if(!str.contains("]"))
    //        return;

    //    str.remove(0, str.indexOf("]") + 1);

    //    str.remove(str.indexOf("["), str.size());


    //    display_loading_progress_bar_obj->pub_set_label_messsge(str.trimmed());

}

void acquire_android_devices::slot_proc_acquire_making_ab_to_tar_finished(int)
{
    bool_acquire_making_ab_to_tar_sucess = true;
}

void acquire_android_devices::slot_proc_acquire_making_ab_to_tar_readyread()
{
    QByteArray arr = proc_acquire_making_ab_to_tar->readLine();

    QString str = QString::fromLocal8Bit(arr);

    //    if(!str.contains("]"))
    //        return;

    //    str.remove(0, str.indexOf("]") + 1);

    //    str.remove(str.indexOf("["), str.size());


    //    display_loading_progress_bar_obj->pub_set_label_messsge(str.trimmed());

}


void acquire_android_devices::slot_proc_acquire_decompress_tar_finished(int)
{
    bool_acquire_decompress_tar_sucess= true;

    QFile mfile(destination_location + QString("acquire_logs.txt"));
    if(mfile.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("File Open ---FAILED---: "  + mfile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + mfile.errorString() , Q_FUNC_INFO);
    }

    QString m_data;
    int cur_row = ui->tableWidget_device_list->currentRow();
    m_data.append("\n");
    m_data.append("Model Name :").append(ui->tableWidget_device_list->item(cur_row, enum_andrd_device_model_name)->text()).append("\n");
    m_data.append("Manufacturer:").append(ui->tableWidget_device_list->item(cur_row, enum_andrd_device_manufacturer)->text()).append("\n");
    m_data.append("Brand :").append(ui->tableWidget_device_list->item(cur_row, enum_andrd_device_brand)->text()).append("\n");
    m_data.append("Serial Number :").append(ui->tableWidget_device_list->item(cur_row, enum_andrd_device_serial_no)->text()).append("\n");
    m_data.append("Start Timestamp :").append(acquiring_start_time_string).append("\n");
    m_data.append("End Timestamp :").append(QDateTime::currentDateTimeUtc().toString("MMM-dd-yyyy hh:mm:ss") + " +00:00").append("\n");
    //  m_data.append("Acquiring Status :").append(m_status).append("\n");
    m_data.append("********** End of Report **********");
    mfile.write(m_data.toLocal8Bit());
    mfile.flush();
    mfile.close();



    // QMessageBox::information(this, "Info", m_message);

}

void acquire_android_devices::slot_proc_acquire_decompress_tar_readyread()
{
    QByteArray arr = proc_acquire_decompress_tar->readLine();

    QString str = QString::fromLocal8Bit(arr);

    //    if(!str.contains("]"))
    //        return;

    //    str.remove(0, str.indexOf("]") + 1);

    //    str.remove(str.indexOf("["), str.size());


    //    display_loading_progress_bar_obj->pub_set_label_messsge(str.trimmed());

}

void acquire_android_devices::on_pushButton_back_clicked()
{
    emit signal_bring_recon_lab_banner_page_in_front();

}
