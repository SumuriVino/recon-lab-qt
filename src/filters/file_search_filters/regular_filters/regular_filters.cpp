#include "regular_filters.h"
#include "ui_regular_filters.h"

regular_filters::regular_filters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regular_filters)
{
    ui->setupUi(this);

    message_box_obj = new message_dialog(this);
}

regular_filters::~regular_filters()
{
    delete ui;
}

void regular_filters::all_ui_connections()
{
    connect(checkbox_create_date,SIGNAL(clicked(bool)),this,SLOT(slot_checkbox_create_date_clicked(bool)));
    connect(checkbox_last_modified_date,SIGNAL(clicked(bool)),this,SLOT(slot_checkbox_last_modified_date_clicked(bool)));
    connect(checkbox_last_read_date,SIGNAL(clicked(bool)),this,SLOT(slot_checkbox_last_read_date_clicked(bool)));
    connect(pushbutton_search,SIGNAL(clicked()),this, SLOT(slot_pushbutton_search_clicked()));
    connect(pushbutton_cancel, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_cancel_clicked(bool)));

    connect(comboBox_create_within,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_comboBox_create_date_within_index_changed(QString)));
    connect(comboBox_last_read_within,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_comboBox_last_read_within_index_changed(QString)));
    connect(comboBox_last_modified_within, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_comboBox_last_modified_within_index_chnaged(QString)));
    connect(combobox_file_size_type, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_combobox_file_size_type_index_changed(QString)));
}


void regular_filters::set_parent_dir_path_for_extraction(QString path)
{
    parent_dir_path = path;
}

QString regular_filters::make_search_string_for_file_size_fields()
{
    QString current_str = combobox_file_size_type->currentText();

    QString file_size_str;

    if(current_str == MACRO_Generic_Filter_Condition_Between)
    {
        QString size_1 = lineEdit_file_size_1->text().trimmed();
        QString size_2 = lineEdit_file_size_2->text().trimmed();
        QString suffix = combobox_file_size_sufix->currentText();

        if(!size_1.isEmpty() && !size_2.isEmpty() && !suffix.isEmpty())
            file_size_str = current_str + MACRO_RECON_Splitter_1_multiple + size_1 + MACRO_RECON_Splitter_2_hash_in_brace + size_2 + MACRO_RECON_Splitter_3_dollar_in_brace + suffix;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Is_Equal)
    {
        QString size_1 = lineEdit_file_size_1->text().trimmed();
        QString suffix = combobox_file_size_sufix->currentText();

        if(!size_1.isEmpty() && !suffix.isEmpty())
            file_size_str = current_str + MACRO_RECON_Splitter_1_multiple + size_1 + MACRO_RECON_Splitter_2_hash_in_brace + suffix;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Is_Greater_Than)
    {
        QString size_1 = lineEdit_file_size_1->text().trimmed();
        QString suffix = combobox_file_size_sufix->currentText();

        if(!size_1.isEmpty() && !suffix.isEmpty())
            file_size_str = current_str + MACRO_RECON_Splitter_1_multiple + size_1 + MACRO_RECON_Splitter_2_hash_in_brace + suffix;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Is_Less_Than)
    {
        QString size_1 = lineEdit_file_size_1->text().trimmed();
        QString suffix = combobox_file_size_sufix->currentText();

        if(!size_1.isEmpty() && !suffix.isEmpty())
            file_size_str = current_str + MACRO_RECON_Splitter_1_multiple + size_1 + MACRO_RECON_Splitter_2_hash_in_brace + suffix;
    }

    return file_size_str;
}

