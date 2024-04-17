#include "source_selector.h"
#include "ui_source_selector.h"

source_selector::source_selector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::source_selector)
{
    ui->setupUi(this);

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

    QFont fnt;
    fnt.setPixelSize(13);
    menu_main   = new QMenu(this);
    menu_main->setFont(fnt);

    M_str_QML_image_path = "Image Path";
    M_str_QML_source_path = "Source Path";
    M_str_QML_angular_brace = " > ";
    M_str_QML_left_big_brace = "[";
    M_str_QML_right_big_brace = "]";
    M_str_QML_browse_text = "...";
    M_str_QML_add = "Add";
    M_str_QML_select_folder = "Select Folder";
    M_str_QML_select_dmg_image = "Select DMG Image";
    M_str_QML_select_sparseimage = "Select Sparseimage";
    M_str_QML_select_forensic_image = "Select Forensic Image";
    M_str_QML_type_password = "Type Password or Recovery Key";
    M_str_QML_select_fusion_image = "Select Fusion Platter Image";
    M_str_QML_is_filevaulted = "Is FileVaulted";
    M_str_QML_is_fusion = "Is Fusion";
    M_str_QML_select_optical_disc_image = "Select Optical Disc Image";
    M_str_QML_select_ram_image = "Select Ram Image";
    M_str_QML_select_mounted_volumes = "Select Mounted Volumes";

    M_str_QML_select_time_machine_backup_folder = "Select Time Machine Backup Folder";
    M_str_QML_select_time_machine_backup_image = "Select Time Machine Backup Image";

    M_str_QML_select_macos_home_directory = "Select macOS Home Directory";
    M_str_QML_select_l01_image = "Select L01 Image";
    M_str_QML_select_file = "Select File";
    M_str_QML_username = "Username";
    M_str_QML_type_username = "Type Username";
    M_str_QML_select_ios_backup = "Select iOS Backup";
    M_str_QML_select_ab_backup = "Select Android Backup";
    M_str_QML_select_android_backup_folder = "Select Android backup Folder";
    M_str_QML_select_google_takeout = "Select Takeout Folder";
    M_str_QML_select_evidence_type = "Select Evidence Type";
    M_str_QML_select_image_type = "Select Image Type";
    M_str_QML_select_backup_type = "Select Backup Type";

    M_str_QML_select_operating_system = "Select Operating System";

    ui->pushButton_add_source->hide();

    recon_helper_standard_obj = new recon_helper_standard;


    ui->pushButton_add_source->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_add_source->text()));

    //---------------------------------Physical Evidence Menu Start--------------------------------------//
    menu_evidence_physical = new QMenu(this);
    menu_evidence_physical = menu_main->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical));
    action_mounted_volumes = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes),menu_evidence_physical);
    action_forensics_image = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage),menu_evidence_physical);
    action_file_vault_image = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage),menu_evidence_physical);
    action_fusion_drive_image = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage),menu_evidence_physical);
    action_RAM_Image = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage),menu_evidence_physical);
    action_optical_disk_image = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage),menu_evidence_physical);

    action_mounted_volumes->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes));
    action_forensics_image->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage));
    action_file_vault_image->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage));
    action_fusion_drive_image->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage));
    action_RAM_Image->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage));
    action_optical_disk_image->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage));



    menu_evidence_physical->addAction(action_mounted_volumes);
    menu_evidence_physical->addAction(action_forensics_image);

    menu_evidence_physical->addAction(action_file_vault_image);
    menu_evidence_physical->addAction(action_fusion_drive_image);
    menu_recon_fs_block_image = menu_evidence_physical->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage));
    QStringList submenu_recon_fs_block_img;
    submenu_recon_fs_block_img << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Display);
    create_submenu_actions(menu_recon_fs_block_image, submenu_recon_fs_block_img);

    menu_evidence_physical->addAction(action_optical_disk_image);
    menu_evidence_physical->addAction(action_RAM_Image);
    //---------------------------------Physical Evidence Menu End--------------------------------------//

    //---------------------------------Logical Evidence Menu Start--------------------------------------//
    menu_evidence_logical = new QMenu(this);
    menu_evidence_logical = menu_main->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical));

    action_time_machine = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder),menu_evidence_logical);
    action_home_directory = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory),menu_evidence_logical);
    action_folder = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder),menu_evidence_logical);
    action_files = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File),menu_evidence_logical);

    action_time_machine->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder));
    action_home_directory->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory));
    action_folder->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder));
    action_files->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File));

    menu_evidence_logical->addAction(action_time_machine);
    menu_evidence_logical->addAction(action_home_directory);

    menu_recon_logical_image = menu_evidence_logical->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage));
    QStringList submenu_rcn_lgcl_img;
    submenu_rcn_lgcl_img << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Display)
                         << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Display)
                         << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Display);
    create_submenu_actions(menu_recon_logical_image, submenu_rcn_lgcl_img);

    //menu_evidence_logical->addAction(action_encase_logical);
    menu_encase_logical_image = menu_evidence_logical->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage));
    QStringList submenu_encase_logical_img;
    submenu_encase_logical_img << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Display);
    create_submenu_actions(menu_encase_logical_image, submenu_encase_logical_img);

    menu_evidence_logical->addAction(action_folder);
    menu_evidence_logical->addAction(action_files);



    //---------------------------------Logical Evidence Menu End--------------------------------------//

    //---------------------------------Mobile Evidence Menu Start--------------------------------------//

    menu_evidence_mobile = new QMenu(this);
    menu_evidence_mobile = menu_main->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile));

    action_ios_backup_itunes = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup),menu_evidence_mobile);
    action_ios_backup_itunes->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup));
    menu_evidence_mobile->addAction(action_ios_backup_itunes);


    menu_ios_backup_cellebrite = menu_evidence_mobile->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup));
    QStringList submenu_ios_backup_cellebrite;
    submenu_ios_backup_cellebrite << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Display)
                                  << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Display);
    create_submenu_actions(menu_ios_backup_cellebrite, submenu_ios_backup_cellebrite);



    menu_android_backup = menu_evidence_mobile->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup));
    QStringList submenu_andrd_bckup;
    submenu_andrd_bckup << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Display)
                        << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Display);
    create_submenu_actions(menu_android_backup, submenu_andrd_bckup);


    //---------------------------------Mobile Evidence Menu End--------------------------------------//


    //---------------------------------Cloud Evidence Menu Start-------------------------------------//
    menu_evidence_cloud = new QMenu(this);
    menu_evidence_cloud = menu_main->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Cloud));

    action_google_takeout = new QAction(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout),menu_evidence_cloud);
    action_google_takeout->setObjectName((MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout));
    menu_evidence_cloud->addAction(action_google_takeout);
    //---------------------------------Cloud Evidence Menu End-------------------------------------//

    //----------------------------Network Acquisition Menu Start----------------------------------//
    menu_evidence_network = new QMenu(this);
    menu_evidence_network = menu_main->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Network));

    menu_recon_mac_sharing_mode = menu_evidence_network->addMenu(global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode));
    QStringList submenu_network_img;
    submenu_network_img << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Display)
                        << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Display)
                        << global_lang_tr_obj.get_translated_string(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Display);
    create_submenu_actions(menu_recon_mac_sharing_mode, submenu_network_img);

    //----------------------------Network Acquisition Menu End----------------------------------//


    ui->pushButton_add_source->setMenu(menu_main);

    connect(ui->pushButton_add_source,SIGNAL(clicked()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_mounted_volumes,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_forensics_image,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_file_vault_image,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_fusion_drive_image,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_home_directory,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_folder,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_files,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_time_machine,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_ios_backup_itunes,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    //connect(menu_ios_backup,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));
    connect(action_RAM_Image,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(action_google_takeout,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(menu_android_backup,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));
    connect(menu_recon_logical_image,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));
    connect(menu_encase_logical_image,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));
    connect(action_optical_disk_image,SIGNAL(triggered()),this,SLOT(slot_toolButton_add_source_clicked()));
    connect(menu_recon_mac_sharing_mode,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));
    connect(menu_recon_fs_block_image,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));
    connect(menu_ios_backup_cellebrite,SIGNAL(triggered(QAction *)),this,SLOT(slot_source_submenu_action_triggered(QAction*)));

    ui->tableWidget_source_scroll_area->hideColumn(1);
    src_widget_id = 0;
    root_count_index = 0;
    prnt_widget_id = 0;
    list_struct_root_info.clear();
    ui->pushButton_add_source->setAttribute(Qt::WA_MacShowFocusRect,false);

}

