#ifndef PLUGIN_VIEWER_H
#define PLUGIN_VIEWER_H

#include "add_notes/add_notes.h"
#include "previewer/av_preview.h"
#include "previewer/blank.h"
#include "previewer/image_preview.h"
#include "previewer/previewer_stack_module.h"
#include "previewer/detached_file_previewer.h"
#include "previewer/detached_file_previewer_module.h"
#include "previewer/detached_metadata_viewer.h"
#include "previewer/detached_metadata_viewer_module.h"
#include "previewer/mdls/detach_mdls_dialog_module.h"
#include "previewer/mdls/full_mdls_dialog_module.h"
#include "timeline_selection_window/timeline_selection_window.h"
#include "usb_device_serial_list_module.h"
//#include "export/thread_file_copier_module.h"
#include "report/report_essentials/report_template_html_standard.h"
#include "header_plugin_extraction_interface.h"

#include "recon_generic_functions/recon_static_functions.h"
#include "report/report_essentials/report_template_html_advance.h"
#include "add_tags/add_tags.h"
#include "add_tags/remove_multiple_tag_selecotor.h"

#include "m_treewidgetitem.h"
#include "file_viewer/plist_viewer/plist_viewer.h"
#include "ui_custom_classes/m_tablewidget.h"
#include "ui_custom_classes/m_treewidget.h"
#include "ui_custom_classes/m_textedit.h"
#include "ui_custom_classes/m_checkbox.h"
#include "ui_custom_classes/m_stackedwidget.h"


#include "tsk/libtsk.h"
#include "file_viewer/sqlite_viewer/sqlite_viewer_starter.h"
#include "manage_column/manage_plugin_columns.h"
#include "full_detailed_information/full_detailed_info.h"
#include "message_box/message_dialog.h"
#include "hashset/hashset_display.h"
#include "hashset/hashset_storage.h"


#include "header_global_variables_3.h"
#include "header_gui_settings.h"
#include "header_report.h"
#include "chat_viewer/chat_viewer.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"

#include "header_global_enum.h"

#include "message_box/message_dialog.h"
#include "file_viewer/text_viewer/text_viewer.h"
#include "report/thread_report_builder/thread_report_builder.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "global_connection_manager.h"
#include "global_recon_file_info/global_recon_file_info.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "export/export_except_reports/export_rsync/export_rsync.h"
#include "gallery_viewer/gallery_viewer.h"
#include "file_search/search_files_with_hash/search_files_with_hash.h"
#include "recon_case/recon_case.h"
#include "file_viewer/log_viewer/log_viewer.h"

namespace Ui {
class plugin_viewer;
}

class plugin_viewer : public QFrame
{
    Q_OBJECT

public:

    explicit plugin_viewer(QWidget *parent = 0);
    ~plugin_viewer();

    void pub_set_plugin_name(QString name);
    void pub_set_display_default_tags();
    void pub_set_essentials();

    void pub_display_data();

    struct_global_advance_report_plugins_tab pub_get_tabs_info(QString report_scope, QStringList selected_tags_list);
    void pub_set_all_plugins_tabs_info(QList<struct_global_advance_report_plugins_tab> list_strct);


    void pub_set_global_manager_obj(global_connection_manager *obj);
    void pub_set_global_recon_file_info_obj(global_recon_file_info *obj);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str);
    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str);
    void pub_set_recon_case_obj(RECON_case *obj);
protected:
    bool eventFilter(QObject *, QEvent *);

