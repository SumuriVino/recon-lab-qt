#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::sqlite_viewer_create_csv_report(QString file_path, int REPORT_CONTENT_FLAG)
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
        recon_static_functions::app_debug("-----FAILED---- to open file" + file_path,Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&file);
    QStringList line;
    outputfile_csv << "\n";
    QString report_type_str;

    if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_FULL_REPORT_int)
    {
        report_type_str = QString(MACRO_REPORT_SCOPE_FULL_QString);
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
    {
        report_type_str = QString(MACRO_REPORT_SCOPE_TAGS_QString);
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
    {
        report_type_str = QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString);
    }

    outputfile_csv << QString(MACRO_Plugin_Name_Sqlite_Viewer) << "\n";

    global_recon_helper_singular_class_obj->write_csv_report_case_details_and_source_info(outputfile_csv,report_type_str, global_narad_muni_class_obj);

    for(int combo = 0; combo < ui->comboBox_dbtables_selection->count(); combo++)
    {
        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
        {
            if(ui->comboBox_dbtables_selection->itemText(combo) != current_tab_name)
                continue;
        }

        struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(combo);

        m_tablewidget *tableWidget_main_data_display  = struct_main_object.tableWidget_MainDataDisplay_ptr;

        QTableWidget *tableWidget_filter_view_search = struct_main_object.tableWidget_FilterViewSearch_ptr;
        QTableWidget *tableWidget_column_selection   = struct_main_object.tableWidget_columns_selection_ptr;

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
                QString table_name = struct_main_object.table_name;
                outputfile_csv <<"\n";
                outputfile_csv << table_name <<"\n";
                outputfile_csv <<"\n";

                line << "Sr. No.";
            }
            else
            {
                if(tableWidget_main_data_display->rowCount() == 0)
                {
                    QString table_name = struct_main_object.table_name;
                    outputfile_csv <<"\n";
                    outputfile_csv << table_name <<"\n";
                    outputfile_csv <<"\n";

                    outputfile_csv << "No Data Available";
                    outputfile_csv <<"\n";outputfile_csv <<"\n";outputfile_csv <<"\n";
                    continue;
                }

                continue;
            }
        }
        else
        {
            QString table_name = struct_main_object.table_name;

            outputfile_csv <<"\n";
            outputfile_csv << table_name << "\n";
            outputfile_csv <<"\n";

            if(tableWidget_main_data_display->rowCount() == 0)
            {
                outputfile_csv << "No Data Available";
                outputfile_csv <<"\n";outputfile_csv <<"\n";
                outputfile_csv <<"\n";
                continue;
            }

            line << "Sr. No.";
        }

        bool bool_preview_coloum_enable = false;
        for(int k = 0; k < tableWidget_filter_view_search->rowCount(); k++)
        {
            QString recived_combobox_text =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(k ,2))->currentText());

            if(recived_combobox_text == "File")
            {
                bool_preview_coloum_enable = true;
                break;
            }
        }


        for(int i = 0; i < tableWidget_main_data_display->columnCount(); i++)
        {
            if((i == 0) || (i == tableWidget_main_data_display->columnCount() -1))
                continue;

            if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
            {
                if(!(tableWidget_main_data_display->columnCount() -  1))
                {
                    if(tableWidget_main_data_display->isColumnHidden(i))
                        continue;
                }
            }

            //---To avoid hidden columns to go in report; continued here ----//

            QString header;
            header = tableWidget_main_data_display->horizontalHeaderItem(i)->text();

            if(header == QString("RECON Examiner Notes") || header == QString("RECON Examiner Tags"))
            {
                ///to remain hide in display but make visible in reports
            }
            else if(tableWidget_main_data_display->isColumnHidden(i))
            {
                continue;
            }


            if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
            {
                if(bool_check_bookmark)
                {
                    QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(i ,2))->currentText());
                    if(recivedComboboxText != "Select Conversion")
                    {
                        if(recivedComboboxText != "File")
                        {
                            QString recivedColumnText =  tableWidget_column_selection->item(i,0)->text();
                            QString original_text = tableWidget_main_data_display->horizontalHeaderItem(i)->text();

                            line << "\"" + original_text.append(" (Original Value)") + "\"";
                            line << "\"" + recivedColumnText.append(" (Converted Value)") + "\"";
                        }
                        else
                        {
                            line << "\"" + tableWidget_main_data_display->horizontalHeaderItem(i)->text() + "\"";
                        }
                    }
                    else
                        line << "\"" + tableWidget_main_data_display->horizontalHeaderItem(i)->text() + "\"";
                }
            }
            else
            {
                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(i ,2))->currentText());
                if(recivedComboboxText != "Select Conversion")
                {
                    if(recivedComboboxText != "File")
                    {
                        QString recivedColumnText =  tableWidget_column_selection->item(i,0)->text();
                        QString original_text = tableWidget_main_data_display->horizontalHeaderItem(i)->text();

                        line << "\"" + original_text.append(" (Original Value)") + "\"";
                        line << "\"" + recivedColumnText.append(" (Converted Value)") + "\"";
                    }
                    else
                    {
                        line << "\"" + tableWidget_main_data_display->horizontalHeaderItem(i)->text() + "\"";
                    }
                }
                else
                    line << "\"" + tableWidget_main_data_display->horizontalHeaderItem(i)->text() + "\"";
            }
        }

        if(ui->checkBox_export->isChecked() && (bool_preview_coloum_enable == true))
        {
            line << "\"" + QObject::tr("New Path") + "\""
                 << "\"" + QObject::tr("Size") + "\""
                 << "\"" + QObject::tr("MD5") + "\""
                 << "\"" + QObject::tr("SHA1") +"\""
                 << "\"" + QObject::tr("Export Time") +"\"";

            outputfile_csv << line.join(",");
            outputfile_csv<< "\n";
            line.clear();
        }
        else
        {
            outputfile_csv << line.join( "," )+"\n";
            line.clear();
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


            QStringList tabledata;
            count++;

            QString num = QString::number(count);   //For Sr. No. Insertion in first coloum.
            tabledata << num;

            QStringList expo_info_str;
            QString coloumnData;
            for(int col = 0; col < tableWidget_main_data_display->columnCount(); col++)
            {
                if((col == 0) || (col == tableWidget_main_data_display->columnCount() -1))
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

                if(header == QString("RECON Examiner Notes"))
                {
                    tabledata << "";
                }
                else if(tableWidget_main_data_display->isColumnHidden(col))
                {
                    continue;
                }

                if(bool_preview_coloum_enable)
                {
                    if(col == 2)
                    {
                        expo_info_str.clear();
                        expo_info_str << get_export_detail_csv(tableWidget_main_data_display->item(row,col)->text(),struct_main_object.table_name);
                    }
                }

                QString data;
                data = tableWidget_main_data_display->item(row,col)->text();

                QString convertedData;

                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(col ,2))->currentText());
                if(recivedComboboxText != "Select Conversion")
                {
                    convertedData = convert_data(data,recivedComboboxText);

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

                    if(recivedComboboxText != "File")
                    {
                        tabledata << QString("\"") + data + QString("\"");
                        coloumnData += convertedData;
                    }
                    else
                        coloumnData += convertedData;
                }
                else
                {
                    coloumnData +=  data ;
                }

                tabledata << QString("\"") + coloumnData + QString("\"");
                coloumnData.clear(),convertedData.clear(),data.clear();
            }

            if(bool_preview_coloum_enable)
            {
                tabledata << QString("\"") + QString("") + QString("\"");

                outputfile_csv << tabledata.join( "," );
                outputfile_csv << expo_info_str.join(",") + "\n";
            }
            else
            {
                outputfile_csv << tabledata.join( "," ) + "\n";
            }
        }

        outputfile_csv << "\n";
        outputfile_csv << "\n";
    }

    file.close();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void sqlite_viewer::create_Screen_csv_report_for_query_execution(QString file_path)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        recon_static_functions::app_debug("-----FAILED---- to open file " + file_path,Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&file);
    QStringList line;
    outputfile_csv << "\n";
    outputfile_csv << QString(MACRO_Plugin_Name_Sqlite_Viewer) +"(" + QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB)  + ")" << "\n";
    outputfile_csv << "\n";

    global_recon_helper_singular_class_obj->write_csv_report_case_details_and_source_info(outputfile_csv,QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString), global_narad_muni_class_obj);

    outputfile_csv << "\n";

    outputfile_csv << (QObject::tr("File Path")) << "," << actual_file_path << "\n" ;
    outputfile_csv << "\n";

    outputfile_csv << (QObject::tr("Executed Query")) << "," << QString("\"") + ui->textEdit_query_insert->toPlainText().trimmed() + QString("\"") << "\n" ;
    outputfile_csv << "\n";

    line.clear();
    QStringList headers;
    headers = coloum_list_query_execution_table;
    headers.insert(0,"Sr. No.");

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

    //***********For Header insertion**********//
    for(int hh = 0; hh < headers.size(); hh++)
    {
        if(headers.at(hh).trimmed().isEmpty())
            continue;

        //line << "\"" + headers.at(hh) + "\"";

        QString header_str = headers.at(hh);
        bool bool_is_conversion_required = check_current_column_selected_for_conversion_in_manage_column(header_str);
        if(bool_is_conversion_required)
        {
            QString convertedData;
            QString recivedComboboxText;

            int col_index = get_current_column_selected_for_conversion_in_manage_column_index(header_str);
            recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col_index) ,2))->currentText());


            bool bool_is_conversion_required_for_file = check_current_column_selected_for_conversion_in_manage_column_for_file(header_str);

            if(bool_is_conversion_required_for_file)
            {
                line << "\"" + header_str + " (Original Value)" + "\"";
                line << "\"" + header_str + " (Converted Value)" + "\"";
            }
            else
            {
                line << "\"" + header_str + "\"";
            }
        }
        else
        {
            line << "\"" + header_str + "\"";
        }
    }

    outputfile_csv << line.join( "," )+"\n";
    line.clear();

    //***********For Data insertion in Table**********//

    int count = 0;
    for(int row = 0; row < ui->tableWidget_query_execution->rowCount(); row++)
    {
        QStringList tabledata;

        count++;
        QString num = QString::number(count);
        tabledata << num;
        for(int col= 0; col < ui->tableWidget_query_execution->columnCount(); col++)
        {
            if( ui->tableWidget_query_execution->item(row,col)->text().trimmed().isEmpty())
                continue;

            QString data_str = ui->tableWidget_query_execution->item(row,col)->text().toLocal8Bit();

            //tabledata << QString("\"") + data_str + QString("\"");

            QString header = ui->tableWidget_query_execution->horizontalHeaderItem(col)->text();

            bool bool_is_conversion_required = check_current_column_selected_for_conversion_in_manage_column(header);

            if(bool_is_conversion_required)
            {
                QString convertedData;
                QString recivedComboboxText;

                int col_index = get_current_column_selected_for_conversion_in_manage_column_index(header);
                recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col_index) ,2))->currentText());

                convertedData =  convert_data(data_str,recivedComboboxText);

                bool bool_is_conversion_required_for_file = check_current_column_selected_for_conversion_in_manage_column_for_file(header);


                if(bool_is_conversion_required_for_file)
                {
                    tabledata << QString("\"") + data_str + QString("\"");
                    tabledata << QString("\"") + convertedData + QString("\"");

                }
                else
                {
                    tabledata << QString("\"") + convertedData + QString("\"");
                }
            }
            else
            {
                tabledata << QString("\"") + data_str + QString("\"");
            }

        }


        outputfile_csv << tabledata.join(",") + "\n";
    }

    outputfile_csv << "\n";
    outputfile_csv << "\n";

    file.close();

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);

}