source_selector::~source_selector()
{
    delete ui;
}

void source_selector::create_submenu_actions(QMenu *mainMenu , QStringList submenuList)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    for(int i = 0; i < submenuList.size(); i++)
    {
        QAction *act_submenu = new QAction(mainMenu);
        act_submenu = mainMenu->addAction(tr(submenuList.at(i).toLocal8Bit().data()));


        if(submenuList.at(i).isEmpty())
            act_submenu->setSeparator(true);

        mainMenu->addAction(act_submenu);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void source_selector::pub_get_essential_value_from_all_roots()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    for(int pp = 0; pp < ui->tableWidget_source_scroll_area->rowCount(); pp++)
    {
        if(ui->tableWidget_source_scroll_area->cellWidget(pp,0) == NULL)
            continue;

        if(ui->tableWidget_source_scroll_area->item(pp,1)->text().trimmed().isEmpty() || ui->tableWidget_source_scroll_area->item(pp,1)->text().trimmed().isEmpty())
            continue;

        source_widgets *source_widget_obj = dynamic_cast<source_widgets*>(ui->tableWidget_source_scroll_area->cellWidget(pp,0));

        QString evidence_type = source_widget_obj->pub_get_evidence_type();
        QString rt_type = source_widget_obj->pub_get_root_type();
        QString src_type_dsply = source_widget_obj->pub_get_source_type_display();
        QString src_type_intrnl = source_widget_obj->pub_get_source_type_internal();
        QString path1 = source_widget_obj->pub_get_path_1_string();
        QString path2 = source_widget_obj->pub_get_path_2_string();
        QString password = source_widget_obj->pub_get_password_string();
        QString username = source_widget_obj->pub_get_user_name_string();
        QString root_cnt_name = source_widget_obj->pub_get_root_count_name();

        struct_global_source_evidencer evd_obj = source_widget_obj->get_source_evidence_info();

        QString encrypted_pswd = recon_static_functions::encrypt_string(password , Q_FUNC_INFO);


        struct_GLOBAL_witness_info_root struct_root_info;
        recon_static_functions::clear_variables_witness_info_root_structure(struct_root_info);

        struct_root_info.evidence_type = evidence_type;
        struct_root_info.root_count_name = root_cnt_name;
        struct_root_info.root_name = QFileInfo(path1).fileName();
        struct_root_info.root_path_1 = path1;
        struct_root_info.root_path_2_for_fusion = path2;
        struct_root_info.root_type = rt_type;
        struct_root_info.source_type_display = src_type_dsply;
        struct_root_info.source_type_internal = src_type_intrnl;
        struct_root_info.root_recovery_passcode_plain = password;
        struct_root_info.root_recovery_passcode_encrypted = encrypted_pswd;

        struct_root_info.home_directory_user_name = username;
        struct_root_info.evidence_number = evd_obj.evidence_by_examiner;
        struct_root_info.description = evd_obj.description_by_examiner;

        QString evidence_password = evd_obj.source_password_by_examiner;
        QString evidence_encrypted_pswd = recon_static_functions::encrypt_string(evidence_password , Q_FUNC_INFO);
        struct_root_info.root_evidence_password_plain = evidence_password;
        struct_root_info.root_evidence_password_encrypted = evidence_encrypted_pswd;

        list_struct_root_info.append(struct_root_info);

    }

    // emit signal_added_roots_details_list(list_struct_root_info);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}


void source_selector::slot_toolButton_add_source_clicked()
{
    QAction *action = qobject_cast<QAction*>(sender());

    if(action)
    {

        if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes)
        {
            function_mounted_volumes_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage)
        {
            function_forensics_image_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage)
        {
            function_file_vault_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
        {
            function_fusion_drive_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder)
        {
            function_time_machine_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup)
        {
            function_ios_uid_backup_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory)
        {
            function_home_directory_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder)
        {
            function_folder_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File)
        {
            function_file_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage)
        {
            function_RAM_image_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout)
        {
            function_google_takeout_triggered();
        }
        else if(action->objectName() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage)
        {
            function_optical_disk_image_triggered();
        }


        if(ui->tableWidget_source_scroll_area->rowCount() > 1)
            emit signal_source_selected(true);
        else
            emit signal_source_selected(false);

    }

}

void source_selector::slot_toolButton_add_source_clicked_with_menu(QAction *current_clicked_action)
{
    QWidget *widget = current_clicked_action->parentWidget();
    if(widget)
    {
        QMenu *menu = dynamic_cast<QMenu*>(widget);
        QString menu_title = global_lang_tr_obj.get_original_string(menu->title());
        QString action_text_recieved = global_lang_tr_obj.get_original_string(current_clicked_action->text());
        QString action_text_modify = action_text_recieved;

        if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && action_text_recieved == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal;
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Display)
        {
            action_text_modify = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal;
        }



        if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)
        {
            function_rcn_logical_image_sparseimage_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal)
        {
            function_rcn_logical_image_dmg_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal)
        {
            function_rcn_logical_image_folder_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
        {
            function_adb_android_backup_ab_file_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
        {
            function_adb_android_backup_folder_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
        {
            function_encase_logical_image_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
        {
            function_recon_mac_sharing_mode_sparseimage_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
        {
            function_recon_mac_sharing_mode_dmg_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)
        {
            function_recon_mac_sharing_mode_folder_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
        {
            function_recon_fs_block_image_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
        {
            function_cellebrite_ios_backup_ufdr_file_triggered();
        }
        else if(menu_title == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup && action_text_modify == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
        {
            function_cellebrite_ios_backup_tar_file_triggered();
        }


    }

    if(ui->tableWidget_source_scroll_area->rowCount() > 1)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);

}

void source_selector::function_RAM_image_triggered()
{
    QString ram_path_str;// = QFileDialog::getOpenFileName(this,"Select Any File",getenv("HOME"));

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            ram_path_str = selected_files.at(0);
        }
    }

    if(ram_path_str.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal,ram_path_str,"","");

}


void source_selector::function_mounted_volumes_triggered()
{
    prnt_widget_id = prnt_widget_id + 1;

    Mounted_Volumes_Class *Mounted_Volumes_Class_obj = new Mounted_Volumes_Class(this);
    Mounted_Volumes_Class_obj->pub_set_essentials();
    Mounted_Volumes_Class_obj->pub_set_id_and_name(prnt_widget_id,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal);
    connect(Mounted_Volumes_Class_obj,SIGNAL(signal_mounted_volumes_selected(QStringList)),this,SLOT(slot_mounted_volumes_selected(QStringList)));

    Mounted_Volumes_Class_obj->show();
}


void source_selector::function_forensics_image_triggered()
{

    QString image_path;
    file_dialog_obj.setNameFilter(("*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW *.vmdk *.vhd *.VHD *.aff4 *.AFF4"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal,image_path,"","");

}

void source_selector::function_recon_fs_block_image_triggered()
{
    QString image_path;
    file_dialog_obj.setNameFilter(("*.Sparseimage *.sparseimage"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal,image_path,"","");

}

void source_selector::function_file_vault_triggered()
{

    file_vault_path.clear();
    file_dialog_obj.setNameFilter(("*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            file_vault_path = selected_files.at(0);
        }
    }

    if(file_vault_path.isEmpty())
        return;

    prnt_widget_id = prnt_widget_id + 1;

    File_Vault_image_Class *File_Vault_image_Class_obj = new File_Vault_image_Class(this);
    File_Vault_image_Class_obj->pub_set_id_and_name(prnt_widget_id,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal);
    connect(File_Vault_image_Class_obj,SIGNAL(signal_file_vault_password_inserted(QString)),this,SLOT(slot_file_vault_password_inserted(QString)));
    File_Vault_image_Class_obj->show();
}

void source_selector::function_fusion_drive_triggered()
{
    prnt_widget_id = prnt_widget_id + 1;
    Fusion_Drive_Class *Fusion_Drive_Class_obj = new Fusion_Drive_Class(this);
    Fusion_Drive_Class_obj->pub_set_id_and_name(prnt_widget_id,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal);
    connect(Fusion_Drive_Class_obj,SIGNAL(signal_fusion_drives_selected(QStringList)),this,SLOT(slot_fusion_drives_selected(QStringList)));
    Fusion_Drive_Class_obj->show();
}

void source_selector::function_home_directory_triggered()
{
    prnt_widget_id = prnt_widget_id + 1;

    Home_Directory_Class *Home_Directory_Class_obj = new Home_Directory_Class(this);
    Home_Directory_Class_obj->pub_set_id_and_name(prnt_widget_id,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal);
    connect(Home_Directory_Class_obj,SIGNAL(signal_home_directory_user_name_inserted(QString)),this,SLOT(slot_home_directory_user_name_inserted(QString)));
    Home_Directory_Class_obj->show();

}

void source_selector::function_optical_disk_image_triggered()
{
    QString image_path;
    file_dialog_obj.setNameFilter(("*.iso *.ISO *.cdr *.CDR"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal,image_path,"","");
}

void source_selector::function_google_takeout_triggered()
{
    QString takeout_path;

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            takeout_path = selected_files.at(0);
        }
    }


    if(takeout_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Cloud,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal,takeout_path,"","");

}

void source_selector::function_rcn_logical_image_sparseimage_triggered()
{
    QString image_path;
    file_dialog_obj.setNameFilter(("*.sparseimage .SPARSEIMAGE"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }
    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal,image_path,"","");

}

void source_selector::function_recon_mac_sharing_mode_sparseimage_triggered()
{
    QString image_path;
    file_dialog_obj.setNameFilter(("*.sparseimage .SPARSEIMAGE"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }
    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Network,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal,image_path,"","");
}

void source_selector::function_recon_mac_sharing_mode_dmg_triggered()
{
    QString image_path;
    file_dialog_obj.setNameFilter(("*.dmg *DMG"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Network,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal,image_path,"","");
}

void source_selector::function_recon_mac_sharing_mode_folder_triggered()
{
    QString folder_path;// = QFileDialog::getExistingDirectory(this,"Select Folder",getenv("HOME"));

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            folder_path = selected_files.at(0);
        }
    }

    if(folder_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Network,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal,folder_path,"","");
}

void source_selector::function_adb_android_backup_ab_file_triggered()
{
    QString andrd_path;
    file_dialog_obj.setNameFilter(("*.ab *AB"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            andrd_path = selected_files.at(0);
        }
    }

    if(andrd_path.isEmpty())
        return;


    if(!QFileInfo(andrd_path).fileName().endsWith(".ab" , Qt::CaseInsensitive))
        return;


    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal,andrd_path,"","");

}

void source_selector::function_adb_android_backup_folder_triggered()
{
    QString andrd_path;

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            andrd_path = selected_files.at(0);
        }
    }


    if(andrd_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal,andrd_path,"","");


}

void source_selector::function_rcn_logical_image_dmg_triggered()
{
    QString image_path;
    file_dialog_obj.setNameFilter(("*.dmg *DMG"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    if(image_path.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal,image_path,"","");

}

void source_selector::function_rcn_logical_image_folder_triggered()
{
    QString folder_path;// = QFileDialog::getExistingDirectory(this,"Select Folder",getenv("HOME"));

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            folder_path = selected_files.at(0);
        }
    }

    if(folder_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal,folder_path,"","");

}

void source_selector::function_encase_logical_image_triggered()
{
    QString image_path;// = QFileDialog::getExistingDirectory(this,"Select Folder",getenv("HOME"));

    file_dialog_obj.setNameFilter(("*.l01 *.L01"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    if(image_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal,image_path,"","");

}

void source_selector::function_folder_triggered()
{
    QString folder_path;

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            folder_path = selected_files.at(0);
        }
    }

    if(folder_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal,folder_path,"","");

}

void source_selector::function_file_triggered()
{

    QString filepath_str;// = QFileDialog::getOpenFileName(this,"Select Any File",getenv("HOME"));

    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    file_dialog_obj.setNameFilter(("*"));
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            filepath_str = selected_files.at(0);
        }
    }

    if(filepath_str.isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal,filepath_str,"","");

}

