#include "desktop_info.h"

desktop_info::desktop_info(QWidget *parent)
{
    recon_static_functions::clear_variables_desktop_info_all(st_desktop_info);
    recon_helper_standard_obj = new recon_helper_standard(this);

    consistency_requirements_failed_list.clear();
}

desktop_info::~desktop_info()
{

}



struct_global_desktop_info desktop_info::pub_get_desktop_info(QString caller_func)
{
    return st_desktop_info;
}


void desktop_info::pub_update_desktop_password(bool mstatus, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    recon_static_functions::clear_variables_desktop_password(st_desktop_info);

    if(!mstatus)
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return;
    }


    QString file_path_orgl = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString).toString() + "confidential_original.txt";
    QString m_pwd_orgl = recon_static_functions::get_plain_data_from_encrypt_file_1(file_path_orgl, Q_FUNC_INFO);
    QString m_pwd_modif = recon_static_functions::get_modify_password(m_pwd_orgl,Q_FUNC_INFO);
    QString m_pwd_modif_script = recon_static_functions::pub_get_simplified_password_for_shell_script(m_pwd_orgl,Q_FUNC_INFO);
    QString m_pwd_modif_hex = recon_static_functions::get_modified_password_replace_symbol_with_hex(m_pwd_orgl,Q_FUNC_INFO);



    st_desktop_info.desktop_password_modified_symbol_hex_value = m_pwd_modif_hex;
    st_desktop_info.desktop_password_modified_general = m_pwd_modif;
    st_desktop_info.desktop_password_original = m_pwd_orgl;
    st_desktop_info.desktop_password_modified_shell_script = m_pwd_modif_script;
    st_desktop_info.bool_desktop_pwd_accurate = true;


    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

}

void desktop_info::update_python_info(struct_global_desktop_info &mobj)
{
    mobj.python_info = QString("Python executable path could not be found by RECON.");
    mobj.lz4_info = QString("lz4 could not be found by RECON.");
    mobj.enum34_info = QString("enum34 could not be found by RECON.");
    mobj.nudenet_info = QString("nudenet could not be found by RECON.");
    mobj.filetype_info = QString("filetype could not be found by RECON.");
    mobj.mime_info = QString("mime could not be found by RECON.");
    mobj.mimelib_info = QString("mimelib could not be found by RECON.");
    mobj.glob2_info = QString("glob2 could not be found by RECON.");
    mobj.moviepy_info = QString("moviepy could not be found by RECON.");
    mobj.ffmpeg_info = QString("ffmpeg could not be found by RECON.");
    mobj.opencv_python_info = QString("opencv-python could not be found by RECON.");
    mobj.pillow_info = QString("pillow could not be found by RECON.");

    bool bool_python_installed_found = false;

    QString python_exe_path = global_varibale_python_path;

    if(QFileInfo(python_exe_path).exists())
    {
        recon_helper_process recon_helper_process_obj;

        QString command = QString(python_exe_path + " --version");
        QString prc_python_info = recon_helper_process_obj.run_command_by_shell(command , Q_FUNC_INFO).trimmed();

        if(prc_python_info.startsWith("Python 3." , Qt::CaseInsensitive))
        {
            bool_python_installed_found = true;
            mobj.python_info = prc_python_info;

        }
    }

    if(bool_python_installed_found)
    {
        recon_helper_process recon_helper_process_obj;

        QString command;

        //Note: Add Power(^) at the begining of the package to find package name start with a particular name.
        //Note: Add space at the end of the package name for found exact match in output.

        command = QString(python_exe_path + " -m pip list | grep -i '^lz4 '");
        QString prc_lz4_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.lz4_info = prc_lz4_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^enum34 '");
        QString prc_enum34_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.enum34_info = prc_enum34_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^nudenet '");
        QString prc_nudenet_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.nudenet_info = prc_nudenet_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^filetype '");
        QString prc_filetype_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.filetype_info = prc_filetype_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^mime '");
        QString prc_mime_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.mime_info = prc_mime_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^mimelib '");
        QString prc_mimelib_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.mimelib_info = prc_mimelib_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^glob2 '");
        QString prc_glob2_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.glob2_info = prc_glob2_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^moviepy '");
        QString prc_moviepy_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.moviepy_info = prc_moviepy_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^ffmpeg '");
        QString prc_ffmpeg_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.ffmpeg_info = prc_ffmpeg_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^opencv-python '");
        QString prc_opencv_python_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.opencv_python_info = prc_opencv_python_info;

        command = QString(python_exe_path + " -m pip list | grep -i '^pillow '");
        QString prc_pillow_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO).trimmed();
        mobj.pillow_info = prc_pillow_info;

    }

}

