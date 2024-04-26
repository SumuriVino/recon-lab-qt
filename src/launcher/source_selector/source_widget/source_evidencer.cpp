#include "source_evidencer.h"
#include "ui_source_evidencer.h"

source_evidencer::source_evidencer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::source_evidencer)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // setWindowFlags(Qt::Window);

    setWindowTitle("Source Info");

    message_dialog_obj = new message_dialog(this);


    ui->lineEdit_evidence->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->label_message->setText(global_lang_tr_obj.get_translated_string(ui->label_message->text()));
    ui->label_evidence->setText(global_lang_tr_obj.get_translated_string(ui->label_evidence->text()));
    ui->label_description->setText(global_lang_tr_obj.get_translated_string(ui->label_description->text()));
    ui->pushButton_ok->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_ok->text()));

    ui->pushButton_cancel->setHidden(true);

    ui->pushButton_src_password_show_hide->setIcon(QIcon("../icons/signs/eye_open.png"));
    ui->pushButton_src_password_show_hide->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->label_source_password->setText(global_lang_tr_obj.get_translated_string(ui->label_source_password->text()));
    ui->lineEdit_source_password->setAttribute(Qt::WA_MacShowFocusRect, false);

}

source_evidencer::~source_evidencer()
{
    delete ui;
}

void source_evidencer::set_source_message(QString message) // Set source message on this window
{
    ui->textEdit_description->clear();
    ui->lineEdit_evidence->clear();
    ui->label_message->setText(global_lang_tr_obj.get_translated_string("Please fill the info for source ") + "'" + message  + "'");
}

struct_global_source_evidencer source_evidencer::get_evicencer_info() //Get evidence info like number, description etc and fill that in structure
{
    struct_global_source_evidencer obj;
    QString evidence_no = ui->lineEdit_evidence->text();

    if(evidence_no.isEmpty())
    {
        evidence_no = ui->lineEdit_evidence->placeholderText();
    }

    QString src_password = ui->lineEdit_source_password->text();
    obj.source_password_by_examiner = src_password;
    obj.evidence_by_examiner = evidence_no;


    //Replace with Splitter Because Description may contains multiple lines
    //So in Source/Root Text Files write data in one line so we can easily extract and fill structure from that.

    QString added_description = ui->textEdit_description->toPlainText().trimmed();
    added_description.replace("\n" ,MACRO_RECON_Splitter_7_at_rate_in_brace);
    obj.description_by_examiner = added_description;

    return obj;
}

void source_evidencer::closeEvent(QCloseEvent *) // click on close button of this window
{
    // pub_bool_operation_ok = false;
}

void source_evidencer::on_pushButton_ok_clicked() //Click ok after adding all the required info
{

    if(ui->lineEdit_evidence->text().trimmed().isEmpty() && ui->lineEdit_evidence->placeholderText().isEmpty())
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_WARNING,"Please insert evidence no.");
        message_dialog_obj->show();
        return;
    }

    bool_evidence_operation_ok = true;

    hide();
}


void source_evidencer::on_pushButton_cancel_clicked() //click on cancel button
{
    bool_evidence_operation_ok = false;
    hide();

}

bool source_evidencer::pub_check_evidence_opertion_status() // Evidence number and other info enter status, true if enters. false if cancel
{
    return bool_evidence_operation_ok;
}

void source_evidencer::pub_hide_cancel_button(bool status) // cancel button
{
    ui->pushButton_cancel->setHidden(status);

}

void source_evidencer::pub_disable_evidence_no_and_description_widgets(bool status) // enable/disable evidence no according to status
{
    ui->lineEdit_evidence->setDisabled(status);
    ui->textEdit_description->setDisabled(status);
}

void source_evidencer::pub_set_evidence_no_and_description(QString evidence_no, QString description) // set evidence no and discription on widget line edit
{
    ui->lineEdit_evidence->setText(global_lang_tr_obj.get_translated_string(evidence_no));
    ui->textEdit_description->setText(global_lang_tr_obj.get_translated_string(description));
}

void source_evidencer::reject() //not in use
{
    return;
}

void source_evidencer::pub_set_evidence_number(QString source_file_path) // set evidence number. we use to name on result directory output
{
    QString image_case_directory_path = source_file_path.remove(source_file_path.lastIndexOf("/"),source_file_path.size());
    if(!image_case_directory_path.endsWith("/"))
        image_case_directory_path.append("/");


    QDir dir(image_case_directory_path);
    dir.setFilter(QDir::Files);
    QStringList directory_internal_file_name_list = dir.entryList();
    for(int ii = 0 ; ii < directory_internal_file_name_list.size() ; ii++)
    {
        QString directory_internal_file_name = directory_internal_file_name_list.at(ii);
        if(!directory_internal_file_name.endsWith("case.txt"))
            continue;

        QString image_case_text_file_path = image_case_directory_path + directory_internal_file_name;
        QFile case_file(image_case_text_file_path);
        if(!case_file.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("file open ---FAILED--- " + image_case_text_file_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + case_file.errorString(),Q_FUNC_INFO);
            return;
        }
        while(!case_file.atEnd())
        {
            QByteArray line = case_file.readLine();
            if(!line.contains("Evidence Number"))
            {
                continue;
            }
            QString evidence_number = line.remove(0,(line.indexOf(":") + 1));
            evidence_number = evidence_number.trimmed();
            ui->lineEdit_evidence->setPlaceholderText(evidence_number);
        }
        case_file.close();
    }
}

void source_evidencer::on_pushButton_src_password_show_hide_clicked(bool checked) //show/hide source password on button
{
    if(checked)
    {
        ui->pushButton_src_password_show_hide->setIcon(QIcon("../icons/signs/eye_close.png"));
        ui->lineEdit_source_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pushButton_src_password_show_hide->setIcon(QIcon("../icons/signs/eye_open.png"));
        ui->lineEdit_source_password->setEchoMode(QLineEdit::Password);
    }

}
