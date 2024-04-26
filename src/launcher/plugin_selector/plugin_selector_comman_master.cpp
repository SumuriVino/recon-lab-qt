#include "plugin_selector.h"
#include "ui_plugin_selector.h"


void plugin_selector::pub_initialise_multi_os_plugins_from_recon_configuration() //Display multi os plugins from recon config database
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";

    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
        return;
    }

    initialise_plugins_for_recon_config_and_new_case_launcher(destination_db_file);
    set_hide_widgets(false);

    //-Show First Display.
    ui->stackedWidget->setCurrentIndex(0);
    set_destination_db_file_path_for_multi_os();

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::initialise_plugins_for_recon_config_and_new_case_launcher(QString destination_db_file) // select all the plugin related info from database (recon_configuration.db)
{ //like their os and types etc
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QList<struct_report_plugin_info> list_plugins_info;
    QStringList list_plugin_parents;

    ///-Get Plugins Info...
    QSqlQuery query_plugin(destination_db);
    QString command_plugin = QString("select name,default_selected,supported_os_scheme_display_list from plugins ORDER BY name");

    query_plugin.prepare(command_plugin);
    if(!query_plugin.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_plugin,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_plugin.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QStringList run_plugin_withtab_list;
    if(launcher_type != MACRO_Launcher_Plugin_Selection_New_Case)
    {
        run_plugin_withtab_list = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);
    }


    while(query_plugin.next())
    {
        struct_report_plugin_info info;
        QString plugin_name = query_plugin.value(0).toString();
        info.plugin_name = plugin_name;

        info.is_selected = false;
        if(query_plugin.value(1).toString() == "1")
        {
            info.is_selected = true;
            emit signal_plugin_selected(true);
        }

        info.parent_name = recon_helper_standard_obj->get_category_of_plugin_by_dbreference(destination_db,plugin_name,Q_FUNC_INFO);
        list_plugin_parents << info.parent_name;

        //-Tabs List.
        info.plugin_tab_name_list = recon_helper_standard_obj->get_tablist_of_plugin_from_case_configuration_by_dbreference(destination_db, plugin_name,Q_FUNC_INFO);

        //-Seleted Tabs List.
        info.selected_plugins_tabs_list = recon_static_functions::get_associated_tabs(info.plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);

        info.supported_os_scheme_display_QStringList = query_plugin.value(2).toString().split(MACRO_RECON_Splitter_5_resolution_scope, Qt::SkipEmptyParts);

        list_plugins_info << info;
    }

    destination_db.close();

    list_plugin_parents.sort();
    list_plugin_parents.prepend(supported_parent_gtakeoutOS);
    list_plugin_parents.prepend(supported_parent_androidOS);
    list_plugin_parents.prepend(supported_parent_winOS);
    list_plugin_parents.prepend(supported_parent_iOS);
    list_plugin_parents.prepend(supported_parent_macOS);
    list_plugin_parents.prepend(M_VAR_PLUGIN_Parent_All_Plugins_QString);

    list_plugins_info_common = list_plugins_info;
    list_plugin_parents_common = list_plugin_parents;

    fill_template_combobox();
    display_plugins_tree_for_multiple_os(list_plugins_info);
    display_parent_category_list_for_multiple_os(list_plugin_parents);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::display_plugins_tree_for_multiple_os(QList<struct_report_plugin_info> &list_plugins_info) // display plugins tree for multiple operating system
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    ui->treeWidget_plugins->clear();
    ui->treeWidget_plugins->hideColumn(enum_plugin_tree_column_export_available);
    ui->treeWidget_plugins->hideColumn(enum_plugin_tree_column_runned_plugin);

    for(int i = 0; i < list_plugins_info.size(); i++)
    {
        struct_report_plugin_info info = list_plugins_info.at(i);
        QTreeWidgetItem *item_plugin_topleve_item = new QTreeWidgetItem;
        item_plugin_topleve_item->setText(enum_plugin_tree_column_plugin_translated_name,global_lang_tr_obj.get_translated_string(info.plugin_name));
        item_plugin_topleve_item->setText(enum_plugin_tree_column_plugin_original_name,info.plugin_name);
        QString plugin_name = info.plugin_name;

        QString icon_path= "../icons/plugins/" + plugin_name.replace(" ","_") + ".png";

        item_plugin_topleve_item->setIcon(enum_plugin_tree_column_plugin_translated_name,QIcon(icon_path));
        item_plugin_topleve_item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);
        if(info.is_selected)
        {
            item_plugin_topleve_item->setIcon(enum_plugin_tree_column_runned_plugin,QIcon("../icons/active.png"));

            if(launcher_type == MACRO_Launcher_Plugin_Selection_RECON_Configuration || launcher_type == MACRO_Launcher_Plugin_Selection_New_Case)
                item_plugin_topleve_item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
        }

        //-Export Available
        if(list_plugins_info.at(i).is_export_available)
        {
            item_plugin_topleve_item->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
        }
        else
        {
            item_plugin_topleve_item->setText(enum_plugin_tree_column_export_available,M_VAR_FUNCTIONALITY_Not_Available_QString);
        }

        //- OS Supported Work.
        QStringList supptd_os_schm_dsply_list = list_plugins_info.at(i).supported_os_scheme_display_QStringList;

        for(int li = 0; li < supptd_os_schm_dsply_list.size(); li++)
        {
            QString icn_path= QString("../icons/supported.png");
            QIcon support_icn(icn_path);


            if(supptd_os_schm_dsply_list.at(li) == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
                item_plugin_topleve_item->setIcon(enum_plugin_tree_column_macOS,support_icn);
            else if(supptd_os_schm_dsply_list.at(li) == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                item_plugin_topleve_item->setIcon(enum_plugin_tree_column_iOS,support_icn);
            else if(supptd_os_schm_dsply_list.at(li) == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
                item_plugin_topleve_item->setIcon(enum_plugin_tree_column_winOS,support_icn);
            else if(supptd_os_schm_dsply_list.at(li) == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display)
                item_plugin_topleve_item->setIcon(enum_plugin_tree_column_gTakeoutOS,support_icn);
            else if(supptd_os_schm_dsply_list.at(li) == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                item_plugin_topleve_item->setIcon(enum_plugin_tree_column_androidOS,support_icn);
        }

        QStringList list_tabs_export = list_plugins_info.at(i).export_available_tabs_list;

        for(int ch = 0; ch < info.plugin_tab_name_list.size(); ch++)
        {
            QTreeWidgetItem *item_child = new QTreeWidgetItem;
            item_child->setText(enum_plugin_tree_column_plugin_translated_name,global_lang_tr_obj.get_translated_string(info.plugin_tab_name_list.at(ch)));
            item_child->setText(enum_plugin_tree_column_plugin_original_name,info.plugin_tab_name_list.at(ch));
            item_child->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);

            if(info.selected_plugins_tabs_list.size() > 0)
            {
                if(info.selected_plugins_tabs_list.contains(info.plugin_tab_name_list.at(ch)))
                {
                    if(launcher_type == MACRO_Launcher_Plugin_Selection_RECON_Configuration || launcher_type == MACRO_Launcher_Plugin_Selection_New_Case)
                    {
                        item_child->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);

                        bool_plugin_is_checked = true;
                    }
                    else{
                        item_child->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);
                    }
                }
            }

            if(list_tabs_export.contains(info.plugin_tab_name_list.at(ch)))
            {
                item_child->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
            }
            else
            {
                item_child->setText(enum_plugin_tree_column_export_available,M_VAR_FUNCTIONALITY_Not_Available_QString);
            }

            item_plugin_topleve_item->addChild(item_child);
        }

        ui->treeWidget_plugins->addTopLevelItem(item_plugin_topleve_item);
    }

    if(launcher_type == MACRO_Launcher_Plugin_Selection_Run_Plugin)
    {
        ui->treeWidget_plugins->showColumn(enum_plugin_tree_column_runned_plugin);
    }
    else if(launcher_type == MACRO_Launcher_Plugin_Selection_G_Report)
    {
        ui->treeWidget_plugins->showColumn(enum_plugin_tree_column_export_available);
    }

    QTreeWidgetItem *item_header_macos = new QTreeWidgetItem();
    item_header_macos->setIcon(enum_plugin_tree_column_macOS,QIcon("../icons/os/macOS.png"));
    item_header_macos->setIcon(enum_plugin_tree_column_iOS,QIcon("../icons/os/iOS.png"));
    item_header_macos->setIcon(enum_plugin_tree_column_winOS,QIcon("../icons/os/winOS.png"));
    item_header_macos->setIcon(enum_plugin_tree_column_gTakeoutOS,QIcon("../icons/os/google_takeoutOS.png"));
    item_header_macos->setIcon(enum_plugin_tree_column_androidOS,QIcon("../icons/os/androidOS.png"));

    ui->treeWidget_plugins->setHeaderItem(item_header_macos);


    QStringList list_labels;
    list_labels << global_lang_tr_obj.get_translated_string("Plugins") << "" << " " << global_lang_tr_obj.get_translated_string("Export") << " " <<" " <<" " << " " << "";
    ui->treeWidget_plugins->setHeaderLabels(list_labels);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_plugin_translated_name,300);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_runned_plugin,40);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_export_available,100);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_macOS,30);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_iOS,30);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_winOS,30);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_gTakeoutOS,30);
    ui->treeWidget_plugins->setColumnWidth(enum_plugin_tree_column_androidOS,30);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::display_parent_category_list_for_multiple_os( QStringList &list_plugin_parents) // display parent category list of multiple operating system
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QStringList list_parents = list_plugin_parents;
    list_parents.removeDuplicates();

    while(ui->tableWidget_parent_list->rowCount())
        ui->tableWidget_parent_list->removeRow(0);

    for(int ii = 0; ii < list_parents.size(); ii++)
    {
        QTableWidgetItem *item_translated = new QTableWidgetItem;
        QTableWidgetItem *item_original = new QTableWidgetItem;
        item_translated->setText(global_lang_tr_obj.get_translated_string(list_parents.at(ii)));
        item_original->setText(list_parents.at(ii));

        if(list_parents.at(ii) == M_VAR_PLUGIN_Parent_All_Plugins_QString)
        {
            item_translated->setBackground(QColor("#434343"));
            item_translated->setForeground(QColor("#ffffff"));
        }
        else if(list_parents.at(ii) == supported_parent_macOS || list_parents.at(ii) == supported_parent_iOS
                || list_parents.at(ii) == supported_parent_winOS || list_parents.at(ii) == supported_parent_gtakeoutOS
                || list_parents.at(ii) == supported_parent_androidOS)
        {
            item_translated->setBackground(QColor("#bfbbbb"));
            item_translated->setForeground(QColor("#ffffff"));
        }

        ui->tableWidget_parent_list->insertRow(ii);
        ui->tableWidget_parent_list->setItem(ii,enum_parent_tablewidget_column_translated_name,item_translated);
        ui->tableWidget_parent_list->setItem(ii,enum_parent_tablewidget_column_original_name,item_original);
        ui->tableWidget_parent_list->setRowHeight(ii,26);
    }

    ui->tableWidget_parent_list->setCurrentCell(0,enum_parent_tablewidget_column_translated_name);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

}

