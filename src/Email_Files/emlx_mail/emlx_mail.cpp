#include "emlx_mail.h"
#include "ui_emlx_mail.h"

emlx_mail::emlx_mail(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::emlx_mail)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    installEventFilter(this);


    //==============(Temp)-Start
    //ui->lineEdit_search->hide();
    //ui->pushButton_filters->hide();
    //ui->pushButton_advance_search->hide();
    //==============(Temp)-End

    ui->textBrowser_message->setReadOnly(true);
    //ui->tableWidget_emails->hideColumn(0);
    ui->tableWidget_emails->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_emails->verticalHeader()->setStretchLastSection(false);

    ui->tableWidget_emails->hideColumn(enum_emlx_account_id_column);
    ui->tableWidget_emails->hideColumn(enum_emlx_mbox_column);
    ui->tableWidget_emails->hideColumn(enum_emlx_source_file_column);
    ui->tableWidget_emails->hideColumn(enum_emlx_file_name_column);
    ui->tableWidget_emails->hideColumn(enum_emlx_source_count_name_column);

    ui->tableWidget_emails->setColumnWidth(enum_emlx_bookmark_column , 30);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_tag_column , 30);

    ui->tableWidget_emails->installEventFilter(this);

    ui->lineEdit_message_search->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_message_search->setClearButtonEnabled(true);


    ui->treeWidget_accounts->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->treeWidget_accounts->hideColumn(1);


    av_preview_obj    = new av_preview(this);
    image_preview_obj = new image_preview(this);
    blank_obj = new blank(this);

    ui->stackedWidget_preview->setFixedWidth(300);

    ui->stackedWidget_preview->addWidget(blank_obj);
    ui->stackedWidget_preview->addWidget(av_preview_obj);
    ui->stackedWidget_preview->addWidget(image_preview_obj);

    ui->stackedWidget_preview->setCurrentIndex(enum_no_preview_index);

    ui->tableWidget_attachments->setColumnWidth(0,250);
    ui->tableWidget_attachments->setColumnWidth(1,160);
    ui->tableWidget_attachments->setColumnWidth(2,200);



    menu_search_filters  = new QMenu(this);
    ui->pushButton_filters->setMenu(menu_search_filters);

    QStringList filters_list;
    filters_list << "Sr. No." << "To" << "From" << "Cc" << "Subject" << "Message Body";

    display_header_name_list  << "Sr. No." << "bookmarks" <<"Recon Tag Icon";
    //display_header_name_list  << filters_list;
    display_header_name_list << "To" << "From" << "Cc" << "Subject" << "Message Body";

    for(int count = 0; count < filters_list.size(); count++)
    {
        QAction *action_filters   =  new QAction(filters_list.at(count) , this);
        action_filters->setCheckable(true);

        if(filters_list.at(count).trimmed() == QString("To") || filters_list.at(count).trimmed() == QString("Message Body"))
            action_filters->setChecked(true);

        menu_search_filters->addAction(action_filters);
    }


    bool_sort_ascending = true;
    connect((QObject*)ui->tableWidget_emails->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));

    ui->treeWidget_accounts->setStyleSheet("QTreeWidget {"
                                           "selection-background-color: #CDCDCD;"
                                           "selection-color: black;"
                                           "}"
                                           "QTreeWidget::item {"
                                           "height: 23px;"

                                           "}"
                                           "QTreeWidget::item:selected:!active {"
                                           "color:black;"
                                           " background-color:  #CDCDCD;"

                                           "}"
                                           );

    ui->tableWidget_emails->setAlternatingRowColors(true);
    ui->tableWidget_emails->setStyleSheet(QString(

                                              "QTableView::item {"
                                              "border-top: 1px solid white;"
                                              " }"

                                              "QTableView::item:selected:!active {"
                                              "color:white;"
                                              "background-color:  #a1c9f7;"
                                              "}"

                                              "QTableView::indicator:checked {background-color:black;border: 1px solid #d6d6d6;}"
                                              "QTableView::indicator:unchecked {background-color:#f1f1f1;border: 1px solid #777777;}"

                                              "QTableView::indicator {"
                                              "width: 13px;"
                                              "height: 13px;}"
                                              ));

    ui->tabWidget_details->setCurrentIndex(0);

    email_search_obj = new email_search(this);
    connect(email_search_obj, SIGNAL(signal_email_search_work_done(QString)),this, SLOT(slot_email_search_work_done(QString)));
    connect(email_search_obj, SIGNAL(signal_highlight_searched_text(QString)),this, SLOT(slot_highlight_searched_text(QString)));

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj, SIGNAL(signal_close_loading_display_dialog(bool)), this, SLOT(slot_cancel_loading_search_results(bool)));
    bool_cancel_loading_search_results = false;


    menu_raw_data = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    menu_raw_data->setFont(m_font);

    connect(menu_raw_data, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_menu_raw_data_triggered(QAction*)));

    act_raw_data_select_all = new QAction(QString("Select All"), this);
    menu_raw_data->addAction(act_raw_data_select_all);

    //===========Raw Tag===============//
    sub_menu_raw_data_tags = menu_raw_data->addMenu(tr(QString(MACRO_Generic_Right_Click_Tags).toLocal8Bit().data()));
    sub_menu_raw_data_tags->setFont(m_font);

    add_raw_data_tag_module_obj = new add_tags(this);

    create_raw_data_tags_submenu();

    connect(sub_menu_raw_data_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_raw_data_tags_clicked(QAction*)));
    connect(sub_menu_raw_data_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    //=================================//

    ui->textBrowser_raw_data->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textBrowser_raw_data, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(slot_textBrowser_raw_data_customContextMenuRequested(const QPoint&)));




}

emlx_mail::~emlx_mail()
{
    delete ui;
}

bool emlx_mail::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->tableWidget_emails)
    {
        if(event->type() == QEvent::ContextMenu)
        {
            QModelIndexList selection_model_list = ui->tableWidget_emails->selectionModel()->selectedRows();
            if(selection_model_list.size() < 1)
                return false;

            struct_global_right_clicks_status_depends_on_file_existance obj;
            obj.feature_name = QString(MACRO_CASE_TREE_Email_Files_Parent);
            obj.plugin_name = QString(MACRO_Plugin_Name_Email_Parser);
            obj.record_no = ui->tableWidget_emails->item(ui->tableWidget_emails->currentRow(), enum_emlx_sr_no_column)->text();
            obj.source_count_name = ui->tableWidget_emails->item(ui->tableWidget_emails->currentRow(), enum_emlx_source_count_name_column)->text();
            obj.arguments_values_list.clear();
            obj.arguments_values_list << ui->tableWidget_emails->item(ui->tableWidget_emails->currentRow(), enum_emlx_source_file_column)->text();
            recon_helper_standard_obj->pub_enable_or_disable_right_clicks_depends_on_file_existance(obj ,mainMenu , Q_FUNC_INFO);

            mainMenu->exec(QCursor::pos());
            return true;
        }
    }

    return false;
}

void emlx_mail::pub_set_essentials()
{
    QString featr_emails_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();

    case_confg_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    //source_db_path      =  featr_parent_path + QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_RESULT_DIR_Emlx_Files) + QString("emlx_data.sqlite");
    //attachment_dir_path =  featr_parent_path + QString(MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_RESULT_DIR_Emlx_Files) + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments);

    //    source_db_path      =  featr_parent_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_EMLX_Mail) + QString("emlx_data.sqlite");
    //        attachment_dir_path =  featr_parent_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_EMLX_Mail) + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments);
    source_db_path      =  featr_emails_parent_path + QString("email_data.sqlite");
    attachment_dir_path =  featr_emails_parent_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments);
}

