#include "file_system_post_launcher.h"
#include "ui_file_system_post_launcher.h"

file_system_post_launcher::file_system_post_launcher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_system_post_launcher)
{
    ui->setupUi(this);


    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_recived(bool)));


    QStringList header_list;
    header_list << "Source No."
                <<"Source Name"
               << MACRO_JobType_Apple_Metadata
               << MACRO_JobType_Exif_Metadata
               << MACRO_JobType_Signature_Analysis
               << MACRO_JobType_Hashes
               << MACRO_JobType_Face_Analysis
               << MACRO_JobType_Optical_Character_Recognition
               << MACRO_JobType_Skin_Tone_Detection
               << MACRO_JobType_Weapons
               << MACRO_JobType_Fire_Analysis
               << MACRO_JobType_Unified_Logs
               << "Verification"
                  //<< MACRO_JobType_Text_Indexing
               << ""  // For remove source
               << ""; // for blank column

    ui->tableWidget_sources->setColumnCount(header_list.size());
    ui->tableWidget_sources->setHorizontalHeaderLabels(header_list);
    ui->tableWidget_sources->horizontalHeader()->setStretchLastSection(true);

    //ui->tableWidget_sources->hideColumn(enum_column_position_Source_Count_Name);

    recon_helper_standard_obj = new recon_helper_standard(this);

    verify_source_obj = new verify_source(this);
    connect(verify_source_obj,SIGNAL(signal_source_verifed(QString)),this,SLOT(slot_source_verifed(QString)));

    connect(verify_source_obj,SIGNAL(signal_source_verification_started(QString)),this,SIGNAL(signal_verifying_source_start(QString)));
    connect(verify_source_obj,SIGNAL(signal_PBAR_source_verification_value(QString,QString,bool,qint64,int,bool)),this,SIGNAL(signal_PBAR_source_verifying_value(QString,QString,bool,qint64,int,bool)));
    connect(verify_source_obj,SIGNAL(signal_source_verification_finished(QString)),this,SIGNAL(signal_source_verfication_finished(QString)));
    connect(verify_source_obj,SIGNAL(signal_set_progress_bar_max_value(struct_global_task_type_and_title,int)),this,SIGNAL(signal_set_progress_bar_max_value(struct_global_task_type_and_title,int)));
    connect(verify_source_obj,SIGNAL(signal_source_verification_running_status(bool,QString)),this,SLOT(slot_source_verification_running_status(bool,QString)));
    connect(this,SIGNAL(signal_cancel_source_verfication(struct_global_task_type_and_title)),verify_source_obj,SLOT(slot_cancel_verifying_source(struct_global_task_type_and_title)));
    bool_source_is_verifying = false;


}

file_system_post_launcher::~file_system_post_launcher()
{
    delete ui;
}

