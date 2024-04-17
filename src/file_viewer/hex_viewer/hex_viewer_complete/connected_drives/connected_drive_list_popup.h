#ifndef CONNECTED_DRIVE_LIST_POPUP_H
#define CONNECTED_DRIVE_LIST_POPUP_H

#include <QDialog>
#include "connected_drives.h"
#include "file_viewer/hex_viewer/hex_viewer_complete/common_header.h"

namespace Ui {
class connected_drive_list_popup;
}

class connected_drive_list_popup : public QDialog
{
    Q_OBJECT

public:
    explicit connected_drive_list_popup(QWidget *parent = nullptr);
    ~connected_drive_list_popup();

    void pub_extract_device_list();

signals:
    void signal_selected_device(struct_device_info);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_open_clicked();

private:
    Ui::connected_drive_list_popup *ui;
};

#endif // CONNECTED_DRIVE_LIST_POPUP_H