void emlx_mail::pub_set_global_connection_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void emlx_mail::pub_display_data_in_table()
{
    emlx_show_data_table_widget();


    add_tags_module_obj = new add_tags(this);
    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_submenu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    sub_menu_tags = new QMenu(this);


    mainMenu = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_file_system_menu_right_clicked(QAction*)));



    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Add_Bookmark)
             << QString(MACRO_Generic_Right_Click_Remove_Bookmarks);

    submenu_bookmark = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Bookmark), mainMenu, submenu_bookmark);
    create_right_click_actions(submenu_bookmark,menuList);

    sub_menu_tags = create_right_click_submenu(QString(MACRO_Generic_Right_Click_Tags) , mainMenu , sub_menu_tags);

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_tags_submenu_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));
    connect(this, SIGNAL(signal_add_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    create_tags_submenu();


    create_right_click_actions(mainMenu,QStringList(""));

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Quick_look);
    create_right_click_actions(mainMenu ,menuList);

    create_right_click_actions(mainMenu,QStringList(""));

    menuList.clear();
    menuList << QString(MACRO_Generic_Right_Click_Go_To_Record);
    create_right_click_actions(mainMenu ,menuList);

}

void emlx_mail ::emlx_show_data_table_widget()
{
    ui->tableWidget_emails->setRowCount(0);
    //ui->tableWidget_emails->hideColumn(0);

    ui->stackedWidget_preview->setCurrentIndex(enum_no_preview_index);


    ui->textBrowser_message->clear();
    QTextCursor cursor = ui->textBrowser_message->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textBrowser_message->setTextCursor(cursor);
    ui->textBrowser_message->append("\n\n\n\n\n\n");
    ui->textBrowser_message->append("<font size = 70>No Email Selected</font>");

    int row = ui->tableWidget_emails->rowCount();

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(source_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    QList_struct_mail_pst_and_mbox_file_info_details.clear();

    QString cmd = "SELECT INT, bookmark, emlx_to, emlx_from, emlx_cc, "
                  "emlx_date, emlx_subject, emlx_msg_body, email_attachment, account_id,"
                  "mbox, source_file, recon_tag_value, source_count_name, file_name,file_raw_bytes FROM emlx_data";

    if(!query.exec(cmd))
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
        source_db.close();
        return;
    }

    bool_cancel_loading_search_results = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    qint64 m_counter = 0;

    while(query.next())
    {
        if(bool_cancel_loading_search_results)
        {
            break;
        }

        if(m_counter % 50 == 0)
        {
            m_counter = 0;
            QCoreApplication::processEvents();
        }
        m_counter++;

        QString sr_no_str    = query.value(0).toString().trimmed();
        QString bookmark_str = query.value(1).toString().trimmed();
        QString to_str       = query.value(2).toString().trimmed();
        QString from_str     = query.value(3).toString().trimmed();
        QString cc_str       = query.value(4).toString().trimmed();
        QString datetime_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(5).toString().trimmed(), Q_FUNC_INFO);
        QString subject_str  = query.value(6).toString().trimmed();
        //QString message_str  = query.value(7).toString().trimmed(); //not in use
        QString attachment_str   = query.value(8).toString().trimmed();
        QString acc_id_str       = query.value(9).toString().trimmed();
        QString message_box_str  = query.value(10).toString().trimmed();
        QString source_file_str  = query.value(11).toString().trimmed();
        QString tag_value  = query.value(12).toString().trimmed();
        QString source_count_name  = query.value(13).toString().trimmed();

        QString file_name = query.value("file_name").toString().trimmed();

        QTableWidgetItem *item_tag        = new QTableWidgetItem;
        QTableWidgetItem *item_sr_no      = new QTableWidgetItem;
        QTableWidgetItem *item_bookmark   = new QTableWidgetItem;
        QTableWidgetItem *item_to         = new QTableWidgetItem;
        QTableWidgetItem *item_from       = new QTableWidgetItem;
        QTableWidgetItem *item_cc         = new QTableWidgetItem;
        QTableWidgetItem *item_datetime   = new QTableWidgetItem;
        QTableWidgetItem *item_subject    = new QTableWidgetItem;
        QTableWidgetItem *item_attachment = new QTableWidgetItem;
        QTableWidgetItem *item_account_id = new QTableWidgetItem;
        QTableWidgetItem *item_mbox       = new QTableWidgetItem;
        QTableWidgetItem *item_source       = new QTableWidgetItem;
        QTableWidgetItem *item_source_count = new QTableWidgetItem;
        QTableWidgetItem *item_file_name = new QTableWidgetItem;

        item_sr_no->setData(Qt::EditRole, sr_no_str.toLongLong());
        item_sr_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_to->setText(to_str);
        item_from->setText(from_str);
        item_cc->setText(cc_str);
        item_datetime->setText(datetime_str);
        item_subject->setText(subject_str);
        item_account_id->setText(acc_id_str);
        item_mbox->setText(message_box_str);
        item_source->setText(source_file_str);

        item_file_name->setText(file_name);

        if(attachment_str.trimmed().isEmpty())
            item_attachment->setData(Qt::EditRole, QString::number(0).toLongLong());
        else
            item_attachment->setData(Qt::EditRole,QString::number(attachment_str.split(",").size()).toLongLong());

        item_attachment->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        if(bookmark_str == QString::number(1))
            item_bookmark->setCheckState(Qt::Checked);
        else
            item_bookmark->setCheckState(Qt::Unchecked);


        item_source_count->setText(source_count_name);

        //ui->tableWidget_emails->hideColumn(0);
        ui->tableWidget_emails->insertRow(row);
        ui->tableWidget_emails->setRowHeight(row,22);


        ui->tableWidget_emails->setItem(row, enum_emlx_sr_no_column,item_sr_no);
        ui->tableWidget_emails->setItem(row, enum_emlx_bookmark_column,item_bookmark);
        if(tag_value.isEmpty())
        {
            QIcon icon("");
            item_tag->setIcon(icon);
            ui->tableWidget_emails->setItem(row, enum_emlx_tag_column,item_tag);
        }
        else
        {
            QString icon_str = tag_value;
            icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_value,case_confg_db_path , Q_FUNC_INFO);
            icon_str = icon_str.toLower();
            icon_str = icon_str.replace(" ","_");
            QIcon icon("../icons/tag_icons/"+ icon_str +".png");
            item_tag->setIcon(icon);

            ui->tableWidget_emails->setItem(row, enum_emlx_tag_column,item_tag);

        }
        ui->tableWidget_emails->setItem(row, enum_emlx_to_column,item_to);
        ui->tableWidget_emails->setItem(row, enum_emlx_from_column,item_from);
        ui->tableWidget_emails->setItem(row, enum_emlx_Cc_column,item_cc);
        ui->tableWidget_emails->setItem(row, enum_emlx_subject_column,item_subject);
        ui->tableWidget_emails->setItem(row, enum_emlx_datetime_column,item_datetime);
        ui->tableWidget_emails->setItem(row, enum_emlx_attachments_column,item_attachment);
        ui->tableWidget_emails->setItem(row, enum_emlx_account_id_column, item_account_id);
        ui->tableWidget_emails->setItem(row, enum_emlx_mbox_column, item_mbox);
        ui->tableWidget_emails->setItem(row, enum_emlx_source_file_column, item_source);
        ui->tableWidget_emails->setItem(row, enum_emlx_source_count_name_column, item_source_count);
        ui->tableWidget_emails->setItem(row, enum_emlx_file_name_column, item_file_name);

        if(file_name.endsWith(".pst") || source_file_str.endsWith(".mbox"))
        {
            QByteArray file_raw_bytes = query.value("file_raw_bytes").toByteArray().trimmed();
            struct_mail_pst_and_mbox_file_info struct_pst_and_mbox_file_info_obj;

            struct_pst_and_mbox_file_info_obj.record_int = sr_no_str.trimmed();
            struct_pst_and_mbox_file_info_obj.folder_name = message_box_str.trimmed();
            struct_pst_and_mbox_file_info_obj.source_count_name = source_count_name.trimmed();
            struct_pst_and_mbox_file_info_obj.file_data_bytes = file_raw_bytes.trimmed();

            //this list is used to diplay the raw data on each record click from the db only for pst and mbox files
            //As PST is convert into Emlx format files at adhoc path
            //In PST the emlx(adhoc path) file not exist physically so we saved all the raw data in db and used it from there only
            //In MBOX the emlx file not exist so we saved all the raw data in db and used it from there only

            QList_struct_mail_pst_and_mbox_file_info_details << struct_pst_and_mbox_file_info_obj;

            bool bool_item_exist_already = check_item_already_exist_in_complete_tree(acc_id_str,true);

            if(bool_item_exist_already)
            {
                bool bool_item_child_exit_already = check_item_already_exist_in_specified_tree(parent_item, message_box_str , false);
                if(!bool_item_child_exit_already)
                {
                    QTreeWidgetItem *acc_tree_child_item;
                    QString acc_tree_child_str = message_box_str;
                    acc_tree_child_item = new QTreeWidgetItem();
                    acc_tree_child_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                    acc_tree_child_item->setText(0,acc_tree_child_str);
                    acc_tree_child_item->setText(1,acc_id_str);
                    parent_item->addChild(acc_tree_child_item);
                }
            }
            else
            {
                QTreeWidgetItem  *parent_acc_item = new QTreeWidgetItem();
                parent_acc_item->setText(0,acc_id_str);
                ui->treeWidget_accounts->addTopLevelItem(parent_acc_item);
            }
        }
        else
        {
            ///=================================Accounts Tree================================
            QString tree_file_path = source_file_str;
            QStringList root_parent_split_list = tree_file_path.split("/");
            QString mbox_str = message_box_str;

            if(mbox_str.endsWith(QString(MACRO_RECON_Splitter_1_multiple)))
                mbox_str.chop(QString(MACRO_RECON_Splitter_1_multiple).size());


            bool bool_item_exist_already = check_item_already_exist_in_complete_tree(acc_id_str , true);


            if(!bool_item_exist_already)
            {
                ///add account id
                QTreeWidgetItem *tree_item;
                tree_item = new QTreeWidgetItem();
                tree_item->setText(0,acc_id_str);
                ui->treeWidget_accounts->addTopLevelItem(tree_item);

                /// add mbox
                QStringList mbox_index_list;
                mbox_index_list = get_index_of_mbox_from_path(root_parent_split_list);

                if(!mbox_index_list.isEmpty())
                {
                    for(int count = 0; count < mbox_index_list.size(); count++)
                    {
                        int mbox_index = mbox_index_list.at(count).toInt();
                        QString mbox_str = root_parent_split_list.at(mbox_index);

                        if(mbox_str.contains(QString(MACRO_RECON_Splitter_1_multiple)))
                        {
                            QStringList mbox_split_list;
                            mbox_split_list = mbox_str.split(QString(MACRO_RECON_Splitter_1_multiple));


                            QTreeWidgetItem *root_mbox_item;
                            QString root_mbox_str = mbox_split_list.takeAt(0);
                            root_mbox_item = new QTreeWidgetItem();
                            root_mbox_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                            root_mbox_item->setText(0,root_mbox_str);
                            root_mbox_item->setText(1,acc_id_str);
                            tree_item->addChild(root_mbox_item);

                            for(int count = 0; count < mbox_split_list.size(); count++)
                            {
                                QTreeWidgetItem *mbox_item;
                                QString mbox_str = mbox_split_list.at(count);
                                mbox_item = new QTreeWidgetItem();
                                mbox_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                                mbox_item->setText(0,mbox_str);
                                mbox_item->setText(1,acc_id_str);
                                root_mbox_item->addChild(mbox_item);

                                ///to add next mbox under it.
                                root_mbox_item = mbox_item;
                            }
                        }
                        else
                        {
                            QTreeWidgetItem *mbox_item;
                            mbox_item = new QTreeWidgetItem();
                            mbox_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                            mbox_item->setText(0,mbox_str);
                            mbox_item->setText(1,acc_id_str);
                            tree_item->addChild(mbox_item);
                        }
                    }
                }
            }
            else
            {

                /// add mbox
                if(mbox_str.contains(QString(MACRO_RECON_Splitter_1_multiple)))
                {
                    QStringList mbox_split_list;
                    mbox_split_list = mbox_str.split(QString(MACRO_RECON_Splitter_1_multiple));

                    QTreeWidgetItem *root_mbox_item;
                    QString root_mbox_str = mbox_split_list.takeAt(0);
                    bool bool_item_exist_already = check_item_already_exist_in_specified_tree(parent_item ,root_mbox_str , true);

                    if(!bool_item_exist_already)
                    {
                        root_mbox_item = new QTreeWidgetItem();
                        root_mbox_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                        root_mbox_item->setText(0,root_mbox_str);
                        root_mbox_item->setText(1,acc_id_str);
                        parent_item->addChild(root_mbox_item);
                    }
                    else
                    {
                        root_mbox_item = parent_mbox_item;
                    }

                    for(int count = 0; count < mbox_split_list.size(); count++)
                    {
                        QTreeWidgetItem *mbox_item;
                        QString mbox_str = mbox_split_list.at(count);
                        bool bool_item_exist_already = check_item_already_exist_in_specified_tree(root_mbox_item ,mbox_str , true);

                        if(!bool_item_exist_already)
                        {
                            mbox_item = new QTreeWidgetItem();
                            mbox_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                            mbox_item->setText(0,mbox_str);
                            mbox_item->setText(1,acc_id_str);
                            root_mbox_item->addChild(mbox_item);
                        }
                        else
                        {
                            mbox_item = parent_mbox_item;
                        }

                        ///to add next mbox under it.
                        root_mbox_item = mbox_item;
                    }
                }
                else
                {
                    bool bool_item_exist_already = check_item_already_exist_in_specified_tree(parent_item ,mbox_str , false);

                    if(!bool_item_exist_already)
                    {
                        QTreeWidgetItem *mbox_item;
                        mbox_item = new QTreeWidgetItem();
                        mbox_item->setIcon(0, QIcon("../icons/signs/mbox.png"));
                        mbox_item->setText(0,mbox_str);
                        mbox_item->setText(1,acc_id_str);
                        parent_item->addChild(mbox_item);
                    }
                }
            }
            ///=================================================================
        }
    }

    source_db.close();

    ui->treeWidget_accounts->sortByColumn(0,Qt::AscendingOrder);
    ui->tableWidget_emails->sortByColumn(enum_emlx_sr_no_column,Qt::AscendingOrder);

    ui->tableWidget_emails->setColumnWidth(enum_emlx_tag_column , 30);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_bookmark_column,25);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_to_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_from_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_Cc_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_subject_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_datetime_column,200);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_attachments_column,100);

    display_loading_progress_bar_obj->hide();

}


