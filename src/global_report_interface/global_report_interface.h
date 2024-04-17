#ifndef GLOBAL_REPORT_INTERFACE_H
#define GLOBAL_REPORT_INTERFACE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

                                   
#include "header_global_variables_1.h"
#include "header_global_structures.h"
#include "progress_bar/progress_bar_window.h"
#include "header_global_variables_2.h"

//--REPORT LAUNCHER.
#include "global_report_interface/report_launcher.h"
#include "header_report.h"

namespace Ui {
class global_report_interface;
}

class global_report_interface : public QWidget
{
    Q_OBJECT

public:
    explicit global_report_interface(QWidget *parent = 0);
    ~global_report_interface();

    void set_initialisation();
    void set_narad_muni_for_report(narad_muni *obj);

signals:
    void signal_global_report_plugin_selection_workdone(narad_muni *mobj, QStringList report_plugin_info_list, QString rpt_name, QStringList rpt_file_path_list,QStringList rpt_type_list, QString rpt_scope,QStringList selctd_tags_list);


private slots:
    void slot_all_report_selection_completed();
    void slot_set_window_title(QString title);

private:
    Ui::global_report_interface *ui;

    narad_muni narad_muni_for_report_obj;


    int index_report_type;
    int index_report_scope;
    QStringList list_selected_plugins;
    bool bool_export_available;

    void create_global_report_directory_and_path();
    bool bool_pushbutton_logo_enable;


    //--REPORT LAUNCHER.
    report_launcher *report_launcher_object;
    void report_generate_clicked();


    //--Report Changes
    QString report_name;
    QString repot_scope;
    QStringList report_type_list;
    QStringList selected_tags_list;
    QStringList report_file_path_list;

    QString report_agency_logo_path;
    QString report_encryption_password;



    void create_resource_and_files_dir(QString parent_dir_path, QString report_type);

    progress_bar_window *progress_bar_obj;

};

#endif // GLOBAL_REPORTS_H

