#include "snapshots.h"
#include "ui_snapshots.h"

snapshots::snapshots(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::snapshots)
{
    ui->setupUi(this);

    // setWindowFlags(Qt::Window| Qt::FramelessWindowHint);

    setWindowTitle(QString(MACRO_Plugin_Name_Snapshots));
    setWindowModality(Qt::ApplicationModal);

    ui->tableWidget_snapshots->installEventFilter(this);
    ui->tableWidget_snapshots->verticalHeader()->setHighlightSections(false);
    ui->tableWidget_snapshots->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_snapshots->horizontalHeader()->setSectionsMovable(true);

    ui->tableWidget_snapshots->hideColumn(enum_source_count_name_column);
    ui->tableWidget_snapshots->hideColumn(enum_fs_run_status_column);

    ui->tableWidget_snapshots->setColumnWidth(enum_snapshot_name_column, 350);
    ui->tableWidget_snapshots->setColumnWidth(enum_snapshot_processed_status_column, 120);

    recon_helper_standard_obj = new recon_helper_standard(this);
    thread_extract_snapshots_obj = new thread_extract_snapshots(this);
    connect(thread_extract_snapshots_obj, SIGNAL(signal_thread_custom_finished()), this, SLOT(slot_thread_snapshot_custom_finished()));

    connect(thread_extract_snapshots_obj, SIGNAL(signal_PBAR_thread_snapshot_fs_extraction_started(QString)), this, SIGNAL(signal_PBAR_snapshot_fs_extraction_started(QString)));
    connect(thread_extract_snapshots_obj, SIGNAL(signal_PBAR_thread_snapshot_file_system_value(QString,QString,bool,qint64,int,bool)), this, SIGNAL(signal_PBAR_snapshot_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_extract_snapshots_obj, SIGNAL(signal_PBAR_thread_snapshot_file_system_finished(QString)), this, SIGNAL(signal_PBAR_snapshot_file_system_finished(QString)));

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    QRegularExpression rx("^[a-zA-Z0-9_ -]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->lineEdit_snapshot_name->setValidator(validator);

    recon_helper_process_obj = new recon_helper_process(this);
    witness_info_store_obj = new witness_info_store(this);

}

snapshots::~snapshots()
{
    delete ui;
}

void snapshots::pub_set_source_count_name_and_source_db_path(QString source_cnt_nm)
{
    source_count_name_str = source_cnt_nm;

    main_source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_str + "/file_system.sqlite";
}

bool snapshots::pub_is_snapshot_thread_running()
{
    return bool_snapshots_thread_running;
}

void snapshots::pub_set_mutex_for_fs_main_db(QMutex *mutex)
{
    mutex_for_fs_main_db = mutex;
}

void snapshots::pub_create_ui()
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);

    ui->tableWidget_snapshots->setRowCount(0);
    ui->lineEdit_snapshot_name->clear();

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);

    for(int row = 0; row < struct_source_info.snapshots_QList.size(); row++)
    {
        struct_GLOBAL_snapshots_info snapshots_info_obj = struct_source_info.snapshots_QList.at(row);

        ui->tableWidget_snapshots->insertRow(row);
        ui->tableWidget_snapshots->setRowHeight(row, 22);

        QString snp_name = snapshots_info_obj.snapshot_actual_name;

        QTableWidgetItem *item_status_already_processed = new QTableWidgetItem;
        item_status_already_processed->setText("Processed");

        checkbox_snapshot_obj = new QCheckBox(this);
        checkbox_snapshot_obj->setText(snp_name);
        checkbox_snapshot_obj->setFocusPolicy(Qt::NoFocus);

        ui->label->hide();
        ui->lineEdit_snapshot_name->setHidden(true);

        ///Get list of already run snapshots from sources db
        QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

        QString cmd_select_snapshot = QString("Select fs_run_status from snapshots Where source_count_name = ? AND snapshot_name = ?");
        QStringList val_list;
        val_list  << source_count_name_str << snp_name;
        QString snapshot_run_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_select_snapshot, val_list, 0, sources_db_path, Q_FUNC_INFO);

        if(snapshot_run_status == QString("1"))
        {
            checkbox_snapshot_obj->setDisabled(true);
            item_status_already_processed->setFlags(Qt::ItemIsEditable);
            ui->tableWidget_snapshots->setItem(row,enum_snapshot_processed_status_column, item_status_already_processed);
        }

        QTableWidgetItem *item_source_count_name = new QTableWidgetItem;
        item_source_count_name->setText(snapshots_info_obj.source_count_name);

        QTableWidgetItem *item_fs_run_status = new QTableWidgetItem;
        item_fs_run_status->setText(snapshots_info_obj.fs_run_status);

        ui->tableWidget_snapshots->setCellWidget(row,enum_snapshot_name_column, checkbox_snapshot_obj);
        ui->tableWidget_snapshots->setItem(row,enum_source_count_name_column, item_source_count_name);
        ui->tableWidget_snapshots->setItem(row,enum_fs_run_status_column, item_fs_run_status);

    }

    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);
}

