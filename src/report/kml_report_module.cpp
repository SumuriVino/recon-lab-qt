#include "kml_report_module.h"

kml_report_module::kml_report_module(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    report_generic_function_obj = new report_generic_function(this);
    export_file_obj = new export_file_module(this);
}

void kml_report_module::set_essentials()
{


    clear_fields();

    fill_variable_from_narad_muni();

    report_generic_function_obj->pub_set_essentials();
    export_file_obj->set_essentials();

}
void kml_report_module::set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
    report_generic_function_obj->pub_set_narad_muni_for_report(mobj);
    export_file_obj->set_narad_muni_for_report(mobj);

}

void kml_report_module::clear_exportfilelist_table()
{
    export_file_obj->set_export_entry_null();
}

void kml_report_module::fill_variable_from_narad_muni()
{
    table_export_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList).toStringList();
    table_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList).toStringList();
    table_export_criteria_list_tablewidget   = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList).toStringList();

    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();
    module_name_str  = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();

    table_kml_report_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_KML_Report_Available_Tablewidget_QStringList).toStringList();
    tab_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList).toStringList();

    list_m_tablewidgets.clear();
    QList<QVariant> screen_tablewidget_list_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList).toList();
    for(int i=0; i < screen_tablewidget_list_list.size(); i++)
    {
        qint64 nn = screen_tablewidget_list_list.at(i).toLongLong();
        list_m_tablewidgets.append((m_tablewidget *) nn);
    }


    rt_kml_col_name_list.clear();
    QList<QVariant> kml_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_KML_Col_Name_QList).toList();
    for(int i=0; i < kml_col_name_list.size(); i++)
        rt_kml_col_name_list.append(kml_col_name_list.at(i).toStringList());

    rt_kml_data_type_list.clear();
    QList<QVariant> kml_data_type_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_KML_Data_Type_QList).toList();
    for(int i=0; i < kml_data_type_list.size(); i++)
        rt_kml_data_type_list.append(kml_data_type_list.at(i).toStringList());

    rt_kml_d_name_list.clear();
    QList<QVariant> kml_d_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_KML_Display_Name_QList).toList();
    for(int i=0; i < kml_d_name_list.size(); i++)
        rt_kml_d_name_list.append(kml_d_name_list.at(i).toStringList());


    rt_kml_order_list.clear();
    QList<QVariant> kml_order_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_KML_Order_QList).toList();
    for(int i=0; i < kml_order_list.size(); i++)
        rt_kml_order_list.append(kml_order_list.at(i).toStringList());



    tw_data_type_list_tablewidget.clear();
    QList<QVariant> data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList).toList();
    for(int i=0; i < data_type_tablewidget_list.size(); i++)
        tw_data_type_list_tablewidget.append(data_type_tablewidget_list.at(i).toStringList());

}


void kml_report_module::clear_fields()
{
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_KML_Filedetail_QStringList , QStringList());

}