void source_selector::function_time_machine_triggered()
{
    QString tm_path;// = QFileDialog::getExistingDirectory(this,"Select Time Machine Backup",getenv("HOME"));


    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            tm_path = selected_files.at(0);
        }
    }

    if(tm_path.trimmed().isEmpty())
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal,tm_path,"","");


}

void source_selector::function_ios_uid_backup_triggered()
{
    QString mbdb_file_path;// = QFileDialog::getOpenFileName(this, tr("Select iOS Backup"),getenv("HOME"), tr("Files (Manifest.db Manifest.mbdb)"));

    file_dialog_obj.setNameFilter(("Manifest.db Manifest.mbdb"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            mbdb_file_path = selected_files.at(0);
        }
    }
    if(mbdb_file_path.trimmed().isEmpty())
        return;

    //Check for required Manifest file.
    if(QFileInfo(mbdb_file_path).fileName() != QString("Manifest.db") && QFileInfo(mbdb_file_path).fileName() != QString("Manifest.mbdb"))
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal,QFileInfo(mbdb_file_path).absoluteDir().absolutePath(),"","");

}

void source_selector::function_cellebrite_ios_backup_ufdr_file_triggered()
{
    QString ufdr_file_path;
    file_dialog_obj.setNameFilter(("*.ufdr *.UFDR"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            ufdr_file_path = selected_files.at(0);
        }
    }

    if(ufdr_file_path.trimmed().isEmpty())
        return;

    if(!QFileInfo(ufdr_file_path).fileName().endsWith(".ufdr" , Qt::CaseInsensitive))
        return;


    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal,ufdr_file_path,"","");
}


