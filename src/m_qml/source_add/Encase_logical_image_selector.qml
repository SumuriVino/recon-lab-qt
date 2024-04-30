import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
/* This class has the front-end UI work behind the Encase logical image button. When we click on the Encase logical image button, the buttons you will see are like lo1. */
Rectangle
{
    id: rectangle_encase_logical_image
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_rectangle_encase_logical_image
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_encase_logical_L01
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
                id: rectangle_encase_logical_L01
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_encase_logical_L01
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_EncaseLogicalImage_L01_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_encase_logical_L01
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_encase_logical_L01.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_encase_logical_L01.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_encase_logical_L01.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Encase_logical_image_l01.qml")
                }

            }
        }
    }

    Label
    {
        id: label_encase_logical_L01
        x: 462
        y: 332
        width: 32
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_EncaseLogicalImage_L01_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_encase_logical
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_encase_logical
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
              + source_selector_obj.pbin_get_label("RootType_EncaseLogicalImage")
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
