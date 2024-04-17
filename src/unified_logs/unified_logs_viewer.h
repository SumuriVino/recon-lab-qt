#ifndef UNIFIED_LOGS_VIEWER_H
#define UNIFIED_LOGS_VIEWER_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QSqlDatabase>
#include <QtCore>
#include <QSqlQuery>
#include "unified_logs/unified_logs_proxy_model.h"
#include "header_global_variables_2.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "unified_logs_model.h"
#include "m_tableview.h"
#include "header_global_enum.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "header_global_enum.h"
#include "global_connection_manager.h"
#include "header_global_structures.h"
#include "add_tags/add_tags.h"


QT_BEGIN_NAMESPACE
namespace Ui { class unified_logs_viewer; }
QT_END_NAMESPACE

class unified_logs_viewer : public QWidget
{
    Q_OBJECT

public:
    unified_logs_viewer(QWidget *parent = nullptr);
    ~unified_logs_viewer();

    void set_essentials_for_unified_logs();
    void fetch_metadata_for_unified_logs_viewer(int row);
    void set_viewer_display_name(QString display_nm);
    void pub_set_global_manager_obj(global_connection_manager *obj);

signals:
    void signal_set_metadata_info(struct_global_set_metadata_info m_obj);

private slots:
    void on_pushButton_search_clicked();
    void slot_tableview_single_clicked(QModelIndex);

    void on_lineEdit_search_returnPressed();
    void on_lineEdit_search_textChanged(const QString &arg1);

    void slot_arrow_key_pressed_on_tableview();
    void on_pushButton_refresh_clicked();
    void slot_sub_menu_tags_clicked(QAction *current_clicked_action);
    void slot_add_tags(QString tag_data, QString colour);

private:
    Ui::unified_logs_viewer *ui;

    QPlainTextEdit *textEdit;
    QScrollBar  *scrollBar;
    QMenu *mainMenu;

    unified_logs_model *unified_logs_model_obj;
    unified_logs_proxy_model *unified_logs_proxy_model_obj;

    QString unified_logs_display_name;
    QString source_count_name;
    m_tableview *m_tableview_obj;
    recon_helper_standard *recon_helper_standard_obj;

    QMenu *sub_menu_tags;
    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    add_tags *add_tags_module_obj;
    global_connection_manager *global_connection_manager_obj;

    void contextMenuEvent(QContextMenuEvent *evt);
    void create_main_menu();
    QMenu *create_right_click_submenu(QString submenuStr, QMenu *mainMenu, QMenu *subMenu);
    void create_tags_submenu();
    void action_create_tags();
    void update_tags_value(QString tag_data, QString colour);
    void action_submenu_tagcolor_triggered(QString tag_name);
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    bool bool_search_triggered = false;

};
#endif // UNIFIED_LOGS_VIEWER_H
