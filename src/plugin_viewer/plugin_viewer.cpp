#include "plugin_viewer.h"
#include "ui_plugin_viewer.h"

plugin_viewer::plugin_viewer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::plugin_viewer)
{
    ui->setupUi(this);
    
    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_search->hide();
    ui->pushButton_search->hide();
    ui->pushButton_showall->hide();

    ui->checkBox_export->setIcon(QIcon("../icons/signs/export.png"));
    
    check_bookmark_status = false;
    
    timeline_start_time = 946665001;
    timeline_end_time = 2524608000;
    
    search_files_with_hash_obj = new search_files_with_hash();

    QDateTime dt11 = QDateTime::currentDateTime();
    QDateTime dt22 = dt11.toUTC();
    dt11.setTimeSpec(Qt::UTC);
    
    thread_report_builder_obj = new thread_report_builder(this);
    connect(thread_report_builder_obj,SIGNAL(finished()),this,SLOT(slot_thread_report_finished_or_cancelled()),Qt::QueuedConnection);

    act_seperator_bookmark = new QAction(this);
    act_seperator_bookmark->setSeparator(true);

    menu = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    menu->setFont(m_font);

    submenu_bookmark = menu->addMenu(tr(MACRO_Generic_Right_Click_Bookmark));
    submenu_bookmark->setFont(m_font);

    act_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Bookmark),this->submenu_bookmark);
    act_remove_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Bookmarks), this->submenu_bookmark);
    submenu_bookmark->addAction(act_bookmark);
    submenu_bookmark->addAction(act_remove_bookmark);

    tag_sub_menu = menu->addMenu(MACRO_Generic_Right_Click_Tags);
    tag_sub_menu->setFont(m_font);

    submenu_notes = menu->addMenu(tr(MACRO_Generic_Right_Click_Notes));
    submenu_notes->setFont(m_font);

    act_add_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Note), this->submenu_notes);
    act_remove_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Note), this->submenu_notes);
    submenu_notes->addAction(act_add_notes);
    submenu_notes->addAction(act_remove_notes);

    menu->addAction(act_seperator_bookmark);

    act_add_notes_to_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Notes_to_Bookmarks), this);
    act_remove_notes_from_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks), this);
    act_export = new QAction(QObject::tr(MACRO_Generic_Right_Click_Export), this);
    act_copy_to_clipboard = new QAction(QObject::tr(MACRO_Generic_Right_Click_Copy_To_Clipboard), this);
    act_quick_look = new QAction(QObject::tr(MACRO_Generic_Right_Click_Quick_look), this) ;

    connect(act_bookmark,SIGNAL(triggered()),this,SLOT(slot_act_bookmark_triggered()));
    connect(act_remove_bookmark, SIGNAL(triggered()), this, SLOT(slot_act_remove_bookmark()));
    // connect(act_remove_all_bookmark, SIGNAL(triggered()), this, SLOT(slot_act_remove_all_bookmark()));
    connect(act_add_notes, SIGNAL(triggered()), this, SLOT(slot_act_add_notes()));
    connect(act_add_notes_to_bookmark, SIGNAL(triggered()), this, SLOT(slot_act_add_notes_to_bookmarks()));
    connect(act_export, SIGNAL(triggered()), this, SLOT(slot_act_export()));
    connect(act_remove_notes, SIGNAL(triggered()), this, SLOT(slot_act_remove_notes()));
    connect(act_remove_notes_from_bookmark, SIGNAL(triggered()), this, SLOT(slot_act_remove_notes_from_bookmarks()));
    //  connect(act_remove_all_notes, SIGNAL(triggered()), this, SLOT(slot_act_remove_all_notes()));
    connect(act_copy_to_clipboard, SIGNAL(triggered()), this, SLOT(slot_act_copy_to_clipboard()));
    connect(act_quick_look, SIGNAL(triggered()), this, SLOT(slot_act_quick_look_triggered()));
    
    manage_plugin_columns_obj = new manage_plugin_columns(this);
    connect(manage_plugin_columns_obj,SIGNAL(signal_manage_column_clicked(int,int )),this,SLOT(slot_manage_column_clicked(int , int)));
    connect(this,SIGNAL(signal_manage_columns_checkstate()),manage_plugin_columns_obj,SLOT(slot_manage_columns_checkstate()));
    
    act_manage_columns = new QAction(QObject::tr(MACRO_Generic_Right_Click_Manage_Columns), this);  //----dinu nov 5
    connect(act_manage_columns, SIGNAL(triggered()), this, SLOT(slot_act_manage_columns()));
    
    //    progress_window_module_popobj = new progress_window_module(mas_get_module_name(),this);
    //    connect(progress_window_module_popobj, SIGNAL(signal_progress_window_module_closed()), this, SLOT(slot_progress_window_module_closed()));
    
    timeline_selection_window_obj = new timeline_selection_window(this);
    connect(timeline_selection_window_obj, SIGNAL(signal_time_line(qint64,qint64)), this, SLOT(slot_time_line(qint64,qint64)));
    
    add_notes_dialog_module_popobj2 = new add_notes(this);
    connect(add_notes_dialog_module_popobj2, SIGNAL(signal_notes(QString)), this, SLOT(slot_add_notes_to_bookmarks(QString)));
    
    add_notes_dialog_module_popobj1 = new add_notes(this);
    connect(add_notes_dialog_module_popobj1, SIGNAL(signal_notes(QString)), this, SLOT(slot_add_notes(QString)));
    
    detached_file_previewer_module_popobj = new detached_file_previewer_module(get_plugin_name(),this);
    detached_metadata_viewer_module_popobj = new detached_metadata_viewer_module(get_plugin_name(),this);
    
    menu_manage_column = new QMenu(this);
    QFont m_font1;
    m_font1.setPointSize(MACRO_QMenu_Font_Size_int);
    menu_manage_column->setFont(m_font1);
    
    connect(menu_manage_column, SIGNAL(triggered(QAction*)), this, SLOT(slot_action_manage_column_clicked(QAction*)));
    
    menu->addAction(act_quick_look);
    
    submenu_open_with        =   menu->addMenu(tr(MACRO_Generic_Right_Click_Open_With));
    submenu_open_with->setFont(m_font);
    
    act_plist_viewer_openwith = new QAction(QObject::tr(MACRO_Plugin_Name_Plist_Viewer), this->submenu_open_with);
    connect(act_plist_viewer_openwith, SIGNAL(triggered()), this, SLOT(slot_act_extract_plist_viewer_openwith()));
    act_hex_viewer_openwith   = new QAction(QObject::tr(MACRO_CASE_TREE_Hex_Viewer), this->submenu_open_with);
    connect(act_hex_viewer_openwith, SIGNAL(triggered()), this, SLOT(slot_act_hex_viewer_openwith()));
    act_sqlite_viewer_openwith = new QAction(QObject::tr(MACRO_Plugin_Name_Sqlite_Viewer), this->submenu_open_with);
    connect(act_sqlite_viewer_openwith, SIGNAL(triggered()), this, SLOT(slot_act_extract_sqlite_viewer_openwith()));
    act_log_viewer_openwith = new QAction(QObject::tr(MACRO_Plugin_Name_Log_Viewer), this->submenu_open_with);
    connect(act_log_viewer_openwith, SIGNAL(triggered()), this, SLOT(slot_act_extract_log_viewer_openwith()));

    submenu_open_with->addAction(act_plist_viewer_openwith);
    submenu_open_with->addAction(act_hex_viewer_openwith);
    submenu_open_with->addAction(act_sqlite_viewer_openwith);
    submenu_open_with->addAction(act_log_viewer_openwith);

    act_full_detail_info = new QAction(QObject::tr(MACRO_Generic_Right_Click_Open_In_Window), this);
    connect(act_full_detail_info, SIGNAL(triggered()), this, SLOT(slot_full_detail_info_triggered()));

    menu->addAction(act_full_detail_info);
    menu->addAction(act_copy_to_clipboard);

    act_seperator_notes = new QAction(this);
    act_seperator_notes->setSeparator(true);

    menu->addAction(act_seperator_notes);

    submenu_send_to_bucket        =   menu->addMenu(tr(MACRO_Generic_Right_Click_Send_To_Bucket));
    submenu_send_to_bucket->setFont(m_font);
    
    act_plist_viewer_sendtobucket = new QAction(QObject::tr(MACRO_Plugin_Name_Plist_Viewer), this->submenu_send_to_bucket);
    connect(act_plist_viewer_sendtobucket, SIGNAL(triggered()), this, SLOT(slot_act_plist_viewer_sendtobucket()));
    act_hex_viewer_sendtobucket   = new QAction(QObject::tr(MACRO_CASE_TREE_Hex_Viewer), this->submenu_send_to_bucket);
    connect(act_hex_viewer_sendtobucket, SIGNAL(triggered()), this, SLOT(slot_act_hex_viewer_sendtobucket()));
    act_sqlite_viewer_sendtobucket = new QAction(QObject::tr(MACRO_Plugin_Name_Sqlite_Viewer), this->submenu_send_to_bucket);
    connect(act_sqlite_viewer_sendtobucket, SIGNAL(triggered()), this, SLOT(slot_act_sqlite_viewer_sendtobucket()));
    act_log_viewer_sendtobucket = new QAction(QObject::tr(MACRO_Plugin_Name_Log_Viewer), this->submenu_send_to_bucket);
    connect(act_log_viewer_sendtobucket, SIGNAL(triggered()), this, SLOT(slot_act_log_viewer_sendtobucket()));

    submenu_send_to_bucket->addAction(act_plist_viewer_sendtobucket);
    submenu_send_to_bucket->addAction(act_hex_viewer_sendtobucket);
    submenu_send_to_bucket->addAction(act_sqlite_viewer_sendtobucket);
    // submenu_send_to_bucket->addAction(act_text_viewer_sendtobucket);
    submenu_send_to_bucket->addAction(act_log_viewer_sendtobucket);

    QAction *act_seperator_send = new QAction(this);
    act_seperator_send->setSeparator(true);

    menu->addAction(act_seperator_send);
    
    
    menu_hashset_add_files = menu->addMenu(tr(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database));
    menu_hashset_add_files->setFont(m_font);
    
    QList <QMenu*> hahses_menu_list;
    hahses_menu_list.append(menu_hashset_add_files);
    action_search_file_with_same_md5_hash = new QAction(QObject::tr(MACRO_Generic_Right_Click_Search_File_With_Same_Hash), this);
    connect(action_search_file_with_same_md5_hash, SIGNAL(triggered()), this, SLOT(slot_action_search_file_with_same_hash_clicked()));
    menu->addAction(action_search_file_with_same_md5_hash);
    recon_helper_standard_obj->pub_create_disabled_empty_action_in_menu(hahses_menu_list, Q_FUNC_INFO);

    hashset_display_obj = new hashset_display(this);
    hashset_storage_obj = new hashset_storage(this);
    connect(hashset_storage_obj, SIGNAL(signal_hashset_storage_workdone_clicked(QStringList, QString)), this, SLOT(slot_hashset_storage_workdone_clicked(QStringList, QString)));
    
    connect(menu_hashset_add_files, SIGNAL(triggered(QAction*)), this, SLOT(slot_hashset_menu_add_file_clicked(QAction*)));
    
    act_seperator_viewers = new QAction(this);
    act_seperator_viewers->setSeparator(true);
    menu->addAction(act_seperator_viewers);
    menu->addAction(act_export);
    
    installEventFilter(this);
    
    bool_detach_mdls_set = false;
    bool_full_mdls_set = false;
    detach_mdls_dialog_module_popobj = new detach_mdls_dialog_module(get_plugin_name(),this);
    connect(detach_mdls_dialog_module_popobj, SIGNAL(signal_detach_mdls_dialog_window_closed()), this, SLOT(slot_detach_mdls_dialog_window_closed()));
    
    ui->pushButton_timeline->setEnabled(false);
    
    bool_cancel_extraction = false;
    bool_export_available = false;
    bool_cancel_global_search = false;
    
    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect, 0);
    
    remove_multiple_tag_selecotor_obj = new remove_multiple_tag_selecotor(this);
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_remaining_tags_list(QStringList, QString , QString )),this,SLOT(slot_tag_list_after_remove_multiple_tag(QStringList, QString , QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_tag_list_to_be_removed(QStringList, QString , QString)),this,SLOT(slot_removed_tags_list(QStringList, QString , QString)));
    
    //    tag_sub_menu = menu->addMenu(MACRO_Generic_Right_Click_Tags);
    //    tag_sub_menu->setFont(m_font);
    
    
    
    act_open_in_detach  =   new QAction("", this);
    
    QAction *act_seperator_export = new QAction(this);
    act_seperator_export->setSeparator(true);
    menu->addAction(act_seperator_export);
    
    //  connect(act_full_detail_info, SIGNAL(triggered()), this, SLOT(slot_full_detail_info_triggered()));
    connect(act_open_in_detach, SIGNAL(triggered()), this, SLOT(slot_open_in_detach_triggered()));
    
    submenu_go_to_artifact = menu->addMenu(tr(MACRO_Generic_Right_Click_Go_To_Artifacts_Source));
    submenu_go_to_artifact->setFont(m_font);
    
    connect(submenu_go_to_artifact,SIGNAL(triggered(QAction*)),this,SLOT(slot_submenu_go_to_artifact_triggered(QAction*)));
    
    
    add_tags_obj = new add_tags(this);
    connect(add_tags_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_in_actions_menu(bool,QString,QString)));
    connect(add_tags_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags_to_plugin(QString,QString)));
    
    
    ///selecting which tag was clicked
    connect(tag_sub_menu, SIGNAL(triggered(QAction*)), this, SLOT(slot_selected_tag(QAction*)));
    connect(tag_sub_menu,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    connect(this, SIGNAL(signal_add_tags(QString,QString)), this, SLOT(slot_add_tags_to_plugin(QString,QString)));
    
    menu->addAction(act_manage_columns);
    
    ///--------------------------------
    bool_show_all_clicked = false;
    message_dialog_object = new message_dialog(this);
    bool_message_box_yes_or_no_receiver = false;
    
    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)) , this , SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;
    
    display_loading_progress_bar_for_reports_from_plugin_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_for_reports_from_plugin_obj,SIGNAL(signal_close_loading_display_dialog(bool)) , this , SLOT(slot_hide_progressing_bar_on_cancel_loading_report(bool)));
    bool_cancel_loading_reports = false;
    
    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    

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

    export_rsync_obj = new export_rsync(this);

    map_bookmarked_records_from_table_display.clear();

}

