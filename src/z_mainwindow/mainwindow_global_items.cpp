#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::common_right_click_global_report_finished_or_cancelled(QStringList report_paths)
{

    progress_bar_fixed_in_statusbar_obj->hide();
    message_dialog_object->pub_set_file_path_message_for_global_report(MACRO_MSG_REPORT_CREATE,report_paths);
    message_dialog_object->show();

}
void MainWindow::common_add_tag_name_into_case_tree(QString tag_name)
{
    recon_static_functions::debug_intensive(" start", Q_FUNC_INFO);


    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {

        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_TAGS)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Tags))
        {

            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_TAGS)->childCount(); j++)
            {
                QString tab_text = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_TAGS)->child(j)->text(enum_global_case_tree_display_column_ZERO);
                if(tab_text.contains("("))
                    tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());
                tab_text = tab_text.trimmed();

                if(tab_text == tag_name)
                {

                    int count = 0;
                    QString icon_path;
                    if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
                    {
                        QString bookmarks_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
                        count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, bookmarks_db_path, QStringList(), Q_FUNC_INFO);
                        icon_path = "../icons/checked.png";

                    }
                    else
                    {

                        QString tags_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
                        QString color_name = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tab_text,tags_db_path, Q_FUNC_INFO);

                        QString tag_color_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tab_text + ".sqlite";
                        QFile file(tag_color_db_path);
                        if(file.exists())
                        {
                            count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, tag_color_db_path, QStringList(), Q_FUNC_INFO);
                        }

                        icon_path = "../icons/tag_icons/"+ color_name.replace(" ","_").toLower() + ".png";


                    }


                    QTreeWidgetItem *sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_TAGS)->child(j);
                    sub_child->setText(enum_global_case_tree_display_column_ZERO, tab_text + " (" + QString::number(count) + ")");
                    sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
                    sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Tags));
                    sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                    return;

                }


            }

            QTreeWidgetItem *sub_child = new QTreeWidgetItem;

            QString icon_path;
            if(tag_name == MACRO_Tag_Name_Bookmarks)
            {
                icon_path = "../icons/checked.png";

                QString bookmarks_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
                int bookmark_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, bookmarks_db_path, QStringList(), Q_FUNC_INFO);

                ////if particular tag does not contain any records then text color of tag will becomes red.

                sub_child->setText(0, tag_name + " (" + QString::number(bookmark_count) + ")");
                sub_child->setIcon(0,QIcon(icon_path));
                sub_child->setText(1,QString(MACRO_CASE_TREE_Tags));
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            }
            else
            {

                QString tags_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
                QString color_name = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_name,tags_db_path, Q_FUNC_INFO);

                QString tag_color_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_name + ".sqlite";
                QFile file(tag_color_db_path);
                int tag_count = 0;
                if(file.exists())
                {
                    tag_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, tag_color_db_path, QStringList(), Q_FUNC_INFO);

                }
                icon_path = "../icons/tag_icons/"+ color_name.replace(" ","_").toLower() + ".png";

                sub_child->setText(0, tag_name + " (" + QString::number(tag_count) + ")");
                sub_child->setIcon(0,QIcon(icon_path));
                sub_child->setText(1,QString(MACRO_CASE_TREE_Tags));
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            }

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_TAGS)->addChild(sub_child);
            break;
        }
    }


    recon_static_functions::debug_intensive(" end", Q_FUNC_INFO);
}

void MainWindow::insert_tags_details_in_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug(" START ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName("../Resources/tags/recon_tags.sqlite");

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_source_hash_set_details(source_db);

    if(!query_source_hash_set_details.exec("select tag_name,selected_colour,is_default_tag,color_hex from tags"))
    {
        recon_static_functions::app_debug(" Source colours  ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_source_hash_set_details.lastError().text(),Q_FUNC_INFO);
    }
    while(query_source_hash_set_details.next())
    {
        QString command_insert = QString("insert into 'tags' (tag_name,selected_colour,is_default_tag,bookmark,color_hex) values(?,?,?,?,?)");

        QStringList list_value;
        list_value << query_source_hash_set_details.value(0).toString() << query_source_hash_set_details.value(1).toString();
        list_value << query_source_hash_set_details.value(2).toString() << QString::number(1);
        list_value << query_source_hash_set_details.value(3).toString();

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,list_value,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" Insert -----FAILED-----",Q_FUNC_INFO);
        }
    }



    source_db.close();

    recon_static_functions::app_debug(" END ",Q_FUNC_INFO);
}

