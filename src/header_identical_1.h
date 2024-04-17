#ifndef HEADER_IDENTICAL_1
#define HEADER_IDENTICAL_1
#include <QtCore>

#define MACRO_APP_NAME_FROM_INFO_PLIST                   "Forensodigital.RECON-LAB"

#define MACRO_RECON_RESULT_PREFIX                        "RECON_RESULT~~~"


#define MACRO_RECON_CASE_RUNNER_TYPE_FIRST_LAUNCHER      "MACRO_RECON_CASE_RUNNER_TYPE_FIRST_LAUNCHER"
#define MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE           "MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE"
#define MACRO_RECON_CASE_RUNNER_TYPE_ADD_SOURCE          "MACRO_RECON_CASE_RUNNER_TYPE_ADD_SOURCE"
#define MACRO_RECON_CASE_RUNNER_TYPE_POST_LAUNCH         "MACRO_RECON_CASE_RUNNER_TYPE_POST_LAUNCH"
#define MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE           "MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE"


#define MACRO_APPLE_METADATA_RUNNER_TYPE_FULL_SOURCE     1
#define MACRO_APPLE_METADATA_RUNNER_TYPE_RIGHT_CLICK     2
#define MACRO_APPLE_METADATA_RUNNER_TYPE_LEFT_CLICK      3




#define MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG     "MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG"
#define MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER    "MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER"
#define MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER    "MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER"

#define MACRO_ACTION_MENU_TREE_CASE_int                  0
#define MACRO_ACTION_MENU_TREE_KEYWORD_SEARCH_int        1
#define MACRO_ACTION_MENU_TREE_TIMELINE_SEARCH_int       1

//=========================================================//
// e.g - dont take new spliter value'#' which is contains by other spliter '(##)'
#define MACRO_RECON_Splitter_1_multiple                  ";++::++;"
#define MACRO_RECON_Splitter_2_hash_in_brace             "(##)"
#define MACRO_RECON_Splitter_3_dollar_in_brace           "[$$]"
#define MACRO_RECON_Splitter_4_forward                   ">>"
#define MACRO_RECON_Splitter_5_resolution_scope          "::"
#define MACRO_RECON_Splitter_6_comma                     ","
#define MACRO_RECON_Splitter_7_at_rate_in_brace          "[(@@)]"
#define MACRO_RECON_Splitter_8_equal_hash_equal          ":====:"

//=========================================================//

#define MACRO_TRUE_VALUE_NUMERIC_QString               "1"
#define MACRO_FALSE_VALUE_NUMERIC_QString              "0"

#define MACRO_TRUE_VALUE_READABLE_QString              "TRUE"
#define MACRO_FALSE_VALUE_READABLE_QString             "FALSE"


#define MACRO_PASS_VALUE_READABLE_QString              "PASS"
#define MACRO_FAIL_VALUE_READABLE_QString              "FAIL"

#define MACRO_PASS_VALUE_NUMERIC_int                     1
#define MACRO_FAIL_VALUE_NUMERIC_int                    -1



#define MACRO_Target_Disk_ImageType_Unsupported           "Unsupported"
#define MACRO_Target_Disk_ImageType_Apple                 "APPLE"
#define MACRO_Target_Disk_ImageType_RAW_FULL              "RAW_FULL"
#define MACRO_Target_Disk_ImageType_RAW_SPLIT             "RAW_SPLIT"
#define MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL       "ENCASE_PHYSICAL"
#define MACRO_Target_Disk_ImageType_ENCASE_LOGICAL        "ENCASE_LOGICAL"
#define MACRO_Target_Disk_ImageType_VMDK                  "VMDK"
#define MACRO_Target_Disk_ImageType_VHD                   "VHD"
#define MACRO_Target_Disk_ImageType_AFF4                  "AFF4"
#define MACRO_Target_Disk_ImageType_Optical_Disc          "Optical_Disc"



#define Macro_JobType_Completion_status_done              "1"
#define Macro_JobType_Completion_status_not_done          "0"
#define Macro_JobType_Completion_status_not_supported     "-1"

