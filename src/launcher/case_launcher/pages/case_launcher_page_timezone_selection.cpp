#include "case_launcher_page_timezone_selection.h"
#include "ui_case_launcher_page_timezone_selection.h"

case_launcher_page_timezone_selection::case_launcher_page_timezone_selection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::case_launcher_page_timezone_selection)
{
    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_system_time_zone->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->comboBox_date_format->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->comboBox_user_time_zone->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->radioButton_select_timezone->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->radioButton_system_timezone->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->radioButton_utc_timezone->setAttribute(Qt::WA_MacShowFocusRect,false);

    create_time_zone_list_combobox();

    ui->radioButton_utc_timezone->setText(global_lang_tr_obj.get_translated_string(ui->radioButton_utc_timezone->text()));
    ui->radioButton_system_timezone->setText(global_lang_tr_obj.get_translated_string(ui->radioButton_system_timezone->text()));
    ui->radioButton_select_timezone->setText(global_lang_tr_obj.get_translated_string(ui->radioButton_select_timezone->text()));
    ui->label_date_format->setText(global_lang_tr_obj.get_translated_string(ui->label_date_format->text()));

    ui->lineEdit_utc_timezone->setText("UTC +0000");

    recon_static_functions::app_debug(" end", Q_FUNC_INFO);
}

case_launcher_page_timezone_selection::~case_launcher_page_timezone_selection()
{
    delete ui;
}

