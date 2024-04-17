#ifndef HEADER_GLOBAL_STRUCTURES
#define HEADER_GLOBAL_STRUCTURES

#include <QtCore>
#include "ui_custom_classes/m_tablewidget.h"
#include <QSqlDatabase>
#include "rcbuild/build_items_recon.h"
#include "narad_muni/narad_muni.h"



struct struct_global_desktop_info
{
    //------------------------------------------------//

    bool bool_desktop_pwd_accurate;
    QString desktop_password_original;
    QString desktop_password_modified_general;
    QString desktop_password_modified_shell_script;
    QString desktop_password_modified_symbol_hex_value;

    //------------------------------------------------//

    bool bool_processor_chip_silicon;

    QString os_version_full;
    QString os_version_extended;

    QString model_name_info;
    QString processor_brand_info;
    QString ram_size_info;
    QString os_version_info;
    QString model_year_info;
    QString power_settings_info;
    QString file_vault_status_info;
    QString security_controller_info; // Apple T2 Controller


    QString hardware_overview;
    QString software_overview;
    QString csrutil_status_info;

    QString label_name;
    QString volume_name;
    QString device_disk_node;
    QString file_system_type;

    QString osxfuse_info;
    QString java_info;
    QString python_info;
    QString lz4_info;
    QString enum34_info;
    QString nudenet_info;
    QString filetype_info;
    QString mime_info;
    QString mimelib_info;
    QString glob2_info;
    QString moviepy_info;
    QString ffmpeg_info;
    QString opencv_python_info;
    QString pillow_info;
    QString xcode_info;
    QString full_disk_acces_info;

    bool bool_consistency_requirements_satisfied;

    QStringList consistency_requirements_failure_items_QStringList;

    int block_size_numeric_int;                        // 4096
    qint64 available_space_bytes_numeric_qint64;       // 23338754048
    qint64 total_space_bytes_numeric_qint64;           // 195000508416

    QString timezone_name_complete_QString;     // Asia/Kolkata-IST-GMT+5:30
    QString timezone_name_QString;              // Asia/Kolkata
    qint64 timezone_offset_numeric_qint64;      // 19800   OR  -32400
    QString timezone_offset_readable_QString;   // +5:30


};


struct struct_global_destination_drive_info
{

    QString label_name;
    QString volume_name;
    QString device_disk_node;
    QString drive_path;
    QString file_system_type;

    int block_size_numeric_int;
    qint64 available_size_byte_at_case_uptime_numeric_qint64;
    qint64 total_size_byte_numeric_qint64;

    bool bool_read_only;
    bool bool_ready_to_work;
    bool bool_system_root_volume;
    bool bool_valid_drive;

};


struct struct_global_file_for_zip_export
{
    QString source_path;
    QString destination_path;
};

struct struct_global_recon_file_info
{
    QString display_file_path;
    QString recon_file_info_path;
    QString recon_file_from;
    QString id_path;
    QString complete_file_path;
    bool bool_file_from_result = false;
    bool bool_file_is_dir = false;
    QString db_path;
    QString db_table_name;
    QString source_count_name;

};

struct struct_global_set_metadata_info
{
    QString db_file_path;
    QString detailed_info;
    QString source_count_name;
    QString final_file_path;
    QString record_no;
    QString display_searched_text;
    QString content_searched_text;
    QString plugin_name;
    QString tab_name;
    QString quicklook_file_path;

    bool bool_file_from_result;

};


struct struct_global_tagging_plist_viewer_record
{
    QString INT_record;
    QString plugin_name;
    QString tab_name;
    QString plist_table_name;
    QString source_count_name;


    QString source_file;
    QString file_size;

    QString bookmark_status;
    QString tag_status;
    QString notes_text;

    QString item0;///plist file name
    QString item1;///plist file path
    QString item2;///key
    QString item3;///type
    QString item4;///key value
    QString item5;///display tab name

    QString keypath;///keypath

    QString color_hex;///hex color
    QString recon_file_from;
    QString recon_file_infopath;


};

struct struct_global_tagging_saved_graph_record
{
    QString INT_record;
    QString plugin_name;
    QString tab_name;

    QString item0;
    QString item1;
    QString item2;
    QString item3;
    QString item4;
    QString item5;

};

struct struct_global_tagging_artifact_timeline_record
{
    QString plugin_record_no;
    QString plugin_name; // item0
    QString tab_name; // item1
    QString source_count_name;

    QString item2; // item 2
    QString item3; // item3
    QString item4; // Timestamp
    QString item5; // Timestamp Type
    QString item6; // Timestamp Type full
};

struct struct_global_tagging_redefined_result_record
{
    QString INT_record;
    QString redefined_plugin_name; // feature name
    QString redefined_tab_name; // Messenger or browser in Tab Name and category

    QString plugin_name; // item0
    QString tab_name; // item1
    QString source_count_name;

    QString item2; //
    QString item3; //
    QString item4; //
    QString item5; // Timestamp

};

struct struct_global_tagging_carved_passwords
{
    QString INT_record;
    QString plugin_name; // Carved Password
    QString tab_name; // Passwords

    QString source_count_name;

    QString item0; //password
    QString item1; //account
    QString item2; //server
    QString item3; //description
    QString item4; //tab label name

};

struct struct_global_tagging_carved_files
{
    QString INT_record;
    QString plugin_name; // Carved Files
    QString tab_name; // Files

    QString source_count_name;

    QString item0;
    QString item1;
    QString item2;
    QString item3; //result tab name

};

