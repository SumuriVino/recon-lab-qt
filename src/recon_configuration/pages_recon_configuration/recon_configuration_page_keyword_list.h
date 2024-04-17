#ifndef RECON_CONFIGURATION_PAGE_KEYWORD_LIST_H
#define RECON_CONFIGURATION_PAGE_KEYWORD_LIST_H

#include <QWidget>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QtCore>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QMenu>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QSplitter>

                                   
#include "recon_generic_functions/recon_static_functions.h"
#include <QApplication>
#include <QClipboard>
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_configuration_page_keyword_list;
}

class recon_configuration_page_keyword_list : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_keyword_list(QWidget *parent = 0);
    ~recon_configuration_page_keyword_list();

    void pub_set_caller_type(QString caller_type);

private slots:
    void on_pushButton_category_add_clicked();
    void on_pushButton_category_remove_clicked();
    void on_pushButton_keyword_add_clicked();
    void on_pushButton_keyword_remove_clicked();

    void on_tableWidget_category_cellClicked(int row, int column);

    void slot_lineedit_add_category_text_changed(QString text);
    void slot_pushbutton_dialog_add_category_clicked();
    void slot_category_cancel_button_clicked();
    void slot_lineedit_add_keyword_text_changed(QString text);
    void slot_pushbutton_dialog_add_keyword_clicked();
    void slot_keyword_cancel_button_clicked();

    void slot_line_edit_keyword_return_pressed();
    void slot_new_category_line_edit_return_pressed();

   void on_pushButton_keyword_clipboard_clicked();

   void on_tableWidget_keywords_cellClicked(int row, int column);


   void slot_pushbutton_dialog_edit_add_category_clicked();
   void slot_lineedit_edit_category_text_changed(QString text);
   void on_pushButton_edit_category_clicked();

   void slot_edit_category_cancel_button_clicked();
   void on_pushButton_edit_keyword_clicked();

   void slot_lineedit_edit_keyword_text_changed(QString text);
   void slot_pushbutton_dialog_edit_keyword_add_clicked();
   void slot_edit_keyword_cancel_button_clicked();
private:
    Ui::recon_configuration_page_keyword_list *ui;

    QFont font_s;

    ///=====Add Category
    QDialog     *dialog_add_category;
    QPushButton *pushbutton_add_category;
    QLineEdit   *lineedit_add_category;
    QLabel      *label_message_category;

    QLineEdit   *lineedit_edit_category;
    QDialog     *dialog_edit_category;
    QPushButton *pushbutton_edit_category;
    QLabel      *label_message_edit_category;



    ///=====Add Keyword
    QDialog     *dialog_add_keyword;
    QPushButton *pushbutton_add_keyword;
    QLineEdit   *lineedit_add_keyword;
    QLabel      *label_message_keyword;

    QLineEdit   *lineedit_edit_keyword;
    QDialog     *dialog_edit_keyword;
    QPushButton *pushbutton_edit_keyword;
    QLabel      *label_message_edit_keyword;



    void prepare_category_display();
    void prepare_keyword_display();
    void prepare_add_category_dialog();
    void prepare_add_keyword_dialog();

    QLineEdit *new_keyword_LineEdit;
    QLineEdit *new_category_LineEdit;

    QString destination_db_path;

    QString category_to_update_str;
    QString keyword_to_update_str;

    void prepare_edit_category_dialog();
    void prepare_edit_keyword_dialog();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif
