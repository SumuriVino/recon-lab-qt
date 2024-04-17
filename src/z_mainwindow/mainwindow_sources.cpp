#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::update_narad_muni_paths_for_nested_sources()
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    bool source_nested = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Nested_bool).toBool();

    if(source_nested)
    {
        QString source_path_1 =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString(); //source_path 1
        QString source_path_2 =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString).toString(); //source_path 2
        bool source_decompressed = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Decompressed_bool).toBool();

        QString init_path;
        QStringList hrrchy_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Hierarchy_QString).toString().split(MACRO_RECON_Splitter_4_forward);
        if(hrrchy_list.size() > 1)
        {
            QString parent_cnt_name = hrrchy_list.at(hrrchy_list.size() - 1);
            struct_GLOBAL_witness_info_source prnt_struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(parent_cnt_name);
            init_path = prnt_struct_source_info.virtual_source_path;
        }

        if(source_decompressed)
        {
            init_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        }


        if(source_path_1.startsWith(init_path))
            source_path_1.remove(0, init_path.size());

        if(!source_path_1.isEmpty() && !source_path_1.startsWith("/"))
            source_path_1.prepend("/");

        if(source_path_2.startsWith(init_path))
            source_path_2.remove(0, init_path.size());

        if(!source_path_2.isEmpty() && !source_path_2.startsWith("/"))
            source_path_2.prepend("/");


        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_1_QString, source_path_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString, source_path_2);

    }

    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);

}

void MainWindow::prepare_sources_detail_in_narad_muni_1(QString src_type_intrnl, QString received_path, QString received_name)
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    QString source_names, virtual_source_path;
    QString os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display;
    QString os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal;

    if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
    {
        virtual_source_path = received_path;
        source_names = received_name;

        QString tmp_val = find_os_scheme_if_os_is_installed_into_source(virtual_source_path);
        if(!tmp_val.isEmpty())
        {
            QStringList tmp_list = tmp_val.split(MACRO_RECON_Splitter_5_resolution_scope);
            os_scheme_display = tmp_list.at(0);
            os_scheme_internal = tmp_list.at(1);
        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal)
    {
        virtual_source_path = received_path;
        source_names = received_name;

        QString tmp_val = find_os_scheme_if_os_is_installed_into_source(virtual_source_path);
        if(!tmp_val.isEmpty())
        {
            QStringList tmp_list = tmp_val.split(MACRO_RECON_Splitter_5_resolution_scope);
            os_scheme_display = tmp_list.at(0);
            os_scheme_internal = tmp_list.at(1);
        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        QString tmp_val = find_os_scheme_if_os_is_installed_into_source(virtual_source_path);
        if(!tmp_val.isEmpty())
        {
            QStringList tmp_list = tmp_val.split(MACRO_RECON_Splitter_5_resolution_scope);
            os_scheme_display = tmp_list.at(0);
            os_scheme_internal = tmp_list.at(1);
        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        QString tmp_val = find_os_scheme_if_os_is_installed_into_source(virtual_source_path);
        if(!tmp_val.isEmpty())
        {
            QStringList tmp_list = tmp_val.split(MACRO_RECON_Splitter_5_resolution_scope);
            os_scheme_display = tmp_list.at(0);
            os_scheme_internal = tmp_list.at(1);
        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
    {
        virtual_source_path = received_path;
        source_names = received_name;

        QString tmp_val = find_os_scheme_if_os_is_installed_into_source(virtual_source_path);

        if(!tmp_val.isEmpty())
        {
            QStringList tmp_list = tmp_val.split(MACRO_RECON_Splitter_5_resolution_scope);
            os_scheme_display = tmp_list.at(0);
            os_scheme_internal = tmp_list.at(1);
        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID;
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML;

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID;
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform;
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal;
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal;

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
    {
        virtual_source_path = received_path;
        source_names = received_name;

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal;
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal)
    {
        virtual_source_path = received_path;
        QFileInfo info(received_path);
        source_names = info.fileName();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Internal;
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
    {
        QFileInfo info(received_path);
        source_names = info.fileName();
        virtual_source_path = info.absolutePath();

        os_scheme_display = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display;
        os_scheme_internal = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal;
    }
    else
    {
        recon_static_functions::app_debug("---FAILED--- Unsupported source type " + src_type_intrnl, Q_FUNC_INFO);
    }

    if(!virtual_source_path.trimmed().isEmpty() && !virtual_source_path.endsWith("/"))
        virtual_source_path.append("/");


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Name_QString,source_names);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Virtual_Source_Path_QString,virtual_source_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Scheme_Display_QString,os_scheme_display);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString,os_scheme_internal);

    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);

}

void MainWindow::prepare_sources_detail_in_narad_muni_2()
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    QString os_schme_intrnl = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString).toString();

    if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
        set_r_case_essentials_macos();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
        set_r_case_essentials_ios_uid();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
        set_r_case_essentials_ios_xml();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform)
        set_r_case_essentials_ios_uniform();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
        set_r_case_essentials_winos();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal)
        set_r_case_essentials_androidos();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Internal)
        set_r_case_essentials_gtakeoutos();
    else if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal)
        set_r_case_essentials_otherOS();


    update_narad_muni_paths_for_nested_sources();
    witness_info_store_obj->pub_insert_source_info_in_db_and_text_file();
    witness_info_store_obj->pub_update_root_info_in_db_and_text_file();

    if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
    {
        // because source_count_name is settled in witness info store, when source table is creating
        QString src_cnt_nm = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
        map_ios_uid_backup_parser_obj->insert(src_cnt_nm,ios_uid_backup_parser_obj);
    }

    source_count_name_list_to_be_process << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();

    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);

}

