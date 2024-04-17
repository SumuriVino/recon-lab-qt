#include "file_search_regular_filters.h"
#include "ui_file_search_regular_filters.h"

file_search_regular_filters::file_search_regular_filters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_search_regular_filters)
{
    ui->setupUi(this);

    ui->radioButton_all_filters->setChecked(true);
    ui->lineEdit_file_name->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_file_size_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_file_size_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_content_creation_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_content_creation_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_content_modification_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_content_modification_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_accessed_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_accessed_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_added_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_added_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_change_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_change_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_modified_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_modified_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_last_used_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_last_used_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_created_1->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_date_created_2->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_search_label->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_ocr_text->setAttribute(Qt::WA_MacShowFocusRect, false);


    ui->checkBox_content_creation->setIcon(QIcon("../icons/Features/apple_metadata.png"));
    ui->checkBox_content_modification->setIcon(QIcon("../icons/Features/apple_metadata.png"));
    ui->checkBox_date_added->setIcon(QIcon("../icons/Features/apple_metadata.png"));
    ui->checkBox_last_used->setIcon(QIcon("../icons/Features/apple_metadata.png"));
    ui->pushButton_select_source_list->setIcon(QIcon("../icons/signs/check_list.png"));

    pub_load_defaults();

    select_source_interface_obj = new select_source_interface(this);
    connect(select_source_interface_obj,SIGNAL(signal_selected_source_list(QStringList)),this,SLOT(slot_selected_source_list(QStringList)));

}

file_search_regular_filters::~file_search_regular_filters()
{
    delete ui;
}

void file_search_regular_filters::pub_load_defaults()
{

    ui->lineEdit_file_name->clear();
    ui->lineEdit_file_size_1->clear();
    ui->lineEdit_file_size_2->clear();
    ui->lineEdit_search_label->clear();

    ui->lineEdit_ocr_text->clear();
    ui->lineEdit_file_name->clear();
    ui->lineEdit_file_size_1->setDisabled(true);
    ui->lineEdit_file_size_2->setDisabled(true);
    ui->lineEdit_file_name->setDisabled(true);
    ui->comboBox_file_size_options->setDisabled(true);
    ui->comboBox_file_size_units->setDisabled(true);
    ui->comboBox_file_name_options->setDisabled(true);
    ui->pushButton_file_name_select_category->setDisabled(true);
    ui->checkBox_content_creation->setChecked(false);
    ui->checkBox_content_modification->setChecked(false);
    ui->checkBox_date_accessed->setChecked(false);
    ui->checkBox_date_added->setChecked(false);
    ui->checkBox_date_change->setChecked(false);
    ui->checkBox_date_modified->setChecked(false);
    ui->checkBox_date_created->setChecked(false);
    ui->checkBox_file_size->setChecked(false);
    ui->checkBox_last_used->setChecked(false);
    ui->checkBox_ocr_text->setChecked(false);
    ui->checkBox_file_name->setChecked(false);

    ui->comboBox_content_creation_options->setCurrentIndex(0);
    ui->comboBox_content_modification_options->setCurrentIndex(0);
    ui->comboBox_date_accessed_options->setCurrentIndex(0);
    ui->comboBox_date_added_options->setCurrentIndex(0);
    ui->comboBox_date_change_options->setCurrentIndex(0);
    ui->comboBox_date_modified_options->setCurrentIndex(0);
    ui->comboBox_date_created_options->setCurrentIndex(0);
    ui->comboBox_file_name_options->setCurrentIndex(0);
    ui->comboBox_file_size_options->setCurrentIndex(0);
    ui->comboBox_file_size_units->setCurrentIndex(0);
    ui->comboBox_last_used_options->setCurrentIndex(0);

    ui->radioButton_all_filters->setChecked(true);
    ui->radioButton_any_filter->setChecked(false);

    ui->radioButton_all_source->setChecked(true);
    ui->pushButton_select_source_list->setDisabled(true);

    on_checkBox_content_creation_clicked(false);
    on_checkBox_content_modification_clicked(false);
    on_checkBox_date_accessed_clicked(false);
    on_checkBox_date_added_clicked(false);
    on_checkBox_date_change_clicked(false);
    on_checkBox_date_modified_clicked(false);
    on_checkBox_date_created_clicked(false);
    on_checkBox_last_used_clicked(false);
    on_checkBox_ocr_text_clicked(false);
    on_checkBox_file_name_clicked(false);

    add_category_in_pushbutton_file_name_select_category();
}