void emlx_mail::on_tableWidget_emails_cellPressed(int row, int column)
{

    Q_UNUSED(column);

    if(row < 0)
        return;

    ui->textBrowser_message->setUndoRedoEnabled(false);
    ui->textBrowser_raw_data->setUndoRedoEnabled(false);

    ui->textBrowser_raw_data->clear();
    ui->textBrowser_message->clear();
    ui->tableWidget_attachments->setRowCount(0);
    ui->label_emlx_file_path->clear();

    current_record_sr_str = ui->tableWidget_emails->item(row,enum_emlx_sr_no_column)->text().trimmed();

    if(current_record_sr_str.isEmpty())
        return;

    emlx_show_data_text_edit();
    emlx_show_data_in_raw_data_text_edit();

    on_pushButton_search_message_clicked();
    on_pushButton_raw_data_search_clicked();

    // preview attachment
    emlx_preview_attachment_file();

    QString emlx_source_file = ui->tableWidget_emails->item(ui->tableWidget_emails->currentRow(),enum_emlx_source_file_column)->text().trimmed();

    QString source_count_name = ui->tableWidget_emails->item(row,enum_emlx_source_count_name_column)->text().trimmed();
    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString full_path = struct_source_info.virtual_source_path + emlx_source_file;

    emit signal_quick_look_preview_filepath_from_email_files(full_path);

    ui->label_emlx_file_path->setText(emlx_source_file);

    ui->label_emlx_file_path->setToolTip(emlx_source_file);

    slot_highlight_searched_text(highlight_searched_text_in_result);

}

