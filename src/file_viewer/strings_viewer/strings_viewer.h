#ifndef STRINGS_VIEWER_H
#define STRINGS_VIEWER_H

#include <QWidget>
#include <QtCore>
#include "progress_bar/progress_bar_window.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include "add_tags/add_tags.h"
#include "header_identical_2.h"
#include "header_global_variables_2.h"

#include "header_gui_settings.h"
#include "header_global_variables_3.h"                         
#include "global_connection_manager.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class strings_viewer;
}

class strings_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit strings_viewer(QWidget *parent = 0);
    ~strings_viewer();
    void pub_clear_data();
    void pub_set_essentials();

    void pub_show_file_data(QString file_path, QString record_no);

    void pub_set_source_count_name(QString str);


    void pub_highlight_keyword_hit(QString keyword_str);
    void pub_set_highlight_keyword(QString str);
    void pub_hide_search_navigate_buttons(bool status);
    void pub_set_global_manager_obj(global_connection_manager *obj);
    void pub_set_recon_file_info(struct_global_recon_file_info obj);
signals:
    void signal_add_tag_to_tag_search(QString tag_name , QString tagged_data, QString filepath, QString source_count_str, QString record_no);

private slots:
    void slot_proc_ready_read();
    void slot_proc_strings_finished(int state);
    void ShowContextMenu(const QPoint &pos);
    void slot_act_create_tags();
    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_sub_menu_tags_clicked(QAction *current_clicked_action);
    void slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour);
    void slot_add_tags(QString tag_data, QString colour);

    void on_pushButton_search_clicked();

    void on_lineEdit_search_returnPressed();

    void on_pushButton_prev_clicked();

    void on_pushButton_next_clicked();

    void slot_abouttoshow_tags_in_submenu();
private:
    Ui::strings_viewer *ui;

    QProcess *proc_strings;

    QString current_filepath;
    QString current_record_no;
    QString source_count_name;

    QMenu *mainMenu;

    ///-----Tags----
    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QMenu *sub_menu_tags;

    add_tags *add_tags_module_obj;

    QAction *act_select_all;



    void create_tags_submenu();
    void action_submenu_tag_triggered(QString tag_name);
    void update_tags_value(QString tag_data, QString colour);

    QList<QTextCursor> list_highlight_cursor;
    int current_search_index;

    QString highlight_keyword;

    global_connection_manager *global_connection_manager_obj;

    void create_table_export_content();

    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // STRINGS_VIEWER_H
