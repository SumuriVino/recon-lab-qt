#include "verify_source.h"
#include "ui_verify_source.h"

verify_source::verify_source(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::verify_source)
{
    ui->setupUi(this);


    setWindowModality(Qt::ApplicationModal);

    M_VAR_HASHES_QString_filename_QString = "hashes";

    message_dialog_obj = new message_dialog(this);
    recon_helper_process_obj = new recon_helper_process(this);

    bool_process_finisehed = false;
    bool_cancel_verifying = false;


    process_dd_verification = new QProcess(this);
    process_dd_verification->setProcessChannelMode(QProcess::MergedChannels);
    connect(process_dd_verification,SIGNAL(finished(int)),this,SLOT(slot_process_dd_finished(int)));



    process_ewf_verification = new QProcess(this);
    process_ewf_verification->setProcessChannelMode(QProcess::MergedChannels);
    connect(process_ewf_verification,SIGNAL(finished(int)),this,SLOT(slot_process_ewf_finished(int)));

    bool_show_details_already_verified_source = false;

}

verify_source::~verify_source()
{
    delete ui;
}

void verify_source::pub_set_essentials()
{
    bool_cancel_verifying = false;
    bool_process_finisehed = false;
    source_count_name_str.clear();
    output_file_path.clear();
    M_VAR_SSD_OR_PLATTER_QString_label_QString.clear();

    root_source_name_str.clear();
    root_count_name_str.clear();
    speed_label_str.clear();
    root_type_str.clear();
    bool_show_details_already_verified_source = false;

}

void verify_source::pub_set_source_count_name(QString src_cnt_nm)
{
    source_count_name_str = src_cnt_nm;
}


