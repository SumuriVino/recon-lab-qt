#ifndef CARVED_FILES_LOADER_H
#define CARVED_FILES_LOADER_H


#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"
#include "gallery_viewer/gallery_viewer.h"


class carved_files_loader : public table_result_loader
{
    Q_OBJECT

public:

    explicit carved_files_loader(QWidget *parent = 0);
    void pub_set_carved_files_display_db_path(QString db_path);
    void populate_data_in_table();
    void pub_create_custom_ui();

    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString rec_no_str);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString rec_no_str);
private slots:
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_act_quick_look_triggered();
    void slot_sub_menu_open_file_with_other_app_clicked(QAction *current_clicked_action);
    void slot_goto_screenshot_record(QString record_str);

    void slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1);
    void slot_main_tab_widget_current_changed(int index);
    void slot_pushbutton_showall_clicked();
    void slot_add_file_to_text_indexing_queue_clicked();


    void slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1);
    void slot_gallery_view_invalid_tem_clicked();

    void slot_pushbutton_search_clicked();

    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);

    void slot_table_widget_header_item_clicked(int column_index);
    void slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info f1);
    void slot_open_with_submenu_clicked(QAction* current_clicked_action);

private:

    enum enum_carved_File_TabWidget_Indexes
    {
        enum_carved_File_TabWidget_Indexes_TABLE_VIEW = 0,
        enum_carved_File_TabWidget_Indexes_GALLERY_VIEW
    };


    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);
    void update_tags_value(QString tag_data, QString colour);
    void update_notes_value(QString text, int current_row);
    void action_remove_note_triggered();
    void action_open_full_detailed_info_triggered();
    void action_open_detach_detailed_info_triggered();
    void action_export_triggered();
    int insert_record_in_table_display(QString database_path, QString db_cmd, QStringList tab_list_from_csv, QStringList addbindvalue_list);
    void update_bookmark_value(QString status, int row, int column);
    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_open_with_log_viewer_triggered();
    void action_sendToBucket_log_viewer_triggered();
    void action_sendToBucket_plist_viewer_triggered();
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void add_applications_to_open_application_menu();

    QString destination_db_path;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    int file_name_index , offset_index, notes_icon_index, tag_icon_index, bookmark_value_index;
    QMenu   *submenu_open_files_with_other_app;

    void action_add_note_triggered();

    QString tab_name_str ;

    bool bool_did_tableview_display_changed = false;

    gallery_viewer *gallery_viewer_obj;
    QList<struct_global_gallery_view_file_info> st_gallery_view_file_info_list;
   // QAction *act_add_file_to_text_indexing;
    recon_helper_process *recon_helper_process_obj;

};

#endif // CARVED_FILES_LOADER_H
