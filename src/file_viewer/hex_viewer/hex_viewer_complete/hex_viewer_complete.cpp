#include "hex_viewer_complete.h"

hex_viewer_complete::hex_viewer_complete(QWidget *parent) : QWidget(parent)
{
    scroll_bar_64_obj = new Scrollbar64(this);
    scroll_bar_64_obj->setMinimumWidth(12);

    recon_helper_standard_obj = new recon_helper_standard(this);

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;


    menu_templates = new QMenu("Show as");
    action_template_mbr = menu_templates->addAction("MBR");
    action_template_gpt = menu_templates->addAction("GPT");
    menu_templates->addSeparator();
    action_template_fat_vbr = menu_templates->addAction("FAT VBR");
    action_template_fat32_vbr = menu_templates->addAction("FAT32 VBR");
    action_template_fat_directory_entry = menu_templates->addAction("FAT 32 bit Directory Entry");
    menu_templates->addSeparator();
    action_template_ntfs_vbr = menu_templates->addAction("NTFS VBR");
    action_template_mft_entry = menu_templates->addAction("NTFS MFT Entry");
    //action_template_standard_attribute = menu_templates->addAction("NTFS Standard Attribute");
    connect(action_template_fat_directory_entry, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_fat_directory_entry_triggered(bool)));
    connect(action_template_mbr, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_mbr_triggered(bool)));
    connect(action_template_ntfs_vbr, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_ntfs_vbr_triggered(bool)));
    connect(action_template_mft_entry, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_mft_entry_triggered(bool)));
    connect(action_template_gpt, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_gpt_triggered(bool)));
    connect(action_template_fat32_vbr, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_fat32_vbr_triggered(bool)));
    connect(action_template_fat_vbr, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_fat_vbr_triggered(bool)));

    menu_templates_color = new QMenu("Apply Template");
    action_template_mbr_color = menu_templates_color->addAction("MBR");
    action_template_fat_vbr_color = menu_templates_color->addAction("FAT VBR");
    action_template_fat_directory_entry_color = menu_templates_color->addAction("FAT 32 bit Directory Entry");
    action_template_standard_attribute_color = menu_templates_color->addAction("NTFS Standard Attribute");
    menu_templates_color->addSeparator();
    action_template_manual = menu_templates_color->addAction("Custom Template");
    connect(action_template_fat_directory_entry_color, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_fat_directory_entry_color_triggered(bool)));
    connect(action_template_manual, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_manual_triggered(bool)));
    connect(action_template_mbr_color, SIGNAL(triggered(bool)), this, SLOT(slot_action_template_mbr_color_triggered(bool)));

    create_ui();

    pushButton_save->hide();
    pushButton_read_file_as_image->hide();
    comboBox_bytes_per_line->setCurrentText("32");

    create_tag_display();
    tableWidget_tags_list->hideColumn(enum_TAG_TABLE_DESCRIPTION);

    create_data_interpreter_display();

    hex_viewer_search_dialog_obj = new hex_viewer_search_dialog(this);
    connect(hex_viewer_search_dialog_obj, SIGNAL(signal_search_text_and_type(QString,QString,bool)), this, SLOT(slot_search_text_and_type(QString,QString,bool)));

    get_address_dialog_obj = new get_address_dialog(this);
    connect(get_address_dialog_obj, SIGNAL(signal_jump_to_offset(qint64,int)), this, SLOT(slot_jump_to_offset(qint64,int)));

    hexviewer_progress_bar_obj  = new hexviewer_progress_bar(this);
    hexviewer_progress_bar_obj->hide();


    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;

    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));
    message_dialog_obj->hide();


    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));


}

void hex_viewer_complete::pub_set_global_connection_manager(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void hex_viewer_complete::pub_set_data(QByteArray data_arr)
{
    hex_widget->setData(data_arr);
}

bool hex_viewer_complete::check_is_data_available()
{
    return hex_widget->pub_is_data_available();
}

void hex_viewer_complete::pub_set_file_path(QString complete_filepath, QString display_filepath)
{
    tag_create_popup_obj->pub_set_tags_data();

    fill_the_existing_blocks_details_in_list();

    complete_filepath.replace("//", "/");

    source_file_path = complete_filepath;
    data_file.setFileName(source_file_path);


    label_filename_value->clear();
    label_filepath_value->clear();
    label_filesize_value->clear();

    QString hex_export_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString();

    bool bool_is_hex_exported_file = false;
    if(complete_filepath.startsWith(hex_export_path))
        bool_is_hex_exported_file = true;

    if(bool_is_hex_exported_file)
    {
        label_filename_title->hide();
        label_filepath_title->hide();
        label_filesize_title->hide();

        label_filename_value->hide();
        label_filepath_value->hide();
        label_filesize_value->hide();
    }
    else
    {
        QFileInfo file_info(complete_filepath);

        label_filename_value->setText(recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_filepath));
        label_filesize_value->setText(recon_static_functions::human_readable_size(QString::number(file_info.size()), Q_FUNC_INFO));
        label_filepath_value->setText(display_filepath);

        label_filename_value->setToolTip(label_filename_value->text());
        label_filepath_value->setToolTip(label_filepath_value->text());
        label_filesize_value->setToolTip(label_filesize_value->text());

        label_filename_title->show();
        label_filepath_title->show();
        label_filesize_title->show();

        label_filename_value->show();
        label_filepath_value->show();
        label_filesize_value->show();
    }


    hex_widget->pub_set_source_file(source_file_path);
    //    hex_widget->pub_set_source_size(QFileInfo(source_file_path).size());
    //    hex_widget->pub_set_source_block_size(512);

    hex_widget->setData(data_file);

    //create_marker_display();
    create_tag_display();

    pub_disable_ui_if_data_not_available();

}


void hex_viewer_complete::pub_set_hex_viewer_open_file_data_obj(struct_global_hex_viewer_open_file_data obj)
{
    hex_viewer_open_file_data_obj = obj;
}

void hex_viewer_complete::pub_disable_ui_if_data_not_available()
{
    if(source_file_size <= 0)
    {
        side_widget->setEnabled(false);
        toolbar_widget->setEnabled(false);

        comboBox_bytes_per_line->setEnabled(false);

        hex_widget->setEnabled(false);
        pushButton_search_keyword->setEnabled(false);
        pushButton_search_next->setEnabled(false);
        pushButton_search_previous->setEnabled(false);

        pushButton_jump_to_offset_in_file->setEnabled(false);
        pushButton_show_hide_tags->setEnabled(false);

        menu_right_click->setEnabled(false);
    }
    else
    {
        side_widget->setEnabled(true);
        toolbar_widget->setEnabled(true);

        comboBox_bytes_per_line->setEnabled(true);

        hex_widget->setEnabled(true);
        pushButton_search_keyword->setEnabled(true);
        pushButton_search_next->setEnabled(true);
        pushButton_search_previous->setEnabled(true);

        pushButton_jump_to_offset_in_file->setEnabled(true);
        pushButton_show_hide_tags->setEnabled(true);

        menu_right_click->setEnabled(true);
    }
}

void hex_viewer_complete::pub_set_source_file_size(qint64 size)
{
    hex_widget->pub_set_source_size(size);
    source_file_size = size;

    pub_disable_ui_if_data_not_available();

}

void hex_viewer_complete::pub_set_source_block_size(int size)
{
    hex_widget->pub_set_source_block_size(size) ;
    source_file_block_size = size;
}

void hex_viewer_complete::pub_highlight_keyword(QString keyword_str)
{
    hex_viewer_search_dialog_obj->pub_highlight_keyword(keyword_str);
}

void hex_viewer_complete::slot_selected_data_offsets(qint64 start_address, qint64 end_address)
{
    selection_start = start_address;
    selection_end = end_address - 1;

    QString selection_in_hex = "0x" + QString::number((selection_end - selection_start + 1), 16);

    label_selection_start->setText("Selection : " + QString::number(selection_start) + "-" + QString::number(selection_end));
    label_selection_count->setText("Selection Count : " + QString::number(selection_end - selection_start + 1) + " (" + selection_in_hex + ")"  );
}