void snapshots::on_pushButton_cancel_clicked()
{
    this->hide();
}


void snapshots::on_pushButton_difference_clicked()
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);

    for(int ii = 0; ii < ui->tableWidget_snapshots->rowCount(); ii++)
    {
        QCheckBox *widget_checkbox_button_ptr = dynamic_cast<QCheckBox *>(ui->tableWidget_snapshots->cellWidget(ii,0));
        if(widget_checkbox_button_ptr->isChecked())
        {
            selected_snapshot_name_list << widget_checkbox_button_ptr->text();
        }
    }

    selected_snapshot_name_list.removeDuplicates();

    if(selected_snapshot_name_list.size() < 1)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select a snapshot");
        return;
    }

    mount_all_selected_snapshots();

    this->hide();

    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);
}

void snapshots::mount_all_selected_snapshots()
{
    //TakeAt is use to get the fist item evey time and call this fucn again until list size become zero
    selected_snapshot_name = selected_snapshot_name_list.takeAt(0);
    QString difference_name_str = selected_snapshot_name.trimmed();
    selected_snapshot_name = selected_snapshot_name.trimmed();

    snapshot_db_filename = selected_snapshot_name;
    snapshot_db_filename.replace(" ", "_");
    snapshot_db_filename.replace(".", "_");


    difference_name_str.replace(" ", "_");
    difference_name_str.replace(".", "_");
    mount_snapshot_on_difference_clicked(source_count_name_str, selected_snapshot_name, difference_name_str);

}

void snapshots::start_snapshot_compare()
{
    ///compare here after snapshot fs extraction finished

    snapshot_source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_File_System_In_Result_QString).toString() + source_count_name_str + "/" + snapshot_db_filename + ".sqlite";

    save_result_info_and_set_destinations();
    extract_and_save_snapshots_data();

    emit signal_go_to_snapshots_display(saved_snapshot_name);

}

QString snapshots::get_insert_command(QString tble_name , QStringList existing_colname_list)
{
    QStringList colvalues_list;
    for(int ii = 0 ; ii < existing_colname_list.size() ;ii++)
        colvalues_list << "?";

    QString columns_fields = existing_colname_list.join(",");
    QString values_fields = colvalues_list.join(",");


    QString cmd = "insert into " + tble_name + "(" + columns_fields + ") values(" + values_fields + ")";

    return cmd;
}