struct struct_global_selected_plugins_info {
    QString plugin_name;
    QStringList selected_plugin_tab_names;
    bool extraction_completed;
    QStringList plugin_selected_table_list;
    QStringList total_table_list;
    QStringList supported_os_schemes_list;
};

struct struct_global_tagging_string_viewer_record
{
    QString INT_record;
    QString plugin_name;
    QString tab_name;
    QString source_count_name;

    QString item0;//string data
    QString item1;//actual file path
    QString item2;//file name
    QString item3;//content_file_name
    QString item4;//file size
    QString hex_color;//file size
    QString export_file_path;//export file
    QString recon_filefrom;
    QString recon_file_infopath;

};

struct struct_global_tagging_log_viewer_record
{
    QString INT_record;
    QString plugin_name;
    QString tab_name;
    QString log_table_name;
    QString source_count_name;

    QString source_file;
    QString file_size;

    QString tag_status;
    QString notes_text;

    QString log_file_name;
    QString source_file_path;
    QString tagged_data_str;
    QString item3;
    QString file_size_str;
    QString viewer_display_name;

    qint64 cursor_start_index;
    qint64 cursor_end_index;

    QString colour_value;
    QString recon_file_from;
    QString recon_file_infopath;

};

struct struct_global_tagging_registry_viewer
{
    QString INT_record;
    QString plugin_name; // Registry Viewer
    QString tab_name; // Registry Viewer

    QString source_count_name;

    QString item0;///file path
    QString item1;///node path
    QString item2;///node name
    QString item3;///key
    QString item4;///type
    QString item5;///value
    QString item6;///node timestamp
    QString item7;
    QString item8;
    QString item9;
    QString item10;

    QString item11; //result tab name

};

struct struct_global_tagging_unified_logs_record
{
    QString INT_record;
    QString plugin_name;
    QString tab_name;
    QString log_table_name;
    QString source_count_name;
    QString category;
    QString timestamp;
    QString process;
    QString event_type;
    QString event_message;
    QString viewer_display_name;
    QString colour_value;

};

struct struct_global_tagging_carved_data
{
    QString INT_record;
    QString plugin_name; // Carved Data
    QString tab_name; // Carved Data
    QString category_name; // dynamic
    QString table_name; // dynamic

    QString source_count_name;
    QString root_count_name;

    QString bookmark_status;
    QString tag_str;
    QString notes_str;
    QString tagging_on_record;


    QString item0;
    QString item1;
    QString item2;
    QString item3;
    QString item4;
    QString item5;
    QString item6;
    QString item7;
    QString item8;
    QString item9;
    QString item10;


    QString item11;
    QString item12;


    QString source_file_name;
    QString source_file_path;
    QString source_plugin_name;
    QString source_tab_name;
    QString source_record_no;

    QString recon_file_from;
    QString recon_file_infopath;

};

struct struct_global_tagging_sqlite_viewer
{
    QString INT_record;
    QString plugin_name; // Sqlite Viewer
    QString tab_name; // Browse Data OR Execute SQL

    QString source_count_name;
    QString notes_text;

    QString item0;
    QString item1;
    QString item2;
    QString item3;
    QString item4;
    QString item5;
    QString item6;
    QString item7;
    QString item8;
    QString item9;
    QString item10;

    QString item11; //result tab name

};

struct struct_global_gallery_view_file_info
{
    int table_view_row_no;
    QString bookmark;
    QString INT;
    QString source_count_name;
    QString file_path;
    QString thumbnail_file_path;
    QString full_filepath;
    QString file_name;
    QString latitude;
    QString longitude;
    QString last_access_timestamp;
    qint64  index;

};


struct struct_global_redefined_result_selected_plugin_and_tab_info
{
    QString plugin_name;
    QString tab_name;
    QString plugin_INT;
    QStringList selected_columns_of_tab_list;
    QStringList given_columns_name_list;
};

struct struct_global_file_system_post_launcher_job_info
{
    QStringList source_list_apple_metadata;
    QStringList source_list_exif_metadata;
    QStringList source_list_mime_type;
    QStringList source_list_signature_analysis;
    QStringList source_list_hashes;
    QStringList source_list_text_indexing;
    QStringList source_list_face_analysis;
    QStringList source_list_optical_character_recognition;
    QStringList source_list_skin_tone_detection;
    QStringList source_list_weapons;
    QStringList source_list_fire_analysis;
    QStringList source_list_unified_logs;

};


struct struct_global_macOS_global_preferences
{
    QString model;
    QString latitude;
    QString longitude;
    QString country;
    QString city_name;
};



struct struct_global_task_type_and_title
{
    QString type;
    QString title;
    QString source_count_name;

};


struct struct_global_hex_viewer_start_and_end_index
{
    QString current_block_name;
    uint64_t start_index;
    uint64_t end_index;
    QColor block_highlighting_color;

    uint64_t block_actual_start_offset_in_file;
    uint64_t block_actual_end_offset_in_file;
};

struct struct_global_sqlite_viewer_single_dbtable_info
{

    qint64 record_count;
    int cols_count;

    QStringList cols_list;
    QStringList h_header_list;
    QStringList v_header_list;
    QStringList view_list;
    QStringList search_list;
    QStringList list_convert_coloum_data_type;
    QStringList list_db_coloum_datatype;

    QStringList list_CustomCombobox_CurrentText;

};

struct struct_global_sqlite_viewer_tablewidget_main{

    QString table_name;
    struct_global_sqlite_viewer_single_dbtable_info obj_struct_dbtable_detail;
    m_tablewidget *tableWidget_MainDataDisplay_ptr;
    QTableWidget *tableWidget_FilterViewSearch_ptr;
    QTableWidget *tableWidget_columns_selection_ptr;

};

