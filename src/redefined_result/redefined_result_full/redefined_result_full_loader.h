#ifndef REDEFINED_RESULT_FULL_LOADER_H
#define REDEFINED_RESULT_FULL_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_csv_template.h"
#include <QtCharts>
#include "timeline_selection_window/timeline_selection_window.h"

#include "header_plugin_extraction_interface.h"
#include "ui_custom_classes/m_barset.h"
#include "save_result_dialog_box/save_result_dialog_box.h"
#include "saved_graphs_dialog_box/saved_graphs_dialog_box.h"
#include "export_dialog_box/export_dialog_box_1/export_dialog_box_1.h"

class redefined_result_full_loader : public table_result_loader
{
    Q_OBJECT
public:
    explicit redefined_result_full_loader(QWidget *parent = 0);
    ~redefined_result_full_loader();

    void pub_set_destination_db_path(QString db_path);
    void redefined_result_populate_data_in_table();
    void pub_create_ui();
    void pub_set_graph_on_display();
signals:
    void signal_show_hide_metadata(bool value);
    void signal_save_tabs_from_redefined_result(QString db_path , QString plugin_name);
    void signal_update_bookmarks_status_in_tabs_generated_by_redefined_result_graph(QString record_no ,QString plugin_name_str ,QString tab_name_str,QString status,QString tag_name ,QString os_name_str);
    void signal_update_tags_status_in_tab_generated_by_redefined_result_graph(QString record_no ,QString plugin_name_str ,QString tab_name_str,QString status,QString tag_name ,QString os_name_str);
    void signal_update_notes_status_in_tab_generated_by_redefined_result_graph(QString record_no ,QString plugin_name_str ,QString tab_name_str,QString text,QString tag_name ,QString os_name_str);
    void signal_saved_graph_category_name(QString category_name);
    void signal_display_data_for_redefined_results(QString feature_name,QString tab_text , QString db_path , QString plugin_name);

private slots:

    void slot_add_notes_pushbutton_clicked(QString text);
    void slot_goto_artifact_source_submenu_clicked(QAction *clicked_action);
    void slot_hashset_storage_workdone_clicked(QStringList list_dbs_addmenu, QString callfrom_str);
    void slot_add_files_to_hashset_submenu_clicked(QAction *current_clicked_action);

    void slot_time_line(qint64 start_time, qint64 end_time);
    void slot_pushButton_timeline_clicked();
    void slot_tabbar_clicked(int index);
    void slot_hovered_toolTip_on_barsets(bool status, int index, QBarSet *barset);
    void slot_link_label_accounts_list(QString text);
    void slot_link_label_participants_list(QString text);
    void slot_tablewidget_participants_list_cell_single_clicked(int row, int coloum);
    void slot_tablewidget_accounts_configures_item_clicked(int row, int coloum);
    void slot_hovered_toolTip_on_pieslices(QPieSlice *slice, bool status);

    void slot_pushbutton_previous_clicked(bool is_clicked);
    void slot_pushbutton_next_clicked(bool is_clicked);
    void slot_radioButton_yearly_clicked(bool checked);
    void slot_radioButton_monthly_clicked(bool checked);
    void slot_radioButton_daywise_clicked(bool checked);
    void slot_radioButton_hourly_clicked(bool checked);
    void slot_pushbutton_go_to_page_clicked();

    void slot_line_edit_go_to_page_text_changed(QString text);
    void slot_barset_doubleClicked(int index);
    void slot_act_show_data_of_current_barset_triggered();
    void slot_pushbutton_save_clicked(bool value);
    void slot_pushbutton_close_tab_clicked(bool value);
    void slot_act_show_all_data_triggered();
    void slot_barset_pressed(int index);
    void slot_pie_slice_doubleClicked();
    void slot_pie_slice_pressed();

    void slot_act_remove_tags();

    void slot_save_result_name_value(QString save_name);
    void slot_pushbutton_export_graph_clicked();
    void slot_pushButton_topurl_export_clicked();
    void slot_save_graph_name_value(QString graph_name);
    void slot_pushButton_save_url_pie_clicked();
    void slot_pushButton_save_graph_clicked();
    void slot_export_graph_by_saved_name (QString given_name , QString selected_dir_path, QString exp_type_str);
    void slot_export_graphs_cancel_results();