void snapshots::insert_deleted_data(QSqlDatabase &destination_db)
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);

    QString connection_name = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_name);
    QSqlDatabase main_source_db = QSqlDatabase::addDatabase("QSQLITE",connection_name);
    main_source_db.setDatabaseName(snapshot_source_db_path);
    if(!main_source_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " + main_source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(main_source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }


    QSqlQuery query(main_source_db);

    QString fs_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(main_source_db_path));
    if(!query.exec(fs_db_attach_commmand))
    {
        recon_static_functions::app_debug(" - db attach -------FAILED------ " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() , Q_FUNC_INFO);
        main_source_db.close();
        return;
    }

    //    QString command = ("select * from main.files as a left outer join filesystemDB.files as b ON a.filepath = b.filepath "
    //                       "where b.filepath is null");

    //    QString command = (" SELECT a.filename, a.filepath, a.file_parent_path, a.inode_no, a.file_size, a.extension,"
    //                       " a.date_created, a.date_time_created, a.date_last_modified, a.date_time_last_modified,"
    //                       " a.date_last_read,a.date_time_last_read, a.date_added, a.date_time_added, a.source_count_name,"
    //                       " a.recon_file_infopath, a.is_dir, a.recon_filefrom from main.files as a left outer join filesystemDB.files as b ON a.filepath = b.filepath"
    //                       " WHERE b.filepath is null");

    QString command = (" SELECT a.filename, a.filepath, a.file_parent_path, a.inode_no, a.file_size, a.extension,"
                       " a.z_date_time_change, a.z_date_time_modified,"
                       " a.z_date_time_accessed,a.z_date_time_created, a.source_count_name,"
                       " a.recon_file_infopath, a.is_dir, a.recon_filefrom from main.files as a left outer join filesystemDB.files as b ON a.filepath = b.filepath"
                       " WHERE b.filepath is null");

    QSqlQuery select_qry(main_source_db);
    select_qry.prepare(command);
    if(!select_qry.exec())
    {
        recon_static_functions::app_debug(" - dbpath -" + main_source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_qry.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_qry.executedQuery(), Q_FUNC_INFO);

        QString filesystem_db_dettach_commmand_1 = QString("DETACH DATABASE filesystemDB");
        query.exec(filesystem_db_dettach_commmand_1);

        main_source_db.close();
        return;
    }

    QString table_name = "tbl_deleted_files";

    QSqlQuery insert_qry(destination_db);

    //    QString command_pragma = QString("PRAGMA TABLE_INFO( '") + table_name + "')";
    //    QStringList existing_colname_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_pragma, 1, destination_db, Q_FUNC_INFO);
    //    existing_colname_list.removeAll("INT");

    QStringList existing_colname_list;

    existing_colname_list << "filename" << "filepath" << "file_parent_path" << "inode_no" << "file_size"
                          << "extension" << "z_date_time_change" << "z_date_time_modified" << "z_date_time_accessed" << "z_date_time_created"
                          << "source_count_name" << "recon_file_infopath" << "is_dir" << "recon_filefrom";

    QString insert_command = get_insert_command(table_name ,existing_colname_list);
    int tmp_count = 0;

    while(select_qry.next())
    {
        tmp_count++;
        if(tmp_count % 50 == 0)
        {
            QCoreApplication::processEvents();
            tmp_count = 0;
        }

        insert_qry.prepare(insert_command);

        int tt_count = 0;
        for(int ii = 0 ; ii < existing_colname_list.size() ; ii++)
        {
            tt_count++;
            if(tt_count % 10 == 0)
            {
                QCoreApplication::processEvents();
                tt_count = 0;
            }

            insert_qry.addBindValue(select_qry.value(existing_colname_list.at(ii)).toString());
        }

        if(!insert_qry.exec())
        {
            recon_static_functions::app_debug(" - dbpath -" + destination_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(insert_qry.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(insert_qry.executedQuery(), Q_FUNC_INFO);
            continue ;
        }

    }

    create_and_update_snapshots_col_in_difference_db(destination_db,table_name);

    QString filesystem_db_dettach_commmand_1 = QString("DETACH DATABASE filesystemDB");
    query.exec(filesystem_db_dettach_commmand_1);

    main_source_db.close();

    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);

}

