#ifndef PLIST_VIEWER_INTERFACE_H
#define PLIST_VIEWER_INTERFACE_H

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
#include<QShortcut>
#include<QKeyEvent>
#include<QAction>
#include<QtCore>
#include<QLabel>
#include <QAction>
#include <QMenu>

#include "add_notes/add_notes.h"
#include "plist_viewer_report_html_template.h"
#include "header_global_variables_1.h"
#include "add_tags/add_tags.h"
#include "header_identical_2.h"
#include "header_global_variables_2.h"

#include "add_tags/remove_multiple_tag_selecotor.h"
#include "header_global_variables_3.h"                         
#include "header_report.h"
#include "global_connection_manager.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "manage_plist_key/manage_plist_key.h"


namespace Ui {
class plist_viewer_interface;
}

class plist_viewer_interface : public QWidget
{
    Q_OBJECT

public:
    explicit plist_viewer_interface(QWidget *parent = 0);
    ~plist_viewer_interface();

    void keyPressEvent(QKeyEvent *event);
    void set_viewer_display_name(QString display_nm);
    void set_initialisation();
    void set_db_path(QString path);

    void pub_set_global_manager_obj(global_connection_manager *obj);

    QStringList keys_list_of_plist_file;
    QStringList values_list_of_plist_file;

signals:
    void signal_add_tags(QString tag_name, QString color_name);
    void signal_add_tag_from_plist_viewer(QString, QString, QString, QString, QString, QString, QString, QString, QString);

    void signal_remove_tags_from_plist_viewer();
    void signal_plist_viewer_removed_tags_list(QString fs_plugin_name, QString tab_name, QString record_no_prepended_zero, QStringList list_removed_tags, QString os_scheme);
    void signal_update_tag_search_db_and_display(QString fs_plugin_name, QString tab_name, QString record_no_prepended_zero, QStringList list_tags_remained,QString os_scheme);
    void signal_save_openwith_changes_to_bucket(QString viewer_type);

    void signal_plist_item_clicked(QString searched_text,QString data, QString current_media, QString source_count_name);

    void signal_update_tag_submenu_of_plugins(bool status, QString new_tag_name, QString new_color_name);
    void signal_statusbar_progress_bar_show_hide(bool status, QString message);
    void signal_add_tags_in_case_tree(QString);

public slots:
    void slot_plist_viewer_db_path_changed(QString db_path);
private slots:

    void on_pushButton_report_clicked();
    void on_lineEdit_search_textChanged(const QString &arg1);
    void on_treeWidget_plist_itemClicked(QTreeWidgetItem *item, int column);
    void slot_plist_menu_clicked(QAction *);
    void slot_add_notes(QString data);
    void slot_add_notes_to_bookmarks(QString data);

    void on_treeWidget_plist_itemExpanded(QTreeWidgetItem *item);

    void slot_action_create_tag_triggered();
    void slot_sub_menu_tags_clicked(QAction *action);
    void slot_add_tags_plist_viewer_record(QString tag_name, QString colour_name);
    void slot_update_tag_selected_bytes_submenu(bool status, QString new_tag, QString tag_colour);
    void slot_remove_tags_from_plist_viewer();
    void slot_tag_list_after_remove_multiple_tag(QStringList list_tags_remained, QString table_name, QString record);
    void slot_removed_tags_list_plist_viewer(QStringList list_removed_tags, QString table_name, QString record_no);

    void slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour);

    void on_treeWidget_plist_itemChanged(QTreeWidgetItem *item, int column);

    void on_pushButton_refresh_treewidget_data_clicked();
    void on_pushButton_search_clicked();
    void slot_abouttoshow_tags_in_submenu();
    void slot_act_manage_keys_triggered();
    void slot_update_conversion_type_on_combobox_conversion_change(QString data_str, QString key_name);

