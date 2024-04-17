#include "unified_logs_model.h"

unified_logs_model::unified_logs_model()
{
    column_list << "" << "" << "Timestamp" << "Process" << "Event Type" << "Event Message";
}

int unified_logs_model::rowCount(const QModelIndex &parent) const
{
    return _row_count;
}

int unified_logs_model::columnCount(const QModelIndex &parent) const
{
    return column_list.size();
}

QVariant unified_logs_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if(section == enum_unified_logs_seq_no)
            return "";

        else if(section == enum_unified_logs_tags)
            return "";

        else if(section == enum_unified_logs_timestamp)
            return "Timestamp";

        else if(section == enum_unified_logs_process)
            return "Process";

        else if(section == enum_unified_logs_event_type)
            return "Event Type";

        else if(section == enum_unified_logs_event_message)
            return "Event Message";
    }

    return QVariant();
}


void unified_logs_model::pub_set_table_view_obj(m_tableview *obj)
{
    m_tableview_obj = obj;
}

void unified_logs_model::pub_set_source_count_name(QString src_count_name)
{
    source_count_name = src_count_name;
}

void unified_logs_model::pub_update_tag_record(qint64 row, QString tag)
{
    if(tags_list.size() >= row)
    {
        tags_list.removeAt(row);
        tags_list.insert(row, tag);
    }
}


QVariant unified_logs_model::data(const QModelIndex &index, int role) const
{
    QHeaderView *header = m_tableview_obj->horizontalHeader();

    if(index.column() == enum_unified_logs_seq_no && role == Qt::DisplayRole)
    {
        header->resizeSection(index.column(),20);

        if(index.row() > int_list.size())
            return "";

        return int_list.at(index.row());
    }
    else if(index.column() == enum_unified_logs_tags && role == Qt::DecorationRole)
    {
        QIcon icn;
        header->resizeSection(index.column(),25);

        if(index.row() > tags_list.size())
            return icn;

        QString tags = tags_list.at(index.row());

        QString tags_icon_path;
        if(!tags.isEmpty())
        {
            QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
            QString color_name = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags,tag_db_path, Q_FUNC_INFO);
            tags_icon_path = recon_static_functions::get_tags_icon_path(color_name , tags, Q_FUNC_INFO);
        }

        QIcon tag_icon(tags_icon_path);
        return tag_icon;
    }
    else if(index.column() == enum_unified_logs_timestamp && role == Qt::DisplayRole)
    {
        header->resizeSection(index.column(),250);

        if(index.row() > timestamp_list.size())
            return "";

        return timestamp_list.at(index.row());
    }
    else if(index.column() == enum_unified_logs_process && role == Qt::DisplayRole)
    {
        header->resizeSection(index.column(),200);

        if(index.row() > process_list.size())
            return "";

        return process_list.at(index.row());
    }
    else if(index.column() == enum_unified_logs_event_type && role == Qt::DisplayRole)
    {
        header->resizeSection(index.column(),200);

        if(index.row() > event_type_list.size())
            return "";

        return event_type_list.at(index.row());
    }
    else if(index.column() == enum_unified_logs_event_message && role == Qt::DisplayRole)
    {
        header->setStretchLastSection(true);

        if(index.row() > event_message_list.size())
            return "";

        return event_message_list.at(index.row());
    }

    return QVariant();
}

void unified_logs_model::pub_get_log_data_list_from_unified_logs_db()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int_list.clear();
    tags_list.clear();
    timestamp_list.clear();
    process_list.clear();
    event_type_list.clear();
    event_message_list.clear();

    _row_count = 0;

    QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    QString unified_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";
    QString select_command = QString("SELECT db_name from tbl_index WHERE source_count_name = ?");
    QStringList arg_list;
    arg_list << source_count_name;
    QString unified_logs_db = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_command, arg_list, 0, unified_logs_db_path, Q_FUNC_INFO);

    QString unified_dest_path = unified_logs_dir_path + unified_logs_db;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase unified_logs_dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    unified_logs_dest_db.setDatabaseName(unified_dest_path);

    if(!unified_logs_dest_db.open())
    {
        recon_static_functions::app_debug("Database open ---FAILED--- " + unified_logs_dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(unified_logs_dest_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    unified_logs_dest_db.transaction();

    QSqlQuery query_select(unified_logs_dest_db);

    QString count_cmd = "select count(*) from log_data";
    QString total_rec = recon_helper_standard_obj->get_string_from_db_by_dbreference(count_cmd, 0, unified_logs_dest_db, Q_FUNC_INFO);

    qint64 total_records_num = total_rec.toLongLong();

    if(total_records_num > 500000)
    {
        for(qint64 ii = 0; ii < total_records_num; ii+=5000000)
        {
            qint64 total_record_count = 0;
            qint64 countt = 0;

            QString command = QString("SELECT INT, category, timestamp_readable, process, event_type, event_message, recon_tag_value from log_data where INT >= '" + QString::number(ii) + "' AND INT <= '" + QString::number(ii + 4999999) + "'");

            query_select.prepare(command);

            if(!query_select.exec())
            {
                recon_static_functions::app_debug("select query ---FAILED--- " + unified_logs_dest_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Error : " + query_select.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Query : " + query_select.executedQuery(),Q_FUNC_INFO);
                unified_logs_dest_db.close();
                return;
            }

            while(query_select.next())
            {
                countt++;
                if(countt % 500 == 0)
                {
                    countt = 0;
                    QCoreApplication::processEvents();
                }

                int_list.append(query_select.value("INT").toString());
                tags_list.append(query_select.value("recon_tag_value").toString());
                timestamp_list.append(query_select.value("timestamp_readable").toString());
                process_list.append(query_select.value("process").toString());
                event_type_list.append(query_select.value("event_type").toString());
                event_message_list.append(query_select.value("event_message").toString());

                total_record_count++;
            }
        }
    }
    else
    {
        QString command = QString("SELECT INT, category, timestamp_readable, process, event_type, event_message, recon_tag_value from log_data");
        query_select.prepare(command);

        if(!query_select.exec())
        {
            recon_static_functions::app_debug("select query ---FAILED--- " + unified_logs_dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Error : " + query_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Query : " + query_select.executedQuery(),Q_FUNC_INFO);
            unified_logs_dest_db.close();
            return;
        }

        qint64 total_record_count = 0;
        qint64 countt = 0;
        while(query_select.next())
        {
            countt++;
            if(countt % 500 == 0)
            {
                countt = 0;
                QCoreApplication::processEvents();
            }

            int_list.append(query_select.value("INT").toString());
            tags_list.append(query_select.value("recon_tag_value").toString());
            timestamp_list.append(query_select.value("timestamp_readable").toString());
            process_list.append(query_select.value("process").toString());
            event_type_list.append(query_select.value("event_type").toString());
            event_message_list.append(query_select.value("event_message").toString());

            total_record_count++;
        }
    }

    _row_count = total_records_num;
    unified_logs_dest_db.commit();
    unified_logs_dest_db.close();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}
