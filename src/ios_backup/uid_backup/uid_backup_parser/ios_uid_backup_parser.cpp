#include "ios_uid_backup_parser.h"

ios_uid_backup_parser::ios_uid_backup_parser(QObject *parent) :
    QObject(parent)
{

}

void ios_uid_backup_parser::pub_set_backup_path(QString path)
{
    ios_uid_backup_path = path;

    if(!ios_uid_backup_path.isEmpty() && !ios_uid_backup_path.endsWith("/"))
        ios_uid_backup_path.append("/");

}

void ios_uid_backup_parser::pub_set_os_version(QString version)
{
    version.remove(version.indexOf("."),version.size());
    os_version_stripped = version;
}

void ios_uid_backup_parser::pub_parse_backup()
{
    if(os_version_stripped == "7" || os_version_stripped == "8" || os_version_stripped == "9")
    {
        list_node_entry_for_ios_7_8_9.clear();
        start_extraction_7_8_9();
    }
    else if(os_version_stripped == "10" || os_version_stripped == "11" || os_version_stripped == "12" || os_version_stripped == "13"

            || os_version_stripped == "14" || os_version_stripped == "15"  || os_version_stripped == "16" || os_version_stripped == "17")
    {
        list_node_entry_for_ios_10_11.clear();
        start_extraction_10_11();
    }
}

QString ios_uid_backup_parser::pub_get_artifact_source_file_uid_path(QString file_path_from_domain)
{
    recon_static_functions::debug_intensive("START ",Q_FUNC_INFO);

    QString id_path;

    int indx_strt = 0;
    indx_strt = file_path_from_domain.indexOf(QString("/"),0);

    QString domain_name = file_path_from_domain.mid(0,indx_strt);
    QString filepath_without_domain = file_path_from_domain.mid(indx_strt + 1, file_path_from_domain.size() - indx_strt);

    if(os_version_stripped == "7" || os_version_stripped == "8" || os_version_stripped == "9")
    {
        for(int itr = 0; itr < list_node_entry_for_ios_7_8_9.size(); itr++)
        {
            struct_global_main_node_entry_for_ios_7_8_9 hldr = list_node_entry_for_ios_7_8_9.at(itr);

            if(hldr.node_domain == (domain_name.toLocal8Bit()))
            {
                for(int iter = 0; iter < hldr.list_subdomain.size(); iter++)
                {
                    struct struct_global_sub_node_entry_for_ios_7_8_9 ptr = hldr.list_subdomain.at(iter);

                    if(ptr.sub_filename == (filepath_without_domain.toLocal8Bit()))
                    {
                        id_path =  ios_uid_backup_path + QString::fromLocal8Bit(ptr.sub_fileID);

                        return id_path;
                    }
                }
            }
        }
    }
    else if(os_version_stripped == "10" || os_version_stripped == "11" || os_version_stripped == "12" || os_version_stripped == "13"
            || os_version_stripped == "14" || os_version_stripped == "15" || os_version_stripped == "16" || os_version_stripped == "17")
    {
        for(int i = 0; i < list_node_entry_for_ios_10_11.size(); i++)
        {
            if((list_node_entry_for_ios_10_11.at(i).domain_name == domain_name.toLocal8Bit())  &&  (list_node_entry_for_ios_10_11.at(i).file_absolutepath_without_domain == filepath_without_domain.toLocal8Bit()))
            {

                id_path =  ios_uid_backup_path + QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_id);
                if(!QFileInfo(id_path).exists())
                {
                    QString dir_name = QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_id);
                    dir_name = dir_name.mid(0,2);

                    id_path =  ios_uid_backup_path + dir_name + "/" +  QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_id);
                }


                return id_path;
            }
        }
    }

    recon_static_functions::debug_intensive("END ",Q_FUNC_INFO);
    return id_path;
}

