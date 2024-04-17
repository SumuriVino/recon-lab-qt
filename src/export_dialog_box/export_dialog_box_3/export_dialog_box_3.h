#ifndef EXPORT_DIALOG_BOX_3_H
#define EXPORT_DIALOG_BOX_3_H

#include <QDialog>
#include <QFileDialog>
#include "header_macro_generic.h"
#include <QMessageBox>
#include "recon_generic_functions/recon_static_functions.h"

namespace Ui {
class export_dialog_box_3;
}

class export_dialog_box_3 : public QDialog
{
    Q_OBJECT

public:
    explicit export_dialog_box_3(QWidget *parent = 0);
    ~export_dialog_box_3();

    void pub_set_window_title(QString w_title_name);

    void pub_set_export_as_type(QString type_str);
    void pub_set_file_name_and_file_path(QString file_name, QString output_dir_path);
private slots:

    void on_pushButton_export_clicked();

    void on_pushButton_select_export_path_clicked();

    void on_radioButton_current_dir_clicked();

    void on_radioButton_screen_items_clicked();

    void on_checkBox_recursive_clicked();

    void on_radioButton_screen_items_toggled(bool checked);

    void on_radioButton_current_dir_toggled(bool checked);

signals:

    void signal_start_write_data_as_csv_sqlite(QString dir_path , QString size_str,bool bool_recursive_checked , bool bool_current_dir_checked,QString info_file_path, QString export_as_type);

private:
    Ui::export_dialog_box_3 *ui;

    QString file_path;

    QFileDialog file_dialog_obj;
    QString export_as_type;

};

#endif // export_dialog_box_3_H