void plugin_selector::set_destination_db_file_path_for_multi_os() //Database file path for multi os
{
    if(launcher_type == MACRO_Launcher_Plugin_Selection_RECON_Configuration)
    {
        destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";
    }
    else if(launcher_type == MACRO_Launcher_Plugin_Selection_New_Case)
    {
        destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";
    }
    if(launcher_type == MACRO_Launcher_Plugin_Selection_New_Case || launcher_type == MACRO_Launcher_Plugin_Selection_G_Report
            || launcher_type == MACRO_Launcher_Plugin_Selection_G_Timeline  || launcher_type == MACRO_Launcher_Plugin_Selection_K_Search)
    {
        destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + ""+ "case_configuration.sqlite";
    }
}


void plugin_selector::pub_update_default_multi_OS_plugins_db_for_recon_configuration() //update status in db after seelcting plugins to process
{
    recon_static_functions::app_debug(" -Start" ,Q_FUNC_INFO);

    if(!bool_plugins_tree_updated)
        return;


    QString destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";


    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
        return;
    }

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    int toplevel_item_count = 0;

    toplevel_item_count =  ui->treeWidget_plugins->topLevelItemCount();

    for(int i_toplevel = 0; i_toplevel < toplevel_item_count; i_toplevel++)
    {
        QString plugin =  "";
        int check_state = 0;

        plugin =  ui->treeWidget_plugins->topLevelItem(i_toplevel)->text(enum_plugin_tree_column_plugin_translated_name).trimmed();

        check_state = ui->treeWidget_plugins->topLevelItem(i_toplevel)->checkState(enum_plugin_tree_column_plugin_translated_name);

        QString command_plugin = "select INT from plugins where name = '" + plugin + "'";
        QString plugin_INT = recon_helper_standard_obj->get_string_from_db_by_dbreference(command_plugin,0,destination_db,Q_FUNC_INFO);

        QString command;
        if(check_state == Qt::Checked)
            command = "update plugins set default_selected = 1 where name = '" + plugin + "'";
        else
            command = "update plugins set default_selected = 0 where name = '" + plugin + "'";

        if(!recon_helper_standard_obj->execute_db_command_by_dbreference(command,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" plugin query --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        }

        int toplevel_item_child_count = 0;

        toplevel_item_child_count =  ui->treeWidget_plugins->topLevelItem(i_toplevel)->childCount();

        for(int j_child = 0; j_child < toplevel_item_child_count; j_child++)
        {
            QString tab_name = "";
            int check_state_child = 0;

            tab_name = ui->treeWidget_plugins->topLevelItem(i_toplevel)->child(j_child)->text(enum_plugin_tree_column_plugin_translated_name).trimmed();
            check_state_child = ui->treeWidget_plugins->topLevelItem(i_toplevel)->child(j_child)->checkState(enum_plugin_tree_column_plugin_translated_name);

            QString command_tabs;

            if(check_state_child == Qt::Checked)
                command_tabs = "update plugin_tabs set default_selected = 1 where name = '" + tab_name + "' AND plugin_no = " + plugin_INT;
            else
                command_tabs = "update plugin_tabs set default_selected = 0 where name = '" + tab_name + "' AND plugin_no = " + plugin_INT;

            if(!recon_helper_standard_obj->execute_db_command_by_dbreference(command_tabs,destination_db,Q_FUNC_INFO))
            {
                recon_static_functions::app_debug(" tab query --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
            }
        }
    }

    destination_db.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

//-----------------------------------------------------Main Launcher -------------------------------------------------------//

bool plugin_selector::pub_bool_run_plugin_having_record()
{
    // if plugin has no record, then not listed in tree

    if(ui->treeWidget_plugins->topLevelItemCount() == 0)
        return false;

    return true;
}

void plugin_selector::pub_initialise_plugin_info_for_main_new_case_launcher() //initialise plugins which are selected
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    if(launcher_type == MACRO_Launcher_Plugin_Selection_New_Case)
    {
        destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";
    }
    else if(launcher_type == MACRO_Launcher_Plugin_Selection_Add_Source)
        destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";


    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
        return;
    }

    initialise_plugins_for_recon_config_and_new_case_launcher(destination_db_file);

    ui->stackedWidget->setCurrentIndex(0);

    emit signal_plugin_selected(bool_plugin_is_checked);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::pub_get_selected_plugins_from_main_case_launcher() // get selected plugins from pre launcher
{
    get_selected_plugins_from_run_plugin_common();
}

//-----------------------------------------------------Global REPORT START-------------------------------------------------------//
void plugin_selector::pub_initialise_plugin_info_for_global_report() //display plugins in global report also
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    ui->lineEdit_plugin_search->clear();

    destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
        return;
    }

    initialise_plugins_for_global_report_comman_for_multi_os(destination_db_file);
    set_hide_widgets(true);

    set_destination_db_file_path_for_multi_os();

    //    disable_device_combobox_according_to_schema_db();

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::initialise_plugins_for_global_report_comman_for_multi_os(QString destination_db_file) // set them for global report
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }



    QSqlQuery query_plugin(destination_db);
    QString command_plugin = QString("select name,category,supported_os_scheme_display_list from plugins ORDER BY name");

    query_plugin.prepare(command_plugin);
    if(!query_plugin.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_plugin,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_plugin.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QList<struct_report_plugin_info> list_plugins_info;
    QStringList list_plugin_parents;

    total_plugin_selected = 0;

    int native_macos_found = 0;
    int native_ios_found = 0;
    int native_winos_found = 0;
    int native_gtakeoutos_found = 0;
    int native_androidos_found = 0;


    bool bool_is_any_plugin_run = false;

    QStringList run_plugin_withtab_list = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);
    while(query_plugin.next())
    {

        struct_report_plugin_info info;
        info.plugin_name = query_plugin.value(0).toString();

        QString plugin_name_str = info.plugin_name;
        if(plugin_name_str.contains(" "))
            plugin_name_str.replace(" ", "_");

        QString plugin_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str +  "/" + plugin_name_str+ ".sqlite";
        if(!QFileInfo::exists(plugin_db_path))
            continue;

        if(recon_helper_standard_obj->get_db_total_record_by_dbpath(true, plugin_db_path, QStringList(), Q_FUNC_INFO) <= 0)
            continue;


        bool_is_any_plugin_run = true;

        info.parent_name = query_plugin.value(1).toString();
        list_plugin_parents << info.parent_name;

        //-Tabs List.

        info.plugin_tab_name_list = recon_static_functions::get_associated_tabs(info.plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);


        info.is_selected = false;
        QStringList tab_list = recon_static_functions::get_associated_tabs(info.plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);


        if(tab_list.size() >= 1)
        {
            info.selected_plugins_tabs_list = tab_list;
            info.is_selected = true;
        }

        bool export_is_available;
        if(global_csv_reader_class_full_obj->is_export_available_in_plugin(info.plugin_name))
        {
            export_is_available = true;
            info.is_export_selected = true;
        }
        else
        {
            export_is_available = false;
            info.is_export_selected = false;
        }
        info.is_export_available = export_is_available;
        if(export_is_available)
        {
            info.export_available_tabs_list = global_csv_reader_class_full_obj->get_export_tab_list(info.plugin_name);
        }


        info.supported_os_scheme_display_QStringList = query_plugin.value(2).toString().split(MACRO_RECON_Splitter_5_resolution_scope, Qt::SkipEmptyParts);
        list_plugins_info << info;

    }


    destination_db.close();

    if(bool_is_any_plugin_run)
    {
        QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
        for(int count = 0; count < list_target_source_info.size(); count++)
        {
            struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
            QString os_schme_dsply = obj.os_scheme_display;
            if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
                native_macos_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                native_ios_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
                native_winos_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display)
                native_gtakeoutos_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                native_androidos_found++;
        }
    }


    list_plugin_parents.sort();

    if(native_gtakeoutos_found > 0)
        list_plugin_parents.prepend(supported_parent_gtakeoutOS);
    if(native_androidos_found > 0)
        list_plugin_parents.prepend(supported_parent_androidOS);
    if(native_winos_found > 0)
        list_plugin_parents.prepend(supported_parent_winOS);
    if(native_ios_found > 0)
        list_plugin_parents.prepend(supported_parent_iOS);
    if(native_macos_found > 0)
        list_plugin_parents.prepend(supported_parent_macOS);
    list_plugin_parents.prepend(M_VAR_PLUGIN_Parent_All_Plugins_QString);


    list_plugins_info_common = list_plugins_info;
    list_plugin_parents_common = list_plugin_parents;

    fill_template_combobox();
    display_plugins_tree_for_multiple_os(list_plugins_info);

    display_parent_category_list_for_multiple_os(list_plugin_parents);

    if(list_plugins_info.size() >= 1)
        emit signal_plugin_selected(true);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::pub_put_plugin_info_for_global_report() // set in narad global class about plugins info for global report
{
    QStringList list_selected_plugins_with_os_naming;

    list_selected_plugins_with_os_naming << get_plugin_info_for_global_report_common();

    narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Selected_Plugins_With_Tabs_AND_Export_Status_QStringList, get_plugin_info_for_global_report_common());
}

