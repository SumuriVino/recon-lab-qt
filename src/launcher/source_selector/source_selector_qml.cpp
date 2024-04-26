#include "source_selector.h"
#include "ui_source_selector.h"


void source_selector::on_pushButton_add_source_qml_clicked() // When we click add source button, it opens the QML add source window
{
    QUrl url_add_source_launcher_window = QUrl("qrc:/All_qml_agent.qml");

    QQuickWidget *quick_widget = new QQuickWidget();

    QQmlContext *context = quick_widget->rootContext();

    context->setContextProperty("source_selector_obj",this);

    quick_widget->setSource(url_add_source_launcher_window);

    quick_widget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    emit signal_hide_source_selector_parent_window();

}

void source_selector::pbin_button_back_clicked() // Click back from QML
{
    emit signal_qml_button_initial_back_clicked();
}

void source_selector::pbin_on_close_clicked() // close qml window
{
    emit signal_qml_button_initial_back_clicked();
}

QString source_selector::pbin_get_label(QString m_value) //return display name to set on qml pages like physical evidence, logical evidence, dmg, sparseimage etc
{
    if(m_value == "OS_SCHEME_macOS_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display;
    else if(m_value == "OS_SCHEME_winOS_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display;
    else if(m_value == "OS_SCHEME_otherOS_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display;
    else if(m_value == "EvidenceType_Physical")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical;
    else if(m_value == "EvidenceType_Logical")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical;
    else if(m_value == "EvidenceType_Mobile")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile;
    else if(m_value == "EvidenceType_Cloud")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Cloud;
    else if(m_value == "EvidenceType_Network")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Network;
    else if(m_value == "RootType_ForensicsImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage;
    else if(m_value == "RootType_FileVaultImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage;
    else if(m_value == "RootType_FusionImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage;
    else if(m_value == "RootType_MountedVolumes")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes;
    else if(m_value == "RootType_RECONFSBlockImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage;
    else if(m_value == "RootType_OpticalDiscImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage;
    else if(m_value == "RootType_RAMImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage;
    else if(m_value == "RootType_RECONLogicalImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage;
    else if(m_value == "RootType_TimeMachineBackupFolder")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder;
    else if(m_value == "RootType_TimeMachineBackupImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage;
    else if(m_value == "RootType_macOSHomeDirectory")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory;
    else if(m_value == "RootType_EncaseLogicalImage")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage;
    else if(m_value == "RootType_Folder")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder;
    else if(m_value == "RootType_File")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File;
    else if(m_value == "RootType_iTunesiOSBackup")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup;
    else if(m_value == "RootType_CellebriteiOSBackup")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup;
    //    else if(m_value == "RootType_GrayKeyiOSBackup")
    //        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyiOSBackup;
    else if(m_value == "RootType_GrayKeyBackup")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyBackup;
    else if(m_value == "RootType_ADBAndroidBackup")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup;
    else if(m_value == "RootType_GoogleTakeout")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout;
    else if(m_value == "RootType_RECONMACSharingMode")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode;
    else if(m_value == "SourceType_ForensicsImage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Display;
    else if(m_value == "SourceType_ForensicsImage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal;
    else if(m_value == "SourceType_FileVaultImage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Display;
    else if(m_value == "SourceType_FileVaultImage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal;
    else if(m_value == "SourceType_FusionImage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Display;
    else if(m_value == "SourceType_FusionImage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal;
    else if(m_value == "SourceType_MountedVolumes_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Display;
    else if(m_value == "SourceType_MountedVolumes_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal;
    else if(m_value == "SourceType_TimeMachineBackupFolder_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Display;
    else if(m_value == "SourceType_TimeMachineBackupFolder_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal;
    else if(m_value == "SourceType_TimeMachineBackupImage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Display;
    else if(m_value == "SourceType_TimeMachineBackupImage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal;
    else if(m_value == "SourceType_macOSHomeDirectory_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Display;
    else if(m_value == "SourceType_macOSHomeDirectory_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal;
    else if(m_value == "SourceType_EncaseLogicalImage_L01_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Display;
    else if(m_value == "SourceType_EncaseLogicalImage_L01_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal;
    else if(m_value == "SourceType_Folder_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Display;
    else if(m_value == "SourceType_Folder_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal;
    else if(m_value == "SourceType_File_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Display;
    else if(m_value == "SourceType_File_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal;
    else if(m_value == "SourceType_iTunesiOSBackup_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Display;
    else if(m_value == "SourceType_iTunesiOSBackup_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal;
    else if(m_value == "SourceType_CellebriteiOSBackup_TAR_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Display;
    else if(m_value == "SourceType_CellebriteiOSBackup_TAR_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal;
    else if(m_value == "SourceType_CellebriteiOSBackup_UFDR_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Display;
    else if(m_value == "SourceType_CellebriteiOSBackup_UFDR_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal;
    else if(m_value == "SourceType_GrayKeyiOSBackup_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Display;
    else if(m_value == "SourceType_GrayKeyiOSBackup_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal;
    else if(m_value == "SourceType_GrayKeyAndroidBackup_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Display;
    else if(m_value == "SourceType_GrayKeyAndroidBackup_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal;
    else if(m_value == "SourceType_ADBAndroidBackup_AB_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Display;
    else if(m_value == "SourceType_ADBAndroidBackup_AB_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal;
    else if(m_value == "SourceType_ADBAndroidBackup_Folder_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Display;
    else if(m_value == "SourceType_ADBAndroidBackup_Folder_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal;
    else if(m_value == "SourceType_GoogleTakeout_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Display;
    else if(m_value == "SourceType_GoogleTakeout_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal;
    else if(m_value == "SourceType_RECONMACSharingMode_Sparseimage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Display;
    else if(m_value == "SourceType_RECONMACSharingMode_Sparseimage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal;
    else if(m_value == "SourceType_RECONMACSharingMode_DMG_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Display;
    else if(m_value == "SourceType_RECONMACSharingMode_DMG_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal;
    else if(m_value == "SourceType_RECONMACSharingMode_Folder_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Display;
    else if(m_value == "SourceType_RECONMACSharingMode_Folder_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal;
    else if(m_value == "SourceType_RECONFSBlockImage_Sparseimage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Display;
    else if(m_value == "SourceType_RECONFSBlockImage_Sparseimage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal;
    else if(m_value == "SourceType_RECONFSBlockImage_DMG_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Display;
    else if(m_value == "SourceType_RECONFSBlockImage_DMG_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal;
    else if(m_value == "SourceType_OpticalDiscImage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Display;
    else if(m_value == "SourceType_OpticalDiscImage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal;
    else if(m_value == "SourceType_RAMImage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Display;
    else if(m_value == "SourceType_RAMImage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal;
    else if(m_value == "SourceType_RECONLogicalImage_Sparseimage_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Display;
    else if(m_value == "SourceType_RECONLogicalImage_Sparseimage_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal;
    else if(m_value == "SourceType_RECONLogicalImage_DMG_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Display;
    else if(m_value == "SourceType_RECONLogicalImage_DMG_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal;
    else if(m_value == "SourceType_RECONLogicalImage_Folder_Display")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Display;
    else if(m_value == "SourceType_RECONLogicalImage_Folder_Internal")
        return MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal;
    else if(m_value == ">")
        return M_str_QML_angular_brace;
    else if(m_value == "[")
        return M_str_QML_left_big_brace;
    else if(m_value == "]")
        return M_str_QML_right_big_brace;
    else if(m_value == "...")
        return M_str_QML_browse_text;
    else if(m_value == "Image_Path")
        return M_str_QML_image_path;
    else if(m_value == "Source_Path")
        return M_str_QML_source_path;
    else if(m_value == "Add")
        return M_str_QML_add;
    else if(m_value == "Select_Folder")
        return M_str_QML_select_folder;
    else if(m_value == "Select_DMG_Image")
        return M_str_QML_select_dmg_image;
    else if(m_value == "Select_Sparseimage")
        return M_str_QML_select_sparseimage;
    else if(m_value == "Select_Optical_Disc_Image")
        return M_str_QML_select_optical_disc_image;
    else if(m_value == "Select_Ram_Image")
        return M_str_QML_select_ram_image;
    else if(m_value == "Select_Mounted_Volumes")
        return M_str_QML_select_mounted_volumes;
    else if(m_value == "Select_Forensic_Image")
        return M_str_QML_select_forensic_image;
    else if(m_value == "Select_Fusion_Platter_Image")
        return M_str_QML_select_fusion_image;
    else if(m_value == "Is_FileVaulted")
        return M_str_QML_is_filevaulted;
    else if(m_value == "Is_Fusion")
        return M_str_QML_is_fusion;
    else if(m_value == "Type_Password")
        return M_str_QML_type_password;
    else if(m_value == "Select_Time_Machine_Backup_Folder")
        return M_str_QML_select_time_machine_backup_folder;
    else if(m_value == "Select_Time_Machine_Backup_Image")
        return M_str_QML_select_time_machine_backup_image;
    else if(m_value == "Select_Macos_Home_Directory")
        return M_str_QML_select_macos_home_directory;
    else if(m_value == "Select_L01_Image")
        return M_str_QML_select_l01_image;
    else if(m_value == "Select_File")
        return M_str_QML_select_file;
    else if(m_value == "Username")
        return M_str_QML_username;
    else if(m_value == "Type_Username")
        return M_str_QML_type_username;
    else if(m_value == "Select_iOS_Backup")
        return M_str_QML_select_ios_backup;
    else if(m_value == "Select_Android_Backup")
        return M_str_QML_select_ab_backup;
    else if(m_value == "Select_Android_Backup_Folder")
        return M_str_QML_select_android_backup_folder;
    else if(m_value == "Select_Takeout_Folder")
        return M_str_QML_select_google_takeout;
    else if(m_value == "Select_Operating_System")
        return M_str_QML_select_operating_system;
    else if(m_value == "Select_Image_Type")
        return M_str_QML_select_image_type;
    else if(m_value == "Select_Backup_Type")
        return M_str_QML_select_backup_type;
    else if(m_value == "Select_Evidence_Type")
        return M_str_QML_select_evidence_type;
    else if(m_value == "App_Name")
        return recon_static_functions::get_app_name();

}

QString source_selector::pbin_button_source_path_selection_clicked(QString rt_value, QString src_internal_value) //when we click on browse button of qml to select source path
{                                                                                                               // according to evidence
    QString image_path;

    if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW *.vmdk *.vhd *.VHD *.aff4 *.AFF4"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *.DMG *.raw *.RAW *.dd *.DD *.e01 *.E01 *.ex01 *.Ex01 *.s01 *.S01 *.000 *.001 *.00001"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.iso *.ISO *.cdr *.CDR"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.Sparseimage *.sparseimage"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *DMG"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *.DMG *.Sparseimage *.sparseimage"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
    {
        file_dialog_obj.setNameFilter(("*.l01 *.L01"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.Sparseimage *.sparseimage"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *DMG"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        file_dialog_obj.setNameFilter(("*"));
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
    {
        file_dialog_obj.setNameFilter(("Manifest.db Manifest.mbdb"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
    {
        file_dialog_obj.setNameFilter(("*.tar *.TAR"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
    {
        file_dialog_obj.setNameFilter(("*.ufdr *.UFDR"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    //    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyiOSBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
    {
        file_dialog_obj.setNameFilter(("*.zip *.ZIP"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
    {
        file_dialog_obj.setNameFilter(("*.zip *.ZIP"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
    {
        file_dialog_obj.setNameFilter(("*.ab *AB"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.Sparseimage *.sparseimage"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *DMG"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)
    {
        file_dialog_obj.setFileMode(QFileDialog::Directory);
    }


    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            image_path = selected_files.at(0);
        }
    }

    return image_path;
}

void source_selector::pbin_button_add_selected_sources(QString evdnc_type,QString rt_type,QString src_display_type,QString src_internal_type,QString src_path, QString fusion_path,QString vault_password, QString home_dir_username)
{ // After selecting source, click on add button to send that path from qml to c++ with their root type, source type etc

    if(rt_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage && src_internal_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
    {
        prnt_widget_id = prnt_widget_id + 1;

        File_Vault_image_Class *File_Vault_image_Class_obj = new File_Vault_image_Class(this);
        File_Vault_image_Class_obj->pub_set_id_and_name(prnt_widget_id,src_internal_type);

        set_info_for_sources_widgets(evdnc_type,rt_type,src_display_type,src_internal_type,src_path,vault_password,"");

    }
    else if(rt_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage && src_internal_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        QStringList fusion_str;

        fusion_str << src_path << fusion_path;

        src_path =  fusion_str.join(MACRO_RECON_Splitter_6_comma);

        set_info_for_sources_widgets(evdnc_type,rt_type,src_display_type,src_internal_type,src_path,"","");

    }
    else if(rt_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes && src_internal_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
    {
        prnt_widget_id = prnt_widget_id + 1;

        Mounted_Volumes_Class *Mounted_Volumes_Class_obj = new Mounted_Volumes_Class(this);
        connect(Mounted_Volumes_Class_obj,SIGNAL(signal_mounted_volumes_selected(QStringList)),this,SLOT(slot_mounted_volumes_selected(QStringList)));
        Mounted_Volumes_Class_obj->pub_set_essentials();
        Mounted_Volumes_Class_obj->pub_set_id_and_name(prnt_widget_id,src_internal_type);
        Mounted_Volumes_Class_obj->show();
    }
    else if(rt_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup && src_internal_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
    {
        src_path = QFileInfo(src_path).absoluteDir().absolutePath();
        set_info_for_sources_widgets(evdnc_type,rt_type,src_display_type,src_internal_type,src_path,vault_password,home_dir_username);
    }
    else
    {
        set_info_for_sources_widgets(evdnc_type,rt_type,src_display_type,src_internal_type,src_path,vault_password,home_dir_username);
    }


    if(ui->tableWidget_source_scroll_area->rowCount() > 0)
        emit signal_source_selected(true);
    else
        emit signal_source_selected(false);

}

QString source_selector::pbin_get_icons_path(QString icon_name) //Return icons path to set on qml window of add source
{
    QString icon_path_1_src = QApplication::applicationDirPath() + "/../icons/source/";
    QString icon_path_2_os = QApplication::applicationDirPath() + "/../icons/os/";
    QString icon_path_3_sign = QApplication::applicationDirPath() + "/../icons/signs/";
    QString icon_path_4_root = QApplication::applicationDirPath() + "/../icons/root/";

    QString icon_path_final;

    if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
        icon_path_final = icon_path_2_os + "macOS.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
        icon_path_final = icon_path_2_os + "winOS.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display)
        icon_path_final = icon_path_2_os + "otherOS.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Physical)
        icon_path_final = icon_path_1_src + "evidence_physical.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Logical)
        icon_path_final = icon_path_1_src + "evidence_logical.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Mobile)
        icon_path_final = icon_path_1_src + "evidence_mobile.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Cloud)
        icon_path_final = icon_path_1_src + "evidence_cloud.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_EvidenceType_Network)
        icon_path_final = icon_path_1_src + "network_acquisition.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_MountedVolumes)
        icon_path_final = icon_path_4_root + "mounted_volumes.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage)
        icon_path_final = icon_path_4_root + "forensic_image.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_OpticalDiscImage)
        icon_path_final = icon_path_4_root + "optical_disc_image.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RAMImage)
        icon_path_final = icon_path_4_root + "ram.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage)
        icon_path_final = icon_path_4_root + "recon_fs_block_image.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupFolder)
        icon_path_final = icon_path_4_root + "time_machine_backup_folder.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage)
        icon_path_final = icon_path_4_root + "time_machine_backup_image.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_macOSHomeDirectory)
        icon_path_final = icon_path_4_root + "macos_home_directory.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage)
        icon_path_final = icon_path_4_root + "recon_logical_image.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_EncaseLogicalImage)
        icon_path_final = icon_path_4_root + "encase_logical_image.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_Folder)
        icon_path_final = icon_path_4_root + "folder.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_File)
        icon_path_final = icon_path_4_root + "file.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GoogleTakeout)
        icon_path_final = icon_path_4_root + "google_takeout_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONMACSharingMode)
        icon_path_final = icon_path_4_root + "recon_mac_sharing_mode.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup)
        icon_path_final = icon_path_4_root + "itunes_ios_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup)
        icon_path_final = icon_path_4_root + "cellebrite_ios_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_GrayKeyBackup)
        icon_path_final = icon_path_4_root + "graykey_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ADBAndroidBackup)
        icon_path_final = icon_path_4_root + "adb_android_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
        icon_path_final = icon_path_1_src + "sparseimage.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
        icon_path_final = icon_path_1_src + "dmg.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
        icon_path_final = icon_path_1_src + "graykey_iOS_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
        icon_path_final = icon_path_1_src + "graykey_android_backup.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
        icon_path_final = icon_path_1_src + "tar.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
        icon_path_final = icon_path_1_src + "ufdr.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
        icon_path_final = icon_path_1_src + "android_backup_general.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
        icon_path_final = icon_path_1_src + "folder.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
        icon_path_final = icon_path_1_src + "sparseimage.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
        icon_path_final = icon_path_1_src + "dmg.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)
        icon_path_final = icon_path_1_src + "folder.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal)
        icon_path_final = icon_path_1_src + "sparseimage.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal)
        icon_path_final = icon_path_1_src + "dmg.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal)
        icon_path_final = icon_path_1_src + "folder.png";
    else if(icon_name == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
        icon_path_final = icon_path_1_src + "l01.png";
    else if(icon_name == "back")
        icon_path_final = icon_path_3_sign + "back.png";
    else if(icon_name == "source_splash_image")
        icon_path_final = icon_path_1_src + "source_splash.jpg";
    else if(icon_name == "eye_open")
        icon_path_final = icon_path_3_sign + "eye_open.png";
    else if(icon_name == "eye_close")
        icon_path_final = icon_path_3_sign + "eye_close.png";


    return icon_path_final;
}

void source_selector::pbin_set_messagebox_for_unsupported_image(QString rt_value, QString src_internal_value) //Display message of unsupport image on qml if satisfy
{
    if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "This Filevault image format is unsupported");
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "This Fusion image format is unsupported");
    }
}

bool source_selector::pbin_is_image_supported(QString rt_value, QString src_internal_value, QString image_path) // return true if image supported
{

    QString img_extn = image_path.mid(image_path.lastIndexOf("."), image_path.size());
    img_extn = img_extn.prepend("*");  //*.dd

    if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
    {
        QString supported_frmt = "*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW";
        QStringList supp_fmt_list = supported_frmt.split(" ");
        supp_fmt_list.removeAll(" ");

        if(supp_fmt_list.contains(img_extn))
        {
            return true;
        }
    }
    else if(rt_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage && src_internal_value == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {

        QString supported_frmt = "*.dmg *.DMG *.raw *.RAW *.dd *.DD *.e01 *.E01 *.ex01 *.Ex01 *.s01 *.S01 *.000 *.001 *.00001";
        QStringList supp_fmt_list = supported_frmt.split(" ");
        supp_fmt_list.removeAll(" ");

        if(supp_fmt_list.contains(img_extn))
        {
            return true;
        }
    }

    return false;
}