struct struct_global_sqlite_viewer_plugin_info
{
    QStringList selectedTableList;
    QString customPath;
};

struct struct_global_fetch_metadata
{
    QString metadata;
    QString file_path;
    QString preview_file_path;
    bool bool_virtual_export;
};

struct struct_GLOBAL_snapshots_info
{
    QString snapshot_actual_name;
    QString snapshot_mount_path;
    QString fs_run_status;
    QString source_count_name;
};

struct struct_GLOBAL_witness_info_root
{
    QString evidence_type;                       // Physical Evidence,Network Acquisition

    QString evidence_number;                     // Evidence Number entered by Examiner
    QString description;                         // Description entered by Examiner


    QString root_count_name;                     // Root1 or  Root2 [count of actually selected media by examiner]
    QString root_type;                           // Forensics Image OR 'ADB Android Backup'
    QString source_type_display;                 // Forensics Image OR DMG
    QString source_type_internal;                // Forensics_Image OR RECON_Logical_Image_DMG
    QString root_name;                           // Jermyn_small_Image.dmg
    QString root_path_1;                         // /Volumes/Jermyn_small_Image.dmg
    QString root_path_2_for_fusion;              // /Volumes/platter.dmg
    QString root_disk_identifier_1;              // disk8
    QString root_disk_node_1;                    // /dev/disk8
    QString root_disk_identifier_2_for_fusion;   // disk9
    QString root_disk_node_2_for_fusion;         // /dev/disk9


    QString root_recovery_passcode_plain;          // abcdefgh123 - for encrypted source(such as File Vault)
    QString root_recovery_passcode_encrypted;      // @%^&%$*^$(&%^

    QString root_evidence_password_plain;        // mac-boy123
    QString root_evidence_password_encrypted;    // $$@%^&%$*^$(&%^%

    QString root_hash_md5;                       // be41c03f98c1a9bc699a5584f9d5de4f
    QString root_hash_sha1;                      // 44ebedb7afdd7952ae76bc03bfbbbc639cfa5c48
    QString home_directory_user_name;            // vasudev [In launcher if user dir is selected Home Directory - given by examiner]

    QStringList source_count_name_all_QStringList;    // All Source count name list(prime and nested) in one Root like Source1,Source2,Source3,Source4
    QStringList source_count_name_prime_QStringList;  // These are actually came with Root   Source1,Source2 (Root1>>Source1, Root1>>Source2)
    QStringList source_count_name_nested_QStringList; // These are extarcted from other sources like Source3,Source4 (Source1>>Source3 , Source2>>Source4)

};

struct struct_global_system_account_info
{
    QString system_user;
    QString domain_user;

};


struct struct_GLOBAL_witness_info_source
{
    QString evidence_type;                              // Physical Evidence,Network Acquisition

    QString evidence_number;                            // Evidence Number entered by Examiner
    QString description;                                // Description entered by Examiner

    QString root_count_name;                            // Root1 or  Root2 [count of actually selected media by examiner]
    QString root_type;                                  // Mounted Volumes or Forensics Image
    QString root_name;                                  // Jermyn_small_Image.dmg
    QString root_path_1;                                // Examiner selected path - /Volumes/Jermyn_small_Image.dmg
    QString root_path_2_for_fusion;                     // Examiner selected path - /Volumes/platter.dmg
    QString root_disk_identifier;                       // disk7
    QString root_recovery_passcode_plain;               // abcdefgh123 - for encrypted source(such as File Vault)

    QString source_count_name;                          // Source1 or Source2

    //---------------------------------------------------------------------------------------------------//
    QString source_path_1;
    // /Volumes/Jermyn_small_Image.dmg
    // OR /Users/rahul/Backup/00008020-001E10EC3C84003A (nested)
    // OR /Lab_Features/File_System/Source20/Decompressed_Files/_ios14.zip/ios14/73008020-001E10EC3C64008C (decompressed nested)
    //---------------------------------------------------------------------------------------------------//

    QString source_path_2_for_fusion;                   // /Volumes/platter.dmg

    //---------------------------------------------------------------------------------------------------//
    QString source_main_path;
    // /Volumes/Jermyn_small_Image.dmg
    // OR /Users/rahul/Backup/00008020-001E10EC3C84003A (nested)
    // OR /ios14.zip/ios14/73008020-001E10EC3C64008C (decompressed nested)
    //---------------------------------------------------------------------------------------------------//

    //---------------------------------------------------------------------------------------------------//
    QString virtual_source_path;
    // /private/var/tmp/RECON_MNT_disk8s1_Jermyn_small_Image.dmg/
    // OR /private/var/tmp/RECON_MNT_disk9s1_Books_BIg_sur.dmg/Users/rahul/Backup/00008020-001E10EC3C84003A/ (nested)
    // OR RECON_RESULT_DIR/Lab_Features/File_System/Source20/Decompressed_Files/_ios14.zip/ios14/73008020-001E10EC3C64008C/ (decompressed nested)
    //---------------------------------------------------------------------------------------------------//

    //---------------------------------------------------------------------------------------------------//
    QString fs_extraction_source_path;
    // /private/var/tmp/RECON_MNT_disk8s1_Jermyn_small_Image.dmg/
    // OR /private/var/tmp/RECON_MNT_disk9s1_Books_BIg_sur.dmg/Users/rahul/Backup/00008020-001E10EC3C84003A/ (nested)
    // OR RECON_RESULT_DIR/Lab_Features/File_System/Source20/Decompressed_Files/_ios14.zip/ios14/73008020-001E10EC3C64008C/ (decompressed nested)
    //---------------------------------------------------------------------------------------------------//