QStringList plugin_selector::pub_get_seleted_tabs_and_plugins_for_Keyword_Search() // selected tabs and plugins for kryword search
{
    return get_seleted_tabs_and_plugins_for_Keyword_Search_common();
}

void plugin_selector::pub_initialise_plugin_info_for_keyword_search() // plugin info when we search any keyword using search box line edit
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    ui->lineEdit_plugin_search->clear();

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;
    list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();


    destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
        return;
    }
    initialise_plugin_info_for_keyword_search_comman_for_multi_os(destination_db_file);
    set_hide_widgets(true);

    //    for(int i = 0; i < list_target_source_info.size(); i++)
    //    {
    //        destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    //        QFileInfo info(destination_db_file);
    //        if(!info.exists())
    //        {
    //            recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
    //            continue;
    //        }

    //        initialise_plugin_info_for_keyword_search_comman_for_multi_os(destination_db_file);
    //        set_hide_widgets(true);
    //    }

    set_destination_db_file_path_for_multi_os();

    //    disable_device_combobox_according_to_schema_db();
    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::initialise_plugin_info_for_keyword_search_comman_for_multi_os(QString destination_db_file) // plugin info when we search any keyword using search box line edit common for multi os
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }

    QList<struct_report_plugin_info> list_plugins_info;
    QStringList list_plugin_parents;

    QSqlQuery query_plugin(destination_db);
    QString command_plugin = QString("select name,category,supported_os_scheme_display_list from plugins ORDER BY name ");
    query_plugin.prepare(command_plugin);
    if(!query_plugin.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_plugin,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_plugin.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QStringList run_plugin_withtab_list = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);

    bool bool_is_any_plugin_run = false;

    while(query_plugin.next())
    {
        struct_report_plugin_info info;
        info.plugin_name = query_plugin.value(0).toString();

        QString plugin_name_str = info.plugin_name;
        if(plugin_name_str.contains(" "))
            plugin_name_str.replace(" ", "_");

        QString plugin_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_str +  "/" + plugin_name_str+ ".sqlite";

        if(!QFileInfo::exists(plugin_db_path))
            continue;

        if(recon_helper_standard_obj->get_db_total_record_by_dbpath(true, plugin_db_path, QStringList(), Q_FUNC_INFO) <= 0)
            continue;


        bool_is_any_plugin_run = true;


        info.parent_name = query_plugin.value(1).toString();
        list_plugin_parents << info.parent_name;

        //-Tabs List.
        info.plugin_tab_name_list = recon_static_functions::get_associated_tabs(info.plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);
        info.is_selected = false;
        QStringList tab_list = recon_static_functions::get_associated_tabs(info.plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);




        if(tab_list.size() >= 1)
        {
            info.selected_plugins_tabs_list = tab_list;
            info.is_selected = false;
        }

        info.supported_os_scheme_display_QStringList = query_plugin.value(2).toString().split(MACRO_RECON_Splitter_5_resolution_scope, Qt::SkipEmptyParts);
        list_plugins_info << info;

    }
    destination_db.close();

    int native_macos_found = 0;
    int native_ios_found = 0;
    int native_winos_found = 0;
    int native_gtakeoutos_found = 0;
    int native_androidos_found = 0;


    if(bool_is_any_plugin_run)
    {
        QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
        for(int count = 0; count < list_target_source_info.size(); count++)
        {
            struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
            QString os_schme_dsply = obj.os_scheme_display;
            if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
                native_macos_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                native_ios_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
                native_winos_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display)
                native_gtakeoutos_found++;
            else if(os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                native_androidos_found++;
        }
    }


    list_plugin_parents.sort();

    if(native_gtakeoutos_found > 0)
        list_plugin_parents.prepend(supported_parent_gtakeoutOS);
    if(native_androidos_found > 0)
        list_plugin_parents.prepend(supported_parent_androidOS);
    if(native_winos_found > 0)
        list_plugin_parents.prepend(supported_parent_winOS);
    if(native_ios_found > 0)
        list_plugin_parents.prepend(supported_parent_iOS);
    if(native_macos_found > 0)
        list_plugin_parents.prepend(supported_parent_macOS);
    list_plugin_parents.prepend(M_VAR_PLUGIN_Parent_All_Plugins_QString);


    //    list_plugin_parents.sort();
    //    list_plugin_parents.prepend(supported_parent_gtakeoutOS);
    //    list_plugin_parents.prepend(supported_parent_androidOS);
    //    list_plugin_parents.prepend(supported_parent_winOS);
    //    list_plugin_parents.prepend(supported_parent_iOS);
    //    list_plugin_parents.prepend(supported_parent_macOS);
    //    list_plugin_parents.prepend(M_VAR_PLUGIN_Parent_All_Plugins_QString);

    list_plugins_info_common = list_plugins_info;
    list_plugin_parents_common = list_plugin_parents;

    fill_template_combobox();
    display_plugins_tree_for_multiple_os(list_plugins_info);
    display_parent_category_list_for_multiple_os(list_plugin_parents);
    set_hide_widgets(true);

    if(list_plugins_info.size() >= 1)
        emit signal_plugin_selected(true);


    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::pub_initialise_plugin_info_for_right_click_run_plugin()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    ui->lineEdit_plugin_search->clear();

    destination_db_file = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(" DB File Not Exist------>> " + destination_db_file,Q_FUNC_INFO);
        return;
    }

    initialise_plugin_info_for_right_click_run_plugin_comman_for_multi_os(destination_db_file);
    set_hide_widgets(true);


    set_destination_db_file_path_for_multi_os();


    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::initialise_plugin_info_for_right_click_run_plugin_comman_for_multi_os(QString destination_db_file)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }

    QList<struct_report_plugin_info> list_plugins_info;
    QStringList list_plugin_parents;

    QSqlQuery query_plugin(destination_db);
    QString command_plugin = QString("select name,category,supported_os_scheme_display_list from plugins ORDER BY name");
    query_plugin.prepare(command_plugin);
    if(!query_plugin.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_plugin,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_plugin.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }
    QStringList run_plugin_withtab_list = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);

    while(query_plugin.next())
    {

        struct_report_plugin_info info;
        info.plugin_name = query_plugin.value(0).toString();
        info.parent_name = query_plugin.value(1).toString();
        list_plugin_parents << info.parent_name;

        //-Tabs List.
        info.plugin_tab_name_list = recon_helper_standard_obj->get_tablist_of_plugin_from_case_configuration_by_dbreference(destination_db, info.plugin_name,Q_FUNC_INFO);

        info.is_selected = false;
        QStringList tab_list = recon_static_functions::get_associated_tabs(info.plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);

        if(tab_list.size() >= 1)
        {
            info.selected_plugins_tabs_list = tab_list;
            info.is_selected = true;
        }

        info.supported_os_scheme_display_QStringList = query_plugin.value(2).toString().split(MACRO_RECON_Splitter_5_resolution_scope, Qt::SkipEmptyParts);
        list_plugins_info << info;
    }

    list_plugin_parents.sort();
    list_plugin_parents.prepend(supported_parent_gtakeoutOS);
    list_plugin_parents.prepend(supported_parent_androidOS);
    list_plugin_parents.prepend(supported_parent_winOS);
    list_plugin_parents.prepend(supported_parent_iOS);
    list_plugin_parents.prepend(supported_parent_macOS);
    list_plugin_parents.prepend(M_VAR_PLUGIN_Parent_All_Plugins_QString);

    list_plugins_info_common = list_plugins_info;
    list_plugin_parents_common = list_plugin_parents;

    fill_template_combobox();
    display_plugins_tree_for_multiple_os(list_plugins_info);
    display_parent_category_list_for_multiple_os(list_plugin_parents);
    set_hide_widgets(true);
    destination_db.close();

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void plugin_selector::on_treeWidget_plugins_itemClicked_new_case_common(QTreeWidgetItem *item, int column)
{
    if(column == enum_plugin_tree_column_plugin_translated_name)
    {
        if(item->checkState(enum_plugin_tree_column_plugin_translated_name) == 2)
        {
            //-To Check Toplevel Item if any Child get Checked.
            if(item->parent() != 0)
            {
                QString parent_name = item->parent()->text(enum_plugin_tree_column_plugin_translated_name);
                for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
                {
                    if(ui->treeWidget_plugins->topLevelItem(tp)->text(enum_plugin_tree_column_plugin_translated_name) != parent_name)
                        continue;

                    ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                    break;
                }
            }

            total_plugin_selected_common = total_plugin_selected_common + 1;

            if(ui->comboBox_template_plugins_selection->currentText() == M_VAR_PLUGIN_TEMPLATE_Select_All_Without_Export_QString)
            {
                //-To check export Coloum.
                if(item->text(enum_plugin_tree_column_export_available) != M_VAR_FUNCTIONALITY_Not_Available_QString)
                {
                    item->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
                }

                for(int i = 0; i < item->childCount(); i++)
                {
                    //-To check plugins Tabs.
                    item->child(i)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);

                    //-To check export childs.
                    if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                        continue;

                    item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
                }

            }
            else
            {
                //-To check export Coloum.
                if(item->text(enum_plugin_tree_column_export_available) != M_VAR_FUNCTIONALITY_Not_Available_QString)
                {
                    item->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
                }

                for(int i = 0; i < item->childCount(); i++)
                {
                    //-To check plugins Tabs.
                    item->child(i)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);

                    //-To check export childs.
                    if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                        continue;

                    item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
                }

            }
        }
        else
        {
            if(total_plugin_selected_common == 0) //-Many times when its value is zero it become -1, which is not posible.
                total_plugin_selected_common = 1;

            total_plugin_selected_common = total_plugin_selected_common - 1;

            //-To uncheck export childs.
            if(item->text(enum_plugin_tree_column_export_available) != M_VAR_FUNCTIONALITY_Not_Available_QString)
            {
                item->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
            }


            //-To Uncheck Parent if All Childs Get unchecked.
            if(item->parent() != 0)
            {
                QString parent_name = item->parent()->text(enum_plugin_tree_column_plugin_translated_name);
                for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
                {
                    if(ui->treeWidget_plugins->topLevelItem(tp)->text(enum_plugin_tree_column_plugin_translated_name) != parent_name)
                        continue;

                    int checked_tab_count = 0;
                    for(int ch = 0; ch < ui->treeWidget_plugins->topLevelItem(tp)->childCount(); ch++)
                    {
                        if(ui->treeWidget_plugins->topLevelItem(tp)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
                            continue;

                        checked_tab_count++;
                    }

                    if(checked_tab_count == 0)
                    {
                        ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);
                        break;
                    }

                }

            }
            else // To uncheck if parent Get unchecked.
            {
                for(int i = 0; i < item->childCount(); i++)
                {
                    //-To uncheck plugins Tabs.
                    item->child(i)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);

                    //                    if(ui->comboBox_template_plugins_selection->currentText() != PLUGIN_TEMPLATE_SELECT_ALL_WITHOUT_EXPORT)
                    //                    {

                    //-To uncheck export childs.
                    if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                        continue;
                    item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
                    //                    }

                }
            }
        }
    }
    else if(column == enum_plugin_tree_column_export_available)
    {

        if(item->checkState(enum_plugin_tree_column_export_available) == 2)
        {

            for(int i = 0; i < item->childCount(); i++)
            {
                if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                    continue;

                item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
            }
        }
        else
        {
            for(int i = 0; i < item->childCount(); i++)
            {
                if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                    continue;

                item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
            }
        }
    }


    bool bool_any_plugin_selected = false;
    for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
    {
        if(ui->treeWidget_plugins->topLevelItem(tp)->checkState(enum_plugin_tree_column_plugin_translated_name) == Qt::Checked)
        {
            bool_any_plugin_selected = true;
        }
    }

    emit signal_plugin_selected(bool_any_plugin_selected);

}

