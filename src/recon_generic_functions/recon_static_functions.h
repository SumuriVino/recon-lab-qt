#ifndef RECON_STATIC_FUNCTIONS_H
#define RECON_STATIC_FUNCTIONS_H

#include <QtCore>
#include <QDateTimeEdit>
#include <QtSql/QtSql>
#include "encryption/encryptor_file_1/header.h"
#include "encryption/encryptor_file_2/encryptor_file_2.h"
#include "encryption/encryptor_string/encryptor_string.h"
#include "header_macro_generic.h"
#include "header_identical_1.h"
#include "header_identical_2.h"
#include "header_identical_3.h"
#include <QApplication>
#include "openssl/md5.h"
#include "openssl/sha.h"
#include "narad_muni/narad_muni.h"
#include "header_global_structures.h"
#include "header_global_enum.h"
#include "header_global_variables_2.h"
#include "recon_generic_functions/recon_helper_process.h"

#include <QMenu>
#include <set>


#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


//#define MACRO_RECON_STATIC_APP_RUN_BY_FORENSO                   // Always disable this macro for final build


//==========NOTARISE==Starts==========//
//Do enable for final build.
#define MACRO_RECON_STATIC_NOTARISE_ESSENTIALS

//==========NOTARISE==Ends==========//


class recon_static_functions : public QWidget
{

public:
    explicit recon_static_functions(QWidget *parent = 0);

    static QString size_string(long double size, QString caller_func);
    static QString get_app_version();


    static QString get_app_build_modified();
    static QString get_app_build_regular();

    static QString get_code_timestamp();
    static QString get_recon_library_version();
    static QString get_app_name();
    static QString get_appilcation_relative_path();

    static void encrypt_file_1(QString file_path, QString caller_func);

    static QString decrypt_file_1(QString file_path, QString caller_func);

    static bool encrypt_file_2(QString source_file, QString dest_file, QString caller_func);
    static bool decrypt_file_2(QString source_file, QString dest_file, QString caller_func);

    static QString encrypt_string(QString plain_str, QString caller_func);
    static QString decrypt_string(QString encrypted_str, QString caller_func);

    static void app_debug(QString data, QString caller_func);
    static void debug_intensive(QString data, QString caller_func);
    static void debug_conditional(QString data, QString caller_func);


    static QString prepare_callerfun(QString data);


    static QString get_debug_filepath_regular();
    static QString get_debug_filepath_examiner();
    static QString get_debug_filepath_root();
    static QString get_debug_filepath_source();

    static QString get_available_stringname(QString string_name, QStringList stringlist_values, QString caller_func);
    static QString get_available_filename(QString file_name, QString path, QString caller_func);
    static QString get_available_filename_as_last_braces(QString file_name, QString path, QString caller_func);
    static  bool is_file_exist_in_dir(QString dir_path, QString rcvd_filename, QString caller_func);

    static void set_result_path(QString path);

    static void create_debug_file();
    static void close_debug_file();


    static bool copy_directory( QString srcPath,  QString dstPath, QString caller_func);

    static QStringList get_search_string_list(QString keyword, QString caller_func);

    static QString get_partial_command_forsearch(QStringList search_cols_list, QStringList search_string_list, QString caller_func);


    static QString mining_size(QString path, QString caller_func);

    static QString human_readable_size(QString size_in_bytes, QString caller_func);
    static QString human_readable_size(qint64 size_in_qint64, QString caller_func);

    static bool remove_safely_blank_dir_OR_file(QString path, QString caller_func);




    static  qint64 overwrite_data_into_file(QString file_path, QString data, QString caller_func);
    static  qint64 append_data_into_file(QString file_path, QString our_data, int append_status, QString caller_func);
    static bool remove_data_contain_line_from_file(QString file_path, QString data, QString caller_func);

    static QString get_plain_data_from_encrypt_file_1(QString file_path, QString caller_func);
    static void write_diskutil_info_in_file(QString file_path , QString disk_node, QString caller_func);
    static void write_diskutil_list_in_file(QString file_path , QString caller_func);
    static void write_mountinfo_in_file(QString file_path, QString caller_func);