void emlx_mail::emlx_preview_attachment_file()
{
    ui->stackedWidget_preview->setCurrentIndex(enum_no_preview_index);

    if(ui->tableWidget_attachments->rowCount() > 0)
    {
        /// preview only first file
        QString attachment_filename = ui->tableWidget_attachments->item(0,0)->text().trimmed();

        QString file_path = attachment_dir_path + attachment_filename;

        QFileInfo info(file_path);
        if(info.exists())
        {

            QString file_mime_type = mime_database_obj.mimeTypeForFile(file_path).name();

            if(file_mime_type.startsWith("video/"))
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_audio_video_preview_index);
                av_preview_obj->load_source(file_path);
            }
            else if (file_mime_type.startsWith("audio/"))
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_audio_video_preview_index);
                av_preview_obj->load_source(file_path);
            }
            else if(recon_helper_standard_obj->check_is_it_image_file(file_path,file_path, Q_FUNC_INFO))
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_image_preview_index);
                image_preview_obj->set_image_by_path(file_path);
            }
        }
    }

}

void emlx_mail::emlx_show_data_text_edit()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(global_variable_macOS_appearance_light_mode_bool)
    {
        QTextCharFormat fmt;
        fmt.setForeground(QBrush(QColor(Qt::black)));
        ui->textBrowser_message->mergeCurrentCharFormat(fmt);
    }
    else {
        QTextCharFormat fmt;
        fmt.setForeground(QBrush(QColor(Qt::white)));
        ui->textBrowser_message->mergeCurrentCharFormat(fmt);
    }

    int data_index = current_record_sr_str.toInt();

    if(data_index < 0)
        return;

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(source_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    QString cmd = "select emlx_from,emlx_date,emlx_to,emlx_cc,emlx_subject,emlx_msg_body,email_attachment from emlx_data where INT = (?) ";
    query.prepare(cmd);
    query.addBindValue(data_index);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(query.next())
    {
        QString attachment = query.value(6).toString().trimmed();

        QString datetime_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value("emlx_date").toString(), Q_FUNC_INFO);

        ui->textBrowser_message->append("<b>" "From: "+ query.value("emlx_from").toString() + "</b>");
        ui->textBrowser_message->append("To: "+ query.value("emlx_to").toString());
        ui->textBrowser_message->append("Cc: "+ query.value("emlx_cc").toString());
        ui->textBrowser_message->append("Subject: " + query.value("emlx_subject").toString());
        ui->textBrowser_message->append("Date: " + datetime_str);

        QString message = query.value("emlx_msg_body").toString();

        ui->textBrowser_message->append("\n-------------------Message Body---------------------------\n");
        ui->textBrowser_message->append(message);

        attachment = attachment.trimmed();

        if(!attachment.isEmpty())
        {
            QStringList attachment_message_list = attachment.split(",");

            for(int i = 0 ; i < attachment_message_list.size(); i++ )
            {
                QString filepath = attachment_dir_path + attachment_message_list.at(i);


                if(recon_helper_standard_obj->check_is_it_image_file(filepath,filepath, Q_FUNC_INFO))
                {
                    ui->textBrowser_message->append("<html><body><img src =" + filepath + " width =" + QString("200") +  "></body></html>");
                }

                QTableWidgetItem *item_attchmnt = new QTableWidgetItem();

                item_attchmnt->setText(attachment_message_list.at(i));

                QLabel * label_quick_look = new QLabel();
                label_quick_look->setText("<a href="+ QUrl::toPercentEncoding(filepath) + " > " + QString("Quicklook") + " </a>");
                label_quick_look->setTextInteractionFlags(Qt::TextBrowserInteraction);
                connect(label_quick_look, SIGNAL(linkActivated(QString)), this, SLOT(slot_quicklook_link_clicked(QString)));

                QLabel * label_open_in_finder = new QLabel();

                label_open_in_finder->setText("<a href="+ QUrl::toPercentEncoding(filepath) + " > " + QString("Open in Source") + " </a>");
                label_open_in_finder->setTextInteractionFlags(Qt::TextBrowserInteraction);
                connect(label_open_in_finder, SIGNAL(linkActivated(QString)), this, SLOT(slot_open_in_finder_link_clicked(QString)));


                ui->tableWidget_attachments->insertRow(i);
                ui->tableWidget_attachments->setRowHeight(i,22);
                ui->tableWidget_attachments->setItem(i, 0 , item_attchmnt);

                ui->tableWidget_attachments->setCellWidget(i, 1 , label_quick_look);
                ui->tableWidget_attachments->setCellWidget(i, 2 , label_open_in_finder);


                QTableWidgetItem *item_existence = new QTableWidgetItem();
                if(QFileInfo(filepath).exists())
                    item_existence->setText("Yes");
                else
                    item_existence->setText("No");

                ui->tableWidget_attachments->setItem(i, 3 , item_existence);


            }
        }

    }

    source_db.close();

    ui->textBrowser_message->moveCursor(QTextCursor::Start);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void emlx_mail::emlx_show_data_in_raw_data_text_edit()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);


    int row = ui->tableWidget_emails->currentRow();
    if(row < 0)
        return;

    int column_count = ui->tableWidget_emails->columnCount();
    if(column_count < enum_emlx_source_file_column || column_count < enum_emlx_source_count_name_column)
        return;


    QTableWidgetItem *item_source_file = ui->tableWidget_emails->item(row,enum_emlx_source_file_column);
    if(item_source_file == NULL)
        return;

    QString source_filepath = item_source_file->text().trimmed();
    if(source_filepath.isEmpty())
        return;


    QTableWidgetItem *item_source_count_name = ui->tableWidget_emails->item(row,enum_emlx_source_count_name_column);
    if(item_source_count_name == NULL)
        return;

    QString source_count_name = item_source_count_name->text().trimmed();

    QTableWidgetItem *item_file_name = ui->tableWidget_emails->item(row,enum_emlx_file_name_column);
    if(item_file_name == NULL)
        return;

    QTableWidgetItem *source_file_str = ui->tableWidget_emails->item(row,enum_emlx_source_file_column);
    if(source_file_str == NULL)
        return;

    if(item_file_name->text().endsWith(".pst") || source_file_str->text().endsWith(".mbox"))
    {
        if(QList_struct_mail_pst_and_mbox_file_info_details.size() <= 0)
            return;

        QTableWidgetItem *item_INT = ui->tableWidget_emails->item(row,enum_emlx_sr_no_column);
        QString mail_pst_sno = item_INT->text();
        QTableWidgetItem *item_acc_child_name = ui->tableWidget_emails->item(row,enum_emlx_mbox_column);
        QString acc_child_name = item_acc_child_name->text();

        for(int ii = 0; ii < QList_struct_mail_pst_and_mbox_file_info_details.size(); ii++)
        {
            QString record_int_frm_qlist = QList_struct_mail_pst_and_mbox_file_info_details.at(ii).record_int;
            QString acc_child_nme_frm_qlist = QList_struct_mail_pst_and_mbox_file_info_details.at(ii).folder_name;
            QString src_name_frm_list = QList_struct_mail_pst_and_mbox_file_info_details.at(ii).source_count_name;

            if(mail_pst_sno == record_int_frm_qlist && acc_child_name == acc_child_nme_frm_qlist && source_count_name == src_name_frm_list)
            {
                QByteArray file_rw_byte = QList_struct_mail_pst_and_mbox_file_info_details.at(ii).file_data_bytes;

                std::string std_str(file_rw_byte.constData(), file_rw_byte.length());
                QString data_str = QString::fromStdString(std_str);
                ui->textBrowser_raw_data->setText(data_str);

                break;
            }
        }
    }
    else
    {
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        source_filepath = struct_source_info.virtual_source_path + source_filepath;

        QFile myFile(source_filepath);

        if(!myFile.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("File Open -----FAILED---- " + myFile.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + myFile.errorString(), Q_FUNC_INFO);
            return;
        }

        QByteArray data_arr = myFile.readAll();
        myFile.close();

        std::string std_str(data_arr.constData(), data_arr.length());
        QString data_str = QString::fromStdString(std_str);


        ui->textBrowser_raw_data->setText(data_str);
    }
    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void emlx_mail::on_pushButton_search_clicked()
{

    //=============(Temp)-Start
    //on_pushButton_advanced_search_clicked();
    //return;
    //=============(Temp)-End


    highlight_searched_text_in_result.clear();

    ui->label_emlx_file_path->clear();
    ui->textBrowser_message->clear();
    ui->textBrowser_raw_data->clear();
    ui->lineEdit_raw_data_search->clear();
    ui->lineEdit_message_search->clear();
    ui->tableWidget_attachments->setRowCount(0);

    QString search_text = ui->lineEdit_search->text().toLower().trimmed();

    if(search_text.isEmpty())
        return;


    QList<QAction *> menu_actions_list = menu_search_filters->actions();
    if(menu_actions_list.isEmpty())
        return;

    ui->textBrowser_message->clear();

    /// hide every row first
    for(int row = 0; row < ui->tableWidget_emails->rowCount(); row++)
    {
        ui->tableWidget_emails->hideRow(row);
    }


    QString search_cmd;
    bool bool_any_action_checked = false;

    search_text.prepend("%");
    search_text.append("%");

    QStringList record_count_list;
    for(int count = 0; count < menu_actions_list.size(); count++)
    {
        if(!menu_actions_list.at(count)->isChecked())
            continue;

        bool_any_action_checked = true;

        QString action_text_str = menu_actions_list.at(count)->text().trimmed();

        if(action_text_str == QString("To"))
        {
            search_cmd = QString("Select INT from emlx_data Where emlx_to LIKE '" + search_text + "'");
            record_count_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(search_cmd, 0, source_db_path, Q_FUNC_INFO);
        }
        else if(action_text_str == QString("From"))
        {
            search_cmd = QString("Select INT from emlx_data Where emlx_from LIKE '" + search_text + "'");
            record_count_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(search_cmd, 0, source_db_path, Q_FUNC_INFO);
        }
        else if(action_text_str == QString("Cc"))
        {
            search_cmd = QString("Select INT from emlx_data Where emlx_cc LIKE '" + search_text + "'");
            record_count_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(search_cmd, 0, source_db_path, Q_FUNC_INFO);
        }
        else if(action_text_str == QString("Subject"))
        {
            search_cmd = QString("Select INT from emlx_data Where emlx_subject LIKE '" + search_text + "'");
            record_count_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(search_cmd, 0, source_db_path, Q_FUNC_INFO);
        }
        else if(action_text_str == QString("Message Body"))
        {
            search_cmd = QString("Select INT from emlx_data Where emlx_msg_body LIKE '" + search_text + "'");
            record_count_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(search_cmd, 0, source_db_path, Q_FUNC_INFO);
        }
        else if(action_text_str == QString("Sr. No."))
        {
            QString tmp_search_text = search_text;
            tmp_search_text.replace("%", "");
            tmp_search_text = tmp_search_text.trimmed();

            search_cmd = QString("Select INT from emlx_data Where INT='" + tmp_search_text + "'");

            record_count_list << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(search_cmd, 0, source_db_path, Q_FUNC_INFO);
        }
    }

    record_count_list.removeDuplicates();

    if(!bool_any_action_checked || record_count_list.isEmpty())
        return;


    for(int row = 0; row < ui->tableWidget_emails->rowCount(); row++)
    {
        QString sr_no_str = ui->tableWidget_emails->item(row, enum_emlx_sr_no_column)->text();

        if(record_count_list.contains(sr_no_str))
            ui->tableWidget_emails->showRow(row);

    }
}


