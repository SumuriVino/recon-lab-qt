#include "registry_interface.h"
#include "ui_registry_interface.h"

registry_interface::registry_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registry_interface)
{
    ui->setupUi(this);

    M_VAR_CHILD_ENTRY_QString_TEMP_in_tree_QString = "TEMP_CHILD_ENRTY";


    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->treeWidget_registry->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->tableWidget_registry->setAttribute(Qt::WA_MacShowFocusRect, false);

    /////-----treewidget
    ui->treeWidget_registry->setColumnWidth(enum_tree_widget_INT_Index,30);
    ui->treeWidget_registry->setColumnWidth(enum_tree_widget_Bookmark_Index,30);
    ui->treeWidget_registry->setColumnWidth(enum_tree_widget_Tag_Icon_Index,30);
    ui->treeWidget_registry->setColumnWidth(enum_tree_widget_Notes_Icon_Index,30);
    ui->treeWidget_registry->setColumnWidth(enum_tree_widget_Item_Name_Index,250);
    ui->treeWidget_registry->setColumnWidth(enum_tree_widget_Timestamp_Index,100);



    ui->treeWidget_registry->hideColumn(enum_tree_widget_Parent_Path_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_INT_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_Bookmark_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_Tag_Icon_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_Tag_Text_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_Notes_Icon_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_note_Text_Index);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_Blank_Column_Index);

    ui->treeWidget_registry->hideColumn(enum_tree_widget_source_count_name);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_file_path);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_node_path);
    ui->treeWidget_registry->hideColumn(enum_tree_widget_node_name);



    /////-----tablewidget
    ui->tableWidget_registry->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_registry->setGridStyle(Qt::NoPen);
    ui->tableWidget_registry->setShowGrid(false);
    ui->tableWidget_registry->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_registry->setAutoScroll(true);
    ui->tableWidget_registry->installEventFilter(this);
    ui->tableWidget_registry->verticalHeader()->setHighlightSections(false);
    ui->tableWidget_registry->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_registry->horizontalHeader()->stretchLastSection();
    ui->tableWidget_registry->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_registry->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_registry->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget_registry->horizontalHeader()->setSectionsMovable(true);



    ui->tableWidget_registry->setColumnWidth(enum_table_widget_INT_Index,30);
    ui->tableWidget_registry->setColumnWidth(enum_table_widget_Bookmark_Index,30);
    ui->tableWidget_registry->setColumnWidth(enum_table_widget_Tag_Icon_Index,30);
    ui->tableWidget_registry->setColumnWidth(enum_table_widget_Notes_Icon_Index,30);
    ui->tableWidget_registry->setColumnWidth(enum_table_widget_Key_Index,200);
    ui->tableWidget_registry->setColumnWidth(enum_table_widget_Type_Index,100);
    ui->tableWidget_registry->setColumnWidth(enum_table_widget_Values_Index,100);



    ui->tableWidget_registry->hideColumn(enum_table_widget_INT_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_Tag_Text_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_note_Text_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_Blank_Column_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_source_count_name_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_file_path_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_node_name_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_node_path_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_node_timestamp_Index);
    ui->tableWidget_registry->hideColumn(enum_table_widget_Hex_Values_Index);


    /// searched tabs tablewidget items
    ui->tableWidget_searched_items->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_searched_text->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_INT_Index,30);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Bookmark_Index,30);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Tag_Icon_Index,30);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Notes_Icon_Index,30);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Key_Index,200);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Type_Index,100);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Values_Index,100);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_node_name_Index,300);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_node_timestamp_Index,200);
    ui->tableWidget_searched_items->setColumnWidth(enum_table_widget_Hex_Values_Index,200);


    ui->tableWidget_searched_items->hideColumn(enum_table_widget_INT_Index);
    ui->tableWidget_searched_items->hideColumn(enum_table_widget_Tag_Text_Index);
    ui->tableWidget_searched_items->hideColumn(enum_table_widget_note_Text_Index);
    ui->tableWidget_searched_items->hideColumn(enum_table_widget_source_count_name_Index);
    ui->tableWidget_searched_items->hideColumn(enum_table_widget_file_path_Index);
    ui->tableWidget_searched_items->hideColumn(enum_table_widget_node_path_Index);


    ui->tableWidget_searched_items->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_searched_items->setGridStyle(Qt::NoPen);
    ui->tableWidget_searched_items->setShowGrid(false);
    ui->tableWidget_searched_items->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_searched_items->setAutoScroll(true);
    ui->tableWidget_searched_items->installEventFilter(this);
    ui->tableWidget_searched_items->verticalHeader()->setHighlightSections(false);
    ui->tableWidget_searched_items->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_searched_items->horizontalHeader()->stretchLastSection();
    ui->tableWidget_searched_items->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_searched_items->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_searched_items->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget_searched_items->horizontalHeader()->setSectionsMovable(true);
    connect(ui->lineEdit_searched_text,SIGNAL(returnPressed()),this,SLOT(on_pushButton_search_clicked()));

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    plugin_name_str = QString(MACRO_Plugin_Name_Registry_Viewer);
    tab_name_str = QString(MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer);

    bool_sort_ascending = true;
    connect((QObject*)ui->tableWidget_searched_items->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));
    connect((QObject*)ui->tableWidget_registry->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));

}

