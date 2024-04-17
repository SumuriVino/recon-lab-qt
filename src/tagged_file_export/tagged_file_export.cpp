#include "tagged_file_export.h"
#include "ui_tagged_file_export.h"

tagged_file_export::tagged_file_export(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tagged_file_export)
{
    ui->setupUi(this);

    QString m_title = "Tagged File Export";

    setWindowTitle(m_title);
    setWindowModality(Qt::ApplicationModal);


    setFixedWidth(500);
    setFixedHeight(500);
    ui->lineEdit_di_path->setAttribute(Qt::WA_MacShowFocusRect,false);


    QTableWidgetItem *horizontal_header_select_tags = ui->tableWidget_tags->horizontalHeaderItem(0);
    horizontal_header_select_tags->setText("Tags");
    horizontal_header_select_tags->setCheckState(Qt::Unchecked);
    horizontal_header_select_tags->setIcon(QIcon("../icons/unchecked.png"));
    ui->tableWidget_tags->setHorizontalHeaderItem(0,horizontal_header_select_tags);
    QObject::connect((QObject*)ui->tableWidget_tags->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_tags_horizontal_header_clicked(int)));


    ui->tableWidget_tags->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->tableWidget_tags->setHorizontalHeaderItem(0,horizontal_header_select_tags);
    ui->tableWidget_tags->installEventFilter(this);
    ui->tableWidget_tags->verticalHeader()->setHighlightSections(false);
    ui->tableWidget_tags->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_tags->horizontalHeader()->setSectionsMovable(true);
    ui->tableWidget_tags->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_tags->setColumnWidth(0, 550);

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);
    export_rsync_obj = new export_rsync(this);

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


    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    thread_zip_file_export_obj = new thread_zip_file_export;
    connect(thread_zip_file_export_obj, SIGNAL(finished()), this, SLOT(slot_thread_zip_file_export_finished()));


    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    M_VAR_export_dir_name_QString = m_title.replace(" ", "_");


    prepare_encrypted_zip_password_dialog_box();

    ui->checkBox_recursive->show();
    ui->checkBox_recursive->setEnabled(false);


    ui->checkBox_encrypt_zip->setEnabled(false);
    ui->checkBox_encrypt_zip->setAttribute(Qt::WA_MacShowFocusRect, false);

}


void tagged_file_export::slot_tags_horizontal_header_clicked(int index)
{
    QTableWidgetItem *horizontal_header_item = ui->tableWidget_tags->horizontalHeaderItem(0);
    if(horizontal_header_item->checkState() == Qt::Unchecked)
    {
        horizontal_header_item->setIcon(QIcon("../icons/checked.png"));
        horizontal_header_item->setCheckState(Qt::Checked);

        for(int i = 0; i < availbale_tags_from_config_db.size(); i++)
        {
            ui->tableWidget_tags->item(i,0)->setCheckState(Qt::Checked);
        }
    }
    else if(horizontal_header_item->checkState() == Qt::Checked)
    {
        horizontal_header_item->setIcon(QIcon("../icons/unchecked.png"));
        horizontal_header_item->setCheckState(Qt::Unchecked);

        for(int i = 0; i < availbale_tags_from_config_db.size(); i++)
        {
            ui->tableWidget_tags->item(i,0)->setCheckState(Qt::Unchecked);
        }
    }
}



tagged_file_export::~tagged_file_export()
{
    delete ui;
}

void tagged_file_export::closeEvent(QEvent *event)
{
    hide();
}


void tagged_file_export::configure_ui()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    availbale_tags_from_config_db.clear();

    ui->tableWidget_tags->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);
    ui->tableWidget_tags->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
    ui->checkBox_md5->setChecked(false);
    ui->checkBox_sha1->setChecked(false);


    result_dir_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString command = "SELECT tag_name FROM tags";
    QString tags_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QStringList list_tags_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,tags_db_path,Q_FUNC_INFO);

    QString command_2 = "SELECT selected_colour FROM tags";
    QStringList list_tag_colour_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_2,0,tags_db_path,Q_FUNC_INFO);

    list_tags_from_db.removeAll("");
    list_tags_from_db.removeDuplicates();

    list_tag_colour_from_db.removeAll("");

    if(list_tags_from_db.size() != list_tag_colour_from_db.size())
    {
        recon_static_functions::app_debug("---FAILED--- 'list_tags_from_db' not equal with 'list_tag_colour_from_db' ", Q_FUNC_INFO);
        recon_static_functions::app_debug(" End",Q_FUNC_INFO);
        return;
    }


    //-Insert one bookmark tag
    list_tags_from_db.prepend(QString(MACRO_Tag_Name_Bookmarks));
    list_tag_colour_from_db.prepend(""); //To make both list count equal.


    list_tags_from_db.append(MACRO_Tag_Name_Notes);
    list_tag_colour_from_db.append(""); //To make both list count equal.


    availbale_tags_from_config_db = list_tags_from_db;

    ///clearing previous tag entries
    while(ui->tableWidget_tags->rowCount())
        ui->tableWidget_tags->removeRow(0);



    for(int i = 0; i < list_tags_from_db.size(); i++)
    {
        if(list_tags_from_db.at(i) == QString(""))
            continue;

        QTableWidgetItem *item_tags = new QTableWidgetItem();

        if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Bookmarks))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/tag_icons/bookmarks.png"));
        }
        else if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Notes))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/Features/notes.png"));
        }
        else
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/tag_icons/" + list_tag_colour_from_db.at(i).toLower().replace(" ","_")));
        }

        ui->tableWidget_tags->insertRow(i);
        ui->tableWidget_tags->setRowHeight(i,25);
        ui->tableWidget_tags->setItem(i,0,item_tags);
    }

    ui->radioButton_export_to_zip->setChecked(true);

    set_zip_file_path();


    ui->checkBox_encrypt_zip->setChecked(false);
    ui->checkBox_recursive->setChecked(false);

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void tagged_file_export::pub_set_recon_case_obj(RECON_case *r_case_obj)
{
    recon_case_obj = r_case_obj;

}

