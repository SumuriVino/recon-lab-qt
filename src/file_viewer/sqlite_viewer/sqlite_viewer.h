#ifndef SQLITE_VIEWER_H
#define SQLITE_VIEWER_H

///-Project Classes

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"
#include "thread_file_copier_module_sqlite_viewer.h"

#include"Extraction/sqlite_viewer_basecoreclass.h"
#include "progress_bar/progress_bar_window.h"
#include "add_notes/add_notes.h"

#include "previewer/av_preview.h"
#include "previewer/image_preview.h"
#include"previewer/blank.h"
#include "previewer/detached_file_previewer.h"
#include "previewer/detached_file_previewer_module.h"
#include"previewer/detached_metadata_viewer.h"
#include "previewer/detached_metadata_viewer_module.h"

#include"Extraction/sqlite_viewer_load_savedstateclass.h"
#include"Reports/sqlite_viewer_report_html_template.h"
#include"DisplayGuiWidgets/SaveStates/sqlite_viewer_savestate.h"
#include"DisplayGuiWidgets/SaveStates/sqlite_viewer_plugin_savestate.h"

#include"file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_status.h"

#include"file_viewer/sqlite_viewer/sqlite_viewer_manage_colums_type.h"

#include"plugin_viewer/progress_window_module.h"
#include"full_detailed_information/full_detailed_info.h"
#include "header_global_variables_1.h"
#include "message_box/message_dialog.h"
#include "header_global_variables_2.h"

#include "add_tags/add_tags.h"
#include "add_tags/remove_multiple_tag_selecotor.h"
#include "header_report.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"

#include "report/pdf_report_module.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "recon_case/recon_case.h"


namespace Ui {
class sqlite_viewer;
}

class sqlite_viewer : public QFrame
{
    Q_OBJECT

public:
    explicit sqlite_viewer(QWidget *parent = 0);
    ~sqlite_viewer();

    QString set_recon_result_dir(QString sqlite_db_path, QString tab_text);
    QList<struct_global_sqlite_viewer_tablewidget_main> list_struct_tablewidget_data_sqlite_viewer;

    void pub_set_global_connection_manager_obj(global_connection_manager *obj);
    void pub_set_recon_case_obj(RECON_case *obj);
signals:
    void signal_add_tags_sqlite_viewer(QString tag_name,QString colour_name);
    void signal_sqlite_viewer_metadata(QString,QString,QString,QString);
    void signal_refresh_widgets();
    void signal_show_metadata(bool);
    void signal_sqlite_viewer_change_content_of_detach_window(QString,QString,QString,int,QString,QString);
    void signal_sqlite_viewer_open_in_detach_clicked_table_result(QString metadata_str, QString export_file_path, QString current_media_path, int preview_index, QString searched_keyword,QString source_count_name);
    // void signal_insert_tag_detail_into_tag_search_db_from_sqlite_viewer(QString sqlite_plugin_name, QString tag_name, QString record_no, QString sqlite_viewer_display_name,QString os_scheme,QString db_table_name_of_taged_item, QString executed_query, QString sql_tab_name);
    void signal_remove_tag_entry_from_tag_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList removed_tags_list, QString os_scheme);

    void signal_update_tag_search_db_and_display(QString fs_plugin_name, QString tab_name, QString record_no_prepended_zero, QStringList list_tags_remained,QString os_scheme);
    void signal_save_openwith_changes_to_bucket(QString viewer_type);

    void signal_statusbar_progress_bar_show_hide(bool show_hide, QString msg);

    void signal_update_tag_submenu_of_plugins(bool status, QString new_tag_name, QString new_color_name);
    // void signal_change_bookmark_value_from_sqlite_viewer_to_bookmark_serach(QString record_no,QString plugin_name,QString tab_name,QString status,QString applied_tags, QString display_cols_values ,QString file_path, QString os_name_str, QString display_name_in_tree_and_tab , QString display_tab_name , QString dest_db_path);

    void signal_create_pdf_report(QString , QString);

    void signal_common_add_tag_name_into_case_tree(QString tag_name);
protected:
    bool eventFilter(QObject *, QEvent *);