#define MACRO_JobType_Adding_Source_To_Case         "Adding source to case"
#define MACRO_JobType_Extract_Apple_Timestamps      "Extracting Apple Timestamps"
#define MACRO_JobType_Artifacts                     "Artifacts"
#define MACRO_JobType_Artifacts_Keyword_Search      "Artifacts Keyword Search"
#define MACRO_JobType_G_Report                      "Global Report"
#define MACRO_JobType_Artifacts_Timeline            "Artifacts Timeline"
#define MACRO_JobType_Plist_Viewer                  "Plist Viewer"
#define MACRO_JobType_Hex_Viewer                    "Hex Viewer"
#define MACRO_JobType_Redefined_Result              "Redefined Result"
#define MACRO_JobType_File_Search                   "File Search"
#define MACRO_JobType_Text_Indexing                 "Text Indexing"
#define MACRO_JobType_Content_Search                "Content Search"
#define MACRO_JobType_File_System_Events            "File System Events"
#define MACRO_JobType_Apple_Metadata                "Apple Metadata"
#define MACRO_JobType_Signature_Analysis            "Signature Analysis"
#define MACRO_JobType_Mime_Type                     "Mime Type"
#define MACRO_JobType_Exif_Metadata                 "Exif Metadata"
#define MACRO_JobType_Artifact_Extraction           "Extracting Artifacts"
#define MACRO_JobType_Artifact_Loading              "Loading Artifacts"

#define MACRO_JobType_Hashes                        "Hashes"
#define MACRO_JobType_Generating_Timeline           "Generating Timeline"
#define MACRO_JobType_Carved_Files                  "Carved Files"
#define MACRO_JobType_Carved_Data                   "Carved Data"
#define MACRO_JobType_Analysing_Snapshot            "Analysing Snapshot"
#define MACRO_JobType_Processing_Unallocated_Data   "Processing Unallocated Data"
#define MACRO_JobType_Verifying                     "Verifying"
#define MACRO_JobType_Exif_Metadata_Search          "Exif Metadata Search"
#define MACRO_JobType_Face_Search                    "Face Search"
#define MACRO_JobType_Face_Analysis                  "Face Analysis"
#define MACRO_JobType_Optical_Character_Recognition   "Optical Character Recognition"
#define MACRO_JobType_Skin_Tone_Detection               "Skin Tone Detection"
#define MACRO_JobType_Weapons                          "Weapons"
#define MACRO_JobType_Fire_Analysis                    "Fire Analysis"

#define MACRO_JobType_Email_Parser_Pst_Mail                 "Email - Pst"
#define MACRO_JobType_Email_Parser_Emlx_Mail                "Email - Emlx"
#define MACRO_JobType_Email_Parser_Mbox_Mail                "Email - Mbox"
#define MACRO_JobType_Unified_Logs                         "Unified Logs"

#define MACRO_Plugin_Name_Tag_Search                        "Tag Search"
#define MACRO_Plugin_Name_File_System                       "File System"
#define MACRO_Plugin_Name_Notes_Search                      "Notes Search"
#define MACRO_Plugin_Name_Hex_Viewer                        "Hex Viewer"
#define MACRO_Plugin_Name_Text_Viewer                       "Text Viewer"
#define MACRO_Plugin_Name_Strings_Viewer                    "Strings Viewer"
#define MACRO_Plugin_Name_Plist_Viewer                      "Plist Viewer"
#define MACRO_Plugin_Name_Sqlite_Viewer                     "SQLite Viewer"
#define MACRO_Plugin_Name_Registry_Viewer                   "Registry Viewer"
#define MACRO_Plugin_Name_Log_Viewer                        "Log Viewer"
#define MACRO_Plugin_Name_Unified_Logs                       "Unified Logs"

