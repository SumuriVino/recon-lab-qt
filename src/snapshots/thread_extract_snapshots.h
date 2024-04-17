#ifndef THREAD_EXTRACT_SNAPSHOTS_H
#define THREAD_EXTRACT_SNAPSHOTS_H

#include <QObject>
#include <QtCore>
#include <QWidget>
#include <QThread>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

                                   
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"
#include "header_global_variables_2.h"


class thread_extract_snapshots : public QThread
{
    Q_OBJECT

public:
    thread_extract_snapshots(QWidget *parent = 0);
    ~thread_extract_snapshots();

    void run();

    void pub_set_source_count_name(QString str);
    void pub_set_selected_snapshot_name(QString snap_str, QString db_name);
    void pub_set_snapshot_thread_running_status(bool status);
    void pub_set_snapshot_mount_path(QString str);

signals:
    void signal_PBAR_thread_snapshot_fs_extraction_started(QString job_type);
    void signal_PBAR_thread_snapshot_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress,bool bool_progress_infinite_status);
    void signal_PBAR_thread_snapshot_file_system_finished(QString job_type);

    void signal_thread_custom_finished();

private slots:


private:

    bool bool_snapshot_thread_running_status = false;
    QString source_count_name;
    QString selected_snapshot_str;
    QString snapshot_fs_db_name;

    QString snapshot_index_db_path;
    QString snapshot_fs_db_path;
    QString snapshot_fs_dir_path;

    recon_helper_standard *recon_helper_standard_obj;
    struct stat fileStat;

    qint64 total_records_fs;
    struct_GLOBAL_witness_info_source struct_target_info_ptr;

    QString snapshot_mount_path;

    void set_essentials();
    void extract_filesystem_for_snapshot(QString snap_mount_path);
    void iterate_and_insert_snapshot_filesystem_data(QSqlDatabase &destination_db, QString snap_mount_path);
};

#endif // THREAD_EXTRACT_SNAPSHOTS_H
