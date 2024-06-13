#include "registry_viewer.h"

registry_viewer::registry_viewer(QWidget *parent) : QWidget(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

registry_viewer::registry_viewer()
{

}

void registry_viewer::pub_set_destination_db_path(QString db_path)
{
    destination_db_file = db_path;
}

void registry_viewer::pub_set_case_tree_tab_name(QString tab_name)
{
    QStringList list_value = tab_name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(list_value.size() > 0)
        registry_file_name = list_value.at(0);

    if(list_value.size() > 1)
        registry_plugin_name = list_value.at(1);

    if(list_value.size() > 2)
        registry_plugin_category_name = list_value.at(2);

    if(list_value.size() > 3)
        registry_plugin_record_no = list_value.at(3);


}

void registry_viewer::pub_set_essentials()
{
    QString file_name = registry_file_name + " [" + registry_plugin_name +" "+ registry_plugin_record_no + "]" + " [" + source_count_name + "]";

    //- Display name..
    display_tab_name = file_name;

}


void registry_viewer::pub_set_source_file_path(QString src_file_path)
{
    registry_source_file_path = src_file_path;
}

void registry_viewer::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}

void registry_viewer::pub_set_source_count_name(QString src_count_name)
{
    source_count_name = src_count_name;
}

void registry_viewer::pub_set_registry_destination_path(QString index_db_path)
{
    dest_index_db_path = index_db_path;

}

