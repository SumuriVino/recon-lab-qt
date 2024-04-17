#include "file_export_dialog.h"
#include "ui_file_export_dialog.h"

file_export_dialog::file_export_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_export_dialog)
{
    ui->setupUi(this);

    // ui->lineEdit_di_path->setText(getenv("HOME") + QString("/Desktop"));

    ui->radioButton_export_to_zip->setChecked(true);

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

    prepare_encrypted_zip_password_dialog_box();

    ui->checkBox_recursive->show();
    ui->checkBox_recursive->setEnabled(false);

}

file_export_dialog::~file_export_dialog()
{
    delete ui;
}

void file_export_dialog::closeEvent(QCloseEvent *)
{
    ui->checkBox_encrypt_zip->setChecked(false);

    hide();
}

void file_export_dialog::on_pushButton_open_clicked()
{

    if(ui->radioButton_export_to_folder->isChecked())
    {
        QString path;

        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                path = selected_files.at(0);
            }
        }
        ui->lineEdit_di_path->setText(path);
    }
    else if(ui->radioButton_export_to_zip->isChecked())
    {
        QString path;

        file_dialog_obj.setNameFilter(("*.zip"));
        file_dialog_obj.setFileMode(QFileDialog::AnyFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                path = selected_files.at(0);

                if(!path.endsWith(".zip"))
                    path.append(".zip");
            }
        }

        ui->lineEdit_di_path->setText(path);
    }
    else if(ui->radioButton_export_to_dmg->isChecked())
    {
        QString path;

        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                path = selected_files.at(0);
            }
        }

        ui->lineEdit_di_path->setText(path);
    }
}

void file_export_dialog::on_pushButton_export_clicked()
{

    if(ui->radioButton_export_to_folder->isChecked())
    {

        bool bool_do_keep_dir_structure = false;

        if(ui->checkBox_recursive->isChecked())
            bool_do_keep_dir_structure = true;


        if(ui->lineEdit_di_path->text().trimmed() == QString(""))
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select a directory");
            return;
        }

        emit signal_export_file_dialog_work_done(bool_do_keep_dir_structure, ui->lineEdit_di_path->text() + "/");
    }
    else if(ui->radioButton_export_to_zip->isChecked())
    {
        if(ui->lineEdit_di_path->text().trimmed() == QString(""))
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select file path");
            return;
        }


        bool bool_encrypt_zip = false;
        if(ui->checkBox_encrypt_zip->isChecked())
            bool_encrypt_zip = true;

        QString password_enc_zip = lineedit_password_zip_enc->text();
        if(bool_encrypt_zip && password_enc_zip.isEmpty())
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please enter Password for encryption.");
            ui->checkBox_encrypt_zip->setChecked(false);
            return;
        }

        emit signal_export_file_dialog_work_done_for_zip(ui->lineEdit_di_path->text(), bool_encrypt_zip, password_enc_zip);

    }
    else if(ui->radioButton_export_to_dmg->isChecked())
    {
        if(ui->lineEdit_di_path->text().trimmed() == QString(""))
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select file path");
            return;
        }

        bool bool_do_keep_dir_structure = false;

        if(ui->checkBox_recursive->isChecked())
            bool_do_keep_dir_structure = true;

        emit signal_export_file_dialog_work_done_for_dmg(bool_do_keep_dir_structure, ui->lineEdit_di_path->text() + "/");
    }
}

void file_export_dialog::on_radioButton_export_to_zip_toggled(bool checked)
{
    ui->lineEdit_di_path->clear();

    if(checked)
    {
        ui->checkBox_recursive->setEnabled(false);
        ui->checkBox_encrypt_zip->setEnabled(true);
    }
    else
    {
        ui->checkBox_recursive->setEnabled(true);
        ui->checkBox_encrypt_zip->setEnabled(false);
    }

}

void file_export_dialog::on_radioButton_export_to_folder_toggled(bool checked)
{
    ui->lineEdit_di_path->clear();

    if(checked)
    {
        ui->checkBox_recursive->setEnabled(true);
        ui->checkBox_encrypt_zip->setEnabled(false);
    }
    else
    {
        ui->checkBox_recursive->setEnabled(false);
        ui->checkBox_encrypt_zip->setEnabled(true);
    }
}


void file_export_dialog::on_radioButton_export_to_dmg_toggled(bool checked)
{
    ui->lineEdit_di_path->clear();

    if(checked)
    {
        ui->checkBox_recursive->setEnabled(true);
        ui->checkBox_encrypt_zip->setEnabled(false);
    }
    else
    {
        ui->checkBox_recursive->setEnabled(false);
        ui->checkBox_encrypt_zip->setEnabled(true);
    }
}

void file_export_dialog::on_checkBox_encrypt_zip_clicked(bool checked)
{
    //restore defaults
    lineedit_password_zip_enc->clear();
    label_status_zip_enc->clear();


    //show password dialog
    if(checked)
        dialog_enc_zip_password_asker->show();
}

