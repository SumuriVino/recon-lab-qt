#ifndef RECON_HELPER_STANDARD_H
#define RECON_HELPER_STANDARD_H

#include <QWidget>
#include "recon_static_functions.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"
#include "header_global_variables_3.h"




class recon_helper_standard : public QObject
{
    Q_OBJECT

public:
    recon_helper_standard(QObject *parent = 0);
    ~recon_helper_standard();

    QStringList get_stringlist_from_db_by_dbpath(QString command, int pos, QString destination_db_file, QString caller_func);
    QStringList get_stringlist_from_db_by_dbreference(QString command, int pos, QSqlDatabase &destination_db, QString caller_func);

    QStringList get_stringlist_from_db_with_addbindvalues_by_dbpath(QString command, QStringList values, int pos, QString destination_db_file, QString caller_func);
    QStringList get_stringlist_from_db_with_addbindvalues_by_dbreference(QString command, QStringList values, int pos, QSqlDatabase &destination_db, QString caller_func);

    QString get_string_from_db_by_dbpath(QString command, int pos, QString destination_db_file,QString caller_func);
    QString get_string_from_db_by_dbreference(QString command, int pos, QSqlDatabase &destination_db, QString caller_func);

    QString get_string_from_db_with_addbindvalues_by_dbpath(QString command,QStringList values, int pos, QString destination_db_file,QString caller_func);
    QString get_string_from_db_with_addbindvalues_by_dbreference(QString command, QStringList values, int pos, QSqlDatabase &destination_db, QString caller_func);

    qint64 get_db_total_record_by_dbpath(bool bool_process_db_all_tables, QString db_path, QStringList tables_list, QString caller_func);
    qint64 get_db_total_record_by_dbreference(bool bool_process_db_all_tables, QSqlDatabase &destination_db,  QStringList tables_list, QString caller_func);

    bool execute_db_command_by_dbpath(QString command, QString destination_db_file, QString caller_func);
    bool execute_db_command_by_dbreference(QString command, QSqlDatabase &destination_db, QString caller_func);

    bool execute_db_command_with_addbindvalues_by_dbpath(QString command, QStringList values, QString destination_db_file, QString caller_func);
    bool execute_db_command_with_addbindvalues_by_dbreference(QString command, QStringList values, QSqlDatabase &destination_db, QString caller_func);


    qint64 get_intvalue_from_db_with_addbindvalues_by_dbpath(QString command, QString db_path, int pos, QStringList values, QString caller_func);
    qint64 get_intvalue_from_db_with_addbindvalues_by_dbreference(QString command, QSqlDatabase &destination_db, int pos, QStringList values, QString caller_func);

