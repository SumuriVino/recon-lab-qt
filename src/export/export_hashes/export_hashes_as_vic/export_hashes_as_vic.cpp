#include "export_hashes_as_vic.h"
#include "ui_export_hashes_as_vic.h"

export_hashes_as_vic::export_hashes_as_vic(QWidget *parent) : QDialog(parent),
    ui(new Ui::export_hashes_as_vic)

{
    ui->setupUi(this);

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(parent);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    recon_helper_standard_obj = new recon_helper_standard(this);

    bool_message_box_yes_button_clicked = false;
    obj_message_dialog = new message_dialog;
    connect(obj_message_dialog,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    obj_export_rsync = new export_rsync(this);
    ui->lineEdit_file_path->setReadOnly(true);
    setWindowTitle("Export Hashes - VIC");

    QString place_hldr_txt = QDir(getenv("HOME") + QString("/Desktop")).absolutePath();
    ui->lineEdit_file_path->setPlaceholderText(place_hldr_txt);

}
export_hashes_as_vic::~export_hashes_as_vic()
{
    delete ui;
}

void export_hashes_as_vic::pub_set_essential()
{
    ui->lineEdit_file_path->clear();
    ui->lineEdit_file_name->clear();
}

void export_hashes_as_vic::pub_set_vic_version(QString str)
{
    vic_version = str.trimmed();
}

void export_hashes_as_vic::pub_set_vic_export_dir_path(QString str)
{
    vic_export_dir_filepath = str.trimmed();
}

void export_hashes_as_vic::pub_set_export_file_details(QList<struct_global_export_as_hashes_details>& details_list)
{
    export_as_vic_details_list = details_list;
}

void export_hashes_as_vic::prepare_essentials()
{
    application_name    = recon_static_functions::get_app_name();
    application_version = recon_static_functions::get_app_version();

    vic_data_models_link = QString(MACRO_VIC_Data_Model_Link);
    vic_extn = QString(MACRO_VIC_Data_Model_Extension);


    agency_name    = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString();
    agency_address = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString();
    examiner_name  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString();
    examiner_phone = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString();
    examiner_email = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString();

    case_number = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString();
    case_name   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    case_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString();
    case_notes  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString();
}

void export_hashes_as_vic::pub_start_vic_export_general()
{
    recon_static_functions::app_debug(" -Start", Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QString recon_case_dir_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    QString available_dir_name = recon_static_functions::get_available_filename(recon_case_dir_str,vic_export_dir_filepath, Q_FUNC_INFO);
    vic_export_dir_filepath    = recon_static_functions::get_complete_file_path(vic_export_dir_filepath, available_dir_name, Q_FUNC_INFO);

    if(!create_dir_path(vic_export_dir_filepath))
    {
        recon_static_functions::app_debug("=====vic_export_dir_filepath==Creation -----FAILED-----:" + vic_export_dir_filepath, Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    QString vic_export_files_dir_filepath  = vic_export_dir_filepath + QString("/Files.") + recon_case_dir_str;
    if(!create_dir_path(vic_export_files_dir_filepath))
    {
        recon_static_functions::app_debug("=====vic_export_files_dir_filepath==Creation -----FAILED-----:" + vic_export_files_dir_filepath, Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    vic_json_file_path = vic_export_dir_filepath + "/" + recon_case_dir_str + ".json";

    QFile file_json(vic_json_file_path);
    if(!file_json.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("=====vic_export_json_filepath==File Open -----FAILED-----:" + file_json.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("=====Error===:" + file_json.errorString(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    prepare_essentials();

    ///clear previous data
    json_root_object = QJsonObject();;


    QMimeType var;
    QMimeDatabase obj;
    struct_file_hashes hashes_obj;

    QString source_count_name;

    QString source_count;

    QString time_format = "yyyy-MM-ddThh:mm:ss";

    hashes_struct_list.clear();

    int tmp_cnt = 0;
    for(int count = 0; count < export_as_vic_details_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_global_export_as_hashes_details export_as_vic_details_obj =  export_as_vic_details_list.at(count);
        source_count_name = export_as_vic_details_obj.source_count_name;
        struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString cmplt_filepath = export_as_vic_details_obj.complete_file_path;
        var = obj.mimeTypeForFile(cmplt_filepath);
        QString mime_type_for_file = var.name();
        QString display_filepath = export_as_vic_details_obj.display_filepath;

        bool bool_file_hash_found = false;
        if(recon_helper_standard_obj->check_is_it_image_file(cmplt_filepath,display_filepath, Q_FUNC_INFO))
        {
            hashes_obj = calculate_hashes(cmplt_filepath);

            if(hashes_obj.md5_hash.trimmed().isEmpty())
                continue;

            bool_file_hash_found = true;
            source_count = export_as_vic_details_obj.source_count_name;
        }
        else if(mime_type_for_file.startsWith("video/"))
        {
            hashes_obj = calculate_hashes(cmplt_filepath);

            if(hashes_obj.md5_hash.trimmed().isEmpty())
                continue;

            bool_file_hash_found = true;
            source_count = export_as_vic_details_obj.source_count_name;
        }


        if(bool_file_hash_found)
        {
            if(source_count.startsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag)))
                source_count.remove(0,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).size());

            QString first_level_dir_name  = QString(hashes_obj.md5_hash.at(0))  + QString(hashes_obj.md5_hash.at(1));
            QString second_level_dir_name = QString(hashes_obj.md5_hash.at(2))  + QString(hashes_obj.md5_hash.at(3));
            QString file_export_dir_path  = vic_export_files_dir_filepath + "/" + source_count + "/" + first_level_dir_name + "/" + second_level_dir_name;

            if(!create_dir_path(file_export_dir_path))
            {
                recon_static_functions::app_debug("=====file_export_path==Creation -----FAILED-----:" + file_export_dir_path, Q_FUNC_INFO);
                continue;
            }

            QString file_export_path = file_export_dir_path + "/" + hashes_obj.md5_hash + QString(".") + QFileInfo(hashes_obj.filepath).suffix();
            if(file_export_path.endsWith("."))
                file_export_path.chop(QString(".").size());

            if(struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
            {
                QString display_filename = export_as_vic_details_obj.display_filename;
                QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(display_filename,file_export_dir_path,Q_FUNC_INFO);
                file_export_path = file_export_dir_path +  "/" + available_file_name;
            }

            if(!QFileInfo(file_export_path).exists())
            {
                if(!QFile::copy(hashes_obj.filepath,file_export_path))
                {
                    recon_static_functions::app_debug("=====Copy(source) -----FAILED-----:" + hashes_obj.filepath, Q_FUNC_INFO);
                    recon_static_functions::app_debug("=====Copy(dest) -----FAILED-----:" + file_export_path, Q_FUNC_INFO);
                    continue;
                }

                hashes_obj.display_filepath = export_as_vic_details_obj.display_filepath;
                hashes_obj.display_filename = export_as_vic_details_obj.display_filename;
                hashes_obj.source_type_internal = struct_target_source_info.source_type_internal;

                QString relative_filepath = hashes_obj.display_filepath;
                if(relative_filepath.startsWith("/"))
                    relative_filepath.remove(0,1);

                relative_filepath.replace("/", "\\");
                hashes_obj.relative_filepath = relative_filepath;
                hashes_obj.source_count = source_count;
                hashes_obj.mime_type = mime_type_for_file;

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
        }
    }

    write_data_in_json_format_according_to_version(vic_version);

    QJsonDocument json_doc(json_root_object);
    QByteArray file_data_arr = json_doc.toJson();

    file_json.write(file_data_arr);
    file_json.flush();
    file_json.close();

    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_message_dialog->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
    obj_message_dialog->show();

    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);

}

void export_hashes_as_vic::pub_start_vic_export_1_3()
{
    recon_static_functions::app_debug(" -Start", Q_FUNC_INFO);
    QStringList exported_file_path_list;
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString recon_case_dir_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    QString available_dir_name = recon_static_functions::get_available_filename(recon_case_dir_str,vic_export_dir_filepath, Q_FUNC_INFO);
    vic_export_dir_filepath    = recon_static_functions::get_complete_file_path(vic_export_dir_filepath, available_dir_name, Q_FUNC_INFO);

    if(!create_dir_path(vic_export_dir_filepath))
    {
        recon_static_functions::app_debug("=====vic_export_dir_filepath==Creation -----FAILED-----:" + vic_export_dir_filepath, Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QString axiom_vic_export_files_dir_path  = vic_export_dir_filepath + QString(MACRO_AXIOM_VIC_EXPORT_DIR_PATH);

    if(!create_dir_path(axiom_vic_export_files_dir_path))
    {
        recon_static_functions::app_debug("=====vic_export_files_dir_filepath==Creation -----FAILED-----:" + axiom_vic_export_files_dir_path, Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }
    QString tmp_file_path = axiom_vic_export_files_dir_path;

    tmp_file_path.remove("Attachment/");

    vic_json_file_path = tmp_file_path + recon_case_dir_str + ".json";



    QFile file_json(vic_json_file_path);
    if(!file_json.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("=====vic_export_json_filepath==File Open -----FAILED-----:" + file_json.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("=====Error===:" + file_json.errorString(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }
    prepare_essentials();
    ///clear previous data
    json_root_object = QJsonObject();

    QMimeType var;
    QMimeDatabase obj;
    struct_file_hashes hashes_obj;

    QString source_count;

    QString time_format = "yyyy-MM-ddThh:mm:ss";

    hashes_struct_list.clear();

    int tmp_cnt = 0;
    QString source_count_name;

    for(int count = 0; count < export_as_vic_details_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_global_export_as_hashes_details export_as_vic_details_obj =  export_as_vic_details_list.at(count);
        source_count_name =export_as_vic_details_obj.source_count_name;
        struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString cmplt_filepath = export_as_vic_details_obj.complete_file_path;
        var = obj.mimeTypeForFile(cmplt_filepath);
        QString mime_type_for_file = var.name();
        QString display_file_path = export_as_vic_details_obj.display_filepath;

        bool bool_file_hash_found = false;
        if(recon_helper_standard_obj->check_is_it_image_file(cmplt_filepath,display_file_path, Q_FUNC_INFO)
                || mime_type_for_file.startsWith("video/"))
        {
            hashes_obj = calculate_hashes(cmplt_filepath);

            if(hashes_obj.md5_hash.trimmed().isEmpty())
                continue;

            bool_file_hash_found = true;
            source_count = export_as_vic_details_obj.source_count_name;
        }
        else
        {
            continue;
        }


        QString  source_file_path = QFileInfo(hashes_obj.filepath).filePath();

        QString src_filename = QFileInfo(hashes_obj.filepath).fileName();

        QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(src_filename,axiom_vic_export_files_dir_path,Q_FUNC_INFO);

        QString destination_file_path = axiom_vic_export_files_dir_path + available_file_name;

        if(struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            QString display_filename = export_as_vic_details_obj.display_filename;
            QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(display_filename,axiom_vic_export_files_dir_path,Q_FUNC_INFO);
            destination_file_path = axiom_vic_export_files_dir_path + available_file_name;
        }

        QString exported_file_path = obj_export_rsync->pub_copy_file_using_rsync(source_file_path,destination_file_path, enum_global_rsync_args_type_links_and_recursive);

        exported_file_path_list << exported_file_path;

        hashes_obj.display_filepath = export_as_vic_details_obj.display_filepath;
        hashes_obj.display_filename = export_as_vic_details_obj.display_filename;
        hashes_obj.source_type_internal = struct_target_source_info.source_type_internal;

        QString relative_filepath = hashes_obj.display_filepath;
        if(relative_filepath.startsWith("/"))
            relative_filepath.remove(0,1);

        relative_filepath.replace("/", "\\");
        hashes_obj.relative_filepath = relative_filepath;

        hashes_obj.source_count = source_count;

        hashes_obj.mime_type = mime_type_for_file;

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
    write_data_in_json_format_according_to_version(vic_version);

    QJsonDocument json_doc(json_root_object);
    QByteArray file_data_arr = json_doc.toJson();

    file_json.write(file_data_arr);
    file_json.flush();
    file_json.close();

    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_message_dialog->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
    obj_message_dialog->show();

    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);

}

void export_hashes_as_vic::pub_start_vic_export_2_0()
{
    recon_static_functions::app_debug(" -Start", Q_FUNC_INFO);

    QStringList exported_file_path_list;
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    if(!vic_export_dir_filepath.endsWith("/"))
        vic_export_dir_filepath.append("/");

    QString file_name;
    if(ui->lineEdit_file_name->text().isEmpty())
    {
        file_name = ui->lineEdit_file_name->placeholderText().trimmed();
    }
    else
    {
        file_name = ui->lineEdit_file_name->text().trimmed();
    }

    QString recon_case_dir_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString();
    QString available_dir_name = recon_static_functions::get_available_filename(recon_case_dir_str,vic_export_dir_filepath, Q_FUNC_INFO);
    vic_export_dir_filepath    = recon_static_functions::get_complete_file_path(vic_export_dir_filepath, available_dir_name, Q_FUNC_INFO);

    if(!create_dir_path(vic_export_dir_filepath))
    {
        recon_static_functions::app_debug("=====vic_export_dir_filepath==Creation -----FAILED-----:" + vic_export_dir_filepath, Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QString axiom_vic_export_files_dir_path = vic_export_dir_filepath + QString(MACRO_AXIOM_VIC_EXPORT_DIR_PATH_FOR_VIC_2_0) + file_name + QString("/");

    if(!create_dir_path(axiom_vic_export_files_dir_path))
    {
        recon_static_functions::app_debug("=====vic_export_files_dir_filepath==Creation -----FAILED-----:" + axiom_vic_export_files_dir_path, Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QString tmp_file_path = axiom_vic_export_files_dir_path;
    tmp_file_path.remove(file_name);

    vic_json_file_path = tmp_file_path + file_name + ".json";

    QFile file_json(vic_json_file_path);
    if(!file_json.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("=====vic_export_json_filepath==File Open -----FAILED-----:" + file_json.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("=====Error===:" + file_json.errorString(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    prepare_essentials();

    ///clear previous data
    json_root_object = QJsonObject();

    QMimeType var;
    QMimeDatabase obj;
    struct_file_hashes hashes_obj;

    QString source_count;
    QString time_format = "yyyy-MM-ddThh:mm:ss";

    hashes_struct_list.clear();

    int tmp_cnt = 0;
    QString source_count_name;

    for(int count = 0; count < export_as_vic_details_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_global_export_as_hashes_details export_as_vic_details_obj = export_as_vic_details_list.at(count);
        source_count_name = export_as_vic_details_obj.source_count_name;
        struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString cmplt_filepath = export_as_vic_details_obj.complete_file_path;
        var = obj.mimeTypeForFile(cmplt_filepath);
        QString mime_type_for_file = var.name();
        QString display_file_path = export_as_vic_details_obj.display_filepath;

        bool bool_file_hash_found = false;
        if(recon_helper_standard_obj->check_is_it_image_file(cmplt_filepath,display_file_path, Q_FUNC_INFO)
                || mime_type_for_file.startsWith("video/"))
        {
            hashes_obj = calculate_hashes(cmplt_filepath);

            if(hashes_obj.md5_hash.trimmed().isEmpty())
                continue;

            bool_file_hash_found = true;
            source_count = export_as_vic_details_obj.source_count_name;
        }
        else
        {
            continue;
        }

        QString source_file_path = QFileInfo(hashes_obj.filepath).filePath();

        QString src_filename = QFileInfo(hashes_obj.filepath).fileName();

        QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(src_filename,axiom_vic_export_files_dir_path,Q_FUNC_INFO);

        QString destination_file_path = axiom_vic_export_files_dir_path + available_file_name;

        if(struct_target_source_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            QString display_filename = export_as_vic_details_obj.display_filename;
            QString available_file_name = recon_static_functions::get_available_filename_as_last_braces(display_filename,axiom_vic_export_files_dir_path,Q_FUNC_INFO);
            destination_file_path = axiom_vic_export_files_dir_path + available_file_name;
        }

        QString exported_file_path = obj_export_rsync->pub_copy_file_using_rsync(source_file_path,destination_file_path, enum_global_rsync_args_type_links_and_recursive);

        exported_file_path_list << exported_file_path;

        hashes_obj.display_filepath = export_as_vic_details_obj.display_filepath;
        hashes_obj.display_filename = export_as_vic_details_obj.display_filename;
        hashes_obj.source_type_internal = struct_target_source_info.source_type_internal;

        QString relative_filepath = hashes_obj.display_filepath;
        if(relative_filepath.startsWith("/"))
            relative_filepath.remove(0,1);

        relative_filepath.replace("/", "\\");
        hashes_obj.relative_filepath = relative_filepath;

        hashes_obj.source_count = source_count;

        hashes_obj.mime_type = mime_type_for_file;

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

    write_data_in_json_format_according_to_version(vic_version);

    QJsonDocument json_doc(json_root_object);
    QByteArray file_data_arr = json_doc.toJson();

    file_json.write(file_data_arr);
    file_json.flush();
    file_json.close();

    display_loading_progress_bar_non_cancelable_obj->hide();

    obj_message_dialog->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
    obj_message_dialog->show();

    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);

}

bool export_hashes_as_vic::create_dir_path(QString dir_path)
{
    if(dir_path.trimmed().isEmpty())
        return false;

    bool bool_dir_create_status = false;

    QDir dir(dir_path);
    if(!dir.exists())
        bool_dir_create_status = dir.mkpath(dir_path);
    else
        bool_dir_create_status = true;


    return bool_dir_create_status;
}

export_hashes_as_vic::struct_file_hashes export_hashes_as_vic::calculate_hashes(QString file_path)
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

void export_hashes_as_vic::write_data_in_json_format_according_to_version(QString version_str)
{
    QString start_with_version_str;
    if(version_str == QString("1.1") || version_str == QString("1.2") || version_str == QString("1.3"))
    {
        start_with_version_str = QString("http://github.com/ICMEC/ProjectVic/DataModels/") + version_str + QString(".xml#Cases");
        json_root_object.insert("odata.metadata", QJsonValue::fromVariant(start_with_version_str));

        if(version_str == QString("1.1"))
            export_hashes_for_vic_version_1_1();
        else if(version_str == QString("1.2"))
            export_hashes_for_vic_version_1_2();
        else if(version_str == QString("1.3"))
            export_hashes_for_vic_version_1_3();

    }
    else if(version_str == QString("2.0"))
    {
        start_with_version_str = QString("http://github.com/VICSDATAMODEL/ProjectVic/DataModels/") + version_str + QString(".xml/CUSTOM/$metadata#Media");
        json_root_object.insert("@odata.context", QJsonValue::fromVariant(start_with_version_str));

        export_hashes_for_vic_version_2_0();
    }
}

void export_hashes_as_vic::export_hashes_for_vic_version_1_1()
{
    QJsonArray json_media_array;

    int tmp_cnt = 0;
    for(int count = 0; count < hashes_struct_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_file_hashes hashes_obj = hashes_struct_list.at(count);

        QJsonObject json_media_key;
        QJsonArray empty_arr;

        QFileInfo info(hashes_obj.filepath);

        ///====================///
        json_media_key.insert("MediaSize", QJsonValue::fromVariant(QString::number(info.size())));
        if(hashes_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            json_media_key.insert("Name", QJsonValue::fromVariant(hashes_obj.display_filename));
        }
        else
        {
            json_media_key.insert("Name", QJsonValue::fromVariant(info.fileName()));
        }
        json_media_key.insert("MD5", QJsonValue::fromVariant(hashes_obj.md5_hash));
        json_media_key.insert("SHA1", QJsonValue::fromVariant(hashes_obj.sha1_hash));
        json_media_key.insert("MediaID", QJsonValue::fromVariant(count));
        ///====================///

        ///====================///
        json_media_key.insert("Exifs", empty_arr);
        ///====================///


        ///====================///
        QString exifs_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/Exifs";
        json_media_key.insert("Exifs@odata.navigationLinkUrl", QJsonValue::fromVariant(exifs_nav_val_str));
        ///====================///

        ///====================///
        QJsonArray alternative_hashes_arr;
        QJsonObject json_obj_alternate_hash;

        QString alternate_hashes_nav_link_url_str = "AlternativeHashes(HashName='SHA1',MD5="+ hashes_obj.md5_hash + ")" + "/Media";
        json_obj_alternate_hash.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_link_url_str));
        json_obj_alternate_hash.insert("MD5", hashes_obj.md5_hash);
        json_obj_alternate_hash.insert("HashName", QJsonValue::fromVariant("SHA1"));
        json_obj_alternate_hash.insert("HashValue", QJsonValue::fromVariant(hashes_obj.sha1_hash));

        alternative_hashes_arr.push_back(json_obj_alternate_hash);

        json_media_key.insert("AlternativeHashes", alternative_hashes_arr);
        ///====================///


        ///====================///
        QString alternate_hashes_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/AlternativeHashes";
        json_media_key.insert("AlternativeHashes@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_val_str));
        ///====================///

        ///====================///
        QString odata_id_val_str = "Media(\""+ hashes_obj.md5_hash + "\")";
        json_media_key.insert("odata.id", QJsonValue::fromVariant(odata_id_val_str));
        json_media_array.push_back(json_media_key);
        ///====================///

    }



    ///keys
    QJsonArray json_obj_values_arr;
    QJsonObject json_obj_zero_key;
    json_obj_zero_key.insert("Media", json_media_array);

    ///=========Agency Details=========///
    json_obj_zero_key.insert("Agency", QJsonValue::fromVariant(agency_name));
    json_obj_zero_key.insert("Agency Address", QJsonValue::fromVariant(agency_address));
    json_obj_zero_key.insert("Examiner", QJsonValue::fromVariant(examiner_name));
    json_obj_zero_key.insert("Examiner Phone", QJsonValue::fromVariant(examiner_phone));
    json_obj_zero_key.insert("Examiner Email", QJsonValue::fromVariant(examiner_email));


    ///=========Case Details=========///
    json_obj_zero_key.insert("Case No.", QJsonValue::fromVariant(case_number));
    json_obj_zero_key.insert("Case Name", QJsonValue::fromVariant(case_name));
    json_obj_zero_key.insert("Location", QJsonValue::fromVariant(case_location));
    json_obj_zero_key.insert("Case Notes", QJsonValue::fromVariant(case_notes));

    json_obj_zero_key.insert("TotalMediaFiles", QJsonValue::fromVariant(QString::number(hashes_struct_list.size())));
    json_obj_zero_key.insert("SourceApplicationName", QJsonValue::fromVariant(application_name));
    json_obj_zero_key.insert("SourceApplicationVersion", QJsonValue::fromVariant(application_version));


    json_obj_values_arr.push_back(json_obj_zero_key);

    json_root_object.insert("value", json_obj_values_arr);

}

void export_hashes_as_vic::export_hashes_for_vic_version_1_2()
{
    QJsonArray json_media_array;
    int tmp_cnt = 0;
    for(int count = 0; count < hashes_struct_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_file_hashes hashes_obj = hashes_struct_list.at(count);

        QJsonObject json_media_key;
        QJsonArray empty_arr;

        QFileInfo info(hashes_obj.filepath);

        ///====================///
        json_media_key.insert("MediaSize", QJsonValue::fromVariant(QString::number(info.size())));
        json_media_key.insert("MD5", QJsonValue::fromVariant(hashes_obj.md5_hash));
        json_media_key.insert("SHA1", QJsonValue::fromVariant(hashes_obj.sha1_hash));
        json_media_key.insert("MediaID", QJsonValue::fromVariant(count));
        ///====================///


        ///====================///
        QJsonArray media_files_arr;
        QJsonObject json_obj_media_files;
        //        QString media_nav_link_url_str = "/MediaFiles(FileName='" + info.fileName() + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
        //                + hashes_obj.md5_hash + ")";

        //        json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));
        json_obj_media_files.insert("MD5", hashes_obj.md5_hash);

        if(hashes_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            QString media_nav_link_url_str = "/MediaFiles(FileName='" + hashes_obj.display_filename + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
                    + hashes_obj.md5_hash + ")";
            json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));

            json_media_key.insert("FileName", QJsonValue::fromVariant(hashes_obj.display_filename));
        }
        else
        {
            QString media_nav_link_url_str = "/MediaFiles(FileName='" + info.fileName() + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
                    + hashes_obj.md5_hash + ")";
            json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));

            json_media_key.insert("FileName", QJsonValue::fromVariant(info.fileName()));
        }

        json_obj_media_files.insert("FilePath", QJsonValue::fromVariant(hashes_obj.display_filepath));
        media_files_arr.push_back(json_obj_media_files);

        json_media_key.insert("MediaFiles", media_files_arr);
        ///====================///


        ///====================///
        QString media_files_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/MediaFiles";
        json_media_key.insert("MediaFiles@odata.navigationLinkUrl", QJsonValue::fromVariant(media_files_nav_val_str));
        ///====================///


        ///====================///
        json_media_key.insert("Exifs", empty_arr);
        ///====================///


        ///====================///
        QString exifs_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/Exifs";
        json_media_key.insert("Exifs@odata.navigationLinkUrl", QJsonValue::fromVariant(exifs_nav_val_str));
        ///====================///

        ///====================///
        QJsonArray alternative_hashes_arr;
        QJsonObject json_obj_alternate_hash;

        QString alternate_hashes_nav_link_url_str = "AlternativeHashes(HashName='SHA1',MD5="+ hashes_obj.md5_hash + ")" + "/Media";
        json_obj_alternate_hash.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_link_url_str));
        json_obj_alternate_hash.insert("MD5", hashes_obj.md5_hash);
        json_obj_alternate_hash.insert("HashName", QJsonValue::fromVariant("SHA1"));
        json_obj_alternate_hash.insert("HashValue", QJsonValue::fromVariant(hashes_obj.sha1_hash));

        alternative_hashes_arr.push_back(json_obj_alternate_hash);

        json_media_key.insert("AlternativeHashes", alternative_hashes_arr);
        ///====================///


        ///====================///
        QString alternate_hashes_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/AlternativeHashes";
        json_media_key.insert("AlternativeHashes@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_val_str));
        ///====================///

        ///====================///
        QString odata_id_val_str = "Media(\""+ hashes_obj.md5_hash + "\")";
        json_media_key.insert("odata.id", QJsonValue::fromVariant(odata_id_val_str));
        json_media_array.push_back(json_media_key);
        ///====================///

    }



    ///keys
    QJsonArray json_obj_values_arr;
    QJsonObject json_obj_zero_key;
    json_obj_zero_key.insert("Media", json_media_array);

    ///=========Agency Details=========///
    json_obj_zero_key.insert("Agency", QJsonValue::fromVariant(agency_name));
    json_obj_zero_key.insert("Agency Address", QJsonValue::fromVariant(agency_address));
    json_obj_zero_key.insert("Examiner", QJsonValue::fromVariant(examiner_name));
    json_obj_zero_key.insert("Examiner Phone", QJsonValue::fromVariant(examiner_phone));
    json_obj_zero_key.insert("Examiner Email", QJsonValue::fromVariant(examiner_email));


    ///=========Case Details=========///
    json_obj_zero_key.insert("Case No.", QJsonValue::fromVariant(case_number));
    json_obj_zero_key.insert("Case Name", QJsonValue::fromVariant(case_name));
    json_obj_zero_key.insert("Location", QJsonValue::fromVariant(case_location));
    json_obj_zero_key.insert("Case Notes", QJsonValue::fromVariant(case_notes));

    json_obj_zero_key.insert("TotalMediaFiles", QJsonValue::fromVariant(QString::number(hashes_struct_list.size())));
    json_obj_zero_key.insert("SourceApplicationName", QJsonValue::fromVariant(application_name));
    json_obj_zero_key.insert("SourceApplicationVersion", QJsonValue::fromVariant(application_version));


    json_obj_values_arr.push_back(json_obj_zero_key);

    json_root_object.insert("value", json_obj_values_arr);

}

void export_hashes_as_vic::export_hashes_for_vic_version_1_3()
{
    QJsonArray json_media_array;
    int tmp_cnt = 0;
    for(int count = 0; count < hashes_struct_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_file_hashes hashes_obj = hashes_struct_list.at(count);

        QJsonObject json_media_key;
        QJsonArray empty_arr;

        QFileInfo info(hashes_obj.filepath);

        ///====================///
        json_media_key.insert("Change", QJsonValue::fromVariant(hashes_obj.change_time));
        json_media_key.insert("Written", QJsonValue::fromVariant(hashes_obj.modify_time));
        json_media_key.insert("Accessed", QJsonValue::fromVariant(hashes_obj.access_time));
        json_media_key.insert("Created", QJsonValue::fromVariant(hashes_obj.create_time));
        json_media_key.insert("MimeType", QJsonValue::fromVariant(hashes_obj.mime_type));
        json_media_key.insert("RelativeFilePath", QJsonValue::fromVariant(hashes_obj.relative_filepath));
        json_media_key.insert("Category", QJsonValue::fromVariant(hashes_obj.source_count));
        json_media_key.insert("MD5", QJsonValue::fromVariant(hashes_obj.md5_hash));
        json_media_key.insert("SHA1", QJsonValue::fromVariant(hashes_obj.sha1_hash));
        json_media_key.insert("MediaSize", QJsonValue::fromVariant(QString::number(info.size())));
        json_media_key.insert("MediaID", QJsonValue::fromVariant(count));
        ///====================///


        ///====================///
        QJsonArray media_files_arr;
        QJsonObject json_obj_media_files;
        //        QString media_nav_link_url_str = "/MediaFiles(FileName='" + info.fileName() + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
        //                + hashes_obj.md5_hash + ")";

        //        json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));
        json_obj_media_files.insert("MD5", hashes_obj.md5_hash);

        if(hashes_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            json_media_key.insert("FileName", QJsonValue::fromVariant(hashes_obj.display_filename));
            QString media_nav_link_url_str = "/MediaFiles(FileName='" + hashes_obj.display_filename + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
                    + hashes_obj.md5_hash + ")";
            json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));
        }
        else
        {
            json_media_key.insert("FileName", QJsonValue::fromVariant(info.fileName()));
            QString media_nav_link_url_str = "/MediaFiles(FileName='" + info.fileName() + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
                    + hashes_obj.md5_hash + ")";
            json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));
        }

        json_obj_media_files.insert("FilePath", QJsonValue::fromVariant(hashes_obj.display_filepath));
        json_obj_media_files.insert("SourceID", QJsonValue::fromVariant(hashes_obj.display_filepath));
        media_files_arr.push_back(json_obj_media_files);

        json_media_key.insert("MediaFiles", media_files_arr);
        ///====================///


        ///====================///
        QString media_files_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/MediaFiles";
        json_media_key.insert("MediaFiles@odata.navigationLinkUrl", QJsonValue::fromVariant(media_files_nav_val_str));
        ///====================///


        ///====================///
        json_media_key.insert("Exifs", empty_arr);
        ///====================///


        ///====================///
        QString exifs_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/Exifs";
        json_media_key.insert("Exifs@odata.navigationLinkUrl", QJsonValue::fromVariant(exifs_nav_val_str));
        ///====================///

        ///====================///
        QJsonArray alternative_hashes_arr;
        QJsonObject json_obj_alternate_hash;

        QString alternate_hashes_nav_link_url_str = "AlternativeHashes(HashName='SHA1',MD5="+ hashes_obj.md5_hash + ")" + "/Media";
        json_obj_alternate_hash.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_link_url_str));
        json_obj_alternate_hash.insert("MD5", hashes_obj.md5_hash);
        json_obj_alternate_hash.insert("HashName", QJsonValue::fromVariant("SHA1"));
        json_obj_alternate_hash.insert("HashValue", QJsonValue::fromVariant(hashes_obj.sha1_hash));

        alternative_hashes_arr.push_back(json_obj_alternate_hash);

        json_media_key.insert("AlternativeHashes", alternative_hashes_arr);
        ///====================///


        ///====================///
        QString alternate_hashes_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/AlternativeHashes";
        json_media_key.insert("AlternativeHashes@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_val_str));
        ///====================///

        ///====================///
        QString odata_id_val_str = "Media(\""+ hashes_obj.md5_hash + "\")";
        json_media_key.insert("odata.id", QJsonValue::fromVariant(odata_id_val_str));
        json_media_array.push_back(json_media_key);
        ///====================///

    }



    ///keys
    QJsonArray json_obj_values_arr;
    QJsonObject json_obj_zero_key;
    json_obj_zero_key.insert("Media", json_media_array);

    ///=========Agency Details=========///
    json_obj_zero_key.insert("Agency", QJsonValue::fromVariant(agency_name));
    json_obj_zero_key.insert("Agency Address", QJsonValue::fromVariant(agency_address));
    json_obj_zero_key.insert("Examiner", QJsonValue::fromVariant(examiner_name));
    json_obj_zero_key.insert("Examiner Phone", QJsonValue::fromVariant(examiner_phone));
    json_obj_zero_key.insert("Examiner Email", QJsonValue::fromVariant(examiner_email));


    ///=========Case Details=========///
    json_obj_zero_key.insert("Case No.", QJsonValue::fromVariant(case_number));
    json_obj_zero_key.insert("Case Name", QJsonValue::fromVariant(case_name));
    json_obj_zero_key.insert("Location", QJsonValue::fromVariant(case_location));
    json_obj_zero_key.insert("Case Notes", QJsonValue::fromVariant(case_notes));

    json_obj_zero_key.insert("TotalMediaFiles", QJsonValue::fromVariant(QString::number(hashes_struct_list.size())));
    json_obj_zero_key.insert("SourceApplicationName", QJsonValue::fromVariant(application_name));
    json_obj_zero_key.insert("SourceApplicationVersion", QJsonValue::fromVariant(application_version));


    json_obj_values_arr.push_back(json_obj_zero_key);

    json_root_object.insert("value", json_obj_values_arr);

}

void export_hashes_as_vic::export_hashes_for_vic_version_2_0()
{
    QJsonArray json_media_array;
    int tmp_cnt = 0;
    for(int count = 0; count < hashes_struct_list.size(); count++)
    {
        tmp_cnt++;
        if(tmp_cnt % 50 == 0)
        {
            tmp_cnt = 0;
            QCoreApplication::processEvents();
        }

        struct_file_hashes hashes_obj = hashes_struct_list.at(count);

        QJsonObject json_media_key;
        QJsonArray empty_arr;

        QFileInfo info(hashes_obj.filepath);

        ///====================///

        json_media_key.insert("MimeType", QJsonValue::fromVariant(hashes_obj.mime_type));
        json_media_key.insert("RelativeFilePath", QJsonValue::fromVariant(hashes_obj.relative_filepath));
        json_media_key.insert("Category", QJsonValue::fromVariant(hashes_obj.source_count));
        json_media_key.insert("MD5", QJsonValue::fromVariant(hashes_obj.md5_hash));
        json_media_key.insert("SHA1", QJsonValue::fromVariant(hashes_obj.sha1_hash));
        json_media_key.insert("MediaSize", QJsonValue::fromVariant(QString::number(info.size())));
        json_media_key.insert("MediaID", QJsonValue::fromVariant(count));
        ///====================///


        ///====================///
        QJsonArray media_files_arr;
        QJsonObject json_obj_media_files;

        json_obj_media_files.insert("FileName", QJsonValue::fromVariant(info.fileName()));
        json_obj_media_files.insert("FilePath", QJsonValue::fromVariant(hashes_obj.display_filepath));

        if(hashes_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
        {
            json_media_key.insert("FileName", QJsonValue::fromVariant(hashes_obj.display_filename));
            QString media_nav_link_url_str = "/MediaFiles(FileName='" + hashes_obj.display_filename + "',FilePath='"+ hashes_obj.display_filepath + "',MD5="
                    + hashes_obj.md5_hash + ")";
            json_obj_media_files.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(media_nav_link_url_str));
        }
        else
        {
            json_obj_media_files.insert("Change", QJsonValue::fromVariant(hashes_obj.change_time));
            json_obj_media_files.insert("Written", QJsonValue::fromVariant(hashes_obj.modify_time));
            json_obj_media_files.insert("Accessed", QJsonValue::fromVariant(hashes_obj.access_time));
            json_obj_media_files.insert("Created", QJsonValue::fromVariant(hashes_obj.create_time));
            json_obj_media_files.insert("MD5", hashes_obj.md5_hash);

        }

        media_files_arr.push_back(json_obj_media_files);
        json_media_key.insert("MediaFiles", media_files_arr);
        ///====================///


        ///====================///

        QString media_files_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/MediaFiles";
        json_media_key.insert("MediaFiles@odata.navigationLinkUrl", QJsonValue::fromVariant(media_files_nav_val_str));

        ///====================///


        ///====================///
        json_media_key.insert("Exifs", empty_arr);
        ///====================///


        ///====================///
        QString exifs_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/Exifs";
        json_media_key.insert("Exifs@odata.navigationLinkUrl", QJsonValue::fromVariant(exifs_nav_val_str));

        ///====================///


        ///====================///
        QJsonArray alternative_hashes_arr;
        QJsonObject json_obj_alternate_hash;

        QString alternate_hashes_nav_link_url_str = "AlternativeHashes(HashName='SHA1',MD5="+ hashes_obj.md5_hash + ")" + "/Media";
        json_obj_alternate_hash.insert("Media@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_link_url_str));
        json_obj_alternate_hash.insert("MD5", hashes_obj.md5_hash);
        json_obj_alternate_hash.insert("HashName", QJsonValue::fromVariant("SHA1"));
        json_obj_alternate_hash.insert("HashValue", QJsonValue::fromVariant(hashes_obj.sha1_hash));

        alternative_hashes_arr.push_back(json_obj_alternate_hash);

        json_media_key.insert("AlternativeHashes", alternative_hashes_arr);
        ///====================///


        ///====================///

        QString alternate_hashes_nav_val_str = "/Media("+ hashes_obj.md5_hash + ")" + "/AlternativeHashes";

        json_media_key.insert("AlternativeHashes@odata.navigationLinkUrl", QJsonValue::fromVariant(alternate_hashes_nav_val_str));
        ///====================///


        ///====================///
        QString odata_id_val_str = "Media(\""+ hashes_obj.md5_hash + "\")";
        json_media_key.insert("odata.id", QJsonValue::fromVariant(odata_id_val_str));
        json_media_array.push_back(json_media_key);
        ///====================///

    }

    ///keys
    QJsonArray json_obj_values_arr;
    QJsonObject json_obj_zero_key;
    json_obj_zero_key.insert("Media", json_media_array);

    ///=========Agency Details=========///
    json_obj_zero_key.insert("Agency", QJsonValue::fromVariant(agency_name));
    json_obj_zero_key.insert("Agency Address", QJsonValue::fromVariant(agency_address));
    json_obj_zero_key.insert("Examiner", QJsonValue::fromVariant(examiner_name));
    json_obj_zero_key.insert("Examiner Phone", QJsonValue::fromVariant(examiner_phone));
    json_obj_zero_key.insert("Examiner Email", QJsonValue::fromVariant(examiner_email));

    ///=========Case Details=========///
    json_obj_zero_key.insert("Case No.", QJsonValue::fromVariant(case_number));
    json_obj_zero_key.insert("Case Name", QJsonValue::fromVariant(case_name));
    json_obj_zero_key.insert("Location", QJsonValue::fromVariant(case_location));
    json_obj_zero_key.insert("Case Notes", QJsonValue::fromVariant(case_notes));

    json_obj_zero_key.insert("TotalMediaFiles", QJsonValue::fromVariant(QString::number(hashes_struct_list.size())));
    json_obj_zero_key.insert("SourceApplicationName", QJsonValue::fromVariant(application_name));
    json_obj_zero_key.insert("SourceApplicationVersion", QJsonValue::fromVariant(application_version));

    json_obj_values_arr.push_back(json_obj_zero_key);

    json_root_object.insert("value", json_obj_values_arr);

}

void export_hashes_as_vic::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;

    if(bool_message_box_yes_button_clicked)
    {
        point_to_export_dir(vic_json_file_path);
    }
}
void export_hashes_as_vic::point_to_export_dir(QString path)
{
    QStringList arg;
    path.remove("Attachment/");
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}

void export_hashes_as_vic::on_pushButton_export_clicked()
{
    if(ui->lineEdit_file_path->text().isEmpty())
    {
        vic_export_dir_filepath = ui->lineEdit_file_path->placeholderText();
    }

    hide();

    emit signal_export_button_clicked();
}

void export_hashes_as_vic::on_pushButton_cancel_clicked()
{
    hide();
}

void export_hashes_as_vic::on_pushButton_select_clicked()
{
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            vic_export_dir_filepath = selected_files.at(0);
        }
    }

    if(vic_export_dir_filepath.trimmed().isEmpty())
        return;

    ui->lineEdit_file_path->setText(vic_export_dir_filepath);

}