    static QString recon_result_path;
    static QString debug_file_path_regular;
    static QString debug_file_path_examiner;
    static QString debug_file_path_root;
    static QString debug_file_path_source;
    static QString debug_file_path_sumuri;
    static qint64 recon_jaari_smay;
    static QFile debug_file_regular_ptr;


    static void set_recon_jaari_smay();
    static qint64 get_recon_jaari_smay();



    static QStringList get_system_user_list_of_macOS(QString path, QString src_type_intrnl, QString caller_func);

    static QStringList get_mounted_media_list(QString caller_func);
    static QStringList get_associated_tabs(QString plgn_name,QStringList plugin_and_associated_tabs_list, QString caller_func);
    static bool is_having_macos_user(QString path, QString caller_func);

    static QString create_sha1(QString file_path, QString caller_func);
    static QString create_md5(QString file_path, QString caller_func);
    static QString get_human_readable_time(QString str, QString caller_func);
    static  bool is_it_plist_file(QString file_path, QString caller_func);
    static QString get_tags_icon_path(QString colour, QString data, QString caller_func);

    static QString convert_bytes_to_kilobytes_for_display(QString received_bytes, QString caller_func);


    static QString extract_time_zone_xcode_mm_iOS(QString path, QString caller_func);

    static bool check_is_it_sqlite_file_by_signature(QString file_path, QString caller_func);
    static bool check_is_it_registry_file_by_signature(QString file_path, QString caller_func);

    static bool check_is_it_plist_file_by_signature(QString filepath, QString caller_func);

    static QString extract_time_zone_macOS(QString source_path, QString caller_func);

    static struct_global_macOS_global_preferences extract_global_preferences_xcode_mm_macOS(QString source_path, QString caller_func);
    static QString extract_model_number_xcode_mm_macOS(QString source_path, QString caller_func);

    static QString extract_os_version_extended_xcode_mm_macOS(QString source_path, QString caller_func);
    static QString extract_os_version_full_xcode_mm_macOS(QString source_path, QString caller_func);
    static QString convert_cfstring_to_string_xcode_mm(CFStringRef str, QString caller_func);
    static QString get_nsstring_values_from_nsdictionary_xcode_mm(void *ptr_nsstring, void *ptr_nsdictionary, QString caller_func);
    static QString get_complete_file_path(QString start_str, QString end_str, QString caller_func);

    static struct_global_ios_uid_backup_detail extract_backup_details_iOS_uid(QString path, QString caller_func);

    static bool get_associated_tabs_with_export(QString plgn_name, QString received_tab_name, QStringList plugin_and_associated_tabs_list, QString caller_func);

    static QString get_sqlite_viewer_name();
    static QString get_tag_icon_path_from_colour_name(QString colour, QString caller_func);
    static bool check_file_signature(QString filepath, QString signature_str, QString offset_str, QString caller_func);
    static void copy_tablewidget_data_to_clipboard(m_tablewidget *my_tablewidget, QString caller_func);
    static QString convert_human_readable_size_to_bytes(qint64 size_in_qint64, QString size_unit, QString caller_func);
    static QString get_value_of_key_from_diskutil_info_plist_file(QString plist_file_path, int type, QString key_name, QString caller_func);

    static QString get_value_of_key_from_diskutil_apfs_list_plist_volume(QString plist_file_path, QString volumn_identifier, QString key_name, int key_datatype, QString caller_func);
    static QString extract_build_version_xcode_mm_macOS(QString source_path, QString caller_func);

    static qint64 get_round_up_value_near_hundred_or_thousand(qint64 value, QString caller_func);
    static QString prepend_zero(QString rec, QString upto, QString caller_func);
    static QString remove_record_count_from_feature_name(QString str);
    static bool check_existance_of_action_in_menu(QMenu *menu_hashset_add_files, QString db_name, QString db_path);


    static QString extract_user_name_from_sourcefile(QString path, QString received_initial_path_from_pub);


    static bool is_live_imager_module_list_file_present(QString mount_path);

    static void do_directory_blank_completely(QString dir_path, QString caller_func);
    static void do_directory_blank_partially(QString dir_path, QStringList files_to_skip_list, QString caller_func);

    static void print_image_mounting_info_on_terminal(QList<struct_global_image_mounter_image_info> list_image_info, QString caller_func);


