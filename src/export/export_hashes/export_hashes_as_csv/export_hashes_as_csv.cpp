#include "export_hashes_as_csv.h"
#include "ui_export_hashes_as_csv.h"

export_hashes_as_csv::export_hashes_as_csv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_hashes_as_csv)
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
    setWindowTitle("Export Hashes - CSV");


}

export_hashes_as_csv::~export_hashes_as_csv()
{
    delete ui;
}

void export_hashes_as_csv::pub_set_essential()
{
    ui->lineEdit_file_path->clear();
    ui->lineEdit_file_name->clear();
}

void export_hashes_as_csv::pub_set_export_file_details(QList<struct_global_export_as_hashes_details>& details_list)
{
    export_as_vic_details_list = details_list;
}

void export_hashes_as_csv::pub_start_export_hashes_as_csv()
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(false);
    display_loading_progress_bar_non_cancelable_obj->show();

    if(!csv_export_dest_dirpath.endsWith("/"))
        csv_export_dest_dirpath.append("/");


    struct_file_hashes hashes_obj;

    QString source_count_name;

    QString source_count;

    QString time_format = "yyyy-MM-ddThh:mm:ss";

    hashes_struct_list.clear();
    QString recon_case_dir_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    QString available_dir_name = recon_static_functions::get_available_filename(recon_case_dir_str,csv_export_dest_dirpath, Q_FUNC_INFO);
    csv_export_dest_dirpath = recon_static_functions::get_complete_file_path(csv_export_dest_dirpath, available_dir_name, Q_FUNC_INFO);

    QDir dir;
    dir.mkpath(csv_export_dest_dirpath);



    int tmp_cnt = 0;
    for(int count = 0; count < export_as_vic_details_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
            break;

        struct_global_export_as_hashes_details export_as_vic_details_obj =  export_as_vic_details_list.at(count);
        source_count_name = export_as_vic_details_obj.source_count_name;
        struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString cmplt_filepath = export_as_vic_details_obj.complete_file_path;
        QString display_filepath = export_as_vic_details_obj.display_filepath;
        hashes_obj = calculate_hashes(cmplt_filepath);

        QString  source_file_path = QFileInfo(hashes_obj.filepath).filePath();

        QString src_filename = QFileInfo(hashes_obj.filepath).fileName();

        QString export_dir_path = csv_export_dest_dirpath + "/" +source_count_name + "/";
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

    write_data_in_csv_file();
    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_message_dialog->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
    obj_message_dialog->show();

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

void export_hashes_as_csv::write_data_in_csv_file()
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);



    QString file_name;
    if(ui->lineEdit_file_name->text().isEmpty())
    {
        file_name = ui->lineEdit_file_name->placeholderText().trimmed();
    }
    else
    {
        file_name = ui->lineEdit_file_name->text().trimmed();
    }
    destination_csv_file_path = csv_export_dest_dirpath + "/" + file_name + ".csv";


    QFile csv_file(destination_csv_file_path);
    if(!csv_file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        recon_static_functions::app_debug(" File Opened -----  Failed ----" + csv_file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error -" + csv_file.errorString() ,Q_FUNC_INFO);
        return ;
    }

    csv_file.write("\n");

    QStringList case_name_list;
    case_name_list << "Case Name" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    QString case_name = case_name_list.join(",");
    csv_file.write(case_name.toLocal8Bit());
    csv_file.write("\n");

    QStringList case_ID_list;
    case_ID_list << "Case ID" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString();
    QString case_id = case_ID_list.join(",");
    csv_file.write(case_id.toLocal8Bit());
    csv_file.write("\n");

    QStringList case_location_list;
    case_location_list << "Case Location" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString();
    QString case_location = case_location_list.join(",");
    csv_file.write(case_location.toLocal8Bit());
    csv_file.write("\n");

    QStringList case_notes_list;
    case_notes_list << "Case Notes" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString();
    QString case_notes = case_notes_list.join(",");
    csv_file.write(case_notes.toLocal8Bit());
    csv_file.write("\n");

    QStringList agency_name_list;
    agency_name_list << "Agency Name" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString();
    QString agency_name = agency_name_list.join(",");
    csv_file.write(agency_name.toLocal8Bit());
    csv_file.write("\n");

    QStringList agency_address_list;
    agency_address_list << "Agency Address" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString();
    QString agency_address = agency_address_list.join(",");
    csv_file.write(agency_address.toLocal8Bit());
    csv_file.write("\n");

    QStringList examiner_name_list;
    examiner_name_list << "Examiner Name" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString();
    QString examiner_name = examiner_name_list.join(",");
    csv_file.write(examiner_name.toLocal8Bit());
    csv_file.write("\n");


    QStringList examiner_phone_list;
    examiner_phone_list << "Examiner Phone" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString();
    QString examiner_phone = examiner_phone_list.join(",");
    csv_file.write(examiner_phone.toLocal8Bit());
    csv_file.write("\n");

    QStringList examiner_email_list;
    examiner_email_list << "Examiner Email" << global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString();
    QString examiner_email = examiner_email_list.join(",");
    csv_file.write(examiner_email.toLocal8Bit());
    csv_file.write("\n");

    QStringList application_name_list;
    application_name_list << "Application Name" << recon_static_functions::get_app_name();
    QString application_name = application_name_list.join(",");
    csv_file.write(application_name.toLocal8Bit());
    csv_file.write("\n");

    QStringList application_version_list;
    application_version_list << "Application Version" << recon_static_functions::get_app_version();
    QString application_version = application_version_list.join(",");
    csv_file.write(application_version.toLocal8Bit());

    csv_file.write("\n");
    csv_file.write("\n");
    csv_file.write("\n");
    csv_file.write("\n");



    QStringList data_header_list;

    data_header_list << "Sr No" << "File Name" << "File Size(Byte)" << "Modify Timestamp(UTC)" << "Change Timestamp(UTC)" << "Accessed Timestamp(UTC)"
                     << "Created Timestamp(UTC)" << "MD5 Hash" << "SHA1 Hash" << "File Path" << "Category" ;

    QString data_header_str = data_header_list.join(",");

    csv_file.write(data_header_str.toLocal8Bit());
    csv_file.write("\n");
    csv_file.write("\n");

    QStringList data_list;
    for(int ii = 0; ii < hashes_struct_list.size(); ii++)
    {

        if(bool_cancel_loading)
            break;

        struct_file_hashes st_hashes_obj = hashes_struct_list.at(ii);
        QString file_path = st_hashes_obj.filepath;
        QFileInfo info(file_path);
        data_list.clear();
        data_list << QString::number(ii + 1);
        data_list << st_hashes_obj.display_filename;
        data_list << QString::number(info.size());
        data_list << st_hashes_obj.modify_time;
        data_list << st_hashes_obj.change_time;
        data_list << st_hashes_obj.access_time;
        data_list << st_hashes_obj.create_time;
        data_list << st_hashes_obj.md5_hash;
        data_list << st_hashes_obj.sha1_hash;
        data_list << st_hashes_obj.display_filepath;
        data_list << st_hashes_obj.source_count;

        QString data_str = data_list.join(",");
        csv_file.write("\n");
        csv_file.write(data_str.toLocal8Bit());
    }


    csv_file.flush();
    csv_file.close();
    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

export_hashes_as_csv::struct_file_hashes export_hashes_as_csv::calculate_hashes(QString file_path)
{
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

void export_hashes_as_csv::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;

    if(bool_message_box_yes_button_clicked)
    {
        point_to_export_dir(destination_csv_file_path);
    }
}

void export_hashes_as_csv::point_to_export_dir(QString path)
{
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void export_hashes_as_csv::on_pushButton_export_clicked()
{
    if(ui->lineEdit_file_path->text().isEmpty())
    {
        obj_message_dialog->pub_set_message(MACRO_MSG_WARNING, "Select Output Directory");
        obj_message_dialog->show();
        return;
    }
    hide();

    emit signal_export_button_clicked();
}

void export_hashes_as_csv::on_pushButton_cancel_clicked()
{
    hide();
}

void export_hashes_as_csv::on_pushButton_select_clicked()
{
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            csv_export_dest_dirpath = selected_files.at(0);
        }
    }
    if(csv_export_dest_dirpath.trimmed() == QString(""))
        return;

    ui->lineEdit_file_path->setText(csv_export_dest_dirpath);

}

void export_hashes_as_csv::slot_hide_loading_display_dialog_box(bool status)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    bool_cancel_loading = status;
    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_export_rsync->pub_cancel_rsync_copy(Q_FUNC_INFO);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);


}