void file_system_post_launcher::pub_display_source_info()
{ // display source info, set file system features on post launcher ui

    ui->tableWidget_sources->setRowCount(0);
    QString source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(source_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_dbOpen -------FAILED------ " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString command = QString("select source_count_name, os_scheme_display, ")
            + QString(MACRO_JobType_Apple_Metadata).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Exif_Metadata).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Signature_Analysis).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Hashes).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Face_Analysis).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Optical_Character_Recognition).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Skin_Tone_Detection).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Weapons).replace(" ", "_") + ",  "
            + QString(MACRO_JobType_Fire_Analysis).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Unified_Logs).replace(" ", "_") + " "
            + "from fs_status";

    QSqlQuery query(destination_db);

    if(!query.exec(command))
    {
        recon_static_functions::app_debug("query Open -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    bool bool_source_availble_to_verify = false;

    int row_count = 0;
    while(query.next())
    {
        QTableWidgetItem *item_source_name, *item_source_count_name, *item_apple_metadata, *item_exif_metadata, *item_signature_analysis, *item_blank;

        QTableWidgetItem  *item_hashes,*item_face_analysis,*item_optical_character_recognition, *item_skin_tone_detection, *item_weapons, *item_fire_analysis, *item_unified_logs;

        item_skin_tone_detection = new QTableWidgetItem;
        item_face_analysis = new QTableWidgetItem;
        item_optical_character_recognition = new QTableWidgetItem;
        item_source_name = new QTableWidgetItem;
        item_source_count_name = new QTableWidgetItem;
        item_apple_metadata = new QTableWidgetItem;
        item_exif_metadata = new QTableWidgetItem;
        item_signature_analysis = new QTableWidgetItem;
        item_hashes = new QTableWidgetItem;
        item_weapons = new QTableWidgetItem;
        item_fire_analysis = new QTableWidgetItem;
        item_unified_logs = new QTableWidgetItem;
        item_blank = new QTableWidgetItem;

        QString source_count_name = query.value(0).toString();
        item_source_count_name->setText(source_count_name);

        bool bool_fs_module_sprt = recon_helper_standard_obj->source_applicable_for_extensive_module(source_count_name);

        struct_GLOBAL_witness_info_source st_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString source_name = st_target_source_info.complete_source_name;
        item_source_name->setText(source_name);

        if(bool_fs_module_sprt)
        {
            if(query.value("Apple_Metadata").toString() == Macro_JobType_Completion_status_not_supported)
                item_apple_metadata->setText("Not Supported");
            else if(query.value("Apple_Metadata").toString() == Macro_JobType_Completion_status_done)
                item_apple_metadata->setText("Completed");
            else if(query.value("Apple_Metadata").toString() == Macro_JobType_Completion_status_not_done)
                item_apple_metadata->setCheckState(Qt::Unchecked);


            if(query.value("Exif_Metadata").toString() == Macro_JobType_Completion_status_not_supported)
                item_exif_metadata->setText("Not Supported");
            else if(query.value("Exif_Metadata").toString() == Macro_JobType_Completion_status_done)
                item_exif_metadata->setText("Completed");
            else if(query.value("Exif_Metadata").toString() == Macro_JobType_Completion_status_not_done)
                item_exif_metadata->setCheckState(Qt::Unchecked);


            if(query.value("Signature_Analysis").toString() == Macro_JobType_Completion_status_not_supported)
                item_signature_analysis->setText("Not Supported");
            else if(query.value("Signature_Analysis").toString() == Macro_JobType_Completion_status_done)
            {
                item_signature_analysis->setCheckState(Qt::Unchecked);
                item_signature_analysis->setText("Completed \n select to Re-Run");
            }
            else if(query.value("Signature_Analysis").toString() == Macro_JobType_Completion_status_not_done)
                item_signature_analysis->setCheckState(Qt::Unchecked);


            if(query.value("Hashes").toString() == Macro_JobType_Completion_status_not_supported)
                item_hashes->setText("Not Supported");
            else if(query.value("Hashes").toString() == Macro_JobType_Completion_status_done)
                item_hashes->setText("Completed");
            else if(query.value("Hashes").toString() == Macro_JobType_Completion_status_not_done)
                item_hashes->setCheckState(Qt::Unchecked);

            if(query.value("Face_Analysis").toString() == Macro_JobType_Completion_status_not_supported)
                item_face_analysis->setText("Not Supported");
            else if(query.value("Face_Analysis").toString() == Macro_JobType_Completion_status_done)
                item_face_analysis->setText("Completed");
            else if(query.value("Face_Analysis").toString() == Macro_JobType_Completion_status_not_done)
                item_face_analysis->setCheckState(Qt::Unchecked);

            if(query.value("Optical_Character_Recognition").toString() == Macro_JobType_Completion_status_not_supported)
                item_face_analysis->setText("Not Supported");
            else if(query.value("Optical_Character_Recognition").toString() == Macro_JobType_Completion_status_done)
                item_optical_character_recognition->setText("Completed");
            else if(query.value("Optical_Character_Recognition").toString() == Macro_JobType_Completion_status_not_done)
                item_optical_character_recognition->setCheckState(Qt::Unchecked);

            if(query.value("Skin_Tone_Detection").toString() == Macro_JobType_Completion_status_not_supported)
                item_skin_tone_detection->setText("Not Supported");
            else if(query.value("Skin_Tone_Detection").toString() == Macro_JobType_Completion_status_done)
                item_skin_tone_detection->setText("Completed");
            else if(query.value("Skin_Tone_Detection").toString() == Macro_JobType_Completion_status_not_done)
                item_skin_tone_detection->setCheckState(Qt::Unchecked);

            if(query.value("Weapons").toString() == Macro_JobType_Completion_status_not_supported)
                item_weapons->setText("Not Supported");
            else if(query.value("Weapons").toString() == Macro_JobType_Completion_status_done)
                item_weapons->setText("Completed");
            else if(query.value("Weapons").toString() == Macro_JobType_Completion_status_not_done)
                item_weapons->setCheckState(Qt::Unchecked);

            if(query.value("Fire_Analysis").toString() == Macro_JobType_Completion_status_not_supported)
                item_fire_analysis->setText("Not Supported");
            else if(query.value("Fire_Analysis").toString() == Macro_JobType_Completion_status_done)
                item_fire_analysis->setText("Completed");
            else if(query.value("Fire_Analysis").toString() == Macro_JobType_Completion_status_not_done)
                item_fire_analysis->setCheckState(Qt::Unchecked);

            if(query.value("Unified_Logs").toString() == Macro_JobType_Completion_status_not_supported)
                item_unified_logs->setText("Not Supported");
            else if(query.value("Unified_Logs").toString() == Macro_JobType_Completion_status_done)
                item_unified_logs->setText("Completed");
            else if(query.value("Unified_Logs").toString() == Macro_JobType_Completion_status_not_done)
                item_unified_logs->setCheckState(Qt::Unchecked);
        }
        else
        {
            item_apple_metadata->setCheckState(Qt::Unchecked);
            item_apple_metadata->setFlags(Qt::ItemIsEditable);

            item_exif_metadata->setCheckState(Qt::Unchecked);
            item_exif_metadata->setFlags(Qt::ItemIsEditable);

            item_signature_analysis->setCheckState(Qt::Unchecked);
            item_signature_analysis->setFlags(Qt::ItemIsEditable);

            item_hashes->setCheckState(Qt::Unchecked);
            item_hashes->setFlags(Qt::ItemIsEditable);

            item_face_analysis->setCheckState(Qt::Unchecked);
            item_face_analysis->setFlags(Qt::ItemIsEditable);

            item_optical_character_recognition->setCheckState(Qt::Unchecked);
            item_optical_character_recognition->setFlags(Qt::ItemIsEditable);

            item_skin_tone_detection->setCheckState(Qt::Unchecked);
            item_skin_tone_detection->setFlags(Qt::ItemIsEditable);

            item_weapons->setCheckState(Qt::Unchecked);
            item_weapons->setFlags(Qt::ItemIsEditable);

            item_fire_analysis->setCheckState(Qt::Unchecked);
            item_fire_analysis->setFlags(Qt::ItemIsEditable);

            item_unified_logs->setCheckState(Qt::Unchecked);
            item_unified_logs->setFlags(Qt::ItemIsEditable);

        }

        ui->tableWidget_sources->insertRow(row_count);


        bool is_source_nested = st_target_source_info.bool_source_nested;
        if(!is_source_nested)
        {
            QLabel *label_remove_source_link = new QLabel(this);
            label_remove_source_link->setText("<a href = "+ QString(source_count_name) + " > " + QString("Remove") + " </a>");
            QObject::connect(label_remove_source_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_remove_source_link_activated(QString)));
            ui->tableWidget_sources->setCellWidget(row_count, enum_column_position_Remove_Source, label_remove_source_link);
        }


        QString root_src_type = st_target_source_info.root_type;
        if(root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage)
        {
            if(!st_target_source_info.bool_source_nested && !st_target_source_info.bool_source_decompressed)
            {
                QString image_type = recon_static_functions::get_forensic_image_type(st_target_source_info.root_path_1);
                if(image_type == MACRO_Target_Disk_ImageType_Apple
                        || image_type == MACRO_Target_Disk_ImageType_RAW_FULL
                        || image_type == MACRO_Target_Disk_ImageType_RAW_SPLIT
                        || image_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL
                        || image_type == MACRO_Target_Disk_ImageType_AFF4)
                {
                    bool_source_availble_to_verify = true;
                    QLabel *label_verify_source_link = new QLabel(this);
                    label_verify_source_link->setText("<a href = "+ QString(source_count_name) + " > " + QString("Verify") + " </a>");
                    QObject::connect(label_verify_source_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_verify_source_link_activated(QString)));
                    ui->tableWidget_sources->setCellWidget(row_count, enum_column_position_Verify_Source, label_verify_source_link);
                }
            }
        }


        ui->tableWidget_sources->setItem(row_count, enum_column_position_Source_Count_Name, item_source_count_name);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Source_Name, item_source_name);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Apple_Metadata, item_apple_metadata);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Exif_Metadata, item_exif_metadata);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Signature_Analysis, item_signature_analysis);

        ui->tableWidget_sources->setItem(row_count, enum_column_position_Hashes, item_hashes);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Faces_Analysis, item_face_analysis);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Optical_Character_Recognition, item_optical_character_recognition);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Skin_Tone_Detection, item_skin_tone_detection);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Weapons,item_weapons);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Fire_Analysis, item_fire_analysis);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Unified_Logs, item_unified_logs);
        ui->tableWidget_sources->setItem(row_count, enum_column_position_Blank_Column, item_blank);


        if(!QFileInfo(st_target_source_info.virtual_source_path).exists())
        {
            item_source_count_name->setForeground(Qt::red);
            item_source_name->setForeground(Qt::red);

            ui->tableWidget_sources->item(row_count, enum_column_position_Apple_Metadata)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Exif_Metadata)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Signature_Analysis)->setFlags(Qt::ItemIsEditable);

            ui->tableWidget_sources->item(row_count, enum_column_position_Hashes)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Faces_Analysis)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Optical_Character_Recognition)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Skin_Tone_Detection)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Weapons)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Fire_Analysis)->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_sources->item(row_count, enum_column_position_Unified_Logs)->setFlags(Qt::ItemIsEditable);
        }

        row_count++;

    }


    if(row_count <= 1)
    {
        ui->tableWidget_sources->hideColumn(enum_column_position_Remove_Source);
    }
    else
        ui->tableWidget_sources->showColumn(enum_column_position_Remove_Source);


    if(bool_source_availble_to_verify)
        ui->tableWidget_sources->showColumn(enum_column_position_Verify_Source);
    else
        ui->tableWidget_sources->hideColumn(enum_column_position_Verify_Source);

    ui->tableWidget_sources->setColumnWidth(enum_column_position_Source_Name, 180);
    ui->tableWidget_sources->setColumnWidth(enum_column_position_Signature_Analysis, 120);
    ui->tableWidget_sources->setColumnWidth(enum_column_position_Optical_Character_Recognition, 200);
    ui->tableWidget_sources->setColumnWidth(enum_column_position_Skin_Tone_Detection, 150);

    ui->tableWidget_sources->setColumnWidth(enum_column_position_Blank_Column, 20);

    ui->tableWidget_sources->hideColumn(enum_column_position_Optical_Character_Recognition);
    ui->tableWidget_sources->hideColumn(enum_column_position_Skin_Tone_Detection);
    ui->tableWidget_sources->hideColumn(enum_column_position_Weapons);
    ui->tableWidget_sources->hideColumn(enum_column_position_Fire_Analysis);
    ui->tableWidget_sources->hideColumn(enum_column_position_Faces_Analysis);

}

