
#include "add_notes.h"
#include "ui_add_notes.h"

add_notes::add_notes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_notes)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("Note"));

    message_dialog_object = new message_dialog(this);
}

add_notes::~add_notes()
{
    delete ui;
}

void add_notes::set_notes(QString data)
{
    ui->textEdit_notes->setText(data);
}

void add_notes::on_pushButton_add_clicked()
{
    if(ui->textEdit_notes->toPlainText().trimmed().isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"Notes can not be blank.");
        message_dialog_object->show();
        return;
    }

    emit signal_notes(ui->textEdit_notes->toPlainText());
    ui->textEdit_notes->clear();
}

