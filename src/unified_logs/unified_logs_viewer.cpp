#include "unified_logs_viewer.h"
#include "ui_unified_logs_viewer.h"

unified_logs_viewer::unified_logs_viewer(QWidget *parent) : ui(new Ui::unified_logs_viewer)
{
    ui->setupUi(this);

    unified_logs_model_obj = new unified_logs_model;
    unified_logs_proxy_model_obj = new unified_logs_proxy_model;
    m_tableview_obj = new m_tableview;
    recon_helper_standard_obj = new recon_helper_standard(this);

    unified_logs_model_obj->pub_set_table_view_obj(m_tableview_obj);

    ui->verticalLayout->addWidget(m_tableview_obj);
    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect, false);
    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    add_tags_module_obj = new add_tags;
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    ui->tableView->hide();
    bool_search_triggered = false;
}

unified_logs_viewer::~unified_logs_viewer()
{
    delete unified_logs_proxy_model_obj;
    delete unified_logs_model_obj;
    delete ui;

}

void unified_logs_viewer::contextMenuEvent(QContextMenuEvent *evt)
{
    if(evt == NULL)
        return;

    QModelIndexList selection_model_list = m_tableview_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return;

    QList <QAction *> act_list = mainMenu->actions();

    if(act_list.isEmpty())
        return;

    create_main_menu();

    mainMenu->exec(QCursor::pos());

}

void unified_logs_viewer::set_essentials_for_unified_logs()
{
    //Data display
    QCoreApplication::processEvents();
    unified_logs_model_obj->pub_get_log_data_list_from_unified_logs_db();
    unified_logs_proxy_model_obj->setSourceModel(unified_logs_model_obj);
    QCoreApplication::processEvents();

    connect(m_tableview_obj,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_tableview_single_clicked(QModelIndex)));
    connect(m_tableview_obj,SIGNAL(signal_arrow_key_pressed()),this,SLOT(slot_arrow_key_pressed_on_tableview()));

    create_main_menu();

    m_tableview_obj->setModel(unified_logs_proxy_model_obj);
    m_tableview_obj->setShowGrid(false);
    QCoreApplication::processEvents();

    m_tableview_obj->hideColumn(enum_unified_logs_seq_no);
}

void unified_logs_viewer::on_pushButton_search_clicked()
{
    if(ui->lineEdit_search->text().trimmed().isEmpty())
        return;

    bool_search_triggered = true;

    unified_logs_proxy_model_obj->search_keyword = ui->lineEdit_search->text();
    unified_logs_proxy_model_obj->invalidate();
}

