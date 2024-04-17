#ifndef FILE_SEARCH_INTERFACE_H
#define FILE_SEARCH_INTERFACE_H

#include <QDialog>
#include <QtCore>
#include "filters/file_search_filters/regular_filters/regular_filters.h"
#include "filters/file_search_filters/spotlight_filters/spotlight_filters.h"
#include "filters/file_search_filters/apple_metadata_filters/apple_metadata_filters.h"
#include <QGroupBox>
#include <QtGui>
#include <QStackedWidget>
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class file_search_interface;
}

class file_search_interface : public QDialog
{
    Q_OBJECT

public:
    explicit file_search_interface(QWidget *parent = 0);
    ~file_search_interface();

    void pub_create_ui();
    void pub_set_essentials();
    void pub_set_parent_dir_path(QString parent_dir_path);
    void pub_set_bool_run_full_mode(bool bool_mode);
    void pub_set_file_system_source_count_name(QString source_count_name);
    void pub_hide_apple_filters_for_non_mac_source();
    void pub_set_file_search_stack_widget_index(enum_stack_widget_file_search_filters index);



private slots:
    void slot_pushbutton_search_clicked(bool checked);
    void slot_pushbutton_cancel_clicked(bool checked);

signals:
    void signal_start_file_search_thread(struct_global_selected_regular_filters_info struct_info, QString mdfind_condition, QString search_label_name, bool bool_run_in_full_mode, QString dir_path_to_search, QString file_system_source_count_name);

private:
    Ui::file_search_interface *ui;


    regular_filters *regular_filters_obj;
    spotlight_filters *spotlight_filters_obj;
    apple_metadata_filters *apple_metadata_filters_obj;
    QGroupBox *groupbox_regular_search, *groupbox_spotlight_search,*groupbox_apple_metadata_search;
    void set_ui_for_regular_file_search();
    void set_ui_for_spotlight_search();
    void setup_ui_for_any_all_filters();
    void set_ui_for_search_and_cancel();

    QPushButton *pushbutton_search, *pushbutton_cancel;
    QLineEdit *line_edit_search_label;
    QLabel *label_file_search_name;
    QHBoxLayout *hlayout_search_and_cancel;
    void set_all_connections();

    message_dialog *message_box_obj;

    QString parent_dir_path_for_search;
    bool bool_run_in_full_mode;
    QString file_system_source_count_name;
    void set_linedit_search_label_placeholder_text();
    recon_helper_standard *recon_helper_standard_obj;

    QRadioButton *radioButton_any_filter;
    QRadioButton *radioButton_all_filters;
    QHBoxLayout *hbox_any_all_filters_buttons;
    QGroupBox *group_box_any_all_filter_buttons;


    void set_ui_for_apple_meatdata_search();

    QStackedWidget *stack_widget_filters;

    int file_search_filter_type;

};

#endif // FILE_SEARCH_INTERFACE_H
