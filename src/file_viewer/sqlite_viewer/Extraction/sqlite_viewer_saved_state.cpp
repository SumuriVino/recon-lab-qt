#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "ui_sqlite_viewer.h"


void sqlite_viewer::filter_view_table_saved_state(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    //-1. Create Db
    create_destination_database(destination_db_file);

    //-2. Create Tables in Db
    create_db_tables_for_filter_view_table(destination_db_file);

    //-3. Insert Data in Tables
    insert_dataIn_DB_filter_view_table(destination_db_file);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::data_display_table_saved_state(QString destination_db_file)
{
    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);

    //-1. Create Db
    create_destination_database(destination_db_file);

    //-2. Create Tables in Db
    create_db_tables_for_main_display_data_table(destination_db_file);

    //-3. Insert Data in Tables
    insert_dataIn_DB_main_display_data_table(destination_db_file);

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}

void sqlite_viewer::save_current_state()
{
    QString destination_db_FilterViewTable = saved_state_dir_path + "/" + filter_view_table_sqlite_name(); //FilterViewTable_SavedState.sqlite
    QString destination_db_MainDataDisplay = saved_state_dir_path + "/" + main_datadisplay_table_sqlite_name(); //MainDataDisplay_SavedState.sqlite

    filter_view_table_saved_state(destination_db_FilterViewTable);
    data_display_table_saved_state(destination_db_MainDataDisplay);

}

int sqlite_viewer::create_destination_database(QString destination_db_file)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    bool check = false;
    QFile check_db_exists(destination_db_file);
    check = check_db_exists.exists();

    if(check)
    {
        recon_static_functions::app_debug("DB already exists and now removing existing database...",Q_FUNC_INFO);
        check_db_exists.remove();
    }

    recon_static_functions::app_debug("DB created successfully",Q_FUNC_INFO);
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB -----FAILED-------",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
        return -1;
    }
    else
    {
        destination_db.close();
        recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
        return 1;
    }

}

int sqlite_viewer::create_db_tables_for_filter_view_table(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    db.setDatabaseName(destination_db_file);
    if(!db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED--- for table creation",Q_FUNC_INFO);
        recon_static_functions::app_debug(db.lastError().text(),Q_FUNC_INFO);
        return -1;
    }


    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {


        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);


        QSqlQuery dest_query(db);
        QString command;
        command = "CREATE TABLE '" + struct_obj.table_name +
                "' (RECON_Seq_INT INTEGER,"
                "coloum_name VARCHAR(100),"
                "coloum_view VARCHAR(100),"
                "coloum_search VARCHAR(100),"
                "data_conversion_type VARCHAR(100),"
                "coloum_data_type VARCHAR(100))";

        if(!dest_query.exec(command))
        {
            recon_static_functions::app_debug(" creation table-----FAILED-------",Q_FUNC_INFO);
            recon_static_functions::app_debug(dest_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db_file,Q_FUNC_INFO);
            recon_static_functions::app_debug(command,Q_FUNC_INFO);
        }
    }

    db.close();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return 1;
}

int sqlite_viewer::insert_dataIn_DB_filter_view_table(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    int res = 0;
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file,Q_FUNC_INFO);

        res = -1;
        return res;
    }

    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {


        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);

        QTableWidget *tableWidget = struct_obj.tableWidget_FilterViewSearch_ptr;
        QTableWidget *tableWidget_column_selection = struct_obj.tableWidget_columns_selection_ptr;

        ///- Get the datatypelist from the core db.
        QStringList list_dataType;
        list_dataType = get_db_coloum_datatype(struct_obj.table_name);
        list_dataType.insert(0,"INTEGER"); //- for bookmark
        list_dataType.insert(1,"INTEGER"); //- for INT

        int size = list_dataType.size();
        list_dataType.insert(size, "VARCHAR(500)"); //- for notes

        QSqlQuery dest_query(destination_db);

        for(int row = 0; row < tableWidget->rowCount(); row++)
        {
            if(row % 100 == 0)
                QCoreApplication::processEvents();

            QString INT = QString::number(row + 1);
            QString view;
            if(tableWidget->item(row,0)->checkState())
                view = "1";
            else
                view = "0";

            QString search;
            if(tableWidget->item(row,1)->checkState())
                search = "1";
            else
                search = "0";

            QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget((row) ,2))->currentText());
            QString command = QString("INSERT INTO '" + struct_obj.table_name + "' (RECON_Seq_INT, coloum_name, coloum_view,"
                                                                                "coloum_search, data_conversion_type, "
                                                                                "coloum_data_type) values (?,?,?, ?,?,?)");
            dest_query.prepare(command);
            dest_query.addBindValue(INT);
            dest_query.addBindValue(tableWidget->item(row,0)->text());
            dest_query.addBindValue(view);
            dest_query.addBindValue(search);
            dest_query.addBindValue(recivedComboboxText);

            if(list_dataType.size() == tableWidget->rowCount())
                dest_query.addBindValue(list_dataType.at(row));
            else
                dest_query.addBindValue("VARCHAR(500)");

            if(!dest_query.exec())
            {
                recon_static_functions::app_debug("insert  into FilterViewSearch DB ------FAILED-----",Q_FUNC_INFO);
                recon_static_functions::app_debug(dest_query.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(dest_query.executedQuery(),Q_FUNC_INFO);

                res = -1;
                continue;
            }

            INT.clear(); view.clear(); search.clear();recivedComboboxText.clear();
        }
    }

    destination_db.close();
    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