void file_search_regular_filters::pub_set_sources_in_ui()
{
    QList<struct_GLOBAL_witness_info_source> sources_list =  global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    int row = 0;
    int col;

    source_checkbox_list.clear();
    for(int i = 0; i < sources_list.size() ; i++)
    {

        if(i % 4 == 0)
        {
            row++;
            col = 0;
        }

        QCheckBox *checkBox = new QCheckBox(this);

        checkBox->setText(sources_list.at(i).source_count_name);
        checkBox->setChecked(true);
        checkBox->setToolTip(sources_list.at(i).complete_source_name);
        //ui->gridLayout_sources->addWidget(checkBox, row, col++);

        source_checkbox_list.append(checkBox);

    }

}

void file_search_regular_filters::on_pushButton_search_clicked()
{
    const QDateTime dateTime1 = QDateTime::currentDateTime();
    const QDateTime dateTime2 = QDateTime(dateTime1.date(), dateTime1.time(), Qt::UTC);
    current_machine_offset =   dateTime1.secsTo(dateTime2);
    current_case_offset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toInt();

    round_off_offset = current_case_offset;

    QString command;

    command = "select INT from files where ";

    QString where_combiner;

    if(ui->radioButton_all_filters->isChecked())
        where_combiner = " AND ";
    else
        where_combiner = " OR ";

    QStringList where_items;

    if(ui->checkBox_file_name->isChecked() && !ui->lineEdit_file_name->text().trimmed().isEmpty())
        where_items.append(file_name_sql_predicate());

    if(ui->checkBox_file_size->isChecked() && !ui->lineEdit_file_size_1->text().trimmed().isEmpty())
        where_items.append(file_size_sql_predicate());

    if(ui->checkBox_ocr_text->isChecked() && !ui->lineEdit_ocr_text->text().trimmed().isEmpty())
        where_items.append(ocr_text_sql_predicate());

    if(ui->checkBox_date_change->isChecked())
        where_items.append(date_change_sql_predicate());


    if(ui->checkBox_date_modified->isChecked())
        where_items.append(date_modified_sql_predicate());


    if(ui->checkBox_date_accessed->isChecked())
        where_items.append(date_accessed_sql_predicate());

    if(ui->checkBox_content_creation->isChecked())
        where_items.append(date_content_creation_sql_predicate());

    if(ui->checkBox_content_modification->isChecked())
        where_items.append(date_content_modification_sql_predicate());

    if(ui->checkBox_date_created->isChecked())
        where_items.append(date_created_sql_predicate());

    if(ui->checkBox_date_added->isChecked())
        where_items.append(date_added_sql_predicate());

    if(ui->checkBox_last_used->isChecked())
        where_items.append(date_last_used_sql_predicate());

    if(where_items.size() < 1)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select atleast one filter");
        return;
    }

    //    QStringList source_list_for_searching;
    //    for(int i = 0 ; i < source_checkbox_list.size(); i++)
    //    {
    //        QCheckBox *checkBox = source_checkbox_list.at(i);

    //        if(checkBox->isChecked())
    //            source_list_for_searching.append(checkBox->text());
    //    }


    if(ui->radioButton_all_source->isChecked())
    {
        recon_static_functions::app_debug(" -START",Q_FUNC_INFO);
        ui->pushButton_select_source_list->setEnabled(false);
        QList<struct_GLOBAL_witness_info_source> list_target_source_info =  global_witness_info_manager_class_obj->pub_get_source_structure_QList();
        selected_source_cnt_nm_list.clear();
        for(int ii = 0; ii < list_target_source_info.size() ; ii++)
        {
            selected_source_cnt_nm_list << list_target_source_info.at(ii).source_count_name;
        }
    }
    else
    {
        // selected_source_cnt_nm_list is filled in selected source slot
    }

    QStringList source_list_for_searching  = selected_source_cnt_nm_list;

    if(source_list_for_searching.size() < 1)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select atleast one source");
        return;
    }


    if(ui->lineEdit_search_label->text().trimmed() == QString(""))
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please enter search label");
        return;
    }

    QString where_statement = where_items.join(where_combiner);

    command.append(where_statement);


    emit signal_file_search_regular_filter_work_done(command, source_list_for_searching, ui->lineEdit_search_label->text().trimmed());

}