void file_system_post_launcher::pub_set_recon_case_obj(RECON_case  *obj)
{
    recon_case_obj = obj;
}

void file_system_post_launcher::slot_verify_source_link_activated(QString src_cnt_name)
{ // verify source link on file system which will verify source weather it's still available or detelted
    root_source_name_str.clear();
    root_type_str.clear();
    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_name);
    root_source_name_str = struct_source_info.root_name;
    root_type_str = struct_source_info.root_type;

    if(bool_source_is_verifying)
    {
        if(verify_source_obj->pub_is_source_verified(src_cnt_name))
            return;
        else
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,global_lang_tr_obj.get_translated_string("'Verification' for '"+ current_verifying_root_name_str +"' is already running"));
            message_dialog_obj->show();
            return;
        }
        return;
    }

    verify_source_obj->pub_set_essentials();
    verify_source_obj->pub_set_source_count_name(src_cnt_name);
    verify_source_obj->pub_verify_source();
}

void file_system_post_launcher::slot_source_verification_running_status(bool status , QString source_count_name_str)
{ // slot of source verification running status
    bool_source_is_verifying = status;
    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
    current_verifying_root_name_str = struct_source_info.root_name;
}

void file_system_post_launcher::slot_source_verifed(QString data_str)
{ // slots of source verified link on post launcher ui
    QDialog *qdialog_verified_data = new QDialog(this);
    qdialog_verified_data->setWindowModality(Qt::ApplicationModal);

    qdialog_verified_data->setAttribute(Qt::WA_DeleteOnClose, true);
    qdialog_verified_data->setGeometry(400,400,550,200);
    qdialog_verified_data->setWindowTitle("Verification Details - " + root_source_name_str);


    if(data_str.contains("\n"))
        data_str.replace("\n" , "<br>");


    QTextEdit *text_edit = new QTextEdit(qdialog_verified_data);
    QHBoxLayout *hbox = new QHBoxLayout(qdialog_verified_data);

    hbox->addWidget(text_edit);
    qdialog_verified_data->setLayout(hbox);

    QPalette p = text_edit->palette();
    text_edit->setHtml(data_str);

    p.setColor(QPalette::Base, QColor(241, 241, 241));
    text_edit->setPalette(p);
    text_edit->setReadOnly(true);
    qdialog_verified_data->show();


}
void file_system_post_launcher::on_pushButton_start_clicked()
{ // start button clicked after selection of post launcher features checkboxes
    struct_global_file_system_post_launcher_job_info st_obj;

    for(int i = 0; i < ui->tableWidget_sources->rowCount(); i++)
    {
        QString source_count_name = ui->tableWidget_sources->item(i, enum_column_position_Source_Count_Name)->text();

        if(ui->tableWidget_sources->item(i, enum_column_position_Apple_Metadata)->checkState() == Qt::Checked)
            st_obj.source_list_apple_metadata << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Exif_Metadata)->checkState() == Qt::Checked)
            st_obj.source_list_exif_metadata << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Signature_Analysis)->checkState() == Qt::Checked)
            st_obj.source_list_signature_analysis << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Hashes)->checkState() == Qt::Checked)
            st_obj.source_list_hashes << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Faces_Analysis)->checkState() == Qt::Checked)
            st_obj.source_list_face_analysis << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Optical_Character_Recognition)->checkState() == Qt::Checked)
            st_obj.source_list_optical_character_recognition << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Skin_Tone_Detection)->checkState() == Qt::Checked)
            st_obj.source_list_skin_tone_detection << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Weapons)->checkState() == Qt::Checked)
            st_obj.source_list_weapons << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Fire_Analysis)->checkState() == Qt::Checked)
            st_obj.source_list_fire_analysis << source_count_name;

        if(ui->tableWidget_sources->item(i, enum_column_position_Unified_Logs)->checkState() == Qt::Checked)
            st_obj.source_list_unified_logs << source_count_name;

    }


    emit signal_file_system_post_launcher_job_list(st_obj);
}

