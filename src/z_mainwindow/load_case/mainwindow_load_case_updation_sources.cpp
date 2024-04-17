#include "z_mainwindow/mainwindow.h"


void MainWindow::update_result_necessity_for_sources()
{

    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    if(!bool_need_to_update_result_for_load_case)
        return;


    QString version_number_of_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString().trimmed();


    if(version_number_of_result.isEmpty())
        return;


    //===============================================================//
    if(version_number_of_result == "1.0.0" || version_number_of_result == "1.0.1"
            || version_number_of_result == "1.0.2" || version_number_of_result == "1.0.3"
            || version_number_of_result == "1.0.4" || version_number_of_result == "1.0.5"
            || version_number_of_result == "1.0.6" || version_number_of_result == "1.0.7"
            || version_number_of_result == "1.0.8" || version_number_of_result == "1.0.8.1"
            || version_number_of_result == "1.0.9" || version_number_of_result == "1.1.0"
            || version_number_of_result == "1.1.1")
    {
        update_source_100_to_112();
        update_source_112_to_114();
        update_source_114_to_116();
        update_source_116_to_117();
        update_source_117_to_120();
        update_source_120_to_125();
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();

    }
    else if(version_number_of_result == "1.1.2" || version_number_of_result == "1.1.3")
    {
        update_source_112_to_114();
        update_source_114_to_116();
        update_source_116_to_117();
        update_source_117_to_120();
        update_source_120_to_125();
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.1.4" || version_number_of_result == "1.1.5")
    {
        update_source_114_to_116();
        update_source_116_to_117();
        update_source_117_to_120();
        update_source_120_to_125();
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.1.6")
    {
        update_source_116_to_117();
        update_source_117_to_120();
        update_source_120_to_125();
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.1.7" || version_number_of_result == "1.1.8" || version_number_of_result == "1.1.9")
    {
        update_source_117_to_120();
        update_source_120_to_125();
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.2.0" || version_number_of_result == "1.2.1"
            || version_number_of_result == "1.2.2" || version_number_of_result == "1.2.3"
            || version_number_of_result == "1.2.4")
    {
        update_source_120_to_125();
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.2.5" || version_number_of_result == "1.2.6"
            || version_number_of_result == "1.2.7" || version_number_of_result == "1.2.8"
            || version_number_of_result == "1.2.9" || version_number_of_result == "1.3.0"
            || version_number_of_result == "1.3.1" || version_number_of_result == "1.3.2"
            || version_number_of_result == "1.3.3" || version_number_of_result == "1.3.4")
    {
        update_source_125_to_135();
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.3.5" || version_number_of_result == "1.3.6"
            || version_number_of_result == "1.3.7" || version_number_of_result == "1.3.8"
            || version_number_of_result == "1.3.9")
    {
        update_source_135_to_140();
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
   }
    else if(version_number_of_result == "1.4.0" || version_number_of_result == "1.4.1")
    {
        update_source_140_to_142();
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.4.2" || version_number_of_result == "1.4.3"
            || version_number_of_result == "1.4.4")
    {
        update_source_142_to_145();
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.4.5")
    {
        update_source_145_to_146();
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.4.6")
    {
        update_source_146_to_147();
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.4.7")
    {
        update_source_147_to_148();
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.4.8")
    {
        update_source_148_to_149();
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.4.9")
    {
        update_source_149_to_150();
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.5.0" || version_number_of_result == "1.5.1"
            || version_number_of_result == "1.5.2")
    {
        update_source_150_to_153();
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.5.3" || version_number_of_result == "1.5.4" || version_number_of_result == "1.5.5")
    {
        update_source_153_to_156();
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.5.6" || version_number_of_result == "1.5.7" || version_number_of_result == "1.5.8" || version_number_of_result == "1.5.9")
    {
        update_source_156_to_160();
        update_source_160_to_161();
    }
    else if(version_number_of_result == "1.6.0")
    {
        update_source_160_to_161();
    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}


void MainWindow::update_source_100_to_112()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString version_number_of_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString().trimmed();


    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString db_path = conf_source + "sources_info.sqlite";
    if(QFileInfo(db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(db_path);
        if(source_db.open())
        {
            QStringList new_colname_list;
            new_colname_list <<   "is_source_decompressed" << "is_source_nested" << "source_hierarchy" << "root_source_type" << "root_source_name" << "root_source_path" << "configured_id" << "source_main_name" << "source_main_path" << "is_live_imager_source";
            new_colname_list <<   "root_count_name" << "source_type" << "parent_source_name" << "parent_source_table_name" << "source_path_1" << "item0" << "actual_source_path" << "is_live_image";

            QStringList new_coltype_list;
            new_coltype_list  << "varchar(10)" << "varchar(10)" << "varchar(500)"  << "varchar(500)"  << "varchar(500)"   << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)";
            new_coltype_list  << "varchar(10)" << "varchar(10)" << "varchar(500)"  << "varchar(500)"  << "varchar(500)"   << "varchar(500)" << "varchar(500)" << "varchar(500)";

            QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference("Select table_name from source_index", 0, source_db, Q_FUNC_INFO);
            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(source_cnt_table_list,new_colname_list, new_coltype_list, source_db);

            for(int pp = 0; pp < source_cnt_table_list.size(); pp++)
            {
                QString tbl_name = source_cnt_table_list.at(pp);

                QString main_command = "Select root_count_name,root_source_path,is_source_decompressed,is_source_nested,source_hierarchy,root_source_type,root_source_name,source_type,parent_source_name,parent_source_table_name,source_path_1 from '" + tbl_name + "'";

                QSqlQuery sql_query = recon_helper_standard_obj->get_executed_sqlquery_from_db_by_dbreference(main_command,source_db,Q_FUNC_INFO);
                QString rt_src_path = sql_query.value(1).toString();
                // QString rt_src_path = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 1, source_db, Q_FUNC_INFO).trimmed();
                if(rt_src_path.isEmpty())
                {
                    //QString src_pth_1 = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 10, source_db, Q_FUNC_INFO).trimmed();
                    QString src_pth_1 = sql_query.value(10).toString();
                    rt_src_path = src_pth_1;
                }

                //QString decomp_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 2, source_db, Q_FUNC_INFO).trimmed();
                QString decomp_str = sql_query.value(2).toString();
                if(decomp_str.isEmpty())
                {
                    decomp_str = "0";
                }

                //QString nest_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 3, source_db, Q_FUNC_INFO).trimmed();
                QString nest_str = sql_query.value(3).toString();
                if(nest_str.isEmpty())
                {
                    nest_str = "0";
                }

                //QString src_hirrcy = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 4, source_db, Q_FUNC_INFO).trimmed();
                QString src_hirrcy = sql_query.value(4).toString();
                // mean source hierarchy is accurate in 1.1.1 already
                // and also there should not be check of empty source_hierarchy
                if(version_number_of_result != "1.1.1")
                {
                    //QString rt_cnt_nm = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 0, source_db, Q_FUNC_INFO).trimmed();
                    //QString prnt_src_name = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 9, source_db, Q_FUNC_INFO).trimmed();

                    QString rt_cnt_nm = sql_query.value(0).toString();
                    QString prnt_src_name = sql_query.value(9).toString();

                    if(!prnt_src_name.trimmed().isEmpty())
                        src_hirrcy = rt_cnt_nm + (">>") + prnt_src_name;
                    else
                        src_hirrcy = rt_cnt_nm;
                }

                //QString rt_src_type = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 5, source_db, Q_FUNC_INFO).trimmed();
                QString rt_src_type = sql_query.value(5).toString();
                if(rt_src_type.isEmpty())
                {
                    //QString sorc_type = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 7, source_db, Q_FUNC_INFO).trimmed();
                    QString sorc_type = sql_query.value(7).toString();
                    rt_src_type = sorc_type;
                }

                //QString rt_src_name = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 6, source_db, Q_FUNC_INFO).trimmed();
                QString rt_src_name = sql_query.value(6).toString();
                if(rt_src_name.isEmpty())
                {
                    //QString sorc_name = recon_helper_standard_obj->get_string_from_db_by_dbreference(main_command, 8, source_db, Q_FUNC_INFO).trimmed();
                    QString sorc_name = sql_query.value(8).toString();
                    rt_src_name = sorc_name;
                }

                QString  cmd_updt = "UPDATE '" + tbl_name +"' SET root_source_path='"+ rt_src_path + "', is_source_decompressed='"+ decomp_str + "',";
                cmd_updt.append(" is_source_nested='"+ nest_str + "', source_hierarchy='"+ src_hirrcy + "',");
                cmd_updt.append(" root_source_type='"+ rt_src_type + "', root_source_name='"+ rt_src_name + "',");
                cmd_updt.append(" configured_id=item0, source_main_name=parent_source_name, source_main_path=actual_source_path, is_live_imager_source=is_live_image");

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);
            }

            source_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }

    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_source_112_to_114()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString db_path = conf_source + "sources_info.sqlite";
    if(QFileInfo(db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(db_path);
        if(source_db.open())
        {
            QStringList new_colname_list;
            new_colname_list <<   "system_users_list" << "device_display_name" << "device_id" << "device_guid" << "target_system_timezone_string" << "target_system_timezone_epoch" << "latest_backup_date";
            new_colname_list <<   "users_list" << "display_name" << "id_no" << "guid" << "timezone_string" << "timezone_epoch" << "latest_backup_date_string";

            QStringList new_coltype_list;
            new_coltype_list  << "varchar(10)" << "varchar(10)" << "varchar(500)"  << "varchar(500)"  << "varchar(500)"   << "varchar(500)" << "varchar(500)";
            new_coltype_list  << "varchar(10)" << "varchar(10)" << "varchar(500)"  << "varchar(500)"  << "varchar(500)"   << "varchar(500)" << "varchar(500)";


            QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference("Select table_name from source_index", 0, source_db, Q_FUNC_INFO);
            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(source_cnt_table_list,new_colname_list, new_coltype_list, source_db);

            for(int pp = 0; pp < source_cnt_table_list.size(); pp++)
            {
                QString tbl_name = source_cnt_table_list.at(pp);

                QString cmd_updt = ("UPDATE '" + tbl_name +"' SET users_list=system_users_list, display_name=device_display_name,");
                cmd_updt.append("id_no=device_id, guid=device_guid,");
                cmd_updt.append("timezone_string=target_system_timezone_string, timezone_epoch=target_system_timezone_epoch,");
                cmd_updt.append("latest_backup_date_string=latest_backup_date");

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);
            }

            source_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }

    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_source_114_to_116()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString db_path = conf_source + "sources_info.sqlite";
    if(QFileInfo(db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(db_path);
        if(source_db.open())
        {
            QStringList new_colname_list;
            new_colname_list <<  "added_in_case_timestamp_utc";

            QStringList new_coltype_list;
            new_coltype_list  << "varchar(10)";


            QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference("Select table_name from source_index", 0, source_db, Q_FUNC_INFO);
            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(source_cnt_table_list,new_colname_list, new_coltype_list, source_db);

            for(int pp = 0; pp < source_cnt_table_list.size(); pp++)
            {
                QString tbl_name = source_cnt_table_list.at(pp);

                QString cmd_updt = ("UPDATE '" + tbl_name +"' SET added_in_case_timestamp_utc='0'");

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);
            }

            source_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }

    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_source_116_to_117()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString db_path = conf_source + "sources_info.sqlite";
    if(QFileInfo(db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(db_path);
        if(source_db.open())
        {
            QStringList new_colname_list;
            new_colname_list <<  "recon_logical_image" << "root_disk_identifier" << "source_disk_identifier";

            QStringList new_coltype_list;
            new_coltype_list  << "varchar(10)" << "varchar(500)" << "varchar(500)";


            QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference("Select table_name from source_index", 0, source_db, Q_FUNC_INFO);
            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(source_cnt_table_list,new_colname_list, new_coltype_list, source_db);

            for(int pp = 0; pp < source_cnt_table_list.size(); pp++)
            {
                QString tbl_name = source_cnt_table_list.at(pp);

                QString cmd_updt = ("UPDATE '" + tbl_name +"' SET recon_logical_image='0'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);

                QString cmd_selct = "select source_type from '" + tbl_name + "'";
                QString src_tp = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd_selct, 0, source_db, Q_FUNC_INFO);


                cmd_updt = ("UPDATE '" + tbl_name +"' SET root_source_type='" + "Time Machine Backup" + "' where root_source_type='Time Machine'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);

                cmd_updt = ("UPDATE '" + tbl_name +"' SET source_type='" + "Time Machine Backup" + "' where source_type='Time Machine'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);

                cmd_updt = ("UPDATE '" + tbl_name +"' SET os_scheme='" + "otherOS" + "' where os_scheme='Others'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);


                if(src_tp == "iOS Backup" || src_tp == "Google Takeout"
                        || src_tp == "File" || src_tp == "RAM Image" || src_tp == "macOS Home Directory")
                {
                    cmd_updt = ("UPDATE '" + tbl_name +"' SET file_system_type=''");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);
                }
                else if(src_tp == "Time Machine")
                {
                    cmd_updt = ("UPDATE '" + tbl_name +"' SET file_system_type='" + "hfs" + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);
                }

            }

            QString cmdd_updt1 = ("UPDATE 'source_index' SET os_scheme='" + QString("otherOS") + "' where os_scheme='Others'");
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmdd_updt1, source_db, Q_FUNC_INFO);

            source_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }

    }


    QString file_path = conf_source + "sources_index.txt";


    //    QFile file(file_path);
    //    if(!file.exists())
    //    {
    //        recon_static_functions::app_debug("file not exist ----FAILED---" + file_path,Q_FUNC_INFO);
    //        recon_static_functions::app_debug("Error--- " + file.errorString(),Q_FUNC_INFO);
    //        return;
    //    }

    //    if(!file.open(QIODevice::ReadOnly))
    //    {
    //        recon_static_functions::app_debug("file open ----FAILED---" + file.fileName(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug("Error--- " + file.errorString(),Q_FUNC_INFO);
    //        return;
    //    }
    QFile file(file_path);
    QStringList source_file_name_list;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
            {
                source_file_name_list << QString::fromLocal8Bit(file.readLine()).trimmed();
            }

            file.close();
        }
    }
    for(int pp = 0; pp < source_file_name_list.size(); pp++)
    {
        QString source_file_path = conf_source + source_file_name_list.at(pp);


        QString prt_idntfr = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Volume_Parent_Identifier_QString");
        QString rt_identf = QString("MACRO_NARAD_Source_Root_Disk_Identifier_QString") + "=" + prt_idntfr;
        QString src_idntf = QString("MACRO_NARAD_Source_Disk_Identifier_QString") + "=" + prt_idntfr;
        recon_static_functions::append_data_into_file(source_file_path,rt_identf,enum_global_append_data_in_file_with_append_one_next_line, Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_file_path,src_idntf,enum_global_append_data_in_file_with_append_one_next_line, Q_FUNC_INFO);

        QString rt_src_tp = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Root_Source_Type_QString");
        if(rt_src_tp == "Time Machine")
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_Root_Source_Type_QString" , "Time Machine Backup");


        QString src_tp = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Type_QString");
        if(src_tp == "Time Machine")
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_Type_QString" , "Time Machine Backup");

        QString os_scm = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_OS_Naming_Scheme_QString");
        if(os_scm == "Others")
        {
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_OS_Naming_Scheme_QString" , "otherOS");
        }

        if(src_tp == "iOS Backup" || src_tp == "Google Takeout"
                || src_tp == "File" || src_tp == "RAM Image" || src_tp == "macOS Home Directory")
        {
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_File_System_Type_QString" , "");
        }
        else if(src_tp == "Time Machine")
        {
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_File_System_Type_QString" , "hfs");
        }

    }


    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_source_117_to_120()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString db_path = conf_source + "sources_info.sqlite";
    if(QFileInfo(db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(db_path);
        if(source_db.open())
        {

            QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference("Select table_name from source_index", 0, source_db, Q_FUNC_INFO);
            for(int pp = 0; pp < source_cnt_table_list.size(); pp++)
            {
                QString tbl_name = source_cnt_table_list.at(pp);

                QString cmd_updt = ("UPDATE '" + tbl_name +"' SET source_type='" + "ADB - Backup Folder" + "' where source_type='ADB Android Backup'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);

            }

            source_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }

    }

    QString file_path = conf_source + "sources_index.txt";
    QFile file(file_path);
    QStringList source_file_name_list;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
            {
                source_file_name_list << QString::fromLocal8Bit(file.readLine()).trimmed();
            }

            file.close();
        }
    }

    for(int pp = 0; pp < source_file_name_list.size(); pp++)
    {
        QString source_file_path = conf_source + source_file_name_list.at(pp);

        QString src_tp = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Type_QString");
        if(src_tp == "ADB Android Backup")
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_Type_QString" , "ADB - Backup Folder");
    }


    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_source_120_to_125()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString db_path = conf_source + "sources_info.sqlite";
    if(QFileInfo(db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(db_path);
        if(source_db.open())
        {
            QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference("Select table_name from source_index", 0, source_db, Q_FUNC_INFO);

            for(int pp = 0; pp < source_cnt_table_list.size(); pp++)
            {
                QString tbl_name = source_cnt_table_list.at(pp);

                QString cmd_updt = ("UPDATE '" + tbl_name +"' SET root_source_type='" + "macOS Home Directory" + "' where root_source_type='Home Directory'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);

                cmd_updt = ("UPDATE '" + tbl_name +"' SET source_type='" + "macOS Home Directory" + "' where source_type='Home Directory'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, source_db, Q_FUNC_INFO);

            }

            source_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + source_db.lastError().text(),Q_FUNC_INFO);
        }

    }

    QString file_path = conf_source + "sources_index.txt";
    QFile file(file_path);
    QStringList source_file_name_list;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
            {
                source_file_name_list << QString::fromLocal8Bit(file.readLine()).trimmed();
            }

            file.close();
        }
    }

    for(int pp = 0; pp < source_file_name_list.size(); pp++)
    {
        QString source_file_path = conf_source + source_file_name_list.at(pp);

        QString rt_src_tp = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Root_Source_Type_QString");
        if(rt_src_tp == "Home Directory")
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_Root_Source_Type_QString" , "macOS Home Directory");


        QString src_tp = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Type_QString");
        if(src_tp == "Home Directory")
            update_value_for_source_textinfo_file_only(source_file_path, "MACRO_NARAD_Source_Type_QString" , "macOS Home Directory");

    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}

void MainWindow::update_source_125_to_135()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //=============== DATABASE_TEXT_FILE Start ==========================//

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString root_info_db_path = conf_source + "roots_info.sqlite";
    QString drop_cmd = "DROP TABLE IF EXISTS tbl_root_index";
    recon_helper_standard_obj->execute_db_command_by_dbpath(drop_cmd,root_info_db_path,Q_FUNC_INFO);

    QString command_root = QString("create table tbl_root_index(INT INTEGER PRIMARY KEY, root_count_name varchar(100), root_type varchar(100), source_type varchar(100), root_name varchar(100), source_count_name varchar(100), prime_source_count_name varchar(100),nested_source_count_name varchar(100),root_path_1 varchar(100),"
                                   "root_path_2 Varchar(100), root_password varchar(100), root_disk_identifier_1 varchar(100), root_disk_node_1 varchar(100), root_disk_identifier_2 varchar(100), root_disk_node_2 varchar(100), root_md5_hash varchar(100), root_sha1_hash varchar(100), home_directory_user_name varchar(100),"
                                   "evidence_by_examiner varchar(100) , description_by_examiner varchar(1000))");

    recon_helper_standard_obj->execute_db_command_by_dbpath(command_root,root_info_db_path,Q_FUNC_INFO);
    QString source_db_path = conf_source + "sources_info.sqlite";
    QStringList root_cnt_name_list;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_info_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_info_db.setDatabaseName(source_db_path);
    //    if(!source_info_db.open())
    //    {
    //        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + source_info_db.databaseName(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + source_info_db.lastError().text(),Q_FUNC_INFO);
    //        return;
    //    }
    if(source_info_db.open())
    {
        QString select_command = "select root_count_name from source_index";
        root_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_command, 0, source_db_path, Q_FUNC_INFO);
    }

    QStringList src_cnt_name_lst;

    QString source_index_file_path = conf_source + "sources_index.txt";
    QFile src_index_file(source_index_file_path);
    //    if(!src_index_file.open(QIODevice::ReadOnly))
    //    {
    //        recon_static_functions::app_debug("File Open --------Failed--------",source_index_file_path);
    //        recon_static_functions::app_debug("Error",src_index_file.errorString());
    //    }
    if(src_index_file.open(QIODevice::ReadOnly))
    {
        while(!src_index_file.atEnd())
            src_cnt_name_lst << QString::fromLocal8Bit(src_index_file.readLine()).trimmed();

    }
    src_index_file.close();

    for(int bb = 0; bb < src_cnt_name_lst.size(); bb++)
    {
        QString sources_file_path = conf_source + src_cnt_name_lst.at(bb);
        QString root_count_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(sources_file_path, "MACRO_NARAD_Source_Root_Count_Name_QString");
        root_cnt_name_list << root_count_name;
    }
    root_cnt_name_list.removeDuplicates();
    root_cnt_name_list.sort();

    for(int jj = 0 ; jj < root_cnt_name_list.size() ; jj++)
    {
        QString root_count_name = root_cnt_name_list.at(jj);
        QString cmd_select = "select table_name from source_index where root_count_name = ?";
        QStringList val_list;
        val_list << root_count_name;
        QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd_select,val_list, 0, source_db_path, Q_FUNC_INFO);
        if(source_count_name_list.isEmpty())
        {
            for(int jj = 0; jj < src_cnt_name_lst.size(); jj++)
            {
                QString source_count_name = src_cnt_name_lst.at(jj);
                source_count_name.remove(".txt");
                QString source_file_path = conf_source + src_cnt_name_lst.at(jj);
                QString rt_cnt_nm = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_Root_Count_Name_QString");
                if(root_count_name == rt_cnt_nm)
                {
                    source_count_name_list << source_count_name;
                }
            }
        }

        QStringList nested_source_count_name_list;
        QStringList prime_source_count_name_list;

        QString root_cnt_name,root_type,source_type,root_source_name,root_source_path_1,root_source_path_2,root_source_password,evidence_by_examiner,description_by_examiner,home_directory_user_name;

        for(int ii = 0 ; ii < source_count_name_list.size(); ii++)
        {
            QString source_count_name = source_count_name_list.at(ii);

            QString source_txt_file_path = conf_source + source_count_name + ".txt";

            QString select_cmd = "select root_count_name,root_source_type,source_type,root_source_name,root_source_path,source_path_2_for_fusion,source_password,is_source_nested,users_list,evidence_by_examiner,description_by_examiner from '" +source_count_name+"'";
            QSqlQuery select_query(source_info_db);
            select_query.prepare(select_cmd);
            if(!select_query.exec())
            {
                root_cnt_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Root_Count_Name_QString");
                root_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Root_Source_Type_QString");
                source_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Type_QString");
                root_source_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Root_Source_Name_QString");
                root_source_path_1 = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Root_Source_Path_QString");
                root_source_path_2 = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Path_2_For_Fusion_QString");
                root_source_password = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Password_QString");
                evidence_by_examiner = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Evidence_By_Examiner_QString");
                description_by_examiner = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Description_By_Examiner_QString");

                if(root_type == "macOS Home Directory")
                {
                    home_directory_user_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Users_QStringList");
                }
                QString nested_value = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Nested_bool");
                if(nested_value == "1")
                {
                    nested_source_count_name_list <<  source_count_name;

                }
                else
                {
                    prime_source_count_name_list << source_count_name;

                }

            }
            else
            {
                if(select_query.next())
                {
                    root_cnt_name = select_query.value("root_count_name").toString();

                    root_type = select_query.value("root_source_type").toString();

                    source_type = select_query.value("source_type").toString();

                    root_source_name = select_query.value("root_source_name").toString();

                    root_source_path_1 = select_query.value("root_source_path").toString();

                    root_source_path_2 = select_query.value("source_path_2_for_fusion").toString();

                    root_source_password = select_query.value("source_password").toString();

                    if(root_type == "macOS Home Directory")
                    {
                        home_directory_user_name = select_query.value("users_list").toString();
                    }
                    evidence_by_examiner = select_query.value("evidence_by_examiner").toString();
                    description_by_examiner = select_query.value("description_by_examiner").toString();

                    bool is_source_nested = select_query.value("is_source_nested").toBool();
                    if(is_source_nested)
                    {
                        nested_source_count_name_list <<  source_count_name;
                    }
                    if(!is_source_nested)
                    {
                        prime_source_count_name_list << source_count_name;
                    }
                }
            }
        }

        QString prime_source_count_name = prime_source_count_name_list.join(MACRO_RECON_Splitter_5_resolution_scope);
        QString nested_source_count_name = nested_source_count_name_list.join(MACRO_RECON_Splitter_5_resolution_scope);
        QString source_count_names = source_count_name_list.join(MACRO_RECON_Splitter_5_resolution_scope);

        QString root_disk_identifier_1,root_disk_node_1,root_disk_identifier_2,root_disk_node_2;
        QString root_md5_hash,root_sha1_hash;

        QString command_insert_root_index = "INSERT INTO 'tbl_root_index'(root_count_name,source_type,root_type,root_name,source_count_name,prime_source_count_name,nested_source_count_name,root_path_1,root_path_2,"
                                            "root_password,home_directory_user_name,evidence_by_examiner, description_by_examiner,root_disk_identifier_1,root_disk_node_1,root_disk_identifier_2,root_disk_node_2,root_md5_hash,root_sha1_hash)"
                                            "values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

        QStringList values_list;
        values_list << root_count_name << source_type << root_type << root_source_name << source_count_names
                    << prime_source_count_name << nested_source_count_name << root_source_path_1 << root_source_path_2
                    << root_source_password  << home_directory_user_name << evidence_by_examiner << description_by_examiner
                    << root_disk_identifier_1 << root_disk_node_1 << root_disk_identifier_2 << root_disk_node_2 << root_md5_hash << root_sha1_hash;

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_root_index,values_list,root_info_db_path,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" query insert root_index ---FAILED---",Q_FUNC_INFO);
        }



        QString root_txt_file_path  = conf_source + root_count_name + ".txt";
        QFile root_file(root_txt_file_path);
        //        if(!root_file.open(QIODevice::ReadWrite))
        //        {
        //            recon_static_functions::app_debug("File open -----FAILED----" + root_file.fileName() , Q_FUNC_INFO);
        //            recon_static_functions::app_debug("Error " + root_file.errorString() , Q_FUNC_INFO);
        //            return;
        //        }
        if(root_file.open(QIODevice::ReadWrite))
        {
            root_file.write("MACRO_NARAD_Root_Count_Name_QString" + QString("=" + root_cnt_name + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Type_QString" + QString("=" + root_type + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Source_Type_QString" + QString("=" + source_type + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Name_QString" + QString("=" + root_source_name + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Path_1_QString" + QString("=" + root_source_path_1  + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Path_2_for_fusion_QString" + QString("=" + root_source_path_2 + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Password_Encrypted_QString"+ QString("=" + root_source_password + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString"+ QString("=" + home_directory_user_name + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Disk_Identifier_1_QString"+ QString("=" + root_disk_identifier_1 + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Disk_Node_1_QString"+ QString("=" + root_disk_node_1 + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString"+ QString("=" + root_disk_identifier_2 + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString"+ QString("=" + root_disk_node_2 + "\n").toLocal8Bit());

            root_file.write("MACRO_NARAD_Root_Source_Count_Name_QString" + QString("=" +source_count_names  + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Source_Count_Name_Prime_QString" + QString("=" + prime_source_count_name + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Source_Count_Name_Nested_QString" + QString("=" + nested_source_count_name + "\n").toLocal8Bit());

            root_file.write("MACRO_NARAD_Root_Evidence_By_Examiner_QString" + QString("=" + evidence_by_examiner + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Description_By_Examiner_QString" + QString("=" + description_by_examiner + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Hash_MD5_QString" + QString("=" + root_md5_hash + "\n").toLocal8Bit());
            root_file.write("MACRO_NARAD_Root_Hash_SHA1_QString" + QString("=" + root_sha1_hash + "\n").toLocal8Bit());

            root_file.flush();
            root_file.close();
        }



        //=============== DATABASE_TEXT_FILE end ==========================//


        ///===========================================Root_Index.txt_start=========================================
        QString roots_index_file_path = conf_source + "roots_index.txt";
        QFile root_index_file(roots_index_file_path);
        //        if(!root_index_file.open(QIODevice::WriteOnly))
        //        {
        //            recon_static_functions::app_debug("  ---FAILED--- to open" + root_index_file.fileName(),Q_FUNC_INFO);
        //            recon_static_functions::app_debug("  error -  " + root_index_file.errorString(),Q_FUNC_INFO);
        //            return;
        //        }

        if(root_index_file.open(QIODevice::WriteOnly))
        {
            for(int qq = 0 ; qq < root_cnt_name_list.size(); qq++)
            {
                QString root_count_name = root_cnt_name_list.at(qq);
                root_index_file.write(QString(root_count_name + ".txt\n").toLocal8Bit());
            }

            root_index_file.flush();
            root_index_file.close();
        }

        ///===========================================Root_Index.txt_End===========================================

        recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

    }

    //===========================================Source_Info.txt_Start===========================================
    for(int ii = 0 ; ii < src_cnt_name_lst.size(); ii++)
    {
        QString source_txt_file_path = conf_source + src_cnt_name_lst.at(ii);
        QString recived_rt_cnt_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Root_Count_Name_QString");
        QString root_count_name = QString("MACRO_NARAD_Root_Count_Name_QString=") + recived_rt_cnt_name;
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Root_Count_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,root_count_name,enum_global_append_data_in_file_with_append_one_next_line, Q_FUNC_INFO);
    }
    //===========================================Source_Info.txt_End===========================================
    source_info_db.close();

}

void MainWindow::update_source_135_to_140()
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    //=================================Source_Db_Start==============================//
    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select table_name from source_index", 0, destination_db_file, Q_FUNC_INFO);

    for(int jj = 0; jj < source_cnt_table_list.size(); jj++)
    {
        QString table_name = source_cnt_table_list.at(jj);
        QString cmd = "ALTER TABLE '" + table_name + "' ADD  installer_timestamp_utc";
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, destination_db_file, Q_FUNC_INFO);

        QString source_cnt_name = source_cnt_table_list.at(jj);
        QString destination_text_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + source_cnt_name + ".txt";
        QFile txt_file(destination_text_file);
        if(!txt_file.open(QIODevice::WriteOnly | QFile::Append))
        {
            recon_static_functions::app_debug("File Open -----FAILED---- " + txt_file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + txt_file.errorString(), Q_FUNC_INFO);
        }

        txt_file.write(("MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_qint64" + QString("=0") + "\n").toLocal8Bit());

        txt_file.flush();
        txt_file.close();
    }
    //==============================Source_Text_END=================================//

    //==========================================Source_Hierarchy_Start=========================//
    //Update source hierarchy for these version only
    //because in the rest version source hierarchy information was updated

    QString version_number_of_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString().trimmed();


    if(version_number_of_result == "1.3.6" || version_number_of_result == "1.3.7"
            || version_number_of_result == "1.3.8" || version_number_of_result == "1.3.9")
    {
        for(int kk = 0 ; kk < source_cnt_table_list.size(); kk++)
        {
            QString table_name = source_cnt_table_list.at(kk);
            QString source_hierarchy_value = recon_helper_standard_obj->get_string_from_db_by_dbpath("SELECT source_hierarchy from '" + table_name + "'" ,0,destination_db_file,Q_FUNC_INFO);
            QString is_source_nested_value = recon_helper_standard_obj->get_string_from_db_by_dbpath("SELECT is_source_nested from '" + table_name + "'" ,0,destination_db_file,Q_FUNC_INFO);
            QString root_cnt_name = recon_helper_standard_obj->get_string_from_db_by_dbpath("SELECT root_count_name from '" + table_name +"'" , 0,destination_db_file,Q_FUNC_INFO);

            //This check is for the update source hierarchy for prime sources(sources which are come from root)
            if(source_hierarchy_value.isEmpty() && is_source_nested_value.toLongLong() == 0)
            {
                QString update_command = "UPDATE '" + table_name +"' set source_hierarchy = ?";
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,QStringList(root_cnt_name),destination_db_file,Q_FUNC_INFO);
            }
            //This check is for the update source hierarchy for nested sources(sources which are extracted from root)
            else if(!source_hierarchy_value.isEmpty() && is_source_nested_value.toLongLong() == 1)
            {
                //Use the splitter(>>) static here because in future it will not effect the load case
                if(source_hierarchy_value.startsWith(">>"))
                {
                    source_hierarchy_value = source_hierarchy_value.prepend(root_cnt_name);
                    QString update_command = "UPDATE '" + table_name +"' set source_hierarchy = ?";
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,QStringList(source_hierarchy_value),destination_db_file,Q_FUNC_INFO);
                }
            }

            QString destination_text_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + table_name + ".txt";

            QString source_hierarchy_val ,is_source_nested_val,root_count_name ;
            root_count_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(destination_text_file, "MACRO_NARAD_Root_Count_Name_QString");
            source_hierarchy_val = get_value_of_key_from_source_textinfo_file_from_100_to_148(destination_text_file, "MACRO_NARAD_Source_Hierarchy_QString");
            is_source_nested_val = get_value_of_key_from_source_textinfo_file_from_100_to_148(destination_text_file, "MACRO_NARAD_Source_Nested_bool");

            if(source_hierarchy_val.isEmpty() && is_source_nested_val.toLongLong() == 0)
            {
                source_hierarchy_val = root_count_name;
            }
            else if(!source_hierarchy_val.isEmpty() && is_source_nested_val.toLongLong() == 1)
            {
                //Use the splitter(>>) static here because in future it will not effect the load case
                if(source_hierarchy_val.startsWith(">>"))
                {
                    source_hierarchy_val.prepend(root_count_name);
                }
            }

            QFile txt_file(destination_text_file);
            if(!txt_file.open(QIODevice::ReadWrite))
            {
                recon_static_functions::app_debug("File Open -----FAILED---- " + txt_file.fileName(), Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + txt_file.errorString(), Q_FUNC_INFO);
            }


            QStringList update_file_line_list;

            while(!txt_file.atEnd())
            {
                QString file_line = txt_file.readLine().trimmed();
                if(file_line.contains("MACRO_NARAD_Source_Hierarchy_QString"))
                    continue;

                update_file_line_list << file_line;
            }
            source_hierarchy_val = "MACRO_NARAD_Source_Hierarchy_QString" + QString("=" + source_hierarchy_val + "\n").toLocal8Bit();
            update_file_line_list.append(source_hierarchy_val);

            QString final_data  = update_file_line_list.join("\n");

            txt_file.resize(0);
            txt_file.write(final_data.toLocal8Bit());

            txt_file.flush();
            txt_file.close();
        }
    }
    //==========================================Source_Hierarchy_End=========================//

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
}

void MainWindow::update_source_140_to_142()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //=============================Source_Type_Start==========================//
    QString dest_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
    QStringList source_count_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select table_name from source_index", 0, dest_db_file, Q_FUNC_INFO);
    for(int jj = 0; jj < source_count_table_list.size(); jj++)
    {
        QString table_name = source_count_table_list.at(jj);
        QString select_cmd = "select source_type from " + table_name + "";
        QString source_type = recon_helper_standard_obj->get_string_from_db_by_dbpath(select_cmd,0,dest_db_file,Q_FUNC_INFO);
        QString command = "UPDATE " + table_name + " set source_type = ?";
        if(source_type == "RECON-Sparseimage")
        {
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,QStringList("RECON - Sparseimage"),dest_db_file,Q_FUNC_INFO);
        }
        else if(source_type == "RECON-DMG")
        {
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,QStringList("RECON - DMG"),dest_db_file,Q_FUNC_INFO);
        }
        else if(source_type == "RECON-Folder")
        {
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,QStringList("RECON - Folder"),dest_db_file,Q_FUNC_INFO);
        }

        QString destination_text_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + table_name + ".txt";

        QString src_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(destination_text_file, "MACRO_NARAD_Source_Type_QString");
        if(src_type == "RECON-Sparseimage")
        {
            recon_static_functions::remove_data_contain_line_from_file(destination_text_file,"MACRO_NARAD_Source_Type_QString=",Q_FUNC_INFO);
            QString source_type = QString("MACRO_NARAD_Source_Type_QString=RECON - Sparseimage");
            recon_static_functions::append_data_into_file(destination_text_file,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(src_type == "RECON-DMG")
        {
            recon_static_functions::remove_data_contain_line_from_file(destination_text_file,"MACRO_NARAD_Source_Type_QString=",Q_FUNC_INFO);
            QString source_type = QString("MACRO_NARAD_Source_Type_QString=RECON - DMG");
            recon_static_functions::append_data_into_file(destination_text_file,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(src_type == "RECON-Folder")
        {
            recon_static_functions::remove_data_contain_line_from_file(destination_text_file,"MACRO_NARAD_Source_Type_QString=",Q_FUNC_INFO);
            QString source_type = QString("MACRO_NARAD_Source_Type_QString=RECON - Folder");
            recon_static_functions::append_data_into_file(destination_text_file,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }
    
    
    QString root_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";
    QStringList root_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select root_count_name from tbl_root_index", 0, root_db_path, Q_FUNC_INFO);
    for(int ii = 0; ii < root_cnt_name_list.size(); ii++)
    {
        QStringList args_list;
        QString slect_cmd = "select source_type from tbl_root_index where root_count_name = ? ";
        QString srce_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(slect_cmd,QStringList(root_cnt_name_list.at(ii)),0,root_db_path,Q_FUNC_INFO);
        QString command = "UPDATE tbl_root_index set source_type = ? where root_count_name = ?";
        if(srce_type == "RECON-Sparseimage")
        {
            args_list.clear();
            args_list << "RECON - Sparseimage" << root_cnt_name_list.at(ii);
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,args_list,root_db_path,Q_FUNC_INFO);
        }
        else if(srce_type == "RECON-DMG")
        {
            args_list.clear();
            args_list << "RECON - DMG" << root_cnt_name_list.at(ii);
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,args_list,root_db_path,Q_FUNC_INFO);
        }
        else if(srce_type == "RECON-Folder")
        {
            args_list.clear();
            args_list << "RECON - Folder" << root_cnt_name_list.at(ii);
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,args_list,root_db_path,Q_FUNC_INFO);
        }
    }

    QString roots_index_file_path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_index.txt";
    QFile root_index_file_1(roots_index_file_path_1);
    if(!root_index_file_1.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("  ---FAILED--- to open" + root_index_file_1.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("  error -  " + root_index_file_1.errorString(),Q_FUNC_INFO);
    }
    QStringList root_count_name_list_1;
    while(!root_index_file_1.atEnd())
    {
        root_count_name_list_1 << QString::fromLocal8Bit(root_index_file_1.readLine()).trimmed();
    }
    for(int hh = 0; hh < root_count_name_list_1.size(); hh++)
    {
        QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + root_count_name_list_1.at(hh);
        QString source_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Source_Type_QString");
        if(source_type == "RECON-Sparseimage")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_file_path,"MACRO_NARAD_Root_Source_Type_QString=",Q_FUNC_INFO);
            QString source_type = QString("MACRO_NARAD_Root_Source_Type_QString=RECON - Sparseimage");
            recon_static_functions::append_data_into_file(root_file_path,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(source_type == "RECON-DMG")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_file_path,"MACRO_NARAD_Root_Source_Type_QString=",Q_FUNC_INFO);
            QString source_type = QString("MACRO_NARAD_Root_Source_Type_QString=RECON - DMG");
            recon_static_functions::append_data_into_file(root_file_path,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(source_type == "RECON-Folder")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_file_path,"MACRO_NARAD_Root_Source_Type_QString=",Q_FUNC_INFO);
            QString source_type = QString("MACRO_NARAD_Root_Source_Type_QString=RECON - Folder");
            recon_static_functions::append_data_into_file(root_file_path,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //=============================Source_Type_End==========================//


    //===================Update source Index db Start===========================//

    QString rename_cmd = "ALTER TABLE source_index RENAME COLUMN os_scheme to os_scheme_display";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,dest_db_file,Q_FUNC_INFO);

    QString add_new_column_command = "ALTER TABLE source_index add column os_scheme_internal VARCHAR(100)";
    recon_helper_standard_obj->execute_db_command_by_dbpath(add_new_column_command,dest_db_file,Q_FUNC_INFO);

    QStringList source_cnt_table_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select table_name from source_index", 0, dest_db_file, Q_FUNC_INFO);
    for(int jj  = 0; jj < source_cnt_table_list.size(); jj++)
    {
        QString select_cmd = "select os_scheme_display from source_index where table_name = ?";
        QString os_schme = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(source_cnt_table_list.at(jj)),0,dest_db_file,Q_FUNC_INFO);
        if(os_schme == "iOS")
        {
            os_schme = "iOS_iTunes";
        }

        QString update_command = "UPDATE source_index set os_scheme_internal = ? where table_name = ?";
        QStringList argumnt_list;
        argumnt_list << os_schme << source_cnt_table_list.at(jj);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,argumnt_list,dest_db_file,Q_FUNC_INFO);
    }
    //===================Update source Index db End===========================//


    //===================Update source info db Start===========================//

    for(int mm = 0; mm < source_cnt_table_list.size(); mm++)
    {
        QString table_name = source_cnt_table_list.at(mm);
        QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN os_scheme to os_scheme_display";
        recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,dest_db_file,Q_FUNC_INFO);

        QString add_new_column_command = "ALTER TABLE "+ table_name + " add column os_scheme_internal VARCHAR(100)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(add_new_column_command,dest_db_file,Q_FUNC_INFO);

        QString select_cmd = "select os_scheme_display from " + table_name;
        QString os_schem = recon_helper_standard_obj->get_string_from_db_by_dbpath(select_cmd,0,dest_db_file,Q_FUNC_INFO);
        if(os_schem == "iOS")
        {
            os_schem = "iOS_iTunes";
        }
        QString update_command = "UPDATE '" + table_name +"' set os_scheme_internal = ?";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,QStringList(os_schem),dest_db_file,Q_FUNC_INFO);
    }
    //===================Update source info db End===========================//

    //===================Update source Info txt start===========================//

    QString conf_source_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString source_indx_file_path = conf_source_dir + "sources_index.txt";

    QFile file(source_indx_file_path);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QStringList source_file_name_list;
            while(!file.atEnd())
            {
                source_file_name_list << QString::fromLocal8Bit(file.readLine()).trimmed();
            }

            file.close();

            for(int pp = 0; pp < source_file_name_list.size(); pp++)
            {
                QString source_file_path = conf_source_dir + source_file_name_list.at(pp);

                QString recived_os_schme = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path, "MACRO_NARAD_Source_OS_Naming_Scheme_QString");
                QString os_schme_dsply = QString("MACRO_NARAD_Source_OS_Scheme_Display_QString") + "=" + recived_os_schme;

                if(recived_os_schme == "iOS")
                {
                    recived_os_schme = "iOS_iTunes";
                }

                QString os_schme_intrnl = QString("MACRO_NARAD_Source_OS_Scheme_Internal_QString") + "=" + recived_os_schme;

                recon_static_functions::append_data_into_file(source_file_path,os_schme_dsply,enum_global_append_data_in_file_with_append_one_next_line, Q_FUNC_INFO);
                recon_static_functions::append_data_into_file(source_file_path,os_schme_intrnl,enum_global_append_data_in_file_with_append_one_next_line, Q_FUNC_INFO);

                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_OS_Naming_Scheme_QString="),Q_FUNC_INFO);
                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Root_Source_Type_QString="),Q_FUNC_INFO);
                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Root_Source_Name_QString="),Q_FUNC_INFO);
                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Root_Source_Path_QString="),Q_FUNC_INFO);
                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Root_Disk_Identifier_QString="),Q_FUNC_INFO);
                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Volume_Parent_Identifier_QString="),Q_FUNC_INFO);
                //recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Root_Count_Name_QString="),Q_FUNC_INFO);
                recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Password_QString="),Q_FUNC_INFO);
            }

        }
    }
    //===================Update source Info txt End===========================//


    //-----------------------Update_Root_info Start------------------------------//
    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString root_info_db_path = conf_source + "roots_info.sqlite";

    QStringList root_cnt_name_list_3 = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select root_count_name from tbl_root_index", 0, root_db_path, Q_FUNC_INFO);
    for(int jj = 0 ; jj < root_cnt_name_list_3.size(); jj++)
    {
        QString selc_cmd = "select source_type from tbl_root_index where root_count_name = ?";
        QString source_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(selc_cmd,QStringList(root_cnt_name_list_3.at(jj)),0,root_info_db_path,Q_FUNC_INFO);
        if(source_type =="iOS Backup")
        {
            QString update_command = "Update tbl_root_index set source_type = ? where root_count_name = ?";
            QStringList args_lst;
            args_lst << "iTunes Backup" << root_cnt_name_list_3.at(jj);
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,args_lst,root_info_db_path,Q_FUNC_INFO);
        }
    }
    QString root_indx_file_path_2 = conf_source + "roots_index.txt";
    QFile root_index_file_2(root_indx_file_path_2);
    if(!root_index_file_2.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("  ---FAILED--- to open" + root_index_file_2.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("  error -  " + root_index_file_2.errorString(),Q_FUNC_INFO);
    }
    QStringList root_count_name_list_4;
    while(!root_index_file_2.atEnd())
    {
        root_count_name_list_4 << QString::fromLocal8Bit(root_index_file_2.readLine()).trimmed();
    }
    for(int hh = 0; hh < root_count_name_list_4.size(); hh++)
    {
        QString root_file_path = conf_source + root_count_name_list_4.at(hh);
        QString source_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Source_Type_QString");
        if(source_type == "iOS Backup")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_file_path,QString("MACRO_NARAD_Root_Source_Type_QString="),Q_FUNC_INFO);
            QString source_type =  QString("MACRO_NARAD_Root_Source_Type_QString=iTunes Backup");
            recon_static_functions::append_data_into_file(root_file_path,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }
    //-----------------------Update_Root_info End------------------------------//



    //==================Update Source Type in Db Start=============================//
    QStringList sourc_cnt_names_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select table_name from source_index", 0, dest_db_file, Q_FUNC_INFO);
    for(int mm = 0; mm < sourc_cnt_names_list.size(); mm++)
    {
        QString table_name = sourc_cnt_names_list.at(mm);
        QString source_type = recon_helper_standard_obj->get_string_from_db_by_dbpath("select source_type from " + table_name + "",0,dest_db_file,Q_FUNC_INFO);
        if(source_type == "iOS Backup")
        {
            QString update_command = "Update "+ table_name +" set source_type = ? ";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,QStringList("iTunes Backup"),dest_db_file,Q_FUNC_INFO);
        }
    }

    //==================Update Source Type DB End=============================//


    //==================Update Source Type in Text Start=============================//

    QFile index_file(source_indx_file_path);
    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            QStringList source_count_file_name_list;
            while(!index_file.atEnd())
            {
                source_count_file_name_list << QString::fromLocal8Bit(index_file.readLine()).trimmed();
            }

            index_file.close();

            for(int pp = 0; pp < source_count_file_name_list.size(); pp++)
            {
                QString source_file_path = conf_source_dir + source_count_file_name_list.at(pp);

                QString source_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path,"MACRO_NARAD_Source_Type_QString");
                if(source_type == "iOS Backup")
                {
                    recon_static_functions::remove_data_contain_line_from_file(source_file_path,QString("MACRO_NARAD_Source_Type_QString="),Q_FUNC_INFO);
                    QString source_type =  QString("MACRO_NARAD_Source_Type_QString=iTunes Backup");
                    recon_static_functions::append_data_into_file(source_file_path,source_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
                }
            }

        }
    }
    //==================Update Source Type in Text End=============================//


    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void MainWindow::update_source_142_to_145()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);




    //======================Root Evidence Type Update Start ====================//

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString root_info_db_path = conf_source + "roots_info.sqlite";
    QString command = "PRAGMA TABLE_INFO(tbl_root_index)";
    QStringList column_name_list_from_tbl_root_index = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,root_info_db_path,Q_FUNC_INFO);
    if(!column_name_list_from_tbl_root_index.contains("evidence_type"))
    {
        QString add_clmn_cmd = "ALTER TABLE tbl_root_index add column evidence_type VARCHAR(100)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(add_clmn_cmd,root_info_db_path,Q_FUNC_INFO);
    }

    QStringList root_cnt_name_list_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select root_count_name from tbl_root_index", 0, root_info_db_path, Q_FUNC_INFO);
    for(int jj = 0 ; jj < root_cnt_name_list_from_db.size(); jj++)
    {
        QString root_count_name = root_cnt_name_list_from_db.at(jj);
        QString select_root_type_cmd = "select root_type from tbl_root_index where root_count_name = ?";
        QString root_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_root_type_cmd,QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);
        QString update_cmd = "Update tbl_root_index set evidence_type = ? where root_count_name = ?";
        QStringList args_list;

        if(root_type ==  "Forensics Image" || root_type == "File Vault Image" || root_type ==  "Fusion Image" ||
                root_type == "Optical Disc Image" || root_type == "RAM Image" || root_type == "Mounted Volumes")
        {
            args_list << "Physical Evidence" << root_count_name;
        }
        else if(root_type == "RECON Logical Image" || root_type ==  "macOS Home Directory" || root_type == "Folder" || root_type ==  "File" ||
                root_type == "Time Machine Backup")
        {
            args_list << "Logical Evidence" << root_count_name;
        }
        else if(root_type == "iOS Backup" || root_type ==  "ADB Android Backup")
        {
            args_list << "Mobile Evidence" << root_count_name;
        }
        else if(root_type == "Google Takeout")
        {
            args_list << "Cloud Evidence" << root_count_name;
        }

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,args_list,root_info_db_path,Q_FUNC_INFO);
    }

    QString roots_index_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_index.txt";
    QStringList root_count_name_list_from_txt_file_with_txt;
    QStringList root_count_name_list_from_txt_file_without_txt;
    QFile root_index_file(roots_index_file_path);
    if(root_index_file.exists())
    {
        if(root_index_file.open(QIODevice::ReadOnly))
        {
            while(!root_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(root_index_file.readLine()).trimmed();
                root_count_name_list_from_txt_file_with_txt << line_str;
                line_str.chop(4);      //.txt
                root_count_name_list_from_txt_file_without_txt  << line_str;
            }
            root_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_txt_file_with_txt.size(); hh++)
    {
        QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + root_count_name_list_from_txt_file_with_txt.at(hh);
        QString root_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Type_QString");
        QString evidence_type;
        if(root_type ==  "Forensics Image" || root_type == "File Vault Image" || root_type ==  "Fusion Image" ||
                root_type == "Optical Disc Image" || root_type == "RAM Image" || root_type == "Mounted Volumes")
        {
            evidence_type = QString("MACRO_NARAD_Evidence_Type_QString=Physical Evidence");
            recon_static_functions::append_data_into_file(root_file_path,evidence_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(root_type == "RECON Logical Image" || root_type ==  "macOS Home Directory" || root_type == "Folder" || root_type ==  "File" ||
                root_type == "Time Machine Backup")
        {
            evidence_type = QString("MACRO_NARAD_Evidence_Type_QString=Logical Evidence");
            recon_static_functions::append_data_into_file(root_file_path,evidence_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(root_type == "iOS Backup" || root_type ==  "ADB Android Backup")
        {
            evidence_type = QString("MACRO_NARAD_Evidence_Type_QString=Mobile Evidence");
            recon_static_functions::append_data_into_file(root_file_path,evidence_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
        else if(root_type == "Google Takeout")
        {
            evidence_type = QString("MACRO_NARAD_Evidence_Type_QString=Cloud Evidence");
            recon_static_functions::append_data_into_file(root_file_path,evidence_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //======================Root Evidence Type Update End ====================//




    //==============================Root_Type_Start==========================//
    for(int jj = 0 ; jj < root_cnt_name_list_from_db.size(); jj++)
    {
        QString root_count_name = root_cnt_name_list_from_db.at(jj);
        QString select_root_type_cmd = "select root_type from tbl_root_index where root_count_name = ?";
        QString root_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_root_type_cmd,QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);
        if(root_type == "iOS Backup")
        {
            QStringList args_list;
            QString update_cmd = "Update tbl_root_index set root_type = ? where root_count_name = ?";
            args_list << "iTunes iOS Backup" << root_count_name;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,args_list,root_info_db_path,Q_FUNC_INFO);
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_txt_file_with_txt.size(); hh++)
    {
        QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + root_count_name_list_from_txt_file_with_txt.at(hh);
        QString root_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Type_QString");
        if(root_type == "iOS Backup")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_file_path,"MACRO_NARAD_Root_Type_QString=",Q_FUNC_INFO);
            QString rt_type = QString("MACRO_NARAD_Root_Type_QString=iTunes iOS Backup");
            recon_static_functions::append_data_into_file(root_file_path,rt_type,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //==============================Root_Type_Start==========================//





    //======================================Rooot_Source_Type_Display_Internal_Start==========================//

    if(!column_name_list_from_tbl_root_index.contains("source_type_display"))
    {
        QString cmd_add_column = "ALTER TABLE tbl_root_index ADD COLUMN source_type_display";
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_add_column,root_info_db_path,Q_FUNC_INFO);
    }

    if(column_name_list_from_tbl_root_index.contains("source_type"))
    {
        QString renme_cmd = "ALTER TABLE tbl_root_index RENAME COLUMN source_type to source_type_internal";
        recon_helper_standard_obj->execute_db_command_by_dbpath(renme_cmd,root_info_db_path,Q_FUNC_INFO);
    }

    for(int jj = 0; jj < root_cnt_name_list_from_db.size(); jj++)
    {
        QString select_cmd = "select source_type_internal from tbl_root_index where root_count_name = ?";
        QString recieved_src_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(root_cnt_name_list_from_db.at(jj)),0,root_info_db_path,Q_FUNC_INFO);

        QString src_type_dsply = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,0,Q_FUNC_INFO);
        QString src_type_intrnl = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,1,Q_FUNC_INFO);


        QString update_cmd_2 = "UPDATE tbl_root_index set source_type_display = ?, source_type_internal = ? where root_count_name = ?";
        QStringList args_list_2;
        args_list_2 << src_type_dsply << src_type_intrnl << root_cnt_name_list_from_db.at(jj);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd_2,args_list_2,root_info_db_path,Q_FUNC_INFO);
    }

    for(int ii = 0; ii < root_count_name_list_from_txt_file_with_txt.size(); ii++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_txt_file_with_txt.at(ii);
        QString recieved_src_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path, "MACRO_NARAD_Root_Source_Type_QString");

        QString src_type_dsply = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,0,Q_FUNC_INFO);
        QString src_type_intrnl = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,1,Q_FUNC_INFO);

        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_QString=",Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Display_QString=" + src_type_dsply,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Internal_QString="+ src_type_intrnl,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    }

    //======================================Rooot_Source_Type_Display_Internal_End==========================//






    //======================================Source_Type_Display_Internal_Start==========================//

    QString src_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
    QStringList source_count_nm_list_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select table_name from source_index", 0, src_db_file, Q_FUNC_INFO);
    for(int mm = 0; mm < source_count_nm_list_from_db.size(); mm++)
    {
        QString table_name = source_count_nm_list_from_db.at(mm);
        QString command_pragma = "PRAGMA TABLE_INFO( '" + table_name + "')";
        QStringList column_name_list_from_sources = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_pragma,1,src_db_file,Q_FUNC_INFO);

        if(!column_name_list_from_sources.contains("source_type_display"))
        {
            QString cmd_add_column = "ALTER TABLE " + table_name + " ADD COLUMN source_type_display";
            recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_add_column,src_db_file,Q_FUNC_INFO);
        }

        if(column_name_list_from_sources.contains("source_type"))
        {
            QString renme_cmd = "ALTER TABLE " + table_name + " RENAME COLUMN source_type to source_type_internal";
            recon_helper_standard_obj->execute_db_command_by_dbpath(renme_cmd,src_db_file,Q_FUNC_INFO);
        }

        QString select_cmd = "select source_type_internal from " + table_name + "";
        QString recieved_src_type = recon_helper_standard_obj->get_string_from_db_by_dbpath(select_cmd,0,src_db_file,Q_FUNC_INFO);

        QString src_type_dsply = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,0,Q_FUNC_INFO);
        QString src_type_intrnl = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,1,Q_FUNC_INFO);


        QString update_cmd_1 = "UPDATE " + table_name + " set source_type_display = ?, source_type_internal = ?";
        QStringList args_list_1;
        args_list_1 << src_type_dsply << src_type_intrnl;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd_1,args_list_1,src_db_file,Q_FUNC_INFO);
    }

    QString source_indx_file_path = conf_source + "sources_index.txt";
    QFile index_file(source_indx_file_path);
    QStringList source_count_file_name_list_from_txt_file_with_txt;
    QStringList source_count_file_name_list_from_txt_file_without_txt;

    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            while(!index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(index_file.readLine()).trimmed();
                source_count_file_name_list_from_txt_file_with_txt << line_str;
                line_str.chop(4);      //.txt
                source_count_file_name_list_from_txt_file_without_txt  << line_str;
            }

            index_file.close();
        }
    }

    for(int pp = 0; pp < source_count_file_name_list_from_txt_file_with_txt.size(); pp++)
    {
        QString source_file_path = conf_source + source_count_file_name_list_from_txt_file_with_txt.at(pp);
        QString recieved_src_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_file_path,"MACRO_NARAD_Source_Type_QString");

        QString src_type_dsply = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,0,Q_FUNC_INFO);
        QString src_type_intrnl = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,1,Q_FUNC_INFO);

        recon_static_functions::remove_data_contain_line_from_file(source_file_path,"MACRO_NARAD_Source_Type_QString=",Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_file_path,"MACRO_NARAD_Source_Type_Display_QString=" + src_type_dsply ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_file_path,"MACRO_NARAD_Source_Type_Internal_QString="+src_type_intrnl,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    }

    //======================================Source_Type_Display_Internal_End==========================//






    //======================Encase Logical Root Source Type Start ====================//

    for(int jj = 0 ; jj < root_cnt_name_list_from_db.size(); jj++)
    {
        QString root_count_name = root_cnt_name_list_from_db.at(jj);
        QString evidence_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select evidence_type from tbl_root_index where root_count_name = ?",QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);
        QString root_type = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select root_type from tbl_root_index where root_count_name = ?",QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);
        QString rt_source_type_intrnl = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_type_internal from tbl_root_index where root_count_name = ?",QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);
        QString root_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select root_name from tbl_root_index where root_count_name = ?",QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);


        if(evidence_type == "Physical Evidence" && root_type == "Forensics Image" && rt_source_type_intrnl == "Forensics_Image" && root_name.endsWith(".L01",Qt::CaseInsensitive))
        {
            QString update_cmd = "Update tbl_root_index Set evidence_type = ?, root_type = ?, source_type_display = ?, source_type_internal = ?  where root_count_name = ?";
            QStringList args_list;
            args_list << "Logical Evidence" << "Encase Logical Image" << "L01" << "Encase_Logical_Image_L01" << root_count_name;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,args_list,root_info_db_path,Q_FUNC_INFO);
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_txt_file_with_txt.size(); hh++)
    {
        QString root_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + root_count_name_list_from_txt_file_with_txt.at(hh);

        QString evidence_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Evidence_Type_QString");
        QString root_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Type_QString");
        QString rt_source_type_intrnl = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Source_Type_Internal_QString");
        QString root_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_file_path,"MACRO_NARAD_Root_Name_QString");
        if(evidence_type == "Physical Evidence" && root_type == "Forensics Image" && rt_source_type_intrnl == "Forensics_Image" && root_name.endsWith(".L01",Qt::CaseInsensitive))
        {
            recon_static_functions::remove_data_contain_line_from_file(root_file_path,QString("MACRO_NARAD_Evidence_Type_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_file_path,QString("MACRO_NARAD_Evidence_Type_QString=Logical Evidence"),enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

            recon_static_functions::remove_data_contain_line_from_file(root_file_path,QString("MACRO_NARAD_Root_Type_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_file_path,QString("MACRO_NARAD_Root_Type_QString=Encase Logical Image"),enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

            recon_static_functions::remove_data_contain_line_from_file(root_file_path,QString("MACRO_NARAD_Root_Source_Type_Display_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_file_path,QString("MACRO_NARAD_Root_Source_Type_Display_QString=L01"),enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

            recon_static_functions::remove_data_contain_line_from_file(root_file_path,QString("MACRO_NARAD_Root_Source_Type_Internal_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_file_path,QString("MACRO_NARAD_Root_Source_Type_Internal_QString=Encase_Logical_Image_L01"),enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //======================Encase Logical Root Source Type End ====================//


    //======================Encase Logical Source Type Start ====================//

    for(int jj = 0; jj < source_count_nm_list_from_db.size(); jj++)
    {
        QString table_name = source_count_nm_list_from_db.at(jj);
        QString source_type_intrnl = recon_helper_standard_obj->get_string_from_db_by_dbpath("select source_type_internal from " + table_name + "",0,src_db_file,Q_FUNC_INFO);
        QString source_main_name = recon_helper_standard_obj->get_string_from_db_by_dbpath("select source_main_name from " + table_name + "",0,src_db_file,Q_FUNC_INFO);
        if(source_type_intrnl == "Forensics_Image" && source_main_name.endsWith(".L01",Qt::CaseInsensitive))
        {
            QString update_cmd = "Update " + table_name + " SET source_type_display = ?, source_type_internal = ? where source_main_name = ?";
            QStringList args_list;
            args_list << "L01" << "Encase_Logical_Image_L01" << source_main_name;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,args_list,src_db_file,Q_FUNC_INFO);
        }
    }


    for(int pp = 0; pp < source_count_file_name_list_from_txt_file_with_txt.size(); pp++)
    {
        QString source_txt_file_path = conf_source + source_count_file_name_list_from_txt_file_with_txt.at(pp);

        QString source_type_intrnl = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path,"MACRO_NARAD_Source_Type_Internal_QString");
        QString source_main_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path,"MACRO_NARAD_Source_Main_Name_QString");

        if(source_type_intrnl == "Forensics_Image" && source_main_name.endsWith(".L01",Qt::CaseInsensitive))
        {
            recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Type_Display_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(source_txt_file_path,QString("MACRO_NARAD_Source_Type_Display_QString=L01"),enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

            recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Type_Internal_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(source_txt_file_path,QString("MACRO_NARAD_Source_Type_Internal_QString=Encase_Logical_Image_L01"),enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //======================Encase Logical Source Type End ====================//




    //===================================Rename Source Db Column Start====================//

    for(int jj = 0; jj < source_count_nm_list_from_db.size(); jj++)
    {
        QString table_name = source_count_nm_list_from_db.at(jj);

        QString command = "PRAGMA TABLE_INFO( '" + table_name + "')";

        QStringList column_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,src_db_file,Q_FUNC_INFO);
        for(int hh = 0; hh < column_name_list.size(); hh++)
        {
            QString column_name = column_name_list.at(hh);
            if(column_name == "root_source_type")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN root_source_type to extra_not_in_use_0_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
            else if(column_name == "root_source_name")
            {
                QString  rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN root_source_name to extra_not_in_use_1_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
            else if(column_name == "root_source_path")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN root_source_path to extra_not_in_use_2_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
            else if(column_name == "root_disk_identifier")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN root_disk_identifier to extra_not_in_use_3_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
            else if(column_name == "evidence_by_examiner")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN evidence_by_examiner to extra_not_in_use_4_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
            else if(column_name == "description_by_examiner")
            {
                QString  rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN description_by_examiner to extra_not_in_use_5_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
            else if(column_name == "added_in_case_timestamp_utc")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN added_in_case_timestamp_utc to extra_not_in_use_6_145";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,src_db_file,Q_FUNC_INFO);
            }
        }
    }

    for(int pp = 0; pp < source_count_file_name_list_from_txt_file_with_txt.size(); pp++)
    {
        QString source_txt_file_path = conf_source + source_count_file_name_list_from_txt_file_with_txt.at(pp);

        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Evidence_By_Examiner_QString="),Q_FUNC_INFO);
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Description_By_Examiner_QString="),Q_FUNC_INFO);
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Added_In_Case_Timestamp_UTC_qint64="),Q_FUNC_INFO);
    }

    //======================Rename Source Db Column End====================//




    //===================================Examiner_Number_Column_Name_Rename_Start=======================//

    QString root_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "roots_info.sqlite";
    if(column_name_list_from_tbl_root_index.contains("evidence_by_examiner"))
    {
        QString rename_cmd = "ALTER TABLE tbl_root_index RENAME COLUMN evidence_by_examiner to evidence_number_by_examiner";
        recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,root_db_path,Q_FUNC_INFO);
    }

    for(int jj = 0; jj < root_count_name_list_from_txt_file_with_txt.size(); jj++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_txt_file_with_txt.at(jj);
        QString evidence_no = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path, "MACRO_NARAD_Root_Evidence_By_Examiner_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,"MACRO_NARAD_Root_Evidence_By_Examiner_QString=",Q_FUNC_INFO);
        QString evd_numbr_str = QString("MACRO_NARAD_Root_Evidence_Number_By_Examiner_QString=") + evidence_no;
        recon_static_functions::append_data_into_file(root_txt_file_path,evd_numbr_str,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
    }

    //===================================Examiner_Number_Column_Name_Rename_End=======================//


    //===================================Max_Root_Count_Index_Start====================================//

    QString root_count_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.sqlite";
    QStringList root_cnt_name_list_1;
    root_cnt_name_list_1 << root_cnt_name_list_from_db << root_count_name_list_from_txt_file_without_txt;
    root_cnt_name_list_1.removeDuplicates();

    for(int jj = 0; jj < root_cnt_name_list_1.size(); jj++)
    {
        QString command_insert_root_index = "INSERT INTO 'z_count'(z_root_count_flag,z_root_count) values(?,?)";
        QString root_count_name_1 = root_cnt_name_list_1.at(jj);
        QString root_count_int = root_count_name_1.remove(0,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Root_Flag).size()).trimmed();
        QStringList values_list;
        values_list << root_cnt_name_list_1.at(jj) << root_count_int;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_root_index,values_list,root_count_db_path,Q_FUNC_INFO);

        QString destination_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.txt";
        QFile file(destination_txt_file);
        if(file.open(QIODevice::Append))
        {
            file.write(root_cnt_name_list_1.at(jj).toLocal8Bit() + "\n");

            file.flush();
            file.close();
        }
    }
    //===================================Max_Root_Count_Index_End====================================//


    //===================================Max_Source_Count_Index_Start====================================//

    QString source_count_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.sqlite";
    QStringList source_cnt_name_list_1;
    source_cnt_name_list_1 << source_count_nm_list_from_db << source_count_file_name_list_from_txt_file_without_txt;
    source_cnt_name_list_1.removeDuplicates();

    for(int jj = 0; jj < source_cnt_name_list_1.size(); jj++)
    {
        QString command_insert_root_index = "INSERT INTO 'z_count'(z_source_count_flag,z_source_count) values(?,?)";
        QString src_count_name_1 = source_cnt_name_list_1.at(jj);
        QString src_count_int = src_count_name_1.remove(0,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).size()).trimmed();
        QStringList values_list;
        values_list << source_cnt_name_list_1.at(jj) << src_count_int;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_root_index,values_list,source_count_db_path,Q_FUNC_INFO);

        QString destination_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.txt";
        QFile file(destination_txt_file);
        if(file.open(QIODevice::Append))
        {
            file.write(source_cnt_name_list_1.at(jj).toLocal8Bit() + "\n");

            file.flush();
            file.close();
        }
    }
    //===================================Max_Source_Count_Index_End====================================//


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

QString MainWindow::get_exact_source_type_from_src_type_partial_internal_145(QString recieved_src_type, int status, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString final_source_type;
    QString src_type_dsply,src_type_intrnl;

    if(recieved_src_type == "Forensics Image" || recieved_src_type == "Fusion Image" || recieved_src_type == "File Vault Image"
            || recieved_src_type == "Optical Disc Image" || recieved_src_type == "RAM Image" || recieved_src_type == "Mounted Volumes"
            || recieved_src_type == "macOS Home Directory" || recieved_src_type == "Time Machine Backup"
            || recieved_src_type == "Folder" || recieved_src_type == "File" || recieved_src_type == "Google Takeout")
    {
        src_type_dsply = recieved_src_type;
        src_type_intrnl = recieved_src_type;
        src_type_intrnl.replace(" ", "_");
    }
    else if(recieved_src_type == "RECON - Sparseimage")
    {
        src_type_dsply = "Sparseimage";
        src_type_intrnl = "RECON_Logical_Image_Sparseimage";
    }
    else if(recieved_src_type == "RECON - Folder")
    {
        src_type_dsply = "Folder";
        src_type_intrnl = "RECON_Logical_Image_Folder";
    }
    else if(recieved_src_type == "RECON - DMG")
    {
        src_type_dsply = "DMG";
        src_type_intrnl = "RECON_Logical_Image_DMG";
    }
    else if(recieved_src_type == "iTunes Backup")
    {
        src_type_dsply = "iTunes iOS Backup";
        src_type_intrnl = "iTunes_iOS_Backup";
    }
    else if(recieved_src_type == "ADB - Backup Folder")
    {
        src_type_dsply = "Folder";
        src_type_intrnl = "ADB_Android_Backup_Folder";
    }
    else if(recieved_src_type == "ADB - .ab File")
    {
        src_type_dsply = "AB";
        src_type_intrnl = "ADB_Android_Backup_AB";
    }

    if(status == 0)
    {
        final_source_type =  src_type_dsply;
    }
    else if(status == 1)
    {
        final_source_type =  src_type_intrnl;
    }

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " End  ",Q_FUNC_INFO);

    return final_source_type;
}

void MainWindow::update_source_145_to_146()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //======================sources_info.sqlite Update Start ==================//

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString source_info_db_path = conf_source + "sources_info.sqlite";

    QString command_fs_status = "create table fs_status(INT INTEGER PRIMARY KEY,root_count_name varchar(100),source_count_name varchar(100), os_scheme_display varchar(100),os_scheme_internal varchar(100),"
            + QString("Apple Metadata").replace(" ", "_") + " INTEGER, "
            + QString("Exif Metadata").replace(" ", "_")  + " INTEGER, "
            + QString("Signature Analysis").replace(" ", "_") + " INTEGER, "
            + QString("Mime Type").replace(" ", "_") + " INTEGER, "
            + QString("Hashes").replace(" ", "_") + " INTEGER, "
            + QString("Text Indexing").replace(" ", "_") + " INTEGER, "
            + "feature_x1 INTEGER, feature_x2 INTEGER, feature_x3 INTEGER, feature_x4 INTEGER, feature_x5 INTEGER)";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command_fs_status,source_info_db_path,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_info_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_info_db.setDatabaseName(source_info_db_path);
    if(!source_info_db.open())
    {
        recon_static_functions::app_debug( " source_info_db  opening --------FAILED------ " + source_info_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + source_info_db.lastError().text(),Q_FUNC_INFO);
    }

    //fetch data from old table source_index
    QString command_select_old_source_index = "SELECT root_count_name , table_name , os_scheme_display , os_scheme_internal , "
            + QString("Apple Metadata").replace(" ", "_") + " , "
            + QString("Exif Metadata").replace(" ", "_")  + " , "
            + QString("Signature Analysis").replace(" ", "_") + " , "
            + QString("Mime Type").replace(" ", "_") + " , "
            + QString("Hashes").replace(" ", "_") + " , "
            + QString("Text Indexing").replace(" ", "_") + " , "
            + "feature_x1 , feature_x2 , feature_x3 , feature_x4 , feature_x5 FROM source_index";

    QSqlQuery select_query(source_info_db);
    select_query.prepare(command_select_old_source_index);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug( " select_query   --------FAILED------ " + select_query.lastQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
    }

    while(select_query.next())
    {
        QString command_insert_fs_status = "INSERT INTO 'fs_status' (root_count_name, source_count_name, os_scheme_display, os_scheme_internal,"
                + QString("Apple Metadata").replace(" ", "_") + " , "
                + QString("Exif Metadata").replace(" ", "_")  + " , "
                + QString("Signature Analysis").replace(" ", "_") + " , "
                + QString("Mime Type").replace(" ", "_") + " , "
                + QString("Hashes").replace(" ", "_") + " , "
                + QString("Text Indexing").replace(" ", "_") + " , "
                + "feature_x1, feature_x2, feature_x3, feature_x4, feature_x5)"
                + " VALUES(?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?)";

        QStringList values_list;

        values_list << select_query.value("root_count_name").toString();
        values_list << select_query.value("table_name").toString();
        values_list << select_query.value("os_scheme_display").toString();
        values_list << select_query.value("os_scheme_internal").toString();

        values_list << select_query.value(QString("Apple Metadata").replace(" ", "_")).toString();
        values_list << select_query.value(QString("Exif Metadata").replace(" ", "_")).toString();
        values_list << select_query.value(QString("Signature Analysis").replace(" ", "_")).toString();
        values_list << select_query.value(QString("Mime Type").replace(" ", "_")).toString();
        values_list << select_query.value(QString("Hashes").replace(" ", "_")).toString();
        values_list << select_query.value(QString("Text Indexing").replace(" ", "_")).toString();

        values_list << select_query.value("feature_x1").toString();
        values_list << select_query.value("feature_x2").toString();
        values_list << select_query.value("feature_x3").toString();
        values_list << select_query.value("feature_x4").toString();
        values_list << select_query.value("feature_x5").toString();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert_fs_status,values_list,source_info_db,Q_FUNC_INFO);

    }


    QString command_tbl_source_index = "create table tbl_source_index(INT INTEGER PRIMARY KEY,root_count_name varchar(100),source_count_name varchar(100),"
                                       "source_disk_identifier varchar(500), source_hierarchy varchar(500), is_source_nested varchar(10),is_source_decompressed varchar(10),"
                                       "source_type_display varchar(100), source_type_internal varchar(100), os_scheme_display varchar(100),os_scheme_internal varchar(100),volume_offset INTEGER,volume_uuid varchar(100), users_list varchar(100), source_password varchar(100),"
                                       "volume_type varchar(1024), total_size varchar(1024), free_size varchar(1024), used_size varchar(1024), file_system_type varchar(1024),"
                                       "source_main_path varchar(100),source_main_name varchar(1024),source_name varchar(100), source_path_1 varchar(1000), source_path_2_for_fusion varchar(1000), virtual_source_path varchar(100),"
                                       "product_type varchar(100),os_version_extended varchar(100), os_version_full varchar(100), os_version_base varchar(100),lattitude varchar(100),longitude varchar(100),country varchar(100),city varchar(100),"
                                       "configured_id varchar(100), display_name varchar(100),device_name VARCHAR(100),id_no varchar(100),guid varchar(100),product_name varchar(100),target_type varchar(100),build_version varchar(100),"
                                       "itune_version VARCHAR(100),serial_no varchar(100),phone_no varchar(100),imei varchar(100),iccid varchar(100),latest_backup_date_string varchar(100),latest_backup_date_epoch INTEGER,"
                                       "backup_size varchar(100),timezone_string varchar(100),timezone_epoch varchar(100), recon_logical_image varchar(100),"
                                       "installer_timestamp_utc varchar(100))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command_tbl_source_index,source_info_db_path,Q_FUNC_INFO);

    QString cmd_sources = "SELECT source_count_name FROM fs_status";
    QStringList all_sources_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(cmd_sources, 0, source_info_db, Q_FUNC_INFO);

    for(int ii = 0 ; ii < all_sources_list.size() ; ii++)
    {
        QString table_name = all_sources_list.at(ii);

        QString cmd_select_old_source_index = "SELECT root_count_name , source_disk_identifier, source_hierarchy , is_source_nested ,is_source_decompressed ,"
                                              "source_type_display , source_type_internal , os_scheme_display ,os_scheme_internal,volume_offset,volume_uuid , users_list , source_password ,"
                                              "volume_type , total_size , free_size , used_size , file_system_type ,"
                                              "source_main_path ,source_main_name ,source_name , source_path_1 , source_path_2_for_fusion , virtual_source_path,"
                                              "product_type ,os_version_extended , os_version_full , os_version_base ,lattitude ,longitude ,country ,city ,"
                                              "configured_id , display_name ,device_name ,id_no ,guid ,product_name ,target_type ,build_version ,"
                                              "itune_version ,serial_no ,phone_no ,imei ,iccid ,latest_backup_date_string ,latest_backup_date_epoch ,"
                                              "backup_size ,timezone_string ,timezone_epoch , recon_logical_image ,"
                                              "installer_timestamp_utc FROM '" + table_name + "'";

        QSqlQuery select_tbl_name_query = recon_helper_standard_obj->get_executed_sqlquery_from_db_by_dbreference(cmd_select_old_source_index,source_info_db,Q_FUNC_INFO);

        QString cmd_insert_tbl_source_index = "INSERT into tbl_source_index (source_count_name , root_count_name , source_disk_identifier, source_hierarchy , is_source_nested ,is_source_decompressed ,"
                                              "source_type_display , source_type_internal , os_scheme_display ,os_scheme_internal,volume_offset,volume_uuid , users_list , source_password ,"
                                              "volume_type , total_size , free_size , used_size , file_system_type ,"
                                              "source_main_path ,source_main_name ,source_name , source_path_1 , source_path_2_for_fusion , virtual_source_path,"
                                              "product_type ,os_version_extended , os_version_full , os_version_base ,lattitude ,longitude ,country ,city ,"
                                              "configured_id , display_name ,device_name ,id_no ,guid ,product_name ,target_type ,build_version ,"
                                              "itune_version ,serial_no ,phone_no ,imei ,iccid ,latest_backup_date_string ,latest_backup_date_epoch ,"
                                              "backup_size ,timezone_string ,timezone_epoch , recon_logical_image ,installer_timestamp_utc)"
                                              " VALUES(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?)";

        QStringList list_values;
        list_values << table_name;
        list_values << select_tbl_name_query.value("root_count_name").toString();
        list_values << select_tbl_name_query.value("source_disk_identifier").toString();

        list_values << select_tbl_name_query.value("source_hierarchy").toString();
        list_values << select_tbl_name_query.value("is_source_nested").toString();
        list_values << select_tbl_name_query.value("is_source_decompressed").toString();

        list_values << select_tbl_name_query.value("source_type_display").toString();
        list_values << select_tbl_name_query.value("source_type_internal").toString();
        list_values << select_tbl_name_query.value("os_scheme_display").toString();
        list_values << select_tbl_name_query.value("os_scheme_internal").toString();

        list_values << select_tbl_name_query.value("volume_offset").toString();
        list_values << select_tbl_name_query.value("volume_uuid").toString();
        list_values << select_tbl_name_query.value("users_list").toString();
        list_values << select_tbl_name_query.value("source_password").toString();

        list_values << select_tbl_name_query.value("volume_type").toString();
        list_values << select_tbl_name_query.value("total_size").toString();
        list_values << select_tbl_name_query.value("free_size").toString();
        list_values << select_tbl_name_query.value("used_size").toString();
        list_values << select_tbl_name_query.value("file_system_type").toString();

        list_values << select_tbl_name_query.value("source_main_path").toString();
        list_values << select_tbl_name_query.value("source_main_name").toString();
        list_values << select_tbl_name_query.value("source_name").toString();

        list_values << select_tbl_name_query.value("source_path_1").toString();
        list_values << select_tbl_name_query.value("source_path_2_for_fusion").toString();
        list_values << select_tbl_name_query.value("virtual_source_path").toString();

        list_values << select_tbl_name_query.value("product_type").toString();
        list_values << select_tbl_name_query.value("os_version_extended").toString();
        list_values << select_tbl_name_query.value("os_version_full").toString();
        list_values << select_tbl_name_query.value("os_version_base").toString();

        list_values << select_tbl_name_query.value("lattitude").toString();
        list_values << select_tbl_name_query.value("longitude").toString();
        list_values << select_tbl_name_query.value("country").toString();
        list_values << select_tbl_name_query.value("city").toString();


        list_values << select_tbl_name_query.value("configured_id").toString();
        list_values << select_tbl_name_query.value("display_name").toString();
        list_values << select_tbl_name_query.value("device_name").toString();
        list_values << select_tbl_name_query.value("id_no").toString();

        list_values << select_tbl_name_query.value("guid").toString();
        list_values << select_tbl_name_query.value("product_name").toString();
        list_values << select_tbl_name_query.value("target_type").toString();
        list_values << select_tbl_name_query.value("build_version").toString();

        list_values << select_tbl_name_query.value("itune_version").toString();
        list_values << select_tbl_name_query.value("serial_no").toString();
        list_values << select_tbl_name_query.value("phone_no").toString();
        list_values << select_tbl_name_query.value("imei").toString();

        list_values << select_tbl_name_query.value("iccid").toString();
        list_values << select_tbl_name_query.value("latest_backup_date_string").toString();
        list_values << select_tbl_name_query.value("latest_backup_date_epoch").toString();

        list_values << select_tbl_name_query.value("backup_size").toString();
        list_values << select_tbl_name_query.value("timezone_string").toString();
        list_values << select_tbl_name_query.value("timezone_epoch").toString();
        list_values << select_tbl_name_query.value("recon_logical_image").toString();
        list_values << select_tbl_name_query.value("installer_timestamp_utc").toString();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_insert_tbl_source_index,list_values,source_info_db,Q_FUNC_INFO);

    }

    for(int ii = 0 ; ii < all_sources_list.size() ; ii++)
    {
        QString table_name = all_sources_list.at(ii);
        QString drop_source_table_cmd = "DROP TABLE " + table_name ;
        recon_helper_standard_obj->execute_db_command_by_dbreference(drop_source_table_cmd,source_info_db,Q_FUNC_INFO);
    }

    QString drop_source_index_table = "DROP TABLE source_index" ;
    recon_helper_standard_obj->execute_db_command_by_dbreference(drop_source_index_table,source_info_db,Q_FUNC_INFO);
    //======================sources_info.sqlite Update End ====================//


    //======================Source txt file Insert source_count_name Start====================//
    QString source_indx_file_path = conf_source + "sources_index.txt";
    QStringList source_count_file_name_list_from_txt_file_with_txt;

    QFile index_file(source_indx_file_path);
    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            while(!index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(index_file.readLine()).trimmed();
                source_count_file_name_list_from_txt_file_with_txt << line_str;
            }

            index_file.close();
        }
    }

    for(int pp = 0; pp < source_count_file_name_list_from_txt_file_with_txt.size(); pp++)
    {
        QString source_file_path = conf_source + source_count_file_name_list_from_txt_file_with_txt.at(pp);
        QString source_count_name = source_count_file_name_list_from_txt_file_with_txt.at(pp);
        source_count_name.chop(4);       //.txt

        recon_static_functions::append_data_into_file(source_file_path,"MACRO_NARAD_Source_Count_Name_QString=" + source_count_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
    }

    //======================Source txt file Insert source_count_name End====================//

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void MainWindow::update_source_146_to_147()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();


    //======================roots_info db Update Start ==================//

    QString root_info_db_path = conf_source + "roots_info.sqlite";

    QStringList existing_root_colname_list_to_be_renamed;
    existing_root_colname_list_to_be_renamed   << "source_count_name" << "evidence_number_by_examiner" << "description_by_examiner" ;

    QStringList new_root_colname_list_to_be_renamed;
    new_root_colname_list_to_be_renamed   << "all_source_count_name" << "evidence_number" << "description" ;

    QString root_table_name = QString("tbl_root_index");

    recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList(root_table_name),existing_root_colname_list_to_be_renamed,new_root_colname_list_to_be_renamed,root_info_db_path);

    QStringList root_cnt_name_list_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select root_count_name from tbl_root_index", 0, root_info_db_path, Q_FUNC_INFO);
    for(int jj = 0 ; jj < root_cnt_name_list_from_db.size(); jj++)
    {
        QString root_count_name = root_cnt_name_list_from_db.at(jj);

        QString select_cmd = "select all_source_count_name , prime_source_count_name , nested_source_count_name from tbl_root_index where root_count_name = ?";

        QString all_source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);
        QString prime_source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(root_count_name),1,root_info_db_path,Q_FUNC_INFO);
        QString nested_source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(root_count_name),2,root_info_db_path,Q_FUNC_INFO);

        QStringList all_src_cnt_list = all_source_count_name.split("::");
        QStringList prime_src_cnt_list = prime_source_count_name.split("::");
        QStringList nested_src_cnt_list = nested_source_count_name.split("::");

        all_source_count_name = all_src_cnt_list.join(",");
        prime_source_count_name = prime_src_cnt_list.join(",");
        nested_source_count_name = nested_src_cnt_list.join(",");

        QString update_cmd = "Update tbl_root_index SET all_source_count_name = ? , prime_source_count_name = ? , nested_source_count_name = ?  where root_count_name = ?";
        QStringList args_list;
        args_list << all_source_count_name << prime_source_count_name << nested_source_count_name << root_count_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,args_list,root_info_db_path,Q_FUNC_INFO);
    }

    //======================roots_info db Update End ==================//


    //======================roots_info text Update Start ==================//

    QString rt_index_file_path = conf_source + "roots_index.txt";
    QStringList root_count_name_list_from_txt_file_with_txt;
    QFile rt_index_file(rt_index_file_path);
    if(rt_index_file.exists())
    {
        if(rt_index_file.open(QIODevice::ReadOnly))
        {
            while(!rt_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(rt_index_file.readLine()).trimmed();
                root_count_name_list_from_txt_file_with_txt << line_str;
            }

            rt_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_txt_file_with_txt.size(); hh++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_txt_file_with_txt.at(hh);

        QString all_source_count_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_QString");
        QStringList all_src_cnt_list = all_source_count_name.split("::");
        all_source_count_name = all_src_cnt_list.join(",");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Count_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_All_QStringList=" + all_source_count_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString prime_source_count_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Prime_QString");
        QStringList prime_src_cnt_list = prime_source_count_name.split("::");
        prime_source_count_name = prime_src_cnt_list.join(",");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Count_Name_Prime_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList=" + prime_source_count_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString nested_source_count_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Nested_QString");
        QStringList nested_src_cnt_list = nested_source_count_name.split("::");
        nested_source_count_name = nested_src_cnt_list.join(",");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Count_Name_Nested_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList=" + nested_source_count_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


        QString evidence_number = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Evidence_Number_By_Examiner_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Evidence_Number_By_Examiner_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Evidence_Number_QString=" + evidence_number ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString description = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Description_By_Examiner_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Description_By_Examiner_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Description_QString=" + description ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


    }

    //======================roots_info text Update End ====================//

    //======================sources_info db Update Start ==================//
    QString source_info_db_path = conf_source + "sources_info.sqlite";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(source_info_db_path);
    if(source_db.open())
    {
        QString select_cmd = QString("UPDATE fs_status SET os_scheme_internal='iOS_UID' where os_scheme_internal='iOS_iTunes'");
        recon_helper_standard_obj->execute_db_command_by_dbreference(select_cmd,source_db,Q_FUNC_INFO);

        select_cmd.clear();
        select_cmd = QString("UPDATE tbl_source_index SET os_scheme_internal='iOS_UID' where os_scheme_internal='iOS_iTunes'");
        recon_helper_standard_obj->execute_db_command_by_dbreference(select_cmd,source_db,Q_FUNC_INFO);

        QStringList existing_colname_list_to_be_renamed;
        existing_colname_list_to_be_renamed   << "phone_no" << "iccid" << "latest_backup_date_string" << "latest_backup_date_epoch"
                                              << "timezone_string" << "timezone_epoch" << "backup_size" << "total_size"
                                              << "free_size" << "used_size" << "installer_timestamp_utc"
                                              << "source_password" << "os_version_base";


        QStringList new_colname_list_to_be_renamed;
        new_colname_list_to_be_renamed   << "phone_no_msisdn" << "sim_no_iccid" << "latest_backup_date_readable" << "latest_backup_date_numeric"
                                         << "timezone_readable" << "timezone_numeric" << "backup_size_byte_numeric" << "total_size_byte_numeric"
                                         << "free_size_byte_numeric" << "used_size_byte_numeric" << "installer_timestamp_utc_numeric"
                                         << "extra_not_in_use_0_147" << "extra_not_in_use_1_147" ;


        QStringList table_list;
        table_list << "tbl_source_index";

        recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbreference(table_list,existing_colname_list_to_be_renamed,new_colname_list_to_be_renamed,source_db);


        QStringList new_colname_list_to_be_added;
        new_colname_list_to_be_added  << "product_model" << "bluetooth_device_address" << "wifi_address" << "locale_language" ;

        QStringList new_coltype_list_to_be_added;
        new_coltype_list_to_be_added  << "varchar(100)" << "varchar(100)" << "varchar(100)"  << "varchar(100)";

        recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(table_list, new_colname_list_to_be_added, new_coltype_list_to_be_added, source_db);


        QString delete_cmd = QString("DELETE from fs_status where (TRIM(source_count_name) IS NULL OR TRIM(source_count_name)='') ");
        recon_helper_standard_obj->execute_db_command_by_dbreference(delete_cmd , source_db,Q_FUNC_INFO);

        delete_cmd.clear();
        delete_cmd = QString("DELETE from tbl_source_index where (TRIM(source_count_name) IS NULL OR TRIM(source_count_name)='') ");
        recon_helper_standard_obj->execute_db_command_by_dbreference(delete_cmd , source_db,Q_FUNC_INFO);


        source_db.close();

    }


    //======================sources_info db Update End ==================//

    //======================Source txt file update Start====================//
    QString source_indx_file_path = conf_source + "sources_index.txt";
    QStringList source_count_file_name_list_from_txt_file_with_txt;

    QFile index_file(source_indx_file_path);
    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            while(!index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(index_file.readLine()).trimmed();
                source_count_file_name_list_from_txt_file_with_txt << line_str;
            }

            index_file.close();
        }
    }

    for(int pp = 0; pp < source_count_file_name_list_from_txt_file_with_txt.size(); pp++)
    {
        QString source_txt_file_path = conf_source + source_count_file_name_list_from_txt_file_with_txt.at(pp);

        QString os_scheme_internal = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Scheme_Internal_QString");
        if(os_scheme_internal == "iOS_iTunes")
        {
            recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Scheme_Internal_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Scheme_Internal_QString=iOS_UID",enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }


        QString phone_no_msisdn = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Phone_No_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Phone_No_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Phone_No_MSISDN_QStringList=" + phone_no_msisdn ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString imei_no = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Imei_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Imei_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Imei_QStringList=" + imei_no ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString sim_no_iccid = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Iccid_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Iccid_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Sim_No_Iccid_QStringList=" + sim_no_iccid ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString timezone_readable = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Time_Zone_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Time_Zone_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Time_Zone_Readable_QString=" + timezone_readable ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString timezone_numeric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Time_Zone_qint64");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Time_Zone_qint64="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Time_Zone_Numeric_qint64=" + timezone_numeric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString last_backup_date_readable = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Latest_Backup_Date_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Latest_Backup_Date_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString=" + last_backup_date_readable ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString last_backup_date_numeric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Latest_Backup_Date_qint64");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Latest_Backup_Date_qint64="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64=" + last_backup_date_numeric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString os_installer_timestamp_utc_numeric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_qint64");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_qint64="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64=" + os_installer_timestamp_utc_numeric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString backup_size_byte = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Backup_Size_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Backup_Size_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString=" + backup_size_byte ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString total_size_byte = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Total_Size_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Total_Size_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString=" + total_size_byte ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString used_space_byte = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Used_Space_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Used_Space_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString=" + used_space_byte ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString free_space_byte = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Free_Space_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Free_Space_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString=" + free_space_byte ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Version_Base_QString="),Q_FUNC_INFO);
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Password_Plain_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Product_Model_QString=" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Bluetooth_Device_Address_QString=" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Wifi_Address_QString=" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Locale_Language_QString=" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    }

    //======================Source txt file update End====================//

    //======================z_roots_count db update  Start====================//

    QString z_roots_count_db_path = conf_source + "z_roots_count.sqlite";

    QStringList new_colname_list_to_be_added;
    new_colname_list_to_be_added  << "z_active_status" ;

    QStringList new_coltype_list_to_be_added;
    new_coltype_list_to_be_added  << "varchar(100)" ;

    recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbpath(QStringList("z_count"), new_colname_list_to_be_added, new_coltype_list_to_be_added, z_roots_count_db_path);


    QString roots_info_db_path = conf_source + "roots_info.sqlite";
    QString root_select_cmd = QString("SELECT root_count_name from tbl_root_index");
    QStringList root_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(root_select_cmd, 0 ,roots_info_db_path , Q_FUNC_INFO);


    QString root_indx_file_path = conf_source + "roots_index.txt";

    QFile root_index_file(root_indx_file_path);
    if(root_index_file.exists())
    {
        if(root_index_file.open(QIODevice::ReadOnly))
        {
            while(!root_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(root_index_file.readLine()).trimmed();
                root_cnt_name_list << line_str.remove(".txt");
            }

            root_index_file.close();
        }
    }

    root_cnt_name_list.removeDuplicates();

    for(int pp = 0 ; pp < root_cnt_name_list.size() ; pp++)
    {
        QString root_update_cmd = QString("Update z_count SET z_active_status='TRUE' where z_root_count_flag=?");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(root_update_cmd ,QStringList(root_cnt_name_list.at(pp)), z_roots_count_db_path ,Q_FUNC_INFO);
    }

    QString root_update_cmd = QString("Update z_count SET z_active_status='FALSE' where  (TRIM(z_active_status) IS NULL OR TRIM(z_active_status)='')");
    recon_helper_standard_obj->execute_db_command_by_dbpath(root_update_cmd , z_roots_count_db_path ,Q_FUNC_INFO);

    //======================z_roots_count db update  End====================//



    //======================z_sources_count db update  Start====================//

    QString z_sources_count_db_path = conf_source + "z_sources_count.sqlite";
    new_colname_list_to_be_added.clear();
    new_colname_list_to_be_added  << "z_active_status" ;

    new_coltype_list_to_be_added.clear();
    new_coltype_list_to_be_added  << "varchar(100)" ;
    recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbpath(QStringList("z_count"), new_colname_list_to_be_added, new_coltype_list_to_be_added, z_sources_count_db_path);


    QString source_select_cmd = QString("SELECT source_count_name from tbl_source_index");
    QStringList src_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(source_select_cmd, 0 ,source_info_db_path , Q_FUNC_INFO);

    for(int pp = 0 ; pp < source_count_file_name_list_from_txt_file_with_txt.size() ; pp++)
    {

        QString src_cnt_name = source_count_file_name_list_from_txt_file_with_txt.at(pp);
        src_cnt_name = src_cnt_name.remove(".txt").trimmed();
        src_cnt_name_list << src_cnt_name;

    }

    src_cnt_name_list.removeDuplicates();


    for(int qq = 0 ; qq < src_cnt_name_list.size() ; qq++)
    {
        QString source_update_cmd = QString("Update z_count SET z_active_status='TRUE' where z_source_count_flag=?");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(source_update_cmd ,QStringList(src_cnt_name_list.at(qq)), z_sources_count_db_path ,Q_FUNC_INFO);
    }

    QString source_update_cmd = QString("Update z_count SET z_active_status='FALSE' where  (TRIM(z_active_status) IS NULL OR TRIM(z_active_status)='')");
    recon_helper_standard_obj->execute_db_command_by_dbpath(source_update_cmd , z_sources_count_db_path ,Q_FUNC_INFO);

    //======================z_sources_count.sqlite update  End====================//


    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

}


