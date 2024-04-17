#include "regular_filters.h"

void regular_filters::slot_checkbox_create_date_clicked(bool checked)
{
    comboBox_create_days->setEnabled(checked);
    lineEdit_create->setEnabled(checked);
    comboBox_create_within->setEnabled(checked);
    date_edit_create_1->setEnabled(checked);
    date_edit_create_2->setEnabled(checked);
    label_create_between_to->setEnabled(checked);
    label_extra_create_1->setEnabled(checked);
    label_extra_create_2->setEnabled(checked);
    label_extra_create_3->setEnabled(checked);
}

void regular_filters::slot_checkbox_last_modified_date_clicked(bool checked)
{
    comboBox_last_modified_days->setEnabled(checked);
    lineEdit_last_modified->setEnabled(checked);
    comboBox_last_modified_within->setEnabled(checked);
    date_edit_last_modified_1->setEnabled(checked);
    date_edit_last_modified_2->setEnabled(checked);
    label_last_modified_between_to->setEnabled(checked);
    label_extra_modified_1->setEnabled(checked);
    label_extra_modified_2->setEnabled(checked);
    label_extra_modified_3->setEnabled(checked);
}

void regular_filters::slot_checkbox_last_read_date_clicked(bool checked)
{
    comboBox_last_read_days->setEnabled(checked);
    lineEdit_last_read->setEnabled(checked);
    comboBox_last_read_within->setEnabled(checked);
    date_edit_last_read_1->setEnabled(checked);
    date_edit_last_read_2->setEnabled(checked);
    label_last_read_between_to->setEnabled(checked);
    label_extra_read_1->setEnabled(checked);
    label_extra_read_2->setEnabled(checked);
    label_extra_read_3->setEnabled(checked);
}

void regular_filters::slot_comboBox_create_date_within_index_changed(QString index_str)
{
    lineEdit_create->clear();

    if(index_str == MACRO_Generic_Filter_Condition_Between)
    {
        date_edit_create_1->show();
        label_create_between_to->show();
        date_edit_create_2->show();

        comboBox_create_days->hide();
        lineEdit_create->hide();
        label_extra_create_1->hide();
        label_extra_create_2->hide();
        label_extra_create_3->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Within_Last)
    {
        comboBox_create_days->show();
        lineEdit_create->show();

        date_edit_create_1->hide();
        label_create_between_to->hide();
        date_edit_create_2->hide();
        label_extra_create_2->hide();
        label_extra_create_3->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        date_edit_create_1->show();
        label_extra_create_1->show();
        label_extra_create_2->show();

        comboBox_create_days->hide();
        date_edit_create_2->hide();
        label_create_between_to->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Before)
    {
        date_edit_create_1->show();
        label_extra_create_1->show();
        label_extra_create_2->show();

        comboBox_create_days->hide();
        date_edit_create_2->hide();
        label_create_between_to->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_After)
    {
        date_edit_create_1->show();
        label_extra_create_1->show();
        label_extra_create_2->show();

        comboBox_create_days->hide();
        date_edit_create_2->hide();
        label_create_between_to->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Today)
    {
        label_extra_create_1->show();
        label_extra_create_2->show();
        label_extra_create_3->show();

        date_edit_create_1->hide();
        label_create_between_to->hide();
        date_edit_create_2->hide();
        comboBox_create_days->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        label_extra_create_1->show();
        label_extra_create_2->show();
        label_extra_create_3->show();

        date_edit_create_1->hide();
        label_create_between_to->hide();
        date_edit_create_2->hide();
        comboBox_create_days->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        label_extra_create_1->show();
        label_extra_create_2->show();
        label_extra_create_3->show();

        date_edit_create_1->hide();
        label_create_between_to->hide();
        date_edit_create_2->hide();
        comboBox_create_days->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        label_extra_create_1->show();
        label_extra_create_2->show();
        label_extra_create_3->show();

        date_edit_create_1->hide();
        label_create_between_to->hide();
        date_edit_create_2->hide();
        comboBox_create_days->hide();
        lineEdit_create->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        label_extra_create_1->show();
        label_extra_create_2->show();
        label_extra_create_3->show();

        date_edit_create_1->hide();
        label_create_between_to->hide();
        date_edit_create_2->hide();
        comboBox_create_days->hide();
        lineEdit_create->hide();
    }
}

