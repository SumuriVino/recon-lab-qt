#ifndef DISK_IMAGES_LOADER_H
#define DISK_IMAGES_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "launcher/source_selector/source_widget/source_evidencer.h"



class disk_images_loader : public table_result_loader
{
    Q_OBJECT

public:
    explicit disk_images_loader(QWidget *parent);
    ~disk_images_loader();

    void pub_populate_data_in_table();

    void pub_remove_main_record_bookmark_from_detailed_info_triggered(QString record_no_str);
    void pub_bookmark_main_record_from_detailed_info_triggered(QString record_no_str);


signals:
    void signal_add_source_for_extraction(struct_GLOBAL_witness_info_source);


private slots:

    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);

    void slot_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked_from_open_in_detail(QString record_str, QString bookmark_status);
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

    int src_type_dsply_index;
    int extraction_status_index;
    int file_path_index;
    int root_count_name_index;
    int file_name_index;

    source_evidencer *source_evidencer_obj;

    int bookmark_col_index;
    int tag_col_index;
    int notes_col_index;
    int source_nested_index;
    int source_decompressed_index;

    QString check_running_status;


};

#endif // DISK_IMAGES_LOADER_H