    void slot_hide_loading_display_dialog_box(bool cancel_status);
    void slot_main_table_widget_item_clicked(QTableWidgetItem *item);
    void slot_tablewidget_goto_record(QString record);

    void slot_pushbutton_search_clicked();

    void slot_checkbox_timeline_toggled(bool checked);

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
        QString sender;
        QString receiver;

    };

    enum enum_redefined_result_tabs
    {
        enum_redefined_result_tab_Events = 0,
        enum_redefined_result_tab_stacked_graph,
        enum_redefined_result_tab_pie_or_visit_count_graph
    };

    enum enum_redefined_result_graphs_type
    {
        enum_redefined_result_graphs_yearly = 0,
        enum_redefined_result_graphs_monthly,
        enum_redefined_result_graphs_daywise,
        enum_redefined_result_graphs_hourly
    };

    struct struct_redefined_result_single_chart_detail
    {
        QStringList category_list;
        QStackedBarSeries *stacked_bar_series;
        qint64 roundup_value_axis;
    };

    QString destination_db_redefined_result;
    QPushButton *pushbutton_timeline;

    QCheckBox *checkbox_timeline;

    QStringList years_list;
    QStringList month_year_list;
    QStringList day_month_year_list;
    QStringList hour_day_month_year_list;

    timeline_selection_window *timeline_selection_window_obj;

    QLabel *label_account_configure_list_link;
    QLabel *label_participants_list_link;

    QTableWidget *tablewidget_accounts_configure_list;
    QTableWidget *tablewidget_participants_list;

    QString selected_configured_account_name;
    QMap<QString, QStringList> map_of_accounts_with_particiapnts;

    QTabWidget *tabwidget_redefined_result;
    QChartView *chartView_for_pie_or_visit_count_obj;
    QPushButton *pushButton_save;
    QPushButton *pushButton_close_tab;

    QRadioButton *radiobutton_create_graph_yearly;
    QRadioButton *radiobutton_create_graph_monthly;
    QRadioButton *radiobutton_create_graph_daywise;
    QRadioButton *radiobutton_create_graph_hourly;

    QStackedWidget *stackwidget_obj;

    QPushButton *pushbutton_previous;
    QPushButton *pushbutton_next, *pushButton_export_graph, *pushButton_save_graph;

    int redefined_result_examiner_notes_index;

    QMap<int, QString> map_dates_labels;
    QLabel *label_show_date_time;
    QLabel *label_show_pages;

    QLabel *label_goto_page;
    QLineEdit *linedit_go_to_page;
    QPushButton *pushbutton_go_to_page;

    int stackwidget_index;

    QMenu *menu_graphs;
    QAction *act_show_data_of_current_barset;
    QAction *act_show_all_data;
    bool bool_check_barset;

    QMap<QString, QString> map_of_partipants_with_color;

    qint64 start_timestamp_int;
    qint64 end_timestamp_int;

    QString current_barset_category_str;
    QString current_barset_participant;
    QString current_clicked_domain;

    QList<QChartView*> list_of_charview;
    QMap<int, QList<QChartView*>> map_of_chartview;
    QMap<int, QMap<int, QString>> map_of_date_lables_map;
    QMultiMap<QString,struct_graphs_essentials> map_of_struct_graph_data;


    int redefined_result_bookmark_index;
    int tab_saved_index;
    bool bool_to_save_all_data_with_different_colour_barset;
    bool bool_save_data_for_graphs_tablewidet;

    save_result_dialog_box *save_result_dialog_box_obj;
    QPushButton *pushButton_msg_pie_chart_topurl_export;
    QPushButton *pushButton_save_url_pie;

    saved_graphs_dialog_box *save_graph_dialog_box_obj ;

    void insert_values_into_save_graph_db(QString graph_name , QString saved_graph_db_path , QString saved_graph_path);

    void contextMenuEvent(QContextMenuEvent *evt);
    void tablewidget_general_clicked(int row, int column);
    void action_open_full_detailed_info_triggered();
    void create_action_for_goto_artifact_source_submenu();
    void action_go_to_record_triggered();
    void action_remove_note_triggered();
    void update_notes_value(QString text, int current_row);
    void action_open_detach_detailed_info_triggered();
    void action_sendToBucket_hex_viewer_triggered();
    void action_sendToBucket_sqlite_viewer_triggered();
    void action_sendToBucket_plist_viewer_triggered();
    void action_open_with_plist_viewer_triggered();
    void action_open_with_hex_viewer_triggered();
    void action_open_with_sqlite_viewer_triggered();
    void action_export_triggered();
    void display_data(QString dest_db_path);

    int populate_data_in_tablewidget(QSqlDatabase &received_db_obj, QSqlQuery &query_index);
    void update_bookmark_value(QString status, int row, int column);
    void update_tags_value(QString tag_data, QString colour);


    void configure_ui_for_graphs();
    void set_graphs_essentail();
    void set_chart_in_stackwidget_after_setting_of_all_specification(redefined_result_full_loader::struct_redefined_result_single_chart_detail &struct_chart_detail_obj);
    void create_graphs_messenger();
    void create_graph_messenger_yearly();
    void create_graph_messenger_monthly();
    void create_graph_messenger_hourly();
    void create_graph_messenger_daywise();

    void set_timestamp_from_db_min_max();
    void create_graph_yearly_common_function();
    void create_graph_monthly_common_function();
    void create_graph_daywise_common_function();
    void create_graph_hourly_common_function();

    int populate_data_of_graph_in_tablewidget(QSqlQuery &query_index);
    int set_graphs_data_tablewidget_header();

    QStringList get_colors_stringlist_from_db();

    QString prepare_and_get_tooltip_string_for_barset(qint64 count, QString category, QString label);
    void create_graphs_browsers_history();
    void create_graph_browser_history_top_visit_count_domains();

    void display_data_of_visit_count_in_tablewidget(QString dest_db_path);
    void display_data_of_graph_in_tablewidget();
    void prepare_sqlquery_for_visit_count_tablewidget(QSqlQuery &query_index);
    void prepare_sqlquery_for_piegraph_tablewidget(QSqlQuery &query_index);
    void prepare_sqlquery_for_all_graphs_tablewidget(QSqlQuery &query_index);
    void set_graphs_essentials_for_data_only();///do not delete this function
    void create_piechart_for_selected_account_for_messenger();
    void set_data_in_configured_accounts_list();
    QStringList get_participant_list_of_account(QString account_name);

    void save_redefined_result_data_in_case_treewidget(QString tab_to_be_saved);
    QString create_db_for_redefined_saved_result(QString db_name);
    void insert_values_in_save_result_db(QSqlQuery &index_query, QString db_path, QString result_name);
    void extract_and_save_result(QString saved_db_path, QString result_name);
    void saved_db_info(QString db_name,QString command , QString db_path);
    export_dialog_box_1 *export_dialog_box_1_obj;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading;

    void loading_display_dialog_based_on_records(int total_records, int processed_count);


    void action_bookmark_triggered();
    void action_remove_bookmark_triggered();
    int visit_count_index;
    int populate_data_in_tablewidget_browser_history(QSqlDatabase &received_db_obj,QSqlQuery &query_index);
    int populate_data_tablewidget_messenger(QSqlDatabase &received_db_obj,QSqlQuery &query_index);
    int populate_data_of_graph_in_tablewidget_messenger(QSqlQuery &query_index);
    int populate_data_of_graph_in_tablewidget_browser_history(QSqlQuery &query_index);

    QMap <QString, QString> qmap_barset_participants_str;
    QMap <QString, QString> qmap_barset_catgories_str;
    QMap <QString, QString> qmap_domians_str;
    QMap <QString, bool> qmap_bool_save_data_other_than_pie_and_visit_count_bool;
    QMap <QString, QString> qmap_configured_account_str;
    QMap <QString, QString> qmap_bar_graph_radiobutton_type_str;
    void prepare_sqlquery_for_piegraph_tablewidget_to_save_data(QSqlQuery &query_index , QString account_name_str);
    void prepare_sqlquery_for_all_graphs_tablewidget_to_save_data(QSqlQuery &query_index , QString account_name_str, QString graph_type_str);
    QString get_graph_type();

    void all_column_text_search(QList<struct_search_column_text_for_file_type> search_text_all_column_list);
    QString new_tab_name;

};

#endif //