    QString fuse_mount_path_tsk;                        // /private/var/tmp/tsk_fuse_mnt/Source1
    QString fuse_mount_path_raw;                        // /Users/<USERNAME>/RECON_TMP/tmp_sumuri_recon_img_mount/fuse_path/

    QString fuse_mount_path_zip;

    bool bool_recon_logical_image;                      // true or false
    bool bool_source_nested;                            // true or false
    bool bool_source_decompressed;                      // true or false
    QString source_hierarchy;                           // Root1>>Source1>>Source4


    QString os_scheme_display;                          // macOS OR iOS
    QString os_scheme_internal;                         // macOS OR iOS_UID

    QStringList system_user_QStringList;                      // drishti,amy
    QList<struct_global_system_account_info> system_account_QList;  // SYSTEM_USER='tdoris4987';DOMAIN_USER='(Admin) E. Tim'

    //---------------------------------------------------------------------------------------------------//
    QString source_type_display;
    // Forensics Image OR DMG
    // OR iTunes iOS Backup (nested or decompressed) - This value can be different from Root's source_type_display(Forensic Image)
    //---------------------------------------------------------------------------------------------------//

    //---------------------------------------------------------------------------------------------------//
    QString source_type_internal;
    // Forensics_Image OR RECON_Logical_Image_DMG
    // OR iTunes_iOS_Backup (nested or decompressed) - This value can be different from Root's source_type_internal(Forensic_Image)
    //---------------------------------------------------------------------------------------------------//


    QString source_main_name;                           // Jermyn_small_Image.dmg OR 00008101-001875081AF1003A_files_full-iphone.zip
    QString source_name;                            	// Jermyn_01 OR iPhone
    QString complete_source_name ;                  	// /Jermyn_small_Image.dmg/Jermyn_01 OR /00008101-001875081AF1003A_files_full-iphone.zip

    QString source_evidence_password_plain;             // mac boy123 - also used in Right Click Add source(disk images,  mobile backups etc)
    QString source_evidence_password_encrypted;         // $$@%^&%$*^$(&%^%
    QString source_evidence_password_modified_symbol_hex;             // mac%20boy123

    QString source_disk_identifier;                     // disk7s1

    QString product_name;                               // iPhone 7 Plus OR iPhone XR
    QString product_model;                              // iPhone 7 Plus (A1784) OR Version 14.4.2 (Build 18D70)
    QString product_type;                               // Macmini6,1  or  iPhone6,2

    QString os_version_extended;                        // 10.13.6  or  11.0
    QString os_version_full;                        	// 10.13  or  11.0

    QString build_version;                          	// 17A365   or  15A402
    QString latitude;                               	// 29.6902
    QString longitude;                              	// 77.0051
    QString country;                                	// India  or  IN
    QString city;                               		// Karnal  or  Mumbai

    QStringList configured_ids_QStringList;			    // paladinedge@gmail.com
    QString display_name;                               // Thomas iPhone
    QString device_name;                    			// Thomas iPhone

    QString target_type;                                // Device

    //    QString id_no;                                  	// 64f1899db1dc45affa697c8f1dd00dec6f5ba229
    QString device_id;                                  	// 64f1899db1dc45affa697c8f1dd00dec6f5ba229

    QString guid;                                   	// 86210554B24824AA2F5DF2B7E68418EE
    QString itune_version;                          	// 12.6.2.20
    QString serial_no;                              	// DX3SCSR8FRC5

    QStringList phone_no_msisdn_QStringList;			// +1 (302) 310-0296 , +1 (302) 310-548
    QStringList imei_no_QStringList;                    // 355674072921599
    QStringList sim_no_iccid_QStringList;		        // 89918580400011589865

    QString latest_backup_date_readable_QString;		// 2017/10/06 04:44:35
    qint64  latest_backup_date_numeric_qint64;          // 1507265075
    QString backup_size_byte_numeric_QString;	       	// 734885807
    QString timezone_readable_QString;                  // Asia/Kolkata-IST-GMT+5:30
    qint64  timezone_numeric_qint64;                    // 19800

    QString bluetooth_device_address;                   // 18:65:90:02:9b:be
    QString wifi_address;                               // 18:65:90:02:9b:bd
    QString locale_language;                            // en_US

    QString parent_source_count_name;               	// Source1
    QString image_volume_type;                          // 41504653-0000-11AA-AA11-00306543ECAC or Apple_HFS
    QString image_volume_offset;                       	// 105906176 Bytes
    QString image_volume_uuid;                          // 2E651EC6-993A-4E70-9087-52185BC0A486

    QString total_size_byte_numeric_QString;			// 119824367616
    QString free_space_byte_numeric_QString;			// 45052334080
    QString used_space_byte_numeric_QString;			// 74772033536

    QString file_system_type;                           // apfs or hfs or (empty in iOS)

    qint64  os_installer_timestamp_utc_numeric_qint64;  // 1627894101
    QString role_type;                            	// System or Data or Recovery ...

    QList<struct_GLOBAL_snapshots_info>snapshots_QList; // snapshots

};


struct struct_global_advance_report_plugins_tab
{

    QString plugin_name;
    QStringList plugins_tab_list;
    QString has_multiple_page;

    QStringList list_tabnm_with_tag;
};


struct struct_global_csv_reader_computed_info_plugin
{
    int tab_count_int;

    QStringList tab_name_QStringList;
    QStringList table_name_QStringList;
    QStringList tab_order_QStringList;

};

