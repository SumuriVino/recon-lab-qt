#include "artifacts_timeline_full_loader.h"

void artifacts_timeline_full_loader::pub_initialize_and_display_plugins_and_timestamps()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    QString command = "select distinct plugin_name from tbl_plugin_status";
    QStringList plugin_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,db_path,Q_FUNC_INFO);

    QStringList run_plugin_withtab_list = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);

    for(int zz = 0; zz < plugin_list.size(); zz++)
    {
        QCoreApplication::processEvents();

        QString plugin_name = plugin_list.at(zz);
        QString plugin_name_tmp = plugin_name.trimmed();
        plugin_name_tmp = plugin_name_tmp.replace(" ", "_");

        if(!global_csv_reader_class_selected_obj->is_timeline_available_in_plugin(plugin_name))
            continue;

        QStringList time_tabs_list;
        QStringList tab_list = recon_static_functions::get_associated_tabs(plugin_name,run_plugin_withtab_list, Q_FUNC_INFO);
        for(int tb = 0; tb < tab_list.size(); tb++)
        {
            if(!global_csv_reader_class_selected_obj->is_timeline_available_in_tab(plugin_name,tab_list.at(tb)))
                continue;
            time_tabs_list << tab_list.at(tb);
        }

        if(time_tabs_list.size() <= 0)
            continue;

        QString plugin_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Plugin_Result_Dir_QString).toString() + plugin_name_tmp +  "/" + plugin_name_tmp + ".sqlite";
        if(recon_helper_standard_obj->get_db_total_record_by_dbpath(true, plugin_db_path, QStringList(), Q_FUNC_INFO) <= 0)
            continue;

        for(int tb = 0; tb < time_tabs_list.size(); tb++)
        {
            QStringList list1;
            list1 << plugin_name << time_tabs_list.at(tb);

            QString check_exist = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select INT from saved_events where plugin =? AND category=?",list1, 0,destination_timeline_saved_db_path,Q_FUNC_INFO);
            if(!check_exist.trimmed().isEmpty())
                continue;

            QString command = "insert into saved_events (bookmark, plugin,category)values(?,?,?)";
            QStringList list;
            list << "0" << plugin_name << time_tabs_list.at(tb);

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, list,destination_timeline_saved_db_path,Q_FUNC_INFO);
        }


        map_plugin_with_tab_list.insert(plugin_name, time_tabs_list);
    }

    display_timestamps_list();

    display_plugins_in_treewidget();


    ///====Blank Chart on start====///

    QBarSet *set0 = new QBarSet("");
    QBarSeries *series = new QBarSeries();
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);

    QStringList categories;
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(false);

    QChartView *blank_chart = new QChartView(chart);
    stackwidget_obj->addWidget(blank_chart);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::display_plugins_in_treewidget()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QMap <QString, QStringList>::iterator mapiter;


    bool bool_check_is_any_plugin_bookmarked = false;

    treewidget_plugins_list->setHeaderLabel("Plugins");
    for(mapiter = map_plugin_with_tab_list.begin(); mapiter != map_plugin_with_tab_list.end() ; mapiter++)
    {
        QTreeWidgetItem *item_plugin = new QTreeWidgetItem;

        item_plugin->setText(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name, mapiter.key());
        item_plugin->setFlags(item_plugin->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item_plugin->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Unchecked);


        treewidget_plugins_list->addTopLevelItem(item_plugin);


        for(int i = 0; i < mapiter.value().size(); i++)
        {
            QString value = mapiter.value().at(i);

            QTreeWidgetItem *item_tab = new QTreeWidgetItem;
            item_tab->setFlags(item_tab->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            item_tab->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Unchecked);

            item_tab->setText(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name, value);

            QString command = "select bookmark from saved_events where plugin = ? AND category = ?";
            QStringList list;
            list << mapiter.key() << value;

            QString is_bookmark = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, list,0,destination_timeline_saved_db_path,Q_FUNC_INFO);

            if(is_bookmark == "1")
            {
                bool_check_is_any_plugin_bookmarked = true;
                item_tab->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Checked);
                item_plugin->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Checked);

            }

            item_plugin->addChild(item_tab);

        }

    }


    treewidget_plugins_list->setColumnWidth(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,200);


    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::set_essentail()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    xaxis_title_str = "Time";
    yaxis_title_str = "No. of Events";

    years_list.clear();
    month_year_list.clear();
    day_month_year_list.clear();
    hour_day_month_year_list.clear();


    QDateTime startDate;
    QDateTime endDate;

    startDate.setDate(datetimeedit_start->date());
    endDate.setDate(datetimeedit_end->date());

    quint64 starttime = datetimeedit_start->time().msecsSinceStartOfDay()/1000;
    quint64 endtime = datetimeedit_end->time().msecsSinceStartOfDay()/1000;

    quint64 start_date_epch = startDate.toSecsSinceEpoch();
    quint64 end_date_epch = endDate.toSecsSinceEpoch();

    quint64 temp_start_epchtime = 0;
    quint64 temp_end_epchtime = 0;

    //    m_barset *barset_obj = new m_barset(plugin_name);
    //    // set_barset_colors_according_to_plugins(*barset_obj, plugin_name);

    //    barset_obj->setBorderColor(Qt::gray);

    temp_start_epchtime = start_date_epch + starttime;
    temp_end_epchtime = end_date_epch + endtime;



    qint64 time_between = temp_end_epchtime - temp_start_epchtime;

    qint64 no_of_hours = time_between/3600;

    quint64 start_time_add_an_hour = 0;

    start_time_add_an_hour = temp_start_epchtime;

    progressbar_increment_step = no_of_hours / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;


    for(qint64 hours = 0; hours < no_of_hours; hours++)
    {
        if(start_time_add_an_hour > temp_end_epchtime)
            break;



        if(hours % 100 == 0)
        {
            QCoreApplication::processEvents();
        }



        QDateTime dttime;
        dttime.setTimeSpec(Qt::UTC);
        dttime = dttime.fromSecsSinceEpoch(start_time_add_an_hour);


        QString yearr = QString::number(dttime.date().year());

        QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
        QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");

        QString mnth_year = QString(dttime.date().toString("MMM") + "-" + QString::number(dttime.date().year()));

        if(!hour_day_month_year_list.contains(hour_day_mnth_year))
        {
            hour_day_month_year_list << hour_day_mnth_year;
        }

        if(!day_month_year_list.contains(day_mnth_year))
        {
            day_month_year_list << day_mnth_year;
        }

        if(!years_list.contains(yearr))
        {
            years_list << yearr;
        }


        if(!month_year_list.contains(mnth_year))
        {
            month_year_list << mnth_year;
        }

        start_time_add_an_hour += 3600;

        // temp_start_epchtime = start_time_add_an_hour;
    }



    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_timeline_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("----FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,plugin,category from timeline_data where timestamp >= ?"
                  " AND timestamp <= ? ORDER BY timestamp ASC");


    query.addBindValue(QString::number(temp_start_epchtime));
    query.addBindValue(QString::number(temp_end_epchtime));

    if(!query.exec())
    {
        recon_static_functions::app_debug("query --------FAILED------ ", Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    map_of_struct_graph_data.clear();
    while(query.next())
    {
        QCoreApplication::processEvents();

        struct_graphs_essentials struct_data_database_obj;

        struct_data_database_obj.int_value = query.value(0).toString();
        struct_data_database_obj.timestamp = query.value(1).toString();

        qint64 tmp_date_int = query.value(1).toString().toLongLong();
        tmp_date_int += global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();

        QDateTime dttime;
        dttime.toUTC();
        dttime = dttime.fromSecsSinceEpoch(tmp_date_int, Qt::UTC);

        QString yearr = QString::number(dttime.date().year());
        QString mnth_year = QString(dttime.date().toString("MMM") + "-" + QString::number(dttime.date().year()));
        QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
        QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");

        struct_data_database_obj.year_value = yearr;
        struct_data_database_obj.month_year_value = mnth_year;
        struct_data_database_obj.day_month_year_value = day_mnth_year;
        struct_data_database_obj.hour_day_month_year_value = hour_day_mnth_year;




        struct_data_database_obj.plugin_name = query.value(6).toString();
        struct_data_database_obj.tab_name = query.value(7).toString();

        map_of_struct_graph_data.insert(struct_data_database_obj.year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.month_year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.day_month_year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.hour_day_month_year_value,struct_data_database_obj);
    }



    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::set_graphs_essentials_for_plugins_data_only()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    if(start_timestamp_utc_qint64 > end_timestamp_utc_qint64)
    {
        return;
    }


    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_timeline_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug(" ---FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    QMultiMap<qint64,QStringList> multimap_date_yr_mnth_hour;

    QMap<QString, QStringList>::iterator iter;

    //    QSqlDatabase memory_db = QSqlDatabase::addDatabase("QSQLITE");
    //    memory_db.setDatabaseName(":memory:");
    //    if(!memory_db.open())
    //    {
    //        recon_static_functions::app_debug(" Database in memory open ---FAILED---" + memory_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" Error: " + memory_db.lastError().text(), Q_FUNC_INFO);
    //        return;
    //    }


    //    memory_db.transaction();
    //    QSqlQuery query_create_table(memory_db);

    //    QString db_create_command = "create table timeline_data ( int INTEGER, timestamp INTEGER, year varchar, month_year varchar, day_month_year_value varchar, hour_day_month_year_value varchar, plugin_name varchar, category varchar)";
    //    query_create_table.prepare(db_create_command);

    //    if(!query_create_table.exec())
    //    {
    //        recon_static_functions::app_debug(" query memory table create ----FAILED----  "+ memory_db.databaseName(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + db_create_command,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        memory_db.close();
    //        return;
    //    }

    int temp_counter = 0;

    for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
    {
        QStringList list_of_values;

        QString plugin_name = iter.key();
        QString db_cmd = "select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,plugin,category from timeline_data ";

        db_cmd.append(" where plugin = ? AND (");
        list_of_values << plugin_name;

        for(int tab = 0; tab < iter.value().size(); tab++)
        {
            db_cmd.append(" category = ? OR ");
            list_of_values << iter.value().at(tab);
        }

        if(db_cmd.endsWith("OR "))
        {
            db_cmd.chop(3);
            db_cmd.append(") AND (");
        }

        for(int tab = 0; tab < bookmarked_timestamp_type_list.size(); tab++)
        {
            db_cmd.append(" stamp_type = ? OR ");
            list_of_values << bookmarked_timestamp_type_list.at(tab);
        }

        if(db_cmd.endsWith("OR "))
        {
            db_cmd.chop(3);
            db_cmd.append(")");
        }


        db_cmd.append(" AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");
        list_of_values << QString::number(start_timestamp_utc_qint64) << QString::number(end_timestamp_utc_qint64);


        query.prepare(db_cmd);


        for(int i = 0; i < list_of_values.size(); i++)
        {
            query.addBindValue(list_of_values.at(i));
        }

        if(!query.exec())
        {
            recon_static_functions::app_debug("query ----FAILED--- " + source_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);

            //source_db.close();
            //return;
            continue;

        }

        while(query.next())
        {
            if(temp_counter++ % 500 == 0)
                QCoreApplication::processEvents();

            //            QString insert_command = "insert into timeline_data ( int, timestamp, year, month_year, day_month_year_value, hour_day_month_year_value, plugin_name, category) values(?,?,?,?,?,?,?,?)";
            //            QStringList args_list;
            //            args_list << query.value(0).toString();
            //            args_list << query.value(1).toString();
            //            args_list << query.value(2).toString();
            //            args_list << query.value(3).toString();
            //            args_list << query.value(4).toString();
            //            args_list << query.value(5).toString();
            //            args_list << query.value(6).toString();
            //            args_list << query.value(7).toString();


            //            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_command, args_list, memory_db, Q_FUNC_INFO);


            qint64 timestamp = query.value(1).toULongLong();

            qint64 tmp_date_int = timestamp;
            tmp_date_int += global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();

            QDateTime dttime;
            dttime.toUTC();
            dttime = dttime.fromSecsSinceEpoch(tmp_date_int, Qt::UTC);

            QString yearr = QString::number(dttime.date().year());
            QString mnth_year = QString(dttime.date().toString("MMM") + "-" + QString::number(dttime.date().year()));
            QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
            QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");


            QStringList list;
            list<< yearr << mnth_year << day_mnth_year << hour_day_mnth_year;

            multimap_date_yr_mnth_hour.insert(timestamp,list);

            struct_graphs_essentials struct_data_database_obj;

            struct_data_database_obj.int_value = query.value(0).toString();
            struct_data_database_obj.timestamp = query.value(1).toString();

            struct_data_database_obj.year_value = yearr;
            struct_data_database_obj.month_year_value = mnth_year;
            struct_data_database_obj.day_month_year_value = day_mnth_year;
            struct_data_database_obj.hour_day_month_year_value = hour_day_mnth_year;


            struct_data_database_obj.plugin_name = query.value(6).toString();
            struct_data_database_obj.tab_name    = query.value(7).toString();

            map_of_struct_graph_data.insert(struct_data_database_obj.year_value,struct_data_database_obj);
            map_of_struct_graph_data.insert(struct_data_database_obj.month_year_value,struct_data_database_obj);
            map_of_struct_graph_data.insert(struct_data_database_obj.day_month_year_value,struct_data_database_obj);
            map_of_struct_graph_data.insert(struct_data_database_obj.hour_day_month_year_value,struct_data_database_obj);

        }
    }

    //memory_db.commit();

    source_db.close();

    //    QSqlQuery select_query(memory_db);

    //    QString memory_select_command = "select int, timestamp, year, month_year, day_month_year_value, hour_day_month_year_value, plugin_name, category from timeline_data order by timestamp ASC";


    //    if(!select_query.exec(memory_select_command))
    //    {
    //        recon_static_functions::app_debug(" query memory table create ----FAILED----  "+ memory_db.databaseName(),Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" command -  " + db_create_command,Q_FUNC_INFO);
    //        recon_static_functions::app_debug(" error -  " + query_create_table.lastError().text(),Q_FUNC_INFO);
    //        memory_db.close();
    //        source_db.close();
    //        return;
    //    }

    //    int temp_counter2 = 0;
    //    while(select_query.next())
    //    {

    //        if(temp_counter2++ % 500 == 0)
    //            QCoreApplication::processEvents();

    //        qint64 timestamp = select_query.value(1).toULongLong();

    //        qint64 tmp_date_int = timestamp;
    //        tmp_date_int += global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();

    //        QDateTime dttime;
    //        dttime.toUTC();
    //        dttime = dttime.fromSecsSinceEpoch(tmp_date_int, Qt::UTC);

    //        QString yearr = QString::number(dttime.date().year());
    //        QString mnth_year = QString(dttime.date().shortMonthName(dttime.date().month()) + "-" + QString::number(dttime.date().year()));
    //        QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
    //        QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");




    //        QStringList list;
    //        list<< yearr << mnth_year << day_mnth_year << hour_day_mnth_year;

    //        multimap_date_yr_mnth_hour.insert(timestamp,list);

    //        struct_graphs_essentials struct_data_database_obj;

    //        struct_data_database_obj.int_value = select_query.value(0).toString();
    //        struct_data_database_obj.timestamp = select_query.value(1).toString();

    //        struct_data_database_obj.year_value = yearr;
    //        struct_data_database_obj.month_year_value = mnth_year;
    //        struct_data_database_obj.day_month_year_value = day_mnth_year;
    //        struct_data_database_obj.hour_day_month_year_value = hour_day_mnth_year;



    //        struct_data_database_obj.plugin_name = select_query.value(6).toString();
    //        struct_data_database_obj.tab_name = select_query.value(7).toString();

    //        map_of_struct_graph_data.insert(struct_data_database_obj.year_value,struct_data_database_obj);
    //        map_of_struct_graph_data.insert(struct_data_database_obj.month_year_value,struct_data_database_obj);
    //        map_of_struct_graph_data.insert(struct_data_database_obj.day_month_year_value,struct_data_database_obj);
    //        map_of_struct_graph_data.insert(struct_data_database_obj.hour_day_month_year_value,struct_data_database_obj);
    //    }

    //    memory_db.close();


    QMultiMap<qint64,QStringList>::iterator iter1;
    for(iter1 = multimap_date_yr_mnth_hour.begin(); iter1 != multimap_date_yr_mnth_hour.end(); iter1++)
    {

        QStringList list = iter1.value();

        if(list.size() < 4)
            continue;

        QString yearr = list.at(0);
        QString mnth_year = list.at(1);
        QString day_mnth_year = list.at(2);
        QString hour_day_mnth_year = list.at(3);


        if(!hour_day_month_year_list.contains(hour_day_mnth_year))
        {
            hour_day_month_year_list << hour_day_mnth_year;
        }

        if(!day_month_year_list.contains(day_mnth_year))
        {
            day_month_year_list << day_mnth_year;
        }

        if(!years_list.contains(yearr))
        {
            years_list << yearr;
        }

        if(!month_year_list.contains(mnth_year))
        {
            month_year_list << mnth_year;
        }

    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}


void artifacts_timeline_full_loader::set_graphs_essentials_for_filesystem_data_only(QStringList fs_db_list)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);



    if(start_timestamp_utc_qint64 > end_timestamp_utc_qint64)
    {
        return;
    }

    for(int count = 0; count < fs_db_list.size(); count++)
    {

        if(bool_cancel_loading)
            break;

        QSqlDatabase::removeDatabase(Q_FUNC_INFO );
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(fs_db_list.at(count));
        if(!source_db.open())
        {
            recon_static_functions::app_debug(" ---FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + source_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QSqlQuery query(source_db);

        QMultiMap<qint64,QStringList> multimap_date_yr_mnth_hour;

        QStringList list_of_values;

        QString cmd_records = "Select count (*) from timeline_data ";

        QString db_cmd = "select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,plugin,category from timeline_data";

        db_cmd.append(" where plugin = ? AND (");
        cmd_records.append( " where plugin = ? AND (");
        list_of_values << QString(MACRO_Plugin_Name_File_System);

        db_cmd.append(" category = ? OR ");
        cmd_records.append(" category = ? OR ");

        list_of_values << QString(MACRO_Plugin_File_System_TAB_Name_Files);

        if(db_cmd.endsWith("OR "))
        {
            db_cmd.chop(3);
            db_cmd.append(") AND (");
        }

        if(cmd_records.endsWith("OR "))
        {
            cmd_records.chop(3);
            cmd_records.append(") AND (");
        }


        for(int tab = 0; tab < bookmarked_timestamp_type_list.size(); tab++)
        {
            db_cmd.append(" stamp_type = ? OR ");
            cmd_records.append(" stamp_type = ? OR ");
            list_of_values << bookmarked_timestamp_type_list.at(tab);
        }

        if(db_cmd.endsWith("OR "))
        {
            db_cmd.chop(3);
            db_cmd.append(")");
        }

        if(cmd_records.endsWith("OR "))
        {
            cmd_records.chop(3);
            cmd_records.append(")");
        }


        db_cmd.append(" AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");
        cmd_records.append(" AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");
        list_of_values << QString::number(start_timestamp_utc_qint64) << QString::number(end_timestamp_utc_qint64);

        QString total_records_count_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd_records , list_of_values, 0, source_db , Q_FUNC_INFO);
        total_records_count_str = total_records_count_str.trimmed();
        qint64 total_records_count = 0;

        if(!total_records_count_str.isEmpty())
            total_records_count = total_records_count_str.toLongLong();

        query.prepare(db_cmd);

        for(int i = 0; i < list_of_values.size(); i++)
        {
            query.addBindValue(list_of_values.at(i));
        }

        if(!query.exec())
        {
            recon_static_functions::app_debug("query ----FAILED--- " + source_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
            source_db.close();
            return;
        }

        qint64 records_processed = 0;

        while(query.next())
        {
            QCoreApplication::processEvents();

            if(bool_cancel_loading)
                break;

            records_processed++;
            if(records_processed % 300 == 0)
            {
                loading_display_dialog_based_on_records(total_records_count,records_processed);
            }

            qint64 timestamp = query.value(1).toULongLong();

            qint64 tmp_date_int = timestamp;
            tmp_date_int += global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong();

            QDateTime dttime;
            dttime.toUTC();
            dttime = dttime.fromSecsSinceEpoch(tmp_date_int, Qt::UTC);

            QString yearr = QString::number(dttime.date().year());
            QString mnth_year = QString(dttime.date().toString("MMM") + "-" + QString::number(dttime.date().year()));

            QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
            QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");



            QStringList list;
            list<< yearr << mnth_year << day_mnth_year << hour_day_mnth_year;

            multimap_date_yr_mnth_hour.insert(timestamp,list);

            struct_graphs_essentials struct_data_database_obj;

            struct_data_database_obj.int_value = query.value(0).toString();
            struct_data_database_obj.timestamp = query.value(1).toString();

            struct_data_database_obj.year_value = yearr;
            struct_data_database_obj.month_year_value = mnth_year;
            struct_data_database_obj.day_month_year_value = day_mnth_year;
            struct_data_database_obj.hour_day_month_year_value = hour_day_mnth_year;

            struct_data_database_obj.plugin_name = query.value(6).toString();
            struct_data_database_obj.tab_name = query.value(7).toString();

            map_of_struct_graph_data.insert(struct_data_database_obj.year_value,struct_data_database_obj);
            map_of_struct_graph_data.insert(struct_data_database_obj.month_year_value,struct_data_database_obj);
            map_of_struct_graph_data.insert(struct_data_database_obj.day_month_year_value,struct_data_database_obj);
            map_of_struct_graph_data.insert(struct_data_database_obj.hour_day_month_year_value,struct_data_database_obj);

        }
        display_loading_progress_bar_obj->hide();

        QMultiMap<qint64,QStringList>::iterator iter1;
        for(iter1 = multimap_date_yr_mnth_hour.begin(); iter1 != multimap_date_yr_mnth_hour.end(); iter1++)
        {
            QStringList list = iter1.value();

            if(list.size() < 4)
                continue;

            QString yearr = list.at(0);
            QString mnth_year = list.at(1);
            QString day_mnth_year = list.at(2);
            QString hour_day_mnth_year = list.at(3);


            if(!hour_day_month_year_list.contains(hour_day_mnth_year))
            {
                hour_day_month_year_list << hour_day_mnth_year;
            }

            if(!day_month_year_list.contains(day_mnth_year))
            {
                day_month_year_list << day_mnth_year;
            }

            if(!years_list.contains(yearr))
            {
                years_list << yearr;
            }

            if(!month_year_list.contains(mnth_year))
            {
                month_year_list << mnth_year;
            }

        }

        source_db.close();

    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::create_rightclick_options()
{
    mainMenu->clear();

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark, menuList);

    if(menuList.isEmpty())
    {
        create_default_right_click();
    }
    else
    {
        sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
        create_tags_submenu();

        connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
        connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Add_Note)
                 << QString(MACRO_Generic_Right_Click_Remove_Note);
        submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
        create_right_click_actions(submenu_note, menuList);

        ///----Create Submenu of existing Menu
        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        act_full_detail_info = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Open_In_Window),mainMenu);

        menuList.clear();
        menuList << QString(MACRO_Generic_Right_Click_Copy_To_Clipboard) ;
        create_right_click_actions(mainMenu , menuList);

        create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

        submenu_go_to_artifact_source = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Go_To_Artifacts_Source) , mainMenu , submenu_go_to_artifact_source);
        connect(submenu_go_to_artifact_source,SIGNAL(triggered(QAction*)),this,SLOT(slot_goto_artifact_source_submenu_clicked(QAction*)));

        act_go_to_record = create_and_get_right_click_action(QString(MACRO_Generic_Right_Click_Go_To_Record),mainMenu);
        create_right_click_actions(mainMenu , QStringList(""));
    }

}


