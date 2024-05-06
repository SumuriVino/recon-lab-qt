#include "emlx_mail.h"
#include "ui_emlx_mail.h"

void emlx_mail::slot_file_system_menu_right_clicked(QAction* current_clicked_action)
{ // slot of right click actons

    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
    {
        action_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_remove_bookmarks_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Note))
    {
        action_add_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Notes_to_Bookmarks))
    {
        action_add_note_to_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        action_remove_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks))
    {
        action_remove_note_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Tag))
    {
        action_remove_tag_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
    {
        emit signal_act_quick_look_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Go_To_Record))
    {
        int selected_row_no = ui->tableWidget_emails->currentRow();

        if(selected_row_no > 0)
        {
            QString source_path = ui->tableWidget_emails->item(selected_row_no, enum_emlx_source_file_column)->text().trimmed();
            QString source_count_name = ui->tableWidget_emails->item(selected_row_no, enum_emlx_source_count_name_column)->text().trimmed();

            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

            emit signal_go_to_source_in_file_system_from_emlx(source_path,fs_db_path,source_count_name);
        }
    }

}

void emlx_mail::action_bookmark_triggered()
{ // action add bookmark clicked
    QModelIndexList selection_model_list = ui->tableWidget_emails->selectionModel()->selectedRows();

    QStringList record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(ui->tableWidget_emails->isRowHidden(selected_row_no))
            continue;

        QString record = ui->tableWidget_emails->item(selected_row_no, enum_emlx_sr_no_column)->text().trimmed();
        record_num_list << record ;

        ui->tableWidget_emails->item(selected_row_no, enum_emlx_bookmark_column)->setCheckState(Qt::Checked);


    }

    if(record_num_list.isEmpty())
        return;

    QStringList record_num_list_to_update_bookmarks_db = record_num_list;

    int bunch_count = 50;
    QString command = QString("Update emlx_data Set bookmark = '1' Where ");

    QString tmp_cmd;
    int pos = 0;

    for(int count = (record_num_list.size() -1); count >= 0; count--)
    {
        QString record_no_str = record_num_list.takeAt(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no_str);
        tmp_cmd.append(" OR ");

        pos++;

        if(count == bunch_count || count == 0)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, source_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();

            pos = 0;
        }

    }


    update_bookmarks_in_bookmark_serach(record_num_list_to_update_bookmarks_db , QStringList(""));
    emit signal_common_add_tag_name_into_case_tree(MACRO_Generic_Tag_Bookmarks);


}

void emlx_mail::action_remove_bookmarks_triggered()
{ // action remove bookmark clicked
    QModelIndexList selection_model_list = ui->tableWidget_emails->selectionModel()->selectedRows();

    QStringList record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(ui->tableWidget_emails->isRowHidden(selected_row_no))
            continue;


        QString record = ui->tableWidget_emails->item(selected_row_no, enum_emlx_sr_no_column)->text().trimmed();
        record_num_list << record ;

        ui->tableWidget_emails->item(selected_row_no, enum_emlx_bookmark_column)->setCheckState(Qt::Unchecked);


    }

    if(record_num_list.isEmpty())
        return;

    QStringList records_list_for_bookmarks = record_num_list;


    int bunch_count = 50;
    QString command = QString("Update emlx_data Set bookmark = '0' Where ");

    QString tmp_cmd;
    int pos = 0;
    for(int count = (record_num_list.size() -1); count >= 0; count--)
    {
        QString record_no_str = record_num_list.takeAt(count);

        if(pos == 0)
            tmp_cmd = command;

        tmp_cmd.append("INT = ");
        tmp_cmd.append(record_no_str);
        tmp_cmd.append(" OR ");

        pos++;

        if(count == bunch_count || count == 0)
        {
            if(tmp_cmd.endsWith(" OR "))
                tmp_cmd.chop(QString(" OR ").size());

            recon_helper_standard_obj->execute_db_command_by_dbpath(tmp_cmd, source_db_path, Q_FUNC_INFO);
            tmp_cmd.clear();

            pos = 0;
        }

    }

    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    for(int i = 0 ; i < records_list_for_bookmarks.size() ; i++)
    {

        QString record_no = records_list_for_bookmarks.at(i);
        QStringList arg_list_rm;
        QString del_command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ?";
        arg_list_rm << record_no << QString(MACRO_Plugin_Name_Email_Parser) << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(del_command,arg_list_rm,bookmark_db_path,Q_FUNC_INFO);

    }
    emit signal_common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);

}