qint64 emlx_mail::get_header_column_index(QString header)
{

    int index = -1;
    for(int count = 0; count < display_header_name_list.size(); count++)
    {
        if(header == display_header_name_list.at(count).trimmed())
        {
            index = count;
            break;
        }
    }

    return index;
}


void emlx_mail::on_pushButton_show_all_clicked()
{
    highlight_searched_text_in_result.clear();

    ui->lineEdit_search->clear();
    ui->label_emlx_file_path->clear();
    ui->textBrowser_message->clear();

    ui->lineEdit_raw_data_search->clear();
    ui->textBrowser_raw_data->clear();
    ui->lineEdit_message_search->clear();
    ui->tableWidget_attachments->setRowCount(0);

    ui->tabWidget_details->setCurrentIndex(0);

    /// show every row first
    emlx_show_data_table_widget();

}


bool emlx_mail::check_item_already_exist_in_complete_tree(QString item_text , bool parent_object)
{
    bool item_found = false;

    QTreeWidgetItemIterator m_iter(ui->treeWidget_accounts);
    while(*m_iter)
    {
        if ((*m_iter)->text(0) == item_text)
        {

            if(parent_object)
                parent_item = *m_iter;

            item_found = true;
            break;
        }

        ++m_iter;
    }


    return item_found;
}

bool emlx_mail::check_item_already_exist_in_specified_tree(QTreeWidgetItem *item_tree, QString item_text , bool parent_object)
{
    bool item_found = false;

    QTreeWidgetItemIterator m_iter(item_tree);
    while(*m_iter)
    {
        if ((*m_iter)->text(0) == item_text)
        {
            if(parent_object)
                parent_mbox_item = *m_iter;

            item_found = true;
            break;
        }
        ++m_iter;
    }

    return item_found;
}

QStringList emlx_mail::get_index_of_mbox_from_path(QStringList filepath_list)
{
    QStringList mboxes_index_list;

    for(int count = 0; count < filepath_list.size(); count++)
    {
        if(filepath_list.at(count).endsWith(".mbox"))
        {
            mboxes_index_list << QString::number(count);
        }
    }

    return mboxes_index_list;
}