#define MACRO_Plugin_Name_Screenshots                       "Screenshots"
#define MACRO_Plugin_Name_Story_Board                       "Story Board"
#define MACRO_Plugin_Name_Redefined_Result                  "Redefined Result"
#define MACRO_Plugin_Name_Browser_History_Full_RR           "Browser History"
#define MACRO_Plugin_Name_Messenger_Full_RR                 "Messenger"
#define MACRO_Plugin_Name_Browser_History_Saved_RR          "Browser History Saved"
#define MACRO_Plugin_Name_Messenger_Saved_RR                "Messenger Saved"
#define MACRO_Plugin_Name_Artifacts_Timeline                "Artifacts Timeline"
#define MACRO_Plugin_Name_Artifacts_Timeline_Full           "Artifacts Timeline Full"
#define MACRO_Plugin_Name_Artifacts_Timeline_Saved          "Artifacts Timeline Saved"
#define MACRO_Plugin_Name_Ram_Analysis_Saved                "RAM Analysis Saved"
#define MACRO_Plugin_Name_Saved_Graphs_Messenger            "Messenger Graphs"
#define MACRO_Plugin_Name_Saved_Graphs_Browser_History      "Browser History Graphs"


//#define MACRO_Plugin_Name_Email_File                        "Email File"

#define MACRO_Plugin_Name_Email_Parser                      "Email Parser"

#define MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph    "Artifacts Timeline Graph"
#define MACRO_Plugin_Name_Saved_Maps                        "Saved Maps"
#define MACRO_Plugin_Name_Carved_Files                      "Carved Files"

#define MACRO_Plugin_Name_Carved_Password                   "Carved Password"
#define MACRO_Plugin_Name_Mobile_Backup                     "Mobile Backup"
#define MACRO_Plugin_Name_Disk_Images                       "Disk Images"
#define MACRO_Plugin_Name_RAM_Images                        "RAM Images"
#define MACRO_Plugin_Name_Carved_Data                       "Carved Data"
#define MACRO_Plugin_Name_Snapshots                         "Snapshots"
#define MACRO_Plugin_Name_Disk_Hex_Viewer                   "Disk Hex Viewer"
#define MACRO_Plugin_Name_Examiner_Space                    "Examiner Space"

#define MACRO_Plugin_Name_Email_Parser_Tag                  "Email Parser Tag"


#define MACRO_Plugin_Artifacts_Timeline_TAB_Name_Event                                     "Event"
#define MACRO_Plugin_Artifacts_Timeline_TAB_Name_Timeline                                  "Timeline"
#define MACRO_Plugin_File_System_TAB_Name_Files                                            "Files"
#define MACRO_Plugin_Screenshots_TAB_Name_Screenshots                                      "Screenshots"
#define MACRO_Plugin_Artifacts_Timeline_Saved_Graph_TAB_Name_Artifacts_Timeline_Graph      "Artifacts Timeline Graph"
#define MACRO_Plugin_Saved_Maps_TAB_Name_Saved_Maps                                        "Saved Maps"
#define MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs                      "Messenger Graphs"
#define MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs          "Browser History Graphs"
#define MACRO_Plugin_Tag_Search_TAB_Name_Plugin                                            "Plugin"
#define MACRO_Plugin_Notes_Search_TAB_Name_Plugin                                          "Plugin"
#define MACRO_Plugin_RAM_Analysis_Saved_TAB_Name_Saved_Result                              "Saved Result"

#define MACRO_Plugin_Email_Parser_TAB_Name_Email_Data                                       "Email Data"
#define MACRO_Plugin_Email_Parser_Tag_TAB_Name_Email_Parser_Tag_Data                        "Email Tag Data"