QString file_search_regular_filters::file_name_sql_predicate()
{
    if(ui->lineEdit_file_name->text().trimmed() == QString(""))
        return "";

    QStringList sql_predicate_list;

    QString all_file_name = ui->lineEdit_file_name->text().trimmed();
    QStringList file_name_list =  all_file_name.split(",", Qt::SkipEmptyParts);
    file_name_list.removeAll(" ");

    if(ui->comboBox_file_name_options->currentText() == "Contains")
    {
        for(int ii = 0 ; ii < file_name_list.size() ; ii++)
        {
            QString file_name = file_name_list.at(ii);
            sql_predicate_list.append("filename LIKE '%" + file_name.trimmed() + "%'");
        }
    }
    else if(ui->comboBox_file_name_options->currentText() == "Matches")
    {
        for(int ii = 0 ; ii < file_name_list.size() ; ii++)
        {
            QString file_name = file_name_list.at(ii);
            sql_predicate_list.append("filename = '" + file_name.trimmed()  + "'");
        }
    }
    else if(ui->comboBox_file_name_options->currentText() == "Starts with")
    {
        for(int ii = 0 ; ii < file_name_list.size() ; ii++)
        {
            QString file_name = file_name_list.at(ii);
            sql_predicate_list.append("filename LIKE '" + file_name.trimmed()  + "%'");
        }
    }
    else if(ui->comboBox_file_name_options->currentText() == "Ends with")
    {
        for(int ii = 0 ; ii < file_name_list.size() ; ii++)
        {
            QString file_name = file_name_list.at(ii);
            sql_predicate_list.append("filename LIKE '%" + file_name.trimmed()  + "'");
        }
    }

    QString partial_cmd = sql_predicate_list.join(" OR ");
    partial_cmd.prepend("( ").append(" )");


    return partial_cmd;


    //    if(ui->lineEdit_file_name->text().trimmed() == QString(""))
    //        return "";

    //    QString sql_predicate;

    //    if(ui->comboBox_file_name_options->currentText() == "Contains")
    //    {
    //        sql_predicate = "filename LIKE '%" + ui->lineEdit_file_name->text().trimmed() + "%'";
    //    }
    //    else if(ui->comboBox_file_name_options->currentText() == "Matches")
    //    {
    //        sql_predicate = "filename = '" + ui->lineEdit_file_name->text().trimmed()  + "'";
    //    }
    //    else if(ui->comboBox_file_name_options->currentText() == "Starts with")
    //    {
    //        sql_predicate = "filename LIKE '" + ui->lineEdit_file_name->text().trimmed()  + "%'";

    //    }
    //    else if(ui->comboBox_file_name_options->currentText() == "Ends with")
    //    {
    //        sql_predicate = "filename LIKE '%" + ui->lineEdit_file_name->text().trimmed()  + "'";
    //    }

    //    return sql_predicate;

}