registry_interface::~registry_interface()
{
    delete ui;
}


void registry_interface::set_viewer_display_name(QString display_nm)
{
    display_tab_name = display_nm;
}

void registry_interface::pub_set_destination_db_path(QString path)
{
    destination_db_path = path;
}


void registry_interface::set_initialisation()
{

}


void registry_interface::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void registry_interface::pub_draw_registry_tree()
{

    ui->tableWidget_registry->setRowCount(0);
    ui->treeWidget_registry->clear();
    ui->tabWidget->setCurrentIndex(enum_all_items_tab_index);

    ///table right clicks
    create_registry_viewer_right_click_menu();


    /// tree widget
    draw_registry_tree();

}



void registry_interface::draw_registry_tree()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QSqlDatabase destination_db;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_path);
    if (!destination_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.databaseName() , Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    ////node name
    QSqlQuery select_node_query(destination_db);
    select_node_query.prepare(QString("Select distinct registry_name from tbl_registry_tree"));
    if(!select_node_query.exec())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_node_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_node_query.executedQuery(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        destination_db.close();
        return;
    }

    QTreeWidgetItem *parent_root = new QTreeWidgetItem;
    if(select_node_query.next())
    {
        parent_root->setText(enum_tree_widget_Item_Name_Index , select_node_query.value("registry_name").toString());
        ui->treeWidget_registry->addTopLevelItem(parent_root);
    }
    ////node name



    initial_level_str = recon_helper_standard_obj->get_string_from_db_by_dbreference("Select sub_parent_path from tbl_registry_tree where INT = 1" ,0 ,destination_db,Q_FUNC_INFO);
    QString command = "Select distinct child_name,sub_parent_path,  parent_path ,node_timestamp ,Is_Parent,node_path , file_path , node_name , source_count_name  from tbl_registry_tree where sub_parent_path = ?";
    QSqlQuery select_query(destination_db);
    select_query.prepare(command);
    select_query.addBindValue(initial_level_str);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.executedQuery(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        destination_db.close();
        return;
    }


    QList<QTreeWidgetItem*> child_items_list;

    int counter = 0;
    while(select_query.next())
    {
        counter++;

        QCoreApplication::processEvents();

        QString child_name = select_query.value("child_name").toString();
        QString IS_Parent = select_query.value("Is_Parent").toString();
        QString node_timestamp_str = select_query.value("node_timestamp").toString();
        node_timestamp_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(node_timestamp_str, Q_FUNC_INFO);

        QTreeWidgetItem *sub_parent = new QTreeWidgetItem;
        sub_parent->setText(enum_tree_widget_Item_Name_Index , child_name);
        sub_parent->setText(enum_tree_widget_Parent_Path_Index , select_query.value("parent_path").toString());
        sub_parent->setText(enum_tree_widget_Timestamp_Index ,node_timestamp_str);
        sub_parent->setText(enum_tree_widget_node_path ,select_query.value("node_path").toString());
        sub_parent->setText(enum_tree_widget_file_path ,select_query.value("file_path").toString());
        sub_parent->setText(enum_tree_widget_node_name ,select_query.value("node_name").toString());
        sub_parent->setText(enum_tree_widget_source_count_name ,select_query.value("source_count_name").toString());


        ///--- done intentionally >>--kept temp entry to make parent
        if(IS_Parent == QString("1"))
        {
            QTreeWidgetItem *temp_child = new QTreeWidgetItem;
            temp_child->setText(enum_tree_widget_Item_Name_Index , M_VAR_CHILD_ENTRY_QString_TEMP_in_tree_QString);
            sub_parent->addChild(temp_child);
        }

        child_items_list << sub_parent;

    }
    destination_db.close();


    parent_root->addChildren(child_items_list);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

}

QString registry_interface::get_node_name(QString recevied_path)
{
    QString temp_str = initial_level_str;
    QString node_name = recevied_path;
    QStringList temp_list = node_name.split(temp_str);
    node_name = temp_list.join("");
    return node_name;
}

void registry_interface::create_registry_viewer_right_click_menu()
{
    ///--------creation of right click menu list
    mainMenu = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_registry_viewer_menu_right_clicked(QAction*)));

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);
    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark),mainMenu,submenu_bookmark);
    create_right_click_actions(submenu_bookmark, menuList);

    add_tags_module_obj = new add_tags(this);
    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_submenu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    sub_menu_tags = new QMenu(this);
    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);
    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    connect(this, SIGNAL(signal_add_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    create_tags_submenu();

    //Notes
    menuList.clear();
    menuList   << QString(MACRO_Generic_Right_Click_Add_Note)
               << QString(MACRO_Generic_Right_Click_Remove_Note);

    submenu_note = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Notes),mainMenu,submenu_note);
    create_right_click_actions(submenu_note, menuList);
    create_right_click_actions(mainMenu , QStringList(QString(""))); /// creating a separator

    add_notes_obj = new add_notes(this);
    connect(add_notes_obj,SIGNAL(signal_notes(QString)),this,SLOT(slot_add_notes_pushbutton_clicked(QString)));

    create_right_click_actions(mainMenu ,QStringList(QString("")));
    create_right_click_actions(mainMenu ,QStringList(QString(MACRO_Generic_Right_Click_Copy_To_Clipboard)));

}

