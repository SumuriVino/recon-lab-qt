#include "imager_helper_standard.h"

imager_helper_standard::imager_helper_standard(QWidget *parent)
{
    bool_process_generic_finished = false;
    process_generic = new QProcess(this);
    connect(process_generic,SIGNAL(finished(int)),this,SLOT(slot_generic_process_finished(int)));

}

imager_helper_standard::~imager_helper_standard()
{

}

void imager_helper_standard::pub_change_combobox_text_color(QComboBox &combobox, QList<struct_disk_info> list_disk_full_info)
{
    for(int count = 0; count < combobox.count(); count++)
    {
        QString m_item = combobox.itemText(count);

        if(m_item.trimmed().isEmpty())
            continue;

        QStringList tmp_list = m_item.split(" ");

        QString disk_text = tmp_list.at(0);
        disk_text = disk_text.trimmed();

        struct_disk_info m_obj;
        bool bool_disk_found = false;
        for(int pp = 0; pp < list_disk_full_info.size(); pp++)
        {
            m_obj = list_disk_full_info.at(pp);

            if(m_obj.device_identifier == disk_text)
            {
                bool_disk_found = true;
                break;
            }
        }

        if(!bool_disk_found)
            continue;

        if(m_obj.bool_is_encrypt && !m_obj.bool_is_locked)
            combobox.setItemData(count, QBrush(QColor(color_Olive)), Qt::ForegroundRole);
    }

}

void imager_helper_standard::slot_generic_process_finished(int)
{
    bool_process_generic_finished = true;
}

void imager_helper_standard::pub_set_bool_process_generic(bool status)
{
    bool_process_generic_finished = status;
}

void imager_helper_standard::pub_terminate_generic_process()
{
    process_generic->terminate();
}

qint64 imager_helper_standard::pub_get_network_mount_path_used_size(QString mount_path)
{
    if(mount_path.trimmed().isEmpty())
        return 0;


    QStringList val_list;
    val_list << mount_path;

    QString proc_out = run_command_with_arguments_no_trimmed("df", val_list, Q_FUNC_INFO);

    QString size_str;
    QStringList proc_out_split_list = proc_out.split("\n");

    if(proc_out_split_list.size() > 1)
    {
        size_str = proc_out_split_list.at(1);

        size_str = size_str.trimmed();

        proc_out_split_list.clear();
        proc_out_split_list = size_str.split(" ");

        if(proc_out_split_list.size() > 3)
        {
            size_str = proc_out_split_list.at(3);
        }


    }
    size_str = size_str.trimmed();

    qint64 used_size = 0;
    if(!size_str.isEmpty())
        used_size = (size_str.toLongLong() * 512); //static block size

    return used_size;
}

qint64 imager_helper_standard::pub_get_used_size_using_mount_path(QString mount_path)
{
    if(mount_path.trimmed().isEmpty())
        return 0;


    QStringList val_list;
    val_list << mount_path;

    QString proc_out = run_command_with_arguments_no_trimmed("df", val_list, Q_FUNC_INFO);

    QString size_str;
    QStringList proc_out_split_list = proc_out.split("\n");

    if(proc_out_split_list.size() > 1)
    {
        size_str = proc_out_split_list.at(1);

        size_str = size_str.trimmed();

        proc_out_split_list.clear();
        proc_out_split_list = size_str.split(" ");

        proc_out_split_list.removeAll("");

        if(proc_out_split_list.size() > 3)
        {
            size_str = proc_out_split_list.at(2);
        }
    }
    size_str = size_str.trimmed();

    qint64 used_size = 0;
    if(!size_str.isEmpty())
        used_size = (size_str.toLongLong() * 512); //static block size

    return used_size;
}

qint64 imager_helper_standard::pub_get_used_space_using_disk_node(QString disk_node)
{
    qint64 s_size = 0;

    QStringList args;
    args << "info" << disk_node;
    QString info_str;

    QString cmd_output = run_command_with_arguments("diskutil", args, Q_FUNC_INFO);

    QStringList output_list = cmd_output.split("\n") ;

    for(int ls = 0 ; ls < output_list.size() ; ls++)
    {
        info_str = output_list.at(ls);

        if(info_str.contains("Volume Used Space:"))
        {
            int index_1 = info_str.indexOf("(");

            info_str.remove(0, index_1 + 1);

            int index_2 = info_str.indexOf(" ");

            info_str.remove(index_2, 500);

            s_size =  info_str.toLongLong();

            return s_size;
        }
    }

    return s_size;
}

