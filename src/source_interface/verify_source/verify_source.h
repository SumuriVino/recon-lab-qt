#ifndef VERIFY_SOURCE_H
#define VERIFY_SOURCE_H

#include<QtCore>
#include <QMessageBox>

#include "header_global_variables_1.h"
#include "header_global_variables_2.h"
#include "header_global_variables_3.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "language_translater/language_translator.h"
#include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_helper_process.h"


namespace Ui {
class verify_source;
}

class verify_source : public QDialog
{
    Q_OBJECT

public:
    explicit verify_source(QWidget *parent = 0);
    ~verify_source();

    void pub_set_essentials();
    void pub_set_source_count_name(QString src_cnt_nm);
    void pub_verify_source();
    bool pub_is_source_verified(QString src_cnt_nm);

signals:

    void signal_source_verifed(QString);
    void signal_source_verification_started(QString label);
    void signal_PBAR_source_verification_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress, bool bool_infinite_process);
    void signal_source_verification_finished(QString job_type);
    void signal_set_progress_bar_max_value(struct_global_task_type_and_title, int value);
    void signal_source_verification_running_status(bool,QString source_count_name_str);


private slots:
    void on_pushButton_show_clicked();
    void on_pushButton_reverify_clicked();
//    void slot_hide_loading_display_dialog_box(bool status);
    void slot_process_dd_finished(int);
    void slot_process_ewf_finished(int);
    void slot_process_dd_readyread();
    void slot_proc_ewf_verification_ready_read();
    void slot_cancel_verifying_source(struct_global_task_type_and_title t1);

private:
    Ui::verify_source *ui;


    bool bool_cancel_verifying;
    bool bool_process_finisehed;
    QString output_file_path;
    QString source_count_name_str;
    QString source_count_name_str_2;
    bool bool_show_details_already_verified_source;

    QString M_VAR_HASHES_QString_filename_QString;
    QString M_VAR_SSD_OR_PLATTER_QString_label_QString;///used only in fusion type source other wise it's value is blank


    void start_source_verification();
    void run_command(QString source_path);
    void run_dd_command(QString source_f_path);
    void run_ewf_command(QString source_f_path);
    void show_verification_details();
    QString find_image_type(QString file_path);



    QProcess *process_dd_verification;
    QProcess *process_ewf_verification;


    recon_helper_process *recon_helper_process_obj;
//    display_loading_progress_bar *display_loading_progress_bar_cancelable_obj;
    message_dialog *message_dialog_obj;

    QString root_source_name_str;
    QString root_type_str;
    QString root_count_name_str;
    QString speed_label_str;

    QString parse_data(QString data_str);
    void show_verification_details_2();
    QString display_data(QString current_src_name_str);


};

#endif // VERIFY_SOURCE_H