void MainWindow::common_add_redefined_result_category_name_in_case_tree(QString category_name)
{

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
        {

            QString records_str;

            QString db_name = category_name;
            if(db_name.contains(" "))
                db_name.replace(" ","_");

            QString icon_path;
            QString saved_db_path;
            quint64 total_records_qint = 0;
            if(category_name == MACRO_Redefined_Result_Browser_History)
            {
                saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + db_name + ".sqlite";
                records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath("select count(*) from redefined_result where timestamp notnull AND timestamp != ''" ,0,saved_db_path,Q_FUNC_INFO);
                icon_path = "../icons/Features/browser_history_full.png";
            }
            else if(category_name == MACRO_Redefined_Result_Messenger)
            {
                saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + db_name + ".sqlite";
                records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath("select count(*) from redefined_result where timestamp notnull AND timestamp !=''" ,0,saved_db_path,Q_FUNC_INFO);
                icon_path = "../icons/Features/messenger_full.png";
            }

            QString total_records_str = records_str;

            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->childCount(); j++)
            {

                QString tab_name = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->child(j)->text(enum_global_case_tree_display_column_ZERO);
                if(tab_name.contains(category_name))
                {

                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->child(j)->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->child(j)->setText(enum_global_case_tree_display_column_ZERO, category_name + " (" + total_records_str + ")");
                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->child(j)->setText(enum_global_case_tree_display_column_RECORD_COUNT, total_records_str);
                    return;
                }
            }


            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
            sub_child->setText(enum_global_case_tree_display_column_ZERO, category_name + " (" + total_records_str + ")");
            sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Redefined_Result_Parent));
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, total_records_str);

            total_records_qint = total_records_str.toInt();

            if(total_records_qint != 0)
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT)->addChild(sub_child);
            break;
        }
    }
}


void MainWindow::common_add_notes_name_into_case_tree(QString note_name)
{
    QTreeWidgetItem *sub_child;
    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    int notes_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, notes_db_path, QStringList(), Q_FUNC_INFO);

    QString icon_path = "../icons/Features/notes.png";

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Notes_Parent))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->childCount(); j++)
            {

                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->child(j);
                sub_child->setText(0, note_name + " (" + QString::number(notes_count) + ")");
                sub_child->setIcon(0,QIcon(icon_path));
                sub_child->setText(1,QString(MACRO_CASE_TREE_Notes_Parent));
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                ////if particular tag does not contain any records then text color of tag will becomes red.
                //            if(notes_count == 0)
                //            {
                //               sub_child->setForeground(0, Qt::red);
                //            }

                return;
            }

            sub_child = new QTreeWidgetItem;
            sub_child->setText(0, note_name + " (" + QString::number(notes_count) + ")");
            sub_child->setIcon(0,QIcon(icon_path));
            sub_child->setText(1,QString(MACRO_CASE_TREE_Notes_Parent));
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->addChild(sub_child);
            break;
        }
    }
}

void MainWindow::common_add_screenshots_into_case_tree(QString screenshot_name)
{
    QTreeWidgetItem *sub_child;
    QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
    int screenshots_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, screenshot_db_path, QStringList(), Q_FUNC_INFO);

    QString icon_path = "../icons/Features/screenshots.png";

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SCREENSHOTS)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Screenshots))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SCREENSHOTS)->childCount(); j++)
            {

                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SCREENSHOTS)->child(j);
                sub_child->setText(0, screenshot_name + " (" + QString::number(screenshots_count) + ")");
                sub_child->setIcon(0,QIcon(icon_path));
                sub_child->setText(1,QString(MACRO_CASE_TREE_Screenshots));
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                return;
            }

            sub_child = new QTreeWidgetItem;
            sub_child->setText(0, screenshot_name + " (" + QString::number(screenshots_count) + ")");
            sub_child->setIcon(0,QIcon(icon_path));
            sub_child->setText(1,QString(MACRO_CASE_TREE_Screenshots));
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SCREENSHOTS)->addChild(sub_child);
            break;
        }
    }
}

