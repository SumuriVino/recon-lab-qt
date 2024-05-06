#include "email_search.h"
#include "ui_email_search.h"

email_search::email_search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::email_search)
{
    ui->setupUi(this);

    setWindowTitle(MACRO_Email_Search_Dialog_Display_Name);

    ui->lineEdit_search_keyword->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_subject->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_sender->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_receiver->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_cc->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_search_to->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->dateTimeEdit_search_from->setAttribute(Qt::WA_MacShowFocusRect, false);

    pub_load_defaults();
}

email_search::~email_search()
{
    delete ui;
}

void email_search::pub_load_defaults()
{ // load default settings for email search ui
    ui->lineEdit_search_keyword->clear();
    ui->lineEdit_subject->clear();
    ui->lineEdit_sender->clear();
    ui->lineEdit_receiver->clear();
    ui->lineEdit_cc->clear();

    ui->radioButton_has_attachments->setEnabled(false);
    ui->radioButton_has_no_attachments->setEnabled(false);
    ui->checkBox_date->setChecked(false);
    ui->checkBox_attachments->setChecked(false);

    ui->comboBox_subject_search_filters->setCurrentIndex(0);
    ui->comboBox_sender_search_filters->setCurrentIndex(0);
    ui->comboBox_receiver_search_filters->setCurrentIndex(0);
    ui->comboBox_cc_search_filter->setCurrentIndex(0);
    ui->comboBox_date_time_filter->setCurrentIndex(0);

    on_checkBox_attachments_clicked(false);
    on_checkBox_date_clicked(false);

}


void email_search::on_pushButton_search_clicked()
{ // search button clicked

    if(ui->checkBox_date->isChecked())
    {
        qint64 epch_from = ui->dateTimeEdit_search_from->dateTime().toSecsSinceEpoch();
        qint64 epch_to = ui->dateTimeEdit_search_to->dateTime().toSecsSinceEpoch();

        if(epch_from > epch_to)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Invalid timestamp criteria");
            return;
        }
    }


    QString command = "select INT,bookmark,emlx_to,emlx_from,emlx_cc,emlx_date,emlx_subject,emlx_msg_body,email_attachment,account_id,mbox,source_file,recon_tag_value,source_count_name from emlx_data WHERE ";

    QStringList where_items;

    QString where_combiner;

    where_combiner = " AND ";

    if(ui->lineEdit_search_keyword->text().trimmed() != QString(""))
        where_items.append(get_email_search_sql_predicate());


    if( ui->lineEdit_subject->text().trimmed() != QString(""))
        where_items.append(get_email_subject_sql_predicate());


    if(ui->lineEdit_sender->text().trimmed() != QString(""))
        where_items.append(get_email_sender_sql_predicate());


    if(ui->lineEdit_receiver->text().trimmed() != QString(""))
        where_items.append(get_email_receiver_sql_predicate());

    if(ui->lineEdit_cc->text().trimmed() != QString(""))
        where_items.append(get_email_cc_sql_predicate());

    if(ui->checkBox_attachments->isChecked())
    {
        if( ui->radioButton_has_attachments->isChecked() || ui->radioButton_has_no_attachments->isChecked())
            where_items.append(get_email_attachments_sql_predicate());
    }

    if(ui->checkBox_date->isChecked())
        where_items.append(get_date_filter_sql_predicate());

    if(where_items.size() < 1 )
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please provide atleast one filter");
        return;
    }

    QString where_statement = where_items.join(where_combiner);

    command.append(where_statement);

    emit signal_email_search_work_done(command);

}

void email_search::on_checkBox_date_clicked(bool checked)
{ // checkbox date enable accordingly
    ui->comboBox_date_time_filter->setEnabled(checked);
    ui->dateTimeEdit_search_from->setEnabled(checked);
    ui->dateTimeEdit_search_to->setEnabled(false);
    ui->dateTimeEdit_search_to->setCalendarPopup(true);
    ui->dateTimeEdit_search_from->setCalendarPopup(true);

    if(checked && ui->comboBox_date_time_filter->currentText() == MACRO_Generic_Filter_Condition_Between)
    {
        ui->dateTimeEdit_search_to->setEnabled(checked);
    }

}

