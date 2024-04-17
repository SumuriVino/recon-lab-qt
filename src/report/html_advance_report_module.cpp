#include "html_advance_report_module.h"

html_advance_report_module::html_advance_report_module(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    report_html_template_plugin_advance_obj =  new report_template_html_advance;

    report_generic_function_obj = new report_generic_function(this);
    export_file_obj = new export_file_module(this);
    bool_cancel_report = false;

}


void html_advance_report_module::pub_cancel_report()
{
    bool_cancel_report = true;
    export_file_obj->pub_cancel_export();
}

void html_advance_report_module::set_essentials()
{
    bool_cancel_report = false;

    clear_fields();

    fill_variable_from_narad_muni();

    report_generic_function_obj->pub_set_essentials();
    export_file_obj->set_essentials();
}


void html_advance_report_module::set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
    report_generic_function_obj->pub_set_narad_muni_for_report(mobj);
    export_file_obj->set_narad_muni_for_report(mobj);

}

void html_advance_report_module::clear_exportfilelist_table()
{
    export_file_obj->set_export_entry_null();
}

void html_advance_report_module::fill_variable_from_narad_muni()
{
    table_export_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList).toStringList();
    table_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList).toStringList();

    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();
    module_name_str     = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();

    table_html_report_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Html_Report_Available_Tablewidget_QStringList).toStringList();
    table_html_report_available_list_textedit    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Report_Available_Textedit_QStringList).toStringList();
    table_html_report_available_list_stackwidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Report_Available_Stack_QStringList).toStringList();

    artifact_common_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Artifact_Common_Textedit_QStringList).toStringList();
    artifact_common_list_stack    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Artifact_Common_Stack_QStringList).toStringList();

    tab_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList).toStringList();
    table_name_list_textedit  = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Textedit_QStringList).toStringList();
    tab_name_list_textedit    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Textedit_QStringList).toStringList();
    table_name_list_stack     = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Stack_QStringList).toStringList();

    tab_name_list_stack    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Stack_QStringList).toStringList();
    parent_module_name_str = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Parent_Module_Name_QString).toString();
    table_artifact_common_list_tablewidget    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Artifact_Common_Tablewidget_QStringList).toStringList();
    table_report_html_header_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Report_Html_Header_Tablewidget_QStringList).toStringList();

    table_report_header_column_width_html_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Report_Header_Column_Width_Html_Tablewidget_QStringList).toStringList();
    table_export_criteria_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList).toStringList();

    report_html_header_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Header_Textedit_QStringList).toStringList();
    report_header_column_width_html_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Header_Column_Width_Html_Textedit_QStringList).toStringList();

    report_html_header_list_stack = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Header_Stack_QStringList).toStringList();
    report_header_column_width_html_list_stack = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Header_Column_Width_Html_Stack_QStringList).toStringList();
    reports_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Reports_Dir_Path_QString).toString();
    if(!reports_dir_path.endsWith("/"))
        reports_dir_path.append("/");

    destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_File_Location_QString).toString();

    max_record_per_page = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Global_Adv_Html_Records_Per_Page_int).toInt();

    tw_data_type_list_tablewidget.clear();
    QList<QVariant> data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList).toList();
    for(int i=0; i < data_type_tablewidget_list.size(); i++)
        tw_data_type_list_tablewidget.append(data_type_tablewidget_list.at(i).toStringList());

    rt_html_d_name_list_tablewidget.clear();
    QList<QVariant> html_d_name_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Display_Name_Tablewidget_QList).toList();
    for(int i=0; i < html_d_name_tablewidget_list.size(); i++)
        rt_html_d_name_list_tablewidget.append(html_d_name_tablewidget_list.at(i).toStringList());

    rt_html_order_list_tablewidget.clear();
    QList<QVariant> html_order_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Order_Tablewidget_QList).toList();
    for(int i=0; i < html_order_tablewidget_list.size(); i++)
        rt_html_order_list_tablewidget.append(html_order_tablewidget_list.at(i).toStringList());


    rt_html_data_type_list_tablewidget.clear();
    QList<QVariant> html_data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Data_Type_Tablewidget_QList).toList();
    for(int i=0; i < html_data_type_tablewidget_list.size(); i++)
        rt_html_data_type_list_tablewidget.append(html_data_type_tablewidget_list.at(i).toStringList());


    rt_html_col_name_list_tablewidget.clear();
    QList<QVariant> html_col_name_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Col_Name_Tablewidget_QList).toList();
    for(int i=0; i < html_col_name_tablewidget_list.size(); i++)
        rt_html_col_name_list_tablewidget.append(html_col_name_tablewidget_list.at(i).toStringList());

    report_html_col_name_list_textedit.clear();
    QList<QVariant> report_html_col_name_textedit_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Col_Name_Textedit_QList).toList();
    for(int i=0; i < report_html_col_name_textedit_list.size(); i++)
        report_html_col_name_list_textedit.append(report_html_col_name_textedit_list.at(i).toStringList());

    report_html_datatype_list_textedit.clear();
    QList<QVariant> report_html_datatype_textedit_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Datatype_Textedit_QList).toList();
    for(int i=0; i < report_html_datatype_textedit_list.size(); i++)
        report_html_datatype_list_textedit.append(report_html_datatype_textedit_list.at(i).toStringList());


    report_html_col_name_list_stack.clear();
    QList<QVariant> report_html_col_name_stack_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Col_Name_Stack_QList).toList();
    for(int i=0; i < report_html_col_name_stack_list.size(); i++)
        report_html_col_name_list_stack.append(report_html_col_name_stack_list.at(i).toStringList());

    report_html_datatype_list_stack.clear();
    QList<QVariant> report_html_datatype_stack_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Datatype_Stack_QList).toList();
    for(int i=0; i < report_html_datatype_stack_list.size(); i++)
        report_html_datatype_list_stack.append(report_html_datatype_stack_list.at(i).toStringList());


}

void html_advance_report_module::clear_fields()
{
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList , QStringList());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList , QStringList());
}

void html_advance_report_module::create_tablewidget_html_advanced_report(QString path, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked)
{

    for(int i_tab = 0 ; i_tab < table_name_list_tablewidget.size(); i_tab++)
    {

        if(selected_tab_name != tab_name_list_tablewidget.at(i_tab))
            continue;

        QString is_report_available = table_html_report_available_list_tablewidget.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
            write_data_for_html_report_advance_full(report_scope,path,i_tab,selected_tags_list,tab_export_checked);
        else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
            write_data_for_html_report_advance_tags(report_scope,path,i_tab,selected_tags_list,tab_export_checked);
    }
}

void html_advance_report_module::create_textedit_html_advanced_report(QString path, QString selected_tab_name, QString report_scope, QStringList selected_tags_list)
{
    for(int i_tab = 0 ; i_tab < table_name_list_textedit.size(); i_tab++)
    {
        if(selected_tab_name != tab_name_list_textedit.at(i_tab))
            continue;

        QString is_report_available = table_html_report_available_list_textedit.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        QString table_name = table_name_list_textedit.at(i_tab);
        QString tab_name = tab_name_list_textedit.at(i_tab);
        tab_name.replace(" ","_");

        QString index_file_path = path;

        if(index_file_path.endsWith("index.html"))
            index_file_path.chop(10);


        QString plugin_name = get_plugin_name();

        QString file_path = index_file_path + "Plugins/" + plugin_name + "/";

        QDir dir ;
        dir.mkpath(file_path);

        file_path = file_path + tab_name + "0.html";


        QFile file(file_path);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recon_static_functions::app_debug(" -file opened ----FAILED--- " + file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" -Error" + file.errorString(), Q_FUNC_INFO);
            return;
        }

        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {

            QString is_common_artifact = artifact_common_list_textedit.at(i_tab);
            if(is_common_artifact == QString("1"))
            {
                QString artifacts_str = "<b>" + QObject::tr("Artifacts Source") + ": </b><br>";
                file.write(artifacts_str.toLocal8Bit());

                QStringList source_list = report_generic_function_obj->pub_get_artifacts_source_list(table_name, report_scope,selected_tags_list);
                for(int cc = 0; cc < source_list.size(); cc++)
                {
                    file.write(QString(source_list.at(cc)).toLocal8Bit().data());
                    file.write("<br>");
                }
            }

            QString header_str = report_html_header_list_textedit.at(i_tab);
            QStringList headers = header_str.split("::");
            headers.insert(0,"Sr. No.");

            QString header_str_col_width = report_header_column_width_html_list_textedit.at(i_tab);
            QStringList  headers_column_width = header_str_col_width.split("::");
            headers_column_width.insert(0,"50");

            QString report_data;

            QString report_scope = QString(MACRO_REPORT_SCOPE_FULL_QString);
            QStringList list_artifacts_source = collect_artifacts_source_list(report_scope,QStringList(""));


            ///-1. START FROM TOP TO MIDDLE NAV BAR.
            QString  current_pg_of_total_pg = "Page 1 of 1";
            report_data += sub_report_topbar_to_tab_header(list_artifacts_source,tab_name,current_pg_of_total_pg);

            ///-2. Set table header.
            report_data += sub_report_create_headers(i_tab,report_html_header_list_textedit,report_header_column_width_html_list_textedit);

            ///-3. Insert Table Data list below
            //-------------------------------------------------------

            // QStringList cols_nam_list =  text_column_name_list.at(i);
            QStringList cols_list = report_html_col_name_list_textedit.at(i_tab);
            QStringList cols_data_type_list =  report_html_datatype_list_textedit.at(i_tab);

            int textedit_column_pos = -1;
            for(int j=0; j < cols_list.size(); j++)
            {
                if(cols_data_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_TEXTEDIT_QString){
                    textedit_column_pos = j;
                }
            }

            if(textedit_column_pos == -1)
            {
                recon_static_functions::app_debug(" ---FAILED--- Textedit column not found" + file.fileName(), Q_FUNC_INFO);
                return;
            }


            for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
            {
                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    QString m_rpt_type = QString(MACRO_REPORT_TYPE_HTML_ADVANCE_QString);

                    QString command = "select " + cols_list.join(",") + " from '" + table_name + "'";
                    report_data +=  set_textedit_data(command,textedit_column_pos,m_rpt_type,report_scope,0,destination_db_file_list.at(db_count));
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QString command;
                    QStringList selected_record_no_list;

                    for(int i = 0; i < selected_tags_list.size(); i++)
                    {
                        QString tag_str = selected_tags_list.at(i);
                        command = QString("SELECT INT FROM '" + table_name + "' WHERE recon_tag_value LIKE" + "'%" + tag_str + "%'");
                        report_generic_function_obj->pub_get_only_tags_record_of_table_including_multiple(command,selected_record_no_list,destination_db_file_list.at(db_count));

                    }
                    for(int i = 0; i < selected_record_no_list.size(); i++)
                    {
                        command = QString("select " + cols_list.join(",") + " from '" + table_name + "' where INT=" + "'" + selected_record_no_list.at(i) + "'");
                        report_data +=  set_textedit_data(command,textedit_column_pos,MACRO_REPORT_TYPE_HTML_ADVANCE_QString,MACRO_REPORT_SCOPE_TAGS_QString,i,destination_db_file_list.at(db_count));
                    }
                }
            }

            ///4.-- Table End function
            report_data += report_html_template_plugin_advance_obj->report_end();
            file.write(report_data.toLocal8Bit().data());
            file.close();
        }
    }
}