void MainWindow::common_add_saved_graph_category_name_in_case_tree(QString category_name)
{
    QString icon_path , case_tree_category;
    int record_count = 0;
    QTreeWidgetItem *sub_child = new QTreeWidgetItem;

    if(category_name == MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs)
    {
        QString saved_graph_browser_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
        record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, saved_graph_browser_db_path, QStringList(), Q_FUNC_INFO);
        icon_path = "../icons/Features/graphs.png";
    }
    else if(category_name == MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs)
    {
        QString saved_graph_messenger_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
        record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, saved_graph_messenger_db_path, QStringList(), Q_FUNC_INFO);
        icon_path = "../icons/Features/graphs.png";
    }

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Graph_Result_Saved))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->childCount(); j++)
            {
                case_tree_category = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->child(j)->text(enum_global_case_tree_display_column_ZERO) ;
                if(case_tree_category.contains( "("))
                    case_tree_category = case_tree_category.remove(case_tree_category.lastIndexOf(" ("), case_tree_category.size());

                case_tree_category = case_tree_category.trimmed();
                if(case_tree_category == category_name)
                {
                    sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->child(j);
                    sub_child->setIcon(0,QIcon(icon_path));
                    sub_child->setText(0, category_name + " (" + QString::number(record_count) + ")");
                    sub_child->setText(1,QString(MACRO_CASE_TREE_Graph_Result_Saved));
                    sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                    return;
                }
            }


            sub_child->setIcon(0,QIcon(icon_path));
            sub_child->setText(0, category_name + " (" + QString::number(record_count) + ")");
            sub_child->setText(1,QString(MACRO_CASE_TREE_Graph_Result_Saved));
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());


            if(record_count != 0)
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->addChild(sub_child);

            break;
        }

    }
}

void MainWindow::common_add_story_board_into_case_tree(QString storyboard_name)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Story_Board))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->childCount(); j++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->child(j)->text(enum_global_case_tree_display_column_ZERO) == storyboard_name)
                {
                    return;
                }
            }

            QString icon_path = "../icons/Features/story_board.png";

            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(0, storyboard_name);
            sub_child->setIcon(0,QIcon(icon_path));

            sub_child->setText(1,QString(MACRO_CASE_TREE_Story_Board));
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_STORY_BOARD)->addChild(sub_child);
            // treewidget_case_display_obj->expandItem(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_CASE_TREE_CHILD_Story_Board));
            break;
        }
    }
}
void MainWindow::insert_keyword_category_and_list_in_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug("-Start",Q_FUNC_INFO);

    //    QStringList category_list;
    //    QStringList category_status_list;

    //    category_list << "Sumuri" << "Forenso";
    //    category_status_list << "1" << "1";


    //    for(int i = 0; i < category_list.size(); i++)
    //    {
    //        QString command_insert = QString("insert into 'keyword_category' (category,category_status) values(?,?)");

    //        QStringList list_value;
    //        list_value << category_list.at(i) << category_status_list.at(i);

    //        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,list_value,destination_db,Q_FUNC_INFO))
    //        {
    //            recon_static_functions::app_debug("Insert keyword_category -----FAILED-----",Q_FUNC_INFO);
    //        }
    //    }


    //    QStringList keyword_list_category;
    //    QStringList keyword_list;
    //    QStringList keyword_status_list;

    //    keyword_list_category << "Sumuri" << "Forenso" << "Sumuri" << "Forenso";
    //    keyword_list << "Guns" << "Crime" << "Bomb" << "Criminal";
    //    keyword_status_list << "1" << "1" << "1" << "1";

    //    for(int i = 0; i < keyword_list_category.size(); i++)
    //    {
    //        QString command_insert = QString("insert into 'keyword_list' (category,keyword,keyword_status, is_regex) values(?,?,?,0)");

    //        QStringList list_value;
    //        list_value << keyword_list_category.at(i) << keyword_list.at(i);
    //        list_value << keyword_status_list.at(i);

    //        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,list_value,destination_db,Q_FUNC_INFO))
    //        {
    //            recon_static_functions::app_debug("Insert keyword_list -----FAILED-----",Q_FUNC_INFO);
    //        }
    //    }

    recon_static_functions::app_debug("-End ",Q_FUNC_INFO);
}

void MainWindow::insert_index_files_filesize_default_details_in_db(QSqlDatabase destination_db)
{

    return;

    QString command_insert = QString("insert into 'index_files_filesize' (bookmark,size_label,size_value,size_unit) values(?,?,?,?)");
    QStringList value_list;
    value_list << QString::number(1)
               << QString(MACRO_Generic_Filter_Condition_Is_Greater_Than)
               << QString::number(10)
               << QString(MACRO_Generic_Filter_Condition_KB);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert, value_list , destination_db, Q_FUNC_INFO);

    value_list.clear();
    value_list << QString::number(1)
               << QString(MACRO_Generic_Filter_Condition_Is_Less_Than)
               << QString::number(20)
               << QString(MACRO_Generic_Filter_Condition_MB);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert, value_list , destination_db, Q_FUNC_INFO);

}

