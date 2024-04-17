#ifndef GALLERY_VIEWER_TABLE_DATA_LOADER_H
#define GALLERY_VIEWER_TABLE_DATA_LOADER_H
#include <QtCore>
#include <QWidget>
#include "header_global_structures.h"
#include "gallery_item_cell.h"
#include <QScrollBar>

class gallery_viewer_table_data_loader : public QObject
{
    Q_OBJECT
public:
    explicit gallery_viewer_table_data_loader(QObject *parent = nullptr);

    void pub_set_row_and_row_to_show(int m_row, int m_row_to_show);
    void pub_set_tablewidget(QTableWidget *table);

signals:
    void finished();
public slots:
    void load_data();

private:
    int starting_row, row_to_show;
    QTableWidget *tablewidget;
};

#endif // GALLERY_VIEWER_TABLE_DATA_LOADER_H