void kml_report_module::create_full_report_kml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts", Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open -----FAILED---" + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" error" + file.errorString() , Q_FUNC_INFO);
        return;
    }

    QString firstline = QString("<?xml version=") + "\"" + "1.0" + "\"" + " encoding=" + "\""+ "UTF-8" +"\"" + "?>";
    QString secondline = QString("<kml xmlns=")+ "\""+ "http://www.opengis.net/kml/2.2"+ "\"" + ">";
    QString new_line = "\n";

    file.write(firstline.toLocal8Bit());
    file.write(new_line.toLocal8Bit());
    file.write(secondline.toLocal8Bit());
    file.write(new_line.toLocal8Bit());

    QString buffer;
    QXmlStreamWriter stream_kml(&buffer);
    stream_kml.setAutoFormatting(true);
    stream_kml.writeStartElement("Document"); //Document Start



    //=============== is data available start ==================//
    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list))
    {
        stream_kml.writeStartElement("Placemark");
        stream_kml.writeTextElement("name",get_plugin_name());
        stream_kml.writeTextElement("description", "No Data Available");
        stream_kml.writeEndElement();
        stream_kml.writeEndElement();
        file.write(buffer.toLocal8Bit());
        file.write("</kml>");
        file.flush();
        file.close();
        return;
    }
    //=============== is data available end ==================//



    //=============== export start ==================//
    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);
        if(tabs_export_status_list.at(pp) == "1")
            export_file_obj->export_file(MACRO_REPORT_SCOPE_FULL_QString,tabname,selected_tags_list);
    }
    //=============== export end ==================//


    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        if(!report_generic_function_obj->pub_is_data_available_in_tab(QStringList(tabname), MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list))
            continue;

        bool bool_exp = false;
        if(tabs_export_status_list.at(pp) == "1")
            bool_exp = true;


        create_tablewidget_kml_report(&stream_kml, tabname,MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list,bool_exp);

    }


    stream_kml.writeEndElement();
    file.write(buffer.toLocal8Bit());
    file.write("</kml>");
    file.flush();
    file.close();


    recon_static_functions::app_debug(" -Ends" , Q_FUNC_INFO);

}


void kml_report_module::create_screen_report_kml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    if((selected_tablist.size() < 1) || (tabs_export_status_list.size() < 1))
        return;

    int i_tab = -1;
    QString tab_nam = selected_tablist.at(0);
    for(int pp = 0 ; pp < table_name_list_tablewidget.size() ; pp++)
    {
        if(tab_nam == tab_name_list_tablewidget.at(pp))
        {
            i_tab = pp;
            break;
        }
    }

    if(i_tab == -1)
        return;

    if(list_m_tablewidgets.size() <= i_tab)
        return;


    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file opened ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QString firstline = QString("<?xml version=") + "\"" + "1.0" + "\"" + " encoding=" + "\""+ "UTF-8" +"\"" + "?>";
    QString secondline = QString("<kml xmlns=")+ "\""+ "http://www.opengis.net/kml/2.2"+ "\"" + ">";
    QString new_line = "\n";

    file.write(firstline.toLocal8Bit());
    file.write(new_line.toLocal8Bit());
    file.write(secondline.toLocal8Bit());
    file.write(new_line.toLocal8Bit());

    QString buffer;
    QXmlStreamWriter stream_kml(&buffer);
    stream_kml.setAutoFormatting(true);
    stream_kml.writeStartElement("Document"); //Document Start

    //=============== is data available start ==================//
    bool bool_data_available = false;
    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        bool_data_available = true;
    }
    if(!bool_data_available)
    {
        stream_kml.writeStartElement("Placemark");
        stream_kml.writeTextElement("name",get_plugin_name());
        stream_kml.writeTextElement("description", "No Data Available");
        stream_kml.writeEndElement();
        stream_kml.writeEndElement();
        file.write(buffer.toLocal8Bit());
        file.write("</kml>");
        file.flush();
        file.close();
        return;
    }
    //=============== is data available end ==================//

    //=============== export start ==================//
    bool tab_export_checked = false;
    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(get_plugin_name(), tab_nam);
    if(tabs_export_status_list.at(0) == "1")
        tab_export_checked = true;

    if(tab_export_checked)
    {
        export_file_obj->export_file(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString,tab_nam,QStringList());
    }
    //=============== export end ==================//

    QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
    QStringList cols_list = rt_kml_col_name_list.at(i_tab);
    QStringList col_type_list = tw_data_type_list_tablewidget.at(i_tab);
    QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
    bool bool_multiple_export = false;
    if(is_multiple_export == "2")
        bool_multiple_export = true;

    int INT_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);


    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
        {
            QString INT_str = list_m_tablewidgets.at(i_tab)->item(i, INT_index)->text();
            QString command = "select " + cols_list.join(",") + ", source_count_name from '" + table_name  + "' where INT = " + INT_str ;
            set_table_data_kml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export, &stream_kml ,destination_db_file_list.at(db_count));
        }
    }

    stream_kml.writeEndElement();
    file.write(buffer.toLocal8Bit());
    file.write("</kml>");
    file.flush();
    file.close();


    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);

}

