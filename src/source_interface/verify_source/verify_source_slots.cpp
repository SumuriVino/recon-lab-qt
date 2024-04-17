#include "source_interface/verify_source/verify_source.h"


void verify_source::on_pushButton_show_clicked()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    this->hide();

    if(bool_show_details_already_verified_source)
    {
        ///note:-re-verify is disable if other source is running
        show_verification_details_2();
    }
    else
        show_verification_details();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void verify_source::on_pushButton_reverify_clicked()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    this->hide();

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
    output_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + struct_source_info.root_count_name + "_" + M_VAR_HASHES_QString_filename_QString + ".txt";
    if(QFileInfo(output_file_path).exists())
    {
        ///revome previous file to reverify the source
        if(recon_static_functions::remove_safely_blank_dir_OR_file(output_file_path, Q_FUNC_INFO))
        {
            start_source_verification();
        }
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void verify_source::slot_process_dd_finished(int)
{
    bool_process_finisehed = true;
    emit signal_source_verification_finished(MACRO_JobType_Verifying);
    emit signal_source_verification_running_status(false,source_count_name_str);
}


void verify_source::slot_process_ewf_finished(int)
{
    bool_process_finisehed = true;
    emit signal_source_verification_finished(MACRO_JobType_Verifying);
    emit signal_source_verification_running_status(false,source_count_name_str);
}

void verify_source::slot_process_dd_readyread()
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    ///1857994752 bytes ( 1.7 G ) copied ( 100% ),    8 s, 223 M/s
    QString line = process_dd_verification->readAll();

    QStringList comma_split = line.split(",");

    if(comma_split.size() < 3)
        return;


    QString progress_bar_label;
    QString m_speed = comma_split.at(2);
    m_speed = m_speed.trimmed();

    if(m_speed.contains("/s"))
    {
        progress_bar_label = m_speed;
    }

    QString a_first_part = comma_split.at(0);
    a_first_part = a_first_part.trimmed();

    QStringList a_first_path_1 = a_first_part.split("copied");


    if(a_first_path_1.size() < 2)
        return;

    QString progress = a_first_path_1.at(1);

    progress.remove("(");
    progress.remove(")");


    progress_bar_label;
    int prg_percent_int = progress.remove("%").toInt();

    QString speedlable = M_VAR_SSD_OR_PLATTER_QString_label_QString + progress_bar_label;
    if(prg_percent_int > 0)
        emit signal_PBAR_source_verification_value(MACRO_JobType_Verifying,speedlable,true,-1,prg_percent_int,false);


    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);
}


void verify_source::slot_proc_ewf_verification_ready_read()
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    ///Status: at 0.5%
    ///        verified 346 MiB (363495424 bytes) of total 59 GiB (64023257088 bytes)
    ///        completion in 23 minute(s) and 21 second(s) with 43 MiB/s (45438791 bytes/second)

    QString m_proc_data = process_ewf_verification->readAll();

    QStringList m_proc_data_list = m_proc_data.split("\n");

    int progross_percent = 0;

    for(int i = 0 ; i < m_proc_data_list.size(); i++)
    {
        QString line = m_proc_data_list.at(i);

        if(line.contains("Status:"))
        {
            QStringList percentage_list = line.split(" at ");

            if(percentage_list.size() > 1)
            {
                QString percentage_str = percentage_list.at(1);

                percentage_str = percentage_str.trimmed();
                int prg_percent_int = percentage_str.remove("%").toDouble();

                progross_percent = prg_percent_int;

            }
        }
    }

    for(int i = 0 ; i < m_proc_data_list.size(); i++)
    {
        QString line = m_proc_data_list.at(i);

        if(line.contains("completion"))
        {

            QStringList line_list_1 = line.split(" with ");

            if(line_list_1.size() > 1)
            {
                QString a_1 = line_list_1.at(1);

                QStringList a_1_list = a_1.split("(");

                if(a_1_list.size() > 1)
                {
                    QString speed = a_1_list.at(0);
                    speed = speed.trimmed();
                    speed_label_str = speed;

                }
            }

        }

    }

    QString speedlable = global_lang_tr_obj.get_translated_string(M_VAR_SSD_OR_PLATTER_QString_label_QString + speed_label_str);

    if(progross_percent > 0)
        emit signal_PBAR_source_verification_value(MACRO_JobType_Verifying,speedlable,true,-1,progross_percent,false);

    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);
}


void verify_source::slot_cancel_verifying_source(struct_global_task_type_and_title t1)
{
    recon_static_functions::app_debug("Start ",Q_FUNC_INFO);

    bool_cancel_verifying = true;
    if(QFileInfo(output_file_path).exists())
    {
        /// remove file(/Lab_Conf/Sources/root1_hashes.txt) on cancel source verification
        recon_static_functions::remove_safely_blank_dir_OR_file(output_file_path, Q_FUNC_INFO);
    }

    emit signal_source_verification_running_status(false,source_count_name_str);

    recon_static_functions::app_debug("End ",Q_FUNC_INFO);
}