QString regular_filters::make_search_string_for_creat_date_fields()
{
    QString current_str = comboBox_create_within->currentText();

    QString created_date_str;
    if(current_str == MACRO_Generic_Filter_Condition_Between)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        QString date_edit_val_1 = date_edit_create_1->text();
        QString date_edit_val_2 = date_edit_create_2->text();

        if(!date_edit_val_1.isEmpty() && !date_edit_val_2.isEmpty())
            created_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val_1 + MACRO_RECON_Splitter_2_hash_in_brace + date_edit_val_2;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Within_Last)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        QString lineedit = lineEdit_create->text();
        QString combox_days_val = comboBox_create_days->currentText();

        if(!lineedit.isEmpty())
            created_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + lineedit + MACRO_RECON_Splitter_2_hash_in_brace + combox_days_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        QString date_edit_val = date_edit_create_1->text();

        if(!date_edit_val.isEmpty())
            created_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Before)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        QString date_edit_val = date_edit_create_1->text();

        if(!date_edit_val.isEmpty())
            created_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_After)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        QString date_edit_val = date_edit_create_1->text();

        if(!date_edit_val.isEmpty())
            created_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Today)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        created_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        created_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        created_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        created_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        QString combox_within_val = comboBox_create_within->currentText();
        created_date_str = combox_within_val;
    }

    return created_date_str;
}

QString regular_filters::make_search_string_for_last_modified_date_fields()
{
    QString current_str = comboBox_last_modified_within->currentText();

    QString last_modified_date_str;
    if(current_str == MACRO_Generic_Filter_Condition_Between)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        QString date_edit_val_1 = date_edit_last_modified_1->text();
        QString date_edit_val_2 = date_edit_last_modified_2->text();

        if(!date_edit_val_1.isEmpty() && !date_edit_val_2.isEmpty())
            last_modified_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val_1 + MACRO_RECON_Splitter_2_hash_in_brace + date_edit_val_2;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Within_Last)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        QString lineedit = lineEdit_last_modified->text();
        QString combox_days_val = comboBox_last_modified_days->currentText();

        if(!lineedit.isEmpty())
            last_modified_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + lineedit + MACRO_RECON_Splitter_2_hash_in_brace + combox_days_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        QString date_edit_val = date_edit_last_modified_1->text();

        if(!date_edit_val.isEmpty())
            last_modified_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Before)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        QString date_edit_val = date_edit_last_modified_1->text();

        if(!date_edit_val.isEmpty())
            last_modified_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_After)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        QString date_edit_val = date_edit_last_modified_1->text();

        if(!date_edit_val.isEmpty())
            last_modified_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Today)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        last_modified_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        last_modified_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        last_modified_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        last_modified_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        QString combox_within_val = comboBox_last_modified_within->currentText();
        last_modified_date_str = combox_within_val;
    }

    return last_modified_date_str;
}

QString regular_filters::make_search_string_for_last_read_date_fields()
{
    QString current_str = comboBox_last_read_within->currentText();

    QString last_read_date_str;
    if(current_str == MACRO_Generic_Filter_Condition_Between)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        QString date_edit_val_1 = date_edit_last_read_1->text();
        QString date_edit_val_2 = date_edit_last_read_2->text();

        if(!date_edit_val_1.isEmpty() && !date_edit_val_2.isEmpty())
            last_read_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val_1 + MACRO_RECON_Splitter_2_hash_in_brace + date_edit_val_2;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Within_Last)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        QString lineedit = lineEdit_last_read->text();
        QString combox_days_val = comboBox_last_read_days->currentText();

        if(!lineedit.isEmpty())
            last_read_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + lineedit + MACRO_RECON_Splitter_2_hash_in_brace + combox_days_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        QString date_edit_val = date_edit_last_read_1->text();

        if(!date_edit_val.isEmpty())
            last_read_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Before)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        QString date_edit_val = date_edit_last_read_1->text();

        if(!date_edit_val.isEmpty())
            last_read_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_After)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        QString date_edit_val = date_edit_last_read_1->text();

        if(!date_edit_val.isEmpty())
            last_read_date_str = combox_within_val + MACRO_RECON_Splitter_1_multiple + date_edit_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Today)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        last_read_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        last_read_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        last_read_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        last_read_date_str = combox_within_val;
    }
    else if(current_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        QString combox_within_val = comboBox_last_read_within->currentText();
        last_read_date_str = combox_within_val;
    }

    return last_read_date_str;
}