void regular_filters::slot_comboBox_last_read_within_index_changed(QString index_str)
{
    lineEdit_last_read->clear();

    if(index_str == MACRO_Generic_Filter_Condition_Between)
    {
        date_edit_last_read_1->show();
        label_last_read_between_to->show();
        date_edit_last_read_2->show();

        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        label_extra_read_1->hide();
        label_extra_read_2->hide();
        label_extra_read_3->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Within_Last)
    {
        comboBox_last_read_days->show();
        lineEdit_last_read->show();

        date_edit_last_read_1->hide();
        label_last_read_between_to->hide();
        date_edit_last_read_2->hide();
        label_extra_read_2->hide();
        label_extra_read_3->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        date_edit_last_read_1->show();
        label_extra_read_1->show();
        label_extra_read_2->show();

        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
        label_last_read_between_to->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Before)
    {
        date_edit_last_read_1->show();
        label_extra_read_1->show();
        label_extra_read_2->show();

        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
        label_last_read_between_to->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_After)
    {
        date_edit_last_read_1->show();
        label_extra_read_1->show();
        label_extra_read_2->show();

        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
        label_last_read_between_to->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Today)
    {
        label_extra_read_1->show();
        label_extra_read_2->show();
        label_extra_read_3->show();

        date_edit_last_read_1->hide();
        label_last_read_between_to->hide();
        date_edit_last_read_2->hide();
        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        label_extra_read_1->show();
        label_extra_read_2->show();
        label_extra_read_3->show();

        date_edit_last_read_1->hide();
        label_last_read_between_to->hide();
        date_edit_last_read_2->hide();
        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        label_extra_read_1->show();
        label_extra_read_2->show();
        label_extra_read_3->show();

        date_edit_last_read_1->hide();
        label_last_read_between_to->hide();
        date_edit_last_read_2->hide();
        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        label_extra_read_1->show();
        label_extra_read_2->show();
        label_extra_read_3->show();

        date_edit_last_read_1->hide();
        label_last_read_between_to->hide();
        date_edit_last_read_2->hide();
        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        label_extra_read_1->show();
        label_extra_read_2->show();
        label_extra_read_3->show();

        date_edit_last_read_1->hide();
        label_last_read_between_to->hide();
        date_edit_last_read_2->hide();
        comboBox_last_read_days->hide();
        lineEdit_last_read->hide();
        date_edit_last_read_2->hide();
    }
}

void regular_filters::slot_comboBox_last_modified_within_index_chnaged(QString index_str)
{
    lineEdit_last_modified->clear();

    if(index_str == MACRO_Generic_Filter_Condition_Between)
    {
        date_edit_last_modified_1->show();
        label_last_modified_between_to->show();
        date_edit_last_modified_2->show();

        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
        label_extra_modified_1->hide();
        label_extra_modified_2->hide();
        label_extra_modified_3->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Within_Last)
    {
        comboBox_last_modified_days->show();
        lineEdit_last_modified->show();

        date_edit_last_modified_1->hide();
        label_last_modified_between_to->hide();
        date_edit_last_modified_2->hide();
        label_extra_modified_2->hide();
        label_extra_modified_3->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        date_edit_last_modified_1->show();
        label_extra_modified_1->show();
        label_extra_modified_2->show();

        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
        date_edit_last_modified_2->hide();
        label_last_modified_between_to->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Before)
    {
        date_edit_last_modified_1->show();
        label_extra_modified_1->show();
        label_extra_modified_2->show();

        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
        date_edit_last_modified_2->hide();
        label_last_modified_between_to->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_After)
    {
        date_edit_last_modified_1->show();
        label_extra_modified_1->show();
        label_extra_modified_2->show();

        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
        date_edit_last_modified_2->hide();
        label_last_modified_between_to->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Today)
    {
        label_extra_modified_1->show();
        label_extra_modified_2->show();
        label_extra_modified_3->show();

        date_edit_last_modified_1->hide();
        label_last_modified_between_to->hide();
        date_edit_last_modified_2->hide();
        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        label_extra_modified_1->show();
        label_extra_modified_2->show();
        label_extra_modified_3->show();

        date_edit_last_modified_1->hide();
        label_last_modified_between_to->hide();
        date_edit_last_modified_2->hide();
        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        label_extra_modified_1->show();
        label_extra_modified_2->show();
        label_extra_modified_3->show();

        date_edit_last_modified_1->hide();
        label_last_modified_between_to->hide();
        date_edit_last_modified_2->hide();
        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        label_extra_modified_1->show();
        label_extra_modified_2->show();
        label_extra_modified_3->show();

        date_edit_last_modified_1->hide();
        label_last_modified_between_to->hide();
        date_edit_last_modified_2->hide();
        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
    }
    else if(index_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        label_extra_modified_1->show();
        label_extra_modified_2->show();
        label_extra_modified_3->show();

        date_edit_last_modified_1->hide();
        label_last_modified_between_to->hide();
        date_edit_last_modified_2->hide();
        comboBox_last_modified_days->hide();
        lineEdit_last_modified->hide();
    }
}

