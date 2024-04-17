#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::enable_navigation_combobox_category_and_feature(int item_pos, bool status, QFlatComboBox *combobox_nav)
{
    if(combobox_nav->count() > item_pos)
        qobject_cast<QStandardItemModel *>(combobox_nav->model())->item(item_pos)->setEnabled(status);

}


void MainWindow::comman_remove_feature_from_combobox_and_stackwidget(QString unique_pattrn)
{
    //    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter + combobox_category_navigation->currentText() + MACRO_RECON_Splitter + combobox_current_displayed_feature_opened->currentText();


    slot_refresh_widgets();

    //- Remove Widget from stack.
    for(int i = (stack_widget_cases_plugins_and_global_items->count() -1) ; i >= 0; i--)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        if(widget_ptr->pub_get_widget_unique_name() == unique_pattrn)
        {
            if(widget_ptr->layout()->itemAt(0) != NULL)
            {
                for(int j = 0 ; j < list_stroy_board_obj.size() ; j++)
                {
                    if(list_stroy_board_obj.at(j) == widget_ptr->layout()->itemAt(0)->widget())
                    {
                        list_stroy_board_obj.removeAt(j);
                        break;
                    }
                }
            }


            stack_widget_cases_plugins_and_global_items->removeWidget(widget_ptr);
            center_stackwidget_manager_obj->pub_remove_entry_feature_form_list(unique_pattrn);

            //-Updated on 4th Feb
            remove_entry_form_the_feature_combobox(combobox_current_displayed_feature_opened->currentText());

            emit signal_remove_navigation_list_entry_on_combobox_pusbutton_close(unique_pattrn);
            close_feature_without_save(unique_pattrn);

            break;
        }
    }

    if(combobox_current_displayed_feature_opened->count() == 1)
    {
        combobox_category_navigation->removeItem(combobox_category_navigation->currentIndex());
        combobox_category_navigation->setCurrentIndex(combobox_category_navigation->count() -1 );
    }

    if(combobox_category_navigation->count() == 1 && combobox_category_navigation->currentText() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
    {
        combobox_case_navigation->removeItem(combobox_case_navigation->currentIndex());
        combobox_case_navigation->setCurrentIndex(combobox_case_navigation->count() -1);
    }

}


