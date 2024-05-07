#include "thread_file_copier_module.h"

thread_file_copier_module::thread_file_copier_module(QObject *parent) :
    QThread(parent)
{
    stop_thread = false;

    recon_helper_standard_obj = new recon_helper_standard(this);
    export_rsync_obj = new export_rsync(this);
}

void thread_file_copier_module::run()
{ // run thread and file copy start here to export
    stop_thread = false;

    int copy_counter = 0;

    for(int i = 0; i < src_list.size(); i++)
    {
        copy_counter++;
        if(copy_counter % 50 == 0)
        {
            copy_counter = 0;
            QCoreApplication::processEvents();
        }

        if(stop_thread)
        {
            return;
        }

        QString src_path = src_list.at(i);
        src_path = src_path.trimmed();
        QString dest_path = dst_list.at(i);
        dest_path = dest_path.trimmed();


        QString rec_no = rec_no_list.at(i);
        rec_no = rec_no.trimmed();
        QString table_name = table_name_list.at(i);
        table_name = table_name.trimmed();

        QString command_os_scheme = "SELECT os_scheme_display FROM '" + table_name + "' WHERE INT = ?";

        QStringList values_list;
        values_list << rec_no;
        QFileInfo info(src_path);

        QFile source_file(src_path);
        QFile dest_file(dest_path);
        if(source_file.exists())
        {
            QString proc_cmd = "rsync";
            QStringList arg_list;
            if(info.isDir())
            {
                QString source_path = src_path;
                arg_list << "-a" << source_path.append("/.") << dest_path;
            }
            else
                arg_list << "-a" << src_path << dest_path;

            QProcess proc_rsync ;

            proc_rsync.start(proc_cmd, arg_list);

            if(!proc_rsync.waitForStarted())
            {
                recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
                recon_static_functions::app_debug("Error:- " + proc_rsync.errorString() ,Q_FUNC_INFO);
            }

            int m_cnt = 0;

            while(1)
            {
                m_cnt++;
                if(m_cnt % 10 == 0)
                {
                    m_cnt = 0;
                    QCoreApplication::processEvents();
                }

                if(proc_rsync.state() != QProcess::Running)
                {
                    proc_rsync.terminate();
                    break;
                }

            }

            ///-Create Softlink of the destination file to open in preview
            QString os_schme_intrnl = recon_helper_standard_obj->get_target_os_scheme_internal_on_current_record_source(rec_no,table_name,destination_db_file,Q_FUNC_INFO);

            if(os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                    || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
            {
                if(softlink_filepath_list.size() > i)
                {
                    QString soft_dest_path = dest_path;

                    soft_dest_path = QString("..") + soft_dest_path.remove(0, soft_dest_path.lastIndexOf("/"));

                    QFile::link(soft_dest_path,softlink_filepath_list.at(i).trimmed());
                }
            }
        }

    }
}

void thread_file_copier_module::set_source_list(QStringList list)
{
    src_list = list;
}

void thread_file_copier_module::set_destination_list(QStringList list)
{
    dst_list = list;
}

void thread_file_copier_module::set_tablename(QStringList table_name)
{
    table_name_list = table_name;
}

void thread_file_copier_module::set_record_no(QStringList rec)
{
    rec_no_list = rec;
}

void thread_file_copier_module::set_softlink_filepath_list(QStringList list)
{
    softlink_filepath_list = list;
}

void thread_file_copier_module::set_destination_db(QString path)
{
    destination_db_file = path;
}

void thread_file_copier_module::pub_stop_thread()
{
    stop_thread = true;
}


