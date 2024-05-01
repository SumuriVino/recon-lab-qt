import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Basic 6.0
import QtQuick.Dialogs 6.0
/*This class has the front-end ui of selecting the Recon Mac sharing mode folder as a source by clicking on browse button and when we click on add button
it will send that path to c++ backend work for taking that path as a source path for further adding process.
*/
Rectangle
{
    id: rectangle_network_acq_folder_path
    x: 0
    y: 1
    width: 950
    height: 550
    anchors.verticalCenterOffset: 0
    anchors.horizontalCenterOffset: 0

    Image
    {
        id: image_rectangle_network_acq_folder_path
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Label
        {
            id: label_network_acq_folder
            x: 26
            y: 196
            width: 73
            height: 16
            color: "#ffc600"
            text: source_selector_obj.pbin_get_label("Source_Path")
            font.pixelSize: 13
        }

        TextField
        {
            id: textfield_network_acq_folder_filepath
            x: 157
            y: 192
            width: 688
            height: 24
            placeholderText: source_selector_obj.pbin_get_label("Select_Folder")
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
            id: button_browse_network_acq_folder
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
                let image_path = source_selector_obj.pbin_button_source_path_selection_clicked(source_selector_obj.pbin_get_label("RootType_RECONMACSharingMode"),source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Folder_Internal"))
                if(image_path !== "")
                {
                    textfield_network_acq_folder_filepath.clear()
                    textfield_network_acq_folder_filepath.insert(0,image_path)
                }
            }
            background: Rectangle
            {
                id:rectangle_browse_network_acq_folder
                radius:20
            }
        }
        Button
        {
            id: button_add_network_acq_folder
            x: 427
            y: 233
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
            enabled: textfield_network_acq_folder_filepath.text ? true:false
            onClicked:
            {
                source_selector_obj.pbin_button_add_selected_sources(source_selector_obj.pbin_get_label("EvidenceType_Network"),source_selector_obj.pbin_get_label("RootType_RECONMACSharingMode"),source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Folder_Display"),source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Folder_Internal"),textfield_network_acq_folder_filepath.text,"","","")
                setVisible(false)
            }
            background: Rectangle
            {
                id:rectangle_add_network_acq_folder
                radius:20
            }
        }

        Button
        {
            id: button_back_network_acq_folder
            x: 14
            y: 16
            width: 45
            height: 45
            background: Rectangle
            {
                id:rectangle_back_network_acq_folder
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
                stackview_source.push("qrc:/Recon_mac_sharing_mode_selector.qml")
            }
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
              + source_selector_obj.pbin_get_label("EvidenceType_Network")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_RECONMACSharingMode")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("SourceType_RECONMACSharingMode_Folder_Display")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"
    }
}
