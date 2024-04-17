#include "plist_viewer_interface.h"
#include "ui_plist_viewer_interface.h"

plist_viewer_interface::plist_viewer_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plist_viewer_interface)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->treeWidget_plist->setColumnHidden(enum_plist_viewer_interface_TREE_KEYPATH, true);
    ui->treeWidget_plist->setColumnHidden(enum_plist_viewer_interface_TREE_INT, true);
    ui->treeWidget_plist->setColumnHidden(enum_plist_viewer_interface_TREE_TAG_NAMES, true);

    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_KEY,500);
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_VALUE,300);
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_NOTES,500);
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_TAG,80);
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_TAG_NAMES,100);
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_NOTE_ICON,20);

    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_TYPE,300);
    //  ui->treeWidget_plist->setColumnHidden(enum_plist_viewer_interface_TREE_NOTE_ICON, true);///show when notes available

    ///single row should be selected at a time
    ui->treeWidget_plist->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_plist->setStyleSheet( QString("QTreeView::item:selected:!active {"
                                                 "color:white;"
                                                 " }"

                                                 "QTreeView {"
                                                 "   }"

                                                 "QTreeView::item:selected:!active {"
                                                 "color:white;"
                                                 "background-color:  #a1c9f7;"
                                                 "}"));

    ui->treeWidget_plist->setAttribute(Qt::WA_MacShowFocusRect,0);
    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,0);

    add_tags_module_obj = new add_tags(this);
    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_in_actions_menu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags_plist_viewer_record(QString,QString)));
    connect(this, SIGNAL(signal_add_tags(QString,QString)), this, SLOT(slot_add_tags_plist_viewer_record(QString,QString)));
    connect(this, SIGNAL(signal_remove_tags_from_plist_viewer()), this, SLOT(slot_remove_tags_from_plist_viewer()));

    add_notes_dialog_add_note_obj = new add_notes(this);
    connect(add_notes_dialog_add_note_obj, SIGNAL(signal_notes(QString)), this, SLOT(slot_add_notes(QString)));

    add_notes_dialog_add_note_bookmark_obj = new add_notes(this);
    connect(add_notes_dialog_add_note_bookmark_obj, SIGNAL(signal_notes(QString)), this, SLOT(slot_add_notes_to_bookmarks(QString)));

    manage_plist_key_obj = new manage_plist_key(this);
    connect(manage_plist_key_obj,SIGNAL(signal_data_conversion_timestamp_type_change(QString, QString)),this,SLOT(slot_update_conversion_type_on_combobox_conversion_change(QString,QString)));

    remove_multiple_tag_selecotor_obj = new remove_multiple_tag_selecotor(this);

    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_remaining_tags_list(QStringList, QString , QString)),this,SLOT(slot_tag_list_after_remove_multiple_tag(QStringList, QString , QString)));
    connect(remove_multiple_tag_selecotor_obj,SIGNAL(signal_tag_list_to_be_removed(QStringList, QString , QString)),this,SLOT(slot_removed_tags_list_plist_viewer(QStringList, QString , QString)));

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);


}

plist_viewer_interface::~plist_viewer_interface()
{
    delete ui;
}

void plist_viewer_interface::slot_plist_viewer_db_path_changed(QString db_path)
{
    destination_db_path = db_path;
}

void plist_viewer_interface::set_viewer_display_name(QString display_nm)
{
    plist_viewer_display_name = display_nm;
}

void plist_viewer_interface::set_db_path(QString path)
{
    destination_db_path = path;
}


void plist_viewer_interface::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}


QString plist_viewer_interface::get_plist_viewer_name()
{
    return QString(MACRO_Plugin_Name_Plist_Viewer);
}


void plist_viewer_interface::set_initialisation()
{
    create_right_clicks();

    get_info_from_index_table();

    display_tree_view();
    manage_plist_key_obj->pub_set_plist_table_name(plist_db_table_name);
    manage_plist_key_obj->pub_set_plist_db_path(destination_db_path);

    report_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Parent_In_Result_QString).toString() + get_plist_viewer_name().replace(" ","_").toLower();
    QDir dir;
    dir.mkdir(report_path);
}

