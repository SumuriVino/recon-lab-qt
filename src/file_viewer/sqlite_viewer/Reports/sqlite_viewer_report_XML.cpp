#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::sqlite_viewer_create_xml_report(QString file_path, int REPORT_CONTENT_FLAG)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(ui->checkBox_export->isChecked())
    {
        sqlite_viewer_export_db_sanity_check(file_path,REPORT_CONTENT_FLAG);
    }

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        recon_static_functions::app_debug("RECON -----FAILED---- to open file",Q_FUNC_INFO);
        return;
    }

    //1.
    QXmlStreamWriter stream(&file);

    QString sqlite_plugin_name =  QString(MACRO_Plugin_Name_Sqlite_Viewer);
    stream.writeStartElement(sqlite_plugin_name.replace(" ","_")); // XML Start

    if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_FULL_REPORT_int)
    {
        global_recon_helper_singular_class_obj->write_xml_report_case_info_and_source_info(&stream,QString(MACRO_REPORT_SCOPE_FULL_QString), global_narad_muni_class_obj);
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
    {
        global_recon_helper_singular_class_obj->write_xml_report_case_info_and_source_info(&stream,QString(MACRO_REPORT_SCOPE_TAGS_QString), global_narad_muni_class_obj);
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
    {
        global_recon_helper_singular_class_obj->write_xml_report_case_info_and_source_info(&stream,QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString), global_narad_muni_class_obj);
    }


    stream.setAutoFormatting(true);

    for(int combo = 0; combo < ui->comboBox_dbtables_selection->count(); combo++)
    {
        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
        {
            if(ui->comboBox_dbtables_selection->itemText(combo) != current_tab_name)
                continue;
        }

        struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(combo);
        m_tablewidget *tableWidget_main_data_display = struct_main_object.tableWidget_MainDataDisplay_ptr;

        QTableWidget *tableWidget_filter_view_search = struct_main_object.tableWidget_FilterViewSearch_ptr;
        QTableWidget *tableWidget_column_selection = struct_main_object.tableWidget_columns_selection_ptr;

        QString table_name = struct_main_object.table_name;

        bool bool_preview_coloum_enable = false;
        QSqlDatabase dest_db;

        if(ui->checkBox_export->checkState())
        {
            for(int k = 0; k < tableWidget_filter_view_search->rowCount(); k++)
            {
                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(k ,2))->currentText());

                if(recivedComboboxText == "File")
                {
                    bool_preview_coloum_enable = true;
                    break;
                }
            }

            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            dest_db.setDatabaseName(export_db_destination_path);
            if(!dest_db.open())
            {
                recon_static_functions::app_debug("dest_export_db  open ----FAILED--- " + export_db_destination_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + dest_db.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug("DB PATH " + export_db_destination_path,Q_FUNC_INFO);
            }
        }

        //2.
        bool bool_check_bookmark = false;
        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
        {
            for(int row = 0; row < tableWidget_main_data_display->rowCount(); row++)
            {
                if(tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Unchecked)
                    continue;
                else
                {
                    bool_check_bookmark = true;
                    break;
                }
            }

            if(bool_check_bookmark)
            {
                stream.writeStartElement(table_name.replace(" ","_")); // Table Start
            }
            else
            {
                if(dest_db.isOpen())
                    dest_db.close();
                continue;
            }
        }
        else
        {
            stream.writeStartElement(table_name.replace(" ","_")); // Table Start
        }

        int count = 0;

        for(int row = 0; row < tableWidget_main_data_display->rowCount(); row++)
        {
            if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
            {
                if(tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Unchecked)
                    continue;
            }

            if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
            {
                if(tableWidget_main_data_display->isRowHidden(row))
                    continue;
            }

            count++;

            stream.writeStartElement(table_name + "_Detail"); //Table Detail start

            QString num = QString::number(count);

            stream.writeTextElement(QObject::tr("Sr._No."),num);

            QString coloumnData;
            QString recon_int_column;
            for(int col = 0; col < tableWidget_main_data_display->columnCount(); col++)
            {
                if(col == 0 || (col == tableWidget_main_data_display->columnCount() - 1))
                    continue;

                if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
                {
                    if(!(tableWidget_main_data_display->columnCount() -1))
                    {
                        if(tableWidget_main_data_display->isColumnHidden(col))
                            continue;
                    }
                }

                //---To avoid hidden columns to go in report; continued here ----//

                QString header;
                header = tableWidget_main_data_display->horizontalHeaderItem(col)->text();

                if(header == QString("RECON Examiner Notes") || header == QString("RECON Examiner Tags"))
                {
                    ///to remain hide in display but make visible in reports
                }
                else if(tableWidget_main_data_display->isColumnHidden(col))
                {
                    continue;
                }

                if(col == (tableWidget_main_data_display->columnCount() - 1) || col ==  1)
                {
                    continue;
                }

                if(col == enum_coloumn_Sqlite_viewer_record_no)
                {
                    recon_int_column = tableWidget_main_data_display->item(row,col)->text();
                }

                QString data;

                data = tableWidget_main_data_display->item(row,col)->text();

                QString convertedData;

                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(col ,2))->currentText());

                if(recivedComboboxText != "Select Conversion")
                {
                    convertedData =  convert_data(data,recivedComboboxText);

                    if(convertedData.contains("<b>File Exist: </b>"))
                    {
                        convertedData.replace("<br>","\n");
                        convertedData.remove("\t");
                        convertedData.remove("<b>");
                        convertedData.remove("</b>");
                        convertedData.remove("<font  color=\"red\">");
                        convertedData.remove("<font  color=\"green\">");
                        convertedData.remove("</font>");
                    }

                    if(recivedComboboxText == "File")
                    {
                        coloumnData += convertedData;
                        QString headers = tableWidget_main_data_display->horizontalHeaderItem(col)->text();
                        stream.writeTextElement(headers.replace(" ","_"),coloumnData);
                    }
                    else
                    {
                        coloumnData +=  data ;
                        QString header = tableWidget_main_data_display->horizontalHeaderItem(col)->text();
                        stream.writeTextElement(header.replace(" ","_").append("_Original_Value"),coloumnData);

                        coloumnData.clear();
                        coloumnData += convertedData;
                        QString headers = tableWidget_main_data_display->horizontalHeaderItem(col)->text();
                        stream.writeTextElement(headers.replace(" ","_").append("_Converted_Value"),coloumnData);
                    }
                }
                else
                {
                    coloumnData +=  data ;
                    QString headers = tableWidget_main_data_display->horizontalHeaderItem(col)->text();
                    stream.writeTextElement(headers.replace(" ","_"),coloumnData);
                }

                coloumnData.clear(),convertedData.clear(),data.clear();
            }

            if((ui->checkBox_export->isChecked()) && (bool_preview_coloum_enable == true))
            {
                stream.writeStartElement(QObject::tr("Exported_File_Detail"));

                QSqlQuery query_export_detail(dest_db);

                QString cmnd = QString("SELECT exp_relativepath, exp_size, exp_hashmd5, exp_hashsha1, exp_time"
                                       " FROM exportfilelist where table_name = ? AND INT = ?");
                query_export_detail.prepare(cmnd);
                query_export_detail.addBindValue(table_name);
                query_export_detail.addBindValue(recon_int_column);
                if(!query_export_detail.exec())
                {
                    recon_static_functions::app_debug("query_export_detail ----FAILED---"+ query_export_detail.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("ERROR:-" + query_export_detail.lastError().text(),Q_FUNC_INFO);
                }

                bool bool_export_detail = false;
                while(query_export_detail.next())
                {
                    //                        QString f_name = query_export_detail.value(1).toString().trimmed();
                    QString f_relative_path = QString::fromUtf8(query_export_detail.value(0).toString().toLocal8Bit().data());
                    QString f_size = recon_static_functions::human_readable_size(query_export_detail.value(1).toString(), Q_FUNC_INFO)
                            + " (" + query_export_detail.value(1).toString() + QObject::tr(" bytes") + ")";
                    QString f_md5_string  = query_export_detail.value(2).toString();
                    QString f_sha1_string = query_export_detail.value(3).toString();
                    QString f_export_time = query_export_detail.value(4).toString();

                    QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
                    QString numeric_timezone_value;
                    if(machine_timezone_string.contains("GMT"))
                    {
                        QStringList tmp_list = machine_timezone_string.split("GMT");
                        numeric_timezone_value = tmp_list.at(1);
                    }

                    if(f_relative_path != QString(""))
                    {
                        bool_export_detail = true;
                        stream.writeStartElement(QObject::tr("File_Detail"));
                        //                            stream.writeTextElement(QObject::tr("File_Name"), f_name);
                        stream.writeTextElement(QObject::tr("New_Path"), f_relative_path);
                        stream.writeTextElement(QObject::tr("Size"), f_size);
                        stream.writeTextElement(QObject::tr("MD5"), f_md5_string);
                        stream.writeTextElement(QObject::tr("SHA1"), f_sha1_string);
                        stream.writeTextElement(QObject::tr("Export_Time"),global_recon_helper_singular_class_obj->convert_numeric_to_readable_export_time(f_export_time + " " +  numeric_timezone_value, Q_FUNC_INFO) );
                        stream.writeEndElement();
                    }
                }
                if(bool_export_detail)
                    stream.writeEndElement();
                else
                {
                    stream.writeTextElement(QObject::tr("sr."),QObject::tr("File_does_not_exist"));
                    stream.writeEndElement();
                }
            }

            stream.writeEndElement(); //Table Detail end
        }

        stream.writeEndElement(); // Table End

        if(dest_db.isOpen())
            dest_db.close();
    }

    stream.writeEndElement(); //END of page

    file.close();

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void sqlite_viewer::create_Screen_xml_report_for_query_execution(QString file_path)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        recon_static_functions::app_debug("-----FAILED---- to open file " + file_path,Q_FUNC_INFO);
        return;
    }

    QXmlStreamWriter stream(&file);

    QString sqlite_plugin_name =  QString(MACRO_Plugin_Name_Sqlite_Viewer);
    stream.writeStartElement(sqlite_plugin_name.replace(" ","_")); // XML Start
    stream.setAutoFormatting(true);

    //------//
    struct_global_sqlite_viewer_tablewidget_main obj_main;
    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return ;
    }

    m_tablewidget *tablewidget_main_data_display;
    tablewidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

    QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

    global_recon_helper_singular_class_obj->write_xml_report_case_info_and_source_info(&stream,QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString), global_narad_muni_class_obj);

    stream.writeTextElement("File_Path",actual_file_path);
    //Table Detail start
    stream.writeTextElement("Executed_Query",ui->textEdit_query_insert->toPlainText());

    stream.writeStartElement("Query_Execution"); //Table Detail start
    int count = 0;
    for(int row = 0; row < ui->tableWidget_query_execution->rowCount(); row++)
    {
        stream.writeStartElement("Query_Execution_Detail"); //Table Detail start
        count++;
        QString num = QString::number(count);
        stream.writeTextElement(QObject::tr("Sr._No."),num);

        QString data_str;

        for(int col= 0; col < ui->tableWidget_query_execution->columnCount(); col++)
        {
            QString header_str = ui->tableWidget_query_execution->horizontalHeaderItem(col)->text();

            if(header_str.trimmed().isEmpty())
                continue;


            data_str = ui->tableWidget_query_execution->item(row,col)->text();

            bool bool_is_conversion_required = check_current_column_selected_for_conversion_in_manage_column(header_str);

            if(bool_is_conversion_required)
            {
                QString convertedData;
                QString recivedComboboxText;

                int col_index = get_current_column_selected_for_conversion_in_manage_column_index(header_str);
                recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col_index) ,2))->currentText());

                convertedData =  convert_data(data_str,recivedComboboxText);

                bool bool_is_conversion_required_for_file = check_current_column_selected_for_conversion_in_manage_column_for_file(header_str);

                if(bool_is_conversion_required_for_file)
                {
                    stream.writeTextElement(header_str.replace(" ","_") + QString(" Original Value").replace(" ", "_"),data_str);
                    stream.writeTextElement(header_str.replace(" ","_") + QString(" Converted Value").replace(" ", "_"),convertedData);
                }
                else
                {
                    stream.writeTextElement(header_str.replace(" ","_"),convertedData);
                }
            }
            else
            {
                stream.writeTextElement(header_str.replace(" ","_"),data_str);
            }
        }

        stream.writeEndElement(); //Table Detail end
    }

    stream.writeEndElement(); // Table Detail End
    stream.writeEndElement(); // XML End

    file.close();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);

}


