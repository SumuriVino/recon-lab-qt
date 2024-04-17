#include "narad_muni.h"


narad_muni::narad_muni(QWidget *parent)
{
    clear_fields();
}

narad_muni::~narad_muni()
{

}

void narad_muni::set_field(QString key, QVariant value)
{
    key_value_map.insert(key, value);
}

QVariant narad_muni::get_field(QString key)
{
    return key_value_map.value(key);
}

void narad_muni::clear_fields()
{
    QList<QVariant> qlist_variant;

    set_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString, QString());
    set_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString, QString());
    set_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString, QString());
    set_field(MACRO_NARAD_User_Selected_Language_int, 0);


    set_field(MACRO_NARAD_Output_Directory_QString, QString());
    set_field(MACRO_NARAD_RECON_Result_Dir_QString, QString());

    set_field(MACRO_NARAD_Adhoc_Path_Location_Parent_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Hex_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Plist_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Database_Registry_Viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Log_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Database_Log_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Optical_Character_Recognition_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Dir_Export_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Apple_Metadata_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Preview_Links_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Data_In_Result_QString,QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Files_In_Result_QString,QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Miscellenous_In_Result_QString,QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString,QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Recognize_Faces_In_Result_QString,QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Skin_Tone_Detection_In_Result_QString,QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Weapons_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Fire_Analysis_In_Result_QString, QString());
    set_field(MACRO_NARAD_Adhoc_Path_Location_Email_Parser_In_Result_QString,QString());

    set_field(MACRO_NARAD_Feature_Path_Location_Parent_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Activity_Log_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Hashset_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Screenshots_Images_Directory_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Google_Mail_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_Images_Directory_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_Images_Directory_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Index_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Snapshots_File_System_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Disk_Hex_View_In_Result_QString, QString());
    set_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString, QString());


    set_field(MACRO_NARAD_Exported_File_Location_Parent_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_File_System_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_Files_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_Hex_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_Plugin_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_SQLite_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_RAM_Analysis_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_Graphs_Browser_History_In_Result_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_Graphs_Messenger_In_Result_QString, QString());
    set_field(MACRO_NARAD_Exported_File_Location_Graphs_Artifact_Timeline_In_Result_QString, QString());



    set_field(MACRO_NARAD_Exported_Content_Location_Parent_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString, QString());
    set_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString, QString());

    set_field(MACRO_NARAD_Extracted_Content_Location_Parent_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Parent_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString, QString());
    set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString, QString());



    set_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Chat_View_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Artifacts_Timeline_Full_In_Result_QString, QString());
    set_field(MACRO_NARAD_Conf_Path_Location_Caches_In_Result_QString, QString());




    set_field(MACRO_NARAD_Report_Location_Parent_In_Result_QString, QString());
    set_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString, QString());
    set_field(MACRO_NARAD_Report_Location_Plugin_In_Result_QString, QString());
    set_field(MACRO_NARAD_Report_Location_SQLite_Viewer_In_Result_QString, QString());
    set_field(MACRO_NARAD_Report_Location_Timeline_In_Result_QString, QString());




    set_field(MACRO_NARAD_Plugin_Result_Dir_QString, QString());


    set_field(MACRO_NARAD_Root_Evidence_Number_QString, QString());
    set_field(MACRO_NARAD_Root_Description_QString, QString());
    set_field(MACRO_NARAD_Evidence_Type_QString, QString());
    set_field(MACRO_NARAD_Root_Count_Name_QString, QString());
    set_field(MACRO_NARAD_Root_Type_QString, QString());
    set_field(MACRO_NARAD_Root_Source_Type_Display_QString, QString());
    set_field(MACRO_NARAD_Root_Source_Type_Internal_QString, QString());
    set_field(MACRO_NARAD_Root_Name_QString, QString());
    set_field(MACRO_NARAD_Root_Path_1_QString, QString());
    set_field(MACRO_NARAD_Root_Path_2_for_fusion_QString, QString());
    set_field(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString, QString());
    set_field(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString, QString());

    set_field(MACRO_NARAD_Root_Hash_MD5_QString, QString());
    set_field(MACRO_NARAD_Root_Hash_SHA1_QString, QString());
    set_field(MACRO_NARAD_Root_Source_Count_Name_All_QStringList, QStringList());
    set_field(MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList, QStringList());
    set_field(MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList, QStringList());
    set_field(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString, QString());
    set_field(MACRO_NARAD_Root_Disk_Node_1_QString,QString());
    set_field(MACRO_NARAD_Root_Disk_Identifier_1_QString, QString());
    set_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString,QString());
    set_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString, QString());

    set_field(MACRO_NARAD_Source_RUNTIME_Root_Recovery_Passcode_Plain_QString, QString());
    set_field(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString, QString());

    set_field(MACRO_NARAD_Source_OS_Scheme_Display_QString, QString());
    set_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString, QString());
    set_field(MACRO_NARAD_Source_Type_Display_QString, QString());
    set_field(MACRO_NARAD_Source_Type_Internal_QString, QString());
    set_field(MACRO_NARAD_Source_Path_1_QString, QString());
    set_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString, QString());
    set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString, QString());
    set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString, QString());
    set_field(MACRO_NARAD_Source_File_System_Type_QString, QString());
    set_field(MACRO_NARAD_Source_Volume_Type_QString, QString());
    set_field(MACRO_NARAD_Source_Volume_Offset_QString, QString());
    set_field(MACRO_NARAD_Source_Volume_UUID_QString, QString());
    set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString, QString());
    set_field(MACRO_NARAD_Source_Disk_Identifier_QString, QString());
    set_field(MACRO_NARAD_Source_Nested_bool, false);
    set_field(MACRO_NARAD_Source_Hierarchy_QString, QString());
    set_field(MACRO_NARAD_Source_Decompressed_bool, false);
    set_field(MACRO_NARAD_Source_Main_Name_QString, QString());
    set_field(MACRO_NARAD_Source_Name_QString, QString());
    set_field(MACRO_NARAD_Source_Main_Path_QString, QString());
    set_field(MACRO_NARAD_Source_Virtual_Source_Path_QString, QString());
    set_field(MACRO_NARAD_Source_System_Account_QList, qlist_variant);
    set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString, QString());
    set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64, 0);
    set_field(MACRO_NARAD_Source_Product_Type_QString, QString());
    set_field(MACRO_NARAD_Source_OS_Version_Extended_QString, QString());
    set_field(MACRO_NARAD_Source_OS_Version_Full_QString, QString());
    set_field(MACRO_NARAD_Source_Country_QString, QString());
    set_field(MACRO_NARAD_Source_City_QString, QString());
    set_field(MACRO_NARAD_Source_Latitude_QString, QString());
    set_field(MACRO_NARAD_Source_Longitude_QString, QString());
    set_field(MACRO_NARAD_Source_Configured_ID_QStringList,QStringList());
    set_field(MACRO_NARAD_Source_Device_Name_QString, QString());
    set_field(MACRO_NARAD_Source_Id_No_QString, QString());
    set_field(MACRO_NARAD_Source_GUID_QString, QString());
    set_field(MACRO_NARAD_Source_Target_Type_QString, QString());
    set_field(MACRO_NARAD_Source_Build_Version_QString, QString());
    set_field(MACRO_NARAD_Source_iTune_Version_QString, QString());
    set_field(MACRO_NARAD_Source_Imei_QStringList, QStringList());
    set_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList, QStringList());
    set_field(MACRO_NARAD_Source_Serial_No_QString, QString());
    set_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString, QString());
    set_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString, QString());
    set_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64, 0);
    set_field(MACRO_NARAD_Source_Product_Name_QString, QString());
    set_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList, QStringList());
    set_field(MACRO_NARAD_Source_Display_Name_QString, QString());
    set_field(MACRO_NARAD_Source_Count_Name_QString, QString());
    set_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList,QStringList());
    set_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool, false);
    set_field(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64, 0);
    set_field(MACRO_NARAD_Source_Product_Model_QString, QString());
    set_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString, QString());
    set_field(MACRO_NARAD_Source_Wifi_Address_QString, QString());
    set_field(MACRO_NARAD_Source_Locale_Language_QString, QString());
    set_field(MACRO_NARAD_Source_Role_Type_QString, QString());

    set_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString, QString());
    set_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString, QString());
    set_field(MACRO_NARAD_Mount_Point_APFS_Snapshot_Dir_QString, QString());
    set_field(MACRO_NARAD_Mount_Point_Dummy_Root_QString, QString());
    set_field(MACRO_NARAD_Mount_Point_Shadow_File_QString, QString());

    set_field(MACRO_NARAD_VAR_TMP_Point_Full_Disk_Access_QString, QString());

    set_field(MACRO_NARAD_Selected_Plugins_QStringList,QStringList());
    set_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList,QStringList());
    set_field(MACRO_NARAD_Selected_Plugins_With_Tab_After_Extraction_QStringList,QStringList());


    set_field(MACRO_NARAD_Case_ID_QString, QString());
    set_field(MACRO_NARAD_Case_Name_QString, QString());
    set_field(MACRO_NARAD_Case_Notes_QString, QString());
    set_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64, 0);
    set_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64, 0);
    set_field(MACRO_NARAD_Case_Created_Timezone_qint64, 0);
    set_field(MACRO_NARAD_Case_Created_Timezone_QString, QString());
    set_field(MACRO_NARAD_Case_Agency_Name_QString, QString());
    set_field(MACRO_NARAD_Case_Agency_Address_QString, QString());
    set_field(MACRO_NARAD_Case_Agency_Logo_QString, QString());
    set_field(MACRO_NARAD_Case_Location_QString, QString());
    set_field(MACRO_NARAD_Current_Machine_Timezone_QString, QString());

    set_field(MACRO_NARAD_Result_App_Version_QString, QString());
    set_field(MACRO_NARAD_Result_Build_Version_QString, QString());
    set_field(MACRO_NARAD_Result_Updated_For_Versions_QString, QString());

    set_field(MACRO_NARAD_Examiner_Name_QString, QString());
    set_field(MACRO_NARAD_Examiner_Phone_QString, QString());
    set_field(MACRO_NARAD_Examiner_Email_QString, QString());
    set_field(MACRO_NARAD_Examiner_Selected_Date_Format_int, 0);
    set_field(MACRO_NARAD_Examiner_Selected_Date_Format_QString, QString());
    set_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString, QString());
    set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString, QString());
    set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64, 0);

    set_field(MACRO_NARAD_Apple_Attributes_All_Key_QStringList,QStringList());
    set_field(MACRO_NARAD_Apple_Attributes_All_Custom_Column_Name_QStringList,QStringList());
    set_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Key_QStringList,QStringList());
    set_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Custom_Column_Name_QStringList,QStringList());
    set_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList,QStringList());
    set_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList,QStringList());

    set_field(MACRO_NARAD_Exif_Attributes_All_Key_QStringList,QStringList());
    set_field(MACRO_NARAD_Exif_Attributes_All_Custom_Column_Name_QStringList,QStringList());
    set_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Key_QStringList,QStringList());
    set_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Custom_Column_Name_QStringList,QStringList());
    set_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList,QStringList());
    set_field(MACRO_NARAD_Exif_Attributes_For_Report_Custom_Column_Name_QStringList,QStringList());


    set_field(MACRO_NARAD_Debug_File_Path_QString, QString());




    ///------html module report


    set_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Checkbox_Export_bool, false);
    set_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Destination_Db_File_List_QString, QString());
    set_field(MACRO_NARAD_Report_Plugin_Name_QString, QString());
    set_field(MACRO_NARAD_Report_Table_Html_Report_Available_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Table_Name_Textedit_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Report_Available_Textedit_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Tab_Name_Textedit_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Table_Name_Stack_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Report_Available_Stack_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Tab_Name_Stack_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Parent_Module_Name_QString, QString());
    set_field(MACRO_NARAD_Report_Table_Artifact_Common_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Table_Report_Html_Header_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Table_Report_Header_Column_Width_Html_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Data_Type_Tablewidget_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Html_Col_Name_Tablewidget_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Artifact_Common_Textedit_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Header_Textedit_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Header_Column_Width_Html_Textedit_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Col_Name_Textedit_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_Html_Datatype_Textedit_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_Artifact_Common_Stack_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Header_Stack_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Header_Column_Width_Html_Stack_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_Html_Col_Name_Stack_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Html_Datatype_Stack_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Export_Path_Tablewidget_QList, qlist_variant);
    set_field(MACRO_NARAD_Reports_Dir_Path_QString, QString());
    set_field(MACRO_NARAD_Report_Tabwidget_CurrentIndex_int, 0);
    set_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Html_Display_Name_Tablewidget_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Html_Order_Tablewidget_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Destination_File_Location_QString, QString());
    set_field(MACRO_NARAD_Report_Path_QString, QString());
    set_field(MACRO_NARAD_Report_Table_CSV_Report_Available_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_CSV_Data_Type_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_CSV_Col_Name_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_CSV_Display_Name_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_CSV_Order_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_Table_XML_Report_Available_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_XML_Col_Name_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_XML_Data_Type_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_XML_Display_Name_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_XML_Order_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_Table_KML_Report_Available_Tablewidget_QStringList,QStringList());
    set_field(MACRO_NARAD_Report_KML_Col_Name_QList, qlist_variant);
    set_field(MACRO_NARAD_Report_KML_Data_Type_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_KML_Display_Name_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_KML_Order_QList, qlist_variant) ;
    set_field(MACRO_NARAD_Report_Global_Adv_Html_Records_Per_Page_int, 0);


    set_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList,QStringList());
    set_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList,QStringList());
    set_field(MACRO_NARAD_Export_KML_Filedetail_QStringList,QStringList());


    set_field(MACRO_NARAD_Global_Report_Selected_Plugins_With_Tabs_AND_Export_Status_QStringList,QStringList());
    set_field(MACRO_NARAD_Global_Report_Selected_Tabs_With_Export_Status_QStringList,QStringList());
    set_field(MACRO_NARAD_Global_Report_Advance_Html_QString, QString());
    set_field(MACRO_NARAD_Global_Report_Standard_Html_QString, QString());
    set_field(MACRO_NARAD_Global_Report_CSV_QString, QString());
    set_field(MACRO_NARAD_Global_Report_PDF_QString, QString());
    set_field(MACRO_NARAD_Global_Report_XML_QString, QString());
    set_field(MACRO_NARAD_Global_Report_Scope_Full_QString, QString());
    set_field(MACRO_NARAD_Global_Report_Scope_Tags_QString, QString());
    set_field(MACRO_NARAD_Global_Report_Selected_Tags_QStringList,QStringList());
    set_field(MACRO_NARAD_Global_Report_Name_QString, QString());
    set_field(MACRO_NARAD_Global_Report_Is_Export_Plugins_for_Tag_Checked_QString, QString());
    set_field(MACRO_NARAD_Global_Report_Censor_Skin_Tone_Image_Checked_QString, QString());


    /// ---  Keyword Search
    set_field(MACRO_NARAD_Keyword_Search_Selected_Plugins_And_Tabs_QStringList,QStringList());

    set_field(MACRO_NARAD_Apple_Metadata_Check_Status_bool, false);
    set_field(MACRO_NARAD_Face_Analysis_Check_Status_bool, false);
    set_field(MACRO_NARAD_Optical_Character_Recognition_Check_Status_bool, false);
    set_field(MACRO_NARAD_Skin_Tone_Detection_Check_Status_bool, false);
    set_field(MACRO_NARAD_Weapons_Check_Status_bool, false);
    set_field(MACRO_NARAD_Fire_Analysis_Check_Status_bool, false);
    set_field(MACRO_NARAD_Signature_Check_Status_bool, false);
    set_field(MACRO_NARAD_Hashset_Check_Status_bool, false);
    set_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool, false);
    set_field(MACRO_NARAD_Unified_Logs_Check_Status_bool, false);
    set_field(MACRO_NARAD_Index_Files_Use_Filter_bool, false);
    set_field(MACRO_NARAD_Index_Files_Less_Than_Size_InBytes_qint64, 0);
    set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, false);
    set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, false);
    set_field(MACRO_NARAD_Index_Files_All_Files_Check_Status_bool, false);


    set_field(MACRO_NARAD_Text_View_Maximum_File_Size_Bytes_qint64, 0);

    set_field(MACRO_NARAD_Preferences_User_Opened_Files_bool, false);
    set_field(MACRO_NARAD_Prefrences_Skin_Tone_Detection_Minimum_File_Size_Bytes_qint64, 0);
    set_field(MACRO_NARAD_Prefrences_Face_Analysis_Minimum_File_Size_Bytes_qint64, 0);
    set_field(MACRO_NARAD_Prefrences_Optical_Character_Recognition_Minimum_File_Size_Bytes_qint64, 0);
    set_field(MACRO_NARAD_Prefrences_Text_View_Maximum_File_Size_Bytes_qint64, 0);

}