void file_system_post_launcher::on_pushButton_cancel_clicked()
{
    hide();
}


void file_system_post_launcher::slot_remove_source_link_activated(QString src_cnt_name)
{ // remove source link activated

    struct_GLOBAL_witness_info_root struct_rt_info = global_witness_info_manager_class_obj->pub_get_root_structure_according_source_count_name(src_cnt_name);
    to_remove_root_count_name.clear();
    to_remove_root_count_name = struct_rt_info.root_count_name;

    QStringList all_src_cnt_name_list;
    all_src_cnt_name_list << struct_rt_info.source_count_name_all_QStringList;


    QString source_name;
    bool bool_extraction_completed = true;
    for(int ii =0; ii < all_src_cnt_name_list.size(); ii++)
    {
        if(!recon_case_obj->pub_is_source_extraction_completed(all_src_cnt_name_list.at(ii)))
        {
            bool_extraction_completed = false;
            break;
        }

        struct_GLOBAL_witness_info_source struct_source_info1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(all_src_cnt_name_list.at(ii));

        source_name.append(struct_source_info1.complete_source_name);
        source_name.append(",");
    }

    if(!bool_extraction_completed)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,global_lang_tr_obj.get_translated_string("Source can't be removed untill source(s) have not been added to case completely"));
        message_dialog_obj->show();
        return ;
    }


    if(source_name.trimmed().endsWith(","))
        source_name.chop(1);

    to_remove_source_count_name_list.clear();
    to_remove_source_count_name_list << all_src_cnt_name_list;


    QString info = QString("<font color= \"Red\"> Once a source is removed its result items can not be recovered. We strongly recommend you to restart " + recon_static_functions::get_app_name() + " after removing source.")
            + "'" + source_name + "' will be removed \n  Do you want to proceed? </font>";

    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO,info);
    message_dialog_obj->show();

}

