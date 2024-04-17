#include "plist_viewer_interface.h"
#include "ui_plist_viewer_interface.h"

void plist_viewer_interface::contextMenuEvent(QContextMenuEvent *)
{
    QList <QTreeWidgetItem *> list = ui->treeWidget_plist->selectedItems();
    if(list.size() < 1)
        return;


    QMenu context_menu(this);
    context_menu.addMenu(menu_plist);
    menu_plist->exec(QCursor::pos());
}

void plist_viewer_interface::slot_plist_menu_clicked(QAction *action)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(action->text() == QString(MACRO_Generic_Right_Click_Bookmark_all))
    {

        display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
        display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...  ");
        display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_non_cancelable_obj->show();

        QTreeWidgetItem *child = new QTreeWidgetItem;
        QTreeWidgetItemIterator iter_tree(ui->treeWidget_plist);
        emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Plist_Viewer));

        create_table_export_content();
        qint64 counter = 0;
        while (*iter_tree)
        {

            counter++;
            if(counter % 100 == 0)
                QCoreApplication::processEvents();

            child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
            child->setCheckState(enum_plist_viewer_interface_TREE_KEY, Qt::Checked);

            QString inttext = child->text(enum_plist_viewer_interface_TREE_INT);
            QString tag_name = child->text(enum_plist_viewer_interface_TREE_TAG_NAMES);
            QString notes_text  = child->text(enum_plist_viewer_interface_TREE_NOTES);

            QStringList values;
            values << plist_db_table_name;
            QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_count_name from plist_viewer_index where db_table_name = ?", values, 0, destination_db_path, Q_FUNC_INFO);


            QList<struct_global_tagging_plist_viewer_record> plist_record_list;

            struct_global_tagging_plist_viewer_record obj;
            obj.INT_record  = inttext;
            obj.plugin_name = QString(MACRO_Plugin_Name_Plist_Viewer);
            obj.tab_name = QString(MACRO_Plugin_Plist_Viewer_TAB_Name_Plist_Viewer);
            obj.source_count_name = source_count_name;
            obj.plist_table_name = plist_db_table_name;
            obj.source_file = plist_file_path;
            obj.tag_status = tag_name;
            obj.bookmark_status = "1";

            obj.item0 = QFileInfo(plist_file_path).fileName();
            obj.item1 = plist_file_path;
            obj.item2 = child->text(enum_plist_viewer_interface_TREE_KEY);
            obj.item3 = child->text(enum_plist_viewer_interface_TREE_TYPE);
            obj.item4 = child->text(enum_plist_viewer_interface_TREE_VALUE);
            obj.item5 = plist_viewer_display_name;
            obj.file_size = QString::number(QFileInfo(complete_file_path).size());

            obj.keypath = child->text(enum_plist_viewer_interface_TREE_KEYPATH);
            obj.recon_file_from = recon_file_from_str;
            obj.recon_file_infopath = recon_file_info_path_str;
            obj.notes_text = child->text(enum_plist_viewer_interface_TREE_NOTES);


            plist_record_list << obj;


            global_connection_manager_obj->pub_set_bookmark_in_plist_viewer_db(QStringList(inttext) ,plist_db_table_name);
            global_connection_manager_obj->pub_add_data_in_export_content_plist_viewer_db(plist_record_list,tag_name,notes_text);
            global_connection_manager_obj->pub_add_bookmark_for_plist_viewer(plist_record_list);


            ++iter_tree;
        }

        display_loading_progress_bar_non_cancelable_obj->hide();

    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Remove_All_Bookmarks))
    {
        display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
        display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...  ");
        display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_non_cancelable_obj->show();


        QTreeWidgetItem *child = new QTreeWidgetItem;
        QTreeWidgetItemIterator iter_tree(ui->treeWidget_plist);

        qint64 counter = 0;
        while (*iter_tree)
        {

            counter++;
            if(counter % 100 == 0)
                QCoreApplication::processEvents();

            child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
            child->setCheckState(enum_plist_viewer_interface_TREE_KEY, Qt::Unchecked);

            QString inttext = child->text(enum_plist_viewer_interface_TREE_INT);
            QString tag_name  = child->text(enum_plist_viewer_interface_TREE_TAG_NAMES);
            QString notes_text  = child->text(enum_plist_viewer_interface_TREE_NOTES);

            global_connection_manager_obj->pub_remove_bookmark_in_plist_viewer_db(QStringList(inttext) ,plist_db_table_name);
            global_connection_manager_obj->pub_delete_bookmark_for_plist_viewer(QStringList(inttext),plist_viewer_display_name);

            if(tag_name.trimmed().isEmpty() && notes_text.trimmed().isEmpty())
                global_connection_manager_obj->pub_delete_entry_export_content_plist_viewer_db(QStringList(inttext) ,plist_viewer_display_name);
            else
                global_connection_manager_obj->pub_update_bookmark_export_content_plist_viewer_db(QStringList(inttext) ,plist_viewer_display_name);

            ++iter_tree;

        }
        display_loading_progress_bar_non_cancelable_obj->hide();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Add_Note))
    {
        add_notes_act();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Add_Notes_to_Bookmarks))
    {
        add_notes_to_bookmarks_act();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        remove_notes_act();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks))
    {
        remove_notes_from_bookmarks_act();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Remove_all_Notes))
    {
        remove_all_notes_act();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Copy_Row_To_Clipboard))
    {
        action_copy_row_to_clipboard_from_treewidget();
    }
    else if(action->text() == QString(MACRO_Generic_Right_Click_Copy_Cell_To_Clipboard))
    {
        action_copy_cell_to_clipboard_from_treewidget();
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void plist_viewer_interface::slot_sub_menu_tags_clicked(QAction *action)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString tag_name = action->text();

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag))
    {
        emit signal_remove_tags_from_plist_viewer();
        return;
    }

    if(tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);

    emit signal_add_tags(tag_name,colour);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::add_notes_to_bookmarks_act()
{
    add_notes_dialog_add_note_bookmark_obj->show();
}

