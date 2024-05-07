#include "export_case.h"
#include "ui_export_case.h"

export_case::export_case(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_case)
{
    ui->setupUi(this);

    setWindowTitle(QString(MACRO_Export_Case_Parent));

    ui->stackedWidget->setCurrentIndex(0);
    ui->radioButton_quick_mode->setChecked(true);

    QString placeholder_text = getenv("HOME") + QString("/Desktop");
    ui->lineEdit_destination->setPlaceholderText(placeholder_text);

    ///quick_mode_category
    ui->tableWidget_quick_mode_category->setHorizontalHeaderLabels(QStringList() << MACRO_LOCAL_EXPORT_CASE_LABEL_CATEGORY);
    ui->tableWidget_quick_mode_category->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_quick_mode_category->setAlternatingRowColors(true);
    ui->tableWidget_quick_mode_category->setGridStyle(Qt::NoPen);
    ui->tableWidget_quick_mode_category->setShowGrid(false);
    ui->tableWidget_quick_mode_category->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_quick_mode_category->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_quick_mode_category->setAutoScroll(true);

    ///expert_mode_tag_list
    ui->tableWidget_expert_mode_tag_list->setHorizontalHeaderLabels(QStringList() << MACRO_LOCAL_EXPORT_CASE_LABEL_TAGS);
    ui->tableWidget_expert_mode_tag_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_expert_mode_tag_list->setAlternatingRowColors(true);
    ui->tableWidget_expert_mode_tag_list->setGridStyle(Qt::NoPen);
    ui->tableWidget_expert_mode_tag_list->setShowGrid(false);
    ui->tableWidget_expert_mode_tag_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_expert_mode_tag_list->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_expert_mode_tag_list->setAutoScroll(true);

    ///quick_mode_list
    ui->tableWidget_quick_mode_list->setHorizontalHeaderLabels(QStringList() << MACRO_LOCAL_EXPORT_CASE_LABEL_LIST);
    ui->tableWidget_quick_mode_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_quick_mode_list->setAlternatingRowColors(true);
    ui->tableWidget_quick_mode_list->setGridStyle(Qt::NoPen);
    ui->tableWidget_quick_mode_list->setShowGrid(false);
    ui->tableWidget_quick_mode_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_quick_mode_list->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_quick_mode_list->setAutoScroll(true);

    recon_helper_standard_obj = new recon_helper_standard(this);

    export_rsync_obj = new export_rsync(this);

}

export_case::~export_case()
{
    delete ui;
}

