#include "existing_hash_column_selection.h"
#include "ui_existing_hash_column_selection.h"

existing_hash_column_selection::existing_hash_column_selection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::existing_hash_column_selection)
{
    ui->setupUi(this);

    message_dialog_obj = new message_dialog(this);

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));

    create_gui();

}

existing_hash_column_selection::~existing_hash_column_selection()
{
    delete ui;
}

void existing_hash_column_selection::pub_set_hashset_toolbar_essentials(bool is_sqlite, bool is_project_vic)
{
    bool_hashset_toolbar_is_sqlite = is_sqlite;
    bool_hashset_toolbar_is_project_vic = is_project_vic;
}

void existing_hash_column_selection::pub_select_file_for_hashset_toolbar()
{
    sqlite_file_path.clear();
    table_widget_Tables->setRowCount(0);
    table_widget_sqlite_data->setRowCount(0);

    if(bool_hashset_toolbar_is_sqlite)
        file_dialog_obj.setNameFilter(("*.sqlite *.db"));
    else if(bool_hashset_toolbar_is_project_vic)
        file_dialog_obj.setNameFilter(("*json"));

    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            sqlite_file_path = selected_files.at(0);
        }
    }
    sqlite_file_path = sqlite_file_path.trimmed();

    if(sqlite_file_path.isEmpty())
    {
        emit signal_pushbutton_cancel_clicked();
        return;
    }


    QFileInfo info(sqlite_file_path);
    if(bool_hashset_toolbar_is_sqlite)
    {
        create_table_display();
        emit signal_selected_hash_database_file(sqlite_file_path);
    }
    else if(bool_hashset_toolbar_is_project_vic)
    {
        emit signal_selected_vic_supported_file(sqlite_file_path);
    }
}

void existing_hash_column_selection::pub_select_file()
{
    sqlite_file_path.clear();
    table_widget_Tables->setRowCount(0);
    table_widget_sqlite_data->setRowCount(0);


    file_dialog_obj.setNameFilter(("*.sqlite *.db *json"));
    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            sqlite_file_path = selected_files.at(0);
        }
    }
    sqlite_file_path = sqlite_file_path.trimmed();

    if(sqlite_file_path.isEmpty())
    {
        emit signal_pushbutton_cancel_clicked();
        return;
    }

    QFileInfo info(sqlite_file_path);
    if(info.suffix() == QString("sqlite") || info.suffix() == QString("db"))
    {
        create_table_display();
        emit signal_selected_hash_database_file(sqlite_file_path);
    }
    else if(info.suffix() == QString("json"))
    {
        emit signal_selected_vic_supported_file(sqlite_file_path);
    }

}


