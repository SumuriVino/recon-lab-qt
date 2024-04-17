#include "saved_graphs_dialog_box.h"
#include "ui_saved_graphs_dialog_box.h"

saved_graphs_dialog_box::saved_graphs_dialog_box(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saved_graphs_dialog_box)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Save Graph");
    ui->lineEdit->setAttribute(Qt::WA_MacShowFocusRect, false);

}

saved_graphs_dialog_box::~saved_graphs_dialog_box()
{
    delete ui;
}


void saved_graphs_dialog_box::pub_set_graph_name(QString graph_name)
{

    ui->lineEdit->setPlaceholderText(graph_name);

}

void saved_graphs_dialog_box::pub_clear_graph_name()
{
   ui->lineEdit->clear();
}


void saved_graphs_dialog_box::on_pushButton_clicked()
{
    QString file_name = ui->lineEdit->text() ;

    if(file_name.trimmed().isEmpty())
        file_name = ui->lineEdit->placeholderText();


    emit signal_save_graph_name(file_name);

}