void export_case::pub_prepare_display()
{ // prepare display of ui of export case in both category like quick and expert mode
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_obj->show();


    prepare_quick_mode_display();
    prepare_expert_mode_display();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_quick_mode_display()
{ // prepare display in quick mode export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    ui->tableWidget_quick_mode_category->setRowCount(0);
    ui->tableWidget_quick_mode_list->setRowCount(0);

    QTableWidgetItem *item_internet_history = new QTableWidgetItem;
    QTableWidgetItem *item_messages = new QTableWidgetItem;
    QTableWidgetItem *item_emails = new QTableWidgetItem;
    QTableWidgetItem *item_images = new QTableWidgetItem;
    QTableWidgetItem *item_multimedia = new QTableWidgetItem;
    QTableWidgetItem *item_documents = new QTableWidgetItem;
    QTableWidgetItem *item_other_artifacts = new QTableWidgetItem;

    item_internet_history->setCheckState(Qt::Checked);
    item_internet_history->setText(QString(MACRO_Export_Case_Category_Internet_History));

    int row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_internet_history);

    item_messages->setCheckState(Qt::Checked);
    item_messages->setText(QString(MACRO_Export_Case_Category_Messages));

    row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_messages);

    item_emails->setCheckState(Qt::Checked);
    item_emails->setText(QString(MACRO_Export_Case_Category_Emails));

    row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_emails);

    item_images->setCheckState(Qt::Checked);
    item_images->setText(QString(MACRO_Export_Case_Category_Image));

    row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_images);

    item_multimedia->setCheckState(Qt::Checked);
    item_multimedia->setText(QString(MACRO_Export_Case_Category_Multimedia));

    row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_multimedia);

    item_documents->setCheckState(Qt::Checked);
    item_documents->setText(QString(MACRO_Export_Case_Category_Document));

    row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_documents);

    item_other_artifacts->setCheckState(Qt::Unchecked);
    item_other_artifacts->setText(QString(MACRO_Export_Case_Category_Other_Artifacts));

    row = ui->tableWidget_quick_mode_category->rowCount();
    ui->tableWidget_quick_mode_category->insertRow(row);
    ui->tableWidget_quick_mode_category->setItem(row,enum_tablewidget_quick_mode_category_items_column_zero_category,item_other_artifacts);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_expert_mode_display()
{ // prepare display of expert mode export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    /// get all updated tags list from tag search db
    QString tag_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";

    QString command = "SELECT tag_name FROM tag_search_index";

    QStringList updated_tags_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,tag_dir_path,Q_FUNC_INFO);

    /// get tag list from export status db
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Tags;

    QStringList tag_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(tag_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - Empty tags list", Q_FUNC_INFO);
        return;
    }

    tag_list.removeDuplicates();

    if(updated_tags_name_list.size() < tag_list.size())
    {
        QStringList new_tags_list;
        for(int jj = 0; jj < updated_tags_name_list.size(); jj++)
        {
            if(tag_list.contains(updated_tags_name_list.at(jj)))
                continue;

            new_tags_list.append(updated_tags_name_list.at(jj));
            tag_list.append(updated_tags_name_list.at(jj));
        }

        if(!new_tags_list.isEmpty())
        {
            QString insert_cmd = "INSERT into 'tbl_export_status' (category_name, list_name, check_status) VALUES(?,?,?)";

            QStringList values;

            for(int ii = 0; ii < new_tags_list.size(); ii++)
            {
                values.clear();
                values << MACRO_Export_Case_Category_Tags << new_tags_list.at(ii) << "1";

                check_state_list.append("1");

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, export_status_db_path, Q_FUNC_INFO);
            }
        }

    }
    prepare_tags_display(tag_list ,check_state_list);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_tags_display(QStringList tag_list ,QStringList check_state_list)
{ // prepare tags display when we up export feature
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    ui->tableWidget_expert_mode_tag_list->setRowCount(0);
    for(int ii = 0; ii < tag_list.size(); ii++)
    {
        QString tag_str = tag_list.at(ii);

        int row = ui->tableWidget_expert_mode_tag_list->rowCount();
        ui->tableWidget_expert_mode_tag_list->insertRow(row);

        QTableWidgetItem *item_tag_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_tag_str->setCheckState(Qt::Checked);
        else
            item_tag_str->setCheckState(Qt::Unchecked);

        item_tag_str->setText(tag_str);

        ui->tableWidget_expert_mode_tag_list->setItem(row, 0, item_tag_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void export_case::pub_set_display_loading_progress_bar(display_loading_progress_bar *obj)
{ // loading progress bar object
    display_loading_progress_bar_obj = obj;
}

void export_case::on_pushButton_select_output_clicked()
{ // destination/ output button clicked
    QString destination_dir = QFileDialog::getExistingDirectory(this, MACRO_Export_Case_Parent);

    ui->lineEdit_destination->setText(destination_dir);

    ui->lineEdit_destination->setReadOnly(true);
}

void export_case::on_pushButton_export_clicked()
{ // export button clicked to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    struct_export_case_artifacts_info_QList.clear();
    struct_file_system_export_tagged_files_info_QList.clear();

    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";
    QStringList values;
    QStringList plugins_list;
    QStringList fs_mod_list;
    QStringList tags_list;

    if(ui->radioButton_quick_mode->isChecked())
    {
        ///quick mode filtered plugins list
        QString plugins_select_cmd = "SELECT list_name from tbl_export_status WHERE check_status = '1' AND (category_name LIKE ? OR category_name LIKE ? OR category_name LIKE ? OR category_name LIKE ?)";

        values.clear();
        values << MACRO_Export_Case_Category_Internet_History << MACRO_Export_Case_Category_Messages << MACRO_Export_Case_Category_Emails << MACRO_Export_Case_Category_Other_Artifacts;

        plugins_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(plugins_select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);

        ///file system related list
        QString select_cmd = "SELECT list_name from tbl_export_status WHERE check_status = '1' AND (category_name LIKE ? OR category_name LIKE ? OR category_name LIKE ?)";
        values.clear();

        values << MACRO_Export_Case_Category_Image << MACRO_Export_Case_Category_Multimedia << MACRO_Export_Case_Category_Document;

        fs_mod_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);

        if(plugins_list.size() <=0 && fs_mod_list.size() <=0)
        {
            QMessageBox::information(this, "Info", "Please select at least one item.");
            return;
        }
    }
    else if(ui->radioButton_expert_mode->isChecked())
    {
        ///tag list
        QString tag_select_cmd = "SELECT list_name from tbl_export_status WHERE check_status = '1' AND category_name LIKE ?";
        values.clear();

        values << MACRO_Export_Case_Category_Tags;

        tags_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(tag_select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);

        if(tags_list.size() <=0)
        {
            QMessageBox::information(this, "Info", "Please select at least one item.");
            return;
        }
    }


    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QString export_case_dir;
    if(ui->lineEdit_destination->text().trimmed().isEmpty())
    {
        export_case_dir = ui->lineEdit_destination->placeholderText().trimmed();
    }
    else
    {
        export_case_dir = ui->lineEdit_destination->text().trimmed();
    }

    QString temp_dir_name = QString("Export_Case");

    QString final_dir_name = recon_static_functions::get_available_filename(temp_dir_name,export_case_dir, Q_FUNC_INFO);

    export_case_dir = export_case_dir + "/" + final_dir_name;

    QDir mkdir;
    mkdir.mkpath(export_case_dir);

    QString installer_path = export_case_dir + "/";
    export_case_dir = export_case_dir + "/" + QString(MACRO_LOCAL_EXPORT_CASE_DIR_CASE_DATA);
    mkdir.mkpath(export_case_dir);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QDir result_dir(result_dir_path);

    QStringList entry_lists;
    if(result_dir.exists())
    {
        entry_lists = result_dir.entryList();
    }

    if(entry_lists.isEmpty())
    {
        display_loading_progress_bar_obj->hide();
        recon_static_functions::app_debug("Ends - Entry list empty", Q_FUNC_INFO);
        return;
    }

    for(int ii = 0; ii < entry_lists.size(); ii++)
    {
        if(entry_lists.at(ii) == "." || entry_lists.at(ii) == "..")
            continue;

        QString sou_dir_path = result_dir_path + "/" + entry_lists.at(ii);

        sou_dir_path.replace("//","/");

        QString dest_dir_path = export_case_dir;

        dest_dir_path.replace("//","/");

        bool_is_copy_finished = false;
        QProcess copy_proc;
        connect(&copy_proc, SIGNAL(finished(int)), this, SLOT(slot_proc_copy_finished(int)));

        //        QString copy_cmd = "cp -rf \"" + sou_dir_path + "\" \"" + dest_dir_path + "\"";
        //        copy_proc.start(copy_cmd);

        QString extrct_contnt_parent = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Parent_QString).toString();

        QString extrct_contnt_andrd_bk = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();

        QString extrct_contnt_ios_bkup = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();

        QString copy_cmd = "rsync";
        QStringList arg;

        QString extrct_contnt_parent_dir_name = QDir(extrct_contnt_parent).dirName();

        if(entry_lists.at(ii) == extrct_contnt_parent_dir_name)
        {
            arg << "-a" << "--exclude=/" + extrct_contnt_parent_dir_name +"/"+ QDir(extrct_contnt_andrd_bk).dirName() << "--exclude=/" + extrct_contnt_parent_dir_name + "/" + QDir(extrct_contnt_ios_bkup).dirName()
                << sou_dir_path << dest_dir_path;
        }
        else
        {
            arg << "-a" << sou_dir_path << dest_dir_path;
        }

        copy_proc.start(copy_cmd,arg);

        while(1)
        {
            if(bool_is_copy_finished)
            {
                copy_proc.close();
                break;
            }
            QCoreApplication::processEvents();
        }

    }

    QString files_dir_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files);
    QDir files_dir(files_dir_path);
    if(!files_dir.exists())
        files_dir.mkpath(files_dir_path);

    QString lab_plugins_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS);

    QDir plugins_dir(lab_plugins_path);

    if(plugins_dir.exists())
    {
        QDirIterator it(plugins_dir, QDirIterator::Subdirectories);

        while(it.hasNext())
        {
            QCoreApplication::processEvents();

            QString path = it.next();

            if(path.endsWith(".sqlite"))
            {
                QString connection_naam = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);

                QSqlDatabase plugins_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
                plugins_db.setDatabaseName(path);
                if(!plugins_db.open())
                {
                    recon_static_functions::app_debug(" plugins_db open  --------FAILED------ " + plugins_db.databaseName(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + plugins_db.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                QStringList tables_list = plugins_db.tables();
                plugins_db.close();

                for(int ii = 0; ii < tables_list.size(); ii++)
                {
                    QString delete_cmd = "DELETE from " + tables_list.at(ii);

                    recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd, path, Q_FUNC_INFO);
                }
            }
        }
    }

    QString file_system_feature_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features) + QString(MACRO_LOCAL_EXPORT_CASE_DIR_FILE_SYSTEM);
    QDir feat_dir(file_system_feature_path);

    if(feat_dir.exists())
    {
        QDirIterator it(feat_dir, QDirIterator::Subdirectories);

        while(it.hasNext())
        {
            QCoreApplication::processEvents();

            QString path = it.next();

            if(path.endsWith("file_system.sqlite"))
            {
                QString delete_cmd = "DELETE from files";

                recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd, path, Q_FUNC_INFO);
            }
        }
    }

    QStringList source_count_name_QStringList;
    QStringList table_name_QStringList_tablewidget;
    QList<QStringList> export_path_column_name_QList_tablewidget;


    //==========================================Tagged Data Export-START==========================================//
    if(!tags_list.isEmpty())
    {
        for(int ii = 0; ii < tags_list.size(); ii++)
        {
            QString db_path;
            if(tags_list.at(ii) == MACRO_Tag_Name_Bookmarks)
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
            else
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tags_list.at(ii) + ".sqlite";

            if(!QFile::exists(db_path))
            {
                continue;
            }

            QString connection_naam = QString(Q_FUNC_INFO);

            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase tags_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            tags_db.setDatabaseName(db_path);
            if(!tags_db.open())
            {
                recon_static_functions::app_debug(" tags_db open  --------FAILED------ " + tags_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + tags_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            QSqlQuery query(tags_db);
            QString cmd = "SELECT record_no, plugin_name, source_count_name, item0, item1, category, tab_name, table_name from tags";
            query.prepare(cmd);

            if(!query.exec())
            {
                recon_static_functions::app_debug(" query - tags_db--------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            while(query.next())
            {
                QCoreApplication::processEvents();

                if(query.value("category").toString() == "File System")
                {
                    struct_global_export_file_system_record obj;
                    obj.record_no = query.value("record_no").toString();
                    obj.source_count_name = query.value("source_count_name").toString();
                    obj.display_file_path = query.value("item1").toString();
                    obj.displayed_file_name = query.value("item0").toString();
                    struct_file_system_export_tagged_files_info_QList.append(obj);

                }
                else if(query.value("category").toString() == "Plugin")
                {
                    QString plugin_name = query.value("plugin_name").toString().replace(" ", "_");

                    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_name;
                    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + plugin_name + ".sqlite" ;

                    struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_full_obj->read_csv_value(query.value("plugin_name").toString());

                    table_name_QStringList_tablewidget.clear();
                    export_path_column_name_QList_tablewidget.clear();

                    table_name_QStringList_tablewidget = obj.table_name_QStringList_tablewidget;

                    export_path_column_name_QList_tablewidget  = obj.export_path_column_name_QList_tablewidget;

                    int table_index = table_name_QStringList_tablewidget.indexOf(query.value("table_name").toString());
                    QStringList export_col_name_list = export_path_column_name_QList_tablewidget.at(table_index);

                    struct_export_case_artifacts_info export_case_artifacts_obj;

                    export_case_artifacts_obj.record_no = query.value("record_no").toString();
                    export_case_artifacts_obj.plugin_name = plugin_name;
                    export_case_artifacts_obj.table_name = query.value("table_name").toString();
                    export_case_artifacts_obj.source_count_name = query.value("source_count_name").toString();

                    struct_export_case_artifacts_info_QList.append(export_case_artifacts_obj);

                    struct_global_artifacts_export_fileinfo art_obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugins_destination_db_file,export_col_name_list,query.value("table_name").toString(),query.value("record_no").toString(),Q_FUNC_INFO);

                    QStringList all_path_list = art_obj.complete_export_filepaths;
                    QStringList display_f_path_list = art_obj.displayed_file_path;

                    for(int i =0 ; i< all_path_list.size() ; i++)
                    {
                        QString complete_path = all_path_list.at(i);

                        QString display_f_path = display_f_path_list.at(i);

                        QString file_export_dir_path = files_dir_path + "/" + query.value("source_count_name").toString() + "/" + display_f_path;

                        QString dest_path(QFileInfo(file_export_dir_path).absolutePath());

                        QDir mdir(dest_path);
                        if(!mdir.exists())
                            mdir.mkpath(dest_path);

                        if(bool_checkbox_blur_status)
                        {
                            export_case_with_blur_effect_on_image(complete_path, dest_path, display_f_path);
                        }
                        else
                        {
                            export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path, dest_path, QFileInfo(display_f_path).fileName(), Q_FUNC_INFO);
                        }
                    }

                }

            }
            tags_db.close();

        }
    }
    //==========================================Tagged Data Export-End==========================================//


    //==========================================Plugins Data Export-START==========================================//
    if(!plugins_list.isEmpty())
    {
        for(int ii = 0 ; ii < plugins_list.size(); ii++)
        {
            QString plugin_name = plugins_list.at(ii);
            plugin_name.replace(" ", "_");
            QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_name;
            QString plugins_destination_db_file = destination_plugin_dir_path + "/" + plugin_name + ".sqlite";

            struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_full_obj->read_csv_value(plugins_list.at(ii));

            table_name_QStringList_tablewidget.clear();
            export_path_column_name_QList_tablewidget.clear();

            table_name_QStringList_tablewidget = obj.table_name_QStringList_tablewidget;

            export_path_column_name_QList_tablewidget  = obj.export_path_column_name_QList_tablewidget;

            for(int ii = 0; ii < table_name_QStringList_tablewidget.size(); ii++)
            {
                QString connection_naam = QString(Q_FUNC_INFO);

                QSqlDatabase::removeDatabase(connection_naam);
                QSqlDatabase artifacts_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                artifacts_db.setDatabaseName(plugins_destination_db_file);
                if(!artifacts_db.open())
                {
                    recon_static_functions::app_debug(" artifacts_db open  --------FAILED------ " + artifacts_db.databaseName(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + artifacts_db.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                QSqlQuery query(artifacts_db);
                QString cmd = "SELECT INT, source_count_name from " + table_name_QStringList_tablewidget.at(ii);
                query.prepare(cmd);

                if(!query.exec())
                {
                    recon_static_functions::app_debug("query - artifacts_db --------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                while(query.next())
                {

                    int table_index = table_name_QStringList_tablewidget.indexOf(table_name_QStringList_tablewidget.at(ii));
                    QStringList export_col_name_list = export_path_column_name_QList_tablewidget.at(table_index);

                    struct_export_case_artifacts_info export_case_artifacts_obj;

                    export_case_artifacts_obj.record_no = query.value("INT").toString();
                    export_case_artifacts_obj.plugin_name = plugin_name;
                    export_case_artifacts_obj.table_name = table_name_QStringList_tablewidget.at(ii);
                    export_case_artifacts_obj.source_count_name = query.value("source_count_name").toString();

                    struct_export_case_artifacts_info_QList.append(export_case_artifacts_obj);

                    struct_global_artifacts_export_fileinfo art_obj = recon_helper_standard_obj->get_export_path_list_and_display_path_list_for_plugin_current_record(plugins_destination_db_file,export_col_name_list,table_name_QStringList_tablewidget.at(ii),query.value("INT").toString(),Q_FUNC_INFO);

                    QStringList all_path_list = art_obj.complete_export_filepaths;
                    QStringList display_f_path_list = art_obj.displayed_file_path;

                    for(int i =0 ; i< all_path_list.size() ; i++)
                    {
                        QString complete_path = all_path_list.at(i);

                        QString display_f_path = display_f_path_list.at(i);

                        QString file_export_dir_path =  files_dir_path + "/" + query.value("source_count_name").toString() + "/" + display_f_path;

                        QString dest_path(QFileInfo(file_export_dir_path).absolutePath());

                        QDir mdir(dest_path);
                        if(!mdir.exists())
                            mdir.mkpath(dest_path);

                        if(bool_checkbox_blur_status)
                        {
                            export_case_with_blur_effect_on_image(complete_path ,dest_path ,display_f_path );
                        }
                        else
                        {
                            export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_path ,dest_path ,QFileInfo(display_f_path).fileName() , Q_FUNC_INFO);
                        }
                    }
                }
            }

        }
    }

    //==========================================Plugins Data Export-End==========================================//
    if(!struct_export_case_artifacts_info_QList.isEmpty())
    {
        for(int ii = 0; ii < struct_export_case_artifacts_info_QList.size(); ii++)
        {
            struct_export_case_artifacts_info export_case_artifacts_obj = struct_export_case_artifacts_info_QList.at(ii);

            QString plugin_name = export_case_artifacts_obj.plugin_name;

            source_count_name_QStringList.append(export_case_artifacts_obj.source_count_name);

            QString plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_name;
            QString plugins_db_file = plugin_dir_path + "/" + plugin_name + ".sqlite" ;

            QString export_plugins_db_file = export_case_dir + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_name + "/" + plugin_name +".sqlite";

            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);

            QSqlDatabase export_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
            export_db.setDatabaseName(export_plugins_db_file);

            if(!export_db.open())
            {
                recon_static_functions::app_debug("export_db open--------FAILED------ " + export_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + export_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            QSqlQuery query_select(export_db);
            QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS plugins_db")
                    .arg(QDir::toNativeSeparators(plugins_db_file));
            query_select.exec(db_attach_commmand);

            QString insert_cmd = "INSERT into " + export_case_artifacts_obj.table_name + " SELECT * from plugins_db." + export_case_artifacts_obj.table_name + " WHERE INT = '" + export_case_artifacts_obj.record_no + "'";

            recon_helper_standard_obj->execute_db_command_by_dbreference(insert_cmd, export_db, Q_FUNC_INFO);

            QSqlQuery query_detach(export_db);
            QString command_detach = QString("DETACH DATABASE plugins_db");
            query_detach.prepare(command_detach);
            if(!query_detach.exec())
            {
                recon_static_functions::app_debug(" query memory detach  --------FAILED------ " + query_detach.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            export_db.close();
        }

    }

    if(!fs_mod_list.isEmpty())
    {
        QString sources_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "/sources_info.sqlite";

        sources_info_db_path.replace("//","/");

        QString select_sources_cmd = "SELECT source_count_name from fs_status";



        QStringList sources_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_sources_cmd, 0, sources_info_db_path, Q_FUNC_INFO);

        if(!sources_list.isEmpty())
        {
            for(int ii = 0; ii < sources_list.size(); ii++)
            {
                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + sources_list.at(ii) + QString("/file_system.sqlite");

                QString connection_naam = QString(Q_FUNC_INFO);

                QSqlDatabase fs_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

                fs_db.setDatabaseName(fs_db_path);

                if(!fs_db.open())
                {
                    recon_static_functions::app_debug(" fs_db  open--------FAILED------ " + fs_db.databaseName(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error -  " + fs_db.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                QString select_fs_data_cmd = "SELECT INT, filepath, filename from files WHERE extension LIKE ?";

                QSqlQuery query(fs_db);

                query.prepare(select_fs_data_cmd);

                for(int jj = 0 ; jj < fs_mod_list.size(); jj++)
                {
                    QString ext_mod = fs_mod_list.at(jj);
                    if(ext_mod.startsWith("."))
                    {
                        ext_mod.remove(0,1); //removing .(dot)
                    }
                    query.addBindValue(ext_mod);

                    if(!query.exec())
                    {
                        recon_static_functions::app_debug(" query - fs_db --------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
                        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
                        continue;
                    }

                    while(query.next())
                    {
                        struct_global_export_file_system_record obj;

                        obj.record_no = query.value("INT").toString();
                        obj.source_count_name = sources_list.at(ii);
                        obj.display_file_path = query.value("filepath").toString();
                        obj.displayed_file_name = query.value("filename").toString();

                        struct_file_system_export_tagged_files_info_QList.append(obj);

                    }

                }

            }
        }

    }

    if(!struct_file_system_export_tagged_files_info_QList.isEmpty())
    {
        for(int ii = 0; ii < struct_file_system_export_tagged_files_info_QList.size(); ii++)
        {
            struct_global_export_file_system_record obj = struct_file_system_export_tagged_files_info_QList.at(ii);

            QString source_count_name = obj.source_count_name;

            if(source_count_name.isEmpty())
                continue;

            source_count_name_QStringList.append(source_count_name);

            QString file_export_dir_path =  files_dir_path + "/" + source_count_name + "/" + obj.display_file_path;

            QString fpath(QFileInfo(file_export_dir_path).absolutePath());

            QDir mdir(fpath);
            if(!mdir.exists())
                mdir.mkpath(fpath);

            QString source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbpath(obj , Q_FUNC_INFO);

            if(bool_checkbox_blur_status)
            {
                export_case_with_blur_effect_on_image(source_file_path, fpath, obj.displayed_file_name);
            }
            else
            {
                QString exported_file_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_file_path, fpath, obj.displayed_file_name, Q_FUNC_INFO);
            }
        }

    }

    source_count_name_QStringList.removeDuplicates();

    for(int ii = 0; ii < source_count_name_QStringList.size(); ii++)
    {
        QString export_file_system_db_path = export_case_dir + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features + QString(MACRO_LOCAL_EXPORT_CASE_DIR_FILE_SYSTEM) +"/" + source_count_name_QStringList.at(ii) + "/file_system.sqlite";

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);

        QSqlDatabase export_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        export_db.setDatabaseName(export_file_system_db_path);

        if(!export_db.open())
            continue;

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name_QStringList.at(ii) + QString("/file_system.sqlite");

        QString export_path = export_case_dir + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files + source_count_name_QStringList.at(ii);

        QDir dir(export_path);

        if(dir.exists())
        {
            QSqlQuery query_select(export_db);
            QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS fs_db")
                    .arg(QDir::toNativeSeparators(fs_db_path));
            query_select.exec(db_attach_commmand);

            QDirIterator it(dir, QDirIterator::Subdirectories);

            while(it.hasNext())
            {
                QCoreApplication::processEvents();

                QString path_str = it.next();

                if(path_str.endsWith("..") || path_str.endsWith("."))
                    continue;

                QFileInfo info(path_str);

                QString file_name = info.fileName();

                QStringList list = path_str.split(source_count_name_QStringList.at(ii));

                QString file_parent_path;

                if(list.size() > 1)
                {
                    path_str = list.at(1);
                    file_parent_path = path_str;
                    file_parent_path.chop(file_name.length()+1);

                    if(file_parent_path.isEmpty())
                        file_parent_path = "/";
                }

                QString insert_cmd = "INSERT into files SELECT * from fs_db.files WHERE filepath = '" + path_str + "' AND file_parent_path = '" + file_parent_path + "'";

                recon_helper_standard_obj->execute_db_command_by_dbreference(insert_cmd, export_db, Q_FUNC_INFO);

            }

            QSqlQuery query_detach(export_db);
            QString command_detach = QString("DETACH DATABASE fs_db");
            query_detach.prepare(command_detach);
            if(!query_detach.exec())
            {
                recon_static_functions::app_debug(" query memory detach  --------FAILED------ " + query_detach.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_detach.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }

        export_db.close();
    }

    //==========================================Encryption - Decryption CSV for Reader-Start==========================================//

    QString lab_csv_dir = export_case_dir + "/Lab_Conf/CSVS/";

    QDirIterator dir_iter(lab_csv_dir, QDirIterator::Subdirectories);

    while (dir_iter.hasNext())
    {
        QString nxt_entry = dir_iter.next();

        if(!QFileInfo(nxt_entry).isFile() || !nxt_entry.endsWith(".csv"))
            continue;

        QString lab_csv_enc_path = nxt_entry;

        QString lab_csv_plain_path = recon_static_functions::decrypt_file_1(lab_csv_enc_path, Q_FUNC_INFO);

        recon_static_functions::remove_safely_blank_dir_OR_file(lab_csv_enc_path, Q_FUNC_INFO);
        recon_static_functions::encrypt_file_2(lab_csv_plain_path , lab_csv_enc_path , Q_FUNC_INFO);


    }
    //==========================================Encryption - Decryption CSV for Reader-End==========================================//

    //==========================================Copy Installer-Start==========================================//

    QString recon_case_reader_exe_name = QString("RECON_CASE_READER.exe");

    QString installer_resource_path =  QString("../ZNoNeedToSign/") +  recon_case_reader_exe_name;

    QFile::copy(installer_resource_path , installer_path + recon_case_reader_exe_name);

    //==========================================Copy Installer-End==========================================//

    display_loading_progress_bar_obj->hide();

    if(ui->radioButton_quick_mode->isChecked())
    {

        QString tags_db_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features) + QString(MACRO_LOCAL_EXPORT_CASE_DIR_TAG_SEARCH) + QString("/index_tag_search.sqlite");

        QString command = "Select tag_db_name from tag_search_index";
        QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command , 0 , tags_db_path, Q_FUNC_INFO);

        tag_db_name_list << "bookmarks";

        if(!tag_db_name_list.isEmpty())
        {
            QString delete_cmd = QString("DELETE from tag_search_index");

            recon_helper_standard_obj->execute_db_command_by_dbpath(delete_cmd, tags_db_path, Q_FUNC_INFO);

            for(int ii = 0; ii < tag_db_name_list.size(); ii++)
            {
                QString tag_name_str = tag_db_name_list.at(ii);

                if(tag_name_str == "bookmarks")
                    tags_db_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features) + QString(MACRO_LOCAL_EXPORT_CASE_DIR_BOOKMARKS_SEARCH) + QString("/bookmarks.sqlite");

                else
                    tags_db_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features) + QString(MACRO_LOCAL_EXPORT_CASE_DIR_TAG_SEARCH) + "/" + tag_db_name_list.at(ii) + QString(".sqlite");

                QString selct_cmd = "SELECT Distinct plugin_name from tags";

                QStringList plugin_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(selct_cmd,0, tags_db_path, Q_FUNC_INFO);

                for(int kk = 0; kk < plugin_name_list.size(); kk++)
                {
                    QString plugin_name = plugin_name_list.at(kk);

                    QString selct_tab_cmd = "SELECT Distinct tab_name from tags where plugin_name = '" + plugin_name + "'";

                    QStringList plugin_tab_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(selct_tab_cmd, 0, tags_db_path, Q_FUNC_INFO);
                    QString plugin_db_path = export_case_dir + "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS) + plugin_name + "/" + plugin_name + QString(".sqlite");
                    plugin_db_path.replace(" ", "_");

                    for(int dd = 0; dd < plugin_tab_list.size(); dd++)
                    {
                        QString update_cmd = "UPDATE '" + plugin_tab_list.at(dd) + "' set bookmark = 0, recon_tag_value = NULL" ;
                        recon_helper_standard_obj->execute_db_command_by_dbpath(update_cmd, plugin_db_path, Q_FUNC_INFO);
                    }
                }
                QFile file(tags_db_path);

                if(file.exists())
                    file.remove();
            }

        }

    }

    QMessageBox::information(this,"info","Export Case completed successfully");

    hide();
    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void export_case::slot_proc_copy_finished(int)
{
    bool_is_copy_finished = true;
}

void export_case::prepare_internet_history_display_list()
{ // prepare internet history display
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Internet_History;

    QStringList plugins_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(plugins_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - empty plugin list", Q_FUNC_INFO);
        return;
    }

    plugins_list.removeDuplicates();

    for(int ii = 0; ii < plugins_list.size(); ii++)
    {

        QString plugin_str = plugins_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_plugin_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_plugin_str->setCheckState(Qt::Checked);

        else
            item_plugin_str->setCheckState(Qt::Unchecked);

        item_plugin_str->setText(plugin_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_plugin_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void export_case::prepare_messages_display_list()
{ // prepare message list to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Messages;

    QStringList messengers_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(messengers_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - Empty messengers list ", Q_FUNC_INFO);
        return;
    }

    messengers_list.removeDuplicates();

    for(int ii = 0; ii < messengers_list.size(); ii++)
    {
        QString messenger_str = messengers_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_messenger_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_messenger_str->setCheckState(Qt::Checked);
        else
            item_messenger_str->setCheckState(Qt::Unchecked);

        item_messenger_str->setText(messenger_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_messenger_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_emails_display_list()
{ // prepare emails list to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Emails;

    QStringList emails_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(emails_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - empty email list", Q_FUNC_INFO);
        return;
    }

    emails_list.removeDuplicates();

    for(int ii = 0; ii < emails_list.size(); ii++)
    {
        QString email_str = emails_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_email_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_email_str->setCheckState(Qt::Checked);

        else
            item_email_str->setCheckState(Qt::Unchecked);

        item_email_str->setText(email_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_email_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_images_display_list()
{ // prepare images display to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Image;

    QStringList images_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(images_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - Image list empty ", Q_FUNC_INFO);
        return;
    }

    images_list.removeDuplicates();

    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString case_conf_select_cmd = "SELECT extension_name from extension_list where category_name = ?";

    QStringList usr_defined_ext_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(case_conf_select_cmd, QStringList() << MACRO_Export_Case_Category_Image, 0, case_conf_db_path, Q_FUNC_INFO);

    QString insert_cmd = "INSERT into tbl_export_status (category_name, list_name, check_status) VALUES(?,?,?)";

    QStringList new_ext_list;
    QStringList new_check_state_list;

    for(int ii = 0; ii < usr_defined_ext_list.size(); ii++)
    {
        if(images_list.contains(usr_defined_ext_list.at(ii)))
            continue;


        new_ext_list.append(usr_defined_ext_list.at(ii));
        new_check_state_list.append("1");

        QStringList values;
        values << MACRO_Export_Case_Category_Image << usr_defined_ext_list.at(ii) << "1";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, export_status_db_path, Q_FUNC_INFO);
    }

    images_list.append(new_ext_list);
    check_state_list.append(new_check_state_list);

    QString delete_cmd = "DELETE from tbl_export_status WHERE list_name = ?";

    for(int ii = images_list.size() - 1; ii >= 0; ii--)
    {
        if(!usr_defined_ext_list.contains(images_list.at(ii)))
        {
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd, QStringList() << images_list.at(ii), export_status_db_path, Q_FUNC_INFO);

            images_list.removeAt(ii);
            check_state_list.removeAt(ii);
        }

    }

    for(int ii = 0; ii < images_list.size(); ii++)
    {
        QString image_ext_str = images_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_image_ext_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_image_ext_str->setCheckState(Qt::Checked);
        else
            item_image_ext_str->setCheckState(Qt::Unchecked);

        item_image_ext_str->setText(image_ext_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_image_ext_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_multimedia_display_list()
{ // prepare multimedia section to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Multimedia;

    QStringList multimedia_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(multimedia_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - Empty multimedia list", Q_FUNC_INFO);
        return;
    }

    multimedia_list.removeDuplicates();

    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString case_conf_select_cmd = "SELECT extension_name from extension_list where category_name = ? OR category_name = ?";

    values.clear();
    values << "Audio" << "Video";

    QStringList usr_defined_ext_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(case_conf_select_cmd, values, 0, case_conf_db_path, Q_FUNC_INFO);

    QString insert_cmd = "INSERT into tbl_export_status (category_name, list_name, check_status) VALUES(?,?,?)";

    QStringList new_ext_list;
    QStringList new_check_state_list;

    for(int ii = 0; ii < usr_defined_ext_list.size(); ii++)
    {
        if(multimedia_list.contains(usr_defined_ext_list.at(ii)))
            continue;


        new_ext_list.append(usr_defined_ext_list.at(ii));
        new_check_state_list.append("1");

        QStringList values;
        values << MACRO_Export_Case_Category_Multimedia << usr_defined_ext_list.at(ii) << "1";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, export_status_db_path, Q_FUNC_INFO);
    }

    multimedia_list.append(new_ext_list);
    check_state_list.append(new_check_state_list);

    QString delete_cmd = "DELETE from tbl_export_status WHERE list_name = ?";

    for(int ii = multimedia_list.size() - 1; ii >= 0; ii--)
    {
        if(!usr_defined_ext_list.contains(multimedia_list.at(ii)))
        {
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd, QStringList() << multimedia_list.at(ii), export_status_db_path, Q_FUNC_INFO);

            multimedia_list.removeAt(ii);
            check_state_list.removeAt(ii);
        }

    }

    for(int ii = 0; ii < multimedia_list.size(); ii++)
    {

        QString multimedia_str = multimedia_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_multimedia_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_multimedia_str->setCheckState(Qt::Checked);
        else
            item_multimedia_str->setCheckState(Qt::Unchecked);

        item_multimedia_str->setText(multimedia_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_multimedia_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_documents_display_list()
{ // prepare document section to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Document;

    QStringList document_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(document_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - Empty document list", Q_FUNC_INFO);
        return;
    }

    document_list.removeDuplicates();

    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString case_conf_select_cmd = "SELECT extension_name from extension_list where category_name = ?";

    QStringList usr_defined_ext_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(case_conf_select_cmd, QStringList() << MACRO_Export_Case_Category_Document, 0, case_conf_db_path, Q_FUNC_INFO);

    QString insert_cmd = "INSERT into tbl_export_status (category_name, list_name, check_status) VALUES(?,?,?)";

    QStringList new_ext_list;
    QStringList new_check_state_list;

    for(int ii = 0; ii < usr_defined_ext_list.size(); ii++)
    {
        if(document_list.contains(usr_defined_ext_list.at(ii)))
            continue;


        new_ext_list.append(usr_defined_ext_list.at(ii));
        new_check_state_list.append("1");

        QStringList values;
        values << MACRO_Export_Case_Category_Document << usr_defined_ext_list.at(ii) << "1";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, export_status_db_path, Q_FUNC_INFO);
    }

    document_list.append(new_ext_list);
    check_state_list.append(new_check_state_list);

    QString delete_cmd = "DELETE from tbl_export_status WHERE list_name = ?";

    for(int ii = document_list.size() - 1; ii >= 0; ii--)
    {
        if(!usr_defined_ext_list.contains(document_list.at(ii)))
        {
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd, QStringList() << document_list.at(ii), export_status_db_path, Q_FUNC_INFO);

            document_list.removeAt(ii);
            check_state_list.removeAt(ii);
        }

    }

    for(int ii = 0; ii < document_list.size(); ii++)
    {
        QString document_str = document_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_document_str = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
            item_document_str->setCheckState(Qt::Checked);
        else
            item_document_str->setCheckState(Qt::Unchecked);

        item_document_str->setText(document_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_document_str);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::prepare_other_plugins_display_list()
{ // prepare other plugins display to export
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name, check_status from tbl_export_status WHERE category_name = ?";

    QStringList values;
    values << MACRO_Export_Case_Category_Other_Artifacts;

    QStringList other_artifacts_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);
    QStringList check_state_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 1, export_status_db_path, Q_FUNC_INFO);

    if(other_artifacts_list.isEmpty())
    {
        recon_static_functions::app_debug("Ends - Empty Remaining Artifact list ", Q_FUNC_INFO);
        return;
    }

    other_artifacts_list.removeDuplicates();
    other_artifacts_list.sort();

    for(int ii = 0; ii < other_artifacts_list.size(); ii++)
    {
        QString other_artifacts_str = other_artifacts_list.at(ii);

        int row = ui->tableWidget_quick_mode_list->rowCount();

        ui->tableWidget_quick_mode_list->insertRow(row);

        QTableWidgetItem *item_other_artifacts = new QTableWidgetItem;

        if(check_state_list.at(ii) == '1')
        {
            item_other_artifacts->setCheckState(Qt::Checked);
        }
        else
        {
            item_other_artifacts->setCheckState(Qt::Unchecked);
        }

        item_other_artifacts->setText(other_artifacts_str);

        ui->tableWidget_quick_mode_list->setItem(row, enum_tablewidget_quick_mode_list_items_column_zero_list, item_other_artifacts);
    }
    ui->tableWidget_quick_mode_list->sortByColumn(enum_tablewidget_quick_mode_list_items_column_zero_list,Qt::AscendingOrder);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::update_export_status_db_based_upon_quick_mode_category(QString str, int value)
{ // update export status in db if select quick mode
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);
    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT check_status from tbl_export_status WHERE category_name LIKE ?";

    QStringList list;
    list << str;

    QStringList status_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, list, 0, export_status_db_path, Q_FUNC_INFO);

    if(value == 0)
    {
        QString cmd = "UPDATE tbl_export_status SET check_status = ? WHERE category_name LIKE ?";

        QStringList values;
        values << QString::number(value) << str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, values, export_status_db_path, Q_FUNC_INFO);
    }
    else if(value == 1)
    {
        if(!status_list.contains("1"))
        {
            QString cmd = "UPDATE tbl_export_status SET check_status = ? WHERE category_name LIKE ?";

            QStringList values;
            values << QString::number(value) << str;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, values, export_status_db_path, Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::update_export_status_db_based_upon_expert_mode_category_and_tag_str(QString category_str, QString tag_str,int value)
{ // update export status database if select exprt  mode and tags
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT check_status from tbl_export_status WHERE (category_name LIKE ? AND list_name LIKE ?)";

    QStringList list;
    list << category_str << tag_str;

    QStringList status_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, list, 0, export_status_db_path, Q_FUNC_INFO);

    if(value == 0)
    {
        QString cmd = "UPDATE tbl_export_status SET check_status = ? WHERE (category_name LIKE ? AND list_name LIKE ?)";

        QStringList values;
        values << QString::number(value) << category_str << tag_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, values, export_status_db_path, Q_FUNC_INFO);
    }
    else if(value == 1)
    {
        if(!status_list.contains("1"))
        {
            QString cmd = "UPDATE tbl_export_status SET check_status = ? WHERE (category_name LIKE ? AND list_name LIKE ?)";

            QStringList values;
            values << QString::number(value) << category_str << tag_str;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, values, export_status_db_path, Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::on_tableWidget_quick_mode_list_cellClicked(int row, int column)
{ // generally click on any cell in quick mode on tablewidget
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(row < 0 || column < 0)
    {
        recon_static_functions::app_debug("Ends - invalid row or column", Q_FUNC_INFO);
        return;
    }

    if(column > 0)
        return;

    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString current_category_name = ui->tableWidget_quick_mode_category->currentItem()->text();
    QString current_list_name = ui->tableWidget_quick_mode_list->item(row,column)->text();

    QString cmd = "UPDATE 'tbl_export_status' SET check_status = ? WHERE list_name = ? AND category_name = ?";

    QStringList values;

    if(ui->tableWidget_quick_mode_list->item(row,column)->checkState() == Qt::Checked)
    {
        values << "1" << current_list_name << current_category_name;
    }

    else
    {
        values << "0" << current_list_name << current_category_name;
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, values, export_status_db_path, Q_FUNC_INFO);

    cmd = "SELECT COUNT(*) from 'tbl_export_status' WHERE check_status = ? AND category_name = ?";

    values.clear();

    values << "1" << current_category_name;

    QString cnt_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd,values,0,export_status_db_path, Q_FUNC_INFO);

    int cnt = cnt_str.toInt();

    if(cnt <= 0)
    {
        ui->tableWidget_quick_mode_category->currentItem()->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->tableWidget_quick_mode_category->currentItem()->setCheckState(Qt::Checked);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::on_tableWidget_quick_mode_category_cellClicked(int row, int column)
{ // generally clicked on quick mode category cell on tablewidget
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(row < 0 || column < 0)
    {
        recon_static_functions::app_debug("Ends - invalid row or column", Q_FUNC_INFO);
        return;
    }

    int current_row_category = ui->tableWidget_quick_mode_category->currentRow();
    if(current_row_category < 0)
    {
        recon_static_functions::app_debug("Ends - invalid row", Q_FUNC_INFO);
        return;
    }

    QString category_str = ui->tableWidget_quick_mode_category->item(row,column)->text();

    ui->tableWidget_quick_mode_list->setRowCount(0);

    if(category_str == QString(MACRO_Export_Case_Category_Internet_History))
    {
        prepare_internet_history_display_list();
    }

    else if(category_str == QString(MACRO_Export_Case_Category_Messages))
    {
        prepare_messages_display_list();
    }

    else if(category_str == QString(MACRO_Export_Case_Category_Emails))
    {
        prepare_emails_display_list();
    }

    else if(category_str == QString(MACRO_Export_Case_Category_Image))
    {
        prepare_images_display_list();
    }

    else if(category_str == QString(MACRO_Export_Case_Category_Multimedia))
    {
        prepare_multimedia_display_list();
    }

    else if(category_str == QString(MACRO_Export_Case_Category_Document))
    {
        prepare_documents_display_list();
    }
    else if(category_str == QString(MACRO_Export_Case_Category_Other_Artifacts))
    {
        prepare_other_plugins_display_list();
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void export_case::on_tableWidget_quick_mode_category_cellChanged(int row, int column)
{ // on tableiwdget quick mode category cell changed to get value
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString category_str = ui->tableWidget_quick_mode_category->item(row,column)->text();

    int check_state = 0;

    if(ui->tableWidget_quick_mode_category->item(row,column)->checkState() == Qt::Checked)
    {
        check_state = 1;
    }

    update_export_status_db_based_upon_quick_mode_category(category_str, check_state);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::on_tableWidget_expert_mode_tag_list_cellChanged(int row, int column)
{ // expert mode tag list cell changed on table widget
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString tag_str = ui->tableWidget_expert_mode_tag_list->item(row,column)->text();

    int check_state = 0;

    if(ui->tableWidget_expert_mode_tag_list->item(row,column)->checkState() == Qt::Checked)
    {
        check_state = 1;
    }

    update_export_status_db_based_upon_expert_mode_category_and_tag_str(MACRO_Export_Case_Category_Tags ,tag_str, check_state);
    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::on_radioButton_quick_mode_toggled(bool checked)
{ // select radio button quick mode
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(ui->radioButton_quick_mode->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(ui->radioButton_expert_mode->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void export_case::export_case_with_blur_effect_on_image(QString file_path, QString dest_file_path, QString display_file_path)
{ // export case with blur effect checkbox feature
    QMimeDatabase mime_database_obj;
    QString file_mime_type = mime_database_obj.mimeTypeForFile(file_path).name();

    if(file_mime_type.startsWith("image/"))
    {
        QString file_name = QFileInfo(display_file_path).fileName();

        if(!dest_file_path.endsWith("/"))
            dest_file_path.append("/");

        dest_file_path = dest_file_path.append(file_name);

        QStringList args_list;

        //        QString cmd_python_path = global_varibale_python_path;
        //        QString python_exe_path = "../Resources/scripts_persistent/blur.py";
        //        args_list << python_exe_path << file_path << dest_file_path;

        QString blur_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/blur";
        args_list << file_path << dest_file_path;

        bool_proc_blur_img_finished = false;
        QProcess *process_blur_effect = new QProcess(this);
        connect(process_blur_effect, SIGNAL(finished(int)), this, SLOT(slot_process_blur_image_finished(int)));

        //        process_blur_effect->start(cmd_python_path, args_list);
        process_blur_effect->start(blur_exe_path, args_list);

        if (!process_blur_effect->waitForStarted())
        {
            recon_static_functions::app_debug("Blur Process Start ----FAILED---- ",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error: " + process_blur_effect->errorString() ,Q_FUNC_INFO);
            //            export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(file_path ,dest_file_path ,QFileInfo(display_file_path).fileName() , Q_FUNC_INFO);
            return;
        }

        int m_cnt = 0;
        while(1)
        {
            m_cnt++;
            if(m_cnt % 500 == 0)
            {
                m_cnt = 0;
                QCoreApplication::processEvents();
            }

            if(bool_proc_blur_img_finished || process_blur_effect->state() == QProcess::NotRunning)
                break;
        }

        delete process_blur_effect;
    }
    else
    {
        export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(file_path ,dest_file_path ,QFileInfo(display_file_path).fileName() , Q_FUNC_INFO);
    }
}

void export_case::slot_process_blur_image_finished(int status)
{
    bool_proc_blur_img_finished = true;
}

void export_case::on_checkBox_blur_status_clicked(bool checked)
{
    bool_checkbox_blur_status = checked;
}
