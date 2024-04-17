#include "gpt_template.h"
#include "ui_gpt_template.h"

gpt_template::gpt_template(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gpt_template)
{
    ui->setupUi(this);
}

gpt_template::~gpt_template()
{
    delete ui;
}

void gpt_template::pub_set_data(QByteArray data)
{
    gpt_data = data;

    gpt_parser parser;

    parser.parse(gpt_data);

    QList<common_structure_for_data_table> protective_mbr_data = parser.gpt_protective_mbr_data();

    QTableWidget *tableWidget_record_header = new QTableWidget(this);
    tableWidget_record_header->setColumnCount(6);
    tableWidget_record_header->horizontalHeader()->setStretchLastSection(true);
    tableWidget_record_header->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_record_header->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_record_header->setWordWrap(true);

    tableWidget_record_header->setHorizontalHeaderItem(0, new QTableWidgetItem("Offset (HEX)"));
    tableWidget_record_header->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset (DEC)"));
    tableWidget_record_header->setHorizontalHeaderItem(2, new QTableWidgetItem("Length"));
    tableWidget_record_header->setHorizontalHeaderItem(3, new QTableWidgetItem("Value"));
    tableWidget_record_header->setColumnWidth(3, 180);
    tableWidget_record_header->setHorizontalHeaderItem(4, new QTableWidgetItem("Value (Hex)"));
    tableWidget_record_header->setColumnWidth(4, 180);
    tableWidget_record_header->setHorizontalHeaderItem(5, new QTableWidgetItem("Description"));


    for(int i = 0 ; i < protective_mbr_data.size(); i++)
    {
        common_structure_for_data_table obj = protective_mbr_data.at(i);

        tableWidget_record_header->insertRow(i);
        tableWidget_record_header->setItem(i, 0, new QTableWidgetItem(obj.offset_hex));
        tableWidget_record_header->setItem(i, 1, new QTableWidgetItem(obj.offset_dec));
        tableWidget_record_header->setItem(i, 2, new QTableWidgetItem(obj.length));
        tableWidget_record_header->setItem(i, 3, new QTableWidgetItem(obj.value));
        tableWidget_record_header->setItem(i, 4, new QTableWidgetItem(obj.value_hex));
        tableWidget_record_header->setItem(i, 5, new QTableWidgetItem(obj.description));
    }



    tableWidget_record_header->setMinimumHeight((protective_mbr_data.size() + 1) * 22);
    ui->verticalLayout_scroll_area->addWidget(new QLabel("Protective MBR"));
    ui->verticalLayout_scroll_area->addWidget(tableWidget_record_header);


    QList<common_structure_for_data_table> gpt_header_data = parser.gpt_header();

    QTableWidget *tableWidget_gpt_header = new QTableWidget(this);
    tableWidget_gpt_header->setColumnCount(6);
    tableWidget_gpt_header->horizontalHeader()->setStretchLastSection(true);
    tableWidget_gpt_header->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_gpt_header->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_gpt_header->setWordWrap(true);

    tableWidget_gpt_header->setHorizontalHeaderItem(0, new QTableWidgetItem("Offset (HEX)"));
    tableWidget_gpt_header->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset (DEC)"));
    tableWidget_gpt_header->setHorizontalHeaderItem(2, new QTableWidgetItem("Length"));
    tableWidget_gpt_header->setHorizontalHeaderItem(3, new QTableWidgetItem("Value"));
    tableWidget_gpt_header->setColumnWidth(3, 180);
    tableWidget_gpt_header->setHorizontalHeaderItem(4, new QTableWidgetItem("Value (Hex)"));
    tableWidget_gpt_header->setColumnWidth(4, 180);
    tableWidget_gpt_header->setHorizontalHeaderItem(5, new QTableWidgetItem("Description"));


    for(int i = 0 ; i < gpt_header_data.size(); i++)
    {
        common_structure_for_data_table obj = gpt_header_data.at(i);

        tableWidget_gpt_header->insertRow(i);
        tableWidget_gpt_header->setItem(i, 0, new QTableWidgetItem(obj.offset_hex));
        tableWidget_gpt_header->setItem(i, 1, new QTableWidgetItem(obj.offset_dec));
        tableWidget_gpt_header->setItem(i, 2, new QTableWidgetItem(obj.length));
        tableWidget_gpt_header->setItem(i, 3, new QTableWidgetItem(obj.value));
        tableWidget_gpt_header->setItem(i, 4, new QTableWidgetItem(obj.value_hex));
        tableWidget_gpt_header->setItem(i, 5, new QTableWidgetItem(obj.description));
    }

    tableWidget_gpt_header->setMinimumHeight((gpt_header_data.size() + 1) * 22);
    ui->verticalLayout_scroll_area->addWidget(new QLabel("GPT Header"));
    ui->verticalLayout_scroll_area->addWidget(tableWidget_gpt_header);



    QList<QList<common_structure_for_data_table>> partition_list = parser.gpt_partition_tables();

    int m_count = 0;
    foreach (QList<common_structure_for_data_table> part_detail, partition_list) {

        QTableWidget *tbl = new QTableWidget(this);
        tbl->setColumnCount(6);
        tbl->horizontalHeader()->setStretchLastSection(true);
        tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
        tbl->setWordWrap(true);

        tbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Offset (HEX)"));
        tbl->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset (DEC)"));
        tbl->setHorizontalHeaderItem(2, new QTableWidgetItem("Length"));
        tbl->setHorizontalHeaderItem(3, new QTableWidgetItem("Value"));
        tbl->setColumnWidth(3, 180);
        tbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Value (Hex)"));
        tbl->setColumnWidth(4, 180);
        tbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Description"));

        for(int i = 0 ; i < part_detail.size(); i++)
        {
            common_structure_for_data_table obj = part_detail.at(i);

            tbl->insertRow(i);
            tbl->setItem(i, 0, new QTableWidgetItem(obj.offset_hex));
            tbl->setItem(i, 1, new QTableWidgetItem(obj.offset_dec));
            tbl->setItem(i, 2, new QTableWidgetItem(obj.length));
            tbl->setItem(i, 3, new QTableWidgetItem(obj.value));
            tbl->setItem(i, 4, new QTableWidgetItem(obj.value_hex));
            tbl->setItem(i, 5, new QTableWidgetItem(obj.description));
        }

        tbl->setMinimumHeight((part_detail.size() + 1) * 22);
        ui->verticalLayout_scroll_area->addWidget(new QLabel("Partition " + QString::number(m_count + 1)));
        ui->verticalLayout_scroll_area->addWidget(tbl);
        m_count++;

    }
}
