#ifndef SELECT_SOURCE_INTERFACE_H
#define SELECT_SOURCE_INTERFACE_H

#include <QDialog>
#include <QtCore>
#include <QCheckBox>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include <QMessageBox>
#include "header_global_variables_2.h"


namespace Ui {
class select_source_interface;
}

class select_source_interface : public QDialog
{
    Q_OBJECT

public:
    explicit select_source_interface(QWidget *parent = 0);
    ~select_source_interface();


    void pub_put_prevoius_selected_source_list(QStringList source_count_name_list);
    void pub_set_ui_essentials();
    void pub_insert_data_in_table();


signals:

    void signal_selected_source_list(QStringList selected_src_count_nm_list);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_checkBox_all_source_clicked(bool checked);
    void on_tableWidget_source_cellClicked(int row, int column);

private:
    Ui::select_source_interface *ui;

    enum enum_tablewidget_item_column_index
    {
        enum_tablewidget_item_column_checkbox = 0,
        enum_tablewidget_item_column_source_count_name,
        enum_tablewidget_item_column_complete_source_name,
        enum_tablewidget_item_column_blank
    };



    QStringList prevoius_selected_src_count_name_list;
    bool is_all_source_selected();
};

#endif // SELECT_SOURCE_INTERFACE_H
