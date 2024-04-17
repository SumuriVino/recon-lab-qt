#include "recon_configuration_page_global_timezone.h"
#include "ui_recon_configuration_page_global_timezone.h"

recon_configuration_page_global_timezone::recon_configuration_page_global_timezone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_global_timezone)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->comboBox_date_format->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->comboBox_timezones->setAttribute(Qt::WA_MacShowFocusRect,false);


    ui->tableWidget_timezone->hideColumn(enum_end_date_numeric_column);
    ui->tableWidget_timezone->hideColumn(enum_start_date_numeric_column);
    ui->tableWidget_timezone->hideColumn(enum_INT_column);
    ui->tableWidget_timezone->setColumnWidth(enum_time_zone_str_column,320);
    ui->tableWidget_timezone->setColumnWidth(enum_start_date_readable_column,180);
    ui->tableWidget_timezone->setColumnWidth(enum_end_date_readable_column,180);

    ui->tableWidget_timezone->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_timezone->setGridStyle(Qt::NoPen);
    ui->tableWidget_timezone->setShowGrid(false);
    ui->tableWidget_timezone->setSelectionMode(QAbstractItemView::SingleSelection);

    qdialog_set_global_timezone = new QDialog(this);
    qdialog_set_global_timezone->setWindowModality(Qt::WindowModal);

    qdialog_set_global_timezone->setFixedSize(386 , 157);

    set_global_timezone_obj = new set_global_timezone(qdialog_set_global_timezone);

    message_dialog_obj = new message_dialog;


    connect(set_global_timezone_obj, SIGNAL(signal_set_time_zone(qint64 , qint64, QString, QString, QString)), this, SLOT(slot_set_time_zone(qint64 , qint64, QString, QString, QString)));
    connect(set_global_timezone_obj, SIGNAL(signal_on_pushbutton_cancel_clicked()), this, SLOT(slot_on_pushbutton_cancel_clicked()));
    connect(set_global_timezone_obj, SIGNAL(signal_edit_time_zone(qint64 , qint64, QString, QString, QString)), this, SLOT(slot_edit_time_zone(qint64 , qint64, QString, QString, QString)));
}