void html_advance_report_module::create_stackwidget_html_advanced_report(QString path, QString selected_tab_name, QString report_scope, QStringList selected_tags_list)
{
    for(int i_tab = 0 ; i_tab < table_name_list_stack.size(); i_tab++)
    {
        if(selected_tab_name != tab_name_list_stack.at(i_tab))
            continue;


        QString is_report_available = table_html_report_available_list_stackwidget.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        QString table_name = table_name_list_stack.at(i_tab);

        QString tab_name = tab_name_list_stack.at(i_tab);
        tab_name.replace(" ","_");

        QString index_file_path = path;

        if(index_file_path.endsWith("index.html"))
            index_file_path.chop(10);


        QString plugin_name = get_plugin_name();

        QString file_path = index_file_path + "Plugins/" + plugin_name + "/";

        QDir dir ;
        dir.mkpath(file_path);

        file_path = file_path + tab_name + "0.html";


        QFile file(file_path);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recon_static_functions::app_debug(" -file opened ----FAILED--- " + file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" -Error" + file.errorString(), Q_FUNC_INFO);
            return;
        }

        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {

            QString is_common_artifact = artifact_common_list_stack.at(i_tab);
            if(is_common_artifact == QString("1"))
            {
                QString artifacts_str = "<b>" + QObject::tr("Artifacts Source") + ": </b><br>";
                file.write(artifacts_str.toLocal8Bit());

                QStringList source_list = report_generic_function_obj->pub_get_artifacts_source_list(table_name, report_scope,selected_tags_list);
                for(int cc = 0; cc < source_list.size(); cc++)
                {
                    file.write(QString(source_list.at(cc)).toLocal8Bit().data());
                    file.write("<br>");
                }
            }

            QString header_str = report_html_header_list_stack.at(i_tab);
            QStringList headers = header_str.split("::");
            headers.insert(0,"Sr. No.");

            QString header_str_col_width = report_header_column_width_html_list_stack.at(i_tab);
            QStringList  headers_column_width = header_str_col_width.split("::");
            headers_column_width.insert(0,"50");


            QString report_data;
            QString report_scope = QString(MACRO_REPORT_SCOPE_FULL_QString);
            QStringList list_artifacts_source = collect_artifacts_source_list(report_scope,QStringList(""));

            ///-1. START FROM TOP TO MIDDLE NAV BAR.
            QString  current_pg_of_total_pg = "Page 1 of 1";
            report_data += sub_report_topbar_to_tab_header(list_artifacts_source,tab_name,current_pg_of_total_pg);


            ///-2. Set table header.
            report_data += sub_report_create_headers(i_tab,report_html_header_list_stack,report_header_column_width_html_list_stack);

            ///-3. Insert Table Data list below
            //-------------------------------------------------------

            QStringList cols_list = report_html_col_name_list_stack.at(i_tab);
            QStringList cols_data_type_list =  report_html_datatype_list_stack.at(i_tab);

            int stack_column_pos = -1;
            for(int j=0; j < cols_list.size(); j++){
                if(cols_data_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_STACK_QString){
                    stack_column_pos = j;
                }
            }

            if(stack_column_pos == -1)
            {
                recon_static_functions::app_debug(" ----FAILED--- Textedit column not found " + file.fileName(), Q_FUNC_INFO);
                return;
            }

            for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
            {
                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    QString m_rpt_type = QString(MACRO_REPORT_TYPE_HTML_ADVANCE_QString);
                    QString command = "select " + cols_list.join(",") + " from '" + table_name + "'";
                    report_data +=  set_stack_data(command,stack_column_pos,m_rpt_type,report_scope,0,destination_db_file_list.at(db_count));

                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QString command;
                    QStringList selected_record_no_list;
                    for(int i = 0; i < selected_tags_list.size(); i++)
                    {
                        QString tag_str = selected_tags_list.at(i);
                        command.clear();
                        command = QString("SELECT INT FROM '" + table_name + "' WHERE recon_tag_value LIKE" + "'%" + tag_str + "%'");

                        report_generic_function_obj->pub_get_only_tags_record_of_table_including_multiple(command,selected_record_no_list,destination_db_file_list.at(db_count));
                    }
                    for(int i = 0; i < selected_record_no_list.size(); i++)
                    {
                        command = QString("select " + cols_list.join(",") + " from '" + table_name + "' where INT=" + "'" + selected_record_no_list.at(i) + "'");
                        report_data +=  set_stack_data(command,stack_column_pos,MACRO_REPORT_TYPE_HTML_ADVANCE_QString,MACRO_REPORT_SCOPE_TAGS_QString,i,destination_db_file_list.at(db_count));
                    }
                }
            }

            ///-- Table End function
            report_data += report_html_template_plugin_advance_obj->report_end();
            file.write(report_data.toLocal8Bit().data());
            file.flush();
            file.close();
        }
    }
}

QString html_advance_report_module::get_plugin_name()
{
    return module_name_str;
}

void html_advance_report_module::create_full_report_advanced_html(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" -file open ---FAILED---  " + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + file.errorString() , Q_FUNC_INFO);
        return;
    }


    selected_tabs_list = selected_tablist;

    bool bool_chat_available = false;
    if(global_csv_reader_class_selected_obj->is_plugin_available_for_chat_view(get_plugin_name()))
    {

        QString tab_name = global_csv_reader_class_selected_obj->get_chat_view_tab_name(get_plugin_name());

        QString index_file_path = file_path;
        if(index_file_path.endsWith("index.html"))
            index_file_path.chop(10);

        QString plugin_name = get_plugin_name();
        QString t_tabname_str = tab_name;

        if(plugin_name.contains(" "))
            plugin_name.replace(" ","_");

        QString file_path_dir = index_file_path + "Plugins/" + plugin_name + "/";
        QDir dir ;
        dir.mkpath(file_path_dir);
        QString chat_file_path = file_path_dir + t_tabname_str.replace(" ","_") + QString::number(0) + ".html";


        QString dest_dir = file_path_dir + "Files/";
        dir.mkpath(dest_dir);

        QString resorurce_dir = file_path_dir + "resources/";
        dir.mkpath(resorurce_dir);
        QFile::copy(recon_static_functions::get_appilcation_relative_path() + "/Resources/graphics/html_reports/css/chat_template.css" , resorurce_dir + "/chat_template.css");


        QFile dest_file(chat_file_path);
        if(!dest_file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recon_static_functions::app_debug(" -file opened ----FAILED--- " + dest_file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" -Error" + dest_file.errorString(), Q_FUNC_INFO);
            return;
        }



        int i_pg = 0;
        QString first_page = t_tabname_str.replace(" ","_") + QString::number(i_pg) + ".html";
        QString previous_link = "";
        QString next_link = t_tabname_str.replace(" ","_") + QString::number(i_pg + 1) + ".html";
        QString last_page = t_tabname_str.replace(" ","_") + QString::number(i_pg) + ".html";






        dest_file.write("<br><br><br>");
        dest_file.write(sub_report_topbar_to_tab_header(QStringList(""),t_tabname_str.replace(" ","_"),"Page 1 of 1").toLocal8Bit());
        dest_file.write(report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page).toLocal8Bit());



        QString account = "Configured_Account";
        QString dir_path_to_itr = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + get_plugin_name().replace(" ","_") + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Chat_View_Page_QString ;
        QDirIterator itr(dir_path_to_itr);
        while(itr.hasNext())
        {
            QFileInfo file_info(itr.next());

            if(file_info.isDir() && file_info.filePath().contains(account))
            {
                if(file_info.fileName() == QString(".") || file_info.fileName() == QString(".."))
                    continue ;

                QString account_info = file_info.filePath() + "/configured_account.txt";
                QByteArray account_info_arr;
                QFile ch_file(account_info);
                if(!ch_file.open(QIODevice::ReadOnly))
                    continue ;

                account_info_arr =  ch_file.readAll();

                QDirIterator chat_dir_itr(file_info.filePath() + "/");
                while(chat_dir_itr.hasNext())
                {

                    QFileInfo cht_file_info(chat_dir_itr.next());
                    if(cht_file_info.fileName() == QString(".") || cht_file_info.fileName() == QString(".."))
                        continue ;

                    if(cht_file_info.filePath().contains("chat_buddy"))
                    {
                        QDirIterator t_itr(cht_file_info.filePath() + "/");
                        while(t_itr.hasNext())
                        {

                            QFileInfo file_info(t_itr.next());
                            if(file_info.filePath().endsWith("chat_buddy_data.html"))
                            {

                                bool_chat_available = true;
                                QString dir_path = file_info.filePath();
                                dir_path.chop(file_info.fileName().size());

                                ///-------copy files dir
                                recon_static_functions::copy_directory(dir_path + "Files" , dest_dir , Q_FUNC_INFO);
                                ///-------copy files dir



                                QString chat_b_info_file_path = dir_path + "chat_buddy_info.txt";
                                QByteArray chat_b_data_arr;
                                QFile ch_file(chat_b_info_file_path);
                                if(!ch_file.open(QIODevice::ReadOnly))
                                    return ;

                                chat_b_data_arr =  ch_file.readAll();
                                dest_file.write(QString("<br><b><u>" + QString("<font size=\"4\">") + QString("Participants") + "</font>" + "</u></b><br>" + "<font size=\"3\">" + account_info_arr + QString("<font size=\"4\">") + QString(" , ") + "</font>" + chat_b_data_arr + "</font>").toLocal8Bit());
                                ch_file.flush();
                                ch_file.close();




                                ///-------write data in file
                                QString html_file_path = dir_path + "chat_buddy_data.html";
                                QFile source_file(html_file_path);
                                if(!source_file.open(QIODevice::ReadOnly))
                                    return ;


                                QByteArray data_arr;
                                data_arr =  source_file.readAll();
                                dest_file.write(data_arr);
                                source_file.close();



                                dest_file.flush();

                            }

                        }

                    }


                }

            }

        }

        dest_file.write(report_html_template_plugin_advance_obj->report_end().toLocal8Bit());
        dest_file.flush();
        dest_file.close();


    }





    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist,MACRO_REPORT_SCOPE_FULL_QString,selected_tags_list))
        {
            QString report_data;

            QString index_file_path = file_path;

            if(index_file_path.endsWith("index.html"))
                index_file_path.chop(10);

            QString plugin_name = get_plugin_name();
            QString file_path = index_file_path + "Plugins/" + plugin_name + "/";

            QDir dir ;
            dir.mkpath(file_path);

            file_path = file_path + tabname + "0.html";

            QFile file_m(file_path);
            if(!file_m.open(QIODevice::ReadWrite | QIODevice::Truncate))
            {
                recon_static_functions::app_debug(" -file opened ----FAILED--- " + file_m.fileName(), Q_FUNC_INFO);
                recon_static_functions::app_debug(" -Error" + file_m.errorString(), Q_FUNC_INFO);
                return;
            }

            report_data += sub_report_no_data_available();
            file_m.write(report_data.toLocal8Bit());
            file_m.flush();
            file_m.close();
            return;
        }
        else
        {
            //=============== export start ==================//

            if(tabs_export_status_list.at(pp) == "1")
                export_file_obj->export_file(MACRO_REPORT_SCOPE_FULL_QString,tabname,selected_tags_list);

            //=============== export end ==================//

        }

    }


    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        bool bool_exp = false;
        if(tabs_export_status_list.at(pp) == "1")
            bool_exp = true;

        create_tablewidget_html_advanced_report(file_path,tabname,MACRO_REPORT_SCOPE_FULL_QString,selected_tags_list,bool_exp);

        create_textedit_html_advanced_report(file_path,tabname,MACRO_REPORT_SCOPE_FULL_QString,selected_tags_list);

        create_stackwidget_html_advanced_report(file_path,tabname,MACRO_REPORT_SCOPE_FULL_QString,selected_tags_list);
    }

    file.close();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