QString file_search_regular_filters::file_size_sql_predicate()
{

    QString sql_predicate;

    QString file_size = ui->lineEdit_file_size_1->text();
    QString file_size_2 = ui->lineEdit_file_size_2->text();

    if(ui->comboBox_file_size_units->currentText() == "KB")
    {
        file_size = QString::number(file_size.toLong() * 1024);
        file_size_2 = QString::number(file_size_2.toLong() * 1024);
    }
    else if(ui->comboBox_file_size_units->currentText() == "MB")
    {
        file_size = QString::number(file_size.toLong() * 1024 * 1024);
        file_size_2 = QString::number(file_size_2.toLong() * 1024 * 1024);

    }
    else if(ui->comboBox_file_size_units->currentText() == "GB")
    {
        file_size = QString::number(file_size.toLong() * 1024 * 1024 * 1024);
        file_size_2 = QString::number(file_size_2.toLong() * 1024 * 1024 * 1024);
    }


    QString option = ui->comboBox_file_size_options->currentText();

    if(option == "Greater than")
    {
        sql_predicate = QString("(file_size > ") + file_size + ")";
    }
    else if(option == "Less than")
    {
        sql_predicate = QString("(file_size < ") + file_size + ")";
    }
    else if(option == "Exact")
    {
        sql_predicate = QString("(file_size = ") + file_size + ")";
    }
    else if(option == "Between")
    {
        sql_predicate = QString("(file_size >= ") + file_size + " AND file_size <= " + file_size_2 + ")";
    }

    return sql_predicate;

}

QString file_search_regular_filters::ocr_text_sql_predicate()
{
    if(ui->lineEdit_ocr_text->text().trimmed().isEmpty())
        return "";

    QStringList sql_predicate_list;

    QString ocr_text_str = ui->lineEdit_ocr_text->text().trimmed();
    QStringList ocr_text_list =  ocr_text_str.split(",", Qt::SkipEmptyParts);
    ocr_text_list.removeAll(" ");

    for(int ii = 0 ; ii < ocr_text_list.size() ; ii++)
    {
        QString m_ocr_text = ocr_text_list.at(ii);
        sql_predicate_list.append("ocr_text LIKE '%" + m_ocr_text.trimmed() + "%'");
    }

    QString partial_cmd = sql_predicate_list.join(" OR ");
    partial_cmd.prepend("( ").append(" )");

    return partial_cmd;

}

QString file_search_regular_filters::date_change_sql_predicate()
{

    QString sql_predicate;

    qint64 epch_1, epch_2;

    QString option = ui->comboBox_date_change_options->currentText();

    epch_1 = ui->dateTimeEdit_date_change_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_date_change_2->dateTime().toSecsSinceEpoch();

    if(option == "Between")
    {
        sql_predicate = "(z_date_time_change >= " + QString::number(epch_1 - round_off_offset) + " AND z_date_time_change <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(z_date_time_change is not null) AND (z_date_time_change != '') AND (z_date_time_change <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {
        sql_predicate = "(z_date_time_change is not null) AND (z_date_time_change != '') AND (z_date_time_change >= " + QString::number(epch_1 - round_off_offset) + ")";
    }


    return sql_predicate;
}

QString file_search_regular_filters::date_modified_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    QString option = ui->comboBox_date_modified_options->currentText();

    epch_1 = ui->dateTimeEdit_date_modified_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_date_modified_2->dateTime().toSecsSinceEpoch();

    if(option == "Between")
    {
        sql_predicate = "(z_date_time_modified >= " + QString::number(epch_1 - round_off_offset) + " AND z_date_time_modified <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(z_date_time_modified is not null) AND (z_date_time_modified != '') AND (z_date_time_modified <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {
        sql_predicate = "(z_date_time_modified is not null) AND (z_date_time_modified != '') AND (z_date_time_modified >= " + QString::number(epch_1 - round_off_offset) + ")";
    }

    return sql_predicate;

}

QString file_search_regular_filters::date_accessed_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    epch_1 = ui->dateTimeEdit_date_accessed_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_date_accessed_2->dateTime().toSecsSinceEpoch();

    QString option = ui->comboBox_date_accessed_options->currentText();

    if(option == "Between")
    {
        sql_predicate = "(z_date_time_accessed >= " + QString::number(epch_1 - round_off_offset) + " AND z_date_time_accessed <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(z_date_time_accessed is not null) AND (z_date_time_accessed != '') AND (z_date_time_accessed <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {
        sql_predicate = "(z_date_time_accessed is not null) AND (z_date_time_accessed != '') AND (z_date_time_accessed >= " + QString::number(epch_1 - round_off_offset) + ")";
    }

    return sql_predicate;

}