void recon_configuration_page_global_timezone::pub_set_caller_type(QString caller_type)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    caller_type_str = caller_type;

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");
    else
        return ;

    set_essential_data_lists(true);
    set_essential_variables_for_default_timezones();
    set_default_time_zone(true);
    display_data();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::display_data()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QList <struct_global_added_timezone_details> qlist_struct_timezone;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command  = "select start_date_epoch,end_date_epoch,start_date_str, end_date_str, timezone , INT ,  timezone_offset , display_time from tbl_saved_timezones";
    QSqlQuery select_qry(destination_db);
    select_qry.prepare(command);
    if(!select_qry.exec())
    {
        recon_static_functions::app_debug(" Select Query --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_qry.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }


    ui->tableWidget_timezone->setRowCount(0);

    qint64 row = 0;
    while(select_qry.next())
    {
        ui->tableWidget_timezone->insertRow(row);
        ui->tableWidget_timezone->setRowHeight(row, 23);



        QString t_start_date_str = select_qry.value("start_date_str").toString();
        QString t_end_date_str = select_qry.value("end_date_str").toString();
        QString t_start_date_epch = select_qry.value("start_date_epoch").toString();
        QString t_end_date_epch = select_qry.value("end_date_epoch").toString();
        QString t_timezone = select_qry.value("timezone").toString();
        QString t_offset   = select_qry.value("timezone_offset").toString();
        QString t_display_time   = select_qry.value("display_time").toString();



        QTableWidgetItem *start_date_item = new QTableWidgetItem;
        start_date_item->setText(t_start_date_str);
        ui->tableWidget_timezone->setItem(row,enum_start_date_readable_column,start_date_item);

        QTableWidgetItem *end_date_item = new QTableWidgetItem;
        end_date_item->setText(t_end_date_str);
        ui->tableWidget_timezone->setItem(row,enum_end_date_readable_column,end_date_item);


        QTableWidgetItem *start_epch_item = new QTableWidgetItem;
        start_epch_item->setText(t_start_date_epch);
        ui->tableWidget_timezone->setItem(row,enum_start_date_numeric_column,start_epch_item);


        QTableWidgetItem *end_epch_item = new QTableWidgetItem;
        end_epch_item->setText(t_end_date_epch);
        ui->tableWidget_timezone->setItem(row,enum_end_date_numeric_column,end_epch_item);


        QTableWidgetItem *timezone_item = new QTableWidgetItem;
        timezone_item->setText(t_timezone);
        ui->tableWidget_timezone->setItem(row,enum_time_zone_str_column,timezone_item);


        QTableWidgetItem *INT_item = new QTableWidgetItem;
        INT_item->setText(select_qry.value("INT").toString());
        ui->tableWidget_timezone->setItem(row,enum_INT_column,INT_item);


        struct_global_added_timezone_details obj;
        obj.offset = t_offset;
        obj.end_date_numeric_qint64 = t_end_date_epch.toLongLong();
        obj.end_date_str = t_end_date_str;
        obj.start_date_numeric_qint64 = t_start_date_epch.toLongLong();
        obj.start_date_str = t_start_date_str;
        obj.timezone = t_timezone;

        if(t_display_time.contains("GMT") || t_display_time.contains("GMT-GMT"))
        {
            QStringList tm_list;
            if(t_display_time.contains("GMT-GMT"))
                tm_list = t_display_time.split("GMT-GMT");
            else
                tm_list = t_display_time.split("GMT");

            if(tm_list.size() > 1)
                t_display_time = tm_list.at(1);
        }

        obj.display_timezone_offset_name = t_display_time;
        qlist_struct_timezone << obj;

        row++;
    }
    destination_db.close();


    global_recon_helper_singular_class_obj->pub_set_added_timezone_details(qlist_struct_timezone);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void recon_configuration_page_global_timezone::set_default_time_zone(bool bool_set_label_value)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString cmd = ("select case_created_machine_timezone_qstring,"
                   "case_created_machine_timezone_qint64,"
                   "examiner_selected_timezone_qstring,"
                   "examiner_selected_timezone_qint64,"
                   "examiner_selected_date_format_int,"
                   "examiner_selected_date_format_qstring from timezone_details");


    QSqlQuery select_qry(destination_db);
    select_qry.prepare(cmd);
    if(!select_qry.exec())
    {
        recon_static_functions::app_debug(" Select Query --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_qry.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(select_qry.next())
    {
        QString casemachine_timezone_qstring = select_qry.value("case_created_machine_timezone_qstring").toString();
        QString casemachine_timezone_qint64 = select_qry.value("case_created_machine_timezone_qint64").toString();
        QString examiner_timezone_qstring = select_qry.value("examiner_selected_timezone_qstring").toString();
        QString examiner_timezone_qint64 = select_qry.value("examiner_selected_timezone_qint64").toString();
        QString dateformat_qint64 = select_qry.value("examiner_selected_date_format_int").toString();
        QString dateformat_qstring = select_qry.value("examiner_selected_date_format_qstring").toString();


        struct_global_default_timezone_details obj;
        obj.d_case_created_machine_timezn_readable_QString = casemachine_timezone_qstring;
        obj.d_examiner_selected_date_format_readable_QString = dateformat_qstring + " " + "hh:mm:ss";
        obj.d_examiner_selected_timezone_readable_QString = examiner_timezone_qstring;
        obj.d_case_created_machine_timezn_numeric_qint64 = casemachine_timezone_qint64.toLongLong();
        obj.d_examiner_selected_date_format_numeric_qint64 = dateformat_qint64.toInt();
        obj.d_examiner_selected_timezone_numeric_qint64 = examiner_timezone_qint64.toLongLong();

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64,examiner_timezone_qint64);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString,examiner_timezone_qstring);


        current_timezone_offset = 0;
        for(int i = 0 ; i < timezone_qlist.size() ;i++)
        {
            if(timezone_qlist.at(i) == examiner_timezone_qstring)
            {
                current_timezone_offset = offset_qlist.at(i).toLongLong();
                display_time = display_time_name_qlist.at(i);
                break ;
            }
        }

        if(display_time.trimmed().isEmpty())
            obj.timezone_offset_name = "+00:00";
        else
            obj.timezone_offset_name = display_time;

        global_recon_helper_singular_class_obj->pub_set_default_timezone(obj);


    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void recon_configuration_page_global_timezone::set_essential_variables_for_default_timezones()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    current_timezone_offset = 0;
    QString selected_timezone = ui->comboBox_timezones->currentText();
    if(selected_timezone == "GMT+00:00")
    {
        selected_timezone = "GMT-GMT-GMT+0:00";
    }

    for(int ii = 0 ; ii < timezone_qlist.size() ;ii++)
    {
        if(timezone_qlist.at(ii) == selected_timezone)
        {
            current_timezone_offset = offset_qlist.at(ii).toLongLong();
            display_time = display_time_name_qlist.at(ii);
            break ;
        }
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::set_essential_data_lists(bool bool_add_items_combobox)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    ui->comboBox_timezones->blockSignals(true);
    if(bool_add_items_combobox)
        ui->comboBox_timezones->clear();
    ui->comboBox_timezones->blockSignals(false);

    timezone_qlist.clear();
    display_time_name_qlist.clear();
    offset_qlist.clear();

    //    ui->comboBox_timezones->addItem("<Select a Time Zone>");


    QStringList combobox_list_items;
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName("../Resources/timezone.sqlite");
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery select_qry(destination_db);
    select_qry.prepare("select distinct name , code ,timezone ,off_seconds  from timezone");
    if(!select_qry.exec())
    {
        recon_static_functions::app_debug(" Select Query --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_qry.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }


    qint64 counter = 0;
    while(select_qry.next())
    {

        if(counter%100 == 0)
            QCoreApplication::processEvents();

        QString country_name = select_qry.value("name").toString();
        QString time = select_qry.value("code").toString();
        QString country_timezone = select_qry.value("timezone").toString();
        QString time_zone_text = country_name + "-" + time + "-" + country_timezone;


        combobox_list_items << time_zone_text;

        QString tm_str = country_timezone;

        bool bool_is_negative_offset = false;
        if(tm_str.contains("GMT"))
        {
            QStringList tm_list =  tm_str.split("GMT");
            if(tm_list.size() > 1)
            {
                if(tm_str.contains("-"))
                {
                    bool_is_negative_offset = true;
                }
                //                else
                //                {
                //                    tm_str = tm_list.at(1);
                //                }

                tm_str = tm_list.at(1);

            }
        }

        display_time_name_qlist << tm_str;

        if(bool_is_negative_offset)
        {
            offset_qlist << select_qry.value("off_seconds").toString().prepend("-");
        }
        else
        {
            offset_qlist << select_qry.value("off_seconds").toString();
        }

        timezone_qlist << time_zone_text;
        counter++;
    }

    destination_db.close();

    set_combobox_items(combobox_list_items ,bool_add_items_combobox);


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::set_essential_variables_for_added_timezones(QString tm_zone)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    current_timezone_offset = 0;
    for(int i = 0 ; i < timezone_qlist.size() ;i++)
    {
        if(timezone_qlist.at(i) == tm_zone)
        {
            current_timezone_offset = offset_qlist.at(i).toLongLong();
            display_time = display_time_name_qlist.at(i);
            break ;
        }
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

bool recon_configuration_page_global_timezone::is_timezone_already_added(qint64 start_date_epch, qint64 end_date_epch)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);
    qint64 current_start_epch_value = start_date_epch;
    qint64 current_end_epch_value = end_date_epch;

    QString commd = "select start_date_epoch , end_date_epoch from tbl_saved_timezones";
    QStringList start_time_epch_list,end_time_offset_list;
    start_time_epch_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(commd,0,destination_db_file,Q_FUNC_INFO);
    end_time_offset_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(commd,1,destination_db_file,Q_FUNC_INFO);


    for(int ii = 0 ; ii < start_time_epch_list.size() ;ii++)
    {
        QString t_start_str = start_time_epch_list.at(ii);
        qint64 saved_start_value = t_start_str.toLongLong();

        QString t_end_str = end_time_offset_list.at(ii);
        qint64 saved_end_value = t_end_str.toLongLong();


        message_dialog_obj->pub_set_message(MACRO_MSG_WARNING , QString("This Timezone overlaps with existing timezones. Please use a different timezone."));

        if((saved_start_value <= current_start_epch_value) && (saved_start_value >= current_start_epch_value))
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
        else if((saved_end_value <= current_end_epch_value) && (saved_end_value >= current_end_epch_value))
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
        else if(saved_end_value == current_start_epch_value)
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

    return false;
}

bool recon_configuration_page_global_timezone::is_timezone_already_added_for_edit(qint64 start_date_epch , qint64 end_date_epch , QString INT_str)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);
    qint64 current_start_epch_value = start_date_epch;
    qint64 current_end_epch_value = end_date_epch;

    QString commd = "select start_date_epoch , end_date_epoch from tbl_saved_timezones where INT != ?";
    QStringList start_time_epch_list,end_time_offset_list;
    start_time_epch_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(commd,QStringList(INT_str),0,destination_db_file,Q_FUNC_INFO);
    end_time_offset_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(commd,QStringList(INT_str),1,destination_db_file,Q_FUNC_INFO);

    for(int ii = 0 ; ii < start_time_epch_list.size() ;ii++)
    {
        QString t_start_str = start_time_epch_list.at(ii);
        qint64 saved_start_value = t_start_str.toLongLong();

        QString t_end_str = end_time_offset_list.at(ii);
        qint64 saved_end_value = t_end_str.toLongLong();


        message_dialog_obj->pub_set_message(MACRO_MSG_WARNING , QString("This Timezone overlaps with existing timezones. Please use a different timezone."));

        if((saved_start_value <= current_start_epch_value) && (saved_start_value >= current_start_epch_value))
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
        else if((saved_end_value <= current_end_epch_value) && (saved_end_value >= current_end_epch_value))
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
        else if(saved_end_value == current_start_epch_value)
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
        else if(saved_start_value == current_end_epch_value)
        {
            message_dialog_obj->show();
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
            return true;
        }
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
    return false;
}

recon_configuration_page_global_timezone::~recon_configuration_page_global_timezone()
{
    delete ui;
}

void recon_configuration_page_global_timezone::on_pushButton_add_timezone_clicked()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    qdialog_set_global_timezone->show();
    set_global_timezone_obj->pub_set_timezone(timezone_qlist);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::slot_on_pushbutton_cancel_clicked()
{
    qdialog_set_global_timezone->hide();
}

void recon_configuration_page_global_timezone::slot_set_time_zone(qint64 start_date_epch , qint64 end_date_epch, QString timezone , QString start_date_str, QString end_date_str)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    qdialog_set_global_timezone->hide();

    if(is_timezone_already_added(start_date_epch,end_date_epch))
        return ;

    set_essential_data_lists(true);
    set_essential_variables_for_added_timezones(timezone);

    QString command = QString("Insert into tbl_saved_timezones(start_date_epoch,end_date_epoch,"
                              "timezone ,start_date_str,end_date_str,timezone_offset,display_time)"
                              "values(?,?,?,? ,?,?,?)");


    QStringList arg_list;
    arg_list << QString::number(start_date_epch)
             << QString::number(end_date_epch)
             << timezone
             << start_date_str
             << end_date_str
             << QString::number(current_timezone_offset)
             << display_time;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_file,Q_FUNC_INFO);

    display_data();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::slot_edit_time_zone(qint64 start_date_epch , qint64 end_date_epch, QString timezone , QString start_date_str, QString end_date_str)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    qdialog_set_global_timezone->hide();

    int current_row =  ui->tableWidget_timezone->currentRow();
    if(current_row < 0)
        return;

    QString INT_str = ui->tableWidget_timezone->item(current_row, enum_INT_column)->text();
    if(is_timezone_already_added_for_edit(start_date_epch , end_date_epch , INT_str))
        return ;


    set_essential_data_lists(false);
    set_essential_variables_for_added_timezones(timezone);
    QString command = "Update tbl_saved_timezones set start_date_epoch = ? , end_date_epoch = ? , timezone = ? , start_date_str = ? , end_date_str = ? ,display_time = ? , timezone_offset = ? where INT =?";

    QStringList arg_list;
    arg_list << QString::number(start_date_epch)
             << QString::number(end_date_epch)
             << timezone
             << start_date_str
             << end_date_str
             << display_time
             << QString::number(current_timezone_offset)
             << INT_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_file,Q_FUNC_INFO);

    display_data();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::on_pushButton_remove_timezone_clicked()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    int current_row =  ui->tableWidget_timezone->currentRow();
    if(current_row < 0)
        return ;

    QModelIndexList selection_model_list = ui->tableWidget_timezone->selectionModel()->selectedRows();
    if(selection_model_list.size() == 0)
        return ;


    QString INT_str =  ui->tableWidget_timezone->item(current_row,enum_INT_column)->text();
    ui->tableWidget_timezone->removeRow(current_row);
    QString delete_cmd = "delete from tbl_saved_timezones where INT = ?";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd,QStringList(INT_str),destination_db_file,Q_FUNC_INFO);

    display_data();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void recon_configuration_page_global_timezone::on_pushButton_edit_timezone_clicked()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    int current_row =  ui->tableWidget_timezone->currentRow();
    if(current_row < 0)
        return;


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return ;
    }

    QString command = QString("Select timezone, timezone_offset,start_date_epoch,end_date_epoch,start_date_str,end_date_str from tbl_saved_timezones where INT = ?");
    QString INT_str = ui->tableWidget_timezone->item(current_row, enum_INT_column)->text();

    QSqlQuery query_select(destination_db);
    query_select.prepare(command);
    query_select.addBindValue(INT_str);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query select timezone --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_select.lastError().text(),Q_FUNC_INFO);

        QSqlDatabase::removeDatabase(connection_naam);
        destination_db.close();
    }

    QString timezone, timezone_offset, start_date_epch, end_date_epch, start_date_str, end_date_str;
    if(query_select.next())
    {
        timezone = query_select.value("timezone").toString().trimmed();
        timezone_offset  = query_select.value("timezone_offset").toString().trimmed();
        start_date_epch = query_select.value("start_date_epoch").toString().trimmed();
        end_date_epch = query_select.value("end_date_epoch").toString().trimmed();
        start_date_str = query_select.value("start_date_str").toString().trimmed();
        end_date_str   = query_select.value("end_date_str").toString().trimmed();
    }
    destination_db.close();

    set_global_timezone_obj->pub_edit_timezone(timezone_qlist, timezone, start_date_epch, end_date_epch);

    qdialog_set_global_timezone->show();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void recon_configuration_page_global_timezone::on_comboBox_timezones_currentTextChanged(const QString &arg1)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    if(arg1.trimmed().isEmpty())
        return ;

    set_essential_data_lists(false);
    set_essential_variables_for_default_timezones();


    QString cmd = "update timezone_details set examiner_selected_timezone_qstring = ? , examiner_selected_timezone_qint64 = ?";
    QStringList argm_list;
    argm_list << arg1
              << QString::number(current_timezone_offset);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,argm_list, destination_db_file,Q_FUNC_INFO);

    set_default_time_zone(false);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void recon_configuration_page_global_timezone::on_comboBox_date_format_currentTextChanged(const QString &arg1)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    if(arg1.trimmed().isEmpty())
        return ;

    QString cmd = "update timezone_details set examiner_selected_date_format_qstring = ? , examiner_selected_date_format_int =?";
    QStringList argm_list;
    argm_list << arg1
              << QString::number(ui->comboBox_date_format->currentIndex());

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,argm_list, destination_db_file,Q_FUNC_INFO);

    set_default_time_zone(true);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void recon_configuration_page_global_timezone::set_combobox_items(QStringList combobox_list_items , bool bool_add_items_combobox)
{
    QString cmd = ("select examiner_selected_timezone_qstring,"
                   "examiner_selected_date_format_qstring from timezone_details");

    QString examiner_timezone_qstring = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd,0 , destination_db_file,Q_FUNC_INFO);
    QString dateformat_qstring = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd,1 , destination_db_file,Q_FUNC_INFO);


    ui->comboBox_timezones->blockSignals(true);

    if(examiner_timezone_qstring == QString("UTC +0000"))
        examiner_timezone_qstring = "GMT+00:00";

    if(bool_add_items_combobox)
    {
        ui->comboBox_timezones->addItem(examiner_timezone_qstring);
        for(int i = 0 ; i < combobox_list_items.size() ;i++)
        {
            if(combobox_list_items.at(i) == examiner_timezone_qstring)
            {
                combobox_list_items.removeOne(examiner_timezone_qstring);
                break;
            }
        }


        ui->comboBox_timezones->addItems(combobox_list_items);
    }

    ui->comboBox_timezones->blockSignals(false);


    ui->comboBox_date_format->clear();
    QStringList date_format_list;
    date_format_list << QString("yyyy-MMM-dd") << QString("MMM-dd-yyyy") << QString("dd-MMM-yyyy");
    for(int i = 0 ; i < date_format_list.size() ;i++)
    {
        QString str = date_format_list.at(i);
        ui->comboBox_date_format->blockSignals(true);
        ui->comboBox_date_format->addItem(str);
        ui->comboBox_date_format->blockSignals(false);
    }

    for(int i = 0 ; i < date_format_list.size() ;i++)
    {
        ui->comboBox_date_format->blockSignals(true);
        QString str = date_format_list.at(i);
        if(str == dateformat_qstring)
            ui->comboBox_date_format->setCurrentText(dateformat_qstring);

        ui->comboBox_date_format->blockSignals(false);
    }
}