    static void print_destination_drive_info(struct_global_destination_drive_info pwd_obj, QString caller_func);

    static void print_examiner_info(struct_global_desktop_info pwd_obj, QString caller_func);
    static void print_examiner_info_for_sumuri(struct_global_desktop_info pwd_obj, QString caller_func);

    static void print_witness_root_info(QList<struct_GLOBAL_witness_info_root> list_root_info, QStringList newly_added_root_count_name);
    static void print_witness_source_info(QList<struct_GLOBAL_witness_info_source> list_source_info, QStringList processed_source_count_name);

    static QString convert_string_to_unicode(QString file_data, QString caller_func);

    static bool check_path_removable(QString dir_path, QString caller_func);

    static QString get_icon_path_from_colour_name(QString colour, QString caller_func);
    static QString get_tags_on_add_or_remove_bookmarks(QString tag_value, QString status, QString caller_func);
    static QString get_exportfilelist_command();
    static void open_file_with_specified_app(QString file_path, QString application_path, QString caller_fun);
    static QStringList get_unmatched_varible_list_of_parentlist(QStringList parent_list, QStringList child_list, QString caller_func);

    static void clear_variables_witness_info_source_structure(struct_GLOBAL_witness_info_source &struct_source_info);
    static void clear_variables_witness_info_root_structure(struct_GLOBAL_witness_info_root &struct_root_info);
    static void clear_variables_image_mounting_partiton_info(struct_global_image_mounter_partition_info &prtn_info);
    static void clear_variables_image_mounting_image_info(struct_global_image_mounter_image_info &img_info);
    static void clear_variables_detailed_information(struct_global_set_metadata_info &st_meta_d_obj);
    static void clear_variables_desktop_info_all(struct_global_desktop_info &mobj);
    static void clear_variables_desktop_password(struct_global_desktop_info &mobj);
    static void clear_variables_desktop_general(struct_global_desktop_info &mobj);

    static void clear_variables_destination_drive(struct_global_destination_drive_info &mobj);

    static void clear_variables_hex_viewer_open_file_data(struct_global_hex_viewer_open_file_data &struct_hex_file_info);
    static void clear_variables_recon_and_artifact_desktop_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_desktop_info &mobj);
    static void clear_variables_recon_and_artifact_general_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_general_info &mobj);
    static void clear_variables_recon_and_artifact_common_source_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_source_info &mobj);
    static void clear_variables_recon_and_artifact_common_system_account_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_system_account &mobj);
    static void clear_variables_ios_uniform_backup_structure(struct_global_ios_uniform_backup_detail &mobj);
    static void clear_variables_system_account_info(struct_global_system_account_info &m_obj);

    static QString get_fs_extraction_source_path(narad_muni *narad_muni_obj, QString src_type_intrnl, QString virtual_source_path, QString caller_func);
    static QString get_filesystem_db_create_command(QString table_name);


    static QString get_modify_password(QString m_password, QString caller_func);
    static QString pub_get_simplified_password_for_shell_script(QString passwd, QString caller_func);
    static QString get_modified_password_replace_symbol_with_hex(QString passwd, QString caller_func);

    static bool is_source_supported_for_tsk(struct_GLOBAL_witness_info_source src_obj);
    static QString get_forensic_image_type(QString file_path);

    static void get_graykey_device_name_and_timezone_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func);
    static void get_graykey_serial_no_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func);
    static void get_graykey_device_guid_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func);
    static void get_graykey_imei_phone_and_iccid_no_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func);
    static void get_graykey_product_type_and_country_code_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func);
    static void get_graykey_product_vrsn_build_model_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func);

    static QStringList get_system_user_list_from_system_account_QList(QList<struct_global_system_account_info> system_account_QList, QString caller_func);
    static QString get_system_users_and_domain_users_from_system_account_QList(QList<struct_global_system_account_info> list_sys_account_info, QString caller_func);


private:

    static QString extract_time_zone_macOS_by_api_current_machine(QString caller_func);
    static QString extract_time_zone_macOS_by_command(QString path_str, QString caller_func);
    static QString extract_time_zone_macOS_by_file(QString source_path, QString caller_func);


};


#endif
