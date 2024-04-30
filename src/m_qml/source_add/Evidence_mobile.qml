import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
/*This class has the front-end UI work behind the Mobile evidence button. When we click on Mobile evidence, the buttons you will see are like iTunes iOS backup,
 cellebrite iOS backup, adb android backup, Greykey backup etc.*/
Rectangle
{
    id: rectangle_mobile_evidence
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_mobile_evidences
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_mobile_cellebrite_ios_backup
            x: 325
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
                id: rectangle_mobile_cellebrite_ios_backup
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_mobile_cellebrite_ios_backup
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_CellebriteiOSBackup"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_mobile_cellebrite_ios_backup
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_cellebrite_ios_backup.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_cellebrite_ios_backup.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_mobile_cellebrite_ios_backup.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Cellebrite_ios_backup_selector.qml")
                }

            }
        }

        Button
        {
            id: button_mobile_graykey_backup
            x: 520
            y: 225
            width: 100
            height: 100
            autoRepeat: false
            background: Rectangle
            {
                id: rectangle_mobile_graykey_backup
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    id: img_mobile_graykey_backup
                    width: 100
                    height: 100
                    anchors.fill: parent
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_GrayKeyBackup"))
                    fillMode: Image.PreserveAspectFit
                }
            }

            MouseArea
            {
                id: mousearea_graykey_backup
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_graykey_backup.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_graykey_backup.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_mobile_graykey_backup.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Graykey_backup_selector.qml")
                }
            }

            wheelEnabled: true
            enabled: true
            display: AbstractButton.IconOnly
            focusPolicy: Qt.StrongFocus
        }

        Button
        {
            id: button_mobile_adb_android_backup
            x: 715
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
                id: rectangle_mobile_adb_android_backup
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_mobile_adb_android_backup
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_ADBAndroidBackup"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_mobile_adb_android_backup
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_adb_android_backup.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_adb_android_backup.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_mobile_adb_android_backup.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Adb_android_backup_selector.qml")
                }
            }
        }

        Button
        {
            id: button_mobile_itunes_ios_backup
            x: 120
            y: 225
            width: 100
            height: 100
            autoRepeat: false
            background: Rectangle
            {
                id: rectangle_mobile_itunes_ios_backup
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    id: img_mobile_itunes_ios_backup
                    width: 100
                    height: 100
                    anchors.fill: parent
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_iTunesiOSBackup"))
                    fillMode: Image.PreserveAspectFit
                }
            }

            MouseArea
            {
                id: mousearea_itunes_ios_backup
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_itunes_ios_backup.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_itunes_ios_backup.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_mobile_itunes_ios_backup.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Itunes_ios_backup.qml")
                }
            }

            wheelEnabled: true
            enabled: true
            display: AbstractButton.IconOnly
            focusPolicy: Qt.StrongFocus
        }
    }

    Label
    {
        id: label_mobile_itunes_ios_backup
        x: 104
        y: 334
        width: 129
        height: 21
        text: source_selector_obj.pbin_get_label("RootType_iTunesiOSBackup")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label
    {
        id: label_mobile_cellebrite_ios_backup
        x: 291
        y: 334
        width: 145
        height: 21
        text: source_selector_obj.pbin_get_label("RootType_CellebriteiOSBackup")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Label
    {
        id: label_mobile_adb_android_backup
        x: 689
        y: 334
        width: 155
        height: 21
        text: source_selector_obj.pbin_get_label("RootType_ADBAndroidBackup")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"

    }

    Label
    {
        id: label_mobile_graykey_backup
        x: 501
        y: 334
        width: 155
        height: 21
        text: source_selector_obj.pbin_get_label("RootType_GrayKeyBackup")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_mobile_evidence
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_mobile_evidence
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

    Label
    {
        id: label_source_path
        x: 8
        y: 527
        width: 249
        height: 16
        text: source_selector_obj.pbin_get_label("[")
              + source_selector_obj.pbin_get_label("EvidenceType_Mobile")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"
    }

    Label
    {
        id: label_os_type
        x: 283
        y: 100
        width: 404
        height: 41
        text: source_selector_obj.pbin_get_label("Select_Backup_Type")
        font.bold: true
        font.pointSize: 35
        color: "#ffc600"
    }
}