void MainWindow::insert_mime_type_details_in_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug("- START ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName("../Resources/features_resources/features_configuration.sqlite");

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source_db ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery query_source_hash_set_details(source_db);
    if(!query_source_hash_set_details.exec("select bookmark,category_name,mime_type from tbl_mime_type"))
    {
        recon_static_functions::app_debug(" Source mime_type  ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + query_source_hash_set_details.lastError().text(),Q_FUNC_INFO);
    }

    QStringList mime_category_list;
    QStringList list_value;
    QString command_hashset_insert = QString("insert into 'tbl_mime_type' (bookmark,category_name,mime_type) values(?,?,?)");
    while(query_source_hash_set_details.next())
    {
        QString category_name_str = query_source_hash_set_details.value(1).toString();

        list_value << query_source_hash_set_details.value(0).toString()
                   << category_name_str
                   << query_source_hash_set_details.value(2).toString();

        mime_category_list << category_name_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_hashset_insert,list_value,destination_db,Q_FUNC_INFO);
        list_value.clear();

    }


    mime_category_list.removeDuplicates();

    list_value.clear();
    command_hashset_insert = QString("insert into 'mime_type_category' (bookmark,category_name) values(?,?)");

    for(int count = 0; count < mime_category_list.size(); count++)
    {
        list_value << QString::number(1)
                   << mime_category_list.at(count);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_hashset_insert,list_value,destination_db,Q_FUNC_INFO);

        list_value.clear();
    }

    source_db.close();
    recon_static_functions::app_debug("- END ",Q_FUNC_INFO);
}

void MainWindow::insert_color_info_in_recon_configuration_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug("- START ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName("../Resources/features_resources/features_configuration.sqlite");

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source_db ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }



    QSqlQuery select_query(source_db);
    if(!select_query.exec("select color_name,color_hex from colors"))
    {
        recon_static_functions::app_debug(" Source mime_type  ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + select_query.lastError().text(),Q_FUNC_INFO);
    }

    QString command = QString("insert into 'colors' (color_name,color_hex) values(?,?)");
    while(select_query.next())
    {
        QStringList list_value;
        list_value << select_query.value(0).toString()
                   << select_query.value(1).toString();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list_value,destination_db,Q_FUNC_INFO);
        list_value.clear();
    }


    source_db.close();
    recon_static_functions::app_debug("- END ",Q_FUNC_INFO);
}

void MainWindow::disable_case_tree_sidebar(bool status)
{
    treewidget_case_display_obj->setDisabled(status);
    combobox_case_navigation->setDisabled(status);
    combobox_case_tree_display->setDisabled(status);
    combobox_category_navigation->setDisabled(status);
    combobox_current_displayed_feature_opened->setDisabled(status);
    pushButton_combobox_case_close->setDisabled(status);
    pushButton_combobox_case_close->setDisabled(status);
    pushButton_combobox_category_close->setDisabled(status);
    pushButton_combobox_current_displayed_feature_close->setDisabled(status);
    pushButton_topbar_next->setDisabled(status);
    pushButton_topbar_previous->setDisabled(status);
}