void MainWindow::insert_plist_file_entry_from_tmp_to_recon(QString viewer_display_name,QString plist_sqlite_path_tmp,QString plist_db_path_recon)
{
    recon_static_functions::app_debug("---Starts-----" ,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(plist_sqlite_path_tmp);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + plist_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    QString select_command = "SELECT viewer_display_name, db_table_name, file_name, file_path, "
                             "plugin_name, category_name, record_no, creation_timestamp, "
                             "source_count_name ,recon_filefrom , recon_file_infopath from 'plist_viewer_index' where viewer_display_name = ?";
    query.prepare(select_command);
    query.addBindValue(viewer_display_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + plist_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QStringList arg_list;
    QString db_table_name;
    if(query.next())
    {
        db_table_name = query.value("db_table_name").toString();
        arg_list << query.value("viewer_display_name").toString()
                 << query.value("db_table_name").toString()
                 << query.value("file_name").toString()
                 << query.value("file_path").toString()
                 << query.value("plugin_name").toString()
                 << query.value("category_name").toString()
                 << query.value("record_no").toString()
                 << query.value("creation_timestamp").toString()
                 << query.value("source_count_name").toString()
                 << query.value("recon_filefrom").toString()
                 << query.value("recon_file_infopath").toString();
    }

    destination_db.close();

    QString command = "INSERT INTO 'plist_viewer_index'(viewer_display_name,db_table_name,file_name,"
                      "file_path,plugin_name,category_name,record_no,creation_timestamp, "
                      "source_count_name, recon_filefrom,recon_file_infopath ) VALUES(?,?,?,?,?,?,?,?, ?,?,?)";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,plist_db_path_recon,Q_FUNC_INFO);

    copy_plist_file_detail_from_tmp_to_recon(db_table_name,plist_sqlite_path_tmp,plist_db_path_recon);

    recon_static_functions::app_debug("---Ends-----" ,Q_FUNC_INFO);
    return;
}

void MainWindow::copy_from_tmp_for_registry_viewer(QString display_tab_name)
{
    recon_static_functions::app_debug("---Starts-----" ,Q_FUNC_INFO);

    QString registry_viewer_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString();
    QString cmd = "Select source_count_name ,relative_db_path from tbl_registry_viewer_index";
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd , 0,registry_viewer_tmp_db_path , Q_FUNC_INFO);
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd , 1,registry_viewer_tmp_db_path , Q_FUNC_INFO);

    QString regsitry_dir_path =  registry_viewer_result_dir_path + source_count_name;
    QDir dir;
    dir.mkpath(regsitry_dir_path);


    QFileInfo file_info(relative_db_path);
    QString sqlite_name = file_info.fileName();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString tmp_registry_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);
    QString saved_db_path = regsitry_dir_path + "/" + sqlite_name;


    QString new_relative_file_path = saved_db_path;
    if(new_relative_file_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
        new_relative_file_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());

    if(!new_relative_file_path.startsWith("/"))
        new_relative_file_path.prepend("/");



    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(registry_viewer_tmp_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    QString temp_cmd = "Select display_tab_name , source_file_name , source_file_path , source_count_name ,recon_filefrom ,recon_file_infopath from tbl_registry_viewer_index where display_tab_name = ?";

    query.prepare(temp_cmd);
    query.addBindValue(display_tab_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QStringList arg_list;
    if(query.next())
    {
        arg_list << query.value("display_tab_name").toString()
                 << query.value("source_file_name").toString()
                 << query.value("source_file_path").toString()
                 << query.value("source_count_name").toString()
                 << new_relative_file_path
                 << query.value("recon_filefrom").toString()
                 << query.value("recon_file_infopath").toString();
    }

    destination_db.close();

    QString command = "INSERT INTO tbl_registry_viewer_index(display_tab_name,source_file_name,source_file_path,source_count_name,"
                      "relative_db_path ,recon_filefrom , recon_file_infopath) "
                      "VALUES(?,?,?,?,?,?,?)";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite"),Q_FUNC_INFO);



    ////1. copy db at /Lab_Feature/Registry_Viewer/
    QFile::copy(tmp_registry_db_path , saved_db_path);

    ///2. delete entries
    remove_entries_for_registry_viewer(display_tab_name ,tmp_registry_db_path , registry_viewer_tmp_db_path);

    emit signal_registry_viewer_db_path_changed(saved_db_path);

}

void MainWindow::remove_entries_for_registry_viewer(QString display_tab_name, QString db_path , QString index_db_path)
{
    QString delete_cmd = "Delete from tbl_registry_viewer_index where display_tab_name = ?";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd,QStringList(display_tab_name),index_db_path,Q_FUNC_INFO);

    //// delete db where results are saved
    recon_static_functions::remove_safely_blank_dir_OR_file(db_path, Q_FUNC_INFO);

    recon_static_functions::app_debug("---Ends-----" ,Q_FUNC_INFO);
}

void MainWindow::copy_plist_file_detail_from_tmp_to_recon(QString db_table_name_of_temp,QString plist_sqlite_path_tmp,QString plist_db_pth_recon)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);


    QString command_create_table = "CREATE TABLE '" + db_table_name_of_temp + "' (bookmark INTEGER,INT INTEGER PRIMARY KEY,id INTEGER,key VARCHAR(300),type VARCHAR(300),value VARCHAR(300),"
                                                                              "parent VARCHAR(300),keypath VARCHAR(300),parentpath VARCHAR(300), data_conversion_type VARCHAR(500), recon_tag_value VARCHAR(500), notes VARCHAR(500))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command_create_table,plist_db_pth_recon,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(plist_sqlite_path_tmp);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + plist_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT bookmark,id,key,type,value,parent,keypath,parentpath,notes, recon_tag_value,data_conversion_type FROM '" + db_table_name_of_temp + "'");

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + plist_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase destination_db_recon = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    destination_db_recon.setDatabaseName(plist_db_pth_recon);
    if(!destination_db_recon.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + plist_db_pth_recon,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db_recon.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QStringList arg_list;

        arg_list << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
                 << query.value(3).toString() << query.value(4).toString() << query.value(5).toString()
                 << query.value(6).toString() << query.value(7).toString() << query.value(8).toString()


                 << query.value(9).toString() << query.value(10).toString();
        QString command = "INSERT INTO '"+ db_table_name_of_temp + "' (bookmark,id,key,type,value,parent,keypath,parentpath, notes, recon_tag_value, data_conversion_type) VALUES(?,?,?,?,?, ?,?,?,?,?,?)";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db_recon,Q_FUNC_INFO);
    }

    destination_db.close();
    destination_db_recon.close();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);

}