void hex_viewer_complete::slot_cursor_position(qint64 pos)
{
    if(pos > source_file_size)
        pos = source_file_size;

    current_cursor_position = pos;

    QString cursor_positior_in_hex = "0x" + QString::number(current_cursor_position, 16);
    label_cursor_position->setText("Cursor Position : " + QString::number(current_cursor_position) + " (" + cursor_positior_in_hex + ")");

    qint64 start_pos = current_cursor_position;
    //    if(current_cursor_position > selection_start)
    //        start_pos = selection_start;


    //QByteArray chunk = get_selected_data_block_from_source(selection_start, 8);
    QByteArray chunk = get_selected_data_block_from_source(start_pos, 8);
    if(chunk.size() < 8)
        return;

    char ch_8bit = chunk.at(0);
    QByteArray _16_bit_array = chunk.mid(0, 2);
    QByteArray _32_bit_array = chunk.mid(0, 4);
    QByteArray _24_bit_array = chunk.mid(0, 3);
    QByteArray _64_bit_array = chunk.mid(0, 8);

    uint16_t _16_bit_unsigned = 0;
    qint16 int_val_16 = 0;
    uint32_t _24_bit_unsigned = 0;
    qint32 int_val_24 = 0;
    uint32_t _32_bit_unsigned = 0;
    qint32 int_val_32 = 0;
    uint64_t _64_bit_unsigned = 0;
    qint64 int_val_64 = 0;

    char bit_8_signed = chunk.at(0);
    unsigned char bit_8_unsigned = (unsigned char)chunk.at(0);

    if(checkBox_little_endian->isChecked())
    {
        _16_bit_unsigned = utility_obj.convert_bytearray_to_number_16bit_unsigned(_16_bit_array, LITTLE_ENDIAN_HEX);
        int_val_16 = utility_obj.convert_bytearray_to_number_16bit(_16_bit_array, LITTLE_ENDIAN_HEX);

        _32_bit_unsigned = utility_obj.convert_bytearray_to_number_32bit_unsigned(_32_bit_array, LITTLE_ENDIAN_HEX);
        int_val_32 = utility_obj.convert_bytearray_to_number_32bit(_32_bit_array, LITTLE_ENDIAN_HEX);

        _64_bit_unsigned = utility_obj.convert_bytearray_to_number_64bit_unsigned(_64_bit_array, LITTLE_ENDIAN_HEX);
        int_val_64 = utility_obj.convert_bytearray_to_number_64bit(_64_bit_array, LITTLE_ENDIAN_HEX);

    }
    else {
        _16_bit_unsigned = utility_obj.convert_bytearray_to_number_16bit_unsigned(_16_bit_array, BIG_ENDIAN_HEX);
        int_val_16 = utility_obj.convert_bytearray_to_number_16bit(_16_bit_array, BIG_ENDIAN_HEX);

        _24_bit_unsigned = utility_obj.convert_bytearray_to_number_24bit_unsigned(_24_bit_array, BIG_ENDIAN_HEX);
        int_val_24 = utility_obj.convert_bytearray_to_number_24_bit(_24_bit_array, BIG_ENDIAN_HEX);

        _32_bit_unsigned = utility_obj.convert_bytearray_to_number_32bit_unsigned(_32_bit_array, BIG_ENDIAN_HEX);
        int_val_32 = utility_obj.convert_bytearray_to_number_32bit(_32_bit_array, BIG_ENDIAN_HEX);

        _64_bit_unsigned = utility_obj.convert_bytearray_to_number_64bit_unsigned(_64_bit_array, BIG_ENDIAN_HEX);
        int_val_64 = utility_obj.convert_bytearray_to_number_64bit(_64_bit_array, BIG_ENDIAN_HEX);
    }

    tableWidget_data_interpreter->item(enum_data_interpreter_8_bit_binary, 1)->setText(utility_obj.convert_8bit_to_binary(ch_8bit));
    tableWidget_data_interpreter->item(enum_data_interpreter_8_bit_signed, 1)->setText(QString::number(bit_8_signed));
    tableWidget_data_interpreter->item(enum_data_interpreter_8_bit_unsigned, 1)->setText(QString::number(bit_8_unsigned));
    tableWidget_data_interpreter->item(enum_data_interpreter_16_bit_signed, 1)->setText(QString::number(int_val_16));
    tableWidget_data_interpreter->item(enum_data_interpreter_16_bit_unsigned, 1)->setText(QString::number(_16_bit_unsigned));

    //tableWidget_data_interpreter->item(enum_data_interpreter_24_bit_signed, 1)->setText(QString::number(int_val_24));

    tableWidget_data_interpreter->item(enum_data_interpreter_32_bit_signed, 1)->setText(QString::number(int_val_32));
    tableWidget_data_interpreter->item(enum_data_interpreter_32_bit_unsigned, 1)->setText(QString::number(_32_bit_unsigned));
    tableWidget_data_interpreter->item(enum_data_interpreter_64_bit_signed, 1)->setText(QString::number(int_val_64));
    tableWidget_data_interpreter->item(enum_data_interpreter_64_bit_unsigned, 1)->setText(QString::number(_64_bit_unsigned));
    tableWidget_data_interpreter->item(enum_data_interpreter_DOS_time, 1)->setText(utility_obj.fat_time(_16_bit_array));
    tableWidget_data_interpreter->item(enum_data_interpreter_DOS_date, 1)->setText(utility_obj.fat_date(_16_bit_array));
    tableWidget_data_interpreter->item(enum_data_interpreter_Unix_timestamp, 1)->setText(QDateTime::fromSecsSinceEpoch(_32_bit_array.toHex().toULong(0, 16)).toString("dd-MMM-yy hh:mm:ss"));
    tableWidget_data_interpreter->item(enum_data_interpreter_Windows_timestamp, 1)->setText(utility_obj.convert_ntfs_time_to_unix_epoch_string(chunk));

}

void hex_viewer_complete::slot_comboBox_bytes_per_line_current_index_changed(QString str)
{
    hex_widget->setBytesPerLine(str.toInt());

    hex_widget->pub_set_jump_to_address(current_cursor_position);
}

void hex_viewer_complete::slot_pushButton_show_tables_clicked()
{
    if(pushButton_show_hide_tags->text().contains("Show", Qt::CaseInsensitive))
    {
        pushButton_show_hide_tags->setText("Hide Tags");
        main_tab_widget_obj->show();
    }
    else
    {
        pushButton_show_hide_tags->setText("Show Tags");
        main_tab_widget_obj->hide();
    }
}

void hex_viewer_complete::slot_hex_widget_right_clicked()
{
    menu_right_click->exec(QCursor::pos());
}

void hex_viewer_complete::slot_hex_widget_left_clicked()
{
    label_selection_start->setText("Selection : ");
    label_selection_count->setText("Selection Count : ");
}

void hex_viewer_complete::slot_action_tag_bytes_triggered(bool status)
{
    Q_UNUSED(status);
    tag_create_popup_obj->pub_set_start_address(selection_start);
    tag_create_popup_obj->pub_set_end_address(selection_end);
    tag_create_popup_obj->show();
}

void hex_viewer_complete::slot_save_tag_details(qint64 start, qint64 end, QString tag_name, QString color_hex)
{
    struct_hex_viewer_tag tag;
    tag.start_address = start;
    tag.end_address   = end;
    tag.color_hex     = color_hex;
    tag.tag_name_str   = tag_name;
    hex_viewer_tags_list.append(tag);

    save_details_in_hex_viewer_feature_database(tag);

    create_tag_display();
}

void hex_viewer_complete::slot_tab_tablewidget_cell_double_clicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    qint64 start_address = tableWidget_tags_list->item(row, enum_TAG_TABLE_START)->text().toLongLong();

    hex_widget->pub_set_jump_to_address(start_address);
}

void hex_viewer_complete::slot_tableWidget_marker_cell_double_clicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    qint64 start_address = tableWidget_marker->item(row, enum_marker_table_offset_dec)->text().toLongLong();
    hex_widget->pub_set_jump_to_address(start_address);
}

void hex_viewer_complete::slot_search_text_and_type(QString text, QString type, bool bool_clear_prev_list)
{
    text = text.trimmed();
    type = type.trimmed();
    if(text.isEmpty() || type.isEmpty())
        return;

    current_text_searched_pos = -1;

    current_search_text = text;
    current_search_type = type;

    QByteArrayMatcher mar(text.toLocal8Bit());
    QByteArrayMatcher mar_unicode((const char*) (text.utf16()), text.size() * 2);
    QByteArray text_to_search_in_hex = QByteArray::fromHex(text.toLocal8Bit());
    QByteArrayMatcher mar_hex(text_to_search_in_hex);

    QFile file(source_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open -----FAILED----- " + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + file.errorString(), Q_FUNC_INFO);
        return;
    }

    qint64 pos = current_cursor_position;

    if(pos > file.size() || pos < 0)
        pos = 0;

    qint64 offset_in_block = pos % source_file_block_size;

    qint64 file_offset = pos - offset_in_block;
    if(file_offset < 0)
        file_offset = 0;

    file.seek(file_offset);

    bool bool_is_first_read = true;

    qint64 index = 0;
    QByteArray search_array;
    QByteArray ar;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    if(bool_clear_prev_list)
        search_saved_offsets_list.clear();

    int buffer_size = 102400;
    if(file.size() < buffer_size)
        buffer_size = file.size();

    char read_buffer[buffer_size];
    while(1)
    {
        QCoreApplication::processEvents();
        memset(read_buffer, 0,buffer_size);

        int d_read_count = file.read(read_buffer, buffer_size);
        if(d_read_count <= 0 || bool_cancel_loading)
        {
            break;
        }


        display_loading_progress_bar_obj->pub_set_label_messsge("Searching... " + recon_static_functions::human_readable_size(file.pos(), Q_FUNC_INFO));


        ar =  QByteArray::fromRawData(read_buffer, d_read_count);
        if(bool_is_first_read)
        {
            ar.remove(0, offset_in_block);
        }

        if(type.contains("ASCII"))
        {
            index = mar.indexIn(ar);
            search_array = text.toLocal8Bit();
        }
        if(type.contains("16"))
        {
            index = mar_unicode.indexIn(ar);
            search_array = QByteArray((const char*) (text.utf16()), text.size() * 2);
        }
        if(type.contains("Hex", Qt::CaseInsensitive))
        {
            index = mar_hex.indexIn(ar);
            search_array = text_to_search_in_hex;
        }

        if(index > 0)
        {
            pos += index;

            current_text_searched_pos = pos;

            search_saved_offsets_list << QString::number(current_text_searched_pos);

            struct_hex_viewer_tag tag_highlight;
            tag_highlight.start_address = pos;
            tag_highlight.end_address = pos + search_array.size() -1;
            QList<struct_hex_viewer_tag> highlight_tags_list;
            highlight_tags_list.append(tag_highlight);
            hex_widget->pub_set_hightlight_tags_list(highlight_tags_list);
            hex_widget->pub_set_jump_to_address(pos);
            hex_widget->setFocus();


            file.close();
            display_loading_progress_bar_obj->hide();
            return;
        }

        if(bool_is_first_read)
            bool_is_first_read = false;

        pos += ar.size();
    }


    if(current_text_searched_pos < 0 && search_saved_offsets_list.size() > 0)
    {
        QString last_matched_index = search_saved_offsets_list.at(search_saved_offsets_list.size() -1);
        current_text_searched_pos  = last_matched_index.toLongLong();
    }

    display_loading_progress_bar_obj->hide();
    file.close();
}

void hex_viewer_complete::slot_pushButton_search_keyword_clicked(bool status)
{
    Q_UNUSED(status);


    if(!check_is_data_available())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Data Available for Search.");
        return;
    }

    hex_viewer_search_dialog_obj->show();
}

void hex_viewer_complete::slot_pushButton_search_next_clicked(bool status)
{
    Q_UNUSED(status);

    if(!check_is_data_available())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Data Available for Search.");
        return;
    }

    current_cursor_position = current_cursor_position + 1;

    slot_search_text_and_type(current_search_text, current_search_type, false);

    search_saved_offsets_list.removeDuplicates();

}

void hex_viewer_complete::slot_pushButton_search_previous_clicked(bool status)
{
    Q_UNUSED(status);

    if(!check_is_data_available())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Data Available for Search.");
        return;
    }

    if(search_saved_offsets_list.isEmpty())
        return;

    search_saved_offsets_list.removeDuplicates();


    if(search_saved_offsets_list.contains(QString::number(current_text_searched_pos)))
    {

        QByteArray search_array;
        if(current_search_type.contains("ASCII"))
        {
            search_array = current_search_text.toLocal8Bit();
        }
        if(current_search_type.contains("16"))
        {
            search_array = QByteArray((const char*) (current_search_text.utf16()), current_search_text.size() * 2);
        }
        if(current_search_type.contains("Hex", Qt::CaseInsensitive))
        {
            QByteArray text_to_search_in_hex = QByteArray::fromHex(current_search_text.toLocal8Bit());
            search_array = text_to_search_in_hex;
        }


        int pos = search_saved_offsets_list.indexOf(QString::number(current_text_searched_pos));

        pos -= 1; //jump to previous match position from current position

        if(pos < 0)
            pos = 0;

        if(search_saved_offsets_list.size() < pos)
            return;

        QString pos_str = search_saved_offsets_list.at(pos);
        current_text_searched_pos = pos_str.toLongLong();

        struct_hex_viewer_tag tag_highlight;
        tag_highlight.start_address = current_text_searched_pos;
        tag_highlight.end_address = current_text_searched_pos + search_array.size() -1;
        QList<struct_hex_viewer_tag> highlight_tags_list;
        highlight_tags_list.append(tag_highlight);
        hex_widget->pub_set_hightlight_tags_list(highlight_tags_list);
        hex_widget->pub_set_jump_to_address(tag_highlight.start_address);
        hex_widget->setFocus();

        hex_widget->pub_refresh_view();

    }
}

