#ifndef FAT_TEMPLATE_H
#define FAT_TEMPLATE_H

#include <QDialog>
#include <QtCore>

#include "file_viewer/hex_viewer/hex_viewer_complete/utilities_function.h"
#include "file_viewer/hex_viewer/hex_viewer_complete/common_header.h"

namespace Ui {
class FAT_Template;
}

class FAT_Template : public QDialog
{
    Q_OBJECT

public:
    explicit FAT_Template(QWidget *parent = nullptr);
    ~FAT_Template();

    void pub_set_fat_data(QByteArray);

    void pub_set_fat_type(int);

private:
    Ui::FAT_Template *ui;

    int fat_type;

    utilities_function utilities_obj;
};

#endif // FAT_TEMPLATE_H
