#ifndef MOBILE_BACKUP_LOADER_H
#define MOBILE_BACKUP_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "launcher/source_selector/source_widget/source_evidencer.h"



class mobile_backup_loader : public table_result_loader
{
    Q_OBJECT

public:
    explicit mobile_backup_loader(QWidget *parent);
    ~mobile_backup_loader();

    void pub_populate_data_in_table();

signals:
    void signal_add_source_for_extraction(struct_GLOBAL_witness_info_source);


private slots:

    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);

private:

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void action_extract_source_triggered();
    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void action_remove_note_triggered();
    void action_open_full_detailed_info_triggered();

    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_data, QString colour);

    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);


    int backup_datetime_index;
    int backup_file_path_index;
    int source_type_display_index;
    int backup_extraction_status;
    int os_version_index;
    int source_nested_index;
    int source_decompressed_index;
     source_evidencer *source_evidencer_obj;

};

#endif // MOBILE_BACKUP_LOADER_H