void plist_viewer_interface::add_notes_act()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    add_notes_dialog_add_note_obj->show();

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening -----FAILED----- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    child = ui->treeWidget_plist->currentItem();

    QSqlQuery query(destination_db);
    query.prepare("select notes from '" + plist_db_table_name + "' where INT = ?");
    query.addBindValue(child->text(enum_plist_viewer_interface_TREE_INT));
    if(!query.exec())
    {
        recon_static_functions::app_debug("query -----FAILED----- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(query.next())
    {
        add_notes_dialog_add_note_obj->set_notes(query.value(0).toString());
    }

    destination_db.close();
    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::slot_add_notes(QString data)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);
    if(data.trimmed().isEmpty())
        return ;

    ui->treeWidget_plist->blockSignals(true);

    add_notes_dialog_add_note_obj->hide();

    create_table_export_content();

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    child = ui->treeWidget_plist->currentItem();

    QIcon icon("../icons/notes.png");
    child->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
    child->setText(enum_plist_viewer_interface_TREE_NOTES,data);
    QString bookmark_status = "0";
    if(child->checkState(enum_plist_viewer_interface_TREE_KEY))
        bookmark_status = "1";


    QStringList values;
    values << plist_db_table_name;
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_count_name from plist_viewer_index where db_table_name = ?", values, 0, destination_db_path, Q_FUNC_INFO);
    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Plist_Viewer));


    QList<struct_global_tagging_plist_viewer_record> plist_record_list;


    QString inttext = child->text(enum_plist_viewer_interface_TREE_INT);
    struct_global_tagging_plist_viewer_record obj;
    obj.INT_record  = inttext;
    obj.plugin_name = QString(MACRO_Plugin_Name_Plist_Viewer);
    obj.tab_name = QString(MACRO_Plugin_Plist_Viewer_TAB_Name_Plist_Viewer);
    obj.source_count_name = source_count_name;
    obj.plist_table_name = plist_db_table_name;
    obj.source_file = plist_file_path;
    obj.bookmark_status = bookmark_status;

    obj.item0 = QFileInfo(plist_file_path).fileName();
    obj.item1 = plist_file_path;
    obj.item2 = child->text(enum_plist_viewer_interface_TREE_KEY);
    obj.item3 = child->text(enum_plist_viewer_interface_TREE_TYPE);
    obj.item4 = child->text(enum_plist_viewer_interface_TREE_VALUE);
    obj.item5 = plist_viewer_display_name;
    obj.file_size = QString::number(QFileInfo(complete_file_path).size());

    obj.keypath = child->text(enum_plist_viewer_interface_TREE_KEYPATH);
    obj.recon_file_from = recon_file_from_str;
    obj.recon_file_infopath = recon_file_info_path_str;
    obj.notes_text = data;

    plist_record_list << obj;
    QString tag_name  = child->text(enum_plist_viewer_interface_TREE_TAG_NAMES);


    global_connection_manager_obj->pub_set_notes_in_plist_viewer_db(QStringList(inttext),plist_db_table_name , data);
    global_connection_manager_obj->pub_add_data_in_export_content_plist_viewer_db(plist_record_list,tag_name ,data);
    global_connection_manager_obj->pub_add_notes_for_plist_viewer(plist_record_list);
    global_connection_manager_obj->pub_set_notes_in_tags_search_for_plist_viewer(QStringList(inttext),plist_viewer_display_name,data);
    global_connection_manager_obj->pub_set_notes_in_bookmark_search_for_plist_viewer(QStringList(inttext),plist_viewer_display_name,data);

    ui->treeWidget_plist->blockSignals(false);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::slot_add_notes_to_bookmarks(QString data)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    data = data.trimmed();

    add_notes_dialog_add_note_bookmark_obj->hide();

    QStringList list;
    list << data;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("update '"+ plist_db_table_name + "' set notes = ? where bookmark = 1 AND notes is null",list, destination_db_path,Q_FUNC_INFO);

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    QTreeWidgetItemIterator iter_tree(ui->treeWidget_plist);

    while (*iter_tree)
    {
        child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
        if(child->checkState(enum_plist_viewer_interface_TREE_KEY))
        {
            QIcon icon("../icons/notes.png");
            child->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
            if(child->text(enum_plist_viewer_interface_TREE_NOTES) == "")
                child->setText(enum_plist_viewer_interface_TREE_NOTES, data);
        }

        ++iter_tree;
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::remove_notes_act()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    child = ui->treeWidget_plist->currentItem();


    QString INT_str = child->text(enum_plist_viewer_interface_TREE_INT);

    QIcon icon(QString(""));
    child->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
    child->setText(enum_plist_viewer_interface_TREE_NOTES, "");
    add_notes_dialog_add_note_obj->set_notes(QString(""));


    global_connection_manager_obj->pub_remove_notes_in_plist_viewer_db(QStringList(INT_str) ,plist_db_table_name);
    global_connection_manager_obj->pub_delete_notes_for_plist_viewer(QStringList(INT_str) ,plist_viewer_display_name);


    QString tag_name = child->text(enum_plist_viewer_interface_TREE_TAG_NAMES);
    QString bookmark_status = 0;
    if(child->checkState(enum_plist_viewer_interface_TREE_KEY))
        bookmark_status = "1";

    if(tag_name.trimmed().isEmpty() && bookmark_status == "0")
        global_connection_manager_obj->pub_delete_entry_export_content_plist_viewer_db(QStringList(INT_str),plist_viewer_display_name);
    else
        global_connection_manager_obj->pub_update_notes_export_content_plist_viewer_db(QStringList(INT_str),plist_viewer_display_name  , "");


    global_connection_manager_obj->pub_set_notes_in_tags_search_for_plist_viewer(QStringList(INT_str),plist_viewer_display_name,"");
    global_connection_manager_obj->pub_set_notes_in_bookmark_search_for_plist_viewer(QStringList(INT_str),plist_viewer_display_name,"");

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::remove_notes_from_bookmarks_act()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    QStringList list;
    list << "";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("update '" + plist_db_table_name + "' set notes = ? where bookmark = 1",list, destination_db_path,Q_FUNC_INFO);

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    QTreeWidgetItemIterator iter_tree(ui->treeWidget_plist);

    while (*iter_tree)
    {
        child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
        if(child->checkState(enum_plist_viewer_interface_TREE_KEY))
        {
            QIcon icon("");
            child->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
            child->setText(enum_plist_viewer_interface_TREE_NOTES, "");
        }

        ++iter_tree;
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::remove_all_notes_act()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);
    QStringList list;
    list << "";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("update '" + plist_db_table_name + "' set notes = ?",list, destination_db_path,Q_FUNC_INFO);

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    QTreeWidgetItemIterator iter_tree(ui->treeWidget_plist);

    while (*iter_tree)
    {
        child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
        QIcon icon("");
        child->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
        child->setText(enum_plist_viewer_interface_TREE_NOTES, "");

        ++iter_tree;
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::display_tags_submenu()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    submenu_tag->clear();

    ///add new tag
    QAction *act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    submenu_tag->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_action_create_tag_triggered()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_action_create_tag_triggered()));

    ///remove tag
    QAction *act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    submenu_tag->addAction(act_remove_tag);

    //    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_remove_tags_from_plugin()));
    //    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_remove_tags_from_plugin()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    submenu_tag->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                submenu_tag->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";

            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            submenu_tag->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

