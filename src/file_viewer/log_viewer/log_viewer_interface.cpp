#include "log_viewer_interface.h"
#include "ui_log_viewer_interface.h"

log_viewer_interface::log_viewer_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::log_viewer_interface)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,0);

    ui->textBrowser_log_viewer->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textBrowser_log_viewer, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenu(const QPoint&)));

    add_tags_module_obj = new add_tags(this);

    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_in_actions_menu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_update_tags_value(QString,QString)));

    mainMenu = new QMenu(this);

    QFont m_font;
    m_font.setPointSize(12);
    mainMenu->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_main_menu_triggered(QAction*)));

    act_select_all = new QAction(QString("Select All"), this);
    mainMenu->addAction(act_select_all);

    sub_menu_tags = mainMenu->addMenu(tr(QString(MACRO_Generic_Right_Click_Tags).toLocal8Bit().data()));
    sub_menu_tags->setFont(m_font);

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
}

log_viewer_interface::~log_viewer_interface()
{
    delete ui;
}

void log_viewer_interface::slot_log_viewer_db_path_changed(QString db_path)
{
    destination_db_path = db_path;
}

void log_viewer_interface::set_viewer_display_name(QString display_nm)
{
    log_viewer_display_name = display_nm;
}

void log_viewer_interface::set_db_path(QString path)
{
    destination_db_path = path;
}

void log_viewer_interface::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

QString log_viewer_interface::get_log_viewer_name()
{
    return QString(MACRO_Plugin_Name_Log_Viewer);
}

void log_viewer_interface::set_initialisation()
{
    get_info_from_log_index_table();
    display_logs_view();
}

void log_viewer_interface::display_logs_view()
{ // display log view on the tablewidget
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    //Log_Viewer_Display
    QString command = "SELECT log_file_data from 'log_viewer_index' WHERE viewer_display_name = '" + log_viewer_display_name + "'";
    QString data_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_path,Q_FUNC_INFO);

    ui->textBrowser_log_viewer->setText(data_str);

    QString case_confg_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QTextDocument *document = ui->textBrowser_log_viewer->document();

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    QString select_query_cmd = "SELECT tagged_data, cursor_start_index, cursor_end_index, colour_value from '" + log_db_table_name + "'";

    if(!query.exec(select_query_cmd))
    {
        recon_static_functions::app_debug(" Select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error " + query.lastError().text() , Q_FUNC_INFO);
        source_db.close();
        return;
    }

    while(query.next())
    {
        QString tagged_data = query.value("tagged_data").toString().trimmed();
        qint64 cursor_strt_index = query.value("cursor_start_index").toString().trimmed().toInt();
        qint64 cursor_end_index = query.value("cursor_end_index").toString().trimmed().toInt();
        QString tag_colour = query.value("colour_value").toString().trimmed();

        QString command_str = "SELECT color_hex FROM colors Where color_name = ?";

        QStringList value_list;
        value_list << tag_colour;
        QString colour_hex = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,value_list,0,case_confg_db_path,Q_FUNC_INFO);

        cursor.setPosition(cursor_strt_index);
        cursor.setPosition(cursor_end_index, QTextCursor::KeepAnchor);

        QTextCharFormat colorFormat;
        colorFormat.setForeground(QBrush(QColor(colour_hex)));
        cursor.mergeCharFormat(colorFormat);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Alt )
    {
        bool_alt_keypress = true;
    }
}

