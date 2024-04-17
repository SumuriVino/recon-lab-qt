#ifndef STORY_BOARD_LOADER_H
#define STORY_BOARD_LOADER_H

#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "header_identical_2.h"
#include "header_identical_3.h"
#include "story_board_textedit.h"

class story_board_loader : public table_result_loader
{
    Q_OBJECT
public:
    story_board_loader(QWidget *parent);
    ~story_board_loader();

    void pub_set_story_board_loader_essential();

    void populate_data_in_table();
    void pub_tag_search_set_display_db_path(QString db_path);
    void pub_set_current_tag_name(QString tag_name);
    void pub_set_examiner_notes_file_path(QString file_path);
    void pub_create_ui();

    void pub_story_board_search_clicked(QString search_text);
    void pub_story_board_show_all_clicked();
    void pub_set_clear_table_display(bool status);
    void pub_set_bool_for_right_click_actions(bool status);

    void pub_set_story_file_name_and_path(QString file_name, QString file_path);

    void populate_data_in_timeline_display();
    void pub_set_feature_icon_path(QString icon_str);

    void pub_set_tag_icon_path(QString icon_str);
    void pub_set_storyboard_textedit_class_obj(story_board_textedit *obj);

private slots:
    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_tableWidget_doubleClicked(int row, int column);
    void slot_storyboard_tabbar_clicked(int index);
    void slot_tags_combobox_index_changed();
    void slot_act_quick_look_triggered();

    void slot_hide_loading_display_dialog_box(bool cancel_status);

    void slot_timeline_tablewidget_selection_changed();
    void slot_checkbox_blurr_status(bool status);
    void slot_sort_tablewidget_display(int column_index);
    void slot_checkBox_censor_skintone_tag_images_clicked(bool status);

    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);
    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();

signals:
    void signal_add_record_to_story_board_by_loader(QString metadata, QString media_path);
    void signal_add_file_to_story_board_by_loader(QString actual_filename, QString filepath, bool blurr_status, QString label_file_path);
    void signal_show_all_clicked();
    void signal_on_cancel_pushbutton();

    void signal_save_data();

private:

    QString current_tag_name;
    QString searched_text;
    QString destination_db_path_for_tag_search;
    int screenshot_date_creation_index;

    bool bool_clear_table_display;
    bool bool_right_click_actions_added;

    QString add_record_with_files;
    QString add_file_to_story;
    QString add_record;


    enum enum_timeline_column_index
    {
        enum_timeline_timestamp_column = 0,
        enum_timeline_type_column,
        enum_timeline_record_no_column,
        enum_timeline_plugin_column,
        enum_timeline_category_column,
        enum_timeline_item_1_column,
        enum_timeline_item_2_column,
        enum_timeline_source_count_name_column
    };
    enum enum_av_preview
    {
        enum_audio_video_preview_index = 0,
        enum_image_preview_index,
        enum_no_preview_index,
    };


    QTabWidget *tabwidget_storyboard_obj;
    m_tablewidget *m_tablewidget_timeline_obj;

    void contextMenuEvent(QContextMenuEvent *evt);
    void action_add_record_to_story_triggered();
    void action_add_file_to_story_triggered();
    void action_add_record_to_story_with_files_triggered();

    QString create_html_table_start();
    QString create_html_table_header(QStringList header_list);
    QString create_html_table_rows_data_for_general_files(QStringList column_data_list, QString base_64_data);
    QString create_html_table_end();
    void tablewidget_general_clicked(int row, int column);
    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);


    int tag_icon_index;

    int tag_name_index;
    int saved_map_file_path_index;
    int item_0_index;
    int item_1_index;
    int item_2_index;
    int item_3_index;
    int item_4_index;
    int item_5_index;

    int item_6_index;
    int item_7_index;
    int item_8_index;
    int item_9_index;
    int item_10_index;


    int item_11_index;
    int item_12_index;
    int item_13_index;
    int item_14_index;
    int item_15_index;

    int item_16_index;
    int item_17_index;
    int item_18_index;
    int item_19_index;

    bool bool_checkbox_blurr_status;

    av_preview *av_preview_obj;
    image_preview *image_preview_obj;
    blank *blank_obj;
    QStackedWidget *stacked_widget_obj;
    QHBoxLayout *hbox_table_and_preview;

    QString story_file_name;
    QString story_file_path;

    struct_global_recon_file_info get_filepath_for_av();
    void action_goto_record_triggered();
    QString create_html_table_startfor_record();
    QByteArray get_image_hex_data(QString filepath);
    QString create_html_table_rows_data_for_our_created_files(QString metadata_str, QString map_filepath, QString filepath, QString plugin_name_str);
    QByteArray get_image_hex_data_for_maps(QString filepath);

    QString current_icon_path_str;
    QString tag_icon_str;
    void display_data(QString dest_db_path);
    QString ios_dir_adhoc_path;
    story_board_textedit *story_board_textedit_obj;

    QString create_html_table_rows_data_for_for_multiple_files(QStringList column_data_list, QStringList display_file_path_list);

    QString examiner_notes_source_path;
    void display_data_from_examiner_notes_file(QString file_path);
    QString get_examiner_notes_data_for_table_view(QString file_path);
    bool bool_checkbox_censor_skintone_tag_image_status;

    struct struct_search_column_text_for_story_board
    {
        int column_number;
        QString search_text;
    };

    QTableWidget *current_table_widget;
    QList<custom_table_header_widget *> custom_header_widgets_timeline_QList;

    void all_column_text_search(QList<struct_search_column_text_for_story_board> search_text_all_column_list);

};

#endif // STORY_BOARD_LOADER_H
