#include "sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

sqlite_viewer::sqlite_viewer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::sqlite_viewer)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_search->hide();
    ui->pushButton_search->hide();
    ui->pushButton_show_all->hide();

    ///- To Enble Drag And Drop on the window
    setAcceptDrops(true);
    set_widgets_disable(true);

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;



    bool_check_double_click = false;
    is_loadstate = 0;

    agency_logo_path = "resources/agency_logo.png";


    bool_global_export_checkbox_enable_sqlite_viewer = false;
    global_export_checkbox_disable_sqlite_viewer = 0;


    global_export_checkbox_disable_sqlite_viewer = 0;

    ui->checkBox_export->setEnabled(false);
    ui->pushButton_execute_query->setDisabled(true);

    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,0);

    base_core_class_obj = new sqlite_viewer_base_core_class(this);
    base_core_class_obj->pub_set_display_loading_bar(display_loading_progress_bar_obj);
    connect(base_core_class_obj,SIGNAL(signal_combobox_conversion_change()),this,SLOT(slot_savestate_on_combobox_conversion_change()));
    connect(base_core_class_obj,SIGNAL(signal_update_export_checkbox_and_count(bool,int)),this,SLOT(slot_update_sqlite_viewer_export_checkbox_and_count(bool,int)));


    load_saved_state_class_obj = new sqlite_viewer_load_saved_state_class(this);
    connect(load_saved_state_class_obj,SIGNAL(signal_onload_combobox_conversion_change()),this,SLOT(slot_onload_savestate_on_combobox_conversion_change()));
    connect(load_saved_state_class_obj,SIGNAL(signal_onload_update_export_checkbox_and_count(bool,int)),this,SLOT(slot_update_sqlite_viewer_export_checkbox_and_count(bool,int)));

    ///- Right Click Menu bar.
    menu = new QMenu(this);
    QFont menu_font;
    menu_font.setPointSize(MACRO_QMenu_Font_Size_int);
    menu->setFont(menu_font);

    submenu_bookmark = menu->addMenu(tr(MACRO_Generic_Right_Click_Bookmark));

    act_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Bookmark), this->submenu_bookmark);
    act_remove_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Bookmarks), this->submenu_bookmark);

    submenu_bookmark->addAction(act_bookmark);
    submenu_bookmark->addAction(act_remove_bookmark);


    //----- Tag Work..
    add_tags_module_obj = new add_tags(this);
    remove_multiple_tag_selecotor_obj = new remove_multiple_tag_selecotor(this);
    sub_menu_tags =  menu->addMenu(tr(MACRO_Generic_Right_Click_Tags));
    sub_menu_tags->setFont(menu_font);

    //Notes
    submenu_notes = menu->addMenu(tr(MACRO_Generic_Right_Click_Notes));

    act_add_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Note), this->submenu_notes);
    act_remove_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Note), this->submenu_notes);

    submenu_notes->addAction(act_add_notes);
    submenu_notes->addAction(act_remove_notes);

    act_seperator_bookmark = new QAction(this);
    act_seperator_bookmark->setSeparator(true);

    menu->addAction(act_seperator_bookmark);

    act_export = new QAction(QObject::tr(MACRO_Generic_Right_Click_Export), this);

    act_remove_notes_from_bookmarks = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks), this);
    act_columns_selection = new QAction(QObject::tr(MACRO_Generic_Right_Click_Manage_Columns), this);
    act_copy_to_clipboard = new QAction(QObject::tr(MACRO_Generic_Right_Click_Copy_To_Clipboard), this);

    connect(act_copy_to_clipboard, SIGNAL(triggered()), this, SLOT(slot_act_copy_to_clipboard()));
    connect(act_columns_selection,SIGNAL(triggered()),this,SLOT(slot_act_columns_selection()));
    //    connect(act_remove_all_bookmark, SIGNAL(triggered()), this, SLOT(slot_act_remove_all_bookmark()));
    connect(act_add_notes, SIGNAL(triggered()), this, SLOT(slot_act_add_notes()));
    //    connect(act_add_notes_to_bookmark, SIGNAL(triggered()), this, SLOT(slot_act_add_notes_to_bookmarks()));
    connect(act_export, SIGNAL(triggered()), this, SLOT(slot_act_export()));
    //  connect(act_remove_all_notes, SIGNAL(triggered()), this, SLOT(slot_act_remove_all_notes()));
    //    connect(act_remove_notes_from_bookmarks, SIGNAL(triggered()), this, SLOT(slot_act_remove_notes_from_bookmarks()));
    connect(act_remove_notes, SIGNAL(triggered()), this, SLOT(slot_act_remove_notes()));

    act_seperator_export = new QAction(this);
    act_seperator_export->setSeparator(true);

    act_full_detail_info    =   new QAction(tr(MACRO_Generic_Right_Click_Open_In_Window), this);
    connect(act_full_detail_info, SIGNAL(triggered()), this, SLOT(slot_full_detail_info_triggered()));

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked_for_sqlite_viewer(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

    connect(this, SIGNAL(signal_add_tags_sqlite_viewer(QString,QString)), this, SLOT(slot_add_tags_to_sqlite_viewer(QString,QString)));

    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_in_actions_menu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags_to_sqlite_viewer(QString,QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_remaining_tags_list(QStringList, QString , QString)),this,SLOT(slot_tag_list_after_remove_multiple_tag_sqlite_viewer(QStringList, QString , QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_tag_list_to_be_removed(QStringList, QString , QString)),this,SLOT(slot_removed_tags_list_sqlite_viewer(QStringList, QString , QString)));
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slot_rignt_click_menu_trigerred_sqlite_viewer(QAction*)));

    menu->addAction(act_full_detail_info);
    menu->addAction(act_copy_to_clipboard);
    menu->addAction(act_seperator_export);
    menu->addAction(act_export);
    menu->addAction(act_columns_selection);

    installEventFilter(this);

    ///- Right Click Add Notes Dialog
    add_notes_dialog_popup = new add_notes(this);
    connect(add_notes_dialog_popup, SIGNAL(signal_notes(QString)), this, SLOT(slot_add_notes(QString)));

    add_notes_to_bookmarks_dialog_popup = new add_notes(this);
    //    connect(add_notes_to_bookmarks_dialog_popup, SIGNAL(signal_notes(QString)), this, SLOT(slot_add_notes_to_bookmarks(QString)));
    ///-End

    check_bookmark_status = false;
    bool_index_change_from_comboBox = false;
    bool_set_enable_right_click_export = false;
    bool_saved_state_successfull = false;
    bool_combotext_change_on_tab_change = false;

    //    connect(&process_html_to_pdf,SIGNAL(finished(int)),this,SLOT(slot_html_to_pdf_finished(int)));
    connect(ui->tabWidget1,SIGNAL(tabBarClicked(int)),this,SLOT(slot_tab_bar_clicked(int)));


    ui->tabWidget->setTabText(0,"Tables");
    connect(ui->lineEdit_search,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(slot_show_all(int,int)));

    current_tab_index = 0;

    message_dialog_object = new message_dialog(this);
    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    pdf_report_module_obj = new pdf_report_module(this);
    connect(this, SIGNAL(signal_create_pdf_report(QString,QString)), pdf_report_module_obj, SIGNAL(signal_create_pdf(QString,QString)));
    connect(pdf_report_module_obj, SIGNAL(signal_pdf_finished(QString,bool)), this, SLOT(slot_html_to_pdf_finished(QString, bool)));

    ui->tabWidget->setCurrentIndex(0); //sqlite_viewer

}

