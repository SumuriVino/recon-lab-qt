#ifndef HEX_VIEWER_COMPLETE_H
#define HEX_VIEWER_COMPLETE_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtCore>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QAction>
#include <QSplitter>
#include <QGuiApplication>
#include <QApplication>
#include <QMessageBox>
#include <QCheckBox>
#include <QFileIconProvider>
#include <QFileDialog>
#include <QHeaderView>
#include <QMenu>
#include <QTabWidget>

#include "common_header.h"
#include "fs_parser.h"
#include "fat32_parser.h"
#include "fat16_parser.h"
#include "fs_viewer_tsk.h"
#include "FAT/fat_template.h"
#include "mbr_template.h"
#include "tag_create_popup.h"
#include "get_address_dialog.h"
#include "ntfs_vbr_template.h"
#include "mft_entry_template.h"
#include "utilities_function.h"
#include "hexviewer_progress_bar.h"
#include "Templates/gpt_template.h"
#include "hex_viewer_search_dialog.h"
#include "hex_widget/qhexedit_hexedit.h"
#include "fat_directory_entry_template.h"
#include "generic_table_template_viewer.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "global_connection_manager.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "message_box/message_dialog.h"
#include "file_viewer/hex_viewer/hex_viewer_complete/Scrollbar64.h"

class hex_viewer_complete : public QWidget
{
    Q_OBJECT
public:
    explicit hex_viewer_complete(QWidget *parent = nullptr);

    void pub_set_file_path(QString complete_filepath, QString display_filepath);
    void pub_set_source_file_size(qint64);
    void pub_set_source_block_size(int);


    void pub_set_data(QByteArray data_arr);
    void pub_downsize_hexviewer_for_metadata_display();
    void pub_set_hex_viewer_open_file_data_obj(struct_global_hex_viewer_open_file_data obj);
    void pub_set_global_connection_manager(global_connection_manager *obj);
    void pub_highlight_keyword(QString keyword_str);
    void pub_disable_ui_if_data_not_available();
signals:
    void signal_save_openwith_changes_to_bucket(QString viewer_type);

private slots:
    void slot_pushButton_show_tables_clicked();
    void slot_pushButton_search_keyword_clicked(bool);
    void slot_pushButton_search_next_clicked(bool);
    void slot_pushButton_search_previous_clicked(bool status);
    void slot_pushButton_carve_tag_clicked(bool);
    void slot_pushButton_export_tags_clicked(bool);
    void slot_pushButton_import_tags_clicked(bool);
    void slot_pushButton_remove_tag_triggered(bool);
    void slot_pushButton_save_clicked(bool);
    void slot_pushButton_read_file_as_image_clicked(bool);
    void slot_pushButton_jump_to_offset(bool status);

    void slot_action_tag_bytes_triggered(bool);
    void slot_action_start_block_triggered(bool);
    void slot_action_end_block_triggered(bool);
    void slot_action_template_fat_directory_entry_triggered(bool);
    void slot_action_template_fat_directory_entry_color_triggered(bool);
    void slot_action_template_manual_triggered(bool);
    void slot_action_template_mbr_color_triggered(bool);
    void slot_action_template_mbr_triggered(bool);
    void slot_action_template_ntfs_vbr_triggered(bool);
    void slot_action_template_mft_entry_triggered(bool);
    void slot_action_template_gpt_triggered(bool);
    void slot_action_template_fat32_vbr_triggered(bool);
    void slot_action_template_fat_vbr_triggered(bool);
    void slot_action_copy_selected_bytes_triggered(bool status);
    void slot_action_copy_selected_ascii_triggered(bool status);

    void slot_selected_data_offsets(qint64 start_address, qint64 end_address);
    void slot_cursor_position(qint64 pos);
    void slot_comboBox_bytes_per_line_current_index_changed(QString);
    void slot_hex_widget_right_clicked();
    void slot_hex_widget_left_clicked();
    void slot_save_tag_details(qint64 start, qint64 end, QString tag_name, QString color_hex);
    void slot_tab_tablewidget_cell_double_clicked(int row, int column);
    void slot_tableWidget_marker_cell_double_clicked(int row, int column);
    void slot_search_text_and_type(QString, QString, bool bool_clear_prev_list);
    void slot_checkBox_little_endian_toggeled(bool);
    void slot_goto_fat32_file_content(int part_num, qint64 inode_number);
    void slot_goto_fat32_$files_content(int part_num, QString file_name);
    void slot_goto_fat_file_content(int part_num, qint64 inode_number);
    void slot_goto_fat_$files_content(int part_num, QString file_name);
    void slot_goto_ntfs_file_content(int part_num, qint64 inode_number);
    void slot_show_fat_template(int part_num, QString fat_name);
    void slot_goto_offset(qint64);
    void slot_jump_to_offset(qint64 offset, int);
    void slot_hide_loading_display_dialog_box(bool status);
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
private:

    enum enum_TAG_TABLE_COLUMNS{
        enum_TAG_TABLE_INT,
        enum_TAG_TABLE_START,
        enum_TAG_TABLE_END,
        enum_TAG_TABLE_TAG_NAME,
        enum_TAG_TABLE_TAG_COLOR,

