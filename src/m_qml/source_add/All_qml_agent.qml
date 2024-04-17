import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
import QtQuick.Dialogs 6.0
import Qt.labs.platform 1.0
import QtQml 2.3

ApplicationWindow
{
    id:qml_main_window_sources
    property color colorBorder: "darkred"
    property color colorNormal: "red"
    property color colorPressed: "#ffc600"
    property color colorHover: "#0b0a0a"
    property string source: ""
    property int default_pix_density: 4
    property int scale_factor: Screen.pixelDensity/default_pix_density
    width: 950
    height:550
    minimumWidth: 950
    minimumHeight: 550
    maximumWidth: 950
    maximumHeight: 550
    visible: true

    modality: Qt.ApplicationModal

    title: source_selector_obj.pbin_get_label("App_Name")

    onClosing:
    {
        source_selector_obj.pbin_on_close_clicked()
    }


    StackView
    {
        id: stackview_source
        x: 8
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        initialItem: component_evidence_type

        pushEnter: Transition
        {
            PropertyAnimation
            {
                property: "opacity"
                from: 0
                to: 1
                duration: 10
            }
        }

        //*******************Add New Source*****************

        Component
        {
            id:component_evidence_type
            Evidence_type_selector {}
        }

        //***************Evidences*********************

        Component
        {
            id:component_physical_evidences
            Evidence_physical {}
        }

        Component
        {
            id:component_logical_evidences
            Evidence_logical {}
        }

        Component
        {
            id:component_mobile_evidences
            Evidence_mobile {}
        }

        Component
        {
            id:component_cloud_evidences
            Evidence_cloud {}
        }

        Component
        {
            id: component_network_acquisition
            Evidence_network {}
        }

        //**************Evidences images types******************

        Component
        {
            id: component_recon_fs_block_image
            Recon_fs_block_image_selector {}
        }

        Component
        {
            id: component_recon_logical_image
            Recon_logical_image_selector {}
        }

        Component
        {
            id: component_encase_logical_image
            Encase_logical_image_selector {}
        }

        Component
        {
            id: component_cellebrite_ios_backup
            Cellebrite_ios_backup_selector {}
        }

        Component
        {
            id: component_graykey_backup
            Graykey_backup_selector {}
        }

        Component
        {
            id: component_adb_android_backup
            Adb_android_backup_selector {}
        }

        Component
        {
            id: component_recon_mac_sharing_mode
            Recon_mac_sharing_mode_selector {}
        }

        //**************Evidences OS types******************

        Component
        {
            id:component_forensic_image_os_type
            Forensic_vault_fusion_image_os_selector {}
        }

        //**************Images paths***********************

        Component
        {
            id: component_forensic_macOS_image_path
            Forensic_vault_fusion_image_macos {}
        }

        Component
        {
            id: component_forensic_winOS_image_path
            Forensic_image_winos {}
        }

        Component
        {
            id: component_forensic_otherOS_image_path
            Forensic_image_otheros {}
        }

        Component
        {
            id: component_optical_disc_image
            Optical_disc_image {}
        }

        Component
        {
            id: component_ram_image
            Ram_image {}
        }

        Component
        {
            id: component_recon_fs_block_image_sparseimage
            Recon_fs_block_image_sparseimage {}
        }

        Component
        {
            id: component_recon_fs_block_image_dmg
            Recon_fs_block_image_dmg {}
        }

        Component
        {
            id: component_time_machine_backup
            Time_machine_backup_folder {}
        }

        Component
        {
            id: component_time_machine_image
            Time_machine_backup_image {}
        }

        Component
        {
            id: component_macos_home_directory
            Macos_home_directory {}
        }

        Component
        {
            id: component_encase_logical_image_l01
            Encase_logical_image_l01 {}
        }

        Component
        {
            id: component_folder
            Folder {}
        }

        Component
        {
            id: component_file
            File {}
        }

        Component
        {
            id: component_recon_logical_image_dmg_path
            Recon_logical_image_dmg {}
        }

        Component
        {
            id: component_logical_sparseimg
            Recon_logical_image_sparseimage {}
        }

        Component
        {
            id: component_logical_recon_folder
            Recon_logical_image_folder {}
        }

        Component
        {
            id: component_itunes_ios_backup
            Itunes_ios_backup {}
        }

        Component
        {
            id:cellebrite_ios_backup_tar
            Cellebrite_ios_backup_tar {}
        }

        Component
        {
            id: cellebrite_ios_backup_ufdr
            Cellebrite_ios_backup_ufdr {}
        }

        Component
        {
            id: component_graykey_ios_backup
            Graykey_ios_backup {}
        }

        Component
        {
            id: component_graykey_android_backup
            Graykey_android_backup {}
        }

        Component
        {
            id: adb_android_backup
            Adb_android_backup_ab {}
        }

        Component
        {
            id: adb_android_backup_folder
            Adb_android_backup_folder {}
        }

        Component
        {
            id: component_google_takeout
            Google_takeout {}
        }

        Component
        {
            id: component_recon_mac_sharing_mode_sparseimage
            Recon_mac_sharing_mode_sparseimage {}
        }

        Component
        {
            id: component_recon_mac_sharing_mode_dmg
            Recon_mac_sharing_mode_dmg {}
        }

        Component
        {
            id: component_recon_mac_sharing_mode_folder
            Recon_mac_sharing_mode_folder {}
        }

    }
}
