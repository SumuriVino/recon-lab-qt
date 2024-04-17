#include "chat_viewer.h"

chat_viewer::chat_viewer(QWidget *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

chat_viewer::~chat_viewer()
{

}

void chat_viewer::create_ui()
{
    treewidget_configured_account = new m_treewidget(this);
    treewidget_configured_account->setStyleSheet("QTreeWidget {"
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

    treewidget_configured_account->setColumnCount(3);
    QStringList header_list;
    header_list << "Accounts" <<"File Path" << "Stack Index";
    treewidget_configured_account->setHeaderLabels(QStringList(header_list));
    treewidget_configured_account->hideColumn(1);
    treewidget_configured_account->hideColumn(2);
    treewidget_configured_account->setHeaderHidden(true);

    treewidget_configured_account->setMinimumWidth(300);

    stackedwidget_bubble_message_obj = new m_stackedwidget(this);
    stackedwidget_bubble_message_obj->setFrameShape(QFrame::StyledPanel);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(treewidget_configured_account);
    hlayout->addWidget(stackedwidget_bubble_message_obj);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(0);
    hlayout->setStretchFactor(stackedwidget_bubble_message_obj,1);

    connect(treewidget_configured_account,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_treewidget_configured_account_single_clicked(QTreeWidgetItem*,int)));
}

void chat_viewer::pub_set_display_chat_view_in_plugins_tab()
{

    QString chat_view_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin_name + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Chat_View_Page_QString;

    QTreeWidgetItem *item_parent;
    int stack_count = 0;

    QDirIterator itr_config(chat_view_dir,QDirIterator::NoIteratorFlags);
    while(itr_config.hasNext())
    {
        QFileInfo info(itr_config.next());
        if(!info.fileName().contains("Configured_Account") || !info.isDir())
            continue;

        QString chat_buddy_name;
        QString html_file;

        QString account_dir_path = info.filePath();

        QString configured_account = get_configured_account(account_dir_path);
        item_parent = new QTreeWidgetItem;
        item_parent->setCheckState(0,Qt::Unchecked);
        item_parent->setText(0,configured_account);

        QDirIterator itr_buddy(account_dir_path,QDirIterator::NoIteratorFlags);
        while(itr_buddy.hasNext())
        {

            QFileInfo info(itr_buddy.next());
            if(!info.fileName().contains("chat_buddy") || !info.isDir())
                continue;

            QString chat_buddy_dir = info.filePath();

            chat_buddy_name = get_chat_buddy_name(chat_buddy_dir);      //file info
            html_file = get_html_file(chat_buddy_dir);  //html file


            QDir make_dir;
            QString resources_dir = chat_buddy_dir + "/resources/";
            make_dir.mkdir(resources_dir);
            QFile::copy(recon_static_functions::get_appilcation_relative_path() + "/Resources/graphics/html_reports/css/chat_template.css" , resources_dir + "/chat_template.css");


            QString chat_viewer_bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
            QStringList arg_list;
            arg_list << chat_buddy_name << configured_account;
            QString bookmark_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select bookmark from chat_viewer_info where chat_buddy = ? AND configured_account= ?" , arg_list ,0, chat_viewer_bookmark_db_path , Q_FUNC_INFO);
            QString status;
            if(bookmark_status.trimmed().isEmpty())
                status = "0";
            else
                status = "1";

            QTreeWidgetItem *item_child = new QTreeWidgetItem;

            if(status == "0")
                item_child->setCheckState(0,Qt::Unchecked);
            else if(status == "1")
                item_child->setCheckState(0,Qt::Checked);

            item_child->setText(0,chat_buddy_name);
            item_child->setText(1,html_file);
            item_child->setText(2,QString::number(stack_count));
            item_parent->addChild(item_child);

            QWebEngineView *view = new QWebEngineView(stackedwidget_bubble_message_obj);
            view->setContextMenuPolicy(Qt::NoContextMenu);
    //        view->load(QUrl("file://" +  html_file));
            view->setUrl(QUrl("file://" +  html_file));

            stackedwidget_bubble_message_obj->insertWidget(stack_count,view);
            stack_count++;
            treewidget_configured_account->addTopLevelItem(item_parent);
        }
    }

    stackedwidget_bubble_message_obj->setCurrentIndex(0);
    treewidget_configured_account->expandAll();

    if(treewidget_configured_account->topLevelItemCount() > 0)
    {
        if(treewidget_configured_account->topLevelItem(0)->childCount() > 0)
        {
            treewidget_configured_account->topLevelItem(0)->child(0)->setSelected(true);
        }

        treewidget_configured_account->sortItems(treewidget_configured_account->sortColumn(),Qt::AscendingOrder);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

QString chat_viewer::get_configured_account(QString account_dir_path)
{
    account_dir_path.append("/configured_account.txt");

    QString configured_account;

    QFileInfo info(account_dir_path);
    if(!info.exists())
        return configured_account;

    QFile m_File;
    m_File.setFileName(info.filePath());
    if(!m_File.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File open ------FAILED------" + info.filePath(), Q_FUNC_INFO);
        recon_static_functions::app_debug("----error ------" + m_File.errorString(), Q_FUNC_INFO);
        return configured_account;
    }

    configured_account  = m_File.readLine();

    m_File.close();
    return configured_account.trimmed();
}

QString chat_viewer::get_chat_buddy_name(QString info_file_path)
{
    info_file_path.append("/chat_buddy_info.txt");

    QString chat_buddy;

    QFileInfo info(info_file_path);
    if(!info.exists())
        return chat_buddy;

    QFile m_File;
    m_File.setFileName(info_file_path);

    if(!m_File.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File open ------FAILED------" + info_file_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("----error ------" + m_File.errorString(), Q_FUNC_INFO);
        return chat_buddy;
    }

    chat_buddy = m_File.readLine();
    m_File.close();

    return chat_buddy.trimmed();
}

QString chat_viewer::get_html_file(QString html_dir_path)
{
    html_dir_path.append("/chat_buddy_data.html");

    QFileInfo info(html_dir_path);
    if(!info.exists())
        return "";

    return html_dir_path.trimmed();
}

void chat_viewer::pub_set_plugin_name(QString plugin)
{
    plugin_name = plugin;
    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + plugin.replace(" ", "_") + "/" + plugin.replace(" ", "_") + ".sqlite";

}

void chat_viewer::slot_treewidget_configured_account_single_clicked(QTreeWidgetItem *item, int column)
{


    QString chat_viewer_bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";


    int stack_index =  item->text(2).toInt();
    if(stack_index < 0)
        return;

    if(column != 0)
        return;

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE",Q_FUNC_INFO);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" open destination database ---FAILED----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }

    if(item->checkState(0) == Qt::Checked)
    {
        if(item->parent() != NULL)
        {
            QString parent_name = item->parent()->text(0);


            for(int tp = 0; tp < treewidget_configured_account->topLevelItemCount(); tp++)
            {
                if(treewidget_configured_account->topLevelItem(tp)->text(0) != parent_name)
                    continue;

                QString command = "insert into chat_viewer_info (bookmark , plugin_name ,tab_name, configured_account , file_name , chat_buddy) values(?,?,?,?,?,?)";
                QStringList arg_values ;
                arg_values << "1" << plugin_name << tab_name << item->parent()->text(0) << "chat_buddy_data.html" << item->text(0);
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_values,chat_viewer_bookmark_db_path,Q_FUNC_INFO);

                treewidget_configured_account->topLevelItem(tp)->setCheckState(0,Qt::Checked);
                break;
            }
        }
        else
        {

            for(int i = 0; i < item->childCount(); i++)
            {

                QString command = "insert into chat_viewer_info (bookmark , plugin_name ,tab_name, configured_account , file_name , chat_buddy) values(?,?,?,?,?,?)";
                QStringList arg_values ;
                arg_values << "1" << plugin_name << tab_name << item->text(0) << "chat_buddy_data.html" << item->child(i)->text(0);
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_values,chat_viewer_bookmark_db_path,Q_FUNC_INFO);

                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
    }
    else
    {
        if(item->parent() != NULL)
        {
            QString parent_name = item->parent()->text(0);
            for(int tp = 0; tp < treewidget_configured_account->topLevelItemCount(); tp++)
            {
                if(treewidget_configured_account->topLevelItem(tp)->text(0) != parent_name)
                    continue;

                QString command = "Delete from chat_viewer_info where  plugin_name = ? AND tab_name = ? AND configured_account = ? AND  chat_buddy = ?";
                QStringList arg_values;
                arg_values << plugin_name << tab_name <<item->parent()->text(0)  << item->text(0);
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_values,chat_viewer_bookmark_db_path,Q_FUNC_INFO);


                int checked_budy_acnt = 0;
                for(int ch = 0; ch < treewidget_configured_account->topLevelItem(tp)->childCount(); ch++)
                {
                    if(treewidget_configured_account->topLevelItem(tp)->child(ch)->checkState(0) != Qt::Checked)
                        continue;
                    checked_budy_acnt++;
                }

                if(checked_budy_acnt == 0)
                {
                    treewidget_configured_account->topLevelItem(tp)->setCheckState(0,Qt::Unchecked);
                    break;
                }
            }
        }
        else
        {


            for(int i = 0; i < item->childCount(); i++)
            {
                QString command = "Delete from chat_viewer_info where  plugin_name = ? AND tab_name = ? AND configured_account = ? AND chat_buddy = ?";
                QStringList arg_values;
                arg_values << plugin_name << tab_name << item->text(0)  << item->child(i)->text(0);

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_values,chat_viewer_bookmark_db_path,Q_FUNC_INFO);

                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
    }

    treewidget_configured_account->setCurrentItem(item);
    stackedwidget_bubble_message_obj->setCurrentIndex(stack_index);

    destination_db.close();


}

void chat_viewer::pub_set_tab_name(QString tab_name_str)
{
    tab_name = tab_name_str;

}





