#include "super_timeline.h"
#include "ui_super_timeline.h"

super_timeline::super_timeline(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::super_timeline)
{

    recon_helper_standard_obj = new recon_helper_standard(this);


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

    ui->setupUi(this);

    ui->lineEdit_file_name->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_file_path->setAttribute(Qt::WA_MacShowFocusRect, false);


    QRegularExpression rx("^[a-zA-Z0-9_ -]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->lineEdit_file_name->setValidator(validator);


    ui->dateTimeEdit_end_time->blockSignals(true);

    QString current_date_time = QDateTime::currentDateTime().toString("yyyy/MMM/dd hh:mm");
    QDateTime date_time;
    date_time = QDateTime::fromString(current_date_time,"yyyy/MMM/dd hh:mm");
    ui->dateTimeEdit_end_time->setDateTime(date_time);

    ui->dateTimeEdit_end_time->blockSignals(false);


    setWindowTitle(recon_static_functions::get_app_name() + " - Super Timeline");


    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    ui->radioButton_sqlite->setChecked(true);
    ui->lineEdit_record_per_csv->setDisabled(true);

    QRegularExpression rx1("^[0-9]*$");
    QValidator *validator_1 = new QRegularExpressionValidator(rx1, this);
    ui->lineEdit_record_per_csv->setValidator(validator_1);

    ui->radioButton_fs_records->setChecked(true);

}

super_timeline::~super_timeline()
{
    delete ui;
}

void super_timeline::set_essentials()
{
    QList<struct_GLOBAL_witness_info_source> witness_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    bool bool_src_support_fs_timeline = false;
    for(int ii = 0; ii < witness_info_list.size(); ii++)
    {
        if(witness_info_list.at(ii).os_scheme_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                && witness_info_list.at(ii).os_scheme_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
        {
            bool_src_support_fs_timeline = true;
            break;
        }
    }


    if(bool_src_support_fs_timeline)
    {
        ui->radioButton_both->setVisible(true);
        ui->radioButton_fs_records->setVisible(true);
        ui->radioButton_fs_records->setChecked(true);
    }
    else
    {
        ui->radioButton_both->setVisible(false);
        ui->radioButton_fs_records->setVisible(false);
        ui->radioButton_plugins_record->setChecked(true);
    }

    ui->lineEdit_file_name->setPlaceholderText("Super_Timeline");
    ui->lineEdit_file_name->setFocus();
    ui->lineEdit_file_name->setToolTip("Enter File name");
    ui->lineEdit_file_path->setToolTip("Browse File Path");
    ui->radioButton_plugins_record->setVisible(true);

}

void super_timeline::closeEvent(QCloseEvent *)
{
    ui->dateTimeEdit_start_time->calendarWidget()->parentWidget()->close();
    ui->dateTimeEdit_end_time->calendarWidget()->parentWidget()->close();

    on_pushButton_cancel_clicked();
}

void super_timeline::on_pushButton_change_file_path_clicked()
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
    }


    if(path.trimmed().isEmpty())
        return;

    if(!path.endsWith("/"))
        path.append("/");

    ui->lineEdit_file_path->setText(path);

}

void super_timeline::on_pushButton_cancel_clicked()
{
    this->hide();
    emit signal_super_timeline_done(false);

}