void regular_filters::slot_combobox_file_size_type_index_changed(QString value)
{
    lineEdit_file_size_1->clear();
    lineEdit_file_size_2->clear();

    if(value == MACRO_Generic_Filter_Condition_Between)
    {
        lineEdit_file_size_1->show();
        label_file_size_between_to->show();
        lineEdit_file_size_2->show();
        combobox_file_size_sufix->show();
    }
    else if(value == MACRO_Generic_Filter_Condition_Is_Equal)
    {
        lineEdit_file_size_1->show();
        combobox_file_size_sufix->show();

        label_file_size_between_to->hide();
        lineEdit_file_size_2->hide();
    }
    else if(value == MACRO_Generic_Filter_Condition_Is_Greater_Than)
    {
        lineEdit_file_size_1->show();
        combobox_file_size_sufix->show();

        label_file_size_between_to->hide();
        lineEdit_file_size_2->hide();
    }
    else if(value == MACRO_Generic_Filter_Condition_Is_Less_Than)
    {
        lineEdit_file_size_1->show();
        combobox_file_size_sufix->show();

        label_file_size_between_to->hide();
        lineEdit_file_size_2->hide();
    }
}

void regular_filters::slot_pushbutton_search_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    QString file_name = lineEdit_file_name->text().trimmed();

    QString file_name_str;
    if(!file_name.isEmpty())
        file_name_str = file_name + QString(MACRO_RECON_Splitter_1_multiple) + combobox_file_name_filters->currentText();

    QString file_size_str = make_search_string_for_file_size_fields();

    QString created_date_str, added_date_str, last_modified_date_str, last_read_date_str;
    if(checkbox_create_date->isChecked())
    {
        created_date_str = make_search_string_for_creat_date_fields();
    }

    if(checkbox_last_modified_date->isChecked())
    {
        last_modified_date_str = make_search_string_for_last_modified_date_fields();
    }

    if(checkbox_last_read_date->isChecked())
    {
        last_read_date_str = make_search_string_for_last_read_date_fields();
    }

    if(file_name.isEmpty() && file_size_str.isEmpty() && created_date_str.isEmpty()
            && added_date_str.isEmpty() && last_modified_date_str.isEmpty() && last_read_date_str.isEmpty())
    {
        message_box_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please enter atleast one filter to search");
        message_box_obj->show();
        this->hide();
        return;
    }

    QString file_search_label_name = lineEdit_file_search->text().trimmed();

    if(file_search_label_name.isEmpty())
        file_search_label_name = lineEdit_file_search->placeholderText().trimmed();

    bool is_radio_or_clicked;
    if(radio_button_logical_or->isChecked())
        is_radio_or_clicked = true;
    else
        is_radio_or_clicked = false;

    emit signal_pushbutton_search_clicked(file_name_str, file_size_str, created_date_str, added_date_str, last_modified_date_str, last_read_date_str, file_search_label_name, is_radio_or_clicked, parent_dir_path);


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

struct_global_selected_regular_filters_info regular_filters::pub_collect_all_selected_fields_info()
{    
    QString file_name = lineEdit_file_name->text().trimmed();

    QString file_name_str;
    if(!file_name.isEmpty())
        file_name_str = file_name + QString(MACRO_RECON_Splitter_1_multiple) + combobox_file_name_filters->currentText();

    QString file_size_str = make_search_string_for_file_size_fields();

    QString created_date_str, added_date_str, last_modified_date_str, last_read_date_str;
    if(checkbox_create_date->isChecked())
    {
        created_date_str = make_search_string_for_creat_date_fields();
    }

    if(checkbox_last_read_date->isChecked())
    {
        last_read_date_str = make_search_string_for_last_read_date_fields();
    }

    if(checkbox_last_modified_date->isChecked())
    {
        last_modified_date_str = make_search_string_for_last_modified_date_fields();
    }


    struct_global_selected_regular_filters_info struct_filters_info;

    if(file_name.isEmpty() && file_size_str.isEmpty() && created_date_str.isEmpty()
            && added_date_str.isEmpty() && last_modified_date_str.isEmpty() && last_read_date_str.isEmpty())
    {
        struct_filters_info.bool_is_structure_is_empty = true;
    }
    else
        struct_filters_info.bool_is_structure_is_empty = false;

    bool is_radio_or_clicked;
    if(radio_button_logical_or->isChecked())
        is_radio_or_clicked = true;
    else
        is_radio_or_clicked = false;

    struct_filters_info.added_date_str = added_date_str;
    struct_filters_info.change_date_str = created_date_str;
    struct_filters_info.file_name_str = file_name_str;
    struct_filters_info.file_size_str = file_size_str;
    struct_filters_info.last_modified_date_str = last_modified_date_str;
    struct_filters_info.last_read_date_str = last_read_date_str;
    struct_filters_info.bool_is_combined_by_or = is_radio_or_clicked;

    return struct_filters_info;
}

void regular_filters::slot_pushbutton_cancel_clicked(bool bool_clicked)
{
    this->hide();
}
