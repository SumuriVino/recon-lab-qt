#ifndef TASK_STATUS_WINDOW_H
#define TASK_STATUS_WINDOW_H

#include <QFrame>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QtCore>
#include <QTableWidgetItem>
#include "header_plugin_extraction_interface.h"


#include "task_progress_bar.h"
                                   
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"


namespace Ui {
class task_status_window;
}

class task_status_window : public QFrame
{
    Q_OBJECT

public:
    explicit task_status_window(QWidget *parent = 0);
    ~task_status_window();

    struct struct_task_progress_bar_info{
        task_progress_bar * task_progress_bar_obj;
        int row_position;
    };


    void set_fix_size(int w, int h);
    void add_progress_widget(task_progress_bar *widget_scrollArea);

    void show_keyword_search(QString search_name);

    void set_task_in_taskbar(QString name, QString type);

    void set_extracted_records_in_progress_for_artifacts_plugin(qint64 records);

    void pub_set_source_count_name(QString naming);

    void pub_change_display_message_in_task_progress_bar(QString task_type, QString current_plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress);


    void set_next_task_in_same_taskbar_for_fs(struct_global_task_type_and_title finished_task, struct_global_task_type_and_title next_task);
    void set_maximum_value_of_task_progress_bar_in_fs(QString source_count_name, int max_value);
    void pub_task_progressbar_set_value(int total_size_processed, QString source_count_name);

    void set_progressbar_value(int tabs, QString job_title);
    int get_running_tasks_count();
signals:
    void signal_cancel_plugin_extraction(struct struct_global_task_type_and_title);
    void signal_cancel_keyword_search(struct struct_global_task_type_and_title);
    void signal_current_item_for_statusbar(QString);
    void signal_cancel_file_search(struct struct_global_task_type_and_title);
    void signal_cancel_content_search(struct struct_global_task_type_and_title);
    void signal_cancel_signature_type_extraction(struct struct_global_task_type_and_title);
    void signal_PBAR_file_system_job_cancelled_requested(QString job_type);

    void signal_skip_triggered();
    void signal_cancel_carved_files_extraction(QString job_type);
    void signal_cancel_carved_data_extraction(QString job_type);
    void signal_cancel_disk_hex_view_unallocated_extraction(QString job_type);
    void signal_cancel_source_verification(struct struct_global_task_type_and_title);
    void signal_cancel_face_search(QString job_type);

public slots:
    void slot_remove_sub_process_status_window_object(struct struct_global_task_type_and_title);
    void slot_mainwindow_statusbar_button_clicked(bool checked);

    void slot_PBAR_file_system_started(QString job_type);
    void slot_PBAR_file_system_finished(QString job_type);
    void slot_PBAR_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress, bool bool_infinite_process);

    void slot_PBAR_plugin_extraction_status(QString plugin_name, qint64 record_count, QString current_count_and_total_plugins);


    void slot_PBAR_plugin_extraction_status_message(QString plugin_name, QString message);
    void slot_PBAR_file_system_required_time(QString job_type, QString estimated_time);

private slots:
    void slot_job_cancelled_requested_artifacts_plugin(struct struct_global_task_type_and_title);

    void slot_skip_triggered();
    void slot_PBAR_file_system_job_cancelled_requested(struct struct_global_task_type_and_title);


    void slot_set_progress_bar_max_value(struct struct_global_task_type_and_title type_and_title, int value);


    void slot_remove_hashset_status_window_object(struct_global_task_type_and_title type_and_title);

private:
    Ui::task_status_window *ui;

    QList<struct_task_progress_bar_info> status_task_progress_bar_list;

    QSemaphore *semaphore_sub_process_status_window;

    void common_remove_sub_process_status_window_object(struct struct_global_task_type_and_title type_and_title);

    bool bool_status_button_checked;
    QString currently_running_plugin_title_in_status_window;
QString currently_running_plugin_category_in_status_window;
    QString source_count_name;

    void remove_hashset_status_window_obj(struct_global_task_type_and_title type_and_title);

    void sub_mainwindow_statusbar_update();

    void refresh_task_bar();

    task_progress_bar *task_progress_bar_file_system;

};

#endif //
