#include "edit_category_dialog.h"
#include "ui_edit_category_dialog.h"

edit_category_dialog::edit_category_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_category_dialog)
{
    ui->setupUi(this);
    ui->lineEdit_category->setAttribute(Qt::WA_MacShowFocusRect, 0);

    ui->pushButton_save_category->setIcon(QIcon("../icons/signs/save.png"));

}

edit_category_dialog::~edit_category_dialog()
{
    delete ui;
}

void edit_category_dialog::set_category_name(QString cat)
{
    ui->lineEdit_category->setText(cat);
}

void edit_category_dialog::on_pushButton_save_category_clicked()
{

    if(ui->lineEdit_category->text().trimmed() == QString(""))
        return;

    emit signal_edit_category(ui->lineEdit_category->text());
    this->hide();
}
