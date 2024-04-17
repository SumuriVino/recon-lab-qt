#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QFrame>
#include "ui_custom_classes/m_tablewidget.h"
#include <QVBoxLayout>
#include <QtSql>
#include "recon_generic_functions/recon_static_functions.h"

#include "QtCore"
#include <QFileIconProvider>
#include <QSplitter>
#include "header_global_variables_3.h"                         
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <CoreServices/CoreServices.h>

#include "progress_bar/progress_bar_window.h"
#include "header_gui_settings.h"
#include "header_identical_2.h"
#include "header_identical_3.h"
#include "add_notes/add_notes.h"

#include "tsk/libtsk.h"
#include "file_viewer/plist_viewer/plist_viewer.h"
#include "file_viewer/sqlite_viewer/sqlite_viewer_starter.h"

#include "message_box/message_dialog.h"

#include "hashset/hashset_display.h"
#include "hashset/hashset_storage.h"

#include "add_tags/add_tags.h"
#include "add_tags/remove_multiple_tag_selecotor.h"

#include "full_detailed_information/full_detailed_info.h"
#include "header_global_structures.h"

#include "apple_attributes/apple_attributes.h"
#include "header_global_enum.h"
#include "header_global_variables_1.h"
#include "file_viewer/text_viewer/text_viewer.h"
#include "ui_custom_classes/m_hovered_pushbutton_with_signal.h"
#include "filters/keyword_search_filters/keywords_list_selector.h"
#include "file_search/file_search_interface.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "header_global_variables_2.h"

#include "gallery_viewer/gallery_viewer.h"
#include "export_dialog_box/export_dialog_box_3/export_dialog_box_3.h"

#include "report/report_essentials/report_status.h"
#include "thread_db_updation/thread_db_updation_bookmark.h"
#include "global_connection_manager.h"
#include "file_viewer/registry_viewer/registry_viewer.h"
#include "file_system_extraction/decompress_file_system.h"
#include "file_export_dialog.h"

#include "export/export_except_reports/export_zip/thread_zip_file_export.h"
#include "recon_generic_functions/recon_helper_standard.h"
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

#include "ui_custom_classes/r_treewidget.h"
#include "export/export_hashes/export_hashes_as_vic/export_hashes_as_vic.h"
#include "export/export_hashes/export_hashes_as_sqlite/export_hashes_as_sqlite.h"
#include "export/export_hashes/export_hashes_as_csv/export_hashes_as_csv.h"
#include "global_recon_file_info/global_recon_file_info.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"
#include "file_search/search_files_with_hash/search_files_with_hash.h"
#include "recon_case/recon_case.h"
#include "app_related_files/app_related_files.h"
#include "recon_generic_functions/recon_helper_process.h"
#include "custom_table_header/custom_table_header_widget.h"
#include "file_viewer/log_viewer/log_viewer.h"

namespace Ui {
class file_system;
}

class file_system : public QFrame
{
    Q_OBJECT

public:
    explicit file_system(QWidget *parent = 0);
    ~file_system();

    void pub_set_necessary();
    void set_essentials();

    void pub_set_fs_db_path(QString db_path);

    void pub_set_global_connection_manager_obj(global_connection_manager *obj);


    void make_display_for_specified_path(QString file_path);
    void set_source_count_name(QString name);
    void pub_insert_entry_info_frame_dir_navigator(QString file_path);

    QString pub_get_last_display_path_for_check();

    void pub_set_case_treewidget_obj(R_Treewidget *obj);
    void pub_set_global_recon_file_info_obj(global_recon_file_info *obj);
    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str);

    void pub_set_recon_case_obj(RECON_case *obj);
    void pub_set_face_finder_obj(facefinder *obj);

