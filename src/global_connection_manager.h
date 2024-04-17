#ifndef GLOBAL_CONNECTION_MANAGER_H
#define GLOBAL_CONNECTION_MANAGER_H

#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


class global_connection_manager : public QObject
{
    Q_OBJECT
public:
    explicit global_connection_manager(QObject *parent = nullptr);


    /// File System - Bookmarks
    void pub_set_bookmark_in_fs_source_db(QStringList INT_list, QString source_count_name);
    void pub_remove_bookmark_in_fs_source_db(QStringList INT_list, QString source_count_name);
    void pub_add_bookmark_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj);
    void pub_delete_bookmark_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj);
    ///

    /// File System - Tags
    void pub_set_tag_in_fs_source_db(QStringList INT_list, QString source_count_name, QString tag_name);
    void pub_remove_tag_in_fs_source_db(QStringList INT_list, QString source_count_name);
    void pub_add_tag_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj, QString tag_name);
    void pub_delete_tag_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj);
    ///

    /// File System - Notes
    void pub_set_notes_in_fs_source_db(QStringList INT_list, QString source_count_name, QString notes_value);
    void pub_remove_notes_in_fs_source_db(QStringList INT_list, QString source_count_name);
    void pub_add_notes_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj, QString notes_value);
    void pub_delete_notes_for_file_system(QList<struct_global_tagging_tag_notes_db_updation> obj);
    ///



    /// Plugin Viewer - Bookmarks
    void pub_set_bookmark_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_remove_bookmark_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_add_bookmark_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_delete_bookmark_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString source_count_name);
    ///

    /// Plugin Viewer - Tags
    void pub_set_tag_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name, QString tag_name);
    void pub_remove_tag_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_add_tag_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString tag_name);
    void pub_delete_tag_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString source_count_name);
    ///


    /// Plugin Viewer - Notes
    void pub_set_notes_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name, QString notes_value);
    void pub_remove_notes_in_plugin_viewer_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_add_notes_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString notes_value);
    void pub_delete_notes_for_plugin_viewer(QStringList INT_list, QString plugin_name, QString tab_name, QString source_count_name);
    ///

    /// Artifacts Keyword_search - Notes
    void pub_remove_notes_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_set_notes_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name, QString notes_value);


    /// Artifacts Keyword_search - Tags
    void pub_remove_tags_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_set_tags_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name , QString tag_name);


    /// Artifacts Keyword_search - Bookmarks
    void pub_remove_bookmarks_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name);
    void pub_set_bookmarks_in_artifacts_keyword_search_db(QStringList INT_list, QString plugin_name, QString tab_name);

    /// Artifact Timeline - Bookmarks
    void pub_set_bookmark_in_artifact_timeline_db(QStringList INT_list);
    void pub_remove_bookmark_in_artifact_timeline_db(QStringList INT_list);
    void pub_remove_bookmark_in_artifact_timeline_db_from_outside(QList<struct_global_tagging_artifact_timeline_record>);
    void pub_add_bookmark_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record>);
    void pub_delete_bookmark_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record>);
    ///

    /// Artifact Timeline - Tags
    void pub_set_tag_in_artifact_timeline_db(QStringList INT_list, QString tag_name);
    void pub_remove_tag_in_artifact_timeline_db(QStringList INT_list);
    void pub_remove_tag_in_artifact_timeline_db_from_outside(QList<struct_global_tagging_artifact_timeline_record>);
    void pub_add_tag_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record>, QString tag_name);
    void pub_delete_tag_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record>);
    ///

    /// Artifact Timeline - Notes
    void pub_set_notes_in_artifact_timeline_db(QStringList INT_list, QString notes_value);
    void pub_remove_notes_in_artifact_timeline_db(QStringList INT_list);
    void pub_remove_notes_in_artifact_timeline_db_from_outside(QList<struct_global_tagging_artifact_timeline_record>);
    void pub_add_notes_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record>, QString notes_value);
    void pub_delete_notes_for_artifact_timeline(QList<struct_global_tagging_artifact_timeline_record>);
    ///


    /// Redefined Results - Bookmarks
    void pub_set_bookmark_in_redefined_db(QStringList INT_list ,QString plugin_name);
    void pub_remove_bookmark_in_redefined_db(QStringList INT_list , QString plugin_name);
    void pub_add_bookmark_for_redefined_db(QList<struct_global_tagging_redefined_result_record>);
    void pub_delete_bookmark_for_redefined_db(QStringList INT_list ,QString plugin_name);
    ///


    /// Redefined Results - Tags
    void pub_set_tag_in_redefined_db(QStringList INT_list, QString plugin_name, QString tag_name);
    void pub_remove_tag_in_redefined_db(QStringList INT_list , QString plugin_name);
    void pub_add_tag_for_redefined(QList<struct_global_tagging_redefined_result_record> redefined_record_list, QString tag_name);
    void pub_delete_tag_for_redefined(QStringList INT_list ,QString plugin_name);
    ///


    /// Redefined Results - Notes
    void pub_set_notes_in_redefined_db(QStringList INT_list, QString plugin_name, QString notes_value);
    void pub_remove_notes_in_redefined_db(QStringList INT_list , QString plugin_name);
    void pub_add_notes_for_redefined_db(QList<struct_global_tagging_redefined_result_record> redefined_record_list, QString notes_value);
    void pub_delete_notes_for_redefined_db(QStringList INT_list ,QString plugin_name);
    ///

    /// Saved Graphs, Screenshot - Notes
    void pub_set_notes_in_saved_graphs_and_screenshot_db(QStringList INT_list, QString plugin_name, QString notes_value);
    void pub_remove_notes_in_saved_graphs_and_screenshot_db(QStringList INT_list , QString plugin_name);
    void pub_add_notes_for_saved_graphs_and_screenshot_db(QList<struct_global_tagging_saved_graph_record> saved_graphs_record_list, QString notes_value);
    void pub_delete_notes_for_saved_graphs_and_screenshot_db(QStringList INT_list ,QString plugin_name);

    /// String Viewer, Text Viewer - Tags
    void pub_add_data_in_export_content_string_viewer_and_text_viewer_db(QList<struct_global_tagging_string_viewer_record> string_viewer_record_list, QString tag_name, QString plugin_name);
    void pub_delete_tag_export_content_string_viewer_and_text_viewer_db(QStringList INT_list  , QString content_file_name, QString plugin_name);
    void pub_add_tag_for_string_viewer_and_text_viewer(QList<struct_global_tagging_string_viewer_record> string_viewer_record_list, QString tag_name);
    void pub_delete_tag_for_string_viewer_and_text_viewer(QStringList INT_list , QString content_file_name , QString plugin_name);
    ///

    /// Plist Viewer - Tags
    void pub_set_tag_in_plist_viewer_db(QStringList INT_list, QString table_name, QString tag_name);
    void pub_remove_tag_in_plist_viewer_db(QStringList INT_list , QString table_name);
    void pub_update_tag_export_content_plist_viewer_db(QStringList INT_list  , QString display_tab_name);
    void pub_add_tag_for_plist_viewer(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list, QString tag_name);
    void pub_delete_tag_for_plist_viewer(QStringList INT_list , QString display_tab_name);
    ///

    /// Plist Viewer - Export content db
    void pub_add_data_in_export_content_plist_viewer_db(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list, QString tag_name , QString notes_text);
    void pub_delete_entry_export_content_plist_viewer_db(QStringList INT_list  , QString display_tab_name);
    /// Plist Viewer - Export content db


    /// Plist Viewer - Bookmarks
    void pub_set_bookmark_in_plist_viewer_db(QStringList INT_list, QString table_name);
    void pub_remove_bookmark_in_plist_viewer_db(QStringList INT_list , QString table_name);
    void pub_update_bookmark_export_content_plist_viewer_db(QStringList INT_list  , QString display_tab_name);
    void pub_add_bookmark_for_plist_viewer(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list);
    void pub_delete_bookmark_for_plist_viewer(QStringList INT_list , QString display_tab_name);
    ///

    /// Plist Viewer - Notes
    void pub_set_notes_in_plist_viewer_db(QStringList INT_list, QString table_name, QString notes_value);
    void pub_remove_notes_in_plist_viewer_db(QStringList INT_list , QString table_name);
    void pub_update_notes_export_content_plist_viewer_db(QStringList INT_list  , QString display_tab_name, QString notes_value);
    void pub_add_notes_for_plist_viewer(QList<struct_global_tagging_plist_viewer_record> plist_viewer_record_list);
    void pub_delete_notes_for_plist_viewer(QStringList INT_list , QString display_tab_name);
    ///

    /// Plist Viewer - Notes from bookmark search and tag serach
    void pub_set_notes_in_tags_search_for_plist_viewer(QStringList INT_list, QString display_tab_name, QString notes_value);
    void pub_set_notes_in_bookmark_search_for_plist_viewer(QStringList INT_list, QString display_tab_name, QString notes_value);
    ///

    /// Log Viewer - Tags
    void pub_set_tag_in_log_viewer_db(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist, QString table_name, QString tag_name, QString colour_hex, QString destination_db_path);
    void pub_remove_tag_in_log_viewer_db(QStringList INT_list , QString table_name, QString tagged_data_str, QString current_tag);
    void pub_add_data_in_tags_db_for_log_viewer(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist, QString tag_name);
    ///

    /// Log Viewer - Export content db
    void pub_add_data_in_export_content_log_viewer_db(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist, QString tag_name, QString plugin_name);
    void pub_update_tag_export_content_log_viewer_db(QStringList INT_list, QString display_tab_name, QString tagged_data_str);
    ///

    /// Log Viewer - Notes
    void pub_update_notes_export_content_log_viewer_db(QStringList INT_list, QString display_tab_name, QString notes_value, QString tagged_data_str);
    void pub_add_notes_for_log_viewer(QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist);
    void pub_delete_notes_for_log_viewer(QStringList INT_list , QString display_tab_name, QString tagged_data_str);
    ///

    /// Log Viewer - Notes from tag serach
    void pub_set_notes_in_tags_search_for_log_viewer(QStringList INT_list, QString display_tab_name, QString notes_value, QString tagged_data);
    ///

    ///Carved Password  - Bookmarks
    void pub_set_bookmark_in_carved_password_db(QStringList INT_list, QString tab_label_name);
    void pub_remove_bookmark_in_carved_password_db(QStringList INT_list , QString tab_label_name);
    void pub_add_bookmark_for_carved_password_db(QList<struct_global_tagging_carved_passwords> carved_passwords_record_list);
    void pub_delete_bookmark_for_carved_password_db(QStringList INT_list, QString tab_label_name);
    ///


    /// Carved Password - Tags
    void pub_set_tag_in_carved_password_db(QStringList INT_list, QString tab_label_name, QString tag_name);
    void pub_remove_tag_in_carved_password_db(QStringList INT_list , QString tab_label_name);
    void pub_add_tag_for_carved_password(QList<struct_global_tagging_carved_passwords> redefined_record_list, QString tag_name);
    void pub_delete_tag_for_carved_password(QStringList INT_list ,QString tab_label_name);
    ///

    /// Carved Password - Notes
    void pub_set_notes_in_carved_password_db(QStringList INT_list, QString tab_label_name, QString notes_value);
    void pub_remove_notes_in_carved_password_db(QStringList INT_list , QString tab_label_name);
    void pub_add_notes_for_carved_password_db(QList<struct_global_tagging_carved_passwords> carved_password_record_list, QString notes_value);
    void pub_delete_notes_for_carved_password_db(QStringList INT_list ,QString tab_label_name);
    ///

    ///Carved Files  - Bookmarks
    void pub_set_bookmark_in_carved_files_db(QStringList INT_list, QString tab_label_name);
    void pub_remove_bookmark_in_carved_files_db(QStringList INT_list , QString tab_label_name);
    void pub_add_bookmark_for_carved_files_db(QList<struct_global_tagging_carved_files> carved_files_record_list);
    void pub_delete_bookmark_for_carved_files_db(QStringList INT_list, QString tab_label_name);
    ///


    /// Carved Files - Tags
    void pub_set_tag_in_carved_files_db(QStringList INT_list, QString tab_label_name, QString tag_name);
    void pub_remove_tag_in_carved_files_db(QStringList INT_list , QString tab_label_name);
    void pub_add_tag_for_carved_files(QList<struct_global_tagging_carved_files> carved_files_record_list, QString tag_name);
    void pub_delete_tag_for_carved_files(QStringList INT_list ,QString tab_label_name);
    ///

    /// Carved Files - Notes
    void pub_set_notes_in_carved_files_db(QStringList INT_list, QString tab_label_name, QString notes_value);
    void pub_remove_notes_in_carved_files_db(QStringList INT_list , QString tab_label_name);
    void pub_add_notes_for_carved_files_db(QList<struct_global_tagging_carved_files> carved_files_record_list, QString notes_value);
    void pub_delete_notes_for_carved_files_db(QStringList INT_list ,QString tab_label_name);
    ///

    ///Registry Viewer - Bookmarks
    void pub_set_bookmark_in_registry_viewer_db(QStringList INT_list, QString tab_label_name);
    void pub_remove_bookmark_in_registry_viewer_db(QStringList INT_list , QString tab_label_name);
    void pub_add_bookmark_for_registry_viewer_db(QList<struct_global_tagging_registry_viewer> registry_viewer_record_list);
    void pub_delete_bookmark_for_registry_viewer_db(QStringList INT_list, QString tab_label_name);
    ///


    /// Registry Viewer - Tags
    void pub_set_tag_in_registry_viewer_db(QStringList INT_list, QString tab_label_name, QString tag_name);
    void pub_remove_tag_in_registry_viewer_db(QStringList INT_list , QString tab_label_name);
    void pub_add_tag_for_registry_viewer(QList<struct_global_tagging_registry_viewer> registry_viewer_record_list, QString tag_name);
    void pub_delete_tag_for_registry_viewer(QStringList INT_list ,QString tab_label_name);
    ///

    /// Registry Viewer - Notes
    void pub_set_notes_in_registry_viewer_db(QStringList INT_list, QString tab_label_name, QString notes_value);
    void pub_remove_notes_in_registry_viewer_db(QStringList INT_list , QString tab_label_name);
    void pub_add_notes_for_registry_viewer_db(QList<struct_global_tagging_registry_viewer> registry_viewer_record_list, QString notes_value);
    void pub_delete_notes_for_registry_viewer_db(QStringList INT_list ,QString tab_label_name);
    ///


    ///Mobile Backup  - Bookmarks
    void pub_set_bookmark_in_Mobile_Backup_db(QStringList INT_list);
    void pub_remove_bookmark_in_Mobile_Backup_db(QStringList INT_list);
    void pub_add_bookmark_for_Mobile_Backup_db(QList<struct_global_tagging_mobile_backup> mobile_backup_record_list);
    void pub_delete_bookmark_for_Mobile_Backup_db(QStringList INT_list);
    ///


    /// Mobile Backup - Tags
    void pub_set_tag_in_Mobile_Backup_db(QStringList INT_list, QString tag_name);
    void pub_remove_tag_in_Mobile_Backup_db(QStringList INT_list);
    void pub_add_tag_for_Mobile_Backup(QList<struct_global_tagging_mobile_backup> mobile_backup_record_list, QString tag_name);
    void pub_delete_tag_for_Mobile_Backup(QStringList INT_list);
    ///

    /// Mobile Backup - Notes
    void pub_set_notes_in_Mobile_Backup_db(QStringList INT_list, QString notes_value);
    void pub_remove_notes_in_Mobile_Backup_db(QStringList INT_list);
    void pub_add_notes_for_Mobile_Backup_db(QList<struct_global_tagging_mobile_backup> mobile_backup_record_list, QString notes_value);
    void pub_delete_notes_for_Mobile_Backup_db(QStringList INT_list);
    ///

    /// Retrieved Sources - Bookmarks
    void pub_set_bookmark_in_disk_images_db(QStringList INT_list);
    void pub_add_bookmark_for_disk_images_db(QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list);
    void pub_delete_bookmark_for_disk_images_db(QStringList INT_list);
    void pub_remove_bookmark_in_disk_images_db(QStringList INT_list);

    /// Retrieved Sources - Notes
    void pub_remove_notes_in_disk_images_db(QStringList INT_list);
    void pub_delete_notes_for_disk_images_db(QStringList INT_list);
    void pub_set_notes_in_disk_images_db(QStringList INT_list, QString notes_value);
    void pub_add_notes_for_disk_images_db(QList<struct_global_tagging_disk_images> rtrvd_srcs_record_list, QString notes_value);

    /// Retrieved Sources - Tags
    void pub_set_tag_in_disk_images_db(QStringList INT_list, QString tag_name);
    void pub_add_tag_for_disk_images(QList<struct_global_tagging_disk_images> mobile_backup_record_list, QString tag_name);
    void pub_delete_tag_for_disk_images(QStringList INT_list);
    void pub_remove_tag_in_disk_images_db(QStringList INT_list);


    /// SQilte Viewer - Notes
    void pub_set_notes_in_sqlite_viewer_db(QStringList INT_list, QString tab_label_name, QString table_name, QString notes_value);
    void pub_remove_notes_in_sqlite_viewer_db(QStringList INT_list ,QString tab_label_name ,QString table_name , QString selected_sql_tab_name);
    void pub_add_notes_for_sqlite_viewer_db(QList<struct_global_tagging_sqlite_viewer> sqlite_viewer_record_list, QString notes_value);
    void pub_delete_notes_for_sqlite_viewer_db(QStringList INT_list ,QString tab_label_name ,QString table_name, QString selected_sql_tab_name);
    ///


    /// SQilte Viewer - Export content db
    void pub_delete_or_update_record_export_content_sqlite_viewer_db(QStringList INT_List  , QString table_name , QString file_path, QString tab_name_str, QString notes_value);
    ///

    /// SQilte Viewer - Notes from bookmark search and tag serach
    void pub_set_notes_in_tags_search_for_sqlite_viewer(QStringList INT_list, QString selected_sql_tab_name, QString table_name ,QString tab_label_name , QString notes_value);
    void pub_set_notes_in_bookmark_search_for_sqlite_viewer(QStringList INT_list, QString selected_sql_tab_name, QString table_name ,QString tab_label_name , QString notes_value);
    ///


    ///Carved Data  - Bookmarks
    void pub_set_bookmark_in_carved_data_db(QStringList INT_list, QString table_name);
    void pub_remove_bookmark_in_carved_data_db(QStringList INT_list , QString table_name);
    void pub_update_bookmark_export_content_carved_data_db(QStringList INT_list  , QString category_name , QStringList source_count_namelist, QString bookmark_value);
    void pub_add_bookmark_for_carved_data_db(QList<struct_global_tagging_carved_data> carved_data_record_list);
    void pub_delete_bookmark_for_carved_data_db(QStringList INT_list, QString category_name, QStringList source_count_name_list);
    ///


    /// Carved Data - Export content db
    void pub_add_data_in_export_content_carved_data_db(QList<struct_global_tagging_carved_data> carved_data_record_list, QString tag_name );
    void pub_delete_entry_export_content_carved_data_db(QStringList INT_list  , QString category_name, QStringList source_count_namelist, QStringList is_tagded_record);
    /// Carved Data - Export content db


    /// Carved Data - Notes
    void pub_set_notes_in_carved_data_db(QStringList INT_list, QString table_name , QString notes_value);
    void pub_remove_notes_in_carved_data_db(QStringList INT_list , QString table_name);
    void pub_update_notes_export_content_carved_data_db(QStringList INT_list  , QString category_name , QStringList source_count_namelist, QString notes_value);
    void pub_add_notes_for_carved_data_db(QList<struct_global_tagging_carved_data> carved_data_record_list);
    void pub_delete_notes_for_carved_data_db(QStringList INT_list, QString category_name, QStringList source_count_name_list);
    ///


    /// Carved Data - Tags
    void pub_set_tag_in_carved_data_db(QStringList INT_list, QString table_name, QString tag_name);
    void pub_remove_tag_in_carved_data_db(QStringList INT_list , QString table_name);
    void pub_update_tag_export_content_carved_data_db(QStringList INT_list  , QString category_name , QStringList source_count_namelist, QString tag_name);
    void pub_add_tag_for_carved_data(QList<struct_global_tagging_carved_data> carved_data_record_list, QString tag_name);
    void pub_delete_tag_for_carved_data(QStringList INT_list ,QString category_name, QStringList source_count_name_list);
    ///

    /// Hex Viewer - Notes
    void pub_delete_notes_for_hex_viewer_db(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation);
    void pub_set_notes_in_tags_search_for_hex_viewer(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation);
    ///

    /// Hex Viewer - Tags
    void pub_add_tags_for_hex_viewer(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation, QString tag_name);
    void pub_delete_tags_for_hex_viewer(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation, QString tag_name);
    ///


    /// Snapshots - Bookmarks
    void pub_set_bookmark_in_snapshots_difference_db(QStringList INT_list,QString snapshots_label_name, QString table_name);
    void pub_remove_bookmark_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name , QString table_name);
    void pub_add_bookmark_for_snapshots_difference_system(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation);
    void pub_delete_bookmark_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation);
    ///

    /// Snapshots - Tags
    void pub_set_tag_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name, QString table_name, QString tag_name);
    void pub_remove_tag_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name , QString table_name);
    void pub_add_tag_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tag_db_updation, QString tag_name);
    void pub_delete_tag_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tag_db_updation);
    ///

    /// Snapshots - Notes
    void pub_set_notes_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name ,QString table_name, QString notes_value);
    void pub_remove_notes_in_snapshots_difference_db(QStringList INT_list, QString snapshots_label_name ,QString table_name);
    void pub_add_notes_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_notes_db_updation, QString notes_text);
    void pub_delete_notes_for_snapshots_difference(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_notes_db_updation);
    ///



    /// Carved File - Content Search
    void pub_set_bookmark_in_carved_index_files_db(QStringList INT_list, QString tab_label_name);
    void pub_remove_bookmark_in_carved_index_files_db(QStringList INT_list, QString tab_label_name);
    void pub_set_notes_in_carved_index_files_db(QStringList INT_list, QString tab_label_name, QString notes_value);
    void pub_remove_notes_in_carved_index_files_db(QStringList INT_list, QString tab_label_name);
    void pub_set_tags_in_carved_index_files_db(QStringList INT_list, QString tab_label_name, QString tag_name);
    void pub_remove_tag_in_carved_index_files_db(QStringList INT_list, QString tab_label_name);
    ///


    void pub_set_files_list_having_same_hash(QString source_name, QString source, QString file_path, QString record_number, QString fs_db_path, QString os_scheme);

    /// EMLX Tag
    void pub_add_tag_for_emlx_viewer_raw_data(QList<struct_global_tagging_string_viewer_record> string_viewer_record_list, QString tag_name);
    void pub_delete_tag_export_content_emlx_raw_tag_db(QStringList INT_list, QString content_file_name);
    void pub_delete_tag_for_emlx_raw_tag(QStringList INT_list, QString content_file_name, QString plugin_name);
    ///

    ///Unified Logs
    void pub_add_data_in_tags_db_for_unified_logs(QList<struct_global_tagging_unified_logs_record> log_viewer_record_qlist, QString tag_name);
    void pub_set_tag_in_unified_logs_db(QStringList INT_list, QString table_name, QString destination_db_path);
    void pub_remove_data_in_tags_db_for_unified_logs(QList<struct_global_tagging_unified_logs_record> log_viewer_record_qlist, QString tag_name);
    void pub_update_tag_unified_logs_db(QStringList INT_list, QString source_count_name);


signals:
    void signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString);
    void signal_common_add_tag_name_into_case_tree(QString tag_name);
    void signal_common_add_notes_name_into_case_tree(QString note_name);
    void signal_go_to_source_for_search_file_with_same_hash(QString source_name, QString parent_name, QString file_path, QString record_no, QString db_path, QString os_naming_scheme);


public slots:

private:

    void delete_file_entry_from_all_tag_DBs(QList<struct_global_tagging_tag_notes_db_updation> obj);
    void delete_file_entry_from_all_notes_DBs(QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation);

    recon_helper_standard *recon_helper_standard_obj;

    void delete_file_entry_from_all_tag_DBs_for_unified_logs(QList<struct_global_tagging_unified_logs_record> list_st_global_bookmark_db_updation);

};

#endif // GLOBAL_CONNECTION_MANAGER_H