void html_advance_report_module::create_tag_report_advanced_html(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts-", Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" -file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error"  + file.errorString(), Q_FUNC_INFO);
        return;
    }

    selected_tabs_list.clear();
    //bool bool_chat_available = false;
    selected_tabs_list << selected_tablist;
    selected_tabs_list.removeDuplicates();

    if(global_csv_reader_class_selected_obj->is_plugin_available_for_chat_view(get_plugin_name()))
    {
        QString chat_viewer_bookmark_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select INT from chat_viewer_info where plugin_name = ?", QStringList(get_plugin_name()),0,chat_viewer_bookmark_db_path,Q_FUNC_INFO);
        if(!INT_str.trimmed().isEmpty())
        {


            //bool_chat_available = true;
            if(selected_tags_list.contains(MACRO_Tag_Name_Bookmarks))
            {

                QString tab_name = global_csv_reader_class_selected_obj->get_chat_view_tab_name(get_plugin_name());
                //selected_tabs_list << selected_tablist;

                QString index_file_path = file_path;
                if(index_file_path.endsWith("index.html"))
                    index_file_path.chop(10);

                QString plugin_name = get_plugin_name();
                QString t_tabname_str = tab_name;

                if(plugin_name.contains(" "))
                    plugin_name.replace(" ","_");

                QString file_path_dir = index_file_path + "Plugins/" + plugin_name + "/";
                QDir dir ;
                dir.mkpath(file_path_dir);
                QString chat_file_path = file_path_dir + t_tabname_str.replace(" ","_") + QString::number(0) + ".html";


                QString dest_dir = file_path_dir + "Files/";
                dir.mkpath(dest_dir);

                QString resorurce_dir = file_path_dir + "resources/";
                dir.mkpath(resorurce_dir);
                QFile::copy(recon_static_functions::get_appilcation_relative_path() + "/Resources/graphics/html_reports/css/chat_template.css" , resorurce_dir + "/chat_template.css");


                QFile dest_file(chat_file_path);
                if(!dest_file.open(QIODevice::ReadWrite | QIODevice::Truncate))
                {
                    recon_static_functions::app_debug(" -file opened ----FAILED--- " + dest_file.fileName(), Q_FUNC_INFO);
                    recon_static_functions::app_debug(" -Error" + dest_file.errorString(), Q_FUNC_INFO);
                    file.close();
                    return;
                }



                int i_pg = 0;
                QString first_page = t_tabname_str.replace(" ","_") + QString::number(i_pg) + ".html";
                QString previous_link = "";
                QString next_link = t_tabname_str.replace(" ","_") + QString::number(i_pg + 1) + ".html";
                QString last_page = t_tabname_str.replace(" ","_") + QString::number(i_pg) + ".html";


                dest_file.write("<br><br><br>");
                dest_file.write(sub_report_topbar_to_tab_header(QStringList(""),t_tabname_str.replace(" ","_"),"Page 1 of 1").toLocal8Bit());
                dest_file.write(report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page).toLocal8Bit());



                ///----------------chat data--------
                QString connection_naam = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                dest_db.setDatabaseName(chat_viewer_bookmark_db_path);
                if(!dest_db.open())
                {
                    recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
                    file.close();
                    return ;
                }


                QSqlQuery select_qry(dest_db);
                QString command = "SELECT  DISTINCT chat_buddy , configured_account FROM chat_viewer_info  where plugin_name = ? AND tab_name = ?";
                select_qry.prepare(command);
                select_qry.addBindValue(get_plugin_name());
                select_qry.addBindValue(tab_name);
                if(!select_qry.exec())
                {
                    recon_static_functions::app_debug(" query_index.exec()  ------FAILED-----"+ dest_db.databaseName(), Q_FUNC_INFO);
                    recon_static_functions::app_debug(select_qry.lastError().text() ,Q_FUNC_INFO);
                    recon_static_functions::app_debug(select_qry.executedQuery() ,Q_FUNC_INFO);
                    dest_db.close();
                    file.close();
                    return;
                }


                while(select_qry.next())
                {

                    QString chat_buddy_str = select_qry.value("chat_buddy").toString();
                    QString configured_account_str = select_qry.value("configured_account").toString();


                    QString account = "Configured_Account";
                    QString dir_path_to_itr = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + get_plugin_name().replace(" ","_") + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Chat_View_Page_QString ;
                    QDirIterator itr(dir_path_to_itr,QDirIterator::Subdirectories);
                    while(itr.hasNext())
                    {


                        QFileInfo file_info(itr.next());
                        if(file_info.fileName().endsWith("configured_account.txt") && file_info.filePath().contains(account))
                        {
                            QFile read_file(file_info.filePath());
                            if(!read_file.open(QIODevice::ReadOnly))
                            {
                                recon_static_functions::app_debug(" File Opened ------FAILED-----"+ read_file.fileName(), Q_FUNC_INFO);
                                recon_static_functions::app_debug(read_file.errorString() ,Q_FUNC_INFO);
                                continue ;
                            }

                            QString data_str = read_file.readAll();
                            if(data_str.contains(configured_account_str))
                            {

                                QString chat_buddy_dir_path = file_info.filePath();
                                chat_buddy_dir_path.chop(file_info.fileName().size());

                                QDirIterator chat_b_itr(chat_buddy_dir_path , QDirIterator::Subdirectories);
                                while(chat_b_itr.hasNext())
                                {

                                    QFileInfo chat_buddy_info(chat_b_itr.next());
                                    if(chat_buddy_info.fileName().endsWith(".txt") && chat_buddy_info.filePath().contains("chat_buddy"))
                                    {

                                        QFile ch_file(chat_buddy_info.filePath());
                                        if(!ch_file.open(QIODevice::ReadOnly))
                                        {
                                            recon_static_functions::app_debug(" File Opened ------FAILED-----"+ ch_file.fileName(), Q_FUNC_INFO);
                                            recon_static_functions::app_debug(ch_file.errorString() ,Q_FUNC_INFO);
                                            continue ;
                                        }

                                        QString data_str = ch_file.readAll();
                                        if(data_str.contains(chat_buddy_str))
                                        {

                                            QString chat_b_dir_path = chat_buddy_info.filePath();
                                            chat_b_dir_path.chop(chat_buddy_info.fileName().size());

                                            ///-------copy files dir
                                            recon_static_functions::copy_directory(chat_b_dir_path + "Files" , dest_dir , Q_FUNC_INFO);
                                            ///-------copy files dir


                                            ///-------write data in file
                                            QString html_file_path = chat_b_dir_path + "chat_buddy_data.html";
                                            QFile source_file(html_file_path);
                                            if(!source_file.open(QIODevice::ReadOnly))
                                                return ;


                                            QByteArray data_arr;
                                            data_arr =  source_file.readAll();
                                            source_file.close();

                                            dest_file.write(QString("<br><b><u>" + QString("<font size=\"4\">") + QString("Participants") + "</font>" + "</u></b><br>" + "<font size=\"3\">" + configured_account_str + QString("<font size=\"4\">") + QString(" , ") + "</font>" + chat_buddy_str + "</font>").toLocal8Bit());
                                            dest_file.write(data_arr);
                                            dest_file.flush();
                                            ///-------write data in file

                                            break ;

                                        }

                                    }

                                }

                                break ;
                            }

                        }


                    }

                }



                dest_db.close();

                ///----------------chat data--------






                dest_file.write(report_html_template_plugin_advance_obj->report_end().toLocal8Bit());
                dest_file.flush();
                dest_file.close();

            }

        }

    }


    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        if(!report_generic_function_obj->pub_is_data_available_in_tab(QStringList(tabname),MACRO_REPORT_SCOPE_TAGS_QString,selected_tags_list))
            continue ;

        if(tabs_export_status_list.at(pp) == "1")
            export_file_obj->export_file(MACRO_REPORT_SCOPE_TAGS_QString,tabname,selected_tags_list);

    }

    //    if(!bool_chat_available)
    //        selected_tabs_list << selected_tablist;




    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        bool bool_exp = false;
        if(tabs_export_status_list.at(pp) == "1")
            bool_exp = true;

        create_tablewidget_html_advanced_report(file_path,tabname,MACRO_REPORT_SCOPE_TAGS_QString,selected_tags_list,bool_exp);

        create_textedit_html_advanced_report(file_path,tabname,MACRO_REPORT_SCOPE_TAGS_QString,selected_tags_list);

        create_stackwidget_html_advanced_report(file_path,tabname,MACRO_REPORT_SCOPE_TAGS_QString,selected_tags_list);
    }

    file.close();
    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);
    return;

}

void html_advance_report_module::create_screen_report_advanced_html(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list)
{

}

void html_advance_report_module::pub_set_plugins_and_tabs_info(QStringList plugins_info)
{
    ///used for plugins dropdown
    selected_plugins_info = plugins_info;
}

QStringList html_advance_report_module::collect_artifacts_source_list(QString report_scope,QStringList selected_tags_list)
{
    QStringList list_artifacts_source;
    for(int ar = 0 ; ar < table_name_list_tablewidget.size(); ar++)
    {
        QString is_common_artifact = table_artifact_common_list_tablewidget.at(ar);
        if(is_common_artifact == QString("1"))
        {
            list_artifacts_source << report_generic_function_obj->pub_get_artifacts_source_list(table_name_list_tablewidget.at(ar), report_scope,selected_tags_list);
        }
    }

    ///- removed duplicate enrty of artifacts from the list.
    list_artifacts_source.removeDuplicates();

    return list_artifacts_source;
}


