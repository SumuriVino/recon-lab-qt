#include "tt_old_hex_viewer.h"
#include "ui_tt_old_hex_viewer.h"


hex_viewer::struct_search_page_info hex_viewer::get_available_page_info_for_search(enum_search_clicked click_type, QString searched_type,QString searched_text,  uint64_t page_size)
{
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    struct_search_page_info st_obj;
    st_obj.bool_text_match_found = false;
    st_obj.matched_page_count = -1;

    QString label_no_pages_text = label_no_of_pages->text().trimmed();
    qint64 current_page_pos = -1;
    if(label_no_pages_text.contains(" of "))
    {
        QStringList page_count_split_list;
        page_count_split_list = label_no_pages_text.split(" of ");
        QString current_page_str = page_count_split_list.at(0);

        current_page_pos = current_page_str.toInt();
    }
    if(current_page_pos == -1)
        return st_obj;


    QByteArray searched_hex_arr = QByteArray::fromHex(searched_text.toLocal8Bit());

    QFile myfile(file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return st_obj;
    }


    bool bool_match_found = false;
    int page_count = current_page_pos;

    if(click_type == enum_search_pushbutton_search)
    {
        if(page_count <= 0 || page_count > total_no_of_pages)
            return st_obj;

        qint64 file_currnt_pos = (page_count * page_size) - page_size;
        myfile.seek(file_currnt_pos);
    }
    else if(click_type == enum_search_pushbutton_previous)
    {
        page_count = page_count - 1;
        if(page_count <= 0 || page_count > total_no_of_pages)
            return st_obj;

        qint64 file_currnt_pos = (page_count * page_size) - page_size;
        myfile.seek(file_currnt_pos);


    }
    else if(click_type == enum_search_pushbutton_next)
    {
        page_count = page_count + 1;
        if(page_count > total_no_of_pages)
            return st_obj;

        qint64 file_currnt_pos = (current_page_pos * page_size);
        myfile.seek(file_currnt_pos);

    }

    display_loading_progress_bar_obj->show();


    qint64 mm_count = 0;
    while(1)
    {
        mm_count++;
        if(mm_count % 30 == 0)
        {
            display_loading_progress_bar_obj->pub_set_label_messsge("Searching in Page " + QString::number(page_count));
            QCoreApplication::processEvents();
        }

        if(bool_cancel_loading)
            break;

        if(myfile.atEnd() || page_count <= 0 || page_count > total_no_of_pages)
        {
            break;
        }


        QByteArray single_page_data = myfile.read(page_size);
        if(single_page_data.isEmpty())
        {
            break;
        }

        if(searched_type == "ASCII")
        {
            QString ascii;
            for(int pp = 0; pp < single_page_data.size(); pp++)
            {
                if(!(((char)single_page_data[pp]<0x20) || ((char)single_page_data[pp]>0x7e)))
                {
                    ascii.append(single_page_data.at(pp));
                }
                else
                {
                    ascii.append(".");
                }
            }

            if(ascii.toLower().contains(searched_text.toLower()))
            {
                bool_match_found = true;
                st_obj.bool_text_match_found = true;
                st_obj.matched_page_count = page_count;

            }

        }
        else if(searched_type == "HEX")
        {
            if(single_page_data.toLower().contains(searched_hex_arr.toLower()))
            {
                bool_match_found = true;
                st_obj.bool_text_match_found = true;
                st_obj.matched_page_count = page_count;

            }
        }

        if(bool_match_found)
        {
            break;
        }
        else
        {
            if(click_type == enum_search_pushbutton_previous)
            {
                page_count--;
                if(page_count <= 0 || page_count > total_no_of_pages)
                    break;

                qint64 file_currnt_pos1 = (page_count * page_size) - page_size;
                myfile.seek(file_currnt_pos1);

            }
            else
                page_count++;
        }
    }

    myfile.close();

    display_loading_progress_bar_obj->hide();
    return st_obj;

}
void hex_viewer::on_pushButton_search_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString searched_type = comboBox_search_type->itemText(comboBox_search_type->currentIndex());
    QString searched_text_trimmed 	= lineEdit_search->text().trimmed();

    if(searched_type == "HEX")
    {
        if(searched_text_trimmed.trimmed().isEmpty())
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid Hex to search");
            message_dialog_obj->show();
            return;
        }


        if(searched_text_trimmed.contains(" "))
            searched_text_trimmed.replace(" ","");

        searched_text_trimmed = searched_text_trimmed.toLower();


        if(searched_text_trimmed.size() % 2 != 0)
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid Hex to search");
            message_dialog_obj->show();
            return;
        }

        for(int i = 0; i < searched_text_trimmed.size(); i++)
        {
            QChar ch = searched_text_trimmed.at(i);

            int ascii = ch.toLatin1();

            if(((ascii < 48) || (ascii > 57)) && ((ascii < 97) || (ascii > 102)))
            {
                if(bool_show_informative_dialogs)
                {
                    message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid Hex to search");
                    message_dialog_obj->show();
                    return;
                }

            }
        }
    }
    else if(searched_type == "ASCII")
    {

        if(searched_text_trimmed.isEmpty())
        {
            if(bool_show_informative_dialogs)
            {
                message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid ASCII to search");
                message_dialog_obj->show();
                return;
            }

        }
    }


    QString searched_text 	= lineEdit_search->text(); // dont use trim

    if(bool_is_from_disk_viewer)
    {
        search_for_disk_hex_view_complete(searched_type, searched_text);
    }
    else
    {
        search_for_file(searched_type,searched_text);
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::search_for_file(QString search_type, QString searched_text)
{
    bool_cancel_loading = false;

    index_list_size = 0;

    uint64_t page_size = spinBox_page_size->text().toULongLong();

    pushButton_search_previous->setEnabled(true);
    pushButton_search_next->setEnabled(true);


    struct_search_page_info st_obj = get_available_page_info_for_search(enum_search_pushbutton_search, search_type, searched_text, page_size);
    if(st_obj.bool_text_match_found)
    {
        common_go_to_page(st_obj.matched_page_count);

        if(search_type == "ASCII")
            emit search_ascii(searched_text, page_size,false, true);
        else if(search_type == "HEX")
            emit search_hex(searched_text.toLocal8Bit(), page_size,false, true);
    }

}

void hex_viewer::on_pushButton_find_next_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    uint64_t page_size = spinBox_page_size->text().toULongLong();

    QString searched_type = comboBox_search_type->itemText(comboBox_search_type->currentIndex());

    if(!bool_at_page_end_for_search_highlight && index_list_size > 0)
    {
        QString searched_text1 = lineEdit_search->text();
        if(searched_type == "HEX")
        {
            emit search_hex(searched_text1.toLocal8Bit(), page_size,false,false);
        }
        else
        {
            emit search_ascii(searched_text1, page_size, false, false);
        }
    }
    else
    {
        QString searched_text 	= lineEdit_search->text(); // dont use trim

        struct_search_page_info st_obj;
        if(bool_is_from_disk_viewer)
        {
            if(bool_disk_viewer_unallocated_disk)
                st_obj = get_available_page_info_for_diskview_unallocated_search(enum_search_pushbutton_next,searched_type, searched_text);
            else
                st_obj = get_available_page_info_for_diskview_complete_search(enum_search_pushbutton_next,searched_type, searched_text);
        }
        else
            st_obj = get_available_page_info_for_search(enum_search_pushbutton_next, searched_type, searched_text, page_size);

        if(st_obj.bool_text_match_found)
        {
            if(bool_is_from_disk_viewer)
                common_go_to_page_for_disk_hex_viewer(st_obj.matched_page_count);
            else
                common_go_to_page(st_obj.matched_page_count);

            if(searched_type == "ASCII")
                emit search_ascii(searched_text, page_size,false, false);
            else if(searched_type == "HEX")
                emit search_hex(searched_text.toLocal8Bit(), page_size,false, false);
        }

    }

    pushButton_search_previous->setEnabled(true);
    pushButton_search_next->setEnabled(true);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::on_pushButton_find_previous_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    pushButton_search_next->setEnabled(true);

    current_cursor_pos_for_prev_clicked -= 1;


    uint64_t page_size = spinBox_page_size->text().toULongLong();
    QString searched_type = comboBox_search_type->itemText(comboBox_search_type->currentIndex());

    if(current_cursor_pos_for_prev_clicked < 0)
    {
        QString searched_text 	= lineEdit_search->text(); // dont use trim

        struct_search_page_info st_obj;
        if(bool_is_from_disk_viewer)
        {
            if(bool_disk_viewer_unallocated_disk)
                st_obj = get_available_page_info_for_diskview_unallocated_search(enum_search_pushbutton_previous,searched_type, searched_text);
            else
                st_obj = get_available_page_info_for_diskview_complete_search(enum_search_pushbutton_previous,searched_type, searched_text);
        }
        else
            st_obj = get_available_page_info_for_search(enum_search_pushbutton_previous, searched_type, searched_text, page_size);


        if(st_obj.bool_text_match_found)
        {
            if(bool_is_from_disk_viewer)
                common_go_to_page_for_disk_hex_viewer(st_obj.matched_page_count);
            else
                common_go_to_page(st_obj.matched_page_count);

            if(searched_type == "ASCII")
                emit search_ascii(searched_text, page_size,true, false);
            else if(searched_type == "HEX")
                emit search_hex(searched_text.toLocal8Bit(), page_size,true, false);
        }

    }
    else
    {
        int searched_data_size = 0;
        if(comboBox_search_type->itemText(comboBox_search_type->currentIndex()) == "HEX")
            searched_data_size = lineEdit_search->text().size()/2;
        else if(comboBox_search_type->itemText(comboBox_search_type->currentIndex()) == "ASCII")
            searched_data_size = lineEdit_search->text().size();

        if(current_cursor_pos_for_prev_clicked >= 0)
            emit search_previous(current_cursor_pos_for_prev_clicked, searched_data_size);
    }

    pushButton_search_previous->setEnabled(true);
    pushButton_search_next->setEnabled(true);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked();
}

void hex_viewer::on_lineEdit_search_text_change(QString text)
{
    label_search_results->clear();
    text = text.trimmed();

    if(text.isEmpty())
    {
        pushButton_search->setEnabled(false);
        pushButton_search_previous->setEnabled(false);
        pushButton_search_next->setEnabled(false);
    }
    else
    {
        pushButton_search->setEnabled(true);
    }
}

void hex_viewer::slot_index_list_size(int indexsize)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    index_list_size = indexsize;
    label_search_results->setText(QString::number(indexsize) + " Match(s)");

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_page_end_reached_for_search_highlight(bool status)
{
    bool_at_page_end_for_search_highlight = status;
}

void hex_viewer::slot_cursor_current_pos_for_previous_click(qint64 pos)
{
    current_cursor_pos_for_prev_clicked = pos;
}
