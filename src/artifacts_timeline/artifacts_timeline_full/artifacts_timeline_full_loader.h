#ifndef ARTIFACTS_TIMELINE_FULL_LOADER_H
#define ARTIFACTS_TIMELINE_FULL_LOADER_H

#include <QObject>
#include <QSlider>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"
#include "message_box/message_dialog.h"
#include "slider/qSlice.h"
#include <QComboBox>

#include "ui_custom_classes/m_barset.h"
#include <QtCharts>
#include "save_result_dialog_box/save_result_dialog_box.h"

#include "export_dialog_box/export_dialog_box_2/export_dialog_box_2.h"

#include "saved_graphs_dialog_box/saved_graphs_dialog_box.h"


class artifacts_timeline_full_loader : public table_result_loader
{
    Q_OBJECT
public:
    explicit artifacts_timeline_full_loader(QWidget *parent);
    ~artifacts_timeline_full_loader();

    void pub_set_display_db_path(QString db_path);
    void populate_data_in_table();
    void configure_ui_for_timeline();
    void pub_initialize_and_display_plugins_and_timestamps();
    void pub_set_saved_events_db_path(QString db_path);
    void pub_create_ui();

signals:
    void signal_save_tabs_from_artifacts_timeline(QString db_path);
    void signal_update_bookmarks_status_in_tabs_generated_by_timeline_graph(QString record_no ,QString plugin_name_str ,QString tab_name_str,QString status,QString tag_name ,QString os_name_str);
    void signal_update_tags_status_in_tab_generated_by_timeline_graph(QString record_no ,QString plugin_name_str ,QString tab_name_str,QString status,QString tag_name ,QString os_name_str);
    void signal_update_notes_status_in_tab_generated_by_timeline_graph(QString record_no ,QString plugin_name_str ,QString tab_name_str,QString text,QString tag_name ,QString os_name_str);
    void signal_add_saved_timeline_graph_in_case_tree();
    void signal_display_after_save_results(QString feature_name , QString saved_result_name);

private slots:

    void slot_pushbutton_apply_clicked(bool is_clicked);
    void slot_m_treeWidget_plugins_itemClicked(QTreeWidgetItem *item, int column);
    void slot_hovered_toolTip_on_barsets(bool status, int index, QBarSet *barset);
    void slot_pushbutton_previous_clicked(bool is_clicked);
    void slot_pushbutton_next_clicked(bool is_clicked);
    void slot_radioButton_yearly_clicked(bool checked);
    void slot_radioButton_monthly_clicked(bool checked);
    void slot_radioButton_daywise_clicked(bool checked);
    void slot_radioButton_hourly_clicked(bool checked);
    void slot_tabbar_clicked(int index);
    void slot_pushbutton_go_to_page_clicked();

    void slot_line_edit_go_to_page_text_changed(QString text);
    void slot_lineEdit_plugin_search_textChanged(const QString &arg1);
    void slot_lineEdit_timestamp_search_textChanged(const QString &arg1);
    void slot_checkBox_plugin_clicked(bool value);
    void slot_timestamps_horizontal_header_clicked(int column);
    void slot_barset_doubleClicked(int index);
    void slot_act_show_data_of_current_plugin_triggered();
    void slot_pushbutton_save_clicked(bool value);
    void slot_pushbutton_close_tab_clicked(bool value);
    void slot_act_show_all_data_triggered();
    void slot_barset_pressed(int index);
    void slot_act_remove_tags();
    void slot_add_notes_pushbutton_clicked(QString text);


    void slot_save_result_name_value(QString save_name);
    void slot_pushbutton_export_graph_clicked();
    void slot_pushbutton_open_in_tableview_clicked();

    void slot_fs_sources_horizontal_header_clicked(int column);
    void slot_refresh_table_data_on_show_all();

    void slot_pushbutton_export_table_data_clicked(bool value);
    void slot_table_data_export_dialog_ok_clicked(QString given_name, QString selected_dir_path, QString export_type_str, QString export_scope);

    void slot_sources_tabbar_clicked(int index);
    void slot_save_graph_name_value(QString graph_name);
    void slot_pushbutton_save_graph_clicked();
    void slot_hide_loading_display_dialog_box(bool cancel_status);

    void slot_goto_artifact_source_submenu_clicked(QAction *clicked_action);
    void slot_timeline_export_as_clicked(QAction *current_clicked_action);

    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

private:

    struct struct_graphs_essentials
    {
        QString int_value;
        QString timestamp;
        QString year_value;
        QString month_year_value;
        QString day_month_year_value;
        QString hour_day_month_year_value;

        QString plugin_name;
        QString tab_name;

    };

