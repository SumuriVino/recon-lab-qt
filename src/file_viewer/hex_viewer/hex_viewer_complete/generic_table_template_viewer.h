#ifndef GENERIC_TABLE_TEMPLATE_VIEWER_H
#define GENERIC_TABLE_TEMPLATE_VIEWER_H

#include <QDialog>
#include "common_header.h"

namespace Ui {
class generic_table_template_viewer;
}

class generic_table_template_viewer : public QDialog
{
    Q_OBJECT

public:
    explicit generic_table_template_viewer(QWidget *parent = 0);
    ~generic_table_template_viewer();

    void pub_set_table_data(QList<common_structure_for_data_table> list);
    void pub_set_title(QString);

private:
    Ui::generic_table_template_viewer *ui;
};

#endif // GENERIC_TABLE_TEMPLATE_VIEWER_H