void tagged_file_export::prepare_selected_tags_list()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    prepared_tags_to_export_list.clear();
    QStringList selected_tags_list;
    for(int ii = 0; ii < ui->tableWidget_tags->rowCount(); ii++)
    {
        if(ui->tableWidget_tags->item(ii,0)->checkState() == 2)
        {
            selected_tags_list << ui->tableWidget_tags->item(ii,0)->text();
        }
    }
    prepared_tags_to_export_list = selected_tags_list;

    return;
}

bool tagged_file_export::is_export_available_for_selceted_tags(qint64 total_record_count_qint64)
{

    if(total_record_count_qint64 <= 0)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "No file(s) found.");
        ui->checkBox_encrypt_zip->setChecked(false);
        return false;
    }


    return true;
}


void tagged_file_export::export_files_to_folder(bool bool_keep_directory_structure , QString export_file_dir)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    bool_cancel_loading = false;
    QStringList file_name_list;

    if(prepared_tags_to_export_list.size() <= 0)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Please select at least one tag to export file(s)");
        return ;
    }

    /// Select Distinct INT , filepath from files
    struct_command_info st_comnd_info_obj = get_command_info();

    qint64 total_record_count_qint64 = get_record_counts_for_all_selected_tags(st_comnd_info_obj);

    if(!is_export_available_for_selceted_tags(total_record_count_qint64))
    {
        recon_static_functions::app_debug(" no record found to export -End",Q_FUNC_INFO);

        return ;
    }

    exported_folder_dir_path = export_file_dir;

    qint64 progressbar_increment_step = total_record_count_qint64 / 10;
    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;




    hide();


    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...  ");

    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    display_loading_progress_bar_obj->show();


    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    qint64 processing_record_count = 0;
    global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(qint64 count = 0; count < list_target_source_info.size(); count++)
    {

        if(bool_cancel_loading)
            break ;

        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        QString virtual_source_path = obj.virtual_source_path;
        if(!QFileInfo(virtual_source_path).exists())
        {
            recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
            continue ;
        }

        QString source_count_name =  obj.source_count_name;
        if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
            continue ;

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";


        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(filesystem_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        source_db.transaction();

        QStringList arg_list = st_comnd_info_obj.addbind_values_for_unreserved_tags_list;
        QString command = st_comnd_info_obj.record_data_command;

        QSqlQuery query(source_db);
        query.prepare(command);
        for(qint64 tt = 0 ; tt < arg_list.size() ; tt++)
        {
            query.addBindValue(arg_list.at(tt));
        }
        if(!query.exec())
        {
            recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        qint64 qproc_counter = 0;
        while(query.next())
        {

            if(bool_cancel_loading)
                break;

            qproc_counter++;
            if(qproc_counter % 100 == 0)
            {
                qproc_counter = 0;
                QApplication::processEvents();
            }

            processing_record_count++;
            QString current_export_status_str  = "Exporting Files...  "  + QString::number(processing_record_count)  + "/" +QString::number(total_record_count_qint64);
            display_loading_progress_bar_obj->pub_set_label_messsge(current_export_status_str);


            QString record_no = query.value("INT").toString();
            QString display_file_path = query.value("filepath").toString();
            QString is_dir = query.value("is_dir").toString();

            QString dest_dir_path;
            if(bool_keep_directory_structure)
            {
                QString parent_dir =  recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(display_file_path);
                dest_dir_path = export_file_dir + "/"  + source_count_name + parent_dir + "/";
                QDir mdir;
                mdir.mkpath(dest_dir_path);
            }
            else
            {
                dest_dir_path = export_file_dir + "/"  + source_count_name + "/";
                QDir mdir;
                mdir.mkpath(dest_dir_path);
            }


            QString file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);

            QString source_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db, "Files", source_count_name, Q_FUNC_INFO);

            QString os_schme_intrnl = obj.os_scheme_internal;

            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
            {
                struct_global_export_file_system_record export_file_system_recordobj;
                export_file_system_recordobj.record_no = record_no;
                export_file_system_recordobj.source_count_name = source_count_name;
                export_file_system_recordobj.display_file_path = display_file_path;
                export_file_system_recordobj.displayed_file_name = file_name;

                source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbreference(export_file_system_recordobj,source_db , Q_FUNC_INFO);
            }

            QString new_name = recon_static_functions::get_available_stringname(file_name, file_name_list, Q_FUNC_INFO);

            QString dest_file_path = dest_dir_path + "/" + new_name;

            if(is_dir == "1" && !source_file_path.trimmed().isEmpty())
            {
                source_file_path = source_file_path + "/";
            }

            export_rsync_obj->pub_copy_file_using_rsync(source_file_path,dest_file_path, enum_global_rsync_args_type_general);

            file_name_list << new_name;
        }

        source_db.commit();
        source_db.close();
    }

    if(ui->checkBox_md5->isChecked() || ui->checkBox_sha1->isChecked())
    {
        for(qint64 count = 0; count < list_target_source_info.size(); count++)
        {
            if(bool_cancel_loading)
                break ;

            struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
            QString virtual_source_path = obj.virtual_source_path;
            if(!QFileInfo(virtual_source_path).exists())
            {
                recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
                continue ;
            }
            QString source_count_name =  obj.source_count_name;
            if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
                continue ;

            QString csv_file_dir = export_file_dir + "/" + source_count_name;


            write_exported_files_details_in_csv(source_count_name , csv_file_dir);
        }
    }

    if(!bool_cancel_loading && QFileInfo(export_file_dir).exists())
    {
        if(export_file_dir.endsWith("/"))
            export_file_dir.chop(QString("/").size());

        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported. Do you want to open it?");
        message_dialog_obj->show();
    }


    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}