signals:

    void signal_refresh_widgets();
    void signal_keyword_searched_clicked();

    ///----tags---
    void signal_add_tags(QString tag_name,QString colour_name);

    void signal_viewer_extarction_completed(QString result_name,QString viewer_name);


    void signal_open_in_detach_clicked(QString metadata_str, QString export_file_path, QString current_media_path, int preview_index ,QString searched_text, QString source_count_name);
    void signal_show_file_in_viewer_master_frame(QString tab_text,QString viewer_name, QString source_count_name);

    void signal_openwith_after_sendto_bucket_master_frame(QString tab_text,QString viewer_name);
    void signal_hashset_finished(QStringList);

    void signal_change_bookmark_status_in_other_db_also(QString record_no,QString plugin_name,QString tab_name,QString bookmark_status,QStringList applied_tags_list, QString db_path, QString os_scheme, QString feature_name);

    void signal_change_notes_value_in_other_db_also(QString record_no,QString plugin_name,QString tab_name,QString notes_text,QStringList applied_tags_list, QString os_scheme, QString feature_name);

    void signal_remove_current_record_entry_from_tag_search_display(QString plugin_name,QString tab_name,QString record_no,QStringList removed_tags_list, QString os_scheme);

    void signal_add_current_record_info_in_tag_search_db(QString clicked_tag_name,QString is_emitted_from_plugin,QString plugin_name,QString tab_name,QString record_no,QString db_path, QString os_scheme);

    void signal_add_current_tag_into_loader_class_sub_menu(bool status,QString new_tag_name,QString colour_name);

    void signal_update_other_db_and_display_also(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme);


    void signal_update_created_hashset_db_into_other_submenu_from_plugin(QString db_name, QString db_path);
    void signal_go_to_artifact_source_in_file_system(QString source_path, QString fs_db_path, QString source_name);

    void signal_add_notes_added_in_db(QString, QString, QString, QString, QString, QString, QString,QString , QString);
    void signal_statusbar_progress_bar_show_hide(bool status, QString message);
    void signal_update_display_on_remove_notes(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme);
    void signal_manage_columns_checkstate();
    void signal_act_quick_look_triggered();
    void signal_set_metadata_info(struct_global_set_metadata_info m_obj);

    void signal_tablewidget_go_to_mail(QString plugin_name, QString parent_name, QString tab_name ,QString record_no, QString db_path, QString os_name_str);
    void signal_plugin_viewer_space_pressed_for_quicklook(QString filepath);
    void signal_clear_all_metadata();
    void signal_set_bookmark_status_to_gallery_view(bool bookmark_status);
    void signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj);
    void signal_save_openwith_changes_to_bucket_for_full_detail_info(QString viewer_type);

private slots:


    void slot_arrow_key_pressed();
    void slot_bookmark_key_pressed_tablewidget();

    void slot_control_e_key_pressed();
    void slot_control_m_key_pressed();
    void slot_control_n_key_pressed();

    void slot_sort_bookmarked(int index);
    void slot_act_bookmark_triggered();
    void slot_act_remove_all_bookmark();
    void slot_act_add_notes();
    void slot_act_add_notes_to_bookmarks();
    void slot_act_export();
    void slot_add_notes(QString notes_value);
    void slot_add_notes_to_bookmarks(QString data);
    void slot_time_line(qint64, qint64);
    ///---Tags---

    void slot_show_tag();
    void slot_add_tags_to_plugin(QString data, QString colour_name);
    void slot_remove_tags_from_plugin();
    void slot_selected_tag(QAction *action);
    void slot_update_tags_in_actions_menu(bool status, QString new_tag,QString tag_colour);
    void slot_tag_list_after_remove_multiple_tag(QStringList list_tags_remained, QString table_name, QString record);

    void slot_act_remove_notes();
    void slot_act_remove_all_notes();
    void slot_act_remove_notes_from_bookmarks();

    void slot_progress_window_module_closed();


    void on_checkBox_timeline_toggled(bool checked);
    void on_pushButton_timeline_clicked();
    void on_pushButton_showall_clicked();
    void on_pushButton_search_clicked();
    void on_lineEdit_search_returnPressed();

    void slot_detach_mdls_dialog_window_closed();


    void on_pushButton_generate_report_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_lineEdit_search_textChanged(const QString &arg1);


    void slot_fixed_last_blank_column(int col_index);


    void slot_act_extract_plist_viewer_openwith();

    void slot_act_extract_log_viewer_openwith();
    void slot_act_log_viewer_sendtobucket();

    void slot_tableWidget_cellClicked(int row, int column);

    void slot_m_treewidget_itemclicked(QTreeWidgetItem *item, int column);

    void slot_m_checkbox_toggled_textedit( bool checked);

    void slot_m_checkbox_toggled_stack(bool checked);
    void slot_act_hex_viewer_openwith();


    void slot_act_extract_sqlite_viewer_openwith();

    void slot_act_manage_columns();
    void slot_manage_column_clicked(int col, int check_status);
    void slot_full_detail_info_triggered();
    void slot_open_in_detach_triggered();

    void slot_act_plist_viewer_sendtobucket();
    void slot_act_sqlite_viewer_sendtobucket();
    void slot_act_hex_viewer_sendtobucket();

    void slot_hashset_menu_add_file_clicked(QAction *current_clicked_action);
    void slot_action_search_file_with_same_hash_clicked();
    void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);

    void slot_removed_tags_list(QStringList list_tags_removed, QString table_name, QString record);

    void slot_update_tag_submenu(bool status, QString new_tag, QString tag_colour);
    void slot_update_plugin_hashset_submenu(QString db_name, QString db_path);
    void slot_submenu_go_to_artifact_triggered(QAction *clicked_action);

    void slot_message_box_yes_no_recived(bool value);

    void slot_act_copy_to_clipboard();
    void slot_tableWidget_doubleClicked(int row, int column);

    void slot_goto_record(QString tab_name, QString rec);

    void slot_act_remove_bookmark();
    void slot_abouttoshow_tags_in_submenu();

    void slot_thread_report_finished_or_cancelled();

    void slot_action_manage_column_clicked(QAction *current_clicked_action);
    void slot_table_header_customContextMenuRequested(const QPoint &points);

    void slot_act_quick_look_triggered();
    void slot_hide_loading_display_dialog_box(bool cancel_status);

    void slot_tablewidget_current_cell_changed(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void slot_hide_progressing_bar_on_cancel_loading_report(bool cancel_status);
    void slot_act_go_to_mail_triggered();
    void slot_spacebar_quicklook_pressed();
    void slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1);
    void slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1);

    void slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_invalid_tem_clicked();

    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);

    void slot_table_widget_header_item_clicked(int column_index);

    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

