#ifndef RECON_CONFIGURATION_PAGE_FILE_SIZE_H
#define RECON_CONFIGURATION_PAGE_FILE_SIZE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      


namespace Ui {
class recon_configuration_page_file_size;
}

class recon_configuration_page_file_size : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_file_size(QWidget *parent = 0);
    ~recon_configuration_page_file_size();

    void pub_set_essentials();
private slots:
    void on_tableWidget_filter_label_cellClicked(int row, int column);


    void on_pushButton_add_label_clicked();

    void on_pushButton_remove_label_clicked();

    void slot_pushbutton_dialog_add_category_clicked();
    void slot_lineedit_add_category_text_changed(QString text);
    void slot_category_cancel_button_clicked();
private:
    Ui::recon_configuration_page_file_size *ui;

    QString destination_db_file;

    QFont font_s;

    /// Add Catagory
    QDialog     *dialog_add_filter;
    QPushButton *pushbutton_add_filters;
    QLineEdit   *lineedit_add_filters;

    QLineEdit   *lineedit_greater_than_size;
    QLineEdit   *lineedit_less_than_size;
    QLineEdit   *lineedit_beteen_start_size;
    QLineEdit   *lineedit_beteen_end_size;

    QRadioButton *radiobutton_greater_than;
    QRadioButton *radiobutton_less_than;
    QRadioButton *radiobutton_between;

    QComboBox   *combobox_start_size_unit;
    QComboBox   *combobox_end_size_unit;
    QComboBox   *combobox_between_start_size_unit;
    QComboBox   *combobox_between_end_size_unit;

    QLabel      *label_message_filters;

    void prepare_file_size_filters_display();
    void prepare_file_size_filters_details_display();
    void prepare_add_filters_display_dialog();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // RECON_CONFIGURATION_PAGE_FILE_SIZE_H
