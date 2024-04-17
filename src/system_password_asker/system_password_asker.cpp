#include "system_password_asker.h"
#include "ui_system_password_asker.h"

system_password_asker::system_password_asker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::system_password_asker)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    ui->lineEdit_pwd->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);


    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_helper_process_obj = new recon_helper_process(this);

    QPixmap button_open("../icons/signs/eye_open.png");
    QIcon icon;
    icon.addPixmap(button_open, QIcon::Normal, QIcon::Off);

    ui->pushButton_show_hide_password->setIcon(icon);

    setWindowTitle("System Password");
    ui->lineEdit_pwd->setPlaceholderText("Type password");
    ui->lineEdit_pwd->setFocus();
    ui->label_message->setText(recon_static_functions::get_app_name() + " require admin password to run some of its components.");
}

system_password_asker::~system_password_asker()
{
    delete ui;
}

void system_password_asker::closeEvent(QCloseEvent *)
{
    emit signal_system_password_asker_workdone(false, "");

}

void system_password_asker::pub_set_window_modality()
{
    setWindowModality(Qt::WindowModal);

}

void system_password_asker::pub_set_essentials()
{
    ui->label_status->clear();
    // ui->lineEdit_pwd->clear();
}

void system_password_asker::pub_set_label_msg(QString text)
{
    ui->label_message->setText(text);
}

void system_password_asker::on_pushButton_cancel_clicked()
{
    emit signal_system_password_asker_workdone(false, "");
}

void system_password_asker::on_pushButton_ok_clicked()
{
    bool pwd_status = global_recon_helper_singular_class_obj->is_system_pwd_accurate(ui->lineEdit_pwd->text(), Q_FUNC_INFO);

    if(!pwd_status)
    {
        QPalette palette = ui->label_status->palette();
        palette.setColor(ui->label_status->foregroundRole(),QColor(Qt::red));
        ui->label_status->setPalette(palette);

        ui->label_status->setText("<b>Invalid Password.</b>");

        return;
    }


    QString m_password = ui->lineEdit_pwd->text();

    emit signal_system_password_asker_workdone(true, m_password);

}

void system_password_asker::on_pushButton_show_hide_password_clicked(bool checked)
{
    if(checked)
    {
        QPixmap button_open("../icons/signs/eye_close.png");
        QIcon icon;
        icon.addPixmap(button_open, QIcon::Normal, QIcon::Off);
        ui->pushButton_show_hide_password->setIcon(icon);
        ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        QPixmap button_open("../icons/signs/eye_open.png");
        QIcon icon;
        icon.addPixmap(button_open, QIcon::Normal, QIcon::Off);
        ui->pushButton_show_hide_password->setIcon(icon);

        ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    }

}

void system_password_asker::on_lineEdit_pwd_textChanged(const QString &arg1)
{
    ui->label_status->clear();

}

void system_password_asker::on_lineEdit_pwd_returnPressed()
{
    on_pushButton_ok_clicked();
}