plugin_viewer::~plugin_viewer()
{
    if(search_files_with_hash_obj != nullptr)
    {
        delete search_files_with_hash_obj;
    }

    delete ui;
}


void plugin_viewer::remove_extra_widgets()
{
    ui->tabWidget->removeTab(0);
}


bool plugin_viewer::eventFilter(QObject *obj, QEvent *event)
{
    if(event == NULL || event == nullptr)
        return false;

    if(event->type() == QEvent::ContextMenu)
    {

        if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
            return false;

        if(list_tablewidgets.size() <= current_tab_index_only_for_tableview)
            return false;

        m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index_only_for_tableview);

        if(my_tablewidget == NULL)
            return false;


        QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();
        if(selection_model_list.size() < 1)
            return false;

        int current_column_int = my_tablewidget->currentColumn();
        int current_row_int = my_tablewidget->currentRow();
        if(current_column_int < 0 || current_row_int < 0)
            return false;


        if(!my_tablewidget->selectionModel()->selectedRows(current_column_int).isEmpty())
        {

            QList <QAction *> act_list = menu->actions();
            for(int i = 0 ; i < act_list.size() ; i++)
            {
                act_list.at(i)->setEnabled(true);
            }

        }
        else
        {
            return false;
        }

        tablewidget_general_clicked(current_tab_index_only_for_tableview,my_tablewidget->currentRow(),my_tablewidget->currentColumn());

        QString plugin_name_str = get_plugin_name();
        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str, current_tab_text);
        QString tab_name_str =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(plugin_name_str, table_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);

        display_hashset_add_files_submenu();


        if(selection_model_list.size() == 1)
            submenu_go_to_artifact->setEnabled(true);
        else
            submenu_go_to_artifact->setEnabled(false);


        QString record_no_str =  my_tablewidget->item(current_row_int,2)->text();
        struct_global_recon_file_info st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_file_exist_any(get_plugin_name(),tab_name_str ,record_no_str);
        QString complete_file_path = st_globl_recon_file_info_obj.complete_file_path;
        bool bool_virtual_export = st_globl_recon_file_info_obj.bool_file_from_result;

        if(QFileInfo(complete_file_path).exists() && !bool_virtual_export)
        {
            if(global_csv_reader_class_selected_obj->is_export_available_in_tab(tab_name_str,plugin_name_str))
            {
                submenu_open_with->setEnabled(true);

                submenu_send_to_bucket->setEnabled(true);
                act_export->setEnabled(true);
                menu_hashset_add_files->setEnabled(true);
                if(selection_model_list.size() == 1)
                    action_search_file_with_same_md5_hash->setEnabled(true);
                else
                    action_search_file_with_same_md5_hash->setEnabled(false);

                act_quick_look->setEnabled(true);
            }
            else if(bool_virtual_export)
            {
                submenu_open_with->setEnabled(false);
                submenu_send_to_bucket->setEnabled(false);
                act_export->setEnabled(false);
                menu_hashset_add_files->setEnabled(false);
                if(selection_model_list.size() == 1)
                    action_search_file_with_same_md5_hash->setEnabled(true);
                else
                    action_search_file_with_same_md5_hash->setEnabled(false);

                act_quick_look->setEnabled(true);
            }

            if(selection_model_list.size() > 1)
            {
                submenu_open_with->setEnabled(false);
                act_full_detail_info->setEnabled(false);
                submenu_go_to_artifact->setEnabled(false);
                act_quick_look->setEnabled(false);
                //ktktkt
//                act_go_to_mail->setEnabled(false);
                //ktktkt
            }

        }
        else
        {
            submenu_open_with->setEnabled(false);
            submenu_send_to_bucket->setEnabled(false);
            act_export->setEnabled(false);
            menu_hashset_add_files->setEnabled(false);
            if(selection_model_list.size() == 1)
                action_search_file_with_same_md5_hash->setEnabled(true);
            else
                action_search_file_with_same_md5_hash->setEnabled(false);
            act_quick_look->setEnabled(false);
        }

        if(global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(plugin_name_str,current_tab_text))
        {
            act_manage_columns->setDisabled(true);
            act_copy_to_clipboard->setDisabled(true);
        }
        else
        {
            act_copy_to_clipboard->setDisabled(false);
            act_manage_columns->setDisabled(false);
        }

        QStringList tw_col_type_list = tw_data_type_list_tablewidget.at(current_tab_index_only_for_tableview);
        int INT_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
        QString INT_str = my_tablewidget->item(my_tablewidget->currentRow(), INT_index)->text();
        QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(INT_str, table_name, destination_db_file, Q_FUNC_INFO);
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        if(struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
                || struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
                || struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal))
        {
            //ktktkt
//            act_go_to_mail->setDisabled(true);
            //ktktkt
        }


        add_action_to_artifact_source_submenu(INT_str , table_name);

        menu->exec(QCursor::pos());

        return true;
    }

    return false;
}

