#include "tt_old_hex_viewer_qhexedit_p.h"

void QHexEditPrivate::slot_tag_start_offset_triggered()
{
    int start_index                                 = this->Point2Char(this->sel_start)/2;
    struct_obj.start_index                          = start_index;

    emit signal_tag_start_offset_clicked(start_index);
}

void QHexEditPrivate::slot_tag_end_offset_submenu_triggered(QAction *clicked_tag_name)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    int current_index_end = this->Point2Char(this->sel_start)/2;
    uint64_t file_end_index = (current_page_start_offset + current_index_end);

    if(tag_start_offset >= file_end_index)
    {
        QString display_message = "Please set valid block end offset\n(" + QString::number(tag_start_offset) + " - " + QString::number(file_end_index) + ") is not possible";
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,display_message);
        message_dialog_obj->show();
        return;
    }

    start_offset_clicked = false;

    struct_obj.block_actual_start_offset_in_file    = tag_start_offset;
    struct_obj.end_index                            = current_index_end;
    struct_obj.block_actual_end_offset_in_file      = file_end_index;

    tag_end_offset = file_end_index;

    QString tag_name = clicked_tag_name->text();

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_for_selected_bytes_obj->tags_db_path,Q_FUNC_INFO);

    command_str = "SELECT color_hex FROM tags WHERE selected_colour = ?  AND tag_name = ?";
    list1.clear();
    list1 << colour << tag_name;

    QString colour_hex = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_for_selected_bytes_obj->tags_db_path,Q_FUNC_INFO);

    struct_obj.current_block_name      = tag_name;
    struct_obj.block_highlighting_color      = QColor(colour_hex);

    list_struct_indexs.append(struct_obj);

    emit signal_select_tag_name_for_tag_end_offset(tag_name,colour);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void QHexEditPrivate::add_selected_bytes_to_block(QString block_name,QString color_name)
{
    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end))
    {
        int start_index = this->Point2Char(this->sel_start)/2;
        int end_index = this->Point2Char(this->sel_end)/2;

        uint64_t block_actual_start_offset = current_page_start_offset + start_index;
        uint64_t block_actual_end_offset = current_page_start_offset + end_index;

        struct_global_hex_viewer_start_and_end_index struct_obj_1;
        struct_obj_1.start_index = start_index;
        struct_obj_1.end_index = end_index;
        struct_obj_1.block_highlighting_color = QColor(color_name);
        struct_obj_1.current_block_name = block_name;
        struct_obj_1.block_actual_start_offset_in_file = block_actual_start_offset;
        struct_obj_1.block_actual_end_offset_in_file = block_actual_end_offset;

        list_struct_indexs.append(struct_obj_1);
    }
    else if((!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        int start_index = this->Point2Char(this->search_start)/2;
        int end_index = this->Point2Char(this->search_end)/2;


        uint64_t block_actual_start_offset = current_page_start_offset + start_index;
        uint64_t block_actual_end_offset = current_page_start_offset + end_index;

        struct_global_hex_viewer_start_and_end_index struct_obj_1;
        struct_obj_1.start_index = start_index;
        struct_obj_1.end_index = end_index;
        struct_obj_1.block_highlighting_color = QColor(color_name);
        struct_obj_1.current_block_name = block_name;
        struct_obj_1.block_actual_start_offset_in_file = block_actual_start_offset;
        struct_obj_1.block_actual_end_offset_in_file = block_actual_end_offset;

        list_struct_indexs.append(struct_obj_1);
    }
}

void QHexEditPrivate::slot_abouttoshow_tags_in_submenu()
{
    display_tag_end_offset_submenu();
}

void QHexEditPrivate::slot_tag_selected_bytes_submenu_triggered(QAction *action_name)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString tag_name = action_name->text();

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QStringList list1;
    list1 << tag_name;

    QString colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1 ,0,add_tags_for_selected_bytes_obj->tags_db_path,Q_FUNC_INFO);

    emit signal_add_tags_to_selected_bytes(tag_name,colour);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void QHexEditPrivate::slot_export_selected_bytes_triggered()
{
    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end))
    {
        int selection_start=this->Point2Char(this->sel_start)/2;
        int selection_end=this->Point2Char(this->sel_end)/2;

        QByteArray selected_bytes = _data.mid(selection_start,(selection_end-selection_start+1));

        emit export_selected_bytes_offset(selected_bytes);
    }
    else if((!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        int selection_start=this->Point2Char_for_search(this->search_start)/2;
        int selection_end=this->Point2Char_for_search(this->search_end)/2;

        QByteArray selected_bytes = _data.mid(selection_start,(selection_end-selection_start+1));

        emit export_selected_bytes_offset(selected_bytes);
    }
}