QString html_advance_report_module::sub_report_topbar_to_tab_header(QStringList list_artifacts_source, QString tab_name, QString current_pg_of_total_pg)
{

    QString report_data;

    ///1.--To make html header upto the home button;
    report_html_template_plugin_advance_obj->set_essential();
    report_data += report_html_template_plugin_advance_obj->report_set_html_header();


    ///2.-- To list Dropdown button in nav bar
    report_data += report_html_template_plugin_advance_obj->report_drop_down_plugin_list(selected_plugins_info);


    ///3.-- To list Dropdown button in nav bar
    report_data += report_html_template_plugin_advance_obj->report_set_agency_name_and_logo();

    QString plugin_name = get_plugin_name();
    QString parent_category = parent_module_name_str;

    ///4.-- For plugin name and icons
    report_data += report_html_template_plugin_advance_obj->report_set_plugin_name_and_icon(plugin_name,parent_category);


    ///5. -- For Artifacts Source display on the Top
    report_data += report_html_template_plugin_advance_obj->report_set_artifacts_source(list_artifacts_source);


    ///6. -- Set Plugin name in sub navigation bar
    QString navbar_tab_name = tab_name.replace("_"," ");
    report_data += report_html_template_plugin_advance_obj->report_set_category_name_in_subnavigation_bar(navbar_tab_name);

    ///7. -- Set Category dropdown List
    report_data += report_html_template_plugin_advance_obj->report_set_category_dropdownlist(selected_tabs_list,current_pg_of_total_pg);

    return report_data;
}


QString html_advance_report_module::sub_report_no_data_available()
{

    QString plugin_name = get_plugin_name();
    QString parent_category = parent_module_name_str;

    report_html_template_plugin_advance_obj->set_essential();

    QString report_data;
    report_data += report_html_template_plugin_advance_obj->report_set_html_header();
    report_data += report_html_template_plugin_advance_obj->report_drop_down_plugin_list(selected_plugins_info);
    report_data += report_html_template_plugin_advance_obj->report_set_agency_name_and_logo();
    report_data += report_html_template_plugin_advance_obj->report_set_plugin_name_and_icon_on_no_data_available(plugin_name,parent_category);

    return report_data;
}

void html_advance_report_module::write_data_for_html_report_advance_full(QString report_scope, QString path, int i_tab, QStringList selected_tags_list , bool tab_export_checked)
{

    int max_rec_per_pg = max_record_per_page;

    qint64 record_count = report_generic_function_obj->pub_get_total_record_of_table(table_name_list_tablewidget.at(i_tab),report_scope,selected_tags_list);

    QString table_name = table_name_list_tablewidget.at(i_tab);
    QString tab_name = tab_name_list_tablewidget.at(i_tab);

    QStringList list_artifacts_source = collect_artifacts_source_list(report_scope,QStringList(""));

    ///-Getting Page Count Accoring to the devision of the page
    int total_page = record_count/max_rec_per_pg;

    if(record_count % max_rec_per_pg != 0)
    {
        total_page = total_page + 1;
    }

    static int count = 0;



    for(int i_pg = 0; i_pg < total_page ; i_pg++)
    {
        if(bool_cancel_report)
            break ;

        tab_name = tab_name.replace(" ","_");

        QString index_file_path = path;

        if(index_file_path.endsWith("index.html"))
            index_file_path.chop(10);


        QString plugin_name = get_plugin_name();

        if(plugin_name.contains(" "))
            plugin_name.replace(" ","_");


        QString file_path = index_file_path + "Plugins/" + plugin_name + "/";

        QDir dir ;
        dir.mkpath(file_path);

        file_path = file_path + tab_name + QString::number(i_pg) + ".html";


        QFile file(file_path);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recon_static_functions::app_debug(" -file opened ----FAILED--- " + file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" -Error" + file.errorString(), Q_FUNC_INFO);
            return;
        }


        QString report_data;

        int page_no = i_pg + 1;
        QString current_pg_of_total_pg;
        current_pg_of_total_pg = "Page " + QString::number(page_no) + " of " + QString::number(total_page);


        ///-- Report Top bar to middle NAV Bar.
        report_data += sub_report_topbar_to_tab_header(list_artifacts_source,tab_name,current_pg_of_total_pg);

        /// -- If record is over a particular limit and page is divided into multiple sub pages then provide its previous and next link
        ///** All Calculation for the record in one page,
        ///  and linkage of next and previos buttons.**/

        static int start_rec = 0;
        static int end_rec = 0;

        QString first_page = tab_name.replace(" ","_") + QString::number(0) + ".html";
        QString last_page = tab_name.replace(" ","_") + QString::number(total_page -1) + ".html";



        QString temp_1 ,temp_2;
        //-If First Page
        if(i_pg == 0)
        {
            start_rec = page_no; // * max_rec_per_pg; (1 * 20)
            end_rec = page_no * max_rec_per_pg;

            ///*-2. Next and previ linking work
            QString previous_link = "";
            QString next_link = tab_name.replace(" ","_") + QString::number(i_pg + 1) + ".html";
            report_data += report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page);

        }
        else if(page_no !=1 && page_no != total_page) //-If Middle pages
        {
            start_rec =  end_rec + 1;
            end_rec = start_rec + max_rec_per_pg -1;

            ///*-2. Next and previ linking work
            QString previous_link = tab_name.replace(" ","_") + QString::number(i_pg - 1) + ".html";
            QString next_link = tab_name.replace(" ","_") + QString::number(i_pg + 1) + ".html";

            temp_1 = previous_link;
            temp_2 = next_link;

            report_data += report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page);

        }
        else if((i_pg + 1) == total_page)
        {
            start_rec = end_rec + 1;
            end_rec = record_count;

            ///*-2. Next and previ linking work
            QString previous_link = tab_name.replace(" ","_") + QString::number(i_pg - 1) + ".html";
            QString next_link = "";
            report_data += report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page);
        }


        ///-- Set Table Header
        QString is_export_available = table_export_available_list_tablewidget.at(i_tab);

        QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);

        bool bool_multiple_export = false;
        if(is_multiple_export == "2")
            bool_multiple_export = true;

        report_data += sub_report_create_tablewidget_table_headers(i_tab,is_export_available,is_multiple_export,tab_export_checked);

        ///-- Insert Table Data list below
        QStringList cols_list = rt_html_col_name_list_tablewidget.at(i_tab);

        qint64 sr_no = 0;
        for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
        {
            QString db_path = destination_db_file_list.at(db_count);

            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            dest_db.setDatabaseName(db_path);
            if(!dest_db.open())
            {
                recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
                continue ;
            }


            if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
            {
                QString command;

                if(record_count > max_record_per_page)
                    command = QString("select " + cols_list.join(",") + ", source_count_name  from '" + table_name + "' where INT BETWEEN " + QString::number(start_rec) + " AND " + QString::number(end_rec) + " ORDER BY INT ASC");
                else
                    command = "select " + cols_list.join(",") + ", source_count_name  from  '" + table_name + "'";

                report_data += set_table_data(i_tab,command,table_name,tab_export_checked,is_export_available,bool_multiple_export,0,db_path , dest_db);
            }
            //            else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
            //            {

            //                QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, db_path, selected_tags_list, Q_FUNC_INFO);
            //                if(get_plugin_name() == QString(MACRO_Plugin_Name_File_System))
            //                {
            //                    QString temp_db_dir_path = db_path;

            //                    QFileInfo file_info(db_path);
            //                    temp_db_dir_path.chop(file_info.fileName().size()); // "file_system.sqlite"

            //                    QString mime_db_path = temp_db_dir_path + "mime.sqlite";
            //                    QString hashes_db_path = temp_db_dir_path + "hashes.sqlite";
            //                    QString sign_db_path = temp_db_dir_path + "signature.sqlite";
            //                    QString exif_db_path  = temp_db_dir_path + "exif_metadata.sqlite";
            //                    QString apple_meta_db_path = temp_db_dir_path + "apple_metadata.sqlite";

            //                    QSqlQuery query_select(dest_db);
            //                    QString sign_db_attach_commmand = QString("ATTACH DATABASE '%1' AS signatureDB")
            //                            .arg(QDir::toNativeSeparators(sign_db_path));
            //                    query_select.exec(sign_db_attach_commmand);

            //                    QString exif_db_attach_commmand = QString("ATTACH DATABASE '%1' AS exif_metadataDB")
            //                            .arg(QDir::toNativeSeparators(exif_db_path));
            //                    query_select.exec(exif_db_attach_commmand);


            //                    QString apple_meta_db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadataDB")
            //                            .arg(QDir::toNativeSeparators(apple_meta_db_path));
            //                    query_select.exec(apple_meta_db_attach_commmand);

            //                    QString hash_db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashesDB")
            //                            .arg(QDir::toNativeSeparators(hashes_db_path));
            //                    query_select.exec(hash_db_attach_commmand);

            //                    QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
            //                            .arg(QDir::toNativeSeparators(mime_db_path));
            //                    query_select.exec(mime_db_attach_commmand);

            //                    for(int i = 0; i < total_record_list.size(); i++)
            //                    {
            //                        QString fs_record_no = total_record_list.at(i);
            //                        QString command = report_generic_function_obj->get_fs_command(fs_record_no);
            //                        report_data += set_table_data(i_tab,command,table_name,tab_export_checked,is_export_available,bool_multiple_export,sr_no,db_path , dest_db);
            //                        sr_no++;
            //                    }

            //                    QString sign_db_detach_commmand = QString("DETACH DATABASE signatureDB");
            //                    query_select.exec(sign_db_detach_commmand);
            //                    QString exif_db_detach_commmand = QString("DETACH DATABASE exif_metadataDB");
            //                    query_select.exec(exif_db_detach_commmand);
            //                    QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
            //                    query_select.exec(apple_meta_db_detach_commmand);
            //                    QString hash_db_detach_commmand = QString("DETACH DATABASE hashesDB");
            //                    query_select.exec(hash_db_detach_commmand);
            //                    QString mime_db_detach_commmand = QString("DETACH DATABASE mimeDB");
            //                    query_select.exec(mime_db_detach_commmand);


            //                }
            //                else
            //                {

            //                    for(int i = 0; i < total_record_list.size(); i++)
            //                    {
            //                        QString  command = QString("select " + cols_list.join(",") + ", source_count_name  from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
            //                        report_data += set_table_data(i_tab,command,table_name,tab_export_checked,is_export_available,bool_multiple_export,sr_no,db_path , dest_db);
            //                        sr_no++;
            //                    }


            //                }
            //            }

            dest_db.close();
        }

        ///-- Table End function
        report_data += report_html_template_plugin_advance_obj->report_end();
        file.write(report_data.toLocal8Bit().data());
        file.flush();
        file.close();
    }
    ///- To Reset the value of the count
    count = 0;
}