QString imager_helper_standard::run_command_with_arguments(QString command, QStringList arguments , QString caller_func)
{
    bool_process_generic_finished = false;

    process_generic->start(command,arguments);

    //recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic_finished)
        {
            break;
        }
    }
   // recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + "  : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}

QString imager_helper_standard::run_command_without_argument(QString command , QString caller_func)
{
    bool_process_generic_finished = false;


    process_generic->start(command);

   // recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic_finished)
            break;
    }
  //  recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}


QString imager_helper_standard::run_command_with_arguments_no_trimmed(QString command, QStringList arguments , QString caller_func)
{
    bool_process_generic_finished = false;

    process_generic->start(command,arguments);

    //recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic_finished)
            break;
    }
   // recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + "  : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll());
}

QString imager_helper_standard::pub_extract_device_mount_path_using_device_node(QString device_node , QList<struct_disk_info> list_disk_full_info)
{
    if(device_node.startsWith("/dev"))
        device_node = device_node.remove(QString("/dev/"));

    for(int i = 0; i < list_disk_full_info.size(); i++)
    {
        struct_disk_info obj = list_disk_full_info.at(i);

        if(obj.device_identifier == device_node)
        {
            if(obj.mount_point_MountPoint.isEmpty())
                return NOMOUNT;
            else
                return obj.mount_point_MountPoint;

        }
    }

    return NOMOUNT;
}

bool imager_helper_standard::pub_write_data_to_file(QString a_data, QString file_path, int mode)
{
    QFile mfile(file_path);

    bool bool_file_opened = false;

    if(mode == WRITE_WITH_TRUNCATE)
        bool_file_opened = mfile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    else if(mode == WRITE_WITH_APPEND)
        bool_file_opened = mfile.open(QIODevice::ReadWrite | QIODevice::Append);

    if(!bool_file_opened)
        return false;

    mfile.write(a_data.toLocal8Bit());

    mfile.flush();

    mfile.close();

    return true;

}

bool imager_helper_standard::pub_append_file_data_from_another_file(QString first_file_path, QString second_file_path)
{
    QFile first_file(first_file_path);
    QFile second_file(second_file_path);

    if(!first_file.open(QIODevice::ReadWrite | QIODevice::Append))
        return false;

    if(!second_file.open(QIODevice::ReadOnly))
    {
        first_file.close();
        return false;
    }

    QByteArray second_file_data = second_file.readAll();

    second_file.close();

    first_file.write(second_file_data);

    first_file.flush();

    first_file.close();

    return true;
}

QString imager_helper_standard::pub_get_mount_path_of_network_volume(QString network_path)
{
    QString mount_path = QString(NOMOUNT);
    QList<network_share_info> m_list = pub_get_network_share_info();

    for(int i = 0 ; i < m_list.size(); i++)
    {
        network_share_info obj = m_list.at(i);

        if(obj.address == network_path.trimmed())
        {
            return obj.mount_path;
        }
    }

    return mount_path;
}

QString imager_helper_standard::pub_extract_disk_node_from_combobox_item_text(QString str) // disk2s1
{
    if(!str.startsWith("disk"))
        return NONE;

    QStringList str_list = str.split(" ");

    if(str_list.size() < 2)
        return NONE;

    QString disk_name = str_list.at(0);

    if(!disk_name.startsWith("disk"))
        return NONE;



    return disk_name.trimmed();

}

