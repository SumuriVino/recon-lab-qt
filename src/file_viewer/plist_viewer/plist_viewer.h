#ifndef PLIST_VIEWER_H
#define PLIST_VIEWER_H

#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

class plist_viewer : public QObject
{
    Q_OBJECT
public:
    explicit plist_viewer(QWidget *parent = 0);
    ~plist_viewer();

    void pub_set_plist_path(QString path);
    void pub_set_plist_viewer_name(QString name);
    void pub_start_plist_extraction();
    void pub_set_resultsqlite_path(QString name);

    void pub_set_source_count_name(QString name);

    void pub_set_essentials();

    void pub_set_recon_file_info(struct_global_recon_file_info obj);
private:
    void extract_selected_file();
    int xcode_extract_plist(QString source_file);
    void update_tree_for_measure_levels(QSqlQuery select_query, QSqlQuery insert_query);
    int create_destination_database();
    void entry_in_index_table();

    QString destination_plist_path;
    QString destination_plist_result_db_path;
    QString plist_view_db_table_name;
    QString plist_viewer_display_name;

    QString plist_file_name;
    QString plist_plugin_name;
    QString plist_plugin_category_name;
    QString plist_plugin_record_no;

    QString source_count_name;
    void set_source_file_path();
    QString source_file_path;
    QString source_info_relative_path;
    QString recon_file_from;
    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // PLIST_VIEWER_H