void html_advance_report_module::write_data_for_html_report_advance_tags(QString report_scope, QString path, int i_tab, QStringList selected_tags_list , bool tab_export_checked)
{
    int max_rec_per_pg = max_record_per_page;

    ///-- Set Table Header
    QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
    QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);

    bool bool_multiple_export = false;
    if(is_multiple_export == "2")
        bool_multiple_export = true;

    QString table_name = table_name_list_tablewidget.at(i_tab);

    QStringList columns_list;
    QStringList arg_values_list;
    QString select_column_command = QString("Select INT FROM '" + table_name + "' WHERE ");
    for(int ii = 0 ; ii < selected_tags_list.size() ; ii++)
    {
        if(bool_cancel_report)
            break ;

        QString tag_name = selected_tags_list.at(ii);
        if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
        {
            columns_list <<  " bookmark = '1'";
            continue;
        }
        columns_list <<  " recon_tag_value = ?";
        arg_values_list << tag_name;
    }


    struct struct_db_and_record_info
    {
        QStringList databases_name_list;
        QStringList record_list;
    };



    ///--prepare tagged data from all db for feature
    QList <struct_db_and_record_info> q_list_of_struct_db_and_record_info;
    for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
    {
        if(bool_cancel_report)
            break ;

        QString db_path = destination_db_file_list.at(db_count);

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(db_path);
        if(!dest_db.open())
        {
            recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
            continue ;
        }


        QStringList  total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, db_path, selected_tags_list, Q_FUNC_INFO);
        //========================================================//
        //QStringList INT_index_list;
        QStringList INT_index_list = total_record_list;
        //        for(qint64 rr = 0 ; rr < total_record_list.size() ; rr++)
        //        {
        //            if(bool_cancel_report)
        //                break ;

        //            if(rr == 0)
        //            {
        //                INT_index_list <<   "0";
        //                continue ;
        //            }

        //            if(rr % 50 == 0)
        //            {
        //                qint64 record_no_qint64 = QString(total_record_list.at(rr)).toLongLong() - 1;
        //                INT_index_list << QString::number(record_no_qint64);
        //            }
        //        }

        //        INT_index_list.removeDuplicates();
        //        for(qint64 tt = 0 ; tt < INT_index_list.size() ; tt++)
        //        {
        //            if(bool_cancel_report)
        //                break ;

        //            QString m_command  = select_column_command  +"(" + columns_list.join(" OR ") + ") AND (INT > " + INT_index_list.at(tt) + ") LIMIT 50";
        //            QStringList tagged_record_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(m_command,arg_values_list, 0, dest_db, Q_FUNC_INFO);
        //            if(tagged_record_list.size() == 0)
        //                continue ;


        //            struct_db_and_record_info st_db_and_record_obj;
        //            st_db_and_record_obj.databases_name_list  = QStringList(dest_db.databaseName());
        //            st_db_and_record_obj.record_list          = tagged_record_list;


        //            q_list_of_struct_db_and_record_info << st_db_and_record_obj;
        //        }
        QStringList tagged_record_list ;
        int tmp_count = 0;

        for(qint64 tt = 0 ; tt < INT_index_list.size() ; tt++)
        {
            if(bool_cancel_report)
                break ;


            tagged_record_list << INT_index_list.at(tt);

            if(tmp_count == 50)
            {
                struct_db_and_record_info st_db_and_record_obj;
                st_db_and_record_obj.databases_name_list  = QStringList(dest_db.databaseName());
                st_db_and_record_obj.record_list          = tagged_record_list;


                q_list_of_struct_db_and_record_info << st_db_and_record_obj;

                tmp_count = 0;
                tagged_record_list.clear();
            }

            tmp_count++;
        }

        if(tmp_count < 50 && !tagged_record_list.isEmpty())
        {
            struct_db_and_record_info st_db_and_record_obj;
            st_db_and_record_obj.databases_name_list  = QStringList(dest_db.databaseName());
            st_db_and_record_obj.record_list          = tagged_record_list;


            q_list_of_struct_db_and_record_info << st_db_and_record_obj;

            tmp_count = 0;
            tagged_record_list.clear();
        }

        //========================================================//
        dest_db.close();
    }



    ///--write data in file
    QStringList report_all_data_list;
    qint64 s_number = 0;
    for(int tt = 0 ; tt < q_list_of_struct_db_and_record_info.size() ; tt++)
    {

        if(bool_cancel_report)
            break ;

        struct_db_and_record_info obj = q_list_of_struct_db_and_record_info.at(tt);
        QStringList db_path_list    = obj.databases_name_list;
        QStringList record_count_list =  obj.record_list;


        for(int ii = 0 ; ii < db_path_list.size() ; ii++)
        {
            if(bool_cancel_report)
                break ;


            QString db_path = db_path_list.at(ii);

            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            dest_db.setDatabaseName(db_path);
            if(!dest_db.open())
            {
                recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
                continue ;
            }


            if(get_plugin_name() == QString(MACRO_Plugin_Name_File_System))
            {
                QString temp_db_dir_path = db_path;

                QFileInfo file_info(db_path);
                temp_db_dir_path.chop(file_info.fileName().size()); // "file_system.sqlite"

                QString mime_db_path = temp_db_dir_path + "mime.sqlite";
                QString hashes_db_path = temp_db_dir_path + "hashes.sqlite";
                QString sign_db_path = temp_db_dir_path + "signature.sqlite";
                QString exif_db_path  = temp_db_dir_path + "exif_metadata.sqlite";
                QString apple_meta_db_path = temp_db_dir_path + "apple_metadata.sqlite";

                QSqlQuery query_select(dest_db);
                QString sign_db_attach_commmand = QString("ATTACH DATABASE '%1' AS signatureDB")
                        .arg(QDir::toNativeSeparators(sign_db_path));
                query_select.exec(sign_db_attach_commmand);

                QString exif_db_attach_commmand = QString("ATTACH DATABASE '%1' AS exif_metadataDB")
                        .arg(QDir::toNativeSeparators(exif_db_path));
                query_select.exec(exif_db_attach_commmand);


                QString apple_meta_db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadataDB")
                        .arg(QDir::toNativeSeparators(apple_meta_db_path));
                query_select.exec(apple_meta_db_attach_commmand);

                QString hash_db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashesDB")
                        .arg(QDir::toNativeSeparators(hashes_db_path));
                query_select.exec(hash_db_attach_commmand);

                QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                        .arg(QDir::toNativeSeparators(mime_db_path));
                query_select.exec(mime_db_attach_commmand);


                for(qint64 ii = 0; ii < record_count_list.size(); ii++)
                {
                    if(bool_cancel_report)
                        break ;

                    if(s_number == 50)
                        s_number = 0;


                    QString fs_record_no = record_count_list.at(ii);
                    QString command = report_generic_function_obj->pub_get_fs_command(fs_record_no);
                    report_all_data_list << set_table_data(i_tab,command,table_name,tab_export_checked,is_export_available,bool_multiple_export,s_number,db_path , dest_db);

                    s_number++;
                }

                QString sign_db_detach_commmand = QString("DETACH DATABASE signatureDB");
                query_select.exec(sign_db_detach_commmand);
                QString exif_db_detach_commmand = QString("DETACH DATABASE exif_metadataDB");
                query_select.exec(exif_db_detach_commmand);
                QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
                query_select.exec(apple_meta_db_detach_commmand);
                QString hash_db_detach_commmand = QString("DETACH DATABASE hashesDB");
                query_select.exec(hash_db_detach_commmand);
                QString mime_db_detach_commmand = QString("DETACH DATABASE mimeDB");
                query_select.exec(mime_db_detach_commmand);

            }
            else
            {
                for(qint64 ii = 0; ii < record_count_list.size(); ii++)
                {

                    if(s_number == 50)
                        s_number = 0;

                    if(bool_cancel_report)
                        break ;

                    QStringList cols_list = rt_html_col_name_list_tablewidget.at(i_tab);
                    QString command;
                    if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
                    {
                        command = QString("select " + cols_list.join(",") + " , source_file, source_count_name  from '" + table_name + "' where INT=" + "'" + record_count_list.at(ii) + "'");
                    }
                    else
                    {
                        command = QString("select " + cols_list.join(",") + " , source_count_name  from '" + table_name + "' where INT=" + "'" + record_count_list.at(ii) + "'");
                    }
                    report_all_data_list << set_table_data(i_tab,command,table_name,tab_export_checked,is_export_available,bool_multiple_export,s_number,db_path , dest_db);
                    s_number++;
                }
            }
            dest_db.close();
        }
    }

    qint64 record_count = report_all_data_list.size();
    QString tab_name = tab_name_list_tablewidget.at(i_tab);
    QStringList list_artifacts_source = collect_artifacts_source_list(report_scope,QStringList(""));
    ///-Getting Page Count Accoring to the devision of the page
    int total_page = record_count/max_rec_per_pg;

    if(record_count % max_rec_per_pg != 0)
    {
        total_page = total_page + 1;
    }

    bool bool_is_examiner_notes_selected = false;

    if(tab_name == MACRO_Tag_Name_Examiner_Comments)
    {
        total_page = total_page + 1;

        bool_is_examiner_notes_selected = true;
    }

    QStringList data_per_record_added_list;
    for(int i_pg = 0; i_pg < total_page ; i_pg++)
    {

        if(bool_cancel_report)
            break ;

        for(qint64 ll = 0 ; ll <  data_per_record_added_list.size() ;ll++)
        {
            if(bool_cancel_report)
                break ;

            report_all_data_list.removeAll(data_per_record_added_list.at(ll));
        }


        if(bool_cancel_report)
            break ;

        tab_name = tab_name.replace(" ","_");

        QString index_file_path = path;

        if(index_file_path.endsWith("index.html"))
            index_file_path.chop(10);


        QString plugin_name = get_plugin_name();

        if(plugin_name.contains(" "))
            plugin_name.replace(" ","_");


        QString file_path = index_file_path + "Plugins/" + plugin_name + "/";

        QDir dir ;
        dir.mkpath(file_path);

        file_path = file_path + tab_name + QString::number(i_pg) + ".html";


        QFile file(file_path);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recon_static_functions::app_debug(" -file opened ----FAILED--- " + file.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" -Error" + file.errorString(), Q_FUNC_INFO);
            return;
        }


        QString report_data;

        int page_no = i_pg + 1;
        QString current_pg_of_total_pg;
        current_pg_of_total_pg = "Page " + QString::number(page_no) + " of " + QString::number(total_page);


        ///-- Report Top bar to middle NAV Bar.
        report_data += sub_report_topbar_to_tab_header(list_artifacts_source,tab_name,current_pg_of_total_pg);

        /// -- If record is over a particular limit and page is divided into multiple sub pages then provide its previous and next link
        ///** All Calculation for the record in one page,
        ///  and linkage of next and previos buttons.**/

        static int start_rec = 0;
        static int end_rec = 0;

        QString first_page = tab_name.replace(" ","_") + QString::number(0) + ".html";
        QString last_page = tab_name.replace(" ","_") + QString::number(total_page -1) + ".html";



        QString temp_1 ,temp_2;
        //-If First Page
        if(i_pg == 0)
        {
            start_rec = page_no; // * max_rec_per_pg; (1 * 20)
            end_rec = page_no * max_rec_per_pg;

            ///*-2. Next and previ linking work
            QString previous_link = "";
            QString next_link = tab_name.replace(" ","_") + QString::number(i_pg + 1) + ".html";
            report_data += report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page);

        }
        else if(page_no !=1 && page_no != total_page) //-If Middle pages
        {
            start_rec =  end_rec + 1;
            end_rec = start_rec + max_rec_per_pg -1;

            ///*-2. Next and previ linking work
            QString previous_link = tab_name.replace(" ","_") + QString::number(i_pg - 1) + ".html";
            QString next_link = tab_name.replace(" ","_") + QString::number(i_pg + 1) + ".html";

            temp_1 = previous_link;
            temp_2 = next_link;

            report_data += report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page);

        }
        else if((i_pg + 1) == total_page)
        {
            start_rec = end_rec + 1;
            end_rec = record_count;

            ///*-2. Next and previ linking work
            QString previous_link = tab_name.replace(" ","_") + QString::number(i_pg - 1) + ".html";
            QString next_link = "";
            report_data += report_html_template_plugin_advance_obj->report_set_next_previous_pager_buttons(previous_link,next_link,first_page,last_page);
        }


        ///-- Set Table Header
        QString is_export_available = table_export_available_list_tablewidget.at(i_tab);

        QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);

        bool bool_multiple_export = false;
        if(is_multiple_export == "2")
            bool_multiple_export = true;


        report_data += sub_report_create_tablewidget_table_headers(i_tab,is_export_available,is_multiple_export,tab_export_checked);


        for(int rr = 0 ; rr < report_all_data_list.size() ; rr++)
        {
            if(bool_cancel_report)
                break ;

            if(rr == 50)
                break;


            data_per_record_added_list << report_all_data_list.at(rr);

            report_data += report_all_data_list.at(rr);
        }


        if(bool_is_examiner_notes_selected)
        {
            if(report_generic_function_obj->pub_is_data_available_in_examiner_notes())
            {
                QString examiner_data = recon_helper_standard_obj->pub_fetch_examiner_comments_data();
                report_data += QString("<tr><td class=\"table_data-break\"> 1 </td> <td class=\"table_data-break\">" + examiner_data + "</td>");
            }
        }

        ///-- Table End function
        report_data += report_html_template_plugin_advance_obj->report_end();
        file.write(report_data.toLocal8Bit().data());
        file.flush();
        file.close();
    }
}

