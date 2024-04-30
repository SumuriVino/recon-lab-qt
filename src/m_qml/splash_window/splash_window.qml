import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Universal 6.0
import QtQuick.Controls 2.12
/*This class is the main class which up when we open the recon lab. This is the one which includes buttons of new case, load case, acquire and other main screen buttons. We can say this
is the front end UI work of the main splash screen of RECON LAB. This class is connected to case_wizard.cpp which has all the backend work of this UI.
*/
Page
{
    width: 951
    height: 559
    visible: true
    title: case_wizard_obj.pbin_get_label("app_name")

    Rectangle
    {
        id: rectangle_splash_window
        x: 0
        y: 0
        width: 951
        height: 559
        color: "#ffffff"

        property bool needToolTip: mousearea_load_case.containsMouse

        Image
        {
            id: image_splash_window
            x: 0
            y: 0
            width: 951
            height: 559
            source: "file:/" + case_wizard_obj.pbin_get_icons_path("lab_splash_image")
            fillMode: Image.Stretch
            Label
            {
                id:label_new_case
                width: 550
                height: 200
                property string tooltipText: "New Case"
                ToolTip.text: tooltipText
                ToolTip.visible: tooltipText ? mousearea_new_case.containsMouse : false

                MouseArea
                {
                    id: mousearea_new_case
                    x: 105
                    y: 186
                    width: 153
                    height: 161
                    onClicked: case_wizard_obj.pbin_button_on_splash_window_clicked("new_case")
                    hoverEnabled: true

                    Image
                    {
                        id: image_new_case
                        x: 8
                        y: 3
                        width: 130
                        height: 130
                        source: "file:/" + case_wizard_obj.pbin_get_icons_path("new_case")
                        fillMode: Image.PreserveAspectFit
                    }
                }

            }
            Label
            {
                id:label_load_case
                width: 1125
                height: 200
                property string tooltip_load_case: "Load Existing Case"
                ToolTip.text: tooltip_load_case
                ToolTip.visible: tooltip_load_case ? mousearea_load_case.containsMouse : false

                MouseArea
                {
                    id: mousearea_load_case
                    x: 396
                    y: 186
                    width: 155
                    height: 161
                    onClicked: case_wizard_obj.pbin_button_on_splash_window_clicked("load_case")
                    hoverEnabled: true

                    Image
                    {
                        id: image_load_case
                        x: 8
                        y: 3
                        width: 130
                        height: 130
                        source: "file:/" + case_wizard_obj.pbin_get_icons_path("load_case")
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
            Label
            {
                id:label_acquire
                width: 1750
                height: 200
                property string tooltip_acquire: "Acquire iOS Device"
                ToolTip.text: tooltip_acquire
                ToolTip.visible: tooltip_acquire ? mousearea_acquire.containsMouse : false

                MouseArea
                {
                    id: mousearea_acquire
                    x: 689
                    y: 186
                    width: 154
                    height: 161
                    onClicked: case_wizard_obj.pbin_button_on_splash_window_clicked("acquire_ios_device")
                    hoverEnabled: true

                    Image
                    {
                        id: image_acquire
                        x: 8
                        y: 3
                        width: 130
                        height: 130
                        source: "file:/" + case_wizard_obj.pbin_get_icons_path("acquire_ios_device")
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }

            Button
            {
                id: button_settings
                x: 20
                y: 489
                width: 55
                height: 55
                display: AbstractButton.IconOnly
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.text: "RECON Configuration"
                background: Rectangle
                {
                    color: "#000000"

                }

                MouseArea
                {
                    id: mousearea_settings
                    anchors.fill: parent
                    onClicked:
                    {
                        case_wizard_obj.pbin_button_on_splash_window_clicked("settings")
                    }
                }

                Image
                {
                    id: image_setting
                    width: 55
                    height: 55
                    source: "file:/" + case_wizard_obj.pbin_get_icons_path("settings")
                    anchors.fill: parent
                }
            }
        }

        Button
        {
            id:button_about
            x:100
            y:489
            width: 55
            height: 55
            hoverEnabled: true
            ToolTip.visible: hovered
            ToolTip.delay: 500
            ToolTip.timeout: 5000
            ToolTip.text: "About RECON"
            background: Rectangle
            {
                color: "#000000"
            }

            MouseArea
            {
                id: mousearea_about
                anchors.fill: parent
                onClicked: case_wizard_obj.pbin_button_on_splash_window_clicked("about_recon")
            }

            Image
            {
                id: image_about
                width: 55
                height: 55
                source: "file:/" + case_wizard_obj.pbin_get_icons_path("about")
                anchors.verticalCenterOffset: 239
                anchors.horizontalCenterOffset: -373
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
            }
        }

        Button
        {
            id: button_disk_manager
            x:180
            y:489
            width: 55
            height: 55
            hoverEnabled: true
            ToolTip.visible: hovered
            ToolTip.delay: 500
            ToolTip.timeout: 5000
            ToolTip.text: "Disk Manager"
            background: Rectangle
            {
                color: "#000000"
            }
            MouseArea
            {
                id: mousearea_disk_manager
                anchors.fill: parent
                onClicked: case_wizard_obj.pbin_button_on_splash_window_clicked("disk_manager")
            }

            Image
            {
                id: image_disk_manager
                x: 180
                y: 489
                width: 55
                height: 55
                source: "file:/" + case_wizard_obj.pbin_get_icons_path("disk_manager")
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
            }
        }

        Rectangle
        {
            id: button_calendar
            x: 24
            y: 29
            width: 40
            height: button_calendar.width
            radius: 5
            border.width: 0
            anchors.leftMargin: 20
            color: "#000000"

            Image
            {
                id: image_calendar
                x: 24
                y: 29
                width: 35
                height: 35
                anchors.verticalCenter: parent.verticalCenter
                source: "file:/" + case_wizard_obj.pbin_get_icons_path("calendar")
                anchors.verticalCenterOffset: 5
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
            }

            Text
            {
                id: text_license_expiry_date
                x: 41
                y: 5
                height: 22
                width: 158
                color: "#ffffff"
                text: "License Expiry Date"
                font.pixelSize: 16
                minimumPixelSize: 10
                anchors.topMargin: 3
                anchors.leftMargin: 10
                fontSizeMode: Text.VerticalFit
                font.family: "Poppins"
            }

            Text
            {
                id: show_expiry_date
                y: 25
                width: 143
                height: 19
                color: "#ffffff"
                text: case_wizard_obj.pbin_get_label("expiry_date")
                anchors.bottomMargin: -4
                anchors.left: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 16
                anchors.leftMargin: 9
                fontSizeMode: Text.VerticalFit
                font.family: "Poppins"
            }

        }

        Text
        {
            id: text_username
            x: 700
            y: 34
            width: 246
            height: 22
            color: "#ffffff"
            text: case_wizard_obj.pbin_get_label("customer_name")
            textFormat: Text.AutoText
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pixelSize: 25
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            lineHeight: 0
            lineHeightMode: Text.ProportionalHeight
            fontSizeMode: Text.VerticalFit
            font.weight: Font.Bold
            font.family: "Poppins SemiBold"
            anchors.leftMargin: 10
        }
    }
}
