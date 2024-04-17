#ifndef BOOKMARKS_SEARCH_LOADER_H
#define BOOKMARKS_SEARCH_LOADER_H

#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"
#include "global_connection_manager.h"
#include "file_export_dialog.h"
#include "export/export_except_reports/export_zip/thread_zip_file_export.h"
#include "recon_generic_functions/recon_helper_process.h"

class bookmarks_search_loader : public table_result_loader
{
public:
    bookmarks_search_loader(QWidget *parent = 0);
     ~bookmarks_search_loader();

    void pub_bookmarks_search_set_display_db_path(QString db_path);
    void bookmarks_search_populate_data_in_table();

    void pub_set_global_connection_manager_obj(global_connection_manager *obj);
private slots:
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);
    void slot_add_files_to_hashset_submenu_clicked(QAction *current_clicked_action);
    void slot_goto_artifact_source_submenu_clicked(QAction *clicked_action);
    void slot_sort_tablewidget_display(int column_index);


    void slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path);
    void slot_action_export_all_file_system_records_triggered();
    void slot_action_export_selected_file_system_records_triggered();

    void slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip, QString password_enc_zip);

    void slot_thread_zip_file_export_finished();
    void slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_path);
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);

private:

    QString destination_db_path_for_tag_search;

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);
    void action_open_full_detailed_info_triggered();
    void action_open_detach_detailed_info_triggered();
    void update_notes_value(QString text, int current_row);
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void action_sendToBucket_plist_viewer_triggered();
    void action_sendToBucket_log_viewer_triggered();
    void action_open_with_log_viewer_triggered();

    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_export_triggered();
    QStringList export_file_system_record(QString record_no, int current_row);
    void create_action_for_goto_artifact_source_submenu();
    void action_go_to_record_triggered();
    void action_remove_note_triggered();


    int bookmark_index;
    QString bookmark_col_name;
    QString record_no_column_name;
    QString plugin_column_name;
    QString tab_column_name;
    QString os_scheme_column_name;

    int search_db_path_index;
    void update_bookmark_value(QString status, int row, int column);
    void action_remove_bookmark_triggered();

    int item_0_index;
    int item_1_index;
    int item_2_index;
    int item_3_index;
    int item_4_index;
    int item_5_index;

    int item_6_index;
    int item_7_index;
    int item_8_index;
    int item_9_index;
    int item_10_index;


    int item_11_index;
    int item_12_index;
    int item_13_index;
    int item_14_index;
    int item_15_index;

    int item_16_index;
    int item_17_index;
    int item_18_index;
    int item_19_index;

    global_connection_manager *global_connection_manager_obj;
    QMenu *sub_menu_export_files;
    QAction *action_export_all_file_system_records;
    QAction *action_export_selected_file_system_records;
    bool bool_export_all_file_system_records;
    file_export_dialog *file_export_dialog_obj;

    thread_zip_file_export *thread_zip_file_export_obj;

    void action_goto_source_triggered();

    recon_helper_process *recon_helper_process_obj;
    bool bool_dmg_export_done,bool_zip_export_done;
    QString zip_export_file_path,dmg_export_file_path;
    void point_to_export_dir(QString path);
};

#endif //