void tagged_file_export::export_files_to_zip(QString zip_file_destination , QString export_dir_path)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    bool_cancel_loading = false;
    QStringList source_file_list;

    if(prepared_tags_to_export_list.size() <= 0)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), ("Please select at least one tag to export file(s)"));
        return;
    }


    /// Select Distinct INT , filepath from files
    struct_command_info st_comnd_info_obj = get_command_info();

    qint64 total_record_count_qint64 = get_record_counts_for_all_selected_tags(st_comnd_info_obj);
    if(!is_export_available_for_selceted_tags(total_record_count_qint64))
    {
        recon_static_functions::app_debug(" no record found to export -End",Q_FUNC_INFO);
        return ;
    }


    hide();


    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...  ");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QList<struct_global_file_for_zip_export> files_list_for_zip_export;

    for(int count = 0; count < list_target_source_info.size(); count++)
    {

        if(bool_cancel_loading)
            break ;

        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        QString virtual_source_path = obj.virtual_source_path;
        if(!QFileInfo(virtual_source_path).exists())
        {
            recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
            continue ;
        }
        QString source_count_name =  obj.source_count_name;
        if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
            continue ;

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";


        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(filesystem_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        source_db.transaction();

        QString command  = st_comnd_info_obj.record_data_command;
        if(command.trimmed().isEmpty())
        {
            source_db.commit();
            source_db.close();
            recon_static_functions::app_debug(" End",Q_FUNC_INFO);
            continue ;
        }
        QStringList arg_list = st_comnd_info_obj.addbind_values_for_unreserved_tags_list;

        recon_static_functions::app_debug(" command  -- " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" values   -- " + arg_list.join(","),Q_FUNC_INFO);


        QSqlQuery query(source_db);
        query.prepare(command);
        for(qint64 tt = 0 ; tt < arg_list.size() ; tt++)
        {
            query.addBindValue(arg_list.at(tt));
        }
        if(!query.exec())
        {
            source_db.commit();
            source_db.close();
            recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
            continue;
        }


        qint64 qproc_counter = 0;
        while(query.next())
        {

            if(bool_cancel_loading)
                break ;

            qproc_counter++;
            if(qproc_counter % 100 == 0)
            {
                qproc_counter = 0;
                QApplication::processEvents();
            }



            QString record_no = query.value("INT").toString();
            QString display_file_path = query.value("filepath").toString();
            QString file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);


            struct_global_export_file_system_record obj;
            obj.record_no = record_no;
            obj.source_count_name = source_count_name;
            obj.display_file_path = display_file_path;
            obj.displayed_file_name = file_name;


            QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,source_db,"files",source_count_name,Q_FUNC_INFO);

            struct_global_file_for_zip_export st_file_info_for_export_obj;
            if(recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no,source_db,"files",source_count_name,Q_FUNC_INFO))
            {
                qint64 rec_counter = 0;

                QString cmd_2 = "select INT from files where file_parent_path LIKE '" + display_file_path  + "%'";
                QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(cmd_2, 0, source_db, Q_FUNC_INFO);
                for(int ii = 0 ; ii < INT_list.size() ; ii++)
                {
                    rec_counter++;
                    if(rec_counter % 10 == 0)
                    {
                        rec_counter = 0;
                        QApplication::processEvents();
                    }


                    QString relevant_record_no = INT_list.at(ii);
                    QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(relevant_record_no,source_db ,"files" ,source_count_name, Q_FUNC_INFO);

                    if(source_file_list.contains(file_path))
                        continue;

                    source_file_list << file_path;


                    QFileInfo m_file_info(file_path);
                    if(!m_file_info.exists())
                    {
                        continue ;
                    }

                    QString cmd = "select filepath from files where INT =" + relevant_record_no ;
                    QString displ_path = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, source_db, Q_FUNC_INFO);


                    //Note
                    if(m_file_info.isSymLink())
                        st_file_info_for_export_obj.source_path =  m_file_info.symLinkTarget();
                    else
                        st_file_info_for_export_obj.source_path = file_path;

                    st_file_info_for_export_obj.destination_path = source_count_name + displ_path;
                    files_list_for_zip_export.append(st_file_info_for_export_obj);

                }
            }
            else
            {
                QFileInfo info(complete_path);
                if(!info.exists())
                {
                    continue;
                }
                else
                {
                    if(source_file_list.contains(complete_path))
                        continue;

                    source_file_list << complete_path;

                    if(info.isSymLink())
                        st_file_info_for_export_obj.source_path = info.symLinkTarget();
                    else
                        st_file_info_for_export_obj.source_path = complete_path;

                    st_file_info_for_export_obj.destination_path = source_count_name + display_file_path;
                    files_list_for_zip_export.append(st_file_info_for_export_obj);
                }
            }
        }

        source_db.commit();
        source_db.close();
    }

    source_file_list.clear();

    QDir dir;
    dir.mkpath(export_dir_path);
    QString zip_file_nm = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(zip_file_destination);
    zip_file_destination = export_dir_path  + "/" + zip_file_nm;
    exported_zip_dest_file_path = zip_file_destination;
    zip_filename = zip_file_nm;
    export_dir_file_for_zip = export_dir_path;

    bool bool_encrypt_zip = false;
    QString password_enc_zip;
    if(ui->checkBox_encrypt_zip->isChecked())
    {
        bool_encrypt_zip = true;
        password_enc_zip = lineedit_password_zip_enc->text();
    }

    thread_zip_file_export_obj->pub_set_essentials(zip_file_destination, files_list_for_zip_export, bool_encrypt_zip, password_enc_zip);

    thread_zip_file_export_obj->start();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