QStringList plist_viewer_interface::get_tags_and_colours_list_from_db(QString command,QString destination_db_file)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QStringList list_gen;

    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug("Db NOT Exists ---FAILED--- " + destination_db_file ,Q_FUNC_INFO);
        return list_gen;
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return list_gen;
    }

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return list_gen;
    }

    while(query.next())
    {
        QString value_str = query.value(0).toString().trimmed();

        value_str.append("::");
        value_str.append(query.value(1).toString().trimmed());

        list_gen << value_str;
    }

    destination_db.close();
    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

    return list_gen;
}

void plist_viewer_interface::slot_action_create_tag_triggered()
{
    add_tags_module_obj->show();
}

void plist_viewer_interface::slot_update_tag_selected_bytes_submenu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        QAction *act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        submenu_tag->addAction(act_already_saved_tags);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::slot_add_tags_plist_viewer_record(QString tag_name, QString colour_name)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);
    ui->treeWidget_plist->blockSignals(true);



    create_table_export_content();


    QTreeWidgetItem *child = new  QTreeWidgetItem;
    child = ui->treeWidget_plist->currentItem();
    child->setText(enum_plist_viewer_interface_TREE_TAG_NAMES, tag_name);
    QString color_hex = get_tag_color_hex_from_tag_db(tag_name, colour_name);
    child->setForeground(enum_plist_viewer_interface_TREE_KEY,QColor(color_hex));



    QStringList values;
    values << plist_db_table_name;
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_count_name from plist_viewer_index where db_table_name = ?", values, 0, destination_db_path, Q_FUNC_INFO);
    QString bookmark = "0";
    if(child->checkState(enum_plist_viewer_interface_TREE_KEY))
        bookmark = "1";

    QString notes_text  = child->text(enum_plist_viewer_interface_TREE_NOTES);


    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Plist_Viewer));


    QList<struct_global_tagging_plist_viewer_record> plist_record_list;

    struct_global_tagging_plist_viewer_record obj;
    obj.INT_record  = child->text(enum_plist_viewer_interface_TREE_INT);
    obj.plugin_name = QString(MACRO_Plugin_Name_Plist_Viewer);
    obj.tab_name = QString(MACRO_Plugin_Plist_Viewer_TAB_Name_Plist_Viewer);
    obj.source_count_name = source_count_name;
    obj.plist_table_name = plist_db_table_name;
    obj.source_file = plist_file_path;
    obj.bookmark_status = bookmark;

    obj.item0 = QFileInfo(plist_file_path).fileName();
    obj.item1 = plist_file_path;
    obj.item2 = child->text(enum_plist_viewer_interface_TREE_KEY);
    obj.item3 = child->text(enum_plist_viewer_interface_TREE_TYPE);
    obj.item4 = child->text(enum_plist_viewer_interface_TREE_VALUE);
    obj.item5 = plist_viewer_display_name;
    obj.file_size = QString::number(QFileInfo(complete_file_path).size());

    obj.keypath = child->text(enum_plist_viewer_interface_TREE_KEYPATH);
    obj.color_hex = color_hex;
    obj.recon_file_from = recon_file_from_str;
    obj.recon_file_infopath = recon_file_info_path_str;
    obj.notes_text = child->text(enum_plist_viewer_interface_TREE_NOTES);


    plist_record_list << obj;


    global_connection_manager_obj->pub_set_tag_in_plist_viewer_db(QStringList(obj.INT_record) ,plist_db_table_name , tag_name);
    global_connection_manager_obj->pub_add_data_in_export_content_plist_viewer_db(plist_record_list , tag_name , notes_text);
    global_connection_manager_obj->pub_add_tag_for_plist_viewer(plist_record_list , tag_name);

    ui->treeWidget_plist->blockSignals(false);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void plist_viewer_interface::slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        submenu_tag->addAction(act_already_saved_tags);

        emit signal_update_tag_submenu_of_plugins(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}