sqlite_viewer::~sqlite_viewer()
{
    delete ui;

    QFile file(export_db_destination_path);
    if(file.exists())
        file.remove();
}


void sqlite_viewer::pub_set_global_connection_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void sqlite_viewer::pub_set_recon_case_obj(RECON_case  *obj)
{
    recon_case_obj = obj;
}

QString sqlite_viewer::saved_state_dir_name()
{
    return QString("SavedStateSqlite");
}

QString sqlite_viewer::main_datadisplay_table_sqlite_name()
{
    return QString("MainDataDisplay_SavedState.sqlite");
}

QString sqlite_viewer::filter_view_table_sqlite_name()
{
    return QString("FilterViewTable_SavedState.sqlite");
}

bool sqlite_viewer::eventFilter(QObject *obj, QEvent *event)
{
    struct_global_sqlite_viewer_tablewidget_main structMainObject;

    if(event->type() == QEvent::ChildAdded)
        return false;

    if(obj == ui->tableWidget_query_execution)
    {
        if(event->type() == QEvent::ContextMenu)
        {
            QModelIndexList selection_model_list = ui->tableWidget_query_execution->selectionModel()->selectedRows();
            if(selection_model_list.size() < 1)
                return false;


            QList <QAction *> act_list = menu->actions();
            for(int i = 0 ; i < act_list.size() ; i++)
            {
                if((act_list.at(i)->text() == QString(MACRO_Generic_Right_Click_Tags)) || (act_list.at(i)->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard)))
                    act_list.at(i)->setEnabled(true);
                else
                    act_list.at(i)->setEnabled(false);
            }

            menu->exec(QCursor::pos());
            return true;
        }

        return false;
    }

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        structMainObject = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        return false;
    }

    if(obj == structMainObject.tableWidget_MainDataDisplay_ptr)
    {
        if(bool_global_export_checkbox_enable_sqlite_viewer)
        {
            if(global_export_checkbox_disable_sqlite_viewer == 0)
            {
                ui->checkBox_export->setEnabled(false);
            }
            else
                ui->checkBox_export->setEnabled(true);
        }
        else
        {
            ui->checkBox_export->setEnabled(false);
            ui->checkBox_export->setCheckState(Qt::Unchecked);
        }

        if(bool_set_enable_right_click_export)
        {
            act_export->setEnabled(true);
        }
        else
        {
            act_export->setEnabled(false);
        }

        if(event->type() == QEvent::ContextMenu)
        {
            QModelIndexList selection_model_list = structMainObject.tableWidget_MainDataDisplay_ptr->selectionModel()->selectedRows();
            if(selection_model_list.size() < 1)
                return false;

            slot_on_tableWidget_MainDataDisplay_cellClicked(structMainObject.tableWidget_MainDataDisplay_ptr->currentRow(),structMainObject.tableWidget_MainDataDisplay_ptr->currentColumn());

            menu->exec(QCursor::pos());
            return true;
        }

        return false;
    }

    return false;
}

