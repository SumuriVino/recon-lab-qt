#ifndef registry_interface_H
#define registry_interface_H

#include <QWidget>
#include <QtCore>
                                   
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "global_connection_manager.h"
#include "QTreeWidgetItem"
#include "add_tags/add_tags.h"
#include"header_gui_settings.h"
#include "header_global_variables_2.h"

#include "add_notes/add_notes.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class registry_interface;
}

class registry_interface : public QWidget
{
    Q_OBJECT

public:
    explicit registry_interface(QWidget *parent = 0);
    ~registry_interface();


    void set_viewer_display_name(QString display_nm);
    void set_initialisation();
    void pub_set_destination_db_path(QString path);
    void pub_draw_registry_tree();
    void pub_set_global_manager_obj(global_connection_manager *obj);

signals:
    void signal_save_openwith_changes_to_bucket(QString);
    void signal_add_tags(QString tag_name,QString colour_name);
    void signal_set_metadata_info(struct_global_set_metadata_info m_obj);
    void signal_clear_all_metadata();

private slots:

    void on_treeWidget_registry_itemClicked(QTreeWidgetItem *item, int column);
    void on_tableWidget_registry_cellClicked(int row, int column);
    void slot_registry_viewer_menu_right_clicked(QAction *current_clicked_action);
    void slot_tags_submenu_clicked(QAction *current_clicked_action);
    void slot_add_tags(QString tag_data, QString colour);
    void slot_act_create_tags();
    void slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour);
    void slot_add_notes_pushbutton_clicked(QString note_text);
    void on_treeWidget_registry_itemExpanded(QTreeWidgetItem *item);
    void slot_hide_loading_display_dialog_box(bool);
    void on_tableWidget_registry_itemClicked(QTableWidgetItem *item);
    void slot_registry_viewer_db_path_changed(QString dest_db_path);

    void on_pushButton_search_clicked();
    void on_tableWidget_searched_items_cellClicked(int row, int column);
    void on_tableWidget_searched_items_itemClicked(QTableWidgetItem *item);
    void on_tableWidget_registry_itemSelectionChanged();
    void on_tableWidget_searched_items_itemSelectionChanged();
    void on_pushButton_refresh_clicked();
    void on_tabWidget_tabBarClicked(int index);

    void slot_sort_tablewidget_display(int column_index);
    void slot_abouttoshow_tags_in_submenu();
private:

    Ui::registry_interface *ui;

    bool eventFilter(QObject *obj, QEvent *event);

    enum enum_table_widget_registry_viewer
    {
        enum_table_widget_INT_Index = 0,
        enum_table_widget_Bookmark_Index,
        enum_table_widget_Notes_Icon_Index,
        enum_table_widget_Tag_Icon_Index,

        enum_table_widget_node_name_Index,
        enum_table_widget_node_timestamp_Index,


        enum_table_widget_Key_Index,
        enum_table_widget_Type_Index,
        enum_table_widget_Values_Index,
        enum_table_widget_Hex_Values_Index,
        enum_table_widget_file_path_Index,
        enum_table_widget_node_path_Index,


        enum_table_widget_Tag_Text_Index,
        enum_table_widget_note_Text_Index,
        enum_table_widget_source_count_name_Index,
        enum_table_widget_Blank_Column_Index
    };

    enum enum_tree_widget_registry_viewer
    {
        enum_tree_widget_Item_Name_Index = 0,
        enum_tree_widget_Timestamp_Index,
        enum_tree_widget_Parent_Path_Index,
        enum_tree_widget_INT_Index,
        enum_tree_widget_Bookmark_Index,
        enum_tree_widget_Notes_Icon_Index,
        enum_tree_widget_Tag_Icon_Index,
        enum_tree_widget_Tag_Text_Index,
        enum_tree_widget_note_Text_Index,
        enum_tree_widget_source_count_name,
        enum_tree_widget_file_path,
        enum_tree_widget_node_path,
        enum_tree_widget_node_name,
        enum_tree_widget_Blank_Column_Index
    };

    enum enum_tab_index
    {
        enum_all_items_tab_index = 0,
        enum_searched_items_tab_index
    };

    QString destination_db_path;
    QString display_tab_name;
    QString initial_level_str;
    QString M_VAR_CHILD_ENTRY_QString_TEMP_in_tree_QString;
    QString plugin_name_str;
    QString tab_name_str;
    QString source_count_name_str;
    QString source_name_str;
    QString file_path_str;
    QString node_path_str;
    QString node_name_str;
    QString node_timestamp_numeric_QString;

    QAction *act_create_tag;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QMenu *sub_menu_tags;
    QMenu *mainMenu;

    bool bool_sort_ascending;
    QList<QTableWidgetItem *> takeRow(int row);
    void setRow(int row, const QList<QTableWidgetItem *> &rowItems);

    void create_tags_submenu();
    void create_right_click_actions(QMenu *mainMenu, QStringList submenuList);
    QMenu *create_right_click_submenu(QString submenuStr, QMenu *mainMenu, QMenu *subMenu);



    void action_submenu_tag_triggered(QString tag_name);

    void action_bookmark_triggered();
    void action_remove_bookmarks_triggered();

    void action_add_note_triggered();
    void action_remove_note_triggered();

    void action_remove_tag_triggered();

    void action_copy_to_clopboard_triggered();
    recon_helper_standard *recon_helper_standard_obj;


    QTreeWidgetItem *get_currentptr_on_tree_iteration(QString parentpath);
    void set_table_items(QTreeWidgetItem *item);
    void draw_registry_tree();


    QString get_node_name(QString recevied_path);
    void create_registry_viewer_right_click_menu();


    global_connection_manager *global_connection_manager_obj;

    add_notes *add_notes_obj;
    add_tags *add_tags_module_obj;


    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    display_loading_progress_bar *display_loading_progress_bar_obj;

    bool bool_cancel_loading;
    void set_metadata_on_table_widget_click();
    void set_metadata_on_treewidget_click(QTreeWidgetItem *item);

    QString get_searched_command(QStringList search_on_columns_list);
    void draw_searched_items_in_table();
    void set_bookmark(int row, int column);
    QTableWidget *get_m_tablewidget_obj_for_current_tab();

    QMenu *submenu_bookmark;
    QMenu *submenu_note;

};

#endif // registry_interface_H
