#ifndef HEX_VIEWER_H
#define HEX_VIEWER_H

#include <QWidget>
#include <QtCore>
#include "tt_old_hex_viewer_qhexedit.h"
#include "header_global_variables_3.h"                         
//#include "recon_static_functions.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "tt_old_tag/tt_old_hex_viewer_export_tag_module.h"
#include "header_identical_1.h"
#include <QtGui>
#include <QSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include "tt_old_tag/tt_old_hex_viewer_export_selected_bytes.h"
#include "message_box/message_dialog.h"
#include "file_viewer/plist_viewer/plist_viewer.h"
#include "file_viewer/sqlite_viewer/sqlite_viewer_starter.h"
#include "header_global_variables_1.h"
#include <QHeaderView>
#include "header_global_variables_2.h"

#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "global_connection_manager.h"
#include "recon_generic_functions/recon_helper_standard.h"      

#include <QMessageBox>
#include <tsk/libtsk.h>

struct struct_global_tsk_part_info_fuse
{
    bool bool_partition_found;
    TSK_VS_PART_INFO *part_info_ptr;
    bool bool_fs_found;
    TSK_FS_INFO *fs_info_ptr;
    qint64 size;
    QString fs_name;
    QList<qint64> list_unalloc_block;
    qint64 current_sector;
};

namespace Ui {
class hex_viewer;
}

class hex_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit hex_viewer(QWidget *parent = 0);
    ~hex_viewer();

    void setdata(const QByteArray &data);
    void set_file_data(QString path);
    void pub_set_hex_viewer_name(QString name);
    void pub_set_essentials();
    void pub_set_hex_path(QString path);
    void pub_start_hex_extraction();
    void set_viewer_diaplay_name(QString display_nm);
    QString set_file_path_from_db(QString db_path);
    void pub_display_hex_viewer_metadata();
    void pub_set_outsqlite_path(QString name);
    void pub_display_hex_viewer_full();
    void create_ui_full();
    void pub_set_comboBox_block_list();
    void set_file_path_and_db_path(QString file_path_str, QString db_path);
    void pub_set_source_count_name(QString name);

    void pub_set_called_from_detailed_info();
    void pub_highlight_keyword(QString keyword_str);
    void pub_set_plugin_name(QString plugn_nm);
    void pub_set_tab_name(QString tab_nm);
    void pub_set_record_no(QString recrd_no);
    void pub_set_called_from_metadata_display();
    void pub_set_global_connection_manager(global_connection_manager *obj);
    void pub_set_recon_file_info(struct_global_recon_file_info obj);

    void pub_set_disk_viewer_essentials(bool is_disk_viewer, bool is_complete_disk, bool bool_is_unallocated_disk, QString unalloc_tbl_name);
    void pub_set_file_data_for_complete_disk_viewer(QString disk_node);
public slots:
    void slot_set_block_db_path(QString db_path);

signals:
    void search_hex(QByteArray data, uint64_t page_size,bool previous_clicked, bool is_fresh_search);
    void search_ascii(QString text, uint64_t page_size, bool previous_clicked, bool is_fresh_search);
    void go_to_offset(uint64_t offset, uint64_t page_size);
    void search_next(int search_index,int seached_data_size);
    void search_previous(int search_index, int searched_data_size);
    void set_searched_data_clear(int);
    void signal_tag_start_index_clicked(bool start_clicked);
    void signal_change_address_area_value(uint64_t address_value);
    void clear_block_index_list();
    void signal_current_page_details(int current_page_count, uint64_t current_offset, uint64_t page_size);
    void signal_set_blocks_list_for_this_file(QList<struct_global_hex_viewer_start_and_end_index> list_selected_block_details);
    void signal_set_context_menu(bool bool_is_full_or_meta_ui);
    void signal_add_selected_bytes_to_block_clicked(QString tag_name, QString color_hex);
    void signal_open_file_in_other_recon_viewers_from_hex_viewer(QString result_name,QString viewer_name, QString source_count_name);
    void signal_set_tag_start_offset(uint64_t offset);
    void signal_insert_tag_detail_into_tag_search_db(QString file_path, QString tag_name, uint64_t start_index, uint64_t end_index, QString record_no, QString hex_viewer_display_name, QString source_count_name, QString selected_data_str);
    void signal_remove_tag_entry_from_tag_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList removed_tags_list, QString os_scheme);
    void signal_remove_tag_entry_from_notes_db_and_display(QString plugin_name, QString tab_name, QString record_no, QString notes_val, QString os_scheme);
    void signal_save_openwith_changes_to_bucket(QString viewer_type);
    void signal_highlight_ascii_searched_text(QString text, uint64_t page_size);
    void signal_highlight_hex_searched_data(QByteArray data, uint64_t page_size);

    void signal_set_informative_status(bool status);

