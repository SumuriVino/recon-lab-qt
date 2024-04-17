#include "z_mainwindow/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::lic_validate_license_integerity()
{

    lic_integerity_failed = false;

    QFile file(recon_helper_standard_obj->pub_get_app_license_path());

    if(!file.open(QIODevice::ReadOnly))
    {
        lic_file_not_exists = true;
        return;
    }

    QString master_hash = file.readLine().trimmed();
    QString purchase_date_hash = file.readLine().trimmed();


    QString usb_line = file.readLine().trimmed();

    QString expiry_date_hash = file.readLine().trimmed();

    customer_name = file.readLine().trimmed();

    license_set_purchase_expiry_date_hash(purchase_date_hash,expiry_date_hash);

    QStringList module_hash_list;

    while(!file.atEnd())
    {
        module_hash_list << file.readLine().trimmed();
    }
    license_set_module_hash_list(module_hash_list);

    QString file_content_combined;

    file_content_combined.append(purchase_date_hash);
    file_content_combined.append(usb_line);
    file_content_combined.append(expiry_date_hash);
    file_content_combined.append(customer_name);
    for(int i = 0; i < module_hash_list.size(); i++)
    {
        file_content_combined.append(module_hash_list.at(i)); // combined all hashes of modules

    }
    QByteArray file_content_hash = QCryptographicHash::hash(file_content_combined.toUtf8(),QCryptographicHash::Sha1);

    if(file_content_hash.toHex() != master_hash)
    {
        lic_integerity_failed = true;
        return;
    }

}
void MainWindow::create_usb_device_list()
{

    usb_device_serial_list usb_l;
    usb_l.update_usb_list();

    usb_device_list.clear();
    usb_device_list = usb_l.usb_serial_number_list();

}

void MainWindow::lic_validate_connected_usb_device()
{

    licensed_usb_present = false;

    QString ver = "EverGreen.2018.July.01";
    ver.append("#@$@$@## $@vj fdj v43%  $#%4$^^$$^%");

    QFile file(recon_helper_standard_obj->pub_get_app_license_path());

    if(!file.open(QIODevice::ReadOnly))
    {
        //        recon_static_functions::app_debug(" file open ---FAILED--- license file",Q_FUNC_INFO);
        //        recon_static_functions::app_debug(" error " + file.errorString(),Q_FUNC_INFO);

        lic_file_not_exists = true;

        licensed_usb_present = false;
        return;
    }

    QString master_hash = file.readLine().trimmed();
    purchase_date_hash = file.readLine().trimmed();

    QString usb_line = file.readLine().trimmed();

    expiry_date_hash = file.readLine().trimmed();


    QString magic_key = generate_magic_key(1);
    QByteArray magic_key_hash = QCryptographicHash::hash(magic_key.toUtf8(),QCryptographicHash::Md5);

    usb_device_serial_list l1;
    l1.update_usb_list();
    usb_device_list.clear();
    usb_device_list = l1.usb_serial_number_list();


    licensed_usb_device = "";
    for(int i = 0; i < usb_device_list.size(); i++)
    {
        QString usb_serial = usb_device_list.at(i);

        QString usb_string_for_hash = usb_serial + magic_key_hash.toHex() + ver;
        QByteArray usb_hash = QCryptographicHash::hash(usb_string_for_hash.toUtf8(),QCryptographicHash::Sha1);


        if(usb_hash.toHex() == usb_line.trimmed())
        {
            licensed_usb_present = true;
            licensed_usb_device = usb_serial;
            poller = usb_serial;

            break;
        }

    }

}