int sqlite_viewer::create_db_tables_for_main_display_data_table(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    int res = 1;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    db.setDatabaseName(destination_db_file);
    if(!db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(db.lastError().text(),Q_FUNC_INFO);
        return res = -1;
    }



    if(ui->comboBox_dbtables_selection->count() == list_struct_tablewidget_data_sqlite_viewer.size())
    {
    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {

        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);
        m_tablewidget *tableWidget = struct_obj.tableWidget_MainDataDisplay_ptr;


        QStringList listColsDataType = struct_obj.obj_struct_dbtable_detail.list_db_coloum_datatype;
        if(tableWidget->columnCount() != listColsDataType.size())
            continue;

        QSqlQuery dest_query(db);
        QString command = QString("CREATE TABLE '") + struct_obj.table_name + "' ( ";


        QString tmp_entry;
        int size = tableWidget->columnCount();

        QList<custom_table_header_widget *> custom_header_widgets_QList;
        QString current_header_column_name;

        if(base_core_class_obj->tab_custom_header_widgets_QMap.keys().contains(struct_obj.table_name))
        {
            custom_header_widgets_QList.clear();
            custom_header_widgets_QList = base_core_class_obj->tab_custom_header_widgets_QMap.value(struct_obj.table_name);
        }

        for(int col = 0  ;col < tableWidget->columnCount(); col++)
        {
            if(col == 0) //-For first colum name RECON BOOKMARK.
            {
                tmp_entry +=  "RECON_bookmark " + listColsDataType.at(col);
            }
            else if(col == 1)
            {
                tmp_entry +=  "RECON_tag_icon " + listColsDataType.at(col);
            }
            else if(col == (tableWidget->columnCount()-1))
            {
                tmp_entry +=  "RECON_Lastcolumn " + listColsDataType.at(col);
            }
            else
            {

                custom_table_header_widget *header_wdgt_ptr = custom_header_widgets_QList.at(col);
                current_header_column_name = header_wdgt_ptr->pub_get_column_name();
                tmp_entry += current_header_column_name.replace(" ","_") + " " + listColsDataType.at(col);
                //tmp_2 += tableWidget->horizontalHeaderItem(col)->text().replace(" ","_");
            }

            if(col == (size - 1))
            {
                tmp_entry += " ) ";
            }
            else
            {
                tmp_entry += ",";
            }
        }

        command += tmp_entry;

        if(!dest_query.exec(command))
        {
            recon_static_functions::app_debug("Contacts destination table-----FAILED-------",Q_FUNC_INFO);
            recon_static_functions::app_debug(dest_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(command,Q_FUNC_INFO);

            continue;
        }
    }
}
    db.close();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    return res;
}