void source_selector::function_cellebrite_ios_backup_tar_file_triggered()
{
    QString tar_file_path;
    file_dialog_obj.setNameFilter(("*.tar *.TAR"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            tar_file_path = selected_files.at(0);
        }
    }

    if(tar_file_path.trimmed().isEmpty())
        return;

    if(!QFileInfo(tar_file_path).fileName().endsWith(".tar" , Qt::CaseInsensitive))
        return;

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal,tar_file_path,"","");
}




void source_selector::slot_remove_button_clicked(int id,QString name)
{
    for(int ii = 0; ii < ui->tableWidget_source_scroll_area->rowCount(); ii++)
    {
        if(ui->tableWidget_source_scroll_area->cellWidget(ii,0) == NULL)
            continue;
        if(ui->tableWidget_source_scroll_area->item(ii,1)->text() != name || ui->tableWidget_source_scroll_area->item(ii,1)->text().isEmpty())
            continue;

        source_widgets *source_widget_obj = dynamic_cast<source_widgets*>(ui->tableWidget_source_scroll_area->cellWidget(ii,0));
        if(id == source_widget_obj->pub_get_id())
        {
            ui->tableWidget_source_scroll_area->removeRow(ii + 1);
            ui->tableWidget_source_scroll_area->removeRow(ii);
        }

    }

    //This loop is for set all root count name again if we remove entry from UI.
    root_count_index = 0;
    for(int kk = 0 ; kk < ui->tableWidget_source_scroll_area->rowCount(); kk++)
    {
        if(ui->tableWidget_source_scroll_area->cellWidget(kk,0) == NULL)
            continue;
        if(ui->tableWidget_source_scroll_area->item(kk,1)->text() != name || ui->tableWidget_source_scroll_area->item(kk,1)->text().isEmpty())
            continue;

        update_root_count_index();
        QString rt_cnt_nm = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Root_Flag) + QString::number(root_count_index);

        source_widgets *source_widget_obj = dynamic_cast<source_widgets*>(ui->tableWidget_source_scroll_area->cellWidget(kk,0));
        source_widget_obj->pub_set_root_count_name(rt_cnt_nm);
    }

    if(ui->tableWidget_source_scroll_area->rowCount() > 0)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);
}