void super_timeline::on_pushButton_ok_clicked()
{

    QString filename = ui->lineEdit_file_name->text().trimmed();
    QString dir_path = ui->lineEdit_file_path->text().trimmed();


    if(filename.isEmpty())
        filename = ui->lineEdit_file_name->placeholderText();

    if(dir_path.trimmed().isEmpty())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please fill File Path");
        return;
    }

    if(filename.trimmed().isEmpty())
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Please fill File Name");
        return;
    }


    QString records_per_csv_str = ui->lineEdit_record_per_csv->text();
    if(!records_per_csv_str.trimmed().isEmpty())
    {
        qint64 records_per_csv_int = records_per_csv_str.toInt();
        if(records_per_csv_int <= 0)
        {
            QMessageBox::warning(this,recon_static_functions::get_app_name(), "Please enter a valid number");
            return;
        }
        else if(records_per_csv_int > 1000000)
        {
            QMessageBox::warning(this,recon_static_functions::get_app_name(), "Maximum number of records per csv must be less than or equal to '1000000'");
            return;
        }
    }


    filename = filename.replace(" ", "_");

    if(dir_path.isEmpty())
        dir_path = ui->lineEdit_file_path->placeholderText();



    qint64 start_epch = ui->dateTimeEdit_start_time->dateTime().toSecsSinceEpoch();
    qint64 end_epch = ui->dateTimeEdit_end_time->dateTime().toSecsSinceEpoch();

    start_epch = global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(start_epch);
    end_epch = global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(end_epch);

    if(start_epch > end_epch)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Invalid timestamp criteria.");
        return;
    }


    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Generating Timeline...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QCoreApplication::processEvents();

    emit signal_super_timeline_done(true);

    if(ui->radioButton_fs_records->isChecked())
    {
        fs_records(filename,dir_path ,start_epch , end_epch);
    }
    else if(ui->radioButton_plugins_record->isChecked())
    {
        plugins_record(filename , dir_path , start_epch , end_epch);
    }
    else if(ui->radioButton_both->isChecked())
    {
        both_record(filename , dir_path , start_epch , end_epch);
    }



    this->hide();

    display_loading_progress_bar_obj->hide();
    if(bool_cancel_loading)
        return;


    if(!QFileInfo(output_file_dir_path).exists())
    {
        // QMessageBox::warning(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("No timestamp found."));
        return;
    }

    report_status obj;
    obj.set_label("Timeline Generated.Do you want to open file?");

    if(ui->radioButton_sqlite->isChecked())
    {
        if(obj.exec())
        {
            QStringList arg;
            arg << output_file_dir_path;

            QProcess myProcess;// = new QProcess(this);
            myProcess.startDetached("open",arg);
        }
    }
    else if(ui->radioButton_csv->isChecked())
    {
        if(obj.exec())
        {
            QStringList arg;
            arg << output_file_dir_path;

            QProcess myProcess;// = new QProcess(this);
            myProcess.startDetached("open",arg);
        }
    }

}



void super_timeline::slot_hide_loading_display_dialog_box(bool status)
{
    bool_cancel_loading = true;
}

void super_timeline::loading_display_dialog_based_on_records(qint64 total_records, qint64 processed_count)
{
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

}


qint64 super_timeline::get_total_records_available_for_selected_timestmp()
{

    qint64 start_epch = ui->dateTimeEdit_start_time->dateTime().toSecsSinceEpoch();
    qint64 end_epch = ui->dateTimeEdit_end_time->dateTime().toSecsSinceEpoch();


    QString plugin_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(plugin_timeline_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return total_records_for_selected_timestamp;
    }

    QSqlQuery query(destination_db);
    QString command = "select timestamp from main.timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);

    QStringList attachment_name_list;
    QList<struct_GLOBAL_witness_info_source> list_obj = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int zz = 0; zz < list_obj.size(); zz++)
    {
        struct_GLOBAL_witness_info_source mobj = list_obj.at(zz);
        QString fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + mobj.source_count_name + ".sqlite";
        if(!QFileInfo(fs_timeline_db_path).exists())
            continue;

        QString attachment_name = QString("filesystemDB") + QString::number(zz);
        attachment_name_list << attachment_name;

        QString tmp_cnct = QString("ATTACH DATABASE '%1' AS ") + attachment_name;
        QString fs_db_attach_commmand = tmp_cnct
                .arg(QDir::toNativeSeparators(fs_timeline_db_path));
        query.exec(fs_db_attach_commmand);

        command += " UNION ALL select timestamp from " + attachment_name + ".timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);

    }

    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" query select ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return total_records_for_selected_timestamp;
    }


    int records_counter = 0;
    while(query.next())
        records_counter++;

    total_records_for_selected_timestamp = records_counter;

    for(int pp = 0; pp < attachment_name_list.size(); pp++)
    {
        QString filesystem_db_dettach_commmand = QString("DETACH DATABASE ") + attachment_name_list.at(pp);
        query.exec(filesystem_db_dettach_commmand);
    }


    destination_db.close();

    return total_records_for_selected_timestamp;

}

