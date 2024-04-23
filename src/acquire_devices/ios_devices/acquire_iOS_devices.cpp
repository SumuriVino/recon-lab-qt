#include "acquire_devices/ios_devices/acquire_iOS_devices.h"
#include "ui_acquire_iOS_devices.h"


acquire_iOS_devices::acquire_iOS_devices(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::acquire_iOS_devices)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    ui->tableWidget_properties->setColumnWidth(0, 350);

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

    proc_acquire_ios_devices = new QProcess(this);
    connect(proc_acquire_ios_devices, SIGNAL(finished(int)), SLOT(slot_proc_acquire_ios_devices_finished(int)));
    connect(proc_acquire_ios_devices, SIGNAL(readyRead()), this, SLOT(slot_proc_acquire_ios_devices_readyread()));

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


    set_gui_essentials();
}

acquire_iOS_devices::~acquire_iOS_devices()
{
    delete ui;
}

void acquire_iOS_devices::slot_stop_acquire(bool status) //This is to stop acquiring while acquisitation iOS Backup
{
    bool_device_acquired_cancelled = true;
    // proc_acquire_ios_devices->terminate();
    proc_acquire_ios_devices->kill();

}

void acquire_iOS_devices::slot_proc_acquire_ios_devices_readyread() //This will show the counter on display while acquisitation. Ready read function most of time we use to get counter & show.
{

    QByteArray arr = proc_acquire_ios_devices->readLine();

    QString str = QString::fromLocal8Bit(arr);

    if(!str.contains("]"))
        return;

    str.remove(0, str.indexOf("]") + 1);

    str.remove(str.indexOf("["), str.size());


    display_loading_progress_bar_obj->pub_set_label_messsge(str.trimmed());

}

void acquire_iOS_devices::pub_set_id_and_name(int id,QString name) //This function is not in use
{
    received_id = id;
    received_name = name;
}

void acquire_iOS_devices::closeEvent(QCloseEvent *) //when we close the iOS Acquire window, this function get calls and hide the window
{
    emit signal_attached_ios_devices_selected(QStringList());
    this->hide();

}


void acquire_iOS_devices::pub_extract_idevice_info() //When we attach iPhone, we extract all info from that like name, serial number, list of iphone detected
{                                                   // and display it on the table widget. we do this with the help of command idevice_id

    QString cmd_list_id = "./iOS_devices/idevice_id";

    QStringList args_list_id;
    args_list_id << "-l";

    key_value_map_list.clear();

    QString  list_data = recon_helper_process_obj->run_command_with_arguments(cmd_list_id, args_list_id, Q_FUNC_INFO);

    if(list_data.trimmed() == QString(""))
    {
        ui->label_message->setText("No device found, Please attach a device");
        return;
    }

    QStringList device_id_list = list_data.split("\n", Qt::SkipEmptyParts);

    device_id_list.removeDuplicates();

    for(int uuid = 0; uuid < device_id_list.size(); uuid++)
    {
        QString cmd = "./iOS_devices/ideviceinfo";

        QStringList args_list_uuid;
        args_list_uuid << "-u" << device_id_list.at(uuid);

        QMap<QString, QString> key_value_map;

        QString proc_out_data_string  = recon_helper_process_obj->run_command_with_arguments(cmd, args_list_uuid, Q_FUNC_INFO);

        QStringList m_list = proc_out_data_string.split("\n");

        for(int i = 0; i < m_list.size(); i++)
        {
            QString line = m_list.at(i);

            QStringList key_value_list = line.split(":");

            if(key_value_list.size() != 2)
                continue;

            key_value_map.insert(key_value_list.at(0), key_value_list.at(1));
        }

        key_value_map_list.append(key_value_map);
    }

    prepare_display();

    set_gui_essentials();
}