void plugin_viewer::copy_resource_directory(QString source, QString destination)
{
    QDirIterator iter(source, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    while(iter.hasNext())
    {
        QFileInfo info(iter.next());
        QFile resource_item_file(info.absolutePath() + "/" + info.fileName());
        QString resource_dest_item = destination + "/" + info.fileName();
        resource_item_file.copy(resource_dest_item);
    }
}

void plugin_viewer::on_lineEdit_search_textChanged(const QString &arg1)
{
    if(!bool_show_all_clicked)
    {
        if(arg1.trimmed() == QString(""))
            on_pushButton_showall_clicked();
    }
}

QString plugin_viewer::mining_column_name_for_datatype(QStringList col_name_list,QStringList data_type_list,QString looking_for_data_type)
{
    for(int i = 0; i < data_type_list.size(); i++)
    {
        if(data_type_list.at(i).trimmed() == looking_for_data_type.trimmed())
            return col_name_list.at(i).trimmed();
    }
    
    return QString("");
}

int plugin_viewer::mining_position_for_datatype(QStringList data_type_list,QString looking_for_data_type)
{
    for(int i = 0; i < data_type_list.size(); i++)
    {
        if(data_type_list.at(i).trimmed() == looking_for_data_type.trimmed())
            return i;
    }
    
    return -1;
}

QString plugin_viewer::get_icon_path_tags(QString colour,QString data)
{
    QString icon_path;
    
    if(data.contains(","))
        icon_path = "../icons/tag_icons/multiple.png";
    else
        icon_path = "../icons/tag_icons/"+ colour.replace(" ","_").toLower() + ".png";
    
    return icon_path;
}

QString plugin_viewer::get_tag_from_record_number(QString table_name,QString record_str)
{
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Failed to open DB-------------------------FAILED-----------------------" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return QString("");
    }
    
    QSqlQuery query(destination_db);
    QString command;
    
    command = QString("select recon_tag_value from " + table_name + " where INT=" + "'" + record_str + "'");
    
    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" Failed to execute query-------------------------FAILED-----------------------",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Command" + command,Q_FUNC_INFO);
        destination_db.close();
        return QString("");
    }
    
    QString tag_str;
    if(query.next())
    {
        tag_str = query.value(0).toString();
    }
    
    destination_db.close();
    return tag_str;
}


