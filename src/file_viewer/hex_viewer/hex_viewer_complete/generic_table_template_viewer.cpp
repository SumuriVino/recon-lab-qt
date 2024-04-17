#include "generic_table_template_viewer.h"
#include "ui_generic_table_template_viewer.h"

generic_table_template_viewer::generic_table_template_viewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::generic_table_template_viewer)
{
    ui->setupUi(this);
}

generic_table_template_viewer::~generic_table_template_viewer()
{
    delete ui;
}

void generic_table_template_viewer::pub_set_table_data(QList<common_structure_for_data_table> list)
{
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setWordWrap(false);

    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Offset (HEX)"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset (DEC)"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Length"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Value"));
    ui->tableWidget->setColumnWidth(3, 180);
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Value (Hex)"));
    ui->tableWidget->setColumnWidth(4, 180);
    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Description"));

    for(int i = 0 ; i < list.size(); i++)
    {
        common_structure_for_data_table obj = list.at(i);

        QTableWidgetItem *item_length = new QTableWidgetItem(obj.length);
        item_length->setTextAlignment(Qt::AlignHCenter | Qt::AlignRight);
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(obj.offset_hex));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(obj.offset_dec));
        ui->tableWidget->setItem(i, 2, item_length);
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(obj.value));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(obj.value_hex));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(obj.description));
    }


}

void generic_table_template_viewer::pub_set_title(QString str)
{
    setWindowTitle(str);
}
