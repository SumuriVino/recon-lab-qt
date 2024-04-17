#ifndef EXPORT_DIALOG_BOX_1_H
#define EXPORT_DIALOG_BOX_1_H

#include <QDialog>

#include <QFileDialog>
#include "recon_generic_functions/recon_static_functions.h"


namespace Ui {
class export_dialog_box_1;
}

class export_dialog_box_1 : public QDialog
{
    Q_OBJECT

public:
    explicit export_dialog_box_1(QWidget *parent = 0);
    ~export_dialog_box_1();

    void pub_set_name_and_dir_path(QString given_name , QString output_dir_path);
    void pub_set_export_type(QString exp_typ_str);

    void closeEvent(QCloseEvent *);

signals:
    void signal_save_results_by_export_window(QString file_name, QString dir_path, QString export_type);

    void signal_cancel_export_results();

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_select_path_clicked();

private:
    Ui::export_dialog_box_1 *ui;

    QFileDialog file_dialog_obj;

    QString export_type_str;

};

#endif //
