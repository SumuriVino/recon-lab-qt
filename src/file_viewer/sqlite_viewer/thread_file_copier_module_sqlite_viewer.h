#ifndef THREAD_FILE_COPIER_module_sqlite_viewer_H
#define THREAD_FILE_COPIER_module_sqlite_viewer_H

#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "plugin_viewer/progress_window_module.h"
#include "header_global_structures.h"
#include "header_csv_template.h"

#include "ui_custom_classes/m_tablewidget.h"
                                   
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "header_report.h"
#include "header_global_variables_2.h"


class thread_file_copier_module_sqlite_viewer : public QThread
{
    Q_OBJECT
public:
    explicit thread_file_copier_module_sqlite_viewer(QWidget *parent = 0);
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

};


#endif // THREAD_FILE_COPIER_module_sqlite_viewer_H