void hex_viewer_complete::slot_pushButton_jump_to_offset(bool status)
{
    Q_UNUSED(status)

    if(!check_is_data_available())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Data Available for Go to Offset.");
        return;
    }

    get_address_dialog_obj->show();
}

void hex_viewer_complete::slot_jump_to_offset(qint64 offset, int s1)
{
    if(s1 == enum_offset_jump_from_current)
        offset = current_cursor_position + offset;
    if(s1 == enum_offset_jump_from_back)
        offset = QFileInfo(source_file_path).size() - offset;

    hex_widget->pub_set_jump_to_address(offset);

    hex_widget->setCursorPosition(offset * 2);

    hex_widget->setFocus();
}

void hex_viewer_complete::slot_action_start_block_triggered(bool)
{
    marked_start_offset = current_cursor_position;
}

void hex_viewer_complete::slot_action_end_block_triggered(bool)
{
    if(current_cursor_position < marked_start_offset)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "'End Offset' can not be less than 'Start Offset'.");
        return;
    }

    tag_create_popup_obj->pub_set_start_address(marked_start_offset);
    tag_create_popup_obj->pub_set_end_address(current_cursor_position);
    tag_create_popup_obj->show();

    struct_hex_viewer_tag tag_highlight;
    tag_highlight.start_address = marked_start_offset;
    tag_highlight.end_address = current_cursor_position;
    QList<struct_hex_viewer_tag> highlight_tags_list;
    highlight_tags_list.append(tag_highlight);
    hex_widget->pub_set_hightlight_tags_list(highlight_tags_list);
}

void hex_viewer_complete::slot_pushButton_carve_tag_clicked(bool)
{
    int row = tableWidget_tags_list->currentRow();
    if(row < 0)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Tag selected for Export Data.");
        return;
    }

    QString file_path;
    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            file_path = selected_files.at(0);
        }
    }
    if(file_path.trimmed().isEmpty())
        return;

    QString dir_name_str = QFileInfo(file_path).absoluteDir().absolutePath();
    QString filename = recon_static_functions::get_available_filename(QFileInfo(file_path).fileName(),dir_name_str,Q_FUNC_INFO);

    if(dir_name_str.endsWith("/"))
        file_path = dir_name_str + filename;
    else
        file_path = dir_name_str + QString("/") + filename;


    qint64 start_address = tableWidget_tags_list->item(row, enum_TAG_TABLE_START)->text().toLongLong();
    qint64 end_address   = tableWidget_tags_list->item(row, enum_TAG_TABLE_END)->text().toLongLong();

    QFile file(file_path);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        recon_static_functions::app_debug("dest file open -----FAILED----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QFile source(source_file_path);
    if(!source.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("source file open -----FAILED----" + source.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + source.errorString(), Q_FUNC_INFO);
        return;
    }
    source.seek(start_address);
    QByteArray source_data = source.read(end_address - start_address + 1);
    source.close();

    file.write(source_data,source_data.size());
    file.flush();
    file.close();

    message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,QStringList(file_path), dir_name_str);
    message_dialog_obj->show();

}

void hex_viewer_complete::slot_pushButton_export_tags_clicked(bool)
{
    int row = tableWidget_tags_list->currentRow();
    if(row < 0)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Tag selected for Export Tags.");
        return;
    }

    QString file_path;
    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            file_path = selected_files.at(0);
        }
    }
    if(file_path.trimmed().isEmpty())
        return;

    QString dir_name_str = QFileInfo(file_path).absoluteDir().absolutePath();
    QString filename = recon_static_functions::get_available_filename(QFileInfo(file_path).fileName(),dir_name_str,Q_FUNC_INFO);

    if(dir_name_str.endsWith("/"))
        file_path = dir_name_str + filename;
    else
        file_path = dir_name_str + QString("/") + filename;

    QFile file(file_path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        recon_static_functions::app_debug("file open -----FAILED----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    for(int i = 0 ; i < tableWidget_tags_list->rowCount(); i++)
    {
        QString line;

        line.append(tableWidget_tags_list->item(i, enum_TAG_TABLE_START)->text());
        line.append(",");

        line.append(tableWidget_tags_list->item(i, enum_TAG_TABLE_END)->text());
        line.append(",");


        line.append(tableWidget_tags_list->item(i, enum_TAG_TABLE_TAG_NAME)->text());
        line.append(",");

        line.append(tableWidget_tags_list->item(i, enum_TAG_TABLE_TAG_COLOR)->background().color().name());
        //line.append(",");

        file.write(line.toLocal8Bit(),line.toLocal8Bit().size());
        file.write("\n");

    }

    file.flush();
    file.close();

    message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,QStringList(file_path), dir_name_str);
    message_dialog_obj->show();

}

void hex_viewer_complete::slot_pushButton_import_tags_clicked(bool)
{
    if(!check_is_data_available())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Data Available in Hex View.");
        return;
    }

    QString file_path;
    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            file_path = selected_files.at(0);
        }
    }
    if(file_path.trimmed().isEmpty())
        return;

    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open -----FAILED----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QStringList all_lines;
    while(!file.atEnd())
    {
        all_lines << file.readLine();
    }
    file.close();

    for(int i = 0; i < all_lines.size(); i++)
    {
        QString line = all_lines.at(i);

        line = line.trimmed();

        QStringList line_parts = line.split(",");

        if(line_parts.size() < 3)
            continue;

        struct_hex_viewer_tag tag;
        QString start    = line_parts.at(0);
        QString end_addr = line_parts.at(1);
        QString tag_name = line_parts.at(2);
        QString color_name = line_parts.at(3);

        tag.start_address = start.toLongLong();
        tag.end_address = end_addr.toLongLong();
        tag.color_hex   = color_name;
        tag.tag_name_str = tag_name;

        hex_viewer_tags_list.append(tag);

        save_details_in_hex_viewer_feature_database(tag);
    }
    create_tag_display();

    QMessageBox::information(this, recon_static_functions::get_app_name(), "Tags Imported Successfully.");

}

void hex_viewer_complete::slot_action_template_fat_directory_entry_triggered(bool)
{
    generic_table_template_viewer *obj = new generic_table_template_viewer(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);

    QByteArray chunk = data_block_from_source(current_cursor_position, 32);

    fat16_parser fat16_parser_obj;
    QList<common_structure_for_data_table> list = fat16_parser_obj.pub_SFN_data(chunk);

    obj->pub_set_table_data(list);
    obj->show();

}

