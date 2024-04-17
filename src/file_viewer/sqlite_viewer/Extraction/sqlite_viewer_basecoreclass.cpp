#include "sqlite_viewer_basecoreclass.h"

sqlite_viewer_base_core_class::sqlite_viewer_base_core_class(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Window);
    bool_global_export_checkbox_enable_sqlite_viewer = false;
    global_export_checkbox_disable_sqlite_viewer = 0;

    //display_loading_progress_bar_obj  = new display_loading_progress_bar(this);
    //connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

}

sqlite_viewer_base_core_class::~sqlite_viewer_base_core_class()
{

}

void sqlite_viewer_base_core_class::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();
}

QList<struct_global_sqlite_viewer_tablewidget_main> sqlite_viewer_base_core_class::prepare_and_fill_structure_for_main_table_display(QString sourceFile, QString copiedSourceFile, QString recreateSourceFile)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    source_file_path = sourceFile;
    copied_source_file = copiedSourceFile;
    recreated_source_file = recreateSourceFile;

    ///-Everytime When ever this function get call it will insert new sqlite data
    list_struct_tablewidget_data_sqlite_viewer.clear();
    tab_custom_header_widgets_QMap.clear();

    ///-1 Calling for get  all the db tablelist
    QStringList tableNameList = get_db_tablename_list();

    ///-2 Working for each table
    for(int i = 0; i < tableNameList.size(); i++)
    {
        if((i % 500) == 0)
            QCoreApplication::processEvents();

        ///-A. Insert table name
        struct_global_sqlite_viewer_tablewidget_main obj_main_table;
        obj_main_table.table_name = tableNameList.at(i);

        current_table_name = tableNameList.at(i);

        ///-B. Get complete table info ///-Function -------1(b)-------
        struct_global_sqlite_viewer_single_dbtable_info obj_single_table;
        obj_single_table = get_db_table_info(tableNameList.at(i));
        obj_main_table.obj_struct_dbtable_detail = obj_single_table;

        ///-C. Main Display Table Widget Info insertion for display ///-Function -------1(b)-------
        m_tablewidget *tableWidget_data;
        tableWidget_data = populate_tablewidget_main_data_display(obj_single_table,tableNameList.at(i) ,i+1);
        obj_main_table.tableWidget_MainDataDisplay_ptr = tableWidget_data;

        ///-D. FilterViewSearch Table widget info insertion
        QTableWidget *tableWidget_FilterViewSearch;
        tableWidget_FilterViewSearch = populate_tablewidget_filter_view_search(obj_single_table,tableNameList.at(i));
        obj_main_table.tableWidget_FilterViewSearch_ptr = tableWidget_FilterViewSearch;

        ///-E. Columns Selection Table widget info insertion
        QTableWidget *col_sel_ptr;
        col_sel_ptr = populate_tablewidget_columns_selection(obj_single_table,tableNameList.at(i));
        obj_main_table.tableWidget_columns_selection_ptr = col_sel_ptr;

        list_struct_tablewidget_data_sqlite_viewer << obj_main_table;
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    display_loading_progress_bar_obj->hide();

    return list_struct_tablewidget_data_sqlite_viewer;
}

QStringList sqlite_viewer_base_core_class::pub_get_db_tablename_list()
{
    return get_db_tablename_list();
}