void file_export_dialog::prepare_encrypted_zip_password_dialog_box()
{
    dialog_enc_zip_password_asker = new QDialog(this);
    dialog_enc_zip_password_asker->setWindowModality(Qt::WindowModal);
    connect(dialog_enc_zip_password_asker,SIGNAL(rejected()),this,SLOT(slot_rejected()));

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_hashset = new QVBoxLayout(dialog_enc_zip_password_asker);

    pushbutton_okay   = new QPushButton("OK",this);
    pushbutton_cancel = new QPushButton("Cancel",this);

    pushbutton_okay->setFixedSize(QSize(80, 25));
    pushbutton_cancel->setFixedSize(QSize(80, 25));

    connect(pushbutton_okay,SIGNAL(clicked()),this,SLOT(slot_enc_zip_dialog_okay_clicked()));
    connect(pushbutton_cancel,SIGNAL(clicked()),this,SLOT(slot_enc_zip_dialog_cancel_clicked()));

    pushbutton_okay->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_cancel->setAttribute(Qt::WA_MacShowFocusRect,false);

    pushbutton_password_show_hide_zip_enc   = new QPushButton("",this);
    pushbutton_password_show_hide_zip_enc->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_password_show_hide_zip_enc->setCheckable(true);
    connect(pushbutton_password_show_hide_zip_enc,SIGNAL(clicked(bool)),this,SLOT(slot_pushButton_show_hide_password_clicked(bool)));

    pushbutton_password_show_hide_zip_enc->setStyleSheet("QPuhButton:flat{border: none; background-color: transparent;}\nQPuhButton{background:transparent;}\nQPushButton:checked{ background-color: transparent; border: none;  } \n"
                                                         "QPushButton:clicked{ background-color: transparent;  border: none;}               \n"
                                                         "QPushButton:pressed { background-color: transparent; border: none; }");

    label_status_zip_enc = new QLabel(this);
    label_status_zip_enc->clear();


    pushbutton_password_show_hide_zip_enc->setFixedSize(QSize(45, 30));
    QPixmap button_open("../icons/signs/eye_open.png");
    QIcon icon;
    icon.addPixmap(button_open, QIcon::Normal, QIcon::Off);
    pushbutton_password_show_hide_zip_enc->setIcon(icon);
    pushbutton_password_show_hide_zip_enc->setIconSize(QSize(60, 30));
    pushbutton_password_show_hide_zip_enc->setAutoDefault(true);
    pushbutton_password_show_hide_zip_enc->setFlat(true);

    lineedit_password_zip_enc = new QLineEdit(this);
    lineedit_password_zip_enc->setEchoMode(QLineEdit::Password);
    lineedit_password_zip_enc->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_password_zip_enc->setPlaceholderText("Password...");


    QLabel *label_msg = new QLabel("Please enter the password for encrypted zip.", this);

    QVBoxLayout *vbox_layout_lbl_msg = new QVBoxLayout;
    vbox_layout_lbl_msg->addWidget(label_msg);

    QHBoxLayout *hbox_layout_lineedit = new QHBoxLayout;
    hbox_layout_lineedit->addWidget(lineedit_password_zip_enc);
    hbox_layout_lineedit->addWidget(pushbutton_password_show_hide_zip_enc);

    vbox_layout_lbl_msg->addLayout(hbox_layout_lineedit);

    QSpacerItem *spacer = new QSpacerItem(15,10,QSizePolicy::Expanding,QSizePolicy::Maximum);

    QHBoxLayout *hbox_layout_okay_cancel = new QHBoxLayout;
    hbox_layout_okay_cancel->addWidget(label_status_zip_enc);
    hbox_layout_okay_cancel->addSpacerItem(spacer);
    hbox_layout_okay_cancel->addWidget(pushbutton_cancel);
    hbox_layout_okay_cancel->addWidget(pushbutton_okay);

    vboxlayout_hashset->addLayout(vbox_layout_lbl_msg);
    vboxlayout_hashset->addLayout(hbox_layout_okay_cancel);

    vboxlayout_hashset->setContentsMargins(6,6,6,6);
    vboxlayout_hashset->setSpacing(3);

    dialog_enc_zip_password_asker->setLayout(vboxlayout_hashset);
    dialog_enc_zip_password_asker->setMinimumWidth(200);
    dialog_enc_zip_password_asker->setFont(font_s);
    dialog_enc_zip_password_asker->setFixedSize(QSize(300, 110));
}

void file_export_dialog::slot_enc_zip_dialog_okay_clicked()
{
    QString password_enc_zip = lineedit_password_zip_enc->text().trimmed();
    if(password_enc_zip.isEmpty())
    {
        QPalette palette = label_status_zip_enc->palette();
        palette.setColor(label_status_zip_enc->foregroundRole(),QColor(Qt::red));
        label_status_zip_enc->setPalette(palette);

        label_status_zip_enc->setText("<b>Invalid Password.</b>");
        return;
    }


    dialog_enc_zip_password_asker->hide();
}

void file_export_dialog::slot_enc_zip_dialog_cancel_clicked()
{
    dialog_enc_zip_password_asker->hide();
    ui->checkBox_encrypt_zip->setChecked(false);

}


void file_export_dialog::slot_pushButton_show_hide_password_clicked(bool checked)
{

    QIcon icon;
    QPixmap button_close;
    if(checked)
    {
        button_close.load("../icons/signs/eye_close.png");
        lineedit_password_zip_enc->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        button_close.load("../icons/signs/eye_open.png");
        lineedit_password_zip_enc->setEchoMode(QLineEdit::Password);
    }
    icon.addPixmap(button_close, QIcon::Normal, QIcon::Off);
    pushbutton_password_show_hide_zip_enc->setIcon(icon);

}

void file_export_dialog::slot_rejected()
{
    // this slot called with Esc (escape) button press.

    if(lineedit_password_zip_enc->text().isEmpty())
    {
        //QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please enter Password for encryption.");
        ui->checkBox_encrypt_zip->setChecked(false);
        return;
    }
}