void MainWindow::insert_file_signature_details_in_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug("- START ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName("../Resources/features_resources/features_configuration.sqlite");

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source_db ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_source(source_db);
    if(!query_source.exec("select category,category_status from tbl_signature_category"))
    {
        recon_static_functions::app_debug(" Source file signature category  ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + query_source.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_value;
    QString command_str = QString("insert into 'tbl_signature_category' (category,category_status) values(?,?)");
    while(query_source.next())
    {
        list_value << query_source.value(0).toString()
                   << query_source.value(1).toString();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_str,list_value,destination_db,Q_FUNC_INFO);
        list_value.clear();

    }


    query_source.clear();
    if(!query_source.exec("select category,label,header,signature_status,offset from tbl_signature_list"))
    {
        recon_static_functions::app_debug(" Source file signature list  ------FAILED------- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + query_source.lastError().text(),Q_FUNC_INFO);
    }

    list_value.clear();
    command_str = QString("insert into 'tbl_signature_list' (category,label,header,signature_status,offset) values(?,?,?,?,?)");
    while(query_source.next())
    {
        list_value << query_source.value(0).toString()
                   << query_source.value(1).toString()
                   << query_source.value(2).toString()
                   << query_source.value(3).toString()
                   << query_source.value(4).toString();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_str,list_value,destination_db,Q_FUNC_INFO);
        list_value.clear();

    }

    source_db.close();
    recon_static_functions::app_debug("- END ",Q_FUNC_INFO);
}

void MainWindow::common_hashset_finished_or_cancelled_for_rcase(QStringList default_hashset_list, QString  source_count_name)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    int records_count = 0;

    QString hashset_run_status_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select Hashes from fs_status Where source_count_name = ?") , QStringList(source_count_name), 0, sources_db_path, Q_FUNC_INFO);


    if(hashset_run_status_str.trimmed().isEmpty())
        hashset_run_status_str = QString::number(0);

    if(hashset_run_status_str == QString::number(0))
        return;

    QString file_hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/hashes.sqlite");

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(file_hashes_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    display_loading_progress_bar *progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();

    progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait..."));
    progress_bar_non_cancelable_obj->show();

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Hashset))
        {
            int proc_count = 0;
            for(int pp = 0; pp < default_hashset_list.size() ; pp++)
            {
                proc_count++;
                if(proc_count % 10 == 0)
                {
                    proc_count = 0;
                    QCoreApplication::processEvents();
                }

                records_count = 0;
                QStringList list = default_hashset_list.at(pp).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
                if(list.size() != 2)
                    continue;

                QString db_name = list.at(0).trimmed();
                QString db_path = list.at(1).trimmed();

                QString hash_column_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select column_name From tbl_hashset Where database_path = ?"), QStringList(db_path), 0,conf_db_path ,Q_FUNC_INFO);
                hash_column_str = hash_column_str.trimmed();
                if(hash_column_str.isEmpty())
                    continue;

                QString hash_table_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select table_name From tbl_hashset Where database_path = ?"), QStringList(db_path), 0,conf_db_path ,Q_FUNC_INFO);
                hash_table_str = hash_table_str.trimmed();
                if(hash_table_str.isEmpty())
                    continue;

                QStringList hashes_in_hashset_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select " + hash_column_str + " From " + hash_table_str), 0, db_path, Q_FUNC_INFO);

                hashes_in_hashset_list.removeAll("");

                bool bool_child_exists = false;
                QTreeWidgetItem *sub_child = NULL;
                for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->childCount(); j++)
                {
                    QString tab_text = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j)->text(enum_global_case_tree_display_column_ZERO);
                    if(tab_text.contains("("))
                        tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());
                    tab_text = tab_text.trimmed();

                    QString item_db_path = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j)->text(enum_global_case_tree_display_column_THIRD);

                    if(tab_text == db_name && item_db_path == db_path)
                    {
                        sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->child(j);
                        bool_child_exists = true;

                        break;
                    }
                }

                int proc_count_2 = 0;
                int processed_hashsets = 0;
                for(int num = 0; num < hashes_in_hashset_list.size(); num++)
                {
                    processed_hashsets++;
                    proc_count_2++;
                    if(proc_count_2 % 20 == 0)
                    {
                        proc_count_2 = 0;
                        QCoreApplication::processEvents();

                        progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Processing Hashset") + QString(" - ") + db_name + QString(" (") + QString::number(processed_hashsets) + QString(" / ") + QString::number(hashes_in_hashset_list.size()) + QString(")"));
                    }

                    QStringList records_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(QString("Select filesystem_record From files Where md5_hash = ?"), QStringList(hashes_in_hashset_list.at(num)), 0, destination_db, Q_FUNC_INFO);
                    records_list.removeAll("");

                    if(records_list.isEmpty())
                        continue;

                    records_count += records_list.size();
                }

                progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait..."));


                if(bool_child_exists && sub_child != NULL)
                {
                    QString previous_records = sub_child->text(enum_global_case_tree_display_column_RECORD_COUNT);
                    records_count += previous_records.toInt();

                    QString title_str = db_name + " (" + QString::number(records_count) + ")";


                    sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
                    sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(records_count));
                }
                else
                {
                    sub_child = new QTreeWidgetItem;
                    QString title_str = db_name + " (" + QString::number(records_count) + ")";

                    sub_child->setText(enum_global_case_tree_display_column_ZERO, title_str);
                    sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/hashset.png"));
                    sub_child->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Hashset));
                    sub_child->setText(enum_global_case_tree_display_column_THIRD, db_path);
                    sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, QString::number(records_count));

                    treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_HASHSET)->addChild(sub_child);
                }

            }
        }
    }

    destination_db.close();

    progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}


void MainWindow::display_already_created_tags_name(QString out_path)
{
    QString tag_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QString command = "SELECT tag_name FROM tag_search_index";
    QStringList tags_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,tag_dir_path,Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString tag_name = tags_list.at(i);
        common_add_tag_name_into_case_tree(tag_name);
    }
}

void MainWindow::display_already_created_bookmarks(QString out_path)
{
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    QFileInfo info(bookmark_db_path);
    if(info.exists())
    {
        if(recon_helper_standard_obj->get_db_total_record_by_dbpath(true, bookmark_db_path, QStringList(), Q_FUNC_INFO) > 0)
        {
            common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);
        }
    }

}

