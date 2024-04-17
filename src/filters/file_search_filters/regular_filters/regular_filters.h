#ifndef REGULAR_FILTERS_H
#define REGULAR_FILTERS_H

#include <QDialog>
#include <QtCore>
#include "header_identical_1.h"
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QDateEdit>
#include <header_gui_settings.h>
#include "message_box/message_dialog.h"
#include <QRadioButton>
#include "header_identical_2.h"

namespace Ui {
class regular_filters;
}

class regular_filters : public QDialog
{
    Q_OBJECT

public:
    explicit regular_filters(QWidget *parent = 0);
    ~regular_filters();

    void create_ui_full();
    void reset_all_fields_of_ui();
    void set_parent_dir_path_for_extraction(QString path);
    void create_ui_for_global_filters_search();
    struct_global_selected_regular_filters_info pub_collect_all_selected_fields_info();
private slots:
    void slot_checkbox_create_date_clicked(bool checked);
    void slot_checkbox_last_modified_date_clicked(bool checked);
    void slot_checkbox_last_read_date_clicked(bool checked);
    void slot_pushbutton_search_clicked();
    void slot_comboBox_create_date_within_index_changed(QString index_str);
    void slot_comboBox_last_read_within_index_changed(QString index_str);
    void slot_comboBox_last_modified_within_index_chnaged(QString index_str);
    void slot_combobox_file_size_type_index_changed(QString value);
    void slot_pushbutton_cancel_clicked(bool bool_clicked);
signals:
    void signal_pushbutton_search_clicked(QString file_name, QString file_size_str, QString created_date_str, QString added_date_str, QString last_modified_date_str, QString last_read_date_str, QString file_search_label_name, bool is_radio_or_clicked, QString parent_dir_path);

private:
    Ui::regular_filters *ui;

    QVBoxLayout *vbox_layout_main;
    QVBoxLayout *vbox_layout_file_name_and_size, *vboxlayout_all_dates, *vboxLayout_file_search_option, *vboxLayout_search_and_cancel;
    QVBoxLayout *vbox_mode_selection_radio_button;

    /// File Name
    QLineEdit *lineEdit_file_name;
    QLabel *label_file_name;
    QComboBox *combobox_file_name_filters;

    /// File Size
    QLabel *label_file_size, *label_file_size_between_to;
    QLineEdit *lineEdit_file_size_1, *lineEdit_file_size_2;
    QComboBox *combobox_file_size_sufix, *combobox_file_size_type;

    /// Create Date
    QLabel *label_create_between_to, *label_extra_create_1, *label_extra_create_2, *label_extra_create_3;
    QLineEdit *lineEdit_create;
    QDateEdit *date_edit_create_1, *date_edit_create_2;
    QComboBox *comboBox_create_days, *comboBox_create_within;
    QCheckBox *checkbox_create_date;

    /// Last Modified Date
    QLabel *label_last_modified_between_to, *label_extra_modified_1, *label_extra_modified_2, *label_extra_modified_3;
    QLineEdit *lineEdit_last_modified;
    QDateEdit *date_edit_last_modified_1, *date_edit_last_modified_2;
    QComboBox *comboBox_last_modified_days, *comboBox_last_modified_within;
    QCheckBox *checkbox_last_modified_date;

    /// Last Read Date
    QLabel *label_last_read_between_to, *label_extra_read_1, *label_extra_read_2, *label_extra_read_3;
    QLineEdit *lineEdit_last_read;
    QDateEdit *date_edit_last_read_1, *date_edit_last_read_2;
    QComboBox *comboBox_last_read_days, *comboBox_last_read_within;
    QCheckBox *checkbox_last_read_date;

    QLineEdit *lineEdit_file_search;
    QLabel *label_file_search;
    QPushButton *pushbutton_search, *pushbutton_cancel;
    QRadioButton *radio_button_logical_or, *radio_button_logical_and;

    void set_ui_for_file_name();
    void set_ui_for_file_size();
    void set_ui_for_all_dates();
    void set_ui_for_create_date();
    void set_ui_for_last_read_date();
    void set_ui_for_last_modified_date();
    void set_ui_for_search_option();
    void all_ui_connections();
    QString make_search_string_for_creat_date_fields();
    QString make_search_string_for_last_modified_date_fields();
    QString make_search_string_for_last_read_date_fields();

    message_dialog *message_box_obj;
    void set_ui_mode_selection_radio_button();
    QString make_search_string_for_file_size_fields();

    QString parent_dir_path;
    void set_ui_for_search_and_cancel();
};

#endif
