#ifndef FILE_EXTENSION_WIDGET_H
#define FILE_EXTENSION_WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QtGui>
#include <QtCore>
#include "add_ext_category.h"
#include "new_file_signature.h"
#include "edit_category_dialog.h"
#include "edit_signature_dialog.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "message_box/message_dialog.h"
                                   
#include <QLineEdit>
#include "recon_generic_functions/recon_helper_standard.h"      



namespace Ui {
class file_signature_widget;
}

class file_signature_widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit file_signature_widget(QWidget *parent = 0);
    ~file_signature_widget();

    void pub_initialise_signature_details();

    
    void pub_set_ui_awake_status(bool status);
    void pub_set_caller_type(QString caller_type);
private slots:
    void on_pushButton_category_add_clicked();
    void slot_category_added();
    void on_pushButton_signature_add_clicked();

    void on_tableWidget_signature_category_cellClicked(int row, int column);

    void on_pushButton_category_remove_clicked();

    void slot_edit_category(QString cat);

    void on_pushButton_signature_remove_clicked();

    void slot_signature_added();

    void slot_edit_signature(QString label, QString signature , QString offset);

    void on_pushButton_signature_edit_clicked();

    void on_tableWidget_signature_list_cellClicked(int row, int column);

    void on_pushButton_signature_clipboard_clicked();

    void slot_new_category_line_edit_return_pressed();


private:
    Ui::file_signature_widget *ui;
    add_ext_category *add_ext_category_popup;
    new_file_signature *new_file_signature_popup;

    edit_category_dialog *edit_category_dialog_popup;

    edit_signature_dialog *edit_signature_dialog_popup;


    void create_category_list();

    QStringList list_signature_category;
    QStringList list_signature;
    message_dialog *message_dialog_object;

    QString destination_db_file;
    QLineEdit *new_category_LineEdit;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // FILE_EXTENSION_WIDGET_H