void registry_interface::set_table_items(QTreeWidgetItem *item)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString sub_parent_path_str = item->text(enum_tree_widget_Parent_Path_Index);
    ui->tableWidget_registry->setRowCount(0);

    QString root_cmd = "Select distinct node_path ,node_name,file_path ,key_name , k_values ,types ,bookmark ,INT , source_count_name ,node_timestamp , notes , recon_tag_value , hex_values from tbl_registry_tree where parent_path = ?";

    QSqlQuery select_query(destination_db);
    select_query.prepare(root_cmd);
    select_query.addBindValue(sub_parent_path_str);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.executedQuery(), Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    int row_count = 0;
    while(select_query.next())
    {

        if(row_count % 500 == 0)
            QCoreApplication::processEvents();

        QTableWidgetItem *tble_INT_item = new QTableWidgetItem;

        QTableWidgetItem *tble_key_item = new QTableWidgetItem;
        QTableWidgetItem *tble_types_item = new QTableWidgetItem;
        QTableWidgetItem *tble_values_item = new QTableWidgetItem;
        QTableWidgetItem *tble_key_hex_values_item = new QTableWidgetItem;

        QTableWidgetItem *tble_source_count_name_item = new QTableWidgetItem;
        QTableWidgetItem *tble_node_path_item = new QTableWidgetItem;
        QTableWidgetItem *tble_node_name_item = new QTableWidgetItem;
        QTableWidgetItem *tble_file_path_item = new QTableWidgetItem;
        QTableWidgetItem *tble_node_timestamp_item = new QTableWidgetItem;

        QTableWidgetItem *tble_bookmark_item = new QTableWidgetItem;
        QTableWidgetItem *tble_notes_item = new QTableWidgetItem;
        QTableWidgetItem *tble_notes_icon_item = new QTableWidgetItem;
        QTableWidgetItem *item_tag_icon = new QTableWidgetItem;
        QTableWidgetItem *item_tag_text = new QTableWidgetItem;

        QString key_name_str = select_query.value("key_name").toString();
        QString types_str = select_query.value("types").toString();
        QString k_values_str = select_query.value("k_values").toString();
        QString key_hex_value_str = select_query.value("hex_values").toString();

        QString node_path_str = select_query.value("node_path").toString();
        QString node_name_str = select_query.value("node_name").toString();
        QString file_path_str = select_query.value("file_path").toString();
        QString bookmark_value_str = select_query.value("bookmark").toString();
        QString INT_str = select_query.value("INT").toString();
        QString source_count_name_str = select_query.value("source_count_name").toString();
        QString node_timestamp_str = select_query.value("node_timestamp").toString();
        QString notes_value_str = select_query.value("notes").toString();
        QString tag_value_str = select_query.value("recon_tag_value").toString();

        if(!key_name_str.trimmed().isEmpty())
            ui->tableWidget_registry->insertRow(row_count);
        else
            continue ;

        ui->tableWidget_registry->setRowHeight(row_count, 22);


        tble_INT_item->setText(INT_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_INT_Index, tble_INT_item);

        tble_key_item->setText(key_name_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_Key_Index, tble_key_item);

        tble_types_item->setText(types_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_Type_Index, tble_types_item);

        tble_values_item->setText(k_values_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_Values_Index, tble_values_item);

        if(bookmark_value_str == QString("0"))
        {
            tble_bookmark_item->setCheckState(Qt::Unchecked);
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_Bookmark_Index, tble_bookmark_item);
        }
        else
        {
            tble_bookmark_item->setCheckState(Qt::Checked);
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_Bookmark_Index, tble_bookmark_item);
        }

        tble_source_count_name_item->setText(source_count_name_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_source_count_name_Index, tble_source_count_name_item);

        tble_node_path_item->setText(node_path_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_node_path_Index, tble_node_path_item);

        tble_node_name_item->setText(node_name_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_node_name_Index, tble_node_name_item);

        tble_file_path_item->setText(file_path_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_file_path_Index, tble_file_path_item);

        ///// set node timestamp in epoch just to take value from table widget for bookmark , tags and notes
        tble_node_timestamp_item->setText(node_timestamp_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_node_timestamp_Index, tble_node_timestamp_item);

        if(notes_value_str.trimmed().isEmpty())
        {
            tble_notes_item->setText(notes_value_str);
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_note_Text_Index, tble_notes_item);
            tble_notes_icon_item->setIcon(QIcon(""));
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_Notes_Icon_Index, tble_notes_icon_item);
        }
        else
        {
            tble_notes_item->setText(notes_value_str);
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_note_Text_Index, tble_notes_item);
            QIcon iconNotes("../icons/notes.png");
            tble_notes_icon_item->setIcon(QIcon(iconNotes));
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_Notes_Icon_Index, tble_notes_icon_item);
        }

        if(tag_value_str.isEmpty())
        {
            QIcon icon("");
            item_tag_icon->setIcon(icon);
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_Tag_Icon_Index,item_tag_icon);
            item_tag_text->setText(tag_value_str);
            ui->tableWidget_registry->setItem(row_count , enum_table_widget_Tag_Text_Index , item_tag_text);
        }
        else
        {
            QString icon_str = tag_value_str;
            icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_value_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
            icon_str = icon_str.toLower();
            icon_str = icon_str.replace(" ","_");
            QIcon icon("../icons/tag_icons/"+ icon_str +".png");
            item_tag_icon->setIcon(icon);
            ui->tableWidget_registry->setItem(row_count, enum_table_widget_Tag_Icon_Index,item_tag_icon);
            item_tag_text->setText(tag_value_str);
            ui->tableWidget_registry->setItem(row_count , enum_table_widget_Tag_Text_Index , item_tag_text);
        }

        tble_key_hex_values_item->setText(key_hex_value_str);
        ui->tableWidget_registry->setItem(row_count, enum_table_widget_Hex_Values_Index, tble_key_hex_values_item);

        row_count++;
    }

    destination_db.close();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}