QTableWidget *sqlite_viewer_base_core_class::populate_tablewidget_columns_selection(struct_global_sqlite_viewer_single_dbtable_info struct_obj,QString tab_name)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QTableWidget *tableWidget  = new QTableWidget(this);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);

    int cols_count = 3;  //- Total ColoumCount with the display
    tableWidget->setColumnCount(cols_count);
    for(int  z = 0 ; z < cols_count ; z++)
    {
        QTableWidgetItem* item = new QTableWidgetItem;


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

    QStringList cols_list = struct_obj.cols_list;
    QStringList view_list = struct_obj.view_list;
    QStringList search_list = struct_obj.search_list;

    QTableWidgetItem *item_col_name;
    QTableWidgetItem *item_search;
    QTableWidgetItem *item_combobox;

    int count = 0;
    for(int sv = 0 ; sv < (cols_list.size()) ; sv++)
    {
        //-[NOTE]: Below lines to hide RECON_notes from table
        //        if(sv == (cols_list.count() - 1))
        //            continue;



        item_col_name = new QTableWidgetItem;
        item_search = new QTableWidgetItem;
        item_combobox = new QTableWidgetItem;

        item_combobox->setText("");

        ///-1. Coloum Name with View Check box
        item_col_name->setText(cols_list.at(sv));
        if(view_list.at(sv) == QString("0"))
        {
            item_col_name->setCheckState(Qt::Unchecked);
        }
        else
        {
            item_col_name->setCheckState(Qt::Checked);
        }

        ///-2. Search Check box
        if(search_list.at(sv) == QString("0"))
        {
            //3
            item_search->setCheckState(Qt::Unchecked);
        }
        else
        {
            item_search->setCheckState(Qt::Checked);
        }

        QStringList combo_list;
        //        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "File" << "Size Human Readable";
        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "File" << "Size Human Readable" << "ASCII" << "Unicode";

        QComboBox* comboBox = new QComboBox;

        QFont font;
        font.setPixelSize(12);
        comboBox->setFont(font);

        comboBox->setParent(tableWidget);

        comboBox->addItems(combo_list);
        comboBox->setMaximumWidth(150);

        tableWidget->insertRow(count);
        tableWidget->setRowHeight(count,22);
        tableWidget->setItem(count,0,item_col_name);
        tableWidget->setItem(count,1,item_search);
        tableWidget->setCellWidget(count,2,comboBox);

        connect(comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_dynamic_combobox(QString)));

        count++;
    }

    ///- Property's to set on the table Widget

    QFont font;
    font.setPixelSize(13);
    tableWidget->setFont(font);

    tableWidget->hideRow(0);
    tableWidget->hideRow(1);
    tableWidget->hideRow(2);
    tableWidget->hideRow((tableWidget->rowCount() - 2));
    tableWidget->hideRow((tableWidget->rowCount() - 3));
    tableWidget->hideRow((tableWidget->rowCount() - 1));

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

    return tableWidget;
}

bool sqlite_viewer_base_core_class::pub_cancel_loading()
{
    return bool_cancel_loading;
}

void sqlite_viewer_base_core_class::pub_set_display_loading_bar(display_loading_progress_bar *obj)
{
    display_loading_progress_bar_obj = obj;
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
}

QStringList sqlite_viewer_base_core_class::get_db_tablename_list()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);
    QStringList tablenameList;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recreated_source_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(recreated_source_file,Q_FUNC_INFO);
        return tablenameList;
    }

    QSqlQuery query_tableName(destination_db);

    QString type_str = "table";
    QString command_master = QString("SELECT name, sql FROM sqlite_master WHERE type = ?");
    query_tableName.prepare(command_master);
    query_tableName.addBindValue(type_str);
    if(!query_tableName.exec())
    {
        recon_static_functions::app_debug("command_master -----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_tableName.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + command_master,Q_FUNC_INFO);
        destination_db.close();
        return tablenameList;
    }

    while(query_tableName.next())
    {
        tablenameList << query_tableName.value(0).toString();
    }

    tablenameList.sort();
    recon_static_functions::app_debug("END ",Q_FUNC_INFO);
    destination_db.close();
    return tablenameList;
}

void sqlite_viewer_base_core_class::slot_dynamic_combobox(QString text)
{
    emit this->signal_combobox_conversion_change();

    if(text == "File")
    {
        bool_global_export_checkbox_enable_sqlite_viewer = true;
        global_export_checkbox_disable_sqlite_viewer++;

        emit this->signal_update_export_checkbox_and_count(bool_global_export_checkbox_enable_sqlite_viewer,global_export_checkbox_disable_sqlite_viewer);
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

        emit this->signal_update_export_checkbox_and_count(bool_global_export_checkbox_enable_sqlite_viewer,global_export_checkbox_disable_sqlite_viewer);
    }
}

