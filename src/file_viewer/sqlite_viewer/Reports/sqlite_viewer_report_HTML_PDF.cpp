#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::sqlite_viewer_create_html_pdf_report(int report_type, QString file_path, int REPORT_CONTENT_FLAG)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    bool_is_data_available_for_export = false;

    if(ui->checkBox_export->isChecked())
    {
        sqlite_viewer_export_db_sanity_check(file_path,REPORT_CONTENT_FLAG);
    }

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        recon_static_functions::app_debug("-----FAILED---- to open file " + file_path,Q_FUNC_INFO);
        return;
    }

    file.write(sqlite_viewer_report_html_template::html_header1().toLocal8Bit().data());

    QString title =  QObject::tr("RECON_") + QString(MACRO_Plugin_Name_Sqlite_Viewer);
    file.write(sqlite_viewer_report_html_template::html_title(title).toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_header_after_title().toLocal8Bit().data());


    ///-SideBar
    file.write(sqlite_viewer_report_html_template::html_sidebar_start().toLocal8Bit().data());

    for(int combo = 0; combo < ui->comboBox_dbtables_selection->count(); combo++)
    {
        struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(combo);

        m_tablewidget *tableWidget_main_data_display = struct_main_object.tableWidget_MainDataDisplay_ptr;


        bool bool_check_bookmark = false;
        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
        {
            for(int row = 0; row < tableWidget_main_data_display->rowCount(); row++)
            {
                if(!tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState())
                    continue;
                else
                    bool_check_bookmark = true;
            }

            if(bool_check_bookmark)
            {
                QString table_name = ui->comboBox_dbtables_selection->itemText(combo);
                file.write(sqlite_viewer_report_html_template::html_sidebar_link(table_name).toLocal8Bit().data());
            }
        }


        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
        {
            if(ui->comboBox_dbtables_selection->itemText(combo) != current_tab_name)
                continue;

            QString table_name = ui->comboBox_dbtables_selection->itemText(combo);
            file.write(sqlite_viewer_report_html_template::html_sidebar_link(table_name).toLocal8Bit().data());
        }
        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_FULL_REPORT_int)
        {
            QString table_name = ui->comboBox_dbtables_selection->itemText(combo);
            file.write(sqlite_viewer_report_html_template::html_sidebar_link(table_name).toLocal8Bit().data());
        }
    }

    file.write(sqlite_viewer_report_html_template::html_sidebar_end().toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_report_main_title(agency_logo_path,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString()).toLocal8Bit().data());

    QString report_type_str;
    if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_FULL_REPORT_int)
    {
        report_type_str = MACRO_REPORT_SCOPE_FULL_QString;
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
    {
        report_type_str = MACRO_REPORT_SCOPE_TAGS_QString;
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
    {
        report_type_str = MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString;
    }


    file.write(global_recon_helper_singular_class_obj->write_html_case_info_and_source_info(report_type_str, global_narad_muni_class_obj).toLocal8Bit().data());

    QString icon_path = QString("./resources/sqlite_viewer.png");

    if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_FULL_REPORT_int)
    {
        file.write(sqlite_viewer_report_html_template::html_module_header(icon_path,QString(MACRO_REPORT_SCOPE_FULL_QString), QString(MACRO_Plugin_Name_Sqlite_Viewer)).toLocal8Bit().data());
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
    {
        file.write(sqlite_viewer_report_html_template::html_module_header(icon_path,QString(MACRO_REPORT_SCOPE_TAGS_QString), QString(MACRO_Plugin_Name_Sqlite_Viewer)).toLocal8Bit().data());
    }
    else if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
    {
        file.write(sqlite_viewer_report_html_template::html_module_header(icon_path,QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString), QString(MACRO_Plugin_Name_Sqlite_Viewer)).toLocal8Bit().data());
    }

    file.write("<br>");
    file.write("<br>");


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

        bool bool_preview_coloum_enable = false;

        if(ui->checkBox_export->checkState())
        {
            for(int k = 0; k < tableWidget_filter_view_search->rowCount(); k++)
            {
                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(k ,2))->currentText());

                if(recivedComboboxText == "File")
                {
                    bool_preview_coloum_enable = true;
                }

            }
        }


        bool bool_check_bookmark = false;
        if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
        {
            for(int row = 0; row < tableWidget_main_data_display->rowCount(); row++)
            {
                if(tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Unchecked)
                    continue;
                else
                    bool_check_bookmark = true;
            }

            if(bool_check_bookmark)
            {
                QString table_name = struct_main_object.table_name;
                file.write(sqlite_viewer_report_html_template::html_sub_module_header(table_name).toLocal8Bit().data());

                file.write(sqlite_viewer_report_html_template::html_top_link().toLocal8Bit().data());
                file.write(sqlite_viewer_report_html_template::html_table_start().toLocal8Bit().data());

                QStringList headers2;
                //                if(ui->checkBox_export->isChecked())
                if(bool_preview_coloum_enable)
                {
                    headers2 << "Sr No." << "Table Details"<< "File Preview" ;
                    file.write(sqlite_viewer_report_html_template::html_table_headers(headers2,3).toLocal8Bit().data());
                }
                else
                {
                    headers2 << "Sr No." << "Table Details";
                    file.write(sqlite_viewer_report_html_template::html_table_headers(headers2,2).toLocal8Bit().data());
                }
            }
        }
        else
        {
            QString table_name = struct_main_object.table_name;
            file.write(sqlite_viewer_report_html_template::html_sub_module_header(table_name).toLocal8Bit().data());

            if(tableWidget_main_data_display->rowCount() != 0)
                file.write(sqlite_viewer_report_html_template::html_top_link().toLocal8Bit().data());
            file.write(sqlite_viewer_report_html_template::html_table_start().toLocal8Bit().data());


            if(tableWidget_main_data_display->rowCount() == 0)
            {
                file.write(sqlite_viewer_report_html_template::html_table_end().toLocal8Bit().data());

                QString artifacts_str = "<b><font color=\"red\">" +  QObject::tr("No Data Available") + "</font></b>";
                file.write(artifacts_str.toLocal8Bit());

                file.write("<br>");
                file.write("<br>");
                file.write("<br>");
                continue;
            }

            //-----my code ends -------//

            ///-Coloum List for the headers.
            ///- For Coloums greater than 6.
            QStringList headers2;
            if(bool_preview_coloum_enable)
            {
                headers2 << "Sr No." << "Table Details" << "File Preview";
                file.write(sqlite_viewer_report_html_template::html_table_headers(headers2,3).toLocal8Bit().data());
            }
            else
            {
                headers2 << "Sr No." << "Table Details";
                file.write(sqlite_viewer_report_html_template::html_table_headers(headers2,2).toLocal8Bit().data());
            }
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

            QString coloumnData;
            QString preview_data;

            sqlite_viewer_st_export_file_detail obj;
            for(int col = 0; col < tableWidget_main_data_display->columnCount(); col++)
            {
                if((col == 0) || (col == tableWidget_main_data_display->columnCount() - 1))
                    continue;

                if(col == (tableWidget_main_data_display->columnCount() - 1) || col ==  1){
                    continue;
                }

                if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
                {
                    if(!(tableWidget_main_data_display->columnCount() -1))
                    {
                        if(tableWidget_main_data_display->isColumnHidden(col))
                            continue;
                    }
                }


                //---To avoid hidden columns to go in report; continued here ----//
                QString headery;
                headery = tableWidget_main_data_display->horizontalHeaderItem(col)->text();

                if(headery == QString("RECON Examiner Tags") || headery == QString("RECON Examiner Notes"))
                {
                    ///to remain hide in display but make visible in reports
                }
                else if(tableWidget_main_data_display->isColumnHidden(col))
                {
                    continue;
                }

                if(bool_preview_coloum_enable)
                {
                    if(col ==1)
                    {
                        obj.export_filedetail.clear();
                        obj.export_flepreview.clear();

                        obj = get_export_detail_html_final(tableWidget_main_data_display->item(row,col)->text(),struct_main_object.table_name,QString("1"));
                    }
                }


                QString header;
                header = tableWidget_main_data_display->horizontalHeaderItem(col)->text();

                QString data;
                data = tableWidget_main_data_display->item(row, col)->text();

                QString convertedData;

                QString recived_combobox_text =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(col, 2))->currentText());

                if(recived_combobox_text != "Select Conversion")
                {
                    convertedData =  convert_data(data,recived_combobox_text);
                    if(convertedData.contains("<b>File Exist: </b>	<font  color=\"green\"> <b>YES </b></font><br>"))
                    {
                        QFileInfo info(data);
                        preview_data.clear();

                        QString icon_path = "./resources/";

                        QString file_name = info.fileName();
                        QString new_relativepath = "./file/" + file_name;      //---- changed here file = tmp

                        QString image,hyperlink;

                        if(recon_helper_standard_obj->check_is_it_image_file(info.filePath(),"", Q_FUNC_INFO))
                        {
                            image = QString("<img src=") + "\"" + new_relativepath + "\"" + " alt=\"No Preview\"" + " width=\"140\" height=\"150\">";
                            hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                        }
                        else if(recon_helper_standard_obj->check_is_audio_by_extn(file_name, Q_FUNC_INFO) || recon_helper_standard_obj->check_is_video_by_extn(file_name, Q_FUNC_INFO))
                        {
                            image = QString("<img src=") + "\"" + icon_path + "open_file_link.png" + "\"" + " alt=\"No Preview\"" + " width=\"140\" height=\"150\">";
                            hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                        }
                        else
                        {
                            image = QString("<img src=") + "\"" + icon_path + "open_file_link.png" + "\"" + " alt=\"No Preview\"" + " width=\"140\" height=\"150\">";
                            hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                        }

                        preview_data = hyperlink + image + "</a>";
                    }

                    if(recived_combobox_text != "File")
                    {
                        coloumnData += QString("<b>" + header + QString(" (Original Value)") + ":</b>" +QString("\t")+ data +"<br>");  //----28 dec
                        coloumnData += QString("<b>" + header + QString(" (Converted Value)") + ":</b>" +QString("\t")+ convertedData +"<br>");
                    }
                    else
                    {
                        coloumnData += QString("<b>" + header + ":</b>" +QString("\t")+ convertedData +"<br>");
                    }
                }
                else
                {
                    if(headery == QString("RECON Examiner Tags"))
                    {
                        QString tag_text = "";
                        QStringList list_tags = data.split(",", Qt::SkipEmptyParts);
                        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                        {
                            if(list_tags.at(tag_i).isEmpty())
                                continue;

                            QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                            tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                        }

                        if(tag_text.endsWith(","))
                            tag_text.chop(1);

                        coloumnData += QString("<br><b>RECON Examiner Tag:</b>\t") + tag_text + "<br>";
                        continue;
                    }

                    if(headery == QString("RECON Examiner Notes"))
                    {
                        coloumnData += QString("<br><b>" + header + ":</b>\t<font color = \"blue\">" +QString("\t") + data +"<br>" + "</font>");
                        continue;
                    }

                    coloumnData += QString("<b>" + header + ":</b>" + QString("\t")+ data +"<br>");
                }
            }

            if(bool_preview_coloum_enable)
            {
                coloumnData.append(obj.export_filedetail);
                tabledata << coloumnData << preview_data;
            }
            else
            {
                tabledata << coloumnData ;
            }

            coloumnData.clear();

            if(bool_for_the_unicode_conversion)
            {
                QByteArray data_arr = sqlite_viewer_report_html_template::html_table_row_data(tabledata,2).toLocal8Bit();
                file.write(data_arr);
            }
            else
            {
                file.write(sqlite_viewer_report_html_template::html_table_row_data(tabledata,2).toLocal8Bit().data());
            }
        }

        file.write(sqlite_viewer_report_html_template::html_table_end().toLocal8Bit().data());

        file.write("<br>");
        file.write("<br>");
        file.write("<br>");
    }


    switch(report_type)
    {
    case 0:
        file.close();
        recon_static_functions::app_debug("HTML Generated",Q_FUNC_INFO);
        break;
    case 1:
        QStringList args;
        args << file.fileName();

        report_file_path.clear();
        file_path.chop(4); //html
        file_path.append("pdf");
        report_file_path = file_path;
        emit signal_create_pdf_report(file.fileName(), file_path);


        recon_static_functions::app_debug("PDF Generated",Q_FUNC_INFO);
        break;
    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

}

