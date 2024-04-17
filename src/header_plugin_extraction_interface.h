#ifndef HEADER_PLUGIN_EXTRACTION_INTERFACE_H
#define HEADER_PLUGIN_EXTRACTION_INTERFACE_H

#include "header_global_structures.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "ios_backup/uid_backup/uid_backup_parser/ios_uid_backup_parser.h"




class header_plugin_extraction_interface : public QFrame
{
    Q_OBJECT

public:

    virtual void pub_set_engine_type(int engine_type){};


    virtual QString pub_get_artifact_name(){};

    virtual void pub_set_essentials(){};

    virtual void pub_set_source_count(int source_no){};
    virtual void pub_set_ios_uid_backup_pointer(ios_uid_backup_parser *ptr){};

    virtual void pub_set_desktop_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_desktop_info info_obj){};

    virtual void pub_set_general_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_general_info info_obj){};
    virtual void pub_set_source_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_source_info info_obj){};

    virtual void pub_set_essential_for_recon_lab_extraction(QStringList selected_table_list, QStringList total_tables_list){};

    virtual void pub_set_result_directory(QString output_path){};
    virtual void pub_create_db_tables(){};
    virtual void pub_set_clear_db(bool status){};

    virtual void pub_cancel_extraction(){};
    virtual void pub_find_source_file(){};
    virtual void pub_set_extended_attributes_list(QStringList ext_attr_list){};
    virtual void pub_extract_artifacts(){};
    virtual void pub_generate_chat_html_pages(){};
    virtual void pub_remove_temporary_asthetics(){};



    //===========================================================================//
    //===========================================================================//
    //===========================================================================//

    typedef void *(*entry_point_plugin_exraction)(void);

};




#endif //