void kml_report_module::set_table_data_kml(int i_tab, QString command, QString table_name,  bool tab_export_checked,QString is_export_available,bool bool_multiple_export, QXmlStreamWriter *stream_kml, QString destination_db_file)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" db open ---FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error -" + dest_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select(dest_db);

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" ---FAILED--- query  " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -" + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("command -" + command , Q_FUNC_INFO);
        dest_db.close();
        return;
    }

    QString tab_name = tab_name_list_tablewidget.at(i_tab);
    QStringList cols_list = rt_kml_col_name_list.at(i_tab);
    QStringList data_type_list = rt_kml_data_type_list.at(i_tab);
    QStringList col_d_name_list = rt_kml_d_name_list.at(i_tab);
    int cols_size = cols_list.size();

    while(query_select.next())
    {
        QString exported_file_detail;

        if((is_export_available == QString("1")) && (tab_export_checked == true))
        {
            int pos = data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString);
            if(pos != -1)
            {
                get_export_detail_kml(dest_db,query_select.value(pos).toString(),table_name);

                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_KML_Filedetail_QStringList).toStringList().size() > 0)
                    exported_file_detail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_KML_Filedetail_QStringList).toStringList().at(0);
            }
        }

        QString latitude,longitude,altitude,image_name;

        QString final_data;
        QString record_number;

        for(int  i = 0 ; i < cols_size ; i++)
        {
            QString temp_data;
            int ref_index = i;
            QString qvalue;
            QString dvalue;

            dvalue = QString("&lt;b &gt;") + col_d_name_list.at(ref_index) + ": &lt;/b &gt;";

            QString rpt_data_type = data_type_list.at(ref_index);
            QString rpt_data_type_for_tmpuse = rpt_data_type;

            QString our_column = col_d_name_list.at(ref_index);
            QString our_value = query_select.value(ref_index).toString();

            bool colmn_atr =  report_generic_function_obj->pub_is_column_of_apple_attribute(our_column);

            // hide extended attribute column which has no value
            if(colmn_atr)
            {
                if(our_value.trimmed() == QString(""))
                    continue;
            }

            // if want to hide those item which are blank
            if(rpt_data_type.endsWith("Hide"))
            {
                rpt_data_type.remove("Hide");
                if(our_value.trimmed() == "")
                    continue;
            }


            ///Html Report*/
            if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
            {
                record_number = our_value;
            }


            if(rpt_data_type.startsWith("NewLine"))
            {
                rpt_data_type.remove("NewLine");
            }

            if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString)
            {
                QString extended_attr = our_value;
                if(extended_attr.trimmed() == QString(""))
                    continue;

                extended_attr = extended_attr.trimmed();
                extended_attr.replace("\n   ","&lt;br /&gt;&nbsp;");
                extended_attr.replace("\n","&lt;br /&gt;&nbsp;");
                if(extended_attr != "")
                {   dvalue = "";
                    qvalue = QString("&lt;b &gt;") + QObject::tr("Bookmarked Apple Metadata") + ":&lt;/b &gt;" + extended_attr;
                }
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
            {
                qvalue = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_IMAGE_NAME_KML_QString)
            {
                our_value.replace("&","&#38;");
                image_name = (our_value);
                qvalue = our_value;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_LATITUDE_QString)
            {
                latitude = (our_value);
                qvalue = our_value;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_LONGITUDE_QString)
            {
                longitude = (our_value);
                qvalue = our_value;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ALTITUDE_QString)
            {
                altitude = (our_value);
                qvalue = our_value;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                QString value =  our_value;
                if(value == QString(""))
                    qvalue = "";
                else if(value == QString("0"))
                    qvalue =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                else
                    qvalue = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
            {
                QString temp_2 = our_value;
                if(temp_2 != QString(""))
                {
                    QStringList temp;
                    if(our_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        temp = our_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                    }
                    else
                    {
                        temp = our_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                    }

                    if(temp.size() >= 1)
                    {
                        dvalue = (QString("&lt;b&gt; &lt;u&gt;" + our_column + "&lt;/u&gt; &lt;/b&gt;"));
                        for(int k = 0 ; k< temp.size(); k++)
                        {
                            qvalue.append("&lt;br&gt;");
                            qvalue.append("&lt;b&gt;"+  QObject::tr("Date ") + ": &lt;/b&gt;" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                        }
                    }
                }
                else
                {
                    continue;
                }
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
            {
                QString str = our_value;

                if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"&lt;br /&gt;");
                }
                else if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"&lt;br /&gt;");
                }
                str = str.replace("|","&lt;br /&gt;");

                qvalue = str;

                qvalue.prepend("&lt;br /&gt;");
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
            {
                qvalue = qvalue.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"&lt;br /&gt;");

                qvalue = our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"&lt;br /&gt;");

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {
                qvalue  = "<font color = \"blue\">" + query_select.value(i).toString().replace("\n", "&lt;br /&gt;") + "</font>";

                if(is_export_available == QString::number(1))
                {
                    QString extended_list = recon_helper_standard_obj->get_apple_metadata_key_value_from_result(record_number, table_name, destination_db_file, Q_FUNC_INFO);

                    if(!extended_list.isEmpty())
                    {
                        qvalue += "<br><br><b><u><i>More Apple Metadata</u></i></b>"+ extended_list;
                    }
                }
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
            {
                QString tag_value = our_value;

                QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_value, narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                QString tag_text = "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + tag_value + "</font>";
                qvalue = tag_text;
            }
            else
                qvalue = our_value.toHtmlEscaped();

            if(rpt_data_type_for_tmpuse.startsWith("NewLine"))
            {
                temp_data.append("&lt;br /&gt;");
            }

            temp_data.append(dvalue + qvalue + "&lt;br /&gt;") ;

            final_data += temp_data;

        } // for loop col size


        QString full_cordinates;

        if(altitude != QString(""))
            full_cordinates = longitude + "," + latitude +  "," + altitude + ",0";
        else
            full_cordinates = longitude + "," + latitude + ",0";


        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());
        QString m_plgn_info = QString("&lt;b &gt;") + "Source Name" + ": &lt;/b &gt;";
        m_plgn_info += struct_info.complete_source_name + "&lt;br /&gt;";


        m_plgn_info += QString("&lt;b &gt;") + "Plugin" + ": &lt;/b &gt;";


        m_plgn_info += get_plugin_name() + "&lt;br /&gt;";
        m_plgn_info += QString("&lt;b &gt;") + "Category" + ": &lt;/b &gt;";
        m_plgn_info += tab_name + "&lt;br /&gt;" + "&lt;br /&gt;";
        final_data.prepend(m_plgn_info);

        final_data += exported_file_detail;


        stream_kml->writeStartElement("Placemark");
        stream_kml->writeTextElement("name",image_name);
        //stream_kml->writeTextElement("description", final_data);
        stream_kml->writeStartElement("description");
        stream_kml->writeDTD(final_data);
        stream_kml->writeEndElement(); //description end
        stream_kml->writeStartElement("Point");
        stream_kml->writeTextElement("coordinates",full_cordinates);
        stream_kml->writeEndElement(); //point end
        stream_kml->writeEndElement(); //placemark end

    }  // while loop

    dest_db.close();

    recon_static_functions::app_debug(" -Ends"  , Q_FUNC_INFO);

    return;
}


