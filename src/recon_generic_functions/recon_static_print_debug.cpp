#include "recon_static_functions.h"

void recon_static_functions::app_debug(QString data, QString caller_func)
{

    if(!global_variable_debug_mode_status_bool)
        return;

    if(!global_variable_debug_type_general_bool)
        return;


    if(caller_func.trimmed().isEmpty())
    {
        caller_func = "FUNCTION INFO IS EMPTY ---FAILED--- debug!!!";
    }
    else
    {
        QString tmp_func_name = caller_func;
        caller_func = caller_func.trimmed();
        caller_func.remove(caller_func.indexOf("(",0),caller_func.size());
        caller_func = caller_func.trimmed();
        caller_func.remove(0,caller_func.lastIndexOf(" "));
        caller_func = caller_func.trimmed();
        caller_func.remove("*");
        caller_func = caller_func.trimmed();

        if(caller_func.contains("(") || caller_func.contains(" "))
            caller_func = tmp_func_name;
    }

    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss.zzz");

    QString fnl_data = "[" + get_app_name() + "][" + date_time + "][Normal Debug]" + "["+ caller_func + "]:-- " +  data;


    if(global_variable_debug_print_on_terminal_bool)
    {
        //================When quoting is disabled, escape characters will not be printed on terminal like \n , \t=============//
        //================\n will be treated as a next line instead of just a text=============================================//
        qDebug().noquote() <<  fnl_data;
    }


    if(global_variable_debug_print_in_file_bool)
    {
        QString log_file_data = "[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "][Normal Debug]" + "["+ caller_func + "]:-- " +  data;
        fnl_data = log_file_data;

        if(global_variable_debug_encryption_status_bool)
        {
            QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
            debug_file_regular_ptr.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
            debug_file_regular_ptr.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
        }
        else
        {
            debug_file_regular_ptr.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
            debug_file_regular_ptr.write("\n");
        }


        debug_file_regular_ptr.flush();
    }


}

void recon_static_functions::debug_intensive(QString data, QString caller_func)
{

    if(!global_variable_debug_mode_status_bool)
        return;

    if(!global_variable_debug_type_intensive_bool)
        return;

    if(caller_func.trimmed().isEmpty())
    {
        caller_func = "FUNCTION INFO IS EMPTY ---FAILED--- debug!!!";
    }
    else
    {
        QString tmp_func_name = caller_func;
        caller_func = caller_func.trimmed();
        caller_func.remove(caller_func.indexOf("(",0),caller_func.size());
        caller_func = caller_func.trimmed();
        caller_func.remove(0,caller_func.lastIndexOf(" "));
        caller_func = caller_func.trimmed();
        caller_func.remove("*");
        caller_func = caller_func.trimmed();

        if(caller_func.contains("(") || caller_func.contains(" "))
            caller_func = tmp_func_name;
    }

    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");

    QString fnl_data = "[" + get_app_name() + "][" + date_time + "][Intensive Debug]" + "["+ caller_func + "]:-- " +  data;

    if(global_variable_debug_print_on_terminal_bool)
    {
        //================When quoting is disabled, escape characters will not be printed on terminal like \n , \t=============//
        //================\n will be treated as a next line instead of just a text=============================================//
        qDebug().noquote() <<  fnl_data;
    }


    if(global_variable_debug_print_in_file_bool)
    {

        QString log_file_data = "[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "][Intensive Debug]" + "["+ caller_func + "]:-- " +  data;
        fnl_data = log_file_data;

        if(global_variable_debug_encryption_status_bool)
        {
            QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
            debug_file_regular_ptr.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
            debug_file_regular_ptr.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
        }
        else
        {
            debug_file_regular_ptr.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
            debug_file_regular_ptr.write("\n");
        }

        debug_file_regular_ptr.flush();
    }

}

void recon_static_functions::debug_conditional(QString data, QString caller_func)
{

    if(!global_variable_debug_mode_status_bool)
        return;

    if(!global_variable_debug_type_conditional_bool)
        return;


    if(caller_func.trimmed().isEmpty())
    {
        caller_func = "FUNCTION INFO IS EMPTY ---FAILED--- debug!!!";
    }
    else
    {
        QString tmp_func_name = caller_func;
        caller_func = caller_func.trimmed();
        caller_func.remove(caller_func.indexOf("(",0),caller_func.size());
        caller_func = caller_func.trimmed();
        caller_func.remove(0,caller_func.lastIndexOf(" "));
        caller_func = caller_func.trimmed();
        caller_func.remove("*");
        caller_func = caller_func.trimmed();

        if(caller_func.contains("(") || caller_func.contains(" "))
            caller_func = tmp_func_name;
    }

    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");

    QString fnl_data = "[" + get_app_name() + "][" + date_time + "][Conditional Debug]" + "["+ caller_func + "]:-- " +  data;

    if(global_variable_debug_print_on_terminal_bool)
    {
        //================When quoting is disabled, escape characters will not be printed on terminal like \n , \t=============//
        //================\n will be treated as a next line instead of just a text=============================================//
        qDebug().noquote() <<  fnl_data;
    }

    if(global_variable_debug_print_in_file_bool)
    {
        QString log_file_data = "[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "][Conditional Debug]" + "["+ caller_func + "]:-- " +  data;
        fnl_data = log_file_data;

        if(global_variable_debug_encryption_status_bool)
        {
            QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
            debug_file_regular_ptr.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
            debug_file_regular_ptr.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
        }
        else
        {
            debug_file_regular_ptr.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
            debug_file_regular_ptr.write("\n");
        }

        debug_file_regular_ptr.flush();
    }

}

