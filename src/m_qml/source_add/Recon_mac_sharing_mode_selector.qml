import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0

Rectangle
{
    id: rectangle_network_acq_mac_sharing_mode
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_network_acq_mac_sharing_mode
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_network_acq_sparseimage
            x: 425
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
                id: rectangle_network_acq_sparseimage
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_network_acq_sparseimage
                    width: 100
                    height: 100
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Sparseimage_Internal"))
                    anchors.fill: parent
                }
            }
            MouseArea
            {
                id: mousearea_network_acq_sparseimage
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_network_acq_sparseimage.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_network_acq_sparseimage.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_network_acq_sparseimage.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Recon_mac_sharing_mode_sparseimage.qml")
                }

            }
        }

        Button
        {
            id: button_network_acq_dmg
            x: 200
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            enabled: true
            focusPolicy: Qt.StrongFocus
            wheelEnabled: true
            autoRepeat: false
            font.pointSize: 15
            background:  Rectangle
            {
                id: rectangle_network_acq_dmg
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_network_acq_dmg
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_DMG_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_network_acq_dmg
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_network_acq_dmg.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_network_acq_dmg.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_network_acq_dmg.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Recon_mac_sharing_mode_dmg.qml")
                }
            }
        }

        Button
        {
            id: button_network_acq_folder
            x: 650
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            enabled: true
            focusPolicy: Qt.StrongFocus
            wheelEnabled: true
            autoRepeat: false
            font.pointSize: 15
            background:  Rectangle
            {
                id: rectangle_network_acq_folder
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image {
                    fillMode: Image.PreserveAspectFit
                    id: img_network_acq_folder
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Folder_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_network_acq_folder
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_network_acq_folder.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_network_acq_folder.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_network_acq_folder.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Recon_mac_sharing_mode_folder.qml")
                }
            }
        }
    }

    Label
    {
        id: label_network_acq_sparseimage
        x: 430
        y: 334
        width: 68
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Sparseimage_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label
    {
        id: label_network_acq_dmg
        x: 229
        y: 334
        width: 43
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_DMG_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label
    {
        id: label_network_acq_folder
        x: 678
        y: 334
        width: 54
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Folder_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_network_acq_mac_sharing_mode
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_network_acq_mac_sharing_mode
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
            stackview_source.push("qrc:/Evidence_network.qml")
        }
    }

    Label
    {
        id: label_source_path
        x: 8
        y: 527
        width: 249
        height: 16
        text: source_selector_obj.pbin_get_label("[")
              + source_selector_obj.pbin_get_label("EvidenceType_Network")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_RECONMACSharingMode")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"
    }

    Label
    {
        id: label_os_type
        x: 283
        y: 100
        width: 385
        height: 41
        text: source_selector_obj.pbin_get_label("Select_Image_Type")
        font.bold: true
        font.pointSize: 35
        color: "#ffc600"
    }
}