signals:
    void signal_fs_refresh_widgets();
    void signal_show_file_in_viewer_table_result(QString display_name, QString viewer_name, QString source_count_name);
    void signal_viewer_extarction_completed_table_result(QString display_name, QString viewer_name);
    void signal_hashset_create_db_finish(QStringList lidt_db);
    void signal_add_tags(QString tag_name, QString colour_name);
    void signal_add_tag_in_other_locations(QString added_tag_name, QString is_emitted_from_plugins, QString plugin_name, QString table_name, QString record_no_str, QString db_path_filesystem, QString os_scheme);
    void signal_open_in_detach_clicked_table_result(QString metadata_str, QString export_file_path, QString current_media_path, int preview_index, QString searched_keyword,QString source_count_name);
    void signal_change_content_of_detach_window(QString metadata_str, QString export_file_path, QString current_media_path, int preview_index, QString searched_keyword, QString source_count_name);
    void signal_update_bookmark_value_of_other_display_and_db(QString record_no_prepended_zero, QString fs_plugin_name, QString fs_tab_name, QString status, QStringList applied_tags_list, QString db_path_filesystem, QString os_name_str, QString feature_name);
    void signal_update_notes_value_of_other_display_and_db(QString record_no_prepended_zero, QString fs_plugin_name, QString fs_tab_name, QString status, QStringList applied_tags_list, QString os_scheme, QString feature_name);
    void signal_file_system_removed_tags_list(QString fs_plugin_name, QString tab_name, QString record_no_prepended_zero, QStringList list_removed_tags, QString os_scheme);
    void signal_update_other_display_and_db_for_remained_tags(QString fs_plugin_name, QString tab_name, QString record_no_prepended_zero, QStringList list_tags_remained,QString os_scheme);
    void signal_update_tag_submenu_of_plugins(bool status, QString new_tag_name, QString new_color_name);
    void signal_update_hashset_submenu_from_file_system(QString db_name, QString db_path);
    void signal_add_notes_from_file_system(QString record_no, QString tab_name, QString notes_db_path, QString notes_text, QString db_path_filesystem, QString os_scheme);

    void signal_statusbar_progress_bar_show_hide(bool status , QString status_message);
    void signal_update_display_on_remove_notes(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);
    void signal_run_index_k_search_on_current_dir(QStringList keyword_list, bool index_search_in_temp_mode, QString dir_path, QString source_count_name_str);

    void signal_disable_case_tree_sidebar(bool status);
    void signal_act_quick_look_triggered();
    void signal_common_add_tag_name_into_case_tree(QString tag_name);
    void signal_clear_all_metadata();
    void signal_set_metadata_info(struct_global_set_metadata_info m_obj);

    void signal_add_saved_maps_in_case_tree();
    void signal_carve_files_clicked(QString source_count_name, QString file_path, bool bool_carve_unallocated);
    void signal_carve_data_clicked(QString source_count_name, QString file_path, QString plugin_name, QString tab_name, QString record_no);

    void signal_run_fs_module_apple_metadata_already_running_status();
    void signal_run_fs_module_exif_metadata_already_running_status();
    void signal_run_fs_module_signature_analysis_already_running_status();
    void signal_run_fs_module_mime_type_analysis_already_running_status();
    void signal_run_fs_module_hashset_already_running_status();

    void signal_run_fs_module_face_analysis_already_running_status();
    void signal_run_fs_module_optical_character_recognition_already_running_status();
    void signal_run_fs_module_weapons_already_running_status();
    void signal_run_fs_module_fire_analysis_already_running_status();
    void signal_run_fs_module_skin_tone_detection_already_running_status();

    void signal_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_hashset(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_set_bookmark_status_to_gallery_view(bool bookmark_status);
    void signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj);
    void signal_save_openwith_changes_to_bucket_for_full_detail_info(QString viewer_type);

    void signal_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_weapons(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);