QString plist_viewer_interface::get_tag_color_hex_from_tag_db(QString tag_name,QString color_name)
{

    QString tags_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString colour_hex;

    QString command = "SELECT color_hex FROM tags WHERE tag_name=? AND selected_colour=?";
    QStringList val_list;
    val_list << tag_name << color_name;

    colour_hex = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,tags_db_path,Q_FUNC_INFO);

    return colour_hex;
}

QString plist_viewer_interface::update_tag_status_for_current_record(QString tag_data, QString colour, QString record_no_str)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString command = "SELECT recon_tag_value from '" + plist_db_table_name + "' where INT "  + "=?";
    QStringList arg_list;
    arg_list << record_no_str;
    QString previous_tag = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,destination_db_path,Q_FUNC_INFO);

    QString added_tag_name = tag_data;
    if(!previous_tag.trimmed().isEmpty())
    {
        QStringList temp_taglist = previous_tag.split(",", Qt::SkipEmptyParts);
        if(temp_taglist.contains(tag_data))
            return previous_tag;
        else
        {
            added_tag_name.append(",");
            added_tag_name.append(previous_tag);
        }
    }

    command.clear();
    command = "update '" + plist_db_table_name + "' set recon_tag_value = ? where INT = ?";
    arg_list.clear();
    arg_list << added_tag_name << record_no_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return added_tag_name;
}