void verify_source::pub_verify_source()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
    QString root_src_type = struct_source_info.root_type;

    root_source_name_str = struct_source_info.root_name;
    root_count_name_str = struct_source_info.root_count_name;
    root_type_str = struct_source_info.root_type;

    output_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + struct_source_info.root_count_name + "_" + M_VAR_HASHES_QString_filename_QString + ".txt";
    if(QFileInfo(output_file_path).exists())
    {
        if(root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage
                || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage)
        {
            if(QFileInfo(struct_source_info.root_path_1).exists())
                ui->pushButton_reverify->setDisabled(false);
            else
                ui->pushButton_reverify->setDisabled(true);
        }
        else if(root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
        {
            if(QFileInfo(struct_source_info.root_path_1).exists()
                    && QFileInfo(struct_source_info.root_path_2_for_fusion).exists())
            {
                ui->pushButton_reverify->setDisabled(false);
            }
            else
            {
                ui->pushButton_reverify->setDisabled(true);
            }

        }

        QFile mfile(output_file_path);
        if(mfile.open(QIODevice::ReadOnly))
        {
            QString data_str = mfile.readAll();
            if(data_str.trimmed().isEmpty())
            {
                start_source_verification();
            }
            else
                this->show();

        }
    }
    else
    {
        start_source_verification();
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void verify_source::start_source_verification()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
    QString root_src_type = struct_source_info.root_type;

    bool bool_root_source_not_exist = false;
    if(root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage
            || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_TimeMachineBackupImage
            || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage
            || root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage)
    {
        QString source_path  = struct_source_info.root_path_1;
        if(QFileInfo(source_path).exists())
        {
            run_command(source_path);
        }
        else
            bool_root_source_not_exist = true;

    }
    else if(root_src_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
    {
        QString source_path_1 = struct_source_info.root_path_1;
        QString source_path_2 = struct_source_info.root_path_2_for_fusion;

        if(QFileInfo(source_path_1).exists() && QFileInfo(source_path_2).exists())
        {
            M_VAR_SSD_OR_PLATTER_QString_label_QString = "SSD  ";
            run_command(source_path_1);


            M_VAR_SSD_OR_PLATTER_QString_label_QString = "Platter  ";
            run_command(source_path_2);

        }
        else
            bool_root_source_not_exist = true;

    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, global_lang_tr_obj.get_translated_string("Source can not be verified"));
        message_dialog_obj->show();
        return ;
    }

    if(bool_root_source_not_exist)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, global_lang_tr_obj.get_translated_string("Source does not exist"));
        message_dialog_obj->show();
        return ;
    }

    show_verification_details();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void verify_source::run_command(QString source_path)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);


    bool_cancel_verifying = false;

    QString type_str = root_source_name_str;
    struct_global_task_type_and_title task_obj;
    task_obj.source_count_name = source_count_name_str;
    task_obj.title = MACRO_JobType_Verifying;
    task_obj.type = type_str;

    emit signal_set_progress_bar_max_value(task_obj , 100);
    emit signal_source_verification_started(type_str);
    emit signal_source_verification_running_status(true , source_count_name_str);


    QString image_type = recon_static_functions::get_forensic_image_type(source_path);
    if(image_type == MACRO_Target_Disk_ImageType_Apple
            || image_type == MACRO_Target_Disk_ImageType_RAW_FULL
            || image_type == MACRO_Target_Disk_ImageType_RAW_SPLIT
            || image_type == MACRO_Target_Disk_ImageType_AFF4)
    {
        run_dd_command(source_path);
    }
    else if(image_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL)
    {
        run_ewf_command(source_path);
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}
void verify_source::run_dd_command(QString source_f_path)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    bool_process_finisehed = false;

    /// ./dc3dd if=~/tmp/test_image.raw hlog=~/result_dir/Lab_Conf/Sources/root1_hashes.txt hash=md5 hash=sha1 bufsz=512k of=/dev/null

    QString executable_path = "./dc3dd";
    QString command = executable_path;
    if(!QFileInfo(executable_path).exists())
        return;


    connect(process_dd_verification,SIGNAL(readyRead()),this,SLOT(slot_process_dd_readyread()));

    QStringList arg_list;
    arg_list << "if=" + source_f_path << "hlog=" + output_file_path  << "hash=md5" << "hash=sha1" << "bufsz=512k" << "of=/dev/null";


    process_dd_verification->start(command,arg_list);
    if(!process_dd_verification->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + process_dd_verification->errorString() ,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int proc_cnt = 0;
    while(1)
    {
        proc_cnt++;
        if(proc_cnt % 500 == 0)
        {
            proc_cnt = 0;
            QCoreApplication::processEvents();
        }


        if(bool_cancel_verifying || bool_process_finisehed || process_dd_verification->state() == QProcess::NotRunning)
        {
            if(bool_cancel_verifying)
            {
                if(process_dd_verification->state() == QProcess::Running)
                    process_dd_verification->terminate();

                disconnect(process_dd_verification,SIGNAL(readyRead()),this,SLOT(slot_process_dd_readyread()));
            }

            break;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void verify_source::run_ewf_command(QString source_f_path)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    bool_process_finisehed = false;

    QString executable_path = QString("./ewfverify");
    QString command = executable_path;
    if(!QFileInfo(executable_path).exists())
        return ;


    ///./ewfverify -d md5,sha1 -l ~/result_dir/Lab_Conf/Sources/root1_hashes.txt ~/tmp/win10_pro_local_20Dec2016_E01.E01
    QStringList arg_list;
    arg_list << "-d"  << "md5,sha1" << "-l" <<  output_file_path  << source_f_path;

    connect(process_ewf_verification,SIGNAL(readyRead()),this,SLOT(slot_proc_ewf_verification_ready_read()));

    process_ewf_verification->start(command,arg_list);
    if(!process_ewf_verification->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + process_ewf_verification->errorString() ,Q_FUNC_INFO);
        return;
    }


    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int proc_cnt = 0;
    while(1)
    {
        proc_cnt++;
        if(proc_cnt % 500 == 0)
        {
            proc_cnt = 0;
            QCoreApplication::processEvents();
        }


        if(bool_cancel_verifying || bool_process_finisehed || process_ewf_verification->state() == QProcess::NotRunning)
        {
            if(bool_cancel_verifying)
            {
                if(process_ewf_verification->state() == QProcess::Running)
                    process_ewf_verification->terminate();

                disconnect(process_ewf_verification,SIGNAL(readyRead()),this,SLOT(slot_proc_ewf_verification_ready_read()));
            }

            break ;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void verify_source::show_verification_details()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString final_data = display_data(source_count_name_str);
    if(!final_data.trimmed().isEmpty())
        emit signal_source_verifed(final_data);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

    return;
}

bool verify_source::pub_is_source_verified(QString src_cnt_nm)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    source_count_name_str_2 = src_cnt_nm;

    bool bool_source_already_verified;
    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_nm);
    QString output_f_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + struct_source_info.root_count_name + "_" + M_VAR_HASHES_QString_filename_QString + ".txt";
    if(QFileInfo(output_f_path).exists() && (root_count_name_str != struct_source_info.root_count_name))
    {
        bool_source_already_verified = true;
        ui->pushButton_reverify->setDisabled(true);
        this->show();

        bool_show_details_already_verified_source = true;
    }
    else
    {
        bool_show_details_already_verified_source = false;
        bool_source_already_verified = false;
    }
    recon_static_functions::app_debug(" end --- source alread verified --- " + bool_source_already_verified ,Q_FUNC_INFO);

    return bool_source_already_verified;
}

void verify_source::show_verification_details_2()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString final_data = display_data(source_count_name_str_2);

    if(!final_data.trimmed().isEmpty())
        emit signal_source_verifed(final_data);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

QString verify_source::display_data(QString current_src_name_str)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString final_data;
    if(bool_cancel_verifying)
    {
        recon_static_functions::app_debug(" end",Q_FUNC_INFO);
        return final_data;
    }

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(current_src_name_str);
    QString output_f_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + struct_source_info.root_count_name + "_" + M_VAR_HASHES_QString_filename_QString + ".txt";
    if(!QFileInfo(output_f_path).exists())
        return final_data;

    QFile m_file(output_f_path);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open -----FAILED---- " + m_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + m_file.errorString(), Q_FUNC_INFO);
        return final_data;
    }
    QString data_arr = m_file.readAll();
    m_file.close();


    QString source_name_1, source_name_2;
    if(root_type_str == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
    {
        QString ssd_name = struct_source_info.root_name;

        QString platter_name = QFileInfo(struct_source_info.root_path_2_for_fusion).fileName();

        source_name_1 = "SSD:" + ssd_name +"\n";
        source_name_2 = "\n\nPlatter:"+ platter_name +":\n";
    }

    if(data_arr.contains("input results for file",Qt::CaseInsensitive))
    {
        QStringList data_list = data_arr.split("input results for file",Qt::SkipEmptyParts);

        if(data_list.size() > 0)
        {
            QString skip_line = data_list.at(0);
            if(skip_line.contains("command line:",Qt::CaseInsensitive))
            {
                data_list.removeAt(0);
            }
        }

        if(data_list.size() > 0)
        {
            final_data += source_name_1;
            final_data += parse_data(data_list.at(0));
        }
        if(data_list.size() > 1)
        {
            final_data += source_name_2;
            final_data += parse_data(data_list.at(1));
        }

    }
    else
        final_data = data_arr;

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
    return final_data;
}

QString verify_source::parse_data(QString data_str)
{
    QString final_data;
    QStringList data_list = data_str.split("\n",Qt::SkipEmptyParts);
    for(int ii = 0 ; ii < data_list.size() ; ii++)
    {
        QString final_str = data_list.at(ii);

        if(final_str.contains("command line:",Qt::CaseInsensitive))
            continue ;

        if(final_str.contains("md5",Qt::CaseInsensitive) || final_str.contains("sha1",Qt::CaseInsensitive))
        {
            final_data.append(final_str).append("\n");
        }

    }

    return final_data;
}