int sqlite_viewer::insert_dataIn_DB_main_display_data_table(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file,Q_FUNC_INFO);
        return -1;
    }

    destination_db.transaction();


    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {

        QCoreApplication::processEvents();

        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);
        m_tablewidget *m_tableWidget_obj = struct_obj.tableWidget_MainDataDisplay_ptr;

        QSqlQuery query_save_state(destination_db);
        QString table_name = struct_obj.table_name;
        QStringList column_list;

        QList<custom_table_header_widget *> custom_header_widgets_QList;
        QString current_header_column_name;

        if(base_core_class_obj->tab_custom_header_widgets_QMap.keys().contains(table_name))
        {
            custom_header_widgets_QList.clear();
            custom_header_widgets_QList = base_core_class_obj->tab_custom_header_widgets_QMap.value(struct_obj.table_name);
        }

        for(int j = 0; j < m_tableWidget_obj->columnCount(); j++)
        {
            QCoreApplication::processEvents();
            custom_table_header_widget *header_wdgt_ptr = custom_header_widgets_QList.at(j);
            current_header_column_name = header_wdgt_ptr->pub_get_column_name();
            column_list << current_header_column_name.replace(" ", "_");
        }

        if(column_list.size() > 0)
        {
            column_list.removeAt(0);
            column_list.prepend("RECON_bookmark");
        }

        if(column_list.size() > 1)
        {
            column_list.removeAt(1);
            column_list.prepend("RECON_tag_icon");

            column_list.removeAt(column_list.size() - 1);
            column_list.append("RECON_Lastcolumn");
        }

        if(column_list.isEmpty())
            continue;

        for(int row = 0; row < m_tableWidget_obj->rowCount(); row++)
        {
            if(row % 600 == 0)
                QCoreApplication::processEvents();


            QStringList question_str_list;

            QStringList column_values_list;

            for(int cols = 0; cols < m_tableWidget_obj->columnCount(); cols++)
            {
                if(cols % 100 == 0)
                    QCoreApplication::processEvents();

                question_str_list << "?";
                if(cols == 0)
                {
                    if(m_tableWidget_obj->item(row, cols)->checkState())
                        column_values_list << "1";
                    else
                        column_values_list << "0";

                    continue;
                }

                QString ttmp = m_tableWidget_obj->item(row, cols)->text();
                column_values_list << ttmp;
            }


            query_save_state.prepare("INSERT INTO '" + table_name + "' (" + column_list.join(",") + ") " + "VALUES (" + question_str_list.join(",") + ")" );
            for(int cols = 0; cols < m_tableWidget_obj->columnCount(); cols++)
            {
                if(cols % 100 == 0)
                    QCoreApplication::processEvents();

                query_save_state.addBindValue(column_values_list.at(cols));
            }

            if(!query_save_state.exec())
            {
                recon_static_functions::app_debug("query dest_query ------FAILED--------",Q_FUNC_INFO);
                recon_static_functions::app_debug(query_save_state.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(query_save_state.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(destination_db_file,Q_FUNC_INFO);
                continue;
            }
        }
    }



    destination_db.commit();

    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::update_dataIn_DB_main_display_data_table(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error----"+ destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file,Q_FUNC_INFO);
    }

    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);
        m_tablewidget *tableWidget = struct_obj.tableWidget_MainDataDisplay_ptr;

        QString table_name = struct_obj.table_name;

        if(ui->comboBox_dbtables_selection->currentText() != table_name)
        {
            continue;
        }

        qint64 total_rows = tableWidget->rowCount();
        int recon_col = 1;
        for(int row = 0; row < total_rows; row++)
        {
            if(row % 100 == 0)
                QCoreApplication::processEvents();




            QString tag_data = "";
            if(tableWidget->item(row,tableWidget->columnCount() - 3)->text() != "")
            {
                tag_data = tableWidget->item(row,tableWidget->columnCount() - 3)->text();
            }

            int column_data = 0;

            if(tableWidget->item(row,0)->checkState() == Qt::Checked)
            {
                column_data = 1;
                tableWidget->item(row,tableWidget->columnCount() - 3)->setText(tag_data);
            }else
            {
                tableWidget->item(row,tableWidget->columnCount() - 3)->setText(tag_data);

            }

            QString ccmd = QString("UPDATE '") + table_name + QString("' SET RECON_bookmark = ?, RECON_Examiner_Tags = ? WHERE RECON_INT = ?");
            QStringList arg_list;
            arg_list << QString::number(column_data) << tag_data << QString::number(recon_col);

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(ccmd,arg_list,destination_db,Q_FUNC_INFO);
            recon_col++;
        }
    }

    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::update_dataIn_DB_main_display_data_table_single_record(QString destination_db_file)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error----"+ destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file,Q_FUNC_INFO);
        return;
    }

    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);
        m_tablewidget *m_tableWidget_obj = struct_obj.tableWidget_MainDataDisplay_ptr;

        QString table_name = struct_obj.table_name;

        if(ui->comboBox_dbtables_selection->currentText() != table_name)
        {
            continue;
        }

        QModelIndexList selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();
        for(int i = 0; i < selection_model_list.size(); i++)
        {
            if(i % 100 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(i);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tableWidget_obj->rowCount())
                continue;

            if(m_tableWidget_obj->isRowHidden(selected_row_no))
                continue;

            QString record_no = m_tableWidget_obj->item(selected_row_no, 2)->text();

            QString tag_data = "";
            if(m_tableWidget_obj->item(selected_row_no,m_tableWidget_obj->columnCount() - 3)->text() != "")
            {
                tag_data = m_tableWidget_obj->item(selected_row_no,m_tableWidget_obj->columnCount() - 3)->text();
            }

            int column_data = 0;

            if(m_tableWidget_obj->item(selected_row_no,0)->checkState() == Qt::Checked)
            {
                column_data = 1;
                m_tableWidget_obj->item(selected_row_no,m_tableWidget_obj->columnCount() - 3)->setText(tag_data);
            }else
            {
                m_tableWidget_obj->item(selected_row_no,m_tableWidget_obj->columnCount() - 3)->setText(tag_data);

            }

            QString ccmd = QString("UPDATE '") + table_name + QString("' SET RECON_bookmark = ?, RECON_Examiner_Tags = ? WHERE RECON_INT = ?");
            QStringList arg_list;
            arg_list << QString::number(column_data) << tag_data << record_no;


            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(ccmd, arg_list,destination_db,Q_FUNC_INFO);
        }
    }


    destination_db.close();
    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::update_bookmark_value(QString status, int row, int bookmark_col_index)
{

    if(row < 0 || bookmark_col_index < 0)
        return ;



    recon_static_functions::app_debug("-START-",Q_FUNC_INFO);
    int record_no_index = 2;
    int current_display_tab_index = ui->comboBox_dbtables_selection->currentIndex();

    struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(current_display_tab_index);
    m_tablewidget *m_tableWidget_obj = struct_obj.tableWidget_MainDataDisplay_ptr;




    if(bookmark_col_index != -1)
    {
        if(status == QString("1"))
            m_tableWidget_obj->item(row, bookmark_col_index)->setCheckState(Qt::Checked);
        else
            m_tableWidget_obj->item(row, bookmark_col_index)->setCheckState(Qt::Unchecked);
    }

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));


    QStringList col_list = get_data_for_current_record(m_tableWidget_obj);

    QString record_no = m_tableWidget_obj->item(row , record_no_index)->text();

    QString command;
    QStringList arg_list;

    QString destination_db_path = saved_state_dir_path + "/" + main_datadisplay_table_sqlite_name();
    command = QString("UPDATE " + current_tab_name + " SET RECON_Bookmark = ? where RECON_INT = ?");
    arg_list << status << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , arg_list,destination_db_path , Q_FUNC_INFO);

    int column_count = m_tableWidget_obj->columnCount();
    notes_text_on_record = m_tableWidget_obj->item(row,(column_count -2))->text();


    QString sql_tab_name = QString(QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + current_tab_name );

    current_sql_tab = sql_tab_name;

    create_and_insert_exported_content_info_in_db(row ,record_no ,MACRO_Tag_Name_Bookmarks, sqlite_viewer_display_name_in_tree_and_tab, col_list, sql_tab_name , status);


    recon_static_functions::app_debug("-End-",Q_FUNC_INFO);
}
void sqlite_viewer::action_bookmark_triggered()
{

    int current_tab_index = ui->comboBox_dbtables_selection->currentIndex();

    struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    m_tablewidget *m_tableWidget_obj = struct_obj.tableWidget_MainDataDisplay_ptr;

    QModelIndexList selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {

        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= m_tableWidget_obj->rowCount())
            continue;

        if(m_tableWidget_obj->isRowHidden(selected_row_no))
            continue;

        int bookmark_index = 0;
        update_bookmark_value("1",selected_row_no,bookmark_index);
    }


}
void sqlite_viewer::action_remove_bookmark_triggered()
{

    int current_tab_index = ui->comboBox_dbtables_selection->currentIndex();

    struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    m_tablewidget *m_tableWidget_obj = struct_obj.tableWidget_MainDataDisplay_ptr;

    QModelIndexList selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tableWidget_obj->rowCount())
            continue;

        if(m_tableWidget_obj->isRowHidden(selected_row_no))
            continue;

        int bookmark_index = 0;
        update_bookmark_value("0",selected_row_no,bookmark_index);
    }
}

