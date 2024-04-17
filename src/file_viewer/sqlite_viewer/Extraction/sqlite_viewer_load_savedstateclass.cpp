#include "sqlite_viewer_load_savedstateclass.h"

sqlite_viewer_load_saved_state_class::sqlite_viewer_load_saved_state_class(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Window);
    recon_helper_standard_obj = new recon_helper_standard(this);


    bool_global_export_checkbox_enable_sqlite_viewer = false;
    global_export_checkbox_disable_sqlite_viewer = 0;
}

sqlite_viewer_load_saved_state_class::~sqlite_viewer_load_saved_state_class()
{

}

QList<struct_global_sqlite_viewer_tablewidget_main> sqlite_viewer_load_saved_state_class::load_saved_state(QString path)
{
    load_saved_state_private(path);

    return list_struct_tablewidget_data_sqlite_viewer;
}


void sqlite_viewer_load_saved_state_class::load_saved_state_private(QString path)
{ ///-Funcrion -------1-------
    recon_static_functions::app_debug("START",Q_FUNC_INFO);


    ///-Everytime Whern ever this function get call it will insert new sqlite data
    list_struct_tablewidget_data_sqlite_viewer.clear();

    ///-1 Calling for the all db tablelist ///-Function -------1(a)-------
    destination_db_main_display_table_file = path + "/"+ main_datadisplay_table_sqlite_name;
    destination_db_filter_view_table_file = path + "/" + filter_view_table_sqlite_name;


    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_main_display_table_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED----- " + destination_db_main_display_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error---" + destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_main_display_table_file,Q_FUNC_INFO);
        return;
    }

    QStringList tableNameList = destination_db.tables();

    tab_custom_header_widgets_QMap.clear();

    ///-2 Working for each table
    for(int i = 0; i < tableNameList.size(); i++)
    {
        ///-A. Insert table name
        struct_global_sqlite_viewer_tablewidget_main obj_main_table;
        obj_main_table.table_name = tableNameList.at(i);

        current_table_name = tableNameList.at(i);

        ///-B. Get complete table info ///-Function -------1(b)-------
        struct_global_sqlite_viewer_single_dbtable_info obj_single_table;
        obj_single_table = load_dbtable_info(tableNameList.at(i),path);
        obj_main_table.obj_struct_dbtable_detail = obj_single_table;


        ///-C. Main Display Table Widget Info insertion for display ///-Function -------1(b)-------
        m_tablewidget *tableWidget_data;
        tableWidget_data = populate_toLoad_tablewidget_main_data_display(obj_single_table,tableNameList.at(i) , i+1 , tableNameList.size());
        obj_main_table.tableWidget_MainDataDisplay_ptr = tableWidget_data;


        ///-D. FilterViewSearch Table widget info insertion
        QTableWidget *tableWidget_FilterViewSearch;
        tableWidget_FilterViewSearch = populate_toLoad_tablewidget_filter_view_search(obj_single_table,tableNameList.at(i));
        obj_main_table.tableWidget_FilterViewSearch_ptr = tableWidget_FilterViewSearch;


        ///-E. Column Selection Table widget info insertion
        QTableWidget *tableWidget_ColumnSelection;
        tableWidget_ColumnSelection = populate_toLoad_tablewidget_column_selection(obj_single_table,tableNameList.at(i));
        obj_main_table.tableWidget_columns_selection_ptr = tableWidget_ColumnSelection;

        list_struct_tablewidget_data_sqlite_viewer << obj_main_table;
    }

    display_loading_progress_bar_obj->hide();
    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer_load_saved_state_class::pub_set_db_path_for_refresh(QString filter_db_path, QString main_db_path)
{
    destination_db_filter_view_table_file = filter_db_path;
    destination_db_main_display_table_file = main_db_path;
}


