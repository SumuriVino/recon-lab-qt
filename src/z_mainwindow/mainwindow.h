#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QAction>
#include <QtCore>
#include <QHBoxLayout>
#include <QSpacerItem>

#include "ui_custom_classes/r_tabwidget.h"
#include "ui_custom_classes/r_treewidget.h"
#include "ui_custom_classes/m_textedit.h"
#include "ui_custom_classes/m_stackedwidget.h"
#include "ui_custom_classes/r_dockwidget.h"
#include "ui_custom_classes/flatcombo.h"


#include "tsk/libtsk.h"
#include "file_viewer/plist_viewer/plist_viewer_interface.h"


#include "recon_case/recon_case.h"

#include "header_plugin_extraction_interface.h"

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "task_status/task_status_window.h"
#include "task_status/task_progress_bar.h"

#include "case_info/case_info_details.h"
#include "image_mounter/image_mounter.h"
#include "load_case.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"

#include "header_global_variables_3.h"
#include "header_identical_1.h"

#include <QHostAddress>
#include "licensing/usb_device_serial_list.h"
#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "launcher/case_launcher/case_wizard.h"
#include "container_qwidget.h"
#include "center_stackwidget_manager.h"
#include "file_system_viewer/file_system.h"
#include "tags/tag_search/tag_search_loader.h"
#include "center_next_previous_navigation.h"
#include "artifacts_keyword_search/k_search/artifacts_keyword_search_loader.h"
#include "artifacts_timeline/artifacts_timeline_full/artifacts_timeline_full_loader.h"
#include "artifacts_timeline/artifacts_timeline_saved/artifacts_timeline_saved_loader.h"
#include "filters/redefined_result_filters/redefined_result_filters_interface.h"
#include "ios_backup/uid_backup/uid_backup_parser/ios_uid_backup_parser.h"
#include "launcher/source_viewer/add_new_source.h"
#include "quick_look/quick_look.h"
#include "hashset/hashset_storage.h"
#include "tags/notes_search/notes_search_loader.h"
#include "tags/bookmarks_search/bookmarks_search_loader.h"
#include "redefined_result/redefined_result_full/redefined_result_full_loader.h"
#include "redefined_result/redefined_result_saved/redefined_result_saved_loader.h"
#include "detach_detailed_information/detach_manager.h"
#include "file_types/filetypes_loader.h"
#include "progress_bar/progress_bar_fixed_in_statusbar.h"
#include "launcher/source_viewer/load_case_source_displayer.h"
#include "screenshots/screenshots.h"
#include "case_info/case_info_display/case_info_display.h"

#include "activity_logs/activity_logs_storage.h"
#include "activity_logs/activity_logs_display.h"

#include "file_system_extraction/dt_search/dt_search_interface.h"
#include "filters/keyword_search_filters/keywords_list_selector.h"
#include "file_search/file_search_interface.h"
#include "screenshots/screenshots_loader.h"
#include "story_board/story_board.h"
#include "plugin_viewer/plugin_viewer.h"

#include "content_search/content_search.h"
#include "file_system_post_launcher/file_system_post_launcher.h"

#include "recon_configuration/recon_configuration.h"

#include "recon_configuration/pages_recon_configuration/recon_configuration_page_apple_metadata.h"

#include "RAM_Analysis/ram_analysis.h"
#include "system_password_asker/system_password_asker.h"
#include "RAM_Analysis/ram_analysis_saved/ram_analysis_saved_loader.h"
#include "saved_graphs_dialog_box/saved_graphs_dialog_box.h"
#include "saved_graphs_dialog_box/saved_graph_loader.h"
#include "Email_Files/emlx_mail/emlx_mail.h"
#include "maps/offline_maps/offline_maps.h"
#include "file_viewer/strings_viewer/strings_viewer.h"
#include "licensing/license_locator.h"

#include "file_indexing/file_indexing_display.h"
#include "thread_recon_lab_initial_settings.h"
#include "file_viewer/text_viewer/text_viewer_aascii_unicode.h"

#include "initial_warning_message.h"

#include "licensing/network_time.h"
#include "global_connection_manager.h"
#include "super_timeline/super_timeline.h"

#include "unistd.h"
#include "file_search/regular_filter/file_search_regular_filters.h"
#include "file_search/regular_filter/thread_file_search_regular_filters.h"
#include "carving/carved_files/carved_file_display/carved_files_loader.h"
#include "carving/carved_passwords/carved_passwords_loader.h"
#include "file_viewer/registry_viewer/registry_interface.h"
#include "mobile_backup/mobile_backup_loader.h"

#include "carving/carved_files/thread_carve_files.h"
#include "disk_images/disk_images_loader.h"
#include "carving/carved_data/thread_carve_data.h"
#include "carving/carved_data/carved_data_display/carved_data_loader.h"
#include "ram_images/ram_images_loader.h"
#include "snapshots/snapshots.h"
#include "snapshots/snapshots_loader/snapshots_loader.h"
#include "rcbuild/build_items_common_for_recon_and_fsfs.h"
#include "rcbuild/build_items_common_for_recon_and_apple_metadata_reader.h"
#include "global_recon_file_info/global_recon_file_info.h"
#include "recon_generic_functions/recon_helper_process.h"    
#include "file_viewer/hex_viewer/hex_viewer_unallocate/hex_viewer_unallocated.h"
#include "file_viewer/hex_viewer/hex_viewer_unallocate/thread_hex_view_unallocated.h"
#include "hashset/hashset_display_toolbar/hashset_display_toolbar.h"
#include "tagged_file_export/tagged_file_export.h"
#include "file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_complete.h"
#include "disk_manager/Disk_Manager/disk_manager.h"
#include "file_search/exif_metadata_search/thread_exif_metadata_search.h"
#include "file_search/exif_metadata_search/search_filters/exif_metadata_filters.h"
#include "witness_info/witness_info_store/witness_info_store.h"
#include "uncloseable.h"
#include "export/export_case/export_case.h"
#include "ios_backup/xml_backup/xml_backup_parser/ios_xml_backup_parser.h"

#include "face_extractor/facefinder.h"
#include "face_extractor/recognize_face.h"
#include "examiner_space/examiner_space.h"
#include "header_global_variables_4.h"
#include "file_viewer/log_viewer/log_viewer_interface.h"
#include "unified_logs/unified_logs_model.h"
#include "unified_logs/unified_logs_viewer.h"

namespace Ui {

class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

    void show_launcher();

signals:
    void signal_show_r_case_triggered();
    void signal_load_case_clicked();
    void signal_load_case_from_launcher_clicked(QString);


    void signal_goto_plugin_record(QString tab_name,QString record);
    void signal_goto_email_files_record(QString record);
    void signal_goto_screenshot_record(QString record);
    void signal_goto_saved_graphs_record(QString record);
    void signal_goto_carved_files_record(QString record);

    void signal_goto_carved_password_record(QString record);

    void signal_hex_viewer_db_path_changed(QString hex_viewer_db_path);

    void signal_update_tags_search_display_for_singal_record(QString record_no,QString plugin_name,QString tab_name,QString tag_name, QString os_scheme);

    void signal_plist_viewer_db_path_changed(QString);
    void signal_log_viewer_db_path_changed(QString);

    void signal_insert_new_entry_in_navigation_list(QString current_page_unique_name);
    void signal_update_navigation_list_on_combobox_change(QString page_name);
    void signal_remove_navigation_list_entry_on_combobox_pusbutton_close(QString page_name);
    void signal_registry_viewer_db_path_changed(QString destination_db_path);



    void signal_update_tags_submenu_of_tablewidget_loader(bool status,QString new_tag_name,QString colour_name);
    void signal_update_tags_submenu_of_plugin(bool status,QString new_tag_name,QString colour_name);

    void signal_goto_file_system_record(QString file_path,QString record, QString source_count_name);
    void signal_decompressed_files_view_file_system_record(QString dir_path);
    void signal_update_tags_submenu_of_file_system(bool status,QString new_tag_name,QString colour_name);

    void signal_statusbar_button_checked(bool checked);

    void signal_update_hashset_submenu_of_file_system(QString db_name, QString db_path);
    void signal_update_hashset_submenu_of_tablewidget_loader(QString db_name, QString db_path);

    void signal_update_hashset_submenu_of_plugin(QString db_name, QString db_path);

    void signal_statusbar_progress_bar_show_hide(bool status, QString msg);
    void signal_fs_double_clicked_dir_extraction_completed(QString dir_path);
    void signal_update_plugin_display_on_remove_notes(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);
    void signal_set_center_next_previous_navigation_list_page_unique_name(QString previous, QString current);

    void signal_lic_label_text(QString name, qint64 pd, qint64 ed);
    void signal_refresh_storyboard_display();

    void signal_goto_redefined_result(QString);
    void signal_carved_files_separate_process_started(QString job_type);
    void signal_carved_files_separate_process_PBAR_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_carved_files_separate_process_PBAR_finished(QString job_type);

    void signal_PBAR_thread_carve_data_carving_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_carved_data_cancelled(QString job_name);
    void signal_goto_carved_data(QString record);

    void signal_r_case_apple_metadata_running_status(bool status);
    void signal_r_case_exif_metadata_running_status(bool status);
    void signal_r_case_signature_analysis_running_status(bool status);
    void signal_r_case_mime_type_analysis_running_status(bool status);
    void signal_r_case_hashset_running_status(bool status);
    void signal_goto_record_snapshots(QString record , QString tab_name);
    void signal_r_case_face_analysis_running_status(bool status);
    void signal_r_case_optical_character_recognition_running_status(bool status);

    void signal_r_case_weapons_running_status(bool status);
    void signal_r_case_fire_analysis_running_status(bool status);
    void signal_r_case_skin_tone_detection_running_status(bool status);

protected:
    void resizeEvent(QResizeEvent* event);


private slots:

    void slot_action_new_case_triggered();
    void slot_action_load_case_triggered();
    void slot_action_quick_look_triggered();
    void slot_action_add_source_triggered();
    void slot_action_run_plugin_triggered();
    void slot_action_keyword_search_triggered();
    void slot_action_artifacts_timeline_full_triggered();
    void slot_action_global_report_triggered();

