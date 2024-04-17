#ifndef R_TREEWIDGET_H
#define R_TREEWIDGET_H

#include <QTreeWidget>
#include <QtCore>
#include <QAction>
#include <QMenu>
#include "header_identical_1.h"
#include "header_gui_settings.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_2.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"

class R_Treewidget : public QTreeWidget
{
    Q_OBJECT

public:
    R_Treewidget(QWidget *parent = 0);
    ~R_Treewidget();

signals:

    void signal_tree_run_plugins(void);
    void signal_tree_keyword_search(void);
    void signal_artifacts_timeline_full(void);
    void signal_tree_global_report(void);
    void signal_tree_close_case(void);
    void signal_tree_add_source(void);

    void signal_action_k_search_bookmark_all();
    void signal_action_k_search_remove_bookmark_all();
    void signal_action_k_search_goto_record();
    void signal_right_click_remove_selected_item();
    void signal_right_click_add_to_text_indexing(QString item_text, QString source_count_name);
    void signal_right_click_remove_story(QString item_text);
    void signal_right_click_carve_unallocated_space(QString source_count_name,QString item_text, bool bool_carve_unallocated);
    void signal_right_click_snapshot_clicked(QString source_count_name);
    void signal_right_click_disk_hex_view(QString item_text, QString source_count_name, bool bool_complete_soure);
    void signal_right_click_view_forensic_image_in_hex_viewer(QString source_count_name, bool bool_complete_source);


private slots:
    void onCustomContextMenuRequested(const QPoint &pos);

    void slot_action_run_plugins();
    void slot_action_keyword_search();
    void slot_action_artifacts_timeline_full();
    void slot_action_global_report();
    void slot_action_close_case();
    void slot_action_add_source_case();


    void slot_action_k_search_bookmark_all();
    void slot_action_k_search_remove_bookmark_all();

    void slot_action_k_search_goto_record();

    void slot_action_remove_item();

    void slot_submenu_extract_added_sources_triggered(QAction *clicked_action);
    void slot_submenu_extract_apple_metadata_triggered();
    void slot_submenu_extract_signature_triggered();
    void slot_submenu_extract_exif_data_triggered();
    void slot_submenu_extract_file_hashes_triggered();
    void slot_submenu_extract_fs_all_triggered();

    void slot_action_add_to_text_indexing();
    void slot_action_remove_story();
    void slot_action_carve_unallocated_space();

    void slot_action_snapshots_apfs_clicked();

    void slot_action_disk_hex_viewer_triggered();
    void slot_action_view_forensic_image_in_hex_viewer();

private:

    QMenu *menu;
    QAction *action_snapshots_apfs;

    int tree_type;

    void common_action_run_plugins();
    void common_action_keyword_search();
    void common_action_artifacts_timeline_full();
    void common_action_global_report();
    void common_action_close_case();
    void comman_action_add_source();


    void common_action_k_search_bookmark_all();
    void common_action_k_search_remove_bookmark_all();
    void common_action_k_search_goto_record();

    QMenu *menu_delete_from_tree;
    QAction *action_delete_item;
    QMenu *menu_case_tree_sources;
    QAction *action_add_to_indexing;

    void keyPressEvent(QKeyEvent *event);

    QMenu *menu_story_board;
    QAction *action_remove_story;

    QAction *action_carve_unallocated_space;

    bool check_fs_type_supported_for_carve_unallocated(QString fs_type);

    QAction *action_disk_hex_viewer;
    bool bool_complete_source_for_hex_view = false;
    QString source_count_name_for_root_node;
    QMenu *menu_disk_hex_viewer;
    QAction *action_disk_hex_viewer_unallocated;
    QAction *action_view_forensic_image_in_hex_viewer;

};

#endif // R_TREEWIDGET_H
