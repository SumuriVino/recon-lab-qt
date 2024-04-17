#ifndef HEADER_CSV_TEMPLATE
#define HEADER_CSV_TEMPLATE


#define MACRO_CSV_TEMPLATE_CSV_FILE_SEPRATOR ","
#define MACRO_CSV_TEMPLATE_CSV_CUSTOM_SEPRATOR "::"

#define MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET "0"
#define MACRO_CSV_TEMPLATE_TAB_TYPE_TEXTEDIT "1"
#define MACRO_CSV_TEMPLATE_TAB_TYPE_STACK "2"
#define MACRO_CSV_TEMPLATE_TAB_TYPE_TREE "3"     //  [1 - for parent,  2 - for child]
#define MACRO_CSV_TEMPLATE_TAB_TYPE_THUMBNAIL "4"
#define MACRO_CSV_TEMPLATE_TAB_TYPE_CHAT_VIEWER "5"

#define MACRO_CSV_TEMPLATE_DATATYPE_OTHER_QString                                  "0"
#define MACRO_CSV_TEMPLATE_DATATYPE_OTHER_INT                                       0
#define MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString                               "1"
#define MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_INT                                    1
#define MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString                                 "2"
#define MACRO_CSV_TEMPLATE_DATATYPE_RECORD_INT                                      2
#define MACRO_CSV_TEMPLATE_DATATYPE_TEXT_QString                                   "3"
#define MACRO_CSV_TEMPLATE_DATATYPE_TEXT_INT                                        3
#define MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString                                   "4"
#define MACRO_CSV_TEMPLATE_DATATYPE_TIME_INT                                        4
#define MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString                            "5"
#define MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_INT                                 5
#define MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString                                   "6"
#define MACRO_CSV_TEMPLATE_DATATYPE_SIZE_INT                                        6
#define MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString                                  "7"
#define MACRO_CSV_TEMPLATE_DATATYPE_NOTES_INT                                       7
#define MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString                        "8"
#define MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_INT                             8
#define MACRO_CSV_TEMPLATE_DATATYPE_SYSTEM_ACCOUNT_QString                         "9"
#define MACRO_CSV_TEMPLATE_DATATYPE_SYSTEM_ACCOUNT_INT                              9
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_FILEPATH_macOS                        "10"
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_FILEPATH_macOS_INT                     10
#define MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES                                 "11"
#define MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES_INT                              11
#define MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString                         "12"
#define MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_INT                              12
#define MACRO_CSV_TEMPLATE_DATATYPE_FILE_EXISTENCE_QString                         "13"
#define MACRO_CSV_TEMPLATE_DATATYPE_FILE_EXISTENCE_INT                              13
#define MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString              "14" // contacts images
#define MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_INT                   14 // contacts images
#define MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_QString                          "15" // keychain password
#define MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_INT                               15 // keychain password
#define MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString                       "16" // metadata in virtual box, multiple harddisk info ;++::++;
#define MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_INT                            16 // metadata in virtual box, multiple harddisk info ;++::++;
#define MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString                    "17"  //[iOS] ;
#define MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_INT                         17 //[iOS] ;
#define MACRO_CSV_TEMPLATE_DATATYPE_PREPEND_COUNT_QString                          "18"  // [iOS] dynamically want to prepend zero count
#define MACRO_CSV_TEMPLATE_DATATYPE_PREPEND_COUNT_INT                               18 // [iOS] dynamically want to prepend zero count
#define MACRO_CSV_TEMPLATE_DATATYPE_METADATA_PREVIEW_QString                       "19"
#define MACRO_CSV_TEMPLATE_DATATYPE_METADATA_PREVIEW_INT                            19
#define MACRO_CSV_TEMPLATE_DATATYPE_TEXTEDIT_QString                               "20"
#define MACRO_CSV_TEMPLATE_DATATYPE_TEXTEDIT_INT                                    20
#define MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_PATH_QString                    "21"
#define MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_PATH_INT                         21
#define MACRO_CSV_TEMPLATE_DATATYPE_LATITUDE_QString                               "22"
#define MACRO_CSV_TEMPLATE_DATATYPE_LATITUDE_INT                                    22
#define MACRO_CSV_TEMPLATE_DATATYPE_LONGITUDE_QString                              "23"
#define MACRO_CSV_TEMPLATE_DATATYPE_LONGITUDE_INT                                   23
#define MACRO_CSV_TEMPLATE_DATATYPE_ALTITUDE_QString                               "24"
#define MACRO_CSV_TEMPLATE_DATATYPE_ALTITUDE_INT                                    24
#define MACRO_CSV_TEMPLATE_DATATYPE_IMAGE_NAME_KML_QString                         "25"
#define MACRO_CSV_TEMPLATE_DATATYPE_IMAGE_NAME_KML_INT                              25
#define MACRO_CSV_TEMPLATE_DATATYPE_STACK_QString                                  "26"
#define MACRO_CSV_TEMPLATE_DATATYPE_STACK_INT                                       26
#define MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString                                    "27"
#define MACRO_CSV_TEMPLATE_DATATYPE_TAG_INT                                         27
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_SENDING_STATUS             "28"
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_SENDING_STATUS_INT          28
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_SENDER                     "29"
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_SENDER_INT                  29
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_RECEIVER                   "30"
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_RECEIVER_INT                30
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_BODY                       "31"
//#define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_CHAT_VIEWER_MESSAGE_BODY_INT                    31
#define MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACTS_TIMELINE_TIMESTAMP_TYPE_QString       "32"
#define MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACTS_TIMELINE_TIMESTAMP_TYPE_INT            32
// #define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_WINDOW_PATH                           "33"  // Not in use
// #define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_WINDOW_PATH_INT                        33  // Not in use
#define MACRO_CSV_TEMPLATE_DATATYPE_RECON_VIEWERS_TAG_EXPORTED_DATA_QString         "34"
#define MACRO_CSV_TEMPLATE_DATATYPE_RECON_VIEWERS_TAG_EXPORTED_DATA_INT              34
#define MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString                  "35"
#define MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_INT                       35
// #define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_OS_SCHEME_FLAG                        "36" // Not in use
// #define MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_OS_SCHEME_FLAG_INT                     36 // Not in use
#define MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_QString                              "37"
#define MACRO_CSV_TEMPLATE_DATATYPE_NOTES_ICON_INT                                   37
#define MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString                             "38"
#define MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_INT                                  38
#define MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_QString                          "39"
#define MACRO_CSV_TEMPLATE_DATATYPE_LAST_BLANK_COL_INT                               39
#define MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_QString                               "40"
#define MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_INT                                    40
#define MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_NAME_QString                     "41"
#define MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_NAME_INT                          41
#define MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString                      "42"
#define MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_INT                           42
#define MACRO_CSV_TEMPLATE_DATATYPE_DURATION_QString                                "43"
#define MACRO_CSV_TEMPLATE_DATATYPE_DURATION_INT                                     43



#endif // HEADER_CSV_TEMPLATE