void snapshots::insert_created_data(QSqlDatabase &destination_db)
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);


    QString connection_name = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_name );
    QSqlDatabase main_source_db = QSqlDatabase::addDatabase("QSQLITE", connection_name );
    main_source_db.setDatabaseName(main_source_db_path);
    if(!main_source_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + main_source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + main_source_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_name);
        return;
    }

    QSqlQuery query_2(main_source_db);

    QString fs_db_attach_commmand_2 = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(snapshot_source_db_path));
    if(!query_2.exec(fs_db_attach_commmand_2))
    {
        recon_static_functions::app_debug(" Db attach --------FAILED------ " + query_2.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_2.lastError().text(),Q_FUNC_INFO);
        main_source_db.close();
        return;
    }


    QSqlQuery select_qry(main_source_db);

    QString command = ("select * from main.files as a left outer join filesystemDB.files as b ON a.filepath = b.filepath "
                       "where b.filepath is null");

    select_qry.prepare(command);
    if(!select_qry.exec())
    {
        recon_static_functions::app_debug(" - dbpath -" + main_source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_qry.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_qry.executedQuery(), Q_FUNC_INFO);

        QString filesystem_db_dettach_commmand_2 = QString("DETACH DATABASE filesystemDB");
        query_2.exec(filesystem_db_dettach_commmand_2);

        main_source_db.close();
        return;
    }


    QString table_name = "tbl_created_files";

    QSqlQuery insert_qry(destination_db);
    QString command_pragma = QString("PRAGMA TABLE_INFO( '") + table_name + "')";
    QStringList existing_colname_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_pragma, 1, destination_db, Q_FUNC_INFO);
    existing_colname_list.removeAll("INT");

    QString insert_command = get_insert_command(table_name ,existing_colname_list);

    int tmp_count = 0;
    while(select_qry.next())
    {
        tmp_count++;
        if(tmp_count % 50 == 0)
        {
            QCoreApplication::processEvents();
            tmp_count = 0;
        }

        insert_qry.prepare(insert_command);
        int tt_count = 0;
        for(int ii = 0 ; ii < existing_colname_list.size() ; ii++)
        {
            tt_count++;
            if(tt_count % 10 == 0)
            {
                QCoreApplication::processEvents();
                tt_count = 0;
            }
            insert_qry.addBindValue(select_qry.value(existing_colname_list.at(ii)).toString());
        }

        if(!insert_qry.exec())
        {
            recon_static_functions::app_debug(" - dbpath -" + destination_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(insert_qry.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(insert_qry.executedQuery(), Q_FUNC_INFO);
            continue ;
        }
    }

    create_and_update_snapshots_col_in_difference_db(destination_db,table_name);

    QString filesystem_db_dettach_commmand_2 = QString("DETACH DATABASE filesystemDB");
    query_2.exec(filesystem_db_dettach_commmand_2);

    main_source_db.close();

    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);

}

