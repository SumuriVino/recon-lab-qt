#include "report_launcher.h"
#include "ui_report_launcher.h"

report_launcher::report_launcher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::report_launcher)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->pushButton_back->setEnabled(false);
    ui->pushButton_next->setEnabled(true);
    ui->pushButton_next->setText("Next");

    page_report_case_info_object = new page_report_case_info(this);
    ui->stackedWidget_report_launcher->insertWidget(enum_report_launcher_page_case_info,page_report_case_info_object);
    ui->stackedWidget_report_launcher->setCurrentIndex(enum_report_launcher_page_case_info);

    page_report_type_selection_object = new page_report_type_selection(this);
    ui->stackedWidget_report_launcher->insertWidget(enum_report_launcher_page_report_type_selection,page_report_type_selection_object);
    connect(page_report_type_selection_object,SIGNAL(signal_report_type_selected(bool)),this,SLOT(slot_report_type_selected(bool)));
    connect(page_report_type_selection_object,SIGNAL(signal_report_scope_selected(QString)),this,SLOT(slot_report_scope_selected(QString)));

    page_report_plugins_selection_object = new plugin_selector(this);
    ui->stackedWidget_report_launcher->insertWidget(enum_report_launcher_report_plugin_selection,page_report_plugins_selection_object);
    connect(page_report_plugins_selection_object,SIGNAL(signal_plugin_selected(bool)),this,SLOT(slot_plugins_selected(bool)));
    bool_plugins_selected = false;
    bool_report_type_selected = false;
    report_scope_name = MACRO_REPORT_SCOPE_TAGS_QString;

    ui->checkBox_encrypt_report->setChecked(false);
    ui->lineEdit_password->setDisabled(true);
    ui->lineEdit_password->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->checkBox_show_password->setDisabled(true);


}

report_launcher::~report_launcher()
{
    delete ui;
}

void report_launcher::set_narad_muni_for_report(narad_muni *obj)
{
    narad_muni_for_report_obj = obj;
}

void report_launcher::pub_initialise_launcher()
{

    //1. Case Info.
    page_report_case_info_object->set_narad_muni_for_report(narad_muni_for_report_obj);
    page_report_case_info_object->pub_initialise_case_info();

    //2. Report Type Selection.
    page_report_type_selection_object->set_narad_muni_for_report(narad_muni_for_report_obj);
    page_report_type_selection_object->pub_initialise_report_type_page();


    //3. Plugin Selection.
    page_report_plugins_selection_object->pub_set_launcher_type(MACRO_Launcher_Plugin_Selection_G_Report);
    page_report_plugins_selection_object->set_narad_muni_for_report(narad_muni_for_report_obj);
    page_report_plugins_selection_object->pub_initialise_plugin_info_for_global_report();

    signal_set_window_title("Global Report - Case informaion");

    ui->stackedWidget_report_launcher->setCurrentIndex(enum_report_launcher_page_case_info);
    ui->pushButton_next->setEnabled(true);
    ui->pushButton_next->setText("Next");

    report_scope_name = MACRO_REPORT_SCOPE_TAGS_QString;

}