void file_system_post_launcher::slot_message_box_yes_no_recived(bool status)
{ // ask yes/ no on source verification

    if(!status)
    {
        to_remove_source_count_name_list.clear();
        to_remove_root_count_name.clear();
    }
    else
    {
        /*=============:Note:=================
         * No need to unmount sources here, because sources will be automatically
         * unmounted on app close.
         */

        remove_source_info_from_result_custom_paths_1();
        remove_source_info_from_result_custom_paths_2();
        remove_source_info_from_result_data();
        remove_source_info_from_apfs_snapshots();
        remove_root_info_from_root_db_and_text_files();
        remove_source_info_from_source_db_and_text_files();
    }

    hide();
}


void file_system_post_launcher::remove_source_info_from_result_custom_paths_1()
{ // remove source info from everywhere once we click on source verification removal

    QString disk_images_db_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString()   + "disk_images.sqlite";
    QString mobile_backups_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";


    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString source_cnt_name = to_remove_source_count_name_list.at(ii);

        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);

        ///update status for disk images
        QString cmd_str = QString("Delete from tbl_sources_list Where source_count_name = ?");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_str, QStringList(source_cnt_name), disk_images_db_path, Q_FUNC_INFO);

        if(!struct_source_info.parent_source_count_name.trimmed().isEmpty())
        {
            cmd_str = QString("Update tbl_sources_list Set extraction_status_as_source = '0' Where source_count_name = ?");
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_str, QStringList(struct_source_info.parent_source_count_name), disk_images_db_path, Q_FUNC_INFO);
        }

        ///update status for mobile backups
        cmd_str = QString("Delete from tbl_backup_list Where source_count_name = ?");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_str, QStringList(source_cnt_name), mobile_backups_db_path, Q_FUNC_INFO);

        if(!struct_source_info.parent_source_count_name.trimmed().isEmpty())
        {
            cmd_str = QString("Update tbl_backup_list Set extraction_status_as_source = '0' Where source_count_name = ?");
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_str, QStringList(struct_source_info.parent_source_count_name), mobile_backups_db_path, Q_FUNC_INFO);
        }


        QString fs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_cnt_name;
        recon_static_functions::do_directory_blank_completely(fs_dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(fs_dir_path,Q_FUNC_INFO);

        QString fs_timeline_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "file_system_timeline_full_" + source_cnt_name + ".sqlite";
        recon_static_functions::remove_safely_blank_dir_OR_file(fs_timeline_db_path,Q_FUNC_INFO);

        //        QString sourcecount_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + source_cnt_name+".txt";
        //        recon_static_functions::remove_safely_blank_dir_OR_file(sourcecount_txt_file,Q_FUNC_INFO);


        //        QString sourceindex_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
        //        QString m_cmd = "Delete from source_index where table_name = ?";
        //        QStringList args;
        //        args << source_cnt_name;
        //        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, args, sourceindex_db, Q_FUNC_INFO);


        //=================//
        //        QString sourceindex_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_index.txt";
        //        QFile file(sourceindex_txt_file);
        //        if(!file.open(QIODevice::ReadOnly))
        //            continue ;

        //        QByteArray arr_read_line ;
        //        while(!file.atEnd())
        //        {
        //            QString m_line = file.readLine();
        //            if(m_line.contains(source_cnt_name))
        //                continue ;
        //            else
        //                arr_read_line.append(m_line);
        //        }
        //        file.close();
        //        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        //        {
        //            file.write(arr_read_line);

        //            file.flush();
        //            file.close();
        //        }
    }

}