QString file_search_regular_filters::date_created_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    epch_1 = ui->dateTimeEdit_date_created_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_date_created_2->dateTime().toSecsSinceEpoch();

    QString option = ui->comboBox_date_created_options->currentText();

    if(option == "Between")
    {
        sql_predicate = "(z_date_time_created >= " + QString::number(epch_1 - round_off_offset) + " AND z_date_time_created <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(z_date_time_created is not null) AND (z_date_time_created != '') AND (z_date_time_created <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {
        sql_predicate = "(z_date_time_created is not null) AND (z_date_time_created != '') AND (z_date_time_created >= " + QString::number(epch_1 - round_off_offset) + ")";
    }

    return sql_predicate;
}

QString file_search_regular_filters::date_content_creation_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    QString option = ui->comboBox_content_creation_options->currentText();

    epch_1 = ui->dateTimeEdit_content_creation_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_content_creation_2->dateTime().toSecsSinceEpoch();

    if(option == "Between")
    {
        sql_predicate = "(kMDItemContentCreationDate_Created_Date >= " + QString::number(epch_1 - round_off_offset) + " AND kMDItemContentCreationDate_Created_Date <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(kMDItemContentCreationDate_Created_Date is not null) AND (kMDItemContentCreationDate_Created_Date != '') AND (kMDItemContentCreationDate_Created_Date <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {

        sql_predicate = "(kMDItemContentCreationDate_Created_Date is not null) AND (kMDItemContentCreationDate_Created_Date != '') AND (kMDItemContentCreationDate_Created_Date >= " + QString::number(epch_1 - round_off_offset) + ")";
    }


    return sql_predicate;

}

QString file_search_regular_filters::date_content_modification_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    epch_1 = ui->dateTimeEdit_content_modification_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_content_modification_2->dateTime().toSecsSinceEpoch();

    QString option = ui->comboBox_content_modification_options->currentText();

    if(option == "Between")
    {
        sql_predicate = "(kMDItemContentModificationDate_Last_Modified_Date >= " + QString::number(epch_1 - round_off_offset) + " AND kMDItemContentModificationDate_Last_Modified_Date <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(kMDItemContentModificationDate_Last_Modified_Date is not null) AND (kMDItemContentModificationDate_Last_Modified_Date != '') AND (kMDItemContentModificationDate_Last_Modified_Date <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {
        sql_predicate = "(kMDItemContentModificationDate_Last_Modified_Date is not null) AND (kMDItemContentModificationDate_Last_Modified_Date != '') AND (kMDItemContentModificationDate_Last_Modified_Date >= " + QString::number(epch_1 - round_off_offset) + ")";
    }


    return sql_predicate;

}

QString file_search_regular_filters::date_added_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    epch_1 = ui->dateTimeEdit_date_added_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_date_added_2->dateTime().toSecsSinceEpoch();

    QString option = ui->comboBox_date_added_options->currentText();

    if(option == "Between")
    {
        sql_predicate = "(kMDItemDateAdded_Date_Added >= " + QString::number(epch_1 - round_off_offset) + " AND kMDItemDateAdded_Date_Added <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(kMDItemDateAdded_Date_Added is not null) AND (kMDItemDateAdded_Date_Added != '') AND (kMDItemDateAdded_Date_Added <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {
        sql_predicate = "(kMDItemDateAdded_Date_Added is not null) AND (kMDItemDateAdded_Date_Added != '') AND (kMDItemDateAdded_Date_Added >= " + QString::number(epch_1 - round_off_offset) + ")";
    }


    return sql_predicate;

}

QString file_search_regular_filters::date_last_used_sql_predicate()
{
    QString sql_predicate;

    qint64 epch_1, epch_2;

    epch_1 = ui->dateTimeEdit_last_used_1->dateTime().toSecsSinceEpoch();
    epch_2 = ui->dateTimeEdit_last_used_2->dateTime().toSecsSinceEpoch();

    QString option = ui->comboBox_last_used_options->currentText();

    if(option == "Between")
    {
        sql_predicate = "(kMDItemLastUsedDate_Last_Opened_Date >= " + QString::number(epch_1 - round_off_offset) + " AND kMDItemLastUsedDate_Last_Opened_Date <= " + QString::number(epch_2 - round_off_offset) + ")";
    }
    else if(option == "Before")
    {
        sql_predicate = "(kMDItemLastUsedDate_Last_Opened_Date is not null) AND (kMDItemLastUsedDate_Last_Opened_Date != '') AND (kMDItemLastUsedDate_Last_Opened_Date <= " + QString::number(epch_1 - round_off_offset) + ")";
    }
    else if(option == "After")
    {

        sql_predicate = "(kMDItemLastUsedDate_Last_Opened_Date is not null) AND (kMDItemLastUsedDate_Last_Opened_Date != '') AND (kMDItemLastUsedDate_Last_Opened_Date >= " + QString::number(epch_1 - round_off_offset) + ")";
    }


    return sql_predicate;

}

