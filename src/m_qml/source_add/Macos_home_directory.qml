import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Basic 6.0
import QtQuick.Dialogs 6.0

Rectangle
{
    id: rectangle_logical_home_dir_path
    x: 0
    y: 1
    width: 950
    height: 550
    anchors.verticalCenterOffset: 0
    anchors.horizontalCenterOffset: 0

    Image
    {
        id: image_rectangle_logical_home_dir_path
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Label
        {
            id: label_logical_home_dir
            x: 30
            y: 196
            width: 73
            height: 16
            color: "#ffc600"
            text: source_selector_obj.pbin_get_label("Source_Path")
            font.pixelSize: 13
        }

        TextField
        {
            id: textfield_logical_home_dir_filepath
            x: 157
            y: 192
            width: 688
            height: 24
            placeholderText: source_selector_obj.pbin_get_label("Select_Macos_Home_Directory")
            placeholderTextColor: "grey"
            readOnly: true
            background: Rectangle
            {
                border.color:"white"
            }
            //ktktkt
            color: "black"
            font.pixelSize: 12
            //ktktkt
        }

        Button
        {
            id: button_browse_logical_home_dir
            x: 867
            y: 192
            width: 58
            height: 24
            //ktktkt
//            text: source_selector_obj.pbin_get_label("...")
//            font.bold: true
//            font.pointSize: 20
            Text{
                anchors.fill: parent
                horizontalAlignment:   Text.AlignHCenter
                text: source_selector_obj.pbin_get_label("...")
                font.bold: true
                font.pointSize: 20
                color: "black"

            }
            //ktktkt
            onClicked:
            {
                let image_path = source_selector_obj.pbin_button_source_path_selection_clicked(source_selector_obj.pbin_get_label("RootType_macOSHomeDirectory"),source_selector_obj.pbin_get_label("SourceType_macOSHomeDirectory_Internal"))
                if(image_path !== "")
                {
                    textfield_logical_home_dir_filepath.clear()
                    textfield_logical_home_dir_filepath.insert(0,image_path)
                }
            }
            background: Rectangle
            {
                id:rectangle_browse_logical_home_dir
                radius:20
            }
        }
        Button
        {
            id: button_add_logical_home_dir
            x: 427
            y: 279
            width: 148
            height: 24
            //ktktkt
//            text: source_selector_obj.pbin_get_label("Add")
            Text{
                anchors.fill: parent
                horizontalAlignment:   Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                text: source_selector_obj.pbin_get_label("Add")
                color: "black"

            }
            //ktktkt            checkable: false
            checked: false
            enabled: textfield_logical_home_dir_filepath.text && textfield_macos_home_dir_username.text ? true:false
            onClicked:
            {
                source_selector_obj.pbin_button_add_selected_sources(source_selector_obj.pbin_get_label("EvidenceType_Logical"),source_selector_obj.pbin_get_label("RootType_macOSHomeDirectory"),source_selector_obj.pbin_get_label("SourceType_macOSHomeDirectory_Display"),source_selector_obj.pbin_get_label("SourceType_macOSHomeDirectory_Internal"),textfield_logical_home_dir_filepath.text,"","",textfield_macos_home_dir_username.text)
                setVisible(false)
            }
            background: Rectangle
            {
                id:rectangle_add_logical_home_dir
                radius:20
            }
        }

        Button
        {
            id: button_back_logical_home_dir
            x: 14
            y: 16
            width: 45
            height: 45
            background: Rectangle
            {
                id:rectangle_back_logical_home_dir
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

        TextField
        {
            id: textfield_macos_home_dir_username
            x: 157
            y: 237
            width: 688
            height: 24
            placeholderText: source_selector_obj.pbin_get_label("Type_Username")
            placeholderTextColor: "grey"
            background: Rectangle
            {
                border.color:"white"
            }
            //ktktkt
            color: "black"
            font.pixelSize: 12
            //ktktkt
        }

        Label
        {
            id: label_macos_home_dir_username
            x: 30
            y: 237
            width: 73
            height: 24
            text: source_selector_obj.pbin_get_label("Username")
            color: "#ffc600"

        }

    }

    Label
    {
        id: label_source_path
        x: 8
        y: 527
        width: 303
        height: 16
        text: source_selector_obj.pbin_get_label("[")
              + source_selector_obj.pbin_get_label("EvidenceType_Logical")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_macOSHomeDirectory")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"
    }
}