void MainWindow::prepare_roots_detail(QList<struct_GLOBAL_witness_info_root> list_root_info)
{
    recon_static_functions::app_debug(" Root Guru Dronacharya - start ",Q_FUNC_INFO);

    //========================= start =========================//
    // sort Roots list
    // ios must be in starting postion
    // because if artifacts records need File System sqlite for domain name etc.

    QList<struct_GLOBAL_witness_info_root> list_root_info_ios_sorted;
    ///fill root iOS itunes sources first
    for(int zz = 0; zz < list_root_info.size(); zz++)
    {
        struct_GLOBAL_witness_info_root st_root_info1 = list_root_info.at(zz);

        if(st_root_info1.root_type == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup))
        {
            list_root_info_ios_sorted.append(st_root_info1);
        }
    }

    ///fill root iOS Cellebrite sources second
    for(int zz = 0; zz < list_root_info.size(); zz++)
    {
        struct_GLOBAL_witness_info_root st_root_info1 = list_root_info.at(zz);

        if(st_root_info1.root_type == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup))
        {
            list_root_info_ios_sorted.append(st_root_info1);
        }
    }

    ///fill non-iOS sources in same list
    for(int kk = 0; kk < list_root_info.size(); kk++)
    {
        struct_GLOBAL_witness_info_root st_root_info2 = list_root_info.at(kk);

        if((st_root_info2.root_type != QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_iTunesiOSBackup))
                && st_root_info2.root_type != QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_CellebriteiOSBackup))
        {
            list_root_info_ios_sorted.append(st_root_info2);
        }

    }
    list_root_info.clear();
    list_root_info = list_root_info_ios_sorted;
    //========================= end =========================//


    for(int ii = 0; ii < list_root_info.size(); ii++)
    {
        struct_GLOBAL_witness_info_root st_root_info3 = list_root_info.at(ii);
        QString root_path_1 = st_root_info3.root_path_1;

        if(root_path_1.isEmpty())
            continue;

        clear_root_narad_variables_all();

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Count_Name_QString,st_root_info3.root_count_name);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Evidence_Type_QString,st_root_info3.evidence_type);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Path_1_QString,st_root_info3.root_path_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Path_2_for_fusion_QString,st_root_info3.root_path_2_for_fusion);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString,st_root_info3.root_recovery_passcode_encrypted);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString,st_root_info3.root_evidence_password_encrypted);

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Name_QString,st_root_info3.root_name);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Type_QString,st_root_info3.root_type);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Type_Display_QString,st_root_info3.source_type_display);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Type_Internal_QString,st_root_info3.source_type_internal);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString,st_root_info3.home_directory_user_name);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Evidence_Number_QString,st_root_info3.evidence_number);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Description_QString,st_root_info3.description);

        witness_info_store_obj->pub_insert_root_info_in_db_and_text_file();
        root_count_name_list_to_be_process << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();

        QProcess proc;
        QStringList args;
        args << "-a" << "L" << root_path_1;
        proc.execute("SetFile", args);

        struct_GLOBAL_witness_info_source st_source_info = get_source_info_from_root_info(st_root_info3);
        prepare_sources_detail(st_source_info);

    }

    recon_static_functions::app_debug(" Root Guru Dronacharya - end ",Q_FUNC_INFO);
}

