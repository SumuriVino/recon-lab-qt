#include "thread_db_updation_bookmark.h"

thread_db_updation_bookmark::thread_db_updation_bookmark(QObject *parent) : QObject(parent)
{

}

void thread_db_updation_bookmark::set_record_value_list(QList<struct_global_tagging_tag_notes_db_updation> list_obj)
{

    list_st_global_bookmark_db_updation = list_obj;
}

void thread_db_updation_bookmark::pub_set_updation_status(bool status)
{
    bool_updation_status = status;
}

void thread_db_updation_bookmark::slot_bookmark_updation_start()
{

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(bookmark_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);

        list_st_global_bookmark_db_updation.clear();
        emit finished();
        return;
    }

    QSqlQuery m_query(destination_db);

    if(bool_updation_status)
    {
        QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,bookmark,"
                          "source_count_name,csv_plugin_name, csv_tab_name,recon_tag_value,notes,os_scheme_display,item0,item1,item2,item3) VALUES (?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?,?)";


        for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
        {
            struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);

            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(obj.category);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.table_name);
            m_query.addBindValue(obj.record_no);
            m_query.addBindValue(obj.bookmark);
            m_query.addBindValue(obj.source_count_name);
            m_query.addBindValue(obj.csv_plugin_name);
            m_query.addBindValue(obj.csv_tab_name);
            m_query.addBindValue(obj.recon_tag_value);
            m_query.addBindValue(obj.notes);
            m_query.addBindValue(obj.os_scheme_display);
            m_query.addBindValue(obj.item0);
            m_query.addBindValue(obj.item1);
            m_query.addBindValue(obj.item2);
            m_query.addBindValue(obj.item3);

            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query insertion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }
    }
    else
    {
        QString command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ? AND source_count_name = ?" ;

        for(int tt = 0; tt < list_st_global_bookmark_db_updation.size(); tt++)
        {
            struct_global_tagging_tag_notes_db_updation obj = list_st_global_bookmark_db_updation.at(tt);
            m_query.clear();
            m_query.prepare(command);
            m_query.addBindValue(obj.record_no);
            m_query.addBindValue(obj.plugin_name);
            m_query.addBindValue(obj.tab_name);
            m_query.addBindValue(obj.source_count_name);
            if(!m_query.exec())
            {
                recon_static_functions::app_debug(" m_query deletion ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" Error " + m_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" EXecuted query " + m_query.executedQuery(),Q_FUNC_INFO);
                continue;
            }
        }

    }


    destination_db.close();

    list_st_global_bookmark_db_updation.clear();
    emit finished();

}
