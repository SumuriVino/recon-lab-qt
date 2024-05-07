#include "export_hashes_as_sqlite.h"
#include "ui_export_hashes_as_sqlite.h"

export_hashes_as_sqlite::export_hashes_as_sqlite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_hashes_as_sqlite)
{
    ui->setupUi(this);
    recon_helper_standard_obj = new recon_helper_standard(this);

    obj_export_rsync = new export_rsync(this);

    bool_message_box_yes_button_clicked = false;
    obj_message_dialog = new message_dialog;
    connect(obj_message_dialog,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(parent);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    connect(display_loading_progress_bar_non_cancelable_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));

    ui->lineEdit_file_path->setReadOnly(true);
    setWindowTitle("Export Hashes - SQLite");

}

export_hashes_as_sqlite::~export_hashes_as_sqlite()
{
    delete ui;
}

void export_hashes_as_sqlite::pub_set_essential()
{
    ui->lineEdit_file_path->clear();
    ui->lineEdit_file_name->clear();
}
void export_hashes_as_sqlite::pub_set_export_file_details(QList<struct_global_export_as_hashes_details>& details_list)
{
    export_as_sqlite_details_list = details_list;
}


void export_hashes_as_sqlite::pub_start_export_hashes_as_sqlite()
{ // start export in the form of sqlite
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(false);
    display_loading_progress_bar_non_cancelable_obj->show();

    if(!sqlite_export_dest_dirpath.endsWith("/"))
        sqlite_export_dest_dirpath.append("/");


    if(!create_tables_for_exported_sqlite())
        return;

    struct_file_hashes hashes_obj;

    QString source_count_name;

    QString source_count;

    QString time_format = "yyyy-MM-ddThh:mm:ss";

    hashes_struct_list.clear();

    int tmp_cnt = 0;
    for(int count = 0; count < export_as_sqlite_details_list.size(); count++)
    {
        if(bool_cancel_loading)
            break;

        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_global_export_as_hashes_details export_as_vic_details_obj =  export_as_sqlite_details_list.at(count);
        source_count_name = export_as_vic_details_obj.source_count_name;
        struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString cmplt_filepath = export_as_vic_details_obj.complete_file_path;
        QString display_filepath = export_as_vic_details_obj.display_filepath;

        hashes_obj = calculate_hashes(cmplt_filepath);


        source_count = export_as_vic_details_obj.source_count_name;



        if(source_count.startsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag)))
            source_count.remove(0,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).size());


        QString  source_file_path = QFileInfo(hashes_obj.filepath).filePath();
        if(source_file_path.isEmpty())
            continue;

        QString src_filename = QFileInfo(hashes_obj.filepath).fileName();

        QString export_dir_path = sqlite_export_dest_dirpath + "/" +source_count_name + "/";
        QDir  dir;
        dir.mkpath(export_dir_path);

        QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(src_filename,export_dir_path,Q_FUNC_INFO);

        QString destination_file_path = export_dir_path + available_file_name;

        if(struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            QString display_filename = export_as_vic_details_obj.display_filename;
            QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(display_filename,export_dir_path,Q_FUNC_INFO);
            destination_file_path = export_dir_path + available_file_name;
        }

        QString exported_file_path = obj_export_rsync->pub_copy_file_using_rsync(source_file_path,destination_file_path, enum_global_rsync_args_type_links_and_recursive);

        hashes_obj.display_filepath = export_as_vic_details_obj.display_filepath;
        hashes_obj.display_filename = export_as_vic_details_obj.display_filename;
        hashes_obj.source_type_internal = struct_target_source_info.source_type_internal;

        QString relative_filepath = hashes_obj.display_filepath;
        if(relative_filepath.startsWith("/"))
            relative_filepath.remove(0,1);

        relative_filepath.replace("/", "\\");


        hashes_obj.relative_filepath = relative_filepath;
        hashes_obj.source_count = source_count_name;


        QDateTime dt_change;
        dt_change.setTimeSpec(Qt::UTC);
        dt_change = dt_change.fromSecsSinceEpoch(export_as_vic_details_obj.change_timestamp.toLongLong(), Qt::UTC);
        hashes_obj.change_time = dt_change.toString(time_format);

        QDateTime dt_written;
        dt_written.setTimeSpec(Qt::UTC);
        dt_written = dt_written.fromSecsSinceEpoch(export_as_vic_details_obj.modif_timestamp.toLongLong(), Qt::UTC);
        hashes_obj.modify_time  = dt_written.toString(time_format);

        QDateTime dt_access;
        dt_access.setTimeSpec(Qt::UTC);
        dt_access = dt_access.fromSecsSinceEpoch(export_as_vic_details_obj.last_acces_timestamp.toLongLong(), Qt::UTC);
        hashes_obj.access_time = dt_access.toString(time_format);

        QDateTime dt_create;
        dt_create.setTimeSpec(Qt::UTC);
        dt_create = dt_create.fromSecsSinceEpoch(export_as_vic_details_obj.create_timestamp.toLongLong(), Qt::UTC);
        hashes_obj.create_time = dt_create.toString(time_format);

        hashes_struct_list.append(hashes_obj);
    }

    if(bool_cancel_loading)
        return;

    write_data_in_sqlite();

    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_message_dialog->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
    obj_message_dialog->show();


    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

