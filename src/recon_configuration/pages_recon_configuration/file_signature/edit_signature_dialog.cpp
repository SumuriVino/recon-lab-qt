#include "edit_signature_dialog.h"
#include "ui_edit_signature_dialog.h"

edit_signature_dialog::edit_signature_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_signature_dialog)
{
    ui->setupUi(this);

    ui->radioButton_ascii->setChecked(true);
    ui->lineEdit_label->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEdit_signature->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEdit_offset->setAttribute(Qt::WA_MacShowFocusRect, 0);

    ui->pushButton_add_edit_signature->setIcon(QIcon("../icons/signs/save.png"));

    setWindowTitle(global_lang_tr_obj.get_translated_string(windowTitle()));

    ui->label_label->setText(global_lang_tr_obj.get_translated_string(ui->label_label->text()));
    ui->label_offset->setText(global_lang_tr_obj.get_translated_string(ui->label_offset->text()));
    ui->label_signature->setText(global_lang_tr_obj.get_translated_string(ui->label_signature->text()));
    ui->pushButton_add_edit_signature->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_add_edit_signature->text()));


}

edit_signature_dialog::~edit_signature_dialog()
{
    delete ui;
}

void edit_signature_dialog::set_laber_header(QString label, QString header , QString offset)
{
    ui->lineEdit_label->setText(label);
    ui->lineEdit_signature->setText(header);
    ui->lineEdit_offset->setText(offset);

    if(header.startsWith("0x"))
        ui->radioButton_hex->setChecked(true);
    else
        ui->radioButton_ascii->setChecked(true);

}


void edit_signature_dialog::on_pushButton_add_edit_signature_clicked()
{
    this->hide();
    emit signal_signature_edit(ui->lineEdit_label->text(), ui->lineEdit_signature->text() , ui->lineEdit_offset->text());
}
