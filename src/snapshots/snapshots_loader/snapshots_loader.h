#ifndef snapshot_loader_H
#define snapshot_loader_H


#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"
#include "timeline_selection_window/timeline_selection_window.h"
#include "recon_generic_functions/recon_helper_process.h"

class snapshots_loader : public table_result_loader
{
    Q_OBJECT

public:

    explicit snapshots_loader(QWidget *parent = 0);
    ~snapshots_loader();

    void pub_set_snapshots_difference_db_path(QString dest_db);
    void populate_data_in_table();
    void pub_create_ui();
    void pub_set_selected_snapshot_name();

private slots:

    void slot_tablewidget_selection_changed();

    void slot_pushbutton_search_clicked();

    void slot_pushbutton_showall_clicked();

    void slot_pushButton_timeline_clicked();

    void slot_time_line(qint64, qint64);

    void slot_tablewidget_cell_clicked(int row, int column);

    void slot_add_notes_pushbutton_clicked(QString text);

    void slot_act_remove_tags();


    void slot_goto_snapshots_record(QString record_str, QString tab_name);
    void slot_thread_zip_file_export_finished();
    void slot_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path);
    void slot_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip, QString password_enc_zip);
    void slot_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure, QString export_dmg_path);
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
    void slot_on_tab_changed_clicked(int);
    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);
    void slot_checkbox_timeline_toggled(bool checked);

private:

    void contextMenuEvent(QContextMenuEvent *evt);

    void populate_data_in_tablewidget_for_multitabs(QTableWidget *current_table_widget, QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void display_data(QString dest_db_path);

    void tablewidget_general_clicked(int row, int column);

    QString get_command_according_to_table_name(QString table_name);

    void search_keyword_from_db();

    QList<m_tablewidget *> list_tablewidgets;

    QStringList display_tab_list;

    m_tablewidget *m_tablewidget_multitabs;

    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    QPushButton *pushbutton_timeline;
    int current_tab_index = -1;

    int file_name_index = 0;
    int file_size_index = 0;

    int file_extension = 0;
    int modification_time_index = 0;
    int change_time_index = 0;
    int last_access_time_index = 0;
    int created_time_index = 0;

    int bookmark_position = 0;
    int tag_position  = 0;
    int notes_icon_index = 0;
    int notes_value_index = 0;
    int blank_column_index = 0;


    int timeline_col_index = 0;


    qint64 timeline_start_time = 0;
    qint64 timeline_end_time = 0;

    timeline_selection_window *timeline_selection_window_obj;

    bool bool_timeline_is_selected;



    QString table_name;
    QString display_tab_name;
    QString secondary_snapshot;
    QString primary_snapshot;


    void create_snapshots_right_click();


    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();


    void action_open_full_detailed_info_triggered();
    void action_export_triggered();
    void action_add_note_triggered();
    void action_remove_note_triggered();


    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_data, QString colour);

    file_export_dialog *file_export_dialog_obj;
    thread_zip_file_export *thread_zip_file_export_obj;

    void action_goto_source_triggered();

    recon_helper_process *recon_helper_process_obj;
    bool bool_dmg_export_done,bool_zip_export_done;
    QString zip_export_file_path,dmg_export_file_path;
    void point_to_export_dir(QString path);

    QMap<int, QList<custom_table_header_widget *>> tab_custom_header_snapshot_widgets_QMap;
    QTableWidget *current_table_widget;
    QList<custom_table_header_widget *> custom_header_widgets_snapshot_QList;

    void all_column_text_search(QList<struct_search_column_text_for_file_type> search_text_all_column_list);
    void refresh_horizontal_header();
    void clear_all_custom_header_widget_entries();
    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;
    QCheckBox *checkbox_timeline;

};



#endif // snapshot_loader_H