void tagged_file_export::export_files_to_dmg(bool bool_keep_directory_structure , QString tmp_export_dmg_path,QString export_dmg_path)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    bool_cancel_loading = false;
    QStringList file_name_list;

    if(prepared_tags_to_export_list.size() <= 0)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Please select at least one tag to export file(s)");
        return ;
    }

    /// Select Distinct INT , filepath from files
    struct_command_info st_comnd_info_obj = get_command_info();

    qint64 total_record_count_qint64 = get_record_counts_for_all_selected_tags(st_comnd_info_obj);

    if(!is_export_available_for_selceted_tags(total_record_count_qint64))
    {
        recon_static_functions::app_debug(" no record found to export -End",Q_FUNC_INFO);
        return ;
    }

    if(!export_dmg_path.endsWith(".dmg"))
        export_dmg_path.append(".dmg");

    exported_dmg_path = export_dmg_path;

    qint64 progressbar_increment_step = total_record_count_qint64 / 10;
    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    hide();

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    display_loading_progress_bar_obj->pub_set_label_messsge("Exporting Files...  ");

    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    display_loading_progress_bar_obj->show();

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    qint64 processing_record_count = 0;
    global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(qint64 count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_loading)
            break ;

        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        QString virtual_source_path = obj.virtual_source_path;
        if(!QFileInfo(virtual_source_path).exists())
        {
            recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
            continue ;
        }

        QString source_count_name =  obj.source_count_name;
        if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
            continue ;

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        source_db.setDatabaseName(filesystem_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        source_db.transaction();

        QStringList arg_list = st_comnd_info_obj.addbind_values_for_unreserved_tags_list;
        QString command = st_comnd_info_obj.record_data_command;

        QSqlQuery query(source_db);
        query.prepare(command);
        for(qint64 tt = 0 ; tt < arg_list.size() ; tt++)
        {
            query.addBindValue(arg_list.at(tt));
        }
        if(!query.exec())
        {
            recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        qint64 qproc_counter = 0;
        while(query.next())
        {
            if(bool_cancel_loading)
                break;

            qproc_counter++;
            if(qproc_counter % 100 == 0)
            {
                qproc_counter = 0;
                QApplication::processEvents();
            }

            processing_record_count++;
            QString current_export_status_str  = "Exporting Files...  "  + QString::number(processing_record_count)  + "/" +QString::number(total_record_count_qint64);
            display_loading_progress_bar_obj->pub_set_label_messsge(current_export_status_str);


            QString record_no = query.value("INT").toString();
            QString display_file_path = query.value("filepath").toString();
            QString is_dir = query.value("is_dir").toString();

            QString dest_dir_path;
            if(bool_keep_directory_structure)
            {
                QString parent_dir =  recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(display_file_path);
                dest_dir_path = tmp_export_dmg_path + "/"  + source_count_name + parent_dir + "/";

                QDir mdir;
                mdir.mkpath(dest_dir_path);
            }
            else
            {
                dest_dir_path = tmp_export_dmg_path + "/"  + source_count_name + "/";

                QDir mdir;
                mdir.mkpath(dest_dir_path);
            }

            QString file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);

            QString source_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db, "Files", source_count_name, Q_FUNC_INFO);

            QString os_schme_intrnl = obj.os_scheme_internal;

            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                    || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
            {
                struct_global_export_file_system_record export_file_system_recordobj;
                export_file_system_recordobj.record_no = record_no;
                export_file_system_recordobj.source_count_name = source_count_name;
                export_file_system_recordobj.display_file_path = display_file_path;
                export_file_system_recordobj.displayed_file_name = file_name;

                source_file_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbreference(export_file_system_recordobj,source_db , Q_FUNC_INFO);
            }

            QString new_name = recon_static_functions::get_available_stringname(file_name, file_name_list, Q_FUNC_INFO);

            QString dest_file_path = dest_dir_path + "/" + new_name;

            if(is_dir == "1" && !source_file_path.trimmed().isEmpty())
            {
                source_file_path = source_file_path + "/";
            }

            export_rsync_obj->pub_copy_file_using_rsync(source_file_path,dest_file_path, enum_global_rsync_args_type_general);

            file_name_list << new_name;
        }

        source_db.commit();
        source_db.close();
    }

    if(ui->checkBox_md5->isChecked() || ui->checkBox_sha1->isChecked())
    {
        for(qint64 count = 0; count < list_target_source_info.size(); count++)
        {
            if(bool_cancel_loading)
                break ;

            struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
            QString virtual_source_path = obj.virtual_source_path;
            if(!QFileInfo(virtual_source_path).exists())
            {
                recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
                continue ;
            }

            QString source_count_name =  obj.source_count_name;
            if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
                continue ;

            QString csv_file_dir = tmp_export_dmg_path + "/" + source_count_name;


            write_exported_files_details_in_csv(source_count_name , csv_file_dir);
        }
    }

    bool bool_is_image_created = false;

    QStringList args_list_id;
    args_list_id << "create" <<  export_dmg_path << "-fs" << "apfs" << "-srcfolder" << tmp_export_dmg_path;

    QString list_data = recon_helper_process_obj->run_command_with_arguments("hdiutil", args_list_id, Q_FUNC_INFO);

    if(!list_data.contains("created:"))
    {
        message_dialog_obj->pub_set_message("Tagged File Export", MACRO_MSG_WARNING ,"Failed To Export Files");
        message_dialog_obj->show();
    }
    else
    {
        bool_is_image_created = true;
    }

    QDir dir(tmp_export_dmg_path);
    if(dir.exists())
    {
        dir.removeRecursively();
    }

    if(bool_cancel_loading && QFile(export_dmg_path).exists())
    {
        QFile file(export_dmg_path);
        if(file.exists())
        {
            file.remove();
        }
    }

    if(!bool_cancel_loading && bool_is_image_created)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO ,"File(s) has been exported to image. Do you want to open it?");
        message_dialog_obj->show();
    }

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