bool export_hashes_as_sqlite::create_tables_for_exported_sqlite()
{ // create tables in database for exported sqlite
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QString recon_case_dir_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    QString available_dir_name = recon_static_functions::get_available_filename(recon_case_dir_str,sqlite_export_dest_dirpath, Q_FUNC_INFO);
    sqlite_export_dest_dirpath    = recon_static_functions::get_complete_file_path(sqlite_export_dest_dirpath, available_dir_name, Q_FUNC_INFO);
    QDir dir;
    dir.mkpath(sqlite_export_dest_dirpath);

    QString file_name;
    if(ui->lineEdit_file_name->text().isEmpty())
    {
        file_name = ui->lineEdit_file_name->placeholderText().trimmed();
    }
    else
    {
        file_name = ui->lineEdit_file_name->text().trimmed();
    }
    destination_db_path = sqlite_export_dest_dirpath + "/" + file_name + ".sqlite";

    QString create_table_command = "Create Table file(file_name VARCHAR(100), file_size_byte VARCHAR(100),"
                                   "modify_timestamp_utc VARCHAR(300), change_timestamp_utc VARCHAR(300),"
                                   "accesed_timestamps_utc VARCHAR(300), created_timestamp_utc VARCHAR(300), MD5_Hash VARCHAR(200), SHA1_Hash VARCHAR(200),"
                                   "file_path VARCHAR(200), category VARCHAR(100))";

    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(create_table_command,destination_db_path,Q_FUNC_INFO))
    {
        return false;
    }

    QString case_info_command = "CREATE TABLE info(Key VARCHAR(200) , Value VARCHAR(300))";

    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(case_info_command,destination_db_path,Q_FUNC_INFO))
    {
        return false;
    }

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
    return  true;


}