void unified_logs_viewer::fetch_metadata_for_unified_logs_viewer(int row)
{
    recon_static_functions::debug_intensive(" Starts " , Q_FUNC_INFO);

    QString metadata_str;
    QString record_no = m_tableview_obj->model()->index(row,0).data().toString();

    QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    QString unified_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";
    QString command = QString("SELECT db_name from tbl_index WHERE source_count_name = ?");

    QStringList arg_list;
    arg_list << source_count_name;

    QString db_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, arg_list, 0, unified_logs_db_path, Q_FUNC_INFO);

    QString timestamp_str, process_uuid, process_id, process_str,
            process_path, category, event_type, event_message, message_type,
            library,library_path, thread_id,activity_id,trace_id, subsystem, boot_uuid;

    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);


    QString unified_dest_path = unified_logs_dir_path + db_name;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase unified_logs_dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    unified_logs_dest_db.setDatabaseName(unified_dest_path);

    if(!unified_logs_dest_db.open())
    {
        recon_static_functions::app_debug("Database open ---FAILED--- " + unified_logs_dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(unified_logs_dest_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    unified_logs_dest_db.transaction();

    QSqlQuery query_select(unified_logs_dest_db);

    command.clear();
    command = QString(" SELECT INT, category, timestamp_readable, process_id, process, event_type, event_message,"
                      " process_path, process_image_uuid, message_type, library, library_path, thread_id, activity_id, trace_id,"
                      " subsystem, boot_uuid, source_file, recon_tag_value from log_data WHERE INT = ?");

    query_select.prepare(command);
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug("select query ---FAILED--- " + unified_logs_dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error : " + query_select.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query : " + query_select.executedQuery(),Q_FUNC_INFO);
        unified_logs_dest_db.close();
        return;
    }

    while(query_select.next())
    {
        QString tag_text = "";
        QStringList list_tags = query_select.value("recon_tag_value").toString().split(",", Qt::SkipEmptyParts);

        timestamp_str = query_select.value("timestamp_readable").toString();

        process_uuid = query_select.value("process_image_uuid").toString();
        process_id = query_select.value("process_id").toString();
        process_str = query_select.value("process").toString();
        process_path = query_select.value("process_path").toString();

        category = query_select.value("category").toString();
        event_type = query_select.value("event_type").toString();
        event_message = query_select.value("event_message").toString();
        message_type = query_select.value("message_type").toString();

        library = query_select.value("library").toString();
        library_path = query_select.value("library_path").toString();

        thread_id = query_select.value("thread_id").toString();
        activity_id = query_select.value("activity_id").toString();
        trace_id = query_select.value("trace_id").toString();
        subsystem = query_select.value("subsystem").toString();
        boot_uuid = query_select.value("boot_uuid").toString();

        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        metadata_str += "<br>";
        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" + QString("\t") + struct_source_info.complete_source_name +"<br><br>" ;
        metadata_str += "<b>" + QObject::tr("Viewer Display Name") + ":</b>" + QString("\t") + unified_logs_display_name +"<br><br>";

        if(!timestamp_str.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Timestamp") + ":</b>" + QString("\t") + timestamp_str +"<br><br>";
        }

        if(!process_uuid.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Process UUID") + ":</b>" + QString("\t") + process_uuid + "<br>";
        }

        if(!process_id.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Process ID") + ":</b>" + QString("\t") + process_id + "<br>";
        }

        if(!process_str.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Process") + ":</b>" + QString("\t") + process_str + "<br>";
        }

        if(!process_path.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Process Path") + ":</b>" + QString("\t") + process_path +"<br><br>";
        }
        if(!category.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Category") + ":</b>" + QString("\t") + category + "<br>";
        }

        if(!event_type.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Event Type") + ":</b>" + QString("\t") + event_type + "<br>";
        }

        if(!event_message.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Event Message") + ":</b>" + QString("\t") + event_message + "<br>";
        }

        if(!message_type.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Message Type") + ":</b>" + QString("\t") + message_type + "<br><br>";
        }

        if(!library.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Library") + ":</b>" + QString("\t") + library + "<br>";
        }

        if(!library_path.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Library Path") + ":</b>" + QString("\t") + library_path + "<br><br>";
        }

        if(!thread_id.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Thread ID") + ":</b>" + QString("\t") + thread_id + "<br>";
        }
        //            if(!activity_id.isEmpty())
        //            {
        //                metadata_str += "<b>" + QObject::tr("Activity ID") + ":</b>" + QString("\t") + activity_id + "<br>";
        //            }
        //            if(!trace_id.isEmpty())
        //            {
        //                metadata_str += "<b>" + QObject::tr("Trace ID") + ":</b>" + QString("\t") + trace_id + "<br>";
        //            }
        if(!subsystem.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Subsystem") + ":</b>" + QString("\t") + subsystem + "<br>";
        }

        if(!boot_uuid.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("Boot UUID") + ":</b>" + QString("\t") + boot_uuid + "<br><br>";
        }

        metadata_str += "<b>" + QObject::tr("Source File") + ":</b> <br>" + QString("\t") + QString("/private/var/db/uuidtext") + "<br>" + QString("/private/var/db/diagnostics") +"<br><br>";

        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            QString colour =  recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";

    }

    unified_logs_dest_db.commit();
    unified_logs_dest_db.close();

    st_set_meta_obj.detailed_info = metadata_str;
    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::debug_intensive(" Ends " , Q_FUNC_INFO);
}

void unified_logs_viewer::slot_tableview_single_clicked(QModelIndex model_index)
{
    int row = model_index.row();
    fetch_metadata_for_unified_logs_viewer(row);
}

void unified_logs_viewer::slot_arrow_key_pressed_on_tableview()
{
    QModelIndex curr_index = m_tableview_obj->currentIndex();
    slot_tableview_single_clicked(curr_index);
}

void unified_logs_viewer::set_viewer_display_name(QString display_nm)
{
    unified_logs_display_name = display_nm;

    QStringList tmp_list;
    tmp_list = display_nm.split("[");

    if(tmp_list.size() > 1)
    {
        source_count_name = tmp_list.at(1);
        source_count_name.remove("]");
    }
    unified_logs_model_obj->pub_set_source_count_name(source_count_name);

}

void unified_logs_viewer::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked();
}

