import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
/*This class has the front-end UI work behind the ADB Android backup button. When we click on the ADB Android backup button,
 the buttons you will see are like adb android backup ab, folder.*/
Rectangle
{
    id: rectangle_mobile_adb_android_selector
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_mobile_adb_android_selector
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_mobile_ab
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
                id: rectangle_mobile_ab
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_mobile_ab
                    width: 100
                    height: 100
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_ADBAndroidBackup_AB_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_mobile_ab
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_ab.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_ab.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_mobile_ab.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Adb_android_backup_ab.qml")
                }

            }
        }

        Button
        {
            id: button_mobile_folder
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
                id: rectangle_mobile_folder
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_mobile_folder
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("SourceType_ADBAndroidBackup_Folder_Internal"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_mobile_folder
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_folder.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_folder.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_mobile_folder.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Adb_android_backup_folder.qml")
                }

            }
        }
    }

    Label
    {
        id: label_mobile_ab
        x: 339
        y: 331
        width: 25
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_ADBAndroidBackup_AB_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label
    {
        id: label_mobile_folder
        x: 529
        y: 331
        width: 49
        height: 21
        text: source_selector_obj.pbin_get_label("SourceType_ADBAndroidBackup_Folder_Display")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_mobile_adb
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_mobile_adb
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
              + source_selector_obj.pbin_get_label("RootType_ADBAndroidBackup")
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
        text: source_selector_obj.pbin_get_label("Select_Image_Type")
        font.bold: true
        font.pointSize: 35
        color: "#ffc600"
    }
}