void file_search_regular_filters::on_checkBox_date_change_clicked(bool checked)
{

    ui->comboBox_date_change_options->setEnabled(checked);
    ui->dateTimeEdit_date_change_1->setEnabled(checked);
    ui->dateTimeEdit_date_change_2->setEnabled(false);

    if(checked && ui->comboBox_date_change_options->currentText() == "Between")
        ui->dateTimeEdit_date_change_2->setEnabled(checked);


}

void file_search_regular_filters::on_comboBox_date_change_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_date_change_2->setEnabled(true);
    else
        ui->dateTimeEdit_date_change_2->setEnabled(false);

}

void file_search_regular_filters::on_checkBox_date_modified_clicked(bool checked)
{
    ui->comboBox_date_modified_options->setEnabled(checked);
    ui->dateTimeEdit_date_modified_1->setEnabled(checked);
    ui->dateTimeEdit_date_modified_2->setEnabled(false);

    if(checked && ui->comboBox_date_modified_options->currentText() == "Between")
        ui->dateTimeEdit_date_modified_2->setEnabled(checked);

}

void file_search_regular_filters::on_comboBox_date_modified_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_date_modified_2->setEnabled(true);
    else
        ui->dateTimeEdit_date_modified_2->setEnabled(false);
}

void file_search_regular_filters::on_checkBox_date_accessed_clicked(bool checked)
{
    ui->comboBox_date_accessed_options->setEnabled(checked);
    ui->dateTimeEdit_date_accessed_1->setEnabled(checked);
    ui->dateTimeEdit_date_accessed_2->setEnabled(false);

    if(checked && ui->comboBox_date_accessed_options->currentText() == "Between")
        ui->dateTimeEdit_date_accessed_2->setEnabled(checked);

}

void file_search_regular_filters::on_comboBox_date_accessed_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_date_accessed_2->setEnabled(true);
    else
        ui->dateTimeEdit_date_accessed_2->setEnabled(false);
}

void file_search_regular_filters::on_checkBox_date_created_clicked(bool checked)
{
    ui->comboBox_date_created_options->setEnabled(checked);
    ui->dateTimeEdit_date_created_1->setEnabled(checked);
    ui->dateTimeEdit_date_created_2->setEnabled(false);

    if(checked && ui->comboBox_date_created_options->currentText() == "Between")
        ui->dateTimeEdit_date_created_2->setEnabled(checked);

}

void file_search_regular_filters::on_comboBox_date_created_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_date_created_2->setEnabled(true);
    else
        ui->dateTimeEdit_date_created_2->setEnabled(false);
}

void file_search_regular_filters::on_checkBox_date_added_clicked(bool checked)
{
    ui->comboBox_date_added_options->setEnabled(checked);
    ui->dateTimeEdit_date_added_1->setEnabled(checked);
    ui->dateTimeEdit_date_added_2->setEnabled(false);

    if(checked && ui->comboBox_date_added_options->currentText() == "Between")
        ui->dateTimeEdit_date_added_2->setEnabled(checked);
}

void file_search_regular_filters::on_comboBox_date_added_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_date_added_2->setEnabled(true);
    else
        ui->dateTimeEdit_date_added_2->setEnabled(false);
}

void file_search_regular_filters::on_checkBox_content_creation_clicked(bool checked)
{
    ui->comboBox_content_creation_options->setEnabled(checked);
    ui->dateTimeEdit_content_creation_1->setEnabled(checked);
    ui->dateTimeEdit_content_creation_2->setEnabled(false);

    if(checked && ui->comboBox_content_creation_options->currentText() == "Between")
        ui->dateTimeEdit_content_creation_2->setEnabled(checked);

}