void acquire_iOS_devices::prepare_display() // Display list of iphones, their serial number and other info detected on the table widget so user can select which one he wants to take backup of.
{

    ui->tableWidget_device_list->setRowCount(0);

    int row;

    for(int i = 0; i < key_value_map_list.size(); i++)
    {

        row = i;

        ui->tableWidget_device_list->insertRow(row);
        ui->tableWidget_device_list->setRowHeight(row, 22);

        QString device_class, device_name, device_serial, device_phone, device_uuid;
        QTableWidgetItem *item_bookmark, *item_sr, *item_device_class, *item_device_name, *item_device_serial, *item_device_phone, *item_device_uuid, *item_blank;

        QMap<QString, QString> key_value_map;
        key_value_map = key_value_map_list.at(i);

        QMapIterator<QString, QString> iter(key_value_map);

        while (iter.hasNext()) {
            iter.next();

            if(iter.key() == "DeviceName")
                device_name = iter.value();

            if(iter.key() == "DeviceClass")
                device_class = iter.value();

            if(iter.key() == "PhoneNumber")
                device_phone = iter.value();

            if(iter.key() == "SerialNumber")
                device_serial = iter.value();

            if(iter.key() == "UniqueDeviceID")
                device_uuid = iter.value();

        }

        item_bookmark = new QTableWidgetItem;
        item_bookmark->setCheckState(Qt::Unchecked);
        item_sr = new QTableWidgetItem(QString::number(row + 1));
        item_device_class = new QTableWidgetItem(device_class);
        item_device_name = new QTableWidgetItem(device_name);
        item_device_serial = new QTableWidgetItem(device_serial);
        item_device_phone = new QTableWidgetItem(device_phone);
        item_device_uuid = new QTableWidgetItem(device_uuid);
        item_blank = new QTableWidgetItem;


        ui->tableWidget_device_list->setItem(row, enum_idevice_list_bookmark, item_bookmark);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_sr_no, item_sr);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_device_class, item_device_class);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_device_name, item_device_name);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_device_phone, item_device_phone);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_device_serial, item_device_serial);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_device_uuid, item_device_uuid);
        ui->tableWidget_device_list->setItem(row, enum_idevice_list_blank, item_blank);

        ui->tableWidget_device_list->resizeColumnsToContents();
    }

}

void acquire_iOS_devices::set_gui_essentials() //This is columns order of iphone info's.
{
    ui->tableWidget_device_list->hideColumn(enum_idevice_list_bookmark);
    ui->tableWidget_device_list->setColumnWidth(enum_idevice_list_sr_no, 35);
    ui->tableWidget_device_list->setColumnWidth(enum_idevice_list_device_name, 150);
    ui->tableWidget_device_list->setColumnWidth(enum_idevice_list_device_phone, 150);
    ui->tableWidget_device_list->setColumnWidth(enum_idevice_list_device_serial, 150);
    ui->tableWidget_device_list->setColumnWidth(enum_idevice_list_device_uuid, 290);
}