    QString get_file_infopath_by_dbpath(QString record_no, QString destination_db_path, QString table_name, QString source_count_name, QString caller_func);
    QString get_file_infopath_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func);
    bool is_it_dir_record_no_by_dbpath(QString record_no, QString destination_db_path, QString table_name, QString source_count_name, QString caller_func);
    bool is_it_dir_record_no_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func);
    bool check_value_existence_in_db_by_dbpath(QString command, int pos, QString value_to_match, QString destination_db_file, QString caller_func);
    bool check_value_existence_in_db_by_dbreference(QString command, int pos, QString value_to_match, QSqlDatabase destination_db, QString caller_func);


    QSqlQuery get_executed_sqlquery_from_db_by_dbreference(QString command, QSqlDatabase &destination_db, QString caller_func);
    QString get_string_from_db_by_dbpath_no_trimmed(QString command, int pos, QString destination_db_file, QString caller_func);

    QString get_filename_by_filepath_without_fileinfo(QString filepath);
    QString pub_get_absolute_dir_path_without_fileinfo(QString filepath);

    bool pub_check_path_is_directory_by_stat(QString filepath, QString caller_func);
    bool pub_check_path_is_file_by_stat(QString filepath, QString caller_func);
    bool pub_check_path_is_directory_by_fs_dbpath(QString filepath, QString fs_record_no, QString fs_db_path, QString caller_func);
    bool pub_check_path_is_directory_by_fs_dbreference(QString filepath, QString fs_record_no, QSqlDatabase destination_db, QString caller_func);
    bool pub_check_path_is_file_by_fs_dbpath(QString filepath, QString fs_record_no, QString fs_db_path, QString caller_func);
    bool pub_check_path_is_file_by_fs_dbreference(QString filepath, QString fs_record_no, QSqlDatabase destination_db, QString caller_func);

    QString get_apple_metadata_key_value_from_result(QString record_no, QString table_name, QString db_path, QString caller_func);
    QStringList extract_all_tags_from_db(int extraction_type, QStringList tables_list, QString destination_db_file, QString caller_func);
    QStringList get_plist_displayname_list_from_result_db(QString result_path, QString caller_func);
    QStringList get_log_displayname_list_from_result_db(QString result_path, QString caller_func);

    QStringList get_tablist_of_plugin_from_case_configuration_by_dbreference(QSqlDatabase &destination_db, QString plugin_name, QString caller_func);
    void create_db_index_by_db_path(QString index_name, QString column_name, QString table_name, QString db_path, QString caller_func);
    QString create_separate_tag_db_for_tag_search(QString tags_name, QString caller_func);
    QStringList get_total_extracted_plugin_with_extracted_tab_list(QString caller_func);
    QStringList extract_all_record_no_for_selected_tags(QString table_name, QString destination_db_path, QStringList tags_list, QString caller_func);
    QStringList get_sqlite_namelist_from_db(QString result_path, QString caller_func);
    QStringList get_hex_namelist_from_db(QString result_path, QString caller_func);
    QString get_category_of_plugin_by_dbreference(QSqlDatabase &destiantion_db, QString plugin_name, QString caller_func);
    QStringList get_keyword_search_namelist_from_result_db(QString caller_func);
    QString get_colour_name_of_tag_from_db(QString tag_name, QString dbPath, QString caller_func);
    QString get_color_hex_from_color_name(QString color_name, QString dbPath, QString caller_func);

    void pub_enable_or_disable_right_clicks_depends_on_file_existance(struct_global_right_clicks_status_depends_on_file_existance obj, QMenu *menu, QString caller_func);

    QString fetch_metadata_for_ram_analysis_saved(QString record_no, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_saved_graph(QString record_no, QString plugin_name, QString caller_func);

    struct_global_fetch_metadata fetch_file_system_metadata_for_current_record(QString record, QString source_count_name, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_content_search(QString record_no, QString db_path, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_screenshots_tag_entry(QString file_path, QString record_no, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_plist_viewer_tag_entry(QString file_path, QString tag_name, QString plist_viewer_name, QString record_no, QString source_count_name, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_sqlite_viewer_tag_entry(QString file_path, QString tag_name, QString record_no, QString db_table_name, QString source_count_name, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_saved_timeline_graph_entry(QString file_path, QString record_no, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_saved_maps_entry(QString file_path, QString record_no, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_features_plugins(QString table_name, int feature_tab_index, QString record_no, QString feature_plugin, QString db_path, QString command, struct_GLOBAL_witness_info_source struct_info, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_saved_timeline_graph_entry_storyboard(QString file_path, QString record_no, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_carved_files(QString db_path, QString record_no, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_CSV_and_INT_based(QString table_name, int tab_index, QString record_no, QString feature_plugin, QString db_path, QString source_count_name, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_hex_viewer(QString record_no, QString source_count_name, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_of_plugin(QString plugin_name_str, QString tab_name_str, QString record_no_str, QString result_dir_path, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_log_viewer_tag_entry(QString file_path, QString tag_name, QString log_viewer_name, QString record_no, QString source_count_name, QString tagged_data, QString caller_func);
    struct_global_fetch_metadata fetch_metadata_for_unified_logs_tag_entry(QString record_no, QString source_count_name, QString caller_func);

    QIcon get_qicon_of_record_no_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func);

    struct_global_recon_file_info pub_get_recon_file_info_by_dbreference(QString plugin_name, QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func);

    QStringList get_export_path_list_of_plugin_current_record(QString plugin_db_file, QStringList export_col_name_list, QString table_name, QString record_no, QString recon_result_dir_path, QString caller_func);
    struct_global_artifacts_export_fileinfo get_export_path_list_and_display_path_list_for_plugin_current_record(QString plugin_db_file, QStringList export_col_name_list, QString table_name, QString record_no, QString caller_func);
    QStringList get_keyword_list_for_search_from_db(QString caller_func);
    void update_narad_variable_with_apple_metadata_info(QString caller_func);
    void update_narad_variable_with_exif_metadata_info(QString caller_func);


    QString get_virtual_source_path_of_current_record(QString record_no, QString table_name, QString plugin_db_path, QString caller_func);
    QString pub_get_result_version();
    QString pub_get_fs_display_path_according_recon_file_infopath(QString recon_file_infopath, QString source_count_name);
    QString get_source_count_name_on_current_record(QString record_no, QString table_name, QString plugin_db_path, QString caller_func);

    QStringList extract_macos_current_login_apple_ids(QString path, QStringList users_list);

    QString get_target_os_scheme_display_on_current_record_source(QString record_no, QString table_name, QString plugin_db_path, QString caller_func);
    QString get_target_os_scheme_internal_on_current_record_source(QString record_no, QString table_name, QString plugin_db_path, QString caller_func);

    QString get_complete_timezone_name_label(QString time_zone, QString caller_func);
    qint64 get_time_zone_offset_with_sign(QString timezone_string, QString caller_func);
    qint64 get_time_zone_offset_without_sign(QString country_timzone_str , QSqlDatabase &destination_db, QString caller_func);

    void notes_search_delete_dbs(QString result_feature_dir, QString tabname, QString record_no, QString notes_data, QString category, QString caller_func);
    QStringList get_tags_and_colours_list_from_db(QString command, QString destination_db_file, QString caller_func);

    QString get_adhoc_path_for_database_using_QFile(QString main_source_path);
    QString get_adhoc_path_for_database_using_cp_without_sudo(QString source_path, QString destination_dir_path);
    QString get_adhoc_path_for_database_using_cp_with_sudo(QString source_path, QString password, QString destination_dir_path, QString caller_func);

    bool check_is_audio_by_extn(QString filename, QString caller_func);
    bool check_is_video_by_extn(QString filename, QString caller_func);
    bool check_is_it_image_file(QString file_path, QString display_file_path, QString caller_func);

    QString mining_suffix(QString path, QString caller_func);
    void translate_tablewidget_horizontal_header_item_all(QTableWidget *tbl_wdgt, QString caller_func);

    bool is_any_source_exist();

    QString pub_get_raw_data_for_image_file_preview(QString filepath);
    bool is_special_file_for_preview(QString filepath, QString display_file_path, QString caller_func);
    bool is_gallery_view_suppoprted_file(QString file_name, QString caller_func);

    bool is_gallery_view_suppoprted_video_file(QString file_name);
    bool check_record_is_decompressed_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString caller_func);
    bool check_record_is_decompressed_by_dbpath(QString record_no, QString db_path, QString caller_func);
    void pub_add_file_details_to_hex_feature_db_for_sendto_bucket(struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj, QString caller_func);

    QString run_command_with_arguments_process_obj(QProcess *m_obj, QString command, QStringList arguments, QString caller_func);
    QMap<QString, QMap<QString, QString> > extract_apple_metadata_from_script(QString text_file_path, QString csv_file_path, QStringList apple_all_metadata_attribute_list);

    bool bool_is_permission_restricted_file(QString mfile_path, QString caller_func);

    void pub_create_disabled_empty_action_in_menu(QList<QMenu *> menu_list, QString caller_func);
    QString pub_get_html_formated_filename(QString regular_name_str, QString caller_func);
    void create_db_index_by_db_reference(QString index_name, QString column_name, QString table_name, QSqlDatabase destination_db, QString caller_func);


    bool pub_change_database_mode_to_wal_by_db_reference(QSqlDatabase db_refrnce, QString caller_func);
    bool pub_change_database_mode_to_wal_by_db_path(QString db_path_str, QString caller_func);

    QStringList pub_get_records_bookmarked_from_table_display_by_db_refrence(QSqlDatabase source_db, QString table_name, QString records_col_str, QString caller_func);
    void pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(QMap<QString, QStringList> &map_bookmarked_records, QString key_str, QString record_str);
    void pub_remove_bookmarked_record_to_map_for_detailed_info_bookmark_control(QMap<QString, QStringList> &map_bookmarked_records, QString key_str, QString record_str);
    bool pub_check_bookmarked_record_in_map_for_detailed_info_bookmark_control(QMap<QString, QStringList> &map_bookmarked_records, QString key_str, QString record_str);

    struct_global_file_hashes pub_create_md5_and_sha1_hash(QString file_path);

    void pub_set_filesystem_modules_thread_run_status(QString job_type, QString status, QString caller_func);
    QString pub_get_filesystem_modules_thread_run_status(QString job_type, QString caller_func);

    QString pub_get_app_license_path();
    QStringList get_apple_metadata_from_result_apple_metadata_db(QString file_path, QString source_count_name);
    QStringList get_apple_metadata_from_result_extracted_content_apple_metadata_db(QString file_path, QString tmp_apple_metadata_db_path, QString fs_db_path);
    QStringList get_total_tab_name_from_db_by_db_path(QString db_path, QString caller_func);
    int get_maximum_root_count_index_from_db_and_text_file();
    int get_maximum_source_count_index_from_db_and_text_file();


    void alter_db_tables_to_add_column_by_dbpath(QStringList table_list, QStringList new_colname_list, QStringList new_coltype_list, QString destination_db_file);
    void alter_db_tables_to_add_column_by_dbreference(QStringList table_list, QStringList new_colname_list, QStringList new_coltype_list, QSqlDatabase &destination_db);

    void alter_db_tables_to_rename_column_by_dbpath(QStringList table_list, QStringList existing_colname_list, QStringList new_colname_list, QString destination_db_file);
    void alter_db_tables_to_rename_column_by_dbreference(QStringList table_list, QStringList existing_colname_list, QStringList new_colname_list, QSqlDatabase &destination_db);
    bool source_applicable_for_extensive_module(QString src_count_name);
    QString pub_fetch_examiner_comments_data();

    QString get_censored_filepath(QString src_file_path, QString dest_file_path);
    QString get_tag_name_of_record_no_by_source_count_name(QString record_no, QString source_count_name);
    QString get_new_mount_path_of_time_machine_backup_image_and_snapshots(QString recieved_mount_path, QString m_type);

signals:

private slots:

    void slot_generic_process_finished(int);
private:

    QList<struct_global_hashset_db_data> hashset_db_data_list;

    QStringList numeric_image[10];
    QStringList alphabetical_image[26];
    void initialize_image_extension_list();

    QStringList numeric_vedio[10];
    QStringList alphabetical_vedio[26];
    void initialize_vedio_extension_list();

    QStringList numeric_audio[10];
    QStringList alphabetical_audio[26];
    void initialize_audio_extension_list();

    QString get_snapshots_mount_path(QString source_count_name, QString db_path);

    QString get_hex_viewer_format_for_metadata(QString hex_bytes);
    bool check_is_image_by_mimetype(QString filepath, QString display_file_path_str, QString caller_func);

    bool check_is_image_by_extn(QString filename, QString display_file_path_str, QString caller_func);
    QMap<QString, QMap<QString, QString> > extract_apple_metadata_from_spotlight_text_file_data(QStringList each_inode_data_list, QStringList apple_all_attribute_name_list);
    bool bool_process_generic = false;
    QStringList parse_spotlight_parser_file_for_each_inode(QString text_file_path);
};

#endif