private slots:
    void on_comboBox_dbtables_selection_currentIndexChanged(int index);

    void on_lineEdit_search_returnPressed();
    void on_pushButton_search_clicked();
    void on_pushButton_show_all_clicked();
    void on_tabWidget_main_data_panel_tabCloseRequested(int index);
    void on_tabWidget_main_data_panel_currentChanged(int index);

    ///-- Custom slots
    void slot_on_tableWidget_MainDataDisplay_cellClicked(int row, int column);
    void slot_on_tableWidget_FilterViewSearch_cellClicked(int row, int column);
    void slot_on_tableWidget_FilterViewSearch_horizontal_header_clicked(int column);
    void slot_export_as_plugin(struct_global_sqlite_viewer_plugin_info);

    void slot_common_tablewidget_col_sel_cell_clicked(int row, int column);
    void slot_common_tablewidget_col_sel_horizontal_header_clicked(int column);

    ///- RightClick slots
    void slot_act_remove_all_bookmark();
    void slot_act_add_notes();
    void slot_act_add_notes_to_bookmarks();
    void slot_act_export();

    void slot_act_remove_all_notes();
    void slot_act_remove_notes_from_bookmarks();
    void slot_act_remove_notes();
    void slot_rignt_click_menu_trigerred_sqlite_viewer(QAction *current_clicked_action);

    void slot_add_notes(QString notes_text);
    void slot_add_notes_to_bookmarks(QString data);

    void slot_act_columns_selection();

    void slot_full_detail_info_triggered();
    void slot_open_in_detach_triggered();

    ///- Buttons Slots
    void slot_arrow_key_pressed_on_table();
    void slot_bookmark_key_pressed_tablewidget();

    void on_pushButton_generate_report_clicked();
    void on_pushButton_execute_query_clicked();
    void on_textEdit_query_insert_textChanged();

    void on_tableWidget_query_execution_cellDoubleClicked(int row, int column);
    void on_pushButton_generate_report_execute_query_clicked();

    void slot_sort_bookmarked(int index);
    void slot_tab_bar_clicked(int index);

    void slot_savestate_on_combobox_conversion_change();
    void slot_onload_savestate_on_combobox_conversion_change();

    void slot_update_sqlite_viewer_export_checkbox_and_count(bool chckbox,int count);

    void slot_sub_menu_tags_clicked_for_sqlite_viewer(QAction *current_clicked_action);
    void slot_show_tag();
    void slot_remove_tags_from_sqlite_viewer();
    void slot_add_tags_to_sqlite_viewer(QString tag_name, QString colour);
    void slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour);
    void slot_removed_tags_list_sqlite_viewer(QStringList list_removed_tags, QString table_name, QString record_no);
    void slot_tag_list_after_remove_multiple_tag_sqlite_viewer(QStringList list_tags_remained, QString table_name, QString record);
    void slot_act_copy_to_clipboard();
    void slot_on_tableWidget_MainDataDisplay_celldoubleClicked(int row, int column);

    void slot_show_all(int old_pos,int new_pos);

    void slot_html_to_pdf_finished(QString pdf_file_path, bool);

    void slot_hide_loading_display_dialog_box(bool cancel_status);
    void slot_abouttoshow_tags_in_submenu();