void email_search::on_checkBox_attachments_clicked(bool checked)
{ // checkbox attachment
    ui->radioButton_has_attachments->setEnabled(checked);
    ui->radioButton_has_no_attachments->setEnabled(checked);
    ui->radioButton_has_no_attachments->setChecked(checked);
}

void email_search::on_comboBox_date_time_filter_currentTextChanged(const QString &arg)
{ // date time filter line edit perform on text change/enter
    if(arg == QString(MACRO_Generic_Filter_Condition_Between))
    {
        ui->dateTimeEdit_search_to->setEnabled(true);
    }
    else
    {
        ui->dateTimeEdit_search_to->setEnabled(false);
    }
}

QString email_search::get_email_search_sql_predicate()
{ // prepare search command from sql
    if(ui->lineEdit_search_keyword->text().trimmed() == QString(""))
        return "";

    QString sql_predicate_email_keyword;
    QString email_content_search_keyword = ui->lineEdit_search_keyword->text().trimmed();

    sql_predicate_email_keyword.append("emlx_msg_body LIKE '%" + email_content_search_keyword.trimmed() + "%'");
    sql_predicate_email_keyword.prepend("(").append(")");

    emit signal_highlight_searched_text(email_content_search_keyword);

    return sql_predicate_email_keyword;

}

QString email_search::get_email_subject_sql_predicate()
{ // prepare search command for email subject
    if (ui->lineEdit_subject->text().trimmed() == QString(""))
        return "";

    QString sql_predicate_email_subject;
    QString email_subject = ui->lineEdit_subject->text().trimmed();

    if(ui->comboBox_subject_search_filters->currentText() == MACRO_Generic_Filter_Condition_Contains)
    {
        sql_predicate_email_subject.append("emlx_subject LIKE '%" + email_subject.trimmed() + "%'");
    }
    else if(ui->comboBox_subject_search_filters->currentText() == MACRO_Generic_Filter_Condition_Matches)
    {
        sql_predicate_email_subject.append("emlx_subject LIKE '" + email_subject.trimmed()  + "'");
    }
    else if(ui->comboBox_subject_search_filters->currentText() == MACRO_Generic_Filter_Condition_Begins_With)
    {
        sql_predicate_email_subject.append("emlx_subject LIKE '" + email_subject.trimmed() + "%'");
    }
    else if(ui->comboBox_subject_search_filters->currentText() == MACRO_Generic_Filter_Condition_Ends_With)
    {
        sql_predicate_email_subject.append("emlx_subject LIKE '%" + email_subject.trimmed() + "'");
    }

    sql_predicate_email_subject.prepend("(").append(")");

    return sql_predicate_email_subject;

}

QString email_search::get_email_sender_sql_predicate()
{ // prepare search command for sender

    if(ui->lineEdit_sender->text().trimmed() == QString(""))
        return "";

    QString sql_predicate_email_sender;
    QString email_sender_name = ui->lineEdit_sender->text().trimmed();

    if(ui->comboBox_sender_search_filters->currentText() == MACRO_Generic_Filter_Condition_Contains)
    {
        sql_predicate_email_sender.append("emlx_from LIKE '%" + email_sender_name.trimmed() + "%'");
    }
    else if(ui->comboBox_sender_search_filters->currentText() == MACRO_Generic_Filter_Condition_Matches)
    {
        sql_predicate_email_sender.append("emlx_from LIKE '" + email_sender_name.trimmed()  + "'");
    }
    else if(ui->comboBox_sender_search_filters->currentText() == MACRO_Generic_Filter_Condition_Begins_With)
    {
        sql_predicate_email_sender.append("emlx_from LIKE '" + email_sender_name.trimmed() + "%'");
    }
    else if(ui->comboBox_sender_search_filters->currentText() == MACRO_Generic_Filter_Condition_Ends_With)
    {
        sql_predicate_email_sender.append("emlx_from LIKE '%" + email_sender_name.trimmed() + "'");
    }

    sql_predicate_email_sender.prepend("(").append(")");

    return sql_predicate_email_sender;

}