struct struct_global_csv_reader_pub_info_plugin
{
    QString plugin_name;
    QString plugin_category;
    QStringList native_os_QStringList;
    QStringList supported_os_scheme_QStringList;
    QString extraction_mode;
    QString available_for_redefined_result;
    QStringList supported_software_QStringList;
    QString log_show_supported;
    QString root_privileged;
    QString dir_iterated;

};

struct struct_global_csv_reader_cols_detail
{
    QString col_name;
    QString col_parent_child_type;
    QString col_tw_name;
    QString col_tw_visible;
    QString col_tw_order;
    QString col_tw_col_width;
    QString col_tw_data_type;

    QString col_md_name;
    QString col_md_visible;
    QString col_md_order;
    QString col_md_data_type;

    QString col_search_local;
    QString col_md_preview;

    QString col_export_path;

    QString col_report_html_name;
    QString col_report_html_visible;
    QString col_report_html_order;
    QString col_report_html_data_type;

    QString col_report_xml_name;
    QString col_report_xml_visible;
    QString col_report_xml_order;
    QString col_report_xml_data_type;

    QString col_report_csv_name;
    QString col_report_csv_visible;
    QString col_report_csv_order;
    QString col_report_csv_data_type;


    QString col_report_kml_name;
    QString col_report_kml_visible;
    QString col_report_kml_order;
    QString col_report_kml_data_type;

    QString col_g_search_visible;
    QString col_g_search_order;
    QString col_g_search_data_type;

    QString col_g_timeline_visible;
    QString col_g_timeline_order;
    QString col_g_timeline_data_type;

};

struct struct_global_csv_reader_table_detail
{
    QString table_name;
    QString table_tab_widget_type;
    QString tab_name;
    QStringList supported_os_scheme_QStringList;
    QStringList supported_software_QStringList;
    QString log_show_supported;
    QString root_privileged;
    QString dir_iterated;
    QString apple_metadata_available;
    QString html_report_available;
    QString xml_report_available;
    QString csv_report_available;
    QString kml_report_available;

    QString table_export_available;

    QString table_html_header;
    QString table_header_column_width_html;

    QString table_g_search_available;

    QString table_timeline_available;
    QStringList table_timeline_timestamp_type_QStringList;
    QStringList table_timeline_timestamp_column_name_QStringList;

    QString table_artifact_common;

    QList<struct_global_csv_reader_cols_detail> cols_QList;

};

struct struct_global_csv_reader_full_csv_info
{

    QString plugin_csv_file_name;
    QString plugin_csv_file_path;
    QString plugin_name;
    QString plugin_parent_name;
    QStringList plugin_native_os_QStringList;
    QStringList plugin_supported_os_QStringList;
    QString plugin_extraction_mode;
    QString plugin_log_show_supported;
    QString plugin_root_privileged;
    QString plugin_user_iterated;
    QString plugin_available_for_redefined_result;
    QStringList plugin_supported_software_QStringList;

    int overall_tab_count_int;
    QStringList overall_tab_name_QStringList;
    QStringList overall_table_name_QStringList;
    QStringList overall_tab_order_QStringList;


    int tab_count_int_tablewidget;
    QStringList table_name_QStringList_tablewidget;
    QStringList tab_name_QStringList_tablewidget;
    QList<QStringList> tab_supported_os_scheme_QList_tablewidget;
    QList<QStringList> tab_supported_software_QList_tablewidget;
    QStringList tab_log_show_supported_QStringList_tablewidget;
    QStringList tab_root_privileged_QStringList_tablewidget;
    QStringList tab_user_iterated_QStringList_tablewidget;
    QStringList tab_apple_metadata_available_QStringList_tablewidget;
    QStringList tab_html_report_available_QStringList_tablewidget;
    QStringList tab_xml_report_available_QStringList_tablewidget;
    QStringList tab_csv_report_available_QStringList_tablewidget;
    QStringList tab_kml_report_available_QStringList_tablewidget;
    QStringList tab_export_available_QStringList_tablewidget;
    QStringList tab_export_criteria_QStringList_tablewidget;
    QStringList tab_report_html_header_QStringList_tablewidget;
    QStringList tab_report_header_column_width_html_QStringList_tablewidget;
    QStringList tab_g_search_available_QStringList_tablewidget;
    QStringList tab_timeline_available_QStringList_tablewidget;
    QList<QStringList> tab_timeline_timestamp_type_QList_tablewidget;
    QList<QStringList> tab_timeline_timestamp_column_name_QList_tablewidget;
    QStringList tab_artifact_common_QStringList_tablewidget;

    int tab_count_int_textedit;
    QStringList tab_name_QStringList_textedit;
    QStringList table_name_QStringList_textedit;
    QList<QStringList> tab_supported_os_scheme_QList_textedit;
    QList<QStringList> tab_supported_software_QList_textedit;
    QStringList tab_log_show_supported_QStringList_textedit;
    QStringList tab_root_privileged_QStringList_textedit;
    QStringList tab_user_iterated_QStringList_textedit;
    QStringList tab_html_report_available_QStringList_textedit;
    QStringList tab_xml_report_available_QStringList_textedit;
    QStringList tab_csv_report_available_QStringList_textedit;
    QStringList tab_report_html_header_QStringList_textedit;
    QStringList tab_report_header_column_width_html_QStringList_textedit;
    QStringList tab_artifact_common_QStringList_textedit;

    int tab_count_int_stack;
    QStringList tab_name_QStringList_stack;
    QStringList table_name_QStringList_stack;
    QList<QStringList> tab_supported_os_scheme_QList_stack;
    QList<QStringList> tab_supported_software_QList_stack;
    QStringList tab_log_show_supported_QStringList_stack;
    QStringList tab_root_privileged_QStringList_stack;
    QStringList tab_user_iterated_QStringList_stack;
    QStringList tab_html_report_available_QStringList_stack;
    QStringList tab_xml_report_available_QStringList_stack;
    QStringList tab_csv_report_available_QStringList_stack;
    QStringList tab_report_html_header_QStringList_stack;
    QStringList tab_report_header_column_width_html_QStringList_stack;
    QStringList tab_artifact_common_QStringList_stack;