void artifacts_timeline_full_loader::save_timeline_data_in_case_treewidget(QString tab_to_be_saved)
{
    QString tab_name;
    saved_db_path.clear();
    QStringList plugin_tab_list;

    ///for all data
    if(is_clicked_to_save_all_data)
    {
        tab_name =  tab_to_be_saved + ".sqlite";
        saved_db_path = create_db_for_saved_artifacts(tab_name);
        //        if(saved_db_path.trimmed().isEmpty())
        //            return true;

        QMap<QString, QStringList>::iterator iter;
        for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
        {
            QString target_plugin = iter.key();
            plugin_tab_list = iter.value();

            QString fs_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
            //            for(int row = 0; row < tablewidget_filesystem_sources->rowCount(); row++)
            //            {
            //                QTableWidgetItem *item_source = tablewidget_filesystem_sources->item(row , 0);

            //                if(item_source->checkState() == Qt::Unchecked)
            //                    continue;

            //                QTableWidgetItem *item_source_count_name = tablewidget_filesystem_sources->item(row , 1);

            //                destination_timeline_db_path = fs_result_dir_path  + "file_system_timeline_full_" + item_source_count_name->text() + ".sqlite";
            //                extract_and_save_result(target_plugin,plugin_tab_list,tab_to_be_saved);
            //            }

            bool any_plugin_checked = false;
            for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
            {
                if(treewidget_plugins_list->topLevelItem(tp)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
                {
                    any_plugin_checked = true;
                    break;
                }
            }

            if(any_plugin_checked)
            {
                destination_timeline_db_path = fs_result_dir_path + "artifacts_timeline_full.sqlite";

                extract_and_save_result(target_plugin,plugin_tab_list,tab_to_be_saved);
            }

        }
    }
    else
    {
        ///for single data
        tab_name =  tab_to_be_saved + ".sqlite";
        saved_db_path = create_db_for_saved_artifacts(tab_name);
        //        if(saved_db_path.trimmed().isEmpty())
        //            return true;

        plugin_tab_list = bookmarked_plugin_tab_list;

        QString fs_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
        //        for(int row = 0; row < tablewidget_filesystem_sources->rowCount(); row++)
        //        {
        //            QTableWidgetItem *item_source = tablewidget_filesystem_sources->item(row , 0);

        //            if(item_source->checkState() == Qt::Unchecked)
        //                continue;

        //            QTableWidgetItem *item_source_count_name = tablewidget_filesystem_sources->item(row , 1);

        //            destination_timeline_db_path = fs_result_dir_path  + "file_system_timeline_full_" + item_source_count_name->text() + ".sqlite";
        //            extract_and_save_result(bookmarked_plugin_name,plugin_tab_list,tab_to_be_saved);
        //        }

        bool any_plugin_checked = false;
        for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
        {
            if(treewidget_plugins_list->topLevelItem(tp)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
            {
                any_plugin_checked = true;
                break;
            }
        }

        if(any_plugin_checked)
        {
            destination_timeline_db_path = fs_result_dir_path + "artifacts_timeline_full.sqlite";

            extract_and_save_result(bookmarked_plugin_name,plugin_tab_list,tab_to_be_saved);
        }

    }

    saved_db_info(tab_name);
    emit signal_save_tabs_from_artifacts_timeline(tab_name);


}

void artifacts_timeline_full_loader::create_graphs()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    radiobutton_create_graph_yearly->setChecked(true);
    int count_stacks = stackwidget_obj->count();

    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
        }
    }

    label_show_date_time->clear();
    label_show_pages->clear();

    stackwidget_index = 0;


    radiobutton_create_graph_yearly->setDisabled(true);
    radiobutton_create_graph_monthly->setDisabled(true);
    radiobutton_create_graph_daywise->setDisabled(true);
    radiobutton_create_graph_hourly->setDisabled(true);

    map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.clear();
    map_of_chartview.clear();
    list_of_charview.clear();

    create_graph_yearly();
    map_of_chartview.insert(enum_artifacts_timeline_graphs_yearly,list_of_charview);
    map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.insert(enum_artifacts_timeline_graphs_yearly,map_of_pageno_and_timestamplabel);
    if(radiobutton_create_graph_yearly->isChecked())
    {
        list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_yearly);
        map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_yearly);

        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }


    list_of_charview.clear();
    create_graph_monthly();
    map_of_chartview.insert(enum_artifacts_timeline_graphs_monthly,list_of_charview);
    map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.insert(enum_artifacts_timeline_graphs_monthly,map_of_pageno_and_timestamplabel);
    if(radiobutton_create_graph_monthly->isChecked())
    {

        list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_monthly);
        map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_monthly);
        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }


    list_of_charview.clear();
    create_graph_daywise();
    map_of_chartview.insert(enum_artifacts_timeline_graphs_daywise,list_of_charview);
    map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.insert(enum_artifacts_timeline_graphs_daywise,map_of_pageno_and_timestamplabel);
    if(radiobutton_create_graph_daywise->isChecked())
    {
        map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_daywise);
        list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_daywise);
        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);
        }
    }


    list_of_charview.clear();

    radiobutton_create_graph_yearly->setDisabled(false);
    radiobutton_create_graph_monthly->setDisabled(false);
    radiobutton_create_graph_daywise->setDisabled(false);
    radiobutton_create_graph_hourly->setDisabled(false);

    if(stackwidget_obj->count() <= 1)
    {
        pushbutton_previous->setEnabled(false);
        pushbutton_next->setEnabled(false);
    }else
    {
        pushbutton_previous->setEnabled(false);
        pushbutton_next->setEnabled(true);
    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::create_graph_yearly()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //    if(!years_list.isEmpty())
    //        years_list.sort();

    int no_of_years_on_single_page = 5;
    int divednd_result = 0;
    if(years_list.size() > no_of_years_on_single_page)
    {

        qint64 size = years_list.size();

        int remainder = size % no_of_years_on_single_page;

        divednd_result = size/no_of_years_on_single_page;

        if(remainder != 0)
            divednd_result++;
    }

    int no_of_pages = 1;//By Default No of Pages are 1

    if(divednd_result > 0)
        no_of_pages = divednd_result;

    int tmp_year_count = 0;

    stackwidget_index = 0;
    for(int page_i = 0; page_i < no_of_pages; page_i++)
    {
        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;
        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString, QStringList>::iterator iter;
        for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
        {
            QString plugin_name = iter.key();
            m_barset *barset_obj = new m_barset(plugin_name);
            map_list_of_barsets.insert(plugin_name, barset_obj);
        }

        QStringList listofyears_for_single_page;
        if(tmp_year_count < years_list.size())
        {
            listofyears_for_single_page= years_list.mid(tmp_year_count,no_of_years_on_single_page);
            tmp_year_count+=no_of_years_on_single_page;
        }
        else
        {
            listofyears_for_single_page = years_list.mid(tmp_year_count,years_list.size());
        }

        if(listofyears_for_single_page.size() > 0)
        {
            QString year_label = listofyears_for_single_page.at(0) + "-" + listofyears_for_single_page.at(listofyears_for_single_page.size() - 1);
            map_of_pageno_and_timestamplabel.insert(page_i, year_label);
        }

        int index_in_x_axis = 0;
        for(int k = 0; k < listofyears_for_single_page.size(); k++)
        {
            QString yr_name = listofyears_for_single_page.at(k);
            categories_list << yr_name;

            int max_count_in_y_axis = 0;
            QMap<QString, QStringList>::iterator iter;
            for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
            {
                QCoreApplication::processEvents();

                QString plugin_name = iter.key();
                QStringList tab_list = iter.value();

                m_barset *barset_obj = map_list_of_barsets.value(plugin_name);
                barset_obj->setBorderColor(Qt::gray);

                qint64 count = 0;
                QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(yr_name);
                for(int jj = 0;  jj < list_of_struct.size(); jj++)
                {
                    struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);
                    if((struct_data_database_obj.plugin_name == plugin_name) && tab_list.contains(struct_data_database_obj.tab_name))
                    {
                        count++;
                    }
                }

                max_count_in_y_axis+=count;
                *barset_obj << count;

                QString tooltip = prepare_and_get_tooltip_string_for_barset(count, yr_name, barset_obj->label());

                QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                map_insert_index_with_category.insert(index_in_x_axis,tooltip);
                barset_obj->set_tooltip(map_insert_index_with_category);
                map_list_of_barsets.insert(plugin_name, barset_obj);
            }

            if(max_range_for_yaxis <= max_count_in_y_axis)
                max_range_for_yaxis = max_count_in_y_axis;

            index_in_x_axis++;

        }

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries();
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        QMap<QString, QStringList>::iterator iter2;
        for(iter2 = map_bookmarked_plugin_with_tab_list.begin() ; iter2 != map_bookmarked_plugin_with_tab_list.end(); iter2++)
        {
            QString plugin_name = iter2.key();
            m_barset *barset_obj = map_list_of_barsets.value(plugin_name);
            stacked_bar_series->append(barset_obj);

        }

        qint64 roundup_value_axis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_yaxis = roundup_value_axis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;
        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::create_graph_monthly()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //    years_list.sort();

    progressbar_increment_step = years_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    stackwidget_index = 0;
    map_of_pageno_and_timestamplabel.clear();
    for(int i = 0; i < years_list.size(); i++)
    {

        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        QString year = years_list.at(i);


        if(stackwidget_obj->count() > 1)
            pushbutton_next->setEnabled(true);

        map_of_pageno_and_timestamplabel.insert(i, year);

        int index = 0;

        QStackedBarSeries *new_stacked_bar_series = new QStackedBarSeries();
        connect(new_stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;

        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString, QStringList>::iterator iter;

        for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
        {
            QString plugin_name = iter.key();
            m_barset *barset_obj = new m_barset(plugin_name);
            map_list_of_barsets.insert(plugin_name, barset_obj);

        }

        QStringList months_sorted_list = get_sorted_months_list(month_year_list);
        month_year_list.clear();
        month_year_list << months_sorted_list;


        for(int k = 0; k < month_year_list.size(); k++)
        {
            QString mnth_name = month_year_list.at(k);

            if(mnth_name.endsWith(year))
            {

                QString tmp_name = mnth_name;
                tmp_name.remove(tmp_name.indexOf("-"),tmp_name.size());

                categories_list << tmp_name;

                QMap<QString, QStringList>::iterator iter;
                int max_count = 0;
                for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
                {
                    QCoreApplication::processEvents();

                    QString plugin_name = iter.key();
                    QStringList tab_list = iter.value();


                    m_barset *barset_obj = map_list_of_barsets.value(plugin_name);

                    barset_obj->setBorderColor(Qt::gray);


                    qint64 count = 0;
                    QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(mnth_name);
                    for(int jj = 0;  jj < list_of_struct.size(); jj++)
                    {
                        struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);
                        if((struct_data_database_obj.plugin_name == plugin_name) && tab_list.contains(struct_data_database_obj.tab_name))
                        {
                            count++;
                        }
                    }


                    //                    struct_timeline_command_value struct_obj = prepare_command_and_values_list("COUNT(*)",table,plugin_name,tab_list, QString::number(start_timestamp_int),QString::number(end_timestamp_int), "month_year", mnth_name);
                    //                    qint64 count = recon_static_functions::get_intvalue_from_db_with_addbindvalue_by_dbpath(struct_obj.command,destination_timeline_db_path,0,struct_obj.values_list,Q_FUNC_INFO);

                    max_count += count;

                    *barset_obj << count;

                    //                    QString tooltip = "Events - " +  QString::number(count) + "\n";
                    //                    tooltip.append("Category - "+ mnth_name + "\n");
                    //                    tooltip.append("Plugin - "+ barset_obj->label());

                    QString tooltip = prepare_and_get_tooltip_string_for_barset(count, mnth_name, barset_obj->label());

                    QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                    map_insert_index_with_category.insert(index,tooltip);
                    barset_obj->set_tooltip(map_insert_index_with_category);
                    map_list_of_barsets.insert(plugin_name, barset_obj);

                }


                if(max_range_for_yaxis <= max_count)
                    max_range_for_yaxis = max_count;

                index++;
            }


        }

        QMap<QString, QStringList>::iterator iter2;

        for(iter2 = map_bookmarked_plugin_with_tab_list.begin() ; iter2 != map_bookmarked_plugin_with_tab_list.end(); iter2++)
        {
            QString plugin_name = iter2.key();
            m_barset *barset_obj = map_list_of_barsets.value(plugin_name);

            new_stacked_bar_series->append(barset_obj);

        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);
        struct_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_yaxis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = new_stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

    }



    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::create_graph_daywise()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    //    if(!month_year_list.isEmpty())
    //        month_year_list.sort();


    progressbar_increment_step = month_year_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    stackwidget_index = 0;
    map_of_pageno_and_timestamplabel.clear();
    for(int i = 0; i < month_year_list.size(); i++)
    {
        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }




        QString mnth_year = month_year_list.at(i);

        if(stackwidget_obj->count() > 1)
            pushbutton_next->setEnabled(true);

        map_of_pageno_and_timestamplabel.insert(i, mnth_year);

        int index = 0;

        QStackedBarSeries *new_stacked_bar_series = new QStackedBarSeries();
        connect(new_stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;

        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString, QStringList>::iterator iter;

        for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
        {
            QString plugin_name = iter.key();
            m_barset *barset_obj = new m_barset(plugin_name);
            map_list_of_barsets.insert(plugin_name, barset_obj);
        }


        for(int k = 0; k < day_month_year_list.size(); k++)
        {
            QString day_mnth_name = day_month_year_list.at(k);

            if(day_mnth_name.endsWith(mnth_year))
            {
                QString tmp_name = day_mnth_name;
                tmp_name.remove(tmp_name.indexOf("-"),tmp_name.size());
                tmp_name.remove(0,tmp_name.lastIndexOf(" ") + 1);

                categories_list << tmp_name;

                QMap<QString, QStringList>::iterator iter;
                int max_count = 0;
                for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
                {
                    QCoreApplication::processEvents();

                    QString plugin_name = iter.key();
                    QStringList tab_list = iter.value();

                    m_barset *barset_obj = map_list_of_barsets.value(plugin_name);

                    barset_obj->setBorderColor(Qt::gray);


                    qint64 count = 0;
                    QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(day_mnth_name);
                    for(int jj = 0;  jj < list_of_struct.size(); jj++)
                    {
                        struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);
                        if((struct_data_database_obj.plugin_name == plugin_name) && tab_list.contains(struct_data_database_obj.tab_name))
                        {
                            count++;
                        }
                    }


                    //                    struct_timeline_command_value struct_obj = prepare_command_and_values_list("COUNT(*)",table,plugin_name,tab_list, QString::number(start_timestamp_int),QString::number(end_timestamp_int), "day_month_year", day_mnth_name);
                    //                    qint64 count = recon_static_functions::get_intvalue_from_db_with_addbindvalue_by_dbpath(struct_obj.command,destination_timeline_db_path,0,struct_obj.values_list,Q_FUNC_INFO);

                    max_count+=count;

                    *barset_obj << count;

                    //                    QString tooltip = "Events - " +  QString::number(count) + "\n";
                    //                    tooltip.append("Category - "+ day_mnth_name + "\n");
                    //                    tooltip.append("Plugin - "+ barset_obj->label());

                    QString tooltip = prepare_and_get_tooltip_string_for_barset(count, day_mnth_name, barset_obj->label());
                    QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                    map_insert_index_with_category.insert(index,tooltip);

                    barset_obj->set_tooltip(map_insert_index_with_category);

                    map_list_of_barsets.insert(plugin_name, barset_obj);

                }

                if(max_range_for_yaxis <= max_count)
                    max_range_for_yaxis = max_count;

                index++;
            }


        }

        QMap<QString, QStringList>::iterator iter2;

        for(iter2 = map_bookmarked_plugin_with_tab_list.begin() ; iter2 != map_bookmarked_plugin_with_tab_list.end(); iter2++)
        {
            QString plugin_name = iter2.key();
            m_barset *barset_obj = map_list_of_barsets.value(plugin_name);

            new_stacked_bar_series->append(barset_obj);

        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_yaxis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = new_stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

    }


    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::create_graph_hourly()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    //    if(!day_month_year_list.isEmpty())
    //        day_month_year_list.sort();

    progressbar_increment_step = day_month_year_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    stackwidget_index = 0;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    map_of_pageno_and_timestamplabel.clear();
    for(int i = 0; i < day_month_year_list.size(); i++)
    {

        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
        {
            break;
        }


        QString day_mnth_year = day_month_year_list.at(i);

        map_of_pageno_and_timestamplabel.insert(i, day_mnth_year);

        if(stackwidget_obj->count() > 1)
            pushbutton_next->setEnabled(true);

        label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));
        // label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));


        int index = 0;

        QStackedBarSeries *new_stacked_bar_series = new QStackedBarSeries();
        connect(new_stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;

        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString, QStringList>::iterator iter;

        for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
        {
            QString plugin_name = iter.key();
            m_barset *barset_obj = new m_barset(plugin_name);
            map_list_of_barsets.insert(plugin_name, barset_obj);

        }

        for(int k = 0; k < hour_day_month_year_list.size(); k++)
        {
            QString hr_day_mnth_name = hour_day_month_year_list.at(k);

            if(hr_day_mnth_name.startsWith(day_mnth_year))
            {

                QString tmp_name = hr_day_mnth_name;
                tmp_name.remove(0,tmp_name.lastIndexOf(" ") + 1);

                categories_list << tmp_name;

                QMap<QString, QStringList>::iterator iter;

                int max_count = 0;
                for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
                {
                    QCoreApplication::processEvents();

                    QString plugin_name = iter.key();
                    QStringList tab_list = iter.value();


                    m_barset *barset_obj = map_list_of_barsets.value(plugin_name);
                    barset_obj->setBorderColor(Qt::gray);

                    qint64 count = 0;
                    QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(hr_day_mnth_name);
                    for(int jj = 0;  jj < list_of_struct.size(); jj++)
                    {
                        struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);
                        if((struct_data_database_obj.plugin_name == plugin_name) && tab_list.contains(struct_data_database_obj.tab_name))
                        {
                            count++;
                        }
                    }

                    //                    struct_timeline_command_value struct_obj = prepare_command_and_values_list("COUNT(*)",table,plugin_name,tab_list, QString::number(start_timestamp_int),QString::number(end_timestamp_int), "hour_day_month_year", hr_day_mnth_name);
                    //                    qint64 count = recon_static_functions::get_intvalue_from_db_with_addbindvalue_by_dbpath(struct_obj.command,destination_timeline_db_path,0,struct_obj.values_list,Q_FUNC_INFO);

                    max_count += count;
                    *barset_obj << count;

                    //                    QString tooltip = "Events - " +  QString::number(count) + "\n";
                    //                    tooltip.append("Category - "+ hr_day_mnth_name + "\n");
                    //                    tooltip.append("Plugin - "+ barset_obj->label());

                    QString tooltip = prepare_and_get_tooltip_string_for_barset(count, hr_day_mnth_name, barset_obj->label());

                    QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                    map_insert_index_with_category.insert(index,tooltip);

                    barset_obj->set_tooltip(map_insert_index_with_category);

                    map_list_of_barsets.insert(plugin_name, barset_obj);

                }

                if(max_range_for_yaxis <= max_count)
                    max_range_for_yaxis = max_count;

                index++;
            }


        }

        QMap<QString, QStringList>::iterator iter2;

        for(iter2 = map_bookmarked_plugin_with_tab_list.begin() ; iter2 != map_bookmarked_plugin_with_tab_list.end(); iter2++)
        {
            QString plugin_name = iter2.key();
            m_barset *barset_obj = map_list_of_barsets.value(plugin_name);

            new_stacked_bar_series->append(barset_obj);

        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_yaxis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = new_stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

        label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

    }


    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::set_chart_in_stackwidget_after_setting_of_all_specification(artifacts_timeline_full_loader::struct_chart_detail &struct_chart_detail_obj)
{
    ///Added functionality on barset
    QList<QBarSet*> list = struct_chart_detail_obj.stacked_bar_series->barSets();
    for(int i = 0; i < list.size() ; i++)
    {
        m_barset *barset = dynamic_cast<m_barset*> (list.at(i));
        barset->setBorderColor(QColor("transparent"));
        connect(barset, SIGNAL(doubleClicked(int)), this, SLOT(slot_barset_doubleClicked(int)));
        connect(barset, SIGNAL(pressed(int)), this, SLOT(slot_barset_pressed(int)));
    }
    ///-----------

    QChart *chart_obj = new QChart();
    chart_obj->setTitle(QString(MACRO_CASE_TREE_Artifacts_Timeline));
    chart_obj->setAnimationOptions(QChart::SeriesAnimations);
    chart_obj->legend()->setVisible(false);
    chart_obj->legend()->setAlignment(Qt::AlignBottom);

    ///Add series in chart
    chart_obj->addSeries(struct_chart_detail_obj.stacked_bar_series);

    ///Create category Axis- it can be create for x-axis or y-axis
    QBarCategoryAxis *barcategory_axis_obj = new QBarCategoryAxis();
    barcategory_axis_obj->append(struct_chart_detail_obj.category_list);
    struct_chart_detail_obj.stacked_bar_series->setBarWidth(0.40);

    ///Set axis x for chart
    chart_obj->setAxisX(barcategory_axis_obj, struct_chart_detail_obj.stacked_bar_series);
    // chart_obj->axisX()->setTitleText(struct_chart_detail_obj.x_axis_title);

    ///Set axis y for chart
    QValueAxis *value_axisy = new QValueAxis;
    value_axisy->setRange(0,struct_chart_detail_obj.roundup_value_yaxis);
    value_axisy->setLabelFormat("%i");
    // value_axisy->setTitleText(struct_chart_detail_obj.y_axis_title);
    chart_obj->setAxisY(value_axisy, struct_chart_detail_obj.stacked_bar_series);


    QChartView *chartView_common_obj = new QChartView();
    chartView_common_obj->setChart(chart_obj);

    list_of_charview << chartView_common_obj;

    if(radiobutton_create_graph_hourly->isChecked())
        stackwidget_obj->addWidget(chartView_common_obj);
    return;

}

