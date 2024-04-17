#ifndef RECON_CONFIGURATION_PAGE_POINT_HASHSET_H
#define RECON_CONFIGURATION_PAGE_POINT_HASHSET_H

#include <QWidget>
#include <QLineEdit>
#include "existing_hash_column_selection.h"
                                   
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_configuration_page_point_hashset;
}

class recon_configuration_page_point_hashset : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_point_hashset(QWidget *parent = 0);
    ~recon_configuration_page_point_hashset();

    void pub_set_caller_type(QString caller_type);
private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_remove_clicked();
    void slot_pushbutton_save_clicked(QString saved_data_str);
    void slot_pushbutton_cancel_clicked();
    void on_tableWidget_hashset_cellClicked(int row, int column);
    void on_pushButton_create_clicked();
    void slot_hashset_dialog_save_clicked();
    void slot_hashset_dialog_cancel_clicked();
    void slot_lineedit_hashset_md5_text_changed(QString text_str);
    void slot_hashset_dialog_create_new_clicked();
    void slot_hashset_dialog_add_existing_clicked();
    void slot_lineedit_hashset_label_text_changed(QString text_str);
    void slot_create_vic_hashset_clicked();
    void slot_cancel_vic_hashset_clicked();
    void slot_selected_vic_supported_hashset_file(QString filepath);

    void slot_selected_hash_database_file(QString filepath);

private:
    Ui::recon_configuration_page_point_hashset *ui;

    struct struct_vic_parsed_hashes
    {
        QString vic_version;

        QString hash_md5;
        QString hash_sha1;
        QString media_id;
        QString media_size;
        QString name;
        QString odata_id;
    };
    QList <struct_vic_parsed_hashes> hashes_struct_vic_parsed_list;

    existing_hash_column_selection *existing_hash_column_selection_obj;

    QString config_db_file;
    QString caller_type_str;

    QString config_dir_path;

    QDialog *dialog_create_new_hashset;
    QPushButton *pushbutton_create_hashset;
    QLineEdit *lineedit_hashset_md5_hash;
    QLineEdit *lineedit_hashset_label;

    QDialog *dialog_add_hashset;
    QPushButton *pushbutton_create_new_hashset;
    QPushButton *pushbutton_existing_hashset;



    void prepare_table_display();
    void create_dialog_for_create_new_hashset();
    void create_dialog_for_add_hashset();

    QDialog *dialog_add_vic_supported_hashset;
    QPushButton *pushbutton_create_vic_hashset;
    QPushButton *pushbutton_cancel_vic_hashset;
    QLineEdit *lineedit_selected_vic_hashest;
    QLineEdit *lineedit_hashest_name;


    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    void create_dialog_for_import_vic_hashset();
    void extract_and_save_vic_supported_hashset_file(QString filepath);
    recon_helper_standard *recon_helper_standard_obj;

    void parse_unknown_vic_format_file(QString filepath);
    QString get_sha1_hash_for_unknown_vic_format(QByteArray &file_data_arr, int md5_end_index);
};

#endif //
