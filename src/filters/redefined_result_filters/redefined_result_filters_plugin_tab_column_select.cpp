#include "redefined_result_filters_plugin_tab_column_select.h"
#include "ui_redefined_result_filters_plugin_tab_column_select.h"

redefined_result_filters_plugin_tab_column_select::redefined_result_filters_plugin_tab_column_select(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::redefined_result_filters_plugin_tab_column_select)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    QFont m_font = font();

    m_font.setPointSize(12);
    ui->tableWidget_column_list->setFont(m_font);

    QString stylesheet = func_to_set_stylesheet();
    ui->tableWidget_column_list->setStyleSheet(stylesheet);

    message_dialog_ptr = new message_dialog(this);
    message_dialog_ptr->pub_set_message(MACRO_MSG_INFORMATION,"Please set plugin column name");
}

redefined_result_filters_plugin_tab_column_select::~redefined_result_filters_plugin_tab_column_select()
{
    delete ui;
}

QString redefined_result_filters_plugin_tab_column_select::func_to_set_stylesheet()
{
    QString stylesheet = QString(
                "QTableView::item:selected:!active {"
                "color:white;"
                "background-color:  #a1c9f7;"
                "}"

                "QTableView::indicator:checked {background-color:black;border: 1px solid #d6d6d6;}"
                "QTableView::indicator:unchecked {background-color:#f1f1f1;border: 1px solid #777777;}"

                "QTableView::indicator {"
                "width: 13px;"
                "height: 13px;}"
                );

    return stylesheet;
}

void redefined_result_filters_plugin_tab_column_select::pub_set_filter_row_and_column(int row, int column)
{
    filter_column = column;
    filter_row = row;
}

void redefined_result_filters_plugin_tab_column_select::populate_columns_in_tablewidget(QStringList sel_col_list)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    ui->tableWidget_column_list->setRowCount(0);

    QStringList columns_list;
    columns_list.clear();

    for(int i=0; i< sel_col_list.size(); i++){

        if((i==0) || (i == 1)){
            continue;
        }

        columns_list << sel_col_list.at(i);
    }

    for(int itr=0; itr < columns_list.size(); itr++){
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(columns_list.at(itr));

        ui->tableWidget_column_list->insertRow(itr);
        ui->tableWidget_column_list->setItem(itr,0,item);
        ui->tableWidget_column_list->setRowHeight(itr,23);
    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);

}

void redefined_result_filters_plugin_tab_column_select::on_pushButton_add_clicked()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(ui->tableWidget_column_list->currentRow() == -1){
        message_dialog_ptr->show();
        return;
    }

    QString sel_col_str = ui->tableWidget_column_list->currentItem()->text();
    emit signal_sel_col_name(sel_col_str, filter_row, filter_column);
    close();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}
