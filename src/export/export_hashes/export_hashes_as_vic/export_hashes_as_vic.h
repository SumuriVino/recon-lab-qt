#ifndef EXPORT_HASHES_AS_VIC_H
#define EXPORT_HASHES_AS_VIC_H

#include <QObject>
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
class export_hashes_as_vic;
}

class export_hashes_as_vic : public QDialog
{
    Q_OBJECT
public:
    explicit export_hashes_as_vic(QWidget *parent = nullptr);

    ~export_hashes_as_vic();

    void pub_set_vic_version(QString str);
    void pub_set_vic_export_dir_path(QString str);
    void pub_set_export_file_details(QList<struct_global_export_as_hashes_details> &details_list);
    void pub_start_vic_export_general();
    void pub_start_vic_export_1_3();
    void pub_set_essential();
    void pub_start_vic_export_2_0();

signals:
    void signal_export_button_clicked();
public slots:

private slots:
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
    void on_pushButton_export_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_select_clicked();

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
        QString mime_type;
        QString source_type_internal;

        QString change_time;
        QString modify_time;
        QString access_time;
        QString create_time;
    };
    QList <struct_file_hashes> hashes_struct_list;


    struct struct_VIC_parse_hashes
    {
        QString vic_version;

        QString hash_md5;
        QString hash_sha1;
        QString media_id;
        QString media_size;
        QString name;
        QString odata_id;
    };
    QList <struct_VIC_parse_hashes> hashes_struct_VIC_parse_list;

    QJsonObject json_root_object;


    ///=========Agency Details=========///
    QString agency_name;
    QString agency_address;
    QString examiner_name;
    QString examiner_phone;
    QString examiner_email;

    ///=========Case Details=========///
    QString case_number;
    QString case_name;
    QString case_location;
    QString case_notes;


    QString application_name;
    QString application_version;


    QString vic_data_models_link;
    QString vic_extn;

    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    QString vic_version;
    QString vic_export_dir_filepath;
    QList<struct_global_export_as_hashes_details> export_as_vic_details_list;

    bool create_dir_path(QString dir_path);
    struct_file_hashes calculate_hashes(QString file_path);
    void write_data_in_json_format_according_to_version(QString version_str);
    void export_hashes_for_vic_version_1_1();
    void export_hashes_for_vic_version_1_2();
    void export_hashes_for_vic_version_1_3();
    void export_hashes_for_vic_version_2_0();
    Ui::export_hashes_as_vic *ui;
    QFileDialog file_dialog_obj;

    void prepare_essentials();

    recon_helper_standard *recon_helper_standard_obj;
    message_dialog *obj_message_dialog;
    export_rsync *obj_export_rsync;
    QString vic_json_file_path;
    struct_GLOBAL_witness_info_source struct_target_source_info;
    bool bool_message_box_yes_button_clicked;

    void point_to_export_dir(QString path);
};

#endif // EXPORT_HASHES_AS_VIC_H
