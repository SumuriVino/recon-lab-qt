#ifndef WITNESS_INFO_STORE_H
#define WITNESS_INFO_STORE_H

#include <QObject>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_2.h"
#include "header_plugin_extraction_interface.h"

class witness_info_store :public QObject
{

    Q_OBJECT

public:

    explicit witness_info_store(QWidget *parent = 0);
    ~witness_info_store();

    void pub_insert_root_info_in_db_and_text_file();
    void pub_update_root_info_in_db_and_text_file();


    void pub_insert_source_info_in_db_and_text_file();


    void pub_update_key_with_new_value_in_snapshots_plist(QString snapshot_name, QString source_count_name, QString key_str, QString value_str);

    void pub_update_apfs_snapshots_info_in_plist_file();

    void pub_update_apfs_snapshots_info_in_db();

    void pub_update_fs_run_status_snapshots_db(QString snapshot_name, QString source_count_name, QString value_str);

private:

    recon_helper_standard *recon_helper_standard_obj;

    void write_z_root_count_info_in_db();
    void write_z_root_count_info_in_text();
    void write_root_info_in_db();
    void write_root_info_in_text_file();

    void write_z_source_count_info_in_db();
    void write_z_source_count_info_in_text();
    void write_sources_info_in_db();

    void update_root_info_db_for_source_count_name();

    void update_root_info_text_file_for_source_count_name();

    void write_source_info_in_text_file();

    void insert_root_count_in_root_index_txt_file(QString root_count_file_name);

    void insert_source_count_name_in_source_index_txt_file(QString source_count_file_name);

    QString get_system_user_and_domain_user_str();

};

#endif // WITNESS_INFO_STORE_H
