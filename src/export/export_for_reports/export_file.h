#ifndef EXPORT_FILE_MODULE_H
#define EXPORT_FILE_MODULE_H


#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "plugin_viewer/progress_window_module.h"
#include "header_global_structures.h"
#include "header_csv_template.h"

#include "export/export_for_reports/thread_file_copier_module.h"
#include "ui_custom_classes/m_tablewidget.h"

#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "header_report.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"

class export_file_module: public QObject
{
    Q_OBJECT

public:
    explicit export_file_module(QObject *parent = 0);

    void export_file(QString report_scope, QString tab_name, QStringList selected_tags_list);

    void set_essentials();
    void set_export_entry_null();
    void set_narad_muni_for_report(narad_muni *mobj);

    void pub_cancel_export();
private slots:
    void slot_thread_file_copier_finished();
private:


    narad_muni *narad_muni_for_report_obj;

    QStringList file_name_list;

    QStringList file_copy_destination_list, file_copy_source_list,file_copy_recordno_list,file_copy_tablename_list,softlink_filepath_list;

    QStringList table_name_list_tablewidget;
    QList<QStringList>export_path_list_tablewidget;
    QString plugin_name;
    thread_file_copier_module *th_file_copier_module_popobj;
    QStringList destination_db_file_list;

    QString reports_dir_path;

    QList<QStringList>tw_data_type_list_tablewidget;
    int current_tab_index;

    QList<m_tablewidget *> list_m_tablewidgets;

    void fill_variable_from_narad_muni();


    void export_queried_files(QString command_select, QString table_name, QString db_path);


    bool bool_thread_copied_finished;

    recon_helper_standard *recon_helper_standard_obj;

    bool bool_cancel_export;
    export_rsync *export_rsync_obj;

    QString create_md5(QString file_path);
    QString create_sha1(QString file_path);

    bool bool_censored_image_clicked = false;
    bool bool_skin_tone_tag = false;

};

#endif // EXPORT_FILE_MODULE_H