QString email_search::get_email_receiver_sql_predicate()
{ // prepare search command for receiver

    if(ui->lineEdit_receiver->text().trimmed() == QString(""))
        return "";

    QString sql_predicate_email_receiver;
    QString receiver_name = ui->lineEdit_receiver->text().trimmed();

    if(ui->comboBox_receiver_search_filters->currentText() == MACRO_Generic_Filter_Condition_Contains)
    {
        sql_predicate_email_receiver.append("emlx_to LIKE '%" + receiver_name.trimmed() + "%'");
    }
    else if(ui->comboBox_receiver_search_filters->currentText() == MACRO_Generic_Filter_Condition_Matches)
    {
        sql_predicate_email_receiver.append("emlx_to LIKE '" + receiver_name.trimmed() + "'");
    }
    else if(ui->comboBox_receiver_search_filters->currentText() == MACRO_Generic_Filter_Condition_Begins_With)
    {
        sql_predicate_email_receiver.append("emlx_to LIKE '" + receiver_name.trimmed() + "%'");
    }
    else if(ui->comboBox_receiver_search_filters->currentText() == MACRO_Generic_Filter_Condition_Ends_With)
    {
        sql_predicate_email_receiver.append("emlx_to LIKE '%" + receiver_name.trimmed() + "'");
    }

    sql_predicate_email_receiver.prepend("(").append(")");

    return sql_predicate_email_receiver;

}

QString email_search::get_email_cc_sql_predicate()
{ // prepare searech command for cc
    if(ui->lineEdit_cc->text().trimmed() == QString(""))
        return "";

    QString sql_predicate_email_cc;
    QString cc_name = ui->lineEdit_cc->text().trimmed();

    if(ui->comboBox_cc_search_filter->currentText() == MACRO_Generic_Filter_Condition_Contains)
    {
        sql_predicate_email_cc.append("emlx_cc LIKE '%" + cc_name.trimmed() + "%'");
    }
    else if(ui->comboBox_cc_search_filter->currentText() == MACRO_Generic_Filter_Condition_Matches)
    {
        sql_predicate_email_cc.append("emlx_cc LIKE '" + cc_name.trimmed() + "'");
    }
    else if(ui->comboBox_cc_search_filter->currentText() == MACRO_Generic_Filter_Condition_Begins_With)
    {
        sql_predicate_email_cc.append("emlx_cc LIKE '" + cc_name.trimmed() + "%'");
    }
    else if(ui->comboBox_cc_search_filter->currentText() == MACRO_Generic_Filter_Condition_Ends_With)
    {
        sql_predicate_email_cc.append("emlx_cc LIKE '%" + cc_name.trimmed() + "'");
    }

    sql_predicate_email_cc.prepend("(").append(" )");

    return sql_predicate_email_cc;

}

QString email_search::get_email_attachments_sql_predicate()
{ // prepare search command for email attachment
    QString sql_predicate_email_attachments;

    if(ui->radioButton_has_attachments->isChecked())
    {
        sql_predicate_email_attachments.append("email_attachment NOT NULL");
    }
    if(ui->radioButton_has_no_attachments->isChecked())
    {
        sql_predicate_email_attachments.append("email_attachment IS NULL");
    }

    sql_predicate_email_attachments.prepend("(").append(")");

    return sql_predicate_email_attachments;

}

QString email_search::get_date_filter_sql_predicate()
{ // prepare search command for date filter
    QString sql_predicate_date;
    qint64 epch_from, epch_to;
    QString option = ui->comboBox_date_time_filter->currentText();

    epch_from = ui->dateTimeEdit_search_from->dateTime().toSecsSinceEpoch();
    epch_to = ui->dateTimeEdit_search_to->dateTime().toSecsSinceEpoch();

    if(option == MACRO_Generic_Filter_Condition_Between)
    {
        sql_predicate_date = "(emlx_date >= " + QString::number(epch_from) + " AND emlx_date <= " + QString::number(epch_to) + ")";
    }
    else if(option == MACRO_Generic_Filter_Condition_Before)
    {
        sql_predicate_date = "(emlx_date is not null) AND (emlx_date != '') AND (emlx_date <= " + QString::number(epch_from ) + ")";
    }
    else if(option == MACRO_Generic_Filter_Condition_After)
    {
        sql_predicate_date = "(emlx_date is not null) AND (emlx_date != '') AND (emlx_date >= " + QString::number(epch_from ) + ")";
    }

    return sql_predicate_date;
}