void hex_viewer_complete::slot_action_template_mbr_triggered(bool)
{
    QFile file(source_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open file for search -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file.seek(current_cursor_position);

    QByteArray chunk = file.read(512);

    file.close();


    mbr_template *obj = new mbr_template(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);

    obj->pub_set_data(chunk);

    obj->show();

}

void hex_viewer_complete::slot_action_template_ntfs_vbr_triggered(bool)
{
    QFile file(source_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open file for search -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file.seek(current_cursor_position);

    QByteArray chunk = file.read(512);

    file.close();


    ntfs_vbr_template *obj = new ntfs_vbr_template(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);

    obj->pub_set_data(chunk);

    obj->show();
}

void hex_viewer_complete::slot_action_template_mft_entry_triggered(bool)
{
    QFile file(source_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open file for search -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file.seek(current_cursor_position);
    QByteArray chunk = file.read(1024);
    file.close();

    MFT_entry_template *obj = new MFT_entry_template(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);

    obj->pub_set_data(chunk);

    obj->show();

}

void hex_viewer_complete::slot_action_template_gpt_triggered(bool)
{
    QFile file(source_file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open file for search -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file.seek(current_cursor_position);

    QByteArray chunk = file.read(512 + 512 + (512 * 32));

    file.close();

    gpt_template *obj = new gpt_template(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);

    obj->pub_set_data(chunk);

    obj->show();

}

void hex_viewer_complete::slot_action_template_fat32_vbr_triggered(bool)
{

    QFile file(source_file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open file for search -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file.seek(current_cursor_position);

    QByteArray chunk = file.read(512);

    file.close();

    QList<common_structure_for_data_table> list = fat32_parser_obj.pub_fat32_vbr_data(chunk);

    generic_table_template_viewer *obj = new generic_table_template_viewer(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);

    obj->pub_set_table_data(list);
    obj->show();
}

void hex_viewer_complete::slot_action_template_fat_vbr_triggered(bool)
{
    QFile file(source_file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open file for search -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    file.seek(current_cursor_position);

    QByteArray chunk = file.read(512);

    file.close();

    fat16_parser fat16_parser_obj;
    QList<common_structure_for_data_table> list = fat16_parser_obj.pub_fat16_vbr_data(chunk);

    generic_table_template_viewer *obj = new generic_table_template_viewer(this);
    obj->setAttribute(Qt::WA_DeleteOnClose);
    obj->pub_set_title("FAT16 VBR");

    obj->pub_set_table_data(list);
    obj->show();

}

void hex_viewer_complete::slot_pushButton_read_file_as_image_clicked(bool)
{
    hexviewer_progress_bar_obj->show();

    pushButton_read_file_as_image->setEnabled(false);
    fs_parser_obj.pub_set_image_path(source_file_path);
    fs_parser_obj.pub_read_image();

    ntfs_parser ntfs_parser_obj;

    int fs_counter = 1;
    image_part_list = fs_parser_obj.pub_get_partition_details();

    for(int i = 0 ;i < image_part_list.size(); i++)
    {
        partition_detail part = image_part_list.at(i);
        part.fs_viewer = NULL;

        if(part.file_system_name.size() < 1)
            continue;

        part.fs_viewer = new fs_viewer_tsk(this);
        part.fs_viewer->pub_set_fs_type(part.file_system_name);
        part.fs_viewer->pub_set_destination_database(&(part.fs_db));

        connect(part.fs_viewer, SIGNAL(signal_goto_offset(qint64)), this, SLOT(slot_goto_offset(qint64)));
        connect(part.fs_viewer, SIGNAL(signal_goto_fat32_file_content(int,qint64)), this, SLOT(slot_goto_fat32_file_content(int,qint64)));
        connect(part.fs_viewer, SIGNAL(signal_goto_fat32_dollar_files_content(int,QString)), this, SLOT(slot_goto_fat32_$files_content(int,QString)));
        connect(part.fs_viewer, SIGNAL(signal_goto_fat_file_content(int,qint64)), this, SLOT(slot_goto_fat_file_content(int,qint64)));
        connect(part.fs_viewer, SIGNAL(signal_goto_fat_dollar_files_content(int,QString)), this, SLOT(slot_goto_fat_$files_content(int,QString)));
        connect(part.fs_viewer, SIGNAL(signal_goto_ntfs_file_content(int,qint64)), this, SLOT(slot_goto_ntfs_file_content(int,qint64)));
        connect(part.fs_viewer, SIGNAL(signal_show_fat_template(int, QString)), this, SLOT(slot_show_fat_template(int, QString)));

        part.fs_viewer->pub_create_display("/");

        struct_hex_viewer_markers marker;
        marker.marker_address = part.byte_offset;
        marker.marker_partition = "Partition " + QString::number(fs_counter);
        marker.marker_description = "Partition Start Address";

        marker_list.append(marker);

        if(part.file_system_name.contains(MACRO_FILE_SYSTEM_TYPE_NTFS, Qt::CaseInsensitive))
        {
            part.fs_viewer->pub_set_partition_number(i);
            QFile mfile(source_file_path);

            if(!mfile.open(QIODevice::ReadOnly))
                continue;

            mfile.seek(part.byte_offset);

            QByteArray ntfs_vbr = mfile.read(512);

            mfile.close();

            qint64 mft_address =  ntfs_parser_obj.mft_address(ntfs_vbr);

            mft_address += part.byte_offset;
            if(mft_address > 0)
            {
                struct_hex_viewer_markers marker_ntfs_mft;
                marker_ntfs_mft.marker_address = mft_address;
                part.fs_viewer->pub_set_mft_address(mft_address);
                //                part.fs_viewer->pub_set_fs_type(part.file_system_name);
                marker_ntfs_mft.marker_partition = "Partition " + QString::number(fs_counter);
                marker_ntfs_mft.marker_description = "MFT Start Address";
                part.mft_address = mft_address;
                marker_list.append(marker_ntfs_mft);

                part.sector_per_cluster = ntfs_parser_obj.sector_per_cluster(ntfs_vbr);
                part.sector_size = ntfs_parser_obj.sector_size(ntfs_vbr);

            }

            image_part_list.removeAt(i);
            image_part_list.insert(i, part);
        }

        if(part.file_system_name.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive) && part.file_system_name.contains("32"))
        {


            QFile mfile(source_file_path);

            if(!mfile.open(QIODevice::ReadOnly))
                continue;

            mfile.seek(part.byte_offset);

            QByteArray fat32vbr = mfile.read(512);

            mfile.close();

            qint64 fat1_address =  fat32_parser_obj.pub_get_fat1_address(fat32vbr);
            qint64 fat2_address =  fat32_parser_obj.pub_get_fat2_address(fat32vbr);
            qint64 root_dir_address =  fat32_parser_obj.pub_get_root_directory_address(fat32vbr);
            int sector_size = fat32_parser_obj.pub_get_bytes_per_sector(fat32vbr);
            int sector_per_cluster = fat32_parser_obj.pub_get_sectors_per_cluster(fat32vbr);
            int fat_sector_count = fat32_parser_obj.pub_get_fat_sector_count(fat32vbr);

            part.sector_size = sector_size;
            part.sector_per_cluster = sector_per_cluster;
            part.fs_viewer->pub_set_partition_number(i);
            part.fat_sector_count = fat_sector_count;



            fat1_address += part.byte_offset;
            fat2_address += part.byte_offset;
            root_dir_address += part.byte_offset;

            if(fat1_address > 0)
            {
                struct_hex_viewer_markers marker_fat32_fat1;
                marker_fat32_fat1.marker_address = fat1_address;
                marker_fat32_fat1.marker_partition = "Partition " + QString::number(fs_counter);
                marker_fat32_fat1.marker_description = "FAT1 Address";
                marker_list.append(marker_fat32_fat1);
                part.fat1_address = fat1_address;
            }

            if(fat2_address > 0)
            {
                struct_hex_viewer_markers marker_fat32_fat2;
                marker_fat32_fat2.marker_address = fat2_address;
                marker_fat32_fat2.marker_partition = "Partition " + QString::number(fs_counter);
                marker_fat32_fat2.marker_description = "FAT2 Address";
                marker_list.append(marker_fat32_fat2);
                part.fat2_address = fat2_address;
            }

            if(root_dir_address > 0)
            {
                struct_hex_viewer_markers marker_root_directory;
                marker_root_directory.marker_address = root_dir_address;
                marker_root_directory.marker_partition = "Partition " + QString::number(fs_counter);
                marker_root_directory.marker_description = "Root Directory Address";
                marker_list.append(marker_root_directory);
                //                part.fs_viewer->pub_set_fs_type(part.file_system_name);
                part.fs_viewer->pub_set_root_directory_address(root_dir_address);
                part.root_directory_address = root_dir_address;
            }

            image_part_list.removeAt(i);
            image_part_list.insert(i, part);
        }

        if(part.file_system_name.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive) && part.file_system_name.contains("16"))
        {
            QFile mfile(source_file_path);

            if(!mfile.open(QIODevice::ReadOnly))
                continue;

            mfile.seek(part.byte_offset);

            QByteArray vbr = mfile.read(512);

            fat16_parser fat16_parser_obj;
            mfile.close();

            qint64 fat1_address =  fat16_parser_obj.pub_get_fat1_address(vbr);
            qint64 fat2_address =  fat16_parser_obj.pub_get_fat2_address(vbr);
            qint64 root_dir_address =  fat16_parser_obj.pub_get_root_directory_address(vbr);


            int sector_size = fat16_parser_obj.pub_get_bytes_per_sector(vbr);
            int sector_per_cluster = fat16_parser_obj.pub_get_sectors_per_cluster(vbr);
            int fat_sector_count = fat16_parser_obj.pub_get_fat_sector_count(vbr);


            part.sector_size = sector_size;
            part.sector_per_cluster = sector_per_cluster;
            part.fat_sector_count = fat_sector_count;
            part.fs_viewer->pub_set_partition_number(i);

            fat1_address += part.byte_offset;
            fat2_address += part.byte_offset;
            root_dir_address += part.byte_offset;

            if(fat1_address > 0)
            {
                struct_hex_viewer_markers fat1;
                fat1.marker_address = fat1_address;
                fat1.marker_partition = "Partition " + QString::number(fs_counter);
                fat1.marker_description = "FAT1 Address";
                marker_list.append(fat1);
                part.fat1_address = fat1_address;
            }

            if(fat2_address > 0)
            {
                struct_hex_viewer_markers fat2;
                fat2.marker_address = fat2_address;
                fat2.marker_partition = "Partition " + QString::number(fs_counter);
                fat2.marker_description = "FAT2 Address";
                marker_list.append(fat2);
                part.fat2_address = fat2_address;
            }

            if(root_dir_address > 0)
            {
                struct_hex_viewer_markers marker_root_directory;
                marker_root_directory.marker_address = root_dir_address;
                part.fs_viewer->pub_set_root_directory_address(root_dir_address);
                //                part.fs_viewer->pub_set_fs_type(part.file_system_name);
                marker_root_directory.marker_partition = "Partition " + QString::number(fs_counter);
                marker_root_directory.marker_description = "Root Directory Address";
                marker_list.append(marker_root_directory);
                part.root_directory_address = root_dir_address;
            }

            image_part_list.removeAt(i);
            image_part_list.insert(i, part);

        }

        fs_counter++;
    }

    create_marker_display();


    hexviewer_progress_bar_obj->hide();
}

void hex_viewer_complete::slot_goto_offset(qint64 address)
{
    hex_widget->pub_set_jump_to_address(address);
}

void hex_viewer_complete::slot_checkBox_little_endian_toggeled(bool)
{
    slot_cursor_position(current_cursor_position);
}

void hex_viewer_complete::slot_goto_fat32_file_content(int part_num, qint64 inode_number)
{
    partition_detail part = image_part_list.at(part_num);
    qint64 dir_entry_address = part.root_directory_address + ((inode_number - 3)*32);

    QByteArray dir_entry_data = data_block_from_source(dir_entry_address, 32);

    qint64 cluster_number = fat32_parser_obj.pub_get_cluster_number(dir_entry_data);

    slot_goto_offset(part.root_directory_address + ((cluster_number - 2)*(part.sector_per_cluster*part.sector_size)));
}


void hex_viewer_complete::slot_goto_fat32_$files_content(int part_num, QString file_name)
{
    partition_detail part = image_part_list.at(part_num);

    if(file_name == "$FAT1")
    {
        slot_goto_offset(part.fat1_address);
        return;
    }

    if(file_name == "$FAT2")
    {
        slot_goto_offset(part.fat2_address);
        return;
    }


    if(file_name == "$MBR")
    {
        slot_goto_offset(part.byte_offset);
        return;
    }

    if(file_name == ROOT_DIRECTORY)
    {
        slot_goto_offset(part.root_directory_address);
        return;
    }


}

void hex_viewer_complete::slot_goto_fat_file_content(int part_num, qint64 inode_number)
{

    fat16_parser m_parser;
    partition_detail part = image_part_list.at(part_num);

    qint64 dir_entry_address = part.root_directory_address + ((inode_number - 3)*32);

    QByteArray dir_entry_data = data_block_from_source(dir_entry_address, 32);

    qint64 cluster_number = m_parser.pub_get_cluster_number(dir_entry_data);

    slot_goto_offset(part.root_directory_address + ((cluster_number + 2)*(part.sector_per_cluster*part.sector_size)));

}

void hex_viewer_complete::slot_goto_fat_$files_content(int part_num, QString file_name)
{
    partition_detail part = image_part_list.at(part_num);

    if(file_name == "$FAT1")
    {
        slot_goto_offset(part.fat1_address);
        return;
    }

    if(file_name == "$FAT2")
    {
        slot_goto_offset(part.fat2_address);
        return;
    }


    if(file_name == "$MBR")
    {
        slot_goto_offset(part.byte_offset);
        return;
    }

    if(file_name == ROOT_DIRECTORY)
    {
        slot_goto_offset(part.root_directory_address);
        return;
    }

}

void hex_viewer_complete::slot_goto_ntfs_file_content(int part_num, qint64 inode_number)
{
    partition_detail part = image_part_list.at(part_num);


    ntfs_parser obj;

    QByteArray mft_entry_data = data_block_from_source((part.mft_address + (inode_number*1024)), 1024);

    qint64 first_cluster =  obj.get_first_cluster_number(mft_entry_data); /*<< part.sector_per_cluster << part.sector_size;*/

    if(first_cluster > 0)
    {
        qint64 target_offset = part.byte_offset + (first_cluster * part.sector_per_cluster * part.sector_size);
        slot_goto_offset(target_offset);
    }
    else
    {
        qint64 target_offset = part.mft_address + (inode_number * 1024);
        slot_goto_offset(target_offset);
    }

}

void hex_viewer_complete::slot_show_fat_template(int part_num, QString fat_name)
{
    partition_detail part = image_part_list.at(part_num);
    qint64 address_to_fat;

    if(fat_name.contains("1"))
        address_to_fat = part.fat1_address;

    if(fat_name.contains("2"))
        address_to_fat = part.fat2_address;

    FAT_Template *dialog = new FAT_Template(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    QByteArray fat_data = data_block_from_source(address_to_fat, (part.fat_sector_count * part.sector_size));

    fat_data = fat_data.remove(4096, 10000000);
    int fat_type;

    if(part.file_system_name.contains("12"))
        fat_type = 12;

    if(part.file_system_name.contains("16"))
        fat_type = 16;

    if(part.file_system_name.contains("32"))
        fat_type = 32;

    dialog->pub_set_fat_type(fat_type);

    dialog->pub_set_fat_data(fat_data);

    dialog->show();


}

void hex_viewer_complete::slot_pushButton_remove_tag_triggered(bool)
{
    QModelIndexList selection_model_list = tableWidget_tags_list->selectionModel()->selectedRows();

    if(selection_model_list.size() < 1)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No Tag selected for Remove.");
        return;
    }

    for(int i = selection_model_list.size() - 1; i >= 0; i--)
    {
        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();

        if(selected_row_no < 0)
            continue;

        remove_tag_from_hex_features_database_and_export_details(hex_viewer_tags_list.at(selected_row_no), selected_row_no);

        hex_viewer_tags_list.removeAt(selected_row_no);

        tableWidget_tags_list->removeRow(selected_row_no);
    }

    //create_tag_display();

    hex_widget->pub_refresh_view();
}

void hex_viewer_complete::remove_tag_from_hex_features_database_and_export_details(struct_hex_viewer_tag tag_obj, int row_count)
{
    //remove record from 'hex_viewer_blocks'
    QString hex_ftchr_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    QString command_str = QString("Delete from hex_viewer_blocks Where block_name = ? AND block_start_offset = ? AND block_end_offset = ?");
    QStringList vals_list;
    vals_list << tag_obj.tag_name_str << QString::number(tag_obj.start_address) << QString::number(tag_obj.end_address);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, vals_list, hex_ftchr_db_path, Q_FUNC_INFO);

    //remove record from 'hex_viewer_content_info'
    vals_list.clear();
    vals_list << hex_viewer_open_file_data_obj.display_file_path
              << QString::number(tag_obj.start_address)
              << QString::number(tag_obj.end_address)
              << tag_obj.tag_name_str
              << hex_viewer_open_file_data_obj.source_count_name;

    QString hex_cntnt_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";
    command_str = QString("Select content_file_path from hex_content_info Where file_path = ? AND start_index = ? AND end_index = ? AND recon_tag_value = ? AND source_count_name = ?");

    QString content_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, vals_list, 0, hex_cntnt_db_path, Q_FUNC_INFO);
    vals_list.clear();
    vals_list << content_file_path;
    command_str = QString("Delete from hex_content_info Where content_file_path = ?");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, vals_list, hex_cntnt_db_path, Q_FUNC_INFO);

    content_file_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString());
    //content_file_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString());

    recon_static_functions::remove_safely_blank_dir_OR_file(content_file_path, Q_FUNC_INFO);


    ///remove from tag search db
    QString record_count = tableWidget_tags_list->item(row_count, enum_TAG_TABLE_INT)->text().trimmed();
    QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation;
    struct_global_tagging_tag_notes_db_updation obj;
    obj.plugin_name = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.tab_name   = hex_viewer_open_file_data_obj.hex_viewer_display_name;
    obj.record_no  = record_count;

    obj.item2 = QString::number(tag_obj.start_address);
    obj.item3 = QString::number(tag_obj.end_address);

    obj.source_count_name = hex_viewer_open_file_data_obj.source_count_name;

    list_st_global_tagging_db_updation << obj;

    global_connection_manager_obj->pub_delete_tags_for_hex_viewer(list_st_global_tagging_db_updation , tag_obj.tag_name_str);
}