void MainWindow::insert_log_file_index_table_entry_from_tmp_db_to_recon_db(QString viewer_display_name, QString log_sqlite_path_tmp, QString log_db_path_recon)
{
    recon_static_functions::app_debug("---Starts-----" ,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(log_sqlite_path_tmp);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + log_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    QString select_command = " SELECT viewer_display_name, db_table_name, file_name, file_path, log_file_data,"
                             " plugin_name, category_name, record_no, creation_timestamp,"
                             " source_count_name ,recon_filefrom , recon_file_infopath from 'log_viewer_index' where viewer_display_name = ?";

    query.prepare(select_command);
    query.addBindValue(viewer_display_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + log_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QStringList arg_list;
    QString db_table_name;
    if(query.next())
    {
        db_table_name = query.value("db_table_name").toString();

        arg_list << query.value("viewer_display_name").toString()
                 << query.value("db_table_name").toString()
                 << query.value("file_name").toString()
                 << query.value("file_path").toString()
                 << query.value("log_file_data").toString()
                 << query.value("plugin_name").toString()
                 << query.value("category_name").toString()
                 << query.value("record_no").toString()
                 << query.value("creation_timestamp").toString()
                 << query.value("source_count_name").toString()
                 << query.value("recon_filefrom").toString()
                 << query.value("recon_file_infopath").toString()
                 << query.value("record_no").toString(); //INT


    }

    destination_db.close();

    QString command = "INSERT INTO 'log_viewer_index'(viewer_display_name,db_table_name,file_name,"
                      "file_path, log_file_data, plugin_name,category_name,record_no,creation_timestamp, "
                      "source_count_name, recon_filefrom,recon_file_infopath,INT ) VALUES(?,?,?,?,?,?, ?,?,?,?,?,?,?)";

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,log_db_path_recon,Q_FUNC_INFO);

    create_log_file_table_in_log_viewer_db(db_table_name, log_db_path_recon);

    recon_static_functions::app_debug("---Ends-----" ,Q_FUNC_INFO);
    return;
}

void MainWindow::create_log_file_table_in_log_viewer_db(QString db_table_name_of_temp, QString log_db_pth_recon)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);

    QString command_create_table = "CREATE TABLE '" + db_table_name_of_temp + "' (INT INTEGER PRIMARY KEY,record_no INTEGER, tagged_data BLOB,"
                                                                              "  cursor_start_index VARCHAR(300), cursor_end_index VARCHAR(300), filepath VARCHAR(300),"
                                                                              "  recon_tag_value VARCHAR(300), colour_value VARCHAR(300))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command_create_table,log_db_pth_recon,Q_FUNC_INFO);

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);

}

void MainWindow::insert_hex_viewer_file_blocks_from_tmp_to_recon(QString current_file_path,QString hex_sqlite_path_tmp,QString hex_db_path_recon)
{
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(hex_sqlite_path_tmp);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + hex_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT file_name,file_path,block_name,block_start_offset,block_end_offset,block_output_path,block_color,start_page_no, viewer_display_name,recon_filefrom, recon_file_infopath FROM hex_viewer_blocks WHERE file_path=?");
    query.addBindValue(current_file_path);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + hex_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase destination_db_recon = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    destination_db_recon.setDatabaseName(hex_db_path_recon);
    if(!destination_db_recon.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + hex_db_path_recon,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db_recon.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    while(query.next())
    {
        QStringList arg_list;

        arg_list << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
                 << query.value(3).toString() << query.value(4).toString() << query.value(5).toString()
                 << query.value(6).toString() << query.value(7).toString() << query.value(8).toString()
                 << query.value(9).toString() << query.value(10).toString() ;

        QString command = "INSERT INTO hex_viewer_blocks(file_name,file_path,block_name,block_start_offset,block_end_offset,block_output_path,block_color,start_page_no, viewer_display_name,recon_filefrom, recon_file_infopath) VALUES(?,?,?,?,?,?,?,?, ?,?,?)";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,arg_list,destination_db_recon,Q_FUNC_INFO);
    }

    destination_db.close();
    destination_db_recon.close();
}