void source_selector::slot_mounted_volumes_selected(QStringList media_list)
{

    for(int li = 0; li < media_list.size(); li++)
    {
        set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal,media_list.at(li),"","");
    }

    if(ui->tableWidget_source_scroll_area->rowCount() > 0)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);
}

void source_selector::slot_file_vault_password_inserted(QString passwd)
{
    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal,file_vault_path,passwd,"");

    if(ui->tableWidget_source_scroll_area->rowCount() > 0)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);

}

void source_selector::slot_home_directory_user_name_inserted(QString username)
{

    home_directory_path.clear();

    file_dialog_obj.setFileMode(QFileDialog::Directory);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            home_directory_path = selected_files.at(0);
        }
    }

    if(home_directory_path.trimmed().isEmpty())
        return;


    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal,home_directory_path,"",username);

    if(ui->tableWidget_source_scroll_area->rowCount() > 0)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);


}

void source_selector::slot_fusion_drives_selected(QStringList fusion_drives)
{
    QString fusn_path_str =  fusion_drives.join(MACRO_RECON_Splitter_6_comma);

    set_info_for_sources_widgets(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Display,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal,fusn_path_str,"","");

    if(ui->tableWidget_source_scroll_area->rowCount() > 0)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);

}

void source_selector::slot_source_submenu_action_triggered(QAction *current_clicked_action)
{
    slot_toolButton_add_source_clicked_with_menu(current_clicked_action);
}