void hex_viewer_complete::slot_pushButton_save_clicked(bool)
{

    QString tmpFileName = source_file_path + ".~tmp";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile file(tmpFileName);
    bool ok = hex_widget->write(file);

    if(!ok)
    {
        recon_static_functions::app_debug("----FAILED----- to write temp file", Q_FUNC_INFO);
        return;
    }

    QFile a_source(source_file_path);

    if(!a_source.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        recon_static_functions::app_debug("truncate source file -----FAILED-----" + a_source.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + a_source.errorString(), Q_FUNC_INFO);
        return;
    }

    QFile a_tm_file(tmpFileName);

    if(!a_tm_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("open tmp file for read -----FAILED-----" + a_tm_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + a_tm_file.errorString(), Q_FUNC_INFO);
        return;
    }

    qint64 data_written = 0;
    qint64 a_size = QFileInfo(a_tm_file).size();

    while(!a_tm_file.atEnd())
    {
        QByteArray input_data = a_tm_file.read(102400);

        data_written += a_source.write(input_data,input_data.size());
    }

    a_tm_file.close();

    QFile::remove(tmpFileName);
    a_source.close();

    QApplication::restoreOverrideCursor();

    if(data_written != a_size)
    {
        recon_static_functions::app_debug("----FAILED---- to write changes", Q_FUNC_INFO);
        return;
    }


    QMessageBox::information(this, recon_static_functions::get_app_name(), "Changes saved.");
}

void hex_viewer_complete::pub_downsize_hexviewer_for_metadata_display()
{
    side_widget->hide();
    toolbar_widget->hide();
}