void super_timeline::save_timeline_results_in_db(QString filename , QString filepath , QSqlQuery &query)
{

    filename =  recon_static_functions::get_available_filename(filename +".sqlite" , filepath , Q_FUNC_INFO);
    QString destination_db_path = filepath + filename;

    QString create_tbl_command = "create table timeline_data ( INT INTEGER PRIMARY KEY , Timestamp INTEGER, Stamp_Name varchar(300), Stamp_Type varchar(300),Source varchar(300) , Item1 Text, Item2 Text,  Plugin varchar(300), Category varchar(300))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(create_tbl_command, destination_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("Table creation --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
    }

    output_file_dir_path = destination_db_path;



    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_path,Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return ;
    }


    destination_db.transaction();
    QSqlQuery insert_qry(destination_db);
    qint64 counter;
    while(query.next())
    {

        counter++;

        if(counter % 100 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;

        insert_qry.prepare("insert into timeline_data (Timestamp,Source,Item1,Item2,Stamp_Name,Stamp_Type,Plugin,Category) Values (? ,?,?,?,?,?,?,?)");

        QString source_count_name = query.value(7).toString();
        struct_GLOBAL_witness_info_source mobj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        insert_qry.addBindValue(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(0).toString(), Q_FUNC_INFO));
        insert_qry.addBindValue(mobj.complete_source_name);
        insert_qry.addBindValue(query.value(1).toString());
        insert_qry.addBindValue(query.value(2).toString());
        insert_qry.addBindValue(query.value(3).toString());
        insert_qry.addBindValue(query.value(4).toString());
        insert_qry.addBindValue(query.value(5).toString());
        insert_qry.addBindValue(query.value(6).toString());


        if(!insert_qry.exec())
        {
            recon_static_functions::app_debug(" query select ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + insert_qry.lastError().text(),Q_FUNC_INFO);
            destination_db.close();
            display_loading_progress_bar_obj->hide();
            return;
        }

    }

    destination_db.commit();

    destination_db.close();

}

void super_timeline::save_timeline_results_in_csv(QString filename, QString filepath, QSqlQuery &query)
{
    QString records_per_csv_str = ui->lineEdit_record_per_csv->text().trimmed();
    qint64 records_per_csv_int;

    if(records_per_csv_str.trimmed() == QString(""))
        records_per_csv_int = 1000000;
    else
        records_per_csv_int = records_per_csv_str.toInt();


    output_file_dir_path = filepath;

    QString file_name_str = recon_static_functions::get_available_filename((filename + ".csv"), filepath, Q_FUNC_INFO);
    qint64  m_count = 0;
    QFile m_file;
    qint64 serial_number = 1;
    QStringList header_list;
    header_list << "S.No" << "Timestamp" << "Stamp Name" << "Stamp Type" << "Item1" << "Item2"  << "Plugin" << "Category" << "Source";
    QString csv_header_str ;
    csv_header_str = header_list.join(",").append("\n");

    bool create_new_file = true;
    QTextStream output_stream_for_csv_file;
    int file_counter = 1;

    while(query.next())
    {

        if(m_count != 0 && m_count % records_per_csv_int == 0)
            create_new_file = true;

        if(create_new_file)
        {
            if(m_file.isOpen())
                m_file.close();

            QString timeline_csv_file = filepath + file_name_str;
            timeline_csv_file = filepath + filename + QString::number(file_counter) + ".csv";
            m_file.setFileName(timeline_csv_file);
            if(!m_file.open(QIODevice::WriteOnly))
            {
                recon_static_functions::app_debug("file open ---FAILED---" + m_file.fileName(), Q_FUNC_INFO);
                recon_static_functions::app_debug("error " + m_file.errorString() , Q_FUNC_INFO);
                return;
            }

            file_counter++;
            output_stream_for_csv_file.setDevice(&m_file);

            output_stream_for_csv_file << csv_header_str;

            create_new_file = false;
        }


        m_count++;

        if(m_count % 500 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;


        QString src_cnt_name = query.value(7).toString();
        struct_GLOBAL_witness_info_source mobj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_name);
        QString item1 = query.value(1).toString();
        QString item2 = query.value(2).toString();


        if(item1.contains("\n"))
            item1.replace("\n" , " ");

        if(item2.contains("\n"))
            item2.replace("\n" , " ");

        QStringList value_list;
        value_list << QString("\"") + QString::number(serial_number) + QString("\"")
                   << QString("\"") + global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(0).toString(), Q_FUNC_INFO) + QString("\"")

                   << QString("\"") + query.value(3).toString() + QString("\"")///stamp_name
                   << QString("\"") + query.value(4).toString() + QString("\"")///stamp_type
                   << QString("\"") + item1 + QString("\"")
                   << QString("\"") + item2  + QString("\"")

                   << QString("\"") + query.value(5).toString() + QString("\"")///plugin
                   << QString("\"") + query.value(6).toString() + QString("\"")///category
                   << QString("\"") + mobj.complete_source_name + QString("\"");

        output_stream_for_csv_file << value_list.join(",") << "\n";
        serial_number++;
    }

    if(m_file.isOpen())
    {
        m_file.flush();
        m_file.close();
    }
}