    int tab_count_int_tree;
    QStringList tab_name_QStringList_tree;
    QStringList table_name_QStringList_tree;
    QList<QStringList> tab_supported_os_scheme_QList_tree;
    QList<QStringList> tab_supported_software_QList_tree;
    QStringList tab_log_show_supported_QStringList_tree;
    QStringList tab_root_privileged_QStringList_tree;
    QStringList tab_user_iterated_QStringList_tree;

    int tab_count_int_thumbnail;
    QStringList tab_name_QStringList_thumbnail;
    QStringList table_name_QStringList_thumbnail;
    QList<QStringList> tab_supported_os_scheme_QList_thumbnail;
    QList<QStringList> tab_supported_software_QList_thumbnail;
    QStringList tab_log_show_supported_QStringList_thumbnail;
    QStringList tab_root_privileged_QStringList_thumbnail;
    QStringList tab_user_iterated_QStringList_thumbnail;

    int tab_count_int_chat_viewer;
    QStringList tab_name_QStringList_chat_viewer;
    QStringList table_name_QStringList_chat_viewer;
    QList<QStringList> tab_supported_os_scheme_QList_chat_viewer;
    QList<QStringList> tab_supported_software_QList_chat_viewer;
    QStringList tab_log_show_supported_QStringList_chat_viewer;
    QStringList tab_root_privileged_QStringList_chat_viewer;
    QStringList tab_user_iterated_QStringList_chat_viewer;


    //------------------------------------------------------------------------//
    QList<QStringList>display_header_QList_tablewidget;
    QList<QStringList>display_column_name_QList_tablewidget;
    QList<QStringList>display_data_type_QList_tablewidget;
    QList<QStringList>display_col_width_QList_tablewidget;
    QList<QStringList>display_order_QList_tablewidget;

    QList<QStringList>display_column_name_QList_textedit;
    QList<QStringList>display_data_type_QList_textedit;

    QList<QStringList>display_column_name_QList_stack;
    QList<QStringList>display_data_type_QList_stack;

    QList<QStringList>display_header_QList_tree;
    QList<QStringList>display_parent_child_QList_tree;
    QList<QStringList>display_column_name_QList_tree;
    QList<QStringList>display_data_type_QList_tree;
    QList<QStringList>display_col_width_QList_tree;

    QList<QStringList>display_column_name_QList_chat_viewer;
    QList<QStringList>display_data_type_QList_chat_viewer;
    //------------------------------------------------------------------------//

    QList<QStringList>metadata_header_QList_tablewidget;
    QList<QStringList>metadata_column_name_QList_tablewidget;
    QList<QStringList>metadata_data_type_QList_tablewidget;
    QList<QStringList>metadata_preview_QList_tablewidget;

    QList<QStringList>metadata_header_QList_tree;
    QList<QStringList>metadata_column_name_QList_tree;
    QList<QStringList>metadata_data_type_QList_tree;
    QList<QStringList>metadata_preview_QList_tree;

    //------------------------------------------------------------------------//

    QList<QStringList>local_search_column_name_QList_tablewidget;
    QList<QStringList>export_path_column_name_QList_tablewidget;

    //------------------------------------------------------------------------//

    QList<QStringList>report_html_display_name_QList_tablewidget;
    QList<QStringList>report_html_col_name_QList_tablewidget;
    QList<QStringList>report_html_order_QList_tablewidget;
    QList<QStringList>report_html_data_type_QList_tablewidget;

    QList<QStringList>report_html_col_name_QList_textedit;
    QList<QStringList>report_html_datatype_QList_textedit;

    QList<QStringList>report_html_col_name_QList_stack;
    QList<QStringList>report_html_datatype_QList_stack;

    QList<QStringList>report_xml_display_name_QList_tablewidget;
    QList<QStringList>report_xml_col_name_QList_tablewidget;
    QList<QStringList>report_xml_order_QList_tablewidget;
    QList<QStringList>report_xml_data_type_QList_tablewidget;


    QList<QStringList>report_csv_display_name_QList_tablewidget;
    QList<QStringList>report_csv_col_name_QList_tablewidget;
    QList<QStringList>report_csv_order_QList_tablewidget;
    QList<QStringList>report_csv_data_type_QList_tablewidget;


    QList<QStringList>report_kml_display_name_QList_tablewidget;
    QList<QStringList>report_kml_col_name_QList_tablewidget;
    QList<QStringList>report_kml_order_QList_tablewidget;
    QList<QStringList>report_kml_data_type_QList_tablewidget;

    QList<QStringList>g_search_col_name_QList;
    QList<QStringList>g_search_col_order_QList;
    QList<QStringList>g_search_col_data_type_QList;

    QList<QStringList>g_timeline_col_name_QList;
    QList<QStringList>g_timeline_col_data_type_QList;

};

struct struct_global_ios_uid_backup_detail
{
    QStringList imei_QStringList;
    QStringList phone_no_msisdn_QStringList;
    QStringList sim_no_iccid_QStringList;

    qint64 last_backup_date_numeric_qint64;

    QString build_version;
    QString display_name;
    QString guid;
    QString product_name;
    QString target_type;
    QString device_name;
    QString meid;
    QString product_type;
    QString product_version;
    QString serial_number;
    QString target_identifier;
    QString unique_identifier;
    QString itune_version;
    QString backup_size_byte_numeric_QString;
};