void emlx_mail::on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->label_emlx_file_path->clear();

    ui->textBrowser_message->clear();
    ui->tableWidget_attachments->setRowCount(0);
    ui->textBrowser_raw_data->clear();

    if(item == NULL)
        return;

    ui->tableWidget_emails->blockSignals(true);

    if(column == 0)
    {
        QString current_mbox    = item->text(column);
        QString current_account = item->text(column + 1);

        if(!current_account.trimmed().isEmpty())
        {
            for(int row = 0; row < ui->tableWidget_emails->rowCount(); row++)
            {
                QTableWidgetItem *item_acc_id = ui->tableWidget_emails->item(row , enum_emlx_account_id_column);
                QTableWidgetItem *item_mbox   = ui->tableWidget_emails->item(row , enum_emlx_mbox_column);

                if(item_acc_id == NULL || item_mbox == NULL)
                    continue;

                if(item_mbox->text().contains(current_mbox) && item_acc_id->text() == current_account)
                    ui->tableWidget_emails->showRow(row);
                else
                    ui->tableWidget_emails->hideRow(row);
            }
        }
        else
        {
            /// on account id basis
            for(int row = 0; row < ui->tableWidget_emails->rowCount(); row++)
            {
                QTableWidgetItem *item = ui->tableWidget_emails->item(row , enum_emlx_account_id_column);

                if(item == NULL)
                    continue;

                if(item->text() == current_mbox)
                    ui->tableWidget_emails->showRow(row);
                else
                    ui->tableWidget_emails->hideRow(row);


            }
        }
    }

    ui->tableWidget_emails->blockSignals(false);

}

///for creating actions on right click
void emlx_mail::create_right_click_actions(QMenu *mainMenu , QStringList submenuList)
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


void emlx_mail::slot_quicklook_link_clicked(QString link_text)
{
    QString filepath = QString::fromLocal8Bit(QByteArray::fromPercentEncoding(link_text.toLocal8Bit()));

    emit signal_quick_look_preview_filepath_from_email_files(filepath);

    emit signal_act_quick_look_triggered();
}

void emlx_mail::slot_open_in_finder_link_clicked(QString link_text)
{
    QString filepath = QString::fromLocal8Bit(QByteArray::fromPercentEncoding(link_text.toLocal8Bit()));

    QStringList arg;
    arg << "-R"<< filepath;

    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}



void emlx_mail::on_lineEdit_message_search_returnPressed()
{
    on_pushButton_search_message_clicked();
}

void emlx_mail::on_pushButton_search_message_clicked()
{
    QString searchString = ui->lineEdit_message_search->text();

    if(searchString.trimmed().isEmpty())
        return;

    QTextDocument *document = ui->textBrowser_message->document();
    if(document == NULL)
        return;

    bool found = false;

    ui->textBrowser_message->setUndoRedoEnabled(true);

    document->undo();

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    qint64 m_counter = 0;

    while(!highlightCursor.isNull() && !highlightCursor.atEnd())
    {
        if(m_counter % 50 == 0)
        {
            m_counter = 0;
            QCoreApplication::processEvents();
        }
        m_counter++;

        highlightCursor = document->find(searchString, highlightCursor);

        if (!highlightCursor.isNull())
        {
            found = true;

            highlightCursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }

    //    if(!list_highlight_cursor.isEmpty())
    //    {
    //        QTextCursor scrollCursor = list_highlight_cursor.at(0);

    //        ui->textBrowser_message->setTextCursor(scrollCursor);

    //        QPalette pallete = ui->textBrowser_message->palette();
    //        pallete.setColor(QPalette::Highlight, QColor(Qt::yellow));
    //        pallete.setColor(QPalette::HighlightedText, QColor(Qt::red));
    //        ui->textBrowser_message->setPalette(pallete);
    //    }

    cursor.endEditBlock();
}

void emlx_mail::on_tableWidget_emails_itemSelectionChanged()
{
    on_tableWidget_emails_cellPressed(ui->tableWidget_emails->currentRow(), ui->tableWidget_emails->currentColumn());
}

void emlx_mail::on_tableWidget_emails_itemClicked(QTableWidgetItem *item)
{
    if(item->column() == enum_emlx_bookmark_column)
    {
        ui->tableWidget_emails->selectRow(item->row());
        if(item->checkState() == Qt::Checked)
            action_bookmark_triggered();
        else
            action_remove_bookmarks_triggered();
    }
}

void emlx_mail::on_lineEdit_message_search_textChanged(const QString &arg1)
{
    if(arg1.trimmed().isEmpty())
    {
        QTextDocument *document = ui->textBrowser_message->document();
        document->undo();
    }
}

void emlx_mail::on_lineEdit_raw_data_search_textChanged(const QString &arg1)
{
    if(arg1.trimmed().isEmpty())
    {
        QTextDocument *document = ui->textBrowser_raw_data->document();
        document->undo();
    }

}

void emlx_mail::on_lineEdit_raw_data_search_returnPressed()
{
    on_pushButton_raw_data_search_clicked();
}

void emlx_mail::on_pushButton_raw_data_search_clicked()
{
    QString searchString = ui->lineEdit_raw_data_search->text().trimmed();

    if(searchString.isEmpty())
        return;


    QTextDocument *document = ui->textBrowser_raw_data->document();
    if(document == NULL)
        return;

    ui->textBrowser_raw_data->setUndoRedoEnabled(true);

    document->undo();

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    qint64 m_counter = 0;

    while(!highlightCursor.isNull() && !highlightCursor.atEnd())
    {
        if(m_counter % 50 == 0)
        {
            m_counter = 0;
            QCoreApplication::processEvents();
        }
        m_counter++;

        highlightCursor = document->find(searchString, highlightCursor);

        if(!highlightCursor.isNull())
        {
            highlightCursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);

        }
    }

    //    if(!list_highlight_cursor.isEmpty())
    //    {
    //        QTextCursor scrollCursor = list_highlight_cursor.at(0);

    //        ui->textBrowser_raw_data->setTextCursor(scrollCursor);

    //        QPalette pallete = ui->textBrowser_raw_data->palette();
    //        pallete.setColor(QPalette::Highlight, QColor(Qt::yellow));
    //        pallete.setColor(QPalette::HighlightedText, QColor(Qt::red));

    //        ui->textBrowser_raw_data->setPalette(pallete);
    //    }

    cursor.endEditBlock();

}

void emlx_mail::on_tableWidget_attachments_itemSelectionChanged()
{
    int row = ui->tableWidget_attachments->currentRow();
    if(row >= 0)
    {
        QString attachment_filename = ui->tableWidget_attachments->item(row,0)->text().trimmed();

        QString file_path = attachment_dir_path + attachment_filename;

        QFileInfo info(file_path);
        if(info.exists())
        {

            QString file_mime_type = mime_database_obj.mimeTypeForFile(file_path).name();

            if(file_mime_type.startsWith("video/"))
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_audio_video_preview_index);
                av_preview_obj->load_source(file_path);
            }
            else if (file_mime_type.startsWith("audio/"))
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_audio_video_preview_index);
                av_preview_obj->load_source(file_path);
            }
            else if(recon_helper_standard_obj->check_is_it_image_file(file_path,file_path, Q_FUNC_INFO))
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_image_preview_index);
                image_preview_obj->set_image_by_path(file_path);
            }
            else
            {
                ui->stackedWidget_preview->setCurrentIndex(enum_no_preview_index);
            }
        }
    }
    else
    {
        ui->stackedWidget_preview->setCurrentIndex(enum_no_preview_index);
    }

}