private slots:

    void slot_table_header_customContextMenuRequested(const QPoint &points);

    void slot_action_manage_column_clicked(QAction *current_clicked_action);

    void slot_pushButton_next_clicked();
    void slot_pushButton_back_clicked();


    void slot_fs_tablewidget_single_click(int row, int column);
    void slot_fs_tablewidget_double_click(int row, int column);


    void slot_file_system_menu_right_clicked(QAction *current_clicked_action);
    void slot_pushbutton_add_notes_clicked(QString text);
    void slot_pushbutton_add_notes_to_bookmark_clicked(QString text);

    void slot_sub_menu_open_with_clicked(QAction *current_clicked_action);
    void slot_sub_menu_sendToBucket_clicked(QAction *current_clicked_action);


    void slot_show_add_file_to_hashset();
    void slot_sub_menu_add_files_to_hashset_clicked(QAction *current_clicked_action);
    void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);
    void slot_dir_navigator_pushbutton_clicked(QString dir_path);


    void slot_show_tag();
    void slot_sub_menu_tags_clicked(QAction *current_clicked_action);
    void slot_action_remove_tags_clicked();
    void slot_action_add_tags_clicked(QString tag_data, QString colour);
    void slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour);
    void slot_tag_list_after_remove_multiple_tag(QStringList list_tags_remained, QString table_name, QString record);

    void slot_arrow_key_pressed();
    void slot_bookmark_key_pressed_tablewidget();

    void slot_proc_generic_finished(int);
    void slot_sorting_tablewidget_display(int column_index);
    void slot_ctrl_e_key_press_on_tablewidget();
    void slot_removed_tags_list_file_system(QStringList list_removed_tags, QString table_name, QString record_no);
    void slot_sub_menu_open_detailed_info_clicked(QAction *current_clicked_action);
    void slot_update_tags_in_actions_menu_without_signal_emit(bool status, QString new_tag, QString tag_colour);
    void slot_update_hashset_submenu_of_file_system(QString db_name, QString db_path);

    void slot_go_to_file_system_record(QString filepath, QString record, QString source_c_name);

    void slot_pushbutton_showall_clicked();
    void slot_pushbutton_search_clicked();

    void slot_fs_double_clicked_dir_extraction_completed(QString dir_path);
    void slot_dir_navigation_button_hover_enter_or_leave(bool bool_hover_enter_or_leave, m_hovered_pushbutton_with_signal *dir_button);
    void slot_dt_search_keywords_list_selection_done_clicked();

    void slot_pushbutton_search_filters_clicked();
    void slot_show_files_recursively_clicked(bool value);
    void slot_hide_loading_display_dialog_box(bool status);

    void slot_act_quick_look_triggered();
    void slot_sub_menu_open_file_with_other_app_clicked(QAction *current_clicked_action);
    void slot_main_tab_widget_current_changed(int index);
    void slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1);
    void slot_ctrl_a_key_press_on_tablewidget();

    void slot_on_pushbutton_clicked_export_as_csv(bool status);
    void slot_start_write_fs_data_in_csv(QString file_path, QString size_str, bool bool_recursive_checked_for_csv_only, bool bool_current_dir_state , QString info_file_path, QString export_as_type);

    void slot_add_file_to_text_indexing_clicked(bool status);
    void slot_bookmark_updation_finished();
    void slot_tablewidget_current_cell_changed(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void slot_spacebar_quicklook_key_pressed_on_tablewidget();
    void slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info f1);
    void slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path);

    void slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip, QString password_enc_zip);

    void slot_thread_zip_file_export_finished();

    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
    void slot_sub_menu_carve_clicked(QAction *current_clicked_action);

    void slot_r_case_apple_metadata_running_status(bool status);
    void slot_r_case_exif_metadata_running_status(bool status);
    void slot_r_case_signature_analysis_running_status(bool status);
    void slot_r_case_mime_type_analysis_running_status(bool status);
    void slot_r_case_hashset_running_status(bool status);
    void slot_r_case_face_analysis_running_status(bool status);
    void slot_act_face_search_triggered(bool status);
    void slot_r_case_optical_character_recognition_running_status(bool status);
    void slot_r_case_weapons_running_status(bool status);
    void slot_r_case_fire_analysis_running_status(bool status);
    void slot_r_case_skin_tone_detection_running_status(bool status);

    void slot_sub_menu_run_fs_modules_clicked(QAction *current_clicked_action);
    void slot_sub_menu_export_hashes_as_vic_clicked(QAction *current_clicked_action);
    void slot_abouttoshow_tags_in_submenu();
    void slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1);

    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);
    void slot_action_search_file_with_same_hash_clicked();

    void slot_file_system_export_as_clicked(QAction *current_clicked_action);
    void slot_export_hashes_as_csv_clicked();
    void slot_export_hashes_as_sqlite_clicked();
    void slot_export_hashes_as_vic_clicked();
    void slot_gallery_view_invalid_tem_clicked();

    void slot_table_widget_header_item_clicked(int column_index);

    void slot_act_app_related_files_triggered(bool status);

    void slot_process_video_thumbnails_finished(int);

    void slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_path);

    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