private:
    Ui::plist_viewer_interface *ui;

    enum enum_plist_viewer_interface_Tree_Nodes
    {
        enum_plist_viewer_interface_TREE_TAG = 0,
        enum_plist_viewer_interface_TREE_NOTE_ICON,
        enum_plist_viewer_interface_TREE_KEY,
        enum_plist_viewer_interface_TREE_TYPE,
        enum_plist_viewer_interface_TREE_VALUE,
        enum_plist_viewer_interface_TREE_KEYPATH,
        enum_plist_viewer_interface_TREE_INT,
        enum_plist_viewer_interface_TREE_NOTES,
        enum_plist_viewer_interface_TREE_TAG_NAMES

    };

    enum enum_plist_viewer_interface_Tree_Reports
    {
        enum_plist_viewer_interface_TREE_REPORT_TAGS = 0,
        enum_plist_viewer_interface_TREE_REPORT_FULL
    };
    enum enum_plist_viewer_interface_Tree_Static_Levels
    {
        enum_plist_viewer_interface_TREE_STATIC_LEVEL_FIRST = 0,
        enum_plist_viewer_interface_TREE_STATIC_LEVEL_SECOND
    };

    enum enum_plist_viewer_interface_Reports_Type
    {
        enum_plist_viewer_interface_TREE_REPORT_HTML = 0,
        enum_plist_viewer_interface_TREE_REPORT_XML
    };


    void display_tree_view();
    QTreeWidgetItem* get_currentptr_on_tree_iteration(QString parentpath);
    void write_Index_Entry_bookmarks(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item);
    void write_Index_Entry_full(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item);
    void treeitem_expand_recursively(QTreeWidgetItem *item);

    QString destination_db_path,report_path;
    bool bool_alt_keypress = false;
    QString plist_viewer_display_name;

    QString plist_db_table_name;
    QString destination_db_file;
    QString plist_file_path;

    void get_info_from_index_table();
    QString get_plist_viewer_name();


    QMenu *menu_plist;
    QAction *act_bookmark_all;
    QAction *act_remove_all_bookmark;
    QAction *act_add_notes;
    QAction *act_add_notes_to_bookmark;
    QAction *act_remove_notes;
    QAction *act_remove_notes_from_bookmark;
    QAction *act_remove_all_notes;

    QAction *act_copy_to_clipboard, *act_copy_column_to_clipboard;
    QMenu *submenu_tag;

    QAction *act_already_saved_tags;


    add_notes *add_notes_dialog_add_note_obj,*add_notes_dialog_add_note_bookmark_obj;

    void contextMenuEvent(QContextMenuEvent *);
    void add_notes_act();
    void add_notes_to_bookmarks_act();
    void remove_notes_act();
    void remove_notes_from_bookmarks_act();
    void remove_all_notes_act();
    void create_html_reports();

    void display_tags_submenu();

    add_tags *add_tags_module_obj;
    QStringList get_tags_and_colours_list_from_db(QString command, QString destination_db_file);
    void create_right_clicks();

    QString get_tag_color_hex_from_tag_db(QString tag_name, QString color_name);
    QString update_tag_status_for_current_record(QString tag_data, QString colour, QString record_no_str);

    remove_multiple_tag_selecotor *remove_multiple_tag_selecotor_obj;
    void fetch_metadata_for_plist_viewer(QTreeWidgetItem *item);
    void action_copy_row_to_clipboard_from_treewidget();
    void action_copy_cell_to_clipboard_from_treewidget();

    QStringList get_values_from_plist_db_according_to_display_tab(QString record_no);

    void create_table_export_content();
    global_connection_manager *global_connection_manager_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    QString complete_file_path;

    QString recon_file_from_str;
    QString recon_file_info_path_str;

    recon_helper_standard *recon_helper_standard_obj;

    QAction *act_media_keys;
    manage_plist_key *manage_plist_key_obj;
    QString converted_data(QString value_str, QString current_key);
    QString get_data_conversion_type_of_current_selected_key(QString current_key);

    QMenu *submenu_note;
    QMenu *submenu_bookmark;
    QAction *act_seperator_note;

    bool bool_for_the_unicode_conversion = false;
};

#endif // PLIST_VIEWER_INTERFACE_H
