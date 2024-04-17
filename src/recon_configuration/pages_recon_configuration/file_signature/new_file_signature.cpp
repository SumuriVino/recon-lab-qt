#include "new_file_signature.h"
#include "ui_new_file_signature.h"

new_file_signature::new_file_signature(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_file_signature)
{
    ui->setupUi(this);
    ui->radioButton_ascii->setChecked(true);
    ui->lineEdit_label->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEdit_signature->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEdit_offset->setAttribute(Qt::WA_MacShowFocusRect, 0);

    ui->pushButton_add_file_signature->setIcon(QIcon("../icons/signs/add.png"));
    message_dialog_object = new message_dialog;


    setWindowTitle(global_lang_tr_obj.get_translated_string(windowTitle()));
    ui->label_label->setText(global_lang_tr_obj.get_translated_string(ui->label_label->text()));
    ui->label_offset->setText(global_lang_tr_obj.get_translated_string(ui->label_offset->text()));
    ui->label_signature->setText(global_lang_tr_obj.get_translated_string(ui->label_signature->text()));
    ui->pushButton_add_file_signature->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_add_file_signature->text()));

}

new_file_signature::~new_file_signature()
{
    delete ui;
}

void new_file_signature::set_current_category(QString cat)
{
    current_cat = cat;
}

void new_file_signature::pub_set_destination_db_file(QString db_path)
{
    destination_db_file = db_path;
}

void new_file_signature::set_list_signature(QStringList list)
{
    received_list_signature = list;
}

void new_file_signature::on_pushButton_add_file_signature_clicked()
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);

    QString mtext = ui->lineEdit_label->text().trimmed();
    for(int i = 0; i < received_list_signature.count(); i++)
    {
        if(mtext == received_list_signature.at(i))
        {
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"Signature Label Already Exist");
            message_dialog_object->show();
            return;
        }
    }

    if(ui->lineEdit_label->text().trimmed() == QString(""))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,global_lang_tr_obj.get_translated_string("Please add a Label."));
        message_dialog_object->show();

        return;
    }

    if(ui->lineEdit_signature->text().trimmed() == QString(""))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,global_lang_tr_obj.get_translated_string("Please add a Signature."));
        message_dialog_object->show();
        return;
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("insert into tbl_signature_list (category, label, header,signature_status , offset) values(?, ?, ?, ?, ?)");
    query.addBindValue(current_cat);
    query.addBindValue(ui->lineEdit_label->text().trimmed());
    if(ui->radioButton_hex->isChecked())
    {
        query.addBindValue("0x" + ui->lineEdit_signature->text().remove("0x"));
    }
    else
    {
        query.addBindValue(ui->lineEdit_signature->text().trimmed());
    }
    query.addBindValue("1");
    query.addBindValue(ui->lineEdit_offset->text().trimmed());

    if(!query.exec())
    {
        recon_static_functions::app_debug(" insert ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    ui->lineEdit_signature->clear();
    ui->lineEdit_label->clear();
    ui->lineEdit_offset->clear();

    this->hide();

    recon_static_functions::app_debug("- END",Q_FUNC_INFO);

    emit signal_signature_added();
}
