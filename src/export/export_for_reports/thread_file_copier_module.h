#ifndef THREAD_FILE_COPIER_module_H
#define THREAD_FILE_COPIER_module_H

#include <QThread>
#include <QtGui>
#include <CoreServices/CoreServices.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <QtSql>
#include "header_global_structures.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         
#include <recon_generic_functions/recon_helper_standard.h>
#include "export/export_except_reports/export_rsync/export_rsync.h"

class thread_file_copier_module : public QThread
{
    Q_OBJECT
public:
    explicit thread_file_copier_module(QObject *parent = 0);
    void run();
    void set_source_list(QStringList);
    void set_destination_list(QStringList);
    void set_tablename(QStringList);
    void set_record_no(QStringList);
    void set_softlink_filepath_list(QStringList);

    void set_destination_db(QString path);
    void pub_stop_thread();


signals:

public slots:

private:
    QStringList src_list,dst_list,rec_no_list,table_name_list;
    QStringList softlink_filepath_list;

    QString destination_db_file;
    bool stop_thread;

    bool bool_process_softlink_finish;
    recon_helper_standard *recon_helper_standard_obj;

    export_rsync *export_rsync_obj;
};


#endif // THREAD_FILE_COPIER_H