void super_timeline::on_radioButton_sqlite_toggled(bool checked)
{
    ui->lineEdit_record_per_csv->setDisabled(checked);
    ui->pushButton_ok->setDisabled(false);

}


void super_timeline::on_radioButton_csv_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit_record_per_csv->setHidden(false);
        //  qint64 total_records = get_total_records_available_for_selected_timestmp();
        // ui->lineEdit_record_per_csv->setPlaceholderText("Records Per CSV (Must be less than or equal to " + QString::number(total_records) + ")");
        // ui->pushButton_ok->setDisabled(false);
    }
}


void super_timeline::on_dateTimeEdit_start_time_dateTimeChanged(const QDateTime &dateTime)
{
    //    qint64 total_records = get_total_records_available_for_selected_timestmp();

    //    if(total_records == 0)
    //        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Invalid Start Time");
    //    else
    //        ui->lineEdit_record_per_csv->setPlaceholderText("Records Per CSV (Must be less than or equal to " + QString::number(total_records) + ")");

}

void super_timeline::on_dateTimeEdit_end_time_dateTimeChanged(const QDateTime &dateTime)
{
    //    qint64 total_records = get_total_records_available_for_selected_timestmp();

    //    if(total_records == 0)
    //        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Invalid End Time");
    //    else
    //        ui->lineEdit_record_per_csv->setPlaceholderText("Records Per CSV (Must be less than or equal to " + QString::number(total_records) + ")");

}

void super_timeline::fs_records(QString filename, QString dir_path , qint64 start_epch, qint64 end_epch)
{
    QList<struct_GLOBAL_witness_info_source> list_obj = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QString fs_timeline_db_path;
    for(int zz = 0; zz < list_obj.size(); zz++)
    {
        struct_GLOBAL_witness_info_source mobj = list_obj.at(zz);
        fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + mobj.source_count_name + ".sqlite";
        if(!QFileInfo(fs_timeline_db_path).exists())
            continue;

        if(zz == 0)
            break;
    }

    if(!QFileInfo(fs_timeline_db_path).exists())
        return;

    QStringList attachment_name_list;
    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_timeline_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }

    QSqlQuery query(destination_db);
    QString command = "select timestamp,item1,item2,stamp_name,stamp_type,plugin,category,source_count_name from main.timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);

    for(int zz = 0; zz < list_obj.size(); zz++)
    {
        struct_GLOBAL_witness_info_source mobj = list_obj.at(zz);

        if(zz == 0)
            continue ;

        QString joined_dbs_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + mobj.source_count_name + ".sqlite";
        if(!QFileInfo(joined_dbs_path).exists())
            continue;


        QString attachment_name = QString("filesystemDB") + QString::number(zz);
        attachment_name_list << attachment_name;

        QString tmp_cnct = QString("ATTACH DATABASE '%1' AS ") + attachment_name;
        QString fs_db_attach_commmand = tmp_cnct
                .arg(QDir::toNativeSeparators(joined_dbs_path));
        query.exec(fs_db_attach_commmand);

        command += " UNION ALL select timestamp,item1,item2,stamp_name,stamp_type,plugin,category,source_count_name from " + attachment_name + ".timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);

    }
    command += " ORDER BY timestamp ASC";

    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" query select ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        display_loading_progress_bar_obj->hide();
        return;
    }



    if(ui->radioButton_sqlite->isChecked())
    {
        save_timeline_results_in_db(filename , dir_path , query);
    }
    else if(ui->radioButton_csv->isChecked())
    {
        QString dir_name = recon_static_functions::get_available_filename((filename), dir_path, Q_FUNC_INFO);
        dir_path =  dir_path + dir_name + "/";
        QDir dir;
        dir.mkpath(dir_path);

        save_timeline_results_in_csv( filename , dir_path , query);
    }


    for(int pp = 0; pp < attachment_name_list.size(); pp++)
    {
        QString filesystem_db_dettach_commmand = QString("DETACH DATABASE ") + attachment_name_list.at(pp);
        query.exec(filesystem_db_dettach_commmand);
    }

    destination_db.close();
}


