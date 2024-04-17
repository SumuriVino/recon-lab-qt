#ifndef RECON_CONFIGURATION_PAGE_INDEX_FILES_H
#define RECON_CONFIGURATION_PAGE_INDEX_FILES_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_confuguration_page_index_files;
}

class recon_configuration_page_index_files : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_index_files(QWidget *parent = 0);
    ~recon_configuration_page_index_files();

    void pub_enable_index_files_display(bool status);
    void pub_update_value_in_narad_muni();
    void pub_set_caller_type(QString caller_type);
    void pub_set_config_called_from_toolbar(bool status);
private slots:
    void on_pushButton_add_whitelist_category_clicked();
    void on_pushButton_remove_whitelist_category_clicked();
    void on_pushButton_remove_whitelist_extension_clicked();
    void on_pushButton_add_blacklist_category_clicked();
    void on_pushButton_remove_blacklist_category_clicked();
    void on_pushButton_add_blacklist_extension_clicked();
    void on_pushButton_remove_blacklist_extension_clicked();
    void on_radioButton_whitelist_clicked(bool checked);
    void on_radioButton_blacklist_clicked(bool checked);
    void on_pushButton_add_whitelist_extension_clicked();

    void slot_whitelist_new_category_line_edit_return_pressed();
    void slot_whitelist_and_blacklist_new_extension_line_edit_return_pressed();
    void on_tableWidget_whitelist_category_cellClicked(int row, int column);

    void on_tableWidget_blacklist_category_cellClicked(int row, int column);

    void on_lineEdit_size_returnPressed();

    void on_pushButton_filesize_save_clicked();

    void on_pushButton_clipboard_whitelist_extension_clicked();

    void on_pushButton_clipboard_copy_blacklist_extension_clicked();

    void on_radioButton_all_files_clicked(bool checked);

private:
    Ui::recon_confuguration_page_index_files *ui;


    QString destination_db_file;
    QString db_table_category_name;
    QString db_table_extension_name;

    QLineEdit *lineedit_new_whitelist_and_blacklist_category;
    QLineEdit *lineedit_new_whitelist_and_blacklist_extension;

    int tabwidget_current_index;

    QString caller_type;

    void set_table_name_on_stackwidget_index_change(int index);
    void prepare_whitelist_and_blacklist_category_tablewidget_display();
    void prepare_whitelist_and_blacklist_extension_tablewidget_display();
    void clear_and_disable_whitelist_and_blacklist_for_all_files(bool status);
    void insert_selected_text_index_filter_status(int value);
    void set_fields_for_default_selection();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // RECON_CONFIGURATION_PAGE_INDEX_FILES_H
