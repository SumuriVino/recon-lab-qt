import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0

Rectangle
{
    id: rectangle_logical_evidences
    x: 0
    y: 0
    width: 950
    height: 550
    anchors.centerIn: parent

    Image
    {
        id: image_rectangle_logical_evidences
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_logical_time_machine_backup_folder
            x: 50
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
                id: rectangle_logical_time_machine_backup_folder
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_logical_time_machine_backup_folder
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_TimeMachineBackupFolder"))
                    anchors.fill: parent
                }
            }
            MouseArea
            {
                id: mousearea_logical_time_machine_backup_folder
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_time_machine_backup_folder.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_time_machine_backup_folder.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_time_machine_backup_folder.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Time_machine_backup_folder.qml")
                }
            }
        }

        Button
        {
            id: button_logical_home_dir
            x: 218
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
                id: rectangle_logical_home_dir
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image {
                    fillMode: Image.PreserveAspectFit
                    id: image_logical_home_dir
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_macOSHomeDirectory"))
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_logical_home_dir
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_home_dir.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_home_dir.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_home_dir.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Macos_home_directory.qml")
                }
            }
        }
    }

    Button
    {
        id: button_back_logical
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_logical
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
        id: button_recon_logical_image
        x: 366
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_recon_logical_image
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40


            Image
            {
                fillMode: Image.PreserveAspectFit
                id: image_recon_logical_image
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_RECONLogicalImage"))
                anchors.fill: parent
            }
        }

        MouseArea
        {
            id: mousearea_recon_logical_image
            x: 0
            y: 6
            width: 100
            height: 100
            anchors.fill: parent
            hoverEnabled: true
            onPressed:
            {
                rectangle_recon_logical_image.color = qml_main_window_sources.colorPressed
            }
            onReleased:
            {
                rectangle_recon_logical_image.color = qml_main_window_sources.colorHover
            }
            onEntered:
            {
                rectangle_recon_logical_image.color = qml_main_window_sources.colorHover
            }
            onClicked:
            {
                stackview_source.push("qrc:/Recon_logical_image_selector.qml")
            }
        }
    }

    Button
    {
        id: button_encase_logical_image
        x: 520
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_encase_logical_image
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40
            Image
            {
                fillMode: Image.PreserveAspectFit
                id: img_encase_logical_image
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_EncaseLogicalImage"))
                anchors.fill: parent
            }
        }

        MouseArea
        {
            id: mousearea_encase_logical_image
            x: 0
            y: 6
            width: 100
            height: 100
            anchors.fill: parent
            hoverEnabled: true
            onPressed:
            {
                rectangle_encase_logical_image.color = qml_main_window_sources.colorPressed
            }
            onReleased:
            {
                rectangle_encase_logical_image.color = qml_main_window_sources.colorHover
            }

            onEntered:
            {
                rectangle_encase_logical_image.color = qml_main_window_sources.colorHover
            }

            onClicked:
            {
                stackview_source.push("qrc:/Encase_logical_image_selector.qml")
            }
        }
    }

    Button
    {
        id: button_logical_folder
        x: 673
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
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
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_Folder"))
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
                stackview_source.push("qrc:/Folder.qml")
            }

        }
    }

    Button
    {
        id: button_logical_file
        x: 810
        y: 225
        width: 100
        height: 100
        display: AbstractButton.IconOnly
        background:  Rectangle
        {
            id: rectangle_logical_file
            x: 0
            y: 0
            width: 100
            height: 100
            color: "#0b0a0a"
            radius: 40

            MouseArea
            {
                id: mousearea_logical_file
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_file.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_file.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_file.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/File.qml")
                }

            }

            Image {
                fillMode: Image.PreserveAspectFit
                id: img_logical_file
                source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_File"))
                anchors.fill: parent
            }
        }
    }


    Label
    {
        id: label_logical_time_machine_backup_folder
        x: 14
        y: 334
        width: 114
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_TimeMachineBackupFolder")
        font.bold: true
        font.italic: false
        font.underline: false
        font.strikeout: false
        font.pointSize: 12
        color: "#ffc600"
    }

    Label
    {
        id: label_logical_home_dir
        x: 207
        y: 334
        width: 103
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_macOSHomeDirectory")
        font.italic: false
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"
    }

    Label
    {
        id: label_logical_recon_logical_image
        x: 360
        y: 334
        width: 127
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_RECONLogicalImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"
    }

    Label
    {
        id: label_logical_encase_logical_image
        x: 517
        y: 334
        width: 78
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_EncaseLogicalImage")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"
    }

    Label
    {
        id: label_logical_folder
        x: 704
        y: 334
        width: 60
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_Folder")
        font.bold: true
        font.pointSize: 12
        color: "#ffc600"
    }

    Label
    {
        id: label_logical_file
        x: 852
        y: 334
        width: 72
        height: 20
        text: source_selector_obj.pbin_get_label("RootType_File")
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
              + source_selector_obj.pbin_get_label("EvidenceType_Logical")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"
    }
}
