#include "log_viewer_interface.h"
#include "ui_log_viewer_interface.h"

// all the right click actions are given which we have created for log viewer
void log_viewer_interface::slot_sub_menu_tags_clicked(QAction *action)
{ // tags submenu right click action
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    action_submenu_tag_triggered(action->text());

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::action_submenu_tag_triggered(QString tag_name)
{ // actions on right click submenu
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString selected_color = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("SELECT selected_colour FROM tags Where tag_name = ?",QStringList(tag_name) ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);

    slot_update_tags_value(tag_name, selected_color);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::display_tags_submenu()
{ // display tags submenu with by default colors and actions
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    sub_menu_tags->clear();

    ///add new tag
    QAction *act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);

    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_action_create_tag_triggered()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_action_create_tag_triggered()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString select_command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(select_command_str, add_tags_module_obj->tags_db_path, Q_FUNC_INFO);

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

            recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);

            QString icon_path = "";

            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);

        }
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void log_viewer_interface::slot_action_create_tag_triggered()
{ // create tags on submenu clicked
    add_tags_module_obj->show();
}

void log_viewer_interface::slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour)
{ // update tags in action menu
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
        sub_menu_tags->addAction(act_already_saved_tags);

        emit signal_update_tag_submenu_of_plugins(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void log_viewer_interface::slot_update_tags_value(QString tag_data, QString colour)
{ // slot of update tags value in database according to tags add/remove pressed
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QTextCursor cursor = ui->textBrowser_log_viewer->textCursor();
    QString selected_text = cursor.selectedText();

    qint64 cursor_start_index = cursor.selectionStart();
    qint64 cursor_end_index = cursor.selectionEnd();

    if(selected_text.trimmed().isEmpty())
        return;

    QString case_confg_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString select_command_str = "SELECT color_hex FROM colors Where color_name = ?";

    QStringList value_list;
    value_list << colour;
    QString colour_hex = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_command_str,value_list,0,case_confg_db_path,Q_FUNC_INFO);

    //Note: Highlight text according to tag's colour
    QTextCharFormat colorFormat;
    colorFormat.setForeground(QBrush(QColor(colour_hex)));
    cursor.mergeCharFormat(colorFormat);

    create_table_log_export_content();

    QFileInfo log_filepath_info(complete_log_file_path);
    QString log_file_name = QFileInfo(log_file_path).fileName();

    QList<struct_global_tagging_log_viewer_record> log_viewer_record_qlist;
    struct_global_tagging_log_viewer_record struct_global_log_viewer_rec_obj;
    struct_global_log_viewer_rec_obj.INT_record = record_no;
    struct_global_log_viewer_rec_obj.plugin_name = QString(MACRO_Plugin_Name_Log_Viewer);
    struct_global_log_viewer_rec_obj.tab_name = QString(MACRO_Plugin_Name_Log_Viewer);
    struct_global_log_viewer_rec_obj.log_file_name = log_file_name;
    struct_global_log_viewer_rec_obj.source_file_path = log_file_path;
    struct_global_log_viewer_rec_obj.tagged_data_str = selected_text.trimmed();
    struct_global_log_viewer_rec_obj.file_size_str = QString::number(log_filepath_info.size());
    struct_global_log_viewer_rec_obj.viewer_display_name = log_viewer_display_name;

    struct_global_log_viewer_rec_obj.cursor_start_index = cursor_start_index;
    struct_global_log_viewer_rec_obj.cursor_end_index = cursor_end_index;

    struct_global_log_viewer_rec_obj.log_table_name = log_db_table_name;
    struct_global_log_viewer_rec_obj.source_count_name = source_count_name;
    struct_global_log_viewer_rec_obj.colour_value = colour;
    struct_global_log_viewer_rec_obj.recon_file_from = recon_file_from_str;
    struct_global_log_viewer_rec_obj.recon_file_infopath = recon_file_info_path_str;

    log_viewer_record_qlist << struct_global_log_viewer_rec_obj;

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Log_Viewer));

    global_connection_manager_obj->pub_set_tag_in_log_viewer_db(log_viewer_record_qlist, log_db_table_name, tag_data, colour, destination_db_path);
    global_connection_manager_obj->pub_add_data_in_export_content_log_viewer_db(log_viewer_record_qlist, tag_data, MACRO_Plugin_Name_Log_Viewer);
    global_connection_manager_obj->pub_add_data_in_tags_db_for_log_viewer(log_viewer_record_qlist, tag_data);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}