void source_selector::pub_hide_widgets_for_load_case()
{
    ui->pushButton_add_source->hide();
    ui->pushButton_add_source_qml->hide();
}

void source_selector::pub_set_root_info_in_page(QStringList root_path_with_type_list)
{

    for(int li = 0; li < root_path_with_type_list.size(); li++)
    {
        src_widget_id = src_widget_id + 1;

        QStringList tmp_list = root_path_with_type_list.at(li).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(tmp_list.size() < 5)
            continue;

        QString root_count_name = tmp_list.at(0);
        QString source_path = tmp_list.at(1);
        QString evidence_type = tmp_list.at(2);
        QString rt_type = tmp_list.at(3);
        QString src_type_intrnl = tmp_list.at(4);

        source_widgets *source_widget_obj = new source_widgets(this);
        source_widget_obj->pub_set_is_load_case(true);
        source_widget_obj->pub_set_widget_id(src_widget_id);

        source_widget_obj->pub_set_root_type(rt_type);
        source_widget_obj->pub_set_source_type_internal(src_type_intrnl);

        source_widget_obj->pub_set_root_count_name(root_count_name);
        source_widget_obj->pub_set_media_path(source_path);
        source_widget_obj->pub_set_evidence_type(evidence_type);

        connect(source_widget_obj,SIGNAL(signal_remove_button_clicked(int,QString)),this,SLOT(slot_remove_button_clicked(int,QString)));

        int i = ui->tableWidget_source_scroll_area->rowCount();
        ui->tableWidget_source_scroll_area->insertRow(i);
        ui->tableWidget_source_scroll_area->setRowHeight((i),source_widget_obj->pub_get_window_height());
        ui->tableWidget_source_scroll_area->setCellWidget((i),0,source_widget_obj);

        QTableWidgetItem *item_name = new QTableWidgetItem;
        item_name->setText(rt_type);
        ui->tableWidget_source_scroll_area->setItem(i,1,item_name);

        //-Add extra line..
        ui->tableWidget_source_scroll_area->insertRow(i + 1);
        ui->tableWidget_source_scroll_area->setRowHeight(i+1,7);
        QTableWidgetItem *item_2 = new QTableWidgetItem;
        QTableWidgetItem *item_3 = new QTableWidgetItem;
        item_2->setText("");
        item_3->setText("");
        ui->tableWidget_source_scroll_area->setItem(i + 1,0,item_2);
        ui->tableWidget_source_scroll_area->setItem(i+ 1,1,item_3);
    }
}

