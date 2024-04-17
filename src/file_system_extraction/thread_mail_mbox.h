#ifndef THREAD_MAIL_MBOX_H
#define THREAD_MAIL_MBOX_H

#include <QObject>
#include "header_global_structures.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"

class thread_mail_mbox : public QObject
{
    Q_OBJECT
public:
    explicit thread_mail_mbox(QObject *parent = nullptr);
    ~thread_mail_mbox();
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void pub_set_recon_case_runner_type(QString type);
    void pub_set_mutex_for_mail_mbox_db(QMutex *mutex);

public slots:
    void slot_extract_mbox_files_data();

signals:
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_add_mail_mbox_files_to_case_tree(QStringList path_list);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);
    void finished();

private:

    struct struct_filesystem_record_and_filepath
    {
        QString filesystem_record;
        QString filepath;
        QString extension;
        QString is_dir;

    };

    enum Status {
        ENUM_MAIL_CHECK,
        ENUM_MAIL_PROCESS,
        ENUM_MAIL_END
    };

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    QList<struct_filesystem_record_and_filepath> qlist_struct_fs_rec_and_path;


    QMutex *mutex_for_fs_main_db;
    QMutex *mutex_for_email_parser_db;
    QString recon_case_runner_type;
    QStringList job_selected_source_count_name_list;
    recon_helper_standard *recon_helper_standard_obj;

    bool open_tmp_email_parser_db(QString email_parser_db_path);

    QSqlDatabase tmp_email_parser_db;
    QSqlDatabase file_system_db;
    bool open_fs_db(QString fs_db);

    qint64 parser_stop_index;
    QString mbox_from_str;
    QString mbox_subject_str;
    QString mbox_to_str;
    QString mbox_cc_str;
    QString mbox_date_str;
    QString mbox_message_str;
    QString mbox_body_msg_without_text_formatting;
    QString mbox_filepath_str;
    QString mbox_account_id_str;
    QString mbox_mailbox_str;
    QStringList mbox_attachment_list;
    qint64 records_count = 0;
    qint64 total_records_num;
    QString destination_atchmnt_path_feature;

    struct_GLOBAL_witness_info_source target_struct_obj;

    void mail_data_start_parsing_feature(QString data_str);
    void emlx_to(QByteArray *arr);
    void emlx_from(QByteArray *arr);
    void emlx_subject(QByteArray *arr);
    void emlx_cc(QByteArray *arr);
    void emlx_date(QByteArray *arr);
    void emlx_message_body(QByteArray *arr);
    QString emlx_multiple(QByteArray *arr, qint64 end_index);
    bool check_for_multipart(QByteArray *arr);
    bool emlx_failsafe_text_encoded_check(qint64 text_start_index, QByteArray *arr);
    bool emlx_check_text_encoded(QByteArray *arr);
    int emlx_attachment_output(QString filename, QByteArray arr, QString dest_dir_path);
    QStringList email_attachment_filelist(QByteArray *arr);
    bool email_attachment_exists(QByteArray *arr);
    QString emlx_normalize_characters(QString data_str);
    QByteArray emlx_decode_base64(QByteArray data_arr);
    void start_mbox_parsing_for_feature(QString mbox_file_path);
    bool check_text_encoded(QByteArray *arr);
    void emlx_attachment_feature(QByteArray *arr);
    void emlx_mbox(QByteArray *arr);
    void mbox_account_id_from_sentmessage(QByteArray *arr);
    void copy_data_from_tmp_mail_db_to_email_parser_db();
};

#endif // THREAD_MAIL_MBOX_H