struct_global_sqlite_viewer_single_dbtable_info sqlite_viewer_base_core_class::get_db_table_info(QString table_name)
{
    recon_static_functions::app_debug("Start ",Q_FUNC_INFO);

    struct_global_sqlite_viewer_single_dbtable_info obj_struct;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recreated_source_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED----- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(recreated_source_file,Q_FUNC_INFO);
        return obj_struct;
    }

    QSqlQuery pragma_query(destination_db);
    QSqlQuery count_query(destination_db);

    QString command_pragma = "PRAGMA TABLE_INFO( '" + table_name + "')";
    if(!pragma_query.exec(command_pragma))
    {
        recon_static_functions::app_debug("command_pragma -----FAILED----- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(pragma_query.lastError().text(),Q_FUNC_INFO);
    }

    int cols_count = 0;

    while(pragma_query.next())
    {
        cols_count++;
        QString col_name = pragma_query.value(1).toString();
        obj_struct.cols_list << col_name;
        obj_struct.cols_count = cols_count;

        obj_struct.h_header_list << "1";

        obj_struct.view_list << "1";
        obj_struct.search_list << "1";
        obj_struct.list_convert_coloum_data_type <<"0";

    }

    //-Additional Coloums Add for Sqlite Handling purpose.
    cols_count = cols_count + 6;
    obj_struct.cols_count = cols_count;

    obj_struct.cols_list.insert(0,"");  //-For Bookmark
    obj_struct.cols_list.insert(1,""); //-For Tags
    obj_struct.cols_list.insert(2,"RECON INT"); //-For Record No.
    obj_struct.cols_list.insert((cols_count-3),"RECON Examiner Tags"); //-Tags Name
    obj_struct.cols_list.insert((cols_count-2),"RECON Examiner Notes"); //- Notes Written to save in Coloumn
    obj_struct.cols_list.insert((cols_count-1),""); //-Blank Coloumn

    obj_struct.h_header_list << "1" <<"1" <<"1" << "1"<<"1" <<"1";
    obj_struct.view_list << "1" << "1" << "1" << "1" << "1" << "1";
    obj_struct.search_list << "1" << "1" << "1" << "1" << "1" << "1";
    obj_struct.list_convert_coloum_data_type <<"0" <<"0" <<"0" << "0" <<"0" <<"0";


    ///- For Total Row Count or total record in a single table.
    QString command_count  = QString("SELECT count(*) FROM '" + table_name + "'");
    if(!count_query.exec(command_count))
    {
        recon_static_functions::app_debug("count_query -----FAILED----- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(count_query.lastError().text(),Q_FUNC_INFO);
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

    ///- For the datatype of the dbTable for each coloum.
    QStringList DatatypeList = get_data_type_from_database_table(destination_db,table_name);
    DatatypeList.insert(0,"INTEGER");
    DatatypeList.insert(1,"VARCHAR(200)");
    DatatypeList.insert(2,"INTEGER");

    DatatypeList << "VARCHAR(500)" << "VARCHAR(500)" << "VARCHAR(500)";

    obj_struct.list_db_coloum_datatype = DatatypeList;

    destination_db.close();
    recon_static_functions::app_debug("END ",Q_FUNC_INFO);
    return obj_struct;
}

m_tablewidget* sqlite_viewer_base_core_class::populate_tablewidget_main_data_display(struct_global_sqlite_viewer_single_dbtable_info obj_struct, QString dbTableName,int progressing_table_count)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    m_tablewidget *m_tableWidget_obj  = new m_tablewidget(this);

    m_tableWidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tableWidget_obj->horizontalHeader()->setSectionsMovable(true);

    ///************** HEADER SETTING IN COLOUMS ******************///

    int cols_count = obj_struct.cols_list.size();  //- Total ColoumCount with the display
    m_tableWidget_obj->setColumnCount(cols_count);
    for(int  z = 0 ; z < cols_count ; z++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();
        m_tableWidget_obj->setHorizontalHeaderItem(z,item);
        m_tableWidget_obj->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);
    }

    QStringList  coloum_name_list;
    coloum_name_list = obj_struct.cols_list;

    custom_header_widgets_QList.clear();
    QStringList empty_header_stringlist;

    for(int column_count = 0; column_count < coloum_name_list.size(); column_count++)
    {
        QString column_name = coloum_name_list.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;

        custom_table_header_widget_obj->pub_set_column_name(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

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

        m_tableWidget_obj->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        empty_header_stringlist << "";

        custom_header_widgets_QList.append(custom_table_header_widget_obj);
    }
    tab_custom_header_widgets_QMap.insert(current_table_name,custom_header_widgets_QList);

    m_tableWidget_obj->setHorizontalHeaderLabels(empty_header_stringlist);

    ///**************** DATA INSERTION IN DATA TABLE************//

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recreated_source_file);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);

        return m_tableWidget_obj;
    }

    QSqlQuery select_query(destination_db);
    QString command = QString("SELECT * FROM '" + dbTableName + "'");
    if(!select_query.exec(command))
    {
        recon_static_functions::app_debug("select query ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(command,Q_FUNC_INFO);
        destination_db.close();
        return m_tableWidget_obj;
    }

    QTableWidgetItem *item_tablewidget[cols_count];
    int total_table_count =  get_db_tablename_list().size();
    QString lable_t = dbTableName;
    float progressing_percentage = ((progressing_table_count*100)/total_table_count);
    if(progressing_percentage >= 50)
        progressing_percentage = (progressing_percentage - 50);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(progressing_percentage));

    uint64_t row_count = 0;

    while(select_query.next())
    {
        int qcount = 0; //For the query count

        if(row_count % 5000 == 0)
            QCoreApplication::processEvents();


        if(row_count % 20000 == 0)
        {
            QCoreApplication::processEvents();
            if(progressing_percentage < 95)
            {
                progressing_percentage = progressing_percentage + 1;
                display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(progressing_percentage));
            }
        }

        display_loading_progress_bar_obj->pub_set_label_messsge(QString("Loading Table ") +  QString::number(progressing_table_count) + "/" + QString::number(total_table_count)  + QString(" '") +lable_t.replace("_" , " ") + QString("'  ")  + QString::number(row_count));

        display_loading_progress_bar_obj->show();

        if(bool_cancel_loading)
        {
            break ;
        }

        // loop for each coloum item
        for(int j = 0 ; j < cols_count ; j++)
        {

            item_tablewidget[j] = new QTableWidgetItem;

            if(j == 0)
            {//-Insert Bookmark
                if(obj_struct.v_header_list.size() > row_count)
                {
                    if(obj_struct.v_header_list.at(row_count) == QString("0"))
                        item_tablewidget[j]->setCheckState(Qt::Unchecked);
                    else
                        item_tablewidget[j]->setCheckState(Qt::Checked);
                }
            }
            else if (j == 1)
            {//-Insert tags coloumn
                item_tablewidget[j]->setText(QString(""));
            }
            else if (j == 2)
            {//-Insert INT Primary key to give numbering
                item_tablewidget[j]->setData(Qt::EditRole,row_count + 1);
                item_tablewidget[j]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            else if(j == (cols_count -3))
            {//-Insert Tags Name
                item_tablewidget[j]->setText(QString(""));
            }
            else if(j == (cols_count -2))
            {//-Insert Notes
                item_tablewidget[j]->setText(QString(""));
            }
            else if(j == (cols_count -1))
            {//-Last Blank Column
                item_tablewidget[j]->setText(QString(""));
            }
            else
            {

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
                qcount++;
            }

        }

        ///- Insertion of row in the table and display item data
        m_tableWidget_obj->insertRow(row_count);
        m_tableWidget_obj->setRowHeight(row_count,22);

        QFont font;
        font.setPixelSize(12);
        m_tableWidget_obj->setFont(font);

        for(int k = 0; k < cols_count ; k++)
        {
            m_tableWidget_obj->setItem(row_count, k, item_tablewidget[k]);
            m_tableWidget_obj->setColumnWidth(k,100);
        }

        row_count++;
    }

    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(100));

    ///- Property's to set on the table Widget

    m_tableWidget_obj->setColumnWidth(0,50);
    m_tableWidget_obj->setColumnWidth(1,40);

    m_tableWidget_obj->hideColumn(2);

    ///Sqliteviewer---------
    //    tableWidget->setColumnWidth(tableWidget->columnCount() - 1,100);
    //    tableWidget->hideColumn(tableWidget->columnCount() -1);

    m_tableWidget_obj->hideColumn(m_tableWidget_obj->columnCount() -2);
    m_tableWidget_obj->hideColumn(m_tableWidget_obj->columnCount() -3);
    m_tableWidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget_obj->verticalHeader()->setFixedWidth(50);
    m_tableWidget_obj->horizontalHeader()->setSectionsMovable(true);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    destination_db.close();
    return m_tableWidget_obj;
}