        enum_TAG_TABLE_TAG_HEX_DATA,

        enum_TAG_TABLE_DESCRIPTION,
        enum_TAB_TABLE_BLANK
    };

    enum marker_table_widget_columns{
        enum_marker_table_INT,
        enum_marker_table_offset_dec,
        enum_marker_table_offset_hex,
        enum_marker_table_partition,
        enum_marker_table_item,
        enum_marker_table_description,
        enum_marker_table_blank,
        enum_marker_table_column_count
    };


    QLabel *label_address_column;
    QLabel *label_bytes_per_line;
    QTabWidget *main_tab_widget_obj;
    QFrame *toolbar_widget;
    QComboBox *comboBox_bytes_per_line;
    QWidget *side_widget;

    QLineEdit *lineEdit_search;
    QComboBox *comboBox_search_content_type;
    QPushButton *pushButton_search_keyword;
    QPushButton *pushButton_search_next;
    QPushButton *pushButton_search_previous;
    QPushButton *pushButton_jump_to_offset_in_file;
    QPushButton *pushButton_save;
    QTableWidget *tableWidget_tags_list;
    QPushButton *pushButton_show_hide_tags;
    QPushButton *pushButton_read_file_as_image;


    QFrame *tag_and_table_widget;
    QFrame *tag_bar_widget;
    QPushButton *pushButton_remove_tag;
    QPushButton *pushButton_carve_tag;
    QPushButton *pushButton_export_tags;
    QPushButton *pushButton_import_tags;

    QFrame *marker_table_widget_frame;
    QTableWidget   *tableWidget_marker;


    QLabel *label_selection_start;
    QLabel *label_selection_count;
    QLabel *label_cursor_position;

    QLabel *label_filename_title;
    QLabel *label_filepath_title;
    QLabel *label_filesize_title;
    QLabel *label_filename_value;
    QLabel *label_filepath_value;
    QLabel *label_filesize_value;
    QCheckBox *checkBox_little_endian;

    QTableWidget *tableWidget_data_interpreter;

    QMenu *menu_templates;
    QAction *action_template_mbr;
    QAction *action_template_gpt;
    QAction *action_template_fat_vbr;
    QAction *action_template_fat32_vbr;
    QAction *action_template_fat_directory_entry;
    QAction *action_template_ntfs_vbr;
    QAction *action_template_mft_entry;
    QAction *action_template_standard_attribute;

    QMenu *menu_templates_color;
    QAction *action_template_mbr_color;
    QAction *action_template_fat_vbr_color;
    QAction *action_template_fat_directory_entry_color;
    QAction *action_template_standard_attribute_color;
    QAction *action_template_manual;

    QAction *action_tag_bytes;
    QAction *action_start_block;
    QAction *end_block;
    QMenu *menu_right_click;
    QMenu *menu_right_click_copy;


    bool bool_cancel_loading = false;
    bool bool_message_box_yes_button_clicked = false;
    bool bool_message_box_clicked = false;

    QString source_file_path;
    QString current_search_text, current_search_type;
    int source_file_block_size;
    qint64 source_file_size;
    qint64 selection_start, selection_end;
    qint64 current_text_searched_pos;
    qint64 current_cursor_position = 0, current_position = 0;
    qint64 marked_start_offset = 0;

    QFile data_file;
    QFileDialog file_dialog_obj;

    QList<struct_hex_viewer_tag> hex_viewer_tags_list;
    QList<struct_hex_viewer_markers> marker_list;
    QList<partition_detail> image_part_list;
    QStringList search_saved_offsets_list;


    QHexEdit_hexedit *hex_widget;
    fs_parser fs_parser_obj;
    tag_create_popup *tag_create_popup_obj;
    get_address_dialog *get_address_dialog_obj;
    hex_viewer_search_dialog *hex_viewer_search_dialog_obj;
    utilities_function utility_obj;
    fat32_parser fat32_parser_obj;
    hexviewer_progress_bar *hexviewer_progress_bar_obj;
    recon_helper_standard  *recon_helper_standard_obj;
    struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;
    global_connection_manager *global_connection_manager_obj;
    display_loading_progress_bar *display_loading_progress_bar_obj;
    message_dialog  *message_dialog_obj;


    void create_ui();
    void create_and_add_marker_widget();
    void create_tag_display();
    void create_data_interpreter_display();
    void create_marker_display();
    QByteArray data_block_from_source(qint64 offset, int size);
    void save_details_in_hex_viewer_feature_database(struct_hex_viewer_tag tag);
    void save_details_in_hex_viewer_export_content_database(struct_hex_viewer_tag tag, QString exported_file_path);
    QString save_tagged_data_to_hex_export_content(struct_hex_viewer_tag tag);
    void fill_the_existing_blocks_details_in_list();
    void remove_tag_from_hex_features_database_and_export_details(struct_hex_viewer_tag tag_obj, int row_count);
    bool check_is_data_available();
    QString get_tagged_data(qint64 start_offset, qint64 end_offset);
    QByteArray get_selected_data_block_from_source(qint64 start_offset, int size);

    Scrollbar64 *scroll_bar_64_obj;
};

#endif // HEX_VIEWER_COMPLETE_H
