import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Basic 6.0
import QtQuick.Dialogs 6.0
/*This class has the front-end ui of selecting the forensics image, fusion drive image and forensics file vault images of Mac operating system as a source by clicking on browse button
 and when we click on add button it will send that path to c++ backend work for taking that path as a source path for further adding process.*/
Rectangle
{
    id: rectangle_physical_forensic_image_path
    x: 0
    y: 1
    width: 950
    height: 550
    anchors.verticalCenterOffset: 0
    anchors.horizontalCenterOffset: 0

    Image
    {
        id: image_rectangle_physical_forensic_image_path
        x: 0
        y: 0
        width: 950; height: 550
        source: "file:/" + source_selector_obj.pbin_get_icons_path("source_splash_image")


        Label
        {
            id: label_physical_forensic_image
            x: 26
            y: 194
            width: 73
            height: 16
            color: "#ffc600"
            text: source_selector_obj.pbin_get_label("Image_Path")
            font.pixelSize: 13
        }

        CheckBox
        {
            id: checkbox_physical_filevault
            x: 20
            y: 234
            width: 28
            height: 20
            indicator.width: 20
            indicator.height: 20
            Text
            {
                x: 35
                y: 2
                text: source_selector_obj.pbin_get_label("Is_FileVaulted")
                color: "#ffc600"
            }

            onClicked:
            {
                if(checkbox_physical_filevault.checkable)
                {
                    textfield_physical_filevault_password.clear()
                    textfield_physical_fusion.clear()
                    textfield_physical_filevault_password.enabled = false
                    button_filevault_password_show_hide.enabled = false
                }

                if(checkbox_physical_filevault.checked)
                {

                    if(textfield_physical_forensic_filepath.text !== "")
                    {
                        if(!source_selector_obj.pbin_is_image_supported(source_selector_obj.pbin_get_label("RootType_FileVaultImage"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Internal"),textfield_physical_forensic_filepath.text))
                        {
                            source_selector_obj.pbin_set_messagebox_for_unsupported_image(source_selector_obj.pbin_get_label("RootType_FileVaultImage"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Internal"))
                        }
                    }

                    checkbox_physical_fusion.checked = false
                    textfield_physical_fusion.enabled = false
                    textfield_physical_filevault_password.enabled = true
                    button_filevault_password_show_hide.enabled = true
                    button_browse_physical_fusion_image.enabled = false
                }
            }
        }

        TextField
        {
            id: textfield_physical_forensic_filepath
            x: 156
            y: 192
            width: 688
            height: 24
            placeholderText: source_selector_obj.pbin_get_label("Select_Forensic_Image")
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

        TextField
        {
            id: textfield_physical_filevault_password
            x: 156
            y: 232
            width: 688
            height: 24
            placeholderText: source_selector_obj.pbin_get_label("Type_Password")
            placeholderTextColor: "grey"
            echoMode: TextInput.Password
            enabled: checkbox_physical_filevault.checked ? true: false

            background: Rectangle
            {
                border.color: "white"
            }
            //ktktkt
            color: "black"
            font.pixelSize: 12
            //ktktkt

        }

        Button
        {
            id: button_browse_physical_forensic_image
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
                if(checkbox_physical_filevault.checked)
                {
                    let filevault_image_path = source_selector_obj.pbin_button_source_path_selection_clicked(source_selector_obj.pbin_get_label("RootType_FileVaultImage"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Internal"))
                    if(filevault_image_path !== "")
                    {
                        textfield_physical_forensic_filepath.clear()
                        textfield_physical_forensic_filepath.insert(0,filevault_image_path)
                    }
                }
                else if(checkbox_physical_fusion.checked)
                {
                    let fusion_image_path = source_selector_obj.pbin_button_source_path_selection_clicked(source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"))
                    if(fusion_image_path !== "")
                    {
                        textfield_physical_forensic_filepath.clear()
                        textfield_physical_forensic_filepath.insert(0,fusion_image_path)
                    }
                }
                else
                {
                    let forensics_image_path = source_selector_obj.pbin_button_source_path_selection_clicked(source_selector_obj.pbin_get_label("RootType_ForensicsImage"),source_selector_obj.pbin_get_label("SourceType_ForensicsImage_Internal"))
                    if(forensics_image_path !== "")
                    {
                        textfield_physical_forensic_filepath.clear()
                        textfield_physical_forensic_filepath.insert(0,forensics_image_path)
                    }
                }
            }
            background: Rectangle
            {
                id:rectangle_browse_physical_forensic_image
                radius:20
            }
        }
        Button
        {
            id: button_add_physical_forensic_image
            x: 427
            y: 312
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
            enabled: textfield_physical_forensic_filepath.text ? true:false
            onClicked:
            {
                if(checkbox_physical_filevault.checked)
                {
                    if(!source_selector_obj.pbin_is_image_supported(source_selector_obj.pbin_get_label("RootType_FileVaultImage"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Internal"),textfield_physical_forensic_filepath.text))
                    {
                        source_selector_obj.pbin_set_messagebox_for_unsupported_image(source_selector_obj.pbin_get_label("RootType_FileVaultImage"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Internal"))
                        setVisible(true)
                    }
                    else
                    {
                        source_selector_obj.pbin_button_add_selected_sources(source_selector_obj.pbin_get_label("EvidenceType_Physical"),source_selector_obj.pbin_get_label("RootType_FileVaultImage"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Display"),source_selector_obj.pbin_get_label("SourceType_FileVaultImage_Internal"),textfield_physical_forensic_filepath.text,"",textfield_physical_filevault_password.text,"")
                        setVisible(false)
                    }
                }
                else if(checkbox_physical_fusion.checked)
                {
                    if(!source_selector_obj.pbin_is_image_supported(source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"),textfield_physical_forensic_filepath.text))
                    {
                        source_selector_obj.pbin_set_messagebox_for_unsupported_image(source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"))
                        setVisible(true)
                    }
                    else
                    {
                        source_selector_obj.pbin_button_add_selected_sources(source_selector_obj.pbin_get_label("EvidenceType_Physical"),source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Display"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"),textfield_physical_forensic_filepath.text,textfield_physical_fusion.text,"","")
                        setVisible(false)
                    }
                }
                else
                {
                    source_selector_obj.pbin_button_add_selected_sources(source_selector_obj.pbin_get_label("EvidenceType_Physical"),source_selector_obj.pbin_get_label("RootType_ForensicsImage"),source_selector_obj.pbin_get_label("SourceType_ForensicsImage_Display"),source_selector_obj.pbin_get_label("SourceType_ForensicsImage_Internal"),textfield_physical_forensic_filepath.text,"","","")
                    setVisible(false)
                }
            }
            background: Rectangle
            {
                id:rectangle_add_physical_forensic_image
                radius:20
            }
        }

        Button
        {
            id: button_back_physical_forensic_img
            x: 14
            y: 16
            width: 45
            height: 45
            background: Rectangle
            {
                id:rectangle_back_physical_forensic_img
                radius:20
                color: "#000000"
                Image
                {
                    id: image_back
                    x: 0
                    y: 0
                    width: 45
                    height: 45
                    source: "file:/" + source_selector_obj.pbin_get_icons_path("back")

                }
            }
            onClicked:
            {
                stackview_source.push("qrc:/Forensic_vault_fusion_image_os_selector.qml")
            }
        }

        ToolButton
        {
            id: button_filevault_password_show_hide
            x: 867
            y: 232
            width: 58
            height: 24
            opacity: 1

            enabled: checkbox_physical_filevault.checked ? true: false

            onClicked:
            {

                if(textfield_physical_filevault_password.echoMode == TextInput.Password)
                {
                    textfield_physical_filevault_password.echoMode = TextInput.Normal

                    image_filevault_eye_close.visible = true
                    image_filevault_eye_open.visible = false
                }

                else
                {
                    textfield_physical_filevault_password.echoMode = TextInput.Password

                    image_filevault_eye_open.visible = true
                    image_filevault_eye_close.visible = false
                }
            }

            background: Rectangle
            {
                id:rectangle_password_show_hide
                radius:20
            }
        }

        CheckBox
        {
            id: checkbox_physical_fusion
            x: 20
            y: 274
            width: 28
            height: 20
            Text
            {
                x: 34
                y: 2
                text: source_selector_obj.pbin_get_label("Is_Fusion")
                color: "#ffc600"
            }

            indicator.width: 20
            indicator.height: 20
            onClicked:
            {
                if(checkbox_physical_fusion.checkable)
                {
                    textfield_physical_filevault_password.clear()
                    textfield_physical_fusion.clear()
                    textfield_physical_filevault_password.enabled = true
                    button_filevault_password_show_hide.enabled = true

                }

                if(checkbox_physical_fusion.checked)
                {
                    button_filevault_password_show_hide.enabled = false
                    textfield_physical_filevault_password.enabled = false
                    checkbox_physical_filevault.checked = false
                    button_browse_physical_fusion_image.enabled = true
                    textfield_physical_fusion.enabled = true
                    if(textfield_physical_forensic_filepath.length !== 0)
                    {
                        if(!source_selector_obj.pbin_is_image_supported(source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"),textfield_physical_forensic_filepath.text))
                        {
                            source_selector_obj.pbin_set_messagebox_for_unsupported_image(source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"))
                        }
                    }
                }
                else
                {
                    button_browse_physical_fusion_image.enabled = false
                }
            }
        }

        TextField
        {
            id: textfield_physical_fusion
            x: 156
            y: 272
            width: 688
            height: 24
            placeholderText: source_selector_obj.pbin_get_label("Select_Fusion_Platter_Image")
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
            id: button_browse_physical_fusion_image
            x: 867
            y: 272
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
            enabled: false
            background: Rectangle
            {
                id:rectangle_browse_physical_fusion_image
                radius:20
            }
            onClicked:
            {
                let fusion_image_path = source_selector_obj.pbin_button_source_path_selection_clicked(source_selector_obj.pbin_get_label("RootType_FusionImage"),source_selector_obj.pbin_get_label("SourceType_FusionImage_Internal"))
                if(fusion_image_path !== "")
                {
                    textfield_physical_fusion.clear()
                    textfield_physical_fusion.insert(0,fusion_image_path)
                }
            }

        }
    }

    Image
    {
        id: image_filevault_eye_open
        x: 867
        y: 233
        width: 58
        height: 27
        source: "file:/" + source_selector_obj.pbin_get_icons_path("eye_open")
        fillMode: Image.PreserveAspectFit
    }

    Image
    {
        id: image_filevault_eye_close
        x: 878
        y: 233
        width: 35
        height: 27
        opacity: 1
        source: "file:/" + source_selector_obj.pbin_get_icons_path("eye_close")
        fillMode: Image.PreserveAspectFit
        visible: false
    }

    Label
    {
        id: label_source_path
        x: 8
        y: 527
        width: 303
        height: 16
        text: source_selector_obj.pbin_get_label("[")
              + source_selector_obj.pbin_get_label("EvidenceType_Physical")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("RootType_ForensicsImage")
              + source_selector_obj.pbin_get_label(">")
              + source_selector_obj.pbin_get_label("OS_SCHEME_macOS_Display")
              + source_selector_obj.pbin_get_label("]")
        font.pointSize: 15
        color: "#ffc600"

    }
}