void hex_viewer_complete::create_ui()
{
    main_tab_widget_obj = new QTabWidget(this);
    main_tab_widget_obj->hide();

    QVBoxLayout *main_vbox = new QVBoxLayout;

    ///=========Sidebar widget-(Start)==============///
    side_widget = new QWidget(this);
    side_widget->setMinimumWidth(100);
    side_widget->setMaximumWidth(500);
    tableWidget_data_interpreter = new QTableWidget(side_widget);
    tableWidget_data_interpreter->setMaximumHeight(800);

    tableWidget_data_interpreter->setEditTriggers(QAbstractItemView::NoEditTriggers);


    checkBox_little_endian = new QCheckBox("Use Little Endian format for Integers");
    connect(checkBox_little_endian, SIGNAL(toggled(bool)), this, SLOT(slot_checkBox_little_endian_toggeled(bool)));

    QFrame *frame_lables = new QFrame(this);
    QVBoxLayout *vbox_labels = new QVBoxLayout(frame_lables);
    vbox_labels->setContentsMargins(0,0,0,0);
    frame_lables->setContentsMargins(0,0,0,0);

    label_selection_start = new QLabel(frame_lables);
    label_selection_count = new QLabel(frame_lables);
    label_cursor_position = new QLabel(frame_lables);

    label_selection_start->setText("Selection : ");
    label_selection_count->setText("Selection Count : ");

    vbox_labels->addWidget(label_selection_start);
    vbox_labels->addWidget(label_selection_count);
    vbox_labels->addWidget(label_cursor_position);

    QVBoxLayout *vbox_side_widget = new QVBoxLayout(side_widget);
    vbox_side_widget->addWidget(checkBox_little_endian);
    vbox_side_widget->addWidget(tableWidget_data_interpreter);
    vbox_side_widget->addWidget(frame_lables);
    ///=========Sidebar widget-(End)==============///

    ///=========File Details-Sidebar-(Start)==============///
    QFrame *frame_file_details = new QFrame(this);
    QVBoxLayout *vbox_main_file_labels = new QVBoxLayout(frame_file_details);
    vbox_main_file_labels->setContentsMargins(0,0,0,0);
    frame_file_details->setContentsMargins(0,0,0,0);

    QHBoxLayout *hbox_labels_1 = new QHBoxLayout();
    label_filename_title = new QLabel(QString("File Name :"),frame_file_details);
    label_filename_title->setFixedWidth(90);
    label_filename_value = new QLabel(frame_file_details);
    hbox_labels_1->addWidget(label_filename_title);
    hbox_labels_1->addWidget(label_filename_value);
    hbox_labels_1->setContentsMargins(0,0,0,0);
    hbox_labels_1->setSpacing(0);

    QHBoxLayout *hbox_labels_2 = new QHBoxLayout();
    label_filepath_title = new QLabel(QString("File Path :"),frame_file_details);
    label_filepath_title->setFixedWidth(90);
    label_filepath_value = new QLabel(frame_file_details);
    hbox_labels_2->addWidget(label_filepath_title);
    hbox_labels_2->addWidget(label_filepath_value);
    hbox_labels_2->setContentsMargins(0,0,0,0);
    hbox_labels_2->setSpacing(0);

    QHBoxLayout *hbox_labels_3 = new QHBoxLayout();
    label_filesize_title = new QLabel(QString("File Size :"),frame_file_details);
    label_filesize_title->setFixedWidth(90);
    label_filesize_value = new QLabel(frame_file_details);
    hbox_labels_3->addWidget(label_filesize_title);
    hbox_labels_3->addWidget(label_filesize_value);
    hbox_labels_3->setContentsMargins(0,0,0,0);
    hbox_labels_3->setSpacing(0);

    QLabel *blank_label = new QLabel(frame_file_details);
    vbox_main_file_labels->addWidget(blank_label);

    vbox_main_file_labels->addLayout(hbox_labels_1);
    vbox_main_file_labels->addLayout(hbox_labels_2);
    vbox_main_file_labels->addLayout(hbox_labels_3);
    vbox_main_file_labels->setContentsMargins(0,0,0,0);
    vbox_main_file_labels->setSpacing(0);

    vbox_side_widget->addWidget(frame_file_details);
    vbox_side_widget->addStretch();
    vbox_side_widget->setContentsMargins(2,0,2,2);
    ///=========File Details-Sidebar-(End)==============///



    hex_widget = new QHexEdit_hexedit(this);
    hex_widget->pub_set_64_bit_scroll_bar_obj(scroll_bar_64_obj);
    hex_widget->setReadOnly(true);
    connect(hex_widget, SIGNAL(signal_selected_data_offsets(qint64,qint64)), this, SLOT(slot_selected_data_offsets(qint64,qint64)));
    connect(hex_widget, SIGNAL(signal_cursor_position(qint64)), this, SLOT(slot_cursor_position(qint64)));
    connect(hex_widget, SIGNAL(signal_right_click()), this, SLOT(slot_hex_widget_right_clicked()));
    connect(hex_widget, SIGNAL(signal_left_click()), this, SLOT(slot_hex_widget_left_clicked()));
    hex_widget->pub_set_tags_list_obj(&hex_viewer_tags_list);


    pushButton_show_hide_tags = new QPushButton("Show Tags", this);
    pushButton_show_hide_tags->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_show_hide_tags, SIGNAL(clicked()), this, SLOT(slot_pushButton_show_tables_clicked()));

    pushButton_read_file_as_image = new QPushButton("Read as Image", this);
    pushButton_read_file_as_image->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_read_file_as_image, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_read_file_as_image_clicked(bool)));

    tableWidget_tags_list = new QTableWidget(this);

    tableWidget_tags_list->setAlternatingRowColors(true);
    tableWidget_tags_list->setShowGrid(false);
    tableWidget_tags_list->horizontalHeader()->setStretchLastSection(true);

    connect(tableWidget_tags_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slot_tab_tablewidget_cell_double_clicked(int,int)));

    tag_and_table_widget = new QFrame(this);
    QVBoxLayout *vbox_tag_and_table = new QVBoxLayout(tag_and_table_widget);
    vbox_tag_and_table->setContentsMargins(0,0,0,0);
    tag_and_table_widget->setContentsMargins(0,0,0,0);

    tag_bar_widget = new QFrame(this);
    tag_bar_widget->setContentsMargins(0,0,0,0);
    QHBoxLayout *hbox_tag_bar = new QHBoxLayout(tag_bar_widget);
    hbox_tag_bar->setContentsMargins(0,0,0,0);
    pushButton_remove_tag = new QPushButton("Remove", this);
    pushButton_remove_tag->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_remove_tag, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_remove_tag_triggered(bool)));

    pushButton_carve_tag = new QPushButton("Export Data", this);
    pushButton_carve_tag->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_carve_tag, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_carve_tag_clicked(bool)));

    pushButton_export_tags = new QPushButton("Export Tags", this);
    pushButton_export_tags->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_export_tags, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_export_tags_clicked(bool)));

    pushButton_import_tags = new QPushButton("Import Tags", this);
    pushButton_import_tags->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_import_tags, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_import_tags_clicked(bool)));

    pushButton_export_tags->hide();
    pushButton_import_tags->hide();


    hbox_tag_bar->addWidget(pushButton_remove_tag);
    hbox_tag_bar->addWidget(pushButton_carve_tag);
    hbox_tag_bar->addWidget(pushButton_export_tags);
    hbox_tag_bar->addWidget(pushButton_import_tags);
    hbox_tag_bar->addStretch();


    vbox_tag_and_table->addWidget(tableWidget_tags_list);
    vbox_tag_and_table->addWidget(tag_bar_widget);


    main_tab_widget_obj->addTab(tag_and_table_widget, "Tags");

    QHBoxLayout *hbox_hex_scroll_bar = new QHBoxLayout;
    hbox_hex_scroll_bar->addWidget(hex_widget);
    hbox_hex_scroll_bar->addWidget(scroll_bar_64_obj);
    hbox_hex_scroll_bar->setStretch(0,1);
    hbox_hex_scroll_bar->setStretch(1,0);
    hbox_hex_scroll_bar->setContentsMargins(0,0,0,0);
    hbox_hex_scroll_bar->setSpacing(0);

    QWidget *hex_scroll_bar_wdgt = new QWidget(this);
    hex_scroll_bar_wdgt->setLayout(hbox_hex_scroll_bar);


    QSplitter *splitter_hex_widget = new QSplitter(Qt::Vertical);
    splitter_hex_widget->setContentsMargins(0,0,0,0);
    splitter_hex_widget->addWidget(main_tab_widget_obj);
    splitter_hex_widget->addWidget(hex_scroll_bar_wdgt);
    QList<int> t_sizes;
    t_sizes << 200 << 1000;
    splitter_hex_widget->setSizes(t_sizes);

    QSplitter *splitter_hex_and_side_widget = new QSplitter(Qt::Horizontal);
    splitter_hex_and_side_widget->setContentsMargins(0,0,0,0);
    splitter_hex_and_side_widget->addWidget(splitter_hex_widget);
    splitter_hex_and_side_widget->addWidget(side_widget);
    QList<int> a_sizes;
    a_sizes << 1000 << 250;
    splitter_hex_and_side_widget->setSizes(a_sizes);


    ///====toolbar widget==Starts====///
    toolbar_widget = new QFrame(this);
    toolbar_widget->setMinimumHeight(26);
    QHBoxLayout *hbox_toolbar_widget = new QHBoxLayout(toolbar_widget);
    hbox_toolbar_widget->setContentsMargins(0,0,0,0);

    label_bytes_per_line = new QLabel(this);
    label_bytes_per_line->setText("Bytes per line");
    label_bytes_per_line->setMinimumWidth(50);
    comboBox_bytes_per_line = new QComboBox(this);
    connect(comboBox_bytes_per_line, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_comboBox_bytes_per_line_current_index_changed(QString)));

    comboBox_bytes_per_line->addItems(QStringList() << "2" << "4" << "8" << "16" << "32");
    comboBox_bytes_per_line->setCurrentText("16");
    hex_widget->setBytesPerLine(16);


    pushButton_search_keyword = new QPushButton("Search", this);
    pushButton_search_keyword->setToolTip("Search");
    pushButton_search_keyword->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_search_keyword, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_search_keyword_clicked(bool)));

    pushButton_search_next = new QPushButton(">", this);
    pushButton_search_next->setToolTip("Search Next");
    pushButton_search_next->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_search_next, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_search_next_clicked(bool)));

    pushButton_search_previous = new QPushButton("<", this);
    pushButton_search_previous->setToolTip("Search Previous");
    pushButton_search_previous->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_search_previous, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_search_previous_clicked(bool)));

    pushButton_jump_to_offset_in_file = new QPushButton("Go to Offset", this);
    pushButton_jump_to_offset_in_file->setToolTip("Go to Offset");
    pushButton_jump_to_offset_in_file->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_jump_to_offset_in_file, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_jump_to_offset(bool)));

    pushButton_save = new QPushButton("Save", this);
    pushButton_save->setToolTip("Save");
    pushButton_save->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(pushButton_save, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_save_clicked(bool)));


    QHBoxLayout *hbox_top_layout_1 = new QHBoxLayout();
    hbox_top_layout_1->addWidget(label_bytes_per_line);
    hbox_top_layout_1->addWidget(comboBox_bytes_per_line);
    hbox_top_layout_1->setSpacing(4);

    QHBoxLayout *hbox_top_layout_2 = new QHBoxLayout();
    hbox_top_layout_2->addWidget(pushButton_search_previous);
    hbox_top_layout_2->addWidget(pushButton_search_keyword);
    hbox_top_layout_2->addWidget(pushButton_search_next);
    hbox_top_layout_2->setSpacing(2);

    QHBoxLayout *hbox_top_layout_3 = new QHBoxLayout();
    hbox_top_layout_3->addWidget(pushButton_jump_to_offset_in_file);
    hbox_top_layout_3->addWidget(pushButton_show_hide_tags);
    hbox_top_layout_3->setSpacing(5);


    hbox_toolbar_widget->addWidget(pushButton_save);
    hbox_toolbar_widget->addLayout(hbox_top_layout_1);
    hbox_toolbar_widget->addLayout(hbox_top_layout_2);
    hbox_toolbar_widget->addLayout(hbox_top_layout_3);
    hbox_toolbar_widget->addWidget(pushButton_read_file_as_image);
    hbox_toolbar_widget->addStretch();

    main_vbox->addWidget(toolbar_widget,0);
    main_vbox->addWidget(splitter_hex_and_side_widget, 1);

    main_vbox->setContentsMargins(0,0,0,0);
    setLayout(main_vbox);
    ///====toolbar widget==Ends====///


    ///create right clisk actions
    QAction *action_tag_bytes;
    QAction *action_start_block;
    QAction *end_block;
    menu_right_click = new QMenu(this);
    action_tag_bytes = menu_right_click->addAction("Tag Selected bytes");
    action_start_block = menu_right_click->addAction("Start Block");
    end_block = menu_right_click->addAction("End Block");

    connect(action_tag_bytes, SIGNAL(triggered(bool)), this, SLOT(slot_action_tag_bytes_triggered(bool)));
    connect(action_start_block, SIGNAL(triggered(bool)), this, SLOT(slot_action_start_block_triggered(bool)));
    connect(end_block, SIGNAL(triggered(bool)), this, SLOT(slot_action_end_block_triggered(bool)));

    menu_right_click_copy = new QMenu("Copy", this);
    QAction *action_copy_selected_bytes;
    QAction *action_copy_selected_ascii;
    action_copy_selected_bytes = menu_right_click_copy->addAction("Selected Bytes");
    action_copy_selected_ascii = menu_right_click_copy->addAction("Selected ASCII");

    menu_right_click->addMenu(menu_right_click_copy);

    connect(action_copy_selected_bytes, SIGNAL(triggered(bool)), this, SLOT(slot_action_copy_selected_bytes_triggered(bool)));
    connect(action_copy_selected_ascii, SIGNAL(triggered(bool)), this, SLOT(slot_action_copy_selected_ascii_triggered(bool)));

    tag_create_popup_obj = new tag_create_popup(this);
    connect(tag_create_popup_obj, SIGNAL(signal_save_tag_details(qint64,qint64,QString,QString)), this, SLOT(slot_save_tag_details(qint64,qint64,QString,QString)));
}

void hex_viewer_complete::create_and_add_marker_widget()
{
    marker_table_widget_frame = new QFrame(this);
    tableWidget_marker = new QTableWidget(this);
    tableWidget_marker->setWordWrap(false);
    connect(tableWidget_marker, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slot_tableWidget_marker_cell_double_clicked(int,int)));

    QVBoxLayout *vbox = new QVBoxLayout(marker_table_widget_frame);

    vbox->addWidget(tableWidget_marker);

    main_tab_widget_obj->addTab(marker_table_widget_frame, "Markers");

    tableWidget_marker->setColumnCount(enum_marker_table_column_count);

    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_INT, new QTableWidgetItem);
    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_offset_dec, new QTableWidgetItem("Offset (DEC)"));
    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_offset_hex, new QTableWidgetItem("Offset (HEX)"));
    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_partition, new QTableWidgetItem("Partition"));
    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_item, new QTableWidgetItem("Item"));
    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_description, new QTableWidgetItem("Description"));
    tableWidget_marker->setHorizontalHeaderItem(enum_marker_table_blank, new QTableWidgetItem);

    tableWidget_marker->hideColumn(enum_marker_table_INT);

    tableWidget_marker->horizontalHeader()->setStretchLastSection(true);
}