void sqlite_viewer::update_bookmark_value_in_bookmark_serach(QString record_no, QString plugin_name, QString sql_tab_name, QString status, QString tag_name, QString executed_query, QString file_path, QString os_name_str, QString sqliteviewer_db_table_name_of_tag, QString current_table_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    if(status == MACRO_TRUE_VALUE_NUMERIC_QString)
    {
        QStringList arg_list;
        arg_list << plugin_name << sql_tab_name << record_no;
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select INT from tags where plugin_name = ? AND tab_name = ? AND record_no = ?", arg_list, 0, bookmark_db_path, Q_FUNC_INFO);
        if(INT_str.trimmed().isEmpty())
        {
            QFileInfo info(file_path);
            QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                              "os_scheme_display, item0, item1, item2 ,item3,item4 , csv_plugin_name , csv_tab_name , source_count_name , notes) VALUES (?, ?,?,?,?,?,?,?, ?,?,?,? ,?,?,?,?)";

            QStringList arg_list;
            arg_list << plugin_name
                     << plugin_name
                     << sql_tab_name
                     << "sv_content_info"
                     << record_no
                     << tag_name
                     << os_name_str
                     << info.fileName()
                     << file_path
                     << current_table_name
                     << sqliteviewer_db_table_name_of_tag
                     << executed_query.replace(";", "\n")
                     << QString(MACRO_Plugin_Name_Sqlite_Viewer)
                     << QString(MACRO_Plugin_Name_Sqlite_Viewer)
                     << source_count_name
                     << notes_text_on_record;

            global_connection_manager_obj->pub_set_notes_in_tags_search_for_sqlite_viewer(QStringList(record_no),sql_tab_name,current_tab_name,sqlite_viewer_display_name_in_tree_and_tab,notes_text_on_record);

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,bookmark_db_path,Q_FUNC_INFO);
        }
    }
    else if(status == "0")
    {

        QString command = QString("Delete from tags Where record_no=? AND plugin_name=? AND tab_name=?");
        QStringList arg_list;
        arg_list << record_no << plugin_name << sql_tab_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, arg_list, bookmark_db_path, Q_FUNC_INFO);
    }
    else
        return;

    emit signal_common_add_tag_name_into_case_tree(QString(MACRO_Tag_Name_Bookmarks));

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}
void sqlite_viewer::update_tag_in_tag_serach_db(QString file_path, QString tag_name,  QString record_no, QString sqlite_viewer_display_name, QString source_count_name,QString sqliteviewer_db_table_name_of_tag, QString executed_query, QString sql_tab_name)
{
    recon_static_functions::app_debug("- start",Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);
    QFileInfo info(file_path);


    QStringList arg_previous_list;
    arg_previous_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sql_tab_name << record_no << tag_name;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select INT from tags where plugin_name = ? AND tab_name = ? AND record_no = ? AND recon_tag_value = ?  ", arg_previous_list, 0, tag_db_path, Q_FUNC_INFO);
    if(INT_str.isEmpty())
    {
        QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
                          "os_scheme_display,source_count_name, item0, item1, item2,item3,item4,csv_plugin_name,csv_tab_name,notes) VALUES (?,?,?,?,?,?, ?,?,?,?,?, ?,?,?, ?,?)";

        QStringList arg_list;
        arg_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sql_tab_name
                 << "sv_content_info" << record_no << tag_name << QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display) << source_count_name
                 << info.fileName() << file_path << sqliteviewer_db_table_name_of_tag << sqlite_viewer_display_name << executed_query.replace(";", "\n")
                 << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer) << notes_text_on_record;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tag_db_path,Q_FUNC_INFO);
        global_connection_manager_obj->pub_set_notes_in_bookmark_search_for_sqlite_viewer(QStringList(record_no),sql_tab_name,current_tab_name,sqlite_viewer_display_name_in_tree_and_tab,notes_text_on_record);


    }
    emit signal_common_add_tag_name_into_case_tree(tag_name);
    recon_static_functions::app_debug("- end",Q_FUNC_INFO);
}