void MainWindow::common_right_click_keyword_search_finished_or_cancelled(QString search_tag)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Artifacts_Keyword_Search))
        {
            QString tmp_ksearch = search_tag;
            tmp_ksearch = tmp_ksearch.replace(" ", "_");
            tmp_ksearch = tmp_ksearch.replace("-", "_");

            QString ksearch_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + tmp_ksearch + ".sqlite";

            quint64 total_records =  recon_helper_standard_obj->get_db_total_record_by_dbpath(false, ksearch_db_path, QStringList("keyword_search"), Q_FUNC_INFO);

            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(search_tag + " (" + QString::number(total_records) + ")"));

            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/sub_artifacts_keyword_search.png"));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,"Artifacts Keywod Search");
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT,QString::number(total_records));


            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->addChild(sub_child);

            if(!bool_loading_result_operation_running)
            {
                message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "'" + search_tag + "' search has been processed. Do you want to open it?"));
                message_dialog_object->show();

                bool_message_box_clicked = false;
                while(1)
                {
                    QCoreApplication::processEvents();
                    if(bool_message_box_clicked)
                        break;
                }
                recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                if(bool_message_box_yes_button_clicked)
                {
                    treewidget_case_display_obj->setCurrentItem(sub_child,enum_global_case_tree_display_column_ZERO);
                    common_treewidget_case_display_double_clicked(sub_child,enum_global_case_tree_display_column_ZERO);
                }
            }
            break;
        }
    }


}

void MainWindow::common_right_click_artifacts_timeline_full(QString timeline_name)
{

    prepare_display_for_artifacts_timeline_full(timeline_name);

}

void MainWindow::common_file_search_finished_or_cancelled(QString search_label)
{

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FILE_SEARCH)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_File_Search))
        {

            QString db_name = search_label;
            db_name = db_name.replace(" ", "_");
            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + db_name + ".sqlite";

            QString cmd_record_count = QString("select count(*) from files");
            QString rec_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_record_count, 0, search_db_path, Q_FUNC_INFO);

            QString label_for_tree_with_record = search_label + " (" + rec_count + ")";
            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, label_for_tree_with_record);
            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/sub_global_search.png"));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_File_Search);
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, rec_count);

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_FILE_SEARCH)->addChild(sub_child);

            if(!bool_loading_result_operation_running)
            {
                message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "'" + search_label + "' search has been processed. Do you want to open it?"));
                message_dialog_object->show();

                bool_message_box_clicked = false;
                while(1)
                {
                    QCoreApplication::processEvents();
                    if(bool_message_box_clicked)
                        break;
                }
                recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                if(bool_message_box_yes_button_clicked)
                {
                    treewidget_case_display_obj->setCurrentItem(sub_child,enum_global_case_tree_display_column_ZERO);
                    common_treewidget_case_display_double_clicked(sub_child,enum_global_case_tree_display_column_ZERO);
                }
            }

            break;
        }
    }





}

void MainWindow::remove_global_search_entry_from_tree()
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Artifacts_Keyword_Search))
        {
            for(int jj = 0; jj < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->childCount(); jj++)
            {

                //deleting plugin from tree
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->removeChild(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->child(jj));

            }
        }
    }

}

void MainWindow::remove_artifacts_timeline_saved_entry_from_tree()
{

}

