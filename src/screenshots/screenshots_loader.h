#ifndef SCREENSHOTS_LOADER_H
#define SCREENSHOTS_LOADER_H


#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"


class screenshots_loader : public table_result_loader
{
public:

    explicit screenshots_loader(QWidget *parent = 0);
    void pub_set_screenshot_display_db_path(QString db_path);
    void screenshots_populate_data_in_table();

    void pub_set_creation_datetime_conversion(bool status);
private slots:
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_goto_screenshot_record(QString record_str);
    void slot_sort_tablewidget_display(int column_index);
private:

    int creation_date_index;
    int map_file_path_index;
    int latitude_index;
    int longitude_index;
    int tmline_graph_duration_index;

    bool convert_creation_datetime;

    int populate_data_in_tablewidget_artfacts_tmline_svd_graphs(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    int populate_data_in_tablewidget_saved_maps(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    int populate_data_in_tablewidget_screenshots(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void display_data(QString dest_db_path);

    QString destination_db_path_for_screenshots;
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
    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
};

#endif // SCREENSHOTS_LOADER_H