void sqlite_viewer::set_widgets_disable(bool status)
{
    ui->tabWidget_FilterViewSearch->setDisabled(status);
    ui->tabWidget_main_data_panel->setDisabled(status);
    ui->tab_2->setDisabled(status);
    ui->comboBox_dbtables_selection->setDisabled(status);
    ui->pushButton_generate_report->setDisabled(status);
}

void sqlite_viewer::reset_gui_widgets()
{
    emit signal_refresh_widgets();
}

sqlite_viewer::sqlite_viewer_st_export_file_detail sqlite_viewer::get_export_detail_html_final(QString column, QString tablename, QString is_multiple_export)
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    sqlite_viewer_st_export_file_detail obj;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(export_db_destination_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-- destination_db open ----FAILED--",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("db path" + export_db_destination_path,Q_FUNC_INFO);
        return obj;
    }

    QSqlQuery query_export_detail(destination_db);

    query_export_detail.prepare("SELECT exp_relativepath, exp_size, exp_hashmd5, exp_hashsha1, exp_time,"
                                "filepath FROM exportfilelist WHERE table_name = ? AND INT = ?");
    query_export_detail.addBindValue(tablename);
    query_export_detail.addBindValue(column);
    if(!query_export_detail.exec())
    {
        recon_static_functions::app_debug("---FAILED--- query_export_detail",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_export_detail.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + query_export_detail.executedQuery(),Q_FUNC_INFO);
        return obj;
    }

    while(query_export_detail.next())
    {
        QString exp_detail;
        QString exp_preview;

        QString relative_path = query_export_detail.value(0).toString();
        relative_path = relative_path.trimmed();

        QString org_filepath = query_export_detail.value(5).toString();
        QFileInfo info8(org_filepath);
        QString original_filename = info8.fileName();

        if(relative_path == QString(""))
        {
            if(org_filepath.trimmed() == "")
            {
                exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                        + QString("<b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");
            }
            else
            {
                exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                        + "<b>"+  QObject::tr("Original File") + ": </b>" +"\t"+ original_filename +"\t"
                        + QString("<br><b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");
            }
        }
        else
        {
            QString image, hyperlink;

            QString resource_dir;

            resource_dir = "./resources/";

            QString file_name = relative_path;
            file_name = file_name.remove("./tmp/");


            QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
            QString numeric_timezone_value;
            if(machine_timezone_string.contains("GMT"))
            {
                QStringList tmp_list = machine_timezone_string.split("GMT");
                numeric_timezone_value = tmp_list.at(1);
            }

            exp_detail = QString("<br><b> <u> <i>"+  QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                    + "<b>"+  QObject::tr("Original File") + ": </b>" +"\t"+ original_filename +"\t<br>"
                    + "<b>"+  QObject::tr("Exported File") + ": </b>" +"\t"+ relative_path +"\t<br>"
                    + "<b>"+  QObject::tr("Size") + ": </b>" + recon_static_functions::human_readable_size(query_export_detail.value(1).toString(), Q_FUNC_INFO)
                    + " (" + query_export_detail.value(1).toString() + QObject::tr(" bytes") + ")" + "\t<br>"
                    + "<b>"+  QObject::tr("MD5") + ": </b>" +"\t"+ query_export_detail.value(2).toString() +"<br>"
                    +"<b>"+  QObject::tr("SHA1") + ": </b>" +"\t"+ query_export_detail.value(3).toString() +"\t<br>"
                    + "<b>"+  QObject::tr("Export Time") + ": </b>" +"\t"+ QDateTime::currentDateTime().toString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString()) + "&nbsp;" + numeric_timezone_value +  "<br><br>";
            exp_preview =  hyperlink + image + "</a>";
        }

        if(is_multiple_export == "2")
        {
            obj.multiple_export_filedetail << exp_detail;
            obj.multiple_export_flepreview << exp_preview;
        }
        else
        {
            obj.export_filedetail = exp_detail;
        }
    }

    destination_db.close();
    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
    return obj;
}