void snapshots::insert_modified_data(QSqlDatabase &destination_db)
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);

    QString connection_name = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_name );
    QSqlDatabase main_source_db = QSqlDatabase::addDatabase("QSQLITE", connection_name );
    main_source_db.setDatabaseName(snapshot_source_db_path);
    if(!main_source_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + main_source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + main_source_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_name);
        return;
    }

    QSqlQuery query_2(main_source_db);

    QString fs_db_attach_commmand_2 = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(main_source_db_path));
    if(!query_2.exec(fs_db_attach_commmand_2))
    {
        recon_static_functions::app_debug(" Db attach --------FAILED------ " + query_2.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_2.lastError().text(),Q_FUNC_INFO);
        main_source_db.close();
        return;
    }


    QSqlQuery select_qry(main_source_db);

    //    QString command = ("select * from main.files as a left outer join filesystemDB.files as b ON a.filepath = b.filepath "
    //                       "where a.filepath = b.filepath AND a.date_time_last_modified is not b.date_time_last_modified AND a.date_time_last_modified is NOT NULL");


    QString command = (" SELECT a.filename, a.filepath, a.file_parent_path, a.inode_no, a.file_size, a.extension,"
                       " a.z_date_time_change, a.z_date_time_modified, a.z_date_time_accessed, a.z_date_time_created, a.source_count_name,"
                       " a.recon_file_infopath, a.is_dir, a.recon_filefrom from main.files as a left outer join filesystemDB.files as b ON a.filepath = b.filepath"
                       " WHERE a.filepath = b.filepath AND a.z_date_time_modified is not b.z_date_time_modified AND a.z_date_time_modified is NOT NULL");

    select_qry.prepare(command);
    if(!select_qry.exec())
    {
        recon_static_functions::app_debug(" - dbpath -" + main_source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_qry.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_qry.executedQuery(), Q_FUNC_INFO);

        QString filesystem_db_dettach_commmand_2 = QString("DETACH DATABASE filesystemDB");
        query_2.exec(filesystem_db_dettach_commmand_2);

        main_source_db.close();
        return;
    }


    QString table_name = "tbl_modified_files";
    QSqlQuery insert_qry(destination_db);

    //    QString command_pragma = QString("PRAGMA TABLE_INFO( '") + table_name + "')";
    //    QStringList existing_colname_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_pragma, 1, destination_db, Q_FUNC_INFO);
    //    existing_colname_list.removeAll("INT");

    QStringList existing_colname_list;

    existing_colname_list << "filename" << "filepath" << "file_parent_path" << "inode_no" << "file_size"
                          << "extension" << "z_date_time_change" << "z_date_time_modified" << "z_date_time_accessed" << "z_date_time_created"
                          << "source_count_name" << "recon_file_infopath" << "is_dir" << "recon_filefrom";

    QString insert_command = get_insert_command(table_name ,existing_colname_list);

    int tmp_count = 0;
    while(select_qry.next())
    {
        tmp_count++;
        if(tmp_count % 50 == 0)
        {
            QCoreApplication::processEvents();
            tmp_count = 0;
        }

        insert_qry.prepare(insert_command);

        int tt_count = 0;
        for(int ii = 0 ; ii < existing_colname_list.size() ; ii++)
        {
            tt_count++;
            if(tt_count % 10 == 0)
            {
                QCoreApplication::processEvents();
                tt_count = 0;
            }
            insert_qry.addBindValue(select_qry.value(existing_colname_list.at(ii)).toString());
        }


        if(!insert_qry.exec())
        {
            recon_static_functions::app_debug(" - dbpath -" + destination_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(insert_qry.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(insert_qry.executedQuery(), Q_FUNC_INFO);
            continue ;
        }
    }

    create_and_update_snapshots_col_in_difference_db(destination_db,table_name);

    QString filesystem_db_dettach_commmand_2 = QString("DETACH DATABASE filesystemDB");
    query_2.exec(filesystem_db_dettach_commmand_2);

    main_source_db.close();

    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);
}

void snapshots::extract_and_save_snapshots_data()
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);


    QSqlDatabase destination_db;
    QString connection_name = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_name);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",connection_name);
    destination_db.setDatabaseName(snapshots_difference_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    destination_db.transaction();
    mutex_for_fs_main_db->lock();

    insert_deleted_data(destination_db);

    // insert_created_data(destination_db);

    insert_modified_data(destination_db);
    mutex_for_fs_main_db->unlock();

    destination_db.commit();
    destination_db.close();

    emit signal_add_snapshot_in_case_tree(saved_snapshot_name);

    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);
}

void snapshots::insert_in_index_db(QString snapshot_display_name)
{
    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);


    ///update in snapshot differences index db
    QString snapshots_difference_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + "saved_snapshots_index.sqlite";

    QString cmd_save_index = QString("Insert into tbl_saved_index(snapshot_lable_name, primary_source,secondary_source,source_count_name) Values(?,?,?,?)");
    QStringList args_list;
    args_list << snapshot_display_name << source_count_name_str << selected_snapshot_name << source_count_name_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_save_index, args_list, snapshots_difference_index_db_path, Q_FUNC_INFO);


    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);
}

