#ifndef ram_images_loader_H
#define ram_images_loader_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "launcher/source_selector/source_widget/source_evidencer.h"


class ram_images_loader : public table_result_loader
{
    Q_OBJECT

public:
    explicit ram_images_loader(QWidget *parent);
    ~ram_images_loader();

    void populate_data_in_table();
    void pub_set_ram_images_display_db_path(QString dest_db_path);

signals:
    void signal_carve_data_clicked(QString source_count_name, QString file_path, QString plugin_name, QString tab_name, QString record_no);
    void signal_carve_files_clicked(QString source_count_name, QString file_path, bool bool_carve_files);
private slots:

    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);

private:

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void action_extract_carved_data_triggered();
    void action_extract_carved_files_triggered();

    void action_open_full_detailed_info_triggered();

    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);


    int record_no_index;
    int plugin_name_index;
    int tab_name_index;
    int extraction_status_index;
    int source_name_index;
    int source_path_index;
    int source_size_index;
    int source_type_display_index;
    int source_nested_index;
    int source_decompressed_status_index;
    int root_count_name_index;
    int recon_file_path_index;
    int recon_file_from_index;

    int bookmark_index;
    int recon_tag_icon_index;
    int notes_icon_index;
    int notes_text_index;

    QString csv_tab_name;
    QString db_table_name;
};

#endif // ram_images_loader_H