QStringList sqlite_viewer::get_export_detail_csv(QString column, QString tablename)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    QStringList tabledata;
    tabledata.clear();

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(export_db_destination_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("dest_db  ----FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(export_db_destination_path,Q_FUNC_INFO);
        return tabledata;
    }

    QSqlQuery query_export_detail(destination_db);
    QString export_detail;

    query_export_detail.prepare("SELECT exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time"
                                " FROM exportfilelist WHERE table_name = ? AND INT = ?");
    query_export_detail.addBindValue(tablename);
    query_export_detail.addBindValue(column);
    if(!query_export_detail.exec())
    {
        recon_static_functions::app_debug(" query_export_detail ---FAILED----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_export_detail.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + query_export_detail.executedQuery(),Q_FUNC_INFO);
    }

    while(query_export_detail.next())
    {
        QString temp = query_export_detail.value(1).toString();
        QString file_size;
        if(temp == QString(""))
            file_size = QString("");
        else
            file_size = recon_static_functions::human_readable_size(temp, Q_FUNC_INFO) + " (" + temp + QObject::tr(" bytes") + ")";

        QString f_relative_path = query_export_detail.value(0).toString();
        QString relative_path =  QObject::tr("New Path") + ": " + f_relative_path + "      ";
        tabledata << QString("\"") + f_relative_path + QString("\"");

        QString size =  QObject::tr("Size")+  ": " + file_size + "    ";
        tabledata << QString("\"") + file_size + QString("\"");

        QString f_md5_string  = query_export_detail.value(2).toString();
        QString md5_string = QObject::tr("MD5")+  ": " + f_md5_string + "      ";
        tabledata << QString("\"") + f_md5_string + QString("\"");

        QString f_sha1_string = query_export_detail.value(3).toString();
        QString sha1_string =  QObject::tr("SHA1") + ": " + f_sha1_string + "      ";
        tabledata << QString("\"") + f_sha1_string + QString("\"");

        QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
        QString numeric_timezone_value;
        if(machine_timezone_string.contains("GMT"))
        {
            QStringList tmp_list = machine_timezone_string.split("GMT");
            numeric_timezone_value = tmp_list.at(1);
        }

        QString f_export_time = QDateTime::currentDateTime().toString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString()) ;
        QString export_time = QObject::tr("Export Time") +": " + f_export_time + " " + numeric_timezone_value ;
        tabledata << QString("\"") + f_export_time  + numeric_timezone_value + QString("\"");

        export_detail.append(relative_path).append(size).append(md5_string).append(sha1_string).append(export_time);
    }

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);
    destination_db.close();
    return tabledata;
}

QString sqlite_viewer::set_recon_result_dir(QString sqlite_db_path, QString tab_text)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    recieved_sqlite_path_from_mainwindow.clear();
    recieved_sqlite_path_from_mainwindow = sqlite_db_path;

    sqlite_viewer_display_name_in_tree_and_tab = tab_text;



    load_sqlite_and_prepare_data(tab_text);

    display_loading_progress_bar_obj->pub_set_label_messsge("Saving Current State.....");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_obj->show();


    display_tags_submenu_for_sqlite_viewer();

    create_or_update_current_save_state_of_sqlite_viewer();

    QFile file2remove(copied_source_file);
    file2remove.remove();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

    return "";
}

void sqlite_viewer::set_values_obtain_from_sqlite_viewer_index_db(QString destination_db_file,QString targeted_sqlite)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("SELECT file_path, copied_file_path, recreated_file_path, destination_directory_path, is_savestate,"
                  "savedstate_dest_dir_path, source_count_name , INT from 'sv_index' WHERE viewer_display_name = ?");
    query.addBindValue(targeted_sqlite);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    source_file.clear();
    copied_source_file.clear();
    saved_execute_query_maindb_path.clear();
    sqlite_viewer_save_state_path.clear();

    while(query.next())
    {
        if(query.value(4).toString() == QString::number(1))
        {
            saved_state_dir_path.clear();
            saved_state_dir_path = query.value(5).toString();

            if(!saved_state_dir_path.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
                saved_state_dir_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

            sqlite_viewer_save_state_path.clear();
            sqlite_viewer_save_state_path = query.value(3).toString() + QString("/") + QString("SavedState");

            if(!query.value(3).toString().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
                sqlite_viewer_save_state_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

            is_loadstate =1;
            // break;
        }

        source_file = recon_helper_standard_obj->get_file_infopath_by_dbreference(query.value("INT").toString() ,destination_db ,"sv_index",query.value("source_count_name").toString(),Q_FUNC_INFO );

        copied_source_file = query.value(1).toString();
        if(!copied_source_file.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            copied_source_file.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        saved_execute_query_maindb_path = query.value(2).toString();
        if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        sqlite_viewer_save_state_path = query.value(3).toString();
        if(!sqlite_viewer_save_state_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            sqlite_viewer_save_state_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        source_count_name = query.value(6).toString();
    }

    destination_db.close();
    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}

QString sqlite_viewer::convert_data(QString data_item, QString data_type)
{
    bool_for_the_unicode_conversion = false;

    if(data_type.contains("Select Conversion"))
    {
        data_item;
    }
    else if(data_type.contains("File"))
    {
        QString file_path = data_item;

        ///- Show File Existance
        QString file_exist;
        QFileInfo info(file_path);
        if(info.exists())
        {
            file_exist = QString("<b>File Exist: </b>" +QString("\t")+ "<font  color=\"green\"> <b>" "YES" " </b></font>"+"<br>" );
        }
        else
        {
            file_exist = QString("<b>File Exist: </b>" +QString("\t")+ "<font  color=\"red\"> <b>" "NO" "</b></font>"+"<br>" );
        }

        data_item = file_path + "<br>" + file_exist;
    }
    else if(data_type.contains("Size Human Readable"))
    {
        data_item = recon_static_functions::human_readable_size(data_item, Q_FUNC_INFO);
    }
    else if(data_type.contains("Unix Timestamp"))
    {
        data_item = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(data_item, Q_FUNC_INFO);
    }
    //  else if(data_type.contains("Mac HFS+ Timestamp"))
    else if(data_type.contains("Cocoa Core Data Timestamp"))
    {
        //data_item = convert_safari_time_to_human_readable(data_item);
        data_item = global_recon_helper_singular_class_obj->convert_safari_time_to_human_readable(data_item, Q_FUNC_INFO);
    }
    else if(data_type.contains("WebKit/Chrome Timestamp"))
    {
        //data_item = convert_chromium_time_to_epoch(data_item);
        data_item = global_recon_helper_singular_class_obj->convert_webkit_chrome_time_to_human_readable(data_item, Q_FUNC_INFO);
    }
    else if(data_type.contains("ASCII"))
    {
        QByteArray value_str = data_item.toLocal8Bit();
        data_item = global_recon_helper_singular_class_obj->convert_raw_data_to_ascii_convertor(value_str, Q_FUNC_INFO);
    }

    else if(data_type.contains("Unicode"))
    {
        QByteArray value_str = data_item.toLocal8Bit();
        data_item = global_recon_helper_singular_class_obj->convert_raw_data_to_unicode_convertor(value_str, Q_FUNC_INFO);
        bool_for_the_unicode_conversion = true;
    }
    return data_item;
}

void sqlite_viewer::display_tags_submenu_for_sqlite_viewer()
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_show_tag()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_show_tag()));

    ///remove tag
    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    sub_menu_tags->addAction(act_remove_tag);

    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_remove_tags_from_sqlite_viewer()));
    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_remove_tags_from_sqlite_viewer()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = add_tags_module_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path);

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

            icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
            QIcon icon(icon_path);

            act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);

}