struct_global_sqlite_viewer_single_dbtable_info sqlite_viewer_load_saved_state_class::load_dbtable_info(QString table_name, QString path)
{ ///-Function -------1(b)-------
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    struct_global_sqlite_viewer_single_dbtable_info obj_struct;

    //    if(destination_db_filter_view_table_file.trimmed().isEmpty())
    //        destination_db_filter_view_table_file = path;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_filter_view_table_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + destination_db.lastError().text(),Q_FUNC_INFO);
        return obj_struct;
    }

    QSqlQuery query_master(destination_db);
    QString command = QString("SELECT coloum_name, coloum_view, coloum_search, coloum_data_type FROM '" + table_name + "'");
    query_master.prepare(command);
    if(!query_master.exec())
    {
        recon_static_functions::app_debug("query_master -------FAILED-------" + destination_db_filter_view_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_master.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + query_master.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return obj_struct;
    }

    int cols_count = 0;
    while(query_master.next())
    {
        cols_count++;
        obj_struct.cols_count = cols_count;
        obj_struct.cols_list << query_master.value(0).toString();
        obj_struct.view_list << query_master.value(1).toString();
        obj_struct.search_list << query_master.value(2).toString();
        obj_struct.list_CustomCombobox_CurrentText << query_master.value(3).toString();
    }


    ///- For Total Row Count or total record in a single table.
    QSqlQuery count_query(destination_db);
    QString command_count  = QString("select count(*) FROM '" + table_name + "'");
    if(!count_query.exec(command_count))
    {
        recon_static_functions::app_debug("count_query -----FAILED----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + count_query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + count_query.executedQuery(),Q_FUNC_INFO);
    }

    qint64 total_record = 0;
    while(count_query.next())
    {
        total_record = count_query.value(0).toInt();
        obj_struct.record_count = total_record;
    }

    for(int aa = 0 ; aa < total_record ; aa++)
    {
        obj_struct.v_header_list << "0";
    }

    ///- Foth the datatype of the dbTable for each coloum.
    QStringList DatatypeList = get_data_type_from_database_table_to_load(destination_db,table_name);
    obj_struct.list_db_coloum_datatype = DatatypeList;

    destination_db.close();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return obj_struct;
}

m_tablewidget *sqlite_viewer_load_saved_state_class::populate_toLoad_tablewidget_main_data_display(struct_global_sqlite_viewer_single_dbtable_info obj_struct, QString dbTableName, int progressing_table_count , int total_table_count)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    m_tablewidget *tableWidget  = new m_tablewidget(this);

    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableWidget->horizontalHeader()->setSectionsMovable(true);

    ///************** HEADER SETTING IN COLOUMS ******************///

    QStringList viewList = obj_struct.view_list;

    int cols_count = obj_struct.cols_list.size();  //- Total ColoumCount with the display
    tableWidget->setColumnCount(cols_count);
    for(int  z = 0 ; z < cols_count ; z++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(z,item);
        tableWidget->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);
    }

    QStringList  coloum_name_list;
    coloum_name_list = obj_struct.cols_list;

    QStringList empty_header_stringlist;
    custom_header_widgets_QList.clear();

    for(int column_count = 0; column_count < coloum_name_list.size(); column_count++)
    {
        QString column_name = coloum_name_list.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;

        custom_table_header_widget_obj->pub_set_column_name(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

        tableWidget->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        empty_header_stringlist << "";

        connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
        connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
        connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));


        if(column_count ==  enum_tablewidget_column_bookmark && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        else if(column_count ==  enum_tablewidget_column_tags && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }

        custom_header_widgets_QList.append(custom_table_header_widget_obj);
    }

    tab_custom_header_widgets_QMap.insert(current_table_name,custom_header_widgets_QList);
    tableWidget->setHorizontalHeaderLabels(empty_header_stringlist);


    ///**************** DATA INSERTION IN DATA TABLE************//
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_main_display_table_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ------FAILED--------",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_main_display_table_file,Q_FUNC_INFO);
        return tableWidget;
    }

    QSqlQuery select_query(destination_db);
    QString command = QString("SELECT * FROM '" + dbTableName + "'");
    if(!select_query.exec(command))
    {
        recon_static_functions::app_debug("select_query ------FAILED------- " + destination_db_main_display_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + select_query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + command,Q_FUNC_INFO);
        destination_db.close();
        return tableWidget;
    }

    QTableWidgetItem *item_tablewidget[cols_count];
    QString lable_t = dbTableName;
    float progressing_percentage = ((progressing_table_count*100)/total_table_count);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(progressing_percentage));

    int count = 0;
    while(select_query.next())
    {


        if(count % 300 == 0)
            QCoreApplication::processEvents();

        int qcount = 0; //For the query count


        display_loading_progress_bar_obj->pub_set_label_messsge(QString("Loading Table ") +  QString::number(progressing_table_count) + "/" + QString::number(total_table_count)  + QString(" '") + lable_t.replace("_" , " ") + QString("'  ")  + QString::number(count));
        display_loading_progress_bar_obj->show();
        if(bool_cancel_loading)
        {
            display_loading_progress_bar_obj->hide();
            break ;
        }
        // loop for each coloum item
        for(int j = 0 ; j < cols_count ; j++)
        {
            item_tablewidget[j] = new QTableWidgetItem;

            if(j == 0)
            {//-Insert Bookmark
                if(select_query.value(0).toString() == QString("0") || select_query.value(0).toString().trimmed().isEmpty())
                    item_tablewidget[j]->setCheckState(Qt::Unchecked);
                else
                    item_tablewidget[j]->setCheckState(Qt::Checked);
            }
            else if(j == 1)
            {//-Insert Tag

                if(select_query.value(cols_count -3).toString() == QString(""))
                {
                    item_tablewidget[j]->setIcon(QIcon(""));
                }
                else
                {
                    QString previous_tag = select_query.value(cols_count -3).toString();

                    QString icon_path;
                    if(previous_tag.contains(","))
                    {
                        icon_path = "../icons/tag_icons/multiple.png";
                    }
                    else if((previous_tag != "") && (!previous_tag.contains(",")))
                    {
                        QString icon_color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(previous_tag,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                        icon_color = icon_color.toLower();
                        icon_color = icon_color.replace(" ","_");

                        icon_path = "../icons/tag_icons/"+ icon_color +".png";
                    }

                    item_tablewidget[j]->setIcon(QIcon(icon_path));
                }
            }

            else
            {
                int notes_int = (cols_count -2);
                if(select_query.value(notes_int).toString() != "")
                {
                    item_tablewidget[0]->setIcon(QIcon("../icons/notes.png"));
                }
                //-Insert INTEGER entry towards right
                if(obj_struct.list_db_coloum_datatype.size() == cols_count)
                {
                    if(obj_struct.list_db_coloum_datatype.at(j) == "INTEGER")
                    {
                        item_tablewidget[j]->setData(Qt::EditRole,select_query.value(qcount).toLongLong());
                        item_tablewidget[j]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                    }
                }

                item_tablewidget[j]->setText(select_query.value(qcount).toString());
            }

            qcount++;
        }

        ///- Insertion of row in the table and display item data
        tableWidget->insertRow(count);
        tableWidget->setRowHeight(count,22);

        for(int k = 0; k < cols_count ; k++)
        {
            tableWidget->setItem(count, k, item_tablewidget[k]);
            tableWidget->setColumnWidth(k,100);

            ///- To hide coloum which are not checked
            if(cols_count == viewList.size())
            {



                if(k == 0 || k == 1)
                    continue;

                if(viewList.at(k) == "0")
                {
                    tableWidget->hideColumn(k);

                    if(cols_count -1)
                    {

                    }
                }
            }
        }

        count++;
    }

    ///- Property's to set on the table Widget
    QFont font;
    font.setPixelSize(MACRO_QMenu_Font_Size_int);
    tableWidget->setFont(font);

    tableWidget->setColumnWidth(0,50);
    tableWidget->setColumnWidth(1,40);
    tableWidget->hideColumn(tableWidget->columnCount() -2);
    tableWidget->hideColumn(tableWidget->columnCount() -3);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSortingEnabled(false);
    tableWidget->verticalHeader()->setFixedWidth(50);
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableWidget->horizontalHeader()->setSectionsMovable(true);

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);

    destination_db.close();
    return tableWidget;
}