#define MACRO_Plugin_Plist_Viewer_TAB_Name_Plist_Viewer                                    "Plist Viewer"
#define MACRO_Plugin_Registry_Viewer_TAB_Name_Plist_Viewer                                 "Registry Viewer"
#define MACRO_Plugin_Carved_Files_TAB_Name_Files                                           "Files"
#define MACRO_Plugin_Carved_Passwords_TAB_Name_Passwords                                   "Passwords"
#define MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer                              "Registry Viewer"
#define MACRO_Plugin_Mobile_Backup_TAB_Name_Backup                                         "Backup"
#define MACRO_Plugin_Disk_Images_TAB_Name_Images                                           "Images"
#define MACRO_Plugin_RAM_Images_TAB_Name_Images                                            "Images"
#define MACRO_Plugin_Carved_Data_TAB_Name_Carved_Data                                      "Carved Data"
#define MACRO_Plugin_Mail_TAB_Name_Messages                                                "Messages"
#define MACRO_Plugin_Snapshots_TAB_Name_Modified_Files                                     "Modified Files"
#define MACRO_Plugin_Snapshots_TAB_Name_Deleted_Files                                      "Deleted Files"
#define MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete                                     "Disk Hex Viewer - Complete"
#define MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Unallocated                                  "Disk Hex Viewer - Unallocated"
#define MACRO_Plugin_Forensic_Image_In_Hex_Viewer_TAB_Name                                 "Forensic Image Hex Viewer"



#define MACRO_File_System_Blank_Tree_Item_Title             "FS Blank Item"

#define MACRO_Configuration_Feature_Name_RECON_Library      "RECON Library"     // in ../Resources [configuration.sqlite feature to be updated on client machine]

#define MACRO_COMBOBOX_CATGORY_All_CASE_FEATURE             "All Features"

#define MACRO_COMBOBOX_FEATURE_Case_Info                    "Case Info"

#define MACRO_MSG_Nothing                                   "Nothing"
#define MACRO_MSG_INFORMATION                               "Information"
#define MACRO_MSG_WARNING                                   "Warning"
#define MACRO_MSG_REPORT_CREATE                             "Create"
#define MACRO_MSG_CHECK_YES_NO                              "Check_Yes_No"
#define MACRO_MSG_ERROR                                     "ERROR"


#define MACRO_Launcher_Plugin_Selection_New_Case            "MACRO_Launcher_Plugin_Selection_New_Case"
#define MACRO_Launcher_Plugin_Selection_G_Report            "MACRO_Launcher_Plugin_Selection_G_Report"
#define MACRO_Launcher_Plugin_Selection_G_Timeline          "MACRO_Launcher_Plugin_Selection_G_Timeline"
#define MACRO_Launcher_Plugin_Selection_K_Search            "MACRO_Launcher_Plugin_Selection_K_Search"
#define MACRO_Launcher_Plugin_Selection_Run_Plugin          "MACRO_Launcher_Plugin_Selection_Run_Plugin"
#define MACRO_Launcher_Plugin_Selection_RECON_Configuration "MACRO_Launcher_Plugin_Selection_RECON_Configuration"
#define MACRO_Launcher_Plugin_Selection_Add_Source          "MACRO_Launcher_Plugin_Selection_Add_Source"



#define MACRO_TIMEZONE_SELECTION_RECON_CONFIGURATION        "MACRO_TIMEZONE_SELECTION_RECON_CONFIGURATION"
#define MACRO_TIMEZONE_SELECTION_CASE_LAUNCHER              "MACRO_TIMEZONE_SELECTION_CASE_LAUNCHER"

#define MACRO_HASHSET_PLUGIN_VIEWER                         "MACRO_HASHSET_PLUGIN_VIEWER"


#define MACRO_Hashset_New_Hash_Created                      "New Hashset Created"


#define MACRO_Tags_All_Tags_Title                           "All"
#define MACRO_Tag_Name_Untagged_Items                       "Untagged Items"
#define MACRO_Tag_Name_Bookmarks                            "Bookmarks"
#define MACRO_Tag_Name_Screenshots                          "Screenshots"
#define MACRO_Tag_Name_Saved_graphs                         "Saved Graphs"
#define MACRO_Tag_Name_Saved_Maps                           "Saved Maps"
#define MACRO_Tag_Name_Notes                                "Notes"
#define MACRO_Tag_Name_Skin_Tone                            "SkinTone"
#define MACRO_Tag_Name_Examiner_Comments                       "Examiner Comments"

#define MACRO_NEW_Case_Launcher                             "New Case Launcher"
#define MACRO_Multi_Source_Launcher                         "Multi Source Launcher"


