#ifndef GALLERY_ITEM_CELL_H
#define GALLERY_ITEM_CELL_H

#include <QWidget>
#include "header_global_structures.h"
#include "header_global_variables_2.h"




namespace Ui {
class gallery_item_cell;
}

class gallery_item_cell : public QWidget
{
    Q_OBJECT

public:
    explicit gallery_item_cell(QWidget *parent = 0);
    ~gallery_item_cell();


    void pub_set_file_info(struct_global_gallery_view_file_info f1);
    void pub_load_file_data();

    bool pub_is_2file_data_loaded();

    QString pub_file_path();
    void pub_set_bookmark(bool checked_status);

    struct_global_gallery_view_file_info pub_file_info();

protected:

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_checkBox_bookmark_clicked();

signals:
    void signal_bookmark_value_changed(struct_global_gallery_view_file_info f1);
    void signal_spacebar_pressed(struct_global_gallery_view_file_info f1);
    void signal_control_e_key_pressed(struct_global_gallery_view_file_info f1);
    void signal_control_m_key_pressed(struct_global_gallery_view_file_info f1);
    void signal_control_n_key_pressed(struct_global_gallery_view_file_info f1);


private:
    Ui::gallery_item_cell *ui;
    struct_global_gallery_view_file_info file_info;

    bool is_file_data_loaded = false;

    void custom_keypress_event(QKeyEvent *keyEvent);
};

#endif // GALLERY_ITEM_CELL_H