tagged_file_export::struct_command_info tagged_file_export::get_command_info()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    struct_command_info struct_tag_info_obj;
    struct_tag_info_obj.record_data_command.clear();
    struct_tag_info_obj.addbind_values_for_unreserved_tags_list.clear();
    struct_tag_info_obj.record_total_count_command.clear();

    QString select_column_command = "Select INT , filepath ,notes, is_dir  from files where ";

    QString record_count_qry = "Select count(*)  from files where ";


    QStringList columns_list;
    QStringList add_bind_tags_list;
    for(int ii = 0 ; ii < prepared_tags_to_export_list.size() ; ii++)
    {
        QString tag_name = prepared_tags_to_export_list.at(ii);
        if(tag_name == QString(MACRO_Tag_Name_Notes))
        {
            columns_list <<  " notes IS NOT NULL ";
            continue;
        }
        else if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
        {
            columns_list <<  " bookmark = '1'";
            continue;
        }


        columns_list <<  " recon_tag_value = ?";
        add_bind_tags_list << tag_name;
    }

    QString complete_cmd          = select_column_command + columns_list.join(" OR ");
    QString record_count_command  = record_count_qry + columns_list.join(" OR ");

    struct_tag_info_obj.record_data_command = complete_cmd;
    struct_tag_info_obj.addbind_values_for_unreserved_tags_list = add_bind_tags_list;
    struct_tag_info_obj.record_total_count_command = record_count_command;

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

    return struct_tag_info_obj;
}

