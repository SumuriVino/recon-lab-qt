#ifndef ARTIFACTS_KEYWORD_SEARCH_LOADER_H
#define ARTIFACTS_KEYWORD_SEARCH_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"


class artifacts_keyword_search_loader : public table_result_loader
{

     Q_OBJECT

public:
    explicit artifacts_keyword_search_loader(QWidget *parent);

    void configure_keyword_search_loader(QString result_dir_path, QString item_text, qint64 time_offset, QString search_name);

    ~artifacts_keyword_search_loader();

    void pub_populate_data_in_table();

    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str);
signals:
    void signal_act_quick_look_triggered();

private slots:
    void slot_tablewidget_keywords_list_item_doubleclicked(int row, int coloum);
    void slot_link_label_keywords_list(QString text);

    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_goto_artifact_source_submenu_clicked(QAction *clicked_action);
    void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);
    void slot_add_files_to_hashset_submenu_clicked(QAction *current_clicked_action);

    void slot_add_notes_to_bookmark_pushbutton_clicked(QString text);
    void slot_act_quick_look_triggered();
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);

    void slot_act_remove_tags();
    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);
private:
    QString keyword_search_detination_db_path;
    QLabel *link_label;
    QHBoxLayout *hbox_search_info_layout;
    QLabel *label_keywordsearch_name;

    QTableWidget *tablewidget_keywords_list ;

    QString keyword_search_name;
    QString  timeline_creation_time;

    int keyword_hit_index;

    void set_display_db_path(QString db_path);
    void create_keywordsearch_info_on_top_display();
    void get_creation_timestamp(QString search_name, QString destination_db_file);

    void display_searched_keywords_table(QString search_name, QString result_dir);
    QString all_keywords_str;


    void contextMenuEvent(QContextMenuEvent *evt);
    void action_open_full_detailed_info_triggered();
    void tablewidget_general_clicked(int row, int column);
    void action_open_detach_detailed_info_triggered();

    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void action_sendToBucket_plist_viewer_triggered();

    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();

    void action_export_triggered();
    void create_action_for_goto_artifact_source_submenu();
    void action_go_to_record_triggered();
    void action_remove_note_triggered();

    void action_remove_note_bookmark_triggered();
    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_data, QString colour);
    void action_bookmark_triggered();

    int bookmark_index;
    int notes_icon_index;
    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);

};

#endif //