void case_launcher_page_timezone_selection::create_time_zone_list_combobox()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName("../Resources/timezone.sqlite");
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug("../Resources/timezone.sqlite" ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("select distinct name,code, timezone from timezone ORDER BY INT");
    if(!query.exec())
    {
        recon_static_functions::app_debug(" -query execution-------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QString temp = query.value(0).toString() + "-" + query.value(1).toString() + "-" + query.value(2).toString();
        ui->comboBox_user_time_zone->addItem(temp);
    }

    ui->comboBox_user_time_zone->insertItem(0, "<Select Time Zone>");
    ui->comboBox_user_time_zone->setCurrentIndex(0);


    destination_db.close();

    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}


void case_launcher_page_timezone_selection::pub_initialise_timezone_details()
{
    if(page_caller_string == MACRO_TIMEZONE_SELECTION_CASE_LAUNCHER)
    {
        destination_db_file.clear();
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";

    }
    else if(page_caller_string == MACRO_TIMEZONE_SELECTION_RECON_CONFIGURATION)
    {
        destination_db_file.clear();

        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";
    }


    display_timezone_from_db();

}


void case_launcher_page_timezone_selection::pub_update_timezone_info()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName("../Resources/timezone.sqlite");
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug("../Resources/timezone.sqlite" ,Q_FUNC_INFO);
        return;
    }

    //-- New Code.
    QString examiner_timezone_name = "";
    QString examiner_timezone_offset = "0";

    //-We always need system timezone..
    QStringList list_current_user =  ui->lineEdit_system_time_zone->text().trimmed().split("-", Qt::SkipEmptyParts);
    QString timezone_area_name  = "";
    if(list_current_user.size() > 0)
        timezone_area_name = list_current_user.at(0);

    qint64 time_zone_offset =  recon_helper_standard_obj->get_time_zone_offset_without_sign(timezone_area_name,destination_db , Q_FUNC_INFO);
    QString command_offset = "select distinct off_seconds from timezone where name = ? AND off_seconds = ?";
    QStringList argument_list;
    argument_list << timezone_area_name << QString::number(time_zone_offset);
    examiner_timezone_name = ui->lineEdit_system_time_zone->text().trimmed();
    examiner_timezone_offset = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_offset,argument_list,0,destination_db,Q_FUNC_INFO);
    QString current_system_offset = examiner_timezone_offset;

    QString command_offset_sign = "select distinct timezone from timezone where name = ? AND off_seconds = ?";
    QString offset_sign_with_other_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_offset_sign,argument_list,0,destination_db,Q_FUNC_INFO);

    QString machine_timezone_offset_sign;
    if(offset_sign_with_other_str.startsWith("GMT") && offset_sign_with_other_str.size() > 3)
        machine_timezone_offset_sign = offset_sign_with_other_str.at(3);

    current_system_offset.prepend(machine_timezone_offset_sign);

    QString user_timezone_offset_sign;

    if(ui->radioButton_utc_timezone->isChecked())
    {
        examiner_timezone_name = ui->lineEdit_utc_timezone->text().trimmed();
        examiner_timezone_offset = "0000";
    }
    else if(ui->radioButton_select_timezone->isChecked())
    {
        examiner_timezone_name = ui->comboBox_user_time_zone->currentText().trimmed();

        QStringList list_current_user =  examiner_timezone_name.split("-", Qt::SkipEmptyParts);
        QString timezone_area_name  = "";
        if(list_current_user.size() > 0)
            timezone_area_name = list_current_user.at(0);

        QString tm_short_name = list_current_user.at(1);

        argument_list.clear();
        argument_list << timezone_area_name;
        argument_list << tm_short_name.trimmed();

        command_offset = "select off_seconds from timezone where name = ? AND code = ?";
        examiner_timezone_offset = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_offset,argument_list,0,destination_db,Q_FUNC_INFO);

    }

    if(examiner_timezone_name.contains("GMT-"))
        user_timezone_offset_sign = "-";


    examiner_timezone_offset.prepend(user_timezone_offset_sign);


    //-Updatin in Case Configuration db.
    QString command_update = "update timezone_details set case_created_machine_timezone_qstring = ?, case_created_machine_timezone_qint64 = ? ,examiner_selected_timezone_qstring = ?,"
                             "examiner_selected_timezone_qint64= ?, examiner_selected_date_format_int= ?,examiner_selected_date_format_qstring = ?";
    QStringList list;
    list << ui->lineEdit_system_time_zone->text().trimmed() << current_system_offset << examiner_timezone_name << examiner_timezone_offset;
    list << QString::number(ui->comboBox_date_format->currentIndex()) << ui->comboBox_date_format->currentText();
    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,list,destination_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("update timezone_details ---FAILED---",Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void case_launcher_page_timezone_selection::pub_set_page_caller(QString caller_name)
{
    page_caller_string = caller_name;
}

void case_launcher_page_timezone_selection::pub_get_timezone_details()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_timezone_details(destination_db);
    if(!query_timezone_details.exec("select case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,examiner_selected_timezone_qstring,examiner_selected_timezone_qint64,examiner_selected_date_format_int,examiner_selected_date_format_qstring from timezone_details"))
    {
        recon_static_functions::app_debug(" timezone query_timezone_details ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_timezone_details.lastError().text(),Q_FUNC_INFO);
        return;
    }

    struct_global_default_timezone_details obj;

    if(query_timezone_details.next())
    {

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString,query_timezone_details.value(2).toString());  // examiner_selected_timezone_qstring
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64,query_timezone_details.value(3).toLongLong()); // examiner_selected_timezone_qint64

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_int,query_timezone_details.value(4).toInt()); // examiner_selected_date_format_int
        QString examiner_selected_date_format = query_timezone_details.value(5).toString();
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_QString,examiner_selected_date_format); // examiner_selected_date_format_qstring

        if(!examiner_selected_date_format.trimmed().isEmpty())
        {
            QString selected_datetime_format = examiner_selected_date_format + QString(" hh:mm:ss");
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString,selected_datetime_format);

        }

        obj.d_case_created_machine_timezn_readable_QString = query_timezone_details.value(0).toString();
        obj.d_case_created_machine_timezn_numeric_qint64 = query_timezone_details.value(1).toString().toLongLong();


        QString user_selected_t_zone = query_timezone_details.value(2).toString();

        obj.d_examiner_selected_timezone_readable_QString = user_selected_t_zone;
        obj.d_examiner_selected_date_format_numeric_qint64 = query_timezone_details.value(4).toString().toInt();
        obj.d_examiner_selected_date_format_readable_QString = query_timezone_details.value(5).toString() + " " + "hh:mm:ss";
        obj.d_examiner_selected_timezone_numeric_qint64 = query_timezone_details.value(3).toString().toLongLong();

        if(user_selected_t_zone.contains("GMT") || user_selected_t_zone.contains("GMT-GMT"))
        {
            QStringList tm_list;
            if(user_selected_t_zone.contains("GMT-GMT"))
                tm_list = user_selected_t_zone.split("GMT-GMT");
            else
                tm_list = user_selected_t_zone.split("GMT");

            if(user_selected_t_zone.size() > 1)
                user_selected_t_zone = tm_list.at(1);

            obj.timezone_offset_name = user_selected_t_zone;
        }
        else
        {
            user_selected_t_zone = "+00:00";
        }

        obj.timezone_offset_name = user_selected_t_zone;
    }
    else
    {
        struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

        QString current_machine_timezone = strct_exm_obj.timezone_name_complete_QString;
        qint64 current_machine_timezone_offset = strct_exm_obj.timezone_offset_numeric_qint64;

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_QString, "UTC +0000");
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64, 0);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_int, 0);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Date_Format_QString, "yyyy-MMM-dd");
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString, "yyyy-MMM-dd hh:mm:ss");

        obj.d_case_created_machine_timezn_readable_QString = current_machine_timezone;
        obj.d_case_created_machine_timezn_numeric_qint64 = current_machine_timezone_offset;
        obj.d_examiner_selected_timezone_readable_QString = "UTC +0000";
        obj.d_examiner_selected_date_format_numeric_qint64 = 0;
        obj.d_examiner_selected_date_format_readable_QString = "yyyy-MMM-dd hh:mm:ss";
        obj.d_examiner_selected_timezone_numeric_qint64 = 0;
        obj.timezone_offset_name = "+00:00";
    }
    global_recon_helper_singular_class_obj->pub_set_default_timezone(obj);

    destination_db.close();
    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);
}


