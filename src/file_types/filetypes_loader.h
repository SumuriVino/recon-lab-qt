#ifndef FILETYPES_LOADER_H
#define FILETYPES_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"

#include "header_identical_1.h"
# include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "gallery_viewer/gallery_viewer.h"

#include "file_export_dialog.h"
#include "export/export_except_reports/export_zip/thread_zip_file_export.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "export_as_KML/export_as_kml.h"
#include "file_search/search_files_with_hash/search_files_with_hash.h"
#include "export/export_hashes/export_hashes_as_sqlite/export_hashes_as_sqlite.h"
#include "export/export_hashes/export_hashes_as_csv/export_hashes_as_csv.h"
#include "face_extractor/facefinder.h"
#include "recon_generic_functions/recon_helper_process.h"

class filetypes_loader : public table_result_loader
{

    Q_OBJECT

public:
    explicit filetypes_loader(QWidget *parent);
    ~filetypes_loader();

    void configure_filetypes_loader(QString result_dir_path, QString filetype, QString case_tree_type_dsply_name, qint64 records_count, QTreeWidgetItem *currentItem);
    void pub_populate_data_in_table();

    void pub_set_essentials();

    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str);

    void pub_set_face_finder_obj(facefinder *obj);

signals:

    void signal_update_tag_search_for_bookmark(QString record_no_prepended_zero, QString fs_plugin_name, QString fs_tab_name, QString status, QStringList applied_tags_list, QString fs_db_path, QString os_name_str, QString feature_name);
    void signal_update_tag_search_for_notes(QString record_no_prepended_zero, QString fs_plugin_name, QString fs_tab_name, QString status, QStringList applied_tags_list, QString os_scheme, QString feature_name);
    void signal_add_notes_from_file_system(QString record_no, QString tab_name, QString notes_db_path, QString notes_text, QString destination_db_path, QString os_scheme);

    void signal_update_tag_search_db_and_display_on_bookmark(QStringList record_no_list, QString source_count_name, QString bookmark_status, QString plugin_name_str);
    void signal_act_quick_look_triggered();
    void signal_add_saved_maps_in_case_tree();
    void signal_carve_files_clicked(QString source_count_name, QString file_path, bool bool_carve_files);
    void signal_carve_data_clicked(QString source_count_name, QString file_path, QString plugin_name, QString tab_name, QString record_no);
    void signal_run_fs_module_apple_metadata_already_running_status();
    void signal_run_fs_module_exif_metadata_already_running_status();
    void signal_run_fs_module_signature_analysis_already_running_status();
    void signal_run_fs_module_mime_type_analysis_already_running_status();
    void signal_run_fs_module_hashset_already_running_status();

    void signal_run_fs_module_face_analysis_already_running_status();
    void signal_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_optical_character_recognition_already_running_status();
    void signal_run_fs_module_weapons_already_running_status();
    void signal_run_fs_module_fire_analysis_already_running_status();
    void signal_run_fs_module_skin_tone_detection_already_running_status();

    void signal_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_hashset(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_weapons(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list);
    void signal_add_tags_carved_index_files(QString tag_name,QString colour_name);


private slots:

    void slot_table_header_customContextMenuRequested(const QPoint &points);
    void slot_action_manage_column_clicked(QAction *current_clicked_action);

    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_add_notes_to_bookmark_pushbutton_clicked(QString text);
    void slot_open_with_submenu_clicked(QAction *current_clicked_action);
    void slot_sendtobucket_submenu_clicked(QAction *current_clicked_action);

    void slot_add_files_to_hashset_submenu_clicked(QAction *current_clicked_action);
    void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);
    void slot_act_create_hashset();
    void slot_action_search_file_with_same_hash_clicked();
    void slot_add_tags(QString tag_data, QString colour);
    void slot_remaining_tags_list(QStringList list_tags_remained, QString table_name, QString record);
    void slot_tag_list_to_be_removed(QStringList list_removed_tags, QString table_name, QString record_no);
    void slot_act_remove_tags();
    void slot_open_detailed_info_submenu_clicked(QAction *current_clicked_action);

    void slot_pushbutton_search_clicked();
    void slot_pushbutton_showall_clicked();

    void slot_link_label_searched_filters(QString value);
    void slot_hide_loading_display_dialog_box(bool cancel_status);

    void slot_act_quick_look_triggered();
    void slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1);
    void slot_main_tab_widget_current_changed(int index);
    void slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1);

    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path);
    void slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip, QString password_enc_zip);
    void slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_path);
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);

    void slot_thread_zip_file_export_finished();
    void slot_sub_menu_open_file_with_other_app_clicked(QAction *current_clicked_action);
    void slot_tableWidget_doubleClicked(int row, int column);
    void slot_sub_menu_carve_clicked(QAction *current_clicked_action);

    void slot_cancel_export_as_KML_report();
    void slot_continue_export_as_KML_report();

    void slot_sub_menu_export_hashes_as_clicked(QAction *current_clicked_action);
    void slot_link_label_apple_metadata_searched_filters(QString value);
    void slot_r_case_apple_metadata_running_status(bool status);
    void slot_r_case_exif_metadata_running_status(bool status);
    void slot_r_case_signature_analysis_running_status(bool status);
    void slot_r_case_mime_type_analysis_running_status(bool status);
    void slot_r_case_hashset_running_status(bool status);
    void slot_r_case_face_analysis_running_status(bool status);
    void slot_r_case_optical_character_recognition_running_status(bool status);
    void slot_r_case_weapons_running_status(bool status);
    void slot_r_case_fire_analysis_running_status(bool status);
    void slot_r_case_skin_tone_detection_running_status(bool status);

    void slot_sub_menu_run_fs_modules_clicked(QAction *current_clicked_action);
    void slot_sort_tablewidget_display(int column_index);
    void slot_bookmark_key_pressed_tablewidget();
    void slot_tablewidget_indexed_carved_files_current_cell_changed(int cur_row, int cur_col, int pre_row, int pre_col);
    void slot_sub_menu_carved_index_files_open_file_with_other_app_clicked(QAction *current_clicked_action);
    void slot_right_click_carved_index_files_main_menu_triggered(QAction *action);
    void slot_act_remove_tags_carved_index_files();
    void slot_act_create_tags_carved_index_files();
    void slot_update_tags_submenu_carved_index_files(bool status, QString new_tag, QString tag_colour);
    void slot_add_tags_carved_index_files(QString tag_data, QString colour);
    void slot_tags_submenu_carved_index_files_clicked(QAction *current_clicked_action);
    void slot_abouttoshow_tags_in_submenu_for_carved_index_files_clicked();
    void slot_export_carved_index_files_triggered();

    void slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_invalid_tem_clicked();

    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);
    void slot_export_hashes_as_csv_clicked();
    void slot_export_hashes_as_sqlite_clicked();
    void slot_export_hashes_as_vic_clicked();

    void slot_table_widget_header_item_clicked(int column_index);

    void slot_act_face_search_triggered(bool status);
    void slot_action_right_click_remove_record();
    void slot_label_link_searched_face_linkactivated(QString label_text);
    void slot_process_video_thumbnails_finished(int);

    void slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info f1);

    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