void MainWindow::lic_create_module_hash_list()
{

    licensed_usb_present = false;

    QString ver = "EverGreen.2018.July.01";
    ver.append("#@$@$@## $@vj fdj v43%  $#%4$^^$$^%");

    QFile file(recon_helper_standard_obj->pub_get_app_license_path());

    if(!file.open(QIODevice::ReadOnly))
    {

        lic_file_not_exists = true;

        licensed_usb_present = false;
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"License file missing !!!");
        message_dialog_object->show();
        return;
    }

    QString master_hash = file.readLine().trimmed();
    QString purchase_date_hash = file.readLine().trimmed();

    QString usb_line = file.readLine().trimmed();
    QString expiry_date_hash = file.readLine().trimmed();

    license_module_hash_list.clear();
    while(!file.atEnd())
    {
        license_module_hash_list << file.readLine().trimmed();
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}
void MainWindow::extract_purchase_expiry_date()
{

    QString usb_ser = licensed_usb_device;
    license_purchase_date_numeric_qint64 = 0;
    license_expiry_date_numeric_qint64 = 0;


    QString recon_ver = "EverGreen.2018.July.01";
    recon_ver.append("#@$@$@## $@vj fdj v43%  $#%4$^^$$^%");

    QDateTime current_dt = QDateTime::fromMSecsSinceEpoch(1359308916000); // Sun, 27 Jan 2013


    QDateTime mdt = current_dt.toUTC();
    QTime m_time;
    m_time.setHMS(0,0,0);
    mdt.setTime(m_time);


    qint64 current_epch = mdt.toSecsSinceEpoch();
    current_epch = current_epch + (180*24*60*60);   // 1374796800 Fri, 26 Jul 2013 00:00:00 GMT

    qint64 highest_epch = current_epch + (5000*24*60*60); // 1806796800 Sun, 04 Apr 2027 00:00:00 GMT

    QString magic_key = generate_magic_key(1);
    QByteArray magic_key_hash = QCryptographicHash::hash(magic_key.toUtf8(),QCryptographicHash::Md5);


    for(qint64 k = current_epch; k < highest_epch + 1; k+=(24*60*60) )
    {
        QString pd = QString::number(k);

        QString raw_data = pd + usb_ser + magic_key_hash.toHex()  + recon_ver;

        QByteArray  pd_hash = QCryptographicHash::hash(raw_data.toUtf8(),QCryptographicHash::Sha1);

        if(pd_hash.toHex() == purchase_date_hash)
        {
            license_purchase_date_numeric_qint64 = k;
            break;
        }

    }

    qint64 pp_dt = license_purchase_date_numeric_qint64;


    for(qint64 k = pp_dt; k < (pp_dt + (365*10*24*60*60 + 50)); k+=(24*60*60) )
    {
        QString ed = QString::number(k);


        QString raw_data = ed + usb_ser + magic_key_hash.toHex()  + recon_ver;

        QByteArray  ed_hash = QCryptographicHash::hash(raw_data.toUtf8(),QCryptographicHash::Sha1);

        if(ed_hash.toHex() == expiry_date_hash)
        {
            license_expiry_date_numeric_qint64 = k;
            break;
        }

    }

    if((license_expiry_date_numeric_qint64 - license_purchase_date_numeric_qint64)/(24*60*60) < 100 && license_expiry_date_numeric_qint64 < QDateTime::currentSecsSinceEpoch())
    {
        bool_license_invalidation_value = true;

        display_loading_progress_bar_non_cancelable_obj->hide();
        QMessageBox::warning(this, "RECON LAB", "Trial version license expired. Please contact sales@sumuri.com for purchase.");
        return;
    }

}

bool MainWindow::is_lic_passed_for_valid_day_remaining()
{
    // 32400000 // 375 days

    qint64 license_expire_day;

    if(license_expiry_date_numeric_qint64 == 0) // For old single year licenses
    {
        license_expire_day =  license_purchase_date_numeric_qint64 + ((375*24*60*60) -1);
    }
    else
    {
        license_expire_day = license_expiry_date_numeric_qint64;
    }


    if(license_expire_day < recon_jaari_smay_date_numeric_qint64) // 375 days
    {
        //        recon_static_functions::app_debug(" ---FAILED---recon_jaari_smay_date_epoch" + QString::number(recon_jaari_smay_date_epoch),Q_FUNC_INFO);
        //        recon_static_functions::app_debug(" ---FAILED--- license_expire_day " + QString::number(license_expire_day),Q_FUNC_INFO);
        return false;
    }

    return true;
}
void MainWindow::license_set_purchase_expiry_date_hash(QString hash , QString exp_hash)
{
    license_purchase_date_hash = hash;
    license_expiry_date_hash = exp_hash;
}


