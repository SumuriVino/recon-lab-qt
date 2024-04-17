#include "file_viewer/sqlite_viewer/sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::set_execute_SQL_table_clear_and_widgets_enable(bool status)
{
    while(ui->tableWidget_query_execution->rowCount())
        ui->tableWidget_query_execution->removeRow(0);

    while(ui->tableWidget_query_execution->columnCount())
        ui->tableWidget_query_execution->removeColumn(0);

    ui->comboBox_report_type_selection_execute_query->setEnabled(status);
    ui->pushButton_generate_report_execute_query->setEnabled(status);

    ui->pushButton_execute_query->setEnabled(status);
}

void sqlite_viewer::on_pushButton_execute_query_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QFont m_font = font();
    m_font.setPointSize(12);

    QString stylesheet = QString(
                "QTableView::item:selected:!active {"
                "color:white;"
                "background-color:  #a1c9f7;"

                "}");

    ui->tableWidget_query_execution->setStyleSheet(stylesheet);
    ui->tableWidget_query_execution->setAlternatingRowColors(true);
    ui->tableWidget_query_execution->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_query_execution->setGridStyle(Qt::NoPen);
    ui->tableWidget_query_execution->setShowGrid(false);
    ui->tableWidget_query_execution->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_query_execution->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget_query_execution->horizontalHeader()->setSectionsMovable(true);
    ui->tableWidget_query_execution->setAutoScroll(true);
    ui->tableWidget_query_execution->setFont(m_font);

    /////installl
    ui->tableWidget_query_execution->installEventFilter(this);
    ui->tableWidget_query_execution->verticalHeader()->setFixedWidth(50);

    if(ui->textEdit_query_insert->toPlainText() == QString(""))
    {
        ui->label_message->clear();
        set_execute_SQL_table_clear_and_widgets_enable(false);
        return;
    }


    QString command1;
    command1 = "SELECT recreated_file_path FROM sv_index WHERE viewer_display_name = ?";
    QStringList values;
    values << sqlite_viewer_display_name_in_tree_and_tab;

    saved_execute_query_maindb_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command1,values, 0,recieved_sqlite_path_from_mainwindow, Q_FUNC_INFO);
    if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
        saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());


    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(saved_execute_query_maindb_path);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(saved_execute_query_maindb_path , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    QString command = QString(ui->textEdit_query_insert->toPlainText());


    if(command.contains("RECON_bookmark") ||command.contains("RECON_INT") || command.contains("RECON_Examiner_Notes"))
    {
        QPalette palette = ui->label_message->palette();
        palette.setColor(ui->label_message->foregroundRole(),Qt::red);
        QString error;
        error =  "Error: No such column start with RECON_";

        ui->label_message->setText("<b>" +error + "</b>");
        ui->label_message->setPalette(palette);

        set_execute_SQL_table_clear_and_widgets_enable(false);

        destination_db.close();
        return;
    }


    if(!query.exec(command))
    {
        recon_static_functions::app_debug("select query ----FAILED-----" + command,Q_FUNC_INFO);
        recon_static_functions::app_debug("Db path ----FAILED-----" + saved_execute_query_maindb_path,Q_FUNC_INFO);


        QPalette palette = ui->label_message->palette();
        palette.setColor(ui->label_message->foregroundRole(),Qt::red);
        QString error;
        error =  "Error: " + query.lastError().text() ;

        ui->label_message->setText("<b>" +error + "</b>");
        ui->label_message->setPalette(palette);

        set_execute_SQL_table_clear_and_widgets_enable(false);
    }
    else
    {

        coloum_list_query_execution_table.clear();

        QPalette palette = ui->label_message->palette();
        palette.setColor(ui->label_message->foregroundRole(),"#00b300");  //#00b300,#009933,#006600
        ui->label_message->setText("<b> Query Successful</b>");
        ui->label_message->setPalette(palette);

        while(ui->tableWidget_query_execution->rowCount())
            ui->tableWidget_query_execution->removeRow(0);

        while(ui->tableWidget_query_execution->columnCount())
            ui->tableWidget_query_execution->removeColumn(0);



        int coloum_count = 1;
        for(int i = 0 ; i < query.record().count() ; i++)
        {
            if(query.record().fieldName(i) == "RECON_bookmark" || query.record().fieldName(i) == "RECON_INT" || query.record().fieldName(i) == "RECON_Examiner_Notes")
                continue;

            coloum_count = coloum_count + 1;
        }

        //coloum_count = coloum_count + 2;
        //- To set Coloum header
        ui->tableWidget_query_execution->setColumnCount(coloum_count);


        QTableWidgetItem *header_tbl_item_col_tags = new QTableWidgetItem;
        header_tbl_item_col_tags->setText("");
        ui->tableWidget_query_execution->setHorizontalHeaderItem(0,header_tbl_item_col_tags);
        coloum_list_query_execution_table << "";


        int count_header = 0;
        for(int kk = 0 ; kk < query.record().count() ; kk++)
        {
            if(query.record().fieldName(kk) == "RECON_bookmark" || query.record().fieldName(kk) == "RECON_INT" || query.record().fieldName(kk) == "RECON_Examiner_Notes")
                continue;

            count_header++;


            QTableWidgetItem *tbl_header_item = new QTableWidgetItem;
            tbl_header_item->setText(query.record().fieldName(kk));
            ui->tableWidget_query_execution->setHorizontalHeaderItem(count_header,tbl_header_item);
            coloum_list_query_execution_table << query.record().fieldName(kk);

        }

        bool_cancel_loading = false;
        display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_obj->show();

        //-3. For the entry of records.
        QTableWidgetItem *item_tablewidget[coloum_count];
        int count_row = 0;
        qint64 total_row = query.size();
        while(query.next())
        {
            if(bool_cancel_loading)
            {
                bool_cancel_loading = false;
                break;
            }

            ui->tableWidget_query_execution->insertRow(count_row);

            display_loading_progress_bar_obj->pub_set_label_messsge("Executing Query ..." + QString::number(count_row));
            display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(count_row);


            int count_cols = 0;
            if(count_row % 100 == 0)
                QCoreApplication::processEvents();




            item_tablewidget[count_cols] = new QTableWidgetItem;
            item_tablewidget[count_cols]->setText("");
            ui->tableWidget_query_execution->setItem(count_row, count_cols, item_tablewidget[count_cols]);
            ui->tableWidget_query_execution->setColumnWidth(count_cols,50);


            for(int j = 0; j < query.record().count() ; j++)
            {
                if(query.record().fieldName(j) == "RECON_bookmark" || query.record().fieldName(j) == "RECON_INT" || query.record().fieldName(j) == "RECON_Examiner_Notes")
                    continue;

                count_cols++;


                item_tablewidget[count_cols] = new QTableWidgetItem;
                item_tablewidget[count_cols]->setText(query.value(j).toString());

                ui->tableWidget_query_execution->setItem(count_row, count_cols, item_tablewidget[count_cols]);
                ui->tableWidget_query_execution->setColumnWidth(count_cols,100);


            }

            count_row++;
        }

        display_loading_progress_bar_obj->hide();

        ui->comboBox_report_type_selection_execute_query->setEnabled(true);
        ui->pushButton_generate_report_execute_query->setEnabled(true);
    }


    destination_db.close();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void sqlite_viewer::on_textEdit_query_insert_textChanged()
{
    if(ui->textEdit_query_insert->toPlainText().trimmed() == "")
    {
        ui->label_message->clear();
        set_execute_SQL_table_clear_and_widgets_enable(false);
        return;
    }

    ui->pushButton_execute_query->setEnabled(true);
    ui->label_message->clear();
}

void sqlite_viewer::metadata_popup_window(QString text)
{
    QDialog *popup = new QDialog(this);
    popup->setAttribute(Qt::WA_DeleteOnClose, true);
    popup->setGeometry(400,400,400,500);
    popup->setWindowTitle(QString(MACRO_Plugin_Name_Sqlite_Viewer)+ " " + QObject::tr("-[Detailed Information]"));

    QTextEdit *text_edit = new QTextEdit(popup);
    QHBoxLayout *box = new QHBoxLayout(popup);

    box->addWidget(text_edit);
    popup->setLayout(box);

    QPalette p = text_edit->palette();
    text_edit->setHtml(text);

    p.setColor(QPalette::Base, QColor(241, 241, 241));
    text_edit->setPalette(p);
    text_edit->setReadOnly(true);
    popup->show();
}

void sqlite_viewer::on_tableWidget_query_execution_cellDoubleClicked(int row, int column)
{
    return;


    //---------
    QStringList text_data;

    for(int i = 0; i < ui->tableWidget_query_execution->columnCount(); i++)
    {
        if(ui->tableWidget_query_execution->isColumnHidden(i))
            continue;

        QString header;
        header = ui->tableWidget_query_execution->horizontalHeaderItem(i)->text();

        QString data;
        data = ui->tableWidget_query_execution->item(row,i)->text();

        if(!header.isEmpty())
            text_data << QString("<b>" + header + ":</b>" +QString("\t")+ data +"\n" );
    }

    QString tmp_text;
    tmp_text = text_data.join("<br><br>");

    metadata_popup_window(tmp_text);

}



