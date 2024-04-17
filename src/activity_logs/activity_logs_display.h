#ifndef ACTIVITY_LOGS_DISPLAY_H
#define ACTIVITY_LOGS_DISPLAY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtCore>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         

namespace Ui {
class activity_logs_display;
}

class activity_logs_display : public QWidget
{
    Q_OBJECT

public:
    explicit activity_logs_display(QWidget *parent = 0);
    ~activity_logs_display();

    void pub_set_destination_db_path(QString destination_db);
    void pub_initialise_logs();

private slots:
    void on_lineEdit_keyword_search_returnPressed();
    void on_lineEdit_keyword_search_textChanged(const QString &arg1);

private:
    Ui::activity_logs_display *ui;
    enum enum_activity_logs_display
    {
        enum_column_INT = 0,
        enum_column_category,
        enum_column_task_name,
        enum_column_queued_time,
        enum_column_start_time,
        enum_column_end_time,
        enum_column_time_taken,
        enum_column_status,
        enum_column_commnets
    };

    void display_logs_table();
    QString destination_db_file;

    QString timeConversion(qint64 msecs);
};

#endif //
