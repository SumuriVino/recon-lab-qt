#include "thread_carve_data.h"

thread_carve_data::thread_carve_data(QWidget *parent)
{
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard(this);

}

thread_carve_data::~thread_carve_data()
{

}

void thread_carve_data::pub_set_process_obj(QProcess *obj)
{
    process_carve_data = obj;
}

void thread_carve_data::pub_cancel_extraction(bool status)
{
    bool_cancel_extraction = status;
}

void thread_carve_data::pub_set_essentials(QString src_cnt_name, QString source_plugin_name_str, QString source_tab_name_str, QString source_record_no_str, QString source_file_path_str)
{
    source_plugin_name = source_plugin_name_str;
    source_tab_name  = source_tab_name_str;
    source_record_no = source_record_no_str;
    source_filepath  = source_file_path_str;
    source_filename  = QFileInfo(source_filepath).fileName();
    source_count_name = src_cnt_name;

    if(source_plugin_name_str ==  QString(MACRO_Plugin_Name_File_System))
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
        QString cmd = QString("Select filepath,recon_filefrom,recon_file_infopath from files Where INT = ?");

        source_filepath_for_display = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(source_record_no_str), 0, fs_db_path, Q_FUNC_INFO);
        recon_file_from = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(source_record_no_str), 1, fs_db_path, Q_FUNC_INFO);
        recon_file_infopath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(source_record_no_str), 2, fs_db_path, Q_FUNC_INFO);
    }
    else if(source_plugin_name_str ==  QString(MACRO_Plugin_Name_RAM_Images))
    {
        QString ram_images_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString).toString() + QString("ram_images.sqlite");
        QString cmd = QString("Select source_path,recon_filefrom,recon_file_infopath from tbl_ram_list Where INT = ?");

        source_filepath_for_display = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(source_record_no_str), 0, ram_images_db_path, Q_FUNC_INFO);
        recon_file_from = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(source_record_no_str), 1, ram_images_db_path, Q_FUNC_INFO);
        recon_file_infopath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList(source_record_no_str), 2, ram_images_db_path, Q_FUNC_INFO);

    }
}

