#include "activity_logs_storage.h"


activity_logs_storage::activity_logs_storage(QObject *parent)
    : QObject(parent)
{

}

void activity_logs_storage::pub_set_recon_result_directory_path(QString result_dir_path)
{
    recon_result_dir_path = result_dir_path;
}

QString activity_logs_storage::pub_get_activity_logs_storage_db_path()
{

    return activity_log_storage_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Activity_Log_In_Result_QString).toString()
            + "activity_logs.sqlite";
}


void activity_logs_storage::slot_update_log_status(QString category, QString status_MACRO, QString taskname, qint64 current_date_time, QString comments)
{
    create_and_update_task_entry_in_database(category,status_MACRO,taskname,current_date_time,comments);
}

void activity_logs_storage::create_and_update_task_entry_in_database(QString category, QString status_MACRO, QString taskname, qint64 current_date_time, QString comments)
{
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);

    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(pub_get_activity_logs_storage_db_path());
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    if(status_MACRO == MACRO_ACTIVITY_LOGS_TASK_QUEUED)
    {
        QSqlQuery query_task(destination_db);
        query_task.prepare("INSERT INTO activity_logs_entry(category,task,added_timestamp,status,task_comments) values(?,?,?,?,?)");

        query_task.addBindValue(category);
        query_task.addBindValue(taskname);
        query_task.addBindValue(current_date_time);
        query_task.addBindValue(MACRO_ACTIVITY_LOGS_TASK_QUEUED);
        query_task.addBindValue(comments);
        if(!query_task.exec())
        {
            recon_static_functions::app_debug("insert into activity_logs_entry ----FAILED-----",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query_task.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Executed Query " + query_task.executedQuery(),Q_FUNC_INFO);
            destination_db.close();
            return;
        }
    }

    if(status_MACRO == MACRO_ACTIVITY_LOGS_TASK_START)
    {
        QSqlQuery query_task(destination_db);
        query_task.prepare("UPDATE activity_logs_entry SET start_timestamp =?, status = ?, task_comments = ? WHERE task = ? AND end_timestamp IS NULL");

        query_task.addBindValue(current_date_time);
        query_task.addBindValue(MACRO_ACTIVITY_LOGS_TASK_START);
        query_task.addBindValue(comments);
        query_task.addBindValue(taskname);

        if(!query_task.exec())
        {
            recon_static_functions::app_debug("Update into activity_logs_entry 1----FAILED-----",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query_task.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Executed Query " + query_task.executedQuery(),Q_FUNC_INFO);
            destination_db.close();
            return;
        }
    }

    if(status_MACRO == MACRO_ACTIVITY_LOGS_TASK_COMPLETED)
    {
        QSqlQuery query_task_select(destination_db);
        query_task_select.prepare("SELECT added_timestamp, end_timestamp FROM activity_logs_entry WHERE task = ? AND end_timestamp IS NULL");
        query_task_select.addBindValue(taskname);
        if(!query_task_select.exec())
        {
            recon_static_functions::app_debug("Select into activity_logs_entry 2----FAILED-----",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query_task_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Executed Query " + query_task_select.executedQuery(),Q_FUNC_INFO);
            destination_db.close();
            return;
        }

        qint64 start_date = 0;
        QString end_date = "";
        if(query_task_select.next())
        {
            start_date = query_task_select.value(0).toLongLong();
            end_date = query_task_select.value(1).toString().trimmed();
        }

        if(end_date == "")
        {
            QSqlQuery query_task_update(destination_db);
            query_task_update.prepare("UPDATE activity_logs_entry SET end_timestamp =?, status = ?, task_comments = ?, time_taken = ? WHERE task = ? AND end_timestamp IS NULL");
            query_task_update.addBindValue(current_date_time);
            query_task_update.addBindValue(MACRO_ACTIVITY_LOGS_TASK_COMPLETED);
            query_task_update.addBindValue(comments);
            qint64 time_taken = (current_date_time - start_date);
            query_task_update.addBindValue(time_taken);
            query_task_update.addBindValue(taskname);

            if(!query_task_update.exec())
            {
                recon_static_functions::app_debug("Update into activity_logs_entry 2----FAILED-----",Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + query_task_update.lastError().text(),Q_FUNC_INFO);
                destination_db.close();
                return;
            }
        }
    }

    if(status_MACRO == MACRO_ACTIVITY_LOGS_TASK_ABORTED)
    {
        QString command =  "SELECT added_timestamp FROM activity_logs_entry WHERE task = '" + taskname + "'";
        qint64 first_date =  get_int_record_from_db_reference(command,0,destination_db,Q_FUNC_INFO);

        QSqlQuery query_task(destination_db);
        query_task.prepare("UPDATE activity_logs_entry SET end_timestamp = ?, status = ?, task_comments = ?, time_taken = ? WHERE task = ? AND end_timestamp IS NULL");
        query_task.addBindValue(current_date_time);
        query_task.addBindValue(MACRO_ACTIVITY_LOGS_TASK_ABORTED);
        query_task.addBindValue(comments);
        qint64 time_taken = (current_date_time - first_date);
        query_task.addBindValue(time_taken);
        query_task.addBindValue(taskname);

        if(!query_task.exec())
        {
            recon_static_functions::app_debug("Update into activity_logs_entry 2----FAILED-----",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query_task.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Executed Query " + query_task.executedQuery(),Q_FUNC_INFO);
            destination_db.close();
            return;
        }
    }

    destination_db.close();
}


qint64 activity_logs_storage::get_int_record_from_db_reference(QString command, int pos, QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    qint64 int_value = 0;

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" CALLER FUNCTION -  -  " + caller_func,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return int_value;
    }

    if(query.next())
    {
        int_value = query.value(pos).toLongLong();
    }

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
    return int_value;
}