QList<network_share_info> imager_helper_standard::pub_get_network_share_info()
{
    QList<network_share_info> network_info_list;

    QString mount_output = run_command_without_argument("mount", Q_FUNC_INFO);

    QStringList mount_output_list = mount_output.split("\n");

    QStringList network_share_line_list;

    for(int i = 0 ; i < mount_output_list.size(); i++)
    {
        QString a_str = mount_output_list.at(i);

        if(a_str.contains("smbfs"))
        {
            a_str = a_str.trimmed();
            network_share_line_list << a_str;
        }
    }


    for(int ss = 0 ; ss < network_share_line_list.size() ; ss++)
    {
        QString line_smb = network_share_line_list.at(ss);

        QStringList list_network = line_smb.split(QString(" on "));

        if(list_network.size() < 2)
            continue;

        QString address = list_network.at(0);
        address = address.trimmed();

        QString path = list_network.at(1);

        if(path.endsWith("(smbfs)"))
        {
            path.chop(QString("(smbfs)").size());
            path = path.trimmed();
        }

        if(path.contains("(smbfs"))
        {
            int m_index = path.indexOf("(smbfs");

            path = path.remove(m_index, 100).trimmed();
        }

        if(address.isEmpty() || path.isEmpty())
            continue;

        QString share_name;
        if(address.contains("/"))
        {
            share_name = address;
            int share_index = share_name.lastIndexOf("/");
            share_name = share_name.remove(0,share_index + 1);
        }


        network_share_info obj;
        obj.address = address;
        obj.mount_path = path;
        obj.share_name = share_name;

        network_info_list.append(obj);
    }

    return network_info_list;
}

QString imager_helper_standard::get_human_readable_time(QString str, QString caller_func)
{

    if(str.contains("."))
        str = str.remove(str.indexOf("."), str.size());


    bool ok;
    int rcvd = str.toInt(&ok,10);


    if(rcvd < 0 || rcvd > 86399) // 24 hrs
        return "";

    int hr = rcvd/3600;
    int hr_p = rcvd % 3600;

    int min = hr_p / 60;
    int min_p = hr_p % 60;

    int base_value = 10; //decimal
    int req_digit_count = 2;

    QString hour =  QString("%1").arg(hr,req_digit_count,base_value, QChar('0'));

    QString minute =  QString("%1").arg(min,req_digit_count,base_value, QChar('0'));

    QString second =  QString("%1").arg(min_p,req_digit_count,base_value, QChar('0'));

    if(hour.trimmed() == "00")
        return (minute + ":" +second + " Min");
    else
        return (hour + ":" + minute + ":" +second + " Hrs");

}


struct_disk_info imager_helper_standard::get_device_struct_info_according_identifier(QString m_identifier)
{
    if(m_identifier.startsWith("/dev/"))
        m_identifier.remove(0, QString("/dev/").size());

    m_identifier = m_identifier.trimmed();

    QList<struct_disk_info> list_all_disk_info = pub_device_list();

    struct_disk_info m_obj;
    for(int ii = 0; ii < list_all_disk_info.size(); ii++)
    {
        m_obj = list_all_disk_info.at(ii);

        if(m_obj.device_identifier.trimmed() == m_identifier.trimmed())
            return m_obj;
    }

    return m_obj;
}

bool imager_helper_standard::pub_check_is_root_disk(struct_disk_info m_obj)
{
    if(recon_imager_root_disk.trimmed().isEmpty())
        return false;

    struct_disk_info root_disk_obj = get_device_struct_info_according_identifier(recon_imager_root_disk);

    if(root_disk_obj.parent_whole_disk.trimmed() == m_obj.parent_whole_disk.trimmed() && !root_disk_obj.parent_whole_disk.trimmed().isEmpty())
        return true;


    return false;
}


QString imager_helper_standard::create_md5(QString file_path, QString caller_func)
{

    unsigned char c[MD5_DIGEST_LENGTH];
    MD5_CTX md_context;
    int i, bytes;
    unsigned char data[1024];

    QFileInfo info(file_path);
    if(info.isDir())
        return QString("");



    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {

        //        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " target_file.open---FAILED---" + file_path,Q_FUNC_INFO);
        //        recon_static_functions::app_debug(target_file.errorString(),Q_FUNC_INFO);
        return QString("");

    }

    MD5_Init(&md_context);
    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        MD5_Update(&md_context, data, bytes);
    }


    QByteArray ar;
    MD5_Final(c, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        ar.append(c[i]);
    }


    target_file.close();


    return QString(ar.toHex());
}

