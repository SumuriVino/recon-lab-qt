#ifndef SAVED_GRAPHS_LOADER_H
#define SAVED_GRAPHS_LOADER_H


#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"


class saved_graphs_loader : public table_result_loader
{
public:

    explicit saved_graphs_loader(QWidget *parent = 0);
    void pub_set_saved_graphs_display_db_path(QString db_path);
    void saved_graphs_populate_data_in_table();

private slots:
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_goto_saved_graphs_record(QString record_str);
private:

    int creation_date_index;

    QString destination_db_path_for_saved_graphs;
    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void update_tags_value(QString tag_data, QString colour);
    void update_notes_value(QString text, int current_row);
    void action_remove_note_triggered();
    void action_open_full_detailed_info_triggered();
    void action_open_detach_detailed_info_triggered();
    void action_export_triggered();
    void update_bookmark_value(QString status, int row, int column);
};

#endif // SAVED_GRAPHS_LOADER_H
