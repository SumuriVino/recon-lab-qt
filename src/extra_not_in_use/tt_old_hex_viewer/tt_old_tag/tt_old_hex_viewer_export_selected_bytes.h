#ifndef HEX_VIEWER_EXPORT_SELECTED_BYTES_H
#define HEX_VIEWER_EXPORT_SELECTED_BYTES_H

#include <QDialog>
#include <QFileDialog>
#include <QDateTime>
#include "header_identical_1.h"
#include "header_global_variables_2.h"

namespace Ui {
class hex_viewer_export_selected_bytes;
}

class hex_viewer_export_selected_bytes : public QDialog
{
    Q_OBJECT

public:
    explicit hex_viewer_export_selected_bytes(QWidget *parent = 0);
    ~hex_viewer_export_selected_bytes();

    void pub_clear_line_edit_text();
    void pub_set_essentials();

private slots:

    void on_pushButton_open_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

signals:
    void signal_save_exported_file_click(QString,QString);

private:
    Ui::hex_viewer_export_selected_bytes *ui;

    QFileDialog file_dialog_obj;

};

#endif // HEX_VIEWER_EXPORT_SELECTED_BYTES_H
