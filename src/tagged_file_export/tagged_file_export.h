#ifndef TAGGED_FILE_EXPORT_H
#define TAGGED_FILE_EXPORT_H

#include <QDialog>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_report.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include <QTableWidgetItem>
#include "message_box/message_dialog.h"
#include "export/export_except_reports/export_zip/thread_zip_file_export.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"
#include "recon_case/recon_case.h"
#include "recon_generic_functions/recon_helper_process.h"

namespace Ui {
class tagged_file_export;
}

class tagged_file_export : public QDialog
{
    Q_OBJECT

public:
    explicit tagged_file_export(QWidget *parent = 0);
    ~tagged_file_export();

    void closeEvent(QEvent *event);
    void configure_ui();
    void pub_set_recon_case_obj(RECON_case *r_case_obj);

signals:

private slots:

    void on_pushButton_open_clicked();
    void on_pushButton_export_clicked();
    void on_radioButton_export_to_zip_toggled(bool checked);
    void on_radioButton_export_to_folder_toggled(bool checked);
    void slot_tags_horizontal_header_clicked(int index);
    void slot_thread_zip_file_export_finished();
    void slot_hide_loading_display_dialog_box(bool cancel_status);

    void on_checkBox_encrypt_zip_clicked(bool checked);

    void slot_enc_zip_dialog_okay_clicked();
    void slot_enc_zip_dialog_cancel_clicked();
    void slot_pushButton_show_hide_password_clicked(bool checked);
    void slot_rejected();
    void slot_message_box_yes_no_clicked(bool status);
    void on_radioButton_export_to_dmg_toggled(bool checked);

private:

    Ui::tagged_file_export *ui;


    bool bool_cancel_loading;

    QString result_dir_path;

    QStringList availbale_tags_from_config_db;
    QStringList prepared_tags_to_export_list;

    QString exported_zip_dest_file_path;
    QString export_dir_file_for_zip;
    QString exported_folder_dir_path;
    QString exported_dmg_path;
    struct struct_command_info
    {
        QString record_data_command;
        QString record_total_count_command;
        QStringList addbind_values_for_unreserved_tags_list;
    };

    struct struct_file_info_for_hashes
    {
        QString file_name;
        QString display_file_path;
        QString complete_file_path;
        QString examiner_notes_value;
    };


    struct_command_info get_command_info();
    void prepare_selected_tags_list();
    bool is_export_available_for_selceted_tags(qint64 total_record_count_qint64);
    void export_files_to_folder(bool bool_keep_directory_structure, QString export_file_dir);
    void export_files_to_zip(QString export_zip_file_path, QString export_dir_path);
    void export_files_to_dmg(bool bool_keep_directory_structure, QString export_file_dir,QString export_dmg_file_pth);

    qint64 get_record_counts_for_all_selected_tags(struct_command_info st_cmd_info);
    void write_exported_files_details_in_csv(QString source_count_name, QString cvs_dir_path);
    void make_csv_with_hashes(QList<struct_file_info_for_hashes> st_file_detail_in_csv, QString export_file_dir);

    void set_zip_file_path();


    message_dialog *message_dialog_obj;
    thread_zip_file_export *thread_zip_file_export_obj;
    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;

    export_rsync *export_rsync_obj;

    display_loading_progress_bar *display_loading_progress_bar_obj;

    QString M_VAR_export_dir_name_QString;
    QString zip_filename;

    recon_helper_process *recon_helper_process_obj;
    RECON_case *recon_case_obj;

    QDialog *dialog_enc_zip_password_asker;
    QPushButton *pushbutton_okay;
    QPushButton *pushbutton_cancel;
    QPushButton *pushbutton_password_show_hide_zip_enc;
    QLineEdit *lineedit_password_zip_enc;
    QLabel *label_status_zip_enc;

    void prepare_encrypted_zip_password_dialog_box();

    void point_to_export_dir(QString path);


};

#endif // FILE_SYSTEM_EXPORT_H