void plist_viewer_interface::create_right_clicks()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    menu_plist = new QMenu(this);

    act_seperator_note = new QAction(this);
    act_seperator_note->setSeparator(true);

    submenu_bookmark = menu_plist->addMenu(tr(MACRO_Generic_Right_Click_Bookmark));
    act_bookmark_all = new QAction(QObject::tr(MACRO_Generic_Right_Click_Bookmark_all), this->submenu_bookmark);
    act_remove_all_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_All_Bookmarks), this->submenu_bookmark);
    submenu_bookmark->addAction(act_bookmark_all);
    submenu_bookmark->addAction(act_remove_all_bookmark);

    submenu_tag = new QMenu(QObject::tr(MACRO_Generic_Right_Click_Tags),this);
    display_tags_submenu();
    menu_plist->addMenu(submenu_tag);

    submenu_note = menu_plist->addMenu(tr(MACRO_Generic_Right_Click_Notes));
    act_add_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Note), this->submenu_note);
    act_remove_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Note), this->submenu_note);
    submenu_note->addAction(act_add_notes);
    submenu_note->addAction(act_remove_notes);
    menu_plist->addAction(act_seperator_note);

    act_add_notes_to_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Add_Notes_to_Bookmarks), this);
    act_remove_notes_from_bookmark = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_Notes_from_Bookmarks), this);
    act_remove_all_notes = new QAction(QObject::tr(MACRO_Generic_Right_Click_Remove_all_Notes), this);
    act_copy_to_clipboard = new QAction(QObject::tr(MACRO_Generic_Right_Click_Copy_Row_To_Clipboard),this);
    act_copy_column_to_clipboard = new QAction(QObject::tr(MACRO_Generic_Right_Click_Copy_Cell_To_Clipboard),this);

    act_media_keys = new QAction(QObject::tr(MACRO_Generic_Right_Click_Manage_Keys),this);
    connect(act_media_keys, SIGNAL(triggered()), this, SLOT(slot_act_manage_keys_triggered()));

    menu_plist->addAction(act_copy_to_clipboard);
    menu_plist->addAction(act_copy_column_to_clipboard);
    menu_plist->addAction(act_media_keys);

    connect(submenu_tag, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked(QAction*)));
    connect(submenu_tag,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    connect(menu_plist, SIGNAL(triggered(QAction*)), this, SLOT(slot_plist_menu_clicked(QAction*)));

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::display_tree_view()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...  ");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    keys_list_of_plist_file.clear();
    values_list_of_plist_file.clear();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + destination_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug( destination_db.lastError().text(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    QSqlQuery select_query(destination_db);

    ui->treeWidget_plist->clear();
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_KEY,410);
    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_TYPE,85);

    QString command = "SELECT MAX(id) FROM '" + plist_db_table_name  + "'";
    QString id_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_path,Q_FUNC_INFO);
    int id_count = id_str.toInt();

    QTreeWidgetItem *parent_root = new QTreeWidgetItem;

    int mcounter = 0;

    ///insert level 1 & level 2 statically
    for(int kk = 0 ; kk <= id_count ; kk++)
    {
        if(kk >= 2)
            break;

        select_query.clear();
        select_query.prepare("SELECT key,type,value,parentpath,id, keypath,parent,INT,bookmark, notes, recon_tag_value FROM '" + plist_db_table_name +"' WHERE id = ?");
        select_query.addBindValue(QString::number(kk));
        if(!select_query.exec())
        {
            recon_static_functions::app_debug("select_query - 2 - execution-----FAILED----" + destination_db_path, Q_FUNC_INFO);
            recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(select_query.executedQuery() , Q_FUNC_INFO);
            continue;
        }

        while(select_query.next())
        {
            mcounter++;
            if(mcounter % 200 == 0)
                QCoreApplication::processEvents();


            QString key = select_query.value(0).toString();
            QString type = select_query.value(1).toString();
            QString value = select_query.value(2).toString();
            QString keypath = select_query.value(5).toString();
            QString intvalue = select_query.value(7).toString();
            QString bookmark = select_query.value(8).toString();
            QString ex_notes = select_query.value(9).toString();
            QString recon_tag_value = select_query.value(10).toString();

            ///Note: This work is for Manage Keys.
            keys_list_of_plist_file << key;
            values_list_of_plist_file << value;

            if(kk == enum_plist_viewer_interface_TREE_STATIC_LEVEL_FIRST)
            {
                parent_root->setText(enum_plist_viewer_interface_TREE_KEY,key);
                parent_root->setText(enum_plist_viewer_interface_TREE_TYPE,type);
                parent_root->setText(enum_plist_viewer_interface_TREE_VALUE, value);
                parent_root->setText(enum_plist_viewer_interface_TREE_KEYPATH, keypath);
                parent_root->setText(enum_plist_viewer_interface_TREE_INT, intvalue);
                parent_root->setFlags(parent_root->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);

                if(bookmark == "1")
                    parent_root->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Checked);
                else
                    parent_root->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Unchecked);

                ex_notes = ex_notes.trimmed();

                if(!ex_notes.isEmpty())
                {
                    QIcon icon("../icons/notes.png");
                    parent_root->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
                }
                else
                {
                    QIcon icon("");
                    parent_root->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
                }

                parent_root->setText(enum_plist_viewer_interface_TREE_NOTES, ex_notes);
                parent_root->setText(enum_plist_viewer_interface_TREE_TAG_NAMES, recon_tag_value);

                if(!recon_tag_value.isEmpty())
                {
                    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("SELECT selected_colour FROM tags Where tag_name = ?",QStringList(recon_tag_value) ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);
                    parent_root->setForeground(enum_plist_viewer_interface_TREE_KEY,QColor(colour));

                }

                ui->treeWidget_plist->addTopLevelItem(parent_root);
            }

            if(kk == enum_plist_viewer_interface_TREE_STATIC_LEVEL_SECOND)
            {
                QTreeWidgetItem *childA = new QTreeWidgetItem;
                childA->setText(enum_plist_viewer_interface_TREE_KEY,key);
                childA->setText(enum_plist_viewer_interface_TREE_TYPE,type);
                childA->setText(enum_plist_viewer_interface_TREE_VALUE, value);
                childA->setText(enum_plist_viewer_interface_TREE_KEYPATH, keypath);
                childA->setText(enum_plist_viewer_interface_TREE_INT, intvalue);
                childA->setFlags(childA->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);

                if(bookmark == "1")
                    childA->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Checked);
                else
                    childA->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Unchecked);

                ex_notes = ex_notes.trimmed();

                if(!ex_notes.isEmpty())
                {
                    QIcon icon("../icons/notes.png");
                    childA->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
                }
                else
                {
                    QIcon icon("");
                    childA->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
                }

                childA->setText(enum_plist_viewer_interface_TREE_NOTES, ex_notes);
                childA->setText(enum_plist_viewer_interface_TREE_TAG_NAMES, recon_tag_value);

                if(!recon_tag_value.isEmpty())
                {
                    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("SELECT selected_colour FROM tags Where tag_name = ?",QStringList(recon_tag_value) ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);
                    childA->setForeground(enum_plist_viewer_interface_TREE_KEY,QColor(colour));
                }

                parent_root->addChild(childA);
            }
        }
    }

    ///insert items dynamically in tree
    for(int ll = 2 ; ll <= id_count ; ll++)
    {
        select_query.clear();
        select_query.prepare("SELECT key,type,value,parentpath,id, keypath,parent,INT,bookmark, notes, recon_tag_value FROM '" + plist_db_table_name + "' WHERE id = ?");
        select_query.addBindValue(QString::number(ll));
        if(!select_query.exec())
        {
            recon_static_functions::app_debug("select_query - 3 - execution-----FAILED----"+ destination_db_path , Q_FUNC_INFO);
            recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(select_query.executedQuery() , Q_FUNC_INFO);
            continue;
        }

        while(select_query.next())
        {
            mcounter++;
            if(mcounter % 200 == 0)
                QCoreApplication::processEvents();


            QString key = select_query.value(0).toString();
            QString type = select_query.value(1).toString();
            QString value = select_query.value(2).toString();
            QString parentname = select_query.value(3).toString();
            QString keypath = select_query.value(5).toString();
            QString intvalue = select_query.value(7).toString();
            QString bookmark = select_query.value(8).toString();
            QString ex_notes = select_query.value(9).toString();
            QString recon_tag_value = select_query.value(10).toString();

            ///Note: This work is for Manage Keys.
            keys_list_of_plist_file << key;
            values_list_of_plist_file << value;

            ///iterate complete tree : so that we can add adjacent child in tree
            QTreeWidgetItem *item =  get_currentptr_on_tree_iteration(parentname);

            QTreeWidgetItem *childA = new QTreeWidgetItem;
            childA->setText(enum_plist_viewer_interface_TREE_KEY,key);
            childA->setText(enum_plist_viewer_interface_TREE_TYPE,type);
            childA->setText(enum_plist_viewer_interface_TREE_VALUE, value);
            childA->setText(enum_plist_viewer_interface_TREE_KEYPATH, keypath);
            childA->setText(enum_plist_viewer_interface_TREE_INT, intvalue);
            childA->setFlags(childA->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);

            if(bookmark == "1")
            {
                childA->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Checked);
            }
            else
            {
                childA->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Unchecked);
            }

            ex_notes = ex_notes.trimmed();
            if(!ex_notes.isEmpty())
            {
                QIcon icon("../icons/notes.png");
                childA->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
            }
            else
            {
                QIcon icon("");
                childA->setIcon(enum_plist_viewer_interface_TREE_NOTE_ICON,icon);
            }

            childA->setText(enum_plist_viewer_interface_TREE_NOTES, ex_notes);
            childA->setText(enum_plist_viewer_interface_TREE_TAG_NAMES, recon_tag_value);

            if(!recon_tag_value.isEmpty())
            {
                QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("SELECT selected_colour FROM tags Where tag_name = ?",QStringList(recon_tag_value) ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);
                childA->setForeground(enum_plist_viewer_interface_TREE_KEY,QColor(colour));
            }

            item->addChild(childA);
        }
    }

    ///Expand only first level..

    keys_list_of_plist_file.removeDuplicates();

    ui->treeWidget_plist->setColumnWidth(enum_plist_viewer_interface_TREE_TAG,30*id_count);
    ui->treeWidget_plist->hideColumn(enum_plist_viewer_interface_TREE_NOTES);

    ui->treeWidget_plist->expandItem(ui->treeWidget_plist->topLevelItem(enum_plist_viewer_interface_TREE_STATIC_LEVEL_FIRST));

    destination_db.close();

    display_loading_progress_bar_non_cancelable_obj->hide();
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