void source_selector::set_info_for_sources_widgets(QString evidence_type,QString root_type, QString source_type_display, QString source_type_internal, QString source_path, QString password, QString username)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    src_widget_id = src_widget_id + 1;

    if(ui->tableWidget_source_scroll_area->rowCount() <= 0)
    {
        root_count_index = 0;
    }


    update_root_count_index();
    QString rt_cnt_nm = QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Root_Flag) + QString::number(root_count_index);

    source_widgets *source_widget_obj = new source_widgets(this);
    connect(source_widget_obj,SIGNAL(signal_remove_button_clicked(int,QString)),this,SLOT(slot_remove_button_clicked(int,QString)));

    source_widget_obj->pub_set_widget_id(src_widget_id);
    source_widget_obj->pub_set_evidence_type(evidence_type);
    source_widget_obj->pub_set_root_count_name(rt_cnt_nm);
    source_widget_obj->pub_set_root_type(root_type);
    source_widget_obj->pub_set_source_type_display(source_type_display);
    source_widget_obj->pub_set_source_type_internal(source_type_internal);
    source_widget_obj->pub_set_media_path(source_path);
    source_widget_obj->pub_set_password_string(password);
    source_widget_obj->pub_ask_for_eveidence_no(source_path);
    source_widget_obj->pub_set_username_string(username);


    int ii = ui->tableWidget_source_scroll_area->rowCount();
    ui->tableWidget_source_scroll_area->insertRow(ii);
    ui->tableWidget_source_scroll_area->setRowHeight((ii),source_widget_obj->pub_get_window_height());
    ui->tableWidget_source_scroll_area->setCellWidget((ii),0,source_widget_obj);

    QTableWidgetItem *item_name = new QTableWidgetItem;
    item_name->setText(source_type_internal);
    ui->tableWidget_source_scroll_area->setItem(ii,1,item_name);

    //-Add extra line..
    ui->tableWidget_source_scroll_area->insertRow(ii + 1);
    ui->tableWidget_source_scroll_area->setRowHeight(ii+1,7);
    QTableWidgetItem *item_2 = new QTableWidgetItem;
    QTableWidgetItem *item_3 = new QTableWidgetItem;
    item_2->setText("");
    item_3->setText("");
    ui->tableWidget_source_scroll_area->setItem(ii + 1,0,item_2);
    ui->tableWidget_source_scroll_area->setItem(ii+ 1,1,item_3);

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void source_selector::update_root_count_index()
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    // we update the Root Count Index according to the sources we added/remove from User Interface.
    // And from already stored

    int max_root_count_db_txt_int = recon_helper_standard_obj->get_maximum_root_count_index_from_db_and_text_file();

    if(root_count_index == 0)
    {
        int max_root_count_strct = 0;

        QList<struct_GLOBAL_witness_info_root> list_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();
        if(list_root_info.size() > 0)
        {
            QString rt = list_root_info.at(list_root_info.size() - 1).root_count_name;
            rt.remove(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Root_Flag);
            max_root_count_strct = rt.toInt();
        }

        if(max_root_count_strct < max_root_count_db_txt_int)
        {
            max_root_count_strct = max_root_count_db_txt_int;
        }


        if(max_root_count_strct == 0)
        {
            root_count_index =  1;
        }
        else
        {
            root_count_index = max_root_count_strct  + 1;
        }
    }
    else
    {
        if(root_count_index < max_root_count_db_txt_int)
        {
            root_count_index = max_root_count_db_txt_int;
        }

        root_count_index = root_count_index + 1;
    }

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

}


QList<struct_GLOBAL_witness_info_root> source_selector::pub_get_root_info_list_from_gui()
{
    return list_struct_root_info;
}
