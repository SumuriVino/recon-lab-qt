#ifndef TABLE_RESULT_LOADER_H
#define TABLE_RESULT_LOADER_H

#include <QWidget>
#include "ui_custom_classes/m_tablewidget.h"
#include <QtCore>
#include <QtGui>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QCheckBox>

#include <QtSql/QtSql>

#include <QMenu>
#include <QAction>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"

#include "add_tags/add_tags.h"
#include "add_tags/remove_multiple_tag_selecotor.h"
#include "add_notes/add_notes.h"
#include "QHeaderView"

#include "header_gui_settings.h"
#include "progress_bar/progress_bar_window.h"
#include "tsk/libtsk.h"
#include "file_viewer/plist_viewer/plist_viewer.h"
#include "file_viewer/sqlite_viewer/sqlite_viewer_starter.h"
#include "message_box/message_dialog.h"
#include "full_detailed_information/full_detailed_info.h"
#include "header_global_structures.h"
#include "header_identical_2.h"
#include "hashset/hashset_display.h"
#include "hashset/hashset_storage.h"
#include "file_system_viewer/file_system.h"
#include "header_global_enum.h"
#include "header_global_variables_1.h"
#include "file_viewer/text_viewer/text_viewer.h"
#include "ui_custom_classes/r_treewidget.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "gallery_viewer/gallery_viewer.h"
#include "global_connection_manager.h"
#include "recon_case/recon_case.h"
#include "global_recon_file_info/global_recon_file_info.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"
#include "export_dialog_box/export_dialog_box_2/export_dialog_box_2.h"



class table_result_loader : public QWidget
{
    Q_OBJECT
public:
    explicit table_result_loader(QWidget *parent = 0);
    ~table_result_loader();

    struct struct_search_column_text_for_file_type
    {
        int column_number;
        QString search_text;
    };
    QList<custom_table_header_widget *> custom_header_widgets_QList;
    void pub_refresh_horizontal_header();
    void pub_clear_all_custom_header_widget_entries();

    void pub_set_essentials();

    void pub_set_feature_name(QString name);
    void pub_set_display_tab_name_list(QStringList tabs_list);
    virtual void pub_create_ui();

    void pub_set_clicked_item_tab_text(QString tab_text);
    void pub_set_csv_dir_path(QString path);

    void pub_set_plugin_name(QString name);
    void pub_set_table_display_essentials();


    void pub_set_case_tree_obj(R_Treewidget *obj);

    void pub_set_global_manager_obj(global_connection_manager *obj);
    void pub_set_global_recon_file_info_obj(global_recon_file_info *obj);

    void pub_set_recon_case_obj(RECON_case  *obj);
signals:

    void signal_add_tags(QString tag_name,QString colour_name);
    void signal_change_bookmark_value_in_other_db(QString record_no,QString plugin_name,QString tab_name,QString status,QStringList applied_tags_list, QString fs_db_path, QString os_name_str, QString feature_name);
    void signal_change_notes_in_other_db_also(QString record_no,QString plugin_name,QString tab_name, QString notes_text,QStringList applied_tags_list, QString os_name_str, QString feature_name);
    void signal_show_file_in_viewer_table_result(QString viewer_child_name, QString viewer_name, QString source_count_name);
    void signal_viewer_extarction_completed_table_result(QString viewer_child_name, QString viewer_name);
    void signal_open_in_detach_clicked_table_result(QString metadata_str, QString export_file_path, QString current_media_path, int preview_index, QString searched_keyword_str, QString source_count_name);
    void signal_change_content_of_detach_window(QString metadata_str, QString export_file_path, QString current_media_path, int preview_index, QString searched_keyword_str, QString source_count_name);
    void signal_tablewidget_goto_record(QString plugin_name, QString parent_name, QString tab_name ,QString record_no, QString db_path, QString os_name_str);
    void signal_decompressed_files_view(QString plugin_name, QString parent_name, QString tab_name ,QString record_no, QString db_path, QString source_count_name);