void plugin_viewer::display_default_tags()
{
    recon_static_functions::app_debug(" start", Q_FUNC_INFO);
    
    tag_sub_menu->clear();
    
    ///add new tag
    act_create_tag = new QAction(MACRO_Generic_Right_Click_Create_Tag, this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    tag_sub_menu->addAction(act_create_tag);
    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_show_tag()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_show_tag()));
    
    ///remove tag
    act_remove_tag = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    tag_sub_menu->addAction(act_remove_tag);
    
    
    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_remove_tags_from_plugin()));
    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_remove_tags_from_plugin()));
    
    
    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    tag_sub_menu->addAction(separator_main);
    
    
    QString command_str = "SELECT tag_name,selected_colour FROM tags";
    
    QStringList tags_list = add_tags_obj->get_tags_and_colours_list_from_db(command_str , add_tags_obj->tags_db_path);
    
    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);
        
        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                tag_sub_menu->addAction(separator_default_tags);
            }
            
            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);
            
            QString icon_path = "";
            
            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);
            
            icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
            
            QIcon icon(icon_path);
            
            act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            tag_sub_menu->addAction(act_already_saved_tags);
            
        }
    }
    
    recon_static_functions::app_debug(" end", Q_FUNC_INFO);
}


void plugin_viewer::common_add_tags_to_plugin(QString tag_name, QString tag_colour)
{
    recon_static_functions::app_debug(" Starts",Q_FUNC_INFO);
    
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    int tab_index = current_tab_index_only_for_tableview;
    if(list_tablewidgets.size() <= tab_index)
        return;
    
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();
    
    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);
    
    QStringList cols_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    
    QString tab_name = tab_name_list_tablewidget.at(tab_index);
    
    
    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedRows();
    
    QStringList INT_list;
    
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();
        
        QModelIndex model_index = selection_model_list.at(i);
        
        int selected_row_no = model_index.row();
        
        if(selected_row_no < 0 && selected_row_no >= my_tablewidget->rowCount())
            continue;
        
        
        QString icon_path =  get_icon_path_tags(tag_colour,tag_name);
        QIcon icon(icon_path);
        my_tablewidget->item(selected_row_no,1)->setIcon(icon);
        
        
        INT_list << my_tablewidget->item(selected_row_no, record_index)->text();
    }
    
    global_connection_manager_obj->pub_set_tag_in_plugin_viewer_db(INT_list, get_plugin_name(), tab_name, tag_name );
    global_connection_manager_obj->pub_add_tag_for_plugin_viewer(INT_list, get_plugin_name(), tab_name, tag_name);
    //    global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(INT_list, get_plugin_name(), tab_name);
    global_connection_manager_obj->pub_set_tags_in_artifacts_keyword_search_db(INT_list, get_plugin_name(), tab_name, tag_name);

    disable_action_widgets(false);
    
    display_loading_progress_bar_non_cancelable_obj->hide();
    
    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}