void log_viewer_interface::get_info_from_log_index_table()
{ // get all the info of log index table from log_viewer.sqlite database
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString command = "SELECT db_table_name,file_path , INT , source_count_name , recon_filefrom , recon_file_infopath FROM 'log_viewer_index' WHERE viewer_display_name = '" + log_viewer_display_name + "'";

    QSqlQuery query_index(destination_db);
    if(!query_index.exec(command))
    {
        recon_static_functions::app_debug("select_query execution-----FAILED----" + destination_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_index.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + query_index.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(query_index.next())
    {
        log_db_table_name = query_index.value("db_table_name").toString();
        log_file_path = query_index.value("file_path").toString();
        record_no = query_index.value("INT").toString();
        source_count_name = query_index.value("source_count_name").toString();
        complete_log_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,"log_viewer_index",source_count_name,Q_FUNC_INFO);
        recon_file_from_str = query_index.value("recon_filefrom").toString();
        recon_file_info_path_str = query_index.value("recon_file_infopath").toString();
    }

    destination_db.close();

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void log_viewer_interface::on_lineEdit_search_textChanged(const QString &arg1)
{ // search on line edit text changed directly without changing anything
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(ui->lineEdit_search->text().isEmpty())
    {
        QTextCharFormat txt_chr_frmt;
        ui->textBrowser_log_viewer->blockSignals(true);
        ui->textBrowser_log_viewer->clear();
        ui->textBrowser_log_viewer->setCurrentCharFormat(txt_chr_frmt);

        display_logs_view();
        ui->textBrowser_log_viewer->blockSignals(false);
    }

    return;

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::fetch_metadata_for_log_viewer()
{ // fetch metadata of log viewer to display the data in metadata section
    recon_static_functions::debug_intensive(" Starts " , Q_FUNC_INFO);

    QString metadata_str;
    QStringList tbl_name_list;

    tbl_name_list << log_db_table_name;
    QString source_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_count_name from log_viewer_index where db_table_name = ?", tbl_name_list, 0, destination_db_path, Q_FUNC_INFO);
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count);
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.plugin_name = MACRO_Plugin_Name_Log_Viewer;
    st_set_meta_obj.final_file_path = complete_log_file_path;
    st_set_meta_obj.db_file_path = fs_db_path;
    st_set_meta_obj.source_count_name = source_count_name;

    QString complete_source_name = struct_info.complete_source_name;
    metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name +"<br>";

    if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
    {
        metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +QString("\t") + struct_info.source_name +"<br>";

    }
    metadata_str += "<br>";

    metadata_str += "<b>" + QObject::tr("Viewer Display Name") + ":</b>" +QString("\t") + log_viewer_display_name +"<br><br>";

    QFileInfo file(log_file_path);

    metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + file.fileName() +"<br>";
    metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + log_file_path +"<br><br>";

    st_set_meta_obj.detailed_info = metadata_str;
    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::debug_intensive(" Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::slot_abouttoshow_tags_in_submenu()
{ // display tags submenu
    display_tags_submenu();
}

void log_viewer_interface::on_pushButton_search_clicked()
{ // search button clicked to search anything
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    ui->textBrowser_log_viewer->blockSignals(true);

    QTextCharFormat txt_chr_frmt;
    ui->textBrowser_log_viewer->clear();
    ui->textBrowser_log_viewer->setCurrentCharFormat(txt_chr_frmt);
    display_logs_view();

    QString searched_str = ui->lineEdit_search->text().trimmed();

    if(searched_str.isEmpty())
    {
        ui->textBrowser_log_viewer->blockSignals(false);
        return;
    }

    list_highlight_cursor.clear();

    current_search_index = 0;

    QTextDocument *document = ui->textBrowser_log_viewer->document();

    bool bool_match_found = false;

    QTextCursor highlight_cursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...  ");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    while(!highlight_cursor.isNull() && !highlight_cursor.atEnd())
    {
        highlight_cursor = document->find(searched_str, highlight_cursor);

        if(!highlight_cursor.isNull())
        {
            QCoreApplication::processEvents();

            QTextCharFormat plainFormat(highlight_cursor.charFormat());
            QTextCharFormat colorFormat = plainFormat;

            bool_match_found = true;
            colorFormat.setBackground(QColor("#D59B3F"));
            highlight_cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
            highlight_cursor.mergeCharFormat(colorFormat);

            list_highlight_cursor.append(highlight_cursor);
        }
    }

    if(bool_match_found)
    {
        on_pushButton_search_previous_clicked();
    }

    cursor.endEditBlock();

    display_loading_progress_bar_non_cancelable_obj->hide();
    ui->textBrowser_log_viewer->blockSignals(false);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::create_table_log_export_content()
{ // create table of log export content
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";

    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'log_content_info'(bookmark INT, INT INTEGER PRIMARY KEY,record_no VARCHAR(1000),tagged_data BLOB, file_name VARCHAR(1000), file_path VARCHAR(1000),"
                          "recon_filefrom VARCHAR(1000), recon_file_infopath VARCHAR(1000), file_size VARCHAR(1000), recon_tag_value VARCHAR(1000),"
                          "color_value VARCHAR(1000), source_count_name VARCHAR(1000), viewer_display_name VARCHAR(1000), notes VARCHAR(1000), source_file VARCHAR(1000))";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table log_content_info in log viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }

        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void log_viewer_interface::slot_right_click_main_menu_triggered(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString("Select All"))
    {
        ui->textBrowser_log_viewer->selectAll();
    }
}

void log_viewer_interface::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->textBrowser_log_viewer->mapToGlobal(pos);
    mainMenu->exec(globalPos);
}

void log_viewer_interface::on_textBrowser_log_viewer_selectionChanged()
{
    fetch_metadata_for_log_viewer();
}

void log_viewer_interface::on_pushButton_refresh_clicked()
{ // refresh button clicked to display unfiltered data
    QTextCharFormat txt_chr_frmt;
    ui->lineEdit_search->clear();
    ui->textBrowser_log_viewer->blockSignals(true);
    ui->textBrowser_log_viewer->clear();
    ui->textBrowser_log_viewer->setCurrentCharFormat(txt_chr_frmt);
    display_logs_view();
    ui->textBrowser_log_viewer->blockSignals(false);
}

void log_viewer_interface::on_pushButton_search_previous_clicked()
{ // search previous button clicked to see the searched keyword previous
    current_search_index--;

    if(current_search_index < 0)
        current_search_index = 0;

    if(!list_highlight_cursor.isEmpty())
    {
        if(list_highlight_cursor.size() > current_search_index)
        {

            QTextCursor scrollCursor = list_highlight_cursor.at(current_search_index);
            scrollCursor.beginEditBlock();
            ui->textBrowser_log_viewer->setTextCursor(scrollCursor);
            scrollCursor.endEditBlock();

        }
    }

}

void log_viewer_interface::on_pushButton_search_next_clicked()
{ // next button clicked to locate search keyword on displayed data
    current_search_index++;

    if(current_search_index > list_highlight_cursor.size())
        current_search_index = list_highlight_cursor.size() -1;

    if(!list_highlight_cursor.isEmpty())
    {
        if(list_highlight_cursor.size() > current_search_index)
        {

            QTextCursor scrollCursor = list_highlight_cursor.at(current_search_index);
            scrollCursor.beginEditBlock();
            ui->textBrowser_log_viewer->setTextCursor(scrollCursor);
            scrollCursor.endEditBlock();
        }
    }

}

void log_viewer_interface::on_lineEdit_search_returnPressed()
{ // search on enter clicked
    on_pushButton_search_clicked();
}