QString MainWindow::insert_hex_viewer_file_entry_from_tmp_to_recon(QString viewer_display_name,QString hex_sqlite_path_tmp,QString hex_db_path_recon)
{
    QString file_path;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(hex_sqlite_path_tmp);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + hex_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return file_path;
    }

    QSqlQuery query(destination_db);
    query.prepare("SELECT viewer_display_name,db_table_name,file_name,file_path,plugin_name,category_name,record_no,creation_timestamp,source_count_name,recon_filefrom, recon_file_infopath FROM hex_viewer_index WHERE viewer_display_name=?");
    query.addBindValue(viewer_display_name);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + hex_sqlite_path_tmp,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return file_path;
    }

    QStringList arg_list;

    while(query.next())
    {
        file_path = query.value(3).toString();
        arg_list << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
                 << query.value(3).toString() << query.value(4).toString() << query.value(5).toString()
                 << query.value(6).toString() << query.value(7).toString() << query.value(8).toString()
                 << query.value(9).toString() << query.value(10).toString() ;
    }

    destination_db.close();

    QString command = "INSERT INTO hex_viewer_index(viewer_display_name,db_table_name,file_name,file_path,plugin_name,category_name,record_no,creation_timestamp, source_count_name,recon_filefrom, recon_file_infopath) VALUES(?,?,?,?,?,?,?,?,?,?,?)";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,hex_db_path_recon,Q_FUNC_INFO);

    return file_path;
}

int MainWindow::add_sqlite_viewer_in_treedisplay(QString destination_db_path)
{
    recon_static_functions::app_debug(" Starts ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return 0;
    }

    QSqlQuery query(destination_db);

    QString command = "select viewer_display_name, savedstate_dest_dir_path from sv_index where viewer_display_name='" + combobox_current_displayed_feature_opened->currentText() + QString("'");
    query.prepare(command);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return 0;
    }

    QString viewer_display_name;
    QString saved_dir_path;

    while(query.next()){
        viewer_display_name = query.value(0).toString();

        saved_dir_path = query.value(1).toString();

    }


    saved_dir_path = saved_dir_path + "/MainDataDisplay_SavedState.sqlite";
    QStringList values;
    values << saved_dir_path;

    QString destination_sqliteview_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QString command2 = "update 'sv_index'set recreated_file_path = ? where viewer_display_name='" + combobox_current_displayed_feature_opened->currentText() + QString("'");

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command2,values, destination_sqliteview_index_db_path,Q_FUNC_INFO);

    common_viewer_extraction_finish_or_cancelled(viewer_display_name,MACRO_CASE_TREE_Sqlite_Viewer);

    destination_db.close();
    recon_static_functions::app_debug(" Ends ",Q_FUNC_INFO);
    return 0;
}
void MainWindow::delete_entries_from_plist_viewer_tmp_db()
{

    return;

    QString temp_dbpath =  plist_viewer_tmp_db_path;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(temp_dbpath);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + temp_dbpath,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QStringList tables_list = destination_db.tables();

    for(int i = 0 ; i < tables_list.size() ; i++)
    {
        QString table = tables_list.at(i);
        if(table == "plist_viewer_index")
        {
            recon_helper_standard_obj->execute_db_command_by_dbreference("delete from 'plist_viewer_index'",destination_db,Q_FUNC_INFO);
            continue;
        }

        recon_helper_standard_obj->execute_db_command_by_dbreference("drop table '" + table + "'",destination_db,Q_FUNC_INFO);

    }

    destination_db.close();
}


