#include "timeline_selection_window.h"
#include "ui_timeline_selection_window.h"



timeline_selection_window::timeline_selection_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timeline_selection_window)
{
    ui->setupUi(this);

    ui->dateTimeEdit_start_time->setTimeSpec(Qt::UTC);
    ui->dateTimeEdit_end_time->setTimeSpec(Qt::UTC);

    date_time = ui->dateTimeEdit_start_time->dateTime();
    end_date_time = QDateTime::currentDateTime();


    ui->dateTimeEdit_end_time->setDateTime(end_date_time);

    ui->pushButton_set_timeline->setIcon(QIcon("../icons/signs/timeline.png"));


}

timeline_selection_window::~timeline_selection_window()
{
    delete ui;
}

void timeline_selection_window::closeEvent(QCloseEvent *)
{
    ui->dateTimeEdit_start_time->setDateTime(date_time);

    ui->dateTimeEdit_end_time->setDateTime(QDateTime::currentDateTime());

}

void timeline_selection_window::on_pushButton_set_timeline_clicked()
{
   // qint64 start_epoch = 0;
//    qint64 end_epoch = QDateTime::currentMSecsSinceEpoch()/1000;

    qint64 start_epch = ui->dateTimeEdit_start_time->dateTime().toSecsSinceEpoch();
    qint64 end_epch = ui->dateTimeEdit_end_time->dateTime().toSecsSinceEpoch() - end_date_time.time().second();


    if(start_epch > end_epch)
    {
        message_dialog *message_dialog_object = new message_dialog(this);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Invalid timestamp search.");
               message_dialog_object->show();

        return;
    }


    emit signal_time_line(start_epch, end_epch);


}