    void signal_add_tag_in_other_locations(QString added_tag_name, QString is_emitted_from_plugin, QString plugin_name , QString tab_name , QString record_no , QString fs_db_path, QString os_name_str);
    void signal_update_tags_to_other_db(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme);
    void signal_update_sqlite_db_for_remained_tags(QString plugin_name, QString table_name, QString viewer_name, QString record_no, QStringList list_remained_tags, QString os_scheme);
    void signal_update_hex_and_text_db_for_removed_tags(QString plugin_name, QString start_index, QString end_index, QString viewer_name, QStringList list_remained_tags, QString os_scheme);
    void signal_hashset_create_db_finish(QStringList db_list);
    void signal_update_tag_submenu_of_plugins(bool status, QString new_tag, QString colour_name);
    void signal_table_widget_loader_removed_tags_list(QString plugin_name, QString tab_name, QString record_no, QStringList previous_tags_list, QString os_scheme);
    void signal_update_hashset_submenu_from_tablewidget_loader(QString db_name, QString db_path);
    void signal_go_to_artifact_source_in_file_system_from_loader(QString source_path, QString fs_db_path, QString source_name);
    void signal_quick_look_preview_filepath_from_table_result_loader(QString filepath);
    void signal_add_notes_from_result_loader_for_plugin(QString, QString, QString, QString, QString, QString, QString,QString,QString );
    void signal_add_notes_from_result_loader_for_fs(QString, QString, QString, QString, QString, QString);
    void signal_add_notes_from_result_loader_for_hex_viewer_entry(QString record_no, QString tab_name, QString os_name, QString start_offset, QString end_offset, QString file_path, QString clicked_tag_name, QString notes_db_path);
    void signal_add_notes_from_result_loader_for_plist_viewer(QString record_no, QString tab_name, QString os_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString viewer_display_name, QString note_text);
    void signal_add_notes_from_result_loader_for_screenshot(QString record_no, QString tab_name,  QString file_path, QString clicked_tag_name, QString notes_db_path, QString note_text);
    void signal_add_notes_from_result_loader_for_sqlite_viewer(QString record_no, QString tab_name, QString os_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString note_text);
    void signal_progress_count_for_statusbar_progress_bar(qint64 total_record_count,qint64 record_displayed,float progress_percent);
    void signal_statusbar_progress_bar_show_hide(bool status, QString message);

    void signal_update_display_on_remove_notes(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);
    void signal_add_notes_from_result_loader_for_content_search(QString record_no, QString tab_name, QString os_name, QString file_path, QString notes_db_path, QString source_db_path);
    void signal_change_notes_value_in_index_k_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString notes_text,QStringList applied_tags_list, QString index_k_search_db_path, QString os_scheme, QString feature_name);
    void signal_change_bookmark_value_in_index_k_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString notes_text,QStringList applied_tags_list, QString index_k_search_db_path, QString os_scheme, QString feature_name);
    void signal_change_tag_value_in_index_k_search_db_and_display(QString record_no,QString plugin_name,QString tab_name, QString notes_text , QString index_k_search_db_path, QString os_scheme, QString feature_name);

    void signal_disable_case_tree_sidebar(bool status);
    void signal_act_quick_look_triggered();
    void signal_common_add_tag_name_into_case_tree(QString tag_name);
    void signal_common_add_notes_name_into_case_tree(QString note_name);
    void signal_clear_all_metadata();
    void signal_set_metadata_info(struct_global_set_metadata_info m_obj);
    void signal_tablewidget_goto_record_snapshots(QString tab_name ,QString sanpshot_tempered_name,QString feature_name ,QString record_no);
    void signal_set_bookmark_status_to_gallery_view(bool bookmark_status);
    void signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj);
    void signal_save_openwith_changes_to_bucket_for_full_detail_info(QString viewer_type);
    void signal_right_click_removed_record_refresh_in_case_tree(QStringList db_path_list,QString file_type_parent);

private slots:
    void slot_spacebar_quicklook_key_pressed_on_tablewidget();
    void slot_process_video_thumbnails_finished(int);