void hex_viewer_complete::create_marker_display()
{
    tableWidget_marker->setRowCount(0);
    //tableWidget_tags_list->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget_marker->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_marker->setEditTriggers(QAbstractItemView::NoEditTriggers);


    for(int i = 0; i < marker_list.size(); i++)
    {
        struct_hex_viewer_markers mark = marker_list.at(i);

        tableWidget_marker->insertRow(i);
        tableWidget_marker->setItem(i, enum_marker_table_INT, new QTableWidgetItem(QString::number(i)));
        tableWidget_marker->setItem(i, enum_marker_table_offset_dec, new QTableWidgetItem(QString::number(mark.marker_address)));
        tableWidget_marker->setItem(i, enum_marker_table_offset_hex, new QTableWidgetItem(QString::number(mark.marker_address, 16)));
        tableWidget_marker->setItem(i, enum_marker_table_partition, new QTableWidgetItem(mark.marker_partition));
        tableWidget_marker->setItem(i, enum_marker_table_item, new QTableWidgetItem(""));
        tableWidget_marker->setItem(i, enum_marker_table_description, new QTableWidgetItem(mark.marker_description));
        tableWidget_marker->setItem(i, enum_marker_table_blank, new QTableWidgetItem(""));

    }

}

QByteArray hex_viewer_complete::get_selected_data_block_from_source(qint64 start_offset, int size)
{
    QByteArray mdata;

    source_file_path = source_file_path.trimmed();
    if(source_file_path.isEmpty())
        return mdata;

    QFile file(source_file_path);

    if(size > file.size())
        return mdata;

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open File -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return mdata;
    }


    file.seek(start_offset);
    mdata = file.read(size);
    file.close();

    return mdata;
}

QByteArray hex_viewer_complete::data_block_from_source(qint64 offset, int size)
{
    QByteArray mdata;

    source_file_path = source_file_path.trimmed();
    if(source_file_path.isEmpty())
        return mdata;

    QFile file(source_file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("Open File -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return mdata;
    }

    if(offset % source_file_block_size == 0 && size % source_file_block_size == 0)
    {
        file.seek(offset);
        mdata = file.read(size);
        file.close();
        return mdata;
    }


    int offset_in_block = offset % source_file_block_size;
    int offset_in_last_block = (offset + size) % source_file_block_size;

    int start_of_block = offset - offset_in_block;

    file.seek(start_of_block);

    int size_to_read = offset_in_block +  size + (source_file_block_size -  offset_in_last_block);

    mdata = file.read(size_to_read);

    mdata = mdata.remove(0, offset_in_block);

    file.close();
    return mdata;

}



void hex_viewer_complete::create_tag_display()
{
    tableWidget_tags_list->setRowCount(0);

    tableWidget_tags_list->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWidget_tags_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_tags_list->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList header_item_list;

    header_item_list << "Sr." << "Start Address" << "End Address" << "Tag Name" << "Tag Color" << "Tag Data" << "Description" << "";

    tableWidget_tags_list->setColumnCount(header_item_list.size());
    tableWidget_tags_list->setHorizontalHeaderLabels(header_item_list);

    for(int row = 0; row < hex_viewer_tags_list.size(); row++)
    {
        struct_hex_viewer_tag tag = hex_viewer_tags_list.at(row);

        tableWidget_tags_list->insertRow(row);
        tableWidget_tags_list->setRowHeight(row, 20);


        QTableWidgetItem *item_INT   = new QTableWidgetItem;
        QTableWidgetItem *item_start = new QTableWidgetItem;
        QTableWidgetItem *item_end   = new QTableWidgetItem;
        QTableWidgetItem *item_tag_name  = new QTableWidgetItem;
        QTableWidgetItem *item_tag_color = new QTableWidgetItem;

        QTableWidgetItem *item_tag_hex_data = new QTableWidgetItem;

        QTableWidgetItem *item_description = new QTableWidgetItem;
        QTableWidgetItem *item_blank = new QTableWidgetItem;

        item_INT->setText(QString::number(row + 1));

        item_start->setText(QString::number(tag.start_address));
        item_end->setText(QString::number(tag.end_address));
        item_tag_name->setText(tag.tag_name_str);
        item_tag_color->setBackground(QColor(tag.color_hex));
        item_description->setText(tag.description);


        item_tag_hex_data->setText(get_tagged_data(tag.start_address, tag.end_address));

        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_INT, item_INT);
        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_START, item_start);
        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_END, item_end);
        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_TAG_NAME, item_tag_name);
        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_TAG_COLOR, item_tag_color);

        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_TAG_HEX_DATA, item_tag_hex_data);

        tableWidget_tags_list->setItem(row, enum_TAG_TABLE_DESCRIPTION, item_description);
        tableWidget_tags_list->setItem(row, enum_TAB_TABLE_BLANK, item_blank);

    }

    tableWidget_tags_list->setColumnWidth(enum_TAG_TABLE_TAG_HEX_DATA, 300);

}

QString hex_viewer_complete::get_tagged_data(qint64 start_offset, qint64 end_offset)
{
    if(end_offset < start_offset)
        return QString("");

    QFile file(source_file_path);
    if(!file.open(QIODevice::ReadOnly))
        return QString("");

    file.seek(start_offset);

    qint64 bytes = end_offset - start_offset;
    bytes += 1; //include last byte as well

    QByteArray data_arr = file.read(bytes);
    file.close();

    QString data_str = QString::fromLocal8Bit(data_arr.toHex().toUpper(), data_arr.toHex().size());

    return data_str;
}

void hex_viewer_complete::create_data_interpreter_display()
{
    tableWidget_data_interpreter->setRowCount(0);
    tableWidget_data_interpreter->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget_data_interpreter->setSelectionBehavior(QAbstractItemView::SelectRows);


    QStringList header_item_list;
    header_item_list << "Data Type" << "Value";

    tableWidget_data_interpreter->setColumnCount(header_item_list.size());
    tableWidget_data_interpreter->setHorizontalHeaderLabels(header_item_list);

    tableWidget_data_interpreter->horizontalHeader()->setStretchLastSection(true);
    //tableWidget_data_interpreter->verticalHeader()->hide();
    tableWidget_data_interpreter->verticalHeader()->setSectionsMovable(true);


    QTableWidgetItem *item;
    item = new QTableWidgetItem("8 bit binary");
    QTableWidgetItem *item_value;
    item_value = new QTableWidgetItem;


    tableWidget_data_interpreter->insertRow(enum_data_interpreter_8_bit_binary);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_8_bit_binary, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_8_bit_binary, 1, item_value);

    item = new QTableWidgetItem("8 bit signed");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_8_bit_signed);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_8_bit_signed, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_8_bit_signed, 1, item_value);


    item = new QTableWidgetItem("8 bit unsigned");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_8_bit_unsigned);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_8_bit_unsigned, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_8_bit_unsigned, 1, item_value);


    item = new QTableWidgetItem("16 bit signed");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_16_bit_signed);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_16_bit_signed, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_16_bit_signed, 1, item_value);


    item = new QTableWidgetItem("16 bit unsigned");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_16_bit_unsigned);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_16_bit_unsigned, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_16_bit_unsigned, 1, item_value);

    //    item = new QTableWidgetItem("24 bit signed");
    //    item_value = new QTableWidgetItem;
    //    tableWidget_data_interpreter->insertRow(enum_data_interpreter_24_bit_signed);
    //    tableWidget_data_interpreter->setItem(enum_data_interpreter_24_bit_signed, 0, item);
    //    tableWidget_data_interpreter->setItem(enum_data_interpreter_24_bit_signed, 1, item_value);


    item = new QTableWidgetItem("32 bit signed");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_32_bit_signed);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_32_bit_signed, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_32_bit_signed, 1, item_value);

    item = new QTableWidgetItem("32 bit unsigned");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_32_bit_unsigned);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_32_bit_unsigned, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_32_bit_unsigned, 1, item_value);

    item = new QTableWidgetItem("64 bit signed");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_64_bit_signed);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_64_bit_signed, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_64_bit_signed, 1, item_value);

    item = new QTableWidgetItem("64 bit unsigned");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_64_bit_unsigned);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_64_bit_unsigned, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_64_bit_unsigned, 1, item_value);


    item = new QTableWidgetItem("DOS Time");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_DOS_time);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_DOS_time, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_DOS_time, 1, item_value);

    item = new QTableWidgetItem("DOS Date");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_DOS_date);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_DOS_date, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_DOS_date, 1, item_value);

    item = new QTableWidgetItem("Unix Time");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_Unix_timestamp);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_Unix_timestamp, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_Unix_timestamp, 1, item_value);

    item = new QTableWidgetItem("Windows Time");
    item_value = new QTableWidgetItem;
    tableWidget_data_interpreter->insertRow(enum_data_interpreter_Windows_timestamp);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_Windows_timestamp, 0, item);
    tableWidget_data_interpreter->setItem(enum_data_interpreter_Windows_timestamp, 1, item_value);

    //    item = new QTableWidgetItem("Mac Time");
    //    item_value = new QTableWidgetItem;
    //    tableWidget_data_interpreter->insertRow(enum_data_interpreter_Mac_timestamp);
    //    tableWidget_data_interpreter->setItem(enum_data_interpreter_Mac_timestamp, 0, item);
    //    tableWidget_data_interpreter->setItem(enum_data_interpreter_Mac_timestamp, 1, item_value);

}