QString plugin_viewer::get_tags_from_table_record(QSqlDatabase dest_db, QString table_name, QString record)
{
    QString tags_string = "";
    QSqlQuery query_tags(dest_db);
    query_tags.prepare("select recon_tag_value from " + table_name + " where INT = ?");
    query_tags.addBindValue(record);
    if(!query_tags.exec())
    {
        recon_static_functions::app_debug(" query_tags select ----FAILED---- " + query_tags.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query_tags.lastError().text(),Q_FUNC_INFO);
        return "";
    }
    
    if(query_tags.next())
    {
        tags_string = query_tags.value(0).toString();
    }
    
    return tags_string;
    
}

QList<plugin_viewer::struct_manage_column_info> plugin_viewer::get_hidden_column_from_manage_column_db()
{
    QList<struct_manage_column_info> hidden_col_lst;
    QString manage_column_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString() + "manage_columns.sqlite";
    
    QFile file(manage_column_db_path);
    if(file.exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(manage_column_db_path);
        
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" : plugin hidden columns Db opening on load case tree------FAILED------- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
            return hidden_col_lst;
        }
        
        QSqlQuery query(destination_db);
        
        QString command;
        command = QString("select * from hidden_columns where tab_name=") + QString("'") + current_tab_text + QString("'") + QString(" AND plugin_name=") + QString("'") + get_plugin_name() + QString("'");
        query.prepare(command);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query selection of hidden cols on load case tree ---FAILED---",Q_FUNC_INFO);
            recon_static_functions::app_debug("  error " + query.lastError().text(),Q_FUNC_INFO);
        }
        
        while(query.next()){
            
            struct_manage_column_info obj;
            obj.hid_col_no = query.value(2).toInt();
            obj.hid_col_name = query.value(3).toString();
            
            hidden_col_lst << obj;
        }
        
        destination_db.close();
        
    }
    return hidden_col_lst;
}