    enum enum_artifacts_timeline_tabs
    {
        enum_artifacts_timeline_tab_Events = 0,
        enum_artifacts_timeline_tab_stacked_graph
    };

    enum enum_artifacts_timeline_graphs_type
    {
        enum_artifacts_timeline_graphs_yearly = 0,
        enum_artifacts_timeline_graphs_monthly,
        enum_artifacts_timeline_graphs_daywise,
        enum_artifacts_timeline_graphs_hourly
    };

    enum enum_artifacts_timeline_tablewidget_timestamps_list
    {
        enum_artifacts_timeline_tablewidget_timestamps_list_stamp_type = 0,
        enum_artifacts_timeline_tablewidget_timestamps_list_stamp_name
    };


    enum enum_artifacts_timeline_treewidget_column_name{
        enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name = 0,
    };

    struct struct_chart_detail
    {
        QStringList category_list;
        QString chart_title;
        QString x_axis_title;
        QString y_axis_title;
        int series_type;///1 for barseries or 2 for stackedbarseries
        QBarSeries *bar_series;
        QStackedBarSeries *stacked_bar_series;
        qint64 roundup_value_yaxis;
    };


    QMultiMap<QString,struct_graphs_essentials> map_of_struct_graph_data;
    QList<QChartView*> list_of_charview;
    QMap<int, QList<QChartView*>> map_of_chartview;

    QMap<int, QString> map_of_pageno_and_timestamplabel;
    QMap<int, QMap<int, QString>> map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel;


    int tab_saved_index;


    QString destination_timeline_db_path;
    QString destination_timeline_saved_db_path;

    QLabel *label_filter_link;
    QTableWidget *tablewidget_timestamp_list;

    QPushButton *pushButton_save;
    QPushButton *pushButton_close_tab;

    QPushButton *pushButton_export_table_data;


    QDialog *dialog_window_save_timeline_data;
    QPushButton *pushbutton_done_save_tab;
    QLineEdit *lineEdit_save_tab;

    QLabel *label_tab_details;
    QStringList list_tab_details;


    QPushButton *pushButton_move_first_slider_left;
    QPushButton *pushButton_move_first_slider_right;
    QPushButton *pushButton_move_second_slider_left;
    QPushButton *pushButton_move_second_slider_right;

    m_treewidget *treewidget_plugins_list;
    QTabWidget *tabwidget_timeline_filter_obj;
    QTabWidget *tabwidget_timeline_data_obj;
    QTabWidget *tabwidget_timeline_artifacts_obj;

    QMap <QString , QStringList> map_plugin_with_tab_list;
    QMap <QString , QStringList> map_bookmarked_plugin_with_tab_list;



    QLabel *label_artifacts_start_date;
    QLabel *label_artifact_end_date;
    QDateTimeEdit *datetimeedit_start;
    QDateTimeEdit *datetimeedit_end;
    QPushButton *pushbutton_apply;

    QString bookmarked_plugin_name;
    QStringList bookmarked_plugin_tab_list;

    qint64 start_timestamp_utc_qint64;
    qint64 end_timestamp_utc_qint64;

    QGroupBox *groupbox_event_and_timestamp;

    QChartView *chartView_common_obj;

    QBarSeries *barseries_obj;
    QStackedBarSeries *stacked_barseries_obj;

    QRadioButton *radiobutton_create_graph_yearly;
    QRadioButton *radiobutton_create_graph_monthly;
    QRadioButton *radiobutton_create_graph_daywise;
    QRadioButton *radiobutton_create_graph_hourly;

    QStringList years_list;
    QStringList day_month_year_list;
    QStringList hour_day_month_year_list;
    QStringList month_year_list;
    QStringList timestamp_list;

    QStringList categories_list;

    QString xaxis_title_str, yaxis_title_str;
    QStackedWidget *stackwidget_obj;

    QPushButton *pushbutton_previous;
    QPushButton *pushbutton_next;

    QMap<int, QString> map_insert_index_with_category;
    QLabel *label_show_date_time;
    QLabel *label_show_pages;
    QLabel *label_duration;
    QLabel *label_goto_page;
    QLineEdit *linedit_go_to_page;
    QPushButton *pushbutton_go_to_page;

    QPushButton *pushbutton_export_graph;


    QPushButton *pushbutton_save_graph;
    saved_graphs_dialog_box *save_graph_dialog_box_obj ;

    QStringList bookmarked_timestamp_type_list;
    QStringList bookmarked_timestamp_name_list;

    int stackwidget_index;
    message_dialog *message_box_obj;

    QCheckBox *checkbox_plugins;

    QLineEdit *lineEdit_search_plugins;
    QLineEdit *lineEdit_search_timestamps;

    QMenu *menu_graphs;
    QAction *act_show_data_of_current_plugin_barset;
    QAction *act_show_all_data;

