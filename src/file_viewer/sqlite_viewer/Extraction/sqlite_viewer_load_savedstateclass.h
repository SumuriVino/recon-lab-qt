#ifndef SQLITE_VIEWER_LOAD_SAVED_STATE_CLASS_H
#define SQLITE_VIEWER_LOAD_SAVED_STATE_CLASS_H

#include <QObject>
#include<QSql>
#include<QSqlQuery>
#include<QSqlError>
#include<QHeaderView>
#include<QtGui>
#include<QComboBox>
#include<QSqlField>
#include<QSqlRecord>
#include<QWidget>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "header_gui_settings.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      

class sqlite_viewer_load_saved_state_class : public QWidget
{
    Q_OBJECT
public:
    explicit sqlite_viewer_load_saved_state_class(QWidget *parent = 0);
    ~sqlite_viewer_load_saved_state_class();

    QList<struct_global_sqlite_viewer_tablewidget_main> load_saved_state(QString path);

    void saved_state_dir_name_pub(QString);
    void main_datadisplay_table_sqlite_name_pub(QString MainDataDisplay_SavedState);
    void filter_view_table_sqlite_name_pub(QString FilterViewTable_SavedState);

    QTableWidget *populate_toLoad_tablewidget_column_selection(struct_global_sqlite_viewer_single_dbtable_info struct_obj, QString table_name);
    struct_global_sqlite_viewer_single_dbtable_info load_dbtable_info(QString table_name,QString destination_db_path);



    void pub_set_display_loading_bar(display_loading_progress_bar *obj);



    void pub_set_db_path_for_refresh(QString filter_db_path, QString main_db_path);
    void pub_set_current_tab_detail(QString tab_name, m_tablewidget *current_tab_widget);

signals:
    void signal_onload_combobox_conversion_change();
    void signal_onload_update_export_checkbox_and_count(bool,int);

private slots:
    void slot_onload_dynamic_combobox(QString text);
    void slot_hide_loading_display_dialog_box(bool cancel_status);

    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

private:
    QString destination_db_main_display_table_file;
    QString destination_db_filter_view_table_file;

    ///-Load Case
    void load_saved_state_private(QString path);


    m_tablewidget* populate_toLoad_tablewidget_main_data_display(struct_global_sqlite_viewer_single_dbtable_info obj_struct, QString dbTableName , int progressing_table_count, int total_table_count);

    QTableWidget *populate_toLoad_tablewidget_filter_view_search(struct_global_sqlite_viewer_single_dbtable_info struct_obj, QString dbTableName);

    QStringList get_data_type_from_database_table_to_load(QSqlDatabase destination_db, QString table_name);

    recon_helper_standard *recon_helper_standard_obj;

    QString saved_state_dir_name;
    QString main_datadisplay_table_sqlite_name;
    QString filter_view_table_sqlite_name;

    QList<struct_global_sqlite_viewer_tablewidget_main> list_struct_tablewidget_data_sqlite_viewer;

    bool bool_global_export_checkbox_enable_sqlite_viewer;
    int global_export_checkbox_disable_sqlite_viewer;

    display_loading_progress_bar * display_loading_progress_bar_obj;
    bool bool_cancel_loading;

    QList<custom_table_header_widget *> custom_header_widgets_QList;
    struct struct_search_column_text_for_sqlite_viewer
    {
        int column_number;
        QString search_text;
    };

    enum enum_tablewidget_column{
        enum_tablewidget_column_bookmark,
        enum_tablewidget_column_tags
    };

    custom_table_header_widget *custom_table_header_widget_obj;
    QString current_table_name;
    QString current_tab_name;
    m_tablewidget *current_tab_tableWidget_obj;
    QMap<QString, QList<custom_table_header_widget *>> tab_custom_header_widgets_QMap;

    void all_column_text_search(QList<struct_search_column_text_for_sqlite_viewer> search_text_all_column_list);
    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;
};

#endif //