void file_search_regular_filters::on_comboBox_content_creation_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_content_creation_2->setEnabled(true);
    else
        ui->dateTimeEdit_content_creation_2->setEnabled(false);
}

void file_search_regular_filters::on_checkBox_content_modification_clicked(bool checked)
{
    ui->comboBox_content_modification_options->setEnabled(checked);
    ui->dateTimeEdit_content_modification_1->setEnabled(checked);
    ui->dateTimeEdit_content_modification_2->setEnabled(false);

    if(checked && ui->comboBox_content_modification_options->currentText() == "Between")
        ui->dateTimeEdit_content_modification_2->setEnabled(checked);

}

void file_search_regular_filters::on_comboBox_content_modification_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_content_modification_2->setEnabled(true);
    else
        ui->dateTimeEdit_content_modification_2->setEnabled(false);

}

void file_search_regular_filters::on_checkBox_last_used_clicked(bool checked)
{
    ui->comboBox_last_used_options->setEnabled(checked);
    ui->dateTimeEdit_last_used_1->setEnabled(checked);
    ui->dateTimeEdit_last_used_2->setEnabled(false);

    if(checked && ui->comboBox_last_used_options->currentText() == "Between")
        ui->dateTimeEdit_last_used_2->setEnabled(checked);

}

void file_search_regular_filters::on_comboBox_last_used_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == QString("Between"))
        ui->dateTimeEdit_last_used_2->setEnabled(true);
    else
        ui->dateTimeEdit_last_used_2->setEnabled(false);

}


void file_search_regular_filters::on_comboBox_file_size_options_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Between")
        ui->lineEdit_file_size_2->setEnabled(true);
    else
        ui->lineEdit_file_size_2->setEnabled(false);
}

void file_search_regular_filters::on_checkBox_file_size_clicked(bool checked)
{
    ui->comboBox_file_size_options->setEnabled(checked);
    ui->lineEdit_file_size_1->setEnabled(checked);
    ui->lineEdit_file_size_2->setEnabled(false);

    ui->comboBox_file_size_units->setEnabled(checked);
    if(checked && ui->comboBox_file_size_options->currentText() == "Between")
        ui->lineEdit_file_size_2->setEnabled(checked);

}

void file_search_regular_filters::slot_selected_source_list(QStringList selected_src_list)
{
    selected_source_cnt_nm_list = selected_src_list;
    prevoius_selected_source_cnt_nm_list = selected_src_list;


}

void file_search_regular_filters::on_radioButton_all_source_toggled(bool checked)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    if(!checked)
        return ;

    ui->pushButton_select_source_list->setEnabled(false);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void file_search_regular_filters::on_radioButton_select_source_toggled(bool checked)
{
    if(!checked)
        return ;

    selected_source_cnt_nm_list.clear();
    ui->pushButton_select_source_list->setEnabled(true);
}


void file_search_regular_filters::on_pushButton_select_source_list_clicked()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    select_source_interface_obj->pub_put_prevoius_selected_source_list(prevoius_selected_source_cnt_nm_list);
    select_source_interface_obj->pub_set_ui_essentials();
    select_source_interface_obj->pub_insert_data_in_table();
    select_source_interface_obj->show();


    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void file_search_regular_filters::on_checkBox_ocr_text_clicked(bool checked)
{
    {
        if(checked)
            ui->lineEdit_ocr_text->setEnabled(true);
        else
        {
            ui->lineEdit_ocr_text->clear();
            ui->lineEdit_ocr_text->setDisabled(true);
        }
    }
}

void file_search_regular_filters::on_checkBox_file_name_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit_file_name->setEnabled(true);
        ui->comboBox_file_name_options->setEnabled(true);
        ui->pushButton_file_name_select_category->setEnabled(true);
    }
    else
    {
        ui->lineEdit_file_name->clear();
        ui->lineEdit_file_name->setEnabled(false);
        ui->comboBox_file_name_options->setEnabled(false);
        ui->pushButton_file_name_select_category->setEnabled(false);

    }
}

