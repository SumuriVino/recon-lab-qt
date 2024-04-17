#include "z_mainwindow/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::insert_ext_attributes_in_recon_library(QString csv_filepath, QSqlDatabase configuration_db, bool bool_insert_apple_attributes)
{
    recon_static_functions::app_debug(" : insert_ext_attributes_in_recon_library -Starts " , Q_FUNC_INFO);

    QFile file(csv_filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(" : insert_ext_attributes_in_recon_library -File Open-------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(file.errorString() , Q_FUNC_INFO);
        return;
    }

    file.readLine(); // Column Title Line
    int count = 0;
    while(!file.atEnd())
    {
        count++;

        if(count%10 == 0)
            QCoreApplication::processEvents();

        QString line = file.readLine();

        if(line.endsWith("\n"))
            line.chop(1);
        if(line.endsWith(",,,"))
            line.chop(3);

        QStringList list = line.split(",,,", Qt::SkipEmptyParts);

        QString attribute_name, description, widget_type, kmditem_command,command_type;

        if(list.size() > 0)
        {
            attribute_name = list.at(0);
            attribute_name.remove("\"");
        }
        if(list.size() > 1)
        {
            description = list.at(1);
            description.remove("\"");
        }
        if(list.size() > 2)
        {
            widget_type = list.at(2);
            widget_type.remove("\"");
        }
        if(list.size() > 3)
        {
            kmditem_command = list.at(3);
            kmditem_command.remove("\"");
        }

        if(list.size() > 4)
        {
            command_type = list.at(4);
            command_type.remove("\"");
        }

        QString bookmark_display_filter = QString("1") ,bookmark_report, default_str = QString("1");

        QString bookmark_metadata = "0";
        if(count < 7)
        {
            bookmark_report = "1";
        }
        else
        {
            bookmark_report = "0";
        }

        kmditem_command = kmditem_command.trimmed();
        if(kmditem_command == "TYPE YOUR OWN QUERY")
            continue;


        QString attribute_str = attribute_name;
        QString column_name = kmditem_command + "_" + attribute_str.replace(" ", "_");

        if(bool_insert_apple_attributes)
        {
            QString command_insert = "insert into tbl_apple_attributes(bookmark_filter,bookmark_metadata,bookmark_report,is_default, attribute_name, description, widget_type,data_type, metadata_attribute_name,custom_column_name) values(?,?,?,?,?,?,?,?,?,?)";
            QStringList listofvalues;
            listofvalues << bookmark_display_filter << bookmark_metadata << bookmark_report << default_str  << attribute_name << description << widget_type << command_type << kmditem_command << column_name;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,listofvalues,configuration_db,Q_FUNC_INFO);

        }
        else
        {


        }

    }

    file.close();
    recon_static_functions::app_debug(" : insert_ext_attributes_in_recon_library -Ends " , Q_FUNC_INFO);

}

void MainWindow::create_recon_library_structure()
{
    recon_static_functions::app_debug(" : [create_recon_library_structure] - Starts",Q_FUNC_INFO);

    QString result_info_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "loaded_results_info.sqlite";
    if(!QFile::exists(result_info_sqlite_path))
    {
        QString command = "CREATE TABLE 'tbl_loaded_result_info' (INT INTEGER PRIMARY KEY, case_name varchar(1024), result_dir_path varchar(10),"
                          "item1 varchar(1024) ,item2 varchar(1024) ,item3 varchar(1024) ,item4 varchar(1024) ,item5 varchar(1024))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,result_info_sqlite_path,Q_FUNC_INFO);
    }


    QString RECON_Library_vrsn_spcfpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString).toString();

    create_recon_configuration_structure(RECON_Library_vrsn_spcfpath);
    create_app_configuration_structure(RECON_Library_vrsn_spcfpath);

    QDir dir;
    QString result_config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString();
    QFile file(result_config_dir_path);
    if(!file.exists())
    {
        dir.mkpath(result_config_dir_path);
    }

    QCoreApplication::processEvents();


    /// =========== Save Current Datetime =========///
    save_creation_datetime_for_recon_library(RECON_Library_vrsn_spcfpath);


    QString RECON_Library_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString();




    //---------------------------Mounted Image Info start -------------------------//

    QString mounted_images_db_file = RECON_Library_path + QString("mounted_images/");
    dir.mkdir(mounted_images_db_file);
    mounted_images_db_file.append("mounted_images_info.sqlite");

    QFile file_image(mounted_images_db_file);
    if(!file_image.exists())
    {

        QString command_file ="CREATE TABLE images_info (INT INTEGER PRIMARY KEY,image_path_1 VARCHAR(1024),"
                              "image_name_1 VARCHAR(300),shadow_file_path_1 VARCHAR(1024), disk_node_1 VARCHAR(500),"
                              "disk_identifier_1 VARCHAR(500), osx_fuse_mount_point_1 VARCHAR(1024),"
                              "image_path_2 VARCHAR(1024), image_name_2 VARCHAR(300),shadow_file_path_2 VARCHAR(1024),"
                              "disk_node_2 VARCHAR(500), disk_identifier_2 VARCHAR(500), osx_fuse_mount_point_2 VARCHAR(1024),"
                              "image_format_type VARCHAR(500), image_password VARCHAR(500),image_source_type_display VARCHAR(500),image_source_type_internal VARCHAR(500))";

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_file,mounted_images_db_file,Q_FUNC_INFO);

        command_file ="CREATE TABLE partition_info (INT INTEGER PRIMARY KEY,image_id INTEGER, volume_type VARCHAR(300),"
                      "volume_name VARCHAR(300), volume_total_size_byte_numeric VARCHAR(1024), volume_free_size_byte_numeric VARCHAR(1024),"
                      "volume_fs_type VARCHAR(1024), volume_identifier VARCHAR(500), volume_node VARCHAR(500),"
                      "derived_into_disk_node VARCHAR(1024), derived_into_disk_identifier VARCHAR(1024),"
                      "derived_into_volume_name VARCHAR(300), volume_uuid VARCHAR(1024), mount_path VARCHAR(500))";



        recon_helper_standard_obj->execute_db_command_by_dbpath(command_file,mounted_images_db_file,Q_FUNC_INFO);
    }

    //---------------------------Mounted Image Info end -------------------------//


    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}