void kml_report_module::get_export_detail_kml(QSqlDatabase dest_db, QString column, QString tablename)
{

    QSqlQuery query_export_detail(dest_db);

    QString cmnd = QString("select exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time,filepath from exportfilelist where (table_name = '") + tablename + "' AND INT = " + column + ")";

    if(!query_export_detail.exec(cmnd))
    {
        recon_static_functions::app_debug(" query ---FAILED--- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + query_export_detail.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("command -" + cmnd , Q_FUNC_INFO);
        return;
    }

    QStringList exp_detail_list;
    while(query_export_detail.next())
    {
        QString exp_detail;

        QString relative_path = query_export_detail.value(0).toString();
        relative_path = relative_path.trimmed();

        QString org_filepath = query_export_detail.value(5).toString();
        QFileInfo info8(org_filepath);
        QString original_filename = info8.fileName();

        if(relative_path == QString(""))
        {
            if(org_filepath.trimmed() == "")
            {
                exp_detail = QString("&lt;br /&gt; &lt;b&gt; &lt;u&gt; &lt;i&gt;" + QObject::tr("Exported File Detail") + "&lt;/i&gt; &lt;/u&gt; &lt;/b&gt;") +"&lt;br /&gt;"
                        + QString("&lt;b&gt; &lt;font color = \"red\"&gt;" + QObject::tr("File does not exist") + "&lt;/font&gt; &lt;/b&gt; &lt;br/&gt; &lt;br/&gt;");

            }
            else
            {
                exp_detail = QString("&lt;br/&gt; &lt;b&gt; &lt;u&gt; &lt;i&gt;" + QObject::tr("Exported File Detail") + "&lt;/i&gt; &lt;/u&gt; &lt;/b&gt;") +"&lt;br /&gt;"
                        + "&lt;b&gt;"+  QObject::tr("Original File") + ": &lt;/b&gt;" +"\t"+ original_filename +"\t"
                        + QString("&lt;br /&gt;&lt;b&gt; &lt;font color = \"red\"&gt;" + QObject::tr("File does not exist") + "&lt;/font&gt;&lt;/b&gt; &lt;br /&gt;&lt;br /&gt;");

            }
        }
        else
        {
            QString file_name = relative_path;
            file_name = file_name.remove("./files/");

            QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
            QString numeric_timezone_value;
            if(machine_timezone_string.contains("GMT"))
            {
                QStringList tmp_list = machine_timezone_string.split("GMT");
                numeric_timezone_value = tmp_list.at(1);
            }


            exp_detail = QString("&lt;br /&gt;&lt;b&gt; &lt;u&gt; &lt;i&gt;"+  QObject::tr("Exported File Detail") + "&lt;/i&gt;&lt;/u&gt; &lt;/b&gt;") +"&lt;br /&gt;"
                    + "&lt;b&gt;"+  QObject::tr("Original File") + ": &lt;/b&gt;" +"\t"+ original_filename +"\t&lt;br /&gt;"
                    + "&lt;b&gt;"+  QObject::tr("Exported File") + ": &lt;/b&gt;" +"\t"+ relative_path +"\t&lt;br /&gt;"
                    + "&lt;b&gt;"+  QObject::tr("Size") + ": &lt;/b&gt;" + recon_static_functions::human_readable_size(query_export_detail.value(1).toString(), Q_FUNC_INFO)
                    + " (" + query_export_detail.value(1).toString() + QObject::tr(" bytes") + ")" + "\t&lt;br /&gt;"
                    + "&lt;b&gt;"+  QObject::tr("MD5") + ": &lt;/b&gt;" +"\t"+ query_export_detail.value(2).toString() +"&lt;br /&gt;"
                    +"&lt;b&gt;"+  QObject::tr("SHA1") + ": &lt;/b&gt;" +"\t"+ query_export_detail.value(3).toString() +"\t&lt;br /&gt;"
                    + "&lt;b&gt;"+  QObject::tr("Export Time") + ": &lt;/b&gt;" +"\t"+ QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString()) + "&lt;br /&gt;&lt;br /&gt;";
        }

        exp_detail_list << exp_detail;
    }

    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_KML_Filedetail_QStringList , exp_detail_list);

    return;
}