void export_hashes_as_sqlite::write_data_in_sqlite()
{ // write data in tables
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Db open -----Failed------ -  " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
    }


    QString insert_command = "INSERT INTO file (file_name, file_size_byte, modify_timestamp_utc, change_timestamp_utc,"
                             " accesed_timestamps_utc, created_timestamp_utc, MD5_Hash, SHA1_Hash,file_path,category) Values(?,?,?, ?,?,?, ?,?,?,?)";

    for(int ii = 0; ii < hashes_struct_list.size(); ii++)
    {
        if(bool_cancel_loading)
            break;


        QStringList value_list;
        struct_file_hashes st_hashes_obj = hashes_struct_list.at(ii);
        QString file_path = st_hashes_obj.filepath;
        QFileInfo info(file_path);


        value_list << st_hashes_obj.display_filename;
        value_list << QString::number(info.size());
        value_list << st_hashes_obj.modify_time;
        value_list << st_hashes_obj.change_time;
        value_list << st_hashes_obj.access_time;
        value_list << st_hashes_obj.create_time;
        value_list << st_hashes_obj.md5_hash;
        value_list << st_hashes_obj.sha1_hash;
        value_list << st_hashes_obj.display_filepath;
        value_list << st_hashes_obj.source_count;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_command,value_list,destination_db,Q_FUNC_INFO);
    }

    if(bool_cancel_loading)
    {
        destination_db.close();
        return;
    }

    QStringList val_list;
    QString insert_value_command = "INSERT INTO info(Key,Value) values(?,?)";

    val_list  <<  "Case Name" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Case ID" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list << "Case Location" <<  global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list << "Case Notes" <<  global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Agency Name" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Agency Address " << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Examiner Name"<< global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Examiner Phone" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  <<"Examiner Email" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Application Name" <<recon_static_functions::get_app_name();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    val_list.clear();
    val_list  << "Application Version" << recon_static_functions::get_app_version();
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_value_command,val_list,destination_db,Q_FUNC_INFO);

    destination_db.close();


    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

}

export_hashes_as_sqlite::struct_file_hashes export_hashes_as_sqlite::calculate_hashes(QString file_path)
{ // calculate hashes
    struct_file_hashes hashes_obj;

    hashes_obj.filepath  = file_path;
    hashes_obj.md5_hash  = QString("");
    hashes_obj.sha1_hash = QString("");


    unsigned char a_md5[MD5_DIGEST_LENGTH], a_sha1[SHA_DIGEST_LENGTH];
    int i, bytes;
    unsigned char data[1024];

    MD5_CTX md_context;
    MD5_Init(&md_context);

    SHA_CTX sha_context;
    SHA1_Init(&sha_context);

    ///open file and read data
    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {
        return hashes_obj;
    }

    qint64 file_total_size = target_file.size();
    if(file_total_size <= 0)
        return hashes_obj;


    int count = 0;
    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        count++;
        if(count % 30 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        MD5_Update(&md_context, data, bytes);
        SHA1_Update(&sha_context, data, bytes);
    }

    target_file.close();


    QByteArray md5_ar;
    MD5_Final(a_md5, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
        md5_ar.append(a_md5[i]);

    QByteArray sha1_ar;
    SHA1_Final(a_sha1, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
        sha1_ar.append(a_sha1[i]);


    hashes_obj.md5_hash  = QString::fromLocal8Bit(md5_ar.toHex());
    hashes_obj.sha1_hash = QString::fromLocal8Bit(sha1_ar.toHex());

    return hashes_obj;
}

void export_hashes_as_sqlite::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{ // yes/no while exporting
    bool_message_box_yes_button_clicked = yes_no_clicked;
    if(bool_message_box_yes_button_clicked)
    {
        point_to_export_dir(destination_db_path);
        hide();

    }
}
void export_hashes_as_sqlite::point_to_export_dir(QString path)
{ // point to export directory after export done
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void export_hashes_as_sqlite::on_pushButton_export_clicked()
{ // export button clicked
    if(ui->lineEdit_file_path->text().isEmpty())
    {
        obj_message_dialog->pub_set_message(MACRO_MSG_WARNING, "Select Output Directory");
        obj_message_dialog->show();
        return;
    }
    hide();

    emit signal_export_button_clicked();
}

void export_hashes_as_sqlite::on_pushButton_cancel_clicked()
{ // hide on cancel buitton clicked
    hide();

}

void export_hashes_as_sqlite::on_pushButton_select_clicked()
{ // select button clicked
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            sqlite_export_dest_dirpath = selected_files.at(0);
        }
    }


    if(sqlite_export_dest_dirpath.trimmed() == QString(""))
        return;

    ui->lineEdit_file_path->setText(sqlite_export_dest_dirpath);


}

void export_hashes_as_sqlite::slot_hide_loading_display_dialog_box(bool status)
{ // hide progress bar when it's done
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    bool_cancel_loading = status;
    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_export_rsync->pub_cancel_rsync_copy(Q_FUNC_INFO);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);


}