void emlx_mail::action_add_note_triggered()
{

}

void emlx_mail::action_add_note_to_bookmark_triggered()
{

}

void emlx_mail::action_remove_note_triggered()
{

}

void emlx_mail::action_remove_note_bookmark_triggered()
{

}

void emlx_mail::slot_act_create_tags()
{
    add_tags_module_obj->show();
}

void emlx_mail::create_tags_submenu()
{ // create tags submenu for tags adding/removing
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_tags == NULL)
        return;

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));


    ///remove tag
    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    sub_menu_tags->addAction(act_remove_tag);

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                sub_menu_tags->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";
            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);
}


void emlx_mail::slot_add_tags(QString tag_data, QString colour)
{ // slot of add tags in right click
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = ui->tableWidget_emails->selectionModel()->selectedRows();


    QStringList record_num_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= ui->tableWidget_emails->rowCount())
            continue;

        if(ui->tableWidget_emails->isRowHidden(selected_row_no))
            continue;

        QString record = ui->tableWidget_emails->item(selected_row_no, enum_emlx_sr_no_column)->text().trimmed();
        record_num_list << record ;


        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);

        int tag_position = enum_emlx_tag_column;
        ui->tableWidget_emails->item(selected_row_no,tag_position)->setIcon(icon);

        QStringList arg_list;
        QString command = QString("UPDATE emlx_data SET recon_tag_value = ? WHERE INT = ?");;
        arg_list  << tag_data <<  record;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,source_db_path,Q_FUNC_INFO);

    }


    update_tags_in_tag_serach(record_num_list , tag_data);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void emlx_mail::action_remove_tag_triggered()
{ // action remove tags clicking on right click
    QModelIndexList selection_model_list = ui->tableWidget_emails->selectionModel()->selectedRows();

    QStringList record_num_list;


    for(int i = 0; i < selection_model_list.size(); i++)
    {


        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= ui->tableWidget_emails->rowCount())
            continue;


        if(ui->tableWidget_emails->isRowHidden(selected_row_no))
            continue;

        QString record = ui->tableWidget_emails->item(selected_row_no, enum_emlx_sr_no_column)->text().trimmed();
        record_num_list << record ;


        ///------------update table item first
        QIcon icon("");
        ui->tableWidget_emails->item(selected_row_no, enum_emlx_tag_column)->setIcon(icon);


    }

    if(record_num_list.isEmpty())
        return;


    QStringList tags_list_to_be_removed;
    for( int i = 0 ; i <  record_num_list.size(); i++)
    {

        ///---------before update db select recon tag value
        QString record_no = record_num_list.at(i);
        QString select_command = "SELECT recon_tag_value from emlx_data where INT = ?";
        QStringList arg_list;
        arg_list << record_no;
        QString tag_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_command,arg_list,0,source_db_path,Q_FUNC_INFO);



        ///---------update in emlx db
        QString update_command = "Update emlx_data Set recon_tag_value = ? where INT = ?";
        QStringList arg_list_1;
        arg_list_1 << QString("") <<record_no;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,arg_list_1,source_db_path,Q_FUNC_INFO);



        ///---------update tag serach db
        if(!tags_list_to_be_removed.contains(tag_name) && !tag_name.trimmed().isEmpty())
        {

            QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_name + ".sqlite";
            QStringList arg_list_rm;
            QString del_command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ?";
            arg_list_rm << record_no << QString(MACRO_Plugin_Name_Email_Parser) << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data);
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(del_command,arg_list_rm,tag_db_path,Q_FUNC_INFO);

            emit signal_common_add_tag_name_into_case_tree(tag_name);

        }
    }

}

void emlx_mail::slot_abouttoshow_tags_in_submenu()
{
    create_tags_submenu();

    create_raw_data_tags_submenu();

}