void snapshots::save_result_info_and_set_destinations()
{

    recon_static_functions::app_debug(" - start -" , Q_FUNC_INFO);

    QString snapshot_diff_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString();

    QString snapshot_name  = difference_label_str;

    QString snapshot_name_with_sqlite_extsn = recon_static_functions::get_available_filename(snapshot_name + ".sqlite" ,snapshot_diff_result_dir_path , Q_FUNC_INFO );


    QString snapshot_display_name = snapshot_name_with_sqlite_extsn;
    snapshot_display_name.chop(QString(".sqlite").size());

    insert_in_index_db(snapshot_display_name);

    saved_snapshot_name = snapshot_display_name;
    snapshots_difference_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_name_with_sqlite_extsn;

    QString fs_db_command1 = recon_static_functions::get_filesystem_db_create_command("tbl_created_files");
    recon_helper_standard_obj->execute_db_command_by_dbpath(fs_db_command1, snapshots_difference_db_path, Q_FUNC_INFO);

    QString fs_db_command2 = recon_static_functions::get_filesystem_db_create_command("tbl_deleted_files");
    recon_helper_standard_obj->execute_db_command_by_dbpath(fs_db_command2, snapshots_difference_db_path, Q_FUNC_INFO);

    QString fs_db_command3 = recon_static_functions::get_filesystem_db_create_command("tbl_modified_files");
    recon_helper_standard_obj->execute_db_command_by_dbpath(fs_db_command3, snapshots_difference_db_path, Q_FUNC_INFO);

    recon_helper_standard_obj->execute_db_command_by_dbpath(recon_static_functions::get_exportfilelist_command() ,snapshots_difference_db_path,Q_FUNC_INFO);


    recon_static_functions::app_debug(" - end -" , Q_FUNC_INFO);

}

void snapshots::mount_snapshot_on_difference_clicked(QString source_count_name, QString selected_snapshot_str, QString difference_name_str)
{

    struct_GLOBAL_witness_info_source struct_target_info_ptr = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    difference_label_str = difference_name_str;


    /// mount selected snapshot
    QString snapshot_mount_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_APFS_Snapshot_Dir_QString).toString();

    QDir tmp_dir;
    tmp_dir.mkpath(snapshot_mount_dir);

    QString available_mount_point = recon_static_functions::get_available_filename("snapshot_mount", snapshot_mount_dir, Q_FUNC_INFO);

    QString snapshot_mount_path = snapshot_mount_dir + available_mount_point + "/";
    QDir mdir;
    mdir.mkpath(snapshot_mount_path);

    struct_global_desktop_info struct_mchne_info_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
    QString mac_version = struct_mchne_info_obj.os_version_full;