void desktop_info::pub_fill_desktop_info_general(QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    recon_static_functions::clear_variables_desktop_general(st_desktop_info);

    st_desktop_info.os_version_full = recon_static_functions::extract_os_version_full_xcode_mm_macOS("/", Q_FUNC_INFO);
    st_desktop_info.os_version_extended = recon_static_functions::extract_os_version_extended_xcode_mm_macOS("/", Q_FUNC_INFO);

    QStorageInfo storage_info("/");
    st_desktop_info.label_name = storage_info.name();
    st_desktop_info.volume_name = storage_info.displayName();
    st_desktop_info.device_disk_node = storage_info.device();
    st_desktop_info.file_system_type = storage_info.fileSystemType();

    QString extraxted_timezone = recon_static_functions::extract_time_zone_macOS("/", Q_FUNC_INFO);
    QString cmplt_timezone = recon_helper_standard_obj->get_complete_timezone_name_label(extraxted_timezone,Q_FUNC_INFO);
    qint64 tmzone_offset = recon_helper_standard_obj->get_time_zone_offset_with_sign(cmplt_timezone,Q_FUNC_INFO);
    st_desktop_info.timezone_name_complete_QString = cmplt_timezone;
    st_desktop_info.timezone_offset_numeric_qint64 = tmzone_offset;
    st_desktop_info.timezone_name_QString = extraxted_timezone;

    if(cmplt_timezone.contains("GMT"))
    {
        QStringList tmp_list;
        if(cmplt_timezone.contains("GMT-GMT")) // Africa/Accra-GMT-GMT+0:00
        {
            tmp_list = cmplt_timezone.split("GMT-GMT");

        }
        else
        {
            tmp_list = cmplt_timezone.split("GMT"); // Asia/Kolkata-IST-GMT+5:30
        }

        if(tmp_list.size() >= 1)
        {
            st_desktop_info.timezone_offset_readable_QString = tmp_list.at(1);
        }
    }

    st_desktop_info.block_size_numeric_int = storage_info.blockSize();
    st_desktop_info.available_space_bytes_numeric_qint64 = storage_info.bytesAvailable();
    st_desktop_info.total_space_bytes_numeric_qint64 = storage_info.bytesTotal();


    recon_helper_process recon_helper_process_obj;

    QString prc_hardware_system_profile = recon_helper_process_obj.run_command_with_arguments("system_profiler", QStringList("SPHardwareDataType"),Q_FUNC_INFO);
    QString prc_software_system_profile = recon_helper_process_obj.run_command_with_arguments("system_profiler", QStringList("SPSoftwareDataType"),Q_FUNC_INFO);
    QString csrutil_status = recon_helper_process_obj.run_command_with_arguments("csrutil", QStringList("status"),Q_FUNC_INFO);

    st_desktop_info.hardware_overview = prc_hardware_system_profile;
    st_desktop_info.software_overview = prc_software_system_profile;
    st_desktop_info.csrutil_status_info = csrutil_status;


    QStringList args;

    QString home_dir = getenv("HOME");
    args << "-c" << "print :'CPU Names'"  << home_dir + "/Library/Preferences/com.apple.SystemProfiler.plist";
    QString prc_model_year = recon_helper_process_obj.run_command_with_arguments("/usr/libexec/PlistBuddy", QStringList(args),Q_FUNC_INFO);

    args.clear();
    args << "-g" << "live";
    QString prc_power_settings = recon_helper_process_obj.run_command_with_arguments("pmset", QStringList(args),Q_FUNC_INFO);

    args.clear();
    args << "status";
    QString file_vault_status = recon_helper_process_obj.run_command_with_arguments("fdesetup", QStringList(args),Q_FUNC_INFO);


    st_desktop_info.model_year_info = prc_model_year;
    st_desktop_info.power_settings_info = prc_power_settings;
    st_desktop_info.file_vault_status_info = file_vault_status;


    QString command = QString("system_profiler SPHardwareDataType | grep -i 'Model Name:'");
    QString prc_model_name  = recon_helper_process_obj.run_command_by_shell(command ,Q_FUNC_INFO);

    command.clear();
    command = QString("sysctl -n machdep.cpu.brand_string");
    QString prc_processor_brand  = recon_helper_process_obj.run_command_by_shell(command ,Q_FUNC_INFO);

    st_desktop_info.bool_processor_chip_silicon = global_variable_desktop_silicon_bool;

    command.clear();
    command = QString("system_profiler SPHardwareDataType | grep -i 'Memory:'");
    QString prc_ram_size  = recon_helper_process_obj.run_command_by_shell(command ,Q_FUNC_INFO);


    st_desktop_info.model_name_info = prc_model_name;
    st_desktop_info.processor_brand_info = prc_processor_brand;
    st_desktop_info.ram_size_info = prc_ram_size;


    command.clear();
    command = QString("system_profiler SPSoftwareDataType | grep -i 'System Version:'");
    QString prc_os_version  = recon_helper_process_obj.run_command_by_shell(command ,Q_FUNC_INFO);
    st_desktop_info.os_version_info = prc_os_version;


    command.clear();
    command = QString("ioreg -p IOUSB -w0 | sed 's/[^o]*o //; s/@.*$//' | grep -i 'Controller'");
    QString prc_controller_info  = recon_helper_process_obj.run_command_by_shell(command ,Q_FUNC_INFO);
    st_desktop_info.security_controller_info = prc_controller_info;


    args.clear();
    args << "-c" << "print :'CFBundleVersion'" << "/Library/Filesystems/macfuse.fs/Contents/version.plist";
    QString prc_fuse_info = recon_helper_process_obj.run_command_with_arguments("/usr/libexec/PlistBuddy", QStringList(args),Q_FUNC_INFO);
    st_desktop_info.osxfuse_info = prc_fuse_info;

    update_python_info(st_desktop_info);

    command.clear();
    command = QString("/usr/bin/java -version");
    //    command = QString("java -version");
    QString prc_java_info = recon_helper_process_obj.run_command_by_shell(command ,Q_FUNC_INFO);
    st_desktop_info.java_info = prc_java_info;


    command.clear();
    command = QString("/usr/bin/xcodebuild -version");
    //    command = QString("xcodebuild -version");
    QString prc_xcode_info = recon_helper_process_obj.run_command_by_shell(command,Q_FUNC_INFO);
    st_desktop_info.xcode_info = prc_xcode_info;


    command.clear();
    command = QString("sqlite3 \"/Library/Application Support/com.apple.TCC/TCC.db\"  "
                      "'select client from access where service = \"kTCCServiceSystemPolicyAllFiles\"' | grep 'RECON-LAB'");

    QString prc_full_disk_access_info = recon_helper_process_obj.run_command_by_shell(command , Q_FUNC_INFO);

    st_desktop_info.full_disk_acces_info = prc_full_disk_access_info;

    st_desktop_info.bool_consistency_requirements_satisfied = bool_consistency_requirements_satisfied();

    st_desktop_info.consistency_requirements_failure_items_QStringList = consistency_requirements_failed_list;

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
}

