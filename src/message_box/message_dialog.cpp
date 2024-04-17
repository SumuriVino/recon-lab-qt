#include "message_dialog.h"
#include "ui_message_dialog.h"

message_dialog::message_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::message_dialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    setWindowTitle(recon_static_functions::get_app_name());

    ui->label_message->setWordWrap(true);
}

message_dialog::~message_dialog()
{
    delete ui;
}

void message_dialog::pub_set_message(QString msg_type,QString msg)
{
    set_message(msg_type,msg);
}

void message_dialog::pub_set_message(QString windo_title, QString msg_type, QString msg)
{
    setWindowTitle(windo_title);

    set_message(msg_type,msg);

}

void message_dialog::pub_set_file_path_message_for_global_report(QString msg_type,QStringList path_list)
{
    message_dialog::resize(485,130);

    ui->tableWidget->setAutoFillBackground(true);
    ui->tableWidget->setBackgroundRole(this->backgroundRole());
    ui->tableWidget->setForegroundRole(this->foregroundRole());

    ui->stackedWidget->setCurrentIndex(1);

    message = "";
    message_type = msg_type;

    ui->label_message->setText(message);
    ui->pushButton_yes->setText("OK");
    ui->pushButton_no->hide();

    QString icon_path;
    if(msg_type == MACRO_MSG_INFORMATION)
        icon_path = "../icons/signs/information.png";
    else if(msg_type == MACRO_MSG_WARNING)
        icon_path = "../icons/signs/warning.png";
    else if(msg_type == MACRO_MSG_REPORT_CREATE)
    {
        icon_path = "../icons/signs/report.png";
        ui->pushButton_yes->setText("OK");
        //        ui->pushButton_no->show();

        if(path_list.size() > 3)
            this->setMinimumHeight(175);
    }
    else if(msg_type == MACRO_MSG_CHECK_YES_NO)
    {
        icon_path = "../icons/signs/question.png";
        ui->pushButton_yes->setText("YES");
        ui->pushButton_no->show();
    }

    int w = ui->label_icon->width() - 5;
    int h = ui->label_icon->height() - 5;
    QSize msize(w, h);
    QImage img(icon_path);
    ui->label_icon->setPixmap(QPixmap::fromImage(img).scaled(msize,Qt::KeepAspectRatio, Qt::SmoothTransformation));


    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    ui->label_file_export_msg->setText("Report generated");

    for(int i = 0 ; i < path_list.size(); i++)
    {
        QString file_path = path_list.at(i);
        file_open_cell_widget *file_open_cell_widget_obj = new file_open_cell_widget;
        file_open_cell_widget_obj->setAutoFillBackground(true);
        file_open_cell_widget_obj->setBackgroundRole(this->backgroundRole());
        file_open_cell_widget_obj->setForegroundRole(this->foregroundRole());

        file_open_cell_widget_obj->pub_set_global_report_dir_name(file_path);

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setRowHeight(i,25);
        ui->tableWidget->setCellWidget(i,0,file_open_cell_widget_obj);
        ui->tableWidget->setColumnWidth(i,180);
    }

}

void message_dialog::pub_set_file_path_message(QString msg_type,QStringList path_list, QString export_dir_path)
{
    ui->stackedWidget->setCurrentIndex(1);

    message = "";
    message_type = msg_type;

    ui->label_message->setText(message);
    ui->pushButton_yes->setText("OK");
    ui->pushButton_no->hide();

    QString icon_path;
    if(msg_type == MACRO_MSG_INFORMATION)
        icon_path = "../icons/signs/information.png";
    else if(msg_type == MACRO_MSG_WARNING)
        icon_path = "../icons/signs/warning.png";
    else if(msg_type == MACRO_MSG_REPORT_CREATE)
    {
        icon_path = "../icons/signs/report.png";
        ui->pushButton_yes->setText("OK");
        //        ui->pushButton_no->show();
    }
    else if(msg_type == MACRO_MSG_CHECK_YES_NO)
    {
        icon_path = "../icons/signs/question.png";
        ui->pushButton_yes->setText("YES");
        ui->pushButton_no->show();
    }

    QPixmap mpix(icon_path);
    int w = ui->label_icon->width();
    int h = ui->label_icon->height();
    ui->label_icon->setPixmap(mpix.scaled(w,h,Qt::KeepAspectRatio));

    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);


    if(path_list.size() > 10)
    {
        export_dir_path = export_dir_path.trimmed();
        if(export_dir_path.isEmpty())
            export_dir_path = getenv("HOME");

        QString file_path = export_dir_path;
        file_open_cell_widget *file_open_cell_widget_obj = new file_open_cell_widget(file_path);

        ui->tableWidget->insertRow(0);
        ui->tableWidget->setCellWidget(0,0,file_open_cell_widget_obj);
        ui->tableWidget->setColumnWidth(0,180);

    }
    else
    {
        for(int i = 0 ; i < path_list.size(); i++)
        {
            QString file_path = path_list.at(i);
            file_open_cell_widget *file_open_cell_widget_obj = new file_open_cell_widget(file_path);

            ui->tableWidget->insertRow(i);
            ui->tableWidget->setCellWidget(i,0,file_open_cell_widget_obj);
            ui->tableWidget->setColumnWidth(i,180);
        }
    }

    this->setMinimumHeight(150);

}

