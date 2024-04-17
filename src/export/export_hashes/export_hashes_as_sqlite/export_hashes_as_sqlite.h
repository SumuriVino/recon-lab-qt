#ifndef EXPORT_HASHES_AS_SQLITE_H
#define EXPORT_HASHES_AS_SQLITE_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"
#include "header_global_variables_1.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include <QMessageBox>
#include "header_global_variables_2.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "message_box/message_dialog.h"
#include "message_box/file_open_cell_widget.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"

namespace Ui {
class export_hashes_as_sqlite;
}

class export_hashes_as_sqlite : public QDialog
{
    Q_OBJECT

public:
    explicit export_hashes_as_sqlite(QWidget *parent = nullptr);
    ~export_hashes_as_sqlite();

    void pub_start_export_hashes_as_sqlite();
    void pub_set_export_file_details(QList<struct_global_export_as_hashes_details> &details_list);
    void pub_set_essential();
private slots:
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
    void on_pushButton_export_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_select_clicked();

    void slot_hide_loading_display_dialog_box(bool status);
signals:

    void signal_export_button_clicked();


private:
    struct struct_file_hashes
    {
        QString filepath;

        QString display_filepath;
        QString display_filename;
        QString relative_filepath;
        QString md5_hash;
        QString sha1_hash;
        QString source_count;
        QString source_type_internal;

        QString change_time;
        QString modify_time;
        QString access_time;
        QString create_time;
    };

    QFileDialog file_dialog_obj;

    QList <struct_file_hashes> hashes_struct_list;
    export_rsync *obj_export_rsync;
    message_dialog *obj_message_dialog;
    bool bool_message_box_yes_button_clicked;
    m_tablewidget *file_system_tablewidget_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    QList<struct_global_export_as_hashes_details> export_as_sqlite_details_list;
    recon_helper_standard *recon_helper_standard_obj;
    struct_GLOBAL_witness_info_source struct_target_source_info;
    QString sqlite_export_dest_dirpath;
    QString destination_db_path;

    bool bool_cancel_loading = false;

    Ui::export_hashes_as_sqlite *ui;
    void point_to_export_dir(QString path);
    export_hashes_as_sqlite::struct_file_hashes calculate_hashes(QString file_path);
    void write_data_in_sqlite();
    bool create_tables_for_exported_sqlite();
};

#endif // EXPORT_HASHES_AS_SQLITE_H