private slots:
    void on_pushButton_search_clicked();    
    void slot_cur_hex(QByteArray data_64_bit_arr);
    void on_pushButton_next_clicked();
    void on_pushButton_prev_clicked();
    void on_checkBox_big_endian_clicked(bool checked);
    void on_checkBox_unsigned_clicked(bool checked);
    void on_pushButton_goto_offset_clicked();
    void on_pushButton_find_next_clicked();
    void on_pushButton_find_previous_clicked();
    void slot_index_list_size(int indexsize);
    void current_offset_cursor(int offset);
    void on_lineEdit_search_returnPressed();
    void selct_bytes_offsets(int select_start, int select_end, bool selection);
    void on_pushButton_selection_clicked(bool checked);
    void on_pushButton_offset_clicked(bool checked);
    void slot_hex_viewer_tag_start_offset_clicked(int index_value);
    void slot_export_clicked(QString output_path);
    void on_spinBox_page_size_valueChanged(const QString &arg1);
    void on_pushButton_go_to_page_clicked();
    void slot_combobox_export_block_index_change(QString text);
    void slot_export_selected_bytes(QByteArray selected_bytes);
    void slot_export_bytes_save_clicked(QString file_name, QString file_path);
    void slot_pushbutton_export_clicked();
    void slot_pushbutton_remove_block_clicked();
    void slot_line_edit_go_to_page_text_changed(QString text);
    void slot_open_in_other_recon_viewers_clicked();
    void slot_insert_tag_selected_bytes_info_in_db(int start_index, int end_index, QString tag_name, QString color_name);
    void slot_hex_viewer_tag_end_offset_clicked(QString tag_name, QString color_name, uint64_t start_index, uint64_t end_index);

    void slot_page_end_reached_for_search_highlight(bool status);
    void slot_cursor_current_pos_for_previous_click(qint64 pos);
    void on_lineEdit_search_text_change(QString text);
    void slot_hide_loading_display_dialog_box(bool cancel_status);