void report_launcher::on_pushButton_next_clicked()
{
    bool bool_generate_report = false;

    if(ui->stackedWidget_report_launcher->currentIndex() == enum_report_launcher_page_case_info)
    {
        ui->pushButton_back->setEnabled(true);

        if(report_scope_name == MACRO_REPORT_SCOPE_FULL_QString)
            ui->pushButton_next->setText("Next");
        else
            ui->pushButton_next->setText("Report");

        ui->stackedWidget_report_launcher->setCurrentIndex(enum_report_launcher_page_report_type_selection);

        if(bool_report_type_selected)
            ui->pushButton_next->setEnabled(true);
        else
            ui->pushButton_next->setEnabled(false);
        emit signal_set_window_title("Global Report - Report Category");

    }
    else if(ui->stackedWidget_report_launcher->currentIndex() == enum_report_launcher_page_report_type_selection)
    {
        ui->pushButton_back->setEnabled(true);

        if(bool_report_type_selected)
            ui->pushButton_next->setEnabled(true);
        else
            ui->pushButton_next->setEnabled(false);

        emit signal_set_window_title("Global Report - Plugin Selection");

        if(report_scope_name == MACRO_REPORT_SCOPE_FULL_QString)
        {
            bool_generate_report = false;
            ui->pushButton_next->setText("Report");
            ui->stackedWidget_report_launcher->setCurrentIndex(enum_report_launcher_report_plugin_selection);
        }
        else
            bool_generate_report = true;
    }
    else if(ui->stackedWidget_report_launcher->currentIndex() == enum_report_launcher_report_plugin_selection)
    {
        bool_generate_report = true;
        ui->pushButton_back->setEnabled(true);
    }

    if(bool_generate_report && ui->pushButton_next->text() == "Report")
    {

        get_info_from_pages_for_generate_report();

        if(ui->checkBox_encrypt_report->isChecked())
        {
            if(ui->lineEdit_password->text().isEmpty())
            {
                QMessageBox::StandardButton ret;
                ret = QMessageBox::warning(this, tr(""), tr("Please Enter Password for encryption"),QMessageBox::Ok);
                return;
            }
            narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Password_QString, ui->lineEdit_password->text());

        }
        else
            narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Password_QString, "");



        if(report_scope_name == MACRO_REPORT_SCOPE_FULL_QString)
        {
            if(!page_report_plugins_selection_object->pub_bool_run_plugin_having_record())
            {
                QMessageBox::StandardButton ret;
                ret = QMessageBox::warning(this, tr(""), tr(" Artifacts not found "),QMessageBox::Ok);
                return ;
            }

            QStringList report_plugin_info_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Global_Report_Selected_Plugins_With_Tabs_AND_Export_Status_QStringList).toStringList();
            if(report_plugin_info_list.size() == 0)
            {
                QMessageBox::StandardButton ret;
                ret = QMessageBox::warning(this, tr(""), tr(" Please select at least one plugin or category"),QMessageBox::Ok);
                return ;
            }
        }

        emit signal_all_report_selection_completed();
    }

}


void report_launcher::on_pushButton_back_clicked()
{
    int index = ui->stackedWidget_report_launcher->currentIndex();

    if(index <= enum_report_launcher_page_case_info)
        return;

    ui->pushButton_next->setText("Next");
    ui->stackedWidget_report_launcher->setCurrentIndex(index - 1);

    index = index - 1;

    switch (index) {
    case enum_report_launcher_page_case_info:
        ui->pushButton_back->setDisabled(true);
        ui->pushButton_next->setEnabled(true);
        emit signal_set_window_title("Global Report - Case information");

        break;
    case enum_report_launcher_report_plugin_selection:
        ui->pushButton_back->setEnabled(true);
        ui->pushButton_next->setEnabled(true);
        emit signal_set_window_title("Global Report - Plugin Selection");
        break;
    case enum_report_launcher_page_report_type_selection:
        ui->pushButton_back->setEnabled(true);
        ui->pushButton_next->setEnabled(true);

        break;
    default:
        break;
    }

}

void report_launcher::slot_plugins_selected(bool status)
{
    if(!status)
    {
        bool_plugins_selected = false;
        ui->pushButton_next->setEnabled(false);
    }
    else
    {
        bool_plugins_selected = true;
        ui->pushButton_next->setEnabled(true);
    }
}

void report_launcher::slot_report_type_selected(bool check)
{
    bool_report_type_selected = check;

    if(check)
        ui->pushButton_next->setEnabled(true);
    else
        ui->pushButton_next->setEnabled(false);
}

void report_launcher::slot_report_scope_selected(QString scope)
{
    report_scope_name = scope;

    if(scope == MACRO_REPORT_SCOPE_TAGS_QString)
    {
        ui->pushButton_next->setText("Report");
    }
    else if(scope == MACRO_REPORT_SCOPE_FULL_QString)
    {
        ui->pushButton_next->setText("Next");
    }

}

