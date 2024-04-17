#ifndef REDEFINED_RESULT_SAVED_LOADER_H
#define REDEFINED_RESULT_SAVED_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
//#include "generic_report_builder/generic_report_builder.h"


class redefined_result_saved_loader : public table_result_loader
{
public:
    explicit redefined_result_saved_loader(QWidget *parent);
    ~redefined_result_saved_loader();

    void pub_set_destination_db_path(QString db_path);
    void redefined_saved_result_populate_data_in_table();


private slots:

    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_act_remove_tags();
    void slot_pushbutton_search_clicked();
signals:



private:

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_export_triggered();

    void action_sendToBucket_plist_viewer_triggered();
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void action_open_detach_detailed_info_triggered();
    void action_open_full_detailed_info_triggered();

    void action_remove_note_triggered();
    void action_remove_note_bookmark_triggered();

    void update_bookmark_value(QString status, int row, int column);


    void update_notes_value(QString text, int current_row);
    void update_tags_value(QString tag_data, QString colour);



    QString destination_redefined_result_saved_db_path;

    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void display_data(QString dest_db_path);
    int populate_data_in_tablewidget_browser_history(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    int populate_data_in_tablewidget_messenger(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    int visit_count_index;

    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
};

#endif // REDEFINED_RESULT_SAVED_LOADER_H