QString artifacts_timeline_full_loader::prepare_and_get_tooltip_string_for_barset(qint64 count, QString category, QString label)
{
    QString tooltip = "Events - " +  QString::number(count) + "\n";
    tooltip.append("Category - "+ category + "\n");
    tooltip.append("Plugin - "+ label);

    return tooltip;

}


QStringList artifacts_timeline_full_loader::get_sorted_months_list(QStringList month_list)
{
    QStringList years_list;
    QStringList tmp_split_list;
    for(int count = 0; count < month_list.size(); count++)
    {
        QString tmp_month_str = month_list.at(count);

        tmp_split_list.clear();
        if(tmp_month_str.contains("-"))
            tmp_split_list = tmp_month_str.split("-");

        if(tmp_split_list.size() > 1)
            years_list << tmp_split_list.at(1);
    }

    years_list.removeDuplicates();
    years_list.sort();

    /// sort according to year
    QStringList sorted_years_list;
    for(int num = 0; num < years_list.size(); num++)
    {
        QStringList single_year_sorted_list;

        for(int count = 0; count < month_list.size(); count++)
        {
            if(month_list.at(count).endsWith(years_list.at(num)))
            {
                single_year_sorted_list << month_list.at(count);
            }
        }
        sorted_years_list << sort_month_list(single_year_sorted_list);
    }

    return sorted_years_list;
}