qint64 tagged_file_export::get_record_counts_for_all_selected_tags(tagged_file_export::struct_command_info st_cmd_info)
{
    qint64 total_record_count_qint64 = 0;

    QString command  = st_cmd_info.record_total_count_command;
    if(command.trimmed().isEmpty())
    {
        return total_record_count_qint64;
    }


    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(qint64 count = 0; count < list_target_source_info.size(); count++)
    {
        if(bool_cancel_loading)
            break ;

        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        QString virtual_source_path = obj.virtual_source_path;
        if(!QFileInfo(virtual_source_path).exists())
        {
            recon_static_functions::app_debug(" Mount path not found ",Q_FUNC_INFO);
            continue ;
        }

        QString source_count_name =  obj.source_count_name;
        if(!recon_case_obj->pub_is_source_extraction_completed(source_count_name))
            continue ;

        QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        QStringList arg_list = st_cmd_info.addbind_values_for_unreserved_tags_list;
        total_record_count_qint64 += recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,filesystem_db_path,Q_FUNC_INFO).toLongLong();
    }

    return total_record_count_qint64;
}

void tagged_file_export::write_exported_files_details_in_csv(QString source_count_name, QString cvs_dir_path)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    /// Select Distinct INT , filepath from files
    struct_command_info st_comnd_info_obj = get_command_info();

    ///QMap<source_count_name, QList<struct_file_info_for_hashes>>
    QMap<QString, QList<struct_file_info_for_hashes>> qmap_of_st_file_info_for_csv;


    QString filesystem_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(filesystem_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString command  = st_comnd_info_obj.record_data_command;
    if(command.trimmed().isEmpty())
    {
        source_db.close();
        recon_static_functions::app_debug(" End",Q_FUNC_INFO);
        return ;
    }

    QStringList arg_list = st_comnd_info_obj.addbind_values_for_unreserved_tags_list;

    QSqlQuery query(source_db);
    query.prepare(command);
    for(qint64 tt = 0 ; tt < arg_list.size() ; tt++)
    {
        query.addBindValue(arg_list.at(tt));
    }
    if(!query.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
        return;
    }


    QList<struct_file_info_for_hashes> st_file_info_for_hashes_qlist;
    QStringList temp_file_list;

    qint64 qproc_counter = 0;
    while(query.next())
    {

        if(bool_cancel_loading)
            break ;

        qproc_counter++;
        if(qproc_counter % 100 == 0)
        {
            qproc_counter = 0;
            QApplication::processEvents();
        }

        QString record_no = query.value("INT").toString();
        QString display_file_path = query.value("filepath").toString();
        QString notes_value = query.value("notes").toString();
        QString file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);

        struct_global_export_file_system_record obj;
        obj.record_no = record_no;
        obj.source_count_name = source_count_name;
        obj.display_file_path = display_file_path;
        obj.displayed_file_name = file_name;


        QString complete_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,source_db,"files",source_count_name,Q_FUNC_INFO);

        struct_file_info_for_hashes st_file_info_for_to_extract_hashes_obj;

        if(recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no,source_db,"files",source_count_name,Q_FUNC_INFO))
        {
            QString cmd_2 = "select INT from files where file_parent_path LIKE '" + display_file_path  + "%'";
            QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(cmd_2, 0, source_db, Q_FUNC_INFO);
            for(int ii = 0 ; ii < INT_list.size() ; ii++)
            {
                QString relevant_record_no = INT_list.at(ii);
                QString file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(relevant_record_no,source_db ,"files" ,source_count_name, Q_FUNC_INFO);
                QFileInfo m_file_info(file_path);
                if(!m_file_info.exists())
                    continue ;

                QString cmd = "select filepath , notes from files where INT =" + relevant_record_no ;
                QString displ_path = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, source_db, Q_FUNC_INFO);
                QString notes_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 1, source_db, Q_FUNC_INFO);
                QString f_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(displ_path);

                temp_file_list << displ_path;


                st_file_info_for_to_extract_hashes_obj.complete_file_path = file_path;
                st_file_info_for_to_extract_hashes_obj.file_name = f_name;
                st_file_info_for_to_extract_hashes_obj.display_file_path = displ_path;
                st_file_info_for_to_extract_hashes_obj.examiner_notes_value = notes_str;

                st_file_info_for_hashes_qlist << st_file_info_for_to_extract_hashes_obj;
            }
        }
        else
        {
            QFileInfo info(complete_path);
            if(!info.exists())
            {
                continue;
            }
            else
            {
                temp_file_list << display_file_path;

                st_file_info_for_to_extract_hashes_obj.complete_file_path = complete_path;
                st_file_info_for_to_extract_hashes_obj.file_name = file_name;
                st_file_info_for_to_extract_hashes_obj.display_file_path = display_file_path;
                st_file_info_for_to_extract_hashes_obj.examiner_notes_value = notes_value;

                st_file_info_for_hashes_qlist << st_file_info_for_to_extract_hashes_obj;

            }
        }

        qmap_of_st_file_info_for_csv.insert(source_count_name,st_file_info_for_hashes_qlist);
    }
    source_db.close();

    make_csv_with_hashes(qmap_of_st_file_info_for_csv.value(source_count_name) , cvs_dir_path);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}