void existing_hash_column_selection::create_gui()
{

    QVBoxLayout *v_boxlayout = new QVBoxLayout;

    QString table_stylesheet = QString("QTableWidget {"
                                       "selection-background-color: #0069D7;"
                                       "QTableView::item:selected:!active {color:white;"
                                       "background-color:  #0069EB};"
                                       "}");

    table_widget_Tables = new QTableWidget(this);
    table_widget_Tables->setColumnCount(1);
    table_widget_Tables->setSelectionMode(QAbstractItemView::SingleSelection);

    //table_widget_Tables->setShowGrid(false);
    table_widget_Tables->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_widget_Tables->verticalHeader()->setVisible(false);
    table_widget_Tables->alternatingRowColors();
    table_widget_Tables->setStyleSheet(table_stylesheet);

    QTableWidgetItem *tbl_header_item = new QTableWidgetItem;
    tbl_header_item->setText("Tables");
    table_widget_Tables->setHorizontalHeaderItem(0 , tbl_header_item);

    QHBoxLayout *h_boxlayout = new QHBoxLayout;
    h_boxlayout->setContentsMargins(0,0,0,0);
    h_boxlayout->setSpacing(1);
    table_widget_Tables->horizontalHeader()->setStretchLastSection(true);



    QStringList table_data_header_list;
    table_data_header_list << "Columns" << "Data";

    table_widget_sqlite_data = new QTableWidget(this);

    table_widget_sqlite_data->setColumnCount(table_data_header_list.size());
    table_widget_sqlite_data->setHorizontalHeaderLabels(table_data_header_list);
    table_widget_sqlite_data->horizontalHeader()->stretchLastSection();
    table_widget_sqlite_data->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //table_widget_sqlite_data->setShowGrid(false);
    table_widget_sqlite_data->verticalHeader()->setVisible(false);
    table_widget_sqlite_data->alternatingRowColors();
    table_widget_sqlite_data->setStyleSheet(table_stylesheet);

    QSplitter *splitter_main_display = new QSplitter;
    splitter_main_display->setHandleWidth(0);
    splitter_main_display->addWidget(table_widget_Tables);
    splitter_main_display->addWidget(table_widget_sqlite_data);
    //    splitter_main_display->setContentsMargins(1,0,1,0);
    splitter_main_display->setContentsMargins(1,1,1,1);

    splitter_main_display->setStretchFactor(1,1);

    splitter_main_display->setOrientation(Qt::Horizontal);

    h_boxlayout->addWidget(splitter_main_display);


    pushbutton_save   = new QPushButton("Save", this);
    pushbutton_cancel = new QPushButton("Cancel", this);

    connect(pushbutton_save,SIGNAL(clicked(bool)),this,SLOT(slot_pushbutton_save_clicked(bool)));
    connect(pushbutton_cancel,SIGNAL(clicked(bool)),this,SLOT(slot_pushbutton_cancel_clicked(bool)));


    QHBoxLayout *h_box_pushbutton = new QHBoxLayout;

    QSpacerItem *spacer = new QSpacerItem(15,10,QSizePolicy::Expanding,QSizePolicy::Maximum);

    h_box_pushbutton->addSpacerItem(spacer);
    h_box_pushbutton->addWidget(pushbutton_save);
    h_box_pushbutton->addWidget(pushbutton_cancel);

    v_boxlayout->setContentsMargins(0,0,0,0);

    v_boxlayout->setSpacing(2);


    v_boxlayout->addLayout(h_boxlayout);
    v_boxlayout->addLayout(h_box_pushbutton);
    v_boxlayout->setContentsMargins(1,1,1,1);

    setLayout(v_boxlayout);

    connect(table_widget_Tables,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slot_table_item_clicked(QTableWidgetItem*)));

}

void existing_hash_column_selection::create_table_display()
{

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(sqlite_file_path);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Database open ---FAILED---:" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error :" + destination_db.lastError().text() , Q_FUNC_INFO);
        return ;
    }

    QStringList table_list;
    table_list = destination_db.tables();

    destination_db.close();

    for(int tbl_count = 0 ; tbl_count < table_list.size() ; tbl_count++)
    {

        table_widget_Tables->insertRow(tbl_count);
        table_widget_Tables->setRowHeight(tbl_count,25);

        QTableWidgetItem *tables_items = new QTableWidgetItem;
        tables_items->setText(table_list.at(tbl_count));
        table_widget_Tables->setItem(tbl_count,0,tables_items);
        table_widget_Tables->resizeColumnsToContents();
        table_widget_Tables->resizeRowsToContents();

    }

    destination_db.close();

}

void existing_hash_column_selection::slot_pushbutton_save_clicked(bool status)
{
    Q_UNUSED(status);

    int current_table_row = table_widget_Tables->currentRow();

    if(current_table_row < 0)
    {
        /// No table selected message

        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "No Table Selected.");
        message_dialog_obj->show();

        return;
    }

    QString table_name_str = table_widget_Tables->item(current_table_row, 0)->text().trimmed();



    QString column_name_str;
    for(int row = 0; row < table_widget_sqlite_data->rowCount(); row++)
    {
        QRadioButton *radiobutton_table = dynamic_cast<QRadioButton*>(table_widget_sqlite_data->cellWidget(row,0));

        if(radiobutton_table->isChecked())
        {
            column_name_str =  radiobutton_table->text().trimmed();
            break;
        }

    }


    if(column_name_str.isEmpty())
    {
        /// no radiobutton selected
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, "No Column Selected.");
        message_dialog_obj->show();
        return;

    }


    QString saved_data_str = table_name_str + QString(MACRO_RECON_Splitter_1_multiple) + column_name_str + QString(MACRO_RECON_Splitter_1_multiple) + sqlite_file_path;

    emit signal_pushbutton_save_clicked(saved_data_str);

}

