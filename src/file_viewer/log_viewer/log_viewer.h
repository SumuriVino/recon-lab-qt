#ifndef LOG_VIEWER_H
#define LOG_VIEWER_H

#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

class log_viewer : public QObject
{
    Q_OBJECT
public:
    explicit log_viewer(QWidget *parent = 0);
    ~log_viewer();

    void pub_set_log_file_path(QString path);
    void pub_set_log_viewer_name(QString log_viewer_tbl_name);
    void pub_start_log_extraction();
    void pub_set_result_log_viewer_sqlite_path(QString name);

    void pub_set_source_count_name(QString name);
    void pub_set_essentials();
    void pub_set_recon_file_info(struct_global_recon_file_info obj);

private:

    int create_log_viewer_table();
    void entry_in_log_viewer_db_index_table();

    QString destination_log_file_path;
    QString destination_log_result_db_path;
    QString log_view_db_table_name;
    QString log_viewer_display_name;

    QString log_file_name;
    QString log_plugin_name;
    QString log_plugin_category_name;
    QString log_plugin_record_no;

    QString source_count_name;
    void set_source_file_path();
    QString source_file_path;
    QString source_info_relative_path;
    QString recon_file_from;
    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // LOG_VIEWER_H