protected slots:

    virtual void slot_right_click_main_menu_triggered(QAction *current_clicked_action);

    virtual void slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour);
    virtual void slot_act_create_tags();
    virtual void slot_act_remove_tags();
    virtual void slot_add_tags(QString tag_data, QString colour);
    virtual void slot_remaining_tags_list(QStringList list_tags_remained, QString table_name, QString record);

    virtual void slot_tablewidget_cell_clicked(int row, int column);
    virtual void slot_add_notes_pushbutton_clicked(QString text);

    virtual void slot_insert_record_in_tablewidget(QString record_str, QString plugin_name, QString tab_name, QString tag_name, QString os_scheme);

    virtual void slot_open_with_submenu_clicked(QAction *current_clicked_action);
    virtual void slot_sendtobucket_submenu_clicked(QAction *current_clicked_action);
    virtual void slot_pushbutton_search_clicked();

    virtual void slot_bookmark_key_pressed_tablewidget();
    virtual void slot_arrow_key_pressed_on_tablewidget();
    virtual void slot_control_e_key_pressed_on_tablewidget();
    virtual void slot_tags_submenu_clicked(QAction *current_clicked_action);
    virtual void slot_add_files_to_hashset_submenu_clicked(QAction *current_clicked_action);
    virtual void slot_act_create_hashset();

    virtual void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);
    virtual void slot_add_notes_to_bookmark_pushbutton_clicked(QString text);

    virtual void slot_action_search_file_with_same_hash_clicked();

    virtual void slot_pushbutton_showall_clicked();

    virtual void slot_update_tags_in_actions_menu_without_signal_emit(bool status, QString new_tag, QString tag_colour);
    virtual void slot_tag_list_to_be_removed(QStringList list_removed_tags, QString table_name, QString record_no);
    virtual void slot_sort_tablewidget_display(int column_index);

    virtual void slot_open_detailed_info_submenu_clicked(QAction *current_clicked_action);
    virtual void slot_update_hashset_submenu(QString db_name, QString db_path);

    virtual void slot_goto_artifact_source_submenu_clicked(QAction *clicked_action);
    virtual void slot_tableWidget_doubleClicked(int row, int column);

    virtual void slot_hide_loading_display_dialog_box(bool cancel_status);
    virtual void slot_act_quick_look_triggered();
    virtual void slot_goto_screenshot_record(QString record_str);

    virtual void slot_tablewidget_current_cell_changed(int currentRow, int currentColumn, int previousRow, int previousColumn);

    virtual void slot_action_export_all_file_system_records_triggered();
    virtual void slot_action_export_selected_file_system_records_triggered();
    virtual void slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path);
    virtual void slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip, QString password_enc_zip);
    virtual void slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dir_path);
    virtual void slot_main_table_widget_item_clicked(QTableWidgetItem *item);

    virtual void slot_thread_zip_file_export_finished();

    virtual void slot_sub_menu_open_file_with_other_app_clicked(QAction *current_clicked_action);

    virtual  void slot_message_box_yes_no_clicked(bool yes_no_clicked);

    virtual void slot_abouttoshow_tags_in_submenu();
    virtual void slot_add_file_to_text_indexing_queue_clicked();

    virtual void slot_menu_export_as_clicked(QAction *current_clicked_action);
    virtual void slot_save_results_by_export_as_csv_sqlite(QString file_name, QString selected_dir_path, QString export_type_str, QString report_scope);

    virtual void slot_custom_table_header_widget_search_field_return_pressed();
    virtual void slot_custom_table_header_widget_search_field_text_changed();
    virtual void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