void MainWindow::delete_entries_from_hex_viewer_tmp(QString db_path)
{
    QFileInfo info_file(db_path);
    if(!info_file.exists())
        return;

    QString command = "DELETE FROM hex_viewer_blocks";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_path,Q_FUNC_INFO);

    command = "DELETE FROM hex_viewer_index";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_path,Q_FUNC_INFO);
}

void MainWindow::common_viewer_extraction_finish_or_cancelled(QString result_name, QString viewer_name)
{
    QString icon_path;
    int enum_bucket = enum_global_case_tree_display_row_BUCKET ;

    if(viewer_name == MACRO_CASE_TREE_Plist_Viewer)
        icon_path = "../icons/Features/sub_plist_viewer.png";
    else if(viewer_name == MACRO_CASE_TREE_Sqlite_Viewer)
        icon_path = "../icons/Features/sub_sqLite_viewer.png";
    else if(viewer_name == MACRO_CASE_TREE_Hex_Viewer)
        icon_path = "../icons/Features/sub_hex_viewer.png";
    else if(viewer_name == MACRO_CASE_TREE_Registry_Viewer)
        icon_path = "../icons/Features/sub_registry_viewer.png";
    else if(viewer_name == MACRO_CASE_TREE_Log_Viewer)
        icon_path = "../icons/Features/sub_log_viewer.png";


    bool bool_child_added = false;


    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(bool_child_added)
            break;

        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Bucket))
        {
            for(int cc = 0; cc < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->childCount() ; cc++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(cc)->text(enum_global_case_tree_display_column_ZERO) == QString(viewer_name))
                {
                    QTreeWidgetItem *bucket_parent_item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(cc);

                    bool bool_child_exist = false;
                    for(int count = 0; count < bucket_parent_item->childCount(); count++)
                    {
                        QTreeWidgetItem *sub_child = bucket_parent_item->child(count);
                        if(sub_child->text(enum_global_case_tree_display_column_ZERO) == result_name)
                        {
                            bool_child_exist = true;
                            break;
                        }
                    }

                    if(bool_child_exist)
                        break;

                    QTreeWidgetItem *sub_child = new QTreeWidgetItem;
                    sub_child->setText(enum_global_case_tree_display_column_ZERO, result_name);
                    sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
                    sub_child->setText(enum_global_case_tree_display_column_FIRST,viewer_name);
                    sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(cc)->addChild(sub_child);
                    bool_child_added = true;
                    break;
                }
            }
        }
    }
}

void MainWindow::common_tree_add_source_from_fs(struct_GLOBAL_witness_info_source &struct_info)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Source))
        {
            QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES);

            insert_source_entry_in_source_case_tree(item, struct_info);

            break;
        }
    }
}

void MainWindow::insert_source_entry_in_source_case_tree(QTreeWidgetItem *item_received, struct_GLOBAL_witness_info_source &struct_src_info)
{

    QIcon icon_path = global_recon_helper_singular_class_obj->get_qicon_of_source(struct_src_info);


    QTreeWidgetItem *sub_child_item_1_of_1_source = new QTreeWidgetItem;

    //---------------------------//
    if(struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
    {
        /// IF Source is an Image Then Show image partitions as subchild of image
        bool image_name_exists = false;


        QTreeWidgetItem *child_111_item = check_parent_already_exist_and_get_index(item_received, image_name_exists, struct_src_info);

        if(image_name_exists)
        {
            if(child_111_item == NULL)
                return;

            ///get parent source
            sub_child_item_1_of_1_source = child_111_item;
        }
        else
        {
            ///add parent source first

            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_ZERO, struct_src_info.source_main_name);

            struct_GLOBAL_witness_info_root struct_rt_info = global_witness_info_manager_class_obj->pub_get_root_structure_according_source_count_name(struct_src_info.source_count_name);
            sub_child_item_1_of_1_source->setIcon(enum_global_case_tree_display_column_ZERO,global_recon_helper_singular_class_obj->get_qicon_of_root(struct_rt_info));

            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_Parent_Source);
            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_ROOT_COUNT_NAME, struct_src_info.root_count_name);
        }


        ///add child item
        QTreeWidgetItem *image_partition = new QTreeWidgetItem;
        image_partition->setText(enum_global_case_tree_display_column_ZERO, struct_src_info.source_name);
        image_partition->setText(enum_global_case_tree_display_column_FIRST,struct_src_info.source_count_name);
        //  image_partition->setText(enum_global_case_tree_display_column_SECOND, QString(MACRO_CASE_TREE_PARENT_SOURCE));
        image_partition->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
        image_partition->setText(enum_global_case_tree_display_column_ROOT_COUNT_NAME, struct_src_info.root_count_name);

        image_partition->setIcon(enum_global_case_tree_display_column_ZERO,icon_path);


        /// After removeing virtual source path we get only "/"
        image_partition->setText(enum_global_case_tree_display_column_THIRD,"/");

        //  image_partition->setDisabled(true);
        sub_child_item_1_of_1_source->addChild(image_partition);

    }

    item_received->addChild(sub_child_item_1_of_1_source);
}