void plist_viewer_interface::slot_remove_tags_from_plist_viewer()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    ui->treeWidget_plist->blockSignals(true);

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    child = ui->treeWidget_plist->currentItem();

    QString record_no = child->text(enum_plist_viewer_interface_TREE_INT);

    global_connection_manager_obj->pub_remove_tag_in_plist_viewer_db(QStringList(record_no) , plist_db_table_name);

    QString notes_text = child->text(enum_plist_viewer_interface_TREE_NOTES);

    if(child->checkState(enum_plist_viewer_interface_TREE_KEY))
        global_connection_manager_obj->pub_update_tag_export_content_plist_viewer_db(QStringList(record_no) , plist_viewer_display_name);
    else if(!notes_text.trimmed().isEmpty())
        global_connection_manager_obj->pub_update_tag_export_content_plist_viewer_db(QStringList(record_no) , plist_viewer_display_name);
    else
        global_connection_manager_obj->pub_delete_entry_export_content_plist_viewer_db(QStringList(record_no) , plist_viewer_display_name);

    global_connection_manager_obj->pub_delete_tag_for_plist_viewer(QStringList(record_no) , plist_viewer_display_name);


    ui->treeWidget_plist->blockSignals(false);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::slot_tag_list_after_remove_multiple_tag(QStringList list_tags_remained, QString table_name, QString record)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QTreeWidgetItem *child = new  QTreeWidgetItem;
    child = ui->treeWidget_plist->currentItem();

    QString record_no = child->text(enum_plist_viewer_interface_TREE_INT);

    child->setText(enum_plist_viewer_interface_TREE_TAG_NAMES, list_tags_remained.join(","));

    QString tags_str = list_tags_remained.join(",");

    QString command = "update '" + table_name + "' set recon_tag_value=? where INT " + "=?";
    QStringList arg_list;
    arg_list << tags_str << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,destination_db_path,Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

    command.clear();
    command = "update plist_content_info set recon_tag_value = ? WHERE record_no=? AND viewer_display_name = ?";

    arg_list.clear();
    arg_list << tags_str << record << plist_viewer_display_name;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);

    if(tags_str == "")
    {
        QIcon icon(QString(""));
        child->setIcon(enum_plist_viewer_interface_TREE_TAG, icon);
    }
    else if(tags_str.contains(","))
    {
        QString icon_path = "../icons/tag_icons/multiple.png";
        QIcon icon(icon_path);
        child->setIcon(enum_plist_viewer_interface_TREE_TAG, icon);
    }
    else if((tags_str != "") && (!tags_str.contains(",")))
    {
        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags_str, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
        color = color.toLower();
        color = color.replace(" ","_");
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(color, Q_FUNC_INFO);
        QIcon icon(icon_path);
        child->setIcon(enum_plist_viewer_interface_TREE_TAG, icon);
    }

    emit signal_update_tag_search_db_and_display(MACRO_Plugin_Name_Plist_Viewer,QString("Files"), record,list_tags_remained,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::slot_removed_tags_list_plist_viewer(QStringList list_removed_tags, QString table_name, QString record_no)
{
    emit signal_plist_viewer_removed_tags_list(MACRO_Plugin_Name_Plist_Viewer,MACRO_Plugin_Plist_Viewer_TAB_Name_Plist_Viewer,record_no,list_removed_tags,MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display);
}


void plist_viewer_interface::action_copy_row_to_clipboard_from_treewidget()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QList <QTreeWidgetItem *> list = ui->treeWidget_plist->selectedItems();

    QString clipboard_header_str, clipboard_data_str;

    QApplication::clipboard()->clear(QClipboard::Clipboard);

    clipboard_header_str.append(ui->treeWidget_plist->headerItem()->text(enum_plist_viewer_interface_TREE_KEY) + "\t");
    clipboard_header_str.append(ui->treeWidget_plist->headerItem()->text(enum_plist_viewer_interface_TREE_VALUE) + "\t");
    clipboard_header_str.append(ui->treeWidget_plist->headerItem()->text(enum_plist_viewer_interface_TREE_TYPE) + "\t");
    clipboard_header_str.append(ui->treeWidget_plist->headerItem()->text(enum_plist_viewer_interface_TREE_NOTES) + "\t");

    for(int i = 0; i < list.size() ; i++)
    {
        clipboard_data_str.append(list.at(i)->text(enum_plist_viewer_interface_TREE_KEY) + "\t");
        clipboard_data_str.append(list.at(i)->text(enum_plist_viewer_interface_TREE_VALUE) + "\t");
        clipboard_data_str.append(list.at(i)->text(enum_plist_viewer_interface_TREE_TYPE) + "\t");
        clipboard_data_str.append(list.at(i)->text(enum_plist_viewer_interface_TREE_NOTES) + "\t");
    }

    QString combine_data_str;
    combine_data_str = clipboard_header_str.trimmed() + "\n" + clipboard_data_str.trimmed();

    QApplication::clipboard()->setText(combine_data_str, QClipboard::Clipboard);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void plist_viewer_interface::action_copy_cell_to_clipboard_from_treewidget()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QTreeWidgetItem *item = ui->treeWidget_plist->currentItem();

    QApplication::clipboard()->clear(QClipboard::Clipboard);

    QApplication::clipboard()->setText(item->text(ui->treeWidget_plist->currentColumn()).trimmed(), QClipboard::Clipboard);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

QStringList plist_viewer_interface::get_values_from_plist_db_according_to_display_tab(QString record_no)
{

    QStringList values_list;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(destination_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return values_list;
    }

    QSqlQuery query(source_db);
    QString command ;
    if(record_no.trimmed().isEmpty())
    {
        command = "select key,type,value,parent,keypath,parentpath from '" + plist_db_table_name + "'";
        query.prepare(command);
    }
    else
    {
        command = "select key,type,value,parent,keypath,parentpath from '" + plist_db_table_name + "' where INT=? ";
        query.prepare(command);
        query.addBindValue(record_no);
    }

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return values_list;
    }

    if(query.next())
    {
        values_list << query.value("key").toString()
                    << query.value("value").toString()
                    << query.value("type").toString()
                    << query.value("keypath").toString();
    }

    source_db.close();

    return values_list;

}

void plist_viewer_interface::slot_act_manage_keys_triggered()
{
    recon_static_functions::app_debug ("Starts ", Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait......");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->show();

    manage_plist_key_obj->setWindowTitle("Manage Keys");
    manage_plist_key_obj->draw_table_manage_plist_key(keys_list_of_plist_file);
    manage_plist_key_obj->show();

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug ("Ends ", Q_FUNC_INFO);
}

void plist_viewer_interface::slot_update_conversion_type_on_combobox_conversion_change(QString data_str, QString key_name)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QStringList arg_list;
    QString command_update = QString("UPDATE '" + plist_db_table_name + "' SET data_conversion_type = ? WHERE key = ?");
    arg_list << data_str << key_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,destination_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}