QStringList artifacts_timeline_full_loader::sort_month_list(QStringList month_list)
{
    /// create all months count list
    QMap <int , int> map_months_count_and_pos;
    for(int count = 0; count < month_list.size(); count++)
    {
        QString month_str = month_list.at(count);

        if(month_str.startsWith(QString("Jan-")))
            map_months_count_and_pos.insert(0, count);
        else if(month_str.startsWith(QString("Feb-")))
            map_months_count_and_pos.insert(1, count);
        else if(month_str.startsWith(QString("Mar-")))
            map_months_count_and_pos.insert(2, count);
        else if(month_str.startsWith(QString("Apr-")))
            map_months_count_and_pos.insert(3, count);
        else if(month_str.startsWith(QString("May-")))
            map_months_count_and_pos.insert(4, count);
        else if(month_str.startsWith(QString("Jun-")))
            map_months_count_and_pos.insert(5, count);
        else if(month_str.startsWith(QString("Jul-")))
            map_months_count_and_pos.insert(6, count);
        else if(month_str.startsWith(QString("Aug-")))
            map_months_count_and_pos.insert(7, count);
        else if(month_str.startsWith(QString("Sep-")))
            map_months_count_and_pos.insert(8, count);
        else if(month_str.startsWith(QString("Oct-")))
            map_months_count_and_pos.insert(9, count);
        else if(month_str.startsWith(QString("Nov-")))
            map_months_count_and_pos.insert(10, count);
        else if(month_str.startsWith(QString("Dec-")))
            map_months_count_and_pos.insert(11, count);
        else
        {
        }
    }

    ///sort months position list
    QStringList keys_list;
    QMapIterator<int, int> itr(map_months_count_and_pos);
    while (itr.hasNext())
    {
        itr.next();
        keys_list << QString::number(itr.key());
    }
    keys_list.sort();


    /// to avoid sorting as (1, 10 , 2...)
    if(keys_list.contains("10"))
    {
        int index = keys_list.indexOf("10");
        keys_list.removeAt(index);
        keys_list << QString::number(10);
    }
    if(keys_list.contains("11"))
    {
        int index = keys_list.indexOf("11");
        keys_list.removeAt(index);
        keys_list << QString::number(11);
    }
    if(keys_list.contains("12"))
    {
        int index = keys_list.indexOf("12");
        keys_list.removeAt(index);
        keys_list << QString::number(12);
    }


    QStringList sorted_months_pos_list;
    for(int pos = 0; pos < keys_list.size(); pos++)
    {
        QString pos_str = keys_list.at(pos);
        sorted_months_pos_list << QString::number(map_months_count_and_pos.value(pos_str.toInt()));
    }

    /// get months from sorted position
    QStringList sorted_months_list;
    for(int pos = 0; pos < sorted_months_pos_list.size(); pos++)
    {
        QString pos_str = sorted_months_pos_list.at(pos);
        sorted_months_list << month_list.at(pos_str.toInt());
    }

    sorted_months_list.removeDuplicates();

    return sorted_months_list;
}

