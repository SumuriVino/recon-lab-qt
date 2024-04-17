#include "z_mainwindow/mainwindow.h"


void MainWindow::update_examiner_machine_recon_library()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    update_recon_library_by_118();
    update_recon_library_by_145();
    update_recon_library_by_147();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void MainWindow::update_recon_library_by_118()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString file_path_older = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "confidential.txt";

    QFile mfile(file_path_older);
    if(mfile.exists())
    {
        QString file_path_orgnl = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString).toString() + "confidential_original.txt";
        QFile::copy(file_path_older, file_path_orgnl);

        QString m_data = recon_static_functions::get_plain_data_from_encrypt_file_1(file_path_orgnl, Q_FUNC_INFO);

        m_data = recon_static_functions::get_modify_password(m_data, Q_FUNC_INFO);

        QString file_path_modif = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Confidential_Dir_Path_QString).toString() + "confidential_modified.txt";
        recon_static_functions::overwrite_data_into_file(file_path_modif, m_data, Q_FUNC_INFO);
        recon_static_functions::encrypt_file_1(file_path_modif, Q_FUNC_INFO);

        mfile.remove();
    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void MainWindow::update_recon_library_by_145()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString mounted_image_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "mounted_images/mounted_images_info.sqlite";

    if(QFile(mounted_image_db_path).exists())
    {
        QString command = "PRAGMA TABLE_INFO(images_info)";
        QStringList column_name_list_from_images_info = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,mounted_image_db_path,Q_FUNC_INFO);

        if(!column_name_list_from_images_info.contains("image_source_type_display"))
        {
            QString add_col_cmd = "ALTER TABLE images_info add column image_source_type_display VARCHAR(100)";
            recon_helper_standard_obj->execute_db_command_by_dbpath(add_col_cmd,mounted_image_db_path,Q_FUNC_INFO);
        }

        if(column_name_list_from_images_info.contains("image_category"))
        {
            QString rename_cmd = "ALTER TABLE images_info RENAME COLUMN image_category to image_source_type_internal";
            recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,mounted_image_db_path,Q_FUNC_INFO);
        }

        if(column_name_list_from_images_info.contains("image_type"))
        {
            QString rename_cmd = "ALTER TABLE images_info RENAME COLUMN image_type to image_format_type";
            recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,mounted_image_db_path,Q_FUNC_INFO);
        }

    }
    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}


void MainWindow::update_recon_library_by_147()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString recon_library_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString();

    QString mounted_image_db_path = recon_library_path + QString("mounted_images/mounted_images_info.sqlite");

    if(QFile(mounted_image_db_path).exists())
    {
        QString command = "PRAGMA TABLE_INFO(partition_info)";
        QStringList column_name_list_from_partiton_info = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,mounted_image_db_path,Q_FUNC_INFO);

        if(column_name_list_from_partiton_info.contains("volume_total_size"))
        {
            QString rename_cmd = "ALTER TABLE partition_info RENAME COLUMN volume_total_size to volume_total_size_byte_numeric";
            recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,mounted_image_db_path,Q_FUNC_INFO);
        }

        if(column_name_list_from_partiton_info.contains("volume_free_size"))
        {
            QString rename_cmd = "ALTER TABLE partition_info RENAME COLUMN volume_free_size to volume_free_size_byte_numeric";
            recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,mounted_image_db_path,Q_FUNC_INFO);
        }

    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