QTableWidget *sqlite_viewer_load_saved_state_class::populate_toLoad_tablewidget_filter_view_search(struct_global_sqlite_viewer_single_dbtable_info struct_obj, QString table_name)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    QTableWidget *tableWidget  = new QTableWidget(this);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableWidget->horizontalHeader()->setSectionsMovable(true);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_filter_view_table_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED---- " + destination_db_filter_view_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Errror " + destination_db.lastError().text(),Q_FUNC_INFO);
        return tableWidget;
    }


    ///************** HEADER SETTING IN COLOUMS ******************///

    int cols_count = 3;  //- Total ColoumCount with the display
    tableWidget->setColumnCount(cols_count);
    for(int  z = 0 ; z < cols_count ; z++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();

        if((z == 1) || (z == 0))
        {
            item->setIcon(QIcon("../icons/checked.png"));
            item->setCheckState(Qt::Checked);
            tableWidget->setHorizontalHeaderItem(z,item);
            tableWidget->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);

        }
        else
        {
            tableWidget->setHorizontalHeaderItem(z,item);
            tableWidget->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);
        }
    }

    QStringList  coloum_name_list;
    coloum_name_list  << "Column Name" << "Search" << "Data Conversion";
    tableWidget->setHorizontalHeaderLabels(coloum_name_list);

    QTableWidgetItem *item_col_name;
    QTableWidgetItem *item_search;

    QSqlQuery query_master(destination_db);
    QString command = QString("SELECT coloum_name, coloum_view, coloum_search, data_conversion_type FROM '" + table_name + "'");
    if(!query_master.exec(command))
    {
        recon_static_functions::app_debug("query_master -------FAILED------- " + destination_db_filter_view_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_master.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + query_master.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return tableWidget;
    }


    int count = 0;
    while(query_master.next())
    {

        item_col_name = new QTableWidgetItem();
        item_search = new QTableWidgetItem();

        ///-1. Coloum Name with View Check box
        item_col_name->setText(query_master.value(0).toString());
        if(query_master.value(1).toString() == QString("0"))
            item_col_name->setCheckState(Qt::Unchecked);
        else
            item_col_name->setCheckState(Qt::Checked);


        ///-2. Search Check box
        if(query_master.value(2).toString() == QString("0"))      //3
            item_search->setCheckState(Qt::Unchecked);
        else
            item_search->setCheckState(Qt::Checked);

        if(query_master.value(3).toString() == "File")
        {
            bool_global_export_checkbox_enable_sqlite_viewer = true;
            global_export_checkbox_disable_sqlite_viewer++;

            recon_static_functions::app_debug("export_checkbox_count and bool_export_checkbox value[load state from main engine] ",Q_FUNC_INFO);

            emit signal_onload_update_export_checkbox_and_count(bool_global_export_checkbox_enable_sqlite_viewer,global_export_checkbox_disable_sqlite_viewer);
        }

        QStringList combo_list;

        //        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp"  << "File" << "Size Human Readable";
        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "File" << "Size Human Readable" << "ASCII" << "Unicode";

        QComboBox* comboBox = new QComboBox();
        comboBox->addItems(combo_list);
        comboBox->setCurrentText(query_master.value(3).toString());
        comboBox->setMaximumWidth(150);

        tableWidget->insertRow(count);
        tableWidget->setRowHeight(count,22);
        tableWidget->setItem(count,0,item_col_name);
        tableWidget->setItem(count,1,item_search);
        tableWidget->setCellWidget(count,2,comboBox);

        count++;
    }

    QFont font;
    font.setPixelSize(12);
    tableWidget->setFont(font);


    ///- Property's to set on the table Widget
    tableWidget->hideRow(0);
    tableWidget->hideRow(1);
    tableWidget->hideRow(2);
    tableWidget->hideRow((tableWidget->rowCount() - 2));
    tableWidget->hideRow((tableWidget->rowCount() - 1));
    tableWidget->hideRow((tableWidget->rowCount() - 3));

    int mycount = 1;
    for(int iter =3; iter < (tableWidget->rowCount() - 3); iter++)
    {
        tableWidget->setVerticalHeaderItem(iter,new QTableWidgetItem(QString::number(mycount++)));
    }
    tableWidget->setColumnWidth(0,180);
    tableWidget->setColumnWidth(1,65);
    tableWidget->setColumnWidth(2,200);

    tableWidget->horizontalHeader()->setStretchLastSection(true);

    tableWidget->setColumnHidden(2,true);

    tableWidget->setColumnHidden(1,true);

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);

    destination_db.close();
    return tableWidget;
}