void existing_hash_column_selection::slot_pushbutton_cancel_clicked(bool status)
{
    Q_UNUSED(status);

    sqlite_file_path.clear();
    emit signal_pushbutton_cancel_clicked();

}


void existing_hash_column_selection::slot_table_item_clicked(QTableWidgetItem *item)
{
    QString clicked_table_name = item->text();
    prepare_sqlite_file_viewer(clicked_table_name);
}



void existing_hash_column_selection::prepare_sqlite_file_viewer(QString clicked_table_name)
{
    if(sqlite_file_path.isEmpty())
        return;

    QStringList db_column_name_list;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(sqlite_file_path);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Database open ---FAILED---:" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error :" + destination_db.lastError().text() , Q_FUNC_INFO);
        return ;
    }

    QStringList table_list;
    table_list = destination_db.tables();

    if(table_list.isEmpty())
    {
        destination_db.close();
        return;
    }

    QSqlQuery query_tableName(destination_db);
    QString table_name ;

    bool bool_table_item_clicked = false;
    if(!clicked_table_name.trimmed().isEmpty())
    {
        bool_table_item_clicked = true;
        table_name = clicked_table_name;
    }
    else if(table_list.size() > 0)
    {
        table_name  = table_list.at(0);
    }


    ///-------------prepare query
    QString command = "PRAGMA TABLE_INFO( '" + table_name + "')";
    query_tableName.prepare(command);
    if(!query_tableName.exec())
    {
        recon_static_functions::app_debug("Query execution ---FAILED---:" + query_tableName.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error :" + query_tableName.lastError().text() , Q_FUNC_INFO);
        return ;
    }

    while(query_tableName.next())
    {
        db_column_name_list << query_tableName.value(1).toString();
    }


    table_widget_sqlite_data->setRowCount(0);

    QString text_data;
    QString select_command;

    for(int column_count = 0 ; column_count < db_column_name_list.size() ; column_count++)
    {

        if(column_count % 10 == 0)
            QCoreApplication::processEvents();

        QString column_name = db_column_name_list.at(column_count);

        select_command = "select \"" + column_name + "\" from \"" + table_name + "\"";

        query_tableName.prepare(select_command);
        if(!query_tableName.exec())
        {
            recon_static_functions::app_debug("Query execution ---FAILED---:" + query_tableName.executedQuery(), Q_FUNC_INFO);
            recon_static_functions::app_debug("Error :" + query_tableName.lastError().text() , Q_FUNC_INFO);
            continue ;
        }

        if(query_tableName.next())
        {

            table_widget_sqlite_data->insertRow(column_count);
            table_widget_sqlite_data->setRowHeight(column_count,25);

            ///------------------column key
            radio_button_key_col = new QRadioButton;
            radio_button_key_col->setText(column_name);
            table_widget_sqlite_data->setCellWidget(column_count, 0, radio_button_key_col);
            ///------------------column key



            ///------------------column value
            text_data = query_tableName.value(0).toString();

            QTableWidgetItem *t_items_coloumn_value = new QTableWidgetItem;
            t_items_coloumn_value->setText(text_data);
            table_widget_sqlite_data->setItem(column_count,1,t_items_coloumn_value);
            table_widget_sqlite_data->resizeColumnsToContents();
            ///------------------column value

        }
    }

    table_widget_sqlite_data->setColumnWidth(0,230);
    table_widget_sqlite_data->setColumnWidth(1,400);


    destination_db.close();

}