QString sqlite_viewer::create_and_insert_exported_content_info_in_db(int current_row,QString record_no_in_sqliteviewer, QString tag_name, QString sqlite_viewer_display_name, QStringList query_data_list, QString sql_tab_name , QString status)
{
    recon_static_functions::app_debug("START" , Q_FUNC_INFO);

    QString exported_content_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

    QFileInfo info(exported_content_db_file);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'sv_content_info'(INT INTEGER PRIMARY KEY,bookmark INTEGER,file_name VARCHAR(1000), file_path VARCHAR(1000),recon_filefrom VARCHAR(1000) , recon_file_infopath VARCHAR(1000),"
                          "file_size VARCHAR(1000), recon_tag_value VARCHAR(1000),source_count_name VARCHAR(1000),viewer_display_name VARCHAR(1000),db_table_name VARCHAR(1000),record_no_in_sqliteviewer VARCHAR(1000),sqlite_query_data VARCHAR(1000),"
                          "sql_tab_name VARCHAR(1000),notes VARCHAR(1000),LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000))";
        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_db_file,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table sqlite_content_export_info in Sqlite viewer ---FAILED---" + exported_content_db_file,Q_FUNC_INFO);
        }


        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_db_file, Q_FUNC_INFO);


    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + exported_content_db_file , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return "";
    }

    QFileInfo info_temp(complete_file_path);

    QString query_data_str;
    for(int i = 0; i < query_data_list.size() ; i++ )
    {
        QStringList data_list = query_data_list.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(data_list.size() < 2)
            continue;

        QString header = data_list.at(0).trimmed();
        QString data_str = data_list.at(1).trimmed();

        query_data_str += QString("<b>" + header + ":</b>" +QString("\t")+ data_str +"\n<br><br>");
    }

    QString notes_text;
    QStringList arg_previous_list;
    arg_previous_list <<  record_no_in_sqliteviewer << sql_tab_name << actual_file_path;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference("select INT from sv_content_info where record_no_in_sqliteviewer = ? AND sql_tab_name = ? AND file_path = ?", arg_previous_list, 0, destination_db, Q_FUNC_INFO);
    if(INT_str.isEmpty())
    {
        QSqlQuery query_insert(destination_db);
        query_insert.prepare("INSERT INTO 'sv_content_info'(bookmark,file_name, file_path, file_size,  recon_tag_value,notes,"
                             " source_count_name, viewer_display_name,db_table_name,record_no_in_sqliteviewer"
                             ", sqlite_query_data, sql_tab_name,source_file,recon_filefrom,recon_file_infopath) VALUES(?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?)");
        query_insert.addBindValue(status);


        query_insert.addBindValue(QFileInfo(actual_file_path).fileName());
        query_insert.addBindValue(actual_file_path);
        query_insert.addBindValue(info_temp.size());

        if(tag_name == MACRO_Tag_Name_Bookmarks)
        {
            ////---tag empty for notes
            query_insert.addBindValue("");
        }
        else if(tag_name.endsWith(MACRO_CASE_TREE_Notes_Category_All_Notes))
        {
            QStringList tm_list = tag_name.split(MACRO_RECON_Splitter_1_multiple);
            if(tm_list.size() > 0)
                notes_text = tm_list.at(0);

            ////---tag empty for notes
            query_insert.addBindValue("");
        }
        else
        {
            ////insert for tag
            query_insert.addBindValue(tag_name);
        }


        query_insert.addBindValue(notes_text);

        query_insert.addBindValue(source_count_name);
        query_insert.addBindValue(sqlite_viewer_display_name);
        query_insert.addBindValue(current_tab_name);

        query_insert.addBindValue(record_no_in_sqliteviewer);
        query_insert.addBindValue(query_data_list.join(";"));
        query_insert.addBindValue(sql_tab_name);
        query_insert.addBindValue(actual_file_path);
        query_insert.addBindValue(recon_file_from);
        query_insert.addBindValue(recon_file_info_path);


        if(!query_insert.exec())
        {
            recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Query -  " + query_insert.executedQuery(),Q_FUNC_INFO);
        }
    }
    else
    {
        QString command;
        if(find_tagging_on_record(current_row))
        {
            command = "UPDATE sv_content_info SET recon_tag_value = ?  , notes = ?  , bookmark = ?, sqlite_query_data = ? where record_no_in_sqliteviewer = ?  AND sql_tab_name = ? AND file_path = ?";
            QStringList arg_list;
            arg_list << recon_tag_name
                     << recon_notes_name
                     << recon_bookmark_name
                     << query_data_list.join(";")
                     << record_no_in_sqliteviewer
                     << sql_tab_name
                     << actual_file_path;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_db_file,Q_FUNC_INFO);

        }
        else
        {
            command = "DELETE from sv_content_info  where record_no_in_sqliteviewer = ? AND sql_tab_name = ? AND file_path = ?";
            QStringList arg_list;
            arg_list <<  record_no_in_sqliteviewer << sql_tab_name << actual_file_path;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_db_file,Q_FUNC_INFO);
        }
    }

    if(tag_name == MACRO_Tag_Name_Bookmarks)
    {
        update_bookmark_value_in_bookmark_serach(record_no_in_sqliteviewer,QString(MACRO_Plugin_Name_Sqlite_Viewer),sql_tab_name,status,"",query_data_list.join(";"),actual_file_path,QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display),sqlite_viewer_display_name_in_tree_and_tab , current_tab_name);
    }
    else if(tag_name.endsWith(MACRO_CASE_TREE_Notes_Category_All_Notes))///done via global connections
    {
        destination_db.close();
        recon_static_functions::app_debug("END" , Q_FUNC_INFO);
        return exported_content_db_file;
    }
    else
    {
        if(!tag_name.trimmed().isEmpty())
        {
            update_tag_in_tag_serach_db(actual_file_path,  tag_name,  record_no_in_sqliteviewer, sqlite_viewer_display_name_in_tree_and_tab,source_count_name,current_tab_name, query_data_list.join(";"), sql_tab_name);
            delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(tag_name , record_no_in_sqliteviewer , QString(MACRO_Plugin_Name_Sqlite_Viewer) , sql_tab_name);
        }
    }


    destination_db.close();
    recon_static_functions::app_debug("END" , Q_FUNC_INFO);
    return exported_content_db_file;
}