private:

    Ui::plugin_viewer *ui;

    typedef struct{
        int hid_col_no;
        QString hid_col_name;
    }struct_manage_column_info;


    //For generic_work i.e. plugin using template
    QString plugin_template_csv_path;

    bool bool_sort_bookmarked[500];  // 500 is MAX_TABS_TABLEWIDGET

    QStringList total_tabs_list;
    QStringList supported_os_scheme_list;
    QStringList native_os_list;


    QStringList table_name_list_tablewidget;
    QStringList tab_name_list_tablewidget;
    QStringList table_apple_metadata_available_list_tablewidget;
    QStringList table_html_report_available_list_tablewidget;
    QStringList table_xml_report_available_list_tablewidget;
    QStringList table_csv_report_available_list_tablewidget;
    QStringList table_kml_report_available_list_tablewidget;

    QStringList table_export_available_list_tablewidget;

    QStringList table_g_search_available_list_tablewidget;

    QStringList table_timeline_available_list_tablewidget;


    QList<QStringList>export_path_column_list_tablewidget;

    QList<QStringList>tw_header_list_tablewidget;
    QList<QStringList>tw_column_name_list_tablewidget;
    QList<QStringList>tw_data_type_list_tablewidget;
    QList<QStringList>tw_col_width_list_tablewidget;
    QList<QStringList>tw_col_order_list_tablewidget;


    QList<QStringList>md_header_list_tablewidget;
    QList<QStringList>md_column_name_list_tablewidget;
    QList<QStringList>md_data_type_list_tablewidget;
    QList<QStringList>ms_search_local_list_tablewidget;
    QList<QStringList>md_preview_list_tablewidget;

    QList<QStringList>md_header_list_tree;
    QList<QStringList>md_column_name_list_tree;
    QList<QStringList>md_data_type_list_tree;
    QList<QStringList>md_preview_list_tree;







    QList<QStringList>g_search_col_name_list;
    QList<QStringList>g_search_col_order_list;
    QList<QStringList>g_search_col_data_type_list;


    QList<QStringList>tm_col_name_list;
    QList<QStringList>tm_col_data_type_list;



    QStringList tab_name_list_textedit;
    QStringList table_name_list_textedit;
    QList<QStringList>column_name_list_textedit;
    QList<QStringList>data_type_list_textedit;


    QStringList html_report_available_list_textedit;
    QStringList xml_report_available_list_textedit;
    QStringList csv_report_available_list_textedit;




    QStringList table_name_list_tree;
    QStringList tab_name_list_tree;
    QList<QStringList>header_list_tree;
    QList<QStringList>parent_child_list_tree;
    QList<QStringList>column_name_list_tree;
    QList<QStringList>data_type_list_tree;
    QList<QStringList>col_width_list_tree;


    //Variables in all modules

    QDateTimeEdit mindatetime;
    QComboBox combo;

    QString reports_location_path;
    QString path_manually_export;
    QString reports_dir_path;
    QString modules_export_report_path;
    QString adhoc_path;
    QString destination_file_location;
    QString destination_db_file;
    QString recon_result_dir_path;
    QString modules_export_path;
    QString system_password;
    QString system_time_zone;
    QString examiner_time_zone;
    QString current_media;
    QString current_media_linked_file_ios;

    QStringList global_tag_list;
    QStringList global_selected_tabs_with_export_status;




    bool check_bookmark_status;
    bool bool_cancel_extraction;
    bool bool_cancel_global_search;
    bool cancel_exporter_loop;
    bool bool_detach_mdls_set;
    bool bool_full_mdls_set;



    QString received_os_naming_scheme_for_feature_only;

    qint64 target_module_size;
    int received_date_format_from_pub;
    qint64 case_created_machine_offset;
    qint64 system_time_offset;
    qint64 examiner_time_offset;
    qint64 timeline_start_time, timeline_end_time;

    QMenu *menu;
    QMenu *menu_manage_column;
    QMenu *tag_sub_menu;

    QAction *act_bookmark;
    QAction *act_bookmark_all;
    QAction *act_remove_bookmark;
    QAction *act_remove_all_bookmark;
    QAction *act_add_notes;
    QAction *act_add_notes_to_bookmark;
    QAction *act_export;
    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QAction *act_remove_notes;
    QAction *act_remove_notes_from_bookmark;
    QAction *act_remove_all_notes;
    QAction *act_seperator_bookmark;
    QAction *act_seperator_notes;
    QAction *act_copy_to_clipboard;

    //ktktkt
