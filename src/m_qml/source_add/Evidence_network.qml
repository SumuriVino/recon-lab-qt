import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Basic 6.0
/*This class has the front-end UI work behind the Mobile evidence button. When we click on Mobile evidence, the buttons you will see are like RECON MAC Sharing Mode.*/
Rectangle
{
    id: rectangle_network_acq_recon_mac_sharing_mode
    x: 0
    y: 0
    width: 950
    height: 550

    Image
    {
        id: image_network_acq_recon_mac_sharing_mode
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Button
        {
            id: button_network_acq_recon_mac_sharing_mode
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
                id: rectangle_network_mac_sharing_mode
                x: 0
                y: 0
                width: 100
                height: 100
                color: "#0b0a0a"
                radius: 40


                Image
                {
                    fillMode: Image.PreserveAspectFit
                    id: img_network_mac_sharing_mode
                    source: "file:/" + source_selector_obj.pbin_get_icons_path(source_selector_obj.pbin_get_label("RootType_RECONMACSharingMode"))
                    anchors.fill: parent
                }
            }

            MouseArea
            {
                id: mousearea_network_mac_sharing_mode
                x: 0
                y: 6
                width: 100
                height: 100
                anchors.fill: parent
                hoverEnabled: true
                onPressed:
                {
                    rectangle_network_mac_sharing_mode.color = qml_main_window_sources.colorPressed
                }
                onReleased:
                {
                    rectangle_network_mac_sharing_mode.color = qml_main_window_sources.colorHover
                }

                onEntered:
                {
                    rectangle_network_mac_sharing_mode.color = qml_main_window_sources.colorHover
                }

                onClicked:
                {
                    stackview_source.push("qrc:/Recon_mac_sharing_mode_selector.qml")
                }

            }
        }
    }

    Label
    {
        id: label_network_acq_mac_sharing_mode
        x: 379
        y: 336
        width: 194
        height: 21
        text: source_selector_obj.pbin_get_label("RootType_RECONMACSharingMode")
        font.pixelSize: 16
        font.bold: true
        font.capitalization: Font.MixedCase
        color: "#ffc600"
    }

    Button
    {
        id: button_back_network_acq
        x: 14
        y: 16
        width: 45
        height: 45
        background: Rectangle
        {
            id:rectangle_back_network_acq
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
              + source_selector_obj.pbin_get_label("EvidenceType_Network")
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
