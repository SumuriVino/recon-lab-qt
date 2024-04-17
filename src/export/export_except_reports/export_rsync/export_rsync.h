#ifndef EXPORT_RSYNC_H
#define EXPORT_RSYNC_H

#include <QWidget>
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"

class export_rsync : public QObject
{
    Q_OBJECT

public:
    export_rsync(QObject *parent = 0);
    ~export_rsync();

    void pub_cancel_rsync_copy(QString caller_func);
    void set_rsync_copy_essentials(QString caller_func);
    QString pub_copy_file_using_rsync_for_export_as_folder(QString src_path, QString dest_location, QString displayed_file_name, QString caller_func);

    QString pub_get_source_path_to_export_fs_record_by_dbpath(struct_global_export_file_system_record obj, QString caller_func);
    QString pub_get_source_path_to_export_fs_record_by_dbreference(struct_global_export_file_system_record obj, QSqlDatabase &destination_db, QString caller_func);

    QString pub_copy_file_using_rsync(QString src_path, QString dest_path, int arg_status);

signals:

private slots:

    void slot_proc_rsync_finished(int);
private:

    QString remove_ios_dir_adhoc_path;
    bool bool_cancel_rsync_copy;
    QProcess *proc_rsync;
    bool bool_proc_rsync_finished;



    recon_helper_standard *recon_helper_standard_obj;
};

#endif
