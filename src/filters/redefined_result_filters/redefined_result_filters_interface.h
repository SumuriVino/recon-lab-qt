#ifndef REDEFINED_RESULT_FILTERS_INTERFACE_H
#define REDEFINED_RESULT_FILTERS_INTERFACE_H

#include <QFrame>
#include "ui_custom_classes/m_tablewidget.h"
#include "recon_generic_functions/recon_static_functions.h"
                                   
#include "header_global_variables_1.h"
#include "redefined_result_filters_plugins_tab_select.h"
#include "redefined_result_filters_plugin_tab_column_select.h"
#include "redefined_result_filters_add_new_filter.h"
#include "redefined_result/redefined_result_full/redefined_result_full_loader.h"
#include "header_identical_1.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


#include "message_box/message_dialog.h"
#include<QList>
#include<QtCore>

namespace Ui {
class redefined_result_filters_interface;
}

class redefined_result_filters_interface : public QFrame
{
    Q_OBJECT

public:
    explicit redefined_result_filters_interface(QWidget *parent = 0);
    ~redefined_result_filters_interface();

    void pub_display_plugins_category_list_interface();

private slots:

    void slot_selected_plugin_and_tab(QString plugin_name, QString tab_name);
    void slot_select_column_name(QString sel_col_str, int row, int column);
    void slot_add_new_filter_in_tablewidget(QString category_name);
    void slot_remove_column_pushButton_clicked();
    void slot_pushButton_add_filter_clicked();
    void slot_pushButton_remove_filter_clicked();
    void slot_tableWidget_filter_name_cellClicked(int row, int column);
    void slot_pushButton_add_column_clicked();
    void slot_pushButton_add_row_clicked();
    void slot_pushButton_remove_selected_columns_clicked();
    void slot_pushButton_remove_selected_row_clicked();
    void slot_pushbutton_widget_add_coloum_clicked();
    void slot_lineEdit_add_column_text_changed(QString arg1);
    void slot_tableWidget_filter_item_DoubleClicked(int row, int column);
private:
    Ui::redefined_result_filters_interface *ui;

    message_dialog *message_dialog_obj;
    QStringList redefined_result_filters_list;

    QString redefined_result_filter_db_path;

    void set_plugin_specific_tab_list(QString);
    void after_remove_column_insertion_of_table_into_db();
    QString stylesheet_for_tablewidgets(void);
    void create_redefined_result_filters_database_and_tables(void);
    void display_redefined_result_filter_names();
    void display_redefined_result_items_on_click_result_name();
    void create_tableWidget_result_item_headers(QStringList);
    int header_count_for_category(QString category_INT);

    redefined_result_filters_plugins_tab_select *redefined_result_filters_plugins_tab_select_obj;
    redefined_result_filters_plugin_tab_column_select *redefined_result_filters_plugin_tab_column_select_obj;
    redefined_result_filters_add_new_filter *redefined_result_filters_add_new_filter_obj;

    void create_gui();
    QVBoxLayout *vboxlayout_main_central_display;
    QHBoxLayout *hboxlayout_upper_main_display;

    void create_filters_name_layout();
    QVBoxLayout *vboxLayout_filters_name_section;
    QLabel *label_filters_name_heading;
    QTableWidget *tableWidget_filters_name;
    QPushButton *pushButton_add_filter;
    QPushButton *pushButton_remove_filter;

    void create_filter_items_layout();
    QVBoxLayout *vboxlayout_filters_items_section;

    QLabel *label_filters_item_heading;
    QPushButton *pushButton_add_column;
    QPushButton *pushButton_add_row;
    QPushButton *pushButton_remove_selected_row;
    QPushButton *pushButton_remove_selected_columns;
    QTableWidget *tableWidget_filters_item;

    void create_widget_for_the_add_coulum_lineEdit();
    QWidget *widget_add_column;
    QLineEdit *lineEdit_column_name;
    QPushButton *pushButton_widget_add_coloum;
    QLabel *label_add_coloum_widget_message;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // REDEFINED_RESULT_FILTERS_INTERFACE_H
