#include "activity_logs_display.h"
#include "ui_activity_logs_display.h"

activity_logs_display::activity_logs_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::activity_logs_display)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);

    setWindowTitle(recon_static_functions::get_app_name() + " Activity Logs");

    ui->lineEdit_keyword_search->setAttribute(Qt::WA_MacShowFocusRect,false);
}

activity_logs_display::~activity_logs_display()
{
    delete ui;
}

void activity_logs_display::pub_set_destination_db_path(QString destination_db)
{
    destination_db_file = destination_db;
}

void activity_logs_display::pub_initialise_logs()
{
    display_logs_table();
}

void activity_logs_display::display_logs_table()
{
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error-  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_display_logs(destination_db);
    query_display_logs.prepare("SELECT INT,category,task,added_timestamp,start_timestamp,end_timestamp,time_taken,status,task_comments FROM activity_logs_entry");
    if(!query_display_logs.exec())
    {
        recon_static_functions::app_debug("insert query_display_logs ---FAILED--- " , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error  " + query_display_logs.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query  " + query_display_logs.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    int temp_counter = 0;
    while(ui->tableWidget_activity_logs_display->rowCount())
    {
        if(temp_counter % 200 == 0)
            QCoreApplication::processEvents();

        ui->tableWidget_activity_logs_display->removeRow(0);

        temp_counter++;
    }

    int count = 0;
    QTableWidgetItem *item_INT, *item_category, *item_task, *item_queued_timestamp, *item_start_timestamp, *item_end_timestamp, *item_time_take, *item_status, *item_task_comments;
    while(query_display_logs.next())
    {
        if(count % 200 == 0)
            QCoreApplication::processEvents();

        QString queued_time_str = QString::number(query_display_logs.value(enum_column_queued_time).toLongLong()/1000);
        QString start_time_str = QString::number(query_display_logs.value(enum_column_start_time).toLongLong()/1000);
        QString end_time_str = QString::number(query_display_logs.value(enum_column_end_time).toLongLong()/1000);
        qint64 time_taken = query_display_logs.value(enum_column_time_taken).toLongLong();

        if(query_display_logs.value(enum_column_status).toString() == MACRO_ACTIVITY_LOGS_TASK_ABORTED)
        {
            item_INT = new QTableWidgetItem(query_display_logs.value(enum_column_INT).toString());
            item_INT->setForeground(QColor(Qt::red));
            item_category = new QTableWidgetItem(query_display_logs.value(enum_column_category).toString());
            item_category->setForeground(QColor(Qt::red));

            item_task = new QTableWidgetItem(query_display_logs.value(enum_column_task_name).toString());
            item_task->setForeground(QColor(Qt::red));

            item_queued_timestamp = new QTableWidgetItem(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(queued_time_str,Q_FUNC_INFO));
            item_queued_timestamp->setForeground(QColor(Qt::red));

            item_start_timestamp = new QTableWidgetItem(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(start_time_str,Q_FUNC_INFO));
            item_start_timestamp->setForeground(QColor(Qt::red));

            item_end_timestamp = new QTableWidgetItem(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(end_time_str,Q_FUNC_INFO));
            item_end_timestamp->setForeground(QColor(Qt::red));

            item_time_take = new QTableWidgetItem(timeConversion(time_taken));
            item_time_take->setForeground(QColor(Qt::red));

            item_status = new QTableWidgetItem(query_display_logs.value(enum_column_status).toString());
            item_status->setForeground(QColor(Qt::red));

            item_task_comments = new QTableWidgetItem(query_display_logs.value(enum_column_commnets).toString());
            item_task_comments->setForeground(QColor(Qt::red));

        }
        else
        {
            item_INT = new QTableWidgetItem(query_display_logs.value(enum_column_INT).toString());
            item_category = new QTableWidgetItem(query_display_logs.value(enum_column_category).toString());
            item_task = new QTableWidgetItem(query_display_logs.value(enum_column_task_name).toString());
            item_queued_timestamp = new QTableWidgetItem(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(queued_time_str,Q_FUNC_INFO));
            item_start_timestamp = new QTableWidgetItem(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(start_time_str,Q_FUNC_INFO));
            item_end_timestamp = new QTableWidgetItem(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(end_time_str,Q_FUNC_INFO));
            item_time_take = new QTableWidgetItem(timeConversion(time_taken));
            item_status = new QTableWidgetItem(query_display_logs.value(enum_column_status).toString());
            item_task_comments = new QTableWidgetItem(query_display_logs.value(enum_column_commnets).toString());
        }

        ui->tableWidget_activity_logs_display->insertRow(count);
        ui->tableWidget_activity_logs_display->setRowHeight(count,22);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_INT,item_INT);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_category,item_category);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_task_name,item_task);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_queued_time,item_queued_timestamp);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_start_time,item_start_timestamp);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_end_time,item_end_timestamp);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_time_taken,item_time_take);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_status,item_status);
        ui->tableWidget_activity_logs_display->setItem(count,enum_column_commnets,item_task_comments);

        count++;
    }

    ui->tableWidget_activity_logs_display->hideColumn(enum_column_INT);
    ui->tableWidget_activity_logs_display->setColumnWidth(enum_column_task_name,180);
    ui->tableWidget_activity_logs_display->setColumnWidth(enum_column_category,220);
    ui->tableWidget_activity_logs_display->setColumnWidth(enum_column_queued_time,150);
    ui->tableWidget_activity_logs_display->setColumnWidth(enum_column_start_time,150);
    ui->tableWidget_activity_logs_display->setColumnWidth(enum_column_end_time,150);
    ui->tableWidget_activity_logs_display->setColumnWidth(enum_column_time_taken,100);
    ui->tableWidget_activity_logs_display->verticalHeader()->setFixedWidth(40);

    destination_db.close();
}

void activity_logs_display::on_lineEdit_keyword_search_returnPressed()
{
    QString search_keyword = ui->lineEdit_keyword_search->text().trimmed();

    for(int row = 0; row < ui->tableWidget_activity_logs_display->rowCount(); row++)
    {
        bool bool_hide_row = true;
        for(int col = 0; col < ui->tableWidget_activity_logs_display->columnCount(); col++)
        {
            if(ui->tableWidget_activity_logs_display->item(row,col)->text().toLower().contains(search_keyword.toLower()))
            {
                bool_hide_row = false;
                break;
            }
        }

        if(bool_hide_row)
            ui->tableWidget_activity_logs_display->hideRow(row);
        else
            ui->tableWidget_activity_logs_display->showRow(row);
    }
}

void activity_logs_display::on_lineEdit_keyword_search_textChanged(const QString &arg1)
{
    if(ui->lineEdit_keyword_search->text() == "")
    {
        for(int i = 0; i < ui->tableWidget_activity_logs_display->rowCount(); i++)
            ui->tableWidget_activity_logs_display->showRow(i);
    }
}

QString activity_logs_display::timeConversion(qint64 msecs)
{
    QString formattedTime;

    int hours = msecs/(1000*60*60);
    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;
//    int milliseconds = msecs-(seconds*1000)-(minutes*1000*60)-(hours*1000*60*60);

    formattedTime.append(QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0'))); // + ":" +
//                         QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0')));

    return formattedTime;

}