QTreeWidgetItem *MainWindow::check_parent_already_exist_and_get_index(QTreeWidgetItem *item_main_source, bool &image_name_exists, struct_GLOBAL_witness_info_source struct_src_info)
{
    QTreeWidgetItem *root_src_name_item = NULL;
    bool bool_root_source_exist = false;
    for(int j = 0; j < item_main_source->childCount(); j++)
    {
        root_src_name_item = item_main_source->child(j);

        QString rt_name = root_src_name_item->text(enum_global_case_tree_display_column_ZERO).trimmed();

        QString rt_cnt_name = root_src_name_item->text(enum_global_case_tree_display_column_ROOT_COUNT_NAME).trimmed();

        if(rt_name.contains(" (") && rt_name.contains(")"))
        {
            rt_name.remove(rt_name.lastIndexOf(" ("), QString(rt_name).size());
            rt_name = rt_name.trimmed();
        }

        if(struct_src_info.root_name == rt_name && struct_src_info.root_count_name == rt_cnt_name)
        {
            bool_root_source_exist = true;
            break;
        }

    }

    if((root_src_name_item == NULL || !bool_root_source_exist) && !struct_src_info.bool_source_nested)
    {
        QTreeWidgetItem *root_mmt = NULL;
        return root_mmt;
    }


    image_name_exists = true;


    bool bool_src_type_intrnl_image = false;
    if(struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || struct_src_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
    {
        bool_src_type_intrnl_image = true;

        QString image_parent_name = struct_src_info.source_main_name;

        QTreeWidgetItem *image_parent_name_item = NULL;
        bool bool_image_parent_exist = false;
        for(int kk = 0; kk < root_src_name_item->childCount(); kk++)
        {
            image_parent_name_item = root_src_name_item->child(kk);
            QString parnt_name = image_parent_name_item->text(enum_global_case_tree_display_column_ZERO).trimmed();
            if(parnt_name.contains(" (") && parnt_name.contains(")"))
            {
                parnt_name.remove(parnt_name.lastIndexOf(" ("), QString(parnt_name).size());
                parnt_name = parnt_name.trimmed();
            }

            if(image_parent_name == parnt_name)
            {
                bool_image_parent_exist = true;
                break;
            }
        }


        if(bool_root_source_exist && (image_parent_name == root_src_name_item->text(enum_global_case_tree_display_column_ZERO)))
        {
            return root_src_name_item;
        }
        else if(bool_image_parent_exist)
        {
            return image_parent_name_item;
        }
        else
        {

            QTreeWidgetItem *sub_child_item_1_of_1_source = new QTreeWidgetItem;

            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_ZERO, struct_src_info.source_main_name);

            sub_child_item_1_of_1_source->setIcon(enum_global_case_tree_display_column_ZERO,global_recon_helper_singular_class_obj->get_qicon_of_source(struct_src_info));

            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_Parent_Source);
            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child_item_1_of_1_source->setText(enum_global_case_tree_display_column_ROOT_COUNT_NAME, struct_src_info.root_count_name);

            root_src_name_item->addChild(sub_child_item_1_of_1_source);

            return sub_child_item_1_of_1_source;
        }
    }

    return root_src_name_item;
}