void sqlite_viewer::refresh_table_widget_data(QTableWidget *m_table_widget, QString table_name)
{
    QString dest_db_file_path = saved_state_dir_path + "/" + main_datadisplay_table_sqlite_name();
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(dest_db_file_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("open destination DB-----FAILED----- " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error---" + destination_db->lastError().text(),Q_FUNC_INFO);
        return;
    }



    QString command = QString("SELECT * FROM '" + table_name + "'");
    QSqlQuery select_query(*destination_db);
    if(!select_query.exec(command))
    {
        recon_static_functions::app_debug("select_query ------FAILED------- " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + select_query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + command,Q_FUNC_INFO);
    }
    struct_global_sqlite_viewer_tablewidget_main obj_main_table;
    obj_main_table.table_name = table_name;

    struct_global_sqlite_viewer_single_dbtable_info obj_single_table;
    QString filter_tbl_db_path = saved_state_dir_path + "/" + filter_view_table_sqlite_name();
    load_saved_state_class_obj->pub_set_db_path_for_refresh(filter_tbl_db_path,dest_db_file_path);

    obj_single_table = load_saved_state_class_obj->load_dbtable_info(table_name,filter_tbl_db_path);
    obj_main_table.obj_struct_dbtable_detail = obj_single_table;

    QStringList column_list = obj_single_table.cols_list;


    m_table_widget->setRowCount(0);


    int row_count = 0;
    while(select_query.next())
    {
        if(row_count%300 == 0)
            QCoreApplication::processEvents();

        m_table_widget->insertRow(row_count);
        m_table_widget->setRowHeight(row_count,22);

        int column_pos = 0;
        while(column_pos != column_list.size())
        {


            QString dataStr_from_db = select_query.value(column_pos).toString().trimmed();

            if(column_pos == 0)
            {
                QTableWidgetItem *item_tablewidget_bookmark;

                item_tablewidget_bookmark = new QTableWidgetItem;
                item_tablewidget_bookmark->setCheckState(Qt::Unchecked);


                if(dataStr_from_db == QString::number(1))
                    item_tablewidget_bookmark->setCheckState(Qt::Checked);
                else
                    item_tablewidget_bookmark->setCheckState(Qt::Unchecked);

                m_table_widget->setItem(row_count, column_pos, item_tablewidget_bookmark);

            }
            else if(column_list.at(column_pos) == "RECON Examiner Tags")
            {

                QTableWidgetItem *item_tablewidget_tags;
                item_tablewidget_tags = new QTableWidgetItem;

                QString icon_path;
                if(dataStr_from_db.trimmed().isEmpty())
                {
                    item_tablewidget_tags->setIcon(QIcon(icon_path));
                }
                else
                {
                    QString previous_tag = dataStr_from_db;
                    QString icon_color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(previous_tag,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                    icon_color = icon_color.toLower();
                    icon_color = icon_color.replace(" ","_");
                    icon_path = "../icons/tag_icons/"+ icon_color +".png";

                    item_tablewidget_tags->setIcon(QIcon(icon_path));
                    m_table_widget->setItem(row_count,enum_coloumn_Sqlite_viewer_Tag_icon, item_tablewidget_tags);

                }
                item_tablewidget_tags->setIcon(QIcon(icon_path));
                m_table_widget->setItem(row_count, enum_coloumn_Sqlite_viewer_Tag_icon, item_tablewidget_tags);

                QTableWidgetItem *item_tag_str = new QTableWidgetItem;
                item_tag_str->setText(dataStr_from_db);
                m_table_widget->setItem(row_count, column_pos, item_tag_str);

            }
            else if(column_list.at(column_pos) == "RECON Examiner Notes")
            {
                QString notes_value_str = select_query.value("RECON_Examiner_Notes").toString().trimmed();

                if(notes_value_str.trimmed().isEmpty())
                    m_table_widget->item(row_count,enum_coloumn_Sqlite_viewer_Bookmark)->setIcon(QIcon(""));
                else
                    m_table_widget->item(row_count,enum_coloumn_Sqlite_viewer_Bookmark)->setIcon(QIcon("../icons/notes.png"));


                QTableWidgetItem *item_w_notes = new QTableWidgetItem;
                item_w_notes->setText(notes_value_str);
                m_table_widget->setItem(row_count, column_pos, item_w_notes);
            }
            else
            {
                QTableWidgetItem *item_tablewidget_others = new QTableWidgetItem;
                item_tablewidget_others->setText(dataStr_from_db);

                m_table_widget->setItem(row_count, column_pos, item_tablewidget_others);
            }

            column_pos++;
        }

        row_count++;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


}

bool sqlite_viewer::find_tagging_on_record(int current_row)
{
    bool bool_have_any_tag_on_record = false;

    if(current_sql_tab.contains(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB))
    {

        struct_global_sqlite_viewer_tablewidget_main obj_main;
        if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
            obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
        else
        {
            recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
            return bool_have_any_tag_on_record;
        }

        m_tablewidget *tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;
        int column_count = tableWidget->columnCount();
        //    int current_row = tableWidget->currentRow();
        //    if(current_row < 0)
        //        return bool_have_any_tag_on_record;

        QString tag_name = tableWidget->item(current_row  ,column_count - 3)->text();
        QString notes_text = tableWidget->item(current_row ,column_count - 2)->text();
        QString bookmark_status;
        if(tableWidget->item(current_row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Checked)
            bookmark_status = "1";
        else
            bookmark_status = "0";


        recon_bookmark_name = bookmark_status;
        recon_notes_name = notes_text;
        recon_tag_name = tag_name;


        if(bookmark_status.trimmed()  == "1")
            bool_have_any_tag_on_record = true;
        else if(!tag_name.trimmed().isEmpty())
            bool_have_any_tag_on_record = true;
        else if(!notes_text.trimmed().isEmpty())
            bool_have_any_tag_on_record = true;
        else
            bool_have_any_tag_on_record = false;

    }
    else if(current_sql_tab.contains(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB))
    {
        recon_bookmark_name = "0";
        recon_notes_name = "";
        recon_tag_name = tag_name_for_execute_tab;

        bool_have_any_tag_on_record = true;
    }

    return bool_have_any_tag_on_record;
}

QStringList sqlite_viewer::get_data_for_current_record(QTableWidget *m_table_widget)
{
    QStringList data_list;
    int selected_row_no = m_table_widget->currentRow();
    if(selected_row_no < 0)
        return data_list;

    struct_global_sqlite_viewer_tablewidget_main obj_main;
    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return data_list;
    }

    m_tablewidget *tablewidget_main_data_display;
    tablewidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

    QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

    for(int col = 0; col < m_table_widget->columnCount() ; col++ )
    {
        QTableWidgetItem *itemm = m_table_widget->horizontalHeaderItem(col);

        QString header = itemm->text();

        QString data_str = m_table_widget->item(selected_row_no, col)->text();

        if(header == "RECON INT" || header.isEmpty() || header == "RECON Examiner Tags" || header == "RECON Examiner Notes")
            continue;

        if(m_table_widget->isColumnHidden(col))
            continue ;

        //data_list << QString(header + QString(": ") + data_str);
        QString convertedData;
        QString recivedComboboxText;

        recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col) ,2))->currentText());

        if(recivedComboboxText != "Select Conversion")
        {
            convertedData =  convert_data(data_str,recivedComboboxText);

            if(recivedComboboxText != "File")
            {
                data_list << header + " (Original Value)"  + ": " + data_str;
                data_list << header + " (Converted Value)" + ": " + convertedData;
            }
            else
            {
                data_list << header + ": "+ convertedData;
            }
        }
        else
        {
            data_list << QString(header + QString(": ") + data_str);

        }

    }

    return data_list;
}