QTableWidget* sqlite_viewer_load_saved_state_class::populate_toLoad_tablewidget_column_selection(struct_global_sqlite_viewer_single_dbtable_info struct_obj, QString table_name)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QTableWidget *tableWidget  = new QTableWidget(this);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);

    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableWidget->horizontalHeader()->setSectionsMovable(true);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_filter_view_table_file);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ----FAILED---- " + destination_db_filter_view_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return tableWidget;
    }


    ///************** HEADER SETTING IN COLOUMS ******************///
    int cols_count = 3;  //- Total ColoumCount with the display
    tableWidget->setColumnCount(cols_count);

    for(int  z = 0 ; z < cols_count ; z++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();

        if((z == 1) || (z == 0))
        {
            item->setIcon(QIcon("../icons/checked.png"));
            item->setCheckState(Qt::Checked);
            tableWidget->setHorizontalHeaderItem(z,item);
            tableWidget->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);

        }
        else
        {
            tableWidget->setHorizontalHeaderItem(z,item);
            tableWidget->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);
        }
    }

    QStringList  coloum_name_list;

    coloum_name_list  << "Column Name" << "Search" << "Data Conversion";
    tableWidget->setHorizontalHeaderLabels(coloum_name_list);

    QTableWidgetItem *item_col_name;
    QTableWidgetItem *item_search;

    QSqlQuery query_master(destination_db);
    QString command = QString("SELECT coloum_name, coloum_view, coloum_search, data_conversion_type FROM '" + table_name + "'");
    if(!query_master.exec(command))
    {
        recon_static_functions::app_debug("query_master -------FAILED------- " + destination_db_filter_view_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_master.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + query_master.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return tableWidget;
    }

    int count = 0;
    while(query_master.next())
    {
        item_col_name = new QTableWidgetItem();
        item_search = new QTableWidgetItem();

        ///-1. Coloum Name with View Check box
        item_col_name->setText(query_master.value(0).toString());
        if(query_master.value(1).toString() == QString("0"))
            item_col_name->setCheckState(Qt::Unchecked);
        else
            item_col_name->setCheckState(Qt::Checked);


        ///-2. Search Check box
        if(query_master.value(2).toString() == QString("0"))      //3
            item_search->setCheckState(Qt::Unchecked);
        else
            item_search->setCheckState(Qt::Checked);

        if(query_master.value(3).toString() == "File")
        {
            //bool_global_export_checkbox_enable_sqlite_viewer = true;
        }

        QStringList combo_list;

        //        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp"  << "File" << "Size Human Readable";
        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "File" << "Size Human Readable" << "ASCII" << "Unicode";

        QComboBox* comboBox = new QComboBox();

        QFont font;
        font.setPixelSize(12);
        comboBox->setFont(font);

        comboBox->setParent(tableWidget);

        comboBox->addItems(combo_list);
        comboBox->setCurrentText(query_master.value(3).toString());
        comboBox->setMaximumWidth(150);

        tableWidget->insertRow(count);
        tableWidget->setRowHeight(count,22);
        tableWidget->setItem(count,0,item_col_name);
        tableWidget->setItem(count,1,item_search);
        tableWidget->setCellWidget(count,2,comboBox);

        connect(comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_onload_dynamic_combobox(QString)));

        count++;
    }

    QFont font;
    font.setPixelSize(13);
    tableWidget->setFont(font);

    ///- Property's to set on the table Widget
    tableWidget->hideRow(0);
    tableWidget->hideRow(1);
    tableWidget->hideRow(2);
    tableWidget->hideRow((tableWidget->rowCount() - 2));
    tableWidget->hideRow((tableWidget->rowCount() - 1));
    tableWidget->hideRow((tableWidget->rowCount() - 3));

    int mycount = 1;
    for(int iter =3; iter < (tableWidget->rowCount() - 3); iter++)
    {
        tableWidget->setVerticalHeaderItem(iter,new QTableWidgetItem(QString::number(mycount++)));
    }

    tableWidget->setColumnWidth(0,250);
    tableWidget->setColumnWidth(1,65);
    tableWidget->setColumnWidth(2,200);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    tableWidget->setColumnHidden(1,true);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    destination_db.close();

    return tableWidget;

}


