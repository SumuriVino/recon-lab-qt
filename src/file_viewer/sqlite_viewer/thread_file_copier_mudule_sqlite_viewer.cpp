#include "thread_file_copier_module_sqlite_viewer.h"

thread_file_copier_module_sqlite_viewer::thread_file_copier_module_sqlite_viewer(QWidget *parent) :
    QThread(parent)
{
    stop_thread = false;
}

void thread_file_copier_module_sqlite_viewer::run()
{
    stop_thread = false;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db;
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("destination_db open ----FAILED---" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_update_hash(destination_db);


    bool check_is_dir = false;
    for(int i = 0; i < src_list.size(); i++)
    {
        if(stop_thread)
        {
           destination_db.close();
           return;
        }

        QString src_path = src_list.at(i).trimmed();
        QString dest_path = dst_list.at(i).trimmed();
        QString rec_no = rec_no_list.at(i).trimmed();
        QString table_name = table_name_list.at(i).trimmed();


        if(src_path.isEmpty())
            continue;

        QFileInfo info(src_path);
        if(info.isDir())
        {
            recon_static_functions::copy_directory(src_path,dest_path, Q_FUNC_INFO);
            check_is_dir = true;
        }
        else if(info.isFile())
        {
            QFile source_file(src_path);
            QFile dest_file(dest_path);
            if(source_file.exists())
            {
                if(!source_file.open(QIODevice::ReadOnly))
                {
                    recon_static_functions::app_debug(" source_file.open  ----FAILED---" + src_path,Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error " + source_file.errorString(),Q_FUNC_INFO);
                    continue;
                }
                if(!dest_file.open(QIODevice::WriteOnly))
                {
                    recon_static_functions::app_debug("dest_file.open  ----FAILED---" + dest_path,Q_FUNC_INFO);
                    recon_static_functions::app_debug(" eroor " + dest_file.errorString(),Q_FUNC_INFO);
                    source_file.close();
                    continue;
                }

                recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
                while(1)
                {
                    QByteArray dar = source_file.read(102400);
                    if(dar.size() <= 0)
                    {
                        source_file.close();
                        dest_file.close();
                        break;
                    }
                    if(stop_thread)
                    {
                        source_file.close();
                        dest_file.close();
                       destination_db.close();
                       return;
                    }

                    dest_file.write(dar);
                }
                recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                ///-Create Softlink of the destination file to open in preview
//                QString os_naming_scheme = global_recon_555_helper_singular_class_obj->get_target_os_naming_scheme_on_current_record_source(rec_no,table_name,destination_db_file,Q_FUNC_INFO);
//                if(os_naming_scheme == QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS))
//                {
//                    if(softlink_filepath_list.size() > i)
//                        QFile::link(dest_path,softlink_filepath_list.at(i).trimmed());
//                }
            }
        }




        QString old_md5,old_sha1;

        QSqlQuery query_check_hash(destination_db);

        QString command = QString("select exp_hashmd5,exp_hashsha1 from exportfilelist where (table_name = ? AND record = ? AND filepath = ?)");
        query_check_hash.prepare(command);
        query_check_hash.addBindValue(table_name);
        query_check_hash.addBindValue(rec_no);
        query_check_hash.addBindValue(src_path);
        if(!query_check_hash.exec())
        {
            recon_static_functions::app_debug("query_check_hash  ----FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error " + query_check_hash.lastError().text(),Q_FUNC_INFO);
        }
        if(query_check_hash.next())
        {
            old_md5 = query_check_hash.value(0).toString().trimmed();
            old_sha1 = query_check_hash.value(1).toString().trimmed();
        }

        if(old_md5.isEmpty() || old_sha1.isEmpty())
        {
            QString cmd = QString("update exportfilelist set exp_hashmd5=?, exp_hashsha1=? where (table_name = ? AND record = ? AND filepath = ?)");
            query_update_hash.prepare(cmd);
            if(check_is_dir)
            {
                query_update_hash.addBindValue(QObject::tr("Folder"));
                query_update_hash.addBindValue(QObject::tr("Folder"));
            }
            else
            {
                QString md5_string = recon_static_functions::create_md5(src_path, Q_FUNC_INFO);
                QString sha1_string = recon_static_functions::create_sha1(src_path, Q_FUNC_INFO);

                query_update_hash.addBindValue(md5_string);
                query_update_hash.addBindValue(sha1_string);
            }
            query_update_hash.addBindValue(table_name);
            query_update_hash.addBindValue(rec_no);
            query_update_hash.addBindValue(src_path);

            if(!query_update_hash.exec())
            {
                recon_static_functions::app_debug("query_update_hash ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error " + query_check_hash.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug("source path " + src_path,Q_FUNC_INFO);
            }

            check_is_dir = false;

        }

    }

    destination_db.close();

}

void thread_file_copier_module_sqlite_viewer::set_source_list(QStringList list)
{
    src_list = list;
}

void thread_file_copier_module_sqlite_viewer::set_destination_list(QStringList list)
{
    dst_list = list;
}

void thread_file_copier_module_sqlite_viewer::set_tablename(QStringList table_name)
{
    table_name_list = table_name;
}

void thread_file_copier_module_sqlite_viewer::set_record_no(QStringList rec)
{
    rec_no_list = rec;
}

void thread_file_copier_module_sqlite_viewer::set_softlink_filepath_list(QStringList list)
{
    softlink_filepath_list = list;
}


void thread_file_copier_module_sqlite_viewer::set_destination_db(QString path)
{
    destination_db_file = path;
}

void thread_file_copier_module_sqlite_viewer::pub_stop_thread()
{
    stop_thread = true;
}