private:
    Ui::sqlite_viewer *ui;

    enum enum_custom_columns
    {
        enum_coloumn_Sqlite_viewer_Bookmark = 0,
        enum_coloumn_Sqlite_viewer_Tag_icon,
        enum_coloumn_Sqlite_viewer_record_no
    };
    enum enum_custom_columns_sqlite_viewer_executed_query
    {
        enum_coloumn_sqlite_viewer_executed_query_Tag_icon = 0,
        enum_coloumn_sqlite_viewer_executed_query_record_no
    };

    typedef struct
    {
        QString export_filedetail;
        QString export_flepreview;
        QStringList multiple_export_filedetail;
        QStringList multiple_export_flepreview;

    }sqlite_viewer_st_export_file_detail;


    void update_savedstate_info_into_index_db_for_current_state();
    QString sqlite_viewer_display_name_in_tree_and_tab;

    int is_loadstate;

    QString recieved_sqlite_path_from_mainwindow;
    void set_values_obtain_from_sqlite_viewer_index_db(QString destination_db_file, QString targeted_sqlite);


    sqlite_viewer_base_core_class *base_core_class_obj;

    bool check_bookmark_status;

    ///- Source Directory and its path
    QString destination_result_directory_path;
    QString source_file;
    QString copied_source_file;
    QString saved_execute_query_maindb_path;

    QString sqlite_viewer_save_state_path;

    void reset_gui_widgets();
    void connect_table_filter_view_search_for_cell_clicked(int index);
    void load_sqlite_and_prepare_data(QString file_path);
    bool bool_combobox_clear;
    void set_widgets_disable(bool value);

    ///- On right Click
    QMenu *menu;
    QAction *act_add_notes;
    QAction *act_add_notes_to_bookmark;
    QAction *act_export;
    QAction *act_bookmark;
    QAction *act_remove_bookmark;

    QAction *act_remove_notes_from_bookmarks;
    QAction *act_remove_notes;
    QAction *act_columns_selection;
    QAction *act_copy_to_clipboard;

    QAction *act_seperator_bookmark;
    QAction *act_seperator_export;

    QAction *act_full_detail_info;

    add_notes *add_notes_dialog_popup;
    add_notes *add_notes_to_bookmarks_dialog_popup;


    //---Tag
    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QMenu   *sub_menu_tags;

    add_tags *add_tags_module_obj;
    remove_multiple_tag_selecotor *remove_multiple_tag_selecotor_obj;

    void action_submenu_tag_triggered_for_sqlite_viewer(QString tag_name);
    void display_tags_submenu_for_sqlite_viewer();

    void active_spacebar_bookmark_and_metadat_show(struct_global_sqlite_viewer_tablewidget_main obj_main);


    ///-Saved State Work
    void save_current_state();

    QString saved_state_dir_path;

    void filter_view_table_saved_state(QString destination_db_file);
    void data_display_table_saved_state(QString destination_db_file);

    int create_destination_database(QString destination_db_file);
    int create_db_tables_for_filter_view_table(QString destination_db_file);
    int insert_dataIn_DB_filter_view_table(QString destination_db_file);
    int create_db_tables_for_main_display_data_table(QString destination_db_file);
    int insert_dataIn_DB_main_display_data_table(QString destination_db_file);
    void update_dataIn_DB_main_display_data_table(QString destination_db_file);
    void notes_update_data_In_DB_main_display_data_table(QString destination_db_file,int input_type);

    QStringList get_db_coloum_datatype(QString table_name);
    QStringList get_coloum_datatype_from_loaded_result(QString table_name,QString db_path);
    sqlite_viewer_load_saved_state_class *load_saved_state_class_obj;

    ///-Reports
    QStringList headers_check_preview_enable(struct_global_sqlite_viewer_tablewidget_main *);

    QString reports_dir_final_path;
    QProcess process_html_to_pdf;
    bool bool_is_data_available_for_export;

    void sqlite_viewer_create_html_pdf_report(int report_type, QString file_path, int REPORT_CONTENT_FLAG);
    void sqlite_viewer_create_csv_report( QString file_path,int REPORT_CONTENT_FLAG);
    void sqlite_viewer_create_xml_report(QString file_path,int REPORT_CONTENT_FLAG);
    void export_files(QString report_path, int REPORT_CONTENT_FLAG);

    ///- Plugins Creation
    sqlite_viewer_savestate *sqlite_savestate_object;
    QStringList list_selected_tables_for_plugin;
    int create_plugin_Info_db(QString destination_file);
    int insert_dataIn_plugins_Info_db(QString destiantion_file,QString artifactsPath);

    QStringList list_selected_tables_for_savestate;

    sqlite_viewer_plugin_savestate *plugin_savestate_object;

    void set_execute_SQL_table_clear_and_widgets_enable(bool);

    QStringList coloum_list_query_execution_table;

    void metadata_popup_window(QString text);



    void create_screen_html_pdf_report_for_query_execution(int report_type, QString file_path);
    //    QStringList column_name_list_for_report(const struct_table_master &table1);
    void create_Screen_csv_report_for_query_execution(QString file_path);
    void create_Screen_xml_report_for_query_execution(QString file_path);

    bool bool_index_change_from_comboBox;
    bool bool_combotext_change_on_tab_change;

    bool bool_set_enable_right_click_export;
    int current_tab_index;
    QString current_tab_name;
    QString current_media_file;

    bool bool_saved_state_successfull;
    bool bool_sort_bookmarked;

    ///---- Finishing Work-------//
    QString saved_state_dir_name();
    QString main_datadisplay_table_sqlite_name();
    QString filter_view_table_sqlite_name();

    QString report_file_path;
    QString sqlite_keyword_search;
    QString sqlite_viewer_export_db_path;

    QString export_table_name;
    QStringList export_columns_name_list;

    QStringList file_copy_source_list;
    QStringList file_copy_destination_list;
    QStringList file_copy_recordno_list;
    QStringList file_copy_tablename_list;

    QString export_db_destination_path;

    sqlite_viewer_st_export_file_detail get_export_detail_html_final(QString column, QString tablename, QString is_multiple_export);

    void export_file_fun(int value, QString table_name, QString filey_path);

    void sqlite_viewer_export_db_sanity_check(QString file_path, int REPORT_CONTENT_FLAG);
    QStringList get_export_detail_csv(QString column, QString tablename);

    int check_vertical_hdr_with_col_count;
    QString extract_target_sqlite_db_path;

    void set_sqlite_viewer_report_generation_status(QString report_path);

    sqlite_viewer_manage_colums_type *sqlite_viewer_column_selection_obj;
    QTableWidget *ret_tableWidget_from_cols_sel;

    QString table_name_of_cols_sel;

    QString convert_data(QString data_item, QString data_type);

    bool bool_global_export_checkbox_enable_sqlite_viewer;
    int global_export_checkbox_disable_sqlite_viewer;

    int check_savedstate_and_update_filterview_search();
    int check_savedstate_and_update_main_data_display();

    int notes_check_savedstate_and_update_main_data_display(int input_type);

    QString agency_logo_path;

    QString sqlite_viewer_metadata_text;
    QString sqlite_viewer_current_media_file;
    int index_sqlite_viewer_media_type_av_blank_image;

    message_dialog *message_dialog_object;

    void load_savedstate_of_sqlite();
    void create_or_update_current_save_state_of_sqlite_viewer();
    QString create_and_insert_exported_content_info_in_db(int current_row, QString record_no_in_sqliteviewer, QString tag_name, QString sqlite_viewer_display_name, QStringList query_data_list, QString sql_tab_name, QString status);

    QString source_count_name;
    //   QString sqlite_viewer_display_name_in_tree;
    void display_tags_submenu_for_execute_query();


    void add_tags_to_sqlite_viewer(QString tag_name, QString colour);
    void add_tags_to_executed_query(QString tag_name, QString colour);
    QString update_tag_status_for_current_record(QString tag_data, QString status, QString record_no_str, QString sql_tab_name, QStringList text_data);

    bool bool_check_double_click;

    int check_savedstate_and_update_main_data_display_single_record();
    void update_dataIn_DB_main_display_data_table_single_record(QString destination_db_file);

    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading;

    void update_bookmark_value(QString status, int row, int bookmark_index);
    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void refresh_table_widget_data(QTableWidget *m_table_widget, QString table_name);

    void delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag, QString record_no, QString plugin_name, QString sql_tab_name);
    void update_tag_in_tag_serach_db(QString file_path, QString tag_name, QString record_no, QString sqlite_viewer_display_name, QString source_count_name, QString sqliteviewer_db_table_name_of_tag, QString executed_query, QString sql_tab_name);
    void update_bookmark_value_in_bookmark_serach(QString record_no, QString plugin_name, QString sql_tab_name, QString status, QString tag_name, QString executed_query, QString file_path, QString os_name_str, QString sqliteviewer_db_table_name_of_tag, QString current_table_name);

    global_connection_manager *global_connection_manager_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    QString recon_tag_name;
    QString recon_notes_name;
    QString recon_bookmark_name;
    bool find_tagging_on_record(int current_row);
    QStringList get_data_for_current_record(QTableWidget *m_table_widget);

    void set_source_file_essential();
    QString actual_file_path;
    QString recon_file_from;
    QString recon_file_info_path;
    QString complete_file_path ;
    QString notes_text_on_record;

    QString current_sql_tab;
    QString tag_name_for_execute_tab;

    pdf_report_module *pdf_report_module_obj;
    recon_helper_standard *recon_helper_standard_obj;

    bool check_current_column_selected_for_conversion_in_manage_column(QString column_str);
    bool check_current_column_selected_for_conversion_in_manage_column_for_file(QString column_str);
    int get_current_column_selected_for_conversion_in_manage_column_index(QString column_str);
    RECON_case *recon_case_obj;

    QMenu *submenu_bookmark;
    QMenu *submenu_notes;
    bool bool_for_the_unicode_conversion = false;

};

#endif // SQLITE_VIEWER_H
