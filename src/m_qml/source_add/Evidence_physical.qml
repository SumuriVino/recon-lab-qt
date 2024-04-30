import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
/*This class has the front-end UI work behind the Physical evidence button. When we click on physical evidence, the buttons you will see like Ram image,
optical disk image, forensics image, time machine backup, mounted volumes, recon fs block etc. */
Rectangle
{
    id: rectangle_physical_evidences
    x: 0
    y: 0
    width: 950
    height: 550
    anchors.centerIn: parent

    Image
    {
        id: image_physical_evidences
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_physical_mounted_volume
            x: 194
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            enabled: true
            focusPolicy: Qt.StrongFocus
            wheelEnabled: true
            autoRepeat: false
            background:  Rectangle
            {
                id: rectangle_physical_mounted_volume
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_physical_mounted
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_MountedVolumes"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_physical_mounted_volume
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_physical_mounted_volume.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_physical_mounted_volume.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_physical_mounted_volume.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    source_selector_obj.pbin_button_add_selected_sources(source_selector_obj.pbin_get_label("EvidenceType_Physical"),source_selector_obj.pbin_get_label("RootType_MountedVolumes"),source_selector_obj.pbin_get_label("SourceType_MountedVolumes_Display"),source_selector_obj.pbin_get_label("SourceType_MountedVolumes_Internal"),"","","","")
                    setVisible(false)
                }
            }
        }

        Button
        {
            id: button_physical_forensic_image
            x: 47
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            enabled: true
            focusPolicy: Qt.StrongFocus
            wheelEnabled: true
            autoRepeat: false
            background:  Rectangle
            {
                id: rectangle_physical_forensic_img
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_physical_forensic
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_ForensicsImage"))
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_physical_forensic_img
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_physical_forensic_img.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_physical_forensic_img.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_physical_forensic_img.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Forensic_vault_fusion_image_os_selector.qml")
                }
            }
        }
    }

    Button
    {
        id: button_back_physical_evidences
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangke_back_physical_evidences
            radius:20
            color: "#000000"
            Image
            {
                id: image_back
                width: 45
                height: 45
                source: "file:/" + source_selector_obj.pbin_get_icons_path("back")
            }
        }
        onClicked:
        {
            stackview_source.push("qrc:/Evidence_type_selector.qml")
        }
    }

    Button
    {
        id: button_physical_optical_disc_image
        x: 346
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_physical_optical_disc_image
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40

            Image
            {
                fillMode: Image.PreserveAspectFit
                id: img_physical_optical_disc
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_OpticalDiscImage"))
                anchors.fill: parent
            }
        }

        MouseArea
        {
            id: mousearea_physical_optical_disc_image
            x: 0
            y: 6
            width: 100
            height: 100
            anchors.fill: parent
            hoverEnabled: true
            onPressed:
            {
                rectangle_physical_optical_disc_image.color = qml_main_window_sources.colorPressed
            }
            onReleased:
            {
                rectangle_physical_optical_disc_image.color = qml_main_window_sources.colorHover
            }
            onEntered:
            {
                rectangle_physical_optical_disc_image.color = qml_main_window_sources.colorHover
            }
            onClicked:
            {
                stackview_source.push("qrc:/Optical_disc_image.qml")
            }
        }
    }

    Button
    {
        id: button_physical_recon_fs_block_image
        x: 633
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_physical_recon_fs_block_image
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40

            Image
            {
                fillMode: Image.PreserveAspectFit
                id: img_physical_sparseimage
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_RECONFSBlockImage"))
                anchors.fill: parent
            }
        }

        MouseArea
        {
            id: mousearea_physical_recon_fs_block_image
            x: 0
            y: 6
            width: 100
            height: 100
            anchors.fill: parent
            hoverEnabled: true
            onPressed:
            {
                rectangle_physical_recon_fs_block_image.color = qml_main_window_sources.colorPressed
            }
            onReleased:
            {
                rectangle_physical_recon_fs_block_image.color = qml_main_window_sources.colorHover
            }

            onEntered:
            {
                rectangle_physical_recon_fs_block_image.color = qml_main_window_sources.colorHover
            }

            onClicked:
            {
                stackview_source.push("qrc:/Recon_fs_block_image_selector.qml")
            }
        }
    }

    Button
    {
        id: button_physical_ram_image
        x: 487
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_physical_ram_image
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40


            Image
            {
                fillMode: Image.PreserveAspectFit
                id: image_ram_image
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_RAMImage"))
                anchors.fill: parent
            }
        }

        MouseArea
        {
            id: mousearea_physical_ram_image
            x: 0
            y: 6
            width: 100
            height: 100
            anchors.fill: parent
            hoverEnabled: true
            onPressed:
            {
                rectangle_physical_ram_image.color = qml_main_window_sources.colorPressed
            }
            onReleased:
            {
                rectangle_physical_ram_image.color = qml_main_window_sources.colorHover
            }

            onEntered:
            {
                rectangle_physical_ram_image.color = qml_main_window_sources.colorHover
            }

            onClicked:
            {
                stackview_source.push("qrc:/Ram_image.qml")
            }

        }
    }

    Button
    {
        id: button_physical_time_machine_backup_image
        x: 805
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_physical_time_machine_backup_image
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40

            Image
            {
                fillMode: Image.PreserveAspectFit
                id: image_time_machine_backup_image
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_TimeMachineBackupImage"))
                anchors.fill: parent
            }
        }

        MouseArea
        {
            id: mousearea_physical_time_machine_backup_image
            x: 0
            y: 6
            width: 100
            height: 100
            anchors.fill: parent
            hoverEnabled: true
            onPressed:
            {
                rectangle_physical_time_machine_backup_image.color = qml_main_window_sources.colorPressed
            }
            onReleased:
            {
                rectangle_physical_time_machine_backup_image.color = qml_main_window_sources.colorHover
            }

            onEntered:
            {
                rectangle_physical_time_machine_backup_image.color = qml_main_window_sources.colorHover
            }

            onClicked:
            {
                stackview_source.push("qrc:/Time_machine_backup_image.qml")
            }

        }
    }


    Label
    {
        id: label_physical_ram_image
        x: 500
        y: 334
        width: 78
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_RAMImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_optical_disc
        x: 336
        y: 334
        width: 127
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_OpticalDiscImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_forensic_image
        x: 46
        y: 334
        width: 103
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_ForensicsImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_mounted_volume
        x: 188
        y: 334
        width: 114
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_MountedVolumes")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_recon_fs_block_image
        x: 616
        y: 334
        width: 96
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_RECONFSBlockImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_time_machine_image
        x: 767
        y: 334
        width: 164
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_TimeMachineBackupImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"
    }

    Label
    {
        id: label_select_image_type_heading
        x: 283
        y: 100
        width: 385
        height: 41
        text: source_selector_obj.pbin_get_label("Select_Image_Type")
        font.bold: true
        font.pointSize: 35
        color: "#ffc600"

    }

    Label
    {
        id: label_source_path
        x: 8
        y: 527
        width: 145
        height: 16
        text: source_selector_obj.pbin_get_label("[")
              + source_selector_obj.pbin_get_label("EvidenceType_Physical")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"

    }
}