void MainWindow::license_set_module_hash_list(QStringList list)
{
    license_module_hash_list = list;
}
QString MainWindow::generate_magic_key(int choice)
{
    QByteArray magic_key;
    if(choice == 0)
    {
        QHostAddress host(QHostAddress::LocalHost);
        QString str1 = host.toString();

        QDateTimeEdit maxdatetime;
        qint64 year = maxdatetime.maximumDateTime().date().year();
        QString str2 = QString::number(year);


        QColor col = Qt::lightGray;
        QString str3 = QString::number(col.green());

        QComboBox combo;
        qint64 maxcombo = combo.maxCount();
        maxcombo = (maxcombo / 12)*12;

        QString str4 = QString::number(maxcombo);
        // QString str4 = temp.r

        QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
        QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);


        QByteArray total = ar1 + ar3 + ar4 + ar2;

        magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);


        return QString::fromLatin1(magic_key.data());
    }

    if(choice == 1)
    {
        QDateTimeEdit mindatetime;

        QString str1 = mindatetime.minimumDateTime().date().toString("MM-dd-yyy");

        QColor col = Qt::darkMagenta;
        QString str2 = QString::number(col.red());


        QComboBox combo;
        QDataStream stream;
        //   QString str3 = QString::number(combo.maxCount() / stream.version());
        QString str3 = QString::number(178956970);


        QString str4 = "@#$@#$##$#@$@$@";
        str4.append(QString::number(6547654674));

        // QString str4 = temp.r

        QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
        QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);


        QByteArray total = ar1 + ar3 + ar4 + ar2;

        magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);


        return QString::fromLatin1(magic_key.data());
    }

    if(choice == 2)
    {


        QColor col1 = Qt::darkCyan;
        QString str1 = QString::number(col1.blue());


        QString str2 = "@#$@#$##~###$$#@$@$@";
        str2.append(QString::number(15081947));


        QPushButton button;
        QColor col = Qt::darkMagenta;
        QColor col2 = Qt::lightGray;
        button.resize(col.green(), col2.red());

        QString str3 = QString::number(button.width());


        QString str4 = QString::number(Qt::Monday);
        // QString str4 = temp.r

        QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
        QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);


        QByteArray total = ar1 + ar3 + ar4 + ar2;

        magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);


        return QString::fromLatin1(magic_key.data());
    }

    if(choice == 3)
    {

        //  QColor col = Qt;

        QString str1 = QString::number(QFont::Black);

        QDateTimeEdit mindatetime;

        QString str2 = QString::number(mindatetime.minimumDateTime().toMSecsSinceEpoch()/500);


        QString str3 = QString::number(Qt::Monday);


        QString str4 = QString::number(QFont::Decorative);

        QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
        QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);


        QByteArray total = ar1 + ar3 + ar4 + ar2;

        magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);


        return QString::fromLatin1(magic_key.data());
    }

    if(choice == 11)
    {

        //  QColor col = Qt;

        QString str1 = QString::number(QFont::Black);

        QDateTimeEdit mindatetime;

        QString str2 = QString::number(mindatetime.minimumDateTime().toMSecsSinceEpoch()/500);


        QString str3 = QString::number(Qt::Monday);


        QString str4 = QString::number(QFont::Decorative);

        QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
        QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);

        QColor col1 = Qt::darkCyan;
        QString str11 = QString::number(col1.blue());


        QString str22 = "@#$@#$##~###$$#@$@$@@#$jghdklgherogyegfsb##~###$$#@$@$@";
        str22.append(QString::number(15081947));


        QPushButton button;
        QColor col = Qt::darkMagenta;
        QColor col2 = Qt::lightGray;
        button.resize(col.green(), col2.red());

        QString str33 = QString::number(button.width());


        QString str44 = QString::number(Qt::Monday);

        QByteArray ar11 = QCryptographicHash::hash(str11.toUtf8(),QCryptographicHash::Sha1);
        QByteArray ar22 = QCryptographicHash::hash(str22.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar33 = QCryptographicHash::hash(str33.toUtf8(),QCryptographicHash::Md5);
        QByteArray ar44 = QCryptographicHash::hash(str44.toUtf8(),QCryptographicHash::Sha1);

        QByteArray total = ar1 + ar11 + ar3 + ar33 + ar4 + ar44 + ar2 + ar22;
        magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);


        return QString::fromLatin1(magic_key.data());
    }
}

