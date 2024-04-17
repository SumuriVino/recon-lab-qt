#ifndef EXPORT_DIALOG_BOX_2_H
#define EXPORT_DIALOG_BOX_2_H

#include <QDialog>

#include <QFileDialog>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_report.h"


namespace Ui {
class export_dialog_box_2;
}

class export_dialog_box_2 : public QDialog
{
    Q_OBJECT

public:
    explicit export_dialog_box_2(QWidget *parent = 0);
    ~export_dialog_box_2();

    void pub_set_exported_file_name_and_dir_path(QString file_name , QString file_path);
    void pub_set_export_type(QString exp_typ_str);

    void closeEvent(QCloseEvent *);

signals:
    void signal_save_results_by_export_window(QString file_name, QString dir_path, QString export_type, QString report_scope);

    void signal_cancel_export_results();

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_select_path_clicked();

    void on_radioButton_screen_items_toggled(bool checked);

private:
    Ui::export_dialog_box_2 *ui;

    QFileDialog file_dialog_obj;

    QString export_type_str;

    QString report_scope_str;

};

#endif //