void file_search_regular_filters::add_category_in_pushbutton_file_name_select_category()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString case_configuration_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    QString command = "SELECT category FROM  filename_category WHERE category_status = ?";
    QStringList category_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, QStringList("1"), 0, case_configuration_db_path,Q_FUNC_INFO);

    QFont font;
    font.setPointSize(12);

    menu_select_category  = new QMenu(this);
    menu_select_category->setFont(font);

    for(int count = 0 ; count < category_list.size() ; count++)
    {
        QString column_label = category_list.at(count);

        QAction *action_select_filename = new QAction(column_label , this);
        action_select_filename->setCheckable(true);

        menu_select_category->addAction(action_select_filename);
        menu_select_category->setMinimumWidth(ui->pushButton_file_name_select_category->width());

        connect(action_select_filename,SIGNAL(triggered(bool)),this,SLOT(slot_action_select_filename_category_triggered(bool)));
    }

    ui->pushButton_file_name_select_category->setMenu(menu_select_category);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void file_search_regular_filters::slot_action_select_filename_category_triggered(bool checked)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);


    QList<QAction *> menu_actions_list = menu_select_category->actions();

    if(menu_actions_list.isEmpty())
    {
        return;
    }

    existing_text_in_line_edit = ui->lineEdit_file_name->text().trimmed();

    QStringList selected_category_list;
    QStringList unselected_category_list;
    QStringList temp_file_name_list;
    QStringList unselected_file_name_list;

    QString case_configuration_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    QString command = "SELECT filename FROM filename_list  WHERE category = ?";

    for(int count = 0; count < menu_actions_list.size(); count++)
    {
        if(!menu_actions_list.at(count)->isChecked())
        {
            unselected_category_list.clear();
            unselected_category_list << menu_actions_list.at(count)->text().trimmed();
            unselected_file_name_list.append(recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, unselected_category_list, 0, case_configuration_db_path,Q_FUNC_INFO));
        }
        else
        {
            selected_category_list.clear();
            selected_category_list << menu_actions_list.at(count)->text().trimmed();
            temp_file_name_list.append(recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, selected_category_list, 0, case_configuration_db_path,Q_FUNC_INFO));
        }
    }

    for(int ii = 0 ; ii < unselected_file_name_list.size() ; ii++)
    {
        QString unselected_file_name = unselected_file_name_list.at(ii);

        if(existing_text_in_line_edit.contains( "," + unselected_file_name))
        {
            existing_text_in_line_edit.remove("," + unselected_file_name);
        }
        else if(existing_text_in_line_edit.contains(unselected_file_name))
        {
            existing_text_in_line_edit.remove(unselected_file_name);
        }

        ui->lineEdit_file_name->setText(existing_text_in_line_edit);
    }

    QStringList file_name_list;
    file_name_list << temp_file_name_list;
    for(int jj = 0 ; jj < temp_file_name_list.size() ; jj++)
    {
        QString selected_file_name = temp_file_name_list.at(jj);

        if(existing_text_in_line_edit.contains(selected_file_name.trimmed()))
        {
            file_name_list.removeAll(selected_file_name);
        }
    }

    QString file_name = file_name_list.join(",").trimmed();

    if(ui->lineEdit_file_name->text().trimmed().isEmpty())
    {
        ui->lineEdit_file_name->setText(file_name);
    }
    else
    {
        if(existing_text_in_line_edit.endsWith(","))
        {
            file_name = existing_text_in_line_edit + file_name;
            ui->lineEdit_file_name->clear();

        }
        else
        {
            file_name = QString(",") + file_name;
            file_name = existing_text_in_line_edit + file_name;
            ui->lineEdit_file_name->clear();

        }

        if(file_name.endsWith(","))
            file_name.chop(QString(",").size());

        if(file_name.startsWith(","))
            file_name.remove(0,QString(",").size());

        ui->lineEdit_file_name->setText(file_name);

    }
    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void file_search_regular_filters::closeEvent(QCloseEvent *)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    menu_select_category->close();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}
