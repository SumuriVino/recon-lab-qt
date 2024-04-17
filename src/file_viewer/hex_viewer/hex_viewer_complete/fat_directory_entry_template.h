#ifndef FAT_DIRECTORY_ENTRY_TEMPLATE_H
#define FAT_DIRECTORY_ENTRY_TEMPLATE_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class fat_directory_entry_template;
}

class fat_directory_entry_template : public QDialog
{
    Q_OBJECT

public:
    explicit fat_directory_entry_template(QWidget *parent = 0);
    ~fat_directory_entry_template();

    void pub_set_data(QByteArray);

private:
    Ui::fat_directory_entry_template *ui;

    QByteArray dir_entry_data;

    void add_entry_to_table(QString byteoffset, QString size, QString hex_value, QString value, QString desc);

    void extract_file_name();

    void extract_file_extension();

    void extract_file_size();

    void extract_attribute_byte();

};

#endif // FAT_DIRECTORY_ENTRY_TEMPLATE_H
