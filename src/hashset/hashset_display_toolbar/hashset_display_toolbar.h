#ifndef HASHSET_DISPLAY_TOOLBAR_H
#define HASHSET_DISPLAY_TOOLBAR_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include "narad_muni/narad_muni.h"
#include "recon_generic_functions/recon_helper_singular.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_configuration/pages_recon_configuration/import_hashset/recon_configuration_page_point_hashset.h"

#include "thread_parse_nsrl_hashes.h"

namespace Ui {
class hashset_display_toolbar;
}

class hashset_display_toolbar : public QDialog
{
    Q_OBJECT

public:
    explicit hashset_display_toolbar(QWidget *parent = 0);
    ~hashset_display_toolbar();

    void pub_set_hashset_toolbar_essentials();

private slots:
    void on_pushButton_remove_clicked();
    void on_pushButton_create_clicked();
    void on_pushButton_apply_clicked();
    void on_pushButton_cancel_clicked();
    void on_tableWidget_hashset_cellChanged(int row, int column);
    void slot_action_add_db_clicked(bool status);
    void slot_action_add_VIC_clicked(bool status);
    void slot_lineedit_hashset_label_text_changed(QString text_str);
    void slot_hashset_dialog_create_sqlite_clicked();
    void slot_hashset_dialog_cancel_sqlite_clicked();
    void slot_create_vic_hashset_clicked();
    void slot_cancel_vic_hashset_clicked();
    void slot_pushbutton_existing_sqlite_hash_save_clicked(QString saved_data_str);
    void slot_pushbutton_existing_sqlite_hash_cancel_clicked();
    void slot_existing_hash_selected_vic_supported_hashset_file(QString filepath);
    void slot_existing_hash_selected_hash_database_file(QString filepath);
    void slot_checkbox_show_files_clicked(bool status);
    void slot_checkbox_hide_files_clicked(bool status);

    void slot_action_add_NSRL_clicked(bool status);
    void slot_create_nsrl_hashset_clicked();
    void slot_cancel_nsrl_hashset_clicked();
    void slot_thread_parse_nsrl_finished();
    void slot_hide_loading_display_dialog_box(bool status);
    void slot_nsrl_parse_status(qint64 records, int prog_status);

    void on_tableWidget_hashset_cellClicked(int row, int column);

    void slot_action_add_CSV_clicked(bool status);
    void create_dialog_for_import_csv_hashset();
    void slot_create_csv_hashset_clicked();
    void slot_cancel_csv_hashset_clicked();
signals:
    void signal_hashset_toolbar_apply_clicked(QStringList avlble_hashset_list);


private:
    Ui::hashset_display_toolbar *ui;

    recon_helper_standard *recon_helper_standard_obj;

    struct struct_vic_parsed_hashes
    {
        QString vic_version;

        QString hash_md5;
        QString hash_sha1;
        QString media_id;
        QString media_size;
        QString name;
        QString odata_id;
        QString griffeye_tags;
    };
    QList <struct_vic_parsed_hashes> hashes_struct_vic_parsed_list;


    enum enum_hashset_display
    {
        enum_hashset_INT = 0,
        enum_hashset_name,
        enum_hashset_db_name,
        enum_hashset_show_files,
        enum_hashset_hide_files,
        enum_hashset_md5_column,
        enum_hashset_sha1_column
    };

    QString hashes_index_db_path;
    QString hashset_dir_path;

    QDialog *dialog_create_new_hashset;
    QPushButton *pushbutton_create_hashset;
    QLineEdit *lineedit_hashset_md5_hash;
    QLineEdit *lineedit_hashset_label;


    QDialog *dialog_add_vic_supported_hashset;
    QPushButton *pushbutton_create_vic_hashset;
    QPushButton *pushbutton_cancel_vic_hashset;
    QLineEdit *lineedit_selected_vic_hashest;
    QLineEdit *lineedit_vic_hashest_name;

    QDialog *dialog_add_nsrl_supported_hashset;
    QPushButton *pushbutton_create_nsrl_hashset;
    QPushButton *pushbutton_cancel_nsrl_hashset;
    QLineEdit *lineedit_selected_nsrl_hashest;
    QLineEdit *lineedit_nsrl_hashest_name;

    QDialog *dialog_add_csv_supported_hashset;
    QPushButton *pushbutton_create_csv_hashset;
    QPushButton *pushbutton_cancel_csv_hashset;
    QLineEdit *lineedit_selected_csv_hashest;
    QLineEdit *lineedit_csv_hashest_name;


    QFileDialog file_dialog_obj;

    thread_parse_nsrl_hashes *thread_parse_nsrl_hashes_obj;
    display_loading_progress_bar *nsrl_parse_prog_obj;


    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    existing_hash_column_selection *existing_hash_column_selection_obj;

    void create_dialog_for_create_new_sqlite_hashset();
    void create_dialog_for_import_vic_hashset();
    void extract_and_save_vic_supported_hashset_file(QString filepath);
    void prepare_table_display_for_saved_hashsets();
    void create_dialog_for_import_nsrl_hashset();
    QString create_nsrl_destination_db();

    bool bool_checked_from_table = false;
    QString create_csv_destination_db();
    void parse_unknown_vic_format_file(QString filepath);
    QString get_sha1_hash_for_unknown_vic_format(QByteArray &file_data_arr, int md5_end_index);
};

#endif // HASHSET_DISPLAY_TOOLBAR_H