void emlx_mail::slot_sort_tablewidget_display(int column_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);


    if(ui->tableWidget_emails == NULL)
        return;



    //Return if clicked column index other then Bookmark, Tags
    if(column_index != enum_emlx_bookmark_column && column_index != enum_emlx_tag_column)
    {
        ui->tableWidget_emails->sortByColumn(column_index,Qt::AscendingOrder);
        ui->tableWidget_emails->setSortingEnabled(false);
        return;
    }

    ui->tableWidget_emails->clearSelection();

    if(ui->tableWidget_emails->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = ui->tableWidget_emails->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < ui->tableWidget_emails->rowCount(); row++)
        {
            if(ui->tableWidget_emails->item(row, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_emlx_bookmark_column)
                icon_set =  ui->tableWidget_emails->item(row, column_index)->icon();



            ui->tableWidget_emails->item(row, column_index)->checkState();

            // Check for checkstate OR iCon
            if(ui->tableWidget_emails->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column_index != enum_emlx_bookmark_column)
                    icon_shiftrow_acs =  ui->tableWidget_emails->item(row_shift_pos_ascending, column_index)->icon();

                // If row is already on 0th position then increment and continue
                if(ui->tableWidget_emails->item(row_shift_pos_ascending, column_index)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = takeRow(destRow);

                // Shifting of rows from lower to upper
                setRow(destRow, sourceItems);
                setRow(sourceRow, destItems);


                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(ui->tableWidget_emails->item(row, column_index) == NULL)
                continue;

            if(ui->tableWidget_emails->item(row_shift_pos_descending, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_emlx_bookmark_column)
                icon_set =  ui->tableWidget_emails->item(row, column_index)->icon();

            if(ui->tableWidget_emails->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column_index != enum_emlx_bookmark_column)
                    icon_shiftrow_desc =  ui->tableWidget_emails->item(row_shift_pos_descending, column_index)->icon();

                // If row is already on last position then decrement and continue
                if(ui->tableWidget_emails->item(row_shift_pos_descending, column_index)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = takeRow(destRow);

                // Shifting of rows from upper to lower
                setRow(destRow, sourceItems);
                setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

QList<QTableWidgetItem*> emlx_mail::takeRow(int row)
{
    QList<QTableWidgetItem*> rowItems;
    for (int col = 0; col < ui->tableWidget_emails->columnCount(); ++col)
    {
        rowItems << ui->tableWidget_emails->takeItem(row, col);
    }
    return rowItems;
}

// sets the whole row
void emlx_mail::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
    for (int col = 0; col < ui->tableWidget_emails->columnCount(); ++col)
    {
        ui->tableWidget_emails->setItem(row, col, rowItems.at(col));
    }
}

void emlx_mail::slot_cancel_loading_search_results(bool status)
{
    bool_cancel_loading_search_results = status;
    display_loading_progress_bar_obj->hide();
}

void emlx_mail::slot_email_search_work_done(QString command)
{
    email_search_obj->hide();

    ui->tableWidget_emails->setRowCount(0);
    //ui->tableWidget_emails->hideColumn(enum_emlx_sr_no_column);

    ui->stackedWidget_preview->setCurrentIndex(enum_no_preview_index);

    ui->textBrowser_message->clear();
    QTextCursor cursor = ui->textBrowser_message->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textBrowser_message->setTextCursor(cursor);
    ui->textBrowser_message->append("\n\n\n\n\n\n");
    ui->textBrowser_message->append("<font size = 70>No Email Selected</font>");

    int row = ui->tableWidget_emails->rowCount();

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(source_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    query.addBindValue(command);

    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
        source_db.close();
        return;
    }

    bool_cancel_loading_search_results = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    qint64 m_counter = 0;

    while(query.next())
    {

        if(m_counter % 50 == 0)
        {
            m_counter = 0;
            QCoreApplication::processEvents();
        }
        m_counter++;

        if(bool_cancel_loading_search_results)
        {
            break;
        }

        QString sr_no_str    = query.value(0).toString().trimmed();
        QString bookmark_str = query.value(1).toString().trimmed();
        QString to_str       = query.value(2).toString().trimmed();
        QString from_str     = query.value(3).toString().trimmed();
        QString cc_str       = query.value(4).toString().trimmed();
        QString datetime_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(5).toString().trimmed(), Q_FUNC_INFO);
        QString subject_str  = query.value(6).toString().trimmed();
        QString attachment_str   = query.value(8).toString().trimmed();
        QString acc_id_str       = query.value(9).toString().trimmed();
        QString message_box_str  = query.value(10).toString().trimmed();
        QString source_file_str  = query.value(11).toString().trimmed();
        QString tag_value  = query.value(12).toString().trimmed();
        QString source_count_name  = query.value(13).toString().trimmed();


        QTableWidgetItem *item_tag        = new QTableWidgetItem;
        QTableWidgetItem *item_sr_no      = new QTableWidgetItem;
        QTableWidgetItem *item_bookmark   = new QTableWidgetItem;
        QTableWidgetItem *item_to         = new QTableWidgetItem;
        QTableWidgetItem *item_from       = new QTableWidgetItem;
        QTableWidgetItem *item_cc         = new QTableWidgetItem;
        QTableWidgetItem *item_datetime   = new QTableWidgetItem;
        QTableWidgetItem *item_subject    = new QTableWidgetItem;
        QTableWidgetItem *item_attachment = new QTableWidgetItem;
        QTableWidgetItem *item_account_id = new QTableWidgetItem;
        QTableWidgetItem *item_mbox       = new QTableWidgetItem;
        QTableWidgetItem *item_source       = new QTableWidgetItem;
        QTableWidgetItem *item_source_count = new QTableWidgetItem;

        item_sr_no->setData(Qt::EditRole, sr_no_str.toLongLong());
        item_sr_no->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        item_to->setText(to_str);
        item_from->setText(from_str);
        item_cc->setText(cc_str);
        item_datetime->setText(datetime_str);
        item_subject->setText(subject_str);
        item_account_id->setText(acc_id_str);
        item_mbox->setText(message_box_str);
        item_source->setText(source_file_str);

        if(attachment_str.trimmed().isEmpty())
            item_attachment->setData(Qt::EditRole, QString::number(0).toLongLong());
        else
            item_attachment->setData(Qt::EditRole, QString::number(attachment_str.split(",").size()).toLongLong());

        item_attachment->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        if(bookmark_str == QString::number(1))
            item_bookmark->setCheckState(Qt::Checked);
        else
            item_bookmark->setCheckState(Qt::Unchecked);


        item_source_count->setText(source_count_name);

        //ui->tableWidget_emails->hideColumn(0);
        ui->tableWidget_emails->insertRow(row);
        ui->tableWidget_emails->setRowHeight(row,22);


        ui->tableWidget_emails->setItem(row, enum_emlx_sr_no_column,item_sr_no);
        ui->tableWidget_emails->setItem(row, enum_emlx_bookmark_column,item_bookmark);
        if(tag_value.isEmpty())
        {
            QIcon icon("");
            item_tag->setIcon(icon);
            ui->tableWidget_emails->setItem(row, enum_emlx_tag_column,item_tag);
        }
        else
        {
            QString icon_str = tag_value;
            icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_value,case_confg_db_path , Q_FUNC_INFO);
            icon_str = icon_str.toLower();
            icon_str = icon_str.replace(" ","_");
            QIcon icon("../icons/tag_icons/"+ icon_str +".png");
            item_tag->setIcon(icon);

            ui->tableWidget_emails->setItem(row, enum_emlx_tag_column,item_tag);

        }
        ui->tableWidget_emails->setItem(row, enum_emlx_to_column,item_to);
        ui->tableWidget_emails->setItem(row, enum_emlx_from_column,item_from);
        ui->tableWidget_emails->setItem(row, enum_emlx_Cc_column,item_cc);
        ui->tableWidget_emails->setItem(row, enum_emlx_subject_column,item_subject);
        ui->tableWidget_emails->setItem(row, enum_emlx_datetime_column,item_datetime);
        ui->tableWidget_emails->setItem(row, enum_emlx_attachments_column,item_attachment);
        ui->tableWidget_emails->setItem(row, enum_emlx_account_id_column, item_account_id);
        ui->tableWidget_emails->setItem(row, enum_emlx_mbox_column, item_mbox);
        ui->tableWidget_emails->setItem(row, enum_emlx_source_file_column, item_source);
        ui->tableWidget_emails->setItem(row, enum_emlx_source_count_name_column, item_source_count);
    }
    source_db.close();

    ui->tableWidget_emails->setColumnWidth(enum_emlx_tag_column , 30);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_bookmark_column,25);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_to_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_from_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_Cc_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_subject_column,160);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_datetime_column,200);
    ui->tableWidget_emails->setColumnWidth(enum_emlx_attachments_column,100);

    display_loading_progress_bar_obj->hide();

    if(ui->tableWidget_emails->rowCount() > 0)
        ui->tableWidget_emails->selectRow(0);
    else
    {
        ui->lineEdit_message_search->clear();
        ui->lineEdit_raw_data_search->clear();
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No result found.");
        return;
    }

}