void report_launcher::get_info_from_pages_for_generate_report()
{
    page_report_case_info_object->pub_set_case_info_narad_muni();
    page_report_type_selection_object->pub_set_report_type_narad_muni();

    if(ui->pushButton_next->text() == "Report" && report_scope_name == MACRO_REPORT_SCOPE_FULL_QString)
        page_report_plugins_selection_object->pub_put_plugin_info_for_global_report();
    else if((ui->pushButton_next->text() == "Report" && report_scope_name == MACRO_REPORT_SCOPE_TAGS_QString))
        get_taged_selected_plugins_info();
}

void report_launcher::get_taged_selected_plugins_info()
{
    QStringList selected_tags_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Global_Report_Selected_Tags_QStringList).toStringList();

    QString tag_index_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QString command_select = "SELECT tag_db_name FROM tag_search_index";
    QStringList applied_tags_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_select,0,tag_index_db_path,Q_FUNC_INFO);

    QString bokkmarks_index_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString command_select_bookmarks = "SELECT INT FROM tags";
    QStringList list_int = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_select_bookmarks,0,bokkmarks_index_db_path,Q_FUNC_INFO);
    if(!list_int.isEmpty())
        applied_tags_list << QString(MACRO_Tag_Name_Bookmarks);

    QString screen_shot_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
    qint64 total_records_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true,screen_shot_db_path,QStringList("tags"),Q_FUNC_INFO);
    if(total_records_count > 0)
        applied_tags_list << QString(MACRO_Tag_Name_Screenshots);

    QString messenger_graph_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
    bool bool_messenger_graph_is_available = false;
    qint64 total_records_count_messenger = recon_helper_standard_obj->get_db_total_record_by_dbpath(true,messenger_graph_db_path,QStringList("saved_graphs"),Q_FUNC_INFO);
    if(total_records_count_messenger > 0)
    {
        bool_messenger_graph_is_available = true;
    }


    QString browsers_graph_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
    bool bool_brwosers_graph_is_available = false;
    qint64 total_records_count_broswers = recon_helper_standard_obj->get_db_total_record_by_dbpath(true,browsers_graph_db_path,QStringList("saved_graphs"),Q_FUNC_INFO);
    if(total_records_count_broswers > 0)
    {
        bool_brwosers_graph_is_available = true;
    }



    QString saved_timeline_graph_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
    bool bool_timeline_graph_is_available = false;
    qint64 total_records_count_timeline = recon_helper_standard_obj->get_db_total_record_by_dbpath(true,saved_timeline_graph_db_path,QStringList("tags"),Q_FUNC_INFO);
    if(total_records_count_timeline > 0)
    {
        bool_timeline_graph_is_available = true;
    }





    total_records_count = total_records_count_messenger + total_records_count_broswers + total_records_count_timeline;
    if(total_records_count > 0)
        applied_tags_list << QString(MACRO_Tag_Name_Saved_graphs);

    QString saved_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
    total_records_count =  recon_helper_standard_obj->get_db_total_record_by_dbpath(true,saved_db_path,QStringList("tags"),Q_FUNC_INFO);
    if(total_records_count > 0)
        applied_tags_list << QString(MACRO_Tag_Name_Saved_Maps);


    QString chat_viewer_bookmark_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
    qint64 bookmarked_chat_record = recon_helper_standard_obj->get_db_total_record_by_dbpath(true,chat_viewer_bookmark_db_path,QStringList("chat_viewer_info"),Q_FUNC_INFO);
    QStringList plugin_name_list;
    QStringList tab_name_list ;
    if(bookmarked_chat_record != 0)
    {
        plugin_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("SELECT DISTINCT plugin_name FROM chat_viewer_info", 0 , chat_viewer_bookmark_db_path,Q_FUNC_INFO);
        tab_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("SELECT DISTINCT tab_name FROM chat_viewer_info", 0 , chat_viewer_bookmark_db_path,Q_FUNC_INFO);
        if(!applied_tags_list.contains(QString(MACRO_Tag_Name_Bookmarks)))
            applied_tags_list << QString(MACRO_Tag_Name_Bookmarks);

    }


    QString examiner_commennts =  recon_helper_standard_obj->pub_fetch_examiner_comments_data();
    if(!examiner_commennts.trimmed().isEmpty())
        applied_tags_list << QString(MACRO_Tag_Name_Examiner_Comments);

    QStringList plugins_and_tab_list;
    for(int i = 0; i < applied_tags_list.size(); i++)
    {
        QString tag_db_name = applied_tags_list.at(i);
        if(!selected_tags_list.contains(tag_db_name))
            continue;

        QString tag_db_path;
        if(tag_db_name == QString(MACRO_Tag_Name_Bookmarks))
            tag_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
        else if(tag_db_name == MACRO_Tag_Name_Screenshots)
        {
            plugins_and_tab_list << QString(MACRO_Plugin_Name_Screenshots) + MACRO_RECON_Splitter_2_hash_in_brace  + QString(MACRO_Plugin_Name_Screenshots);
            continue;
        }
        else if(tag_db_name == MACRO_Tag_Name_Saved_graphs)
        {
            if(bool_messenger_graph_is_available)
                plugins_and_tab_list << QString(MACRO_Plugin_Name_Saved_Graphs_Messenger) + MACRO_RECON_Splitter_2_hash_in_brace  + QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs);

            if(bool_brwosers_graph_is_available)
                plugins_and_tab_list << QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History) + MACRO_RECON_Splitter_2_hash_in_brace  + QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs);


            if(bool_timeline_graph_is_available)
                plugins_and_tab_list << QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph) + MACRO_RECON_Splitter_2_hash_in_brace  + QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph);


            continue;
        }
        else if(tag_db_name == MACRO_Tag_Name_Saved_Maps)
        {
            plugins_and_tab_list << QString(MACRO_Plugin_Name_Saved_Maps) + MACRO_RECON_Splitter_2_hash_in_brace  + QString(MACRO_Plugin_Saved_Maps_TAB_Name_Saved_Maps);
            continue;
        }
        else if(tag_db_name == MACRO_Tag_Name_Examiner_Comments)
        {
            plugins_and_tab_list << QString(MACRO_Plugin_Name_Examiner_Space) + MACRO_RECON_Splitter_2_hash_in_brace + QString(MACRO_Tag_Name_Examiner_Comments);
            continue;
        }
        else
            tag_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name + ".sqlite";




        QString chat_view_details_plugin_and_tab;
        if(bookmarked_chat_record != 0)
        {
            if(plugin_name_list.size() == tab_name_list.size())
            {
                for(int i = 0 ; i < plugin_name_list.size() ; i++)
                {
                    plugins_and_tab_list << plugin_name_list.at(i) + MACRO_RECON_Splitter_2_hash_in_brace + tab_name_list.at(i);
                }
            }
        }



        QSqlDatabase destination_db;
        QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase::removeDatabase(connection_naam);
        destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(tag_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + tag_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        QSqlQuery query_select(destination_db);
        query_select.prepare("SELECT DISTINCT csv_plugin_name, csv_tab_name FROM tags");
        if(!query_select.exec())
        {
            recon_static_functions::app_debug(" query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Query -  " + query_select.executedQuery(),Q_FUNC_INFO);
            continue;
        }

        while(query_select.next())
        {
            QString plugin_name = query_select.value(0).toString();
            QString tab_name = query_select.value(1).toString();

            QString all_str = plugin_name + MACRO_RECON_Splitter_2_hash_in_brace + tab_name;

            if(plugins_and_tab_list.contains(all_str))
                continue;

            plugins_and_tab_list << all_str;

            if(plugins_and_tab_list.contains(chat_view_details_plugin_and_tab))
                continue;
            else
                plugins_and_tab_list << chat_view_details_plugin_and_tab ;
        }


        destination_db.close();
    }

    QStringList selected_plugins_tabs_with_export_list;
    selected_plugins_tabs_with_export_list << get_plugin_and_tab_list(plugins_and_tab_list,"");

    narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Selected_Plugins_With_Tabs_AND_Export_Status_QStringList, get_plugin_and_tab_list(plugins_and_tab_list,""));
}

