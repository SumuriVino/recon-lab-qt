#include "redefined_result_filters_interface.h"
#include "ui_redefined_result_filters_interface.h"

redefined_result_filters_interface::redefined_result_filters_interface(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::redefined_result_filters_interface)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    setWindowTitle("Redefined Result Filters");
    setWindowFlags(Qt::Window);

    create_gui();

    tableWidget_filters_item->setStyleSheet(stylesheet_for_tablewidgets());
    redefined_result_filters_list.clear();


    message_dialog_obj = new message_dialog(this);

    redefined_result_filters_add_new_filter_obj = new redefined_result_filters_add_new_filter(this);
    connect(redefined_result_filters_add_new_filter_obj,SIGNAL(signal_add_new_filter_in_tablewidget(QString)),this,SLOT(slot_add_new_filter_in_tablewidget(QString)));

    redefined_result_filters_plugins_tab_select_obj = new redefined_result_filters_plugins_tab_select;
    connect(redefined_result_filters_plugins_tab_select_obj, SIGNAL(signal_selected_plugin_and_tab(QString,QString)), this, SLOT(slot_selected_plugin_and_tab(QString,QString)));

    redefined_result_filters_plugin_tab_column_select_obj = new redefined_result_filters_plugin_tab_column_select(this);
    connect(redefined_result_filters_plugin_tab_column_select_obj,SIGNAL(signal_sel_col_name(QString,int,int)),this,SLOT(slot_select_column_name(QString,int,int)));

    create_redefined_result_filters_database_and_tables();
    display_redefined_result_filter_names();
}

redefined_result_filters_interface::~redefined_result_filters_interface()
{
    delete ui;
}


void redefined_result_filters_interface::pub_display_plugins_category_list_interface()
{
    redefined_result_filters_plugins_tab_select_obj->pub_display_plugins_information();
}

void redefined_result_filters_interface::create_redefined_result_filters_database_and_tables()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

#ifndef MACRO_RECON_STATIC_NOTARISE_ESSENTIALS
    return;
#endif

    bool bool_need_to_update_db = false;
    if(!bool_need_to_update_db)
        return;

    redefined_result_filter_db_path = "../Resources/features_resources/redefined_result_filters/redefined_result_filters.sqlite";

    QFileInfo info(redefined_result_filter_db_path);

    if(info.exists())
        return;

    QString command;
    command = "create table category (INT INTEGER PRIMARY KEY, category_name varchar(100))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, redefined_result_filter_db_path,Q_FUNC_INFO))
        recon_static_functions::app_debug(" create table category ------FAILED-----",Q_FUNC_INFO);

    command.clear();
    command = "create table plugin_tab (INT INTEGER PRIMARY KEY, category_INT INTEGER, plugin_tab_name varchar(100))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, redefined_result_filter_db_path,Q_FUNC_INFO))
        recon_static_functions::app_debug("create table plugin_tab ------FAILED-----",Q_FUNC_INFO);

    command.clear();
    command = "create table column_details (INT INTEGER PRIMARY KEY, category_INT INTEGER, plugin_tab_INT INTEGER, header_name, column_name varchar(100))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, redefined_result_filter_db_path,Q_FUNC_INFO))
        recon_static_functions::app_debug("create table column_details ------FAILED-----",Q_FUNC_INFO);

    command.clear();
    command = "create table result_headers (INT INTEGER PRIMARY KEY, category_INT INTEGER,  header_name varchar(100))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, redefined_result_filter_db_path,Q_FUNC_INFO))
        recon_static_functions::app_debug("create table result_headers ------FAILED-----",Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_filters_interface::display_redefined_result_filter_names()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(redefined_result_filter_db_path);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    while( tableWidget_filters_name->rowCount())
        tableWidget_filters_name->removeRow(0);

    QSqlQuery query(destination_db);

    query.exec("select INT, category_name from category");
    int row = 0;
    while(query.next())
    {
        QTableWidgetItem *item_INT = new QTableWidgetItem;
        QTableWidgetItem *item_category_name = new QTableWidgetItem;

        item_INT->setText(query.value(0).toString());
        item_category_name->setText(query.value(1).toString());

        tableWidget_filters_name->insertRow(row);
        tableWidget_filters_name->setRowHeight(row,23);
        tableWidget_filters_name->setItem(row, 0, item_INT);
        tableWidget_filters_name->setItem(row, 1, item_category_name);

        row++;
    }

    destination_db.close();
    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}