struct struct_global_ios_uniform_backup_detail
{
    QString product_version;
    QString product_build_version;
    QString product_name;
    QString product_model;

    QString product_type;
    QString country_code;
    QString locale_language;
    QStringList imei_number_QStringList;
    QStringList phone_number_mssid_QStringList;
    QStringList iccid_number_QStringList;
    QString device_name;
    QString display_name;
    QString time_zone;
    QString serial_number;
    QString device_guid;

};

struct struct_global_android_backup_detail
{
    QString product_build_version;
    QString product_name;
    QString product_model;
    QString device_id;
    QString bluetooth_address;
    QStringList configured_ids_QStringList;
    QStringList imei_number_QStringList;
    QStringList phone_number_mssid_QStringList;
    QStringList iccid_number_QStringList;
    QString serial_number;
    QString time_zone;
    QString country_code;
    QString locale_language;

};

struct struct_global_image_mounter_partition_info
{
    QString parent_type;                        /// Apple_HFS, Apple_CoreStroage, MS-DOS
    QString parent_name;                        /// EFI, mydisk "whatever name of the volume"
    QString parent_identifier;                  /// disk4s1, disk4s2, disk4s3
    QString parent_node;                        /// /dev/disk4s1, /dev/disk4s2, /dev/disk4s3

    QString derived_into_node;             /// The virtual disk node which come from core storage volume /dev/disk3
    QString derived_into_identifier;       /// The virtual disk identifier which came from core storage disk3
    QString derived_into_name;           /// mydisk   whatever name given to core storage volume
    QString derived_into_uuid;                        /// Volume UUID

    QString mount_path;                         /// Mount path where the volume is now mounted

    QString total_size_byte_numeric_QString;                        /// Size of the volume
    QString free_size_byte_numeric_QString;                  /// Fre Size of the volume

    QString fs_type;                    /// FS Type of the volume

    QString volume_offset;                        /// 105906176 Bytes
    QString volume_UUID;                        /// 2E651EC6-993A-4E70-9087-52185BC0A486

    QString volume_type;

    QString role_type;                            	/// System or Data or Recovery ...

};

struct struct_global_image_mounter_image_info
{
    QString image_path_1;
    QString image_name_1;
    QString shadow_file_path_1;
    QString disk_node_1;
    QString disk_identifier_1;
    QString osx_fuse_mount_point_1;

    QString image_path_2;
    QString image_name_2;
    QString shadow_file_path_2;
    QString disk_node_2;
    QString disk_identifier_2;
    QString osx_fuse_mount_point_2;

    QString image_format_type;
    QString password;
    QString image_source_type_display;
    QString image_source_type_internal;

    QList<struct_global_image_mounter_partition_info> list_struct_partition_info;

    //        QStringList direct_found_fs_node_list,all_fs_node_list,mount_path_list;
};


struct struct_global_apple_metadata_parsed_data
{
    ///
    /// Do not remove attribute variables, some are still in use and some are saved for future use
    ///

    QString filesystem_record;

    QString kMDItemTitle_Title, kMDItemKind_Kind , kMDItemFSSize_File_Size;
    QString kMDItemFSName_File_System_File_Name, kMDItemFSCreationDate_Created;
    QString kMDItemFSContentChangeDate_Modified, kMDItemDurationSeconds_Duration;
    QString kMDItemDisplayName_Name , kMDItemDateAdded_Date_Added;
    QString kMDItemContentModificationDate_Last_Modified_Date,kMDItemContentCreationDate_Created_Date;
    QString kMDItemComment_Comment , kMDItemAlbum_Album , kMDItemAudioBitRate_Audio_Bit_Rate;
    QString kMDItemLastUsedDate_Last_Opened_Date;
    QString kMDItemLatitude_Latitude;
    QString kMDItemLongitude_Longitude;
    QString kMDItemWhereFroms_Where_From;

    QStringList extended_attrs_keys_list;
    QStringList extended_attrs_values_list;

};

struct struct_global_exif_data_parsed_data
{
    QString filesystem_record;
    QString exif_make;
    QString exif_model;
    QString exif_author;
    QString exif_latitude;
    QString exif_longitude;

};

struct struct_global_fs_extracted_dir_info
{
    QString dir_name;
    QString dir_path;
};

struct struct_global_indexed_keyword_found_file_info
{
    QString file_name;
    QString file_path;
    QString file_size;
    uint64_t file_total_word_count;
    QString modified_date;
    QStringList word_hit_location_list;
    QString index_path;
};

struct struct_global_dbtable_info
{
    QString table_name;
    QStringList cols_list;
};

struct struct_global_folder_db_info
{
    QList<struct_global_dbtable_info> list_folder_db_table_info;

    bool bool_is_plugin_db;
    QString plugin_name_if_belongs;

    QStringList db_path_list;
    QStringList plugins_tabs_list;
    QString source_os_scheme;

};


struct struct_GLOBAL_plugin_info
{
    QString plugin_name;
    QList<struct_global_dbtable_info> list_db_table_info;
};

struct struct_global_selected_regular_filters_info
{
    bool bool_is_structure_is_empty;
    QString file_name_str;
    QString file_size_str;

    QString change_date_str;
    QString added_date_str;
    QString last_modified_date_str;
    QString last_read_date_str;
    QString created_date_str;
    bool bool_is_combined_by_or;

};

struct struct_global_sub_node_entry_for_ios_7_8_9
{
    QByteArray sub_domain;
    QByteArray sub_filename;
    QByteArray sub_fileID;
    qint64 sub_file_size;
};

