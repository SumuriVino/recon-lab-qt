import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
/*This class has the front-end UI work behind the RECON Logical Image button. When we click on this button, the further buttons you will see are like
RECON Logical image dmg, sparseimage and folder.*/
Rectangle
{
    id: rectangle_recon_logical_image
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_recon_logical_image
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_logical_sparseimage
            x: 200
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
                id: rectangle_logical_sparseimage
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_logical_sparseimage
                    width: 100
                    height: 100
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_Sparseimage_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_logical_sparseimage
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_sparseimage.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_sparseimage.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_sparseimage.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Recon_logical_image_sparseimage.qml")
                }

            }
        }

        Button
        {
            id: button_logical_dmg
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
                id: rectangle_logical_dmg
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_logical_dmg
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_DMG_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_logical_dmg
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_dmg.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_dmg.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_dmg.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Recon_logical_image_dmg.qml")
                }

            }
        }

        Button
        {
            id: button_logical_folder
            x: 650
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
                id: rectangle_logical_folder
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image {
                    fillMode: Image.PreserveAspectFit
                    id: img_logical_folder
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_Folder_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_logical_folder
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_folder.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_folder.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_folder.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Recon_logical_image_folder.qml")
                }
            }
        }
    }

    Label
    {
        id: label_logical_sparseimage
        x: 205
        y: 334
        width: 54
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_Sparseimage_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"

    }

    Label {
        id: label_logical_dmg
        x: 455
        y: 334
        width: 38
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_DMG_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label {
        id: label_logical_folder
        x: 680
        y: 334
        width: 51
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_RECONLogicalImage_Folder_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_recon_logical_image
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_recon_logical_image
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
            stackview_source.push("qrc:/Evidence_logical.qml")
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
              + source_selector_obj.pbin_get_label("EvidenceType_Logical")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_RECONLogicalImage")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"
    }

    Label
    {
        id: label_image_type
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