bool sqlite_viewer::check_current_column_selected_for_conversion_in_manage_column(QString column_str)
{

    struct_global_sqlite_viewer_tablewidget_main obj_main;


    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return false;
    }

    m_tablewidget *m_table_widget = obj_main.tableWidget_MainDataDisplay_ptr;

    QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

    for(int col = 0; col < m_table_widget->columnCount() ; col++ )
    {
        QTableWidgetItem *itemm = m_table_widget->horizontalHeaderItem(col);

        QString header = itemm->text();


        if(header == "RECON INT" || header.isEmpty() || header == "RECON Examiner Tags" || header == "RECON Examiner Notes")
            continue;

        if(m_table_widget->isColumnHidden(col))
            continue ;


        if(header != column_str)
            continue;


        QString convertedData;
        QString recivedComboboxText;

        recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col) ,2))->currentText());


        if(recivedComboboxText != "Select Conversion")
        {
            return true;
        }
    }

    return false;
}

bool sqlite_viewer::check_current_column_selected_for_conversion_in_manage_column_for_file(QString column_str)
{

    struct_global_sqlite_viewer_tablewidget_main obj_main;
    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return false;
    }

    m_tablewidget *m_table_widget = obj_main.tableWidget_MainDataDisplay_ptr;

    QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

    for(int col = 0; col < m_table_widget->columnCount() ; col++ )
    {
        QTableWidgetItem *itemm = m_table_widget->horizontalHeaderItem(col);

        QString header = itemm->text();


        if(header == "RECON INT" || header.isEmpty() || header == "RECON Examiner Tags" || header == "RECON Examiner Notes")
            continue;

        if(m_table_widget->isColumnHidden(col))
            continue ;

        if(header != column_str)
            continue;


        QString convertedData;
        QString recivedComboboxText;

        recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col) ,2))->currentText());

        if(recivedComboboxText != "Select Conversion")
        {

            if(recivedComboboxText != "File")
            {
                return true;
            }
        }
    }

    return false;
}