void plugin_viewer::slot_manage_column_clicked(int col, int check_status)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);
    
    QString manage_column_db_path;
    manage_column_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString() + "manage_columns.sqlite";
    
    bool hidden_col_db_exist = false;
    
    QFile file(manage_column_db_path);
    if(file.exists())
    {
        hidden_col_db_exist = true;
    }
    
    //----------//
    
    /// Since bookmark and tags column are not showed in manage column
    /// add them to get actual column to be hidden / shown
    
    col += 3;
    
    //    bool bool_check_is_system_account_exist = false;
    //    int colum_count = 0;
    //    colum_count = m_tablewidget_obj_hide_unhide_cols->columnCount();
    
    //    for(int i=0;i<colum_count;i++){
    
    //        if((m_tablewidget_obj_hide_unhide_cols->horizontalHeaderItem(i)->text() == QString("System Account")))
    //        {
    //            bool_check_is_system_account_exist = true;
    //            break;
    //        }
    
    //    }
    
    //    if(system_account_name_list.size() <= 1)
    //    {
    //        if(!bool_check_is_system_account_exist)
    //            col = col + 3;///first three items make fixed these are not applicable to disappear on user demands
    //        else
    //            col = col + 4;///first four items make fixed these are not applicable to disappear on user demands
    //    }
    //    else
    //    {
    //        col = col + 3;///first three items make fixed these are not applicable to disappear on user demands
    //    }
    
    // emit signal_manage_columns_checkstate();
    
    int tab_index = ui->tabWidget->currentIndex();

    QList<custom_table_header_widget *> custom_plugin_header_widgets_QList;
    custom_plugin_header_widgets_QList = tab_custom_header_widgets_QMap.value(tab_index);

    if(m_tablewidget_obj_for_manage_column != NULL)
    {
        if(m_tablewidget_obj_for_manage_column->isColumnHidden(col))
        {
            clear_all_custom_header_widget_entries(custom_plugin_header_widgets_QList);

            if(check_status == Qt::Unchecked)
            {
                return;
            }
            m_tablewidget_obj_for_manage_column->showColumn(col);
            
            if(hidden_col_db_exist)
            {
                QString connection_naam = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                destination_db.setDatabaseName(manage_column_db_path);
                
                if(!destination_db.open())
                {
                    recon_static_functions::app_debug(" : plugin hidden columns Db opening ------FAILED------- ",Q_FUNC_INFO);
                    recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                    return;
                }
                
                QSqlQuery query(destination_db);
                
                //-----//
                QString command;
                command = QString("delete from hidden_columns where hidden_columns_nos=") + QString::number(col) + QString(" AND plugin_name='") + get_plugin_name() + QString("'") + QString(" AND tab_name='") + current_tab_text + QString("'");
                
                //-----------//
                
                query.prepare(command);
                //query.prepare(QString("delete from hidden_columns where hidden_columns_nos=") + QString::number(col));
                
                if(!query.exec())
                {
                    recon_static_functions::app_debug(" [slot_of_master_frame_invoked_from_manage_columns] query deletion ---FAILED---",Q_FUNC_INFO);
                    recon_static_functions::app_debug(" [slot_of_master_frame_invoked_from_manage_columns] error " + query.lastError().text(),Q_FUNC_INFO);
                }
                
                destination_db.close();
            }
        }
        else
        {
            if(check_status == Qt::Checked)
            {
                return;
            }

            clear_all_custom_header_widget_entries(custom_plugin_header_widgets_QList);
            m_tablewidget_obj_for_manage_column->hideColumn(col);
            if(hidden_col_db_exist)
            {
                
                QString connection_naam = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam + "_1");
                QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + "_1");
                destination_db.setDatabaseName(manage_column_db_path);
                
                if(!destination_db.open())
                {
                    recon_static_functions::app_debug(" : plugin hidden columns Db opening ------FAILED------- ",Q_FUNC_INFO);
                    recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                    return;
                }
                
                QSqlQuery query(destination_db);
                
                
                query.prepare("insert into hidden_columns (plugin_name, tab_name, hidden_columns_nos,hidden_column_name) values(?,?,?,?)");
                
                query.addBindValue(get_plugin_name());
                query.addBindValue(current_tab_text);
                query.addBindValue(QString::number(col));
                query.addBindValue(((custom_table_header_widget *)custom_plugin_header_widgets_QList.at(col))->pub_get_column_name());
                
                if(!query.exec())
                {
                    recon_static_functions::app_debug(" [slot_manage_column_clicked] query insertion ---FAILED---",Q_FUNC_INFO);
                    recon_static_functions::app_debug(" [slot_manage_column_clicked] error " + query.lastError().text(),Q_FUNC_INFO);
                }
                
                destination_db.close();
                
            }
            
        }
    }
    
    refresh_horizontal_header();
    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
    
}