void emlx_mail::slot_tablewidget_goto_record(QString record )
{ // slot of go to record action on tablewidget on right click


    if(record == "")
    {
        QString command = "SELECT INT FROM emlx_data WHERE INT = ?";
        QStringList arg_list;
        arg_list << record;
        record = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,source_db_path,Q_FUNC_INFO);
    }

    int select_row = 0;
    bool row_found_in_existin_display = false;

    for(int row = 0; row < ui->tableWidget_emails->rowCount(); row++)
    {
        QString INT_str = ui->tableWidget_emails->item(row,enum_emlx_sr_no_column)->text();

        if(INT_str == record)
        {
            row_found_in_existin_display = true;
            select_row = row;
            break;
        }
    }

    if(row_found_in_existin_display)
    {
        ui->tableWidget_emails->selectRow(select_row);

        QModelIndex index = ui->tableWidget_emails->model()->index(select_row, enum_emlx_sr_no_column);
        ui->tableWidget_emails->scrollTo(index, QAbstractItemView::PositionAtCenter);
    }
}

void emlx_mail::slot_tags_submenu_clicked(QAction* current_clicked_action)
{ // tyags submenu clicked
    action_submenu_tag_triggered(current_clicked_action->text());
}

void emlx_mail::action_submenu_tag_triggered(QString tag_name)
{ // action submenu tags clicked on right click
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    tag_name = tag_name.trimmed();

    if(tag_name.isEmpty())
        return;

    if(tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,tag_db_path,Q_FUNC_INFO);

    emit signal_add_tags(tag_name,colour);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

QMenu* emlx_mail::create_right_click_submenu(QString submenuStr ,QMenu *mainMenu , QMenu *subMenu)
{ // create right click submenu
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    subMenu = mainMenu->addMenu(tr(submenuStr.toLocal8Bit().data()));

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    subMenu->setFont(m_font);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return subMenu;
}

void emlx_mail::update_tags_in_tag_serach(QStringList record_no_list , QString tag_data)
{ // update tags in tag search database

    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_data,Q_FUNC_INFO);


    for(int i = 0 ; i < record_no_list.size() ; i++)
    {

        QString record_no = record_no_list.at(i);
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        source_db.setDatabaseName(source_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
            return;
        }
        QStringList arg_list;
        QSqlQuery query(source_db);
        query.prepare("select INT,bookmark,recon_tag_value ,notes,emlx_to,emlx_from,emlx_cc,emlx_date,emlx_subject,emlx_msg_body,email_attachment,account_id,mbox,source_file,os_scheme_display,source_count_name from emlx_data where INT = ?");
        query.addBindValue(record_no);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query select -------FAILED------- " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
            source_db.close();
            return;
        }
        if(!query.next())
        {
            recon_static_functions::app_debug(" --- No Record Exist---",Q_FUNC_INFO);
            return;
        }

        QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=?";
        QStringList arg_list_for_tags;
        arg_list_for_tags << QString(MACRO_Plugin_Name_Email_Parser) << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data) << record_no;
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);



        if(INT_str.isEmpty())
        {
            QString command_insert = "INSERT INTO tags(record_no,bookmark,recon_tag_value,notes,"
                                     "item0,item1,item2,item3,item4,item5,item6,item7,item8,item9"
                                     ",plugin_name,category,tab_name,"
                                     "csv_plugin_name,csv_tab_name,source_count_name, os_scheme_display)"
                                     "VALUES (?,?,?,?, ?,?,?,?, ?,?,?,?, ?,?,?,? ,?,?,?,?,?)";

            arg_list << query.value(0).toString()
                     << query.value(1).toString()
                     << query.value(2).toString()
                     << query.value(3).toString()
                     << query.value(4).toString()
                     << query.value(5).toString()
                     << query.value(6).toString()
                     << query.value(7).toString()
                     << query.value(8).toString()
                     << query.value(9).toString()
                     << query.value(10).toString()
                     << query.value(11).toString()
                     << query.value(12).toString()
                     << query.value(13).toString()
                     << QString(MACRO_Plugin_Name_Email_Parser)
                     << QString(MACRO_Plugin_Name_Email_Parser)
                     << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data)
                     << QString(MACRO_Plugin_Name_Email_Parser)
                     << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data)
                     << query.value("source_count_name").toString()
                     << query.value("os_scheme_display").toString();

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);
        }
        else
        {
            QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE record_no=?";
            QStringList arg_list;
            arg_list << tag_data << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);

        }


        source_db.close();

        delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(tag_data,record_no,QString(MACRO_Plugin_Name_Email_Parser),QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data));

    }

    emit signal_common_add_tag_name_into_case_tree(tag_data);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);


}