void emlx_mail::slot_highlight_searched_text(QString email_searched_text)
{
    highlight_searched_text_in_result = email_searched_text;

    ui->lineEdit_message_search->setText(email_searched_text);
    ui->lineEdit_raw_data_search->setText(email_searched_text);
}

void emlx_mail::on_pushButton_advanced_search_clicked()
{
    highlight_searched_text_in_result.clear();

    ui->label_emlx_file_path->clear();
    ui->textBrowser_message->clear();
    ui->textBrowser_raw_data->clear();
    ui->lineEdit_raw_data_search->clear();
    ui->lineEdit_message_search->clear();
    ui->tableWidget_attachments->setRowCount(0);

    email_search_obj->pub_load_defaults();
    email_search_obj->show();
}

void emlx_mail::slot_textBrowser_raw_data_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->textBrowser_raw_data->mapToGlobal(pos);
    menu_raw_data->exec(globalPos);
}

void emlx_mail::create_raw_data_tags_submenu()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_raw_data_tags == NULL)
        return;

    sub_menu_raw_data_tags->clear();

    ///add new tag
    act_raw_data_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_raw_data_create_tag->setIcon(icon_create);
    sub_menu_raw_data_tags->addAction(act_raw_data_create_tag);

    disconnect(act_raw_data_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));
    connect(act_raw_data_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));


    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_raw_data_tags->addAction(separator_main);

    add_raw_data_tag_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_raw_data_tag_module_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                sub_menu_raw_data_tags->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";
            QString tag_str  = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_raw_data_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);
}

void emlx_mail::action_submenu_raw_data_tag_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;


    QString selected_color = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("SELECT selected_colour FROM tags Where tag_name = ?",QStringList(tag_name) ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);
    update_raw_data_tags_value(tag_name , selected_color);


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void emlx_mail::update_raw_data_tags_value(QString tag_data,QString colour)
{

    QTextCursor cursor = ui->textBrowser_raw_data->textCursor();
    QString selected_text = cursor.selectedText();
    if(selected_text.trimmed().isEmpty())
        return ;

    int row = ui->tableWidget_emails->currentRow();
    if(row < 0)
        return;

    QString current_filepath  = ui->tableWidget_emails->item(row, enum_emlx_source_file_column)->text();
    QString record_no         = ui->tableWidget_emails->item(row, enum_emlx_sr_no_column)->text();
    QString source_count_name = ui->tableWidget_emails->item(row, enum_emlx_source_count_name_column)->text();


    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString cmplt_source_filepath = struct_source_info.virtual_source_path + current_filepath;


    create_table_export_content_for_raw_data_tag();

    QFileInfo file_info(current_filepath);

    QString display_file_path =  current_filepath;
    QString recon_filefrom =  QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);
    QString recon_file_info_path = current_filepath;


    QString file_name = file_info.fileName();
    QString content_file_name_str = file_name;


    QString suffix_str = file_info.suffix();
    if(content_file_name_str.endsWith(suffix_str))
        content_file_name_str.chop(suffix_str.size());

    if(content_file_name_str.endsWith("."))
        content_file_name_str.chop(1);


    QString content_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString();
    content_file_name_str = recon_static_functions::get_available_filename(content_file_name_str, content_file_path, Q_FUNC_INFO);
    content_file_path = content_file_path + content_file_name_str;


    QFile myfile(content_file_path);
    if(!myfile.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + content_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }
    myfile.write(selected_text.toLocal8Bit());
    myfile.flush();
    myfile.close();



    QList<struct_global_tagging_string_viewer_record> record_list;
    struct_global_tagging_string_viewer_record s1;
    s1.INT_record  = record_no;

    s1.plugin_name = QString(MACRO_Plugin_Name_Email_Parser_Tag);

    s1.tab_name    = QString(MACRO_Plugin_Email_Parser_Tag_TAB_Name_Email_Parser_Tag_Data);
    s1.item0 = file_name;
    s1.item1 = display_file_path;
    s1.item2 = selected_text;
    s1.item3 = content_file_name_str;
    s1.item4 = QString::number(QFileInfo(cmplt_source_filepath).size());
    s1.source_count_name = source_count_name;
    s1.hex_color = colour;
    s1.export_file_path = current_filepath;
    s1.recon_filefrom   = recon_filefrom;
    s1.recon_file_infopath = recon_file_info_path;
    record_list << s1;

    //String and Text viewer functions ar used for EMLX viewer.
    global_connection_manager_obj->pub_add_data_in_export_content_string_viewer_and_text_viewer_db(record_list ,tag_data ,MACRO_Plugin_Email_Parser_TAB_Name_Email_Data);

    global_connection_manager_obj->pub_add_tag_for_emlx_viewer_raw_data(record_list , tag_data);

}

void emlx_mail::create_table_export_content_for_raw_data_tag()
{
    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString() + "email_tag_data.sqlite";
    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'emlx_content_info'(INT INTEGER PRIMARY KEY,record_no VARCHAR(100) , bookmark VARCHAR(10), file_name VARCHAR(500), file_path VARCHAR(500),"
                          "file_size VARCHAR(100), text_data VARCHAR(1000), item2 VARCHAR(100) , item3 VARCHAR(100), item4 VARCHAR(100) , item5 VARCHAR(100),"
                          "recon_tag_value VARCHAR(50), extended_attributes_key_value VARCHAR(100),"
                          "color_hex VARCHAR(50), content_file_path VARCHAR(100),export_file_path VARCHAR(100) , content_file_name VARCHAR(100) ,source_count_name VARCHAR(100), notes VARCHAR(100), LINKFILENAME VARCHAR(1000),"
                          "source_file VARCHAR(100),recon_filefrom VARCHAR(100),recon_file_infopath VARCHAR(100))";
        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table emlx_content_info in emlx viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }

        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);

    }
}