struct_GLOBAL_witness_info_source MainWindow::get_source_info_from_root_info(struct_GLOBAL_witness_info_root st_root_info)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    struct_GLOBAL_witness_info_source struct_source_info;
    recon_static_functions::clear_variables_witness_info_source_structure(struct_source_info);

    struct_source_info.root_count_name = st_root_info.root_count_name;
    struct_source_info.evidence_type = st_root_info.evidence_type;
    struct_source_info.root_name = st_root_info.root_name;
    struct_source_info.root_path_1 = st_root_info.root_path_1;
    struct_source_info.root_path_2_for_fusion = st_root_info.root_path_2_for_fusion;
    struct_source_info.root_type = st_root_info.root_type;
    struct_source_info.source_type_display = st_root_info.source_type_display;
    struct_source_info.source_type_internal = st_root_info.source_type_internal;
    struct_source_info.root_recovery_passcode_plain = st_root_info.root_recovery_passcode_plain;

    struct_source_info.source_evidence_password_plain = st_root_info.root_evidence_password_plain;
    struct_source_info.source_evidence_password_encrypted = st_root_info.root_evidence_password_encrypted;
    struct_source_info.source_evidence_password_modified_symbol_hex     = recon_static_functions::get_modified_password_replace_symbol_with_hex(struct_source_info.source_evidence_password_plain,Q_FUNC_INFO);

    struct_source_info.source_path_1 = st_root_info.root_path_1;
    struct_source_info.source_path_2_for_fusion = st_root_info.root_path_2_for_fusion;
    struct_source_info.source_main_path = st_root_info.root_path_1;
    struct_source_info.source_main_name = st_root_info.root_name;
    struct_source_info.bool_source_nested = false;
    struct_source_info.bool_source_decompressed = false;
    struct_source_info.source_hierarchy = struct_source_info.root_count_name;

    struct_global_system_account_info obj;
    recon_static_functions::clear_variables_system_account_info(obj);

    obj.system_user = st_root_info.home_directory_user_name;
    obj.domain_user = "";
    struct_source_info.system_account_QList.append(obj);


    struct_source_info.evidence_number = st_root_info.evidence_number;
    struct_source_info.description = st_root_info.description;

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

    return struct_source_info;
}

QList<struct_GLOBAL_witness_info_root> MainWindow::get_updated_root_structure_list(struct_global_image_mounter_image_info img_obj)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QList<struct_GLOBAL_witness_info_root> updated_root_info_list;

    QList<struct_GLOBAL_witness_info_root> recieved_list_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

    for(int jj = 0; jj < recieved_list_root_info.size(); jj++)
    {
        struct_GLOBAL_witness_info_root recieved_struct_root_info = recieved_list_root_info.at(jj);

        QString root_count_name = recieved_struct_root_info.root_count_name;
        QString root_source_path = recieved_struct_root_info.root_path_1;
        struct_global_image_mounter_image_info struct_image_mount_info = image_mounter_obj->pub_get_struct_image_info_by_image_path(root_source_path);

        struct_GLOBAL_witness_info_root update_struct_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_according_root_count_name(root_count_name);

        update_struct_root_info.root_disk_identifier_1 = struct_image_mount_info.disk_identifier_1;
        update_struct_root_info.root_disk_node_1 = struct_image_mount_info.disk_node_1;
        update_struct_root_info.root_disk_identifier_2_for_fusion = struct_image_mount_info.disk_identifier_2;
        update_struct_root_info.root_disk_node_2_for_fusion = struct_image_mount_info.disk_node_2;

        updated_root_info_list.append(update_struct_root_info);
    }


    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    return updated_root_info_list;
}

QStringList MainWindow::get_source_count_name_list_according_source_path_list(QStringList actual_path_list)
{
    QStringList list;

    for(int path = 0; path < actual_path_list.size(); path++)
    {
        QString actual_path = actual_path_list.at(path);
        QString source_count_name = "";

        QList<struct_GLOBAL_witness_info_source> list_witness_info_source = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

        bool bool_source_count_name_found = false;
        for(int i = 0; i < list_witness_info_source.size(); i++)
        {
            if(list_witness_info_source.at(i).source_path_1 == actual_path)
            {
                bool_source_count_name_found = true;
                source_count_name = list_witness_info_source.at(i).source_count_name;
                list << source_count_name;

                QStringList sub_source_count_name_list =  get_sub_sourcecountnamelist_according_sourcecountname(source_count_name);
                sub_source_count_name_list.removeDuplicates();
                sub_source_count_name_list.removeAll("");
                sub_source_count_name_list.sort();

                if(!sub_source_count_name_list.empty())
                    list << sub_source_count_name_list;
            }
        }


        if(!bool_source_count_name_found)
            list << source_count_name;

    }
    return list;
}