void emlx_mail::update_bookmarks_in_bookmark_serach(QStringList record_no_list, QStringList tag_name_list)
{ // update bookmark in bookmarks database according to status

    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);


    QString record_no;
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    for(int i = 0 ; i < record_no_list.size() ;i++)
    {

        record_no = record_no_list.at(i);
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        source_db.setDatabaseName(source_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
            return;
        }
        QStringList arg_list;
        QSqlQuery query(source_db);
        query.prepare("select INT,bookmark,recon_tag_value ,notes,emlx_to,emlx_from,emlx_cc,emlx_date,emlx_subject,emlx_msg_body,email_attachment,account_id,mbox,source_file,os_scheme_display,source_count_name from emlx_data where INT = ?");
        query.addBindValue(record_no);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query select -------FAILED------- " + query.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
            source_db.close();
            return;
        }
        if(!query.next())
        {
            recon_static_functions::app_debug(" --- No Record Exist---",Q_FUNC_INFO);
            return;
        }

        QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=?";
        QStringList arg_list_for_tags;
        arg_list_for_tags << QString(MACRO_Plugin_Name_Email_Parser) << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data) << record_no;
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,bookmark_db_path,Q_FUNC_INFO);


        if(INT_str.isEmpty())
        {
            QString command_insert = "INSERT INTO tags(record_no,bookmark,recon_tag_value,notes,"
                                     "item0,item1,item2,item3,item4,item5,item6,item7,item8,item9"
                                     ",plugin_name,category,tab_name,"
                                     "csv_plugin_name,csv_tab_name,source_count_name, os_scheme_display,recon_tag_value)"
                                     "VALUES (?,?,?,?, ?,?,?,?, ?,?,?,?, ?,?,?,? ,?,?,?,?,?,?)";

            arg_list << query.value(0).toString()
                     << query.value(1).toString()
                     << query.value(2).toString()
                     << query.value(3).toString()
                     << query.value(4).toString()
                     << query.value(5).toString()
                     << query.value(6).toString()
                     << query.value(7).toString()
                     << query.value(8).toString()
                     << query.value(9).toString()
                     << query.value(10).toString()
                     << query.value(11).toString()
                     << query.value(12).toString()
                     << query.value(13).toString()
                     << QString(MACRO_Plugin_Name_Email_Parser)
                     << QString(MACRO_Plugin_Name_Email_Parser)
                     << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data)
                     << QString(MACRO_Plugin_Name_Email_Parser)
                     << QString(MACRO_Plugin_Email_Parser_TAB_Name_Email_Data)
                     << query.value("source_count_name").toString()
                     << query.value("os_scheme_display").toString()
                     << QString(MACRO_Generic_Tag_Bookmarks);


            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,bookmark_db_path,Q_FUNC_INFO);
        }
        else
        {
            //            QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE record_no=?";
            //            QStringList arg_list;
            //            arg_list << tag_name << record_no;
            //            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);

        }

        source_db.close();

    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);


}


void emlx_mail::delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag , QString record_no , QString plugin_name , QString tab_name)
{ // delete tags from database when this function calls
    QString current_tag_name = current_tag;
    QString tag_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QString command = "Select tag_db_name from tag_search_index";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command , 0 ,tag_index_db_path, Q_FUNC_INFO);




    for(int i = 0 ; i <  tag_db_name_list.size(); i++)
    {

        QString db_name = tag_db_name_list.at(i);
        if(db_name == current_tag_name)
            continue ;

        ///1.---delete record from previous tag db
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + db_name + ".sqlite";
        QStringList arg_list;
        command = "Delete from tags where record_no = ? AND plugin_name = ? AND tab_name = ?";
        arg_list << record_no << plugin_name << tab_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);


        ///2.---refresh case tree tag
        emit signal_common_add_tag_name_into_case_tree(db_name);

    }

}

void emlx_mail::slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour)
{ // update tags submenu
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}




void emlx_mail::slot_right_click_menu_raw_data_triggered(QAction* current_clicked_action)
{ // raw data clicked
    if(current_clicked_action->text() == QString("Select All"))
    {
        ui->textBrowser_raw_data->selectAll();
    }
}

void emlx_mail::slot_sub_menu_raw_data_tags_clicked(QAction *action)
{ // submenu raw data tags clicked
    action_submenu_raw_data_tag_triggered(action->text());
}