void kml_report_module::create_tag_report_kml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts", Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open -----FAILED---" + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" error" + file.errorString() , Q_FUNC_INFO);
        return;
    }

    QString firstline = QString("<?xml version=") + "\"" + "1.0" + "\"" + " encoding=" + "\""+ "UTF-8" +"\"" + "?>";
    QString secondline = QString("<kml xmlns=")+ "\""+ "http://www.opengis.net/kml/2.2"+ "\"" + ">";
    QString new_line = "\n";

    file.write(firstline.toLocal8Bit());
    file.write(new_line.toLocal8Bit());
    file.write(secondline.toLocal8Bit());
    file.write(new_line.toLocal8Bit());

    QString buffer;
    QXmlStreamWriter stream_kml(&buffer);
    stream_kml.setAutoFormatting(true);
    stream_kml.writeStartElement("Document"); //Document Start



    //=============== is data available start ==================//
    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list))
    {
        stream_kml.writeStartElement("Placemark");
        stream_kml.writeTextElement("name",get_plugin_name());
        stream_kml.writeTextElement("description", "No Data Available");
        stream_kml.writeEndElement();
        stream_kml.writeEndElement();
        file.write(buffer.toLocal8Bit());
        file.write("</kml>");
        file.flush();
        file.close();
        return;
    }
    //=============== is data available end ==================//



    //=============== export start ==================//
    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);
        if(tabs_export_status_list.at(pp) == "1")
            export_file_obj->export_file(MACRO_REPORT_SCOPE_TAGS_QString,tabname,selected_tags_list);
    }
    //=============== export end ==================//


    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        if(!report_generic_function_obj->pub_is_data_available_in_tab(QStringList(tabname), MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list))
            continue;

        bool bool_exp = false;
        if(tabs_export_status_list.at(pp) == "1")
            bool_exp = true;


        create_tablewidget_kml_report(&stream_kml, tabname,MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list,bool_exp);

    }


    stream_kml.writeEndElement();
    file.write(buffer.toLocal8Bit());
    file.write("</kml>");
    file.flush();
    file.close();


    recon_static_functions::app_debug(" -Ends" , Q_FUNC_INFO);

}