QTreeWidgetItem *plist_viewer_interface::get_currentptr_on_tree_iteration(QString parentpath)
{
    ///iterate complete tree here...
    QTreeWidgetItem *child = new  QTreeWidgetItem;
    QTreeWidgetItemIterator iter_tree(ui->treeWidget_plist);

    while (*iter_tree)
    {
        if ((*iter_tree)->text(enum_plist_viewer_interface_TREE_KEYPATH) == parentpath)
        {
            child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
            break;
        }

        ++iter_tree;
    }

    return child;
}

void plist_viewer_interface::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Alt )
    {
        bool_alt_keypress = true;
    }
}

void plist_viewer_interface::treeitem_expand_recursively(QTreeWidgetItem *item)
{
    ui->treeWidget_plist->expandItem(item);

    for (int i = 0; i < item->childCount(); ++i)
    {
        treeitem_expand_recursively(item->child(i));
    }
}

void plist_viewer_interface::get_info_from_index_table()
{
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

    QString command = "SELECT db_table_name,file_path , INT , source_count_name , recon_filefrom , recon_file_infopath FROM 'plist_viewer_index' WHERE viewer_display_name = '" + plist_viewer_display_name + "'";
    QSqlQuery query_index(destination_db);
    if(!query_index.exec(command))
    {
        recon_static_functions::app_debug("select_query execution-----FAILED----"+ destination_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_index.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + query_index.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(query_index.next())
    {
        plist_db_table_name = query_index.value("db_table_name").toString();
        plist_file_path = query_index.value("file_path").toString();

        QString record_no = query_index.value("INT").toString();
        QString source_count_name = query_index.value("source_count_name").toString();
        complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,"plist_viewer_index",source_count_name,Q_FUNC_INFO);
        recon_file_from_str = query_index.value("recon_filefrom").toString();
        recon_file_info_path_str = query_index.value("recon_file_infopath").toString();
    }

    destination_db.close();
}

void plist_viewer_interface::on_pushButton_report_clicked()
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    QDateTime time;
    QString current_timestr = time.currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
    QString Report_scope;
    if(ui->comboBox_report_scope->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_TAGS)
    {
        Report_scope = MACRO_REPORT_SCOPE_TAGS_QString;
    }
    else if(ui->comboBox_report_scope->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_FULL)
    {
        Report_scope = MACRO_REPORT_SCOPE_FULL_QString;
    }

    if(ui->comboBox_report_type->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_XML)
    {
        emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

        QString file_name =  get_plist_viewer_name().replace(" ","_") + "_" + Report_scope + "_" + current_timestr;
        QString filepath = report_path + "/" + file_name;
        QDir dir;
        dir.mkpath(filepath);

        QString destfilepath = filepath + "/" + file_name + ".xml";
        QFile file(destfilepath);
        if(!file.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("file open-----FAILED----" + destfilepath, Q_FUNC_INFO);
            recon_static_functions::app_debug(file.errorString() , Q_FUNC_INFO);
            return;
        }

        QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartElement(QObject::tr("Plist_Viewer_Detail"));
        global_recon_helper_singular_class_obj->write_xml_report_case_info_and_source_info(&stream,Report_scope, global_narad_muni_class_obj);
        stream.writeTextElement(QObject::tr("File_Path"),plist_file_path);

        for (int i = 0; i < ui->treeWidget_plist->topLevelItemCount(); ++i)
        {
            if(ui->comboBox_report_scope->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_TAGS)
                write_Index_Entry_bookmarks(&stream, ui->treeWidget_plist->topLevelItem(i));
            else
                if(ui->comboBox_report_scope->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_FULL)
                    write_Index_Entry_full(&stream, ui->treeWidget_plist->topLevelItem(i));
        }

        stream.writeEndElement();
        file.close();

        emit signal_statusbar_progress_bar_show_hide(false, "");

        report_status obj;
        if(obj.exec())
        {
            QStringList arg;
            arg << destfilepath;

            QProcess *myProcess = new QProcess(this);
            myProcess->startDetached("open",arg);
        }
    }

    if(ui->comboBox_report_type->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_HTML)
    {
        create_html_reports();
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::write_Index_Entry_bookmarks(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item)
{
    if(item->checkState(enum_plist_viewer_interface_TREE_KEY) == 2 || !item->text(enum_plist_viewer_interface_TREE_TAG_NAMES).trimmed().isEmpty())
    {
        xmlWriter->writeStartElement(item->text(enum_plist_viewer_interface_TREE_KEY).replace(" ", "_"));
        xmlWriter->writeTextElement("Type", item->text(enum_plist_viewer_interface_TREE_TYPE));
        xmlWriter->writeTextElement("Value", item->text(enum_plist_viewer_interface_TREE_VALUE));

        QString converted_data_str;
        QString key_at_item = item->text(enum_plist_viewer_interface_TREE_KEY);
        QString value_at_item = item->text(enum_plist_viewer_interface_TREE_VALUE);
        if(keys_list_of_plist_file.contains(key_at_item))
        {
            converted_data_str = converted_data(value_at_item, key_at_item);

            //Value is not convertable.
            if(converted_data_str != "-1")
            {
                xmlWriter->writeTextElement("Value_Converted", converted_data_str);
            }

        }
        xmlWriter->writeTextElement("Tag", item->text(enum_plist_viewer_interface_TREE_TAG_NAMES));
        xmlWriter->writeTextElement("Examiner_Notes", item->text(enum_plist_viewer_interface_TREE_NOTES));
    }

    for (int i = 0; i < item->childCount(); ++i)
    {
        write_Index_Entry_bookmarks(xmlWriter, item->child(i));
    }

    if(item->checkState(enum_plist_viewer_interface_TREE_KEY) == 2 || !item->text(enum_plist_viewer_interface_TREE_TAG_NAMES).trimmed().isEmpty())
    {
        xmlWriter->writeEndElement();
    }
}

void plist_viewer_interface::write_Index_Entry_full(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item)
{
    xmlWriter->writeStartElement(item->text(enum_plist_viewer_interface_TREE_KEY).replace(" ", "_"));
    xmlWriter->writeTextElement("Type", item->text(enum_plist_viewer_interface_TREE_TYPE));
    xmlWriter->writeTextElement("Value", item->text(enum_plist_viewer_interface_TREE_VALUE));

    QString converted_data_str;
    QString key_at_item = item->text(enum_plist_viewer_interface_TREE_KEY);
    QString value_at_item = item->text(enum_plist_viewer_interface_TREE_VALUE);
    if(keys_list_of_plist_file.contains(key_at_item))
    {
        converted_data_str = converted_data(value_at_item, key_at_item);

        //Value is not convertable.
        if(converted_data_str != "-1")
        {
            xmlWriter->writeTextElement("Value_Converted", converted_data_str);
        }

    }

    xmlWriter->writeTextElement("Tag", item->text(enum_plist_viewer_interface_TREE_TAG_NAMES));
    xmlWriter->writeTextElement("Examiner_Notes", item->text(enum_plist_viewer_interface_TREE_NOTES));

    for (int i = 0; i < item->childCount(); ++i)
    {
        write_Index_Entry_full(xmlWriter, item->child(i));
    }

    xmlWriter->writeEndElement();
}


void plist_viewer_interface::on_treeWidget_plist_itemClicked(QTreeWidgetItem *item, int column)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(item == NULL)
        return;


    fetch_metadata_for_plist_viewer(item);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void plist_viewer_interface::on_treeWidget_plist_itemChanged(QTreeWidgetItem *item, int column)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    ui->treeWidget_plist->blockSignals(true);


    ui->treeWidget_plist->selectionModel()->clearSelection();

    item->setSelected(true);
    ui->pushButton_refresh_treewidget_data->setEnabled(false);

    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...  ");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    if(item->checkState(enum_plist_viewer_interface_TREE_KEY) == Qt::Checked)
    {
        emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Plist_Viewer));
        QTreeWidgetItem *child = new QTreeWidgetItem;
        QTreeWidgetItemIterator it(ui->treeWidget_plist);

        while(*it)
        {
            if(((*it)->text(enum_plist_viewer_interface_TREE_KEYPATH) == item->text(enum_plist_viewer_interface_TREE_KEYPATH)) || ((*it)->text(enum_plist_viewer_interface_TREE_KEYPATH).startsWith(item->text(enum_plist_viewer_interface_TREE_KEYPATH) +"/")))
            {
                child = dynamic_cast<QTreeWidgetItem *>(*it);
                child->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Checked);

                QString inttext = child->text(enum_plist_viewer_interface_TREE_INT);
                QString tag_name  = child->text(enum_plist_viewer_interface_TREE_TAG_NAMES);
                QString notes_text  = child->text(enum_plist_viewer_interface_TREE_NOTES);

                create_table_export_content();

                QStringList values;
                values << plist_db_table_name;
                QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_count_name from plist_viewer_index where db_table_name = ?", values, 0, destination_db_path, Q_FUNC_INFO);


                QList<struct_global_tagging_plist_viewer_record> plist_record_list;

                struct_global_tagging_plist_viewer_record obj;
                obj.INT_record  = child->text(enum_plist_viewer_interface_TREE_INT);
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
                global_connection_manager_obj->pub_add_data_in_export_content_plist_viewer_db(plist_record_list,tag_name , notes_text);
                global_connection_manager_obj->pub_add_bookmark_for_plist_viewer(plist_record_list);

            }

            ++it;
        }
    }
    else
    {
        QTreeWidgetItem *child = new  QTreeWidgetItem;
        QTreeWidgetItemIterator it(ui->treeWidget_plist);
        while(*it)
        {
            if(((*it)->text(enum_plist_viewer_interface_TREE_KEYPATH) == item->text(enum_plist_viewer_interface_TREE_KEYPATH)) || ((*it)->text(enum_plist_viewer_interface_TREE_KEYPATH).startsWith(item->text(enum_plist_viewer_interface_TREE_KEYPATH) + "/")))
            {
                child = dynamic_cast<QTreeWidgetItem *>(*it);
                child->setCheckState(enum_plist_viewer_interface_TREE_KEY,Qt::Unchecked);

                QString inttext = child->text(enum_plist_viewer_interface_TREE_INT);
                QString tag_name  = child->text(enum_plist_viewer_interface_TREE_TAG_NAMES);
                QString notes_text  = child->text(enum_plist_viewer_interface_TREE_NOTES);

                global_connection_manager_obj->pub_remove_bookmark_in_plist_viewer_db(QStringList(inttext) ,plist_db_table_name);
                global_connection_manager_obj->pub_delete_bookmark_for_plist_viewer(QStringList(inttext),plist_viewer_display_name);

                if(tag_name.trimmed().isEmpty() && notes_text.trimmed().isEmpty())
                    global_connection_manager_obj->pub_delete_entry_export_content_plist_viewer_db(QStringList(inttext) ,plist_viewer_display_name);
                else
                    global_connection_manager_obj->pub_update_bookmark_export_content_plist_viewer_db(QStringList(inttext) ,plist_viewer_display_name);

            }

            ++it;
        }
    }

    display_loading_progress_bar_non_cancelable_obj->hide();

    ui->pushButton_refresh_treewidget_data->setEnabled(true);

    ui->treeWidget_plist->blockSignals(false);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::on_lineEdit_search_textChanged(const QString &arg1)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    return;
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);


    // We are not using this slot for now.

    if(arg1.trimmed().isEmpty())
    {
        ui->label_search->clear();
        QTreeWidgetItemIterator it_search_empty(ui->treeWidget_plist);
        while (*it_search_empty)
        {
            for(int j = 0; j < enum_plist_viewer_interface_TREE_TAG_NAMES ; j++)
            {
                (*it_search_empty)->setBackground(j,  Qt::transparent);
            }

            ++it_search_empty;
        }

        return;
    }

    QString search_text = arg1;
    int count = 0;

    int length = search_text.length();
    search_text = search_text.toLower();

    QTreeWidgetItemIterator iter_keyword(ui->treeWidget_plist);

    while(*iter_keyword)
    {
        for(int j = 0; j < enum_plist_viewer_interface_TREE_TAG_NAMES ; j++)
        {
            if((j != enum_plist_viewer_interface_TREE_KEY) && (j != enum_plist_viewer_interface_TREE_TYPE) && (j != enum_plist_viewer_interface_TREE_VALUE) && (j != enum_plist_viewer_interface_TREE_NOTES))
                continue;

            (*iter_keyword)->setBackground(j,  Qt::transparent);
            if((*iter_keyword)->text(j).toLower().contains(search_text))
            {
                QString item_str = (*iter_keyword)->text(j).toLower();

                int index = 0;
                while (index >= 0)
                {
                    index = item_str.indexOf(search_text, index);
                    if(index < 0)
                    {
                        break;
                    }

                    index = index + length;
                    count++;

                    (*iter_keyword)->setBackground(j, QColor("#D59B3F"));
                }
            }
        }

        ++iter_keyword;
    }

    ui->label_search->setText("Match Found: " + QString::number(count));

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::create_html_reports()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + destination_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QDateTime time;
    QString current_timestr = time.currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");

    QString Report_scope = "";
    QSqlQuery select_query(destination_db);
    if(ui->comboBox_report_scope->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_TAGS)
    {
        select_query.prepare("select key,type,value,keypath, INT,notes, recon_tag_value , bookmark from '" + plist_db_table_name +"' where bookmark = 1 OR recon_tag_value != ''");
        Report_scope = MACRO_REPORT_SCOPE_TAGS_QString;
    }
    else if(ui->comboBox_report_scope->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_FULL)
    {
        select_query.prepare("select key,type,value,keypath, INT,notes,  recon_tag_value from '" + plist_db_table_name + "'");
        Report_scope = MACRO_REPORT_SCOPE_FULL_QString;
    }

    if(!select_query.exec())
    {
        recon_static_functions::app_debug("select_query execution-----FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Executed Query " + select_query.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QString file_name =  get_plist_viewer_name().replace(" ","_") + "_" + Report_scope + "_" + current_timestr;
    QString filepath = report_path + "/" + file_name;
    QDir dir;
    dir.mkpath(filepath);

    if(ui->comboBox_report_type->currentIndex() == enum_plist_viewer_interface_TREE_REPORT_HTML)
    {
        emit signal_statusbar_progress_bar_show_hide(true, "Please wait...");

        QString destfilepath = filepath + "/" + file_name + ".html";
        QString resource_dir = filepath + "/resources";

        QString agency_logo_path = resource_dir + "/agency_logo.png";
        QString icon_path = resource_dir + "/plist_viewer.png";


        QDir dir1;
        dir1.mkpath(resource_dir);
        recon_static_functions::copy_directory("../Resources/graphics/html_reports", resource_dir, Q_FUNC_INFO);
        QFile logo_file(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString());
        logo_file.copy(agency_logo_path);
        QFile icon_file("../icons/Features/plist_viewer.png");
        icon_file.copy(icon_path);


        QFile file(destfilepath);
        if(!file.open(QIODevice::WriteOnly))
        {
            recon_static_functions::app_debug("file open-----FAILED----" + destfilepath , Q_FUNC_INFO);
            recon_static_functions::app_debug( "-------Error-----" + file.errorString() , Q_FUNC_INFO);
            destination_db.close();
            return;
        }

        file.write(plist_viewer_report_html_template::html_header1().toLocal8Bit().data());

        QString title =  get_plist_viewer_name();
        file.write(plist_viewer_report_html_template::html_title(title).toLocal8Bit().data());
        file.write(plist_viewer_report_html_template::html_header_after_title().toLocal8Bit().data());

        file.write(plist_viewer_report_html_template::html_sidebar_start().toLocal8Bit().data());

        file.write(plist_viewer_report_html_template::html_sidebar_link("Items").toLocal8Bit().data());
        file.write(plist_viewer_report_html_template::html_sidebar_end().toLocal8Bit().data());

        file.write(plist_viewer_report_html_template::html_report_main_title(agency_logo_path,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString()).toLocal8Bit().data());
        file.write(global_recon_helper_singular_class_obj->write_html_case_info_and_source_info(Report_scope, global_narad_muni_class_obj).toLocal8Bit().data());

        file.write(plist_viewer_report_html_template::html_table_start().toLocal8Bit().data());
        file.write(plist_viewer_report_html_template::html_module_header(icon_path,Report_scope, get_plist_viewer_name()).toLocal8Bit().data());

        QStringList headers;
        headers << "Sr. No" << "Key" << "Type" << "Value" << "Value(Converted)" << "Key Path" << "Tag" << "Examiner Notes";

        int count = 1;
        QString tabledata;
        while(select_query.next())
        {
            tabledata +=  QString("<td><div style=\"word-wrap: break-word; width: 100px\">" + QString::number(count) + "</div></td>");
            tabledata +=  QString("<td><div style=\"word-wrap: break-word; width: 150px\">" + select_query.value(0).toString() + "</div></td>");
            tabledata +=  QString("<td><div style=\"word-wrap: break-word; width: 100px\">" + select_query.value(1).toString() + "</div></td>");

            QString converted_data_str;
            QString key_at_item = select_query.value("key").toString();
            QString value_at_item = select_query.value("value").toString();
            if(keys_list_of_plist_file.contains(key_at_item))
            {
                converted_data_str = converted_data(value_at_item, key_at_item);

                //Value is not convertable.
                if(converted_data_str == QString("-1"))
                {
                    converted_data_str.clear();
                }

            }

            tabledata +=  QString("<td><div style=\"word-wrap: break-word; width: 120px\">" + select_query.value(2).toString() + "</div></td>");
            tabledata +=  QString("<td><div style=\"word-wrap: break-word; width: 150px\">" + converted_data_str + "</div></td>");
            tabledata +=  QString("<td><div style=\"word-wrap: break-word; width: 120px\">" + select_query.value(3).toString() + "</div></td>");


            QString tag_text = "";
            QStringList list_tags = select_query.value(6).toString().split(",", Qt::SkipEmptyParts);
            for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
            {
                QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
            }

            if(tag_text.endsWith(","))
                tag_text.chop(1);


            if(select_query.value("bookmark").toString() == QString("1"))
            {
                if(!tag_text.trimmed().isEmpty())
                    tag_text.append(",").append(MACRO_Tag_Name_Bookmarks);
                else
                    tag_text.append(MACRO_Tag_Name_Bookmarks);
            }

            tabledata +=  QString("<td>" + tag_text + "</td>");
            tabledata +=  QString("<td><font  color=\"blue\">" + select_query.value(5).toString() + "</font></td>");

            tabledata += QString("<tr>");

            count++;
        }

        if(tabledata == "")
        {
            QString no_data_available = "<b><font color=\"red\">" +  QObject::tr("No Data Available") + "</font></b>";
            file.write(no_data_available.toLocal8Bit());
        }
        else
        {
            file.write(plist_viewer_report_html_template::html_sub_module_header("Items").toLocal8Bit().data());
            file.write(plist_viewer_report_html_template::html_top_link().toLocal8Bit().data());


            QString filepath = "<b>File Path:</b> " + plist_file_path;
            file.write(filepath.toLocal8Bit().data());
            file.write(plist_viewer_report_html_template::html_table_headers(headers,headers.size()).toLocal8Bit().data());

            if(bool_for_the_unicode_conversion)
            {
                QByteArray arr_data = tabledata.toLocal8Bit();
                file.write(arr_data);
            }
            else
            {
                file.write(tabledata.toLocal8Bit().data());
            }
        }

        file.write("<br>");
        file.write("<br>");

        file.write(plist_viewer_report_html_template::html_table_end().toLocal8Bit().data());
        file.close();

        emit signal_statusbar_progress_bar_show_hide(false, "");

        report_status obj;
        if(obj.exec())
        {
            QStringList arg;
            arg << destfilepath;

            QProcess *myProcess = new QProcess(this);
            myProcess->startDetached("open",arg);
        }
    }

    destination_db.close();
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::on_treeWidget_plist_itemExpanded(QTreeWidgetItem *item)
{
    if(bool_alt_keypress)
    {
        treeitem_expand_recursively(item);
        bool_alt_keypress = false;
    }
}

void plist_viewer_interface::fetch_metadata_for_plist_viewer(QTreeWidgetItem *item)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString metadata_str;

    QStringList values;

    values << plist_db_table_name;
    QString source_count = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select source_count_name from plist_viewer_index where db_table_name = ?", values, 0, destination_db_path, Q_FUNC_INFO);
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count);

    QString complete_source_name = struct_info.complete_source_name;
    metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name +"<br>";

    if(item->text(enum_plist_viewer_interface_TREE_KEY) == "Root")
    {
        if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +QString("\t") + struct_info.source_name +"<br>";

        }
        metadata_str += "<br>";
    }

    metadata_str += "<b>" + QObject::tr("Viewer Display Name") + ":</b>" +QString("\t") + plist_viewer_display_name +"<br><br>";

    metadata_str += "<b>" + QObject::tr("Key") + ":</b>" +QString("\t") + item->text(enum_plist_viewer_interface_TREE_KEY) +"<br>";
    metadata_str += "<b>" + QObject::tr("Type") + ":</b>" +QString("\t") + item->text(enum_plist_viewer_interface_TREE_TYPE) +"<br>";
    metadata_str += "<b>" + QObject::tr("Value") + ":</b>" +QString("\t") + item->text(enum_plist_viewer_interface_TREE_VALUE) +"<br>";

    QString converted_data_str;
    QString key_at_selected_item = item->text(enum_plist_viewer_interface_TREE_KEY);
    QString value_at_selected_item = item->text(enum_plist_viewer_interface_TREE_VALUE);

    for(int kk = 0; kk < keys_list_of_plist_file.size(); kk++)
    {
        QString date_key = keys_list_of_plist_file.at(kk);

        if(key_at_selected_item == date_key)
        {
            converted_data_str = converted_data(value_at_selected_item, key_at_selected_item);

            //Value is not convertable.
            if(converted_data_str == QString("-1"))
            {
                continue;
            }

            metadata_str += "<b>" + QObject::tr("Value") + QObject::tr("(Converted)") + ":</b>" + QString("\t") + converted_data_str + QString("\t") + "<br><br>";
        }
    }

    metadata_str += "<b>" + QObject::tr("Key Path") + ":</b>" +QString("\t") + item->text(enum_plist_viewer_interface_TREE_KEYPATH) +"<br><br>";

    QFileInfo file(plist_file_path);

    metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + file.fileName() +"<br>";
    metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + plist_file_path +"<br><br>";

    QString tag_text;
    QStringList list_tags = item->text(enum_plist_viewer_interface_TREE_TAG_NAMES).split(",");

    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
    {
        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
    }

    if(tag_text.endsWith(","))
        tag_text.chop(1);

    metadata_str += QString("\n<b>Tag:</b>\t") + tag_text + "\n<br> <br>";
    metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + item->text(enum_plist_viewer_interface_TREE_NOTES) +"<br>" + "</font>\n";

    emit signal_plist_item_clicked("", metadata_str, "", "");

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::slot_abouttoshow_tags_in_submenu()
{
    display_tags_submenu();
}

