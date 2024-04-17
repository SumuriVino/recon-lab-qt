#ifndef artifacts_timeline_saved_loader_H
#define artifacts_timeline_saved_loader_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"


class artifacts_timeline_saved_loader : public table_result_loader
{
public:
    explicit artifacts_timeline_saved_loader(QWidget *parent);
    ~artifacts_timeline_saved_loader();

    void pub_set_display_db_path(QString db_path);
    void populate_data_in_table();

    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString rec_no_str);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString rec_no_str);

private slots:

    void slot_act_export_as_csv_triggered();
    void slot_act_export_as_xml_triggered();

    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);


    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);
    void slot_goto_artifact_source_submenu_clicked(QAction *clicked_action);
private:

    QString destination_saved_timeline_db_path;

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_open_detach_detailed_info_triggered();
    void action_open_full_detailed_info_triggered();

    void action_sendToBucket_plist_viewer_triggered();
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void action_export_triggered();
    void action_remove_note_triggered();
    void action_remove_note_bookmark_triggered();
    void update_notes_value(QString text, int current_row);

    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_data, QString colour);

    QPushButton *pushButton_export;
    QLabel *label_tab_details;
    QAction *action_export_as_csv;
    QAction *action_export_as_xml;
    QMenu *menu_export;

    int index_for_INT_for_saved_artifact_timeline;

    int stamp_type_index;
    int bookmark_column_index;
    int tag_column_index;
    int notes_column_index;

    void update_bookmarks_in_bookmark_serach(QString status, int selected_row, QString dest_db_path);
    void update_tags_in_tag_serach_db(QString current_tag, QString dest_db_path);
    void update_note_in_notes_serach(QString text_str, int selected_row, QString dest_db_path);

    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();

    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag, QString record_no, QString plugin_name, QString tab_name, QString stamp_type_str, QString source_count_name);
    void display_data(QString dest_db_path);

    void action_go_to_record_triggered();
    void create_action_for_goto_artifact_source_submenu();

};

#endif //artifacts_timeline_saved_loader_H
