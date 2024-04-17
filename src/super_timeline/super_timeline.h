#ifndef SUPER_TIMELINE_H
#define SUPER_TIMELINE_H

#include <QDialog>
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_static_functions.h"
#include <QFileDialog>
#include <QMessageBox>
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "report/report_essentials/report_status.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include <QCalendarWidget>
#include <QApplication>

namespace Ui {
class super_timeline;
}

class super_timeline : public QDialog
{
    Q_OBJECT

public:
    explicit super_timeline(QWidget *parent = 0);
    ~super_timeline();

    void set_essentials();

    void closeEvent(QCloseEvent *);

signals:
    void signal_super_timeline_done(bool status);

private slots:
    void on_pushButton_change_file_path_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void slot_hide_loading_display_dialog_box(bool status);


    void on_radioButton_sqlite_toggled(bool checked);
    void on_radioButton_csv_toggled(bool checked);
    void on_dateTimeEdit_start_time_dateTimeChanged(const QDateTime &dateTime);
    void on_dateTimeEdit_end_time_dateTimeChanged(const QDateTime &dateTime);

private:
    Ui::super_timeline *ui;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    void loading_display_dialog_based_on_records(qint64 total_records, qint64 processed_count);
    bool bool_cancel_loading ;

    qint64 total_records_for_selected_timestamp;

    qint64 get_total_records_available_for_selected_timestmp();

    void save_timeline_results_in_db(QString filename, QString filepath, QSqlQuery &query);

    void save_timeline_results_in_csv(QString filename, QString filepath, QSqlQuery &query);

    QFileDialog file_dialog_obj;

    void both_record(QString filename, QString dir_path, qint64 start_epch, qint64 end_epch);
    void plugins_record(QString filename, QString dir_path, qint64 start_epch, qint64 end_epch);
    void fs_records(QString filename, QString dir_path, qint64 start_epch, qint64 end_epch);

    QString output_file_dir_path;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // SUPER_TIMELINE_H