protected:
    virtual int set_table_header(QStringList list_headers, QStringList headersWidth);
    virtual void create_right_click_actions(QMenu *mainMenu, QStringList submenuList);
    virtual QMenu *create_right_click_submenu(QString submenuStr, QMenu *mainMenu, QMenu *subMenu);
    virtual void fill_display_essentials(QStringList associated_plugins_list);
    virtual void display_data(QString dest_db_path);

    ///-------Right Click Actions
    virtual void action_add_note_triggered();
    virtual void action_remove_note_triggered();
    virtual void action_add_note_to_bookmark_triggered();
    virtual void action_remove_note_bookmark_triggered();

    virtual void update_bookmark_value(QString status, int row, int column);

    virtual void create_default_right_click();

    virtual int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    virtual void action_open_with_plist_viewer_triggered();
    virtual void action_open_with_hex_viewer_triggered();
    virtual void action_open_with_sqlite_viewer_triggered();
    virtual void action_open_with_log_viewer_triggered();
    virtual void action_sendToBucket_log_viewer_triggered();
    virtual void action_sendToBucket_plist_viewer_triggered();
    virtual void action_sendToBucket_hex_viewer_triggered();
    virtual void action_sendToBucket_sqlite_viewer_triggered();
    virtual void action_mark_as_seen_unseen(QString clicked_action_text);
    virtual void action_goto_source_triggered();

    virtual QAction *create_and_get_right_click_action(QString submenuStr, QMenu *mainMenu);
    virtual void action_export_triggered();
    virtual void action_open_full_detailed_info_triggered();
    virtual void action_open_detach_detailed_info_triggered();
    virtual int get_stack_index_for_preview(QString current_media_path, QString display_file_path);
    virtual void tablewidget_general_clicked(int row, int column);
    virtual int insert_record_in_table_display(QString database_path, QString db_cmd, QStringList tab_list_from_csv, QStringList addbindvalue_list);

    virtual void update_notes_value(QString text, int current_row);
    virtual void action_go_to_record_triggered();
    void create_tags_submenu();

    virtual void action_submenu_tag_triggered(QString tag_name);
    virtual void update_tags_value(QString tag_data, QString colour);
    virtual void create_add_files_to_hashset_submenu();

    virtual void contextMenuEvent(QContextMenuEvent *evt);
    virtual void action_bookmark_triggered();
    virtual void action_remove_bookmark_triggered();
    virtual QStringList export_file_system_record(QString record_no, int current_row);

    virtual void create_action_for_goto_artifact_source_submenu();

    virtual void action_copy_to_clipboard();
    virtual void action_remove_record();

    virtual void enable_disable_go_to_source(int current_row ,QString feature_plugin_name);

    //=================================================================//


    enum enum_tablewidget_column{
        enum_tablewidget_column_bookmark,
        enum_tablewidget_column_notes,
        enum_tablewidget_column_tags
    };

    //=================================================================//



    QVBoxLayout *main_vbox;
    QHBoxLayout *hbox_table_widget_extra_widgets;
    QHBoxLayout *upper_widgets_hbox;

    QTabWidget *main_tab_widget_obj;

    QLineEdit *lineEdit_search;
    QPushButton *pushButton_search,*pushbutton_show_all;

    m_tablewidget *m_tablewidget_obj;
    QTreeWidgetItem *tree_case_display_currentItem;

    QMenu *mainMenu;
    QMenu *submenu_open_with;
    QMenu *submenu_open_with_external_application;
    QMenu *submenu_send_to_bucket;
    QAction *act_export;
    QMenu *submenu_add_files_to_hashset_db;
    QAction *action_search_file_with_same_md5_hash;
    QAction *act_create_hashset_db;
    QAction *act_go_to_record;
    QAction *act_quick_look;
    QAction *act_full_detail_info;

    QAction *act_face_search;
    QAction *act_remove_record;

    QString searched_keyword_str;

    QString result_dir_path;
    QString csv_dir_path;
    QString feature_name;
    QString plugin_name;
    QStringList plugin_and_tab_name_list_for_csv;

    QString destination_db_path;

    QStringList display_header_name_list;
    QStringList db_columns_list;
    QStringList table_name_list;
    QStringList table_headers_width_list;
    QStringList table_display_order_list;
    QStringList display_column_data_type_list;


    // New

    QString tablewidget_loader_master_list;

    ///-----Tags----
    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QMenu *sub_menu_tags;
    QMenu *submenu_go_to_artifact_source;

    add_tags *add_tags_module_obj;
    remove_multiple_tag_selecotor *remove_multiple_tag_selecotor_obj;

    QString current_media_path;
    QString preview_file_path;
    int record_no_index;
    int plugin_name_index;
    int tab_name_index;
    int os_name_index;
    int source_count_name_index;
    int file_size_index;
    int file_path_index;
    int notes_value_index;
    int blank_column_index;

    // Hex and Text viewer specific variables
    int hex_and_text_viewer_start_offset_index;
    int hex_and_text_viewer_end_offset_index;

    // Plist viewer specific variables
    int plist_viewer_display_name_index;

    // Sqlite Viewer specific variables
    int sqlite_viewer_db_table_name_index;
    int sqlite_viewer_diplay_name_index;


    message_dialog *message_dialog_obj;
    bool bool_message_box_yes_button_clicked;
    bool bool_message_box_clicked;

    QString clicked_item_tab_text;
    struct_global_csv_reader_full_csv_info st_result_loader_csv_info_obj;

    hashset_storage *hashset_storage_obj;
    hashset_display *hashset_display_obj;

    add_notes *add_notes_to_singal_record_obj;
    add_notes *add_notes_to_bookmarks_obj;
    QString metadata_str;


    qint64 total_record_count;
    qint64 total_covered_count;
    qint64 progressbar_increment_step;
    QString command_for_table_display;
    bool bool_display_table_data_loaded;

    bool bool_sort_ascending;

    int tag_index;

    R_Treewidget *treewidget_case_display_obj;

    QMimeDatabase mime_database_obj;
    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    void loading_display_dialog_based_on_records(int total_records, int processed_count, QString title_str);
    bool bool_cancel_loading;

    global_connection_manager *global_connection_manager_obj;
    global_recon_file_info *global_recon_file_info_obj;

    QString first_tab_name;

    RECON_case  *recon_case_obj;
    bool bool_virtual_export ;

    recon_helper_standard *recon_helper_standard_obj;
    export_rsync *export_rsync_obj;

    struct_global_set_metadata_info st_set_meta_obj;
    bool bool_cell_changed_connection_buildup_for_tablewidget = false;
    QStringList bookmarked_records_from_table_display_list;
    QString get_column_text_from_selected_row(qint64 selected_row_no, qint64 col_no);

    QPushButton *pushButton_export_as;
    export_dialog_box_2 *export_dialog_box_2_obj;
    QMenu *menu_export_as;
    virtual void export_data_as_csv(QString filepath);
    virtual void export_data_as_sqlite(QString filepath);
    void open_exported_file(QString file_path);

    QString report_scope_str;


    enum enum_column_name_index{
        enum_column_notes_index,
        enum_column_record_no_index,
        enum_column_plugin_index,
        enum_column_category_index,
        enum_column_os_name_index,
        enum_column_source_count_name_index,
        enum_column_item_1_index,
        enum_column_item_2_index,
        enum_column_item_3_index,
        enum_column_item_4_index,
        enum_column_item_5_index,
        enum_column_item_6_index,
        enum_column_item_7_index,
        enum_column_item_8_index,
        enum_column_item_9_index,
        enum_column_item_10_index,
        enum_column_item_11_index,
        enum_column_item_12_index,
        enum_column_item_13_index,
        enum_column_item_14_index,
        enum_column_item_15_index,
        enum_column_item_16_index,
        enum_column_item_17_index,
        enum_column_item_18_index,
        enum_column_item_19_index,
        enum_column_item_20_index
    };

    bool bool_process_thumbnails_finished = false;

    QMenu *submenu_bookmark;
    QMenu *submenu_note;
    QMenu *submenu_carve;
    QMenu *submenu_seen;

    custom_table_header_widget *custom_table_header_widget_obj;
    QPushButton *pushbutton_show_column;
    QMap<QString, QList<custom_table_header_widget *>> tab_custom_header_widgets_QMap; ///-----key = feature_name-----value = custom_table_header_widget pointer list-----///

    void all_column_text_search(QList<struct_search_column_text_for_file_type> search_text_all_column_list);
    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;

    QString existing_plugin_name;

    QMap<QString, QList<custom_table_header_widget *>> tab_custom_header_widgets_for_redefine_result_Messenger_QMap; ///-----key = tab_name-----value = custom_table_header_widget pointer list-----///
    QMap<QString, QList<custom_table_header_widget *>> tab_custom_header_widgets_for_redefine_result_browser_history_QMap; ///-----key = tab_name-----value = custom_table_header_widget pointer list-----///
};

#endif // TABLE_RESULT_LOADER_H
