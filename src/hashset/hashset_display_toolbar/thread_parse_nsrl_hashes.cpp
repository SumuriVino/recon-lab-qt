#include "thread_parse_nsrl_hashes.h"

thread_parse_nsrl_hashes::thread_parse_nsrl_hashes(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_parse_nsrl_hashes::~thread_parse_nsrl_hashes()
{
}

void thread_parse_nsrl_hashes::pub_set_essentials(QString filepath, QString dest_db_path)
{
    nsrl_filepath = filepath;
    destination_db_filepath = dest_db_path;
}

void thread_parse_nsrl_hashes::run()
{
    bool_cancel_loading = false;

    if(!QFileInfo(nsrl_filepath).exists() || !QFileInfo(destination_db_filepath).exists())
        return;

    QFile file(nsrl_filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ----FAILED----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    qint64 total_file_size = file.size();

    QSqlDatabase hashes_database =QSqlDatabase::addDatabase("QSQLITE");
    hashes_database.setDatabaseName(destination_db_filepath);
    if(!hashes_database.open())
    {
        recon_static_functions::app_debug("DB open ----FAILED----" + hashes_database.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + hashes_database.lastError().text(), Q_FUNC_INFO);
        return;
    }

    hashes_database.transaction();

    QSqlQuery insert_into_hashes(hashes_database) ;

    QString file_data_str;

    QString splitted_data_str;
    QStringList tmp_split_list;

    qint64 records_count = 0;
    qint64 total_read_file_size = 0;
    while(!file.atEnd())
    {
        if(bool_cancel_loading)
            break;

        file_data_str = QString::fromLocal8Bit(file.readLine()).trimmed() ;

        total_read_file_size += file_data_str.size();

        if(file_data_str.contains("\t"))
        {
            tmp_split_list = file_data_str.split("\t");
            for(int count = 0; count < tmp_split_list.size(); count++)
            {
                if(bool_cancel_loading)
                    break;

                splitted_data_str = tmp_split_list.at(count);
                splitted_data_str = splitted_data_str.trimmed();

                if(splitted_data_str.size() == MD5_HASH_UNIVERSAL_LENGTH_BYTES_int)
                {
                    insert_into_hashes.prepare("insert into saved_hashsets(md5_hash) values (?)");
                    insert_into_hashes.addBindValue(splitted_data_str);

                    if(!insert_into_hashes.exec())
                    {
                        recon_static_functions::app_debug("query insert ----FAILED----" + insert_into_hashes.executedQuery(), Q_FUNC_INFO);
                        recon_static_functions::app_debug("Error:" + insert_into_hashes.lastError().text(), Q_FUNC_INFO);
                        continue;
                    }

                    records_count++;

                    if(records_count % 2000 == 0)
                    {
                        qint64 progress_val = ((total_read_file_size * 100) / total_file_size);
                        if(progress_val > 99)
                            progress_val = 99;

                        emit signal_nsrl_parse_status(records_count, progress_val);

                    }
                }
            }
        }
        else
        {
            if(file_data_str.size() == MD5_HASH_UNIVERSAL_LENGTH_BYTES_int)
            {
                insert_into_hashes.prepare("insert into saved_hashsets(md5_hash) values (?)");
                insert_into_hashes.addBindValue(file_data_str);

                if(!insert_into_hashes.exec())
                {
                    recon_static_functions::app_debug("query insert ----FAILED----" + insert_into_hashes.executedQuery(), Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error:" + insert_into_hashes.lastError().text(), Q_FUNC_INFO);
                    continue;
                }

                records_count++;
                if(records_count % 2000 == 0)
                {
                    qint64 progress_val = ((total_read_file_size * 100) / total_file_size);
                    if(progress_val > 99)
                        progress_val = 99;

                    emit signal_nsrl_parse_status(records_count, progress_val);
                }
            }
        }
    }

    emit signal_nsrl_parse_status(records_count, 100);

    file.close();

    hashes_database.commit();

    QString index_cmd = QString("CREATE INDEX hashset_index ON saved_hashsets(md5_hash)");
    recon_helper_standard_obj->execute_db_command_by_dbreference(index_cmd, hashes_database, Q_FUNC_INFO);

    hashes_database.close();

}

void thread_parse_nsrl_hashes::pub_cancel_extraction()
{
    bool_cancel_loading = true;
}