QTableWidget *sqlite_viewer_base_core_class::populate_tablewidget_filter_view_search(struct_global_sqlite_viewer_single_dbtable_info struct_obj, QString dbTableName)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    bool_global_export_checkbox_enable_sqlite_viewer = false;

    QTableWidget *m_tableWidget_obj  = new QTableWidget(this);
    m_tableWidget_obj->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget_obj->setAlternatingRowColors(true);
    m_tableWidget_obj->horizontalHeader()->setSectionsMovable(true);

    ///************** HEADER SETTING IN COLOUMS ******************///
    int cols_count = 3;  //- Total ColoumCount with the display
    m_tableWidget_obj->setColumnCount(cols_count);

    for(int  z = 0 ; z < cols_count ; z++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();


        if((z == 1) || (z == 0))
        {
            item->setIcon(QIcon("../icons/checked.png"));
            item->setCheckState(Qt::Checked);
            m_tableWidget_obj->setHorizontalHeaderItem(z,item);
            m_tableWidget_obj->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);
        }
        else
        {
            m_tableWidget_obj->setHorizontalHeaderItem(z,item);
            m_tableWidget_obj->horizontalHeaderItem(z)->setTextAlignment(Qt::AlignLeft);
        }
    }

    QStringList  coloum_name_list;
    coloum_name_list  << "Column Name" << "Search" << "Data Conversion";

    m_tableWidget_obj->setHorizontalHeaderLabels(coloum_name_list);

    QStringList cols_list = struct_obj.cols_list;
    QStringList view_list = struct_obj.view_list;
    QStringList search_list = struct_obj.search_list;

    QTableWidgetItem *item_col_name;
    QTableWidgetItem *item_search;
    QTableWidgetItem *item_combobox;

    int count = 0;
    for(int sv = 0 ; sv < (cols_list.size()) ; sv++)
    {
        //-[NOTE]: Below lines to hide RECON_notes from table
        //        if(sv == (cols_list.count() - 1))
        //            continue;


        item_col_name = new QTableWidgetItem();
        item_search = new QTableWidgetItem();
        item_combobox = new QTableWidgetItem;
        item_combobox->setText("");

        ///-1. Coloum Name with View Check box
        item_col_name->setText(cols_list.at(sv));
        if(view_list.at(sv) == QString("0"))
        {
            item_col_name->setCheckState(Qt::Unchecked);
        }
        else
        {
            item_col_name->setCheckState(Qt::Checked);
        }

        ///-2. Search Check box
        if(search_list.at(sv) == QString("0"))
        {
            //3
            item_search->setCheckState(Qt::Unchecked);
        }
        else
        {
            item_search->setCheckState(Qt::Checked);
        }

        QStringList combo_list;
        //        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "File" << "Size Human Readable";
        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "File" << "Size Human Readable" << "ASCII" << "Unicode";

        QComboBox* comboBox = new QComboBox();
        comboBox->setParent(m_tableWidget_obj);
        comboBox->addItems(combo_list);
        comboBox->setMaximumWidth(150);

        m_tableWidget_obj->insertRow(count);
        m_tableWidget_obj->setRowHeight(count,22);
        m_tableWidget_obj->setItem(count,0,item_col_name);
        m_tableWidget_obj->setItem(count,1,item_search);
        m_tableWidget_obj->setCellWidget(count,2,comboBox);

        // connect(comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_dynamic_combobox(QString)));

        count++;
    }

    ///- Property's to set on the table Widget

    QFont font;
    font.setPixelSize(MACRO_QMenu_Font_Size_int);
    m_tableWidget_obj->setFont(font);

    m_tableWidget_obj->hideRow(0);
    m_tableWidget_obj->hideRow(1);
    m_tableWidget_obj->hideRow(2);

    m_tableWidget_obj->hideRow((m_tableWidget_obj->rowCount() - 2));
    m_tableWidget_obj->hideRow((m_tableWidget_obj->rowCount() - 3));
    m_tableWidget_obj->hideRow((m_tableWidget_obj->rowCount() - 1));

    ///Sqliteviewer---------
    int mycount = 1;
    for(int iter =3; iter < (m_tableWidget_obj->rowCount() - 3); iter++){
        m_tableWidget_obj->setVerticalHeaderItem(iter,new QTableWidgetItem(QString::number(mycount++)));
    }

    ///------------------

    m_tableWidget_obj->setColumnWidth(0,180);
    m_tableWidget_obj->setColumnWidth(1,65);
    m_tableWidget_obj->setColumnWidth(2,100);
    m_tableWidget_obj->horizontalHeader()->setStretchLastSection(true);

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);

    m_tableWidget_obj->setColumnHidden(2,true);
    m_tableWidget_obj->setColumnHidden(1,true);

    return m_tableWidget_obj;
}