void redefined_result_filters_interface::set_plugin_specific_tab_list(QString plugin_tab_name)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QStringList list = plugin_tab_name.split("::", Qt::SkipEmptyParts);

    QStringList header_list = global_csv_reader_class_selected_obj->get_header_list_of_tab(list.at(0),list.at(1));
    redefined_result_filters_plugin_tab_column_select_obj->populate_columns_in_tablewidget(header_list);

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_selected_plugin_and_tab(QString plugin_name, QString tab_name)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    redefined_result_filters_plugins_tab_select_obj->hide();

    QString plugin_tab_name = plugin_name + "::" + tab_name;

    QString current_category_INT =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();

    QString command = QString("insert into plugin_tab (plugin_tab_name, category_INT) values(?,?)");
    QStringList values;
    values << plugin_tab_name << current_category_INT;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values, redefined_result_filter_db_path, Q_FUNC_INFO);

    display_redefined_result_items_on_click_result_name();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void redefined_result_filters_interface::slot_select_column_name(QString sel_col_str, int row, int column)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(sel_col_str == QString(""))
        return;

    int current_row_result_item = row;
    int current_column_result_item = column;

    if(current_row_result_item < 0 || current_column_result_item < 0)
        return;

    QString current_category_INT =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();
    QString plugin_tab_name_INT =  tableWidget_filters_item->item(current_row_result_item, 0)->text();
    QString header_name =  tableWidget_filters_item->horizontalHeaderItem(current_column_result_item)->text();

    QString command_del = QString("delete from column_details where category_INT=? AND plugin_tab_INT=? AND header_name=?");
    QStringList values_del;
    values_del << current_category_INT << plugin_tab_name_INT << header_name;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_del, values_del, redefined_result_filter_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("delete from column_details-------FAILED-------",Q_FUNC_INFO);
    }

    QString command = QString("insert into column_details (category_INT, plugin_tab_INT, header_name, column_name) values(?,?,?,?)");
    QStringList values;
    values << current_category_INT << plugin_tab_name_INT << header_name << sel_col_str;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values, redefined_result_filter_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("insert into column_details -------FAILED-------",Q_FUNC_INFO);
    }

    display_redefined_result_items_on_click_result_name();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_add_new_filter_in_tablewidget(QString category_name)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QString command = QString("insert into category (category_name) values(?)");
    QStringList values;
    values << category_name;

    bool bool_1 = recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values, redefined_result_filter_db_path, Q_FUNC_INFO);
    if(!bool_1)
    {
        recon_static_functions::app_debug("insert into category ------FAILED--------",Q_FUNC_INFO);
        return;
    }

    display_redefined_result_filter_names();

    tableWidget_filters_name->selectRow(tableWidget_filters_name->rowCount() - 1);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_remove_column_pushButton_clicked()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    int col_no = -1;
    col_no =  tableWidget_filters_item->currentColumn();

    if(col_no == -1){
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"No Column to Remove");
        message_dialog_obj->show();
        return;
    }

    tableWidget_filters_item->removeColumn(col_no);
    if( tableWidget_filters_item->columnCount() == 0){
        tableWidget_filters_item->setRowCount(0);
    }

    //----- code to remove column info from db ----//

    // --- (1) Delete info of table from db ----//
    int curnt_row =  tableWidget_filters_name->currentRow();
    if(curnt_row == -1){
        recon_static_functions::app_debug("No filter seleted to remove",Q_FUNC_INFO);
        return;
    }

    QString table_name_str =  tableWidget_filters_name->item(curnt_row,0)->text();

    QString command = "DROP TABLE " + table_name_str;
    QString dest_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "index_redefined_result.sqlite";

    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,dest_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" DROP TABLE------FAILED-----",Q_FUNC_INFO);
    }

    QString cmnd = "DELETE FROM all_configs where total_configs='" +  table_name_str + "'";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(cmnd,dest_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("DELETE FROM all_configs ------FAILED-----",Q_FUNC_INFO);
    }

    //---- (2) Recreate coreesponding table in the db ----//

    after_remove_column_insertion_of_table_into_db();

    //----- ends here -----//

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_filters_interface::after_remove_column_insertion_of_table_into_db()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if( tableWidget_filters_name->currentRow() == -1){
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please Select a Category to Save.");
        message_dialog_obj->show();
        return;
    }

    if( tableWidget_filters_item->columnCount() == 0){
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Nothing to Save.");
        message_dialog_obj->show();
        return;
    }

    QString table_name =  tableWidget_filters_name->currentItem()->text();

    QString col_nme;
    QString cmd_str_1;
    cmd_str_1 += "(Plugin_Tab";

    for(int itr=0; itr <  tableWidget_filters_item->columnCount(); itr++){
        if(itr == 0)
            continue;
        col_nme += ",";
        col_nme += "`";
        col_nme +=  tableWidget_filters_item->horizontalHeaderItem(itr)->text();
        col_nme += "`";
        col_nme += " VARCHAR(100)";

        cmd_str_1 += ",";
        cmd_str_1 += "`";
        cmd_str_1 +=  tableWidget_filters_item->horizontalHeaderItem(itr)->text();
        cmd_str_1 += "`";
    }

    cmd_str_1 += ")";

    QString command = "CREATE TABLE " + table_name + "(INT INTEGER PRIMARY KEY,Plugin_Tab VARCHAR(500)" + col_nme + ")";
    QString dest_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "index_redefined_result.sqlite";

    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,dest_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("CREATE TABLE  -------FAILED-------",Q_FUNC_INFO);
    }

    //-----Now table is created .Put the data in it.------//

    for(int itr=0; itr <  tableWidget_filters_item->rowCount(); itr++){

        QStringList values;
        values.clear();

        QString values_str;
        values_str += "values (";

        for(int j=0; j <  tableWidget_filters_item->columnCount(); j++){
            values <<  tableWidget_filters_item->item(itr,j)->text();
            values_str += "?,";
        }

        if(values_str.endsWith(",")){
            values_str.remove((values_str.size() -1),1);
            values_str += ")";
        }

        QString commands;
        commands = "insert into " + table_name + " " + cmd_str_1 + " " + values_str;
        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(commands,values, dest_db_file,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("insert into -------FAILED-------",Q_FUNC_INFO);
        }

    }

    //------ Now put the table name into all-configs ----//

    QString cmnd = "insert into all_configs (total_configs) values (?)";
    QStringList value_lst;
    value_lst << table_name;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmnd,value_lst, dest_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" insert into all_configs -------FAILED-------",Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

