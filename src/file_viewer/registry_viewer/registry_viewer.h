#ifndef REGISTRY_VIEWER_INTERFACE_H
#define REGISTRY_VIEWER_INTERFACE_H

#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"

#include "registry_parse.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      

class registry_viewer : public QWidget , public registry_parse
{
    Q_OBJECT

public:

    explicit registry_viewer(QWidget *parent = 0);
    registry_viewer();


    void pub_set_destination_db_path(QString db_path);
    void pub_set_case_tree_tab_name(QString tab_name);
    void pub_set_source_file_path(QString src_file_path);
    void pub_set_source_count_name(QString src_count_name);
    void pub_set_registry_destination_path(QString index_db_path);
    void pub_set_essentials();

    void pub_parse_windows_registry();

    void pub_set_recon_file_info(struct_global_recon_file_info obj);

private:


    QString source_count_name;
    QString os_nm_scheme_disp;

    QString destination_db_file;
    QString dest_index_db_path;
    QString registry_viewer_index_db_path;
    QString registry_source_file_path;
    QString actual_file_path;
    QString recon_file_from;
    QString recon_file_info_path;


    QString registry_plugin_record_no;
    QString registry_file_name;
    QString registry_plugin_name;
    QString display_tab_name;
    QString registry_plugin_category_name;
    QString initial_root_value_str;

    QString selected_registry_name_str ;
    QString node_path;
    QString node_name;
    QString file_path;


    void create_db_destination_file();

    void get_next_node(hive_h *hive_opened, hive_node_h node, QSqlDatabase destination_db, QString node_name_str, QString path_str);
    void insert_data_in_db(QSqlDatabase destination_db, QString sub_parent_path, size_t have_child_size_t , qint64 epch_time, QString child_node_name, QString arg_compelete_node_path_str);

    void registry_info_in_index_db(QString index_db_path);
    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;
    void set_essentials();

};

#endif // REGISTRY_VIEWER_INTERFACE_H