QString html_advance_report_module::sub_report_create_tablewidget_table_headers(int i_tab, QString is_export_available, QString is_multiple_export ,bool tab_export_checked)
{
    QString report_data;

    QStringList headers;

    QString header_str = table_report_html_header_list_tablewidget.at(i_tab);
    headers = header_str.split("::");
    headers.insert(0,"Sr. No.");

    QString header_str_col_width = table_report_header_column_width_html_list_tablewidget.at(i_tab);

    recon_static_functions::app_debug(" -header_str_col_width " + header_str_col_width,Q_FUNC_INFO);
    QStringList  headers_column_width = header_str_col_width.split("::");
    headers_column_width.insert(0,"5%");

    QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);


    bool virtual_export_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);

    if(is_multiple_export != "2")
    {
        if(!virtual_export_exist)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == false))
            {
                headers.removeLast();
                headers_column_width.removeLast();
            }
        }
    }




    report_data = report_html_template_plugin_advance_obj->report_set_table_header(headers,headers_column_width);

    return report_data;

}

QString html_advance_report_module::set_table_data(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int sr_no, QString destination_db_file, QSqlDatabase &dest_db)
{

    QString report_data;
    QSqlQuery query_select(dest_db);
    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" -query -----FAILED---" + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery(), Q_FUNC_INFO);
        return report_data;
    }

    ///9. --Insert Table Data list below
    QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);
    QStringList col_d_name_list = rt_html_d_name_list_tablewidget.at(i_tab);
    bool bool_virtual_datatype_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);


    int local_count = sr_no;
    while(query_select.next())
    {
        if(bool_cancel_report)
        {
            break;
        }

        local_count++;

        QString num = QString::number(local_count);
        QStringList tabledata;
        tabledata << num;

        emlx_source_file.clear();
        if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            emlx_source_file = query_select.value(12).toString();

        QString exported_file_detail;
        QString exported_file_preview;



        int report_preview = 0;

        if((is_export_available == QString("1")) && (tab_export_checked == true))
        {
            if(get_plugin_name() == MACRO_Plugin_Name_Snapshots)
            {
                get_export_detail_html_final(query_select.value("INT").toString(),table_name,bool_multiple_export,destination_db_file);
            }

            else
            {
                get_export_detail_html_final(query_select.value(0).toString(),table_name,bool_multiple_export,destination_db_file);

            }


            if(bool_multiple_export == true)
            {
                report_preview = 2;
            }
            else
            {
                report_preview = 1;
                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList().size() > 0)
                {
                    exported_file_detail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList().at(0);
                }
                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList().size() > 0)
                {
                    exported_file_preview = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList().at(0);
                }

            }
        }

        QString record_number;
        QString temp_data;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());
        if(report_generic_function_obj->pub_is_tagtype_other_than_screenshot_or_saved_graph(get_plugin_name()))
        {
            temp_data.append("<b>" + QString("Source Name") + " : </b>" + struct_info.complete_source_name + "<br>");

            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
            {
                temp_data.append("<b>" + QObject::tr(MACRO_Generic_Display_Name) + ":</b>" + QString("\t") + struct_info.source_name +"<br>");
            }
        }



        if(get_plugin_name() == MACRO_Plugin_Name_File_System)
        {
            QString title_value;
            QString title_name;

            ///NOTE::--do not change order--

            ///1.--start-- columns are kept in csv
            for(int x = 0 ; x < col_d_name_list.size() - 2; x++)///skip tag and notes column which are taken in step 3
            {
                QString our_value = query_select.value(x).toString();
                QString rpt_data_type = data_type_list.at(x);


                if(our_value.trimmed().isEmpty())
                    continue ;


                if(rpt_data_type.endsWith("Hide"))
                {
                    rpt_data_type.remove("Hide");
                    if(our_value.trimmed().isEmpty())
                        continue;
                }
                else if(rpt_data_type.startsWith("NewLine"))
                {
                    rpt_data_type.remove("NewLine");
                    temp_data.append("<br>");
                }

                ///Html Report*/
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = our_value;
                }
                title_value = our_value;
                title_name = "<b>" + col_d_name_list.at(x) + " : </b>";

                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else if(value == QString("0"))
                        title_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        title_value = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";


                    temp_data.append(title_name + title_value + "<br>");

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    if(!our_value.trimmed().isEmpty())
                    {
                        title_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);
                        temp_data.append(title_name + title_value + "<br>");
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    title_value.clear();
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
                            title_name = (QString("<b> <u>" + col_d_name_list.at(x) + "</u></b>"));
                            for(int k = 0 ; k< temp.size(); k++)
                            {
                                title_value.append("<br>");
                                title_value.append("<b>"+  QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                            }

                        }

                        temp_data.append(title_name + title_value + "<br>");

                    }
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                    {
                        title_value = our_value.toHtmlEscaped();

                    }
                    else
                    {
                        title_name.clear();
                    }

                }
                else
                {
                    if(!our_value.trimmed().isEmpty())
                    {
                        title_value = our_value.toHtmlEscaped();
                        temp_data.append(title_name + title_value + "<br>");
                    }

                }

            }
            ///1.--end--



            ///-----Bookmarked Apple Metadata
            QString bookmarked_apple_m_value = query_select.value(col_d_name_list.size()).toString();
            if(!bookmarked_apple_m_value.trimmed().isEmpty())
            {
                temp_data.append("<br><b>" + QString("Bookmarked Apple MetaData") + "</b><br>");
                if(!bookmarked_apple_m_value.startsWith("\n"))
                    bookmarked_apple_m_value.replace("\n" , "<br>");

                temp_data.append(bookmarked_apple_m_value + "<br>");
            }
            ///-----Bookmarked Apple Metadata

            ///-----Bookmarked Exif Data
            QString exif_data_value = query_select.value("bookmarked_exif_data").toString();
            if(!exif_data_value.trimmed().isEmpty())
            {
                temp_data.append("<br><b>" + QString("Bookmarked Exif Data") + "</b><br>");
                if(!exif_data_value.startsWith("\n"))
                    exif_data_value.replace("\n" , "<br>");


                temp_data.append(exif_data_value + "<br>");
            }
            ///-----Bookmarked Exif Data


            int counter = 1;
            ///----Apple metadata
            QStringList apple_m_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList).toStringList();
            bool bool_apple_metadata_value_exists = false;
            for(int i = 1 ; i <= apple_m_keys_list.size(); i++)
            {
                if(!query_select.value(i + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_apple_metadata_value_exists = true;
                    break;
                }
            }
            if(bool_apple_metadata_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_Apple_Metadata_Parent) + "</b><br>");


            for(int key_value_count = 0 ; key_value_count < apple_m_keys_list.size() ; key_value_count++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                if(!our_value.isEmpty())
                {
                    QString key_name = apple_m_keys_list.at(key_value_count);


                    QString qvalue = our_value;

                    title_name = "<b>" + key_name + " : </b>";
                    qvalue = our_value;

                    if(key_name.contains("kMDItemFSSize"))
                    {
                        QString value =  our_value;
                        if(value == QString(""))
                            qvalue = "";
                        else if(value == QString("0"))
                            qvalue =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                        else
                            qvalue = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";

                    }
                    else if(key_name.contains("Date"))
                    {
                        qvalue = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(qvalue, Q_FUNC_INFO);
                    }


                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }
                counter++;
            }
            ///----Apple metadata




            ///----Exif
            QStringList exif_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList).toStringList();
            bool bool_exif_value_exists = false;
            for(int i = 1 ; i <= exif_keys_list.size() ; i++)
            {
                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_exif_value_exists = true;
                    break;
                }
            }
            if(bool_exif_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_Metadata_TabName_Exif_Meatadta) + "</b><br>");


            for(int key_value_count = 0 ; key_value_count < exif_keys_list.size() ; key_value_count++)
            {
                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = exif_keys_list.at(key_value_count);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///----Exif




            ///------Mime types
            //            bool bool_mime_value_exists = false;
            //            for(int i = 0 ;i < report_generic_function_obj->html_report_file_system_Mime_key_list.size() ;i++)
            //            {
            //                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
            //                {
            //                    bool_mime_value_exists = true;
            //                    break;
            //                }
            //            }
            //            if(bool_mime_value_exists)
            //                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_MIME_Type) + "</b><br>");
            temp_data.append("<br>");
            for(int key_value_count = 0 ;key_value_count < report_generic_function_obj->html_report_file_system_Mime_key_list.size() ;key_value_count++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = report_generic_function_obj->html_report_file_system_Mime_key_list.at(key_value_count);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///------Mime types





            ///----signatures
            bool bool_signatures_value_exists = false;
            for(int i = 1 ; i <= report_generic_function_obj->html_report_file_system_signauture_key_list.size() ; i++)
            {
                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_signatures_value_exists = true;
                    break;
                }
            }
            if(bool_signatures_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_File_Signature_Parent) + "</b><br>");

            for(int sg = 0 ; sg < report_generic_function_obj->html_report_file_system_signauture_key_list.size() ; sg++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = report_generic_function_obj->html_report_file_system_signauture_key_list.at(sg);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///----signatures



            ///2.--start-- tags and Notes columns in report and also kept in csv
            temp_data.append("<br>");
            for(int x = 2 ; x > 0; x--)
            {

                QString rpt_data_type = data_type_list.at(col_d_name_list.size() - x);
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
                {

                    QString tag_value = query_select.value(col_d_name_list.size() - 2).toString();

                    if(tag_value.trimmed().isEmpty())
                        continue ;


                    QString tag_text = "";
                    QStringList list_tags = tag_value.split(",", Qt::SkipEmptyParts);

                    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                    {
                        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                    }

                    if(tag_text.endsWith(","))
                        tag_text.chop(1);

                    title_value =  tag_text ;
                    temp_data.append( "<b> Tag : </b>" + title_value + "<br>");
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString)
                {
                    QString notes_value = query_select.value(col_d_name_list.size() - 1).toString();
                    if(notes_value.trimmed().isEmpty())
                        continue ;

                    title_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";
                    temp_data.append( "<b> Examiner Notes : </b>" + title_value + "<br>");
                }

            }

            ///2 --end-- tags and Notes column in report and also kept in csv

        }
        else
        {
            QString emlx_source_path_value;
            QString emlx_source_path_col_name;
            if(get_plugin_name() == QString(MACRO_Plugin_Name_Email_Parser))
            {
                QString emlx_file_name = QFileInfo(emlx_source_file).fileName();
                if(emlx_file_name.endsWith(".emlx") || emlx_file_name.endsWith(".eml"))
                {
                    QString emlx_file_path = "../../Files/" + emlx_file_name;
                    QString hyperlink = QString("<a href=") + "\"" + emlx_source_file + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + emlx_file_path + "'"
                            + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");

                    emlx_source_path_value = hyperlink + emlx_source_file + "</a>";
                    emlx_source_path_col_name = "<b> Source File : </b>";
                }
            }

            for(int y = 0 ; y < col_d_name_list.size() ; y++)
            {

                int ref_index = y;
                QString title_value;
                QString title_name;


                QString rpt_data_type = data_type_list.at(ref_index);
                QString rpt_data_type_for_tmpuse = rpt_data_type;

                QString our_column = col_d_name_list.at(ref_index);
                QString our_value = query_select.value(ref_index).toString();
                if(our_value.trimmed().isEmpty())
                    continue ;

                title_name = "<b>" + col_d_name_list.at(ref_index) + " : </b>";


                bool colmn_atr =  report_generic_function_obj->pub_is_column_of_apple_attribute(our_column);
                // hide extended attribute column which has no value
                if(colmn_atr)
                {
                    if(our_value.trimmed() == QString(""))
                        continue;
                }



                ///------------ Use Format as per CSV DataType
                if(rpt_data_type_for_tmpuse.endsWith("Hide"))
                {
                    rpt_data_type.remove("Hide");
                    if(our_value.trimmed().isEmpty())
                        continue;

                    if(rpt_data_type.startsWith("NewLine"))
                        rpt_data_type.remove("NewLine");

                }
                else if(rpt_data_type_for_tmpuse.startsWith("NewLine"))
                {
                    rpt_data_type.remove("NewLine");
                    temp_data.append("<br>");

                }
                else if(rpt_data_type_for_tmpuse.contains("_Person", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(7);
                }
                else if(rpt_data_type_for_tmpuse.contains("_Icon", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(5);
                }
                else if(rpt_data_type_for_tmpuse.contains("_WebIcon", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(QString("_WebIcon").size());
                }
                ///------------ Use Format as per CSV DataType


                ///Html Report*/
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = our_value;
                    title_value = record_number;
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString)
                {
                    QString extended_attr = our_value;
                    if(extended_attr.trimmed() == QString(""))
                        continue;

                    extended_attr = extended_attr.trimmed();
                    extended_attr.replace("\n   ","<br>&nbsp;");
                    extended_attr.replace("\n","<br>&nbsp;");
                    if(extended_attr != "")
                    {
                        title_name = "";
                        title_value = QString("<b><u>" + QObject::tr("Bookmarked Apple Metadata") + ": </u></b><br>" + "\t" + extended_attr + "<br>" );
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    title_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else if(value == QString("0"))
                        title_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        title_value = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    title_value.clear();
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
                            title_name = (QString("<b> <u>" + our_column + "</u></b>"));
                            for(int k = 0 ; k< temp.size(); k++)
                            {
                                title_value.append("<br>");
                                title_value.append("<b>"+  QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                            }

                        }
                    }
                    else
                    {
                        //continue;
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
                {

                    QString str = our_value;

                    if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");

                    }
                    else if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                    {
                        str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, "<br>");
                    }
                    str = str.replace("|","<br>");

                    title_value = str;

                    title_value.prepend("<br>");
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
                {
                    QString str = our_value;

                    QStringList timestamps_splitList;

                    timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                    title_value.clear();
                    for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                    {
                        title_value.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");

                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
                {
                    if(!our_value.startsWith("/"))
                        our_value = our_value.prepend("/");

                    our_value.replace(";/","<br>/");

                    our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>/");

                    title_value = our_value;
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
                {
                    QString notes_value = query_select.value(ref_index).toString();
                    if(notes_value.trimmed().isEmpty())
                    {
                        title_name.clear();
                        continue ;
                    }
                    else
                        title_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECON_VIEWERS_TAG_EXPORTED_DATA_QString)
                {
                    if(get_plugin_name() == QString(MACRO_Plugin_Name_Hex_Viewer) || get_plugin_name() == QString(MACRO_Plugin_Name_Registry_Viewer))
                        title_value = report_generic_function_obj->pub_get_hex_viewer_format_for_html_report(our_value);
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_QString)
                {
                    title_value = "<font color = \"red\">" + our_value.toHtmlEscaped() + "</font>";
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
                {

                    temp_data.append("<br>");
                    QString tag_text = "";
                    QStringList list_tags = our_value.split(",", Qt::SkipEmptyParts);
                    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                    {
                        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                    }

                    if(tag_text.endsWith(","))
                        tag_text.chop(1);

                    title_value =  tag_text ;
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString)
                {
                    if(bool_virtual_datatype_exist)
                    {
                        QString virtual_image_file_path = report_generic_function_obj->pub_export_virtual_file(our_value , get_plugin_name() , destination_file_location , reports_dir_path);
                        exported_file_preview = get_virtual_preview_file(virtual_image_file_path);
                    }
                    title_name.clear();

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                    {
                        title_value = our_value.toHtmlEscaped();

                    }
                    else
                    {
                        title_name.clear();
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_QString)
                {
                    if(!our_value.startsWith("\n"))
                        our_value.replace("\n" , "<br>");

                    title_value.append("<br>" + our_value + "<br>");
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString)
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else
                    {
                        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                        QString str_val = our_value;
                        str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                        title_value = str_val.trimmed();
                    }
                }
                else
                    title_value = our_value.toHtmlEscaped();

                title_value.replace("\n","<br>");
                ////start -----append  column name and value
                temp_data.append(title_name + title_value + "<br>");
                ////start -----append  column name and value
            }

            temp_data.append("<br>" + emlx_source_path_col_name + emlx_source_path_value + "<br>");
            emlx_source_path_value.clear();
            emlx_source_path_col_name.clear();

            if(struct_info.os_scheme_display == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
            {
                QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
                if(!custom_col_name_list.isEmpty())
                {
                    if(is_export_available == QString::number(1) && tab_export_checked == true)
                    {
                        QString extended_list = recon_helper_standard_obj->get_apple_metadata_key_value_from_result(record_number, table_name, destination_db_file, Q_FUNC_INFO);

                        if(!extended_list.isEmpty())
                        {
                            temp_data += "<br><br><b><u><i>More Apple Metadata</u></i></b>"+ extended_list;
                        }
                    }
                }
            }


        }



        ///1.--------------
        if(!bool_multiple_export)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == true))
            {
                temp_data.append(exported_file_detail);
            }
        }
        ///1.--------------




        ///2.--------------
        tabledata << temp_data;
        ///2.--------------




        ///3.--------------
        if(bool_virtual_datatype_exist)
        {
            tabledata << exported_file_preview;
        }
        ///3.--------------




        ///--------------report data
        if(bool_multiple_export != true)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == true))
            {
                tabledata  << exported_file_preview;
            }
            report_data += report_html_template_plugin_advance_obj->report_insert_table_data(tabledata,report_preview);

        }
        else if(bool_multiple_export == true && (is_export_available == QString("1")) && (tab_export_checked == true))
        {
            QString table_crt;

            QStringList multiple_export_filedetail ;
            multiple_export_filedetail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList();

            QStringList multiple_export_flepreview;
            multiple_export_flepreview = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList();

            for(int pp = 0; pp < multiple_export_filedetail.size(); pp++)
            {

                if(multiple_export_filedetail.at(pp).trimmed() == "")
                    continue;

                table_crt +=
                        "<table  border = \"0\" border-color = \"RED\" width = \"100%\">"
                        "<tr>"
                        "<td width = \"80%\">" + multiple_export_filedetail.at(pp)  + "</td>"
                                                                                      "<td align=\"center\">" + multiple_export_flepreview.at(pp) + "</td>"
                                                                                                                                                    "</tr>"                                                                                                                         "</table>";
            }


            //            if(table_crt.trimmed().isEmpty())
            //                continue ;

            QString temp_data;
            for(int i = 0; i < tabledata.size(); i++)
            {
                if(!bool_virtual_datatype_exist)
                {
                    if(i == tabledata.size() - 1)
                        temp_data +=  QString("<td>" + tabledata.at(i) + table_crt + "</td>");
                    else
                        temp_data +=  QString("<td>" + tabledata.at(i) + "</td>");

                }
                else
                {
                    if(i == 0 || i == tabledata.size() - 1)
                    {
                        temp_data +=  QString("<td>" + tabledata.at(i) + "</td>");
                    }
                    else
                        temp_data +=  QString("<td>" + tabledata.at(i) + table_crt + "</td>");
                }
            }
            temp_data += QString("<tr>");
            report_data += temp_data;
        }
        else
        {
            report_data += report_html_template_plugin_advance_obj->report_insert_table_data(tabledata,report_preview);
        }
        ///--------------report data

    }

    return report_data;
}