void registry_viewer::pub_parse_windows_registry()
{ // public function which parse the windows registry files
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    set_essentials();

    QString source_file = registry_source_file_path;

    registry_file_name.clear();
    node_path.clear();
    file_path.clear();
    node_name.clear();

    QString registry_name = source_file;
    QFileInfo file_info(registry_name);
    selected_registry_name_str = file_info.fileName();


    if(selected_registry_name_str  == QString("NTUSER.DAT") || selected_registry_name_str == QString("USRCLASS.DAT"))
    {
        node_path = "HKEY_CURRENT_USER (HKCU)";
        file_path = actual_file_path;
    }
    else if(selected_registry_name_str  == QString("SOFTWARE")
            || selected_registry_name_str == QString("SAM")
            || selected_registry_name_str  == QString("SYSTEM")
            || selected_registry_name_str  == QString("SECURITY"))
    {
        node_path = "HKEY_LOCAL_MACHINE (HKLM)";
        file_path = actual_file_path;
    }
    else if(selected_registry_name_str  == QString(".DEFAULT"))
    {
        node_path = "HKEY_USER (HKU)";
        file_path = actual_file_path;
    }
    else
    {
        node_path = "";
        file_path = actual_file_path;
    }

    ///Actual Windows file Format >> "C:\Windows\System32"
    //    QString window_file_path = actual_file_path;
    //    window_file_path.prepend("C:");
    //    if(actual_file_path.startsWith("/Users/"))
    //    {
    //        node_path = "HKEY_LOCAL_MACHINE (HKLM)";
    //        file_path = window_file_path;
    //    }
    //    else if(actual_file_path.startsWith("/Windows/"))
    //    {
    //        node_path = "HKEY_LOCAL_MACHINE (HKLM)";
    //        file_path = window_file_path;
    //    }
    //    else if(actual_file_path.startsWith("/$WINDOWS") || actual_file_path.startsWith("/ProgramData/")
    //            || actual_file_path.startsWith("/$GetCurrent"))
    //    {
    //        node_path = "HKEY_CLASSES_ROOT (HKCR)";
    //        file_path = window_file_path;
    //    }
    //    else
    //    {
    //        node_path = "";
    //        file_path = window_file_path;
    //    }



    int ii = 0;

    hive_h *hive_opened = hivex_open(source_file.toLocal8Bit().data() , 0);//// 0 means NO Flag Set
    if(hive_opened == nullptr)
    {
        return ;
    }


    hive_node_h parent_h_node = hivex_root(hive_opened);

    hive_node_h *childnodes =  hivex_node_children(hive_opened,parent_h_node);

    if(childnodes == nullptr)
    {
        hivex_close(hive_opened);
        return ;
    }

    registry_info_in_index_db(dest_index_db_path);


    ///.--- create a separate destination db for registry file
    create_db_destination_file();
    initial_root_value_str.clear();



    QSqlDatabase destination_db;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug(" db opened ----FAILED---- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        hivex_close(hive_opened);
        return ;
    }

    destination_db.transaction();

    QString complete_node_path_str;

    while(childnodes[ii])
    {

        if(ii % 500 == 0)
            QCoreApplication::processEvents();

        hive_node_h parent_h_node_1 = hivex_node_parent(hive_opened, childnodes[ii]);

        char *child_node = hivex_node_name(hive_opened, parent_h_node_1);
        if(child_node == nullptr)
        {
            continue ;
        }

        if(!complete_node_path_str.endsWith(child_node))
            complete_node_path_str.append("/").append(child_node);


        initial_root_value_str = complete_node_path_str;


        get_next_node(hive_opened , childnodes[ii] , destination_db , QString(child_node) , complete_node_path_str);


        ii++;
    }


    hivex_close(hive_opened);

    destination_db.commit();
    destination_db.close();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void registry_viewer::set_essentials()
{
    actual_file_path = st_globl_recon_file_info_obj.display_file_path;
    recon_file_from = st_globl_recon_file_info_obj.recon_file_from;
    recon_file_info_path = st_globl_recon_file_info_obj.recon_file_info_path;

    struct_GLOBAL_witness_info_source source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    os_nm_scheme_disp = source_info_obj.os_scheme_display;
}

void registry_viewer::create_db_destination_file()
{ // create registry destination database to store the info and select the info to display
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString command;


    command = QString("create table tbl_registry_tree"
                      "(bookmark INTEGER,"
                      "INT INTEGER PRIMARY KEY,"
                      "system_account VARCHAR(100),"

                      "plugin_name varchar(100),"
                      "tab_name varchar(100),"


                      "file_path varchar(100),"
                      "node_path varchar(100),"
                      "node_name varchar(100),"
                      "registry_name varchar(100),"


                      "child_name varchar(100),"
                      "sub_parent_path varchar(100),"
                      "parent_path varchar(100),"
                      "node_timestamp INTEGER,"

                      "key_name varchar(100),"
                      "k_values varchar(100),"
                      "types varchar(100),"
                      "hex_values varchar(100),"


                      "Is_Parent varchar(100),"
                      "attached_children varchar(100),"

                      "item1 varchar(100),"
                      "item2 varchar(100),"
                      "item3 varchar(100),"
                      "item4 varchar(100),"
                      "item5 varchar(100),"
                      "item6 varchar(100),"

                      "item7 varchar(100),"
                      "item8 varchar(100),"
                      "item9 varchar(100),"
                      "item10 varchar(100),"


                      "recon_tag_value varchar(100),"
                      "notes_icon VARCHAR(1024),"
                      "tags_icon VARCHAR(1024),"
                      "os_scheme_display VARCHAR(100),"
                      "source_count_name varchar(100),"
                      "source_file varchar(500),"

                      "recon_filefrom varchar(500),"
                      "recon_file_infopath varchar(500),"


                      "notes VARCHAR(500))");



    QSqlDatabase destination_db;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug(" db opened ----FAILED---- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }


    recon_helper_standard_obj->execute_db_command_by_dbpath(command,destination_db_file,Q_FUNC_INFO);

    destination_db.close();
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void registry_viewer::registry_info_in_index_db(QString index_db_path)
{ // registry info in index database

    QString command = "INSERT INTO 'tbl_registry_viewer_index'(display_tab_name,relative_db_path,source_file_name,source_file_path,source_count_name,recon_filefrom,recon_file_infopath) VALUES (?,?,?,?,?,?,?)";
    QStringList arg_list;

    QString relaitive_db_path = destination_db_file;
    if(relaitive_db_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
        relaitive_db_path.remove(0 , global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());

    if(!relaitive_db_path.startsWith("/"))
        relaitive_db_path.prepend("/");


    arg_list << display_tab_name
             << relaitive_db_path
             << registry_file_name
             << actual_file_path
             << source_count_name
             << recon_file_from
             << recon_file_info_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,index_db_path, Q_FUNC_INFO);
}

void registry_viewer::get_next_node(hive_h *hive_opened, hive_node_h node , QSqlDatabase destination_db , QString node_name_str , QString arg_complete_node_path)
{ // get the node while parsing registry files
    int jj = 0;

    hive_node_h *sub_child_node = hivex_node_children(hive_opened,node);
    if(sub_child_node == nullptr)
    {
        return;
    }

    char *child_node_name = hivex_node_name(hive_opened, node);
    if(child_node_name == nullptr)
    {
        return;
    }

    QString current_node_path = "/" + QString(child_node_name);
    QString sub_parent_path = arg_complete_node_path;
    QString parent_path = arg_complete_node_path + current_node_path;


    QString temp_str = initial_root_value_str;
    QString node_name_without_initial_root_value = arg_complete_node_path + current_node_path;
    QStringList temp_list = node_name_without_initial_root_value.split(temp_str);
    node_name_without_initial_root_value = temp_list.join("");
    QString HKEY_name_without_abrivation = node_path;
    if(!HKEY_name_without_abrivation.trimmed().isEmpty())
    {
        QStringList tm_list = HKEY_name_without_abrivation.split("(");
        if(tm_list.size() >= 1)
            HKEY_name_without_abrivation = tm_list.at(0);
    }
    if(!HKEY_name_without_abrivation.isEmpty())
    {
        node_name = "/" + HKEY_name_without_abrivation.trimmed() + "/" + selected_registry_name_str + node_name_without_initial_root_value;
    }
    else
    {
        node_name = "/" + selected_registry_name_str + node_name_without_initial_root_value;
    }



    /// it will extract all data attached to that particular node
    /// including keys , types , values , hex values etc.

    values_of_current_node(hive_opened , node);


    int64_t timestamp_node = hivex_node_timestamp(hive_opened, node);
    qint64 epch_time;
    if(timestamp_node != -1)
        epch_time = (timestamp_node -116444736000000000)/10000000;
    else
    {
        epch_time = 0;
    }

    size_t have_child_size_t = hivex_node_nr_children(hive_opened , node);

    insert_data_in_db(destination_db , sub_parent_path , have_child_size_t ,epch_time , child_node_name , parent_path);

    while(sub_child_node[jj])
    {
        if(jj % 500 == 0)
            QCoreApplication::processEvents();

        hive_node_h sub_parent_h_node = hivex_node_parent(hive_opened, sub_child_node[jj]);

        char *p_node_name_str = hivex_node_name(hive_opened, sub_parent_h_node);

        if(p_node_name_str == nullptr)
        {
            continue;
        }


        if(!arg_complete_node_path.endsWith(p_node_name_str))
            arg_complete_node_path.append("/").append(p_node_name_str);

        get_next_node(hive_opened , sub_child_node[jj] , destination_db , QString(p_node_name_str) , arg_complete_node_path);
        jj++;
    }

}

void registry_viewer::insert_data_in_db(QSqlDatabase destination_db , QString sub_parent_path , size_t have_child_size_t, qint64 epch_time , QString child_node_name , QString arg_compelete_node_path_str)
{ // insert information in database after parsing registry and node

    QString insert_cmd = "insert into tbl_registry_tree(bookmark ,child_name , sub_parent_path,key_name,"
                         "parent_path,k_values,hex_values,types ,registry_name ,plugin_name , tab_name,node_name , node_path,file_path,"
                         "node_timestamp ,Is_Parent , attached_children , source_file , source_count_name ,"
                         "os_scheme_display ,recon_filefrom ,recon_file_infopath ) values(0,?,?,?,?,?,?,?,?,?, ?,?,?, ?,?,?,?,?,?,?,?,?)";


    QSqlQuery insert_query(destination_db);


    ///---if node don't have any values
    if(keys_name_list.size() == 0)
    {
        insert_query.prepare(insert_cmd);
        insert_query.addBindValue(child_node_name);
        insert_query.addBindValue(sub_parent_path);
        insert_query.addBindValue("");
        insert_query.addBindValue(arg_compelete_node_path_str);
        insert_query.addBindValue("");
        insert_query.addBindValue("");
        insert_query.addBindValue("");
        insert_query.addBindValue(selected_registry_name_str);
        insert_query.addBindValue(QString(MACRO_Plugin_Name_Registry_Viewer));
        insert_query.addBindValue(QString(MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer));
        insert_query.addBindValue(node_name);
        insert_query.addBindValue(node_path);
        insert_query.addBindValue(file_path);
        insert_query.addBindValue(QString::number(epch_time));

        if(have_child_size_t == 0)
            insert_query.addBindValue(QString::number(0));
        else
            insert_query.addBindValue(QString::number(1));

        insert_query.addBindValue(QString::number(have_child_size_t));
        insert_query.addBindValue(actual_file_path);
        insert_query.addBindValue(source_count_name);
        insert_query.addBindValue(os_nm_scheme_disp);
        insert_query.addBindValue(recon_file_from);
        insert_query.addBindValue(recon_file_info_path);
        if(!insert_query.exec())
        {
            recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + insert_query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
            return ;
        }
    }
    else
    {
        for(int kk = 0 ; kk < keys_name_list.size() ; kk++)
        {
            QString key_name_str  = keys_name_list.at(kk);
            if(key_name_str.trimmed().isEmpty())
                key_name_str = "(default)";

            QString value_str  = values_list.at(kk);
            QString hex_value_str  = hex_value_list.at(kk);


            ///---it depends on enum value   so, size of list "hive_type_list" may vary regulate it with a safe check
            QString hive_type_str;
            if(hive_type_list.size() == keys_name_list.size())
                hive_type_str  = hive_type_list.at(kk);
            else
                hive_type_str = "";




            insert_query.prepare(insert_cmd);
            insert_query.addBindValue(child_node_name);
            insert_query.addBindValue(sub_parent_path);
            insert_query.addBindValue(key_name_str);
            insert_query.addBindValue(arg_compelete_node_path_str);
            insert_query.addBindValue(value_str.toLocal8Bit());
            insert_query.addBindValue(hex_value_str);
            insert_query.addBindValue(hive_type_str);
            insert_query.addBindValue(selected_registry_name_str);
            insert_query.addBindValue(QString(MACRO_Plugin_Name_Registry_Viewer));
            insert_query.addBindValue(QString(MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer));
            insert_query.addBindValue(node_name);
            insert_query.addBindValue(node_path);
            insert_query.addBindValue(file_path);

            insert_query.addBindValue(QString::number(epch_time));


            if(have_child_size_t == 0)
                insert_query.addBindValue(QString::number(0));
            else
                insert_query.addBindValue(QString::number(1));

            insert_query.addBindValue(QString::number(have_child_size_t));

            insert_query.addBindValue(actual_file_path);
            insert_query.addBindValue(source_count_name);
            insert_query.addBindValue(os_nm_scheme_disp);

            insert_query.addBindValue(recon_file_from);
            insert_query.addBindValue(recon_file_info_path);

            if(!insert_query.exec())
            {
                recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" command -  " + insert_query.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + insert_query.lastError().text(),Q_FUNC_INFO);
                return ;
            }


        }
    }

}