QStringList plugin_viewer::get_applied_tags_name_on_current_record(QString record_no)
{
    QStringList tag_name_list;
    
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return tag_name_list;

    int tab_index = current_tab_index_only_for_tableview;

    if(list_tablewidgets.size() <= tab_index)
        return tag_name_list;
    
    QString table_name = table_name_list_tablewidget.at(tab_index);
    
    QString tmp_plg_name = get_plugin_name();
    tmp_plg_name.replace(" ","_");
    
    QString destination_plugin_dir_path = recon_result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;
    
    
    QString command = "SELECT recon_tag_value FROM '" + table_name + "' WHERE INT=?";
    QStringList arg_list;
    arg_list << record_no;
    QString applied_tag = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);
    
    if(!applied_tag.isEmpty())
    {
        tag_name_list << applied_tag;
    }
    
    return tag_name_list;
}

void plugin_viewer::add_action_to_artifact_source_submenu(QString INT_str , QString table_name)
{
    submenu_go_to_artifact->clear();

    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(INT_str,table_name,destination_db_file,Q_FUNC_INFO);
    struct_GLOBAL_witness_info_source mobj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    QString command = "SELECT source_file FROM " + table_name + " WHERE INT=?" + " GROUP BY source_file";

    if(mobj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
    {
        command = "SELECT artifacts_relation_source_file FROM " + table_name + " WHERE INT=?" + " GROUP BY artifacts_relation_source_file";
    }

    QStringList arg_list;
    arg_list << INT_str;
    QString artifact_source = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_file,Q_FUNC_INFO);

    if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)
            || artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
    {
        QStringList source_list;

        if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
        {
            source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
        }
        else if(artifact_source.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
        {
            source_list = artifact_source.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
        }

        for(int i = 0; i < source_list.size(); i++)
        {
            QString source_path = source_list.at(i);

            if(source_path.endsWith("/"))
                source_path.chop(1);

            QFileInfo info_file(source_path);

            QAction *act_go_to_actifact = new QAction(QObject::tr(info_file.fileName().toLocal8Bit().data()), this);
            act_go_to_actifact->setToolTip(source_path);
            submenu_go_to_artifact->addAction(act_go_to_actifact);

        }
    }
    else
    {
        if(artifact_source.endsWith("/"))
            artifact_source.chop(1);

        QFileInfo info_file(artifact_source);

        QAction *act_go_to_actifact = new QAction(QObject::tr(info_file.fileName().toLocal8Bit().data()), this);
        act_go_to_actifact->setToolTip(artifact_source);
        submenu_go_to_artifact->addAction(act_go_to_actifact);

    }
}