void sqlite_viewer_load_saved_state_class::pub_set_display_loading_bar(display_loading_progress_bar *obj)
{
    bool_cancel_loading = false;
    display_loading_progress_bar_obj = obj;
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
}

QStringList sqlite_viewer_load_saved_state_class::get_data_type_from_database_table_to_load(QSqlDatabase db, QString table_name)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QStringList dataTypeList;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_main_display_table_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return dataTypeList;
    }

    QString dbCreateStr;
    QSqlRecord dbRecord;

    QSqlQuery queryRead(destination_db);
    queryRead.prepare("SELECT * FROM '" + table_name + "'");
    if(!queryRead.exec())
    {
        recon_static_functions::app_debug("queryRead ---FAILED--- " + destination_db_main_display_table_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + queryRead.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + queryRead.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return dataTypeList;
    }

    dbRecord = queryRead.record();
    QString typeName;

    for(int i = 0; i < dbRecord.count() ; i++)
    {
        QSqlField dbField = dbRecord.field(i);
        QString columnName = dbField.name();

        ///-For the datatype of the table Coloums.
        QVariant dbVariant = dbField.type();


        typeName = dbVariant.typeName();

        if(typeName == "int")
        {
            typeName.clear();
            typeName = "INTEGER";
        }
        else if(typeName == "QString")
        {
            typeName.clear();
            typeName = "VARCHAR(500)";
        }
        else if(typeName == "QByteArray")
        {
            typeName.clear();
            typeName = "VARCHAR(500)";
        }
        else if(typeName == "double")
        {
            typeName.clear();
            typeName = "VARCHAR(500)";
        }
        else
        {
            typeName.clear();
            typeName = "VARCHAR(500)";
        }

        dataTypeList << typeName;
    }

    dbCreateStr.clear();
    typeName.clear();

    destination_db.close();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    return dataTypeList;
}