void file_system_post_launcher::remove_source_info_from_result_custom_paths_2()
{ // same remove source info from other databases
    QString rslt_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    //============================ Android Backup Start ======================//
    QString andrd_indx_db_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString()   + "android_backup_index.sqlite";

    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString source_cnt_name = to_remove_source_count_name_list.at(ii);

        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);
        if(struct_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal
                && struct_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
        {
            continue;
        }

        QString cmd_tt = "select recon_file_infopath from tbl_index where source_count_name = ?";
        QString file_infopath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tt, QStringList(source_cnt_name) , 0, andrd_indx_db_path, Q_FUNC_INFO);

        QString bkp_path = recon_static_functions::get_complete_file_path(rslt_path, file_infopath, Q_FUNC_INFO);

        QProcess proc;
        QStringList args;
        args << "-a" << "l" << bkp_path;
        proc.execute("SetFile", args);

        recon_static_functions::do_directory_blank_completely(bkp_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(bkp_path, Q_FUNC_INFO);
        QString m_cmd = "Delete from tbl_index where source_count_name = ?";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, QStringList(source_cnt_name), andrd_indx_db_path, Q_FUNC_INFO);

    }
    //============================ Android Backup End ======================//

    //============================ iOS Backup Start ======================

    QString ios_indx_db_path  =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString()   + "ios_backup_index.sqlite";

    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString source_cnt_name = to_remove_source_count_name_list.at(ii);

        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);

        if(struct_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal
                && struct_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal
                && struct_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
        {
            continue;
        }

        QString cmd_tt = "select recon_file_infopath from tbl_index where source_count_name = ?";
        QString file_infopath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_tt, QStringList(source_cnt_name) , 0, ios_indx_db_path, Q_FUNC_INFO);

        QString bkp_path = recon_static_functions::get_complete_file_path(rslt_path, file_infopath, Q_FUNC_INFO);

        QProcess proc;
        QStringList args;
        args << "-a" << "l" << bkp_path;
        proc.execute("SetFile", args);

        recon_static_functions::do_directory_blank_completely(bkp_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(bkp_path, Q_FUNC_INFO);
        QString m_cmd = "Delete from tbl_index where source_count_name = ?";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, QStringList(source_cnt_name), ios_indx_db_path, Q_FUNC_INFO);

    }

    //    ============================ iOS Backup End ======================

    //============================ Extracted Content Start ======================//

    QString apple_log_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString).toString();
    QString apple_metadata_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();

    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString src_count_name = to_remove_source_count_name_list.at(ii);

        QString src_log_path = apple_log_dir_path + src_count_name;
        QString src_apl_mtdt_path = apple_metadata_path + src_count_name;

        if(QFileInfo(src_log_path).exists())
        {
            recon_static_functions::do_directory_blank_completely(src_log_path, Q_FUNC_INFO);
            recon_static_functions::remove_safely_blank_dir_OR_file(src_log_path,Q_FUNC_INFO);
        }

        // donot use as else if
        if(QFileInfo(src_apl_mtdt_path).exists())
        {
            recon_static_functions::do_directory_blank_completely(src_apl_mtdt_path, Q_FUNC_INFO);
            recon_static_functions::remove_safely_blank_dir_OR_file(src_apl_mtdt_path,Q_FUNC_INFO);
        }

    }
    //============================ Extracted Content End ======================//
    //===========================Temp Apple Metadata Start=======================//
    QString tmp_apple_metadata_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();
    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString src_count_name = to_remove_source_count_name_list.at(ii);
        QString tmp_rm_source_dir_path = tmp_apple_metadata_dir_path + src_count_name;
        recon_static_functions::do_directory_blank_completely(tmp_rm_source_dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(tmp_rm_source_dir_path,Q_FUNC_INFO);
    }
    //===========================Temp Apple Metadata End=======================//

    //===========================Temp Apple Timestamps Start=======================//
    QString tmp_apple_timestamp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString).toString();
    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString src_count_name = to_remove_source_count_name_list.at(ii);
        QString tmp_rm_source_dir_path = tmp_apple_timestamp_dir_path + src_count_name;
        recon_static_functions::do_directory_blank_completely(tmp_rm_source_dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(tmp_rm_source_dir_path,Q_FUNC_INFO);
    }
    //===========================Temp Apple Timestamps End=======================//

}
// all the remove function to remove the source and it's data from everywhere in result directory output
void file_system_post_launcher::remove_source_info_from_result_data()
{ // remove source info fromresult data

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QDirIterator dir_itr(result_dir_path, QDirIterator::Subdirectories);
    while(dir_itr.hasNext())
    {
        QString file_path = dir_itr.next();

        if(!file_path.endsWith(".sqlite"))
            continue;

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(file_path);
        if (!destination_db.open())
        {
            continue ;
        }

        QStringList table_list = destination_db.tables();
        for(int tt = 0 ; tt < table_list.size() ; tt++)
        {
            QString table_name = table_list.at(tt);

            for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
            {
                QString source_count_name = to_remove_source_count_name_list.at(ii);

                if(table_name == source_count_name)
                {
                    QString dl_command = "DROP table " + table_name;
                    recon_helper_standard_obj->execute_db_command_by_dbreference(dl_command, destination_db, Q_FUNC_INFO);
                    break;
                }

                if(destination_db.record(table_name).contains("source_count_name"))
                {
                    QString dl_command = "Delete from " +  table_name +" where source_count_name = ?";
                    QStringList args;
                    args << source_count_name;
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(dl_command, args,destination_db, Q_FUNC_INFO);
                }
            }
        }

        destination_db.close();

    }

    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString source_count_name = to_remove_source_count_name_list.at(ii);

        remove_ios_preview_cache_dir_from_result_on_source_remove(source_count_name);
    }


}

