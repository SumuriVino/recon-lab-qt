#ifndef SPOTLIGHT_FILTERS_H
#define SPOTLIGHT_FILTERS_H

#include <QWidget>
#include "header_identical_3.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_static_functions.h"
#include "apple_attributes/apple_attributes.h"
#include "header_gui_settings.h"
#include "QLabel"

class spotlight_filters : public QWidget
{
    Q_OBJECT
public:
    explicit spotlight_filters(QWidget *parent = nullptr);

    void pub_create_ui();
    void pub_set_essentials();
    QString pub_collect_all_spotlight_search_filters_information();
private slots:
    void slot_on_combobox_search_type_current_index_changed(int index);
    void slot_on_pushbutton_delete_clicked();
    void slot_on_pushbutton_add_clicked();
    void slot_apple_attributes_work_done(bool bool_ok_or_cancel);
    void slot_on_combobox_ext_attr_kind_current_index_changed(QString current_str);
    void slot_on_combobox_ext_attr_dates_current_index_changed(QString current_str);
private:

    enum enum_File_System_command_type
    {
        enum_File_System_CommandType_TEXT= 0,
        enum_File_System_CommandType_TIMESTAMP,
        enum_File_System_CommandType_SIZE,
        enum_File_System_CommandType_DURATION
    };

    enum enum_file_systemem_Widget_Type
    {
        enum_File_System_Widget_Type_FILESYSTEM_NAME = 0,
        enum_File_System_Widget_Type_FILESYSTEM_OPTIONS,
        enum_File_System_Widget_Type_FILESYSTEM_RANGE,
        enum_File_System_Widget_Type_FILESYSTEM_DURATION,
        enum_File_System_Widget_Type_FILESYSTEM_SIZE,
        enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LINEEDIT,
        enum_File_System_Widget_Type_FILESYSTEM_KIND,
        enum_File_System_Widget_Type_FILESYSTEM_CONTENT,
        enum_File_System_Widget_Type_FILESYSTEM_DATE,
        enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LABEL_N_LINEEDIT,
        enum_File_System_Widget_Type_FILESYSTEM_SINGLE_SYSTEM_COMBOBX,
        enum_File_System_Widget_Type_FILESYSTEM_SINGLE_VISIBLE_COMBOBX,
        enum_File_System_Widget_Type_FILESYSTEM_FILE_LABEL
    };

    enum enum_File_System_fixed_checks
    {
        enum_File_System_Spacer_Width_int = 700,
        enum_File_System_Spacer_Height_int = 10,
        enum_File_System_Spacing_int = 12,
        enum_File_System_Apple_Attribute_Widget_Type_MAX_Value_int = 500,
        enum_File_System_MAX_Index_Of_Widget_In_Layout_int = 500,
        enum_File_System_MAX_Filter_Count_int = 7
    };


    ///List names
    QStringList list_search_types;
    QStringList list_search_type_names_search_items;
    QStringList list_search_type_kind_search_items;
    QStringList list_search_type_kind_images_search_items;
    QStringList list_search_type_kind_music_search_items;
    QStringList list_search_type_kind_textfiles_search_items;
    QStringList list_search_type_date_search_items;
    QStringList list_search_type_date_days_search_items;
    QStringList list_search_type_options_search_items;
    QStringList list_search_type_compare_search_items;
    QStringList list_search_type_size_filters_items;
    QStringList list_search_type_system_search_items;
    QStringList list_search_type_visible_search_items;

    QVBoxLayout *vlayout_main, *vlayout_filters_and_pushbutton_back_next_combobox, *vlayout_filters;
    int search_count;
    int last_save_index;
    int font_size_of_spotlight_search_int;
    QString stylesheet_pushbutton_str;
    apple_attributes *apple_attributes_obj;

    QString db_path_case_confg;

    void on_appl_attribute_ok_button_click();
    void on_appl_attribute_cancel_button_click();
    int sub_set_all_index();
    void set_options_interface(int indexofitem, QString current_text);
    void set_options_interface_add_filter(int indexofitem, QString current_str);
    void set_file_labels_interface(int indexofitem, QString current_text);
    void set_file_labels_interface_add_filter(int indexofitem, QString current_str);
    void set_range_interface(int indexofitem, QString current_text);
    void set_range_interface_add_filter(int indexofitem, QString current_str);
    void set_lineedit_interface(int indexofitem, QString current_text);
    void set_lineedit_interface_add_filter(int indexofitem, QString current_str);
    void set_label_with_lineedit_interface(int indexofitem, QString current_text);
    void set_label_with_lineedit_interface_add_filter(int indexofitem, QString current_str);
    void set_size_interface(int indexofitem, QString current_text);
    void set_size_interface_add_filter(int indexofitem, QString current_str);
    void set_duration_interface(int indexofitem, QString current_text);
    void set_duration_interface_add_filter(int indexofitem, QString current_str);
    void set_single_combobox_interface(int indexofitem, QString current_text);
    void set_single_combobox_interface_add_filter(int indexofitem, QString current_str);
    void set_single_visible_combobox_interface(int indexofitem, QString current_text);
    void set_single_visible_combobox_interface_add_filter(int indexofitem, QString current_str);
    void set_names_interface(int indexofitem, QString current_text);
    void set_names_interface_add_filter(int indexofitem, QString current_str);
    void set_contents_interface(int indexofitem, QString current_text);
    void set_contents_interface_add_filter(int indexofitem, QString current_str);
    void set_kind_interface(int indexofitem, QString current_text);
    void set_kind_interface_add_filter(int indexofitem, QString current_str);
    void set_date_interface(int indexofitem, QString current_text);
    void set_date_interface_add_filter(int indexofitem, QString current_str);
    void set_kind_images_filter(int indexofitem, QString current_str, QStringList listof_kind_data);
    void set_kind_others_filter(int indexofitem, QString current_str);
    void set_kind_interface_all_filter(int indexofitem, QString current_str);
    void set_dates_interface_with_display_format(int indexofitem, QString current_str);
    void set_dates_interface_others_filter(int indexofitem, QString current_str);
    void set_date_interface_within_filter(int indexofitem, QString current_str);
    QStringList get_main_list_from_appl_attribute_database();
    int get_main_combobox_index(QLayoutItem *item_combobox);
    QString make_strings_for_mdfind_search_condition(QStringList list);
    QString get_mdkind_condition_str_for_widget_type_name(QStringList sub_name_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_options(QStringList sub_name_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_range(QStringList sub_name_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_content(QStringList sub_name_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_kind(QStringList sub_name_list);
    QString get_mdkind_condition_str_for_widget_type_date(QStringList sub_name_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_duration(QStringList sub_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_size(QStringList sub_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_simple_lineedit(QStringList sub_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_simple_label_and_lineedit(QStringList sub_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_single_system_combobox(QStringList sub_list, QString command);
    QString get_mdkind_condition_str_for_widget_type_single_visible_combobox(QStringList sub_list, QString command);
};

#endif