void MainWindow::check_for_international()
{    
    QStringList blacklisted_usbs;
    blacklisted_usbs << "2760E402"
                     << "500AD180"
                     << "AA4UWV4VAN6TUCQM"
                     << "8BCB99CE"
                     << "0375718070001346"
                     << "5175E593"
                     << "12623828"
                     << "13F56510"
                     << "ADC5DFA4"
                     << "AADMA53HEL8WTSKV"
                     << "AAY9YP6BZ486B94K"
                     << "AAHMKE3P9IE1H3WO"
                     << "SCY0000000024742"
                     << "FB4D1BE0"
                     << "AAIE857QLID84P50"
                     << "AA4QNL37BEABUSPR"
                     << "AAW01OF9G382BVLF"
                     << "SCY0000000022435"
                     << "6E6F56BE"
                     << "AAOJH062I7PGB3IQ"
                     << "9E15130A"
                     << "AANO9M4F0RNS3HM9"
                     << "E071430D"
                     << "AA1BCHD5F36WYCNG"
                     << "AASHKOGCUIIE5BOP"
                     << "0847ABC8"
                     << "0376218070001216"
                     << "0375418070001150"
                     << "0373118070001316"
                     << "0373118070001234"
                     << "0373218070001641"
                     << "0376318070001310"
                     << "0373118070001312"
                     << "0376318070001199"
                     << "0375418070001156";


    for(int i = 0 ; i < blacklisted_usbs.size(); i++)
    {
        if(licensed_usb_device == blacklisted_usbs.at(i))
        {
            bool_is_international_version = true;
            return;
        }
    }




    QFile file(recon_helper_standard_obj->pub_get_app_license_path());

    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QStringList list;

    while(!file.atEnd())
        list << file.readLine();

    QString magic_key = generate_magic_key(11);
    QString str = magic_key + licensed_usb_device;
    QByteArray magic_key_hash = QCryptographicHash::hash(str.toUtf8(),QCryptographicHash::Sha1);

    for(int i = 0; i < list.size(); i++)
    {
        if(list.at(i).trimmed() == magic_key_hash.toHex())
        {
            bool_is_international_version = true;
            return ;
        }
    }
    return;
}


void MainWindow::check_for_demo_version()
{
    is_it_a_demo = true;
    QFile file(recon_helper_standard_obj->pub_get_app_license_path());



    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- license file", Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString(), Q_FUNC_INFO);
        return;
    }


    QStringList list;

    while(!file.atEnd())
        list << file.readLine();



    QString magic_key = generate_magic_key(2);
    QString str = magic_key + licensed_usb_device;
    QByteArray magic_key_hash = QCryptographicHash::hash(str.toUtf8(),QCryptographicHash::Sha1);


    QString magic_key_international = generate_magic_key(11);
    QString str_international = magic_key_international + licensed_usb_device;
    QByteArray magic_key_hash_international = QCryptographicHash::hash(str_international.toUtf8(),QCryptographicHash::Sha1);

    for(int i = 0; i < list.size(); i++)
    {
        if(list.at(i).trimmed() == magic_key_hash_international.toHex())
        {
            bool_is_international_version = true;
            break;
        }

    }


    for(int i = 0; i < list.size(); i++)
    {
        if(list.at(i).trimmed() == magic_key_hash.toHex())
        {
            return;
        }

    }

    is_it_a_demo = false;

}