void case_launcher_page_timezone_selection::display_timezone_from_db()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_timezone_details(destination_db);
    if(!query_timezone_details.exec("select case_created_machine_timezone_qstring,examiner_selected_timezone_qstring,examiner_selected_date_format_int from timezone_details"))
    {
        recon_static_functions::app_debug(" agency_details query_timezone_details ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_timezone_details.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString examiner_selected_timezone;
    if(query_timezone_details.next())
    {
        examiner_selected_timezone = query_timezone_details.value(1).toString().trimmed();

        struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

        QString current_machine_timezone = strct_exm_obj.timezone_name_complete_QString;
        qint64 current_machine_timezone_offset = strct_exm_obj.timezone_offset_numeric_qint64;

        QString update_cmd = "UPDATE timezone_details set case_created_machine_timezone_qstring = ? , case_created_machine_timezone_qint64 = ?";
        QStringList arglist ;
        arglist << current_machine_timezone << QString::number(current_machine_timezone_offset);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_cmd,arglist,destination_db,Q_FUNC_INFO);
        ui->lineEdit_system_time_zone->setText(current_machine_timezone);


        // ui->lineEdit_system_time_zone->setText(query_timezone_details.value(0).toString());
        ui->comboBox_user_time_zone->setCurrentText(examiner_selected_timezone);
        ui->comboBox_date_format->setCurrentIndex(query_timezone_details.value(2).toInt());

        if(examiner_selected_timezone == QString(MACRO_User_Selected_Timezone_UTC))
            ui->radioButton_utc_timezone->setChecked(true);
        else if(examiner_selected_timezone == QString(MACRO_User_Selected_Timezone_Current_Machine))
            ui->radioButton_system_timezone->setChecked(true);
        else if(examiner_selected_timezone == QString(MACRO_User_Selected_Timezone_Others))
            ui->radioButton_select_timezone->setChecked(true);

    }

    destination_db.close();

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}


void case_launcher_page_timezone_selection::on_radioButton_utc_timezone_toggled(bool checked)
{
    ui->lineEdit_system_time_zone->setEnabled(!checked);
    ui->comboBox_user_time_zone->setEnabled(!checked);
    emit signal_enable_disable_next_button_in_launcher_page(true);
}

void case_launcher_page_timezone_selection::on_radioButton_system_timezone_toggled(bool checked)
{
    ui->lineEdit_utc_timezone->setEnabled(!checked);
    ui->comboBox_user_time_zone->setEnabled(!checked);
    emit signal_enable_disable_next_button_in_launcher_page(true);
}

void case_launcher_page_timezone_selection::on_radioButton_select_timezone_toggled(bool checked)
{
    ui->lineEdit_utc_timezone->setEnabled(!checked);
    ui->lineEdit_system_time_zone->setEnabled(!checked);

    if(ui->comboBox_user_time_zone->currentIndex() == 0)
        emit signal_enable_disable_next_button_in_launcher_page(false);
    else
        emit signal_enable_disable_next_button_in_launcher_page(true);
}

void case_launcher_page_timezone_selection::on_comboBox_user_time_zone_currentIndexChanged(int index)
{
    if(index == 0)
        emit signal_enable_disable_next_button_in_launcher_page(false);
    else
        emit signal_enable_disable_next_button_in_launcher_page(true);
}