int sqlite_viewer::get_current_column_selected_for_conversion_in_manage_column_index(QString column_str)
{

    struct_global_sqlite_viewer_tablewidget_main obj_main;
    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return -1;
    }

    m_tablewidget *m_table_widget = obj_main.tableWidget_MainDataDisplay_ptr;

    QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

    for(int col = 0; col < m_table_widget->columnCount() ; col++ )
    {
        QTableWidgetItem *itemm = m_table_widget->horizontalHeaderItem(col);

        QString header = itemm->text();


        if(header == "RECON INT" || header.isEmpty() || header == "RECON Examiner Tags" || header == "RECON Examiner Notes")
            continue;

        if(m_table_widget->isColumnHidden(col))
            continue ;

        if(header != column_str)
            continue;


        QString convertedData;
        QString recivedComboboxText;

        recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((col) ,2))->currentText());

        if(recivedComboboxText != "Select Conversion")
        {

            if(recivedComboboxText != "File")
            {
                return col;
            }
        }
    }

    return -1;
}


void sqlite_viewer::set_source_file_essential()
{

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recieved_sqlite_path_from_mainwindow);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString command;
    command = "SELECT file_path , recon_filefrom , recon_file_infopath ,INT FROM sv_index WHERE viewer_display_name = ?";
    QStringList values;
    values << sqlite_viewer_display_name_in_tree_and_tab;

    actual_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,values, 0,destination_db, Q_FUNC_INFO);
    recon_file_from = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,values, 1,destination_db, Q_FUNC_INFO);
    recon_file_info_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,values, 2,destination_db, Q_FUNC_INFO);
    QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,values, 3,destination_db, Q_FUNC_INFO);
    complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no ,destination_db,"sv_index",source_count_name,Q_FUNC_INFO);

    destination_db.close();
}