void sqlite_viewer_load_saved_state_class::saved_state_dir_name_pub(QString SavedStateSqlite)
{
    saved_state_dir_name = SavedStateSqlite;
}

void sqlite_viewer_load_saved_state_class::main_datadisplay_table_sqlite_name_pub(QString MainDataDisplay_SavedState)
{
    main_datadisplay_table_sqlite_name = MainDataDisplay_SavedState;
}

void sqlite_viewer_load_saved_state_class::filter_view_table_sqlite_name_pub(QString FilterViewTable_SavedState)
{
    filter_view_table_sqlite_name = FilterViewTable_SavedState;
}


void sqlite_viewer_load_saved_state_class::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();
}

void sqlite_viewer_load_saved_state_class::slot_onload_dynamic_combobox(QString text)
{

    emit signal_onload_combobox_conversion_change();

    if(text == "File")
    {
        bool_global_export_checkbox_enable_sqlite_viewer = true;
        global_export_checkbox_disable_sqlite_viewer++;

        emit signal_onload_update_export_checkbox_and_count(bool_global_export_checkbox_enable_sqlite_viewer,global_export_checkbox_disable_sqlite_viewer);

    }
    else if(text != "File")
    {
        int local_var = 0;

        for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
        {
            struct_global_sqlite_viewer_tablewidget_main obj_main = list_struct_tablewidget_data_sqlite_viewer.at(i);

            QTableWidget *tableWidgetColumnsSelection = obj_main.tableWidget_columns_selection_ptr;

            for(int i = 0; i < tableWidgetColumnsSelection->rowCount(); i++)
            {
                QString recivedComboboxText;
                recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidgetColumnsSelection->cellWidget((i) ,2))->currentText());

                if(recivedComboboxText == "File")
                {
                    local_var++;
                }
            }
        }

        global_export_checkbox_disable_sqlite_viewer = local_var;

        if(global_export_checkbox_disable_sqlite_viewer == 0)
        {
            bool_global_export_checkbox_enable_sqlite_viewer = false;
        }

        emit signal_onload_update_export_checkbox_and_count(bool_global_export_checkbox_enable_sqlite_viewer,global_export_checkbox_disable_sqlite_viewer);
    }
}

