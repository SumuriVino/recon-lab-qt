#ifndef CARVED_DATA_LOADER_H
#define CARVED_DATA_LOADER_H


#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"


class carved_data_loader : public table_result_loader
{
public:

    explicit carved_data_loader(QWidget *parent = 0);
    void pub_set_carved_data_display_db_path(QString db_path);
    void populate_data_in_table();

private slots:
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_pushbutton_search_clicked();
    void slot_goto_screenshot_record(QString record_str);

private:

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void display_data(QString dest_db_path);
    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);


    void action_open_full_detailed_info_triggered();
    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void action_add_note_triggered();
    void action_remove_note_triggered();


    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_name, QString colour);

    QMenu *submenu_open_files_with_other_app;

    QString current_table_name;
    QString category_name ;
    QString carved_data_tab_name;

    int root_count_name_index;
    int source_plugin_name_index;
    int source_record_no_index;
    int source_filename_index;
    int source_filepath_index;
    int source_tab_name_index;
    int number_of_hits_name_index;
    int category_name_index;
    int carved_keyword_index;
    int bookmark_index;
    int tag_icon_index;
    int notes_icon_index;

    QString bookmark_status;
    QString tag_name_str;

    QString recon_file_from;
    QString recon_file_info_path;

    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    QString is_tagged_current_record_record(int selected_row_no, QString record_no, QSqlDatabase &dest_db);
};

#endif // CARVED_DATA_LOADER_H