private:

    Ui::file_system *ui;

    struct struct_search_column_text_for_file_system
    {
        int column_number;
        QString search_text;
    };

    enum enum_file_system_display
    {
        enum_File_System_display_BOOKMARK = 0,
        enum_File_System_display_NOTES_ICON,
        enum_File_System_display_TAG_ICON,
        enum_File_System_display_SEEN,
        enum_File_System_display_INT,
        enum_File_System_display_INODE_NO,
        enum_File_System_display_FILENAME,
        enum_File_System_display_EXTENSION,
        enum_File_System_display_FILESIZE,
        enum_File_System_display_DATE_MODIFIED,
        enum_File_System_display_DATE_CHANGE,
        enum_File_System_display_DATE_ACCESSED,
        enum_File_System_display_DATE_CREATED,
        enum_File_System_display_Last_Used_Date,
        enum_File_System_display_Use_Count,
        enum_File_System_display_Date_Added,
        enum_File_System_display_Content_Creation_Date,
        enum_File_System_display_Content_Modification_Date,
        enum_File_System_display_FILEPATH,
        enum_File_System_display_MIMETYPE,
        enum_File_System_display_HASHSET_NAME,
        enum_File_System_display_HASH_MD5,
        enum_File_System_display_HASH_SHA1,
        enum_File_System_display_DECOMPRESSION_STATUS,
        enum_File_System_display_PERMISSION_RESTRICTED,
        enum_File_System_display_OCR_TEXT,
        enum_File_System_display_SOURCE_COUNT_NAME,
        enum_File_System_display_NOTES_VALUE,
        enum_File_System_display_TAG_VALUE,
        enum_File_System_display_OS_SCHEME,
        enum_File_System_display_BLANK_COLUMN
    };

    enum enum_File_System_fixed_checks
    {
        enum_File_System_Spacer_Width_int = 700,
        enum_File_System_Spacer_Height_int = 10,
    };

    enum enum_File_System_Main_TabWidget_Indexes
    {
        enum_File_System_Main_TabWidget_Indexes_TABLE_VIEW = 0,
        enum_File_System_Main_TabWidget_Indexes_GALLERY_VIEW
    };



    QList<struct_global_hashset_db_data> hashset_db_data_list;
    void create_gui();

    QMenu *menu_manage_column;

    typedef struct{
        int hid_col_no;
        QString hid_col_name;
    }struct_manage_column_info;

    QList<struct_manage_column_info> get_hidden_column_from_manage_column_db();


    m_tablewidget *m_file_system_tablewidget_obj;

    QTabWidget *main_tab_widget_obj;

    gallery_viewer *gallery_viewer_obj;

    void set_fs_display(QString received_command, QString parent_dir_path, bool clear_previous_data, bool is_recursive_display);

    QString source_path;

    QString deepest_path;

    void set_buttons();

    QString extract_command_type(QString attr);


    QPushButton *pushButton_back;
    QPushButton *pushButton_next;

    // QString metadata;
    QString db_path_filesystem;
    QString db_path_signature;
    QString db_path_mime;
    QString db_path_exif_metadata;
    QString db_path_apple_metadata;
    QString table_name;

    QMenu   *mainMenu;
    QMenu   *submenu_open_with;
    QMenu   *submenu_send_to_bucket;
    QMenu   *submenu_add_files_to_hashset_db;
    QMenu   *submenu_open_files_with_other_app;
    QMenu   *submenu_run_fs_modules;
    QMenu   *menu_export_as;


    QAction *act_export;

    void create_right_click_actions(QMenu *mainMenu, QStringList submenuList);

    int bookmark_index;
    int note_icon_index;
    QString pluginName;
    add_notes *add_notes_to_singal_record_obj;
    add_notes *add_notes_to_bookmarks_obj;

    bool bool_message_box_yes_button_clicked;
    bool bool_message_box_clicked;

    void action_add_note_triggered();
    void action_remove_note_triggered();
    void action_add_note_to_bookmark_triggered();
    void action_remove_note_bookmark_triggered();
    void action_open_with_plist_viewer_triggered();
    void action_send_to_bucket_registry_viewer_triggered();
    void action_open_with_registry_viewer_triggered();
    void action_send_to_bucket_plist_viewer_triggered();
    void action_send_to_bucket_hex_viewer_triggered();
    void action_send_to_bucket_sqlite_viewer_triggered();
    void action_send_to_bucket_log_viewer_triggered();
    void action_open_with_log_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_export_triggered();
    void action_open_in_full_triggered();
    void action_open_in_detach_triggered();
    void action_submenu_tag_triggered(QString tag_name);

    QMenu *create_right_click_submenu(QString submenuStr, QMenu *mainMenu, QMenu *subMenu);
    QAction *create_right_click_action_for_enable_disable(QString submenuStr, QMenu *mainMenu, QAction *action);
    bool check_entry_on_row_is_directory(int selected_row);

    message_dialog  *message_dialog_obj;
    hashset_storage *hashset_storage_obj;
    hashset_display *hashset_display_obj;

    void display_hashset_add_files_submenu();
    void action_open_with_sqlite_viewer_triggered();

    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QMenu   *sub_menu_tags;

    QAction *act_add_file_to_text_indexing;
    QAction *act_carving_files;
    QAction *act_carving_data;

    add_tags *add_tags_module_obj;
    remove_multiple_tag_selecotor *remove_multiple_tag_selecotor_obj;

    QString main_db_cmd;
    QString main_initial_db_cmd;

    void display_tags_submenu();
    void update_tag_status_for_current_record(QString tag_data, QString colour);
    int  get_preview_index_for_current_record(QString current_media_path, QString display_file_path);
    void update_bookmarks_status_on_checkbox_click(int row, int column);

    QVBoxLayout *vlayout_main;
    QHBoxLayout *hlayout_back_next_button;
    QHBoxLayout *hlayout_frame_dir_navigation;
    QScrollArea *frame_dir_navigation;

    QString case_confg_dbpath;

    ///List names
    QStringList list_search_types;
    QStringList list_search_type_names_search_items;
    QStringList list_search_type_kind_search_items;
    QStringList list_search_type_kind_images_search_items;
    QStringList list_search_type_kind_music_search_items;
    QStringList list_search_type_kind_textfiles_search_items;
    QStringList list_search_type_date_search_items;
    QStringList list_search_type_date_days_search_items;
    QStringList list_search_type_options_search_items;
    QStringList list_search_type_compare_search_items;
    QStringList list_search_type_size_filters_items;
    QStringList list_search_type_system_search_items;
    QStringList list_search_type_visible_search_items;

    int last_save_index;

    bool bool_proc_generic_finished;

    ///ADD
    int get_main_combobox_index(QLayoutItem *item_combobox);

    QProcess *prc_generic;
    QString run_command_generic(QString command, QStringList arguments);

    QStringList get_applied_tags_name_on_current_record(QString record_no);

    void action_bookmark_triggered();
    void action_remove_bookmarks_triggered();
    void action_extract_compressed_file();

    bool eventFilter(QObject *obj, QEvent *event);
    void tablewidget_general_clicked(int row, int column);

    QString virtual_source_path;
    QString fs_extraction_path;

    QString os_naming_scheme_internal;
    QString os_scheme_display;

    QString source_count_name;
    QString extract_attribute_name(QString attr);

    void action_copy_to_clipboard();
    bool bool_sort_ascending;

    bool bool_did_tableview_display_changed;

    QString current_parent_dir_path;
    QPushButton *pushbutton_search_filters_for_non_hfs;
    QPushButton *pushbutton_show_files_recursively;

    QMenu       *menu_search_filters;

    QLineEdit *lineEdit_search;
    QPushButton *pushButton_search,*pushbutton_show_all;

    QStringList headerlist;
    qint64 get_header_column_index(QString header);

    QSqlDatabase destination_db_obj;
    //  QSqlDatabase *fs_db_temp_ptr;

    struct_GLOBAL_witness_info_source struct_target_source_info;

    int font_size_of_spotlight_search_int;
    QString stylesheet_pushbutton_str;

    keywords_list_selector *keywords_list_selector_obj;

    void change_textcolor_for_seen_unseen(qint64 current_row, bool is_default_color);
    void action_mark_as_seen_unseen(QString current_action_text);

    QMimeDatabase mime_database_obj;
    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    void loading_display_dialog_based_on_records(qint64 total_records, qint64 processed_count);
    bool bool_cancel_loading ;
    QAction *act_quick_look;

    void add_applications_to_open_application_menu();

    QList<struct_global_gallery_view_file_info> st_gallery_view_file_info_list;

    QPushButton *pushbutton_export_as_csv;
    export_dialog_box_3 *export_dialog_box_3_obj;

    QString specified_file_path;

    void open_export_as_FS_csv_file(QString file_path);

    QString get_command_for_export_as_csv(bool is_recursive);
    void update_bookmarks_in_bookmark_serach(bool updation_status, QList<struct_global_tagging_tag_notes_db_updation> list_obj);

    thread_db_updation_bookmark *thread_db_updation_bookmark_obj;
    QThread *qthread_bookmark_db_updation_obj;

    global_connection_manager *global_connection_manager_obj;

    QString current_path_for_mainwindow_check;

    void export_as_csv_for_screen_items(QTextStream &outputfile_csv, struct_GLOBAL_witness_info_source struct_info);

    file_export_dialog *file_export_dialog_obj;

    thread_zip_file_export *thread_zip_file_export_obj;
    QString get_current_complete_file_path();

    recon_helper_standard *recon_helper_standard_obj;

    void change_textcolor_for_compression_decompression(qint64 current_row, bool bool_change_color);

    m_hovered_pushbutton_with_signal *dir_navigation_button_array[200];

    decompress_file_system *decompress_file_system_obj;

    R_Treewidget *treewidget_case_display_obj;

    bool bool_run_fs_modules_status = false;
    bool bool_run_fs_modules_apple_metadata_status = false;
    bool bool_run_fs_modules_exif_metadata_status = false;
    bool bool_run_fs_modules_signature_analysis_status = false;
    bool bool_run_fs_modules_mime_type_analysis_status = false;
    bool bool_run_fs_modules_hashset_status = false;

    QList<struct_global_run_fs_modules_details> run_fs_modules_details_list;
    void action_run_fs_modules_apple_metadata_clicked();
    void action_run_fs_modules_exif_metadata_clicked();
    void action_run_fs_modules_signature_analysis_clicked();
    void action_run_fs_modules_mime_type_analysis_clicked();
    void action_run_fs_modules_hashset_clicked();
    void prepare_fs_run_module_files_list(QString status_column_name, QString feature_name);


    QMenu   *submenu_export_hashes_as;
    export_hashes_as_vic *export_hashes_as_vic_obj;
    export_hashes_as_sqlite *export_hashes_as_sqlite_obj;
    export_hashes_as_csv *export_hashes_as_csv_obj;
    QFileDialog file_dialog_obj;
    QList<struct_global_export_as_hashes_details> export_as_vic_details_list;
    global_recon_file_info *global_recon_file_info_obj;

    bool check_selected_row_is_decompressed();
    export_rsync *export_rsync_obj;

    bool check_fs_record_hidden_for_hashset(QStringList &hashes_hidden_list, QStringList matched_hashset_list);
    QStringList get_hashsets_list_to_be_hidden();
    void prepare_hashset_db_data_list();
    QString get_recon_filefrom_of_selected_row();
    struct_global_set_metadata_info st_set_meta_obj;
    bool bool_cell_changed_connection_buildup_for_tablewidget = false;
    QStringList bookmarked_records_from_table_display_list;

    void prepare_hidden_column_list_from_db();

    QList<QString> hide_column_list;
    QList<int> hidden_column_header_name_index_list;
    QList<int> hidden_column_header_data_index_list;
    QStringList export_as_csv_data_list;
    QStringList export_as_csv_header_list;

    search_files_with_hash *search_files_with_hash_obj;
    QAction *action_search_file_with_same_md5_hash;

    void point_to_export_dir(QString path);
    QString folder_export_dir_path;
    QString zip_export_file_path;
    bool bool_folder_export_done = false;
    bool bool_zip_export_done = false;

    void export_data_as_csv(QStringList csv_headerlist, QString file_path, bool bool_radio_button_screen_item_status, bool bool_recursive_checked_for_csv_only, struct_GLOBAL_witness_info_source struct_info, QStringList hashsets_to_be_hidden, QString info_file_path);
    void export_data_as_sqlite(QStringList csv_headerlist, QString file_path, bool bool_radio_button_screen_item_status, bool bool_recursive_checked_for_csv_only, struct_GLOBAL_witness_info_source struct_info, QStringList hashsets_to_be_hidden,QString info_file_path);
    void write_source_file_data_for_export_as_csv_and_sqlite(QString info_file_path, struct_GLOBAL_witness_info_source struct_info, QString parent_category, QString is_recurcive, QString screen_item);
    void export_as_sqlite_for_screen_items(QSqlDatabase &dest_exp_db, struct_GLOBAL_witness_info_source struct_info, QString insert_command_str);

    void change_row_background_color_for_last_used_date(qint64 current_row);
    RECON_case *recon_case_obj;
    facefinder *facefinder_obj;

    QAction *act_face_search;

    bool bool_run_fs_modules_face_analysis_status = false;
    void action_run_fs_modules_face_analysis_clicked();

    bool bool_run_fs_modules_optical_character_recognition_status = false;
    void action_run_fs_modules_optical_character_recognition_clicked();

    bool bool_run_fs_modules_weapons_status = false;
    void action_run_fs_modules_weapons_clicked();

    bool bool_run_fs_modules_fire_analysis_status = false;
    void action_run_fs_modules_fire_analysis_clicked();

    bool bool_run_fs_modules_skin_tone_detection_status = false;
    void action_run_fs_modules_skin_tone_detection_clicked();

    QAction *act_app_related_file;
    app_related_files *app_related_files_obj;

    bool bool_process_thumbnails_finished = false;
    recon_helper_process *recon_helper_process_obj;
    QString dmg_export_file_path;
    bool bool_dmg_export_done = false;

    QMenu *submenu_bookmark;
    QMenu *submenu_add_note;
    QMenu *submenu_carve;
    QMenu *submenu_seen;

    void action_carve_files_clicked();
    void action_carve_data_clicked();

    custom_table_header_widget *custom_table_header_widget_obj;
    QPushButton *pushbutton_show_column;
    QList<custom_table_header_widget *> custom_header_widgets_QList;

    void all_column_text_search(QList<struct_search_column_text_for_file_system> search_text_all_column_list);
    void refresh_horizontal_header();
    void clear_all_custom_header_widget_entries();
    void prepare_gallery_view_list(int row);

    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;
    bool recursive_view_status = false;
};

#endif // File_System_H