QString redefined_result_filters_interface::stylesheet_for_tablewidgets()
{
    QString stylesheet = QString(
                "QTableView::item {"
                "border-top: 1px solid white;"
                " }"


                "QTableView::item:selected:!active {"
                "color:white;"
                "background-color:  #116CD6;"
                "}"

                "QTableView::indicator:checked {background-color:black;border: 1px solid #d6d6d6;}"
                "QTableView::indicator:unchecked {background-color:#f1f1f1;border: 1px solid #777777;}"

                "QTableView::indicator {"
                "width: 13px;"
                "height: 13px;}"
                );

    return stylesheet;

}

void redefined_result_filters_interface::display_redefined_result_items_on_click_result_name()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(tableWidget_filters_name->currentRow() < 0)
        return;

    while( tableWidget_filters_item->columnCount())
        tableWidget_filters_item->removeColumn(0);

    while( tableWidget_filters_item->rowCount())
        tableWidget_filters_item->removeRow(0);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(redefined_result_filter_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QString current_category_INT =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();
    QString command = QString("select header_name from result_headers where category_INT = ? ORDER by INT ASC");
    QStringList values;
    values << current_category_INT;
    QStringList header_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command, values, 0, destination_db, Q_FUNC_INFO);

    create_tableWidget_result_item_headers(header_list);

    int row_count = 0;
    QString command_1 = QString("select INT, plugin_tab_name from plugin_tab where category_INT=") + current_category_INT;

    QSqlQuery query(destination_db);
    query.exec(command_1);

    while(query.next())
    {
        QString plugin_tab_INT = query.value(0).toString();
        QString plugin_tab_name = query.value(1).toString();

        QTableWidgetItem *item_INT = new QTableWidgetItem;
        QTableWidgetItem *item_plugin_tab_name = new QTableWidgetItem;

        item_INT->setText(plugin_tab_INT);
        item_plugin_tab_name->setText(plugin_tab_name);

        tableWidget_filters_item->insertRow(row_count);

        tableWidget_filters_item->setItem(row_count, 0, item_INT);
        tableWidget_filters_item->setItem(row_count, 1, item_plugin_tab_name);


        for(int i = 0; i < header_list.size(); i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            tableWidget_filters_item->setItem(row_count, i + 2, item);
        }

        for(int i = 0; i < header_list.size(); i++)
        {
            QString command_2 = QString("select column_name from column_details where category_INT=? AND plugin_tab_INT=? AND header_name=?");
            QStringList values_2;
            values_2 << current_category_INT << plugin_tab_INT << header_list.at(i);
            QStringList res_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_2, values_2, 0, redefined_result_filter_db_path, Q_FUNC_INFO);

            if(res_list.size() < 1)
                continue;

            QString column_name = res_list.at(0);

            tableWidget_filters_item->item(row_count, i + 2)->setText(column_name);
        }
        row_count++;
    }

    tableWidget_filters_item->hideColumn(0);

    destination_db.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_filters_interface::create_tableWidget_result_item_headers(QStringList headers_list)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    while( tableWidget_filters_item->columnCount())
        tableWidget_filters_item->removeColumn(0);

    QTableWidgetItem *item_INT = new QTableWidgetItem;
    item_INT->setText("");
    tableWidget_filters_item->insertColumn(0);
    tableWidget_filters_item->setHorizontalHeaderItem(0, item_INT);

    QTableWidgetItem *item_plugin_tab_name = new QTableWidgetItem;
    item_plugin_tab_name->setText("Plugin::Tab");
    tableWidget_filters_item->insertColumn(1);
    tableWidget_filters_item->setHorizontalHeaderItem(1, item_plugin_tab_name);

    for(int i = 0 ; i < headers_list.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;

        item->setText(headers_list.at(i));
        tableWidget_filters_item->insertColumn(i + 2);
        tableWidget_filters_item->setHorizontalHeaderItem(i + 2, item);

    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

int redefined_result_filters_interface::header_count_for_category(QString category_INT)
{
    if( tableWidget_filters_name->currentRow() < 0)
        return -1;

    QString command = "select COUNT(header_name) from result_headers where category_INT=" + category_INT;

    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, redefined_result_filter_db_path, Q_FUNC_INFO);

    if(list.size() < 1)
        return -1;
    else
        return list.at(0).toInt();
}