void super_timeline::plugins_record(QString filename, QString dir_path , qint64 start_epch, qint64 end_epch)
{
    QString plugin_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";
    if(!QFileInfo(plugin_timeline_db_path).exists())
        return;

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(plugin_timeline_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }

    QSqlQuery query(destination_db);
    QString command = "select timestamp,item1,item2,stamp_name,stamp_type,plugin,category,source_count_name from main.timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);
    command += " ORDER BY timestamp ASC";
    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" query select ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        display_loading_progress_bar_obj->hide();
        return;
    }

    if(ui->radioButton_sqlite->isChecked())
    {
        save_timeline_results_in_db(filename , dir_path , query);
    }
    else if(ui->radioButton_csv->isChecked())
    {
        QString dir_name = recon_static_functions::get_available_filename((filename), dir_path, Q_FUNC_INFO);
        dir_path =  dir_path + dir_name + "/";
        QDir dir;
        dir.mkpath(dir_path);

        save_timeline_results_in_csv( filename , dir_path , query);
    }

    destination_db.close();
}

void super_timeline::both_record(QString filename, QString dir_path , qint64 start_epch, qint64 end_epch)
{
    QString plugin_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";
    if(!QFileInfo(plugin_timeline_db_path).exists())
        return;

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(plugin_timeline_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }

    QSqlQuery query(destination_db);
    QString command = "select timestamp,item1,item2,stamp_name,stamp_type,plugin,category,source_count_name from main.timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);

    QStringList attachment_name_list;
    QList<struct_GLOBAL_witness_info_source> list_obj = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int zz = 0; zz < list_obj.size(); zz++)
    {
        struct_GLOBAL_witness_info_source mobj = list_obj.at(zz);
        QString fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + mobj.source_count_name + ".sqlite";
        if(!QFileInfo(fs_timeline_db_path).exists())
            continue;

        QString attachment_name = QString("filesystemDB") + QString::number(zz);
        attachment_name_list << attachment_name;

        QString tmp_cnct = QString("ATTACH DATABASE '%1' AS ") + attachment_name;
        QString fs_db_attach_commmand = tmp_cnct
                .arg(QDir::toNativeSeparators(fs_timeline_db_path));
        query.exec(fs_db_attach_commmand);

        command += " UNION ALL select timestamp,item1,item2,stamp_name,stamp_type,plugin,category,source_count_name from " + attachment_name + ".timeline_data  Where timestamp >= " + QString::number(start_epch) + " AND timestamp <= " + QString::number(end_epch);

    }


    command += " ORDER BY timestamp ASC";

    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" query select ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        display_loading_progress_bar_obj->hide();
        return;
    }

    if(ui->radioButton_sqlite->isChecked())
    {
        save_timeline_results_in_db(filename , dir_path , query);
    }
    else if(ui->radioButton_csv->isChecked())
    {
        QString dir_name = recon_static_functions::get_available_filename((filename), dir_path, Q_FUNC_INFO);
        dir_path =  dir_path + dir_name + "/";
        QDir dir;
        dir.mkpath(dir_path);

        save_timeline_results_in_csv( filename , dir_path , query);
    }


    for(int pp = 0; pp < attachment_name_list.size(); pp++)
    {
        QString filesystem_db_dettach_commmand = QString("DETACH DATABASE ") + attachment_name_list.at(pp);
        query.exec(filesystem_db_dettach_commmand);
    }

    destination_db.close();
}