void plugin_selector::on_treeWidget_plugins_itemClicked_run_plugin_common(QTreeWidgetItem *item, int column) // tree widget item clicked in left side of plugins
{
    if(column == enum_plugin_tree_column_plugin_translated_name)
    {
        if(item->checkState(enum_plugin_tree_column_plugin_translated_name) == 1)
        {
            //-Child To Check.
            //-To Check Toplevel Item if any Child get Checked.
            if(item->parent() != 0)
            {
                QString parent_name = item->parent()->text(enum_plugin_tree_column_plugin_translated_name);
                for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
                {
                    if(ui->treeWidget_plugins->topLevelItem(tp)->text(enum_plugin_tree_column_plugin_translated_name) != parent_name)
                        continue;

                    ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                    break;
                }
            }

            //total_plugin_selected_common = total_plugin_selected_common + 1;
            //-To check export Coloum.
            if(item->text(enum_plugin_tree_column_export_available) != M_VAR_FUNCTIONALITY_Not_Available_QString)
            {
                item->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
            }

            bool check_of_partial = false;
            for(int i = 0; i < item->childCount(); i++)
            {
                //-To check plugins Tabs.
                //                if(item->child(i)->checkState(enum_plugin_tree_column_plugins) == Qt::PartiallyChecked){
                //                    continue;
                //                }

                //                if(select_or_remove == 2){
                //                    item->child(i)->setCheckState(enum_plugin_tree_column_plugins,Qt::Unchecked);
                //                }

                //                if(select_or_remove == 1){
                //                    item->child(i)->setCheckState(enum_plugin_tree_column_plugins,Qt::Checked);
                //                    check_of_partial = true;
                //                }

                //-To check export childs.
                if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                    continue;
                item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
            }

            if(check_of_partial)
                total_plugin_selected_common = total_plugin_selected_common + 1;

        }
        if(item->checkState(enum_plugin_tree_column_plugin_translated_name) == 2)
        {
            QStringList plugin_and_associated_tabs_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Selected_Plugins_With_Tab_After_Extraction_QStringList).toStringList();
            bool bool_item_checked = false;

            //-To Check Toplevel Item if any Child get Checked.
            if(item->parent() != 0)
            {
                QString parent_name = item->parent()->text(enum_plugin_tree_column_plugin_translated_name);

                QStringList plugin_tabs_list = recon_static_functions::get_associated_tabs(parent_name, plugin_and_associated_tabs_list, Q_FUNC_INFO);
                if(plugin_tabs_list.contains(item->text(enum_plugin_tree_column_plugin_translated_name))){
                    item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);

                    bool_item_checked = true;
                }

                for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
                {
                    if(ui->treeWidget_plugins->topLevelItem(tp)->text(enum_plugin_tree_column_plugin_translated_name) != parent_name)
                        continue;

                    if(bool_item_checked == false)
                        ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                    if(bool_item_checked == true)
                        ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                    else
                        ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                    break;
                }
            }


            total_plugin_selected_common = total_plugin_selected_common + 1;
            //-To check export Coloum.
            if(item->text(enum_plugin_tree_column_export_available) != M_VAR_FUNCTIONALITY_Not_Available_QString)
            {
                item->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
            }


            int all_partial_or_not = 0;

            for(int i = 0; i < item->childCount(); i++)
            {
                //-To check plugins Tabs.

                //                if(item->child(i)->checkState(enum_plugin_tree_column_plugins) == Qt::PartiallyChecked){
                //                    continue;
                //                }

                item->child(i)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);

                all_partial_or_not = 1;

                //-To check export childs.
                if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                    continue;
                item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
            }

            if(all_partial_or_not == 1)
            {
                item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
            }

            if(all_partial_or_not == 0){
                if(item->childCount() == 0 && (bool_item_checked == false)){
                    item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                }

                if(item->childCount() == 0 && (bool_item_checked == true)){
                    item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);

                }

                //                if(item->childCount() > 0){
                //                    item->setCheckState(enum_plugin_tree_column_plugins,Qt::PartiallyChecked);
                //                }

            }
        }
        else if(item->checkState(enum_plugin_tree_column_plugin_translated_name) == 0)
        {
            //   QStringList plugin_and_associated_tabs_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Selected_Plugins_With_Tab_After_Extraction_QStringList).toStringList();
            //   bool hmmm = false;

            if(total_plugin_selected_common == 0) //-Many times when its value is zero it become -1, which is not posible.
                total_plugin_selected_common = 1;

            total_plugin_selected_common = total_plugin_selected_common - 1;
            //-To uncheck export childs.
            if(item->text(enum_plugin_tree_column_export_available) != M_VAR_FUNCTIONALITY_Not_Available_QString)
            {
                item->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
            }


            //-To Uncheck Parent if All Childs Get unchecked.
            if(item->parent() != 0)
            {
                QString parent_name = item->parent()->text(enum_plugin_tree_column_plugin_translated_name);
                for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
                {
                    if(ui->treeWidget_plugins->topLevelItem(tp)->text(enum_plugin_tree_column_plugin_translated_name) != parent_name)
                        continue;


                    //   QStringList plugin_tabs_list = recon_static_functions::get_plugin_and_associated_tabs(parent_name, plugin_and_associated_tabs_list, Q_FUNC_INFO);
                    //                    if(plugin_tabs_list.contains(item->text(enum_plugin_tree_column_plugins))){
                    //                        hmmm = true;
                    //                    }

                    int checked_tab_count = 0;
                    bool bool_plugin_check = false;

                    for(int ch = 0; ch < ui->treeWidget_plugins->topLevelItem(tp)->childCount(); ch++)
                    {
                        //                        if(ui->treeWidget_plugins->topLevelItem(tp)->child(ch)->checkState(enum_plugin_tree_column_plugins) != Qt::Checked && ui->treeWidget_plugins->topLevelItem(tp)->child(ch)->checkState(enum_plugin_tree_column_plugins) != Qt::PartiallyChecked)
                        //                            continue;

                        if(ui->treeWidget_plugins->topLevelItem(tp)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
                            continue;

                        if(ui->treeWidget_plugins->topLevelItem(tp)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) == Qt::Checked){
                            bool_plugin_check = true;
                        }

                        checked_tab_count++;

                    }

                    if(bool_plugin_check == true){
                        ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
                        break;
                    }

                    if(checked_tab_count == 0)
                    {
                        ui->treeWidget_plugins->topLevelItem(tp)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);
                        break;
                    }

                }

            }
            else // To uncheck if parent Get unchecked.
            {

                for(int i = 0; i < item->childCount(); i++)
                {
                    //-To uncheck plugins Tabs.
                    item->child(i)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);

                    //-To uncheck export childs.
                    if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                        continue;
                    item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);

                }

                item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);

            }
        }
    }
    else if(column == 1)
    {
        if(item->checkState(enum_plugin_tree_column_export_available) == 2)
        {

            for(int i = 0; i < item->childCount(); i++)
            {
                if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                    continue;



                item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Checked);
            }
        }
        else
        {

            for(int i = 0; i < item->childCount(); i++)
            {
                if(item->child(i)->text(enum_plugin_tree_column_export_available) == M_VAR_FUNCTIONALITY_Not_Available_QString)
                    continue;

                item->child(i)->setCheckState(enum_plugin_tree_column_export_available,Qt::Unchecked);
            }
        }

    }

    bool bool_any_plugin_selected = false;
    for(int tp = 0; tp < ui->treeWidget_plugins->topLevelItemCount(); tp++)
    {
        if(ui->treeWidget_plugins->topLevelItem(tp)->checkState(enum_plugin_tree_column_plugin_translated_name) == Qt::Checked)
        {
            bool_any_plugin_selected = true;
        }
    }

    emit signal_plugin_selected(bool_any_plugin_selected);

}

