import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0

Rectangle
{
    id: rectangle_physical_fs_block_sparseimage
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_physical_fs_block_sparseimage
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_physical_fs_block_sparseimage
            x: 300
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
                id: rectangle_recon_fs_block_sparseimage
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_recon_fs_block_sparseimage
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONFSBlockImage_Sparseimage_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_recon_fs_block_sparseimage
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_recon_fs_block_sparseimage.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_recon_fs_block_sparseimage.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_recon_fs_block_sparseimage.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Recon_fs_block_image_sparseimage.qml")
                }

            }
        }

        Button
        {
            id: button_physical_fs_block_dmg
            x: 500
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
                id: rectangle_recon_fs_block_dmg
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_recon_fs_block_dmg
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONFSBlockImage_DMG_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_recon_fs_block_dmg
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_recon_fs_block_dmg.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_recon_fs_block_dmg.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_recon_fs_block_dmg.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Recon_fs_block_image_dmg.qml")
                }

            }
        }
    }

    Label
    {
        id: label_recon_fs_block_sparseimage
        x: 301
        y: 333
        width: 51
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONFSBlockImage_Sparseimage_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"

    }

    Label
    {
        id: label_recon_fs_block_dmg
        x: 529
        y: 333
        width: 51
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_DMG_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_recon_fs_block_sparseimage
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_recon_fs_block_sparseimage
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
            stackview_source.push("qrc:/Evidence_physical.qml")
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
              + source_selector_obj.pbin_get_label("EvidenceType_Physical")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_RECONFSBlockImage")
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