QStringList report_launcher::get_plugin_and_tab_list(QStringList selected_plugins_with_and_tabs_list, QString os_name)
{
    QMap<QString, QStringList> map_selected_plugins_and_tabs_list;
    for(int i = 0; i < selected_plugins_with_and_tabs_list.size(); i++)
    {
        QString tmp_str = selected_plugins_with_and_tabs_list.at(i);

        QStringList tmp_list = tmp_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            continue;

        QString plugin_name = tmp_list.at(0);
        QString tab_name = tmp_list.at(1);

        QStringList tabs_list = map_selected_plugins_and_tabs_list.take(plugin_name);
        tabs_list << tab_name;
        map_selected_plugins_and_tabs_list.insert(plugin_name,tabs_list);
    }

    QStringList proper_plugins_with_tabs_list;
    QMapIterator<QString, QStringList> map_it(map_selected_plugins_and_tabs_list);
    for(int i = 0; i < map_selected_plugins_and_tabs_list.size(); i++)
    {
        map_it.next();

        QString plugin_name = map_it.key();
        QStringList selected_tabs_list = map_it.value();

        QStringList selected_tabs_list_with_export_status;
        if(plugin_name.contains(QString(MACRO_Plugin_Name_File_System)) || plugin_name.contains(QString(MACRO_Plugin_Name_Hex_Viewer))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Plist_Viewer))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Sqlite_Viewer))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Log_Viewer))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Screenshots))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
                || plugin_name.contains(QString(MACRO_Plugin_Name_Examiner_Space)))
        {
            for(int i = 0; i < selected_tabs_list.size(); i++)
            {
                QString tab_name = selected_tabs_list.at(i);

                QString tab_with_export_status;
                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Global_Report_Is_Export_Plugins_for_Tag_Checked_QString).toString() == "1")
                    tab_with_export_status = tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "1";
                else
                    tab_with_export_status = tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "0";

                selected_tabs_list_with_export_status << tab_with_export_status;
            }
        }
        else
        {
            QStringList export_tabs_list = global_csv_reader_class_selected_obj->get_export_tab_list(plugin_name);

            for(int i = 0; i < selected_tabs_list.size(); i++)
            {
                QString tab_name = selected_tabs_list.at(i);

                QString tab_with_export_status;
                if(export_tabs_list.contains(tab_name) && narad_muni_for_report_obj->get_field(MACRO_NARAD_Global_Report_Is_Export_Plugins_for_Tag_Checked_QString).toString() == "1")
                    tab_with_export_status = tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "1";
                else
                    tab_with_export_status = tab_name + MACRO_RECON_Splitter_3_dollar_in_brace + "0";

                selected_tabs_list_with_export_status << tab_with_export_status;
            }
        }

        QString joined_tabs = selected_tabs_list_with_export_status.join(MACRO_RECON_Splitter_2_hash_in_brace);
        QString joined_plugin_and_tabs = plugin_name + MACRO_RECON_Splitter_1_multiple + joined_tabs;

        proper_plugins_with_tabs_list << joined_plugin_and_tabs;
    }

    return proper_plugins_with_tabs_list;
}

void report_launcher::on_checkBox_show_password_toggled(bool checked)
{
    if(checked)
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

void report_launcher::on_checkBox_encrypt_report_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit_password->setDisabled(false);
        ui->checkBox_show_password->setDisabled(false);
    }
    else
    {
        ui->lineEdit_password->setDisabled(true);
        ui->checkBox_show_password->setDisabled(true);
        ui->lineEdit_password->clear();
    }
}