QStringList MainWindow::get_sub_sourcecountnamelist_according_sourcecountname(QString rcvd_source_count_name)
{
    QStringList list;

    bool bool_found = false;
    QList<struct_GLOBAL_witness_info_source> list_witness_info_source = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int i = 0; i < list_witness_info_source.size(); i++)
    {
        if(list_witness_info_source.at(i).parent_source_count_name.trimmed() == rcvd_source_count_name.trimmed())
        {
            bool_found = true;
            list << list_witness_info_source.at(i).source_count_name;
        }
    }

    return list;
}

void MainWindow::prepare_sources_detail(struct_GLOBAL_witness_info_source st_src_info)
{
    recon_static_functions::app_debug(" Source BHISHAM PITAMHA - start ",Q_FUNC_INFO);

    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(true);
    progress_bar_fixed_in_statusbar_obj->set_status_message("Please wait...");
    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(20);
    progress_bar_fixed_in_statusbar_obj->show();

    clear_root_narad_variables_partial();
    clear_source_narad_variables_all();

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Count_Name_QString,st_src_info.root_count_name);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RUNTIME_Root_Recovery_Passcode_Plain_QString, st_src_info.root_recovery_passcode_plain);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Hierarchy_QString, st_src_info.source_hierarchy);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Type_Display_QString,st_src_info.source_type_display);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Type_Internal_QString,st_src_info.source_type_internal);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_1_QString,st_src_info.source_path_1);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString,st_src_info.source_path_2_for_fusion);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Main_Path_QString,st_src_info.source_main_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Main_Name_QString,st_src_info.source_main_name);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Nested_bool , st_src_info.bool_source_nested);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Decompressed_bool , st_src_info.bool_source_decompressed);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString , st_src_info.source_evidence_password_encrypted);

    QString src_type_intrnl = st_src_info.source_type_internal;

    if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
            || src_type_intrnl ==  MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
    {
        set_device_mounting();

        struct_global_image_mounter_image_info struct_image_mount_info = image_mounter_obj->pub_get_struct_image_info_last_mounted();

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_1_QString,struct_image_mount_info.disk_node_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_1_QString,struct_image_mount_info.disk_identifier_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString,struct_image_mount_info.disk_node_2);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString,struct_image_mount_info.disk_identifier_2);

        QStringList source_name_duplicacy_list;

        for(int k = 0; k < struct_image_mount_info.list_struct_partition_info.size(); k++)
        {
            struct_global_image_mounter_partition_info struct_partition_info = struct_image_mount_info.list_struct_partition_info.at(k);

            if(struct_partition_info.mount_path.isEmpty())
                continue;

            clear_source_narad_variables_partial();
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Type_QString,struct_partition_info.volume_type);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,struct_partition_info.total_size_byte_numeric_QString);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,struct_partition_info.free_size_byte_numeric_QString);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,struct_partition_info.fs_type);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Offset_QString,struct_partition_info.volume_offset);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_UUID_QString,struct_partition_info.volume_UUID);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Role_Type_QString,struct_partition_info.role_type);

            if(struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive) || struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,struct_partition_info.derived_into_identifier);
            else
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,struct_partition_info.parent_identifier);


            uint64 total_size = struct_partition_info.total_size_byte_numeric_QString.toULongLong();
            uint64 free_size = struct_partition_info.free_size_byte_numeric_QString.toULongLong();
            uint64 used_size = total_size - free_size;

            QString used_space_str = QString::number(used_size);

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,used_space_str);

            QString src_name_str = get_source_name_from_mount_strct_info(struct_partition_info);

            if(source_name_duplicacy_list.contains(src_name_str))
            {
                src_name_str = recon_static_functions::get_available_stringname(src_name_str, source_name_duplicacy_list, Q_FUNC_INFO);
            }
            source_name_duplicacy_list << src_name_str;


            QString virtual_source_path = struct_partition_info.mount_path;

            prepare_sources_detail_in_narad_muni_1(src_type_intrnl, virtual_source_path, src_name_str);
            prepare_sources_detail_in_narad_muni_2();


            if(struct_partition_info.fs_type == MACRO_FILE_SYSTEM_TYPE_APFS)
            {
                QString m_vrtl_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();
                QStringList apfs_snapshots_list = get_snapshots_list_for_apfs(m_vrtl_path);

                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList, apfs_snapshots_list);

                witness_info_store_obj->pub_update_apfs_snapshots_info_in_db();
                witness_info_store_obj->pub_update_apfs_snapshots_info_in_plist_file();
            }

            map_key_src_cnt_name_val_raw_fuse_mnt_path.insert(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString(), struct_image_mount_info.osx_fuse_mount_point_1);

        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
    {
        set_device_mounting();

        struct_global_image_mounter_image_info struct_image_mount_info = image_mounter_obj->pub_get_struct_image_info_last_mounted();

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_1_QString,struct_image_mount_info.disk_node_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_1_QString,struct_image_mount_info.disk_identifier_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString,struct_image_mount_info.disk_node_2);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString,struct_image_mount_info.disk_identifier_2);

        QStringList source_name_duplicacy_list;

        for(int k = 0; k < struct_image_mount_info.list_struct_partition_info.size(); k++)
        {
            struct_global_image_mounter_partition_info struct_partition_info = struct_image_mount_info.list_struct_partition_info.at(k);

            if(struct_partition_info.mount_path.isEmpty())
                continue;

            QString mnt_rt_path = struct_partition_info.mount_path;
            struct_partition_info.mount_path = recon_helper_standard_obj->get_new_mount_path_of_time_machine_backup_image_and_snapshots(struct_partition_info.mount_path,QString(MACRO_Mount_Path_Time_Machine_Backup_Image_Main));

            if(struct_partition_info.mount_path.isEmpty())
                continue;

            clear_source_narad_variables_partial();
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Type_QString,struct_partition_info.volume_type);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,struct_partition_info.total_size_byte_numeric_QString);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,struct_partition_info.free_size_byte_numeric_QString);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,struct_partition_info.fs_type);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Offset_QString,struct_partition_info.volume_offset);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_UUID_QString,struct_partition_info.volume_UUID);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Role_Type_QString,struct_partition_info.role_type);

            if(struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive) || struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,struct_partition_info.derived_into_identifier);
            else
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,struct_partition_info.parent_identifier);


            uint64 total_size = struct_partition_info.total_size_byte_numeric_QString.toULongLong();
            uint64 free_size = struct_partition_info.free_size_byte_numeric_QString.toULongLong();
            uint64 used_size = total_size - free_size;

            QString used_space_str = QString::number(used_size);

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,used_space_str);

            QString src_name_str = get_source_name_from_mount_strct_info(struct_partition_info);

            if(source_name_duplicacy_list.contains(src_name_str))
            {
                src_name_str = recon_static_functions::get_available_stringname(src_name_str, source_name_duplicacy_list, Q_FUNC_INFO);
            }
            source_name_duplicacy_list << src_name_str;

            QString virtual_source_path = struct_partition_info.mount_path;

            prepare_sources_detail_in_narad_muni_1(src_type_intrnl, virtual_source_path, src_name_str);
            prepare_sources_detail_in_narad_muni_2();


            if(struct_partition_info.fs_type == MACRO_FILE_SYSTEM_TYPE_APFS)
            {
                QStringList apfs_snapshots_list = get_snapshots_list_for_apfs(mnt_rt_path);

                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList, apfs_snapshots_list);

                witness_info_store_obj->pub_update_apfs_snapshots_info_in_db();
                witness_info_store_obj->pub_update_apfs_snapshots_info_in_plist_file();
            }

            map_key_src_cnt_name_val_raw_fuse_mnt_path.insert(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString(), struct_image_mount_info.osx_fuse_mount_point_1);

        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
    {
        set_device_mounting();

        struct_global_image_mounter_image_info struct_image_mount_info = image_mounter_obj->pub_get_struct_image_info_last_mounted();

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_1_QString,struct_image_mount_info.disk_node_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_1_QString,struct_image_mount_info.disk_identifier_1);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString,struct_image_mount_info.disk_node_2);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString,struct_image_mount_info.disk_identifier_2);

        QStringList source_name_duplicacy_list;
        for(int k = 0; k < struct_image_mount_info.list_struct_partition_info.size(); k++)
        {
            struct_global_image_mounter_partition_info struct_partition_info = struct_image_mount_info.list_struct_partition_info.at(k);

            if(struct_partition_info.mount_path.isEmpty())
                continue;

            clear_source_narad_variables_partial();

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool,true);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Type_QString,struct_partition_info.volume_type);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,struct_partition_info.total_size_byte_numeric_QString);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,struct_partition_info.free_size_byte_numeric_QString);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,struct_partition_info.fs_type);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Offset_QString,struct_partition_info.volume_offset);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_UUID_QString,struct_partition_info.volume_UUID);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Role_Type_QString,struct_partition_info.role_type);

            if(struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive) || struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,struct_partition_info.derived_into_identifier);
            else
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,struct_partition_info.parent_identifier);


            uint64 total_size = struct_partition_info.total_size_byte_numeric_QString.toULongLong();
            uint64 free_size = struct_partition_info.free_size_byte_numeric_QString.toULongLong();
            uint64 used_size = total_size - free_size;

            QString used_space_str = QString::number(used_size);

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,used_space_str);

            QString src_name_str = get_source_name_from_mount_strct_info(struct_partition_info);

            if(source_name_duplicacy_list.contains(src_name_str))
            {
                src_name_str = recon_static_functions::get_available_stringname(src_name_str, source_name_duplicacy_list, Q_FUNC_INFO);
            }
            source_name_duplicacy_list << src_name_str;

            QString virtual_source_path = struct_partition_info.mount_path;

            prepare_sources_detail_in_narad_muni_1(src_type_intrnl, virtual_source_path, src_name_str);
            prepare_sources_detail_in_narad_muni_2();
        }
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
    {
        QString source_path = st_src_info.source_path_1;

        /// Get Mounted volume disk node
        QString mounted_disk_node = global_recon_helper_singular_class_obj->get_disk_node_of_mounted_path(source_path , Q_FUNC_INFO);


        QString disk_identif = mounted_disk_node;
        if(disk_identif.startsWith(QString("/dev/")))
            disk_identif.remove(0,QString("/dev/").size());


        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Node_1_QString, mounted_disk_node);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Disk_Identifier_1_QString, disk_identif);


        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Disk_Identifier_QString,disk_identif);


        /// Get Plist file path after running diskutil info -plist identifier_name > plist_file_path
        QString temp_plist_file_path = global_recon_helper_singular_class_obj->get_plist_file_path_after_running_diskutill_info(mounted_disk_node, global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), Q_FUNC_INFO);

        /// Parse Volume Total Size for Simple Node
        QString total_size = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_Number, "TotalSize", Q_FUNC_INFO);

        /// Parse Volume Free Space for Simple Node
        QString free_size = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_Number, "FreeSpace", Q_FUNC_INFO);

        QString  fs_type = global_recon_helper_singular_class_obj->get_file_system_of_any_node(mounted_disk_node, Q_FUNC_INFO);


        /// Parse Volume Type
        QString volume_type = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(temp_plist_file_path,enum_Global_Plist_File_Key_Type_String, "Content", Q_FUNC_INFO);

        /// Remove diskutil info plist file
        QFile temp_file(temp_plist_file_path);
        if(temp_file.exists())
            temp_file.remove();

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Volume_Type_QString,volume_type);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString,total_size);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString,free_size);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,fs_type);

        uint64 total_size_int = total_size.toULongLong();
        uint64 free_size_int = free_size.toULongLong();
        uint64 used_size_int = total_size_int - free_size_int;

        QString used_space_str = QString::number(used_size_int);

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString,used_space_str);

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,source_path, "");
        prepare_sources_detail_in_narad_muni_2();

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
    {
        QString source_path = st_src_info.source_path_1;
        if(!source_path.trimmed().isEmpty() && !source_path.endsWith("/"))
            source_path.append("/");

        QString backup_log_file_path = source_path + ".Backup.log";
        QString tm_checkpoint_file_path  = source_path + ".com.apple.TMCheckpoint";
        QString plist_file_path = source_path + ".exclusions.plist";
        if(QFileInfo(backup_log_file_path).exists() && QFileInfo(tm_checkpoint_file_path).exists() && QFileInfo(plist_file_path).exists())
        {
            QDir dir(source_path);
            QStringList internal_directory_list = dir.entryList();
            //This is for extract the partition directory path from the selected Timemachine Backup
            //If Selected Bakcup Directory :- 2020-07-07-160321
            for(int jj = 0 ; jj < internal_directory_list.size(); jj++)
            {
                QString partition_dir_name = internal_directory_list.at(jj);
                if(partition_dir_name.startsWith(".") || partition_dir_name.startsWith(".."))
                    continue;
                //Selected directory contain only one partition directory which we backup using time machine
                source_path = source_path + partition_dir_name;
            }
        }
        QString fs_type = MACRO_FILE_SYSTEM_TYPE_HFS;
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,fs_type);

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl, source_path, "");
        prepare_sources_detail_in_narad_muni_2();
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
    {
        QString source_path = st_src_info.source_path_1;

        QString fs_type = global_recon_helper_singular_class_obj->get_file_system_of_filepath(source_path, Q_FUNC_INFO);

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,fs_type);

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl, source_path, "");
        prepare_sources_detail_in_narad_muni_2();
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal)
    {
        QString source_path = st_src_info.source_path_1;

        //QString fs_type = global_recon_helper_singular_class_obj->get_file_system_of_filepath(source_path, Q_FUNC_INFO);
        QString fs_type = MACRO_FILE_SYSTEM_TYPE_HFS;

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool,true);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_File_System_Type_QString,fs_type);

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl, source_path, "");
        prepare_sources_detail_in_narad_muni_2();
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal
            || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal)
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
    {
        QString source_path = st_src_info.source_path_1;
        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,source_path, "");
        prepare_sources_detail_in_narad_muni_2();

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
    {
        QString source_path = st_src_info.source_path_1;

        QString bk_path = get_extracted_android_ab_file_backup_path().trimmed();
        if(bk_path.isEmpty() || (!QFile(bk_path + "/apps").exists())) // mean failure in backup
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to load Android Backup [ERROR-" + QString::number(enum_global_ERROR_1019_Extract_ADB_Android_AB_Failed) + "]");
            message_dialog_object->show();
            goto gotolast;
        }

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,bk_path, "");
        prepare_sources_detail_in_narad_muni_2();

        //================================== start ========================================//
        // saving extracted backup info in db,  when case will load, then no need to repoint .ab file,
        // no need to extract again .ab file
        // directly load bakup from this path
        QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();
        QString rslt_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString fileinfo_path = bk_path;
        fileinfo_path = fileinfo_path.remove(0, rslt_path.size()).trimmed();
        if(!fileinfo_path.startsWith("/"))
            fileinfo_path.prepend("/");

        QString src_cnt_nm = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
        QString db_path = bkup_location + "android_backup_index.sqlite";
        QString cmd = "insert into tbl_index (bookmark, source_count_name, recon_filefrom, recon_file_infopath) VALUES(?,?,?,?)";
        QStringList arg11;
        arg11 << "0" << src_cnt_nm << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result) << fileinfo_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,arg11 , db_path, Q_FUNC_INFO);

        //================================== end ========================================//

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
    {

        QString source_path = st_src_info.source_path_1;

        QString bk_path = get_extracted_cellebrite_ios_ufdr_backup_path();

        if(bk_path.isEmpty() || (!QFile(bk_path + "/report.xml").exists())) // mean failure in backup
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to load iOS Backup [ERROR-" + QString::number(enum_global_ERROR_1020_Extract_Cellebrite_iOS_UFDR_Failed) + "]");
            message_dialog_object->show();
            goto gotolast;
        }


        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,bk_path, "");
        prepare_sources_detail_in_narad_muni_2();

        //================================== start ========================================//
        // saving extracted backup info in db,  when case will load, then no need to repoint .ufdr file,
        // no need to extract again .ufdr file
        // directly load backup from this path

        QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
        QString rslt_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString fileinfo_path = bk_path;
        fileinfo_path = fileinfo_path.remove(0, rslt_path.size()).trimmed();
        if(!fileinfo_path.startsWith("/"))
            fileinfo_path.prepend("/");


        QString src_cnt_nm = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
        QString db_path = bkup_location + "ios_backup_index.sqlite";
        QString cmd = "insert into tbl_index (bookmark, source_count_name, recon_filefrom, recon_file_infopath) VALUES(?,?,?,?)";
        QStringList args_list;
        args_list << "0" << src_cnt_nm << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result) << fileinfo_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,args_list , db_path, Q_FUNC_INFO);

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
    {
        QString source_path = st_src_info.source_path_1;

        QString bk_path = get_extracted_cellebrite_ios_tar_backup_path();
        if(bk_path.isEmpty() || (!QFile(bk_path + "/Backup/Manifest.db").exists())) // mean failure in backup
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to load iOS Backup [ERROR-" + QString::number(enum_global_ERROR_1021_Extract_Cellebrite_iOS_TAR_Failed) + "]");
            message_dialog_object->show();
            goto gotolast;
        }

        // /Extracted_Content/iOS_Backup/Backup/Backup/f630fdc01a6e0cf8b5f111a1f2f98df128b5b23e
        // /Extracted_Content/iOS_Backup/1_Backup/Backup/a900fdc01a6e0cfdf5f111a1f2f98df128b5b96e
        // /Extracted_Content/iOS_Backup/Backup_Cellebrite_Tar/Backup/f630fdc01a6e0cf8b5f111a1f2f98df128b5b23e
        // /Extracted_Content/iOS_Backup/1_Backup_Cellebrite_Tar/Backup/a900fdc01a6e0cfdf5f111a1f2f98df128b5b96e


        // 1st Backup OR 1_Backup OR Backup_Cellebrite_Tar OR 1_Backup_Cellebrite_Tar dir is given by forenso
        // 2nd Backup dir is created by Cellebrite decompression automatically
        // So that we took 2nd Backup dir as a macro

        QString ios_backup_path = bk_path + "/" + MACRO_CELLEBRITE_iOS_BACKUP_TAR_EXTRACTED_DIR_NAME;

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,ios_backup_path, "");
        prepare_sources_detail_in_narad_muni_2();

        //================================== start ========================================//
        // saving extracted backup info in db,  when case will load, then no need to repoint .tar file,
        // no need to extract again .tar file
        // directly load backup from this path

        QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
        QString rslt_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString fileinfo_path = bk_path;
        fileinfo_path = fileinfo_path.remove(0, rslt_path.size()).trimmed();
        if(!fileinfo_path.startsWith("/"))
            fileinfo_path.prepend("/");


        QString src_cnt_nm = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
        QString db_path = bkup_location + "ios_backup_index.sqlite";
        QString cmd = "insert into tbl_index (bookmark, source_count_name, recon_filefrom, recon_file_infopath) VALUES(?,?,?,?)";
        QStringList args_list;
        args_list << "0" << src_cnt_nm << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result) << fileinfo_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,args_list , db_path, Q_FUNC_INFO);

        //================================== end ========================================//

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
    {
        QString bk_path = get_extracted_ios_graykey_backup_path();
        if(bk_path.isEmpty()) // mean failure in backup
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to load iOS Backup [ERROR-" + QString::number(enum_global_ERROR_1022_Extract_GrayKey_iOS_Backup_Failed) + "]");
            message_dialog_object->show();
            goto gotolast;
        }

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,bk_path, "");
        prepare_sources_detail_in_narad_muni_2();

        //================================== start ========================================//
        // saving extracted backup info in db,  when case will load, then no need to repoint .zip file,
        // no need to extract again .zip file
        // directly load backup from this path

        QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
        QString rslt_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString fileinfo_path = bk_path;
        fileinfo_path = fileinfo_path.remove(0, rslt_path.size()).trimmed();

        if(!fileinfo_path.startsWith("/"))
            fileinfo_path.prepend("/");

        QString src_cnt_nm = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
        QString db_path = bkup_location + "ios_backup_index.sqlite";
        QString cmd = "insert into tbl_index (bookmark, source_count_name, recon_filefrom, recon_file_infopath) VALUES(?,?,?,?)";
        QStringList args_list;
        args_list << "0" << src_cnt_nm << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result) << fileinfo_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,args_list , db_path, Q_FUNC_INFO);
        //================================== end ========================================//

    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
    {
        QString bk_path = get_extracted_android_graykey_backup_path();
        if(bk_path.isEmpty()) // mean failure in backup
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Failed to load android Backup [ERROR-" + QString::number(enum_global_ERROR_1023_Extract_GrayKey_Android_Backup_Failed) + "]");
            message_dialog_object->show();
            goto gotolast;
        }
        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,bk_path, "");
        prepare_sources_detail_in_narad_muni_2();

        //================================== start ========================================//
        // saving extracted backup info in db,  when case will load, then no need to repoint .zip file,
        // no need to extract again .zip file
        // directly load backup from this path

        QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();
        QString rslt_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString fileinfo_path = bk_path;
        fileinfo_path = fileinfo_path.remove(0, rslt_path.size()).trimmed();
        if(!fileinfo_path.startsWith("/"))
            fileinfo_path.prepend("/");

        QString src_cnt_nm = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
        QString db_path = bkup_location + "android_backup_index.sqlite";
        QString cmd = "insert into tbl_index (bookmark, source_count_name, recon_filefrom, recon_file_infopath) VALUES(?,?,?,?)";
        QStringList args_list;
        args_list << "0" << src_cnt_nm << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result) << fileinfo_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,args_list , db_path, Q_FUNC_INFO);
    }
    else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
    {
        QString source_path = st_src_info.source_path_1;

        QList <struct_global_system_account_info> list_sys_account_info = st_src_info.system_account_QList;
        QList<QVariant> qvariant_list;
        qvariant_list.append(QVariant::fromValue(list_sys_account_info));
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_System_Account_QList,qvariant_list);

        QString virtual_source_path = source_path;

        QStringList given_username_list = recon_static_functions::get_system_user_list_from_system_account_QList(st_src_info.system_account_QList , Q_FUNC_INFO);

        QFileInfo info1(source_path);
        if(info1.exists())
        {
            QString user;
            QStringList tmp_users_list = given_username_list;
            if(tmp_users_list.size() <= 0)
                user = "";
            else
                user = tmp_users_list.at(0);

            virtual_source_path = soft_mount(source_path,user);
        }

        QString source_names = info1.fileName();

        prepare_sources_detail_in_narad_muni_1(src_type_intrnl,virtual_source_path,source_names);
        prepare_sources_detail_in_narad_muni_2();
    }

gotolast:
    {
        //qDebug() << "lastttt111";
    }


    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(50);


    recon_static_functions::app_debug(" Source BHISHAM PITAMHA - End ",Q_FUNC_INFO);
}