void MainWindow::update_current_config_from_previous_in_db(QSqlDatabase *current_config_db , QSqlDatabase *previous_config_db , QString select_command_str , QString insert_or_update_command_str , int status_update_for , QStringList already_existing_values_list)
{
    recon_static_functions::app_debug("-Start -:" + QString::number(status_update_for),Q_FUNC_INFO);

    if(select_command_str.isEmpty())
        return;

    if(insert_or_update_command_str.isEmpty())
        return;

    /// select previous values
    QSqlQuery query_previous(*previous_config_db);
    query_previous.prepare(select_command_str);
    if(!query_previous.exec())
    {
        recon_static_functions::app_debug(" previous query  --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_command_str, Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_previous.lastError().text(),Q_FUNC_INFO);
        return;
    }


    /// insert previous values to current db

    QSqlQuery query_current(*current_config_db);

    while(query_previous.next())
    {

        if(status_update_for == enum_mainwindow_recon_library_update_agency_details)
        {

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///agency_name
            query_current.addBindValue(query_previous.value(1).toString()); ///agency_address
            query_current.addBindValue(query_previous.value(2).toString()); ///examiner
            query_current.addBindValue(query_previous.value(3).toString()); ///examiner_phone
            query_current.addBindValue(query_previous.value(4).toString()); ///examiner_email
            query_current.addBindValue(query_previous.value(5).toString()); ///date_format

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (agency details) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_case_result_info)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///INT
            query_current.addBindValue(query_previous.value(1).toString()); ///name
            query_current.addBindValue(query_previous.value(2).toString()); ///result_path

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (case result info) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + select_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_exif_attributes)
            //            else if(status_update_for == enum_mainwindow_recon_library_update_apple_attributes)
        {
            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark_filter
            query_current.addBindValue(query_previous.value(1).toString()); ///bookmark_metadata
            query_current.addBindValue(query_previous.value(2).toString()); ///bookmark_report

            //====Condition value====//
            query_current.addBindValue(query_previous.value(3).toString()); ///attribute_key

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (exif attributes) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_update_apple_attributes)
        {

            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark_filter
            query_current.addBindValue(query_previous.value(1).toString()); ///attribute_name
            query_current.addBindValue(query_previous.value(2).toString()); ///description
            query_current.addBindValue(query_previous.value(3).toString()); ///is_default
            query_current.addBindValue(query_previous.value(4).toString()); ///widget_type
            query_current.addBindValue(query_previous.value(6).toString()); ///data_type
            query_current.addBindValue(query_previous.value(7).toString()); ///custom_column_name
            query_current.addBindValue(query_previous.value(8).toString()); ///bookmark_metadata
            query_current.addBindValue(query_previous.value(9).toString()); ///bookmark_report

            //====Condition value====//
            query_current.addBindValue(query_previous.value(5).toString()); ///metadata_attribute_name

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (extended attributes) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_update_extension_category)
        {

            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(2).toString()); ///total_records

            //====Condition value====//
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory_name

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update extension category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_extension_category)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(1).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory_name
            query_current.addBindValue(query_previous.value(2).toString()); ///total_records


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Insert extension category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_extension_list)
        {
            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(2).toString()); ///extension_name

            //====Condition value====//
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory_name

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update extension List) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_extension_list)
        {

            /// skip values which are already updated
            //            if(already_existing_values_list.contains(query_previous.value(1).toString()))
            //                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory_name
            query_current.addBindValue(query_previous.value(2).toString()); ///extension_name


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Insert extension list) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_signature_category)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(0).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(1).toString()); ///category
            query_current.addBindValue(query_previous.value(2).toString()); ///category_status

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update signature category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_update_signature_list)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(2).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///category
            query_current.addBindValue(query_previous.value(1).toString()); ///label
            query_current.addBindValue(query_previous.value(2).toString()); ///header
            query_current.addBindValue(query_previous.value(3).toString()); ///signature_status
            query_current.addBindValue(query_previous.value(4).toString()); ///offset


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Insert signature list) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_hashset)
        {

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///table_name
            query_current.addBindValue(query_previous.value(2).toString()); ///column_name
            query_current.addBindValue(query_previous.value(3).toString()); ///database_path
            query_current.addBindValue(query_previous.value(4).toString()); ///is_created_by_user

            if(select_command_str.contains("recon_filefrom") && select_command_str.contains("recon_file_infopath"))
            {
                query_current.addBindValue(query_previous.value(5).toString()); ///recon_filefrom
                query_current.addBindValue(query_previous.value(6).toString()); ///recon_file_infopath
            }

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Insert hashet) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_index_files_whitelist_category)
        {
            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark

            //====Condition value====//
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update index files whitelist category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_index_files_whitelist_category)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(1).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update index files whitelist extensions) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_index_files_blacklist_category)
        {
            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark

            //====Condition value====//
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update index files blacklist category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_index_files_blacklist_category)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(1).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update index files blacklist extensions) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_index_files_blacklist_extensions)
        {

            //            /// skip values which are already updated
            //            if(already_existing_values_list.contains(query_previous.value(1).toString()))
            //                continue;


            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///category
            query_current.addBindValue(query_previous.value(2).toString()); ///extension


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert index files blacklist extensions) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_index_files_whitelist_extensions)
        {

            //            /// skip values which are already updated
            //            if(already_existing_values_list.contains(query_previous.value(1).toString()))
            //                continue;


            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///category
            query_current.addBindValue(query_previous.value(2).toString()); ///extension


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert index files whitelist extensions) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_volatility_path)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///dir path

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert volatility dir path) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_selected_text_index_filter)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///select_status
            query_current.addBindValue(query_previous.value(1).toString()); ///selected_filter

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert selected text indexing filter) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_external_applications)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///application_name
            query_current.addBindValue(query_previous.value(1).toString()); ///application_path

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert external applications) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_timezone_details)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString());
            query_current.addBindValue(query_previous.value(1).toString());
            query_current.addBindValue(query_previous.value(2).toString());
            query_current.addBindValue(query_previous.value(3).toString());
            query_current.addBindValue(query_previous.value(4).toString());
            query_current.addBindValue(query_previous.value(5).toString());

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert timezone details) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_keyword_search_category)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///category_status
            query_current.addBindValue(query_previous.value(1).toString()); ///category


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert keyword_list category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_filetypes)
        {

            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(2).toString()); ///extension_name
            query_current.addBindValue(query_previous.value(4).toString()); ///signature_header
            query_current.addBindValue(query_previous.value(5).toString()); ///mime_type
            query_current.addBindValue(query_previous.value(6).toString()); ///

            //====Condition value====//
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory_name
            query_current.addBindValue(query_previous.value(3).toString()); ///extension_type

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update Filetypes) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_filetypes)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(1).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(1).toString()); ///catagory_name
            query_current.addBindValue(query_previous.value(2).toString()); ///extension_name

            query_current.addBindValue(query_previous.value(3).toString()); ///extension_type
            query_current.addBindValue(query_previous.value(4).toString()); ///signature_header
            query_current.addBindValue(query_previous.value(5).toString()); ///mime_type
            query_current.addBindValue(query_previous.value(6).toString()); ///

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Insert Filetypes) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_update_Tags)
        {

            query_current.prepare(insert_or_update_command_str);

            //====Update values====//
            query_current.addBindValue(query_previous.value(1).toString()); ///selected_colour
            query_current.addBindValue(query_previous.value(2).toString()); ///is_default_tag
            query_current.addBindValue(query_previous.value(3).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(4).toString()); ///color_hex

            //====Condition value====//
            query_current.addBindValue(query_previous.value(0).toString()); ///tag_name

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Update Tags) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_Tags)
        {
            /// skip values which are already updated
            if(already_existing_values_list.contains(query_previous.value(0).toString()))
                continue;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///tag_name
            query_current.addBindValue(query_previous.value(1).toString()); ///selected_colour
            query_current.addBindValue(query_previous.value(2).toString()); ///is_default_tag

            query_current.addBindValue(query_previous.value(3).toString()); ///bookmark
            query_current.addBindValue(query_previous.value(4).toString()); ///color_hex

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (Insert Tags) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_update_plugin_template)
        {
            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(query_previous.value(0).toString()); ///template_name
            query_current.addBindValue(query_previous.value(1).toString()); ///plugin_name
            query_current.addBindValue(query_previous.value(2).toString()); ///selected_tabs_list
            query_current.addBindValue(query_previous.value(3).toString()); ///os_scheme_display

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (plugin_selection_template) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_keyword_search_category)
        {
            query_current.prepare(insert_or_update_command_str);

            //            query_current.addBindValue(query_previous.value(0).toString()); ///INT
            //            query_current.addBindValue(query_previous.value(1).toString()); ///category
            //            query_current.addBindValue(query_previous.value(2).toString()); ///category_status

            query_current.addBindValue(query_previous.value(0).toString()); ///category
            query_current.addBindValue(query_previous.value(1).toString()); ///category_status

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (keyword_category) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }

        }
        else if(status_update_for == enum_mainwindow_recon_library_keyword_search_keywords_list)
        {
            query_current.prepare(insert_or_update_command_str);

            //            query_current.addBindValue(query_previous.value(0).toString()); ///INT
            //            query_current.addBindValue(query_previous.value(1).toString()); ///category
            //            query_current.addBindValue(query_previous.value(2).toString()); ///keyword
            //            query_current.addBindValue(query_previous.value(3).toString()); ///keyword_status

            query_current.addBindValue(query_previous.value(0).toString()); ///category
            query_current.addBindValue(query_previous.value(1).toString()); ///keyword
            query_current.addBindValue(query_previous.value(2).toString()); ///keyword_status
            query_current.addBindValue(query_previous.value(3).toString()); ///is_regex


            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (keyword_list) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else if(status_update_for == enum_mainwindow_recon_library_insert_into_preferences)
        {
            query_current.prepare(insert_or_update_command_str);

            QString key    = query_previous.value(0).toString();
            QString value  = query_previous.value(1).toString();

            query_current.addBindValue(value);
            query_current.addBindValue(key);

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (preferences) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else
        {

            recon_static_functions::app_debug(" status update for not found  --------FAILED------ " ,Q_FUNC_INFO);
            break;
        }
    }

    //=============================when while condition not extract/fill Start===============================================//

    //insert current machine timezone details if older library had empty details

    if(status_update_for == enum_mainwindow_recon_library_insert_into_timezone_details)
    {
        QString command = "SELECT Count(*)  from timezone_details";

        QString rec_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(command, 0, *current_config_db, Q_FUNC_INFO);
        rec_str = rec_str.trimmed();

        if(rec_str.isEmpty() || rec_str == QString::number(0))
        {
            struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

            QString current_machine_timezone = strct_exm_obj.timezone_name_complete_QString;
            qint64 current_machine_timezone_offset = strct_exm_obj.timezone_offset_numeric_qint64;

            query_current.prepare(insert_or_update_command_str);

            query_current.addBindValue(current_machine_timezone);
            query_current.addBindValue(current_machine_timezone_offset);
            query_current.addBindValue("UTC +0000");
            query_current.addBindValue("0");
            query_current.addBindValue("yyyy-MMM-dd");
            query_current.addBindValue("0");

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (insert timezone details) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
    }

    //==============================when while condition not extract/fill End==============================================//


    recon_static_functions::app_debug("-End",Q_FUNC_INFO);

}

QStringList MainWindow::get_all_previous_recon_configuration_dirs_list_with_timestamp()
{

    // output:   ("1511350379;++::++;/Users/nokia/RECON_Library/1_0_5", "1511415146;++::++;/Users/nokia/RECON_Library/2_0_1")


    recon_static_functions::app_debug("-Start",Q_FUNC_INFO);

    QStringList config_timestamp_db_path_with_value;

    QString recon_library_home_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString();

    QString cmd_config_datetime = QString("Select creation_timestamp From creation_datetime");

    QDirIterator library_itr(recon_library_home_path, QDirIterator::Subdirectories);
    while (library_itr.hasNext())
    {
        QFileInfo info_itr(library_itr.next());

        if(info_itr.fileName() == QString("configuration_datetime.sqlite"))
        {
            QString timestamp_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_config_datetime , 0 , info_itr.filePath() , Q_FUNC_INFO);

            QStringList path_split_list = info_itr.filePath().split("/", Qt::SkipEmptyParts);
            path_split_list.removeLast();

            QString tmp_path = path_split_list.join("/");
            if(tmp_path.endsWith("/tmp/" + recon_static_functions::get_recon_library_version()))
                continue;

            config_timestamp_db_path_with_value << timestamp_str + QString(MACRO_RECON_Splitter_1_multiple) + tmp_path;

        }
        else
            continue;
    }

    recon_static_functions::app_debug("-End",Q_FUNC_INFO);

    return config_timestamp_db_path_with_value;
}