QTreeWidgetItem *registry_interface::get_currentptr_on_tree_iteration(QString parentpath)
{
    QTreeWidgetItem *child = new  QTreeWidgetItem;
    QTreeWidgetItemIterator iter_tree(ui->treeWidget_registry);

    while (*iter_tree)
    {
        if ((*iter_tree)->text(enum_tree_widget_Parent_Path_Index) == parentpath)
        {
            child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);
            break;
        }

        ++iter_tree;
    }

    return child;
}

void registry_interface::action_submenu_tag_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    tag_name = tag_name.trimmed();

    if(tag_name.isEmpty())
        return;

    if(tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,tag_db_path,Q_FUNC_INFO);

    emit signal_add_tags(tag_name,colour);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void registry_interface::create_right_click_actions(QMenu *mainMenu , QStringList submenuList)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    for(int i = 0; i < submenuList.size(); i++)
    {
        QAction *act_submenu = new QAction(submenuList.at(i).toLocal8Bit(), this);

        if(submenuList.at(i).isEmpty())
            act_submenu->setSeparator(true);

        mainMenu->addAction(act_submenu);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


QMenu* registry_interface::create_right_click_submenu(QString submenuStr ,QMenu *mainMenu , QMenu *subMenu)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    subMenu = mainMenu->addMenu(tr(submenuStr.toLocal8Bit().data()));

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    subMenu->setFont(m_font);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    return subMenu;
}


void registry_interface::create_tags_submenu()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_tags == NULL)
        return;

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));

    ///remove tag
    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    QIcon icon_remove("../icons/tag_icons/remove.png");
    act_remove_tag->setIcon(icon_remove);
    sub_menu_tags->addAction(act_remove_tag);

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);


    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";
    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path, Q_FUNC_INFO);

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



void registry_interface::action_bookmark_triggered()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QList<struct_global_tagging_registry_viewer> registry_viewer_record_list;

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();



    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    emit signal_save_openwith_changes_to_bucket(MACRO_CASE_TREE_Registry_Viewer);

    QStringList INT_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);


        int row = model_index.row();

        if(row < 0 && row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        m_tablewidget_obj->item(row , enum_table_widget_Bookmark_Index)->setCheckState(Qt::Checked);

        struct_global_tagging_registry_viewer s1;
        s1.INT_record = m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();
        INT_list << s1.INT_record;

        s1.plugin_name = QString(MACRO_Plugin_Name_Registry_Viewer);
        s1.tab_name = QString(MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer);
        s1.source_count_name = m_tablewidget_obj->item(row, enum_table_widget_source_count_name_Index)->text();

        s1.item0 = m_tablewidget_obj->item(row, enum_table_widget_file_path_Index)->text();
        s1.item1 = m_tablewidget_obj->item(row, enum_table_widget_node_path_Index)->text();
        s1.item2 = m_tablewidget_obj->item(row, enum_table_widget_node_name_Index)->text();
        s1.item3 = m_tablewidget_obj->item(row, enum_table_widget_Key_Index)->text();
        s1.item4 = m_tablewidget_obj->item(row, enum_table_widget_Type_Index)->text();
        s1.item5 = m_tablewidget_obj->item(row, enum_table_widget_Values_Index)->text();
        s1.item6 = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(m_tablewidget_obj->item(row, enum_table_widget_node_timestamp_Index)->text(), Q_FUNC_INFO);
        s1.item11 = display_tab_name;

        registry_viewer_record_list << s1;

    }

    global_connection_manager_obj->pub_set_bookmark_in_registry_viewer_db(INT_list,display_tab_name);
    global_connection_manager_obj->pub_add_bookmark_for_registry_viewer_db(registry_viewer_record_list);


    display_loading_progress_bar_non_cancelable_obj->hide();




    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::action_remove_bookmarks_triggered()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList INT_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);
        int row = model_index.row();

        if(row < 0 && row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        m_tablewidget_obj->item(row , enum_table_widget_Bookmark_Index)->setCheckState(Qt::Unchecked);

        INT_list << m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();
    }

    global_connection_manager_obj->pub_remove_bookmark_in_registry_viewer_db(INT_list,display_tab_name);
    global_connection_manager_obj->pub_delete_bookmark_for_registry_viewer_db(INT_list ,display_tab_name);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::action_add_note_triggered()
{
    add_notes_obj->set_notes("");
    add_notes_obj->show();
}