void acquire_iOS_devices::on_tableWidget_device_list_cellClicked(int row_number, int column) //Here we have list of iPhones and we output here, on which iphone user clicked
{                                                                                   // and which one to take backup according to user.

    if(row_number < 0)
        return;

    ui->tableWidget_properties->setRowCount(0);

    QMap<QString, QString> key_value_map;
    key_value_map = key_value_map_list.at(row_number);

    QMapIterator<QString, QString> iter(key_value_map);

    int row = 0;
    while (iter.hasNext()) {
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

void acquire_iOS_devices::on_pushButton_refresh_list_clicked() //This is button to refresh the detected list of iphones which displayed on tablewidget
{
    ui->label_message->clear();
    ui->tableWidget_device_list->setRowCount(0);
    ui->tableWidget_properties->setRowCount(0);

    pub_extract_idevice_info();
}


void acquire_iOS_devices::on_pushButton_back_clicked() // When we click on back button, signal emitted and send us back to the previous window
{
    emit signal_bring_recon_lab_banner_page_in_front();
}

void acquire_iOS_devices::slot_proc_acquire_ios_devices_finished(int status) //When acquisitation of backup finished, and display message accordingly if it success or failed
{

    display_loading_progress_bar_obj->hide();

    QString m_status, m_message;
    if(bool_device_acquired_cancelled)
    {
        m_status = "Aborted";
        m_message = "Device acquire process has been cancelled.";
    }
    else if(status)
    {
        m_status = "Failed";
        m_message = "Device acquire process has failed.";
    }
    else
    {
        m_status = "Successful";
        m_message = "     Device has been acquired successfully.\n                Do you want to open it?";
    }


    QFile mfile(destination_location + QString("acquire_logs.txt"));
    if(!mfile.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("File Open ---FAILED---: "  + mfile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + mfile.errorString() , Q_FUNC_INFO);
        // return;
    }
    QString m_data;
    int cur_row = ui->tableWidget_device_list->currentRow();
    m_data.append("\n");
    m_data.append("Device Name :").append(ui->tableWidget_device_list->item(cur_row, enum_idevice_list_device_name)->text()).append("\n");
    m_data.append("Device Class :").append(ui->tableWidget_device_list->item(cur_row, enum_idevice_list_device_class)->text()).append("\n");
    m_data.append("Phone Number :").append(ui->tableWidget_device_list->item(cur_row, enum_idevice_list_device_phone)->text()).append("\n");
    m_data.append("Serial Number:").append(ui->tableWidget_device_list->item(cur_row, enum_idevice_list_device_serial)->text()).append("\n");
    m_data.append("Device UUID :").append(ui->tableWidget_device_list->item(cur_row, enum_idevice_list_device_uuid)->text()).append("\n");
    m_data.append("Start Timestamp(UTC) :").append(acquiring_start_time_string).append("\n");
    m_data.append("End Timestamp(UTC) :").append(QDateTime::currentDateTimeUtc().toString()).append("\n");
    m_data.append("Acquiring Status :").append(m_status).append("\n");
    m_data.append("********** End of Report **********");
    mfile.write(m_data.toLocal8Bit());
    mfile.flush();
    mfile.close();




    if(m_status == "Successful")
    {
        report_status obj;
        obj.set_label(m_message);
        if(obj.exec())
        {
            QStringList arg;
            arg << destination_location;

            QProcess *myProcess = new QProcess(this);
            myProcess->startDetached("open",arg);
        }

    }
    else
    {

        QMessageBox::information(this, "Info", m_message);

    }

}



void acquire_iOS_devices::on_pushButton_acquire_clicked() // when we click on acuire button to take backup of iphone, idevicebackup2 command got executed here with some arguments
{                                                       // and display message accordingly

    if(ui->tableWidget_device_list->rowCount() < 1)
    {
        QMessageBox::warning(this, "Error", "Click 'Refresh' to list all connected iOS devices.");
        return;
    }

    if(ui->tableWidget_device_list->currentRow() < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a device from the list.");
        return;
    }

    // destination_location = QFileDialog::getExistingDirectory(this, "Output Directory", getenv("HOME"));
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
    if(destination_location.trimmed() == QString(""))
        return;


    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_obj->show();

    acquiring_start_time_string = QDateTime::currentDateTimeUtc().toString();

    // ./idevicebackup2 backup --full -s 533aa1bbebe6ea093c45877280afddb18da0c972  ~/tmp

    QString uuid = ui->tableWidget_device_list->item(ui->tableWidget_device_list->currentRow(), enum_idevice_list_device_uuid)->text().trimmed();
    //destination_location += ("/" + uuid + "/");


    QString bk_dir_path = recon_static_functions::get_available_filename(uuid, destination_location, Q_FUNC_INFO);
    destination_location += ("/" + bk_dir_path + "/");


    QDir mdir;
    mdir.mkpath(destination_location);

    QString cmd = "./iOS_devices/idevicebackup2";


    QStringList args;
    args << "backup" << "--full" << "-s" << uuid << destination_location;

    bool_device_acquired_cancelled = false;

    proc_acquire_ios_devices->start(cmd, args);
    if (!proc_acquire_ios_devices->waitForStarted())
    {
        recon_static_functions::app_debug("iOS acquiring  ---FAILED----"+ proc_acquire_ios_devices->errorString(),Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }


    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(bool_device_acquired_cancelled || proc_acquire_ios_devices->state() != QProcess::Running)
        {
            break;
        }
    }


}