    void slot_loadcase_launched(QString path);
    void slot_new_case_wizard_finishes(int);
    void slot_treewidget_case_display_double_clicked(QTreeWidgetItem *item, int coloum);
    void slot_metadata_clicked(QString metadata,QString searched_keyword);
    void slot_refresh_widgets();

    void slot_pushbutton_status_right_corner_clicked(bool checked);

    void slot_current_plugin_extraction_finished_or_cancelled(QString plugin_name);

    void slot_tree_right_click_close_case();
    void slot_right_click_keyword_search_finished_or_cancelled(QString search_tag);
    void slot_right_click_artifacts_timeline_full(QString timeline_name);
    void slot_right_click_global_report_finished_or_cancelled(QStringList report_paths);
    void slot_show_progressbar(bool status);
    void slot_keyword_search_extraction_start(QString name);
    void slot_global_report_extraction_start(QString name);
    void slot_global_item_clicked(QString searched_text, QString, QString, QString source_count_name);
    void slot_treewidget_case_display_single_clicked(QTreeWidgetItem *item, int coloum);
    void slot_select_mac_version(QString ver);
    void slot_generic_process_finished(int);
    void slot_selected_users_list(QStringList list);
    void slot_viewer_extraction_finished_or_cancelled(QString result_name, QString viewer_name);


    ///hex viewer
    void slot_change_hex_viewer_content_in_metadata_on_row_click(QString file_path, QString source_count_name);

    void slot_case_launched(QString case_nm, QList<struct_GLOBAL_witness_info_root> list_root_info);
    void slot_launcher_load_Case_clicked(QString path);

    void on_actionShow_Sidebar_triggered();

    void on_actionHide_Sidebar_triggered();

    void on_actionShow_Detailed_Information_triggered();

    void on_actionHide_Detailed_Information_triggered();

    void slot_show_file_in_viewer(QString tab_text, QString viewer_name, QString source_count_name);


    void slot_pushButton_combobox_feature_close_clicked();
    void slot_pushButton_combobox_category_close_clicked();
    void slot_pushButton_combobox_case_close_clicked();
    void slot_combobox_feature_current_text_changed(QString arg1);
    void slot_combobox_category_current_text_changed(QString text);
    void slot_combobox_case_current_text_changed(QString text);
    void slot_combobox_case_tree_display_changed(QString text);

    void slot_show_metadata(bool status);

    void on_actionSend_To_Bucket_triggered();

    void slot_hashset_finished_or_cancelled_for_rcase(QStringList default_hashset_list, QString source_count);

    void slot_openwith_after_sendto_bucket_master_interface(QString tab_text, QString viewer_name);

    void slot_change_metadata_on_click(QString data, QString current_media, QString searched_text, QString source_count_name);

    void slot_pushButton_topbar_next_clicked();
    void slot_pushButton_topbar_previous_clicked();
    void slot_update_bookmark_value_of_other_display_and_db(QString record_no, QString plugin_name, QString tab_name, QString status, QStringList applied_tags_list, QString fs_db_path, QString os_name_str, QString feature_where_from_called);
    void slot_tablewidget_goto_record(QString plugin_name, QString parent_name, QString tab_name, QString record, QString db_path, QString os_name_str);
    void slot_update_notes_value_of_other_display_and_db(QString record_no, QString plugin_name, QString tab_name, QString status, QStringList applied_tags_list, QString os_name_str, QString feature_where_from_called);
    void slot_open_window_for_detach_clicked(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index, QString searched_keyword, QString source_count_name);
    void slot_change_all_detach_window_content(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index, QString searched_keyword, QString source_count_name);
    void slot_add_tag_in_other_db(QString tag_name, QString is_emitted_from_plugin, QString plugin_name, QString tab_name, QString record_no, QString db_path, QString os_name_str);
    void slot_remove_tag_entry_in_other_db(QString plugin_name, QString tab_name, QString record, QStringList list_tags_removed, QString os_scheme);
    void slot_update_tag_submenu_of_tablewidget_loader_and_file_system(bool status, QString new_tag, QString color_name);
    void slot_update_tag_submenu_of_plugin_and_file_system(bool status, QString new_tag, QString colour_name);
    void on_actionRedefined_Result_triggered();
    void slot_update_bookmark_value_from_sqlite_viewer(QString record_no, QString plugin_name, QString sql_tab_name, QString status, QString tag_name, QString display_col_values, QString file_path, QString os_name_str, QString sqliteviewer_db_table_name_of_tag , QString current_table_name , QString saved_state_db_path);

    void slot_update_other_display_and_db_for_remained_tags(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme);
    void slot_update_status_bar_message(QString message);

    void slot_update_tag_submenu_of_plugin_and_tablewidget_loader(bool status, QString new_tag, QString colour_name);
    void slot_update_hashset_menu_in_other_submenu_of_hashset_from_plugin(QString db_name, QString db_path);
    void slot_update_hashset_submenu_from_tablewidget_loader(QString db_name, QString db_path);



    void slot_update_hashset_submenu_from_file_system(QString db_name, QString db_path);
    void slot_extracted_records_by_plugin(qint64 records);
    void slot_go_to_artifact_source(QString artifact_source_path, QString fs_db_path, QString source_count_name);
    void slot_tree_right_click_add_source();

    void slot_toolbar_add_source(QList<struct_GLOBAL_witness_info_root> list_root);


    void slot_quick_look_preview_item(QString source_path);

    void on_actionPlugin_Summary_triggered();
    void slot_right_click_remove_selected_item();
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
    void slot_aboutToShow_menubar_view();

    void slot_update_tags_search_display_for_singal_record(QString record_str, QString plugin_name, QString tab_name, QString tag_name, QString os_scheme);

    void slot_add_notes_added_in_database_for_plugin(QString bookmark, QString category, QString record_no, QString plugin_name, QString tab_name, QString table_name, QString note_data, QString resut_note_db_file, QString os_name);
    void slot_add_notes_for_file_system(QString record_no_prepended_zero, QString tab_name, QString tags_db_path, QString notes_text, QString fs_db_path, QString os_scheme);
    void slot_insert_tag_detail_into_tag_search_db_from_hex_viewer(QString file_path, QString tag_name, uint64_t start_index, uint64_t end_index, QString record_no, QString hex_viewer_display_name, QString source_count_name, QString selected_data_str);
    void slot_add_notes_added_in_database_for_hex_viewer(QString record_no, QString tab_name, QString os_name, QString start_offset, QString end_offset, QString file_path, QString clicked_tag_name, QString notes_db_path);

    void slot_remove_tag_entry_in_notes_search_db_and_display(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);

    void slot_redefined_result_finished_or_cancelled(QString category_name);
    void slot_update_other_display_and_db_for_remained_tags_in_plist_viewer(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme);

    void slot_add_notes_added_in_database_for_plist_viewer(QString record_no, QString tab_name, QString os_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString viewer_display_name, QString note_text);
    void slot_insert_tag_detail_into_tag_search_db_from_sqlite_viewer(QString file_path, QString tag_name,  QString record_no, QString sqlite_viewer_display_name, QString source_count_name, QString sqliteviewer_db_table_name_of_tag, QString executed_query, QString sql_tab_name);

    void slot_add_notes_added_in_database_for_sqlite_viewer(QString record_no, QString tab_name, QString os_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString note_text);

    void slot_save_openwith_changes_to_bucket(QString viewer_type);