void redefined_result_filters_interface::create_gui()
{
    vboxlayout_main_central_display = new QVBoxLayout(this);

    //-Upper Mainlayout.
    hboxlayout_upper_main_display = new QHBoxLayout(vboxlayout_main_central_display->widget());

    create_filters_name_layout();
    create_filter_items_layout();

    hboxlayout_upper_main_display->addLayout(vboxLayout_filters_name_section);
    hboxlayout_upper_main_display->addLayout(vboxlayout_filters_items_section);
    hboxlayout_upper_main_display->setContentsMargins(0,0,0,0);
    hboxlayout_upper_main_display->setSpacing(4);
    hboxlayout_upper_main_display->setStretch(0,0);
    hboxlayout_upper_main_display->setStretch(1,1);


    vboxlayout_main_central_display->addLayout(hboxlayout_upper_main_display);
    vboxlayout_main_central_display->setContentsMargins(8,8,8,8);
    vboxlayout_main_central_display->setSpacing(4);
    vboxlayout_main_central_display->setStretch(0,1);
    vboxlayout_main_central_display->setStretch(1,0);

    this->setLayout(vboxlayout_main_central_display);

    create_widget_for_the_add_coulum_lineEdit();
}

void redefined_result_filters_interface::create_filters_name_layout()
{
    vboxLayout_filters_name_section = new QVBoxLayout;
    label_filters_name_heading = new QLabel(this);
    label_filters_name_heading->setText("Redefined Result");

    tableWidget_filters_name = new QTableWidget(this);
    connect(tableWidget_filters_name,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tableWidget_filter_name_cellClicked(int,int)));
    tableWidget_filters_name->setStyleSheet(stylesheet_for_tablewidgets());
    tableWidget_filters_name->hideColumn(0);
    tableWidget_filters_name->insertColumn(0);
    tableWidget_filters_name->insertColumn(1);
    QStringList list_headers;
    list_headers <<"INT" << "Filter Name";
    tableWidget_filters_name->setHorizontalHeaderLabels(list_headers);
    tableWidget_filters_name->hideColumn(0);
    tableWidget_filters_name->horizontalHeader()->setStretchLastSection(true);
    tableWidget_filters_name->setShowGrid(false);
    tableWidget_filters_name->setSelectionBehavior(QAbstractItemView::SelectRows);

    pushButton_add_filter = new QPushButton("Add",this);
    pushButton_add_filter->setFixedWidth(80);
    connect(pushButton_add_filter,SIGNAL(clicked()),this,SLOT(slot_pushButton_add_filter_clicked()));

    pushButton_remove_filter  = new QPushButton("Remove",this);
    pushButton_remove_filter->setFixedWidth(80);
    connect(pushButton_remove_filter,SIGNAL(clicked()),this,SLOT(slot_pushButton_remove_filter_clicked()));

    QHBoxLayout *hbox_filters_buttons = new QHBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(30,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
    hbox_filters_buttons->addWidget(pushButton_add_filter);
    hbox_filters_buttons->addSpacerItem(spacer);
    hbox_filters_buttons->addWidget(pushButton_remove_filter);

    vboxLayout_filters_name_section->addWidget(label_filters_name_heading);
    vboxLayout_filters_name_section->addWidget(tableWidget_filters_name);
    vboxLayout_filters_name_section->addLayout(hbox_filters_buttons);

}

