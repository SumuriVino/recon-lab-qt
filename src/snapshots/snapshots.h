#ifndef SNAPSHOTS_H
#define SNAPSHOTS_H

#include <QDialog>
#include <QtCore>
#include "message_box/file_open_cell_widget.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_static_functions.h"
                                   
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_helper_process.h"    

#include "thread_extract_snapshots.h"
#include "case_info/case_info_details.h"
#include "witness_info/witness_info_store/witness_info_store.h"

namespace Ui {
class snapshots;
}

class snapshots : public QDialog
{
    Q_OBJECT

public:
    explicit snapshots(QWidget *parent = 0);
    ~snapshots();

    void pub_set_source_count_name_and_source_db_path(QString source_cnt_nm);
    void pub_create_ui();

    bool pub_is_snapshot_thread_running();
    void pub_set_mutex_for_fs_main_db(QMutex *mutex);
    void pub_unmount_all_snapshots();
signals:

    void signal_add_snapshot_in_case_tree(QString);
    void signal_PBAR_snapshot_fs_extraction_started(QString job_type);
    void signal_PBAR_snapshot_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_snapshot_file_system_finished(QString job_type);
    void signal_go_to_snapshots_display(QString snapshot_db_name);


private slots:

    void on_pushButton_difference_clicked();
    void on_pushButton_cancel_clicked();
    void slot_thread_snapshot_custom_finished();
private:
    Ui::snapshots *ui;

    enum enum_snapshots_display
    {
        enum_snapshot_name_column = 0,
        enum_snapshot_processed_status_column,
        enum_source_count_name_column,
        enum_fs_run_status_column
    };

    QString selected_snapshot_name;
    QString source_count_name_str;
    QString main_source_db_path;
    QString snapshot_source_db_path;

    QString saved_snapshot_name;
    QString snapshots_difference_db_path;
    QCheckBox *checkbox_snapshot_obj;
    QStringList selected_snapshot_name_list;
    void mount_all_selected_snapshots();

    QString get_insert_command(QString tble_name, QStringList existing_colname_list);

    void create_table(QString tble_name , QString dest_db_path);
    void insert_deleted_data(QSqlDatabase &destination_db);
    void insert_created_data(QSqlDatabase &destination_db);
    void insert_modified_data(QSqlDatabase &destination_db);

    void start_snapshot_compare();

    void extract_and_save_snapshots_data();
    void insert_in_index_db(QString snapshot_display_name);
    void save_result_info_and_set_destinations();

    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;
    thread_extract_snapshots *thread_extract_snapshots_obj;
    QString difference_label_str;

    void mount_snapshot_on_difference_clicked(QString source_count_name, QString selected_snapshot_str, QString difference_name_str);

    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    bool bool_snapshots_thread_running = false;
    QString snapshot_db_filename;
    QMutex *mutex_for_fs_main_db;

    void create_and_update_snapshots_col_in_difference_db(QSqlDatabase &dest_db, QString table_name);
    witness_info_store *witness_info_store_obj;

};

#endif // SNAPSHOTS_H