void ios_uid_backup_parser::start_extraction_7_8_9()
{
    recon_static_functions::app_debug("START ", Q_FUNC_INFO);

    QString source_file = ios_uid_backup_path + "Manifest.mbdb";


    QFile file(source_file);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Manifest.mbdb open -------FAILED-------" + source_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file_data.clear();
    file_data = file.readAll();
    file.close();

    QByteArray mbdb;
    mbdb = file_data.mid(0,4);

    if(!mbdb.contains("mbdb"))
    {
        recon_static_functions::app_debug("It is not a mbdb file  -------FAILED-------" + source_file, Q_FUNC_INFO);
        return;
    }

    barray_entry_end_index.clear();
    barray_entry_end_index.append(0xFF);

    //start offset
    offset = 0;
    //skipping first 4 bytes containig mbdb
    offset = 4;
    //skipping 2 unknown bytes
    offset += 2;

    int count = 0;
    while(offset < file_data.size())
    {
        count++;
        if(count % 500 == 0)
            QCoreApplication::processEvents();

        /// fill domain
        int_checker_for_mbdb_insertion = 0;
        struct struct_mbdb_data data_contnr;
        fill_value_in_mbdb_structure(&data_contnr);

        /// filename
        int_checker_for_mbdb_insertion = 1;
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

        int_checker_for_mbdb_insertion = 2;
        fill_value_in_mbdb_structure(&data_contnr);                                                  // linktarget //

        int_checker_for_mbdb_insertion = 3;
        fill_value_in_mbdb_structure(&data_contnr);                                                 //  datahash  //

        int_checker_for_mbdb_insertion = 4;
        fill_value_in_mbdb_structure(&data_contnr);                                                 // unknown1  //


        int mode = 0;
        bool ok;
        data_contnr.mode = file_data.mid(offset,2);                                                        //  mode  //
        mode = data_contnr.mode.toHex().toInt(&ok,16);

        //skipping 2 bytes of mode
        offset += 2;

        data_contnr.unknown2 = file_data.mid(offset,4);                                                     //  unknown2  //
        //skipping 4 unknown bytes
        offset += 4;


        data_contnr.unknown3 = file_data.mid(offset,4);                                                    //  unknown3  //
        //skipping 4 unknown bytes
        offset += 4;

        int uid;
        data_contnr.userid = file_data.mid(offset,4);                                                      //  userid  //
        uid = data_contnr.userid.toHex().toInt(&ok,16);
        //skipping 4 bytes of user id
        offset += 4;

        int gid;
        data_contnr.groupid = file_data.mid(offset,4);                                                     //  groupid  //
        gid = data_contnr.groupid.toHex().toInt(&ok,16);
        //skipping 4 bytes of group id
        offset += 4;

        qint64 modi_time = 0;
        data_contnr.mtime = file_data.mid(offset,4);                                                        //  mtime  //
        modi_time = data_contnr.mtime.toHex().toInt(&ok,16);
        //skipping 4 bytes of modification time
        offset += 4;

        qint64 access_time = 0;
        data_contnr.atime = file_data.mid(offset,4);                                                        //  atime  //
        access_time = data_contnr.atime.toHex().toInt(&ok,16);
        //skipping 4 bytes of access time
        offset += 4;

        qint64 crtn_time = 0;
        data_contnr.ctime = file_data.mid(offset,4);                                                        //  ctime  //
        crtn_time = data_contnr.ctime.toHex().toInt(&ok,16);
        //skipping 4 bytes of creation time
        offset += 4;

        qint64 file_len = 0;
        data_contnr.filelen = file_data.mid(offset,8);                                                     //  filelen  //
        file_len = data_contnr.filelen.toHex().toLongLong(&ok,16);
        //skipping 8 bytes of file length
        offset += 8;

        int flag = 0;
        data_contnr.flags = file_data.mid(offset,1);                                                        //  flags  //
        flag = data_contnr.flags.toHex().toInt(&ok,16);
        //skipping 1 byte of flag
        offset += 1;

        int num_of_props = 0;
        data_contnr.numprops = file_data.mid(offset,1);                                                    //  numprops  //
        num_of_props = data_contnr.numprops.toHex().toInt(&ok,16);
        //skipping 1 byte of number of properties
        offset += 1;


        // here iterate num_of_props times and get prop name and corresponding prop value //
        for(int itr = 0; itr < num_of_props; itr++)
        {
            int_checker_for_mbdb_insertion = 5;
            fill_value_in_mbdb_structure(&data_contnr);

            int_checker_for_mbdb_insertion = 6;
            fill_value_in_mbdb_structure(&data_contnr);
        }

        insert_into_root_node(data_contnr);
    }

    recon_static_functions::app_debug("END ", Q_FUNC_INFO);
}