void sqlite_viewer_load_saved_state_class::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);
    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    if(tab_custom_header_widgets_QMap.keys().contains(current_tab_name))
    {
        custom_header_widgets_QList.clear();
        custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(current_tab_name);
    }

    QList<struct_search_column_text_for_sqlite_viewer> search_text_col_QList;

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_sqlite_viewer struct_search_column_text_for_sqlite_viewer_obj;

            struct_search_column_text_for_sqlite_viewer_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_sqlite_viewer_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_sqlite_viewer_obj);
            bool_stop_sorting_tags_bookmark_notes_after_search = true;
        }
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    if(search_text_col_QList.size() < 1)
    {
        for(int row = 0; row < current_tab_tableWidget_obj->rowCount(); row++)
        {
            if(row % 20 == 0)
                QCoreApplication::processEvents();

            current_tab_tableWidget_obj->showRow(row);
        }
        bool_stop_sorting_tags_bookmark_notes_after_search = false;

        display_loading_progress_bar_obj->hide();
        return;
    }

    all_column_text_search(search_text_col_QList);

    current_tab_tableWidget_obj->horizontalHeader()->hide();
    current_tab_tableWidget_obj->horizontalHeader()->show();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void sqlite_viewer_load_saved_state_class::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void sqlite_viewer_load_saved_state_class::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    if(column != enum_tablewidget_column_bookmark
            && column != enum_tablewidget_column_tags)
    {
        if(column_sort_order == 0)
            current_tab_tableWidget_obj->sortByColumn(column, Qt::DescendingOrder);
        else
            current_tab_tableWidget_obj->sortByColumn(column, Qt::AscendingOrder);

        current_tab_tableWidget_obj->horizontalHeader()->hide();
        current_tab_tableWidget_obj->horizontalHeader()->show();
        return;
    }

    if(bool_stop_sorting_tags_bookmark_notes_after_search)
        return;

    current_tab_tableWidget_obj->clearSelection();

    if(current_tab_tableWidget_obj->rowCount() <= 1)
        return;

    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = current_tab_tableWidget_obj->rowCount() - 1;

    if(column_sort_order == 1)
    {
        for(qint64 row = 0; row < current_tab_tableWidget_obj->rowCount(); row++)
        {
            if(current_tab_tableWidget_obj->isRowHidden(row))
            {
                continue;
            }

            if(current_tab_tableWidget_obj->item(row, column) == NULL)
                continue;

            QIcon icon_set;
            if(column != enum_tablewidget_column_bookmark)
                icon_set =  current_tab_tableWidget_obj->item(row, column)->icon();



            current_tab_tableWidget_obj->item(row, column)->checkState();

            if(current_tab_tableWidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column != enum_tablewidget_column_bookmark)
                    icon_shiftrow_acs =  current_tab_tableWidget_obj->item(row_shift_pos_ascending, column)->icon();

                if(current_tab_tableWidget_obj->item(row_shift_pos_ascending, column)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                QList<QTableWidgetItem*> sourceItems = current_tab_tableWidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_tab_tableWidget_obj->takeRow(destRow);

                current_tab_tableWidget_obj->setRow(destRow, sourceItems);
                current_tab_tableWidget_obj->setRow(sourceRow, destItems);


                row_shift_pos_ascending++;
            }
        }
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(current_tab_tableWidget_obj->isRowHidden(row))
            {
                continue;
            }

            if(current_tab_tableWidget_obj->item(row, column) == NULL)
                continue;

            if(current_tab_tableWidget_obj->item(row_shift_pos_descending, column) == NULL)
                continue;

            QIcon icon_set;
            if(column != enum_tablewidget_column_bookmark)
                icon_set =  current_tab_tableWidget_obj->item(row, column)->icon();

            if(current_tab_tableWidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column != enum_tablewidget_column_bookmark)
                    icon_shiftrow_desc =  current_tab_tableWidget_obj->item(row_shift_pos_descending, column)->icon();

                if(current_tab_tableWidget_obj->item(row_shift_pos_descending, column)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                QList<QTableWidgetItem*> sourceItems = current_tab_tableWidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = current_tab_tableWidget_obj->takeRow(destRow);

                current_tab_tableWidget_obj->setRow(destRow, sourceItems);
                current_tab_tableWidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }
    }

    current_tab_tableWidget_obj->horizontalHeader()->hide();
    current_tab_tableWidget_obj->horizontalHeader()->show();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void sqlite_viewer_load_saved_state_class::all_column_text_search(QList<struct_search_column_text_for_sqlite_viewer> search_text_all_column_list)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    for(int row = 0; row < current_tab_tableWidget_obj->rowCount(); row++)
    {
        if(current_tab_tableWidget_obj->isRowHidden(row))
        {
            current_tab_tableWidget_obj->showRow(row);
        }

        if(bool_cancel_loading)
            break;

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool hide_row = false;

        for(int column_count = 0; column_count < search_text_all_column_list.size(); column_count++)
        {
            struct_search_column_text_for_sqlite_viewer struct_search_column_text_for_sqlite_viewer_obj = search_text_all_column_list.at(column_count);

            if(!(current_tab_tableWidget_obj->item(row, struct_search_column_text_for_sqlite_viewer_obj.column_number)->text().trimmed().contains(struct_search_column_text_for_sqlite_viewer_obj.search_text, Qt::CaseInsensitive)))
            {
                hide_row = true;
                break;
            }
        }

        if(hide_row)
        {
            current_tab_tableWidget_obj->hideRow(row);
        }
        else
        {
            current_tab_tableWidget_obj->showRow(row);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void sqlite_viewer_load_saved_state_class::pub_set_current_tab_detail(QString tab_name, m_tablewidget *current_tab_widget)
{
    current_tab_name = tab_name;
    current_tab_tableWidget_obj = current_tab_widget;
}
