#ifndef GALLERY_VIEWER_H
#define GALLERY_VIEWER_H

#include <QWidget>
#include "header_global_structures.h"
#include "gallery_item_cell.h"
#include <QScrollBar>
#include "gallery_viewer_table_data_loader.h"
                                   

namespace Ui {
class gallery_viewer;
}

class gallery_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit gallery_viewer(QWidget *parent = 0);
    ~gallery_viewer();

    void pub_set_file_list(QList<struct_global_gallery_view_file_info> *list);

    void pub_create_and_fill_empty_items();

    void pub_clear_gallery();

    void pub_set_message(QString str);

    void pub_clear_selection();
private slots:
    void slot_scroll_bar_value_changed(int val);
    void on_tableWidget_gallery_cellClicked(int row, int column);

    void slot_scroll_bar_slider_released();

    void slot_bookmark_value_changed(struct_global_gallery_view_file_info f1);

    void on_tableWidget_gallery_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void slot_set_bookmark_status_to_gallery_view(bool bookmark_status);

signals:
    void signal_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1);

    void signal_gallery_viewer_item_clicked(struct_global_gallery_view_file_info f1);

    void signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info f1);
    void signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1);
    void signal_gallery_view_control_n_key_pressed(struct_global_gallery_view_file_info f1);
    void signal_gallery_view_control_m_key_pressed(struct_global_gallery_view_file_info f1);

private:
    Ui::gallery_viewer *ui;
    QList<struct_global_gallery_view_file_info> *file_list;

    QScrollBar *bar;

    qint64 items_loaded_till_time = 0;

    QList<int> list_of_loaded_rows;
    qint64 slot_called_times = 0;

};

#endif //
