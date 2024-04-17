#ifndef carved_passwords_loader_H
#define carved_passwords_loader_H


#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"
#include "export_dialog_box/export_dialog_box_1/export_dialog_box_1.h"

class carved_passwords_loader : public table_result_loader
{
    Q_OBJECT

public:

    explicit carved_passwords_loader(QWidget *parent = 0);
    ~carved_passwords_loader();

    void pub_set_carved_passwords_display_db_path(QString db_path);
    void populate_data_in_table();
    void pub_create_ui();

private slots:

    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_act_remove_tags();
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_table_data_export_dialog_ok_clicked(QString given_name , QString selected_dir_path, QString type_str);

    void slot_goto_carved_password_record(QString record_str);
private:

    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);

    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void action_remove_note_triggered();
    void action_remove_note_bookmark_triggered();
    void action_open_full_detailed_info_triggered();
    void action_create_word_list_triggered();


    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_data, QString colour);

    QString destination_db_path;

    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    int notes_icon_index, tag_icon_index ,password_index ,server_index , account_index , description_index;

    export_dialog_box_1 *export_dialog_box_1_obj;
    void open_window_for_passwords_file(QString file_path);

};

#endif // carved_passwords_loader_H