void plugin_viewer::comman_signal_emit_for_statusbar_progressbar(bool status,QString message)
{
    emit signal_statusbar_progress_bar_show_hide( status,  message);
}


void plugin_viewer::disable_action_widgets(bool status)
{
    if(status)
    {
        QCoreApplication::processEvents();
        
        emit signal_statusbar_progress_bar_show_hide(true, "Please Wait...");
    }
    else
        emit signal_statusbar_progress_bar_show_hide(false, "");
    
    bool spl_tab = false;
    if(global_csv_reader_class_selected_obj->is_tab_type_thumbnail_view(get_plugin_name(), current_tab_text)
            || global_csv_reader_class_selected_obj->is_tab_type_chat_view(get_plugin_name(), current_tab_text)
            || global_csv_reader_class_selected_obj->is_tab_type_chat_view(get_plugin_name(), current_tab_text))
    {
        spl_tab = true;
    }

    if(spl_tab)
    {
        ui->checkBox_timeline->setDisabled(true);
        ui->pushButton_timeline->setDisabled(true);
    }
    else
    {
        ui->checkBox_timeline->setDisabled(false);
    }


}

void plugin_viewer::loading_display_dialog_based_on_records(int total_records, int processed_count, QString title_str)
{
    
    display_loading_progress_bar_obj->pub_set_progress_bar_title(title_str);
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);
    
    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));
    
    if(processed_count >= 500)
        display_loading_progress_bar_obj->show();
    
}

void plugin_viewer::refresh_horizontal_header()
{
    m_tablewidget_obj_for_manage_column->horizontalScrollBar()->setValue(m_tablewidget_obj_for_manage_column->horizontalScrollBar()->value() + 10);
    m_tablewidget_obj_for_manage_column->horizontalScrollBar()->setValue(m_tablewidget_obj_for_manage_column->horizontalScrollBar()->value() - 10);
    m_tablewidget_obj_for_manage_column->horizontalHeader()->hide();
    m_tablewidget_obj_for_manage_column->horizontalHeader()->show();
}

void plugin_viewer::clear_all_custom_header_widget_entries(QList<custom_table_header_widget *> custom_plugin_header_widgets_QList)
{
    for(int ii = 0; ii < custom_plugin_header_widgets_QList.size(); ii++)
    {
        custom_plugin_header_widgets_QList.at(ii)->pub_clear_line_edit_search_field();
    }

    refresh_horizontal_header();
}