//    if(mac_version.contains("10.15") || mac_version.contains("10.16") || mac_version.startsWith("11") || mac_version.startsWith("12"))
//    {
//        selected_snapshot_str  = selected_snapshot_str.remove(".local");
//    }

    // mount -t apfs -o -s=com.apple.TimeMachine.2019-03-15-124935 / ~/tmp/snap_mount/
    QStringList args_list;

    args_list << "-t" << MACRO_FILE_SYSTEM_TYPE_APFS << "-o" << QString("-s=") + selected_snapshot_str << struct_target_info_ptr.virtual_source_path << snapshot_mount_path ;
    QString mount_out = recon_helper_process_obj->run_command_with_arguments("mount", args_list, Q_FUNC_INFO);

    if(!global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(snapshot_mount_path, Q_FUNC_INFO))
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Snapshot '" + selected_snapshot_str + "' mounting failed.");
        return;
    }

    ///=======Update Mounted Path in Sources List=======///

    if(struct_target_info_ptr.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
    {
        QString new_snapshot_mount_path = recon_helper_standard_obj->get_new_mount_path_of_time_machine_backup_image_and_snapshots(snapshot_mount_path,QString(MACRO_Mount_Path_Time_Machine_Backup_Image_Snapshots));
        snapshot_mount_path = new_snapshot_mount_path;
    }

    QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int jj = 0; jj < list_source_info.size(); jj++)
    {
        bool bool_current_src_found = false;

        struct_GLOBAL_witness_info_source src_info_obj = list_source_info.takeAt(jj);
        if(src_info_obj.source_count_name == source_count_name)
        {
            bool_current_src_found = true;

            for(int cc = 0; cc < src_info_obj.snapshots_QList.size(); cc++)
            {
                struct_GLOBAL_snapshots_info snapshots_info_obj =  src_info_obj.snapshots_QList.takeAt(cc);
                if(snapshots_info_obj.snapshot_actual_name == selected_snapshot_str)
                {
                    snapshots_info_obj.snapshot_mount_path = snapshot_mount_path;
                }
                src_info_obj.snapshots_QList.insert(cc, snapshots_info_obj);
            }
        }
        list_source_info.insert(jj, src_info_obj);

        if(bool_current_src_found)
            break;
    }
    global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info);



    thread_extract_snapshots_obj->pub_set_source_count_name(source_count_name);
    thread_extract_snapshots_obj->pub_set_selected_snapshot_name(selected_snapshot_str, snapshot_db_filename);
    thread_extract_snapshots_obj->pub_set_snapshot_thread_running_status(true);
    thread_extract_snapshots_obj->pub_set_snapshot_mount_path(snapshot_mount_path);
    bool_snapshots_thread_running = true;

    thread_extract_snapshots_obj->start();
}

void snapshots::create_and_update_snapshots_col_in_difference_db(QSqlDatabase &dest_db , QString table_name)
{

    QString alter_cmd = QString("ALTER TABLE '" + table_name + "' ADD actual_snapshot_name varchar(300)");
    recon_helper_standard_obj->execute_db_command_by_dbreference(alter_cmd, dest_db, Q_FUNC_INFO);


    QString update_cmd = QString("UPDATE '" + table_name + "' set actual_snapshot_name  = ?");
    QStringList arg_list = QStringList(selected_snapshot_name);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_cmd, arg_list,dest_db, Q_FUNC_INFO);
}

void snapshots::pub_unmount_all_snapshots()
{
    QString snap_mnt_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_APFS_Snapshot_Dir_QString).toString();
    QDirIterator iter(snap_mnt_path , QDir::AllDirs | QDir::NoDotAndDotDot);
    while(iter.hasNext())
    {
        QFileInfo info(iter.next());
        QString file_path = info.filePath();

        QStringList arg;
        arg << "umount" << file_path;

        recon_helper_process_obj->run_command_with_arguments("diskutil",arg,Q_FUNC_INFO);

        if(global_recon_helper_singular_class_obj->is_it_mounted_path_by_exact_match(file_path, Q_FUNC_INFO))
            continue;

        recon_static_functions::do_directory_blank_completely(file_path, Q_FUNC_INFO);

    }
    recon_static_functions::do_directory_blank_completely(snap_mnt_path, Q_FUNC_INFO);

}

void snapshots::slot_thread_snapshot_custom_finished()
{
    thread_extract_snapshots_obj->pub_set_snapshot_thread_running_status(false);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please wait, Processing Snapshot...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    witness_info_store_obj->pub_update_fs_run_status_snapshots_db(selected_snapshot_name, source_count_name_str, "1");
    witness_info_store_obj->pub_update_key_with_new_value_in_snapshots_plist(selected_snapshot_name, source_count_name_str, QString(MACRO_APFS_Snapshot_Plist_Key_FS_Run_Status), QString("1"));

    start_snapshot_compare();

    bool_snapshots_thread_running = false;

    if(selected_snapshot_name_list.size() > 0)
    {
        mount_all_selected_snapshots();
    }
    else
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
    }

}
