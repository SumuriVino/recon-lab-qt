#include "set_global_timezone.h"
#include "ui_set_global_timezone.h"

set_global_timezone::set_global_timezone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::set_global_timezone)
{
    ui->setupUi(this);

    ui->dateEdit_start->setTimeSpec(Qt::UTC);
    ui->dateEdit_end->setTimeSpec(Qt::UTC);


    ui->dateEdit_start->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->dateEdit_end->setAttribute(Qt::WA_MacShowFocusRect,false);

    //    ui->dateEdit_end->setDisplayFormat("MMM-dd-yyyy");
    //    ui->dateEdit_start->setDisplayFormat("MMM-dd-yyyy");


    ui->dateEdit_start->setDisplayFormat("MMM-dd-yyyy hh:mm:ss");
    ui->dateEdit_end->setDisplayFormat("MMM-dd-yyyy hh:mm:ss");

    bool_edit_button_clicked = false;
    //message_dialog_obj = new message_dialog(this);
    message_dialog_obj = new message_dialog(dynamic_cast<QDialog *>(this));


}


set_global_timezone::~set_global_timezone()
{
    delete ui;
}

void set_global_timezone::pub_set_timezone(QList <QString> qlist_obj)
{

    bool_edit_button_clicked = false;

    QDate set_date;
    set_date.setDate(2000,1,1);
    ui->dateEdit_start->setDate(set_date);
    ui->dateEdit_end->setDate(QDate::currentDate());

    ui->comboBox_timezone->clear();

    ui->comboBox_timezone->addItem("<Select a Time Zone>");

    for(int ii = 0 ; ii < qlist_obj.size() ; ii++)
    {
        ui->comboBox_timezone->addItem(qlist_obj.at(ii));
    }

}

void set_global_timezone::pub_edit_timezone(QList <QString> qlist_obj, QString last_tmzn_str, QString last_start_str, QString last_end_str)
{
    bool_edit_button_clicked = true;

    last_tmzn_str  = last_tmzn_str.trimmed();
    last_start_str = last_start_str.trimmed();
    last_end_str   = last_end_str.trimmed();

    QDate set_date;
    QDateTime start_datetime;


    if(last_start_str.isEmpty())
    {
        set_date.setDate(2000,1,1);
        ui->dateEdit_start->setDate(set_date);
    }
    else
    {
        start_datetime.setTimeSpec(Qt::UTC);
        start_datetime = start_datetime.fromSecsSinceEpoch(last_start_str.toLongLong(), Qt::UTC);
        ui->dateEdit_start->setDateTime(start_datetime);
    }



    if(last_end_str.isEmpty())
        ui->dateEdit_end->setDateTime(QDateTime::currentDateTime());
    else
    {
        QDateTime end_datetime;
        end_datetime.setTimeSpec(Qt::UTC);
        end_datetime = end_datetime.fromSecsSinceEpoch(last_end_str.toLongLong(), Qt::UTC);

        ui->dateEdit_end->setDateTime(end_datetime);
    }

    ui->comboBox_timezone->clear();
    ui->comboBox_timezone->addItem("<Select a Time Zone>");

    int offset = 0;
    for(int ii = 0 ; ii < qlist_obj.size() ; ii++)
    {
        QString timezone_str = qlist_obj.at(ii);
        timezone_str = timezone_str.trimmed();

        if(timezone_str == last_tmzn_str)
            offset = ii + 1;

        ui->comboBox_timezone->addItem(timezone_str);
    }

    ui->comboBox_timezone->setCurrentIndex(offset);

}

void set_global_timezone::on_pushButton_cancel_clicked()
{
    emit signal_on_pushbutton_cancel_clicked();
}

void set_global_timezone::on_pushButton_OK_clicked()
{
    qint64 start_epch = ui->dateEdit_start->dateTime().toSecsSinceEpoch();

    qint64 end_epch = ui->dateEdit_end->dateTime().toSecsSinceEpoch();

    //    qint64 one_day_epoch = (1 * 24 * 60 * 60) - 1;
    //    end_epoch += one_day_epoch;


    if(start_epch > end_epch)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_WARNING ,"Invalid timestamp criteria.");
        message_dialog_obj->show();
        return;
    }
    else if(ui->comboBox_timezone->currentIndex() == 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please select a Time Zone");
        message_dialog_obj->show();
        return ;
    }

    if(bool_edit_button_clicked)
        emit signal_edit_time_zone(start_epch ,end_epch , ui->comboBox_timezone->currentText() , ui->dateEdit_start->text() ,ui->dateEdit_end->text());
    else
        emit signal_set_time_zone(start_epch ,end_epch , ui->comboBox_timezone->currentText() , ui->dateEdit_start->text() ,ui->dateEdit_end->text());

}
