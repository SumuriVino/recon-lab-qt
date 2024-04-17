#ifndef FILE_EXPORT_DIALOG_H
#define FILE_EXPORT_DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include "recon_generic_functions/recon_static_functions.h"

namespace Ui {
class file_export_dialog;
}

class file_export_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit file_export_dialog(QWidget *parent = 0);
    ~file_export_dialog();

    void closeEvent(QCloseEvent *);

signals:
    void signal_export_file_dialog_work_done(bool bool_keep_directory_structure, QString export_dir_path);

    void signal_export_file_dialog_work_done_for_zip(QString export_zip_file_path, bool bool_is_enc_zip, QString enc_zip_password);

    void signal_export_file_dialog_work_done_for_dmg(bool bool_keep_directory_structure,QString export_dmg_file_path);

private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_export_clicked();

    void on_radioButton_export_to_zip_toggled(bool checked);

    void on_radioButton_export_to_folder_toggled(bool checked);

    void on_checkBox_encrypt_zip_clicked(bool checked);

    void slot_enc_zip_dialog_okay_clicked();
    void slot_enc_zip_dialog_cancel_clicked();
    void slot_pushButton_show_hide_password_clicked(bool checked);
    void slot_rejected();
    void on_radioButton_export_to_dmg_toggled(bool checked);

private:
    Ui::file_export_dialog *ui;

    QString dir_path;

    QFileDialog file_dialog_obj;


    void prepare_encrypted_zip_password_dialog_box();

    QDialog *dialog_enc_zip_password_asker;
    QPushButton *pushbutton_okay;
    QPushButton *pushbutton_cancel;
    QPushButton *pushbutton_password_show_hide_zip_enc;
    QLineEdit *lineedit_password_zip_enc;
    QLabel *label_status_zip_enc;

};

#endif // FILE_EXPORT_DIALOG_H