bool desktop_info::bool_consistency_requirements_satisfied()
{
    consistency_requirements_failed_list.clear();

    QString osxfuse_info = st_desktop_info.osxfuse_info;
    QString java_info = st_desktop_info.java_info;

    QString python_info = st_desktop_info.python_info;
    QString lz4_info = st_desktop_info.lz4_info;
    QString enum34_info = st_desktop_info.enum34_info;
    QString nudenet_info = st_desktop_info.nudenet_info;
    QString filetype_info = st_desktop_info.filetype_info;
    QString mime_info = st_desktop_info.mime_info;
    QString mimelib_info = st_desktop_info.mimelib_info;
    QString glob2_info = st_desktop_info.glob2_info;
    QString moviepy_info = st_desktop_info.moviepy_info;
    QString ffmpeg_info = st_desktop_info.ffmpeg_info;
    QString opencv_python_info = st_desktop_info.opencv_python_info;
    QString pillow_info = st_desktop_info.pillow_info;
    QString full_disk_acces_info = st_desktop_info.full_disk_acces_info;
    QString xcode_info = st_desktop_info.xcode_info;


    bool bool_osxfuse_available = false;
    bool bool_java_available = false;
    bool bool_python_available = false;
    bool bool_enum34_available = false;
    bool bool_lz4_available = false;
    bool bool_nudenet_available = false;
    bool bool_filetype_available = false;
    bool bool_mime_available = false;
    bool bool_mimelib_available = false;
    bool bool_glob2_available = false;
    bool bool_moviepy_available = false;
    bool bool_ffmpeg_available = false;
    bool bool_opencv_python_available = false;
    bool bool_pillow_available = false;

    bool bool_full_disk_access_available = false;
    bool bool_xcode_available = false;

    //    if(!(osxfuse_info.contains("Does Not Exist" , Qt::CaseInsensitive) || osxfuse_info.trimmed().isEmpty()))
    if(!(osxfuse_info.contains("Does Not Exist" , Qt::CaseInsensitive) || osxfuse_info.contains("Doesn't Exist" , Qt::CaseInsensitive) || osxfuse_info.trimmed().isEmpty()))
    {
        QStringList str_list = osxfuse_info.split(".");
        if(str_list.size() == 3)
        {
            bool_osxfuse_available = true;
        }
        else
            consistency_requirements_failed_list.append("osx Fuse-1");
    }
    else
        consistency_requirements_failed_list.append("osx Fuse-2");


    if(!(java_info.contains("The operation couldn’t be completed. Unable to locate a Java Runtime." , Qt::CaseInsensitive)
         || java_info.trimmed().isEmpty()))
    {
        bool_java_available = true;
    }
    else
        consistency_requirements_failed_list.append("Java");


    if(python_info.startsWith("Python 3." , Qt::CaseInsensitive))
    {
        bool_python_available = true;
    }
    else
        consistency_requirements_failed_list.append("Python3");


    if(lz4_info.startsWith("lz4 " , Qt::CaseInsensitive))
    {
        bool_lz4_available = true;
    }
    else
        consistency_requirements_failed_list.append("Lz4");


    if(enum34_info.startsWith("enum34 " , Qt::CaseInsensitive))
    {
        bool_enum34_available = true;
    }
    else
        consistency_requirements_failed_list.append("enum34");


    if(nudenet_info.startsWith("nudenet " , Qt::CaseInsensitive))
    {
        bool_nudenet_available = true;
    }
    else
        consistency_requirements_failed_list.append("nudenet");

    if(filetype_info.startsWith("filetype " , Qt::CaseInsensitive))
    {
        bool_filetype_available = true;
    }
    else
        consistency_requirements_failed_list.append("filetype");


    if(mime_info.startsWith("mime " , Qt::CaseInsensitive))
    {
        bool_mime_available = true;
    }
    else
        consistency_requirements_failed_list.append("mime");

    if(mimelib_info.startsWith("mimelib " , Qt::CaseInsensitive))
    {
        bool_mimelib_available = true;
    }
    else
        consistency_requirements_failed_list.append("mimelib");

    if(glob2_info.startsWith("glob2 " , Qt::CaseInsensitive))
    {
        bool_glob2_available = true;
    }
    else
        consistency_requirements_failed_list.append("glob2");

    if(moviepy_info.startsWith("moviepy " , Qt::CaseInsensitive))
    {
        bool_moviepy_available = true;
    }
    else
        consistency_requirements_failed_list.append("moviepy");

    if(ffmpeg_info.startsWith("ffmpeg " , Qt::CaseInsensitive))
    {
        bool_ffmpeg_available = true;
    }
    else
        consistency_requirements_failed_list.append("ffmpeg");

    if(opencv_python_info.startsWith("opencv-python " , Qt::CaseInsensitive))
    {
        bool_opencv_python_available = true;
    }
    else
        consistency_requirements_failed_list.append("opencv-python");

    if(pillow_info.startsWith("pillow " , Qt::CaseInsensitive))
    {
        bool_pillow_available = true;
    }
    else
        consistency_requirements_failed_list.append("pillow");

    //    if(!(xcode_info.contains("error: tool 'xcodebuild' requires Xcode" , Qt::CaseInsensitive) || xcode_info.trimmed().isEmpty()))
    if(xcode_info.startsWith("Xcode " , Qt::CaseInsensitive) && !xcode_info.contains("error: tool 'xcodebuild' requires Xcode" , Qt::CaseInsensitive))
    {
        bool_xcode_available = true;
    }
    else
        consistency_requirements_failed_list.append("xcode");


    if(full_disk_acces_info.endsWith("RECON-LAB"))
    {
        bool_full_disk_access_available = true;
    }
    else
        consistency_requirements_failed_list.append("RECON-LAB-Fulldisk-Access");


    if(bool_osxfuse_available && bool_java_available && bool_python_available
            && bool_lz4_available && bool_enum34_available && bool_nudenet_available
            && bool_filetype_available && bool_mime_available
            && bool_mimelib_available && bool_glob2_available && bool_moviepy_available && bool_ffmpeg_available
            && bool_opencv_python_available && bool_pillow_available && bool_xcode_available
            && bool_full_disk_access_available)
    {
        return true;
    }


    return false;
}
