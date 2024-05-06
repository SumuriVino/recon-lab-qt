#include "case_info_details.h"

case_info_details::case_info_details(QWidget *)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

void case_info_details::write_case_info_in_textfile()
{ //store all the case info in text file
    recon_static_functions::app_debug(" write_case_info_in_textfile start",Q_FUNC_INFO);

    QString file_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

    QFile file(file_name);
    if(!file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" write_case_info_in_textfile ---FAILED--- to open case_info.txt file " + file_name,Q_FUNC_INFO);
        recon_static_functions::app_debug(" write_case_info_in_textfile error -  " + file.errorString(),Q_FUNC_INFO);
        return;
    }


    file.write(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64 + QString("=" + QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64).toLongLong()) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64 + QString("=" + QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64).toLongLong()) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Created_Timezone_qint64 + QString("=" + QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_qint64).toLongLong()) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Created_Timezone_QString + QString("=" + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_QString).toString()+ "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_ID_QString + QString("=" + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Name_QString + QString("=" + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Notes_QString + QString("=" + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString().replace("\n", "*-NewLineSub-*") + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Examiner_Selected_Timezone_QString + QString("=" + global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Examiner_Selected_Timezone_qint64 + QString("=" +  QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong()) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Examiner_Selected_Date_Format_int + QString("=" +  QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt()) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Examiner_Name_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Result_App_Version_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Result_Build_Version_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_Build_Version_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Examiner_Email_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Examiner_Phone_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Location_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Agency_Name_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Agency_Address_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Case_Agency_Logo_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString() + "\n").toLocal8Bit());



    /// inserting exif data selection status while new case
    bool bool_exif_data_select_status = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool).toBool();

    QString exif_data_select_status;

    if(bool_exif_data_select_status)
        exif_data_select_status = QString("1");
    else
        exif_data_select_status = QString("0");



    file.write(MACRO_NARAD_Exif_Metadata_Check_Status_bool + QString("=" +  exif_data_select_status + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Examiner_Selected_Date_Format_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Result_Updated_For_Versions_QString + QString("=" +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_Updated_For_Versions_QString).toString() + "\n").toLocal8Bit());


    file.flush();
    file.close();

    recon_static_functions::app_debug(" write_case_info_in_textfile end",Q_FUNC_INFO);
}

void case_info_details::write_case_info_in_database()
{ // write case info in database also
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + destination_db.lastError().text(),Q_FUNC_INFO);
    }


    QString command = QString("insert into 'casedetails' (app_version,build_version,result_updated_for_versions,caseno, casename,location,casenotes, case_created_time_utc,"
                              "case_created_time_with_zone ,machine_timezone_offset , machine_timezone_string) values(?,?,?,?,?, ?,?,?,?,?, ?)");


    QStringList list_values;

    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString(); //recon_version
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_Build_Version_QString).toString(); //build_version

    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_Updated_For_Versions_QString).toString(); //result_updated_for_versions
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString(); //caseno
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(); //casename
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString(); //location
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString(); //casenotes
    list_values << QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64).toLongLong());
    list_values << QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64).toLongLong());//Case_crreated_time_with_zone
    list_values << QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_qint64).toLongLong());//Case_created_timezone_sec
    list_values << (global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_QString).toString());//case_created_time_zone_string

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list_values,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" query insert casedetails ---FAILED---",Q_FUNC_INFO);
    }


    destination_db.close();

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}


