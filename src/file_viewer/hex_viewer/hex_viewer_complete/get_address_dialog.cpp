#include "get_address_dialog.h"
#include "ui_get_address_dialog.h"

get_address_dialog::get_address_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::get_address_dialog)
{
    ui->setupUi(this);
    ui->radioButton_from_start->setChecked(true);

    QRegularExpression rx("^[0-9]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->lineEdit_address->setValidator(validator);
    ui->lineEdit_address->setAttribute(Qt::WA_MacShowFocusRect, false);

}

get_address_dialog::~get_address_dialog()
{
    delete ui;
}

void get_address_dialog::on_pushButton_clicked()
{
    QString jump_offset_str = ui->lineEdit_address->text().trimmed();
    if(jump_offset_str.isEmpty())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Please enter an 'Offset'.");
        return;
    }

    qint64 offset = jump_offset_str.toLongLong() * ui->comboBox_multiplier->currentText().toLongLong();

    if(ui->radioButton_from_start->isChecked())
        emit signal_jump_to_offset(offset, enum_offset_jump_from_start);
    else if(ui->radioButton_fromP_curson_position->isChecked())
        emit signal_jump_to_offset(offset, enum_offset_jump_from_current);
    else
        emit signal_jump_to_offset(offset, enum_offset_jump_from_back);

    hide();
}
