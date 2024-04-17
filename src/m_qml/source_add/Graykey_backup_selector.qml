import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0

Rectangle
{
    id: rectangle_mobile_evidence_graykey_backup
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_mobile_evidence_graykey_backup
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_graykey_iOS_backup
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
                id: rectangle_graykey_iOS_backup
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_graykey_iOS_backup
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_GrayKeyiOSBackup_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_graykey_iOS_backup
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_graykey_iOS_backup.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_graykey_iOS_backup.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_graykey_iOS_backup.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Graykey_ios_backup.qml")
                }

            }
        }

        Button
        {
            id: button_graykey_android_backup
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
                id: rectangle_graykey_android_backup
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40
                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_graykey_android_backup
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_GrayKeyAndroidBackup_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_graykey_android_backup
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_graykey_android_backup.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_graykey_android_backup.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_graykey_android_backup.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Graykey_android_backup.qml")
                }

            }
        }
    }

    Label
    {
        id: label_graykey_iOS_backup
        x: 264
        y: 333
        width: 153
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_GrayKeyiOSBackup_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label
    {
        id: label_graykey_android_backup
        x: 458
        y: 333
        width: 161
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_GrayKeyAndroidBackup_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_graykey_backup
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_graykey_backup
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
            stackview_source.push("qrc:/Evidence_mobile.qml")
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
              + source_selector_obj.pbin_get_label("EvidenceType_Mobile")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_GrayKeyBackup")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"

    }

    Label
    {
        id: label_backup_type
        x: 283
        y: 100
        width: 385
        height: 41
        text: source_selector_obj.pbin_get_label("Select_Backup_Type")
        font.bold: true
        font.pointSize: 35
        color: "#ffc600"

    }
}