QString html_advance_report_module::set_stack_data(QString command, int textedit_col_pos, QString report_type, QString report_scope, int sr_no, QString destination_db_file)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    QString report_data;
    if(bool_cancel_report)
        return report_data;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db;
    dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" -db open -----FAILED---" + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + dest_db.lastError().text(), Q_FUNC_INFO);

        return report_data;
    }

    QSqlQuery query_select(dest_db);

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" ----FAILED--- query" + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery(), Q_FUNC_INFO);

        return report_data;
    }

    int local_count = sr_no;

    local_count++;
    QString num = QString::number(local_count);
    QStringList tabledata;
    tabledata << num;

    QString file_path;

    file_path = destination_file_location + "/screenshot.png";
    QFileInfo info(file_path);
    if(!info.exists())
        file_path = destination_file_location + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + "screenshot.png";


    // right now this is only for first tab(Screenshot Plugin), In future if we want to use tab 2 then do code for it
    QDir dir;
    dir.mkpath(reports_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images);
    QString preview_file_path = reports_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + "screenshot.png";

    QFile::copy(file_path,preview_file_path);
    QString image = QString("<img src=") + "\"" + preview_file_path + "\"" + " alt=\"No Preview\"" + " width=\"500\" height=\"350\">"; //140 150 2880 × 900
    QString hyperlink = QString("<a href=") + "\"" + preview_file_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + preview_file_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");

    QString temp_detail = hyperlink + image;

    tabledata << temp_detail;

    report_data += report_html_template_plugin_advance_obj->report_insert_table_data(tabledata,0);

    dest_db.close();

    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);

    return report_data;
}