void plugin_selector::select_all_with_export_clicked_common() // select all with export
{
    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        if(ui->treeWidget_plugins->topLevelItem(i)->isHidden())
            continue;

        QTreeWidgetItem *item = ui->treeWidget_plugins->topLevelItem(i);

        item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
        on_treeWidget_plugins_itemClicked(item,enum_plugin_tree_column_plugin_translated_name);
    }


}

void plugin_selector::select_all_without_export() // select all plugins without export from combobox
{
    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        if(ui->treeWidget_plugins->topLevelItem(i)->isHidden())
            continue;

        QTreeWidgetItem *item = ui->treeWidget_plugins->topLevelItem(i);

        item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Checked);
        on_treeWidget_plugins_itemClicked(item,enum_plugin_tree_column_plugin_translated_name);
    }

}

void plugin_selector::remove_all_clicked_common() // remove all plugins from selection
{
    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        if(ui->treeWidget_plugins->topLevelItem(i)->isHidden())
            continue;

        QTreeWidgetItem *item = ui->treeWidget_plugins->topLevelItem(i);

        item->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);

        on_treeWidget_plugins_itemClicked(item,enum_plugin_tree_column_plugin_translated_name);
    }

}

void plugin_selector::select_template_plugins_for_common(QString template_name)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    //-Clear list.
    for(int count_i = 0; count_i < ui->treeWidget_plugins->topLevelItemCount(); count_i++)
    {
        QTreeWidgetItem *item = ui->treeWidget_plugins->topLevelItem(count_i);
        ui->treeWidget_plugins->topLevelItem(count_i)->setCheckState(enum_plugin_tree_column_plugin_translated_name,Qt::Unchecked);

        on_treeWidget_plugins_itemClicked(item,enum_plugin_tree_column_plugin_translated_name);
    }

    QString command = QString("select plugin_name from plugin_selection_template where template_name = ?");
    QStringList list_values;
    list_values << template_name;
    QStringList plugins_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,list_values,0,destination_db_file,Q_FUNC_INFO);

    for(int plugin_count = 0; plugin_count < plugins_list.size(); plugin_count++)
    {
        QString db_plugin_str = plugins_list.at(plugin_count);

        command = QString("select selected_tabs_list from plugin_selection_template where plugin_name = ?");
        list_values.clear();
        list_values << db_plugin_str;

        QString selected_tabs_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list_values,0,destination_db_file,Q_FUNC_INFO);

        QStringList list_selected_tabs = selected_tabs_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
        {
            QString selected_plugin_name = ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_original_name);

            if(db_plugin_str == selected_plugin_name)
            {
                QTreeWidgetItem *plugin_tree_item = ui->treeWidget_plugins->topLevelItem(i);

                plugin_tree_item->setCheckState(enum_plugin_tree_column_plugin_translated_name , Qt::Checked);

                for(int count = 0; count < plugin_tree_item->childCount(); count++)
                {
                    if(list_selected_tabs.contains(plugin_tree_item->child(count)->text(enum_plugin_tree_column_plugin_original_name)))
                    {
                        plugin_tree_item->child(count)->setCheckState(enum_plugin_tree_column_plugin_translated_name , Qt::Checked);
                    }
                    else
                    {
                        plugin_tree_item->child(count)->setCheckState(enum_plugin_tree_column_plugin_translated_name , Qt::Unchecked);
                    }

                }
            }
        }

    }

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