    void slot_change_display_message_of_task_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress);

    void slot_progress_count_for_progress_bar(qint64 total_record_count, qint64 record_displayed, float progress_percent);

    void slot_load_case_set_sources_path(QList<struct_GLOBAL_witness_info_source> list_source_info_to_be_mount, bool bool_want_to_mount_root_source);
    void slot_statusbar_progressbar_show_hide(bool value, QString message);

    void slot_prc_generic_without_argument_finished(int);
    void slot_action_examiner_space_triggered();
    void slot_action_screenshot_triggered();
    void slot_screenshots_canceled();
    void slot_change_apple_metadata_content_on_row_click(QStringList file_path_list, QString source_count_name, QString record_no, QString table_name, QString plugin_name);
    void slot_tree_apple_metadata_item_clicked(QTreeWidgetItem *item, int m_column);
    void slot_file_search_finished_or_cancelled(QString search_label);
    void slot_set_progressbar_value(int tabs, QString job_title);
    void slot_set_case_tree_item_bold_on_navigation_click(QString text);

    void slot_action_activity_logs_triggered();

    void slot_action_content_search_triggered();
    void slot_content_search_done_clicked(QString search_label);
    void slot_content_search_extraction_start(QString name);
    void slot_content_search_extraction_finished_or_cancelled(QString search_label);
    void slot_add_notes_added_in_database_for_content_search(QString record_no, QString tab_name, QString os_name, QString file_path, QString notes_db_path, QString source_db_path);
    void slot_change_notes_value_in_index_k_search_db_and_display(QString record_no, QString plugin_name_str, QString tab_name_str, QString text, QStringList applied_tags_list, QString keyword_db_path, QString os_scheme, QString feature_name);
    void slot_change_bookmark_value_in_index_k_search_db_and_display(QString record_no, QString plugin_name_str, QString tab_name_str, QString status, QStringList applied_tags_list, QString keyword_db_path, QString os_scheme, QString feature_name);
    void slot_change_tag_value_in_index_k_search_db_and_display(QString record_no, QString plugin_name_str, QString tab_name_str, QString applied_tag_name, QString keyword_db_path, QString os_scheme, QString feature_name);
    void slot_index_search_on_file_system_current_dir(QStringList keyword_list, bool index_search_in_temp_mode, QString dir_path_to_iterate, QString source_count_name);

    void slot_action_file_search_regular_filters_triggered();
    void slot_start_file_search_thread(struct_global_selected_regular_filters_info struct_search_filters_info, QString mdfind_condition, QString search_label_name, bool bool_run_in_full_mode, QString dir_path_to_search, QString fs_source_count_name);
    void slot_screenshots_work_complete(QString filepath, QString notes, QString tags);
    void slot_add_notes_added_in_database_for_screenshot(QString record_no, QString tab_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString note_text);
    void slot_action_story_board_triggered();
    void slot_action_file_system_post_launcher_triggered();
    void slot_action_recon_configuration_after_launcher_triggered();

    void slot_file_search_regular_filter_work_done(QString cmd,QStringList sources,QString label);
    void slot_change_exif_data_content_on_detatch_click(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index, QString searched_keyword, QString source_count_name);
    void slot_remove_entry_file_type_based_in_stack_and_refresh_widget(QString parent_name, QString child_name);


    void slot_update_sqlite_db_for_remained_tags(QString plugin_name, QString table_name, QString viewer_name, QString record_no, QStringList list_remained_tags, QString os_scheme);
    void slot_update_hex_and_text_db_for_removed_tags(QString plugin_name, QString start_index, QString end_index, QString viewer_name, QStringList list_remained_tags, QString os_scheme);
    void slot_remove_category_and_feature_box_for_run_plugin();
    void slot_remove_plugin_entry_from_display_tree(QStringList plugin_list);
    void slot_storyboard_save_clicked();
    void slot_storyboard_cancel_clicked();
    void slot_lineedit_filename_storyboard_text_changed(QString text_str);

    void slot_save_tabs_in_artifacts_timleine_saved_case_tree(QString tab_name);
    void slot_treewidget_case_display_expanded(QTreeWidgetItem *item);
    void slot_save_tabs_in_redefined_result_saved_case_tree(QString tab_name , QString plugin_name);

    void slot_action_file_apple_metadata_filters_triggered();

    void slot_file_system_post_launcher_job_list(struct_global_file_system_post_launcher_job_info obj);

    void slot_disable_case_tree_sidebar(bool status);

    void slot_action_show_case_tree_sidebar();

    void slot_action_show_detailed_information();


    void slot_lineedit_case_tree_search_text_changed(QString text);
    void slot_update_bookmark_value_in_tag_search_for_filetypes(QStringList record_no_list, QString source_count_name, QString bookmark_status, QString plugin_name_str);

    void slot_common_add_tag_name_into_case_tree(QString tag);
    void slot_action_ram_analysis_triggered();
    void slot_system_password_asker_workdone(bool pwd_status, QString received_pwd);
    void slot_ram_analysis_saved_in_case_tree(QString plugin_name);

    void slot_saved_graph_category_name(QString category_name);
    void slot_show_location_in_offline_maps(QString latitude, QString longitude, QString filepath, QString source_count_name);
    void slot_insert_tag_detail_into_tag_search_db_from_text_viewer(QString file_path, QString tag_name, uint64_t start_index, uint64_t end_index, QString record_no, QString hex_viewer_display_name, QString source_count_name);
    void slot_action_text_indexing_triggered();
    void slot_text_indexing_done_clicked();
    void slot_quick_look_preview_item_email_files(QString source_path);

    void slot_tabwidget_metadata_viewer_tab_changed(int index);
    void slot_update_case_tab_record_number(QString plugin_name);
    void slot_add_notes_into_case_tree(QString notes_str);
    void slot_right_click_add_to_text_indexing(QString item_text, QString source_count_name);
    void slot_right_click_remove_story(QString item_text);
    void slot_add_saved_timeline_graph_in_case_tree();
    void slot_action_super_timeline_triggered();
    void slot_add_saved_maps_in_case_tree();
    void slot_hide_loading_display_dialog_box(bool status);
    void slot_super_timeline_done(bool status);

    void slot_common_add_notes_name_into_case_tree(QString note_name);
    void slot_tree_exif_metadata_item_clicked(QTreeWidgetItem *item, int m_column);
    void slot_clear_all_metadata();
    void slot_set_metadata_info(struct_global_set_metadata_info m_obj);
    void slot_thread_file_search_regular_filter_finish();
    void slot_hide_loading_file_search_regular_filters_display(bool status);
    void slot_hashset_finished_or_cancelled(QStringList default_hashset_list);
    void slot_add_carver_in_case_tree_on_finish();
    void slot_ram_analysis_carved_password_in_case_tree(QString tab_name);
    void slot_google_mail_files_case_tree_add_finish_on_load_case();
    void slot_email_files_case_tree_add_finish_on_load_case();
    void slot_filesystem_extraction_finish_on_load_case();
    void slot_set_image_mounter_progress_message(QString message);
    void slot_carver_process_data();
    void slot_carver_separate_process_cancelled(QString job_type);
    void slot_carve_files_clicked(QString source_count_name, QString complete_file_path, bool bool_carve_unallocated);
    void slot_thread_carved_files_separate_process_finished();

    void slot_decompressed_files_view(QString plugin_name, QString parent_name, QString tab_name, QString record, QString db_path, QString source_count_name);
    void slot_carve_data_clicked(QString source_count_name, QString complete_file_path, QString plugin_name, QString tab_name, QString record_no);
    void slot_thread_carved_data_finished();
    void slot_carver_data_process_data();
    void slot_carve_data_cancelled(QString job_type);

    void slot_run_fs_module_apple_metadata_already_running_status();
    void slot_run_fs_module_exif_metadata_already_running_status();
    void slot_run_fs_module_signature_analysis_already_running_status();
    void slot_run_fs_module_mime_type_analysis_already_running_status();
    void slot_run_fs_module_hashset_already_running_status();
    void slot_run_fs_module_face_analysis_already_running_status();
    void slot_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_optical_character_recognition_already_running_status();
    void slot_run_fs_module_weapons_already_running_status();
    void slot_run_fs_module_fire_analysis_already_running_status();
    void slot_run_fs_module_skin_tone_detection_already_running_status();

    void slot_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_hashset(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_weapons(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void slot_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);

    void slot_plugin_viewer_space_pressed_for_quicklook(QString filepath);
    void slot_add_snapshot_in_case_tree(QString tab_text);
    void slot_tsk_fuse_process_data();
    void slot_tsk_fuse_finished(int exit_code);
    void slot_right_click_snapshot_clicked(QString source_count_name);
    void slot_tablewidget_goto_record_for_snapshots(QString tab_name, QString snanpshot_tempered_name, QString record, QString feature_name);
    void slot_snapshot_display(QString snapshot_db_name);
    void slot_right_click_disk_hex_view(QString item_text, QString source_count_name, bool bool_is_complete_source);
    void slot_right_click_view_forensic_image_in_hex_viewer(QString source_count_name, bool bool_is_complete_source);
    void slot_cancel_disk_hex_view_unallocated_extraction(QString job_type);
    void slot_thread_disk_hex_view_unallocated_finished();
    void slot_action_hashset_from_toolbar_triggered();
    void slot_hashset_toolbar_apply_clicked(QStringList default_hashset_list);
    void slot_global_tagged_file_export_triggered();

    void slot_add_all_available_hashset_in_case_tree();
    void slot_display_for_artifacts_timeline_saved_results_and_graphs(QString category_name , QString matched_str);
    void slot_display_for_redefined_results_saved_results_and_graphs(QString feature_name,QString tab_text, QString db_path, QString plugin_name);
    void slot_display_for_saved_maps(QString tab_text);

    void slot_verifying_source_start(QString name);
    void slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data hex_viewer_data_obj);

    void slot_wapis_jao();
    void slot_action_exif_metadata_filters_triggered();
    void slot_exif_metadata_filter_work_done(QString cmd,QString where_statement, QStringList sources_list, QString search_label);
    void slot_thread_exif_metadata_filter_finish();

    void slot_r_case_thread_exif_metadata_search_cancelled();
    void slot_right_click_add_source(struct_GLOBAL_witness_info_source st_source_info);
    void slot_polling_timer_timeout();
    void slot_prc_apple_metadata_prc_left_click_finished();
    void slot_action_export_case_from_toolbar_triggered();
    void slot_action_recognize_face_triggered();

    void on_action_run_artifacts_mainmenu_triggered();

    void on_action_ram_analysis_mainmenu_triggered();

    void on_action_text_indexing_mainmenu_triggered();

    void on_action_hashset_mainmenu_triggered();

    void on_action_file_search_mainmenu_triggered();

    void on_action_content_search_mainmenu_triggered();

    void on_action_exif_metadata_search_mainmenu_triggered();

    void on_action_apple_metadata_search_mainmenu_triggered();

    void on_action_artifacts_keyword_search_mainmenu_triggered();

    void on_action_recognize_face_mainmenu_triggered();

    void on_action_artifacts_timeline_mainmenu_triggered();

    void on_action_super_timeline_mainmenu_triggered();

    void on_action_tagged_file_export_mainmenu_triggered();

    void on_action_export_case_mainmenu_triggered();

    void on_action_global_report_mainmenu_triggered();

    void on_action_story_board_mainmenu_triggered();
    void slot_graykey_ios_source_decompression_timeout();
    void slot_cellebrite_ios_tar_source_decompression_timeout();
    void slot_graykey_android_source_decompression_timeout();
    void slot_pushbutton_okay_full_disk_access_clicked(bool click);