void file_system_post_launcher::remove_ios_preview_cache_dir_from_result_on_source_remove(QString source_count_name)
{
    QString cache_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString).toString() + source_count_name;
    QDir cache_dir(cache_dir_path);
    if(cache_dir.exists())
    {
        recon_static_functions::do_directory_blank_completely(cache_dir_path ,Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(cache_dir_path ,Q_FUNC_INFO);
    }
}


void file_system_post_launcher::remove_source_info_from_apfs_snapshots()
{

    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString source_count_name = to_remove_source_count_name_list.at(ii);

        global_recon_helper_singular_class_obj->pub_remove_info_from_snapshots_plist_for_source(source_count_name);
    }

}


void file_system_post_launcher::on_pushButton_refresh_clicked()
{ // recfresh button
    pub_display_source_info();

}

void file_system_post_launcher::remove_root_info_from_root_db_and_text_files()
{ //
    //==================================Remove entry from roots_info.sqlite_start============================
    QString root_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";

    QString m_cmd = "Delete from tbl_root_index where root_count_name = ?";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, QStringList(to_remove_root_count_name), root_db_path, Q_FUNC_INFO);

    //==================================Remove entry from roots_info.sqlite_End============================

    //==============================z_roots_count_db -Start=======================================//

    QString z_root_count_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.sqlite";
    m_cmd.clear();
    m_cmd = "UPDATE z_count SET z_active_status = ? where z_root_count_flag = ?";
    QStringList values_status;
    values_status << MACRO_FALSE_VALUE_READABLE_QString << to_remove_root_count_name;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, values_status, z_root_count_db_path, Q_FUNC_INFO);

    //==============================z_roots_count_db -Start=======================================//


    //=========================Remove Entry from roots Txt File-Start====================================
    QString rt_cnt_file_name = to_remove_root_count_name + ".txt";
    QString root_index_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_index.txt";
    recon_static_functions::remove_data_contain_line_from_file(root_index_path,rt_cnt_file_name,Q_FUNC_INFO);


    QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + rt_cnt_file_name;
    recon_static_functions::remove_safely_blank_dir_OR_file(root_file_path,Q_FUNC_INFO);

    //=========================Remove Entry from roots Txt File-End====================================


}