void plist_viewer_interface::on_pushButton_search_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    ui->treeWidget_plist->blockSignals(true);

    QString arg1 = ui->lineEdit_search->text();

    if(arg1.trimmed().isEmpty())
    {
        ui->label_search->clear();
        QTreeWidgetItemIterator it_search_empty(ui->treeWidget_plist);


        while (*it_search_empty)
        {

            for(int j = 0; j < enum_plist_viewer_interface_TREE_TAG_NAMES ; j++)
            {
                (*it_search_empty)->setBackground(j,  Qt::transparent);
            }

            ++it_search_empty;
        }

        recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

        ui->treeWidget_plist->blockSignals(false);

        return;
    }

    QString search_text = arg1;
    int count = 0;

    int length = search_text.length();
    search_text = search_text.toLower();

    QTreeWidgetItemIterator iter_keyword(ui->treeWidget_plist);

    while(*iter_keyword)
    {


        for(int j = 0; j < enum_plist_viewer_interface_TREE_TAG_NAMES ; j++)
        {
            if((j != enum_plist_viewer_interface_TREE_KEY) && (j != enum_plist_viewer_interface_TREE_TYPE) && (j != enum_plist_viewer_interface_TREE_VALUE) && (j != enum_plist_viewer_interface_TREE_NOTES))
                continue;


            (*iter_keyword)->setBackground(j,  Qt::transparent);


            if((*iter_keyword)->text(j).toLower().contains(search_text))
            {

                QString item_str = (*iter_keyword)->text(j).toLower();
                int index = 0;
                while (index >= 0)
                {
                    index = item_str.indexOf(search_text, index);
                    if(index < 0)
                    {
                        break;
                    }

                    index = index + length;
                    count++;


                    (*iter_keyword)->setBackground(j, QColor("#D59B3F"));

                }

            }
        }


        ++iter_keyword;

    }

    ui->label_search->setText("Match Found: " + QString::number(count));

    ui->treeWidget_plist->blockSignals(false);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void plist_viewer_interface::on_pushButton_refresh_treewidget_data_clicked()
{
    emit signal_plist_item_clicked("", "", "", "");
    ui->lineEdit_search->clear();
    ui->label_search->clear();
    display_tree_view();
}