void plugin_selector::lineEdit_plugin_search_textChanged_common(const QString &arg1) // change display of plugins as we search any plugin name from search box
{
    ui->treeWidget_plugins->collapseAll();

    if(arg1.trimmed().isEmpty())
    {
        on_tableWidget_parent_list_cellClicked(ui->tableWidget_parent_list->currentRow() , ui->tableWidget_parent_list->currentColumn());
        return;
    }

    QString text = arg1;
    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        if(ui->treeWidget_plugins->topLevelItem(i)->isHidden())
            continue ;

        if(list_plugins_info.size() == ui->treeWidget_plugins->topLevelItemCount())
        {
            if(list_plugins_info.at(i).plugin_name == ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_translated_name))
            {
                if(ui->tableWidget_parent_list->currentItem()->text() != M_VAR_PLUGIN_Parent_All_Plugins_QString)
                {
                    if(ui->tableWidget_parent_list->currentItem()->text() != list_plugins_info.at(i).parent_name)
                        continue;
                }
            }
        }

        if(ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_translated_name).toLower().contains(text.toLower()))
            ui->treeWidget_plugins->topLevelItem(i)->setHidden(false);
        else
            ui->treeWidget_plugins->topLevelItem(i)->setHidden(true);
    }
}

void plugin_selector::get_selected_plugins_from_main_case_launcher_common() // get selected plugins for case launcher
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QStringList selected_plugins_with_tab_list;
    QStringList selected_plugins_list;
    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        QString plgn_nm;
        if(ui->treeWidget_plugins->topLevelItem(i)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
            continue;

        plgn_nm =  ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_translated_name).trimmed();

        if(selected_plugins_list.contains(plgn_nm))
            continue;

        selected_plugins_list << plgn_nm;


        plgn_nm += MACRO_RECON_Splitter_1_multiple;

        for(int ch = 0; ch < ui->treeWidget_plugins->topLevelItem(i)->childCount(); ch++)
        {
            if(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
                continue;

            plgn_nm += QString(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->text(enum_plugin_tree_column_plugin_translated_name) + MACRO_RECON_Splitter_2_hash_in_brace);
        }

        QString splitter = MACRO_RECON_Splitter_2_hash_in_brace;

        if(plgn_nm.endsWith(MACRO_RECON_Splitter_2_hash_in_brace))
            plgn_nm.chop(splitter.size());

        selected_plugins_with_tab_list << plgn_nm;
    }

    narad_muni_for_report_obj->set_field(MACRO_NARAD_Selected_Plugins_QStringList,selected_plugins_list);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList,selected_plugins_with_tab_list);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

}