void hex_viewer_complete::save_details_in_hex_viewer_feature_database(struct_hex_viewer_tag tag)
{
    ///add tag details to feature DB
    QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + QString("hex_viewer.sqlite");

    //=======create index in hex feature database===========//
    recon_helper_standard_obj->pub_add_file_details_to_hex_feature_db_for_sendto_bucket(hex_viewer_open_file_data_obj, Q_FUNC_INFO);


    //=======insert blocks in hex feature database===========//
    //check for record already exist in db
    QString command_str = "Select INT from hex_viewer_blocks where file_name = ? "
                          "AND file_path = ? AND block_name = ? AND block_start_offset = ? "
                          "AND block_end_offset = ? AND block_color = ? "
                          "AND start_page_no = ? AND viewer_display_name = ?";
    QStringList vals_list;
    vals_list << recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(hex_viewer_open_file_data_obj.display_file_path)
              << hex_viewer_open_file_data_obj.display_file_path
              << tag.tag_name_str
              << QString::number(tag.start_address)
              << QString::number(tag.end_address)
              << tag.color_hex
              << QString::number(1) //all data shown on single page
              << hex_viewer_open_file_data_obj.hex_viewer_display_name;

    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,vals_list,0,hex_feature_db_path,Q_FUNC_INFO);
    if(!INT_str.trimmed().isEmpty())
    {
        vals_list.clear();
        vals_list <<  tag.tag_name_str << QString::number(tag.start_address) << QString::number(tag.end_address) << tag.color_hex << QString::number(1);

        command_str = "UPDATE hex_viewer_blocks Set block_name = ?,block_start_offset = ?,block_end_offset = ? ,block_color = ?,start_page_no = ? "
                      "Where viewer_display_name = ? AND INT = ?";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str,vals_list,hex_feature_db_path,Q_FUNC_INFO);
    }
    else
    {
        vals_list.clear();
        command_str = QString("Insert into hex_viewer_blocks (file_name,file_path,block_name,block_start_offset,block_end_offset,block_color,"
                              "start_page_no,viewer_display_name,recon_filefrom,recon_file_infopath) Values(?,?,?,?,?, ?,?,?,?,?)");

        vals_list << recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(hex_viewer_open_file_data_obj.display_file_path)
                  << hex_viewer_open_file_data_obj.display_file_path
                  << tag.tag_name_str
                  << QString::number(tag.start_address)
                  << QString::number(tag.end_address)
                  << tag.color_hex
                  << QString::number(1) //all data shown on single page
                  << hex_viewer_open_file_data_obj.hex_viewer_display_name
                  << hex_viewer_open_file_data_obj.recon_filefrom
                  << hex_viewer_open_file_data_obj.recon_file_infopath;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, vals_list, hex_feature_db_path, Q_FUNC_INFO);
    }

    QString exported_file_path = save_tagged_data_to_hex_export_content(tag);
    QFile myfile(exported_file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + myfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + myfile.errorString(),Q_FUNC_INFO);
        return;
    }
    QByteArray export_data_arr = myfile.readAll();
    myfile.close();
    QString selected_data_str = QString::fromLocal8Bit(export_data_arr, export_data_arr.size());

    save_details_in_hex_viewer_export_content_database(tag, exported_file_path);

    QString content_export_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";
    command_str = QString("SELECT INT FROM hex_content_info ORDER BY INT DESC LIMIT 1");
    QString record_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str,0,content_export_db_path,Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_tgt_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(hex_viewer_open_file_data_obj.source_count_name);
    QString file_path_str = hex_viewer_open_file_data_obj.display_file_path;

    QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation;
    struct_global_tagging_tag_notes_db_updation obj;
    obj.plugin_name = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.category   = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.tab_name   = hex_viewer_open_file_data_obj.hex_viewer_display_name;
    obj.table_name = "hex_content_info";
    obj.record_no  = record_count;

    obj.item0 = QFileInfo(file_path_str).fileName();
    obj.item1 = file_path_str;
    obj.item2 = QString::number(tag.start_address);
    obj.item3 = QString::number(tag.end_address);
    obj.item4 = selected_data_str;

    obj.source_count_name = hex_viewer_open_file_data_obj.source_count_name;
    obj.os_scheme_display  = struct_tgt_src_info.os_scheme_display;

    obj.csv_plugin_name = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.csv_tab_name    = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.recon_tag_value = tag.tag_name_str;

    list_st_global_tagging_db_updation << obj;

    global_connection_manager_obj->pub_add_tags_for_hex_viewer(list_st_global_tagging_db_updation , tag.tag_name_str);

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Hex_Viewer));
}

QString hex_viewer_complete::save_tagged_data_to_hex_export_content(struct_hex_viewer_tag tag)
{
    QString exported_file_path;

    ///prepare selected data
    QFile myfile(hex_viewer_open_file_data_obj.complete_file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + myfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + myfile.errorString(),Q_FUNC_INFO);
        return exported_file_path;
    }
    myfile.seek(tag.start_address);
    QByteArray selected_array = myfile.read(tag.end_address - tag.start_address + 1);
    myfile.close();

    ///save selected data to hex export in result
    QString file_dir_name = hex_viewer_open_file_data_obj.hex_viewer_display_name;
    file_dir_name.replace(" ","_");

    QString content_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + file_dir_name;
    QDir dir(content_dir_path);
    if(!dir.exists())
        dir.mkdir(content_dir_path);

    QString tag_name_tmp = tag.tag_name_str;

    QString file_name = tag_name_tmp.replace(" ","_") + "_" + QString::number(tag.start_address) + "_" + QString::number(tag.end_address);

    QString svd_file_path = content_dir_path + "/" + file_name;
    QFile file_write(svd_file_path);
    if(!file_write.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + svd_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file_write.errorString(),Q_FUNC_INFO);
        return exported_file_path;
    }

    file_write.write(selected_array,selected_array.size());
    file_write.flush();
    file_write.close();

    exported_file_path = svd_file_path;

    return exported_file_path;

}

void hex_viewer_complete::save_details_in_hex_viewer_export_content_database(struct_hex_viewer_tag tag, QString exported_file_path)
{
    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";
    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'hex_content_info'(INT INTEGER PRIMARY KEY,bookmark,file_name VARCHAR(1000), file_path VARCHAR(1000),"
                          "file_size VARCHAR(1000), start_index VARCHAR(1000), end_index VARCHAR(1000), recon_tag_value VARCHAR(1000),"
                          "color_hex VARCHAR(1000), content_file_path VARCHAR(1000), source_count_name VARCHAR(1000), viewer_display_name VARCHAR(1000), notes VARCHAR(1000), hex_data BLOB,"
                          "LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000) , recon_filefrom varchar(10),recon_file_infopath varchar(300))";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table hex_content_info in hex viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }


        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + exported_content_info_db , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString disp_file_path_str   = hex_viewer_open_file_data_obj.display_file_path;
    QString recon_file_from      = hex_viewer_open_file_data_obj.recon_filefrom;
    QString recon_file_info_path = hex_viewer_open_file_data_obj.recon_file_infopath;

    QString complete_file_path = hex_viewer_open_file_data_obj.complete_file_path;
    QString source_count_name = hex_viewer_open_file_data_obj.source_count_name;

    QString filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(disp_file_path_str);
    qint64 file_size = QFile(complete_file_path).size();

    //    QString filename_str;
    //    qint64 file_size = 0;

    //    if(hex_viewer_open_file_data_obj.plugin_name_str == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
    //    {
    //        filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(disp_file_path_str);
    //        struct_GLOBAL_derived_source_info struct_source_info = global_target_sources_info_class_obj->pub_get_derived_info_structure_according_source_count_name(source_count_name);
    //        file_size = struct_source_info.total_size_byte_QString.toLongLong();
    //    }
    //    else
    //    {
    //        filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(complete_file_path);
    //        file_size = QFile(complete_file_path).size();
    //    }



    QFile myfile(exported_file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + myfile.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + myfile.errorString(),Q_FUNC_INFO);
        return;
    }
    QByteArray export_data_arr = myfile.readAll();
    myfile.close();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    if(exported_file_path.startsWith(result_dir_path))
        exported_file_path.remove(0, result_dir_path.size());

    exported_file_path = exported_file_path.trimmed();
    if(!exported_file_path.isEmpty() && !exported_file_path.startsWith("/"))
        exported_file_path.prepend("/");

    QSqlQuery query_insert(destination_db);
    query_insert.prepare("INSERT INTO 'hex_content_info'(bookmark,file_name, file_path, file_size, start_index, end_index, recon_tag_value,"
                         "color_hex, content_file_path, source_count_name, viewer_display_name, hex_data,source_file,recon_filefrom, recon_file_infopath) VALUES(?,?,?,?,? ,?,?,?,?,?, ?,?,?,?,?)");

    query_insert.addBindValue("0");
    query_insert.addBindValue(filename_str);
    query_insert.addBindValue(disp_file_path_str);
    query_insert.addBindValue(file_size);
    query_insert.addBindValue(QString::number(tag.start_address));
    query_insert.addBindValue(QString::number(tag.end_address));
    query_insert.addBindValue(tag.tag_name_str);
    query_insert.addBindValue(tag.color_hex);
    query_insert.addBindValue(exported_file_path);
    query_insert.addBindValue(source_count_name);
    query_insert.addBindValue(hex_viewer_open_file_data_obj.hex_viewer_display_name);
    query_insert.addBindValue(export_data_arr.toHex().toUpper());
    query_insert.addBindValue(disp_file_path_str);
    query_insert.addBindValue(recon_file_from);
    query_insert.addBindValue(recon_file_info_path);

    if(!query_insert.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_insert.executedQuery(),Q_FUNC_INFO);
    }

    destination_db.close();

}

void hex_viewer_complete::fill_the_existing_blocks_details_in_list()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + QString("hex_viewer.sqlite");

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(block_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select_blocks_info(destination_db);

    QString command = QString("SELECT block_name,block_start_offset,block_end_offset,block_color FROM hex_viewer_blocks WHERE file_path = ? AND start_page_no = ?");

    query_select_blocks_info.prepare(command);
    query_select_blocks_info.addBindValue(hex_viewer_open_file_data_obj.display_file_path);
    query_select_blocks_info.addBindValue(QString::number(1));

    if(!query_select_blocks_info.exec())
    {
        recon_static_functions::app_debug("query_select_blocks_info execution-----FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select_blocks_info.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select_blocks_info.executedQuery() ,Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    hex_viewer_tags_list.clear();
    while(query_select_blocks_info.next())
    {
        struct_hex_viewer_tag tag;
        tag.tag_name_str  = query_select_blocks_info.value(0).toString();
        tag.start_address = query_select_blocks_info.value(1).toULongLong();
        tag.end_address   = query_select_blocks_info.value(2).toULongLong();
        tag.color_hex     = query_select_blocks_info.value(3).toString();
        hex_viewer_tags_list.append(tag);
    }
    destination_db.close();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer_complete::slot_action_copy_selected_bytes_triggered(bool status)
{
    Q_UNUSED(status);

    QByteArray selected_data = hex_widget->pub_get_selected_bytes().toHex();
    if(selected_data.isEmpty())
        return;

    for (qint64 idx = 32; idx < selected_data.size(); idx +=33)
        selected_data.insert(idx, "\n");

    QString data_str = QString::fromLocal8Bit(selected_data, selected_data.size());

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(data_str);
}

void hex_viewer_complete::slot_action_copy_selected_ascii_triggered(bool status)
{
    Q_UNUSED(status);

    QByteArray selected_data = hex_widget->pub_get_selected_bytes();
    if(selected_data.isEmpty())
        return;

    for (qint64 idx = 32; idx < selected_data.size(); idx +=33)
        selected_data.insert(idx, "\n");

    QString data_str = QString::fromLocal8Bit(selected_data, selected_data.size());

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(data_str);
}

void hex_viewer_complete::slot_hide_loading_display_dialog_box(bool status)
{
    Q_UNUSED(status);

    bool_cancel_loading = true;
}

void hex_viewer_complete::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;
}
