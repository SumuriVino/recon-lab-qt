#include "mbr_template.h"
#include "ui_mbr_template.h"

mbr_template::mbr_template(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mbr_template)
{
    ui->setupUi(this);

    create_ui();
}

mbr_template::~mbr_template()
{
    delete ui;
}

void mbr_template::pub_set_data(QByteArray ar)
{
    mbr_data = ar;

    QByteArray partition_1_data = ar.mid(446, 16);
    QByteArray partition_2_data = ar.mid(462, 16);
    QByteArray partition_3_data = ar.mid(478, 16);
    QByteArray partition_4_data = ar.mid(494, 16);


    fill_partition_table(tableWidget_partition1, partition_1_data);
    fill_partition_table(tableWidget_partition2, partition_2_data);
    fill_partition_table(tableWidget_partition3, partition_3_data);
    fill_partition_table(tableWidget_partition4, partition_4_data);

}

void mbr_template::create_ui()
{

    tableWidget_partition1 = new QTableWidget(this);
    tableWidget_partition2 = new QTableWidget(this);
    tableWidget_partition3 = new QTableWidget(this);
    tableWidget_partition4 = new QTableWidget(this);

    table_list << tableWidget_partition1 << tableWidget_partition2 << tableWidget_partition3 << tableWidget_partition4;

    foreach (QTableWidget *table, table_list) {
        QTableWidgetItem *item = new QTableWidgetItem("Data");
        QTableWidgetItem *item1 = new QTableWidgetItem("Value");
        QTableWidgetItem *item2 = new QTableWidgetItem("Description");

        table->setColumnCount(3);
        table->setHorizontalHeaderItem(0, item);
        table->setHorizontalHeaderItem(1, item1);
        table->setHorizontalHeaderItem(2, item2);


        table->setColumnWidth(0, 70);
        table->setColumnWidth(1, 70);

        table->setMinimumHeight(160);
        table->horizontalHeader()->setStretchLastSection(true);
        table->setWordWrap(false);
        //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }




    QLabel *label_part1 = new QLabel("Partition 1");
    QLabel *label_part2 = new QLabel("Partition 2");
    QLabel *label_part3 = new QLabel("Partition 3");
    QLabel *label_part4 = new QLabel("Partition 4");

    QVBoxLayout *vbox = ui->verticalLayout_scrollarea;

    vbox->addWidget(label_part1);
    vbox->addWidget(tableWidget_partition1);

    vbox->addWidget(label_part2);
    vbox->addWidget(tableWidget_partition2);

    vbox->addWidget(label_part3);
    vbox->addWidget(tableWidget_partition3);


    vbox->addWidget(label_part4);
    vbox->addWidget(tableWidget_partition4);

    //ui->scrollArea->setLayout(vbox);

    //    QVBoxLayout *mainVbox = new QVBoxLayout;
    //    mainVbox->addWidget(scrollArea);

    //    setLayout(mainVbox);
}

void mbr_template::fill_partition_table(QTableWidget *table, QByteArray partition_data)
{
    QByteArray boot_indicator = partition_data.mid(0, 1);
    QString boot_indicator_string;

    if(boot_indicator.at(0) == 0x80)
        boot_indicator_string = "Bootable";
    else
        boot_indicator_string = "Non Bootable";


    QByteArray starting_CHS;
    starting_CHS.append(partition_data.at(3));
    starting_CHS.append(partition_data.at(2));
    starting_CHS.append(partition_data.at(1));

    QByteArray partition_type = partition_data.mid(0x04, 1);

    QByteArray ending_CHS;
    ending_CHS.append(partition_data.at(0x07));
    ending_CHS.append(partition_data.at(0x06));
    ending_CHS.append(partition_data.at(0x05));

    QByteArray  start_sector;
    start_sector.append(partition_data.at(0x0B));
    start_sector.append(partition_data.at(0x0A));
    start_sector.append(partition_data.at(0x09));
    start_sector.append(partition_data.at(0x08));

    QByteArray  total_sector;
    total_sector.append(partition_data.at(0x0f));
    total_sector.append(partition_data.at(0x0e));
    total_sector.append(partition_data.at(0x0d));
    total_sector.append(partition_data.at(0x0c));


    table->insertRow(0);
    table->setItem(0, 0, new QTableWidgetItem(QString(boot_indicator.toHex())));
    table->setItem(0, 1, new QTableWidgetItem(boot_indicator_string));
    table->setItem(0, 2, new QTableWidgetItem("Boot Indicator"));

    table->insertRow(1);
    table->setItem(1, 0, new QTableWidgetItem(QString(starting_CHS.toHex())));
    table->setItem(1, 1, new QTableWidgetItem(QString::number(starting_CHS.toHex().toLongLong(0,16))));
    table->setItem(1, 2, new QTableWidgetItem("Starting CHS value address"));

    table->insertRow(2);
    table->setItem(2, 0, new QTableWidgetItem(QString(partition_type.toHex())));
    table->setItem(2, 1, new QTableWidgetItem(partition_type_to_string(partition_type)));
    table->setItem(2, 2, new QTableWidgetItem("Partition Type"));

    table->insertRow(3);
    table->setItem(3, 0, new QTableWidgetItem(QString(ending_CHS.toHex())));
    table->setItem(3, 1, new QTableWidgetItem(QString::number(ending_CHS.toHex().toLongLong(0,16))));
    table->setItem(3, 2, new QTableWidgetItem("Ending CHS value address"));

    table->insertRow(4);
    table->setItem(4, 0, new QTableWidgetItem(QString(start_sector.toHex())));
    table->setItem(4, 1, new QTableWidgetItem(QString::number(start_sector.toHex().toLongLong(0,16))));
    table->setItem(4, 2, new QTableWidgetItem("Start Sector"));

    table->insertRow(5);
    table->setItem(5, 0, new QTableWidgetItem(QString(total_sector.toHex())));
    table->setItem(5, 1, new QTableWidgetItem(QString::number(total_sector.toHex().toLongLong(0,16))));
    table->setItem(5, 2, new QTableWidgetItem("Total Sectors"));

}

QString mbr_template::partition_type_to_string(QByteArray ar)
{

    unsigned char part_code = ( unsigned char)ar.at(0);

    if(part_code == 0x04)
        return "DOS FAT 16";
    else if(part_code == 0x05)
        return "DOS Extended Partition";
    else if(part_code == 0x06)
        return "FAT 16";
    else if(part_code == 0x07)
        return "NTFS";
    else if(part_code == 0x0B)
        return "FAT 32";
    else if(part_code == 0x0C)
        return "FAT 32x";
    else if(part_code == 0x0F)
        return "Extended Partition";
    else if(part_code == 0x83)
        return "Linux";
    else if(part_code == 0xEE)
        return "GPT";
    else
        return "Unknown";

}