#define MACRO_Apple_Attributes_Widget_Type_Timestamp_int     8
#define MACRO_Apple_Attributes_Widget_Type_REAL_VALUE_int    9
#define MACRO_Apple_Attributes_Widget_Type_SECONDS_VALUE_int 3

#define MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB                  "Browse Data"
#define MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB                  "Execute SQL"


#define MACRO_File_System_Total_Size_Processed_Byte_int      52428800        // 52428800 = 50 MB

#define MACRO_ACTIVITY_LOGS_TASK_QUEUED                      "QUEUED"
#define MACRO_ACTIVITY_LOGS_TASK_START                       "STARTED"
#define MACRO_ACTIVITY_LOGS_TASK_COMPLETED                   "COMPLETED"
#define MACRO_ACTIVITY_LOGS_TASK_ABORTED                     "ABORTED"


#define MACRO_CASE_DISPLAY_WIDGET_PLUGIN_AND_GLOBAL_ITEMS_int 0
#define MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int           1


#define MACRO_User_Selected_Timezone_UTC                      "UTC Timezone"
#define MACRO_User_Selected_Timezone_Current_Machine          "Current Machine Timezone"
#define MACRO_User_Selected_Timezone_Others                   "Others Timezone"

#define Macro_Redefined_Result_Messenger_All_Accounts         "All Accounts"
#define Macro_Redefined_Result_Messenger_All_Participants     "All Participants"

#define Macro_Redefined_Result_creating_graph_yearly          "creating graph yearly..."
#define Macro_Redefined_Result_creating_graph_monthly         "creating graph monthly..."
#define Macro_Redefined_Result_creating_graph_daywise         "creating graph daywise..."
#define Macro_Redefined_Result_creating_graph_hourly          "creating graph hourly..."

#define MACRO_Disk_Partition_Type_EFI                         "EFI"
#define MACRO_Disk_Partition_Type_Apple_APFS                  "Apple_APFS"
#define MACRO_Disk_Partition_Type_Apple_HFS                   "Apple_HFS"
#define MACRO_Disk_Partition_Type_Apple_CoreStorage           "Apple_CoreStorage"
#define MACRO_Disk_Partition_Type_Apple_Boot                  "Apple_Boot"
#define MACRO_Disk_Partition_Type_APFS_Volume                 "APFS Volume"
#define MACRO_Disk_Partition_Type_Microsoft_Basic_Data        "Microsoft Basic Data"


#define MACRO_FILE_SYSTEM_TYPE_FAT                             "fat"
#define MACRO_FILE_SYSTEM_TYPE_HFS                             "hfs"
#define MACRO_FILE_SYSTEM_TYPE_APFS                            "apfs"
#define MACRO_FILE_SYSTEM_TYPE_NTFS                            "ntfs"
#define MACRO_FILE_SYSTEM_TYPE_MSDOS                           "msdos"
#define MACRO_FILE_SYSTEM_TYPE_EXFAT                           "exfat"       // Extended File Allocation Table
#define MACRO_FILE_SYSTEM_TYPE_UDF                             "udf"         // Universal Disk Format, filesystem used by DVD-ROM (including DVD-video and DVD-audio) discs, and by many CD-R/RW packet-writing programs
#define MACRO_FILE_SYSTEM_TYPE_CD9660                          "cd9660"     // ISO9660 is a file system for optical disc media.
#define MACRO_FILE_SYSTEM_TYPE_CDDAFS                          "cddafs"     // A file system used to mount audio CDs and present audio tracks on disc to users as AIFF-C encoded files.
#define MACRO_FILE_SYSTEM_TYPE_LIFS                            "lifs"     // A new file system used from Macos 12 for windows.

#define MACRO_SOURCE_ROLE_TYPE_SYSTEM                         "System"
#define MACRO_SOURCE_ROLE_TYPE_DATA                           "Data"
#define MACRO_SOURCE_ROLE_TYPE_PREBOOT                        "Preboot"
#define MACRO_SOURCE_ROLE_TYPE_VM                             "VM"
#define MACRO_SOURCE_ROLE_TYPE_RECOVERY                       "Recovery"
#define MACRO_SOURCE_ROLE_TYPE_UPDATE                         "Update"

