#ifndef RECON_CONFIGURATION_PAGE_FILENAME_LIST_H
#define RECON_CONFIGURATION_PAGE_FILENAME_LIST_H


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
class recon_configuration_page_filename_list;
}

class recon_configuration_page_filename_list : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_filename_list(QWidget *parent = 0);
    ~recon_configuration_page_filename_list();

    void pub_set_caller_type(QString caller_type);

private slots:
    void on_pushButton_category_add_clicked();
    void on_pushButton_category_remove_clicked();
    void on_pushButton_filename_add_clicked();
    void on_pushButton_filename_remove_clicked();

    void on_tableWidget_category_cellClicked(int row, int column);


    void slot_filename_line_add_return_pressed();


    //void slot_filename_line_edit_return_pressed();
    void slot_new_category_line_edit_return_pressed();

   void on_pushButton_filename_clipboard_clicked();

   void slot_pushbutton_add_edited_category_clicked();
   void slot_lineedit_edit_category_text_changed(QString text);
   void on_pushButton_edit_category_clicked();

   void slot_edit_category_cancel_button_clicked();
   void on_pushButton_edit_filename_clicked();

   void slot_lineedit_edit_filename_text_changed(QString text);
   void slot_pushbutton_add_edited_filename_clicked();
   void slot_edit_filename_cancel_button_clicked();

private:
    Ui::recon_configuration_page_filename_list *ui;

    QFont font_s;

    ///=====Add Category
    QDialog     *dialog_add_category;
    QPushButton *pushbutton_add_category;
    QLineEdit   *lineedit_add_category;
    QLabel      *label_message_category;

    QLineEdit   *lineedit_edit_category;
    QDialog     *dialog_edit_category;
    QPushButton *pushbutton_add_edited_category;
    QLabel      *label_message_edit_category;



    ///=====Add Filename
    QDialog     *dialog_add_filename;
    QPushButton *pushbutton_add_filename;
    QLineEdit   *lineedit_add_filename;
    QLabel      *label_message_filename;

    QLineEdit   *lineedit_edit_filename;
    QDialog     *dialog_edit_filename;
    QPushButton *pushbutton_add_edited_filename;
    QLabel      *label_message_edit_filename;


    void prepare_category_display();
    void prepare_filename_display();

    QLineEdit *new_filename_LineEdit;
    QLineEdit *new_category_LineEdit;

    QString destination_db_path;

    QString category_to_update_str;
    QString filename_to_update_str;

    void prepare_edit_category_dialog();
    void prepare_edit_filename_dialog();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif
