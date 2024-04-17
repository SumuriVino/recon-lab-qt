#include "witness_info_store.h"

witness_info_store::witness_info_store(QWidget *parent)
{
    recon_helper_standard_obj = new recon_helper_standard;

}


witness_info_store::~witness_info_store()
{

}

void witness_info_store::pub_insert_root_info_in_db_and_text_file()
{
    write_z_root_count_info_in_db();
    write_z_root_count_info_in_text();
    write_root_info_in_db();
    write_root_info_in_text_file();
}

void witness_info_store::write_z_root_count_info_in_db()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString root_count_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase z_root_count_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    z_root_count_db.setDatabaseName(root_count_db_file);
    if(!z_root_count_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + root_count_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + z_root_count_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command_insert_root_index = "INSERT INTO 'z_count'(z_root_count_flag,z_root_count,z_active_status) values(?,?,?)";

    QString root_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();
    QString root_count = root_count_name.remove(0,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Root_Flag).size()).trimmed();
    QStringList values_list;
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString() << root_count << MACRO_TRUE_VALUE_READABLE_QString;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert_root_index,values_list,z_root_count_db,Q_FUNC_INFO);

    z_root_count_db.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void witness_info_store::write_z_root_count_info_in_text()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString roots_count_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.txt";
    QFile root_count_file(roots_count_txt_file);
    if(!root_count_file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug("Destination Text File opening --------FAILED------ " + roots_count_txt_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + root_count_file.errorString(),Q_FUNC_INFO);
        return;
    }


    QString root_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();
    root_count_file.write(root_count_name.toLocal8Bit() + "\n");

    root_count_file.flush();
    root_count_file.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void witness_info_store::write_root_info_in_db()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command_insert_root_index = "INSERT INTO 'tbl_root_index'(root_count_name, evidence_type, root_type, source_type_display,"
                                        "source_type_internal, root_name, root_path_1, root_path_2, root_recovery_passcode, root_evidence_password, home_directory_user_name, "
                                        "evidence_number, description) values(?,?,?,?, ?,?,?,?, ?,?,?,?,?)";

    QStringList values_list;
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Evidence_Type_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Type_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Type_Display_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Type_Internal_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Name_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Path_1_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Path_2_for_fusion_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Evidence_Number_QString).toString();
    values_list << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Description_QString).toString();

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert_root_index,values_list,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" query insert root_index ---FAILED---",Q_FUNC_INFO);
    }

    destination_db.close();
    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void witness_info_store::write_root_info_in_text_file()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString root_count_file_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString() + ".txt";

    //QString complete_file_name = insert_root_count_in_root_index_txt_file(root_count_name);
    insert_root_count_in_root_index_txt_file(root_count_file_name);

    QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + root_count_file_name;

    QFile root_file(root_file_path);
    if(!root_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("  ---FAILED--- to open " + root_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("  error -  " + root_file.errorString(),Q_FUNC_INFO);
        return;
    }

    QString m_equal_str = QString(MACRO_RECON_Splitter_8_equal_hash_equal);

    root_file.write(MACRO_NARAD_Root_Count_Name_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Evidence_Type_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Evidence_Type_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Type_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Type_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Source_Type_Display_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Type_Display_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Source_Type_Internal_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Type_Internal_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Name_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Name_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Path_1_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Path_1_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Path_2_for_fusion_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Path_2_for_fusion_QString).toString() + "\n").toLocal8Bit());

    root_file.write(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Evidence_Password_Encrypted_QString).toString() + "\n").toLocal8Bit());

    root_file.write(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString+ QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Evidence_Number_QString+ QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Evidence_Number_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Description_QString+ QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Description_QString).toString() + "\n").toLocal8Bit());

    root_file.write(MACRO_NARAD_Root_Hash_MD5_QString+ QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Hash_MD5_QString).toString() + "\n").toLocal8Bit());
    root_file.write(MACRO_NARAD_Root_Hash_SHA1_QString+ QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Hash_SHA1_QString).toString() + "\n").toLocal8Bit());

    root_file.flush();
    root_file.close();
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void witness_info_store::insert_root_count_in_root_index_txt_file(QString root_count_file_name)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString roots_index_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_index.txt";
    recon_static_functions::append_data_into_file(roots_index_file_path,root_count_file_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

    //    //QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";

    //    QString file_name;

    //    QString roots_index_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_index.txt";
    //    QFile root_index_file(roots_index_file_path);
    //    if(!root_index_file.open(QIODevice::Append))
    //    {
    //        recon_static_functions::app_debug("  ---FAILED--- to open" + roots_index_file_path,Q_FUNC_INFO);
    //        recon_static_functions::app_debug("  error -  " + root_index_file.errorString(),Q_FUNC_INFO);
    //        return;
    //    }

    //    //QString command = "select root_count_name from tbl_root_index";

    //    // QStringList root_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, destination_db_file, Q_FUNC_INFO);

    //    //    for(int jj = 0; jj < root_cnt_name_list.size(); jj++)
    //    //    {
    //    //        QString root_cnt_str = root_cnt_name_list.at(jj);
    //    //        root_index_file.write(QString(root_cnt_str + ".txt\n").toLocal8Bit());
    //    //    }

    //    root_index_file.write(QString(root_count_file_name + ".txt\n").toLocal8Bit());

    //    root_index_file.flush();
    //    root_index_file.close();

    //    QString root_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();
    //    file_name = root_count_name + ".txt";

    //    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}


void witness_info_store::pub_insert_source_info_in_db_and_text_file()
{
    write_z_source_count_info_in_db();
    write_z_source_count_info_in_text();
    write_sources_info_in_db();
    write_source_info_in_text_file();
}

void witness_info_store::write_z_source_count_info_in_db()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString z_sources_count_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase z_source_count_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    z_source_count_db.setDatabaseName(z_sources_count_db_file);
    if(!z_source_count_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + z_sources_count_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + z_source_count_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    int max_source_count_name_idx = recon_helper_standard_obj->get_maximum_source_count_index_from_db_and_text_file();
    max_source_count_name_idx = max_source_count_name_idx + 1;

    QString source_count_name = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag + QString::number(max_source_count_name_idx);

    QString command_insert = "INSERT INTO 'z_count'(z_source_count_flag,z_source_count,z_active_status) values(?,?,?)";
    QStringList values_list;
    values_list << source_count_name << QString::number(max_source_count_name_idx) << MACRO_TRUE_VALUE_READABLE_QString;


    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,values_list,z_source_count_db,Q_FUNC_INFO);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Count_Name_QString,source_count_name);

    z_source_count_db.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void witness_info_store::write_z_source_count_info_in_text()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString z_sources_count_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.txt";
    QFile z_source_count_file(z_sources_count_txt_file);
    if(!z_source_count_file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug("Destination txt file opening --------FAILED------ " + z_sources_count_txt_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + z_source_count_file.errorString(),Q_FUNC_INFO);
        return;
    }

    QString source_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
    // If MACRO_NARAD_Source_Count_Name_QString is Empty while settle up by Db
    if(source_count_name.isEmpty())
    {
        int max_source_count_name_idx = recon_helper_standard_obj->get_maximum_source_count_index_from_db_and_text_file();
        max_source_count_name_idx = max_source_count_name_idx + 1;

        source_count_name = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag + QString::number(max_source_count_name_idx);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Count_Name_QString,source_count_name);
    }

    z_source_count_file.write(source_count_name.toLocal8Bit() + "\n");

    z_source_count_file.flush();
    z_source_count_file.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void witness_info_store::write_source_info_in_text_file()
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);

    QString src_cnt_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
    if(src_cnt_name.trimmed().isEmpty())
    {
        return;
    }

    QString source_count_file_name =  src_cnt_name + ".txt";
    insert_source_count_name_in_source_index_txt_file(source_count_file_name);

    QString source_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + source_count_file_name;

    QFile file(source_file_path);
    if(!file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("  ---FAILED--- to open case_info.txt file " + source_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("  error -  " + file.errorString(),Q_FUNC_INFO);
        return;
    }

    QString equal_symbol = QString(MACRO_RECON_Splitter_8_equal_hash_equal);

    file.write(MACRO_NARAD_Root_Count_Name_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Count_Name_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Hierarchy_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Hierarchy_QString).toString() + "\n").toLocal8Bit());

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Nested_bool).toBool())
        file.write(MACRO_NARAD_Source_Nested_bool + QString(equal_symbol + "1\n").toLocal8Bit());
    else
        file.write(MACRO_NARAD_Source_Nested_bool + QString(equal_symbol + "0\n").toLocal8Bit());

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Decompressed_bool).toBool())
        file.write(MACRO_NARAD_Source_Decompressed_bool + QString(equal_symbol + "1\n").toLocal8Bit());
    else
        file.write(MACRO_NARAD_Source_Decompressed_bool + QString(equal_symbol + "0\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Type_Display_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Display_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Type_Internal_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Name_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Main_Path_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Main_Path_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Path_1_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Path_2_For_Fusion_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Time_Zone_Readable_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Time_Zone_Readable_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Time_Zone_Numeric_qint64 + QString(equal_symbol + QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64).toLongLong()) + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_OS_Scheme_Display_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Display_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_OS_Scheme_Internal_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString).toString() + "\n").toLocal8Bit());

    QString system_account_str = get_system_user_and_domain_user_str();
    file.write(MACRO_NARAD_Source_System_Account_QList + QString(equal_symbol + system_account_str + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Virtual_Source_Path_QString + QString(equal_symbol + global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Product_Type_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Product_Type_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_OS_Version_Extended_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Version_Extended_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_OS_Version_Full_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Version_Full_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Country_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Country_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_City_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_City_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Latitude_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Latitude_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Longitude_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Longitude_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Build_Version_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Build_Version_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Display_Name_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Display_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Device_Name_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Device_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Id_No_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Id_No_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_GUID_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_GUID_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Product_Name_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Product_Name_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Target_Type_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Target_Type_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_iTune_Version_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_iTune_Version_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Serial_No_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Serial_No_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Imei_QStringList + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Imei_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Sim_No_Iccid_QStringList + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64 + QString(equal_symbol +  QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64).toLongLong()) + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Configured_ID_QStringList + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Configured_ID_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Main_Name_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Main_Name_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Volume_Type_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Volume_Type_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Volume_Offset_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Volume_Offset_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Volume_UUID_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Volume_UUID_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_File_System_Type_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_File_System_Type_QString).toString() + "\n").toLocal8Bit());

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool).toBool())
        file.write(MACRO_NARAD_Source_RECON_Logical_Mode_bool + QString(equal_symbol + "1\n").toLocal8Bit());
    else
        file.write(MACRO_NARAD_Source_RECON_Logical_Mode_bool + QString(equal_symbol + "0\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Disk_Identifier_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Disk_Identifier_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Role_Type_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Role_Type_QString).toString() + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64 + QString(equal_symbol + QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64).toLongLong()) + "\n").toLocal8Bit());

    file.write(MACRO_NARAD_Source_Product_Model_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Product_Model_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Bluetooth_Device_Address_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Wifi_Address_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Wifi_Address_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Locale_Language_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Locale_Language_QString).toString() + "\n").toLocal8Bit());
    file.write(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString + QString(equal_symbol +  global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString).toString() + "\n").toLocal8Bit());

    file.flush();
    file.close();

    recon_static_functions::app_debug("- START",Q_FUNC_INFO);
}

void witness_info_store::write_sources_info_in_db()
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);


    QString source_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
    if(source_count_name.trimmed().isEmpty())
    {
        return;
    }


    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    //-Insert into fs_status table
    QString command_insert_index = "INSERT INTO 'fs_status' (source_count_name, os_scheme_display,os_scheme_internal,root_count_name,"
            +  QString(MACRO_JobType_Apple_Metadata).replace(" ", "_") + " , "
            + QString(MACRO_JobType_Exif_Metadata).replace(" ", "_")  + " , "
            + QString(MACRO_JobType_Signature_Analysis).replace(" ", "_") + " , "
            + QString(MACRO_JobType_Mime_Type).replace(" ", "_") + " , "
            + QString(MACRO_JobType_Hashes).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Text_Indexing).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Face_Analysis).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Optical_Character_Recognition).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Skin_Tone_Detection).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Weapons).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Fire_Analysis).replace(" ", "_") + ", "
            + QString(MACRO_JobType_Unified_Logs).replace(" ", "_") + ", "
            + "feature_x1, feature_x2, feature_x3, feature_x4, feature_x5)"
            + " VALUES(?,?,?, ?,?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?)";

    QString os_scheme_display = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Display_QString).toString();
    QString os_scheme_internal = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString).toString();
    QStringList list_index_value;
    list_index_value << source_count_name;
    list_index_value << os_scheme_display;
    list_index_value << os_scheme_internal;
    list_index_value << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();

    QString apple_metadata_status = Macro_JobType_Completion_status_not_supported;
    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_File_System_Type_QString).toString() == MACRO_FILE_SYSTEM_TYPE_HFS
            || global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_File_System_Type_QString).toString() == MACRO_FILE_SYSTEM_TYPE_APFS
            || global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString() == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
    {
        apple_metadata_status = Macro_JobType_Completion_status_not_done;
    }

    list_index_value << apple_metadata_status;

    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;
    list_index_value << Macro_JobType_Completion_status_not_done;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert_index,list_index_value,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" insert table index ---FAILED---",Q_FUNC_INFO);
    }


    QString command_system_info_insert = "insert into 'tbl_source_index' (root_count_name,source_count_name,os_scheme_display,os_scheme_internal,"
                                         "users_list, source_type_display, source_type_internal, source_name, source_path_1,"
                                         "source_path_2_for_fusion, source_main_path, virtual_source_path, product_type,"
                                         "os_version_extended, os_version_full, lattitude, longitude, country, city, "
                                         "display_name ,device_name, id_no, guid, product_name, target_type, build_version, itune_version,"
                                         "serial_no ,phone_no_msisdn,imei ,sim_no_iccid ,latest_backup_date_readable ,latest_backup_date_numeric,"
                                         "backup_size_byte_numeric, source_main_name, volume_type,total_size_byte_numeric, free_size_byte_numeric,"
                                         "used_size_byte_numeric, file_system_type, timezone_readable, timezone_numeric, recon_logical_image,"
                                         "volume_offset, volume_uuid, configured_id, source_hierarchy ,is_source_nested,is_source_decompressed,"
                                         "source_disk_identifier, installer_timestamp_utc_numeric ,product_model, bluetooth_device_address,"
                                         "wifi_address , locale_language, source_evidence_password, role_type)"
                                         " values(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,? ,?,?)";


    QStringList list_values;
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Display_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Scheme_Internal_QString).toString();


    QString sys_acc_str = get_system_user_and_domain_user_str();
    list_values << sys_acc_str;

    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Display_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Name_QString).toString();

    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Main_Path_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Virtual_Source_Path_QString).toString();

    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Product_Type_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Version_Extended_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Version_Full_QString).toString();

    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Latitude_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Longitude_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Country_QString).toString();
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_City_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Display_Name_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Device_Name_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Id_No_QString).toString();

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_GUID_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Product_Name_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Target_Type_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Build_Version_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_iTune_Version_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Serial_No_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Phone_No_MSISDN_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma);
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Imei_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma);
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Sim_No_Iccid_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma);

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString).toString();
    list_values  << QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64).toLongLong());

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString).toString();


    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Type_Internal_QString).toString() == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal))
    {
        QFileInfo info(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Path_1_QString).toString());
        list_values  << info.fileName();
    }
    else
    {
        list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Main_Name_QString).toString();
    }

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Volume_Type_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_File_System_Type_QString).toString();

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Time_Zone_Readable_QString).toString();
    list_values  << QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Time_Zone_Numeric_qint64).toLongLong());

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_RECON_Logical_Mode_bool).toBool())
        list_values <<  QString::number(1);
    else
        list_values <<  QString::number(0);


    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Volume_Offset_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Volume_UUID_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Configured_ID_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma);
    list_values << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Hierarchy_QString).toString();

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Nested_bool).toBool())
        list_values <<  QString::number(1);
    else
        list_values <<  QString::number(0);

    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Decompressed_bool).toBool())
        list_values <<  QString::number(1);
    else
        list_values <<  QString::number(0);

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Disk_Identifier_QString).toString();

    list_values << QString::number(global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64).toLongLong());

    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Product_Model_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Bluetooth_Device_Address_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Wifi_Address_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Locale_Language_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Evidence_Password_Encrypted_QString).toString();
    list_values  << global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Role_Type_QString).toString();

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_system_info_insert,list_values,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" query insert tbl_source_index ---FAILED---",Q_FUNC_INFO);
    }

    destination_db.close();

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}

void witness_info_store::insert_source_count_name_in_source_index_txt_file(QString source_count_file_name)
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);

    QString source_index_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_index.txt";
    recon_static_functions::append_data_into_file(source_index_file_path,source_count_file_name,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


    recon_static_functions::app_debug("- END",Q_FUNC_INFO);


    //    QString file_name;

    //    QString source_index_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_index.txt";

    //    QFile file(source_index_file_path);
    //    if(!file.open(QIODevice::Append))
    //    {
    //        recon_static_functions::app_debug("  ---FAILED--- to open file " + file.fileName(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug("  error -  " + file.errorString(),Q_FUNC_INFO);
    //        return file_name;
    //    }

    //    //    int max_source_count_name_idx = 0;
    //    //    while(!file.atEnd())
    //    //    {
    //    //        QString m_name = file.readLine().trimmed();
    //    //        m_name.remove(0, QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).size()).chop(4); //.txt
    //    //        max_source_count_name_idx = qMax(max_source_count_name_idx, m_name.toInt());
    //    //    }
    //    //file_name = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag + QString::number(++max_source_count_name_idx);

    //    file_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();

    //    file.write(QString(file_name + ".txt\n").toLocal8Bit());

    //    file.flush();
    //    file.close();

    //    recon_static_functions::app_debug("- END",Q_FUNC_INFO);

    //    return file_name;

}


void witness_info_store::pub_update_root_info_in_db_and_text_file()
{
    update_root_info_db_for_source_count_name();
    update_root_info_text_file_for_source_count_name();

}

void witness_info_store::update_root_info_db_for_source_count_name()
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    QString root_info_db_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(root_info_db_file_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Db opening --------FAILED------ " + root_info_db_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString root_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();

    QString source_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();

    QString select_all_source_cnt_nm_command   = "select all_source_count_name from tbl_root_index where root_count_name = ?";



    QString select_prime_cnt_nm_command = "select prime_source_count_name from tbl_root_index where root_count_name = ?";
    QString select_nested_cnt_nm_command = "select nested_source_count_name from tbl_root_index where root_count_name = ?";

    QStringList selec_cmd_list;
    selec_cmd_list << root_count_name;


    //========================All_Source_Count_name_Update start ===============================
    QString previous_src_cnt_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_all_source_cnt_nm_command,selec_cmd_list,0,destination_db,Q_FUNC_INFO);

    QStringList previous_source_count_name_list = previous_src_cnt_name.split(MACRO_RECON_Splitter_6_comma);

    previous_source_count_name_list << source_count_name;
    previous_source_count_name_list.removeAll("");
    previous_source_count_name_list.removeDuplicates();

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Count_Name_All_QStringList, previous_source_count_name_list);


    //========================All_Source_Count_name_Update end ===============================


    bool bool_source_nested = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Nested_bool).toBool();
    //========================Prime_source_Count_name_Update start ===============================
    QString prime_src_value =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_prime_cnt_nm_command,selec_cmd_list,0,destination_db,Q_FUNC_INFO);

    QStringList tmp_prime_cnt_name_list = prime_src_value.split(MACRO_RECON_Splitter_6_comma);


    if(!bool_source_nested)
    {
        tmp_prime_cnt_name_list << source_count_name;
        tmp_prime_cnt_name_list.removeAll("");
        tmp_prime_cnt_name_list.removeDuplicates();

    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList, tmp_prime_cnt_name_list);


    //========================Prime_source_Count_name_Update end ===============================



    //========================Nested_source_Count_name_Update start ===============================
    QString nested_src_values =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_nested_cnt_nm_command,selec_cmd_list,0,destination_db,Q_FUNC_INFO);

    QStringList nested_cnt_nm_list = nested_src_values.split(MACRO_RECON_Splitter_6_comma) ;

    if(bool_source_nested)
    {

        nested_cnt_nm_list  <<  source_count_name;
        nested_cnt_nm_list.removeAll("");
        nested_cnt_nm_list.removeDuplicates();

    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList, nested_cnt_nm_list);


    //========================Nested_source_Count_name_Update end ===============================


    //=============================Root_Disk_Identifier start =========================================
    QString root_disk_identifier_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Identifier_1_QString).toString();
    QString root_node_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Node_1_QString).toString();
    QString root_disk_identifier_2_for_fusion = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString).toString();
    QString root_node_2_for_fusion = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString).toString();
    //=============================Root_Disk_Identifier end =========================================





    QString command = "Update tbl_root_index set all_source_count_name = ?, prime_source_count_name = ?, nested_source_count_name = ?, root_disk_identifier_1 = ?, root_disk_node_1 = ?, root_disk_identifier_2 = ?, root_disk_node_2 = ? where root_count_name = ?";


    QStringList val_list;
    val_list  << previous_source_count_name_list.join(MACRO_RECON_Splitter_6_comma)
              << tmp_prime_cnt_name_list.join(MACRO_RECON_Splitter_6_comma)
              << nested_cnt_nm_list.join(MACRO_RECON_Splitter_6_comma)
              << root_disk_identifier_1 <<  root_node_1 <<  root_disk_identifier_2_for_fusion
              << root_node_2_for_fusion << root_count_name;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,val_list,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" query insert Root_info ---FAILED---",Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" end ",Q_FUNC_INFO);

}

void witness_info_store::update_root_info_text_file_for_source_count_name()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString root_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Count_Name_QString).toString();
    QString file_name = root_count_name + ".txt";

    QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + file_name;

    QFile root_file(root_file_path);
    QStringList tmp_data_list;

    if(!root_file.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("  ---FAILED--- to open " + root_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("  error -  " + root_file.errorString(),Q_FUNC_INFO);
        return;
    }
    while(!root_file.atEnd())
    {
        QString root_line_data = root_file.readLine();
        if(root_line_data.size() <= 0)
            continue;

        //These Conditions is for Skip the previous entry from the text files and update new entry.
        if(root_line_data.contains(MACRO_NARAD_Root_Source_Count_Name_All_QStringList)
                || root_line_data.contains(MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList)
                || root_line_data.contains(MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList)
                || root_line_data.contains(MACRO_NARAD_Root_Disk_Identifier_1_QString)
                || root_line_data.contains(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString)
                || root_line_data.contains(MACRO_NARAD_Root_Disk_Node_1_QString)
                || root_line_data.contains(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString))
        {
            continue;
        }

        tmp_data_list <<  root_line_data;
    }

    //Resize this because update the source_count_name so remove the previous entry
    root_file.resize(0);

    QString m_equal_str = QString(MACRO_RECON_Splitter_8_equal_hash_equal);

    QString all_source_count_name = MACRO_NARAD_Root_Source_Count_Name_All_QStringList + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Count_Name_All_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit();
    QString prime_source_count_name = MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit();
    QString nested_source_count_name = MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList).toStringList().join(MACRO_RECON_Splitter_6_comma) + "\n").toLocal8Bit();


    QString root_disk_identifier_1 = MACRO_NARAD_Root_Disk_Identifier_1_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Identifier_1_QString).toString() + "\n").toLocal8Bit();
    QString root_disk_node_1 =  MACRO_NARAD_Root_Disk_Node_1_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Node_1_QString).toString() + "\n").toLocal8Bit();
    QString root_disk_identifier_2 = MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString).toString() + "\n").toLocal8Bit();
    QString root_disk_node_2 =  MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString + QString(m_equal_str + global_narad_muni_class_obj->get_field(MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString).toString() + "\n").toLocal8Bit();

    tmp_data_list.append(all_source_count_name);
    tmp_data_list.append(prime_source_count_name);
    tmp_data_list.append(nested_source_count_name);
    tmp_data_list.append(root_disk_identifier_1);
    tmp_data_list.append(root_disk_node_1);
    tmp_data_list.append(root_disk_identifier_2);
    tmp_data_list.append(root_disk_node_2);

    QString new_data = tmp_data_list.join("");

    root_file.write(new_data.toLocal8Bit());

    root_file.flush();
    root_file.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}


void witness_info_store::pub_update_apfs_snapshots_info_in_db()
{
    recon_static_functions::app_debug(" -Start", Q_FUNC_INFO);

    QStringList saved_snapshots_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList).toStringList();
    if(saved_snapshots_list.isEmpty())
        return;

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery insert_query(destination_db);

    QString source_count_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();

    for(int count = 0; count < saved_snapshots_list.size(); count++)
    {
        QString snapshot_name = saved_snapshots_list.at(count).trimmed();
        if(snapshot_name.isEmpty())
            continue;

        insert_query.prepare("Insert into snapshots(snapshot_name, source_count_name, fs_run_status) Values(?,?,?)");

        insert_query.addBindValue(snapshot_name);
        insert_query.addBindValue(source_count_name);
        insert_query.addBindValue(QString("0"));

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug("query insert --------FAILED------ " + insert_query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();

    recon_static_functions::app_debug(" -End", Q_FUNC_INFO);

}

void witness_info_store::pub_update_fs_run_status_snapshots_db(QString snapshot_name, QString source_count_name, QString value_str)
{
    ///update in snapshot table in sources db
    QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString cmd_src_update = QString("Update snapshots set fs_run_status = ? Where snapshot_name = ? AND source_count_name = ?");
    QStringList val_list;
    val_list << value_str << snapshot_name << source_count_name;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_src_update, val_list, sources_db_path, Q_FUNC_INFO);
}


QString witness_info_store::get_system_user_and_domain_user_str()
{
    QList <struct_global_system_account_info> list_sys_account_info;
    list_sys_account_info.clear();

    QList <QVariant> qvariant_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_System_Account_QList).toList();

    for(int pp = 0; pp < qvariant_list.size(); pp++)
    {
        QVariant var = qvariant_list.at(pp);

        if(var.canConvert<QList<struct_global_system_account_info>>())
        {
            list_sys_account_info = var.value<QList<struct_global_system_account_info>>();
        }
    }

    QStringList all_user_lists;

    for(int qq = 0; qq < list_sys_account_info.size(); qq++)
    {
        struct_global_system_account_info mobj = list_sys_account_info.at(qq);

        QStringList users_list ;
        users_list << QString(MACRO_System_Account_List_Prefix_SYSTEM_USER) + "'" + mobj.system_user + "'" << QString( MACRO_System_Account_List_Prefix_DOMAIN_USER) + "'" + mobj.domain_user + "'";

        QString users_str = users_list.join(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon);
        all_user_lists << users_str;

    }


    QString system_accounts_str = all_user_lists.join(MACRO_RECON_Splitter_5_resolution_scope);

    return system_accounts_str;

}