struct struct_global_main_node_entry_for_ios_7_8_9
{
    QByteArray node_domain;
    QByteArray node_filename;
    QByteArray node_fileID;
    QByteArray flags;
    qint64 file_size;

    QList<struct_global_sub_node_entry_for_ios_7_8_9> list_subdomain;
};

struct struct_global_main_node_entry_for_ios_10_11
{
    QByteArray domain_name;
    QByteArray file_absolutepath_without_domain;
    QByteArray file_id;
    QByteArray datahash;
    QByteArray flags;
};

struct struct_global_global_report_info
{
    int report_maker;
    QString report_name;
    QStringList report_type_list;
    QString report_scope;
    QStringList report_path_list;
    QStringList selected_tags_list;
    QStringList report_plugin_info_list;
    narad_muni *narad_muni_report_obj;
};

struct struct_global_tagging_tag_notes_db_updation
{
    QString bookmark;
    QString category;
    QString record_no;
    QString notes_icon;
    QString plugin_name;
    QString tab_name;
    QString csv_plugin_name;
    QString csv_tab_name;
    QString table_name;
    QString recon_tag_value;
    QString notes;
    QString os_scheme_display;
    QString source_count_name;
    QString feature_db_path;
    QString item0;
    QString item1;
    QString item2;
    QString item3;
    QString item4;
    QString item5;
    QString item6;
    QString item7;
    QString item8;
    QString item9;
    QString item10;
    QString item11;
    QString item12;
    QString item13;
    QString item14;
    QString item15;
    QString item16;
    QString item17;
    QString item18;
    QString item19;
};


struct struct_global_source_evidencer
{
    QString evidence_by_examiner;
    QString description_by_examiner;
    QString source_password_by_examiner;
};

struct struct_global_hashset_db_data
{
    QString hashset_db_path;
    QString hashet_table_name;
    QString hashet_column_name;
    QString hash_name_str;
};

struct struct_global_tagging_mobile_backup
{
    QString INT_record;
    QString plugin_name; // Mobile Backup
    QString tab_name; // Backup

    QString source_count_name;

    QString item0;
    QString item1;
    QString item2;
    QString item3;
    QString item4;
    QString item5;
    QString item6;
    QString item7;
    QString item8;
    QString item9;
    QString item10;
    QString item11;
    QString item12;
    QString item13;
    QString item14;
    QString item15;
    QString item16;
    QString item17;
    QString item18;

};

struct struct_global_tagging_disk_images
{
    QString INT_record;
    QString plugin_name; // Disk Images
    QString tab_name; // Sources

    QString source_count_name;
    QString item0;
    QString item1;
    QString item2;
    QString item3;
    QString item4;
    QString item5;

};

struct struct_global_added_timezone_details
{
    QString offset ;
    QString timezone ;
    QString start_date_str;
    QString end_date_str;
    QString display_timezone_offset_name;

    qint64 start_date_numeric_qint64;
    qint64 end_date_numeric_qint64;
};

struct struct_global_default_timezone_details
{
    QString d_examiner_selected_date_format_readable_QString;
    qint64  d_examiner_selected_date_format_numeric_qint64 = 0;

    QString d_examiner_selected_timezone_readable_QString;
    qint64  d_examiner_selected_timezone_numeric_qint64 = 0;

    QString d_case_created_machine_timezn_readable_QString;
    qint64  d_case_created_machine_timezn_numeric_qint64 = 0;

    QString timezone_offset_name;
};

struct struct_global_run_fs_modules_details
{
    QString fs_record_no;
    QString complete_file_path;
    QString display_filepath;
    QString source_count_name;

    QString examiner_selected_file_or_dir_name;
};

struct struct_global_export_file_system_record
{
    QString record_no;
    QString source_count_name;

    QString displayed_file_name ;


    QString db_file_path;
    QString copy_dir_path;


    QString display_file_path;// required for ios export
};


struct struct_global_artifacts_export_fileinfo
{
    QStringList displayed_file_path;
    QStringList complete_export_filepaths;
    QStringList recon_file_info_path_list;
    QStringList recon_filefrom_list;
};

struct struct_global_export_as_hashes_details
{
    QString fs_record_no;
    QString complete_file_path;
    QString display_filepath;
    QString display_filename;
    QString source_count_name;
    QString change_timestamp;
    QString modif_timestamp;
    QString last_acces_timestamp;
    QString create_timestamp;
    QString examiner_selected_file_or_dir_name;
};

struct struct_global_right_clicks_status_depends_on_file_existance
{
    QString feature_name;
    QString plugin_name;
    QString tab_name;
    QString record_no;
    QString source_count_name;
    QString result_dir;
    int selected_row_count = 0;
    QStringList arguments_values_list;
};

struct struct_global_available_hashset_data
{
    QString saved_table_name;
    QString md5_hash_column;
    QString recon_filefrom ;
    QString recon_file_infopath;
    QString hashes_db_path ;
    qint64  records_count;
};

struct struct_global_hex_viewer_open_file_data
{
    bool bool_is_file_in_bucket;
    QString hex_viewer_display_name;
    QString complete_file_path;
    QString display_file_path;
    QString record_no_str;
    QString source_count_name;
    QString plugin_name_str;
    QString plugin_tab_name_str;
    QString hex_feature_db_path;
    QString recon_filefrom;
    QString recon_file_infopath;

};

struct struct_global_file_hashes
{
    QString md5_hash;
    QString sha1_hash;
};


Q_DECLARE_METATYPE(struct_global_system_account_info)

#endif // HEADER_GLOBAL_STRUCTURES