void redefined_result_filters_interface::create_filter_items_layout()
{
    vboxlayout_filters_items_section = new QVBoxLayout;
    label_filters_item_heading = new QLabel(this);
    label_filters_item_heading->setText("Result Entries");
    tableWidget_filters_item = new QTableWidget(this);
    connect(tableWidget_filters_item,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_filter_item_DoubleClicked(int, int)));

    tableWidget_filters_item->horizontalHeader()->setStretchLastSection(true);
    tableWidget_filters_item->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *hbox_buttons_layout = new QHBoxLayout;
    pushButton_add_column = new QPushButton("Add Column",this);
    pushButton_add_column->setFixedWidth(120);
    connect(pushButton_add_column,SIGNAL(clicked()),this,SLOT(slot_pushButton_add_column_clicked()));

    pushButton_add_row = new QPushButton("Add Row",this);
    pushButton_add_row->setFixedWidth(120);
    connect(pushButton_add_row,SIGNAL(clicked()),this,SLOT(slot_pushButton_add_row_clicked()));

    pushButton_remove_selected_columns = new QPushButton("Remove Column",this);
    pushButton_remove_selected_columns->setFixedWidth(120);
    connect(pushButton_remove_selected_columns,SIGNAL(clicked()),this,SLOT(slot_pushButton_remove_selected_columns_clicked()));
    pushButton_remove_selected_columns->setHidden(true);
    pushButton_remove_selected_row = new QPushButton("Remove Row",this);
    pushButton_remove_selected_row->setFixedWidth(120);
    connect(pushButton_remove_selected_row,SIGNAL(clicked()),this,SLOT(slot_pushButton_remove_selected_row_clicked()));

    QSpacerItem *spacer = new QSpacerItem(20,10,QSizePolicy::Expanding,QSizePolicy::Minimum);

    hbox_buttons_layout->addWidget(pushButton_add_column);
    hbox_buttons_layout->addWidget(pushButton_add_row);
    hbox_buttons_layout->addSpacerItem(spacer);
    hbox_buttons_layout->addWidget(pushButton_remove_selected_columns);
    hbox_buttons_layout->addWidget(pushButton_remove_selected_row);

    vboxlayout_filters_items_section->addWidget(label_filters_item_heading);
    vboxlayout_filters_items_section->addWidget(tableWidget_filters_item);
    vboxlayout_filters_items_section->addLayout(hbox_buttons_layout);

}