//    QAction *act_go_to_mail;
    //ktktkt
    QAction *action_search_file_with_same_md5_hash;

    add_notes *add_notes_dialog_module_popobj1;
    add_notes *add_notes_dialog_module_popobj2;
    detached_metadata_viewer_module *detached_metadata_viewer_module_popobj;
    detached_file_previewer_module *detached_file_previewer_module_popobj;
    timeline_selection_window *timeline_selection_window_obj;

    detach_mdls_dialog_module *detach_mdls_dialog_module_popobj;

    QString get_plugin_name();
    QString module_plugin_name;
    QString parent_module_name;
    bool bool_export_available;

    QString debug_file_path;
    QString soft_link_directory_path;

    void initialize_tablewidget_array();
    void draw_headers_tablewidget();
    void populate_data_in_tablewidgets();
    void draw_table(QString command, int index, int cols_count, QStringList col_type_list, int tw_timeline_col_index, int tw_notes_index,int tw_os_scheme_index ,QStringList col_width_list,QStringList search_col_list,QStringList search_string_list);



    //Functions in all modules
    void disable_action_widgets(bool status);

    void copy_resource_directory(QString source, QString destination);



    QString mining_global_stamp_name(QString);

    //Module specific
    void find_source_file(void);
    int create_db_tables(void);
    int extract_module_artifacts(void);
    void recreate_sourcefile_list();

    void tablewidget_general_clicked(int tab, int row, int column);

    bool is_column_of_apple_metadata(QString column_name);


    QString data_prepare_metadata;
    QString searched_keyword;

    QList<struct_global_advance_report_plugins_tab> list_all_plugins_tabs_info;
    int max_record_per_page;
    qint64 get_total_record_of_table(QString table_name, int type, QStringList tags_list);


    QString set_table_data(int int_tab,QString command, QString table_name, QString is_export_available,QString is_multiple_export,int report_pattern,int report_scope, int sr_no);
    void set_table_data_kml(int int_tab, QString command, QString table_name, QString is_export_available, QXmlStreamWriter *obj);


    qint64 total_record_count;
    qint64 total_covered_count;
    //  qint64 progressbar_increment_step;

    void remove_extra_widgets();
    void initialize_textedit_array();
    void populate_data_in_textedit();
    void draw_textedit(int index, QString table_name, QStringList col_name_list, QStringList col_datatype_list);
    void generic_checkbox_textedit(bool checked);
    void initialize_tree_array();
    void populate_data_in_tree();
    QString mining_parent_data_type_for_tree(QStringList parent_childlist, QStringList col_data_type_list);
    QString mining_parent_column_for_tree(QStringList parent_childlist, QStringList col_namelist);
    void draw_tree(int index, QString table_name, QString parent_column_name, QString parent_datatype, QStringList col_name_list, QStringList col_type_list, QStringList col_width_list);
    void draw_headers_tree();



    QStringList table_name_list_thumbnail;
    QStringList tab_name_list_thumbnail;


    QStringList table_name_list_chat_viewer;
    QStringList tab_name_list_chat_viewer;


    QList<QStringList>display_column_name_list_chat_viewer;
    QList<QStringList>display_data_type_list_chat_viewer;



    void populate_data_in_thumbnail_viewer();
    void draw_thumbnail(QString table_name, int m_index);
    void treewidget_general_clicked(int tab_index, QTreeWidgetItem *item, int column);



    QString mining_column_name_for_datatype(QStringList col_name_list, QStringList data_type_list, QString looking_for_data_type);
    int mining_position_for_datatype(QStringList data_type_list, QString looking_for_data_type);


    QStringList tab_name_list_stack;
    QStringList table_name_list_stack;
    QList<QStringList>column_name_list_stack;
    QList<QStringList>data_type_list_stack;


    QStringList html_report_available_list_stack;
    QStringList xml_report_available_list_stack;
    QStringList csv_report_available_list_stack;




    void initialize_stack_array();
    void populate_data_in_stack();
    void draw_stack(int index, QString table_name, QStringList col_name_list, QStringList col_datatype_list);


    void clear_csv_lists();

    ///-----Tags----
    add_tags *add_tags_obj;
    QString get_icon_path_tags(QString colour, QString data);
    QString get_tag_from_record_number(QString table_name, QString tag_str);
    void display_default_tags();
    void common_add_tags_to_plugin(QString data, QString colour);
    QString get_tags_from_table_record(QSqlDatabase dest_db, QString table, QString record);
    remove_multiple_tag_selecotor *remove_multiple_tag_selecotor_obj;

    QAction *act_seperator_viewers;
    QAction *act_plist_viewer_openwith;
    plist_viewer *plist_viewer_obj;
    bool bool_show_all_clicked;


    QList<m_tablewidget *> list_tablewidgets;
    QList<m_treewidget *> list_treewidgets;
    QList<m_textedit *> list_texteditwidgets;
    QList<m_checkbox *> list_checkbox_textedit_widgets;
    QList<m_stackedwidget *> list_stackedwidgetwidgets;
    QList<m_checkbox *> list_checkbox_stackedwidget_widgets;
    QList<chat_viewer *> list_chat_viewer_widgets;
    QList<gallery_viewer *> list_thumbnail_viewer_widgets;



    //============================================//

    QList<QList<struct_global_gallery_view_file_info> *> st_gallery_view_file_info_QList_QList;

    // Hex Viewer
    QAction *act_hex_viewer_openwith;


    QAction *act_sqlite_viewer_openwith;
    sqlite_viewer_starter *sqlite_viewer_starter_obj;


    QAction *act_manage_columns;
    m_tablewidget *m_tablewidget_obj_for_manage_column;
    manage_plugin_columns *manage_plugin_columns_obj;

    QList<struct_manage_column_info> get_hidden_column_from_manage_column_db();
    QString current_tab_text;


    QMenu *submenu_go_to_artifact;
    QMenu *submenu_open_with;
    QMenu *submenu_send_to_bucket;
    QAction *act_full_detail_info;

    QAction *act_plist_viewer_sendtobucket;
    QAction *act_sqlite_viewer_sendtobucket;
    QAction *act_hex_viewer_sendtobucket;

    QString preview_file_path;
    int preview_index_full;
    QAction *act_open_in_detach;

    void forward_set_essentials();



    ///-----------------------Device Info
    QString device_name;
    QString device_id;
    QString device_imei;
    QString device_phone_no;
    QString device_serial_no;
    QString device_backup_path;
    QString device_backup_size;
    QString latest_backup_date_readable_QString;
    qint64  latest_backup_date_numeric_qint64;
    QString device_product_name;
    QString device_iccid;
    QString device_display_name;
    QString device_country;
    QString device_city;
    QString device_longitude;
    QString device_latitude;
    QString device_timezone;
    qint64 device_timezone_offset_seconds;
    QString device_product_type;
    QString device_os_full_version;
    QString device_os_initial_version;

    /// target system info


    QString product_type;
    QString os_version_full_target_sys;
    QString os_version_target_sys;

    QString sys_timezone_name;
    qint64 sys_timezone_seconds;
    QString country,city,latitude,longitude;

    message_dialog *message_dialog_object;

    QMenu *menu_hashset_add_files;

    hashset_storage *hashset_storage_obj;
    hashset_display *hashset_display_obj;

    QStringList get_applied_tags_name_on_current_record(QString record_no);


    void initialise_chat_viewer_array();
    void populate_data_in_chat_viewer();
    void draw_chat_viewer(int index, QString command, QStringList cols_data_type_list);
    void add_action_to_artifact_source_submenu(QString INT_str, QString table_name);


    bool bool_message_box_yes_or_no_receiver;


    void comman_signal_emit_for_statusbar_progressbar(bool status, QString message);
    QStringList display_tablewidget_table_name_list;
    void set_destination_location();

    thread_report_builder *thread_report_builder_obj;
    QAction *act_quick_look;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    bool bool_cancel_loading;
    void loading_display_dialog_based_on_records(int total_records, int processed_count, QString title_str);

    void display_hashset_add_files_submenu();

    qint64 get_total_record_of_db_according_search(QStringList search_string_list);

    display_loading_progress_bar *display_loading_progress_bar_for_reports_from_plugin_obj;
    bool bool_cancel_loading_reports;

    global_connection_manager *global_connection_manager_obj;
    global_recon_file_info *global_recon_file_info_obj;

    int get_stack_index_for_preview(QString current_media_path, QString display_file_path);
    QFileDialog file_dialog_obj;

    QStringList display_file_path_list;
    recon_helper_standard *recon_helper_standard_obj;
    export_rsync *export_rsync_obj;

    void initialize_thumbnail_viewer_array();
    int current_tab_index_only_for_tableview = MACRO_FAIL_VALUE_NUMERIC_int;
    struct_global_set_metadata_info st_set_meta_obj;
    QMap<QString,QStringList> map_bookmarked_records_from_table_display;

    void prepare_record_no_str_and_hash_md5_str_for_search_files_with_same_hash(QString fs_db_path, QString file_path);

    QString record_no_for_search_files_with_same_hash;
    QString hash_md5_for_search_files_with_same_hash;
    search_files_with_hash *search_files_with_hash_obj;

    int get_total_record_count_in_plugin_tab(QString tab_name);
    QString get_exact_tab_name_from_ui(int tab_index);

    RECON_case *recon_case_obj;
    QMenu *submenu_bookmark;
    QMenu *submenu_notes;

    struct struct_plugin_search_column_text
    {
        int column_number;
        QString search_text;
    };

    enum enum_file_types_display_fields
    {
        enum_Plugin_display_Bookmarks = 0,
        enum_Plugin_display_Notes_Icon,
        enum_Plugin_display_INT
    };

    custom_table_header_widget *custom_table_header_widget_obj;
    QTableWidget *current_table_widget;
    QMap<int, QList<custom_table_header_widget *>> tab_custom_header_widgets_QMap;

    void all_column_text_search(QList<struct_plugin_search_column_text> search_text_all_column_list);
    void refresh_horizontal_header();
    void clear_all_custom_header_widget_entries(QList<custom_table_header_widget *> custom_plugin_header_widgets_QList);
    bool bool_sort_with_search = false;

    QAction *act_log_viewer_openwith;
    QAction *act_log_viewer_sendtobucket;

};

#endif // PLUGIN_VIEWER_H
