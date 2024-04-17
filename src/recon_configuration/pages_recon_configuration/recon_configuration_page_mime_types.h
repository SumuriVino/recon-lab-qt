#ifndef RECON_CONFIGURATION_PAGE_MIME_TYPES_H
#define RECON_CONFIGURATION_PAGE_MIME_TYPES_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_configuration_page_mime_types;
}

class recon_configuration_page_mime_types : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_mime_types(QWidget *parent = 0);
    ~recon_configuration_page_mime_types();

    void pub_set_essentials();
private slots:
    void on_pushButton_add_category_clicked();
    void on_pushButton_remove_category_clicked();
    void on_pushButton_add_mime_type_clicked();
    void on_pushButton_remove_mime_type_clicked();
    void on_tableWidget_category_cellClicked(int row, int column);

    void slot_pushbutton_dialog_add_category_clicked();
    void slot_lineedit_add_category_text_changed(QString text);
    void slot_category_cancel_button_clicked();
    void slot_lineedit_add_extension_text_changed(QString text);
    void slot_pushbutton_dialog_add_mime_type_clicked();
    void slot_extension_cancel_button_clicked();

    void slot_new_mime_type_LineEdit_pressed();
    void slot_new_category_line_edit_return_pressed();

private:
    Ui::recon_configuration_page_mime_types *ui;

    QString destination_db_file;

    QFont font_s;

    /// Add Catagory
    QDialog     *dialog_add_category;
    QPushButton *pushbutton_add_category;
    QLineEdit   *lineedit_add_category;
    QLabel      *label_message_category;

    /// Add Extension
    QDialog     *dialog_add_mime_type;
    QPushButton *pushbutton_add_extension;
    QLineEdit   *lineedit_add_mime_type;
    QLabel      *label_message_extension;

    void prepare_mime_category_display();
    void prepare_mime_type_display();
    void prepare_category_display_dialog();
    void prepare_extension_display_dialog();

    QLineEdit *new_mime_type_LineEdit;

    QLineEdit *new_category_LineEdit;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // RECON_CONFIGURATION_PAGE_MIME_TYPES_H
