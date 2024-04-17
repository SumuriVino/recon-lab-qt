#ifndef HEADER_GLOBAL_ENUM
#define HEADER_GLOBAL_ENUM


enum enum_global_preview_stack_widget{
    enum_global_preview_stack_widget_BLANK = 0,
    enum_global_preview_stack_widget_IMAGE,
    enum_global_preview_stack_widget_Audio_Video
};

enum enum_global_plist_file_key_type
{
    enum_Global_Plist_File_Key_Type_Boolean = 0,
    enum_Global_Plist_File_Key_Type_Data,
    enum_Global_Plist_File_Key_Type_Date,
    enum_Global_Plist_File_Key_Type_String,
    enum_Global_Plist_File_Key_Type_Number,
    enum_Global_Plist_File_Key_Type_Array
};

enum enum_stack_widget_file_search_filters {
    enum_global_stack_widget_file_search_regular = 0,
    enum_global_stack_widget_file_search_apple_metadata
};


enum enum_global_case_tree_display_row{

    enum_global_case_tree_display_row_SOURCES = 0,

    enum_global_case_tree_display_row_MOBILE_BACKUP,
    enum_global_case_tree_display_row_DISK_IMAGES,
    enum_global_case_tree_display_row_RAM_IMAGES,
    enum_global_case_tree_display_row_APPLE_METADATA,
    enum_global_case_tree_display_row_EXIF_METADATA,
    enum_global_case_tree_display_row_MIME_TYPES,
    enum_global_case_tree_display_row_FACE_ANALYSIS,
    enum_global_case_tree_display_row_FACE_SEARCH,
    enum_global_case_tree_display_row_OPTICAL_CHARACTER_RECOGNITION,
    enum_global_case_tree_display_row_SKIN_TONE_DETECTION,
    enum_global_case_tree_display_row_WEAPONS,
    enum_global_case_tree_display_row_FIRE_ANALYSIS,
    enum_global_case_tree_display_row_EXTENSIONS,
    enum_global_case_tree_display_row_FILE_SIGNATURE,
    enum_global_case_tree_display_row_FILE_SIZE,
    enum_global_case_tree_display_row_UNIFIED_LOGS,
    enum_global_case_tree_display_row_SNAPSHOTS,
    enum_global_case_tree_display_row_HASHSET,
    enum_global_case_tree_display_row_GRIFFEYE_DATA,
    enum_global_case_tree_display_row_EMAIL_FILES,
    enum_global_case_tree_display_row_ARTIFACTS,
    enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH,
    enum_global_case_tree_display_row_FILE_SEARCH,
    enum_global_case_tree_display_row_EXIF_METADATA_SEARCH,
    enum_global_case_tree_display_row_CONTENT_SEARCH,
    enum_global_case_tree_display_row_ARTIFACTS_TIMELINE,
    enum_global_case_tree_display_row_BUCKET,
    enum_global_case_tree_display_row_TAGS,
    enum_global_case_tree_display_row_NOTES,
    enum_global_case_tree_display_row_REDEFINED_RESULT,
    enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED,
    enum_global_case_tree_display_row_SCREENSHOTS,
    enum_global_case_tree_display_row_STORY_BOARD,
    enum_global_case_tree_display_row_CARVED_PASSWORD,
    enum_global_case_tree_display_row_CARVED_FILES,
    enum_global_case_tree_display_row_CARVED_DATA,
    enum_global_case_tree_display_row_RAM_ANALYSIS_SAVED,
    enum_global_case_tree_display_row_SAVED_GRAPH_RESULT,
    enum_global_case_tree_display_row_SAVED_MAPS

};


enum enum_global_case_tree_display_column
{
    enum_global_case_tree_display_column_ZERO = 0,
    enum_global_case_tree_display_column_FIRST,
    enum_global_case_tree_display_column_SECOND,
    enum_global_case_tree_display_column_THIRD,
    enum_global_case_tree_display_column_RECORD_COUNT,
    enum_global_case_tree_display_column_ROOT_COUNT_NAME
};


enum enum_global_exif_data_interface_exif_data{
    enum_global_exif_data_interface_exif_data_KEY = 0,
    enum_global_exif_data_interface_exif_data_Value,
    enum_global_exif_data_interface_exif_data_Plugin_Table_Name,
    enum_global_exif_data_interface_exif_data_Plugin_Name,
    enum_global_exif_data_interface_exif_data_Plugin_Record_no,
    enum_global_exif_data_interface_exif_data_Plugin_Source_count_name,
    enum_global_exif_data_interface_exif_data_Plugin_Db_Path
};


enum enum_global_language_translater{
    enum_global_language_translater_original = 0,
    enum_global_language_translater_english,
    enum_global_language_translater_chinese
};

enum enum_global_ERRORS
{
    enum_global_ERROR_1001_Mount_Point_Creation_Failed = 1001,
    enum_global_ERROR_1002_Mounting_All_Partitions_Failed,
    enum_global_ERROR_1003_Open_Vault_Image_Failed,
    enum_global_ERROR_1004_Fuse_Already_Mounted,
    enum_global_ERROR_1005_Fuse_Executable_Not_Exist,
    enum_global_ERROR_1006_Hdiutil_Output_Desired_Pattern_Not_Found,
    enum_global_ERROR_1007_Vault_UID_Not_Found,
    enum_global_ERROR_1008_Encryption_Script_Creation_Failed1,
    enum_global_ERROR_1009_Apfs_Script_Creation_Failed,
    enum_global_ERROR_1010_Apfs_Disk_Not_Found,
    enum_global_ERROR_1011_Mount_Point_Creation_Encase_Logical_Failed,
    enum_global_ERROR_1012_Image_Path1_Not_Exist,
    enum_global_ERROR_1013_Image_Path2_Not_Exist,
    enum_global_ERROR_1014_Encryption_Script_Creation_Failed2,
    enum_global_ERROR_1015_Corestorage_Script_Creation_Failed,
    enum_global_ERROR_1016_Corestorage_Volume_Not_Attached1,
    enum_global_ERROR_1017_Corestorage_Volume_Not_Attached2,
    enum_global_ERROR_1018_Core_Storage_Disk_Not_Found,
    enum_global_ERROR_1019_Extract_ADB_Android_AB_Failed,
    enum_global_ERROR_1020_Extract_Cellebrite_iOS_UFDR_Failed,
    enum_global_ERROR_1021_Extract_Cellebrite_iOS_TAR_Failed,
    enum_global_ERROR_1022_Extract_GrayKey_iOS_Backup_Failed,
    enum_global_ERROR_1023_Extract_GrayKey_Android_Backup_Failed

};


enum enum_global_append_data_in_file
{
    enum_global_append_data_in_file_as_it_is = 0,
    enum_global_append_data_in_file_with_prepend_one_next_line,
    enum_global_append_data_in_file_with_append_one_next_line
};

enum enum_global_rsync_args_type
{
    enum_global_rsync_args_type_general = 0,
    enum_global_rsync_args_type_links_and_recursive
};

enum enum_unified_logs
{
    enum_unified_logs_seq_no = 0,
    enum_unified_logs_tags,
    enum_unified_logs_timestamp,
    enum_unified_logs_process,
    enum_unified_logs_event_type,
    enum_unified_logs_event_message,
    enum_unified_logs_category
};

#endif // HEADER_GLOBAL_ENUM