QStringList sqlite_viewer_base_core_class::get_data_type_from_database_table(QSqlDatabase &destination_db, QString table_name)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    QStringList dataTypeList;

    QString dbCreateStr;
    QSqlRecord dbRecord;

    QSqlQuery queryRead(destination_db);
    queryRead.prepare("SELECT * FROM '" + table_name + "'");
    if(!queryRead.exec())
    {
        recon_static_functions::app_debug("Update query ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(queryRead.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(queryRead.executedQuery(),Q_FUNC_INFO);
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
        else{
            typeName.clear();
            typeName = "VARCHAR(500)";
        }

        dataTypeList << typeName;
    }

    dbCreateStr.clear();
    typeName.clear();

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);

    return dataTypeList;
}


void sqlite_viewer_base_core_class::slot_custom_table_header_widget_search_field_return_pressed()
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

void sqlite_viewer_base_core_class::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void sqlite_viewer_base_core_class::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);


    if(column != enum_tablewidget_column_bookmark
            && column != enum_tablewidget_column_tags)
    {
        if(column_sort_order == 0)
            current_tab_tableWidget_obj->sortItems(column, Qt::DescendingOrder);
        else
            current_tab_tableWidget_obj->sortItems(column, Qt::AscendingOrder);

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

void sqlite_viewer_base_core_class::all_column_text_search(QList<struct_search_column_text_for_sqlite_viewer> search_text_all_column_list)
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

void sqlite_viewer_base_core_class::pub_set_current_tab_detail(QString tab_name, m_tablewidget *current_tab_widget)
{
    current_tab_name = tab_name;
    current_tab_tableWidget_obj = current_tab_widget;
}
