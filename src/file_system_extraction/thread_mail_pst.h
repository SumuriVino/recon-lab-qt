#ifndef THREAD_MAIL_PST_H
#define THREAD_MAIL_PST_H

#include <QObject>
#include <QtCore>
#include <QProcess>

#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_structures.h"

class thread_mail_pst : public QObject
{
    Q_OBJECT
public:

    explicit thread_mail_pst(QObject *parent = nullptr);
    ~thread_mail_pst();

    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void set_recon_case_runner_type(QString type);

    void pub_set_mutex_for_emlx_db(QMutex *mutex);
signals:

    void finished();
    void signal_add_pst_mail_to_case_tree(QStringList pst_mail_db_path);
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);

public slots:
    void slot_pst_mail_extraction();

private:

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    QStringList job_selected_source_count_name_list;

    bool bool_pst_mail_extraction_process_finished = false;

    QMutex *mutext_for_fs_main_db;
    QSqlDatabase  destination_db;

    qint64  total_records_num = 0;
    int rec_count = 0;

    QString recon_case_runner_type;
    QString source_name;
    QString source_type;

    recon_helper_standard *recon_helper_standard_obj;

    //==========pst_mail
    qint64 parser_stop_index;

    QString eml_from_str;
    QString eml_subject_str;
    QString eml_to_str;
    QString eml_cc_str;
    QString eml_date_str;
    QString eml_message_str;
    QString elm_body_msg_without_text_formatting;
    QString eml_filepath_str;
    QString eml_filepath;
    QString eml_account_id_str;
    QString eml_mailbox_str;
    QByteArray eml_file_bytes;
    QStringList eml_attachment_list;
    QString destination_atchmnt_loc_eml_files;

    void eml_start_parsing(QString filepath);
    void eml_extract_to(QByteArray *arr);
    void eml_extract_from(QByteArray *arr);
    void eml_extract_subject(QByteArray *arr);
    void eml_extract_cc(QByteArray *arr);
    void eml_extract_date(QByteArray *arr);
    void eml_extract_message_body(QByteArray *arr);
    void eml_extract_attachment(QByteArray *arr);
    QString eml_multiple(QByteArray *arr, qint64 end_index);
    bool check_for_multipart(QByteArray *arr);
    bool eml_failsafe_text_encoded_check(qint64 text_start_index, QByteArray *arr);
    bool eml_check_text_encoded(QByteArray *arr);
    int eml_attachment_output(QString filename, QByteArray arr);
    QStringList email_attachment_filelist(QByteArray *arr);
    bool email_attachment_exists(QByteArray *arr);
    QString eml_normalize_characters(QString data_str);
    QByteArray eml_decode_base64(QByteArray data_arr);

    QString pub_get_account_id_str(QString dir_path);
    bool bool_account_id = false;
    QString account_id_str;

    struct struct_mail_pst_file_details
    {
        QString record_no;
        QString filepath;
        QString file_name;
        QString source_count_name;
    };

    QList <struct_mail_pst_file_details> qlist_struct_pst_mail_file;

    QMutex *mutext_for_mail_pst_main_db;
    void copy_data_from_tmp_mail_db_to_email_parser_db();
};

#endif // THREAD_MAIL_PST_H