void sqlite_viewer::notes_update_data_In_DB_main_display_data_table(QString destination_db_file, int input_type)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    for(int i = 0 ; i < ui->comboBox_dbtables_selection->count(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(i);
        m_tablewidget *tableWidget = struct_obj.tableWidget_MainDataDisplay_ptr;

        QString table_name = struct_obj.table_name;

        if(ui->comboBox_dbtables_selection->currentText() != table_name)
        {
            continue;
        }

        if(input_type == 0)
        {
            QModelIndexList selection_model_list = tableWidget->selectionModel()->selectedRows();

            if(selection_model_list.size() > 1)
            {
                for(int i = 0; i < selection_model_list.size(); i++)
                {
                    if(i % 100 == 0)
                        QCoreApplication::processEvents();


                    QModelIndex model_index = selection_model_list.at(i);

                    int selected_row_no = model_index.row();

                    if(selected_row_no < 0 && selected_row_no >= tableWidget->rowCount())
                        continue;

                    if(tableWidget->isRowHidden(selected_row_no))
                        continue;

                    int column_count = tableWidget->columnCount();

                    QString data = tableWidget->item(selected_row_no,(column_count - 2))->text();

                    selected_row_no +=1;
                    QString ccmd = QString("UPDATE '") + table_name + QString("' SET RECON_Examiner_Notes = ? WHERE RECON_INT = ?");
                    QStringList values;
                    values << data << QString::number(selected_row_no);
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(ccmd,values,destination_db_file,Q_FUNC_INFO);
                }
            }
        }
        else if(input_type == 1)
        {
            int column_count = tableWidget->columnCount();
            for(int row = 0; row < tableWidget->rowCount(); row++)
            {
                if(row % 100 == 0)
                    QCoreApplication::processEvents();


                if(tableWidget->item(row,0)->checkState())
                {
                    int tmp_var = row;
                    tmp_var +=1;

                    QString data = tableWidget->item(row,(column_count - 2))->text();
                    QString index_str = QString::number(tmp_var);

                    QString ccmd = QString("UPDATE '") + table_name + QString("' SET RECON_Examiner_Notes = ? WHERE RECON_INT = ?");
                    QStringList values;
                    values << data << index_str;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(ccmd,values,destination_db_file,Q_FUNC_INFO);
                }
            }
        }
        else if(input_type == 3)
        {
            int column_count = tableWidget->columnCount();
            for(int row = 0; row < tableWidget->rowCount(); row++)
            {

                if(row % 100 == 0)
                    QCoreApplication::processEvents();


                int tmp_var = row;
                tmp_var += 1;

                QString data = tableWidget->item(row,(column_count - 2))->text();

                QString ccmd = QString("UPDATE '") + table_name + QString("' SET RECON_Examiner_Notes = ? WHERE RECON_INT = ?");
                QStringList values;
                values << data << QString::number(tmp_var);

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(ccmd,values,destination_db_file,Q_FUNC_INFO);
            }
        }
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

QStringList sqlite_viewer::get_db_coloum_datatype(QString table_name)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);
    if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
        saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());


    QStringList dataTypeList;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(saved_execute_query_maindb_path);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return dataTypeList;
    }

    QString dbCreateStr;
    QSqlRecord dbRecord;

    QSqlQuery queryRead(destination_db);
    queryRead.prepare("SELECT * FROM '" + table_name + "'");

    if(!queryRead.exec())
    {
        recon_static_functions::app_debug("queryRead ---FAILED---" + saved_execute_query_maindb_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(queryRead.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(queryRead.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return dataTypeList;
    }

    dbRecord = queryRead.record();
    QString typeName;

    for(int i = 0; i < dbRecord.count() ; i++)
    {
        QSqlField dbField = dbRecord.field(i);

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

    destination_db.close();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
    return dataTypeList;
}

QStringList sqlite_viewer::get_coloum_datatype_from_loaded_result(QString table_name, QString db_path)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QStringList dataTypeList;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening ------FAILED-------",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return dataTypeList;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT coloum_data_type FROM '" + table_name + "'");
    if(!query.exec())
    {
        recon_static_functions::app_debug("query ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return dataTypeList;
    }

    while(query.next())
    {
        dataTypeList << query.value(0).toString();
    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

    destination_db.close();
    return dataTypeList;
}

QStringList sqlite_viewer::headers_check_preview_enable(struct_global_sqlite_viewer_tablewidget_main *objectMain)
{
    QStringList tableHeader;
    tableHeader  << "Sr No." << "Table Details";

    QTableWidget *tableWidget = objectMain->tableWidget_FilterViewSearch_ptr;

    for(int i = 0; i < tableWidget->rowCount(); i++)
    {
        QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget->cellWidget(i ,2))->currentText());
        if(recivedComboboxText != "Select Conversion")
        {
            if(recivedComboboxText == "File")
            {
                tableHeader <<"File Preview";
                break;
            }
        }
    }

    return tableHeader;
}