#define MACRO_APFS_Snapshot_Plist_Key_FS_Run_Status            "FS Run Status"
#define MACRO_APFS_Snapshot_Plist_Key_Source_Count_Name        "Source Count Name"
#define MACRO_APFS_Snapshot_Plist_Key_Snapshot_Name            "Snapshot Name"

#define MACRO_LOG_macOS_SOURCE_DIR_PATH_Diagnostics            "/private/var/db/diagnostics/"
#define MACRO_LOG_macOS_SOURCE_DIR_PATH_UUID                   "/private/var/db/uuidtext/"

#define MACRO_Spotlight_Source_Store_V2_Dir_Path_1               ".Spotlight-V100/Store-V2/"
#define MACRO_Spotlight_Source_Store_V2_Dir_Path_2               "_Spotlight-V100/Store-V2/"
#define MACRO_Spotlight_Source_Store_V2_Dir_Path_3               "._Spotlight-V100/Store-V2/"
#define MACRO_Spotlight_Source_Store_V2_Dir_Path_4               ".Spotlight-V100-back/Store-V2/"

#define MACRO_Spotlight_Extracted_Text_File_Name               "spotlight-store_data.txt"
#define MACRO_Spotlight_Extracted_CSV_File_Name                "spotlight-store_fullpaths.csv"


#define MACRO_APPLE_METADATA_Finalize_Message                   "Finalizing Apple Metadata Please Wait..."

#define MACRO_Export_Case_Parent                                 "Export Case"
#define MACRO_Export_Case_Quick_Mode                             "Quick Mode"
#define MACRO_Export_Case_Expert_Mode                            "Expert Mode"

#define MACRO_Export_Case_Category_Internet_History              "Internet History"
#define MACRO_Export_Case_Category_Messages                      "Messages"
#define MACRO_Export_Case_Category_Emails                        "Emails"
#define MACRO_Export_Case_Category_Image                        "Image"
#define MACRO_Export_Case_Category_Multimedia                    "Multimedia"
#define MACRO_Export_Case_Category_Document                     "Document"
#define MACRO_Export_Case_Category_Tags                          "Tags"
#define MACRO_Export_Case_Category_Other_Artifacts               "Other Artifacts"

#define MACRO_Result_Preferences_Key_Highlight_User_Opened_Files    "Highlight User Opened Files"
#define MACRO_Result_Preferences_Key_Minimum_File_Size_Skin_Tone_Detection_in_KB                "Minimum File Size For Skin Tone Detection in KB"
#define MACRO_Result_Preferences_Key_Minimum_File_Size_Face_Analysis_in_KB                      "Minimum File Size For Face Analysis in KB"
#define MACRO_Result_Preferences_Key_Minimum_File_Size_Optical_Character_Recognition_in_KB      "Minimum File Size For Optical Character Recognition in KB"
#define MACRO_Result_Preferences_Key_Minimum_File_Size_Weapons_in_KB                            "Minimum File Size For Weapons in KB"
#define MACRO_Result_Preferences_Key_Minimum_File_Size_Fire_Analysis_in_KB                      "Minimum File Size For Fire Analysis in KB"
#define MACRO_Result_Preferences_Key_Maximum_File_Size_Text_View_in_MB                          "Maximum File Size For Text View in MB"
#define MACRO_App_Preferences_Key_Enable_Debug_Mode                 "Enable Debug Mode"


#define MACRO_System_Account_List_Prefix_SYSTEM_USER                 "SYSTEM_USER="
#define MACRO_System_Account_List_Prefix_DOMAIN_USER                 "DOMAIN_USER="

#define MACRO_Mount_Path_Time_Machine_Backup_Image_Main              "Image_Main"
#define MACRO_Mount_Path_Time_Machine_Backup_Image_Snapshots         "Image_Snapshots"

#endif // HEADER_IDENTICAL

