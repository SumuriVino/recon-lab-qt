#ifndef TASK_PROGRESS_BAR_H
#define TASK_PROGRESS_BAR_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include "header_plugin_extraction_interface.h"


namespace Ui {
class task_progress_bar;
}

class task_progress_bar : public QWidget
{
    Q_OBJECT

public:
    explicit task_progress_bar(QWidget *parent = 0);
    ~task_progress_bar();

    void set_title(QString title);

    void set_job_type_and_title(struct struct_global_task_type_and_title, QString icon_path);
    struct struct_global_task_type_and_title get_job_type_and_title();
    void pub_progressbar_set_value(int value);
    void pub_progressbar_set_value_for_plugins(int value);

    void common_set_extracted_records_in_progress(qint64 records, struct_global_task_type_and_title type_and_title_obj, bool bool_show_close_button, int progress, bool bool_infinite_process);
    void change_display_messge_in_progress_display(QString msg, bool bool_show_close_button, int max_size_of_progress);
    void set_next_task_with_same_parent_in_status_bar_for_fs(struct_global_task_type_and_title t1);
    void set_progress_maximum_bar_value(qint64 max_val);

    QString pub_get_current_feature_name_and_status();
    void pub_set_infinite_progress_bar();
    void pub_show_skip_label(bool status);

    void pub_set_progress_message(QString);

    void pub_set_icon_for_plugin(QString plugin_name);

    void pub_hide_cancel_button();
    void common_set_extracted_estimated_time(QString estimated_time);
signals:
    void signal_task_cancelled_requested(struct struct_global_task_type_and_title);
    void signal_skip_triggered();
private slots:
    void on_pushButton_close_clicked();

    void on_label_skip_linkActivated(const QString &link);

private:
    Ui::task_progress_bar *ui;

    struct struct_global_task_type_and_title type_and_title;
    bool bool_request_for_cancellation;

    QTimer *timer;

    void set_infinite_progress_bar();
};

#endif //
