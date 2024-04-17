import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0

Rectangle
{
    id: rectangle_physical_forensic_image_os_type
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_physical_forensic_image_os_type
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_physical_windows
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
                id: rectangle_physical_windows
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_physical_windows
                    width: 100
                    height: 100
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("OS_SCHEME_winOS_Display"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_physical_windows
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_physical_windows.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_physical_windows.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_physical_windows.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Forensic_image_winos.qml")
                }
            }
        }

        Button
        {
            id: button_physical_macos
            x: 206
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
                id: rectangle_physical_macos
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_physical_macos
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("OS_SCHEME_macOS_Display"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_physical_macos
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_physical_macos.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_physical_macos.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_physical_macos.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Forensic_vault_fusion_image_macos.qml")
                }

            }
        }

        Button
        {
            id: button_physical_others
            x: 669
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
                id: rectangle_physical_others
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_physical_others
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("OS_SCHEME_otherOS_Display"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_physical_others
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_physical_others.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_physical_others.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_physical_others.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Forensic_image_otheros.qml")
                }
            }
        }
    }

    Label
    {
        id: label_physical_windows
        x: 451
        y: 334
        width: 51
        height: 21
        text: source_selector_obj.pbin_get_label("OS_SCHEME_winOS_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_others
        x: 692
        y: 334
        width: 51
        height: 21
        text: source_selector_obj.pbin_get_label("OS_SCHEME_otherOS_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"

    }

    Label
    {
        id: label_physical_macos
        x: 230
        y: 334
        width: 54
        height: 21
        text: source_selector_obj.pbin_get_label("OS_SCHEME_macOS_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"

    }

    Button
    {
        id: button_back_physical_os
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_physical_os
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
              + source_selector_obj.pbin_get_label("RootType_ForensicsImage")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"

    }

    Label
    {
        id: label_os_type
        x: 273
        y: 100
        width: 404
        height: 41
        text: source_selector_obj.pbin_get_label("Select_Operating_System")
        font.bold: true
        font.pointSize: 35
        color: "#ffc600"

    }
}