void MainWindow::save_creation_datetime_for_recon_library(QString recon_library_path)
{
    recon_static_functions::app_debug("-Start",Q_FUNC_INFO);

    QString recon_datetime_db_path = recon_library_path + QString("configuration_datetime.sqlite");

    if(!QFileInfo(recon_datetime_db_path).exists())
    {
        QString cmd_current_datetime = QString("CREATE TABLE creation_datetime (INT INTEGER PRIMARY KEY,creation_timestamp INT)");

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase recon_datetime_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

        recon_datetime_db.setDatabaseName(recon_datetime_db_path);
        if(recon_datetime_db.open())
        {
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_current_datetime,recon_datetime_db,Q_FUNC_INFO);

            cmd_current_datetime = QString("INSERT INTO creation_datetime(creation_timestamp) values(?)");
            QStringList value_list;
            value_list << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch());


            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_current_datetime , value_list , recon_datetime_db , Q_FUNC_INFO);
            recon_datetime_db.close();

        }
        else
        {
            recon_static_functions::app_debug("Destination Db opening creation_datetime --------FAILED------ " + recon_datetime_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + recon_datetime_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug("-End",Q_FUNC_INFO);

}

QString MainWindow::get_last_run_recon_config_db_dir_path_from_paths_list(QStringList paths_list)
{

    /* This function will return back single path with largest timestamp of all the values in list.
     *
     * Input:   ("1511350379;++::++;/Users/nokia/RECON_Library/1_0_5", "1511415146;++::++;/Users/nokia/RECON_Library/2_0_9")
     * Output:  ("1511415146;++::++;/Users/nokia/RECON_Library/2_0_9")
     */

    recon_static_functions::app_debug("-Start",Q_FUNC_INFO);

    QString latest_config_path_str;

    if(paths_list.size() == 1)
    {
        QString tmp_config_str = paths_list.at(0);

        QStringList tmp_split_list = tmp_config_str.split(QString(MACRO_RECON_Splitter_1_multiple), Qt::SkipEmptyParts);
        latest_config_path_str = tmp_split_list.at(1);

        return latest_config_path_str;
    }

    if(paths_list.size() > 1)
    {

        ///prepairing timestamps list
        QStringList timestamp_list;
        for(int count = 0; count < paths_list.size(); count++)
        {
            QString tmp_config_str     = paths_list.at(count);
            QStringList tmp_split_list = tmp_config_str.split(QString(MACRO_RECON_Splitter_1_multiple), Qt::SkipEmptyParts);

            QString timestamp_str = tmp_split_list.at(0);

            timestamp_list << timestamp_str;
        }

        /// get largest of all timestamps from list
        QString max_str = *std::max_element(timestamp_list.begin(), timestamp_list.end());


        /// getting the latest configuration path
        for(int count = 0; count < paths_list.size(); count++)
        {
            QString tmp_config_str     = paths_list.at(count);
            QStringList tmp_split_list = tmp_config_str.split(QString(MACRO_RECON_Splitter_1_multiple), Qt::SkipEmptyParts);

            QString timestamp_str = tmp_split_list.at(0);

            if(timestamp_str == max_str)
            {
                latest_config_path_str = tmp_split_list.at(1); /// latest
                break;
            }
        }
    }

    recon_static_functions::app_debug("-End",Q_FUNC_INFO);

    return latest_config_path_str;
}

bool MainWindow::is_newer_lib_version_available(QString current_version , QString last_version)
{
    bool is_latest_version = false;

    if(current_version.contains("."))
        current_version = current_version.remove(QString("."));

    if(last_version.contains("."))
        last_version = last_version.remove(QString("."));


    if(current_version.toUInt() > last_version.toUInt())
        is_latest_version = true;

    return is_latest_version;
}


void MainWindow::create_recon_configuration_structure(QString RECON_Library_vrsn_spcfpath)
{

    QString apple_attributes_csv_path = QString("../Resources/apple_attributes/apple_attributes_list.csv");

    //--------------------------- RECON configuration start -------------------------//

    QString recon_configuration_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString();


    // ==================== get RECON_Library updated status - Start ==============///
    QString configuration_db_path = QString("../Resources/configuration.sqlite");
    QString command_str = QString("Select is_updated From configuration_status Where feature_name = ?");
    QStringList value_list;
    value_list << QString(MACRO_Configuration_Feature_Name_RECON_Library);

    QString recon_Library_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str , value_list, 0, configuration_db_path , Q_FUNC_INFO);

    QDir dir;

    if(recon_Library_status == QString::number(0))
    {
        ///Recon library needs to be updated
        if(QFileInfo(RECON_Library_vrsn_spcfpath).exists())
        {
            /// copy temporarily before removal
            QString tmp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString()
                    + QString("tmp/") + recon_static_functions::get_recon_library_version() + QString("/");

            bool status = dir.mkpath(tmp_dir_path);
            if(!status)
            {
                recon_static_functions::app_debug("tmp copy directory creation --------FAILED------ " + tmp_dir_path,Q_FUNC_INFO);
                // m_file.remove();
                return;
            }
            else
            {
                QString tmp_library_path = RECON_Library_vrsn_spcfpath;
                if(tmp_library_path.endsWith("/"))
                    tmp_library_path.chop(1);

                recon_static_functions::copy_directory(tmp_library_path , tmp_dir_path , Q_FUNC_INFO);
            }

            recon_static_functions::do_directory_blank_partially(RECON_Library_vrsn_spcfpath , QStringList(QString("configuration_datetime.sqlite")) , Q_FUNC_INFO);
        }
    }
    else
    {
        ///Recon library already updated
        if(QFileInfo(RECON_Library_vrsn_spcfpath).exists())
        {
            //  m_file.remove();
            recon_static_functions::app_debug(" end ",Q_FUNC_INFO);
            return;
        }
    }


    QCoreApplication::processEvents();

    bool status = dir.mkpath(recon_configuration_path);

    if(!status)
    {
        recon_static_functions::app_debug("recon configuration directory creation --------FAILED------ " + recon_configuration_path,Q_FUNC_INFO);
        // m_file.remove();
        return;
    }


    QString recon_configuration_db_file = recon_configuration_path + QString("recon_configuration.sqlite");
    QFile file_recon_config(recon_configuration_db_file);
    if(!file_recon_config.exists())
    {

        QString  cmd_agency_details = QString("create table agency_details (agency_name varchar(100), agency_address varchar(100), examiner varchar(100), examiner_phone varchar(100),"
                                              "examiner_email varchar(100),date_format VARCHAR(500))");

        QString  cmd_extension_catagory = QString("CREATE TABLE extension_category(bookmark INTEGER,INT INTEGER PRIMARY KEY, category_name VARCHAR(500), total_records VARCHAR(500))");

        QString  cmd_extension_list = QString("CREATE TABLE extension_list(bookmark INTEGER,INT INTEGER PRIMARY KEY, category_name VARCHAR(500), extension_name varchar(500))");


        QString  cmd_apl_attributes = "CREATE TABLE tbl_apple_attributes(bookmark_filter INTEGER,bookmark_metadata INTEGER,bookmark_report INTEGER,INT INTEGER PRIMARY KEY,attribute_name VARCHAR(100),description VARCHAR(100),"
                                      "is_default VARCHAR(100), widget_type VARCHAR(100),metadata_attribute_name VARCHAR(100),data_type varchar(1000),custom_column_name varchar(1000))";

        QString  cmd_exf_attributes = "CREATE TABLE tbl_exif_attributes(bookmark_filter INTEGER,bookmark_metadata INTEGER,bookmark_report INTEGER,INT INTEGER PRIMARY KEY,is_default VARCHAR(100),parent_name VARCHAR(100),attribute_title varchar(100), attribute_key varchar(100), custom_column_name varchar(1000), description VARCHAR(100))";

        QString cmd_timezone_selection = "CREATE TABLE timezone_details (INT INTEGER PRIMARY KEY,case_created_machine_timezone_qstring varchar(100),case_created_machine_timezone_qint64 INTEGER ,examiner_selected_timezone_qstring varchar(100),examiner_selected_timezone_qint64 INTEGER, examiner_selected_date_format_qstring varchar(100),examiner_selected_date_format_int INTEGER)";

        //---HAsh Set


        QString cmd_tags_tags = "CREATE TABLE tags (INT INTEGER PRIMARY KEY,tag_name varchar(100),selected_colour varchar(100),is_default_tag varchar(100),bookmark varchar(100),color_hex varchar(100))";

        QString cmd_volatility_path = "CREATE TABLE tbl_volatility (volatility_dir varchar(500))";

        QString cmd_external_applications = "CREATE TABLE tbl_external_applications (INT INTEGER PRIMARY KEY, application_name varchar(500), application_path varchar(500))";

        QString cmd_plugins_template = "CREATE TABLE plugin_selection_template (INT INTEGER PRIMARY KEY,template_name varchar(100),plugin_name varchar(100) , selected_tabs_list varchar(100) , os_scheme_display varchar(100))";
        QString cmd_plugin_tabs = "CREATE TABLE plugin_tabs (INT INTEGER PRIMARY KEY,plugin_no varchar(100),name VARCHAR(100), export_available varchar(100),default_selected varchar(100),supported_os_scheme_display_list varchar(100))";
        QString cmd_plugins = "CREATE TABLE plugins (INT INTEGER PRIMARY KEY,category varchar(100),name VARCHAR(100),export_available varchar(100),is_plugin_run varchar(100),is_supported varchar(100),default_selected varchar(100), supported_os_scheme_display_list varchar(100) )";
        QString cmd_result_info = "CREATE TABLE case_result_info (INT INTEGER PRIMARY KEY,name VARCHAR(100),result_path varchar(100) )";
        QString cmd_keyword_category = "CREATE TABLE keyword_category (INT INTEGER PRIMARY KEY,category VARCHAR(100),category_status varchar(100))";
        QString cmd_keyword_list     = "CREATE TABLE keyword_list (INT INTEGER PRIMARY KEY,category VARCHAR(100),keyword varchar(100),keyword_status varchar(100), is_regex INTEGER)";
        QString cmd_mime_catagory = QString("CREATE TABLE mime_type_category(bookmark INTEGER,INT INTEGER PRIMARY KEY, category_name VARCHAR(500), total_records INT)");
        QString cmd_mime_type     = QString("CREATE TABLE tbl_mime_type (bookmark INTEGER , INT INTEGER PRIMARY KEY,category_name VARCHAR(100),mime_type varchar(100))");
        QString cmd_file_size_filters  = QString("CREATE TABLE file_size_filters (bookmark INTEGER , INT INTEGER PRIMARY KEY, filter_name VARCHAR(100), greater_than_size varchar(100), greater_than_size_unit varchar(10), less_than_size varchar(100), less_than_size_unit varchar(10), start_size varchar(100), start_size_unit varchar(10), end_size varchar(100), end_size_unit varchar(100))");
        QString cmd_file_signature_category = QString("CREATE TABLE tbl_signature_category (INT INTEGER PRIMARY KEY,category varchar(100),category_status VARCHAR(100))");
        QString cmd_file_signature_list     = QString("CREATE TABLE tbl_signature_list (INT INTEGER PRIMARY KEY,category varchar(100),label varchar(100),header varchar(100),signature_status VARCHAR(100),offset VARCHAR(100))");
        QString cmd_index_files_whitelist_category   = QString("CREATE TABLE tbl_index_files_whitelist_category  (bookmark INTEGER ,INT INTEGER PRIMARY KEY,category varchar(100))");
        QString cmd_index_files_blacklist_category   = QString("CREATE TABLE tbl_index_files_blacklist_category  (bookmark INTEGER ,INT INTEGER PRIMARY KEY,category varchar(100))");
        QString cmd_index_files_whitelist_extension  = QString("CREATE TABLE tbl_index_files_whitelist_extension (bookmark INTEGER ,INT INTEGER PRIMARY KEY,category varchar(100),extension varchar(100))");
        QString cmd_index_files_blacklist_extension  = QString("CREATE TABLE tbl_index_files_blacklist_extension (bookmark INTEGER ,INT INTEGER PRIMARY KEY,category varchar(100),extension varchar(100))");
        QString cmd_tags_colours           = QString("CREATE TABLE colors (INT INTEGER PRIMARY KEY,color_name varchar(100),color_hex varchar(100))");
        QString cmd_hashset  = QString("CREATE TABLE tbl_hashset (bookmark INTEGER ,INT INTEGER PRIMARY KEY,table_name varchar(100),column_name varchar(100),database_path varchar(500), is_created_by_user INTEGER, recon_filefrom varchar(10), recon_file_infopath varchar(100))");
        QString cmd_saved_tm_zone = "CREATE TABLE tbl_saved_timezones (INT INTEGER PRIMARY KEY,timezone varchar(100),timezone_offset INTEGER , start_date_epoch INTEGER,end_date_epoch INTEGER ,start_date_str varchar(100),end_date_str varchar(100),display_time varchar(100))";
        QString cmd_preferences = "CREATE TABLE tbl_preferences (key varchar(500), value varchar(500))";
        QString cmd_filename_category = "CREATE TABLE filename_category (INT INTEGER PRIMARY KEY,category VARCHAR(100),category_status varchar(100))";
        QString cmd_filename_list     = "CREATE TABLE filename_list (INT INTEGER PRIMARY KEY,category VARCHAR(100),filename varchar(100),filename_status varchar(100))";

        QCoreApplication::processEvents();

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase recon_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        recon_config_db.setDatabaseName(recon_configuration_db_file);
        if(recon_config_db.open())
        {
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_agency_details,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_extension_catagory,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_extension_list,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_exf_attributes,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_apl_attributes,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_timezone_selection,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_tags_tags,recon_config_db,Q_FUNC_INFO);

            QCoreApplication::processEvents();

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_plugins_template,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_plugin_tabs,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_plugins,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_result_info,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_keyword_category,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_keyword_list,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_mime_catagory,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_mime_type,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_file_size_filters,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_file_signature_category,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_file_signature_list,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_index_files_whitelist_category,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_index_files_blacklist_category,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_index_files_whitelist_extension,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_index_files_blacklist_extension,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_tags_colours,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_volatility_path,recon_config_db,Q_FUNC_INFO);
            QCoreApplication::processEvents();

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_external_applications,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_hashset,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_saved_tm_zone,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_preferences,recon_config_db,Q_FUNC_INFO);

            QCoreApplication::processEvents();
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_filename_category,recon_config_db,Q_FUNC_INFO);
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_filename_list,recon_config_db,Q_FUNC_INFO);

            insert_ext_attributes_in_recon_library(apple_attributes_csv_path,recon_config_db, true);


            insert_index_files_filesize_default_details_in_db(recon_config_db);
            insert_case_info_in_db(recon_config_db);
            insert_exif_attr_in_db(recon_config_db);
            insert_index_files_filter_in_db(recon_config_db);
            insert_extensions_in_db(recon_config_db);
            insert_preferences_detail_in_db(recon_config_db);

            QCoreApplication::processEvents();

            insert_timezone_details(recon_config_db);
            insert_tags_details_in_db(recon_config_db);
            QCoreApplication::processEvents();

            insert_keyword_category_and_list_in_db(recon_config_db);
            QCoreApplication::processEvents();

            insert_mime_type_details_in_db(recon_config_db);
            insert_file_signature_details_in_db(recon_config_db);
            insert_color_info_in_recon_configuration_db(recon_config_db);

            QCoreApplication::processEvents();

            insert_plugins_info_in_config_db(recon_config_db,QString("../Resources/CSVS/Plugins_CSV/"));
            recon_config_db.close();


            // =========== Update Recon Library updated status - Start =========///
            command_str = QString("Update configuration_status set is_updated = ? Where feature_name = ?");
            value_list.clear();
            value_list << QString::number(1) << QString(MACRO_Configuration_Feature_Name_RECON_Library);

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, configuration_db_path , Q_FUNC_INFO);
            // =========== Update Recon Library updated status - End =========///


            ///===========Get previous recon configuration =========///

            QStringList config_timestamp_db_path_with_value = get_all_previous_recon_configuration_dirs_list_with_timestamp();


            QString latest_config_path_str = get_last_run_recon_config_db_dir_path_from_paths_list(config_timestamp_db_path_with_value);


            ///======Library Updation Check=====///
            //Shouldn't update from higher version to lower

            QStringList last_version_split_list = latest_config_path_str.split("/", Qt::SkipEmptyParts);
            QString last_version_str = QString("");
            if(!last_version_split_list.isEmpty())
                last_version_str = last_version_split_list.at(last_version_split_list.size() -1);
            if(last_version_str.contains("-"))
            {
                QStringList tmp_split_list = last_version_str.split("-");
                last_version_str = tmp_split_list.at(0);
            }

            last_version_str.replace("_" , ".");


            QString current_app_version = recon_static_functions::get_recon_library_version();
            if(current_app_version.contains("-"))
            {
                QStringList tmp_split_list = current_app_version.split("-");
                current_app_version = tmp_split_list.at(0);
            }

            current_app_version.replace("_" , ".");


            /// =========== Update Configuration =========///
            bool status = is_newer_lib_version_available(current_app_version,last_version_str);
            if(status)
            {
                update_currently_created_configurations_with_previous_config(RECON_Library_vrsn_spcfpath , latest_config_path_str);
            }

            /// ===========Remove temporary directory=========///

            QString tmp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString()
                    + QString("tmp");

            if(QFileInfo(tmp_dir_path).exists())
            {
                recon_static_functions::do_directory_blank_completely(tmp_dir_path , Q_FUNC_INFO);
                QDir().rmdir(tmp_dir_path);
            }

        }
        else
        {
            recon_static_functions::app_debug("Destination Db opening recon configuration --------FAILED------ " + recon_configuration_db_file,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + recon_config_db.lastError().text(),Q_FUNC_INFO);
        }
    }
    else
    {
        QCoreApplication::processEvents();
        update_current_timezone_in_recon_configuration(recon_configuration_db_file);
    }

}