void ios_uid_backup_parser::start_extraction_10_11()
{
    recon_static_functions::app_debug("START ", Q_FUNC_INFO);

    QString m_path = ios_uid_backup_path + "Manifest.db";

    QString source_db_file = global_recon_helper_singular_class_obj->get_adhoc_path_for_database(m_path);

    QSqlDatabase::removeDatabase(this->metaObject()->className()+QString(__FUNCTION__));
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", this->metaObject()->className()+QString(__FUNCTION__));
    source_db.setDatabaseName(source_db_file);

    if (!source_db.open())
    {
        recon_static_functions::app_debug("Source_file_file-----FAILED------" + source_db_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }

    QSqlQuery read_source_db(source_db);
    read_source_db.prepare("SELECT fileID,domain,relativePath,flags,file from Files");
    if(!read_source_db.exec())
    {
        recon_static_functions::app_debug("query read_source_db ---FAILED--- in file db " + source_db_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("Erro " + read_source_db.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Querey " + read_source_db.executedQuery(), Q_FUNC_INFO);
        source_db.close();
        return;
    }

    int count = 0;
    while(read_source_db.next())
    {
        if(count % 500 == 0)
            QCoreApplication::processEvents();

        count++;

        struct struct_mbdb_data data_contnr;

        data_contnr.fileID = read_source_db.value(0).toByteArray();
        data_contnr.domain = read_source_db.value(1).toByteArray();
        data_contnr.filename = read_source_db.value(2).toByteArray();
        data_contnr.flags = read_source_db.value(3).toByteArray();
        data_contnr.datahash = read_source_db.value(4).toByteArray();

        insert_into_root_node(data_contnr);
    }

    source_db.close();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void ios_uid_backup_parser::fill_value_in_mbdb_structure(struct struct_mbdb_data *ptr)
{
    recon_static_functions::debug_intensive("Start ", Q_FUNC_INFO);

    bool value = file_data.mid(offset,1).toHex().contains(barray_entry_end_index.toHex());
    bool value_1 = file_data.mid(offset+1,1).toHex().contains(barray_entry_end_index.toHex());

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

    recon_static_functions::debug_intensive("End ", Q_FUNC_INFO);
}

void ios_uid_backup_parser::insert_into_root_node(struct struct_mbdb_data data_contnr)
{
    recon_static_functions::debug_intensive("START ", Q_FUNC_INFO);

    if(os_version_stripped == "7" || os_version_stripped == "8" || os_version_stripped == "9")
    {
        if(list_node_entry_for_ios_7_8_9.size() == 0)
        {
            struct_global_main_node_entry_for_ios_7_8_9 var;
            var.node_domain = data_contnr.domain;
            var.node_filename = data_contnr.filename;
            var.node_fileID = data_contnr.fileID.toHex();
            var.flags = data_contnr.flags;

            qint64 file_len = 0;
            bool ok = false;
            file_len = data_contnr.filelen.toHex().toLongLong(&ok,16);
            var.file_size = file_len;

            list_node_entry_for_ios_7_8_9.append(var);
        }
        else if(list_node_entry_for_ios_7_8_9.size() > 0)
        {
            bool bool_nodedomain_already_exist = false;

            for(int iterator = 0; iterator < list_node_entry_for_ios_7_8_9.size(); iterator++)
            {
                struct_global_main_node_entry_for_ios_7_8_9 hldr = list_node_entry_for_ios_7_8_9.at(iterator);
                if(hldr.node_domain.trimmed() == data_contnr.domain.trimmed())
                {
                    // Add children if domain is same. //
                    struct struct_global_sub_node_entry_for_ios_7_8_9 var;
                    var.sub_domain = data_contnr.domain;
                    var.sub_filename = data_contnr.filename;
                    var.sub_fileID = data_contnr.fileID.toHex();

                    qint64 file_len = 0;
                    bool ok = false;
                    file_len = data_contnr.filelen.toHex().toLongLong(&ok,16);
                    var.sub_file_size = file_len;

                    hldr.list_subdomain.append(var);
                    hldr.flags = data_contnr.flags;

                    list_node_entry_for_ios_7_8_9.removeAt(iterator);
                    list_node_entry_for_ios_7_8_9.insert(iterator,hldr);

                    bool_nodedomain_already_exist = true;
                    break;
                }
            }

            if(!bool_nodedomain_already_exist)
            {
                // add the new node domain. //
                struct_global_main_node_entry_for_ios_7_8_9 var;
                var.node_domain = data_contnr.domain;
                var.node_filename = data_contnr.filename;
                var.node_fileID = data_contnr.fileID.toHex();
                var.flags = data_contnr.flags;

                qint64 file_len = 0;
                bool ok = false;
                file_len = data_contnr.filelen.toHex().toLongLong(&ok,16);
                var.file_size = file_len;

                list_node_entry_for_ios_7_8_9.append(var);
            }
        }
    }
    else if(os_version_stripped == "10" || os_version_stripped == "11" || os_version_stripped == "12" || os_version_stripped == "13"
            || os_version_stripped == "14" || os_version_stripped == "15" || os_version_stripped == "16" || os_version_stripped == "17")
    {
        node_entry_for_ios_10_11.domain_name = data_contnr.domain;
        node_entry_for_ios_10_11.file_absolutepath_without_domain = data_contnr.filename;
        node_entry_for_ios_10_11.file_id = data_contnr.fileID;

        node_entry_for_ios_10_11.datahash = data_contnr.datahash;
        node_entry_for_ios_10_11.flags = data_contnr.flags;

        list_node_entry_for_ios_10_11.append(node_entry_for_ios_10_11);
    }

    recon_static_functions::debug_intensive("END ", Q_FUNC_INFO);
}


QStringList ios_uid_backup_parser::pub_get_artifact_source_file_from_directory_scan(QString received_domain_name)
{
    QStringList uid_filename;

    if(os_version_stripped == "7" || os_version_stripped == "8" || os_version_stripped == "9")
    {
        recon_static_functions::app_debug("START ", Q_FUNC_INFO);

        for(int itr = 0; itr < list_node_entry_for_ios_7_8_9.size(); itr++)
        {
            struct_global_main_node_entry_for_ios_7_8_9 hldr = list_node_entry_for_ios_7_8_9.at(itr);
            if(hldr.node_domain == received_domain_name.toLocal8Bit()){

                for(int iter = 0; iter < hldr.list_subdomain.size(); iter++)
                {
                    struct struct_global_sub_node_entry_for_ios_7_8_9 ptr = hldr.list_subdomain.at(iter);

                    QString ret_string = QString::fromLocal8Bit(ptr.sub_fileID) + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple) + QString::fromLocal8Bit(ptr.sub_filename);
                    uid_filename << ret_string;
                }

                return uid_filename;
            }
        }
    }
    else if(os_version_stripped == "10" || os_version_stripped == "11" || os_version_stripped == "12" || os_version_stripped == "13"
            || os_version_stripped == "14" || os_version_stripped == "15" || os_version_stripped == "16" || os_version_stripped == "17")
    {
        recon_static_functions::app_debug("START ", Q_FUNC_INFO);

        for(int i = 0; i < list_node_entry_for_ios_10_11.size(); i++)
        {
            if(received_domain_name.toLocal8Bit() == list_node_entry_for_ios_10_11.at(i).domain_name)
            {
                QString uid_file_path = ios_uid_backup_path  + QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_id);

                if(!QFileInfo(uid_file_path).exists())
                {
                    QString dir_name = QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_id);
                    dir_name = dir_name.mid(0,2);
                    uid_file_path = ios_uid_backup_path + dir_name + "/" + QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_id);
                }

                if(uid_file_path.startsWith(ios_uid_backup_path))
                    uid_file_path.remove(0,ios_uid_backup_path.size());

                QString  ret_string = uid_file_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple) + QString::fromLocal8Bit(list_node_entry_for_ios_10_11.at(i).file_absolutepath_without_domain);

                uid_filename << ret_string;

            }
        }

        return uid_filename;
    }

    recon_static_functions::app_debug("END ", Q_FUNC_INFO);
    return uid_filename;
}