void QHexEditPrivate::pub_set_start_offset_clicked(bool is_clicked)
{
    start_offset_clicked = is_clicked;
}

void QHexEditPrivate::pub_set_start_offset(uint64_t offset)
{
    tag_start_offset = offset;
}


uint64_t QHexEditPrivate::get_start_offset_of_block(QString block_name)
{
    uint64_t start_offset = 0;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(block_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return start_offset;
    }


    QSqlQuery query_select_blocks_info(destination_db);
    query_select_blocks_info.prepare("SELECT block_start_offset FROM hex_viewer_blocks WHERE block_name=?");
    query_select_blocks_info.addBindValue(block_name);
    if(!query_select_blocks_info.exec())
    {
        recon_static_functions::app_debug("query_select_blocks_info execution-----FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select_blocks_info.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select_blocks_info.executedQuery() ,Q_FUNC_INFO);
        destination_db.close();
        return start_offset;
    }

    if(query_select_blocks_info.next())
    {
        start_offset = query_select_blocks_info.value(0).toULongLong();
    }

    destination_db.close();

    return start_offset;
}

void QHexEditPrivate::display_tag_selected_bytes_submenu()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    this->p_menu_tag_selected_bytes->clear();

    ///add new tag
    QAction *act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    this->p_menu_tag_selected_bytes->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_create_new_tag_for_selected_bytes()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_create_new_tag_for_selected_bytes()));


    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    this->p_menu_tag_selected_bytes->addAction(separator_main);

    add_tags_for_selected_bytes_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_for_selected_bytes_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                this->p_menu_tag_selected_bytes->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", QString::SkipEmptyParts);

            QString icon_path = "";

            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            this->p_menu_tag_selected_bytes->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void QHexEditPrivate::display_tag_end_offset_submenu()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    this->p_menu_tag_end_offset->clear();

    ///add new tag
    QAction *act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    this->p_menu_tag_end_offset->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_create_new_tag_for_end_offset()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_create_new_tag_for_end_offset()));


    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    this->p_menu_tag_end_offset->addAction(separator_main);

    add_tags_for_end_offset_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_for_end_offset_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                this->p_menu_tag_end_offset->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", QString::SkipEmptyParts);

            QString icon_path = "";

            QString tag_str = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            this->p_menu_tag_end_offset->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}


void QHexEditPrivate::slot_create_new_tag_for_selected_bytes()
{
    add_tags_for_selected_bytes_obj->show();
}

void QHexEditPrivate::slot_create_new_tag_for_end_offset()
{
    add_tags_for_end_offset_obj->show();
}

void QHexEditPrivate::slot_select_tag_name_for_tag_end_offset(QString tag_name, QString color_name)
{    
    if(tag_name.isEmpty())
        return;

    emit signal_tag_end_offset_clicked(tag_name,color_name, tag_start_offset, tag_end_offset);
}

void QHexEditPrivate::slot_add_tag_to_selected_bytes(QString tag_name, QString color_name)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end))
    {
        int start_index = this->Point2Char(this->sel_start)/2;
        int end_index = this->Point2Char(this->sel_end)/2;

        emit signal_insert_tag_selected_bytes_info_in_db(start_index, end_index, tag_name, color_name);
    }
    else if((!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        int start_index = this->Point2Char_for_search(this->search_start)/2;
        int end_index = this->Point2Char_for_search(this->search_end)/2;

        emit signal_insert_tag_selected_bytes_info_in_db(start_index, end_index, tag_name, color_name);
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}



void QHexEditPrivate::slot_update_tag_selected_bytes_submenu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        QAction *act_already_saved_tags_selected_bytes = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags_selected_bytes->setIcon(icon);
        this->p_menu_tag_selected_bytes->addAction(act_already_saved_tags_selected_bytes);

        QAction *act_already_saved_tags_end_offset = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags_end_offset->setIcon(icon);
        this->p_menu_tag_end_offset->addAction(act_already_saved_tags_end_offset);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void QHexEditPrivate::slot_update_tag_end_offset_submenu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        QAction *act_already_saved_tags_end_offset = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags_end_offset->setIcon(icon);
        this->p_menu_tag_end_offset->addAction(act_already_saved_tags_end_offset);

        QAction *act_already_saved_tags_selected_bytes = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags_selected_bytes->setIcon(icon);
        this->p_menu_tag_selected_bytes->addAction(act_already_saved_tags_selected_bytes);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

