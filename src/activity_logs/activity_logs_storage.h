#ifndef ACTIVITY_LOGS_STORAGE_H
#define ACTIVITY_LOGS_STORAGE_H

#include <QObject>
#include <QtCore>
#include <QSqlDatabase>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "header_global_variables_2.h"



class activity_logs_storage : public QObject
{
    Q_OBJECT
public:
    explicit activity_logs_storage(QObject *parent = nullptr);
    void pub_set_recon_result_directory_path(QString result_dir_path);
    QString pub_get_activity_logs_storage_db_path();

signals:

public slots:
    void slot_update_log_status(QString category, QString status_MACRO, QString taskname , qint64 current_date_time, QString comments);

private:

    QString activity_log_storage_db_path;
    QString recon_result_dir_path;

    void create_and_update_task_entry_in_database(QString category, QString status_MACRO, QString taskname, qint64 current_date_time, QString comments);
    qint64 get_int_record_from_db_reference(QString command, int pos, QSqlDatabase &destination_db, QString caller_func);

};

#endif //