QString sqlite_viewer::update_tag_status_for_current_record(QString tag_data, QString status, QString record_no_str, QString sql_tab_name, QStringList text_data)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

    QFileInfo info(destination_db_path);
    if(!info.exists())
    {
        return tag_data;
    }

    QString command = "UPDATE sv_content_info SET recon_tag_value = ? WHERE record_no_in_sqliteviewer = ? AND sql_tab_name = ? AND sqlite_query_data = ?";
    QStringList arg_list;
    arg_list << tag_data << record_no_str << sql_tab_name << text_data.join(";");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return tag_data;
}

void sqlite_viewer::export_file_fun(int value, QString table_name,QString filey_path)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    file_copy_source_list.clear();
    file_copy_destination_list.clear();
    file_copy_recordno_list.clear();
    file_copy_tablename_list.clear();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(export_db_destination_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("export db opening ---FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString dest_path = filey_path;
    dest_path.remove("/index.html");
    dest_path.remove("/index.csv");
    dest_path.remove("/index.xml");
    dest_path = dest_path + "/file";

    QDir dir;
    dir.setPath(dest_path);
    if(!dir.exists())
        dir.mkpath(dest_path);

    QString file_path_column;

    for(int i = 0; i < ui->comboBox_dbtables_selection->count(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main struct_main_object = list_struct_tablewidget_data_sqlite_viewer.at(i);

        m_tablewidget *tableWidget_main_data_display = struct_main_object.tableWidget_MainDataDisplay_ptr;

        QTableWidget *tableWidget_filter_view_search = struct_main_object.tableWidget_FilterViewSearch_ptr;
        QTableWidget *tableWidget_column_selection = struct_main_object.tableWidget_columns_selection_ptr;

        if(struct_main_object.table_name != table_name)
        {
            continue;
        }

        for(int j = 0; j < export_columns_name_list.size(); j++)
        {
            file_path_column = export_columns_name_list.at(j);

            int export_col_no=0;
            for(int k = 0; k < tableWidget_filter_view_search->rowCount(); k++)
            {
                QString recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_column_selection->cellWidget(k ,2))->currentText());

                if(recivedComboboxText == "File")
                {
                    QString recivedColmnnameText =  tableWidget_filter_view_search->item(k,0)->text();
                    if(recivedColmnnameText == file_path_column)
                    {
                        export_col_no = k;
                        break;
                    }
                }
            }

            if(value == 1 || value == 2 || value == 3)
            {
                for(int row = 0; row < tableWidget_main_data_display->rowCount(); row++)
                {
                    if(value == 2)
                    {
                        if(tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Unchecked)
                            continue;
                    }

                    if(value == 3)
                    {
                        if(tableWidget_main_data_display->isRowHidden(row))
                            continue;
                    }


                    QString file_path = tableWidget_main_data_display->item(row,export_col_no)->text();
                    QString record = tableWidget_main_data_display->item(row,enum_coloumn_Sqlite_viewer_record_no)->text();


                    if(file_path == "")
                        continue;

                    QStringList all_path_list;
                    all_path_list << file_path;

                    for(int i =0 ; i< all_path_list.size() ; i++)
                    {
                        QString complete_path = all_path_list.at(i);

                        QFileInfo info(complete_path);
                        if(!info.exists())
                        {
                            recon_static_functions::app_debug("export file does not exist path " + info.filePath(),Q_FUNC_INFO);

                            QSqlQuery query_export_exist(destination_db);
                            QString cmd = "";

                            cmd = QString("insert into exportfilelist (table_name,INT, filepath,"
                                          "exp_newpath,exp_relativepath,exp_size,"
                                          "exp_time) values (") +
                                    + "\"" + table_name + "\"," +
                                    + "\"" + record + "\"," +
                                    //+ "\"" + vm_name + "\"," +
                                    // "\"" + vm_name + "\"," +
                                    "\"" + info.filePath() + "\"," +
                                    "\"" + "" + "\"," +
                                    "\"" + "" + "\"," +
                                    "\"" + "" + "\"," +
                                    "\"" + "" + "\")";

                            if(!query_export_exist.exec(cmd))
                            {
                                recon_static_functions::app_debug("---FAILED---- to execute query_export_exist",Q_FUNC_INFO);
                                recon_static_functions::app_debug(query_export_exist.lastError().text(),Q_FUNC_INFO);
                                recon_static_functions::app_debug(cmd,Q_FUNC_INFO);
                            }

                            continue;
                        }

                        QFile file(info.filePath());
                        QString dest_file_path = dest_path + "/" + info.fileName();

                        int cnt = 0;

                        recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
                        while(1)
                        {
                            bool file_dest_exists = false;
                            for(int p = 0 ; p < file_copy_destination_list.size(); p++)
                            {
                                if(dest_file_path == file_copy_destination_list.at(p))
                                {
                                    file_dest_exists = true;
                                    break;
                                }
                            }

                            QFileInfo infocheck(dest_file_path);
                            if(file_dest_exists || infocheck.exists())
                            {
                                cnt++;
                                dest_file_path = dest_path + "/" + QString::number(cnt) + "_" + info.fileName();
                                //  break;
                            }
                            else
                            {
                                break;
                            }
                        }
                        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


                        file_copy_source_list << file.fileName();
                        file_copy_destination_list << dest_file_path;
                        file_copy_recordno_list << record;
                        file_copy_tablename_list << table_name;

                        QFileInfo file_info(dest_file_path);
                        QString file_relativepath = "./file/" + file_info.fileName();

                        qint64 pp  = QDateTime::currentMSecsSinceEpoch()/1000;
                        QString export_time = QString::number(pp);

                        bool ok = true;
                        qint64 temp = export_time.toULongLong(&ok);
                        //    temp -= system_time_offset;
                        export_time = QString::number(temp);

                        QSqlQuery query_export(destination_db);
                        query_export.prepare("INSERT INTO exportfilelist (table_name,INT, filepath, exp_newpath, exp_relativepath, exp_size, exp_time) values (?,?,?,?,?, ?,?)");

                        query_export.addBindValue(table_name);
                        query_export.addBindValue(record);
                        query_export.addBindValue(file.fileName());
                        query_export.addBindValue(dest_file_path);
                        query_export.addBindValue(file_relativepath);

                        query_export.addBindValue(recon_static_functions::mining_size(file.fileName(), Q_FUNC_INFO));
                        query_export.addBindValue(export_time);

                        if(!query_export.exec())
                        {
                            recon_static_functions::app_debug("---FAILED---- to execute query_export",Q_FUNC_INFO);
                            recon_static_functions::app_debug(query_export.lastError().text(),Q_FUNC_INFO);
                            recon_static_functions::app_debug(query_export.lastQuery(),Q_FUNC_INFO);
                            return;
                        }

                    }//all_path_list
                }
            }//if
        }//export_columns_name_list
    }

    destination_db.close();


    thread_file_copier_module_sqlite_viewer *thread_file_copier_module_sqlite_viewer_obj = new thread_file_copier_module_sqlite_viewer(this);

    thread_file_copier_module_sqlite_viewer_obj->set_destination_list(file_copy_destination_list);
    thread_file_copier_module_sqlite_viewer_obj->set_source_list(file_copy_source_list);
    thread_file_copier_module_sqlite_viewer_obj->set_destination_db(export_db_destination_path);
    thread_file_copier_module_sqlite_viewer_obj->set_tablename(file_copy_tablename_list);
    thread_file_copier_module_sqlite_viewer_obj->set_record_no(file_copy_recordno_list);

    thread_file_copier_module_sqlite_viewer_obj->start();

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(thread_file_copier_module_sqlite_viewer_obj->isFinished())
            break;
        QCoreApplication::processEvents();

    }

    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    recon_static_functions::app_debug("ENDS",Q_FUNC_INFO);
}

