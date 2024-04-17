#ifndef MANAGE_PLUGIN_COLUMNS_H
#define MANAGE_PLUGIN_COLUMNS_H

#include <QFrame>
#include<QtCore>
namespace Ui {
class manage_plugin_columns;
}

class manage_plugin_columns : public QFrame
{
    Q_OBJECT

public:
    explicit manage_plugin_columns(QWidget *parent = 0);
    ~manage_plugin_columns();

    void draw_table_manage_plugins_columns(QStringList var);
    QList<int> popup_hid_col_lst_var;
    QList<QString> popup_hid_col_name_lst_var;

private:
    Ui::manage_plugin_columns *ui;


private slots:
    void slot_popup_horizontal_header_clicked(int column);
    void slot_on_tableWidget_popup_cellClicked(int row,int column);
    void slot_manage_columns_checkstate();

signals:
    void signal_manage_column_clicked(int row , int check_status);

};

#endif // MANAGE_PLUGIN_COLUMNS_H