void recon_static_functions::print_image_mounting_info_on_terminal(QList<struct_global_image_mounter_image_info> list_image_info, QString caller_func)
{


    recon_static_functions::app_debug(prepare_callerfun(caller_func) + " START ", Q_FUNC_INFO);

    recon_static_functions::app_debug(("Image list size  " + QString::number(list_image_info.size())), Q_FUNC_INFO);

    recon_static_functions::app_debug("^^^^^^^^^^^^^^^^^^^^^^^^^-Image Info START-^^^^^^^^^^^^^^^^^^^^^^^^^", Q_FUNC_INFO);

    for(int tt = 0; tt < list_image_info.size(); tt++)
    {
        struct_global_image_mounter_image_info st_image_info_obj = list_image_info.at(tt);

        recon_static_functions::app_debug(("Image image_path_1+++++---" + st_image_info_obj.image_path_1), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image image_name_1+++++---" + st_image_info_obj.image_name_1), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image shadow_file_path_1+++++---" + st_image_info_obj.shadow_file_path_1), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image disk_node_1+++++---" + st_image_info_obj.disk_node_1), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image disk_identifier_1+++++---" + st_image_info_obj.disk_identifier_1), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image osx_fuse_mount_point_1+++++---" + st_image_info_obj.osx_fuse_mount_point_1), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image image_path_2+++++---" + st_image_info_obj.image_path_2), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image image_name_2+++++---" + st_image_info_obj.image_name_2), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image shadow_file_path_2+++++---" + st_image_info_obj.shadow_file_path_2), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image disk_node_2+++++---" + st_image_info_obj.disk_node_2), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image disk_identifier_2+++++---" + st_image_info_obj.disk_identifier_2), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image osx_fuse_mount_point_2+++++---" + st_image_info_obj.osx_fuse_mount_point_2), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image image_format_type+++++---" + st_image_info_obj.image_format_type), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image password+++++---" + st_image_info_obj.password), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image image_source_type_display+++++---" + st_image_info_obj.image_source_type_display), Q_FUNC_INFO);
        recon_static_functions::app_debug(("Image image_source_type_internal+++++---" + st_image_info_obj.image_source_type_internal), Q_FUNC_INFO);

        QList<struct_global_image_mounter_partition_info> list_struct_part_info = st_image_info_obj.list_struct_partition_info;
        recon_static_functions::app_debug(("PARTITION LIST SIZE  " + QString::number(list_struct_part_info.size())), Q_FUNC_INFO);

        for(int pp = 0; pp < list_struct_part_info.size(); pp++)
        {
            struct_global_image_mounter_partition_info st_part_info_obj = list_struct_part_info.at(pp);

            recon_static_functions::app_debug("----------------------- PARTITION-" + QString::number(pp)  + " Start -----------------------", Q_FUNC_INFO);

            recon_static_functions::app_debug(("PARTition parent_type-------" + st_part_info_obj.parent_type), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition parent_name-------" + st_part_info_obj.parent_name), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition parent_identifier-------" + st_part_info_obj.parent_identifier), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition parent_node-------" + st_part_info_obj.parent_node), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition derived_into_node-------" + st_part_info_obj.derived_into_node), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition derived_into_identifier-------" + st_part_info_obj.derived_into_identifier), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition derived_into_name-------" + st_part_info_obj.derived_into_name), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition derived_into_uuid-------" + st_part_info_obj.derived_into_uuid), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition mount_path-------" + st_part_info_obj.mount_path), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition total_size_byte_numeric_QString-------" + st_part_info_obj.total_size_byte_numeric_QString), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition free_size_byte_numeric_QString-------" + st_part_info_obj.free_size_byte_numeric_QString), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition fs_type-------" + st_part_info_obj.fs_type), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition volume_offset-------" + st_part_info_obj.volume_offset), Q_FUNC_INFO);
            recon_static_functions::app_debug(("PARTition volume_UUID-------" + st_part_info_obj.volume_UUID), Q_FUNC_INFO);

            recon_static_functions::app_debug("----------------------- PARTITION-" + QString::number(pp)  + " End -----------------------", Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug("^^^^^^^^^^^^^^^^^^^^^^^^^-Image Info END-^^^^^^^^^^^^^^^^^^^^^^^^^", Q_FUNC_INFO);

    recon_static_functions::app_debug(prepare_callerfun(caller_func) + " END ", Q_FUNC_INFO);

}