void sqlite_viewer::update_savedstate_info_into_index_db_for_current_state()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString command;
    command = "UPDATE sv_index SET is_savestate= 1, savedstate_dest_dir_path = ? WHERE viewer_display_name = ?";
    QStringList values;
    QString temp_save_state_path = saved_state_dir_path;
    if(temp_save_state_path.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
        temp_save_state_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString().size());

    values << temp_save_state_path << sqlite_viewer_display_name_in_tree_and_tab;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,values,recieved_sqlite_path_from_mainwindow,Q_FUNC_INFO);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void sqlite_viewer::set_sqlite_viewer_report_generation_status(QString report_path)
{
    sqlite_viewer_report_status obj;
    if(obj.exec())
    {
        QStringList arg;
        arg << report_path;

        QProcess *myProcess = new QProcess;
        myProcess->startDetached("open",arg);
    }
}

void sqlite_viewer::create_or_update_current_save_state_of_sqlite_viewer()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    set_source_file_essential();

    QString command;
    command = "SELECT is_savestate FROM sv_index WHERE viewer_display_name = ?";

    QStringList values;
    values << sqlite_viewer_display_name_in_tree_and_tab;

    QString already_exist = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,values, 0,recieved_sqlite_path_from_mainwindow, Q_FUNC_INFO);
    if(already_exist == QString::number(1))
    {
        return;
    }

    bool_saved_state_successfull = false;
    saved_state_dir_path.clear();

    QString recived_custom_detination_path = sqlite_viewer_save_state_path;


    QTime time;
    time = QTime::currentTime();
    QString time_string = time.toString("HH-mm-ss");

    QDate m_date = QDate::currentDate();
    QString date_string = m_date.toString("MM-dd-yyyy");

    QString saved_DirPath;

    saved_DirPath =  recived_custom_detination_path + "/" + QString("SavedState/") + saved_state_dir_name() + date_string + "T" + time_string;

    QDir dir;
    if(!dir.mkpath(saved_DirPath))
    {
        recon_static_functions::app_debug("Default Saved State Dir Create -----FAILED-------",Q_FUNC_INFO);
        bool_saved_state_successfull = false;
        return;
    }
    else
    {
        bool_saved_state_successfull = true;
    }

    saved_state_dir_path = saved_DirPath;

    save_current_state();

    if(bool_saved_state_successfull)
    {
        recon_static_functions::app_debug("----STATE SAVED SUCCESS---- ",Q_FUNC_INFO);
    }

    update_savedstate_info_into_index_db_for_current_state();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::load_savedstate_of_sqlite()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString path;
    path = saved_state_dir_path;

    ui->tabWidget_FilterViewSearch->clear();
    ui->tabWidget_main_data_panel->clear();

    bool_combobox_clear = false;
    ui->comboBox_dbtables_selection->clear();
    bool_combobox_clear = true;

    QString final_dir_str;
    QStringList mylist = path.split("/", Qt::SkipEmptyParts);
    for(int i=0; i < mylist.size(); i++)
    {
        final_dir_str = mylist.at(i);
    }

    if(!final_dir_str.startsWith("SavedStateSqlite"))
    {
        return;
    }

    QFileInfo info_filtertable(path + "/" + filter_view_table_sqlite_name() );

    if(!info_filtertable.exists())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Loading Wrong Result or Result is Corrupted !!!");
        message_dialog_object->show();
        return;
    }

    QFileInfo info_Maintable(path + "/" + main_datadisplay_table_sqlite_name() );
    if(!info_Maintable.exists())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Loading Wrong Result or Result is Corrupted !!!");
        message_dialog_object->show();

        return;
    }

    saved_execute_query_maindb_path = path + "/" + main_datadisplay_table_sqlite_name();
    if(!saved_execute_query_maindb_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
        saved_execute_query_maindb_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

    //-before load sending the the sqlite name kept comman in main file.
    load_saved_state_class_obj->main_datadisplay_table_sqlite_name_pub(main_datadisplay_table_sqlite_name());
    load_saved_state_class_obj->filter_view_table_sqlite_name_pub(filter_view_table_sqlite_name());
    load_saved_state_class_obj->saved_state_dir_name_pub(saved_state_dir_name());




    load_saved_state_class_obj->pub_set_display_loading_bar(display_loading_progress_bar_obj);
    list_struct_tablewidget_data_sqlite_viewer = load_saved_state_class_obj->load_saved_state(path);
    set_widgets_disable(false);

    for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
    {
        ui->comboBox_dbtables_selection->addItem(list_struct_tablewidget_data_sqlite_viewer.at(i).table_name);
    }





    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}
