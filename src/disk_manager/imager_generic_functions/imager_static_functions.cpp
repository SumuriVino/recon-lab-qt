#include "imager_static_functions.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>


imager_static_functions::imager_static_functions(QWidget *parent)
{

}

//These are imager generic classes which we rarely using in LAB. These class are just for future references. Some of functions we use for disk manager only.

QString imager_static_functions::human_readable_size(qint64 size_in_qint64, QString caller_func)
{

    qint64 size_in_int;

    size_in_int = size_in_qint64;

    if(size_in_int <= MACRO_Generic_Value_KB_int - 1)
    {
        return QString::number(size_in_int) + " B";
    }

    if(size_in_int >= MACRO_Generic_Value_KB_int && size_in_int <= ((MACRO_Generic_Value_MB_int ) - 1))
    {
        double size_double = size_in_int / (1024.0);

        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " KB";
    }

    if(size_in_int >= (MACRO_Generic_Value_MB_int) && size_in_int <= ((MACRO_Generic_Value_GB_int ) - 1))
    {
        double size_double = size_in_int / (1024.0 * 1024.0);
        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " MB";

    }

    if(size_in_int >= (MACRO_Generic_Value_GB_int))
    {
        double size_double = size_in_int / (1024.0 * 1024.0 * 1024.0);
        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " GB";
    }


    return QString("0 KB");
}

QString imager_static_functions::get_available_filename(QString file_name, QString dir_path, QString caller_func)
{
    QDir dir;
    dir.mkpath(dir_path);

    int count = 1;
    QString file_tmp = file_name;

    while(1)
    {
        if(is_file_exist_in_dir(dir_path,file_tmp, Q_FUNC_INFO))
        {
            file_tmp = QString::number(count) + "_" + file_name;
            count++;
            continue;
        }
        break;
    }

    return file_tmp;
}

QString imager_static_functions::get_available_or_existing_dirname(QString dir_name, QString dir_path, QString caller_func)
{
    QDir dir;
    dir.mkpath(dir_path);

    int count = 1;
    QString dir_tmp = dir_name;

    QString full_dir_path = dir_path + "/" + dir_tmp;

    QFileInfo m_info(full_dir_path);

    if(m_info.exists())
    {
        if(m_info.isDir())
            return dir_tmp;
    }


    while(1)
    {
        if(is_file_exist_in_dir(dir_path,dir_tmp, Q_FUNC_INFO))
        {
            dir_tmp = QString::number(count) + "_" + dir_name;
            count++;
            continue;
        }
        break;
    }

    return dir_tmp;
}

bool imager_static_functions::is_file_exist_in_dir(QString dir_path, QString filename, QString caller_func)
{
    QDirIterator it(dir_path); // Never ever use any diriterator flags here.
    while(it.hasNext())
    {
        QFileInfo info(it.next());
        if(info.fileName().trimmed() == filename.toLower().trimmed())
        {
            return true;
        }
    }

    return false;
}



bool imager_static_functions::execute_db_command_by_dbpath(QString command, QString destination_db_file, QString caller_func)
{

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        //  recon_static_functions::app_debug(prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        //recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);

        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return false;
    }

    QSqlQuery query(*destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        // recon_static_functions::app_debug(prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
        //recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        // recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);

        if(destination_db != NULL)
            delete destination_db;
        destination_db->close();
        QSqlDatabase::removeDatabase(connection_naam);
        return false;
    }

    if(destination_db != NULL)
        delete destination_db;
    destination_db->close();
    QSqlDatabase::removeDatabase(connection_naam);

    return true;
}


bool imager_static_functions::execute_db_command_with_addbindvalues_by_dbpath(QString command, QStringList values, QString destination_db_file, QString caller_func)
{
    //  recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        //  recon_static_functions::app_debug(prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
       // recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);


        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return false;
    }

    QSqlQuery query(*destination_db);
    query.prepare(command);
    for(int i = 0; i < values.size(); i++)
    {
        query.addBindValue(values.at(i));
    }
    if(!query.exec())
    {
        //   recon_static_functions::app_debug(prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
        //    recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        // recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);


        if(destination_db != NULL)
            delete destination_db;
        destination_db->close();
        QSqlDatabase::removeDatabase(connection_naam);
        return false;
    }
    if(destination_db != NULL)
        delete destination_db;
    destination_db->close();
    QSqlDatabase::removeDatabase(connection_naam);

    // recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return true;
}
