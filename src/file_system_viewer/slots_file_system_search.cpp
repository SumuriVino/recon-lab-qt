#include "file_system.h"

void file_system::slot_show_files_recursively_clicked(bool value)
{
    Q_UNUSED(value);

    clear_all_custom_header_widget_entries();

    QFont font;
    font.setPointSize(12);
    pushbutton_show_files_recursively->setFont(font);
    pushbutton_show_files_recursively->setToolTip("Recursive View");

    if(recursive_view_status)  //show all
    {
        recursive_view_status = false;

        pushbutton_show_files_recursively->setIcon(QIcon("../icons/signs/recursive_on.png"));

        slot_pushbutton_showall_clicked();
    }
    else  //recursive view
    {
        recursive_view_status = true;

        pushbutton_show_files_recursively->setIcon(QIcon("../icons/signs/recursive_off.png"));

        //lineEdit_search->clear();

        QString parent_dir_path  = current_parent_dir_path;
        QString tmp_virtual_path = fs_extraction_path;

        QString current_parent_path = parent_dir_path;
        current_parent_path = current_parent_path.remove(0,tmp_virtual_path.size());

        //If path is empty then do add "/", done intentionally
        if(!current_parent_path.startsWith("/"))
            current_parent_path.prepend("/");

        //pushbutton_show_files_recursively->setDisabled(true);

        QString recursive_display_command = main_initial_db_cmd + QString(" LIKE  ?)");

        emit signal_disable_case_tree_sidebar(true);
        // emit signal_statusbar_progress_bar_show_hide(true , QString("Please wait..."));

        set_fs_display(recursive_display_command , current_parent_path , true , true);

        emit signal_disable_case_tree_sidebar(false);
        // emit signal_statusbar_progress_bar_show_hide(false , QString(""));
    }
}

void file_system::slot_hide_loading_display_dialog_box(bool status)
{
    bool_cancel_loading = status;
    display_loading_progress_bar_obj->hide();

    export_rsync_obj->pub_cancel_rsync_copy(Q_FUNC_INFO);
}

int file_system::get_main_combobox_index(QLayoutItem *item_combobox)
{
    int index_main = 0;

    for(int j = 0 ; j < item_combobox->layout()->count() ; j++)
    {
        QLayoutItem *item2 = item_combobox->layout()->itemAt(j);

        if(item2->spacerItem())
        {
            continue;
        }

        QString str_classname = item2->widget()->metaObject()->className();

        if(str_classname == "QComboBox")
        {
            QComboBox *box = new QComboBox(this);
            box =(QComboBox*) item2->widget();
            index_main = box->currentIndex();
            break;
        }
    }

    return index_main;
}

void file_system::slot_proc_generic_finished(int)
{
    bool_proc_generic_finished = true;
}

QString file_system::run_command_generic(QString command, QStringList arguments)
{
    bool_proc_generic_finished = false;
    prc_generic->start(command,arguments);


    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_proc_generic_finished)
        {
            break;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(prc_generic->readAll().trimmed());
}


void file_system::slot_dt_search_keywords_list_selection_done_clicked()
{
    keywords_list_selector_obj->hide();

    QStringList keyword_list;
    keyword_list << recon_helper_standard_obj->get_keyword_list_for_search_from_db(Q_FUNC_INFO);

    emit signal_run_index_k_search_on_current_dir(keyword_list, true, current_parent_dir_path, source_count_name);
}

void file_system::slot_pushbutton_search_filters_clicked()
{
   // menu_search_filters->exec();
}


void file_system::slot_table_header_customContextMenuRequested(const QPoint &points)
{
    return;
    QString manage_col_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString() + QString("/manage_columns.sqlite");
    QStringList vals_list;
    vals_list << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files);

    QString command = QString("Select hidden_column_name from hidden_columns Where plugin_name = ? AND tab_name = ?");
    QStringList hidden_columns_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, vals_list, 0, manage_col_db_path, Q_FUNC_INFO);

    foreach (QAction *action, menu_manage_column->actions())
    {
        if(hidden_columns_list.contains(action->text()))
        {
            action->setChecked(false);
        }
        else
        {
            action->setChecked(true);
        }
    }


    menu_manage_column->exec(QCursor::pos());
}


void file_system::slot_action_manage_column_clicked(QAction* current_clicked_action)
{

    current_clicked_action->setChecked(false);

    QString manage_column_db_path;
    manage_column_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString() + "manage_columns.sqlite";

    bool hidden_col_db_exist = false;

    QFile file(manage_column_db_path);
    if(file.exists())
    {
        hidden_col_db_exist = true;
    }

    if(m_file_system_tablewidget_obj != NULL)
    {
        for(int count = 0 ; count < m_file_system_tablewidget_obj->columnCount() ; count++)
        {
            clear_all_custom_header_widget_entries();
            custom_table_header_widget *header_widget_ptr = custom_header_widgets_QList.at(count);
            if(header_widget_ptr->pub_get_column_name() == current_clicked_action->text())
            {
                if(m_file_system_tablewidget_obj->isColumnHidden(count) && current_clicked_action->isChecked() == false)
                {
                    m_file_system_tablewidget_obj->showColumn(count);
                    current_clicked_action->setChecked(true);

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
                        QString command;
                        command = QString("delete from hidden_columns where hidden_columns_nos=") + QString::number(count) + QString(" AND plugin_name='") + QString(MACRO_Plugin_Name_File_System) + QString("'") + QString(" AND tab_name='") + QString(MACRO_Plugin_File_System_TAB_Name_Files) + QString("'") + QString(" AND os_scheme_display ='") + os_scheme_display + QString("'");

                        query.prepare(command);
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
                    m_file_system_tablewidget_obj->hideColumn(count);

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


                        query.prepare("insert into hidden_columns (plugin_name, tab_name, os_scheme_display, hidden_columns_nos, hidden_column_name) values(?,?,?,?,?)");

                        query.addBindValue(QString(MACRO_Plugin_Name_File_System));
                        query.addBindValue(QString(MACRO_Plugin_File_System_TAB_Name_Files));
                        query.addBindValue(os_scheme_display);
                        query.addBindValue(QString::number(count));
                        query.addBindValue(header_widget_ptr->pub_get_column_name());

                        if(!query.exec())
                        {
                            recon_static_functions::app_debug(" [slot_manage_column_clicked] query insertion ---FAILED---",Q_FUNC_INFO);
                            recon_static_functions::app_debug(" [slot_manage_column_clicked] error " + query.lastError().text(),Q_FUNC_INFO);
                        }
                        destination_db.close();

                    }
                }
            }
        }
    }
    refresh_horizontal_header();
}


QList<file_system::struct_manage_column_info> file_system::get_hidden_column_from_manage_column_db()
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
        command = QString("select * from hidden_columns where tab_name=") + QString("'") + QString(MACRO_Plugin_File_System_TAB_Name_Files) + QString("'") + QString(" AND plugin_name=") + QString("'") + QString(MACRO_Plugin_Name_File_System) + QString("'") + QString(" AND os_scheme_display=") + QString("'") + os_scheme_display + QString("'");
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