private:

    struct struct_search_column_text_for_file_type_loader
    {
        int column_number;
        QString search_text;
    };

    struct selected_file_info {
        QString record_no;
        QString os_sceheme;
        QString source_count_name;
        QString file_name;
        QString file_path;
        QString date_created;
        QString date_modified;
        QString date_accessed;
    };

    enum enum_file_types_display_fields
    {
        enum_Bookmarks_Index = 0,
        enum_Notes_Icon_Index, /// only for display column //1
        enum_Tag_Icon_Index, //2
        enum_Record_No_Index,//3
        enum_Inode_No_Index, //4
        enum_File_Name_Index,//5
        enum_Extension_Index,//6
        enum_File_Size_Index,//7
        enum_Date_Modified_Index,//8
        enum_Date_Change_Index,//9
        enum_Date_Accessed_Index,//10
        enum_Date_Created_Index,//11
        enum_Last_Used_Date_Index,//12
        enum_Use_Count_Index,//13
        enum_Date_Added_Index,//14
        enum_Content_Creation_Date_Index,//15
        enum_Content_Modification_Date_Index,//16
        enum_Mime_Type_Index,//17
        enum_Apple_Metadata_Column_Index,//18
        enum_Apple_Latitude_Index,//19
        enum_Apple_Longitude_Index,//20
        enum_WhereFrom_Index,//21
        enum_Exif_Metadata_Column_Index,//22
        enum_Exif_Make_Index,//23
        enum_Exif_Model_Index,//24
        enum_Exif_Latitude_Index,//25
        enum_Exif_Longitude_Index,//26
        enum_Signature_Name_Index,//27
        enum_Signature_Value_Index,//28
        enum_Hashset_Name_Index,//29
        enum_Hash_MD5_Index,//30
        enum_Hash_SHA1_Index,//31
        enum_Content_search_Number_of_hits,//32
        enum_Content_search_Keyword_Hit,//33
        enum_Content_search_Word_Count,//34
        enum_Content_search_Hit_Locations,//35
        enum_Source_Count_Name_Index,//36
        enum_Source_Name_Index,//37
        enum_OCR_Text_Index,//38
        enum_File_Path_Index,//39
        enum_File_Decompression_Status_Index,//40
        enum_Notes_Value_Index, /// for notes value //41
        enum_Blank_Column_Index //42


    };

    enum enum_File_Types_Main_TabWidget_Indexes
    {
        enum_File_Types_Main_TabWidget_Indexes_TABLE_VIEW = 0,
        enum_File_Types_Main_TabWidget_Indexes_GALLERY_VIEW,
        enum_File_Types_Main_TabWidget_Indexes_CARVED_FILES
    };

    enum enum_indexed_carved_files_display_fields
    {
        enum_indexed_carved_files_Bookmarks_Index = 0,
        enum_indexed_carved_files_Notes_Icon_Index, /// only for display column
        enum_indexed_carved_files_Tag_Icon_Index,
        enum_indexed_carved_files_Record_No_Index,
        enum_indexed_carved_files_File_Name_Index,
        enum_indexed_carved_files_File_Size_Index,
        enum_indexed_carved_files_Extension_Index,
        enum_indexed_carved_files_Number_of_hits,
        enum_indexed_carved_files_Keyword_Hit,
        enum_indexed_carved_files_Word_Count,
        enum_indexed_carved_files_Hit_Locations,
        enum_indexed_carved_files_File_Path_Index,
        enum_indexed_carved_files_Source_Count_Name_Index,
        enum_indexed_carved_files_Source_Name_Index,
        enum_indexed_carved_files_Notes_Value_Index, /// for notes value
        enum_indexed_carved_files_Blank_Column_Index
    };

    typedef struct{
        int hid_col_no;
        QString hid_col_name;
    }struct_manage_column_info;

    QList<struct_manage_column_info> get_hidden_column_from_manage_column_db();





    QList<struct_global_hashset_db_data> hashset_db_data_list;


    gallery_viewer *gallery_viewer_obj;
    bool bool_did_tableview_display_changed = false;
    QList<struct_global_gallery_view_file_info> st_gallery_view_file_info_list;

    QString file_type_child;
    QString file_type_parent;
    QString filetypes_destination_dir_path;


    int filepath_index;

    QString current_signature;

    void set_display_db_path(QString db_dir_path);

    QMenu *menu_manage_column;



    void display_data();
    void tablewidget_general_clicked(int row, int column);
    void contextMenuEvent(QContextMenuEvent *evt);
    void action_bookmark_triggered();
    QString get_record_number_from_selected_row(int selected_row_no);
    QString get_source_count_name_from_selected_row(int selected_row_no);
    void update_bookmark_value(QString status, int row, int column);
    void action_remove_bookmark_triggered();
    void action_bookmark_all_triggered();
    void update_bookmark_status_for_all(QString status);
    void action_remove_bookmark_all_triggered();
    void action_add_note_triggered();
    void update_notes_value(QString text, int current_row);
    void action_remove_note_triggered();
    void action_add_note_to_bookmark_triggered();
    void action_remove_note_bookmark_triggered();
    void action_remove_all_notes_triggered();
    void action_export_triggered();
    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_open_with_log_viewer_triggered();
    void action_sendToBucket_log_viewer_triggered();
    void action_sendToBucket_plist_viewer_triggered();
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void update_tags_value(QString tag_data, QString colour);
    void action_open_full_detailed_info_triggered();
    void action_open_detach_detailed_info_triggered();
    void action_sendToBucket_registry_viewer_triggered();
    void action_open_with_registry_viewer_triggered();

    void update_notes_value_in_bookmark_for_filetypes(QString notes_text, bool is_remove_note);

    QStringList get_applied_tags_name_on_current_record(QString record_no, QString fs_db_path);
    void action_goto_source_triggered();
    bool check_record_already_exists_in_display(QString filename_str, QString INT_str , QString source_count_name);

    QPushButton *pushbutton_search_filters;
    QMenu       *menu_search_filters;


    qint64 get_header_column_index(QString header);

    void change_background_for_seen_unseen(qint64 current_row, bool is_default_color);
    void action_mark_as_seen_unseen(QString clicked_action_text);
    QString get_file_size_display_command(QString file_size_category);

    QString exif_make_str , exif_model_str , exif_author_str;

    QSqlDatabase in_memory_fs_db;
    QSqlDatabase in_memory_exif_db;


    void prepare_display_for_filetypes(QSqlDatabase &destination_db, QString fs_db_path, QString received_cmd, QString source_count_name, QString m_caller);
    bool attach_fs_db_in_memory(QString fs_db_path);

    qint64 extract_record_count_from_case_tree_display(int feature_position, QString feature_name, QString sub_feature_name);

    QTextEdit *text_edit_search_filters;
    QLabel *label_text_edit_show_link;
    QAction *act_add_file_to_text_indexing_queue;
    QAction *act_carving_files;
    QAction *act_carving_data;

    void set_data_in_text_edit_searched_filter(QString clicked_text);
    QString get_text_edit_display_str_for_spotlight_filters(QString selected_mdfind_str);
    QString get_text_edit_string_for_dates(QString entered_date_str, QString header_name);
    QString get_widget_type_name_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator);
    QString get_widget_type_range_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator);
    QString get_widget_type_duration_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator);
    QString get_widget_type_size_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator);
    QString get_widget_type_kind_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator);
    QString get_widget_type_date_filter_display_string(QString selected_value, QString attr_display_name, QString joined_operator);

    void disable_widgets(bool);
    void loading_display_dialog_based_on_records(int total_records , int d_count );
    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    bool bool_cancel_loading;

    QMenu   *submenu_run_fs_modules;

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
    void prepare_fs_run_module_files_list(QString status_column_name);

    m_tablewidget *m_table_carved_index_files;
    QMenu *menu_carved_index_files_search_filters ;

    QAction *act_carved_index_files_quick_look;
    QAction *act_carved_index_files_export;
    QAction *act_carved_index_files_full_detail_info;

    QAction *act_create_tag_carved_index_files;
    QAction *act_remove_tag_carved_index_files;
    QAction *act_already_saved_tags_carved_index_files;

    QMenu *menu_carved_index_files_right_click;
    QMenu *submenu_carved_index_files_open_with;
    QMenu *submenu_carved_index_files_send_to_bucket;
    QMenu *submenu_carved_index_files_open_with_external_application;
    QMenu *sub_carved_index_files_menu_tags;

    add_tags *add_tags_module_carved_index_files_obj;

    void action_export_as_kml_clicked();
    void action_text_indexing_queue_clicked();

    file_export_dialog *file_export_dialog_obj;

    thread_zip_file_export *thread_zip_file_export_obj;

    void add_applications_to_open_application_menu();
    QString get_current_complete_file_path();
    void action_extract_compressed_file();

    recon_helper_standard *recon_helper_standard_obj;

    void change_textcolor_for_compression_decompression(qint64 current_row, bool bool_change_color);

    decompress_file_system *decompress_file_system_obj;

    export_as_KML *export_as_KML_dialog_box_obj;
    exif_data_image_files *exif_data_image_files_obj;
    QStringList make_kml_report();


    QMenu   *submenu_export_hashes_as;
    export_hashes_as_vic *export_hashes_as_vic_obj;
    export_hashes_as_sqlite *export_hashes_as_sqlite_obj;
    export_hashes_as_csv *export_hashes_as_csv_obj;
    QList<struct_global_export_as_hashes_details> export_as_hashes_details_list;
    QFileDialog file_dialog_obj;

    void set_apple_metadata_searched_filters_in_textedit();
    QLabel *label_searched_apple_metadata_filters_show_link;
    QTextEdit *text_edit_search_apple_metadata_filters;

    void prepare_hashset_db_data_list();
    QStringList get_hashsets_list_to_be_hidden();
    bool check_fs_record_hidden_for_hashset(QStringList &hashes_hidden_list, QStringList matched_hashset_list);
    void prepare_display_for_carved_files_content_search(QSqlDatabase &destination_db);
    bool check_is_carved_files_content_search_record(QString record_str);
    void prepare_indexed_carved_files_tab_display();
    void tablewidget_indexed_carved_files_general_clicked(int row, int column);
    void add_applications_to_open_application_menu_for_carved_index_files();
    void action_carved_index_files_bookmark_clicked();
    void action_carved_index_files_remove_bookmark_clicked();
    void action_carved_index_files_add_note_clicked(QString note_text);
    void action_carved_index_files_remove_note_clicked();
    QString get_carver_label_from_filepath(QString file_path);
    QString get_carver_record_no_from_filepath(QString file_path);
    void action_open_with_plist_viewer_carved_index_files_triggered();
    void action_open_with_hex_viewer_carved_index_files_triggered();
    void action_open_with_sqlite_viewer_carved_index_files_triggered();
    QString get_carver_db_path_from_file_path(QString file_path);
    void action_sendToBucket_plist_viewer_carved_index_files_triggered();
    void action_sendToBucket_hex_viewer_carved_index_files_triggered();
    void action_sendToBucket_sqlite_viewer_carved_index_files_triggered();
    void action_export_carved_index_files_triggered();
    void create_tags_submenu_for_carved_index_files();
    void update_tags_value_carved_index_files(QString tag_data, QString colour);
    void action_submenu_tag_carved_index_files_triggered(QString tag_name);
    void action_copy_to_clipboard_carved_index_files();
    void action_open_full_detailed_info_carved_index_files_triggered();
    void action_pushbutton_search_carved_index_files_clicked();
    void action_pushbutton_show_all_carved_index_files_clicked();

    QMap<QString,QStringList> map_bookmarked_records_from_table_display;
    search_files_with_hash *search_files_with_hash_obj;

    bool bool_columns_ajust_complete = false;

    void adjust_important_columns_according_to_feature_in_table_display();
    void change_row_background_color_for_last_used_date(qint64 current_row);

    facefinder *facefinder_obj;

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

    bool bool_act_remove_record = false;

    QLabel *label_link_searched_face;
    QLabel *label_picture_searched_face;
    bool bool_process_thumbnails_finished = false;

    recon_helper_process *recon_helper_process_obj;
    bool bool_dmg_export_done,bool_zip_export_done;
    QString zip_export_file_path,dmg_export_file_path;
    void point_to_export_dir(QString path);

    void action_carve_files_clicked();
    void action_carve_data_clicked();

    void clear_all_custom_header_widget_entries();
    void all_column_text_search(QList<struct_search_column_text_for_file_type_loader> search_text_all_column_list);
    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;
};

#endif // FILETYPES_LOADER_H