    bool bool_check_barset;

    QString current_barset_time_str;
    QString current_barset_plugin;
    save_result_dialog_box *save_result_dialog_box_obj;

    int bookmark_column_index;
    int tag_column_index;
    int notes_column_index;

    export_dialog_box_2 *export_dialog_box_2_obj;
    QString report_scope_str;
    int timestamp_index;
    int item_1_index;
    int item_2_index;


    void display_plugins_in_treewidget();
    void display_data(QString dest_db_path);
    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void set_timeline();
    void create_graph_yearly();

    void create_graphs();
    void set_chart_in_stackwidget_after_setting_of_all_specification(artifacts_timeline_full_loader::struct_chart_detail &struct_chart_detail_obj);
    void create_graph_daywise();

    void prepare_timeline_groupbox();
    void create_graph_hourly();


    void create_graph_monthly();
    void set_essentail();
    void set_bookmarked_timestamp_type_list();


    int populate_data_of_graph_in_tablewidget(QSqlQuery &query_index);
    void display_data_of_graph_tablewidget(QString dest_db_path);
    int set_graphs_data_tables_header();

    void update_bookmark_status_for_all(QString status);
    void update_tags_value(QString tag_data, QString colour);
    void update_notes_value(QString text, int current_row);

    void tablewidget_general_clicked(int row, int column);
    void contextMenuEvent(QContextMenuEvent *evt);
    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_export_triggered();
    void display_timestamps_list();

    void action_sendToBucket_plist_viewer_triggered();
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void action_open_detach_detailed_info_triggered();
    void action_open_full_detailed_info_triggered();
    void action_remove_note_triggered();
    void action_remove_note_bookmark_triggered();

    QString saved_db_path; ///need  confirmation for macro
    bool is_clicked_to_save_all_data;
    QString create_db_for_saved_artifacts(QString db_name);
    QString prepare_command(QStringList tab_list);
    void insert_values_in_save_result_db(QSqlQuery &index_query, QString result_name);
    void extract_and_save_result(QString plugin_str, QStringList tabs_list, QString result_name);

    void saved_db_info(QString db_name);

    QString prepare_and_get_tooltip_string_for_barset(qint64 count, QString category, QString label);

    void set_graphs_essentials_for_plugins_data_only();
    void save_timeline_data_in_case_treewidget(QString tab_to_be_saved);

    QPushButton  *pushbutton_open_in_tableview;
    m_tablewidget *tablewidget_filesystem_sources;

    QString get_command_for_open_in_tableview();

    void disable_enable_initial_ui(bool status);
    void insert_data_in_table_on_show_in_tableview_clicked();
    QStringList sort_month_list(QStringList month_list);
    QStringList get_sorted_months_list(QStringList month_list);
    void set_graphs_essentials_for_filesystem_data_only(QStringList fs_db_list);

    void create_rightclick_options();
    void update_tags_in_tag_serach_db(QString current_tag, QString dest_db_path);
    void delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag, QString record_no, QString plugin_name, QString tab_name, QString stamp_type_str, QString source_count_name);
    void update_note_in_notes_serach(QString text_str, int selected_row, QString dest_db_path);
    int index_for_INT_only_for_artifact_timeline;

    int stamp_type_index;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading;

    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    void loading_display_dialog_based_on_records(int total_records, int processed_count);

    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    void update_bookmark_value(QString status, int selected_row_no, int column);
    void open_exported_file(QString file_path);

    QFileDialog file_dialog_obj;
    void action_go_to_record_triggered();
    void create_action_for_goto_artifact_source_submenu();

    QMenu *menu_export_as;
    void export_timeline_data_as_csv(QString filepath);
    void export_timeline_data_as_sqlite(QString filepath);

    struct struct_artifacts_timeline_table_view_info
    {
        QString tab_str;
        QString timestamp_str;
        QString graph_type;
        bool is_open_in_tableview_clicked;
    };
    QList<struct_artifacts_timeline_table_view_info> struct_artifacts_timeline_table_view_info_QList;
    int index_struct_artifacts_timeline_table_view_info = -1;

    struct struct_search_column_text_for_artifact_timeline
    {
        int column_number;
        QString search_text;
    };
    QList<custom_table_header_widget *> custom_header_widgets_QList;

    custom_table_header_widget *custom_table_header_widget_obj;
    QMap<QString, QList<custom_table_header_widget *>> tab_custom_header_widgets_QMap; ///-----key = feature_name-----value = custom_table_header_widget pointer list-----///

    void all_column_text_search(QList<struct_search_column_text_for_artifact_timeline> search_text_all_column_list);
    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;
    QString new_tab_name;

};

#endif // ARTIFACTS_TIMELINE_FULL_LOADER_H