void MainWindow::create_app_configuration_structure(QString RECON_Library_vrsn_spcfpath)
{

    QString app_configuration_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_App_Configuration_QString).toString();

    QCoreApplication::processEvents();

    QDir dir;
    bool status = dir.mkpath(app_configuration_path);
    if(!status)
    {
        recon_static_functions::app_debug("app configuration directory creation --------FAILED------ " + app_configuration_path,Q_FUNC_INFO);
        return;
    }


    QString app_conf_db_file = app_configuration_path + QString("app_configuration.sqlite");
    QFile file_app_config(app_conf_db_file);
    if(!file_app_config.exists())
    {
        QString cmd_general = "CREATE TABLE tbl_preferences (key varchar(500), value varchar(500))";

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase app_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        app_config_db.setDatabaseName(app_conf_db_file);
        if(app_config_db.open())
        {
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_general ,app_config_db ,Q_FUNC_INFO);

            insert_app_config_tbl_preferences_details_in_db(app_config_db);

            QStringList config_timestamp_db_path_with_value = get_all_previous_recon_configuration_dirs_list_with_timestamp();

            QString last_config_path_str = get_last_run_recon_config_db_dir_path_from_paths_list(config_timestamp_db_path_with_value);

            ///======Library Updation Check=====///
            //Shouldn't update from higher version to lower

            QStringList last_version_split_list = last_config_path_str.split("/", Qt::SkipEmptyParts);
            QString last_version_str = QString("");
            if(!last_version_split_list.isEmpty())
                last_version_str = last_version_split_list.at(last_version_split_list.size() -1);
            if(last_version_str.contains("-"))
            {
                QStringList tmp_split_list = last_version_str.split("-");
                last_version_str = tmp_split_list.at(0);
            }

            last_version_str.replace("_" , ".");

            QString current_app_version = recon_static_functions::get_recon_library_version();
            if(current_app_version.contains("-"))
            {
                QStringList tmp_split_list = current_app_version.split("-");
                current_app_version = tmp_split_list.at(0);
            }

            current_app_version.replace("_" , ".");

            /// =========== Update Configuration =========///
            bool status = is_newer_lib_version_available(current_app_version,last_version_str);

            if(status)
            {
                update_currently_created_app_configurations_with_previous_app_config(RECON_Library_vrsn_spcfpath , last_config_path_str);
            }

            app_config_db.close();
        }

    }

}