private:
    Ui::hex_viewer *ui;

    enum enum_tablewidget_conversion_row_index
    {
        enum_binary,
        enum_8_bit,
        enum_16_bit,
        enum_32_bit,
        enum_mac_timestamp,
        enum_unix_timestamp
    };

    enum enum_search_clicked
    {
        enum_search_pushbutton_search = 0,
        enum_search_pushbutton_previous,
        enum_search_pushbutton_next

    };

    struct struct_search_page_info
    {
        bool bool_text_match_found;
        qint64 matched_page_count;
    };

    struct struct_disk_hex_view_data
    {
        TSK_FS_INFO *fs_info_ptr = NULL;

        qint64 current_page_index = 0;
        qint64 block_start_index = 0;
        qint64 last_processed_total_blocks = 0;
        QByteArray current_page_data;

        bool bool_last_click_was_next = false;
    };
    QList<struct_disk_hex_view_data>list_disk_hex_view_data;

    bool bool_is_called_from_detailed_info = false;
    bool bool_is_called_from_metadata_display = false;

    void extract_block_name_list();

    // GUI
    QGroupBox *groupBox_main, *groupbox_1_go_offset_to_page, *groupBox_2_search, *groupBox_3_conversion, *groupBox_5_current_selection, *groupBox_6_page_size, *groupBox_go_to_offset;
    QGroupBox *groupBox_blocks_list, *groupBox_file_metadata, *groupBox_open_in_other_recon_viewers;
    QFrame *frame_hex;
    QLabel *label_no_of_pages, *label_binary, *label_8_bit, *label_16_bit, *label_32_bit, *label_search_results, *label_mac_timestamp, *label_unix_timestamp;
    QLabel *label_selection_info_offset, *label_selection_info_byte_count, *label_page_size, *label_file_name,*label_file_path, *label_file_size;
    QLabel *label_block_start_end_index,*label_block_info;
    QPushButton *pushButton_next_goto_page, *pushButton_prev_goto_page, *pushButton_search, *pushButton_search_next, *pushButton_search_previous;
    QPushButton *pushButton_goto_offset, *pushButton_selection_info_offset, *pushButton_selection_info_byte_count, *pushbutton_go_to_page,*pushbutton_export_block;
    QPushButton *pushButton_remove_block,*pushbutton_open_in_other_recon_viewers;
    QLineEdit *lineEdit_go_to_offset, *lineEdit_search, *lineEdit_binary, *lineEdit_8_bit, *lineEdit_16_bit, *lineEdit_32_bit, *lineedit_goto_page;
    QLineEdit *lineEdit_mac_timestamp, *lineEdit_unix_timestamp;
    QComboBox *comboBox_search_type, *comboBox_go_to_offset_type,*comboBox_blocks_list;
    QCheckBox *checkBox_big_endian,*checkBox_unsigned;
    QSpinBox *spinBox_page_size;
    m_tablewidget *m_tablewidget_conversion_obj;

    QHexEdit *hex_edit_obj = NULL;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    QByteArray data_arr;

    QByteArray _8_bit_array;
    QByteArray _16_bit_array, _16_bit_converted_array;
    QByteArray _32_bit_array, _32_bit_converted_array;

    int index_list_size = 0;
    int current_page_count = 0;
    qint64 tmp_no_of_pages;

    bool bool_at_page_end_for_search_highlight;
    qint64 current_cursor_pos_for_prev_clicked;

    uint64_t current_offset = 0;
    uint64_t previous_entered_size = 0;

    void show_binary_value(const QByteArray &data_arr);
    void show_8_bit_signed_value(const QByteArray &data_arr, bool checked);
    void show_16_bit_signed_value(const QByteArray &data_arr, bool checked);
    void show_32_bit_signed_value(const QByteArray &data_arr, bool checked);

    int endian_checker();
    QByteArray endian_converter(QByteArray data);

    bool selection_button_checked;
    bool offset_button_checked;

    qint64 selected_data_size = 0;

    QString source_count_name;

    qint64 total_file_size = 0;
    QString file_path;

    export_tag_module *export_tag_module_obj;

    QString block_db_path;

    QStringList tag_name_with_start_end_index_list;

    QString recon_result_dir_path;
    QString destination_hex_path;
    QString destination_hexview_index_db_path;
    QString hex_view_db_table_name;
    QString hex_viewer_display_name;


    QString hex_file_name;
    QString hex_plugin_name;
    QString hex_plugin_category_name;
    QString hex_plugin_record_no;

    bool bool_cancel_extraction;
    void entry_in_index_table();

    void viewer_connections();
    bool is_this_decimal(QString value_str);
    bool is_this_val_hex(QString value_str);

    void set_ui_go_to_page_next_and_pre();
    void set_ui_for_search_section();
    void set_ui_for_conversion_section_without_tablewidget();
    void set_ui_for_current_offset_and_seleted_bytes();
    void set_ui_for_go_to_offset();
    void set_ui_for_export_blocks_section();
    void set_ui_for_page_size_selection();
    void set_ui_for_frame_hex();
    void set_converstion_to_default_attributes();
    void set_ui_go_to_page_next_and_pre_for_metadata();

    hex_viewer_export_selected_bytes *export_selected_bytes_obj;
    QByteArray export_selected_bytes_arr;
    void fill_the_existing_blocks_details_in_list();
    QString first_label_display_string;
    QStringList combobox_display_list;
    qint64 total_no_of_pages;

    void set_palette_lineedit_goto_page(int);

    bool is_full_or_meta;

    void set_widget_fonts();

    void reset_all_the_widgets_data();
    void show_mac_timestamp(QByteArray data_arr);

    void show_unix_timestamp(QByteArray data_arr);


    message_dialog *message_dialog_obj;
    void set_ui_for_file_metadata_display();
    void set_ui_for_open_in_other_recon_viewers();
    QString get_tag_color_hex_from_tag_db(QString tag_name, QString color_name);

    QString selected_block_name;
    uint64_t tag_start_index_in_file;
    uint64_t tag_end_index_in_file;
    QStringList matched_pages_list;

    bool bool_show_informative_dialogs;

    QString get_tag_name_with_start_and_end_offset_string(QString tag_name, QString start_index_str, QString end_index_str);
    QStringList get_already_applied_tags_list_for_display_in_combobox();
    void export_bytes_into_tag_temporatrily_and_insert_details_into_tag_search_db(QString tag_name, QString color_hex, uint64_t start_index, uint64_t end_index);
    QString insert_exported_content_info_in_db(QString tag_name, QString color_hex, uint64_t start_index, uint64_t end_index, QString export_file_path);
    void remove_this_tag_entry_in_other_db_and_display(QString tag_name, QString start_index_str, QString end_index_str, QString file_path_str);

    void set_ui_for_conversion_section_with_tablewidget();
    struct_search_page_info get_available_page_info_for_search(enum_search_clicked click_type, QString searched_type, QString searched_text, uint64_t page_size);

    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    void loading_display_dialog_based_on_records(int total_records, int processed_count);

    void common_go_to_page(qint64 enter_page_index);

    QString plugin_name_str;
    QString tab_name_str;
    QString record_no;

    global_connection_manager *global_connection_manager_obj;
    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

    bool bool_is_from_disk_viewer          = false;
    bool bool_disk_viewer_complete_disk    = false;
    bool bool_disk_viewer_unallocated_disk = false;

    bool bool_disk_hex_data_processed = false;
    bool bool_disk_hex_view_first_load = false;
    QByteArray disk_data_arr;

    TSK_IMG_INFO *img_info_ptr = NULL;
    TSK_VS_INFO  *vs_info_ptr  = NULL;
    TSK_FS_INFO  *fs_info_ptr  = NULL;

    int tsk_start_image_processing(QString image_path_str);
    int tsk_process_volume_filesystem(TSK_IMG_INFO *img_info, TSK_OFF_T start);
    int get_blocks_for_complete_disk_hex_view(TSK_FS_INFO *fs_info_ptr);
    int get_next_block_for_complete_disk_hex_view(TSK_FS_INFO *fs_info_ptr);
    int get_previous_block_for_complete_disk_hex_view(TSK_FS_INFO *fs_info_ptr);
    void set_file_data_for_complete_hex_view();
    void common_go_to_page_for_disk_hex_viewer(qint64 enter_page_index);
    void go_to_offset_for_file(qint64 offset);
    void go_to_offset_for_disk_hex_viewer(qint64 offset);
    void search_for_file(QString search_type, QString searched_text);
    void search_for_disk_hex_view_complete(QString search_type, QString searched_text);
    struct_search_page_info get_available_page_info_for_diskview_complete_search(enum_search_clicked click_type, QString searched_type, QString searched_text);

    void prepare_disk_hex_view_for_unallocated();
    int get_next_block_for_unallocated_disk_hex_view();
    void set_file_data_for_unallocated_disk_hex_view();
    int get_previous_block_for_unallocated_disk_hex_view();
    QByteArray get_data_for_specified_block(TSK_FS_INFO *fs_info_ptr, qint64 blk_count);
    QByteArray get_data_for_specified_page(qint64 page_num);
    void go_to_page_for_disk_hex_view_unallocated(qint64 page_num);
    struct_search_page_info get_available_page_info_for_diskview_unallocated_search(enum_search_clicked click_type, QString searched_type, QString searched_text);
    QString disk_hex_view_unalloc_tbl_str;
    QString disk_hex_view_db_path;
    qint64 last_processed_total_blocks_unalloc = 0;
    int tsk_default_block_size = 0;
    qint64 disk_hex_viewer_total_pages = 0;

};

#endif // HEX_VIEWER_H