void sqlite_viewer::slot_html_to_pdf_finished(QString pdf_file_path, bool)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    sqlite_viewer_report_status report_s_obj;
    if(report_s_obj.exec())
    {
        QStringList arg;
        arg << pdf_file_path;

        QProcess myProcess ;
        myProcess.startDetached("open",arg);
    }

    recon_static_functions::app_debug("end",Q_FUNC_INFO);

}


void sqlite_viewer::export_files(QString report_path, int REPORT_CONTENT_FLAG)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);


    QString export_dir_path = report_path;
    export_dir_path.remove("/index.html");
    export_dir_path.remove("/index.csv");
    export_dir_path.remove("/index.xml");
    export_dir_path = export_dir_path + "/files/";

    QDir dir;
    if(!dir.mkdir(export_dir_path))
    {
        recon_static_functions::app_debug("Dir making -----FAILED---- " + export_dir_path,Q_FUNC_INFO);
        return;
    }


    for(int i = 0; i < ui->comboBox_dbtables_selection->count(); i++)
    {

        struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(i);

        m_tablewidget *tableWidget_MainDataDisplay = struct_main_object.tableWidget_MainDataDisplay_ptr;

        QTableWidget *tableWidgetColumnSelection = struct_main_object.tableWidget_columns_selection_ptr;

        for(int row= 0; row < tableWidget_MainDataDisplay->rowCount(); row++)
        {
            if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
            {
                if(tableWidget_MainDataDisplay->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Unchecked)
                    continue;
            }

            if(REPORT_CONTENT_FLAG == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
            {
                if(tableWidget_MainDataDisplay->isRowHidden(row))
                    continue;
            }

            for(int col = 0; col < tableWidget_MainDataDisplay->columnCount(); col++)
            {
                if(col == 0)
                    continue;

                QString source_path;
                source_path = tableWidget_MainDataDisplay->item(row,col)->text();

                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidgetColumnSelection->cellWidget((col) ,2))->currentText());
                //                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidgetFilterViewSearch->cellWidget((col) ,2))->currentText());

                if(recivedComboboxText != "Select Conversion")
                {
                    if(recivedComboboxText == "File")
                    {
                        QFileInfo info(source_path);
                        if(info.exists())
                        {
                            //-CopyFile or directory to the Folder
                            QString dest_file_path = export_dir_path + info.fileName();

                            if(info.isDir())
                            {

                                recon_static_functions::copy_directory(source_path,dest_file_path, Q_FUNC_INFO);
                                //                                check_is_dir = true;
                            }

                            if(info.isFile())
                            {

                                QFile source_file(source_path);
                                QFile dest_file(dest_file_path);
                                if(source_file.exists())
                                {
                                    if(!source_file.open(QIODevice::ReadOnly))
                                    {
                                        recon_static_functions::app_debug("source_file.open  ----FAILED--- " + source_path,Q_FUNC_INFO);
                                        recon_static_functions::app_debug(source_file.errorString(),Q_FUNC_INFO);
                                    }
                                    if(!dest_file.open(QIODevice::WriteOnly))
                                    {
                                        recon_static_functions::app_debug("dest_file.open  ----FAILED--- " + dest_file_path,Q_FUNC_INFO);
                                        recon_static_functions::app_debug(dest_file.errorString(),Q_FUNC_INFO);
                                    }

                                    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
                                    while(1)
                                    {
                                        QByteArray dar = source_file.read(102400);
                                        if(dar.size() <= 0)
                                        {
                                            source_file.close();
                                            dest_file.close();
                                            break;
                                        }

                                        dest_file.write(dar);
                                    }
                                    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                                }
                            }
                        }
                    }
                }
            }
        }
    }

    recon_static_functions::app_debug("- END",Q_FUNC_INFO);

}