void thread_carve_data::run()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    st_source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    emit signal_PBAR_thread_carved_data_started(MACRO_JobType_Carved_Data);

    emit signal_PBAR_thread_carve_data_processing_value(MACRO_JobType_Carved_Data, source_filename, true, MACRO_Generic_Progress_No_Records_int, 0,false);

    bool_cancel_extraction = false;

    QString output_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Data_In_Result_QString).toString();
    QDir dir_out(output_str);
    if(dir_out.exists())
    {
        ///remove tmp dir
        recon_static_functions::do_directory_blank_completely(output_str, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(output_str, Q_FUNC_INFO);
    }
    dir_out.mkdir(output_str);

    QStringList args_list;
    args_list << "-o" << output_str << source_filepath;

    process_carve_data->start("./bulk_extractor", args_list);
    if (!process_carve_data->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + process_carve_data->errorString() ,Q_FUNC_INFO);
        emit signal_PBAR_thread_carved_data_finished(MACRO_JobType_Carved_Data);
        return;
    }

    ///forcing thread to wait and read when data available from process.
    int count = 0;
    while(1)
    {
        count++;
        if(count % 100 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(process_carve_data->state() == QProcess::NotRunning || bool_cancel_extraction)
            break;

        process_carve_data->waitForReadyRead();
    }


    if(bool_cancel_extraction)
    {
        if(process_carve_data->state() == QProcess::Running)
            process_carve_data->terminate();

        emit signal_PBAR_thread_carved_data_finished(MACRO_JobType_Carved_Data);
        return;
    }



    ///parse data
    QStringList text_files_list;
    QString tmp_path_str;
    QDirIterator itr(output_str, QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    while (itr.hasNext())
    {
        if(bool_cancel_extraction)
            break;

        tmp_path_str = itr.next();
        if(QFileInfo(tmp_path_str).fileName().contains("_histogram"))
            text_files_list << tmp_path_str;
    }
    parse_bulk_extractor_txt_files(text_files_list);


    ///remove tmp dir
    recon_static_functions::do_directory_blank_completely(output_str, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(output_str, Q_FUNC_INFO);

    emit signal_PBAR_thread_carved_data_finished(MACRO_JobType_Carved_Data);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

}

void thread_carve_data::parse_bulk_extractor_txt_files(QStringList txt_files_list)
{

    if(txt_files_list.isEmpty() || bool_cancel_extraction)
        return;

    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    dest_db.setDatabaseName(dest_db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + dest_db.lastError().text() , Q_FUNC_INFO);
        return;
    }


    dest_db.transaction();

    int textfile_processed_count = 0;
    qint64 total_record_count = 0;
    for(int count = 0; count < txt_files_list.size(); count++)
    {

        if(bool_cancel_extraction)
            break;

        QString txt_file_path = txt_files_list.at(count);

        QFile file_txt(txt_file_path);
        if(!file_txt.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("----FAILED---- file open:" + file_txt.fileName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + file_txt.errorString() ,Q_FUNC_INFO);
            continue;
        }
        textfile_processed_count++;

        QString table_name = QFileInfo(txt_file_path).fileName();
        QStringList tmp_split_list = table_name.split("_histogram");
        table_name = tmp_split_list.at(0);


        QString command_create = QString("create table IF NOT EXISTS " + table_name +
                                         " (bookmark INTEGER,INT INTEGER PRIMARY KEY,plugin_name varchar(100), tab_name varchar(100) , category_name varchar(100),number_of_hits varchar(100),carved_keyword varchar(500),"
                                         "source_plugin_name varchar(100),source_tab_name varchar(100),source_record_no varchar(100),source_file_name varchar(100),source_file_path varchar(500),"
                                         "root_count_name varchar(100), source_count_name varchar(100),"
                                         "notes_icon varchar(1000),recon_tag_value varchar(1000),notes varchar(1000),recon_filefrom varchar(10),recon_file_infopath varchar(500),blank_column varchar(10))");

        recon_helper_standard_obj->execute_db_command_by_dbreference(command_create, dest_db, Q_FUNC_INFO);


        QString line_str;
        QString item1_str, item2_str;
        tmp_split_list.clear();

        bool bool_records_found = false;
        while(!file_txt.atEnd())
        {
            if(bool_cancel_extraction)
                break;

            line_str = QString::fromLocal8Bit(file_txt.readLine()).trimmed();

            if(line_str.startsWith("#") || line_str.trimmed().isEmpty())
                continue;

            if(line_str.contains("\t"))
            {
                item1_str.clear();
                item2_str.clear();

                tmp_split_list = line_str.split("\t");

                item1_str = tmp_split_list.at(0);
                if(item1_str.startsWith("n="))
                    item1_str.remove(0,2);

                item1_str = item1_str.trimmed();

                if(tmp_split_list.size() > 1)
                    item2_str = tmp_split_list.at(1);

                item2_str = item2_str.trimmed();

                if(item2_str.isEmpty())
                    continue;

                bool_records_found = true;
                total_record_count++;

                QString command_insert = QString("INSERT INTO " + table_name + "(bookmark,plugin_name , tab_name , category_name, number_of_hits,carved_keyword,"
                                                                               "source_plugin_name,source_tab_name,source_record_no,source_file_name,source_file_path,"
                                                                               "root_count_name,source_count_name , recon_filefrom , recon_file_infopath)"
                                                                               " VALUES(?,?,?,?,?,?,?, ?,?,?,?,?, ?,?,?)");




                QStringList value_list;
                value_list << QString::number(0)
                           << QString(MACRO_Plugin_Name_Carved_Data) <<  QString(MACRO_Plugin_Carved_Data_TAB_Name_Carved_Data)
                           << table_name << item1_str << item2_str
                           << source_plugin_name << source_tab_name << source_record_no << source_filename << source_filepath_for_display
                           << st_source_info_obj.root_count_name << st_source_info_obj.source_count_name << recon_file_from << recon_file_infopath;

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert, value_list, dest_db, Q_FUNC_INFO);

                if(total_record_count % 500 == 0)
                    emit signal_PBAR_thread_carve_data_processing_value(MACRO_JobType_Carved_Data, source_filename, true, total_record_count, 0,true);

            }
        }

        file_txt.close();

        ///remove blank table
        if(!bool_records_found)
        {
            ///check previous records
            QString cmd = QString("Select count(*) from " + table_name);
            QString records = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, dest_db, Q_FUNC_INFO);
            int records_num = records.toInt();
            if(records_num == 0)
            {
                //drop empty table only
                QString cmd = QString("DROP table " + table_name);
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
            }
        }


    }

    dest_db.commit();
    dest_db.close();
}