void recon_static_functions::print_destination_drive_info(struct_global_destination_drive_info pwd_obj, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    if(!global_variable_debug_mode_status_bool)
        return;


    QString fnl_data;

    //================Result Drive info Start =======================//

    QString splitter_str = " : ";

    QString label_name       = "Label Name" + splitter_str + pwd_obj.label_name + "\n";
    QString volume_name      = "Volume Name" + splitter_str + pwd_obj.volume_name + "\n";
    QString device_node      = "Device Disk Node" + splitter_str + pwd_obj.device_disk_node + "\n";
    QString drive_path       = "Drive Path" + splitter_str + pwd_obj.drive_path + "\n";
    QString block_size       = "Block Size" + splitter_str + recon_static_functions::human_readable_size(pwd_obj.block_size_numeric_int, Q_FUNC_INFO) + " (" + QString::number(pwd_obj.block_size_numeric_int) + " B)" + "\n";

    QString output_dir_total_space = "Total Space" + splitter_str + recon_static_functions::human_readable_size(pwd_obj.total_size_byte_numeric_qint64, Q_FUNC_INFO) + " (" + QString::number(pwd_obj.total_size_byte_numeric_qint64) + " B)" + "\n";
    QString output_dir_available_space = "Available Space" + splitter_str + recon_static_functions::human_readable_size(pwd_obj.available_size_byte_at_case_uptime_numeric_qint64,Q_FUNC_INFO) + " (" + QString::number(pwd_obj.available_size_byte_at_case_uptime_numeric_qint64) + " B)" + "\n";
    QString output_dir_file_System = "File System" + splitter_str + pwd_obj.file_system_type + "\n";

    QString read_only = "Read Only" + splitter_str + QVariant(pwd_obj.bool_read_only).toString() + "\n";

    QString ready_to_work = "Ready to Work" + splitter_str + QVariant(pwd_obj.bool_ready_to_work).toString() + "\n";
    QString system_root_volume = "System Root Volume" + splitter_str + QVariant(pwd_obj.bool_system_root_volume).toString() + "\n";
    QString valid_drive = "Valid Drive" + splitter_str + QVariant(pwd_obj.bool_valid_drive).toString() + "\n";
    QString result_dir_path = "RECON Result Dir Path" + splitter_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "\n";

    //================Result Drive info End =======================//


    fnl_data.append("****************************Destination Info Start*************************\n");
    fnl_data.append("=================================================================");
    fnl_data.append("\n");

    fnl_data.append(label_name.toLocal8Bit());
    fnl_data.append(volume_name.toLocal8Bit());
    fnl_data.append(device_node.toLocal8Bit());
    fnl_data.append(drive_path.toLocal8Bit());
    fnl_data.append(block_size.toLocal8Bit());

    fnl_data.append(output_dir_total_space.toLocal8Bit());
    fnl_data.append(output_dir_available_space.toLocal8Bit());
    fnl_data.append(output_dir_file_System.toLocal8Bit());
    fnl_data.append(read_only.toLocal8Bit());
    fnl_data.append(ready_to_work.toLocal8Bit());
    fnl_data.append(system_root_volume.toLocal8Bit());
    fnl_data.append(valid_drive.toLocal8Bit());
    fnl_data.append(result_dir_path.toLocal8Bit());

    fnl_data.append("=================================================================");
    fnl_data.append("\n");
    fnl_data.append("****************************Destination Info End***************************");
    fnl_data.append("\n\n");



    //======================Print on Terminal Start=======================//
    recon_static_functions::app_debug(fnl_data.toLocal8Bit(), Q_FUNC_INFO);
    //======================Print on Terminal End=========================//

    if(global_variable_debug_print_in_file_bool)
    {
        QFile examiner_log_file(debug_file_path_examiner);

        if(examiner_log_file.open(QIODevice::Append))
        {
            if(global_variable_debug_encryption_status_bool)
            {
                QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
                examiner_log_file.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
                examiner_log_file.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
            }
            else
            {
                examiner_log_file.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
            }

            examiner_log_file.flush();
            examiner_log_file.close();
        }

    }


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}



