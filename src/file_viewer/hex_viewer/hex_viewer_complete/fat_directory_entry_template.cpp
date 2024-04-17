#include "fat_directory_entry_template.h"
#include "ui_fat_directory_entry_template.h"

fat_directory_entry_template::fat_directory_entry_template(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fat_directory_entry_template)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(1, 70);
    ui->tableWidget->setColumnWidth(2, 120);
}

fat_directory_entry_template::~fat_directory_entry_template()
{
    delete ui;
}

void fat_directory_entry_template::pub_set_data(QByteArray ar)
{
    dir_entry_data = ar;

    extract_file_name();

    extract_file_extension();

    extract_attribute_byte();

    extract_file_size();
}

void fat_directory_entry_template::add_entry_to_table(QString byteoffset, QString size, QString hex_value, QString value, QString desc)
{
    QTableWidgetItem *item_byteoffset = new QTableWidgetItem(byteoffset);
    QTableWidgetItem *item_size       = new QTableWidgetItem(size);
    QTableWidgetItem *item_hex_value  = new QTableWidgetItem(hex_value);
    QTableWidgetItem *item_value      = new QTableWidgetItem(value);
    QTableWidgetItem *item_desc       = new QTableWidgetItem(desc);


    int row = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, item_byteoffset);
    ui->tableWidget->setItem(row, 1, item_size);
    ui->tableWidget->setItem(row, 2, item_hex_value);
    ui->tableWidget->setItem(row, 3, item_value);
    ui->tableWidget->setItem(row, 4, item_desc);

}

void fat_directory_entry_template::extract_file_name()
{
    QByteArray filename = dir_entry_data.mid(0, 8);

    QString file_name_str = QString::fromLocal8Bit(filename);

    file_name_str = file_name_str.trimmed();

    add_entry_to_table(QString::number(0),
                       QString::number(8),
                       filename.toHex(),
                       file_name_str,
                       "File Name");
}

void fat_directory_entry_template::extract_file_extension()
{
    QByteArray file_extension = dir_entry_data.mid(8, 3);

    QString file_extension_str = QString::fromLocal8Bit(file_extension);

    file_extension_str = file_extension_str.trimmed();

    add_entry_to_table(QString::number(8),
                       QString::number(3),
                       file_extension.toHex(),
                       file_extension_str,
                       "File Extension");
}

void fat_directory_entry_template::extract_file_size()
{
    QByteArray filesize;// = dir_entry_data.mid(0x1c, 4);

    filesize.append(dir_entry_data.at(31));
    filesize.append(dir_entry_data.at(30));
    filesize.append(dir_entry_data.at(29));
    filesize.append(dir_entry_data.at(28));

    QString file_size_str = QString::number(filesize.toHex().toLongLong(0, 16));


    add_entry_to_table(QString::number(28),
                       QString::number(4),
                       filesize.toHex(),
                       file_size_str,
                       "File Size");

}




void fat_directory_entry_template::extract_attribute_byte()
{
    QByteArray attr_byte;// = dir_entry_data.mid(0x1c, 4);

    attr_byte.append(dir_entry_data.at(0xB));

    QString attr;
    if(attr_byte.at(0) & 0x01)
        attr.append("Read Only ");

    if(attr_byte.at(0) & 0x02)
        attr.append("Hidden ");

    if(attr_byte.at(0) & 0x04)
        attr.append("System ");

    if(attr_byte.at(0) & 0x08)
        attr.append("Volume Label ");

    if(attr_byte.at(0) & 0x10)
        attr.append("Directory ");

    if(attr_byte.at(0) & 0x20)
        attr.append("Archive On ");


    add_entry_to_table(QString::number(0x8),
                       QString::number(1),
                       attr_byte.toHex(),
                       attr,
                       "Attribute");
}
