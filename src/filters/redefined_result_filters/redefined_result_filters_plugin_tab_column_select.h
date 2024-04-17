#ifndef REDEFINED_RESULT_FILTERS_PLUGIN_TAB_COLUMN_SELECT_H
#define REDEFINED_RESULT_FILTERS_PLUGIN_TAB_COLUMN_SELECT_H

#include <QFrame>
#include<QtCore>
#include "message_box/message_dialog.h"

namespace Ui {
class redefined_result_filters_plugin_tab_column_select;
}

class redefined_result_filters_plugin_tab_column_select : public QFrame
{
    Q_OBJECT

public:
    explicit redefined_result_filters_plugin_tab_column_select(QWidget *parent = 0);
    ~redefined_result_filters_plugin_tab_column_select();

    void populate_columns_in_tablewidget(QStringList sel_col_list);
    QString func_to_set_stylesheet();
    void pub_set_filter_row_and_column(int row, int column);


signals:
    void signal_sel_col_name(QString, int, int);

private slots:
    void on_pushButton_add_clicked();

private:
    Ui::redefined_result_filters_plugin_tab_column_select *ui;
    message_dialog *message_dialog_ptr;
    int filter_row, filter_column;

};

#endif