void recon_static_functions::print_examiner_info(struct_global_desktop_info pwd_obj, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    if(!global_variable_debug_mode_status_bool)
        return;


    QString fnl_data;

    QString splitter_str = " : ";

    QString label_name       = "Label Name" + splitter_str + pwd_obj.label_name + "\n";
    QString volume_name      = "Volume Name" + splitter_str + pwd_obj.volume_name + "\n";
    QString device_node      = "Device Disk Node" + splitter_str + pwd_obj.device_disk_node + "\n";
    QString file_system_type = "File System" + splitter_str + pwd_obj.file_system_type + "\n";
    QString block_size       = "Block Size" + splitter_str + recon_static_functions::human_readable_size(pwd_obj.block_size_numeric_int, Q_FUNC_INFO) + " (" + QString::number(pwd_obj.block_size_numeric_int) + " B)" + "\n";

    QString total_space      = "Total Space" + splitter_str + recon_static_functions::human_readable_size(pwd_obj.total_space_bytes_numeric_qint64, Q_FUNC_INFO) + " (" + QString::number(pwd_obj.total_space_bytes_numeric_qint64) + " B)" + "\n";
    QString available_space  = "Available Space" + splitter_str + recon_static_functions::human_readable_size(pwd_obj.available_space_bytes_numeric_qint64,Q_FUNC_INFO) + " (" + QString::number(pwd_obj.available_space_bytes_numeric_qint64) + " B)" + "\n";

    QString timezone_readable_cpmlt = "Time Zone Complete Name" + splitter_str + pwd_obj.timezone_name_complete_QString + "\n";
    QString timezone_readable = "Time Zone Name" + splitter_str + pwd_obj.timezone_name_QString + "\n";
    QString timezone_ofst_readable = "Time Zone Offset" + splitter_str + pwd_obj.timezone_offset_readable_QString + "\n";
    QString timezone_ofst_qint64 = "Time Zone Offset Value" + splitter_str + QString::number(pwd_obj.timezone_offset_numeric_qint64) + "\n";


    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
    fnl_data.append("[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "]" + "\n\n");


    fnl_data.append("****************************Examiner Machine Info Start*********************\n");
    fnl_data.append("=================================================================");
    fnl_data.append("\n");

    fnl_data.append(label_name.toLocal8Bit());
    fnl_data.append(volume_name.toLocal8Bit());
    fnl_data.append(device_node.toLocal8Bit());
    fnl_data.append(file_system_type.toLocal8Bit());
    fnl_data.append(block_size.toLocal8Bit());
    fnl_data.append(total_space.toLocal8Bit());
    fnl_data.append(available_space.toLocal8Bit());

    fnl_data.append(timezone_readable_cpmlt.toLocal8Bit());
    fnl_data.append(timezone_readable.toLocal8Bit());
    fnl_data.append(timezone_ofst_readable.toLocal8Bit());
    fnl_data.append(timezone_ofst_qint64.toLocal8Bit());

    fnl_data.append("\n");
    fnl_data.append("=================================================================");
    fnl_data.append("\n");

    QString consistency_str;
    if(pwd_obj.bool_consistency_requirements_satisfied)
    {
        consistency_str = MACRO_TRUE_VALUE_READABLE_QString;
    }
    else
    {
        consistency_str = QString("-----") + MACRO_FALSE_VALUE_READABLE_QString + QString("---- ");
        consistency_str =  consistency_str + "[" + pwd_obj.consistency_requirements_failure_items_QStringList.join(", ") + "]";

        if(consistency_str.contains("Python3"))
        {
            consistency_str.append(" [NOT TO WORRY about python as python modules are running through our executables.]");
        }

    }

    fnl_data.append("Consistency Requirements Satisfied" + splitter_str + "\n\t" + consistency_str.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("OSXFuse" + splitter_str + "\n\t" +  pwd_obj.osxfuse_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("Java" + splitter_str + "\n\t" +  pwd_obj.java_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("Xcode" + splitter_str + "\n\t" +  pwd_obj.xcode_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("Python" + splitter_str + "\n\t" +  pwd_obj.python_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("LZ4" + splitter_str + "\n\t" +  pwd_obj.lz4_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("ENUM34" + splitter_str + "\n\t" +  pwd_obj.enum34_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("NUDENET" + splitter_str + "\n\t" +  pwd_obj.nudenet_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("FILETYPE" + splitter_str + "\n\t" +  pwd_obj.filetype_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("MIME" + splitter_str + "\n\t" +  pwd_obj.mime_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("MIMELIB" + splitter_str + "\n\t" +  pwd_obj.mimelib_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("GLOB2" + splitter_str + "\n\t" +  pwd_obj.glob2_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("MOVIEPY" + splitter_str + "\n\t" +  pwd_obj.moviepy_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("FFMPEG" + splitter_str + "\n\t" +  pwd_obj.ffmpeg_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("OPENCV-PYTHON" + splitter_str + "\n\t" +  pwd_obj.opencv_python_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("PILLOW" + splitter_str + "\n\t" +  pwd_obj.pillow_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("Full Disk Access" + splitter_str + "\n\t" +  pwd_obj.full_disk_acces_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("=================================================================");
    fnl_data.append("\n\n\n");


    fnl_data.append("\n");
    fnl_data.append("Model Name" + splitter_str + "\n\t" + pwd_obj.model_name_info);
    fnl_data.append("\n\n");
    fnl_data.append("Model Year" + splitter_str + "\n\t" + pwd_obj.model_year_info);
    fnl_data.append("\n\n");
    fnl_data.append("Processor Brand" + splitter_str + "\n\t" + pwd_obj.processor_brand_info);
    fnl_data.append("\n\n");
    fnl_data.append("Ram Size" + splitter_str + "\n\t" + pwd_obj.ram_size_info);
    fnl_data.append("\n\n");
    fnl_data.append("OS Version" + splitter_str + "\n\t" + pwd_obj.os_version_info);
    fnl_data.append("\n\n");
    fnl_data.append("OS Encryption" + splitter_str + "\n\t" + pwd_obj.file_vault_status_info.toLocal8Bit());
    fnl_data.append("\n\n");
    fnl_data.append("System Integrity" + splitter_str + "\n\t" +  pwd_obj.csrutil_status_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("Security Controller (T2)" + splitter_str + "\n\t" +  pwd_obj.security_controller_info.toLocal8Bit());
    fnl_data.append("\n\n");

    QString power_settings_notes = QString("(1. sleep 0 means prevent computer from sleeping automatically. \n"
                                           " 2. displaysleep 0 means Never (values are in minutes). \n"
                                           " 3. disksleep 0 mean not to put hard disk to sleep. \n"
                                           " 4. powernap 0 means power nap is disabled.) \n\n");

    fnl_data.append("Power Sleep Settings" + splitter_str + "\n" + power_settings_notes + pwd_obj.power_settings_info.toLocal8Bit());
    fnl_data.append("\n\n");

    fnl_data.append("=================================================================");
    fnl_data.append("\n\n\n");

    fnl_data.append("=================================================================");
    fnl_data.append("\n");
    fnl_data.append(pwd_obj.hardware_overview.toLocal8Bit());
    fnl_data.append("\n");
    fnl_data.append("=================================================================");
    fnl_data.append("\n\n\n");

    fnl_data.append("=================================================================");
    fnl_data.append("\n");
    fnl_data.append(pwd_obj.software_overview.toLocal8Bit());
    fnl_data.append("\n");
    fnl_data.append("=================================================================");
    fnl_data.append("\n");
    fnl_data.append("****************************Examiner Machine Info End**********************");


    QString pwd_str;
    pwd_str.append("\n\n\n");
    pwd_str.append("=================================================================");
    pwd_str.append("\n");
    //===========Add START and END to show exact password, if password contain space at start or end position=========//
    pwd_str.append("System Password Original" + splitter_str.toLocal8Bit() + "START" + pwd_obj.desktop_password_original.toLocal8Bit() + "END");
    pwd_str.append("\n");
    pwd_str.append( "System Password General" + splitter_str.toLocal8Bit() + "START" + pwd_obj.desktop_password_modified_general.toLocal8Bit() + "END");
    pwd_str.append("\n");
    pwd_str.append("System Password Shell" + splitter_str.toLocal8Bit() + "START"+ pwd_obj.desktop_password_modified_shell_script.toLocal8Bit() + "END");
    pwd_str.append("\n");
    pwd_str.append("System Password Hex" + splitter_str.toLocal8Bit() + "START"+ pwd_obj.desktop_password_modified_symbol_hex_value.toLocal8Bit() + "END");
    pwd_str.append("\n");
    pwd_str.append("=================================================================");
    pwd_str.append("\n\n\n");

    //=============Enable if want to Print on Terminal Start=================//
    //fnl_data.append(pwd_str);
    //=============Enable if want to Print on Terminal End=================//


    //======================Print on Terminal Start=======================//
    recon_static_functions::app_debug(fnl_data.toLocal8Bit(), Q_FUNC_INFO);
    //======================Print on Terminal End=========================//

    fnl_data.append(pwd_str);

    if(global_variable_debug_print_in_file_bool)
    {
        QFile examiner_log_file(debug_file_path_examiner);

        if(examiner_log_file.open(QIODevice::WriteOnly | QIODevice::Truncate))

        {
            if(global_variable_debug_encryption_status_bool)
            {
                QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
                examiner_log_file.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
                examiner_log_file.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
            }
            else
            {
                examiner_log_file.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
            }


            examiner_log_file.flush();
            examiner_log_file.close();
        }


        QStorageInfo storage_info("/");
        QString disk_node = storage_info.device();

        QString desktop_plist_path = QFileInfo(debug_file_path_examiner).absolutePath() + "/f_diskinfo_desktop.plist";
        write_diskutil_info_in_file(desktop_plist_path , disk_node , Q_FUNC_INFO);

        if(global_variable_debug_encryption_status_bool)
        {
            recon_static_functions::encrypt_file_1(desktop_plist_path, Q_FUNC_INFO);
        }


    }


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void recon_static_functions::print_examiner_info_for_sumuri(struct_global_desktop_info pwd_obj, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    if(!global_variable_debug_mode_status_bool)
        return;


    QString sumuri_fnl_data;
    sumuri_fnl_data.append("----SUMURI_LOG_FILE----\n\n");

    QString splitter_str = " : ";

    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
    sumuri_fnl_data.append("[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "]" + "\n\n");


    sumuri_fnl_data.append("=================================================================");
    sumuri_fnl_data.append("\n");

    QString consistency_str;
    if(pwd_obj.bool_consistency_requirements_satisfied)
    {
        consistency_str = MACRO_TRUE_VALUE_READABLE_QString;
    }
    else
    {
        consistency_str = QString("-----") + MACRO_FALSE_VALUE_READABLE_QString + QString("---- ");
        consistency_str =  consistency_str + "[" + pwd_obj.consistency_requirements_failure_items_QStringList.join(", ") + "]";

        if(consistency_str.contains("Python3"))
        {
            consistency_str.append(" [NOT TO WORRY about python as python modules are running through our executables.]");
        }

    }

    sumuri_fnl_data.append("Consistency Requirements Satisfied" + splitter_str + "\n\t" + consistency_str.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");

    sumuri_fnl_data.append("OSXFuse" + splitter_str + "\n\t" +  pwd_obj.osxfuse_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");

    sumuri_fnl_data.append("Java" + splitter_str + "\n\t" +  pwd_obj.java_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");

    sumuri_fnl_data.append("Xcode" + splitter_str + "\n\t" +  pwd_obj.xcode_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");

    sumuri_fnl_data.append("Python" + splitter_str + "\n\t" +  pwd_obj.python_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("LZ4" + splitter_str + "\n\t" +  pwd_obj.lz4_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("ENUM34" + splitter_str + "\n\t" +  pwd_obj.enum34_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("NUDENET" + splitter_str + "\n\t" +  pwd_obj.nudenet_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("FILETYPE" + splitter_str + "\n\t" +  pwd_obj.filetype_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("MIME" + splitter_str + "\n\t" +  pwd_obj.mime_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("MIMELIB" + splitter_str + "\n\t" +  pwd_obj.mimelib_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("GLOB2" + splitter_str + "\n\t" +  pwd_obj.glob2_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("MOVIEPY" + splitter_str + "\n\t" +  pwd_obj.moviepy_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("FFMPEG" + splitter_str + "\n\t" +  pwd_obj.ffmpeg_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("OPENCV-PYTHON" + splitter_str + "\n\t" +  pwd_obj.opencv_python_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("PILLOW" + splitter_str + "\n\t" +  pwd_obj.pillow_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");
    sumuri_fnl_data.append("Full Disk Access" + splitter_str + "\n\t" +  pwd_obj.full_disk_acces_info.toLocal8Bit());
    sumuri_fnl_data.append("\n\n");

    sumuri_fnl_data.append("=================================================================");
    sumuri_fnl_data.append("\n\n\n");




    if(global_variable_debug_print_in_file_bool)
    {
        QFile examiner_log_file(debug_file_path_sumuri);

        if(examiner_log_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            if(global_variable_debug_encryption_status_bool)
            {
                QString encrypted_data = recon_static_functions::encrypt_string(sumuri_fnl_data , Q_FUNC_INFO);
                examiner_log_file.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
                examiner_log_file.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
            }
            else
            {
                examiner_log_file.write(sumuri_fnl_data.toLocal8Bit() , sumuri_fnl_data.toLocal8Bit().size());
            }


            examiner_log_file.flush();
            examiner_log_file.close();
        }
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void recon_static_functions::write_diskutil_info_in_file(QString file_path , QString disk_node, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    if(file_path.trimmed().isEmpty() || disk_node.trimmed().isEmpty())
        return;

    recon_helper_process recon_helper_process_obj;

    QString command = QString("diskutil info -plist " + disk_node + " > " + file_path);
    QString diskutil_outout = recon_helper_process_obj.run_command_by_shell(command , Q_FUNC_INFO);


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void recon_static_functions::write_diskutil_list_in_file(QString file_path, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    if(file_path.trimmed().isEmpty())
        return;


    recon_helper_process recon_helper_process_obj;

    QString command;
    command = QString("diskutil list > " + file_path);
    QString prc_diskutility_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO);


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}


void recon_static_functions::write_mountinfo_in_file(QString file_path, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    if(file_path.trimmed().isEmpty())
        return;


    recon_helper_process recon_helper_process_obj;

    QString command;
    command = QString("mount > " + file_path);
    QString prc_diskutility_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO);


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}


void recon_static_functions::print_witness_root_info(QList<struct_GLOBAL_witness_info_root> list_root_info, QStringList processed_root_count_name)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    if(!global_variable_debug_mode_status_bool)
        return;


    for(int jj = 0; jj < list_root_info.size(); jj++)
    {
        struct_GLOBAL_witness_info_root st_root_info = list_root_info.at(jj);
        if(processed_root_count_name.contains(st_root_info.root_count_name))
        {
            QString fnl_data;
            QString splitter_str = " : ";

            QString label_marco_narad = QString("MACRO_NARAD_");

            fnl_data.append("======================================================================================");
            fnl_data.append("\n");

            fnl_data.append(QString(MACRO_NARAD_Root_Count_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_count_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Evidence_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.evidence_type + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_type + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Source_Type_Display_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.source_type_display + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Source_Type_Internal_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.source_type_internal + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Path_1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_path_1 + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Path_2_for_fusion_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_path_2_for_fusion + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.home_directory_user_name + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Evidence_Number_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.evidence_number + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Description_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.description + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Hash_MD5_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_hash_md5 + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Hash_SHA1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_hash_sha1 + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Disk_Identifier_1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_disk_identifier_1 + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Disk_Node_1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_disk_node_1 + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_disk_identifier_2_for_fusion + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_disk_node_2_for_fusion + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_recovery_passcode_encrypted + "\n").toLocal8Bit());
            fnl_data.append(QString("root_recovery_passcode_plain").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_recovery_passcode_plain + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_evidence_password_encrypted + "\n").toLocal8Bit());
            fnl_data.append(QString("root_evidence_password_plain").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.root_evidence_password_plain + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Source_Count_Name_All_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.source_count_name_all_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.source_count_name_prime_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_root_info.source_count_name_nested_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());

            fnl_data.append("======================================================================================");
            fnl_data.append("\n\n\n");


            //===========================Print on Terminal Start========================//
            recon_static_functions::app_debug(fnl_data.toLocal8Bit(), Q_FUNC_INFO);
            //===========================Print on Terminal End==========================//


            if(global_variable_debug_print_in_file_bool)
            {
                QFile root_debug_file(debug_file_path_root);
                if(!root_debug_file.exists())
                {
                    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
                    fnl_data.prepend("[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "]\n\n");
                }

                if(root_debug_file.open(QIODevice::Append))
                {
                    if(global_variable_debug_encryption_status_bool)
                    {
                        QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
                        root_debug_file.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
                        root_debug_file.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
                    }
                    else
                    {
                        root_debug_file.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
                    }


                    root_debug_file.flush();
                    root_debug_file.close();
                }

                QString disk_node = st_root_info.root_disk_node_1;
                if(!disk_node.trimmed().isEmpty())
                {
                    QString root_info_plist_path = QFileInfo(debug_file_path_root).absolutePath() + "/f_diskinfo_" + st_root_info.root_count_name + ".plist";

                    write_diskutil_info_in_file(root_info_plist_path , disk_node , Q_FUNC_INFO);

                    if(global_variable_debug_encryption_status_bool)
                    {
                        recon_static_functions::encrypt_file_1(root_info_plist_path, Q_FUNC_INFO);
                    }
                }

            }
        }

    }


    if(global_variable_debug_print_in_file_bool)
    {
        QString dir_path = QFileInfo(debug_file_path_root).absolutePath();

        QString diskutility_file_name = QString("f_diskutil_list_upto_Root") + QString::number(list_root_info.size()) + ".log";

        QString avalbl_file_name = recon_static_functions::get_available_filename(diskutility_file_name , dir_path , Q_FUNC_INFO);
        QString diskutility_file_path = dir_path + "/" + avalbl_file_name;
        write_diskutil_list_in_file(diskutility_file_path , Q_FUNC_INFO);

        QString mountinfo_file_name = QString("f_mountinfo_upto_Root") + QString::number(list_root_info.size()) + ".log";
        QString mount_avalbl_file_name = recon_static_functions::get_available_filename(mountinfo_file_name , dir_path , Q_FUNC_INFO);
        QString mountinfo_file_path = dir_path + "/" + mount_avalbl_file_name;
        write_mountinfo_in_file(mountinfo_file_path , Q_FUNC_INFO);

        if(global_variable_debug_encryption_status_bool)
        {
            recon_static_functions::encrypt_file_1(diskutility_file_path, Q_FUNC_INFO);
            recon_static_functions::encrypt_file_1(mountinfo_file_path, Q_FUNC_INFO);
        }

    }


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void recon_static_functions::print_witness_source_info(QList<struct_GLOBAL_witness_info_source> list_source_info, QStringList newly_added_source_count_name)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    if(!global_variable_debug_mode_status_bool)
        return;


    for(int hh = 0; hh < list_source_info.size(); hh++)
    {
        struct_GLOBAL_witness_info_source st_source_info = list_source_info.at(hh);
        if(newly_added_source_count_name.contains(st_source_info.source_count_name))
        {

            QString fnl_data;

            QString splitter_str = " : ";

            QString label_marco_narad = QString("MACRO_NARAD_");

            fnl_data.append("======================================================================================");
            fnl_data.append("\n");

            fnl_data.append(QString(MACRO_NARAD_Source_Count_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_count_name + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Evidence_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.evidence_type + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Count_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_count_name + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Root_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_type + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Path_1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_path_1 + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Path_2_for_fusion_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_path_2_for_fusion + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Root_Disk_Identifier_1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_disk_identifier + "\n").toLocal8Bit());

            fnl_data.append(QString("parent_source_count_name").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.parent_source_count_name + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Hierarchy_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_hierarchy + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_RECON_Logical_Mode_bool).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + QString(QVariant(st_source_info.bool_recon_logical_image).toString())).toLocal8Bit() + "\n");
            fnl_data.append(QString(MACRO_NARAD_Source_Nested_bool).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + QString(QVariant(st_source_info.bool_source_nested).toString())).toLocal8Bit() + "\n");
            fnl_data.append(QString(MACRO_NARAD_Source_Decompressed_bool).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + QString(QVariant(st_source_info.bool_source_decompressed).toString())).toLocal8Bit() + "\n");
            fnl_data.append(QString(MACRO_NARAD_Source_Disk_Identifier_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.source_disk_identifier + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Role_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.role_type + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Type_Display_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_type_display + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Type_Internal_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_type_internal + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Main_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.source_main_name + "\n").toLocal8Bit());
            fnl_data.append(QString("complete_source_name").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.complete_source_name + "\n").toLocal8Bit());


            fnl_data.append(QString(MACRO_NARAD_Source_Main_Path_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_main_path + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Path_1_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_path_1 + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Path_2_For_Fusion_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_path_2_for_fusion + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Virtual_Source_Path_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.virtual_source_path + "\n").toLocal8Bit());

            fnl_data.append(QString("fs_extraction_source_path").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.fs_extraction_source_path + "\n").toLocal8Bit());
            fnl_data.append(QString("fuse_mount_path_tsk").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.fuse_mount_path_tsk + "\n").toLocal8Bit());
            fnl_data.append(QString("fuse_mount_path_raw").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.fuse_mount_path_raw + "\n").toLocal8Bit());

            fnl_data.append(QString("source_recovery_passcode_plain").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.root_recovery_passcode_plain + "\n").toLocal8Bit());

            fnl_data.append(QString("source_evidence_password_plain").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_evidence_password_plain + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.source_evidence_password_encrypted + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Time_Zone_Readable_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + st_source_info.timezone_readable_QString + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Time_Zone_Numeric_qint64).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + QString::number(st_source_info.timezone_numeric_qint64) + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_OS_Scheme_Display_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.os_scheme_display + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_OS_Scheme_Internal_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.os_scheme_internal + "\n").toLocal8Bit());

            fnl_data.append(QString("system_users_QStringList").remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.system_user_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());

            QString sys_acc_str = recon_static_functions::get_system_users_and_domain_users_from_system_account_QList(st_source_info.system_account_QList , Q_FUNC_INFO);
            fnl_data.append(QString(MACRO_NARAD_Source_System_Account_QList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + sys_acc_str + "\n").toLocal8Bit());


            fnl_data.append(QString(MACRO_NARAD_Source_OS_Version_Extended_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.os_version_extended + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_OS_Version_Full_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.os_version_full + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Country_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.country + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_City_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.city + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Latitude_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.latitude + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Longitude_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.longitude + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Build_Version_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.build_version + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Product_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.product_type + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Product_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.product_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Display_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.display_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Device_Name_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.device_name + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Id_No_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.device_id + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_GUID_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.guid + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Target_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.target_type + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_iTune_Version_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.itune_version + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Serial_No_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.serial_no + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Imei_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.imei_no_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Sim_No_Iccid_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.latest_backup_date_readable_QString + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  QString::number(st_source_info.latest_backup_date_numeric_qint64) + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.backup_size_byte_numeric_QString + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Product_Model_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.product_model + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Bluetooth_Device_Address_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.bluetooth_device_address + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Wifi_Address_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.wifi_address + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Locale_Language_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.locale_language + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Configured_ID_QStringList).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.configured_ids_QStringList.join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Volume_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.image_volume_type + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Volume_Offset_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.image_volume_offset + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Volume_UUID_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.image_volume_uuid + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.total_size_byte_numeric_QString + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.used_space_byte_numeric_QString + "\n").toLocal8Bit());

            fnl_data.append(QString(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.free_space_byte_numeric_QString + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_File_System_Type_QString).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str +  st_source_info.file_system_type + "\n").toLocal8Bit());
            fnl_data.append(QString(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64).remove(label_marco_narad).toLocal8Bit() + QString(splitter_str + QString::number(st_source_info.os_installer_timestamp_utc_numeric_qint64) + "\n").toLocal8Bit());

            fnl_data.append("======================================================================================");
            fnl_data.append("\n\n\n");


            //===========================Print on Terminal Start=========================//
            recon_static_functions::app_debug(fnl_data.toLocal8Bit(), Q_FUNC_INFO);
            //===========================Print on Terminal End============================//

            if(global_variable_debug_print_in_file_bool)
            {
                QFile source_debug_file(debug_file_path_source);
                if(!source_debug_file.exists())
                {
                    QString date_time = QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
                    fnl_data.prepend("[" + get_app_name() + "-" + get_app_version() + get_app_build_modified().trimmed() + "] [" + date_time + "]\n\n");
                }

                if(source_debug_file.open(QIODevice::Append))
                {
                    if(global_variable_debug_encryption_status_bool)
                    {
                        QString encrypted_data = recon_static_functions::encrypt_string(fnl_data , Q_FUNC_INFO);
                        source_debug_file.write(encrypted_data.toLocal8Bit() , encrypted_data.toLocal8Bit().size());
                        source_debug_file.write(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_3_Brackets);
                    }
                    else
                    {
                        source_debug_file.write(fnl_data.toLocal8Bit() , fnl_data.toLocal8Bit().size());
                    }


                    source_debug_file.flush();
                    source_debug_file.close();
                }


                QString disk_node = st_source_info.source_disk_identifier;
                if(!disk_node.trimmed().isEmpty())
                {
                    QString source_info_plist_path = QFileInfo(debug_file_path_source).absolutePath() + "/f_diskinfo_" + st_source_info.source_count_name + ".plist";

                    write_diskutil_info_in_file(source_info_plist_path , "/dev/" + disk_node , Q_FUNC_INFO);

                    if(global_variable_debug_encryption_status_bool)
                    {
                        recon_static_functions::encrypt_file_1(source_info_plist_path, Q_FUNC_INFO);
                    }
                }

            }

        }
    }


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

