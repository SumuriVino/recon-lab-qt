#ifndef SQLITE_VIEWER_STARTER_H
#define SQLITE_VIEWER_STARTER_H

#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include<QWidget>
#include <QObject>
                                   
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


class sqlite_viewer_starter: public QObject
{
    Q_OBJECT
public:
    sqlite_viewer_starter(QWidget *parent);
    ~sqlite_viewer_starter();

    void pub_set_recon_result_dir_path(QString path);
    void pub_set_sqlite_path(QString path);
    void pub_set_sqlite_viewer_name(QString name);
    void pub_start_sqlite_extraction(QString destination_location_path);
    void pub_cancel_extraction();


    void pub_set_outsqlite_path(QString path);
    void pub_set_source_count_name(QString name);

    void pub_set_recon_file_info(struct_global_recon_file_info obj);
    void pub_set_essentials();


private:

    void insert_entry_in_index_table();
    void set_destination_location(QString path);
    void copy_and_recreate_source_file();

    QString recon_result_dir_path;
    QString destination_sqlite_path;
    QString destination_sqliteview_index_db_path;

    QString destination_result_directory_path;
    QString recreate_source_file;
    QString copied_source_file;

    QString sqlite_view_db_table_name;
    QString sqlite_viewer_display_name;

    QString sqlite_file_name;
    QString sqlite_plugin_name;
    QString sqlite_plugin_category_name;
    QString sqlite_plugin_record_no;

    bool bool_cancel_extraction;
    QString source_count_name;

    void set_source_file_essential();
    QString actual_source_file;
    QString recon_file_from;
    QString recon_file_info_path;
    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // SQLITE_VIEWER_STARTER_H