QString imager_helper_standard::create_sha1(QString file_path, QString caller_func)
{

    unsigned char c[SHA_DIGEST_LENGTH];
    SHA_CTX sha_context;
    int i, bytes;
    unsigned char data[1024];
    QString utf8_file_name = QString::fromUtf8(file_path.toLocal8Bit().data());

    QFileInfo info(file_path);
    if(info.isDir())
        return QString("");

    QFile target_file(file_path);

    if(!target_file.open(QIODevice::ReadOnly))
    {

        //        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " target_file.open---FAILED---" + file_path,Q_FUNC_INFO);
        //        recon_static_functions::app_debug(target_file.errorString(),Q_FUNC_INFO);
        return QString("");
    }
    SHA1_Init(&sha_context);

    while ((bytes = target_file.read((char *)data,1024)) > 0)
        SHA1_Update(&sha_context, data, bytes);


    QByteArray ar;
    SHA1_Final(c, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
        ar.append(c[i]);


    target_file.close();

    return QString(ar.toHex());
}

QString imager_helper_standard::extract_dev_rdisk_from_combobox_item_text(QString str) // /dev/rdisk2s1
{
    if(!str.startsWith("disk"))
        return NONE;

    QStringList str_list = str.split(" ");

    if(str_list.size() < 2)
        return NONE;

    QString disk_name = str_list.at(0);

    if(!disk_name.startsWith("disk"))
        return NONE;

    QString rdisk_name = "/dev/r" + disk_name;

    return rdisk_name;

}

QString imager_helper_standard::extract_dev_disk_node_from_combobox_item_text(QString str) // /dev/disk2s1
{
    if(!str.startsWith("disk"))
        return NONE;

    QStringList str_list = str.split(" ");

    if(str_list.size() < 2)
        return NONE;

    QString disk_name = str_list.at(0);

    if(!disk_name.startsWith("disk"))
        return NONE;

    disk_name = "/dev/" + disk_name;

    return disk_name;

}

QString imager_helper_standard::extract_disk_node_from_combobox_item_text(QString str) // disk2s1
{
    if(!str.startsWith("disk"))
        return NONE;

    QStringList str_list = str.split(" ");

    if(str_list.size() < 2)
        return NONE;

    QString disk_name = str_list.at(0);

    if(!disk_name.startsWith("disk"))
        return NONE;



    return disk_name.trimmed();

}

bool imager_helper_standard::write_data_to_file(QString a_data, QString file_path, int mode)
{
    QFile mfile(file_path);

    bool bool_file_opened = false;

    if(mode == WRITE_WITH_TRUNCATE)
        bool_file_opened = mfile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    else if(mode == WRITE_WITH_APPEND)
        bool_file_opened = mfile.open(QIODevice::ReadWrite | QIODevice::Append);

    if(!bool_file_opened)
        return false;

    mfile.write(a_data.toLocal8Bit());

    mfile.flush();

    mfile.close();

    return true;

}

bool imager_helper_standard::append_file_data_from_another_file(QString first_file_path, QString second_file_path)
{
    QFile first_file(first_file_path);
    QFile second_file(second_file_path);

    if(!first_file.open(QIODevice::ReadWrite | QIODevice::Append))
        return false;

    if(!second_file.open(QIODevice::ReadOnly))
    {
        first_file.close();
        return false;
    }

    QByteArray second_file_data = second_file.readAll();

    second_file.close();

    first_file.write(second_file_data);

    first_file.flush();

    first_file.close();

    return true;
}


QString imager_helper_standard::extract_device_mount_path_using_device_node(QString device_node)
{
    if(device_node.startsWith("/dev"))
        device_node = device_node.remove(QString("/dev/"));

    for(int i = 0; i < list_disk_full_info.size(); i++)
    {
        struct_disk_info obj = list_disk_full_info.at(i);

        if(obj.device_identifier == device_node)
        {
            if(obj.mount_point_MountPoint.isEmpty())
                return NOMOUNT;
            else
                return obj.mount_point_MountPoint;

        }
    }

    return NOMOUNT;
}

QString imager_helper_standard::extract_device_fs_name_using_device_node(QString device_node)
{

    if(device_node.startsWith("/dev/"))
        device_node = device_node.remove(0, QString("/dev/").size());

    for(int i = 0; i < list_disk_full_info.size(); i++)
    {
        struct_disk_info obj = list_disk_full_info.at(i);

        if(obj.device_identifier == device_node)
            return obj.filesystem_FilesystemType;
    }

    return NONE;

}

QList<network_share_info> imager_helper_standard::get_network_share_info()
{
    QList<network_share_info> network_info_list;

    QString mount_output = run_command_without_argument("mount", Q_FUNC_INFO);

    QStringList mount_output_list = mount_output.split("\n");

    QStringList network_share_line_list;

    for(int i = 0 ; i < mount_output_list.size(); i++)
    {
        QString a_str = mount_output_list.at(i);

        if(a_str.contains("smbfs"))
        {
            a_str = a_str.trimmed();
            network_share_line_list << a_str;
        }
    }


    for(int ss = 0 ; ss < network_share_line_list.size() ; ss++)
    {
        QString line_smb = network_share_line_list.at(ss);

        QStringList list_network = line_smb.split(QString(" on "));

        if(list_network.size() < 2)
            continue;

        QString address = list_network.at(0);
        address = address.trimmed();

        QString path = list_network.at(1);

        if(path.endsWith("(smbfs)"))
        {
            path.chop(QString("(smbfs)").size());
            path = path.trimmed();
        }

        if(path.contains("(smbfs"))
        {
            int m_index = path.indexOf("(smbfs");

            path = path.remove(m_index, 100).trimmed();
        }

        if(address.isEmpty() || path.isEmpty())
            continue;

        QString share_name;
        if(address.contains("/"))
        {
            share_name = address;
            int share_index = share_name.lastIndexOf("/");
            share_name = share_name.remove(0,share_index + 1);
        }


        network_share_info obj;
        obj.address = address;
        obj.mount_path = path;
        obj.share_name = share_name;

        network_info_list.append(obj);
    }

    return network_info_list;
}

void imager_helper_standard::pub_set_recon_imager_root_disk(QString disk_name)
{
    recon_imager_root_disk = disk_name;
}

QString imager_helper_standard::pub_get_recon_imager_root_disk()
{
    return recon_imager_root_disk;
}


QString imager_helper_standard::get_network_mount_path_acc_share_name(QString share_name, QString ip_address)
{
    QString mount_output = run_command_without_argument("mount", Q_FUNC_INFO);
    QStringList mount_output_list = mount_output.split("\n");

    QStringList network_share_line_list;
    QString mount_path ;

    for(int i = 0 ; i < mount_output_list.size(); i++)
    {
        QString a_str = mount_output_list.at(i);

        if(a_str.contains("smbfs"))
        {
            a_str = a_str.trimmed();
            network_share_line_list << a_str;
        }
    }


    for(int ss = 0 ; ss < network_share_line_list.size() ; ss++)
    {
        QString line_smb = network_share_line_list.at(ss);

        QStringList list_network = line_smb.split(QString(" on "));
        if(list_network.size() < 2)
            continue;

        QString address = list_network.at(0);
        address = address.trimmed();

        if(!address.contains(share_name , Qt::CaseInsensitive) && !address.contains(ip_address))
            continue;

        QString path = list_network.at(1);
        if(path.endsWith("(smbfs)"))
        {
            path.chop(QString("(smbfs)").size());
            path = path.trimmed();
        }

        if(path.contains("(smbfs"))
        {
            int m_index = path.indexOf("(smbfs");
            path = path.remove(m_index, 100).trimmed();
        }

        if(path.isEmpty())
            continue;

        if(path.contains(share_name, Qt::CaseInsensitive))
            mount_path = path;

    }

    return mount_path;
}

QString imager_helper_standard::get_mount_path_of_network_volume(QString network_path)
{
    QString mount_path = QString(NOMOUNT);
    QList<network_share_info> m_list = get_network_share_info();

    for(int i = 0 ; i < m_list.size(); i++)
    {
        network_share_info obj = m_list.at(i);

        if(obj.address == network_path.trimmed())
        {
            return obj.mount_path;
        }
    }

    return mount_path;
}

qint64 imager_helper_standard::get_used_space_using_disk_node(QString disk_node)
{
    qint64 s_size = 0;

    QStringList args;
    args << "info" << disk_node;
    QString info_str;

    QString cmd_output = run_command_with_arguments("diskutil", args, Q_FUNC_INFO);

    QStringList output_list = cmd_output.split("\n") ;

    for(int ls = 0 ; ls < output_list.size() ; ls++)
    {
        info_str = output_list.at(ls);

        if(info_str.contains("Volume Used Space:"))
        {
            int index_1 = info_str.indexOf("(");

            info_str.remove(0, index_1 + 1);

            int index_2 = info_str.indexOf(" ");

            info_str.remove(index_2, 500);

            s_size =  info_str.toLongLong();

            return s_size;
        }
    }

    return s_size;
}

qint64 imager_helper_standard::get_disk_physical_size(QString device_node)
{
    if(device_node.startsWith("/dev/"))
        device_node = device_node.remove(0, QString("/dev/").size());

    for(int i = 0; i < list_disk_full_info.size(); i++)
    {
        struct_disk_info obj = list_disk_full_info.at(i);

        if(obj.device_identifier == device_node)
            return obj.total_size_qint64_TotalSize;
    }

    return 0;
}

QString imager_helper_standard::recon_imger_data_partition_mount_path(QString other_device_mount_path)
{
    for(int i = 0; i < list_disk_full_info.size(); i++)
    {
        struct_disk_info obj = list_disk_full_info.at(i);

        if(obj.mount_point_MountPoint == other_device_mount_path)
        {

            for(int i_i = 0; i_i < list_disk_full_info.size(); i_i++)
            {
                struct_disk_info obj_obj = list_disk_full_info.at(i_i);

                if(obj_obj.parent_whole_disk == obj.parent_whole_disk && obj_obj.name_VolumeName.contains("RECON Imager Data", Qt::CaseInsensitive))
                {
                    return obj_obj.device_identifier;
                }

            }

        }
    }

    return NONE;
}


QString imager_helper_standard::pub_recon_imger_data_partition_mount_path_during_inital_phase()
{
    for(int i = 0; i < list_disk_full_info.size(); i++)
    {
        struct_disk_info obj = list_disk_full_info.at(i);


        for(int i_i = 0; i_i < list_disk_full_info.size(); i_i++)
        {
            struct_disk_info obj_obj = list_disk_full_info.at(i_i);

            if(obj_obj.name_VolumeName.contains("RECON Imager Data", Qt::CaseInsensitive))
            {
                return obj_obj.mount_point_MountPoint;
            }

        }


    }

    return NONE;
}

void imager_helper_standard::pub_set_recon_imager_data_partition_mount_path(QString str)
{
    recon_imager_data_partition_mount_path = str;
}

QString imager_helper_standard::pub_get_recon_imager_data_partition_mount_path()
{
    return recon_imager_data_partition_mount_path;
}

qint64 imager_helper_standard::get_network_mount_path_used_size(QString mount_path)
{
    if(mount_path.trimmed().isEmpty())
        return 0;


    QStringList val_list;
    val_list << mount_path;

    QString proc_out = run_command_with_arguments_no_trimmed("df", val_list, Q_FUNC_INFO);

    QString size_str;
    QStringList proc_out_split_list = proc_out.split("\n");

    if(proc_out_split_list.size() > 1)
    {
        size_str = proc_out_split_list.at(1);

        proc_out_split_list.clear();
        proc_out_split_list = size_str.split(" ");
        if(proc_out_split_list.size() > 3)
        {
            size_str = proc_out_split_list.at(3);
        }
    }
    size_str = size_str.trimmed();

    qint64 used_size = 0;
    if(!size_str.isEmpty())
        used_size = (size_str.toLongLong() * 512); //static block size

    return used_size;
}

QString imager_helper_standard::pub_get_system_profiler_hardware_info()
{
    QStringList val_list;
    val_list << "SPHardwareDataType";

    QString proc_out = run_command_with_arguments_no_trimmed("/Applications/system_profiler", val_list, Q_FUNC_INFO);

    return proc_out;
}

QString imager_helper_standard::pub_get_system_profiler_software_info()
{
    QStringList val_list;
    val_list << "SPSoftwareDataType";

    QString proc_out = run_command_with_arguments_no_trimmed("/Applications/system_profiler", val_list, Q_FUNC_INFO);

    return proc_out;
}


struct_imager_hashes_with_details imager_helper_standard::pub_create_md5_SHA1_with_details(QString file_path)
{

    struct_imager_hashes_with_details obj;

    unsigned char a_md5[MD5_DIGEST_LENGTH], a_sha1[SHA_DIGEST_LENGTH];
    int i, bytes;
    unsigned char data[1024];

    ///=============MD5===========///
    MD5_CTX md_context;
    MD5_Init(&md_context);


    ///=============SHA1===========///
    SHA_CTX sha_context;
    SHA1_Init(&sha_context);

    ///open file and read data
    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {
        return obj;
    }

    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        MD5_Update(&md_context, data, bytes);
        SHA1_Update(&sha_context, data, bytes);
    }
    target_file.close();


    ///process data to calculate hashes

    ///=============MD5===========///
    QByteArray md5_ar;
    MD5_Final(a_md5, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
        md5_ar.append(a_md5[i]);


    ///=============SHA1===========///
    QByteArray sha1_ar;
    SHA1_Final(a_sha1, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
        sha1_ar.append(a_sha1[i]);

    ///final hash
    QString hashes_str = QString("MD5 : ") + QString::fromLocal8Bit(md5_ar.toHex()) + QString(" SHA1 : ") + QString::fromLocal8Bit(sha1_ar.toHex());

    obj.md5 = QString::fromLocal8Bit(md5_ar.toHex());
    obj.sha1 = QString::fromLocal8Bit(sha1_ar.toHex());
    obj.hash_string = hashes_str;

    return obj;
}

qint64 imager_helper_standard::pub_write_date_into_file(QString file_path, QString our_data, QString caller_func)
{
    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        ///file open error
        return -1;
    }

    qint64 bt_wrtn = file.write(our_data.toLocal8Bit(),our_data.toLocal8Bit().size());

    file.flush();
    file.close();


    return bt_wrtn;
}

bool imager_helper_standard::pub_source_other_than_system_and_data(QString d_node, QList<struct_disk_info> list_disk_full_info)
{
    bool bool_device_node_other_than_system_and_data = false;
    QString system_role_str = QString("System");
    QString Data_role_str = QString("Data");

    if(d_node.startsWith("/dev"))
        d_node = d_node.remove(QString("/dev/"));

    for(int ii = 0; ii < list_disk_full_info.size(); ii++)
    {
        struct_disk_info obj = list_disk_full_info.at(ii);
        if(obj.device_identifier == d_node)
        {

            if(!(obj.apfs_volume_roles_list.contains(system_role_str) || obj.apfs_volume_roles_list.contains(Data_role_str)))
            {
                bool_device_node_other_than_system_and_data = true;
                return true;
            }
            break;
        }
    }
    return false;
}

bool imager_helper_standard::pub_check_is_catalina_drive(struct_disk_info disk_node_obj, QList<struct_disk_info> list_disk_full_info)
{
    ///get all disks of same parent
    QList<struct_disk_info> list_disk_same_parent_info;
    for(int count = 0; count < list_disk_full_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_full_info.at(count);
        if(disk_info_obj.parent_whole_disk == disk_node_obj.parent_whole_disk)
            list_disk_same_parent_info.append(disk_info_obj);
    }

    if(list_disk_same_parent_info.isEmpty())
        return false;

    ///check for role having (System and Empty)
    bool bool_system_role_found = false;
    QString system_role_str = QString("System");
    for(int count = 0; count < list_disk_same_parent_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_same_parent_info.at(count);
        if(disk_info_obj.apfs_volume_roles_list.contains(system_role_str))
        {
            bool_system_role_found = true;
            break;
        }
    }


    if(!bool_system_role_found)
        return false;


    ///check any disk have partition named in Empty role - Data
    QList<struct_disk_info> list_disk_empty_role_info;
    for(int count = 0; count < list_disk_same_parent_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_same_parent_info.at(count);

        if(disk_info_obj.name_VolumeName.contains(" - Data")
                && (disk_info_obj.apfs_volume_roles_list.isEmpty() || disk_info_obj.apfs_volume_roles_list.contains("Data")))
        {
            list_disk_empty_role_info.append(disk_info_obj);
        }
    }

    if(list_disk_empty_role_info.isEmpty())
        return false;

    ///check -Data named drive must not have empty UUID in listUsers
    bool bool_disk_data_found = false;
    for(int count = 0; count < list_disk_empty_role_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_empty_role_info.at(count);
        QStringList apfs_volume_user_id_list = get_apfs_users_id_list(disk_info_obj.device_identifier);
        if(!apfs_volume_user_id_list.isEmpty())
        {
            bool_disk_data_found = true;

            break;
        }
    }

    if(!bool_disk_data_found)
        return false;

    return true;
}

QStringList imager_helper_standard::get_apfs_users_id_list(QString disk_node_str)
{
    disk_node_str = disk_node_str.trimmed();

    QStringList user_id_list;
    if(disk_node_str.isEmpty())
        return user_id_list;

    QStringList args_list;
    args_list << "apfs" << "listUsers" << disk_node_str;

    QProcess process;
    process.start("diskutil", args_list);

    process.waitForFinished();

    QByteArray output_arr = process.readAllStandardOutput();

    QStringList output_split_list = QString::fromLocal8Bit(output_arr, output_arr.size()).split("\n");

    for(int count = 0; count < output_split_list.size(); count++)
    {
        QString tmp_split_str = output_split_list.at(count);
        if(tmp_split_str.startsWith("+--"))
        {
            tmp_split_str.remove("+--");
            tmp_split_str = tmp_split_str.trimmed();

            user_id_list << tmp_split_str;
        }
    }
    return user_id_list;
}

QStringList imager_helper_standard::pub_get_catalina_system_and_data_disk_node_list(struct_disk_info disk_node_obj, QList<struct_disk_info> list_disk_full_info)
{
    /*  Always call 'pub_check_is_catalina_drive'
     *  before calling this function.
     */

    QStringList system_and_data_disk_node_list;

    ///get all disks of same parent
    QList<struct_disk_info> list_disk_same_parent_info;
    for(int count = 0; count < list_disk_full_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_full_info.at(count);
        if(disk_info_obj.parent_whole_disk == disk_node_obj.parent_whole_disk)
            list_disk_same_parent_info.append(disk_info_obj);
    }


    ///check for role having (System and Empty)
    QString system_role_str = QString("System");
    for(int count = 0; count < list_disk_same_parent_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_same_parent_info.at(count);
        if(disk_info_obj.apfs_volume_roles_list.contains(system_role_str))
        {
            system_and_data_disk_node_list << disk_info_obj.device_identifier;
            break;
        }
    }

    ///check any disk have partition named in Empty role - Data
    QList<struct_disk_info> list_disk_empty_role_info;
    for(int count = 0; count < list_disk_same_parent_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_same_parent_info.at(count);

        if(disk_info_obj.name_VolumeName.contains(" - Data")
                && (disk_info_obj.apfs_volume_roles_list.isEmpty() || disk_info_obj.apfs_volume_roles_list.contains("Data")))
            list_disk_empty_role_info.append(disk_info_obj);
    }


    ///check -Data named drive must not have empty UUID in listUsers
    for(int count = 0; count < list_disk_empty_role_info.size(); count++)
    {
        struct_disk_info disk_info_obj = list_disk_empty_role_info.at(count);
        QStringList apfs_volume_user_id_list = get_apfs_users_id_list(disk_info_obj.device_identifier);
        if(!apfs_volume_user_id_list.isEmpty())
        {
            system_and_data_disk_node_list << disk_info_obj.device_identifier;
            break;
        }
    }

    return system_and_data_disk_node_list;
}