void sqlite_viewer::create_screen_html_pdf_report_for_query_execution(int report_type, QString file_path)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        recon_static_functions::app_debug("-----FAILED---- to open file " + file_path,Q_FUNC_INFO);
        return;
    }

    file.write(sqlite_viewer_report_html_template::html_header1().toLocal8Bit().data());
    QString sqlite_plugin_name =  QString(MACRO_Plugin_Name_Sqlite_Viewer);
    QString title = sqlite_plugin_name.replace(" ","_") + "_" + QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB);

    file.write(sqlite_viewer_report_html_template::html_title(title).toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_header_after_title().toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_sidebar_start().toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_sidebar_link(QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB)).toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_sidebar_end().toLocal8Bit().data());
    file.write(sqlite_viewer_report_html_template::html_report_main_title(agency_logo_path,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString()).toLocal8Bit().data());

    QString report_type_str;
    if(report_type == MACRO_SQLITE_VIEWER_FULL_REPORT_int)
    {
        report_type_str = MACRO_REPORT_SCOPE_FULL_QString;
    }
    else if(report_type == MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int)
    {
        report_type_str = MACRO_REPORT_SCOPE_TAGS_QString;
    }
    else if(report_type == MACRO_SQLITE_VIEWER_SCREEN_REPORT_int)
    {
        report_type_str = MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString;
    }

    file.write(global_recon_helper_singular_class_obj->write_html_case_info_and_source_info(report_type_str, global_narad_muni_class_obj).toLocal8Bit().data());

    QString icon_path = QString("./resources/") + sqlite_plugin_name.replace(" ","_") + ".png";

    file.write(sqlite_viewer_report_html_template::html_module_header(icon_path,QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB), QString(MACRO_Plugin_Name_Sqlite_Viewer)).toLocal8Bit().data());
    file.write("<br>");
    file.write("<br>");

    file.write(sqlite_viewer_report_html_template::html_sub_module_header(QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB)).toLocal8Bit().data());

    //------//

    QString source_file_path = "<b>" + QObject::tr("File Path") + ":  </b>  "+ actual_file_path +" <br>";
    file.write(source_file_path.toLocal8Bit());
    file.write("<br>");
    file.write("<br>");

    QString exec_str = "<b>" + QObject::tr("Executed Query") + ": </b><br>";
    file.write(exec_str.toLocal8Bit());

    QString query_exec = ui->textEdit_query_insert->toPlainText();
    file.write(query_exec.toLocal8Bit());
    file.write("<br>");

    //----------//


    QStringList headers;

    ///************ For tables coloumns lessthan than 6 ************//

    if(ui->tableWidget_query_execution->columnCount() <= 6)
    {

        struct_global_sqlite_viewer_tablewidget_main obj_main;
        if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        {
            obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
        }
        else
        {
            recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
            return;
        }

        m_tablewidget *tablewidget_main_data_display;
        tablewidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

        QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

        //***********For Header insertion**********//

        headers = coloum_list_query_execution_table;
        headers.insert(0,"Sr. No.");

        file.write(sqlite_viewer_report_html_template::html_top_link().toLocal8Bit().data());
        file.write(sqlite_viewer_report_html_template::html_table_start().toLocal8Bit().data());
        file.write(sqlite_viewer_report_html_template::html_table_headers(headers,1).toLocal8Bit().data());

        //***********For Data insertion in Table**********//

        int count = 0;

        for(int row = 0; row < ui->tableWidget_query_execution->rowCount(); row++)
        {

            QStringList tabledata;

            count++;
            QString num = QString::number(count);
            tabledata << num;

            QString all_date_details;
            for(int col= 0; col < ui->tableWidget_query_execution->columnCount(); col++)
            {
                if(coloum_list_query_execution_table.at(col).trimmed().isEmpty())
                    continue;

                QString data_str = ui->tableWidget_query_execution->item(row,col)->text();
                QString header = coloum_list_query_execution_table.at(col);


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
                        all_date_details += QString("<b>" + header + " (Original Value)"  + ": </b>" +"\t"+ data_str + "<br>");
                        all_date_details += QString("<b>" + header + " (Converted Value)"  +": </b>" +"\t"+ convertedData + "<br>");
                    }
                    else
                    {
                        all_date_details += QString("<b>" + header +": </b>" +"\t"+ convertedData + "<br>");
                    }
                }
                else
                {
                    all_date_details += QString("<b>" + header +": </b>" +"\t"+ data_str + "<br>");
                }

                tabledata << all_date_details;

            }
            file.write(sqlite_viewer_report_html_template::html_table_row_data(tabledata,1).toLocal8Bit().data());


            //            for(int col= 0; col < ui->tableWidget_query_execution->columnCount(); col++)
            //            {
            //                tabledata << ui->tableWidget_query_execution->item(row,col)->text();
            //            }

            //            file.write(sqlite_viewer_report_html_template::html_table_row_data(tabledata,1).toLocal8Bit().data());
        }


    }


    ///*********** For Data in 2 Coloums Sr. No. And Data**************//
    if(ui->tableWidget_query_execution->columnCount() > 6)
    {

        struct_global_sqlite_viewer_tablewidget_main obj_main;
        if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        {
            obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
        }
        else
        {
            recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
            return;
        }

        m_tablewidget *tablewidget_main_data_display;
        tablewidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

        QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

        headers << "Sr No." << "Table Details";
        file.write(sqlite_viewer_report_html_template::html_top_link().toLocal8Bit().data());
        file.write(sqlite_viewer_report_html_template::html_table_start().toLocal8Bit().data());
        file.write(sqlite_viewer_report_html_template::html_table_headers(headers,2).toLocal8Bit().data());


        int count = 0;
        for(int row = 0; row < ui->tableWidget_query_execution->rowCount(); row++)
        {

            QStringList tabledata;

            count++;
            if(headers.at(0) == "Sr No.")
            {
                QString num = QString::number(count);
                tabledata << num;
            }

            //*******Insertion of all coloum data in second coloum with there headers*********//

            if(headers.at(1) == "Table Details")
            {
                QString all_date_details;
                for(int col= 0; col < ui->tableWidget_query_execution->columnCount(); col++)
                {
                    if(coloum_list_query_execution_table.at(col).trimmed().isEmpty())
                        continue;

                    QString data_str = ui->tableWidget_query_execution->item(row,col)->text();
                    QString header = coloum_list_query_execution_table.at(col);


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
                            all_date_details += QString("<b>" + header + " (Original Value)"  + ": </b>" +"\t"+ data_str + "<br>");
                            all_date_details += QString("<b>" + header + " (Converted Value)"  +": </b>" +"\t"+ convertedData + "<br>");
                        }
                        else
                        {
                            all_date_details += QString("<b>" + header +": </b>" +"\t"+ convertedData + "<br>");
                        }
                    }
                    else
                    {
                        all_date_details += QString("<b>" + header +": </b>" +"\t"+ data_str + "<br>");
                    }


                }

                tabledata << all_date_details;
            }

            if(bool_for_the_unicode_conversion)
            {
                QByteArray data_arr = sqlite_viewer_report_html_template::html_table_row_data(tabledata,2).toLocal8Bit();
                file.write(data_arr);
            }
            else
            {
                file.write(sqlite_viewer_report_html_template::html_table_row_data(tabledata,2).toLocal8Bit().data());
            }
        }





    }

    file.write(sqlite_viewer_report_html_template::html_table_end().toLocal8Bit().data());

    file.close();


    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}
