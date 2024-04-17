#ifndef LOG_VIEWER_INTERFACE_H
#define LOG_VIEWER_INTERFACE_H

#include "report/report_essentials/report_status.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"

#include "progress_bar/progress_bar_window.h"

#include <QWidget>
#include <QMainWindow>
#include <QtSql/QtSql>
#include <QFileDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QShortcut>
#include <QKeyEvent>
#include <QAction>
#include <QtCore>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QSqlQuery>

#include "add_notes/add_notes.h"
#include "header_global_variables_1.h"
#include "add_tags/add_tags.h"
#include "header_identical_2.h"
#include "header_global_variables_2.h"

#include "add_tags/remove_multiple_tag_selecotor.h"
#include "header_global_variables_3.h"                         
#include "global_connection_manager.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"

namespace Ui {
class log_viewer_interface;
}

class log_viewer_interface : public QWidget
{
    Q_OBJECT

public:
    explicit log_viewer_interface(QWidget *parent = 0);
    ~log_viewer_interface();

    void keyPressEvent(QKeyEvent *event);
    void set_viewer_display_name(QString display_nm);
    void set_initialisation();
    void set_db_path(QString path);
    void pub_set_global_manager_obj(global_connection_manager *obj);
    void fetch_metadata_for_log_viewer();

signals:
    void signal_add_tags(QString tag_name, QString color_name);
    void signal_add_tag_from_log_viewer(QString, QString, QString, QString, QString, QString, QString, QString, QString);

    void signal_save_openwith_changes_to_bucket(QString viewer_type);

    void signal_set_metadata_info(struct_global_set_metadata_info m_obj);
    void signal_update_tag_submenu_of_plugins(bool status, QString new_tag_name, QString new_color_name);
    void signal_statusbar_progress_bar_show_hide(bool status, QString message);
    void signal_add_tags_in_case_tree(QString);

public slots:
    void slot_log_viewer_db_path_changed(QString db_path);

private slots:

    void on_lineEdit_search_textChanged(const QString &arg1);
    void slot_action_create_tag_triggered();
    void slot_sub_menu_tags_clicked(QAction *action);
    void slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour);

    void on_pushButton_search_clicked();
    void slot_abouttoshow_tags_in_submenu();
    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void ShowContextMenu(const QPoint &pos);

    void on_textBrowser_log_viewer_selectionChanged();
    void slot_update_tags_value(QString tag_data, QString colour);
    void on_pushButton_refresh_clicked();

    void on_pushButton_search_previous_clicked();

    void on_pushButton_search_next_clicked();
    void on_lineEdit_search_returnPressed();

private:
    Ui::log_viewer_interface *ui;

    void display_logs_view();

    QString destination_db_path;
    bool bool_alt_keypress = false;
    QString log_viewer_display_name;

    QString log_db_table_name;
    QString destination_db_file;
    QString log_file_path;
    QString record_no;
    QString source_count_name;

    void get_info_from_log_index_table();
    QString get_log_viewer_name();

    void display_tags_submenu();

    add_tags *add_tags_module_obj;

    remove_multiple_tag_selecotor *remove_multiple_tag_selecotor_obj;

    void create_table_log_export_content();
    global_connection_manager *global_connection_manager_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    QString complete_log_file_path;

    QString recon_file_from_str;
    QString recon_file_info_path_str;

    recon_helper_standard *recon_helper_standard_obj;
    QList<QTextCursor> list_highlight_cursor;

    int current_search_index;

    QMenu *mainMenu;
    QMenu *sub_menu_tags;

    QAction *act_select_all;
    QAction *act_already_saved_tags;

    void action_submenu_tag_triggered(QString tag_name);
};

#endif // LOG_VIEWER_INTERFACE_H