void plist_viewer_interface::create_table_export_content()
{

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'plist_content_info'(INT INTEGER PRIMARY KEY,record_no VARCHAR(1000),bookmark,key VARCHAR(1000),value VARCHAR(1000),type VARCHAR(1000),keypath VARCHAR(1000), file_name VARCHAR(1000), file_path VARCHAR(1000),"
                          "recon_filefrom VARCHAR(1000), recon_file_infopath VARCHAR(1000), file_size VARCHAR(1000), recon_tag_value VARCHAR(1000),"
                          "color_hex VARCHAR(1000), source_count_name VARCHAR(1000), viewer_display_name VARCHAR(1000), notes VARCHAR(1000), LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000))";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table plist_content_info in plist viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }

        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);
    }

}

QString plist_viewer_interface::converted_data(QString value_str, QString current_key)
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    bool_for_the_unicode_conversion = false;

    value_str = value_str.trimmed();

    QString data_conversion_type = get_data_conversion_type_of_current_selected_key(current_key);

    if(data_conversion_type == QString("Unix Timestamp"))
    {
        value_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(value_str, Q_FUNC_INFO);
    }
    else if(data_conversion_type == QString("Cocoa Core Data Timestamp"))
    {
        value_str = global_recon_helper_singular_class_obj->convert_safari_time_to_human_readable(value_str, Q_FUNC_INFO);
    }
    else if(data_conversion_type == QString("WebKit/Chrome Timestamp"))
    {
        value_str = global_recon_helper_singular_class_obj->convert_webkit_chrome_time_to_human_readable(value_str, Q_FUNC_INFO);
    }
    else if(data_conversion_type == QString("ASCII"))
    {
        QByteArray value_data_array = value_str.toLocal8Bit();
        value_str = global_recon_helper_singular_class_obj->convert_raw_data_to_ascii_convertor(value_data_array, Q_FUNC_INFO);
    }
    else if(data_conversion_type == QString("Unicode"))
    {
        QByteArray value_data_array = value_str.toLocal8Bit();
        value_str = global_recon_helper_singular_class_obj->convert_raw_data_to_unicode_convertor(value_data_array, Q_FUNC_INFO);
        bool_for_the_unicode_conversion = true;
    }
    else
    {
        return QString("-1");
    }

    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);

    return value_str;

}

QString plist_viewer_interface::get_data_conversion_type_of_current_selected_key(QString current_key)
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    QString command_str = "SELECT data_conversion_type FROM '" + plist_db_table_name + "' Where key = '" + current_key + "'";

    QString data_conversion_type = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str,0,destination_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);

    return data_conversion_type;

}
