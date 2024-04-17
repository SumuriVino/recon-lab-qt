#ifndef GLOBAL_RECON_FILE_INFO_H
#define GLOBAL_RECON_FILE_INFO_H

#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


class global_recon_file_info : public QObject
{
    Q_OBJECT
public:
    explicit global_recon_file_info(QObject *parent = nullptr);


    struct_global_recon_file_info pub_get_recon_info_for_file_system_by_dbreference(QString record_no , QSqlDatabase &dest_db,  QString source_count_name);
    struct_global_recon_file_info pub_get_recon_info_for_file_system_by_db_path(QString record_no , QString fs_db_path,  QString source_count_name);
    struct_global_recon_file_info pub_get_recon_info_for_snapshots_by_db_path(QString record_no, QString display_tab_name, QString fs_db_path, QString source_count_name);
    struct_global_recon_file_info pub_get_recon_info_for_carved_files_by_db_path(QString record_no , QString dest_db,  QString source_count_name);


    struct_global_recon_file_info pub_get_recon_info_for_artifacts_timeline_graphs(QString record_no);
    struct_global_recon_file_info pub_get_recon_info_for_screenshots(QString record_no);
    struct_global_recon_file_info pub_get_recon_info_for_saved_maps(QString record_no);

    struct_global_recon_file_info pub_get_recon_info_for_messenger_saved_graphs(QString record_no);
    struct_global_recon_file_info pub_get_recon_info_for_browser_histroy_saved_graphs(QString record_no);

    struct_global_recon_file_info pub_get_recon_info_for_string_viewer(QString record_n,QString source_count_name);
    struct_global_recon_file_info pub_get_recon_info_for_text_viewer(QString record_no,QString source_count_name);
    struct_global_recon_file_info pub_get_recon_info_for_hex_viewer(QString record_no, QString source_count_name);

    struct_global_recon_file_info pub_get_recon_info_for_artifacts_first_file_by_dbpath(QString plugin_name_str, QString tab_name_str , QString record_no, QString dest_db_path);
    struct_global_recon_file_info pub_get_recon_info_for_artifacts_first_file_by_dbreference(QString plugin_name_str, QString tab_name_str , QString record_no, QSqlDatabase &destination_db);

    QList<struct_global_recon_file_info> pub_get_recon_info_for_artifacts_all_file(QString plugin_name_str, QString tab_name_str, QString record_no);
    struct_global_recon_file_info pub_get_recon_info_for_artifacts_file_exist_any(QString plugin_name_str, QString tab_name_str , QString record_no);


signals:

public slots:

private:

    void clear_recon_file_st_variable(struct_global_recon_file_info &st_recn_f_info_obj);

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // GLOBAL_RECON_FILE_INFO_H
