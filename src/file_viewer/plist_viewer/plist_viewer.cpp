#include "plist_viewer.h"

plist_viewer::plist_viewer(QWidget *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

plist_viewer::~plist_viewer()
{

}

void plist_viewer::pub_set_plist_path(QString path)
{
    destination_plist_path = path;
}

void plist_viewer::pub_set_resultsqlite_path(QString name)
{
    destination_plist_result_db_path = name;
}

void plist_viewer::pub_set_source_count_name(QString name)
{
    source_count_name = name;
}

void plist_viewer::pub_set_plist_viewer_name(QString name)
{
    QStringList list_value = name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(list_value.size() > 0)
        plist_file_name = list_value.at(0);

    if(list_value.size() > 1)
        plist_plugin_name = list_value.at(1);

    if(list_value.size() > 2)
        plist_plugin_category_name = list_value.at(2);

    if(list_value.size() > 3)
        plist_plugin_record_no = list_value.at(3);

}

void plist_viewer::pub_set_essentials()
{
    QString file_name = plist_file_name + " [" + plist_plugin_name +" "+ plist_plugin_record_no + "]" + " [" + source_count_name + "]";

    //- Display name..
    plist_viewer_display_name = file_name;

    file_name = plist_file_name + " " + plist_plugin_name +" "+ plist_plugin_record_no + " " + source_count_name;

    //db_table_name.
    file_name = file_name.replace(" ","_");
    file_name = file_name.replace("-","_");
    file_name = file_name.replace(".","_");

    plist_view_db_table_name = file_name;

}


void plist_viewer::pub_start_plist_extraction()
{
    set_source_file_path();
    entry_in_index_table();
    create_destination_database();
    extract_selected_file();
}

void plist_viewer::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}

int plist_viewer::create_destination_database()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString command = "CREATE TABLE '" + plist_view_db_table_name + "'"+
            "(bookmark INTEGER,"
            "INT INTEGER PRIMARY KEY,"
            "id INTEGER,"
            "key VARCHAR(300),"
            "type VARCHAR(300),"
            "value VARCHAR(300),"
            "parent VARCHAR(300),"
            "keypath VARCHAR(300),"
            "parentpath VARCHAR(300),"
            "data_conversion_type VARCHAR(500),"
            "recon_tag_value VARCHAR(300),"
            "notes VARCHAR(500))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command,destination_plist_result_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
    return 1;
}

void plist_viewer::entry_in_index_table()
{
    QString command = "INSERT INTO 'plist_viewer_index'(viewer_display_name,db_table_name,file_name,file_path,"
                      "plugin_name,category_name,record_no,creation_timestamp, source_count_name,"
                      "recon_filefrom , recon_file_infopath) VALUES (?,?,?,?,?, ?,?,?,?,?,?)";
    QStringList list;

    list << plist_viewer_display_name << plist_view_db_table_name << plist_file_name << source_file_path <<plist_plugin_name << plist_plugin_category_name;
    list << plist_plugin_record_no << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch()) << source_count_name << recon_file_from << source_info_relative_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_plist_result_db_path, Q_FUNC_INFO);
}

void plist_viewer::set_source_file_path()
{
    source_file_path          = st_globl_recon_file_info_obj.display_file_path;
    recon_file_from           =  st_globl_recon_file_info_obj.recon_file_from;
    source_info_relative_path = st_globl_recon_file_info_obj.recon_file_info_path;

}

void plist_viewer::extract_selected_file()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    ///Main Parsing Occurs Here.....
    xcode_extract_plist(destination_plist_path);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_plist_result_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug( destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery select_query(destination_db);
    QSqlQuery insert_query(destination_db);

    ///Db updating to measure level of treewidget items
    // update_tree_for_measure_levels(select_query,insert_query);

    destination_db.close();
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer::update_tree_for_measure_levels(QSqlQuery select_query,QSqlQuery insert_query)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    select_query.prepare("SELECT key,type,value,keypath, INT FROM '" + plist_view_db_table_name + "'");
    if(!select_query.exec())
    {
        recon_static_functions::app_debug("select_query execution-----FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.executedQuery() , Q_FUNC_INFO);
        return;
    }

    while(select_query.next())
    {
        QString keypath = select_query.value(3).toString();
        QString int1 = select_query.value(4).toString();

        unsigned int hCount = 0;
        for(QString::const_iterator iter_const(keypath.begin()); iter_const != keypath.end(); ++iter_const)
        {
            if(*iter_const == '/')
                ++hCount;
        }

        insert_query.prepare("UPDATE '" + plist_view_db_table_name + "' SET id = ? WHERE INT = ?");
        insert_query.addBindValue(QString::number(hCount));
        insert_query.addBindValue(int1);
        if(!insert_query.exec())
        {
            recon_static_functions::app_debug("insert_query execution-----FAILED----" , Q_FUNC_INFO);
            recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(select_query.executedQuery() , Q_FUNC_INFO);
            continue;
        }
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}