void message_dialog::pub_set_minimum_window_size(int w, int h)
{
    this->setMinimumSize(w,h);
}


void message_dialog::on_pushButton_yes_clicked()
{

    if(message_type == MACRO_MSG_REPORT_CREATE)
    {
        //        emit signal_open_report(true);
    }
    else if(message_type == MACRO_MSG_CHECK_YES_NO)
    {
        emit signal_yes_no_check(true);
    }

    this->hide();
}

void message_dialog::on_pushButton_no_clicked()
{
    this->hide();
    if(message_type == MACRO_MSG_REPORT_CREATE)
    {
        emit signal_open_report(false);
    }
    else if(message_type == MACRO_MSG_CHECK_YES_NO)
    {
        emit signal_yes_no_check(false);
    }


    this->hide();

}

void message_dialog::set_message(QString msg_type, QString msg)
{

    ui->stackedWidget->setCurrentIndex(0);
    ui->label_file_export_msg->setText("");

    message = msg;
    message_type = msg_type;

    ui->label_message->setText(message);
    ui->pushButton_yes->setText("OK");
    ui->pushButton_no->hide();

    QString icon_path;
    if(msg_type == MACRO_MSG_INFORMATION)
        icon_path = "../icons/signs/information.png";
    else if(msg_type == MACRO_MSG_WARNING)
        icon_path = "../icons/signs/warning.png";
    else if(msg_type == MACRO_MSG_REPORT_CREATE)
    {
        icon_path = "../icons/signs/report.png";
        ui->pushButton_yes->setText("YES");
        ui->pushButton_no->show();

    }
    else if(msg_type == MACRO_MSG_CHECK_YES_NO)
    {
        icon_path = "../icons/signs/question.png";
        ui->pushButton_yes->setText("YES");
        ui->pushButton_no->show();
    }

    QPixmap mpix(icon_path);
    int w = ui->label_icon->width();
    int h = ui->label_icon->height();
    ui->label_icon->setPixmap(mpix.scaled(w,h,Qt::KeepAspectRatio));

    if(msg_type == MACRO_MSG_Nothing)
    {
        ui->label_icon->hide();
        ui->label_message->setAlignment(Qt::AlignLeft);
    }
    else
        ui->label_message->setAlignment(Qt::AlignCenter);

    ui->label_message->setScaledContents(true);
    ui->label_message->setWordWrap(true);

}
void message_dialog::pub_set_multiple_message_list(QString msg_type,QStringList message_list)
{
    ui->stackedWidget->setCurrentIndex(1);

    message_type = msg_type;

    ui->label_message->setText(message);
    ui->pushButton_yes->setText("OK");
    ui->pushButton_no->hide();

    QString icon_path;
    if(msg_type == MACRO_MSG_INFORMATION)
        icon_path = "../icons/signs/information.png";
    else if(msg_type == MACRO_MSG_WARNING)
        icon_path = "../icons/signs/warning.png";
    else if(msg_type == MACRO_MSG_REPORT_CREATE)
    {
        icon_path = "../icons/signs/report.png";
        ui->pushButton_yes->setText("OK");
        //        ui->pushButton_no->show();
    }
    else if(msg_type == MACRO_MSG_CHECK_YES_NO)
    {
        icon_path = "../icons/signs/question.png";
        ui->pushButton_yes->setText("YES");
        ui->pushButton_no->show();
    }

    QPixmap mpix(icon_path);
    int w = ui->label_icon->width();
    int h = ui->label_icon->height();
    ui->label_icon->setPixmap(mpix.scaled(w,h,Qt::KeepAspectRatio));

    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    ui->label_file_export_msg->setText("");

    for(int i = 0 ; i < message_list.size(); i++)
    {
        QString message = message_list.at(i);
        QLabel *message_text_label = new QLabel;
        message_text_label->setText(message);

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setCellWidget(i,0,message_text_label);
        ui->tableWidget->setColumnWidth(i,180);
    }
}

void message_dialog::pub_set_window_frameless(bool status)
{
    if(status)
        setWindowFlags(Qt::Window| Qt::FramelessWindowHint);
    else
        setWindowFlags(Qt::Window);

}