R_Treewidget* MainWindow::set_keyword_search_tree_widget(QString search_name)
{
    recon_static_functions::app_debug(" : Start",Q_FUNC_INFO);

    R_Treewidget *treewidget = new R_Treewidget(this);
    treewidget->setColumnCount(5);
    QStringList list_headers;
    list_headers << "Timestamp" << "Record No." << "Category" << "Item 1" << "Item 2";
    treewidget->setHeaderLabels(list_headers);

    treewidget->setColumnWidth(0,180);
    treewidget->setColumnWidth(1,100);
    treewidget->setColumnWidth(2,150);
    treewidget->setColumnWidth(3,250);
    treewidget->setColumnWidth(4,250);
    treewidget->setIndentation(0);
    treewidget->setAlternatingRowColors(true);
    treewidget->setStyleSheet(
                "QTreeWidget::item:has-children"
                "{ "
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #636363, stop: 0.25 #535353, stop:0.50 #434343, stop:1.0 #333333);"
                "color:white;"
                "} "

                "QTreeWidget {"
                "selection-background-color: #0069D7;"
                "}"
                "QTreeWidget::item:selected:!active {"
                " color:white;"
                " background-color:  #CDCDCD;"
                "}"
                );


    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";
    QString command = "select table_name from 'keyword_search_index' where search_tag = ?";

    QStringList list1;
    list1 << search_name;
    QString db_table_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list1,0,destination_db_path,Q_FUNC_INFO);

    QString command_2 = "SELECT DISTINCT plugin FROM '" + db_table_name + "'";
    QStringList plugins_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_2,0,destination_db_path,Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return treewidget;
    }

    //    QSqlQuery query_index(destination_db);
    //    query_index.prepare("select table_name from 'index' where search_tag = ?");
    //    query_index.addBindValue(search_name);
    //    if(!query_index.exec())
    //    {
    //        recon_static_functions::app_debug(" query_index.exec()  ------FAILED-----",Q_FUNC_INFO);
    //        recon_static_functions::app_debug(query_index.lastError().text() ,Q_FUNC_INFO);
    //        return treewidget;
    //    }

    //    QString db_table_name = "";
    //    if(query_index.next())
    //    {
    //        db_table_name = query_index.value(0).toString();
    //    }

    // QStringList plugins_list = get_pliugins_list(destination_db,db_table_name);


    for(int i = 0; i < plugins_list.size(); i++)
    {


        QSqlQuery query_table(destination_db);
        QString command = "select item3,bookmark,record_no,category,item1,item2,plugin from '" + db_table_name + "' where plugin = '" + plugins_list.at(i) + "'";
        query_table.prepare(command);
        if(!query_table.exec())
        {
            recon_static_functions::app_debug(" query_table.exec()  ------FAILED-----",Q_FUNC_INFO);
            recon_static_functions::app_debug(query_table.lastError().text() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(command,Q_FUNC_INFO);
            return treewidget;
        }

        QTreeWidgetItem *item_parent = new QTreeWidgetItem;
        item_parent->setText(0,plugins_list.at(i));

        while(query_table.next())
        {

            QTreeWidgetItem *item_child  = new QTreeWidgetItem;
            item_child->setText(0, query_table.value(0).toString()); //item3

            if(query_table.value(1).toString() == "0"){//book + record_no

                item_child->setCheckState(1,Qt::Unchecked) ;
                item_child->setText(1,query_table.value(2).toString());

            }
            else
            {
                item_child->setCheckState(1,Qt::Checked) ;
                item_child->setText(1,query_table.value(2).toString());
            }
            item_child->setTextAlignment(1,Qt::AlignRight | Qt::AlignVCenter);

            item_child->setText(2,query_table.value(3).toString()); //category
            item_child->setText(3,query_table.value(4).toString()); //item1
            item_child->setText(4,query_table.value(5).toString()); //item2

            item_parent->addChild(item_child);


        }
        item_parent->setExpanded(true);
        treewidget->addTopLevelItem(item_parent);
    }
    return treewidget;
}

void MainWindow::common_content_search_finished_or_cancelled(QString search_label)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_CONTENT_SEARCH)->text(0) == QString(MACRO_CASE_TREE_Content_Search))
        {
            QString db_name = search_label;
            db_name = db_name.replace(" ", "_");
            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + db_name + ".sqlite";

            QString cmd_record_count = QString("select count(*) from files");
            QString rec_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_record_count, 0, search_db_path, Q_FUNC_INFO);
            rec_count = rec_count.trimmed();
            if(rec_count.isEmpty())
                rec_count = QString::number(0);

            QString label_for_tree_with_record = search_label + " (" + rec_count + ")";

            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, label_for_tree_with_record);
            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/sub_content_search.png"));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_Content_Search);
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, rec_count);

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_CONTENT_SEARCH)->addChild(sub_child);

            if(!bool_loading_result_operation_running)
            {
                message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "'" + search_label + "' search has been processed. Do you want to open it?"));
                message_dialog_object->show();

                bool_message_box_clicked = false;
                while(1)
                {
                    QCoreApplication::processEvents();
                    if(bool_message_box_clicked)
                        break;
                }
                recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                if(bool_message_box_yes_button_clicked)
                {
                    treewidget_case_display_obj->setCurrentItem(sub_child,enum_global_case_tree_display_column_ZERO);
                    common_treewidget_case_display_double_clicked(sub_child,enum_global_case_tree_display_column_ZERO);
                }
            }

            break;


        }
    }
}

void MainWindow::common_add_artifacts_timeline_name_into_case_tree(QString tab_name)
{
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_TIMELINE)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Artifacts_Timeline))
        {
            QString case_display_name = tab_name;
            case_display_name = case_display_name.replace(" ","_");
            case_display_name.chop(7);///remove sqlite extension
            QString saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + tab_name;
            quint64 total_records =  recon_helper_standard_obj->get_db_total_record_by_dbpath(false, saved_db_path, QStringList("timeline_data"), Q_FUNC_INFO);


            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(0, QString(case_display_name + " (" + QString::number(total_records) + ")"));
            //sub_child->setText(0, QString(case_display_name));///without records

            sub_child->setIcon(0,QIcon("../icons/Features/sub_timeline_saved.png"));
            sub_child->setText(1,"Saved Timeline");
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_TIMELINE)->addChild(sub_child);
        }
    }
}

