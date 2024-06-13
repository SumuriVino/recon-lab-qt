
#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::slot_abouttoshow_tags_in_submenu()
{
    display_tags_submenu_for_sqlite_viewer();
}
// functions not in use
void sqlite_viewer::slot_export_as_plugin(struct_global_sqlite_viewer_plugin_info obj_info)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    list_selected_tables_for_plugin.clear();

    list_selected_tables_for_plugin = obj_info.selectedTableList;
    QString customPath = obj_info.customPath;


    QString destinationDbFile;

    QDir dir;
    QString saved_DirPath =  destination_result_directory_path + "/SavedStatePlugin"; //

    saved_state_dir_path.clear();
    saved_state_dir_path = saved_DirPath;
    if(!dir.mkpath(saved_DirPath))
    {
        recon_static_functions::app_debug("Saved State Dir Create -----FAILED-------" + saved_DirPath,Q_FUNC_INFO);
    }

    destinationDbFile = saved_state_dir_path + "/plugin_info.sqlite";

    //1. Create Db
    create_destination_database(destinationDbFile);

    //2.-Insert Tables
    create_plugin_Info_db(destinationDbFile);

    //3. Insert Data
    insert_dataIn_plugins_Info_db(destinationDbFile,customPath);

    //4. Save Current State of FilterViewTable
    filter_view_table_saved_state( saved_state_dir_path +"/FilterViewTable_SavedState.sqlite");

    //5. Create PLugin From DB
//    create_plugin_object->setDestinationDirPathForPlugin(savedStateDirPath);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

int sqlite_viewer::create_plugin_Info_db(QString destination_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    db.setDatabaseName(destination_file);
    if(!db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_file,Q_FUNC_INFO);
        return -1;
    }

    QSqlQuery dest_query(db);
    QString command;
    command = "CREATE TABLE plugin_info"
              "(INT INTEGER PRIMARY KEY,"
              "table_name varchar(100),"
              "received_icon varchar(100),"
              "lib_name varchar(100),"
              "lib_path varchar(100),"
              "icon_path varchar(100),"
              "artifacts_source varchar(100))";

    if(!dest_query.exec(command))
    {
        recon_static_functions::app_debug("plugin_info destination table-----FAILED-------",Q_FUNC_INFO);
        recon_static_functions::app_debug(dest_query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(command,Q_FUNC_INFO);
    }

    db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return 1;
}

int sqlite_viewer::insert_dataIn_plugins_Info_db(QString destiantion_file, QString artifactsPath)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    db.setDatabaseName(destiantion_file);
    if(!db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(db.lastError().text(),Q_FUNC_INFO);
        return -1;
    }

    QString source_path;

    if(artifactsPath == "")
    {
        QString tmp_path = artifactsPath;

        if(tmp_path.startsWith("/Users/"))
        {
            tmp_path.remove("/Users/");
            QStringList tmp_list = tmp_path.split("/", Qt::SkipEmptyParts);
            tmp_list.removeAt(0);

            source_path = tmp_list.join("/");

            source_path = "/" + source_path;
        }
    }
    else
    {
        source_path = artifactsPath;
    }

    int count = 0;
    for(int i = 0; i < list_selected_tables_for_plugin.size(); i++)
    {
        QSqlQuery dest_query(db);

        count++;

        QString command = ("INSERT INTO plugin_info (INT, table_name, artifacts_source) VALUES (?,?,?)");
        dest_query.prepare(command);
        dest_query.addBindValue(QString::number(count));
        dest_query.addBindValue(list_selected_tables_for_plugin.at(i));
        dest_query.addBindValue(source_path);
        if( !dest_query.exec() )
        {
            recon_static_functions::app_debug("query dest_query ------FAILED-------- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(dest_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(dest_query.executedQuery(),Q_FUNC_INFO);
            continue;
        }
    }

    db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}
