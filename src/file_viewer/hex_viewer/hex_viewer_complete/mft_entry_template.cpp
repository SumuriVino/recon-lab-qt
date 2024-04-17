#include "mft_entry_template.h"
#include "ui_mft_entry_template.h"

MFT_entry_template::MFT_entry_template(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MFT_entry_template)
{
    ui->setupUi(this);

    setWindowTitle("$MFT Entry");
}

MFT_entry_template::~MFT_entry_template()
{
    delete ui;
}

void MFT_entry_template::pub_set_data(QByteArray data)
{
    mft_entry_data = data;

    ntfs_parser parser;

    QList<common_structure_for_data_table> mft_header_record = parser.ntfs_mft_entry_file_record_header(mft_entry_data);

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
    tableWidget_record_header->setHorizontalHeaderItem(4, new QTableWidgetItem("Value (Hex)"));
    tableWidget_record_header->setHorizontalHeaderItem(5, new QTableWidgetItem("Description"));


    for(int i = 0 ; i < mft_header_record.size(); i++)
    {
        common_structure_for_data_table obj = mft_header_record.at(i);

        tableWidget_record_header->insertRow(i);
        tableWidget_record_header->setItem(i, 0, new QTableWidgetItem(obj.offset_hex));
        tableWidget_record_header->setItem(i, 1, new QTableWidgetItem(obj.offset_dec));
        tableWidget_record_header->setItem(i, 2, new QTableWidgetItem(obj.length));
        tableWidget_record_header->setItem(i, 3, new QTableWidgetItem(obj.value));
        tableWidget_record_header->setItem(i, 4, new QTableWidgetItem(obj.value_hex));
        tableWidget_record_header->setItem(i, 5, new QTableWidgetItem(obj.description));
    }

    tableWidget_record_header->setColumnWidth(3, 170);
    tableWidget_record_header->setColumnWidth(4, 120);


    tableWidget_record_header->setMinimumHeight((mft_header_record.size() + 1) * 22);
    ui->verticalLayout_scoll_area->addWidget(new QLabel("File Record Header"));
    ui->verticalLayout_scoll_area->addWidget(tableWidget_record_header);


    QList<ntfs_attribute_and_offset> attr_list = parser.get_mft_attributes_list(mft_entry_data);

    QList<common_structure_for_data_table> m_list;
    QString attr_name;
    foreach (ntfs_attribute_and_offset attr, attr_list) {

        attr_name = parser.attribute_type_to_name(attr.attribute_type);

        m_list.clear();
        m_list = parser.ntfs_mft_parse_attribute(data, attr);

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
        tbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Value (Hex)"));
        tbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Description"));

        tbl->setColumnWidth(3, 170);
        tbl->setColumnWidth(4, 120);

        for(int row = 0 ; row < m_list.size(); row++)
        {
            common_structure_for_data_table obj = m_list.at(row);

            tbl->insertRow(row);
            tbl->setItem(row, 0, new QTableWidgetItem(obj.offset_hex));
            tbl->setItem(row, 1, new QTableWidgetItem(obj.offset_dec));
            tbl->setItem(row, 2, new QTableWidgetItem(obj.length));
            tbl->setItem(row, 3, new QTableWidgetItem(obj.value));
            tbl->setItem(row, 4, new QTableWidgetItem(obj.value_hex));
            tbl->setItem(row, 5, new QTableWidgetItem(obj.description));
        }

        tbl->setMinimumHeight((m_list.size() + 1) * 22);

        ui->verticalLayout_scoll_area->addWidget(new QLabel(attr_name));
        ui->verticalLayout_scoll_area->addWidget(tbl);
    }
}