void MainWindow::common_add_redefined_saved_result_name_into_case_tree(QString tab_name, QString plugin_name)
{

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Redefined_Result_Saved_Parent))
        {
            if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED)->childCount() == 0)
            {

                QString icon_path1 = "../icons/Features/messenger_saved.png";
                QTreeWidgetItem *sub_child1 = new QTreeWidgetItem;

                sub_child1->setIcon(0,QIcon(icon_path1));
                sub_child1->setText(0, MACRO_Redefined_Result_Messenger);
                sub_child1->setText(1,QString(MACRO_CASE_TREE_Redefined_Result_Saved_Parent));
                sub_child1->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED)->addChild(sub_child1);

                QString icon_path2 = "../icons/Features/browser_history_saved.png";
                QTreeWidgetItem *sub_child2 = new QTreeWidgetItem;
                sub_child2->setIcon(0,QIcon(icon_path2));
                sub_child2->setText(0, MACRO_Redefined_Result_Browser_History);
                sub_child2->setText(1,QString(MACRO_CASE_TREE_Redefined_Result_Saved_Parent));
                sub_child2->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED)->addChild(sub_child2);

            }
            if(plugin_name == QString(MACRO_Redefined_Result_Messenger) || plugin_name == QString(MACRO_Plugin_Name_Messenger_Full_RR))
            {
                QString case_display_name = tab_name;
                QString saved_db_path;

                case_display_name.replace("_"," ");
                saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + tab_name + ".sqlite";

                quint64 total_records =  recon_helper_standard_obj->get_db_total_record_by_dbpath(false, saved_db_path, QStringList("redefined_result_saved"), Q_FUNC_INFO);

                QTreeWidgetItem *sub_child = new QTreeWidgetItem;
                sub_child->setText(0, QString(case_display_name + " (" + QString::number(total_records) + ")"));
                sub_child->setIcon(0,QIcon("../icons/Features/redefined_result.png"));
                sub_child->setText(1,MACRO_CASE_TREE_Redefined_Result_Saved_Parent);
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED)->child(enum_global_case_tree_display_column_ZERO)->addChild(sub_child);

            }

            if(plugin_name == QString(MACRO_Redefined_Result_Browser_History) || plugin_name == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
            {
                QString case_display_name = tab_name;
                QString saved_db_path;

                case_display_name.replace("_"," ");
                saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + tab_name + ".sqlite";

                quint64 total_records =  recon_helper_standard_obj->get_db_total_record_by_dbpath(false, saved_db_path, QStringList("redefined_result_saved"), Q_FUNC_INFO);

                QTreeWidgetItem *sub_child = new QTreeWidgetItem;
                sub_child->setText(0, QString(case_display_name + " (" + QString::number(total_records) + ")"));
                sub_child->setIcon(0,QIcon("../icons/Features/redefined_result.png"));
                sub_child->setText(1,MACRO_CASE_TREE_Redefined_Result_Saved_Parent);
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_REDEFINED_RESULT_SAVED)->child(enum_global_case_tree_display_column_FIRST)->addChild(sub_child);

            }
        }
    }
}
void MainWindow::common_exif_metadata_search_finished_or_cancelled(QString search_label)
{

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_EXIF_METADATA_SEARCH)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Exif_Metadata_Search))
        {

            QString db_name = search_label;
            db_name = db_name.replace(" ", "_");
            QString search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + db_name + ".sqlite";

            QString cmd_record_count = QString("select count(*) from files");
            QString rec_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(cmd_record_count, 0, search_db_path, Q_FUNC_INFO);

            QString label_for_tree_with_record = search_label + " (" + rec_count + ")";
            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, label_for_tree_with_record);
            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon("../icons/Features/sub_global_search.png"));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,MACRO_CASE_TREE_Exif_Metadata_Search);
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            sub_child->setText(enum_global_case_tree_display_column_RECORD_COUNT, rec_count);

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_EXIF_METADATA_SEARCH)->addChild(sub_child);

            if(!bool_loading_result_operation_running)
            {
                message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "'" + search_label + "' search has been processed. Do you want to open it?"));
                message_dialog_object->show();

                bool_message_box_clicked = false;
                while(1)
                {
                    QCoreApplication::processEvents();
                    if(bool_message_box_clicked)
                        break;
                }
                recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

                if(bool_message_box_yes_button_clicked)
                {
                    treewidget_case_display_obj->setCurrentItem(sub_child,enum_global_case_tree_display_column_ZERO);
                    common_treewidget_case_display_double_clicked(sub_child,enum_global_case_tree_display_column_ZERO);
                }
            }

            break;
        }
    }

}