void kml_report_module::create_tablewidget_kml_report(QXmlStreamWriter *stream_kml, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked)
{

    for(int i_tab = 0 ; i_tab < table_name_list_tablewidget.size() ; i_tab++)
    {
        if(selected_tab_name != tab_name_list_tablewidget.at(i_tab))
            continue;

        QString is_report_available = table_kml_report_available_list_tablewidget.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        QString table_name = table_name_list_tablewidget.at(i_tab);
        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {
            QString is_export_available  = table_export_available_list_tablewidget.at(i_tab);
            QStringList cols_list = rt_kml_col_name_list.at(i_tab);
            QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
            bool bool_multiple_export = false;
            if(is_multiple_export == "2")
                bool_multiple_export = true;


            for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
            {
                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {

                    QString command = "select " + cols_list.join(",") + ", source_count_name from '" + table_name + "'";

                    set_table_data_kml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export, stream_kml,destination_db_file_list.at(db_count));
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, destination_db_file_list.at(db_count), selected_tags_list, Q_FUNC_INFO);
                    for(int i = 0; i < total_record_list.size(); i++)
                    {
                        QString  command = QString("select " + cols_list.join(",") + ", source_count_name from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");

                        set_table_data_kml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export, stream_kml,destination_db_file_list.at(db_count));
                    }
                }
            }
        }
    }

}

QString kml_report_module::get_plugin_name()
{
    return module_name_str;
}