void redefined_result_filters_interface::create_widget_for_the_add_coulum_lineEdit()
{
    widget_add_column = new QWidget(pushButton_add_column);
    widget_add_column->setWindowTitle("Add Column");
    widget_add_column->setWindowFlags(Qt::Window);
    QVBoxLayout *vboxLayout_addcoloum_widget = new QVBoxLayout(widget_add_column);
    lineEdit_column_name = new QLineEdit(widget_add_column);
    lineEdit_column_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(lineEdit_column_name,SIGNAL(textChanged(QString)),this,SLOT(slot_lineEdit_add_column_text_changed(QString)));

    pushButton_widget_add_coloum = new QPushButton("Add",widget_add_column);
    connect(pushButton_widget_add_coloum,SIGNAL(clicked()),this,SLOT(slot_pushbutton_widget_add_coloum_clicked()));


    label_add_coloum_widget_message = new QLabel(widget_add_column);
    label_add_coloum_widget_message->setMinimumWidth(200);
    QHBoxLayout *hbox_button_layout = new QHBoxLayout;
    hbox_button_layout->addWidget(label_add_coloum_widget_message);
    hbox_button_layout->addWidget(pushButton_widget_add_coloum);
    hbox_button_layout->setContentsMargins(4,4,4,4);

    vboxLayout_addcoloum_widget->addWidget(lineEdit_column_name);
    vboxLayout_addcoloum_widget->addLayout(hbox_button_layout);
    vboxLayout_addcoloum_widget->setContentsMargins(4,4,4,4);
    widget_add_column->setLayout(vboxLayout_addcoloum_widget);
}

void redefined_result_filters_interface::slot_pushButton_add_filter_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    redefined_result_filters_add_new_filter_obj->refresh_filters_list_on_add_new_filter(redefined_result_filters_list);
    redefined_result_filters_add_new_filter_obj->show();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_pushButton_remove_filter_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    if(tableWidget_filters_name->currentRow() < 0)
        return;

    QString current_category_INT =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();

    QString command_1 = QString("delete from result_headers where category_INT=?");
    QStringList values_1;
    values_1 << current_category_INT;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_1, values_1, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from result_headers  ------FAILED-----",Q_FUNC_INFO);


    QString command_2 = QString("delete from plugin_tab where category_INT=?");
    QStringList values_2;
    values_2 << current_category_INT;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_2, values_2, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from plugin_tab  ------FAILED-----",Q_FUNC_INFO);

    QString command_3 = QString("delete from column_details where category_INT=?");
    QStringList values_3;
    values_3 << current_category_INT;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_3, values_3, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from column_details  ------FAILED-----",Q_FUNC_INFO);

    QString command_4 = QString("delete from category where INT=?");
    QStringList values_4;
    values_4 << current_category_INT;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_4, values_4, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from category  ------FAILED-----",Q_FUNC_INFO);

    display_redefined_result_filter_names();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void redefined_result_filters_interface::slot_tableWidget_filter_name_cellClicked(int row, int column)
{
    lineEdit_column_name->clear();
    display_redefined_result_items_on_click_result_name();
}

void redefined_result_filters_interface::slot_pushButton_add_column_clicked()
{
    widget_add_column->show();
}

void redefined_result_filters_interface::slot_pushButton_add_row_clicked()
{
    redefined_result_filters_plugins_tab_select_obj->pub_clear_plugins_tab_tablewidget();
    redefined_result_filters_plugins_tab_select_obj->show();
}

