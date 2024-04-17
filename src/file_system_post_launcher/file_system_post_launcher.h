#ifndef FILE_SYSTEM_POST_LAUNCHER_H
#define FILE_SYSTEM_POST_LAUNCHER_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"
#include <QtGui>
#include <QtCore>
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"
#include <QLabel>
#include "message_box/message_dialog.h"
#include "source_interface/verify_source/verify_source.h"
#include "QTextEdit"
#include "recon_case/recon_case.h"

namespace Ui {
class file_system_post_launcher;
}

class file_system_post_launcher : public QDialog
{
    Q_OBJECT

public:
    explicit file_system_post_launcher(QWidget *parent = 0);
    ~file_system_post_launcher();


    void pub_display_source_info();

    void pub_set_recon_case_obj(RECON_case *obj);
signals:
    void signal_file_system_post_launcher_job_list(struct_global_file_system_post_launcher_job_info);
    void signal_verifying_source_start(QString);
    void signal_PBAR_source_verifying_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_cancel_source_verfication(struct_global_task_type_and_title);
    void signal_source_verfication_finished(QString job_type);
    void signal_set_progress_bar_max_value(struct_global_task_type_and_title, int value);


private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_cancel_clicked();

    void slot_remove_source_link_activated(QString src_cnt_name);
    void slot_message_box_yes_no_recived(bool status);

    void slot_source_verifed(QString data_str);
    void slot_verify_source_link_activated(QString src_cnt_name);
    void slot_source_verification_running_status(bool status, QString source_count_name_str);


    void on_pushButton_refresh_clicked();

private:
    Ui::file_system_post_launcher *ui;

    enum enum_column_position {

        enum_column_position_Source_Count_Name,
        enum_column_position_Source_Name,
        enum_column_position_Apple_Metadata,
        enum_column_position_Exif_Metadata,
        enum_column_position_Signature_Analysis,
        enum_column_position_Hashes,
        enum_column_position_Faces_Analysis,
        enum_column_position_Optical_Character_Recognition,
        enum_column_position_Skin_Tone_Detection,
        enum_column_position_Weapons,
        enum_column_position_Fire_Analysis,
        enum_column_position_Unified_Logs,
        enum_column_position_Verify_Source,
        enum_column_position_Remove_Source,
        enum_column_position_Blank_Column
    };


    message_dialog *message_dialog_obj;

    QStringList to_remove_source_count_name_list;

    void remove_source_info_from_result_custom_paths_1();
    void remove_source_info_from_result_data();

    recon_helper_standard *recon_helper_standard_obj;

    void remove_source_info_from_apfs_snapshots();
    void remove_source_info_from_result_custom_paths_2();

    verify_source *verify_source_obj;

    bool bool_source_is_verifying;
    QString root_source_name_str;
    QString root_type_str;
    QString current_verifying_root_name_str;
    RECON_case *recon_case_obj;
    QString to_remove_root_count_name;

    void remove_root_info_from_root_db_and_text_files();
    void remove_source_info_from_source_db_and_text_files();
    void remove_ios_preview_cache_dir_from_result_on_source_remove(QString source_count_name);
};

#endif // FILE_SYSTEM_POST_LAUNCHER_H
