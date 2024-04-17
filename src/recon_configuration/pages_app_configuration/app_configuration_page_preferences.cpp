#include "app_configuration_page_preferences.h"
#include "ui_app_configuration_page_preferences.h"

app_configuration_page_preferences::app_configuration_page_preferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::app_configuration_page_preferences)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->checkBox_enable_debug_mode->setToolTip("In 'Debug Mode' processes might take more time than expected.");
}

app_configuration_page_preferences::~app_configuration_page_preferences()
{
    delete ui;
}


void app_configuration_page_preferences::update_databases(QString m_key , QString m_value)
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    QString select_cmd = QString("SELECT key from tbl_preferences");
    QStringList key_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd,0,destination_db_file,Q_FUNC_INFO);

    if(key_list.contains(m_key))
    {
        QString update_command = "UPDATE tbl_preferences SET value = ? WHERE key = ?";
        QStringList args_list;
        args_list << m_value << m_key;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command ,args_list ,destination_db_file ,Q_FUNC_INFO);
    }
    else
    {
        QString insert_command = "INSERT into tbl_preferences (key,value) values(?,?)";
        QStringList args_list;
        args_list << m_key << m_value;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_command ,args_list ,destination_db_file ,Q_FUNC_INFO);

    }

    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);

}

void app_configuration_page_preferences::pub_set_caller_type(QString caller_type)
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    caller_type_str = caller_type;

    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_App_Configuration_QString).toString() + QString("app_configuration.sqlite");

    display_data();

    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);

}


void app_configuration_page_preferences::display_data()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    QString command = "select value from tbl_preferences Where key = ?";

    QString m_enable_debug_mode_str =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_App_Preferences_Key_Enable_Debug_Mode),0,destination_db_file, Q_FUNC_INFO);
    if(m_enable_debug_mode_str == QString(MACRO_TRUE_VALUE_READABLE_QString))
    {
        ui->checkBox_enable_debug_mode->setCheckState(Qt::Checked);

    }
    else
    {
        ui->checkBox_enable_debug_mode->setCheckState(Qt::Unchecked);
    }

    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);

}


void app_configuration_page_preferences::on_checkBox_enable_debug_mode_clicked(bool checked)
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);


    if(checked)
    {
        update_databases(MACRO_App_Preferences_Key_Enable_Debug_Mode , QString(MACRO_TRUE_VALUE_READABLE_QString));

        global_variable_debug_mode_status_bool = true;
        global_variable_debug_print_in_file_bool = true;
        global_variable_debug_type_general_bool = true;

        recon_static_functions::create_debug_file();

        if(caller_type_str == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
        {
            struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
            recon_static_functions::print_examiner_info(pwd_obj, Q_FUNC_INFO);
            recon_static_functions::print_examiner_info_for_sumuri(pwd_obj, Q_FUNC_INFO);

        }
        else if(caller_type_str == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        {

            //=============== Root,Source and destination drive info could not available on splash screen configuraton Start=============//

            struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
            recon_static_functions::print_examiner_info(pwd_obj, Q_FUNC_INFO);
            recon_static_functions::print_examiner_info_for_sumuri(pwd_obj, Q_FUNC_INFO);

            struct_global_destination_drive_info destination_drive_info_obj = global_recon_helper_singular_class_obj->get_saved_destination_drive_info(Q_FUNC_INFO);
            recon_static_functions::print_destination_drive_info(destination_drive_info_obj, Q_FUNC_INFO);

            QList<struct_GLOBAL_witness_info_root> list_root_info_all = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

            QStringList rt_cnt_name_list_to_print;
            for(int pp = 0 ; pp < list_root_info_all.size() ; pp++)
            {
                rt_cnt_name_list_to_print << list_root_info_all.at(pp).root_count_name;
            }


            QList<struct_GLOBAL_witness_info_source> list_source_info_all = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
            QStringList src_cnt_name_list_to_print;
            for(int pp = 0 ; pp < list_source_info_all.size() ; pp++)
            {
                src_cnt_name_list_to_print << list_source_info_all.at(pp).source_count_name;
            }


            recon_static_functions::print_witness_root_info(list_root_info_all ,rt_cnt_name_list_to_print);
            recon_static_functions::print_witness_source_info(list_source_info_all , src_cnt_name_list_to_print);

        }

        //=============== Root,Source and Examiner info could not available on splash screen configuraton End=============//

    }
    else
    {
        update_databases(MACRO_App_Preferences_Key_Enable_Debug_Mode , QString(MACRO_FALSE_VALUE_READABLE_QString));
        global_variable_debug_mode_status_bool = false;
        global_variable_debug_print_in_file_bool = false;
        global_variable_debug_type_general_bool = false;
        recon_static_functions::close_debug_file();
    }

    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);

}