void unified_logs_viewer::on_lineEdit_search_textChanged(const QString &arg1)
{
    if(arg1.size() <= 0)
    {
        if(bool_search_triggered)
        {
            unified_logs_proxy_model_obj->search_keyword = ui->lineEdit_search->text();
            unified_logs_proxy_model_obj->invalidate();
            bool_search_triggered = false;
        }
    }

}

void unified_logs_viewer::on_pushButton_refresh_clicked()
{
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("It may take several minutes to reload this result. Please wait...");
    display_loading_progress_bar_non_cancelable_obj->show();

    bool_search_triggered = false;

    unified_logs_model_obj->pub_get_log_data_list_from_unified_logs_db();
    ui->lineEdit_search->clear();
    unified_logs_proxy_model_obj->search_keyword = ui->lineEdit_search->text();
    unified_logs_proxy_model_obj->invalidate();

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void unified_logs_viewer::create_main_menu()
{
    mainMenu = new QMenu;

    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
    create_tags_submenu();

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked(QAction*)));

}

void unified_logs_viewer::slot_sub_menu_tags_clicked(QAction* current_clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->show();

    action_submenu_tagcolor_triggered(current_clicked_action->text());

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void unified_logs_viewer::action_submenu_tagcolor_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;
    QString tags_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,tags_db_path,Q_FUNC_INFO);

    update_tags_value(tag_name,colour);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void unified_logs_viewer::action_create_tags()
{
    add_tags_module_obj->show();
}

void unified_logs_viewer::slot_add_tags(QString tag_data , QString colour)
{
    update_tags_value(tag_data,colour);

}

void unified_logs_viewer::update_tags_value(QString tag_data,QString colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QModelIndexList selection_model_list = m_tableview_obj->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tableview_obj->model()->rowCount())
            continue;

        if(m_tableview_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_seq_no).data().toString() == "0")
            continue;

        QString record_no = m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_seq_no).data().toString();

        QList<struct_global_tagging_unified_logs_record> unified_logs_record_qlist;
        struct_global_tagging_unified_logs_record struct_global_unified_logs_rec_obj;

        struct_global_unified_logs_rec_obj.INT_record = record_no;
        struct_global_unified_logs_rec_obj.plugin_name = QString(MACRO_Plugin_Name_Unified_Logs);
        struct_global_unified_logs_rec_obj.tab_name = QString(MACRO_Plugin_Name_Unified_Logs);

        struct_global_unified_logs_rec_obj.category = m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_category).data().toString();
        struct_global_unified_logs_rec_obj.timestamp = m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_timestamp).data().toString();
        struct_global_unified_logs_rec_obj.process = m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_process).data().toString();
        struct_global_unified_logs_rec_obj.event_type = m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_event_type).data().toString();
        struct_global_unified_logs_rec_obj.event_message = m_tableview_obj->model()->index(model_index.row(),enum_unified_logs_event_message).data().toString();

        struct_global_unified_logs_rec_obj.source_count_name = source_count_name;
        struct_global_unified_logs_rec_obj.log_table_name = "log_data";
        struct_global_unified_logs_rec_obj.colour_value = colour;

        unified_logs_record_qlist << struct_global_unified_logs_rec_obj;

        QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
        QString unified_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";
        QString destination_db_path;
        QString command = QString("SELECT db_name from tbl_index where source_count_name = ?");
        QStringList values;
        values << source_count_name;
        QString unified_logs_db = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, values,0, unified_logs_db_path, Q_FUNC_INFO);

        destination_db_path = unified_logs_dir_path + unified_logs_db;

        INT_list << record_no;

        global_connection_manager_obj->pub_set_tag_in_unified_logs_db(INT_list, struct_global_unified_logs_rec_obj.colour_value,destination_db_path);
        global_connection_manager_obj->pub_add_data_in_tags_db_for_unified_logs(unified_logs_record_qlist, tag_data);
        unified_logs_model_obj->pub_update_tag_record(record_no.toInt() - 1, tag_data);

    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void unified_logs_viewer::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void unified_logs_viewer::create_tags_submenu()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_tags == NULL)
        return;

    sub_menu_tags->clear();

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QString tags_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str, tags_db_path, Q_FUNC_INFO);

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

QMenu* unified_logs_viewer::create_right_click_submenu(QString submenuStr ,QMenu *mainMenu , QMenu *subMenu)
{
    subMenu = mainMenu->addMenu(tr(submenuStr.toLocal8Bit().data()));
    return subMenu;
}