QStringList plugin_selector::get_plugin_info_for_global_report_common() //get plugin info for global report
{
    QStringList selected_plugin_info_list;
    QString all_plugins_and_tab_str;

    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = ui->treeWidget_plugins->topLevelItem(i);

        QString plugin_name;
        QString child_string;

        if(item->checkState(enum_plugin_tree_column_plugin_translated_name) == 2)
        {
            plugin_name = item->text(enum_plugin_tree_column_plugin_translated_name);

            for(int j = 0; j < item->childCount(); j++)
            {
                if(item->child(j)->checkState(enum_plugin_tree_column_plugin_translated_name) == 2)
                {
                    if(item->child(j)->checkState(enum_plugin_tree_column_export_available) == 2)
                    {
                        QString selected_tab_name = item->child(j)->text(enum_plugin_tree_column_plugin_translated_name);
                        QString plugin_info_str = MACRO_RECON_Splitter_2_hash_in_brace + selected_tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "1";
                        child_string.append(plugin_info_str);
                    }
                    else
                    {
                        QString selected_tab_name = item->child(j)->text(enum_plugin_tree_column_plugin_translated_name);
                        QString plugin_info_str = MACRO_RECON_Splitter_2_hash_in_brace + selected_tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "0";
                        child_string.append(plugin_info_str);
                    }
                }
            }

            if(global_csv_reader_class_selected_obj->is_plugin_available_for_chat_view(plugin_name))
            {
                QString chat_view_tab_name = global_csv_reader_class_selected_obj->get_chat_view_tab_name(plugin_name);
                QString plugin_info_with_chat_view_tab = MACRO_RECON_Splitter_2_hash_in_brace + chat_view_tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "0";
                child_string.append(plugin_info_with_chat_view_tab);
            }

            QString splitter_2 = MACRO_RECON_Splitter_2_hash_in_brace;
            if(child_string.startsWith(splitter_2))
                child_string.remove(0,splitter_2.size());

            QString temp_str = plugin_name + MACRO_RECON_Splitter_1_multiple + child_string;
            selected_plugin_info_list << temp_str;
        }
    }


    return selected_plugin_info_list;
}

