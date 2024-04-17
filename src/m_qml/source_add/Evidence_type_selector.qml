import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
import QtQuick.Dialogs 6.0
import Qt.labs.platform 1.0


Rectangle
{
    id: rectangle_evidences
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_rectangle_evidences
        x: 0
        y: 0
        width: 950
        height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_physical_evidence
            x: 94
            y: 225
            width: 100
            height: 100
            visible: true
            font.letterSpacing: 0.3
            display: AbstractButton.IconOnly
            enabled: true
            focusPolicy: Qt.StrongFocus
            wheelEnabled: true
            autoRepeat: false
            background:  Rectangle
            {
                id: rectangle_physical_evidence
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_physical_evidence
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("EvidenceType_Physical"))
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_physical_evidence
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_physical_evidence.color = qml_main_window_sources.colorPressed

                }
                onReleased:
                {
                    rectangle_physical_evidence.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_physical_evidence.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Evidence_physical.qml")
                }

            }
        }

        Button
        {
            id: button_logical_evidence
            x: 261
            y: 225
            width: 100
            height: 100
            layer.textureSize.height: 0
            layer.textureSize.width: 0
            display: AbstractButton.IconOnly
            autoRepeat: false
            focus: false
            transformOrigin: Item.Center
            background:  Rectangle
            {
                id: rectangle_logical_evidence
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40

                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_logical_evidence
                    width: 100
                    height: 100
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("EvidenceType_Logical"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_logical_evidence
                x: 8
                y: 10
                width: 100
                height: 100
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: -1
                anchors.leftMargin: 0
                anchors.topMargin: 1
                hoverEnabled: true
                onPressed:
                {
                    rectangle_logical_evidence.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_logical_evidence.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_logical_evidence.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Evidence_logical.qml")
                }
            }
        }

        Button
        {
            id: button_cloud_evidence
            x: 599
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            background:  Rectangle
            {
                id: rectangle_cloud_evidence
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40
                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_cloud_evidence
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("EvidenceType_Cloud"))
                    anchors.fill: parent
                }
            }
            MouseArea
            {
                id: mousearea_cloud_evidence
                x: 8
                y: 10
                width: 100
                height: 100
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: -6
                anchors.leftMargin: 5
                anchors.topMargin: 4
                hoverEnabled: true
                onPressed:
                {
                    rectangle_cloud_evidence.color = qml_main_window_sources.colorPressed
                }

                onReleased:
                {
                    rectangle_cloud_evidence.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_cloud_evidence.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Evidence_cloud.qml")
                }
            }
        }

        Button
        {
            id: button_mobile_evidence
            x: 425
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            background:  Rectangle
            {
                id: rectangle_mobile_evidence
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40
                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: image_mobile_evidence
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("EvidenceType_Mobile"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_mobile_evidence
                x: 8
                y: 10
                width: 100
                height: 100
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                hoverEnabled: true
                onPressed:
                {
                    rectangle_mobile_evidence.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_mobile_evidence.color = qml_main_window_sources.colorHover
                }
                onEntered:
                {
                    rectangle_mobile_evidence.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Evidence_mobile.qml")
                }
            }
        }

        Button
        {
            id: button_network_acquisition
            x: 774
            y: 225
            width: 100
            height: 100
            display: AbstractButton.IconOnly
            focus: false
            background:  Rectangle
            {
                id: rectangle_network_acquisition
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40
                Image {
                    fillMode: Image.PreserveAspectFit
                    id: image_network_acquisition
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("EvidenceType_Network"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_network_acquisition
                x: 8
                y: 10
                width: 100
                height: 100
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: 1
                hoverEnabled: true
                onPressed:
                {
                    rectangle_network_acquisition.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_network_acquisition.color = qml_main_window_sources.colorHover
                }
                onEntered:
                {
                    rectangle_network_acquisition.color = qml_main_window_sources.colorHover
                }
                onClicked:
                {
                    stackview_source.push("qrc:/Evidence_network.qml")
                }
            }
        }
    }

    Button
    {
        id: button_back_evidence
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:button_rectangle_back
            color: "#000000"
            radius: 20
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
            source_selector_obj.pbin_button_back_clicked()
            setVisible(false)
        }
    }

    Label
    {
        id: label_physical_evidence
        x: 75
        y: 334
        width: 148
        height: 27
        color: "#ffc600"
        text: source_selector_obj.pbin_get_label("EvidenceType_Physical")
        font.bold: true
        font.pointSize: 16
    }

    Label
    {
        id: label_logical_evidence
        x: 248
        y: 334
        width: 129
        height: 27
        color: "#ffc600"
        text: source_selector_obj.pbin_get_label("EvidenceType_Logical")
        font.bold: true
        font.pointSize: 16
    }

    Label
    {
        id: label_mobile_evidence
        x: 411
        y: 334
        width: 129
        height: 27
        color: "#ffc600"
        text: source_selector_obj.pbin_get_label("EvidenceType_Mobile")
        font.bold: true
        font.pointSize: 16
    }

    Label
    {
        id: label_cloud_evidence
        x: 586
        y: 334
        width: 128
        height: 27
        color: "#ffc600"
        text: source_selector_obj.pbin_get_label("EvidenceType_Cloud")
        font.bold: true
        font.pointSize: 16
    }

    Label
    {
        id: label_network_acquisition
        x: 751
        y: 334
        width: 158
        height: 27
        color: "#ffc600"
        text: source_selector_obj.pbin_get_label("EvidenceType_Network")
        font.bold: true
        font.pointSize: 16
    }

    Label
    {
        id: label_select_evidence_type_heading
        x: 261
        y: 100
        width: 429
        height: 41
        color: "#ffc600"
        text: source_selector_obj.pbin_get_label("Select_Evidence_Type")
        font.italic: false
        font.pointSize: 35
        font.bold: true
    }

}