void MainWindow::update_source_147_to_148()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();


    //======================roots_info db Update Start ==================//

    QString root_info_db_path = conf_source + "roots_info.sqlite";

    QStringList root_cnt_name_list_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("Select root_count_name from tbl_root_index", 0, root_info_db_path, Q_FUNC_INFO);
    for(int jj = 0 ; jj < root_cnt_name_list_from_db.size(); jj++)
    {
        QString root_count_name = root_cnt_name_list_from_db.at(jj);

        QString select_cmd = "select root_password from tbl_root_index where root_count_name = ?";

        QString root_password = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(root_count_name),0,root_info_db_path,Q_FUNC_INFO);

        if(root_password.isEmpty())
        {
            continue;
        }
        else
        {
            root_password = recon_static_functions::encrypt_string(root_password , Q_FUNC_INFO);
        }

        QString update_cmd = "Update tbl_root_index SET root_password = ? where root_count_name = ?";
        QStringList args_list;
        args_list << root_password << root_count_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,args_list,root_info_db_path,Q_FUNC_INFO);
    }

    //======================roots_info db Update End ==================//


    //======================roots_info text Update Start ==================//

    QString rt_index_file_path = conf_source + "roots_index.txt";
    QStringList root_count_name_list_from_txt_file_with_txt;
    QFile rt_index_file(rt_index_file_path);
    if(rt_index_file.exists())
    {
        if(rt_index_file.open(QIODevice::ReadOnly))
        {
            while(!rt_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(rt_index_file.readLine()).trimmed();
                root_count_name_list_from_txt_file_with_txt << line_str;
            }

            rt_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_txt_file_with_txt.size(); hh++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_txt_file_with_txt.at(hh);

        QString root_password = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Password_Encrypted_QString");

        if(root_password.isEmpty())
        {
            root_password = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Password_Plain_QString");
        }

        if(!root_password.isEmpty())
        {
            root_password = recon_static_functions::encrypt_string(root_password , Q_FUNC_INFO);

        }

        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Password_Plain_QString="),Q_FUNC_INFO);
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Password_Encrypted_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Password_Encrypted_QString=" + root_password ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    }

    //======================roots_info text Update End ====================//

    //======================sources_info db Update Start ==================//

    QString source_info_db_path = conf_source + "sources_info.sqlite";
    QString select_src_cnt_cmd = QString("SELECT source_count_name from tbl_source_index");

    QStringList src_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_src_cnt_cmd ,0,source_info_db_path,Q_FUNC_INFO);


    for(int pp = 0 ; pp < src_cnt_name_list.size(); pp++)
    {
        QString source_cnt_name = src_cnt_name_list.at(pp);

        QString select_cmd = QString("SELECT users_list , timezone_readable from tbl_source_index WHERE source_count_name = ?");

        QStringList values;
        values << source_cnt_name;

        QString users_str_list = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, source_info_db_path,Q_FUNC_INFO);
        QStringList tmp_usr_list = users_str_list.split(";++::++;");

        QStringList systm_acc_List;

        for(int zz = 0; zz < tmp_usr_list.size(); zz++)
        {
            QString system_user_str = tmp_usr_list.at(zz).trimmed();

            if(!system_user_str.trimmed().isEmpty())
            {
                QStringList users_list;
                users_list << QString("SYSTEM_USER=") + "'" + tmp_usr_list.at(zz) + "'" << QString("DOMAIN_USER=") + "'" + "'";
                QString users_str = users_list.join(";");
                systm_acc_List << users_str;
            }

        }

        users_str_list = systm_acc_List.join("::");

        QString timezone_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , values, 1, source_info_db_path,Q_FUNC_INFO);
        if(timezone_str.trimmed() == "Not Found")
        {
            timezone_str.clear();
        }

        QString update_cmd = QString("UPDATE tbl_source_index SET users_list = ? , timezone_readable = ? WHERE source_count_name = ?");

        values.clear();
        values << users_str_list << timezone_str << source_cnt_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,values,source_info_db_path,Q_FUNC_INFO);

    }

    //======================sources_info db Update End ==================//


    //======================Source txt file update Start====================//

    QString source_indx_file_path = conf_source + "sources_index.txt";
    QStringList source_count_file_name_list_from_txt_file_with_txt;

    QFile index_file(source_indx_file_path);
    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            while(!index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(index_file.readLine()).trimmed();
                source_count_file_name_list_from_txt_file_with_txt << line_str;
            }

            index_file.close();
        }
    }

    for(int pp = 0; pp < source_count_file_name_list_from_txt_file_with_txt.size(); pp++)
    {
        QString source_txt_file_path = conf_source + source_count_file_name_list_from_txt_file_with_txt.at(pp);

        QString users_str_list = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path,"MACRO_NARAD_Source_Users_QStringList");

        QStringList tmp_usr_list = users_str_list.split(";++::++;");

        QStringList systm_acc_list;

        for(int zz = 0; zz < tmp_usr_list.size(); zz++)
        {
            QString system_user_str = tmp_usr_list.at(zz).trimmed();

            if(!system_user_str.trimmed().isEmpty())
            {
                QStringList users_list;
                users_list << QString("SYSTEM_USER=") + "'" + tmp_usr_list.at(zz) + "'" << QString("DOMAIN_USER=") + "'" + "'";
                QString users_str = users_list.join(";");
                systm_acc_list << users_str;
            }

        }

        users_str_list = systm_acc_list.join("::");

        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Users_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_System_Account_QList=" + users_str_list ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


        QString timezone_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path,"MACRO_NARAD_Source_Time_Zone_Readable_QString");
        if(timezone_str.trimmed() == "Not Found")
        {
            timezone_str.clear();
            recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Time_Zone_Readable_QString="),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Time_Zone_Readable_QString=" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }

    }

    //======================Source txt file update End====================//


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_source_148_to_149()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();

    //======================sources_info db Update Start ==================//

    QString source_info_db_path = conf_source + "sources_info.sqlite";

    QString new_tablename = "tbl_source_index";

    QStringList new_columns_name_list;
    new_columns_name_list << "role_type";

    QStringList columns_field_type_list;
    columns_field_type_list << "varchar(100)";

    recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbpath(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, source_info_db_path);

    //======================sources_info db Update End ==================//

    //======================roots_info text Update Start ==================//

    QString rt_index_file_path = conf_source + "roots_index.txt";
    QStringList root_count_name_list_from_text_file_with_txt;
    QFile rt_index_file(rt_index_file_path);
    if(rt_index_file.exists())
    {
        if(rt_index_file.open(QIODevice::ReadOnly))
        {
            while(!rt_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(rt_index_file.readLine()).trimmed();
                root_count_name_list_from_text_file_with_txt << line_str;
            }

            rt_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_text_file_with_txt.size(); hh++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_text_file_with_txt.at(hh);

        QString root_cnt_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Count_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Count_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Count_Name_QString:====:" + root_cnt_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString evidnce_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Evidence_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Evidence_Type_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Evidence_Type_QString:====:" + evidnce_type ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString root_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Type_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Type_QString:====:" + root_type ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString scr_type_disply = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Display_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Type_Display_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Display_QString:====:" + scr_type_disply ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString scr_typ_intrl = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Internal_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Type_Internal_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Internal_QString:====:" + scr_typ_intrl ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString root_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Name_QString:====:" + root_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString root_pth_1 = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Path_1_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Path_1_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Path_1_QString:====:" + root_pth_1 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString root_pth_2 = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Path_2_for_fusion_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Path_2_for_fusion_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Path_2_for_fusion_QString:====:" + root_pth_2 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString root_pswrd_encrptd = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Password_Encrypted_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Password_Encrypted_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Password_Encrypted_QString:====:" + root_pswrd_encrptd ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString home_dir_usr_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Home_Directory_Given_User_Name_QString:====:" + home_dir_usr_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString evidnce_nmbr = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Evidence_Number_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Evidence_Number_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Evidence_Number_QString:====:" + evidnce_nmbr ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString root_descptn = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Description_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Description_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Description_QString:====:" + root_descptn ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString hash_md5_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Hash_MD5_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Hash_MD5_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Hash_MD5_QString:====:" + hash_md5_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString hash_sha_1_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Hash_SHA1_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Hash_SHA1_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Hash_SHA1_QString:====:" + hash_sha_1_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString scr_cnt_nme = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_All_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Count_Name_All_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_All_QStringList:====:" + scr_cnt_nme ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_cnt_nm_prime = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Prime_QStringList:====:" + src_cnt_nm_prime ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_cnt_nm_nstd = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Count_Name_Nested_QStringList:====:" + src_cnt_nm_nstd ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString disk_idnfr_1 = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Disk_Identifier_1_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Disk_Identifier_1_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Disk_Identifier_1_QString:====:" + disk_idnfr_1 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString disk_node_1 = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Disk_Node_1_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Disk_Node_1_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Disk_Node_1_QString:====:" + disk_node_1 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString disk_idnfr_2 = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Disk_Identifier_2_for_fusion_QString:====:" + disk_idnfr_2 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString disk_node_2 = get_value_of_key_from_source_textinfo_file_from_100_to_148(root_txt_file_path,"MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Disk_Node_2_for_fusion_QString:====:" + disk_node_2 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


    }

    //======================roots_info text Update End =====================//

    //======================Source txt file update Start====================//

    QString source_indx_file_path = conf_source + "sources_index.txt";
    QStringList source_count_file_name_list_from_text_file_with_txt;

    QFile index_file(source_indx_file_path);
    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            while(!index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(index_file.readLine()).trimmed();
                source_count_file_name_list_from_text_file_with_txt << line_str;
            }

            index_file.close();
        }
    }


    for(int pp = 0; pp < source_count_file_name_list_from_text_file_with_txt.size(); pp++)
    {
        QString source_txt_file_path = conf_source + source_count_file_name_list_from_text_file_with_txt.at(pp);

        QString root_cnt_nm = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Root_Count_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Root_Count_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Root_Count_Name_QString:====:" + root_cnt_nm ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_cnt_nme = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Count_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Count_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Count_Name_QString:====:" + src_cnt_nme ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_hirarchy = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Hierarchy_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Hierarchy_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Hierarchy_QString:====:" + src_hirarchy ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_nstd = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Nested_bool");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Nested_bool="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Nested_bool:====:" + src_nstd ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_dcmprsd = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Decompressed_bool");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Decompressed_bool="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Decompressed_bool:====:" + src_dcmprsd ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_type_dsply = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Type_Display_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Type_Display_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Type_Display_QString:====:" + src_type_dsply ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_type_intrl = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Type_Internal_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Type_Internal_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Type_Internal_QString:====:" + src_type_intrl ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Name_QString:====:" + src_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_main_pth = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Main_Path_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Main_Path_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Main_Path_QString:====:" + src_main_pth ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_pth_1 = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Path_1_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Path_1_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Path_1_QString:====:" + src_pth_1 ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_pth_2_fsn = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Path_2_For_Fusion_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Path_2_For_Fusion_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Path_2_For_Fusion_QString:====:" + src_pth_2_fsn ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString timezn_readble = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Time_Zone_Readable_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Time_Zone_Readable_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Time_Zone_Readable_QString:====:" + timezn_readble ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString timezn_numeric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Time_Zone_Numeric_qint64");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Time_Zone_Numeric_qint64="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Time_Zone_Numeric_qint64:====:" + timezn_numeric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString os_schm_disply = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Scheme_Display_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Scheme_Display_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Scheme_Display_QString:====:" + os_schm_disply ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString os_schm_intrnl = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Scheme_Internal_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Scheme_Internal_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Scheme_Internal_QString:====:" + os_schm_intrnl ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString systm_accnt = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_System_Account_QList");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_System_Account_QList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_System_Account_QList:====:" + systm_accnt ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString vir_src_pth = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Virtual_Source_Path_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Virtual_Source_Path_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Virtual_Source_Path_QString:====:" + vir_src_pth ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString prodct_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Product_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Product_Type_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Product_Type_QString:====:" + prodct_type ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString os_ver_extd = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Version_Extended_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Version_Extended_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Version_Extended_QString:====:" + os_ver_extd ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString os_ver_full = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Version_Full_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Version_Full_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Version_Full_QString:====:" + os_ver_full ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString country_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Country_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Country_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Country_QString:====:" + country_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString city_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_City_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_City_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_City_QString:====:" + city_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString latitude_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Latitude_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Latitude_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Latitude_QString:====:" + latitude_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString longitude_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Longitude_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Longitude_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Longitude_QString:====:" + longitude_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString build_ver = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Build_Version_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Build_Version_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Build_Version_QString:====:" + build_ver ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString dsply_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Display_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Display_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Display_Name_QString:====:" + dsply_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString device_nm = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Device_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Device_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Device_Name_QString:====:" + device_nm ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_id = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Id_No_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Id_No_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Id_No_QString:====:" + src_id ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString guid_str = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_GUID_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_GUID_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_GUID_QString:====:" + guid_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString prodct_nme = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Product_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Product_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Product_Name_QString:====:" + prodct_nme ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString trgt_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Target_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Target_Type_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Target_Type_QString:====:" + trgt_type ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString itune_ver = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_iTune_Version_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_iTune_Version_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_iTune_Version_QString:====:" + itune_ver ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString serial_no = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Serial_No_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Serial_No_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Serial_No_QString:====:" + serial_no ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString phn_no_msisdn = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Phone_No_MSISDN_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Phone_No_MSISDN_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Phone_No_MSISDN_QStringList:====:" + phn_no_msisdn ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString imei_no = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Imei_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Imei_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Imei_QStringList:====:" + imei_no ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString sim_no = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Sim_No_Iccid_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Sim_No_Iccid_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Sim_No_Iccid_QStringList:====:" + sim_no ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString ltst_bckup_date_rdble = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Latest_Backup_Date_Readable_QString:====:" + ltst_bckup_date_rdble ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString ltst_bckup_date_numric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Latest_Backup_Date_Numeric_qint64:====:" + ltst_bckup_date_numric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString byte_size_numeric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Backup_Size_Byte_Numeric_QString:====:" + byte_size_numeric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString config_id = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Configured_ID_QStringList");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Configured_ID_QStringList="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Configured_ID_QStringList:====:" + config_id ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_main_name = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Main_Name_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Main_Name_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Main_Name_QString:====:" + src_main_name ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString vol_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Volume_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Volume_Type_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Volume_Type_QString:====:" + vol_type ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString vol_offset = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Volume_Offset_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Volume_Offset_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Volume_Offset_QString:====:" + vol_offset ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString vol_uuid = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Volume_UUID_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Volume_UUID_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Volume_UUID_QString:====:" + vol_uuid ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString byte_numeric = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Total_Size_Byte_Numeric_QString:====:" + byte_numeric ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString space_byte_num = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Used_Space_Byte_Numeric_QString:====:" + space_byte_num ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString free_space_byte = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Free_Space_Byte_Numeric_QString:====:" + free_space_byte ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString file_sys_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_File_System_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_File_System_Type_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_File_System_Type_QString:====:" + file_sys_type ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString lgl_str = "0";
        QString logical_mode = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_RECON_Logical_Mode_bool");
        QString logical_img = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_RECON_Logical_Image_bool");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_RECON_Logical_Image_bool="),Q_FUNC_INFO);
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_RECON_Logical_Mode_bool="),Q_FUNC_INFO);
        if(logical_mode == "1" || logical_img == "1")
        {
            lgl_str = "1";
        }
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_RECON_Logical_Mode_bool:====:" + lgl_str ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_disk_idenfir = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Disk_Identifier_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Disk_Identifier_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Disk_Identifier_QString:====:" + src_disk_idenfir ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString os_instlr_time_stmp = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_OS_Installer_Timestamp_UTC_Numeric_qint64:====:" + os_instlr_time_stmp ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString prodct_model = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Product_Model_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Product_Model_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Product_Model_QString:====:" + prodct_model ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString bluetooth_dvc_add = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Bluetooth_Device_Address_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Bluetooth_Device_Address_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Bluetooth_Device_Address_QString:====:" + bluetooth_dvc_add ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString wifi_add = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Wifi_Address_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Wifi_Address_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Wifi_Address_QString:====:" + wifi_add ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        QString src_locale_lang = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Locale_Language_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Locale_Language_QString="),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Locale_Language_QString:====:" + src_locale_lang ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        recon_static_functions::append_data_into_file(source_txt_file_path, "MACRO_NARAD_Source_Role_Type_QString:====:" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

    }

    //======================Source txt file update End====================//

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void MainWindow::update_source_149_to_150()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString source_info_db_path = conf_source + "sources_info.sqlite";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(source_info_db_path);
    if(source_db.open())
    {
        //======================sources_info.sqlite tbl_source_index Update Start ==================//

        QString cmd_update = "UPDATE tbl_source_index SET volume_type = ''";

        recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_update, source_db, Q_FUNC_INFO);

        //======================sources_info.sqlite tbl_source_index Update End ==================//



        //======================sources_info.sqlite fs_status Update Start ==================//

        QString ftr_str_face = QString("Face Analysis").replace(" ", "_") ;
        QString ftr_str_ocr = QString("Optical Character Recognition").replace(" ", "_");
        QString ftr_str_ndty = QString("Skin Tone Detection").replace(" ", "_") ;
        QString ftr_str_weapons = QString("Weapons").replace(" ", "_");
        QString ftr_str_fire_analysis = QString("Fire Analysis").replace(" ", "_");

        QStringList new_colname_list;
        new_colname_list <<  ftr_str_face << ftr_str_ocr << ftr_str_ndty << ftr_str_weapons << ftr_str_fire_analysis;

        QStringList new_coltype_list;
        new_coltype_list  << "varchar(10)" << "varchar(10)" << "varchar(10)" << "varchar(10)" << "varchar(10)";

        for(int ii  = 0 ; ii < new_colname_list.size() ; ii++)
        {
            if(!source_db.record("fs_status").contains(new_colname_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE 'fs_status' ADD '"+ new_colname_list.at(ii) + "' '" + new_coltype_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, source_db, Q_FUNC_INFO);

                QString cmd_update = "UPDATE 'fs_status' SET '" + new_colname_list.at(ii) + "' = '0'";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_update, source_db, Q_FUNC_INFO);
            }
        }
        //======================sources_info.sqlite fs_status Update End ==================//

        source_db.close();
    }

    //======================sources text_file Update Start ==================//

    QString source_indx_file_path = conf_source + "sources_index.txt";
    QStringList source_count_file_name_list_from_text_file_with_txt;

    QFile index_file(source_indx_file_path);
    if(index_file.exists())
    {
        if(index_file.open(QIODevice::ReadOnly))
        {
            while(!index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(index_file.readLine()).trimmed();
                source_count_file_name_list_from_text_file_with_txt << line_str;
            }

            index_file.close();
        }
    }

    for(int pp = 0; pp < source_count_file_name_list_from_text_file_with_txt.size(); pp++)
    {
        QString source_txt_file_path = conf_source + source_count_file_name_list_from_text_file_with_txt.at(pp);

        //            QString vol_type = get_value_of_key_from_source_textinfo_file_from_100_to_148(source_txt_file_path, "MACRO_NARAD_Source_Volume_Type_QString");
        recon_static_functions::remove_data_contain_line_from_file(source_txt_file_path,QString("MACRO_NARAD_Source_Volume_Type_QString:====:"),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(source_txt_file_path,"MACRO_NARAD_Source_Volume_Type_QString:====:" + QString("") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
    }
    //======================sources text_file Update End ==================//


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void MainWindow::update_source_150_to_153()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=======================roots_info db update starts=======================//
    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString destination_db_file = conf_source + "roots_info.sqlite";

    QString cmd_update_root_index = "update tbl_root_index set root_type = 'GrayKey Backup' where root_type = 'GrayKey iOS Backup'";

    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_update_root_index, destination_db_file, Q_FUNC_INFO);

    //=======================roots_info db update ends=======================//

    //=======================root_count file update starts=======================//

    QString rt_index_file_path = conf_source + "roots_index.txt";
    QStringList root_count_name_list_from_text_file_with_txt;
    QFile rt_index_file(rt_index_file_path);
    if(rt_index_file.exists())
    {
        if(rt_index_file.open(QIODevice::ReadOnly))
        {
            while(!rt_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(rt_index_file.readLine()).trimmed();
                root_count_name_list_from_text_file_with_txt << line_str;
            }

            rt_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_text_file_with_txt.size(); hh++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_text_file_with_txt.at(hh);
        QString m_root_type = get_value_of_key_from_source_textinfo_file_from_149_to_current_version(root_txt_file_path, "MACRO_NARAD_Root_Type_QString");

        if(m_root_type == "GrayKey iOS Backup")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Type_QString:====:"),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Type_QString:====:" + QString("GrayKey Backup") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //=======================root_count file update ends=======================//

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_source_153_to_156()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=======================roots_info db update starts=======================//
    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString destination_db_root_file = conf_source + "roots_info.sqlite";

    QString cmd_update_root_index = "update tbl_root_index set root_type = 'Time Machine Backup Folder', source_type_display = 'Time Machine Backup Folder', source_type_internal = 'Time_Machine_Backup_Folder' where root_type = 'Time Machine Backup'";

    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_update_root_index, destination_db_root_file, Q_FUNC_INFO);
    //=======================roots_info db update ends=======================//


    //=======================sources_info db update starts=======================//
    QString destination_db_source_file = conf_source + "sources_info.sqlite";

    QString cmd_update_source_index = "update tbl_source_index set source_type_display = 'Time Machine Backup Folder', source_type_internal = 'Time_Machine_Backup_Folder' WHERE source_type_display = 'Time Machine Backup'";

    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_update_source_index, destination_db_source_file, Q_FUNC_INFO);
    //=======================sources_info db update ends=======================//


    //=======================root_count file update starts=======================//

    QString rt_index_file_path = conf_source + "roots_index.txt";
    QStringList root_count_name_list_from_text_file_with_txt;
    QFile rt_index_file(rt_index_file_path);
    if(rt_index_file.exists())
    {
        if(rt_index_file.open(QIODevice::ReadOnly))
        {
            while(!rt_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(rt_index_file.readLine()).trimmed();
                root_count_name_list_from_text_file_with_txt << line_str;
            }

            rt_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_text_file_with_txt.size(); hh++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_text_file_with_txt.at(hh);
        QString m_root_type = get_value_of_key_from_source_textinfo_file_from_149_to_current_version(root_txt_file_path, "MACRO_NARAD_Root_Type_QString");

        if(m_root_type == "Time Machine Backup")
        {
            recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Type_QString:====:"),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Type_QString:====:" + QString("Time Machine Backup Folder") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


            recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Type_Display_QString:====:"),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Display_QString:====:" + QString("Time Machine Backup Folder") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);


            recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Source_Type_Internal_QString:====:"),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Source_Type_Internal_QString:====:" + QString("Time_Machine_Backup_Folder") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }
    //=======================root_count file update ends=======================//


    //=======================source_count file update starts=======================//
    QString src_index_file_path = conf_source + "sources_index.txt";
    QStringList src_count_name_list_from_text_file_with_txt;
    QFile src_index_file(src_index_file_path);
    if(src_index_file.exists())
    {
        if(src_index_file.open(QIODevice::ReadOnly))
        {
            while(!src_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(src_index_file.readLine()).trimmed();
                src_count_name_list_from_text_file_with_txt << line_str;
            }

            src_index_file.close();
        }
    }

    for(int hh = 0; hh < src_count_name_list_from_text_file_with_txt.size(); hh++)
    {
        QString src_txt_file_path = conf_source + src_count_name_list_from_text_file_with_txt.at(hh);
        QString m_source_type_internal = get_value_of_key_from_source_textinfo_file_from_149_to_current_version(src_txt_file_path, "MACRO_NARAD_Source_Type_Internal_QString");

        if(m_source_type_internal == "Time_Machine_Backup")
        {
            recon_static_functions::remove_data_contain_line_from_file(src_txt_file_path,QString("MACRO_NARAD_Source_Type_Display_QString:====:"),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(src_txt_file_path,"MACRO_NARAD_Source_Type_Display_QString:====:" + QString("Time Machine Backup Folder") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

            recon_static_functions::remove_data_contain_line_from_file(src_txt_file_path,QString("MACRO_NARAD_Source_Type_Internal_QString:====:"),Q_FUNC_INFO);
            recon_static_functions::append_data_into_file(src_txt_file_path,"MACRO_NARAD_Source_Type_Internal_QString:====:" + QString("Time_Machine_Backup_Folder") ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
        }
    }

    //=======================source_count file update ends=======================//


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_source_156_to_160()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=======================roots_info db update starts=======================//

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString destination_db_file = conf_source + "roots_info.sqlite";

    QStringList new_colname_list;
    new_colname_list <<  "root_evidence_password";

    QStringList new_coltype_list;
    new_coltype_list  << "varchar(100)";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_file);
    if(source_db.open())
    {

        for(int ii  = 0; ii < new_colname_list.size(); ii++)
        {
            if(!source_db.record("tbl_root_index").contains(new_colname_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE 'tbl_root_index' ADD '"+ new_colname_list.at(ii) + "' '" + new_coltype_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, source_db, Q_FUNC_INFO);

            }
        }

        if(source_db.record("tbl_root_index").contains("root_password"))
        {
            QString rename_cmd = "ALTER TABLE tbl_root_index RENAME COLUMN root_password to root_recovery_passcode";
            recon_helper_standard_obj->execute_db_command_by_dbreference(rename_cmd, source_db, Q_FUNC_INFO);
        }
        source_db.close();
    }
    //=======================roots_info db update ends=======================//

    //=======================root_count file update starts=======================//

    QString rt_index_file_path = conf_source + "roots_index.txt";
    QStringList root_count_name_list_from_text_file_with_txt;
    QFile rt_index_file(rt_index_file_path);
    if(rt_index_file.exists())
    {
        if(rt_index_file.open(QIODevice::ReadOnly))
        {
            while(!rt_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(rt_index_file.readLine()).trimmed();
                root_count_name_list_from_text_file_with_txt << line_str;
            }

            rt_index_file.close();
        }
    }

    for(int hh = 0; hh < root_count_name_list_from_text_file_with_txt.size(); hh++)
    {
        QString root_txt_file_path = conf_source + root_count_name_list_from_text_file_with_txt.at(hh);

        QString root_password = get_value_of_key_from_source_textinfo_file_from_149_to_current_version(root_txt_file_path,"MACRO_NARAD_Root_Password_Encrypted_QString");
        recon_static_functions::remove_data_contain_line_from_file(root_txt_file_path,QString("MACRO_NARAD_Root_Password_Encrypted_QString:====:"),Q_FUNC_INFO);
        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Recovery_Passcode_Encrypted_QString:====:" + root_password,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);

        recon_static_functions::append_data_into_file(root_txt_file_path,"MACRO_NARAD_Root_Evidence_Password_Encrypted_QString:====:" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
    }

    //=======================root_count file update ends=======================//


    //=======================sources_info db update starts=======================//

    QString srcs_destination_db_file = conf_source + "sources_info.sqlite";

    new_colname_list.clear();
    new_colname_list <<  "source_evidence_password";

    new_coltype_list.clear();
    new_coltype_list  << "varchar(100)";

    QSqlDatabase::removeDatabase(QString(Q_FUNC_INFO) + "_1");
    QSqlDatabase srcs_source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    srcs_source_db.setDatabaseName(srcs_destination_db_file);
    if(srcs_source_db.open())
    {

        for(int ii  = 0 ; ii < new_colname_list.size() ; ii++)
        {
            if(!source_db.record("tbl_source_index").contains(new_colname_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE 'tbl_source_index' ADD '"+ new_colname_list.at(ii) + "' '" + new_coltype_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, srcs_source_db, Q_FUNC_INFO);

            }
        }

        srcs_source_db.close();
    }
    //=======================sources_info db update ends=======================//

    //=======================source_count file update starts=======================//
    QString src_index_file_path = conf_source + "sources_index.txt";
    QStringList src_count_name_list_from_text_file_with_txt;
    QFile src_index_file(src_index_file_path);
    if(src_index_file.exists())
    {
        if(src_index_file.open(QIODevice::ReadOnly))
        {
            while(!src_index_file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(src_index_file.readLine()).trimmed();
                src_count_name_list_from_text_file_with_txt << line_str;
            }

            src_index_file.close();
        }
    }

    for(int hh = 0; hh < src_count_name_list_from_text_file_with_txt.size(); hh++)
    {
        QString src_txt_file_path = conf_source + src_count_name_list_from_text_file_with_txt.at(hh);

        recon_static_functions::append_data_into_file(src_txt_file_path,"MACRO_NARAD_Source_Evidence_Password_Encrypted_QString:====:" ,enum_global_append_data_in_file_with_append_one_next_line,Q_FUNC_INFO);
    }

    //=======================source_count file update ends=======================//

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_source_160_to_161()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //======================sources_info.sqlite fs_status Update Start ==================//

    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    QString srcs_destination_db_file = conf_source + "sources_info.sqlite";

    QSqlDatabase::removeDatabase(QString(Q_FUNC_INFO));
    QSqlDatabase srcs_source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    srcs_source_db.setDatabaseName(srcs_destination_db_file);
    if(srcs_source_db.open())
    {
        QString unified_logs_str = QString("Unified Logs").replace(" ", "_") ;

        QStringList new_colname_list;
        new_colname_list <<  unified_logs_str;

        QStringList new_coltype_list;
        new_coltype_list  << "varchar(10)";

        for(int ii  = 0 ; ii < new_colname_list.size() ; ii++)
        {
            if(!srcs_source_db.record("fs_status").contains(new_colname_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE 'fs_status' ADD '"+ new_colname_list.at(ii) + "' '" + new_coltype_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, srcs_source_db, Q_FUNC_INFO);

                QString cmd_update = "UPDATE 'fs_status' SET '" + new_colname_list.at(ii) + "' = '0'";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_update, srcs_source_db, Q_FUNC_INFO);
            }
        }
    }
    //======================sources_info.sqlite fs_status Update End ==================//
    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}
