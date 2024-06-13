#include "ios_uid_backup_extractor.h"

ios_uid_backup_extractor::ios_uid_backup_extractor()
{

}

ios_uid_backup_extractor::~ios_uid_backup_extractor()
{

}

void ios_uid_backup_extractor::pub_extract_backup(QString source, QString destination_path)
{
    source_backup_path = source + "/";
    extraction_destination_path = destination_path + "/";

    if(os_version_stripped == "7" || os_version_stripped == "8" || os_version_stripped == "9")
    {
        create_database_table_7_8_9();
        prepare_structure_and_database_7_8_9();
    }
    else if(os_version_stripped == "10" || os_version_stripped == "11"|| os_version_stripped == "12" || os_version_stripped == "13"
            || os_version_stripped == "14" || os_version_stripped == "15" || os_version_stripped == "16" || os_version_stripped == "17")
    {
        create_database_table_10_11();
        prepare_structure_and_database_10_11();
    }
    else
        return;

    backup_spawn_func();
}

void ios_uid_backup_extractor::pub_set_full_os_version(QString version)
{
    version.remove(version.indexOf("."),version.size());
    os_version_stripped = version;
}

void ios_uid_backup_extractor::backup_spawn_func()
{
    recon_static_functions::app_debug("START ", Q_FUNC_INFO);

    QString destination_dbs_file(extraction_destination_path + "mbdb.sqlite");

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_dbs_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("mbdb.sqlite open ---FAILED---" + destination_dbs_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }

    QSqlQuery query_select(destination_db);
    if(!query_select.exec("SELECT fullpath,fileID FROM file_stats"))
    {
        recon_static_functions::app_debug("query_select exe() ---FAILED---", Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + query_select.executedQuery(), Q_FUNC_INFO);
        return;
    }

    int count = 0;

    /// go to first record
    query_select.first();

    // All the directories should be created which we extract from Manifest.mbdb
    //    while(query_select.next())
    //    {
    //        count++;
    //        if(count % 500 == 0)
    //            QCoreApplication::processEvents();

    //        // making  directory
    //        QString file_to_copied = extraction_destination_path + query_select.value(0).toString();
    //        QFileInfo info_file_to_copied(file_to_copied);
    //        QDir dir;
    //        dir.mkpath(info_file_to_copied.absolutePath());

    //        QDirIterator itr(source_backup_path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    //        while(itr.hasNext())
    //        {
    //            /// if it is < 40 >  mean it is not a uuid
    //            QFileInfo myfile(itr.next());
    //            if(myfile.fileName().size() != 40)
    //            {
    //                continue;
    //            }

    //            QString source_file_uuid = myfile.fileName();

    //            // file which does exist in actual backup, should be copied
    //            if(source_file_uuid.trimmed() == query_select.value(1).toString().trimmed())
    //            {
    //                QFile::copy(myfile.filePath() , file_to_copied);
    //                break;
    //            }
    //        } // while source_backup_path ends
    //    }

    while(query_select.next())
    {
        count++;
        if(count % 10 == 0)
            QCoreApplication::processEvents();

        // making  directory
        QString file_to_copied = extraction_destination_path + query_select.value("fullpath").toString();
        QFileInfo info_file_to_copied(file_to_copied);
        QDir dir;
        dir.mkpath(info_file_to_copied.absolutePath());

        QString fileID_str = query_select.value("fileID").toString();

        if(fileID_str.isEmpty())
        {
            continue;
        }

        QString source_files_path = source_backup_path + fileID_str.at(0) + fileID_str.at(1)  + QString("/") + fileID_str;
        source_files_path = source_files_path.trimmed();
        if(!QFileInfo(source_files_path).exists())
        {
            source_files_path = source_backup_path + fileID_str.at(0) + fileID_str.at(1) + fileID_str.at(2)  + QString("/") + fileID_str;
        }

        QFileInfo my_file(source_files_path);
        QString source_file_uuid = my_file.fileName();
        if(source_file_uuid.trimmed() == query_select.value("fileID").toString().trimmed())
        {
            QFile::copy(my_file.filePath() , file_to_copied);
            continue;
        }
    }
    destination_db.close();

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void ios_uid_backup_extractor::file_copier(QString file_to_copied, QString source_id)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QFileInfo info_source_id(source_backup_path + source_id);
    if(!info_source_id.exists())
    {
        recon_static_functions::app_debug("Source UUID File Not Exist -----FAILED-----", Q_FUNC_INFO);
        return;
    }

    QFileInfo info_file_to_copied(file_to_copied);
    QString dir_path = info_file_to_copied.absolutePath();
    QDir dir;
    dir.mkpath(dir_path);

    QFile::copy(info_source_id.filePath() ,  dir_path + "/" + info_file_to_copied.fileName());

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void ios_uid_backup_extractor::create_database_table_7_8_9()
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    ///---Out put Destination
    QString destination_dbs_file(extraction_destination_path + "mbdb.sqlite");
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_dbs_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("destination_dbs_file-----FAILED----- " + destination_dbs_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }

    QSqlQuery create_table(destination_db);

    if(!create_table.exec("CREATE TABLE file_stats"
                          "(INT INTEGER PRIMARY KEY,"
                          "domain VARCHAR(1024),"
                          "filename varchar(1024),"
                          "fullpath varchar(1024),"
                          "fileID varchar(1024),"

                          "linktarget varchar(1024),"
                          "datahash varchar(1024),"
                          "unknown1 varchar(1024),"
                          "mode varchar(1024),"

                          "unknown2 varchar(1024),"
                          "unknown3 varchar(1024),"
                          "userid varchar(1024),"
                          "groupid varchar(1024),"

                          "mtime varchar(1024),"
                          "atime varchar(1024),"
                          "ctime varchar(1024),"
                          "filelen varchar(1024),"

                          "flags varchar(1024),"
                          "numprops  varchar(1024))"))
    {
        recon_static_functions::app_debug("query create_table ---FAILED--- in file db", Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + create_table.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Executes Query " + create_table.executedQuery(), Q_FUNC_INFO);
    }

    destination_db.close();
    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}


void ios_uid_backup_extractor::prepare_structure_and_database_7_8_9()
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString destination_dbs_file( extraction_destination_path + "mbdb.sqlite");

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_dbs_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("destination_dbs_file-----FAILED------" + destination_dbs_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }

    ///- Input Source of Backup
    QFile file(source_backup_path + "Manifest.mbdb");
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Manifest.mbdb open  ---FAILED---" + source_backup_path + "Manifest.mbdb", Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + file.errorString(), Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    file_data.clear();
    file_data = file.readAll();
    file.close();


    QByteArray mbdb;
    mbdb = file_data.mid(0,4);

    if(!mbdb.contains("mbdb"))
    {
        recon_static_functions::app_debug("It is not mbdb file ---FAILED---" + source_backup_path + "Manifest.mbdb", Q_FUNC_INFO);
        destination_db.close();
        return;
    }


    ///start offset
    offset = 0;

    barray_entry_end_index.clear();
    barray_entry_end_index.append(0xFF);

    ///Skipping first 4 bytes i.e mbdb
    offset = 4;

    ///skipping extra 2 bytes
    offset += 2;

    int count = 0;
    while(offset < file_data.size())
    {
        count++;

        if(count % 500 == 0)
            QCoreApplication::processEvents();

        struct struct_mbdb_data_extractor data_contnr;

        /// fill domain
        int_checker_for_mbdb_insertion = 0;
        fill_value_in_mbdb_structure(&data_contnr);

        /// filename
        int_checker_for_mbdb_insertion =1;
        fill_value_in_mbdb_structure(&data_contnr);

        /// fullpath
        if(data_contnr.filename.trimmed() != "")
        {
            data_contnr.fullpath = data_contnr.domain + ("/") + data_contnr.filename;
        }
        else
        {
            data_contnr.fullpath = data_contnr.domain + "/";
        }

        /// fileID
        /// we are using as it is from python script
        QByteArray temprary;
        temprary = data_contnr.domain + QByteArray("-") + data_contnr.filename;

        data_contnr.fileID = QCryptographicHash::hash(temprary,QCryptographicHash::Sha1);


        /// linktarget
        int_checker_for_mbdb_insertion = 2;
        fill_value_in_mbdb_structure(&data_contnr);

        ///  datahash
        int_checker_for_mbdb_insertion = 3;
        fill_value_in_mbdb_structure(&data_contnr);

        /// unknown1
        int_checker_for_mbdb_insertion = 4;
        fill_value_in_mbdb_structure(&data_contnr);

        ///  mode
        int mode = 0;
        bool ok;
        data_contnr.mode = file_data.mid(offset,2);
        mode = data_contnr.mode.toHex().toInt(&ok,16);
        //skipping 2 bytes of mode
        offset += 2;

        data_contnr.unknown2 = file_data.mid(offset,4);                                                            //  unknown2  //
        //skipping 4 unknown bytes
        offset += 4;

        data_contnr.unknown3 = file_data.mid(offset,4);                                                             //  unknown3  //
        //skipping 4 unknown bytes
        offset += 4;

        int uid = 0;
        data_contnr.userid = file_data.mid(offset,4);                                                                //  userid  //
        uid = data_contnr.userid.toHex().toInt(&ok,16);
        //skipping 4 bytes of uid
        offset += 4;

        int gid = 0;
        data_contnr.groupid = file_data.mid(offset,4);                                                               //  groupid  //
        gid = data_contnr.groupid.toHex().toInt(&ok,16);

        //skipping 4 bytes gid
        offset += 4;

        qint64 modi_time = 0;
        data_contnr.mtime = file_data.mid(offset,4);                                                                   //  mtime  //
        modi_time = data_contnr.mtime.toHex().toInt(&ok,16);
        //skipping 4 bytes of modification time
        offset += 4;

        qint64 access_time = 0;
        data_contnr.atime = file_data.mid(offset,4);                                                                   //  atime  //
        access_time = data_contnr.atime.toHex().toInt(&ok,16);
        //skipping 4 bytes of access time
        offset += 4;

        qint64 crtn_time = 0;
        data_contnr.ctime = file_data.mid(offset,4);                                                                   //  ctime  //
        crtn_time = data_contnr.ctime.toHex().toInt(&ok,16);
        //skipping 4 bytes of access time
        offset += 4;

        qint64 file_len = 0;
        data_contnr.filelen = file_data.mid(offset,8);                                                                 //  filelen  //
        file_len = data_contnr.filelen.toHex().toLongLong(&ok,16);
        //skipping 8 bytes of file length
        offset += 8;

        int flag = 0;
        data_contnr.flags = file_data.mid(offset,1);                                                                    //  flags  //
        flag = data_contnr.flags.toHex().toInt(&ok,16);
        //skipping 1 byte of flag
        offset += 1;

        int num_of_props = 0;
        data_contnr.numprops = file_data.mid(offset,1);                                                                //  numprops  //
        num_of_props = data_contnr.numprops.toHex().toInt(&ok,16);
        //skipping 1 byte of number of properties
        offset += 1;

        /// here iterate num_of_props times and get prop name and corresponding prop value
        for(int itr = 0; itr < num_of_props; itr++)
        {
            int_checker_for_mbdb_insertion = 5;
            fill_value_in_mbdb_structure(&data_contnr);

            int_checker_for_mbdb_insertion = 6;
            fill_value_in_mbdb_structure(&data_contnr);
        }

        QSqlQuery query_insert(destination_db);

        query_insert.prepare("insert into file_stats(domain,filename,fullpath,fileID,linktarget,datahash,unknown1,mode,unknown2,unknown3,userid,groupid,mtime,atime,ctime,filelen,flags,numprops) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query_insert.addBindValue(data_contnr.domain);
        query_insert.addBindValue(data_contnr.filename);
        query_insert.addBindValue(data_contnr.fullpath);
        query_insert.addBindValue(data_contnr.fileID.toHex());
        query_insert.addBindValue(data_contnr.linktarget);
        query_insert.addBindValue(data_contnr.datahash.toHex());
        query_insert.addBindValue("empty");
        query_insert.addBindValue(mode);
        query_insert.addBindValue("empty");
        query_insert.addBindValue("empty");
        query_insert.addBindValue(QString::number(uid));
        query_insert.addBindValue(QString::number(gid));
        query_insert.addBindValue(QString::number(modi_time));
        query_insert.addBindValue(QString::number(access_time));
        query_insert.addBindValue(QString::number(crtn_time));
        query_insert.addBindValue(QString::number(file_len));
        query_insert.addBindValue(QString::number(flag));
        query_insert.addBindValue(QString::number(num_of_props));

        if(!query_insert.exec())
        {
            recon_static_functions::app_debug("Insertion Query -----FAILED-----" + query_insert.lastError().text(), Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query_insert.executedQuery(), Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}

void ios_uid_backup_extractor::create_database_table_10_11()
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    ///---Out put Destination
    QString destination_dbs_file(extraction_destination_path + "mbdb.sqlite");
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_dbs_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("destination_dbs_file-----FAILED----- " + destination_dbs_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }

    QSqlQuery create_table(destination_db);
    if(!create_table.exec("CREATE TABLE file_stats"
                          "(INT INTEGER PRIMARY KEY,"
                          "domain VARCHAR(1024),"
                          "filename varchar(1024),"
                          "fullpath varchar(1024),"
                          "fileID varchar(1024),"
                          "flags  varchar(1024))"))
    {
        recon_static_functions::app_debug("query create_table ---FAILED--- in file db", Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + create_table.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + create_table.executedQuery(), Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);

    destination_db.close();
}

void ios_uid_backup_extractor::prepare_structure_and_database_10_11()
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    QString destination_dbs_file( extraction_destination_path + "mbdb.sqlite");
    QString connection_naam_1 = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam_1);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_1);
    destination_db.setDatabaseName(destination_dbs_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("destination_dbs_file-----FAILED------" + destination_dbs_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }

    QSqlQuery insert_into_destination(destination_db);

    ///- Input Source of Backup
    QString source_db_file(source_backup_path + "Manifest.db");

    QString connection_naam_2 = QString(Q_FUNC_INFO) + "2";
    QSqlDatabase::removeDatabase(connection_naam_2);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_2);
    source_db.setDatabaseName(source_db_file);
    if (!source_db.open())
    {
        recon_static_functions::app_debug("source_file_file-----FAILED------ " + source_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text(), Q_FUNC_INFO);
        destination_db.close();
        return ;
    }

    QSqlQuery read_source_db(source_db);
    read_source_db.prepare("SELECT fileID,domain,relativePath,flags from Files");
    if(!read_source_db.exec())
    {
        recon_static_functions::app_debug("query read_source_db ---FAILED--- in file db ", Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + read_source_db.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + read_source_db.executedQuery(), Q_FUNC_INFO);
    }

    while(read_source_db.next())
    {
        QString domain = read_source_db.value(1).toString();
        QString file_name = read_source_db.value(2).toString();

        QString full_path = domain + "/" + file_name;

        insert_into_destination.prepare("INSERT INTO file_stats(domain,filename,fullpath,fileID,flags) VALUES (?,?,?,?,?)");
        insert_into_destination.addBindValue(domain);
        insert_into_destination.addBindValue(file_name);
        insert_into_destination.addBindValue(full_path);
        insert_into_destination.addBindValue(read_source_db.value(0).toString());
        insert_into_destination.addBindValue(read_source_db.value(3).toString());

        if(!insert_into_destination.exec())
        {
            recon_static_functions::app_debug("query insert_into_destination ---FAILED--- in file db ", Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + insert_into_destination.lastError().text(), Q_FUNC_INFO);
            recon_static_functions::app_debug("Executed Query " + insert_into_destination.executedQuery(), Q_FUNC_INFO);
        }
    }

    source_db.close();
    destination_db.close();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void ios_uid_backup_extractor::fill_value_in_mbdb_structure(struct_mbdb_data_extractor *ptr)
{
    recon_static_functions::app_debug("Start ", Q_FUNC_INFO);

    bool value = file_data.mid(offset,1).toHex().contains(barray_entry_end_index.toHex());
    bool value_1 = file_data.mid(offset+1,1).toHex().contains(barray_entry_end_index.toHex());

    /// as it is of python script
    if(value && value_1)
    {
        offset = offset + 2;
        return;
    }

    QByteArray tmp;
    bool ok;

    tmp = file_data.mid(offset,2);

    int len;
    len = tmp.toHex().toInt(&ok,16);

    if(int_checker_for_mbdb_insertion == 0)
    {
        ptr->domain = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }
    else if(int_checker_for_mbdb_insertion == 1)
    {
        ptr->filename = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }
    else if(int_checker_for_mbdb_insertion == 2)
    {
        ptr->linktarget = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }
    else if(int_checker_for_mbdb_insertion == 3)
    {
        ptr->datahash = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }
    else if(int_checker_for_mbdb_insertion == 4)
    {
        ptr->unknown1 = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }
    else if(int_checker_for_mbdb_insertion == 5)
    {
        ptr->propname = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }
    else if(int_checker_for_mbdb_insertion == 6)
    {
        ptr->propval = file_data.mid(offset + 2,tmp.toHex().toInt(&ok,16));
    }

    offset = offset + 2 + len;

    recon_static_functions::app_debug("End ", Q_FUNC_INFO);
}
