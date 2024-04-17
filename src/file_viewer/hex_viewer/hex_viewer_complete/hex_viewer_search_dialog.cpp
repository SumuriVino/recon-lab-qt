#include "hex_viewer_search_dialog.h"
#include "ui_hex_viewer_search_dialog.h"

hex_viewer_search_dialog::hex_viewer_search_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hex_viewer_search_dialog)
{
    ui->setupUi(this);

    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->comboBox_content_type->setCurrentIndex(0);
}

hex_viewer_search_dialog::~hex_viewer_search_dialog()
{
    delete ui;
}

void hex_viewer_search_dialog::pub_highlight_keyword(QString keyword_str)
{
    keyword_str = keyword_str.trimmed();
    if(keyword_str.isEmpty())
        return;

    ui->lineEdit_search->setText(keyword_str);
    ui->comboBox_content_type->setCurrentText("ASCII");

    on_pushButton_search_clicked();
}

void hex_viewer_search_dialog::on_pushButton_search_clicked()
{
    QString selected_type = ui->comboBox_content_type->currentText().trimmed();
    QString searched_text_trimmed = ui->lineEdit_search->text().trimmed();

    if(searched_text_trimmed.trimmed().isEmpty())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Please insert valid text to search");
        return;
    }

    if(selected_type == QString("HEX"))
    {
        if(searched_text_trimmed.contains(" "))
            searched_text_trimmed.replace(" ","");

        searched_text_trimmed = searched_text_trimmed.toLower();

        if(searched_text_trimmed.size() % 2 != 0)
        {
            QMessageBox::information(this, recon_static_functions::get_app_name(), "Please insert valid Hex to search");
            return;
        }

        for(int i = 0; i < searched_text_trimmed.size(); i++)
        {
            QChar ch = searched_text_trimmed.at(i);

            int ascii = ch.toLatin1();

            if(((ascii < 48) || (ascii > 57)) && ((ascii < 97) || (ascii > 102)))
            {
                QMessageBox::information(this, recon_static_functions::get_app_name(), "Please insert valid Hex to search");
                return;
            }
        }
    }
    else if(selected_type == QString("ASCII"))
    {
        if(searched_text_trimmed.isEmpty())
        {
            QMessageBox::information(this, recon_static_functions::get_app_name(), "Please insert valid ASCII to search");
            return;
        }
    }
    else if(selected_type == QString("UTF-16"))
    {
        if(searched_text_trimmed.isEmpty())
        {
            QMessageBox::information(this, recon_static_functions::get_app_name(), "Please insert valid UTF-16 to search");
            return;
        }
    }

    emit signal_search_text_and_type(searched_text_trimmed, selected_type, true);

    hide();
}