bool MainWindow::need_to_update_result_for_load_case()
{
    QString current_app_version = recon_static_functions::get_app_version();

    QString case_conf_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";

    ///create column first if not exist and add version from result
    QString table_name     = "casedetails";
    QString command_pragma = QString("PRAGMA TABLE_INFO( '") + table_name + "')";
    QStringList existing_colname_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_pragma, 1, case_conf_db_file, Q_FUNC_INFO);

    QString column_str = "result_updated_for_versions";
    if(!existing_colname_list.contains(column_str))
    {
        QStringList columns_name_list;
        columns_name_list <<  column_str;

        QStringList columns_field_type_list;
        columns_field_type_list << "varchar(500)" ;

        for(int ii = 0; ii < columns_name_list.size(); ii++)
        {
            QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
            recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, case_conf_db_file, Q_FUNC_INFO);

            cmd = "UPDATE '" + table_name +"' SET "+ column_str + "=app_version";
            recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, case_conf_db_file, Q_FUNC_INFO);
        }
    }


    ///get version from db
    QString vers_command = "select " + column_str + " from casedetails";
    QString updated_all_versions = recon_helper_standard_obj->get_string_from_db_by_dbpath(vers_command, 0, case_conf_db_file, Q_FUNC_INFO);
    updated_all_versions = updated_all_versions.trimmed();

    ///get version from text file
    if(updated_all_versions.isEmpty())
    {
        ///extract from text file
        QString case_conf_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";
        QFile case_file(case_conf_txt_file);
        if(case_file.open(QIODevice::ReadOnly))
        {
            while(!case_file.atEnd())
            {
                QByteArray line_arr = case_file.readLine();
                if(line_arr.startsWith("MACRO_NARAD_Result_Updated_For_Versions_QString"))
                {
                    QString tmp_str = QString::fromLocal8Bit(line_arr);
                    if(tmp_str.contains(QString("=")))
                    {
                        QStringList tmp_split_list = tmp_str.split(QString("="));
                        updated_all_versions = tmp_split_list.at(tmp_split_list.size() -1);
                    }

                    break;
                }
            }
            case_file.close();
        }
    }
    updated_all_versions = updated_all_versions.trimmed();

    ///check result is already updated
    bool bool_need_to_update = true;
    if(updated_all_versions.contains(MACRO_RECON_Splitter_5_resolution_scope))
    {
        QStringList tmp_split_list = updated_all_versions.split(MACRO_RECON_Splitter_5_resolution_scope);
        for(int count = 0; count < tmp_split_list.size(); count++)
        {
            QString tmp_versn_str = tmp_split_list.at(count);
            if(tmp_versn_str.trimmed() == current_app_version.trimmed())
            {
                bool_need_to_update = false;
                break;
            }
        }
    }
    else
    {
        if(updated_all_versions.trimmed() == current_app_version.trimmed())
            bool_need_to_update = false;
    }

    ///update version in case details
    if(bool_need_to_update)
    {
        QString final_update_str = updated_all_versions.trimmed();

        if(!final_update_str.trimmed().isEmpty())
            final_update_str.append(MACRO_RECON_Splitter_5_resolution_scope);

        final_update_str.append(current_app_version);

        vers_command = "Update " + table_name + " Set " + column_str + " = '" + final_update_str + "'";
        recon_helper_standard_obj->execute_db_command_by_dbpath(vers_command, case_conf_db_file, Q_FUNC_INFO);

        ///Update in text file
        QString case_text_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

        QFile file(case_text_file_path);
        if(file.open(QIODevice::ReadWrite))
        {
            QStringList file_data_list;

            while(!file.atEnd())
            {
                QString line_str = QString::fromLocal8Bit(file.readLine()).trimmed();

                if(line_str.startsWith(QString("MACRO_NARAD_Result_Updated_For_Versions_QString")))
                    continue;

                file_data_list << line_str;
            }

            file_data_list << QString("MACRO_NARAD_Result_Updated_For_Versions_QString") + QString("=") + final_update_str;

            QString data_str = file_data_list.join("\n");

            file.resize(0);
            file.write(data_str.toLocal8Bit());

            file.flush();
            file.close();
        }

    }

    return bool_need_to_update;
}


void MainWindow::update_value_for_source_textinfo_file_only(QString file_path, QString m_keyword, QString m_value)
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QFile m_file(file_path);
    if(!m_file.exists())
        return;

    if(!m_file.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug( " text file open ---FAILED--- " + m_file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + m_file.errorString(), Q_FUNC_INFO);
        return;
    }

    m_keyword = m_keyword.trimmed();
    m_value = m_value.trimmed();

    QString redefined_data;
    QTextStream txt_strm(&m_file);
    while(!txt_strm.atEnd())
    {
        QString line = txt_strm.readLine();

        if(line.contains("="))
        {
            QStringList tmp_list = line.split("=", Qt::SkipEmptyParts);
            if(tmp_list.at(0).trimmed() != m_keyword)
                redefined_data.append(line + "\n");
        }
    }

    QString final_value = m_keyword + QString("=" + m_value + "\n");
    redefined_data.append(final_value + "\n");

    m_file.resize(0);
    txt_strm << redefined_data;
    m_file.close();

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);
}

QString MainWindow::get_value_of_key_from_source_textinfo_file_from_100_to_148(QString file_path, QString mkey)
{
    return get_value_of_key_from_source_textinfo_file_only(file_path,mkey,"=");
}

QString MainWindow::get_value_of_key_from_source_textinfo_file_from_149_to_current_version(QString file_path, QString mkey)
{
    return get_value_of_key_from_source_textinfo_file_only(file_path,mkey,":====:");
}

QString MainWindow::get_value_of_key_from_source_textinfo_file_only(QString file_path, QString mkey, QString m_splitter)
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    QString line;
    QFile m_file(file_path);
    if(!m_file.exists())
        return line;

    if(!m_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug( " text file open ---FAILED--- " + m_file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + m_file.errorString(), Q_FUNC_INFO);
        return line;
    }

    bool bool_key_found = false;

    while(!m_file.atEnd())
    {
        line = m_file.readLine();
        QString pattrn = mkey + m_splitter;

        if(line.startsWith(pattrn))
        {
            bool_key_found = true;
            line.remove(0, pattrn.size());
            break;
        }
    }

    m_file.close();

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

    if(bool_key_found)
    {
        return line.trimmed();
    }
    else
    {
        return  "";
    }


}


