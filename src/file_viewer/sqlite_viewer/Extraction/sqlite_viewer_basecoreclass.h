#ifndef SQLITE_VIEWER_BASE_CORE_CLASS_H
#define SQLITE_VIEWER_BASE_CORE_CLASS_H

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
#include "ui_custom_classes/m_tablewidget.h"
#include "header_gui_settings.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"


class sqlite_viewer_base_core_class : public QWidget
{
    Q_OBJECT
public:
    explicit sqlite_viewer_base_core_class(QWidget *parent = 0);
    ~sqlite_viewer_base_core_class();

    QList<struct_global_sqlite_viewer_tablewidget_main> prepare_and_fill_structure_for_main_table_display(QString sourceFile,QString copiedSourceFile,QString recreateSourceFile);
    QStringList pub_get_db_tablename_list();

    QTableWidget *populate_tablewidget_columns_selection(struct_global_sqlite_viewer_single_dbtable_info struct_obj,QString tab_name);

    bool pub_cancel_loading();

    void pub_set_display_loading_bar(display_loading_progress_bar *obj);
    QList<custom_table_header_widget *> custom_header_widgets_QList;
    void pub_set_current_tab_detail(QString tab_name, m_tablewidget *current_tab_widget);
    QMap<QString, QList<custom_table_header_widget *>> tab_custom_header_widgets_QMap;

signals:
    void signal_combobox_conversion_change();
    void signal_update_export_checkbox_and_count(bool,int);

public slots:
    void slot_dynamic_combobox(QString);
    void slot_hide_loading_display_dialog_box(bool cancel_status);

private slots:
    void slot_custom_table_header_widget_search_field_return_pressed();
    void slot_custom_table_header_widget_search_field_text_changed();
    void slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order);

private:

    QString source_file_path;
    QString copied_source_file;
    QString recreated_source_file;

    QStringList get_db_tablename_list();

    struct_global_sqlite_viewer_single_dbtable_info get_db_table_info(QString table_name);

    m_tablewidget* populate_tablewidget_main_data_display(struct_global_sqlite_viewer_single_dbtable_info obj_struct, QString dbTableName, int table_count);

    QTableWidget *populate_tablewidget_filter_view_search(struct_global_sqlite_viewer_single_dbtable_info struct_obj, QString dbTableName);

    QStringList get_data_type_from_database_table(QSqlDatabase &destination_db, QString table_name);

    QList<struct_global_sqlite_viewer_tablewidget_main> list_struct_tablewidget_data_sqlite_viewer;

    bool bool_global_export_checkbox_enable_sqlite_viewer;
    int global_export_checkbox_disable_sqlite_viewer;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading;

    struct struct_search_column_text_for_sqlite_viewer
    {
        int column_number;
        QString search_text;
    };

    custom_table_header_widget *custom_table_header_widget_obj;

    enum enum_tablewidget_column{
        enum_tablewidget_column_bookmark,
        enum_tablewidget_column_tags
    };

    QString current_table_name;
    QString current_tab_name;
    m_tablewidget *current_tab_tableWidget_obj;

    void all_column_text_search(QList<struct_search_column_text_for_sqlite_viewer> search_text_all_column_list);

    bool bool_stop_sorting_tags_bookmark_notes_after_search = false;

};

#endif