QStringList plugin_selector::get_seleted_tabs_and_plugins_for_Keyword_Search_common() // selected tabs of plugins runs after search
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QStringList selected_plugins_with_tab_list;
    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        QString str;
        if(ui->treeWidget_plugins->topLevelItem(i)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
            continue;

        str =  ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_translated_name).trimmed();

        str += MACRO_RECON_Splitter_1_multiple;

        for(int ch = 0; ch < ui->treeWidget_plugins->topLevelItem(i)->childCount(); ch++)
        {
            if(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
                continue;

            str += QString(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->text(enum_plugin_tree_column_plugin_translated_name) + MACRO_RECON_Splitter_2_hash_in_brace);
        }

        QString splitter = MACRO_RECON_Splitter_2_hash_in_brace;

        if(str.endsWith(MACRO_RECON_Splitter_2_hash_in_brace))
            str.chop(splitter.size());

        selected_plugins_with_tab_list << str;
    }

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

    return selected_plugins_with_tab_list;
}

bool plugin_selector::get_selected_plugins_from_run_plugin_common() //get selected plugins to run
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QStringList selected_plugins_with_tab_list;
    QStringList selected_plugins_list;


    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        QString str;
        if(ui->treeWidget_plugins->topLevelItem(i)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
            continue;

        QString p_plugin_name =  ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_original_name).trimmed();

        if(p_plugin_name.contains("Apple Installed Applications") || p_plugin_name.contains("FS Events"))
            continue;


        selected_plugins_list << p_plugin_name;

        str =  p_plugin_name;

        str += MACRO_RECON_Splitter_1_multiple;

        for(int ch = 0; ch < ui->treeWidget_plugins->topLevelItem(i)->childCount(); ch++)
        {
            if(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
                continue;

            str += QString(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->text(enum_plugin_tree_column_plugin_original_name) + MACRO_RECON_Splitter_2_hash_in_brace);
        }

        QString splitter = MACRO_RECON_Splitter_2_hash_in_brace;

        if(str.endsWith(MACRO_RECON_Splitter_2_hash_in_brace))
            str.chop(splitter.size());

        selected_plugins_with_tab_list << str;
    }


    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        QString str;
        if(ui->treeWidget_plugins->topLevelItem(i)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
            continue;

        QString p_plugin_name =  ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_original_name).trimmed();

        if(!p_plugin_name.contains("Apple Installed Applications") && !p_plugin_name.contains("FS Events"))
            continue;


        selected_plugins_list << p_plugin_name;

        str =  p_plugin_name;

        str += MACRO_RECON_Splitter_1_multiple;

        for(int ch = 0; ch < ui->treeWidget_plugins->topLevelItem(i)->childCount(); ch++)
        {
            if(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->checkState(enum_plugin_tree_column_plugin_translated_name) != Qt::Checked)
                continue;

            str += QString(ui->treeWidget_plugins->topLevelItem(i)->child(ch)->text(enum_plugin_tree_column_plugin_original_name) + MACRO_RECON_Splitter_2_hash_in_brace);
        }

        QString splitter = MACRO_RECON_Splitter_2_hash_in_brace;

        if(str.endsWith(MACRO_RECON_Splitter_2_hash_in_brace))
            str.chop(splitter.size());

        selected_plugins_with_tab_list << str;
    }


    narad_muni_for_report_obj->set_field(MACRO_NARAD_Selected_Plugins_QStringList,selected_plugins_list);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList,selected_plugins_with_tab_list);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

    return true;
}



bool plugin_selector::pub_get_selected_plugins_from_launcher_for_add_source() // selected plugins from pre launcher
{
    bool bool_plugins_selected = false;

    bool_plugins_selected = get_selected_plugins_from_run_plugin_common();

    return bool_plugins_selected;
}


bool plugin_selector::pub_get_selected_plugins_from_launcher_for_run_plugin() // get selected plugins
{    
    return get_selected_plugins_from_run_plugin_common();
}