void registry_interface::action_remove_note_triggered()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);
    add_notes_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);
        int row = model_index.row();

        if(row < 0 && row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        m_tablewidget_obj->item(row, enum_table_widget_Notes_Icon_Index)->setIcon(QIcon(""));
        m_tablewidget_obj->item(row, enum_table_widget_note_Text_Index)->setText("");

        INT_list << m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();


    }

    global_connection_manager_obj->pub_remove_notes_in_registry_viewer_db(INT_list,display_tab_name);
    global_connection_manager_obj->pub_delete_notes_for_registry_viewer_db(INT_list ,display_tab_name);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::action_remove_tag_triggered()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        QModelIndex model_index = selection_model_list.at(i);

        int row = model_index.row();

        if(row < 0 && row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;

        m_tablewidget_obj->item(row, enum_table_widget_Tag_Icon_Index)->setIcon(QIcon(""));
        m_tablewidget_obj->item(row, enum_table_widget_Tag_Text_Index)->setText("");

        INT_list << m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();
    }

    global_connection_manager_obj->pub_remove_tag_in_registry_viewer_db(INT_list,display_tab_name);
    global_connection_manager_obj->pub_delete_tag_for_registry_viewer(INT_list ,display_tab_name);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::action_copy_to_clopboard_triggered()
{

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    recon_static_functions::app_debug("--Starts---", Q_FUNC_INFO);

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedIndexes();

    QString clipboard_data_str;
    QString clipboard_header_str;

    QApplication::clipboard()->clear(QClipboard::Clipboard);

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        int selected_column_no = model_index.column();

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        if(m_tablewidget_obj->item(selected_row_no, selected_column_no) == NULL || m_tablewidget_obj->isColumnHidden(selected_column_no))
            continue;

        if(selected_column_no == (model_index.model()->columnCount() - 1))// this is empty column in plugin that added in last
        {
            clipboard_data_str.append("\n");
            clipboard_header_str.append("\n");

            continue;
        }

        QString header_str = m_tablewidget_obj->horizontalHeaderItem(selected_column_no)->text().trimmed();

        if(header_str.isEmpty() || header_str == QString(Macro_Generic_OS_Scheme) || header_str == QString(MACRO_Generic_OS_Name))
            continue;

        clipboard_header_str.append(header_str + "\t");

        clipboard_data_str.append(m_tablewidget_obj->item(selected_row_no, selected_column_no)->text() + "\t");
    }


    clipboard_header_str.remove(clipboard_header_str.indexOf("\n"), clipboard_header_str.size());
    clipboard_header_str = clipboard_header_str.trimmed();

    QString combine_data_str;
    if(clipboard_header_str.contains("\t"))
        combine_data_str = clipboard_header_str.trimmed() + "\n" + clipboard_data_str.trimmed();
    else
        combine_data_str = clipboard_data_str.trimmed();


    QApplication::clipboard()->setText(combine_data_str, QClipboard::Clipboard);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("--Ends---", Q_FUNC_INFO);
}

void registry_interface::on_treeWidget_registry_itemExpanded(QTreeWidgetItem *item)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);



    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait....");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();

    QTreeWidgetItem *child_item =  new QTreeWidgetItem;
    child_item = item->child(enum_tree_widget_Item_Name_Index);
    if(child_item != NULL)
    {
        ///---remove intentionally --- temp entry that kept before
        if(child_item->text(enum_tree_widget_Item_Name_Index) == M_VAR_CHILD_ENTRY_QString_TEMP_in_tree_QString)
            item->removeChild(child_item);

    }

    ////return check is used here for redundant entries in treewidgetitem if expand item again
    if(item->child(enum_tree_widget_Item_Name_Index) != NULL)
        return ;



    QString sub_parent_path_str = item->text(enum_tree_widget_Parent_Path_Index);


    QSqlDatabase destination_db;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.databaseName() , Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }


    QString root_cmd = "Select distinct  child_name ,  parent_path , node_timestamp , node_path ,file_path , node_name ,source_count_name , Is_Parent from tbl_registry_tree where sub_parent_path = ?";
    QSqlQuery select_query(destination_db);
    select_query.prepare(root_cmd);
    select_query.addBindValue(sub_parent_path_str);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.executedQuery(), Q_FUNC_INFO);
        display_loading_progress_bar_non_cancelable_obj->hide();
        destination_db.close();
        return;
    }


    QList<QTreeWidgetItem*> child_items_list;
    qint64 counter = 0;
    while(select_query.next())
    {
        if(counter % 50 == 0)
            QApplication::processEvents();

        if(counter == 50)
            display_loading_progress_bar_non_cancelable_obj->show();


        QString parent_path = select_query.value("parent_path").toString();
        QString IS_Parent = select_query.value("Is_Parent").toString();

        //QTreeWidgetItem *new_child = get_currentptr_on_tree_iteration(parent_path);
        QTreeWidgetItem *new_child = new  QTreeWidgetItem;
        new_child->setText(enum_tree_widget_Item_Name_Index , select_query.value("child_name").toString());
        new_child->setText(enum_tree_widget_Parent_Path_Index , parent_path);
        new_child->setText(enum_tree_widget_Timestamp_Index , global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(select_query.value("node_timestamp").toString(), Q_FUNC_INFO));
        new_child->setText(enum_tree_widget_node_path ,select_query.value("node_path").toString());
        new_child->setText(enum_tree_widget_file_path ,select_query.value("file_path").toString());
        new_child->setText(enum_tree_widget_node_name ,select_query.value("node_name").toString());
        new_child->setText(enum_tree_widget_source_count_name ,select_query.value("source_count_name").toString());


        ///-- done intenationally---kept temp entry to make parent
        if(IS_Parent == QString("1"))
        {
            QTreeWidgetItem *temp_child = new QTreeWidgetItem;
            temp_child->setText(enum_tree_widget_Item_Name_Index , M_VAR_CHILD_ENTRY_QString_TEMP_in_tree_QString);
            new_child->addChild(temp_child);
        }

        child_items_list << new_child;

        counter++;
    }

    destination_db.close();

    item->addChildren(child_items_list);

    ui->treeWidget_registry->expandItem(item);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::slot_hide_loading_display_dialog_box(bool status)
{
    bool_cancel_loading = true;
}

void registry_interface::on_tableWidget_registry_itemClicked(QTableWidgetItem *item)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    set_metadata_on_table_widget_click();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::set_metadata_on_treewidget_click(QTreeWidgetItem *item)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    emit signal_clear_all_metadata();

    file_path_str.clear();
    node_path_str.clear();
    node_name_str.clear();
    node_timestamp_numeric_QString.clear();

    file_path_str = item->text(enum_tree_widget_file_path);
    node_path_str = item->text(enum_tree_widget_node_path);
    node_name_str = item->text(enum_tree_widget_node_name);



    source_count_name_str = item->text(enum_tree_widget_source_count_name);
    struct_GLOBAL_witness_info_source source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);
    source_name_str = source_info_obj.complete_source_name;


    if(source_name_str.trimmed().isEmpty())
        return ;

    QString metadata_str;
    metadata_str += "<b>" + QString("Plugin") + ":</b>" +QString("\t")+ plugin_name_str +"<br>" ;
    metadata_str += "<b>" + QString("Tab Name") + ":</b>" +QString("\t")+ tab_name_str +"\n<br><br>";
    metadata_str += "<b>" + QString("Source Name") + ":</b>" +QString("\t")+ source_name_str +"\n<br><br>";
    metadata_str += "<b>" + QString("File Path") + ":</b>" +QString("\t")+ file_path_str +"\n<br>";
    if(!node_path_str.trimmed().isEmpty())
        metadata_str += "<b>" + QString("Node Path") + ":</b>" +QString("\t")+ node_path_str +"\n<br>";

    metadata_str += "<b>" + QString("Node Name") + ":</b>" +QString("\t") + node_name_str +"\n<br>";



    QString select_cmd = "select distinct node_timestamp from tbl_registry_tree where parent_path =?";
    node_timestamp_numeric_QString = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,QStringList(item->text(enum_tree_widget_Parent_Path_Index)) ,0,destination_db_path,Q_FUNC_INFO);
    metadata_str += "<b>" + QString("Node Timestamp") + ":</b>" +QString("\t") + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(node_timestamp_numeric_QString, Q_FUNC_INFO) +"\n<br>";


    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);


    st_set_meta_obj.detailed_info = metadata_str;
    st_set_meta_obj.source_count_name = source_count_name_str;
    st_set_meta_obj.record_no = "";

    st_set_meta_obj.display_searched_text = ui->lineEdit_searched_text->text().trimmed();
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = tab_name_str;


    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

}

QString registry_interface::get_searched_command(QStringList search_on_columns_list)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);


    QStringList search_string_list = recon_static_functions::get_search_string_list(ui->lineEdit_searched_text->text().trimmed(), Q_FUNC_INFO);
    QString partial_command = recon_static_functions::get_partial_command_forsearch(search_on_columns_list,search_string_list, Q_FUNC_INFO);


    QStringList db_columns_list ;
    db_columns_list << "INT"             << "bookmark"  << "recon_tag_value" << "notes" << "source_count_name"
                    << "node_timestamp"  << "node_path" << "node_name"       << "file_path"
                    << "key_name"        << "k_values"  << "types"           << "hex_values";


    QString select_columns_for_table_widget = db_columns_list.join(",");
    QString complete_cmd = "select distinct " + select_columns_for_table_widget + " from tbl_registry_tree" + partial_command;

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

    return complete_cmd;
}


void registry_interface::draw_searched_items_in_table()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    ui->tabWidget->setCurrentIndex(enum_searched_items_tab_index);


    QTableWidget *m_tablewidget_obj_for_searched_item_tab = ui->tableWidget_searched_items;
    if(ui->lineEdit_searched_text->text().trimmed().isEmpty())
    {
        m_tablewidget_obj_for_searched_item_tab->setRowCount(0);
        return ;
    }





    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Searching...  ");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    QSqlDatabase destination_db;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }


    QStringList search_on_columns_list;
    search_on_columns_list  << "node_path" << "node_name" << "file_path" <<"node_timestamp"
                            << "key_name"  << "k_values"  << "types"     << "hex_values";

    QString searched_command = get_searched_command(search_on_columns_list);

    QString searched_text = ui->lineEdit_searched_text->text().trimmed();

    QSqlQuery select_query(destination_db);
    select_query.prepare(searched_command);
    for(int ii = 0 ; ii < search_on_columns_list.size() ; ii++)
    {
        select_query.addBindValue("%" + searched_text + "%");
    }
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(select_query.executedQuery(), Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        destination_db.close();
        return;
    }

    m_tablewidget_obj_for_searched_item_tab->setRowCount(0);

    int row_count = 0;
    while(select_query.next())
    {
        if(row_count % 500 == 0)
            QCoreApplication::processEvents();


        if(bool_cancel_loading)
            break ;


        m_tablewidget_obj_for_searched_item_tab->insertRow(row_count);

        QTableWidgetItem *tble_INT_item = new QTableWidgetItem;

        QTableWidgetItem *tble_key_item = new QTableWidgetItem;
        QTableWidgetItem *tble_types_item = new QTableWidgetItem;
        QTableWidgetItem *tble_values_item = new QTableWidgetItem;
        QTableWidgetItem *tble_key_hex_values_item = new QTableWidgetItem;

        QTableWidgetItem *tble_source_count_name_item = new QTableWidgetItem;
        QTableWidgetItem *tble_node_path_item = new QTableWidgetItem;
        QTableWidgetItem *tble_node_name_item = new QTableWidgetItem;
        QTableWidgetItem *tble_file_path_item = new QTableWidgetItem;
        QTableWidgetItem *tble_node_timestamp_item = new QTableWidgetItem;

        QTableWidgetItem *tble_bookmark_item = new QTableWidgetItem;
        QTableWidgetItem *tble_notes_item = new QTableWidgetItem;
        QTableWidgetItem *tble_notes_icon_item = new QTableWidgetItem;
        QTableWidgetItem *item_tag_icon = new QTableWidgetItem;
        QTableWidgetItem *item_tag_text = new QTableWidgetItem;

        QString key_name_str = select_query.value("key_name").toString();
        QString types_str = select_query.value("types").toString();
        QString k_values_str = select_query.value("k_values").toString();
        QString key_hex_value_str = select_query.value("hex_values").toString();

        QString node_path_str = select_query.value("node_path").toString();
        QString node_name_str = select_query.value("node_name").toString();
        QString file_path_str = select_query.value("file_path").toString();
        QString bookmark_value_str = select_query.value("bookmark").toString();
        QString INT_str = select_query.value("INT").toString();
        QString source_count_name_str = select_query.value("source_count_name").toString();
        QString node_timestamp_str = select_query.value("node_timestamp").toString();
        QString notes_value_str = select_query.value("notes").toString();
        QString tag_value_str = select_query.value("recon_tag_value").toString();



        m_tablewidget_obj_for_searched_item_tab->setRowHeight(row_count, 22);

        tble_INT_item->setText(INT_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_INT_Index, tble_INT_item);

        tble_key_item->setText(key_name_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Key_Index, tble_key_item);

        tble_types_item->setText(types_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Type_Index, tble_types_item);

        tble_values_item->setText(k_values_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Values_Index, tble_values_item);

        if(bookmark_value_str == QString("0"))
        {
            tble_bookmark_item->setCheckState(Qt::Unchecked);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Bookmark_Index, tble_bookmark_item);
        }
        else
        {
            tble_bookmark_item->setCheckState(Qt::Checked);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Bookmark_Index, tble_bookmark_item);
        }

        tble_source_count_name_item->setText(source_count_name_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_source_count_name_Index, tble_source_count_name_item);

        tble_node_path_item->setText(node_path_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_node_path_Index, tble_node_path_item);

        tble_node_name_item->setText(node_name_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_node_name_Index, tble_node_name_item);

        tble_file_path_item->setText(file_path_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_file_path_Index, tble_file_path_item);


        tble_node_timestamp_item->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(node_timestamp_str, Q_FUNC_INFO));
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_node_timestamp_Index, tble_node_timestamp_item);


        if(notes_value_str.trimmed().isEmpty())
        {
            tble_notes_item->setText(notes_value_str);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_note_Text_Index, tble_notes_item);
            tble_notes_icon_item->setIcon(QIcon(""));
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Notes_Icon_Index, tble_notes_icon_item);
        }
        else
        {
            tble_notes_item->setText(notes_value_str);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_note_Text_Index, tble_notes_item);
            QIcon iconNotes("../icons/notes.png");
            tble_notes_icon_item->setIcon(QIcon(iconNotes));
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Notes_Icon_Index, tble_notes_icon_item);
        }

        if(tag_value_str.isEmpty())
        {
            QIcon icon("");
            item_tag_icon->setIcon(icon);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Tag_Icon_Index,item_tag_icon);
            item_tag_text->setText(tag_value_str);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count , enum_table_widget_Tag_Text_Index , item_tag_text);
        }
        else
        {
            QString icon_str = tag_value_str;
            icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_value_str,global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
            icon_str = icon_str.toLower();
            icon_str = icon_str.replace(" ","_");
            QIcon icon("../icons/tag_icons/"+ icon_str +".png");
            item_tag_icon->setIcon(icon);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Tag_Icon_Index,item_tag_icon);
            item_tag_text->setText(tag_value_str);
            m_tablewidget_obj_for_searched_item_tab->setItem(row_count , enum_table_widget_Tag_Text_Index , item_tag_text);
        }

        tble_key_hex_values_item->setText(key_hex_value_str);
        m_tablewidget_obj_for_searched_item_tab->setItem(row_count, enum_table_widget_Hex_Values_Index, tble_key_hex_values_item);

        row_count++;
    }

    destination_db.close();

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::set_bookmark(int row, int column)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return ;

    if(column != enum_table_widget_Bookmark_Index)
        return ;


    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    m_tablewidget_obj->item(row , column)->text();

    QString INT_str = m_tablewidget_obj->item(row , enum_table_widget_INT_Index)->text();
    QList<struct_global_tagging_registry_viewer> registry_viewer_record_list;
    if(row < 0 && row >= m_tablewidget_obj->rowCount())
        return;


    struct_global_tagging_registry_viewer s1;
    s1.INT_record = m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();

    s1.plugin_name = plugin_name_str;
    s1.tab_name = tab_name_str;
    s1.source_count_name = m_tablewidget_obj->item(row, enum_table_widget_source_count_name_Index)->text();

    s1.item0 = m_tablewidget_obj->item(row, enum_table_widget_file_path_Index)->text();
    s1.item1 = m_tablewidget_obj->item(row, enum_table_widget_node_path_Index)->text();
    s1.item2 = m_tablewidget_obj->item(row, enum_table_widget_node_name_Index)->text();
    s1.item3 = m_tablewidget_obj->item(row, enum_table_widget_Key_Index)->text();
    s1.item4 = m_tablewidget_obj->item(row, enum_table_widget_Type_Index)->text();
    s1.item5 = m_tablewidget_obj->item(row, enum_table_widget_Values_Index)->text();
    s1.item6 = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(m_tablewidget_obj->item(row, enum_table_widget_node_timestamp_Index)->text(), Q_FUNC_INFO);
    s1.item11 = display_tab_name;

    registry_viewer_record_list << s1;


    if(m_tablewidget_obj->item(row , column)->checkState() == Qt::Checked)
    {
        emit signal_save_openwith_changes_to_bucket(MACRO_CASE_TREE_Registry_Viewer);

        global_connection_manager_obj->pub_set_bookmark_in_registry_viewer_db(QStringList(INT_str) ,display_tab_name);
        global_connection_manager_obj->pub_add_bookmark_for_registry_viewer_db(registry_viewer_record_list);
    }
    else if(m_tablewidget_obj->item(row ,column)->checkState() == Qt::Unchecked)
    {
        global_connection_manager_obj->pub_remove_bookmark_in_registry_viewer_db(QStringList(INT_str) ,display_tab_name);
        global_connection_manager_obj->pub_delete_bookmark_for_registry_viewer_db(QStringList(INT_str) ,display_tab_name);
    }

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

QTableWidget *registry_interface::get_m_tablewidget_obj_for_current_tab()
{
    QTableWidget *m_tablewidget_obj;
    if(ui->tabWidget->currentIndex() == enum_all_items_tab_index)
    {
        m_tablewidget_obj = ui->tableWidget_registry;
    }
    else if(ui->tabWidget->currentIndex() == enum_searched_items_tab_index)
    {
        m_tablewidget_obj = ui->tableWidget_searched_items;
    }

    return m_tablewidget_obj;
}

void registry_interface::set_metadata_on_table_widget_click()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    ui->tabWidget->blockSignals(true);

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    if(selection_model_list.size() < 1)
        return ;


    qint64 t_current_selected_row = m_tablewidget_obj->currentRow();
    if(m_tablewidget_obj->item(t_current_selected_row ,enum_table_widget_INT_Index) == NULL)
        return;

    if(m_tablewidget_obj->item(t_current_selected_row ,enum_table_widget_source_count_name_Index) == NULL)
        return;

    QString record_no = m_tablewidget_obj->item(t_current_selected_row ,enum_table_widget_INT_Index)->text();
    QString src_cnt_nm = m_tablewidget_obj->item(t_current_selected_row ,enum_table_widget_source_count_name_Index)->text();

    struct_global_fetch_metadata struct_metadata_values_obj = recon_helper_standard_obj->fetch_metadata_CSV_and_INT_based("tbl_registry_tree",0,record_no,plugin_name_str,destination_db_path,src_cnt_nm,Q_FUNC_INFO);
    QString metadata_str;
    metadata_str += struct_metadata_values_obj.metadata;

    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.detailed_info = metadata_str;
    st_set_meta_obj.source_count_name = src_cnt_nm;
    st_set_meta_obj.record_no = "";
    st_set_meta_obj.display_searched_text = ui->lineEdit_searched_text->text().trimmed();
    st_set_meta_obj.plugin_name = plugin_name_str;
    st_set_meta_obj.tab_name = tab_name_str;



    emit signal_set_metadata_info(st_set_meta_obj);


    ui->tabWidget->blockSignals(false);

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}