void tagged_file_export::make_csv_with_hashes(QList <struct_file_info_for_hashes> st_file_detail_in_csv , QString export_file_dir)
{
    recon_static_functions::app_debug(" start" , Q_FUNC_INFO);

    if(st_file_detail_in_csv.size() == 0)
    {
        recon_static_functions::app_debug("end" + QString::number(st_file_detail_in_csv.size()) , Q_FUNC_INFO);
        return ;
    }

    QString csv_file_path = export_file_dir + "_hashes.csv";

    QFile m_file;
    m_file.setFileName(csv_file_path);
    if(!m_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED---" + m_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + m_file.errorString() , Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&m_file);
    QStringList csv_headerlist;
    csv_headerlist << "S.N" << "File Name" << "File Path";
    if(ui->checkBox_md5->isChecked())
    {
        csv_headerlist << "MD5";
    }
    if(ui->checkBox_sha1->isChecked())
    {
        csv_headerlist << "SHA1";
    }
    csv_headerlist << "Examiner Notes";

    for(int i = 0 ; i < csv_headerlist.size() ; i++)
    {
        outputfile_csv << "\"" + csv_headerlist.at(i) + "\"" + ",";
    }

    display_loading_progress_bar_obj->pub_set_label_messsge("Creating Hashes...   ");


    qint64 sr_number = 1;
    int qproc_counter = 0;


    QStringList displayfilePathlist;
    for(qint64 ii = 0 ; ii < st_file_detail_in_csv.size() ; ii++)
    {
        qproc_counter++;
        if(qproc_counter % 50 == 0)
        {
            qproc_counter = 0;
            QApplication::processEvents();
        }

        if(bool_cancel_loading)
            break ;

        struct_file_info_for_hashes obj = st_file_detail_in_csv.at(ii);

        QString complete_file_path = obj.complete_file_path;

        QString s_nmuber = QString::number(sr_number);
        QString file_name = obj.file_name;
        QString display_file_path = obj.display_file_path;
        QString notes_str = obj.examiner_notes_value;

        if(displayfilePathlist.contains(display_file_path))
            continue ;

        displayfilePathlist << display_file_path;


        QStringList csv_valuelist;
        csv_valuelist << s_nmuber << file_name << display_file_path;

        if(ui->checkBox_md5->isChecked())
        {
            csv_valuelist <<  recon_static_functions::create_md5(complete_file_path,Q_FUNC_INFO);
        }

        if(ui->checkBox_sha1->isChecked())
        {
            csv_valuelist << recon_static_functions::create_sha1(complete_file_path , Q_FUNC_INFO);
        }

        csv_valuelist << notes_str;

        outputfile_csv << "\n";///next row
        for(int jj = 0 ; jj < csv_headerlist.size() ; jj++)
        {
            QString value_str  = csv_valuelist.at(jj);
            outputfile_csv <<  "\"" + value_str + "\"" + ",";
        }

        sr_number++;
    }

    m_file.close();

    recon_static_functions::app_debug(" end" , Q_FUNC_INFO);
}


void tagged_file_export::set_zip_file_path()
{
    if(!ui->radioButton_export_to_zip->isChecked())
        return ;

    QString dir_name_with_date_time = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString() +  M_VAR_export_dir_name_QString + QString("_") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));
    dir_name_with_date_time = dir_name_with_date_time + ".zip";

    ui->lineEdit_di_path->setPlaceholderText(dir_name_with_date_time);
}


void tagged_file_export::on_checkBox_encrypt_zip_clicked(bool checked)
{
    //restore defaults
    lineedit_password_zip_enc->clear();
    label_status_zip_enc->clear();

    //show password dialog
    if(checked)
        dialog_enc_zip_password_asker->show();
}