void MainWindow::update_current_app_config_from_previous_in_db(QSqlDatabase *current_config_db , QSqlDatabase *previous_config_db , QString select_command_str , QString insert_or_update_command_str , int status_update_for , QStringList already_existing_values_list)
{
    recon_static_functions::app_debug("-Start -:" + QString::number(status_update_for),Q_FUNC_INFO);

    if(select_command_str.isEmpty())
        return;

    if(insert_or_update_command_str.isEmpty())
        return;

    /// select previous values
    QSqlQuery query_previous(*previous_config_db);
    query_previous.prepare(select_command_str);
    if(!query_previous.exec())
    {
        recon_static_functions::app_debug(" previous query  --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_command_str, Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_previous.lastError().text(),Q_FUNC_INFO);
        return;
    }


    /// insert previous values to current db

    QSqlQuery query_current(*current_config_db);

    while(query_previous.next())
    {
        if(status_update_for == enum_mainwindow_app_configurations_preferences)
        {
            query_current.prepare(insert_or_update_command_str);

            QString key    = query_previous.value(0).toString();
            QString value  = query_previous.value(1).toString();


            query_current.addBindValue(key);
            query_current.addBindValue(value);

            if(!query_current.exec())
            {
                recon_static_functions::app_debug(" current query (projects) --------FAILED------ " ,Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_or_update_command_str, Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_current.lastError().text(),Q_FUNC_INFO);
            }
        }
        else
        {

            recon_static_functions::app_debug(" status update for not found  --------FAILED------ " ,Q_FUNC_INFO);
            break;
        }
    }


    recon_static_functions::app_debug("-End",Q_FUNC_INFO);

}