void file_system_post_launcher::remove_source_info_from_source_db_and_text_files()
{
    for(int ii = 0; ii < to_remove_source_count_name_list.size(); ii++)
    {
        QString source_cnt_name = to_remove_source_count_name_list.at(ii);

        //==============================Source_Db_Start=======================================//

        QString src_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

        QString m_cmd = "Delete from fs_status where source_count_name = ?";

        QStringList args;
        args << source_cnt_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, args, src_db_file, Q_FUNC_INFO);



        m_cmd.clear();
        m_cmd = "Delete from tbl_source_index where source_count_name = ?";

        args.clear();
        args << source_cnt_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, args, src_db_file, Q_FUNC_INFO);


        //==============================Source_Db_End=======================================//

        //==============================z_sources_count_db -Start=======================================//

        QString z_source_count_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.sqlite";
        m_cmd.clear();
        m_cmd = "UPDATE z_count SET z_active_status = ? where z_source_count_flag = ?";
        QStringList values_status;
        values_status << MACRO_FALSE_VALUE_READABLE_QString << source_cnt_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(m_cmd, values_status, z_source_count_db_path, Q_FUNC_INFO);

        //==============================z_sources_count_db -End=======================================//



        //==============================Source_Txt_Start=======================================//
        QString src_cnt_file_name = source_cnt_name + ".txt";
        QString sourceindex_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_index.txt";
        recon_static_functions::remove_data_contain_line_from_file(sourceindex_txt_file,src_cnt_file_name,Q_FUNC_INFO);


        QString sourcecount_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + src_cnt_file_name;
        recon_static_functions::remove_safely_blank_dir_OR_file(sourcecount_txt_file,Q_FUNC_INFO);
        //==============================Source_Txt_End=======================================//


    }


}
