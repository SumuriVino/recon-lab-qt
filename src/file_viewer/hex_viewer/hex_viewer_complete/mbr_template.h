#ifndef MBR_TEMPLATE_H
#define MBR_TEMPLATE_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QHeaderView>

namespace Ui {
class mbr_template;
}

class mbr_template : public QDialog
{
    Q_OBJECT

public:
    explicit mbr_template(QWidget *parent = 0);
    ~mbr_template();

    void pub_set_data(QByteArray ar);

private:
    Ui::mbr_template *ui;

    void create_ui();

    QTableWidget *tableWidget_partition1;
    QTableWidget *tableWidget_partition2;
    QTableWidget *tableWidget_partition3;
    QTableWidget *tableWidget_partition4;

    QList<QTableWidget *> table_list;

    QScrollArea *scrollArea;

    QByteArray mbr_data;

    void fill_partition_table(QTableWidget *table, QByteArray partition_data);

    QString partition_type_to_string(QByteArray ar);

};

#endif // MBR_TEMPLATE_H