private:
    Ui::MainWindow *ui;


    enum enum_mainwindow_apple_metadata{
        enum_mainwindow_apple_metadata_KEY = 0,
        enum_mainwindow_apple_metadata_Value,
        enum_mainwindow_apple_metadata_Plugin_Table_Name,
        enum_mainwindow_apple_metadata_Plugin_Db_Path,
        enum_mainwindow_apple_metadata_Plugin_Record_no,
        enum_mainwindow_apple_metadata_File_Path
    };


    enum enum_mainwindow_update_recon_library
    {
        enum_mainwindow_recon_library_update_agency_details = 0,
        enum_mainwindow_recon_library_update_case_result_info,
        enum_mainwindow_recon_library_update_exif_attributes,
        enum_mainwindow_recon_library_update_apple_attributes,
        enum_mainwindow_recon_library_update_extension_category,
        enum_mainwindow_recon_library_insert_into_extension_category,

        enum_mainwindow_recon_library_update_extension_list,
        enum_mainwindow_recon_library_insert_into_extension_list,
        enum_mainwindow_recon_library_update_signature_category,
        enum_mainwindow_recon_library_update_signature_list,
        enum_mainwindow_recon_library_insert_hashset,
        enum_mainwindow_recon_library_update_index_files_whitelist_category,
        enum_mainwindow_recon_library_insert_into_index_files_whitelist_category,
        enum_mainwindow_recon_library_update_index_files_blacklist_category,
        enum_mainwindow_recon_library_insert_into_index_files_blacklist_category,
        enum_mainwindow_recon_library_insert_into_index_files_blacklist_extensions,
        enum_mainwindow_recon_library_insert_into_index_files_whitelist_extensions,
        enum_mainwindow_recon_library_insert_into_volatility_path,
        enum_mainwindow_recon_library_insert_into_selected_text_index_filter,
        enum_mainwindow_recon_library_insert_into_external_applications,
        enum_mainwindow_recon_library_insert_into_timezone_details,
        enum_mainwindow_recon_library_insert_keyword_search_category,

        enum_mainwindow_recon_library_update_filetypes,
        enum_mainwindow_recon_library_insert_into_filetypes,
        enum_mainwindow_recon_library_update_Tags,
        enum_mainwindow_recon_library_insert_into_Tags,
        enum_mainwindow_recon_library_update_plugin_template,
        enum_mainwindow_recon_library_keyword_search_category,
        enum_mainwindow_recon_library_keyword_search_keywords_list,
        enum_mainwindow_recon_library_insert_into_preferences

    };


    enum enum_mainwindow_update_app_configurations
    {
        enum_mainwindow_app_configurations_preferences = 0
    };


    QString customer_name;
    qint64 license_purchase_date_numeric_qint64;
    qint64 license_expiry_date_numeric_qint64;


    //---------------------------------------------------//

    bool lic_integerity_failed;
    bool lic_file_not_exists;
    void lic_validate_license_integerity();

    QStringList usb_device_list;
    void create_usb_device_list();

    bool licensed_usb_present;
    QString licensed_usb_device;
    qint64 lic_purchase_date_numeric_qint64;
    void lic_validate_connected_usb_device();

    QStringList license_module_hash_list;
    void lic_create_module_hash_list();
    QByteArray purchase_date_hash;
    QByteArray expiry_date_hash;
    void extract_purchase_expiry_date();
    void set_recon_jaari_smay();
    qint64 recon_jaari_smay_date_numeric_qint64;

    bool is_lic_passed_for_valid_day_remaining();

    void license_set_purchase_expiry_date_hash(QString hash, QString exp_hash);
    void license_set_module_hash_list(QStringList list);
    QString generate_magic_key(int);

    QString license_purchase_date_hash;
    QString license_expiry_date_hash;
    QString license_usb_serial;

    //------------------------------------------------------//

    void update_examiner_machine_recon_library();
    void update_recon_library_by_118();
    void update_recon_library_by_145();
    void update_recon_library_by_147();


    bool need_to_update_result_for_load_case();
    void update_result_necessity_for_case_info();
    void update_case_info_100_to_114();
    void update_case_info_114_to_133();
    void update_case_info_133_to_142();
    void update_case_info_142_to_148();


    void update_result_necessity_for_sources();
    void update_source_100_to_112();
    void update_source_112_to_114();
    void update_source_114_to_116();
    void update_source_116_to_117();
    void update_source_117_to_120();
    void update_source_120_to_125();
    void update_source_125_to_135();
    void update_source_135_to_140();
    void update_source_140_to_142();
    void update_source_142_to_145();
    void update_source_145_to_146();
    void update_source_146_to_147();
    void update_source_147_to_148();
    void update_source_148_to_149();
    void update_source_149_to_150();
    void update_source_150_to_153();
    void update_source_153_to_156();
    void update_source_156_to_160();
    void update_source_160_to_161();

    void update_result_necessity_for_configuration();
    void update_configuration_136_to_139();
    void update_configuration_106_to_136();
    void update_configuration_139_to_142();
    void update_configuration_142_to_143();
    void update_configuration_143_to_148();
    void update_configuration_148_to_150();
    void update_configuration_150_to_152();
    void update_configuration_152_to_155();
    void update_configuration_155_to_158();

    void update_result_necessity_for_data();
    void update_result_106_to_107();
    void update_result_107_to_109();
    void update_result_109_to_111();
    void update_result_111_to_112();
    void update_result_112_to_114();
    void update_result_114_to_116();
    void update_result_116_to_117();
    void update_result_117_to_120();
    void update_result_120_to_123();
    void update_result_123_to_125();
    void update_result_125_to_126();
    void update_result_126_to_128();
    void update_result_128_to_129();
    void update_result_129_to_130();
    void update_result_130_to_132();
    void update_result_132_to_133();
    void update_result_133_to_135();
    void update_result_135_to_136();
    void update_result_136_to_137();
    void update_result_137_to_138();
    void update_result_138_to_139();
    void update_result_139_to_140();
    void update_result_140_to_141();
    void update_result_141_to_142();
    void update_result_142_to_143();
    void update_result_143_to_144();
    void update_result_144_to_145();
    void update_result_145_to_146();
    void update_result_146_to_147();
    void update_result_147_to_148();
    void update_result_148_to_150();
    void update_result_150_to_151();
    void update_result_151_to_152();
    void update_result_152_to_153();
    void update_result_153_to_154();
    void update_result_154_to_155();
    void update_result_155_to_157();
    void update_result_157_to_158();

    void update_result_158_to_159();

    void update_result_159_to_160();
    void update_result_160_to_161();

    void update_result_feature_plist_viewer_123(QString db_path);
    void update_result_feature_hex_viewer_123(QString db_path);
    void update_result_plugin_gmail_123();
    void update_result_plugin_android_contacts_123();
    void update_result_plugin_android_photos_123();
    void update_result_plugin_android_installed_application_123();
    void update_result_plugin_online_accounts_123();
    void update_result_plugin_sound_recorder_123();
    void update_result_plugin_airport_wireless_network_123();
    void update_result_plugin_calendar_123();
    void update_result_plugin_camera_123();
    void update_result_plugin_clock_123();
    void update_result_plugin_current_wallpaper_123();
    void update_result_plugin_google_chrome_123();
    void update_result_plugin_google_drive_123();
    void update_result_plugin_launchpad_123();
    void update_result_plugin_messages_123();
    void update_result_plugin_bluetooth_123();
    void update_result_plugin_youtube_123();
    void update_result_plugin_wifi_location_123();
    void update_result_plugin_printer_spool_123();
    void update_result_plugin_opera_mini_123();
    void update_result_plugin_google_keep_123();
    void update_result_plugin_vlc_123();
    void update_result_plugin_link_files_123();
    void update_result_plugin_viber_123();

    void update_result_plugin_user_logs_125();
    void update_result_plugin_bluetooth_logs_125();
    void update_result_plugin_network_logs_125();
    void update_result_plugin_icloud_logs_125();
    void update_result_plugin_hfs_plus_devices_logs_125();
    void update_result_plugin_usb_logs_125();
    void update_result_plugin_wake_reason_logs_125();
    void update_result_plugin_exe_files_125();
    void update_result_plugin_opera_mini_125();

    void update_result_plugin_fs_events_126();

    void update_result_plugin_voice_memos_128();
    void update_result_plugin_clock_128();
    void update_result_plugin_line_128();

    void update_result_plugin_google_chrome_129();
    void update_result_plugin_safari_129();
    void update_result_plugin_mozilla_firefox_129();
    void update_result_plugin_photos_129();

    void update_result_plugin_photos_130();
    void update_result_plugin_translate_130();
    void update_result_plugin_utorrent_web_130();
    void update_result_plugin_brave_browser_130();
    void update_result_plugin_microsoft_edge_130();

    void update_result_plugin_brave_browser_132();


    void update_result_plugin_utorrent_web_133();
    void update_result_plugin_screen_time_133();
    void update_result_plugin_tv_133();
    void update_result_plugin_podcasts_133();
    void update_result_plugin_network_logs_133();
    void update_result_plugin_user_logs_133();
    void update_case_fs_module_status_to_133();

    void update_result_plugin_stocks_135();
    void update_result_plugin_mail_135();
    void update_result_plugin_brave_browser_135();

    void update_result_plugin_utorrent_web_136();
    void update_result_plugin_podcasts_136();
    void update_result_plugin_screen_time_136();
    void update_result_plugin_brave_browser_136();
    void update_result_plugin_bittorrent_web_136();

    void update_result_plugin_fs_events_137();

    void update_result_plugin_connected_ios_devices_138();
    void update_result_plugin_finder_138();
    void update_result_plugin_recent_items_138();

    void update_result_plugin_reminders_139();
    void update_result_plugin_photos_139();

    void update_result_plugin_tv_140();
    void update_result_plugin_brave_browser_140();
    void update_result_plugin_podcasts_140();
    void update_result_plugin_online_accounts_140();
    void update_result_plugin_uc_browser_140();
    void update_result_plugin_whatsapp_140();
    void update_result_plugin_textme_140();
    void update_result_plugin_skype_140();
    void update_result_plugin_icloud_140();
    void update_result_plugin_itunes_140();
    void update_result_plugin_mail_140();
    void update_result_plugin_viber_141();
    void update_result_plugin_skype_141();

    void update_result_os_scheme_142();
    void update_result_plugin_knowledgec_142();
    void update_result_plugin_voice_mail_142();
    void update_result_plugin_skype_142();
    void update_result_plugin_facetime_142();
    void update_result_plugin_notes_142();

    void update_result_plugin_safari_143();
    void update_result_plugin_messages_143();
    void update_result_plugin_tv_143();

    void update_result_plugin_airport_wirelessnetwork_144();
    void update_result_plugin_maps_144();
    void update_result_plugin_notifications_144();
    void update_result_plugin_music_144();

    void update_result_plugin_wifi_location_145();
    void update_result_plugin_android_thumbnails_145();
    void update_result_plugin_music_library_145();


    void update_result_feature_emlx_mail_146();

    void update_result_plugin_linkedin_147();
    void update_result_plugin_contacts_147();
    void update_result_plugin_airport_wireless_network_147();

    void update_result_plugin_user_accounts_148();
    void update_result_all_artifacts_for_domain_user_148();

    void update_result_features_150();
    void update_result_all_artifacts_for_system_user_150();
    void update_result_plugin_reminders_150();

    void update_result_features_151();

    void update_result_plugin_messages_152();
    void update_result_plugin_linkedin_152();
    void update_result_plugin_whatsapp_152();
    void update_result_plugin_mail_152();
    void update_result_plugin_airport_wireless_network_152();
    void update_result_plugin_photos_152();
    void update_result_plugin_knowledgec_152();

    void update_result_features_153();
    void update_result_plugin_messages_153();

    void update_result_plugin_messages_154();

    void update_result_plugin_daily_out_155();

    void update_result_features_157();
    void update_result_plugin_safari_157();

    void update_result_features_158();

    void update_result_features_159();

    void update_result_plugin_thumb_cache_160();
    void update_result_plugin_keychain_160();

    void update_result_features_161();

    void update_result_plugin_reminders_999();
    void update_result_plugin_system_logs_999();
    void update_result_plugin_mount_devices_logs_999();
    void update_result_plugin_truecaller_999();
    void update_result_plugin_apple_agent_logs_999();
    void update_result_plugin_screen_sharing_logs_999();
    void update_result_plugin_applications_track_logs_999();
    void update_result_plugin_login_window_logs_999();
    void update_result_plugin_wifi_logs_999();
    void update_result_plugin_notifications_logs_999();
    void update_result_plugin_books_999();
    void update_result_plugin_facebook_999();
    void update_result_plugin_music_999();
    void update_result_plugin_google_locations_999();



    ///- RECON Interface -(START)
    void draw_complete_gui();
    void set_layout_1_main_tree_layout_and_case_tab();
    void set_layout_2_attch_previewer_tab_with_layout_1();

    void create_file_previewer_tabs();
    void set_layout_tree_widgets();

    QWidget *main_widget;
    QHBoxLayout *layout_hbox_1_main;
    QHBoxLayout *layout_hbox_2_atch_metadata_tabs;
    QSplitter *splitter_main_display;

    R_Treewidget *treewidget_case_display_obj;

    //R_Tabwidget *tabwidget_cases;
    //  R_Tabwidget *tabwidget_plugins_and_global_items;

    QSplitter *splitter_case_tab_and_metadata_display;
    QVBoxLayout *layout_vbox_1_attach_casetab_and_metadatatab;
    R_Tabwidget *tabwidget_metadata_viewer;
    R_Tabwidget *tabwidget_av_and_images_previewer;

    QWidget *widget_metadat_display;

    image_preview *image_preview_object;
    blank *blank_object;
    av_preview *av_preview_object;

    //m_textedit *textedit_metadata;
    QPlainTextEdit *textedit_detailed_information;
    m_stackedwidget *stackwidget_previewer;
    m_treewidget *tree_detailed_info_apple_metadata;

    QPushButton *pushbutton_statusbar;

    ///- RECON Interface -(END).

    // QAction *action_new_case;
    //  QAction *action_load_case;
    QAction *action_carver;
    QAction *action_quick_look;

    //    QAction *action_keyword_search;
    //    QAction *action_artifacts_timeline_full;

    QAction *action_global_report;

    //    QAction *action_run_plugins;

    QAction *action_add_source;
    QAction *action_examiner_space;
    QAction *action_screenshot;
    QAction *action_activity_logs;

    //    QAction *action_super_timeline;
    //    QAction *action_content_search;
    //    QAction *action_file_search_regular_filters;
    //    QAction *action_file_search_apple_metadata_filters;

    QAction *action_story_board;

    QAction *action_file_system_post_launcher;
    QAction *action_recon_configuration_after_launch;
    QAction *action_show_case_tree_sidebar;
    QAction *action_show_detailed_information;

    //    QAction *action_text_indexing;
    //    QAction *action_file_search_exif_metadata_filters;
    //    QAction *action_recognize_face;
    //    QAction *action_export_case;

    export_case *export_case_obj;
    void prepare_feature_export_case_essentials();
    void fill_export_case_status_database(QString category_name, QStringList list_items);


    file_indexing_display *file_indexing_display_obj;


    //    QAction *action_ram_analysis;


    QList<RECON_case *> recon_case_list;

    void create_adhoc_paths(void);

    ///- CAse Display
    void case_tree_display();

    void display_plugin(QString plugin_name_with_record_count);

    void create_recon_necessary_paths();
    void create_and_retrieve_recon_result_dir_structure(QString rslt_path);
    void create_recon_library_structure();

    void create_recon_configuration_structure(QString RECON_Library_vrsn_spcfpath);
    void create_app_configuration_structure(QString RECON_Library_vrsn_spcfpath);

    void create_recon_case_result_file(QString dest_path);
    void copy_resource_files_to_destination();
    void highlight_search_text(QString pattern);

    void set_right_cornered_status_window();
    void set_r_case_essentials_macos();
    void set_r_case_essentials_winos();

    void set_r_case_essentials_ios_uid();
    void set_r_case_essentials_ios_xml();

    void set_r_case_essentials_ios_uniform();
    void set_r_case_essentials_otherOS();

    task_status_window *task_status_window_obj;


    RECON_case *recon_case_obj;
    case_info_details *case_info_details_obj;

    image_mounter *image_mounter_obj;

    load_case * load_case_obj;

    void remove_plugins_entry_from_tree(QStringList plugin_list);
    void remove_media_path_entry_from_tree();
    void remove_global_search_entry_from_tree();
    void remove_artifacts_timeline_saved_entry_from_tree();


    R_Treewidget *set_keyword_search_tree_widget(QString search_name);

    void r_case_connections();

    void common_remove_plugin_entry_from_display_tree(QStringList plugin_list);
    void common_current_plugin_extraction_finished_or_cancelled(QString plugin_name);
    void common_right_click_keyword_search_finished_or_cancelled(QString search_tag);
    void common_right_click_artifacts_timeline_full(QString timeline_name);
    void common_right_click_global_report_finished_or_cancelled(QStringList report_paths);
    void common_pushbutton_status_right_corner_clicked(bool checked);
    void common_treewidget_case_display_single_clicked(QTreeWidgetItem *item, int coloum);
    void common_treewidget_case_display_double_clicked(QTreeWidgetItem *item, int coloum);
    void common_tree_close_case();



    bool is_plugin_availble_for_version(QString plugin_name ,QString version);
    bool bool_loadcase_image_selection_done;

    bool bool_os_version_found;
    QString mounted_media_os_version;
    QProcess *process_generic;
    bool bool_process_generic;

    QProcess *process_apple_metadata;

    QString extract_volume_name_from_mount_path(QString path);
    QString run_command_generic(QString command, QStringList arguments);
    QString create_available_path_for_soft_mounting(QString path);
    QString soft_mount(QString path, QString username);

    bool bool_user_list_found;
    QStringList selected_user_list_from_mounted_media;

    void common_viewer_extraction_finish_or_cancelled(QString result_name, QString viewer_name);


    ///hex viewer
    // hex_viewer *hex_viewer_metadata_obj;
    hex_viewer_complete *hex_displayer_metadata_obj;


    case_wizard *case_wizard_object;
    void show_launcher_add_source(QList<struct_GLOBAL_witness_info_root> list_root_info);
    bool bool_main_launcher_call;

    r_dockwidget *dockwidget_case_tree_obj;
    r_dockwidget *dockwidget_metadata_obj;
    bool bool_metadata_show;

    void display_case_tabs(QString tab_text, QString current_case_name, QString task_type, QString itemp2, QString db_path,QString caller_func);

    QList <struct_global_system_account_info> get_system_account_QList_for_macos(QString virtual_source_path, QString src_type_intrnl);

    message_dialog *message_dialog_object;

    void create_navigation_combobox_bar();
    QHBoxLayout *hbox_layout_top_combobox_bar;

    QFlatComboBox *combobox_case_navigation;
    QPushButton *pushButton_combobox_case_close;

    QFlatComboBox *combobox_category_navigation;
    QPushButton *pushButton_combobox_category_close;
    QPushButton *pushButton_topbar_next;
    QPushButton *pushButton_topbar_previous;

    QFlatComboBox *combobox_current_displayed_feature_opened;
    QPushButton *pushButton_combobox_current_displayed_feature_close;

    QFlatComboBox *combobox_case_tree_display;
    m_stackedwidget *stack_widget_cases_plugins_and_global_items;

    void insert_struct_for_display_combobox_and_stackwidget(QString case_name, QString category, QString current_tab_name);

    QString current_case_name;
    QString current_category_name;

    bool bool_insert_data_in_stack;

    center_stackwidget_manager *center_stackwidget_manager_obj;

    void refresh_combobox_category_for_feature_list(QString case_name, QString category_text, QString feature_name);

    void remove_category_page_from_stackwidget(QString page_category);
    void remove_case_from_stackwidget(QString case_name);


    void fill_recon_essential_info_in_narad_muni();

    void show_metadata_widget(bool status);
    void insert_case_info_in_db(QSqlDatabase db_path);
    void insert_exif_attr_in_db(QSqlDatabase db);
    void insert_index_files_filter_in_db(QSqlDatabase destination_db);
    void insert_extensions_in_db(QSqlDatabase destination_db_file);
    int add_sqlite_viewer_in_treedisplay(QString destination_db_path);
    // QScrollArea *scroll_area_for_hex;

    void insert_file_signature_info_in_feature_resources_db(QSqlDatabase destination_db);
    void insert_timezone_details(QSqlDatabase destination_db);
    void update_current_timezone_in_recon_configuration(QString db_path);

    QTextEdit *textedit_case_info;

    void copy_configurations_in_result(QString destpath);
    void insert_tags_details_in_db(QSqlDatabase destination_db);

    add_tags *add_tags_module_obj;

    QString insert_hex_viewer_file_entry_from_tmp_to_recon(QString viewer_display_name, QString hex_sqlite_path_tmp, QString hex_db_path_recon);
    void insert_hex_viewer_file_blocks_from_tmp_to_recon(QString current_file_path, QString hex_sqlite_path_tmp, QString hex_db_path_recon);
    void delete_entries_from_hex_viewer_tmp(QString db_path);
    QString hex_viewer_tmp_db_path;

    void common_add_tag_name_into_case_tree(QString tag_name);
    void common_hashset_finished_or_cancelled_for_rcase(QStringList default_hashset_list, QString source_count);
    void display_already_created_tags_name(QString out_path);
    detach_window_manager *detach_window_manager_obj;

    void copy_plist_file_detail_from_tmp_to_recon(QString db_table_name, QString plist_sqlite_path_tmp, QString plist_db_pth_recon);
    void insert_plist_file_entry_from_tmp_to_recon(QString viewer_display_name, QString plist_sqlite_path_tmp, QString plist_db_path_recon);

    QString plist_viewer_tmp_db_path;



    void delete_entries_from_plist_viewer_tmp_db();
    void insert_items_in_naavigatin_combobox(QString case_name, QString category);
    void set_focus_on_case_treewidget_item(QString unique_name);
    QTreeWidgetItem *get_treewidget_item_from_item_name(QString item_name);
    center_next_previous_navigation *center_next_previous_navigation_obj;


    bool bool_update_navigation_list;
    bool bool_combobox_items_added;

    QLabel *label_status_bar;


    void set_stackwidget_display_by_unique_name(QString unique_pattern);
    void update_remained_tags_in_plugin_db(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString os_scheme);
    void update_bookmark_value_in_plugin_db(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_name_str);
    void add_current_record_details_in_tag_search_db(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString applied_tag_str, QString os_name_str);
    QStringList get_column_value_list_from_plugin_db(QString plugin_db_file, QString record_val, QStringList current_table_colum_list, QString timestamp_column_name, QString file_size_column_name, QString table_name, QStringList tag_search_arg_value_list);
    QString get_insert_query_for_tag_search(QStringList current_table_colum_list);
    void add_tags_value_in_plugin_db(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString os_name_str);
    void update_bookmark_value_in_tag_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QStringList applied_tags_list, QString os_name_str);
    void update_notes_value_in_plugin_db(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_name_str);
    void update_notes_value_in_tag_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QStringList applied_tags_list, QString os_scheme);
    void update_bookmark_value_in_keyword_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme);
    void update_notes_value_in_keyword_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_scheme);
    void update_keyword_search_db_for_current_record(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString os_scheme);
    void update_bookmark_value_in_content_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme);
    void update_tag_search_db_and_display_for_remove_tag(QString plugin_name, QString tab_name, QString record, QStringList list_tags_removed, QString os_scheme);
    void update_remained_tag_in_content_search_db_and_display(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString os_scheme);
    void insert_plugins_info_in_config_db(QSqlDatabase destination_db, QString csv_dir_path);
    void add_current_record_details_from_file_system_to_tag_search_db(QString tag_name, QString plugin_name, QString table_name, QString record_no, QString applied_tag_str, QString fs_db_path, QString os_scheme);

    void update_notes_value_in_notes_search_db_and_display_from_ram_analysis_saved(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_scheme);
    void update_notes_value_in_ram_analysis_saved_db_and_display_from_notes_search(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_scheme);
    void update_remained_tag_in_ram_analysis_saved_db_and_display(QString plugin_name, QString tab_name, QString record_no, QString tag_name, QString os_scheme);

    void update_tags_in_notes_search_db_and_display_from_feature(QString plugin_name, QString tab_name, QString record_no, QString tag_name, QString os_scheme);

    void update_bookmark_value_in_graph_result_saved_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme);

    redefined_result_filters_interface *redefined_result_filters_interface_obj;
    ios_uid_backup_parser *ios_uid_backup_parser_obj;

    void insert_record_entry_into_tag_search_db_for_file_system(QString record_no_prepended_zero, QString table_name, QString tags_db_path, QString tag_name, QString fs_db_path, QString os_scheme);
    void update_remained_tags_in_tag_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme);
    void remove_entry_form_the_feature_combobox(QString item_name);
    void update_remained_tags_in_notes_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme);
    void update_bookmark_value_in_bookmarks_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString fs_db_path, QString os_name_str);
    void insert_value_into_bookmark_db_for_plugin(QString record_no, QString plugin_name, QString tab_name, QString status, QString tag_db_path, QString os_name_str);
    void update_bookmark_db_and_display_for_current_record(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString os_scheme);
    void update_notes_value_in_bookmark_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme);
    void comman_tree_add_source();


    void set_device_mounting();

    quick_look *quick_look_obj;
    bool bool_quick_look_preview_clicked;
    QString quick_look_file_preview_path;
    void common_right_click_remove_selected_clicked();
    void right_click_remove_widget_by_unique_name(QString unique_name, QString viewer_display_name);
    void remove_right_click_keyword_search_info(QString dbpath, QString viewer_display_name);
    void remove_right_click_artifacts_timeline_saved_info(QString dbpath, QString viewer_display_name);
    void remove_right_click_plist_info(QString dbpath, QString viewer_display_name);
    void remove_right_click_sqlite_info(QString dbpath, QString viewer_display_name);
    void remove_right_click_hexviewer_info(QString dbpath, QString viewer_display_name);
    void common_tree_add_source_from_fs(struct_GLOBAL_witness_info_source &struct_info);

    bool bool_message_box_yes_button_clicked;
    bool bool_message_box_clicked;

    void prepare_root_sources_path_for_load_case();


    QList <struct_global_system_account_info> get_system_accounts_QList_for_winos(QString essential_dir_path);
    void run_user_accounts_executable_for_winos(QString mount_path, QString essential_dir_path);
    QString get_timezone_for_winos(QString essential_path);
    void run_system_info_executable_for_winos(QString mount_path, QString essential_dir_path);

    void show_r_case_common_add_source();


    QMap<QString, ios_uid_backup_parser*> *map_ios_uid_backup_parser_obj;
    void set_current_tree_item_highlighted(QString case_name, QString category_text, QString feature_name);


    QStringList list_Selected_Plugins_With_Tab_After_Extraction_macOS;
    QStringList list_Selected_Plugins_With_Tab_After_Extraction_iOS;
    QStringList list_Selected_Plugins_With_Tab_After_Extraction_winOS;

    void common_add_notes_name_into_case_tree(QString note_name);
    QString get_insert_query_for_notes_search(QStringList current_table_colum_list, QString tablename);
    void update_bookmark_value_in_notes_search_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_name_str);
    void update_notes_db_and_display_for_remove_tag(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);
    void common_add_redefined_result_category_name_in_case_tree(QString category_name);
    void update_notes_value_in_redefined_result_full_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_scheme);
    void update_bookmark_value_in_plist_content_info_db(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_name_str);
    void update_notes_value_in_plist_content_info_db(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_name_str);
    void delete_empty_tags_from_case_tree_display();
    void delete_entry_of_note_from_case_tree();

    filetypes_loader *filetypes_obj;

    void update_notes_value_in_sqlite_content_info_db(QString record_no, QString plugin_name, QString tab_name, QString notes_text, QString os_name_str);

    bool is_it_winos_compatible_source(QString mount_path);
    bool is_it_macos_compatible_source(QString mount_path);
    QString find_os_scheme_if_os_is_installed_into_source(QString mount_path);

    void remove_right_click_content_serach_info(QString dbpath, QString viewer_display_name);
    void remove_right_click_file_search_info(QString dbpath, QString viewer_display_name);

    void insert_current_mounted_image_info_in_db();
    void delete_entries_from_mounted_image_db();
    void unmount_previously_mounted_images();
    void unmount_previous_images_according_to_mount_output();
    void unlink_previous_mounted_softlinks();

    text_viewer *text_viewer_metadata_obj;

    strings_viewer *strings_viewer_obj;

    QMap<QString, file_system*> map_file_system_obj;

    void case_tree_widget_combobox_list_display();
    bool bool_case_tree_combobox_filling;

    progress_bar_fixed_in_statusbar *progress_bar_fixed_in_statusbar_obj;

    void prepare_sources_detail_in_narad_muni_1(QString src_type_intrnl, QString received_path, QString received_name);
    void prepare_sources_detail_in_narad_muni_2();

    void insert_source_entry_in_source_case_tree(QTreeWidgetItem *item, struct_GLOBAL_witness_info_source &struct_src_info);

    load_case_source_displayer *load_case_source_displayer_obj;
    bool bool_display_load_case_page_ok_clicked;

    m_treewidget *tree_exif_metadata_display;

    void close_feature_without_save(QString unique_pattern);
    bool bool_prc_generic_without_argument;
    QProcess *prc_generic_without_argument;

    QStringList get_mounted_volumes_path_with_node_list();
    QString run_command_without_argument(QString command);
    void clear_source_narad_variables_all();
    void clear_source_narad_variables_partial();
    void clear_root_narad_variables_all();
    void clear_root_narad_variables_partial();

    void clear_case_info_narad_variables();

    void prepare_roots_detail(QList<struct_GLOBAL_witness_info_root> list_root_info);
    void prepare_sources_detail(struct_GLOBAL_witness_info_source st_src_info);

    screenshots *screenshots_obj;


    QStringList get_apple_metadata_key_value_for_file_system(QString record_no, QString source_count_name);
    void common_file_search_finished_or_cancelled(QString search_label);

    exif_data_interface *exif_interface_obj;
    QString launched_case_name;

    case_info_display *case_info_display_obj;
    QString plugin_name_for_detach_ext_attribute, table_name_for_detach_ext_attribute, record_number_for_detach_ext_attribute;

    activity_logs_storage *activity_log_storage_obj;
    activity_logs_display *activity_log_display_obj;


    void comman_remove_feature_from_combobox_and_stackwidget(QString unique_pattrn);
    void insert_default_values_in_features_resources_db(QSqlDatabase feature_config_db);
    void insert_colours_for_tags_details_in_feature_resources_db(QSqlDatabase destination_db);
    void create_feature_resources_structure();

    void display_already_created_bookmarks(QString out_path);

    void extract_online_accounts_for_redefined_result();

    header_plugin_extraction_interface* plugin_extraction_interface_obj;
    header_plugin_extraction_interface *find_plugin_ptr_by_pluginname(QString plugin_name);
    dt_search_interface *dt_search_interface_obj;

    keywords_list_selector *keywords_list_selector_obj;
    content_search *content_search_obj;

    void common_content_search_finished_or_cancelled(QString search_label);
    void insert_record_entry_into_tag_search_db_for_content_search(QString record_no, QString plugin_name, QString tab_name, QString status, QString tag_db_path, QString indexed_db_path, QString os_name_str, QString tag_name);
    void add_current_record_details_from_content_search_to_tag_search_db(QString tag_name, QString plugin_name, QString tab_name, QString record_no, QString applied_tag_str, QString index_db_path, QString os_scheme);


    bool is_sumuri_live_imager_plugin_list_file_present(QString mount_path);
    QString current_fs_display_dir_path;
    file_search_interface *file_search_interface_regular_filters_obj;
    file_search_interface *file_search_interface_apple_metadata_filters_obj;

    void common_add_screenshots_into_case_tree(QString screenshot_name);
    void add_tag_in_database_for_screenshots(QString tag_name, QString is_emitted_from_plugin, QString plugin_name_str, QString tab_name_str, QString record_no, QString dest_path, QString os_name);

    QList <story_board*> list_stroy_board_obj;

    void update_screenshot_db_and_display(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);
    void common_add_story_board_into_case_tree(QString storyboard_name);
    void update_story_board_display_on_adding_tags();

    //================================//
    /* Note:
     * These are private because they are refreshed on double click.
     * To search and delete them in case tree.
     */
    bookmarks_search_loader *bookmarks_search_loader_obj;
    tag_search_loader *tag_search_obj;
    notes_search_loader *notes_search_loader_obj;
    //================================//

    file_system_post_launcher *file_system_post_launcher_obj;

    void prepare_display_for_keyword_search(QString tab_text);
    void prepare_display_for_filetypes_and_based(QString tab_text);
    void prepare_display_for_filesystem(QString tab_text, QString itemp2, QString db_path);
    void prepare_display_for_artifacts_timeline_full(QString tab_text);
    void prepare_display_artifacts_timeline_saved(QString tab_text);
    void prepare_display_for_plist_viewer(QString tab_text, QString db_path);
    void prepare_display_for_sqlite_viewer(QString tab_text, QString db_path);
    void prepare_display_for_bookmark_search(QString tab_text);
    void prepare_display_for_tag_search(QString tab_text);
    void prepare_display_for_notes_search(QString tab_text);
    void prepare_display_for_redefined_result_full(QString tab_text);
    void prepare_display_for_screenshots(QString tab_text);
    void prepare_display_for_storyboard(QString tab_text);
    void prepare_display_redefined_result_saved(QString tab_text , QString db_path , QString plugin_name);
    void prepare_display_carved_password(QString tab_text);
    void prepare_display_for_email_files(QString tab_text);
    void prepare_display_ram_analysis_saved(QString tab_text);
    void prepare_display_for_saved_graphs(QString tab_text, QString db_path, QString plugin_name);
    void prepare_display_for_saved_maps(QString tab_text);
    void prepare_display_for_carved_files(QString tab_text);
    void prepare_display_for_registry_viewer(QString tab_text, QString index_db_path);
    void prepare_display_for_mobile_backup(QString tab_text);
    void prepare_display_for_carved_data(QString tab_text);
    void prepare_display_for_ram_images(QString tab_text);
    void prepare_display_snapshots(QString tab_text);
    void prepare_display_for_disk_hex_view(QString source_count_name, bool bool_is_complete_source, bool bool_is_unallocated_disk_view, QString unalloc_db_tbl_name);
    void prepare_display_for_disk_images(QString tab_text);
    void prepare_display_for_saved_timeline_graphs(QString tab_text);

    QString get_last_run_recon_config_db_dir_path_from_paths_list(QStringList paths_list);
    void save_creation_datetime_for_recon_library(QString recon_library_path);
    QStringList get_all_previous_recon_configuration_dirs_list_with_timestamp();
    void update_currently_created_configurations_with_previous_config(QString current_config_dir_path, QString previous_config_dir_path);
    void update_current_config_from_previous_in_db(QSqlDatabase *current_config_db, QSqlDatabase *previous_config_db, QString select_command_str, QString insert_command_str, int status_update_for, QStringList already_existing_values_list);

    void update_currently_created_app_configurations_with_previous_app_config(QString current_config_dir_path, QString previous_config_dir_path);
    void update_current_app_config_from_previous_in_db(QSqlDatabase *current_config_db, QSqlDatabase *previous_config_db, QString select_command_str, QString insert_command_str, int status_update_for, QStringList already_existing_values_list);

    bool bool_launcher_work_completed;

    offline_maps *maps_metadata_obj;

    void remove_selected_plugins_from_display_on_add_source();
    void remove_case_details_from_display_on_add_source();

    void enable_navigation_combobox_category_and_feature(int item_pos, bool status, QFlatComboBox * combobox_nav);


    QDialog *dialog_save_storyboard;
    QPushButton *pushbutton_create_storyboard;
    QLineEdit *lineedit_storyboard_filename;
    void create_dialog_for_storyboard_file_save();

    void common_add_artifacts_timeline_name_into_case_tree(QString tab_name);
    void common_add_redefined_saved_result_name_into_case_tree(QString tab_name , QString plugin_name);

    void insert_keyword_category_and_list_in_db(QSqlDatabase destination_db);
    void insert_mime_type_details_in_db(QSqlDatabase destination_db);
    void insert_file_signature_details_in_db(QSqlDatabase destination_db);
    void insert_index_files_filesize_default_details_in_db(QSqlDatabase destination_db);

    // Configuration after launch and load case
    recon_configuration *recon_configuration_after_launch_obj;


    void insert_color_info_in_recon_configuration_db(QSqlDatabase destination_db);

    void disable_case_tree_sidebar(bool status);

    QMimeDatabase mime_database_obj;

    void check_and_update_services_db_for_recon_lab();

    system_password_asker *system_password_asker_obj;
    void show_case_wizard();

    void common_add_ram_analysis_into_case_tree(QString plugin_name);
    void update_bookmark_value_in_ram_analysis_saved_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme);
    void insert_record_entry_into_bookmarks_db_for_ram_analysis_saved(QString record_no, QString plugin_name, QString tab_name, QString status, QString bookmark_db_path, QString dest_db_path, QString os_name_str, QString tag_name);
    void update_tag_search_db_and_display_for_remove_tag_for_ram_analysis(QString plugin_name, QString tab_name, QString record, QStringList list_tags_removed, QString os_scheme);
    void add_and_update_tags_in_database_for_ram_analysis_saved(QString tag_name, QString is_emitted_from_plugin, QString plugin_name_str, QString tab_name_str, QString record_no, QString dest_path, QString os_name);
    void common_add_saved_graph_category_name_in_case_tree(QString category_name);
    void insert_record_entry_into_bookmarks_db_for_saved_graph_result(QString record_no, QString plugin_name, QString tab_name, QString status, QString bookmarks_db_path, QString saved_graphs_db_path, QString os_name_str, QString tag_name);
    void add_and_update_tags_in_database_for_graph_result_saved(QString tag_name, QString is_emitted_from_plugin, QString plugin_name_str, QString tab_name_str, QString record_no, QString saved_graphs_db_path, QString os_name);
    void update_remained_tag_in_graph_result_saved_db_and_display(QString plugin_name, QString tab_name, QString record_no, QString tag_name);
    void insert_ext_attributes_in_recon_library(QString csv_filepath, QSqlDatabase configuration_db, bool bool_insert_apple_attributes);
    void create_redefined_result_db_table_and_insert_into_index_table();

    void delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag, QString record_no, QString plugin_name, QString tab_name);

    license_locator *license_locator_obj;


    text_viewer_aascii_unicode *text_viewer_aascii_unicode_obj;
    QString content_search_keyword_str;

    QStringList detailed_info_pane_file_list ;
    QString detailed_info_pane_file, detailed_info_pane_file_source_count_name;
    QString detailed_info_pane_file_record_no, detailed_info_pane_file_table_name, detailed_info_pane_file_plugin_name;


    initial_warning_message *initial_warning_message_obj;

    bool bool_loading_result_operation_running;
    void check_for_demo_version();
    bool is_it_a_demo;
    bool bool_is_international_version;
    void check_for_international();

    network_time *network_time_mainwindow;


    global_connection_manager *global_connection_manager_obj;

    bool bool_text_indexing_running;

    void set_up_global_connection_manager();


    void stop_rcase_all_threads();

    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    void loading_display_dialog_based_on_records(qint64 total_records, qint64 processed_count);
    bool bool_cancel_loading ;
    super_timeline *super_timeline_obj;

    QTcpSocket *socket_quicklook;
    bool bool_is_lab_running_from_root;

    struct_global_set_metadata_info st_metadata_obj;
    void set_metadata_information();
    void set_metadata_info_detailed_info();
    void set_metadata_info_previewers();
    void set_metadata_info_quicklook();
    void set_metadata_info_strings_viewer();
    void set_metadata_info_text_viewer();
    void set_metadata_info_hex_viewer();
    void set_metadata_info_apple_metadata();
    void set_metadata_info_exif_metadata();
    void set_metadata_info_maps();

    void remove_right_click_redefined_result_messenger_saved_info(QString dbpath, QString viewer_display_name);
    void remove_right_click_redefined_result_browsers_saved_info(QString dbpath, QString viewer_display_name);

    file_search_regular_filters *file_search_regular_filters_obj;
    thread_file_search_regular_filters *thread_file_search_regular_filters_obj;

    display_loading_progress_bar *file_search_regular_filters_progress_bar_obj;
    bool bool_file_search_regular_filters_cancel = false;
    QString file_search_regular_label;

    bool bool_metadata_info_already_cleared;
    QString create_link_path_for_preview(QString extension_str, QString file_path);
    void set_r_case_essentials_gtakeoutos();
    bool is_newer_lib_version_available(QString current_version, QString last_version);
    void common_add_carved_password_into_case_tree(QString tab_name);


    bool bool_fs_extraction_finish_on_load_case = false;
    bool bool_email_files_case_tree_add_finish_on_load_case = false;
    bool bool_google_mail_files_case_tree_add_finish_on_load_case = false;

    QString registry_viewer_tmp_db_path;
    void copy_from_tmp_for_registry_viewer(QString display_tab_name);
    void remove_entries_for_registry_viewer(QString display_tab_name , QString db_path, QString index_db_path);



    thread_carve_files *thread_carve_files_obj;
    QProcess *process_carve_files;
    bool bool_thread_carve_files_seprate_process = false;

    QString set_carver_essentials_database(struct_GLOBAL_witness_info_source struct_source_info, bool bool_carve_unallocated, QString complete_file_path);
    void insert_recent_load_case_result_info(QString result_dir_path);
    QString get_source_name_from_mount_strct_info(struct_global_image_mounter_partition_info struct_partition_info);
    QTreeWidgetItem * check_parent_already_exist_and_get_index(QTreeWidgetItem *item, bool &image_name_exists, struct_GLOBAL_witness_info_source struct_src_info);
    void remove_open_with_entries_for_sqlite_viewer(QString unique_pattern);
    void update_narad_muni_paths_for_nested_sources();
    QString get_virtual_source_path_for_load_case_except_images(struct_GLOBAL_witness_info_source struct_received_obj);

    bool bool_need_to_update_result_for_load_case;

    void add_carved_data_in_case_tree_on_finish();
    QProcess *process_carve_data;
    bool bool_thread_carve_data_running = false;
    thread_carve_data *thread_carve_data_obj;
    QString carved_data_current_file;

    QString extract_iOS_configured_id(QString db_path);
    void update_value_for_source_textinfo_file_only(QString file_path, QString m_keyword, QString m_value);
    QString get_value_of_key_from_source_textinfo_file_from_100_to_148(QString file_path, QString mkey);
    QString get_value_of_key_from_source_textinfo_file_from_149_to_current_version(QString file_path, QString mkey);
    QString get_value_of_key_from_source_textinfo_file_only(QString file_path, QString mkey, QString m_splitter);
    void remove_entries_for_carved_files(QString display_tab_name);

    void update_tsk_mount_path_for_source();
    void update_raw_fuse_mount_path_for_source();
    void update_snapshot_mount_path_for_load_case();

    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;
    recon_helper_process *recon_helper_process_android_obj;
    recon_helper_process *recon_helper_process_ios_obj;


    void common_add_snapshots_diff_into_case_tree(QString snapshot_name);

    QProcess *process_tsk_fuse;
    bool bool_process_tsk_fuse_finished = false;


    QStringList get_snapshots_list_for_apfs(QString mount_path);

    snapshots *snapshots_obj;
    struct_global_recon_file_info st_globl_recon_file_info_obj;

    void create_apfs_snapshots_table_in_sources_db(QString sources_db_path);
    void unmount_previously_mounted_paths_from_home();
    void set_up_global_recon_file_info();
    global_recon_file_info *global_recon_file_info_obj;

    void set_r_case_essentials_androidos();
    QStringList get_redefined_result_saved_name_from_result_db(QString result_path, QString caller_func);
    QStringList get_content_search_namelist_from_result_db(QString result_feature_path, QString caller_func);
    QStringList get_redefined_result_namelist_from_db(QString result_path, QString caller_func);
    QStringList get_story_board_namelist_from_db(QString result_path, QString caller_func);
    QStringList get_saved_graph_result_namelist_from_db(QString result_path, QString caller_func);
    QStringList get_file_search_namelist_from_result_db(QString result_feature_path, QString caller_func);
    QStringList get_artifacts_timeline_saved_name_from_result_db(QString result_path, QString caller_func);
    QString get_plugin_db_path_from_result_using_plugin_name(QString plugin_name);
    QStringList extract_androidos_configured_id();
    void prepare_thread_for_disk_hex_view_unallocated(QString source_count_name, bool bool_is_complete_source);
    bool bool_thread_disk_hex_view_unallocated_running = false;
    thread_hex_view_unallocated *thread_hex_view_unallocated_obj;

    QString extract_androidos_system_timezone();
    void set_narad_and_essentials_for_language_translater();


    //    QAction *action_hashset_from_toolbar;
    //    QAction *action_export;

    hashset_display_toolbar *hashset_display_toolbar_obj;
    tagged_file_export *tagged_file_export_obj;
    QList<struct_global_available_hashset_data> available_hashset_data_list;
    void prepare_available_hashsets_data_list();

    QString get_extracted_android_ab_file_backup_path();
    QString get_extracted_cellebrite_ios_tar_backup_path();
    QString get_extracted_cellebrite_ios_ufdr_backup_path();



    void lic_validate_connected_usb_device_newer_from_june_2019();
    void lic_validate_license_integerity_newer_from_june_2019();

    void update_load_case_artifacts_plugin_related_config(QString plgn_name, csv_reader *csv_reader_local);
    void update_load_case_artifacts_plugin_entry_config(QString plgn_name, csv_reader *csv_reader_local);
    void update_load_case_artifacts_plugin_export_case_config_for_reader(QString plgn_name, csv_reader *csv_reader_local);

    void update_load_case_artifacts_tab_rename_config(QString plgn_name, QStringList old_tab_nm_list, QStringList new_tab_nm_list);
    void update_load_case_artifacts_tab_entry_config(QString plgn_name, csv_reader *csv_reader_local);
    void update_load_case_artifacts_tab_remove_config(QString plgn_name, QStringList removable_tab_list);

    struct_global_hex_viewer_open_file_data st_hex_viewer_open_file_data_obj;
    void prepare_display_for_hex_viewer(QString tab_text, QString db_path);
    void prepare_hex_viewer_display_structure_data(QTreeWidgetItem *item, QString db_path);

    bool bool_license_invalidation_value = false;


    QString get_os_version_for_winos(QString essential_dir_path);
    void action_openwith_for_hex_viewer_complete(QString source_count_name, bool bool_is_complete_source);
    QMap<QString, QString> map_key_src_cnt_name_val_raw_fuse_mnt_path;

    void create_and_fill_mounting_path_in_narad();
    void handle_bookmark_from_detailed_info_exif_metadata_clicked(QString record_no_str, QString bookmark_status);
    void handle_bookmark_from_detailed_info_apple_metadata_clicked(QString record_no_str, QString bookmark_status);
    Container_QWidget *get_currenly_loaded_display_obj();
    bool check_any_exif_or_apple_metadata_item_selected();
    disk_manager *disk_manager_obj;



    exif_metadata_filters  *exif_metadata_filters_obj;
    thread_exif_metadata_search *thread_exif_metadata_search_obj;

    QString exif_metadata_search_label;

    void remove_right_click_exif_metadata_search_info(QString dbpath, QString viewer_display_name);
    void common_exif_metadata_search_finished_or_cancelled(QString search_label);
    QStringList get_exif_metadata_search_namelist_from_result_db(QString result_feature_path, QString caller_func);


    struct_GLOBAL_witness_info_source get_source_info_from_root_info(struct_GLOBAL_witness_info_root st_root_info);
    witness_info_store *witness_info_store_obj;

    QList<struct_GLOBAL_witness_info_root> get_updated_root_structure_list(struct_global_image_mounter_image_info img_obj);
    QStringList get_source_count_name_list_according_source_path_list(QStringList actual_path_list);
    QStringList get_sub_sourcecountnamelist_according_sourcecountname(QString rcvd_source_count_name);

    QStringList root_count_name_list_to_be_process;
    QStringList source_count_name_list_to_be_process;

    QTimer *polling_timer;
    uncloseable *uncloseable_wdgt;
    QString poller;
    int close_counter = 1;

    usb_device_serial_list usb_device_serial_list_obj;

    void insert_preferences_detail_in_db(QSqlDatabase feature_config_db);

    void create_system_and_case_info_table_in_case_configuration_db(QString case_info_db_path);
    void insert_app_config_tbl_preferences_details_in_db(QSqlDatabase feature_config_db);
    QString get_select_query_for_app_config_preferences_table(QString previous_version_number_of_config);

    void extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name, QString virtual_source_path);

    bool bool_apple_metdatata_process_already_running = false;

    QString get_quicklook_link_path_for_ios_quicklook(QString display_filepath, QString complete_filepath, QString record_no_str, QString source_count_name);
    QString get_select_query_for_plugin_template_table(QString previous_version_number_of_config);
    QString get_select_query_for_timezone_details_table(QString previous_version_number_of_config);
    QString get_select_query_for_tbl_preferences_table(QString previous_version_number_of_config);

    QString get_exact_source_type_from_src_type_partial_internal_145(QString recieved_src_type, int status, QString caller_func);

    void clear_disk_node_for_roots_and_sources_structures(QList<struct_GLOBAL_witness_info_source> &list_source_info_from_result, struct_GLOBAL_witness_info_source struct_received_obj);

    facefinder *facefinder_obj;
    recognize_face *recognize_face_obj;

    examiner_space *examiner_space_obj;

    QPlainTextEdit *textedit_optical_character_recognition_information;
    void set_metadata_info_optical_character_recognition();
    QString get_extracted_ios_graykey_backup_path();
    QString decompressed_graykey_ios_backup_path;
    QString decompressed_cellebrite_ios_tar_backup_path;

    void update_zip_mount_path_for_source();

    QString decompressed_graykey_android_backup_path;
    QString get_extracted_android_graykey_backup_path();
    void extract_product_version_name_model_language_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void extract_country_code_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void extract_iccid_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void extract_configured_id_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void extract_bluetooth_address_device_id_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void extract_phone_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void extract_imei_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj);
    void clear_android_backup_detail_info_structure(struct_global_android_backup_detail &mobj);
    void extract_serial_no_for_graykey_android_backup(struct_global_android_backup_detail &mobj);

    void extract_timezone_for_graykey_android_backup(struct_global_android_backup_detail &mobj);

    void common_add_category_name_into_case_tree_griffeye(QStringList category_name_list, QStringList griffeye_db_path_list);
    void add_griffeye_data_category_according_to_available_hashset_list();
    void add_rec_with_category_name_into_griffeye_db(QStringList griffeye_category, QStringList md5_str);
    bool get_macos_appearence_mode();

    bool validate_full_disk_access_functionality();
    bool check_full_disk_access(QString source_db_path, QString tcc_db_adoch_path);
    void show_message_widget_for_full_disk_access();
    QString copy_tcc_db_to_adhoc_path(QString source_db_path);

    void create_log_file_table_in_log_viewer_db(QString db_table_name, QString log_db_pth_recon);
    void insert_log_file_index_table_entry_from_tmp_db_to_recon_db(QString viewer_display_name, QString log_sqlite_path_tmp, QString log_db_path_recon);
    QString log_viewer_tmp_db_path;
    void remove_right_click_log_info(QString dbpath, QString viewer_display_name);
    void prepare_display_for_log_viewer(QString tab_text, QString db_path);
    void right_click_action_view_complete_forensic_image_with_hex_viewer(QString source_count_name, bool bool_is_complete_source);
    bool get_macos_desktop_processor_chip_arch_type();
    void prepare_display_for_unified_logs_viewer(QString tab_text, QString db_path);

};

#endif // MAINWINDOW_H