void MainWindow::lic_validate_connected_usb_device_newer_from_june_2019()
{

    licensed_usb_present = false;

    QString ver = "EverGreen.2018.July.01";
    ver.append("#@$@$@## $@vj fdj v43%  $#%4$^^$$^%");

    QFile file(recon_helper_standard_obj->pub_get_app_license_path());

    if(!file.open(QIODevice::ReadOnly))
    {
        //        recon_static_functions::app_debug(" file open ---FAILED--- license file",Q_FUNC_INFO);
        //        recon_static_functions::app_debug(" error " + file.errorString(),Q_FUNC_INFO);

        lic_file_not_exists = true;

        licensed_usb_present = false;
        return;
    }

    QString master_hash = file.readLine().trimmed();
    file.readLine();
    purchase_date_hash = file.readLine().trimmed();

    QString usb_line = file.readLine().trimmed();

    expiry_date_hash = file.readLine().trimmed();


    QString magic_key = generate_magic_key(1);
    QByteArray magic_key_hash = QCryptographicHash::hash(magic_key.toUtf8(),QCryptographicHash::Md5);

    usb_device_serial_list l1;
    l1.update_usb_list();
    usb_device_list.clear();
    usb_device_list = l1.usb_serial_number_list();


    licensed_usb_device = "";
    for(int i = 0; i < usb_device_list.size(); i++)
    {
        QString usb_serial = usb_device_list.at(i);

        QString usb_string_for_hash = usb_serial + magic_key_hash.toHex() + ver;
        QByteArray usb_hash = QCryptographicHash::hash(usb_string_for_hash.toUtf8(),QCryptographicHash::Sha1);


        if(usb_hash.toHex() == usb_line.trimmed())
        {
            licensed_usb_present = true;
            licensed_usb_device = usb_serial;
            poller = licensed_usb_device;

            break;

        }

    }

}


void MainWindow::lic_validate_license_integerity_newer_from_june_2019()
{

    lic_integerity_failed = false;

    QFile file(recon_helper_standard_obj->pub_get_app_license_path());

    if(!file.open(QIODevice::ReadOnly))
    {
        lic_file_not_exists = true;
        return;
    }

    QString master_hash = file.readLine().trimmed();
    file.readLine(); // Ignore dummy line
    QString purchase_date_hash = file.readLine().trimmed();


    QString usb_line = file.readLine().trimmed();

    QString expiry_date_hash = file.readLine().trimmed();

    customer_name = file.readLine().trimmed();

    license_set_purchase_expiry_date_hash(purchase_date_hash,expiry_date_hash);

    QStringList module_hash_list;

    while(!file.atEnd())
    {
        module_hash_list << file.readLine().trimmed();
    }
    license_set_module_hash_list(module_hash_list);

    QString file_content_combined;

    file_content_combined.append(purchase_date_hash);
    file_content_combined.append(usb_line);
    file_content_combined.append(expiry_date_hash);
    file_content_combined.append(customer_name);
    for(int i = 0; i < module_hash_list.size(); i++)
    {
        file_content_combined.append(module_hash_list.at(i)); // combined all hashes of modules

    }
    QByteArray file_content_hash = QCryptographicHash::hash(file_content_combined.toUtf8(),QCryptographicHash::Sha1);

    if(file_content_hash.toHex() != master_hash)
    {

        lic_integerity_failed = true;
        return;
    }

}