void redefined_result_filters_interface::slot_pushButton_remove_selected_columns_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    if(tableWidget_filters_item->currentColumn() < 2)
        return;

    QString current_category_INT =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();
    QString current_selected_column_header =  tableWidget_filters_item->horizontalHeaderItem( tableWidget_filters_item->currentColumn())->text();
    QString current_selected_plugin_tab_INT =  tableWidget_filters_item->item( tableWidget_filters_item->currentRow(), 0)->text();

    QString command_1 = QString("delete from result_headers where category_INT=? AND header_name=?");
    QStringList values_1;
    values_1 << current_category_INT << current_selected_column_header;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_1, values_1, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from result_headers  ------FAILED-----",Q_FUNC_INFO);

    QString command_2 = QString("delete from column_details where category_INT=? AND plugin_tab_INT=? AND header_name=?");
    QStringList values_2;
    values_2 << current_category_INT << current_selected_plugin_tab_INT << current_selected_column_header;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_2, values_2, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from column_details  ------FAILED-----",Q_FUNC_INFO);

    display_redefined_result_items_on_click_result_name();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_pushButton_remove_selected_row_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    if(tableWidget_filters_name->currentRow() < 0)
        return;

    if(tableWidget_filters_item->currentRow() < 0)
        return;

    QString current_category_INT =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();
    QString current_selected_plugin_tab_INT =  tableWidget_filters_item->item( tableWidget_filters_item->currentRow(), 0)->text();

    //    QString command_1 = QString("delete from result_headers where category_INT=?");
    //    QStringList values_1;
    //    values_1 << current_category_INT;
    //    if(!recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_1, values_1, redefined_result_filter_db_path, Q_FUNC_INFO))
    //        recon_static_functions::app_debug("delete from result_headers  ------FAILED-----",Q_FUNC_INFO);

    QString command_2 = QString("delete from plugin_tab where category_INT=? AND INT=?");
    QStringList values_2;
    values_2 << current_category_INT << current_selected_plugin_tab_INT;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_2, values_2, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from plugin_tab  ------FAILED-----",Q_FUNC_INFO);

    QString command_3 = QString("delete from column_details where category_INT=? AND plugin_tab_INT=?");
    QStringList values_3;
    values_3 << current_category_INT << current_selected_plugin_tab_INT;
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_3, values_3, redefined_result_filter_db_path, Q_FUNC_INFO))
        recon_static_functions::app_debug("delete from column_details  ------FAILED-----",Q_FUNC_INFO);

    display_redefined_result_items_on_click_result_name();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_pushbutton_widget_add_coloum_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    if( tableWidget_filters_name->currentRow() < 0)
        return;

    if( lineEdit_column_name->text().trimmed() == QString(""))
    {
        lineEdit_column_name->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        label_add_coloum_widget_message->setText("Enter Coloum name.");
        return;
    }

    bool bool_header_exists_with_same_name = false;

    for(int i = 0; i <  tableWidget_filters_item->columnCount(); i++)
    {
        if( tableWidget_filters_item->horizontalHeaderItem(i)->text() ==  lineEdit_column_name->text().trimmed())
            bool_header_exists_with_same_name = true;
    }

    if(bool_header_exists_with_same_name)
    {
        lineEdit_column_name->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        label_add_coloum_widget_message->setText("Coloum already exist.");
        return;
    }

    widget_add_column->hide();

    QString category_int =  tableWidget_filters_name->item( tableWidget_filters_name->currentRow(), 0)->text();

    QString command;

    command = QString("insert into result_headers (category_INT, header_name) values(?,?)");
    QStringList values;
    values << category_int <<  lineEdit_column_name->text().trimmed();

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values, redefined_result_filter_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" insert into result_headers  ------FAILED-----",Q_FUNC_INFO);
    }

    lineEdit_column_name->clear();
    display_redefined_result_items_on_click_result_name();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_filters_interface::slot_lineEdit_add_column_text_changed(QString arg1)
{
    if(arg1 != "")
    {
        lineEdit_column_name->setStyleSheet(styleSheet());
        label_add_coloum_widget_message->clear();
    }
}

void redefined_result_filters_interface::slot_tableWidget_filter_item_DoubleClicked(int row, int column)
{
    if(column < 2)
        return;

    set_plugin_specific_tab_list( tableWidget_filters_item->item(row, 1)->text());
    redefined_result_filters_plugin_tab_column_select_obj->pub_set_filter_row_and_column(row, column);
    redefined_result_filters_plugin_tab_column_select_obj->show();

}
