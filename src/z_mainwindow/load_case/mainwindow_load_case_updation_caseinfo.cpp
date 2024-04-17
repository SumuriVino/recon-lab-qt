#include "z_mainwindow/mainwindow.h"

void MainWindow::update_result_necessity_for_case_info()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    if(!bool_need_to_update_result_for_load_case)
        return;


    QString version_number_of_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString().trimmed();

    if(version_number_of_result.isEmpty())
        return;


    if(version_number_of_result == "1.0.0" || version_number_of_result == "1.0.1"
            || version_number_of_result == "1.0.2" || version_number_of_result == "1.0.3"
            || version_number_of_result == "1.0.4" || version_number_of_result == "1.0.5"
            || version_number_of_result == "1.0.6" || version_number_of_result == "1.0.7"
            || version_number_of_result == "1.0.8" || version_number_of_result == "1.0.8.1"
            || version_number_of_result == "1.0.9" || version_number_of_result == "1.1.0"
            || version_number_of_result == "1.1.1" || version_number_of_result == "1.1.2"
            || version_number_of_result == "1.1.3")
    {
        update_case_info_100_to_114();
        update_case_info_114_to_133();
        update_case_info_133_to_142();
        update_case_info_142_to_148();

    }
    else if(version_number_of_result == "1.1.4" || version_number_of_result == "1.1.5"
            || version_number_of_result == "1.1.6" || version_number_of_result == "1.1.7"
            || version_number_of_result == "1.1.8" || version_number_of_result == "1.1.9"
            || version_number_of_result == "1.2.0" || version_number_of_result == "1.2.1"
            || version_number_of_result == "1.2.2" || version_number_of_result == "1.2.3"
            || version_number_of_result == "1.2.4" || version_number_of_result == "1.2.5"
            || version_number_of_result == "1.2.6" || version_number_of_result == "1.2.7"
            || version_number_of_result == "1.2.8" || version_number_of_result == "1.2.9"
            || version_number_of_result == "1.3.0" || version_number_of_result == "1.3.1"
            || version_number_of_result == "1.3.2")

    {
        update_case_info_114_to_133();
        update_case_info_133_to_142();
        update_case_info_142_to_148();

    }
    else if(version_number_of_result == "1.3.3" || version_number_of_result == "1.3.4"
            || version_number_of_result == "1.3.5" || version_number_of_result == "1.3.6"
            || version_number_of_result == "1.3.7" || version_number_of_result == "1.3.8"
            || version_number_of_result == "1.3.9" || version_number_of_result == "1.4.0"
            || version_number_of_result == "1.4.1")
    {
        update_case_info_133_to_142();
        update_case_info_142_to_148();

    }
    else if(version_number_of_result == "1.4.2" || version_number_of_result == "1.4.3"
            || version_number_of_result == "1.4.4" || version_number_of_result == "1.4.5"
            || version_number_of_result == "1.4.6" || version_number_of_result == "1.4.7")
    {
        update_case_info_142_to_148();
    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_case_info_100_to_114()
{
    ///==========Update case info database and text file===========///
    QString case_details_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";

    ///======Update in database
    QFileInfo file_info_case_info(case_details_db_path);
    if(file_info_case_info.exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(case_details_db_path);
        if(dest_db.open())
        {
            QString table_name = "casedetails";

            QStringList colms_name_list;
            QStringList colms_field_type_list;
            colms_name_list   << "case_created_time_utc";
            colms_field_type_list  << "varchar(50)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),colms_name_list, colms_field_type_list, dest_db);

            /// update columns values
            QString update_command = "Update " + table_name + " Set case_created_time_utc = '0'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(update_command, dest_db, Q_FUNC_INFO);
            dest_db.close();
        }
        else
        {
            recon_static_functions::app_debug(" Destination Db opening(case details) --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    ///======Update in Text File
    QString case_text_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

    QFile file(case_text_file_path);
    if(file.exists() && file.open(QIODevice::ReadWrite))
    {
        QStringList file_data_list;

        while(!file.atEnd())
        {
            QString line_str = QString::fromLocal8Bit(file.readLine()).trimmed();

            if(line_str.startsWith(QString("MACRO_NARAD_Case_Created_Timestamp_UTC_qint64")))
                continue;

            file_data_list << line_str;
        }

        file_data_list << QString("MACRO_NARAD_Case_Created_Timestamp_UTC_qint64") + QString("=0");

        QString data_str = file_data_list.join("\n");

        file.resize(0);
        file.write(data_str.toLocal8Bit());

        file.flush();
        file.close();
    }
}

void MainWindow::update_case_info_114_to_133()
{
    ///==========Update case info database and text file===========///
    QString case_details_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";

    ///======Update in database
    QFileInfo file_info_case_info(case_details_db_path);
    if(file_info_case_info.exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(case_details_db_path);
        if(dest_db.open())
        {

            QString table_name = "casedetails";

            QStringList colms_name_list;
            QStringList colms_field_type_list;
            colms_name_list   << "case_created_time_with_zone" << "machine_timezone_offset" << "machine_timezone_string";
            colms_field_type_list  << "varchar(100)"  << "varchar(100)" << "varchar(100)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),colms_name_list, colms_field_type_list, dest_db);

            /// update columns values

            QString update_command = "Update " + table_name + " Set case_created_time_with_zone = '0' , machine_timezone_offset = '0', machine_timezone_string = '""' ";

            recon_helper_standard_obj->execute_db_command_by_dbreference(update_command, dest_db, Q_FUNC_INFO);
            dest_db.close();

        }
        else
        {
            recon_static_functions::app_debug(" Destination Db opening(case details) --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        }

    }
    ///======Update in Text File
    QString case_text_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

    QFile file(case_text_file_path);
    if(file.exists() && file.open(QIODevice::ReadWrite))
    {
        QStringList file_data_list;

        while(!file.atEnd())
        {
            QString line_str = QString::fromLocal8Bit(file.readLine()).trimmed();

            if(line_str.startsWith(QString("MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64")) || line_str.startsWith(QString("MACRO_NARAD_Case_Created_Timezone_qint64"))
                    || line_str.startsWith(QString("MACRO_NARAD_Case_Created_Timezone_QString")))
                continue;

            file_data_list << line_str;
        }

        file_data_list << QString("MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64") + QString("=0");
        file_data_list << QString("MACRO_NARAD_Case_Created_Timezone_qint64") + QString("=0");
        file_data_list << QString("MACRO_NARAD_Case_Created_Timezone_QString") + QString("=");

        QString data_str = file_data_list.join("\n");

        file.resize(0);
        file.write(data_str.toLocal8Bit());

        file.flush();
        file.close();
    }
    else
    {
        recon_static_functions::app_debug(" File opening --------FAILED------ " + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + file.errorString(),Q_FUNC_INFO);
    }

}

void MainWindow::update_case_info_133_to_142()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString case_text_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";
    QString date_format = get_value_of_key_from_source_textinfo_file_from_100_to_148(case_text_file_path, "MACRO_NARAD_Examiner_Selected_Date_Format_qint64");
    date_format.prepend("MACRO_NARAD_Examiner_Selected_Date_Format_int=");

    recon_static_functions::remove_data_contain_line_from_file(case_text_file_path,"MACRO_NARAD_Examiner_Selected_Date_Format_qint64=",Q_FUNC_INFO);
    recon_static_functions::append_data_into_file(case_text_file_path,date_format,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void MainWindow::update_case_info_142_to_148()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //==========Update case info database Start===========//
    QString case_details_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";

    QFileInfo file_info_case_info(case_details_db_path);
    if(file_info_case_info.exists())
    {
        QString table_name = "casedetails";

        QStringList colms_name_list;
        QStringList colms_field_type_list;
        colms_name_list   << "build_version";
        colms_field_type_list  << "varchar(50)";

        recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbpath(QStringList(table_name),colms_name_list, colms_field_type_list, case_details_db_path);

    }

    QString drop_command = QString("DROP table miscellaneous");
    recon_helper_standard_obj->execute_db_command_by_dbpath(drop_command,case_details_db_path,Q_FUNC_INFO);

    //==========Update case info database End=============//


    //==========Update case info txt file Start===========//

    QString case_text_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

    QString app_version = get_value_of_key_from_source_textinfo_file_from_100_to_148(case_text_file_path, "MACRO_NARAD_Result_Version_QString");
    recon_static_functions::remove_data_contain_line_from_file(case_text_file_path,"MACRO_NARAD_Result_Version_QString=" ,Q_FUNC_INFO);
    recon_static_functions::append_data_into_file(case_text_file_path,"MACRO_NARAD_Result_App_Version_QString=" + app_version ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    recon_static_functions::append_data_into_file(case_text_file_path,"MACRO_NARAD_Result_Build_Version_QString=" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    recon_static_functions::remove_data_contain_line_from_file(case_text_file_path,"MACRO_NARAD_RECON_Library_Version_Specific_Dir_Path_QString=" ,Q_FUNC_INFO);


    //==========Update case info txt file End=============//


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}
