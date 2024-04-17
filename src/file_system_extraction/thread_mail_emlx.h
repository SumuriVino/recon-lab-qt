#ifndef THREAD_MAIL_EMLX_H
#define THREAD_MAIL_EMLX_H

#include <QObject>
#include "header_global_structures.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
class thread_mail_emlx : public QObject
{
    Q_OBJECT
public:
    explicit thread_mail_emlx(QObject *parent = nullptr);
    ~thread_mail_emlx();
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_set_job_selected_source_count_name_list(QStringList m_list);
    void pub_set_recon_case_runner_type(QString type);
    void pub_set_mutex_for_mail_emlx_db(QMutex *mutex);
public slots:
    void slot_extract_emlx_files_data();
signals:
    void signal_PBAR_thread_file_system_started(QString job_type);
    void signal_PBAR_thread_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_file_system_finished(QString job_type);
    void signal_add_mail_emlx_files_to_case_tree(QStringList path_list);
    void signal_PBAR_thread_file_system_required_time(QString job_type, QString total_time);
    void finished();

private:

    struct struct_filesystem_record_and_filepath
    {
        QString filesystem_record;
        QString filepath;
        QString extension;
        QString is_dir;
        QString ios_filepath;

    };

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    QList<struct_filesystem_record_and_filepath> qlist_struct_fs_rec_and_path;


    qint64 progress_value;
    QMutex *mutex_for_fs_main_db;
    QMutex *mutex_for_email_parser_db;
    QString recon_case_runner_type;
    QStringList job_selected_source_count_name_list;
    recon_helper_standard *recon_helper_standard_obj;

    bool open_and_create_email_parser_db(QString email_parser_db_path);

    QSqlDatabase email_parser_db;
    QSqlDatabase file_system_db;
    bool open_fs_db(QString fs_db);

    qint64 parser_stop_index;

    QString emlx_from_str;
    QString emlx_subject_str;
    QString emlx_to_str;
    QString emlx_cc_str;
    QString emlx_date_str;
    QString emlx_message_str;
    QString elmx_body_msg_without_text_formatting;
    QString emlx_filepath_str;
    QString emlx_filepath;
    QString emlx_account_id_str;
    QString emlx_mailbox_str;
    QStringList emlx_attachment_list;
    QString destination_atchmnt_loc_emlx_files;
    bool bool_account_id = false;

    void emlx_start_parsing(QString filepath);
    void emlx_to(QByteArray *arr);
    void emlx_from(QByteArray *arr);
    void emlx_subject(QByteArray *arr);
    void emlx_cc(QByteArray *arr);
    void emlx_date(QByteArray *arr);
    void emlx_message_body(QByteArray *arr);
    void emlx_attachment(QByteArray *arr);
    void emlx_partial_attachments_decode();
    QString emlx_multiple(QByteArray *arr, qint64 end_index);
    bool check_for_multipart(QByteArray *arr);
    bool emlx_failsafe_text_encoded_check(qint64 text_start_index, QByteArray *arr);
    bool emlx_check_text_encoded(QByteArray *arr);
    int emlx_attachment_output(QString filename, QByteArray arr);
    QStringList email_attachment_filelist(QByteArray *arr);
    bool email_attachment_exists(QByteArray *arr);
    QString emlx_normalize_characters(QString data_str);
    QByteArray emlx_decode_base64(QByteArray data_arr);
    QStringList partial_attachment_files(QString filename);
    QString extract_non_emlx_attachments(QString filename, QString dir_path);
    QStringList get_index_of_mbox_from_path(QStringList filepath_list);

    QString get_account_id_str(QString filepath);
    QString emlx_account_id_from_sentmessage(QByteArray *arr);
    QMap<QString,QString> map_key_account_folder_path_value_account_id;
    void copy_data_from_tmp_mail_db_to_email_parser_db();

};

#endif // THREAD_MAIL_EMLX_H