void tagged_file_export::prepare_encrypted_zip_password_dialog_box()
{
    dialog_enc_zip_password_asker = new QDialog(this);
    dialog_enc_zip_password_asker->setWindowModality(Qt::WindowModal);
    connect(dialog_enc_zip_password_asker,SIGNAL(rejected()),this,SLOT(slot_rejected()));

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_hashset = new QVBoxLayout(dialog_enc_zip_password_asker);

    pushbutton_okay   = new QPushButton("OK",this);
    pushbutton_cancel = new QPushButton("Cancel",this);

    pushbutton_okay->setFixedSize(QSize(80, 25));
    pushbutton_cancel->setFixedSize(QSize(80, 25));

    connect(pushbutton_okay,SIGNAL(clicked()),this,SLOT(slot_enc_zip_dialog_okay_clicked()));
    connect(pushbutton_cancel,SIGNAL(clicked()),this,SLOT(slot_enc_zip_dialog_cancel_clicked()));

    pushbutton_okay->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_cancel->setAttribute(Qt::WA_MacShowFocusRect,false);

    pushbutton_password_show_hide_zip_enc   = new QPushButton("",this);
    pushbutton_password_show_hide_zip_enc->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_password_show_hide_zip_enc->setCheckable(true);
    connect(pushbutton_password_show_hide_zip_enc,SIGNAL(clicked(bool)),this,SLOT(slot_pushButton_show_hide_password_clicked(bool)));

    pushbutton_password_show_hide_zip_enc->setStyleSheet("QPuhButton:flat{border: none; background-color: transparent;}\nQPuhButton{background:transparent;}\nQPushButton:checked{ background-color: transparent; border: none;  } \n"
                                                         "QPushButton:clicked{ background-color: transparent;  border: none;}               \n"
                                                         "QPushButton:pressed { background-color: transparent; border: none; }");

    label_status_zip_enc = new QLabel(this);
    label_status_zip_enc->clear();


    pushbutton_password_show_hide_zip_enc->setFixedSize(QSize(45, 30));
    QPixmap button_open("../icons/signs/eye_open.png");
    QIcon icon;
    icon.addPixmap(button_open, QIcon::Normal, QIcon::Off);
    pushbutton_password_show_hide_zip_enc->setIcon(icon);
    pushbutton_password_show_hide_zip_enc->setIconSize(QSize(60, 30));
    pushbutton_password_show_hide_zip_enc->setAutoDefault(true);
    pushbutton_password_show_hide_zip_enc->setFlat(true);

    lineedit_password_zip_enc = new QLineEdit(this);
    lineedit_password_zip_enc->setEchoMode(QLineEdit::Password);
    lineedit_password_zip_enc->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_password_zip_enc->setPlaceholderText("Password...");


    QLabel *label_msg = new QLabel("Please enter the password for encrypted zip.", this);

    QVBoxLayout *vbox_layout_lbl_msg = new QVBoxLayout;
    vbox_layout_lbl_msg->addWidget(label_msg);

    QHBoxLayout *hbox_layout_lineedit = new QHBoxLayout;
    hbox_layout_lineedit->addWidget(lineedit_password_zip_enc);
    hbox_layout_lineedit->addWidget(pushbutton_password_show_hide_zip_enc);

    vbox_layout_lbl_msg->addLayout(hbox_layout_lineedit);

    QSpacerItem *spacer = new QSpacerItem(15,10,QSizePolicy::Expanding,QSizePolicy::Maximum);

    QHBoxLayout *hbox_layout_okay_cancel = new QHBoxLayout;
    hbox_layout_okay_cancel->addWidget(label_status_zip_enc);
    hbox_layout_okay_cancel->addSpacerItem(spacer);
    hbox_layout_okay_cancel->addWidget(pushbutton_cancel);
    hbox_layout_okay_cancel->addWidget(pushbutton_okay);

    vboxlayout_hashset->addLayout(vbox_layout_lbl_msg);
    vboxlayout_hashset->addLayout(hbox_layout_okay_cancel);

    vboxlayout_hashset->setContentsMargins(6,6,6,6);
    vboxlayout_hashset->setSpacing(3);

    dialog_enc_zip_password_asker->setLayout(vboxlayout_hashset);
    dialog_enc_zip_password_asker->setMinimumWidth(200);
    dialog_enc_zip_password_asker->setFont(font_s);
    dialog_enc_zip_password_asker->setFixedSize(QSize(300, 110));
}

void tagged_file_export::slot_enc_zip_dialog_okay_clicked()
{
    QString password_enc_zip = lineedit_password_zip_enc->text().trimmed();
    if(password_enc_zip.isEmpty())
    {
        QPalette palette = label_status_zip_enc->palette();
        palette.setColor(label_status_zip_enc->foregroundRole(),QColor(Qt::red));
        label_status_zip_enc->setPalette(palette);

        label_status_zip_enc->setText("<b>Invalid Password.</b>");
        return;
    }


    dialog_enc_zip_password_asker->hide();
}

void tagged_file_export::slot_enc_zip_dialog_cancel_clicked()
{
    dialog_enc_zip_password_asker->hide();
    ui->checkBox_encrypt_zip->setChecked(false);

}


void tagged_file_export::slot_pushButton_show_hide_password_clicked(bool checked)
{
    QIcon icon;
    QPixmap button_close;
    if(checked)
    {
        button_close.load("../icons/signs/eye_close.png");
        lineedit_password_zip_enc->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        button_close.load("../icons/signs/eye_open.png");
        lineedit_password_zip_enc->setEchoMode(QLineEdit::Password);
    }
    icon.addPixmap(button_close, QIcon::Normal, QIcon::Off);
    pushbutton_password_show_hide_zip_enc->setIcon(icon);
}

void tagged_file_export::point_to_export_dir(QString path)
{
    QStringList arg;
    arg << "-R" << path;
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}
