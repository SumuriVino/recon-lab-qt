#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::set_r_case_essentials_winos()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString virtual_source_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();


    QString db_output_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display) + "/";
    QString dir_name = recon_static_functions::get_available_filename("winSource",db_output_dir_path, Q_FUNC_INFO);
    db_output_dir_path = db_output_dir_path + dir_name + QString("/");
    QDir dir;
    dir.mkpath(db_output_dir_path);

    run_user_accounts_executable_for_winos(virtual_source_path, db_output_dir_path);
    run_system_info_executable_for_winos(virtual_source_path, db_output_dir_path);

    QString os_version = get_os_version_for_winos(db_output_dir_path);
    QString timezone_winos = get_timezone_for_winos(db_output_dir_path);

    QList <struct_global_system_account_info> list_sys_account_info = get_system_accounts_QList_for_winos(db_output_dir_path);
    QList<QVariant> qvariant_list;
    qvariant_list.append(QVariant::fromValue(list_sys_account_info));
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_System_Account_QList, qvariant_list);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Full_QString,os_version);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_OS_Version_Extended_QString,os_version);

    QString target_timezone = recon_helper_standard_obj->get_complete_timezone_name_label(timezone_winos,Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(target_timezone,Q_FUNC_INFO);
    timezone_winos = target_timezone;


    if(tmzone_offset == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,QString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,-1);
    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Readable_QString,timezone_winos);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64,tmzone_offset);
    }

    recon_static_functions::app_debug(" ENDS",Q_FUNC_INFO);
}

QString MainWindow::get_timezone_for_winos(QString essential_path)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QString timezone;

    QString system_info_db_path = essential_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + "System_Information/System_Information.sqlite";
    QString command = "SELECT time_zone_name FROM suspect_system_time_zone";
    timezone = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,system_info_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

    return timezone;
}

QList <struct_global_system_account_info> MainWindow::get_system_accounts_QList_for_winos(QString essential_dir_path)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QList <struct_global_system_account_info> sys_account_QList;
    sys_account_QList.clear();

    QString info_db_path = essential_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + "User_Accounts/User_Accounts.sqlite";

    QString command = "SELECT user_names FROM local_user_accounts";

    QStringList local_sys_usr_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,info_db_path,Q_FUNC_INFO);

    for(int pp = 0; pp < local_sys_usr_list.size(); pp++)
    {
        struct_global_system_account_info m_obj;
        recon_static_functions::clear_variables_system_account_info(m_obj);

        m_obj.system_user = local_sys_usr_list.at(pp);
        m_obj.domain_user = "";

        sys_account_QList.append(m_obj);

    }

    command.clear();
    command = "SELECT user_names, domain_user FROM domain_users_accounts";

    QStringList sys_usr_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,info_db_path,Q_FUNC_INFO);
    QStringList domain_usr_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,info_db_path,Q_FUNC_INFO);

    for(int pp = 0; pp < sys_usr_list.size(); pp++)
    {
        struct_global_system_account_info m_obj;
        recon_static_functions::clear_variables_system_account_info(m_obj);

        m_obj.system_user = sys_usr_list.at(pp);
        m_obj.domain_user = domain_usr_list.at(pp);

        sys_account_QList.append(m_obj);

    }


    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);

    return sys_account_QList;

}

QString MainWindow::get_os_version_for_winos(QString essential_dir_path)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QString os_versn;

    QString info_db_path = essential_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + "System_Information/System_Information.sqlite";
    QString command = "select value from system_information where key_name = 'OSVersion'";
    os_versn = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,info_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

    return os_versn;
}

void MainWindow::run_system_info_executable_for_winos(QString mount_path, QString essential_dir_path)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QStringList arg_list;
    arg_list << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
             << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
             << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_CALLER_ENGINE_TYPE_RECON_LAB)
             << mount_path
             << essential_dir_path;


    QString exec_path = "./winOS_system_information.app/Contents/MacOS/system_information";

    QFile file_check(exec_path);
    if(!file_check.exists())
    {
        recon_static_functions::app_debug("-----FAILED---- to find  Executable,it does not Exist " + exec_path, Q_FUNC_INFO);
        return;
    }

    run_command_generic(exec_path, arg_list);

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}


void MainWindow::run_user_accounts_executable_for_winos(QString mount_path, QString essential_dir_path)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);


    QStringList arg_list;
    arg_list << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
             << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
             << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_CALLER_ENGINE_TYPE_RECON_LAB)
             << mount_path
             << essential_dir_path;


    QString exec_path = "./winOS_user_accounts.app/Contents/MacOS/user_accounts";

    QFile file_check(exec_path);
    if(!file_check.exists())
    {
        recon_static_functions::app_debug("-----FAILED---- to find  Executable,it does not Exist " + exec_path, Q_FUNC_INFO);
        return;
    }

    run_command_generic(exec_path, arg_list);

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}