void case_info_details::extract_details_for_case_info_from_db()
{ // extract case info data from database to use in code and display in software
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + destination_db.lastError().text(),Q_FUNC_INFO);
    }


    QSqlQuery query_select(destination_db);

    // Timezone Details

    query_select.exec("select case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,examiner_selected_timezone_qstring,examiner_selected_timezone_qint64,"
                      "examiner_selected_date_format_int,examiner_selected_date_format_qstring from timezone_details");
    if(query_select.next())
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString,query_select.value("examiner_selected_timezone_qstring").toString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64,query_select.value("examiner_selected_timezone_qint64").toLongLong());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_int,query_select.value("examiner_selected_date_format_int").toInt());
        QString examiner_selected_date_format = query_select.value("examiner_selected_date_format_qstring").toString();
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_QString,examiner_selected_date_format);

        if(!examiner_selected_date_format.trimmed().isEmpty())
        {
            QString selected_datetime_format = examiner_selected_date_format + QString(" hh:mm:ss");
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString,selected_datetime_format);

        }

        struct_global_default_timezone_details obj;
        obj.d_case_created_machine_timezn_readable_QString = query_select.value("case_created_machine_timezone_qstring").toString();
        obj.d_case_created_machine_timezn_numeric_qint64 = query_select.value("case_created_machine_timezone_qint64").toString().toLongLong();
        QString examiner_selected_t_zone = query_select.value("examiner_selected_timezone_qstring").toString();
        obj.d_examiner_selected_timezone_readable_QString = examiner_selected_t_zone;
        obj.d_examiner_selected_date_format_numeric_qint64 = query_select.value("examiner_selected_date_format_int").toString().toInt();
        obj.d_examiner_selected_date_format_readable_QString = query_select.value("examiner_selected_date_format_qstring").toString() + " " + "hh:mm:ss";
        obj.d_examiner_selected_timezone_numeric_qint64 = query_select.value("examiner_selected_timezone_qint64").toString().toLongLong();

        if(examiner_selected_t_zone.contains("GMT") || examiner_selected_t_zone.contains("GMT-GMT"))
        {
            QStringList tm_list;
            if(examiner_selected_t_zone.contains("GMT-GMT"))
                tm_list = examiner_selected_t_zone.split("GMT-GMT");
            else
                tm_list = examiner_selected_t_zone.split("GMT");

            if(examiner_selected_t_zone.size() > 1)
                examiner_selected_t_zone = tm_list.at(1);

            obj.timezone_offset_name = examiner_selected_t_zone;
        }
        else
        {
            examiner_selected_t_zone = "+00:00";
            obj.timezone_offset_name = examiner_selected_t_zone;
        }

        global_recon_helper_singular_class_obj->pub_set_default_timezone(obj);

    }
    else
    {
        recon_static_functions::app_debug("query ----FAILED---  timezone_details ",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + query_select.lastError().text(),Q_FUNC_INFO);
    }

    //-ExaminerAnd Agency Info.
    QSqlQuery query_plugin(destination_db);
    QString command_plugin = QString("select agency_name,examiner,examiner_phone,examiner_email,agency_address from agency_details");

    query_plugin.prepare(command_plugin);
    if(!query_plugin.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_plugin,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_plugin.lastError().text(),Q_FUNC_INFO);
    }

    if(query_plugin.next())
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Name_QString,query_plugin.value(0).toString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Name_QString,query_plugin.value(1).toString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Phone_QString,query_plugin.value(2).toString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Email_QString,query_plugin.value(3).toString());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Address_QString,query_plugin.value(4).toString());
    }

    destination_db.close();



    QString case_config_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";
    connection_naam = QString(Q_FUNC_INFO) + QString("_1");
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase case_conf_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    case_conf_db.setDatabaseName(case_config_db_file);
    if(case_conf_db.open())
    {
        QSqlQuery query_case_details(case_conf_db);

        //-Caseinfo
        query_case_details.exec("select app_version,build_version,caseno, casename,location,casenotes,result_updated_for_versions,case_created_time_utc from casedetails");

        if(query_case_details.next())
        {
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_ID_QString,query_case_details.value("caseno").toString().trimmed());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Name_QString,query_case_details.value("casename").toString().trimmed());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Location_QString,query_case_details.value("location").toString());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Notes_QString,query_case_details.value("casenotes").toString().trimmed());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_Updated_For_Versions_QString,query_case_details.value("result_updated_for_versions").toString().trimmed());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64,query_case_details.value("case_created_time_utc").toLongLong());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64,query_case_details.value("case_created_time_with_zone").toLongLong());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timezone_qint64,query_case_details.value("machine_timezone_offset").toLongLong());
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timezone_QString,query_case_details.value("machine_timezone_string").toString());
        }
        else
        {
            recon_static_functions::app_debug("query ----FAILED---  casedetails ",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error--- " + query_case_details.lastError().text(),Q_FUNC_INFO);
        }

        case_conf_db.close();
    }
    else
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + case_config_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + case_conf_db.lastError().text(),Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void case_info_details::extract_case_info_from_textfile()
{ // extract case info data from text file to use in code and display in software
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString file_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ----FAILED---" + file_name,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + file.errorString(),Q_FUNC_INFO);
        return;
    }

    QStringList line_list;

    while(!file.atEnd())
        line_list << file.readLine();

    file.close();

    for(int i = 0; i < line_list.size(); i++)
    {
        QString line = line_list.at(i);

        if(line.contains(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64 + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64).toLongLong() == 0)
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64,line.remove(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64 + QString("=")).trimmed().toLongLong());
        }
        else if(line.contains(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64 + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64).toLongLong() == 0)
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64,line.remove(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64 + QString("=")).trimmed().toLongLong());
        }
        else if(line.contains(MACRO_NARAD_Case_Created_Timezone_qint64 + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_qint64).toLongLong() == 0)
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timezone_qint64,line.remove(MACRO_NARAD_Case_Created_Timezone_qint64 + QString("=")).trimmed().toLongLong());
        }
        else if(line.contains(MACRO_NARAD_Case_Created_Timezone_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_QString).toString() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timezone_QString,line.remove(MACRO_NARAD_Case_Created_Timezone_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_ID_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_ID_QString,line.remove(MACRO_NARAD_Case_ID_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_Name_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Name_QString,line.remove(MACRO_NARAD_Case_Name_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_Notes_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Notes_QString,line.remove(MACRO_NARAD_Case_Notes_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Examiner_Name_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Name_QString,line.remove(MACRO_NARAD_Examiner_Name_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Examiner_Selected_Timezone_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString,line.remove(MACRO_NARAD_Examiner_Selected_Timezone_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Examiner_Selected_Timezone_qint64 + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong() == 0)
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64,line.remove(MACRO_NARAD_Examiner_Selected_Timezone_qint64 + QString("=")).trimmed().toLongLong());
        }
        else if(line.contains(MACRO_NARAD_Examiner_Selected_Date_Format_int + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt() == 0)
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_int,line.remove(MACRO_NARAD_Examiner_Selected_Date_Format_int + QString("=")).trimmed().toInt());
        }
        else if(line.contains(MACRO_NARAD_Examiner_Phone_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Phone_QString,line.remove(MACRO_NARAD_Examiner_Phone_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Examiner_Email_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Email_QString,line.remove(MACRO_NARAD_Examiner_Email_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_Location_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Location_QString,line.remove(MACRO_NARAD_Case_Location_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_Agency_Name_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Name_QString,line.remove(MACRO_NARAD_Case_Agency_Name_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_Agency_Address_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Address_QString,line.remove(MACRO_NARAD_Case_Agency_Address_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Case_Agency_Logo_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,line.remove(MACRO_NARAD_Case_Agency_Logo_QString + QString("=")).trimmed());
        }
        else if(line.contains(MACRO_NARAD_Exif_Metadata_Check_Status_bool + QString("=")))
        {

            bool bool_filetypes_create = false;

            QString line_value_str = line.remove(MACRO_NARAD_Exif_Metadata_Check_Status_bool + QString("=")).trimmed();

            if(line_value_str == QString("1"))
                bool_filetypes_create = true;

            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool).toBool() == false)
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool, bool_filetypes_create);

        }
        else if(line.contains(MACRO_NARAD_Result_Updated_For_Versions_QString + QString("=")))
        {
            if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_Updated_For_Versions_QString).toString().trimmed() == "")
                global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_Updated_For_Versions_QString,line.remove(MACRO_NARAD_Result_Updated_For_Versions_QString + QString("=")).trimmed());
        }


    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}


void case_info_details::pub_write_case_info_in_textfile_and_db()
{ // write case info data in text file and database
    write_case_info_in_database();
    write_case_info_in_textfile();
}

void case_info_details::pub_extract_full_caseinfo()
{ // extract full case info from db and txt file
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Debug_File_Path_QString,recon_static_functions::get_debug_filepath_regular());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Plugin_Result_Dir_QString,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS);
    // global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,"../icons/agency_logo.png");

    QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,agency_logo_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64,0);

    ///This code is for get Current machine Timezone For Load Case.

    struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
    QString current_machine_timezone_str = strct_exm_obj.timezone_name_complete_QString;

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Current_Machine_Timezone_QString,current_machine_timezone_str);

    extract_details_for_case_info_from_db();
    recon_helper_standard_obj->update_narad_variable_with_apple_metadata_info(Q_FUNC_INFO);
    recon_helper_standard_obj->update_narad_variable_with_exif_metadata_info(Q_FUNC_INFO);
    extract_case_info_from_textfile();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