QString html_advance_report_module::sub_report_create_headers(int i_tab, QStringList header_name_list, QStringList header_width_list)
{
    QString report_data;

    QStringList headers;
    QString header_str = header_name_list.at(i_tab);
    headers = header_str.split("::");
    headers.insert(0,"Sr. No.");

    QString header_str_col_width = header_width_list.at(i_tab);
    QStringList  headers_column_width = header_str_col_width.split("::");
    headers_column_width.insert(0,"5%");

    report_data = report_html_template_plugin_advance_obj->report_set_table_header(headers,headers_column_width);

    return report_data;
}

QString html_advance_report_module::get_virtual_preview_file(QString virtual_file_path)
{

    QString  resource_dir = "../../resources/";
    QFileInfo info66(virtual_file_path);
    QString new_relative_path = QString("../../") + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images) + info66.fileName();


    QString image;
    QString hyperlink;
    QString virtual_preview;
    if(recon_helper_standard_obj->check_is_it_image_file(virtual_file_path,virtual_file_path, Q_FUNC_INFO))
    {
        image = QString("<img src=") + "\"" + new_relative_path + "\"" + " alt=\"No Preview\"" + " width=\"142\" height=\"150\">";
        hyperlink = QString("<a href=") + "\"" + new_relative_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
        virtual_preview =  hyperlink + image + "</a>";
    }
    else
    {
        image = QString("<a href=") + "\"" + new_relative_path + "\"" + QString(" target=\"_blank\"") + " >" + "<img src=" + "\"" + resource_dir + "open_file_link.png\"" + "style=\" width=\"90\" height=\"100\"\">" + "</a>";
        hyperlink = QString("<a href=") + "\"" + new_relative_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
        virtual_preview =  hyperlink + image + "</a>";
    }


    return virtual_preview;

}

QString html_advance_report_module::set_textedit_data(QString command, int textedit_col_pos, QString report_type, QString report_scope, int sr_no, QString destination_db_file)
{
    recon_static_functions::app_debug(" -Start" , Q_FUNC_INFO);

    QString report_data;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db;
    dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" -db open ---FAILED--- " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error-" + dest_db.lastError().text() , Q_FUNC_INFO);

        return report_data;
    }

    QSqlQuery query_select(dest_db);

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" -Select query ---FAILED--- " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery(), Q_FUNC_INFO);
        return report_data;
    }

    int local_count = sr_no;

    local_count++;
    QString num = QString::number(local_count);
    QStringList tabledata;
    tabledata << num;

    QString data;

    while(query_select.next())
    {
        if(bool_cancel_report)
            break ;
        data.append(query_select.value(textedit_col_pos).toString() + "<br>");
    }

    tabledata << data;

    report_data += report_html_template_plugin_advance_obj->report_insert_table_data(tabledata,0);

    dest_db.close();

    recon_static_functions::app_debug(" -Ends-", Q_FUNC_INFO);

    return report_data;
}

void html_advance_report_module::get_export_detail_html_final(QString column, QString tablename, bool bool_multiple_export, QString destination_db_file)
{
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db;
    dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" -dest_db open ----FAILED---" + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + dest_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_export_detail(dest_db);
    QString cmnd;

    QStringList  exp_detail_list , exp_preview_list ;

    cmnd = QString("select exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time,filepath,exp_LINKFILENAME from exportfilelist where (table_name = '") + tablename + "' AND record = " + column + ")";

    if(!query_export_detail.exec(cmnd))
    {
        recon_static_functions::app_debug(" -query ---FAILED-- " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error" + query_export_detail.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(query_export_detail.executedQuery(), Q_FUNC_INFO);
        return;
    }

    while(query_export_detail.next())
    {

        QString exp_detail;
        QString exp_preview;

        QString relative_path = query_export_detail.value(0).toString();
        relative_path = relative_path.trimmed();

        QString org_filepath = query_export_detail.value(5).toString();

        QString link_file = query_export_detail.value(6).toString();

        QString relative_softlink_path;
        if(!link_file.trimmed().isEmpty())
            relative_softlink_path = "./files/Links/" + query_export_detail.value(6).toString();


        QFileInfo info8(org_filepath);
        QString original_filename = info8.fileName();

        if(relative_path == QString(""))
        {
            if(org_filepath.trimmed() == "")
            {
                //                exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                //                        + QString("<b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");


                if(get_plugin_name() == QString(MACRO_Plugin_Name_Email_Parser))
                {
                    ////Note::For 'EMLX Files' Feature 'Auto Export' is Enabled
                    ///To prevent unnecessary text "File does not exist" if In actual record does not contains attachment file
                    exp_detail = "";
                }
                else
                {
                    exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                            + QString("<b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");
                }
            }
            else
            {
                exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                        + "<b>"+  QObject::tr("Original File") + ": </b>" +"\t"+ original_filename +"\t"
                        + QString("<br><b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");
            }

            exp_detail_list << exp_detail;
            exp_preview_list << exp_preview;

        }
        else
        {
            QString image, hyperlink;
            QString new_relativepath;
            if(!link_file.trimmed().isEmpty())
            {
                new_relativepath = relative_softlink_path;
            }
            else
            {
                new_relativepath = relative_path;
            }

            QString resource_dir;

            new_relativepath.prepend("../.");
            resource_dir = "../../resources/";


            QString file_name = "";

            if(!link_file.trimmed().isEmpty())
            {
                file_name = relative_softlink_path;

            }
            else
            {
                file_name = relative_path;
            }

            file_name = file_name.remove("./files/");

            new_relativepath = recon_helper_standard_obj->pub_get_html_formated_filename(new_relativepath,Q_FUNC_INFO);

            QString filename_with_extn = new_relativepath;

            if(recon_helper_standard_obj->check_is_it_image_file(org_filepath,filename_with_extn, Q_FUNC_INFO))
            {
                if(recon_helper_standard_obj->is_special_file_for_preview(org_filepath,filename_with_extn , Q_FUNC_INFO))
                {
                    QString image_raw_data_str = recon_helper_standard_obj->pub_get_raw_data_for_image_file_preview(org_filepath);

                    image = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\"")
                            + " >" + QString("<img src=data:image/png;base64,") + image_raw_data_str  + ">";

                    hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                }
                else
                {
                    image = QString("<img src=") + "\"" + new_relativepath + "\"" + " alt=\"No Preview\"" + " width=\"142\" height=\"150\">";
                    hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                }
            }
            else
            {
                image = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\"") + " >" + "<img src=" + "\"" + resource_dir + "open_file_link.png\"" + "style=\" width=\"90\" height=\"100\"\">" + "</a>";
            }

            QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
            QString numeric_timezone_value;
            if(machine_timezone_string.contains("GMT"))
            {
                QStringList tmp_list = machine_timezone_string.split("GMT");
                numeric_timezone_value = tmp_list.at(1);
            }

            exp_detail = QString("<br><b> <u> <i>"+  QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                    + "<b>"+  QObject::tr("Original File") + ": </b>" +"\t"+ original_filename +"\t<br>"
                    + "<b>"+  QObject::tr("Exported File") + ": </b>" +"\t"+ relative_path +"\t<br>"
                    + "<b>"+  QObject::tr("Size") + ": </b>" + recon_static_functions::human_readable_size(query_export_detail.value(1).toString(), Q_FUNC_INFO)
                    + " (" + query_export_detail.value(1).toString() + QObject::tr(" bytes") + ")" + "\t<br>"
                    + "<b>"+  QObject::tr("MD5") + ": </b>" +"\t"+ query_export_detail.value(2).toString() +"<br>"
                    +"<b>"+  QObject::tr("SHA1") + ": </b>" +"\t"+ query_export_detail.value(3).toString() +"\t<br>"
                    + "<b>"+  QObject::tr("Export Time") + ": </b>" +"\t"+ QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString()) + "&nbsp;" + numeric_timezone_value + "<br><br>";

            exp_preview =  hyperlink + image + "</a>";

            exp_detail_list << exp_detail;

            exp_preview_list << exp_preview;

        }

    }


    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList , exp_detail_list);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList , exp_preview_list);

    dest_db.close();

    return ;
}

QString html_advance_report_module::export_virtual_file(QString file_path)
{
    if(file_path.trimmed() == QString(""))
        return "";


    QFileInfo info(file_path);
    QString file_name = info.fileName();

    if((file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images))) || (file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
            || (file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
        file_path =  destination_file_location + file_path.remove(0,1);

    //    QString source_file =  destination_file_location + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + file_name;

    //    if(!QFile::exists(source_file))
    //        source_file =  destination_file_location + "Maps/" + file_name;

    QDir dirp;
    dirp.mkpath(reports_dir_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images));

    QString dest_loc = reports_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images;
    file_name = recon_static_functions::get_available_filename(file_name,dest_loc, Q_FUNC_INFO);
    QString dest_file = dest_loc + file_name;


    QFile file;
    //    if(!QFile(source_file).exists())
    //    {
    //        file.copy(file_path,dest_file);
    //    }
    //    else
    //    {
    //        file.copy(source_file,dest_file);
    //    }

    file.copy(file_path,dest_file);

    return dest_file;
}

